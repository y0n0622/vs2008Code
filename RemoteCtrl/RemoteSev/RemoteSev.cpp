// RemoteSev.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "RemoteSev.h"
#include "ServSock.h"

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	CServSock m_ServSock;
	m_ServSock.MyStart();

	hEve = CreateEvent(NULL, TRUE, FALSE, NULL);
	WaitForSingleObject(hEve, INFINITE);

	WSACleanup();
	
	return 0;
}