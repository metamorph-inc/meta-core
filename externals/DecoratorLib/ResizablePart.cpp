//################################################################################################
//
// Resizable part class (decorator part)
//	ResizablePart.cpp
//
//################################################################################################

#include "StdAfx.h"
#include "ResizablePart.h"
#include "DecoratorExceptions.h"
#include "DialogTemplate.h"
#include "SetSizeDialog.h"


namespace DecoratorSDK {

//################################################################################################
//
// CLASS : ResizablePart
//
//################################################################################################

ResizablePart::ResizablePart(PartBase* pPart, CComPtr<IMgaCommonDecoratorEvents>& eventSink):
	PartBase			(pPart, eventSink),
	resizeLogic			(NULL, NULL),
	m_setSizeDlg		(NULL)
{
	resizeLogic.SetParentPart(this);
	inSizeDlgProcedure = false;
}

ResizablePart::~ResizablePart()
{
}

void ResizablePart::Initialize(CComPtr<IMgaProject>& pProject, CComPtr<IMgaMetaPart>& pPart, CComPtr<IMgaFCO>& pFCO)
{
	if (m_spFCO)
		resizeLogic.SetResizeFeatures(ResizeLogic::Resizeable);

	PartBase::Initialize(pProject, pPart, pFCO);
}

void ResizablePart::Destroy(void)
{
	OperationCanceledByGME();
	if (m_bResizable)
		resizeLogic.Destroy();
}

feature_code ResizablePart::GetFeatures(void) const
{
	return m_bResizable ? F_RESIZABLE | F_MOUSEEVENTS : 0;
}

CSize ResizablePart::GetPreferredSize(void) const
{
	long cx = 0;
	long cy = 0;

	if (m_bResizable) {
		COMTRY {
			CComPtr<IMgaMetaAspect> mAspect;
			COMTHROW(m_spPart->get_ParentAspect(&mAspect));

			long status;
			COMTHROW(m_spProject->get_ProjectStatus(&status));
			bool inTrans = (status & 0x08L) != 0;
			CComPtr<IMgaTerritory> terr;
			CComBSTR bstrVal;
			CComPtr<IMgaPart> part;
			CComBSTR regName(PREF_PREFERREDSIZE);
			if (!inTrans) {
				COMTHROW(m_spProject->CreateTerritory(NULL, &terr, 0));
				COMTHROW(m_spProject->BeginTransaction(terr, TRANSACTION_READ_ONLY));

				CComPtr<IMgaFCO> terrFco;
				COMTHROW(terr->OpenFCO(m_spFCO, &terrFco));
				status = OBJECT_ZOMBIE;
				COMTHROW(terrFco->get_Status(&status));
				if (status == OBJECT_EXISTS) {
					COMTHROW(terrFco->get_Part(mAspect, &part));
					if (part)
						COMTHROW(part->get_RegistryValue(regName, &bstrVal));
				}

				m_spProject->CommitTransaction();
			} else {
				COMTHROW(m_spFCO->get_Part(mAspect, &part));
				if (part)
					COMTHROW(part->get_RegistryValue(regName, &bstrVal));
			}

			if (bstrVal) {
				int numConverted = swscanf(bstrVal, OLESTR("%ld,%ld"), &cx, &cy);
				if (numConverted != 2) {
					cx = 0;
					cy = 0;
				}
			}
		}
		catch(hresult_exception &e)
		{
			ASSERT(FAILED(e.hr));
			SetErrorInfo(e.hr);
		}
	}

	return CSize(cx, cy);
}

void ResizablePart::SetLocation(const CRect& location)
{
	PartBase::SetLocation(location);
	resizeLogic.SetResizeTargetLocation(location);
}

CRect ResizablePart::GetLocation(void) const
{
	return PartBase::GetLocation();
}

void ResizablePart::Draw(CDC* pDC, Gdiplus::Graphics* gdip)
{
	if (m_bResizable)
		resizeLogic.Draw(pDC, gdip);
}

// New functions
void ResizablePart::InitializeEx(CComPtr<IMgaProject>& pProject, CComPtr<IMgaMetaPart>& pPart, CComPtr<IMgaFCO>& pFCO,
								 HWND parentWnd, PreferenceMap& preferences)
{
	if (pFCO)
		resizeLogic.InitializeEx(pProject, pPart, pFCO, parentWnd, preferences);
	PartBase::InitializeEx(pProject, pPart, pFCO, parentWnd, preferences);
}

bool ResizablePart::MouseMoved(UINT nFlags, const CPoint& point, HDC transformHDC)
{
	if (m_bResizable)
		return resizeLogic.MouseMoved(nFlags, point, transformHDC);

	return false;
}

bool ResizablePart::MouseLeftButtonDown(UINT nFlags, const CPoint& point, HDC transformHDC)
{
	if (m_bResizable)
		return resizeLogic.MouseLeftButtonDown(nFlags, point, transformHDC);

	return false;
}

bool ResizablePart::MouseLeftButtonUp(UINT nFlags, const CPoint& point, HDC transformHDC)
{
	if (m_bResizable)
		return resizeLogic.MouseLeftButtonUp(nFlags, point, transformHDC);

	return false;
}

bool ResizablePart::MouseRightButtonDown(HMENU hCtxMenu, UINT nFlags, const CPoint& point, HDC transformHDC)
{
	if (m_bResizable && m_bActive) {
		CRect ptRect = GetLocation();
		if (ptRect.PtInRect(point)) {
			::AppendMenu(hCtxMenu, MF_STRING | MF_ENABLED, CTX_MENU_ID_SETSIZE, CTX_MENU_STR_SETSIZE);
			if (ResizablePart::GetPreferredSize() != CSize(0, 0)) {
				::AppendMenu(hCtxMenu, MF_STRING | MF_ENABLED, CTX_MENU_ID_RESETSIZE, CTX_MENU_STR_RESETSIZE);
			}
			return true;
		}
	}

	return false;
}

bool ResizablePart::MenuItemSelected(UINT menuItemId, UINT nFlags, const CPoint& point, HDC transformHDC)
{
	bool handled = false;
	if (m_bResizable) {
		if (menuItemId == CTX_MENU_ID_SETSIZE) {
			bool isPermanentCWnd = true;
			CWnd* cWnd = CWnd::FromHandlePermanent(m_parentWnd);
			if (cWnd == NULL) {
				isPermanentCWnd = false;
				cWnd = CWnd::FromHandle(m_parentWnd);
			}

			CDialogTemplate dlgTemplate(_T("Set size"),
										DS_SETFONT | DS_MODALFRAME | DS_FIXEDSYS | WS_POPUP | WS_CAPTION | WS_SYSMENU,	// Window Styles
										0,	// Extended Window Styles
										0, 0, SetSizeDialogWidth, SetSizeDialogHeight, _T("MS Shell Dlg"), 8);
			dlgTemplate.AddButton(_T("OK"), WS_VISIBLE, 0, 6, 42, 50, 14, IDOK);
			dlgTemplate.AddButton(_T("Cancel"), WS_VISIBLE, 0, 60, 42, 50, 14, IDCANCEL);
			dlgTemplate.AddStatic(_T("Width:"), WS_VISIBLE, 0, 6, 6, 26, 8, WidthTextCtrlId);
			dlgTemplate.AddStatic(_T("Height:"), WS_VISIBLE, 0, 6, 24, 26, 8, HeightTextCtrlId);
			dlgTemplate.AddEditBox(_T(""), WS_VISIBLE | ES_AUTOHSCROLL, 0, 36, 6, 72, 14, WidthEditCtrlId);
			dlgTemplate.AddEditBox(_T(""), WS_VISIBLE | ES_AUTOHSCROLL, 0, 36, 24, 72, 14, HeightEditCtrlId);

			CSetSizeDialog setSizeDlg;
			setSizeDlg.SetParameters(this, GetLocation(), point, resizeLogic.DeterminePotentialResize(point),
									 cWnd, isPermanentCWnd, transformHDC, nFlags);
			BOOL success = setSizeDlg.InitModalIndirect(dlgTemplate, cWnd);
			INT_PTR retVal = 0;
			if (success != FALSE) {
				inSizeDlgProcedure = true;
				m_setSizeDlg = &setSizeDlg;
				retVal = setSizeDlg.DoModal();
			}
			m_setSizeDlg = NULL;
			inSizeDlgProcedure = false;

			if (!isPermanentCWnd)
				cWnd->Detach();

			handled = true;
		} else if (menuItemId == CTX_MENU_ID_RESETSIZE) {
			m_parentPart->GeneralOperationStarted(NULL);
			CComBSTR regName(PREF_PREFERREDSIZE);
			// Getting regnode
			CComPtr<IMgaMetaAspect> mAspect;
			COMTHROW(m_spPart->get_ParentAspect(&mAspect));

			long status;
			COMTHROW(m_spProject->get_ProjectStatus(&status));
			bool inTrans = (status & 0x08L) != 0;
			CComPtr<IMgaPart> part;
			CComPtr<IMgaRegNode> ccpMgaRegNode;
			if (!inTrans) {
				ASSERT(false);	// We cannot commit a transaction here, cause it would destroy ourselves
			} else {
				COMTHROW(m_spFCO->get_Part(mAspect, &part));
				if (part)
					CComBSTR regName(PREF_PREFERREDSIZE);
					COMTHROW(part->get_RegistryNode(regName, &ccpMgaRegNode));
			}
			if (ccpMgaRegNode) {
				// Delete the registry entries
				COMTHROW(ccpMgaRegNode->RemoveTree());
			}
			m_parentPart->GeneralOperationFinished(NULL);
			handled = true;
		}
	}

	return handled;
}

bool ResizablePart::OperationCanceledByGME(void)
{
	SetLocation(resizeLogic.GetOriginalLocation());
	if (m_bResizable)
		return resizeLogic.OperationCanceledByGME();

	return false;
}

void ResizablePart::WindowResizing(UINT nFlags, CRect& location)
{
	ASSERT(m_bResizable);
	SetLocation(location);
	PartBase::WindowResizing(nFlags, location);
}

void ResizablePart::WindowResizingFinished(UINT nFlags, CRect& location)
{
	ASSERT(m_bResizable);
	SetLocation(location);
	if (m_bResizable && ((inSizeDlgProcedure && m_setSizeDlg->GetDlgResult()) ||
						 (!inSizeDlgProcedure && resizeLogic.IsSizeChanged()))) {
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
					CPoint pt = location.TopLeft();
					COMTHROW(part->SetGmeAttrs((BSTR)0, pt.x, pt.y));
					// Save preferred size part
					CSize size(location.Width(), location.Height());
					if (size.cx >= 0 && size.cy >= 0) {
						OLECHAR bbc[40];
						swprintf(bbc, 40, OLESTR("%ld,%ld"), size.cx, size.cy);
						CComBSTR bb(bbc);
						COMTHROW(part->put_RegistryValue(CComBSTR(PREF_PREFERREDSIZE), bb));
					}
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
	}
	PartBase::WindowResizingFinished(nFlags, location);
}

}; // namespace DecoratorSDK
