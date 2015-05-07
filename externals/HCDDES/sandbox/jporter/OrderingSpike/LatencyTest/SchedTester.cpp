// SchedTester Main 
//

#include "ConfigKeeper.h"
#include "NetGraph.h"

#include <iostream>

using namespace std;

int main(int argc, char **argv)
{
	ConfigKeeper * cfg = ConfigKeeper::inst();
	if (!cfg->processCmdLine(argc, argv))
	{
		return 1; // exit early
	}

	cout << cfg->getNodes() << " " << cfg->getNetworks() << " ";
	cout << cfg->getSysPeriodSecs() << " " << cfg->getOutfileName() << endl;

	NetGraph ng;
	ng.print();

	return 0;
}