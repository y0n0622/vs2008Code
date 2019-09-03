// UITreeCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "BrowserFrame.h"
#include "UITreeCtrl.h"
#include "resource.h"


// CUITreeCtrl

IMPLEMENT_DYNAMIC(CUITreeCtrl, CTreeCtrl)

CUITreeCtrl::CUITreeCtrl()
{
	m_imgCollapse.LoadFromResource(AfxGetInstanceHandle(), IDB_COLLAPSE);
	m_imgExpand.LoadFromResource(AfxGetInstanceHandle(), IDB_EXPAND);
}

CUITreeCtrl::~CUITreeCtrl()
{
}


BOOL CUITreeCtrl::SetItemHeightEx(HTREEITEM hTreeItem, UINT cyItemHeight)
{
	int nOldItemHeight = GetItemHeight();
	TVITEMEX itemEx;
	itemEx.mask = TVIF_INTEGRAL;
	itemEx.hItem = hTreeItem;
	itemEx.iIntegral = cyItemHeight / nOldItemHeight;
	BOOL bRet = (BOOL)SendMessage(TVM_SETITEM, 0, LPARAM(&itemEx));
	return bRet;

}

BEGIN_MESSAGE_MAP(CUITreeCtrl, CTreeCtrl)
	ON_WM_PAINT()
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, &CUITreeCtrl::OnTvnSelchanged)
END_MESSAGE_MAP()



// CUITreeCtrl 消息处理程序



void CUITreeCtrl::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CTreeCtrl::OnPaint()

	DrawBackground(&dc);
	HTREEITEM hTreeItem = GetFirstVisibleItem();

	while (hTreeItem)
	{
		DrawItem(&dc, hTreeItem);
		hTreeItem = GetNextVisibleItem(hTreeItem);
	}
}



void CUITreeCtrl::DrawBackground(CDC *pDC)
{
	CRect rcClient;
	GetClientRect(rcClient);
	pDC->FillSolidRect(rcClient, RGB(240, 248, 251));
}

void CUITreeCtrl::DrawItem(CDC *pDC, HTREEITEM hTreeItem)
{
	CRect rcClient;
	GetClientRect(rcClient);
	BOOL bHasChild = ItemHasChildren(hTreeItem);
	if (bHasChild)
	{
		CRect rcItem;
		CRect rcIcon;
		CRect rcLable;
		CRect rcSelectedItem;

		GetItemRect(hTreeItem, rcItem, TRUE);
		rcItem.right = rcClient.right;

		rcIcon = rcItem;
		rcIcon.right = rcItem.left + rcIcon.Height();
		rcIcon.DeflateRect(2, 2, 2, 2);

		rcLable = rcItem;
		rcLable.left = rcIcon.left + rcIcon.Height();

		CString strTest = GetItemText(hTreeItem);
		UINT uStatus = GetItemState(hTreeItem, TVIS_EXPANDED);
	/*	UINT uStatusSelected = GetItemState(hTreeItem, TVIS_SELECTED);
		if (uStatusSelected & TVIS_SELECTED)
		{
			pDC->FillSolidRect(rcItem, RGB(252, 240, 192));

		} 
		else
		{
			pDC->FillSolidRect(rcItem, RGB(240, 248, 251));

		}*/

		if (uStatus&TVIS_EXPANDED)
		{
			m_imgExpand.Draw(pDC->m_hDC, rcIcon);
		}
		else
		{
			m_imgCollapse.Draw(pDC->m_hDC, rcIcon);

		}
		pDC->DrawText(strTest, rcLable, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_WORD_ELLIPSIS);
	}
	else
	{
CRect rcItem;
		CRect rcIcon;
		CRect rcLable;
		CRect rcSelectedItem;

		GetItemRect(hTreeItem, rcItem, TRUE);
		rcItem.right = rcClient.right;
		rcSelectedItem = rcItem;
		rcSelectedItem.left = 0;

		rcIcon = rcItem;
		rcIcon.right = rcItem.left + rcIcon.Height();
		rcIcon.DeflateRect(2, 2, 2, 2);

		rcLable = rcItem;
		rcLable.left = rcIcon.left + rcIcon.Height();

		CString strTest = GetItemText(hTreeItem);
		UINT uStatus = GetItemState(hTreeItem, TVIS_EXPANDED);
		UINT uStatusSelected = GetItemState(hTreeItem, TVIS_SELECTED);
		if (uStatusSelected & TVIS_SELECTED)
		{
			pDC->FillSolidRect(rcSelectedItem, RGB(252, 240, 192));

		} 
		else
		{
			pDC->FillSolidRect(rcSelectedItem, RGB(240, 248, 251));

		}

		HICON hIcon = (HICON)GetItemData(hTreeItem);
		if (hIcon)
		{
			::DrawIconEx(pDC->m_hDC, rcIcon.left, rcIcon.top, hIcon, rcIcon.Width(), rcIcon.Height(), 0, 0, DI_NORMAL);
		}
		pDC->DrawText(strTest, rcLable, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_WORD_ELLIPSIS);
	}
}

void CUITreeCtrl::OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	SetRedraw(FALSE);
	if (pNMTreeView->itemNew.hItem)
	{
		BOOL bHasChild = ItemHasChildren(pNMTreeView->itemNew.hItem);
		if (!bHasChild)
		{
			SetItemHeightEx(pNMTreeView->itemNew.hItem, 80);
		}
	}

	if (pNMTreeView->itemOld.hItem)
	{
		BOOL bHasChild = ItemHasChildren(pNMTreeView->itemOld.hItem);
		if (!bHasChild)
		{
			SetItemHeightEx(pNMTreeView->itemOld.hItem, 40);
		}
	}
	SetRedraw(TRUE);
	*pResult = 0;
}
