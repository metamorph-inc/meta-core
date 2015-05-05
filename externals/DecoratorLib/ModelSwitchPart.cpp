//################################################################################################
//
// Model switch part class (decorator part)
//	ModelSwitchPart.cpp
//
//################################################################################################

#include "StdAfx.h"
#include "ModelSwitchPart.h"

#include "ModelComplexPart.h"
#include "ModelTransparentPart.h"
#include "ModelSwitchButtonPart.h"
#include "DecoratorExceptions.h"
#include <algorithm>


namespace DecoratorSDK {

//################################################################################################
//
// CLASS : ModelSwitchPart
//
//################################################################################################

ModelSwitchPart::ModelSwitchPart(PartBase* pPart, CComPtr<IMgaCommonDecoratorEvents>& eventSink):
	CompositePart	(pPart, eventSink)
{
}

ModelSwitchPart::~ModelSwitchPart()
{
}

void ModelSwitchPart::SetLocation(const CRect& location)
{
	PartBase::SetLocation(location);
	// The first element is the model
	// Teh second is the switch button, we supress this message
}

// New functions
void ModelSwitchPart::InitializeEx(CComPtr<IMgaProject>& pProject, CComPtr<IMgaMetaPart>& pPart, CComPtr<IMgaFCO>& pFCO,
								   HWND parentWnd, PreferenceMap& preferences)
{
	bool bExpanded = false;
	if (pFCO) {
		COMTRY {
			CComPtr<IMgaMetaAspect> mAspect;
			COMTHROW(pPart->get_ParentAspect(&mAspect));

			long status;
			COMTHROW(pProject->get_ProjectStatus(&status));
			bool inTrans = (status & 0x08L) != 0;
			CComPtr<IMgaTerritory> terr;
			CComBSTR bstrVal;
			CComPtr<IMgaPart> part;
			if (!inTrans) {
				COMTHROW(pProject->CreateTerritory(NULL, &terr, 0));
				COMTHROW(pProject->BeginTransaction(terr, TRANSACTION_READ_ONLY));

				CComPtr<IMgaFCO> terrFco;
				COMTHROW(terr->OpenFCO(pFCO, &terrFco));
				status = OBJECT_ZOMBIE;
				COMTHROW(terrFco->get_Status(&status));
				if (status == OBJECT_EXISTS) {
					COMTHROW(terrFco->get_Part(mAspect, &part));
					if (part) {
						CComBSTR regName(PREF_ISMODELEXPANDED);
						COMTHROW(part->get_RegistryValue(regName, &bstrVal));
					}
				}

				pProject->CommitTransaction();
			} else {
				COMTHROW(pFCO->get_Part(mAspect, &part));
				if (part) {
					CComBSTR regName(PREF_ISMODELEXPANDED);
					COMTHROW(part->get_RegistryValue(regName, &bstrVal));
				}
			}

			if (bstrVal == L"1")
				bExpanded = true;
			else
				bExpanded = false;
		}
		catch(hresult_exception &e)
		{
			ASSERT(FAILED(e.hr));
			SetErrorInfo(e.hr);
		}
		preferences[PREF_ISMODELEXPANDED] = PreferenceVariant(bExpanded);
	}

	if (bExpanded)
		AddPart(new ModelTransparentPart(this, m_eventSink));
	else
		AddPart(new ModelComplexPart(this, m_eventSink));
	if (pFCO)
		AddPart(new ModelSwitchButtonPart(this, m_eventSink));

	CompositePart::InitializeEx(pProject, pPart, pFCO, parentWnd, preferences);
}

bool ModelSwitchPart::MouseMoved(UINT nFlags, const CPoint& point, HDC transformHDC)
{
	SwapParts();
	bool retVal = CompositePart::MouseMoved(nFlags, point, transformHDC);
	SwapParts();
	return retVal;
}

bool ModelSwitchPart::MouseLeftButtonDown(UINT nFlags, const CPoint& point, HDC transformHDC)
{
	SwapParts();
	bool retVal = CompositePart::MouseLeftButtonDown(nFlags, point, transformHDC);
	SwapParts();
	return retVal;
}

bool ModelSwitchPart::MouseLeftButtonUp(UINT nFlags, const CPoint& point, HDC transformHDC)
{
	SwapParts();
	bool retVal = CompositePart::MouseLeftButtonUp(nFlags, point, transformHDC);
	SwapParts();
	return retVal;
}

bool ModelSwitchPart::MouseLeftButtonDoubleClick(UINT nFlags, const CPoint& point, HDC transformHDC)
{
	SwapParts();
	bool retVal = CompositePart::MouseLeftButtonDoubleClick(nFlags, point, transformHDC);
	SwapParts();
	return retVal;
}

bool ModelSwitchPart::MouseRightButtonDown(HMENU hCtxMenu, UINT nFlags, const CPoint& point, HDC transformHDC)
{
	SwapParts();
	bool retVal = CompositePart::MouseRightButtonDown(hCtxMenu, nFlags, point, transformHDC);
	SwapParts();
	return retVal;
}

bool ModelSwitchPart::MenuItemSelected(UINT menuItemId, UINT nFlags, const CPoint& point, HDC transformHDC)
{
	SwapParts();
	bool retVal = CompositePart::MenuItemSelected(menuItemId, nFlags, point, transformHDC);
	SwapParts();
	return retVal;
}

void ModelSwitchPart::SetBoxLocation(const CRect& cRect)
{
	m_compositeParts[0]->SetBoxLocation(cRect);
	if (m_spFCO) {
		PartBase* switchButton = m_compositeParts[1];
		CSize iconSize = switchButton->GetPreferredSize();
		CPoint pos = CPoint(cRect.left + (cRect.right - cRect.left) / 2, cRect.top - iconSize.cy / 2);
		switchButton->SetBoxLocation(CRect(pos, iconSize));
	}
}

void ModelSwitchPart::SwapParts(void)
{
	long size = GetSize();
	ASSERT(size == 1 || size == 2);
	if (size == 2) {
		PartBase* swap = m_compositeParts[0];
		m_compositeParts[0] = m_compositeParts[1];
		m_compositeParts[1] = swap;
	}
}

}; // namespace DecoratorSDK
