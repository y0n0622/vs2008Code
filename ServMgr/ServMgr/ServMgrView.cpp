
// ServMgrView.cpp : CServMgrView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "ServMgr.h"
#endif

#include "ServMgrDoc.h"
#include "ServMgrView.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CServMgrView

IMPLEMENT_DYNCREATE(CServMgrView, CListView)

BEGIN_MESSAGE_MAP(CServMgrView, CListView)

	ON_NOTIFY_REFLECT(NM_RCLICK, &CServMgrView::OnNMRClick)
	ON_COMMAND(IDR_START_SERVICE, &CServMgrView::OnStartService)
	ON_COMMAND(IDR_STOP_SERVICE, &CServMgrView::OnStopService)
	ON_COMMAND(IDR_PAUSE_SERVICE, &CServMgrView::OnPauseService)
	ON_COMMAND(IDR_CONTINUE_SERVICE, &CServMgrView::OnContinueService)
	ON_COMMAND(ID_R_AUTO, &CServMgrView::OnRAuto)
	ON_COMMAND(ID_R_DEMAND, &CServMgrView::OnRDemand)
	ON_COMMAND(ID_R_DISABLE, &CServMgrView::OnRDisable)
	ON_COMMAND(ID_R_DEL_SERV, &CServMgrView::OnRDelServ)
	ON_COMMAND(ID_R_ADD_SERV, &CServMgrView::OnRAddServ)
	ON_COMMAND(ID_R_REFRESH, &CServMgrView::OnRRefresh)
	ON_WM_DESTROY()
	ON_COMMAND(ID_R_RUN, &CServMgrView::OnRRun)
END_MESSAGE_MAP()

// CServMgrView ����/����

CServMgrView::CServMgrView()
{
	// TODO: �ڴ˴���ӹ������
	m_pServList = NULL;

}

CServMgrView::~CServMgrView()
{
}

BOOL CServMgrView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CListView::PreCreateWindow(cs);
}

void CServMgrView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();


	// TODO: ���� GetListCtrl() ֱ�ӷ��� ListView ���б�ؼ���
	//  �Ӷ������������ ListView��
	SetWindowLong(GetListCtrl().m_hWnd, GWL_STYLE, GetWindowLong(GetListCtrl().m_hWnd, GWL_STYLE) | LVS_REPORT);
	GetListCtrl().SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	GetListCtrl().InsertColumn(0, _T("�������"), LVCFMT_LEFT, 200);
	GetListCtrl().InsertColumn(1, _T("����״̬"), LVCFMT_LEFT, 90);
	GetListCtrl().InsertColumn(2, _T("��������"), LVCFMT_LEFT, 90);
	GetListCtrl().InsertColumn(3, _T("�ļ�·��"), LVCFMT_LEFT, 200);
	GetListCtrl().InsertColumn(4, _T("��������"), LVCFMT_LEFT, 450);
	CServConfig m_ServCfg;
	CServItem *m_pHeader = m_ServCfg.EnumServList();
	m_pServList = m_pHeader;
	//GetListCtrl().InsertItem(GetListCtrl().GetItemCount(), _T(""));
	for (int idx = 0; m_pHeader != NULL; idx++)
	{
		GetListCtrl().InsertItem(idx, _T(""));
		GetListCtrl().SetItemText(idx, 0, m_pHeader->m_strServDispName);
		GetListCtrl().SetItemText(idx, 1, m_ServCfg.GetStateString(m_pHeader->m_dwServStatus));
		GetListCtrl().SetItemText(idx, 2, m_ServCfg.GetStartTypeString(m_pHeader->m_dwStartType));
		GetListCtrl().SetItemText(idx, 3, m_pHeader->m_strBinPath);
		GetListCtrl().SetItemText(idx, 4, m_pHeader->m_strDescription);

		GetListCtrl().SetItemData(idx, (DWORD_PTR)m_pHeader);
		m_pHeader = m_pHeader->m_pNext;
	}
	
}


// CServMgrView ���

#ifdef _DEBUG
void CServMgrView::AssertValid() const
{
	CListView::AssertValid();
}

void CServMgrView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}

CServMgrDoc* CServMgrView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CServMgrDoc)));
	return (CServMgrDoc*)m_pDocument;
}
#endif //_DEBUG


// CServMgrView ��Ϣ�������




void CServMgrView::OnNMRClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(pNMItemActivate->iItem < 0)
	{
		return ;
	}
	CMenu mMenu, *pMenu = NULL;
	mMenu.LoadMenu(IDR_R_MENU);
	pMenu = mMenu.GetSubMenu(0);
	CPoint pt;
	GetCursorPos(&pt);
	CServItem *m_pItem = (CServItem *)GetListCtrl().GetItemData(pNMItemActivate->iItem);
	ASSERT(m_pItem);
	CServConfig m_ServCfg;
	DWORD dwCurrStatus = 0;
	DWORD dwAccepted = m_ServCfg.GetServCtrlAccepted(m_pItem->m_strServName, &dwCurrStatus);
	if (dwCurrStatus == -1)
	{
		mMenu.EnableMenuItem(IDR_START_SERVICE, MF_GRAYED);
		mMenu.EnableMenuItem(IDR_STOP_SERVICE, MF_GRAYED);
		mMenu.EnableMenuItem(IDR_PAUSE_SERVICE, MF_GRAYED);
		mMenu.EnableMenuItem(IDR_CONTINUE_SERVICE, MF_GRAYED);
		goto __Track_menu;
	}
	if (!(dwAccepted & SERVICE_ACCEPT_PAUSE_CONTINUE))
	{
		mMenu.EnableMenuItem(IDR_PAUSE_SERVICE, MF_GRAYED);
		mMenu.EnableMenuItem(IDR_CONTINUE_SERVICE, MF_GRAYED);
	}
	if (!(dwAccepted & SERVICE_ACCEPT_STOP))
	{
		mMenu.EnableMenuItem(IDR_STOP_SERVICE, MF_GRAYED);
	}
	if (dwCurrStatus == SERVICE_RUNNING)
	{
		mMenu.EnableMenuItem(IDR_START_SERVICE, MF_GRAYED);
		mMenu.EnableMenuItem(IDR_CONTINUE_SERVICE, MF_GRAYED);
	}
	else if (dwCurrStatus == SERVICE_STOPPED)
	{
		mMenu.EnableMenuItem(IDR_STOP_SERVICE, MF_GRAYED);
		mMenu.EnableMenuItem(IDR_PAUSE_SERVICE, MF_GRAYED);
		mMenu.EnableMenuItem(IDR_CONTINUE_SERVICE, MF_GRAYED);
	}
	else if (dwCurrStatus == SERVICE_PAUSED)
	{
		mMenu.EnableMenuItem(IDR_START_SERVICE, MF_GRAYED);
		mMenu.EnableMenuItem(IDR_PAUSE_SERVICE, MF_GRAYED);
	}
	else
	{
		mMenu.EnableMenuItem(IDR_START_SERVICE, MF_GRAYED);
		mMenu.EnableMenuItem(IDR_STOP_SERVICE, MF_GRAYED);
		mMenu.EnableMenuItem(IDR_PAUSE_SERVICE, MF_GRAYED);
		mMenu.EnableMenuItem(IDR_CONTINUE_SERVICE, MF_GRAYED);
	}

__Track_menu:
	pMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, this);
	*pResult = 0;
}


void CServMgrView::OnStartService()
{
	// TODO: �ڴ���������������
	int iSelected = GetListCtrl().GetSelectionMark();
	if (iSelected >= 0)
	{
		CServConfig m_ServCfg;
		CServItem *m_pItem = (CServItem *)GetListCtrl().GetItemData(iSelected);
		ASSERT(m_pItem);
		if (!m_ServCfg.CtrlServStatus(m_pItem->m_strServName, SERVICE_RUNNING))
		{
			MessageBox(_T("��������ʧ�ܣ�"), _T("��ʾ"), MB_OK);
			return ;
		}
		GetListCtrl().SetItemText(iSelected, 1, _T("������"));
	}
}


void CServMgrView::OnStopService()
{
	// TODO: �ڴ���������������
	int iSelected = GetListCtrl().GetSelectionMark();
	if (iSelected >= 0)
	{
		CServConfig m_ServCfg;
		CServItem *m_pItem = (CServItem *)GetListCtrl().GetItemData(iSelected);
		ASSERT(m_pItem);
		if (!m_ServCfg.CtrlServStatus(m_pItem->m_strServName, SERVICE_STOPPED))
		{
			MessageBox(_T("ֹͣ����ʧ�ܣ�"), _T("��ʾ"), MB_OK);
			return ;
		}
		GetListCtrl().SetItemText(iSelected, 1, _T(""));
	}
}


void CServMgrView::OnPauseService()
{
	// TODO: �ڴ���������������
	int iSelected = GetListCtrl().GetSelectionMark();
	if (iSelected >= 0)
	{
		CServConfig m_ServCfg;
		CServItem *m_pItem = (CServItem *)GetListCtrl().GetItemData(iSelected);
		ASSERT(m_pItem);
		if (!m_ServCfg.CtrlServStatus(m_pItem->m_strServName, SERVICE_PAUSED))
		{
			MessageBox(_T("��ͣ����ʧ�ܣ�"), _T("��ʾ"), MB_OK);
			return ;
		}
		GetListCtrl().SetItemText(iSelected, 1, _T("����ͣ"));
	}
}


void CServMgrView::OnContinueService()
{
	// TODO: �ڴ���������������
	int iSelected = GetListCtrl().GetSelectionMark();
	if (iSelected >= 0)
	{
		CServConfig m_ServCfg;
		CServItem *m_pItem = (CServItem *)GetListCtrl().GetItemData(iSelected);
		ASSERT(m_pItem);
		if (!m_ServCfg.CtrlServStatus(m_pItem->m_strServName, SERVICE_RUNNING))
		{
			MessageBox(_T("�ָ�����ʧ�ܣ�"), _T("��ʾ"), MB_OK);
			return ;
		}
		GetListCtrl().SetItemText(iSelected, 1, _T("������"));
	}
}


void CServMgrView::OnRAuto()
{
	// TODO: �ڴ���������������
	int iSelected = GetListCtrl().GetSelectionMark();
	if (iSelected >= 0)
	{
		CServConfig m_ServCfg;
		CServItem *m_pItem = (CServItem *)GetListCtrl().GetItemData(iSelected);
		ASSERT(m_pItem);
		if (!m_ServCfg.CtrlServStartType(m_pItem->m_strServName, SERVICE_AUTO_START))
		{
			MessageBox(_T("�ı����������ʽΪ�Զ�ʧ�ܣ�"), _T("��ʾ"), MB_OK);
			return ;
		}
		GetListCtrl().SetItemText(iSelected, 2, _T("�Զ�"));
	}
}


void CServMgrView::OnRDemand()
{
	// TODO: �ڴ���������������
	int iSelected = GetListCtrl().GetSelectionMark();
	if (iSelected >= 0)
	{
		CServConfig m_ServCfg;
		CServItem *m_pItem = (CServItem *)GetListCtrl().GetItemData(iSelected);
		ASSERT(m_pItem);
		if (!m_ServCfg.CtrlServStartType(m_pItem->m_strServName, SERVICE_DEMAND_START))
		{
			MessageBox(_T("�ı����������ʽΪ�ֶ�ʧ�ܣ�"), _T("��ʾ"), MB_OK);
			return ;
		}
		GetListCtrl().SetItemText(iSelected, 2, _T("�ֶ�"));
	}
}


void CServMgrView::OnRDisable()
{
	// TODO: �ڴ���������������
	int iSelected = GetListCtrl().GetSelectionMark();
	if (iSelected >= 0)
	{
		CServConfig m_ServCfg;
		CServItem *m_pItem = (CServItem *)GetListCtrl().GetItemData(iSelected);
		ASSERT(m_pItem);
		if (!m_ServCfg.CtrlServStartType(m_pItem->m_strServName, SERVICE_DISABLED))
		{
			MessageBox(_T("�ı����������ʽΪ����ʧ�ܣ�"), _T("��ʾ"), MB_OK);
			return ;
		}
		GetListCtrl().SetItemText(iSelected, 2, _T("�ѽ���"));
	}
}


void CServMgrView::OnRDelServ()
{
	// TODO: �ڴ���������������
	int iSelected = GetListCtrl().GetSelectionMark();
	if (iSelected < 0)
	{
		return ;
	}
	INT iRet = MessageBox(_T("ȷ��ɾ���˷�����"), _T("��ʾ"), MB_OKCANCEL | MB_ICONWARNING);
	if (iRet != MB_OK)
	{
		return ;
	}
	CServItem *m_pItem = (CServItem *)GetListCtrl().GetItemData(iSelected);
	CServConfig m_ServCfg;
	if (m_ServCfg.DeleteServ(m_pItem->m_strServName))
	{
		GetListCtrl().DeleteItem(iSelected);
	}
	else
	{
		AfxMessageBox(_T("ɾ��ʧ�ܣ�"));
	}
}


void CServMgrView::OnRAddServ()
{
	// TODO: �ڴ���������������
	CCreateServDlg m_CreateDlg;
	m_CreateDlg.DoModal();

}


void CServMgrView::OnRRefresh()
{
	// TODO: �ڴ���������������
	CServConfig m_ServCfg;
	CServItem *m_pHeader = m_ServCfg.EnumServList();
	if (!m_pHeader)
	{
		return;
	}
	if (m_pServList)
	{
		m_ServCfg.FreeServList(m_pServList);
	}
	m_pServList = m_pHeader;
	GetListCtrl().DeleteAllItems();
	for (int idx = 0; m_pHeader != NULL; idx++)
	{
		GetListCtrl().InsertItem(idx, _T(""));
		GetListCtrl().SetItemText(idx, 0, m_pHeader->m_strServDispName);
		GetListCtrl().SetItemText(idx, 1, m_ServCfg.GetStateString(m_pHeader->m_dwServStatus));
		GetListCtrl().SetItemText(idx, 2, m_ServCfg.GetStartTypeString(m_pHeader->m_dwStartType));
		GetListCtrl().SetItemText(idx, 3, m_pHeader->m_strBinPath);
		GetListCtrl().SetItemText(idx, 4, m_pHeader->m_strDescription);

		GetListCtrl().SetItemData(idx, (DWORD_PTR)m_pHeader);
		m_pHeader = m_pHeader->m_pNext;
	}
}


void CServMgrView::OnDestroy()
{
	CListView::OnDestroy();

	// TODO: �ڴ˴������Ϣ����������
	CServConfig m_SverCfg;
	if (m_pServList)
	{
		m_SverCfg.FreeServList(m_pServList);
	}
}


void CServMgrView::OnRRun()
{
	// TODO: �ڴ���������������
	AfxMessageBox(_T("����̫����ʲôҲû�����£�"));
}
