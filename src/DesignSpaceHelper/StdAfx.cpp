// stdafx.cpp : source file that includes just the standard includes
//  stdafx.pch will be the pre-compiled header
//  stdafx.obj will contain the pre-compiled type information

#include "StdAfx.h"

#ifdef _ATL_STATIC_REGISTRY
#include <statreg.h>
#endif

CString utf82cstring(const char* input, int length)
{
    if (length == 0) {
        return CString();
    }

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


CString utf82cstring(LPCSTR utf8) {
    return utf82cstring(utf8, strlen(utf8));
}

CString utf82cstring(const std::string& utf8) {
	return utf82cstring(utf8.c_str(), utf8.length());
}

std::string tstring2utf8(const wchar_t* input, int length) {
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

std::string tstring2utf8(const std::wstring& input) {
	return tstring2utf8(input.c_str(), input.length());
}

std::string tstring2utf8(const CString& input) {
	return tstring2utf8(static_cast<const wchar_t*>(input), input.GetLength());
}

std::string tstring2utf8(wchar_t const *input) {
	return tstring2utf8(input, wcslen(input));
}
