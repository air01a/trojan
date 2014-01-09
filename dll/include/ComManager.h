#if !defined(COMMANAGER_H__36DB8046_25CF_412B_8619_391C20E99D42__INCLUDED_)
#define COMMANAGER_H__36DB8046_25CF_412B_8619_391C20E99D42__INCLUDED_

#define MAX_PARAMETERS_SIZE 1024
#define _PARAMCHAIN "#PARAM="
#define _COMMANDCHAIN "#COMMAND="
#define _IDCHAIN "#ID="



#include "../../shared/include/loadDll.h"
#include "../../shared/include/CHTTPClass.h"

typedef struct _COMMANDPARAMETERS {
        _COMMANDPARAMETERS *pNext;
        char sParam[MAX_PARAMETERS_SIZE];
} COMMANDPARAMETERS;

typedef struct _COMMAND {
        int command;
        int numParameters;
        int idCommand;
        int commandResult;
        char sCommandResult[MAX_PATH];
        _COMMAND *pPrev;
        _COMMAND *pNext;
        COMMANDPARAMETERS *pFirstParameters;
} COMMAND;

typedef int (*fnCOMMAND)(API *pApi, COMMAND *pCommand);

typedef struct _FUNCTIONCOMMAND {
        int command;
        _FUNCTIONCOMMAND *pNext;
        fnCOMMAND pPtrCommand;
} FUNCTIONCOMMAND;

COMMAND *CheckCommand(API *pAPI);
void FinalizeCommand(API *pAPI,COMMAND *pCommand);
BOOL ReferenceCommand(int command,fnCOMMAND pCommand);
void PrintCommand(void);
int ExecuteCommand(API *pApi, COMMAND *pCommand);

#endif
