#pragma once
#include "afxwin.h"
#include "ServConfig.h"

// CCreateServDlg �Ի���

class CCreateServDlg : public CDialog
{
	DECLARE_DYNAMIC(CCreateServDlg)

public:
	CCreateServDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCreateServDlg();

// �Ի�������
	enum { IDD = IDD_CREATE_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBrowse();
	virtual BOOL OnInitDialog();
	CComboBox m_StartType;
	afx_msg void OnBnClickedOk();
};
