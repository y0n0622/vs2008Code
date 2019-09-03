#pragma once
#include "afxcmn.h"
#include "resource.h"
#include "sys.h"


class CRemoteClientView;
// CProcess �Ի���

class CProcess : public CDialog
{
	DECLARE_DYNAMIC(CProcess)

public:
	CProcess(CWnd* pParent = NULL, SOCKET sk = NULL);   // ��׼���캯��
	virtual ~CProcess();

// �Ի�������
	enum { IDD = IDD_PROCESS };
    

	SOCKET itemsSock;
	CRemoteClientView *pView;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
