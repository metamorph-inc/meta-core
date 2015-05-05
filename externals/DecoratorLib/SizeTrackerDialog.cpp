// SizeTrackerDialog.cpp : implementation file
//

#include "stdafx.h"
#include "SizeTrackerDialog.h"

namespace DecoratorSDK {

// CSizeTrackerDialog dialog

IMPLEMENT_DYNAMIC(CSizeTrackerDialog, CDialog)
CSizeTrackerDialog::CSizeTrackerDialog() :
	CDialog(),
	m_staticWnd(NULL),
	m_realParentCWnd(NULL)
{
}

CSizeTrackerDialog::~CSizeTrackerDialog()
{
}

void CSizeTrackerDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSizeTrackerDialog)
	//}}AFX_DATA_MAP
}

void CSizeTrackerDialog::PostNcDestroy(void)
{
	CDialog::PostNcDestroy();

	if (!m_bPermanentCWnd)
		m_intendedParentCWnd->Detach();
}

BEGIN_MESSAGE_MAP(CSizeTrackerDialog, CDialog)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CSizeTrackerDialog message handlers

BOOL CSizeTrackerDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_realParentCWnd = GetParent();

	m_intendedParentCWnd->GetClientRect(&m_boundsLimit);

	///////////////////////////////////////////////////////////////////////////////////////////////////

	m_staticWnd = (CStatic*)GetDlgItem(IDC_INPLACETEXTEDIT);

	TRACE1("CONSTRUCT: %X\n", m_staticWnd);

	AdjustPositionAndText(m_mouseClick, m_initialRect);
	m_staticWnd->MoveWindow(0, 0, SizeTrackerWidth - 1, SizeTrackerHeight);

	ShowWindow(SW_SHOWNORMAL);	// For safety
	SetFocus();					// For safety

	return FALSE;	// return TRUE unless you set the focus to a control
					// EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CSizeTrackerDialog::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;	// We pretend that we processed the message
					// We don't paint anything, because RichEdit20 covers the whole dialog surface anyway
					// So we block grey-white flicker that way during resize
}

CString	CSizeTrackerDialog::GetStrToDisplay(const CRect& location)
{
	CString sizeTxt;
	sizeTxt.Format(_T("size:(%ld,%ld)"), location.Width(), location.Height());

	return sizeTxt;
}

CPoint	CSizeTrackerDialog::GetAdjustedPosition(const CPoint& pt)
{
	long dx = 0;
	long dy = 0;
	switch(m_resizeType) {
		case RightEdgeResize:			dx = SizeTrackerMargin, dy = 0;																	break;
		case BottomEdgeResize:			dx = 0, dy = SizeTrackerMargin;																	break;
		case LeftEdgeResize:			dx = -(SizeTrackerWidth + SizeTrackerMargin), dy = 0;											break;
		case TopEdgeResize:				dx = 0, dy = -(SizeTrackerHeight + SizeTrackerMargin);											break;
		case TopLeftCornerResize:		dx = -(SizeTrackerWidth + SizeTrackerMargin), dy = -(SizeTrackerHeight + SizeTrackerMargin);	break;
		case TopRightCornerResize:		dx = SizeTrackerMargin, dy = -(SizeTrackerHeight + SizeTrackerMargin);							break;
		case BottomRightCornerResize:	dx = SizeTrackerMargin, dy = SizeTrackerMargin;													break;
		case BottomLeftCornerResize:	dx = -(SizeTrackerWidth + SizeTrackerMargin), dy = SizeTrackerMargin;							break;
	}

	CPoint offsetted = pt;
	offsetted.Offset(dx, dy);
	// TODO: calculate with boundsLimit also
	return offsetted;
}

void CSizeTrackerDialog::SetParameters(const CRect& initialRect, const CPoint& mouseClick, ResizeType resizeType,
									   CWnd* parentCWnd, bool isPermanentCWnd, HDC transformHDC)
{
	m_initialRect				= initialRect;
	m_mouseClick				= mouseClick;
	m_intendedParentCWnd		= parentCWnd;
	m_bPermanentCWnd			= isPermanentCWnd;
	m_transformHDC				= transformHDC;
	m_resizeType				= resizeType;
}

void CSizeTrackerDialog::AdjustPositionAndText(const CPoint& point, const CRect& location)
{	
	if(!::IsWindow(m_staticWnd->m_hWnd)) // Quick fix by Tihamer.
	{
		m_staticWnd = (CStatic*)GetDlgItem(IDC_INPLACETEXTEDIT);
	}

	m_staticWnd->SetWindowText(GetStrToDisplay(location));
	CPoint adjPos = GetAdjustedPosition(point);
	MoveWindow(adjPos.x, adjPos.y, SizeTrackerWidth, SizeTrackerHeight);
}

}
