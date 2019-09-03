#include "StdAfx.h"
#include "CaptionPanel.h"


CCaptionPanel::CCaptionPanel(void)
{
	m_captionLeftPanel.m_hWnd = NULL;
	m_captionCenterPanel.m_hWnd = NULL;
	m_captionRightPanel.m_hWnd = NULL;
}

CCaptionPanel::~CCaptionPanel(void)
{
}
BEGIN_MESSAGE_MAP(CCaptionPanel, CPanel)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

int CCaptionPanel::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CPanel::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	if (m_captionLeftPanel.m_hWnd)
	{
		return 0;
	}
	if (m_captionCenterPanel.m_hWnd)
	{
		return 0;
	}
	if (m_captionRightPanel.m_hWnd)
	{
		return 0;
	}

	m_captionLeftPanel.Create(_T("CaptionLeftPanel"), WS_CHILD | WS_VISIBLE, CRect(0, 0, lpCreateStruct->cx - 290, lpCreateStruct->cy), this, 10001);
	m_captionCenterPanel.Create(_T("CaptionCenterPanel"), WS_CHILD | WS_VISIBLE, CRect(lpCreateStruct->cx - 290, 0, lpCreateStruct->cx - 50, lpCreateStruct->cy), this, 10002);
	m_captionRightPanel.Create(_T("CaptionRightPanel"), WS_CHILD | WS_VISIBLE, CRect(lpCreateStruct->cx - 50, 0, lpCreateStruct->cx, lpCreateStruct->cy), this, 10003);

	return 0;
}

void CCaptionPanel::OnSize(UINT nType, int cx, int cy)
{
	CPanel::OnSize(nType, cx, cy);
	if (NULL == m_captionLeftPanel.m_hWnd)
	{
		return ;
	}
	if (NULL == m_captionCenterPanel.m_hWnd)
	{
		return ;
	}
	if (NULL == m_captionRightPanel.m_hWnd)
	{
		return ;
	}

	CRect rcClient;
	GetClientRect(rcClient);
	m_captionLeftPanel.MoveWindow(0, 0, rcClient.Width() - 317, rcClient.Height());
	m_captionCenterPanel.MoveWindow(rcClient.Width() - 317, 0, 240, rcClient.Height());
	m_captionRightPanel.MoveWindow(rcClient.Width() - 77, 0, 77, rcClient.Height());

	// TODO: 在此处添加消息处理程序代码
}
