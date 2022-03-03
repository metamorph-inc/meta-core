///////////////////////////////////////////////////////////////////////////
// RawComponent.cpp, the main RAW COM component implementation file
// This is the file (along with its header RawComponent.h)
// that the component implementor is expected to modify in the first place
//
///////////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#include "ComHelp.h"
#include "GMECOM.h"
#include "ComponentConfig.h"
#include "UdmConfig.h"
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
#include <time.h>
#include "UdmConfig.h"
// #include <boost/filesystem.hpp>

extern const wchar_t* g_COCLASS_PROGIDW;

void ThrowComError(HRESULT hr, LPOLESTR err);

bool ConsoleMessagesOn;
bool Automation;
string RefactoredTypeOrAction = "";
wstring ExceptionMessage = L"";

// Global config object
_config config;

//ATL::CComPtr<IGMEOLEApp>GMEConsole::Console::gmeoleapp = NULL;

// this method is called after all the generic initialization is done
// this should be empty, unless application-specific initialization is needed
STDMETHODIMP RawComponent::Initialize(struct IMgaProject *) {
#ifdef CONSOLE_ON
	ConsoleMessagesOn = true;
#else
	ConsoleMessagesOn = false;
#endif
	Automation = false;
	RefactoredTypeOrAction = "";
	ExceptionMessage = L"";
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
		AfxMessageBox(L"This component does not support the obsolete invoke mechanism");
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
	if(CUdmApp::Initialize())
	{
		return S_FALSE;
	}

	CComPtr<IMgaProject>ccpProject(project);

		long prefmask;

	ccpProject->get_Preferences(&prefmask);
	// KMS magic number MGAPREF_NO_NESTED_TX
#define MGAPREF_NO_NESTED_TX 0x80
	prefmask |= MGAPREF_NO_NESTED_TX; 
	ccpProject->put_Preferences(prefmask);
	std::shared_ptr<CComPtr<IMgaProject>> mgaPrefRestore(std::addressof(ccpProject), [prefmask](ATL::CComPtr<IMgaProject>* p){ (*p)->put_Preferences(prefmask & (~MGAPREF_NO_NESTED_TX));  } );

	 CComPtr<IMgaComponentEx> constrMgr;
	  CComPtr<IMgaComponentEx> cyphyAddon;
      // Finding constraint manager among the addons
      CComPtr<IMgaComponents> comps;
      COMTHROW( ccpProject->get_AddOnComponents(&comps));
      MGACOLL_ITERATE(IMgaComponent, comps) 
      {
            CComBSTR name;
            COMTHROW(MGACOLL_ITER->get_ComponentName(&name));
            if(name == L"ConstraintManager") 
            {
				constrMgr = CComQIPtr<IMgaComponentEx>(MGACOLL_ITER); 
            }
      } MGACOLL_ITERATE_END;

	  if (constrMgr)
	      COMTHROW(constrMgr->Enable(false));

//	if ( (project != NULL))
//	{		
//		CComBSTR bstrName("GME.Application");
//		CComPtr<IMgaClient> pClient;
//		HRESULT hr = project->GetClientByName(bstrName, &pClient);
//		if (SUCCEEDED(hr) && pClient) 
//		{
//			CComPtr<IDispatch> pDispatch;
//			hr = pClient->get_OLEServer(&pDispatch);
//			if (SUCCEEDED(hr) && pDispatch) 
//			{
//				hr = pDispatch.QueryInterface(&CUdmApp::gme);
//				if (FAILED(hr)) 
//				{
//					CUdmApp::gme = NULL;
//				}
//			}
//		}
////		CUdmApp::gme->put_ConsoleContents(BSTR(""));
//	}

	try
	{
		// Setting up the console
		  if(!interactive)
			  param = 128;
		
		  GMEConsole::Console::setupConsole(ccpProject);
		  GMEConsole::Console::clear();
		  char currdate[128];
		  char currtime[128];
		  _strdate_s( currdate);
		  _strtime_s( currtime );
		  std::string cd(currdate);
		  std::string ct(currtime);
		  GMEConsole::Console::Info::writeLine("== "+cd+" "+ct+"> Call Cyphy DesignSpace Refactor Tool: ==");

	//	  if(interactive)
	  {

		CComBSTR projname;
		CComBSTR focusname = "<nothing>";

		CComPtr<IMgaProject> mgaProject = project;
		CComBSTR    connString;
		COMTHROW(mgaProject->get_ProjectConnStr(&connString));

		CString mgaFilePath = connString;
		int e = mgaFilePath.ReverseFind('\\');
		CUdmApp::mgaPath = mgaFilePath.Mid(4, mgaFilePath.GetLength());			

		// Setting up Udm
#ifdef _DYNAMIC_META
	#ifdef _DYNAMIC_META_DOM
			// Loading the meta for the project
			UdmDom::DomDataNetwork  ddnMeta(Uml::diagram);
			Uml::Diagram theUmlDiagram;

			// Opening the XML meta of the project
			ddnMeta.OpenExisting(config.metaPath,"uml.dtd", Udm::CHANGES_LOST_DEFAULT);

			// Casting the DataNetwork to diagram
			theUmlDiagram = Uml::Diagram::Cast(ddnMeta.GetRootObject());

			// Creating the UDM diagram
			Udm::UdmDiagram udmDataDiagram;
			udmDataDiagram.dgr = &theUmlDiagram;
			udmDataDiagram.init = dummy;

	#elif defined _DYNAMIC_META_STATIC
			// Loading the meta for the project
			UdmStatic::StaticDataNetwork  dnsMeta(Uml::diagram);
			Uml::Diagram theUmlDiagram;

			// Opening the static meta of the project
			dnsMeta.OpenExisting(config.metaPath, "", Udm::CHANGES_LOST_DEFAULT);

			// Casting the DataNetwork to diagram
			theUmlDiagram = Uml::Diagram::Cast(dnsMeta.GetRootObject());

			// Creating the UDM diagram
			Udm::UdmDiagram udmDataDiagram;
			udmDataDiagram.dgr = &theUmlDiagram;
			udmDataDiagram.init = dummy;

	#else
			ASSERT((0,"Nor _DYNAMIC_META_DOM either _DYNAMIC_META_STATIC defined for dynamic loading"));
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

			set<Udm::Object> selectedObjects;

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
			UdmStatic::StaticDataNetwork dnsCacheBackend(udmDataDiagram);
	#else
			UdmStatic::StaticDataNetwork dnsCacheBackend(META_NAMESPACE::diagram);
	#endif

			const Uml::Class & safeType = Uml::SafeTypeContainer::GetSafeType(dngBackend.GetRootObject().type());

			dnsCacheBackend.CreateNew("","",safeType, Udm::CHANGES_LOST_DEFAULT);

			Udm::Object nullObject(&Udm::__null);
			UdmUtil::copy_assoc_map copyAssocMap;
			copyAssocMap[currentObject]=nullObject; // currentObject may be null object

			for(set<Udm::Object>::iterator p_CurrSelObject=selectedObjects.begin();
				p_CurrSelObject!=selectedObjects.end();p_CurrSelObject++)
			{
					pair<Udm::Object const, Udm::Object> item(*p_CurrSelObject, nullObject);

					pair<UdmUtil::copy_assoc_map::iterator, bool> insRes = copyAssocMap.insert(item);

					if (!insRes.second)
					{
						ASSERT(NULL);
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
			set<Udm::Object> selectedObjectsCache;

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
			//if(!CUdmApp::isvalid)
			//{
			//	if(MessageBox(0,"The SCAMLA model is not validated. Please see details in the console window below. \n\n Do you want to run constraint manager to check other constraints?\n","SCAMLA Model Error",MB_OKCANCEL)==IDOK)
			//		CUdmApp::gme->CheckAllConstraints();
			//}
			//else
			//	CUdmApp::gme->CheckAllConstraints();
#endif

	//	}

		//try 
		//{				
		//	CComPtr<IMgaProject> spProject = project;
		//	//CComPtr<IMgaFCO> spContextFCO = currentobj;
		//	//CComPtr<IMgaFCOs> spSelectedFCOs = selectedobjs;
		//	//long lParam = param;

		//
		//	CComBSTR bstrCompName( L"MGA.AddOn.ConstraintManager" );

		//	CComPtr<IMgaComponentEx> grComponent;
		//	COMTHROW( grComponent.CoCreateInstance( bstrCompName ) ) ;
		//	COMTHROW( grComponent->put_InteractiveMode( VARIANT_TRUE ) );
		//	COMTHROW( grComponent->Enable( VARIANT_TRUE ) );
		//	COMTHROW( grComponent->Initialize( spProject ) );
		//	STDMETHODIMP ret = grComponent->ObjectsInvokeEx( spProject,  NULL, NULL, NULL);

		//	if(ret != S_OK)
		//	{				
		//		return ret;
		//	}				
		//}
		//catch(...) 
		//{ 
		//	
		//		AfxMessageBox("Exception");
		//	
		//}
			//CUdmApp::gme = NULL;

			//if(CUdmApp::gme!=0)
			//{
			//	CUdmApp::gme.Release();
			//	CUdmApp::gme=0;
			//}
		}
		catch(udm_exception &exc)
		{
#ifdef _META_ACCESS_MEMORY
			dnCacheBackend.CloseNoUpdate();
#endif
			// Close GME Backend (we may close it twice, but GmeDataNetwork handles it)
			GMEConsole::Console::Error::writeLine(exc.what());
			GMEConsole::Console::freeConsole();
			dngBackend.CloseNoUpdate();
			if (constrMgr)
		      COMTHROW(constrMgr->Enable(true));

			ThrowComError(E_FAIL, _bstr_t(exc.what()));
		}
	  }
	}
	catch(udm_exception &exc)
	{
		GMEConsole::Console::Error::writeLine(exc.what());
		GMEConsole::Console::freeConsole();
		if (constrMgr)
	      COMTHROW(constrMgr->Enable(true));
		ThrowComError(S_FALSE, _bstr_t(exc.what()));
	}
	catch(...)
	{
		ccpProject->AbortTransaction();

		std::string errMsg = "An unexpected error has occured during the interpretation process.";
		GMEConsole::Console::Out::writeLine(errMsg);
		GMEConsole::Console::freeConsole();
		if (constrMgr)
	      COMTHROW(constrMgr->Enable(true));
		ThrowComError(E_UNEXPECTED, _bstr_t("Unexpected error"));
	}
	GMEConsole::Console::Info::writeLine("==== End ====");
	GMEConsole::Console::freeConsole();
	if (constrMgr)
	      COMTHROW(constrMgr->Enable(true));
	return S_OK;
}

// GME currently does not use this function
// you only need to implement it if other invokation mechanisms are used
STDMETHODIMP RawComponent::ObjectsInvokeEx( IMgaProject *project,  IMgaObject *currentobj,  IMgaObjects *selectedobjs,  long param) {
	if(interactive) {
		AfxMessageBox(L"Tho ObjectsInvoke method is not implemented");
	}
	return E_MGA_NOT_SUPPORTED;
}


// implement application specific parameter-mechanism in these functions:
STDMETHODIMP RawComponent::get_ComponentParameter(BSTR name, VARIANT *pVal) {
	std::string _name = _bstr_t(name);
	string Message = "";
	bool found = false;

	map<string, _variant_t>::const_iterator it;
	it = componentParameters.find(_name);
	if (it != componentParameters.end())
	{
		found = true;
	}

	if (_name == "parameter_list")
	{
		CComSafeArray<BSTR> safe;
		for (map<string, _variant_t>::const_iterator it = componentParameters.begin();
			it != componentParameters.end();
			++it)
		{
			safe.Add(_bstr_t(it->first.c_str()));
		}
		CComVariant(safe).Detach(pVal);
		return S_OK;
	}
	else if (found)
	{
		CComVariant(it->second).Detach(pVal);
		return S_OK;
	}
	else if (_name == "exception")
	{
		CComVariant(ExceptionMessage.c_str()).Detach(pVal);
		return S_OK;
	}
	else
	{
		// default message
		string helpMessage = "";
		// Magic string ERROR:
		Message += "ERROR: Parameter not found. Use the parameter_list.\n";
		Message += "Please use the help or the parameter_list variables.\n ";
		CComVariant(Message.c_str()).Detach(pVal);
		return S_OK;
	}
	return S_OK;
}

void RawComponent::UpdateParameters()
{
	for (map<string, _variant_t>::const_iterator it = componentParameters.begin();
		it != componentParameters.end();
		++it)
	{
		std::string _name = it->first;
		_variant_t _newVal = it->second;

		if (_name == "refactored_type_or_action")
		{
			std::string value = _bstr_t(_newVal.bstrVal);
			if (value.length() > 0)
			{
				RefactoredTypeOrAction = value;
			}
		}
		else if (_name == "automation")
		{
			std::string value = _bstr_t(_newVal.bstrVal);
			if (value == "true")
			{
				Automation = true;
			}
			else
			{
				Automation = false;
			}
		}
		else if (_name == "console_messages")
		{
			std::string value = _bstr_t(_newVal.bstrVal);
			if (value == "on")
			{
				ConsoleMessagesOn = true;
			}
			else
			{
				ConsoleMessagesOn = false;
			}
		}
/*		else if (_name == "original_project_file")
		{
			std::string value = _bstr_t(_newVal.bstrVal);
			if (value.length() > 0)
			{
				if (value.substr(value.length()-1) == "\\")
				{
					value = value.substr(0, value.length()-1);
				}

				boost::filesystem::path p(value);
				value = p.make_preferred().parent_path().string();
			}
			CUdmApp::projectDir_SOT = value;
		}
*/
	}
}

STDMETHODIMP RawComponent::put_ComponentParameter(BSTR name, VARIANT newVal) {
	std::string _name = _bstr_t(name);

	bool found = false;

	map<string, _variant_t>::const_iterator it;
	it = componentParameters.find(_name);
	if (it != componentParameters.end())
	{
		found = true;
	}

	if (found)
	{
		componentParameters[_name] = newVal;
		// updates all parameters later we can pass the _name string
		// in order to update only the parameter that we received
		UpdateParameters();
		return S_OK;
	}

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
