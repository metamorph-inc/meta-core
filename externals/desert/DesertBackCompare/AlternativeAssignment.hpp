#ifndef _ISIS_ALTERNATIVEASSIGNMENT_
#define _ISIS_ALTERNATIVEASSIGNMENT_

#include <set>

#include <DesertIfaceBack.h>

class AlternativeAssignment {

public:
	struct Comparator {
		bool operator()( const DesertIfaceBack::AlternativeAssignment &alternativeAssignment1, const DesertIfaceBack::AlternativeAssignment &alternativeAssignment2 );
	};

	typedef std::set< DesertIfaceBack::AlternativeAssignment, Comparator > set;

	static bool compare( DesertIfaceBack::AlternativeAssignment compareFromAlternativeAssignment, DesertIfaceBack::AlternativeAssignment compareToAlternativeAssignment );

};

#endif /* _ISIS_ALTERNATIVEASSIGNMENT_ */