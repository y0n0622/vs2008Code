#pragma once
#include "resource.h"
#include "CmdShell.h"
#include "afxcmn.h"
#include "Winsvc.h"
class CRemoteClientView;
class CNewSVC;
// CSvcManage 对话框

class CSvcManage : public CDialog
{
	DECLARE_DYNAMIC(CSvcManage)

public:
	CSvcManage(CWnd* pParent = NULL, SOCKET sk = NULL);   // 标准构造函数
	virtual ~CSvcManage();



public:



// 对话框数据
	enum { IDD = IDD_SVNMANAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	virtual void OnCancel();
	
	void MySvcCommend(COMMEND cmd);
	CString MyGetCurrentState(DWORD State);
	CString MyGetStartType(DWORD StartType);
private:
	SOCKET AllSock;
	CRemoteClientView *pView;
	CNewSVC *m_NewSvc;
	void ShowSVCList(COMMEND cmd);

public:
	CListCtrl m_SvcList;
	afx_msg void OnSvcstart();
	afx_msg void OnNMRClickListSvc(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBursh();
	afx_msg void OnStopsvc();
	afx_msg void OnAuto();
	afx_msg void OnBoot();
	afx_msg void OnDisable();
	afx_msg void OnSvcdelete();
	afx_msg void OnCreatesvc();
};
