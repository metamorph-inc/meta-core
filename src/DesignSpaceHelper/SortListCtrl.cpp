/*----------------------------------------------------------------------
Copyright (C)2001 MJSoft. All Rights Reserved.
          This source may be used freely as long as it is not sold for
					profit and this copyright information is not altered or removed.
					Visit the web-site at www.mjsoft.co.uk
					e-mail comments to info@mjsoft.co.uk
File:     SortListCtrl.cpp
Purpose:  Provides a sortable list control, it will sort text, numbers
          and dates, ascending or descending, and will even draw the
					arrows just like windows explorer!
----------------------------------------------------------------------*/

#include "StdAfx.h"
#include "SortListCtrl.h"
#include "resource.h"
#include "ConstraintMainDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


LPCTSTR g_pszSection = _T("ListCtrls");


struct ItemData
{
public:
	ItemData() : arrpsz( NULL ), dwData( NULL ) {}

	LPTSTR* arrpsz;
	DWORD dwData;
	int uid;

private:
	// ban copying.
	ItemData( const ItemData& );
	ItemData& operator=( const ItemData& );
};


CSortListCtrl::CSortListCtrl()
	: m_iNumColumns( 0 )
	, m_iSortColumn( -1 )
	, m_bSortAscending( TRUE )
	, m_showInfoTip( TRUE )
{
}


CSortListCtrl::~CSortListCtrl()
{
	if(!m_pwchTip)      
		  delete m_pwchTip;
}

void CSortListCtrl::SetColumnNum(int numColumns)
{
	m_iNumColumns = numColumns;
}

BEGIN_MESSAGE_MAP(CSortListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CSortListCtrl)
	ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, OnColumnClick)
//	ON_NOTIFY_EX(TTN_NEEDTEXTA, 0, OnToolNeedText)
	ON_NOTIFY_EX(TTN_NEEDTEXTW, 0, OnToolNeedText)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSortListCtrl message handlers

void CSortListCtrl::PreSubclassWindow()
{
	// the list control must have the report style.
	ASSERT( GetStyle() & LVS_REPORT );

	CListCtrl::PreSubclassWindow();

	#if (_WIN32_WINNT >= 0x501)
	SetExtendedStyle(LVS_EX_DOUBLEBUFFER | GetExtendedStyle());
#endif
	SetExtendedStyle(GetExtendedStyle() | LVS_EX_FULLROWSELECT);
	SetExtendedStyle(GetExtendedStyle() | LVS_EX_HEADERDRAGDROP);
	SetExtendedStyle(GetExtendedStyle() | LVS_EX_GRIDLINES);

	VERIFY( m_ctlHeader.SubclassWindow( GetHeaderCtrl()->GetSafeHwnd() ) );
}

BOOL CSortListCtrl::initHeadListCtr(BOOL showHeadCheckBox)
{
	m_ctlHeader.SetHeadCheckBox(showHeadCheckBox);
	if(!showHeadCheckBox) return TRUE;

	CHeaderCtrl* pHeadCtrl = GetHeaderCtrl();
	ASSERT(pHeadCtrl->GetSafeHwnd());

//	VERIFY( m_ctlHeader.SubclassWindow( GetHeaderCtrl()->GetSafeHwnd() ) );
	VERIFY( m_checkImgList.Create(IDB_CHECKBOXES, 16, 3, RGB(255,0,255)) );
	int i = m_checkImgList.GetImageCount();
	m_ctlHeader.SetImageList(&m_checkImgList);
	
	HDITEM hdItem;
	hdItem.mask = HDI_IMAGE | HDI_FORMAT;
	VERIFY( m_ctlHeader.GetItem(0, &hdItem) );
	hdItem.iImage = 1;
	hdItem.fmt |= HDF_IMAGE;
	
	VERIFY( m_ctlHeader.SetItem(0, &hdItem) );

	return TRUE;
}

BOOL CSortListCtrl::SetHeadings( UINT uiStringID )
{
	CString strHeadings;
	VERIFY( strHeadings.LoadString( uiStringID ) );
	return SetHeadings( strHeadings );
}


// the heading text is in the format column 1 text,column 1 width;column 2 text,column 3 width;etc.
BOOL CSortListCtrl::SetHeadings( const CString& strHeadings )
{
	int iStart = 0;

	for( ;; )
	{
		const int iComma = strHeadings.Find( _T(','), iStart );

		if( iComma == -1 )
			break;

		const CString strHeading = strHeadings.Mid( iStart, iComma - iStart );

		iStart = iComma + 1;

		int iSemiColon = strHeadings.Find( _T(';'), iStart );

		if( iSemiColon == -1 )
			iSemiColon = strHeadings.GetLength();

		const int iWidth = _ttoi( strHeadings.Mid( iStart, iSemiColon - iStart ) );
		
		iStart = iSemiColon + 1;

		if( InsertColumn( m_iNumColumns++, strHeading, LVCFMT_LEFT, iWidth ) == -1 )
			return FALSE;
	}

	return TRUE;
}


int CSortListCtrl::AddItem(int id,  LPCTSTR pszText, ... )
{
	const int iIndex = InsertItem( GetItemCount(), pszText );

	LPTSTR* arrpsz = new LPTSTR[ m_iNumColumns ];
	arrpsz[ 0 ] = new TCHAR[ lstrlen( pszText ) + 1 ];
	(void)lstrcpy( arrpsz[ 0 ], pszText );

 	va_list list;
	va_start( list, pszText );

	for( int iColumn = 1; iColumn < m_iNumColumns; iColumn++ )
	{
		pszText = va_arg( list, LPCTSTR );
		ASSERT_VALID_STRING( pszText );
		VERIFY( CListCtrl::SetItem( iIndex, iColumn, LVIF_TEXT, pszText, 0, 0, 0, 0 ) );

		arrpsz[ iColumn ] = new TCHAR[ lstrlen( pszText ) + 1 ];
		(void)lstrcpy( arrpsz[ iColumn ], pszText );
	}

	va_end( list );

	VERIFY( SetTextArray( iIndex, arrpsz, id ) );

	return iIndex;
}


void CSortListCtrl::FreeItemMemory( const int iItem )
{
	ItemData* pid = reinterpret_cast<ItemData*>( CListCtrl::GetItemData( iItem ) );

	LPTSTR* arrpsz = pid->arrpsz;

	for( int i = 0; i < m_iNumColumns; i++ )
		delete[] arrpsz[ i ];

	delete[] arrpsz;
	delete pid;

	VERIFY( CListCtrl::SetItemData( iItem, NULL ) );
}


BOOL CSortListCtrl::DeleteItem( int iItem )
{
	FreeItemMemory( iItem );
	return CListCtrl::DeleteItem( iItem );
}


BOOL CSortListCtrl::DeleteAllItems()
{
	for( int iItem = 0; iItem < GetItemCount(); iItem ++ )
		FreeItemMemory( iItem );

	return CListCtrl::DeleteAllItems();
}


bool IsNumber( LPCTSTR pszText )
{
	ASSERT_VALID_STRING( pszText );

	for( int i = 0; i < lstrlen( pszText ); i++ ) {
		if( !_istdigit( pszText[ i ] ) ) {
			return false;
		}
	}

	return true;
}


int NumberCompare( LPCTSTR pszNumber1, LPCTSTR pszNumber2 )
{
	ASSERT_VALID_STRING( pszNumber1 );
	ASSERT_VALID_STRING( pszNumber2 );

	const int iNumber1 = _ttoi( pszNumber1 );
	const int iNumber2 = _ttoi( pszNumber2 );

	if( iNumber1 < iNumber2 ) {
		return -1;
	}
	
	if( iNumber1 > iNumber2 ) {
		return 1;
	}

	return 0;
}

int CALLBACK CSortListCtrl::CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamData )
{
	CSortListCtrl* pListCtrl = reinterpret_cast<CSortListCtrl*>( lParamData );
	ASSERT( pListCtrl->IsKindOf( RUNTIME_CLASS( CListCtrl ) ) );

	ItemData* pid1 = reinterpret_cast<ItemData*>( lParam1 );
	ItemData* pid2 = reinterpret_cast<ItemData*>( lParam2 );

	ASSERT( pid1 );
	ASSERT( pid2 );

	LPCTSTR pszText1 = pid1->arrpsz[ pListCtrl->m_iSortColumn ];
	LPCTSTR pszText2 = pid2->arrpsz[ pListCtrl->m_iSortColumn ];

	ASSERT_VALID_STRING( pszText1 );
	ASSERT_VALID_STRING( pszText2 );

	if( IsNumber( pszText1 ) && IsNumber( pszText2 ))
		return pListCtrl->m_bSortAscending ? NumberCompare( pszText1, pszText2 ) : NumberCompare( pszText2, pszText1 );
	else
		// text.
		return pListCtrl->m_bSortAscending ? lstrcmp( pszText1, pszText2 ) : lstrcmp( pszText2, pszText1 );
}


void CSortListCtrl::OnColumnClick( NMHDR* pNMHDR, LRESULT* pResult )
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	const int iColumn = pNMListView->iSubItem;

	// if it's a second click on the same column then reverse the sort order,
	// otherwise sort the new column in ascending order.
	if(iColumn!=0)
		Sort( iColumn, iColumn == m_iSortColumn ? !m_bSortAscending : TRUE );

	*pResult = 0;
}


void CSortListCtrl::Sort( int iColumn, BOOL bAscending )
{
	m_iSortColumn = iColumn;
	m_bSortAscending = bAscending;

	// show the appropriate arrow in the header control.
//	m_ctlHeader.SetSortArrow( m_iSortColumn, m_bSortAscending );

	VERIFY( SortItems( CompareFunction, reinterpret_cast<DWORD>( this ) ) );
}

void CSortListCtrl::Sort()
{
	if(m_iSortColumn<=0)
		return;

//	m_ctlHeader.SetSortArrow( m_iSortColumn, m_bSortAscending );
	VERIFY( SortItems( CompareFunction, reinterpret_cast<DWORD>( this ) ) );
}

void CSortListCtrl::LoadColumnInfo()
{
	// you must call this after setting the column headings.
	ASSERT( m_iNumColumns > 0 );

	CString strKey;
	strKey.Format( _T("%d"), GetDlgCtrlID() );

	UINT nBytes = 0;
	BYTE* buf = NULL;
	if( AfxGetApp()->GetProfileBinary( g_pszSection, strKey, &buf, &nBytes ) )
	{
		if( nBytes > 0 )
		{
			CMemFile memFile( buf, nBytes );
			CArchive ar( &memFile, CArchive::load );
			m_ctlHeader.Serialize( ar );
			ar.Close();

			m_ctlHeader.Invalidate();
		}

		delete[] buf;
	}
}


void CSortListCtrl::SaveColumnInfo()
{
	ASSERT( m_iNumColumns > 0 );

	CString strKey;
	strKey.Format( _T("%d"), GetDlgCtrlID() );

	CMemFile memFile;

	CArchive ar( &memFile, CArchive::store );
	m_ctlHeader.Serialize( ar );
	ar.Close();

	DWORD dwLen = memFile.GetLength();
	BYTE* buf = memFile.Detach();	

	VERIFY( AfxGetApp()->WriteProfileBinary( g_pszSection, strKey, buf, dwLen ) );

	free( buf );
}


void CSortListCtrl::OnDestroy() 
{
	for( int iItem = 0; iItem < GetItemCount(); iItem ++ )
		FreeItemMemory( iItem );

	CListCtrl::OnDestroy();
}


BOOL CSortListCtrl::SetItemText( int nItem, int nSubItem, LPCTSTR lpszText )
{
	if( !CListCtrl::SetItemText( nItem, nSubItem, lpszText ) )
		return FALSE;

	LPTSTR* arrpsz = GetTextArray( nItem );
	LPTSTR pszText = arrpsz[ nSubItem ];
	delete[] pszText;
	pszText = new TCHAR[ lstrlen( lpszText ) + 1 ];
	(void)lstrcpy( pszText, lpszText );
	arrpsz[ nSubItem ] = pszText;

	return TRUE;
}


BOOL CSortListCtrl::SetItemData( int nItem, DWORD dwData )
{
	if( nItem >= GetItemCount() )
		return FALSE;

	ItemData* pid = reinterpret_cast<ItemData*>( CListCtrl::GetItemData( nItem ) );
	ASSERT( pid );
	pid->dwData = dwData;

	return TRUE;
}


DWORD CSortListCtrl::GetItemData( int nItem ) const
{
	ASSERT( nItem < GetItemCount() );

	ItemData* pid = reinterpret_cast<ItemData*>( CListCtrl::GetItemData( nItem ) );
	ASSERT( pid );
	return pid->dwData;
}


BOOL CSortListCtrl::SetTextArray( int iItem, LPTSTR* arrpsz, int id )
{
	ASSERT( CListCtrl::GetItemData( iItem ) == NULL );
	ItemData* pid = new ItemData;
	pid->arrpsz = arrpsz;
	pid->uid = id;
	return CListCtrl::SetItemData( iItem, reinterpret_cast<DWORD>( pid ) );
}


LPTSTR* CSortListCtrl::GetTextArray( int iItem ) const
{
	ASSERT( iItem < GetItemCount() );

	ItemData* pid = reinterpret_cast<ItemData*>( CListCtrl::GetItemData( iItem ) );
	return pid->arrpsz;
}

int CSortListCtrl::GetItemID(int nItem)
{
	ItemData* pid = reinterpret_cast<ItemData*>( CListCtrl::GetItemData(nItem) );
	return pid->uid;
}

void CSortListCtrl::CellHitTest(const CPoint& pt, int& nRow, int& nCol) const
{
	nRow = -1;
	nCol = -1;

	LVHITTESTINFO lvhti = {0};
	lvhti.pt = pt;
	nRow = ListView_SubItemHitTest(m_hWnd, &lvhti);	// SubItemHitTest is non-const
	nCol = lvhti.iSubItem;
	if (!(lvhti.flags & LVHT_ONITEMLABEL))
		nRow = -1;
}

BOOL CSortListCtrl::OnToolNeedText(UINT id, NMHDR* pNMHDR, LRESULT* pResult)
{
	CPoint pt(GetMessagePos());
	ScreenToClient(&pt);

	int nRow, nCol;
	CellHitTest(pt, nRow, nCol);

	CString tooltip = GetToolTipText(nRow, nCol);
	if (tooltip.IsEmpty())
		return FALSE;

	// Non-unicode applications can receive requests for tooltip-text in unicode
	
	TOOLTIPTEXTW* pTTTW = (TOOLTIPTEXTW*)pNMHDR;
	
	if(!m_pwchTip)      
		  delete m_pwchTip;

	m_pwchTip = tooltip;

	//get the maxwidth for the tooltip
	int maxwidth = 0;
	CString tmp = tooltip;
	int newline = tmp.Find('\n');
	while(newline > 0 && tmp.GetLength()>1)
	{
		if(newline>maxwidth)
			maxwidth = newline;
		tmp = tmp.Right(tmp.GetLength()-newline);
		newline = tmp.Find('\n');
	}
	if(tmp.GetLength()>maxwidth)
		maxwidth =  tmp.GetLength();
	
	::SendMessage(pNMHDR->hwndFrom, TTM_SETMAXTIPWIDTH, 0, maxwidth*6);
	pTTTW->lpszText = const_cast<LPTSTR>((const WCHAR*)m_pwchTip);
	
	// If wanting to display a tooltip which is longer than 80 characters,
	// then one must allocate the needed text-buffer instead of using szText,
	// and point the TOOLTIPTEXT::lpszText to this text-buffer.
	// When doing this, then one is required to release this text-buffer again
	return TRUE;
}

bool CSortListCtrl::ShowToolTip(const CPoint& pt) const
{
	// Lookup up the cell
	int nRow, nCol;
	CellHitTest(pt, nRow, nCol);

	if (nRow!=-1 && nCol!=-1)
		return true;
	else
		return false;
}

CString CSortListCtrl::GetToolTipText(int nRow, int nCol)
{
	if (!m_showInfoTip)
		return CString("");

	if (nRow!=-1 && nCol==1)
	{
		int i = GetItemID(nRow);
		map<int, std::string>::iterator pos = CConstraintMainDialog::constraintExprMap.find(i);
		if(pos!=CConstraintMainDialog::constraintExprMap.end())
			return (LPSTR)(CConstraintMainDialog::constraintExprMap[i].c_str());
		else
			return CString("");
	}
	else
		return CString("");
}
