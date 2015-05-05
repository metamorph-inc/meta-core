
#include "stdafx.h"
#include "DecoratorMfc.h"
#include <stdio.h>

// --------------------------- Error

void DisplayError(const TCHAR *msg, HRESULT hr)
{
	ASSERT( msg != NULL );

	CString a;
	a.Format(_T(" (0x%x)"), hr);
	a.Insert(0, msg);

	CString desc;
	GetErrorInfo(hr, PutOut(desc));

	if( !desc.IsEmpty() )
	{
		a += ": ";
		a += desc;
	}

	AfxMessageBox(a, MB_OK | MB_ICONSTOP);
}

