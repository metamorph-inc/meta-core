#ifndef _ISIS_PROPERTYASSIGNMENT_
#define _ISIS_PROPERTYASSIGNMENT_

#include <set>

#include <DesertIfaceBack.h>

class PropertyAssignment {

public:
	struct Comparator {
		bool operator()( const DesertIfaceBack::PropertyAssignment &propertyAssignment1, const DesertIfaceBack::PropertyAssignment &propertyAssignment2 );
	};

	typedef std::set< DesertIfaceBack::PropertyAssignment, Comparator > set;

	static bool compare( DesertIfaceBack::PropertyAssignment compareFromPropertyAssignment, DesertIfaceBack::PropertyAssignment compareToPropertyAssignment );
};

#endif /* _ISIS_PROPERTYASSIGNMENT_ */