#include "BExprMapBuilder.hpp"
#include "Uml.h"

void BExprMapBuilder::build( const std::string &prefix, BENodeSPVector &beNodeSPVector, SLSF::TypeBase typeBase ) {

	if ( typeBase.type() == SLSF::Matrix::meta ) return;

	// SAFE CAST
	SLSF::TypeStruct typeStruct = SLSF::TypeStruct::Cast( typeBase );
	TypeBaseRefSet typeBaseRefSet = typeStruct.TypeBaseRef_children_sorted( TypeBaseRefSorter() );

	TypeBaseRefSet::iterator trsItr = typeBaseRefSet.begin();
	BENodeSPVector::iterator nsvItr = beNodeSPVector.begin();

	while( trsItr != typeBaseRefSet.end() ) {

		std::string newPrefix = beNodeSPProxyCast( *nsvItr )->first;
		if ( !prefix.empty() ) newPrefix = prefix + "." + newPrefix;

		SLSF::TypeBase currentTypeBase = trsItr->ref();
		_bExprMap[ newPrefix ] = currentTypeBase;
		build(  newPrefix, beNodeSPProxyCast( *nsvItr )->second, currentTypeBase  );

		(void)++trsItr;
		(void)++nsvItr;
	}
}

void BExprMapBuilder::build( const std::string &prefix, SLSF::TypeBase typeBase ) {

	if ( typeBase.type() == SLSF::Matrix::meta ) return;

	// SAFE CAST
	SLSF::TypeStruct typeStruct = SLSF::TypeStruct::Cast( typeBase );
	TypeBaseRefSet typeBaseRefSet = typeStruct.TypeBaseRef_children_sorted( TypeBaseRefSorter() );

	TypeBaseRefSet::iterator trsItr = typeBaseRefSet.begin();
	while( trsItr != typeBaseRefSet.end() ) {

		std::string newPrefix = trsItr->name();
		if ( !prefix.empty() ) newPrefix = prefix + "." + newPrefix;

		SLSF::TypeBase currentTypeBase = trsItr->ref();
		_bExprMap[ newPrefix ] = currentTypeBase;
		build( newPrefix, currentTypeBase );

		(void)++trsItr;
	}
}

BExprMapBuilder::BusSelectionVector BExprMapBuilder::getBusSelectionVector( const std::string busSelectionExpr ) {

	BusSelectionVector busSelectionVector;

	BExprOutputVector bExprOutputVector = getBExprOutputVector( busSelectionExpr );

	for(
	 BExprOutputVector::iterator bovItr = bExprOutputVector.begin() ;
	 bovItr != bExprOutputVector.end() ;
	 (void)++bovItr
	) {
		DotVector dotVector = *bovItr;

		DotVector::iterator dtvItr = dotVector.begin();
		std::string busSelection = *dtvItr++;

		while( dtvItr != dotVector.end() ) {
			busSelection += std::string( "." ) + *dtvItr++;
		}

		busSelectionVector.push_back( busSelection );
	}

	return busSelectionVector;
}

BExprMapBuilder::TypeBaseVector BExprMapBuilder::getOutputVector(
 const std::string &bExpr, SLSF::TypeBase typeBase, const std::string &busSelectionExpr
) {
	BExprMapBuilder bExprMapBuilder( bExpr, typeBase );

	BusSelectionVector busSelectionVector = getBusSelectionVector( busSelectionExpr );

	TypeBaseVector outputTypeBaseVector;

	for(
	 BusSelectionVector::iterator bsvItr = busSelectionVector.begin() ;
	 bsvItr != busSelectionVector.end();
	 (void)++bsvItr
	) {
		outputTypeBaseVector.push_back( bExprMapBuilder._bExprMap.find( *bsvItr )->second );
	}

	return outputTypeBaseVector;
}
