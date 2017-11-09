
#ifndef __INTELLISENSE__
#import "progid:Mga.MgaMetaFolder" /*no_implementation*/ auto_search no_namespace no_search_namespace
#import "progid:Mga.MgaFolders" /*no_implementation*/ auto_search no_namespace no_search_namespace
#import "libid:BF48BD03-072D-41A4-95F1-3200310005A1" auto_search
#else
// IntelliSense has a known issue with the above lines.
//  http://connect.microsoft.com/VisualStudio/feedback/details/533526/vc-2010-intellisense-import-directive-using-libid-does-not-work
#ifdef _DEBUG
// If IntelliSense reports "Cannot open source file", compile then reopen the project
#include "Debug\Mga.tlh"
#include "Debug\ReferenceSwitcher.tlh"
#else
#include "Release\Mga.tlh"
#include "Release\ReferenceSwitcher.tlh"
#endif
#endif

#include <Objbase.h>

#define __Meta_h__
#define __Mga_h__


__declspec(noreturn) void ThrowComError(HRESULT hr, LPOLESTR err);

void RunCyPhyElaborate_ComputePropertyValues(IMgaFCO *currentobj)
{
	IDispatchPtr disp;
	HRESULT hr = disp.CreateInstance(L"MGA.Interpreter.CyPhyElaborateCS", nullptr, CLSCTX_INPROC);
	if (FAILED(hr))
	{
		ThrowComError(hr, L"Could not create CyPhyElaborateCS");
	}
	_variant_t obj = currentobj;
	DISPID dispid;
	wchar_t* name = L"ComputePropertyValues";
	hr = disp->GetIDsOfNames(IID_NULL, &name, 1, LOCALE_USER_DEFAULT, &dispid);
	if (FAILED(hr))
	{
		throw _com_error(hr, nullptr);
	}
	DISPPARAMS dispparams = { &obj, NULL, 1, 0 };
	EXCEPINFO info;
	hr = disp->Invoke(dispid, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &dispparams, nullptr, &info, NULL);
	if (FAILED(hr))
	{
		if (hr == DISP_E_EXCEPTION)
		{
			ThrowComError(hr, info.bstrDescription);
		}
		else
		{
			IErrorInfo *info;
			if (SUCCEEDED(GetErrorInfo(0, &info))) {
				throw _com_error(hr, info);
			}
		}
		throw _com_error(hr, nullptr);
	}
}
