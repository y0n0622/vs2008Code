#pragma once
#include "CmdShell.h"
#include "afxcmn.h"
#include "FileSock.h"


class CRemoteClientView;
// CFileManage 对话框

class CFileManage : public CDialog
{
	DECLARE_DYNAMIC(CFileManage)

public:
	CFileManage(CWnd* pParent = NULL, SOCKET sk = NULL);   // 标准构造函数
	virtual ~CFileManage();

// 对话框数据
	enum { IDD = IDD_FILEMANEGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

private:
	SOCKET AllSock;
	
	CImageList m_ImageList;
	HTREEITEM hNgrow;
	int int_hard;
	int int_dvd;
	int int_upan;
	int int_file;
	int int_net;
	int int_fly;

	HTREEITEM hItem;
public:
	virtual BOOL OnInitDialog();

	virtual void OnCancel();
public:
	
	CString m_FilePath;
	CTreeCtrl m_Tree;
	CListCtrl m_FileList;
	CProgressCtrl m_Prograss;
	CRemoteClientView *pView;
	BOOL bl;
	void MyFileCommend(COMMEND cmd);
	void MyDrive(COMMEND cmd);
	void MyGetFile(COMMEND cmd);
	void MyGetPath(HTREEITEM htree, CString tem);
	void MyDeleteC(HTREEITEM htree);
	afx_msg void OnNMClickTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnFilemenuRun();
	afx_msg void OnFilemenuDel();
	afx_msg void OnNMRClickList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnFilemenuCreatedir();
	afx_msg void OnFilemenuDeldir();
	afx_msg void OnFilemenuDown();
	afx_msg void OnFilemenuUp();

	CFileSock m_FileSock;
	afx_msg void OnFilemenuBrush();
	afx_msg void OnBnClickedButtonCansend();
};
