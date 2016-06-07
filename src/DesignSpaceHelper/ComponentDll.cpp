// Component.cpp : Defines the initialization routines for the DLL.
//

#include "StdAfx.h"
//#include <afxctl.h>

#include "MgaUtil.h"
#include "ComponentDll.h"
#include "ComponentLib.h"

extern bool const g_REGISTER_SYSTEMWIDE;
extern const TCHAR* g_COMPONENT_NAME;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//
//	Note!
//
//		If this DLL is dynamically linked against the MFC
//		DLLs, any functions exported from this DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

/////////////////////////////////////////////////////////////////////////////
// CComponentApp

BEGIN_MESSAGE_MAP(CComponentApp, CWinApp)
	//{{AFX_MSG_MAP(CComponentApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CComponentApp construction

CComponentApp::CComponentApp() : CWinApp(g_COMPONENT_NAME)
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CComponentApp object

CComponentApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CComponentApp initialization

BOOL CComponentApp::InitInstance()
{
#if _MSC_VER < 1700
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
			szData[dwDataBufSize] = '\0';
			uAfxSetAmbientActCtxMod = _tcstoul(szData, NULL, 10);
		}

		RegCloseKey(hKey);
	}
	if (uAfxSetAmbientActCtxMod != 0)
	{
		AfxSetAmbientActCtx(FALSE);
	}
#endif

	BOOL success = CWinApp::InitInstance();
	ASSERT(success == TRUE);

	// Register all OLE server (factories) as running.  This enables the
	//  OLE libraries to create objects from other applications.
	success = COleObjectFactory::RegisterAll();
	ASSERT(success == TRUE);

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// Special entry points required for inproc servers

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return AfxDllGetClassObject(rclsid, riid, ppv);
}

STDAPI DllCanUnloadNow(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return AfxDllCanUnloadNow();
}


#define COMRETURN(hr) { HRESULT res; if((res = (hr)) != S_OK) return res; }

// this flag supresses running Dll(Un)RegisterServer if DllInstall is also used
// bool called = false; 

// by exporting DllRegisterServer, you can use regsvr.exe
STDAPI DllRegisterServer(void)
{
/*	
	if(called) return S_OK;
	called = true;
*/
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (!CUACUtils::isElevated()) {
		AfxMessageBox(_T("Component registration requires elevated rights on Windows Vista or later."), MB_ICONSTOP | MB_OK);
		return SELFREG_E_CLASS;
	}

	if (!COleObjectFactory::UpdateRegistryAll(TRUE)) {
		return SELFREG_E_CLASS;
	}

	// Note:
	// We can register the typelib either from the .tlb file
	// or from the resources. But the resource ID of the 
	// TYPELIB must be 1 !!!

	if( !AfxOleRegisterTypeLib(AfxGetInstanceHandle(), LIBID_MgaComponentLib, NULL) )
		return E_FAIL;

	CComponentReg reg;
	if (g_REGISTER_SYSTEMWIDE)
		COMRETURN(reg.RegisterParadigms(REGACCESS_SYSTEM))
	else
		COMRETURN(reg.RegisterParadigms())
	

	return S_OK;
}

STDAPI DllUnregisterServer(void)
{
/*	
	if(called) return S_OK;
	called = true;
*/
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (!COleObjectFactory::UpdateRegistryAll(FALSE))
		return SELFREG_E_CLASS;

	CComponentReg reg;
    if (g_REGISTER_SYSTEMWIDE)
		COMRETURN(reg.UnregisterParadigms(REGACCESS_SYSTEM))
	else
		COMRETURN(reg.UnregisterParadigms())
	
	return S_OK;
}

#define DIM(x) (sizeof(x)/sizeof((x)[0]))

STDAPI DllInstall(BOOL bInstall, LPCWSTR cmdl) {

	regaccessmode_enum code = REGACCESS_NONE;
	if(iswdigit(cmdl[0]) && cmdl[1] == 0) {
		code = regaccessmode_enum(cmdl[0] - L'0');
	}
	else {
		struct {
			LPCWSTR name;
			regaccessmode_enum cc;
		} mnemonics[] = { { L"USER" , REGACCESS_USER},
						  { L"REGACCESS_USER" , REGACCESS_USER},	
						  { L"SYSTEM" , REGACCESS_SYSTEM},	
						  { L"REGACCESS_SYSTEM" , REGACCESS_SYSTEM},	
						  { L"BOTH" , REGACCESS_BOTH},	
						  { L"REGACCESS_BOTH" , REGACCESS_BOTH} };
		for(int i = 0; i < DIM(mnemonics); i++) {
			if(_wcsicmp(cmdl, mnemonics[i].name) == 0) {
				code = mnemonics[i].cc;
				break;
			}
		}
	}
	
//	if(called || code == REGACCESS_NONE) return E_INVALIDARG;
//	called = true;

	CComponentReg reg;
	if(bInstall) { COMRETURN(reg.RegisterParadigms(code)); }
	else		 { COMRETURN(reg.UnregisterParadigms(code)); }
	return S_OK;
};



