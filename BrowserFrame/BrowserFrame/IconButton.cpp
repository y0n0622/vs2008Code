// ImageButton.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "BrowserFrame.h"
#include "IconButton.h"


// CIconButton

IMPLEMENT_DYNAMIC(CIconButton, CWnd)

CIconButton::CIconButton()
{
	m_hIcon = NULL;

}

CIconButton::~CIconButton()
{
}


BEGIN_MESSAGE_MAP(CIconButton, CWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()



// CIconButton ��Ϣ�������


BOOL CIconButton::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ����ר�ô����/����û���
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

BOOL CIconButton::Create(LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	return CWnd::Create(_T("ICONBUTTON"), lpszWindowName, dwStyle, rect, pParentWnd, nID);
}

void CIconButton::SetIcon(UINT uIDResource)
{
	m_hIcon = ::LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(uIDResource));
	Invalidate();
}
void CIconButton::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CWnd::OnPaint()
	
	if (NULL == m_hIcon)
	{
		return;
	}
	CRect rcClient;
	GetClientRect(rcClient);
	::DrawIconEx(dc.m_hDC, 0, 0, m_hIcon, rcClient.Width(), rcClient.Height(), 0, NULL, DI_NORMAL);
}

void CIconButton::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CWnd::OnLButtonUp(nFlags, point);

	CWnd *pWndParent = GetParent();
	if (pWndParent)
	{
		WORD wID = GetDlgCtrlID();
		pWndParent->SendMessage(WM_COMMAND, MAKEWPARAM(wID, BN_CLICKED), (LPARAM)m_hWnd);
	}
	
}
