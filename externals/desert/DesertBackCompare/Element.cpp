#include "Element.hpp"
#include "VariableProperty.hpp"
#include <string>


bool Element::Comparator::operator()( const DesertIfaceBack::Element &element1, const DesertIfaceBack::Element &element2 ) {

	if ( element1.id() < element2.id() ) return true;
	if ( element1.id() > element2.id() ) return false;

	if ( element1.externalID() < element2.externalID() ) return true;
	if ( element1.externalID() > element2.externalID() ) return false;

	std::string name1 = element1.name();
	std::string name2 = element2.name();

	if ( name1 < name2 ) return true;
	if ( name1 > name2 ) return false;

	VariableProperty::set variablePropertySet1 = element1.VariableProperty_kind_children_sorted( VariableProperty::Comparator() );
	VariableProperty::set variablePropertySet2 = element2.VariableProperty_kind_children_sorted( VariableProperty::Comparator() );

	VariableProperty::set::iterator vpsItr1 = variablePropertySet1.begin();
	VariableProperty::set::iterator vpsItr2 = variablePropertySet2.begin();
	while( vpsItr1 != variablePropertySet1.end() && vpsItr2 != variablePropertySet2.end() ) {
		if (  VariableProperty::Comparator()( *vpsItr1, *vpsItr2 )  ) return true;
		if (  VariableProperty::Comparator()( *vpsItr2, *vpsItr1 )  ) return false;
		++vpsItr1;
		++vpsItr2;
	}
	if ( vpsItr1 != variablePropertySet1.end() ) return false;
	return vpsItr2 != variablePropertySet2.end();

}

bool Element::compare( DesertIfaceBack::Element compareFromElement, DesertIfaceBack::Element compareToElement ) {
	if (
		static_cast< std::string >( compareFromElement.name() ) != static_cast< std::string >( compareToElement.name() ) ||
		compareFromElement.id() != compareToElement.id() ||
		compareFromElement.externalID() != compareToElement.externalID() ||
		static_cast< std::string >( compareFromElement.name() ) != static_cast< std::string >( compareToElement.name() )
	) return false;

	VariableProperty::set compareFromVariablePropertySet = compareFromElement.VariableProperty_kind_children_sorted( VariableProperty::Comparator() );
	VariableProperty::set compareToVariablePropertySet = compareToElement.VariableProperty_kind_children_sorted( VariableProperty::Comparator() );

	if ( compareFromVariablePropertySet.size() != compareToVariablePropertySet.size() ) return false;

	VariableProperty::set::iterator vpsItr1 = compareFromVariablePropertySet.begin();
	VariableProperty::set::iterator vpsItr2 = compareToVariablePropertySet.begin();
	while( vpsItr1 != compareFromVariablePropertySet.end() ) {
		if (  !VariableProperty::compare( *vpsItr1, *vpsItr2 )  ) return false;
		++vpsItr1;
		++vpsItr2;
	}

	return true;
}
