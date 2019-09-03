#pragma once
#include "Winsvc.h"

class CServItem
{
	public:
	CString m_strServName;
	CString m_strServDispName;
	DWORD m_dwServStatus;
	CString m_strBinPath;
	DWORD m_dwStartType;
	CString m_strDescription;
	CServItem *m_pNext;
	CServItem()
	{
		m_dwServStatus = 0;
		m_pNext = NULL;
	}
};
class CServConfig
{
public:
	CServConfig(void);
	~CServConfig(void);

public:
	CServItem *EnumServList();
	CString GetStateString(DWORD dwCurrState);
	BOOL GetServPathAndStartType(LPCTSTR lpszServName, CServItem &tItem);
	CString GetStartTypeString(DWORD dwStartType);
	CString GetServDescription(LPCTSTR lpszServName);
	long GetServCtrlAccepted(LPCTSTR lpszServName, DWORD *pdwCurrStatus = NULL);
	BOOL CtrlServStatus(LPCTSTR lpszServName, DWORD dwNewStatus);
	BOOL CtrlServStartType(LPCTSTR lpszServName, DWORD dwNewType);
	BOOL DeleteServ(LPCTSTR lpszServName);
	BOOL CreateServ(LPCTSTR lpszServName, LPCTSTR lpszBinPath, LPCTSTR lpszDispName = NULL, DWORD dwStartType = SERVICE_AUTO_START, LPCTSTR lpzsDesc = NULL, LPCTSTR lpszPass = NULL);
	void FreeServList(CServItem *pServList);
};

