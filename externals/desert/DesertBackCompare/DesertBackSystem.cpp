#include "DesertBackSystem.hpp"
#include "Configuration.hpp"
#include "Element.hpp"
#include "CustomMember.hpp"
#include "NaturalMember.hpp"


bool DesertBackSystem::compare( DesertIfaceBack::DesertBackSystem compareFromDesertBackSystem, DesertIfaceBack::DesertBackSystem compareToDesertBackSystem ) {

	Configuration::set compareFromConfigurationSet = compareFromDesertBackSystem.Configuration_kind_children_sorted( Configuration::Comparator() );
	Configuration::set compareToConfigurationSet = compareToDesertBackSystem.Configuration_kind_children_sorted( Configuration::Comparator() );

	if ( compareFromConfigurationSet.size() != compareToConfigurationSet.size() ) return false;

	Configuration::set::iterator cfsItr1 = compareFromConfigurationSet.begin();
	Configuration::set::iterator cfsItr2 = compareToConfigurationSet.begin();
	while( cfsItr1 != compareFromConfigurationSet.end() ) {
		if (  !Configuration::compare( *cfsItr1, *cfsItr2 )  ) return false;
		++cfsItr1;
		++cfsItr2;
	}


	Element::set compareFromElementSet = compareFromDesertBackSystem.Element_kind_children_sorted( Element::Comparator() );
	Element::set compareToElementSet = compareToDesertBackSystem.Element_kind_children_sorted( Element::Comparator() );

	if ( compareFromElementSet.size() != compareToElementSet.size() ) return false;

	Element::set::iterator elsItr1 = compareFromElementSet.begin();
	Element::set::iterator elsItr2 = compareToElementSet.begin();
	while( elsItr1 != compareFromElementSet.end() ) {
		if (  !Element::compare( *elsItr1, *elsItr2 )  ) return false;
		++elsItr1;
		++elsItr2;
	}


	CustomMember::set compareFromCustomMemberSet = compareFromDesertBackSystem.CustomMember_kind_children_sorted( CustomMember::Comparator() );
	CustomMember::set compareToCustomMemberSet = compareToDesertBackSystem.CustomMember_kind_children_sorted( CustomMember::Comparator() );

	if ( compareFromCustomMemberSet.size() != compareToCustomMemberSet.size() ) return false;

	CustomMember::set::iterator cmsItr1 = compareFromCustomMemberSet.begin();
	CustomMember::set::iterator cmsItr2 = compareToCustomMemberSet.begin();
	while( cmsItr1 != compareFromCustomMemberSet.end() ) {
		if (  !CustomMember::compare( *cmsItr1, *cmsItr2 )  ) return false;
		++cmsItr1;
		++cmsItr2;
	}


	NaturalMember::set compareFromNaturalMemberSet = compareFromDesertBackSystem.NaturalMember_kind_children_sorted( NaturalMember::Comparator() );
	NaturalMember::set compareToNaturalMemberSet = compareToDesertBackSystem.NaturalMember_kind_children_sorted( NaturalMember::Comparator() );

	if ( compareFromNaturalMemberSet.size() != compareToNaturalMemberSet.size() ) return false;

	NaturalMember::set::iterator nmsItr1 = compareFromNaturalMemberSet.begin();
	NaturalMember::set::iterator nmsItr2 = compareToNaturalMemberSet.begin();
	while( nmsItr1 != compareFromNaturalMemberSet.end() ) {
		if (  !NaturalMember::compare( *nmsItr1, *nmsItr2 )  ) return false;
		++nmsItr1;
		++nmsItr2;
	}


	return true;
}
