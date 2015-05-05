#ifndef _ISIS_CUSTOMMEMBER_
#define _ISIS_CUSTOMMEMBER_

#include <DesertIfaceBack.h>

class CustomMember {

public:
	struct Comparator {
		bool operator()( const DesertIfaceBack::CustomMember &customMember1, const DesertIfaceBack::CustomMember &customMember2 ) {
			if ( customMember1.id() < customMember2.id() ) return true;
			if ( customMember1.id() > customMember2.id() ) return false;

			if ( customMember1.externalID() < customMember2.externalID() ) return true;
			if ( customMember1.externalID() > customMember2.externalID() ) return false;

			std::string name1 = customMember1.name();
			std::string name2 = customMember2.name();

			return name1 < name2;
		}
	};

	typedef std::set< DesertIfaceBack::CustomMember, Comparator > set;

	static bool compare( DesertIfaceBack::CustomMember compareFromCustomMember, DesertIfaceBack::CustomMember compareToCustomMember ) {
		return compareFromCustomMember.id() == compareToCustomMember.id() &&
		 compareFromCustomMember.externalID() == compareToCustomMember.externalID() &&
		 compareFromCustomMember.name() == compareToCustomMember.name();
	}

};

#endif /* _ISIS_CUSTOMMEMBER_ */