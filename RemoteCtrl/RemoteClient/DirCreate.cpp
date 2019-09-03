// DirCreate.cpp : 实现文件
//

#include "stdafx.h"
#include "RemoteClient.h"
#include "DirCreate.h"
#include "FileManage.h"
#include "RemoteClientView.h"



// CDirCreate 对话框

IMPLEMENT_DYNAMIC(CDirCreate, CDialog)

CDirCreate::CDirCreate(CWnd* pParent /*=NULL*/, SOCKET sk, CString temp)
	: CDialog(CDirCreate::IDD, pParent)
	, m_DirName(_T(""))
{
	AllSock = sk;
	pView = (CRemoteClientView *)pParent;
	tem = temp;
}

CDirCreate::~CDirCreate()
{
}

void CDirCreate::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_DirName);
}


BEGIN_MESSAGE_MAP(CDirCreate, CDialog)
	ON_BN_CLICKED(IDOK, &CDirCreate::OnBnClickedOk)
END_MESSAGE_MAP()


// CDirCreate 消息处理程序

void CDirCreate::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	m_DirName = tem+m_DirName;
	pView->m_clientsock.MySendCommond(AllSock,MYFILEMANAGE,(char*)m_DirName.GetBuffer(MAX_PATH),MAX_PATH,DIRCREATE);
	OnOK();
}
