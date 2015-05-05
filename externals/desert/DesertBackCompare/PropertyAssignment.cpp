#include "PropertyAssignment.hpp"
#include "EquivAssignment.hpp"
#include "CustomMember.hpp"
#include "VariableProperty.hpp"
#include "NaturalMember.hpp"


bool PropertyAssignment::Comparator::operator()( const DesertIfaceBack::PropertyAssignment &propertyAssignment1, const DesertIfaceBack::PropertyAssignment &propertyAssignment2 ) {

	DesertIfaceBack::VariableProperty variableProperty1 = propertyAssignment1.value_of_end();
	DesertIfaceBack::VariableProperty variableProperty2 = propertyAssignment2.value_of_end();
	if (  VariableProperty::Comparator()( variableProperty1, variableProperty2 )  ) return true;
	if (  VariableProperty::Comparator()( variableProperty2, variableProperty1 )  ) return false;

	DesertIfaceBack::Value value1 = propertyAssignment1.value_end();
	DesertIfaceBack::Value value2 = propertyAssignment2.value_end();
	if (  value1.type() == DesertIfaceBack::CustomMember::meta ) {
		if (  value2.type() == DesertIfaceBack::CustomMember::meta ) {
			DesertIfaceBack::CustomMember customMember1 = DesertIfaceBack::CustomMember::Cast( value1 );
			DesertIfaceBack::CustomMember customMember2 = DesertIfaceBack::CustomMember::Cast( value2 );
			if (  CustomMember::Comparator()( customMember1, customMember2 )  ) return true;
			return false;
		} else {
			return true;
		}
	} else {
		if (  value2.type() == DesertIfaceBack::CustomMember::meta ) {
			return false;
		} else {
			DesertIfaceBack::NaturalMember naturalMember1 = DesertIfaceBack::NaturalMember::Cast( value1 );
			DesertIfaceBack::NaturalMember naturalMember2 = DesertIfaceBack::NaturalMember::Cast( value2 );
			if (  NaturalMember::Comparator()( naturalMember1, naturalMember2 )  ) return true;
			return false;
		}
	}
	assert( false );
	return false;
}

bool PropertyAssignment::compare( DesertIfaceBack::PropertyAssignment compareFromPropertyAssignment, DesertIfaceBack::PropertyAssignment compareToPropertyAssignment ) {

	DesertIfaceBack::VariableProperty compareFromVariableProperty = compareFromPropertyAssignment.value_of_end();
	DesertIfaceBack::VariableProperty compareToVariableProperty = compareToPropertyAssignment.value_of_end();
	if (  !VariableProperty::compare( compareFromVariableProperty, compareToVariableProperty )  ) return false;

	DesertIfaceBack::Value compareFromValue = compareFromPropertyAssignment.value_end();
	DesertIfaceBack::Value compareToValue = compareToPropertyAssignment.value_end();
	if (  Udm::IsDerivedFrom( compareFromValue.type(), DesertIfaceBack::CustomMember::meta )  ) {
		if (  Udm::IsDerivedFrom( compareToValue.type(), DesertIfaceBack::CustomMember::meta )  ) {
			DesertIfaceBack::CustomMember compareFromCustomMember = DesertIfaceBack::CustomMember::Cast( compareFromValue );
			DesertIfaceBack::CustomMember compareToCustomMember = DesertIfaceBack::CustomMember::Cast( compareToValue );
			if (  !CustomMember::compare( compareFromCustomMember, compareToCustomMember )  ) return false;
		} else {
			return false;
		}
	} else {
		if (  Udm::IsDerivedFrom( compareToValue.type(), DesertIfaceBack::CustomMember::meta )  ) {
			return false;
		} else {
			DesertIfaceBack::NaturalMember compareFromNaturalMember = DesertIfaceBack::NaturalMember::Cast( compareFromValue );
			DesertIfaceBack::NaturalMember compareToNaturalMember = DesertIfaceBack::NaturalMember::Cast( compareToValue );
			if (  !NaturalMember::compare( compareFromNaturalMember, compareToNaturalMember )  ) return false;
		}
	}

	EquivAssignment::set compareFromEquivAssignmentSet = compareFromPropertyAssignment.EquivAssignment_kind_children_sorted( EquivAssignment::Comparator() );
	EquivAssignment::set compareToEquivAssignmentSet = compareToPropertyAssignment.EquivAssignment_kind_children_sorted( EquivAssignment::Comparator() );

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
