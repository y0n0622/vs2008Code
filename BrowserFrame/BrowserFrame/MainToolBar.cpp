#include "StdAfx.h"
#include "MainToolBar.h"
#include "resource.h"


#define IDC_BACKWARD    10001
#define IDC_FORWARD     10002
#define IDC_STOP	    10003
#define IDC_REFRESH	    10004
#define IDC_HOME		 10005
#define IDC_RESTORE		 10006
#define IDC_FAVORITE	 10007


CMainToolBar::CMainToolBar(void)
{
	m_btnBackward.m_hWnd = NULL;
	m_btnForward.m_hWnd = NULL;
	m_btnStop.m_hWnd = NULL;
	m_btnRefresh.m_hWnd = NULL;
	m_btnHome.m_hWnd = NULL;
	m_btnRestore.m_hWnd = NULL;
	m_btnFavorite.m_hWnd = NULL;

}

CMainToolBar::~CMainToolBar(void)
{
}
BEGIN_MESSAGE_MAP(CMainToolBar, CPanel)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_FAVORITE, OnFavorite)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

int CMainToolBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CPanel::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	if (m_btnBackward.m_hWnd)
	{
		return 0;
	}
	if (m_btnForward.m_hWnd)
	{
		return 0;
	}
	if (m_btnStop.m_hWnd)
	{
		return 0;
	}
	if (m_btnRefresh.m_hWnd)
	{
		return 0;
	}
	if (m_btnHome.m_hWnd)
	{
		return 0;
	}
	if (m_btnRestore.m_hWnd)
	{
		return 0;
	}
	if (m_btnFavorite.m_hWnd)
	{
		return 0;
	}
	
	m_btnBackward.Create(_T(""), WS_CHILD | WS_VISIBLE, CRect(0, 0, 48, 52), this, IDC_BACKWARD);
	m_btnBackward.SetImage(IDB_BACKWORD);
	//m_btnBackward.EnableWindow(FALSE);//按钮变灰

	m_btnForward.Create(_T(""), WS_CHILD | WS_VISIBLE, CRect(48, 0, 96, 52), this, IDC_FORWARD);
	m_btnForward.SetImage(IDB_FORWARD);

	m_btnStop.Create(_T(""), WS_CHILD | WS_VISIBLE, CRect(96, 0, 144, 52), this, IDC_STOP);
	m_btnStop.SetImage(IDB_STOP);

	m_btnRefresh.Create(_T(""), WS_CHILD | WS_VISIBLE, CRect(144, 0, 192, 52), this, IDC_REFRESH);
	m_btnRefresh.SetImage(IDB_REFRESH);

	m_btnHome.Create(_T(""), WS_CHILD | WS_VISIBLE, CRect(192, 0, 240, 52), this, IDC_HOME);
	m_btnHome.SetImage(IDB_HOME);

	m_btnRestore.Create(_T(""), WS_CHILD | WS_VISIBLE, CRect(240, 0, 288, 52), this, IDC_RESTORE);
	m_btnRestore.SetImage(IDB_RESTORE);

	m_btnFavorite.Create(_T(""), WS_CHILD | WS_VISIBLE, CRect(288, 0, 336, 52), this, IDC_FAVORITE);
	m_btnFavorite.SetImage(IDB_FAVORITE);
	return 0;
}

void CMainToolBar::OnFavorite()
{
	SendMessage(WM_LBUTTONDOWN, BN_CLICKED, 0);
}
void CMainToolBar::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	
	
	CPanel::OnLButtonDown(nFlags, point);
}
