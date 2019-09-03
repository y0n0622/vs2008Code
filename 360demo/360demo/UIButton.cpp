#include "StdAfx.h"
#include "UIButton.h"

CUIButton::CUIButton(UINT nID)
{
	m_bitmap.LoadBitmap(nID);
}

CUIButton::~CUIButton(void)
{
}

void CUIButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{

	// TODO:  添加您的代码以绘制指定项
	CDC *pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	memDC.SelectObject(m_bitmap);

	pDC->BitBlt(0, 0, 27, 29, &memDC, 0, 0, SRCCOPY);
	/*CRect rect = lpDrawItemStruct->rcItem;

	pDC->FillSolidRect(&rect, RGB(61, 140, 76));*/
}
