/*----------------------------------------------------------------------
Copyright (C)2001 MJSoft. All Rights Reserved.
          This source may be used freely as long as it is not sold for
					profit and this copyright information is not altered or removed.
					Visit the web-site at www.mjsoft.co.uk
					e-mail comments to info@mjsoft.co.uk
File:     SortListCtrl.h
Purpose:  Provides a sortable list control, it will sort text, numbers
          and dates, ascending or descending, and will even draw the
					arrows just like windows explorer!
----------------------------------------------------------------------*/

#ifndef SORTLISTCTRL_H
#define SORTLISTCTRL_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef SORTHEADERCTRL_H
	#include "SortHeaderCtrl.h"
#endif	// SORTHEADERCTRL_H

#ifdef _DEBUG
	#define ASSERT_VALID_STRING( str ) ASSERT( !IsBadStringPtr( str, 0xfffff ) )
#else	//	_DEBUG
	#define ASSERT_VALID_STRING( str ) ( (void)0 )
#endif	//	_DEBUG


class CSortListCtrl : public CListCtrl
{
// Construction
public:
	CSortListCtrl();

// Attributes
public:

// Operations
public:
	BOOL SetHeadings( UINT uiStringID );
	BOOL SetHeadings( const CString& strHeadings );

	BOOL initHeadListCtr(BOOL showHeadCheckBox=true);

	int AddItem(int id, LPCTSTR pszText, ... );
	BOOL DeleteItem( int iItem );
	BOOL DeleteAllItems();
	void LoadColumnInfo();
	void SaveColumnInfo();
	BOOL SetItemText( int nItem, int nSubItem, LPCTSTR lpszText );
	void Sort( int iColumn, BOOL bAscending );
	void Sort();
	BOOL SetItemData(int nItem, DWORD dwData);
	DWORD GetItemData(int nItem) const;
	int GetItemID(int nItem);
	void SetColumnNum(int numColumns);
	void TurnOffToolTip(){m_showInfoTip = FALSE;};
	void TurnOnToolTip() {m_showInfoTip = TRUE; };
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSortListCtrl)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

	void CellHitTest(const CPoint& pt, int& nRow, int& nCol) const;
	bool ShowToolTip(const CPoint& pt) const;
	CString GetToolTipText(int nRow, int nCol);

// Implementation
public:
	virtual ~CSortListCtrl();

	// Generated message map functions
protected:
	static int CALLBACK CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamData );
	void FreeItemMemory( const int iItem );
	BOOL CSortListCtrl::SetTextArray( int iItem, LPTSTR* arrpsz, int id );
	LPTSTR* CSortListCtrl::GetTextArray( int iItem ) const;

	CSortHeaderCtrl m_ctlHeader;
	CImageList	m_checkImgList;
	int m_iNumColumns;
	int m_iSortColumn;
	BOOL m_bSortAscending;
	BOOL m_showInfoTip;

	CString m_pwchTip;

	//{{AFX_MSG(CSortListCtrl)
	afx_msg void OnColumnClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDestroy();
	//}}AFX_MSG

	//for infotip
	afx_msg BOOL OnToolNeedText(UINT id, NMHDR* pNMHDR, LRESULT* pResult);

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // SORTLISTCTRL_H
