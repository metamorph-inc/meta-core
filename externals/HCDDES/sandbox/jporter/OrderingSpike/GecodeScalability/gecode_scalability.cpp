#include <iostream>
#include <fstream>
#include <utility>
#include <iterator>
#include <vector>
#include <set>
#include <map>


#include "boost/pending/property.hpp"
#include "boost/graph/properties.hpp"
#include "boost/property_map/property_map.hpp"
#include "boost/graph/subgraph.hpp"
#include "boost/graph/adjacency_list.hpp"
#include "boost/graph/graph_traits.hpp"
#include "boost/graph/graph_utility.hpp"
#include "boost/graph/topological_sort.hpp"
#include "boost/graph/graphviz.hpp"
#include "boost/graph/graphml.hpp"

#include <gecode/int.hh>
#include <gecode/search.hh>

using namespace std;
using namespace boost;
using namespace Gecode;

typedef long idx_t;
typedef long len_t;

typedef struct
{
    idx_t  min_order;
    idx_t  max_order;
    len_t  asap;
    len_t  alap;

} DepVertexPropStruct;

typedef adjacency_list< vecS, vecS, bidirectionalS,
            property<vertex_color_t, idx_t,
                //property<vertex_name_t, string,
                    boost::property< boost::vertex_distance_t, idx_t,
                        DepVertexPropStruct > >,
            property< edge_index_t, idx_t,
                boost::property< boost::edge_weight_t, idx_t > > > DepGraph;
typedef subgraph< DepGraph > DepGraphSub;
typedef boost::graph_traits<DepGraph>::edge_descriptor DepEdge;
typedef boost::graph_traits<DepGraph>::edge_iterator DepEdgeIter;
typedef boost::graph_traits<DepGraph>::in_edge_iterator DepInEdgeIter;
typedef boost::graph_traits<DepGraph>::out_edge_iterator DepOutEdgeIter;
typedef boost::graph_traits<DepGraph>::vertex_descriptor DepVertex;
typedef boost::graph_traits<DepGraph>::vertex_iterator DepVertexIter;
typedef boost::graph_traits<DepGraph>::adjacency_iterator DepAdjIter;

typedef set< idx_t > indexSet;
typedef vector< indexSet > rankVector;

class GraphRanking
{
public:
    GraphRanking( DepGraph & dg ) {

        indexSet current_set, next_set;
        DepVertexIter vi, vi_end;
        for ( tie( vi, vi_end ) = vertices(dg); vi != vi_end; vi++ )
        {
            if ( in_degree( *vi, dg ) == 0 )
                current_set.insert( get( vertex_index, dg, *vi ) );
        }

        indexSet edge_tokens;
        typedef property_map< DepGraph, edge_index_t >::type EdgeIdxMap_t;
        EdgeIdxMap_t emap = get( edge_index, dg );

        // Number the edges
        DepEdgeIter dei, dei_end;
        idx_t edge_count = 0;
        for ( tie( dei, dei_end ) = edges( dg ); dei != dei_end; dei++ )
        {
            put( emap, *dei, edge_count++ );
        }

        // Iterate until we have no more successors
        bool done = false;
        while ( !done ) // next rank loop
        {
            indexSet keepers;
            indexSet::iterator curIter;
            for ( curIter = current_set.begin();
                  curIter != current_set.end(); curIter++ )
            {
                bool found_edge_without_token = false;

                DepInEdgeIter iei, iei_end;
                for ( tie( iei, iei_end ) = in_edges( *curIter, dg );
                           iei != iei_end; iei++ )
                {
                     if ( edge_tokens.count( get( emap, *iei ) ) == 0 )
                     {
                         found_edge_without_token = true;
                         // put the current vertex into the list for the next cycle
                         next_set.insert( get( vertex_index, dg, *curIter ) );
                         break;
                     }
                }

                // Only add the vertex to this rank if we have seen
                // all of its input tokens
                if ( !found_edge_without_token )
                {
                    keepers.insert( get( vertex_index, dg, *curIter ) );
                }
            }

            for ( indexSet::iterator keepIter = keepers.begin();
                    keepIter != keepers.end(); keepIter++ )
            {
                // mark output edges if we have all input edges
                // put the target vertices into the next set
                DepOutEdgeIter oei, oei_end;
                for ( tie( oei, oei_end ) = out_edges( *keepIter, dg );
                           oei != oei_end; oei++ )
                {
                    edge_tokens.insert( get( emap, *oei ) );
                    next_set.insert( get( vertex_index, dg, target( *oei, dg ) ) );
                }
            }

            ranks.push_back( keepers );
            current_set = next_set;
            next_set.clear();

            if ( edge_tokens.size() == num_edges( dg ) ) // the correct end criterion is when all edge tokens are true - this could exit early
            {
                done = true;
            }
        }

        ranks.push_back( current_set );
    }

    virtual ~GraphRanking() { }

    void print() {
        idx_t count = 0;
        cout << "Ranking: " << endl;
        for ( rankVector::iterator rIter = ranks.begin(); rIter != ranks.end();
              rIter++ )
        {
            cout << count << ": ";

            for ( indexSet::iterator vIter = ((*rIter).begin()); vIter != ((*rIter).end());
                  vIter++ )
            {
                cout << *vIter << " ";
            }
            cout << endl;
            count++;
        }
        cout << "done." << endl;

    }

    indexSet getRank( idx_t idx ) { return ranks[idx]; }
    idx_t num_ranks() { return ranks.size(); }

protected:
    rankVector ranks;
    vector< len_t > rank_durations;

};


class TestOrdering : public Space {
  protected:
        IntVarArray l;
  public:
        TestOrdering(const DepGraph & G, const DepVertex & firstvert, const DepVertex & lastvert):  l(*this, num_vertices(G), 0, num_vertices(G)-1) {
            //IntVar firstnode(l[firstvert]), lastnode(l[num_vertices(G)-1]);
            IntVar firstnode(l[firstvert]), lastnode(l[lastvert]);

            IntArgs c(2);
            c[0]=1;
            c[1]=-1;
            for ( DepEdgeIter ei = edges(G).first; ei != edges(G).second; ei++ )
            {
                IntVarArgs x(2);
                x[0] = l[source(*ei, G)];
                x[1] = l[target(*ei, G)];
                linear( *this, c, x, IRT_LE, 0 );
            }

            rel( *this, firstnode, IRT_EQ, 0 );
            rel( *this, lastnode, IRT_EQ, num_vertices(G)-1 );

            distinct( *this, l );

            //branch(*this, l, INT_VAR_RND, INT_VAL_RND);
            branch(*this, l, INT_VAR_SIZE_MAX, INT_VAL_MIN);
            //branch(*this, l, INT_VAR_NONE, INT_VAL_MIN);
            //branch(*this, l, INT_VAR_SIZE_MAX, INT_VAL_MAX);
        }

        TestOrdering( bool share, TestOrdering & s) : Space( share, s ) {
            l.update(*this, share, s.l);
        }

        virtual Space* copy( bool share) {
            return new TestOrdering(share,*this);
        }

        void print() const {
            std::cout << l << std::endl;
            vector< idx_t > vertex_results( l.size(), 0 );
            for ( idx_t idx = 0; idx < l.size(); idx++ )
            {
                // Switch from positions to vertices
                vertex_results[ l[idx].val() ] = idx;
            }
            cout << "vertices: ";
            for ( idx_t idx = 0; idx < l.size(); idx++ )
            {
                cout << vertex_results[idx] << " ";
            }
            cout << endl;
        }

        vector< idx_t > get_results() {
            vector< idx_t > vertex_results( l.size(), 0 );
            for ( idx_t idx = 0; idx < l.size(); idx++ )
            {
                // Switch from positions to vertices
                vertex_results[ l[idx].val() ] = idx;
            }
            return vertex_results;
        }

        // iterate forward and backward, calculating the min/max ordering position
        //   for each vertex, and the ASAP/ALAP times (with respect to the critical path)
        // based on MCP code from Wu @ UMN
        void add_ranges( DepGraph & dg, DepVertex & init, DepVertex & fin )
        {
            idx_t init_idx, final_idx;
            len_t init_len, final_len, cp_len = 0;

            vector< idx_t > flevel( num_vertices(dg), 0 );

            // iterate forward
            init_idx = 1;
            DepVertex cur = init;

            // iterate backward
            final_idx = num_vertices( dg );
            vector< idx_t > ready_q;
            ready_q.push_back( fin );


        }
};

class VertexOrdering
{
public:
    VertexOrdering( const vector< idx_t > & order, const DepGraph & dg ) :
        _ordering( order ), _edge_latency( num_edges( dg ), 0 )
    {
        // Figure out the latency of a particular ordering

    }

protected:
    vector< idx_t > _ordering;
    unsigned long long _latency;
    vector< unsigned long long  > _edge_latency;

};

int main( int argc, char *argv[] )
{
    string filename( "test17.gml" );
    if ( argc > 1 )
        filename = argv[1];

    /*typedef std::pair<int, int> Edge;
    Edge edge_arr[6] = { Edge(0,1), Edge(2,4),
                      Edge(2,5),
                      Edge(0,3), Edge(1,4),
                      Edge(4,3) };
    DepGraph G(edge_arr, edge_arr + 6, 6);*/
    //DepGraph G(6);
    //add_edge(0,1,G);
    //add_edge(2,4,G);
    //add_edge(2,5,G);
    //add_edge(0,3,G);
    //add_edge(1,4,G);
    //add_edge(4,3,G);

  /* Topological sort example */
  /*
  typedef std::vector< Vertex > container;
  container c;
  topological_sort(G, std::back_inserter(c));

  cout << "A topological ordering: ";
  for ( container::reverse_iterator ii=c.rbegin(); ii!=c.rend(); ++ii)
    cout << *ii << " ";
  cout << endl; */

    //ExtractSubgraph es(G,0,3);

    /* Get graph from dot file */
    cout << "Reading from file " << filename << "." << endl;
    DepGraph G(0);
    dynamic_properties dp;
    dp.property("vertex_distance", get( vertex_distance_t(), G ));
    dp.property("edge_weight", get( edge_weight_t(), G ));
    //dp.property("node_id", get(vertex_name, G));

    ifstream infile( filename.c_str() );
    //if ( !read_graphviz( infile, G, dp ) )
    //{
    //     cout << "Unable to read file " << filename << "!" << endl;
    //}

    try {
        read_graphml( infile, G, dp );
    }
    catch ( directed_graph_error & e1 ) {
        cout << "Directed graph error: " << e1.what() << endl;
    }
    catch ( parse_error & e2 ) {
        cout << "Parse error: " << e2.what() << endl;
    }
    catch ( graph_exception & en ) {
        cout << "Graph exception: " << en.what() << endl;
    }

    //write_graphviz( cout, G );

    // Test the ranking function
    GraphRanking gr( G );
    gr.print();

    vector< DepVertex > c;
    topological_sort( G, std::back_inserter( c ) );

    for ( idx_t index = 0; index < c.size(); index++ )
    {
        cout << index << ": " << get( vertex_index, G, c[(c.size()-index)-1] )
            << " (" << get( vertex_distance, G, c[(c.size()-index)-1] ) << ")" << endl;
    }

    /*  Gecode example */
    try {

        //TestOrdering * m = new TestOrdering(G,0,5);
        //TestOrdering * m = new TestOrdering(G,0,3);
        cout << "First: " << get( vertex_index, G, c[c.size()-1] )
             << " Last: " << get( vertex_index, G, c[0] ) << endl;
        //cout << "First: " << num_vertices(G)-2 << " Last: " << num_vertices(G)-1 << endl;
        //TestOrdering * m = new TestOrdering(G,c[c.size()-1],c[0]);
        TestOrdering * m = new TestOrdering(G, get( vertex_index, G, c[c.size()-1] ),
                                            get( vertex_index, G, c[0] ) );

        DFS<TestOrdering> e(m);
        delete m;



        if (TestOrdering * s = e.next()) {
            s->print(); delete s;
        }
        else {
            cout << "No solution found!!!" << endl;
        }
        if (TestOrdering * s = e.next()) {
            s->print(); delete s;
        }
        else {
            cout << "No solution found!!!" << endl;
        }
        if (TestOrdering * s = e.next()) {
            s->print(); delete s;
        }
        else {
            cout << "No solution found!!!" << endl;
        }
        if (TestOrdering * s = e.next()) {
            s->print(); delete s;
        }
        else {
            cout << "No solution found!!!" << endl;
        }
        if (TestOrdering * s = e.next()) {
            s->print(); delete s;
        }
        else {
            cout << "No solution found!!!" << endl;
        }
    } catch (Exception e) {
        std::cerr << "Gecode exception: " << e.what() << std::endl;
        return 1;

    }






    return 0;
}
