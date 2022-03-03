///////////////////////////////////////////////////////////////////////////
// RawComponent.cpp, the main RAW COM component implementation file
// This is the file (along with its header RawComponent.h)
// that the component implementor is expected to modify in the first place
//
///////////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#include <atlsafe.h>
#include <fstream>

#include <ComHelp.h>
#include <GMECOM.h>
#include "ComponentConfig.h"
#include "RawComponent.h"
#include "UdmConsole.h"
#include "UdmApp.h"

__declspec(noreturn) void ThrowComError(HRESULT hr, LPOLESTR err);

using namespace std;

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
		AfxMessageBox(L"This component does not support the obsolete invoke mechanism");
	}
	return E_MGA_NOT_SUPPORTED;
#endif
}

#ifdef _DYNAMIC_META
			void dummy(void) {; } // Dummy function for UDM meta initialization
#endif

std::string GetMetaPath() {
	std::string metapath;
	HKEY software_meta;
	if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, "Software\\META", 0, KEY_READ, &software_meta) == ERROR_SUCCESS)
	{
		BYTE data[MAX_PATH];
		DWORD type, size = sizeof(data) / sizeof(data[0]);
		if (RegQueryValueExA(software_meta, "META_PATH", 0, &type, data, &size) == ERROR_SUCCESS)
		{
			metapath = std::string(data, data + strnlen((const char*)data, size));
		}
		RegCloseKey(software_meta);
	}
	if (!metapath.length())
	{
		throw python_error("Could not read META_PATH from HKLM\\Software\\META");
	}
	return metapath;
}

HMODULE LoadPythonDll(const std::string& metapath) {
	std::string python_dll_path = metapath + "\\bin\\Python27\\Scripts\\python27.dll";
	HMODULE python_dll = LoadLibraryA(python_dll_path.c_str());
	if (python_dll == nullptr)
		throw python_error("Could not load Python27.dll at " + python_dll_path);
	return python_dll;
}

// This is the main component method for interpereters and plugins. 
// May als be used in case of invokeable addons
STDMETHODIMP RawComponent::InvokeEx( IMgaProject *project,  IMgaFCO *currentobj,  
									IMgaFCOs *selectedobjs,  long param) 
{
	bool quiet_mode = false;
	auto _quiet_mode_it = componentParameters.find(_bstr_t(L"_quiet_mode"));
	if (_quiet_mode_it != componentParameters.end() && _quiet_mode_it->second.vt == VT_BOOL && _quiet_mode_it->second.boolVal == VARIANT_TRUE)
	{
		quiet_mode = true;
	}

	CComPtr<IMgaProject>ccpProject(project);
    long status = 0;
    ccpProject->get_ProjectStatus(&status);

	try
	{
		// Setting up the console
		GMEConsole::Console::SetupConsole(ccpProject);

		CComBSTR projname;
		CComBSTR focusname = "<nothing>";
		CComPtr<IMgaTerritory> terr;
		COMTHROW(ccpProject->CreateTerritory(NULL, &terr));

		try
		{
			// Opening backend
			if (!(status & 8))
				COMTHROW(ccpProject->BeginTransaction(terr, TRANSACTION_NON_NESTED));

			std::string metapath = GetMetaPath();
			HMODULE python_dll = LoadPythonDll(metapath);
			RAIIFreeLibrary python_dll_cleanup(python_dll);
			
			CComPtr<IMgaFCO> ccpFocus(currentobj);

			std::set<CComPtr<IMgaFCO> > selectedObjects;

			CComPtr<IMgaFCOs> ccpSelObject(selectedobjs);

			MGACOLL_ITERATE(IMgaFCO,ccpSelObject){
				if(MGACOLL_ITER)
				{
					selectedObjects.insert(MGACOLL_ITER);
				}
			} MGACOLL_ITERATE_END;

			std::string workingDir;
			_bstr_t tmpbstr;
			const char* mgaFile = 0;
			auto original_project_it = componentParameters.find(_bstr_t(L"original_project_file"));
			if (original_project_it != componentParameters.end() && original_project_it->second.vt == VT_BSTR && wcslen(original_project_it->second.bstrVal))
			{
				tmpbstr = original_project_it->second.bstrVal;
				mgaFile = static_cast<const char*>(tmpbstr);
			}
			else
			{
				COMTHROW(project->get_ProjectConnStr(tmpbstr.GetAddress()));
				if (_wcsnicmp(L"MGA=", tmpbstr, 4) == 0)
				{
					mgaFile = (static_cast<const char*>(tmpbstr) + 4);
				}
			}
			if (mgaFile)
			{
				char fullPath[MAX_PATH] = { '\0' };
				LPSTR filepart;
				if (GetFullPathNameA(mgaFile, sizeof(fullPath) / sizeof(fullPath[0]), fullPath, &filepart))
				{
					*(filepart-1) = '\0';
					workingDir = fullPath;
				}
			}

			Main(metapath, project, currentobj, selectedObjects, param, componentParameters, workingDir);


            if (!(status & 8))
                COMTHROW(ccpProject->CommitTransaction());
            terr = 0;

		}
        catch (python_error &exc)
        {
            if (!(status & 8))
                COMTHROW(ccpProject->AbortTransaction());
            throw;
        }
		catch (std::exception& e)
		{
			if (!(status & 8))
				COMTHROW(ccpProject->AbortTransaction());
			throw;
		}
    }
	catch (python_error& e)
	{
		if (componentParameters.find(L"output_dir") != componentParameters.end())
		{
			_bstr_t output_dir = componentParameters.find(L"output_dir")->second;
			if (output_dir.length() > 0)
			{
				std::ofstream failed_txt(static_cast<const wchar_t*>(output_dir + L"\\_FAILED.txt"));
				if (failed_txt.good())
				{
					failed_txt << e.what();
				}
				else
				{
					ASSERT(false);
				}
				std::ofstream CyPhyPython_error(static_cast<const wchar_t*>(output_dir + L"\\CyPhyPython_error.txt"));
				if (CyPhyPython_error.good())
				{
					CyPhyPython_error << e.what();
				}
				else
				{
					ASSERT(false);
				}
			}
		}
		if (quiet_mode == false)
		{
			GMEConsole::Console::Error::writeLine(html_encode<char>(e.what()));
		}
		GMEConsole::Console::gmeoleapp = 0;
		ThrowComError(E_FAIL, _bstr_t(e.what()));
	}
	catch (std::exception& e)
	{
		if (quiet_mode == false)
		{
			GMEConsole::Console::Error::writeLine(html_encode<char>(e.what()));
		}
		GMEConsole::Console::gmeoleapp = 0;
		ThrowComError(E_FAIL, _bstr_t(e.what()));
	}
	catch (...)
	{
		ccpProject->AbortTransaction();
		GMEConsole::Console::gmeoleapp = 0;
		// This can be a problem with the GME Console, so we display it in a message box
		AfxMessageBox(L"An unexpected error has occurred during the interpretation process.");
		ThrowComError(E_FAIL, L"An unexpected error has occurred during the interpretation process.");
	}
	GMEConsole::Console::gmeoleapp = 0;
	return S_OK;

}

// GME currently does not use this function
// you only need to implement it if other invokation mechanisms are used
STDMETHODIMP RawComponent::ObjectsInvokeEx( IMgaProject *project,  IMgaObject *currentobj,  IMgaObjects *selectedobjs,  long param) {
	if(interactive) {
		AfxMessageBox(L"The ObjectsInvoke method is not implemented");
	}
	return E_MGA_NOT_SUPPORTED;
}


// implement application specific parameter-mechanism in these functions:
STDMETHODIMP RawComponent::get_ComponentParameter(BSTR name, VARIANT *pVal) {
	if (wcscmp(name, L"parameter_list") == 0)
	{
		CComSafeArray<BSTR> safe;
		for (map<_bstr_t, _variant_t>::const_iterator it = componentParameters.begin(); it != componentParameters.end(); ++it)
		{
			safe.Add(_bstr_t(it->first));
		}
		safe.Add(_bstr_t(L"script_file"));
		CComVariant(safe).Detach(pVal);
		return S_OK;
	}
	auto ent = componentParameters.find(_bstr_t(name));
	if (ent != componentParameters.end())
	{
		_variant_t copy = ent->second;
		*pVal = copy.Detach();
		return S_OK;
	}

	CComVariant(L"").Detach(pVal);
	return S_OK;
}

STDMETHODIMP RawComponent::put_ComponentParameter(BSTR name, VARIANT newVal) {
	componentParameters[_bstr_t(name)] = _variant_t(newVal);
	return S_OK;
}

