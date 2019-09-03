#pragma once
#include <atlimage.h>
#include "panel.h"
#include "ImageButton.h"


class CCaptionRightPanel :
	public CPanel
{
public:
	CCaptionRightPanel(void);
	~CCaptionRightPanel(void);
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnClickedMinBtn();
	afx_msg void OnClickedMaxBtn();
	afx_msg void OnClickedCloseBtn();
public:
	CImage m_imgCaption;
	CImageButton m_imgMinBtn;
	CImageButton m_imgMaxBtn;
	CImageButton m_imgCloseBtn;
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};
