#include <fstream>
#include <sstream>
#include <cstring>

#include "mfile2SFC.hpp"
#include "mfileScope.hpp"
#include "SFCUdmEngine.hpp"

#include <boost/lexical_cast.hpp>

DTList getDTList( ifstream &inputFile );

class UniqueVarNameFunctor : public ScopeSymbolTable::UniqueVarNameFunctor {
public:
	typedef ScopeSymbolTable::UniqueVarNameFunctor Super;
	typedef boost::shared_ptr< Super > SP;

private:
	int _uniqueNo;

public:
	UniqueVarNameFunctor( void ) : _uniqueNo( 0 ) { }

	virtual std::string operator()( const std::string &varName ) {
		return varName + boost::lexical_cast< std::string >( _uniqueNo++ );
	}
	virtual SP clone( void ) const { return SP(  new UniqueVarNameFunctor( *this )  ); }
};

int main( int argc, char *argv[] ) {

	if ( argc != 2 ) {
		std::cerr << "Please specify file name." << std::endl;
		return 1;
	}

	std::string inputFilename( argv[1] );
	ifstream inputFile( inputFilename.c_str() );
	if ( !inputFile.good() ) {
		std::cerr << "Error opening file \"" << argv[1] << "\".";
		return 2;
	}

//	std::string sfcFilename = inputFilename.substr(  0 , inputFilename.rfind( "." )  ) + "_SFC.xml";
//
//	SFCUdmEngine::get_singleton().createNewDataNetwork( sfcFilename );
//	SFC::Project project = SFCUdmEngine::get_singleton().getRoot();
//	SFCTypesManager::initSingleton( project );
//
//	SFC::Program program = SFC::Program::Create( project );
//	SFCManager::numberStatement( program );
//	SFC::Class sfcClass = SFC::Class::Create( program );
//	SFCManager::numberStatement( sfcClass );
//	SFC::Struct sfcStruct = SFC::Struct::Create( sfcClass );
//	SFCManager::numberStatement( sfcStruct );
//
//	DTList dtList = getDTList( inputFile );

	std::string matLabFunction;
	char line[ 1024 ];
	while(  inputFile.getline( line, 1024 )  ) {
		matLabFunction += std::string( line ) + "\n";
	}
	std::cout << matLabFunction << std::endl << std::endl;
	inputFile.close();

	ScopeSymbolTable scopeSymbolTable;

	scopeSymbolTable.setUniqueVarNameFunctor( UniqueVarNameFunctor() );
	scopeSymbolTable.addSymbol( "x" );

//	mfile2SFC( sfcClass, dtList, matLabFunction );
	MFileScope mfileScope;
	std::string outputString = mfileScope.scopeTrans( matLabFunction, scopeSymbolTable );

	std::cout << outputString;

	return 0;
}

DTList getDTList( ifstream &inputFile ) {
	
	DTList dtList;
	char line[1024];

	while(  inputFile.getline( line, 1024 )  ) {
		if (  strlen( line ) <= 2 ) break;

		istringstream typeLine( line );

		std::string type;
		typeLine >> type;

		SFC::BasicType basicType = SFCTypesManager::getSingleton().getBasicType( type );
		ArrayDTBuilder arrayDTBuilder( basicType );
		int dim;
		while( typeLine >> dim ) arrayDTBuilder.addArray( dim );
		dtList.push_back( arrayDTBuilder.getDT() );
	}

	return dtList;
}
