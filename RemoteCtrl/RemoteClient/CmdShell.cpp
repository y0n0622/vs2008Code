// CmdShell.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "RemoteClient.h"
#include "CmdShell.h"
#include "RemoteClientView.h"


// CCmdShell �Ի���

IMPLEMENT_DYNAMIC(CCmdShell, CDialog)

CCmdShell::CCmdShell(CWnd* pParent /*=NULL*/, SOCKET sk)
	: CDialog(CCmdShell::IDD, pParent)
{
	pView = (CRemoteClientView *)pParent;
	AllSock = sk;
}

CCmdShell::~CCmdShell()
{
}

void CCmdShell::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CMDSHOW, m_CmdShow);
}


BEGIN_MESSAGE_MAP(CCmdShell, CDialog)
END_MESSAGE_MAP()


// CCmdShell ��Ϣ�������

void CCmdShell::OnCancel()
{
	// TODO: �ڴ����ר�ô����/����û���
	pView->m_clientsock.MySendCommond(AllSock, MYCMDSHELL, "exit", 1024, WRITECMD);
	pView->m_CmdShell = NULL;
	delete[]pView->m_CmdShell;
	CDialog::OnCancel();
}

BOOL CCmdShell::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	pView->m_clientsock.MySendCommond(AllSock, MYCMDSHELL, NULL, 0, STARTCMD);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CCmdShell::CmdCommend(COMMEND cmd)
{
	switch (cmd.fID)
	{
	case STARTCMD:
		MyShowText(cmd);
		break;
	case STARTCMDERROR:
		AfxMessageBox(_T("��cmd����ʧ��!"));
		break;
	
	default:
		break;
	}
}


void CCmdShell::MyShowText(COMMEND cmd)
{
	CString temp = pView->m_some.MyCharToWide(cmd.teste);
	m_CmdShow.SetSel(-1, -1);
	//wchar_t tText[2048] = {0};
	//m_CmdShow.GetWindowText(tText, 2048);
	m_CmdShow.ReplaceSel(temp);
	//temp.Format(_T("%s\r\n%s"), tText, temp);
	//m_CmdShow.SetWindowText(temp);
}
//������Ϣ�ַ���
BOOL CCmdShell::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	char sTemp[1024]={0};
	wchar_t buf[1024]={0};
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN)
		{
			int idx = m_CmdShow.GetLineCount() - 1;
			//wchar_t buf[1024] = {0};
			m_CmdShow.GetLine(idx, buf, 1024);
			wchar_t *pos = wcsrchr(buf, _T('>'));
			pos++;
			//pView->m_some.MyCharToWide(pos);
			//char sTemp[1024] = {0};
			wsprintfA(sTemp, "%s", pView->m_some.MyWideToChar(pos));
			pView->m_clientsock.MySendCommond(AllSock, MYCMDSHELL, sTemp, 1024, WRITECMD);
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}
