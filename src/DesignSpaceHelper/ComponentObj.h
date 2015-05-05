#pragma once


#include "ComHelp.h"
#include "GMECOM.h"
#include "Exceptions.h"
#include "MgaUtil.h"
#include "Core.h"
#include "Mga.h"

#include "ComponentConfig.h"

#if defined(BUILDER_OBJECT_NETWORK)
#else
#ifdef BUILDER_OBJECT_NETWORK_V2
#include "BON.h"
#include <BON2Component.h>
#else
#include <RawComponent.h>
#endif // BUILDER_OBJECT_NETWORK_V2
#endif // BUILDER_OBJECT_NETWORK

#pragma once

class CComponentObj;

#ifdef GME_ADDON

#ifndef RAWCOMPONENT_H
// BY PAKA BEGIN
#ifndef BON2Component_h
#error GME AddOn-s must be built with the RAW Component interface or BON2 Component Interface
#endif // BON2Component_h
// BY PAKA END
#endif // RAWCOMPONENT_H

class CEventSink : public CCmdTarget {
	DECLARE_DYNCREATE(CEventSink)
	CEventSink();           // protected constructor used by dynamic creation
public:
	CComponentObj *comp;

	IMgaEventSink* GetInterface() { return &m_xComponent; }

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEventSink)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation

public:
	virtual ~CEventSink();

	DECLARE_MESSAGE_MAP()
	DECLARE_OLECREATE(CEventSink)

	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CEventSink)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()

	DECLARE_INTERFACE_MAP()
public:
	BEGIN_INTERFACE_PART(Component, IMgaEventSink)
		STDMETHODIMP GlobalEvent(globalevent_enum event);
		STDMETHODIMP ObjectEvent(IMgaObject * obj, unsigned long eventmask, VARIANT v);
	END_INTERFACE_PART(Component)
};

#endif // GME_ADDON

	struct __declspec(uuid("270b4f86-b17c-11d3-9ad1-00aa00b6fe26")) /* LIBID */ ___MGALib;

	static HRESULT WINAPI GetIDispatchComponentEx(IMgaComponentEx* this_, REFIID riid, LPVOID* ppv) {
		// Use this if your interface is defined in this project
		// wchar_t typelibPath[MAX_PATH];
		// GetModuleFileNameW(HINST_THISCOMPONENT, thisModuleFileName, sizeof(thisModuleFileName) / sizeof(thisModuleFileName[0]));
		_bstr_t typelibPath;
		HRESULT hr = QueryPathOfRegTypeLib(__uuidof(___MGALib), 1, 0, 0, typelibPath.GetAddress());
		if (FAILED(hr))
			return hr;

		ITypeLibPtr typeLib;
		hr = LoadTypeLib(static_cast<const wchar_t*>(typelibPath), &typeLib);
		if (FAILED(hr))
			return hr;

		ITypeInfoPtr typeInfo;
		hr = typeLib->GetTypeInfoOfGuid(__uuidof(IMgaComponentEx), &typeInfo);
		if (FAILED(hr))
			return hr;

		IUnknownPtr pUnk;
		hr = CreateStdDispatch(NULL, this_, typeInfo, &pUnk);
		if (FAILED(hr))
			return hr;

		return pUnk->QueryInterface(__uuidof(IDispatch), ppv);
	}


/////////////////////////////////////////////////////////////////////////////
// CComponentObj command target

class __declspec(uuid(COCLASS_UUID)) CComponentObj : public CCmdTarget
{

	DECLARE_DYNCREATE(CComponentObj)

	CComponentObj();           // protected constructor used by dynamic creation
	void RegisterActiveObject();
	unsigned long registeractiveobjectret;

// Attributes
public:

// Operations
public:
	IMgaComponentEx* GetInterface() { return &m_xComponent; }

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CComponentObj)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

	virtual LPUNKNOWN GetInterfaceHook(const void* riid)
	{
		if (*((const IID*)riid) == __uuidof(IDispatch))
		{
			CComPtr<IMgaComponentEx> pComp;
			IID iidIMgaComponentEx = __uuidof(IMgaComponentEx);
			HRESULT hr = __super::ExternalQueryInterface(&iidIMgaComponentEx, (void**)&pComp.p);
			CComPtr<IUnknown> ret;
			if (SUCCEEDED(GetIDispatchComponentEx(pComp, *((const IID*)riid), (void**)&ret.p)))
			{
				return ret.Detach();
			}
		}
		return NULL;
	}
    /*virtual HRESULT STDMETHODCALLTYPE QueryInterface( 
        REFIID riid,
        __RPC__deref_out void __RPC_FAR *__RPC_FAR *ppvObject)
	{
		return __super::QueryInterface(riid, ppvObject);
	}*/

// Implementation
protected:
	virtual ~CComponentObj();
	void UnregisterActiveObject();

	// Generated message map functions
	//{{AFX_MSG(CComponentObj)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	DECLARE_OLECREATE(CComponentObj)

	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CComponentObj)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()

	DECLARE_INTERFACE_MAP()
	BEGIN_INTERFACE_PART(Component, IMgaComponentEx)
		STDMETHODIMP InvokeEx( IMgaProject *project,  IMgaFCO *currentobj,  IMgaFCOs *selectedobjs, long param);
		STDMETHODIMP ObjectsInvokeEx( IMgaProject *project,  IMgaObject *currentobj,  IMgaObjects *selectedobjs,  long param);
		STDMETHODIMP Invoke(IMgaProject* gme, IMgaFCOs *models, long param);
		STDMETHODIMP Initialize(struct IMgaProject *);
		STDMETHODIMP Enable(VARIANT_BOOL newVal);
        STDMETHODIMP get_InteractiveMode(VARIANT_BOOL *enabled);

        STDMETHODIMP get_ComponentParameter(BSTR name, VARIANT *pVal);
        STDMETHODIMP put_ComponentParameter(BSTR name, VARIANT newVal);


        STDMETHODIMP put_InteractiveMode(VARIANT_BOOL enabled);
        STDMETHODIMP get_ComponentType( componenttype_enum *t);
        STDMETHODIMP get_ComponentProgID(BSTR *pVal) {
			*pVal = CComBSTR(COCLASS_PROGID).Detach();
			return S_OK;
		};
        STDMETHODIMP get_ComponentName(BSTR *pVal) {
			*pVal = CComBSTR(COMPONENT_NAME).Detach();
			return S_OK;
		};
        STDMETHODIMP get_Paradigm( BSTR *pVal) {
#ifdef PARADIGM_INDEPENDENT
			*pVal = CComBSTR("*").Detach();
#else
			*pVal = CComBSTR(PARADIGMS).Detach();
#endif // PARADIGM_INDEPENDENT
			return S_OK;
		};
	END_INTERFACE_PART(Component)

	BEGIN_INTERFACE_PART(VersionInfo, IGMEVersionInfo)
		STDMETHODIMP get_version(enum GMEInterfaceVersion *pVal);
	END_INTERFACE_PART(VersionInfo)

	BEGIN_INTERFACE_PART(SupportErrorInfo, ISupportErrorInfo)
		STDMETHODIMP InterfaceSupportsErrorInfo(REFIID riid)
		{
			if (riid == __uuidof(IMgaComponentEx) || riid == __uuidof(IMgaComponent))
			{
				return S_OK;
			}
			return S_FALSE;
		}
	END_INTERFACE_PART(SupportErrorInfo)
public:
	bool interactive;

#ifdef BUILDER_OBJECT_NETWORK
	typedef CMap<CString, LPCTSTR, CString, LPCTSTR> CStringMap;
	CStringMap parmap;
#endif

#ifdef RAWCOMPONENT_H
	RawComponent rawcomp;

#ifdef GME_ADDON
	CComPtr<IMgaEventSink> e_sink;
#endif // GME_ADDON

#endif // RAWCOMPONENT_H

#ifdef BUILDER_OBJECT_NETWORK_V2
	BON::Component 				bon2Comp;

#ifdef GME_ADDON
	CComPtr<IMgaAddOn> 		addon;
	CComPtr<IMgaEventSink> 	e_sink;
#endif // GME_ADDON

#endif // BUILDER_OBJECT_NETWORK_V2

	void HandleError( Util::Exception* pEx );
}; // CComponentObj


#ifndef __AFXPRIV_H__

class CPushRoutingFrame
{
protected:
	CFrameWnd* pOldRoutingFrame;
	_AFX_THREAD_STATE* pThreadState;

public:
	CPushRoutingFrame(CFrameWnd* pNewRoutingFrame)
	{
		pThreadState = AfxGetThreadState();
		pOldRoutingFrame = pThreadState->m_pRoutingFrame;
		pThreadState->m_pRoutingFrame = pNewRoutingFrame;
	}
	~CPushRoutingFrame()
	{ pThreadState->m_pRoutingFrame = pOldRoutingFrame; }
};

#endif // __AFXPRIV_H__

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.
