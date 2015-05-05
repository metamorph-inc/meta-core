#if !defined(AFX_GRIDCELLMULTILINE_H__ECD42822_16DF_11D1_992F_895E185F9C72__INCLUDED_)
#define AFX_GRIDCELLMULTILINE_H__ECD42822_16DF_11D1_992F_895E185F9C72__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// GRIDCELLMULTILINE.h : header file
//
// MFC Grid Control - Grid cell multiline class header file
//
// Written by Chris Maunder <cmaunder@mail.com>
// Copyright (c) 1998-2000. All Rights Reserved.
//
// This code may be used in compiled form in any way you desire. This
// file may be redistributed unmodified by any means PROVIDING it is 
// not sold for profit without the authors written consent, and 
// providing that this notice and the authors name and all copyright 
// notices remains intact. 
//
// An email letting me know how you are using it would be nice as well. 
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability for any damage/loss of business that
// this product may cause.
//
// For use with CGridCtrl v2.10
//
//////////////////////////////////////////////////////////////////////


#include "../GridCtrl_src/GridCell.h"
#include "../GridCtrl_src/GridCtrl.h"


class CGridCellMultiLine : public CGridCell
{
	friend class CGridCtrl;
	DECLARE_DYNCREATE(CGridCellMultiLine)

public:
	CGridCellMultiLine();

// editing cells
public:
	virtual BOOL Edit(int nRow, int nCol, CRect rect, CPoint point, UINT nID, UINT nChar);
	virtual void EndEdit();

// CGridCellMultiLine specific calls
public:
	void	SetStyle(DWORD dwStyle) { m_dwStyle = dwStyle; }
	DWORD	GetStyle()				{ return m_dwStyle;    }

	void	SetHoldWidth(BOOL bHoldWidth = TRUE)	{ m_bHoldWidth = bHoldWidth; }
	BOOL	GetHoldWidth()							{ return m_bHoldWidth; }

	// additional function for using multiline cells
	CSize	GetTextExtentEx(int width, LPCTSTR str, CDC* pDC = NULL);
	CSize	GetCellExtentEx(int width,CDC* pDC);


protected:
	virtual BOOL	Draw(CDC* pDC, int nRow, int nCol, CRect rect, BOOL bEraseBkgnd = TRUE);

	CStringArray	m_Strings;
	DWORD			m_dwStyle;
	BOOL			m_bHoldWidth;					// beibehalten der Spaltenbreite 
};


/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// CInPlaceMultiLine window

class CInPlaceMultiLine : public CEdit
{
// Construction
public:
	CInPlaceMultiLine(CWnd* pParent, CRect& rect, DWORD dwStyle, UINT nID,
				 int nRow, int nColumn, CString sInitText, UINT nFirstChar);

// Attributes
public:
 
// Operations
public:
	 void EndEdit();
	 CSize GetTextExtentMultiLine(int width, LPCTSTR szText, CDC* pDC /*= NULL*/);
 
// Overrides
	 // ClassWizard generated virtual function overrides
	 //{{AFX_VIRTUAL(CInPlaceMultiLine)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL
 
// Implementation
public:
	 virtual ~CInPlaceMultiLine();
 
// Generated message map functions
protected:
	//{{AFX_MSG(CInPlaceMultiLine)
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg UINT OnGetDlgCode();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
    CWnd*	m_pParentGC;
    int     m_nRow;
    int     m_nColumn;
    CString m_sInitText;
    UINT    m_nLastChar;
    BOOL    m_bExitOnArrows;
    CRect   m_Rect;

	BOOL	m_bInEditModus;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRIDCELLMULTILINE_H__ECD42822_16DF_11D1_992F_895E185F9C72__INCLUDED_)
