// Process.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "RemoteClient.h"
#include "Process.h"
#include "RemoteClientView.h"
//#include "resource.h"


// CProcess �Ի���

IMPLEMENT_DYNAMIC(CProcess, CDialog)

CProcess::CProcess(CWnd* pParent /*=NULL*/, SOCKET sk)
	: CDialog(CProcess::IDD, pParent)
{
	itemsSock = sk;
	pView = (CRemoteClientView *)pParent;
}

CProcess::~CProcess()
{
}

void CProcess::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST3, m_PrList);
	DDX_Control(pDX, IDC_LIST4, m_ModuleList);
}


BEGIN_MESSAGE_MAP(CProcess, CDialog)
	ON_NOTIFY(NM_RCLICK, IDC_LIST3, &CProcess::OnNMRClickList3)
	ON_COMMAND(ID_PROCESS_MODULE, &CProcess::OnProcessModule)
	ON_COMMAND(ID_PROCESS_BURSH, &CProcess::OnProcessBursh)
	ON_COMMAND(ID_PROCESS_EXITPRO, &CProcess::OnProcessExitpro)
END_MESSAGE_MAP()


// CProcess ��Ϣ�������

BOOL CProcess::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	
	m_PrList.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
	m_PrList.InsertColumn(0, _T("��������"), 0, 120, -1);
	m_PrList.InsertColumn(1, _T("PID"), 0, 90, -1);
	m_PrList.InsertColumn(2, _T("����·��"), 0, 200, -1);
	m_ModuleList.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
	m_ModuleList.InsertColumn(0, _T("ģ������"), 0, 120, -1);
	m_ModuleList.InsertColumn(1, _T("ģ��·��"), 0, 290, -1);

	pView->m_clientsock.MySendCommond(itemsSock, MYGETPROCESS, NULL, 0, PROCESSLIST);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CProcess::ProcessCommend(COMMEND cmd)
{
	switch (cmd.fID)
	{
	case PROCESSLIST:
		MyShowProList(cmd);
		break;
	case MODULELIST:
		MyShowModule(cmd);
		break;
	case EXITPROCESS:
		AfxMessageBox(_T("���̽���!"));
		OnProcessBursh();
		break;
	default:
		break;
	}
}

void CProcess::MyShowProList(COMMEND cmd)
{
	PROCESSINFO proinfo;
	ZeroMemory(&proinfo, sizeof(proinfo));
	memcpy(&proinfo, cmd.teste, sizeof(PROCESSINFO));

	int idx = m_PrList.GetItemCount();
	m_PrList.InsertItem(idx, proinfo.pName);
	CString PID;
	PID.Format(_T("%d"), proinfo.Pid);
	m_PrList.SetItemText(idx,1,PID);
	m_PrList.SetItemText(idx,2,proinfo.pPath);
	
}
void CProcess::OnNMRClickList3(NMHDR *pNMHDR, LRESULT *pResult)
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
	menu.LoadMenu(IDR_MEN_PROCESS);
	pMenu = menu.GetSubMenu(0);
	CPoint pt;
	GetCursorPos(&pt);

	pMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_TOPALIGN|TPM_RIGHTBUTTON, pt.x, pt.y, this);
	*pResult = 0;
}

void CProcess::OnProcessModule()
{
	// TODO: �ڴ���������������
	int idx = m_PrList.GetSelectionMark();
	wchar_t pid[20] = {0};
	m_PrList.GetItemText(idx, 1, pid, 20);
	pView->m_clientsock.MySendCommond(itemsSock, MYGETPROCESS, (char*)pid, 20, MODULELIST);

}

void CProcess::MyShowModule(COMMEND cmd)
{
	PROCESSINFO proinfo;
	ZeroMemory(&proinfo,sizeof(proinfo));
	memcpy(&proinfo,cmd.teste,sizeof(PROCESSINFO));

	/*wchar_t *pos = wcsrchr(tem, _T('\\'));
	pos++;*/

	int idx = m_ModuleList.GetItemCount();
	m_ModuleList.InsertItem(idx, proinfo.pName);
	m_ModuleList.SetItemText(idx, 1, proinfo.pPath);
}
void CProcess::OnCancel()
{
	// TODO: �ڴ����ר�ô����/����û���
	pView->m_Process = NULL;
	delete[] pView->m_Process;
	CDialog::OnCancel();
}

void CProcess::OnProcessBursh()
{
	// TODO: �ڴ���������������
	m_PrList.DeleteAllItems();
	pView->m_clientsock.MySendCommond(itemsSock, MYGETPROCESS, NULL, 0, PROCESSLIST);
}

void CProcess::OnProcessExitpro()
{
	// TODO: �ڴ���������������
	int idx = m_PrList.GetSelectionMark();
	wchar_t pid[20] = {0};
	m_PrList.GetItemText(idx, 1, pid, 20);
	pView->m_clientsock.MySendCommond(itemsSock, MYGETPROCESS, (char*)pid, 20, EXITPROCESS);
}
