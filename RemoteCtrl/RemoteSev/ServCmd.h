#pragma once
#include "ServProcess.h"

class CServSock;

class CServCmd
{
public:
	CServCmd(void);
	~CServCmd(void);
	void MyCmdCommend(COMMEND cmd, SOCKET sk);
	static DWORD WINAPI MyReadGFUNC(LPVOID lp);
private:
	void Pipe2(SOCKET sk);

public:
	HANDLE hWrite1; 
	HANDLE hRead2;
	CServSock *m_ServSock;
	SOCKET AllSock;
	void MyWriteCmd(COMMEND cmd);

};
