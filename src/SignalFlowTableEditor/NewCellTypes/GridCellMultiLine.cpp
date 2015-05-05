// GridCellMultiLine.cpp : implementation file
//
// MFC Grid Control - Main grid cell multiline class
//
// Provides the implementation for a multiline cell type of the
// grid control.
//
// Written by Chris Maunder <cmaunder@mail.com>
// Copyright (c) 1998-2000. All Rights Reserved.
//
// Parts of the code contained in this file are based on the original
// CInPlaceMultiLine from http://www.codeguru.com/listview
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
// History:
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GridCellMultiLine.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// CInPlaceMultiLine

CInPlaceMultiLine::CInPlaceMultiLine(CWnd* pParent, CRect& rect, DWORD dwStyle, UINT nID,
						   int nRow, int nColumn, CString sInitText, 
						   UINT nFirstChar)
{
    if (pParent->IsKindOf(RUNTIME_CLASS(CGridCtrl)))
		m_pParentGC	= pParent;
	else
		m_pParentGC	= NULL;
    m_sInitText     = sInitText;
    m_nRow          = nRow;
    m_nColumn       = nColumn;
    m_nLastChar     = 0; 
    m_bExitOnArrows = (nFirstChar != VK_LBUTTON);    // If mouse click brought us here,
                                                     // then no exit on arrows

	m_bExitOnArrows = FALSE;
	
	m_bInEditModus	= FALSE;

    m_Rect = rect;  // For bizarre CE bug.
    
	//added code to properly size Edit box for the multiline text - Brian
	CWindowDC dc(this);
	CSize proper = GetTextExtentMultiLine(m_Rect.Width (), m_sInitText, &dc);
	m_Rect.right = m_Rect.left + proper.cx * 3 / 2;
	m_Rect.bottom = m_Rect.top + proper.cy;
	//end addition

    DWORD dwEditStyle = WS_THICKFRAME|WS_CHILD|WS_VISIBLE|WS_VSCROLL| WS_HSCROLL |
						ES_AUTOVSCROLL|ES_MULTILINE|ES_WANTRETURN|ES_LEFT|
						dwStyle;
	if (!Create(dwEditStyle, m_Rect, pParent, nID)) return;
	
	SetFont(pParent->GetFont());
	
	SetWindowText(m_sInitText);
	SetFocus();
	
	switch (nFirstChar){
		case VK_LBUTTON: 
		case VK_RETURN:   // break; //SetSel((int)_tcslen(m_sInitText), -1); return;
		case VK_BACK:	  SetSel((int)_tcslen(m_sInitText), -1); break;
		case VK_TAB:
		case VK_DOWN: 
		case VK_UP:   
		case VK_RIGHT:
		case VK_LEFT:  
		case VK_NEXT:  
		case VK_PRIOR: 
		case VK_HOME:
		case VK_SPACE:
		case VK_END:	  SetSel(0,-1); return;
		default:		  SetSel(0,-1);
	}
	
	SendMessage(WM_CHAR, nFirstChar);
}

CInPlaceMultiLine::~CInPlaceMultiLine()
{
}

BEGIN_MESSAGE_MAP(CInPlaceMultiLine, CEdit)
	//{{AFX_MSG_MAP(CInPlaceMultiLine)
	ON_WM_KILLFOCUS()
	ON_WM_CHAR()
	ON_WM_KEYDOWN()
	ON_WM_GETDLGCODE()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

////////////////////////////////////////////////////////////////////////////
// CInPlaceMultiLine message handlers

// If an arrow key (or associated) is pressed, then exit if
//	a) The Ctrl key was down, or
//	b) m_bExitOnArrows == TRUE
void CInPlaceMultiLine::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	m_bInEditModus = TRUE;

	if ((nChar == VK_PRIOR || nChar == VK_NEXT ||
		nChar == VK_DOWN  || nChar == VK_UP   ||
		nChar == VK_RIGHT || nChar == VK_LEFT) &&
		(m_bExitOnArrows || GetKeyState(VK_CONTROL) < 0))
	{
		m_nLastChar = nChar;
		GetParent()->SetFocus();
		return;
	}
	
	CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}

// As soon as this edit loses focus, kill it.
void CInPlaceMultiLine::OnKillFocus(CWnd* pNewWnd)
{
	CEdit::OnKillFocus(pNewWnd);
	EndEdit();
}

void CInPlaceMultiLine::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_TAB ) //|| nChar == VK_RETURN)
	{
		m_nLastChar = nChar;
		GetParent()->SetFocus();	// This will destroy this window
		return;
	}
	if (nChar == VK_ESCAPE) 
	{
		SetWindowText(m_sInitText);    // restore previous text
		m_nLastChar = nChar;
		GetParent()->SetFocus();
		return;
	}
	if (nChar == VK_RETURN && !m_bInEditModus)
	{
		return;
	}

	CEdit::OnChar(nChar, nRepCnt, nFlags);
	
	// Resize edit control if needed
	
	// Get text extent
	CString str;
	GetWindowText( str );

	// add some extra buffer
	str += _T("  ");
	
	CWindowDC dc(this);
	CFont *pFontDC = dc.SelectObject(GetFont());
//	  CSize size = dc.GetTextExtent( str );
	CSize size = GetTextExtentMultiLine(m_Rect.Width (), str, &dc);

	dc.SelectObject( pFontDC );
	   
	// Get client rect
	CRect ParentRect;
	GetParent()->GetClientRect( &ParentRect );
	
	// Check whether control needs to be resized
	// and whether there is space to grow
	if (size.cx > m_Rect.Width())
	{
		if( size.cx + m_Rect.left < ParentRect.right )
			m_Rect.right = m_Rect.left + size.cx;
		else
			m_Rect.right = ParentRect.right;
		MoveWindow( &m_Rect );
	}
}

UINT CInPlaceMultiLine::OnGetDlgCode() 
{
	return DLGC_WANTALLKEYS;
}

////////////////////////////////////////////////////////////////////////////
// CInPlaceMultiLine overrides

// Stoopid win95 accelerator key problem workaround - Matt Weagle.
BOOL CInPlaceMultiLine::PreTranslateMessage(MSG* pMsg) 
{
	// Catch the Alt key so we don't choke if focus is going to an owner drawn button
	if (pMsg->message == WM_SYSCHAR)
		return TRUE;
	
	return CWnd::PreTranslateMessage(pMsg);
}

// Auto delete
void CInPlaceMultiLine::PostNcDestroy() 
{
	CEdit::PostNcDestroy();
	
	delete this;	
}

////////////////////////////////////////////////////////////////////////////
// CInPlaceMultiLine implementation

void CInPlaceMultiLine::EndEdit()
{
	CString str;

    // EFW - BUG FIX - Clicking on a grid scroll bar in a derived class
    // that validates input can cause this to get called multiple times
    // causing assertions because the edit control goes away the first time.
    static BOOL bAlreadyEnding = FALSE;

    if(bAlreadyEnding)
        return;

    bAlreadyEnding = TRUE;
    GetWindowText(str);

	//if (m_pParentGC!=NULL)
	//{
	//	if ( !((CGridCtrl*)m_pParentGC)->GetItemNullable(m_nRow, m_nColumn) &&	// Cell is NOT nullable and
	//		str.IsEmpty()														// String is empty
	//		) 
	//	{
	//		str = m_sInitText;
	//		SetWindowText(str);
	//		
	//		AfxMessageBox("Diese Zelle darf nicht leer sein !\nDer vorherige Inhalt wurde wiederhergestellt !", 
	//						MB_OK|MB_APPLMODAL|MB_ICONEXCLAMATION,
	//						0);
	//	}
	//}

    // Send Notification to parent
    GV_DISPINFO dispinfo;

    dispinfo.hdr.hwndFrom = GetSafeHwnd();
    dispinfo.hdr.idFrom   = GetDlgCtrlID();
    dispinfo.hdr.code     = GVN_ENDLABELEDIT;

    dispinfo.item.mask    = LVIF_TEXT|LVIF_PARAM;
    dispinfo.item.row     = m_nRow;
    dispinfo.item.col     = m_nColumn;
    dispinfo.item.strText  = str;
    dispinfo.item.lParam  = (LPARAM) m_nLastChar;

    CWnd* pOwner = GetOwner();
    if (pOwner)
        pOwner->SendMessage(WM_NOTIFY, GetDlgCtrlID(), (LPARAM)&dispinfo );

    // Close this window (PostNcDestroy will delete this)
    if (IsWindow(GetSafeHwnd()))
        SendMessage(WM_CLOSE, 0, 0);
    bAlreadyEnding = FALSE;
}

CSize CInPlaceMultiLine::GetTextExtentMultiLine(int width, LPCTSTR szText, CDC* pDC /*= NULL*/)
{ 
	// If the cell is a multiline cell, then use the width of the cell
	// to get the height
	
	int nFormat = 0;
	CSize sz;
	CRect rect;
	
	nFormat = DT_LEFT|DT_WORDBREAK;
	sz.cx = sz.cy = 0;
	rect.SetRect(0, 0, width, 0);

	pDC->DrawText(szText, -1, rect, nFormat | DT_CALCRECT);
	
	sz = rect.Size();

	return sz;	
}

/////////////////////////////////////////////////////////////////////////////
// CGridCellMultiLine 
/////////////////////////////////////////////////////////////////////////////


IMPLEMENT_DYNCREATE(CGridCellMultiLine, CGridCell)

CGridCellMultiLine::CGridCellMultiLine() : CGridCell()
{
	SetStyle(ES_LEFT|ES_MULTILINE|ES_AUTOVSCROLL|ES_WANTRETURN);  // CBS_DROPDOWN, CBS_DROPDOWNLIST, CBS_SIMPLE, CBS_SORT

	SetHoldWidth ( TRUE);
}

// Create a control to do the editing
BOOL CGridCellMultiLine::Edit(int nRow, int nCol, CRect rect, CPoint /* point */, UINT nID, UINT nChar)
{
	m_bEditing = TRUE;
	
	// CInPlaceMultiLine auto-deletes itself
	m_pEditWnd = new CInPlaceMultiLine (GetGrid(), rect, GetStyle(), nID, nRow, nCol, GetText(), nChar);

	return TRUE;
}

// Cancel the editing.
void CGridCellMultiLine::EndEdit()
{
	if (m_pEditWnd)
		((CInPlaceMultiLine*)m_pEditWnd)->EndEdit();
}

// Override draw so that when the cell is selected, a drop arrow is shown in the RHS.
BOOL CGridCellMultiLine::Draw(CDC* pDC, int nRow, int nCol, CRect rect,  BOOL bEraseBkgnd /*=TRUE*/)
{
	return CGridCell::Draw(pDC, nRow, nCol, rect,  bEraseBkgnd);
}

// This hack allows you to determine the height that a cell should be in order to display
// stuff properly in the given width.
CSize CGridCellMultiLine::GetTextExtentEx(int width, LPCTSTR szText, CDC* pDC /*= NULL*/)
{
	CGridCtrl* pGrid = GetGrid();
	ASSERT(pGrid);
	
	BOOL bReleaseDC = FALSE;
	if (pDC == NULL)
	{
		pDC = pGrid->GetDC();
		if (!pDC) 
		{
			CGridDefaultCell* pDefCell = (CGridDefaultCell*) GetDefaultCell();
			ASSERT(pDefCell);
			return CSize(pDefCell->GetWidth(), pDefCell->GetHeight());
		}
		bReleaseDC = TRUE;
	}
	
	CFont *pOldFont = NULL,
	*pFont = GetFontObject();
	if (pFont)
		pOldFont = pDC->SelectObject(pFont);
	
	CSize size;
	int nFormat = GetFormat();
	TEXTMETRIC tm;
	pDC->GetTextMetrics(&tm);

	int textWidth = width - (4*GetMargin ());

	// corrects the bug if resizing column gives a text width smaller than (4*Getmargin())
	if (textWidth <= 0) 
	{
		textWidth = 1;
	}
	
	// If the cell is a multiline cell, then use the width of the cell
	// to get the height
	if ((nFormat & DT_WORDBREAK) && !(nFormat & DT_SINGLELINE))
	{
		CRect rect;
		rect.SetRect(0, 0, textWidth, 0);
		pDC->DrawText(szText, -1, rect, nFormat | DT_CALCRECT);

		size.cx = rect.Width ();
		size.cy = rect.Height ();
	}
	else
		size = pDC->GetTextExtent(szText, _tcslen(szText));
	
	size.cx += (tm.tmOverhang);
	
	if (pOldFont)
		pDC->SelectObject(pOldFont);
	
	size += CSize(4*GetMargin(), 2*GetMargin());
	
	// Kludge for vertical text
	LOGFONT *pLF = GetFont();
	if (pLF->lfEscapement == 900 || pLF->lfEscapement == -900)
	{
		int nTemp = size.cx;
		size.cx = size.cy;
		size.cy = nTemp;
		size += CSize(0, 4*GetMargin());
	}
	
	if (bReleaseDC)
		pGrid->ReleaseDC(pDC);
	
	return size;
}

// Hack for AutoSizeRowEx()...
CSize CGridCellMultiLine::GetCellExtentEx(int width, CDC*pDC)
{
	CSize ImageSize(0,0);
	
	int nImage = GetImage();
	if (nImage >= 0) 
	{
		CGridCtrl* pGrid = GetGrid();
		ASSERT(pGrid);
		
		if (pGrid->GetImageList()) 
		{
			IMAGEINFO Info;
			if (pGrid->GetImageList()->GetImageInfo(nImage, &Info))
				ImageSize = CSize(Info.rcImage.right-Info.rcImage.left+1, 
				Info.rcImage.bottom-Info.rcImage.top+1);
		}
	}
	
	
	CSize size = GetTextExtentEx(width - ImageSize.cx, GetText(), pDC);

	CSize retSize(size.cx + ImageSize.cx, max(size.cy, ImageSize.cy));

	return retSize;
}


