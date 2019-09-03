#pragma once
#include "panel.h"
#include <atlimage.h>
#include "ImageButton.h"
#include "IconButton.h"


class CSearchBar :
	public CPanel
{
public:
	CSearchBar(void);
	~CSearchBar(void);
private:
	CImage m_imgTool;
	CImage m_imgBand;
	CImageButton m_btnSearch;
	CEdit m_EditSearch;
	CIconButton m_IconButton;

public:
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnSelIconButton();
};
