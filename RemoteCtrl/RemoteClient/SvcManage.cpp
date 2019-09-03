// SvcManage.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "RemoteClient.h"
#include "SvcManage.h"
#include "RemoteClientView.h"
#include "NewSVC.h"


// CSvcManage �Ի���

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


// CSvcManage ��Ϣ�������

BOOL CSvcManage::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_SvcList.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	m_SvcList.InsertColumn(0, _T("������ʵ����"), 0, 140, -1);
	m_SvcList.InsertColumn(1, _T("������ʾ����"), 0, 140, -1);
	m_SvcList.InsertColumn(2, _T("����״̬"), 0, 90, -1);
	m_SvcList.InsertColumn(3, _T("����������ʽ"), 0, 90, -1);
	m_SvcList.InsertColumn(4, _T("��������"), 0, 220, -1);
	m_SvcList.InsertColumn(5, _T("����·��"), 0, 220, -1);
	pView->m_clientsock.MySendCommond(AllSock, MYSVCMANAGE, NULL, 0, GETSVCLIST);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CSvcManage::OnCancel()
{
	// TODO: �ڴ����ר�ô����/����û���
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
		AfxMessageBox(_T("���񴴽��ɹ�!"));
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
		sst = _T("������");
		break;
	case SERVICE_PAUSED:
		sst = _T("��ͣ");
		break;
	case SERVICE_PAUSE_PENDING:
		sst = _T("������ͣ��...");
		break;
	case SERVICE_START_PENDING:
		sst = _T("����������..");
		break;
	case SERVICE_STOPPED:
		sst = _T("");
		break;
	case SERVICE_STOP_PENDING:
		sst = _T("����ֹͣ��..");
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
		sttr = _T("�Զ�");
		break;
	case SERVICE_DEMAND_START:
		sttr = _T("�ֶ�");
		break;
	case SERVICE_DISABLED:
		sttr = _T("�ѽ���");
		break;
	default:
		sttr = _T("");
		break;
	}
	return sttr;
}
void CSvcManage::OnSvcstart()
{
	// TODO: �ڴ���������������
	int idx = m_SvcList.GetSelectionMark();
	wchar_t svname[MAX_PATH];
	m_SvcList.GetItemText(idx, 0, svname, MAX_PATH);
	pView->m_clientsock.MySendCommond(AllSock, MYSVCMANAGE, (char*)svname, MAX_PATH, MYSTARTSVC);

}

void CSvcManage::OnNMRClickListSvc(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	if (temp == _T("�Զ�"))
	{
		menu.EnableMenuItem(ID_AUTO, MF_GRAYED);
	}
	else if (temp == _T("�ֶ�"))
	{
		menu.EnableMenuItem(ID_BOOT, MF_GRAYED);
	}
	else if (temp == _T("�ѽ���"))
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
	// TODO: �ڴ���������������
	m_SvcList.DeleteAllItems();
	pView->m_clientsock.MySendCommond(AllSock, MYSVCMANAGE, NULL, 0, GETSVCLIST);
}

void CSvcManage::OnStopsvc()
{
	// TODO: �ڴ���������������
	int idx = m_SvcList.GetSelectionMark();
	wchar_t svcname[MAX_PATH];
	m_SvcList.GetItemText(idx, 0, svcname, MAX_PATH);
	pView->m_clientsock.MySendCommond(AllSock, MYSVCMANAGE, (char*)svcname, MAX_PATH, SVCSTOP);
}

void CSvcManage::OnAuto()
{
	// TODO: �ڴ���������������
	int idx = m_SvcList.GetSelectionMark();
	wchar_t svcname[MAX_PATH];
	m_SvcList.GetItemText(idx, 0, svcname, MAX_PATH);
	pView->m_clientsock.MySendCommond(AllSock, MYSVCMANAGE, (char*)svcname, MAX_PATH, SVCAUTOTYPE);

}

void CSvcManage::OnBoot()
{
	// TODO: �ڴ���������������
	int idx = m_SvcList.GetSelectionMark();
	wchar_t svcname[MAX_PATH];
	m_SvcList.GetItemText(idx, 0, svcname, MAX_PATH);
	pView->m_clientsock.MySendCommond(AllSock, MYSVCMANAGE, (char*)svcname, MAX_PATH, SVCBOOTYPE);

}

void CSvcManage::OnDisable()
{
	// TODO: �ڴ���������������
	int idx = m_SvcList.GetSelectionMark();
	wchar_t svcname[MAX_PATH];
	m_SvcList.GetItemText(idx, 0, svcname, MAX_PATH);
	pView->m_clientsock.MySendCommond(AllSock, MYSVCMANAGE, (char*)svcname, MAX_PATH, SVCDISABLETYPE);

}

void CSvcManage::OnSvcdelete()
{
	// TODO: �ڴ���������������
	int idx = m_SvcList.GetSelectionMark();
	wchar_t svcname[MAX_PATH];
	m_SvcList.GetItemText(idx, 0, svcname, MAX_PATH);
	pView->m_clientsock.MySendCommond(AllSock, MYSVCMANAGE, (char*)svcname, MAX_PATH, SVCDELETE);

}

void CSvcManage::OnCreatesvc()
{
	// TODO: �ڴ���������������
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
