#ifndef _STRUCTDLL_H_
#define _STRUCTDLL_H_
#include <windows.h>
typedef struct _parameters {
        char sInstalledKey[MAX_PATH];
        char sInstalledRep[MAX_PATH];
        char sTmpDirectory[MAX_PATH];
        char sWindowsVersion[MAX_PATH];
        char sSystemDirectory[MAX_PATH]; 
        char sComputerName[MAX_PATH];
        char sUserName[MAX_PATH];    
        char sMyDocuments[MAX_PATH];
} PARAMETERS;

#endif
