#include "StdAfx.h"
#include "DeskContent.h"

CDeskContent::CDeskContent(void)
{
}

CDeskContent::~CDeskContent(void)
{
}
BEGIN_MESSAGE_MAP(CDeskContent, CPanel)
	ON_WM_SIZE()
END_MESSAGE_MAP()

void CDeskContent::OnSize(UINT nType, int cx, int cy)
{
	CPanel::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	CRect rcClient;
	GetClientRect(rcClient);
	CWnd *pWnd = GetWindow(GW_CHILD);
	if (pWnd && pWnd->m_hWnd)
	{
		pWnd->MoveWindow(rcClient);
	}
}
