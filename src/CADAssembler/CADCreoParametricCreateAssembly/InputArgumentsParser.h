#ifndef INPUT_ARGUMENTS_PARSER_H
#define INPUT_ARGUMENTS_PARSER_H

// File: ParseInputArguments.h
// Author: Kevin Zeillmann
// Last Modified: 6/3/13
// Description: This file provides the struct used to parse the input arguments,
//  defines the isis::application_exception (to make it work with the tester file,
//  and the prototype function header for the ParseInputArguments file.
#include <boost/program_options.hpp>
#include <boost/algorithm/string.hpp>

#include <iostream>
#include "cc_LoggerBoost.h"


// Taken from the files on the META SVN
// Required to throw isis::application_exception properly in the test files
namespace isis
{
	//!  application_exception class. 
	/*!
	A wrapper class for std::exception.  application_exception would normally be thrown by
	CAD routines developed by ISIS. 
	*/


//	class application_exception : public std::exception
//	{
//		public:
//			application_exception(const char *in_Message) throw() : exception(in_Message){};
//			virtual ~application_exception() throw(){};
//	};
	
//}


const std::string MAJOR_MODE_DESIGN = "design";
const std::string MAJOR_MODE_COMPONENT = "component";
const std::string MAJOR_MODE_PASSIVE = "passive";

const std::string LOG_VERBOSITY_NOTSET = "none";
const std::string LOG_VERBOSITY_DEBUG = "debug";
const std::string LOG_VERBOSITY_INFO = "inform";
const std::string LOG_VERBOSITY_WARN = "warning";
const std::string LOG_VERBOSITY_ERROR = "error";
const std::string LOG_VERBOSITY_FATAL = "fatal";
const std::string LOG_VERBOSITY_CRIT = "critical";
const std::string LOG_VERBOSITY_EMERG = "emergency";
const std::string LOG_VERBOSITY_NOTICE = "notice";
const std::string LOG_VERBOSITY_ALERT = "alert";

// Description: This struct contains the names of the files, directories, and options 
//  needed to run the CADCreoParametricCreateAssembly.exe application. These data 
//  members are taken from the input arguments for the application and parsed in 
//  the ParseInputArguments function.
//
struct ProgramInputArguments
{
       bool						graphicsModeOn;            // default false
       std::string				inputXmlFileName;          // could be the file name or the path and file name
       std::string				workingDirectory;          // could be the complete path or a relative path
       std::string				logFileName;               // could be the file name or the path and file name
	   isis_LogSeverityLevel	logVerbosity;
       std::string				logConfigFile;             // the file name for the log configuration 
       bool						promptBeforeExiting;       // default false
	   std::string				auxiliaryCADDirectory;     // empty string by default - could be complete path or relative path
	   bool						synchronizeWithCyPhy;      // Default false
	   std::string				majorMode;                 // {"design", "component"}
       std::string				syncConnectionString;      // If this is populated, then synchronizeWithCyPhy must be true.
	   std::string				designID;				   // this is used by MetaLink to associate CyPhy session with a CyPhy Design 
 
       ProgramInputArguments(): graphicsModeOn(false), promptBeforeExiting(false), 
		   logFileName("cad-assembler.log"), logVerbosity(isis_INFO), logConfigFile(""),
		   auxiliaryCADDirectory(""), synchronizeWithCyPhy(false), majorMode(MAJOR_MODE_DESIGN) {};

	   bool is_designMode();
	   bool is_componentMode();
	   bool is_passiveMode();

	   // Allows us to print debug information for the class
	   friend std::ostream& operator<<(std::ostream & in_stream, const ProgramInputArguments &in_args);
};


// Description:
//           Parse the input arguments to this program and populate out_ProgramInputArguments
//           Keys
//                  -w  Working directory.  The directory where the generated files would be persisted.
//                  -i  Input XML file name.  This file defines the CAD assembly definition.
//                  -a  Auxiliary CAD directory.  Would contain CAD parts. 
//                  -l  Log file name.   
//                  -p  Prompt before exiting.  Not prompting is the default. 
//                  -g  Graphics mode.  No graphics is the default.
//                  -h  Help, Display keys along with the usage.
//                  -s  Connection string (e.g. localhost:4949)
//                  -m  Major mode, editing either {design, component}
//				`	-d  Design ID 
//
//           Required Keys: -w, -l
//           Optional Keys: -a, -p, -g ,-s, i, d
//           Key Order: No particular order required
//           Key Grouping:  -p and -g may be grouped (e.g. -pg)
//           Keys are case insensitive.
//
//           String Contents
//           Key          File_Name    Full_Path_And_File_Name    Relative_Path_And_File_Name   Contain_Spaces (Must be enclosed in double quotes)
//           -w           N/A                 Yes                 Yes                              Yes
//           -i           Yes                 Yes                 Yes                              Yes
//           -a           N/A                 Yes                 Yes                              Yes
//           -l           Yes                 Yes                 Yes                              Yes
//			 -s			  Yes                 Yes                 Yes	                           Yes
//			 -d			  N/A				  N/A				  N/A							   No
//           Example Invocations:
//                  CADCreoParametricCreateAssembly.exe  -i CADAssembly.xml  -w . -a .\Cad_Components_Directory -l  CADAssembly.xml.log -p
//                  CADCreoParametricCreateAssembly.exe  -w "C:\Temp\aaa_Regression_Models\01_IFV\IFV Complete Assembly" -a "C:\Temp\aaa_Regression_Models\CAD_Lib\01_IFV\All Components" -i IFV_cfg125_Cad.xml -l IFV_cfg125_Cad.xml.log -gp
//                  CADCreoParametricCreateAssembly.exe  -d  {5b096535-3aef-4ebd-a89f-85d18e886e6d} -w . -a .\Cad_Components_Directory -l  CADAssembly.xml.log -p -s localhost:4949
//                 
// Pre-Conditions:
//           in_argc  > 0
//           in_argv pointer to a valid array containing in_argc elements
//           
//
// Post-Conditions
//           throw isis::application_exception if any of the following conditions occur
//                  -w, -l are not present
//                  key is repeated (however, enabling -p, -g, -gp, and -pg in any combination is allowed)
//                  -h is present,  for this case the exception must contain the usage information
//
//           If no exceptions, return populated out_ProgramInputArguments
//              If the file/directory names were enclosed in double-quotes, those double-quotes are removed and not preserved in the string
//                    
void ParseInputArguments(  int                        in_argc,
                           const char                 * const in_argv[],
                           ProgramInputArguments      &out_ProgramInputArguments );


void CheckInputArguments ( const ProgramInputArguments   &in_ProgramInputArguments );


void ThrowExecption_If_InvalidInputArguments(   int                        in_argc,
												const char                 * const in_argv[],
												const ProgramInputArguments &in_ProgramInputArguments)
																			throw (isis::application_exception);


}

#endif