
#include "TestOrdering.h"

using namespace Gecode;

TestOrdering::TestOrdering( const DepGraph & G, const DepVertex & firstvert, const DepVertex & lastvert ) :  
	l(*this, num_vertices(G), 0, num_vertices(G)-1)
{
	IntVar firstnode(l[firstvert]), lastnode(l[num_vertices(G)-1]);

	IntArgs c(2);
	c[0]=1;
	c[1]=-1;
	for ( DepEdgeIter ei = edges(G).first; ei != edges(G).second; ei++ )
	{
		IntVarArgs x(2);
		x[0] = l[source(*ei, G)];
		x[1] = l[target(*ei, G)];
		linear( *this, c, x, IRT_LQ, 0 );
	}

	rel( *this, firstnode, IRT_EQ, 0 );
	rel( *this, lastnode, IRT_EQ, lastvert );

	distinct( *this, l );

	//branch(*this, l, INT_VAR_RND, INT_VAL_RND);
	//branch(*this, l, INT_VAR_SIZE_MIN, INT_VAL_MIN);
	branch(*this, l, INT_VAR_SIZE_MAX, INT_VAL_MAX);
}

