//################################################################################################
//
// Typeable bitmap part class (decorator part)
//	TypeableBitmapPart.cpp
//
//################################################################################################

#include "StdAfx.h"
#include "TypeableBitmapPart.h"


namespace DecoratorSDK {

//################################################################################################
//
// CLASS : TypeableBitmapPart
//
//################################################################################################

TypeableBitmapPart::TypeableBitmapPart(PartBase* pPart, CComPtr<IMgaCommonDecoratorEvents>& eventSink):
	BitmapPart(pPart, eventSink),
	m_bTypeIconEnabled(false),
	m_iTypeInfo(0)
{
}

TypeableBitmapPart::~TypeableBitmapPart()
{
}

// New functions
void TypeableBitmapPart::InitializeEx(CComPtr<IMgaProject>& pProject, CComPtr<IMgaMetaPart>& pPart, CComPtr<IMgaFCO>& pFCO,
									  HWND parentWnd, PreferenceMap& preferences)
{
	BitmapPart::InitializeEx(pProject, pPart, pFCO, parentWnd, preferences);

	if (m_spFCO) {
		PreferenceMap::iterator it = preferences.find(PREF_TYPEINFOSHOWN);
		if (it != preferences.end()) {
			m_bTypeIconEnabled = it->second.uValue.bValue;
		} else {
			if (!getFacilities().getPreference(m_spFCO, PREF_TYPEINFOSHOWN, m_bTypeIconEnabled))
				m_bTypeIconEnabled = false;
		}

		VARIANT_BOOL bInstance = VARIANT_FALSE;
		COMTHROW(m_spFCO->get_IsInstance(&bInstance));
		if (bInstance == VARIANT_TRUE) {
			m_iTypeInfo = 3;
		} else {
			CComPtr<IMgaFCO> spType;
			COMTHROW(m_spFCO->get_DerivedFrom(&spType));
			if (spType) {
				m_iTypeInfo = 2;
			} else {
				CComPtr<IMgaFCOs> spFCOs;
				COMTHROW(m_spFCO->get_DerivedObjects(&spFCOs));
				long lCount = 0;
				COMTHROW( spFCOs->get_Count(&lCount));
				m_iTypeInfo = (lCount == 0) ? 0 : 1;
			}
		}
	}

	// TypeInheritance Bitmap and Tiles

	if (m_spFCO && (m_iTypeInfo == 3 || m_iTypeInfo == 2)) {
		BitmapBase* pBitmap = NULL;
		TileVector* pTileVector = NULL;
		PreferenceMap::iterator it = preferences.find((m_iTypeInfo == 2) ? PREF_SUBTYPEICON : PREF_INSTANCEICON);
		if (it != preferences.end()) {
#ifndef OLD_DECORATOR_LOOKANDFEEL
			if (m_bOverlay)
				pBitmap = getFacilities().getBitmapB(*it->second.uValue.pstrValue, m_crOverlay);
			else
				pBitmap = getFacilities().getBitmap(*it->second.uValue.pstrValue);
#else
			pBitmap = getFacilities().getBitmap(*it->second.uValue.pstrValue);
#endif
			PreferenceMap::iterator tileIt = preferences.find(PREF_TILES);
			if (tileIt != preferences.end())
				pTileVector = tileIt->second.uValue.pTiles;
			else
				pTileVector = getFacilities().getTileVector(TILE_ATOMDEFAULT);
		}
		if (!pBitmap) {
			CString strIcon;
			if (getFacilities().getPreference(m_spFCO, (m_iTypeInfo == 2) ? PREF_SUBTYPEICON : PREF_INSTANCEICON, strIcon)) {
				PreferenceMap::iterator tileIt = preferences.find(PREF_TILESUNDEF);
				if (tileIt != preferences.end())
					pTileVector = tileIt->second.uValue.pTiles;
				else
					pTileVector = getFacilities().getTileVector(TILE_ATOMDEFAULT);
				if (!strIcon.IsEmpty()) {
#ifndef OLD_DECORATOR_LOOKANDFEEL
					if (m_bOverlay)
						pBitmap = getFacilities().getBitmapB(strIcon, m_crOverlay);
					else
						pBitmap = getFacilities().getBitmap(strIcon);
#else
					pBitmap = getFacilities().getBitmap(strIcon);
#endif
				}
			}
		}
		if (pBitmap) {
			m_pBitmap = pBitmap;
			m_pTileVector = pTileVector;
		}
	}
}


void TypeableBitmapPart::DrawIcons(CDC* pDC, Gdiplus::Graphics* gdip)
{
	BitmapPart::DrawIcons(pDC, gdip);

	if (m_bTypeIconEnabled && m_iTypeInfo != 0) {
		CRect cRect = GetBoxLocation(false);
		CString strRes;
		if (m_bActive)
			strRes = createResString((m_iTypeInfo == 1) ? IDB_ICON_TYPE : (m_iTypeInfo == 2) ? IDB_ICON_SUBTYPE : IDB_ICON_INSTANCE);
		else
			strRes = createResString((m_iTypeInfo == 1) ? IDB_ICON_TYPE_DIS : (m_iTypeInfo == 2) ? IDB_ICON_SUBTYPE_DIS : IDB_ICON_INSTANCE_DIS);
		BitmapBase* pBitmap = getFacilities().getBitmap(strRes);
		int width = (cRect.Width() - pBitmap->getWidth()) / 2;
		int height = (cRect.Height() - pBitmap->getHeight()) / 2;
		CRect rDest(cRect.left + width, cRect.top + height, cRect.left + width + pBitmap->getWidth(), cRect.top + height + pBitmap->getHeight());
		pBitmap->draw(gdip, pDC, rDest, TileVector(1, BackgroundTile()));
	}
}

}; // namespace DecoratorSDK
