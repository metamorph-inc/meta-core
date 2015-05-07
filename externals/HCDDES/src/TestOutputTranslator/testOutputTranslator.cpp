#include <boost/regex.hpp>
#include <string>
#include <iostream>
#include <fstream>

void usage( std::string progname );

int main( int argc, char *argv[] ) {

	if ( argc != 2 ) {
		usage( argv[0] );
		return 1;
	}

	std::string fileName = argv[1];
	std::string fileContents;

	{
		std::ifstream file( fileName.c_str() );
		fileContents = std::string(  std::istreambuf_iterator< char >( file ), std::istreambuf_iterator< char >()  );
		file.close();
	}

	boost::regex infinityRegex( "Infinity", boost::regex_constants::perl );
	fileContents = boost::regex_replace(  fileContents, infinityRegex, "Inf"  );

	{
		std::ofstream file( fileName.c_str() );
		file.write( fileContents.c_str(), fileContents.size() );
		file.close();
	}
	
	return 0;
}

void usage( std::string progname ) {
	std::cerr << "Usage:" << std::endl;
	std::cerr << progname << " <fileName>" << std::endl;
	std::cerr << "\"" << progname << "\" converts the output of a Java program into output appropriate" << std::endl;
	std::cerr << "for MatLab.  In particular, it translates the word \"Infinity\" to \"Inf\"" << std::endl;
}