#include "StdAfx.h"
#include "StatusPanel.h"
#include "resource.h"

CStatusPanel::CStatusPanel(void)
{
}

CStatusPanel::~CStatusPanel(void)
{
}
BEGIN_MESSAGE_MAP(CStatusPanel, CPanel)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_NCHITTEST()
END_MESSAGE_MAP()

int CStatusPanel::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CPanel::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	if (m_img.IsNull())
	{
		m_img.LoadFromResource(AfxGetInstanceHandle(), IDB_STATUSBAR);
	}

	if (m_imgSize.IsNull())
	{
		m_imgSize.LoadFromResource(AfxGetInstanceHandle(), IDB_STATUSBARSIZE);
	}
	return 0;
}

void CStatusPanel::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CPanel::OnPaint()
	CRect rcClient;
	GetClientRect(rcClient);
	if (!m_img.IsNull())
	{
		m_img.Draw(dc.m_hDC, rcClient);
	}

	if (!m_imgSize.IsNull())
	{
		CRect rcSize = rcClient;
		rcSize.left = rcClient.Width() - 16;
		m_imgSize.Draw(dc.m_hDC, rcSize);
	}
}

LRESULT CStatusPanel::OnNcHitTest(CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CPoint pt = point;
	ScreenToClient(&pt);
	CRect rcClient;
	GetClientRect(rcClient);

	CRect rcSize = rcClient;
	rcSize.left = rcClient.Width() - 16;
	rcSize.top = rcClient.Height() - 16;
	if (rcSize.PtInRect(pt))
	{
		return HTBOTTOMRIGHT;
	}
	return CPanel::OnNcHitTest(point);
}
