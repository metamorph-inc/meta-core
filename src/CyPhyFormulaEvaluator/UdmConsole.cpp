#include "stdafx.h"
#include "Gme.h"
#include "UdmConsole.h"
#include "UdmBase.h"


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

            throw udm_exception(
                (error == ERROR_NO_UNICODE_TRANSLATION) ?
                "Invalid UTF-8 sequence found in input string." :
                "Can't get length of UTF-16 string (MultiByteToWideChar failed).");
        }

        BSTR utf16 = SysAllocStringByteLen(NULL, utf16Length * 2);
        if (utf16 == NULL)
            throw std::bad_alloc();

        if (!::MultiByteToWideChar(CP_UTF8, 0, utf8.data(), utf8.length(), utf16, utf16Length))
        {
            DWORD error = ::GetLastError();
            SysFreeString(utf16);
            throw udm_exception("Can't convert string from UTF-8 to UTF-16 (MultiByteToWideChar failed).");
        }

        CComBSTR ret;
        ret.m_str = utf16;
        return ret;
    }

    CComPtr<IGMEOLEApp> Console::gmeoleapp = 0;
    void Console::SetupConsole(CComPtr<IMgaProject> project)
    {
        setupConsole(project);
    }

    void Console::setupConsole(CComPtr<IMgaProject> project)
    {
        CComPtr<IMgaClient> client;
        CComQIPtr<IDispatch> pDispatch;
        HRESULT s1 = project->GetClientByName(CComBSTR(L"GME.Application"), &client);

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
                    throw udm_exception("GME Console could not be accessed.");
                }
            }
        }
    }

    void Console::writeLine(const std::string& message, msgtype_enum type)
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
        }
        else {
            if (S_OK != gmeoleapp->ConsoleMessage(CComBSTR((int)message.length(), message.c_str()), type))
                throw udm_exception("Could not write to GME console.");
        }
    }

    void Console::writeLine(const std::wstring& message, msgtype_enum type)
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
        }
        else {
            if (S_OK != gmeoleapp->ConsoleMessage(CComBSTR((int)message.length(), message.c_str()), type))
                throw udm_exception("Could not write to GME console.");
        }
    }

    void Console::setContents(const std::string& contents)
    {
        if (gmeoleapp != 0)
            if (S_OK != gmeoleapp->put_ConsoleContents(CComBSTR((int)contents.length(), contents.c_str())))
                throw udm_exception("Could not set the contents of GME console.");
    }
    void Console::setContents(const std::wstring& contents)
    {
        if (gmeoleapp != 0)
            if (S_OK != gmeoleapp->put_ConsoleContents(CComBSTR((int)contents.length(), contents.c_str())))
                throw udm_exception("Could not set the contents of GME console.");
    }

}