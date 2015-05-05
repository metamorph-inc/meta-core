// cl exec_java_env.c
// cl exec_java_env.c /link /out:exec_java_env_x64.exe
// scp exec_java_env_x64.exe meta@metabuild4:

#include <Windows.h>
#include <wchar.h>
#include <string.h>
#include <stdio.h>
#include <Userenv.h>

#pragma comment(lib, "Kernel32.lib")
#pragma comment(lib, "Userenv.lib")
#pragma comment(lib, "Advapi32.lib")

int main(int argc, char** argv) {
	HANDLE hToken;
	wchar_t *env;
	wchar_t commandLine[1024*32], *cmd;
	DWORD exitCode;
	PROCESS_INFORMATION processInformation = {0};
	STARTUPINFOW startupInfo = {0};
	startupInfo.cb = sizeof(startupInfo);

	if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken) == 0)
		return GetLastError();
	if (CreateEnvironmentBlock((LPVOID*)&env, hToken, FALSE) == FALSE)
		return GetLastError();
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
		env,
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
