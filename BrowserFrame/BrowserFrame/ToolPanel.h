#pragma once
#include "panel.h"
#include <atlimage.h>
#include "MainToolBar.h"
#include "AddressBar.h"
#include "SearchBar.h"

class CToolPanel :
	public CPanel
{
public:
	CToolPanel(void);
	~CToolPanel(void);

private:
	CImage m_imgTool;
	CMainToolBar m_MainToolBar;
	CAddressBar m_AddressBar;
	CSearchBar m_SearchBar;
public:
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
