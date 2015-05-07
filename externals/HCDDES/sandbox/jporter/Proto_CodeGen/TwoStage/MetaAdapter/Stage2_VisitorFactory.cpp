#include "Stage2_VisitorFactory.h"
#include "Stage2_SchedVisitor.h"

#include <sstream>

using namespace std;

Stage2_VisitorFactory::Stage2_VisitorFactory()
{
}

Stage2_AbstractVisitor * Stage2_VisitorFactory::getVisitorInstance( const std::string & name, Semantics::RootFolder & rf, timeStamp & ts )
{
	Stage2Instantiator func = (getInstantiatorMap())[name];

	if ( func )
	{
		return func(rf, ts);
	}
	else
	{
		return NULL;
	}
}

// Static member for registration
bool Stage2_VisitorFactory::registerInstantiator( const std::string & name, Stage2Instantiator func)
{
	(getInstantiatorMap()).insert( std::make_pair( name, func ) );
	return true;
}

std::string Stage2_VisitorFactory::listTranslators() {

	std::string retval( "" );

	bool printComma = false;
	for( InstantiatorMap::iterator pTr = (getInstantiatorMap()).begin() ; 
		 pTr != Stage2_VisitorFactory::getInstantiatorMap().end() ; pTr++ ) {

		if ( printComma ) retval += std::string( ", " );
		else              printComma = true;

		retval += pTr->first;
	}

	return retval;
}