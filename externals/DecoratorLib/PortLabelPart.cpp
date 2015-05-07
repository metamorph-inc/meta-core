//################################################################################################
//
// PortLabel part class (decorator part)
//	PortLabelPart.cpp
//
//################################################################################################

#include "StdAfx.h"
#include "PortLabelPart.h"


namespace DecoratorSDK {

//################################################################################################
//
// CLASS : PortLabelPart
//
//################################################################################################

PortLabelPart::PortLabelPart(PartBase* pPart, CComPtr<IMgaCommonDecoratorEvents>& eventSink):
	LabelPart(pPart, eventSink)
{
	m_bInsideContainer	= true;
	m_eAdjust			= L_WEST;
}

PortLabelPart::~PortLabelPart()
{
}

// New functions
void PortLabelPart::InitializeEx(CComPtr<IMgaProject>& pProject, CComPtr<IMgaMetaPart>& pPart, CComPtr<IMgaFCO>& pFCO,
								 HWND parentWnd, PreferenceMap& preferences)
{
	// Determines if port label displayed inside or outside container?
	m_bInsideContainer = preferences[PREF_PORTLABELINSIDE].uValue.bValue;

	// Label Font
	if(m_bInsideContainer) {
		preferences[PREF_LABELFONT] = PreferenceVariant((long) FONT_PORT);
	} else {
		preferences[PREF_LABELFONT] = PreferenceVariant((long) FONT_PORT_OUTSIDE);
	}

	m_eAdjust = preferences[PREF_LABELLOCATION].uValue.eLocation;

	LabelPart::InitializeEx(pProject, pPart, pFCO, parentWnd, preferences);

	// Text's Max Length
	m_iMaxTextLength = MAX_LABEL_LENGTH;
	PreferenceMap::iterator it = preferences.find(PREF_LABELLENGTH);
	if (it != preferences.end())
		m_iMaxTextLength = it->second.uValue.lValue;
}

bool PortLabelPart::MouseLeftButtonDoubleClick(UINT nFlags, const CPoint& point, HDC transformHDC)
{
	return PartBase::MouseLeftButtonDoubleClick(nFlags, point, transformHDC);
}

CPoint PortLabelPart::GetTextPosition(CDC* pDC, Gdiplus::Graphics* gdip) const
{
	int d = (m_eAdjust == L_EAST)? 1 : -1;
	CPoint pt = LabelPart::GetTextPosition(pDC, gdip);
	if (!m_bInsideContainer)
		pt += CPoint(d * 7, -7);

	return pt;
}

}; // namespace DecoratorSDK
