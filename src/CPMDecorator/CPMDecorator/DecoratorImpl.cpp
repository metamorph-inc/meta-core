//################################################################################################
//
// Decorator COM side Implementation
//	DecoratorImpl.cpp
// This class represents the COM/ATL side of the decorator COM connection layer
// For the pure C++ side see Decorator.h,cpp and DecoratorCompositePart.h,cpp
//
//################################################################################################

#include "StdAfx.h"
#include "DecoratorConfig.h"
#include "DecoratorImpl.h"
#include "Decorator.h"
#include "DecoratorExceptions.h"
#include "DecoratorInterface.h"


//################################################################################################
//
// CLASS : CDecoratorImpl
//
//################################################################################################

#define VERIFY_INITIALIZATION					\
	if (!m_pElementDecorator)					\
		return E_DECORATOR_UNINITIALIZED;

#define VERIFY_LOCATION							\
	if (!m_bLocationSet)						\
		return E_DECORATOR_LOCISNOTSET;

CDecoratorImpl::CDecoratorImpl():
	m_pElementDecorator		(NULL),
	m_bLocationSet		(false),
	m_bInitCallFromEx	(false)
{
}

CDecoratorImpl::~CDecoratorImpl()
{
}

STDMETHODIMP CDecoratorImpl::Initialize(IMgaProject* pProject, IMgaMetaPart* pPart, IMgaFCO* pFCO)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	//
	// TODO: read all important data from MGA and cache them for later use
	//

	if (pFCO && !m_bInitCallFromEx)
		return E_DECORATOR_USING_DEPRECATED_FUNCTION;

	return S_OK;
}

STDMETHODIMP CDecoratorImpl::Destroy()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	//
	// TODO: At least free all references to MGA objects
	//

	VERIFY_INITIALIZATION

	HRESULT retVal = S_OK;
	try {
		m_pElementDecorator->Destroy();
	}
	catch(hresult_exception& e) {
		retVal = e.hr;
	}
	catch(DecoratorException& e) {
		retVal = e.GetHResult();
	}

	m_bLocationSet = false;
	if (m_pElementDecorator) {
		delete m_pElementDecorator;
		m_pElementDecorator = NULL;
	}

	return retVal;
}

STDMETHODIMP CDecoratorImpl::GetMnemonic(BSTR* bstrMnemonic)
{
	//
	// TODO: Return the logical name of the decorator (currently not used by GME)
	//
	*bstrMnemonic = CComBSTR(DECORATOR_NAME).Detach();

	return S_OK;
}

STDMETHODIMP CDecoratorImpl::GetFeatures(feature_code* pFeatureCodes)
{
	//
	// TODO: Return supported features (combine multiple features with bitwise-OR)
	// Available feature codes are found in MgaDecorator.idl
	// (curently not used by GME)
	if (m_pElementDecorator != NULL)
		*pFeatureCodes = m_pElementDecorator->GetFeatures();
	else
		*pFeatureCodes = F_HASLABEL | F_MOUSEEVENTS | F_RESIZEAFTERMOD;

	return S_OK;
}

STDMETHODIMP CDecoratorImpl::SetParam(BSTR bstrName, VARIANT vValue)
{
	//
	// TODO:  Parse and set all supported parameters, otherwise return error
	// (currently all values are BSTR type)
	// 
	//  Akos: added passing it down to the element decorator  5/21/13
	//
	m_bstrName = bstrName;
	m_vValue = vValue;
	HRESULT retVal = S_OK;
	try {
		m_pElementDecorator->SetParam(bstrName,vValue);
	}
	catch(hresult_exception& e) {
		retVal = e.hr;
	}
	catch(DecoratorException& e) {
		retVal = e.GetHResult();
	}
	return retVal;
}

STDMETHODIMP CDecoratorImpl::GetParam(BSTR bstrName, VARIANT* pvValue)
{
	//
	// TODO: Return values of supported and previously set parameters, otherwise return error
	// (currently GME does not use this method)
	//
	VERIFY_INITIALIZATION

	HRESULT retVal = S_OK;
	try {
		m_pElementDecorator->GetParam(bstrName, pvValue);
	}
	catch(hresult_exception& e) {
		retVal = e.hr;
	}
	catch(DecoratorException& e) {
		retVal = e.GetHResult();
	}
	return retVal;
}

STDMETHODIMP CDecoratorImpl::SetActive(VARIANT_BOOL vbIsActive)
{
	//
	// TODO: If isActive == VARIANT_FALSE, draw your object in GME_GREYED_OUT, otherwise use the color of the object
	//
	VERIFY_INITIALIZATION

	HRESULT retVal = S_OK;
	try {
		m_pElementDecorator->SetActive(vbIsActive == VARIANT_TRUE);
	}
	catch(hresult_exception& e) {
		retVal = e.hr;
	}
	catch(DecoratorException& e) {
		retVal = e.GetHResult();
	}
	return retVal;
}

STDMETHODIMP CDecoratorImpl::GetPreferredSize(LONG* plWidth, LONG* plHeight)
{
	//
	// TODO: Give GME a hint about the object size. Do not expect GME to take it into account
	//
	VERIFY_INITIALIZATION

	HRESULT retVal = S_OK;
	try {
		CSize cSize = m_pElementDecorator->GetPreferredSize();
		*plWidth = cSize.cx;
		*plHeight = cSize.cy;
	}
	catch(hresult_exception& e) {
		retVal = e.hr;
	}
	catch(DecoratorException& e) {
		retVal = e.GetHResult();
	}
	return retVal;
}


STDMETHODIMP CDecoratorImpl::SetLocation(LONG sx, LONG sy, LONG ex, LONG ey)
{
	//
	// TODO: Draw the object exactly to the this location later
	//
	VERIFY_INITIALIZATION

	HRESULT retVal = S_OK;
	try {
		m_pElementDecorator->SetLocation(CRect(sx, sy, ex, ey));
		m_bLocationSet = true;
	}
	catch(hresult_exception& e) {
		retVal = e.hr;
	}
	catch(DecoratorException& e) {
		retVal = e.GetHResult();
	}
	return retVal;
}

STDMETHODIMP CDecoratorImpl::GetLocation(LONG* sx, LONG* sy, LONG* ex, LONG* ey)
{
	//
	// TODO: Return previously set location parameters
	// (currently GME does not call this)
	// 
	VERIFY_INITIALIZATION
	VERIFY_LOCATION

	HRESULT retVal = S_OK;
	try {
		CRect cRect = m_pElementDecorator->GetLocation();
		*sx = cRect.left;
		*sy = cRect.top;
		*ex = cRect.right;
		*ey = cRect.bottom;
	}
	catch(hresult_exception& e) {
		retVal = e.hr;
	}
	catch(DecoratorException& e) {
		retVal = e.GetHResult();
	}
	return retVal;
}

STDMETHODIMP CDecoratorImpl::GetLabelLocation(LONG* sx, LONG* sy, LONG* ex, LONG* ey)
{
	//
	// TODO: Return the location of the text box of your label if you support labels.
	// (currently GME does not call this)
	//
	VERIFY_INITIALIZATION
	VERIFY_LOCATION

	HRESULT retVal = S_OK;
	try {
		CRect labelRect = m_pElementDecorator->GetLabelLocation();
		*sx = labelRect.left;
		*sy = labelRect.top;
		*ex = labelRect.right;
		*ey = labelRect.bottom;
	}
	catch(hresult_exception& e) {
		retVal = e.hr;
	}
	catch(DecoratorException& e) {
		retVal = e.GetHResult();
	}
	return retVal;
}

STDMETHODIMP CDecoratorImpl::GetPortLocation(IMgaFCO* pFCO, LONG* sx, LONG* sy, LONG* ex, LONG* ey)
{
	//
	// TODO: Return the location of the specified port if ports are supported in the decorator
	//
	VERIFY_INITIALIZATION
	VERIFY_LOCATION

	HRESULT retVal = S_OK;
	try {
		CRect portLocation = m_pElementDecorator->GetPortLocation(CComPtr<IMgaFCO>(pFCO));
		*sx = portLocation.left;
		*sy = portLocation.top;
		*ex = portLocation.right;
		*ey = portLocation.bottom;
	}
	catch(hresult_exception& e) {
		retVal = e.hr;
	}
	catch(DecoratorException& e) {
		retVal = e.GetHResult();
	}
	return retVal;
}

STDMETHODIMP CDecoratorImpl::GetPorts(IMgaFCOs** portFCOs)
{
	//
	// TODO: Return a collection of mga objects represented as ports.
	//
	VERIFY_INITIALIZATION

	HRESULT retVal = S_OK;
	try {
		CComPtr<IMgaFCOs> portMgaFCOs;
		m_pElementDecorator->GetPorts(portMgaFCOs);
		*portFCOs = portMgaFCOs;
	}
	catch(hresult_exception& e) {
		retVal = e.hr;
	}
	catch(DecoratorException& e) {
		retVal = e.GetHResult();
	}
	return retVal;
}

STDMETHODIMP CDecoratorImpl::Draw
#if GME_VERSION_MAJOR >= 11
(ULONG hdc)
#else
(HDC hdc)
#endif
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	//
	// TODO: (In case of old decorator) Draw your object.
	// Saving & restoring the HDC is a good practice, however it is not a strict requirement
	// Use DecoratorSDK::getFacilities() for brushes/pens/fonts.
	//

	VERIFY_INITIALIZATION
	VERIFY_LOCATION

	return E_DECORATOR_USING_DEPRECATED_FUNCTION;
}

STDMETHODIMP CDecoratorImpl::SaveState()
{
	//
	// TODO: The only method where we are in read-write transaction. Store all permanent information
	// (currently GME does not support this) 
	//
	VERIFY_INITIALIZATION

	return S_OK;
}

// New functions
STDMETHODIMP CDecoratorImpl::InitializeEx(IMgaProject* pProject, IMgaMetaPart* pPart, IMgaFCO* pFCO,
										  IMgaCommonDecoratorEvents* eventSink, ULONGLONG parentWnd)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	//
	// TODO: handle extra parameters, call Initialize with the rest
	//

	m_bInitCallFromEx = true;

	HRESULT retVal = S_OK;
	try {
		Decor::Decorator* decorator = new Decor::Decorator(CComPtr<IMgaCommonDecoratorEvents>(eventSink));
		m_pElementDecorator = decorator;
		decorator->InitializeEx(CComPtr<IMgaProject>(pProject), CComPtr<IMgaMetaPart>(pPart),
								CComPtr<IMgaFCO>(pFCO), (HWND)parentWnd);
	}
	catch(hresult_exception& e) {
		retVal = e.hr;
	}
	catch(DecoratorException& e) {
		retVal = e.GetHResult();
	}

	return retVal;
}

STDMETHODIMP CDecoratorImpl::DrawEx
#if GME_VERSION_MAJOR >= 11
(ULONG hdc, ULONGLONG gdipGraphics)
#else
(HDC hdc, ULONGLONG gdipGraphics)
#endif
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	//
	// TODO: gdipGraphics is a Gdiplus::Graphics* variable, it is advisable to use this for drawing and don't use the HDC
	//
	VERIFY_INITIALIZATION
	VERIFY_LOCATION

	HRESULT retVal = S_OK;

	CDC dc;
	dc.Attach((HDC)hdc);
	{
		try {
			m_pElementDecorator->Draw(&dc, (Gdiplus::Graphics*)gdipGraphics);
		}
		catch(hresult_exception& e) {
			retVal = e.hr;
		}
		catch(DecoratorException& e) {
			retVal = e.GetHResult();
		}
	}
	dc.Detach();

	return retVal;
}

STDMETHODIMP CDecoratorImpl::SetSelected(VARIANT_BOOL vbIsSelected)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	//
	// TODO: memorize selected state, it might be needed for some operations later
	//

	VERIFY_INITIALIZATION

	HRESULT retVal = S_OK;
	try {
		m_pElementDecorator->SetSelected(vbIsSelected == VARIANT_TRUE);
	}
	catch(hresult_exception& e) {
		retVal = e.hr;
	}
	catch(DecoratorException& e) {
		retVal = e.GetHResult();
	}
	return retVal;
}

STDMETHODIMP CDecoratorImpl::MouseMoved(ULONG nFlags, LONG pointx, LONG pointy, ULONGLONG transformHDC)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	//
	// TODO: if you respond to the message, for example change the mouse cursor, you should return
	// S_DECORATOR_EVENT_HANDLED, else you should return S_DECORATOR_EVENT_NOT_HANDLED
	// nFlags shows additional button states (see WM_MOUSEMOVE MSDN page for example, see MK_CONTROL, MK_SHIFT, etc constants)
	// you can use transformHDC HDC to transform point coordinates to screen coordinates (GMEView can be zoomed and scrolled)
	//

	HRESULT retVal = S_OK;
	try {
		if (m_pElementDecorator->MouseMoved(nFlags, CPoint(pointx, pointy), (HDC)transformHDC))
			retVal = S_DECORATOR_EVENT_HANDLED;
		else
			retVal = S_DECORATOR_EVENT_NOT_HANDLED;
	}
	catch(hresult_exception& e) {
		retVal = e.hr;
	}
	catch(DecoratorException& e) {
		retVal = e.GetHResult();
	}
	return retVal;
}

STDMETHODIMP CDecoratorImpl::MouseLeftButtonDown(ULONG nFlags, LONG pointx, LONG pointy, ULONGLONG transformHDC)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	//
	// TODO: if you respond to the message, you should return
	// S_DECORATOR_EVENT_HANDLED, else you should return S_DECORATOR_EVENT_NOT_HANDLED
	// nFlags shows additional button states (see WM_MOUSEMOVE MSDN page for example, see MK_CONTROL, MK_SHIFT, etc constants)
	// you can use transformHDC HDC to transform point coordinates to screen coordinates (GMEView can be zoomed and scrolled)
	//

	HRESULT retVal = S_OK;
	try {
		if (m_pElementDecorator->MouseLeftButtonDown(nFlags, CPoint(pointx, pointy), (HDC)transformHDC))
			retVal = S_DECORATOR_EVENT_HANDLED;
		else
			retVal = S_DECORATOR_EVENT_NOT_HANDLED;
	}
	catch(hresult_exception& e) {
		retVal = e.hr;
	}
	catch(DecoratorException& e) {
		retVal = e.GetHResult();
	}
	return retVal;
}

STDMETHODIMP CDecoratorImpl::MouseLeftButtonUp(ULONG nFlags, LONG pointx, LONG pointy, ULONGLONG transformHDC)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	//
	// TODO: if you respond to the message, you should return
	// S_DECORATOR_EVENT_HANDLED, else you should return S_DECORATOR_EVENT_NOT_HANDLED
	// nFlags shows additional button states (see WM_MOUSEMOVE MSDN page for example, see MK_CONTROL, MK_SHIFT, etc constants)
	// you can use transformHDC HDC to transform point coordinates to screen coordinates (GMEView can be zoomed and scrolled)
	//

	HRESULT retVal = S_OK;
	try {
		if (m_pElementDecorator->MouseLeftButtonUp(nFlags, CPoint(pointx, pointy), (HDC)transformHDC))
			retVal = S_DECORATOR_EVENT_HANDLED;
		else
			retVal = S_DECORATOR_EVENT_NOT_HANDLED;
	}
	catch(hresult_exception& e) {
		retVal = e.hr;
	}
	catch(DecoratorException& e) {
		retVal = e.GetHResult();
	}
	return retVal;
}

STDMETHODIMP CDecoratorImpl::MouseLeftButtonDoubleClick(ULONG nFlags, LONG pointx, LONG pointy, ULONGLONG transformHDC)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	//
	// TODO: if you respond to the message, you should return
	// S_DECORATOR_EVENT_HANDLED, else you should return S_DECORATOR_EVENT_NOT_HANDLED
	// nFlags shows additional button states (see WM_MOUSEMOVE MSDN page for example, see MK_CONTROL, MK_SHIFT, etc constants)
	// you can use transformHDC HDC to transform point coordinates to screen coordinates (GMEView can be zoomed and scrolled)
	//

	HRESULT retVal = S_OK;
	try {
		if (m_pElementDecorator->MouseLeftButtonDoubleClick(nFlags, CPoint(pointx, pointy), (HDC)transformHDC))
			retVal = S_DECORATOR_EVENT_HANDLED;
		else
			retVal = S_DECORATOR_EVENT_NOT_HANDLED;
	}
	catch(hresult_exception& e) {
		retVal = e.hr;
	}
	catch(DecoratorException& e) {
		retVal = e.GetHResult();
	}
	return retVal;
}

STDMETHODIMP CDecoratorImpl::MouseRightButtonDown(ULONGLONG hCtxMenu, ULONG nFlags, LONG pointx, LONG pointy, ULONGLONG transformHDC)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	//
	// TODO: if you respond to the message, you should return
	// S_DECORATOR_EVENT_HANDLED, else you should return S_DECORATOR_EVENT_NOT_HANDLED
	// hCtxMenu is a HMENU type variable, you can add menu items to it which will be displayed in the context menu
	// nFlags shows additional button states (see WM_MOUSEMOVE MSDN page for example, see MK_CONTROL, MK_SHIFT, etc constants)
	// you can use transformHDC HDC to transform point coordinates to screen coordinates (GMEView can be zoomed and scrolled)
	//

	HRESULT retVal = S_OK;
	try {
		if (m_pElementDecorator->MouseRightButtonDown((HMENU) hCtxMenu, nFlags, CPoint(pointx, pointy), (HDC)transformHDC))
			retVal = S_DECORATOR_EVENT_HANDLED;
		else
			retVal = S_DECORATOR_EVENT_NOT_HANDLED;
	}
	catch(hresult_exception& e) {
		retVal = e.hr;
	}
	catch(DecoratorException& e) {
		retVal = e.GetHResult();
	}
	return retVal;
}

STDMETHODIMP CDecoratorImpl::MouseRightButtonUp(ULONG nFlags, LONG pointx, LONG pointy, ULONGLONG transformHDC)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	//
	// TODO: if you respond to the message, you should return
	// S_DECORATOR_EVENT_HANDLED, else you should return S_DECORATOR_EVENT_NOT_HANDLED
	// nFlags shows additional button states (see WM_MOUSEMOVE MSDN page for example, see MK_CONTROL, MK_SHIFT, etc constants)
	// you can use transformHDC HDC to transform point coordinates to screen coordinates (GMEView can be zoomed and scrolled)
	//

	HRESULT retVal = S_OK;
	try {
		if (m_pElementDecorator->MouseRightButtonUp(nFlags, CPoint(pointx, pointy), (HDC)transformHDC))
			retVal = S_DECORATOR_EVENT_HANDLED;
		else
			retVal = S_DECORATOR_EVENT_NOT_HANDLED;
	}
	catch(hresult_exception& e) {
		retVal = e.hr;
	}
	catch(DecoratorException& e) {
		retVal = e.GetHResult();
	}
	return retVal;
}

STDMETHODIMP CDecoratorImpl::MouseRightButtonDoubleClick(ULONG nFlags, LONG pointx, LONG pointy, ULONGLONG transformHDC)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	//
	// TODO: if you respond to the message, you should return
	// S_DECORATOR_EVENT_HANDLED, else you should return S_DECORATOR_EVENT_NOT_HANDLED
	// nFlags shows additional button states (see WM_MOUSEMOVE MSDN page for example, see MK_CONTROL, MK_SHIFT, etc constants)
	// you can use transformHDC HDC to transform point coordinates to screen coordinates (GMEView can be zoomed and scrolled)
	//

	HRESULT retVal = S_OK;
	try {
		if (m_pElementDecorator->MouseRightButtonDoubleClick(nFlags, CPoint(pointx, pointy), (HDC)transformHDC))
			retVal = S_DECORATOR_EVENT_HANDLED;
		else
			retVal = S_DECORATOR_EVENT_NOT_HANDLED;
	}
	catch(hresult_exception& e) {
		retVal = e.hr;
	}
	catch(DecoratorException& e) {
		retVal = e.GetHResult();
	}
	return retVal;
}

STDMETHODIMP CDecoratorImpl::MouseMiddleButtonDown(ULONG nFlags, LONG pointx, LONG pointy, ULONGLONG transformHDC)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	//
	// TODO: if you respond to the message, you should return
	// S_DECORATOR_EVENT_HANDLED, else you should return S_DECORATOR_EVENT_NOT_HANDLED
	// nFlags shows additional button states (see WM_MOUSEMOVE MSDN page for example, see MK_CONTROL, MK_SHIFT, etc constants)
	// you can use transformHDC HDC to transform point coordinates to screen coordinates (GMEView can be zoomed and scrolled)
	//

	HRESULT retVal = S_OK;
	try {
		if (m_pElementDecorator->MouseMiddleButtonDown(nFlags, CPoint(pointx, pointy), (HDC)transformHDC))
			retVal = S_DECORATOR_EVENT_HANDLED;
		else
			retVal = S_DECORATOR_EVENT_NOT_HANDLED;
	}
	catch(hresult_exception& e) {
		retVal = e.hr;
	}
	catch(DecoratorException& e) {
		retVal = e.GetHResult();
	}
	return retVal;
}

STDMETHODIMP CDecoratorImpl::MouseMiddleButtonUp(ULONG nFlags, LONG pointx, LONG pointy, ULONGLONG transformHDC)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	//
	// TODO: if you respond to the message, you should return
	// S_DECORATOR_EVENT_HANDLED, else you should return S_DECORATOR_EVENT_NOT_HANDLED
	// nFlags shows additional button states (see WM_MOUSEMOVE MSDN page for example, see MK_CONTROL, MK_SHIFT, etc constants)
	// you can use transformHDC HDC to transform point coordinates to screen coordinates (GMEView can be zoomed and scrolled)
	//

	HRESULT retVal = S_OK;
	try {
		if (m_pElementDecorator->MouseMiddleButtonUp(nFlags, CPoint(pointx, pointy), (HDC)transformHDC))
			retVal = S_DECORATOR_EVENT_HANDLED;
		else
			retVal = S_DECORATOR_EVENT_NOT_HANDLED;
	}
	catch(hresult_exception& e) {
		retVal = e.hr;
	}
	catch(DecoratorException& e) {
		retVal = e.GetHResult();
	}
	return retVal;
}

STDMETHODIMP CDecoratorImpl::MouseMiddleButtonDoubleClick(ULONG nFlags, LONG pointx, LONG pointy, ULONGLONG transformHDC)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	//
	// TODO: if you respond to the message, you should return
	// S_DECORATOR_EVENT_HANDLED, else you should return S_DECORATOR_EVENT_NOT_HANDLED
	// nFlags shows additional button states (see WM_MOUSEMOVE MSDN page for example, see MK_CONTROL, MK_SHIFT, etc constants)
	// you can use transformHDC HDC to transform point coordinates to screen coordinates (GMEView can be zoomed and scrolled)
	//

	HRESULT retVal = S_OK;
	try {
		if (m_pElementDecorator->MouseMiddleButtonDoubleClick(nFlags, CPoint(pointx, pointy), (HDC)transformHDC))
			retVal = S_DECORATOR_EVENT_HANDLED;
		else
			retVal = S_DECORATOR_EVENT_NOT_HANDLED;
	}
	catch(hresult_exception& e) {
		retVal = e.hr;
	}
	catch(DecoratorException& e) {
		retVal = e.GetHResult();
	}
	return retVal;
}

STDMETHODIMP CDecoratorImpl::MouseWheelTurned(ULONG nFlags, LONG distance, LONG pointx, LONG pointy, ULONGLONG transformHDC)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	//
	// TODO: if you respond to the message, you should return
	// S_DECORATOR_EVENT_HANDLED, else you should return S_DECORATOR_EVENT_NOT_HANDLED
	// nFlags shows additional button states (see WM_MOUSEMOVE MSDN page for example, see MK_CONTROL, MK_SHIFT, etc constants)
	// you can use transformHDC HDC to transform point coordinates to screen coordinates (GMEView can be zoomed and scrolled)
	//

	HRESULT retVal = S_OK;
	try {
		if (m_pElementDecorator->MouseWheelTurned(nFlags, (short)distance, CPoint(pointx, pointy), (HDC)transformHDC))
			retVal = S_DECORATOR_EVENT_HANDLED;
		else
			retVal = S_DECORATOR_EVENT_NOT_HANDLED;
	}
	catch(hresult_exception& e) {
		retVal = e.hr;
	}
	catch(DecoratorException& e) {
		retVal = e.GetHResult();
	}
	return retVal;
}

STDMETHODIMP CDecoratorImpl::DragEnter(ULONG* dropEffect, ULONGLONG pCOleDataObject, ULONG keyState, LONG pointx, LONG pointy, ULONGLONG transformHDC)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	//
	// TODO: if you respond to the message, you should return
	// S_DECORATOR_EVENT_HANDLED, else you should return S_DECORATOR_EVENT_NOT_HANDLED
	// If you are interested only file drag-drops you should only hook up to DropFile event and inhibit DragEnter, DragOver, Drop
	// If you want more complicated things though, you should handle the three mentioned event in Windows fashion.
	// See OnDragEnter, OnDragOver, OnDrop MFC notifications in MSDN or GME source
	// pCOleDataObject is a COleDataObject pointer
	//

	HRESULT retVal = S_OK;
	try {
		if (m_pElementDecorator->DragEnter((DROPEFFECT*)dropEffect, (COleDataObject*)pCOleDataObject, (DWORD)keyState, CPoint(pointx, pointy), (HDC)transformHDC)) {
			retVal = S_DECORATOR_EVENT_HANDLED;
		} else {
			retVal = S_DECORATOR_EVENT_NOT_HANDLED;
			*dropEffect = DROPEFFECT_NONE;
		}
	}
	catch(hresult_exception& e) {
		retVal = e.hr;
	}
	catch(DecoratorException& e) {
		retVal = e.GetHResult();
	}
	return retVal;
}

STDMETHODIMP CDecoratorImpl::DragOver(ULONG* dropEffect, ULONGLONG pCOleDataObject, ULONG keyState, LONG pointx, LONG pointy, ULONGLONG transformHDC)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	//
	// TODO: if you respond to the message, you should return
	// S_DECORATOR_EVENT_HANDLED, else you should return S_DECORATOR_EVENT_NOT_HANDLED
	// If you are interested only file drag-drops you should only hook up to DropFile event and inhibit DragEnter, DragOver, Drop
	// If you want more complicated things though, you should handle the three mentioned event in Windows fashion.
	// See OnDragEnter, OnDragOver, OnDrop MFC notifications in MSDN or GME source
	// pCOleDataObject is a COleDataObject pointer
	//

	HRESULT retVal = S_OK;
	try {
		if (m_pElementDecorator->DragOver((DROPEFFECT*)dropEffect, (COleDataObject*)pCOleDataObject, (DWORD)keyState, CPoint(pointx, pointy), (HDC)transformHDC)) {
			retVal = S_DECORATOR_EVENT_HANDLED;
		} else {
			retVal = S_DECORATOR_EVENT_NOT_HANDLED;
			*dropEffect = DROPEFFECT_NONE;
		}
	}
	catch(hresult_exception& e) {
		retVal = e.hr;
	}
	catch(DecoratorException& e) {
		retVal = e.GetHResult();
	}
	return retVal;
}

STDMETHODIMP CDecoratorImpl::Drop(ULONGLONG pCOleDataObject, ULONG dropEffect, LONG pointx, LONG pointy, ULONGLONG transformHDC)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	//
	// TODO: if you respond to the message, you should return
	// S_DECORATOR_EVENT_HANDLED, else you should return S_DECORATOR_EVENT_NOT_HANDLED
	// If you are interested only file drag-drops you should only hook up to DropFile event and inhibit DragEnter, DragOver, Drop
	// If you want more complicated things though, you should handle the three mentioned event in Windows fashion.
	// See OnDragEnter, OnDragOver, OnDrop MFC notifications in MSDN or GME source
	// pCOleDataObject is a COleDataObject pointer
	//

	HRESULT retVal = S_OK;
	try {
		if (m_pElementDecorator->Drop((COleDataObject*)pCOleDataObject, (DROPEFFECT)dropEffect, CPoint(pointx, pointy), (HDC)transformHDC))
			retVal = S_DECORATOR_EVENT_HANDLED;
		else
			retVal = S_DECORATOR_EVENT_NOT_HANDLED;
	}
	catch(hresult_exception& e) {
		retVal = e.hr;
	}
	catch(DecoratorException& e) {
		retVal = e.GetHResult();
	}
	return retVal;
}

STDMETHODIMP CDecoratorImpl::DropFile(ULONGLONG hDropInfo, LONG pointx, LONG pointy, ULONGLONG transformHDC)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	//
	// TODO: if you respond to the message, you should return
	// S_DECORATOR_EVENT_HANDLED, else you should return S_DECORATOR_EVENT_NOT_HANDLED
	// hDropInfo is a HDROP type variable, you should use this to extract the needed data
	//

	HRESULT retVal = S_OK;
	try {
		if (m_pElementDecorator->DropFile((HDROP)hDropInfo, CPoint(pointx, pointy), (HDC)transformHDC))
			retVal = S_DECORATOR_EVENT_HANDLED;
		else
			retVal = S_DECORATOR_EVENT_NOT_HANDLED;
	}
	catch(hresult_exception& e) {
		retVal = e.hr;
	}
	catch(DecoratorException& e) {
		retVal = e.GetHResult();
	}
	return retVal;
}

STDMETHODIMP CDecoratorImpl::MenuItemSelected(ULONG menuItemId, ULONG nFlags, LONG pointx, LONG pointy, ULONGLONG transformHDC)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	//
	// TODO: if you respond to the message, you should return
	// S_DECORATOR_EVENT_HANDLED, else you should return S_DECORATOR_EVENT_NOT_HANDLED
	// In menuItemId you should get back one of the menu IDs you previously added to the context menu
	//

	HRESULT retVal = S_OK;
	try {
		if (m_pElementDecorator->MenuItemSelected(menuItemId, nFlags, CPoint(pointx, pointy), (HDC)transformHDC))
			retVal = S_DECORATOR_EVENT_HANDLED;
		else
			retVal = S_DECORATOR_EVENT_NOT_HANDLED;
	}
	catch(hresult_exception& e) {
		retVal = e.hr;
	}
	catch(DecoratorException& e) {
		retVal = e.GetHResult();
	}
	return retVal;
}

STDMETHODIMP CDecoratorImpl::OperationCanceled()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	//
	// TODO: if you handle the message, you should return S_DECORATOR_EVENT_HANDLED,
	// else you should return S_DECORATOR_EVENT_NOT_HANDLED
	//

	HRESULT retVal = S_OK;
	try {
		if (m_pElementDecorator->OperationCanceledByGME())
			retVal = S_DECORATOR_EVENT_HANDLED;
		else
			retVal = S_DECORATOR_EVENT_NOT_HANDLED;
	}
	catch(hresult_exception& e) {
		retVal = e.hr;
	}
	catch(DecoratorException& e) {
		retVal = e.GetHResult();
	}
	return retVal;
}
