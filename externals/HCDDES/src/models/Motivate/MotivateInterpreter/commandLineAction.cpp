/*** Included Header Files ***/
#include "commandLineAction.h"


/************************************************************************************/


ActionStatus CommandLineAction::Execute( std::map< std::string,std::string > params ) {
	Console::Out::WriteLine( "Executing CommandLine Action" );
	return Failure;
}


/************************************************************************************/

