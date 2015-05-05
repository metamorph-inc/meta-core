// DecoratorApp.cpp : Implementation of DLL Exports.


// Note: Proxy/Stub Information
//      To build a separate proxy/stub DLL, 
//      run nmake -f DecoratorAppps.mk in the project directory.

#include "stdafx.h"
#include <initguid.h>
#include "Resource.h"
#include "DecoratorLib.h"
#include "DecoratorConfig.h"
#include "DecoratorLib.h"
#include "DecoratorImpl.h"


CComModule _Module;

BEGIN_OBJECT_MAP(ObjectMap)
OBJECT_ENTRY(CLSID_Decorator, CDecoratorImpl)
END_OBJECT_MAP()

class CDecoratorApp : public CWinApp
{
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDecoratorApp)
	public:
    virtual BOOL InitInstance();
    virtual int ExitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CDecoratorApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

BEGIN_MESSAGE_MAP(CDecoratorApp, CWinApp)
	//{{AFX_MSG_MAP(CDecoratorApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CDecoratorApp theApp;

BOOL CDecoratorApp::InitInstance()
{
	// See MSDN example code for CWinApp::InitInstance: http://msdn.microsoft.com/en-us/library/ae6yx0z0.aspx
	// MFC module state handling code is changed with VC80.
	// We follow the Microsoft's suggested way, but in case of any trouble the set the
	// HKCU\Software\GME\AfxSetAmbientActCtxMod key to 0
	UINT uAfxSetAmbientActCtxMod = 1;
	HKEY hKey;
	if (RegOpenKeyEx(HKEY_CURRENT_USER, _T("Software\\GME\\"),
					 0, KEY_QUERY_VALUE, &hKey) == ERROR_SUCCESS)
	{
		TCHAR szData[128];
		DWORD dwKeyDataType;
		DWORD dwDataBufSize = sizeof(szData)/sizeof(TCHAR);

		if (RegQueryValueEx(hKey, _T("AfxSetAmbientActCtxMod"), NULL, &dwKeyDataType,
							(LPBYTE) &szData, &dwDataBufSize) == ERROR_SUCCESS)
		{
			uAfxSetAmbientActCtxMod = _tcstoul(szData, NULL, 10);
		}

		RegCloseKey(hKey);
	}
	if (uAfxSetAmbientActCtxMod != 0)
	{
		AfxSetAmbientActCtx(FALSE);
	}

    _Module.Init(ObjectMap, m_hInstance, &LIBID_DecoratorLib);
    return CWinApp::InitInstance();
}

int CDecoratorApp::ExitInstance()
{
    _Module.Term();
    return CWinApp::ExitInstance();
}

/////////////////////////////////////////////////////////////////////////////
// Used to determine whether the DLL can be unloaded by OLE

STDAPI DllCanUnloadNow(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

    return (AfxDllCanUnloadNow()==S_OK && _Module.GetLockCount()==0) ? S_OK : S_FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// Returns a class factory to create an object of the requested type

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
    return _Module.GetClassObject(rclsid, riid, ppv);
}

/////////////////////////////////////////////////////////////////////////////
// DllRegisterServer - Adds entries to the system registry

STDAPI DllRegisterServer(void)
{
	_ATL_REGMAP_ENTRY regMap[] = {
		{CONSTOLESTR("COCLASS_PROGID"), CONSTOLESTR(COCLASS_PROGID)},
		{CONSTOLESTR("COCLASS_NAME"), CONSTOLESTR(COCLASS_NAME)},
		{CONSTOLESTR("COCLASS_UUID"), CONSTOLESTR(COCLASS_UUID)},
		{CONSTOLESTR("TYPELIB_UUID"), CONSTOLESTR(TYPELIB_UUID)},
		{0, 0}
	};
	HRESULT hr = _Module.UpdateRegistryFromResourceD( IDR_DECORATOR, TRUE, regMap );
	/* if ( SUCCEEDED( hr ) ) //do not use this !
    {
		// registers object, typelib and all 
		// interfaces in typelib
		hr = _Module.RegisterServer(TRUE);
    }
	*/
	return hr;
}

/////////////////////////////////////////////////////////////////////////////
// DllUnregisterServer - Removes entries from the system registry

STDAPI DllUnregisterServer(void)
{
		_ATL_REGMAP_ENTRY regMap[] = {
		{CONSTOLESTR("COCLASS_PROGID"), CONSTOLESTR(COCLASS_PROGID)},
		{CONSTOLESTR("COCLASS_NAME"), CONSTOLESTR(COCLASS_NAME)},
		{CONSTOLESTR("COCLASS_UUID"), CONSTOLESTR(COCLASS_UUID)},
		{CONSTOLESTR("TYPELIB_UUID"), CONSTOLESTR(TYPELIB_UUID)},
		{0, 0}
	};
	HRESULT hr = _Module.UpdateRegistryFromResourceD( IDR_DECORATOR, TRUE, regMap );
	/* if ( SUCCEEDED( hr ) ) //do not use this !
    {
		// registers object, typelib and all 
		// interfaces in typelib
		hr = _Module.UnregisterServer(TRUE);
    }
	*/
	return hr;
}


