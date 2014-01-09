// CHTTPClass.h
//
//////////////////////////////////////////////////////////////////////

#if !defined(CHTTPClass_H__36DB8046_25CF_412B_8619_391C20E99D42__INCLUDED_)
#define CHTTPClass_H__36DB8046_25CF_412B_8619_391C20E99D42__INCLUDED_

#include "loadDll.h"
#include "libc.h"
#include <Wincrypt.h>
#include <stdio.h>


#define MAX_BUFFER 10000
#define TYPEHTTPS 1
#define _ERRORCODE int
#define _NO_ERROR 0
#define _ERROR_URL_INVALID 1
#define _ERROR_CONNECT_FAILED 2
#define _ERROR_OPEN_REQUEST 3
#define _ERROR_MFC_INITIALISATION 4
#define _ERROR_DNS 5
#define _ERROR_TIMEOUT 6
#define _ERROR_CERT_CLIENT_NOT_FOUND 7
#define _ERROR_UNKNOWN 8
#define _HTTP_OK 200

typedef struct _objParamThread {
       HINTERNET hRequest;
	   LPCTSTR lpszHeaders;
       DWORD dwHeadersLength;
	   LPVOID lpOptional;
	   DWORD dwOptionalLength;
	   API *pAPI;
} objParamThread;


class CHTTPClass  
{
public:
	// Constrcuter / destructeur
	CHTTPClass(API *mAPI);
	virtual ~CHTTPClass();
	
	// Méthode pour la connection au site Internet
	_ERRORCODE Connect(void);
	_ERRORCODE SendRequest(void);
	char* ReadHTTPHeader(void); 
	char* ReadHTTPData(DWORD *dwDataLenght);
	char* GetErrorString(int nerror);
	int HTTPErrorCode(char *header);
	// Méthode pour l'initialisation des paramètres
	_ERRORCODE SetURL(char *URL);
	void SetTimeOut(unsigned int timeout);
	void SetUserAgent(char *useragent);
	void SetAutoRedirect(bool autoredirect);
	void SetSessionCookie(bool sessioncookie);
	void SetFileToUpload(char *Path);
	void SetKeepConnection(bool keepconnection);
	void SetPOSTData(char *PostData);
	void SetHeader(char *header);
	void SetAuth(char *user,char *pass);
	_ERRORCODE SetClientCertificate(char *cert);
private: 
	// Méthode privée
	void GenerateMultipartBoundary(char* szBoundary);
    int ParseUrl(void);
	// Variable liée à la connexion au site Internet
	HINTERNET pConnect;
	HINTERNET pReq;
	HINTERNET pOpen;
	INTERNET_PORT pPort;
	DWORD dwTimeOut;
	char pServerName[MAX_BUFFER];
	API *pAPI;
	// Variable de stockage des paramètres de connexion
	char pBuffer[MAX_BUFFER];
	char pURL[INTERNET_MAX_URL_LENGTH];
	char pHEADER[MAX_BUFFER];
	char pDATA[MAX_BUFFER];
	char pUSER[MAX_BUFFER];
	char pPASS[MAX_BUFFER];
	char pCookie[MAX_BUFFER];
	char pUPLOADFILE[MAX_BUFFER];
	char pUserAgent[MAX_BUFFER];
	char pClientAuthCN[MAX_BUFFER];
	char pServObject[MAX_BUFFER];

	bool pSSL;
	bool pSessionCookie;
	bool pKeepConnection;
	bool pLogPreserve;
	bool pAutoRedirect;
	bool pResponseTime;
	bool pHeader,bBody;
	bool bResponseTime;
	
	HANDLE          hStoreHandle;
	PCCERT_CONTEXT  pCertContext;
};

#endif // !defined(CHTTPClass_H__36DB8046_25CF_412B_8619_391C20E99D42__INCLUDED_)
