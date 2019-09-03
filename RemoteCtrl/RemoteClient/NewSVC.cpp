// NewSVC.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "RemoteClient.h"
#include "NewSVC.h"
#include "RemoteClientView.h"
#include "MainFrm.h"


// CNewSVC �Ի���

IMPLEMENT_DYNAMIC(CNewSVC, CDialog)

CNewSVC::CNewSVC(CWnd* pParent /*=NULL*/, SOCKET sk)
	: CDialog(CNewSVC::IDD, pParent)
	, m_SVCName(_T(""))
	, m_SVCDisName(_T(""))
	, m_Discription(_T(""))
	, m_BinPath(_T(""))
{
	AllSock = sk;
}

CNewSVC::~CNewSVC()
{
}

void CNewSVC::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_SVCNAME, m_SVCName);
	DDX_Text(pDX, IDC_SVCDISNAME, m_SVCDisName);
	DDX_Text(pDX, IDC_SVCDISCREAPTION, m_Discription);

	DDX_Text(pDX, IDC_SVCPATH, m_BinPath);
	DDX_Control(pDX, IDC_COM_CURRENT, m_Start);
	DDX_Control(pDX, IDC_COM_STATUS, m_Status);
}


BEGIN_MESSAGE_MAP(CNewSVC, CDialog)
	ON_BN_CLICKED(IDOK, &CNewSVC::OnBnClickedOk)
END_MESSAGE_MAP()


// CNewSVC ��Ϣ�������



BOOL CNewSVC::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_Start.SetCurSel(2);
	m_Status.SetCurSel(0);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CNewSVC::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	if (m_SVCName.IsEmpty() || m_BinPath.IsEmpty())
	{
		AfxMessageBox(_T("������ʵ���ƻ�·������Ϊ��!"));
		return ;
	}
	if (m_SVCDisName.IsEmpty())
	{
		m_SVCDisName = m_SVCName;
	}
	if (m_BinPath.IsEmpty())
	{
		m_BinPath = m_SVCName;
	}

	SVICELISTINFO svcinfo;
	ZeroMemory(&svcinfo, sizeof(SVICELISTINFO));
	wsprintfW(svcinfo.ServiceName, _T("%s"), m_SVCName);
	wsprintfW(svcinfo.DisplayName, _T("%s"), m_SVCDisName);
	wsprintfW(svcinfo.Description, _T("%s"), m_Discription);
	wsprintfW(svcinfo.BinPath, _T("%s"), m_BinPath);

	DWORD stype;
	switch (m_Start.GetCurSel())
	{
	case 0:
		stype = SERVICE_DEMAND_START;
		break;
	case 1:
		stype = SERVICE_DISABLED;
		break;
	case 2:
		stype = SERVICE_AUTO_START;
		break;
	default:
		break;
	}
	svcinfo.stype = stype;

	DWORD Curr = 0;
	switch (m_Status.GetCurSel())
	{
	case 0:
		Curr = SERVICE_RUNNING;
		break;
	case 1:
		Curr = SERVICE_STOP;
		break;
	default:
		break;
	}
	svcinfo.CurrentStatus = Curr;

	CMainFrame *pFram = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	pView = (CRemoteClientView *)pFram->GetActiveView();

	pView->m_clientsock.MySendCommond(AllSock, MYSVCMANAGE, (char*)&svcinfo, sizeof(SVICELISTINFO), SVCCREATE);

	//OnOK();
}