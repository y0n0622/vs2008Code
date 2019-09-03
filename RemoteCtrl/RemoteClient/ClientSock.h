#pragma once
#include "sys.h"
//#include <Afxtempl.h>
class CRemoteClientView;

class ServItem
{
public:
	SOCKET sk;
	CString ip;
	CString cmpname;
	UINT memsize;
	DWORD os;
};

extern UINT m_MyPort;
extern CArray<ServItem, ServItem&> m_ServArray;

class CClientSock
{
public:
	CClientSock(void);
	~CClientSock(void);
	static UINT __cdecl MyControllingFuction(LPVOID pParam);
	BOOL MySelect(SOCKET sk);
	BOOL MySendCommond(SOCKET sk, int Id, char *text, int size, int fd = NULL);
	BOOL MyRecvCommond(SOCKET sk, COMMEND &cmd);
	static UINT __cdecl MyRecvFuction(LPVOID pParam);
private:
	BOOL IsExist(CString ip, int & idx);
public:
	SOCKET MyBindSock(UINT Port);
};
