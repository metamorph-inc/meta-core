#ifndef _STAGE2_VISITORFACTORY_H
#define _STAGE2_VISITORFACTORY_H

#include <map>
#include <vector>

//#include "Stage2_AbstractVisitor.h"
#include "Semantics.h"
#include "utils.h"

class Stage2_AbstractVisitor;

// We use this to make specific visitors
class Stage2_VisitorFactory {

public:
	typedef Stage2_AbstractVisitor * (*Stage2Instantiator)(Semantics::RootFolder & rf, timeStamp & ts );
	typedef std::map< const std::string, Stage2Instantiator> InstantiatorMap;

private:
	static InstantiatorMap &getInstantiatorMap( void ) {
		static InstantiatorMap instantiatorMap;
		return instantiatorMap;
	}

public:
	Stage2_VisitorFactory();
	virtual ~Stage2_VisitorFactory() { }

	Stage2_AbstractVisitor * getVisitorInstance( const std::string & name, Semantics::RootFolder & rf, timeStamp & ts );
	static bool registerInstantiator( const std::string &, Stage2Instantiator );

	static std::string listTranslators();
};

#endif // _STAGE2_VISITOR_H