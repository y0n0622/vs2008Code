#pragma once
#include "CmdShell.h"

// CDirCreate �Ի���
class CFileManage;
class CRemoteClientView;
class CDirCreate : public CDialog
{
	DECLARE_DYNAMIC(CDirCreate)

public:
	CDirCreate(CWnd* pParent = NULL, SOCKET sk = NULL, CString temp=_T(""));   // ��׼���캯��
	virtual ~CDirCreate();

// �Ի�������
	enum { IDD = IDD_DIRCREATE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
