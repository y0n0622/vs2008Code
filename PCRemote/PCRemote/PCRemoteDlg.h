// PCRemoteDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"


// CPCRemoteDlg �Ի���
class CPCRemoteDlg : public CDialog
{
// ����
public:
	CPCRemoteDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_PCREMOTE_DIALOG };

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
	CListCtrl m_CList_Online;
	CListCtrl m_CList_Message;
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
