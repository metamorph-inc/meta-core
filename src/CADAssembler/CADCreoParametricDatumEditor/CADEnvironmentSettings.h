#ifndef CAD_ENVIRONMENT_SETTINGS_H
#define CAD_ENVIRONMENT_SETTINGS_H
#include <string>
#include <vector>
#include "isis_application_exception.h"
namespace isis
{



	void SetCreoEnvirVariable_RetrieveSystemSettings( int   in_argc, 
													char *in_argv[],
													std::string		&out_CreoStartCommand,
													std::string		&out_XmlInputFile_PathAndFileName,  // may not contain path, if so reads from the same directory as the bat file location
													std::string		&out_LogFile_PathAndFileName )		// may not contain path, if so is written to the same directory as the bat file location
																	throw (isis::application_exception);
}
#endif
