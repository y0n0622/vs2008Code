#pragma once
#include "panel.h"
#include "ImageButton.h"
#include "SideBar.h"



class CMainToolBar :
	public CPanel
{
public:
	CMainToolBar(void);
	~CMainToolBar(void);
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnFavorite();

private:
	CImageButton m_btnBackward;
	CImageButton m_btnForward;
	CImageButton m_btnStop;
	CImageButton m_btnRefresh;
	CImageButton m_btnHome;
	CImageButton m_btnRestore;
	CImageButton m_btnFavorite;

	

public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	CSideBar *m_sideBar;
};
