#include "RuleReturn.hpp"

SFC::LocalVar findMember( SFC::Struct sfcStruct, const std::string &name ) {
	MemberSet memberSet = sfcStruct.LocalVar_kind_children_sorted( MemberSorter() );
	for( MemberSet::iterator mbsItr = memberSet.begin() ; mbsItr != memberSet.end() ; (void)++mbsItr ) {
		if (  name == static_cast< std::string >( mbsItr->name() )  ) return *mbsItr;
	}
	return SFC::LocalVar();
}

ExprsProxyVector &unifyExprsProxyVectors( ExprsProxyVector &exprsProxyVector1, ExprsProxyVector &exprsProxyVector2 ) {
	ExprsProxyVector *exprsProxyVectorPtr1 = &exprsProxyVector1;
	ExprsProxyVector *exprsProxyVectorPtr2 = &exprsProxyVector2;

	if ( exprsProxyVectorPtr1->size() < exprsProxyVectorPtr2->size() ) {
		std::swap( exprsProxyVectorPtr1, exprsProxyVectorPtr2 );
	}

	ExprsProxyVector::iterator vpvItr1 = exprsProxyVectorPtr1->end();
	ExprsProxyVector::iterator vpvItr2 = exprsProxyVectorPtr2->end();
	while( vpvItr2 != exprsProxyVectorPtr2->begin() ) (--vpvItr2)->unify( *--vpvItr1 );

	return *exprsProxyVectorPtr1;
}

