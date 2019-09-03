// CreateServDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ServMgr.h"
#include "CreateServDlg.h"
#include "afxdialogex.h"


// CCreateServDlg �Ի���

IMPLEMENT_DYNAMIC(CCreateServDlg, CDialog)

CCreateServDlg::CCreateServDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCreateServDlg::IDD, pParent)
{

}

CCreateServDlg::~CCreateServDlg()
{
}

void CCreateServDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_START_TYPE, m_StartType);
}


BEGIN_MESSAGE_MAP(CCreateServDlg, CDialog)
	ON_BN_CLICKED(IDC_BROWSE, &CCreateServDlg::OnBnClickedBrowse)
	ON_BN_CLICKED(IDOK, &CCreateServDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CCreateServDlg ��Ϣ�������


void CCreateServDlg::OnBnClickedBrowse()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CFileDialog m_fileDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("All_Files(*.*)|*.*||"));
	if (m_fileDlg.DoModal() == IDOK)
	{
		SetDlgItemText(IDC_SERV_PATH, m_fileDlg.GetPathName());
	}
}


BOOL CCreateServDlg::OnInitDialog()
{
	CDialog::OnInitDialog();


	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_StartType.InsertString(0, _T("�Զ�"));
	m_StartType.InsertString(1, _T("�ֶ�"));
	m_StartType.InsertString(2, _T("�ѽ���"));
	return TRUE;  // return TRUE unless you set the focus to a control
}


void CCreateServDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);

	DWORD dwStartType = 0;
	CString strServName, strServDisp, strServPath, strServDesc, strStartType;
	GetDlgItemText(IDC_SERV_NAME, strServName);
	GetDlgItemText(IDC_SERV_DISP_NAME, strServDisp);
	GetDlgItemText(IDC_SERV_PATH, strServPath);
	GetDlgItemText(IDC_SERV_DES, strServDesc);
	GetDlgItemText(IDC_START_TYPE, strStartType);
	if (strServName.IsEmpty())
	{
		AfxMessageBox(_T("�������Ʋ���Ϊ�գ�"));
		return ;
	}
	if (strServDisp.IsEmpty())
	{
		AfxMessageBox(_T("��ʾ���Ʋ���Ϊ�գ�"));
		return ;
	}
	if (strServPath.IsEmpty())
	{
		AfxMessageBox(_T("����·������Ϊ�գ�"));
		return ;
	}
	if (strStartType.IsEmpty())
	{
		AfxMessageBox(_T("��ѡ���������ͣ�"));
		return ;
	}
	if (strStartType == _T("�Զ�"))
	{
		dwStartType = SERVICE_AUTO_START;
	}
	else if (strStartType == _T("�ֶ�"))
	{
		dwStartType = SERVICE_DEMAND_START;
	}
	else if(strStartType == _T("�ѽ���"))
	{
		dwStartType = SERVICE_DISABLED;
	}
	CServConfig m_ServCfg;
	if (m_ServCfg.CreateServ(strServName, strServPath, strServDisp, dwStartType, strServDesc))
	{
		AfxMessageBox(_T("��������ɹ���"));
	}
	else
	{
		AfxMessageBox(_T("��������ʧ�ܣ�"));
	}
	
	CDialog::OnOK();
}
