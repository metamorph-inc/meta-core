//################################################################################################
//
// Port bitmap part decorator class (decorator part)
//	PortBitmapPart.cpp
//
//################################################################################################

#include "StdAfx.h"
#include "PortBitmapPart.h"


namespace DecoratorSDK {

//################################################################################################
//
// CLASS : PortBitmapPart
//
//################################################################################################

PortBitmapPart::PortBitmapPart(PartBase* pPart, CComPtr<IMgaCommonDecoratorEvents>& eventSink, const CPoint& ptInner):
	TypeableBitmapPart(pPart, eventSink),
	m_ptInner(ptInner)
{
}

PortBitmapPart::~PortBitmapPart()
{
}

CSize PortBitmapPart::GetPreferredSize(void) const
{
	CSize size = ResizablePart::GetPreferredSize();
	if (size.cx * size.cy != 0)
		return size;

	return CSize(WIDTH_PORT, HEIGHT_PORT);
}

// New functions
void PortBitmapPart::InitializeEx(CComPtr<IMgaProject>& pProject, CComPtr<IMgaMetaPart>& pPart, CComPtr<IMgaFCO>& pFCO,
									HWND parentWnd, PreferenceMap& preferences)
{
	CString strIcon;
	if (getFacilities().getPreference(pFCO, PREF_PORTICON, strIcon)) {
		preferences[PREF_ICON] = PreferenceVariant(strIcon);
		preferences[PREF_TILES] = PreferenceVariant(getFacilities().getTileVector(TILE_PORTDEFAULT));
	}

	COMTHROW(pFCO->get_ObjType(&m_eType));
	switch (m_eType) {
		case OBJTYPE_MODEL :
			strIcon = createResString(IDB_MODELPORT);
			break;
		case OBJTYPE_SET :
			strIcon = createResString(IDB_SETPORT);
			break;
		case OBJTYPE_REFERENCE :
			strIcon = createResString(IDB_REFERENCEPORT);
			break;
		default :
			strIcon = createResString(IDB_ATOMPORT);
			break;
	}
	preferences[PREF_ICONDEFAULT]	= PreferenceVariant(strIcon);
	preferences[PREF_TILESDEFAULT]	= PreferenceVariant(getFacilities().getTileVector(TILE_PORTDEFAULT));
	preferences[PREF_TILESUNDEF]	= PreferenceVariant(getFacilities().getTileVector(TILE_PORTDEFAULT));
	preferences[PREF_ITEMSHADOWCAST]= PreferenceVariant(false);
	preferences[PREF_TYPEINFOSHOWN] = PreferenceVariant(false);

	TypeableBitmapPart::InitializeEx(pProject, pPart, pFCO, parentWnd, preferences);
}

void PortBitmapPart::DrawBackground(CDC* pDC, Gdiplus::Graphics* gdip)
{
	CRect cRect = GetBoxLocation(false);
	if (m_bActive) {
		m_pBitmap->draw(gdip, pDC, cRect, *m_pTileVector);
	} else {
		cRect.right--;
		cRect.bottom--;
		getFacilities().DrawRect(gdip, cRect, COLOR_GRAY, 1);
	}
}

}; // namespace DecoratorSDK
