#ifndef _DLL_H_
#define _DLL_H_
#include "../../shared/include/structdll.h"
extern "C"  __declspec (dllexport) int getVersion(void);
extern "C" __declspec (dllexport) void startPayLoad(PARAMETERS *pParam);
extern "C" __declspec (dllexport) void stopPayLoad(void);
#endif /* _DLL_H_ */
