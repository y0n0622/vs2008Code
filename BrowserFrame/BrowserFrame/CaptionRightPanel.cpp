#include "StdAfx.h"
#include "CaptionRightPanel.h"
#include "resource.h"
#include "MainFrame.h"
#define IDC_MINBTN     10001
#define IDC_MAXBTN     10002
#define IDC_CLOSEBTN	10003

CCaptionRightPanel::CCaptionRightPanel(void)
{
	m_imgMinBtn.m_hWnd = NULL;
	m_imgMaxBtn.m_hWnd = NULL;
	m_imgCloseBtn.m_hWnd = NULL;
}

CCaptionRightPanel::~CCaptionRightPanel(void)
{
}
BEGIN_MESSAGE_MAP(CCaptionRightPanel, CPanel)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_BN_CLICKED(IDC_MINBTN, OnClickedMinBtn)
	ON_BN_CLICKED(IDC_MAXBTN, OnClickedMaxBtn)
	ON_BN_CLICKED(IDC_CLOSEBTN, OnClickedCloseBtn)
END_MESSAGE_MAP()

int CCaptionRightPanel::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CPanel::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	if (!m_imgCaption.IsNull())
	{
		return 0;
	}
	if (m_imgMaxBtn.m_hWnd)
	{
		return 0;
	}
	if (m_imgMinBtn.m_hWnd)
	{
		return 0;
	}
	if (m_imgCloseBtn.m_hWnd)
	{
		return 0;
	}
	m_imgCaption.LoadFromResource(AfxGetInstanceHandle(), IDB_CAPTION);

	m_imgMinBtn.Create(_T(""), WS_CHILD | WS_VISIBLE, CRect(0, 0, 25, 25), this, IDC_MINBTN);
	m_imgMinBtn.SetImage(IDB_MINBTN);

	m_imgMaxBtn.Create(_T(""), WS_CHILD | WS_VISIBLE, CRect(25, 0, 50, 25), this, IDC_MAXBTN);
	m_imgMaxBtn.SetImage(IDB_MAXBTN);

	m_imgCloseBtn.Create(_T(""), WS_CHILD | WS_VISIBLE, CRect(50, 0, 75, 25), this, IDC_CLOSEBTN);
	m_imgCloseBtn.SetImage(IDB_CLOSEBTN);
	return 0;
}

void CCaptionRightPanel::OnPaint()
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
void CCaptionRightPanel::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();
	if (pMainFrame)
	{
		pMainFrame->SendMessage(WM_NCLBUTTONDOWN, HTCAPTION, 0);
	}
	CPanel::OnLButtonDown(nFlags, point);
}

void CCaptionRightPanel::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();

	if (pMainFrame)
	{
		pMainFrame->SendMessage(WM_NCLBUTTONDBLCLK, HTCAPTION, 0);
	}
	CPanel::OnLButtonDblClk(nFlags, point);
}

void CCaptionRightPanel::OnClickedMinBtn()
{
	CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();
	if (pMainFrame)
	{
		pMainFrame->SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
	}
}
void CCaptionRightPanel::OnClickedMaxBtn()
{
	CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();
	if (pMainFrame)
	{
		if (pMainFrame->IsZoomed())
		{
			pMainFrame->SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0);
		}
		else
		{
			pMainFrame->SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0);
		}
	}
}
void CCaptionRightPanel::OnClickedCloseBtn()
{
	CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();
	if (pMainFrame)
	{
		pMainFrame->SendMessage(WM_SYSCOMMAND, SC_CLOSE, 0);
	}
}