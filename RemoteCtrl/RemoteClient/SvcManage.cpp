// SvcManage.cpp : 实现文件
//

#include "stdafx.h"
#include "RemoteClient.h"
#include "SvcManage.h"
#include "RemoteClientView.h"
#include "NewSVC.h"


// CSvcManage 对话框

IMPLEMENT_DYNAMIC(CSvcManage, CDialog)

CSvcManage::CSvcManage(CWnd* pParent /*=NULL*/, SOCKET sk)
	: CDialog(CSvcManage::IDD, pParent)
{
	AllSock = sk;
	pView = (CRemoteClientView *)pParent;
	m_NewSvc = NULL;
}

CSvcManage::~CSvcManage()
{
}

void CSvcManage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_SVC, m_SvcList);
}


BEGIN_MESSAGE_MAP(CSvcManage, CDialog)
	ON_COMMAND(ID_SVCSTART, &CSvcManage::OnSvcstart)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_SVC, &CSvcManage::OnNMRClickListSvc)
	ON_COMMAND(ID_BURSH, &CSvcManage::OnBursh)
	ON_COMMAND(ID_STOPSVC, &CSvcManage::OnStopsvc)
	ON_COMMAND(ID_AUTO, &CSvcManage::OnAuto)
	ON_COMMAND(ID_BOOT, &CSvcManage::OnBoot)
	ON_COMMAND(ID_DISABLE, &CSvcManage::OnDisable)
	ON_COMMAND(ID_SVCDELETE, &CSvcManage::OnSvcdelete)
	ON_COMMAND(ID_CREATESVC, &CSvcManage::OnCreatesvc)
END_MESSAGE_MAP()


// CSvcManage 消息处理程序

BOOL CSvcManage::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_SvcList.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	m_SvcList.InsertColumn(0, _T("服务真实名称"), 0, 140, -1);
	m_SvcList.InsertColumn(1, _T("服务显示名称"), 0, 140, -1);
	m_SvcList.InsertColumn(2, _T("服务状态"), 0, 90, -1);
	m_SvcList.InsertColumn(3, _T("服务启动方式"), 0, 90, -1);
	m_SvcList.InsertColumn(4, _T("服务描述"), 0, 220, -1);
	m_SvcList.InsertColumn(5, _T("程序路径"), 0, 220, -1);
	pView->m_clientsock.MySendCommond(AllSock, MYSVCMANAGE, NULL, 0, GETSVCLIST);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CSvcManage::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	pView->m_SvcManage = NULL;
	delete pView->m_SvcManage;
	CDialog::OnCancel();
}

void CSvcManage::MySvcCommend(COMMEND cmd)
{
	wchar_t temp[MAX_PATH] = {0};
	switch (cmd.fID)
	{
	case GETSVCLIST:
		ShowSVCList(cmd);
		break;
	case SVCMSG:
		memcpy(temp, cmd.teste, MAX_PATH);
		AfxMessageBox(temp);
		OnBursh();
		break;
	case SVCCREATE:
		if (m_NewSvc != NULL)
		{
			m_NewSvc->PostMessage(WM_CLOSE);
			m_NewSvc = NULL;
		}
		AfxMessageBox(_T("服务创建成功!"));
		break;
	default:
		break;
	}
}

void CSvcManage::ShowSVCList(COMMEND cmd)
{
	SVICELISTINFO svcin;
	ZeroMemory(&svcin, sizeof(SVICELISTINFO));
	memcpy(&svcin, cmd.teste, sizeof(SVICELISTINFO));
	int idx = m_SvcList.GetItemCount();
	m_SvcList.InsertItem(idx, svcin.ServiceName);
	m_SvcList.SetItemText(idx, 1, svcin.DisplayName);
	m_SvcList.SetItemText(idx, 2, MyGetCurrentState(svcin.CurrentStatus));
	m_SvcList.SetItemText(idx, 3, MyGetStartType(svcin.stype));
	m_SvcList.SetItemText(idx, 4, svcin.Description);
	m_SvcList.SetItemText(idx, 5, svcin.BinPath);
}

CString CSvcManage::MyGetCurrentState(DWORD State)
{
	CString sst;
	switch (State)
	{
	case SERVICE_RUNNING:
		sst = _T("已启动");
		break;
	case SERVICE_PAUSED:
		sst = _T("暂停");
		break;
	case SERVICE_PAUSE_PENDING:
		sst = _T("正在暂停中...");
		break;
	case SERVICE_START_PENDING:
		sst = _T("正在启动中..");
		break;
	case SERVICE_STOPPED:
		sst = _T("");
		break;
	case SERVICE_STOP_PENDING:
		sst = _T("正在停止中..");
		break;
	default:
		sst = _T("");
		break;
	}
	return sst;
}
CString CSvcManage::MyGetStartType(DWORD StartType)
{
	CString sttr;
	switch(StartType)
	{
	case SERVICE_AUTO_START:
		sttr = _T("自动");
		break;
	case SERVICE_DEMAND_START:
		sttr = _T("手动");
		break;
	case SERVICE_DISABLED:
		sttr = _T("已禁用");
		break;
	default:
		sttr = _T("");
		break;
	}
	return sttr;
}
void CSvcManage::OnSvcstart()
{
	// TODO: 在此添加命令处理程序代码
	int idx = m_SvcList.GetSelectionMark();
	wchar_t svname[MAX_PATH];
	m_SvcList.GetItemText(idx, 0, svname, MAX_PATH);
	pView->m_clientsock.MySendCommond(AllSock, MYSVCMANAGE, (char*)svname, MAX_PATH, MYSTARTSVC);

}

void CSvcManage::OnNMRClickListSvc(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	int idx = pNMItemActivate->iItem;
	if (idx < 0)
	{
		return ;
	}
	//m_ServArray.GetAt(idx).sk;
	CMenu menu, *pMenu = NULL;
	menu.LoadMenu(IDR_MEN_SVCCTRL);
	pMenu = menu.GetSubMenu(0);
	CPoint pt;
	GetCursorPos(&pt);
	CString temp;
	temp = m_SvcList.GetItemText(idx, 3);
	if (temp == _T("自动"))
	{
		menu.EnableMenuItem(ID_AUTO, MF_GRAYED);
	}
	else if (temp == _T("手动"))
	{
		menu.EnableMenuItem(ID_BOOT, MF_GRAYED);
	}
	else if (temp == _T("已禁用"))
	{
		menu.EnableMenuItem(ID_DISABLE, MF_GRAYED);
	}
	else
	{

	}
	pMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_TOPALIGN|TPM_RIGHTBUTTON, pt.x, pt.y, this);
	*pResult = 0;
}

void CSvcManage::OnBursh()
{
	// TODO: 在此添加命令处理程序代码
	m_SvcList.DeleteAllItems();
	pView->m_clientsock.MySendCommond(AllSock, MYSVCMANAGE, NULL, 0, GETSVCLIST);
}

void CSvcManage::OnStopsvc()
{
	// TODO: 在此添加命令处理程序代码
	int idx = m_SvcList.GetSelectionMark();
	wchar_t svcname[MAX_PATH];
	m_SvcList.GetItemText(idx, 0, svcname, MAX_PATH);
	pView->m_clientsock.MySendCommond(AllSock, MYSVCMANAGE, (char*)svcname, MAX_PATH, SVCSTOP);
}

void CSvcManage::OnAuto()
{
	// TODO: 在此添加命令处理程序代码
	int idx = m_SvcList.GetSelectionMark();
	wchar_t svcname[MAX_PATH];
	m_SvcList.GetItemText(idx, 0, svcname, MAX_PATH);
	pView->m_clientsock.MySendCommond(AllSock, MYSVCMANAGE, (char*)svcname, MAX_PATH, SVCAUTOTYPE);

}

void CSvcManage::OnBoot()
{
	// TODO: 在此添加命令处理程序代码
	int idx = m_SvcList.GetSelectionMark();
	wchar_t svcname[MAX_PATH];
	m_SvcList.GetItemText(idx, 0, svcname, MAX_PATH);
	pView->m_clientsock.MySendCommond(AllSock, MYSVCMANAGE, (char*)svcname, MAX_PATH, SVCBOOTYPE);

}

void CSvcManage::OnDisable()
{
	// TODO: 在此添加命令处理程序代码
	int idx = m_SvcList.GetSelectionMark();
	wchar_t svcname[MAX_PATH];
	m_SvcList.GetItemText(idx, 0, svcname, MAX_PATH);
	pView->m_clientsock.MySendCommond(AllSock, MYSVCMANAGE, (char*)svcname, MAX_PATH, SVCDISABLETYPE);

}

void CSvcManage::OnSvcdelete()
{
	// TODO: 在此添加命令处理程序代码
	int idx = m_SvcList.GetSelectionMark();
	wchar_t svcname[MAX_PATH];
	m_SvcList.GetItemText(idx, 0, svcname, MAX_PATH);
	pView->m_clientsock.MySendCommond(AllSock, MYSVCMANAGE, (char*)svcname, MAX_PATH, SVCDELETE);

}

void CSvcManage::OnCreatesvc()
{
	// TODO: 在此添加命令处理程序代码
	if (m_NewSvc == NULL)
	{
		m_NewSvc = new CNewSVC(this, AllSock);
		m_NewSvc->Create(IDD_SVCNEW, this);
		m_NewSvc->ShowWindow(SW_SHOW);
	}
	else
	{
		m_NewSvc->ShowWindow(SW_SHOW);
	}
}
