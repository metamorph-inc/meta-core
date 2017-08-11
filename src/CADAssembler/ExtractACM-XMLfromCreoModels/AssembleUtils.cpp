#include <AssembleUtils.h>
#include <CommonUtilities.h>
#include <CADCommonConstants.h>
#include <CommonFeatureUtils.h>
#include <JsonHelper.h>
#include <time.h>
#include <Windows.h>

namespace isis
{
	// copied from CADCreoParametricCreateAssemblyFunctions AssembleUtils.cpp
	std::string META_PATH()
	{
		std::string metapath;
		HKEY software_meta;
		if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, "Software\\META", 0, KEY_READ, &software_meta) == ERROR_SUCCESS)
		{
			BYTE data[MAX_PATH];
			DWORD type, size = sizeof(data) / sizeof(data[0]);
			if (RegQueryValueExA(software_meta, "META_PATH", 0, &type, data, &size) == ERROR_SUCCESS)
			{
				metapath = std::string(data, data + strnlen((const char*)data, size));
			}
			RegCloseKey(software_meta);
		}
		return metapath;
	}


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