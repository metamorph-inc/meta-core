/*** Included Header Files ***/
#include <UdmBase.h>
#include "Visitor.h"
#include "ConfigKeeper.h"


/***************************************************************************************/


int main( int argc, char **argv ) {
	ConfigKeeper &config = ConfigKeeper::Inst();
	if ( !config.ProcessCmdLine( argc, argv )  ) return 1;
	// Process the model
	try {

		if ( config.GetParadigm() == string( "BIP" ) )
		{
			Udm::SmartDataNetwork sdn( BIP::diagram );
			sdn.OpenExisting( config.GetInfileName() );

			BIP::RootFolder brf = BIP::RootFolder::Cast(sdn.GetRootObject());
			TimeStamp ts;
			BIPVisitor bv( brf, ts );

			// Read-only access to input model
			sdn.CloseNoUpdate();
		}
		else if ( config.GetParadigm() == string( "ESMoL" ) )
		{
			Udm::SmartDataNetwork sdn( ESMoL::diagram ); 
			sdn.OpenExisting( config.GetInfileName() );
		
			//Get the root folder
			ESMoL::RootFolder erf = ESMoL::RootFolder::Cast(sdn.GetRootObject());

			// Read-only access to input model
			sdn.CloseNoUpdate();
		}

	} catch( udm_exception &ex )	{
		std::cout << "Exception occured in udm " << std::endl;
		std::cout << ex.what() << std::endl;
	}
	return 0;
}

