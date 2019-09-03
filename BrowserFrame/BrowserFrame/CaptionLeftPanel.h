#pragma once
#include <atlimage.h>
#include "panel.h"


class CCaptionLeftPanel :
	public CPanel
{
public:
	CCaptionLeftPanel(void);
	~CCaptionLeftPanel(void);

public:
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
public:
	CImage m_imgCaption;
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
};
