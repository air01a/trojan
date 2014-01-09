// CmdExec.h: interface for the CCmdExec class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(CMDEXEC_H__6B3EA99C_707C_40FF_850E_C2E5ABC0C165__INCLUDED_)
#define CMDEXEC_H__6B3EA99C_707C_40FF_850E_C2E5ABC0C165__INCLUDED_

#include <Windows.h>
#include <stdio.h>
class CCmdExec  
{
public:
	BOOL IsActive();
	BOOL KillProcess();
	BOOL Execute(FILE *pFile);
	BOOL Execute();
	CCmdExec();
	CCmdExec(char *cmd);
	virtual ~CCmdExec();

    HANDLE InvokeImage (char*  pImageName) ;
private:
	HANDLE m_hProcess;
    HANDLE      m_hChildStdoutRd;
    HANDLE      m_hChildStdoutWr;
    HANDLE      m_hChildStdinRd;
    void ReadFromChild  (FILE *pFile);
	char m_strCommand[255];
};

#endif // !defined(AFX_CMDEXEC_H__6B3EA99C_707C_40FF_850E_C2E5ABC0C165__INCLUDED_)
