// stdafx.h : include file for standard system include files,
//      or project specific include files that are used frequently,
//      but are changed infrequently

#pragma once

#define STRICT
#define WINVER 0x0502
#define _WIN32_WINNT 0x0502
#define _WIN32_IE 0x0500
#define _ATL_APARTMENT_THREADED

#define _CRT_SECURE_NO_WARNINGS

#define GME_COMPONENT_ISUPPORTERRORINFO 1

#include <afxwin.h>
#include <afxdisp.h>

#include <atlbase.h>
//You may derive a class from CComModule and use it if you want to override
//something, but do not change the name of _Module
extern CComModule _Module;
#include <atlcom.h>
#include <afxdhtml.h>
#include <afxcontrolbars.h>

#pragma warning( disable : 4666)

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#include <string>
#include <vector>
#include <map>
#include <set>
#include <UdmBase.h>
#include <Uml.h>
#include <UmlExt.h>
#include "CyPhyML.h"
#include "BackIface.h"

typedef std::wstring tstring;
typedef std::wstringstream tstringstream;

CString utf82cstring(LPCSTR utf8);
CString utf82cstring(const std::string& utf8);
std::string tstring2utf8(const std::wstring& input);
std::string tstring2utf8(const CString& input);
std::string tstring2utf8(const wchar_t* input);

