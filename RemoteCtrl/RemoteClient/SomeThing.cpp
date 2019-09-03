#include "StdAfx.h"
#include "SomeThing.h"

CSomeThing::CSomeThing(void)
{
}

CSomeThing::~CSomeThing(void)
{
}

BOOL CSomeThing::FreeBMP(LPCTSTR pszResName,LPCTSTR pszResType,LPCTSTR szFileName)   
{   
	BOOL bRet = TRUE;   
	HINSTANCE hInst = GetModuleHandle(NULL);   

	//�ж�ָ������Դ�Ƿ����   
	HRSRC hResInfo = FindResource(hInst,pszResName,pszResType);   
	if(hResInfo == NULL)   
		return FALSE;   

	HANDLE hFile = NULL;   
	DWORD dwWritten = 0;   
	//����ָ������Դ���ڴ�   
	HGLOBAL hResData = LoadResource(hInst,hResInfo);   
	LPVOID pvResData = LockResource(hResData);   
	DWORD dwResSize = SizeofResource(hInst,hResInfo);   

	//����Ŀ���ļ��������������ļ�   
	hFile = CreateFile(szFileName, GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE|FILE_SHARE_DELETE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_HIDDEN, NULL);   
	if(pszResType == RT_BITMAP)   
	{   
		//���λͼ��Ϣͷ   
		BITMAPINFOHEADER bmpInfoHdr;   
		CopyMemory(&bmpInfoHdr, pvResData, sizeof(BITMAPINFOHEADER));   
		//���λͼ��ɫ����RGBQUAD�ṹ�ĸ���   
		long nColorEntries;   
		switch(bmpInfoHdr.biBitCount)   
		{   
		case 1:   
			nColorEntries = 2;   
			break;   
		case 4:   
			nColorEntries = 16;   
			break;   
		case 8:   
			nColorEntries = 256;   
			break;   
		default:   
			nColorEntries = 0;   
		}   
		//����λͼ��Ϣͷ����λͼ�ļ�ͷ   
		BITMAPFILEHEADER bmpFileHdr;   
		bmpFileHdr.bfType = 0x4d42;  // 'BM'   
		bmpFileHdr.bfSize = sizeof(BITMAPINFOHEADER) + dwResSize;   
		bmpFileHdr.bfReserved1 = bmpFileHdr.bfReserved2 = 0;   
		bmpFileHdr.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * nColorEntries;   
		//д��λͼ�ļ�ͷ   
		WriteFile(hFile, &bmpFileHdr, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);   
	}   
	WriteFile(hFile, pvResData, dwResSize, &dwWritten, NULL);   
	//�ͷ��й��ڴ���Դ   
	CloseHandle(hFile);   
	FreeResource(hResData);   
	return TRUE;   
}

CString CSomeThing::MyGetOS(int id)
{
	CString OS;

	switch (id)
	{
	case 1:
		OS = _T("Windows 2000");
		break;
	case 2:
		OS = _T("Windows XP");
		break;
	case 3:
		OS = _T("Windows Server 2003");
		break;
	case 4:
		OS = _T("Windows Vista");
		break;
	case 5:
		OS = _T("Windows7/Windows2008");
		break;
	//case 6:
		//OS = _T("Windows7���ϰ汾");
	default:
		OS = _T("Windows8/8.1/10");
		break;
	}
	return OS;
}
CString CSomeThing::MyCharToWide(char * ch)
{
	wchar_t temp[1024] = {0};
	MultiByteToWideChar(CP_ACP, NULL, ch, 1024, temp, 1024);
	return temp;
}

char* CSomeThing::MyWideToChar(CString st)
{
	char temp[1024] = {0};
	WideCharToMultiByte(CP_ACP, NULL, st, 1024, temp, 1024, NULL, NULL);
	return temp;
}
