// 360demoDlg.h : ͷ�ļ�
//

#pragma once
#include "uibutton.h"


// CMy360demoDlg �Ի���
class CMy360demoDlg : public CDialog
{
// ����
public:
	CMy360demoDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MY360DEMO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnBnClickedBtnClose();
	afx_msg void OnBnClickedBtnMin();
	CUIButton m_min;
	CUIButton m_close;
};
