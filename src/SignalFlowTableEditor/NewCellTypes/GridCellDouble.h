// GridCellDouble.h: a cell for editing doubles
//
// still passes data in and out as strings, but restricts you
// while editing
//
// For use with CGridCtrl v2.22+
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DOUBLECELL_H__A0B7DA0A_0AFE_4567_A00E_846C96D7507A__INCLUDED_)
#define AFX_DOUBLECELL_H__A0B7DA0A_0AFE_4567_A00E_846C96D7507A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../GridCtrl_src/GridCell.h"
#include "../GridCtrl_src/InPlaceEdit.h"	// for making CInPlaceDoubleEdit

class CGridCellDouble : public CGridCell  
{
  friend class CGridCtrl;
  DECLARE_DYNCREATE(CGridCellDouble)

  //CTime m_cTime;
  DWORD m_dwStyle;

public:
	CGridCellDouble();
	virtual ~CGridCellDouble();

  // editing cells
public:
	virtual BOOL  Edit(int nRow, int nCol, CRect rect, CPoint point, UINT nID, UINT nChar);

};

class CInPlaceDoubleEdit : public CInPlaceEdit
{
// Construction
public:
	    CInPlaceDoubleEdit(CWnd* pParent, CRect& rect, DWORD dwStyle, UINT nID,
                 int nRow, int nColumn, CString sInitText, UINT nFirstChar);


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInPlaceList)
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	//virtual ~CInPlaceDoubleEdit();

// Generated message map functions
protected:
	//{{AFX_MSG(CInPlaceList)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

private:

};

#endif // !defined(AFX_DOUBLECELL_H__A0B7DA0A_0AFE_4567_A00E_846C96D7507A__INCLUDED_)
