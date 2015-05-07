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
 *  FILE NAME:	SFCUdmEngine.cpp 
 *	SYNOPSIS:	Implementation of class "SFCUdmEngine", for setting up a Udm file to
 *				accept the SFC translation of an ECSL_DP (or similar paradigm) file.
 *  AUTHOR:		Harmon Nine - hnine@isis.vanderbilt.edu
 *  CREATED:	12/12/07
 *  LAST UPDATE: 
 */
#include <boost/regex.hpp>
#include "SFCUdmEngine.hpp"

SFC::Class SFCUdmEngine::getGlobalFunctionsClass( void ) {
	if ( _globalFunctionsClass != Udm::null ) return _globalFunctionsClass;

	SFC::Program program;
	ProgramSet programSet = getRoot().Program_kind_children();
	for( ProgramSet::iterator pgsItr = programSet.begin() ; pgsItr != programSet.end() ; ++pgsItr ) {
		if ( pgsItr->filename() == "Globals" ) {
			program = *pgsItr;
			break;
		}
	}
	if ( program == Udm::null ) {
		program = SFC::Program::Create( getRoot() );
		SFCManager::numberStatement( program );
		program.filename() = "Globals";
	}
	
	ClassSet classSet = program.Class_kind_children();
	for( ClassSet::iterator clsItr = classSet.begin() ; clsItr != classSet.end() ; ++clsItr ) {
		if ( clsItr->name() == "Globals" ) {
			_globalFunctionsClass = *clsItr;
			break;
		}
	}
	if ( _globalFunctionsClass == Udm::null ) {
		_globalFunctionsClass = SFC::Class::Create( program );
		SFCManager::numberStatement( _globalFunctionsClass );
		_globalFunctionsClass.name() = "Globals";
	}

	return _globalFunctionsClass;
}

std::string SFCManager::unescapeChars( const std::string &str ) {
	static boost::regex escapesRegex( "&#[Xx]([[:xdigit:]]{1,2});", boost::regex_constants::perl );
	boost::match_results< std::string::const_iterator > results;

	std::string newString = str;
	while (  boost::regex_search( newString, results, escapesRegex )  ) {
		std::string hexString = std::string( results[1].first, results[1].second );
		std::istringstream unescape( hexString );
		int unescapedChar = 0;
		unescape >> std::hex >> unescapedChar;

		newString = std::string( results.prefix().first, results.prefix().second ) + char( unescapedChar )
		 + std::string( results.suffix().first, results.suffix().second );
	}

	return newString;
}

void SFCManager::numberStatement( SFC::Statement statement ) {
	SFC::CompoundStatement compoundStatement = statement.csPar();
	__int64 statementCount = compoundStatement.statementCount();
	statement.statementIndex() = statementCount++;
	compoundStatement.statementCount() = statementCount;
}

SFC::LocalVar SFCManager::createUniqueLocalVar( SFC::CompoundStatement block, const std::string &name ) {
	SFC::LocalVar localVar = SFC::LocalVar::Create( block );
	localVar.name() = name + boost::lexical_cast< std::string >( localVar.uniqueId() );
	numberStatement( localVar );
	return localVar;
}

SFC::LocalVar SFCManager::createLocalVar( SFC::CompoundStatement context, const std::string &name, SFC::DT dt ) {
	SFC::LocalVar localVar = SFC::LocalVar::Create( context );
	numberStatement( localVar );
	localVar.name() = name;
	localVar.dt() = dt;
	return localVar;
}

SFC::Project SFCManager::getProject( SFC::Statement statement ) {
	return getSFCObject< SFC::Project >( statement );
}

SFC::Program SFCManager::getProgram( SFC::Statement statement ) {
	return getSFCObject< SFC::Program >( statement );
}

SFC::Class SFCManager::getClass( SFC::Statement statement ) {
	return getSFCObject< SFC::Class >( statement );
}

SFC::Function SFCManager::getFunction( SFC::Statement statement ) {
	return getSFCObject< SFC::Function >( statement );
}

SFC::Struct SFCManager::getClassStruct( SFC::Statement statement ) {
	SFC::Class sfcClass = getClass( statement );
	if ( sfcClass == Udm::null ) return SFC::Struct();

	SFC::StructSet structSet = sfcClass.Struct_kind_children();

	return structSet.empty() ? SFC::Struct() : *structSet.begin();
}

bool SFCManager::isContextVariable( SFC::ArgDeclBase argDeclBase ) {
	if ( argDeclBase != Udm::null && argDeclBase.type() != SFC::LocalVar::meta ) return false;
	SFC::LocalVar localVar = SFC::LocalVar::Cast( argDeclBase );

	SFC::Struct classStruct = getClassStruct( localVar );
	if ( classStruct == Udm::null ) return false;
	SFC::DT dt = localVar.dt();
	return dt != Udm::null && dt == classStruct;
}

SFC::Arg SFCManager::getFirstFunctionArg( SFC::Statement statement ) {

	SFC::Function function = getFunction( statement );

	if ( function == Udm::null ) return SFC::Arg::Cast( statement );

	SFC::ArgSet argSet = function.Arg_children_sorted( SFC::ArgSorter() );
	return *argSet.begin();
}

void SFCManager::printDT( SFC::DT dt, const std::string &indent, bool useIndent ) {

	if ( useIndent ) std::cout << indent;

	if ( dt == Udm::null ) {
		std::cout << "NULL TYPE" << std::endl;
		return;
	}
	
	if ( dt.type() == SFC::BasicType::meta ) {
		SFC::BasicType basicType = SFC::BasicType::Cast( dt );
		std::cout << basicType.name() << std::endl;
		return;
	} 
	
	if ( dt.type() == SFC::Array::meta ) {

		bool notFirst = false;
		std::cout << indent;
		while( dt.type() == SFC::Array::meta ) {
			if ( notFirst ) std::cout << "x";
			else            notFirst = true;
			SFC::Array sfcArray = SFC::Array::Cast( dt );
			std::cout << sfcArray.noelem();
			dt = sfcArray.dt();
		}
		std::cout << " ARRAY of ";
		printDT( dt, indent + "  ", false );
		return;
	}

	SFC::Struct sfcStruct = SFC::Struct::Cast( dt );
	std::cout << "STRUCT " << sfcStruct.name() << ":" << std::endl;
	SFCTypesManager::LocalVarSet localVarSet = sfcStruct.LocalVar_kind_children_sorted( SFCTypesManager::StatementSorter() );
	std::string newIndent = indent + "  ";
	for( SFCTypesManager::LocalVarSet::iterator lvsItr = localVarSet.begin() ; lvsItr != localVarSet.end() ; ++lvsItr ) {
		printDT( lvsItr->dt(), newIndent );
	}
	return;
}

void SFCTypesManager::makeUnique( const std::string &typeName, SFC::DT &dt ) {
	SFCTypeMap::iterator stmItr = _sfcTypeMap.find( typeName );
	if ( stmItr == _sfcTypeMap.end() ) {
		if ( dt.name() == "" ) dt.name() = typeName;
		SFCManager::numberStatement( dt );
		_sfcTypeMap.insert(  std::make_pair( typeName, dt )  );
	} else if ( dt != stmItr->second ) {
		transferType( dt, stmItr->second );
		dt.DeleteObject();
		dt = stmItr->second;
	}
}

void SFCTypesManager::transferType( SFC::DT &from, SFC::DT &to ) {
	if ( to.type() == SFC::Struct::meta ) {

		LocalVarSet fromLocalVarSet = SFC::Struct::Cast( from ).LocalVar_kind_children_sorted( StatementSorter() );
		LocalVarSet toLocalVarSet = SFC::Struct::Cast( to ).LocalVar_kind_children_sorted( StatementSorter() );

		LocalVarSet::iterator flsItr = fromLocalVarSet.begin();
		LocalVarSet::iterator tlsItr = toLocalVarSet.begin();
		for( ; flsItr != fromLocalVarSet.end() ; ++flsItr, ++tlsItr ) {
			ArgDeclRefSet fromArgDeclRefSet = flsItr->ref();
			std::for_each(  fromArgDeclRefSet.begin(), fromArgDeclRefSet.end(), TransferMemberFunctor( *tlsItr )  );
		}
	}
	TypedEntitySet typedEntitySet = from.te();
	std::for_each(  typedEntitySet.begin(), typedEntitySet.end(), TransferTypeFunctor( to )  );
}


SFC::BasicType SFCTypesManager::getBasicType( const std::string &typeName ) {
	SFCTypeMap::iterator stmItr = _sfcTypeMap.find( typeName );
	if ( stmItr == _sfcTypeMap.end() ) {
		SFC::BasicType basicType = SFC::BasicType::Create( _typesBlock );
		basicType.name() = typeName;
		SFCManager::numberStatement( basicType );
		_sfcTypeMap.insert(  std::make_pair( typeName, basicType )  );
		return basicType;
	}

	SFC::DT dt = stmItr->second;
	assert( dt.type() == SFC::BasicType::meta );
	return SFC::BasicType::Cast( dt );
}


SFC::DT SFCTypesManager::copyArrayDT( SFC::DT dt ) {

	SFC::DT dtCopy;
	SFC::Array lastArray;
	while( dt.type() == SFC::Array::meta ) {
		SFC::Array sfcArray = SFC::Array::Cast( dt );
		dt = sfcArray.dt();

		if ( lastArray == Udm::null ) {
			dtCopy = lastArray = makeArray(  static_cast< int >( sfcArray.noelem() )  );
		} else {
			SFC::Array newLastArray = makeArray(  static_cast< int >( sfcArray.noelem() )  );
			lastArray.dt() = newLastArray;
			lastArray = newLastArray;
		}
	}
	if ( dtCopy == Udm::null ) dtCopy = dt;
	else                       lastArray.dt() = dt;

	return dtCopy;
}


SFC::Array SFCTypesManager::getArrayTail( SFC::DT dt ) {
	SFC::Array arrayTail;
	while( dt.type() == SFC::Array::meta ) {
		arrayTail = SFC::Array::Cast( dt );
		dt = arrayTail.dt(); 
	}
	return arrayTail;
}


void SFCTypesManager::getRowsAndColumns( SFC::DT dt, __int64 &rows, __int64 &columns ) {

	rows = 0;
	columns = 0;

	if ( dt == Udm::null ) return;

	rows = 1;
	columns = 1;

	if ( dt.type() == SFC::Array::meta ) {
		SFC::Array sfcArray = SFC::Array::Cast( dt );
		rows = sfcArray.noelem();

		dt = sfcArray.dt();
		if ( dt.type() == SFC::Array::meta ) {
			sfcArray = SFC::Array::Cast( dt );
			columns = sfcArray.noelem();
		}
	}
}


SFC::DT SFCTypesManager::getMatrix( int rows, int columns, SFC::DT dt ) {

	if ( rows == 1 && columns == 1 ) return dt;

	SFC::Array sfcArray = makeArray( rows );

	if ( columns > 1 ) {
		SFC::Array sfcArray2 = makeArray( columns );
		sfcArray2.dt() = dt;
		sfcArray.dt() = sfcArray2;
	} else {
		sfcArray.dt() = dt;
	}

	makeTypeUnique( sfcArray );
	return sfcArray;
}

SFC::DT SFCTypesManager::combineDT( SFC::DT dt1, SFC::DT dt2 ) {
	if ( dt1 == Udm::null ) return dt2;
	if ( dt2 == Udm::null ) return dt1;

	SFC::DT baseDT1 = getBaseDT( dt1 );
	SFC::DT baseDT2 = getBaseDT( dt2 );

	SFC::DT dominantType;
	if ( baseDT1.type() == SFC::BasicType::meta ) {
		assert( baseDT2.type() == SFC::BasicType::meta );
		dominantType = getDominantType(  SFC::BasicType::Cast( baseDT1 ),  SFC::BasicType::Cast( baseDT2 )  );
	} else {
		assert( baseDT2.type() != SFC::BasicType::meta );

		TypedEntitySet typedEntitySet = baseDT2.te();
		SFC::TypedEntity typedEntity = *typedEntitySet.begin();
		SFCTypesManager::getSingleton().makeTypeUnique( baseDT1 );
		baseDT2 = typedEntity.dt();

		typedEntitySet = baseDT1.te();
		typedEntity = *typedEntitySet.begin();
		SFCTypesManager::getSingleton().makeTypeUnique( baseDT2 );
		baseDT1 = typedEntity.dt();

		assert(  static_cast< std::string >( baseDT1.name() ) == static_cast< std::string >( baseDT2.name() )  );
		dominantType = baseDT1;

	}

	DimensionVector dimensionVector1 = getDimensions( dt1 );
	DimensionVector allDimensionVector1 = getAllDimensions( dt1 );
	DimensionVector dimensionVector2 = getDimensions( dt2 );
	DimensionVector allDimensionVector2 = getAllDimensions( dt2 );

	if ( dimensionVector1.size() < dimensionVector2.size() ) {
		return getNewTypeArray( dt2, dominantType );
	} else if ( dimensionVector1.size() > dimensionVector2.size() ) {
		return getNewTypeArray( dt1, dominantType );
	// COLUMN VECTORS DOMINANT OVER ROW VECTORS
	} else if ( allDimensionVector1.size() < allDimensionVector2.size() ) {
		return getNewTypeArray( dt1, dominantType );
	} else {
		return getNewTypeArray( dt2, dominantType );
	}

}

SFC::DT SFCTypesManager::getNewTypeArray( SFC::DT arrayDT, SFC::DT baseDT ) {

	if ( arrayDT.type() != SFC::Array::meta ) return baseDT;

	ArrayDTBuilder arrayDTBuilder( baseDT );
	while( arrayDT.type() == SFC::Array::meta ) {
		SFC::Array sfcArray = SFC::Array::Cast( arrayDT );
		arrayDTBuilder.addArray(  static_cast< int >( sfcArray.noelem() )  );
		arrayDT = sfcArray.dt();
	}
	return arrayDTBuilder.getDT();
}

SFC::DT SFCTypesManager::getNewTypeArray( SFC::DT arrayDT, const std::string &baseDTName ) {
	return getNewTypeArray(  arrayDT, this->getBasicType( baseDTName )  );
}


SFCTypesManager::DimensionVector SFCTypesManager::getDimensions( SFC::DT dt ) {
	DimensionVector dimensionVector;
	while( dt.type() == SFC::Array::meta ) {
		SFC::Array sfcArray = SFC::Array::Cast( dt );
		int noelem = static_cast< int >(  sfcArray.noelem()  );
		if ( noelem > 1 ) dimensionVector.push_back( noelem );
		dt = sfcArray.dt();
	}
	return dimensionVector;
}
SFCTypesManager::DimensionVector SFCTypesManager::getAllDimensions( SFC::DT dt ) {
	DimensionVector dimensionVector;
	while( dt.type() == SFC::Array::meta ) {
		SFC::Array sfcArray = SFC::Array::Cast( dt );
		int noelem = static_cast< int >( sfcArray.noelem() );
		dimensionVector.push_back( noelem );
		dt = sfcArray.dt();
	}
	return dimensionVector;
}

SFC::LocalVar SFCTypesManager::createMember( SFC::Struct sfcStruct, const std::string &memberName, SFC::DT dt ) {
	SFC::LocalVar localVar = SFC::LocalVar::Create( sfcStruct, SFC::Struct::meta_memb );
	localVar.name() = memberName + boost::lexical_cast< std::string >( localVar.uniqueId() );
	localVar.dt() = dt;

	__int64 memberCount = sfcStruct.memberCount();
	localVar.statementIndex() = memberCount++;
	sfcStruct.memberCount() = memberCount;

	// DON'T MAKE UNIQUE -- LET TRANSFORM DO IT.
	return localVar;
}


std::string SFCTypesManager::makeTypeUnique( SFC::DT &dt ) {
	if ( dt == Udm::null ) { /* THROW ERROR */ assert( false ); }

	if ( dt.type() == SFC::BasicType::meta ) {
		SFC::BasicType basicType = SFC::BasicType::Cast( dt );
		std::string typeName = basicType.name();
		makeUnique( typeName, dt );
		return typeName;
	}

	if ( dt.type() == SFC::Array::meta ) {
		SFC::Array sfcArray = SFC::Array::Cast( dt );
		SFC::DT arrayDT = sfcArray.dt();
		std::string typeName = makeTypeUnique( arrayDT );
		typeName = std::string( "A" ) +
		 boost::lexical_cast< std::string >(  static_cast< __int64 >( sfcArray.noelem() )  ) + typeName + "A";
		makeUnique( typeName, dt );
		return typeName;
	}

	if ( dt.type() == SFC::Struct::meta ) {
		SFC::Struct sfcStruct = SFC::Struct::Cast( dt );
		std::string typeName = "S";
		LocalVarSet localVarSet = sfcStruct.LocalVar_kind_children_sorted( StatementSorter() );

		StructUniqueFunctor structUniqueFunctor = for_each(
		 localVarSet.begin(), localVarSet.end(), StructUniqueFunctor( *this )
		);
		typeName += structUniqueFunctor.getCompoundName() + "S";
		makeUnique( typeName, dt );
		return typeName;
	}

	assert( false );
	return "ERROR: NO TYPE NAME";
}

SFC::DT ArrayDTBuilder::getDT( void ) {
	_isFinished = true;

	if ( _dt == Udm::null ) return _dtBase;

	int noDimensions = 0;
	int secondNoElems = 0;
	SFC::DT dt1 = _dt;
	while( dt1 != Udm::null ) {
		SFC::Array sfcArray = SFC::Array::Cast( dt1 );
		(void)++noDimensions;
		if ( noDimensions == 2 ) secondNoElems = static_cast< int >( sfcArray.noelem() );
		dt1 = sfcArray.dt();
	}

	if ( noDimensions == 2 && secondNoElems > 1 ) {

		dt1 = _sfcArray;

	} else {

		dt1 = SFC::DT();
		SFC::DT dt2 = _dt;

		while( dt2 != Udm::null ) {
			SFC::Array sfcArray2 = SFC::Array::Cast( dt2 );
			SFC::DT dt3 = sfcArray2.dt();

			if ( sfcArray2.noelem() == 1 ) {
				dt2.DeleteObject();
				if ( dt1 == Udm::null ) _dt = dt3;
				else                    dt1.dt() = dt3;
				dt2 = dt3;
			} else {
				dt1 = dt2;
				dt2 = dt3;
			}
		}

	}

	if ( _dt == Udm::null ) _dt = _dtBase;
	else                    SFC::Array::Cast( dt1 ).dt() = _dtBase;

	SFCTypesManager::getSingleton().makeTypeUnique( _dt );
	return _dt;
}