// ScreemManage.cpp : 实现文件
//

#include "stdafx.h"
#include "RemoteClient.h"
#include "ScreemManage.h"
#include "RemoteClientView.h"
#include <atlimage.h>


#define WM_ENDTHREAD (WM_USER+200)

// CScreemManage 对话框

IMPLEMENT_DYNAMIC(CScreemManage, CDialog)

CScreemManage::CScreemManage(CWnd* pParent /*=NULL*/, SOCKET sk)
	: CDialog(CScreemManage::IDD, pParent)
{
	AllSock = sk;
	pView = (CRemoteClientView *)pParent;
	fSock = NULL;
	pWin = NULL;

}

CScreemManage::~CScreemManage()
{
}

void CScreemManage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CScreemManage, CDialog)
END_MESSAGE_MAP()


// CScreemManage 消息处理程序

BOOL CScreemManage::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	pView->m_clientsock.MySendCommond(AllSock,MYSCREENMANAGE, NULL, 0, STARTSCREEEN);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CScreemManage::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	pView->m_clientsock.MySendCommond(AllSock,MYSCREENMANAGE, NULL, 0, STARTSENDPIC);
	Sleep(20);
	PostThreadMessage(pWin->m_nThreadID, WM_ENDTHREAD, 0, 0);
	pView->m_ScreenManage = NULL;
	delete pView->m_ScreenManage;
	CDialog::OnCancel();
}

void CScreemManage::MyStartDC(SOCKET sk)
{
	fSock = sk;
	pWin = AfxBeginThread(MyDCFunction, this);
}

UINT __cdecl CScreemManage::MyDCFunction(LPVOID lp)
{
	CScreemManage *pthis = (CScreemManage*)lp;
	FILESRCMD cmd;
	HANDLE hFile = INVALID_HANDLE_VALUE;
	CString PicTemp;
	MSG lpmsg;
	GetTempPath(MAX_PATH, PicTemp.GetBuffer(MAX_PATH));
	PicTemp.Format(_T("%s%d.jpg"), PicTemp, GetTickCount());
	while(1)
	{
		if(PeekMessage(&lpmsg, 0/*pthis->m_hWnd*/, WM_ENDTHREAD, WM_ENDTHREAD, PM_REMOVE))
		{
			closesocket(pthis->fSock);
			return 0;
		}
		if (pthis->pView->m_clientsock.MySelect(pthis->fSock))
		{
			pthis->m_FileSock.MyRecvFILE(pthis->fSock, cmd);
			if (cmd.ID == FILECREATE)
			{
				
				hFile = CreateFile(PicTemp, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, NULL, NULL);
			}
			else if (cmd.ID == STARTSCREEEN)
			{
				DWORD wd = 0;
				WriteFile(hFile, cmd.teste, cmd.fID, &wd, NULL);
				if (cmd.fID < FILESIZE)
				{
					CloseHandle(hFile);
					Sleep(30);
					pthis->MyShowPic(PicTemp);
					DeleteFile(PicTemp);
				}
			}
		}
		Sleep(50);
		
	}
	
	return 0;
}

void CScreemManage::MyShowPic(CString pPath)
{
	CDC *pDC = NULL;
	pDC = CDC::FromHandle(::GetDC(this->m_hWnd));

	CImage m_image;
	m_image.Load(pPath);

	SelectObject(pDC->m_hDC, m_image);
	//BitBlt(pDC->m_hDC, 0, 0, m_image.GetWidth(), m_image.GetHeight(), m_image.GetDC(), 0, 0, SRCCOPY);
	m_image.BitBlt(pDC->m_hDC, 0, 0);
	m_image.Destroy();
	ReleaseDC(pDC);
	//DeleteDC(pDC->m_hDC);
	/*CString PicTemp;
	GetTempPath(MAX_PATH, PicTemp.GetBuffer(MAX_PATH));
	PicTemp += _T("123.temp");
	MoveFile(pPath, PicTemp);*/
	DeleteFile(pPath);
}

BOOL CScreemManage::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_LBUTTONDOWN || pMsg->message == WM_RBUTTONDOWN || pMsg->message == WM_LBUTTONDBLCLK)
	{
		DWORD MyMsg = pMsg->message;
		CRect m_client, m_win;
		GetClientRect(&m_client);
		GetWindowRect(&m_win);
		CPoint m_pt = pMsg->pt;
		int minHight = m_win.Height() - m_client.Height();
		int minWidth = (m_win.Width() - m_client.Width())/2;
		
		m_pt.x -= (m_win.TopLeft().x + minWidth);
		m_pt.y -= (m_win.TopLeft().y + minHight - minWidth);
		pView->m_clientsock.MySendCommond(AllSock, MYSCREENMANAGE, (char*)&m_pt, sizeof(POINT), MyMsg);
	}
	return CDialog::PreTranslateMessage(pMsg);
}
