#pragma once
#include "ServSock.h"
class CServFileSock
{
public:
	CServFileSock(void);
	~CServFileSock(void);
	void FileSockStart(COMMEND cmd);
	BOOL MySendFile(SOCKET sk, int Id, char *text, int size, int fd);
	BOOL MyRecvFile(SOCKET sk,FILESRCMD &cmd);
private:
	CServSock m_ServSock;
};
