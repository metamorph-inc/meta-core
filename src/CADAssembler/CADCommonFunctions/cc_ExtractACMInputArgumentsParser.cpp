// File: ParseInputARguments.cpp
// Author: Kevin Zeillmann
// Last Modified: 9/28/2017
// Description: This file provides the definition of the ProgramInputArguments 
//   operator<< function to let it output debug information, and it contains
//   the definition of the ParseInputArguments function for use in the 
//   CADCreoParametricCreateAssembly program.
//
// UPDATE Modified: 19/28/2017
// Description: John Gawel copied and modified this file for the ExtractACM-XMLfromCreoModels program 

#include "cc_ExtractACMInputArgumentsParser.h"
#include <boost/filesystem.hpp>

namespace isis
{

//There might be a better way to do this using Boost's fusion library, but this works
//  for now, since this code will likely not be modular and used for other programs
// Overriding the << operator allows us to print the object to cout or any other
//  useful output stream for debugging
std::ostream& operator<<(std::ostream & in_stream, const ExtractACMInputArguments &in_args)
{
	in_stream << "graphicsModeOn = "		<< in_args.graphicsModeOn << std::endl;
	in_stream << "inputCADFileName = "		<< in_args.inputCADFileName << std::endl;
	in_stream << "outputXmlFileName = "		<< in_args.outputXmlFileName << std::endl;
	in_stream << "workingDirectory = "		<< in_args.workingDirectory << std::endl;
	in_stream << "promptBeforeExiting = "	<< in_args.promptBeforeExiting << std::endl;
	return in_stream;
}

void ExtractACMInputArguments::ParseInputArguments(  int                        in_argc,
													const char                 * const in_argv[])
													throw (isis::application_exception)
{
	//Create a local alias for the boost::program_options namespace to prevent us
	// from having to use "boost::program_options" too often and cluttering up
	// the program. It's easier to use a 2 character name than a 22 character name
	namespace po = boost::program_options;
	po::options_description visibleOptions("Allowed options");
	
	try {
		// We want to ensure that out_ProgramInputArguments will contain the correct
		//  info when we pass it back to the caller. The following code is in place 
		//  just in case the ProgramInputArguments object was used in another
		//  function or had already been through this function.
		// The object is wiped clean of its old data, except for the required options
		//  which are always set later. The optional fields need to be wiped clean.


		// If our program options increase, it might be best to turn ProgramInputArguments
		//  into a class, and include a reset() method that will do this work for us. 
		outputXmlFileName = "";

		// We begin by making options groups. Options groups allow us to specify 
		//  the different kinds of command line options that are available for 
		//  the caller. We'll make three groups: visibleOptions, groupedOptions,
		//  and allOptions. 

		//Options visible to the user in the help message.
		//If you want to define new flags, you can do so here. 
		//Begin with the name of the flag, then the value it expects (a single token here), 
		// and then the description that will be included in for generated help documentation

		visibleOptions.add_options()
			("c", po::value<std::string>()->required(), " (Required) Input CAD file name.  This file defines the CAD assembly definition.")
			("x", po::value<std::string>()->required(), " (Required) Output XML file name.  This file defines the CAD assembly definition.")
			("w", po::value<std::string>(),				" (Optional) Default (\".\"),  Working directory.  The directory where the generated files would be persisted.")
			("l", po::value<std::string>(),				" (Optional) Default (\"ExtractACMInputArguments.log\"), Log file name.")
			("p",                                       " (Optional) Prompt before exiting.  Not prompting is the default.")
			("g",                                       " (Optional) Graphics mode.  No graphics is the default.")
			("h",                                       " Help - displays keys along with the usage")
		;
		// Boost's program_options library is nice in that it allows certain arguments to be
		//  required. We have done that for w, i, and l. If these arguments are not included,
		//  the notify(vm) method throws an exception.

		// This seems to be the best way of accomplishing key grouping. We want to allow
		//  the user to group "p" and "g" together on the command line so that they can
		//  use "-gp" or "-pg" to enable both the "-g" and "-p" options. 
		//  Boost doesn't support this easily, so it seemed best to use brute force. 
		//  If there were more options to group, I would probably need to run the arguments
		//  through another parser first before letting Boost use them.
		// We don't want to allow these arguments to be displayed to the user - only
		//  the arguments in visibleOptions should be displayed in the help message. 
		po::options_description groupedOptions("Options for use with key grouping");
		groupedOptions.add_options()
			("pg", "Enables graphics mode and prompts before exiting")
			("gp", "Enables graphics mode and prompts before exiting")	
        ;

		// We'll add both groups to allOptions. 
		po::options_description allOptions("Allowed options");
		allOptions.add(groupedOptions).add(visibleOptions);
            
			
		// This data structure will store our options used 
        po::variables_map vm;

		// There are traditionally two ways to do command line options. One way is with the
		//  long option (--help), and another is with the short version, which uses only one dash
		//  and an initial of the long version it represents (-h). Boost prefers long options, so the 
		//  current implementation uses long options with a single character to represent each 
		//  option. With the "allow_long_disguise" flag, we can use only a single dash on these
		//  long options. The combination of these two allows us to make a "short option" which
		//  is internally represented as a long option. This is more of a side effect
		//  of the hack than an intended feature. You can call the application using two
		//  dashes, but the short, one-dash version is preferred. 
		// Of course, the case_insensitive style allows for case_insensitive options.

		po::store(po::command_line_parser(in_argc, in_argv).options(allOptions).
			style(po::command_line_style::default_style | 
			po::command_line_style::case_insensitive | 
			po::command_line_style::allow_long_disguise).run(), vm);
        

		// Boost library functions ensure that:
		//   1) we don't have any duplicate arguments
		//   2) we have the three required arguments (w, i, l)
		// These are enforced when we call the notify function later.
		// First test if the "help" argument was called
        if (vm.count("h"))
		{
            std::stringstream stream;
			// Earlier, we created two option groups, groupedOptions and visibleOptions, and 
			//  added them to allOptions, which was parsed. Doing this allows us to only print
			//  the visible options when the user wants the help information.
			visibleOptions.print(stream);
			std::string errorString = stream.str();
			// Since the Boost library thinks of these as long options, we
			//  should remove the double dashes from the help information
			//  so that we don't confuse the user
			boost::algorithm::replace_all(errorString, "--", "-");
			throw isis::application_exception(errorString.c_str());
        }

		// This function raises exceptions if our required arguments (w, i, l) were not passed.
		// Normally this call to notify would occur immediately after po::store, so that we 
		//  can detect errors as soon as possible. However, in this case it's better to 
		//  wait until we test if the "-h" argument was passed. This way, we can use the argument
		//  without having to enter in a value for w or l. 
//		po::notify(vm); 
        
		// Since we're sure that we'll get a valid string for required fields,
		//  we can insert these values directly into the ProgramInputArguments object
		inputCADFileName = vm["c"].as<std::string>();

		// Now we test if x, w, p, or g were passed in as arguments
		// Again, we insert these directly into the ProgramInputArguments object
		if(vm.count("x"))
		{
			outputXmlFileName = vm["x"].as<std::string>();	
		}
		if(vm.count("w"))
		{
			workingDirectory = vm["w"].as<std::string>();
		}
		else
		{
			// if a specific working directory is not passed in, use the directory of the part file
			std::string fullpath = inputCADFileName;

			// check for slash going both ways
			int endIdx = fullpath.rfind('\\');
			if (endIdx == -1)
				endIdx = fullpath.rfind('/');

			workingDirectory = fullpath.substr(0, endIdx);
			inputCADFileName = fullpath.substr(endIdx+1);
		}

		if(vm.count("l")) {
		  logFileName = vm["l"].as<std::string>();
		}

		if(vm.count("p"))
		{
			promptBeforeExiting = true;
		}
		if(vm.count("g"))
		{
			graphicsModeOn = true;
		}
		if(vm.count("pg") || vm.count("gp"))
		{
			graphicsModeOn = true;
			promptBeforeExiting = true;
		}
    }
	catch(isis::application_exception &e)
	{
		throw isis::application_exception(e);
	}
	catch(boost::program_options::error& e)
	{
		std::string errorString = e.what();
		// Add input line
		errorString += "\n" + std::string("Input Line: ");
		for ( int i = 0; i < in_argc; ++i) errorString += in_argv[i] + std::string(" ");

		// Add help information
        std::stringstream stream;
		visibleOptions.print(stream);
		errorString += "\n";
		errorString += stream.str();
		//Again, this is internally represented as long options
		// so we should remove the double dashes from the 
		// debug information.

		boost::algorithm::replace_all(errorString, "--", "-");
		throw isis::application_exception(errorString.c_str());
	}

}

	void ExtractACMInputArguments::ThrowExecption_If_InvalidInputArguments(   
									int                        in_argc,
									const char                 * const in_argv[])
																	throw (isis::application_exception)
	{
	}

} // END namespace isis