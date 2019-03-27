//////////////////////////////////////////////////////////////////////////////
// ComponentObj.cpp : implementation file
//
// In case of standard and simple components, this file does not need to be modified
// However, if anything except Component.[cpp,h] or RawComponent.[cpp,h] is to be modified,
// this is the likely and preferred candidate
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// How to convert deprecated BON invoke mechanism into new one
// SITUATION DESCRIPTION:
// The IMgaComponent interface has been extended (IMgaComponentEx), and, accordingly,
//	the BON invoke mechanism has also changed, for some time, the
//  old mechanism (old Component.h) will also be supported to a limited extent.
// The old BON invoke function was
//	void Invoke(CBuilder &builder, CBuilderObjectList &selected, long param);
// The new one is:
//	void InvokeEx(CBuilder &builder,CBuilderObject *focus, CBuilderObjectList &selected, long param);
// Differences:
//   1. The focus object and the list of selected objects is sparated int two prameters
//	 2. If IMgaComponentEx::ObjectsInvokeEx is used (currently not used by GME),
//		the FCO and folder objects are separated, and  the folders are passed in the
//			CBuilderFolder *focusfolder;
//			CBuilderFolderList selectedfolders;
//		FCO parameters are passed as function arguments to InvokeEx
// PROCEDURE
//	 1. #define NEW_BON_INVOKE in the Component.h header file
//	 2. define CComponent to include the following data and method members
//	class CComponent {
//		public:
//		CComponent() : focusfolder(NULL) { ; }
//
//		CBuilderFolder *focusfolder;
//		CBuilderFolderList selectedfolders;
//		void InvokeEx(CBuilder &builder,CBuilderObject *focus, CBuilderObjectList &selected, VARIANT *param);
//	}
//	3. Based on you Invoke() implementation implement InvokeEx()
//		(If it will only be called from GME, you may rely on the folder members being NULL,
//		 and the VARIANT parameter to contain a single lVal on invocation)
//	4. If you want to leave the Ivoke implementation, leave it in CComponent,
//     and define another macro: #define DEPRECATED_BON_INVOKE_IMPLEMENTED
//	   This will result in a call to the old invoke if the IMgaComponent::Invoke is used
//////////////////////////////////////////////////////////////////////////////


#include "StdAfx.h"

#include "MgaUtil.h"
#include "ComponentConfig.h"

#include "GMECOM.h"

#include "ComponentDll.h"
#include "ComponentObj.h"

#define _OLESTR(x) OLESTR(x)
extern const char* g_COCLASS_PROGIDA = COCLASS_PROGID;
extern const wchar_t* g_COCLASS_PROGIDW = _OLESTR(COCLASS_PROGID);
extern const char* g_COMPONENT_NAMEA = COMPONENT_NAME;
extern const wchar_t* g_COMPONENT_NAMEW = _OLESTR(COMPONENT_NAME);
#ifdef UNICODE
extern const TCHAR* g_COMPONENT_NAME = g_COMPONENT_NAMEW;
#else
extern const TCHAR* g_COMPONENT_NAME = g_COMPONENT_NAMEA;
#endif
#ifdef REGISTER_SYSTEMWIDE
extern const bool g_REGISTER_SYSTEMWIDE = true;
#else
extern const bool g_REGISTER_SYSTEMWIDE = false;
#endif
#ifdef GME_ADDON
extern const bool g_GME_ADDON = true;
#else
extern const bool g_GME_ADDON = false;
#endif
#ifdef TEST_META_CONFORMANCE_INSIDE_BON
extern const bool g_TEST_META_CONFORMANCE_INSIDE_BON = true;
#else
extern const bool g_TEST_META_CONFORMANCE_INSIDE_BON = false;
#endif

#include "Core_i.c"
#include "Mga_i.c"
#include "ComponentLib_i.c"

#ifdef BUILDER_OBJECT_NETWORK
#include "Mga.h"
#include <BONComponent.h>
#include "Builder.h"
#endif

#include "ComHelp.h"

#include "ComponentLib.h"

#include "Console.h"


#include "Gme.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif // _DEBUG

// --------------------------------------- GmeDllDesc

// Syntax:
//   interpreter,<paradigm>,<description>,<progid>
//   add-on,<paradigm>,<progid>
//
// Examples:
//   L"<GMEDLLDESC> interpreter,SF,SF Code Generator,MGA.Component.SF <END>"
//   L"<GMEDLLDESC> add-on,*,My first general add-on,MGA.GenAddon.MyAddon <END>"

#define WCHAR_L() L
#define WCHAR(PAR) WCHAR_L()PAR

#ifdef PARADIGM_INDEPENDENT
#define EXCETYPE (componenttype_enum)(CETYPE|COMPONENTTYPE_PARADIGM_INDEPENDENT)
#undef PARADIGMS
#define PARADIGMS "*"
#else // PARADIGM_INDEPENDENT
#define EXCETYPE CETYPE
#endif // PARADIGM_INDEPENDENT


#ifdef GME_ADDON
#define CETYPE	COMPONENTTYPE_ADDON
#else
#ifdef GME_INTERPRETER
#define CETYPE	COMPONENTTYPE_INTERPRETER
#else
#error No GME Componenttype (one of GME_ADDON or GME_INTERPRETER) is defined
#endif
#endif
#ifdef GME_ADDON
/////////////////////////////////////////////////////////////////////////////
// CEventSink

IMPLEMENT_DYNCREATE(CEventSink, CCmdTarget)

CEventSink::CEventSink()
{
	EnableAutomation();

	// To keep the application running as long as an OLE automation
	//	object is active, the constructor calls AfxOleLockApp.

	AfxOleLockApp();
}

CEventSink::~CEventSink()
{
	// To terminate the application when all objects created with
	// 	with OLE automation, the destructor calls AfxOleUnlockApp.

	AfxOleUnlockApp();
}

BEGIN_MESSAGE_MAP(CEventSink, CCmdTarget)
	//{{AFX_MSG_MAP(CEventSink)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CEventSink, CCmdTarget)
	//{{AFX_DISPATCH_MAP(CEventSink)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CEventSink, CCmdTarget)
	INTERFACE_PART(CEventSink, IID_IMgaEventSink, Component)
END_INTERFACE_MAP()

void CEventSink::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CCmdTarget::OnFinalRelease();
}

#define COMCLASS		CEventSink::XComponent
#define COMPROLOGUE		METHOD_PROLOGUE(CEventSink,Component)

STDMETHODIMP COMCLASS::GlobalEvent(globalevent_enum event) {
	COMPROLOGUE;
	#ifdef BUILDER_OBJECT_NETWORK_V2
		try {
			pThis->comp->bon2Comp.globalEventPerformed( event );
		}
		catch ( Util::Exception& ex ) {
			pThis->comp->HandleError( &ex );
		}
		catch ( ... ) {
			AfxMessageBox( _T("Unhandled and unknown exception was thrown in BON2Component GlobalEvent!") );
		}
		return S_OK;
	#else
		return pThis->comp->rawcomp.GlobalEvent(event);
	#endif // BUILDER_OBJECT_NETWORK_V2
}

STDMETHODIMP COMCLASS::ObjectEvent(IMgaObject * obj, unsigned long eventmask, VARIANT v) {
	COMPROLOGUE;
	#ifdef BUILDER_OBJECT_NETWORK_V2
		try {
			BON::Object object = BON::Object::attach( obj );
			for ( MON::ObjectEventType eEvent = MON::OET_ObjectCreated ; eEvent != MON::OET_All ; eEvent++ ) {
				if ( eventmask & eEvent ) {
					BON::Event event( eEvent, object );
					object->eventPerformedI( event );
					if ( eEvent & ADDON_EVENTMASK ) {
						object->getProject()->performEvent( event );
						object->performEvent( event );
					}
				}
			}
			pThis->comp->bon2Comp.objectEventPerformed( object, eventmask, v );
		}
		catch ( Util::Exception& ex ) {
			pThis->comp->HandleError( &ex );
		}
		catch ( ... ) {
			AfxMessageBox( _T("Unhandled and unknown exception was thrown in BON2Component ObjectEvent!") );
		}
		return S_OK;
	#else
		return pThis->comp->rawcomp.ObjectEvent(obj, eventmask, v);
	#endif // BUILDER_OBJECT_NETWORK_V2
}

STDMETHODIMP_(ULONG) COMCLASS::AddRef()
{
	COMPROLOGUE;
	return pThis->ExternalAddRef();
}

STDMETHODIMP_(ULONG) COMCLASS::Release()
{
	COMPROLOGUE;
	return pThis->ExternalRelease();
}

STDMETHODIMP COMCLASS::QueryInterface(REFIID riid, void** ppv)
{
	COMPROLOGUE;
	return pThis->ExternalQueryInterface(&riid, ppv);
}

#undef COMCLASS
#undef COMPROLOGUE

#endif  // GME_ADDON



/////////////////////////////////////////////////////////////////////////////
// CComponentObj

IMPLEMENT_DYNCREATE(CComponentObj, CCmdTarget)

CComponentObj::CComponentObj()
{
	EnableAutomation();

	// To keep the application running as long as an OLE automation
	//	object is active, the constructor calls AfxOleLockApp.

	AfxOleLockApp();

	registeractiveobjectret = 0;
	interactive = true;
#ifdef RAWCOMPONENT_H
	rawcomp.interactive = interactive;
#endif // RAWCOMPONENT_H
#ifdef BUILDER_OBJECT_NETWORK_V2
	bon2Comp.m_bIsInteractive = interactive;
#endif // BUILDER_OBJECT_NETWORK_V2
}

void CComponentObj::RegisterActiveObject()
{
	ASSERT( registeractiveobjectret == 0 );

	COMVERIFY( ::RegisterActiveObject(GetInterface(), CLSID_MgaComponent,
		ACTIVEOBJECT_STRONG, &registeractiveobjectret) );

	ASSERT( registeractiveobjectret );
}

CComponentObj::~CComponentObj()
{
	// To terminate the application when all objects created with
	// 	with OLE automation, the destructor calls AfxOleUnlockApp.

	ASSERT( registeractiveobjectret == 0 );
	GMEConsole::Console::gmeoleapp.Release();

	AfxOleUnlockApp();
}

void CComponentObj::UnregisterActiveObject()
{
	ASSERT( registeractiveobjectret );
	COMVERIFY( ::RevokeActiveObject(registeractiveobjectret, NULL) );
	registeractiveobjectret = 0;
}

void CComponentObj::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CCmdTarget::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CComponentObj, CCmdTarget)
	//{{AFX_MSG_MAP(CComponentObj)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CComponentObj, CCmdTarget)
	//{{AFX_DISPATCH_MAP(CComponentObj)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_DISPATCH_MAP
//	DISP_FUNCTION(CComponentObj, "Initialize", Initialize, VT_EMPTY, VTS_DISPATCH)
END_DISPATCH_MAP()

// Note: We get the type and name of the embedded class
// "XComponent" and "m_xComponent" from the "Component" parameter

BEGIN_INTERFACE_MAP(CComponentObj, CCmdTarget)
#ifndef IMPLEMENT_OLD_INTERFACE_ONLY
	INTERFACE_PART(CComponentObj, IID_IMgaComponentEx, Component)
	INTERFACE_PART(CComponentObj, IID_IDesignSpaceHelper, Component)
#endif
	INTERFACE_PART(CComponentObj, IID_IMgaComponent, Component)
	INTERFACE_PART(CComponentObj, IID_IGMEVersionInfo, VersionInfo)
#ifdef GME_COMPONENT_ISUPPORTERRORINFO
	INTERFACE_PART(CComponentObj, IID_ISupportErrorInfo, SupportErrorInfo)
#endif
END_INTERFACE_MAP()

// We register the ComponentClass
// CLSID_MGAComponentClass

#ifndef GME_COMPONENT_COM_THREAD
#define GME_COMPONENT_COM_THREAD afxRegApartmentThreading
#endif
IMPLEMENT_OLECREATE_FLAGS(CComponentObj, COCLASS_PROGID, GME_COMPONENT_COM_THREAD,
						   COCLASS_UUID_EXPLODED1,
						   COCLASS_UUID_EXPLODED2,
						   COCLASS_UUID_EXPLODED3,
						   COCLASS_UUID_EXPLODED4,
						   COCLASS_UUID_EXPLODED5,
						   COCLASS_UUID_EXPLODED6,
						   COCLASS_UUID_EXPLODED7,
						   COCLASS_UUID_EXPLODED8,
						   COCLASS_UUID_EXPLODED9,
						   COCLASS_UUID_EXPLODED10,
						   COCLASS_UUID_EXPLODED11)

/////////////////////////////////////////////////////////////////////////////
// CComponentObj::XInterface

#define COMCLASS		CComponentObj::XComponent
#define COMPROLOGUE		METHOD_PROLOGUE(CComponentObj,Component)

STDMETHODIMP_(ULONG) COMCLASS::AddRef()
{
	COMPROLOGUE;
	return pThis->ExternalAddRef();
}

STDMETHODIMP_(ULONG) COMCLASS::Release()
{
	COMPROLOGUE;
	return pThis->ExternalRelease();
}

STDMETHODIMP COMCLASS::QueryInterface(REFIID riid, void** ppv)
{
	COMPROLOGUE;
	return pThis->ExternalQueryInterface(&riid, ppv);
}


#if defined(BUILDER_OBJECT_NETWORK)
STDMETHODIMP COMCLASS::Invoke(IMgaProject *gme, IMgaFCOs *psa, long param)
{
	COMPROLOGUE;
	CPushRoutingFrame temp(NULL);		// hack!!

	ASSERT( gme != NULL );

	COMTRY {
	long prefs;
	COMTHROW(gme->get_Preferences(&prefs));

	COMTHROW(gme->put_Preferences(prefs | MGAPREF_RELAXED_RDATTRTYPES | MGAPREF_RELAXED_WRATTRTYPES));
	CBuilder builder(gme);

	CBuilderObjectList objects;
	if(psa) {
		MGACOLL_ITERATE(IMgaFCO, psa) {
			CBuilderObject* o = CBuilder::theInstance->FindObject(MGACOLL_ITER);
			ASSERT( o != NULL );
			objects.AddTail(o);
		} MGACOLL_ITERATE_END;
	}

#ifndef DEPRECATED_BON_INVOKE_IMPLEMENTED
	CComponent comp;
	CBuilderObject *focus;
	POSITION p = objects.GetHeadPosition();
	if(p) {
		focus = objects.GetAt(p);
		objects.RemoveAt(p);
	}
	comp.InvokeEx(builder, focus, objects, param);
#else
#ifdef BUILDER_OBJECT_NETWORK
	CComponent comp;
	comp.Invoke(builder, objects, param);
#else
	CInterpreter intp;
	intp.Interpret(builder, objects, param);
#endif
#endif

	COMTHROW(gme->put_Preferences(prefs));
	} COMCATCH(;)
	return S_OK;
}


STDMETHODIMP COMCLASS::InvokeEx( IMgaProject *project,  IMgaFCO *currentobj,  IMgaFCOs *selectedobjs,  long param) {
	COMPROLOGUE;
	CPushRoutingFrame temp(NULL);		// hack!!
	COMTRY {
#ifdef NEW_BON_INVOKE
	ASSERT( project != NULL );

	long prefs;
	COMTHROW(project->get_Preferences(&prefs));

	COMTHROW(project->put_Preferences(prefs | MGAPREF_RELAXED_RDATTRTYPES | MGAPREF_RELAXED_WRATTRTYPES));

	CComPtr<IMgaTerritory> terr;
	COMTHROW(project->CreateTerritory(NULL, &terr));
	COMTHROW(project->BeginTransaction(terr));
	{
		try	{
			CBuilder builder(project, &pThis->parmap);
			CBuilderObject *focus = NULL;
			if (currentobj) {
				CComPtr<IMgaFCO> currobj;
				COMCHECK2(terr, terr->OpenFCO(currentobj,&currobj));
				focus = CBuilder::theInstance->FindObject(currobj);
				ASSERT( focus != NULL );
			}
			CBuilderObjectList objects;
			if(selectedobjs) {
			  CComPtr<IMgaFCOs> fcos;
			  COMCHECK2(terr, terr->OpenFCOs(selectedobjs,&fcos));
			  MGACOLL_ITERATE(IMgaFCO, fcos) {
				CBuilderObject* o = CBuilder::theInstance->FindObject(MGACOLL_ITER);
				ASSERT( o != NULL );
				objects.AddTail(o);
			  } MGACOLL_ITERATE_END;
			}

#ifdef BON_CUSTOM_TRANSACTIONS
			COMCHECK2(project, project->CommitTransaction());
#endif

			CComponent comp;
			comp.InvokeEx(builder, focus, objects, param);

#ifndef BON_CUSTOM_TRANSACTIONS
			COMCHECK2(project, project->CommitTransaction());
#endif
		} catch (Util::Exception e) {
			pThis->HandleError(&e);
			COMTHROW(project->AbortTransaction());
			COMTHROW(E_FAIL);
		} catch (HRESULT) {
			COMTHROW(project->AbortTransaction());
			throw;
		} catch(...) {
			AfxMessageBox(_T("Unhandled and unknown exception was thrown in BON2Component Invoke!"));
			COMTHROW(project->AbortTransaction());
			COMTHROW(E_FAIL);
		}
	}
	COMTHROW(project->put_Preferences(prefs));
#else
		if(pThis->interactive) {
			AfxMessageBox(_T("This interpreter has been upgraded to the new component interface \n")
						   _T("but the user-provided files (Component.cpp and Component.h) \n")
						   _T("use the deprecated interface. You can proceed now, but you are adviced to\n")
						   _T("#define NEW_BON_INVOKE in Component.h, and implement other\n")
						   _T("modifications as described at the top of the ComponentObj.cpp file"));
		}
		CComPtr<IMgaTerritory> terr;
		COMTHROW(project->CreateTerritory(NULL, &terr));
		COMTHROW(project->BeginTransaction(terr));
		try {
			CComPtr<IMgaFCOs> objs;
			if(currentobj) {
				COMTHROW(currentobj->CreateCollection(&objs));
			}
			else objs = selectedobjs;
			long param = 0;
			COMTHROW(Invoke(project, objs, param));
			COMTHROW(project->CommitTransaction());
		}
		catch(...) {
			project->AbortTransaction();
			throw;
		}
#endif
	} COMCATCH(;);
}

STDMETHODIMP COMCLASS::ObjectsInvokeEx( IMgaProject *project,  IMgaObject *currentobj,  IMgaObjects *selectedobjs,  long param) {
  COMPROLOGUE;
  CPushRoutingFrame temp(NULL);		// hack!!
  COMTRY {
#ifdef NEW_BON_INVOKE
	ASSERT( project != NULL );

	long prefs;
	COMTHROW(project->get_Preferences(&prefs));

	COMTHROW(project->put_Preferences(prefs | MGAPREF_RELAXED_RDATTRTYPES | MGAPREF_RELAXED_WRATTRTYPES));
	CComPtr<IMgaTerritory> terr;
	COMTHROW(project->CreateTerritory(NULL, &terr));
	COMTHROW(project->BeginTransaction(terr));
	try {
		{
			CBuilder builder(project);

			CComponent comp;
			const CBuilderFolderList *allfolders = builder.GetFolders();
			CBuilderObject *focus =  NULL;
			CComPtr<IMgaObject> currobj;
			COMTHROW(terr->OpenObj(currentobj,&currobj));
			CComQIPtr<IMgaFCO> co = currobj;
			if (co) {
				focus = CBuilder::theInstance->FindObject(co);
				ASSERT( focus != NULL );
			}
			else if(currobj) {
				CComQIPtr<IMgaFolder> cf = currentobj;
				POSITION p = allfolders->GetHeadPosition();
				while(p) {
					CBuilderFolder *f = allfolders->GetNext(p);
					if(f->GetIFolder() == cf) {
						comp.focusfolder = f;
						break;
					}
				}
				ASSERT(comp.focusfolder != NULL);
			}
			CBuilderObjectList objects;
			if(selectedobjs) {
			  MGACOLL_ITERATE(IMgaObject, selectedobjs) {
				CComPtr<IMgaObject> currobj;
				COMTHROW(terr->OpenObj(MGACOLL_ITER,&currobj));
				CComQIPtr<IMgaFCO> co = currobj;
				if (co) {
					CBuilderObject* o = CBuilder::theInstance->FindObject(co);
					ASSERT( o != NULL );
					objects.AddTail(o);
				}
				else if(currobj) {
					CComQIPtr<IMgaFolder> cf = currentobj;
					CBuilderFolder *sf = NULL;
					POSITION p = allfolders->GetHeadPosition();
					while(p) {
						CBuilderFolder *f = allfolders->GetNext(p);
						if(f->GetIFolder() == cf) {
							sf = f;
							break;
						}
					}
					ASSERT( sf != NULL );
					comp.selectedfolders.AddTail(sf);
				}
			  } MGACOLL_ITERATE_END;
			}

			comp.InvokeEx(builder, focus, objects, param);
		}
		COMTHROW(project->CommitTransaction());
	}	catch(...) { project->AbortTransaction(); throw; }
	COMTHROW(project->put_Preferences(prefs));
#else
		if(pThis->interactive) {
			AfxMessageBox(_T"This interpreter has been upgraded to the new component interface \n")
						   _T("but the user-provided files (Component.cpp and Component.h) \n")
						   _T("use the deprecated interface.\n")
						   _T("#define NEW_BON_INVOKE in Component.h, and implement other\n")
						   _T("modifications as described at the top of the ComComponent.cpp file"));
		}
		return E_MGA_NOT_IMPLEMENTED;
#endif
  }	COMCATCH(;);
}



// You may also want to modify the implementations for the following methods

STDMETHODIMP COMCLASS::Initialize(struct IMgaProject *p) {
	COMTRY {
		GMEConsole::Console::SetupConsole(p);
	} COMCATCH(;);
};

STDMETHODIMP COMCLASS::Enable(VARIANT_BOOL newVal) {
	return S_OK;
};
STDMETHODIMP COMCLASS::get_InteractiveMode(VARIANT_BOOL *enabled) {
	COMPROLOGUE;
	if(enabled) *enabled = pThis->interactive ? VARIANT_TRUE : VARIANT_FALSE;
	return S_OK;
};

STDMETHODIMP COMCLASS::put_InteractiveMode(VARIANT_BOOL enabled) {
	COMPROLOGUE;
	pThis->interactive = (enabled == VARIANT_TRUE);
	return S_OK;
};

STDMETHODIMP COMCLASS::get_ComponentParameter(BSTR name, VARIANT *pVal) {
	COMPROLOGUE;
	CComVariant vv;
	CString bb;
	if (pThis->parmap.Lookup(CString(name), bb)) {
		vv = CComBSTR(bb);
		vv.Detach(pVal);
	}
    // FIXME: pVal needs to be set in all cases
	return S_OK;
}
STDMETHODIMP COMCLASS::put_ComponentParameter(BSTR name, VARIANT newVal) {
	COMPROLOGUE;
	CComVariant dest;
	HRESULT hr = ::VariantChangeType(&dest, &newVal, VARIANT_NOVALUEPROP, VT_BSTR);
	if (hr == S_OK) pThis->parmap.SetAt(CString(name), CString(dest.bstrVal));
	return hr;
}


#else // BUILDER_OBJECT_NETWORK

#ifdef BUILDER_OBJECT_NETWORK_V2
// If BUILDER OBJECT NETWORK 2 IS USED

STDMETHODIMP COMCLASS::Invoke( IMgaProject *gme, IMgaFCOs *psa, long param )
{
	COMPROLOGUE;
	CPushRoutingFrame temp( NULL ); // hack!!

	try  {
		ASSERT( gme != NULL );
		BON::Project project = BON::Project::attach( gme );

		std::set<BON::FCO> setFCOs;
		if ( psa ) {
			MGACOLL_ITERATE( IMgaFCO, psa ) {
				setFCOs.insert( BON::FCO::attach( MGACOLL_ITER ) );
			} MGACOLL_ITERATE_END;
		}

		pThis->bon2Comp.invoke( project, setFCOs, param );
	}
	catch ( Util::Exception& ex ) {
		pThis->HandleError( &ex );
	}
	catch ( ... ) {
		AfxMessageBox( _T("Unhandled and unknown exception was thrown in BON2Component Invoke!") );
	}

	return S_OK;
}

STDMETHODIMP COMCLASS::InvokeEx( IMgaProject *gme,  IMgaFCO *currentobj,  IMgaFCOs *selectedobjs,  long param)
{
	COMPROLOGUE;
	CPushRoutingFrame temp( NULL ); // hack!!

	COMTRY {
	ASSERT( gme != NULL );

	long prefs;
	COMTHROW( gme->get_Preferences( &prefs ) );
	COMTHROW( gme->put_Preferences( prefs | MGAPREF_RELAXED_RDATTRTYPES | MGAPREF_RELAXED_WRATTRTYPES ) );

	CComPtr<IMgaTerritory> spTerritory;
	COMTHROW( gme->CreateTerritory( NULL, &spTerritory ) );
	COMTHROW( gme->BeginTransaction( spTerritory ) );

	try {
		BON::Project project = BON::Project::attach( gme );
		project->assignTerritory(spTerritory);
		
		CComPtr<IMgaFCO> spFCO;
		if ( currentobj )
			COMTHROW( spTerritory->OpenFCO( currentobj, &spFCO ) );
		BON::FCO fco = BON::FCO::attach( spFCO );

		std::set<BON::FCO> setFCOs;
		if ( selectedobjs ) {
			CComPtr<IMgaFCOs> spFCOs;
			COMTHROW( spTerritory->OpenFCOs( selectedobjs, &spFCOs ) );
			MGACOLL_ITERATE( IMgaFCO, spFCOs ) {
				setFCOs.insert( BON::FCO::attach( MGACOLL_ITER ) );
			} MGACOLL_ITERATE_END;
		}

		#ifdef BON_CUSTOM_TRANSACTIONS
			COMTHROW( gme->CommitTransaction() );
		#endif

		pThis->bon2Comp.invokeEx( project, fco, setFCOs, param );

		#ifndef BON_CUSTOM_TRANSACTIONS
			COMTHROW( gme->CommitTransaction() );
		#endif
	}
	catch ( Util::Exception& ex ) {
		pThis->HandleError( &ex );
		#ifndef BON_CUSTOM_TRANSACTIONS
			COMTHROW( gme->AbortTransaction() );
		#endif
	}
	catch ( ... ) {
		AfxMessageBox( _T("Unhandled and unknown exception was thrown in BON2Component InvokeEx!") );
		#ifndef BON_CUSTOM_TRANSACTIONS
			COMTHROW( gme->AbortTransaction() );
		#endif
	}
	} COMCATCH(;)
}

STDMETHODIMP COMCLASS::ObjectsInvokeEx( IMgaProject *gme,  IMgaObject *currentobj,  IMgaObjects *selectedobjs,  long param)
{
	COMPROLOGUE;
	CPushRoutingFrame temp( NULL ); // hack!!

	ASSERT( gme != NULL );

	COMTRY {
	long prefs;
	COMTHROW( gme->get_Preferences( &prefs ) );
	COMTHROW( gme->put_Preferences( prefs | MGAPREF_RELAXED_RDATTRTYPES | MGAPREF_RELAXED_WRATTRTYPES ) );

	CComPtr<IMgaTerritory> spTerritory;
	COMTHROW( gme->CreateTerritory( NULL, &spTerritory ) );
	COMTHROW( gme->BeginTransaction( spTerritory ) );

	BON::Project project = BON::Project::attach( gme );
	project->assignTerritory(spTerritory);

	CComPtr<IMgaObject> spObject;
	if ( currentobj )
		COMTHROW( spTerritory->OpenObj( currentobj, &spObject ) );
	BON::Object object = BON::Object::attach( spObject );

	std::set<BON::Object> setObjects;
	if ( selectedobjs ) {
		MGACOLL_ITERATE( IMgaObject, selectedobjs ) {
			CComPtr<IMgaObject> spObject;
			COMTHROW( spTerritory->OpenObj( MGACOLL_ITER, &spObject ) );
			setObjects.insert( BON::Object::attach( spObject ) );
		} MGACOLL_ITERATE_END;
	}

	try {
		pThis->bon2Comp.objectInvokeEx( project, object, setObjects, param );
		COMTHROW( gme->CommitTransaction() );
	}
	catch ( Util::Exception& ex ) {
		pThis->HandleError( &ex );
		COMTHROW( gme->AbortTransaction() );
	}
	catch ( ... ) {
		AfxMessageBox( _T("Unhandled and unknown exception was thrown in BON2Component ObjectInvokeEx!") );
		COMTHROW( gme->AbortTransaction() );
	}
	} COMCATCH(;)

	return S_OK;
}

STDMETHODIMP COMCLASS::Initialize( struct IMgaProject *p )
{
	COMTRY {
		COMPROLOGUE;

		#ifdef GME_ADDON
			CEventSink *es = new CEventSink;
			pThis->e_sink = &es->m_xComponent;
			es->comp = pThis;

			pThis->addon = NULL;
			COMTHROW( p->CreateAddOn( pThis->e_sink, &( pThis->addon ) ) );
			COMTHROW( pThis->addon->put_EventMask( ADDON_EVENTMASK ) );
		#endif

		GMEConsole::Console::SetupConsole(p);

		CComPtr<IMgaTerritory> spTerritory;
		COMTHROW( p->CreateTerritory( NULL, &spTerritory ) );
		COMTHROW( p->BeginTransaction( spTerritory ) );

		BON::Project project = BON::Project::attach( p );
		pThis->bon2Comp.m_project = project;
		try {
			pThis->bon2Comp.initialize( project );
		}
		catch ( Util::Exception& ex ) {
			pThis->HandleError( &ex );
		}
		catch ( ... ) {
			AfxMessageBox( _T("Unhandled and unknown exception was thrown in BON2Component Initialization!") );
		}

		COMTHROW( p->CommitTransaction() );

		return S_OK;
	} COMCATCH(;);
}

STDMETHODIMP COMCLASS::Enable( VARIANT_BOOL newVal )
{
	return S_OK;
}

STDMETHODIMP COMCLASS::get_InteractiveMode( VARIANT_BOOL *enabled )
{
	COMPROLOGUE;
	if( enabled )
		*enabled = pThis->interactive ? VARIANT_TRUE : VARIANT_FALSE;
	return S_OK;
}

STDMETHODIMP COMCLASS::put_InteractiveMode( VARIANT_BOOL enabled )
{
	COMPROLOGUE;
	pThis->interactive = enabled == VARIANT_TRUE;
	pThis->bon2Comp.m_bIsInteractive = pThis->interactive;
	return S_OK;
}

STDMETHODIMP COMCLASS::get_ComponentParameter( BSTR name, VARIANT *pVal )
{
	COMPROLOGUE;
	std::string strValue = pThis->bon2Comp.getParameter( Util::Copy( CComBSTR( name ) ) );
	CComVariant vv = Util::Copy( strValue );
	vv.Detach( pVal );
	return S_OK;
}

STDMETHODIMP COMCLASS::put_ComponentParameter( BSTR name, VARIANT newVal )
{
	COMPROLOGUE;
	CComVariant dest;
	HRESULT hr = ::VariantChangeType( &dest, &newVal, VARIANT_NOVALUEPROP, VT_BSTR );
	if( hr == S_OK )
		pThis->bon2Comp.setParameter( Util::Copy( CComBSTR( name ) ), Util::Copy( CComBSTR( dest.bstrVal ) ) );
	return hr;
}

#else

// If BUILDER OBJECT NETWORK IS NOT USED, THESE METHODS ARE CALLS INTO RawComponent

#ifndef RAWCOMPONENT_H
#error This part should be visible only in the RAW Component case
#endif

STDMETHODIMP COMCLASS::Invoke(IMgaProject *gme, IMgaFCOs *psa, long param)
{
	COMPROLOGUE;
	CPushRoutingFrame temp(NULL);		// hack!!

	ASSERT( gme != NULL );

	COMTRY {
		return pThis->rawcomp.Invoke(gme, psa, param);
	} COMCATCH(;);
}


STDMETHODIMP COMCLASS::InvokeEx( IMgaProject *gme,  IMgaFCO *currentobj,  IMgaFCOs *selectedobjs,  long param) {
	COMPROLOGUE;
	CPushRoutingFrame temp(NULL);		// hack!!

	ASSERT( gme != NULL );

	COMTRY {
		return pThis->rawcomp.InvokeEx(gme, currentobj, selectedobjs, param);
	} COMCATCH(;);
}

STDMETHODIMP COMCLASS::ApplyConstraintsAndGenerateCWCs(IMgaProject *project, IMgaFCO *currentobj, VARIANT_BOOL applyConstraints) {
	COMPROLOGUE;
	CPushRoutingFrame temp(NULL);		// hack!!

	COMTRY {
		pThis->rawcomp.interactive = false;
		return pThis->rawcomp.Main(project, currentobj, applyConstraints != VARIANT_FALSE);
	} COMCATCH(;);
}

STDMETHODIMP COMCLASS::ExportDesertXML(IMgaProject *project, IMgaFCO *currentObj, BSTR exportPath) {
	COMPROLOGUE;
	CPushRoutingFrame temp(NULL);		// hack!!

	COMTRY{
		return pThis->rawcomp.ExportDesertXML(project, currentObj, exportPath);
	} COMCATCH(;);
}

STDMETHODIMP COMCLASS::ImportConfigsFromXML(IMgaProject *project, IMgaFCO *designContainer, BSTR desertXmlPath, BSTR desertBackXmlPath, BSTR *exportedConfigurationsName)
{
	COMPROLOGUE;
	CPushRoutingFrame temp(NULL);		// hack!!

	COMTRY{
		return pThis->rawcomp.ImportConfigsFromXML(project, designContainer, desertXmlPath, desertBackXmlPath, exportedConfigurationsName);
	} COMCATCH(;);
}

STDMETHODIMP COMCLASS::ObjectsInvokeEx( IMgaProject *gme,  IMgaObject *currentobj,  IMgaObjects *selectedobjs,  long param) {
	COMPROLOGUE;
	CPushRoutingFrame temp(NULL);		// hack!!

	ASSERT( gme != NULL );


	COMTRY {
		return pThis->rawcomp.ObjectsInvokeEx(gme, currentobj, selectedobjs, param);
	} COMCATCH(;);
}

// You may also want to modify the implementations for the following methods

STDMETHODIMP COMCLASS::Initialize(struct IMgaProject *p) {
	COMTRY {
		COMPROLOGUE;
#ifdef GME_ADDON

		CEventSink *es = new CEventSink;
		pThis->e_sink = &es->m_xComponent;
		es->comp = pThis;

		pThis->rawcomp.addon = NULL;
		COMTHROW(p->CreateAddOn(pThis->e_sink, &(pThis->rawcomp.addon)) );
		COMTHROW(pThis->rawcomp.addon->put_EventMask(ADDON_EVENTMASK));

#endif
		// FIXME: This always fails in Addons (E_MGA_NAME_NOT_FOUND)
		GMEConsole::Console::SetupConsole(p);
		return pThis->rawcomp.Initialize(p);
	} COMCATCH(;);
};

STDMETHODIMP COMCLASS::Enable(VARIANT_BOOL newVal) {
	return S_OK;
};
STDMETHODIMP COMCLASS::get_InteractiveMode(VARIANT_BOOL *enabled) {
	COMPROLOGUE;
	if (enabled)
		*enabled = pThis->interactive ? VARIANT_TRUE : VARIANT_FALSE;
	return S_OK;
};

STDMETHODIMP COMCLASS::put_InteractiveMode(VARIANT_BOOL enabled) {
	COMPROLOGUE;
	pThis->interactive = (enabled == VARIANT_TRUE);
#ifdef RAWCOMPONENT_H
	pThis->rawcomp.interactive = pThis->interactive;
#endif
	return S_OK;
};

STDMETHODIMP COMCLASS::get_ComponentParameter( BSTR name, VARIANT *pVal )
{
	COMPROLOGUE;
	COMTRY {
		pThis->rawcomp.get_ComponentParameter(name, pVal);
	} COMCATCH(;);
}

STDMETHODIMP COMCLASS::put_ComponentParameter( BSTR name, VARIANT newVal )
{
	COMPROLOGUE;
	COMTRY {
		return pThis->rawcomp.put_ComponentParameter(name, newVal);
	} COMCATCH(;);
}

#endif // BUILDER_OBJECT_NETWORK_V2

#endif // BUILDER_OBJECT_NETWORK

void CComponentObj::HandleError( Util::Exception* pEx )
{
	std::string strOut = "Exception kind : " + pEx->getKind() + "\n";
	strOut += "Exception message : " + pEx->getErrorMessage();
	if ( pEx->getKind() == "MON::Exception" ) {
		char chBuffer[ 100 ];
		sprintf_s( chBuffer, "%x", ( (MON::Exception*) pEx)->getHResult() );
		strOut += "\nException hresult : " + std::string( chBuffer );
	}
	else if ( pEx->getKind() == "BON::Exception" ) {
		char chBuffer[ 100 ];
		sprintf_s( chBuffer, "%x", ( (BON::Exception*) pEx)->getHResult() );
		strOut += "\nException hresult : " + std::string( chBuffer );
	}
	AfxMessageBox( CString(strOut.c_str()) );
}


STDMETHODIMP COMCLASS::get_ComponentType( componenttype_enum *t)
{
	COMPROLOGUE;
	*t = EXCETYPE;
	return S_OK;
}

#undef COMCLASS
#undef COMPROLOGUE

/////////////////////////////////////////////////////////////////////////////
// CComponentObj::XVersionInfo

#define COMCLASS		CComponentObj::XVersionInfo
#define COMPROLOGUE		METHOD_PROLOGUE(CComponentObj,VersionInfo)

STDMETHODIMP_(ULONG) COMCLASS::AddRef()
{
	COMPROLOGUE;
	return pThis->ExternalAddRef();
}

STDMETHODIMP_(ULONG) COMCLASS::Release()
{
	COMPROLOGUE;
	return pThis->ExternalRelease();
}

STDMETHODIMP COMCLASS::QueryInterface(REFIID riid, void** ppv)
{
	COMPROLOGUE;
	return pThis->ExternalQueryInterface(&riid, ppv);
}

STDMETHODIMP COMCLASS::get_version(enum GMEInterfaceVersion *pVal)
{
	COMPROLOGUE;

	if( pVal == NULL )
		return E_POINTER;

	*pVal = GMEInterfaceVersion_Current;
	return S_OK;
}

#undef COMCLASS
#undef COMPROLOGUE

/////////////////////////////////////////////////////////////////////////////
// CComponentObj::XSupportErrorInfo

#define COMCLASS		CComponentObj::XSupportErrorInfo
#define COMPROLOGUE		METHOD_PROLOGUE(CComponentObj, SupportErrorInfo)

STDMETHODIMP_(ULONG) COMCLASS::AddRef()
{
	COMPROLOGUE;
	return pThis->ExternalAddRef();
}

STDMETHODIMP_(ULONG) COMCLASS::Release()
{
	COMPROLOGUE;
	return pThis->ExternalRelease();
}

STDMETHODIMP COMCLASS::QueryInterface(REFIID riid, void** ppv)
{
	COMPROLOGUE;
	return pThis->ExternalQueryInterface(&riid, ppv);
}

#undef COMCLASS
#undef COMPROLOGUE

// --------------------------- CComponentReg

CComponentReg::CComponentReg()
{
	CString pars = PARADIGMS;
#ifndef PARADIGM_INDEPENDENT
	while( !pars.IsEmpty() )
	{
		CString trash = pars.SpanIncluding(_T(" ,;"));
		pars = pars.Mid(trash.GetLength());
		if( pars.IsEmpty() )
			break;
		CString par = pars.SpanExcluding(_T(" ,;"));
		pars = pars.Mid(par.GetLength());
		ASSERT(!par.IsEmpty());
		paradigms.AddTail(par);
	}
#endif // PARADIGM_INDEPENDENT
}


#define COMRETURN(hr) { HRESULT res; if((res = (hr)) != S_OK) { ASSERT(false); return res; } }


HRESULT CComponentReg::UnregisterParadigms(regaccessmode_enum loc) {
	CComPtr<IMgaRegistrar> registrar;
	COMRETURN(registrar.CoCreateInstance(OLESTR("Mga.MgaRegistrar")));
	COMRETURN(registrar->UnregisterComponent(CComBSTR(COCLASS_PROGID), loc));
	return S_OK;
}

HRESULT CComponentReg::RegisterParadigms(regaccessmode_enum loc) {
	CComPtr<IMgaRegistrar> registrar;
	COMRETURN(registrar.CoCreateInstance(OLESTR("Mga.MgaRegistrar")));
	COMRETURN(registrar->RegisterComponent(CComBSTR(COCLASS_PROGID),EXCETYPE, CComBSTR(COMPONENT_NAME), loc));
#ifdef BON_ICON_SUPPORT
	COMRETURN(registrar->put_ComponentExtraInfo(loc, CComBSTR(COCLASS_PROGID), CComBSTR("Icon"), CComBSTR(",IDI_COMPICON")));
#endif
    COMRETURN(registrar->put_ComponentExtraInfo(loc, CComBSTR(COCLASS_PROGID), CComBSTR("Tooltip"), CComBSTR(TOOLTIP_TEXT)));

	POSITION pos = paradigms.GetHeadPosition();
	while(pos)
	{
		CString paradigm = paradigms.GetNext(pos);
		COMRETURN(registrar->Associate(CComBSTR(COCLASS_PROGID), CComBSTR(paradigm), loc));
	}
	return S_OK;
}