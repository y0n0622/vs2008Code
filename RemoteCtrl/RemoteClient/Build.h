#pragma once


// CBuild 对话框

class CBuild : public CDialog
{
	DECLARE_DYNAMIC(CBuild)

public:
	CBuild(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CBuild();

// 对话框数据
	enum { IDD = IDD_BUILD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CString m_IP;
	UINT m_Port;
	afx_msg void OnBnClickedOk2();
};
