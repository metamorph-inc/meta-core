/*
 *	Copyright (c) Vanderbilt University, 2006
 *	ALL RIGHTS RESERVED
 *
 *	Vanderbilt University disclaims all warranties with regard to this
 *	software, including all implied warranties of merchantability
 *	and fitness.  In no event shall Vanderbilt University be liable for
 *	any special, indirect or consequential damages or any damages
 *	whatsoever resulting from loss of use, data or profits, whether
 *	in an action of contract, negligence or other tortious action,
 *	arising out of or in connection with the use or performance of
 *	this software.
 *
 *  FILE NAME:	MatLabUdmBlock.hpp 
 *	SYNOPSIS:	Definition of class "MatLabUdmBlock", translates SL/SF 'Simulink.Block'
 *				objects that are NOT PORTS (i.e. not 'Simulink.Inport', 'Simulink.Outport', etc).
 *				into equivalent GME ESMoL Systems/Primitives.					
 *  AUTHOR:		Harmon Nine - hnine@isis.vanderbilt.edu
 *  CREATED:	07/20/06
 *  LAST UPDATE: 
 */
#ifndef _ISIS_MATLABUDMBLOCK
#define _ISIS_MATLABUDMBLOCK

#include "MatLabUdm.hpp"
#include "MatLabUdmPort.hpp"
#include "UdmEngine.hpp"

class MatLabUdmBlock : public MatLabUdm {

public:
	enum BlockType {
		SYSTEM,
		REFERENCE,
		PRIMITIVE,
	};

	typedef std::list< IntList::iterator > IntListItrList;
	typedef std::set< std::string > SymbolSet;

	typedef std::vector< SLSF::TypeBase > TypeBaseVector;
	typedef std::map< std::string, SLSF::StateDE > NameDEMap;

	typedef bool (*SpecialBlockFunction)( MatLabUdmBlock &matLabUdmBlock );
	typedef std::map< std::string, SpecialBlockFunction > SpecialBlockFunctionMap;

private:
	static bool makeMuxType( MatLabUdmBlock &matLabUdmBlock );
	static bool makeBusSelectorType( MatLabUdmBlock &matLabUdmBlock );
	static TypeBaseVector getMatrixVector( SLSF::TypeBase typeBase );
	static bool makeDemuxType( MatLabUdmBlock &matLabUdmBlock );
	static bool makeMergeType( MatLabUdmBlock &matLabUdmBlock );
	static NameDEMap getNameDEMap( SLSF::State state );
	static bool makeSFunctionType( MatLabUdmBlock &matLabUdmBlock );
	static bool makeSubSystemType( MatLabUdmBlock &matLabUdmBlock );
	static bool makeDefaultType( MatLabUdmBlock &matLabUdmBlock ) { return true; }

	static int getSubSystemIndex( void ) {
		static int subSystemIndex = 0;
		return subSystemIndex++;
	}

	BlockType _blockType;
	std::string _embeddedBlockName;
	bool _hasMask;

	class SpecialBlocks {
	private:
		SpecialBlockFunctionMap _specialBlockFunctionMap;

	public:
		SpecialBlocks( void ) {
			_specialBlockFunctionMap.insert(  std::make_pair( "Mux" , &makeMuxType )  );
			_specialBlockFunctionMap.insert(  std::make_pair( "BusCreator", &makeMuxType )  );
			_specialBlockFunctionMap.insert(  std::make_pair( "BusSelector", &makeBusSelectorType )  );
			_specialBlockFunctionMap.insert(  std::make_pair( "Demux", &makeDemuxType )  );
			_specialBlockFunctionMap.insert(  std::make_pair( "Merge", &makeMergeType )  );
			_specialBlockFunctionMap.insert(  std::make_pair( "S-Function", &makeSFunctionType )  );
			_specialBlockFunctionMap.insert(  std::make_pair( "SubSystem", &makeSubSystemType )  );

			_specialBlockFunctionMap.insert(  std::make_pair( "UnitDelay", &makeMergeType )  );
		}

		SpecialBlockFunctionMap &getSpecialBlockFunctionMap( void ) { return _specialBlockFunctionMap; }
	};

	static SpecialBlockFunctionMap &getSpecialBlockFunctionMap( void ) {
		static SpecialBlocks specialBlocks;
		return specialBlocks.getSpecialBlockFunctionMap();
	}

	bool isSpecialBlock( void );

	static SpecialBlockFunction getSpecialBlockFunction( const std::string &blockType ) {
		SpecialBlockFunctionMap::iterator sbmItr = getSpecialBlockFunctionMap().find( blockType );
		return sbmItr == getSpecialBlockFunctionMap().end() ? &makeDefaultType : sbmItr->second;
	}
	static SpecialBlockFunction getSpecialBlockFunction( MatLabUdmBlock matLabUdmBlock ) {
		std::string blockType = matLabUdmBlock.getUdmBlock().BlockType();
		return getSpecialBlockFunction( blockType );
	}
	static bool callSpecialBlockFunction( MatLabUdmBlock matLabUdmBlock ) {
		return getSpecialBlockFunction( matLabUdmBlock )( matLabUdmBlock );
	}

	Udm::Object distinguish( Udm::Object &udmParent, const std::string &matLabName, int matLabObjectNo );

	typedef std::vector< MatLabUdmBlock > MatLabUdmBlockVector;

	static MatLabUdmBlockVector &getMatLabUdmBlockVector( void ) {
		static MatLabUdmBlockVector matLabUdmBlockVector;
		return matLabUdmBlockVector;
	}

public:
	static void getAllPortTypes( void  ) {
		for(
		 MatLabUdmBlockVector::iterator mbvItr = getMatLabUdmBlockVector().begin() ;
		 mbvItr != getMatLabUdmBlockVector().end() ;
		 ++mbvItr
		) {
			mbvItr->getPortTypes();
		}
	}

	static void getAllSampleTimes( void ) {
		for(
			MatLabUdmBlockVector::iterator mbvIter = getMatLabUdmBlockVector().begin();
			mbvIter != getMatLabUdmBlockVector().end() ;
			++mbvIter
		) {
			mbvIter->getSampleTime();
		}
	}

	static std::string checkAllValues( void ) {
		std::string valueErrorMsg;
		for(
			MatLabUdmBlockVector::iterator mbvIter = getMatLabUdmBlockVector().begin();
			mbvIter != getMatLabUdmBlockVector().end() ;
			++mbvIter
		) {
			
			string strResult = mbvIter->checkValue();
			if ( !strResult.empty() )
			{
				ostringstream tmpStr;
				tmpStr << strResult << std::endl;
				valueErrorMsg += tmpStr.str();
			}
		}

		return valueErrorMsg;
	}


private:
	void getPortTypes( const std::string &portType );
	void getSampleTime( void );
	void getPortTypes( void );
	std::string checkValue( void );
	void setupBlock( void );
	void setupBlockReference( void );
	void setupPorts( void );

public:
	MatLabUdmBlock( Udm::Object udmParent, const std::string &matLabName, int matLabObjectNo ) :
	 MatLabUdm( matLabName, matLabObjectNo ), _embeddedBlockName( "" ) {
		setUdmObject(  distinguish( udmParent, matLabName, matLabObjectNo )  );
		setupBlock();
	}

	MatLabUdmBlock( Udm::Object udmParent, SLSF::Subsystem archetype, const std::string &matLabName, int matLabObjectNo ) :
	 MatLabUdm(  archetype.CreateInstance( udmParent ), matLabName, matLabObjectNo  ), _embeddedBlockName( "" ) {
		setupBlock();
	}

	BlockType getBlockType( void ) { return _blockType; }

	Block getUdmBlock( void ) const { return Block::Cast( getUdmObject() ); }

	Subsystem getUdmSystem( void ) { return Subsystem::Cast( getUdmObject() ); }
	Reference getUdmReference( void ) { return Reference::Cast( getUdmObject() ); }
	Primitive getUdmPrimitive( void ) { return Primitive::Cast( getUdmObject() ); }

private:
	class MatLabTypeInit {
	private:
		std::string _matLabType;

	public:
		MatLabTypeInit( void ) {
			_matLabType = std::string("'-isa', 'Simulink.Block'");

			const MatLabUdmPort::PortTypes &portTypes = MatLabUdmPort::getPortTypes();			
			for( MatLabUdmPort::PortTypes::const_iterator pttItr = portTypes.begin() ; pttItr != portTypes.end() ; ++pttItr ) {
				_matLabType += std::string(", '-and', '-not', '-isa', '") + *pttItr + '\'';
			}
		}
		const std::string &getMatLabType( void ) const { return _matLabType; }
	};

protected:
	bool evalTest( const std::string &fieldName );

	virtual void delayedSetupAux( void );

public:
	static const std::string &getMatLabChar( void ) {
		static const std::string matLabChar( "b" );
		return matLabChar;
	}
	static const std::string &getMatLabType( void ) {
		static const MatLabTypeInit matLabTypeInit;
		return matLabTypeInit.getMatLabType();
	}

	virtual bool collect( void ) const {
		return
		 (
		  getUdmObject().type() != SLSF::Primitive::meta ||
		  static_cast< std::string >( getUdmBlock().BlockType() ) == "S-Function"
		 );
	}

	virtual void cleanup( void ) {
		if ( _hasMask ) {
			getScopeSymbolTable().deleteScope();
			getScopeSymbolTable().clearSpecialSymbols();
		}
	}

	class Factory : public MatLabUdm::Factory {
	public:
		virtual MatLabUdm::SmartPointer create( Udm::Object &udmParent, const std::string &matLabName, int matLabObjectNo ) const;

		virtual const std::string &getMatLabChar( void ) const { return MatLabUdmBlock::getMatLabChar(); }
		virtual const std::string &getMatLabType( void ) const { return MatLabUdmBlock::getMatLabType(); }

		virtual bool exclude( const std::string &matLabObject ) const;
	};

	static void constructBlocks( MatLabUdm::SmartPointer &matLabUdmParentSP );

private:

	class EPS {
	private:
		double _eps;

	public:
		EPS( void ) {
			_eps = MatLabEngine::globalEngine().getMxDoubleValue( "eps" );
		}

		double getEPS( void ) { return _eps; }
	};

	static double getEPS( void ) {
		static EPS eps;
		return eps.getEPS();
	}

	std::string zeroSubstitution( std::string value );

#if PARADIGM != CyberComposition_PARADIGM
	typedef std::set< SLSF::ConnectorRef > ConnectorRefSet;
#endif

	typedef std::vector< SLSF::Port > PortVector;
	typedef std::set< SLSF::Block > BlockSet;
	typedef std::set< SLSF::State > StateSet;
	typedef std::set< SLSF::Subsystem > SubsystemSet;
	typedef std::set< SLSF::Primitive > PrimitiveSet;
	typedef std::set< SLSF::InPort > InPortSet;
	typedef std::set< SLSF::OutPort > OutPortSet;
	typedef std::set< SLSF::Line > LineSet;

	struct MatLabUdmBlockComparator {
		bool operator()( const MatLabUdmBlock &matLabUdmBlock1, const MatLabUdmBlock &matLabUdmBlock2 ) {
			return matLabUdmBlock1.getUdmBlock() < matLabUdmBlock2.getUdmBlock();
		}
	};
	typedef std::set< MatLabUdmBlock, MatLabUdmBlockComparator > MatLabUdmBlockSet;

	template< typename DE >
	struct DataEventSorter {
		bool operator()( const DE &de1, const DE &de2 ) {
			std::string scope1 = de1.Scope();
			std::string scope2 = de2.Scope();
			if ( scope1 != scope2 ) return scope1 < scope2;
			return static_cast< int >( de1.Port() ) < static_cast< int >( de2.Port() );
		}
	};

	typedef DataEventSorter< SLSF::Data > DataSorter;
	typedef DataEventSorter< SLSF::Event > EventSorter;

	typedef std::set< SLSF::Data, DataSorter > DataSet;
	typedef std::set< SLSF::Event, EventSorter > EventSet;

	struct TypeBaseRefSorter {
		bool operator()( const SLSF::TypeBaseRef &tbr1, const SLSF::TypeBaseRef &tbr2 ) {
			return static_cast< int >( tbr1.MemberIndex() ) < static_cast< int >( tbr2.MemberIndex() );
		}
	};
	typedef std::set< SLSF::TypeBaseRef, TypeBaseRefSorter > TypeBaseRefSet;

	typedef std::vector< SLSF::Block > BlockVector;

	typedef std::map< std::string, SLSF::Subsystem > ReferenceMap;

	static ReferenceMap &getReferenceMap( void ) {
		static ReferenceMap referenceMap;
		return referenceMap;
	}

public:
	typedef std::vector< MatLabUdmBlock > MFileSFuncs;

	static MFileSFuncs &getMFileSFuncs( void ) {
		static MFileSFuncs mFileSFuncs;
		return mFileSFuncs;
	}

	static void kludgeMFileSFuncs( void );

private:
	typedef std::list< SLSF::Block > BlockList;
	typedef std::map< std::string, BlockList > GotoFromBlockMap;

	typedef std::vector< SLSF::Parameter > ParameterVector;

	void addToBlockMap( GotoFromBlockMap &gotoFromBlockMap );

	static GotoFromBlockMap &getGotoBlockMap( void ) {
		static GotoFromBlockMap gotoBlockMap;
		return gotoBlockMap;
	}

	static GotoFromBlockMap &getFromBlockMap( void ) {
		static GotoFromBlockMap fromBlockMap;
		return fromBlockMap;
	}

	static GotoFromBlockMap &getGotoTagVisibilityBlockMap( void ) {
		static GotoFromBlockMap gotoTagVisibilityBlockMap;
		return gotoTagVisibilityBlockMap;
	}

	void addToGotoBlockMap( void ) { addToBlockMap( getGotoBlockMap() ); }
	void addToFromBlockMap( void ) { addToBlockMap( getFromBlockMap() ); }
	void addToGotoTagVisibilityBlockMap( void ) { addToBlockMap( getGotoTagVisibilityBlockMap() ); }

public:
	static void processGotoFromBlocks( void );

private:
	class PortConnector {
	private:
		SLSF::Port _fromPort;
		SLSF::Block _lineParent;
	public:
		PortConnector( SLSF::Port fromPort, SLSF::Block lineParent ) :
		 _fromPort( fromPort ), _lineParent( lineParent ) { }
	
		void operator()( SLSF::Port toPort ) {
			SLSF::Line line = SLSF::Line::Create( _lineParent );
			line.RefId() = getRefId();
			line.srcLine_end() = _fromPort;
			line.dstLine_end() = toPort;
		}
	};

	static void getGotoAndLCABlocks(
	 const std::string &gotoTag, SLSF::Block fromBlock,
	 SLSF::Block &gotoBlock, SLSF::Block &lcaBlock
	);
	static SLSF::Block getLCABlock( SLSF::Block fromBlock, SLSF::Block gotoBlock );
	static bool isGotoVisible( SLSF::Block localLCABlock, const std::string &gotoTag );

	static SLSF::Port getGotoSourcePort( SLSF::Block gotoBlock, SLSF::Block lcaBlock );
	static SLSF::Port makeNewOutputPort( SLSF::Port port, SLSF::Block block );
	static SLSF::Port getNonFromBlockPort( PortSet &portSet );

	static PortSet getFromSourcePort( SLSF::Block gotoBlock, SLSF::Block lcaBlock );
	static SLSF::Port makeNewInputPort( SLSF::Port port, SLSF::Block block );

	static PortSet getConnectedOutPorts( PortSet &portSet );
	static PortSet getConnectedOutPorts( SLSF::Port port ) {
		PortSet portSet;
		portSet.insert( port );
		return getConnectedOutPorts( portSet );
	}

	static bool noLines( SLSF::Block block );
	static void deleteBlocks( GotoFromBlockMap &gotoFromBlockMap );

	static BlockList getAncestors( SLSF::Block block ) {
		BlockList blockList;

		Udm::Object udmObject = block.GetParent();
		while(  udmObject != Udm::null && Udm::IsDerivedFrom( udmObject.type(), SLSF::Block::meta )  ) {
			blockList.push_front(  SLSF::Block::Cast( udmObject )  );
			udmObject = udmObject.GetParent();
		}

		return blockList;
	}

	static std::string getBlockPath( SLSF::Block block ) {
		BlockList blockList = getAncestors( block );

		std::string retval;
		bool first = true;
		for( BlockList::iterator bklItr = blockList.begin() ; bklItr != blockList.end() ; ++bklItr ) {
			if ( first ) first = false;
			else         retval += "/";
			retval += bklItr->name();
		}

		retval += block.name();

		return retval;
	}

	static int getBlockLevel( SLSF::Block block ) {
		int level = 0;

		Udm::Object udmObject = block;
		while(  udmObject != Udm::null && Udm::IsDerivedFrom( udmObject.type(), SLSF::Block::meta )  ) {
			udmObject = udmObject.GetParent();
			(void)++level;
		}

		return level;
	}

public:
	static Udm::Object getTopLevelArchetype( Udm::Object object ) {
		if ( object != Udm::null ) {
			while ( object.isInstance() ) {
				object = object.archetype();
			}
		}
		return object;
	}

	static SubsystemSet getTopLevelSubsystems( void ) {
#if PARADIGM == CyberComposition_PARADIGM
		SLSF_ROOT::SimulinkWrapper rootSimulinkModel = UdmEngine::get_singleton().getRootComponent();
		SubsystemSet subsystemSet = rootSimulinkModel.Simulink_Subsystem_kind_children();
#else
		SLSF::Dataflow dataflow = UdmEngine::get_singleton().getDataflow();
		SubsystemSet subsystemSet = dataflow.Subsystem_kind_children();
#endif
		return subsystemSet;
	}

	static void propagateTypes( void );
	static void propagateArchetypeOutPorts( void );

	static void processCollectors( void );
	static void propagateTypesToArchetypes( void );
	static void checkTypes( void );

	typedef std::map< std::string, SLSF::Parameter > ParameterMap;
	static ParameterMap getParameterMap( SLSF::Block &block );

private:
	static MatLabUdmBlockSet &getCollectorSet( void ) {
		static MatLabUdmBlockSet collectorSet;
		return collectorSet;
	}
	static BlockSet &getCollectorBlockSet( void ) {
		static BlockSet collectorBlockSet;
		return collectorBlockSet;
	}

	static bool hasObjectInstances( SLSF::Subsystem Subsystem );
	static void topLevelPropagateTypes( SLSF::Subsystem subsystem );
	static void propagateTypes( SLSF::Subsystem subsystem );
	static void propagateTypes( SLSF::Primitive primitive );
	static void propagateArchetypeOutPorts( SLSF::OutPort outPort );

	static void propagateType( SLSF::TypeBase typeBase, SLSF::Port port );
	static void propagateType( SLSF::Port port );
	static ObjectSet getDeepInstances( Udm::Object object );
	static bool getInputTypes( SLSF::Block block, TypeBaseVector &typeBaseVector );

	static void propagateTypesToArchetypes( SLSF::Block blockInstance );
	static void checkTypes( SLSF::Block block );

	static SLSF::TypeStruct typeBundle( const TypeBaseVector &typeBaseVector );

	typedef std::map< std::string, SLSF::Block > BlockMap;
	static BlockMap getChildBlockMap( SLSF::Subsystem subsystem ) {
		BlockMap blockMap;
		BlockSet blockSet = subsystem.Block_children();
		for( BlockSet::iterator bksItr = blockSet.begin() ; bksItr != blockSet.end() ; ++bksItr ) {
			blockMap.insert(  std::make_pair( bksItr->Name(), *bksItr )  );
		}
		return blockMap;
	}

};

#endif
