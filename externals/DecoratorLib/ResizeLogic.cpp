//################################################################################################
//
// Resize logic common codes class
//	ResizeLogic.cpp
//
//################################################################################################

#include "StdAfx.h"
#include "ResizeLogic.h"
#include "DecoratorExceptions.h"
#include "ResizablePart.h"
#include "SizeTrackerDialog.h"
#include "DialogTemplate.h"


namespace DecoratorSDK {

//################################################################################################
//
// CLASS : ResizeLogic
//
//################################################################################################

ResizeLogic::ResizeLogic(PartBase* pPart, HWND parentWnd):
	m_parentPart	(pPart),
	m_parentWnd		(parentWnd),
	sizeTrackerDlg	(NULL)
{
	m_minSize.SetSize(0, 0);
}

ResizeLogic::~ResizeLogic()
{
}

short ResizeLogic::GetResizeFeatures(void) const
{
	return m_resizeFeatures;
}

void ResizeLogic::SetResizeFeatures(short resizeFeatures)
{
	m_resizeFeatures = resizeFeatures;
}

CRect ResizeLogic::GetResizeTargetLocation(void) const
{
	return m_targetLocation;
}

void ResizeLogic::SetResizeTargetLocation(CRect targetLocation)
{
	m_targetLocation = targetLocation;
}

void ResizeLogic::SetParentPart(PartBase* pPart)
{
	m_parentPart = pPart;
}

void ResizeLogic::Initialize(CComPtr<IMgaProject>& pProject, CComPtr<IMgaMetaPart>& pPart, CComPtr<IMgaFCO>& pFCO)
{
	m_resizeState				= NotInResize;
	m_resizeFeatures			= Resizeable;
	m_targetLocation.SetRectEmpty();
	m_bCursorSaved				= false;
}

void ResizeLogic::Destroy(void)
{
	OperationCanceledByGME();
}

void ResizeLogic::Draw(CDC* pDC, Gdiplus::Graphics* gdip)
{
	if ((m_resizeFeatures & DrawResizeCorner) != 0) {
		// draw resize corner: currently done by GME
	} else if ((m_resizeFeatures & DrawSelectionRectangle) != 0) {
		// draw selection rectangle: currently done by GME
	}
}

// New functions
void ResizeLogic::InitializeEx(CComPtr<IMgaProject>& pProject, CComPtr<IMgaMetaPart>& pPart, CComPtr<IMgaFCO>& pFCO,
							   HWND parentWnd, PreferenceMap& preferences)
{
	m_parentWnd = parentWnd;
	Initialize(pProject, pPart, pFCO);
}

bool ResizeLogic::MouseMoved(UINT nFlags, const CPoint& point, HDC transformHDC)
{
	if (m_parentPart->IsActive()) {
		m_targetLocation = m_parentPart->GetLocation();
		ResizeType resizeTypeCandidate = DeterminePotentialResize(point);
		HRESULT retVal = S_OK;
		if (m_resizeState == NotInResize) {
			if (resizeTypeCandidate != NotInResize) {
				ChangeCursorForm(resizeTypeCandidate);
				m_bCursorSaved = true;
				return true;
			} else {
				RestoreCursor();
			}
		} else {
			long deltax = point.x - m_originalMousePosition.x;
			long deltay = point.y - m_originalMousePosition.y;
			// Change size/m_targetLocation
			CRect newTargetLocation = m_originalLocation;
			long widthMinLimit = max(5, m_minSize.cx);
			long heightMinLimit = max(5, m_minSize.cy);
			// Horizontal limiting
			switch(m_resizeState) {
				case RightEdgeResize:
				case TopRightCornerResize:
				case BottomRightCornerResize:
					{
						if (newTargetLocation.Width() + deltax < widthMinLimit)
							deltax = widthMinLimit - newTargetLocation.Width();
					}
					break;
				case LeftEdgeResize:
				case TopLeftCornerResize:
				case BottomLeftCornerResize:
					{
						if (newTargetLocation.Width() - deltax < widthMinLimit)
							deltax = newTargetLocation.Width() - widthMinLimit;
					}
					break;
			}
			// Vertical limiting
			switch(m_resizeState) {
				case BottomEdgeResize:
				case BottomRightCornerResize:
				case BottomLeftCornerResize:
					{
						if (newTargetLocation.Height() + deltay < heightMinLimit)
							deltay = heightMinLimit - newTargetLocation.Height();
					}
					break;
				case TopEdgeResize:
				case TopLeftCornerResize:
				case TopRightCornerResize:
					{
						if (newTargetLocation.Height() - deltay < heightMinLimit)
							deltay = newTargetLocation.Height() - heightMinLimit;
					}
					break;
			}
			// Nop filtering
			switch(m_resizeState) {
				case RightEdgeResize:
				case LeftEdgeResize:
					{
						if (deltax == 0)
							return true;
					}
					break;
				case BottomEdgeResize:
				case TopEdgeResize:
					{
						if (deltay == 0)
							return true;
					}
					break;
				case TopLeftCornerResize:
				case TopRightCornerResize:
				case BottomRightCornerResize:
				case BottomLeftCornerResize:
					{
						if (deltax == 0 && deltay == 0)
							return true;
					}
					break;
			}
			// Adjust rect
			switch(m_resizeState) {
				case RightEdgeResize:			newTargetLocation.InflateRect(0,		0,			deltax,	0		);	break;
				case BottomEdgeResize:			newTargetLocation.InflateRect(0,		0,			0,		deltay	);	break;
				case LeftEdgeResize:			newTargetLocation.InflateRect(-deltax,	0,			0,		0		);	break;
				case TopEdgeResize:				newTargetLocation.InflateRect(0,		-deltay,	0,		0		);	break;
				case TopLeftCornerResize:		newTargetLocation.InflateRect(-deltax,	-deltay,	0,		0		);	break;
				case TopRightCornerResize:		newTargetLocation.InflateRect(0,		-deltay,	deltax,	0		);	break;
				case BottomRightCornerResize:	newTargetLocation.InflateRect(0,		0,			deltax,	deltay	);	break;
				case BottomLeftCornerResize:	newTargetLocation.InflateRect(-deltax,	0,			0,		deltay	);	break;
				case MoveOperation:				newTargetLocation.OffsetRect(deltax, deltay);							break;
			}
			// keep aspect ratio if it's a corner resize
			if ((nFlags & MK_CONTROL) != 0) {
				double aspect_ratio = (double)m_originalLocation.Width() / (double)m_originalLocation.Height();
				long other_y = (long)(newTargetLocation.Width() / aspect_ratio);
				long other_x = (long)(newTargetLocation.Height() * aspect_ratio);
				if (other_y > newTargetLocation.Height()) {
					long dy = other_y - newTargetLocation.Height();
					if (m_resizeState == TopRightCornerResize ||
						m_resizeState == TopLeftCornerResize ||
						m_resizeState == TopEdgeResize)
					{
						newTargetLocation.top -= dy;
					} else {
						newTargetLocation.bottom += dy;
					}
				} else if (other_x > newTargetLocation.Width()) {
					long dx = other_x - newTargetLocation.Width();
					if (m_resizeState == TopLeftCornerResize ||
						m_resizeState == BottomLeftCornerResize ||
						m_resizeState == LeftEdgeResize)
					{
						newTargetLocation.left -= dx;
					} else {
						newTargetLocation.right += dx;
					}
				} else {
					// Cannot be satisfied completely because of the rounding
					//ASSERT((double)newTargetLocation.Width() / (double)newTargetLocation.Height() ==
					//	   (double)m_originalLocation.Width() / (double)m_originalLocation.Height());
				}
			}
			m_targetLocation = newTargetLocation;
			if (m_resizeState == MoveOperation) {
				m_parentPart->WindowMoving(nFlags, m_targetLocation);
			} else {
				m_parentPart->WindowResizing(nFlags, m_targetLocation);
				if (sizeTrackerDlg != NULL)
					sizeTrackerDlg->AdjustPositionAndText(point, m_targetLocation);
			}
			return true;
		}
	} else {
		RestoreCursor();
	}

	return false;
}

bool ResizeLogic::MouseLeftButtonDown(UINT nFlags, const CPoint& point, HDC transformHDC)
{
	if (m_parentPart->IsActive() && m_resizeState == NotInResize) {
		ResizeType resizeTypeCandidate = DeterminePotentialResize(point);
		if (resizeTypeCandidate != NotInResize) {
			m_originalMousePosition = point;
			m_originalLocation = m_targetLocation;
			m_resizeState = resizeTypeCandidate;
			if (resizeTypeCandidate == MoveOperation) {
				m_parentPart->WindowMovingStarted(nFlags, m_targetLocation);
			} else {
				m_parentPart->WindowResizingStarted(nFlags, m_targetLocation);

				bool isPermanentCWnd = true;
				CWnd* cWnd = CWnd::FromHandlePermanent(m_parentWnd);
				if (cWnd == NULL) {
					isPermanentCWnd = false;
					cWnd = CWnd::FromHandle(m_parentWnd);
				}

				CDialogTemplate dlgTemplate(_T(""),
											WS_CHILD | WS_VISIBLE | WS_BORDER,	// Window Styles
											0,	// Extended Window Styles
											0, 0, SizeTrackerWidth, SizeTrackerHeight);
				dlgTemplate.AddStatic(_T("EDT"), WS_VISIBLE, 0, 0, 0, SizeTrackerWidth - 1, SizeTrackerHeight, IDC_INPLACETEXTEDIT);

				sizeTrackerDlg = new CSizeTrackerDialog;
				sizeTrackerDlg->SetParameters(m_targetLocation, point, resizeTypeCandidate, cWnd, isPermanentCWnd, transformHDC);
				BOOL success = sizeTrackerDlg->CreateIndirect(dlgTemplate, cWnd);
				if (success != FALSE)
					success = sizeTrackerDlg->ShowWindow(SW_SHOWNORMAL);
			}
			return true;
		}
	}

	return false;
}

bool ResizeLogic::MouseLeftButtonUp(UINT nFlags, const CPoint& point, HDC transformHDC)
{
	if (m_parentPart->IsActive() && m_resizeState != NotInResize) {
		if (m_resizeState == MoveOperation) {
			long deltax = m_targetLocation.left - m_originalLocation.left;
			long deltay = m_targetLocation.top - m_originalLocation.top;
			m_parentPart->WindowMoved(nFlags, CPoint(deltax, deltay));
			m_parentPart->WindowMovingFinished(nFlags, m_targetLocation);
		} else {
			long deltax = m_targetLocation.Width() - m_originalLocation.Width();
			long deltay = m_targetLocation.Height() - m_originalLocation.Height();
			m_parentPart->WindowResized(nFlags, CSize(deltax, deltay));
			m_parentPart->WindowResizingFinished(nFlags, m_targetLocation);
			if (sizeTrackerDlg != NULL)
				delete sizeTrackerDlg;
		}
		m_resizeState = NotInResize;
		RestoreCursor();
		return true;
	}

	return false;
}

bool ResizeLogic::OperationCanceledByGME(void)
{
	if (m_resizeState != NotInResize) {
		m_resizeState = NotInResize;
		RestoreCursor();
		m_targetLocation = m_originalLocation;
		if (sizeTrackerDlg != NULL)
			delete sizeTrackerDlg;
	}
	return true;
}

ResizeType ResizeLogic::DeterminePotentialResize(CPoint cursorPoint) const
{
	// Topleft corner
	long actualSensitivity = 0;
	long targetWidth = m_targetLocation.Width();
	long targetHeight = m_targetLocation.Height();
	// FIXME: should use GetSystemMetrics(SM_CXSIZEFRAME) ?
	if (targetWidth <= 35 || targetHeight <= 25) {
		actualSensitivity = DECORATOR_MINSENSITIVITYDISTANCE;
	} else if (targetWidth >= 80 && targetHeight >= 40) {
		actualSensitivity = DECORATOR_MAXSENSITIVITYDISTANCE;
	} else {
		long actualSensitivityByWidth = ((targetWidth - 35) * 6 + (80 - targetWidth) * 2) / targetWidth;
		long actualSensitivityByHeight = ((targetHeight - 25) * 6 + (40 - targetHeight) * 2) / targetHeight;
		actualSensitivity = min(actualSensitivityByWidth, actualSensitivityByHeight);
		actualSensitivity = min(max(actualSensitivity, DECORATOR_MINSENSITIVITYDISTANCE), DECORATOR_MAXSENSITIVITYDISTANCE);
	}
	CRect cornerRect(m_targetLocation.left, m_targetLocation.top, m_targetLocation.left + 1, m_targetLocation.top + 1);
	cornerRect.InflateRect(actualSensitivity, actualSensitivity);
	if (cornerRect.PtInRect(cursorPoint))
		return TopLeftCornerResize;
	// Topright corner
	cornerRect.SetRect(m_targetLocation.right - 1, m_targetLocation.top, m_targetLocation.right, m_targetLocation.top + 1);
	cornerRect.InflateRect(actualSensitivity, actualSensitivity);
	if (cornerRect.PtInRect(cursorPoint))
		return TopRightCornerResize;
	// Bottomright corner
	cornerRect.SetRect(m_targetLocation.right - 1, m_targetLocation.bottom - 1, m_targetLocation.right, m_targetLocation.bottom);
	cornerRect.InflateRect(actualSensitivity, actualSensitivity);
	if (cornerRect.PtInRect(cursorPoint))
		return BottomRightCornerResize;
	// Bottomleft corner
	cornerRect.SetRect(m_targetLocation.left, m_targetLocation.bottom - 1, m_targetLocation.left + 1, m_targetLocation.bottom);
	cornerRect.InflateRect(actualSensitivity, actualSensitivity);
	if (cornerRect.PtInRect(cursorPoint))
		return BottomLeftCornerResize;

	// Left edge
	cornerRect.SetRect(m_targetLocation.left, m_targetLocation.top, m_targetLocation.left + 1, m_targetLocation.bottom);
	cornerRect.InflateRect(actualSensitivity, actualSensitivity);
	if (cornerRect.PtInRect(cursorPoint))
		return LeftEdgeResize;
	// Top edge
	cornerRect.SetRect(m_targetLocation.left, m_targetLocation.top, m_targetLocation.right, m_targetLocation.top + 1);
	cornerRect.InflateRect(actualSensitivity, actualSensitivity);
	if (cornerRect.PtInRect(cursorPoint))
		return TopEdgeResize;
	// Right edge
	cornerRect.SetRect(m_targetLocation.right - 1, m_targetLocation.top, m_targetLocation.right, m_targetLocation.bottom);
	cornerRect.InflateRect(actualSensitivity, actualSensitivity);
	if (cornerRect.PtInRect(cursorPoint))
		return RightEdgeResize;
	// Bottom edge
	cornerRect.SetRect(m_targetLocation.left, m_targetLocation.bottom - 1, m_targetLocation.right, m_targetLocation.bottom);
	cornerRect.InflateRect(actualSensitivity, actualSensitivity);
	if (cornerRect.PtInRect(cursorPoint))
		return BottomEdgeResize;

	// Inside rect: move operation possible
	if ((m_resizeFeatures & Movable) != 0) {
		cornerRect = m_targetLocation;
		cornerRect.InflateRect(-actualSensitivity, -actualSensitivity);
		if (cornerRect.PtInRect(cursorPoint))
			return MoveOperation;
	}

	return NotInResize;
}

bool ResizeLogic::IsSizeChanged(void) const
{
	return m_targetLocation.EqualRect(m_originalLocation) == FALSE;
}

CRect ResizeLogic::GetOriginalLocation(void) const
{
	return m_originalLocation;
}

void ResizeLogic::SetMinimumSize(CSize minSize)
{
	m_minSize = minSize;
}

void ResizeLogic::ChangeCursorForm(ResizeType resizeType, bool notify)
{
	LPTSTR cursorRes = NULL;
	switch(resizeType) {
		case NotInResize:				cursorRes = IDC_ARROW;		break;
		case RightEdgeResize:			cursorRes = IDC_SIZEWE;		break;
		case BottomEdgeResize:			cursorRes = IDC_SIZENS;		break;
		case LeftEdgeResize:			cursorRes = IDC_SIZEWE;		break;
		case TopEdgeResize:				cursorRes = IDC_SIZENS;		break;
		case TopLeftCornerResize:		cursorRes = IDC_SIZENWSE;	break;
		case TopRightCornerResize:		cursorRes = IDC_SIZENESW;	break;
		case BottomRightCornerResize:	cursorRes = IDC_SIZENWSE;	break;
		case BottomLeftCornerResize:	cursorRes = IDC_SIZENESW;	break;
		case MoveOperation:				cursorRes = IDC_SIZEALL;	break;
	}
	HCURSOR wantedCursor = LoadCursor(NULL, cursorRes);
	m_originalCursor = SetCursor(wantedCursor);
	if (notify)
		m_parentPart->CursorChanged((long)cursorRes);
}

void ResizeLogic::RestoreCursor(void)
{
	if (m_bCursorSaved) {
		SetCursor(m_originalCursor);
		m_bCursorSaved = false;
		m_parentPart->CursorRestored();
	}
}

}; // namespace DecoratorSDK
