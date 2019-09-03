#pragma once
#include "panel.h"
#include "CaptionLeftPanel.h"
#include "CaptionCenterPanel.h"
#include "CaptionRightPanel.h"

class CCaptionPanel :
	public CPanel
{
public:
	CCaptionPanel(void);
	~CCaptionPanel(void);

private:
	CCaptionLeftPanel m_captionLeftPanel;
	CCaptionCenterPanel m_captionCenterPanel;
	CCaptionRightPanel m_captionRightPanel;

public:
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
