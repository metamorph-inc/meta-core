#ifndef CC_EXTRACT_ACM_INPUT_ARGUMENTS_PARSER_H
#define CC_EXTRACT_ACM_INPUT_ARGUMENTS_PARSER_H

// File: ParseInputArguments.h
// Author: Kevin Zeillmann
// Last Modified: 9/28/2017
// Description: This file provides the struct used to parse the input arguments,
//  defines the isis::application_exception (to make it work with the tester file,
//  and the prototype function header for the ParseInputArguments file.
#include "isis_application_exception.h"
#include <boost/program_options.hpp>
#include <boost/algorithm/string.hpp>

#include <iostream>


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



// Description: This struct contains the names of the files, directories, and options 
//  needed to run the CADCreoParametricCreateAssembly.exe application. These data 
//  members are taken from the input arguments for the application and parsed in 
//  the ParseInputArguments function.
//
class ExtractACMInputArguments
{
public:

    bool                graphicsModeOn;            // -g    default false
    std::string         outputXmlFileName;         // -x 
    std::string         inputCADFileName;          // -c
    std::string         workingDirectory;          // -w    default "."
    std::string         logFileName;               // -l	default "ExtractACMInputArguments.log"
    bool                promptBeforeExiting;       // -p    default false
 
	ExtractACMInputArguments(): graphicsModeOn(false), promptBeforeExiting(false), workingDirectory("."), logFileName("ExtractACMInputArguments.log") {};

	// Allows us to print debug information for the class
	friend std::ostream& operator<<(std::ostream & in_stream, const ExtractACMInputArguments &in_args);

	// Description:
	//		("c", po::value<std::string>()->required(), " (Required) Input CAD file name.  This file defines the CAD assembly definition.")
	//		("x", po::value<std::string>()->required(), " (Required) Output XML file name.  This file defines the CAD assembly definition.")
	//		("w", po::value<std::string>(),				" (Optional) Default (\".\"),  Working directory.  The directory where the generated files would be persisted.")
	//		("l", po::value<std::string>(),				" (Optional) Default (\"ExtractACMInputArguments.log\"), Log file name.")
	//		("p",                                       " (Optional) Prompt before exiting.  Not prompting is the default.")
	//		("g",                                       " (Optional) Graphics mode.  No graphics is the default.")
	//		("h",                                       " Help - displays keys along with the usage")
	//
	//       Example Invocations:
	//			
	//			ExtractACM-XMLfromCreoModels.exe -c "D:\temp\Test_Models\Component_Authoring\CAD_Models\component_mount_plate.prt.9" -x C:\Users\rowens\AppData\Local\Temp\tmpBE7D.tmp
	//                 
	// Pre-Conditions:
	//           in_argc  > 0
	//           in_argv pointer to a valid array containing in_argc elements
	//           
	//
	// Post-Conditions
	//           throw isis::application_exception if any of the following conditions occur
	//                ...()->required() argument not specified  ( see above e.g. po::value<std::string>()->required())
	//
	//           If no exceptions, return populated out_ProgramInputArguments
	//              If the file/directory names were enclosed in double-quotes, those double-quotes are removed and not preserved in the string
	//                    
	void ParseInputArguments(	int                        in_argc,
								const char                 * const in_argv[] )
								throw (isis::application_exception);



	void ThrowExecption_If_InvalidInputArguments(   int                        in_argc,
													const char                 * const in_argv[])
																				throw (isis::application_exception);

};




}

#endif