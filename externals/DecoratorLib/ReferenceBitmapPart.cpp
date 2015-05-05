//################################################################################################
//
// Reference bitmap part class (decorator part)
//	ReferenceBitmapPart.cpp
//
//################################################################################################

#include "StdAfx.h"
#include "ReferenceBitmapPart.h"
#include "AtomBitmapPart.h"
#include "SetBitmapPart.h"
#include "ModelComplexPart.h"
#include "DecoratorExceptions.h"


namespace DecoratorSDK {

//################################################################################################
//
// CLASS : ReferenceBitmapPart
//
//################################################################################################

ReferenceBitmapPart::ReferenceBitmapPart(PartBase* pPart, CComPtr<IMgaCommonDecoratorEvents>& eventSink):
	TypeableBitmapPart	(pPart, eventSink),
	m_referencedPart	(NULL),
	m_bIconRequired		(false)
{
}

ReferenceBitmapPart::~ReferenceBitmapPart()
{
	if (m_referencedPart != NULL)
		delete m_referencedPart;
}

void ReferenceBitmapPart::Initialize(CComPtr<IMgaProject>& pProject, CComPtr<IMgaMetaPart>& pPart, CComPtr<IMgaFCO>& pFCO)
{
	if (m_referencedPart != NULL)
		m_referencedPart->Initialize(pProject, pPart, pFCO);
	TypeableBitmapPart::Initialize(pProject, pPart, pFCO);
}

void ReferenceBitmapPart::Destroy()
{
	if (m_referencedPart != NULL)
		m_referencedPart->Destroy();
	TypeableBitmapPart::Destroy();
}

CString ReferenceBitmapPart::GetMnemonic(void) const
{
	return TypeableBitmapPart::GetMnemonic();
}

feature_code ReferenceBitmapPart::GetFeatures(void) const
{
	feature_code featureCodes = 0;

	if (m_referencedPart != NULL) {
		feature_code referencedFeatureCodes = m_referencedPart->GetFeatures();
		featureCodes |= referencedFeatureCodes;
	}

	feature_code partFeatureCodes = TypeableBitmapPart::GetFeatures();
	featureCodes |= partFeatureCodes;

	return featureCodes;
}

void ReferenceBitmapPart::SetParam(const CString& strName, VARIANT vValue)
{
	if (m_referencedPart != NULL)
		m_referencedPart->SetParam(strName, vValue);
	TypeableBitmapPart::SetParam(strName, vValue);
}

bool ReferenceBitmapPart::GetParam(const CString& strName, VARIANT* pvValue)
{
	try {
		if (TypeableBitmapPart::GetParam(strName, pvValue))
			return true;
	}
	catch(UnknownParameterException&) {
	}
	if (m_referencedPart != NULL) {
		try {
			if (m_referencedPart->GetParam(strName, pvValue))
				return true;
		}
		catch(UnknownParameterException&) {
		}
	}
	return false;
}

void ReferenceBitmapPart::SetActive(bool bIsActive)
{
	if (m_referencedPart != NULL)
		m_referencedPart->SetActive(bIsActive);
	TypeableBitmapPart::SetActive(bIsActive);
}

CSize ReferenceBitmapPart::GetPreferredSize(void) const
{
	if (m_referencedPart != NULL) {
		CSize plSize = m_referencedPart->GetPreferredSize();
		return plSize + CSize(m_lBorderWidth * 2, m_lBorderWidth * 2);
	}

	return TypeableBitmapPart::GetPreferredSize();
}

void ReferenceBitmapPart::SetLocation(const CRect& location)
{
	if (m_referencedPart != NULL)
		m_referencedPart->SetLocation(location);
	TypeableBitmapPart::SetLocation(location);
}

CRect ReferenceBitmapPart::GetLocation(void) const
{
	return TypeableBitmapPart::GetLocation();
}

CRect ReferenceBitmapPart::GetLabelLocation(void) const
{
	CRect labelLocation;
	try {
		labelLocation = TypeableBitmapPart::GetLabelLocation();
	}
	catch(NotImplementedException&) {
		if (m_referencedPart != NULL)
			labelLocation = m_referencedPart->GetLabelLocation();
	}
	return labelLocation;
}

CRect ReferenceBitmapPart::GetPortLocation(CComPtr<IMgaFCO>& fco) const
{
	CRect portLocation(0,0,0,0);
	if (m_referencedPart != NULL) {
		try {
			portLocation = m_referencedPart->GetPortLocation(fco);
		}
		catch(PortNotFoundException&) {
		}
	}
	if (portLocation.IsRectEmpty()) {
		try {
			portLocation = TypeableBitmapPart::GetPortLocation(fco);
		}
		catch(PortNotFoundException&) {
		}
	}
	return portLocation;
}

bool ReferenceBitmapPart::GetPorts(CComPtr<IMgaFCOs>& portFCOs) const
{
	if (m_referencedPart != NULL) {
		try {
			if (m_referencedPart->GetPorts(portFCOs))
				return true;
		}
		catch(PortNotFoundException&) {
		}
	}
	try {
		if (TypeableBitmapPart::GetPorts(portFCOs))
			return true;
	}
	catch(PortNotFoundException&) {
	}
	return false;
}


void ReferenceBitmapPart::Draw(CDC* pDC, Gdiplus::Graphics* gdip)
{
	TypeableBitmapPart::Draw(pDC, gdip);
	if (m_referencedPart != NULL)
		m_referencedPart->Draw(pDC, gdip);
}

void ReferenceBitmapPart::SaveState()
{
	TypeableBitmapPart::SaveState();
	if (m_referencedPart != NULL)
		m_referencedPart->SaveState();
}

// New functions
void ReferenceBitmapPart::InitializeEx(CComPtr<IMgaProject>& pProject, CComPtr<IMgaMetaPart>& pPart, CComPtr<IMgaFCO>& pFCO,
									   HWND parentWnd, PreferenceMap& preferences)
{
	bool model_ref = false;
	PartBase* pReferenced = NULL;
	CComPtr<IMgaFCO> spReferenced;
	if (m_spFCO) {
		CComQIPtr<IMgaReference> spReference = m_spFCO;

		CComObjPtr<IMgaFCOs> visitedRefs;
		COMTHROW(visitedRefs.CoCreateInstance(L"Mga.MgaFCOs"));

		while (spReference) {
			spReferenced = NULL;

			long res;
			if ((visitedRefs->Find(spReference, 1L, &res)) == E_NOTFOUND) {
				COMTHROW(visitedRefs->Append(spReference));
				COMTHROW(spReference->get_Referred(&spReferenced));
			}

			spReference = spReferenced;
		}

		if (spReferenced) {
			objtype_enum eType;
			COMTHROW(spReferenced->get_ObjType(&eType));
			switch (eType) {
				case OBJTYPE_ATOM:
					pReferenced = new AtomBitmapPart(this, m_eventSink);
					break;
				case OBJTYPE_SET:
					pReferenced = new SetBitmapPart(this, m_eventSink);
					break;
				case OBJTYPE_MODEL:
					model_ref = true;
					pReferenced = new ModelComplexPart(this, m_eventSink);
					break;
			}
		}
	}

	preferences[PREF_ICONDEFAULT]	= PreferenceVariant(createResString(IDB_REFERENCE));
	preferences[PREF_TILESDEFAULT]	= PreferenceVariant(getFacilities().getTileVector(TILE_ATOMDEFAULT));
	preferences[PREF_TILESUNDEF]	= PreferenceVariant(getFacilities().getTileVector(TILE_ATOMDEFAULT));
	preferences[PREF_TYPEINFOSHOWN]	= PreferenceVariant(false);
	preferences[PREF_TYPESHOWN]		= PreferenceVariant(false);

	TypeableBitmapPart::InitializeEx(pProject, pPart, pFCO, parentWnd, preferences);	// TODO: is this needed?

	if (pReferenced) {
		m_referencedPart = pReferenced;
		PreferenceMap mapRefPrefs;

		if (m_bOverlay)
			mapRefPrefs[PREF_OVERLAYCOLOR] = PreferenceVariant(m_crOverlay);

		if (m_pBitmap->getName() != createResString(IDB_REFERENCE)) {
			mapRefPrefs[PREF_ICON] = PreferenceVariant(m_pBitmap->getName());
			m_lBorderWidth = 0;
			objtype_enum eType;
			COMTHROW(spReferenced->get_ObjType(&eType));
			mapRefPrefs[PREF_TILES] = PreferenceVariant(getFacilities().getTileVector((eType == OBJTYPE_MODEL) ? TILE_PORTDEFAULT : TILE_ATOMDEFAULT));
		} else {
			m_lBorderWidth = 2;
		}

		mapRefPrefs[PREF_TYPESHOWN] = PreferenceVariant(false);

		COLORREF crPortLabel;
		if (getFacilities().getPreference(m_spFCO, PREF_PORTLABELCOLOR, crPortLabel))
			mapRefPrefs[PREF_PORTLABELCOLOR] = PreferenceVariant(crPortLabel);

		if (getFacilities().getPreference(m_spFCO, PREF_BORDERCOLOR, m_crBorder))
			mapRefPrefs[PREF_BORDERCOLOR] = PreferenceVariant(m_crBorder);
		else
			getFacilities().getPreference(spReferenced, PREF_BORDERCOLOR, m_crBorder);

		if (m_spFCO && model_ref) {
			// these values will be get from the old PrefMap: preferences or from the m_spFCO's registry (most likely)
			bool port_label_inside = true;
			long port_label_length = MAX_PORT_LENGTH; // the default value in Preferences

			PreferenceMap::iterator it;
			it = preferences.find(PREF_PORTLABELINSIDE);
			if (it != preferences.end()) // the mapRefs map is not expected to contain the value
				port_label_inside = it->second.uValue.bValue;
			else // the object's registry may contain it
				getFacilities().getPreference(m_spFCO, PREF_PORTLABELINSIDE, port_label_inside);

			it = preferences.find(PREF_PORTLABELLENGTH);
			if (it != preferences.end())
				port_label_length = it->second.uValue.lValue;
			else // if not found in registry use MAX_PORT_LENGTH
				getFacilities().getPreference(m_spFCO, PREF_PORTLABELLENGTH, port_label_length);

			ASSERT(port_label_length >= 0 && port_label_length <= 1000);

			// store the values in the new map: mapRefPrefs
			mapRefPrefs[PREF_PORTLABELINSIDE] = PreferenceVariant(port_label_inside);
			mapRefPrefs[PREF_PORTLABELLENGTH] = PreferenceVariant((long) port_label_length);
		}
		mapRefPrefs[PREF_VIOLATED] = PreferenceVariant(false);

		pReferenced->InitializeEx(pProject, pPart, spReferenced, parentWnd, mapRefPrefs);
		pReferenced->SetReferenced(true);
	} else {
		m_lBorderWidth = 0;
		CString strIcon;
		if (m_spFCO) {
			getFacilities().getPreference(m_spFCO, PREF_NULLREFICON, strIcon);
		} else {
			CComPtr<IMgaMetaRole> spRole;
			COMTHROW(m_spPart->get_Role(&spRole));
			CComPtr<IMgaMetaFCO> spMetaFCO;
			COMTHROW(spRole->get_Kind(&spMetaFCO));
			getFacilities().getPreference(spMetaFCO, PREF_NULLREFICON, strIcon);
		}
		if (!strIcon.IsEmpty()) {
			BitmapBase* pBitmap = NULL;
#ifndef OLD_DECORATOR_LOOKANDFEEL
			if (m_bOverlay)
				pBitmap = getFacilities().getBitmapB(strIcon, m_crOverlay);
			else
				pBitmap = getFacilities().getBitmap( strIcon );
#else
			pBitmap = getFacilities().getBitmap(strIcon);
#endif
			if (pBitmap)
				m_pBitmap = pBitmap;
		}
	}

	// m_bIconRequired = m_spFCO && pReferenced && !m_pBitmap->hasBackgroundColor() && m_pBitmap->getName() == createResString(IDB_REFERENCE);
	m_bIconRequired = true;
}

void ReferenceBitmapPart::SetSelected(bool bIsSelected)
{
	if (m_referencedPart != NULL)
		m_referencedPart->SetSelected(bIsSelected);
	TypeableBitmapPart::SetSelected(bIsSelected);
}

bool ReferenceBitmapPart::MouseMoved(UINT nFlags, const CPoint& point, HDC transformHDC)
{
	HRESULT retVal = S_OK;
	try {
		if (TypeableBitmapPart::MouseMoved(nFlags, point, transformHDC))
			return true;
	}
	catch(hresult_exception& e) {
		retVal = e.hr;
	}
	catch(DecoratorException& e) {
		retVal = e.GetHResult();
	}
	if (m_referencedPart != NULL && (retVal == S_OK || retVal == S_DECORATOR_EVENT_NOT_HANDLED || retVal == E_DECORATOR_NOT_IMPLEMENTED))
		if (m_referencedPart->MouseMoved(nFlags, point, transformHDC))
			return true;

	return false;
}

bool ReferenceBitmapPart::MouseLeftButtonDown(UINT nFlags, const CPoint& point, HDC transformHDC)
{
	HRESULT retVal = S_OK;
	try {
		if (TypeableBitmapPart::MouseLeftButtonDown(nFlags, point, transformHDC))
			return true;
	}
	catch(hresult_exception& e) {
		retVal = e.hr;
	}
	catch(DecoratorException& e) {
		retVal = e.GetHResult();
	}
	if (m_referencedPart != NULL && (retVal == S_OK || retVal == S_DECORATOR_EVENT_NOT_HANDLED || retVal == E_DECORATOR_NOT_IMPLEMENTED))
		if (m_referencedPart->MouseLeftButtonDown(nFlags, point, transformHDC))
			return true;

	return false;
}

bool ReferenceBitmapPart::MouseLeftButtonUp(UINT nFlags, const CPoint& point, HDC transformHDC)
{
	HRESULT retVal = S_OK;
	try {
		if (TypeableBitmapPart::MouseLeftButtonUp(nFlags, point, transformHDC))
			return true;
	}
	catch(hresult_exception& e) {
		retVal = e.hr;
	}
	catch(DecoratorException& e) {
		retVal = e.GetHResult();
	}
	if (m_referencedPart != NULL && (retVal == S_OK || retVal == S_DECORATOR_EVENT_NOT_HANDLED || retVal == E_DECORATOR_NOT_IMPLEMENTED))
		if (m_referencedPart->MouseLeftButtonUp(nFlags, point, transformHDC))
			return true;

	return false;
}

bool ReferenceBitmapPart::MouseLeftButtonDoubleClick(UINT nFlags, const CPoint& point, HDC transformHDC)
{
	HRESULT retVal = S_OK;
	try {
		if (TypeableBitmapPart::MouseLeftButtonDoubleClick(nFlags, point, transformHDC))
			return true;
	}
	catch(hresult_exception& e) {
		retVal = e.hr;
	}
	catch(DecoratorException& e) {
		retVal = e.GetHResult();
	}
	if (m_referencedPart != NULL && (retVal == S_OK || retVal == S_DECORATOR_EVENT_NOT_HANDLED || retVal == E_DECORATOR_NOT_IMPLEMENTED))
		if (m_referencedPart->MouseLeftButtonDoubleClick(nFlags, point, transformHDC))
			return true;

	return false;
}

bool ReferenceBitmapPart::MouseRightButtonDown(HMENU hCtxMenu, UINT nFlags, const CPoint& point, HDC transformHDC)
{
	HRESULT retVal = S_OK;
	try {
		if (TypeableBitmapPart::MouseRightButtonDown(hCtxMenu, nFlags, point, transformHDC))
			return true;
	}
	catch(hresult_exception& e) {
		retVal = e.hr;
	}
	catch(DecoratorException& e) {
		retVal = e.GetHResult();
	}
	if (m_referencedPart != NULL && (retVal == S_OK || retVal == S_DECORATOR_EVENT_NOT_HANDLED || retVal == E_DECORATOR_NOT_IMPLEMENTED))
		if (m_referencedPart->MouseRightButtonDown(hCtxMenu, nFlags, point, transformHDC))
			return true;

	return false;
}

bool ReferenceBitmapPart::MouseRightButtonUp(UINT nFlags, const CPoint& point, HDC transformHDC)
{
	HRESULT retVal = S_OK;
	try {
		if (TypeableBitmapPart::MouseRightButtonUp(nFlags, point, transformHDC))
			return true;
	}
	catch(hresult_exception& e) {
		retVal = e.hr;
	}
	catch(DecoratorException& e) {
		retVal = e.GetHResult();
	}
	if (m_referencedPart != NULL && (retVal == S_OK || retVal == S_DECORATOR_EVENT_NOT_HANDLED || retVal == E_DECORATOR_NOT_IMPLEMENTED))
		if (m_referencedPart->MouseRightButtonUp(nFlags, point, transformHDC))
			return true;

	return false;
}

bool ReferenceBitmapPart::MouseRightButtonDoubleClick(UINT nFlags, const CPoint& point, HDC transformHDC)
{
	HRESULT retVal = S_OK;
	try {
		if (TypeableBitmapPart::MouseRightButtonDoubleClick(nFlags, point, transformHDC))
			return true;
	}
	catch(hresult_exception& e) {
		retVal = e.hr;
	}
	catch(DecoratorException& e) {
		retVal = e.GetHResult();
	}
	if (m_referencedPart != NULL && (retVal == S_OK || retVal == S_DECORATOR_EVENT_NOT_HANDLED || retVal == E_DECORATOR_NOT_IMPLEMENTED))
		if (m_referencedPart->MouseRightButtonDoubleClick(nFlags, point, transformHDC))
			return true;

	return false;
}

bool ReferenceBitmapPart::MouseMiddleButtonDown(UINT nFlags, const CPoint& point, HDC transformHDC)
{
	HRESULT retVal = S_OK;
	try {
		if (TypeableBitmapPart::MouseMiddleButtonDown(nFlags, point, transformHDC))
			return true;
	}
	catch(hresult_exception& e) {
		retVal = e.hr;
	}
	catch(DecoratorException& e) {
		retVal = e.GetHResult();
	}
	if (m_referencedPart != NULL && (retVal == S_OK || retVal == S_DECORATOR_EVENT_NOT_HANDLED || retVal == E_DECORATOR_NOT_IMPLEMENTED))
		if (m_referencedPart->MouseMiddleButtonDown(nFlags, point, transformHDC))
			return true;

	return false;
}

bool ReferenceBitmapPart::MouseMiddleButtonUp(UINT nFlags, const CPoint& point, HDC transformHDC)
{
	HRESULT retVal = S_OK;
	try {
		if (TypeableBitmapPart::MouseMiddleButtonUp(nFlags, point, transformHDC))
			return true;
	}
	catch(hresult_exception& e) {
		retVal = e.hr;
	}
	catch(DecoratorException& e) {
		retVal = e.GetHResult();
	}
	if (m_referencedPart != NULL && (retVal == S_OK || retVal == S_DECORATOR_EVENT_NOT_HANDLED || retVal == E_DECORATOR_NOT_IMPLEMENTED))
		if (m_referencedPart->MouseMiddleButtonUp(nFlags, point, transformHDC))
			return true;

	return false;
}

bool ReferenceBitmapPart::MouseMiddleButtonDoubleClick(UINT nFlags, const CPoint& point, HDC transformHDC)
{
	HRESULT retVal = S_OK;
	try {
		if (TypeableBitmapPart::MouseMiddleButtonDoubleClick(nFlags, point, transformHDC))
			return true;
	}
	catch(hresult_exception& e) {
		retVal = e.hr;
	}
	catch(DecoratorException& e) {
		retVal = e.GetHResult();
	}
	if (m_referencedPart != NULL && (retVal == S_OK || retVal == S_DECORATOR_EVENT_NOT_HANDLED || retVal == E_DECORATOR_NOT_IMPLEMENTED))
		if (m_referencedPart->MouseMiddleButtonDoubleClick(nFlags, point, transformHDC))
			return true;

	return false;
}

bool ReferenceBitmapPart::MouseWheelTurned(UINT nFlags, short distance, const CPoint& point, HDC transformHDC)
{
	HRESULT retVal = S_OK;
	try {
		if (TypeableBitmapPart::MouseWheelTurned(nFlags, distance, point, transformHDC))
			return true;
	}
	catch(hresult_exception& e) {
		retVal = e.hr;
	}
	catch(DecoratorException& e) {
		retVal = e.GetHResult();
	}
	if (m_referencedPart != NULL && (retVal == S_OK || retVal == S_DECORATOR_EVENT_NOT_HANDLED || retVal == E_DECORATOR_NOT_IMPLEMENTED))
		if (m_referencedPart->MouseWheelTurned(nFlags, distance, point, transformHDC))
			return true;

	return false;
}

bool ReferenceBitmapPart::DragEnter(DROPEFFECT* dropEffect, COleDataObject* pDataObject, DWORD dwKeyState, const CPoint& point, HDC transformHDC)
{
	HRESULT retVal = S_OK;
	try {
		if (TypeableBitmapPart::DragEnter(dropEffect, pDataObject, dwKeyState, point, transformHDC))
			return true;
	}
	catch(hresult_exception& e) {
		retVal = e.hr;
	}
	catch(DecoratorException& e) {
		retVal = e.GetHResult();
	}
	if (m_referencedPart != NULL && (retVal == S_OK || retVal == S_DECORATOR_EVENT_NOT_HANDLED || retVal == E_DECORATOR_NOT_IMPLEMENTED))
		if (m_referencedPart->DragEnter(dropEffect, pDataObject, dwKeyState, point, transformHDC))
			return true;

	return false;
}

bool ReferenceBitmapPart::DragOver(DROPEFFECT* dropEffect, COleDataObject* pDataObject, DWORD dwKeyState, const CPoint& point, HDC transformHDC)
{
	HRESULT retVal = S_OK;
	try {
		if (TypeableBitmapPart::DragOver(dropEffect, pDataObject, dwKeyState, point, transformHDC))
			return true;
	}
	catch(hresult_exception& e) {
		retVal = e.hr;
	}
	catch(DecoratorException& e) {
		retVal = e.GetHResult();
	}
	if (m_referencedPart != NULL && (retVal == S_OK || retVal == S_DECORATOR_EVENT_NOT_HANDLED || retVal == E_DECORATOR_NOT_IMPLEMENTED))
		if (m_referencedPart->DragOver(dropEffect, pDataObject, dwKeyState, point, transformHDC))
			return true;

	return false;
}

bool ReferenceBitmapPart::Drop(COleDataObject* pDataObject, DROPEFFECT dropEffect, const CPoint& point, HDC transformHDC)
{
	HRESULT retVal = S_OK;
	try {
		if (TypeableBitmapPart::Drop(pDataObject, dropEffect, point, transformHDC))
			return true;
	}
	catch(hresult_exception& e) {
		retVal = e.hr;
	}
	catch(DecoratorException& e) {
		retVal = e.GetHResult();
	}
	if (m_referencedPart != NULL && (retVal == S_OK || retVal == S_DECORATOR_EVENT_NOT_HANDLED || retVal == E_DECORATOR_NOT_IMPLEMENTED))
		if (m_referencedPart->Drop(pDataObject, dropEffect, point, transformHDC))
			return true;

	return false;
}

bool ReferenceBitmapPart::DropFile(HDROP p_hDropInfo, const CPoint& point, HDC transformHDC)
{
	HRESULT retVal = S_OK;
	try {
		if (TypeableBitmapPart::DropFile(p_hDropInfo, point, transformHDC))
			return true;
	}
	catch(hresult_exception& e) {
		retVal = e.hr;
	}
	catch(DecoratorException& e) {
		retVal = e.GetHResult();
	}
	if (m_referencedPart != NULL && (retVal == S_OK || retVal == S_DECORATOR_EVENT_NOT_HANDLED || retVal == E_DECORATOR_NOT_IMPLEMENTED))
		if (m_referencedPart->DropFile(p_hDropInfo, point, transformHDC))
			return true;

	return false;
}

bool ReferenceBitmapPart::MenuItemSelected(UINT menuItemId, UINT nFlags, const CPoint& point, HDC transformHDC)
{
	HRESULT retVal = S_OK;
	try {
		if (TypeableBitmapPart::MenuItemSelected(menuItemId, nFlags, point, transformHDC))
			return true;
	}
	catch(hresult_exception& e) {
		retVal = e.hr;
	}
	catch(DecoratorException& e) {
		retVal = e.GetHResult();
	}
	if (m_referencedPart != NULL && (retVal == S_OK || retVal == S_DECORATOR_EVENT_NOT_HANDLED || retVal == E_DECORATOR_NOT_IMPLEMENTED))
		if (m_referencedPart->MenuItemSelected(menuItemId, nFlags, point, transformHDC))
			return true;

	return false;
}

bool ReferenceBitmapPart::OperationCanceledByGME(void)
{
	HRESULT retVal = S_OK;
	bool handled = false;
	try {
		handled = TypeableBitmapPart::OperationCanceledByGME();
	}
	catch(hresult_exception& e) {
		retVal = e.hr;
	}
	catch(DecoratorException& e) {
		retVal = e.GetHResult();
	}
	if (!handled && m_referencedPart != NULL && (retVal == S_OK || retVal == S_DECORATOR_EVENT_NOT_HANDLED || retVal == E_DECORATOR_NOT_IMPLEMENTED))
		handled = m_referencedPart->OperationCanceledByGME();

	return handled;
}

void ReferenceBitmapPart::DrawBackground(CDC* pDC, Gdiplus::Graphics* gdip)
{
	BitmapPart* bitmapPart = NULL;
	if (m_referencedPart != NULL)
		bitmapPart = m_referencedPart->dynamic_cast_BitmapPart();
	if (bitmapPart) {
		bitmapPart->DrawBackground(pDC, gdip);
	} else {
#ifndef OLD_DECORATOR_LOOKANDFEEL
		TypeableBitmapPart::DrawBackground(pDC, gdip);
#else
		if (m_bActive) {
			TypeableBitmapPart::DrawBackground(pDC, gdip);
		} else {
			CRect cRect = TypeableBitmapPart::GetBoxLocation(false);
			int iDepth = 2;
			getFacilities().DrawBox(gdip, cRect, COLOR_LIGHTGRAY, iDepth);
			getFacilities().DrawRect(gdip, cRect, COLOR_GRAY);
			cRect.DeflateRect(iDepth, iDepth);
			getFacilities().DrawRect(gdip, cRect, COLOR_GRAY);
		}
#endif
	}
}

void ReferenceBitmapPart::DrawIcons(CDC* pDC, Gdiplus::Graphics* gdip)
{
	BitmapPart* bitmapPart = NULL;
	if (m_referencedPart != NULL)
		bitmapPart = m_referencedPart->dynamic_cast_BitmapPart();
	if (bitmapPart)
		bitmapPart->DrawIcons(pDC, gdip);
	else
		TypeableBitmapPart::DrawIcons(pDC, gdip);

	if (/*false*/ m_spFCO && m_bIconRequired) {
		BitmapBase* pBitmap = getFacilities().getBitmap(createResString((m_bActive) ? IDB_ICON_REFERENCE : IDB_ICON_REFERENCE_DIS));
		CRect cRect = TypeableBitmapPart::GetBoxLocation(false);
		cRect.left += GAP_LABEL;
		cRect.top += GAP_LABEL;
		cRect.right = cRect.left + pBitmap->getWidth();
		cRect.bottom = cRect.top + pBitmap->getHeight();
		pBitmap->draw(gdip, pDC, cRect, TileVector(1, BackgroundTile()));
	}
}

void ReferenceBitmapPart::SetBoxLocation(const CRect& cRect)
{
	TypeableBitmapPart::SetBoxLocation(cRect);
	if (m_referencedPart != NULL) {
		CRect cRect2 = m_Rect;
		cRect2.DeflateRect(m_lBorderWidth, m_lBorderWidth);
		m_referencedPart->SetBoxLocation(cRect2);
	}
}

void ReferenceBitmapPart::SetReferenced(bool referenced)
{
	TypeableBitmapPart::SetReferenced(referenced);
	if (m_referencedPart != NULL)
		m_referencedPart->SetReferenced(referenced);
}

void ReferenceBitmapPart::SetParentPart(PartBase* pPart)
{
	TypeableBitmapPart::SetParentPart(pPart);
	if (m_referencedPart != NULL)
		m_referencedPart->SetParentPart(pPart);
}

PartBase* ReferenceBitmapPart::GetReferenced(void) const
{
	return m_referencedPart;
}

}; // namespace DecoratorSDK
