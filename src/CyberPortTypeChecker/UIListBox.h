#pragma once

// UIListBox

class UIListBox : public CListBox
{
	DECLARE_DYNAMIC(UIListBox)

public:
	UIListBox();
	virtual ~UIListBox();

protected:
	DECLARE_MESSAGE_MAP()
private:
	CTime clock;
	void clearAll();
	void clearSelected();
	void selectAll();
	void copySelectedToClipboard();
	void copyAllToClipboard();
	afx_msg void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
public:
	afx_msg int VKeyToItem(UINT /*nKey*/, UINT /*nIndex*/);
	afx_msg void OnLbnSelchange();
};


