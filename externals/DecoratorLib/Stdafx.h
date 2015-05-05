// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#if !defined(AFX_STDAFX_H__73C5F260_656B_4130_A53C_F6EF8BCA184A__INCLUDED_)
#define AFX_STDAFX_H__73C5F260_656B_4130_A53C_F6EF8BCA184A__INCLUDED_

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Exclude rarely-used stuff from Windows headers
#endif

#include "targetver.h"

#define _CRT_SECURE_NO_WARNINGS

//#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit

// turns off MFC's hiding of some common and often safely ignored warning messages
#define _AFX_ALL_WARNINGS


#define _ATL_APARTMENT_THREADED

#include <afxwin.h>
#include <afxdisp.h>
#include <afxdlgs.h>
#include <afxcmn.h>
#include <afxcoll.h>
#include <afxtempl.h>

#include <gdiplus.h>
//using namespace Gdiplus;

#include <atlbase.h>
//You may derive a class from CComModule and use it if you want to override
//something, but do not change the name of _Module
extern CComModule _Module;
#include <atlcom.h>

#include "DecoratorError.h"
#include "DecoratorSmart.h"
#include "DecoratorMfc.h"
#include <stdio.h>

#ifdef UNICODE
#define GetLogFontT GetLogFontW
#else
#define GetLogFontT GetLogFontA
#endif

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.


#endif // !defined(AFX_STDAFX_H__73C5F260_656B_4130_A53C_F6EF8BCA184A__INCLUDED_)


