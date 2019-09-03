// MainFrame.cpp : 实现文件
//

#include "stdafx.h"
#include "BrowserFrame.h"
#include "MainFrame.h"
#include "BrowserFrame.h"
#include "resource.h"


#define  IDC_CAPTIONPANEL  10001
#define  IDC_TOOLPANEL	   10002
#define  IDC_TABPANEL	   10003
#define  IDC_SIDEBAR	   10004
#define  IDC_STATUSPANEL   10005
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CWnd)

CMainFrame::CMainFrame()
{
	m_captionPanel.m_hWnd = NULL;
	m_toolPanel.m_hWnd = NULL;
	m_tabPanel.m_hWnd = NULL;
	m_sideBar.m_hWnd = NULL;
	m_StatusPanel.m_hWnd = NULL;
}

CMainFrame::~CMainFrame()
{
}


BEGIN_MESSAGE_MAP(CMainFrame, CWnd)
	ON_WM_GETMINMAXINFO()
	ON_WM_NCCALCSIZE()
	ON_WM_NCPAINT()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_NCACTIVATE()
END_MESSAGE_MAP()



// CMainFrame 消息处理程序



BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
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
	wcex.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = cs.lpszClass;

	bRet = RegisterClassEx(&wcex);
	return bRet;
}

void CMainFrame::PostNcDestroy()
{
	// TODO: 在此添加专用代码和/或调用基类
	delete this;
	CWnd::PostNcDestroy();
}

void CMainFrame::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	lpMMI->ptMinTrackSize.x = 800;
	lpMMI->ptMinTrackSize.y = 550;
	lpMMI->ptMaxPosition.x = 0;
	lpMMI->ptMaxPosition.y = 0;

	CRect rcWorkArea;
	::SystemParametersInfo(SPI_GETWORKAREA, 0, &rcWorkArea, 0);
	lpMMI->ptMaxSize.x = lpMMI->ptMaxTrackSize.x - (rcWorkArea.Width()/78);
	lpMMI->ptMaxSize.y = lpMMI->ptMaxTrackSize.y - (rcWorkArea.Height()/16);

	CWnd::OnGetMinMaxInfo(lpMMI);
}

void CMainFrame::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	int nCxBorder = ::GetSystemMetrics(SM_CXSIZEFRAME);
	int nCyBorder = ::GetSystemMetrics(SM_CYSIZEFRAME);
	lpncsp->rgrc[0].left -= nCxBorder;
	lpncsp->rgrc[0].right += nCxBorder;
	lpncsp->rgrc[0].top -= nCyBorder;
	lpncsp->rgrc[0].bottom += nCyBorder;

	lpncsp->rgrc[0].left += 2;
	lpncsp->rgrc[0].right -= 2;
	lpncsp->rgrc[0].top += 2;
	lpncsp->rgrc[0].bottom -= 2;
	CWnd::OnNcCalcSize(bCalcValidRects, lpncsp);
}

void CMainFrame::OnNcPaint()
{
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CWnd::OnNcPaint()
	if (m_imageBorder.IsNull())
	{
		return;
	}
	CWindowDC dc(this);
	CRect rcWnd;
	GetWindowRect(rcWnd);
	m_imageBorder.Draw(dc.m_hDC, 0, 0, rcWnd.Width(), 2);
	m_imageBorder.Draw(dc.m_hDC, 0, 0, 2, rcWnd.Height());
	m_imageBorder.Draw(dc.m_hDC, rcWnd.Width() - 2, 0, 2, rcWnd.Height());
	m_imageBorder.Draw(dc.m_hDC, 0, rcWnd.Height() - 2, rcWnd.Width(), 2);

}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	//去掉win7，win8风格
	HINSTANCE hIst = LoadLibrary(_T("UxTheme.dll"));
	if (hIst)
	{
		typedef HRESULT (WINAPI *PFUN_SetWindowTheme)(HWND, LPCTSTR, LPCTSTR);
		PFUN_SetWindowTheme pFun = (PFUN_SetWindowTheme)GetProcAddress(hIst, "SetWindowTheme");
		if (pFun)
		{
			pFun(m_hWnd, _T(""), _T(""));
		}
		FreeLibrary(hIst);
	}
	hIst = LoadLibrary(_T("dwmapi.dll"));
	if (hIst)
	{
		typedef HRESULT (WINAPI *TmpFun)(HWND, DWORD, LPCVOID, DWORD);
		TmpFun DwmSetWindowAttributeEx = (TmpFun)::GetProcAddress(hIst, "DwmSetWindowAttributeEx");
		if (DwmSetWindowAttributeEx)
		{
			DWORD dwAttr = 1;
			DwmSetWindowAttributeEx(GetSafeHwnd(), 2, &dwAttr, 4);
		}
		FreeLibrary(hIst);
	}

	////////
	if (!m_imageBorder.IsNull())
	{
		return 0;
	}
	if (m_captionPanel.m_hWnd)
	{
		return 0;
	}

	if (m_toolPanel.m_hWnd)
	{
		return 0;
	}

	m_imageBorder.LoadFromResource(AfxGetInstanceHandle(), IDB_BORDER);
	m_captionPanel.Create(_T(""), WS_CHILD | WS_VISIBLE, CRect(0, 0, lpCreateStruct->cx, 25), this, IDC_CAPTIONPANEL);

	m_toolPanel.Create(_T(""), WS_CHILD | WS_VISIBLE, CRect(0, 25, lpCreateStruct->cx, 77), this, IDC_TOOLPANEL);

	if (m_tabPanel.m_hWnd)
	{
		return 0;
	}
	m_tabPanel.Create(_T(""), WS_CHILD | WS_VISIBLE, CRect(0, 77, lpCreateStruct->cx, 106), this, IDC_TABPANEL);

	
	if (NULL == m_sideBar.m_hWnd)
	{
		m_sideBar.Create(_T(""), WS_CHILD | WS_VISIBLE, CRect(0, 106, 200, lpCreateStruct->cy - 26), this, IDC_SIDEBAR);
	}


	if(m_StatusPanel.m_hWnd == NULL)
	{
		m_StatusPanel.Create(_T(""), WS_CHILD | WS_VISIBLE, CRect(0, lpCreateStruct->cy - 26, lpCreateStruct->cx, lpCreateStruct->cy), this, IDC_STATUSPANEL);

	}
	return 0;
}

void CMainFrame::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if (NULL == m_captionPanel.m_hWnd)
	{
		return ;
	}
	if (NULL == m_toolPanel.m_hWnd)
	{
		return ;
	}
	CRect rcClient;
	GetClientRect(rcClient);
	CRect rcCaptionPanel = rcClient;
	rcCaptionPanel.bottom = 25;
	m_captionPanel.MoveWindow(rcCaptionPanel);

	CRect rcToolPanel = rcClient;
	rcToolPanel.top = 25;
	rcToolPanel.bottom = 77;
	m_toolPanel.MoveWindow(rcToolPanel);

	if (NULL == m_tabPanel.m_hWnd)
	{
		return;
	}
	CRect rcTabPanel = rcClient;
	rcTabPanel.top = 77;
	rcTabPanel.bottom = 106;
	m_tabPanel.MoveWindow(rcTabPanel);

	if (m_sideBar.m_hWnd)
	{
		CRect rcWnd;
		m_sideBar.GetWindowRect(rcWnd);
		m_sideBar.MoveWindow(0, 106, rcWnd.Width(), rcClient.Height() - 106 - 26);
	}

	if(m_StatusPanel.m_hWnd)
	{
		CRect rcStatusPanel = rcClient;
		rcStatusPanel.top = rcClient.Height() - 26;
		m_StatusPanel.MoveWindow(rcStatusPanel);

	}
}

BOOL CMainFrame::OnNcActivate(BOOL bActive)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	BOOL bRet = CWnd::OnNcActivate(bActive);
	Invalidate();
	SendMessage(WM_NCPAINT, 0, 0);
	return bRet;
}
