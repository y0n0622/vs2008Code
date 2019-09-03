// BrowserFrame.cpp : ����Ӧ�ó��������Ϊ��
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


// CBrowserFrameApp ����

CBrowserFrameApp::CBrowserFrameApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CBrowserFrameApp ����

CBrowserFrameApp theApp;


// CBrowserFrameApp ��ʼ��

BOOL CBrowserFrameApp::InitInstance()
{
	CWinApp::InitInstance();
	
	CMainFrame *pMainFrame = new CMainFrame();
	pMainFrame->CreateEx(0, _T("MAINFRAME"),
		_T("�Ի���������� by Fzy v1.0"),
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
	// TODO: �ڴ����ר�ô����/����û���
	//delete m_pMainWnd;
	return CWinApp::ExitInstance();
}
