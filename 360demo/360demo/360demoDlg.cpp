// 360demoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "360demo.h"
#include "360demoDlg.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMy360demoDlg 对话框




CMy360demoDlg::CMy360demoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMy360demoDlg::IDD, pParent)
	, m_min(IDB_MIN)
	, m_close(IDB_CLOSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_LOGO);
}

void CMy360demoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_MIN, m_min);
	DDX_Control(pDX, IDC_BTN_CLOSE, m_close);
}

BEGIN_MESSAGE_MAP(CMy360demoDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_NCHITTEST()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BTN_CLOSE, &CMy360demoDlg::OnBnClickedBtnClose)
	ON_BN_CLICKED(IDC_BTN_MIN, &CMy360demoDlg::OnBnClickedBtnMin)
END_MESSAGE_MAP()


// CMy360demoDlg 消息处理程序

BOOL CMy360demoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMy360demoDlg::OnPaint()
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
HCURSOR CMy360demoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


LRESULT CMy360demoDlg::OnNcHitTest(CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	UINT nHitTest = CDialog::OnNcHitTest(point);
	CRect rect;
	GetClientRect(&rect);
	rect.bottom = 390;
	ScreenToClient(&point);
	if(rect.PtInRect(point))
		if (nHitTest == HTCLIENT)
			nHitTest = HTCAPTION;
	
	return nHitTest;
}

BOOL CMy360demoDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CRect rect, captionRect, clientRect;
	GetClientRect(&rect);
	captionRect = clientRect = rect;
	captionRect.bottom = 390;
	clientRect.top = 390;
	pDC->FillSolidRect(&captionRect, RGB(61, 140, 61));
	pDC->FillSolidRect(&clientRect, RGB(255, 255, 255));
	pDC->DrawIcon(3, 1, m_hIcon);
	pDC->SetBkMode(TRANSPARENT);
	CFont font;
	font.CreatePointFont(100, _T("宋体"));
	CFont *oldfont = pDC->SelectObject(&font);
	pDC->SetTextColor(RGB(255, 255, 255));
	pDC->TextOutW(40, 10, _T("360安全卫士界面自绘 By Fzy"));

	CBitmap bmp_arrow;
	bmp_arrow.LoadBitmap(IDB_ARROW);

	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	memDC.SelectObject(&bmp_arrow);
	pDC->TransparentBlt(216, 9, 15, 15, &memDC, 0, 0, 15, 15, RGB(61, 140, 61));

	return TRUE;
	//return CDialog::OnEraseBkgnd(pDC);
}

void CMy360demoDlg::OnBnClickedBtnClose()
{
	// TODO: 在此添加控件通知处理程序代码
	EndDialog(TRUE);
}

void CMy360demoDlg::OnBnClickedBtnMin()
{
	// TODO: 在此添加控件通知处理程序代码
	ShowWindow(SW_MINIMIZE);
}
