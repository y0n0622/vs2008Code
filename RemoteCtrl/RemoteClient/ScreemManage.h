#pragma once
#include "CmdShell.h"
#include "FileSock.h"

// CScreemManage 对话框

class CRemoteClientView;

class CScreemManage : public CDialog
{
	DECLARE_DYNAMIC(CScreemManage)

public:
	CScreemManage(CWnd* pParent = NULL, SOCKET sk = NULL);   // 标准构造函数
	virtual ~CScreemManage();

// 对话框数据
	enum { IDD = IDD_SCREEM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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
