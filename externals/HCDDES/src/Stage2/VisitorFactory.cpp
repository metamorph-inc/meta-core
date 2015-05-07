/*** Warning Workaround ***/
#pragma warning( disable: 4251 )


/*** Included Header Files ***/
#include "VisitorFactory.h"
#include "SchedVisitor.h"


/***************************************************************************************/


AbstractVisitor* VisitorFactory::GetVisitorInstance( const std::string &name,
	Semantics::RootFolder &rootFolder, TimeStamp &timeStamp ) {
	// Get the function from the map
	Stage2Instantiator func = (GetInstantiatorMap())[name];
	// If the function exists - return its return value
	if (func) return func( rootFolder, timeStamp );
	// Otherwise return NULL;
	else return NULL;
}


bool VisitorFactory::RegisterInstantiator( const std::string &name, Stage2Instantiator func ) {
	// Insert the func into the list of instantiators
	(GetInstantiatorMap()).insert( std::make_pair( name, func ) );
	return true;
}


std::string VisitorFactory::ListTranslators() {
	std::string retval("");
	bool printComma = false;
	// Loop through all of the translators
	InstantiatorMap::iterator mapIter = (GetInstantiatorMap()).begin(); 
	for(; mapIter != (GetInstantiatorMap()).end(); mapIter++) {
		// Only print comma on second or later translators
		if ( printComma )
			retval += std::string( ", " );
		else
			printComma = true;
		// Build up the return string
		retval += mapIter->first;
	}
	// Return the string of translators
	return retval;
}


/***************************************************************************************/

