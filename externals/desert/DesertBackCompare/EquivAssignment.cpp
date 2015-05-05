#include "EquivAssignment.hpp"


bool EquivAssignment::Comparator::operator()( const DesertIfaceBack::EquivAssignment &equivAssignment1, const DesertIfaceBack::EquivAssignment &equivAssignment2 ) const {
	if ( equivAssignment1.id() < equivAssignment2.id() ) return true;
	if ( equivAssignment1.id() > equivAssignment2.id() ) return false;

	if ( equivAssignment1.externalID() < equivAssignment2.externalID() ) return true;
	if ( equivAssignment1.externalID() > equivAssignment2.externalID() ) return false;

	std::string name1( equivAssignment1.name() );
	std::string name2( equivAssignment2.name() );

	return name1 < name2;
}
