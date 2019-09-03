#include "StdAfx.h"
#include "SearchBar.h"
#include "resource.h"
#include "MenuEx.h"

#define IDC_ICONBUTTON  10003
CSearchBar::CSearchBar(void)
{
	m_btnSearch.m_hWnd = NULL;
	m_EditSearch.m_hWnd = NULL;
	m_IconButton.m_hWnd = NULL;
}

CSearchBar::~CSearchBar(void)
{
}
BEGIN_MESSAGE_MAP(CSearchBar, CPanel)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_ICONBUTTON, OnSelIconButton)
END_MESSAGE_MAP()

int CSearchBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CPanel::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	if (m_imgTool.IsNull())
	{
		m_imgTool.LoadFromResource(AfxGetInstanceHandle(), IDB_TOOLBAR);
	}

	if (m_imgBand.IsNull())
	{
		m_imgBand.LoadFromResource(AfxGetInstanceHandle(), IDB_COMBOBOX);
	}
	if (m_btnSearch.m_hWnd)
	{
		return 0;
	}
	CRect rcClient;
	GetClientRect(rcClient);
	m_btnSearch.Create(_T(""), WS_VISIBLE | WS_CHILD, CRect(rcClient.Width() - 39, 8, rcClient.Width() - 2, 42), this, 10001);
	m_btnSearch.SetImage(IDB_SEARCH_GO);

	if (m_EditSearch.m_hWnd)
	{
		return 0;
	}
	m_EditSearch.Create(WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, CRect(30, 17, rcClient.Width() - 39, 36), this, 10002);


	if (m_IconButton.m_hWnd)
	{
		return 0;
	}
	m_IconButton.Create(_T(""), WS_VISIBLE | WS_CHILD, CRect(7, 16, 26, 35), this, IDC_ICONBUTTON);
	m_IconButton.SetIcon(IDI_BAIDU);
	return 0;
}

void CSearchBar::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CPanel::OnPaint()
	if (m_imgTool.IsNull())
	{
		return;
	}
	CRect rcClient;
	GetClientRect(rcClient);
	m_imgTool.Draw(dc.m_hDC, rcClient);

	if (m_imgBand.IsNull())
	{
		return;
	}
	CRect rcLeft;
	rcLeft.left = 0;
	rcLeft.right = 10;
	rcLeft.top = 11;
	rcLeft.bottom = 39;
	m_imgBand.Draw(dc.m_hDC, rcLeft, CRect(0, 0, 10, m_imgBand.GetHeight()));

	CRect rcRight = rcLeft;
	rcRight.left = rcLeft.right;
	rcRight.right = rcClient.right - 40;
	m_imgBand.Draw(dc.m_hDC, rcRight, CRect(10, 0, m_imgBand.GetWidth(), m_imgBand.GetHeight()));

}

void CSearchBar::OnSelIconButton()
{
	CMenuEx menu;
	BOOL bRet = menu.CreatePopupMenu();
	ASSERT(bRet);

	MENUITEM mi1;
	mi1.hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_BAIDU));
	mi1.m_strText = _T("百度");
	menu.AppendMenu(MF_OWNERDRAW | MF_BYCOMMAND, 10001, (LPCTSTR)&mi1);

	MENUITEM mi2;
	mi2.hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_GOOGLE));
	mi2.m_strText = _T("谷歌");
	menu.AppendMenu(MF_OWNERDRAW | MF_BYCOMMAND, 10002, (LPCTSTR)&mi2);

	CPoint pt;
	GetCursorPos(&pt);

	int nID = menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RETURNCMD, pt.x, pt.y, this);
	switch(nID)
	{
	case 10001://百度
		if (m_IconButton.m_hWnd)
		{
			m_IconButton.SetIcon(IDI_BAIDU);
		}
		break;
	case 10002://谷歌
		if (m_IconButton.m_hWnd)
		{
			m_IconButton.SetIcon(IDI_GOOGLE);
		}
		break;
	case 0:
		return;
	default:
		ASSERT(FALSE);
		break;
	}
}
