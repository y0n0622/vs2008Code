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

	// TODO:  �ڴ������ר�õĴ�������
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
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CPanel::OnPaint()
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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
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
