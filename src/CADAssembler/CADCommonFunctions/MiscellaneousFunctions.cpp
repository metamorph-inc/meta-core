#include "MiscellaneousFunctions.h"
#include <time.h>

namespace isis_CADCommon
{
	////////////////////////////////////////////////////////////////////////////
	std::string IntegerToString( int in_Integer)
	{
		char buffer[64];
		_itoa_s(in_Integer, buffer,10);
		return std::string(buffer);
	}


	std::string GetDayMonthTimeYear()
	{
		time_t time_start;		// calendar time 
		time_start = time(NULL);	// get current cal time 

		struct tm timeinfo;
        localtime_s(&timeinfo, &time_start);

		char timebuf[32];
		errno_t error_code = asctime_s(timebuf, sizeof(timebuf), &timeinfo);
		std::string dayMonthTimeYear(timebuf);
		dayMonthTimeYear.replace(dayMonthTimeYear.end() -1, dayMonthTimeYear.end(), "");
		return dayMonthTimeYear;
	}

	////////////////////////////////////////////////////////////////////////////
}  // end namespace isis
