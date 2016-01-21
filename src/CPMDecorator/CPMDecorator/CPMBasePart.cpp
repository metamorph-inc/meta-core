/*
*    Copyright (c) Vanderbilt University,  2011
*    ALL RIGHTS RESERVED
*
*/ 


//################################################################################################
//
// CPM decorator base part class
//	CPMBasePart.cpp
// Contains the specific decorator parts which compose the final decorator
//
//################################################################################################

#include "StdAfx.h"
#include "DecoratorLib.h"
#include "CPMBasePart.h"
#include "ClassLabelPart.h"

#include "Decorator.h"
#include "CPMAttributePart.h"

#include "DecoratorExceptions.h"

namespace Decor {

//################################################################################################
//
// CLASS : CPMBasePart
//
//################################################################################################

CPMBasePart::CPMBasePart(PartBase* pPart, CComPtr<IMgaCommonDecoratorEvents>& eventSink, COLORREF color):
	VectorPart					(pPart, eventSink),

	m_LabelPart					(NULL),

	m_lMaxTextWidth				(0),
	m_lMaxTextHeight			(0),
	m_lMinTextWidth				(LONG_MAX),
	m_lMinTextHeight			(LONG_MAX),
	m_backgroundColor(color)
{
	m_calcSize.SetSize(0, 0);
}

CPMBasePart::~CPMBasePart()
{
	if (m_LabelPart != NULL)
		delete m_LabelPart;
	unsigned long i = 0;
	for (i = 0; i < m_AttributeParts.size(); i++) {
		delete m_AttributeParts[i];
	}
	m_AttributeParts.clear();
}

void CPMBasePart::Initialize(CComPtr<IMgaProject>& pProject, CComPtr<IMgaMetaPart>& pPart, CComPtr<IMgaFCO>& pFCO)
{
	if (m_LabelPart != NULL)
		m_LabelPart->Initialize(pProject, pPart, pFCO);
	for (std::vector<DecoratorSDK::PartInterface*>::iterator ii = m_AttributeParts.begin(); ii != m_AttributeParts.end(); ++ii) {
		(*ii)->Initialize(pProject, pPart, pFCO);
	}
	VectorPart::Initialize(pProject, pPart, pFCO);
}

void CPMBasePart::Destroy()
{
	if (m_LabelPart != NULL)
		m_LabelPart->Destroy();
	for (std::vector<DecoratorSDK::PartInterface*>::iterator ii = m_AttributeParts.begin(); ii != m_AttributeParts.end(); ++ii) {
		(*ii)->Destroy();
	}
	VectorPart::Destroy();
}

CString CPMBasePart::GetMnemonic(void) const
{
	return VectorPart::GetMnemonic();
}

feature_code CPMBasePart::GetFeatures(void) const
{
	feature_code featureCodes = 0;

	if (m_LabelPart != NULL) {
		feature_code labelFeatureCodes = m_LabelPart->GetFeatures();
		featureCodes |= labelFeatureCodes;
	}

	for (std::vector<DecoratorSDK::PartInterface*>::const_iterator ii = m_AttributeParts.begin(); ii != m_AttributeParts.end(); ++ii) {
		feature_code attrubuteFeatureCodes = (*ii)->GetFeatures();
		featureCodes |= attrubuteFeatureCodes;
	}

	feature_code partFeatureCodes = VectorPart::GetFeatures();
	featureCodes |= partFeatureCodes;

	return featureCodes;
}

void CPMBasePart::SetParam(const CString& strName, VARIANT vValue)
{
	if (m_LabelPart != NULL)
		m_LabelPart->SetParam(strName, vValue);
	for (std::vector<DecoratorSDK::PartInterface*>::iterator ii = m_AttributeParts.begin(); ii != m_AttributeParts.end(); ++ii) {
		(*ii)->SetParam(strName, vValue);
	}
	VectorPart::SetParam(strName, vValue);
}

bool CPMBasePart::GetParam(const CString& strName, VARIANT* pvValue)
{
	try {
		if (VectorPart::GetParam(strName, pvValue))
			return true;
	}
	catch(UnknownParameterException&) {
	}
	return false;
}

void CPMBasePart::SetActive(bool bIsActive)
{
	if (m_LabelPart != NULL)
		m_LabelPart->SetActive(bIsActive);
	for (std::vector<DecoratorSDK::PartInterface*>::iterator ii = m_AttributeParts.begin(); ii != m_AttributeParts.end(); ++ii) {
		(*ii)->SetActive(bIsActive);
	}
	VectorPart::SetActive(bIsActive);
}

CSize CPMBasePart::GetPreferredSize(void) const
{
	CSize size = ResizablePart::GetPreferredSize();
	if (size.cx * size.cy != 0)
		return size;

	ASSERT(m_calcSize.cx > 0 && m_calcSize.cy > 0);
	return m_calcSize;
}

void CPMBasePart::SetLocation(const CRect& location)
{
	if (m_LabelPart != NULL)
		m_LabelPart->SetLocation(location);
	for (std::vector<DecoratorSDK::PartInterface*>::iterator ii = m_AttributeParts.begin(); ii != m_AttributeParts.end(); ++ii) {
		(*ii)->SetLocation(location);
	}
	VectorPart::SetLocation(location);
}

CRect CPMBasePart::GetLocation(void) const
{
	return VectorPart::GetLocation();
}

CRect CPMBasePart::GetLabelLocation(void) const
{
	CRect labelLocation;
	if (m_LabelPart == NULL) {
		labelLocation.top = labelLocation.bottom = labelLocation.right = labelLocation.left = 0;
		return labelLocation;
	}
	try {
		labelLocation = m_LabelPart->GetLabelLocation();
	}
	catch(NotImplementedException&) {
	}
	return labelLocation;
}

void CPMBasePart::Draw(CDC* pDC, Gdiplus::Graphics* gdip)
{
	CalcRelPositions(pDC, gdip);
	VectorPart::Draw(pDC, gdip);
	if (m_LabelPart != NULL)
		m_LabelPart->Draw(pDC, gdip);
	for (std::vector<DecoratorSDK::PartInterface*>::iterator ii = m_AttributeParts.begin(); ii != m_AttributeParts.end(); ++ii) {
		(*ii)->Draw(pDC, gdip);
	}
}

void CPMBasePart::SaveState()
{
	if (m_LabelPart != NULL)
		m_LabelPart->SaveState();
	for (std::vector<DecoratorSDK::PartInterface*>::iterator ii = m_AttributeParts.begin(); ii != m_AttributeParts.end(); ++ii) {
		(*ii)->SaveState();
	}
	VectorPart::SaveState();
}

// New functions
void CPMBasePart::InitializeEx(CComPtr<IMgaProject>& pProject, CComPtr<IMgaMetaPart>& pPart, CComPtr<IMgaFCO>& pFCO,
									HWND parentWnd, PreferenceMap& preferences)
{
	DecoratorSDK::getFacilities().loadPathes(pProject, true);

	VectorPart::InitializeEx(pProject, pPart, pFCO, parentWnd, preferences);
	CalcRelPositions();
}

void CPMBasePart::SetSelected(bool bIsSelected)
{
	if (m_LabelPart != NULL)
		m_LabelPart->SetSelected(bIsSelected);
	for (std::vector<DecoratorSDK::PartInterface*>::iterator ii = m_AttributeParts.begin(); ii != m_AttributeParts.end(); ++ii) {
		(*ii)->SetSelected(bIsSelected);
	}
	VectorPart::SetSelected(bIsSelected);
}

bool CPMBasePart::MouseMoved(UINT nFlags, const CPoint& point, HDC transformHDC)
{
	HRESULT retVal = S_OK;
	try {
		if (VectorPart::MouseMoved(nFlags, point, transformHDC))
			return true;
	}
	catch(hresult_exception& e) {
		retVal = e.hr;
	}
	catch(DecoratorException& e) {
		retVal = e.GetHResult();
	}
	if (m_LabelPart != NULL && (retVal == S_OK || retVal == S_DECORATOR_EVENT_NOT_HANDLED || retVal == E_DECORATOR_NOT_IMPLEMENTED)) {
		try {
			if (m_LabelPart->MouseMoved(nFlags, point, transformHDC))
				return true;
		}
		catch(hresult_exception& e) {
			retVal = e.hr;
		}
		catch(DecoratorException& e) {
			retVal = e.GetHResult();
		}
	}
	if (retVal == S_OK || retVal == S_DECORATOR_EVENT_NOT_HANDLED || retVal == E_DECORATOR_NOT_IMPLEMENTED) {
		for (std::vector<DecoratorSDK::PartInterface*>::iterator ii = m_AttributeParts.begin(); ii != m_AttributeParts.end(); ++ii) {
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
	}

	return false;
}

bool CPMBasePart::MouseLeftButtonDown(UINT nFlags, const CPoint& point, HDC transformHDC)
{
	HRESULT retVal = S_OK;
	try {
		if (VectorPart::MouseLeftButtonDown(nFlags, point, transformHDC))
			return true;
	}
	catch(hresult_exception& e) {
		retVal = e.hr;
	}
	catch(DecoratorException& e) {
		retVal = e.GetHResult();
	}
	if (m_LabelPart != NULL && (retVal == S_OK || retVal == S_DECORATOR_EVENT_NOT_HANDLED || retVal == E_DECORATOR_NOT_IMPLEMENTED)) {
		try {
			if (m_LabelPart->MouseLeftButtonDown(nFlags, point, transformHDC))
				return true;
		}
		catch(hresult_exception& e) {
			retVal = e.hr;
		}
		catch(DecoratorException& e) {
			retVal = e.GetHResult();
		}
	}
	if (retVal == S_OK || retVal == S_DECORATOR_EVENT_NOT_HANDLED || retVal == E_DECORATOR_NOT_IMPLEMENTED) {
		for (std::vector<DecoratorSDK::PartInterface*>::iterator ii = m_AttributeParts.begin(); ii != m_AttributeParts.end(); ++ii) {
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
			if (retVal != S_OK && retVal != S_DECORATOR_EVENT_NOT_HANDLED && retVal != E_DECORATOR_NOT_IMPLEMENTED)
				break;
		}
	}

	return false;
}

bool CPMBasePart::MouseLeftButtonUp(UINT nFlags, const CPoint& point, HDC transformHDC)
{
	HRESULT retVal = S_OK;
	try {
		if (VectorPart::MouseLeftButtonUp(nFlags, point, transformHDC))
			return true;
	}
	catch(hresult_exception& e) {
		retVal = e.hr;
	}
	catch(DecoratorException& e) {
		retVal = e.GetHResult();
	}
	if (m_LabelPart != NULL && (retVal == S_OK || retVal == S_DECORATOR_EVENT_NOT_HANDLED || retVal == E_DECORATOR_NOT_IMPLEMENTED)) {
		try {
			if (m_LabelPart->MouseLeftButtonUp(nFlags, point, transformHDC))
				return true;
		}
		catch(hresult_exception& e) {
			retVal = e.hr;
		}
		catch(DecoratorException& e) {
			retVal = e.GetHResult();
		}
	}
	if (retVal == S_OK || retVal == S_DECORATOR_EVENT_NOT_HANDLED || retVal == E_DECORATOR_NOT_IMPLEMENTED) {
		for (std::vector<DecoratorSDK::PartInterface*>::iterator ii = m_AttributeParts.begin(); ii != m_AttributeParts.end(); ++ii) {
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
			if (retVal != S_OK && retVal != S_DECORATOR_EVENT_NOT_HANDLED && retVal != E_DECORATOR_NOT_IMPLEMENTED)
				break;
		}
	}

	return false;
}

bool CPMBasePart::MouseLeftButtonDoubleClick(UINT nFlags, const CPoint& point, HDC transformHDC)
{
	HRESULT retVal = S_OK;
	try {
		if (VectorPart::MouseLeftButtonDoubleClick(nFlags, point, transformHDC))
			return true;
	}
	catch(hresult_exception& e) {
		retVal = e.hr;
	}
	catch(DecoratorException& e) {
		retVal = e.GetHResult();
	}
	if (m_LabelPart != NULL && (retVal == S_OK || retVal == S_DECORATOR_EVENT_NOT_HANDLED || retVal == E_DECORATOR_NOT_IMPLEMENTED)) {
		try {
			if (m_LabelPart->MouseLeftButtonDoubleClick(nFlags, point, transformHDC))
				return true;
		}
		catch(hresult_exception& e) {
			retVal = e.hr;
		}
		catch(DecoratorException& e) {
			retVal = e.GetHResult();
		}
	}
	if (retVal == S_OK || retVal == S_DECORATOR_EVENT_NOT_HANDLED || retVal == E_DECORATOR_NOT_IMPLEMENTED) {
		for (std::vector<DecoratorSDK::PartInterface*>::iterator ii = m_AttributeParts.begin(); ii != m_AttributeParts.end(); ++ii) {
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
	}

	return false;
}

bool CPMBasePart::MouseRightButtonDown(HMENU hCtxMenu, UINT nFlags, const CPoint& point, HDC transformHDC)
{
	HRESULT retVal = S_OK;
	try {
		if (VectorPart::MouseRightButtonDown(hCtxMenu, nFlags, point, transformHDC))
			return true;
	}
	catch(hresult_exception& e) {
		retVal = e.hr;
	}
	catch(DecoratorException& e) {
		retVal = e.GetHResult();
	}
	if (m_LabelPart != NULL && (retVal == S_OK || retVal == S_DECORATOR_EVENT_NOT_HANDLED || retVal == E_DECORATOR_NOT_IMPLEMENTED)) {
		try {
			if (m_LabelPart->MouseRightButtonDown(hCtxMenu, nFlags, point, transformHDC))
				return true;
		}
		catch(hresult_exception& e) {
			retVal = e.hr;
		}
		catch(DecoratorException& e) {
			retVal = e.GetHResult();
		}
	}
	if (retVal == S_OK || retVal == S_DECORATOR_EVENT_NOT_HANDLED || retVal == E_DECORATOR_NOT_IMPLEMENTED) {
		for (std::vector<DecoratorSDK::PartInterface*>::iterator ii = m_AttributeParts.begin(); ii != m_AttributeParts.end(); ++ii) {
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
	}

	return false;
}

bool CPMBasePart::MouseRightButtonUp(UINT nFlags, const CPoint& point, HDC transformHDC)
{
	HRESULT retVal = S_OK;
	try {
		if (VectorPart::MouseRightButtonUp(nFlags, point, transformHDC))
			return true;
	}
	catch(hresult_exception& e) {
		retVal = e.hr;
	}
	catch(DecoratorException& e) {
		retVal = e.GetHResult();
	}
	if (m_LabelPart != NULL && (retVal == S_OK || retVal == S_DECORATOR_EVENT_NOT_HANDLED || retVal == E_DECORATOR_NOT_IMPLEMENTED)) {
		try {
			if (m_LabelPart->MouseRightButtonUp(nFlags, point, transformHDC))
				return true;
		}
		catch(hresult_exception& e) {
			retVal = e.hr;
		}
		catch(DecoratorException& e) {
			retVal = e.GetHResult();
		}
	}
	if (retVal == S_OK || retVal == S_DECORATOR_EVENT_NOT_HANDLED || retVal == E_DECORATOR_NOT_IMPLEMENTED) {
		for (std::vector<DecoratorSDK::PartInterface*>::iterator ii = m_AttributeParts.begin(); ii != m_AttributeParts.end(); ++ii) {
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
	}

	return false;
}

bool CPMBasePart::MouseRightButtonDoubleClick(UINT nFlags, const CPoint& point, HDC transformHDC)
{
	HRESULT retVal = S_OK;
	try {
		if (VectorPart::MouseRightButtonDoubleClick(nFlags, point, transformHDC))
			return true;
	}
	catch(hresult_exception& e) {
		retVal = e.hr;
	}
	catch(DecoratorException& e) {
		retVal = e.GetHResult();
	}
	if (m_LabelPart != NULL && (retVal == S_OK || retVal == S_DECORATOR_EVENT_NOT_HANDLED || retVal == E_DECORATOR_NOT_IMPLEMENTED)) {
		try {
			if (m_LabelPart->MouseRightButtonDoubleClick(nFlags, point, transformHDC))
				return true;
		}
		catch(hresult_exception& e) {
			retVal = e.hr;
		}
		catch(DecoratorException& e) {
			retVal = e.GetHResult();
		}
	}
	if (retVal == S_OK || retVal == S_DECORATOR_EVENT_NOT_HANDLED || retVal == E_DECORATOR_NOT_IMPLEMENTED) {
		for (std::vector<DecoratorSDK::PartInterface*>::iterator ii = m_AttributeParts.begin(); ii != m_AttributeParts.end(); ++ii) {
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
	}

	return false;
}

bool CPMBasePart::MouseMiddleButtonDown(UINT nFlags, const CPoint& point, HDC transformHDC)
{
	HRESULT retVal = S_OK;
	try {
		if (VectorPart::MouseMiddleButtonDown(nFlags, point, transformHDC))
			return true;
	}
	catch(hresult_exception& e) {
		retVal = e.hr;
	}
	catch(DecoratorException& e) {
		retVal = e.GetHResult();
	}
	if (m_LabelPart != NULL && (retVal == S_OK || retVal == S_DECORATOR_EVENT_NOT_HANDLED || retVal == E_DECORATOR_NOT_IMPLEMENTED)) {
		try {
			if (m_LabelPart->MouseMiddleButtonDown(nFlags, point, transformHDC))
				return true;
		}
		catch(hresult_exception& e) {
			retVal = e.hr;
		}
		catch(DecoratorException& e) {
			retVal = e.GetHResult();
		}
	}
	if (retVal == S_OK || retVal == S_DECORATOR_EVENT_NOT_HANDLED || retVal == E_DECORATOR_NOT_IMPLEMENTED) {
		for (std::vector<DecoratorSDK::PartInterface*>::iterator ii = m_AttributeParts.begin(); ii != m_AttributeParts.end(); ++ii) {
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
	}

	return false;
}

bool CPMBasePart::MouseMiddleButtonUp(UINT nFlags, const CPoint& point, HDC transformHDC)
{
	HRESULT retVal = S_OK;
	try {
		if (VectorPart::MouseMiddleButtonUp(nFlags, point, transformHDC))
			return true;
	}
	catch(hresult_exception& e) {
		retVal = e.hr;
	}
	catch(DecoratorException& e) {
		retVal = e.GetHResult();
	}
	if (m_LabelPart != NULL && (retVal == S_OK || retVal == S_DECORATOR_EVENT_NOT_HANDLED || retVal == E_DECORATOR_NOT_IMPLEMENTED)) {
		try {
			if (m_LabelPart->MouseMiddleButtonUp(nFlags, point, transformHDC))
				return true;
		}
		catch(hresult_exception& e) {
			retVal = e.hr;
		}
		catch(DecoratorException& e) {
			retVal = e.GetHResult();
		}
	}
	if (retVal == S_OK || retVal == S_DECORATOR_EVENT_NOT_HANDLED || retVal == E_DECORATOR_NOT_IMPLEMENTED) {
		for (std::vector<DecoratorSDK::PartInterface*>::iterator ii = m_AttributeParts.begin(); ii != m_AttributeParts.end(); ++ii) {
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
	}

	return false;
}

bool CPMBasePart::MouseMiddleButtonDoubleClick(UINT nFlags, const CPoint& point, HDC transformHDC)
{
	HRESULT retVal = S_OK;
	try {
		if (VectorPart::MouseMiddleButtonDoubleClick(nFlags, point, transformHDC))
			return true;
	}
	catch(hresult_exception& e) {
		retVal = e.hr;
	}
	catch(DecoratorException& e) {
		retVal = e.GetHResult();
	}
	if (m_LabelPart != NULL && (retVal == S_OK || retVal == S_DECORATOR_EVENT_NOT_HANDLED || retVal == E_DECORATOR_NOT_IMPLEMENTED)) {
		try {
			if (m_LabelPart->MouseMiddleButtonDoubleClick(nFlags, point, transformHDC))
				return true;
		}
		catch(hresult_exception& e) {
			retVal = e.hr;
		}
		catch(DecoratorException& e) {
			retVal = e.GetHResult();
		}
	}
	if (retVal == S_OK || retVal == S_DECORATOR_EVENT_NOT_HANDLED || retVal == E_DECORATOR_NOT_IMPLEMENTED) {
		for (std::vector<DecoratorSDK::PartInterface*>::iterator ii = m_AttributeParts.begin(); ii != m_AttributeParts.end(); ++ii) {
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
	}

	return false;
}

bool CPMBasePart::MouseWheelTurned(UINT nFlags, short distance, const CPoint& point, HDC transformHDC)
{
	HRESULT retVal = S_OK;
	try {
		if (VectorPart::MouseWheelTurned(nFlags, distance, point, transformHDC))
			return true;
	}
	catch(hresult_exception& e) {
		retVal = e.hr;
	}
	catch(DecoratorException& e) {
		retVal = e.GetHResult();
	}
	if (m_LabelPart != NULL && (retVal == S_OK || retVal == S_DECORATOR_EVENT_NOT_HANDLED || retVal == E_DECORATOR_NOT_IMPLEMENTED)) {
		try {
			if (m_LabelPart->MouseWheelTurned(nFlags, distance, point, transformHDC))
				return true;
		}
		catch(hresult_exception& e) {
			retVal = e.hr;
		}
		catch(DecoratorException& e) {
			retVal = e.GetHResult();
		}
	}
	if (retVal == S_OK || retVal == S_DECORATOR_EVENT_NOT_HANDLED || retVal == E_DECORATOR_NOT_IMPLEMENTED) {
		for (std::vector<DecoratorSDK::PartInterface*>::iterator ii = m_AttributeParts.begin(); ii != m_AttributeParts.end(); ++ii) {
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
	}

	return false;
}

bool CPMBasePart::DragEnter(DROPEFFECT* dropEffect, COleDataObject* pDataObject, DWORD dwKeyState, const CPoint& point, HDC transformHDC)
{
	HRESULT retVal = S_OK;
	try {
		if (VectorPart::DragEnter(dropEffect, pDataObject, dwKeyState, point, transformHDC))
			return true;
	}
	catch(hresult_exception& e) {
		retVal = e.hr;
	}
	catch(DecoratorException& e) {
		retVal = e.GetHResult();
	}
	if (m_LabelPart != NULL && (retVal == S_OK || retVal == S_DECORATOR_EVENT_NOT_HANDLED || retVal == E_DECORATOR_NOT_IMPLEMENTED)) {
		try {
			if (m_LabelPart->DragEnter(dropEffect, pDataObject, dwKeyState, point, transformHDC))
				return true;
		}
		catch(hresult_exception& e) {
			retVal = e.hr;
		}
		catch(DecoratorException& e) {
			retVal = e.GetHResult();
		}
	}
	if (retVal == S_OK || retVal == S_DECORATOR_EVENT_NOT_HANDLED || retVal == E_DECORATOR_NOT_IMPLEMENTED) {
		for (std::vector<DecoratorSDK::PartInterface*>::iterator ii = m_AttributeParts.begin(); ii != m_AttributeParts.end(); ++ii) {
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
	}

	return false;
}

bool CPMBasePart::DragOver(DROPEFFECT* dropEffect, COleDataObject* pDataObject, DWORD dwKeyState, const CPoint& point, HDC transformHDC)
{
	HRESULT retVal = S_OK;
	try {
		if (VectorPart::DragOver(dropEffect, pDataObject, dwKeyState, point, transformHDC))
			return true;
	}
	catch(hresult_exception& e) {
		retVal = e.hr;
	}
	catch(DecoratorException& e) {
		retVal = e.GetHResult();
	}
	if (m_LabelPart != NULL && (retVal == S_OK || retVal == S_DECORATOR_EVENT_NOT_HANDLED || retVal == E_DECORATOR_NOT_IMPLEMENTED)) {
		try {
			if (m_LabelPart->DragOver(dropEffect, pDataObject, dwKeyState, point, transformHDC))
				return true;
		}
		catch(hresult_exception& e) {
			retVal = e.hr;
		}
		catch(DecoratorException& e) {
			retVal = e.GetHResult();
		}
	}
	if (retVal == S_OK || retVal == S_DECORATOR_EVENT_NOT_HANDLED || retVal == E_DECORATOR_NOT_IMPLEMENTED) {
		for (std::vector<DecoratorSDK::PartInterface*>::iterator ii = m_AttributeParts.begin(); ii != m_AttributeParts.end(); ++ii) {
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
	}

	return false;
}

bool CPMBasePart::Drop(COleDataObject* pDataObject, DROPEFFECT dropEffect, const CPoint& point, HDC transformHDC)
{
	HRESULT retVal = S_OK;
	try {
		if (VectorPart::Drop(pDataObject, dropEffect, point, transformHDC))
			return true;
	}
	catch(hresult_exception& e) {
		retVal = e.hr;
	}
	catch(DecoratorException& e) {
		retVal = e.GetHResult();
	}
	if (m_LabelPart != NULL && (retVal == S_OK || retVal == S_DECORATOR_EVENT_NOT_HANDLED || retVal == E_DECORATOR_NOT_IMPLEMENTED)) {
		try {
			if (m_LabelPart->Drop(pDataObject, dropEffect, point, transformHDC))
				return true;
		}
		catch(hresult_exception& e) {
			retVal = e.hr;
		}
		catch(DecoratorException& e) {
			retVal = e.GetHResult();
		}
	}
	if (retVal == S_OK || retVal == S_DECORATOR_EVENT_NOT_HANDLED || retVal == E_DECORATOR_NOT_IMPLEMENTED) {
		for (std::vector<DecoratorSDK::PartInterface*>::iterator ii = m_AttributeParts.begin(); ii != m_AttributeParts.end(); ++ii) {
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
	}

	return false;
}

bool CPMBasePart::DropFile(HDROP p_hDropInfo, const CPoint& point, HDC transformHDC)
{
	HRESULT retVal = S_OK;
	try {
		if (VectorPart::DropFile(p_hDropInfo, point, transformHDC))
			return true;
	}
	catch(hresult_exception& e) {
		retVal = e.hr;
	}
	catch(DecoratorException& e) {
		retVal = e.GetHResult();
	}
	if (m_LabelPart != NULL && (retVal == S_OK || retVal == S_DECORATOR_EVENT_NOT_HANDLED || retVal == E_DECORATOR_NOT_IMPLEMENTED)) {
		try {
			if (m_LabelPart->DropFile(p_hDropInfo, point, transformHDC))
				return true;
		}
		catch(hresult_exception& e) {
			retVal = e.hr;
		}
		catch(DecoratorException& e) {
			retVal = e.GetHResult();
		}
	}
	if (retVal == S_OK || retVal == S_DECORATOR_EVENT_NOT_HANDLED || retVal == E_DECORATOR_NOT_IMPLEMENTED) {
		for (std::vector<DecoratorSDK::PartInterface*>::iterator ii = m_AttributeParts.begin(); ii != m_AttributeParts.end(); ++ii) {
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
	}

	return false;
}

bool CPMBasePart::MenuItemSelected(UINT menuItemId, UINT nFlags, const CPoint& point, HDC transformHDC)
{
	HRESULT retVal = S_OK;
	try {
		if (VectorPart::MenuItemSelected(menuItemId, nFlags, point, transformHDC))
			return true;
	}
	catch(hresult_exception& e) {
		retVal = e.hr;
	}
	catch(DecoratorException& e) {
		retVal = e.GetHResult();
	}
	if (m_LabelPart != NULL && (retVal == S_OK || retVal == S_DECORATOR_EVENT_NOT_HANDLED || retVal == E_DECORATOR_NOT_IMPLEMENTED)) {
		try {
			if (m_LabelPart->MenuItemSelected(menuItemId, nFlags, point, transformHDC))
				return true;
		}
		catch(hresult_exception& e) {
			retVal = e.hr;
		}
		catch(DecoratorException& e) {
			retVal = e.GetHResult();
		}
	}
	if (retVal == S_OK || retVal == S_DECORATOR_EVENT_NOT_HANDLED || retVal == E_DECORATOR_NOT_IMPLEMENTED) {
		for (std::vector<DecoratorSDK::PartInterface*>::iterator ii = m_AttributeParts.begin(); ii != m_AttributeParts.end(); ++ii) {
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
	}

	return false;
}

bool CPMBasePart::OperationCanceledByGME(void)
{
	HRESULT retVal = S_OK;
	try {
		if (VectorPart::OperationCanceledByGME())
			return true;
	}
	catch(hresult_exception& e) {
		retVal = e.hr;
	}
	catch(DecoratorException& e) {
		retVal = e.GetHResult();
	}
	if (m_LabelPart != NULL && (retVal == S_OK || retVal == S_DECORATOR_EVENT_NOT_HANDLED || retVal == E_DECORATOR_NOT_IMPLEMENTED)) {
		try {
			if (m_LabelPart->OperationCanceledByGME())
				return true;
		}
		catch(hresult_exception& e) {
			retVal = e.hr;
		}
		catch(DecoratorException& e) {
			retVal = e.GetHResult();
		}
	}
	if (retVal == S_OK || retVal == S_DECORATOR_EVENT_NOT_HANDLED || retVal == E_DECORATOR_NOT_IMPLEMENTED) {
		for (std::vector<DecoratorSDK::PartInterface*>::iterator ii = m_AttributeParts.begin(); ii != m_AttributeParts.end(); ++ii) {
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
	}

	return false;
}

void CPMBasePart::CalcRelPositions(CDC* pDC, Gdiplus::Graphics* gdip)
{
	m_calcSize.cx = 0;
	m_calcSize.cy = CPM_DECORATOR_MARGINY;

	// Calculate positions of m_InternalParts as well as overall box size
	for (std::vector<std::vector<IInternalPart*> >::iterator i = m_InternalParts.begin(); i != m_InternalParts.end(); ++i) {
		m_lMaxTextWidth		= 0;
		m_lMaxTextHeight	= 0;
		m_lMinTextWidth		= LONG_MAX;
		m_lMinTextHeight	= LONG_MAX;
		long width = CPM_DECORATOR_MARGINX;

		for (std::vector<IInternalPart*>::iterator ii = i->begin(); ii != i->end(); ++ii) {
			CSize extent = (*ii)->GetRelSize(pDC, gdip);
			m_lMaxTextWidth = max(m_lMaxTextWidth, extent.cx);
			m_lMaxTextHeight = max(m_lMaxTextHeight, extent.cy);
			width += extent.cx;
		}
		m_calcSize.cy += m_lMaxTextHeight;
		m_calcSize.cx = max(m_calcSize.cx, width);

		long x = CPM_DECORATOR_MARGINX;
		for (std::vector<IInternalPart*>::iterator ii = i->begin(); ii != i->end(); ++ii) {
			(*ii)->SetTextRelYPosition(m_calcSize.cy);
			(*ii)->SetTextRelXPosition(x);
			CSize extent = (*ii)->GetRelSize(pDC, gdip);
			x += extent.cx - 3;
		}
		m_calcSize.cy += CPM_DECORATOR_GAPY;
	}

	// Ensure the box is as big as the label, so the label isn't rendered as "CPM::...n"
	if (m_LabelPart != NULL) {
		CRect extent = m_LabelPart->GetTextLocation(pDC, gdip);
		m_calcSize.cx = max(m_calcSize.cx, extent.Width());
	}

	m_calcSize.cx += CPM_DECORATOR_MARGINX;
	m_calcSize.cy += CPM_DECORATOR_MARGINY;
	m_calcSize.cy = ((m_calcSize.cy + GME_GRID_SIZE - 1) / GME_GRID_SIZE) * GME_GRID_SIZE;
	//m_calcSize.cx += CPM_DECORATOR_BORDERSIZE;
	//m_calcSize.cy += CPM_DECORATOR_BORDERSIZE;
	if (m_coordCommands.size() > 0) {
	} else {
		AddBorderCommands(pDC, gdip);
	}

	if (m_LabelPart != NULL) {
		CRect labelLoc = m_LabelPart->GetTextLocation(pDC, gdip);
		m_LabelPart->SetTextRelYPosition(m_calcSize.cy + labelLoc.Height());
	}

}

void CPMBasePart::AddBorderCommands(CDC* pDC, Gdiplus::Graphics* gdip)
{
	// Thin black border
	SimpleCoordCommand* leftMost		= new SimpleCoordCommand(LeftMost);
	SimpleCoordCommand* topMost			= new SimpleCoordCommand(TopMost);
	SimpleCoordCommand* rightMost		= new SimpleCoordCommand(RightMost);
	SimpleCoordCommand* bottomMost		= new SimpleCoordCommand(BottomMost);

	m_coordCommands.push_back(leftMost);
	m_coordCommands.push_back(topMost);
	m_coordCommands.push_back(rightMost);
	m_coordCommands.push_back(bottomMost);

	std::vector<const CoordCommand*> m_roundRectangleParams;
	m_roundRectangleParams.push_back(leftMost);
	m_roundRectangleParams.push_back(topMost);
	m_roundRectangleParams.push_back(rightMost);
	m_roundRectangleParams.push_back(bottomMost);
	AddCommand(VectorCommand(m_roundRectangleParams, VectorCommand::DrawRectangle));

	// Thick grey border
	AbsoluteCoordCommand* grayedColorCmd	= new AbsoluteCoordCommand(m_backgroundColor);
	m_coordCommands.push_back(grayedColorCmd);
	AddCommand(VectorCommand(grayedColorCmd, grayedColorCmd, VectorCommand::SelectPen));
	AddCommand(VectorCommand(grayedColorCmd, grayedColorCmd, VectorCommand::SelectBrush));

	RelativeCoordCommand* rightOffset		= new RelativeCoordCommand(RelativeCoordCommand::RIGHT, -CPM_DECORATOR_BORDERSIZE);
	m_coordCommands.push_back(rightOffset);
	RelativeCoordCommand* leftOffset		= new RelativeCoordCommand(RelativeCoordCommand::LEFT, CPM_DECORATOR_BORDERSIZE);
	m_coordCommands.push_back(leftOffset);
	RelativeCoordCommand* topOffset			= new RelativeCoordCommand(RelativeCoordCommand::TOP, CPM_DECORATOR_BORDERSIZE);
	m_coordCommands.push_back(topOffset);
	RelativeCoordCommand* bottomOffset		= new RelativeCoordCommand(RelativeCoordCommand::BOTTOM, -CPM_DECORATOR_BORDERSIZE);
	m_coordCommands.push_back(bottomOffset);
	std::vector<const CoordCommand*> polyParams;
	polyParams.push_back(rightMost);
	polyParams.push_back(topMost);

	polyParams.push_back(rightOffset);
	polyParams.push_back(topOffset);

	polyParams.push_back(rightOffset);
	polyParams.push_back(bottomOffset);

	polyParams.push_back(leftOffset);
	polyParams.push_back(bottomOffset);

	polyParams.push_back(leftMost);
	polyParams.push_back(bottomMost);

	polyParams.push_back(rightMost);
	polyParams.push_back(bottomMost);

	AddCommand(VectorCommand(polyParams, VectorCommand::FillPolygon));
}

void CPMBasePart::SetBoxLocation(const CRect& cRect)
{
	VectorPart::SetBoxLocation(cRect);
	CalcRelPositions();
}

void CPMBasePart::SetReferenced(bool referenced)
{
	VectorPart::SetReferenced(referenced);
}

void CPMBasePart::SetParentPart(PartBase* pPart)
{
	VectorPart::SetParentPart(pPart);
}

void CPMBasePart::NewInternalPartRow() {
	m_InternalParts.push_back(std::vector<IInternalPart*>());
}

void CPMBasePart::AddInternalPart(IInternalPart* part1, PartInterface* part2) {
	m_InternalParts.back().push_back(part1);
	m_AttributeParts.push_back(part2);
}

void CPMBasePart::AddInternalPart(IInternalPart* part) {
	m_InternalParts.back().push_back(part);
}

void CPMBasePart::AddBottomLabel(CComPtr<IMgaFCO>& pFCO, CComPtr<IMgaProject>& pProject, CComPtr<IMgaMetaPart>& pPart,
								 HWND& parentWnd, DecoratorSDK::PreferenceMap& preferences) {
	CComBSTR bstr;
	COMTHROW(pFCO->get_Name(&bstr));
    CString textStr = bstr;

	m_LabelPart = new DecoratorSDK::ClassLabelPart(this, m_eventSink, false);
	m_LabelPart->SetText(textStr);
	m_LabelPart->InitializeEx(pProject, pPart, pFCO, parentWnd, preferences);
}

}; // namespace Decor
