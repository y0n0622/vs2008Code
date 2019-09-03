// SysSettingPort.cpp : 实现文件
//

#include "stdafx.h"
#include "RemoteClient.h"
#include "SysSettingPort.h"


// CSysSettingPort 对话框

IMPLEMENT_DYNAMIC(CSysSettingPort, CDialog)

CSysSettingPort::CSysSettingPort(CWnd* pParent /*=NULL*/)
	: CDialog(CSysSettingPort::IDD, pParent)
	, m_LPort(6789)
{

}

CSysSettingPort::~CSysSettingPort()
{
}

void CSysSettingPort::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_LPort);
}


BEGIN_MESSAGE_MAP(CSysSettingPort, CDialog)
	ON_BN_CLICKED(IDOK, &CSysSettingPort::OnBnClickedOk)
END_MESSAGE_MAP()


// CSysSettingPort 消息处理程序

void CSysSettingPort::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码

	UpdateData(TRUE);
	m_MyPort = m_LPort;
	AfxMessageBox(_T("设置端口成功!"));
}
