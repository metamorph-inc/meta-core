#ifndef CC_METALINK_INPUT_ARGUMENTS_PARSER
#define CC_METALINK_INPUT_ARGUMENTS_PARSER

// File: ParseInputArguments.h
// Author: Kevin Zeillmann
// Last Modified:  9/28/2017
// Description: This file provides the struct used to parse the input arguments,
//  defines the isis::application_exception (to make it work with the tester file,
//  and the prototype function header for the ParseInputArguments file.
#include "isis_application_exception.h"
#include <boost/program_options.hpp>
#include <boost/algorithm/string.hpp>
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


const std::string MAJOR_MODE_DESIGN = "design";
const std::string MAJOR_MODE_COMPONENT = "component";
const std::string MAJOR_MODE_PASSIVE = "passive";

/****
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
class MetaLinkInputArguments
{
public:
    bool					graphicsModeOn;				//  -g  Default false
    std::string				inputXmlFileName;           //  -i   ??? Why is this needed ???
    std::string				workingDirectory;           //	-w  Default   "."
    std::string				logFileName;                //  -l  MetaLink.log    
	isis_LogSeverityLevel	logVerbosity;				//  -v  Default isis_INFO			
    std::string				auxiliaryCADDirectory;		//  -a 
    bool					synchronizeWithCyPhy;		//  -s  Default false, -s will set this to true
    std::string				majorMode;					//  -m
    std::string				syncConnectionString;		//  -s 
    std::string				designID;					//  -d
    std::string				configPro;					//  -c
	std::string				instanceID;					//  -id


    MetaLinkInputArguments(): graphicsModeOn(false),
		logFileName("MetaLink.log"), logVerbosity(isis_INFO), workingDirectory("."),
        auxiliaryCADDirectory(""), synchronizeWithCyPhy(false), majorMode(MAJOR_MODE_DESIGN) {};

    bool is_designMode() const;
    bool is_componentMode() const;
    bool is_passiveMode() const;

    // Allows us to print debug information for the class
    friend std::ostream& operator<<(std::ostream & in_stream, const MetaLinkInputArguments &in_args);

	// Description:
	//           Parse the input arguments to this program and populate out_ProgramInputArguments
	//           Keys
	//	("w",  po::value<std::string>(),			" (Optional) Default (\".\"),  Working directory.  The directory where the generated files would be persisted.")
	//	("i",  po::value<std::string>(),            " (Optional) Input XML file name.  This file (e.g. CADAssembly.xml) defines the CAD assembly definition.")
	//	("l",  po::value<std::string>(),			" (Optional) Default (\"MetaLink.log\"), Log file name.")
	//	("v",  po::value<std::string>(),			" (Optional) Default (\"inform\"), Log vebosity level {\"debug\" \"inform\" \"warning\" \"error\"}.")
	//	("a",  po::value<std::string>(),			" (Optional) Default (\"\"), Auxiliary CAD directory.  Would contain additional CAD parts that are not in component directories.")
    //  ("m",  po::value<std::string>(),			" (Optional) Major mode in { \"design\" \"component\" \"passive\" }.")
	//	("c",  po::value<std::string>(),			" (Optional) Name of file to concat after generated config.pro")
	//	("id", po::value<std::string>()->required()," (Required) Synced component instance ID.")
    //  ("s",  po::value<std::string>()->required()," (Required) Connection string (e.g. localhost:4949)"
	//												" The optional connection string (e.g. localhost:4949). Not synchronized is the default.")
	//	("g",										" (Optional) Default (no graphics), Graphics mode.")
    //  ("d", po::value<std::string>()->required(),	" (Required) Object ID. For assemblies the DesignID identifies a CyPhy design. \n"
	//												" For component mode this is the AVMComponentID. \n"
	//												" This is used by MetaLink to associate a Creo session with a CyPhy object. \n"
	//												" In the case of passive mode the object identifier is meaningless.")
	//	("o", po::value<std::string>(),             " (Optional) Assembly Options (e.g. fullregen=1, 1 full assembly regen after adding each part/sub-assembly) related to assembly here.")
	//	("h",										" (Optional) Help - displays keys along with the usage")																		c
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
    //			 Assembly:
	//				C:\Program Files (x86)\META\bin\CAD\Creo\bin\CADCreoParametricMetaLink.exe -v debug -g -s 127.0.0.1:15150 
	//					-w D:\temp\Test_Models\Bracket_Solid_Model\MetaLink_0913105703 -l D:\temp\Test_Models\Bracket_Solid_Model\log\MetaLink_CreoCreateAssembly_u43mgh4o.log 
	//					-id 48800000 -d 766270dc-49da-4f9a-8f0f-31c85677bb13 
	//
	//				C:\Program Files (x86)\META\bin\CAD\Creo\bin\CADCreoParametricMetaLink.exe -v debug -g -s 127.0.0.1:15150 
	//					-w D:\temp\01_Example_Models\Excavator\MetaLink_1025155231 -l D:\temp\01_Example_Models\Excavator\log\MetaLink_CreoCreateAssembly_m4txameu.log 
	//					-o fullregen=1, -id 181200000 -d aa2f48c6-d47d-4d9d-8344-dfa4f87f803e 
	//
	//			 Part:
	//				C:\Program Files (x86)\META\bin\CAD\Creo\bin\CADCreoParametricMetaLink.exe -v debug -g -s 127.0.0.1:15150 
	//					-w D:\temp\Test_Models\Bracket_Solid_Model\components\jthwsrlu\CAD 
	//					-l D:\temp\Test_Models\Bracket_Solid_Model\log\MetaLink_CreoCreateAssembly_giifaq1o.log  -id 676800000 -d d6932cc3-c8eb-49e3-8601-116873b8e717 -m component 
	//
	// Pre-Conditions:
	//           in_argc  > 0
	//           in_argv pointer to a valid array containing in_argc elements
	//
	//
	// Post-Conditions
	//           throw isis::application_exception if any of the following conditions occur
	//                   ...()->required() argument not specified  ( see above e.g. ("m",  po::value<std::string>()->required())
	//                  key is repeated (however, enabling -p, -g, -gp, and -pg in any combination is allowed)
	//                  -h is present,  for this case the exception must contain the usage information
	//
	//           If no exceptions, return populated out_ProgramInputArguments
	//              If the file/directory names were enclosed in double-quotes, those double-quotes are removed and not preserved in the string

    void ParseInputArguments(int                        in_argc,
                             const char                 * const in_argv[]);


	void ThrowException_If_InvalidInputArguments(	int                        in_argc,
													const char                 * const in_argv[] )
																throw(isis::application_exception);
};



}

#endif