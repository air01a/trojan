#if !defined(OSVERSION_H__36DB8046_25CF_412B_8619_391C20E99D42__INCLUDED_)
#define OSVERSION_H__36DB8046_25CF_412B_8619_391C20E99D42__INCLUDED_

#include <windows.h>
extern BOOL bIsWinNT;
extern BOOL bIsWinXP;
void GetOSVersion(char *version);
#endif
