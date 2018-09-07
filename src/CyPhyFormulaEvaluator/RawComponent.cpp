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
#include "Mga.h"
#include "RawComponent.h"
#include <iostream>
#include <fstream>

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
#include <boost/filesystem.hpp>
#include "UnitUtil.h"

void RunCyPhyElaborate_ComputePropertyValues(IMgaFCO *currentobj);

__declspec(noreturn) void ThrowComError(HRESULT hr, LPOLESTR err);

// this method is called after all the generic initialization is done
// this should be empty, unless application-specific initialization is needed
STDMETHODIMP RawComponent::Initialize(struct IMgaProject *) {
#ifdef CONSOLE_ON
	ConsoleMessagesOn = true;
#else
	ConsoleMessagesOn = false;
#endif
	Automation = false;
	DoNotGeneratePostProcessing = false;
	OutputDir = "";
	ExceptionMessage = "";
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
	if(app.Initialize())
	{
		return S_FALSE;
	}

	CComPtr<IMgaProject>ccpProject(project);
	
	try
	{
		if (Automation == false) {
			RunCyPhyElaborate_ComputePropertyValues(currentobj);
			return S_OK;
		}

		// Setting up the console
		if (ConsoleMessagesOn) {
			GMEConsole::Console::setupConsole(ccpProject);
			// GMEConsole::Console::clear();
		}
		else {
			GMEConsole::Console::freeConsole();
		}

		char tmpbuf[128];
		_strdate_s(tmpbuf,128);
		std::string date(tmpbuf);
		_strtime_s(tmpbuf,128);
		std::string time(tmpbuf);

		// GMEConsole::Console::writeLine(date + " " + time + " Starting CyPhyFormulaEvaluator Interpreter", MSG_INFO);


	  if(interactive)
	  {
		CComBSTR projname;
		CComBSTR focusname = "<nothing>";
		CComPtr<IMgaTerritory> terr;
		//COMTHROW(ccpProject->CreateTerritory(NULL, &terr));

		// Setting up Udm
		using namespace META_NAMESPACE;

		// Loading the project
		UdmGme::GmeDataNetwork dngBackend(META_NAMESPACE::diagram);

		try
		{
			long status;
			COMTHROW(ccpProject->get_ProjectStatus(&status));
			bool startedTx = false;
			if (!(status & 8))
			{
				COMTHROW(ccpProject->BeginTransactionInNewTerr(TRANSACTION_NON_NESTED, &terr));
				startedTx = true;
			}
			// Opening backend
			try {
				dngBackend.OpenExisting(ccpProject, Udm::CHANGES_LOST_DEFAULT, true);

				CComBSTR bstrPath;
				COMTHROW(ccpProject->get_ProjectConnStr(&bstrPath));
				app.projectDir = CStringA(bstrPath);

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

				// Calling the main entry point
				app.UdmMain(&dngBackend, currentObject, selectedObjects, param);
				// Closing backend
				dngBackend.CloseWithUpdate();

				if (startedTx) {
					ccpProject->CommitTransaction();
				}
			} catch (...) {
				if (startedTx) {
					ccpProject->AbortTransaction();
				}
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

			GMEConsole::Console::Error::writeLine(exc.what());
			GMEConsole::Console::gmeoleapp = 0;
			ThrowComError(E_FAIL, _bstr_t(exc.what()));
		}
	  }
	}
	catch (udm_exception &e) {
		GMEConsole::Console::gmeoleapp = 0;
		std::string msg = "Udm error: ";
		msg += e.what();
		ThrowComError(E_FAIL, _bstr_t(msg.c_str()));
	}
	catch (const _com_error&)
	{
		GMEConsole::Console::gmeoleapp = 0;
		throw;
	}
	catch(...)
	{
		GMEConsole::Console::gmeoleapp = 0;
		ccpProject->AbortTransaction();
		// This can be a problem with the GME Console, so we display it in a message box
		AfxMessageBox(L"An unexpected error has occurred during the interpretation process.");
		return E_FAIL;
	}


	//GMEConsole::Console::writeLine("Output files are <a href=\"file:///" + OutputDir + "\" target=\"_blank\">" + OutputDir + "</a>.", MSG_INFO);

	// GMEConsole::Console::writeLine("End of CyPhyFormulaEvaluator Interpreter", MSG_INFO);
	GMEConsole::Console::freeConsole();


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
	else if (_name == "numericLeafNodes")
	{
		VariantInit(pVal);

		SAFEARRAYBOUND aDim[1];
		aDim[0].lLbound = 0;
		aDim[0].cElements = app.numericLeafNodes.size();

		SAFEARRAY* sa = SafeArrayCreate(VT_BSTR, 1, aDim);
		if (sa)
		{
			for (LONG i = 0; i < app.numericLeafNodes.size(); i++)
			{
				// note: makes a copy, DOES NOT pass ownership
				SafeArrayPutElement(sa, &i, CComBSTR(app.numericLeafNodes[i].c_str()));
			}

			pVal->vt = VT_ARRAY | VT_BSTR;
			pVal->parray = sa;
		}
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

		if (_name == "output_dir")
		{
			std::string value = _bstr_t(_newVal.bstrVal);
			if (value.length() > 0)
			{
				if (value.substr(value.length()-1) != "\\")
				{
					value.append("\\");
				}
			}
			OutputDir = value;
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
		else if (_name == "do_not_generate_post_processing")
		{
			std::string value = _bstr_t(_newVal.bstrVal);
			if (value == "true")
			{
				DoNotGeneratePostProcessing = true;
			}
			else
			{
				DoNotGeneratePostProcessing = false;
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
		else if (_name == "original_project_file")
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
			app.projectDir_SOT = value;
		}
		else if (_name == "traceability") {
			IUnknownPtr traceability;
			if (_newVal.vt == VT_DISPATCH) {
				traceability = _newVal.pdispVal;
			}
			else if (_newVal.vt == VT_UNKNOWN) {
				traceability = _newVal.punkVal;
			}
			else {
				ThrowComError(E_INVALIDARG, L"Invalid traceability");
			}
			app.traceability = traceability;
		}
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
#if 0
	if(eventmask & OBJEVENT_CREATED) {
		CComBSTR objID;
		COMTHROW(obj->get_ID(&objID));
		AfxMessageBox( "Object created! ObjID: " + CString(objID)); 
	}	
#endif

	return S_OK;
}

#endif


extern "C" __declspec(dllexport) int AreUnitsEqual(IMgaFCO* fco1, IMgaFCO* fco2)
{
	UnitUtil u;
	UdmGme::GmeDataNetwork dn(CyPhyML::diagram);
	CComPtr<IMgaProject> project;
	fco1->get_Project(&project);
	dn.OpenExisting(project, Udm::CHANGES_LOST_DEFAULT, true);
	auto unit1 = CyPhyML::unit::Cast(dn.Gme2Udm(fco1));
	auto unit2 = CyPhyML::unit::Cast(dn.Gme2Udm(fco2));

	UnitUtil::DimensionRep unit1rep;
	u.ConvertToSIEquivalent(unit1, 1, unit1rep);
	UnitUtil::DimensionRep unit2rep;
	u.ConvertToSIEquivalent(unit2, 1, unit2rep);

	return unit1rep == unit2rep;
}
