#include "cc_LoggerBoost.h"
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/filesystem.hpp>
#include <iomanip> 
#include <boost/algorithm/string.hpp>
#include "cc_CommonUtilities.h"
#include "cc_MiscellaneousFunctions.h"


isis_boost_logger_type lg;

namespace logging = boost::log;
namespace expr = boost::log::expressions;
namespace keywords = boost::log::keywords;

// Define the attribute keywords
BOOST_LOG_ATTRIBUTE_KEYWORD(line_id, "LineID", unsigned int)
BOOST_LOG_ATTRIBUTE_KEYWORD(severity, "Severity", isis_LogSeverityLevel)
BOOST_LOG_ATTRIBUTE_KEYWORD(channel, "Channel", std::string)

//<-
std::ostream& operator<< (std::ostream& strm, isis_LogSeverityLevel level)
{
    static const char* strings[] =
    {
		"DEBUG",
		"INFO",
		"WARN",
		"ERROR" 
    };

    if (static_cast< std::size_t >(level) < sizeof(strings) / sizeof(*strings))
        strm << strings[level];
    else
        strm << static_cast< int >(level);

    return strm;
}
//->

void init_logging_boost(	bool				in_include_severity_level_in_msg_file,
							bool				in_include_severity_level_in_msg_console,
							isis_LogSeverityLevel		in_severity_level_file,
							isis_LogSeverityLevel		in_severity_level_console,
							const std::string	&in_log_file_name )
{
    // Create a minimal severity table filter
    typedef expr::channel_severity_filter_actor< std::string, isis_LogSeverityLevel > min_severity_filter;
    min_severity_filter min_severity = expr::channel_severity_filter(channel, severity);
	
	

	//min_severity["FILE"] = in_severity_level_file;
    //min_severity["CONSOLE"] = in_severity_level_console;

	min_severity[isis_FILE] = in_severity_level_file;
    min_severity[isis_CONSOLE] = in_severity_level_console;
	min_severity[isis_CONSOLE_FILE] = in_severity_level_console;


	if (in_include_severity_level_in_msg_console )
	{
		logging::add_console_log
		(
			std::clog,
			keywords::filter =  (channel == isis_CONSOLE  || channel == isis_CONSOLE_FILE) && ( min_severity || severity >= isis_ERROR),
			keywords::format = ( expr::stream << std::setw(7) <<  std::left << severity << expr::smessage )
			// Old keywords::format = ( expr::stream << line_id << ": <" << severity << "> [" << channel << "] " << expr::smessage )
		);
	}
	else
	{
		logging::add_console_log
		(
			std::clog,
			keywords::filter =  (channel == isis_CONSOLE  || channel == isis_CONSOLE_FILE) && (min_severity || severity >= isis_ERROR),
			keywords::format = (expr::stream << expr::smessage)
		);
	}
	
	if ( in_log_file_name.size() > 0 )
	{
		if ( in_include_severity_level_in_msg_file )
		{
			logging::add_file_log
			(
				in_log_file_name,
				keywords::filter = (channel == isis_FILE || channel == isis_CONSOLE_FILE) && (min_severity || severity >= isis_ERROR),
				keywords::format = ( expr::stream << std::setw(7) <<  std::left << severity << expr::smessage),
				keywords::auto_flush = true
			);
		}
		else
		{
			logging::add_file_log
			(
				in_log_file_name,
				keywords::filter = (channel == isis_FILE || channel == isis_CONSOLE_FILE) && (min_severity || severity >= isis_ERROR),
				keywords::format = ( expr::stream << expr::smessage),
				keywords::auto_flush = true
			);
		}
	}
}

void SetupLogging(	const std::string		&in_SubDir,  
					std::string				&in_Logfilename, 
					bool					in_include_severity_level_in_msg_file,
					bool					in_include_severity_level_in_msg_consol,
					isis_LogSeverityLevel	in_severity_level_file,
					isis_LogSeverityLevel	in_severity_level_console)

{

	std::string logfilenamepath;

	std::string subDir = in_SubDir;
	boost::trim(subDir);

	if ( subDir.length() > 0 )
	{
		logfilenamepath = subDir + "\\"+ in_Logfilename;

		if ( !boost::filesystem::exists(subDir)) boost::filesystem::create_directory(subDir);
	}
	else
	{
		logfilenamepath = in_Logfilename;
	}

	init_logging_boost(	in_include_severity_level_in_msg_file, 
						in_include_severity_level_in_msg_consol, 
						in_severity_level_file, 
						in_severity_level_console, 
						logfilenamepath);

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
void	LogMainNonZeroExitCode( const std::string			&in_ExeName,
								int							in_ExitCode, 
							    const std::string			&in_InputLine,
								bool						in_Logging_Set_Up, 
								const std::string			in_LogFileName,  
								const std::string			&in_ExceptionErrorStringStream )
{

       // Write to _FAILED.txt
        std::string failedTxtFileName = "_FAILED.txt";
        bool addLineFeed = false;
        if(isis::FileExists(failedTxtFileName.c_str()))
        {
            addLineFeed = true;
        }

        std::ofstream failedTxtFileStream;
        failedTxtFileStream.open(failedTxtFileName, std::ios::app);

        if(failedTxtFileStream.is_open())
        {
            if(addLineFeed)		failedTxtFileStream << std::endl;

			failedTxtFileStream << std::endl <<	"Input Line: "   << in_InputLine <<  std::endl;
			//failedTxtFileStream << std::endl <<	"Input Line: " << in_InputLine <<  std::endl;
            failedTxtFileStream << std::endl <<  isis_CADCommon::GetDayMonthTimeYear() << ", " << in_ExeName << " error code: " << in_ExitCode;
			failedTxtFileStream << std::endl <<  in_ExceptionErrorStringStream;

			if(in_Logging_Set_Up)	failedTxtFileStream << std::endl << "For additional information, scroll to the bottom of " << in_LogFileName;

            failedTxtFileStream.close();
        }

        if(in_Logging_Set_Up)
        {
            
            isis_LOG(lg, isis_FILE, isis_ERROR) << in_ExceptionErrorStringStream;
        }
        else
        {
            std::cerr << std::endl << std::endl << in_ExceptionErrorStringStream << std::endl << std::endl;
		}

}

//]