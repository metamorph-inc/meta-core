// Compile 32bit to execute 32bit java.exe, 64bit for 64bit
// cl exec_java_env.c
// cl exec_java_env.c /link /out:exec_java_env_x64.exe
// scp exec_java_env_x64.exe meta@metabuild4:

#include <Windows.h>
#include <wchar.h>
#include <string.h>
#include <stdio.h>
#include <Userenv.h>
#include <Lmcons.h>

#pragma comment(lib, "Kernel32.lib")
#pragma comment(lib, "Userenv.lib")
#pragma comment(lib, "Advapi32.lib")

/*
import java.lang.System;
import java.util.*;

public class PrintEnv {
    public static void main(String[] args) {
        Map<String, String> env = System.getenv();
        for (String envName : env.keySet()) {
            System.out.format("%s=%s%n", envName, env.get(envName));
        }
    }
}
*/

wchar_t* concat(wchar_t* szUsername, wchar_t* szzEnvBlock)
{
    wchar_t* ret;
    wchar_t* envBlockEnd = szzEnvBlock;
    wchar_t usernameEquals[] = L"USERNAME=";
    while (wcslen(envBlockEnd))
    {
        envBlockEnd = envBlockEnd + wcslen(envBlockEnd) + 1;
    }
    envBlockEnd += 2;
    ret = (wchar_t*)malloc(wcslen(usernameEquals) * sizeof(wchar_t)
        + (wcslen(szUsername) + 1) * sizeof(wchar_t)
        + (envBlockEnd - szzEnvBlock) * sizeof(wchar_t));
    memcpy(ret, usernameEquals, wcslen(usernameEquals) * sizeof(wchar_t));
    memcpy(ret + wcslen(usernameEquals), szUsername, (wcslen(szUsername) + 1) * sizeof(wchar_t));
    memcpy(ret + wcslen(usernameEquals) + wcslen(szUsername) + 1, szzEnvBlock, (envBlockEnd - szzEnvBlock) * sizeof(wchar_t));
    // printf("%S\n\n\n", ret);
    // printf("%S\n\n\n", ret + wcslen(usernameEquals) + wcslen(szUsername) + 1);
    return ret;
}

int main(int argc, char** argv) {
	HANDLE hToken;
	wchar_t *env;
    wchar_t *env_with_username;
    wchar_t username[UNLEN+1];
    DWORD username_len = sizeof(username) / sizeof(username[0]);
	wchar_t commandLine[1024*32], *cmd;
	DWORD exitCode;
	PROCESS_INFORMATION processInformation = {0};
	STARTUPINFOW startupInfo = {0};
	startupInfo.cb = sizeof(startupInfo);

	if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken) == 0)
		return GetLastError();
	if (CreateEnvironmentBlock((LPVOID*)&env, hToken, FALSE) == FALSE)
		return GetLastError();
    if (GetUserNameW(username, &username_len) == 0)
        return GetLastError();
    env_with_username = concat(username, env);
	cmd = commandLine + 10;
	wcscpy(cmd, GetCommandLineW());
	if (*cmd != L'\"')
	{
		cmd = wcschr(cmd, L' ');
		// FIXME: could be tab or other whitespace
	}
	else
	{
		cmd = wcschr(cmd + 1, L'"');
	}
	if (cmd)
	{
		/*if (wcsstr(cmd, L" -c ")) // ssh host command => shell -c "command"
		{
			cmd = wcsstr(cmd, L" -c ") + 3;
			if (wcschr(cmd, L'"'))
			{
				*wcschr(cmd, L'"') = L' ';
				//printf("%ld %ld", cmd, wcslen(cmd));
				if (*(cmd + wcslen(cmd) - 1) == L'"')
				{
					*(cmd + wcslen(cmd) - 1) = L'\0';
				}
			}
		}
		else */
			cmd++;
		cmd = cmd - 5;
		wcsncpy(cmd, L"java ", 5);
	}
	else
	{
		wcscpy(cmd = commandLine, L"java");
	}
	// wcscpy(cmd, L"java -jar c:\\jenkins_builds\\slave.jar");
	// printf("cmd %S\n", cmd);
	if (CreateProcessW(L"C:\\Windows\\System32\\java.exe",
		cmd,
		NULL, NULL,
		TRUE,
		CREATE_UNICODE_ENVIRONMENT,
		env_with_username,
		NULL,
		&startupInfo,
		&processInformation) == 0)
		return GetLastError();
	if (WaitForSingleObject(processInformation.hProcess, INFINITE) == WAIT_FAILED)
		return GetLastError();
	GetExitCodeProcess(processInformation.hProcess, &exitCode);
	CloseHandle(processInformation.hProcess);
	CloseHandle(processInformation.hThread);
	
	return exitCode;
}
