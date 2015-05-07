//################################################################################################
//
// Text part class (decorator part)
//	TextPart.cpp
//
//################################################################################################

#include "StdAfx.h"
#include "TextPart.h"
#include "PortLabelPart.h"
#include "DecoratorExceptions.h"
#include "InPlaceEditDialog.h"
#include "DialogTemplate.h"

namespace DecoratorSDK {

//################################################################################################
//
// CLASS : TextPart
//
//################################################################################################

TextPart::TextPart(PartBase* pPart, CComPtr<IMgaCommonDecoratorEvents>& eventSink):
	PartBase			(pPart, eventSink),
	resizeLogic			(NULL, NULL),
	m_bCursorSaved		(false),
	m_eTextLocation		(L_SOUTH),
	m_bTextEnabled		(true),	// ?
	m_bTextEditable		(true),
	m_iTextWrapCount	(0),
	m_crText			(COLOR_BLACK),
	m_iFontKey			(FONT_LABEL),
	m_iMaxTextLength	(MAX_LABEL_LENGTH),
	m_bMultiLine		(false)
{
	resizeLogic.SetParentPart(this);
}

TextPart::~TextPart()
{
}

void TextPart::Initialize(CComPtr<IMgaProject>& pProject, CComPtr<IMgaMetaPart>& pPart, CComPtr<IMgaFCO>& pFCO)
{
	if (m_spFCO)
		resizeLogic.SetResizeFeatures(ResizeLogic::Resizeable | /* TODO: temp ResizeLogic::Movable |*/ ResizeLogic::DrawSelectionRectangle);

	PartBase::Initialize(pProject, pPart, pFCO);
}

void TextPart::Destroy(void)
{
	OperationCanceledByGME();
	if (m_spFCO)
		resizeLogic.Destroy();
}

feature_code TextPart::GetFeatures(void) const
{
	return m_bTextEditable ? F_MOUSEEVENTS : 0 | F_HASLABEL;
}

CSize TextPart::GetPreferredSize(void) const
{
	return CSize(0, 0);
}

CRect TextPart::GetLabelLocation(void) const
{
	return GetTextLocation();
}

// New functions
void TextPart::InitializeEx(CComPtr<IMgaProject>& pProject, CComPtr<IMgaMetaPart>& pPart, CComPtr<IMgaFCO>& pFCO,
							HWND parentWnd, PreferenceMap& preferences)
{
	VARIANT_BOOL libObject = VARIANT_FALSE;
	if (pFCO) {
		pFCO->get_IsLibObject(&libObject);
	}
	if (libObject != VARIANT_FALSE) {
		m_bTextEditable = false;
	}
	// Check if editability is disabled/enabled
	PreferenceMap::iterator it = preferences.find(PREF_ITEMEDITABLE);
	if (it != preferences.end())
		m_bTextEditable = it->second.uValue.bValue;

	it = preferences.find(PREF_MULTILINEINPLACEEDIT);
	if (it != preferences.end())
		m_bMultiLine = it->second.uValue.bValue;

	if (m_spFCO)
		resizeLogic.InitializeEx(pProject, pPart, pFCO, parentWnd, preferences);
	PartBase::InitializeEx(pProject, pPart, pFCO, parentWnd, preferences);

	// Get Text
	it = preferences.find(DecoratorSDK::PREF_TEXTOVERRIDE);
	if (it == preferences.end()) {
		CComBSTR bstrText;
		CComPtr<IMgaMetaFCO> spMetaFCO;
		if (!m_spFCO) {
			if (m_spPart) {
				CComPtr<IMgaMetaRole> spRole;
				COMTHROW(m_spPart->get_Role(&spRole));
				COMTHROW(spRole->get_Kind(&spMetaFCO));

				CComBSTR bstrKindName;
				COMTHROW(spMetaFCO->get_Name(&bstrKindName));
				CComBSTR bstrRoleName;
				COMTHROW(spRole->get_Name(&bstrRoleName));
				if (bstrKindName == bstrRoleName) {
					COMTHROW(spMetaFCO->get_DisplayedName(&bstrText));
				}
				else {
					bstrText = bstrRoleName;
				}
			}
		} else {
			COMTHROW(m_spFCO->get_Name(&bstrText));
		}
		m_strText = bstrText;
	}

	// Text's Font
	m_iFontKey = FONT_LABEL;
	it = preferences.find(PREF_LABELFONT);
	if (it != preferences.end())
		m_iFontKey = it->second.uValue.lValue;

	// Text's Color
	it = preferences.find(DecoratorSDK::PREF_TEXTCOLOROVERRIDE);
	if (it == preferences.end()) {
		m_crText = COLOR_BLACK;
		it = preferences.find(PREF_LABELCOLOR);
		if (it != preferences.end()) {
			m_crText = it->second.uValue.crValue;
		} else {
			if (m_spFCO)
				getFacilities().getPreference(m_spFCO, PREF_LABELCOLOR, m_crText);
		}
	}

	m_iMaxTextLength = MAX_LABEL_LENGTH;

	// Text's Location
	m_eTextLocation = L_SOUTH;
	it = preferences.find(PREF_LABELLOCATION);
	if (it != preferences.end()) {
		m_eTextLocation = it->second.uValue.eLocation;
	} else {
		if (m_spFCO)
			getFacilities().getPreference(m_spFCO, PREF_LABELLOCATION, m_eTextLocation);
	}

	// Text's Enabled
	if (m_spFCO) {
		if (!getFacilities().getPreference(m_spFCO, PREF_LABELENABLED, m_bTextEnabled))
			m_bTextEnabled = true;
	} else {
		m_bTextEnabled = true;
	}

	// Text's Wrap
	if (m_spFCO) {
		if (getFacilities().getPreference(m_spFCO, PREF_LABELWRAP, m_iTextWrapCount)) {
			m_iTextWrapCount = max(m_iTextWrapCount, 0L);
		} else {
			m_iTextWrapCount = 0;
		}
	} else {
		m_iTextWrapCount = 0;
	}
}

bool TextPart::MouseMoved(UINT nFlags, const CPoint& point, HDC transformHDC)
{
	if (m_bActive && m_bSelected && m_bTextEditable) {
		CRect ptRect = GetTextLocation();
		CRect ptRectInflated = ptRect;
		ptRectInflated.InflateRect(3, 3);
		if (ptRect.PtInRect(point)) {
			HCURSOR wantedCursor = LoadCursor(NULL, IDC_IBEAM);
			m_originalCursor = SetCursor(wantedCursor);
			CursorChanged((long)IDC_IBEAM);
			m_bCursorSaved = true;
			return true;
		} else {
			if (m_bCursorSaved) {
				SetCursor(m_originalCursor);
				m_bCursorSaved = false;
				CursorRestored();
			}
		}
	} else {
		if (m_bCursorSaved) {
			SetCursor(m_originalCursor);
			m_bCursorSaved = false;
			CursorRestored();
		}
	}

	return false;
}

bool TextPart::MouseLeftButtonUp(UINT nFlags, const CPoint& point, HDC transformHDC)
{
	return HandleTextEditOperation(false, point, transformHDC);
}

bool TextPart::MouseLeftButtonDoubleClick(UINT nFlags, const CPoint& point, HDC transformHDC)
{
	return HandleTextEditOperation(true, point, transformHDC);
}

bool TextPart::MouseRightButtonDown(HMENU hCtxMenu, UINT nFlags, const CPoint& point, HDC transformHDC)
{
	if (m_bActive && m_bSelected && m_bTextEditable) {
		CRect ptRect = GetTextLocation();
		CRect ptRectInflated = ptRect;
		ptRectInflated.InflateRect(3, 3);
		if (ptRectInflated.PtInRect(point)) {
			::AppendMenu(hCtxMenu, MF_STRING | MF_ENABLED, CTX_MENU_ID_RENAME, CTX_MENU_STR_RENAME);
			return true;
		}
	}

	return false;
}

bool TextPart::MenuItemSelected(UINT menuItemId, UINT nFlags, const CPoint& point, HDC transformHDC)
{
	if (menuItemId == CTX_MENU_ID_RENAME)	// simulate left click -> starting text editing action
		return MouseLeftButtonDown(nFlags, point, transformHDC);

	return false;
}

bool TextPart::OperationCanceledByGME(void)
{
	// destroy inplace edit window and stuff if needed
	// can't happen currently because of Modal dialog style
	if (m_spFCO)
		return resizeLogic.OperationCanceledByGME();

	return false;
}

long TextPart::GetLongest(void) const
{
	return m_strText.GetLength();
}

CRect TextPart::GetTextLocation(void) const
{
	CRect txtLoc;

	txtLoc = GetTextLocation(getFacilities().getCDC(), getFacilities().getGraphics());

	return txtLoc;
}

bool TextPart::HandleTextEditOperation(bool isDoubleClick, const CPoint& point, HDC transformHDC)
{
	if (m_bActive && (m_bSelected || isDoubleClick) && m_bTextEditable) {
		CRect ptRect = GetTextLocation();
		CRect ptRectInflated = ptRect;
		ptRectInflated.InflateRect(3, 3);
		if (!ptRectInflated.PtInRect(point))
			return false;

		bool isPermanentCWnd = true;
		CWnd* cWnd = CWnd::FromHandlePermanent(m_parentWnd);
		if (cWnd == NULL) {
			isPermanentCWnd = false;
			cWnd = CWnd::FromHandle(m_parentWnd);
		}

		CInPlaceEditDialog inPlaceEditDlg;
		CDialogTemplate dlgTemplate(_T(""),
									WS_POPUP | WS_VISIBLE | WS_BORDER,	// Window Styles
									0,	// Extended Window Styles
									0, 0, 76, 16);
		DWORD editStyle = WS_VISIBLE | WS_TABSTOP | ES_AUTOHSCROLL;	// ES_AUTOHSCROLL is important for proper auto width handling!
		if (m_bMultiLine)
			editStyle |= ES_MULTILINE | ES_AUTOVSCROLL | ES_WANTRETURN;

		dlgTemplate.AddRichEdit(_T("REDT"), editStyle, 0, 0, 0, 75, 16, IDC_INPLACETEXTEDIT);

		bool inflateToRight = true;
		PortLabelPart* portLabelPart = dynamic_cast_PortLabelPart();
		if (portLabelPart != NULL)
			if (portLabelPart->GetLocationAdjust() == L_WEST)
				inflateToRight = false;

		inPlaceEditDlg.SetProperties(m_strText, this, ptRect, point, cWnd, isPermanentCWnd, transformHDC,
									 m_iFontKey, inflateToRight, m_bMultiLine);
		BOOL success = ::AfxInitRichEdit2();	// See http://support.microsoft.com/kb/166132
		success = inPlaceEditDlg.InitModalIndirect(dlgTemplate, cWnd);
		INT_PTR retVal = 0;
		if (success != FALSE)
			retVal = inPlaceEditDlg.DoModal();

		cWnd->SetFocus();	// Give back focus to GME (JIRA GME-226)
		if (!isPermanentCWnd)
			cWnd->Detach();

		return true;
	}

	return false;
}

}; // namespace DecoratorSDK
