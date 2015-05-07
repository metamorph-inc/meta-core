#pragma once
#include "UIEdit.h"
#include "UIComboBox.h"

class UIPropEditorBar
{
public:
	UIPropEditorBar(void);
	~UIPropEditorBar(void);
	BOOL intialize(CWnd* parentWnd, CRect rect, UINT capBoxCtrlId, UINT capTextCtrlId, UINT contentBoxCtrlId, BOOL editable, BOOL dropDown);
	void showText(LPCTSTR str);
	void clearEditorText();
	void addDropDownString(LPCTSTR str);
	void disable();
	void enable();

private:
	static const UINT	CAPTION_WIDTH = 150;
	static const UINT	DROPDOWN_ICON_WIDTH = 10;
	CStatic				caption;
	UIEdit				editor;
	UIComboBox			dropDownViewer;
	BOOL				dropDown;
};

