#pragma once


// CBuild �Ի���

class CBuild : public CDialog
{
	DECLARE_DYNAMIC(CBuild)

public:
	CBuild(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CBuild();

// �Ի�������
	enum { IDD = IDD_BUILD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CString m_IP;
	UINT m_Port;
	afx_msg void OnBnClickedOk2();
};
