#include <AssembleUtils.h>
#include <CommonUtilities.h>
#include <cc_CommonConstants.h>
#include <CommonFeatureUtils.h>
#include <cc_JsonHelper.h>
#include <time.h>
#include <Windows.h>

namespace isis
{

	std::string GetDayMonthTimeYear()
	{
		time_t time_start;		// calendar time 
		time_start=time(NULL);	// get current cal time 

		std::string dayMonthTimeYear = asctime(localtime(&time_start));
		if (dayMonthTimeYear.size() > 0 )
		{
			// Remove the extra linefeed from the above string.
			dayMonthTimeYear.replace(dayMonthTimeYear.end() -1, dayMonthTimeYear.end(), "");

		}
		return dayMonthTimeYear;
	}
} // end namespace isis