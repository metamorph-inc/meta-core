//################################################################################################
//
// Atom bitmap decorator class
//	AtomBitmapPart.cpp
//
//################################################################################################

#include "StdAfx.h"
#include "AtomBitmapPart.h"


namespace DecoratorSDK {

//################################################################################################
//
// CLASS : AtomBitmapPart
//
//################################################################################################

AtomBitmapPart::AtomBitmapPart(PartBase* pPart, CComPtr<IMgaCommonDecoratorEvents>& eventSink):
	TypeableBitmapPart(pPart, eventSink)
{
}

AtomBitmapPart::~AtomBitmapPart()
{
}

// New functions
void AtomBitmapPart::InitializeEx(CComPtr<IMgaProject>& pProject, CComPtr<IMgaMetaPart>& pPart, CComPtr<IMgaFCO>& pFCO,
								  HWND parentWnd, PreferenceMap& preferences)
{
	preferences[PREF_ICONDEFAULT]	= PreferenceVariant(createResString(IDB_ATOM));
	preferences[PREF_TILESDEFAULT]	= PreferenceVariant(getFacilities().getTileVector(TILE_ATOMDEFAULT));
	preferences[PREF_TILESUNDEF]	= PreferenceVariant(getFacilities().getTileVector(TILE_ATOMDEFAULT));

	TypeableBitmapPart::InitializeEx(pProject, pPart, pFCO, parentWnd, preferences);
}


void AtomBitmapPart::DrawBackground(CDC* pDC, Gdiplus::Graphics* gdip)
{
#ifndef OLD_DECORATOR_LOOKANDFEEL
	TypeableBitmapPart::DrawBackground(pDC, gdip);
#else
	if (m_bActive) {
		TypeableBitmapPart::DrawBackground(pDC, gdip);
	} else {
		CRect cRect = GetBoxLocation(false);
		int iDepth = (m_bReferenced) ? 2 : 8;
		getFacilities().DrawBox(gdip, cRect, COLOR_LIGHTGRAY, iDepth);
		getFacilities().DrawRect(gdip, cRect, COLOR_GRAY);
		cRect.DeflateRect(iDepth, iDepth);
		getFacilities().DrawRect(gdip, cRect, COLOR_GRAY);
	}
#endif
}

}; // namespace DecoratorSDK
