// 360demoDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "360demo.h"
#include "360demoDlg.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMy360demoDlg �Ի���




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


// CMy360demoDlg ��Ϣ�������

BOOL CMy360demoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMy360demoDlg::OnPaint()
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
HCURSOR CMy360demoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


LRESULT CMy360demoDlg::OnNcHitTest(CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

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
	font.CreatePointFont(100, _T("����"));
	CFont *oldfont = pDC->SelectObject(&font);
	pDC->SetTextColor(RGB(255, 255, 255));
	pDC->TextOutW(40, 10, _T("360��ȫ��ʿ�����Ի� By Fzy"));

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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	EndDialog(TRUE);
}

void CMy360demoDlg::OnBnClickedBtnMin()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	ShowWindow(SW_MINIMIZE);
}
