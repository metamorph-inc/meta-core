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
#include "UdmDom.h"

using namespace std;

#ifdef _USE_DOM
	#include "UdmDOM.h"
#endif

#include "UdmGme.h"
#include "UdmStatic.h"
#include "UdmUtil.h"

#include "UdmApp.h"
#include "UdmConfig.h"

#include <boost/filesystem.hpp>

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


struct CyPhyXSDSaver
{
	std::string xsd_data;
	CyPhyXSDSaver()
	{
		auto entry = UdmDom::str_xsd_storage::static_xsd_container.find("CyPhyML.xsd");
		if (entry != UdmDom::str_xsd_storage::static_xsd_container.end())
		{
			xsd_data = entry->second;
			UdmDom::str_xsd_storage::static_xsd_container.erase(entry);
		}
	}
	~CyPhyXSDSaver()
	{
		if (xsd_data != "")
		{
			auto entry = UdmDom::str_xsd_storage::static_xsd_container.find("CyPhyML.xsd");
			if (entry != UdmDom::str_xsd_storage::static_xsd_container.end())
			{
				UdmDom::str_xsd_storage::static_xsd_container.erase(entry);
			}
			UdmDom::str_xsd_storage::static_xsd_container.emplace(make_pair(std::string("CyPhyML.xsd"), std::move(xsd_data)));
		}
		else
		{
			UdmDom::str_xsd_storage::static_xsd_container.erase(std::string("CyPhyML.xsd"));
		}
	}
};

// This is the main component method for interpereters and plugins. 
// May als be used in case of invokeable addons
STDMETHODIMP RawComponent::InvokeEx( IMgaProject *project,  IMgaFCO *currentobj,  
									IMgaFCOs *selectedobjs,  long param) 
{
	CUdmApp app;
	// Calling the user's initialization function
	if(app.Initialize())
	{
		return S_FALSE;
	}

	// save original working directory
	auto original_wroking_dir = boost::filesystem::current_path();

	CyPhyXSDSaver saver;
	CComPtr<IMgaProject>ccpProject(project);
	long status = 0;
	
	try
	{
		// Setting up the console
		GMEConsole::Console::SetupConsole(ccpProject);

	  if(interactive)
	  {
		CComBSTR projname;
		CComBSTR focusname = "<nothing>";
		CComPtr<IMgaTerritory> terr;

		CComPtr<IMgaProject> mgaProject = project;
		CComBSTR    connString;
		COMTHROW(mgaProject->get_ProjectConnStr(&connString));

		app.mgaPath = connString;
		app.mgaPath.TrimLeft("MGA=");
		// Setting up Udm
		using namespace META_NAMESPACE;

		// Loading the project
		UdmGme::GmeDataNetwork dngBackend(META_NAMESPACE::diagram);

		try
		{
			COMTHROW(ccpProject->get_ProjectStatus(&status));
			if (!(status & 8))
				COMTHROW(ccpProject->BeginTransactionInNewTerr(TRANSACTION_GENERAL, &terr));
			else
				app.master = true;
			// Opening backend
			dngBackend.OpenExisting(ccpProject, Udm::CHANGES_LOST_DEFAULT, true);


			CComPtr<IMgaFCO> ccpFocus(currentobj);
			Udm::Object currentObject;
			if(ccpFocus)
			{
				currentObject=dngBackend.Gme2Udm(ccpFocus);
			}

			std::set<Udm::Object> selectedObjects;

			if(selectedobjs!=NULL)
			{
				CComPtr<IMgaFCOs> ccpSelObject(selectedobjs);

				MGACOLL_ITERATE(IMgaFCO,ccpSelObject){
					Udm::Object currObj;
					if(MGACOLL_ITER)
					{
						currObj=dngBackend.Gme2Udm(MGACOLL_ITER);
					}
				 selectedObjects.insert(currObj);
				}MGACOLL_ITERATE_END;
			}
			// Calling the main entry point
			_variant_t outdir;
			get_ComponentParameter(_bstr_t(L"output_dir"), &outdir);
			app.outputDir = std::string(_bstr_t(outdir));

			_variant_t automation;
			get_ComponentParameter(_bstr_t(L"automation"), &automation);	
			std::string value = std::string(_bstr_t(automation));
			if (value == "true")
				app.master = true;
			else
				app.master = false;

			app.UdmMain(&dngBackend,currentObject,selectedObjects,param);
			// Closing backend
			dngBackend.CloseWithUpdate();
			app.outputDir.clear();
			try {
			 if (!(status & 8))
                   ccpProject->CommitTransaction();
              } catch (...) {
                   if (!(status & 8))
                        ccpProject->AbortTransaction();
                   throw;
              }
		}
		catch(udm_exception &exc)
		{
#ifdef _META_ACCESS_MEMORY
			dnCacheBackend.CloseNoUpdate();
#endif
			// Close GME Backend (we may close it twice, but GmeDataNetwork handles it)
			dngBackend.CloseNoUpdate();
			if (!(status & 8))
				ccpProject->AbortTransaction();
			GMEConsole::Console::Error::writeLine(exc.what());

			if(app.master)
				throw exc;

			return S_FALSE;
		}
	  }
	}
	catch (udm_exception& e)
	{
		if (!(status & 8))
			ccpProject->AbortTransaction();
		GMEConsole::Console::gmeoleapp = 0;
		std::string msg = "Udm exception: ";
		msg += e.what();
		AfxMessageBox(msg.c_str());
		
		if(app.master)
			throw e;

		return E_FAIL;
	}
	catch(exception &e)
	{
		if(app.master)
			throw e;

		return E_FAIL;
	}
	catch (_com_error &)
	{
		if (!(status & 8))
			ccpProject->AbortTransaction();
		throw;
	}
	catch(...)
	{
		if (!(status & 8))
			ccpProject->AbortTransaction();
		GMEConsole::Console::gmeoleapp = 0;
		// This can be a problem with the GME Console, so we display it in a message box
		AfxMessageBox("An unexpected error has occurred during the interpretation process.");

		if(app.master)
			throw;

		return E_FAIL;
	}

	// retrieve original working directory
	boost::filesystem::current_path(original_wroking_dir);

	GMEConsole::Console::gmeoleapp = 0;
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
	std::string _name = _bstr_t(name);
	string Message = "";
	bool found = false;

	map<string, _variant_t>::const_iterator it;
	it = componentParameters.find(_name);
	if (it != componentParameters.end())
	{
		found = true;
		CComVariant(it->second).Detach(pVal);
		return S_OK;
	}
	else
	{
		// default message
		//Message += "ERROR: Paramter not found. Use \"output_dir\".\n";
		//CComVariant(Message.c_str()).Detach(pVal);
		return S_FALSE;
	}
}

STDMETHODIMP RawComponent::put_ComponentParameter(BSTR name, VARIANT newVal) {
	std::string _name = _bstr_t(name);
	
	componentParameters[_name] = newVal;
	
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
