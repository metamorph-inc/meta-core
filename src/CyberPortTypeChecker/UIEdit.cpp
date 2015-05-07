// UIEdit.cpp : implementation file
//

#include "stdafx.h"
#include "UIEdit.h"
#include "resource.h"

// UIEdit

IMPLEMENT_DYNAMIC(UIEdit, CEdit)

UIEdit::UIEdit()
{

}

UIEdit::~UIEdit()
{
}


BEGIN_MESSAGE_MAP(UIEdit, CEdit)
	ON_WM_CONTEXTMENU()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()



// UIEdit message handlers




void UIEdit::OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/)
{
	// TODO: Add your message handler code here
	CPoint	point;	// current pos for cursor
	CMenu	menu;
	menu.CreatePopupMenu();
	menu.LoadMenu(PROPERTY_EDITOR_BAR_RCLK_MENU);
	GetCursorPos(&point);
	UINT selectedMenuItem = menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN | TPM_TOPALIGN | TPM_RETURNCMD | TPM_LEFTBUTTON | TPM_NOANIMATION,
															   point.x,
															   point.y,
															   this);

	switch(selectedMenuItem)
	{
		case ID_PROPEDIT_COPYSELECTED:
			Copy();
			break;
		//case ID_RCLKMENU_PASTE:
		//	Paste();
		//	break;
		//case ID_RCLKMENU_CLEAR:
		//	Clear();
		//	break;	
	}
}


void UIEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	if(GetKeyState(VK_CONTROL) < 0)
	{
		switch(nChar)
		{
			case _T('A'):
				this->SetSel(0, this->LineLength());
				break;
		}
	}
	else
	CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}
