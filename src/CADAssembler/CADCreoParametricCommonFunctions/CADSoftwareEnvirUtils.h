#ifndef CAD_SOFTWARE_ENVIR_UTILS_H
#define CAD_SOFTWARE_ENVIR_UTILS_H

#pragma warning( disable : 4290 )

#include "isis_application_exception.h"
#include <string>
#include <vector>

namespace isis
{

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	//	Description:
	//		This function either retrieves system settings information from the registry or from system 
	//		environment variables.   
	//
	//		If  (environment  variable CREO_PARAMETRIC_USE_ENVIR_VARS == TRUE)
	//			then
	//				settings are retrieved from system environment variables.
	//				See  "0Readme - CreateAssembly.txt" for environment variable setup instructions.
	//				This file is typical located at "C:\Program Files (x86)\META\bin\CAD\Creo\"
	//
	//			else
	//				settings are retrieved from the registry
	//
	//	Note -	This function also sets the system environment variable PRO_COMM_MSG_EXE.  This setting is
	//			required to run the Creo SDK.
	//	
	//	Pre-Conditions:
	//		None
	//
	//	Post-Conditions:
	//		If  ( environment  variable CREO_PARAMETRIC_USE_ENVIR_VARS == TRUE)
	//			then
	//				if CREO_PARAMETRIC_INSTALL_PATH or CREO_PARAMETRIC_COMM_MSG_EXE system environment variable not defined
	//					then
	//						throw isis::application_exception
	//			else
	//				if Creo install information not in the windows registry
	//					then
	//						throw isis::application_exception
	//
	//		If no exceptions
	//			then
	//				set PRO_COMM_MSG_EXE system environment variable
	//				return out_CreoStartCommand		
	void SetupCreoEnvironmentVariables( bool			in_graphicsModeOn,
										bool			in_CreoExceptInputFromThisProgramAndCreoUI,
										std::string &out_CreoStartCommand ) throw (isis::application_exception);

}
#endif
