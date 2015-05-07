#include <Windows.h>
#include <string>

STDAPI DllRegisterServer(void);
STDAPI DllUnregisterServer(void);

EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)

static HMODULE python_dll = 0;
static void LoadPython()
{
	if (python_dll != 0)
		return;
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
        OutputDebugStringA("Could not read META_PATH from HKLM\\Software\\META\n");
        return;
    }
#ifdef BOOST_DEBUG_PYTHON
    std::string python_dll_path = metapath + "\\bin\\Python27\\Scripts\\python27_d.dll";
#else
    std::string python_dll_path = metapath + "\\bin\\Python27\\Scripts\\python27.dll";
#endif
    python_dll = LoadLibrary(python_dll_path.c_str());
    if (python_dll == nullptr)
        OutputDebugStringA(("Could not load Python27.dll at " + python_dll_path + "\n").c_str());
}

static HMODULE cyber2slc_codegen = 0;
static void LoadCyber2SLC_CodeGen()
{
	if (cyber2slc_codegen != 0)
		return;
	TCHAR loader_path[MAX_PATH];
    if (GetModuleFileName(HINST_THISCOMPONENT, loader_path, MAX_PATH) != 0)
	{
		TCHAR fullpath[MAX_PATH];
		TCHAR* filepart;
		if (GetFullPathName(loader_path, MAX_PATH, fullpath, &filepart) != 0 && filepart != NULL)
		{
			*filepart = 0;
			std::string path_with_dll = fullpath;
#ifdef _DEBUG
			path_with_dll += "Cyber2SLC_CodeGend.dll";
#else
			path_with_dll += "Cyber2SLC_CodeGen.dll";
#endif
			cyber2slc_codegen = LoadLibraryEx(path_with_dll.c_str(), NULL, LOAD_WITH_ALTERED_SEARCH_PATH);
		}
    }
}

STDAPI DllGetClassObject_(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
	LoadPython();
	if (python_dll == NULL)
		return E_FAIL;
	LoadCyber2SLC_CodeGen();
	if (cyber2slc_codegen == NULL)
		return E_FAIL;
	typedef HRESULT (STDAPICALLTYPE *stdcall_DllGetClassObject)(REFCLSID rclsid, REFIID riid, LPVOID* ppv);
	stdcall_DllGetClassObject _DllGetClassObject = (stdcall_DllGetClassObject) GetProcAddress(cyber2slc_codegen, "DllGetClassObject");
	return (*_DllGetClassObject)(rclsid, riid, ppv);
}

typedef HRESULT (STDAPICALLTYPE *stdcall_noarg)();
STDAPI DllCanUnloadNow_(void)
{
	LoadPython();
	if (python_dll == NULL)
		return E_FAIL;
	LoadCyber2SLC_CodeGen();
	if (cyber2slc_codegen == NULL)
		return E_FAIL;
	stdcall_noarg _DllCanUnloadNow = (stdcall_noarg) GetProcAddress(cyber2slc_codegen, "DllCanUnloadNow");
	HRESULT hr = (*_DllCanUnloadNow)();
	if (hr == S_OK)
	{
		// We intentionally don't unload either Cyber2SLC or Python27.dll
		// http://www.boost.org/doc/libs/1_55_0/libs/python/doc/tutorial/doc/html/python/embedding.html states Py_Finalize must not be called
		// Cyber2SLC has Python types defined, and without Py_Finalize, there may be instances of those types still around
		// If Cyber2SLC was unloaded, those instances would have garbage for a type
		//if (cyber2slc_codegen != NULL)
		//	FreeLibrary(cyber2slc_codegen);
		//if (python_dll != NULL)
		//	FreeLibrary(python_dll);
	}
	return hr;
}

STDAPI DllRegisterServer_(void)
{
	LoadPython();
	if (python_dll == NULL)
		return E_FAIL;
	LoadCyber2SLC_CodeGen();
	if (cyber2slc_codegen == NULL)
		return E_FAIL;
	stdcall_noarg _DllRegisterServer = (stdcall_noarg) GetProcAddress(cyber2slc_codegen, "DllRegisterServer");
	HRESULT hr = (*_DllRegisterServer)();
	if (FAILED(hr))
		return hr;
	TCHAR loader_path[MAX_PATH];
    if (GetModuleFileName(HINST_THISCOMPONENT, loader_path, MAX_PATH) == 0)
		return HRESULT_FROM_WIN32(GetLastError());
	if (RegSetValue(HKEY_CLASSES_ROOT, "CLSID\\{1A98A829-6FFA-42E9-8F97-8A8804D44662}\\InProcServer32", REG_SZ, loader_path, 0) != ERROR_SUCCESS)
		return HRESULT_FROM_WIN32(GetLastError());
	return S_OK;
}

STDAPI DllUnregisterServer_(void)
{
	LoadPython();
	if (python_dll == NULL)
		return E_FAIL;
	LoadCyber2SLC_CodeGen();
	if (cyber2slc_codegen == NULL)
		return E_FAIL;
	stdcall_noarg _DllUnregisterServer = (stdcall_noarg) GetProcAddress(cyber2slc_codegen, "DllUnregisterServer");
	return (*_DllUnregisterServer)();
}

BOOLEAN WINAPI DllMain(HINSTANCE hDllHandle, DWORD nReason, LPVOID Reserved)
{
	switch (nReason)
	{
		case DLL_PROCESS_ATTACH:
			DisableThreadLibraryCalls(hDllHandle);
		break;

		case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}
