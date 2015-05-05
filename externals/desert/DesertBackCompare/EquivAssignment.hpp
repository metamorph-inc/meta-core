#ifndef _ISIS_EQUIVASSIGNMENT_
#define _ISIS_EQUIVASSIGNMENT_

#include <set>

#include <DesertIfaceBack.h>

class EquivAssignment {

public:
	struct Comparator {
		bool operator()( const DesertIfaceBack::EquivAssignment &equivAssignment1, const DesertIfaceBack::EquivAssignment &equivAssignment2 ) const;
	};

	typedef std::set< DesertIfaceBack::EquivAssignment, Comparator > set;

	static bool compare( DesertIfaceBack::EquivAssignment compareFromEquivAssignment, DesertIfaceBack::EquivAssignment compareToEquivAssignment ) {
		return compareFromEquivAssignment.id() == compareToEquivAssignment.id() &&
		 compareFromEquivAssignment.externalID() == compareToEquivAssignment.externalID() &&
		 compareFromEquivAssignment.name() == compareToEquivAssignment.name();
	}

};

#endif /* _ISIS_EQUIVASSIGNMENT_ */