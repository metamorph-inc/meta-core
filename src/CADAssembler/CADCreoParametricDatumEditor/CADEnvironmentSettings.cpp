#include "CADEnvironmentSettings.h"
#include "CADSoftwareEnvirUtils.h"
#include <CommonUtilities.h>
#include <windows.h>
#include <malloc.h>
//#include <stdio.h>
#include <iostream>
#include <vector>
//#include <list>
#include <tchar.h>
#include <algorithm>
#include <sstream>
#include <ISISConstants.h>
// #define BUFFER 8192

namespace isis
{		

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	//	Description:	
	//		Based on the input arguments (i.e. in_argv), this function sets the PRO_COMM_MSG_EXE system 
	//		environment variable and retrieves the system settings.  See the function 
	//		SetupCreoEnvironmentVariables (above in this file) for more information about setting the evirmoment 
	//		variable and the source of the retrieved data.
	//
	//		If the data cannot be retrieved then isis::application_exception will be thrown.
	void SetCreoEnvirVariable_RetrieveSystemSettings( int   in_argc, 
													char *in_argv[],
													std::string		&out_CreoStartCommand,
													std::string		&out_XmlInputFile_PathAndFileName,  // may not contain path, if so reads from the same directory as the bat file location
													std::string		&out_LogFile_PathAndFileName )		// may not contain path, if so is written to the same directory as the bat file location
																	throw (isis::application_exception)
	{
		///////////////////////////////////////////////////////////////
		// Log: Use environment  variables or look values in registry
		//////////////////////////////////////////////////////////////
		char *envVariable_CREO_PARAMETRIC_USE_ENVIR_VARS;
		envVariable_CREO_PARAMETRIC_USE_ENVIR_VARS = getenv ("CREO_PARAMETRIC_USE_ENVIR_VARS");
		std::clog << std::endl;
		if ( envVariable_CREO_PARAMETRIC_USE_ENVIR_VARS == NULL )
			std::clog << std::endl << "Environment Variable CREO_PARAMETRIC_USE_ENVIR_VARS: Not Defined";
		else 
			std::clog << std::endl << "Environment Variable CREO_PARAMETRIC_USE_ENVIR_VARS: " << envVariable_CREO_PARAMETRIC_USE_ENVIR_VARS;


		int xmlFileArg;
		int logFileArg;

		//New Case: %WORKING_DIR%   %PARTS_DIR%    %ASSEMBLY_XML_FILE%     %LOG_FILE%     %EXIT_PROMPT%


		xmlFileArg = 2;
		logFileArg = 3;

		//////////////////////////
		// Log Arguments
		//////////////////////////
		std::clog << std::endl << "arg 0, EXE:                     "  << in_argv[0];				// 0 EXE				e.g. C:\Program Files\Proe ISIS Extensions\bin\CADCreoParametricDatumEditor.exe
		std::clog << std::endl << "arg 1, Switch                   "  << in_argv[1];				// 1 Switch				e.g. -check/-edit
		std::clog << std::endl << "arg 2, ASSEMBLY_XML_FILE:       "  << in_argv[xmlFileArg];		// 3 ASSEMBLY_XML_FILE	e.g. RLC_Assembly_5_CAD.xml
		std::clog << std::endl << "arg 3, LOG_FILE:                "  << in_argv[logFileArg];		// 4 LOG_FILE			e.g. RLC_Assembly_5_CAD.xml.log

		//////////////////////////
		// Prompt Before Exiting
		//////////////////////////

		/////////////////
		// Directories
		/////////////////

		out_XmlInputFile_PathAndFileName = in_argv[xmlFileArg];
		out_LogFile_PathAndFileName = in_argv[logFileArg];

		/////////////////////////////
		// Creo Start Command
		/////////////////////////////
		out_CreoStartCommand = ""; 
		isis::SetupCreoEnvironmentVariables(out_CreoStartCommand);

		/////////////////////////////
		// Log Resulting Settings
		/////////////////////////////
		std::clog << std::endl << std::endl << "************** Begin Environment Variables and System Settings *****************";
		std::clog << std::endl << "CreoStartCommand:              "	<< out_CreoStartCommand; 
		std::clog << std::endl << "PRO_COMM_MSG_EXE:              "	<<  getenv ("PRO_COMM_MSG_EXE"); 
		std::clog << std::endl << "XmlInputFile_PathAndFileName:  "	<< out_XmlInputFile_PathAndFileName; 
		std::clog << std::endl << "LogFile:                       "	<< out_LogFile_PathAndFileName;  
		std::clog << std::endl << "************** End Environment Variables and System Settings *****************";

	}

} // end namespace isis