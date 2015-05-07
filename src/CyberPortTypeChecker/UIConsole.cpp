#include "StdAfx.h"
#include "UIConsole.h"
#include "UI.h"


UIConsole::UIConsole(void)
{
}


UIConsole::~UIConsole(void)
{
}

BOOL UIConsole::initialize(CString captionText, UINT captionBoxCtrlId, const CRect rect, CWnd * parentWnd, UINT dispBoxCtrlID)
{
	CRect captionRect(rect.left, rect.top, rect.right, rect.top + UI::CAPTION_HEIGHT);	// Caption Box Area
	CRect displayRect(rect.left, captionRect.bottom, rect.right, rect.bottom);		// Display Box Area

	// Create Caption as the CStatic
	if(!this->caption.Create(captionText, 
							WS_CHILD | WS_VISIBLE | WS_BORDER, 
							captionRect,
							parentWnd,
							captionBoxCtrlId))
	{
		AfxMessageBox(_T("Failed to Create \"") + captionText + _T("\"'s Caption Box! Closing."));
		return FALSE;
	}

	// Create console /*for CListBox*/
	if(!this->listBox.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL | WS_HSCROLL | LBS_DISABLENOSCROLL | LBS_EXTENDEDSEL | LBS_MULTIPLESEL | LBS_HASSTRINGS | LBS_NOTIFY | LBS_OWNERDRAWFIXED | LBS_WANTKEYBOARDINPUT,
							displayRect,
							parentWnd, 
							dispBoxCtrlID)) 
	{	
		AfxMessageBox(_T("Failed to Create \"") + captionText + _T("\"'s Display Box! Closing."));
		return FALSE;
	}

	return TRUE;
}
