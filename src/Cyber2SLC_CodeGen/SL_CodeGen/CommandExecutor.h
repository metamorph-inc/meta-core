#pragma once

#ifndef __COMMANDEXECUTOR_H
#define __COMMANDEXECUTOR_H


#include "Gme.h"
#include <exception>
#include <string>
//#include "stdafx.h"
#include "ComponentLib.h"

// exception type
class CmdExecEx : public std::exception
{
public:
	CmdExecEx() throw();
	CmdExecEx(const CmdExecEx &a) throw() : description(a.description) { }
	CmdExecEx(const std::string &d) throw() : description(d) { }
	CmdExecEx(const char *d) throw() : description(d) { }
	const CmdExecEx &operator =(const CmdExecEx &a) throw()
		{ description = a.description; return *this; }
	virtual ~CmdExecEx() throw() { }
	virtual const char *what() const throw() { return description.c_str(); }

protected:
	std::string description;
}; 

// command executor type
class CommandExecutor {
public:
	//CommandExecutor( IMessagePrinter& mp);
	CommandExecutor( );
	bool execute(const std::string& cmdline);
	void printMsg(const std::string& msg, msgtype_enum msgtype=MSG_NORMAL);

protected:
	//
	void createChildProcess( const std::string& cmdline);
	void redirectStdOutput();
	void restoreStdOutput();
	void readFromPipe(); 

private:
	bool _done;
	PROCESS_INFORMATION piProcInfo; 
	HANDLE _hSaveStdout;
	HANDLE _hChildStdoutRdDup;
	HANDLE _hChildStdoutWr;
};

#endif