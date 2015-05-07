#include <iostream>
#include <UdmBase.h>
#include "Stage1_Visitor.h"


int main( int argc, char **argv ) {

	ConfigKeeperS1 &cfg = ConfigKeeperS1::inst();
	if (  !cfg.processCmdLine( argc, argv )  ) return 1;
	
	// process the model
	//try
	{

		Udm::SmartDataNetwork sdn(ESMoL::diagram); 
		Udm::SmartDataNetwork newsdn(Semantics::diagram);
		
		sdn.OpenExisting( ConfigKeeperS1::inst().getInfileName() );
		newsdn.CreateNew( ConfigKeeperS1::inst().getOutfileName(), "Semantics", Semantics::RootFolder::meta );	

		{
			//Get the root folder of both projects
			ESMoL::RootFolder erf = ESMoL::RootFolder::Cast(sdn.GetRootObject());
			Semantics::RootFolder srf = Semantics::RootFolder::Cast(newsdn.GetRootObject());

			Stage1_ConstraintVisitor visitor0(srf);
			erf.Accept(visitor0);

			unsigned long errs = visitor0.getNumFailures();
			if ( errs )
			{
				cout << errs << " error(s) found in model...exiting." << endl;
				exit(1);
			}
			else
			{
				cout << "No errors found." << endl;
			}

			Stage1_ElementVisitor visitor1(srf);
			erf.Accept(visitor1);

			//newsdn.CommitEditSequence();

			Stage1_RelationVisitor visitor2(srf);
			erf.Accept(visitor2);
		}

		sdn.CloseNoUpdate(); // Read-only access to input model
		newsdn.CloseWithUpdate(); // Built new model
	}
	//catch(udm_exception &e)	{
	//	std::cout << "Exception occured in udm" << endl;
	//	std::cout << e.what() << endl;
	//}

	return 0;
}

