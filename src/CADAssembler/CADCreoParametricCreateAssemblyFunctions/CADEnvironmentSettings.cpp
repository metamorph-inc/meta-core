#include "CADEnvironmentSettings.h"
#include "CADSoftwareEnvirUtils.h"
#include <CommonUtilities.h>
#include <windows.h>
#include "WindowsFunctions.h"
#include <malloc.h>
//#include <stdio.h>
#include <iostream>
#include <vector>
//#include <list>
#include <tchar.h>
#include <algorithm>
#include <sstream>
#include <ISISConstants.h>
#include "LoggerBoost.h"

#include "CommonDefinitions.h"
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
														std::string		&out_CADToolDir,	
														std::string		&out_TemplateFile_PathAndFileName ) 
																					throw (isis::application_exception)
	{
		///////////////////////////////////////////////////////////////
		// Log: Use environment  variables or look values in registry
		//////////////////////////////////////////////////////////////
		
		const char *envVariable_CREO_PARAMETRIC_USE_ENVIR_VARS;
		envVariable_CREO_PARAMETRIC_USE_ENVIR_VARS = getenv ("CREO_PARAMETRIC_USE_ENVIR_VARS");
		if ( envVariable_CREO_PARAMETRIC_USE_ENVIR_VARS == NULL )
			isis_LOG(lg, isis_FILE, isis_WARN) << "Environment Variable CREO_PARAMETRIC_USE_ENVIR_VARS: Not Defined";
		else 
			isis_LOG(lg, isis_FILE, isis_INFO) << "Environment Variable CREO_PARAMETRIC_USE_ENVIR_VARS: " << envVariable_CREO_PARAMETRIC_USE_ENVIR_VARS;


		/////////////////////////////
		// Creo Start Command
		/////////////////////////////
		out_CreoStartCommand = ""; 
		isis::SetupCreoEnvironmentVariables(	in_graphicsModeOn,
												in_CreoExceptInputFromThisProgramAndCreoUI,
												out_CreoStartCommand);
		out_CADToolDir = META_PATH() + "\\bin\\CAD\\Creo";
		out_TemplateFile_PathAndFileName = out_CADToolDir + "\\templates\\" + isis::TEMPLATE_MODEL_NAME_METRIC + isis::TEMPLATE_MODEL_NAME_METRIC_SUFFIX;	


		/////////////////////////////
		// Log Resulting Settings
		/////////////////////////////
		isis_LOG(lg, isis_FILE, isis_INFO) << "";
		isis_LOG(lg, isis_FILE, isis_INFO)  << "************** Begin Environment Variables and System Settings *****************";
		isis_LOG(lg, isis_FILE, isis_INFO) << "CADToolDir:                    "	<< out_CADToolDir; 
		isis_LOG(lg, isis_FILE, isis_INFO) << "CreoStartCommand:              "	<< out_CreoStartCommand; 
		isis_LOG(lg, isis_FILE, isis_INFO) << "PRO_COMM_MSG_EXE:              "	<<  static_cast<const char*>(getenv ("PRO_COMM_MSG_EXE")); 
		isis_LOG(lg, isis_FILE, isis_INFO) << "TemplateFile_PathAndFileName:  "	<< out_TemplateFile_PathAndFileName; 
		isis_LOG(lg, isis_FILE, isis_INFO) << "************** End Environment Variables and System Settings *****************";

	}

} // end namespace isis