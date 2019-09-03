
// ServMgrView.h : CServMgrView ��Ľӿ�
//

#pragma once
#include "ServConfig.h"
#include "CreateServDlg.h"

class CServMgrView : public CListView
{
protected: // �������л�����
	CServMgrView();
	DECLARE_DYNCREATE(CServMgrView)

// ����
public:
	CServMgrDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // ������һ�ε���

// ʵ��
public:
	virtual ~CServMgrView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
public:

	afx_msg void OnNMRClick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnStartService();
	afx_msg void OnStopService();
	afx_msg void OnPauseService();
	afx_msg void OnContinueService();
	afx_msg void OnRAuto();
	afx_msg void OnRDemand();
	afx_msg void OnRDisable();
	afx_msg void OnRDelServ();
	afx_msg void OnRAddServ();
	afx_msg void OnRRefresh();
	CServItem *m_pServList;
	afx_msg void OnDestroy();
	afx_msg void OnRRun();
};

#ifndef _DEBUG  // ServMgrView.cpp �еĵ��԰汾
inline CServMgrDoc* CServMgrView::GetDocument() const
   { return reinterpret_cast<CServMgrDoc*>(m_pDocument); }
#endif

