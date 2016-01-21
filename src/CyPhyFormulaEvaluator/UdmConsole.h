#pragma once
#include "stdafx.h"
#include "ComHelp.h"
#include "GMECOM.h"
#include "UdmBase.h"
#include <string>
#include "GMEVersion.h"
#include "Gme.h"

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
        static void SetupConsole(CComPtr<IMgaProject> project);

        static void writeLine(const std::string& message, msgtype_enum type);

        static void writeLine(const std::wstring& message, msgtype_enum type);

        static void clear()
        {
            if (gmeoleapp != 0)
                gmeoleapp->put_ConsoleContents(NULL);
        }

        static void setContents(const std::string& contents);
        static void setContents(const std::wstring& contents);

        class Error
        {
        public:
            static void writeLine(const std::string& message)
            {
                Console::writeLine(message, MSG_ERROR);
            }
            static void writeLine(const std::wstring& message)
            {
                Console::writeLine(message, MSG_ERROR);
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
                Console::writeLine(message, MSG_INFO);
            }
            static void writeLine(const std::wstring& message)
            {
                Console::writeLine(message, MSG_INFO);
            }
        };
    };
}