// File: ParseInputARguments.cpp
// Author: Kevin Zeillmann
// Last Modified: 6/3/13
// Description: This file provides the definition of the ProgramInputArguments 
//   operator<< function to let it output debug information, and it contains
//   the definition of the ParseInputArguments function for use in the 
//   CADCreoParametricCreateAssembly program.

#include "stdafx.h"
#include "InputArgumentsParser.h"
#include "AssemblyOptions.h"

namespace isis
{

//There might be a better way to do this using Boost's fusion library, but this works
//  for now, since this code will likely not be modular and used for other programs
// Overriding the << operator allows us to print the object to cout or any other
//  useful output stream for debugging
std::ostream& operator<<(std::ostream & in_stream, const ProgramInputArguments &in_args)
{
	in_stream << "graphicsModeOn = "		<< in_args.graphicsModeOn << std::endl;
	in_stream << "inputXmlFileName = "		<< in_args.inputXmlFileName << std::endl;
	in_stream << "workingDirectory = "		<< in_args.workingDirectory << std::endl;
	in_stream << "logFileName = "			<< in_args.logFileName << std::endl;
	in_stream << "logVerbosity = "      	<< in_args.logVerbosity << std::endl;
	in_stream << "logConfig = "       		<< in_args.logConfigFile << std::endl;
	in_stream << "promptBeforeExiting = "	<< in_args.promptBeforeExiting << std::endl;
	in_stream << "auxiliaryCADDirectory = " << in_args.auxiliaryCADDirectory << std::endl;
	in_stream << "synchronizeWithCyPhy = "	<< in_args.synchronizeWithCyPhy << std::endl;
	in_stream << "syncConnectionString = "	<< in_args.syncConnectionString << std::endl;
	in_stream << "designID = "				<< in_args.designID << std::endl;
	in_stream << "majorMode = "				<< in_args.majorMode << std::endl;
	return in_stream;
}
  
bool ProgramInputArguments::is_designMode() {
	return this->majorMode == MAJOR_MODE_DESIGN;
}

bool ProgramInputArguments::is_componentMode() {
	return this->majorMode == MAJOR_MODE_COMPONENT;
}

bool ProgramInputArguments::is_passiveMode() {
	return this->majorMode == MAJOR_MODE_PASSIVE;
}

void ParseInputArguments(  int                        in_argc,
                           const char                 * const in_argv[],
                           ProgramInputArguments      &out_ProgramInputArguments )
                                                            
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
		out_ProgramInputArguments.auxiliaryCADDirectory = "";
		out_ProgramInputArguments.graphicsModeOn = false;
		out_ProgramInputArguments.promptBeforeExiting = false;
		out_ProgramInputArguments.synchronizeWithCyPhy = false;
		out_ProgramInputArguments.syncConnectionString = "";
		out_ProgramInputArguments.designID = "";
		out_ProgramInputArguments.designID = MAJOR_MODE_DESIGN;

		// We begin by making options groups. Options groups allow us to specify 
		//  the different kinds of command line options that are available for 
		//  the caller. We'll make three groups: visibleOptions, groupedOptions,
		//  and allOptions. 

		//Options visible to the user in the help message.
		//If you want to define new flags, you can do so here. 
		//Begin with the name of the flag, then the value it expects (a single token here), 
		// and then the description that will be included in for generated help documentation

		visibleOptions.add_options()
			("o", po::value<std::string>(), " Assembly Options. Specify options related to assembly here.")
			("w", po::value<std::string>(), " Working directory.  The directory where the generated files would be persisted.")
			("i", po::value<std::string>(), " Input XML file name.  This file defines the CAD assembly definition.")
			("l", po::value<std::string>(), " Log file name.")
			("v", po::value<std::string>(), " Log vebosity level {\"debug\" \"info\" \"warn\" \"error\"}.")
			("c", po::value<std::string>(), " Log configuration file name.")
			("a", po::value<std::string>(), " Auxiliary CAD directory.  Would contain CAD parts.")
			("m", po::value<std::string>(), " Major mode in { \"design\" \"component\" \"passive\" }.")
			("s", po::value<std::string>(), " Synchronize Creo with CyPhy.  "
			                                                  "The optional connection string (e.g. localhost:4949). Not synchronized is the default.")
			("p",                           " Prompt before exiting.  Not prompting is the default.")
			("g",                           " Graphics mode.  No graphics is the default.")
			("d", po::value<std::string>(), " Object ID. For assemblies the DesignID identifies a CyPhy design. \n"
			                                "For component mode this is the AVMComponentID. \n"
											"This is used by MetaLink to associate a Creo session with a CyPhy object. \n"
											"In the case of passive mode the object identifier is meaningless.")
			("h",                           "Help - displays keys along with the usage")
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
		  out_ProgramInputArguments.workingDirectory = vm["w"].as<std::string>();
		} else {
		  out_ProgramInputArguments.workingDirectory = ".";
		}

		// Parameters related to logging.
		if(vm.count("l")) {
		  out_ProgramInputArguments.logFileName = vm["l"].as<std::string>();
		}
		if(vm.count("v")) {
		  std::string lv = vm["v"].as<std::string>();
		  if (boost::iequals(lv, LOG_VERBOSITY_DEBUG)) {
			  out_ProgramInputArguments.logVerbosity = log4cpp::Priority::DEBUG;
		  } else if (boost::iequals(lv, LOG_VERBOSITY_INFO)) {
			  out_ProgramInputArguments.logVerbosity = log4cpp::Priority::INFO;
		  } else if (boost::iequals(lv, LOG_VERBOSITY_WARN)) {
			  out_ProgramInputArguments.logVerbosity = log4cpp::Priority::WARN;
		  } else if (boost::iequals(lv, LOG_VERBOSITY_ERROR)) {
			  out_ProgramInputArguments.logVerbosity = log4cpp::Priority::ERROR;
		  } else if (boost::iequals(lv, LOG_VERBOSITY_CRIT)) {
			  out_ProgramInputArguments.logVerbosity = log4cpp::Priority::CRIT;
		  } else if (boost::iequals(lv, LOG_VERBOSITY_ALERT)) {
			  out_ProgramInputArguments.logVerbosity = log4cpp::Priority::ALERT;
		  } else if (boost::iequals(lv, LOG_VERBOSITY_FATAL)) {
			  out_ProgramInputArguments.logVerbosity = log4cpp::Priority::FATAL;
		  } else if (boost::iequals(lv, LOG_VERBOSITY_EMERG)) {
			  out_ProgramInputArguments.logVerbosity = log4cpp::Priority::EMERG;
		  } else if (boost::iequals(lv, LOG_VERBOSITY_NOTSET)) {
			  out_ProgramInputArguments.logVerbosity = log4cpp::Priority::NOTSET;
		  } else {
			  std::cerr << "The log level is not recognized" << std::endl;
			  out_ProgramInputArguments.logVerbosity = log4cpp::Priority::NOTSET;
		  } 

		}
		if(vm.count("c")) {
		  out_ProgramInputArguments.logConfigFile = vm["c"].as<std::string>();
		}
		
		
		// Now we test if a, p, or g were passed in as arguments
		// Again, we insert these directly into the ProgramInputArguments object
		if(vm.count("i"))
		{
			out_ProgramInputArguments.inputXmlFileName = vm["i"].as<std::string>();	
		}

		if(vm.count("d"))
		{
			out_ProgramInputArguments.designID = vm["d"].as<std::string>();
		}
		if(vm.count("a"))
		{
			out_ProgramInputArguments.auxiliaryCADDirectory = vm["a"].as<std::string>();
		}
		if(vm.count("p"))
		{
			out_ProgramInputArguments.promptBeforeExiting = true;
		}
		if(vm.count("g"))
		{
			out_ProgramInputArguments.graphicsModeOn = true;
		}
		if(vm.count("pg") || vm.count("gp"))
		{
			out_ProgramInputArguments.graphicsModeOn = true;
			out_ProgramInputArguments.promptBeforeExiting = true;
		}
		//New option added 9/2
		if(vm.count("m"))
		{
			out_ProgramInputArguments.majorMode = vm["m"].as<std::string>();
		}
		//New option added 6/3
		if(vm.count("s"))
		{
			out_ProgramInputArguments.synchronizeWithCyPhy = true;
			out_ProgramInputArguments.syncConnectionString = vm["s"].as<std::string>();
		}
		if (vm.count("o"))
		{
			AssemblyOptions::Create(vm["o"].as<std::string>());
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

	void ThrowExecption_If_InvalidInputArguments(   int                        in_argc,
													const char                 * const in_argv[],
													const ProgramInputArguments &in_ProgramInputArguments)
																			throw (isis::application_exception)
	{

		if ( in_ProgramInputArguments.synchronizeWithCyPhy &&  !in_ProgramInputArguments.graphicsModeOn )
		{		
			std::cout << in_ProgramInputArguments;
			std::stringstream errorString;
			errorString << "Input Arguments: " << std::endl;
			for ( int i = 0; i < in_argc; ++i) errorString << in_argv[i] << std::string(" ");
			errorString << std::endl << "Input arguments error, if -s  present then -g must also be present." ;
			throw isis::application_exception(errorString.str());

		}
		if ( in_ProgramInputArguments.synchronizeWithCyPhy && in_ProgramInputArguments.designID.size() == 0 )
		{
			std::cout << in_ProgramInputArguments;
			std::stringstream errorString;
			errorString << "Input Arguments: " << std::endl;
			for ( int i = 0; i < in_argc; ++i) errorString << in_argv[i] << std::string(" ");
			errorString << std::endl << "Input arguments error, if -s  present then -d must also be present." ;
			throw isis::application_exception(errorString.str());
		}
	}


} // END namespace isis