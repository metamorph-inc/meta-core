// NEW GME related stuff
#ifndef DECORCOM_H
#define DECORCOM_H

#include <exception>
// --------------------------- hresult_exception

class hresult_exception : public exception
{
public:
	hresult_exception() throw();
	hresult_exception(const hresult_exception &e) throw();
	hresult_exception(HRESULT hr) throw();
	hresult_exception &operator=(const hresult_exception &e) throw();
	hresult_exception &operator=(HRESULT hr) throw();
	~hresult_exception() throw();
	virtual const char *what() const throw();

public:
	HRESULT hr;
};

inline hresult_exception::hresult_exception()
{
	hr = 0;
}

inline hresult_exception::hresult_exception(const hresult_exception &e)
{
	hr = e.hr;
}

inline hresult_exception::hresult_exception(HRESULT a)
{
	hr = a;
}

inline hresult_exception &hresult_exception::operator=(const hresult_exception &e)
{
	hr = e.hr;
	return *this;
}

inline hresult_exception &hresult_exception::operator=(HRESULT a)
{
	hr = a;
	return *this;
}

inline hresult_exception::~hresult_exception()
{
}

inline const char *hresult_exception::what() const
{
	static char message[80];
	sprintf(message, "HRESULT (0x%08lx) exception", hr);
	return message;
}


// --------------------------- Exceptions and COM
#ifdef _DEBUG
#define COMTHROW(FUNC) \
do { \
	HRESULT _hr = (FUNC); \
	if( FAILED(_hr) ) {\
		ASSERT(("COMTHROW: Throwing HRESULT exception. Press IGNORE", false)); \
		throw hresult_exception(_hr);  \
	}\
} while(false)
#else
#define COMTHROW(FUNC) \
do { \
	HRESULT _hr = (FUNC); \
	if( FAILED(_hr) ) \
		throw hresult_exception(_hr); \
} while(false)
#endif


inline void COMVERIFY(HRESULT hr)
{
	VERIFY(SUCCEEDED(hr));
}

inline void COMASSERT(HRESULT hr)
{
	ASSERT(SUCCEEDED(hr));
}

template <class TTT>
class COMArray {
public:
	CComPtr<TTT> *array;
	COMArray(long len)  { array = new CComPtr<TTT>[len]; };
	~COMArray() { delete[] array; };
};


#define COM_EQUAL(a, b) (static_cast<IUnknown *>(a) == b ? true : a.IsEqualObject(b))

#define MGACOLL_ITERATE(iftype, collifptr) \
{ \
	ASSERT( collifptr != NULL ); \
	long iter_count = 0; \
	COMTHROW( collifptr->get_Count(&iter_count) ); \
	ASSERT( iter_count >= 0 ); \
	CComPtr<iftype> *arrptr, *arrend, *array = new CComPtr<iftype>[iter_count]; \
	if(iter_count > 0) \
		COMTHROW( collifptr->GetAll(iter_count, &(*array)) ); \
	arrend = array+iter_count; \
	for(arrptr = array; arrptr != arrend; arrptr++)

#define MGACOLL_ITER (*arrptr)
#define MGACOLL_AT_END (arrptr == arrend)

#define MGACOLL_ITERATE_END delete []array; }



#endif // DECORCOM_H
