// RemoteClient.h : RemoteClient Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CRemoteClientApp:
// �йش����ʵ�֣������ RemoteClient.cpp
//

class CRemoteClientApp : public CWinApp
{
public:
	CRemoteClientApp();


// ��д
public:
	virtual BOOL InitInstance();

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CRemoteClientApp theApp;