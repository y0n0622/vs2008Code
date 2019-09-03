#include "StdAfx.h"
#include "TabPanel.h"
#include "resource.h"

#define  IDC_TABCTRL      10001
#define  IDC_TABMANAGER    10002
CTabPanel::CTabPanel(void)
{
	m_wndTabCtr.m_hWnd = NULL;
	m_tabManager.m_hWnd = NULL;
}

CTabPanel::~CTabPanel(void)
{
}
BEGIN_MESSAGE_MAP(CTabPanel, CPanel)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_SIZE()
END_MESSAGE_MAP()

int CTabPanel::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CPanel::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	if (m_imgTabPanel.IsNull())
	{
		m_imgTabPanel.LoadFromResource(AfxGetInstanceHandle(), IDB_TABBAR);
	}

	CRect rcClient;
	GetClientRect(rcClient);
	if (m_wndTabCtr.m_hWnd)
	{
		return 0;
	}
	CRect rcTabCtrl = rcClient;
	rcTabCtrl.left = 4;
	rcTabCtrl.right = rcClient.Width() - 100;
	m_wndTabCtr.Create(_T(""), WS_CHILD | WS_VISIBLE, rcTabCtrl, this, IDC_TABCTRL);
	m_wndTabCtr.AddItem(_T("Item1"));
	m_wndTabCtr.AddItem(_T("Item2"));
	m_wndTabCtr.AddItem(_T("Item3"));

	if (m_tabManager.m_hWnd)
	{
		return 0;
	}
	CRect rcTabManager = rcClient;
	rcTabManager.left = rcClient.Width() - 100;
	rcTabManager.right = rcClient.Width();
	m_tabManager.Create(_T(""), WS_CHILD | WS_VISIBLE, rcTabManager, this, IDC_TABMANAGER);

	return 0;
}

void CTabPanel::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CPanel::OnPaint()
	if (m_imgTabPanel.IsNull())
	{
		return;
	}
	CRect rcClient;
	GetClientRect(rcClient);
	m_imgTabPanel.Draw(dc.m_hDC, rcClient);
}

void CTabPanel::OnSize(UINT nType, int cx, int cy)
{
	CPanel::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	CRect rcClient;
	GetClientRect(rcClient);
 
	if (m_wndTabCtr.m_hWnd)
	{
		CRect rcTabCtrl = rcClient;
		rcTabCtrl.left = 4;
		rcTabCtrl.right = rcClient.Width() - 100;
		m_wndTabCtr.MoveWindow(rcTabCtrl);
	}

	if (m_tabManager.m_hWnd)
	{
		CRect rcTabManager = rcClient;
		rcTabManager.left = rcClient.Width() - 100;
		rcTabManager.right = rcClient.Width();
		m_tabManager.MoveWindow(rcTabManager);
	}
}
