// SysSettingPort.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "RemoteClient.h"
#include "SysSettingPort.h"


// CSysSettingPort �Ի���

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


// CSysSettingPort ��Ϣ�������

void CSysSettingPort::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	UpdateData(TRUE);
	m_MyPort = m_LPort;
	AfxMessageBox(_T("���ö˿ڳɹ�!"));
}
