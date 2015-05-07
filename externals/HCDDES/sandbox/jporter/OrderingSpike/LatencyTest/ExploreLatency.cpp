#include <iostream>
#include <vector>
#include <map>
#include <sstream>

#include "ExtractSubgraph.h"
#include "TestOrdering.h"

#include "permlib/permutation.h"
#include "permlib/permutationword.h"

using namespace std;
using namespace permlib;

typedef Permutation PERM;

string stringify( const PERM & p )
{
    ostringstream out;
    out << p;
    return out.str();
}

int main( int argc, char *argv[] )
{
    unsigned long long N = 6;
    DepGraph G(N);
    add_edge(0,1,G);
    add_edge(0,2,G);
    add_edge(1,3,G);
    add_edge(2,4,G);
    add_edge(3,5,G);
    add_edge(4,5,G);

    vector< unsigned long long > order;
    try {

        TestOrdering * m = new TestOrdering(G,0,N-1);
        Gecode::DFS< TestOrdering > e(m);
        delete m;

        /* while( */ TestOrdering * s = e.next(); /* ) { */
            s->print(); 

            s->getValues( order );
          
            // From here:
            //    enumerate permutations on (2..N-1)
            //    apply them and check latency
            //    compare with the predicted latency
            //    print the results
            
 
            delete s;
        /* } */

    } catch (Gecode::Exception e) {
 
        cerr << "Gecode exception: " << e.what() << endl;
        return 1;
    }

    PERM i(6, string( "" ) );
    PERM a(6, string( "2 3" ) );
    PERM b(6, string( "3 4" ) );
    PERM c(6, string( "4 5" ) );
    // vector< PERM > gens(3);

    map< string, vector<string> > perms;

    perms[ stringify(i) ].push_back(string("e"));
    perms[ stringify(a) ].push_back(string("a"));
    perms[ stringify(b) ].push_back(string("b"));
    perms[ stringify(c) ].push_back(string("c"));
    perms[ stringify(a * b) ].push_back(string("ab"));
    perms[ stringify(a * c) ].push_back(string("ac"));
    perms[ stringify(b * a) ].push_back(string("ba"));
    perms[ stringify(b * c) ].push_back(string("bc"));
    perms[ stringify(c * a) ].push_back(string("ca"));
    perms[ stringify(c * b) ].push_back(string("cb"));
    perms[ stringify(a * b * a) ].push_back(string("aba"));
    perms[ stringify(a * b * c) ].push_back(string("abc"));
    perms[ stringify(a * c * a) ].push_back(string("aca"));
    perms[ stringify(a * c * b) ].push_back(string("acb"));
    perms[ stringify(b * a * b) ].push_back(string("bab"));
    perms[ stringify(b * a * c) ].push_back(string("bac"));
    perms[ stringify(b * c * a) ].push_back(string("bca"));
    perms[ stringify(b * c * b) ].push_back(string("bcb"));
    perms[ stringify(c * a * b) ].push_back(string("cab"));
    perms[ stringify(c * a * c) ].push_back(string("cac"));
    perms[ stringify(c * b * a) ].push_back(string("cba"));
    perms[ stringify(c * b * c) ].push_back(string("cbc"));
    
    perms[ stringify(a * b * a * b) ].push_back(string("abab"));
    perms[ stringify(a * b * a * c) ].push_back(string("abac"));
    perms[ stringify(a * b * c * a) ].push_back(string("abca"));
    perms[ stringify(a * b * c * b) ].push_back(string("abcb"));
    perms[ stringify(a * c * a * b) ].push_back(string("acab"));
    perms[ stringify(a * c * a * c) ].push_back(string("acac"));
    perms[ stringify(a * c * b * a) ].push_back(string("acba"));
    perms[ stringify(a * c * b * c) ].push_back(string("acbc"));
    perms[ stringify(b * a * b * a) ].push_back(string("baba"));
    perms[ stringify(b * a * b * c) ].push_back(string("babc"));
    perms[ stringify(b * a * c * a) ].push_back(string("baca"));
    perms[ stringify(b * a * c * b) ].push_back(string("bacb"));
    perms[ stringify(b * c * a * b) ].push_back(string("bcab"));
    perms[ stringify(b * c * a * c) ].push_back(string("bcac"));
    perms[ stringify(b * c * b * a) ].push_back(string("bcba"));
    perms[ stringify(b * c * b * c) ].push_back(string("bcbc"));
    perms[ stringify(c * a * b * a) ].push_back(string("caba"));
    perms[ stringify(c * a * b * c) ].push_back(string("cabc"));
    perms[ stringify(c * a * c * a) ].push_back(string("caca"));
    perms[ stringify(c * a * c * b) ].push_back(string("cacb"));
    perms[ stringify(c * b * a * b) ].push_back(string("cbab"));
    perms[ stringify(c * b * a * c) ].push_back(string("cbac"));
    perms[ stringify(c * b * c * a) ].push_back(string("cbca"));
    perms[ stringify(c * b * c * b) ].push_back(string("cbcb"));
    
    /*
    cout << "ident: " << i << endl;
    cout << "a: " << a << " b: " << b << endl;
    cout << "a * b: " << (a * b) << " b * a: " << (b * a) << endl;
    cout << "a * b * a: " << (a * b * a) << " b * a * b: " << (b * a * b) << endl;
    cout << "a * b * a == b * a * b: " << (( (a * b * a) == (b * a * b) ) ? string("true") : string("false")) << endl;
    */

    map< string, vector<string> >::iterator permIter;
    vector< string >::iterator strIter;

    for ( permIter = perms.begin(); permIter != perms.end(); permIter++ )
    {
        cout << permIter->first << ": ";
        for ( strIter = (permIter->second).begin(); strIter != (permIter->second).end(); strIter++ )
        {
            cout << "( " << *strIter << " ) ";
        }
        cout << endl;
    }

    // Explore over the permutations of S4
    vector< PERM > permlist;
    permlist.push_back( i );
    permlist.push_back( a );
    permlist.push_back( a * c );
    permlist.push_back( b * a );
    permlist.push_back( c * b * a );
    permlist.push_back( b * c * b * a );
    permlist.push_back( b * a * c );
    permlist.push_back( a * b * a );
    permlist.push_back( b * a * c * b );
    permlist.push_back( a * b );
    permlist.push_back( a * c * b * a );
    permlist.push_back( a * c * b );
    permlist.push_back( a * b * c * b );
    permlist.push_back( a * b * a * c );
    permlist.push_back( a * b * c );
    permlist.push_back( b );
    permlist.push_back( c * b );
    permlist.push_back( b * c * b );
    permlist.push_back( b * c );
    permlist.push_back( c );

    // Capture the resulting ordering from the search,
    //  and then multiply it by elements of S4
    //  Simulate and record delay for each element
    //
    //  Predict the delay for each element and compare 

    // TODO: Think about this -- the order permutation is an execution order
    //   over slots in the graph.  The S4 elements are slot transpositions.
    //   We need to make sure we're performing compatible operations.  
    //   On the other hand, each order permutation represents a change from
    //   a permutation of a particular ordering (the "standard" ordering).
    //   Can we analyze the latencies from the perspective of the standard
    //   ordering?  Then we may only need a single set of delay operations,
    //   along with knowledge of how a particular generated ordering 
    //   permutes the standard ordering.
 
    ostringstream out;
    for ( unsigned long idx = 1; idx < order.size() - 1; idx++ )
    {
        out << order[idx]+1 << " ";
        cout << "Order[" << idx << "]: " << order[idx] << endl;
    }
    cout << "Order: " << out.str() << endl;

    PERM ord( 6, out.str() );

    cout << "Products: " << endl;
    vector< PERM > permprods;
    for ( vector< PERM >::iterator pIter = permlist.begin(); pIter != permlist.end(); pIter++ ) {
 
        permprods.push_back( ord * (*pIter) );
        cout << ( ord * (*pIter) ) << endl;
    }

    return 0;
}

