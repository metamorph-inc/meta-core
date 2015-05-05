#include "Configuration.hpp"
#include "AlternativeAssignment.hpp"
#include "PropertyAssignment.hpp"
#include "EquivAssignment.hpp"

bool Configuration::Comparator::operator()( const DesertIfaceBack::Configuration &configuration1, const DesertIfaceBack::Configuration &configuration2 ) {

	AlternativeAssignment::set alternativeAssignmentSet1 = configuration1.AlternativeAssignment_kind_children_sorted( AlternativeAssignment::Comparator() );
	AlternativeAssignment::set alternativeAssignmentSet2 = configuration2.AlternativeAssignment_kind_children_sorted( AlternativeAssignment::Comparator() );

	AlternativeAssignment::set::iterator aasItr1 = alternativeAssignmentSet1.begin();
	AlternativeAssignment::set::iterator aasItr2 = alternativeAssignmentSet2.begin();
	while( aasItr1 != alternativeAssignmentSet1.end() && aasItr2 != alternativeAssignmentSet2.end() ) {
		if (  AlternativeAssignment::Comparator()( *aasItr1, *aasItr2 )  ) return true;
		if (  AlternativeAssignment::Comparator()( *aasItr2, *aasItr1 )  ) return false;
		++aasItr1;
		++aasItr2;
	}
	if ( aasItr1 != alternativeAssignmentSet1.end() ) return false;
	if ( aasItr2 != alternativeAssignmentSet2.end() ) return true;


	PropertyAssignment::set propertyAssignmentSet1 = configuration1.PropertyAssignment_kind_children_sorted( PropertyAssignment::Comparator() );
	PropertyAssignment::set propertyAssignmentSet2 = configuration2.PropertyAssignment_kind_children_sorted( PropertyAssignment::Comparator() );

	PropertyAssignment::set::iterator pasItr1 = propertyAssignmentSet1.begin();
	PropertyAssignment::set::iterator pasItr2 = propertyAssignmentSet2.begin();
	while( pasItr1 != propertyAssignmentSet1.end() && pasItr2 != propertyAssignmentSet2.end() ) {
		if (  PropertyAssignment::Comparator()( *pasItr1, *pasItr2 )  ) return true;
		if (  PropertyAssignment::Comparator()( *pasItr2, *pasItr1 )  ) return false;
		++pasItr1;
		++pasItr2;
	}
	if ( pasItr1 != propertyAssignmentSet1.end() ) return false;
	if ( pasItr2 != propertyAssignmentSet2.end() ) return true;


	EquivAssignment::set equivAssignmentSet1 = configuration1.EquivAssignment_kind_children_sorted( EquivAssignment::Comparator() );
	EquivAssignment::set equivAssignmentSet2 = configuration2.EquivAssignment_kind_children_sorted( EquivAssignment::Comparator() );

	EquivAssignment::set::iterator easItr1 = equivAssignmentSet1.begin();
	EquivAssignment::set::iterator easItr2 = equivAssignmentSet2.begin();
	while( easItr1 != equivAssignmentSet1.end() && easItr2 != equivAssignmentSet2.end() ) {
		if (  EquivAssignment::Comparator()( *easItr1, *easItr2 )  ) return true;
		if (  EquivAssignment::Comparator()( *easItr2, *easItr1 )  ) return false;
		++easItr1;
		++easItr2;
	}
	if ( easItr1 != equivAssignmentSet1.end() ) return false;
	return easItr2 != equivAssignmentSet2.end();
}

bool Configuration::compare( DesertIfaceBack::Configuration compareFromConfiguration, DesertIfaceBack::Configuration compareToConfiguration ) {

	AlternativeAssignment::set compareFromAlternativeAssignmentSet = compareFromConfiguration.AlternativeAssignment_kind_children_sorted( AlternativeAssignment::Comparator() );
	AlternativeAssignment::set compareToAlternativeAssignmentSet = compareToConfiguration.AlternativeAssignment_kind_children_sorted( AlternativeAssignment::Comparator() );

	if ( compareFromAlternativeAssignmentSet.size() != compareToAlternativeAssignmentSet.size() ) return false;

	AlternativeAssignment::set::iterator aasItr1 = compareFromAlternativeAssignmentSet.begin();
	AlternativeAssignment::set::iterator aasItr2 = compareToAlternativeAssignmentSet.begin();
	while( aasItr1 != compareFromAlternativeAssignmentSet.end() ) {
		if (  !AlternativeAssignment::compare( *aasItr1, *aasItr2 )  ) return false;
		++aasItr1;
		++aasItr2;
	}


	PropertyAssignment::set compareFromPropertyAssignmentSet = compareFromConfiguration.PropertyAssignment_kind_children_sorted( PropertyAssignment::Comparator() );
	PropertyAssignment::set compareToPropertyAssignmentSet = compareToConfiguration.PropertyAssignment_kind_children_sorted( PropertyAssignment::Comparator() );

	if ( compareFromPropertyAssignmentSet.size() != compareToPropertyAssignmentSet.size() ) return false;

	PropertyAssignment::set::iterator pasItr1 = compareFromPropertyAssignmentSet.begin();
	PropertyAssignment::set::iterator pasItr2 = compareToPropertyAssignmentSet.begin();
	while( pasItr1 != compareFromPropertyAssignmentSet.end() ) {
		if (  !PropertyAssignment::compare( *pasItr1, *pasItr2 )  ) return false;
		++pasItr1;
		++pasItr2;
	}


	EquivAssignment::set compareFromEquivAssignmentSet = compareFromConfiguration.EquivAssignment_kind_children_sorted( EquivAssignment::Comparator() );
	EquivAssignment::set compareToEquivAssignmentSet = compareToConfiguration.EquivAssignment_kind_children_sorted( EquivAssignment::Comparator() );

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
