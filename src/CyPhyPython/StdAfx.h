// stdafx.h : include file for standard system include files,
//      or project specific include files that are used frequently,
//      but are changed infrequently

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Exclude rarely-used stuff from Windows headers
#endif

#include "targetver.h"

#define _CRT_SECURE_NO_WARNINGS

#define _ATL_APARTMENT_THREADED

#define GME_COMPONENT_ISUPPORTERRORINFO

#include <afxwin.h>
#include <afxdisp.h>

#include <atlbase.h>
//You may derive a class from CComModule and use it if you want to override
//something, but do not change the name of _Module
extern CComModule _Module;
#include <atlcom.h>

#include <string>

template<typename char_type>
std::wstring html_encode(const std::basic_string<char_type> & data) {
	std::wstring buffer;
	buffer.reserve(data.size());
	for (size_t pos = 0; pos != data.size(); ++pos) {
		switch (data[pos]) {
		case '&':  buffer.append(L"&amp;");       break;
		case '\"': buffer.append(L"&quot;");      break;
		case '\'': buffer.append(L"&apos;");      break;
		case '<':  buffer.append(L"&lt;");        break;
		case '>':  buffer.append(L"&gt;");        break;
		case '\n': buffer.append(L"<br/>");       break;
		default:   buffer.append((size_t)1, static_cast<char_type>(data[pos]));  break;
		}
	}
	return buffer;
}
