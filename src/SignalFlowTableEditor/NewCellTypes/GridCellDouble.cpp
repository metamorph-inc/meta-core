///////////////////////////////////////////////////////////////////////////
//
// GridCellDateTime.cpp: implementation of the CGridCellDateTime class.
//
// Provides the implementation for a datetime picker cell type of the
// grid control.
//
// Written by Podsypalnikov Eugen 15 Mar 2001
// Modified:
//    31 May 2001  Fixed m_cTime bug (Chris Maunder)
//
// For use with CGridCtrl v2.22+
//
///////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "../GridCtrl_src/GridCtrl.h"
#include "../GridCtrl_src/GridCell.h"
#include "GridCellDouble.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// CGridCellDateTime

IMPLEMENT_DYNCREATE(CGridCellDouble, CGridCell)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGridCellDouble::CGridCellDouble() : CGridCell()
{
	m_dwStyle = 0;
}


CGridCellDouble::~CGridCellDouble()
{
}

BOOL CGridCellDouble::Edit(int nRow, int nCol, CRect rect, CPoint /* point */, UINT nID, UINT nChar)
{
    if ( m_bEditing )
	{      
        if (m_pEditWnd)
		    m_pEditWnd->SendMessage ( WM_CHAR, nChar );    
    }  
	else  
	{   
		DWORD dwStyle = ES_LEFT;
		if (GetFormat() & DT_RIGHT) 
			dwStyle = ES_RIGHT;
		else if (GetFormat() & DT_CENTER) 
			dwStyle = ES_CENTER;

		m_bEditing = TRUE;
		
		// InPlaceEdit auto-deletes itself
		CGridCtrl* pGrid = GetGrid();
		m_pEditWnd = new CInPlaceDoubleEdit(pGrid, rect, dwStyle, nID, nRow, nCol, GetText(), nChar);
    }
    return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CInPlaceDoubleEdit


CInPlaceDoubleEdit::CInPlaceDoubleEdit(CWnd* pParent, CRect& rect, DWORD dwStyle, UINT nID,
                           int nRow, int nColumn, CString sInitText, 
                           UINT nFirstChar):CInPlaceEdit(pParent, rect, dwStyle, nID,
                           nRow, nColumn, sInitText, 
                           nFirstChar)
{
 //do nothing, just pass to the default constructor for CInPlaceEdit
	
}


BEGIN_MESSAGE_MAP(CInPlaceDoubleEdit, CInPlaceEdit)
	//{{AFX_MSG_MAP(CInPlaceDateTime)
    ON_WM_CHAR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()




void CInPlaceDoubleEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//AfxMessageBox(CString(nChar));

	if (nChar == VK_TAB || nChar == VK_RETURN || nChar == VK_ESCAPE || 
		nChar == 127 || nChar == VK_BACK) //VK_DELETE is 46, but that's '.', 127 is the ascii for Delete
	{
		CInPlaceEdit::OnChar(nChar, nRepCnt, nFlags);
		return;
	}
	
	
	if ((nChar >= '0') && (nChar <= '9')) 
	{
		CInPlaceEdit::OnChar(nChar, nRepCnt, nFlags);
		return;
	}


	CString currentText;
	this->GetWindowText(currentText.GetBuffer(100),100);
	
	if (nChar == '.') //can only allow one '.'
	{
		if (currentText.FindOneOf(".") != -1)
		{
			//AfxMessageBox("has '.' ");
			return;
		}
		else
		{
			//AfxMessageBox("no '.'");
			CInPlaceEdit::OnChar(nChar, nRepCnt, nFlags);
			return;
		}
	}

	if (nChar == '-') //can only allow one '-'
	{
		if (currentText.FindOneOf("-") != -1)
		{
			//AfxMessageBox("has '-' ");
			return;
		}
		else
		{
			//AfxMessageBox("no '-'");
			this->SetSel(0,0); //'-' can only appear at the beginning of a double
			CInPlaceEdit::OnChar(nChar, nRepCnt, nFlags);
			return;
		}
	}
	
}



