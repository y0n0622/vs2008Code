#pragma once
#include <atlimage.h>

// CUITreeCtrl

class CUITreeCtrl : public CTreeCtrl
{
	DECLARE_DYNAMIC(CUITreeCtrl)

public:
	CUITreeCtrl();
	virtual ~CUITreeCtrl();
	
public:
	CImage m_imgCollapse;
	CImage m_imgExpand;

public:
	void DrawBackground(CDC *pDC);
	void DrawItem(CDC *pDC, HTREEITEM hTreeItem);
	BOOL SetItemHeightEx(HTREEITEM hTreeItem, UINT cyItemHeight);

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult);
};


