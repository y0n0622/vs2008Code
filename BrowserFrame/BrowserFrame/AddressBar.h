#pragma once
#include "panel.h"
#include <atlimage.h>
#include "AddressComboBox.h"
#include "ImageButton.h"

class CAddressBar :
	public CPanel
{
public:
	CAddressBar(void);
	~CAddressBar(void);
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

public:
	CImage m_imgTool;
	CAddressComboBox m_addrComboBox;
	CImageButton m_btnGo;
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
