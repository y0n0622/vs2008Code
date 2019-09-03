// BrowserFrame.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "BrowserFrame.h"
#include "BrowserFrameDlg.h"
#include "MainFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CBrowserFrameApp

BEGIN_MESSAGE_MAP(CBrowserFrameApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CBrowserFrameApp 构造

CBrowserFrameApp::CBrowserFrameApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CBrowserFrameApp 对象

CBrowserFrameApp theApp;


// CBrowserFrameApp 初始化

BOOL CBrowserFrameApp::InitInstance()
{
	CWinApp::InitInstance();
	
	CMainFrame *pMainFrame = new CMainFrame();
	pMainFrame->CreateEx(0, _T("MAINFRAME"),
		_T("自绘浏览器界面 by Fzy v1.0"),
		WS_POPUPWINDOW | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_THICKFRAME,
		CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, NULL);
	m_pMainWnd = pMainFrame;
	pMainFrame->CenterWindow();
	pMainFrame->ShowWindow(SW_SHOW);
	pMainFrame->UpdateWindow();
	return TRUE;
}


int CBrowserFrameApp::ExitInstance()
{
	// TODO: 在此添加专用代码和/或调用基类
	//delete m_pMainWnd;
	return CWinApp::ExitInstance();
}
