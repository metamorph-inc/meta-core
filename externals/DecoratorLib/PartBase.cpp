//################################################################################################
//
// Decorator parts base class
//	PartBase.cpp
//
//################################################################################################

#include "StdAfx.h"
#include "PartBase.h"
#include "DecoratorExceptions.h"


namespace DecoratorSDK {

//################################################################################################
//
// CLASS : PartBase
//
//################################################################################################

PartBase::PartBase(PartBase* pPart, CComPtr<IMgaCommonDecoratorEvents>& eventSink):
	m_parentPart	(pPart),
	m_eventSink		(eventSink),
	m_parentWnd		(NULL),
	m_viewDC		(NULL),
	m_bHasViolation	(false),
	m_bActive		(true),
	m_bSelected		(false),
	m_lBorderWidth	(0),
	m_bReferenced	(false),
	m_bResizable	(false)
{
	m_Rect.SetRectEmpty();
}

PartBase::~PartBase()
{
	// TODO: anything to destroy?
}

void PartBase::Initialize(CComPtr<IMgaProject>& pProject, CComPtr<IMgaMetaPart>& pPart, CComPtr<IMgaFCO>& pFCO)
{
	m_spProject	= pProject;
	m_spPart	= pPart;
	m_spFCO		= pFCO;

	if (m_spFCO) {
		COMTHROW(m_spFCO->get_ObjType(&m_eType));
	} else if (m_spPart) {
		CComPtr<IMgaMetaRole> spRole;
		COMTHROW(m_spPart->get_Role(&spRole));

		CComPtr<IMgaMetaFCO> spMetaFCO;
		COMTHROW(spRole->get_Kind(&spMetaFCO));

		COMTHROW(spMetaFCO->get_ObjType(&m_eType));
		m_spMetaFCO = spMetaFCO;
	}

	m_bHasViolation	= false;
	m_bActive		= true;
	m_bSelected		= false;
	m_lBorderWidth	= 0;
	m_bReferenced	= false;
}

void PartBase::Destroy(void)
{
}

CString PartBase::GetMnemonic(void) const
{
	return CString();
}

feature_code PartBase::GetFeatures(void) const
{
	return 0;
}

void PartBase::SetParam(const CString& strName, VARIANT vValue)
{
}

bool PartBase::GetParam(const CString& strName, VARIANT* pvValue)
{
	return false;
}

void PartBase::SetActive(bool bIsActive)
{
	m_bActive = bIsActive;
}

CSize PartBase::GetPreferredSize(void) const
{
	return CSize(0, 0);	// empty
}

void PartBase::SetLocation(const CRect& location)
{
	SetBoxLocation(location);
}

CRect PartBase::GetLocation(void) const
{
	return GetBoxLocation(true);
}

CRect PartBase::GetLabelLocation(void) const
{
	return CRect(0,0,0,0);	// empty
}

CRect PartBase::GetPortLocation(CComPtr<IMgaFCO>& pFCO) const
{
	return CRect(0,0,0,0);	// empty
}

bool PartBase::GetPorts(CComPtr<IMgaFCOs>& portFCOs) const
{
	return false;
}

void PartBase::Draw(CDC* pDC, Gdiplus::Graphics* gdip)
{
	// empty default implementation
}

void PartBase::SaveState(void)
{
	// empty default implementation
}

void PartBase::InitializeEx(CComPtr<IMgaProject>& pProject, CComPtr<IMgaMetaPart>& pPart, CComPtr<IMgaFCO>& pFCO,
							HWND parentWnd, PreferenceMap& preferences)
{
	m_parentWnd = parentWnd;
	Initialize(pProject, pPart, pFCO);

	// HasViolations
	if (m_spFCO) {
		PreferenceMap::iterator it = preferences.find(PREF_VIOLATED);
		if (it != preferences.end())
			m_bHasViolation = it->second.uValue.bValue;
		else
			m_bHasViolation = getFacilities().getPreferenceStatus(m_spFCO, PREF_VIOLATED) == PS_HERE;

		it = preferences.find(PREF_ITEMRESIZABLE);
		if (it != preferences.end())
			m_bResizable = it->second.uValue.bValue;
		else
			getFacilities().getPreference(m_spFCO, PREF_ITEMRESIZABLE, m_bResizable);
	} else {
		PreferenceMap::iterator it = preferences.find(PREF_ITEMSHADOWCAST);
		if (it == preferences.end())
			preferences[PREF_ITEMSHADOWCAST] = PreferenceVariant(false);
		it = preferences.find(PREF_ITEMRESIZABLE);
		if (it == preferences.end())
			preferences[PREF_ITEMRESIZABLE] = PreferenceVariant(false);
	}

	m_lBorderWidth = 0;
}

void PartBase::SetSelected(bool bIsSelected)
{
	m_bSelected = bIsSelected;
}

bool PartBase::MouseMoved(UINT nFlags, const CPoint& point, HDC transformHDC)
{
	return false;
}

bool PartBase::MouseLeftButtonDown(UINT nFlags, const CPoint& point, HDC transformHDC)
{
	return false;
}

bool PartBase::MouseLeftButtonUp(UINT nFlags, const CPoint& point, HDC transformHDC)
{
	return false;
}

bool PartBase::MouseLeftButtonDoubleClick(UINT nFlags, const CPoint& point, HDC transformHDC)
{
	return false;
}

bool PartBase::MouseRightButtonDown(HMENU hCtxMenu, UINT nFlags, const CPoint& point, HDC transformHDC)
{
	return false;
}

bool PartBase::MouseRightButtonUp(UINT nFlags, const CPoint& point, HDC transformHDC)
{
	return false;
}

bool PartBase::MouseRightButtonDoubleClick(UINT nFlags, const CPoint& point, HDC transformHDC)
{
	return false;
}

bool PartBase::MouseMiddleButtonDown(UINT nFlags, const CPoint& point, HDC transformHDC)
{
	return false;
}

bool PartBase::MouseMiddleButtonUp(UINT nFlags, const CPoint& point, HDC transformHDC)
{
	return false;
}

bool PartBase::MouseMiddleButtonDoubleClick(UINT nFlags, const CPoint& point, HDC transformHDC)
{
	return false;
}

bool PartBase::MouseWheelTurned(UINT nFlags, short distance, const CPoint& point, HDC transformHDC)
{
	return false;
}

bool PartBase::DragEnter(DROPEFFECT* dropEffect, COleDataObject* pDataObject, DWORD dwKeyState, const CPoint& point, HDC transformHDC)
{
	dropEffect = DROPEFFECT_NONE;
	return false;
}

bool PartBase::DragOver(DROPEFFECT* dropEffect, COleDataObject* pDataObject, DWORD dwKeyState, const CPoint& point, HDC transformHDC)
{
	dropEffect = DROPEFFECT_NONE;
	return false;
}

bool PartBase::Drop(COleDataObject* pDataObject, DROPEFFECT dropEffect, const CPoint& point, HDC transformHDC)
{
	return false;
}

bool PartBase::DropFile(HDROP p_hDropInfo, const CPoint& point, HDC transformHDC)
{
	return false;
}

bool PartBase::MenuItemSelected(UINT menuItemId, UINT nFlags, const CPoint& point, HDC transformHDC)
{
	return false;
}

bool PartBase::OperationCanceledByGME(void)
{
	return false;
}

void PartBase::Refresh(refresh_mode_enum refreshMode)
{
	if (m_eventSink)
		m_eventSink->Refresh(refreshMode);
}

void PartBase::CursorChanged(long newCursorID)
{
	if (m_eventSink)
		m_eventSink->CursorChanged(newCursorID);
}

void PartBase::CursorRestored(void)
{
	if (m_eventSink)
		m_eventSink->CursorRestored();
}

void PartBase::OperationCanceledByDecorator(void)
{
	if (m_eventSink)
		m_eventSink->OperationCanceled();
}

void PartBase::LabelEditingStarted(CRect& location)
{
	if (m_eventSink)
		m_eventSink->LabelEditingStarted(location.left, location.top, location.right, location.bottom);
}

void PartBase::LabelEditingFinished(CRect& location)
{
	if (m_eventSink)
		m_eventSink->LabelEditingFinished(location.left, location.top, location.right, location.bottom);
}

void PartBase::LabelChanged(CString& newLabel)
{
	if (m_eventSink) {
		CComBSTR newLabelBStr(newLabel);
		m_eventSink->LabelChanged(newLabelBStr);
	}
}

void PartBase::LabelMovingStarted(UINT nFlags, CRect& location)
{
	if (m_eventSink)
		m_eventSink->LabelMovingStarted(nFlags, location.left, location.top, location.right, location.bottom);
}

void PartBase::LabelMoving(UINT nFlags, CRect& location)
{
	if (m_eventSink)
		m_eventSink->LabelMoving(nFlags, location.left, location.top, location.right, location.bottom);
}

void PartBase::LabelMovingFinished(UINT nFlags, CRect& location)
{
	if (m_eventSink)
		m_eventSink->LabelMovingFinished(nFlags, location.left, location.top, location.right, location.bottom);
}

void PartBase::LabelMoved(UINT nFlags, CPoint& point)
{
	if (m_eventSink)
		m_eventSink->LabelMoved(nFlags, point.x, point.y);
}

void PartBase::LabelResizingStarted(UINT nFlags, CRect& location)
{
	if (m_eventSink)
		m_eventSink->LabelResizingStarted(nFlags, location.left, location.top, location.right, location.bottom);
}

void PartBase::LabelResizing(UINT nFlags, CRect& location)
{
	if (m_eventSink)
		m_eventSink->LabelResizing(nFlags, location.left, location.top, location.right, location.bottom);
}

void PartBase::LabelResizingFinished(UINT nFlags, CRect& location)
{
	if (m_eventSink)
		m_eventSink->LabelResizingFinished(nFlags, location.left, location.top, location.right, location.bottom);
}

void PartBase::LabelResized(UINT nFlags, CSize& size)
{
	if (m_eventSink)
		m_eventSink->LabelResized(nFlags, size.cx, size.cy);
}

void PartBase::GeneralOperationStarted(void* operationData)
{
	if (m_eventSink)
		m_eventSink->GeneralOperationStarted((ULONGLONG)operationData);
}

void PartBase::GeneralOperationFinished(void** operationData)
{
	if (m_eventSink)
		m_eventSink->GeneralOperationFinished((ULONGLONG*)operationData);
}

void PartBase::WindowMovingStarted(UINT nFlags, CRect& location)
{
	if (m_eventSink) {
		CComQIPtr<IMgaElementDecoratorEvents> edes(m_eventSink);
		if (edes)
			edes->WindowMovingStarted(nFlags, location.left, location.top, location.right, location.bottom);
		else
			ASSERT(false);
	}
}

void PartBase::WindowMoving(UINT nFlags, CRect& location)
{
	if (m_eventSink) {
		CComQIPtr<IMgaElementDecoratorEvents> edes(m_eventSink);
		if (edes)
			edes->WindowMoving(nFlags, location.left, location.top, location.right, location.bottom);
		else
			ASSERT(false);
	}
}

void PartBase::WindowMovingFinished(UINT nFlags, CRect& location)
{
	if (m_eventSink) {
		CComQIPtr<IMgaElementDecoratorEvents> edes(m_eventSink);
		if (edes)
			edes->WindowMovingFinished(nFlags, location.left, location.top, location.right, location.bottom);
		else
			ASSERT(false);
	}
}

void PartBase::WindowMoved(UINT nFlags, CPoint& point)
{
	if (m_eventSink) {
		CComQIPtr<IMgaElementDecoratorEvents> edes(m_eventSink);
		if (edes)
			edes->WindowMoved(nFlags, point.x, point.y);
		else
			ASSERT(false);
	}
}

void PartBase::WindowResizingStarted(UINT nFlags, CRect& location)
{
	if (m_eventSink) {
		CComQIPtr<IMgaElementDecoratorEvents> edes(m_eventSink);
		if (edes)
			edes->WindowResizingStarted(nFlags, location.left, location.top, location.right, location.bottom);
		else
			ASSERT(false);
	}
}

void PartBase::WindowResizing(UINT nFlags, CRect& location)
{
	if (m_eventSink) {
		CComQIPtr<IMgaElementDecoratorEvents> edes(m_eventSink);
		if (edes)
			edes->WindowResizing(nFlags, location.left, location.top, location.right, location.bottom);
		else
			ASSERT(false);
	}
}

void PartBase::WindowResizingFinished(UINT nFlags, CRect& location)
{
	if (m_eventSink) {
		CComQIPtr<IMgaElementDecoratorEvents> edes(m_eventSink);
		if (edes)
			edes->WindowResizingFinished(nFlags, location.left, location.top, location.right, location.bottom);
		else
			ASSERT(false);
	}
}

void PartBase::WindowResized(UINT nFlags, CSize& size)
{
	if (m_eventSink) {
		CComQIPtr<IMgaElementDecoratorEvents> edes(m_eventSink);
		if (edes)
			edes->WindowResized(nFlags, size.cx, size.cy);
		else
			ASSERT(false);
	}
}

ECoordRefPoint PartBase::GetAlignment(ELocation loc) const
{
	switch (loc) {
		case L_NORTH:
		case L_CENTER:
		case L_SOUTH:
			return CRP_CENTER;
		case L_NORTHEAST:
		case L_EAST:
		case L_SOUTHEAST:
			return CRP_BEGIN;
		default:
			return CRP_END;
	}
}

long PartBase::GetBorderWidth(bool bActive) const
{
	long lBorderWidth = m_lBorderWidth;
	if ((m_bActive || bActive) && m_bHasViolation)
		lBorderWidth += WIDTH_BORDERVIOLATION + 1;
	return lBorderWidth;
}

void PartBase::SetBoxLocation(const CRect& cRect)
{
	m_Rect = cRect;
}

CRect PartBase::GetBoxLocation(bool bWithBorder) const
{
	if (bWithBorder)
		return m_Rect;
	CRect cRect = m_Rect;
	cRect.DeflateRect(GetBorderWidth(false), GetBorderWidth(false));
	return cRect;
}

void PartBase::SetReferenced(bool referenced)
{
	m_bReferenced = referenced;
}

};	// namespace DecoratorSDK
