///////////////////////////////////////////////////////////////////////////
// RawComponent.cpp, the main RAW COM component implementation file
// This is the file (along with its header RawComponent.h)
// that the component implementor is expected to modify in the first place
//
///////////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#include <ComHelp.h>
#include <GMECOM.h>
#include "ComponentConfig.h"
#include "RawComponent.h"

// Console
#include "UdmConsole.h"


// Udm includes
#include "UdmBase.h"
#include "Uml.h"
#include "UmlExt.h"

using namespace std;

#ifdef _USE_DOM
	#include "UdmDOM.h"
#endif

#include "UdmGme.h"
#include "UdmStatic.h"
#include "UdmUtil.h"

#include "UdmApp.h"
#include "UdmConfig.h"

__declspec(noreturn) void ThrowComError(HRESULT hr, LPOLESTR err);

// this method is called after all the generic initialization is done
// this should be empty, unless application-specific initialization is needed
STDMETHODIMP RawComponent::Initialize(struct IMgaProject *) {
	return S_OK;
}

// this is the obsolete component interface
// this present implementation either tries to call InvokeEx, or returns an error;
STDMETHODIMP RawComponent::Invoke(IMgaProject* gme, IMgaFCOs *models, long param) {
#ifdef SUPPORT_OLD_INVOKE
	CComPtr<IMgaFCO> focus;
	CComVariant parval = param;
	return InvokeEx(gme, focus, selected, parvar);
#else
	if(interactive) {
		AfxMessageBox("This component does not support the obsolete invoke mechanism");
	}
	return E_MGA_NOT_SUPPORTED;
#endif
}

#ifdef _DYNAMIC_META
			void dummy(void) {; } // Dummy function for UDM meta initialization
#endif


// This is the main component method for interpereters and plugins. 
// May als be used in case of invokeable addons
STDMETHODIMP RawComponent::InvokeEx( IMgaProject *project,  IMgaFCO *currentobj,  
									IMgaFCOs *selectedobjs,  long param) 
{
	// Calling the user's initialization function
//	if(CUdmApp::Initialize())
//	{
//		return S_FALSE;
//	}

	CComPtr<IMgaProject>ccpProject(project);
	
	try
	{
		// Setting up the console
		GMEConsole::Console::setupConsole(ccpProject);
		GMEConsole::Console::clear();

		char tmpbuf[128];
		_strdate_s(tmpbuf,128);
		std::string date(tmpbuf);
		_strtime_s(tmpbuf,128);
		std::string time(tmpbuf);

		GMEConsole::Console::writeLine(date + " " + time + " Starting CAD2CyPhy Interpreter", MSG_INFO);

	  if(interactive)
	  {
		CComBSTR projname;
		CComBSTR focusname = "<nothing>";
		CComPtr<IMgaTerritory> terr;
		COMTHROW(ccpProject->CreateTerritory(NULL, &terr));

		// Setting up Udm
#ifdef _DYNAMIC_META
	#ifdef _DYNAMIC_META_DOM
			// Loading the meta for the project
			UdmDom::DomDataNetwork  ddnMeta(Uml::diagram);
			Uml::Diagram theUmlDiagram;

			// Opening the XML meta of the project
			ddnMeta.OpenExisting(META_PATH,"uml.dtd", Udm::CHANGES_LOST_DEFAULT);

			// Casting the DataNetwork to diagram
			theUmlDiagram = Uml::Diagram::Cast(ddnMeta.GetRootObject());

			// Creating the UDM diagram
			Udm::UdmDiagram udmDataDiagram;
			udmDataDiagram.dgr = &theUmlDiagram;
			udmDataDiagram.init = dummy;

	#elif defined _DYNAMIC_META_STATIC
			// Loading the meta for the project
			Udm::SmartDataNetwork  dnsMeta(Uml::diagram);
			Uml::Diagram theUmlDiagram;

			// Opening the static meta of the project
			dnsMeta.OpenExisting(META_PATH, "", Udm::CHANGES_LOST_DEFAULT);

			// Casting the DataNetwork to diagram
			theUmlDiagram = Uml::Diagram::Cast(dnsMeta.GetRootObject());

			// Creating the UDM diagram
			Udm::UdmDiagram udmDataDiagram;
			udmDataDiagram.dgr = &theUmlDiagram;
			udmDataDiagram.init = dummy;

	#else
			#error "Neither _DYNAMIC_META_DOM or _DYNAMIC_META_STATIC defined for dynamic loading"
	#endif
			// Loading the project
			UdmGme::GmeDataNetwork dngBackend(udmDataDiagram);

#else
		using namespace META_NAMESPACE;

		// Loading the project
		UdmGme::GmeDataNetwork dngBackend(META_NAMESPACE::diagram);

#endif
		try
		{
			// Opening backend
			dngBackend.OpenExisting(ccpProject, Udm::CHANGES_LOST_DEFAULT);


			CComPtr<IMgaFCO> ccpFocus(currentobj);
			Udm::Object currentObject;
			if(ccpFocus)
			{
				currentObject=dngBackend.Gme2Udm(ccpFocus);
			}

			std::set<Udm::Object> selectedObjects;

			CComPtr<IMgaFCOs> ccpSelObject(selectedobjs);

			MGACOLL_ITERATE(IMgaFCO,ccpSelObject){
				Udm::Object currObj;
				if(MGACOLL_ITER)
				{
					currObj=dngBackend.Gme2Udm(MGACOLL_ITER);
				}
			 selectedObjects.insert(currObj);
			}MGACOLL_ITERATE_END;

#ifdef _ACCESS_MEMORY
			// Creating Cache
	#ifdef _DYNAMIC_META
			Udm::SmartDataNetwork dnsCacheBackend(udmDataDiagram);
	#else
			Udm::SmartDataNetwork dnsCacheBackend(META_NAMESPACE::diagram);
	#endif

			const Uml::Class & safeType = Uml::SafeTypeContainer::GetSafeType(dngBackend.GetRootObject().type());

			dnsCacheBackend.CreateNew("tmp.mem","",safeType, Udm::CHANGES_LOST_DEFAULT);

			Udm::Object nullObject(&Udm::_null);
			UdmUtil::copy_assoc_map copyAssocMap;
			copyAssocMap[currentObject]=nullObject; // currentObject may be null object

			std::set<Udm::Object>::iterator p_CurrSelObject;
			for(p_CurrSelObject=selectedObjects.begin();
				p_CurrSelObject!=selectedObjects.end();p_CurrSelObject++)
			{
					std::pair<Udm::Object const, Udm::Object> item(*p_CurrSelObject, nullObject);

					std::pair<UdmUtil::copy_assoc_map::iterator, bool> insRes = copyAssocMap.insert(item);

					if (!insRes.second)
					{
						assert(false);
					}

			}

			// Copying from GME to memory
			UdmUtil::CopyObjectHierarchy(
				dngBackend.GetRootObject().__impl(),
				dnsCacheBackend.GetRootObject().__impl(),
				&dnsCacheBackend,
				copyAssocMap);

			// Searching for focus object
			Udm::Object currentObjectCache;
			UdmUtil::copy_assoc_map::iterator currObject = copyAssocMap.find(currentObject);
			if (currObject != copyAssocMap.end()) // It is in the map
			{
				currentObjectCache=currObject->second;
			}


			// Searching for selected objects
			std::set<Udm::Object> selectedObjectsCache;

			for( p_CurrSelObject=selectedObjects.begin();
				p_CurrSelObject!=selectedObjects.end();p_CurrSelObject++)
			{
				Udm::Object object;
				UdmUtil::copy_assoc_map::iterator currSelObjectIt = copyAssocMap.find(*p_CurrSelObject);
				if (currSelObjectIt != copyAssocMap.end()) // It is in the map
				{
					object=currSelObjectIt->second;
					selectedObjectsCache.insert(object);
				}
			}


			// Closing GME backend
			dngBackend.CloseNoUpdate();

			// Calling the main entry point
			CUdmApp::UdmMain(&dnsCacheBackend,currentObjectCache,selectedObjectsCache,param);
			// Close cache backend
			dnsCacheBackend.CloseNoUpdate();

#else
			// Calling the main entry point
			CUdmApp::UdmMain(&dngBackend,currentObject,selectedObjects,param);
			// Closing backend
			dngBackend.CloseWithUpdate();
#endif

		}
		catch(udm_exception &exc)
		{
#ifdef _META_ACCESS_MEMORY
			dnCacheBackend.CloseNoUpdate();
#endif
			// Close GME Backend (we may close it twice, but GmeDataNetwork handles it)
			dngBackend.CloseNoUpdate();

			GMEConsole::Console::Error::writeLine(exc.what());
			return S_FALSE;
		}
	  }
	}
	catch (udm_exception &e) {
		GMEConsole::Console::gmeoleapp = 0;
		std::string msg = "Udm error: ";
		msg += e.what();
		ThrowComError(E_FAIL, _bstr_t(msg.c_str()));
	}
	catch(...)
	{
		ccpProject->AbortTransaction();
		// This can be a problem with the GME Console, so we display it in a message box
		AfxMessageBox("An unexpected error has occurred during the interpretation process.");
		return E_FAIL;
	}

	GMEConsole::Console::writeLine("End of CyPhy2CAD Interpreter", MSG_INFO);
	GMEConsole::Console::freeConsole();
	return S_OK;

}

// GME currently does not use this function
// you only need to implement it if other invokation mechanisms are used
STDMETHODIMP RawComponent::ObjectsInvokeEx( IMgaProject *project,  IMgaObject *currentobj,  IMgaObjects *selectedobjs,  long param) {
	if(interactive) {
		AfxMessageBox("Tho ObjectsInvoke method is not implemented");
	}
	return E_MGA_NOT_SUPPORTED;
}


// implement application specific parameter-mechanism in these functions:
STDMETHODIMP RawComponent::get_ComponentParameter(BSTR name, VARIANT *pVal) {
	return S_OK;
}

STDMETHODIMP RawComponent::put_ComponentParameter(BSTR name, VARIANT newVal) {
	return S_OK;
}


#ifdef GME_ADDON

// these two functions are the main 
STDMETHODIMP RawComponent::GlobalEvent(globalevent_enum event) { 
	if(event == GLOBALEVENT_UNDO) {
		AfxMessageBox("UNDO!!");
	}
	return S_OK; 
}

STDMETHODIMP RawComponent::ObjectEvent(IMgaObject * obj, unsigned long eventmask, VARIANT v) {
	if(eventmask & OBJEVENT_CREATED) {
		CComBSTR objID;
		COMTHROW(obj->get_ID(&objID));
		AfxMessageBox( "Object created! ObjID: " + CString(objID)); 
	}		
	return S_OK;
}

#endif
