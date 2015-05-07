//################################################################################################
//
// Attribute part class (decorator part)
//	AttributePart.cpp
//
//################################################################################################

#include "StdAfx.h"
#include "AttributePart.h"


namespace DecoratorSDK {

//################################################################################################
//
// CLASS : AttributePart
//
//################################################################################################

AttributePart::AttributePart(PartBase* pPart, CComPtr<IMgaCommonDecoratorEvents>& eventSink):
	TextPart(pPart, eventSink)
{
}

AttributePart::~AttributePart()
{
}

void AttributePart::Draw(CDC* pDC, Gdiplus::Graphics* gdip)
{
	if (m_bTextEnabled) {
		CRect loc = GetLocation();
		DecoratorSDK::GdipFont* pFont = getFacilities().GetFont(m_iFontKey);
		CSize size = getFacilities().MeasureText(gdip, pFont, m_strText);
		getFacilities().DrawString(gdip,
								   m_strText + ATTRIBUTE_SEP,
								   CRect(loc.left + DECORATOR_MARGINX, loc.top + m_textRelYPosition - size.cy,
										 loc.right - DECORATOR_MARGINX, loc.top + m_textRelYPosition),
								   pFont,
								   (m_bActive) ? m_crText : COLOR_GREY,
								   TA_BOTTOM | TA_LEFT,
								   m_iMaxTextLength,
								   "",
								   "",
								   false);
		getFacilities().DrawString(gdip,
								   m_strType,
								   CRect(loc.left + DECORATOR_MARGINX, loc.top + m_textRelYPosition - size.cy,
										 loc.right - DECORATOR_MARGINX, loc.top + m_textRelYPosition),
								   pFont,
								   (m_bActive) ? m_crText : COLOR_GREY,
								   TA_BOTTOM | TA_RIGHT,
								   m_iMaxTextLength,
								   "",
								   "",
								   false);
	}
	if (m_spFCO)
		resizeLogic.Draw(pDC, gdip);
}

CPoint	AttributePart::GetTextPosition(CDC* pDC, Gdiplus::Graphics* gdip) const
{
	return GetTextLocation(pDC, gdip).TopLeft();
}

CRect AttributePart::GetTextLocation(CDC* pDC, Gdiplus::Graphics* gdip) const
{
	CRect loc = GetLocation();

	DecoratorSDK::GdipFont* pFont = getFacilities().GetFont(m_iFontKey);
	CSize cSize = getFacilities().MeasureText(gdip, pFont, m_strText);

	return CRect(loc.left + DECORATOR_MARGINX,
				 loc.top + m_textRelYPosition - cSize.cy,
				 loc.left + DECORATOR_MARGINX + cSize.cx,
				 loc.top + m_textRelYPosition);
}

CSize AttributePart::GetNameSize(CDC* pDC, Gdiplus::Graphics* gdip) const
{
	ASSERT(gdip != NULL);
	DecoratorSDK::GdipFont* pFont = getFacilities().GetFont(m_iFontKey);
	return getFacilities().MeasureText(gdip, pFont, m_strText);
}

CSize AttributePart::GetTypeSize(CDC* pDC, Gdiplus::Graphics* gdip) const
{
	ASSERT(gdip != NULL);
	DecoratorSDK::GdipFont* pFont = getFacilities().GetFont(m_iFontKey);
	return getFacilities().MeasureText(gdip, pFont, m_strType);
}

CSize AttributePart::GetTextSize(CDC* pDC, Gdiplus::Graphics* gdip) const
{
	DecoratorSDK::GdipFont* pFont = getFacilities().GetFont(m_iFontKey);
	return getFacilities().MeasureText(gdip, pFont, m_strText + ATTRIBUTE_SEP + m_strType);
}

}; // namespace DecoratorSDK
