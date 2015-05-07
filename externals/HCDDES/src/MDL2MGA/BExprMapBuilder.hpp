#ifndef _ISIS_BEXPR_MAP_BUILDER
#define _ISIS_BEXPR_MAP_BUILDER

#include "SLSF.h"

#include <string>
#include <set>
#include <map>
#include <vector>

#include "BExprMapTree.hpp"

class BExprMapBuilder {
private:
	typedef std::map< std::string, SLSF::TypeBase > BExprMap;
	typedef std::vector< std::string > BusSelectionVector;
	typedef std::vector< SLSF::TypeBase > TypeBaseVector;

private:
	BExprMap _bExprMap;

	class TypeBaseRefSorter {
	public:
		bool operator()( const SLSF::TypeBaseRef &tbr1, const SLSF::TypeBaseRef &tbr2 ) {
			return static_cast< __int64 >( tbr1.MemberIndex() ) < static_cast< __int64 >( tbr2.MemberIndex() );
		}
	};

	typedef std::set< SLSF::TypeBaseRef, TypeBaseRefSorter > TypeBaseRefSet;

	void build( const std::string &prefix, BENodeSPVector &beBENodeSPVector, SLSF::TypeBase typeBase );
	void build( const std::string &prefix, SLSF::TypeBase typeBase );

	BExprMapBuilder( const std::string &bExpr, SLSF::TypeBase typeBase ) {
		if ( bExpr == "{}" ) {
			build( "", typeBase );
		} else {
			BENodeSPVector beBENodeSPVector = getBExprMapTree( bExpr );
			build( "", beBENodeSPVector, typeBase );
		}
	}

	static BusSelectionVector getBusSelectionVector( const std::string busSelectionExpr );

public:

	static TypeBaseVector getOutputVector(
	 const std::string &bExpr, SLSF::TypeBase typeBase, const std::string &busSelectionExpr
	);

};

#endif
