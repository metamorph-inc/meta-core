// -*-C++-*-
// utils.cpp
// Implementation of utility functions

#include "core/utils.h"



int
log2(int val)
{
  int ret = 0;
  val--;
  while(val /= 2)
    ret++;

  return ret+1;
}

int
compareInt( const void *arg1, const void *arg2)
{
  int val1 = *(int *)arg1;
  int val2 = *(int *)arg2;

  if (val1 < val2) return -1;
  if (val1 > val2) return 1;

  return 0;
}

int *
toVector(int val, int len, int *vec)
{
  int i=len-1;
  for(int n=0; n<len; n++, i--, val=val>>1)
    vec[i] = val & 0x1;

  return vec;
}

int
toInteger(int *vec, int len)
{
  int ret = 0;
  for (int i=0; i<len; i++)
  {
    ret = ret << 1;
    ret |= vec[i];
  }
  return ret;
}

void
RemoveCRLF(CString& text)
{
  int idx;
  while( (idx = text.Find('\n')) >= 0 )
    text.SetAt(idx, ' '); 
  while( (idx = text.Find('\r')) >= 0 )
    text.SetAt(idx, ' '); 
}



#include "common/error.h"

//error handling with exceptions


IMPLEMENT_DYNAMIC(CDesertException, CException)

inline CDesertException::CDesertException()						{fatal = true;		what = NULL;};			
inline CDesertException::CDesertException(bool _fatal)			{fatal = _fatal;	what = NULL;};
inline CDesertException::CDesertException(const TCHAR * _what)	{fatal = true;		if (_what) what = new TCHAR[_tcslen(_what)+1];_tcscpy(what, _what);};
inline CDesertException::CDesertException(const TCHAR * _what, const TCHAR * _name)	
{	
	fatal = true;		
	if (_what) 
	{
		what = new TCHAR[_tcslen(_what)+1];
		_tcscpy(what, _what); 
	}
	if(_name) 
	{
		constraint_name=new TCHAR[_tcslen(_name)+1];
		_tcscpy(constraint_name, _name);
	}
};

inline CDesertException::CDesertException(const CString _what)	{fatal = true;		if (_what.GetLength()) what = new TCHAR[_what.GetLength()+1];_tcscpy(what, (LPCTSTR)_what);};
inline CDesertException::CDesertException(const CString _what, const CString _name)	
{	
	fatal = true;		
	if (_what.GetLength()) 
	{
		what = new TCHAR[_what.GetLength()+1];
		_tcscpy(what, (LPCTSTR)_what);
	}
	if(_name) 
	{
		constraint_name=new TCHAR[_name.GetLength()+1];
		_tcscpy(constraint_name, (LPCTSTR)_name);
	}
};
inline CDesertException::CDesertException(bool _fatal, const TCHAR * _what)	{fatal = _fatal;	if (_what) what = new TCHAR[_tcslen(_what)+1];_tcscpy(what, _what);};
inline CDesertException::CDesertException(bool _fatal, const CString _what)	{fatal = _fatal;	if (_what.GetLength()) what = new TCHAR[_what.GetLength()+1];_tcscpy(what, (LPCTSTR)_what);};

inline BOOL CDesertException::GetErrorMessage( LPTSTR lpszError, UINT nMaxError, PUINT pnHelpContext)	
{
	if (what) 
	{
		_tcsncpy(lpszError, what, nMaxError);
		return true;
	}
	return false;
};

inline CString CDesertException::GetErrorMessage()	
{
	if (what) 
	{
		return CString(what);
	}
	return CString("");
};

inline CString CDesertException::GetConstraintName()
{
	if (constraint_name) 
	{
		return CString(constraint_name);
	}
	return CString("");
};

inline bool CDesertException::Fatal()							{return fatal;};	
inline CDesertException::~CDesertException()					{if (what) delete[] what;};						

IMPLEMENT_DYNAMIC(CDesertNoneElementException, CDesertException)
inline  CDesertNoneElementException::CDesertNoneElementException()						{fatal = true;		what = NULL;};	
inline CDesertNoneElementException::CDesertNoneElementException(const TCHAR * _what)	{fatal = true;		if (_what) what = new TCHAR[_tcslen(_what)+1];_tcscpy(what, _what);_tcscpy(element,_what);};



