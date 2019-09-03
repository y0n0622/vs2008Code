#include "StdAfx.h"
#include "CaptionCenterPanel.h"
#include "resource.h"
#include "MainFrame.h"

#define  IDC_MENUBAR    10001
CCaptionCenterPanel::CCaptionCenterPanel(void)
{
	m_menuBar.m_hWnd = NULL;
}

CCaptionCenterPanel::~CCaptionCenterPanel(void)
{
}
BEGIN_MESSAGE_MAP(CCaptionCenterPanel, CPanel)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()

int CCaptionCenterPanel::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CPanel::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	if (!m_imgCaption.IsNull())
	{
		return 0;
	}
	m_imgCaption.LoadFromResource(AfxGetInstanceHandle(), IDB_CAPTION);

	if (m_menuBar.m_hWnd == NULL)
	{
		CRect rcClient;
		GetClientRect(rcClient);
		CRect rcMenuBar = rcClient;
		rcMenuBar.left = rcMenuBar.right - 150;
		rcMenuBar.bottom = 22;

		m_menuBar.Create(_T(""), WS_CHILD | WS_VISIBLE, rcMenuBar, this, IDC_MENUBAR);
		m_menuBar.LoadMenuFromResource(IDR_MENU1);
	}
	return 0;
}

void CCaptionCenterPanel::OnPaint()
{
	if (m_imgCaption.IsNull())
	{
		return ;
	}
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CPanel::OnPaint()
	CRect rcClient;
	GetClientRect(rcClient);
	m_imgCaption.Draw(dc.m_hDC, rcClient);
}

void CCaptionCenterPanel::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();
	if (pMainFrame)
	{
		pMainFrame->SendMessage(WM_NCLBUTTONDOWN, HTCAPTION, 0);
	}
	CPanel::OnLButtonDown(nFlags, point);
}

void CCaptionCenterPanel::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();

	if (pMainFrame)
	{
		pMainFrame->SendMessage(WM_NCLBUTTONDBLCLK, HTCAPTION, 0);
	}
	CPanel::OnLButtonDblClk(nFlags, point);
}
