/* NAME:	Error Log (Dummy)
 * AUTHOR:	Feng Shi - fengshi@isis-server.isis.vanderbilt.edu
 */
#ifndef __CGLOG_H
#define __CGLOG_H

#ifdef _MSC_VER
#pragma warning (disable : 4786)
#pragma warning (disable : 4503) 
#endif


#include <string>
#include <fstream>
#include <iostream>


class CGLog
{
public:
	static std::string filePath;
	static CGLog* Instance();
	static CGLog* Instance(const std::string& file_path, bool append=false);
	void print(const std::string& msg);
	void close();
protected:
	CGLog();
	CGLog( const std::string& file_path, bool append);
	~CGLog();
private:
    static CGLog* instance;
	std::ofstream logFile;
};

//Open log file
extern void openLogFile(const std::string& file_path, bool append=false);
extern void closeLogFile();
extern void printLog( const std::string& msg);

#endif //__CGLOG_H
