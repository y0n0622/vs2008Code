#pragma once
#include "resource.h"
#include "sys.h"
#include "ClientSock.h"
#include "afxwin.h"

class CRemoteClientView;

// CCmdShell �Ի���

class CCmdShell : public CDialog
{
	DECLARE_DYNAMIC(CCmdShell)

public:
	CCmdShell(CWnd* pParent = NULL, SOCKET sk = NULL);   // ��׼���캯��
	virtual ~CCmdShell();

// �Ի�������
	enum { IDD = IDD_CMDSHELL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()


private:
	CRemoteClientView *pView;
	SOCKET AllSock;
	void MyShowText(COMMEND cmd);
public:
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	CEdit m_CmdShow;
	void CmdCommend(COMMEND cmd);
	
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
