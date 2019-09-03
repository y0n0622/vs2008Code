#include "StdAfx.h"
#include "SideBar.h"
#include "resource.h"
#include <math.h>


#define IDC_SIDEBARHEAD    10001
#define IDC_DESKPANEL      10004
#define IDC_UITREECTRL     10005

BOOL bFavorite = TRUE;
CSideBar::CSideBar(void)
{
	m_btnSideBarHead.m_hWnd = NULL;
	m_btnSideBarClose.m_hWnd = NULL;
	m_deskPanel.m_hWnd = NULL;
	m_uiTreeCtrl.m_hWnd = NULL;
}

CSideBar::~CSideBar(void)
{
}
BEGIN_MESSAGE_MAP(CSideBar, CPanel)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_NCHITTEST()
	ON_WM_GETMINMAXINFO()
//	ON_BN_CLICKED(IDC_SIDEBARCLOSE, OnSideBarClose)
	ON_BN_CLICKED(IDC_SIDEBARCLOSE, OnShowSideBar)
END_MESSAGE_MAP()

int CSideBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CPanel::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	CRect rcClient;
	GetClientRect(rcClient);
	if (NULL == m_btnSideBarHead.m_hWnd)
	{
		CRect rcSideBarHead = rcClient;
		rcSideBarHead.bottom = 27;
		rcSideBarHead.right = rcClient.Width() - 20;
		m_btnSideBarHead.Create(_T(""), WS_CHILD | WS_VISIBLE, rcSideBarHead, this, IDC_SIDEBARHEAD);
		m_btnSideBarHead.SetImage(IDB_SIDEBARHEAD);
	}

	if (NULL == m_btnSideBarClose.m_hWnd)
	{
		CRect rcSideBarClose = rcClient;
		rcSideBarClose.left = rcClient.Width() - 20;
		rcSideBarClose.right = rcClient.Width() - 3;
		rcSideBarClose.bottom = 27;
		m_btnSideBarClose.Create(_T(""), WS_CHILD | WS_VISIBLE, rcSideBarClose, this, IDC_SIDEBARCLOSE);
		m_btnSideBarClose.SetImage(IDB_SIDEBARCLOSE);
	}

	if (m_imgRight.IsNull())
	{
		m_imgRight.LoadFromResource(AfxGetInstanceHandle(), IDB_SIDEBARSPIT);
	}

	if (NULL == m_deskPanel.m_hWnd)
	{
		CRect rcDeskPanel;
		rcDeskPanel.left = 0;
		rcDeskPanel.right = rcClient.Width() - 3;
		rcDeskPanel.top = 27;
		rcDeskPanel.bottom = rcClient.bottom;
		m_deskPanel.Create(_T("sss"), WS_CHILD | WS_VISIBLE, rcDeskPanel, this, IDC_DESKPANEL);

		for (int i = 0; i < 3; i++)
		{
			CString strTitle;
			strTitle.Format(_T("item-%d"), i);
			m_deskPanel.AddDesk(strTitle);
		}
		DESK *pDesk =  m_deskPanel.GetDesk(0);
		if (pDesk)
		{
			CRect rcItem0;
			pDesk->m_wndPanel.GetClientRect(rcItem0);
			m_uiTreeCtrl.Create(WS_CHILD | WS_VISIBLE | TVS_FULLROWSELECT | TVS_SINGLEEXPAND, rcItem0, &(pDesk->m_wndPanel), IDC_UITREECTRL);
			m_uiTreeCtrl.SetItemHeight(20);
			for(int i = 0; i < 5; i++)
			{
				CString strItem;
				strItem.Format(_T("root-%d"), i);
				HTREEITEM hRoot = m_uiTreeCtrl.InsertItem(strItem);

				for (int j = 0; j < 10; j++)
				{
					strItem.Format(_T("item-%d-%d"), i, j);
					HTREEITEM hTreeItem = m_uiTreeCtrl.InsertItem(strItem, 0, 0, hRoot);
					HICON hIcon = NULL;
					int n = rand()%5;
					switch (n)
					{
					case 0:
						hIcon = LoadIcon(NULL, IDI_APPLICATION);
						break;
					case 1:
						hIcon = LoadIcon(NULL, IDI_HAND);
						break;
					case 2:
						hIcon = LoadIcon(NULL, IDI_QUESTION);
						break;
					case 3:
						hIcon = LoadIcon(NULL, IDI_EXCLAMATION);
						break;
					case 4:
						hIcon = LoadIcon(NULL, IDI_WARNING);
						break;
					default:
						hIcon = LoadIcon(NULL, IDI_APPLICATION);
						break;
					}
					
					m_uiTreeCtrl.SetItemData(hTreeItem, (DWORD)hIcon);
					m_uiTreeCtrl.SetItemHeightEx(hTreeItem, 40);
				}
				m_uiTreeCtrl.Expand(hRoot, TVE_EXPAND);
			}
		}
	}
	
	return 0;
}

void CSideBar::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CPanel::OnPaint()
	CRect rcClient;
	GetClientRect(rcClient);

	dc.FillSolidRect(rcClient, RGB(255, 255, 255));
	if (!m_imgRight.IsNull())
	{
		CRect rcSpit = rcClient;
		rcSpit.left = rcClient.Width() - 3;
		m_imgRight.Draw(dc.m_hDC, rcSpit);
	}
}

void CSideBar::OnSize(UINT nType, int cx, int cy)
{
	CPanel::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	CRect rcClient;
	GetClientRect(rcClient);
	if (m_btnSideBarHead.m_hWnd)
	{
		CRect rcSideBarHead = rcClient;
		rcSideBarHead.bottom = 27;
		rcSideBarHead.right = rcClient.Width() - 20;
		m_btnSideBarHead.MoveWindow(rcSideBarHead);
	}

	if (m_btnSideBarClose.m_hWnd)
	{
		CRect rcSideBarClose = rcClient;
		rcSideBarClose.left = rcClient.Width() - 20;
		rcSideBarClose.right = rcClient.Width() - 3;
		rcSideBarClose.bottom = 27;
		m_btnSideBarClose.MoveWindow(rcSideBarClose);
	}


	if (m_deskPanel.m_hWnd)
	{
		CRect rcDeskPanel;
		rcDeskPanel.left = 0;
		rcDeskPanel.right = rcClient.Width() - 3;
		rcDeskPanel.top = 27;
		rcDeskPanel.bottom = rcClient.bottom;
		m_deskPanel.MoveWindow(rcDeskPanel);
	}
}

LRESULT CSideBar::OnNcHitTest(CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CRect rcClient;
	GetClientRect(rcClient);
	CRect rcSpit = rcClient;
	rcSpit.left = rcSpit.Width() - 3;

	CPoint pt = point;
	ScreenToClient(&pt);
	BOOL bPtInRect = rcSpit.PtInRect(pt);
	if (bPtInRect)
	{
		return HTRIGHT;
	}

	return CPanel::OnNcHitTest(point);
}

void CSideBar::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	lpMMI->ptMinTrackSize.x = 100;
	lpMMI->ptMaxTrackSize.x = 300;
	CPanel::OnGetMinMaxInfo(lpMMI);
}

void CSideBar::OnSideBarClose()
{
	
	
}

void CSideBar::OnShowSideBar()
{
	if (bFavorite == TRUE)
	{
		bFavorite = FALSE;
		ShowWindow(SW_HIDE);
	}
	else
	{
		bFavorite = TRUE;
		ShowWindow(SW_SHOW);
	}
}
