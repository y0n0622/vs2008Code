#pragma once
#include <atlimage.h>

// CDeskButton

class CDeskButton : public CWnd
{
	DECLARE_DYNAMIC(CDeskButton)

public:
	CDeskButton();
	virtual ~CDeskButton();
public:
	BOOL Create(LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

public:
	void SetImage(UINT uIDResource);
	void SetSelected(BOOL bIsSelected);
	BOOL GetSelected();

private:
	CImage m_imgButton;
	BOOL m_bIsSelected;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg LRESULT OnMouseHover(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};


