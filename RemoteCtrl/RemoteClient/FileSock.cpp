#include "StdAfx.h"
#include "FileSock.h"
#include "RemoteClientView.h"
#include "MainFrm.h"


CArray<SOCKET, SOCKET>m_FlSock;
BOOL bl = FALSE; 
CString FSPath = _T("");	
BOOL bFileCancel = TRUE;
CFileSock::CFileSock(void)
{
	FileSock = INVALID_SOCKET;
	CMainFrame *pMain = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	pView = (CRemoteClientView *)pMain->GetActiveView();
	//bl = FALSE;
	hFile = NULL;
//	FSPath = _T("");
}

CFileSock::~CFileSock(void)
{
}

void CFileSock::MyFSockStart()
{
	if (m_FlSock.GetCount() == 0)
	{
		SOCKET FileSock = pView->m_clientsock.MyBindSock(6791);
		m_FlSock.Add(FileSock);
		AfxBeginThread(FileSockFunc, this);
	}
}

UINT __cdecl CFileSock::FileSockFunc(LPVOID lp)
{
	CFileSock *pthis = (CFileSock*)lp;
	//pthis->bl = TRUE;
	SOCKET accsock;
	while (1)
	{
		sockaddr_in acaddr;
		int aint = sizeof(sockaddr_in);
		accsock = accept(m_FlSock.GetAt(0), (sockaddr*)&acaddr, &aint);
		FileSenRcv((LPVOID)accsock);
		
		break;
	}
	//pthis->bl = FALSE;
	//AfxBeginThread(FileSenRcv, (LPVOID)accsock);
	return 0;
}

UINT __cdecl CFileSock::FileSenRcv(LPVOID lp)
{
	SOCKET stem = (SOCKET)lp;
	CFileSock m_FileSock;
	FILESRCMD cmd;
	while (1)
	{
		ZeroMemory(&cmd, FILESRSIZE);
		if (m_FileSock.pView->m_clientsock.MySelect(stem))
		{
			if (!m_FileSock.MyRecvFILE(stem, cmd))
			{
				break;
			}
			switch(cmd.ID)
			{
			case FILECREATE:
				m_FileSock.DownFile(cmd);
				break;
			case FILEDOWN:
				m_FileSock.DownFile(cmd);
				break;
			case STARTSCREEEN:
				if (m_FileSock.pView->m_ScreenManage != NULL)
				{
					m_FileSock.pView->m_ScreenManage->MyStartDC(stem);
				}
				return 0;
			default:
				break;

			}
		}
		if (bl && !FSPath.IsEmpty())
		{
			AfxBeginThread(FileUpFunc, (LPVOID)stem);
		}
		
	}
	
	return 0;
}

UINT __cdecl CFileSock::FileUpFunc(LPVOID lp)
{
	SOCKET stem = (SOCKET)lp;
	CFileSock m_FileSock;
	bl = FALSE;
	HANDLE hUpFile = CreateFile(FSPath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, NULL, NULL);
	DWORD fs = GetFileSize(hUpFile, &fs);
	int one = fs/FILESIZE + 1;
	m_FileSock.pView->m_FileManage->m_Prograss.SetRange(0, one);
	m_FileSock.pView->m_FileManage->m_Prograss.SetStep(1);
	char *vbuf = new char[FILESIZE];
	while(1)
	{
		ReadFile(hUpFile, vbuf, FILESIZE, &fs, NULL);
		m_FileSock.MySendFile(stem, FILEUP, vbuf, fs, fs);
		if (m_FileSock.pView->m_FileManage == NULL || bFileCancel == FALSE)
		{
			CloseHandle(hUpFile);
			break;
		}
		m_FileSock.pView->m_FileManage->m_Prograss.StepIt();
		if (fs < FILESIZE)
		{
			CloseHandle(hUpFile);
			AfxMessageBox(_T("文件上传完成!"));
			m_FileSock.pView->m_FileManage->m_Prograss.SetPos(0);
			break;
		}
		Sleep(100);
	}
	
	return 0;
}

void CFileSock::MyUpFileStart(CString path)
{
	bl = TRUE;
	FSPath = path;
}


BOOL CFileSock::MySendFile(SOCKET sk, int Id, char *text, int size, int fd)
{
	FILESRCMD cmd;
	ZeroMemory(&cmd, FILESRSIZE);
	cmd.ID = Id;
	if (text != NULL)
	{
		memcpy(cmd.teste, text, size);
	}
	cmd.fID = fd;

	char *buf = new char[FILESRSIZE];
	ZeroMemory(buf, FILESRSIZE);
	memcpy(buf, &cmd, FILESRSIZE);
	int nleft = FILESRSIZE;
	int idx = 0;
	while(nleft > 0)
	{
		int dx = send(sk, &buf[idx], FILESRSIZE, 0);
		if (dx == SOCKET_ERROR)
		{
			return FALSE;
		}
		nleft = nleft - dx;
		idx += idx;
	}
	delete []buf;

	return TRUE;
}

void CFileSock::DownFile(FILESRCMD cmd)
{
	
	if (cmd.ID == FILECREATE)
	{
		wchar_t temp[MAX_PATH] = {0};
		memcpy(temp, cmd.teste, MAX_PATH);
		wchar_t *pos = wcsrchr(temp, _T('\\'));
		pos++;
		TCHAR szFilters[] = _T("All Files (*.*)|*.*||");
		CFileDialog FileDialog(FALSE, pos, pos, OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilters);
		if (FileDialog.DoModal() == IDOK)
		{
			CString pathName = FileDialog.GetPathName();
			hFile = CreateFile(pathName, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, NULL, NULL);
			int one = cmd.fID/FILESIZE + 1;
			pView->m_FileManage->m_Prograss.SetRange(0, one);
			pView->m_FileManage->m_Prograss.SetStep(one);

		}
	}
	else if (cmd.ID == FILEDOWN)
	{
		if (hFile == NULL)
		{
			AfxMessageBox(_T("文件未创建!"));
			return;
		}
		DWORD lpWritten = 0;
		WriteFile(hFile, cmd.teste, cmd.fID, &lpWritten, NULL);
	/*	if (pView->m_FileManage == NULL)
		{
			CloseHandle(hFile);
			hFile = NULL;
			return;
		}*/
		pView->m_FileManage->m_Prograss.StepIt();
		if (cmd.fID < FILESIZE)
		{
			CloseHandle(hFile);
			hFile = NULL;
			AfxMessageBox(_T("文件下载完成!"));
			pView->m_FileManage->m_Prograss.SetPos(0);
		}
	}
}

BOOL CFileSock::MyRecvFILE(SOCKET sk, FILESRCMD &cmd)
{
	ZeroMemory(&cmd, FILESRSIZE);

	char *buf = new char[FILESRSIZE];
	ZeroMemory(buf, FILESRSIZE);

	int nleft = FILESRSIZE;
	int idx = 0;
	while(nleft > 0)
	{
		int dx = recv(sk, &buf[idx], FILESRSIZE, 0);
		if (dx == SOCKET_ERROR)
		{
			return FALSE;
		}
		nleft = nleft - dx;
		idx += idx;
	}
	memcpy(&cmd, buf, FILESRSIZE);
	delete []buf;

	return TRUE;
}

