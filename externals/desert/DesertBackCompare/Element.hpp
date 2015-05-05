#ifndef _ISIS_ELEMENT_
#define _ISIS_ELEMENT_

#include <set>

#include <DesertIfaceBack.h>


class Element {

public:
	struct Comparator {
		bool operator()( const DesertIfaceBack::Element &element1, const DesertIfaceBack::Element &element2 ) ;
	};

	typedef std::set< DesertIfaceBack::Element, Comparator > set;

	static bool compare( DesertIfaceBack::Element compareFromElement, DesertIfaceBack::Element compareToElement );

};

#endif /* _ISIS_ELEMENT_ */