#pragma once
#include "afxwin.h"

class CUIButton :
	public CButton
{
public:
	CBitmap m_bitmap;
	CUIButton(UINT nID);
	~CUIButton(void);
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
};
