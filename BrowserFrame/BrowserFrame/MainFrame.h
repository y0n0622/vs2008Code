#pragma once
#include <atlimage.h>
#include "CaptionPanel.h"
#include "ToolPanel.h"
#include "TabPanel.h"
#include "SideBar.h"
#include "StatusPanel.h"

// CMainFrame

class CMainFrame : public CWnd
{
	DECLARE_DYNAMIC(CMainFrame)

public:
	CMainFrame();
	virtual ~CMainFrame();

public:
	
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void PostNcDestroy();
protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp);
	afx_msg void OnNcPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnNcActivate(BOOL bActive);

public:
	CImage m_imageBorder; 
	CCaptionPanel m_captionPanel;
	CToolPanel m_toolPanel;
	CTabPanel m_tabPanel;
	CSideBar m_sideBar;
	CStatusPanel m_StatusPanel;
	
};


