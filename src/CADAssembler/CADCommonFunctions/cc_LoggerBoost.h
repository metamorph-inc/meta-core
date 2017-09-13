#ifndef LOGGER_BOOST_H
#define LOGGER_BOOST_H

#include <cstddef>
#include <iostream>
#include <boost/log/expressions.hpp>
#include <boost/log/sources/severity_channel_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
//#include <boost/log/utility/setup/console.hpp>
//#include <boost/log/utility/setup/common_attributes.hpp>

#define isis_LOG(lg, chan, lvl) BOOST_LOG_CHANNEL_SEV(lg, chan, lvl)


namespace src = boost::log::sources;


const std::string isis_FILE = "FILE";
const std::string isis_CONSOLE = "CONSOLE";
const std::string isis_CONSOLE_FILE = "CONSOLE_FILE";

const std::string isis_EOL = "\n";


enum isis_LogSeverityLevel
{
	isis_DEBUG,
	isis_INFO,
	isis_WARN,
	isis_ERROR
};

// Logger Types
//typedef src::severity_channel_logger< isis_LogSeverityLevel, std::string >    isis_boost_logger_type;
// Use multi-threaded, we will use the multi-threaded logger even though the single threaded logger will
// probably work.  Don't want to risk future threading issues breaking the code.
typedef src::severity_channel_logger_mt< isis_LogSeverityLevel, std::string > isis_boost_logger_type;

extern isis_boost_logger_type lg;



void init_logging_boost(	bool				in_include_severity_level_in_msg_file,
							bool				in_include_severity_level_in_msg_console,
							isis_LogSeverityLevel		in_severity_level_file,
							isis_LogSeverityLevel		in_severity_level_console,
							const std::string	&in_log_file_name );

namespace isis {
	template< typename CharT, typename TraitsT, typename AllocatorT, typename T >
	inline boost::log::basic_formatting_ostream< CharT, TraitsT, AllocatorT >&
	operator<< (boost::log::basic_formatting_ostream< CharT, TraitsT, AllocatorT >& strm, T const& value)
	{
		strm.stream() << value;
		return strm;
	}
}

#endif // !LOGGER_H
