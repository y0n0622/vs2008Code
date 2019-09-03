// TabButton.cpp : 实现文件
//

#include "stdafx.h"
#include "BrowserFrame.h"
#include "TabButton.h"


// CTabButton

IMPLEMENT_DYNAMIC(CTabButton, CWnd)

CTabButton::CTabButton()
{
	m_bIsSelected = FALSE;
}

CTabButton::~CTabButton()
{
}


BEGIN_MESSAGE_MAP(CTabButton, CWnd)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()



// CTabButton 消息处理程序


BOOL CTabButton::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此添加专用代码和/或调用基类
	HINSTANCE hInstance = (HINSTANCE)AfxGetInstanceHandle();
	ASSERT(hInstance);
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	BOOL bRet = GetClassInfoEx(hInstance, cs.lpszClass, &wcex);
	if (bRet)
	{
		return TRUE;
	}

	wcex.style = CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS;
	wcex.lpfnWndProc = AfxWndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	//wcex.hIcon = wcex.hIcon = (HICON)::LoadIcon(hInstance, MAKEINTRESOURCE(IDR_MAINFRAME));
	wcex.hIcon = wcex.hIconSm = (HICON)AfxGetApp()->LoadIcon(MAKEINTRESOURCE(IDR_MAINFRAME));
	//wcex.hIcon = wcex.hIconSm = (HICON)theApp.LoadIcon(hInstance, MAKEINTRESOURCE(IDR_MAINFRAME));
	wcex.hCursor = ::LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)GetStockObject(DKGRAY_BRUSH);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = cs.lpszClass;

	bRet = RegisterClassEx(&wcex);
	return bRet;

}

BOOL CTabButton::Create(LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	return CWnd::Create(_T("TABBUTTON"), lpszWindowName, dwStyle, rect, pParentWnd, nID);
}

void CTabButton::SetImage(UINT uIDResource)
{
	bool bIsNull = m_imgButton.IsNull();
	if (!bIsNull)
	{
		m_imgButton.Destroy();
	}
	m_imgButton.LoadFromResource(AfxGetInstanceHandle(), uIDResource);
	Invalidate();
}
void CTabButton::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CWnd::OnPaint()
	
	if (m_imgButton.IsNull())
	{
		return;
	}
	//正常，经过，按下，禁用
	CRect rcClient;
	GetClientRect(rcClient);
	BOOL bIsEnabled = IsWindowEnabled();
	if (!bIsEnabled)
	{
		m_imgButton.Draw(dc.m_hDC, rcClient, CRect(m_imgButton.GetWidth()*3/4, 0, m_imgButton.GetWidth()*4/4, m_imgButton.GetHeight()));
		return;
	}
	
	if (m_bIsSelected)
	{
		//tabButton,left,center,right
		CRect rcItemLeft = rcClient;
		rcItemLeft.right = m_imgButton.GetWidth() / 4 / 2 - 1;
		m_imgButton.Draw(dc.m_hDC, rcItemLeft, CRect(m_imgButton.GetWidth() * 2 / 4, 0, m_imgButton.GetWidth() * 2 / 4 + m_imgButton.GetWidth() / 4 / 2 - 1, m_imgButton.GetHeight()));

		CRect rcItemCenter = rcClient;
		rcItemCenter.left = m_imgButton.GetWidth() / 4 / 2 - 1;
		rcItemCenter.right = rcClient.Width() - (m_imgButton.GetWidth() / 4 / 2 - 1);
		m_imgButton.Draw(dc.m_hDC, rcItemCenter, CRect(m_imgButton.GetWidth() * 2 / 4 + m_imgButton.GetWidth() / 4 / 2 - 1, 0, m_imgButton.GetWidth() * 2 / 4 + m_imgButton.GetWidth() / 4 / 2, m_imgButton.GetHeight()));


		CRect rcItemRight = rcClient;
		rcItemRight.left = rcClient.Width() - (m_imgButton.GetWidth() / 4 / 2 - 1);
		m_imgButton.Draw(dc.m_hDC, rcItemRight, CRect(m_imgButton.GetWidth() * 2 / 4 + m_imgButton.GetWidth() / 4 / 2 + 1, 0, m_imgButton.GetWidth() * 3 / 4, m_imgButton.GetHeight()));

	}
	else
	{
		//tabButton,left,center,right
		CRect rcItemLeft = rcClient;
		rcItemLeft.right = m_imgButton.GetWidth() / 4 / 2 - 1;
		m_imgButton.Draw(dc.m_hDC, rcItemLeft, CRect(m_imgButton.GetWidth() * 0 / 4, 0, m_imgButton.GetWidth() * 0 / 4 + m_imgButton.GetWidth() / 4 / 2 - 1, m_imgButton.GetHeight()));

		CRect rcItemCenter = rcClient;
		rcItemCenter.left = m_imgButton.GetWidth() / 4 / 2 - 1;
		rcItemCenter.right = rcClient.Width() - (m_imgButton.GetWidth() / 4 / 2 - 1);
		m_imgButton.Draw(dc.m_hDC, rcItemCenter, CRect(m_imgButton.GetWidth() * 0 / 4 + m_imgButton.GetWidth() / 4 / 2 - 1, 0, m_imgButton.GetWidth() * 0 / 4 + m_imgButton.GetWidth() / 4 / 2, m_imgButton.GetHeight()));


		CRect rcItemRight = rcClient;
		rcItemRight.left = rcClient.Width() - (m_imgButton.GetWidth() / 4 / 2 - 1);
		m_imgButton.Draw(dc.m_hDC, rcItemRight, CRect(m_imgButton.GetWidth() * 0 / 4 + m_imgButton.GetWidth() / 4 / 2 + 1, 0, m_imgButton.GetWidth() * 1 / 4, m_imgButton.GetHeight()));

	}

	int OldBkMode = dc.SetBkMode(TRANSPARENT);

	CString strText;
	GetWindowText(strText);
	if (!strText.IsEmpty())
	{
		dc.DrawText(strText, rcClient, DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_WORD_ELLIPSIS);
	}

	dc.SetBkMode(OldBkMode);
}

void CTabButton::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	TRACKMOUSEEVENT tme;
	tme.cbSize = sizeof(TRACKMOUSEEVENT);
	tme.hwndTrack = m_hWnd;
	tme.dwFlags = TME_HOVER | TME_LEAVE;
	tme.dwHoverTime = HOVER_DEFAULT;
	TrackMouseEvent(&tme);
	CWnd::OnMouseMove(nFlags, point);
}

LRESULT CTabButton::OnMouseHover(WPARAM wParam, LPARAM lParam)
{
	return TRUE;
}

LRESULT CTabButton::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	return TRUE;
}
void CTabButton::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (!m_bIsSelected)
	{
		m_bIsSelected = TRUE;
		Invalidate();
	}
	CWnd *pWndParent = GetParent();
	ASSERT(pWndParent);
	WORD wID = GetDlgCtrlID();
	pWndParent->SendMessage(WM_COMMAND, MAKEWPARAM(wID, BN_CLICKED), (LPARAM)m_hWnd);
	CWnd::OnLButtonDown(nFlags, point);
}

void CTabButton::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CWnd::OnLButtonUp(nFlags, point);
}

void CTabButton::SetSelected(BOOL bIsSelected)
{
	if(bIsSelected != m_bIsSelected)
	{
		m_bIsSelected = bIsSelected;
		Invalidate();
	}
}
