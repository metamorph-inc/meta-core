#include "StdAfx.h"
#include "UIPropEditorBar.h"
#include "resource.h"

UIPropEditorBar::UIPropEditorBar(void)
{
}


UIPropEditorBar::~UIPropEditorBar(void)
{
}


BOOL UIPropEditorBar::intialize(CWnd* pParentWnd, CRect rect, UINT capBoxCtrlId, UINT capTextCtrlId, UINT contentBoxCtrlId, BOOL editable, BOOL dropDown)
{
	// initialize Caption
	CString captionText;
	DWORD	dwStyle;
	CRect captionRect(rect.left, rect.top, rect.left + CAPTION_WIDTH, rect.bottom);	// Caption Box Area
	CRect contentRect(captionRect.right, rect.top, rect.right, rect.bottom);		// Display Box Area

	// Create Caption as the CStatic
	captionText.LoadString(capTextCtrlId);
	if(!this->caption.Create(captionText, 
							WS_CHILD | WS_VISIBLE | WS_BORDER, 
							captionRect,
							pParentWnd,
							contentBoxCtrlId))
	{
		AfxMessageBox(_T("Failed to Create \"") + captionText + _T("\"'s Caption Box! Closing."));
		return FALSE;
	}

	this->dropDown = dropDown;

	if(!dropDown)
	{
		dwStyle = WS_CHILD | WS_VISIBLE /*| WS_DISABLED */| WS_BORDER | ES_AUTOHSCROLL | ES_LEFT;
	
		if(!editable)
			dwStyle |= ES_READONLY;

		if(!editor.Create(dwStyle, contentRect, pParentWnd, contentBoxCtrlId))
		{
			AfxMessageBox(_T("Failed to Create \"") + captionText + _T("\"'s Content Viewer/Editor Box! Closing."));
			return FALSE;
		}


		// Diable the editor bar currently
		disable();
	}
	else
	{
		dwStyle = WS_CHILD | WS_VISIBLE | CBS_AUTOHSCROLL | CBS_HASSTRINGS;

		if(!editable)
			dwStyle |= CBS_DROPDOWNLIST;
		else
			dwStyle |= CBS_DROPDOWN;

		// Adjust the dropdown icon to come within window
		contentRect.right -= DROPDOWN_ICON_WIDTH;

		if(!dropDownViewer.Create(dwStyle, contentRect, pParentWnd, contentBoxCtrlId))
		{
			AfxMessageBox(_T("Failed to Create \"") + captionText + _T("\"'s Content Viewer/Editor Box! Closing."));
			return FALSE;
		}

		// Insert default string
		CString defStr;
		defStr.LoadString(PROPERTY_EDITORBAR_DROPDOWN_DEFAULT_TEXT);
		dropDownViewer.AddString(defStr);

		// Disable the dropdown
		disable();
	}
	return TRUE;
}

void UIPropEditorBar::showText(LPCTSTR str)
{
	if(!dropDown)
		editor.SetWindowText(str);
	else
	{
		if(dropDownViewer.SelectString(-1, str) == CB_ERR)
		{
			// if string not found show NO TYPE
			CString defStr;
			defStr.LoadString(PROPERTY_EDITORBAR_DROPDOWN_DEFAULT_TEXT);
			dropDownViewer.SelectString(-1, defStr);
		}
	}
}

void UIPropEditorBar::clearEditorText()
{
	editor.Clear();
}

void UIPropEditorBar::addDropDownString(LPCTSTR str)
{
	// if string not already present in the drop down list then add it
	if(dropDownViewer.FindStringExact(-1, str) == CB_ERR)
	{
		dropDownViewer.AddString(str);
	}
}


void UIPropEditorBar::disable()
{
	if(!dropDown)
		editor.ModifyStyle(NULL, WS_DISABLED);
	else
		dropDownViewer.ModifyStyle(NULL, WS_DISABLED);
}

void UIPropEditorBar::enable()
{
	if(!dropDown)
		editor.ModifyStyle(WS_DISABLED, NULL);
	else
		dropDownViewer.ModifyStyle(WS_DISABLED, NULL);
}
