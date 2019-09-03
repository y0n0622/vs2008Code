#pragma once
#include "CmdShell.h"

// CDirCreate 对话框
class CFileManage;
class CRemoteClientView;
class CDirCreate : public CDialog
{
	DECLARE_DYNAMIC(CDirCreate)

public:
	CDirCreate(CWnd* pParent = NULL, SOCKET sk = NULL, CString temp=_T(""));   // 标准构造函数
	virtual ~CDirCreate();

// 对话框数据
	enum { IDD = IDD_DIRCREATE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
private:
	CFileManage *m_FileManage;
	SOCKET AllSock;
	CRemoteClientView *pView;
public:
	CString m_DirName;
	CString tem;
};
