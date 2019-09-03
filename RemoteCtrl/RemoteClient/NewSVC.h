#pragma once
#include "SvcManage.h"
#include "afxwin.h"


class CRemoteClientView;

// CNewSVC 对话框

class CNewSVC : public CDialog
{
	DECLARE_DYNAMIC(CNewSVC)

public:
	CNewSVC(CWnd* pParent = NULL, SOCKET sk = NULL);   // 标准构造函数
	virtual ~CNewSVC();

// 对话框数据
	enum { IDD = IDD_SVCNEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_SVCName;
	CString m_SVCDisName;
	CString m_Discription;
	
	CString m_BinPath;
	CComboBox m_Start;
	CComboBox m_Status;
private:
	SOCKET AllSock;
	CRemoteClientView *pView;
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
