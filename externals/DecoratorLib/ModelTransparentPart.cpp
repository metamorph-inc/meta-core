//################################################################################################
//
// Model transparent part class (decorator part)
//	ModelTransparentPart.cpp
//
//################################################################################################

#include "StdAfx.h"
#include "ModelTransparentPart.h"
#include "DecoratorExceptions.h"
#include <algorithm>


namespace DecoratorSDK {

//################################################################################################
//
// CLASS : ModelTransparentPart
//
//################################################################################################

ModelTransparentPart::ModelTransparentPart(PartBase* pPart, CComPtr<IMgaCommonDecoratorEvents>& eventSink):
	TypeableBitmapPart			(pPart, eventSink)
{
}

ModelTransparentPart::~ModelTransparentPart()
{
	// TODO: delete things
}

void ModelTransparentPart::Initialize(CComPtr<IMgaProject>& pProject, CComPtr<IMgaMetaPart>& pPart, CComPtr<IMgaFCO>& pFCO)
{
	TypeableBitmapPart::Initialize(pProject, pPart, pFCO);
}

void ModelTransparentPart::Destroy()
{
	// TODO: forward message?
	// TODO: destroy FCOs and everything
	TypeableBitmapPart::Destroy();
}

CString ModelTransparentPart::GetMnemonic(void) const
{
	return TypeableBitmapPart::GetMnemonic();
}

feature_code ModelTransparentPart::GetFeatures(void) const
{
	feature_code featureCodes = 0;

	feature_code partFeatureCodes = TypeableBitmapPart::GetFeatures();
	featureCodes |= partFeatureCodes;

	return featureCodes;
}

void ModelTransparentPart::SetParam(const CString& strName, VARIANT vValue)
{
	TypeableBitmapPart::SetParam(strName, vValue);
}

bool ModelTransparentPart::GetParam(const CString& strName, VARIANT* pvValue)
{
	try {
		if (TypeableBitmapPart::GetParam(strName, pvValue))
			return true;
	}
	catch(UnknownParameterException&) {
	}
	return false;
}

void ModelTransparentPart::SetActive(bool bIsActive)
{
	TypeableBitmapPart::SetActive(bIsActive);
}

CSize ModelTransparentPart::GetPreferredSize(void) const
{
	CSize size = ResizablePart::GetPreferredSize();
	if (size.cx * size.cy != 0)
		return size;

	if (!m_pBitmap || m_pBitmap->getName() == createResString(IDB_MODEL)) {
		return CSize(WIDTH_MODEL, HEIGHT_MODEL);
	} else {
		return TypeableBitmapPart::GetPreferredSize();
	}

	// TODO: give estimate for needed expanded model size
}

void ModelTransparentPart::SetLocation(const CRect& location)
{
	TypeableBitmapPart::SetLocation(location);
}

CRect ModelTransparentPart::GetLocation(void) const
{
	return TypeableBitmapPart::GetLocation();
}

CRect ModelTransparentPart::GetLabelLocation(void) const
{
	CRect labelLocation(0,0,0,0);
	try {
		labelLocation = TypeableBitmapPart::GetLabelLocation();
	}
	catch(NotImplementedException&) {
	}
	return labelLocation;
}

CRect ModelTransparentPart::GetPortLocation(CComPtr<IMgaFCO>& pFCO) const
{
	CRect portLocation(0,0,0,0);
	// TODO: for AutoRouter this is needed
	// return the element's location
	return portLocation;
}

bool ModelTransparentPart::GetPorts(CComPtr<IMgaFCOs>& portFCOs) const
{
	CComPtr<IMgaFCOs> spFCOs;
	COMTHROW(spFCOs.CoCreateInstance(OLESTR("Mga.MgaFCOs")));

	// TODO: enumerate the displayed elements

	portFCOs = spFCOs.Detach();
	return true;
}

void ModelTransparentPart::Draw(CDC* pDC, Gdiplus::Graphics* gdip)
{
	TypeableBitmapPart::Draw(pDC, gdip);
}

void ModelTransparentPart::SaveState()
{
	// TODO
	TypeableBitmapPart::SaveState();
}

// New functions
void ModelTransparentPart::InitializeEx(CComPtr<IMgaProject>& pProject, CComPtr<IMgaMetaPart>& pPart, CComPtr<IMgaFCO>& pFCO,
									HWND parentWnd, PreferenceMap& preferences)
{
	// TODO
	preferences[PREF_ICONDEFAULT]	= PreferenceVariant(createResString(IDB_MODEL));
	preferences[PREF_TILESDEFAULT]	= PreferenceVariant(getFacilities().getTileVector(TILE_MODELDEFAULT));
	preferences[PREF_TILESUNDEF]	= PreferenceVariant(getFacilities().getTileVector(TILE_PORTDEFAULT));
	TypeableBitmapPart::InitializeEx(pProject, pPart, pFCO, parentWnd, preferences);
}

void ModelTransparentPart::SetSelected(bool bIsSelected)
{
	// TODO
	TypeableBitmapPart::SetSelected(bIsSelected);
}

bool ModelTransparentPart::MouseMoved(UINT nFlags, const CPoint& point, HDC transformHDC)
{
	// TODO: relay events
	return false;
}

bool ModelTransparentPart::MouseLeftButtonDown(UINT nFlags, const CPoint& point, HDC transformHDC)
{
	// TODO: relay events
	return false;
}

bool ModelTransparentPart::MouseLeftButtonUp(UINT nFlags, const CPoint& point, HDC transformHDC)
{
	// TODO: relay events
	return false;
}

bool ModelTransparentPart::MouseLeftButtonDoubleClick(UINT nFlags, const CPoint& point, HDC transformHDC)
{
	// TODO: relay events
	return false;
}

bool ModelTransparentPart::MouseRightButtonDown(HMENU hCtxMenu, UINT nFlags, const CPoint& point, HDC transformHDC)
{
	// TODO: relay events
	return false;
}

bool ModelTransparentPart::MouseRightButtonUp(UINT nFlags, const CPoint& point, HDC transformHDC)
{
	// TODO: relay events
	return false;
}

bool ModelTransparentPart::MouseRightButtonDoubleClick(UINT nFlags, const CPoint& point, HDC transformHDC)
{
	// TODO: relay events
	return false;
}

bool ModelTransparentPart::MouseMiddleButtonDown(UINT nFlags, const CPoint& point, HDC transformHDC)
{
	// TODO: relay events
	return false;
}

bool ModelTransparentPart::MouseMiddleButtonUp(UINT nFlags, const CPoint& point, HDC transformHDC)
{
	// TODO: relay events
	return false;
}

bool ModelTransparentPart::MouseMiddleButtonDoubleClick(UINT nFlags, const CPoint& point, HDC transformHDC)
{
	// TODO: relay events
	return false;
}

bool ModelTransparentPart::MouseWheelTurned(UINT nFlags, short distance, const CPoint& point, HDC transformHDC)
{
	// TODO: relay events
	return false;
}

bool ModelTransparentPart::DragEnter(DROPEFFECT* dropEffect, COleDataObject* pDataObject, DWORD dwKeyState, const CPoint& point, HDC transformHDC)
{
	// TODO: relay events
	return false;
}

bool ModelTransparentPart::DragOver(DROPEFFECT* dropEffect, COleDataObject* pDataObject, DWORD dwKeyState, const CPoint& point, HDC transformHDC)
{
	// TODO: relay events
	return false;
}

bool ModelTransparentPart::Drop(COleDataObject* pDataObject, DROPEFFECT dropEffect, const CPoint& point, HDC transformHDC)
{
	// TODO: relay events
	return false;
}

bool ModelTransparentPart::DropFile(HDROP p_hDropInfo, const CPoint& point, HDC transformHDC)
{
	// TODO: relay events
	return false;
}

bool ModelTransparentPart::MenuItemSelected(UINT menuItemId, UINT nFlags, const CPoint& point, HDC transformHDC)
{
	// TODO: relay events
	return false;
}

bool ModelTransparentPart::OperationCanceledByGME(void)
{
	// TODO: relay events
	return false;
}

void ModelTransparentPart::DrawBackground(CDC* pDC, Gdiplus::Graphics* gdip)
{
	TypeableBitmapPart::DrawBackground(pDC, gdip);
	// TODO
}

void ModelTransparentPart::SetBoxLocation(const CRect& cRect)
{
	TypeableBitmapPart::SetBoxLocation(cRect);
	// TODO
}

void ModelTransparentPart::SetReferenced(bool referenced)
{
	// TODO: relay
	TypeableBitmapPart::SetReferenced(referenced);
}

void ModelTransparentPart::SetParentPart(PartBase* pPart)
{
	// TODO: relay
	TypeableBitmapPart::SetParentPart(pPart);
}

}; // namespace DecoratorSDK
