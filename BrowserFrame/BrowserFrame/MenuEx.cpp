#include "StdAfx.h"
#include "MenuEx.h"

CMenuEx::CMenuEx(void)
{
}

CMenuEx::~CMenuEx(void)
{
}

void CMenuEx::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{

	// TODO:  添加您的代码以绘制指定项
	MENUITEM *pMi = (MENUITEM*)lpDrawItemStruct->itemData;
	ASSERT(pMi);
	HDC hDC = lpDrawItemStruct->hDC;
	CRect rcItem = lpDrawItemStruct->rcItem;
	UINT nItemState = lpDrawItemStruct->itemState;


	CDC dc;
	dc.Attach(hDC);
	int nBkMode = dc.SetBkMode(TRANSPARENT);

	CRect rcIcon = rcItem;
	rcIcon.right = rcIcon.left + rcItem.Height();
	rcIcon.DeflateRect(2, 2, 2, 2);
	DrawIconEx(hDC, rcIcon.left, rcIcon.top, pMi->hIcon, rcIcon.Width(), rcIcon.Height(), 0, NULL, DI_NORMAL);

	CRect rcText = rcItem;
	rcText.left = rcText.Height();

	if (nItemState & ODS_SELECTED)
	{
		dc.FillRect(rcText, &CBrush(0xff0000));
	}
	else
	{
		dc.FillRect(rcText, &CBrush(0xffffff));
	}
	dc.DrawText(pMi->m_strText, rcText, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_WORD_ELLIPSIS);

	dc.SetBkMode(nBkMode);
	dc.Detach();
}

void CMenuEx::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{

	// TODO:  添加您的代码以确定指定项的大小
	lpMeasureItemStruct->itemHeight = 20;
	lpMeasureItemStruct->itemWidth = 50;
}
