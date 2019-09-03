#include "StdAfx.h"
#include "ServSVC.h"
#include "ServSock.h"
CServSVC::CServSVC(void)
{
	AllSock = NULL;
}

CServSVC::~CServSVC(void)
{
}


void CServSVC::MySvcCommend(COMMEND cmd, SOCKET sk)
{
	AllSock = sk;
	switch (cmd.fID)
	{
	case GETSVCLIST:
		MyGetSVCList();
		break;
	case MYSTARTSVC:
		MyStartSVC(cmd);
		break;
	case SVCSTOP:
		MyStopSVC(cmd);
		break;
	case SVCAUTOTYPE:
		MyChangeType(cmd, SERVICE_AUTO_START);
		break;
	case SVCBOOTYPE:
		MyChangeType(cmd, SERVICE_BOOT_START);
		break;
	case SVCDISABLETYPE:
		MyChangeType(cmd, SERVICE_DISABLED);
		break;
	case SVCDELETE:
		MyDeleteSVC(cmd);
	case SVCCREATE:
		MyCreateSVC(cmd);
	default:
		break;
	}
}

void CServSVC::MyGetSVCList()
{
	//EnableDebugPrivilege(SE_SHUTDOWN_NAME);
	SC_HANDLE scHandle = OpenSCManager(NULL, NULL, SC_MANAGER_ENUMERATE_SERVICE);

	ENUM_SERVICE_STATUS *estatus = NULL;
	DWORD cbsize = 0, pNeeded = 0, lpReturned = 0;
	BOOL bRet = EnumServicesStatus(scHandle, SERVICE_WIN32, SERVICE_STATE_ALL, NULL, 0, &pNeeded, &lpReturned, 0);
	if (!bRet && GetLastError() == ERROR_MORE_DATA)
	{
		cbsize = pNeeded + 1;
		estatus = new ENUM_SERVICE_STATUS[cbsize];
		bRet = EnumServicesStatus(scHandle, SERVICE_WIN32, SERVICE_STATE_ALL, estatus, cbsize, &pNeeded, &lpReturned, 0);
		SVICELISTINFO svinfo;
		for (DWORD i = 0; i < lpReturned; i++)
		{
			ZeroMemory(&svinfo, sizeof(SVICELISTINFO));
			wsprintfW(svinfo.ServiceName, _T("%s"), estatus[i].lpServiceName);
			wsprintfW(svinfo.DisplayName, _T("%s"), estatus[i].lpDisplayName);
			svinfo.CurrentStatus = estatus[i].ServiceStatus.dwCurrentState;
			SC_HANDLE oHandle = OpenService(scHandle, estatus[i].lpServiceName, SERVICE_QUERY_CONFIG/*SERVICE_ALL_ACCESS*/);
			DWORD QueSize = 0, pNeed = 0;
			QUERY_SERVICE_CONFIG *lpConfig = NULL;

			bRet = QueryServiceConfig(oHandle, NULL, 0, &pNeed);
			if (!bRet && GetLastError() == ERROR_INSUFFICIENT_BUFFER)
			{
				QueSize = pNeed + 1;
				lpConfig = new QUERY_SERVICE_CONFIG[QueSize];
				bRet = QueryServiceConfig(oHandle, lpConfig, QueSize, &pNeed);
				svinfo.stype = lpConfig->dwStartType;
				wsprintfW(svinfo.BinPath, _T("%s"), lpConfig->lpBinaryPathName);
			}
			delete []lpConfig;

			SERVICE_DESCRIPTION *sdr = NULL;
			bRet = QueryServiceConfig2(oHandle, SERVICE_CONFIG_DESCRIPTION, NULL, 0, &pNeed);
			if (!bRet && GetLastError() == ERROR_INSUFFICIENT_BUFFER)
			{
				QueSize = pNeed + 1;
				sdr = new SERVICE_DESCRIPTION[QueSize];
				bRet = QueryServiceConfig2(oHandle, SERVICE_CONFIG_DESCRIPTION, (LPBYTE)sdr, QueSize, &pNeed);
				wsprintfW(svinfo.Description, _T("%s"), sdr->lpDescription);
			}
			delete []sdr;

			CloseServiceHandle(oHandle);
			m_ServSock->MySendCommond(AllSock, MYSVCMANAGE, (char*)&svinfo, sizeof(SVICELISTINFO), GETSVCLIST);
			Sleep(50);
		}
	}
	CloseServiceHandle(scHandle);
}

bool CServSVC::EnableDebugPrivilegeSame(TCHAR* Privileges)   
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

void CServSVC::MyStartSVC(COMMEND cmd)
{
	wchar_t svcname[MAX_PATH] = {0};
	memcpy(svcname, cmd.teste, MAX_PATH);
	EnableDebugPrivilegeSame(SE_SECURITY_NAME);
	SC_HANDLE scHandle = OpenSCManager(NULL, NULL, SC_MANAGER_ENUMERATE_SERVICE|SC_MANAGER_CONNECT/*SC_MANAGER_ALL_ACCESS*/);
	SC_HANDLE oHandle = OpenService(scHandle, svcname, SERVICE_ALL_ACCESS);
	
	if (oHandle == NULL)
	{
		m_ServSock->MySendCommond(AllSock, MYSVCMANAGE, (char*)_T("打开服务成功!"), MAX_PATH, SVCMSG);
	}

	if (int i = StartService(oHandle, NULL, NULL))
	{
		m_ServSock->MySendCommond(AllSock, MYSVCMANAGE, (char*)_T("启动服务成功!"), MAX_PATH, SVCMSG);
	}
	else
	{
		m_ServSock->MySendCommond(AllSock, MYSVCMANAGE, (char*)_T("启动服务失败!"), MAX_PATH, SVCMSG);
	}

	CloseServiceHandle(oHandle);
	CloseServiceHandle(scHandle);
}

void CServSVC::MyStopSVC(COMMEND cmd)
{
	wchar_t svcname[MAX_PATH] = {0};
	memcpy(svcname, cmd.teste, MAX_PATH);
	EnableDebugPrivilegeSame(SE_SECURITY_NAME);
	SC_HANDLE scHandle = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS/*SC_MANAGER_ALL_ACCESS*/);
	SC_HANDLE oHandle = OpenService(scHandle, svcname, SERVICE_ALL_ACCESS);

	if (oHandle == NULL)
	{
		m_ServSock->MySendCommond(AllSock, MYSVCMANAGE, (char*)_T("打开服务失败!"), MAX_PATH, SVCMSG);
	}
	SERVICE_STATUS scsrp;
	ZeroMemory(&scsrp, sizeof(SERVICE_STATUS));
	scsrp.dwCurrentState = SERVICE_STOP;
	if (ControlService(oHandle, SERVICE_CONTROL_STOP, &scsrp))
	{
		m_ServSock->MySendCommond(AllSock, MYSVCMANAGE, (char*)_T("服务已停止!"), MAX_PATH, SVCMSG);
	}
	else
	{
		m_ServSock->MySendCommond(AllSock, MYSVCMANAGE, (char*)_T("服务停止失败!"), MAX_PATH, SVCMSG);
	}
	CloseServiceHandle(oHandle);
	CloseServiceHandle(scHandle);
}

void CServSVC::MyChangeType(COMMEND cmd, DWORD ty)
{
	wchar_t svcname[MAX_PATH] = {0};
	memcpy(svcname, cmd.teste, MAX_PATH);
	EnableDebugPrivilegeSame(SE_SECURITY_NAME);
	SC_HANDLE scHandle = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS/*SC_MANAGER_ALL_ACCESS*/);
	SC_HANDLE oHandle = OpenService(scHandle, svcname, SERVICE_ALL_ACCESS);

	if (oHandle == NULL)
	{
		m_ServSock->MySendCommond(AllSock, MYSVCMANAGE, (char*)_T("打开服务失败!"), MAX_PATH, SVCMSG);
	}

	if (ChangeServiceConfig(oHandle, SERVICE_WIN32_OWN_PROCESS, ty, SERVICE_ERROR_NORMAL, NULL, NULL, NULL, NULL, NULL, NULL, NULL))
	{
		m_ServSock->MySendCommond(AllSock,MYSVCMANAGE,(char*)_T("服务启动方式已改变！"),MAX_PATH,SVCMSG);
	}
	else
	{
		m_ServSock->MySendCommond(AllSock,MYSVCMANAGE,(char*)_T("服务启动方式未改变！"),MAX_PATH,SVCMSG);

	}	


	CloseServiceHandle(oHandle);
	CloseServiceHandle(scHandle);
}

void CServSVC::MyDeleteSVC(COMMEND cmd)
{
	wchar_t svcname[MAX_PATH] = {0};
	memcpy(svcname, cmd.teste, MAX_PATH);
	EnableDebugPrivilegeSame(SE_SECURITY_NAME);
	SC_HANDLE scHandle = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS/*SC_MANAGER_ALL_ACCESS*/);
	SC_HANDLE oHandle = OpenService(scHandle, svcname, SERVICE_ALL_ACCESS);

	if (oHandle == NULL)
	{
		m_ServSock->MySendCommond(AllSock, MYSVCMANAGE, (char*)_T("打开服务失败!"), MAX_PATH, SVCMSG);
	}
	
	if (DeleteService(oHandle))
	{
		m_ServSock->MySendCommond(AllSock,MYSVCMANAGE,(char*)_T("服务已删除!"),MAX_PATH,SVCMSG);
	}
	else
	{
		m_ServSock->MySendCommond(AllSock,MYSVCMANAGE,(char*)_T("服务删除失败!"),MAX_PATH,SVCMSG);
	}
	CloseServiceHandle(oHandle);
	CloseServiceHandle(scHandle);
}

void CServSVC::MyCreateSVC(COMMEND cmd)
{
	SVICELISTINFO svcinfo;
	ZeroMemory(&svcinfo, sizeof(SVICELISTINFO));

	memcpy(&svcinfo, cmd.teste, sizeof(SVICELISTINFO));
	EnableDebugPrivilegeSame(SE_SECURITY_NAME);
	SC_HANDLE scHandle = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS/*SC_MANAGER_ALL_ACCESS*/);
	SC_HANDLE crHandle = CreateService(scHandle, svcinfo.ServiceName, svcinfo.DisplayName, SERVICE_ALL_ACCESS, SERVICE_WIN32_OWN_PROCESS, svcinfo.stype, SERVICE_ERROR_NORMAL, svcinfo.BinPath, NULL, NULL, NULL, NULL, NULL);
	if (crHandle == NULL)
	{
		m_ServSock->MySendCommond(AllSock, MYSVCMANAGE, (char*)_T("服务创建失败!"), MAX_PATH, SVCMSG);
		return ;
	}
	SERVICE_DESCRIPTION sdes;
	ZeroMemory(&sdes, sizeof(SERVICE_DESCRIPTION));
	//wsprintfW(sdes->lpDescription, _T("%s"), svcinfo.Description);
	//memcpy(&sdes->lpDescription, svcinfo.Description, MAX_PATH);
	sdes.lpDescription = svcinfo.Description;
	if (ChangeServiceConfig2(crHandle, SERVICE_CONFIG_DESCRIPTION, &sdes))
	{
		m_ServSock->MySendCommond(AllSock, MYSVCMANAGE, NULL, 0, SVCCREATE);
	}
	else
	{
		m_ServSock->MySendCommond(AllSock, MYSVCMANAGE, (char*)_T("创建服务描述失败!"), MAX_PATH, SVCMSG);
	}
	StartService(crHandle, NULL, NULL);
	CloseServiceHandle(crHandle);
	CloseServiceHandle(scHandle);
}