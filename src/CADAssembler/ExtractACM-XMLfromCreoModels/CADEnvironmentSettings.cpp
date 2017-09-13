#include "CADEnvironmentSettings.h"
#include "CADSoftwareEnvirUtils.h"
#include <CommonUtilities.h>
#include <windows.h>
#include "cc_WindowsFunctions.h"
#include <malloc.h>
//#include <stdio.h>
#include <iostream>
#include <vector>
//#include <list>
#include <tchar.h>
#include <algorithm>
#include <sstream>
#include <ISISConstants.h>
#include "AssembleUtils.h"

// #define BUFFER 8192

namespace isis
{		
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	//	Description:	
	//		Based on the input arguments (i.e. in_argv), this function sets the PRO_COMM_MSG_EXE system 
	//		environment variable and retrieves the system settings.  See the function 
	//		SetupCreoEnvironmentVariables for more information about setting the evirmoment 
	//		variable and the source of the retrieved data.
	//
	//		If the data cannot be retrieved then isis::application_exception will be thrown.

	void SetCreoEnvirVariable_RetrieveSystemSettings(	bool			in_graphicsModeOn,
														bool			in_CreoExceptInputFromThisProgramAndCreoUI,
														std::string		&out_CreoStartCommand,
														std::string		&out_CADExtensionsDir,	
														std::string		&out_TemplateFile_PathAndFileName ) 
																					throw (isis::application_exception)
	{
		///////////////////////////////////////////////////////////////
		// Log: Use environment  variables or look values in registry
		//////////////////////////////////////////////////////////////
		char *envVariable_CREO_PARAMETRIC_USE_ENVIR_VARS;
		envVariable_CREO_PARAMETRIC_USE_ENVIR_VARS = getenv ("CREO_PARAMETRIC_USE_ENVIR_VARS");
		//std::clog << std::endl;
		//if ( envVariable_CREO_PARAMETRIC_USE_ENVIR_VARS == NULL )
		//	std::clog << std::endl << "Environment Variable CREO_PARAMETRIC_USE_ENVIR_VARS: Not Defined";
		//else 
		//	std::clog << std::endl << "Environment Variable CREO_PARAMETRIC_USE_ENVIR_VARS: " << envVariable_CREO_PARAMETRIC_USE_ENVIR_VARS;


		/////////////////////////////
		// Creo Start Command
		/////////////////////////////
		out_CreoStartCommand = ""; 
		isis::SetupCreoEnvironmentVariables(	in_graphicsModeOn,
												in_CreoExceptInputFromThisProgramAndCreoUI,
												out_CreoStartCommand);
        out_CADExtensionsDir = META_PATH() + "\\bin\\CAD\\Creo";
		out_TemplateFile_PathAndFileName =  out_CADExtensionsDir + "\\templates\\" + isis::TEMPLATE_MODEL_NAME_METRIC + isis::TEMPLATE_MODEL_NAME_METRIC_SUFFIX;	

		/////////////////////////////
		// Log Resulting Settings
		/////////////////////////////
		//std::clog << std::endl << std::endl << "************** Begin Environment Variables and System Settings *****************";
		//std::clog << std::endl << "CADExtensionsDir:                    "	<< out_CADExtensionsDir; 
		//std::clog << std::endl << "CreoStartCommand:              "	<< out_CreoStartCommand; 
		//std::clog << std::endl << "PRO_COMM_MSG_EXE:              "	<<  getenv ("PRO_COMM_MSG_EXE"); 
		//std::clog << std::endl << "TemplateFile_PathAndFileName:  "	<< out_TemplateFile_PathAndFileName; 
		//std::clog << std::endl << "************** End Environment Variables and System Settings *****************";

	}

} // end namespace isis