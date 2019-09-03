// CreateServDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ServMgr.h"
#include "CreateServDlg.h"
#include "afxdialogex.h"


// CCreateServDlg 对话框

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


// CCreateServDlg 消息处理程序


void CCreateServDlg::OnBnClickedBrowse()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog m_fileDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("All_Files(*.*)|*.*||"));
	if (m_fileDlg.DoModal() == IDOK)
	{
		SetDlgItemText(IDC_SERV_PATH, m_fileDlg.GetPathName());
	}
}


BOOL CCreateServDlg::OnInitDialog()
{
	CDialog::OnInitDialog();


	// TODO:  在此添加额外的初始化
	m_StartType.InsertString(0, _T("自动"));
	m_StartType.InsertString(1, _T("手动"));
	m_StartType.InsertString(2, _T("已禁用"));
	return TRUE;  // return TRUE unless you set the focus to a control
}


void CCreateServDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
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
		AfxMessageBox(_T("服务名称不能为空！"));
		return ;
	}
	if (strServDisp.IsEmpty())
	{
		AfxMessageBox(_T("显示名称不能为空！"));
		return ;
	}
	if (strServPath.IsEmpty())
	{
		AfxMessageBox(_T("程序路径不能为空！"));
		return ;
	}
	if (strStartType.IsEmpty())
	{
		AfxMessageBox(_T("请选择启动类型！"));
		return ;
	}
	if (strStartType == _T("自动"))
	{
		dwStartType = SERVICE_AUTO_START;
	}
	else if (strStartType == _T("手动"))
	{
		dwStartType = SERVICE_DEMAND_START;
	}
	else if(strStartType == _T("已禁用"))
	{
		dwStartType = SERVICE_DISABLED;
	}
	CServConfig m_ServCfg;
	if (m_ServCfg.CreateServ(strServName, strServPath, strServDisp, dwStartType, strServDesc))
	{
		AfxMessageBox(_T("创建服务成功！"));
	}
	else
	{
		AfxMessageBox(_T("创建服务失败！"));
	}
	
	CDialog::OnOK();
}
