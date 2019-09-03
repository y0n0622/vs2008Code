#pragma once
#include "afxwin.h"
#include "ServConfig.h"

// CCreateServDlg 对话框

class CCreateServDlg : public CDialog
{
	DECLARE_DYNAMIC(CCreateServDlg)

public:
	CCreateServDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCreateServDlg();

// 对话框数据
	enum { IDD = IDD_CREATE_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBrowse();
	virtual BOOL OnInitDialog();
	CComboBox m_StartType;
	afx_msg void OnBnClickedOk();
};
