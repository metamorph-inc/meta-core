//################################################################################################
//
// Composite part class (decorator part)
//	CompositePart.cpp
//
//################################################################################################

#include "StdAfx.h"
#include "CompositePart.h"
#include "DecoratorExceptions.h"


namespace DecoratorSDK {

//################################################################################################
//
// CLASS : CompositePart
//
//################################################################################################

CompositePart::CompositePart(PartBase* pPart, CComPtr<IMgaCommonDecoratorEvents>& eventSink):
	PartBase(pPart, eventSink)
{
}

CompositePart::~CompositePart()
{
	for (std::vector<PartBase*>::iterator ii = m_compositeParts.begin(); ii != m_compositeParts.end(); ++ii) {
		if ((*ii) != NULL) {
			delete (*ii);
		}
	}
	m_compositeParts.clear();
}

void CompositePart::AddPart(PartBase* part)
{
	ASSERT(part != NULL);

	part->SetParentPart(this);
	m_compositeParts.push_back(part);
}

void CompositePart::RemovePart(PartBase* part)
{
	ASSERT(part != NULL);

	for (std::vector<PartBase*>::iterator ii = m_compositeParts.begin(); ii != m_compositeParts.end(); ++ii) {
		if (part == (*ii)) {
			m_compositeParts.erase(ii);
			return;
		}
	}

	throw DecoratorException(EFail);
}

void CompositePart::RemovePart(int index)
{
	ASSERT(index >= 0);
	ASSERT(index < (int)m_compositeParts.size());

	int i = 0;
	for (std::vector<PartBase*>::iterator ii = m_compositeParts.begin(); ii != m_compositeParts.end(); ++ii, i++) {
		if (i == index) {
			m_compositeParts.erase(ii);
			return;
		}
	}

	throw DecoratorException(EFail);
}

PartBase* CompositePart::GetPart(int index)
{
	ASSERT(index >= 0);
	ASSERT(index < (int)m_compositeParts.size());

	int i = 0;
	for (std::vector<PartBase*>::iterator ii = m_compositeParts.begin(); ii != m_compositeParts.end(); ++ii, i++) {
		if (i == index) {
			return (*ii);
		}
	}

	return NULL;
}

long CompositePart::GetSize(void) const
{
	return (long)m_compositeParts.size();
}

void CompositePart::Initialize(CComPtr<IMgaProject>& pProject, CComPtr<IMgaMetaPart>& pPart, CComPtr<IMgaFCO>& pFCO)
{
	PartBase::Initialize(pProject, pPart, pFCO);
	for (std::vector<PartBase*>::iterator ii = m_compositeParts.begin(); ii != m_compositeParts.end(); ++ii) {
		(*ii)->Initialize(pProject, pPart, pFCO);
	}
}

void CompositePart::Destroy()
{
	for (std::vector<PartBase*>::iterator ii = m_compositeParts.begin(); ii != m_compositeParts.end(); ++ii) {
		(*ii)->Destroy();
	}
	PartBase::Destroy();
}

CString CompositePart::GetMnemonic(void) const
{
	HRESULT retVal = S_OK;
	for (std::vector<PartBase*>::const_iterator ii = m_compositeParts.begin(); ii != m_compositeParts.end(); ++ii) {
		try {
			CString mnemonic = (*ii)->GetMnemonic();
			if (!mnemonic.IsEmpty())
				return mnemonic;
		}
		catch(hresult_exception& e) {
			retVal = e.hr;
		}
		catch(DecoratorException& e) {
			retVal = e.GetHResult();
		}
		if (retVal != E_DECORATOR_NOT_IMPLEMENTED)
			break;
	}

	if (retVal != S_OK)
		throw DecoratorException((DecoratorExceptionCode)retVal);

	return CString();
}

feature_code CompositePart::GetFeatures(void) const
{
	feature_code featureCodes = 0;

	for (std::vector<PartBase*>::const_iterator ii = m_compositeParts.begin(); ii != m_compositeParts.end(); ++ii) {
		try {
			featureCodes |= (*ii)->GetFeatures();
		}
		catch(NotImplementedException&) {
		}
	}
	return featureCodes;
}

void CompositePart::SetParam(const CString& strName, VARIANT vValue)
{
	for (std::vector<PartBase*>::iterator ii = m_compositeParts.begin(); ii != m_compositeParts.end(); ++ii) {
		CComVariant value(vValue);
		VARIANT vPartValue;
		try {
			if ((*ii)->GetParam(strName, &vPartValue)) {
				CComVariant partValue(vPartValue);
				if (value != partValue)
					(*ii)->SetParam(strName, vValue);
			} else {
				(*ii)->SetParam(strName, vValue);
			}
		}
		catch(NotImplementedException&) {
		}
		catch(UnknownParameterException&) {
		}
	}
}

bool CompositePart::GetParam(const CString& strName, VARIANT* pvValue)
{
	HRESULT retVal = S_OK;
	for (std::vector<PartBase*>::iterator ii = m_compositeParts.begin(); ii != m_compositeParts.end(); ++ii) {
		try {
			if ((*ii)->GetParam(strName, pvValue))
				return true;
		}
		catch(hresult_exception& e) {
			retVal = e.hr;
		}
		catch(DecoratorException& e) {
			retVal = e.GetHResult();
		}
		if (retVal != E_DECORATOR_UNKNOWN_PARAMETER || retVal != E_DECORATOR_NOT_IMPLEMENTED)
			break;
	}

	if (retVal != S_OK)
		throw DecoratorException((DecoratorExceptionCode)retVal);

	return false;
}

void CompositePart::SetActive(bool bIsActive)
{
	PartBase::SetActive(bIsActive);
	for (std::vector<PartBase*>::iterator ii = m_compositeParts.begin(); ii != m_compositeParts.end(); ++ii) {
		(*ii)->SetActive(bIsActive);
	}
}

CSize CompositePart::GetPreferredSize(void) const
{
	HRESULT retVal = S_OK;
	for (std::vector<PartBase*>::const_iterator ii = m_compositeParts.begin(); ii != m_compositeParts.end(); ++ii) {
		try {
			CSize preferredSize = (*ii)->GetPreferredSize();
			if (preferredSize != CSize(0, 0))
				return preferredSize;
		}
		catch(hresult_exception& e) {
			retVal = e.hr;
		}
		catch(DecoratorException& e) {
			retVal = e.GetHResult();
		}
		if (retVal != E_DECORATOR_NOT_IMPLEMENTED)
			break;
	}

	if (retVal != S_OK)
		throw DecoratorException((DecoratorExceptionCode)retVal);

	return CSize(0, 0);
}


void CompositePart::SetLocation(const CRect& location)
{
	for (std::vector<PartBase*>::iterator ii = m_compositeParts.begin(); ii != m_compositeParts.end(); ++ii) {
		(*ii)->SetLocation(location);
	}
}

CRect CompositePart::GetLocation(void) const
{
	HRESULT retVal = S_OK;
	for (std::vector<PartBase*>::const_iterator ii = m_compositeParts.begin(); ii != m_compositeParts.end(); ++ii) {
		try {
			CRect location = (*ii)->GetLocation();
			if (!location.IsRectEmpty())
				return location;
		}
		catch(hresult_exception& e) {
			retVal = e.hr;
		}
		catch(DecoratorException& e) {
			retVal = e.GetHResult();
		}
		if (retVal != E_DECORATOR_NOT_IMPLEMENTED)
			break;
	}

	if (retVal != S_OK)
		throw DecoratorException((DecoratorExceptionCode)retVal);

	return CRect(0,0,0,0);
}

CRect CompositePart::GetLabelLocation(void) const
{
	HRESULT retVal = S_OK;
	for (std::vector<PartBase*>::const_iterator ii = m_compositeParts.begin(); ii != m_compositeParts.end(); ++ii) {
		try {
			CRect labelLocation = (*ii)->GetLabelLocation();
			if (!labelLocation.IsRectEmpty())
				return labelLocation;
		}
		catch(hresult_exception& e) {
			retVal = e.hr;
		}
		catch(DecoratorException& e) {
			retVal = e.GetHResult();
		}
		if (retVal != E_DECORATOR_NOT_IMPLEMENTED)
			break;
	}

	if (retVal != S_OK)
		throw DecoratorException((DecoratorExceptionCode)retVal);

	return CRect(0,0,0,0);
}

CRect CompositePart::GetPortLocation(CComPtr<IMgaFCO>& fco) const
{
	HRESULT retVal = S_OK;
	for (std::vector<PartBase*>::const_iterator ii = m_compositeParts.begin(); ii != m_compositeParts.end(); ++ii) {
		try {
			CRect portLocation = (*ii)->GetPortLocation(fco);
			if (!portLocation.IsRectEmpty())
				return portLocation;
		}
		catch(hresult_exception& e) {
			retVal = e.hr;
		}
		catch(DecoratorException& e) {
			retVal = e.GetHResult();
		}
		if (retVal != E_DECORATOR_NOT_IMPLEMENTED)
			break;
	}

	if (retVal != S_OK)
		throw DecoratorException((DecoratorExceptionCode)retVal);

	return CRect(0,0,0,0);
}

bool CompositePart::GetPorts(CComPtr<IMgaFCOs>& portFCOs) const
{
	HRESULT retVal = S_OK;
	for (std::vector<PartBase*>::const_iterator ii = m_compositeParts.begin(); ii != m_compositeParts.end(); ++ii) {
		try {
			if ((*ii)->GetPorts(portFCOs))
				return true;
		}
		catch(hresult_exception& e) {
			retVal = e.hr;
		}
		catch(DecoratorException& e) {
			retVal = e.GetHResult();
		}
		if (retVal != E_DECORATOR_NOT_IMPLEMENTED)
			break;
	}

	if (retVal != S_OK)
		throw DecoratorException((DecoratorExceptionCode)retVal);

	return false;
}


void CompositePart::Draw(CDC* pDC, Gdiplus::Graphics* gdip)
{
	for (std::vector<PartBase*>::iterator ii = m_compositeParts.begin(); ii != m_compositeParts.end(); ++ii) {
		(*ii)->Draw(pDC, gdip);
	}
}

void CompositePart::SaveState()
{
	for (std::vector<PartBase*>::iterator ii = m_compositeParts.begin(); ii != m_compositeParts.end(); ++ii) {
		(*ii)->SaveState();
	}
}

// New functions
void CompositePart::InitializeEx(CComPtr<IMgaProject>& pProject, CComPtr<IMgaMetaPart>& pPart, CComPtr<IMgaFCO>& pFCO,
								 HWND parentWnd, PreferenceMap& preferences)
{
	PartBase::InitializeEx(pProject, pPart, pFCO, parentWnd, preferences);
	for (std::vector<PartBase*>::iterator ii = m_compositeParts.begin(); ii != m_compositeParts.end(); ++ii) {
		(*ii)->InitializeEx(pProject, pPart, pFCO, parentWnd, preferences);
	}
}

void CompositePart::SetSelected(bool bIsSelected)
{
	PartBase::SetSelected(bIsSelected);
	for (std::vector<PartBase*>::iterator ii = m_compositeParts.begin(); ii != m_compositeParts.end(); ++ii) {
		(*ii)->SetSelected(bIsSelected);
	}
}

bool CompositePart::MouseMoved(UINT nFlags, const CPoint& point, HDC transformHDC)
{
	HRESULT retVal = S_OK;
	for (std::vector<PartBase*>::const_iterator ii = m_compositeParts.begin(); ii != m_compositeParts.end(); ++ii) {
		try {
			if ((*ii)->MouseMoved(nFlags, point, transformHDC))
				return true;
		}
		catch(hresult_exception& e) {
			retVal = e.hr;
		}
		catch(DecoratorException& e) {
			retVal = e.GetHResult();
		}
		if (retVal != S_OK && retVal != S_DECORATOR_EVENT_NOT_HANDLED && retVal != E_DECORATOR_NOT_IMPLEMENTED)
			break;
	}

	if (retVal != S_OK && retVal != S_DECORATOR_EVENT_NOT_HANDLED)
		throw DecoratorException((DecoratorExceptionCode)retVal);

	return false;
}

bool CompositePart::MouseLeftButtonDown(UINT nFlags, const CPoint& point, HDC transformHDC)
{
	HRESULT retVal = S_OK;
	for (std::vector<PartBase*>::const_iterator ii = m_compositeParts.begin(); ii != m_compositeParts.end(); ++ii) {
		try {
			if ((*ii)->MouseLeftButtonDown(nFlags, point, transformHDC))
				return true;
		}
		catch(hresult_exception& e) {
			retVal = e.hr;
		}
		catch(DecoratorException& e) {
			retVal = e.GetHResult();
		}
		if (retVal != S_OK && retVal != E_DECORATOR_NOT_IMPLEMENTED)
			break;
	}

	if (retVal != S_OK && retVal != S_DECORATOR_EVENT_NOT_HANDLED)
		throw DecoratorException((DecoratorExceptionCode)retVal);

	return false;
}

bool CompositePart::MouseLeftButtonUp(UINT nFlags, const CPoint& point, HDC transformHDC)
{
	HRESULT retVal = S_OK;
	for (std::vector<PartBase*>::const_iterator ii = m_compositeParts.begin(); ii != m_compositeParts.end(); ++ii) {
		try {
			if ((*ii)->MouseLeftButtonUp(nFlags, point, transformHDC))
				return true;
		}
		catch(hresult_exception& e) {
			retVal = e.hr;
		}
		catch(DecoratorException& e) {
			retVal = e.GetHResult();
		}
		if (retVal != S_OK && retVal != E_DECORATOR_NOT_IMPLEMENTED)
			break;
	}

	if (retVal != S_OK && retVal != S_DECORATOR_EVENT_NOT_HANDLED)
		throw DecoratorException((DecoratorExceptionCode)retVal);

	return false;
}

bool CompositePart::MouseLeftButtonDoubleClick(UINT nFlags, const CPoint& point, HDC transformHDC)
{
	HRESULT retVal = S_OK;
	for (std::vector<PartBase*>::const_iterator ii = m_compositeParts.begin(); ii != m_compositeParts.end(); ++ii) {
		try {
			if ((*ii)->MouseLeftButtonDoubleClick(nFlags, point, transformHDC))
				return true;
		}
		catch(hresult_exception& e) {
			retVal = e.hr;
		}
		catch(DecoratorException& e) {
			retVal = e.GetHResult();
		}
		if (retVal != S_OK && retVal != S_DECORATOR_EVENT_NOT_HANDLED && retVal != E_DECORATOR_NOT_IMPLEMENTED)
			break;
	}

	if (retVal != S_OK && retVal != S_DECORATOR_EVENT_NOT_HANDLED)
		throw DecoratorException((DecoratorExceptionCode)retVal);

	return false;
}

bool CompositePart::MouseRightButtonDown(HMENU hCtxMenu, UINT nFlags, const CPoint& point, HDC transformHDC)
{
	HRESULT retVal = S_OK;
	for (std::vector<PartBase*>::const_iterator ii = m_compositeParts.begin(); ii != m_compositeParts.end(); ++ii) {
		try {
			if ((*ii)->MouseRightButtonDown(hCtxMenu, nFlags, point, transformHDC))
				return true;
		}
		catch(hresult_exception& e) {
			retVal = e.hr;
		}
		catch(DecoratorException& e) {
			retVal = e.GetHResult();
		}
		if (retVal != S_OK && retVal != S_DECORATOR_EVENT_NOT_HANDLED && retVal != E_DECORATOR_NOT_IMPLEMENTED)
			break;
	}

	if (retVal != S_OK && retVal != S_DECORATOR_EVENT_NOT_HANDLED)
		throw DecoratorException((DecoratorExceptionCode)retVal);

	return false;
}

bool CompositePart::MouseRightButtonUp(UINT nFlags, const CPoint& point, HDC transformHDC)
{
	HRESULT retVal = S_OK;
	for (std::vector<PartBase*>::const_iterator ii = m_compositeParts.begin(); ii != m_compositeParts.end(); ++ii) {
		try {
			if ((*ii)->MouseRightButtonUp(nFlags, point, transformHDC))
				return true;
		}
		catch(hresult_exception& e) {
			retVal = e.hr;
		}
		catch(DecoratorException& e) {
			retVal = e.GetHResult();
		}
		if (retVal != S_OK && retVal != S_DECORATOR_EVENT_NOT_HANDLED && retVal != E_DECORATOR_NOT_IMPLEMENTED)
			break;
	}

	if (retVal != S_OK && retVal != S_DECORATOR_EVENT_NOT_HANDLED)
		throw DecoratorException((DecoratorExceptionCode)retVal);

	return false;
}

bool CompositePart::MouseRightButtonDoubleClick(UINT nFlags, const CPoint& point, HDC transformHDC)
{
	HRESULT retVal = S_OK;
	for (std::vector<PartBase*>::const_iterator ii = m_compositeParts.begin(); ii != m_compositeParts.end(); ++ii) {
		try {
			if ((*ii)->MouseRightButtonDoubleClick(nFlags, point, transformHDC))
				return true;
		}
		catch(hresult_exception& e) {
			retVal = e.hr;
		}
		catch(DecoratorException& e) {
			retVal = e.GetHResult();
		}
		if (retVal != S_OK && retVal != S_DECORATOR_EVENT_NOT_HANDLED && retVal != E_DECORATOR_NOT_IMPLEMENTED)
			break;
	}

	if (retVal != S_OK && retVal != S_DECORATOR_EVENT_NOT_HANDLED)
		throw DecoratorException((DecoratorExceptionCode)retVal);

	return false;
}

bool CompositePart::MouseMiddleButtonDown(UINT nFlags, const CPoint& point, HDC transformHDC)
{
	HRESULT retVal = S_OK;
	for (std::vector<PartBase*>::const_iterator ii = m_compositeParts.begin(); ii != m_compositeParts.end(); ++ii) {
		try {
			if ((*ii)->MouseMiddleButtonDown(nFlags, point, transformHDC))
				return true;
		}
		catch(hresult_exception& e) {
			retVal = e.hr;
		}
		catch(DecoratorException& e) {
			retVal = e.GetHResult();
		}
		if (retVal != S_OK && retVal != S_DECORATOR_EVENT_NOT_HANDLED && retVal != E_DECORATOR_NOT_IMPLEMENTED)
			break;
	}

	if (retVal != S_OK && retVal != S_DECORATOR_EVENT_NOT_HANDLED)
		throw DecoratorException((DecoratorExceptionCode)retVal);

	return false;
}

bool CompositePart::MouseMiddleButtonUp(UINT nFlags, const CPoint& point, HDC transformHDC)
{
	HRESULT retVal = S_OK;
	for (std::vector<PartBase*>::const_iterator ii = m_compositeParts.begin(); ii != m_compositeParts.end(); ++ii) {
		try {
			if ((*ii)->MouseMiddleButtonUp(nFlags, point, transformHDC))
				return true;
		}
		catch(hresult_exception& e) {
			retVal = e.hr;
		}
		catch(DecoratorException& e) {
			retVal = e.GetHResult();
		}
		if (retVal != S_OK && retVal != S_DECORATOR_EVENT_NOT_HANDLED && retVal != E_DECORATOR_NOT_IMPLEMENTED)
			break;
	}

	if (retVal != S_OK && retVal != S_DECORATOR_EVENT_NOT_HANDLED)
		throw DecoratorException((DecoratorExceptionCode)retVal);

	return false;
}

bool CompositePart::MouseMiddleButtonDoubleClick(UINT nFlags, const CPoint& point, HDC transformHDC)
{
	HRESULT retVal = S_OK;
	for (std::vector<PartBase*>::const_iterator ii = m_compositeParts.begin(); ii != m_compositeParts.end(); ++ii) {
		try {
			if ((*ii)->MouseMiddleButtonDoubleClick(nFlags, point, transformHDC))
				return true;
		}
		catch(hresult_exception& e) {
			retVal = e.hr;
		}
		catch(DecoratorException& e) {
			retVal = e.GetHResult();
		}
		if (retVal != S_OK && retVal != S_DECORATOR_EVENT_NOT_HANDLED && retVal != E_DECORATOR_NOT_IMPLEMENTED)
			break;
	}

	if (retVal != S_OK && retVal != S_DECORATOR_EVENT_NOT_HANDLED)
		throw DecoratorException((DecoratorExceptionCode)retVal);

	return false;
}

bool CompositePart::MouseWheelTurned(UINT nFlags, short distance, const CPoint& point, HDC transformHDC)
{
	HRESULT retVal = S_OK;
	for (std::vector<PartBase*>::const_iterator ii = m_compositeParts.begin(); ii != m_compositeParts.end(); ++ii) {
		try {
			if ((*ii)->MouseWheelTurned(nFlags, distance, point, transformHDC))
				return true;
		}
		catch(hresult_exception& e) {
			retVal = e.hr;
		}
		catch(DecoratorException& e) {
			retVal = e.GetHResult();
		}
		if (retVal != S_OK && retVal != S_DECORATOR_EVENT_NOT_HANDLED && retVal != E_DECORATOR_NOT_IMPLEMENTED)
			break;
	}

	if (retVal != S_OK && retVal != S_DECORATOR_EVENT_NOT_HANDLED)
		throw DecoratorException((DecoratorExceptionCode)retVal);

	return false;
}

bool CompositePart::DragEnter(DROPEFFECT* dropEffect, COleDataObject* pDataObject, DWORD dwKeyState, const CPoint& point, HDC transformHDC)
{
	HRESULT retVal = S_OK;
	for (std::vector<PartBase*>::const_iterator ii = m_compositeParts.begin(); ii != m_compositeParts.end(); ++ii) {
		try {
			if ((*ii)->DragEnter(dropEffect, pDataObject, dwKeyState, point, transformHDC))
				return true;
		}
		catch(hresult_exception& e) {
			retVal = e.hr;
		}
		catch(DecoratorException& e) {
			retVal = e.GetHResult();
		}
		if (retVal != S_OK && retVal != S_DECORATOR_EVENT_NOT_HANDLED && retVal != E_DECORATOR_NOT_IMPLEMENTED)
			break;
	}

	if (retVal != S_OK && retVal != S_DECORATOR_EVENT_NOT_HANDLED)
		throw DecoratorException((DecoratorExceptionCode)retVal);

	return false;
}

bool CompositePart::DragOver(DROPEFFECT* dropEffect, COleDataObject* pDataObject, DWORD dwKeyState, const CPoint& point, HDC transformHDC)
{
	HRESULT retVal = S_OK;
	for (std::vector<PartBase*>::const_iterator ii = m_compositeParts.begin(); ii != m_compositeParts.end(); ++ii) {
		try {
			if ((*ii)->DragOver(dropEffect, pDataObject, dwKeyState, point, transformHDC))
				return true;
		}
		catch(hresult_exception& e) {
			retVal = e.hr;
		}
		catch(DecoratorException& e) {
			retVal = e.GetHResult();
		}
		if (retVal != S_OK && retVal != S_DECORATOR_EVENT_NOT_HANDLED && retVal != E_DECORATOR_NOT_IMPLEMENTED)
			break;
	}

	if (retVal != S_OK && retVal != S_DECORATOR_EVENT_NOT_HANDLED)
		throw DecoratorException((DecoratorExceptionCode)retVal);

	return false;
}

bool CompositePart::Drop(COleDataObject* pDataObject, DROPEFFECT dropEffect, const CPoint& point, HDC transformHDC)
{
	HRESULT retVal = S_OK;
	for (std::vector<PartBase*>::const_iterator ii = m_compositeParts.begin(); ii != m_compositeParts.end(); ++ii) {
		try {
			if ((*ii)->Drop(pDataObject, dropEffect, point, transformHDC))
				return true;
		}
		catch(hresult_exception& e) {
			retVal = e.hr;
		}
		catch(DecoratorException& e) {
			retVal = e.GetHResult();
		}
		if (retVal != S_OK && retVal != S_DECORATOR_EVENT_NOT_HANDLED && retVal != E_DECORATOR_NOT_IMPLEMENTED)
			break;
	}

	if (retVal != S_OK && retVal != S_DECORATOR_EVENT_NOT_HANDLED)
		throw DecoratorException((DecoratorExceptionCode)retVal);

	return false;
}

bool CompositePart::DropFile(HDROP p_hDropInfo, const CPoint& point, HDC transformHDC)
{
	HRESULT retVal = S_OK;
	for (std::vector<PartBase*>::const_iterator ii = m_compositeParts.begin(); ii != m_compositeParts.end(); ++ii) {
		try {
			if ((*ii)->DropFile(p_hDropInfo, point, transformHDC))
				return true;
		}
		catch(hresult_exception& e) {
			retVal = e.hr;
		}
		catch(DecoratorException& e) {
			retVal = e.GetHResult();
		}
		if (retVal != S_OK && retVal != S_DECORATOR_EVENT_NOT_HANDLED && retVal != E_DECORATOR_NOT_IMPLEMENTED)
			break;
	}

	if (retVal != S_OK && retVal != S_DECORATOR_EVENT_NOT_HANDLED)
		throw DecoratorException((DecoratorExceptionCode)retVal);

	return false;
}

bool CompositePart::MenuItemSelected(UINT menuItemId, UINT nFlags, const CPoint& point, HDC transformHDC)
{
	HRESULT retVal = S_OK;
	for (std::vector<PartBase*>::const_iterator ii = m_compositeParts.begin(); ii != m_compositeParts.end(); ++ii) {
		try {
			if ((*ii)->MenuItemSelected(menuItemId, nFlags, point, transformHDC))
				return true;
		}
		catch(hresult_exception& e) {
			retVal = e.hr;
		}
		catch(DecoratorException& e) {
			retVal = e.GetHResult();
		}
		if (retVal != S_OK && retVal != S_DECORATOR_EVENT_NOT_HANDLED && retVal != E_DECORATOR_NOT_IMPLEMENTED)
			break;
	}

	if (retVal != S_OK && retVal != S_DECORATOR_EVENT_NOT_HANDLED)
		throw DecoratorException((DecoratorExceptionCode)retVal);

	return false;
}

bool CompositePart::OperationCanceledByGME(void)
{
	HRESULT retVal = S_OK;
	for (std::vector<PartBase*>::const_iterator ii = m_compositeParts.begin(); ii != m_compositeParts.end(); ++ii) {
		try {
			if ((*ii)->OperationCanceledByGME())
				return true;
		}
		catch(hresult_exception& e) {
			retVal = e.hr;
		}
		catch(DecoratorException& e) {
			retVal = e.GetHResult();
		}
		if (retVal != S_OK && retVal != S_DECORATOR_EVENT_NOT_HANDLED && retVal != E_DECORATOR_NOT_IMPLEMENTED)
			break;
	}

	if (retVal != S_OK && retVal != S_DECORATOR_EVENT_NOT_HANDLED)
		throw DecoratorException((DecoratorExceptionCode)retVal);

	return false;
}

void CompositePart::SetBoxLocation(const CRect& cRect)
{
	for (std::vector<PartBase*>::iterator ii = m_compositeParts.begin(); ii != m_compositeParts.end(); ++ii) {
		(*ii)->SetBoxLocation(cRect);
	}
}

void CompositePart::SetReferenced(bool referenced)
{
	for (std::vector<PartBase*>::iterator ii = m_compositeParts.begin(); ii != m_compositeParts.end(); ++ii) {
		(*ii)->SetReferenced(referenced);
	}
}

void CompositePart::SetParentPart(PartBase* pPart)
{
	for (std::vector<PartBase*>::iterator ii = m_compositeParts.begin(); ii != m_compositeParts.end(); ++ii) {
		(*ii)->SetParentPart(pPart);
	}
}

}; // namespace DecoratorSDK
