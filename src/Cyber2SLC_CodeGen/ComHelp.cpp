#include "StdAfx.h"

#include "Comdef.h"

extern const wchar_t* g_COCLASS_PROGIDW;

void SetErrorInfo(HRESULT hr, const wchar_t* err)
{
	ICreateErrorInfoPtr errCreate;
	if (SUCCEEDED(CreateErrorInfo(&errCreate))
		&& SUCCEEDED(errCreate->SetDescription(const_cast<wchar_t*>(err)))
		&& SUCCEEDED(errCreate->SetSource(const_cast<wchar_t*>(g_COCLASS_PROGIDW)))
		)
	{
		IErrorInfoPtr errorInfo = errCreate;
		SetErrorInfo(0, errorInfo);
	}
}

__declspec(noreturn) void ThrowComError(HRESULT hr, LPOLESTR err)
{
	ICreateErrorInfoPtr errCreate;
	if (SUCCEEDED(CreateErrorInfo(&errCreate))
		&& SUCCEEDED(errCreate->SetDescription(const_cast<wchar_t*>(err)))
		&& SUCCEEDED(errCreate->SetSource(const_cast<wchar_t*>(g_COCLASS_PROGIDW)))
		)
	{
		IErrorInfoPtr errorInfo = errCreate;
		throw _com_error(hr, errorInfo.Detach());
	}
	throw _com_error(hr);
}
