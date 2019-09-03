// FileManage.cpp : 实现文件
//

#include "stdafx.h"
#include "RemoteClient.h"
#include "FileManage.h"
#include "RemoteClientView.h"
#include "DirCreate.h"

// CFileManage 对话框

IMPLEMENT_DYNAMIC(CFileManage, CDialog)

CFileManage::CFileManage(CWnd* pParent /*=NULL*/, SOCKET sk)
	: CDialog(CFileManage::IDD, pParent)
	, m_FilePath(_T(""))
{
	AllSock = sk;
	pView = (CRemoteClientView*)pParent;

	int_hard = -1;
	int_dvd = -1;
	int_upan = -1;
	int_file = -1;
	int_net = -1;
	int_fly=-1;
	bl = TRUE;
}

CFileManage::~CFileManage()
{
}

void CFileManage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_FILEPATH, m_FilePath);
	DDX_Control(pDX, IDC_TREE, m_Tree);
	DDX_Control(pDX, IDC_LIST, m_FileList);
	DDX_Control(pDX, IDC_PROGRESS1, m_Prograss);
}


BEGIN_MESSAGE_MAP(CFileManage, CDialog)
	
	ON_NOTIFY(NM_CLICK, IDC_TREE, &CFileManage::OnNMClickTree)
	ON_COMMAND(ID_FILEMENU_RUN, &CFileManage::OnFilemenuRun)
	ON_COMMAND(ID_FILEMENU_DEL, &CFileManage::OnFilemenuDel)
	ON_NOTIFY(NM_RCLICK, IDC_LIST, &CFileManage::OnNMRClickList)
	ON_COMMAND(ID_FILEMENU_CREATEDIR, &CFileManage::OnFilemenuCreatedir)
	ON_COMMAND(ID_FILEMENU_DELDIR, &CFileManage::OnFilemenuDeldir)
	ON_COMMAND(ID_FILEMENU_DOWN, &CFileManage::OnFilemenuDown)
	ON_COMMAND(ID_FILEMENU_UP, &CFileManage::OnFilemenuUp)
	ON_COMMAND(ID_FILEMENU_BRUSH, &CFileManage::OnFilemenuBrush)
	ON_BN_CLICKED(IDC_BUTTON_CANSEND, &CFileManage::OnBnClickedButtonCansend)
END_MESSAGE_MAP()


// CFileManage 消息处理程序

BOOL CFileManage::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_ImageList.Create(32, 32, ILC_COLOR32,	20, 20);

	HICON ico_cmp = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON_CMP));
	int int_cmp = m_ImageList.Add(ico_cmp);
	HICON ico_hard = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON_HARDISK));
	int_hard = m_ImageList.Add(ico_hard);
	HICON ico_dvd = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON_DVD));
	int_dvd = m_ImageList.Add(ico_dvd);
	HICON ico_upan = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON_UPAN));
	int_upan = m_ImageList.Add(ico_upan);
	HICON ico_file = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON_FILE));
	int_file = m_ImageList.Add(ico_file);
	HICON ico_net = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON_NETDISK));
	int_net = m_ImageList.Add(ico_net);
	HICON ico_flp = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON_FLOPY));
	int_fly = m_ImageList.Add(ico_flp);
	

	m_Tree.SetImageList(&m_ImageList, TVSIL_NORMAL);
	m_FileList.SetImageList(&m_ImageList, LVSIL_NORMAL);
	hNgrow = m_Tree.InsertItem(_T("远程电脑"), int_cmp, int_cmp, NULL, NULL);
	
	

	pView->m_clientsock.MySendCommond(AllSock, MYFILEMANAGE, NULL, 0, FILEDRIVER);
	m_FileSock.MyFSockStart();//文件套接字
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CFileManage::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	pView->m_FileManage = NULL;
	delete pView->m_FileManage;
	CDialog::OnCancel();
}



void CFileManage::MyFileCommend(COMMEND cmd)
{
	switch (cmd.fID)
	{
		case FILEDRIVER:
			MyDrive(cmd);
			break;
		case FILEGET:
			MyGetFile(cmd);
			break;
		case MYSTARTCHECK:
			bl = TRUE;
			break;
		default:
			break;
	}
}

void CFileManage::MyDrive(COMMEND cmd)
{
	DRIVEINFO dinfo;
	ZeroMemory(&dinfo, sizeof(DRIVEINFO));
	memcpy(&dinfo, cmd.teste, sizeof(DRIVEINFO));
	CString tem;
	int imgID;
	switch (dinfo.dType)
	{
	case DRIVE_REMOVABLE:
		tem = dinfo.Drive;
		if (tem==_T("A:")||tem==_T("B:"))
		{
			imgID = int_fly;
		}
		else
		{
			imgID = int_upan;
		}		
		break;
	case DRIVE_FIXED:
		imgID = int_hard;
		break;
	case DRIVE_REMOTE:
		imgID = int_net;
		break;
	case DRIVE_CDROM:
		imgID = int_dvd;
		break;
	default:
		break;

	}
	m_Tree.InsertItem(dinfo.Drive, imgID, imgID, hNgrow, NULL);
	m_Tree.Expand(hNgrow,TVE_EXPAND);
}

void CFileManage::OnNMClickTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	if (bl == FALSE)
	{
		return;
	}
	UpdateData(TRUE);
	m_FileList.DeleteAllItems();
	NM_TREEVIEW *TREEVIEW = (NM_TREEVIEW *)pNMHDR;
	TVITEM m_item = TREEVIEW->itemNew;
	hItem = m_item.hItem;
	if (hItem == hNgrow)
	{
		return;
	}
	HTREEITEM htreeitem = m_Tree.GetSelectedItem();
	CString tem = m_Tree.GetItemText(htreeitem);
	if (tem == _T(""))
	{
		return;
	}
	bl = FALSE;
	MyDeleteC(htreeitem);
	MyGetPath(htreeitem, tem);
	pView->m_clientsock.MySendCommond(AllSock, MYFILEMANAGE, (char*)m_FilePath.GetBuffer(MAX_PATH), MAX_PATH, FILEGET);
	*pResult = 0;
}

//获取文件和文件夹，文件夹不显示。需要分析此函数
void CFileManage::MyGetFile(COMMEND cmd)
{
	WIN32_FIND_DATA wfd;
	ZeroMemory(&wfd, sizeof(WIN32_FIND_DATA));
	memcpy(&wfd, cmd.teste, sizeof(WIN32_FIND_DATA));
	if (wfd.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY || wfd.dwFileAttributes == 17 || wfd.dwFileAttributes == 18 || wfd.dwFileAttributes == 22)
	{
		
		if (_wcsicmp(wfd.cFileName,_T("."))==0||_wcsicmp(wfd.cFileName,_T(".."))==0)
		{
			return;
		}
		m_Tree.InsertItem(wfd.cFileName, int_file, int_file, hItem, NULL);
		m_Tree.Expand(hItem, TVE_EXPAND);
	}
	else
	{
		SHFILEINFO sho;
		ZeroMemory(&sho, sizeof(SHFILEINFO));
		SHGetFileInfo(wfd.cFileName, NULL, &sho, sizeof(SHFILEINFO), SHGFI_ICON|SHGFI_USEFILEATTRIBUTES);
		int image = m_ImageList.Add(sho.hIcon);
		int idx = m_FileList.GetItemCount();
		m_FileList.InsertItem(idx, wfd.cFileName, image);
	}
}
void CFileManage::MyGetPath(HTREEITEM htree, CString tem)
{
	SetDlgItemText(IDC_FILEPATH,_T(""));
	CString path;
	CString temp = tem;
	while (htree != hNgrow)
	{
		temp = m_Tree.GetItemText(htree);
		path = temp + _T("\\") + path;
		htree = m_Tree.GetParentItem(htree);
	}
	m_FilePath = path;
	UpdateData(FALSE);
}

void CFileManage::MyDeleteC(HTREEITEM htree)
{

	while (NULL != m_Tree.GetChildItem(htree))
	{
		m_Tree.DeleteItem(m_Tree.GetChildItem(htree));
	}
}
void CFileManage::OnFilemenuRun()
{
	// TODO: 在此添加命令处理程序代码
	int idx = m_FileList.GetSelectionMark();
	CString tem = m_FileList.GetItemText(idx, 0);
	tem = m_FilePath + tem;
	pView->m_clientsock.MySendCommond(AllSock, MYFILEMANAGE, (char*)tem.GetBuffer(MAX_PATH), MAX_PATH, MYRUNFILE);

}

void CFileManage::OnFilemenuDel()
{
	// TODO: 在此添加命令处理程序代码
	UpdateData(TRUE);
	int idx = m_FileList.GetSelectionMark();
	CString tem = m_FileList.GetItemText(idx,0);
	tem = m_FilePath+tem;
	pView->m_clientsock.MySendCommond(AllSock,MYFILEMANAGE,(char*)tem.GetBuffer(MAX_PATH),MAX_PATH,MYDELFILE);
	OnFilemenuBrush();
}

void CFileManage::OnNMRClickList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	/*int idx = pNMItemActivate->iItem;
	if (idx<0)
	{
		return;
	}*/
	//m_ServArry.GetAt(idx).sk;
	CMenu menu,*pMunu=NULL;
	menu.LoadMenu(IDR_MEN_FILE);
	pMunu = menu.GetSubMenu(0);
	CPoint pt;
	GetCursorPos(&pt);

	pMunu->TrackPopupMenu(TPM_LEFTALIGN|TPM_TOPALIGN|TPM_RIGHTBUTTON,pt.x,pt.y,this);
	*pResult = 0;
}

void CFileManage::OnFilemenuCreatedir()
{
	// TODO: 在此添加命令处理程序代码
	CDirCreate *dlg = new CDirCreate(pView,AllSock,m_FilePath);
	dlg->Create(IDD_DIRCREATE,this);
	dlg->ShowWindow(SW_SHOW);
	OnFilemenuBrush();
}

void CFileManage::OnFilemenuDeldir()
{
	// TODO: 在此添加命令处理程序代码
	UpdateData(TRUE);
	pView->m_clientsock.MySendCommond(AllSock,MYFILEMANAGE,(char*)m_FilePath.GetBuffer(MAX_PATH),MAX_PATH,DELETEDIRIC);
	OnFilemenuBrush();
}

void CFileManage::OnFilemenuDown()
{
	// TODO: 在此添加命令处理程序代码
	UpdateData(TRUE);
	int idx = m_FileList.GetSelectionMark();
	CString tem = m_FileList.GetItemText(idx, 0);
	tem = m_FilePath + tem;
	pView->m_clientsock.MySendCommond(AllSock, FILESOCKSTART, (char *)tem.GetBuffer(MAX_PATH), MAX_PATH, FILEDOWN);
	OnFilemenuBrush();
}

void CFileManage::OnFilemenuUp()
{
	// TODO: 在此添加命令处理程序代码
	UpdateData(TRUE);
	if(m_FilePath.IsEmpty())
	{
		AfxMessageBox(_T("请选择上传文件的路径"));
		return ;
	}
	CFileDialog dlg(TRUE);
	if (dlg.DoModal() == IDOK)
	{
		CString temp = dlg.GetFileName();
		temp = m_FilePath + temp;
		pView->m_clientsock.MySendCommond(AllSock, FILESOCKSTART, (char *)temp.GetBuffer(MAX_PATH), MAX_PATH, FILEUP);
		m_FileSock.MyUpFileStart(dlg.GetPathName());
	}
	OnFilemenuBrush();
}

void CFileManage::OnFilemenuBrush()
{
	// TODO: 在此添加命令处理程序代码
	UpdateData(TRUE);
	m_FileList.DeleteAllItems();
	pView->m_clientsock.MySendCommond(AllSock, MYFILEMANAGE, (char*)m_FilePath.GetBuffer(MAX_PATH), MAX_PATH, FILEGET);
}

void CFileManage::OnBnClickedButtonCansend()
{
	// TODO: 在此添加控件通知处理程序代码
	bFileCancel = FALSE;
	pView->m_clientsock.MySendCommond(AllSock, MYFILEMANAGE, (char*)m_FilePath.GetBuffer(MAX_PATH), MAX_PATH, FILEGET);

}
