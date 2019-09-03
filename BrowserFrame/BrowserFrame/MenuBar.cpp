#include "StdAfx.h"
#include "MenuBar.h"
#include "resource.h"
HHOOK gHook = NULL;
CMenuBar *gMenuBar = NULL;
CMenuItem::CMenuItem()
{
	m_dwState = Normal;
}

CMenuItem::~CMenuItem()
{

}

void CMenuItem::Draw(CDC* pDC)
{
	int nOldBkMode = pDC->SetBkMode(TRANSPARENT);
	if (m_dwState == Pressed)
	{
		CPen pen(PS_SOLID, 1, 0x234567);
		CPen *pOldPen = pDC->SelectObject(&pen);
		
		pDC->Rectangle(m_rcMenu);

		pDC->SelectObject(pOldPen);

	}

	if (m_dwState == Hover)
	{
		CPen pen(PS_SOLID, 1, 0x765432);
		CPen *pOldPen = pDC->SelectObject(&pen);

		pDC->Rectangle(m_rcMenu);

		pDC->SelectObject(pOldPen);
	}

	if (m_dwState == Normal)
	{

	}

	pDC->DrawText(m_strText, m_rcMenu, DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_WORD_ELLIPSIS);

	pDC->SetBkMode(nOldBkMode);
}

LRESULT CALLBACK MessageProc(int code, WPARAM wParam, LPARAM lParam)
{
	
	if (code == MSGF_MENU)
	{
		MSG *pMsg = (MSG*)lParam;
		CPoint pt;
		int nItem = gMenuBar->HitTest(pt);
		switch(pMsg->message)
		{
		case WM_MOUSEMOVE:
		
			pt.x = LOWORD(pMsg->lParam);
			pt.y = HIWORD(pMsg->lParam);
			gMenuBar->ScreenToClient(&pt);
			gMenuBar->OnMouseMove(0, pt);

			
			if (nItem >= 0)
			{
				CMenuItem *pMenuItem = gMenuBar->GetItem(nItem);
				pMenuItem->ShowPopupMenu(gMenuBar);
			}

			break;
		default:
			break;
		}
	}
	return CallNextHookEx(gHook, code, wParam, lParam);
}


void CMenuItem::ShowPopupMenu(CWnd* pWnd)
{
	CPoint pt;
	pt.x = m_rcMenu.left;
	pt.y = m_rcMenu.bottom;

	pWnd->ClientToScreen(&pt);
	if (m_hPopupMenu)
	{
		DWORD dwThreadId = GetCurrentThreadId();
		if (NULL == gHook)
		{
			gHook = SetWindowsHookEx(WH_MSGFILTER, MessageProc, NULL, dwThreadId);
		}
		
		::TrackPopupMenuEx(m_hPopupMenu, TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON | TPM_VERTICAL, pt.x, pt.y, pWnd->m_hWnd, NULL);
		
		if (gHook == NULL)
		{
			UnhookWindowsHookEx(gHook);
			gHook = NULL;
		}
		
	}
}


void CMenuItem::Updata(DWORD dwNewState, CWnd* pWnd)
{
	if (m_dwState != dwNewState)
	{
		m_dwState = dwNewState;
		pWnd->InvalidateRect(m_rcMenu);
	}
}

CMenuBar::CMenuBar(void)
{
	m_font.m_hObject = NULL;

}

CMenuBar::~CMenuBar(void)
{
	clear();
}

void CMenuBar::clear()
{
	int nMenuItemCount = GetMenuItemCount();
	for (int i = 0; i < nMenuItemCount; i++)
	{
		CMenuItem* pMenuItem = GetItem(i);
		if (pMenuItem)
		{
			delete pMenuItem;
			pMenuItem = NULL;
		}
	}
	m_vecMenuItem.clear();
}

int CMenuBar::HitTest(CPoint pt)
{
	int nMenuItemCount = GetMenuItemCount();
	for (int i = 0; i < nMenuItemCount; i++)
	{
		CMenuItem* pMenuItem = GetItem(i);
		if (pMenuItem)
		{
			BOOL bPcInRect = pMenuItem->m_rcMenu.PtInRect(pt);
			if (bPcInRect)
			{
				return i; 
			}
		}
	}
	return HTERROR;
}



BEGIN_MESSAGE_MAP(CMenuBar, CPanel)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEHOVER()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

void CMenuBar::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CPanel::OnPaint()
	CFont* pOldFont = (CFont*)dc.SelectObject(&m_font);
	if (!m_imgBkGnd.IsNull())
	{
		CRect rcClient;
		GetClientRect(rcClient);
		m_imgBkGnd.Draw(dc.m_hDC, rcClient, CRect(0, 0, m_imgBkGnd.GetWidth(), 22));
	}
	int nMenuItemCount = GetMenuItemCount();
	for (int i = 0; i < nMenuItemCount; i++)
	{
		CMenuItem *pMenuItem = GetItem(i);
		pMenuItem->Draw((CDC*)&dc);
	}
	dc.SelectObject(pOldFont);
}

int CMenuBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CPanel::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	if (m_imgBkGnd.IsNull())
	{
		m_imgBkGnd.LoadFromResource(AfxGetInstanceHandle(), IDB_CAPTION);
	}
	if (m_font.m_hObject == NULL)
	{
		m_font.CreateFont(14, 0, 0, 0, 1, 0, 0,	 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("arial"));
		//SetFont(&m_font);
	}
	return 0;
}

void CMenuBar::LoadMenuFromResource(UINT uIDResource)
{
	ASSERT(uIDResource>0);
	
	menu.LoadMenu(uIDResource);
	ASSERT(menu.m_hMenu);

	int nMenuItemCount = menu.GetMenuItemCount();
	for (int i = 0; i < nMenuItemCount; i++)
	{
		CString strText;
		menu.GetMenuString(i, strText, MF_BYPOSITION);

		CMenuItem* pMenuItem = new CMenuItem;
		pMenuItem->m_strText = strText;
		pMenuItem->m_rcMenu = CRect(i*50, 0, i*50+50, 22);
		pMenuItem->m_hPopupMenu = menu.GetSubMenu(i)->m_hMenu;
		m_vecMenuItem.push_back(pMenuItem);
	}
	Invalidate();
}

int CMenuBar::GetMenuItemCount()
{
	return m_vecMenuItem.size();
}

CMenuItem* CMenuBar::GetItem(int nItem)
{
	ASSERT(nItem >= 0);
	ASSERT(nItem < GetMenuItemCount());
	return m_vecMenuItem.at(nItem);
}
void CMenuBar::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	
	CPanel::OnLButtonDown(nFlags, point);
	int nItem = HitTest(point);
	if (nItem >= 0)
	{
		gMenuBar = this;
		CMenuItem* pMenuItem = GetItem(nItem);
		pMenuItem->ShowPopupMenu(this);

	}
}

void CMenuBar::OnMouseHover(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CPanel::OnMouseHover(nFlags, point);
	
	
}

void CMenuBar::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	
	CPanel::OnMouseMove(nFlags, point);
	int nItem = HitTest(point);
	if (nItem >= 0)
	{
		int nMenuItemCount = GetMenuItemCount();
		for (int i = 0; i < nMenuItemCount; i++)
		{
			CMenuItem* pMenuItem = GetItem(i);
			if (i == nItem)
			{
				pMenuItem->Updata(CMenuItem::Hover, this);
			}
			else
			{
				pMenuItem->Updata(CMenuItem::Normal, this);
			}
			
			/*if (pMenuItem)
			{
				if (pMenuItem->m_dwState != CMenuItem::Hover)
				{
					pMenuItem->m_dwState = CMenuItem::Hover;
					InvalidateRect(pMenuItem->m_rcMenu);
				}
			}
			else
			{

			}*/
		}
		
	}
	else
	{
		int nMenuItemCount = GetMenuItemCount();
		for (int i = 0; i < nMenuItemCount; i++)
		{
			CMenuItem* pMenuItem = GetItem(i);
			pMenuItem->Updata(CMenuItem::Normal, this);
		}
	}
}
