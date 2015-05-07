#include <string>
#include <iostream>

#include <boost/filesystem.hpp>
#include <DesertIfaceBack.h>

#include "ConfigKeeper.hpp"
#include "DesertBackSystem.hpp"


void usage( const std::string &programName );

int main( int argc, TCHAR *argv[] ) {

	try {

		if (  ConfigKeeper::getSingleton().getCount( "help" )  ) {
			usage( argv[0] );
			return 0;
		}

		if ( !ConfigKeeper::getSingleton().processCommandLineArguments( argc, argv ) ) {
			usage( argv[ 0 ] );
			return 1;
		}

		// Get the input file name
		if (  !ConfigKeeper::getSingleton().getCount( "compareFrom" )  ) {
			usage( argv[0] );
			return 1;
		}
		std::string compareFromFilename = ConfigKeeper::getSingleton().getStringValue( "compareFrom" );
		if (  !boost::filesystem::exists( compareFromFilename )  ) {
			std::cerr << "ERROR: Could not access file \"" << compareFromFilename << "\"." << std::endl << std::endl;
			return 2;
		}

		if (  !ConfigKeeper::getSingleton().getCount( "compareTo" )  ) {
			usage( argv[0] );
			return 1;
		}
		std::string compareToFilename = ConfigKeeper::getSingleton().getStringValue( "compareTo" );
		if (  !boost::filesystem::exists( compareToFilename )  ) {
			std::cerr << "ERROR: Could not access file \"" << compareToFilename << "\"." << std::endl << std::endl;
			return 2;
		}

		Udm::SmartDataNetwork compareFromSmartDataNetwork( DesertIfaceBack::diagram );
		compareFromSmartDataNetwork.OpenExisting( compareFromFilename, "DesertIfaceBack.xsd", Udm::CHANGES_LOST_DEFAULT );
		DesertIfaceBack::DesertBackSystem compareFromDesertBackSystem = DesertIfaceBack::DesertBackSystem::Cast( compareFromSmartDataNetwork.GetRootObject() );

		Udm::SmartDataNetwork compareToSmartDataNetwork( DesertIfaceBack::diagram );
		compareToSmartDataNetwork.OpenExisting( compareToFilename, "DesertIfaceBack.xsd", Udm::CHANGES_LOST_DEFAULT );
		DesertIfaceBack::DesertBackSystem compareToDesertBackSystem = DesertIfaceBack::DesertBackSystem::Cast( compareToSmartDataNetwork.GetRootObject() );

		return DesertBackSystem::compare( compareFromDesertBackSystem, compareToDesertBackSystem ) ? 0 : 1;

	} catch ( udm_exception &err ) {

		std::cerr << "The following UDM exception occurred:" << std::endl;
		std::cerr << err.what() << std::endl << std::endl;

	} catch ( std::exception &err ) {

		std::cerr << "The following std::exception occurred:" << std::endl;
		std::cerr << err.what() << std::endl << std::endl;

	} catch ( ... ) {

		std::cerr << "Unknown exception caught" << std::endl;
	}

	return 2;
}

void usage( const std::string &programName ) {
	std::cerr << "Usage:  " << programName << " <compareFrom> <compareTo>" << std::endl;
	std::cerr << programName << " compares <compareFrom> file and <compareTo> file to see if they contain identical DesertIfaceBack models." << std::endl << std::endl;
}