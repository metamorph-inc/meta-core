// cl exec_unelevated.cpp

// Not exactly unelevated: the Integrety Level is High. But execed programs can't write to HKLM, so it is good enough

#define STRICT
#define UNICODE
#define _UNICODE

#include <Windows.h>
#include <WinSafer.h>

#include <stdio.h>

#pragma comment(linker, "/subsystem:CONSOLE")
#pragma comment(lib, "Advapi32.lib")

int wmain(int argc, wchar_t *argv[])
{
    SAFER_LEVEL_HANDLE hLevel = NULL;
    if (!SaferCreateLevel(SAFER_SCOPEID_USER, SAFER_LEVELID_NORMALUSER, SAFER_LEVEL_OPEN, &hLevel, NULL))
    {
        return GetLastError();
    }

    HANDLE hRestrictedToken = NULL;
    if (!SaferComputeTokenFromLevel(hLevel, NULL, &hRestrictedToken, 0, NULL))
    {
        SaferCloseLevel(hLevel);
        return GetLastError();
    }

    SaferCloseLevel(hLevel);

    wchar_t commandLine[1024*32];
    wcscpy_s(commandLine, GetCommandLineW());
    // skip our exe name
    wchar_t *cmd = commandLine;
    if (*cmd != L'\"')
    {
        cmd = wcschr(cmd, L' ') + 2;
        // FIXME: could be tab or other whitespace
    }
    else
    {
        cmd = wcschr(cmd + 1, L'"') + 2;
    }

    //Create startup info
    STARTUPINFO si = {0};
    PROCESS_INFORMATION pi = {0};
    //si.lpDesktop = L"winsta0\\default";
    si.cb = sizeof( si );
    si.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
    si.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    si.hStdError = GetStdHandle(STD_ERROR_HANDLE);
    si.dwFlags = STARTF_USESTDHANDLES;

    // printf("%S\n", cmd);

    // Start the new (non-elevated) restricted process
    if (!CreateProcessAsUser(hRestrictedToken, NULL, cmd, NULL, NULL, TRUE, CREATE_UNICODE_ENVIRONMENT, GetEnvironmentStringsW() /* leaks */, NULL, &si, &pi))
    {
        CloseHandle(hRestrictedToken);
        return GetLastError();
    }

    CloseHandle(hRestrictedToken);
    CloseHandle(pi.hThread);

    DWORD wso = WaitForSingleObject(pi.hProcess, INFINITE);
    if (wso != WAIT_OBJECT_0)
        return wso;

    DWORD exitCode = 2;
    GetExitCodeProcess(pi.hProcess, &exitCode);
    CloseHandle(pi.hProcess);

    return exitCode;
}
