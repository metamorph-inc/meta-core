#ifndef _ISIS_NATURALMEMBER_
#define _ISIS_NATURALMEMBER_

#include <DesertIfaceBack.h>

class NaturalMember {

public:
	struct Comparator {
		bool operator()( const DesertIfaceBack::NaturalMember &n1, const DesertIfaceBack::NaturalMember &n2 ) {
			return static_cast< int >( n1.value() ) < static_cast< int >( n2.value() );
		}
	};

	typedef std::set< DesertIfaceBack::NaturalMember, Comparator > set;

	static bool compare( DesertIfaceBack::NaturalMember compareFromNaturalMember, DesertIfaceBack::NaturalMember compareToNaturalMember ) {
		return compareFromNaturalMember.value() == compareToNaturalMember.value();
	}

};

#endif /* _ISIS_NATURALMEMBER_ */