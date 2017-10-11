#ifndef CC_CREATE_ASSEMBLY_INPUT_ARGUMENTS_PARSER
#define CC_CREATE_ASSEMBLY_INPUT_ARGUMENTS_PARSER

// File: ParseInputArguments.h
// Author: Kevin Zeillmann
// Last Modified:  9/28/2017
// Description: This file provides the struct used to parse the input arguments,
//  defines the isis::application_exception (to make it work with the tester file,
//  and the prototype function header for the ParseInputArguments file.
#include "isis_application_exception.h"
#include <boost/program_options.hpp>
#include <boost/algorithm/string.hpp>

#include <iostream>
#include "cc_LoggerBoost.h"
#include "cc_ParserCommon.h"


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


/***
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
****/

// Description: This struct contains the names of the files, directories, and options 
//  needed to run the CADCreoParametricCreateAssembly.exe application. These data 
//  members are taken from the input arguments for the application and parsed in 
//  the ParseInputArguments function.
//
class CreateAssemblyInputArguments
{

public:
	bool					graphicsModeOn;			//  -g  Default false
	std::string				inputXmlFileName;       //  -i
	std::string				workingDirectory;		//	-w  Default   "."
	std::string				logFileName;			//  -l  Default "cad-assembler.log" 
	isis_LogSeverityLevel	logVerbosity;			//  -v  Default isis_INFO	
	std::string				logConfigFile;          //  -c
	bool					promptBeforeExiting;    //  -p  Default false
	std::string				auxiliaryCADDirectory;	//  -a  Default ""
 
	CreateAssemblyInputArguments(): graphicsModeOn(false), promptBeforeExiting(false), 
		   logFileName("cad-assembler.log"), logVerbosity(isis_INFO), logConfigFile(""),
		   workingDirectory("."), auxiliaryCADDirectory("") {};

	bool is_designMode();
	bool is_componentMode();
	bool is_passiveMode();

	// Allows us to print debug information for the class
	friend std::ostream& operator<<(std::ostream & in_stream, const CreateAssemblyInputArguments &in_args);


	// Description:
	//           Parse the input arguments to this program and populate out_ProgramInputArguments
	//           Keys
	//		("w", po::value<std::string>(),				" (Optional) Default (\".\"), Working directory.  The directory where the generated files would be persisted.")
	//		("i", po::value<std::string>()->required(), " (Required), Input XML file name.  This file (e.g. CADAssembly.xml) defines the CAD assembly definition.")
	//		("l", po::value<std::string>(),				" (Optional) Default (\"cad-assembler.log\"), Log file name.")
	//		("v", po::value<std::string>(),				" (Optional) Default (\"inform\"), Log vebosity level {\"debug\" \"inform\" \"warning\" \"error\"}.")
	//		("c", po::value<std::string>(),				" (Optional) Default (\"\"), Log configuration file name.  Currently not used. Reserved for future use.")
	//		("a", po::value<std::string>(),				" (Optional) Default (\"\"), Auxiliary CAD directory.  Would contain additional CAD parts that are not in component directories.")
	//		("p",										" (Optional) Default (no prompting), Prompt before exiting.")
	//		("g",										" (Optional) Default (no graphics), Graphics mode")
	//		("h",										" (Optional) Help - displays keys along with the usage")
	//
	//           Key Order: No particular order required
	//           Key Grouping:  -p and -g may be grouped (e.g. -pg)
	//           Keys are case insensitive.
	//
	//           File/Path String Contents
	//           Key          File_Name    Full_Path_And_File_Name    Relative_Path_And_File_Name   Contain_Spaces (Must be enclosed in double quotes)
	//           -w           N/A                 Yes                 Yes                              Yes
	//           -i           Yes                 Yes                 Yes                              Yes
	//           -a           N/A                 Yes                 Yes                              Yes
	//           -l           Yes                 Yes                 Yes                              Yes
	//
	//           Example Invocations:
	//					CADCreoParametricCreateAssembly.exe -i CADAssembly.xml 
	//                  CADCreoParametricCreateAssembly.exe  -i CADAssembly.xml  -w . -a .\Cad_Components_Directory -l  CADAssembly.xml.log -p
	//                  CADCreoParametricCreateAssembly.exe  -w "C:\Temp\aaa_Regression_Models\01_IFV\IFV Complete Assembly" -a "C:\Temp\aaa_Regression_Models\CAD_Lib\01_IFV\All Components" -i CADAssembly.xml -l IFV_cfg125_Cad.xml.log -gp
	//
	//					CADJobDriver.py  
	//						create_asm = os.path.join(cad_library.META_PATH, 'bin', 'CAD', 'Creo', 'bin', 'CADCreoParametricCreateAssembly.exe')
	//						result = os.system('\"' + create_asm + '" -i CADAssembly.xml')
	//
	// Pre-Conditions:
	//           in_argc  > 0
	//           in_argv pointer to a valid array containing in_argc elements
	//           
	//
	// Post-Conditions
	//           throw isis::application_exception if any of the following conditions occur
	//                 ...()->required() argument not specified  ( see above e.g. ("i", po::value<std::string>()->required())
	//                 key is repeated (however, enabling -p, -g, -gp, and -pg in any combination is allowed)
	//                 -h is present,  for this case the exception would contain the usage information
	//
	//           If no exceptions, return populated out_ProgramInputArguments
	//              If the file/directory names were enclosed in double-quotes, those double-quotes are removed and not preserved in the string
	//   
	void ParseInputArguments(  int                      in_argc,
								const char              * const in_argv[]);

	void ThrowExecption_If_InvalidInputArguments(   int                        in_argc,
													const char                 * const in_argv[])
																				throw (isis::application_exception);
};

   
}

#endif