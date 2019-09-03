// QQSendToolDemoDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "QQSendToolDemo.h"
#include "QQSendToolDemoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CQQSendToolDemoDlg �Ի���




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


// CQQSendToolDemoDlg ��Ϣ�������

BOOL CQQSendToolDemoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CQQSendToolDemoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
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
		AfxMessageBox(_T("��ըֹͣ!"));
	}
	
	return 0;
}
void CQQSendToolDemoDlg::OnBnClickedQqmessage()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	AfxBeginThread(MyThreadFuction, this);
	
	
	/*if (hWnd)
	{
		::SendMessage(hWnd, WM_PASTE, 0, 0);
		::SendMessage(hWnd, WM_KEYDOWN, VK_RETURN, 0);
	}*/
}

void CQQSendToolDemoDlg::OnBnClickedStopsend()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	bFlag = FALSE;
	AfxEndThread(0);
}
