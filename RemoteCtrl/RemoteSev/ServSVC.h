#pragma once
#include "ServCmd.h"
#include "Winsvc.h"
class CServSock;
class CServSVC
{
public:
	CServSVC(void);
	~CServSVC(void);
	void MySvcCommend(COMMEND cmd, SOCKET sk);
	bool EnableDebugPrivilegeSame(TCHAR* Privileges); //ÌáÈ¨º¯Êý
private:
	SOCKET AllSock;
	CServSock *m_ServSock;

	void MyGetSVCList();
	void MyStartSVC(COMMEND cmd);
	void MyStopSVC(COMMEND cmd);
	void MyChangeType(COMMEND cmd, DWORD ty);
	void MyDeleteSVC(COMMEND cmd);
	void MyCreateSVC(COMMEND cmd);
	
};
