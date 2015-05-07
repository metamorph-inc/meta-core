#pragma once
#include "stdafx.h"
#include "ComHelp.h"
#include "GMECOM.h"
#include "UdmBase.h"
#include "UdmFormatter.h"
#include <string>

#include "GmeVersion.h"
#if defined(GME_VERSION_MAJOR) && GME_VERSION_MAJOR >= 11
#include "Gme.h"
#endif

namespace GMEConsole
{
	class Console
	{
	friend class RawComponent;

	public:
		static void SetupConsole(CComPtr<IMgaProject> project); 
		static CComPtr<IGMEOLEApp> gmeoleapp;

		static void writeLine(const std::string& message, msgtype_enum type)
		{
			if(S_OK != gmeoleapp->ConsoleMessage( CComBSTR(message.length(),message.c_str()),type))
				throw udm_exception("Could not write GME console.");
		}

		static void clear()
		{
			/* Does not work:
			if(S_OK != gmeoleapp->ConsoleClear())
				throw udm_exception("Could not clear GME console.");
				*/
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