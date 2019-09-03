#include "StdAfx.h"
#include "ServProcess.h"
#include <TlHelp32.h>
#include "ServSock.h"
#include <Psapi.h>
#pragma comment(lib,"Psapi.lib")

CServProcess::CServProcess(void)
{
}

CServProcess::~CServProcess(void)
{
}

void CServProcess::ProcessCommend(COMMEND cmd, SOCKET sk)
{
	switch(cmd.fID)
	{
	case PROCESSLIST:
		SendProList(sk);
		break;
	case MODULELIST:
		//MyGetModule(cmd, sk);
		MyGetModule1(cmd,sk);
		break;
	case EXITPROCESS:
		MyExitPro(cmd, sk);
		break;
	case MYLOGOUT:
		MySysManage(cmd);
		break;
	case MYRESTART:
		MySysManage(cmd);
		break;
	case MYSHUTDOWN:
		MySysManage(cmd);
		break;
	default:
		break;
	}
}

bool CServProcess::EnableDebugPrivilege(TCHAR* Privileges)   
{   
	HANDLE hToken;   
	LUID sedebugnameValue;   
	TOKEN_PRIVILEGES tkp;   
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
	{   
		return   FALSE;   
	}   
	if (!LookupPrivilegeValue(NULL, Privileges, &sedebugnameValue))  
	{   
		CloseHandle(hToken);   
		return false;   
	}   
	tkp.PrivilegeCount = 1;   
	tkp.Privileges[0].Luid = sedebugnameValue;   
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;   
	if (!AdjustTokenPrivileges(hToken, FALSE, &tkp, sizeof(tkp), NULL, NULL)) 
	{   
		CloseHandle(hToken);   
		return false;   
	}   
	return true;   
}

void CServProcess::SendProList(SOCKET sk)
{
	EnableDebugPrivilege(SE_DEBUG_NAME);
	wchar_t path[MAX_PATH];
	ZeroMemory(path, MAX_PATH);
	PROCESSINFO pls;
	COMMEND commend;
	HANDLE hPlHandle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32 p32;
	ZeroMemory(&p32, sizeof(PROCESSENTRY32));
	p32.dwSize = sizeof(PROCESSENTRY32);
	BOOL bRet = Process32First(hPlHandle, &p32);
	HMODULE hMod = NULL;
	while (bRet)
	{
		wsprintfW(pls.pName, _T("%s"), p32.szExeFile);

		pls.Pid = p32.th32ProcessID;
		memcpy(pls.pPath, MyGetProPathFile(p32.th32ProcessID), MAX_PATH);
		m_ServSock->MySendCommond(sk, MYGETPROCESS, (char*)&pls, sizeof(PROCESSINFO), PROCESSLIST);
		ZeroMemory(path, MAX_PATH);
		ZeroMemory(&pls, sizeof(PROCESSLIST));
		ZeroMemory(&commend, sizeof(COMMEND));
		bRet = Process32Next(hPlHandle, &p32);
	}
	CloseHandle(hPlHandle);
}

wchar_t *CServProcess::MyGetProPathFile(int pid)
{
	HANDLE hpro = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	wchar_t path[MAX_PATH] = {0};
	GetModuleFileNameEx(hpro, NULL, path, MAX_PATH);
	CloseHandle(hpro);
	return path;
}

void CServProcess::MyGetModule(COMMEND cmd, SOCKET sk)
{
	wchar_t temp[20];
	memcpy(temp, cmd.teste, 20);

	int pid = _wtoi(temp);

	HANDLE hpro = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);

	DWORD rsize = 0;
	HMODULE hDword[1024] = {0};
	//EnumProcessModulesEx(hpro, hDword, 1024, &rsize, LIST_MODULES_ALL);

	int k = rsize/sizeof(HMODULE);
	
	for (int i = 0; i < k; i++)
	{
		wchar_t mod[MAX_PATH] = {0};
		GetModuleFileNameEx(hpro, hDword[i], mod, MAX_PATH);
		m_ServSock->MySendCommond(sk, MYGETPROCESS, (char*)&mod, sizeof(PROCESSINFO), MODULELIST);
	}
	CloseHandle(hpro);
}

void CServProcess::MyExitPro(COMMEND cmd, SOCKET sk)
{
	wchar_t temp[20];
	memcpy(temp, cmd.teste, 20);

	int pid = _wtoi(temp);

	HANDLE hpro = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	if (TerminateProcess(hpro, PROCESS_ALL_ACCESS))
	{
		m_ServSock->MySendCommond(sk, MYGETPROCESS, NULL, 0, EXITPROCESS);
	}
	
}

void CServProcess::MyGetModule1(COMMEND cmd,SOCKET sk)
{
	wchar_t temp[20];
	memcpy(temp,cmd.teste,20);

	int pid = _wtoi(temp);
	PROCESSINFO ModuInfo;
	ZeroMemory(&ModuInfo,sizeof(PROCESSINFO));
	HANDLE tlHand = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE,pid);
	MODULEENTRY32 p32;
	ZeroMemory(&p32,sizeof(MODULEENTRY32));
	p32.dwSize = sizeof(MODULEENTRY32);
	BOOL bRet = Module32First(tlHand,&p32);

	while(bRet)
	{
		wsprintfW(ModuInfo.pName,_T("%s"),p32.szModule);
		wsprintfW(ModuInfo.pPath,_T("%s"),p32.szExePath);
		m_ServSock->MySendCommond(sk,MYGETPROCESS,(char*)&ModuInfo,sizeof(ModuInfo),MODULELIST);
		ZeroMemory(&ModuInfo,sizeof(PROCESSINFO));
		bRet = Module32Next(tlHand,&p32);
	}
}

void CServProcess::MySysManage(COMMEND cmd)
{
	DWORD flag;
	switch (cmd.fID)
	{
	case MYLOGOUT:
		flag = EWX_LOGOFF;
		break;
	case MYRESTART:
		flag = EWX_REBOOT;
		break;
	case MYSHUTDOWN:
		flag = EWX_SHUTDOWN;
		break;
	default:
		break;
	}
	EnableDebugPrivilege(SE_SHUTDOWN_NAME);
	ExitWindowsEx(flag|EWX_FORCE, 0);
}