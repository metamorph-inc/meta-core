//################################################################################################
//
// Class label part class (decorator part)
//	ClassLabelPart.cpp
//
//################################################################################################

#include "StdAfx.h"
#include "ClassLabelPart.h"


namespace DecoratorSDK {

//################################################################################################
//
// CLASS : ClassLabelPart
//
//################################################################################################

ClassLabelPart::ClassLabelPart(PartBase* pPart, CComPtr<IMgaCommonDecoratorEvents>& eventSink, bool bIsAbstract):
	TextPart(pPart, eventSink),
	m_bIsAbstract(bIsAbstract)
{
}

ClassLabelPart::~ClassLabelPart()
{
}

void ClassLabelPart::Draw(CDC* pDC, Gdiplus::Graphics* gdip)
{
	if (m_bTextEnabled) {
		CRect loc = GetLocation();
		DecoratorSDK::GdipFont* pFont = getFacilities().GetFont(m_iFontKey);
		CSize size = getFacilities().MeasureText(gdip, pFont, m_strText);
		getFacilities().DrawString(gdip,
								   m_strText,
								   CRect(loc.left, loc.top + m_labelRelYPosition - size.cy,
										 loc.right, loc.top + m_labelRelYPosition),
								   pFont,
								   (m_bActive) ? m_crText : COLOR_GREY,
								   TA_BOTTOM | TA_CENTER,
								   m_iMaxTextLength,
								   "",
								   "",
								   false);
	}
	if (m_spFCO)
		resizeLogic.Draw(pDC, gdip);
}

// New functions
void ClassLabelPart::InitializeEx(CComPtr<IMgaProject>& pProject, CComPtr<IMgaMetaPart>& pPart, CComPtr<IMgaFCO>& pFCO,
								  HWND parentWnd, PreferenceMap& preferences)
{
	TextPart::InitializeEx(pProject, pPart, pFCO, parentWnd, preferences);

	if (m_bIsAbstract)
		m_iFontKey = FONT_ABSTRACT;

	VARIANT_BOOL isLibObject = VARIANT_TRUE;
	if (pFCO)
		pFCO->get_IsLibObject(&isLibObject);
	m_bTextEditable = isLibObject == VARIANT_FALSE;
}

CPoint	ClassLabelPart::GetTextPosition(CDC* pDC, Gdiplus::Graphics* gdip) const
{
	return GetTextLocation(pDC, gdip).TopLeft();
}

CRect ClassLabelPart::GetTextLocation(CDC* pDC, Gdiplus::Graphics* gdip) const
{
	CRect loc = GetLocation();

	DecoratorSDK::GdipFont* pFont = getFacilities().GetFont(m_iFontKey);
	CSize cSize = getFacilities().MeasureText(gdip, pFont, m_strText);

	long centerline = (loc.left + loc.right) / 2;
	return CRect(centerline - cSize.cx / 2,
				 loc.top + m_labelRelYPosition - cSize.cy,
				 centerline + cSize.cx / 2,
				 loc.top + m_labelRelYPosition);
}

void ClassLabelPart::ExecuteOperation(const CString& newString)
{
	// transaction operation begin
	m_strText = newString;
	CComBSTR bstr;
	CopyTo(newString, bstr);
	COMTHROW(m_spFCO->put_Name(bstr));
	// transaction operation end
}

}; // namespace DecoratorSDK
