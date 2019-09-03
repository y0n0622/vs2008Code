// ImageButton.cpp : 实现文件
//

#include "stdafx.h"
#include "BrowserFrame.h"
#include "ImageButton.h"


// CImageButton

IMPLEMENT_DYNAMIC(CImageButton, CWnd)

CImageButton::CImageButton()
{
	m_bIsHover = FALSE;
	m_bIsDown = FALSE;
}

CImageButton::~CImageButton()
{
}


BEGIN_MESSAGE_MAP(CImageButton, CWnd)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()



// CImageButton 消息处理程序


BOOL CImageButton::PreCreateWindow(CREATESTRUCT& cs)
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

BOOL CImageButton::Create(LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	return CWnd::Create(_T("IMAGEBUTTON"), lpszWindowName, dwStyle, rect, pParentWnd, nID);
}

void CImageButton::SetImage(UINT uIDResource)
{
	bool bIsNull = m_imgButton.IsNull();
	if (!bIsNull)
	{
		m_imgButton.Destroy();
	}
	m_imgButton.LoadFromResource(AfxGetInstanceHandle(), uIDResource);
	Invalidate();
}
void CImageButton::OnPaint()
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
	if (m_bIsHover)
	{
		if (m_bIsDown)
		{
			m_imgButton.Draw(dc.m_hDC, rcClient, CRect(m_imgButton.GetWidth()*2/4, 0, m_imgButton.GetWidth()*3/4, m_imgButton.GetHeight()));
		}
		else
		{
			m_imgButton.Draw(dc.m_hDC, rcClient, CRect(m_imgButton.GetWidth()*1/4, 0, m_imgButton.GetWidth()*2/4, m_imgButton.GetHeight()));
		}
	}
	else
	{
		m_imgButton.Draw(dc.m_hDC, rcClient, CRect(m_imgButton.GetWidth()*0/4, 0, m_imgButton.GetWidth()*1/4, m_imgButton.GetHeight()));
	}
}

void CImageButton::OnMouseMove(UINT nFlags, CPoint point)
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

LRESULT CImageButton::OnMouseHover(WPARAM wParam, LPARAM lParam)
{
	if (!m_bIsHover)
	{
		m_bIsHover = TRUE;
		Invalidate();
	}
	return TRUE;
}

LRESULT CImageButton::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	if (m_bIsHover)
	{
		m_bIsHover = FALSE;
		Invalidate();
	}
	return TRUE;
}
void CImageButton::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (!m_bIsDown)
	{
		m_bIsDown = TRUE;
		Invalidate();
	}
	CWnd::OnLButtonDown(nFlags, point);
}

void CImageButton::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CWnd::OnLButtonUp(nFlags, point);
	if (m_bIsDown)
	{
		m_bIsDown = FALSE;
		Invalidate();
	}

	CWnd *pWndParent = GetParent();
	if (pWndParent)
	{
		WORD wID = GetDlgCtrlID();
		pWndParent->SendMessage(WM_COMMAND, MAKEWPARAM(wID, BN_CLICKED), (LPARAM)m_hWnd);
	}
	
}
