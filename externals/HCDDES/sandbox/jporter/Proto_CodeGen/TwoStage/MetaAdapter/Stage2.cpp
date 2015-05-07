// Stage2.cpp : Generate things
//

#include <iostream>
#include <fstream>
#include <algorithm>

#include "google/template.h"

#include <UdmBase.h>
#include "utils.h"
#include "Stage2_VisitorFactory.h"
#include "Stage2_AbstractVisitor.h"

#include "ConfigKeeperS2.h"

using namespace std;


int main( int argc, char *argv[] ) {


	ConfigKeeperS2 &cfg = ConfigKeeperS2::inst();
	if (  !cfg.processCmdLine( argc, argv )  ) {

		if ( cfg.getList() ) {
			cout << "Available translators: " << Stage2_VisitorFactory::listTranslators() << endl;
		}
		/*else
		{
			cout << "Failed to process command line: " << std::endl;
			for (int lidx = 0; lidx < argc; lidx++ )
			{
				cout << argv[lidx] << endl;
			}
		}
		cout << endl; */
		return 1; // exit early
	}

	std::string fname( cfg.getInfileName() );

	// process the model
	try {

		Udm::SmartDataNetwork sdn( Semantics::diagram );

		sdn.OpenExisting( fname );

		//Get the root folder
		Semantics::RootFolder srf = Semantics::RootFolder::Cast( sdn.GetRootObject() );

		// Set up the translator factory
		Stage2_VisitorFactory fact;
		timeStamp ts;

		// Iterate over the requested translators and do the right thing!
		vector< string > translist = cfg.getTranslators();
		for ( vector< string >::iterator idx = translist.begin() ; idx != translist.end() ; ++idx ) {

			cout << "Translating with " << *idx << "... " << endl;
			Stage2_AbstractVisitor *vis = fact.getVisitorInstance( *idx, srf, ts );

			if ( vis ) {
				srf.Accept( *vis );
			} else {
				cout << "Unable to find requested translator: " << *idx << "." << endl;
				cout << "Options are: " << Stage2_VisitorFactory::listTranslators() << endl;
			}
		}

		sdn.CloseNoUpdate(); // Read-only access to input model

	} catch ( udm_exception &e ) {

		std::cout << "Exception occured in udm:" << endl;
		std::cout << e.what() << endl;
		cin.get();

	}

	return 0;
}

