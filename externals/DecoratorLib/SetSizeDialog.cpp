// SetSizeDialog.cpp : implementation file
//

#include "stdafx.h"
#include "SetSizeDialog.h"
#include "ResizablePart.h"

namespace DecoratorSDK {

// CSetSizeDialog dialog

IMPLEMENT_DYNAMIC(CSetSizeDialog, CDialog)
CSetSizeDialog::CSetSizeDialog() :
	CDialog(),
	m_parentPart(NULL),
	m_widthEditWnd(NULL),
	m_heightEditWnd(NULL),
	m_bDlgResult(true),
	m_realParentCWnd(NULL),
	m_lWidth(0),
	m_lHeight(0)
{
	m_bInited = true;
	m_bClosed = false;
}

CSetSizeDialog::~CSetSizeDialog()
{
}

void CSetSizeDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetSizeDialog)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSetSizeDialog, CDialog)
	ON_WM_NCACTIVATE()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CSetSizeDialog message handlers

BOOL CSetSizeDialog::OnInitDialog()
{
	m_bInited = true;
	m_bClosed = false;
	m_bDlgResult = true;
	m_lWidth = 0;
	m_lHeight = 0;

	CDialog::OnInitDialog();

	m_realParentCWnd = GetParent();

	m_intendedParentCWnd->GetClientRect(&m_boundsLimit);
	m_intendedParentCWnd->ClientToScreen(&m_boundsLimit);

	///////////////////////////////////////////////////////////////////////////////////////////////////

	m_widthEditWnd = (CEdit*)GetDlgItem(WidthEditCtrlId);
	m_heightEditWnd = (CEdit*)GetDlgItem(HeightEditCtrlId);

	AdjustPositionAndValues(m_mouseClick, m_initialRect);

	m_parentPart->WindowResizingStarted(m_nFlags, m_initialRect);

	ShowWindow(SW_SHOWNORMAL);	// For safety
	SetFocus();					// For safety

	m_bInited = true;

	return FALSE;	// return TRUE unless you set the focus to a control
					// EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CSetSizeDialog::OnNcActivate(BOOL bActive)
{
	if (m_bInited)
	{
		m_bInited = false;
	}
	else
	{
		if (!bActive && !m_bClosed) {
			m_bClosed = true;
			EndDialog(IDOK);
		}
	}
	return FALSE;	// CDialog::OnNcActivate(bActive);
}
/*
BOOL CSetSizeDialog::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;	// We pretend that we processed the message
					// We don't paint anything, because RichEdit20 covers the whole dialog surface anyway
					// So we block grey-white flicker that way during resize
}
*/
void CSetSizeDialog::OnBnClickedCancel()
{
	m_bDlgResult = false;
	if (GetSizeAndIsValid())
		CDialog::OnCancel();
}

void CSetSizeDialog::OnBnClickedOk()
{
	if (GetSizeAndIsValid())
		CDialog::OnOK();
}

CPoint	CSetSizeDialog::GetAdjustedPosition(const CPoint& pt)
{
	long dx = 0;
	long dy = 0;
	switch(m_resizeType) {
		case RightEdgeResize:			dx = SetSizeDialogMargin, dy = 0;																		break;
		case BottomEdgeResize:			dx = 0, dy = SetSizeDialogMargin;																		break;
		case LeftEdgeResize:			dx = -(SetSizeDialogWidth + SetSizeDialogMargin), dy = 0;												break;
		case TopEdgeResize:				dx = 0, dy = -(SetSizeDialogHeight + SetSizeDialogMargin);												break;
		case TopLeftCornerResize:		dx = -(SetSizeDialogWidth + SetSizeDialogMargin), dy = -(SetSizeDialogHeight + SetSizeDialogMargin);	break;
		case TopRightCornerResize:		dx = SetSizeDialogMargin, dy = -(SetSizeDialogHeight + SetSizeDialogMargin);							break;
		case BottomRightCornerResize:	dx = SetSizeDialogMargin, dy = SetSizeDialogMargin;														break;
		case BottomLeftCornerResize:	dx = -(SetSizeDialogWidth + SetSizeDialogMargin), dy = SetSizeDialogMargin;								break;
	}

	CPoint offsetted = pt;
	m_intendedParentCWnd->ClientToScreen(&offsetted);
	offsetted.Offset(dx, dy);
	// TODO: calculate with boundsLimit also
	return offsetted;
}

bool CSetSizeDialog::GetSizeAndIsValid(void)
{
	CString sizeStr;
	m_widthEditWnd->GetWindowText(sizeStr);
	m_lWidth = _ttoi(sizeStr);
	m_heightEditWnd->GetWindowText(sizeStr);
	m_lHeight = _ttoi(sizeStr);
	if (m_lWidth <= 0 || m_lHeight <= 0) {
		AfxMessageBox(_T("Please provide valid non-negative integer values!"), MB_OK | MB_ICONSTOP);
		return false;
	}
	return true;
}

void CSetSizeDialog::SetParameters(ResizablePart* parentPart, const CRect& initialRect, const CPoint& mouseClick,
								   ResizeType resizeType, CWnd* parentCWnd, bool isPermanentCWnd, HDC transformHDC, UINT nFlags)
{
	m_parentPart				= parentPart;
	m_initialRect				= initialRect;
	m_mouseClick				= mouseClick;
	m_intendedParentCWnd		= parentCWnd;
	m_bPermanentCWnd			= isPermanentCWnd;
	m_transformHDC				= transformHDC;
	m_resizeType				= resizeType;
	m_nFlags					= nFlags;
}

void CSetSizeDialog::AdjustPositionAndValues(const CPoint& point, const CRect& location)
{
	CString sizeStr;
	sizeStr.Format(_T("%ld"), location.Width());
	m_widthEditWnd->SetWindowText(sizeStr);
	sizeStr.Format(_T("%ld"), location.Height());
	m_heightEditWnd->SetWindowText(sizeStr);
	CPoint adjPos = GetAdjustedPosition(point);
	MoveWindow(adjPos.x, adjPos.y, SetSizeDialogWidth, SetSizeDialogHeight);
}

bool CSetSizeDialog::GetDlgResult(void) const
{
	return m_bDlgResult;
}

void CSetSizeDialog::EndDialog(int nResult)
{
	m_bClosed = true;

	CRect newRect = m_initialRect;
	if (m_bDlgResult) {
		newRect.SetRect(m_initialRect.left, m_initialRect.top, m_initialRect.left + m_lWidth, m_initialRect.top + m_lHeight);
		m_parentPart->WindowResized(m_nFlags, CSize(m_lWidth, m_lHeight));
	}
	m_parentPart->WindowResizingFinished(m_nFlags, newRect);

	CDialog::EndDialog(nResult);
}

}
