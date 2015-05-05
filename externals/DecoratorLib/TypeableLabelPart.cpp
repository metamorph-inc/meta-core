//################################################################################################
//
// Typeable Label part class (decorator part)
//	TypeableLabelPart.cpp
//
//################################################################################################

#include "StdAfx.h"
#include "TypeableLabelPart.h"


namespace DecoratorSDK {

//################################################################################################
//
// CLASS : TypeableLabelPart
//
//################################################################################################

TypeableLabelPart::TypeableLabelPart(PartBase* pPart, CComPtr<IMgaCommonDecoratorEvents>& eventSink):
	LabelPart(pPart, eventSink),
	m_bTypeNameEnabled(false),
	m_iTypeInfo(0),
	m_strTypeName("")
{
}

TypeableLabelPart::~TypeableLabelPart()
{
}

void TypeableLabelPart::Draw(CDC* pDC, Gdiplus::Graphics* gdip)
{
	LabelPart::Draw(pDC, gdip);

	if (m_bTypeNameEnabled && (m_iTypeInfo == 3 || m_iTypeInfo == 2)) {
		ECoordRefPoint eAlign = GetAlignment(m_eTextLocation);
		int iAlign = 0;
		switch (eAlign) {
			case CRP_BEGIN:		iAlign = TA_LEFT;	break;
			case CRP_CENTER:	iAlign = TA_CENTER;	break;
			case CRP_END:
			default:			iAlign = TA_RIGHT;	break;
		}
		iAlign |= TA_TOP;
		CRect cRect = GetTextLocation(pDC, gdip);
		int iLabelSize = getFacilities().GetFont(FONT_TYPE)->iSize;
		long yPos = cRect.top + GAP_LABEL + iLabelSize * m_vecText.size();

		getFacilities().DrawString(gdip,
								   m_strTypeName,
								   CRect(cRect.left - GAP_LABEL, yPos,
										 cRect.right + GAP_LABEL, yPos + iLabelSize),
								   getFacilities().GetFont(FONT_TYPE),
								   (m_bActive) ? m_crText : COLOR_GRAY,
								   iAlign,
								   MAX_TYPE_LENGTH,
								   "[ ",
								   " ]");
	}
}

// New functions
void TypeableLabelPart::InitializeEx(CComPtr<IMgaProject>& pProject, CComPtr<IMgaMetaPart>& pPart, CComPtr<IMgaFCO>& pFCO,
									 HWND parentWnd, PreferenceMap& preferences)
{
	if (m_spFCO) {
		PreferenceMap::iterator it = preferences.find(PREF_TYPESHOWN);
		if (it != preferences.end()) {
			m_bTypeNameEnabled = it->second.uValue.bValue;
		} else {
			if (!getFacilities().getPreference(m_spFCO, PREF_TYPESHOWN, m_bTypeNameEnabled))
				m_bTypeNameEnabled = false;
		}

		VARIANT_BOOL bInstance = VARIANT_FALSE;
		COMTHROW(m_spFCO->get_IsInstance(&bInstance));
		if (bInstance == VARIANT_TRUE) {
			m_iTypeInfo = 3;
			if (m_bTypeNameEnabled) {
				CComPtr<IMgaFCO> spType;
				COMTHROW(m_spFCO->get_DerivedFrom(&spType));
				CComBSTR bstrName;
				COMTHROW(spType->get_Name(&bstrName));
				m_strTypeName = bstrName;
			}
		} else {
			CComPtr<IMgaFCO> spType;
			COMTHROW(m_spFCO->get_DerivedFrom(&spType));
			if (spType) {
				m_iTypeInfo = 2;
				// proba [
				CComBSTR bstrName;
				COMTHROW(spType->get_Name(&bstrName));
				m_strTypeName = bstrName;
				// ] proba
			} else {
				CComPtr<IMgaFCOs> spFCOs;
				COMTHROW(m_spFCO->get_DerivedObjects(&spFCOs));
				long lCount = 0;
				COMTHROW( spFCOs->get_Count(&lCount));
				m_iTypeInfo = (lCount == 0) ? 0 : 1;
			}
		}
	}

	LabelPart::InitializeEx(pProject, pPart, pFCO, parentWnd, preferences);
}

CPoint	TypeableLabelPart::GetTextPosition(CDC* pDC, Gdiplus::Graphics* gdip) const
{
	CPoint pt = LabelPart::GetTextPosition(pDC, gdip);
	if (m_bTypeNameEnabled && (m_iTypeInfo == 3 || m_iTypeInfo == 2)) {
		int iLabelSize = getFacilities().GetFont(FONT_TYPE)->iSize;
		switch(m_eTextLocation) {
			case L_NORTH:
			case L_NORTHWEST:
			case L_NORTHEAST:
				pt.y = pt.y - iLabelSize - GAP_LABEL;
				break;
			case L_CENTER:
			case L_WEST:
			case L_EAST:
				pt.y = pt.y - iLabelSize / 2 - GAP_LABEL / 2;
				break;
		}
	}
	return pt;
}

CRect TypeableLabelPart::GetTextLocation(CDC* pDC, Gdiplus::Graphics* gdip) const
{
	CRect cRect = LabelPart::GetTextLocation(pDC, gdip);

	if (m_bTypeNameEnabled && (m_iTypeInfo == 3 || m_iTypeInfo == 2)) {
		LOGFONT logFont;
		getFacilities().GetFont(FONT_TYPE)->gdipFont->GetLogFontT(gdip, &logFont);
		ECoordRefPoint eAlign = GetAlignment(m_eTextLocation);

		CSize cSize(logFont.lfWidth * m_strTypeName.GetLength(), logFont.lfHeight);
		cRect.bottom += cSize.cy + GAP_LABEL;
		if (cSize.cx > cRect.Width()) {
			switch (eAlign) {
				case CRP_BEGIN :
					cRect.right += cSize.cx - cRect.Width();
					break;
				case CRP_CENTER :
					cRect.left -= (cSize.cx - cRect.Width()) / 2;
					cRect.right += (cSize.cx - cRect.Width()) / 2;
					break;
				case CRP_END :
					cRect.left -= cSize.cx - cRect.Width();
					break;
			}
		}
	}

	return cRect;
}

}; // namespace DecoratorSDK
