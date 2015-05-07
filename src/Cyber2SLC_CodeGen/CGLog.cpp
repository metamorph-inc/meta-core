#ifndef __CGLOG_CPP
#define __CGLOG_CPP

#ifdef _MSC_VER
#pragma warning (disable : 4786)
#pragma warning (disable : 4503) 
#endif


#include "CGLog.h"

void CGLog::print(const std::string& msg)
{
	if ( logFile.is_open())
	{
		logFile<<msg<<std::endl;
	}

}
	
void CGLog::close()
{
	if ( logFile.is_open())
		logFile.close();
	if(instance)
		delete instance, instance = 0;
}

CGLog::CGLog(){};
CGLog::CGLog( const std::string& file_path, bool append)
{
	filePath = file_path;
	if(append)
		logFile.open( filePath.c_str(), std::ios::app);
	else
		logFile.open( filePath.c_str());
}

CGLog::~CGLog()
{
	if ( logFile.is_open() && instance)
	{	
		logFile.close();
		delete instance, instance = 0;
	}
}

std::string CGLog::filePath = "";
CGLog* CGLog::instance = 0;

CGLog* CGLog::Instance()
{
    if( instance == 0)
    {
        instance = new CGLog();
	}
    return instance;
}

CGLog* CGLog::Instance(const std::string& file_path, bool append)
{
    if( instance == 0)
    {
        instance = new CGLog(file_path, append);
	}
    return instance;
}
//Open log file
extern void openLogFile(const std::string& file_path, bool append)
{
	CGLog *log = CGLog::Instance(file_path, append);
}
// Close log file
extern void closeLogFile()
{
	CGLog *log = CGLog::Instance();
	log->close();
}

extern void printLog( const std::string& msg)
{
	CGLog *log = CGLog::Instance();
	log->print(msg);
}

#endif //__CGLOG_CPP
