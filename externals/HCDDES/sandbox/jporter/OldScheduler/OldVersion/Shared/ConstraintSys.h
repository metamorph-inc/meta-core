
// Actual constraint instance

#ifndef _CONSTRAINT_SYS_H_
#define _CONSTRAINT_SYS_H_

#include "gecode/kernel.hh"
#include "gecode/int.hh"
#include "gecode/search.hh"
#include "gecode/minimodel.hh"

#include "CSched.h"

#include "stdafx.h"
#include <sstream>

using namespace Gecode;
using namespace CSched;

// FailTimeStop class graciously provided by Christian Schulte and friends
// from the Gecode examples
namespace {
  /// Stop object for controlling stoppage based on both time and
  /// failures.
  class FailTimeStop : public Search::Stop {
  private:
    Search::TimeStop *ts;
    Search::FailStop *fs;
    FailTimeStop(int fails, int time) {
      ts = new Search::TimeStop(time);
      fs = new Search::FailStop(fails);
    }
  public:
    bool stop(const Search::Statistics& s) {
      return fs->stop(s) || ts->stop(s);
    }
    /// Create appropriate stop-object
    static Search::Stop* create(int fails, int time) {
      if (fails < 0 && time < 0) return NULL;
      if (fails < 0) return new Search::TimeStop( time);
      if (time  < 0) return new Search::FailStop(fails);
      return new FailTimeStop(fails, time);
    }
  };
}

class ConstraintSys : public Space
{
protected:
	IntVarArray vals;

	void CreateEqualityConstraints( const ConstraintProblem & cp, map< string, int > & vmap );
	void CreateInequalityConstraints( const CSched::ConstraintProblem & cp, map< string, int > & vmap );
	void CreateRangeConstraints( const CSched::ConstraintProblem & cp, map< string, int > & vmap );
	void CreateSerialConstraints( const CSched::ConstraintProblem & cp, map< string, int > & vmap );

public:
	ConstraintSys( int size, int lbound, int ubound, const ConstraintProblem & cp, map< string, int > & vmap ) :
		vals( this, size, lbound, ubound )
	{
		// Go over each type of constraint
		CreateEqualityConstraints( cp, vmap );
		CreateInequalityConstraints( cp, vmap );
		CreateRangeConstraints( cp, vmap );
		CreateSerialConstraints( cp, vmap );

		branch(this, vals, BVAR_SIZE_MIN, BVAL_MIN);
	}

	ConstraintSys( bool share, ConstraintSys & cs ) :
		Space( share, cs )
	{
		vals.update( this, share, cs.vals );
	}

	virtual Space * copy( bool share )
	{
		return new ConstraintSys( share, *this );
	}
	
	// a = b + offset
	void AddEqConstraint( const vector<int> & aidx, const vector<int> & bidx, const int offset )
	{
		IntArgs coeff(vals.size()); // initialize to zero

		/*ostringstream dbg;
		dbg << "Sizes vals:" << vals.size() << " aidx:" << aidx.size() << " bidx:" << bidx.size() << endl;*/
		unsigned int idx;
		for ( idx = 0; idx < vals.size(); idx++ )
		{
			coeff[idx] = 0;
		}
		for ( idx = 0; idx < aidx.size(); idx++ ) // All of the A's go on the left-hand side
		{
			/*dbg << "Set coeff " << aidx[idx] << " to 1" << endl;*/
			coeff[aidx[idx]] = 1;
		}
		for ( idx = 0; idx < bidx.size(); idx++ ) // All the B's on the right-hand side
		{
			/*dbg << "Set coeff " << bidx[idx] << " to -1" << endl;*/
			coeff[bidx[idx]] = -1;
		} 
		/*dbg << "Created constraint" << endl;
		AfxMessageBox( dbg.str().c_str() );*/
		linear( this, coeff, vals, IRT_EQ, offset );
	}

	// a <= b + offset
	void AddIneqConstraint( const vector<int> & aidx, const vector<int> & bidx, const int offset )
	{
		IntArgs coeff(vals.size()); // initialize to zero

		unsigned int idx;
		for ( idx = 0; idx < vals.size(); idx++ )
		{
			coeff[idx] = 0;
		}
		for ( idx = 0; idx < aidx.size(); idx++ ) // All the A's on the left-hand side
		{
			coeff[aidx[idx]] = 1;
		}
		for ( idx = 0; idx < bidx.size(); idx++ ) // All the B's on the right-hand side
		{
			coeff[bidx[idx]] = -1;
		}
		linear( this, coeff, vals, IRT_LQ, offset );		
	}

	// lower <= x <= upper
	void AddRangeConstraint( int idx, int lower, int upper )
	{
		rel( this, vals[idx], IRT_GQ, lower );
		rel( this, vals[idx], IRT_LQ, upper );
	}

	// enforce serial ordering -- pairs are (var idx, duration)
	void AddSerialConstraint( vector< pair< int, int > > idxs )
	{
		IntVarArray vidx( this, idxs.size() );
		IntArgs durations( idxs.size() );
		vector< pair< int, int > >::iterator pIdx;
		int count = 0;
		for ( pIdx = idxs.begin(); pIdx != idxs.end(); pIdx++ )
		{
			vidx[count] = vals[pIdx->first];
			durations[count++] = pIdx->second;
		}
		serialized( this, vidx, durations );
	}

	// Return a big, honkin array of indices and values
	void GetResults( vector< int > & results )
	{
		unsigned int idx;
		for ( idx = 0; idx < vals.size(); idx++ )
		{
			results.push_back( vals[idx].val() );
		}
	}
};

#endif // _CONSTRAINT_SYS_H_