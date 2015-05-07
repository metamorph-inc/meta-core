
#include "cpSolver.h"

#include <iostream>

#define DBG_CONSTRAINTS 0

using namespace std;

// CP Solver management
CPSolver::CPSolver(bool debug) : _instances(), _debug(debug)
{
}

CPSolver::~CPSolver(void)
{
}

// High-level convenience functions
void CPSolver::SerializeInstances( std::vector< std::string > & instnames, std::vector< unsigned long long > & instsizes )
{
	cout << "Serializing: ";
	_instances.SerializeInstances( instnames, instsizes );
	unsigned int s = instnames.size();
	for ( unsigned int count = 0; count < s; count++ )
	{
		cout << instnames[count] << "(" << instsizes[count] << ") ";
	}
	cout << endl;
}

void CPSolver::OrderInstances( std::vector< std::string > & instnames, unsigned long long dist, unsigned long long dur, bool strict )
{
	cout << "Ordering(" << (strict ? "strict" : "nonstrict") << "," << dist << "): ";
	_instances.OrderInstances( instnames, dist, dur, strict );
	unsigned int s = instnames.size();
	for ( unsigned int count = 0; count < s; count++ )
	{
		cout << instnames[count] << " ";
	}
	cout << endl;
}

void CPSolver::AddLatencyBound( std::vector< std::string > & sndrnames, std::vector< std::string > & rcvrnames, unsigned long long maxdist )
{
	_instances.AddLatencyBound( sndrnames, rcvrnames, maxdist );
}

// Low-level interface
void CPSolver::AddVariable( std::string & instname )
{
	_instances.AddVar( instname );
	cout << "Added constraint variable for " << instname << endl;
}

// Invocation
bool CPSolver::Solve()
{
	bool bSuccess = false;
	_feasible = false;
	CPEngine * cpe;

	if (_debug)
	{
		cout << "************* DEBUG!!! **************************************" << endl;
		cpe = new CPEngine( & _instances, _min, _max, true );
	}
	else
	{
		cout << "************* NON-DEBUG!!! *********************************" << endl;
		cpe = new CPEngine( &_instances, _min, _max );
	}

	// code taken from Gecode examples
	unsigned int n_p = -1;
    unsigned int n_b = -1;
    if (cpe->status() != SS_FAILED) {
      n_p = cpe->propagators();
      n_b = cpe->branchings();
    }

	// Run the search engine like crazy!
	// @todo -- push these up to the command line
	// @todo -- maxsolutions should always be 1 for debug mode
	Search::Stop* stop = NULL;
	unsigned int maxsolutions = 1;
	unsigned int maxfailures = 4000000000;
	unsigned int maxruntimesec = 1000000; // 1000 sec ~ 17 min.
    stop = FailTimeStop::create( maxfailures, maxruntimesec * 1000 /* Convert to msec */ );

	
	//DFS< CPEngine > srch( cpe, Search::Config::c_d, Search::Config::a_d, stop ); 
	Search::Options opt;
	opt.a_d = 30;
	opt.c_d = 120;
	//opt.a_d = 0;
	//opt.c_d = 1000000;
	opt.stop = stop;
	LDS< CPEngine > srch( cpe, 1000, opt ); 
	//DFS< CPEngine > srch( cpe, opt ); 
	
    delete cpe;

	CPEngine * result = NULL;
	try
	{
		result = srch.next();
	}
	catch ( MemoryExhausted & m ) 
	{
		cout << "Ran out of memory!!! (" << m.what() << ")" << endl;
	}

	Search::Statistics stat = srch.statistics();
	ostringstream stat_str;
	stat_str << "Search Stats" << endl << endl;
	stat_str << "Initial" << endl
			 << "\tpropagators:   " << n_p << endl
			 << "\tbranchings:    " << n_b << endl
			 << endl
			 << "Summary" << endl
			 // << "\truntime:       " << t.stop() << endl
			 //<< "\tsolutions:     " << abs(static_cast<int>(o.solutions) - i) << endl
			 << "\tpropagations:  " << stat.propagate << endl
			 << "\tfailures:      " << stat.fail << endl
			 << "\tclones:        " << stat.clone << endl
			 << "\tcommits:       " << stat.commit << endl
			 << "\tpeak memory:   "
			 << static_cast<int>((stat.memory+1023) / 1024) << " KB";

	cout << "Stats: " << stat_str.str() << endl << endl;	

    if ( result == NULL )
    {
        cout << "No solutions found!" << endl;
        bSuccess = false; 
    }
    else 
    {
        time_t ct;
        time( &ct );
        tm * t = localtime(&ct);

        ostringstream tmstr;
        tmstr << t->tm_hour << t->tm_min << t->tm_sec;

        int scount = 0;
        while ( result && ( scount < maxsolutions ) )
        {
			_feasible = true;
            result->CollectResults(&_instances, _debug);

			if (!_debug)
			{
				ostringstream result_str;      
				result_str << "Solution" << ++scount << "_" << tmstr.str();
				result_str << endl;

				unsigned int idx;
				for ( idx = 0; idx < _instances.NumVars(); idx++ )
				{
					std::string instname;
					if ( _instances.FindName( idx, instname ) )
					{
						result_str << instname << ": " << _instances.GetResult( instname ) << endl;
					}
				}

				cout << result_str.str() << endl;
			}

            result = srch.next();
			scount++;
        }

		bSuccess = true;
    }

	return bSuccess; 
}

// Collecting results
void CPSolver::CollectInstances( std::map< std::string, unsigned long long > & inst_results )
{	
	unsigned int nv = _instances.NumVars();
	for ( unsigned int idx = 0; idx < nv; idx++ )
	{
		std::string instname;
		if ( _instances.FindName( idx, instname ) )
		{
			inst_results[ instname ] = (unsigned long long) _instances.GetResult( instname );
		}
	}
}

// Return feasibility
void CPSolver::CollectDebugInfo( std::vector< std::string > & dbg_results, bool & feasible, bool & empty )
{
	dbg_results = _instances.GetDebugResults();
	feasible = _feasible;
	cout << "dbg_results.size() " << dbg_results.size() << " # lin groups " << _instances.GetNumLinearGroups() << endl;
	empty = (dbg_results.size() == _instances.GetNumLinearGroups());
}

