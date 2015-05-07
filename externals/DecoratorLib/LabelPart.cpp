//################################################################################################
//
// Label part class (decorator part)
//	LabelPart.cpp
//
//################################################################################################

#include "StdAfx.h"
#include "LabelPart.h"


namespace DecoratorSDK {

//################################################################################################
//
// CLASS : LabelPart
//
//################################################################################################

LabelPart::LabelPart(PartBase* pPart, CComPtr<IMgaCommonDecoratorEvents>& eventSink):
	TextPart(pPart, eventSink)
{
}

LabelPart::~LabelPart()
{
}

void LabelPart::Draw(CDC* pDC, Gdiplus::Graphics* gdip)
{
	if (m_bTextEnabled) {
		ECoordRefPoint eAlign = GetAlignment(m_eTextLocation);
		int iAlign = 0;
		switch (eAlign) {
			case CRP_BEGIN:		iAlign = TA_LEFT;	break;
			case CRP_CENTER:	iAlign = TA_CENTER;	break;
			case CRP_END:
			default:			iAlign = TA_RIGHT;	break;
		}
		iAlign |= TA_TOP;

		DecoratorSDK::GdipFont* pFont = getFacilities().GetFont(m_iFontKey);
		int iLabelSize = pFont->iSize;
		CRect cRect = GetTextLocation(pDC, gdip);
		for (unsigned int i = 0; i < m_vecText.size(); i++)
			getFacilities().DrawString(gdip,
									   m_vecText[i],
									   CRect(cRect.left - GAP_LABEL, cRect.top + i * iLabelSize,
											 cRect.right + GAP_LABEL, cRect.top + (i + 1) * iLabelSize),
									   pFont,
									   (m_bActive) ? m_crText : COLOR_GRAY,
									   iAlign,
									   m_iMaxTextLength,
									   "",
									   "",
									   false);
	}
	if (m_spFCO)
		resizeLogic.Draw(pDC, gdip);
}

CPoint	LabelPart::GetTextPosition(CDC* pDC, Gdiplus::Graphics* gdip) const
{
	CPoint pt;
	CRect cRect = GetLocation();	// GetBoxLocation(true)
	int iLabelSize = getFacilities().GetFont(m_iFontKey)->iSize * m_vecText.size();
	switch(m_eTextLocation) {
		case L_NORTH:
		case L_NORTHWEST:
		case L_NORTHEAST:
			pt.y = cRect.top - iLabelSize - GAP_LABEL;
			break;
		case L_SOUTH:
		case L_SOUTHWEST:
		case L_SOUTHEAST:
			pt.y = cRect.bottom + GAP_LABEL;
			break;
		default:
			pt.y = cRect.CenterPoint().y - iLabelSize / 2;
			break;
	}
	switch(m_eTextLocation) {
		case L_WEST:
		case L_NORTHWEST:
		case L_SOUTHWEST:
			pt.x = cRect.left - GAP_LABEL;
			break;
		case L_NORTH:
		case L_CENTER:
		case L_SOUTH:
			pt.x = cRect.CenterPoint().x;
			break;
		default:
			pt.x = cRect.right + GAP_LABEL;
			break;
	}
	return pt;
}

CRect LabelPart::GetTextLocation(CDC* pDC, Gdiplus::Graphics* gdip) const
{
	CPoint pt = GetTextPosition(pDC, gdip);
	ECoordRefPoint eAlign = GetAlignment(m_eTextLocation);

	DecoratorSDK::GdipFont* pFont = getFacilities().GetFont(m_iFontKey);

	CSize cSize(0, 0);
	for (unsigned int i = 0 ; i < m_vecText.size(); i++) {
		CSize tmpSize = getFacilities().MeasureText(gdip, pFont, m_iMaxTextLength >= 0 ? m_vecText[i].Left(m_iMaxTextLength) : m_vecText[i]);
		cSize.cy += tmpSize.cy;
		cSize.cx = max(cSize.cx, tmpSize.cx);
	}

	if (eAlign == CRP_CENTER)
		pt.x -= cSize.cx / 2;
	else if (eAlign == CRP_END)
		pt.x -= cSize.cx;

	return CRect(pt.x, pt.y, pt.x + cSize.cx, pt.y + cSize.cy);
}

void LabelPart::ExecuteOperation(const CString& newString)
{
	// transaction operation begin
	m_strText = newString;
	CComBSTR bstr;
	CopyTo(newString, bstr);
	COMTHROW(m_spFCO->put_Name(bstr));
	// transaction operation end
}

void LabelPart::InitializeEx(CComPtr<IMgaProject>& pProject, CComPtr<IMgaMetaPart>& pPart, CComPtr<IMgaFCO>& pFCO,
							HWND parentWnd, PreferenceMap& preferences)
{
	TextPart::InitializeEx(pProject, pPart, pFCO, parentWnd, preferences);
	PreferenceMap::iterator it = preferences.find(PREF_LABEL);
	if (it != preferences.end())
		m_strText = *it->second.uValue.pstrValue;

	m_vecText = getFacilities().wrapString(m_strText, m_iTextWrapCount, m_iMaxTextLength);
}

long LabelPart::GetLongest(void) const
{
	long maxv = 0;
	for (unsigned int i = 0; i < m_vecText.size(); i++) {
		long ilen = m_vecText[i].GetLength();
		if (m_iMaxTextLength > 0)
			ilen = min(ilen, m_iMaxTextLength);
		if (maxv < ilen)
			maxv = ilen;
	}
	return maxv;
}

}; // namespace DecoratorSDK
