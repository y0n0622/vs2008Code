#include "StdAfx.h"
#include "TabManager.h"
#include "resource.h"

#define  IDC_TABBUTTONNEW          10001
#define  IDC_TABBUTTONFULLSCREEN    10002
#define  IDC_TABBUTTONLEFT         10003
#define  IDC_TABBUTTONRIGHT        10004
#define  IDC_TABBUTTONCLOSE        10005

CTabManager::CTabManager(void)
{
	m_imgTabButtonNew.m_hWnd = NULL;
	m_imgTabButtonFullScreen.m_hWnd = NULL;
	m_imgTabButtonLeft.m_hWnd = NULL;
	m_imgTabButtonRight.m_hWnd = NULL;
	m_imgTabButtonClose.m_hWnd = NULL;
}

CTabManager::~CTabManager(void)
{
}
BEGIN_MESSAGE_MAP(CTabManager, CPanel)
	ON_WM_CREATE()
	ON_WM_PAINT()
	//ON_BN_CLICKED(IDC_TABBUTTONNEW, OnTabButtonNew)   ////////ͨ����Ϣ��Ӧ����ʵ����������addItem����ʵ��
END_MESSAGE_MAP()

int CTabManager::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CPanel::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������
	if (m_imgTabPanel.IsNull())
	{
		m_imgTabPanel.LoadFromResource(AfxGetInstanceHandle(), IDB_TABBAR);
	}

	if (m_imgTabButtonNew.m_hWnd == NULL)
	{
		m_imgTabButtonNew.Create(_T(""), WS_VISIBLE | WS_CHILD, CRect(0, 0, 20, 29), this, IDC_TABBUTTONNEW);
		m_imgTabButtonNew.SetImage(IDB_TABBUTTONNEW);
	}
	
	if (m_imgTabButtonFullScreen.m_hWnd == NULL)
	{
		m_imgTabButtonFullScreen.Create(_T(""), WS_VISIBLE | WS_CHILD, CRect(20, 0, 40, 29), this, IDC_TABBUTTONFULLSCREEN);
		m_imgTabButtonFullScreen.SetImage(IDB_FULLSCREEN);
	}

	if (m_imgTabButtonLeft.m_hWnd == NULL)
	{
		m_imgTabButtonLeft.Create(_T(""), WS_VISIBLE | WS_CHILD, CRect(40, 0, 60, 29), this, IDC_TABBUTTONLEFT);
		m_imgTabButtonLeft.SetImage(IDB_TABBUTTONLEFT);
	}

	if (m_imgTabButtonRight.m_hWnd == NULL)
	{
		m_imgTabButtonRight.Create(_T(""), WS_VISIBLE | WS_CHILD, CRect(60, 0, 80, 29), this, IDC_TABBUTTONRIGHT);
		m_imgTabButtonRight.SetImage(IDB_TABBUTTONRIGHT);
	}

	if (m_imgTabButtonClose.m_hWnd == NULL)
	{
		m_imgTabButtonClose.Create(_T(""), WS_VISIBLE | WS_CHILD, CRect(80, 0, 100, 29), this, IDC_TABBUTTONCLOSE);
		m_imgTabButtonClose.SetImage(IDB_TABBUTTONCLOSE);
	}

	
	return 0;
}

void CTabManager::OnPaint()
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
