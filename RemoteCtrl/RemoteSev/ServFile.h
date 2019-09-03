#pragma once
#include "ServCmd.h"

class CServFile
{
public:
	CServFile(void);
	~CServFile(void);
	void FileCommend(COMMEND cmd, SOCKET sk);
	void MyGetDriver();
	void MyGetFile(COMMEND cmd);
	void MyRunFile(COMMEND cmd);
	void MyDelFile(COMMEND cmd);
	void MyDelDir(COMMEND cmd);
	void MyCreateDir(COMMEND cmd);
private:
	SOCKET AllSock;
	CServSock *m_ServSock;
};
