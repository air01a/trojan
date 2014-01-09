#include "../include/CHTTPClass.h"

using namespace std;

char *tError[8]={"Aucune erreur détectée","L'URL transmis est invalide","Erreur lors de l'ouverture de la session HTTP","Erreur lors de l'envoi de la requête","Erreur à l'initialisation des API MFCs","Erreur de résolution DNS","Time Out sur la connexion internet","Erreur inconnue"};

DWORD WINAPI WorkerProcess(IN LPVOID vThreadParam)
{
	   DWORD dwError=0;

       objParamThread *pThreadParam;
       pThreadParam = (objParamThread*)vThreadParam;
	   dwError=pThreadParam->pAPI->pHttpSendRequest(pThreadParam->hRequest,pThreadParam->lpszHeaders,pThreadParam->dwHeadersLength,pThreadParam->lpOptional,pThreadParam->dwOptionalLength);
	   if (!dwError)
		   dwError=GetLastError();
	   return dwError;
}

DWORD threadHttpSendRequest(API *pAPI,HINTERNET pRequest,LPCTSTR lpszHeaders,DWORD dwHeadersLength,LPVOID lpOptional,DWORD dwOptionalLength, DWORD dwTimeOut)
{
	HANDLE   hThread;
    DWORD    dwThreadID;
	objParamThread stThreadParam;
	DWORD dwReturnCode;
	stThreadParam.hRequest=pRequest;
	stThreadParam.lpszHeaders=lpszHeaders;
    stThreadParam.dwHeadersLength=dwHeadersLength;
	stThreadParam.lpOptional=lpOptional;
	stThreadParam.dwOptionalLength=dwOptionalLength;
	stThreadParam.pAPI=pAPI;
	hThread = CreateThread(
                     NULL,            // Pointer to thread security attributes
                     0,               // Initial thread stack size, in bytes
                     WorkerProcess,  // Pointer to thread function
                     &stThreadParam,     // The argument for the new thread
                     0,               // Creation flags
                     &dwThreadID      // Pointer to returned thread identifier
                 );
	     

    if (WaitForSingleObject ( hThread, dwTimeOut ) == WAIT_TIMEOUT )
    {
		   pAPI->pInternetCloseHandle (pRequest);
		   CloseHandle (hThread);
           return _ERROR_TIMEOUT;
    }

	GetExitCodeThread( hThread, &dwReturnCode);
    return  dwReturnCode;
}


CHTTPClass::CHTTPClass(API *mAPI)
{
    pAPI=mAPI;
	pURL[0]=0;pHEADER[0]=0;pDATA[0]=0;
	pUSER[0]=0;pPASS[0]=0;pCookie[0]=0;pUPLOADFILE[0]=0;
	pSessionCookie=false;pKeepConnection=true;
	pUserAgent[0]=0;
	dwTimeOut=60000;pPort=80;
	strncpy(pUserAgent,"Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 6.0; SLCC1; .NET CLR 2.0.50727; .DOT 1.0)",MAX_BUFFER);
	hStoreHandle=NULL;
	pCertContext=NULL;
}

CHTTPClass::~CHTTPClass()
{
	pAPI->pInternetCloseHandle(pReq);
	pAPI->pInternetCloseHandle(pConnect);
	pAPI->pInternetCloseHandle(pOpen);

	pConnect = NULL;
}

// correspond à une chaine ne devant pas être présente dans
// le fichier uploadé
void CHTTPClass::GenerateMultipartBoundary(char* szBoundary){
	int r0 = rand() & 0xffff;
	int r1 = rand() & 0xffff;
	int r2 = rand() & 0xffff;
	sprintf(szBoundary, "%04X%04X%04X", r0, r1, r2);
}

void CHTTPClass::SetTimeOut(unsigned int timeout)
{
	dwTimeOut=timeout;
}

void CHTTPClass::SetPOSTData(char *PostData)
{
	strncpy(pDATA,PostData,MAX_BUFFER);
	
	char* pDest=0;
	while((pDest=strstr(pDATA,"#CRLF#"))!= NULL)
	{
		*(pDest++)='\n';*(pDest++)='\r';
		while(*(pDest+4)!='\0'){
			*(pDest) = *(pDest+4);pDest++;
		}
		*pDest = '\0';
	}	
}

void CHTTPClass::SetHeader(char *header)
{
	strncpy(pHEADER,header,MAX_BUFFER);
	char* pDest=0;
	while((pDest=strstr(pHEADER,"#CRLF#"))!= NULL)
	{
		*(pDest++)='\n';*(pDest++)='\r';
		while(*(pDest+4)!='\0'){
			*(pDest) = *(pDest+4);pDest++;
		}
		*pDest = '\0';
	}
}


void CHTTPClass::SetAuth(char *user,char *pass)
{
	strncpy(pUSER,user,MAX_BUFFER);
	strncpy(pPASS,pass,MAX_BUFFER);
}

_ERRORCODE CHTTPClass::SetURL(char *URL) 
{
	char tmp[10];
	
	sprintf(tmp,"?id=%i",pAPI->idTroj);
    
	if (inStr(URL,'?'))
       tmp[0]='&';                 
	strncpy(pURL,URL,INTERNET_MAX_URL_LENGTH);
    strncpy(pURL+strlen(pURL),tmp, INTERNET_MAX_URL_LENGTH-strlen(pURL));
	
	return _NO_ERROR;
}
	

void CHTTPClass::SetUserAgent(char *useragent) 
{
	strncpy(pUserAgent,useragent,MAX_BUFFER);
}


void CHTTPClass::SetAutoRedirect(bool autoredirect)
{
	pAutoRedirect=autoredirect;
}

void CHTTPClass::SetSessionCookie(bool sessioncookie)
{
	pSessionCookie=sessioncookie;
}

void CHTTPClass::SetFileToUpload(char *Path)
{
	strncpy(pUPLOADFILE,Path,MAX_BUFFER);
}

void CHTTPClass::SetKeepConnection(bool keepconnection) {

	pKeepConnection=keepconnection;
}

int CHTTPClass::HTTPErrorCode(char *header) {
    char tmp[3];
    if (header==NULL || strncmp(header,"HTTP/1.", 6)!=0)
       return 0;

    strncpy(tmp,header+9,3);
    return atoi(tmp);
}

int CHTTPClass::ParseUrl(void) {
     
    char *ptr, *pBase;
    char tmp[MAX_BUFFER];
    
    ptr=pBase=pURL;
    
    while ((ptr<(pBase+strlen(pURL))) && (*ptr!='/') && (*ptr!=':')) {
       tmp[ptr-pBase]=*ptr;
       ptr++;
    }
    tmp[ptr-pBase]=0;
    pSSL=false;
    if (!strcasecmp("https",tmp))
      pSSL=true;

    ptr+=3;
    if (ptr>pBase+strlen(pURL))
      return 0;
  
    pBase=ptr;
    while ((ptr<(pURL+strlen(pURL))) && (*ptr!='/') && (*ptr!=':')) {
         pServerName[ptr-pBase]=*ptr;
         ptr++;
    }
    pServerName[ptr-pBase]=0;
    
    pPort=0;
    if (*ptr==':') {
      ptr++;pBase=ptr;
      while ((ptr<(pURL+strlen(pURL))) && (*ptr!='/')) {
            tmp[ptr-pBase]=*ptr;ptr++;
      }
      pPort=atoi(tmp);
    }

    if (!pPort) {
       if (pSSL)
          pPort=443;
       else
          pPort=80;     
    }
   tmp[0]=0;pBase=ptr;
   while (ptr<(pURL+strlen(pURL))) {
            pServObject[ptr-pBase]=*ptr;ptr++;
   }
   pServObject[ptr-pBase]=0;
   if (pServObject[0]==0) {
      pServObject[0]='/';pServObject[1]=0;
   }
   return 1;
}

_ERRORCODE  CHTTPClass::Connect(void)
{
	int iError=_NO_ERROR;

	if (!ParseUrl())
	{
			return _ERROR_URL_INVALID;
	}	
                    
	if(pConnect != NULL)
	{
		pAPI->pInternetCloseHandle(pReq);
		pAPI->pInternetCloseHandle(pConnect);
		pAPI->pInternetCloseHandle(pOpen);
		pConnect = NULL;
	}

	if(pConnect == NULL)
	{
		pOpen = pAPI->pInternetOpen(pUserAgent,INTERNET_OPEN_TYPE_PRECONFIG,NULL,NULL,0); //INTERNET_FLAG_RELOAD

		if(!pSSL)
		{
			pConnect = pAPI->pInternetConnect(
							pOpen,       // hInternetSession
							(LPCTSTR)pServerName,      // lpszServerName
							pPort, //INTERNET_DEFAULT_HTTPS_PORT, // nServerPort
							pUSER,        // lpszUsername
							pPASS,        // lpszPassword
							INTERNET_SERVICE_HTTP,   // dwService
							0,//INTERNET_FLAG_SECURE,   // dwFlags
							0);        // dwContext
		} else {
			pConnect = pAPI->pInternetConnect(
							pOpen,       // hInternetSession
							(LPCTSTR)pServerName,      // lpszServerName
							pPort,		//INTERNET_DEFAULT_HTTPS_PORT, 
							pUSER,        // lpszUsername
							pPASS,        // lpszPassword
							INTERNET_SERVICE_HTTP,   // dwService
							INTERNET_FLAG_SECURE|INTERNET_FLAG_IGNORE_CERT_CN_INVALID,   // dwFlags
							0);        // dwContext
		}
		if(pConnect == NULL) {
				//cerr << "Erreur InternetConnect" << endl;return 0;
				iError=_ERROR_CONNECT_FAILED;
	    }
    }
	return iError;
}
		

_ERRORCODE CHTTPClass::SendRequest(void) {
	int dwError;
	char pBoundary[39];
	int  pPostDataSize=0;
	int  pPostBinDataSize=0;char *pBINDATA=NULL; 

	DWORD dwFlags = INTERNET_FLAG_RELOAD|INTERNET_FLAG_NO_CACHE_WRITE ;//
	dwFlags |= (!pAutoRedirect)? INTERNET_FLAG_NO_AUTO_REDIRECT : NULL;
	dwFlags |= (pSessionCookie)? NULL : INTERNET_FLAG_NO_COOKIES;
	dwFlags |= (pSSL)? INTERNET_FLAG_SECURE|INTERNET_FLAG_IGNORE_CERT_CN_INVALID:NULL;//
	dwFlags |= (pKeepConnection)?INTERNET_FLAG_KEEP_CONNECTION:NULL;

	if(strlen(pDATA) == 0 && strlen(pUPLOADFILE)==0 ) {
		pReq = pAPI->pHttpOpenRequest (
					pConnect,
					"GET",
					(LPCTSTR)pServObject,
					HTTP_VERSION,
					"",
					NULL,
					dwFlags,
					0
				);
	} else {
		pReq = pAPI->pHttpOpenRequest (
					pConnect,
					"POST",
					(LPCTSTR)pServObject,
					HTTP_VERSION,
					"",
					NULL,
					dwFlags,
					0
				);
	}

	if(pReq == NULL)
	{
        return _ERROR_OPEN_REQUEST;
		//cerr << "Erreur HttpOpenRequest" << endl;return 0;
	}

	
	if(strlen(pUPLOADFILE)==0){                          
		if(strlen(pDATA) != 0  ) {
			pAPI->pHttpAddRequestHeaders(pReq,"Content-Type: application/x-www-form-urlencoded\n\rAccept: */*\n\rAccept-Language: fr\n\r",-1L,HTTP_ADDREQ_FLAG_COALESCE_WITH_SEMICOLON);
			sprintf(pBuffer,"Length: %i",strlen(pDATA));
			pAPI->pHttpAddRequestHeaders(pReq,pBuffer,-1L,HTTP_ADDREQ_FLAG_COALESCE_WITH_SEMICOLON);
			pPostDataSize = strlen(pDATA);
		}
	} else{			

		char szContentTypeHeader[2*MAX_BUFFER];
		GenerateMultipartBoundary(pBoundary); 
		sprintf(szContentTypeHeader,"Accept: */*\n\rContent-Type: multipart/form-data; boundary=---------------------------%s\n\rAccept-Encoding: gzip, deflate\n\r",pBoundary);
        pAPI->pHttpAddRequestHeaders(pReq,szContentTypeHeader,-1L,HTTP_ADDREQ_FLAG_COALESCE_WITH_SEMICOLON);


		// deux passages seront nécessaires
		// le premier calcul la taille cumulées des données à poster
		// le second les charge en mémoire

		int iMaxData = 1000;
		int iCount=0;
		pPostBinDataSize=0;
		pBINDATA=(char *)calloc(iMaxData+1,sizeof(char)); // taille maximale des transferts
		ZeroMemory(pBINDATA,iMaxData+1);

		// la zone temporaire sera élargie lorsque la taille réelle sera connue

		for(int	iPass = 0;iPass<=1;iPass++){
			if(iPass == 1){
				free(pBINDATA);
				iMaxData = pPostBinDataSize;
				pPostBinDataSize = 0;
				pBINDATA=(char *)calloc(iMaxData+1,sizeof(char)); 
			}
				
			if(strlen(pUPLOADFILE)!=0){
			//
			// traitement des POST de fichiers
			//
					
			// mémorisation de la variable d'origine
		
				char szBakUPLOADFILE[MAX_BUFFER];
				strcpy(szBakUPLOADFILE,pUPLOADFILE);

					// lecture des noms et variables
				strcat(pUPLOADFILE,"&");
				char sep[] = "&";
				char *token;
				token = strtok( pUPLOADFILE, sep );

				// DEBUT PROBLEME
                
                while( token != NULL )
				{
                       
					char* szPos = strchr(token,'=');
					
				
					if(szPos!=NULL){
                                    
				// token contient le nom de la variable et szPos+1 le nom du fichier
						*szPos++='\0';	
						sprintf(pBuffer,"-----------------------------%s\r\nContent-Disposition: form-data; name=\"%s\"; filename=\"%s\"\r\nContent-Type: application/octet-stream\r\n\r\n\0",pBoundary,token,szPos);	
						if(iPass == 1)
							strcat(pBINDATA+pPostBinDataSize,pBuffer);
							
							pPostBinDataSize+=strlen(pBuffer);
							FILE* file = fopen(szPos,"rb");
				          
							if(file == NULL){

								       // sprintf("fichier %s introuvable\r\n",szPos);
								         		free(pDATA);
		                             free(pBINDATA);
                                    return _ERROR_UNKNOWN;
   
							}
							else{

								if(iPass == 1)
									iCount = fread(pBINDATA+pPostBinDataSize, sizeof(char), iMaxData-pPostBinDataSize, file);
								else
									iCount = fread(pBINDATA, sizeof(char), iMaxData, file);
								pPostBinDataSize+=iCount;
								//return ((1==2)?true:false);

								while(iCount != 0 && ((iPass == 1)? (pPostBinDataSize < iMaxData):true)){
									
									if(iPass == 1)
										iCount = fread(pBINDATA+pPostBinDataSize , sizeof(char), iMaxData-pPostBinDataSize, file);
									else
										iCount = fread(pBINDATA, sizeof(char), iMaxData, file);
								
									pPostBinDataSize+=iCount;
									
								}
								fclose(file);
							}
							
							
							if(pPostBinDataSize >= iMaxData && (iPass == 1)){
								printf("La taille cumulée des fichiers à transférer dépasse le maximum prévu de %3.2fMo",(float)(iMaxData/1024/1024));
							}
							
							if(iPass == 1){
								 
                                 strcpy(pBINDATA+pPostBinDataSize,"\r\n\0");
							
                          }
                            pPostBinDataSize+=2;
							
						}else{
                              
                              // FIN PROBLEME
                              
                              
							// le token ne contient que le nom du fichier
							sprintf(pBuffer,"-----------------------------%s\r\nContent-Disposition: form-data; name=\"\"; filename=\"%s\"\r\nContent-Type: application/octet-stream\r\n\r\n\0",pBoundary,token);
							if(iPass == 1)
								strcpy(pBINDATA+pPostBinDataSize,pBuffer);
							
							pPostBinDataSize+=strlen(pBuffer);
								
							FILE* file = fopen(token,"rb");

							if(file == NULL){
								if(iPass == 1)
									printf("fichier %s introuvable\r\n",szPos);
							}else{

								if(iPass == 1)
									iCount = fread(pBINDATA+pPostBinDataSize, sizeof(char), iMaxData-pPostBinDataSize, file);
								else
									iCount = fread(pBINDATA, sizeof(char), iMaxData, file);
								
								pPostBinDataSize+=iCount;
								
	
								while(iCount != 0 && ((iPass == 1)? (pPostBinDataSize < iMaxData):true)){
									if(iPass == 1)
										iCount = fread(pBINDATA+pPostBinDataSize , sizeof(char), iMaxData-pPostBinDataSize, file);
									else
										iCount = fread(pBINDATA , sizeof(char), iMaxData, file);

									pPostBinDataSize+=iCount;
									
								}
								fclose(file);
							}
							
							
							
							if(pPostBinDataSize >= iMaxData && (iPass == 1)){
								printf("La taille cumulée des fichiers à transférer dépasse le maximum prévu de %3.2fMo",(float)(iMaxData/1024/1024));
							}
							
							if(iPass == 1)
								strcpy(pBINDATA+pPostBinDataSize,"\r\n\0");
							
							pPostBinDataSize+=2;
						}
						
						
						
						token = strtok( NULL, sep );
						
					}
					
					strcpy(pUPLOADFILE,szBakUPLOADFILE);
				} //fin strlen(szUPLOADFILE)!=0
		


			if(strlen(pDATA)!=0){
			//
			// traitement des POST de données 
			// en mode binaire
			//
			
			// mémorisation de la variable d'origine
				
			char szBakDATA[MAX_BUFFER];
			strcpy(szBakDATA,pDATA);
			strcat(pDATA,"&");
			char sep[] = "&";
			char *token;
			token = strtok( pDATA, sep );
							
			while( token != NULL )
			{
				char* szPos = strchr(token,'=');
						
				if(szPos!=NULL){
					if (*(szPos+1)=='=') {
						char *szPos2 = strchr(szPos+2,'=');
						
						if (szPos2!=NULL) {
							strncpy(szPos+1,szPos+2,strlen(szPos+2)+1);
							szPos=--szPos2;
							
						} else
							szPos++;
					}		
					
					*szPos++='\0';
					// token = nom de la variable
					// szPos = valeur
					sprintf(pBuffer,"-----------------------------%s\r\nContent-Disposition: form-data; name=\"%s\";\r\nContent-Type: application/octet-stream;\r\n\r\n%s\r\n\0",pBoundary,token,szPos);
							
					if(iPass == 1)
						strcpy(pBINDATA+pPostBinDataSize,pBuffer);
							
					pPostBinDataSize+=strlen(pBuffer);
				}
						
				token = strtok( NULL, sep );
			}

					// restauration de la valeur
			strcpy(pDATA,szBakDATA);				
		}
				
		sprintf(pBuffer,"-----------------------------%s--\r\n\0",pBoundary);
				
		if(iPass == 1)
			strcpy(pBINDATA+pPostBinDataSize,pBuffer);
				
			pPostBinDataSize+=strlen(pBuffer);
			

		}// fin boucle for

	}
	//
	// ajouter les headers
	//

	if(strlen(pHEADER) != 0)
		{
			pAPI->pHttpAddRequestHeaders(pReq,pHEADER,-1L,HTTP_ADDREQ_FLAG_ADD);
		}
	
	if(strlen(pCookie)>0)
	{
		if(!pSSL)
			sprintf(pBuffer,"http://%s",pServerName);
		else
			sprintf(pBuffer,"https://%s",pServerName);

		BOOL res = true;
		strcat(pCookie,";");

		// lecture des cookies
		char sep[] = ";";
		char *token;
		token = strtok( pCookie, sep );

		while( token != NULL )
		{
			char* szPos = strchr(token,'=');
				//token[]='\0';
			if(szPos!=NULL){
				*szPos='\0';
				res = pAPI->pInternetSetCookie(pBuffer,token,szPos+1);
			} else{
				res = pAPI->pInternetSetCookie(pBuffer,NULL,token);
			}
			token = strtok( NULL, sep );
		}
	
	}
	//
	// envoi de la requête générée
	//
	dwError = 0;
		
	if(pPostDataSize == 0 && pPostBinDataSize==0)
		dwError=threadHttpSendRequest(pAPI,pReq,NULL,0,NULL,0,dwTimeOut);
	else
		if(pPostDataSize != 0)
			dwError=threadHttpSendRequest(pAPI,pReq,NULL, 0,(LPVOID)pDATA,pPostDataSize,dwTimeOut);
		else 
			dwError=threadHttpSendRequest(pAPI,pReq,NULL, 0,(LPVOID)pBINDATA,pPostBinDataSize,dwTimeOut);

	if(dwError == ERROR_INTERNET_NAME_NOT_RESOLVED)
	{
		free(pDATA);
		free(pBINDATA);
		return _ERROR_DNS;
	}
		
	if(pReq == 0 )
	{
		DWORD err = GetLastError();
		free(pDATA);
		free(pBINDATA);
		return _ERROR_UNKNOWN;
	}
    free(pDATA);
	free(pBINDATA);
	return dwError;
}

char* CHTTPClass::ReadHTTPHeader(void) {
	DWORD iLength = MAX_BUFFER;	
	if (!pAPI->pHttpQueryInfo(pReq,HTTP_QUERY_RAW_HEADERS_CRLF,(LPVOID)&pBuffer, &iLength,NULL)) 
       return NULL;
	return pBuffer;
}

char* CHTTPClass::ReadHTTPData(DWORD *dwDataLenght) {
    DWORD dwError;
	DWORD iLength = 0;
	dwError=pAPI->pInternetReadFile(pReq,(LPVOID)pBuffer,MAX_BUFFER,dwDataLenght);
    if (!dwError)
       return NULL;
   
    if (!*dwDataLenght) {
       return NULL;
    }
       
	return pBuffer;
}

char* CHTTPClass::GetErrorString(int nerror) {
	if (nerror < 0 && nerror>9)
		nerror=8;

	return tError[nerror];
}
					
