//################################################################################################
//
// Stereo label part class (decorator part)
//	StereoLabelPart.cpp
//
//################################################################################################

#include "StdAfx.h"
#include "StereoLabelPart.h"


namespace DecoratorSDK {

//################################################################################################
//
// CLASS : StereoLabelPart
//
//################################################################################################

StereoLabelPart::StereoLabelPart(PartBase* pPart, CComPtr<IMgaCommonDecoratorEvents>& eventSink):
	TextPart(pPart, eventSink)
{
}

StereoLabelPart::~StereoLabelPart()
{
}

void StereoLabelPart::Draw(CDC* pDC, Gdiplus::Graphics* gdip)
{
	if (m_bTextEnabled) {
		CRect loc = GetLocation();
		DecoratorSDK::GdipFont* pFont = getFacilities().GetFont(m_iFontKey);
		CSize size = getFacilities().MeasureText(gdip, pFont, m_strText);
		getFacilities().DrawString(gdip,
								   DecoratorSDK::getFacilities().getStereotyped(m_strText),
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

CPoint	StereoLabelPart::GetTextPosition(CDC* pDC, Gdiplus::Graphics* gdip) const
{
	return GetTextLocation(pDC, gdip).TopLeft();
}

CRect StereoLabelPart::GetTextLocation(CDC* pDC, Gdiplus::Graphics* gdip) const
{
	CRect loc = GetLocation();

	DecoratorSDK::GdipFont* pFont = getFacilities().GetFont(m_iFontKey);
	CSize cSize = getFacilities().MeasureText(gdip, pFont, DecoratorSDK::getFacilities().getStereotyped(m_strText));

	long centerline = (loc.left + loc.right) / 2;
	return CRect(centerline - cSize.cx / 2,
				 loc.top + m_labelRelYPosition - cSize.cy,
				 centerline + cSize.cx / 2,
				 loc.top + m_labelRelYPosition);
}

}; // namespace DecoratorSDK
