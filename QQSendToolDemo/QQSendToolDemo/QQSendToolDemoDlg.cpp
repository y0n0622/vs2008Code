// QQSendToolDemoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "QQSendToolDemo.h"
#include "QQSendToolDemoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CQQSendToolDemoDlg 对话框




CQQSendToolDemoDlg::CQQSendToolDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CQQSendToolDemoDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	
}

void CQQSendToolDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CQQSendToolDemoDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_QQMESSAGE, &CQQSendToolDemoDlg::OnBnClickedQqmessage)
	ON_BN_CLICKED(IDC_STOPSEND, &CQQSendToolDemoDlg::OnBnClickedStopsend)
END_MESSAGE_MAP()


// CQQSendToolDemoDlg 消息处理程序

BOOL CQQSendToolDemoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CQQSendToolDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CQQSendToolDemoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CQQSendToolDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
 
BOOL CALLBACK EnumWindowsProc(HWND hWnd, LPARAM lParam)
{
	//char strWindowName[MAXBYTE] = {0};
	//char strClassName[MAXBYTE] = {0};
	//GetWindowTextA(hWnd, strWindowName, MAXBYTE);
	//GetClassNameA(hWnd, strClassName, MAXBYTE);
	//if (0 == strcmp("ForegroundStaging", strClassName))
	//{
	//	if (0 != strcmp(strWindowName, "QQ") /*&& 0 != strcmp(strWindowName, "")*/)
	//	{
			::SendMessage(hWnd, WM_PASTE, 0, 0);
			::SendMessage(hWnd, WM_KEYDOWN, VK_CONTROL, 0);
			::SendMessage(hWnd, WM_KEYDOWN, VK_RETURN, 0);
			//::SendMessage(hWnd, WM_CLOSE, 0, 0);
	//	}
		
	//}
	return TRUE;
}
static BOOL bFlag = TRUE;
UINT __cdecl MyThreadFuction(LPVOID pParam)
{
	HWND hWnd = ::FindWindow(_T("TXGuiFoundation"), _T("QQ"));
	if (hWnd)
	{
		//int number = 0;
		::SendMessage(hWnd, WM_KEYDOWN, VK_DOWN, 0);
		::SendMessage(hWnd, WM_KEYDOWN, VK_RETURN, 0);
		
		while (TRUE)
		{
			EnumWindows(EnumWindowsProc, NULL);
			Sleep(10);
			if (bFlag == FALSE)
			{
				break;
			}
			/*number++;
			if (number == 5)
			{
				break;
			}*/
		}
		AfxMessageBox(_T("轰炸停止!"));
	}
	
	return 0;
}
void CQQSendToolDemoDlg::OnBnClickedQqmessage()
{
	// TODO: 在此添加控件通知处理程序代码
	AfxBeginThread(MyThreadFuction, this);
	
	
	/*if (hWnd)
	{
		::SendMessage(hWnd, WM_PASTE, 0, 0);
		::SendMessage(hWnd, WM_KEYDOWN, VK_RETURN, 0);
	}*/
}

void CQQSendToolDemoDlg::OnBnClickedStopsend()
{
	// TODO: 在此添加控件通知处理程序代码
	bFlag = FALSE;
	AfxEndThread(0);
}
