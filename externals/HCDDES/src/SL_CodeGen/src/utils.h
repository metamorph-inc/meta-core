#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <string>
#include <set>
#include <list>
#include <cctype>	// for isalpha, isalnum
#include <boost/utility.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/topological_sort.hpp>
#include <boost/graph/depth_first_search.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/visitors.hpp>
#include <boost/graph/strong_components.hpp>
#include <boost/lexical_cast.hpp>

#include <set>
#include <list>

#include "SLSF.h"
#include "SFC.h"
#include "Uml.h"
#include "MatrixBuilder.hpp"
#include "UdmStatementBuilder.hpp"
#include "UdmExprsBuilder.hpp"
#include "mfile2SFC.hpp"
#include "SFCUdmEngine.hpp"

using namespace boost;

class SLSFUtils {
public:
	static bool &getHasRefSubsystem( void ) {
		static bool hasRefSubsystem = false;
		return hasRefSubsystem;
	}

	typedef std::set< SLSF::Subsystem > SubsystemSet;
	typedef std::set< SLSF::SubsystemRef > SubsystemRefSet;

	static bool hasInstanceInRefSubsystem( SLSF::Subsystem subsystem ) {
		SubsystemSet subsystemSet = subsystem.Instances();
		std::string subsystemName = subsystem.name();
		for( SubsystemSet::iterator sssItr = subsystemSet.begin() ; sssItr != subsystemSet.end() ; ++sssItr ) {
			Udm::Object object = sssItr->parent();
			std::string objectType = object.type().name();
			while( object != Udm::null && object.type() == SLSF::Subsystem::meta ) {
				SLSF::Subsystem ancestorSubsystem = SLSF::Subsystem::Cast( object );
				std::string ancestorSubsystemName = ancestorSubsystem.name();
				SubsystemRefSet subsystemRefSet = ancestorSubsystem.referedbySubsystemRef();
				if ( !subsystemRefSet.empty() ) return true;
				object = ancestorSubsystem.parent();
			}
		}
		return false;
	}
};

class SFUtils {
private:
	struct StatementSorter {
		bool operator()( const SFC::Statement &s1, const SFC::Statement &s2 ) const {
			return static_cast< int >( s1.statementIndex() ) < static_cast< int >( s2.statementIndex() );
		}
	};

	struct ArgSorter {
		bool operator()( const SFC::Arg &a1, const SFC::Arg &a2 ) const {
			return static_cast< int >( a1.argIndex() ) < static_cast< int >( a2.argIndex() );
		}
	};

public:
	typedef std::set< SFC::Declaration, StatementSorter > DeclarationSet;
	typedef std::set< SFC::Arg, ArgSorter >               ArgSet;

/*
	Topo Sort:
	a) construct a boost adjacency graph - use the node uuid as the vertex ID
	b) invoke the topo sort algo on it - gives a sorted container back 
	c) 
*/
	typedef std::vector< SLSF::Block > BlockVector;
	typedef std::set< SLSF::Line > LineSet;

	typedef boost::adjacency_list<  boost::vecS, boost::vecS, boost::directedS, boost::property< boost::vertex_color_t, boost::default_color_type >  > Graph;
	typedef boost::graph_traits< Graph >::vertex_descriptor Vertex;
	typedef boost::graph_traits< Graph >::edge_descriptor Edge;
	typedef std::list< Vertex > VertexList;
	typedef std::map< int, SLSF::Block > VertexIndexBlockMap;
	typedef std::map< SLSF::Block, int > BlockVertexIndexMap;

	typedef std::map< Vertex, boost::default_color_type > ColorMap;
	typedef boost::color_traits< boost::default_color_type > ColorTraits;

	class LoopDetector {
	public:

		class DFSVisitor {
		public:
			class Data {
			private:
				bool _hasCycle;
				bool _startVertexOnly;
				bool _startVertexProcessed;

			public:
				Data( bool startVertexOnly ) : _hasCycle( false ), _startVertexOnly( startVertexOnly ), _startVertexProcessed( false ) { }

				bool getHasCycle( void ) { return _hasCycle; }
				void setHasCycle( bool hasCycle ) { _hasCycle = hasCycle; }

				bool isStartVertexProcessed( void ) {
				   bool oldStartVertexProcessed = _startVertexProcessed;
				   _startVertexProcessed = _startVertexOnly;
				   return oldStartVertexProcessed;
				}
			};

			typedef boost::shared_ptr< Data > DataSP;

		private:
			ColorMap &_colorMap;
			DataSP _dataSP;

			void blackenColorMap( void ) {
				for( ColorMap::iterator clmItr = _colorMap.begin() ; clmItr != _colorMap.end() ; ++clmItr ) {
					clmItr->second = ColorTraits::black();
				}
			}

		public:
			DFSVisitor( ColorMap &colorMap, bool startVertexOnly = false ) : _colorMap( colorMap ), _dataSP(  new Data( startVertexOnly )  ) { }

			void initialize_vertex( Vertex &vertex, const Graph &graph ) { }
			void start_vertex( Vertex &vertex, const Graph &graph ) {
				if ( _dataSP->isStartVertexProcessed() ) {
					blackenColorMap();
				}
			}
			void discover_vertex( Vertex &vertex, const Graph &graph ) { }
			void examine_edge( const Edge &edge, const Graph &graph ) { }
			void tree_edge( const Edge &edge, const Graph &graph ) { }
			void back_edge( const Edge &edge, const Graph &graph ) {
				setHasCycle( true );
				blackenColorMap();
			}
			void forward_or_cross_edge( const Edge &edge, const Graph &graph ) { }
			void finish_vertex( Vertex &vertex, const Graph &graph ) { }

			void setHasCycle( bool hasCycle ) { _dataSP->setHasCycle( hasCycle ); }
			bool getHasCycle( void ) { return _dataSP->getHasCycle(); }
		};

	private:
		Graph _graph;
		ColorMap _colorMap;

	public:
		LoopDetector( Graph &graph ) : _graph( graph ) { }

		bool check( void ) {
			boost::associative_property_map< ColorMap > apmColorMap( _colorMap );
			DFSVisitor dfsVisitor( _colorMap );
			boost::depth_first_search( _graph, dfsVisitor, apmColorMap );
			return dfsVisitor.getHasCycle();
		}

		bool check( Vertex vertex ) {
			boost::associative_property_map< ColorMap > apmColorMap( _colorMap );
			DFSVisitor dfsVisitor( _colorMap, true );
			boost::depth_first_search( _graph, dfsVisitor, apmColorMap, vertex );
			return dfsVisitor.getHasCycle();
		}

	};


	//class MemoryStoreBlocks {
	//public:
	//	typedef std::vector< int > BlockNumberVector;

	//private:
	//	BlockNumberVector _dataStoreReadVector;
	//	BlockNumberVector _dataStoreWriteVector;

	//public:
	//	void addDataStoreRead( int number ) {
	//		_dataStoreReadVector.push_back( number );
	//	}
	//	void addDataStoreWrite( int number ) {
	//		_dataStoreWriteVector
	//	}
	//};


	static void DoTopologicalSort( SLSF::Subsystem subsystem );

	static std::string convertToCPlusPlusName( const std::string& name);

	static Udm::Object getTopArchetype( Udm::Object object );
	static Udm::Object getTopLevelInstance( Udm::Object object );

	static bool InstanceEquivalent( const Udm::Object &object1, const Udm::Object &object2 );

	static bool InstanceCorrespond( SLSF::Port port1, SLSF::Port port2 ) {
		return InstanceCorrespond( port1, port2 );
	}

	static bool InstanceCorrespond( Udm::Object object1, Udm::Object object2 ) {
		if (   (  object1 = getTopLevelInstance( object1 )  )  ==  Udm::null   ) return false;
		if (   (  object2 = getTopLevelInstance( object2 )  )  ==  Udm::null   ) return false;

		return object1.uniqueId() == object2.uniqueId();
	}

	typedef std::set< Udm::Object > ObjectSet;
	typedef std::list< Udm::Object > ObjectList;
	static ObjectSet getAllInstanceEquivalent( Udm::Object object );

	static SFC::Function getFunction( SFC::Statement statement );

	static std::string ZeroInit( SFC::DT dt, std::string argDeclBaseName, std::string indent );
	static void ZeroInit( SFC::Function function, SFC::ArgDeclBase argDeclBase );
	static void InitParameter( SFC::Function function, SLSF::Parameter parameter, SFC::LocalVar localVar );
	static void InitParameter( SFC::Function function, SFC::LocalVar localVar );
	static void Assignment( SFC::CompoundStatement block, SFC::ArgDeclBase output, SFC::ArgDeclBase input );

private:
	static bool &getPortTypeErrorValue( void ) {
		static bool portTypeErrorValue = false;
		return portTypeErrorValue;
	}
public:
	static bool getPortTypeError( void ) {
		return getPortTypeErrorValue();
	}
	static void setPortTypeError( void ) {
		getPortTypeErrorValue() = true;
	}

private:
	static bool &getErrorReportedValue( void ) {
		static bool errorReportedValue = false;
		return errorReportedValue;
	}
public:
	static bool getErrorReported( void ) {
		return getErrorReportedValue();
	}
	static void setErrorReported( void ) {
		getErrorReportedValue() = true;
	}

};


#endif //UTILS_H
