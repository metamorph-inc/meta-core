#pragma once
#include "UIListBox.h"

class UIConsole
{
public:
	UIConsole(void);
	~UIConsole(void);
	BOOL initialize(CString captionText, UINT captionBoxCtrlId, const CRect rect, CWnd * parentWnd, UINT dispBoxCtrlID);

private:

public:
	static const UINT	ERROR_ITEM_CONSOLE = 0;
	static const UINT	NORMAL_ITEM_CONSOLE = 1;
	static const UINT	WARNING_ITEM_CONSOLE = 2;
	
	CStatic				caption;
	UIListBox			listBox;
};

