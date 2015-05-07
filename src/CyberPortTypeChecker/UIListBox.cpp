// UIListBox.cpp : implementation file
//

#include "stdafx.h"
#include "UIListBox.h"
#include "resource.h"
#include "MyColors.h"
// UIListBox

IMPLEMENT_DYNAMIC(UIListBox, CListBox)

UIListBox::UIListBox()
{
}

UIListBox::~UIListBox()
{
}


BEGIN_MESSAGE_MAP(UIListBox, CListBox)
	ON_WM_DRAWITEM_REFLECT()
	ON_WM_KEYDOWN()
	ON_WM_CONTEXTMENU()
	ON_WM_VKEYTOITEM_REFLECT()
	ON_CONTROL_REFLECT(LBN_SELCHANGE, &UIListBox::OnLbnSelchange)
END_MESSAGE_MAP()



// UIListBox message handlers
void copyContentToClipboard(HWND newOwnerHnd, CString contentStr)
{
	if ( !OpenClipboard(newOwnerHnd) )
	{
		AfxMessageBox( _T("Cannot open the Clipboard") );
		return;
	}
	// Remove the current Clipboard contents 
	if( !EmptyClipboard() )
	{
		AfxMessageBox( _T("Cannot empty the Clipboard") );
		return;
	}
				
	HGLOBAL hGlob = GlobalAlloc(GMEM_FIXED, (contentStr.GetLength() + 1) * sizeof(contentStr[0]));

	if(hGlob == NULL) 
	{
		CString msg;
		msg.Format(_T("Unable to set Clipboard data, error: %d"), GetLastError());
		AfxMessageBox( msg );
		CloseClipboard();
		return;
	}
					
	CString::CopyChars((LPTSTR) hGlob, (LPCTSTR) contentStr, contentStr.GetLength() + 1);

	if(SetClipboardData( CF_TEXT, hGlob ) == NULL)
	{
		CString msg;
		msg.Format(_T("Unable to set Clipboard data, error: %d"), GetLastError());
		AfxMessageBox( msg );
		CloseClipboard();
		GlobalFree(hGlob);
		return;
	}

	CloseClipboard();
}

void UIListBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{

	// TODO:  Add your code to draw the specified item
	CString		itemText;			// text from the current item
	CDC			dc;					// current device context
	CSize		sz;					// output text extent size for itemText
	COLORREF	crOldTextColor;		// old text color
	COLORREF	crOldBkColor;		// old background color
	CString		logErrPrefix;		// log statement prefix string
	CString		logNorPrefix;
	CString		logWarPrefix;

	// If there is no Item then do nothing
	if(lpDrawItemStruct->itemID == -1) return;

	// Get current item's text
	this->GetText(lpDrawItemStruct->itemID, itemText); 

	// Attach to the listbox's DC 
	dc.Attach(lpDrawItemStruct->hDC);
	
	// Save current color values
	crOldTextColor = dc.GetTextColor();
	crOldBkColor = dc.GetBkColor();

	if(lpDrawItemStruct->CtlID == LB_CONSOLE)
	{
		// Load Prefix from String Table
		logErrPrefix.LoadString(LOG_ERR_PREF);
		logNorPrefix.LoadString(LOG_NOR_PREF);
		logWarPrefix.LoadString(LOG_WAR_PREF);
	
		// check if the item text is an error log
		if(!itemText.Mid(10, logErrPrefix.GetLength()).Compare(logErrPrefix))
		{
			// Change text and bkcolor to red
			dc.SetTextColor(MY_RED);
			dc.SetBkColor(MY_LIGHTRED);
		}
		else if(!itemText.Mid(10, logNorPrefix.GetLength()).Compare(logNorPrefix))
		{
			dc.SetTextColor(MY_BLUE);
		}		
		else if(!itemText.Mid(10, logWarPrefix.GetLength()).Compare(logWarPrefix))
		{
			dc.SetTextColor(MY_ORANGE);
		}

	}

	// Set BK color greyish blue if the item is currently selected
	if(this->GetSel(lpDrawItemStruct->itemID))
		dc.SetBkColor(MY_GREYISHBLUE);
	

	// Draw the text.
	dc.DrawText(
		itemText,
		&lpDrawItemStruct->rcItem,
		DT_LEFT|DT_SINGLELINE|DT_NOCLIP);


	// Set the horizontal extent for enabling scrollbar
	sz = dc.GetOutputTextExtent(itemText);
	if(sz.cx > this->GetHorizontalExtent())
		this->SetHorizontalExtent(sz.cx);

	// Reset the background color and the text color back to their
	// original values.
	dc.SetTextColor(crOldTextColor);
	dc.SetBkColor(crOldBkColor);

	dc.Detach();
}

void UIListBox::selectAll()
{
	for(int i = 0; i < GetCount(); i++)
		SetSel(i, TRUE);
}

void UIListBox::copySelectedToClipboard()
{
	CArray<INT,INT> selItemArray;	// Selected items Array
	LPINT			pItem;
	CString			finalStr;

	selItemArray.SetSize(GetSelCount());
	GetSelItems(GetSelCount(), selItemArray.GetData()); 

	pItem = selItemArray.GetData();
	for(INT i = 0; i < GetSelCount(); i++, pItem++)
	{
		CString str;
		GetText(*pItem, str);
		finalStr += str + _T("\r\n");
	}
				
	// Get the currently selected data
	if(finalStr.GetLength())
	{
		copyContentToClipboard(this->GetSafeHwnd(), finalStr);
	}
}

void UIListBox::copyAllToClipboard()
{
	selectAll();
	copySelectedToClipboard();
}

void UIListBox::clearSelected()
{
	INT i=0;
	while(i<GetCount())
	{
		// if ith item is selected
		if(GetSel(i) > 0)
		{
			// Delete the ith item and do not increase i as the next item will take deleted item's place
			DeleteString(i);
			continue;
		}
		// if the ith item is not selected then check for the next item in the list
		++i;
	}
}

void UIListBox::clearAll()
{
	ResetContent();
}

void UIListBox::OnKeyDown(
                UINT nChar,
                UINT nRepCnt,
                UINT nFlags
                )
{
	if(GetKeyState(VK_CONTROL) < 0 && GetCount())
	{
		switch(nChar)
		{
			case _T('A'):
				selectAll();
				break;
			case _T('C'):
				copySelectedToClipboard();
				break;
		}
	}
	else
	CListBox::OnKeyDown(nChar, nRepCnt, nFlags);
}

void UIListBox::OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/)
{
	// TODO: Add your message handler code here
	CPoint	point;	// current pos for cursor
	CMenu	menu;
	menu.CreatePopupMenu();
	menu.LoadMenu(CONSOLE_MENU);
	GetCursorPos(&point);
	UINT selectedMenuItem = menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN | TPM_TOPALIGN | TPM_RETURNCMD | TPM_LEFTBUTTON | TPM_NOANIMATION,
															   point.x,
															   point.y,
															   this);

	switch(selectedMenuItem)
	{
		case ID_CONSOLEMENU_COPY:
			copySelectedToClipboard();
			break;
		case ID_CONSOLEMENU_COPYALL:
			copyAllToClipboard();
			break;
		case ID_CONSOLEMENU_CLEARALL:
			clearAll();
			break;
		case ID_CONSOLEMENU_CLEAR:
			clearSelected();
			break;
	}
}


int UIListBox::VKeyToItem(UINT nKey, UINT /*nIndex*/)
{
	// TODO:  Replace the next line with your message handler code
	switch(nKey)
	{
		// DEL key
		case VK_DELETE:
			clearSelected();
			break;
	}
	return -1;
}


void UIListBox::OnLbnSelchange()
{
	// TODO: Add your control notification handler code here
	
	// Update the display for all strings.
	this->RedrawWindow();
}
