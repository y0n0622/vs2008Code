#include "StdAfx.h"
#include "TabCtrEx.h"
#include "resource.h"

#define  IDC_TABBUTTON 10001

CTabCtrEx::CTabCtrEx(void)
{
}

CTabCtrEx::~CTabCtrEx(void)
{
	int nCount = m_vecTabButton.size();
	if (nCount == 0)
	{
		return;
	}
	for (int i = 0; i < nCount; i++)
	{
		CTabButton *pTabButton = m_vecTabButton.at(i);
		delete pTabButton;
		pTabButton = NULL;
	}
	m_vecTabButton.clear();
}
BEGIN_MESSAGE_MAP(CTabCtrEx, CPanel)
	ON_WM_CREATE()
	ON_WM_PAINT()
END_MESSAGE_MAP()

int CTabCtrEx::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CPanel::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������
	if (m_imgTabPanel.IsNull())
	{
		m_imgTabPanel.LoadFromResource(AfxGetInstanceHandle(), IDB_TABBAR);
	}
	return 0;
}

void CTabCtrEx::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CPanel::OnPaint()
	if (m_imgTabPanel.IsNull())
	{
		return;
	}
	CRect rcClient;
	GetClientRect(rcClient);
	m_imgTabPanel.Draw(dc.m_hDC, rcClient);
}

int CTabCtrEx::AddItem(CString strText)
{
	CRect rcTabButton;
	int nIndex = m_vecTabButton.size();
	ASSERT(nIndex >= 0);
	if (nIndex == 0)
	{
		rcTabButton.left = 0;
		rcTabButton.right = 100;
		rcTabButton.top = 2;
		rcTabButton.bottom = 29;
	}
	else
	{
		CTabButton *pLastTabButton = m_vecTabButton.at(nIndex - 1);
		ASSERT(pLastTabButton);
		ASSERT(pLastTabButton->m_hWnd);
		CRect rcLastTabButton;
		pLastTabButton->GetWindowRect(rcLastTabButton);
		ScreenToClient(rcLastTabButton);

		rcTabButton.left = rcLastTabButton.right + 2;
		rcTabButton.right = rcTabButton.left + 100;
		rcTabButton.top = 2;
		rcTabButton.bottom = 29;
	}

	CTabButton *pTabButton = new CTabButton();
	ASSERT(pTabButton);
	pTabButton->Create(strText, WS_CHILD | WS_VISIBLE, rcTabButton, this, IDC_TABBUTTON);
	ASSERT(pTabButton->m_hWnd);
	pTabButton->SetImage(IDB_TABBUTTON);

	m_vecTabButton.push_back(pTabButton);
	return nIndex;
}
BOOL CTabCtrEx::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: �ڴ����ר�ô����/����û���
	WORD wNotifyCode = HIWORD(wParam);
	if(wNotifyCode == BN_CLICKED)
	{
		WORD wID = LOWORD(wParam);
		ASSERT(wID == IDC_TABBUTTON);
		HWND hWndButton = (HWND)lParam;
		ASSERT(hWndButton);

		int nCount = m_vecTabButton.size();
		for (int i = 0; i < nCount; i++)
		{
			CTabButton *pTabButton = m_vecTabButton.at(i);
			ASSERT(pTabButton);
			ASSERT(pTabButton->m_hWnd);

			if (pTabButton->m_hWnd == hWndButton)
			{
				pTabButton->SetSelected(TRUE);
			}
			else
			{
				pTabButton->SetSelected(FALSE);
			}
		}
	}
	return CPanel::OnCommand(wParam, lParam);
}
