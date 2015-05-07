#include "stdafx.h"

#include <direct.h>
#include <stdio.h>
#include <stdlib.h>
#include "CommandExecutor.h"
#include "UdmConsole.h"

CommandExecutor::CommandExecutor( ): _done(false)
{}

void CommandExecutor::printMsg(const std::string& msg, msgtype_enum msgtype)
{
	GMEConsole::Console::writeLine(msg.c_str(), msgtype);
}

bool CommandExecutor::execute(const std::string& cmdline) {
	try{
		_done = false;
		redirectStdOutput();
		createChildProcess( cmdline);
		restoreStdOutput();
		readFromPipe();

		DWORD dwExitCode;
		GetExitCodeProcess(piProcInfo.hProcess, &dwExitCode);

		return (dwExitCode == 0);
	}
	catch(...)
	{
		return false;
	}
}

void CommandExecutor::redirectStdOutput() {
// The steps for redirecting child process's STDOUT: 
   //     1. Save current STDOUT, to be restored later. 
   //     2. Create anonymous pipe to be STDOUT for child process. 
   //     3. Set STDOUT of the parent process to be write handle to 
   //        the pipe, so it is inherited by the child process. 
   //     4. Create a noninheritable duplicate of the read handle and
   //        close the inheritable read handle. 
    SECURITY_ATTRIBUTES saAttr; 
    saAttr.nLength = sizeof(SECURITY_ATTRIBUTES); 
    saAttr.bInheritHandle = TRUE; 
    saAttr.lpSecurityDescriptor = NULL; 
    BOOL fSuccess= FALSE; 
// Save the handle to the current STDOUT. 
    _hSaveStdout = GetStdHandle(STD_OUTPUT_HANDLE); 
 // Create a pipe for the child process's STDOUT. 
	HANDLE hChildStdoutRd;
    if (! CreatePipe(&hChildStdoutRd, &_hChildStdoutWr, &saAttr, 0)) 
            throw CmdExecEx("Internal error. Stdout pipe creation failed in CommandExecutor::redirectStdOutput.");
 // Set a write handle to the pipe to be STDOUT. 
    if (! SetStdHandle(STD_OUTPUT_HANDLE, _hChildStdoutWr)) 
      throw CmdExecEx("Internal error. Redirecting STDOUT failed in CommandExecutor::redirectStdOutput.");
 // Create noninheritable read handle and close the inheritable read 
// handle. 
    fSuccess = DuplicateHandle(GetCurrentProcess(), hChildStdoutRd,
        GetCurrentProcess(), &_hChildStdoutRdDup , 0,
        FALSE,
        DUPLICATE_SAME_ACCESS);
    if( !fSuccess )
		throw CmdExecEx("Internal error. DuplicateHandle failed in CommandExecutor::redirectStdOutput.");
    CloseHandle(hChildStdoutRd);
}

void CommandExecutor::createChildProcess( const std::string& cmdline) {
//	_mp.print( getCwd());
//	_mp.print( cmdline, true);
	printMsg(cmdline);
//	PROCESS_INFORMATION piProcInfo; 
    STARTUPINFO siStartInfo; 
 // Set up members of the PROCESS_INFORMATION structure. 
    ZeroMemory( &piProcInfo, sizeof(PROCESS_INFORMATION) );
 // Set up members of the STARTUPINFO structure. 
    ZeroMemory( &siStartInfo, sizeof(STARTUPINFO) );
    siStartInfo.cb = sizeof(STARTUPINFO); 
	siStartInfo.dwFlags= STARTF_USESTDHANDLES;
    siStartInfo.hStdOutput = _hChildStdoutWr;
	siStartInfo.hStdError = _hChildStdoutWr;	/* use the same handle for STD error - so we catch the errors as well */

	DWORD dwExitCode = -1;
	// Create the child process. 
    if( !CreateProcess(NULL, 
      LPSTR(cmdline.c_str()),       // command line 
      NULL,          // process security attributes 
      NULL,          // primary thread security attributes 
      TRUE,          // handles are inherited 
      CREATE_NO_WINDOW,             // creation flags 
      NULL,          // use parent's environment 
      NULL,          // use parent's current directory 
      &siStartInfo,  // STARTUPINFO pointer 
      &piProcInfo	// receives PROCESS_INFORMATION 
	  ))
	{
		char cTemp[256];
		DWORD dTemp = GetLastError();
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, 0, dTemp, 0, cTemp, 256, NULL);
		std::string errorMsg = "Create process with command line: " + cmdline + " failed: " + cTemp;
		printMsg(errorMsg, MSG_ERROR);
		throw CmdExecEx( errorMsg );
	}
}

void CommandExecutor::restoreStdOutput() {
	if (! SetStdHandle(STD_OUTPUT_HANDLE, _hSaveStdout)) 
      throw CmdExecEx("Internal error. Redirecting Stdout failed in CommandExecutor::restoreStdOutput."); 

}

void CommandExecutor::readFromPipe() 
{ 
   #define BUFSIZE 4096 
   CHAR chBuf[BUFSIZE+1]; 
   DWORD dwRead= 0; 
   if (!CloseHandle(_hChildStdoutWr)) 
     throw CmdExecEx("Internal error. Closing handle failed  in CommandExecutor::readFromPipe.");
// Read output from the child process, and write to parent's STDOUT. 
   for (;!_done;) 
   { 
		if( !ReadFile( _hChildStdoutRdDup, chBuf, BUFSIZE, &dwRead, 
			NULL) || dwRead == 0) break;
		chBuf[ dwRead]= '\0';		
		//	  _mp.print( chBuf);
		printMsg(chBuf);
		DWORD dwExitCode;
		GetExitCodeProcess(piProcInfo.hProcess, &dwExitCode);
		_done = !(dwExitCode == STILL_ACTIVE); /* if not still active then its done */
   } 
} 