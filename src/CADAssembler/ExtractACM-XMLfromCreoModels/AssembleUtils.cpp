#include <AssembleUtils.h>
#include <CommonUtilities.h>
#include <CADCommonConstants.h>
#include <CommonFeatureUtils.h>
#include <JsonHelper.h>
#include <time.h>

namespace isis
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////
::boost::filesystem::path SetupWorkingDirectory( std::string & inout_workingDirectory ) {
   ::boost::filesystem::current_path(inout_workingDirectory);
   ::boost::filesystem::path workingDir = ::boost::filesystem::current_path();
   inout_workingDirectory = workingDir.generic_string();
   return workingDir;
}

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