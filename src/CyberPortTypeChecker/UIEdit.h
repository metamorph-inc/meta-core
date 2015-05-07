#pragma once


// UIEdit

class UIEdit : public CEdit
{
	DECLARE_DYNAMIC(UIEdit)

public:
	UIEdit();
	virtual ~UIEdit();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};


