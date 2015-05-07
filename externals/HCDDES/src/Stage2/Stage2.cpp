/*** Warning Workaround ***/
#pragma warning( disable: 4251 )


/*** Included Header Files ***/
#include "ctemplate/template.h"
#include <UdmBase.h>
#include "utils.h"
#include "VisitorFactory.h"
#include "AbstractVisitor.h"
#include "ConfigKeeper.h"


/***************************************************************************************/


int main( int argc, char *argv[] ) {
	// Setup the command line parser
	ConfigKeeper &config = ConfigKeeper::inst();
	if ( !config.ProcessCommandLine( argc, argv ) ) {
		if ( config.GetList() ) {
			std::cout << "Available translators: " << VisitorFactory::ListTranslators() << std::endl;
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

	// Get the input file name
	std::string inputFileName = config.GetInfileName();
	// Try to process the model
	try {
		// Create the UDM smart data network
		Udm::SmartDataNetwork sdn(Semantics::diagram);
		// Open the input file
		sdn.OpenExisting(inputFileName);
		//Get the root folder
		Semantics::RootFolder srf = Semantics::RootFolder::Cast( sdn.GetRootObject() );
		// Set up the translator factory
		VisitorFactory factory;
		TimeStamp timeStamp;
		// Iterate over the requested translators and do the right thing!
		vector<string> translist = config.GetTranslators();
		for (vector<string>::iterator listIter = translist.begin(); listIter != translist.end(); ++listIter) {
			// Show which translator is being used
			std::cout << "Translating with " << *listIter << "... " << std::endl;
			// Get an instance of that visitor
			AbstractVisitor *visitor = factory.GetVisitorInstance( *listIter, srf, timeStamp );
			// If the visitor exists
			if (visitor) {
				// Then use it
				srf.Accept(*visitor);
			// Otherwise, debug some output
			} else {
				std::cout << "Unable to find requested translator: " << *listIter << "." << std::endl;
				std::cout << "Options are: " << VisitorFactory::ListTranslators() << std::endl;
			}
		}
		// Read-only access to input model - hence noUpdate
		sdn.CloseNoUpdate();
	// Catch any exceptions from UDM
	} catch (udm_exception &e)
	{
		// What is the exception?
		std::cout << "Exception occured in udm:" << std::endl;
		std::cout << e.what() << std::endl;
		// Wait for a return
		cin.get();
		exit(1);
	} catch ( std::exception & e )
	{
		std::cout << "General exception: " << e.what() << std::endl;

	} catch ( ... ) {
		std::cout << "Unknown exception in Stage 2!" << std::endl;
		exit(2);
	}
	// And...we are done here
	return 0;
}


/***************************************************************************************/

