#include "StdAfx.h"
#include "ToolPanel.h"
#include "resource.h"

CToolPanel::CToolPanel(void)
{
	m_MainToolBar.m_hWnd = NULL;
	m_AddressBar.m_hWnd = NULL;
	m_SearchBar.m_hWnd = NULL;
}

CToolPanel::~CToolPanel(void)
{
}
BEGIN_MESSAGE_MAP(CToolPanel, CPanel)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_SIZE()
END_MESSAGE_MAP()

int CToolPanel::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CPanel::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	if (!m_imgTool.IsNull())
	{
		return 0;
	}
	m_imgTool.LoadFromResource(AfxGetInstanceHandle(), IDB_TOOLBAR);

	if (m_MainToolBar.m_hWnd)
	{
		return 0;
	}

	if (m_AddressBar.m_hWnd)
	{
		return 0;
	}

	if (m_SearchBar.m_hWnd)
	{
		return 0;
	}
	m_MainToolBar.Create(_T(""), WS_CHILD | WS_VISIBLE, CRect(0, 0, 336, 52), this, 10001);

	CRect rcClient;
	GetClientRect(rcClient);
	m_AddressBar.Create(_T(""), WS_CHILD | WS_VISIBLE, CRect(336, 0, rcClient.Width() - 200, 52), this, 10002);
	
	m_SearchBar.Create(_T(""), WS_CHILD | WS_VISIBLE, CRect(rcClient.Width() - 200, 0, rcClient.Width(), 52), this, 10003);
	return 0;
}

void CToolPanel::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CPanel::OnPaint()
	if (m_imgTool.IsNull())
	{
		return ;
	}
	CRect rcClient;
	GetClientRect(rcClient);
	m_imgTool.Draw(dc.m_hDC, rcClient);
}

void CToolPanel::OnSize(UINT nType, int cx, int cy)
{
	CPanel::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if (NULL == m_AddressBar.m_hWnd)
	{
		return;
	}

	if (NULL == m_SearchBar.m_hWnd)
	{
		return;
	}
	CRect rcClient;
	GetClientRect(rcClient);
	m_AddressBar.MoveWindow(CRect(336, 0, rcClient.Width() - 200, 52));

	m_SearchBar.MoveWindow(CRect(rcClient.Width() - 200, 0, rcClient.Width(), 52));
}
