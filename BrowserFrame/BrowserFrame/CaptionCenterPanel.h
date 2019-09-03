#pragma once
#include <atlimage.h>
#include "MenuBar.h"
#include "panel.h"


class CCaptionCenterPanel :
	public CPanel
{
public:
	CCaptionCenterPanel(void);
	~CCaptionCenterPanel(void);
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
public:
	CImage m_imgCaption;
	CMenuBar m_menuBar;
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};
