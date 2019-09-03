// AddressComboBox.cpp : 实现文件
//

#include "stdafx.h"
#include "BrowserFrame.h"
#include "AddressComboBox.h"
#include "resource.h"


// CAddressComboBox

IMPLEMENT_DYNAMIC(CAddressComboBox, CComboBox)

CAddressComboBox::CAddressComboBox()
{
	m_bIsBtnHover = FALSE;
	m_bIsBtnDown = FALSE;
}

CAddressComboBox::~CAddressComboBox()
{
}


int CAddressComboBox::AddString(LPCTSTR lpszString, HICON hIcon)
{
	int nItem = CComboBox::AddString(lpszString);
	SetItemData(nItem, (DWORD)hIcon);
	return nItem;	
}

BEGIN_MESSAGE_MAP(CAddressComboBox, CComboBox)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
END_MESSAGE_MAP()



// CAddressComboBox 消息处理程序



void CAddressComboBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{

	// TODO:  添加您的代码以绘制指定项
	HDC hDC = lpDrawItemStruct->hDC;
	int nItem = lpDrawItemStruct->itemID;
	CRect rcItem = lpDrawItemStruct->rcItem;
	HICON hIcon = (HICON)lpDrawItemStruct->itemData;
	if (hIcon)
	{
		DrawIconEx(hDC, rcItem.left, rcItem.top, hIcon, 16, 16, 0, 0, DI_NORMAL);
	}
	CString strItem;
	GetLBText(nItem, strItem);
	if (!strItem.IsEmpty())
	{
		CRect rcText = rcItem;
		rcText.left = 20;
		DrawText(hDC, strItem, strItem.GetLength(), rcText, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_WORD_ELLIPSIS);
	}
	
}

void CAddressComboBox::MeasureItem(LPMEASUREITEMSTRUCT /*lpMeasureItemStruct*/)
{

	// TODO:  添加您的代码以确定指定项的大小
}

int CAddressComboBox::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CComboBox::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	SetItemHeight(-1, 22);
	if (!m_imgComboxBox.IsNull())
	{
		return 0;
	}

	if (!m_imgComboBoxBtn.IsNull())
	{
		return 0;
	}
	m_imgComboxBox.LoadFromResource(AfxGetInstanceHandle(), IDB_COMBOBOX);
	m_imgComboBoxBtn.LoadFromResource(AfxGetInstanceHandle(), IDB_COMBOBOX_BUTTON);
	return 0;
}

void CAddressComboBox::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CComboBox::OnPaint()
	CRect rcComboBox;
	GetClientRect(rcComboBox);
	if (m_imgComboxBox.IsNull())
	{
		return ;
	}
	if (m_imgComboBoxBtn.IsNull())
	{
		return;
	}
	//left
	CRect rcComboBoxLeft = rcComboBox;
	rcComboBoxLeft.right = 10;
	m_imgComboxBox.Draw(dc.m_hDC, rcComboBoxLeft, CRect(0, 0, 10, 28));

	//top
	CRect rcComboBoxTop = rcComboBox;
	rcComboBoxTop.left = 10;
	rcComboBoxTop.right = rcComboBox.right;
	rcComboBoxTop.top = 0;
	rcComboBoxTop.bottom = 5;
	m_imgComboxBox.Draw(dc.m_hDC, rcComboBoxTop, CRect(10, 0, 20, 5));

	//bottom
	CRect rcComboBoxBottom = rcComboBox;
	rcComboBoxBottom.left = 10;
	rcComboBoxBottom.right = rcComboBox.right;
	rcComboBoxBottom.top = 23;
	rcComboBoxBottom.bottom = 28;
	m_imgComboxBox.Draw(dc.m_hDC, rcComboBoxBottom, CRect(10, 23, 20, 28));

	//right
	COMBOBOXINFO comboboxinfo;
	comboboxinfo.cbSize = sizeof(COMBOBOXINFO);
	GetComboBoxInfo(&comboboxinfo);
	CRect rcEdit;
	::GetClientRect(comboboxinfo.hwndItem, rcEdit);
	CRect rcComboBoxBtn = rcComboBox;
	rcComboBoxBtn.left = 10 + rcEdit.Width();
	rcComboBoxBtn.right = rcComboBox.right;
	rcComboBoxBtn.top = 5;
	rcComboBoxBtn.bottom = 23;

	if (m_bIsBtnHover)
	{
		if (m_bIsBtnDown)
		{
			m_imgComboBoxBtn.Draw(dc.m_hDC, rcComboBoxBtn, CRect(42, 0, 63, 21));
		}
		else
		{
			m_imgComboBoxBtn.Draw(dc.m_hDC, rcComboBoxBtn, CRect(21, 0, 42, 21));
		}
	}
	else
	{
		m_imgComboBoxBtn.Draw(dc.m_hDC, rcComboBoxBtn, CRect(0, 0, 21, 21));
	}
	
}

void CAddressComboBox::OnSize(UINT nType, int cx, int cy)
{
	CComboBox::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	COMBOBOXINFO comboboxinfo;
	comboboxinfo.cbSize = sizeof(COMBOBOXINFO);
	GetComboBoxInfo(&comboboxinfo);

	CRect rcEdit;
	rcEdit.left = 10;
	rcEdit.top = 5;
	rcEdit.bottom = 23;
	rcEdit.right = comboboxinfo.rcButton.left;

	::MoveWindow(comboboxinfo.hwndItem, rcEdit.left, rcEdit.top, rcEdit.Width(), rcEdit.Height(), TRUE);



}

void CAddressComboBox::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	TRACKMOUSEEVENT tme;
	tme.cbSize = sizeof(TRACKMOUSEEVENT);
	tme.hwndTrack = m_hWnd;
	tme.dwFlags = TME_HOVER | TME_LEAVE;
	tme.dwHoverTime = HOVER_DEFAULT;
	TrackMouseEvent(&tme);
	CComboBox::OnMouseMove(nFlags, point);
}

LRESULT CAddressComboBox::OnMouseHover(WPARAM wParam, LPARAM lParam)
{
	CPoint pt;
	pt.x = LOWORD(lParam);
	pt.y = HIWORD(lParam);
	if (!m_bIsBtnHover)
	{
		CRect rcComboBox;
		GetClientRect(rcComboBox);
		COMBOBOXINFO comboboxinfo;
		comboboxinfo.cbSize = sizeof(COMBOBOXINFO);
		GetComboBoxInfo(&comboboxinfo);
		CRect rcEdit;
		::GetClientRect(comboboxinfo.hwndItem, rcEdit);
		CRect rcComboBoxBtn = rcComboBox;
		rcComboBoxBtn.left = 10 + rcEdit.Width();
		rcComboBoxBtn.right = rcComboBox.right;
		rcComboBoxBtn.top = 5;
		rcComboBoxBtn.bottom = 23;

		if (rcComboBoxBtn.PtInRect(pt))
		{
			m_bIsBtnHover = TRUE;
			if (wParam == MK_LBUTTON)
			{
				m_bIsBtnDown = TRUE;
			}
			else
			{
				m_bIsBtnDown = FALSE;
			}
			
			Invalidate();
		}
		
	}
	return TRUE;
}

LRESULT CAddressComboBox::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	if (m_bIsBtnHover)
	{
		m_bIsBtnHover = FALSE;
		m_bIsBtnDown = FALSE;
		Invalidate();
	}
	return TRUE;
}