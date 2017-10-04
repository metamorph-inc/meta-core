// File: ParseInputARguments.cpp
// Author: Kevin Zeillmann
// Last Modified: 9/28/2017
// Description: This file provides the definition of the ProgramInputArguments 
//   operator<< function to let it output debug information, and it contains
//   the definition of the ParseInputArguments function for use in the 
//   CADCreoParametricCreateAssembly program.

//#include "stdafx.h"
#include "cc_CreateAssemblyInputArgumentsParser.h"
#include "cc_AssemblyOptions.h"

namespace isis
{

//There might be a better way to do this using Boost's fusion library, but this works
//  for now, since this code will likely not be modular and used for other programs
// Overriding the << operator allows us to print the object to cout or any other
//  useful output stream for debugging
std::ostream& operator<<(std::ostream & in_stream, const CreateAssemblyInputArguments &in_args)
{
	in_stream << "graphicsModeOn = "		<< in_args.graphicsModeOn << std::endl;
	in_stream << "inputXmlFileName = "		<< in_args.inputXmlFileName << std::endl;
	in_stream << "workingDirectory = "		<< in_args.workingDirectory << std::endl;
	in_stream << "logFileName = "			<< in_args.logFileName << std::endl;
	in_stream << "logVerbosity = "      	<< in_args.logVerbosity << std::endl;
	in_stream << "logConfig = "       		<< in_args.logConfigFile << std::endl;
	in_stream << "promptBeforeExiting = "	<< in_args.promptBeforeExiting << std::endl;
	in_stream << "auxiliaryCADDirectory = " << in_args.auxiliaryCADDirectory << std::endl;
	return in_stream;
}
  
void CreateAssemblyInputArguments::ParseInputArguments(  
							int								in_argc,
							const char						* const in_argv[] )
                                                            
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
		auxiliaryCADDirectory = "";
		graphicsModeOn = false;
		promptBeforeExiting = false;

		// We begin by making options groups. Options groups allow us to specify 
		//  the different kinds of command line options that are available for 
		//  the caller. We'll make three groups: visibleOptions, groupedOptions,
		//  and allOptions. 

		//Options visible to the user in the help message.
		//If you want to define new flags, you can do so here. 
		//Begin with the name of the flag, then the value it expects (a single token here), 
		// and then the description that will be included in for generated help documentation

		visibleOptions.add_options()
			("w", po::value<std::string>(),				" (Optional) Default (\".\"),  Working directory.  The directory where the generated files would be persisted.")
			("i", po::value<std::string>()->required(), " (Required) Input XML file name.  This file (e.g. CADAssembly.xml) defines the CAD assembly definition.")
			("l", po::value<std::string>(),				" (Optional) Default (\"cad-assembler.log\"), Log file name.")
			("v", po::value<std::string>(),				" (Optional) Default (\"inform\"), Log vebosity level {\"debug\" \"inform\" \"warning\" \"error\"}.")
			("c", po::value<std::string>(),				" (Optional) Default (\"\"), Log configuration file name.  Currently not used. Reserved for future use.")
			("a", po::value<std::string>(),				" (Optional) Default (\"\"), Auxiliary CAD directory.  Would contain additional CAD parts that are not in component directories.")
			("p",										" (Optional) Default (no prompting), Prompt before exiting.")
			("g",										" (Optional) Default (no graphics), Graphics mode")
			("h",										" (Optional) Help - displays keys along with the usage")
		;
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
		po::notify(vm); 
        
		// Since we're sure that we'll get a valid string for each of these,
		//  we can insert these values directly into the ProgramInputArguments object
		if(vm.count("w")) {
		  workingDirectory = vm["w"].as<std::string>();
		}


		// Parameters related to logging.
		if(vm.count("l")) {
		  logFileName = vm["l"].as<std::string>();
		}
		if(vm.count("v")) {
		  std::string lv = vm["v"].as<std::string>();
		  if (boost::iequals(lv, LOG_VERBOSITY_DEBUG)) {
			  logVerbosity = 	isis_DEBUG;
		  } else if (boost::iequals(lv, LOG_VERBOSITY_INFO)) {
			  logVerbosity = isis_INFO;
		  } else if (boost::iequals(lv, LOG_VERBOSITY_WARN)) {
			  logVerbosity = isis_WARN;
		  } else if (boost::iequals(lv, LOG_VERBOSITY_ERROR)) {
			  logVerbosity = isis_ERROR;
		  } else {
			  std::cerr << "The log level is not recognized.  Setting log level to INFO." << std::endl;
			  logVerbosity = isis_INFO;
		  } 

		}
		if(vm.count("c")) {
		  logConfigFile = vm["c"].as<std::string>();
		}
		
		
		// Now we test if a, p, or g were passed in as arguments
		// Again, we insert these directly into the ProgramInputArguments object
		if(vm.count("i"))
		{
			inputXmlFileName = vm["i"].as<std::string>();	
		}

		if(vm.count("a"))
		{
			auxiliaryCADDirectory = vm["a"].as<std::string>();
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
		std::cerr << "could not process command line correctly" << std::endl;
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
		std::cerr << "could not process command line correctly " << errorString << std::endl;
		throw isis::application_exception(errorString.c_str());
	}

}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void CreateAssemblyInputArguments::ThrowExecption_If_InvalidInputArguments(   
													int						in_argc,
													const char* const		in_argv[])
																					throw (isis::application_exception)
	{

		// Note - Errors could occur for the following reasons;
		//		1. in_argv does not contain sufficient information
		//		2. The default values set by the CreateAssemblyInputArguments constructor have been changed.
		// In either case, we need to catch errors before proceeding

		if ( inputXmlFileName.length() == 0 )
		{		
			std::cout << *this;
			std::stringstream errorString;
			errorString << "Function - " << __FUNCTION__ << ", " << std::endl <<
					"Input Arguments: " << std::endl;
			for ( int i = 0; i < in_argc; ++i) errorString << in_argv[i] << std::string(" ");
			errorString << std::endl << "inputXmlFileName not set." ;
			throw isis::application_exception(errorString.str());
		}

		if ( workingDirectory.length() == 0 )
		{		
			std::cout << *this;
			std::stringstream errorString;
			errorString << "Function - " << __FUNCTION__ << ", " << std::endl <<
						    std::endl << "workingDirectory not set." ;
			throw isis::application_exception(errorString.str());
		}

		if ( logFileName.length() == 0 )
		{		
			std::cout << *this;
			std::stringstream errorString;
			errorString << "Function - " << __FUNCTION__ << ", " << std::endl <<
						    std::endl << "logFileName not set." ;
			throw isis::application_exception(errorString.str());
		}


//		if ( in_ProgramInputArguments.synchronizeWithCyPhy && in_ProgramInputArguments.designID.size() == 0 )
//		{
//			std::cout << in_ProgramInputArguments;
//			std::stringstream errorString;
//			errorString << "Input Arguments: " << std::endl;
//			for ( int i = 0; i < in_argc; ++i) errorString << in_argv[i] << std::string(" ");
//			errorString << std::endl << "Input arguments error, if -s  present then -d must also be present." ;
//			throw isis::application_exception(errorString.str());
//		}
	}


} // END namespace isis