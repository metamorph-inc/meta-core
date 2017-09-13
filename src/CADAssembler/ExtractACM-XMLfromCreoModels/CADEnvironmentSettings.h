#ifndef CAD_ENVIRONMENT_SETTINGS_H
#define CAD_ENVIRONMENT_SETTINGS_H
#include <string>
#include <vector>
#include "isis_application_exception.h"
namespace isis
{



	void SetCreoEnvirVariable_RetrieveSystemSettings(	bool			in_graphicsModeOn,
														bool			in_CreoExceptInputFromThisProgramAndCreoUI,
														std::string		&out_CreoStartCommand,
														std::string		&out_CADExtensionsDir,	
														std::string		&out_TemplateFile_PathAndFileName ) 
																					throw (isis::application_exception);

}
#endif
