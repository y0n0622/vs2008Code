#pragma once
#include "panel.h"
#include "ImageButton.h"
#include "DeskPanel.h"
#include "UITreeCtrl.h"

extern BOOL bFavorite;
#define IDC_SIDEBARCLOSE   10002
#define  IDC_SIDEBARSHOW  10003
class CSideBar :
	public CPanel
{
public:
	CSideBar(void);
	~CSideBar(void);

public:
	CImageButton m_btnSideBarHead;
	CImageButton m_btnSideBarClose;
	CImage m_imgRight;
	CDeskPanel m_deskPanel;
	CUITreeCtrl m_uiTreeCtrl;

	void show();

public:
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void OnSideBarClose();
	afx_msg void OnShowSideBar();
};
