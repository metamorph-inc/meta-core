#include "CPrinter.hpp"
#include "JPrinter.hpp"
#include "ConfigKeeper.hpp"
#include "SFC.h"
#include "InputFileRegistry.h"
#include <Uml.h>

#include <iostream>

//UDM_USE_DOM
#ifdef _WIN32
UDM_USE_MGA
#endif

void usage( std::string progname );

int main(int argc, char *argv[]) {

	// Try to process the command line
	if ( !ConfigKeeper::getSingleton().processCommandLineArguments( argc, argv ) ) {
		return 0;
	}
	// See if help was requested
	if (  ConfigKeeper::getSingleton().getCount( "help" )  ) {
		std::cout << ConfigKeeper::getSingleton().getDescription();
		return 0;
	}
	// Make sure an input was specified
	if (  !ConfigKeeper::getSingleton().getCount( "sfc-file" )  ) {
		std::cerr << "Input file must be specified." << std::endl;
		std::cerr << ConfigKeeper::getSingleton().getDescription();
		return 1;
	}

	bool java = ConfigKeeper::getSingleton().getCount( "c" ) == 0 && ConfigKeeper::getSingleton().getCount( "java" ) != 0;
	bool annotations = ConfigKeeper::getSingleton().getCount( "annotations" ) != 0;
	bool generateStatusFunction = java && ConfigKeeper::getSingleton().getCount( "status" );

	std::string inputFilename = ConfigKeeper::getSingleton().getStringValue( "sfc-file" );

	InputFileRegistry ifr;
	ifr.registerFile( "SFC = \"" + inputFilename + "\" !\"SFC.xsd\" " );

	try {
		Udm::SmartDataNetwork dataNetwork( SFC::diagram );

		dataNetwork.OpenExisting(ifr.getFileName( "SFC" ), 
			UseXSD()( ifr.getFileName( "SFC" )) ? ifr.getXsdName( "SFC" ) : "SFC", 
			Udm::CHANGES_LOST_DEFAULT);

		SFC::Project project = SFC::Project::Cast( dataNetwork.GetRootObject() );
		SFCTypesManager::initSingleton( project );
		if ( java ) {
			JPrinter::printProject( project, annotations, generateStatusFunction );
		} else {
			CPrinter::print( project );
		}

		dataNetwork.CloseNoUpdate();
	} catch( udm_exception &e) {
		cout << e.what() << endl;
		return 1;
	}
	return 0;
}
