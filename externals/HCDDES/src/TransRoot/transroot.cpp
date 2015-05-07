#include <boost/regex.hpp>
#include <string>
#include <iostream>
#include <fstream>

void usage( std::string progname );

int main( int argc, char *argv[] ) {

	if ( argc != 4 ) {
		usage( argv[0] );
		return 1;
	}

	std::string scanFileName = argv[1];

	std::string classNamePrefix = argv[2];
	std::string pattern = std::string( "class (" ) + classNamePrefix + "_[A-Fa-f0-9]*)";

	std::string outputFileName = argv[3];

	std::ifstream scanFile( scanFileName.c_str() );

	boost::regex classNamePattern( pattern, boost::regex_constants::perl );
	boost::match_results< std::string::const_iterator > results;
	std::string line;

	while( true ) {
		char cline[1024];
		scanFile.getline( cline, 1024 );
		if ( !scanFile.good() ) break;

		line = cline;
		std::string::const_iterator start = line.begin();
		std::string::const_iterator finish = line.end();

		if (  boost::regex_search( start, finish, results, classNamePattern )  ) break;
	}

	scanFile.close();

	if ( !scanFile.good() ) {
		std::cerr << "Pattern \"" << pattern << "\" not found." << std::endl;
		return 1;
	}

	std::string className( results[1].first, results[1].second );

	std::ofstream outputFile( outputFileName.c_str() );
	outputFile << "typedef " << className << " TransClass;" << std::endl;
	outputFile.close();
	
	return 0;
}

void usage( std::string progname ) {
	std::cerr << "Usage:" << std::endl;
	std::cerr << progname << " <inputFileName> <pattern> <outputFileName>" << std::endl;
	std::cerr << "\"" << progname << "\" searches <inputFileName> for the regular expression" << std::endl;
	std::cerr << "\"class (<pattern>_[A-Fa-f0-9]*)\".  If found, it writes" << std::endl;
	std::cerr << "\"typedef {MATCH} TransClass;\" to <outputFileName>." << std::endl;
	std::cerr << "The purpose of the program is to provide a constant name for the class" << std::endl;
	std::cerr << "that embodies the start of a GReAT transform in the C++ code generated from" << std::endl;
	std::cerr << "the transform.  That is, in the transform, this class has the name <pattern>" << std::endl;
	std::cerr << "and is represented by the uppermost \"Block\" or \"ForBlock\" in the transform." << std::endl;
	std::cerr << "In the C++ code generated from the transform, this class is given the unique" << std::endl;
	std::cerr << "name <pattern>_XXXX.  The problem \"" << progname << "\" solves stems from" << std::endl;
	std::cerr << "the fact that the value of XXXX can vary over time, in particular when the" << std::endl;
	std::cerr << "transform is modified and the C++ code (and unique name <pattern>_XXXX) is" << std::endl;
	std::cerr << "re-generated from the transform.  Any manually-written code that defines" << std::endl;
	std::cerr << "an object of class <pattern>_XXXX has to be modified to take the new value" << std::endl;
	std::cerr << "of XXXX into account (otherwise, a compiler error results).  With \"program\"," << std::endl;
	std::cerr << "such a manually-written program now simply has to include <outputFilename>" << std::endl;
	std::cerr << "and define an object of the \"TransClass\" class." << std::endl << std::endl;
}