#include "stdafx.h"
#include "ServConfig.h"


CServConfig::CServConfig(void)
{
}


CServConfig::~CServConfig(void)
{
}

CServItem *CServConfig::EnumServList()
{
	//提权
	//HANDLE TokenHandle;
	//if(!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &TokenHandle))
	//{
	//	return NULL;
	//}

	//TOKEN_PRIVILEGES t_privileges = {0};
	//if(!LookupPrivilegeValue(NULL, SE_SECURITY_NAME/*SE_SHUTDOWN_NAME*/, &t_privileges.Privileges[0].Luid))
	//{
	//	return NULL;
	//}
	//t_privileges.PrivilegeCount = 1;
	//t_privileges.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	//if(!AdjustTokenPrivileges(TokenHandle, FALSE, &t_privileges, sizeof(TOKEN_PRIVILEGES), NULL, NULL))
	//{
	//	CloseHandle(TokenHandle);
	//	return NULL;
	//}

	//打开服务
	SC_HANDLE hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_ENUMERATE_SERVICE/*SC_MANAGER_ALL_ACCESS*/);
	if (!hSCM)
	{
		return NULL;
	}
	CServItem *pServHeader = NULL, *pServPre = NULL, *pServNext = NULL; 
	LPENUM_SERVICE_STATUS pServStatus = NULL;
	DWORD dwBytesNeeded = 0, dwServCound = 0, dwResume = 0, dwRealBytes = 0;
	BOOL bRet = EnumServicesStatus(hSCM, SERVICE_WIN32, SERVICE_STATE_ALL, NULL, 0, &dwBytesNeeded, &dwServCound, &dwResume);
	if (!bRet && GetLastError() == ERROR_MORE_DATA)
	{
		dwRealBytes = dwBytesNeeded;
		pServStatus = new ENUM_SERVICE_STATUS[dwRealBytes + 1];
		ZeroMemory(pServStatus, dwRealBytes + 1);
		bRet = EnumServicesStatus(hSCM, SERVICE_WIN32, SERVICE_STATE_ALL, pServStatus, dwRealBytes, &dwBytesNeeded, &dwServCound, &dwResume);
		if (!bRet)
		{
			CloseServiceHandle(hSCM);
			return NULL;
		}
	}
	else
	{
		CloseServiceHandle(hSCM);
		return NULL;
	}

	pServPre = pServNext;
	for (DWORD dwIdx = 0; dwIdx < dwServCound; dwIdx++)
	{
		pServNext = new CServItem;
		pServNext->m_strServName = pServStatus[dwIdx].lpServiceName;
		pServNext->m_strServDispName = pServStatus[dwIdx].lpDisplayName;
		pServNext->m_dwServStatus = pServStatus[dwIdx].ServiceStatus.dwCurrentState;
		GetServPathAndStartType(pServNext->m_strServName, *pServNext);
		pServNext->m_strDescription = GetServDescription(pServNext->m_strServName);
		(pServHeader == NULL)?(pServHeader = pServNext):(pServHeader);
		(pServPre == NULL)?(pServPre = pServNext):(pServPre->m_pNext = pServNext, pServPre = pServNext);
	}
	delete[] pServStatus;
	return pServHeader;
}

CString CServConfig::GetStateString(DWORD dwCurrState)
{
	CString strResult;
	switch (dwCurrState)
	{
		case SERVICE_START_PENDING:
			strResult = _T("正在启动");
			break;
		case SERVICE_STOP_PENDING:
			strResult = _T("正在停止");
			break;
		case SERVICE_RUNNING:
			strResult = _T("已启动");
			break;
		case SERVICE_CONTINUE_PENDING:
			strResult = _T("继续中");
			break;
		case SERVICE_PAUSE_PENDING:
			strResult = _T("暂停中");
			break;
		case SERVICE_PAUSED:
			strResult = _T("暂停");
			break;
		case SERVICE_STOPPED:
		default:
			break;
	}
	return strResult;
}

BOOL CServConfig::GetServPathAndStartType(LPCTSTR lpszServName, CServItem &tItem)
{
	BOOL bRet = FALSE;
	SC_HANDLE hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_ENUMERATE_SERVICE);
	if (NULL == hSCM)
	{
		return FALSE;
	}
	SC_HANDLE hSvc = OpenService(hSCM, lpszServName, SERVICE_QUERY_CONFIG);
	if (NULL == hSvc)
	{
		CloseServiceHandle(hSCM);
		return FALSE;
	}

	QUERY_SERVICE_CONFIG *pServCfg = NULL;
	DWORD cbBytesNeeded = 0, cbBufferSize = 0;
	bRet = QueryServiceConfig(hSvc, NULL, 0, &cbBytesNeeded);
	if (FALSE == bRet)
	{
		if (ERROR_INSUFFICIENT_BUFFER == GetLastError())
		{
			pServCfg = new QUERY_SERVICE_CONFIG[cbBytesNeeded + 1];
			ZeroMemory(pServCfg, cbBytesNeeded + 1);
			cbBufferSize = cbBytesNeeded;
			cbBytesNeeded = 0;
		}
		else
		{
			goto __Error_End;
		}
	}
	bRet = QueryServiceConfig(hSvc, pServCfg, cbBufferSize, &cbBytesNeeded);
	if (FALSE == bRet)
	{
		goto __Error_End;
	}
	tItem.m_strBinPath = pServCfg->lpBinaryPathName;
	tItem.m_dwStartType = pServCfg->dwStartType;
	bRet = TRUE;

__Error_End:
	CloseServiceHandle(hSvc);
	CloseServiceHandle(hSCM);
	return bRet;
}

CString CServConfig::GetStartTypeString(DWORD dwStartType)
{
	CString strResult;
	switch (dwStartType)
	{
		case SERVICE_AUTO_START:
			 strResult = _T("自动");
			 break;
		case SERVICE_DEMAND_START:
			strResult = _T("手动");
			break;
		case SERVICE_DISABLED:
			strResult = _T("已禁止");
			break;
		default:
			strResult = _T("未知");
			break;
	}
	return strResult;
}

CString CServConfig::GetServDescription(LPCTSTR lpszServName)
{
	CString strResult;
	BOOL bRet = FALSE;
	SC_HANDLE hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_ENUMERATE_SERVICE);
	if (NULL == hSCM)
	{
		return strResult;
	}
	SC_HANDLE hSvc = OpenService(hSCM, lpszServName, SERVICE_QUERY_CONFIG);
	if (NULL == hSvc)
	{
		CloseServiceHandle(hSCM);
		return strResult;
	}
	DWORD dwNeeded = 0, dwLen = 0;
	LPSERVICE_DESCRIPTION pDescription = NULL;
	bRet = QueryServiceConfig2(hSvc, SERVICE_CONFIG_DESCRIPTION, NULL, 0, &dwNeeded);
	if (!bRet && ERROR_INSUFFICIENT_BUFFER == GetLastError())
	{
		dwLen = dwNeeded + 1;
		pDescription = new SERVICE_DESCRIPTION[dwLen];
		ZeroMemory(pDescription, dwLen);
		if (QueryServiceConfig2(hSvc, SERVICE_CONFIG_DESCRIPTION, (LPBYTE)pDescription, dwLen, &dwNeeded))
		{
			strResult = pDescription->lpDescription;
		}
	}
	CloseServiceHandle(hSvc);
	CloseServiceHandle(hSCM);
	return strResult;

}

long CServConfig::GetServCtrlAccepted(LPCTSTR lpszServName, DWORD *pdwCurrStatus /* = NULL */)
{
	long dwCtrlAcceptted = -1;
	SC_HANDLE hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_ENUMERATE_SERVICE);
	if (!hSCM)
	{
		return -1;
	}
	SC_HANDLE hSvc = OpenService(hSCM, lpszServName, SERVICE_QUERY_STATUS);
	if (!hSvc)
	{
		CloseServiceHandle(hSvc);
		return -1;
	}
	SERVICE_STATUS sTs = {0};
	if(!QueryServiceStatus(hSvc, &sTs))
	{
		goto __Error_End;
	}
	dwCtrlAcceptted = sTs.dwControlsAccepted;
	if (pdwCurrStatus)
	{
		*pdwCurrStatus = sTs.dwCurrentState;
	}
__Error_End:
	CloseServiceHandle(hSCM);
	CloseServiceHandle(hSvc);

	return dwCtrlAcceptted;
}

BOOL CServConfig::CtrlServStatus(LPCTSTR lpszServName, DWORD dwNewStatus)
{
	BOOL bRet = FALSE;
	SC_HANDLE hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_ENUMERATE_SERVICE);
	if (!hSCM)
	{
		return FALSE;
	}
	SC_HANDLE hSvc = OpenService(hSCM, lpszServName, SERVICE_ALL_ACCESS);
	if (!hSvc)
	{
		CloseServiceHandle(hSCM);
		return FALSE;
	}
	SERVICE_STATUS sTs = {0};
	if(!QueryServiceStatus(hSvc, &sTs))
	{
		goto __Error_End;
	}
	if (dwNewStatus == sTs.dwCurrentState)
	{
		return TRUE;
	}
	else if(sTs.dwCurrentState == SERVICE_START_PENDING || 
		sTs.dwCurrentState == SERVICE_STOP_PENDING ||
		sTs.dwCurrentState == SERVICE_CONTINUE_PENDING ||
		sTs.dwCurrentState == SERVICE_PAUSE_PENDING)
	{
		return FALSE;
	}
	else if (sTs.dwCurrentState == SERVICE_STOPPED && dwNewStatus == SERVICE_RUNNING)
	{
		bRet = StartService(hSvc, NULL, NULL);
	}
	else if ((sTs.dwCurrentState == SERVICE_RUNNING || sTs.dwCurrentState == SERVICE_PAUSED) && dwNewStatus == SERVICE_STOPPED)
	{
		bRet = ControlService(hSvc, SERVICE_CONTROL_STOP, &sTs);
	}
	else if(sTs.dwCurrentState == SERVICE_PAUSED && dwNewStatus == SERVICE_RUNNING)
	{
		bRet = ControlService(hSvc, SERVICE_CONTROL_CONTINUE, &sTs);
	}
	else if(sTs.dwCurrentState == SERVICE_RUNNING && dwNewStatus == SERVICE_PAUSED)
	{
		bRet = ControlService(hSvc, SERVICE_CONTROL_PAUSE, &sTs);
	}
__Error_End:
	CloseServiceHandle(hSCM);
	CloseServiceHandle(hSvc);
	
	return bRet;

}

BOOL CServConfig::CtrlServStartType(LPCTSTR lpszServName, DWORD dwNewType)
{
	BOOL bRet = FALSE;
	SC_HANDLE hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (!hSCM)
	{
		return FALSE;
	}
	SC_HANDLE hSvc = OpenService(hSCM, lpszServName, SERVICE_ALL_ACCESS);
	if (!hSvc)
	{
		CloseServiceHandle(hSCM);
		return FALSE;
	}
	CServItem tItem;
	if (GetServPathAndStartType(lpszServName, tItem) && tItem.m_dwStartType == dwNewType)
	{
		bRet = TRUE;
		goto __Error_End;
	}
	bRet = ChangeServiceConfig(hSvc, SERVICE_NO_CHANGE, dwNewType, SERVICE_NO_CHANGE, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
__Error_End:
	CloseServiceHandle(hSCM);
	CloseServiceHandle(hSvc);
	return bRet;
}

BOOL CServConfig::DeleteServ(LPCTSTR lpszServName)
{
	BOOL bRet = FALSE;
	SC_HANDLE hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (!hSCM)
	{
		return FALSE;
	}
	SC_HANDLE hSvc = OpenService(hSCM, lpszServName, SERVICE_ALL_ACCESS);
	if (!hSvc)
	{
		CloseServiceHandle(hSCM);
		return FALSE;
	}
	if (CtrlServStatus(lpszServName, SERVICE_CONTROL_STOP))
	{
		bRet = DeleteService(hSvc);
	}
	CloseServiceHandle(hSCM);
	CloseServiceHandle(hSvc);
	return bRet;
}

BOOL CServConfig::CreateServ(LPCTSTR lpszServName, LPCTSTR lpszBinPath, LPCTSTR lpszDispName, DWORD dwStartType, LPCTSTR lpszDesc, LPCTSTR lpszPass)
{
	BOOL bRet = FALSE;
	SC_HANDLE hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (!hSCM)
	{
		return FALSE;
	}
	CString strDispName, strPass;
	if (!lpszDispName)
	{
		lpszDispName = lpszServName;
	}
	else
	{
		lpszDispName = lpszDispName;
	}
	SC_HANDLE hSvc = CreateService(hSCM, lpszServName, lpszDispName, SERVICE_ALL_ACCESS, SERVICE_WIN32_OWN_PROCESS,
		dwStartType, SERVICE_ERROR_NORMAL, lpszBinPath, NULL, NULL, NULL, NULL, lpszPass);
	if (hSvc && lpszDesc)
	{
		bRet = TRUE;
		TCHAR szDesc[MAX_PATH] = {0};
		SERVICE_DESCRIPTION sServDesc = {0};
		_tcscpy_s(szDesc, MAX_PATH, lpszDesc);
		sServDesc.lpDescription = szDesc;
		ChangeServiceConfig2(hSvc, SERVICE_CONFIG_DESCRIPTION, &sServDesc);
		CloseServiceHandle(hSvc);
	}
	CloseServiceHandle(hSCM);
	return bRet;
}

void CServConfig::FreeServList(CServItem *pServList)
{
	CServItem *pT = NULL;
	while (pServList)
	{
		pT = pServList->m_pNext;
		delete pServList;
		pServList = NULL;
	}
}