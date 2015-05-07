
#include "JohnsonsAlgorithm.h"

#include <iostream>

using namespace std;
using namespace boost;

int main( int argc, char * argv )
{
	CompGraph g1(5); // First test (w/ single cycle)

	add_edge(0,1,g1);
	add_edge(1,2,g1);
	add_edge(2,3,g1);
	add_edge(3,4,g1);
	add_edge(4,0,g1);

	JohnsonsAlgorithm ja1( g1 );
	cout << "Graph 1 num cycles: " << ja1.getNumCycles() << endl;
	ja1.print();

	CompGraph g2(8); // Second test (w/ multiple cycles)

	add_edge(0,1,g2);
	add_edge(1,2,g2);
	add_edge(2,3,g2);
	add_edge(3,4,g2);
	add_edge(4,0,g2);
	add_edge(1,5,g2);
	add_edge(5,6,g2);
	add_edge(6,7,g2);
	add_edge(6,0,g2);

	JohnsonsAlgorithm ja2( g2 );
	cout << "Graph 2 num cycles: " << ja2.getNumCycles() << endl;
	ja2.print();

	// Third test (small double cycles)
	CompGraph g3(4); 

	add_edge(0,1,g3);
	add_edge(1,2,g3);
	add_edge(2,0,g3);
	add_edge(1,3,g3);
	add_edge(3,0,g3);
	
	JohnsonsAlgorithm ja3( g3 );
	cout << "Graph 3 num cycles: " << ja3.getNumCycles() << endl;
	ja3.print();

	// Fourth test (complex cycles)
	CompGraph g4(9); 

	add_edge(0,1,g4);
	add_edge(1,2,g4);
	add_edge(2,3,g4);
	add_edge(3,4,g4);
	add_edge(4,5,g4);
	add_edge(5,6,g4);
	add_edge(4,2,g4);
	add_edge(5,1,g4);
	add_edge(5,0,g4);
	add_edge(0,7,g4);
	add_edge(7,8,g4);
	add_edge(8,0,g4);

	JohnsonsAlgorithm ja4( g4 );
	cout << "Graph 4 num cycles: " << ja4.getNumCycles() << endl;
	ja4.print();

	// Fifth test (no cycles)
	CompGraph g5(9); 

	add_edge(0,1,g5);
	add_edge(1,2,g5);
	add_edge(3,2,g5);
	add_edge(3,4,g5);
	add_edge(4,5,g5);
	add_edge(5,6,g5);
	add_edge(4,2,g5);
	add_edge(5,1,g5);
	add_edge(5,0,g5);
	add_edge(0,7,g5);
	add_edge(8,7,g5);
	add_edge(8,0,g5);

	JohnsonsAlgorithm ja5( g5 );
	cout << "Graph 5 num cycles: " << ja5.getNumCycles() << endl;
	ja5.print();

	return 0;
}