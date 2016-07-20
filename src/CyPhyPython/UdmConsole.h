#pragma once
#include "stdafx.h"
#include "ComHelp.h"
#include "GMECOM.h"
#include <string>
#include "GMEVersion.h"
#include "UdmApp.h"

#if GME_VERSION_MAJOR >= 11
#include "Gme.h"
#endif

namespace GMEConsole
{
	CComBSTR BSTRFromUTF8(const std::string & utf8);

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
		static void setupConsole(CComPtr<IMgaProject> project);
		static void freeConsole()
		{
			if (gmeoleapp != 0)
			{
				gmeoleapp.Release();
				gmeoleapp = 0;
			}
		}
		static void SetupConsole(CComPtr<IMgaProject> project) { setupConsole(project); }

		static void writeLine(const std::string& message, msgtype_enum type)
		{
			if (gmeoleapp == 0) {
				switch (type) {
				case MSG_NORMAL:
				case MSG_INFO:
				case MSG_WARNING:
					printf("%s", message.c_str());
					break;
				case MSG_ERROR:
					fprintf(stderr, "%s", message.c_str());
					break;
				}
			} else {
				if(S_OK != gmeoleapp->ConsoleMessage(CComBSTR(message.length(), message.c_str()), type))
					throw python_error("Could not write to GME console.");
			}
		}

		static void writeLine(const std::wstring& message, msgtype_enum type)
		{
			if (gmeoleapp == 0) {
				switch (type) {
				case MSG_NORMAL:
				case MSG_INFO:
				case MSG_WARNING:
					wprintf(L"%s", message.c_str());
					break;
				case MSG_ERROR:
					fwprintf(stderr, L"%s", message.c_str());
					break;
				}
			} else {
				if(S_OK != gmeoleapp->ConsoleMessage(CComBSTR(message.length(), message.c_str()), type))
					throw python_error("Could not write to GME console.");
			}
		}

		static void clear()
		{
			if(gmeoleapp!=0)
				gmeoleapp->put_ConsoleContents(NULL);
		}

		static void setContents(const std::string& contents)
		{
			if (gmeoleapp != 0)
				if(S_OK != gmeoleapp->put_ConsoleContents( CComBSTR(contents.length(),contents.c_str()) ))
					throw python_error("Could not set the contents of GME console.");
		}
		static void setContents(const std::wstring& contents)
		{
			if (gmeoleapp != 0)
				if(S_OK != gmeoleapp->put_ConsoleContents( CComBSTR(contents.length(),contents.c_str()) ))
					throw python_error("Could not set the contents of GME console.");
		}

		class Error
		{
		public:
			static void writeLine(const std::string& message)
			{
				Console::writeLine(message,MSG_ERROR);
			}
			static void writeLine(const std::wstring& message)
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
			static void writeLine(const std::wstring& message)
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
			static void writeLine(const std::wstring& message)
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
			static void writeLine(const std::wstring& message)
			{
				Console::writeLine(message,MSG_INFO);
			}
		};
	};
}