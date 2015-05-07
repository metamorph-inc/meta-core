#ifndef _ISIS_BEXPR_MAP_TREE
#define _ISIS_BEXPR_MAP_TREE

#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>

typedef boost::shared_ptr< void >                BENodeSPProxy;
typedef std::vector< BENodeSPProxy >             BENodeSPVector;
typedef std::pair< std::string, BENodeSPVector > BENode;
typedef boost::shared_ptr< BENode >              BENodeSP;

typedef std::vector< std::string > DotVector;
typedef std::vector< DotVector >   BExprOutputVector;

inline BENodeSP beNodeSPProxyCast( BENodeSPProxy &beNodeSPProxy ) {
	return boost::static_pointer_cast< BENode, void >( beNodeSPProxy );
}

BENodeSPVector getBExprMapTree( std::string bExpr );
BExprOutputVector getBExprOutputVector( std::string bExpr );

#endif
