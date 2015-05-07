#include "cpSolver.h"

// CP Solver Engine
CPDebugEngine::CPDebugEngine( InstanceKeeper * pIK, unsigned long long min, unsigned long long max ) : 
vars( this, pIK->NumVars(), (int) min, (int) max ),  // instantiate constraint vars
latencies( this, pIK->NumLatencies()+1, 0, (int) 2 * max ), lin_cstr(this, pIK->GetNumLinearRelations())
_pIK( pIK )
{
	unsigned long dbg_idx = 0;

	// All equality constraints
	LinearRelation lrel;
	bool loop = pIK->GetNextEqualityPair( lrel, true );
	while (loop)
	{
		IntVarArgs v(2);
		v[0] = vars[lrel.xvar];
		v[1] = vars[lrel.yvar];
		IntArgs a(2);
		a[0] = 1; // 'x' of x = y + o
		a[1] = -1; // 'y' of x = y + o
		pIK->SaveLinearConstraint(dbg_idx, InstanceKeeper::printEq(lrel));
		linear( this, a, v, IRT_EQ, lrel.offset, lin_cstr[dbg_idx++] );

		//cout << "Posted equality relation (x,y) = (" << lrel.xvar << "," << lrel.yvar << ") w/ offset " << lrel.offset << std::endl;
		loop = pIK->GetNextEqualityPair( lrel );
	}

	// All inequality constraints
	loop = pIK->GetNextInequalityPair( lrel, true );
	while (loop)
	{
		IntVarArgs v(2);
		v[0] = vars[lrel.xvar];
		v[1] = vars[lrel.yvar];
		IntArgs a(2);
		a[0] = 1; // 'x' of x <= y + o
		a[1] = -1; // 'y' of x <= y + o
		pIK->SaveLinearConstraint(dbg_idx, InstanceKeeper::printIneq(lrel));
		linear( this, a, v, IRT_LQ, lrel.offset, lin_cstr[dbg_idx++] );
		//cout << "Posted inequality relation (x,y) = (" << lrel.xvar << "," << lrel.yvar << ") w/ offset " << lrel.offset << std::endl;
		loop = pIK->GetNextInequalityPair( lrel );		
	}

	dbg_idx = 0;
	// All duration constraints
	DurativeVariable dv;
	loop = pIK->GetNextDuration( dv, true );
	while ( loop )
	{
		IntVarArgs v(1);
		v[0] = vars[dv.xvar];
		IntArgs a(1);
		a[0] = 1;
		pIK->SaveDurConstraint(dbg_idx++, InstanceKeeper::printDur(dv));
		linear( this, a, v, IRT_LQ, ((int)max) - dv.duration ); // don't run over the end
		//cout << "Posted duration relation (x) = (" << dv.xvar << ") w/ length " << dv.duration << std::endl;
		loop = pIK->GetNextDuration( dv );
	}

	dbg_idx = 0;
	// All serialization constraints
	int ser_count = 0;
	std::vector< DurativeVariable > vdv;
	loop = pIK->GetNextSerialization( vdv, true );
	while (loop) // && false)
	{
		IntVarArgs v( (int) vdv.size() );
		IntVarArgs e( (int) vdv.size() );
		IntArgs d( (int) vdv.size() );

		//cout << "Serializing (x,d): ";
		for (unsigned int idx = 0; idx < vdv.size(); idx++ )
		{
			v[idx] = vars[vdv[idx].xvar];
			d[idx] = (int) vdv[idx].duration;
			e[idx].init(this,min,max);
			//	cout << "(" << vdv[idx].xvar << "," << vdv[idx].duration << "), ";
		}
		//cout << endl;

		// serialized( this, v, d );  went away!
		IntArgs height(v.size()), m(v.size());
		for ( int idx = v.size(); idx--; ) { height[idx] = 1; m[idx] = 0 /*ser_count*/; }
		pIK->SaveSerialConstraint(dbg_idx++, InstanceKeeper::printSer(vdv));
		cumulatives(this, m, v, d, e, height, 1, true );
		loop = pIK->GetNextSerialization( vdv );
		ser_count++;
	}

	dbg_idx = 0;
	// Instantiate latency variables here
	std::vector< LatencyBound > vlb;
	loop = pIK->GetNextLatencyBound( vlb, true );
	while ( loop )
	{

		std::vector< LatencyBound >::iterator pLB = vlb.begin();
		// First set up the range for the latency variables
		rel( this, latencies[pLB->latvar], IRT_GQ, 1 );
		rel( this, latencies[pLB->latvar], IRT_LQ, pLB->maxdist );

		// Then define the relationship between the instances
		// and the latencies
		int k = 0;
		BoolVarArgs bs(vlb.size());
		for ( pLB = vlb.begin(); pLB != vlb.end(); pLB++ )
		{
			BoolVar b(this,0,1);
			IntVarArgs v(2);
			v[0] = vars[pLB->rcvrvar];
			v[1] = vars[pLB->sendervar];
			IntArgs a(2);
			a[0] = 1; // 'x' of x = y + o
			a[1] = -1; // 'y' of x = y + o
			linear( this, a, v, IRT_EQ, latencies[pLB->latvar], b );
			bs[k++] = b;
		}
		pIK->SaveLatConstraint(dbg_idx++, InstanceKeeper::printLat(vlb));
		linear( this, bs, IRT_GR, 0 ); // Force at least one of the constraints in this collection to be true

		loop = pIK->GetNextLatencyBound( vlb );
	}

	// Dummy latency in case there are no latencies in the spec!
	// We have to branch on something!
	rel( this, latencies[pIK->NumLatencies()], IRT_GQ, 1 );
	rel( this, latencies[pIK->NumLatencies()], IRT_LQ, 1 );

	// Do branching
	branch(this, vars, INT_VAR_REGRET_MAX_MAX, INT_VAL_MED);
	branch(this, latencies, INT_VAR_SIZE_MIN, INT_VAL_MAX ); // start with max latency and work down
}

void CPDebugEngine::constrain( CPDebugEngine * cpe )
{
	// Use InstanceKeeper here --
	//   iterate over all latency variables
	//   call back to instance keeper to get a configuration for 
	//     each of the latency variables and build the necessary
	//     disjunctive constraints
	//   branch on the median of everything in vars
	/*std::vector< LatencyBound > vlb;
	bool loop = _pIK->GetNextLatencyBound( vlb, true );
	while ( loop )
	{

	std::vector< LatencyBound >::iterator pLB = vlb.begin();

	// Define the relationship between the instances
	// and the latencies
	int k = 0;
	BoolVarArgs bs(vlb.size());
	for ( pLB = vlb.begin(); pLB != vlb.end(); pLB++ )
	{
	BoolVar b(this,0,1);
	IntVarArgs v(2);
	v[0] = vars[pLB->rcvrvar];
	v[1] = vars[pLB->sendervar];
	IntArgs a(2);
	a[0] = 1; // 'x' of x = y + o
	a[1] = -1; // 'y' of x = y + o
	linear( this, a, v, IRT_EQ, latencies[pLB->latvar], b );
	bs[k++] = b;
	}
	linear( this, bs, IRT_GR, 0 ); // Force only one of the constraints in this collection to be true

	loop = _pIK->GetNextLatencyBound( vlb );
	}*/



	/*std::vector< LatencyBound > vlb;
	bool loop = _pIK->GetNextLatencyBound( vlb, true );
	while ( loop )
	{
	BoolVar b;
	std::vector< LatencyBound >::iterator pLB;
	for ( pLB = vlb.begin(); pLB != vlb.end(); pLB++ )
	{
	IntVarArgs v(2);
	v[0] = vars[pLB->rcvrvar];
	v[1] = vars[pLB->sendervar];
	IntArgs a(2);
	a[0] = 1; // 'x' of x = y + o
	a[1] = -1; // 'y' of x = y + o
	linear( this, a, v, IRT_EQ, latencies[pLB->latvar], b );
	}

	loop = _pIK->GetNextLatencyBound( vlb );
	}*/
	//branch(this, vars, INT_VAR_NONE, INT_VAL_MED);
}

void CPDebugEngine::CollectResults( InstanceKeeper * pIK )
{
	// Go back and check -- 
	// The original should have been infeasible.
	// If this one is feasible, check the boolean control variables
	// to see which ones failed.
	unsigned int idx;
	for ( idx = 0; idx < lin_cstr.size(); idx++ )
	{
		if ( lin_cstr[idx].val() == 0 )
			pIK->SaveConstraintResult( pIK->getLinearConstraint(idx) );
	}
}

