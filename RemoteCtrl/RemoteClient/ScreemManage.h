#pragma once
#include "CmdShell.h"
#include "FileSock.h"

// CScreemManage �Ի���

class CRemoteClientView;

class CScreemManage : public CDialog
{
	DECLARE_DYNAMIC(CScreemManage)

public:
	CScreemManage(CWnd* pParent = NULL, SOCKET sk = NULL);   // ��׼���캯��
	virtual ~CScreemManage();

// �Ի�������
	enum { IDD = IDD_SCREEM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

private:
	SOCKET AllSock;
	CRemoteClientView *pView;
	SOCKET fSock;
	CWinThread* pWin;
	CFileSock m_FileSock;
	static UINT __cdecl MyDCFunction(LPVOID lp);
	void MyShowPic(CString pPath);
public:
	virtual BOOL OnInitDialog();

	virtual void OnCancel();
	void MyStartDC(SOCKET sk);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
