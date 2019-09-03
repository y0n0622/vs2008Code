#pragma once
#include "panel.h"

class CDeskContent :
	public CPanel
{
public:
	CDeskContent(void);
	~CDeskContent(void);
	DECLARE_MESSAGE_MAP()
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
