///////////////////////////////////////////////////////////////////////////
// RawComponent.cpp, the main RAW COM component implementation file
// This is the file (along with its header RawComponent.h)
// that the component implementor is expected to modify in the first place
//
///////////////////////////////////////////////////////////////////////////
#include "StdAfx.h"

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

#ifdef _USE_DOM
	#include "UdmDOM.h"
#endif

#include "UdmGme.h"
#include "UdmStatic.h"
#include "UdmUtil.h"

#include "UdmApp.h"
#include <time.h>

__declspec(noreturn) void ThrowComError(HRESULT hr, LPOLESTR err);

// Global config object
_config config;

//ATL::CComPtr<IGMEOLEApp>GMEConsole::Console::gmeoleapp = NULL;

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
	return E_MGA_NOT_SUPPORTED;
#endif
}

#ifdef _DYNAMIC_META
			void dummy(void) {; } // Dummy function for UDM meta initialization
#endif

STDMETHODIMP RawComponent::InvokeEx(IMgaProject *project,  IMgaFCO *currentobj,
							IMgaFCOs *selectedobjs,  long param)
{
	if (param == GME_SILENT_MODE) {
		interactive = false;
	}
	return Main(project, currentobj, true);
}

STDMETHODIMP RawComponent::ExportDesertXML(IMgaProject *project, IMgaFCO *currentObj, BSTR exportPath)
{
	CUdmApp app;

	CComPtr<IMgaProject>ccpProject(project);
	long prefmask;
	ccpProject->get_Preferences(&prefmask);
	// KMS magic number MGAPREF_NO_NESTED_TX
#define MGAPREF_NO_NESTED_TX 0x80
	prefmask |= MGAPREF_NO_NESTED_TX;
	ccpProject->put_Preferences(prefmask);
	std::shared_ptr<CComPtr<IMgaProject>> mgaPrefRestore(std::addressof(ccpProject), [prefmask](ATL::CComPtr<IMgaProject>* p) { (*p)->put_Preferences(prefmask & (~MGAPREF_NO_NESTED_TX));  });
	long status = 0;
	ccpProject->get_ProjectStatus(&status);

	CComPtr<IMgaComponentEx> cyPhyAddOn;
	try
	{
		// Setting up the console
		GMEConsole::Console::setupConsole(ccpProject);
		if (clearConsole == true)
		{
			GMEConsole::Console::clear();
		}
		char currdate[128];
		char currtime[128];
		_strdate(currdate);
		_strtime(currtime);
		std::string cd(currdate);
		std::string ct(currtime);
		GMEConsole::Console::Info::writeLine("== " + cd + " " + ct + "> Call Cyphy DesignSpace Helper Tool: ==");

		// Finding CyPhyAddOn among the addons
		CComPtr<IMgaComponents> comps;
		COMTHROW(project->get_AddOnComponents(&comps));
		MGACOLL_ITERATE(IMgaComponent, comps)
		{
			CComBSTR name;
			COMTHROW(MGACOLL_ITER->get_ComponentName(&name));
			if (name == L"CyPhyAddOn")
			{
				cyPhyAddOn = CComQIPtr<IMgaComponentEx>(MGACOLL_ITER);
			}
		} MGACOLL_ITERATE_END;
		if (cyPhyAddOn)
			COMTHROW(cyPhyAddOn->put_ComponentParameter(_bstr_t(L"turnedon"), CComVariant(false)));


		//	  if(interactive)
		{

			CComBSTR projname;
			CComBSTR focusname = "<nothing>";
			CComPtr<IMgaTerritory> terr;
			COMTHROW(ccpProject->CreateTerritory(NULL, &terr));

			CComPtr<IMgaProject> mgaProject = project;
			CComBSTR    connString;
			COMTHROW(mgaProject->get_ProjectConnStr(&connString));

			CString mgaFilePath = connString;
			if (mgaFilePath.Find(L"MGA=", 0) == 0) {
				int e = mgaFilePath.ReverseFind('\\');
				//	if(e != -1)
				{
					app.mgaPath = mgaFilePath.Mid(4, mgaFilePath.GetLength());
				}
			}
			else {
				mgaFilePath = mgaFilePath.Right(mgaFilePath.GetLength() - wcslen(L"MGX=\""));
				mgaFilePath = mgaFilePath.Left(mgaFilePath.Find(L"\"", 0));
				app.mgaPath = mgaFilePath;
			}

			// Setting up Udm
			using namespace META_NAMESPACE;

			// Loading the project
			UdmGme::GmeDataNetwork dngBackend(META_NAMESPACE::diagram);

			try
			{
				CComPtr<IMgaTerritory> terr;
				if (!(status & 8))
					COMTHROW(ccpProject->BeginTransactionInNewTerr(TRANSACTION_NON_NESTED, &terr));
				// Opening backend
				dngBackend.OpenExisting(ccpProject, Udm::CHANGES_LOST_DEFAULT, true);

				CComPtr<IMgaFCO> ccpFocus(currentObj);
				Udm::Object currentObject;
				if (ccpFocus)
				{
					currentObject = dngBackend.Gme2Udm(ccpFocus);
				}

				// Calling the main entry point
				app.ExportDesertXml(&dngBackend, currentObject, std::wstring(exportPath));
				// Closing backend
				dngBackend.CloseWithUpdate();

				if (!(status & 8))
					COMTHROW(ccpProject->CommitTransaction());
				terr = 0;
			}
			catch (udm_exception &exc)
			{
#ifdef _META_ACCESS_MEMORY
				dnCacheBackend.CloseNoUpdate();
#endif
				dngBackend.CloseNoUpdate();
				if (!(status & 8))
					COMTHROW(ccpProject->AbortTransaction());
				throw;
			}
		}
	}
	catch (udm_exception &exc)
	{
		GMEConsole::Console::Error::writeLine(exc.what());
		GMEConsole::Console::freeConsole();
		if (cyPhyAddOn)
			cyPhyAddOn->put_ComponentParameter(_bstr_t(L"turnedon"), CComVariant(true));
		ThrowComError(E_FAIL, _bstr_t(exc.what()));
	}
	catch (_com_error&)
	{
		if (!(status & 8))
			ccpProject->AbortTransaction();
		GMEConsole::Console::freeConsole();
		if (cyPhyAddOn)
			cyPhyAddOn->put_ComponentParameter(_bstr_t(L"turnedon"), CComVariant(true));
		throw;
	}
	catch (exception& e)
	{
		if (!(status & 8))
			ccpProject->AbortTransaction();
		GMEConsole::Console::freeConsole();
		if (cyPhyAddOn)
			cyPhyAddOn->put_ComponentParameter(_bstr_t(L"turnedon"), CComVariant(true));
		ThrowComError(E_FAIL, _bstr_t(e.what()));
	}
	// TODO: catch HRESULT
	catch (...)
	{
		if (!(status & 8))
			ccpProject->AbortTransaction();
		GMEConsole::Console::freeConsole();
		if (cyPhyAddOn)
			cyPhyAddOn->put_ComponentParameter(_bstr_t(L"turnedon"), CComVariant(true));
		ThrowComError(E_FAIL, _bstr_t("An unexpected error has occured during the interpretation process."));
	}
	GMEConsole::Console::Info::writeLine("==== End ====");
	GMEConsole::Console::freeConsole();
	if (cyPhyAddOn)
		cyPhyAddOn->put_ComponentParameter(_bstr_t(L"turnedon"), CComVariant(true));
	return S_OK;
}

HRESULT RawComponent::ImportConfigsFromXML(IMgaProject *project, IMgaFCO *designContainer, BSTR desertXmlPath, BSTR desertBackXmlPath, BSTR *exportedConfigurationsName)
{
	CUdmApp app;

	CComPtr<IMgaProject>ccpProject(project);
	long prefmask;
	ccpProject->get_Preferences(&prefmask);
	// KMS magic number MGAPREF_NO_NESTED_TX
#define MGAPREF_NO_NESTED_TX 0x80
	prefmask |= MGAPREF_NO_NESTED_TX;
	ccpProject->put_Preferences(prefmask);
	std::shared_ptr<CComPtr<IMgaProject>> mgaPrefRestore(std::addressof(ccpProject), [prefmask](ATL::CComPtr<IMgaProject>* p) { (*p)->put_Preferences(prefmask & (~MGAPREF_NO_NESTED_TX));  });
	long status = 0;
	ccpProject->get_ProjectStatus(&status);

	CComPtr<IMgaComponentEx> cyPhyAddOn;
	try
	{
		// Setting up the console
		GMEConsole::Console::setupConsole(ccpProject);
		if (clearConsole == true)
		{
			GMEConsole::Console::clear();
		}
		char currdate[128];
		char currtime[128];
		_strdate(currdate);
		_strtime(currtime);
		std::string cd(currdate);
		std::string ct(currtime);
		GMEConsole::Console::Info::writeLine("== " + cd + " " + ct + "> Call Cyphy DesignSpace Helper Tool: ==");

		// Finding CyPhyAddOn among the addons
		CComPtr<IMgaComponents> comps;
		COMTHROW(project->get_AddOnComponents(&comps));
		MGACOLL_ITERATE(IMgaComponent, comps)
		{
			CComBSTR name;
			COMTHROW(MGACOLL_ITER->get_ComponentName(&name));
			if (name == L"CyPhyAddOn")
			{
				cyPhyAddOn = CComQIPtr<IMgaComponentEx>(MGACOLL_ITER);
			}
		} MGACOLL_ITERATE_END;
		if (cyPhyAddOn)
			COMTHROW(cyPhyAddOn->put_ComponentParameter(_bstr_t(L"turnedon"), CComVariant(false)));


		//	  if(interactive)
		{

			CComBSTR projname;
			CComBSTR focusname = "<nothing>";
			CComPtr<IMgaTerritory> terr;
			COMTHROW(ccpProject->CreateTerritory(NULL, &terr));

			CComPtr<IMgaProject> mgaProject = project;
			CComBSTR    connString;
			COMTHROW(mgaProject->get_ProjectConnStr(&connString));

			CString mgaFilePath = connString;
			if (mgaFilePath.Find(L"MGA=", 0) == 0) {
				int e = mgaFilePath.ReverseFind('\\');
				//	if(e != -1)
				{
					app.mgaPath = mgaFilePath.Mid(4, mgaFilePath.GetLength());
				}
			}
			else {
				mgaFilePath = mgaFilePath.Right(mgaFilePath.GetLength() - wcslen(L"MGX=\""));
				mgaFilePath = mgaFilePath.Left(mgaFilePath.Find(L"\"", 0));
				app.mgaPath = mgaFilePath;
			}

			// Setting up Udm
			using namespace META_NAMESPACE;

			// Loading the project
			UdmGme::GmeDataNetwork dngBackend(META_NAMESPACE::diagram);

			try
			{
				CComPtr<IMgaTerritory> terr;
				if (!(status & 8))
					COMTHROW(ccpProject->BeginTransactionInNewTerr(TRANSACTION_NON_NESTED, &terr));
				// Opening backend
				dngBackend.OpenExisting(ccpProject, Udm::CHANGES_LOST_DEFAULT, true);

				CComPtr<IMgaFCO> ccpFocus(designContainer);
				Udm::Object currentObject;
				if (ccpFocus)
				{
					currentObject = dngBackend.Gme2Udm(ccpFocus);
				}

				std::wstring exportedConfigurationsNameStlString;

				// Calling the main entry point
				app.ImportConfigsFromXML(&dngBackend, currentObject, std::wstring(desertXmlPath), std::wstring(desertBackXmlPath), exportedConfigurationsNameStlString);

				BSTR configsName = CComBSTR(exportedConfigurationsNameStlString.c_str()).Detach();

				*exportedConfigurationsName = configsName;

				// Closing backend
				dngBackend.CloseWithUpdate();

				if (!(status & 8))
					COMTHROW(ccpProject->CommitTransaction());
				terr = 0;
			}
			catch (udm_exception &exc)
			{
#ifdef _META_ACCESS_MEMORY
				dnCacheBackend.CloseNoUpdate();
#endif
				dngBackend.CloseNoUpdate();
				if (!(status & 8))
					COMTHROW(ccpProject->AbortTransaction());
				throw;
			}
		}
	}
	catch (udm_exception &exc)
	{
		GMEConsole::Console::Error::writeLine(exc.what());
		GMEConsole::Console::freeConsole();
		if (cyPhyAddOn)
			cyPhyAddOn->put_ComponentParameter(_bstr_t(L"turnedon"), CComVariant(true));
		ThrowComError(E_FAIL, _bstr_t(exc.what()));
	}
	catch (_com_error&)
	{
		if (!(status & 8))
			ccpProject->AbortTransaction();
		GMEConsole::Console::freeConsole();
		if (cyPhyAddOn)
			cyPhyAddOn->put_ComponentParameter(_bstr_t(L"turnedon"), CComVariant(true));
		throw;
	}
	catch (exception& e)
	{
		if (!(status & 8))
			ccpProject->AbortTransaction();
		GMEConsole::Console::freeConsole();
		if (cyPhyAddOn)
			cyPhyAddOn->put_ComponentParameter(_bstr_t(L"turnedon"), CComVariant(true));
		ThrowComError(E_FAIL, _bstr_t(e.what()));
	}
	// TODO: catch HRESULT
	catch (...)
	{
		if (!(status & 8))
			ccpProject->AbortTransaction();
		GMEConsole::Console::freeConsole();
		if (cyPhyAddOn)
			cyPhyAddOn->put_ComponentParameter(_bstr_t(L"turnedon"), CComVariant(true));
		ThrowComError(E_FAIL, _bstr_t("An unexpected error has occured during the interpretation process."));
	}
	GMEConsole::Console::Info::writeLine("==== End ====");
	GMEConsole::Console::freeConsole();
	if (cyPhyAddOn)
		cyPhyAddOn->put_ComponentParameter(_bstr_t(L"turnedon"), CComVariant(true));
	return S_OK;
}

HRESULT RawComponent::Main(IMgaProject *project,  IMgaFCO *currentobj, bool applyConstraintsInNoninteractive)
{
	CUdmApp app;

	CComPtr<IMgaProject>ccpProject(project);
	long prefmask;
	ccpProject->get_Preferences(&prefmask);
	// KMS magic number MGAPREF_NO_NESTED_TX
#define MGAPREF_NO_NESTED_TX 0x80
	prefmask |= MGAPREF_NO_NESTED_TX; 
	ccpProject->put_Preferences(prefmask);
	std::shared_ptr<CComPtr<IMgaProject>> mgaPrefRestore(std::addressof(ccpProject), [prefmask](ATL::CComPtr<IMgaProject>* p){ (*p)->put_Preferences(prefmask & (~MGAPREF_NO_NESTED_TX));  } );
	long status = 0;
	ccpProject->get_ProjectStatus(&status);

	CComPtr<IMgaComponentEx> cyPhyAddOn;
	try
	{
		// Setting up the console
		  GMEConsole::Console::setupConsole(ccpProject);
		  if (clearConsole == true)
		  {
			  GMEConsole::Console::clear();
		  }
		  char currdate[128];
		  char currtime[128];
		  _strdate( currdate);
		  _strtime( currtime );
		  std::string cd(currdate);
		  std::string ct(currtime);
		  GMEConsole::Console::Info::writeLine("== "+cd+" "+ct+"> Call Cyphy DesignSpace Helper Tool: ==");

		// Finding CyPhyAddOn among the addons
		CComPtr<IMgaComponents> comps;
		COMTHROW(project->get_AddOnComponents(&comps));
		MGACOLL_ITERATE(IMgaComponent, comps) 
		{
			CComBSTR name;
			COMTHROW(MGACOLL_ITER->get_ComponentName(&name));
			if (name == L"CyPhyAddOn") 
			{
				cyPhyAddOn = CComQIPtr<IMgaComponentEx>(MGACOLL_ITER); 
			}
		} MGACOLL_ITERATE_END;
		if (cyPhyAddOn)
			COMTHROW(cyPhyAddOn->put_ComponentParameter(_bstr_t(L"turnedon"), CComVariant(false)));


	//	  if(interactive)
	  {

		CComBSTR projname;
		CComBSTR focusname = "<nothing>";
		CComPtr<IMgaTerritory> terr;
		COMTHROW(ccpProject->CreateTerritory(NULL, &terr));

		CComPtr<IMgaProject> mgaProject = project;
		CComBSTR    connString;
		COMTHROW(mgaProject->get_ProjectConnStr(&connString));

		CString mgaFilePath = connString;
		if (mgaFilePath.Find(L"MGA=", 0) == 0) {
			int e = mgaFilePath.ReverseFind('\\');
			//	if(e != -1)
			{
				app.mgaPath = mgaFilePath.Mid(4, mgaFilePath.GetLength());
			}
		}
		else {
			mgaFilePath = mgaFilePath.Right(mgaFilePath.GetLength() - wcslen(L"MGX=\""));
			mgaFilePath = mgaFilePath.Left(mgaFilePath.Find(L"\"", 0));
			app.mgaPath = mgaFilePath;
		}

		// Setting up Udm
		using namespace META_NAMESPACE;

		// Loading the project
		UdmGme::GmeDataNetwork dngBackend(META_NAMESPACE::diagram);

		try
		{
			CComPtr<IMgaTerritory> terr;
			if (!(status & 8))
				COMTHROW(ccpProject->BeginTransactionInNewTerr(TRANSACTION_NON_NESTED, &terr));
			// Opening backend
			dngBackend.OpenExisting(ccpProject, Udm::CHANGES_LOST_DEFAULT, true);

			CComPtr<IMgaFCO> ccpFocus(currentobj);
			Udm::Object currentObject;
			if(ccpFocus)
			{
				currentObject=dngBackend.Gme2Udm(ccpFocus);
			}

			// Calling the main entry point
			app.UdmMain(&dngBackend,currentObject, interactive ? 0 : 128, applyConstraintsInNoninteractive);
			// Closing backend
			dngBackend.CloseWithUpdate();
			
			if (!(status & 8))
				COMTHROW(ccpProject->CommitTransaction());
			terr = 0;
			if (GMEConsole::Console::gmeoleapp)
			{
				CComPtr<IGMEOLEIt> model;
				if (SUCCEEDED(GMEConsole::Console::gmeoleapp->get_OleIt(&model)))
				{
					model->SetCurrentAspect(_bstr_t(L"DesignSpaceAspect"));
				}
			}
		}
		catch(udm_exception &exc)
		{
#ifdef _META_ACCESS_MEMORY
			dnCacheBackend.CloseNoUpdate();
#endif
			dngBackend.CloseNoUpdate();
			if (!(status & 8))
				COMTHROW(ccpProject->AbortTransaction());
			throw;
		}
	  }
	}
	catch(udm_exception &exc)
	{
		GMEConsole::Console::Error::writeLine(exc.what());
		GMEConsole::Console::freeConsole();
		if (cyPhyAddOn)
			cyPhyAddOn->put_ComponentParameter(_bstr_t(L"turnedon"), CComVariant(true));
		ThrowComError(E_FAIL, _bstr_t(exc.what()));
	}
	catch (_com_error&)
	{
		if (!(status & 8))
			ccpProject->AbortTransaction();
		GMEConsole::Console::freeConsole();
		if (cyPhyAddOn)
			cyPhyAddOn->put_ComponentParameter(_bstr_t(L"turnedon"), CComVariant(true));
		throw;
	}
	catch(exception& e)
	{
		if (!(status & 8))
			ccpProject->AbortTransaction();
		GMEConsole::Console::freeConsole();
		if (cyPhyAddOn)
			cyPhyAddOn->put_ComponentParameter(_bstr_t(L"turnedon"), CComVariant(true));
		ThrowComError(E_FAIL, _bstr_t(e.what()));
	}
	// TODO: catch HRESULT
	catch(...)
	{
		if (!(status & 8))
			ccpProject->AbortTransaction();
		GMEConsole::Console::freeConsole();
		if (cyPhyAddOn)
			cyPhyAddOn->put_ComponentParameter(_bstr_t(L"turnedon"), CComVariant(true));
		ThrowComError(E_FAIL, _bstr_t("An unexpected error has occured during the interpretation process."));
	}
	GMEConsole::Console::Info::writeLine("==== End ====");
	GMEConsole::Console::freeConsole();
	if (cyPhyAddOn)
		cyPhyAddOn->put_ComponentParameter(_bstr_t(L"turnedon"), CComVariant(true));
	return S_OK;
}

// GME currently does not use this function
// you only need to implement it if other invokation mechanisms are used
STDMETHODIMP RawComponent::ObjectsInvokeEx( IMgaProject *project,  IMgaObject *currentobj,  IMgaObjects *selectedobjs,  long param) {
	return E_MGA_NOT_SUPPORTED;
}


// implement application specific parameter-mechanism in these functions:
STDMETHODIMP RawComponent::get_ComponentParameter(BSTR name, VARIANT *pVal) {
	return S_OK;
}

STDMETHODIMP RawComponent::put_ComponentParameter(BSTR name, VARIANT newVal) {
	if (wcscmp(name, L"clearConsole") == 0)
	{
		this->clearConsole = newVal.boolVal != VARIANT_FALSE;
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
