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
#include <iomanip> 


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
				keywords::format = ( expr::stream << std::setw(7) <<  std::left << severity << expr::smessage)
				//, keywords::auto_flush = true
			);
		}
		else
		{
			logging::add_file_log
			(
				in_log_file_name,
				keywords::filter = (channel == isis_FILE || channel == isis_CONSOLE_FILE) && (min_severity || severity >= isis_ERROR),
				keywords::format = ( expr::stream << expr::smessage)
				//, keywords::auto_flush = true
			);
		}
	}
}

//]