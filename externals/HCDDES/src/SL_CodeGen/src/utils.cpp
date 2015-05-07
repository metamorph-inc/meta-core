#include "utils.h"

struct BlockProperties {
	int _graphIndex;

};


void SFUtils::DoTopologicalSort( SLSF::Subsystem subsystem ) {

	int vertexIndex = 0;
	VertexIndexBlockMap vertexIndexBlockMap;
	BlockVertexIndexMap blockVertexIndexMap;
	BlockVector blockVector = subsystem.Block_kind_children();
	for( BlockVector::iterator blvItr = blockVector.begin(); blvItr != blockVector.end(); ++blvItr, ++vertexIndex ) {
		SLSF::Block block = *blvItr;
		vertexIndexBlockMap[ vertexIndex ] = block;
		blockVertexIndexMap[ block ] = vertexIndex;

		std::string blockType = block.BlockType();
		if ( blockType == "UnitDelay" )	{  // check on other delay blocks as well ...
			++vertexIndex;	               // UnitDelay is vertexed twice - one for outputs (timestep n-1), and one for inputs: vertexIndex is as destination, vertexIndex + 1 is as source
		}
	}

	Graph graph( vertexIndex );
	LineSet lineSet = subsystem.Line_kind_children();
	for( LineSet::iterator lnsItr = lineSet.begin(); lnsItr != lineSet.end() ; ++lnsItr ) {

		SLSF::Line line = *lnsItr;
		SLSF::Port sourcePort = line.srcLine_end();
		SLSF::Port destinationPort = line.dstLine_end();
		SLSF::Block sourceBlock = sourcePort.Block_parent();
		SLSF::Block destinationBlock = destinationPort.Block_parent();

		if ( sourceBlock == subsystem || destinationBlock == subsystem ) continue;

		int sourceBlockVertexIndex = blockVertexIndexMap[ sourceBlock ];
		if (  static_cast< std::string >( sourceBlock.BlockType() ) == "UnitDelay"  ) {
			++sourceBlockVertexIndex;
		}

		int destinationBlockVertexIndex = blockVertexIndexMap[ destinationBlock ];

		boost::add_edge( sourceBlockVertexIndex, destinationBlockVertexIndex, graph );
	}

	LoopDetector loopDetector( graph );

	if ( loopDetector.check() ) {
		// TODO: add support for loops involving integrator and other stateful blocks 

		// Determine what Blocks caused the loop
		typedef std::map< Vertex, int > VertexStrongComponentIndexMap;
		VertexStrongComponentIndexMap vertexStrongComponentIndexMap;
		boost::associative_property_map< VertexStrongComponentIndexMap > apmVertexStrongComponentIndexMap( vertexStrongComponentIndexMap );
		strong_components( graph, apmVertexStrongComponentIndexMap );

		typedef std::vector< Vertex > VertexVector;
		typedef std::map< int, VertexVector > StrongComponentIndexVertexGroupMap;
		StrongComponentIndexVertexGroupMap strongComponentIndexVertexGroupMap;
		for( VertexStrongComponentIndexMap::iterator vsmItr = vertexStrongComponentIndexMap.begin(); vsmItr != vertexStrongComponentIndexMap.end(); ++vsmItr ) {
			strongComponentIndexVertexGroupMap[ vsmItr->second ].push_back( vsmItr->first );
		}

		std::string error( "Dataflow Graph '" + static_cast< std::string >( subsystem.Name() ) + "' has unhandled loops: " );
		for( StrongComponentIndexVertexGroupMap::iterator svmItr = strongComponentIndexVertexGroupMap.begin(); svmItr != strongComponentIndexVertexGroupMap.end(); ++svmItr ) {
			VertexVector vertexVector = svmItr->second;
			if ( vertexVector.size() <= 1 ) continue;
			error.append( "\n" );
			for( VertexVector::iterator vtvItr = vertexVector.begin(); vtvItr != vertexVector.end(); ++vtvItr ) {
				error.append( blockVector[ *vtvItr ].getPath("/") );
				error.append( ", " );
			}
			error.erase( error.size() - 2 );
		}

		throw udm_exception(error);
	}

	typedef std::set< Vertex > VertexSet;
	typedef std::map< int, VertexSet > PriorityVertexSetMap;

	PriorityVertexSetMap priorityVertexSetMap;
	for( BlockVector::iterator blvItr = blockVector.begin() ; blvItr != blockVector.end() ; ++blvItr ) {

		SLSF::Block block = *blvItr;
		int priority = block.Priority();
		if ( priority == 0 ) continue;

		Vertex vertex = blockVertexIndexMap[ block ];
		priorityVertexSetMap[ priority ].insert( vertex );
	}

	if ( priorityVertexSetMap.size() > 1 ) {
		PriorityVertexSetMap::iterator lstPvmItr = priorityVertexSetMap.end();
		--lstPvmItr;
		for( PriorityVertexSetMap::iterator pvmItr = priorityVertexSetMap.begin() ; pvmItr != lstPvmItr ; ) {
			PriorityVertexSetMap::iterator nxtPvmItr = pvmItr;
			++nxtPvmItr;

			VertexSet &higherPriorityVertexSet = pvmItr->second;
			VertexSet &lowerPriorityVertexSet = nxtPvmItr->second;

			for( VertexSet::iterator hvsItr = higherPriorityVertexSet.begin() ; hvsItr != higherPriorityVertexSet.end() ; ++hvsItr ) {
				for( VertexSet::iterator lvsItr = lowerPriorityVertexSet.begin() ; lvsItr != lowerPriorityVertexSet.end() ; ++lvsItr ) {
					boost::add_edge( *hvsItr, *lvsItr, graph );
					LoopDetector loopDetector( graph );
					if (  loopDetector.check( *hvsItr )  ) {
						SLSF::Block higherPriorityBlock = vertexIndexBlockMap[ *hvsItr ];
						SLSF::Block lowerPriorityBlock = vertexIndexBlockMap[ *lvsItr ];

						std::cerr << "WARNING:  Cannot implement priority difference between block \"" << higherPriorityBlock.getPath( "/" ) << "\" (Priority = " << *hvsItr << ") and " << std::endl;
						std::cerr << "          block \"" << lowerPriorityBlock.getPath( "/" ) << "\" (Priority = " << *lvsItr << "): contradicts topology of subsystem or other implemented block priority order." << std::endl;
						boost::remove_edge( *hvsItr, *lvsItr, graph );
					}
				}
			}
			pvmItr = nxtPvmItr;
		}
	}

	VertexList vertexList;
	boost::topological_sort(  graph, std::back_inserter( vertexList )  );


	/* PUT ALL "DataStoreMemory" BLOCKS AT END OF "C" SO THEY HAVE HIGHEST PRIORITY */
	VertexList::reverse_iterator vtlRit = vertexList.rbegin();
	while( vtlRit != vertexList.rend() ) {
		int index = *vtlRit;
		SLSF::Block block = vertexIndexBlockMap[ index ];

		(void)++vtlRit;
		if ( block != Udm::null && static_cast< std::string >( block.BlockType() ) == "DataStoreMemory"  ) {
			VertexList::reverse_iterator vtlRit2 = vtlRit;
			vertexList.splice( vertexList.end(), vertexList, vtlRit2.base() );
		}
	}

	int priority = 0;
	for( VertexList::reverse_iterator vtlRit = vertexList.rbegin() ; vtlRit != vertexList.rend() ; ++vtlRit ) {
		int index = *vtlRit;
		SLSF::Block block = vertexIndexBlockMap[ index ];
		if ( block == Udm::null ) { // unit delay as source is not registered - we will invoke it initially, and invoke it as destination in the priority order
			// const std::string& bt = blk.BlockType();
			// assert(bt.compare("UnitDelay") == 0);
			/* Unit Delay Block as destination */
			continue;
		}
		block.Priority() = priority++;
	}
}

std::string SFUtils::convertToCPlusPlusName( const std::string& name ) {
/*	unsigned char unaut= 0xfc;
	unsigned char unaut= 'ü';
	std::cout << unaut;
	if ( isalpha( unaut))
		std::cout << " is alpha."; 
	else 
		std::cout << " is not alpha.";
	std::cout << std::endl;
*/	// empty name
	if ( name.empty())
		return "";
	// name must start with letter
	std::string converted;
	if ( false== isalpha( (unsigned char)name[ 0]))
		converted= "pre";
	// name can contain alphanum characthers and '_'
	for( std::string::const_iterator i= name.begin(); i!= name.end(); ++i) {
		if ( isalnum( (unsigned char)*i) || (*i== '_'))
			converted+= *i;
		else
			converted+= '_';
	}
	return converted;
}

Udm::Object SFUtils::getTopArchetype( Udm::Object object ) {
	if ( object == Udm::null ) return object;

	Udm::Object archetype = object.archetype();
	while( archetype != Udm::null ) {
		object = archetype;
		archetype = object.archetype();
	}
	return object;
}

Udm::Object SFUtils::getTopLevelInstance( Udm::Object object ) {
	if ( object == Udm::null ) return Udm::null;

	Udm::Object parent = object.GetParent();

	Udm::Object archetype = parent.archetype();
	while( archetype != Udm::null ) {
		if (  ( object = object.archetype() ) == Udm::null  ) return Udm::null;
		parent = archetype;
		archetype = parent.archetype();
	}
	return object;
}

bool SFUtils::InstanceEquivalent( const Udm::Object &object1, const Udm::Object &object2 ) {

	Udm::Object archetype1 = getTopArchetype( object1 );
	Udm::Object archetype2 = getTopArchetype( object2 );

	if ( archetype1 == Udm::null || archetype2 == Udm::null ) return false;

	return archetype1.uniqueId() == archetype2.uniqueId();
}

SFUtils::ObjectSet SFUtils::getAllInstanceEquivalent( Udm::Object object ) {

	Udm::Object archetype = getTopArchetype( object );
	if ( archetype == Udm::null ) return ObjectSet();

	ObjectList objectList;
	objectList.push_back( archetype );
	for( ObjectList::iterator oblItr = objectList.begin() ; oblItr != objectList.end() ; (void)++oblItr ) {
		ObjectSet objectSet = oblItr->instances();
		objectList.insert( objectList.end(), objectSet.begin(), objectSet.end() );
	}
	return ObjectSet( objectList.begin(), objectList.end() );
}

std::string SFUtils::ZeroInit( SFC::DT dt, std::string argDeclBaseName, std::string indent ) {

	std::string zeroInitStatement;

	if ( dt.type() == SFC::BasicType::meta ) {

		zeroInitStatement = indent + argDeclBaseName + " = 0\n";

	} else if ( dt.type() == SFC::Struct::meta ) {

		SFC::Struct sfcStruct = SFC::Struct::Cast( dt );
		DeclarationSet declarationSet = sfcStruct.Declaration_kind_children_sorted( StatementSorter() );
		for(
		 DeclarationSet::iterator dcsItr = declarationSet.begin() ;
		 dcsItr != declarationSet.end() ;
		 (void)++dcsItr
		) {
			SFC::Declaration declaration = *dcsItr;
			std::string memberName = declaration.name();
			zeroInitStatement += ZeroInit( declaration.dt(), argDeclBaseName + "." + memberName, indent );
		}

	} else { // dt.type == SFC::Array::meta

		int ix = 0;
		while( dt.type() == SFC::Array::meta ) {
			SFC::Array sfcArray = SFC::Array::Cast( dt );
			int noelem = sfcArray.noelem();

			std::string indexVarName = std::string( "ix" ) + "_" + boost::lexical_cast< std::string >( ix ); 
			zeroInitStatement +=
			 indent + indexVarName + " = 0\n" +
			 indent + "while " + indexVarName + " < " + boost::lexical_cast< std::string >( noelem ) + "\n";

			indent += std::string( "  " );

			argDeclBaseName += ix == 0 ? std::string( "( " ) : std::string( ", " );
			argDeclBaseName += indexVarName;

			dt = sfcArray.dt();
			(void)++ix;
		}

		argDeclBaseName += std::string( " )" );

		zeroInitStatement += ZeroInit( dt, argDeclBaseName, indent );

		while( ix > 0 ) {
			(void)--ix;
			std::string indexVarName = std::string( "ix" ) + "_" + boost::lexical_cast< std::string >( ix );
			zeroInitStatement += indent + indexVarName + " = " + indexVarName + " + 1\n";
			indent = indent.substr( 2 );
			zeroInitStatement += indent + "end\n";
		}
	}

	return zeroInitStatement;
}

void SFUtils::ZeroInit( SFC::Function function, SFC::ArgDeclBase argDeclBase ) {
	std::string argDeclBaseName = argDeclBase.name();

	getSFCSymbolTable().push( argDeclBaseName, argDeclBase );

	std::string zeroInitStatement = ZeroInit( argDeclBase.dt(), argDeclBaseName, "" );
	mstat2SFC( function, zeroInitStatement, true );

	getSFCSymbolTable().clear();
}

void SFUtils::InitParameter( SFC::Function function, SLSF::Parameter parameter, SFC::LocalVar localVar ) {

	std::string initStatement = "";

	std::string matrix = parameter.Value();

	SFC::Arg contextArg = SFCManager::getFirstFunctionArg( function );
	SFC::DT dt = contextArg.dt();

//	SFC::LocalVar localVar = parameter.memb();
	std::string memberName = localVar.name();

	if ( dt.type() == SFC::Struct::meta && localVar.GetParent() == dt ) {

		std::string contextArgName = contextArg.name();

		getSFCSymbolTable().push( contextArgName, contextArg );
		initStatement = contextArgName + "." + memberName + " = " + matrix + "\n";

	} else {

		getSFCSymbolTable().push( memberName, localVar );
		initStatement = memberName + " = " + matrix + "\n";

	}

	mstat2SFC( function, initStatement );

	getSFCSymbolTable().clear();
}

void SFUtils::InitParameter( SFC::Function function, SFC::LocalVar localVar ) {

	std::string initStatement = "";

	SFC::Arg contextArg = SFCManager::getFirstFunctionArg( function );
	SFC::DT dt = contextArg.dt();

//	SFC::LocalVar localVar = parameter.memb();
	std::string memberName = localVar.name();
	std::string initialValue = localVar.initial();
	std::string::size_type firstCharPos = initialValue.find_first_not_of( " \t\r\n" );
	if ( firstCharPos == std::string::npos ) {
		initialValue = "0";
	} else {
		std::string::size_type lastCharPos = initialValue.find_last_not_of( " \t\r\n" );
		initialValue = initialValue.substr( firstCharPos, lastCharPos - firstCharPos + 1 );
	}

	if ( dt.type() == SFC::Struct::meta && localVar.GetParent() == dt ) {

		std::string contextArgName = contextArg.name();

		getSFCSymbolTable().push( contextArgName, contextArg );
		initStatement = contextArgName + "." + memberName + " = " + initialValue + "\n";

	} else {

		getSFCSymbolTable().push( memberName, localVar );
		initStatement = memberName + " = " + initialValue + "0\n";

	}

	mstat2SFC( function, initStatement );

	getSFCSymbolTable().clear();
}

void SFUtils::Assignment( SFC::CompoundStatement block, SFC::ArgDeclBase output, SFC::ArgDeclBase input ) {

	std::string outputName = output.name();
	getSFCSymbolTable().push( outputName, output );

	std::string inputName = input.name();
	getSFCSymbolTable().push( inputName, input );

	std::string assignmentStatement = outputName + " = " + inputName + "\n";

	mstat2SFC( block, assignmentStatement, true );

	getSFCSymbolTable().clear();
}
