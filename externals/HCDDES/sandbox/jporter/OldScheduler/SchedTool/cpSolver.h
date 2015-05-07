
#ifndef _CP_SOLVER_H_
#define _CP_SOLVER_H_

#include "solverAdapter.h"
#include "InstanceKeeper.h"

#include "gecode/kernel.hh"
#include "gecode/support.hh"
#include "gecode/minimodel.hh"
#include "gecode/search.hh"
#include "gecode/serialization.hh"

#include <iostream>

using namespace Gecode;



// FailTimeStop class graciously provided by Christian Schulte and friends
// from the Gecode examples
namespace {

  /**
   * \brief Stop object based on both time and failures
   */
  class FailTimeStop : public Search::Stop {
  private:
    Search::TimeStop *ts; ///< Used time stop object
    Search::FailStop *fs; ///< Used fail stop object
    /// Initialize stop object
    FailTimeStop(int fails, int time) {
      ts = new Search::TimeStop(time);
      fs = new Search::FailStop(fails);
    }
  public:
    /// Test whether search must be stopped
    virtual bool stop(const Search::Statistics& s) {
		bool bSuccess = fs->stop(s) || ts->stop(s);
		if ( bSuccess )
			std::cout << "Stopped because of FTS!!!" << std::endl;

      return bSuccess; //fs->stop(s) || ts->stop(s);
    }
    /// Create appropriate stop-object
    static Search::Stop* create(unsigned int fail, unsigned int time) {
      if ((fail == 0) && (time == 0)) 
        return NULL;
      if (fail == 0) 
        return new Search::TimeStop(time);
      if (time  == 0) 
        return new Search::FailStop(fail);
      return new FailTimeStop(fail,time);
    }
  };
}

class CPEngine : public Space
{
public:
	CPEngine( InstanceKeeper * pIK, unsigned long long min, unsigned long long max );
	CPEngine( InstanceKeeper * pIK, unsigned long long min, unsigned long long max, bool dbg );

	CPEngine( bool share, CPEngine & cpe ) :
		Space( share, cpe ), _pIK( cpe.GetIK() )
	{
		vars.update(this, share, cpe.vars);
		latencies.update(this, share, cpe.latencies);
		lin_cstr.update(this, share, cpe.lin_cstr);
		sat_cstr.update(this, share, cpe.sat_cstr);
		//latency_disj.update(this, share, cpe.latency_disj);
	}

	virtual Space * copy(bool share)
	{
		return new CPEngine( share, *this );
	}

	virtual void constrain( CPEngine * cpe );
	virtual ~CPEngine() { }

	void CollectResults( InstanceKeeper * pIK, bool debug=false );

	InstanceKeeper * GetIK() { return _pIK; }

protected:
	IntVarArray vars;
	IntVarArray latencies;

	BoolVarArray lin_cstr;
	IntVarArray sat_cstr;

	//BoolVarArray latency_disj;
	InstanceKeeper * _pIK;
};

// We can probably merge this back into the CPEngine if it works -- look carefully at 
//  the effect of the reified constraints.
/*
class CPDebugEngine : public Space
{
public:
	CPDebugEngine( InstanceKeeper * pIK, unsigned long long min, unsigned long long max );

	CPDebugEngine( bool share, CPDebugEngine & cpe ) :
	Space( share, cpe ), _pIK( cpe.GetIK() )
	{
		vars.update(this, share, cpe.vars);
		latencies.update(this, share, cpe.latencies);
		lin_cstr.update(this, share, cpe.lin_cstr);
		//latency_disj.update(this, share, cpe.latency_disj);
	}

	virtual Space * copy(bool share)
	{
		return new CPDebugEngine( share, *this );
	}

	virtual void constrain( CPDebugEngine * cpe );
	virtual ~CPDebugEngine() { }

	void CollectResults( InstanceKeeper * pIK );

	InstanceKeeper * GetIK() { return _pIK; }

protected:
	IntVarArray vars;
	IntVarArray latencies;

	BoolVarArray lin_cstr;
	//BoolVarArray dur_cstr;
	//BoolVarArray lat_cstr;

	//BoolVarArray latency_disj;
	InstanceKeeper * _pIK;
};
*/

class CPSolver :
	public SolverAdapter
{
public:
	CPSolver(bool debug=false);
	virtual ~CPSolver(void);

	// High-level convenience functions
	virtual void SerializeInstances( std::vector< std::string > & instnames, std::vector< unsigned long long > & instsizes );
	virtual void OrderInstances( std::vector< std::string > & instnames, unsigned long long dist, unsigned long long dur, bool strict = true );
	virtual void AddLatencyBound( std::vector< std::string > & sndrnames, std::vector< std::string > & rcvrnames, unsigned long long maxdist );

	// Low-level interface
	void SetDomain( unsigned long long min, unsigned long long max ) { _min = min; _max = max-1; _instances.SetHyperperiod(max); }
	virtual void AddVariable( std::string & instname );

	// Invocation
	virtual bool Solve();

	// Collecting results
	virtual void CollectInstances( std::map< std::string, unsigned long long > & inst_results );
	virtual void CollectDebugInfo( std::vector< std::string > & dbg_results, bool & feasible, bool & empty );

protected:
	unsigned long long _min;
	unsigned long long _max;
	bool _debug;
	bool _feasible;

	InstanceKeeper _instances;
};

#endif // _CP_SOLVER_H_
