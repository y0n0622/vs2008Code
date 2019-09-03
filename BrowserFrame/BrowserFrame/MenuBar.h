#pragma once
#include "panel.h"
#include <atlimage.h>
#include <vector>
using namespace std;

class CMenuItem
{
public:
	CMenuItem();
	~CMenuItem();
	enum MENUSTATUE{Normal = 0, Hover, Pressed};
	void Draw(CDC* pDC);
	void ShowPopupMenu(CWnd* pWnd);
	void Updata(DWORD dwNewState, CWnd* pWnd);
public:
	CString m_strText;
	CRect m_rcMenu;
	HMENU m_hPopupMenu;
	DWORD m_dwState;
};


class CMenuBar :
	public CPanel
{
public:
	CMenuBar(void);
	~CMenuBar(void);
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();

	

public:
	void LoadMenuFromResource(UINT uIDResource);
	int GetMenuItemCount();
	CMenuItem* GetItem(int nItem);
	void clear();
	int HitTest(CPoint pt);
	
	
public:
	CImage m_imgBkGnd;
	vector<CMenuItem*> m_vecMenuItem;
	CFont m_font;
	CMenu menu;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseHover(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};
