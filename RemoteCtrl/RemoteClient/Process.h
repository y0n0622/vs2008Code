#pragma once
#include "afxcmn.h"
#include "resource.h"
#include "sys.h"


class CRemoteClientView;
// CProcess 对话框

class CProcess : public CDialog
{
	DECLARE_DYNAMIC(CProcess)

public:
	CProcess(CWnd* pParent = NULL, SOCKET sk = NULL);   // 标准构造函数
	virtual ~CProcess();

// 对话框数据
	enum { IDD = IDD_PROCESS };
    

	SOCKET itemsSock;
	CRemoteClientView *pView;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CListCtrl m_PrList;
	CListCtrl m_ModuleList;
public:
	virtual BOOL OnInitDialog();
	void ProcessCommend(COMMEND cmd);

private:
	void MyShowProList(COMMEND cmd);
	void MyShowModule(COMMEND cmd);
public:
	afx_msg void OnNMRClickList3(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnProcessModule();
public:
	virtual void OnCancel();
	afx_msg void OnProcessBursh();
	afx_msg void OnProcessExitpro();
};
