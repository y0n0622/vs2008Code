// BrowserFrame.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CBrowserFrameApp:
// �йش����ʵ�֣������ BrowserFrame.cpp
//

class CBrowserFrameApp : public CWinApp
{
public:
	CBrowserFrameApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
	
	virtual int ExitInstance();
};

extern CBrowserFrameApp theApp;