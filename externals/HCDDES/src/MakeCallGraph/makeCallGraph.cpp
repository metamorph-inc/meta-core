#include <string>
#include <iostream>

#include "CallGraph.hpp"

void usage( std::string progname );

int main( int argc, char *argv[] ) {
	if ( argc < 2 ) {
		usage( argv[0] );
		return 1;
	}

	std::string filename = argv[1];

	CallGraph callGraph( filename );
	int maxDepth = callGraph.dfsAnalyze( true );
	if (  maxDepth < 0  ) {
		std::cout << "CALL GRAPH HAS CYCLE(S)" << std::endl;
	} else {
		std::cout << "CALL GRAPH HAS NO CYCLES" << std::endl;
		std::cout << "DEPTH OF CALL GRAPH = " << maxDepth << std::endl;
	}

	if ( argc > 2 ) {
		std::string dottyFilename = argv[2];
		callGraph.print( dottyFilename );
	}

	return 0;
}

void usage( std::string progname ) {
	std::string::size_type lastSlashPos = progname.rfind( '/' );
	if ( lastSlashPos != std::string::npos ) progname = progname.substr( lastSlashPos + 1 );

	std::cerr << "Usage: " << progname << "<SFC-model-file> [dotty-file]" << std::endl;
}