#include "ReferenceReturn.hpp"
#include "ArgReturn.hpp"
#include "LitMatrixExprReturn.hpp"
#include "VarMatrixExprReturn.hpp"
#include "MFileManager.hpp"
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <limits>
#include <cmath>

ArgReturnSP argTree( const pANTLR3_BASE_TREE tree, SFC::CompoundStatement block, bool lvalue = false );

COMBINE_VISITOR_DEFINITION( ReferenceReturn )

ExprReturnSP ReferenceReturn::create(
 SFC::CompoundStatement block,
 const std::string &id,
 const PAntlr3BaseTreeList &pAntlr3BaseTreeList,
 bool lvalue,
 ExprReturnSP exprReturnSP
) {

	SFC::ArgDeclBase argDeclBase;

	if ( pAntlr3BaseTreeList.empty() ) {

		if ( id == "pi" ) return LitMatrixExprReturn::create( block, M_PI );
		if ( id == "e" ) return LitMatrixExprReturn::create( block, M_E );
		if ( id == "eps" && pAntlr3BaseTreeList.empty() ) return LitMatrixExprReturn::create(  block, std::pow( 2.0, -52.0 )  );

// KLUDGE DUE TO "max" DEFINED AS MACRO -- INTERFERES WITH std::numeric_limits
#define temp_max max
#undef max
		if ( id == "inf" ) return LitMatrixExprReturn::create( block, std::numeric_limits< float >::max() );
#define max temp_max
#undef temp_max

		if ( id == "true" ) return LitMatrixExprReturn::create( block, true );
		if ( id == "false" ) return LitMatrixExprReturn::create( block, false );
	}

	/* GET LOCAL VARIABLE BASED ON "id" */

	/* AT THIS POINT, "exprReturnSP" DETERMINES IF THE VARIABLE WHOSE NAME IS "id" SHOULD BE LOOKED FOR
	   IN THE SYMBOL TABLE (I.E. exprReturnSP == 0), OR AS A MEMBER OF A STRUCTURE (WHERE exprReturnSP->getDT()
	   IS THIS STRUCTURE).  ANY OTHER VALUE FOR dt IS AN ERROR.
	*/
	if ( exprReturnSP == 0 ) {

		argDeclBase = getSFCSymbolTable().lookup( id );

		/* IF VARIABLE NAMED "id" NOT FOUND IN SYMBOL TABLE */
		if ( argDeclBase == Udm::null ) {

			assert( !lvalue || pAntlr3BaseTreeList.empty() );

			/* IF WE'RE NOT DEFINING A VARIABLE, THEN ASSUME ITS A FUNCTION CALL */
			if ( !lvalue ) return createFunctionCall(  block, id, getArgReturnSPList( block, pAntlr3BaseTreeList )  );

			/* CREATE VARIABLE NAMED "id", WHICH IS, FOR NOW, TYPELESS */
			SFC::LocalVar localVar = SFCManager::createUniqueLocalVar(  SFCManager::getFunction( block ), id  );
			getSFCSymbolTable().push( id, localVar );

			argDeclBase = localVar;
		}

	/* VARIABLE SHOULD BE MEMBER OF A STRUCTURE */
	} else {

		SFC::DT dt = exprReturnSP->getDT();

		/* MUST GIVE ArgDeclBase TO LEFT OF DOT (.) A STRUCT TYPE HERE IF IT DOESN'T HAVE ONE. */
		/* THIS IS BECAUSE WE NEED A STRUCTURE IN WHICH TO LOOK FOR A MEMBER REPRESENTED BY "id" */

		if ( dt == Udm::null ) {
			assert( exprReturnSP->getClassName() == ReferenceReturn::getName() );

			ReferenceReturnSP referenceReturnSP = ExprReturn::static_pointer_cast< ReferenceReturn >( exprReturnSP );
			dt = SFCTypesManager::getSingleton().getStruct();
			referenceReturnSP->setDT( dt );
		}


		SFC::DT baseDT = SFCTypesManager::getSingleton().getBaseDT( dt );
		assert( baseDT.type() == SFC::Struct::meta );

		SFC::Struct sfcStruct = SFC::Struct::Cast( baseDT );
		argDeclBase = findMember( sfcStruct, id );

		/* IF LocalVar NAMED "id" IS NOT A MEMBER OF THE STRUCT */
		if ( argDeclBase == Udm::null) {

			/* TO DEFINE A STRUCTURE MEMBER, THE STRUCTURE VARIABLE MUST BE A PURE STRUCT, */
			/* I.E. NOT AN ARRAY OF STRUCT */
			assert( lvalue && dt.type() == SFC::Struct::meta );

			/* CREATE MEMBER NAMED "id" IN STRUCT */
			SFC::LocalVar localVar = SFC::LocalVar::Create( sfcStruct );
			localVar.name() = id;
			__int64 memberCount = sfcStruct.memberCount();
			localVar.statementIndex() = memberCount++;
			sfcStruct.memberCount() = memberCount;

			argDeclBase = localVar;
		}

	}

	/* GET TYPE OF argDeclBase */
	SFC::DT argDeclBaseDT = argDeclBase.dt();

	if ( argDeclBaseDT == Udm::null ) return ReferenceReturn::create( block, argDeclBase );


	/* PROCESS ARGUMENTS */
	Exprs exprs = VE( argDeclBase );

	std::string indexBaseString = argDeclBase.indexBase();
	int indexBase = 0;
	boost::trim( indexBaseString );
	if ( indexBaseString.empty() ) {
		indexBase = getZeroIndex() ? 0 : 1;
	} else {
		indexBase = boost::lexical_cast< int >( indexBaseString );
	}

	ArrayDTBuilder arrayDTBuilder( argDeclBaseDT );
	ExprsProxyVector exprsProxyVector;

	/* ADD ARGUMENTS AS INDEXES TO LOCALVAR */
	if ( !pAntlr3BaseTreeList.empty() ) {

		getEndStack().push_back( -1 );

		for( PAntlr3BaseTreeList::const_iterator pblCit = pAntlr3BaseTreeList.begin() ; pblCit != pAntlr3BaseTreeList.end() ; ++pblCit ) {

			/* FOR EASE OF ACCESS TO THE argReturn OBJECT BEING PROCESSED */
			pANTLR3_BASE_TREE tree = *pblCit;

			if ( argDeclBaseDT.type() == SFC::BasicType::meta ) {
				ArgReturnSP argReturnSP = argTree( tree, block ); 
				ExprReturnSP exprReturnSP = argReturnSP->getExprReturnSP();
				if ( exprReturnSP->getClassName() == "LitMatrixExprReturn" ) {
					LitMatrixExprReturnSP litMatrixExprReturnSP = ExprReturn::static_pointer_cast< LitMatrixExprReturn >( exprReturnSP );
					LitMatrixExprReturn::Sequence &sequence = litMatrixExprReturnSP->getSequence();
					int baseValue = getZeroIndex() ? 0 : 1;
					if ( sequence.size() == 1 && *sequence.begin() == baseValue ) continue;
				}
				std::cerr << "Too many indexing operators!!" << std::endl;
			}

			assert( argDeclBaseDT.type() == SFC::Array::meta );

			/* UPDATE argDeclBaseDT -- I.E. MOVE IT DOWN THE SFC::Array TYPE-CHAIN */
			SFC::Array sfcArray = SFC::Array::Cast( argDeclBaseDT );
			argDeclBaseDT = sfcArray.dt();

			/* CHECK FOR A ROW-VECTOR (THIS SHOULD ONLY HAPPEN IN CASE OF A ROW VECTOR) */
			if ( sfcArray.noelem() == 1 ) {
				arrayDTBuilder.addArray( 1 );
				sfcArray = SFC::Array::Cast( argDeclBaseDT );
				argDeclBaseDT = sfcArray.dt();
			}

			{
				int noElem = (int) sfcArray.noelem();
				if ( getZeroIndex() ) --noElem;
				getEndStack().replace_back( noElem );
			}
			ArgReturnSP argReturnSP = argTree( tree, block );

			Exprs indexExprs;

			switch( argReturnSP->getType() ) {
			
				case ArgReturn::EXPRRETURN:
					{
						ExprReturnSP exprReturnSP1 = argReturnSP->getExprReturnSP();
						if ( indexBase != 0 ) exprReturnSP1 = exprReturnSP1->combine( "-", indexBase );

						VarExprReturnSP varExprReturnSP = exprReturnSP1->convertToVarExprReturn();
						indexExprs = varExprReturnSP->getExprs();
						SFC::DT indexExprsDT = varExprReturnSP->getDT();
						assert( indexExprsDT != Udm::null );

						if ( varExprReturnSP->getExprsProxyVector().size() != 0 ) {
							if ( varExprReturnSP->getExprsProxyVector().size() > 1 ) { /* THROW ERROR */ assert( false ); }

							SFC::Array indexSFCArray = SFC::Array::Cast( indexExprsDT );
							/* IN CASE OF ROW VECTOR */
							if ( indexSFCArray.noelem() == 1 ) indexSFCArray = SFC::Array::Cast( indexSFCArray.dt() );
							arrayDTBuilder.addArray( (int) indexSFCArray.noelem() );

							exprsProxyVector.push_back( varExprReturnSP->getExprsProxyVector().front() );
						}
						break;
					}

				case ArgReturn::COLONEXPR:
					{
						arrayDTBuilder.addArray( (int) sfcArray.noelem() );

						exprsProxyVector.push_back( ExprsProxy() );
						indexExprs = exprsProxyVector.back();

						break;
					}

				default:
					{
						assert( false );
						break;
					}
			}

			exprs = BE( exprs, "[", indexExprs );
		}

		getEndStack().pop_back();

		while( argDeclBaseDT.type() == SFC::Array::meta ) {
			SFC::Array sfcArray = SFC::Array::Cast( argDeclBaseDT );
			argDeclBaseDT = sfcArray.dt();

			if ( sfcArray.noelem() == 1 ) continue;

			exprs = BE(  exprs, "[", IE( 0 )  );
		}

	} else {

		while( argDeclBaseDT.type() == SFC::Array::meta ) {
			SFC::Array sfcArray = SFC::Array::Cast( argDeclBaseDT );
			argDeclBaseDT = sfcArray.dt();

			arrayDTBuilder.addArray( (int) sfcArray.noelem() );

			if ( sfcArray.noelem() == 1 ) continue;

			exprsProxyVector.push_back( ExprsProxy() );
			exprs = BE( exprs, "[", exprsProxyVector.back() );
		}

	}
	return VarExprReturn::create( block, exprs, exprsProxyVector, arrayDTBuilder.getDT() );
}

ArgReturnSPList ReferenceReturn::getArgReturnSPList( SFC::CompoundStatement block, const PAntlr3BaseTreeList &pAntlr3BaseTreeList ) {
	ArgReturnSPList argReturnSPList;
	for( PAntlr3BaseTreeList::const_iterator pblCit = pAntlr3BaseTreeList.begin() ; pblCit != pAntlr3BaseTreeList.end() ; ++pblCit ) {
		argReturnSPList.push_back(  argTree( *pblCit, block )  );
	}
	return argReturnSPList;
}

int ReferenceReturn::getNextIndex( DimensionVector &countVector, DimensionVector &dimensionVector ) {
		
	unsigned int index = 0;
	unsigned int pos = 0;
	while( pos < (unsigned int) dimensionVector.size() ) {
		index += countVector[ pos ];
		if ( ++pos < dimensionVector.size() ) index *= dimensionVector[ pos ];
	}

	for( pos = 0 ; pos < (unsigned int) countVector.size() ; ++pos ) {
		if ( ++countVector[ pos ] < dimensionVector[ pos ] ) break;
		countVector[ pos ] = 0;
	}

	return index;
}

template< typename SEQUENCE >
SEQUENCE ReferenceReturn::reshape( SEQUENCE &sourceSequence, DimensionVector &sourceDimensionVector, DimensionVector &dimensionVector ) {

	typedef SEQUENCE Sequence;

	DimensionVector sourceCountVector( sourceDimensionVector.size(), 0 );
	DimensionVector countVector( dimensionVector.size(), 0 );
		
	int noElements = 1;
	for( DimensionVector::iterator dmvItr = sourceDimensionVector.begin() ; dmvItr != sourceDimensionVector.end() ; ++dmvItr ) {
		noElements *= *dmvItr;
	}
	
	Sequence sequence( noElements );
	for( int count = 0 ; count < noElements ; ++count ) {

		int sourceIndex = getNextIndex( sourceCountVector, sourceDimensionVector );
		int index = getNextIndex( countVector, dimensionVector );

		sequence[ index ] = sourceSequence[ sourceIndex ];
	}

	return sequence;
}


ExprReturnSP ReferenceReturn::createFunctionCall( SFC::CompoundStatement block, const std::string &id, const ArgReturnSPList &argReturnSPList ) {

	if ( id == "class" ) {
		ArgReturnSPList::const_iterator aslCit = argReturnSPList.begin();
		ExprReturnSP exprReturnSP = (*aslCit)->getExprReturnSP();
		SFC::DT dt = exprReturnSP->getDT();

		std::string baseDTName = SFCTypesManager::getBaseDTName( dt );
		SFC::BasicType intType = SFCTypesManager::getSingleton().getBasicType( "int" );

		LitMatrixExprReturn::Sequence sequence;

		sequence.insert( sequence.end(), baseDTName.begin(), baseDTName.end() );

		LitMatrixExprReturnSP litMatrixExprReturnSP = LitMatrixExprReturn::create( block, 1, (int) sequence.size(), intType, sequence );
		litMatrixExprReturnSP->setIsText( true );
		return litMatrixExprReturnSP;
	}

	if ( id == "eps" ) {

		ExprReturnSP exprReturnSP = argReturnSPList.front()->getExprReturnSP();

		if ( exprReturnSP->getClassName() == "LitMatrixExprReturn" ) {
			LitMatrixExprReturnSP litMatrixExprReturnSP = boost::static_pointer_cast< LitMatrixExprReturn >( exprReturnSP );
			
			std::string typeName;
			SFC::DT dt;
			double value = 0;
			if ( litMatrixExprReturnSP->getIsText() ) {
				typeName = litMatrixExprReturnSP->getText();
				dt = SFCTypesManager::getSingleton().getBasicType( typeName );
				value = 1.0;
			} else {
				dt = litMatrixExprReturnSP->getDT();
				typeName = SFCTypesManager::getSingleton().getBaseDTName( dt );
				LitMatrixExprReturn::Sequence &sequence = litMatrixExprReturnSP->getSequence();
				value = sequence.front();
			}

			bool isFloat = typeName.find( "float" ) != std::string::npos || typeName.find( "single" ) != std::string::npos;

			int subtrahend = isFloat ? 23 : 52;
			int minExponent = isFloat ? -149 : -1074;

			int exponent = int( floor( log( value )/log( 2.0 )  ) ) - subtrahend;

			if ( exponent < minExponent ) exponent = minExponent;

			value = pow( 2.0, exponent );

			LitMatrixExprReturn::Sequence sequence;
			sequence.push_back( value );

			return LitMatrixExprReturn::create( block, 1, 1, dt, sequence );
		}
	}

	if ( id == "eye" ) {
		LitMatrixExprReturnSP litMatrixExprReturnSP =
		 boost::static_pointer_cast< LitMatrixExprReturn >( argReturnSPList.front()->getExprReturnSP() );

		int size = (int) litMatrixExprReturnSP->getSequence().front();
		LitMatrixExprReturn::Sequence sequence;
		for( int ix = 0 ; ix < size ; (void)++ix ) {
			for( int jx = 0 ; jx < size ; (void)++jx ) {
				sequence.push_back( ix == jx ? 1 : 0 );
			}
		}

		SFC::BasicType doubleType = SFCTypesManager::getSingleton().getBasicType( "double" );
		return LitMatrixExprReturn::create( block, size, size, doubleType, sequence );
	}

	if ( id == "isempty" ) {
		VarExprReturnSP varExprReturnSP = boost::static_pointer_cast< VarExprReturn >( argReturnSPList.back()->getExprReturnSP() );
		Exprs exprs = varExprReturnSP->getExprs();
		std::string className = exprs.getClassName();
		if ( className == "VE" ) {
			boost::shared_ptr< VE > veSP = exprs.dynamic_exprs_cast< VE >();
			std::string name = static_cast< std::string >( veSP->getArgDeclBase().name() ) + "_init";
			SFC::ArgDeclBase argDeclBase_init = getSFCSymbolTable().lookup( name );
			Exprs exprs = BE(  VE( argDeclBase_init ), "==", IE( 0 )  );

			ExprReturnSP exprReturnSP = VarExprReturn::create(  block, exprs, SFCTypesManager::getSingleton().getBasicType( "boolean" )  );

			StaticVariableAttribute staticVariableAttribute( argDeclBase_init );
			exprReturnSP->addAttribute( staticVariableAttribute );

			return exprReturnSP;
		}

		LitMatrixExprReturn::Sequence sequence;
		sequence.push_back( 0 );
		return LitMatrixExprReturn::create( block, 1, 1, SFCTypesManager::getSingleton().getBasicType( "int" ), sequence );
	}

	if ( id == "isnan" || id == "isinf" ) {
		ExprsProxyVector exprsProxyVector;

		FE fe( id );
		for( ArgReturnSPList::const_iterator aslCit = argReturnSPList.begin() ; aslCit != argReturnSPList.end() ; ++aslCit ) {
			VarExprReturnSP varExprReturnSP = (*aslCit)->getExprReturnSP()->convertToVarExprReturn();
			fe.addArgVal( varExprReturnSP->getExprs() );
			exprsProxyVector = unifyExprsProxyVectors( exprsProxyVector, varExprReturnSP->getExprsProxyVector() );
		}

		Exprs exprs = fe;
		return VarExprReturn::create(  block, exprs, exprsProxyVector, SFCTypesManager::getSingleton().getBasicType( "boolean" )  );
	}

	if ( id == "ones" ) {
		return initArray( block, argReturnSPList, 1 );
	}

	if ( id == "reshape" ) {

		ArgReturnSPList::const_iterator aslCit = argReturnSPList.begin();
		ExprReturnSP exprReturnSP = (*aslCit)->getExprReturnSP();

		DimensionVector sourceDimensionVector = exprReturnSP->getDimensions();

		SFC::DT dt = SFCTypesManager::getBaseDT( exprReturnSP->getDT() );
		ArrayDTBuilder arrayDTBuilder( dt );

		DimensionVector dimensionVector;
		++aslCit;
		if ( argReturnSPList.size() == 2 ) {
			LitMatrixExprReturnSP litMatrixExprReturnSP = boost::static_pointer_cast< LitMatrixExprReturn >( (*aslCit)->getExprReturnSP() );
			LitMatrixExprReturn::Sequence sequence = litMatrixExprReturnSP->getSequence();
			for( LitMatrixExprReturn::Sequence::iterator lsqItr = sequence.begin() ; lsqItr != sequence.end() ; ++lsqItr ) {
				double item = *lsqItr;
				arrayDTBuilder.addArray( (int) item );
				dimensionVector.push_back( (int) item );
			}
		} else {
			while( aslCit != argReturnSPList.end() ) {
				LitMatrixExprReturnSP litMatrixExprReturnSP = boost::static_pointer_cast< LitMatrixExprReturn >( (*aslCit)->getExprReturnSP() );
				double item = litMatrixExprReturnSP->getSequence().front();
				arrayDTBuilder.addArray( (int) item );
				dimensionVector.push_back( (int) item );
				++aslCit;
			}
		}

		dt = arrayDTBuilder.getDT();

		if ( exprReturnSP->getClassName() == "LitMatrixExprReturn" ) {
			LitMatrixExprReturnSP litMatrixExprReturnSP = boost::static_pointer_cast< LitMatrixExprReturn >( exprReturnSP );
			return LitMatrixExprReturn::create(  block, dt, reshape( litMatrixExprReturnSP->getSequence(), sourceDimensionVector, dimensionVector )  );
		}

		VarMatrixExprReturnSP varMatrixExprReturnSP = exprReturnSP->convertToVarMatrixExprReturn();

		std::cout << varMatrixExprReturnSP->getClassName() << std::endl;
		VarMatrixExprReturn::Sequence vSequence = varMatrixExprReturnSP->getSequence();
		return VarMatrixExprReturn::create(  block, dt, reshape( varMatrixExprReturnSP->getSequence(), sourceDimensionVector, dimensionVector )  );
	}

	if ( id == "size" ) {
		ArgReturnSPList::const_iterator aslCit = argReturnSPList.begin();
		ExprReturnSP exprReturnSP = (*aslCit)->getExprReturnSP();
		SFCTypesManager::DimensionVector dimensionVector = exprReturnSP->getAllDimensions();

		LitMatrixExprReturn::Sequence sequence;

		SFC::BasicType intType = SFCTypesManager::getSingleton().getBasicType( "int" );

		++aslCit;
		if ( aslCit != argReturnSPList.end() ) {
			LitMatrixExprReturnSP litMatrixExprReturnSP = boost::static_pointer_cast< LitMatrixExprReturn >( argReturnSPList.back()->getExprReturnSP() );
			int dim = int(litMatrixExprReturnSP->getSequence().front()) - 1;
			sequence.push_back( dimensionVector[ dim ] );

			return LitMatrixExprReturn::create( block, 1, 1, intType, sequence );
		}

		sequence.insert( sequence.end(), dimensionVector.begin(), dimensionVector.end() );
		return LitMatrixExprReturn::create( block, 1, (int) sequence.size(), intType, sequence );
	}

	if ( id == "struct" ) {
		assert( argReturnSPList.size() % 2 == 0 );

		static int uniqueId = 0;
		std::string localVarName = std::string( "structvar" ) + boost::lexical_cast< std::string >( ++uniqueId );

		ExprReturnSP structExprReturnSP = ReferenceReturn::create( block, localVarName, PAntlr3BaseTreeList(), true, ExprReturnSP() );
		VarExprReturnSP structVarExprReturnSP = ExprReturn::static_pointer_cast< VarExprReturn >( structExprReturnSP );

		ArgReturnSPList::const_iterator aslCit = argReturnSPList.begin();
		while( aslCit != argReturnSPList.end() ) {
			ExprReturnSP memberNameExprReturnSP = (*aslCit)->getExprReturnSP();
			(void)++aslCit;
			assert( memberNameExprReturnSP->getClassName() == LitMatrixExprReturn::getName() );

			LitMatrixExprReturnSP litMatrixExprReturnSP = ExprReturn::static_pointer_cast< LitMatrixExprReturn >( memberNameExprReturnSP );
			std::string memberName;
			LitMatrixExprReturn::Sequence sequence = litMatrixExprReturnSP->getSequence();
			for( LitMatrixExprReturn::Sequence::iterator lesItr = sequence.begin() ; lesItr != sequence.end() ; ++lesItr ) {
				memberName.push_back(  static_cast< char > ( *lesItr )  );
			}

			ExprReturnSP memberExprReturnSP = ReferenceReturn::create( block, memberName, PAntlr3BaseTreeList(), true, structVarExprReturnSP );
			ReferenceReturnSP memberReferenceReturnSP = ExprReturn::static_pointer_cast< ReferenceReturn >( memberExprReturnSP );

			ReferenceReturnSP referenceReturnSP = structVarExprReturnSP->dotCombine( memberReferenceReturnSP );
			ExprReturnSP exprReturnSP1 = ExprReturn::static_pointer_cast< ExprReturn >( referenceReturnSP );

			ExprReturnSP exprReturnSP2 = (*aslCit)->getExprReturnSP();
			(void)++aslCit;
			referenceReturnSP->acquireType( exprReturnSP2->getDT() );
			exprReturnSP1->combine( "=", exprReturnSP2 )->collapse();
		}
		return ReferenceReturn::create( block, localVarName, PAntlr3BaseTreeList(), false );
	}

	if ( id == "transpose" ) {
		return argReturnSPList.front()->getExprReturnSP()->transpose();
	}

	if (
	 id == "uint8"  || id == "int8"  || id == "byte"   || id == "char" ||
	 id == "uint16" || id == "int16" || id == "short"  ||
	 id == "uint32" || id == "int32" || id == "int"    || id == "integer" ||
	 id == "uint64" || id == "int64" || id == "long"   ||
	 id == "double" || id == "float" || id == "single" 
	) {
		ExprReturnSP exprReturnSP = argReturnSPList.front()->getExprReturnSP();
		SFC::DT newBaseType = SFCTypesManager::getSingleton().getBasicType( id );
		SFC::DT newArrayType = SFCTypesManager::getSingleton().getNewTypeArray( exprReturnSP->getDT(), newBaseType );
		exprReturnSP->setDT( newArrayType );
		return exprReturnSP;
	}

	if ( id == "zeros" ) {
		return initArray( block, argReturnSPList, 0 );
	}

	if (  MFileManager::get_singleton().hasMFileFunction( id )  ) {

		MFileManager::DTVector inputDTVector;
		for( ArgReturnSPList::const_iterator aslCit = argReturnSPList.begin() ; aslCit != argReturnSPList.end() ; ++aslCit ) {
			inputDTVector.push_back( (*aslCit)->getExprReturnSP()->getDT() );
		}
		SFC::Function sfcFunction = MFileManager::get_singleton().getMFileFunction( id, inputDTVector );

		SFC::ArgSet argSet = sfcFunction.Arg_children_sorted( SFC::ArgSorter() );

		assert( argSet.size() == inputDTVector.size() + 1 );

		SFC::LocalVar localVar = SFCManager::createUniqueLocalVar( block, "temp" );
		localVar.dt() = argSet.rbegin()->dt();

		FE fe( sfcFunction );
		for( ArgReturnSPList::const_iterator aslCit = argReturnSPList.begin() ; aslCit != argReturnSPList.end() ; ++aslCit ) {
			VarExprReturnSP varExprReturnSP = (*aslCit)->getExprReturnSP()->convertToVarExprReturn();
			fe.addArgVal( varExprReturnSP->getExprs() );
		}
		fe.addArgVal(  VE( localVar )  );
		fe.buildUdm( block, SFC::CompoundStatement::meta_stmnt );

		return VarExprReturn::create( block, localVar, localVar.dt() );

	}

	ExprsProxyVector exprsProxyVector;
	SFC::DT dt;

	FE fe( id );
	for( ArgReturnSPList::const_iterator aslCit = argReturnSPList.begin() ; aslCit != argReturnSPList.end() ; ++aslCit ) {
		VarExprReturnSP varExprReturnSP = (*aslCit)->getExprReturnSP()->convertToVarExprReturn();
		fe.addArgVal( varExprReturnSP->getExprs() );
		exprsProxyVector = unifyExprsProxyVectors( exprsProxyVector, varExprReturnSP->getExprsProxyVector() );
		dt = SFCTypesManager::getSingleton().combineDT( dt, varExprReturnSP->getDT() );
	}

	Exprs exprs = fe;
	return VarExprReturn::create( block, exprs, exprsProxyVector, dt );
}

ExprReturnSP ReferenceReturn::initArray( SFC::CompoundStatement block, const ArgReturnSPList &argReturnSPList, double val ) {

	LitMatrixExprReturnSPList litMatrixExprReturnSPList;

	for( ArgReturnSPList::const_iterator aslCit = argReturnSPList.begin() ; aslCit != argReturnSPList.end() ; (void)++aslCit ) {
		ExprReturnSP exprReturnSP = (*aslCit)->getExprReturnSP();
		assert( exprReturnSP->getClassName() == LitMatrixExprReturn::getName() );
		LitMatrixExprReturnSP litMatrixExprReturnSP = ExprReturn::static_pointer_cast< LitMatrixExprReturn >( exprReturnSP );
		litMatrixExprReturnSPList.push_back( litMatrixExprReturnSP );
	}
	
	typedef LitMatrixExprReturn::Sequence Sequence;

	DimensionVector dimensionVector;
	LitMatrixExprReturnSP firstLitMatrixExprReturnSP = litMatrixExprReturnSPList.front(); litMatrixExprReturnSPList.pop_front();

	// CHECK IF THE FIRST LitMatrixExprReturn HAS A SEQUENCE OF LENGTH > 1, INDICATING A VECTOR
	// IF THIS IS THE CASE, ALL DIMENSION SPECIFICATIONS ARE IN THIS SEQUENCE
	Sequence &firstSequence = firstLitMatrixExprReturnSP->getSequence();
	if ( firstSequence.size() > 1 ) {
		for( Sequence::iterator sqnItr = firstSequence.begin() ; sqnItr != firstSequence.end() ; ++sqnItr ) {
			dimensionVector.push_back( (int) *sqnItr );
		}
	// OTHERWISE, THE DIMENSIONS ARE IN MULTIPLE SINGLETON LitMatrixExprReturn SEQUENCES
	} else {
		dimensionVector.push_back( (int) *firstSequence.begin() );
		while( !litMatrixExprReturnSPList.empty() ) {
			LitMatrixExprReturnSP litMatrixExprReturnSP = litMatrixExprReturnSPList.front();
			if ( litMatrixExprReturnSP->getIsText() ) break;
			dimensionVector.push_back( (int) *litMatrixExprReturnSP->begin() );
			litMatrixExprReturnSPList.pop_front();
		}
	}

	// IF litMatrixExprReturnSPList IS NOT EMPTY AT THIS POINT, ITS LAST MEMBER SHOULD BE A TYPE SPECIFICATION
	std::string typeName = "double";
	if ( !litMatrixExprReturnSPList.empty() ) {
		typeName = "";
		Sequence &sequence = litMatrixExprReturnSPList.back()->getSequence();
		for( Sequence::iterator sqnItr = sequence.begin() ; sqnItr != sequence.end() ; ++sqnItr ) {
			typeName += int( *sqnItr );
		}
	}

	ArrayDTBuilder arrayDTBuilder(  SFCTypesManager::getSingleton().getBasicType( typeName )  );

	for( DimensionVector::iterator dmvItr = dimensionVector.begin() ; dmvItr != dimensionVector.end() ; ++dmvItr ) {
		arrayDTBuilder.addArray( *dmvItr );
	}
	if ( dimensionVector.size() == 1 ) arrayDTBuilder.addArray( dimensionVector.front() );

	Exprs exprs = DE( val );
	return VarExprReturn::create(  block, exprs, ExprsProxyVector(), arrayDTBuilder.getDT()  );
}

ExprReturnSP ReferenceReturn::combine( const std::string &op, MatrixExprReturn &matrixExprReturn, bool func ) {
	acquireType( matrixExprReturn.getDT() );
	return VarExprReturn::combine( op, matrixExprReturn, func );
}

