
// ServMgr.h : ServMgr Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CServMgrApp:
// �йش����ʵ�֣������ ServMgr.cpp
//

class CServMgrApp : public CWinApp
{
public:
	CServMgrApp();


// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CServMgrApp theApp;
