#pragma once
#include "panel.h"
#include <atlimage.h>

class CStatusPanel :
	public CPanel
{
public:
	CStatusPanel(void);
	~CStatusPanel(void);

public:
	CImage m_img;
	CImage m_imgSize;
public:
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg LRESULT OnNcHitTest(CPoint point);
};
