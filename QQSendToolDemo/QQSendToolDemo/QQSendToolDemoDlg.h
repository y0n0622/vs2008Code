// QQSendToolDemoDlg.h : ͷ�ļ�
//

#pragma once


// CQQSendToolDemoDlg �Ի���
class CQQSendToolDemoDlg : public CDialog
{
// ����
public:
	CQQSendToolDemoDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_QQSENDTOOLDEMO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedQqmessage();
	afx_msg void OnBnClickedStopsend();
	
	//static UINT __cdecl MyThreadFuction(LPVOID pParam);
};
