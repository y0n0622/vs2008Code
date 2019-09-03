#pragma once


// CIconButton

class CIconButton : public CWnd
{
	DECLARE_DYNAMIC(CIconButton)

public:
	CIconButton();
	virtual ~CIconButton();
public:
	BOOL Create(LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

public:
	void SetIcon(UINT uIDResource);

private:
	HICON m_hIcon;


protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};


