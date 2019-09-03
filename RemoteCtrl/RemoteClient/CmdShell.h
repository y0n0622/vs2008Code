#pragma once
#include "resource.h"
#include "sys.h"
#include "ClientSock.h"
#include "afxwin.h"

class CRemoteClientView;

// CCmdShell 对话框

class CCmdShell : public CDialog
{
	DECLARE_DYNAMIC(CCmdShell)

public:
	CCmdShell(CWnd* pParent = NULL, SOCKET sk = NULL);   // 标准构造函数
	virtual ~CCmdShell();

// 对话框数据
	enum { IDD = IDD_CMDSHELL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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
