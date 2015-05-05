// mfcdual.cpp: Helpful functions for adding dual interface support to
//              MFC applications

// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

#include "stdafx.h"

#include <afxpriv.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DualHandleException

HRESULT DualHandleException(REFIID riidSource, const CException* pAnyException)
{
	ASSERT_VALID(pAnyException);

	TRACE0("DualHandleException called\n");

	// Set ErrInfo object so that VTLB binding container
	// applications can get rich error information.
	ICreateErrorInfo* pcerrinfo;
	HRESULT hr = CreateErrorInfo(&pcerrinfo);
	if (SUCCEEDED(hr))
	{
		TCHAR   szDescription[256];
		LPCTSTR pszDescription = szDescription;
		GUID    guid = GUID_NULL;
		DWORD   dwHelpContext = 0;
		BSTR    bstrHelpFile = NULL;
		BSTR    bstrSource = NULL;
		if (pAnyException->IsKindOf(RUNTIME_CLASS(COleDispatchException)))
		{
			// specific IDispatch style exception
			COleDispatchException* e = (COleDispatchException*)pAnyException;

			guid = riidSource;
			hr = MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF,
							  (e->m_wCode + 0x200));

			pszDescription = e->m_strDescription;
			dwHelpContext = e->m_dwHelpContext;

			// propagate source and help file if present
			// call ::SysAllocString directly so no further exceptions are thrown
			if (!e->m_strHelpFile.IsEmpty()) {
				CT2COLE strHelpFile(e->m_strHelpFile);
				bstrHelpFile = ::SysAllocString(strHelpFile);
			}
			if (!e->m_strSource.IsEmpty()) {
				CT2COLE strSource(e->m_strSource);
				bstrSource = ::SysAllocString(strSource);
			}

		}
		else if (pAnyException->IsKindOf(RUNTIME_CLASS(CMemoryException)))
		{
			// failed memory allocation
			AfxLoadString(AFX_IDP_FAILED_MEMORY_ALLOC, szDescription);
			hr = E_OUTOFMEMORY;
		}
		else
		{
			// other unknown/uncommon error
			AfxLoadString(AFX_IDP_INTERNAL_FAILURE, szDescription);
			hr = E_UNEXPECTED;
		}

		if (bstrHelpFile == NULL && dwHelpContext != 0) {
			CT2COLE strHelpFilePath(AfxGetApp()->m_pszHelpFilePath);
			bstrHelpFile = ::SysAllocString(strHelpFilePath);
		}

		if (bstrSource == NULL) {
			CT2COLE strAppName(AfxGetAppName());
			bstrSource = ::SysAllocString(strAppName);
		}

		// Set up ErrInfo object
		pcerrinfo->SetGUID(guid);
		CT2COLE strDescription(pszDescription);
		pcerrinfo->SetDescription(::SysAllocString(strDescription));
		pcerrinfo->SetHelpContext(dwHelpContext);
		pcerrinfo->SetHelpFile(bstrHelpFile);
		pcerrinfo->SetSource(bstrSource);

		TRACE("\tSource = %ws\n", bstrSource);
		TRACE("\tDescription = %s\n", pszDescription);
		TRACE("\tHelpContext = %lx\n", dwHelpContext);
		TRACE("\tHelpFile = %ws\n", bstrHelpFile);

		// Set the ErrInfo object for the current thread
		IErrorInfo* perrinfo;
		if (SUCCEEDED(pcerrinfo->QueryInterface(IID_IErrorInfo, (LPVOID*)&perrinfo)))
		{
			SetErrorInfo(0, perrinfo);
			perrinfo->Release();
		}

		pcerrinfo->Release();
	}

	TRACE("DualHandleException returning HRESULT %lx\n", hr);

	return hr;
}
