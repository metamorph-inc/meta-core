#ifndef __VISITORFACTORY_H__
#define __VISITORFACTORY_H__


/*** Included Header Files ***/
#include "Semantics.h"
#include "utils.h"


/*** Class Pre-Defines ***/
class AbstractVisitor;


/***************************************************************************************/


// We use this to make specific visitors
class VisitorFactory {
public:
	typedef AbstractVisitor * (*Stage2Instantiator)(Semantics::RootFolder & rf, TimeStamp & ts );
	typedef std::map< const std::string, Stage2Instantiator> InstantiatorMap;
private:
	static InstantiatorMap &GetInstantiatorMap( void ) {
		static InstantiatorMap instantiatorMap;
		return instantiatorMap;
	}

public:
	// Constructors and Destructors
	VisitorFactory()				{ }
	virtual ~VisitorFactory()		{ }

	AbstractVisitor * GetVisitorInstance( const std::string &name, Semantics::RootFolder &rootFolder, TimeStamp &timeStamp );
	static bool RegisterInstantiator( const std::string &name, Stage2Instantiator intantiator);
	static std::string ListTranslators();
};


/***************************************************************************************/


#endif // __VISITORFACTORY_H__

