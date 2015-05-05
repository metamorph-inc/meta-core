//################################################################################################
//
// Masked Bitmap decorator class
//	MaskedBitmapPart.cpp
//
//################################################################################################

#include "StdAfx.h"
#include "MaskedBitmapPart.h"


namespace DecoratorSDK {

//################################################################################################
//
// CLASS : MaskedBitmapPart
//
//################################################################################################

MaskedBitmapPart::MaskedBitmapPart(PartBase* pPart, CComPtr<IMgaCommonDecoratorEvents>& eventSink, long bitmapResID,
								   COLORREF transparentColor, COLORREF grayedOutColor):
	BitmapPart		(pPart, eventSink),
	m_lBitmapResID	(bitmapResID),
	m_crTransparent	(transparentColor),
	m_crGrayedOut	(grayedOutColor)
{
}

MaskedBitmapPart::~MaskedBitmapPart()
{
}

// New functions
void MaskedBitmapPart::InitializeEx(CComPtr<IMgaProject>& pProject, CComPtr<IMgaMetaPart>& pPart, CComPtr<IMgaFCO>& pFCO,
										HWND parentWnd, PreferenceMap& preferences)
{
	bool bResizeable = true;
	PreferenceMap::iterator it = preferences.find(PREF_ITEMRESIZABLE);
	if (it != preferences.end())
		bResizeable = it->second.uValue.bValue;

	preferences[PREF_ISMASKEDBITMAP]	= PreferenceVariant(true);
	preferences[PREF_ITEMRESIZABLE]		= PreferenceVariant(false);

	preferences[PREF_ICONDEFAULT]		= PreferenceVariant(createResString(IDB_ATOM));
	preferences[PREF_TILESDEFAULT]		= PreferenceVariant(getFacilities().getTileVector(TILE_ATOMDEFAULT));
	preferences[PREF_TILESUNDEF]		= PreferenceVariant(getFacilities().getTileVector(TILE_ATOMDEFAULT));
	preferences[PREF_TILES]				= PreferenceVariant(getFacilities().getTileVector(TILE_ATOMDEFAULT));

	preferences[PREF_ICON]				= PreferenceVariant(m_lBitmapResID);
	preferences[PREF_TRANSPARENTCOLOR]	= PreferenceVariant(m_crTransparent);
	preferences[PREF_GRAYEDOUTCOLOR]	= PreferenceVariant(m_crGrayedOut);
	BitmapPart::InitializeEx(pProject, pPart, pFCO, parentWnd, preferences);
	preferences[PREF_ISMASKEDBITMAP]	= PreferenceVariant(false);
	preferences[PREF_ITEMRESIZABLE]		= PreferenceVariant(bResizeable);
}

void MaskedBitmapPart::DrawBackground(CDC* pDC, Gdiplus::Graphics* gdip)
{
	CRect cRect = GetBoxLocation(false);
	UINT modifFlags = MF_TRANSPARENT;
	if (!m_bActive)
		modifFlags |= MF_GREYED;
	m_pBitmap->draw(gdip, pDC, cRect, *m_pTileVector, modifFlags);
}

}; // namespace DecoratorSDK
