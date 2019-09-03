#pragma once
#include "resource.h"
#include "ClientSock.h"

// CSysSettingPort 对话框

class CSysSettingPort : public CDialog
{
	DECLARE_DYNAMIC(CSysSettingPort)

public:
	CSysSettingPort(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSysSettingPort();

// 对话框数据
	enum { IDD = IDD_PORT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	UINT m_LPort;
	afx_msg void OnBnClickedOk();
};
