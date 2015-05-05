#include "AlternativeAssignment.hpp"
#include "EquivAssignment.hpp"
#include "Element.hpp"


bool AlternativeAssignment::Comparator::operator()( const DesertIfaceBack::AlternativeAssignment &alternativeAssignment1, const DesertIfaceBack::AlternativeAssignment &alternativeAssignment2 ) {

	DesertIfaceBack::Element element1 = alternativeAssignment1.alternative_end();
	DesertIfaceBack::Element element2 = alternativeAssignment2.alternative_end();
	if (  Element::Comparator()( element1, element2 )  ) return true;
	if (  Element::Comparator()( element2, element1 )  ) return false;

	element1 = alternativeAssignment1.alternative_of_end();
	element2 = alternativeAssignment2.alternative_of_end();
	if (  Element::Comparator()( element1, element2 )  ) return true;
	if (  Element::Comparator()( element2, element1 )  ) return false;

	EquivAssignment::set equivAssignmentSet1 = alternativeAssignment1.EquivAssignment_kind_children_sorted( EquivAssignment::Comparator() );
	EquivAssignment::set equivAssignmentSet2 = alternativeAssignment2.EquivAssignment_kind_children_sorted( EquivAssignment::Comparator() );

	EquivAssignment::set::iterator easItr1 = equivAssignmentSet1.begin();
	EquivAssignment::set::iterator easItr2 = equivAssignmentSet2.begin();
	while( easItr1 != equivAssignmentSet1.end() &&  easItr1 != equivAssignmentSet2.end()) {
		if (  EquivAssignment::Comparator()( *easItr1, *easItr2 )  ) return true;
		if (  EquivAssignment::Comparator()( *easItr2, *easItr1 )  ) return false;
		++easItr1;
		++easItr2;
	}
	if ( easItr1 != equivAssignmentSet1.end() ) return false;
	return easItr2 != equivAssignmentSet2.end();
}

bool AlternativeAssignment::compare( DesertIfaceBack::AlternativeAssignment compareFromAlternativeAssignment, DesertIfaceBack::AlternativeAssignment compareToAlternativeAssignment ) {

	DesertIfaceBack::Element compareFromElement = compareFromAlternativeAssignment.alternative_end();
	DesertIfaceBack::Element compareToElement = compareToAlternativeAssignment.alternative_end();
	if (  !Element::compare( compareFromElement, compareToElement )  ) return false;

	compareFromElement = compareFromAlternativeAssignment.alternative_of_end();
	compareToElement = compareToAlternativeAssignment.alternative_of_end();
	if (  !Element::compare( compareFromElement, compareToElement )  ) return false;

	EquivAssignment::set compareFromEquivAssignmentSet = compareFromAlternativeAssignment.EquivAssignment_kind_children_sorted( EquivAssignment::Comparator() );
	EquivAssignment::set compareToEquivAssignmentSet = compareToAlternativeAssignment.EquivAssignment_kind_children_sorted( EquivAssignment::Comparator() );

	if ( compareFromEquivAssignmentSet.size() != compareToEquivAssignmentSet.size() ) return false;

	EquivAssignment::set::iterator easItr1 = compareFromEquivAssignmentSet.begin();
	EquivAssignment::set::iterator easItr2 = compareToEquivAssignmentSet.begin();
	while( easItr1 != compareFromEquivAssignmentSet.end() ) {
		if (  !EquivAssignment::compare( *easItr1, *easItr2 )  ) return false;
		++easItr1;
		++easItr2;
	}

	return true;
}
