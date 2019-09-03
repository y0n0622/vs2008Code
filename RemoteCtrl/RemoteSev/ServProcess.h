#pragma once
#include "..\RemoteClient\sys.h"
#include <WinSock2.h>
#pragma comment(lib, "Ws2_32.lib")
class CServSock;
class CServProcess
{
public:
	CServProcess(void);
	~CServProcess(void);
	void ProcessCommend(COMMEND cmd, SOCKET sk);
	bool EnableDebugPrivilege(TCHAR* Privileges); //ÌáÈ¨º¯Êý
private:
	void SendProList(SOCKET sk);
	CServSock *m_ServSock;
	wchar_t *MyGetProPathFile(int pid);
	void MyGetModule(COMMEND cmd, SOCKET sk);
	void MyGetModule1(COMMEND cmd, SOCKET sk);
	void MyExitPro(COMMEND cmd, SOCKET sk);
	void MySysManage(COMMEND cmd);
};
