#pragma once
#include "panel.h"
#include <atlimage.h>
#include "TabCtrEx.h"
#include "TabManager.h"

class CTabPanel :
	public CPanel
{
public:
	CTabPanel(void);
	~CTabPanel(void);
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();

private:
	CImage m_imgTabPanel;
	CTabCtrEx m_wndTabCtr;
	CTabManager m_tabManager;

public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
