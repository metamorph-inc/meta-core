
#include "ConfigKeeper.h"
#include "ExtractSubgraph.h"
#include "TestOrdering.h"

#include "front_end_tt/NWBuilder_tt.h" // TODO: Hide this in a factory
#include "NWModel.h"
#include "DiscNWModel.h"
#include "ExpNWGraph.h"

#include <iostream>
#include <utility>

using namespace std;

int main(int argc, char * argv[] )
{

	ConfigKeeper * cfg = ConfigKeeper::inst();
	if (!cfg->processCmdLine(argc, argv))
	{
		std::cout << std::endl;
		return 1; // exit early
	}

	NWBuilder_tt nwb( cfg->getInFileName() );
	//nwb.Print();

	ProblemPtr prob = nwb.getProblem();
	DiscProblemPtr discprob = boost::make_shared< DiscProblem >( prob );
	discprob->Setup();
	ExpProblemPtr expprob = boost::make_shared< ExpProblem >( discprob );
	expprob->Setup();
	
	prob->Print();
	discprob->Print();
	expprob->Print();
	
    return 0;
}

