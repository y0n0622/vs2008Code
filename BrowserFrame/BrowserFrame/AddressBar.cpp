#include "StdAfx.h"
#include "AddressBar.h"
#include "resource.h"

CAddressBar::CAddressBar(void)
{
	m_addrComboBox.m_hWnd = NULL;
	m_btnGo.m_hWnd = NULL;
}

CAddressBar::~CAddressBar(void)
{
}
BEGIN_MESSAGE_MAP(CAddressBar, CPanel)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()


int CAddressBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CPanel::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	if (!m_imgTool.IsNull())
	{
		return 0;
	}
	m_imgTool.LoadFromResource(AfxGetInstanceHandle(), IDB_TOOLBAR);

	if (m_btnGo.m_hWnd)
	{
		return 0;
	}

	if (m_addrComboBox.m_hWnd)
	{
		return 0;
	}

	CRect rcClient;
	GetClientRect(rcClient);
	m_addrComboBox.Create(WS_VISIBLE | WS_CHILD | WS_VSCROLL | CBS_AUTOHSCROLL | CBS_DROPDOWN | CBS_HASSTRINGS | CBS_OWNERDRAWVARIABLE, CRect(20, 11, rcClient.Width() - 22, 200), this, 10001);

	for (int i = 0; i < 10; i++)
	{
		CString strItem;
		strItem.Format(_T("http://www.baidu.com/%d.html"), i);
		HICON hIcon = ::LoadIcon(NULL, IDI_INFORMATION);
		m_addrComboBox.AddString(strItem, hIcon);
	}

	m_btnGo.Create(_T(""), WS_CHILD | WS_VISIBLE, CRect(rcClient.Width() - 22, 11, rcClient.Width(), 39), this, 10002);
	m_btnGo.SetImage(IDB_GO);
	return 0;
}

void CAddressBar::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CPanel::OnPaint()
	if (m_imgTool.IsNull())
	{
		return ;
	}
	CRect rcClient;
	GetClientRect(rcClient);
	m_imgTool.Draw(dc.m_hDC, rcClient);
}
void CAddressBar::OnSize(UINT nType, int cx, int cy)
{
	CPanel::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if (NULL == m_btnGo.m_hWnd)
	{
		return ;
	}

	if (NULL == m_addrComboBox.m_hWnd)
	{
		return ;
	}
	CRect rcClient;
	GetClientRect(rcClient);
	m_addrComboBox.MoveWindow(CRect(20, 11, rcClient.Width() - 22, 200));
	m_btnGo.MoveWindow(CRect(rcClient.Width() - 22, 11, rcClient.Width(), 39));

}
