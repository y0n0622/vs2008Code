#pragma once
#include "sys.h"
class CRemoteClientView;
extern CArray<SOCKET, SOCKET>m_FlSock;
extern BOOL bl; 
extern CString FSPath;
extern BOOL bFileCancel;
class CFileSock
{
public:
	CFileSock(void);
	~CFileSock(void);
	void MyFSockStart();
	void MyUpFileStart(CString path);
	

public:
	CRemoteClientView *pView;
	SOCKET FileSock;
	static UINT __cdecl FileSockFunc(LPVOID lp);
	static UINT __cdecl FileSenRcv(LPVOID lp);
	BOOL MyRecvFILE(SOCKET sk, FILESRCMD &cmd);
	void DownFile(FILESRCMD cmd);
	
	HANDLE hFile;
	static UINT __cdecl FileUpFunc(LPVOID lp);
	BOOL MySendFile(SOCKET sk, int Id, char *text, int size, int fd);

	
};
