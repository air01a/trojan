#include "../include/osversion.h"
#include<windows.h>
#include <stdio.h>

BOOL bIsWinNT;
BOOL bIsWinXP;

void GetOSVersion(char *version){
 	OSVERSIONINFO osvi;

	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	
	if(GetVersionEx(&osvi)==FALSE) {

	}

	if(osvi.dwPlatformId==VER_PLATFORM_WIN32s) {

	}
	
	if (osvi.dwPlatformId == VER_PLATFORM_WIN32_NT) 
		bIsWinNT = 1;
	else 
		bIsWinNT = 0;

	if (osvi.dwMajorVersion >= 5) 
		bIsWinXP = 1;
	else 
		bIsWinXP = 0;
	
	sprintf(version,"%i.%i",osvi.dwMajorVersion,osvi.dwMinorVersion);	
}
