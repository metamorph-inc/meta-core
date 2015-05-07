#include <iostream>
#include <utility>
#include <vector>
#include <set>
#include <map>

//#include "boost/property_map/property_map.hpp"
#include "boost/graph/subgraph.hpp"
#include "boost/graph/adjacency_list.hpp"
//#include "boost/graph/properties.hpp"
#include "boost/graph/graph_traits.hpp"
#include "boost/graph/graph_utility.hpp"
//#include "boost/graph/topological_sort.hpp"

#include <gecode/int.hh>
#include <gecode/search.hh>

using namespace std;
using namespace boost;
using namespace Gecode;

typedef adjacency_list< vecS, vecS, directedS, property<vertex_color_t, int>, property< edge_index_t, int> > DepGraphBase;
typedef subgraph< DepGraphBase > DepGraph;
typedef boost::graph_traits<DepGraphBase>::edge_descriptor DepEdge;
typedef boost::graph_traits<DepGraphBase>::edge_iterator DepEdgeIter;
typedef boost::graph_traits<DepGraphBase>::vertex_descriptor DepVertex;
typedef boost::graph_traits<DepGraphBase>::vertex_iterator DepVertexIter;
typedef boost::graph_traits<DepGraphBase>::adjacency_iterator DepAdjIter;

class ExtractSubgraph {
    protected:
        DepGraph & _depgr;
        DepGraph & _depsubgr;
        typedef vector< DepVertex > DepPath;
        typedef set<DepVertex> DepCollection;
        typedef map<DepVertex, DepVertex > DepSubMap;
        DepCollection * _depcollector;
        DepSubMap _depSubMap;


    public:
        ExtractSubgraph(DepGraph & G, const DepVertex & firstvert, const DepVertex & lastvert) :
            _depgr(G), _depsubgr( _depgr.create_subgraph() ) {

            //_depsubgr = _depgr.create_subgraph();
            _depcollector = new DepCollection();

            DepPath p;
            cout << "Finding path from " << firstvert << " to " << lastvert << endl;
            findPath( firstvert, lastvert, p);


            DepCollection::iterator pVertex;
            for ( pVertex = _depcollector->begin(); pVertex != _depcollector->end(); pVertex ++ ) {
                DepVertex new_vert = add_vertex( *pVertex, _depsubgr );
                _depSubMap.insert( make_pair( new_vert, *pVertex ) );
            }

            delete _depcollector;

            //print_graph( _depsubgr, get(vertex_index, _depgr) );
            DepVertexIter dvi, dvi_end;
            for ( tie( dvi, dvi_end) = vertices(_depsubgr); dvi != dvi_end; dvi++ )
            {
                cout << "vertex " << *dvi << endl;

            }

            DepEdgeIter dei, dei_end;
            for ( tie( dei, dei_end ) = edges(_depsubgr); dei != dei_end; dei++ )
            {
                cout << "s: source " << source(*dei, _depsubgr) << " target " << target(*dei, _depsubgr) << endl;
                cout << "g: source " << _depSubMap[source(*dei, _depgr)]
                    << " target " << _depSubMap[target(*dei, _depgr)] << endl;
            }
        }

        // Technique from http://forums.sun.com/thread.jspa?threadID=623461&start=15&tstart=0
        void findPath( DepVertex current, DepVertex target, DepPath & path) {

            cout << "cur " << current << " target " << target << endl;
            if( current == target ) {
                path.push_back(current);
                // Add all of the vertices in the path to the subgraph
                DepPath::iterator pIter;
                cout << "Adding path: ";
                for ( pIter = path.begin(); pIter != path.end(); pIter++ )
                {
                    cout << *pIter << " ";
                    _depcollector->insert( *pIter );
                    //add_vertex( *pIter, _depsubgr );
                }
                cout << endl;
                path.pop_back();
                return;
            }

            // We already have a DAG
            //if(path.contains(current)) {
            //    return;
            //}

            path.push_back(current);

            DepAdjIter dai, dai_end;
            for( tie(dai, dai_end) = adjacent_vertices(current, _depgr); dai != dai_end; dai++ ) {

                findPath(*dai,target,path);
            }
            path.pop_back();
	}


};

class TestOrdering : public Space {
  protected:
        IntVarArray l;
  public:
        TestOrdering(const DepGraph & G, const DepVertex & firstvert, const DepVertex & lastvert):  l(*this, num_vertices(G), 0, num_vertices(G)-1) {
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

        TestOrdering( bool share, TestOrdering & s) : Space( share, s ) {
            l.update(*this, share, s.l);
        }

        virtual Space* copy( bool share) {
            return new TestOrdering(share,*this);
        }

        void print() const {
            std::cout << l << std::endl;
        }


};

int main()
{

    /*typedef std::pair<int, int> Edge;
    Edge edge_arr[6] = { Edge(0,1), Edge(2,4),
                      Edge(2,5),
                      Edge(0,3), Edge(1,4),
                      Edge(4,3) };
    DepGraph G(edge_arr, edge_arr + 6, 6);*/
    DepGraph G(6);
    add_edge(0,1,G);
    add_edge(2,4,G);
    add_edge(2,5,G);
    add_edge(0,3,G);
    add_edge(1,4,G);
    add_edge(4,3,G);

  /* Topological sort example */
  /*
  typedef std::vector< Vertex > container;
  container c;
  topological_sort(G, std::back_inserter(c));

  cout << "A topological ordering: ";
  for ( container::reverse_iterator ii=c.rbegin(); ii!=c.rend(); ++ii)
    cout << *ii << " ";
  cout << endl; */

    ExtractSubgraph es(G,0,3);

    /*  Gecode example */
    try {

        //TestOrdering * m = new TestOrdering(G,0,5);
        TestOrdering * m = new TestOrdering(G,0,3);
        DFS<TestOrdering> e(m);
        delete m;



        while (TestOrdering * s = e.next()) {
            s->print(); delete s;
        }
    } catch (Exception e) {
        std::cerr << "Gecode exception: " << e.what() << std::endl;
        return 1;

    }






    return 0;
}
