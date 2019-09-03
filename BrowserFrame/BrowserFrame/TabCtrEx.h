#pragma once
#include "panel.h"
#include "TabButton.h"
#include <atlimage.h>
#include <vector>
using namespace std;

class CTabCtrEx :
	public CPanel
{
public:
	CTabCtrEx(void);
	~CTabCtrEx(void);

private:
	CImage m_imgTabPanel;
	vector<CTabButton*> m_vecTabButton;

public:
	int AddItem(CString strText);

public:
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
};
