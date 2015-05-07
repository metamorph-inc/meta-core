#if !defined(AFX_INTERPRETEROBJ_H__1DBDD345_023D_11D2_BBB3_0040051F7117__INCLUDED_)
#define AFX_INTERPRETEROBJ_H__1DBDD345_023D_11D2_BBB3_0040051F7117__INCLUDED_


#include "ComHelp.h"
#include "GMECOM.h"
#include <ComponentConfig.h>

#if defined(BUILDER_OBJECT_NETWORK)
#else
// BY PAKA BEGIN
#ifdef BUILDER_OBJECT_NETWORK_V2
#include "BON.h"
#include <BON2Component.h>
#else
#include <RawComponent.h>
#endif // BUILDER_OBJECT_NETWORK_V2
// BY PAKA END
#endif // BUILDER_OBJECT_NETWORK


#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// InterpreterObj.h : header file
//



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


/////////////////////////////////////////////////////////////////////////////
// CComponentObj command target

class CComponentObj : public CCmdTarget
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

	BEGIN_INTERFACE_PART(VersionInfo, IMgaVersionInfo)
		STDMETHODIMP get_version(enum MgaInterfaceVersion *pVal);
	END_INTERFACE_PART(VersionInfo)

public:
	bool interactive;

#ifdef BUILDER_OBJECT_NETWORK
	typedef CMap<CString, LPCSTR, CString, LPCSTR> CStringMap;
	CStringMap parmap;
#endif

#ifdef RAWCOMPONENT_H
	RawComponent rawcomp;

#ifdef GME_ADDON
	CComPtr<IMgaEventSink> e_sink;
#endif // GME_ADDON

#endif // RAWCOMPONENT_H

// BY PAKA BEGIN
#ifdef BUILDER_OBJECT_NETWORK_V2
	BON::Component 				bon2Comp;

#ifdef GME_ADDON
	CComPtr<IMgaAddOn> 		addon;
	CComPtr<IMgaEventSink> 	e_sink;
#endif // GME_ADDON

	void HandleError( Util::Exception* pEx );

#endif // BUILDER_OBJECT_NETWORK_V2
// BY PAKA END

}; // CComponentObj



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



class CComponentReg
{
public:
	CComponentReg();

	CStringList paradigms;
	HRESULT RegisterParadigms(regaccessmode_enum loc = REGACCESS_USER);
	HRESULT UnregisterParadigms(regaccessmode_enum loc = REGACCESS_USER);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INTERPRETEROBJ_H__1DBDD345_023D_11D2_BBB3_0040051F7117__INCLUDED_)
