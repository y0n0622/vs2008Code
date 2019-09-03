#pragma once
#include "ServCmd.h"
class CServSock;
class CServFileSock;
class CServScreen
{
public:
	CServScreen(void);
	~CServScreen(void);
	void ScreenCommand(COMMEND cmd, SOCKET sk);
	SOCKET AllSock;
	
private:
	static DWORD WINAPI MyDcFunc(LPVOID lp);
	DWORD threadID;
	CServSock *m_ServSock;
	CServFileSock *m_ServFileSock;
public:
	void MyMouse(COMMEND cmd, DWORD dr);
};
