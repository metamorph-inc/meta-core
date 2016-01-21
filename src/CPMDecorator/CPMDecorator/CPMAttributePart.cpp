/*
*    Copyright (c) Vanderbilt University,  2011
*    ALL RIGHTS RESERVED
*
*/ 

//################################################################################################
//
// CPM attribute part class (decorator part)
//	CPMAttributePart.cpp
//
//################################################################################################

#include "StdAfx.h"

#include "DecoratorDefs.h"
#include "DecoratorUtil.h"

#include "CPMAttributePart.h"

#include "DecoratorLib.h"

namespace Decor {

//################################################################################################
//
// CLASS : CPMAttributePart
//
//################################################################################################

CPMInternalTextPart::CPMInternalTextPart(PartBase* pPart, CComPtr<IMgaCommonDecoratorEvents> eventSink,
										const CString& strText, CComPtr<IMgaFCO>& pFCO, bool bMultiLine,
										int iFontKey):
	TextPart(pPart, eventSink),
	m_spActualFCO(pFCO)
{
	m_strText = strText;
	m_strText.Replace(L"\r", NULL);
	m_strText.Replace(L"\n", L"\r\n");
	m_bMultiLine = bMultiLine;
	m_textRelXPosition = 0;
	m_textRelYPosition = 0;
	m_iFontKey = iFontKey;

	if (m_strText.GetLength() > 35)
	{
		m_strText = m_strText.Left(30) + L"...";
		m_bTextEditable = false;
	}
}

CPMInternalTextPart::~CPMInternalTextPart()
{
}

void CPMInternalTextPart::Draw(CDC* pDC, Gdiplus::Graphics* gdip)
{
	if (m_bTextEnabled) {
		CRect loc = GetLocation();
		DecoratorSDK::GdipFont* pFont = DecoratorSDK::getFacilities().GetFont(m_iFontKey);
		CSize size = GetTextSize(gdip, pFont);
		if (m_strText.GetLength())
		{
			DecoratorSDK::getFacilities().DrawString(gdip,
									   m_strText,
									   CRect(loc.left + m_textRelXPosition, loc.top + m_textRelYPosition - size.cy,
											 loc.right + m_textRelXPosition, loc.top + m_textRelYPosition),
									   pFont,
									   (m_bActive) ? m_crText : DecoratorSDK::COLOR_GREY,
									   TA_BOTTOM | TA_LEFT,
									   INT_MAX,
									   L"",
									   L"",
									   false);
		}
		else
		{
			DecoratorSDK::getFacilities().DrawRect(gdip, CRect(loc.left + m_textRelXPosition + 3 , loc.top + m_textRelYPosition - size.cy,
											 loc.left + m_textRelXPosition + size.cx - 3 - 1, loc.top + m_textRelYPosition - 1), DecoratorSDK::COLOR_GREY, 1);
		}
	}
	if (m_spFCO)
		resizeLogic.Draw(pDC, gdip);
}

CRect CPMInternalTextPart::GetTextLocation(CDC* pDC, Gdiplus::Graphics* gdip) const
{
	CRect loc = GetLocation();

	DecoratorSDK::GdipFont* pFont = DecoratorSDK::getFacilities().GetFont(m_iFontKey);
	auto cSize = GetTextSize(gdip, pFont);

	return CRect(loc.left + m_textRelXPosition,
				 loc.top + m_textRelYPosition - cSize.cy,
				 loc.left + m_textRelXPosition + cSize.cx,
				 loc.top + m_textRelYPosition);
}

CPoint CPMInternalTextPart::GetTextPosition(CDC* pDC, Gdiplus::Graphics* gdip) const
{
	return GetTextLocation(pDC, gdip).TopLeft();
}

CSize CPMInternalTextPart::GetRelSize(CDC* pDC, Gdiplus::Graphics* gdip) const
{
	DecoratorSDK::GdipFont* pFont = DecoratorSDK::getFacilities().GetFont(m_iFontKey);
	return GetTextSize(gdip, pFont);
}

void CPMInternalTextPart::InitializeEx(CComPtr<IMgaProject>& pProject, CComPtr<IMgaMetaPart>& pPart,
									   CComPtr<IMgaFCO>& pFCO, HWND parentWnd, DecoratorSDK::PreferenceMap& preferences) 
{
	//int iFontKey = m_iFontKey;
	DecoratorSDK::TextPart::InitializeEx(pProject, pPart, pFCO, parentWnd, preferences);
	//m_iFontKey = iFontKey;
}

}; // namespace Decor
