//################################################################################################
//
// Model switch button part class (decorator part)
//	ModelSwitchButtonPart.cpp
//
//################################################################################################

#include "StdAfx.h"
#include "ModelSwitchButtonPart.h"
#include "DecoratorExceptions.h"
#include "DecoratorLibResource.h"


namespace DecoratorSDK {

//################################################################################################
//
// CLASS : ModelSwitchButtonPart
//
//################################################################################################

ModelSwitchButtonPart::ModelSwitchButtonPart(PartBase* pPart, CComPtr<IMgaCommonDecoratorEvents>& eventSink):
	MaskedBitmapPart(pPart, eventSink, IDB_EXPAND_SIGN, COLOR_TRANSPARENT, COLOR_GRAYED_OUT)
{
}

ModelSwitchButtonPart::~ModelSwitchButtonPart()
{
}

// New functions
void ModelSwitchButtonPart::InitializeEx(CComPtr<IMgaProject>& pProject, CComPtr<IMgaMetaPart>& pPart, CComPtr<IMgaFCO>& pFCO,
										 HWND parentWnd, PreferenceMap& preferences)
{
	bool bExpanded = false;
	PreferenceMap::iterator it = preferences.find(PREF_ISMODELEXPANDED);
	if (it != preferences.end())
		bExpanded = it->second.uValue.bValue;

	m_lBitmapResID = bExpanded ? IDB_COLLAPSE_SIGN : IDB_EXPAND_SIGN;

	MaskedBitmapPart::InitializeEx(pProject, pPart, pFCO, parentWnd, preferences);
}

bool ModelSwitchButtonPart::MouseMoved(UINT nFlags, const CPoint& point, HDC transformHDC)
{
	if (m_spFCO && m_bActive) {
		CRect cRect = GetLocation();
		if (cRect.PtInRect(point)) {
			HCURSOR wantedCursor = LoadCursor(NULL, IDC_CROSS);
			m_originalCursor = SetCursor(wantedCursor);
			m_parentPart->CursorChanged((long)IDC_CROSS);
			m_bCursorSaved = true;
			return true;
		} else {
			cRect.InflateRect(DECORATOR_MAXSENSITIVITYDISTANCE, DECORATOR_MAXSENSITIVITYDISTANCE);
			if (cRect.PtInRect(point)) {
				if (m_bCursorSaved) {
					SetCursor(m_originalCursor);
					m_bCursorSaved = false;
					m_parentPart->CursorRestored();
				}
				return true;
			}
		}
	}

	return false;
}

bool ModelSwitchButtonPart::MouseLeftButtonDown(UINT nFlags, const CPoint& point, HDC transformHDC)
{
	if (m_spFCO && m_bActive) {
		CRect cRect = GetLocation();
		cRect.InflateRect(DECORATOR_MAXSENSITIVITYDISTANCE, DECORATOR_MAXSENSITIVITYDISTANCE);
		if (cRect.PtInRect(point)) {
			InitiateModelExpandStateSwitch();
			return true;
		}
	}

	return false;
}

bool ModelSwitchButtonPart::MouseLeftButtonUp(UINT nFlags, const CPoint& point, HDC transformHDC)
{
	if (m_spFCO && m_bActive) {
		CRect cRect = GetLocation();
		cRect.InflateRect(DECORATOR_MAXSENSITIVITYDISTANCE, DECORATOR_MAXSENSITIVITYDISTANCE);
		if (cRect.PtInRect(point)) {
			// supress message
			return true;
		}
	}

	return false;
}

bool ModelSwitchButtonPart::MouseLeftButtonDoubleClick(UINT nFlags, const CPoint& point, HDC transformHDC)
{
	return MouseLeftButtonDown(nFlags, point, transformHDC);
}

bool ModelSwitchButtonPart::MouseRightButtonDown(HMENU hCtxMenu, UINT nFlags, const CPoint& point, HDC transformHDC)
{
	if (m_spFCO && m_bActive) {
		CRect cRect = GetLocation();
		cRect.InflateRect(DECORATOR_MAXSENSITIVITYDISTANCE, DECORATOR_MAXSENSITIVITYDISTANCE);
		if (cRect.PtInRect(point)) {
			::AppendMenu(hCtxMenu,
						 MF_STRING | MF_ENABLED,
						 m_lBitmapResID == IDB_EXPAND_SIGN ? CTX_MENU_ID_EXPANDMODEL : CTX_MENU_ID_COLLAPSEMODEL,
						 m_lBitmapResID == IDB_EXPAND_SIGN ? CTX_MENU_STR_EXPANDMODEL : CTX_MENU_STR_COLLAPSEMODEL);
			return true;
		}
	}

	return false;
}

bool ModelSwitchButtonPart::MenuItemSelected(UINT menuItemId, UINT nFlags, const CPoint& point, HDC transformHDC)
{
	if (m_spFCO && m_bActive) {
		if (menuItemId == CTX_MENU_ID_EXPANDMODEL || menuItemId == CTX_MENU_ID_COLLAPSEMODEL) {
			InitiateModelExpandStateSwitch();
			return true;
		}
	}

	return false;
}

void ModelSwitchButtonPart::InitiateModelExpandStateSwitch(void)
{
	m_parentPart->GeneralOperationStarted(NULL);
	COMTRY {
		CComPtr<IMgaMetaAspect> mAspect;
		COMTHROW(m_spPart->get_ParentAspect(&mAspect));

		long status;
		COMTHROW(m_spProject->get_ProjectStatus(&status));
		bool inTrans = (status & 0x08L) != 0;
		CComPtr<IMgaTerritory> terr;
		if (!inTrans) {
			COMTHROW(m_spProject->CreateTerritory(NULL, &terr, 0));
			COMTHROW(m_spProject->BeginTransaction(terr, TRANSACTION_GENERAL));
		} else {
			COMTHROW(m_spProject->get_ActiveTerritory(&terr));
		}

		CComBSTR bstrVal;
		CComPtr<IMgaFCO> terrFco;
		COMTHROW(terr->OpenFCO(m_spFCO, &terrFco));
		status = OBJECT_ZOMBIE;
		COMTHROW(terrFco->get_Status(&status));
		if (status == OBJECT_EXISTS) {
			CComPtr<IMgaPart> part;
			COMTHROW(terrFco->get_Part(mAspect, &part));

			ASSERT(part);
			if (part) {
				OLECHAR bbc[10];
				swprintf(bbc, 10, OLESTR("%ld"), m_lBitmapResID == IDB_EXPAND_SIGN ? 1 : 0);
				CComBSTR bb(bbc);
				COMTHROW(part->put_RegistryValue(CComBSTR(PREF_ISMODELEXPANDED), bb));
			}
		}

		if (!inTrans) {
			m_spProject->CommitTransaction();
		}
	}
	catch(hresult_exception &e)
	{
		ASSERT(FAILED(e.hr));
		SetErrorInfo(e.hr);
	}
	m_parentPart->GeneralOperationFinished(NULL);
}

}; // namespace DecoratorSDK
