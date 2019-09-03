#include "StdAfx.h"
#include "DeskPanel.h"
#include "resource.h"

#define DESKHIGHT   25
#define IDC_DESKBUTTON   10001
#define IDC_PANEL        10002

CDeskPanel::CDeskPanel(void)
{
	m_hWnd = NULL;
}

CDeskPanel::~CDeskPanel(void)
{

	int nCount = m_vecDesk.size();


	for (int i = 0; i < nCount; i++)
	{
		DESK *pDeskTemp = m_vecDesk.at(i);
		delete pDeskTemp;
		pDeskTemp = NULL;
	}
	m_vecDesk.clear();
}

int CDeskPanel::AddDesk(CString strTitle)
{
	if (NULL == m_hWnd)
	{
		return 0;
	}

	CRect rcClient;
	GetClientRect(rcClient);

	CRect rcNewDesk;
	int nCount = m_vecDesk.size();
	rcNewDesk.left = 0;
	rcNewDesk.right = rcClient.Width();
	rcNewDesk.top = DESKHIGHT * nCount;
	rcNewDesk.bottom = rcNewDesk.top + DESKHIGHT;

	for (int i = 0; i < nCount; i++)
	{
		DESK *pDeskTemp = m_vecDesk.at(i);
		pDeskTemp->deskButton.SetSelected(FALSE);
	}
	DESK *pDesk = new DESK;
	pDesk->deskButton.Create(strTitle, WS_CHILD | WS_VISIBLE, rcNewDesk, this, IDC_DESKBUTTON);
	pDesk->deskButton.SetImage(IDB_DESKBUTTON);
	pDesk->deskButton.SetSelected(TRUE);

	pDesk->m_wndPanel.Create(strTitle, WS_CHILD | WS_VISIBLE, rcNewDesk, this, IDC_PANEL); 
	m_vecDesk.push_back(pDesk);
	ArrangeDesk();
	return nCount;
}

void CDeskPanel::ArrangeDesk()
{
	if (NULL == m_hWnd)
	{
		return ;
	}

	CRect rcClient;
	GetClientRect(rcClient);

	CRect rcNewDesk;
	int nCount = m_vecDesk.size();
	if (nCount == 0)
	{
		return ;
	}
	

	int nPos = 0;
	for (int i = 0; i < nCount; i++)
	{
		DESK *pDeskTemp = m_vecDesk.at(i);
		CRect rcDesk;
		rcDesk.top = nPos;
		rcDesk.bottom = rcDesk.top + DESKHIGHT;
		rcDesk.left = 0;
		rcDesk.right = rcClient.Width();
		pDeskTemp->deskButton.MoveWindow(rcDesk);
		BOOL bIsSelected = pDeskTemp->deskButton.GetSelected();
		if (bIsSelected)
		{
			CRect rcDeskPanel;
			rcDeskPanel.left = 0;
			rcDeskPanel.right = rcClient.Width();
			rcDeskPanel.top = rcDesk.bottom;
			rcDeskPanel.bottom = rcDeskPanel.top + (rcClient.Height() - nCount * DESKHIGHT);
			pDeskTemp->m_wndPanel.MoveWindow(rcDeskPanel);
			pDeskTemp->m_wndPanel.ShowWindow(TRUE);
		}
		else
		{
			pDeskTemp->m_wndPanel.ShowWindow(SW_HIDE);

		}
		nPos += DESKHIGHT;

		if (bIsSelected)
		{
			nPos += (rcClient.Height() - nCount * DESKHIGHT);
		}
	}
}BEGIN_MESSAGE_MAP(CDeskPanel, CPanel)
ON_WM_SIZE()
END_MESSAGE_MAP()

void CDeskPanel::OnSize(UINT nType, int cx, int cy)
{
	CPanel::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	ArrangeDesk();
}

BOOL CDeskPanel::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类
	WORD wNotifyCode = HIWORD(wParam);
	if(wNotifyCode == BN_CLICKED)
	{
		WORD wID = LOWORD(wParam);
		ASSERT(wID == IDC_DESKBUTTON);
		HWND hWndButton = (HWND)lParam;
		ASSERT(hWndButton);

		int nCount = m_vecDesk.size();
		for (int i = 0; i < nCount; i++)
		{
			DESK *pDeskButton = m_vecDesk.at(i);
			ASSERT(pDeskButton);

			if (pDeskButton->deskButton.m_hWnd == hWndButton)
			{
				pDeskButton->deskButton.SetSelected(TRUE);
			}
			else
			{
				pDeskButton->deskButton.SetSelected(FALSE);
			}
		}
		ArrangeDesk();
	}
	return CPanel::OnCommand(wParam, lParam);
}

DESK* CDeskPanel::GetDesk(int nIndex)
{
	ASSERT(nIndex>=0 && nIndex<m_vecDesk.size());
	return m_vecDesk.at(nIndex);
}