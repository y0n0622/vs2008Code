// QQSendToolDemo.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CQQSendToolDemoApp:
// �йش����ʵ�֣������ QQSendToolDemo.cpp
//

class CQQSendToolDemoApp : public CWinApp
{
public:
	CQQSendToolDemoApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CQQSendToolDemoApp theApp;