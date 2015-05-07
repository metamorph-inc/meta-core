#ifndef _ISIS_CALLGRAPHPRINT
#define _ISIS_CALLGRAPHPRINT

#include <algorithm>
#include <fstream>
#include <map>
#include <set>
#include <stdlib.h>
#include <string>
#include <vector>

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/properties.hpp>
#include <boost/property_map/property_map.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/graph/depth_first_search.hpp>
#include <boost/lexical_cast.hpp>


#include "SFC.h"
#include "Uml.h"

// THIS CLASS CONSTRUCTS A BOOST CALL GRAPH OF AN SFC PROGRAM MODEL.  SPECIFICALLY, IT CONSTUCTS SUCH
// A GRAPH FOR AN SFC MODEL THAT WAS TRANFORMED FROM AN ESMoL MODEL USING A "GReAT" TRANSFORM.
// THE MAIN POINT OF CONSTRUCTING A CALL-GRAPH IS TO SHOW THAT THERE IS NO RECURSION IN SUCH AN SFC MODEL,
// AND THEREFORE NO CHANCE OF INFINITE RECURSION.

// FROM INSPECTION OF SUCH AN SFC MODEL (USU. BY LOOKING AT IT'S "C"-CODE TRANSLATION), IT CAN BE SEEN THERE IS
// A POTENTIAL FOR RECURSION IN THE MODEL, I.E. FUNCTION (A) HAS A CALL TO FUNCTION (B), AND AS WELL FUNCTION (B)
// HAS A CALL TO FUNCTION (A).  THIS, IN PARTICULAR, IS TRUE OF THE FUNCTIONS WHOSE NAMES END IN "_enter".

// HOWEVER, THIS MUTUAL CALLING BETWEEN TWO OF THESE "_enter" FUNCTIONS CAN BE SHOWN TO BE NOT TRULY RECURSIVE.
// THIS IS BECAUSE AN "_enter" FUNCTION TAKES TWO PARAMETERS, "entryMode" AND "tpp".  FOR INSTANCE:
// 
// void STATE1_243_enter( int entryMode, int tpp );
//
// IMPORTANTLY, "entryMode" AND "tpp" ARE USED IN DETERMINING (VIA CONDITIONALS) WHICH OTHER FUNCTIONS
// THE GIVEN "_enter" FUNCTION CALLS. FOR EXAMPLE, WITHIN AN "_enter" FUNCTION, YOU WILL FIND SOMETHING LIKE THIS:
//
// if ( entryMode < -2 ) { STATE2_532_enter( -1, 0 ); }
//
// MOREOVER, "entryMode" AND "tpp" HAVE TWO IMPORTANT PROPERTIES:
// 1) THEIR VALUES ARE SET BY CONSTANT INTEGER ARGUMENT VALUES WHEN THE "_enter" FUNCTION IS CALLED.
// 2) THEIR VALUE ARE NOT MODIFIED WITHIN THE "_enter" FUNCTION BODY.
// BECAUSE OF THIS, AN "_enter" FUNCTION CAN BE THOUGHT OF AS A "FUNCTION TEMPLATE" WHICH IS INSTANTIATED
// BY THE VALUES OF "entryMode" AND "tpp" FOR A GIVEN CALL.  THAT IS:
//
// STATE1_243_enter( -1, 0 )
// STATE1_243_enter( 0, 1 )
//
// CAN BE THOUGHT OF A CALLS TO 2 *DIFFERENT* FUNCTIONS.  THIS IS DUE TO THE CONTROL LOGIC (I.E. CONDITIONALS)
// IN THE "_enter" FUNCTION BEING CONTROLLED BY THE (UNCHANGING, I.E. CONSTANT) VALUES OF "entryMode" AND "tpp".
// ONCE THIS IS TAKEN INTO ACCOUNT, IT CAN BE SHOWN, VIA A CALL-GRAPH, THAT THERE IS NO RECURSION IN THE SFC MODEL.


class CallGraph {

public:
	struct VertexProperties {
		std::string name;
		bool isAndSS;

		VertexProperties( void ) : name( "" ), isAndSS( false ) { }
	};

	typedef boost::adjacency_list< boost::setS, boost::vecS, boost::directedS, VertexProperties > Graph;
	typedef boost::graph_traits< Graph >::vertex_descriptor Vertex;
	typedef boost::graph_traits< Graph >::edge_descriptor Edge;
	typedef boost::graph_traits< Graph >::out_edge_iterator OutEdgeItr;
	typedef std::pair< OutEdgeItr, OutEdgeItr > OutEdgeItrPair;

private:
	std::string _mainFunctionName;
	Graph _graph;

	//
	// BUILD THE CALL GRAPH
	//

	void init( const SFC::Program &program );


public:
	CallGraph( const std::string &filename );
	CallGraph( const SFC::Program &program );

private:
	struct StatementOrder {
		bool operator()( const SFC::Statement &s1, const SFC::Statement &s2 ) {
			return static_cast< int >( s1.statementIndex() )  <  static_cast< int >( s2.statementIndex() );
		}
	};
	typedef std::set< SFC::Statement, StatementOrder > StatementSet;
	typedef StatementSet::iterator StatementSetItr;

	typedef std::set< SFC::Arg >        ArgSet;
	typedef std::set< SFC::ArgVal >     ArgValSet;
	typedef std::set< SFC::Condition >  ConditionSet;
	typedef std::set< SFC::Function >   FunctionSet;
	typedef std::set< SFC::StateVar >   StateVarSet;
	typedef std::set< SFC::StateLabel > StateLabelSet;

	typedef std::map< std::string, bool > FuncCoverageMap;
	static FuncCoverageMap &getFuncCoverageMap( void ) {
		static FuncCoverageMap funcCoverageMap;
		return funcCoverageMap;
	}

	static void registerFuncCoverage( const std::string &functionName ) {
		FuncCoverageMap::iterator fcmItr = CallGraph::getFuncCoverageMap().find( functionName );
		if ( fcmItr == CallGraph::getFuncCoverageMap().end() ) {
			CallGraph::getFuncCoverageMap()[ functionName ] = false;
		}
	}

	static void listUncalledFunctions( void );

	typedef std::set< std::string > FuncCondCoverageSet;
	static FuncCondCoverageSet &getFuncCondCoverageSet( void ) {
		static FuncCondCoverageSet funcCondCoverageSet;
		return funcCondCoverageSet;
	}

	static void listLackingFunctions( void );


	//
	// CLASS FOR KEEPING TRACK OF WHAT CallGraph VERTEX PERTAINS TO WHAT FUNCTION IN THE CALL GRAPH.
	// NameVertexMap MAPS A FUNCTION'S EXTENDED NAME TO A VERTEX REPRESENTING THAT FUNCTION IN THE CALL GRAPH.
	//
	typedef std::map< std::string, CallGraph::Vertex > NameVertexMap;
	static NameVertexMap &getNameVertexMap( void ) {
		static NameVertexMap nameVertexMap;
		return nameVertexMap;
	}

	typedef std::map< SFC::ConditionalBlock, bool > CondBlockTakenMap;
	static CondBlockTakenMap &getCondBlockTakenMap( void ) {
		static CondBlockTakenMap condBlockTakenMap;
		return condBlockTakenMap;
	}

	static void registerCondBlock( const SFC::ConditionalBlock &conditionalBlock ) {
		CondBlockTakenMap::iterator ctmItr = CallGraph::getCondBlockTakenMap().find( conditionalBlock );
		if ( ctmItr == CallGraph::getCondBlockTakenMap().end() ) {
			CallGraph::getCondBlockTakenMap()[ conditionalBlock ] = false;
		}
	}

	static int exprsCount( const SFC::Exprs &exprs );
	static int subStatementCount( const SFC::CompoundStatement &compoundStatement );

	static std::string identifyConditionalBlock( const SFC::ConditionalBlock &conditionalBlock ) {
		return std::string( "Statement Index " ) + 
		 boost::lexical_cast< std::string >(  SFC::Statement::Cast( conditionalBlock ).statementIndex()  );
	}
	static std::string identifyConditionalGroup( const SFC::ConditionalGroup &conditionalGroup ) {
		return std::string( "Statement Index " ) +
		 boost::lexical_cast< std::string >(  SFC::Statement::Cast( conditionalGroup ).statementIndex()  );
	}

	static void displayDeadCond( SFC::CompoundStatement compoundStatement );
	static void findDeadCode( void );

	class ArgValProcessor;
	typedef std::map< std::string, SFC::Exprs > ArgExprsMap;
	typedef std::map< std::string, int > ArgValMap;

	// TRI-STATE LOGIC TYPE - FOR PROCESSING ConditionalGroups (I.E. IF-ELSE-IF'S).
	enum Ternary { FALSE, PASS, TRUE };

	// THE FunctionProcessor CLASS CHARACTERIZES A FUNCTION IN SFC.  IT'S DATA MEMBERS STORE:
	// _function         : A HANDLE TO A GIVEN FUNCTION, I.E. THE ONE THAT THIS FunctionProcessor OBJECT REPRESENTS
	// _functionName     : THE NAME OF THE GIVEN FUNCTION
	// _hasBeenTraversed : A BOOLEAN VALUE THAT INDICATES IF THIS _function HAS ALREADY BEEN EVALUATED TO SEE WHAT
	//                     FUNCTIONS IT CALLS, AND SO NEED NOT BE EVALUATED AGAIN.
	// _entryMode        : VALUE WHICH PARAMETERIZES _function AND SO MAKES IT UNIQUE (LIKE A TEMPLATE INSTANTIATION).
	// _tpp              : ANOTHER VALUE WHICH PARAMETERIZES _function.
	class FunctionProcessor {
	private:
		const SFC::Function _function;
		const std::string _functionName; // NEEDED BECAUSE OF "LIBRARY FUNCTIONS"

		bool _hasBeenTraversed;
		int _entryMode;
		int _tpp;

		Graph &_graph;

		typedef std::map<  SFC::ConditionalBlock, std::pair< bool, bool >  > FuncCondBlockTakenMap;
		FuncCondBlockTakenMap _funcCondBlockTakenMap;

		void init( const ArgValProcessor &argValProcessor );

	public:
		FunctionProcessor( const SFC::Function &function, const ArgValProcessor &argValProcessor, Graph &graph ) :
		 _function( function ), _functionName( function.name() ), _entryMode( -99 ), _tpp( -99 ), _graph( graph )
		{
			init( argValProcessor );
		}

		FunctionProcessor( const std::string &functionName, const ArgValProcessor &argValProcessor, Graph &graph ) :
		 _functionName( functionName ), _entryMode( -99 ), _tpp( -99 ), _graph( graph )
		{
			init( argValProcessor );
		}

		~FunctionProcessor( void );


		// MEMBER ACCESS FUNCTIONS
		const std::string &getFunctionName( void ) const { return _functionName; }
		bool hasBeenTraversed( void ) const { return _hasBeenTraversed; }
		bool getHasEntryModeTpp( void ) const { return _entryMode != -99; }
		int getEntryMode( void ) const { return _entryMode; }
		int getTpp( void ) const { return _tpp; }

		// GENERATES UNIQUE FUNCTION NAME.  "entryMode" AND "tpp" ARE USED TO GENERATE THIS UNIQUE NAME, BUT
		// ONLY FOR FUNCTIONS THAT TAKE THEM AS PARAMETERS.
		std::string getExtendedName( void ) const {
			std::string extendedName = _functionName;
			if ( getHasEntryModeTpp() ) extendedName += std::string( "(" ) +
			 boost::lexical_cast< std::string >( _entryMode ) + "," + boost::lexical_cast< std::string >( _tpp ) + ")";
			return extendedName;
		}

		// THESE FUNCTIONS PROCESS PERTINTENT PARTS OF A FUNCTION TO DETERMINE WHICH OTHER FUNCTIONS IT CALLS, AND
		// UNDER WHAT CONDITIONS.  THEY RETURN A bool VALUE TO INDICATE IF A Return STATEMENT HAS BEEN ENCOUNTERED,
		// AND THUS NO SUCCEEDING STATEMENTS SHOULD BE EVALUATED.
		bool processStatement( const SFC::Statement &statement );
		bool processConditionalGroup( const SFC::ConditionalGroup &conditionalGroup );
		bool processCompoundStatement( const SFC::CompoundStatement &compoundStatement );
		bool processFunctionCall( const SFC::FunctionCall &functionCall );

		// THESE FUNCTION PROCESS EXPRESSIONS, IN PARTICULAR THOSE FOUND IN THE CONDITIONS OF CONDITIONALS.
		// THEY RETURN A TERNARY-LOGICAL VALUE TO PROCESS AN SFC CONSTRUCT CALLED A ConditionalGroup, WHICH
		// IS ANALOGOUS TO AN IF-ELSE-IF CONSTRUCT IN "C".

		// TERNARY LOGIC IS USED TO DETERMINE WHICH ConditionalBlock'S (I.E. "IF'S") TO EVALUATE WHEN EVALUATING
		// A ConditionalGroup.  ConditionalBlock CONDITIONS THAT INVOLVE A FUNCTION PARAMETER SUCH AS
		// "entryMode" OR "tpp" MAY BE DETERMINED TO BE "TRUE" OR "FALSE".
		// - IF A CONDITION EVALUATES TO "TRUE", THE ConditionalBlock BODY IS EVALUATED, AND, MOREOVER, THE
		// EVALUATION OF THE ConditinalGroup ENDS, IN ACCORDANCE WITH THE SEMANTICS OF AN "IF-ELSE-IF".
		// - IF A CONDITION EVALUATES TO "FALSE", THE ConditionalBlock BODY IS SKIPPED, AND THE NEXT ConditionalBlock
		// IN THE ConditionalGroup IS EVALUATED.
		// IF, HOWEVER, THE CONDITION OF A ConditionalBlock IS DEPENDENT, IN WHOLE OR IN PART, BY AN EXPRESSION
		// THAT DOES NOT DEPEND ON "entryMode" OR "tpp", THEN IT'S TRUTH VALUE IS ONLY DETERMINABLE AT RUN-TIME,
		// I.E. IT CANNOT BE DETERMINED BY THIS PROGRAM.  SUCH A CONDITION THUS EVALUATES TO "PASS" (THE
		// UNDETERMINED VALUE OF THE TERNARY LOGIC).
		// - IF A CONDITION EVALUATES TO "PASS", THE ConditinalBlock BODY IS EVALUATED, BUT THE NEXT ConditionalBlock
		// IN THE ConditionalGroup IS ALSO EVALUATED, AS "PASS" INDICATES THE *POSSIBILITY* OF FALSEHOOD.
		// 
		Ternary processExprs( const SFC::Exprs &exprs );
		Ternary processBinaryExprs( const SFC::BinaryExprs &binaryExprs );
		Ternary processUnaryExprs( const SFC::UnaryExprs &unaryExprs );
		Ternary processNullaryExprs( const SFC::NullaryExprs &nullaryExprs );

		void processSelf( void ) {
			processCompoundStatement(  SFC::CompoundStatement( _function )  );
		}

		int getIntVal( const SFC::Exprs &exprs ) const;
		static bool isEntryMode( const SFC::Exprs &exprs );

		FuncCondBlockTakenMap &getFuncCondBlockTakenMap( void ) { return _funcCondBlockTakenMap; }

		void funcRegisterCondBlock( const SFC::ConditionalBlock &conditionalBlock ) {
			FuncCondBlockTakenMap::iterator ctmItr = _funcCondBlockTakenMap.find( conditionalBlock );
			if ( ctmItr == getFuncCondBlockTakenMap().end() ) {
				getFuncCondBlockTakenMap()[ conditionalBlock ] = std::make_pair( false, false );
			}
		}

	private:

		// MAKE AN EDGE BETWEEN TWO FUNCTIONS
		void makeCallGraphEdge( const FunctionProcessor &functionProcessor ) const;


	//public:
	//	static makeArbitraryCallGraphEdge( const std::string &name1, const std::string &name2 ) {
	//
	//		NameVertexMap &nameVertexMap = CallGraph::getNameVertexMap();
	//
	//		NameVertexMap::iterator nvmItr1 = nameVertexMap.find( name1 );
	//		NameVertexMap::iterator nvmItr2 = nameVertexMap.find( name2 );
	//
	//		boost::add_edge( nvmItr1->second, nvmItr2->second, _graph );
	//	}
	};

	class ConditionalBlockProcessor {
	private:
		bool _result;
		bool _terminate;
		bool _definite;
		bool _indefinite;
		FunctionProcessor &_functionProcessor;

	public:
		ConditionalBlockProcessor( FunctionProcessor &functionProcessor	) :
		 _result( true ), _terminate( true ), _definite( false ), _indefinite( true ),
		 _functionProcessor( functionProcessor ) { }

		void operator()( const SFC::Condition &condition );
		bool getResult( void )     { return _result; }
		bool getTerminate( void )  { return _terminate; }
		bool getDefinite( void )   { return _definite; }
		bool getIndefinite( void ) { return _indefinite; }

		int getEntryMode( void ) const { return _functionProcessor.getEntryMode(); }
		int getTpp( void )       const { return _functionProcessor.getTpp(); }
	};

	class ArgValProcessor {
	private:
		ArgExprsMap _argExprsMap;
		ArgValMap _argValMap;
		FunctionProcessor *_functionProcessor;

		void init( void ) {
			_argValMap[ "entryMode" ] = -99;
			_argValMap[ "tpp" ] = -99;
		}

	public:
		ArgValProcessor( void ) : _functionProcessor( 0 ) {
			init();
		}

		ArgValProcessor( FunctionProcessor &functionProcessor ) : _functionProcessor( &functionProcessor ) {
			init();
		}

		void operator()( const SFC::ArgVal &argVal );

		int getEntryModeArg( void ) const { return _argValMap.find( "entryMode" )->second; }
		int getTppArg( void ) const { return _argValMap.find( "tpp" )->second; }
	};

	//
	// GRAPH ACCESS
	//
public:
	Graph &getGraph( void ) { return _graph; }

	//
	// DEPTH-FIRST-SEARCH ANALYSIS
	//
private:
	class DFSAnalysis : public boost::dfs_visitor<> {
	private:
		Vertex _startVertex;
		int &_maxDepth;
		bool _diagnostics;

		typedef std::map< Vertex, Vertex > VertexSequenceMap;
		VertexSequenceMap _predecessorMap;
		VertexSequenceMap _successorMap;
		
		struct DepthData {
			int _depth;
			int _maxDepth;

			DepthData( int depth = 0, int maxDepth = 0 ) : _depth( depth ), _maxDepth( maxDepth ) { }
		};

		typedef std::map< Vertex, DepthData > DepthMap;
		typedef DepthMap::iterator DepthMapItr;
		DepthMap _depthMap;

	public:
		DFSAnalysis( const Vertex &startVertex, int &maxDepth, bool diagnostics = false )
		 : _startVertex( startVertex ), _maxDepth( maxDepth ), _diagnostics( diagnostics ) {
			_depthMap.insert(  std::make_pair( _startVertex, DepthData() )  );
		}

		void forward_or_cross_edge( Edge edge, const Graph &graph );
		void tree_edge( Edge edge, const Graph &graph );
		void back_edge( Edge edge, const Graph &graph );

		void finish_vertex( Vertex vertex, const Graph &graph );
    };

public:
	int dfsAnalyze( bool diagnostics = false ) {

		typedef std::map< Vertex, boost::default_color_type > ColorMap;
		ColorMap colorMap;
		boost::associative_property_map< ColorMap > colorPropMap( colorMap );

		int maxDepth = 0;

		Vertex startVertex = CallGraph::getNameVertexMap().find( _mainFunctionName )->second;
		boost::depth_first_search(
		 _graph,
		 DFSAnalysis( startVertex, maxDepth, diagnostics ),
		 colorPropMap,
		 startVertex
		);

		return maxDepth;
	}

	//
	// PRINT GRAPH
	//
private:
	class DottyPropertyWriter {
	private:
		const Graph &_graph;

	public:
		DottyPropertyWriter( const Graph &graph ) : _graph( graph ) { }

		void operator()( ostream &os, const Vertex &vertex ) {
			os << "[ label=\"" << _graph[ vertex ].name << "\"";
			if ( _graph[ vertex ].isAndSS ) {
				os << ", shape=\"rectangle\"";
			}
			os << " ]";
		}
	};

public:
	void print( const std::string &filename ) {
		std::ofstream dottyStream( filename.c_str() );
		write_graphviz(
		 dottyStream, _graph, DottyPropertyWriter( _graph ), boost::default_writer(), boost::default_writer()
		);
	}

};

#endif
