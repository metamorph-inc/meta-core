//################################################################################################
//
// Class complex part class (decorator part)
//	ClassComplexPart.cpp
//
//################################################################################################

#include "StdAfx.h"
#include "ClassComplexPart.h"
#include "DecoratorExceptions.h"
#include <algorithm>


namespace DecoratorSDK {

//################################################################################################
//
// CLASS : ClassComplexPart
//
//################################################################################################

ClassComplexPart::ClassComplexPart(PartBase* pPart, CComPtr<IMgaCommonDecoratorEvents>& eventSink):
	VectorPart					(pPart, eventSink),

	m_LabelPart					(NULL),
	m_StereotypePart			(NULL),
	m_copySignPart				(NULL),

	m_crAttributeText			(COLOR_BLACK),
	m_bRoundCornerRect			(false),
	m_bRoundCornerRadius		(9),
	m_lMaxTextWidth				(0),
	m_lMaxTextHeight			(0),
	m_lMinTextWidth				(LONG_MAX),
	m_lMinTextHeight			(LONG_MAX),

	m_DecoratorMarginX			(DECORATOR_MARGINX),
	m_DecoratorMarginY			(DECORATOR_MARGINY),
	m_DecoratorGapY				(DECORATOR_GAPY),
	m_DecoratorMinAttrSize		(DECORATOR_MINATTRSIZE)
{
	m_calcSize.SetSize(0, 0);
}

ClassComplexPart::~ClassComplexPart()
{
	if (m_LabelPart != NULL)
		delete m_LabelPart;
	if (m_StereotypePart != NULL)
		delete m_StereotypePart;
	unsigned long i = 0;
	for (i = 0; i < m_AttributeParts.size(); i++) {
		delete m_AttributeParts[i];
	}
	m_AttributeParts.clear();
	if (m_copySignPart != NULL)
		delete m_copySignPart;
}

void ClassComplexPart::Initialize(CComPtr<IMgaProject>& pProject, CComPtr<IMgaMetaPart>& pPart, CComPtr<IMgaFCO>& pFCO)
{
	if (m_LabelPart != NULL)
		m_LabelPart->Initialize(pProject, pPart, pFCO);
	if (m_StereotypePart != NULL)
		m_StereotypePart->Initialize(pProject, pPart, pFCO);
	for (std::vector<AttributePart*>::iterator ii = m_AttributeParts.begin(); ii != m_AttributeParts.end(); ++ii) {
		(*ii)->Initialize(pProject, pPart, pFCO);
	}
	if (m_copySignPart != NULL)
		m_copySignPart->Initialize(pProject, pPart, pFCO);
	VectorPart::Initialize(pProject, pPart, pFCO);
}

void ClassComplexPart::Destroy()
{
	if (m_LabelPart != NULL)
		m_LabelPart->Destroy();
	if (m_StereotypePart != NULL)
		m_StereotypePart->Destroy();
	for (std::vector<AttributePart*>::iterator ii = m_AttributeParts.begin(); ii != m_AttributeParts.end(); ++ii) {
		(*ii)->Destroy();
	}
	if (m_copySignPart != NULL)
		m_copySignPart->Destroy();
	VectorPart::Destroy();
}

CString ClassComplexPart::GetMnemonic(void) const
{
	return VectorPart::GetMnemonic();
}

feature_code ClassComplexPart::GetFeatures(void) const
{
	feature_code featureCodes = 0;

	if (m_LabelPart != NULL) {
		feature_code labelFeatureCodes = m_LabelPart->GetFeatures();
		featureCodes |= labelFeatureCodes;
	}
	if (m_StereotypePart != NULL) {
		feature_code stereotypeFeatureCodes = m_StereotypePart->GetFeatures();
		featureCodes |= stereotypeFeatureCodes;
	}

	for (std::vector<AttributePart*>::const_iterator ii = m_AttributeParts.begin(); ii != m_AttributeParts.end(); ++ii) {
		feature_code attrubuteFeatureCodes = (*ii)->GetFeatures();
		featureCodes |= attrubuteFeatureCodes;
	}

	feature_code partFeatureCodes = VectorPart::GetFeatures();
	featureCodes |= partFeatureCodes;

	return featureCodes;
}

void ClassComplexPart::SetParam(const CString& strName, VARIANT vValue)
{
	if (m_LabelPart != NULL)
		m_LabelPart->SetParam(strName, vValue);
	if (m_StereotypePart != NULL)
		m_StereotypePart->SetParam(strName, vValue);
	for (std::vector<AttributePart*>::iterator ii = m_AttributeParts.begin(); ii != m_AttributeParts.end(); ++ii) {
		(*ii)->SetParam(strName, vValue);
	}
	if (m_copySignPart != NULL)
		m_copySignPart->SetParam(strName, vValue);
	VectorPart::SetParam(strName, vValue);
}

bool ClassComplexPart::GetParam(const CString& strName, VARIANT* pvValue)
{
	try {
		if (VectorPart::GetParam(strName, pvValue))
			return true;
	}
	catch(UnknownParameterException&) {
	}
	return false;
}

void ClassComplexPart::SetActive(bool bIsActive)
{
	if (m_LabelPart != NULL)
		m_LabelPart->SetActive(bIsActive);
	if (m_StereotypePart != NULL)
		m_StereotypePart->SetActive(bIsActive);
	for (std::vector<AttributePart*>::iterator ii = m_AttributeParts.begin(); ii != m_AttributeParts.end(); ++ii) {
		(*ii)->SetActive(bIsActive);
	}
	if (m_copySignPart != NULL)
		m_copySignPart->SetActive(bIsActive);
	VectorPart::SetActive(bIsActive);
}

CSize ClassComplexPart::GetPreferredSize(void) const
{
	CSize size = ResizablePart::GetPreferredSize();
	if (size.cx * size.cy != 0)
		return size;

	ASSERT(m_calcSize.cx > 0 && m_calcSize.cy > 0);
	return m_calcSize;
}

void ClassComplexPart::SetLocation(const CRect& location)
{
	if (m_LabelPart != NULL)
		m_LabelPart->SetLocation(location);
	if (m_StereotypePart != NULL)
		m_StereotypePart->SetLocation(location);
	for (std::vector<AttributePart*>::iterator ii = m_AttributeParts.begin(); ii != m_AttributeParts.end(); ++ii) {
		(*ii)->SetLocation(location);
	}
	VectorPart::SetLocation(location);
}

CRect ClassComplexPart::GetLocation(void) const
{
	return VectorPart::GetLocation();
}

CRect ClassComplexPart::GetLabelLocation(void) const
{
	CRect labelLocation;
	try {
		labelLocation = m_LabelPart->GetLabelLocation();
	}
	catch(NotImplementedException&) {
	}
	return labelLocation;
}

void ClassComplexPart::Draw(CDC* pDC, Gdiplus::Graphics* gdip)
{
	// KMS: we don't CalcRelPositions here, as our size is already set, and Draw from DumpWindowsMetaFile calculates
	// different positions than in InitializeEx
	//CalcRelPositions(pDC, gdip);
	VectorPart::Draw(pDC, gdip);
	if (m_LabelPart != NULL)
		m_LabelPart->Draw(pDC, gdip);
	if (m_StereotypePart != NULL)
		m_StereotypePart->Draw(pDC, gdip);
	for (std::vector<AttributePart*>::iterator ii = m_AttributeParts.begin(); ii != m_AttributeParts.end(); ++ii) {
		(*ii)->Draw(pDC, gdip);
	}
	if (m_copySignPart != NULL)
		m_copySignPart->Draw(pDC, gdip);
}

void ClassComplexPart::SaveState()
{
	if (m_LabelPart != NULL)
		m_LabelPart->SaveState();
	if (m_StereotypePart != NULL)
		m_StereotypePart->SaveState();
	for (std::vector<AttributePart*>::iterator ii = m_AttributeParts.begin(); ii != m_AttributeParts.end(); ++ii) {
		(*ii)->SaveState();
	}
	if (m_copySignPart != NULL)
		m_copySignPart->SaveState();
	VectorPart::SaveState();
}

// New functions
void ClassComplexPart::InitializeEx(CComPtr<IMgaProject>& pProject, CComPtr<IMgaMetaPart>& pPart, CComPtr<IMgaFCO>& pFCO,
									HWND parentWnd, PreferenceMap& preferences)
{
	PreferenceMap::iterator it;

	it = preferences.find(PREF_DECORATOR_MARGINX);
	if (it != preferences.end())
		m_DecoratorMarginX = it->second.uValue.lValue;
	it = preferences.find(PREF_DECORATOR_MARGINY);
	if (it != preferences.end())
		m_DecoratorMarginY = it->second.uValue.lValue;
	it = preferences.find(PREF_DECORATOR_GAPY);
	if (it != preferences.end())
		m_DecoratorGapY = it->second.uValue.lValue;
	it = preferences.find(PREF_DECORATOR_MINATTRSIZE);
	if (it != preferences.end())
		m_DecoratorMinAttrSize = it->second.uValue.lValue;

	if (m_LabelPart != NULL)
		m_LabelPart->InitializeEx(pProject, pPart, pFCO, parentWnd, preferences);
	if (m_StereotypePart != NULL)
		m_StereotypePart->InitializeEx(pProject, pPart, pFCO, parentWnd, preferences);
	for (std::vector<AttributePart*>::iterator ii = m_AttributeParts.begin(); ii != m_AttributeParts.end(); ++ii) {
		(*ii)->InitializeEx(pProject, pPart, pFCO, parentWnd, preferences);
	}
	if (m_copySignPart != NULL)
		m_copySignPart->InitializeEx(pProject, pPart, pFCO, parentWnd, preferences);
	VectorPart::InitializeEx(pProject, pPart, pFCO, parentWnd, preferences);

	m_bRoundCornerRect = false;
	it = preferences.find(PREF_ROUNDCORNERRECT);
	if (it != preferences.end()) {
		m_bRoundCornerRect = it->second.uValue.bValue;
	} else {
		getFacilities().getPreference(m_spFCO, m_spMetaFCO, PREF_ROUNDCORNERRECT, m_bRoundCornerRect);
	}

	m_bRoundCornerRadius = 9;
	it = preferences.find(PREF_ROUNDCORNERRADIUS);
	if (it != preferences.end()) {
		m_bRoundCornerRadius = it->second.uValue.bValue;
	} else {
		getFacilities().getPreference(m_spFCO, m_spMetaFCO, PREF_ROUNDCORNERRADIUS, m_bRoundCornerRadius, false);
	}
}

void ClassComplexPart::SetSelected(bool bIsSelected)
{
	if (m_LabelPart != NULL)
		m_LabelPart->SetSelected(bIsSelected);
	if (m_StereotypePart != NULL)
		m_StereotypePart->SetSelected(bIsSelected);
	for (std::vector<AttributePart*>::iterator ii = m_AttributeParts.begin(); ii != m_AttributeParts.end(); ++ii) {
		(*ii)->SetSelected(bIsSelected);
	}
	if (m_copySignPart != NULL)
		m_copySignPart->SetSelected(bIsSelected);
	VectorPart::SetSelected(bIsSelected);
}

bool ClassComplexPart::MouseMoved(UINT nFlags, const CPoint& point, HDC transformHDC)
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
	if (m_StereotypePart != NULL && (retVal == S_OK || retVal == S_DECORATOR_EVENT_NOT_HANDLED || retVal == E_DECORATOR_NOT_IMPLEMENTED)) {
		try {
			if (m_StereotypePart->MouseMoved(nFlags, point, transformHDC))
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
		for (std::vector<AttributePart*>::iterator ii = m_AttributeParts.begin(); ii != m_AttributeParts.end(); ++ii) {
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

bool ClassComplexPart::MouseLeftButtonDown(UINT nFlags, const CPoint& point, HDC transformHDC)
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
	if (m_StereotypePart != NULL && (retVal == S_OK || retVal == S_DECORATOR_EVENT_NOT_HANDLED || retVal == E_DECORATOR_NOT_IMPLEMENTED)) {
		try {
			if (m_StereotypePart->MouseLeftButtonDown(nFlags, point, transformHDC))
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
		for (std::vector<AttributePart*>::iterator ii = m_AttributeParts.begin(); ii != m_AttributeParts.end(); ++ii) {
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

bool ClassComplexPart::MouseLeftButtonUp(UINT nFlags, const CPoint& point, HDC transformHDC)
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
	if (m_StereotypePart != NULL && (retVal == S_OK || retVal == S_DECORATOR_EVENT_NOT_HANDLED || retVal == E_DECORATOR_NOT_IMPLEMENTED)) {
		try {
			if (m_StereotypePart->MouseLeftButtonUp(nFlags, point, transformHDC))
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
		for (std::vector<AttributePart*>::iterator ii = m_AttributeParts.begin(); ii != m_AttributeParts.end(); ++ii) {
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

bool ClassComplexPart::MouseLeftButtonDoubleClick(UINT nFlags, const CPoint& point, HDC transformHDC)
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
	if (m_StereotypePart != NULL && (retVal == S_OK || retVal == S_DECORATOR_EVENT_NOT_HANDLED || retVal == E_DECORATOR_NOT_IMPLEMENTED)) {
		try {
			if (m_StereotypePart->MouseLeftButtonDoubleClick(nFlags, point, transformHDC))
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
		for (std::vector<AttributePart*>::iterator ii = m_AttributeParts.begin(); ii != m_AttributeParts.end(); ++ii) {
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

bool ClassComplexPart::MouseRightButtonDown(HMENU hCtxMenu, UINT nFlags, const CPoint& point, HDC transformHDC)
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
	if (m_StereotypePart != NULL && (retVal == S_OK || retVal == S_DECORATOR_EVENT_NOT_HANDLED || retVal == E_DECORATOR_NOT_IMPLEMENTED)) {
		try {
			if (m_StereotypePart->MouseRightButtonDown(hCtxMenu, nFlags, point, transformHDC))
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
		for (std::vector<AttributePart*>::iterator ii = m_AttributeParts.begin(); ii != m_AttributeParts.end(); ++ii) {
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

bool ClassComplexPart::MouseRightButtonUp(UINT nFlags, const CPoint& point, HDC transformHDC)
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
	if (m_StereotypePart != NULL && (retVal == S_OK || retVal == S_DECORATOR_EVENT_NOT_HANDLED || retVal == E_DECORATOR_NOT_IMPLEMENTED)) {
		try {
			if (m_StereotypePart->MouseRightButtonUp(nFlags, point, transformHDC))
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
		for (std::vector<AttributePart*>::iterator ii = m_AttributeParts.begin(); ii != m_AttributeParts.end(); ++ii) {
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

bool ClassComplexPart::MouseRightButtonDoubleClick(UINT nFlags, const CPoint& point, HDC transformHDC)
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
	if (m_StereotypePart != NULL && (retVal == S_OK || retVal == S_DECORATOR_EVENT_NOT_HANDLED || retVal == E_DECORATOR_NOT_IMPLEMENTED)) {
		try {
			if (m_StereotypePart->MouseRightButtonDoubleClick(nFlags, point, transformHDC))
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
		for (std::vector<AttributePart*>::iterator ii = m_AttributeParts.begin(); ii != m_AttributeParts.end(); ++ii) {
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

bool ClassComplexPart::MouseMiddleButtonDown(UINT nFlags, const CPoint& point, HDC transformHDC)
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
	if (m_StereotypePart != NULL && (retVal == S_OK || retVal == S_DECORATOR_EVENT_NOT_HANDLED || retVal == E_DECORATOR_NOT_IMPLEMENTED)) {
		try {
			if (m_StereotypePart->MouseMiddleButtonDown(nFlags, point, transformHDC))
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
		for (std::vector<AttributePart*>::iterator ii = m_AttributeParts.begin(); ii != m_AttributeParts.end(); ++ii) {
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

bool ClassComplexPart::MouseMiddleButtonUp(UINT nFlags, const CPoint& point, HDC transformHDC)
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
	if (m_StereotypePart != NULL && (retVal == S_OK || retVal == S_DECORATOR_EVENT_NOT_HANDLED || retVal == E_DECORATOR_NOT_IMPLEMENTED)) {
		try {
			if (m_StereotypePart->MouseMiddleButtonUp(nFlags, point, transformHDC))
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
		for (std::vector<AttributePart*>::iterator ii = m_AttributeParts.begin(); ii != m_AttributeParts.end(); ++ii) {
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

bool ClassComplexPart::MouseMiddleButtonDoubleClick(UINT nFlags, const CPoint& point, HDC transformHDC)
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
	if (m_StereotypePart != NULL && (retVal == S_OK || retVal == S_DECORATOR_EVENT_NOT_HANDLED || retVal == E_DECORATOR_NOT_IMPLEMENTED)) {
		try {
			if (m_StereotypePart->MouseMiddleButtonDoubleClick(nFlags, point, transformHDC))
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
		for (std::vector<AttributePart*>::iterator ii = m_AttributeParts.begin(); ii != m_AttributeParts.end(); ++ii) {
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

bool ClassComplexPart::MouseWheelTurned(UINT nFlags, short distance, const CPoint& point, HDC transformHDC)
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
	if (m_StereotypePart != NULL && (retVal == S_OK || retVal == S_DECORATOR_EVENT_NOT_HANDLED || retVal == E_DECORATOR_NOT_IMPLEMENTED)) {
		try {
			if (m_StereotypePart->MouseWheelTurned(nFlags, distance, point, transformHDC))
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
		for (std::vector<AttributePart*>::iterator ii = m_AttributeParts.begin(); ii != m_AttributeParts.end(); ++ii) {
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

bool ClassComplexPart::DragEnter(DROPEFFECT* dropEffect, COleDataObject* pDataObject, DWORD dwKeyState, const CPoint& point, HDC transformHDC)
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
	if (m_StereotypePart != NULL && (retVal == S_OK || retVal == S_DECORATOR_EVENT_NOT_HANDLED || retVal == E_DECORATOR_NOT_IMPLEMENTED)) {
		try {
			if (m_StereotypePart->DragEnter(dropEffect, pDataObject, dwKeyState, point, transformHDC))
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
		for (std::vector<AttributePart*>::iterator ii = m_AttributeParts.begin(); ii != m_AttributeParts.end(); ++ii) {
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

bool ClassComplexPart::DragOver(DROPEFFECT* dropEffect, COleDataObject* pDataObject, DWORD dwKeyState, const CPoint& point, HDC transformHDC)
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
	if (m_StereotypePart != NULL && (retVal == S_OK || retVal == S_DECORATOR_EVENT_NOT_HANDLED || retVal == E_DECORATOR_NOT_IMPLEMENTED)) {
		try {
			if (m_StereotypePart->DragOver(dropEffect, pDataObject, dwKeyState, point, transformHDC))
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
		for (std::vector<AttributePart*>::iterator ii = m_AttributeParts.begin(); ii != m_AttributeParts.end(); ++ii) {
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

bool ClassComplexPart::Drop(COleDataObject* pDataObject, DROPEFFECT dropEffect, const CPoint& point, HDC transformHDC)
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
	if (m_StereotypePart != NULL && (retVal == S_OK || retVal == S_DECORATOR_EVENT_NOT_HANDLED || retVal == E_DECORATOR_NOT_IMPLEMENTED)) {
		try {
			if (m_StereotypePart->Drop(pDataObject, dropEffect, point, transformHDC))
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
		for (std::vector<AttributePart*>::iterator ii = m_AttributeParts.begin(); ii != m_AttributeParts.end(); ++ii) {
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

bool ClassComplexPart::DropFile(HDROP p_hDropInfo, const CPoint& point, HDC transformHDC)
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
	if (m_StereotypePart != NULL && (retVal == S_OK || retVal == S_DECORATOR_EVENT_NOT_HANDLED || retVal == E_DECORATOR_NOT_IMPLEMENTED)) {
		try {
			if (m_StereotypePart->DropFile(p_hDropInfo, point, transformHDC))
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
		for (std::vector<AttributePart*>::iterator ii = m_AttributeParts.begin(); ii != m_AttributeParts.end(); ++ii) {
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

bool ClassComplexPart::MenuItemSelected(UINT menuItemId, UINT nFlags, const CPoint& point, HDC transformHDC)
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
	if (m_StereotypePart != NULL && (retVal == S_OK || retVal == S_DECORATOR_EVENT_NOT_HANDLED || retVal == E_DECORATOR_NOT_IMPLEMENTED)) {
		try {
			if (m_StereotypePart->MenuItemSelected(menuItemId, nFlags, point, transformHDC))
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
		for (std::vector<AttributePart*>::iterator ii = m_AttributeParts.begin(); ii != m_AttributeParts.end(); ++ii) {
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

bool ClassComplexPart::OperationCanceledByGME(void)
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
	if (m_StereotypePart != NULL && (retVal == S_OK || retVal == S_DECORATOR_EVENT_NOT_HANDLED || retVal == E_DECORATOR_NOT_IMPLEMENTED)) {
		try {
			if (m_StereotypePart->OperationCanceledByGME())
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
		for (std::vector<AttributePart*>::iterator ii = m_AttributeParts.begin(); ii != m_AttributeParts.end(); ++ii) {
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
	if (m_copySignPart != NULL && (retVal == S_OK || retVal == S_DECORATOR_EVENT_NOT_HANDLED || retVal == E_DECORATOR_NOT_IMPLEMENTED)) {
		try {
			if (m_copySignPart->OperationCanceledByGME())
				return true;
		}
		catch(hresult_exception& e) {
			retVal = e.hr;
		}
		catch(DecoratorException& e) {
			retVal = e.GetHResult();
		}
	}

	return false;
}

void ClassComplexPart::CalcRelPositions(CDC* pDC, Gdiplus::Graphics* gdip)
{
	m_lMaxTextWidth			= 0;
	m_lMaxTextHeight		= 0;
	m_lMinTextWidth			= LONG_MAX;
	m_lMinTextHeight		= LONG_MAX;

	long numberOfGaps = 0;
	long heightPreEstimation = m_DecoratorMarginY;
	if (m_LabelPart != NULL) {
		CRect labelLoc = m_LabelPart->GetTextLocation(pDC, gdip);
		m_lMaxTextWidth = max(m_lMaxTextWidth, labelLoc.Width());
		m_lMaxTextHeight = max(m_lMaxTextHeight, labelLoc.Height());
		heightPreEstimation += m_lMaxTextHeight;
	}

	if (m_StereotypePart != NULL) {
		CRect stereoLoc = m_StereotypePart->GetTextLocation(pDC, gdip);
		m_lMaxTextWidth = max(m_lMaxTextWidth, stereoLoc.Width());
		m_lMaxTextHeight = max(m_lMaxTextHeight, stereoLoc.Height());
		numberOfGaps++;
		heightPreEstimation += m_DecoratorGapY;
		heightPreEstimation += m_lMaxTextHeight;
	}
	heightPreEstimation += m_DecoratorMarginY;
	heightPreEstimation++;	// separator
	heightPreEstimation += m_DecoratorMarginY;

	for (std::vector<AttributePart*>::iterator ii = m_AttributeParts.begin(); ii != m_AttributeParts.end(); ++ii) {
		CSize extent = (*ii)->GetTextSize(pDC, gdip);
		m_lMaxTextWidth = max(m_lMaxTextWidth, extent.cx);
		m_lMaxTextHeight = max(m_lMaxTextHeight, extent.cy);
		numberOfGaps++;
		heightPreEstimation += m_lMaxTextHeight;
		heightPreEstimation += m_DecoratorGapY;
	}

	long attribCount = (long)m_AttributeParts.size();
	if (m_copySignPart) {
		heightPreEstimation += m_DecoratorMarginY;
		CSize iconSize = m_copySignPart->GetPreferredSize();
		heightPreEstimation += iconSize.cy;
	} else if (attribCount == 0) {
		heightPreEstimation += m_DecoratorMinAttrSize;
	}
	heightPreEstimation += m_DecoratorMarginY;

	CRect location = GetLocation();
	CPoint offset = GetLocation().TopLeft();

	long stretchedGapSize = m_DecoratorGapY;
	long gapSizeModify = 0;
	if (m_spFCO) {	// stretch logic only should work outside of PartBrowser
		if (!location.IsRectEmpty()) {
			if (location.Height() - heightPreEstimation != 0)
				gapSizeModify = (location.Height() - heightPreEstimation) / (numberOfGaps + (attribCount > 0 ? 3 : 2));
		}
		stretchedGapSize += gapSizeModify;
	}

	int xcenterpos = (2 * m_DecoratorMarginX + m_lMaxTextWidth) / 2; 
	int	xleftpos = m_DecoratorMarginX;
	int	xrightpos = m_DecoratorMarginX + m_lMaxTextWidth;
	int ypos = m_DecoratorMarginY;

	if (m_LabelPart != NULL) {
		ypos += m_lMaxTextHeight;
		ypos += gapSizeModify;

		m_LabelPart->SetTextRelYPosition(ypos);
	}

	if (m_StereotypePart != NULL) {
		ypos += stretchedGapSize;
		ypos += m_lMaxTextHeight;
		if (m_LabelPart == NULL)
			ypos += gapSizeModify;

		m_StereotypePart->SetTextRelYPosition(ypos);
	}

	ypos += m_DecoratorMarginY;
	ypos += gapSizeModify;
	m_SeparatorLoc = ypos + offset.y;

	if (m_coordCommands.size() > 0) {
		ASSERT(GetCommandNumber() >= 1);
		RemoveLastCommand();	// Remove the last commands
		delete m_coordCommands[m_coordCommands.size() - 1];	// Delete the AbsoulteCommand (the separator location)
		m_coordCommands.pop_back();
	} else {
		SimpleCoordCommand* leftMost		= new SimpleCoordCommand(LeftMost);
		SimpleCoordCommand* topMost			= new SimpleCoordCommand(TopMost);
		SimpleCoordCommand* rightMost		= new SimpleCoordCommand(RightMost);
		SimpleCoordCommand* bottomMost		= new SimpleCoordCommand(BottomMost);
		AbsoluteCoordCommand* radiusCommand	= new AbsoluteCoordCommand(m_bRoundCornerRect ? m_bRoundCornerRadius : 0);

		m_coordCommands.push_back(leftMost);
		m_coordCommands.push_back(topMost);
		m_coordCommands.push_back(rightMost);
		m_coordCommands.push_back(bottomMost);
		m_coordCommands.push_back(radiusCommand);

		AddCommand(VectorCommand::BeginPath);
		std::vector<const CoordCommand*> m_roundRectangleParams;
		m_roundRectangleParams.push_back(leftMost);
		m_roundRectangleParams.push_back(topMost);
		m_roundRectangleParams.push_back(rightMost);
		m_roundRectangleParams.push_back(bottomMost);
		m_roundRectangleParams.push_back(radiusCommand);
		AddCommand(VectorCommand(m_roundRectangleParams, VectorCommand::AddRoundRectangleToPath));
		AddCommand(VectorCommand(VectorCommand::EndPath));
		AddCommand(VectorCommand(VectorCommand::CopyShadowPath));
		AddCommand(VectorCommand(VectorCommand::CastShadowPath));

		AbsoluteCoordCommand* fillColorCmd = new AbsoluteCoordCommand(m_crBrush);
		AbsoluteCoordCommand* grayedCmd = new AbsoluteCoordCommand(COLOR_GRAYED_OUT);
		m_coordCommands.push_back(fillColorCmd);
		m_coordCommands.push_back(grayedCmd);
		AddCommand(VectorCommand(fillColorCmd, grayedCmd, VectorCommand::StrokeAndFillPath));
	}

	AbsoluteCoordCommand* sepLocCoordCmd = new AbsoluteCoordCommand(m_SeparatorLoc);
	m_coordCommands.push_back(sepLocCoordCmd);
	AddCommand(VectorCommand(m_coordCommands[0], sepLocCoordCmd, m_coordCommands[2], sepLocCoordCmd, VectorCommand::DrawLine));

	ypos += m_DecoratorMarginY;
	if (m_AttributeParts.size() > 0)
		ypos += gapSizeModify;

	for (std::vector<AttributePart*>::iterator ii = m_AttributeParts.begin(); ii != m_AttributeParts.end(); ++ii) {
		ypos += m_lMaxTextHeight;
		(*ii)->SetTextRelYPosition(ypos);
		ypos += stretchedGapSize;
	}

	if (m_copySignPart) {
		ypos += m_DecoratorMarginY;
		CSize iconSize = m_copySignPart->GetPreferredSize();
		CPoint pos = CPoint(xleftpos, ypos);
		pos.Offset(offset);
		m_copySignPart->SetLocation(CRect(pos, iconSize));
		ypos += iconSize.cy;
	} else if (m_AttributeParts.size() == 0) {
		ypos += m_DecoratorMinAttrSize;
	}
	m_calcSize.cx = xrightpos + m_DecoratorMarginX;
	m_calcSize.cy = ypos + m_DecoratorMarginY;

	resizeLogic.SetMinimumSize(CSize(m_calcSize.cx, heightPreEstimation));
}

void ClassComplexPart::SetBoxLocation(const CRect& cRect)
{
	VectorPart::SetBoxLocation(cRect);
	CalcRelPositions();
}

void ClassComplexPart::SetReferenced(bool referenced)
{
	if (m_LabelPart != NULL)
		m_LabelPart->SetReferenced(referenced);
	if (m_StereotypePart != NULL)
		m_StereotypePart->SetReferenced(referenced);
	for (std::vector<AttributePart*>::iterator ii = m_AttributeParts.begin(); ii != m_AttributeParts.end(); ++ii) {
		(*ii)->SetReferenced(referenced);
	}
	if (m_copySignPart != NULL)
		m_copySignPart->SetReferenced(referenced);
	VectorPart::SetReferenced(referenced);
}

void ClassComplexPart::SetParentPart(PartBase* pPart)
{
	if (m_LabelPart != NULL)
		m_LabelPart->SetParentPart(pPart);
	if (m_StereotypePart != NULL)
		m_StereotypePart->SetParentPart(pPart);
	for (std::vector<AttributePart*>::iterator ii = m_AttributeParts.begin(); ii != m_AttributeParts.end(); ++ii) {
		(*ii)->SetParentPart(pPart);
	}
	if (m_copySignPart != NULL)
		m_copySignPart->SetParentPart(pPart);
	VectorPart::SetParentPart(pPart);
}

struct AttributeLess
{
	bool operator()(AttributePart* papA, AttributePart* papB)
	{
		return papA->IsLesser(papB);
	};
};

void ClassComplexPart::SortAttributes(void)
{
	if (m_AttributeParts.size() < 2)
		return;

	std::sort(m_AttributeParts.begin(), m_AttributeParts.end(), AttributeLess());
}

}; // namespace DecoratorSDK
