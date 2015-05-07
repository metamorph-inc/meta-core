// Latency.cpp : Defines the entry point for the console application.
//
#include "ConfigKeeper.h"
#include "NetGraph.h"

#include <iostream>

using namespace std;

// Procedure:
//    Generate a random directed, acyclic, polar graph
//    Come up with a low-latency schedule
//    Iterate over the edges - for each edge:
//        What is the execution latency if we swap the execution order for the edge endpoints?
//        Print vertex, order number, in-degree, and out-degree for both vertices along with latency

int main(int argc, char * argv[])
{
	ConfigKeeper * cfg = ConfigKeeper::inst();
	if (!cfg->processCmdLine(argc, argv))
	{
		return 1; // exit early
	}

	cout << cfg->getNodes() << " " << cfg->getNetworks() << " ";
	cout << cfg->getSysPeriodSecs() << " " << cfg->getOutfileName() << endl;

	NetGraph ng;
	ng.print_graph();

	return 0;
}

