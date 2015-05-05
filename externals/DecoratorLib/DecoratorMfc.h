
#ifndef MGA_COMMONMFC_H
#define MGA_COMMONMFC_H

#ifndef MGA_COMMONSMART_H
#include "DecoratorSmart.h"
#endif

#include "afxtempl.h"

// --------------------------- CString

inline void CopyTo(const CStringA &s, BSTR *b) { CopyTo(s, s.GetLength(), b); }
inline void CopyTo(const CStringA &s, VARIANT *v) { CopyTo(s, s.GetLength(), v); }
inline void CopyTo(const CStringA &s, CComBstrObj &a) { CopyTo(s, s.GetLength(), a); }
inline void CopyTo(const CStringW &s, CComBstrObj &a) { a = CComBstrObj(static_cast<const wchar_t*>(s)); }
inline void CopyTo(const CStringW &s, CComBSTR &a) { a = static_cast<const wchar_t*>(s); }
inline void CopyTo(const CComBstrObj &s, CStringW &a) { a = static_cast<const wchar_t*>(s); }
inline void CopyTo(const CStringA &s, CComVariant &a) { CopyTo(s, s.GetLength(), a); }

inline void CopyTo(BSTR b, CStringA &s)
{
	int len = GetCharLength(b);
	CopyTo(b, s.GetBufferSetLength(len), len);
	s.ReleaseBuffer(len);
}

inline void CopyTo(VARIANT &v, CStringA &s)
{
	int len = GetCharLength(v);
	CopyTo(v, s.GetBufferSetLength(len), len);
	s.ReleaseBuffer(len);
}

inline void CopyTo(const CStringA &s, CComBSTR &b) { CopyTo(s, &b); }

template<class char_t>
class PutOutCString
{
	typedef ATL::CStringT< char_t, StrTraitMFC_DLL< char_t > > CString_t;
private:
	friend PutOutCString PutOut(CString_t &s);

	PutOutCString(CString_t &a) : s(a)
	{
		ASSERT( &s != NULL ); 
	}

public:
	operator BSTR *() { return PutOut(b); }
	
	~PutOutCString()
	{
		CopyTo(b, s);
	}

private:
	// to prevent misuse
	PutOutCString();
	PutOutCString(const PutOutCString &a);

private:
	CComBstrObj b;
	CString_t &s;
};

inline PutOutCString<char> PutOut(CStringA &s)
{
	return PutOutCString<char>(s);
}

inline PutOutCString<wchar_t> PutOut(CStringW &s)
{
	return PutOutCString<wchar_t>(s);
}

class PutInCString
{
public:
	template<class T>
	PutInCString(T t) { CopyTo(t, b); }

	operator const CStringA &() { return b; }
	operator const char *() { return b; }

	CStringA b;
};

// --------------------------- CComObjPtrArray

template<class ELEM>
class CComObjPtrList : public CList< CComObjPtr<ELEM>, CComObjPtr<ELEM> >
{ };

template<class ELEM>
class CComObjPtrArray : public CArray< CComObjPtr<ELEM>, CComObjPtr<ELEM> >
{ };

template<class COLL, class ELEM>
void CopyTo(COLL *coll, CComObjPtrArray<ELEM> &array)
{
	ASSERT( coll != NULL );
	ASSERT( array.GetSize() == 0 );

	long count = 0;
	COMTHROW( coll->get_Count(&count) );
	ASSERT( count >= 0 );

	array.SetSize(count);

	COMTHROW( coll->GetAll(count, (ELEM**)array.GetData()) );
}

template<class COLL, class ELEM>
void CopyTo(const CComObjPtr<COLL> &coll, CComObjPtrArray<ELEM> &array) 
{
	CopyTo((COLL*)coll, array); 
}

template<class COLL, class ELEM>
void CopyTo(const CComPtr<COLL> &coll, CComObjPtrArray<ELEM> &array) 
{
	CopyTo((COLL*)coll, array); 
}

template<class COLL, class ELEM>
class PutOutArrayObj
{
public:
	PutOutArrayObj(CComObjPtrArray<ELEM> &a) : array(a) { array.RemoveAll(); }
	~PutOutArrayObj() { ASSERT( coll != NULL); CopyTo(coll, array); }

	operator COLL **() { return PutOut(coll); }

	CComObjPtrArray<ELEM> &array;
	CComObjPtr<COLL> coll;
};

template<class COLL, class ELEM>
PutOutArrayObj<COLL, ELEM> PutOut(CComObjPtrArray<ELEM> &coll)
{
	return coll;
}

// --------------------------- CArray<CComBstrObj, CComBstrObj&>

typedef CArray<CComBstrObj, CComBstrObj&> CComBstrObjArray;

inline void MoveTo(VARIANT *p, CComBstrObjArray &array)
{
	ASSERT( array.GetSize() == 0 );

	array.SetSize(GetArrayLength(*p));
	MoveTo(p, array.GetData(), array.GetData() + array.GetSize());
}

inline void MoveTo(CComVariant &v, CComBstrObjArray &array) { MoveTo(&v, array); }

inline void CopyTo(const VARIANT &p, CComBstrObjArray &array)
{
	ASSERT( array.GetSize() == 0 );

	array.SetSize(GetArrayLength(p));
	CopyTo(p, array.GetData(), array.GetData() + array.GetSize());
}

inline void MoveTo(CComBstrObjArray &array, VARIANT *p)
{
	ASSERT( p != NULL );

	MoveTo(array.GetData(), array.GetData() + array.GetSize(), p);
}

inline void CopyTo(const CComBstrObjArray &array, VARIANT *p)
{
	ASSERT( p != NULL );

	CopyTo(array.GetData(), array.GetData() + array.GetSize(), p);
}

// --------------------------- CStringArray

inline void CopyTo(VARIANT p, CStringArray &array)
{
	ASSERT( array.GetSize() == 0 );

	CComBstrObjArray a;
	CopyTo(p, a);

	array.SetSize( a.GetSize() );

	for(int i = 0; i < a.GetSize(); ++i)
		CopyTo(a[i], array[i]);
}

inline void CopyTo(const CStringArray &array, VARIANT *p)
{
	CComBstrObjArray a;
	a.SetSize( array.GetSize() );

	for(int i = 0; i < array.GetSize(); ++i)
		CopyTo(array[i], a[i]);

	CopyTo(a, p);
}

// --------------------------- Error

void DisplayError(const char *msg, HRESULT hr) NOTHROW;

#define MSGTRY try

#define MSGCATCH(MSG,CLEANUP) \
	catch(hresult_exception &e) \
	{ \
		{ CLEANUP; } \
		DisplayError(MSG, e.hr); \
	}

#endif//MGA_COMMONMFC_H
