#define BOOST_PYTHON_STATIC_LIB 1
#define BOOST_PYTHON_NO_LIB
#include <Python.h>
#include <boost/lexical_cast.hpp>
#include <boost/python.hpp>

#include "BlockTemplateModule.hpp"
#include "BlockTemplateModuleBase.hpp"
#include "CodeGenerator.hpp"


std::string convertToMatLabIdentifierName( const std::string &name ) {

	std::string matLabIdentifierName;

	for( std::string::const_iterator strCit = name.begin() ; strCit != name.end() ; ++strCit ) {
		char character = *strCit;

		if (  !isalpha( character ) && character != '_'  ) matLabIdentifierName += "_" + boost::lexical_cast< std::string >(  static_cast< int >( character )  ) + "_";
		else                                               matLabIdentifierName += character;
	}

	return matLabIdentifierName;
}

boost::python::list flatten( boost::python::object object ) {

	boost::python::list retval;

	int noElements = boost::python::len( object );
	for( int ix = 0 ; ix < noElements ; ++ix ) {
		boost::python::extract< boost::python::list > subList( object[ ix ] );
		if ( subList.check() ) {
			retval.extend(  flatten( subList() )  );
			continue;
		}

		boost::python::extract< boost::python::tuple > subTuple( object[ ix ] );
		if ( subTuple.check() ) {
			retval.extend(  flatten( subTuple() )  );
			continue;
		}

		retval.append( object[ ix ] );
	}

	return retval;
}

std::string DTBase::getTypeString( void ) const {
	if ( isNull() )                                return "NULL";
	else if ( _dt.type() == SFC::BasicType::meta ) return "BasicType";
	else if ( _dt.type() == SFC::Array::meta )     return "ArrayBase";
	else if ( _dt.type() == SFC::Struct::meta )    return "StructBase";
	return "UNKNOWN";
}

int DTBase::getNoElements( void ) const {
	int noElements = 1;

	SFCTypesManager::DimensionVector dimensionVector = SFCTypesManager::getDimensions( _dt );
	for(
	 SFCTypesManager::DimensionVector::iterator dmvItr = dimensionVector.begin() ;
	 dmvItr != dimensionVector.end() ;
	 ++dmvItr
	) noElements *= *dmvItr;

	return noElements;
}

boost::python::list DTBase::getMembers( void ) const {
	boost::python::list bpList;
	if ( isStruct() ) {
		SFC::DeclarationSet declarationSet = SFC::Struct::Cast( _dt ).Declaration_kind_children_sorted( SFC::StatementSorter() );
		for(
		 SFC::DeclarationSet::iterator dcnItr = declarationSet.begin() ;
		 dcnItr != declarationSet.end() ;
		 ++dcnItr
		) {
			bpList.append(  ArgDeclBase( *dcnItr )  );
		}
	}
	return bpList;
}

boost::python::list DTBase::getDimensions( void ) const {
	boost::python::list bpList;
	SFCTypesManager::DimensionVector dimensionVector = SFCTypesManager::getDimensions( _dt );
	for(
	 SFCTypesManager::DimensionVector::iterator dmvItr = dimensionVector.begin() ;
	 dmvItr != dimensionVector.end() ;
	 ++dmvItr
	) {
		bpList.append( *dmvItr );
	}
	return bpList;
}

boost::python::list DTBase::getAllDimensions( void ) const {
	boost::python::list bpList;
	SFCTypesManager::DimensionVector dimensionVector = SFCTypesManager::getAllDimensions( _dt );
	for(
	 SFCTypesManager::DimensionVector::iterator dmvItr = dimensionVector.begin() ;
	 dmvItr != dimensionVector.end() ;
	 ++dmvItr
	) {
		bpList.append( *dmvItr );
	}
	return bpList;
}

boost::python::list ArrayBase::normalizeList( boost::python::list list ) {
	boost::python::list retval;
	DT dt;

	int noElements = boost::python::len( list );
	for( int ix = 0 ; ix < noElements ; ++ix ) {
		boost::python::extract< int > subInt( list[ ix ] );
		if ( subInt.check() ) {
			retval.append( list[ ix ] );
			continue;
		}

		boost::python::extract< DT > subDT( list[ ix ] );
		if ( subDT.check() ) {
			if ( dt.isNull() ) dt = subDT();
			continue;
		}

		boost::python::extract< boost::python::str > subStr( list[ ix ] );
		if ( subStr.check() ) {
			std::string typeName = boost::python::extract< std::string >(  subStr().encode( "ascii" )  );
			dt = BasicType( typeName );
		}
	}

	if ( dt.isNull() ) {
		std::cout << "WARNING:  NO ELEMENT TYPE SPECIFIED FOR ARRAY.  DEFAULTING TO INT." << std::endl;
		dt == BasicType( "int" );
	}

	retval.append( dt );

	return retval;
}

ArrayBase::ArrayBase( boost::python::tuple args, boost::python::dict keywordArgs ) {
	boost::python::list normalizedList = normalizeList(  flatten( args )  );

	int noArgs = boost::python::len( normalizedList );
	int noDimensions = noArgs - 1;

	SFC::DT sfcDT;
	boost::python::extract< DT > subDT( normalizedList[ noArgs - 1 ] );
	sfcDT = subDT().getSFCType();

	SFCTypesManager::DimensionVector dimensionVector;
	for( int ix = 0 ; ix < noDimensions ; ++ix ) {
		int dimension = boost::python::extract< int >( args[ ix ] );
		dimensionVector.push_back( dimension );
	}

	if ( dimensionVector.empty() ) {
		setSFCType( sfcDT );
		return;
	}

	SFC::DT elementType = SFCTypesManager::getBaseDT( sfcDT );

	bool rowVectorAllowed = true;
	if ( elementType.type() == SFC::Struct::meta ) {
		rowVectorAllowed = false;
	} else if ( sfcDT.type() == SFC::Array::meta ) {
		SFC::Array sfcArray = SFC::Array::Cast( sfcDT );
		SFCTypesManager::DimensionVector subDimensionVector = SFCTypesManager::getAllDimensions( sfcArray );
		if ( subDimensionVector.size() == 2 && subDimensionVector[ 0 ] == 1 ) {
			if ( dimensionVector.size() == 1 && dimensionVector[ 0 ] == 1 ) {
				setSFCType( sfcDT );
				return;
			}
			subDimensionVector.erase( subDimensionVector.begin() );
			sfcArray = SFC::Array::Cast( sfcArray.dt() );
		}
		rowVectorAllowed = false;
	}

	if ( noDimensions == 2 && rowVectorAllowed ) {
		if ( dimensionVector.back() <= 1 ) dimensionVector.pop_back();
		if ( dimensionVector.front() < 1 ) dimensionVector.erase( dimensionVector.begin() );
		if ( dimensionVector.size() == 1 && dimensionVector.front() <= 1 ) dimensionVector.clear();
	} else {
		SFCTypesManager::DimensionVector::iterator dmvItr = dimensionVector.begin();
		while( dmvItr != dimensionVector.end() ) {
			if ( *dmvItr <= 1 ) dmvItr = dimensionVector.erase( dmvItr );
			else                ++dmvItr;
		}
	}

	for(
	 SFCTypesManager::DimensionVector::reverse_iterator dmvRit = dimensionVector.rbegin() ;
	 dmvRit != dimensionVector.rend() ;
	 ++dmvRit
	) {
		SFC::Array sfcArray = SFCTypesManager::getSingleton().makeArray( *dmvRit );
		sfcArray.dt() = sfcDT;
		sfcDT = sfcArray;
	}
	
	SFCTypesManager::getSingleton().makeTypeUnique( sfcDT );

	setSFCType( sfcDT );
}

StructBase::StructBase( boost::python::tuple positionalArgs, boost::python::dict keywordArgs ) {

	boost::python::object args = positionalArgs;

	// IT IS POSSIBLE THAT THE ORIGINAL StructBase CONSTRUCTOR ARGUMENTS WERE SPECIFIED IN A LIST PROVIDED AS THE
	// ONLY ARGUMENT, E.G. StructBase( [ [n1,t1],[n2,t2],... ] ), RATHER THAN AS A COMMA-SEPARATED LIST OF ARGUMENTS
	// E.G. StructBase( [n1,t1],[n2,t2],... )
	if (  boost::python::len( args ) == 1  ) {
		boost::python::object element = args[ 0 ];

		boost::python::extract< boost::python::list > subList( element );
		boost::python::extract< boost::python::tuple > subTuple( element );

		// IF [ [n1,t1] ] THEN LEAVE ALONE
		if (  ( subList.check() || subTuple.check() ) && boost::python::len( element ) == 2  ) {
			boost::python::extract< std::string > subString( element[ 0 ] );
			boost::python::extract< DT > subDT( element[ 1 ] );
			boost::python::extract< boost::python::str > basicTypeStr( element[ 1 ] );
			if (   !(  subString.check() && ( subDT.check() || basicTypeStr.check() )  )   ) args = element;
		} else {
			args = element;
		}
	}

	SFC::Struct sfcStruct = SFCTypesManager::getSingleton().getStruct();
	int noArgs = boost::python::len( args );
	int signalNo = 1;
	for( int ix = 0 ; ix < noArgs ; ++ix ) {

		boost::python::object object = args[ ix ];
		std::string name = std::string( "signal" ) + boost::lexical_cast< std::string >( signalNo );
		DT dt;

		boost::python::extract< boost::python::list > subList( object );
		boost::python::extract< boost::python::tuple > subTuple( object );
		boost::python::extract< DT > subDT( object );
		boost::python::extract< boost::python::str > basicTypeStr( object );

		if ( subList.check() || subTuple.check() ) {
			int noElements = boost::python::len( object );
			if ( noElements == 0 || noElements > 2 ) continue;
			if ( noElements == 1 ) {
				boost::python::extract< DT > subDT( object[ 0 ] );
				if ( subDT.check() ) dt = subDT();
				else                 continue;
			}
			if ( noElements == 2 ) {
				boost::python::extract< std::string > subString( object[ 0 ] );
				if ( subString.check() ) name = subString();
				else                 continue;
				boost::python::extract< DT > subDT( object[ 1 ] );
				if ( subDT.check() ) dt = subDT();
				else                 continue;
			}
		} else if ( subDT.check() ) {
			dt = subDT();
		} else if ( basicTypeStr.check() ) {
			std::string basicTypeName = boost::python::extract< std::string >(  basicTypeStr().encode( "ascii" )  );
			dt = BasicType( basicTypeName );
		} else {
			continue;
		}

		++signalNo;
		SFCTypesManager::createMember( sfcStruct, name, dt.getSFCType() );
	}

	SFCTypesManager::getSingleton().makeTypeUnique( sfcStruct );
	setSFCType( sfcStruct );
}


/*******************/
/* ArgDeclBaseBase */
/*******************/
void ArgDeclBaseBase::registerInSymbolTable( void ) const {
	if ( _sfcArgDeclBase == Udm::null ) {
		std::cerr << "Warning:  Attempt to register NULL variable in symbol table" << std::endl;
	} else if ( _sfcArgDeclBase.GetParent().type() != SFC::Struct::meta ) {
		getSFCSymbolTable().push( _symbolTableName, _sfcArgDeclBase );
	}
}

ArgDeclBaseBase::ArgDeclBaseBase( SFC::ArgDeclBase sfcArgDeclBase, bool partOfContext ) :
 _sfcArgDeclBase( sfcArgDeclBase ),
 _partOfContext( partOfContext ) {
	if ( _sfcArgDeclBase == Udm::null ) {
		std::cerr << "WARNING:  ArgDeclBaseBase created from NULL SFC::ArgDeclBase" << std::endl;
	} else {
		_symbolTableName = _sfcArgDeclBase.name();
	}
	registerInSymbolTable();
}

void ArgDeclBaseBase::setInitialValue( const std::string &initialValue ) {
	if ( _sfcArgDeclBase.type() == SFC::LocalVar::meta ) {
		SFC::LocalVar localVar = SFC::LocalVar::Cast( _sfcArgDeclBase );
		SFC::DT dt = localVar.dt();
		if ( dt.type() != SFC::Struct::meta ) {
			localVar.initial() = initialValue;
		} else {
			std::cerr << "WARNING: ATTEMPT TO INITIALIZE A VARIABLE THAT DOES NOT HAVE A BASIC TYPE (e.g. int, string ).  IGNORED." << std::endl;
		}
	} else {
		std::cerr << "WARNING: ATTEMPT TO INITIALIZE A VARIABLE THAT IS A FUNCTION ARGUMENT.  IGNORED." << std::endl;
	}
}

/**********/
/* IOBase */
/**********/
IOBase::IOBase( PrimitiveBlock &primitiveBlock, SFC::ArgDeclBase argDeclBase ) :
 ArgDeclBaseBase( argDeclBase ),
 _primitiveBlockBaseProxySP( primitiveBlock.getProxySP() ),
 _convertToArray( true ),
 _alreadyAccessed( false ) {
 
	setSymbolTableName(  convertToMatLabIdentifierName( Super::getSFCArgDeclBase().name() )  );
}

IOBase::IOBase( PrimitiveBlock &primitiveBlock, ArgDeclBase argDeclBase ) :
 ArgDeclBaseBase( argDeclBase.getSFCArgDeclBase() ),
 _primitiveBlockBaseProxySP( primitiveBlock.getProxySP() ),
 _convertToArray( true ),
 _alreadyAccessed( false ) { }

void IOBase::checkFirstAccess( void ) {
	if ( _alreadyAccessed ) return;
	_alreadyAccessed = true;

	std::string name = Super::getName();
	if ( name.empty() ) return;
	registerInSymbolTable();
	convertToArray();
}

void IOBase::convertToArrayBase( bool inputConversion ) {
	if ( _primitiveBlockBaseProxySP->getKeepStructs() || getKeepStruct() || !isStruct() ) return;

	std::string newName = Super::getName() + "_array";
	ArgDeclBase argDeclBase = _primitiveBlockBaseProxySP->createVariable( newName, SFC::DT() );

	boost::python::list inputs, outputs;
	inputs.append( argDeclBase );
	outputs.append(  IO( *this )  );

	if ( inputConversion ) std::swap( inputs, outputs );

	std::string result = CodeGenerator::getSingleton().generateMatLabCode( "Demux", inputs, outputs, boost::python::dict() );
	_primitiveBlockBaseProxySP->addStatement( result, inputConversion );

	setArgDeclBase( argDeclBase.getArgDeclBaseBase() );
}

void IOBase::alias( const std::string &name ) {
	if ( _sfcArgDeclBase == Udm::null ) {
		std::cerr << "Warning:  Attempt to alias null variable" << std::endl;
	} else {
		checkFirstAccess();
		getSFCSymbolTable().push( name, _sfcArgDeclBase );
	}
}

ParameterBase::ParameterBase( PrimitiveBlock &primitiveBlock, SLSF::Parameter parameter ) :
 _primitiveBlockBaseProxySP( primitiveBlock.getProxySP() ),
 _parameter( parameter ),
 _argDeclBaseSP(  static_cast< ArgDeclBase * >( 0 )  ) {
	if (  SFC::LocalVar::Cast( _parameter.memb() ) == Udm::null  ) return;

	SFC::LocalVar parameterLV = SFC::LocalVar::Cast( _parameter.memb() );

	_argDeclBaseSP = ArgDeclBaseSP(  new ArgDeclBase( parameterLV, true )  );
}

ArgDeclBase ParameterBase::placeInContext( void ) {

	if ( _argDeclBaseSP != 0 ) return *_argDeclBaseSP;

	SFC::CompoundStatement context = _primitiveBlockBaseProxySP->getContext();
	SLSF::Block block = SLSF::Block::Cast( _parameter.GetParent() );
	SFC::Struct sfcStruct = SFCManager::getClassStruct( context );

	SFC::DT dt = getMatrixSFCType( context, getValue() );

	if ( dt == Udm::null ) {
		std::cerr << "ERROR:  placeInContext:  Could not get data type from value (\"" << getValue() << "\") of parameter \"" << getName() << "\"" << std::endl;
		std::cerr << "        Full path of parameter is \"" << _parameter.getPath( "/" ) << "\"" << std::endl;
		throw std::exception( "Bad value for \"placeInContext\"" );
	}

	SFC::BasicType basicType = _primitiveBlockBaseProxySP->getPrimitiveBlockBaseSP()->getParameterBaseType();
	if ( basicType != Udm::null ) dt = SFCTypesManager::getSingleton().getNewTypeArray( dt, basicType );

	SFCTypesManager::getSingleton().makeTypeUnique( dt );
	SFC::LocalVar parameterLV = SFCTypesManager::createMember( sfcStruct, getName(), dt );
	_parameter.memb() = parameterLV;

	_argDeclBaseSP = ArgDeclBaseSP(  new ArgDeclBase( parameterLV, true )  );
	return *_argDeclBaseSP;
}

ArgDeclBase ParameterBase::placeInContext( DT dt ) {
	ArgDeclBase argDeclBase = placeInContext();
	argDeclBase.setType( dt );
	return argDeclBase;
}

Block BlockBase::getParentBlock( void ) {

	Udm::Object object = _block.GetParent();
	if (  object != Udm::null && !Udm::IsDerivedFrom( object.type(), SLSF::Block::meta )  ) {
		object = Udm::null;
	}

	return Block(  SLSF::Block::Cast( object )  );
}

PrimitiveBlockBaseProxy::PrimitiveBlockBaseProxy( PrimitiveBlockBaseWP primitiveBlockBaseWP, SFC::CompoundStatement context ) :
 _primitiveBlockBaseWP( primitiveBlockBaseWP ),
 _context( context ),
 _contextArgName(  SFCManager::getFirstFunctionArg( context ).name()  ),
 _zeroIndexingFlag( true ),
 _acceptCSyntaxFlag( false ),
 _keepStructsFlag( false ) { }


ArgDeclBase PrimitiveBlockBaseProxy::createGlobalVariable( const std::string &name, const ParameterBase &initialValue ) {

	SLSF::Parameter initialValueParameter = initialValue.getESMoLParameter();

	SFC::Struct sfcStruct = SFCManager::getClassStruct( _context );
	SFC::Project project = SFCManager::getProject( sfcStruct );

	SFC::DT valueParameter_dt = getMatrixSFCType( _context, initialValueParameter.Value() );
	SFC::BasicType basicType = getPrimitiveBlockBaseSP()->getParameterBaseType();
	if ( basicType != Udm::null ) valueParameter_dt = SFCTypesManager::getSingleton().getNewTypeArray( valueParameter_dt, basicType );

	SFC::LocalVar localVar = SFCManager::createLocalVar( project, "", valueParameter_dt );

	std::string varName = name;
	Block varBlock = Block( *_primitiveBlockBaseWP.lock() );
	while( varBlock.hasParentBlock() ) {
		varBlock = varBlock.getParentBlock();
		varName = varBlock.getName() + "_" + varName;
	}
	localVar.name() = varName;

	localVar.pstrct() = sfcStruct;
	initialValueParameter.memb() = localVar;

	return ArgDeclBase( localVar );
}

ArgDeclBase PrimitiveBlockBaseProxy::createContextVariable( const std::string &name, DTBase dt ) {
	SFC::Struct sfcStruct = SFCManager::getClassStruct( _context );
	std::string newName = name + "_" + boost::lexical_cast< std::string >( Block::getUniqueNumber() );
	SFC::LocalVar contextLV = SFCTypesManager::createMember( sfcStruct, newName, dt.getSFCType() );

	return ArgDeclBase( contextLV, true );
}

ArgDeclBase PrimitiveBlockBaseProxy::createContextVariable( const std::string &name, const std::string &typeName ) {
	SFC::Struct sfcStruct = SFCManager::getClassStruct( _context );
	std::string newName = name + "_" + boost::lexical_cast< std::string >( Block::getUniqueNumber() );
	SFC::LocalVar contextLV = SFCTypesManager::createMember(  sfcStruct, newName, SFCTypesManager::getSingleton().getBasicType( typeName )  );

	return ArgDeclBase( contextLV, true );
}

ArgDeclBase PrimitiveBlockBaseProxy::createVariable( const std::string &name, DTBase dt ) {
	std::string newName = name + "_" + boost::lexical_cast< std::string >( Block::getUniqueNumber() );
	SFC::LocalVar localVar = SFCManager::createLocalVar( _context, newName, dt.getSFCType() );

	return ArgDeclBase( localVar );
}

ArgDeclBase PrimitiveBlockBaseProxy::createVariable( const std::string &name, const std::string &typeName ) {
	std::string newName = name + "_" + boost::lexical_cast< std::string >( Block::getUniqueNumber() );
	SFC::LocalVar localVar = SFCManager::createLocalVar(  _context, newName, SFCTypesManager::getSingleton().getBasicType( typeName )  );

	return ArgDeclBase( localVar );
}

PrimitiveBlockBase::PrimitiveBlockBase(
 SLSF::Block block, SFC::CompoundStatement context, ArgDeclBaseList &inputs, ArgDeclBaseList &outputs, ParameterMap &parameters
) :
 Super( block ),
 _context( context ),
 _rawInputs( inputs ),
 _rawOutputs( outputs ),
 _rawParameters( parameters ),
 _hasParameterBaseType( false ) { }

void PrimitiveBlockBase::initialize( void ) {

	_primitiveBlockBaseProxySP = PrimitiveBlockBaseProxySP(
		new PrimitiveBlockBaseProxy(  boost::static_pointer_cast< PrimitiveBlockBase >( getWP().lock() ), _context  )
	);

	for( ArgDeclBaseList::iterator inpItr = _rawInputs.begin() ; inpItr != _rawInputs.end() ; ++inpItr ) {
		PrimitiveBlock primitiveBlock( *this );
		_inputs.append(  Input( primitiveBlock, *inpItr )  );
	}

	for( ArgDeclBaseList::iterator otpItr = _rawOutputs.begin() ; otpItr != _rawOutputs.end() ; ++otpItr ) {
		PrimitiveBlock primitiveBlock( *this );
		_outputs.append(  Output( primitiveBlock, *otpItr )  );
	}

	for( ParameterMap::iterator prmItr = _rawParameters.begin() ; prmItr != _rawParameters.end() ; ++prmItr ) {
		PrimitiveBlock primitiveBlock( *this );
		_parameters[ prmItr->first ] = Parameter( primitiveBlock, prmItr->second );
	}
}

SFC::BasicType PrimitiveBlockBase::getParameterBaseType( void ) {
	if ( _hasParameterBaseType ) return _parameterBaseType;
	_hasParameterBaseType = true;

	ParameterMap::iterator prmItr = _rawParameters.find( "ParamDataTypeStr" );
	if ( prmItr != _rawParameters.end() ) {
		std::string baseTypeName = prmItr->second.Value();
		if (  notInherited( baseTypeName )  ) {
			return _parameterBaseType = SFCTypesManager::getSingleton().getBasicType( baseTypeName );
		}
	}

	if (  _rawInputs.empty() || _rawParameters.find( "OutDataTypeStr" ) == _rawParameters.end()  ) {
		for( ArgDeclBaseList::iterator ablItr = _rawOutputs.begin() ; ablItr != _rawOutputs.end() ; ++ablItr ) {
			SFC::DT dt = SFCTypesManager::getSingleton().getBaseDT( ablItr->dt() );
			if ( dt.type() == SFC::BasicType::meta ) return _parameterBaseType = SFC::BasicType::Cast( dt );
		}
	}

	return SFC::BasicType();
}
