#pragma once
#include "resource.h"
#include "ClientSock.h"

// CSysSettingPort �Ի���

class CSysSettingPort : public CDialog
{
	DECLARE_DYNAMIC(CSysSettingPort)

public:
	CSysSettingPort(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSysSettingPort();

// �Ի�������
	enum { IDD = IDD_PORT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	UINT m_LPort;
	afx_msg void OnBnClickedOk();
};
