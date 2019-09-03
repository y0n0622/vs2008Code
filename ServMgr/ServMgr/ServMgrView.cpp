
// ServMgrView.cpp : CServMgrView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
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

// CServMgrView 构造/析构

CServMgrView::CServMgrView()
{
	// TODO: 在此处添加构造代码
	m_pServList = NULL;

}

CServMgrView::~CServMgrView()
{
}

BOOL CServMgrView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CListView::PreCreateWindow(cs);
}

void CServMgrView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();


	// TODO: 调用 GetListCtrl() 直接访问 ListView 的列表控件，
	//  从而可以用项填充 ListView。
	SetWindowLong(GetListCtrl().m_hWnd, GWL_STYLE, GetWindowLong(GetListCtrl().m_hWnd, GWL_STYLE) | LVS_REPORT);
	GetListCtrl().SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	GetListCtrl().InsertColumn(0, _T("服务管理"), LVCFMT_LEFT, 200);
	GetListCtrl().InsertColumn(1, _T("服务状态"), LVCFMT_LEFT, 90);
	GetListCtrl().InsertColumn(2, _T("启动类型"), LVCFMT_LEFT, 90);
	GetListCtrl().InsertColumn(3, _T("文件路径"), LVCFMT_LEFT, 200);
	GetListCtrl().InsertColumn(4, _T("服务描述"), LVCFMT_LEFT, 450);
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


// CServMgrView 诊断

#ifdef _DEBUG
void CServMgrView::AssertValid() const
{
	CListView::AssertValid();
}

void CServMgrView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}

CServMgrDoc* CServMgrView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CServMgrDoc)));
	return (CServMgrDoc*)m_pDocument;
}
#endif //_DEBUG


// CServMgrView 消息处理程序




void CServMgrView::OnNMRClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	
	// TODO: 在此添加控件通知处理程序代码
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
	// TODO: 在此添加命令处理程序代码
	int iSelected = GetListCtrl().GetSelectionMark();
	if (iSelected >= 0)
	{
		CServConfig m_ServCfg;
		CServItem *m_pItem = (CServItem *)GetListCtrl().GetItemData(iSelected);
		ASSERT(m_pItem);
		if (!m_ServCfg.CtrlServStatus(m_pItem->m_strServName, SERVICE_RUNNING))
		{
			MessageBox(_T("启动服务失败！"), _T("提示"), MB_OK);
			return ;
		}
		GetListCtrl().SetItemText(iSelected, 1, _T("已启动"));
	}
}


void CServMgrView::OnStopService()
{
	// TODO: 在此添加命令处理程序代码
	int iSelected = GetListCtrl().GetSelectionMark();
	if (iSelected >= 0)
	{
		CServConfig m_ServCfg;
		CServItem *m_pItem = (CServItem *)GetListCtrl().GetItemData(iSelected);
		ASSERT(m_pItem);
		if (!m_ServCfg.CtrlServStatus(m_pItem->m_strServName, SERVICE_STOPPED))
		{
			MessageBox(_T("停止服务失败！"), _T("提示"), MB_OK);
			return ;
		}
		GetListCtrl().SetItemText(iSelected, 1, _T(""));
	}
}


void CServMgrView::OnPauseService()
{
	// TODO: 在此添加命令处理程序代码
	int iSelected = GetListCtrl().GetSelectionMark();
	if (iSelected >= 0)
	{
		CServConfig m_ServCfg;
		CServItem *m_pItem = (CServItem *)GetListCtrl().GetItemData(iSelected);
		ASSERT(m_pItem);
		if (!m_ServCfg.CtrlServStatus(m_pItem->m_strServName, SERVICE_PAUSED))
		{
			MessageBox(_T("暂停服务失败！"), _T("提示"), MB_OK);
			return ;
		}
		GetListCtrl().SetItemText(iSelected, 1, _T("已暂停"));
	}
}


void CServMgrView::OnContinueService()
{
	// TODO: 在此添加命令处理程序代码
	int iSelected = GetListCtrl().GetSelectionMark();
	if (iSelected >= 0)
	{
		CServConfig m_ServCfg;
		CServItem *m_pItem = (CServItem *)GetListCtrl().GetItemData(iSelected);
		ASSERT(m_pItem);
		if (!m_ServCfg.CtrlServStatus(m_pItem->m_strServName, SERVICE_RUNNING))
		{
			MessageBox(_T("恢复服务失败！"), _T("提示"), MB_OK);
			return ;
		}
		GetListCtrl().SetItemText(iSelected, 1, _T("已启动"));
	}
}


void CServMgrView::OnRAuto()
{
	// TODO: 在此添加命令处理程序代码
	int iSelected = GetListCtrl().GetSelectionMark();
	if (iSelected >= 0)
	{
		CServConfig m_ServCfg;
		CServItem *m_pItem = (CServItem *)GetListCtrl().GetItemData(iSelected);
		ASSERT(m_pItem);
		if (!m_ServCfg.CtrlServStartType(m_pItem->m_strServName, SERVICE_AUTO_START))
		{
			MessageBox(_T("改变服务启动方式为自动失败！"), _T("提示"), MB_OK);
			return ;
		}
		GetListCtrl().SetItemText(iSelected, 2, _T("自动"));
	}
}


void CServMgrView::OnRDemand()
{
	// TODO: 在此添加命令处理程序代码
	int iSelected = GetListCtrl().GetSelectionMark();
	if (iSelected >= 0)
	{
		CServConfig m_ServCfg;
		CServItem *m_pItem = (CServItem *)GetListCtrl().GetItemData(iSelected);
		ASSERT(m_pItem);
		if (!m_ServCfg.CtrlServStartType(m_pItem->m_strServName, SERVICE_DEMAND_START))
		{
			MessageBox(_T("改变服务启动方式为手动失败！"), _T("提示"), MB_OK);
			return ;
		}
		GetListCtrl().SetItemText(iSelected, 2, _T("手动"));
	}
}


void CServMgrView::OnRDisable()
{
	// TODO: 在此添加命令处理程序代码
	int iSelected = GetListCtrl().GetSelectionMark();
	if (iSelected >= 0)
	{
		CServConfig m_ServCfg;
		CServItem *m_pItem = (CServItem *)GetListCtrl().GetItemData(iSelected);
		ASSERT(m_pItem);
		if (!m_ServCfg.CtrlServStartType(m_pItem->m_strServName, SERVICE_DISABLED))
		{
			MessageBox(_T("改变服务启动方式为禁用失败！"), _T("提示"), MB_OK);
			return ;
		}
		GetListCtrl().SetItemText(iSelected, 2, _T("已禁用"));
	}
}


void CServMgrView::OnRDelServ()
{
	// TODO: 在此添加命令处理程序代码
	int iSelected = GetListCtrl().GetSelectionMark();
	if (iSelected < 0)
	{
		return ;
	}
	INT iRet = MessageBox(_T("确定删除此服务吗？"), _T("提示"), MB_OKCANCEL | MB_ICONWARNING);
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
		AfxMessageBox(_T("删除失败！"));
	}
}


void CServMgrView::OnRAddServ()
{
	// TODO: 在此添加命令处理程序代码
	CCreateServDlg m_CreateDlg;
	m_CreateDlg.DoModal();

}


void CServMgrView::OnRRefresh()
{
	// TODO: 在此添加命令处理程序代码
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

	// TODO: 在此处添加消息处理程序代码
	CServConfig m_SverCfg;
	if (m_pServList)
	{
		m_SverCfg.FreeServList(m_pServList);
	}
}


void CServMgrView::OnRRun()
{
	// TODO: 在此添加命令处理程序代码
	AfxMessageBox(_T("作者太懒，什么也没有留下！"));
}
