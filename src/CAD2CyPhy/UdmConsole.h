#pragma once

#include "stdafx.h"
#include "ComHelp.h"
#include "GMECOM.h"
#include "UdmBase.h"
#include <string>
#include "GMEVersion.h"

#if GME_VERSION_MAJOR >= 11
#include "Gme.h"
#endif

namespace GMEConsole
{
	class Console
	{
	friend class RawComponent;

#if GME_VERSION_MAJOR <= 10 && GME_VERSION_MINOR <= 2
	private:
#else
	public:
#endif
		static CComPtr<IGMEOLEApp> gmeoleapp;
	public:
		static void setupConsole(CComPtr<IMgaProject> project)
		{
			freeConsole();
			CComBSTR bstrName("GME.Application");
			CComPtr<IMgaClient> pClient;
			HRESULT hr = project->GetClientByName(bstrName, &pClient);
			if (SUCCEEDED(hr) && pClient) 
			{
				CComPtr<IDispatch> pDispatch;
				hr = pClient->get_OLEServer(&pDispatch);
				if (SUCCEEDED(hr) && pDispatch) 
				{
					hr = pDispatch.QueryInterface(&gmeoleapp);
					if (FAILED(hr)) 
					{
						freeConsole();
					}
				}
			}
		}
		static void freeConsole()
		{
			if(gmeoleapp!=0)
			{
				gmeoleapp.Release();
				gmeoleapp=0;
			}
		}
	public:

		static void writeLine(const std::string& message, msgtype_enum type)
		{
			if(gmeoleapp!=0)
				if(S_OK != gmeoleapp->ConsoleMessage( CComBSTR(message.length(),message.c_str()),type))
					throw udm_exception("Could not write GME console.");
		}


		static void clear()
		{
			/* Does not work:
			if(S_OK != gmeoleapp->ConsoleClear())
				throw udm_exception("Could not clear GME console.");
				*/
			if(gmeoleapp!=0)
				gmeoleapp->put_ConsoleContents(L"");
		}

		static void setContents(const std::string& contents)
		{
			if(S_OK != gmeoleapp->put_ConsoleContents( CComBSTR(contents.length(),contents.c_str()) ))
				throw udm_exception("Could not set the contents of GME console.");

		}

		class Error
		{
		public:
			static void writeLine(const std::string& message)
			{
				Console::writeLine(message,MSG_ERROR);
			}
		};
		class Out
		{
		public:
			static void writeLine(const std::string& message)
			{
				Console::writeLine(message, MSG_NORMAL);
			}
		};
		class Warning
		{
		public:
			static void writeLine(const std::string& message)
			{
				Console::writeLine(message, MSG_WARNING);
			}
		};
		class Info
		{
		public:
			static void writeLine(const std::string& message)
			{
				Console::writeLine(message,MSG_INFO);
			}
		};
	};
}