#include "stdafx.h"
#include "UdmConsole.h"

namespace GMEConsole
{
	CComBSTR BSTRFromUTF8(const std::string& utf8)
	{
		if (utf8.empty())
			return CComBSTR();

		// Fail if an invalid input character is encountered
		const DWORD conversionFlags = MB_ERR_INVALID_CHARS;

		const int utf16Length = ::MultiByteToWideChar(CP_UTF8, conversionFlags, utf8.data(), utf8.length(), NULL, 0);
		if (utf16Length == 0)
		{
			DWORD error = ::GetLastError();

			throw python_error(
				(error == ERROR_NO_UNICODE_TRANSLATION) ? 
					"Invalid UTF-8 sequence found in input string." :
					"Can't get length of UTF-16 string (MultiByteToWideChar failed).");
		}

		BSTR utf16 = SysAllocStringByteLen(NULL, utf16Length*2);
		if (utf16 == NULL)
			throw std::bad_alloc();

		if (!::MultiByteToWideChar(CP_UTF8, 0, utf8.data(), utf8.length(), utf16, utf16Length))
		{
			DWORD error = ::GetLastError();
			SysFreeString(utf16);
			throw python_error("Can't convert string from UTF-8 to UTF-16 (MultiByteToWideChar failed).");
		}

		CComBSTR ret;
		ret.m_str = utf16;
		return ret;
	}

	thread_local CComPtr<IGMEOLEApp> Console::gmeoleapp=0;

	void Console::setupConsole(CComPtr<IMgaProject> project)
	{
		CComPtr<IMgaClient> client;	
		CComQIPtr<IDispatch> pDispatch;
		HRESULT s1 = project->GetClientByName(CComBSTR(L"GME.Application"),&client);

		if ((SUCCEEDED(s1)) && (client != 0))
		{
			HRESULT s2 = client->get_OLEServer(&pDispatch);
			if ((SUCCEEDED(s2)) && (pDispatch != 0))
			{
				// release here in case setupConsole was called twice
				gmeoleapp.Release();
				HRESULT s3 = pDispatch->QueryInterface(&gmeoleapp);
				if ((SUCCEEDED(s3)) && (gmeoleapp != 0))
				{
					// gmeoleapp->ConsoleClear();
					// gmeoleapp->put_ConsoleContents(NULL);
				}
				else
				{
					throw python_error("GME Console could not be accessed.");
				}
			}
		}
	}
}