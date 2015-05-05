
#include "stdafx.h"
#include "DecoratorSmart.h"

// --------------------------- CComBstrObj

int CComBstrObj::Compare(BSTR q) const
{
	unsigned int pl = p ? SysStringLen(p) : 0;
	unsigned int ql = q ? SysStringLen(q) : 0;

	if( pl != ql )
		return pl - ql;

	return wcsncmp(p, q, pl);
}

void CopyTo(const char *p, int len, BSTR *b)
{
	ASSERT( len >= 0 );
	ASSERT( b != NULL );

	if(*b)
	{
		SysFreeString(*b);
		*b = NULL;
	}

	if( len <= 0 )
		return;

	UINT acp = GetACP();
	int blen = MultiByteToWideChar(acp, 0, p, len, NULL, 0);

	if( blen <= 0 )
		HR_THROW(E_CONVERSION);

	*b = SysAllocStringLen(NULL, blen);
	if( *b == NULL )
		HR_THROW(E_OUTOFMEMORY);

	int tlen = MultiByteToWideChar(acp, 0, p, len, *b, blen);

	if( tlen <= 0 )
		HR_THROW(E_CONVERSION);
	
	ASSERT( tlen == blen );

	(*b)[blen] = '\0';
}

int GetCharLength(const OLECHAR *p, int olelen)
{
	ASSERT( olelen >= -1 );

	if( olelen == 0 )
		return 0;

	UINT acp = GetACP();

	int charlen = WideCharToMultiByte(acp, 0, p, olelen,
		NULL, 0, NULL, NULL);

	// zero if failed
	ASSERT( charlen > 0 );

	return charlen;
}

void CopyTo(const OLECHAR *p, int olelen, char *s, int charlen)
{
	ASSERT( olelen >= -1 && charlen >= 0 );
	ASSERT( charlen == 0 || p != NULL );

	if( charlen <= 0 )
		return;

	UINT acp = GetACP();

	int len = WideCharToMultiByte(acp, 0, p, olelen, 
		s, charlen, NULL, NULL);

	// zero if failed
	ASSERT( len > 0 );

	ASSERT( len == charlen );
}		

void CopyTo(const OLECHAR *p, GUID &guid)
{
	ASSERT( p != NULL );

	if( FAILED(IIDFromString(const_cast<OLECHAR*>(p), static_cast<IID*>(&guid))) )
		HR_THROW(E_CONVERSION);
}

void CopyTo(const GUID &guid, BSTR *p)
{
	ASSERT( p != NULL );

	OLECHAR *q = NULL;

	if( FAILED(StringFromIID(static_cast<IID>(guid), &q)) )
		HR_THROW(E_CONVERSION);

	if(*p != NULL)
		SysFreeString(*p);
	
	*p = SysAllocString(q);
	CoTaskMemFree(q);
}

// --------------------------- CComSafeArray

void CopyTo(const CComBstrObj *start, const CComBstrObj *end, SAFEARRAY **p)
{
	ASSERT( p != NULL && *p == NULL );
	ASSERT( start <= end );

	*p = SafeArrayCreateVector(VT_BSTR, 1, end - start);
	if( *p == NULL )
		HR_THROW(E_OUTOFMEMORY);

	try
	{
		BSTR *q = NULL;

		COMTHROW( SafeArrayAccessData(*p, (void**)&q) );
		ASSERT( q != NULL );

		while( start < end )
		{
			CopyTo(*start, q);

			++q;
			++start;
		}

		COMTHROW( SafeArrayUnaccessData(*p) );
	}
	catch(hresult_exception &)
	{
		SafeArrayDestroy(*p);
		*p = NULL;

		throw;
	}
}

void MoveTo(CComBstrObj *start, CComBstrObj *end, SAFEARRAY **p)
{
	ASSERT( p != NULL && *p == NULL );
	ASSERT( start <= end );

	*p = SafeArrayCreateVector(VT_BSTR, 1, end - start);
	if( *p == NULL )
		HR_THROW(E_OUTOFMEMORY);

	try
	{
		BSTR *q = NULL;

		COMTHROW( SafeArrayAccessData(*p, (void**)&q) );
		ASSERT( q != NULL );

		while( start < end )
		{
			MoveTo(*start, q);

			++q;
			++start;
		}

		COMTHROW( SafeArrayUnaccessData(*p) );
	}
	catch(hresult_exception &)
	{
		SafeArrayDestroy(*p);
		*p = NULL;

		throw;
	}
}

void CopyTo(const unsigned char *start, const unsigned char *end, SAFEARRAY **p)
{
	ASSERT( p != NULL && *p == NULL );
	ASSERT( start <= end );

	*p = SafeArrayCreateVector(VT_UI1, 1, end - start);
	if( *p == NULL )
		HR_THROW(E_OUTOFMEMORY);

	try
	{
		unsigned char *q = NULL;

		COMTHROW( SafeArrayAccessData(*p, (void**)&q) );
		ASSERT( q != NULL );

		while( start < end )
		{
			*q = *start;

			++q;
			++start;
		}

		COMTHROW( SafeArrayUnaccessData(*p) );
	}
	catch(hresult_exception &)
	{
		SafeArrayDestroy(*p);
		*p = NULL;

		throw;
	}
}

void CopyTo(const long *start, const long *end, SAFEARRAY **p)
{
	ASSERT( p != NULL && *p == NULL );
	ASSERT( start <= end );

	*p = SafeArrayCreateVector(VT_I4, 1, end - start);
	if( *p == NULL )
		HR_THROW(E_OUTOFMEMORY);

	try
	{
		long *q = NULL;

		COMTHROW( SafeArrayAccessData(*p, (void**)&q) );
		ASSERT( q != NULL );

		while( start < end )
		{
			*q = *start;

			++q;
			++start;
		}

		COMTHROW( SafeArrayUnaccessData(*p) );
	}
	catch(hresult_exception &)
	{
		SafeArrayDestroy(*p);
		*p = NULL;

		throw;
	}
}

void CopyTo(const GUID *start, const GUID *end, SAFEARRAY **p)
{
	ASSERT( p != NULL && *p == NULL );
	ASSERT( start <= end );

	SAFEARRAYBOUND bounds[2];
	bounds[0].cElements = end - start;
	bounds[0].lLbound = 1;
	bounds[1].cElements = sizeof(GUID);
	bounds[1].lLbound = 0;

	*p = SafeArrayCreate(VT_UI1, 2, bounds);
	if( *p == NULL )
		HR_THROW(E_OUTOFMEMORY);

	try
	{
		GUID *q = NULL;

		COMTHROW( SafeArrayAccessData(*p, (void**)&q) );
		ASSERT( q != NULL );

		while( start < end )
		{
			*q = *start;

			++q;
			++start;
		}

		COMTHROW( SafeArrayUnaccessData(*p) );
	}
	catch(hresult_exception &)
	{
		SafeArrayDestroy(*p);
		*p = NULL;

		throw;
	}
}

long GetArrayLength(SAFEARRAY *p)
{
	ASSERT( p != NULL );

	if( !(p->cDims == 1 || p->cDims == 2) )
		HR_THROW(E_INVALIDARG);

	ASSERT( p->rgsabound[0].cElements >= 0 );
	return p->rgsabound[0].cElements;
}

void GetArrayStart(SAFEARRAY *p, CComBstrObj *&start)
{
	ASSERT( p != NULL );

	if( (p->fFeatures & FADF_BSTR) != FADF_BSTR )
		HR_THROW(E_INVALIDARG);

	start = (CComBstrObj*)p->pvData;
}

void GetArrayStart(SAFEARRAY *p, long *&start)
{
	ASSERT( p != NULL );

	if( p->cbElements != 4 )
		HR_THROW(E_INVALIDARG);

	start = (long*)p->pvData;
}

void GetArrayStart(SAFEARRAY *p, unsigned char *&start)
{
	ASSERT( p != NULL );

	if( p->cbElements != 1 )
		HR_THROW(E_INVALIDARG);

	start = (unsigned char*)p->pvData;
}

void GetArrayStart(SAFEARRAY *p, GUID *&start)
{
	ASSERT( p != NULL );

	if( p->cbElements != 1 || p->cDims != 2 || p->rgsabound[1].cElements != sizeof(GUID) )
		HR_THROW(E_INVALIDARG);

	start = (GUID*)p->pvData;
}

void CopyTo(SAFEARRAY *p, CComBstrObj *start, CComBstrObj *end)
{
	ASSERT( p != NULL );
	ASSERT( start <= end );

	ASSERT( (p->fFeatures & FADF_BSTR) == FADF_BSTR );

	BSTR *q = NULL;

	COMTHROW( SafeArrayAccessData(p, (void**)&q) );
	ASSERT( q != NULL );

	try
	{
		ASSERT( GetArrayLength(p) == (end - start) );

		while( start < end )
		{
			CopyTo(*q, *start);

			++q;
			++start;
		}
	}
	catch(hresult_exception &)
	{
		SafeArrayUnaccessData(p);
		throw;
	}

	COMTHROW( SafeArrayUnaccessData(p) );
}

void MoveTo(SAFEARRAY *p, CComBstrObj *start, CComBstrObj *end)
{
	ASSERT( p != NULL );
	ASSERT( start <= end );

	ASSERT( (p->fFeatures & FADF_BSTR) == FADF_BSTR );

	BSTR *q = NULL;

	COMTHROW( SafeArrayAccessData(p, (void**)&q) );
	ASSERT( q != NULL );

	try
	{
		ASSERT( GetArrayLength(p) == (end - start) );

		while( start < end )
		{
			MoveTo(q, *start);

			++q;
			++start;
		}
	}
	catch(hresult_exception &)
	{
		SafeArrayUnaccessData(p);
		throw;
	}

	COMTHROW( SafeArrayUnaccessData(p) );
}

void CopyTo(SAFEARRAY *p, unsigned char *start, unsigned char *end)
{
	ASSERT( p != NULL );
	ASSERT( start <= end );

	ASSERT( p->cbElements == 1 );

	const unsigned char *q = NULL;

	COMTHROW( SafeArrayAccessData(p, (void**)&q) );
	ASSERT( q != NULL );

	ASSERT( GetArrayLength(p) == (end - start) );
	memcpy( start, q, (end - start) * sizeof(unsigned char) );

	COMTHROW( SafeArrayUnaccessData(p) );
}

void CopyTo(SAFEARRAY *p, long *start, long *end)
{
	ASSERT( p != NULL );
	ASSERT( start <= end );

	ASSERT( p->cbElements == 4 );

	const long *q = NULL;

	COMTHROW( SafeArrayAccessData(p, (void**)&q) );
	ASSERT( q != NULL );

	ASSERT( GetArrayLength(p) == (end - start) );
	memcpy( start, q, (end - start) * sizeof(long) );

	COMTHROW( SafeArrayUnaccessData(p) );
}

void CopyTo(SAFEARRAY *p, GUID *start, GUID *end)
{
	ASSERT( p != NULL );
	ASSERT( start <= end );

	ASSERT( p->cbElements == 1 );

	const GUID *q = NULL;

	COMTHROW( SafeArrayAccessData(p, (void**)&q) );
	ASSERT( q != NULL );

	ASSERT( GetArrayLength(p) == (end - start) );
	memcpy( start, q, (end - start) * sizeof(GUID) );

	COMTHROW( SafeArrayUnaccessData(p) );
}

// --------------------------- CComVariant

void CopyTo(unsigned char a, VARIANT *v)
{
	ASSERT( v != NULL );

	if( v->vt != VT_UI1 )
	{
		if( v->vt != VT_EMPTY )
			COMTHROW( VariantClear(v) );

		v->vt = VT_UI1;
	}
	v->bVal = a;
}

void CopyTo(short a, VARIANT *v)
{
	ASSERT( v != NULL );

	if( v->vt != VT_I2 )
	{
		if( v->vt != VT_EMPTY )
			COMTHROW( VariantClear(v) );

		v->vt = VT_I2;
	}
	v->iVal = a;
}

void CopyTo(long a, VARIANT *v)
{
	ASSERT( v != NULL );

	if( v->vt != VT_I4 )
	{
		if( v->vt != VT_EMPTY )
			COMTHROW( VariantClear(v) );

		v->vt = VT_I4;
	}
	v->lVal = a;
}

void CopyTo(double a, VARIANT *v)
{
	ASSERT( v != NULL );

	if( v->vt != VT_R8 )
	{
		if( v->vt != VT_EMPTY )
			COMTHROW( VariantClear(v) );

		v->vt = VT_R8;
	}
	v->dblVal = a;
}

void CopyTo(BSTR b, VARIANT *v)
{
	ASSERT( v != NULL );

	if( v->vt != VT_EMPTY )
		COMTHROW( VariantClear(v) );

	v->bstrVal = SysAllocStringLen(b, SysStringLen(b));
	v->vt = VT_BSTR;
}

void CopyTo(IDispatch *p, VARIANT *v)
{
	ASSERT( v != NULL );

	if( v->vt != VT_EMPTY )
		COMTHROW( VariantClear(v) );

	v->pdispVal = p;
	if(p) p->AddRef();
	v->vt = VT_DISPATCH;
}

void MoveTo(IDispatch **p, VARIANT *v)
{
	ASSERT( p != NULL );
	ASSERT( v != NULL );

	if( v->vt != VT_EMPTY )
		COMTHROW( VariantClear(v) );

	v->pdispVal = *p;
	*p = NULL;
	v->vt = VT_DISPATCH;
}

void CopyTo(const VARIANT &v, unsigned char &a)
{
	if( v.vt == VT_UI1 )
		a = v.bVal;
	else if( v.vt == VT_EMPTY )
		a = 0;
	else
	{
		CComVariant w;
		COMTHROW( w.ChangeType(VT_UI1, &v) );
		ASSERT( w.vt == VT_UI1 );
		a = w.bVal;
	}
}

void CopyTo(const VARIANT &v, short &a)
{
	if( v.vt == VT_I2 )
		a = v.iVal;
	else if( v.vt == VT_EMPTY )
		a = 0;
	else
	{
		CComVariant w;
		COMTHROW( w.ChangeType(VT_I2, &v) );
		ASSERT( w.vt == VT_I2 );
		a = w.iVal;
	}
}

void CopyTo(const VARIANT &v, long &a)
{
	if( v.vt == VT_I4 )
		a = v.lVal;
	else if( v.vt == VT_EMPTY )
		a = 0;
	else
	{
		CComVariant w;
		COMTHROW( w.ChangeType(VT_I4, &v) );
		ASSERT( w.vt == VT_I4 );
		a = w.lVal;
	}
}

void CopyTo(const VARIANT &v, double &a)
{
	if( v.vt == VT_R8 )
		a = v.dblVal;
	else if( v.vt == VT_EMPTY )
		a = 0.0;
	else
	{
		CComVariant w;
		COMTHROW( w.ChangeType(VT_R8, &v) );
		ASSERT( w.vt == VT_R8 );
		a = w.dblVal;
	}
}

void CopyTo(const VARIANT &v, BSTR *a)
{
	ASSERT( a != NULL );

	if( *a != NULL )
	{
		SysFreeString(*a);
		*a = NULL;
	}

	if( v.vt == VT_BSTR )
		*a = SysAllocStringLen(v.bstrVal, SysStringLen(v.bstrVal));
	else if( v.vt != VT_EMPTY )
	{
		CComVariant w;
		COMTHROW( w.ChangeType(VT_BSTR, &v) );
		ASSERT( w.vt == VT_BSTR );

		*a = w.bstrVal;
		w.vt = VT_EMPTY;
	}
}

void CopyTo(const char *p, int len, VARIANT *v)
{
	ASSERT( v != NULL );

	if( v->vt != VT_EMPTY )
		COMTHROW( VariantClear(v) );

	v->bstrVal = NULL;
	CopyTo(p, len, &v->bstrVal);
	v->vt = VT_BSTR;
}

int GetCharLength(VARIANT &v)
{
	if( v.vt == VT_EMPTY )
		return 0;
	else if( v.vt == VT_DISPATCH )
	{
		// VB passes strings as VT_DISPATCH
		COMTHROW( VariantChangeType(&v, &v, 0, VT_BSTR) );
		ASSERT( v.vt == VT_BSTR );
	}

	if( v.vt == VT_BSTR )
		return GetCharLength(v.bstrVal);

	HR_THROW(E_INVALIDARG);
}

void CopyTo(const VARIANT &v, char *s, int charlen)
{
	if( v.vt == VT_EMPTY )
	{
		ASSERT( charlen == 0 );
		return;
	}
	
	ASSERT( v.vt == VT_BSTR );
	CopyTo(v.bstrVal, s, charlen);
}

long GetArrayLength(const VARIANT &v)
{
	if( (v.vt & VT_ARRAY) == VT_ARRAY )
		return GetArrayLength(v.parray);
	else if( v.vt == VT_EMPTY )
		return 0;

	HR_THROW(E_INVALIDARG);
}

void GetArrayStart(const VARIANT &v, CComBstrObj *&start)
{
	if( (v.vt & VT_ARRAY) != VT_ARRAY )
		HR_THROW(E_INVALIDARG);

	GetArrayStart(v.parray, start);
}

void GetArrayStart(const VARIANT &v, unsigned char *&start)
{
	if( (v.vt & VT_ARRAY) != VT_ARRAY )
		HR_THROW(E_INVALIDARG);

	GetArrayStart(v.parray, start);
}

void GetArrayStart(const VARIANT &v, long *&start)
{
	if( (v.vt & VT_ARRAY) != VT_ARRAY )
		HR_THROW(E_INVALIDARG);

	GetArrayStart(v.parray, start);
}

void CopyTo(const CComBstrObj *start, const CComBstrObj *end, VARIANT *v)
{
	ASSERT( v != NULL );

	if( v->vt != VT_EMPTY )
		COMTHROW( VariantClear(v) );

	v->parray = NULL;
	CopyTo(start, end, &v->parray);
	v->vt = VT_BSTR | VT_ARRAY;
}

void MoveTo(CComBstrObj *start, CComBstrObj *end, VARIANT *v)
{
	ASSERT( v != NULL );

	if( v->vt != VT_EMPTY )
		COMTHROW( VariantClear(v) );

	v->parray = NULL;
	MoveTo(start, end, &v->parray);
	v->vt = VT_BSTR | VT_ARRAY;
}

void CopyTo(const unsigned char *start, const unsigned char *end, VARIANT *v)
{
	ASSERT( v != NULL );

	if( v->vt != VT_EMPTY )
		COMTHROW( VariantClear(v) );

	v->parray = NULL;
	CopyTo(start, end, &v->parray);
	v->vt = VT_UI1 | VT_ARRAY;
}

void CopyTo(const long *start, const long *end, VARIANT *v)
{
	ASSERT( v != NULL );

	if( v->vt != VT_EMPTY )
		COMTHROW( VariantClear(v) );

	v->parray = NULL;
	CopyTo(start, end, &v->parray);
	v->vt = VT_I4 | VT_ARRAY;
}

void CopyTo(const VARIANT &v, CComBstrObj *start, CComBstrObj *end)
{
	ASSERT( start <= end );

	if( v.vt == VT_EMPTY && start == end )
		return;

	if( v.vt != (VT_BSTR | VT_ARRAY) )
		HR_THROW(E_INVALIDARG);

	CopyTo(v.parray, start, end);
}

void MoveTo(VARIANT *v, CComBstrObj *start, CComBstrObj *end)
{
	ASSERT( v != NULL );
	ASSERT( start <= end );

	if( v->vt == VT_EMPTY && start == end )
		return;

	if( v->vt != (VT_BSTR | VT_ARRAY) )
		HR_THROW(E_INVALIDARG);

	MoveTo(v->parray, start, end);
	COMTHROW( VariantClear(v) );
}

void CopyTo(const VARIANT &v, unsigned char *start, unsigned char *end)
{
	ASSERT( start <= end );

	if( v.vt == VT_EMPTY && start == end )
		return;

	if( v.vt != (VT_UI1 | VT_ARRAY) )
		HR_THROW(E_INVALIDARG);

	CopyTo(v.parray, start, end);
}

void CopyTo(const VARIANT &v, long *start, long *end)
{
	ASSERT( start <= end );

	if( v.vt == VT_EMPTY && start == end )
		return;

	if( v.vt != (VT_I4 | VT_ARRAY) )
		HR_THROW(E_INVALIDARG);

	CopyTo(v.parray, start, end);
}

void CopyTo(const VARIANT &v, GUID &guid)
{
	if( v.vt != (VT_UI1 | VT_ARRAY) || GetArrayLength(v) != sizeof(GUID) )
		HR_THROW(E_CONVERSION);
	
	CopyTo(v, (unsigned char*)&guid, (unsigned char*)(&guid+1));
}

// --------------------------- COM Helper functions

bool IsEqualObject(IUnknown *p, IUnknown *q)
{
	if( p == q )
		return true;
	else if( p == NULL || q == NULL )
		return false;

	CComObjPtr<IUnknown> a;
	CComObjPtr<IUnknown> b;
	COMTHROW( p->QueryInterface(IID_IUnknown, (void**)PutOut(a)) );
	COMTHROW( q->QueryInterface(IID_IUnknown, (void**)PutOut(b)) );

	return a == b;
}

