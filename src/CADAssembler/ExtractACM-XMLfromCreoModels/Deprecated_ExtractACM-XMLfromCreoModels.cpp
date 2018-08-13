#include <process.h>
#include <stdio.h>
#include <Windows.h>

int main(int argc, char* argv[]) {

	wchar_t commandLine[1024 * 32];
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
	if (cmd < commandLine) {
		cmd = L"";
	}

	wchar_t exeFilename[MAX_PATH];
	if (!GetModuleFileNameW(NULL, exeFilename, _countof(exeFilename))) {
		return GetLastError();
	}
	wchar_t exeDirectory[MAX_PATH];
	wchar_t* filePart = nullptr;
	if (!GetFullPathNameW(exeFilename, _countof(exeDirectory), exeDirectory, &filePart)) {
		return GetLastError();
	}
	*filePart = '\0';

	wchar_t invocation[1024 * 32 + MAX_PATH];
	// quotes at start and end:
	// "Otherwise, old behavior is to see if the first character is a quote character and if so, strip the leading character and remove the last quote character on the command line, preserving any text after the last quote character."
	swprintf_s(invocation, L"\"\"%sCADCreoParametricCreateAssembly.exe\" ExtractACM-XMLfromCreoModels %s\"", exeDirectory, cmd);

	return _wsystem(invocation);
}