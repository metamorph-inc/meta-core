// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//
#pragma once

#include <fstream>
#include <string>
using namespace std;

#ifdef _UNICODE
typedef std::wstring tstring;
typedef std::wstringstream tstringstream;
#define PRINTF_TSTRING "%S"
#else
typedef std::string tstring;
typedef std::stringstream tstringstream;
#define PRINTF_TSTRING "%s"
#endif

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#include "DesertIface.h"

#include "DesertIfaceBack.h"

#include "../desertdll/desertdll.h"

// Udm uses utf-8 strings
#ifdef _UNICODE
typedef std::wstring tstring;
static CString utf82cstring(const char* input, int length)
{
	if (length == 0)
		return CString();

	// Fail if an invalid input character is encountered
	const DWORD conversionFlags = MB_ERR_INVALID_CHARS;

	const int utf16Length = ::MultiByteToWideChar(CP_UTF8, conversionFlags, input, length, NULL, 0);
	if (utf16Length == 0)
	{
		DWORD error = ::GetLastError();

		return CString();
		//    (error == ERROR_NO_UNICODE_TRANSLATION) ? 
		//        "Invalid UTF-8 sequence found in input string." :
		//        "Can't get length of UTF-16 string (MultiByteToWideChar failed).");
	}

	CString ret;
	wchar_t* buf = ret.GetBuffer(utf16Length);

	if (!::MultiByteToWideChar(CP_UTF8, 0, input, length, buf, utf16Length))
	{
		DWORD error = ::GetLastError();
		ret.ReleaseBuffer(0);
		return ret;
	}

	ret.ReleaseBuffer(utf16Length);
	return ret;
}


static CString utf82cstring(LPCSTR utf8) {
	return utf82cstring(utf8, strlen(utf8));
}

static CString utf82cstring(const std::string& utf8) {
	return utf82cstring(utf8.c_str(), utf8.length());
}

static std::string tstring2utf8(const wchar_t* input, int length) {
	if (length == 0)
		return std::string();

	const int utf8Length = ::WideCharToMultiByte(CP_UTF8, 0, input, length, NULL, 0, NULL, NULL);
	if (utf8Length == 0)
	{
		DWORD error = ::GetLastError();

		return std::string();
		//    (error == ERROR_NO_UNICODE_TRANSLATION) ? 
		//        "Invalid UTF-8 sequence found in input string." :
		//        "Can't get length of UTF-16 string (MultiByteToWideChar failed).");
	}

	char* buf = (char*)malloc(utf8Length);

	if (!::WideCharToMultiByte(CP_UTF8, 0, input, length, buf, utf8Length, NULL, NULL))
	{
		DWORD error = ::GetLastError();
		free(buf);
		return std::string();
	}

	std::string ret(buf, utf8Length);
	free(buf);
	return ret;
}

static std::string tstring2utf8(const std::wstring& input) {
	return tstring2utf8(input.c_str(), input.length());
}

static std::string tstring2utf8(const CString& input) {
	return tstring2utf8(static_cast<const wchar_t*>(input), input.GetLength());
}

#else
typedef std::string tstring;
static CString utf82cstring(LPCSTR utf8) {
	return CString(utf8);
}

static CString utf82cstring(const std::string& utf8) {
	return CString(utf8.c_str());
}
static std::string tstring2utf8(const tstring& input) {
	return std::string(static_cast<const char*>(input.c_str()));
}
static std::string tstring2utf8(const CString& input) {
	return std::string(static_cast<const char*>(input));
}
#endif
