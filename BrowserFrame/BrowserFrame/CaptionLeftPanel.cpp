#include "StdAfx.h"
#include "CaptionLeftPanel.h"
#include "resource.h"
#include "MainFrame.h"

CCaptionLeftPanel::CCaptionLeftPanel(void)
{
}

CCaptionLeftPanel::~CCaptionLeftPanel(void)
{
}
BEGIN_MESSAGE_MAP(CCaptionLeftPanel, CPanel)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_CONTEXTMENU()
END_MESSAGE_MAP()

int CCaptionLeftPanel::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CPanel::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	if (!m_imgCaption.IsNull())
	{
		return 0;
	}
	m_imgCaption.LoadFromResource(AfxGetInstanceHandle(), IDB_CAPTION);
	return 0;
}

void CCaptionLeftPanel::OnPaint()
{
	if (m_imgCaption.IsNull())
	{
		return ;
	}
	CRect rcClient;
	GetClientRect(rcClient);
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CPanel::OnPaint()
	
	m_imgCaption.Draw(dc.m_hDC, rcClient);
	CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();
	if (pMainFrame)
	{
		HICON hIcon = (HICON)GetClassLong(pMainFrame->m_hWnd, GCL_HICONSM);
		if (hIcon)
		{
			DrawIconEx(dc.m_hDC, 4, 4, hIcon, 16, 16, 0, 0, DI_NORMAL);
		}
		
		CString strText;
		pMainFrame->GetWindowText(strText);
		if (strText.IsEmpty())
		{
			return;
		}
		int nOldBkMode = dc.SetBkMode(TRANSPARENT);
		CRect rcText = rcClient;
		rcText.left = 25;
		dc.DrawText(strText, rcText, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_WORD_ELLIPSIS);
		dc.SetBkMode(nOldBkMode);
	}
}

void CCaptionLeftPanel::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();
	if (pMainFrame)
	{
		pMainFrame->SendMessage(WM_NCLBUTTONDOWN, HTCAPTION, 0);
	}
	CPanel::OnLButtonDown(nFlags, point);
}

void CCaptionLeftPanel::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();
	
	if (pMainFrame)
	{
		pMainFrame->SendMessage(WM_NCLBUTTONDBLCLK, HTCAPTION, 0);
	}
	CPanel::OnLButtonDblClk(nFlags, point);
}

void CCaptionLeftPanel::OnContextMenu(CWnd* pWnd, CPoint point)
{
	// TODO: 在此处添加消息处理程序代码
	CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();

	if (pMainFrame)
	{
		CMenu *pSysMenu = pMainFrame->GetSystemMenu(0);
		if (pSysMenu)
		{
			int nID = pSysMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RETURNCMD, point.x, point.y, pMainFrame);
			if (nID > 0)
			{
				if (nID == SC_CLOSE)
				{
					pMainFrame->SendMessage(WM_SYSCOMMAND, nID, 0);
					return;
				}
				pMainFrame->SendMessage(WM_SYSCOMMAND, nID, 0);
			}
		}
	}
}
