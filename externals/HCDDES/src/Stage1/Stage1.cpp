/*** Included Header Files ***/
#include <UdmBase.h>
#include "Visitor.h"
#include "InferenceVisitor.h"
#include "ConfigKeeper.h"


/***************************************************************************************/


int main( int argc, char **argv ) {
	ConfigKeeper &config = ConfigKeeper::Inst();
	if ( !config.ProcessCmdLine( argc, argv )  ) return 1;
	// Process the model
	try {
		Udm::SmartDataNetwork sdn( ESMoL::diagram );
		Udm::SmartDataNetwork newsdn( Semantics::diagram );
		sdn.OpenExisting( ConfigKeeper::Inst().GetInfileName(), "", Udm::CHANGES_LOST_DEFAULT  );
		newsdn.CreateNew( ConfigKeeper::Inst().GetOutfileName(), "Semantics", Semantics::RootFolder::meta, Udm::CHANGES_LOST_DEFAULT );
		{
		//Get the root folder of both projects
		ESMoL::RootFolder erf = ESMoL::RootFolder::Cast(sdn.GetRootObject());
		Semantics::RootFolder srf = Semantics::RootFolder::Cast(newsdn.GetRootObject());

		// Step 1: Check model constraints
		ConstraintVisitor visitor0(srf);
		erf.Accept(visitor0);
		unsigned long errs = visitor0.getNumFailures();
		// Check for errors
		if ( errs ) {
			std::cout << errs << " error(s) found in model...exiting." << std::endl;
			exit(1);
		} else {
			std::cout << "No errors found." << std::endl;
		}

		// Step 2: Check inference
		InferenceVisitor inferenceVisitor( erf );

		// Step 3: Visit Elements
		ElementVisitor elemVisitor(srf);
		erf.Accept(elemVisitor);
		newsdn.CommitEditSequence();

		// Step 4: Visit Relationships
		RelationVisitor relationVisitor(srf);
		erf.Accept(relationVisitor);
	
		}
		// Built new model
		newsdn.CloseWithUpdate();
		// Read-only access to input model
		sdn.CloseNoUpdate();
	// Must be some exceptions somewhere
	} catch( udm_exception & ex )	{
		std::cout << "Exception occured in udm " << std::endl;
		std::cout << ex.what() << std::endl;
		exit(1);
	} catch( ... ) {
		std::cout << "Unknown exception in Stage1!" << std::endl;
		exit(2);
	}

	// We are done here
	return 0;
}

