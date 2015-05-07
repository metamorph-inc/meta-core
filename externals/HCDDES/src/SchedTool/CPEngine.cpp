#include "cpSolver.h"
#include <iostream>
#include "ConfigKeeper.h"
//#include "gecode/scheduling.hh"

using namespace std;




// CP Solver Engine
CPEngine::CPEngine( InstanceKeeper * pIK, unsigned long long min, unsigned long long max ):vars( *this, pIK->NumVars(  ), ( int ) min, ( int ) max + 1 ),       // instantiate constraint vars
latencies( *this, pIK->NumLatencies(  ) + 1, 0, ( int ) 2 * max ),
lin_cstr( *this, 1, 0, 1 ),
sat_cstr( *this, 1, 0, ( int ) pIK->GetNumLinearRelations(  ) ),
_pIK( pIK ) {

	pIK->Dump();

    // All equality constraints
    LinearRelation lrel;
    bool loop = pIK->GetNextEqualityPair( lrel, true );
    while( loop ) {
        IntVarArgs v( 2 );
        v[0] = vars[lrel.xvar];
        v[1] = vars[lrel.yvar];
        IntArgs a( 2 );
        a[0] = 1;               // 'x' of x = y + o
        a[1] = -1;              // 'y' of x = y + o
        linear( *this, a, v, IRT_EQ, lrel.offset );
        //cout << "Posted equality relation (x,y) = (" << lrel.xvar << "," << lrel.yvar << ") w/ offset " << lrel.offset << std::endl;
        loop = pIK->GetNextEqualityPair( lrel );
    }

    // All inequality constraints
    loop = pIK->GetNextInequalityPair( lrel, true );
    while( loop ) {
        IntVarArgs v( 2 );
        v[0] = vars[lrel.xvar];
        v[1] = vars[lrel.yvar];
        IntArgs a( 2 );
        a[0] = 1;               // 'x' of x <= y + o
        a[1] = -1;              // 'y' of x <= y + o
        linear( *this, a, v, IRT_LQ, lrel.offset );
        //cout << "Posted inequality relation (x,y) = (" << lrel.xvar << "," << lrel.yvar << ") w/ offset " << lrel.offset << std::endl;
        loop = pIK->GetNextInequalityPair( lrel );
    }

    // All duration constraints
    DurativeVariable dv;
    loop = pIK->GetNextDuration( dv, true );
    while( loop ) {
        IntVarArgs v( 1 );
        v[0] = vars[dv.xvar];
        IntArgs a( 1 );
        a[0] = 1;
        linear( *this, a, v, IRT_LQ, ( ( ( int ) max ) - dv.duration ) );   // don't run over the end
        //cout << "Posted duration relation (x) = (" << dv.xvar << ") w/ length " << dv.duration << " (max " << (((int)max) - dv.duration)+1 << ")" << std::endl;
        loop = pIK->GetNextDuration( dv );
    }

    // All serialization constraints
    int ser_count = 0;
    std::vector < DurativeVariable > vdv;
    loop = pIK->GetNextSerialization( vdv, true );
    while( loop ) {
        IntVarArgs v( ( int ) vdv.size(  ) );
        IntVarArgs e( ( int ) vdv.size(  ) );
        IntArgs d( ( int ) vdv.size(  ) );

        cout << "Serializing (x,d): ";
        for( unsigned int idx = 0; idx < vdv.size(  ); idx++ ) {
            v[idx] = vars[vdv[idx].xvar];
            d[idx] = ( int ) vdv[idx].duration;
            //e[idx].init( *this, min, max + 1 );
			e[idx] = IntVar( *this, min, max + 1 );
            cout << "(" << vdv[idx].xvar << "," << vdv[idx].duration << "), ";
        }
        cout << endl;

        IntArgs h( vdv.size(  ) ),
          m( vdv.size(  ) ),
          l( vdv.size(  ) );
        for( int idx = 0; idx < vdv.size(  ); idx++ ) {
            h[idx] = 1;
            m[idx] = 1;
            l[idx] = 1;
            //cout << m[idx] << " (" << vdv[idx].xvar << ") " << d[idx] << " (" << min << "," << max+1 << ") " << h[idx] << " " << l[idx] << " " << true << endl;
        }
        cumulatives( *this, m, v, d, e, h, l, true );
        loop = pIK->GetNextSerialization( vdv );
        ser_count++;
    }

    // All dependency constraints
    LinearRelation ldc;
    loop = pIK->GetNextLocalDependency( ldc, true );
    while( loop ) {
        IntVarArgs v( 2 );
        v[0] = vars[ldc.xvar];
        v[1] = vars[ldc.yvar];
        IntArgs a( 2 );
        a[0] = 1;               // 'x' of x <= y + o
        a[1] = -1;              // 'y' of x <= y + o
        linear( *this, a, v, IRT_LQ, ldc.offset );
        //cout << "Posted local dependency (x,y) = (" << lrel.xvar << "," << lrel.yvar << ") w/ offset " << lrel.offset << std::endl;
        loop = pIK->GetNextLocalDependency( ldc );
    }

    // All range constraints
    RangeVariable rv;
    loop = pIK->GetNextRange( rv, true );
    while( loop ) {

		if ( rv.minpos == rv.maxpos )
			dom( *this, vars[rv.xvar], rv.minpos ); // post the simpler equality constraint
		else
			dom( *this, vars[rv.xvar], rv.minpos, rv.maxpos );

		cout << "Domain x ( " << rv.xvar << " ) in range( " << rv.minpos << ", " << rv.maxpos << " )" << endl;
        loop = pIK->GetNextRange( rv );
    }

    // Instantiate latency variables here
    std::vector < LatencyBound > vlb;
    loop = pIK->GetNextLatencyBound( vlb, true );
    while( loop ) {

        std::vector < LatencyBound >::iterator pLB = vlb.begin(  );
        // First set up the range for the latency variables
        rel( *this, latencies[pLB->latvar], IRT_GQ, 1 );
        rel( *this, latencies[pLB->latvar], IRT_LQ, pLB->maxdist );

        //cout << "Latency (l) = (" << pLB->latvar << ") alternatives: ";

        // Then define the relationship between the instances
        // and the latencies
        int k = 0;
        BoolVarArgs bs( vlb.size(  ) );
        for( pLB = vlb.begin(  ); pLB != vlb.end(  ); pLB++ ) {
            BoolVar b( *this, 0, 1 );
            IntVarArgs v( 2 );
            v[0] = vars[pLB->rcvrvar];
            v[1] = vars[pLB->sendervar];
            IntArgs a( 2 );
            a[0] = 1;           // 'x' of x = y + o
            a[1] = -1;          // 'y' of x = y + o
            //cout << "(s,r) = (" << pLB->sendervar << "," << pLB->rcvrvar << ") ";
            linear( *this, a, v, IRT_EQ, latencies[pLB->latvar], b );
            bs[k++] = b;
        }
        //cout << endl;
        linear( *this, bs, IRT_GR, 0 ); // Force at least one of the constraints in this collection to be true

        loop = pIK->GetNextLatencyBound( vlb );
    }

    // Dummy latency in case there are no latencies in the spec!
    // We have to branch on something!
    rel( *this, latencies[pIK->NumLatencies(  )], IRT_GQ, 1 );
    rel( *this, latencies[pIK->NumLatencies(  )], IRT_LQ, 1 );
    //cout << "Dummy latency (==1)" << endl;

    // Do branching
    if( ConfigKeeper::inst(  )->getBranching(  ) == string( "med" ) )
        branch( *this, vars, INT_VAR_REGRET_MAX_MAX, INT_VAL_MED );
    else if( ConfigKeeper::inst(  )->getBranching(  ) == string( "min" ) )
        branch( *this, vars, INT_VAR_REGRET_MAX_MAX, INT_VAL_MIN );

    branch( *this, latencies, INT_VAR_SIZE_MIN, INT_VAL_MAX );  // start with max latency and work down
}

/*** DEBUG CONSTRUCTOR ***********************************************************************************/

CPEngine::CPEngine( InstanceKeeper * pIK, unsigned long long min, unsigned long long max, bool dbg ):vars( *this, pIK->NumVars(  ), ( int ) min, ( int ) max ), // instantiate constraint vars
latencies( *this, pIK->NumLatencies(  ) + 1, 0, ( int ) 2 * max ),
        //lin_cstr(*this, pIK->GetNumLinearRelations(),0,1),
lin_cstr( *this, pIK->GetNumLinearGroups(  ), 0, 1 ),
        //sat_cstr(*this, 1, (int) 0, (int) pIK->GetNumLinearRelations() ),
sat_cstr( *this, 1, ( int ) 0, ( int ) pIK->GetNumLinearGroups(  ) - 1 ),
_pIK( pIK ) {
    unsigned long dbg_idx = 0;

    cout << "Problem size estimates: " << endl;
    cout << " Vars: " << vars.size(  ) << endl;
    cout << " Latencies: " << latencies.size(  ) << endl;
    cout << " Linear control variables: " << lin_cstr.size(  ) << endl;
    cout << endl;

    // All equality constraints
    LinearRelation lrel;
    bool loop = pIK->GetNextEqualityPair( lrel, true );
    while( loop ) {
        IntVarArgs v( 2 );
        v[0] = vars[lrel.xvar];
        v[1] = vars[lrel.yvar];
        IntArgs a( 2 );
        a[0] = 1;               // 'x' of x = y + o
        a[1] = -1;              // 'y' of x = y + o
        //pIK->SaveLinearConstraint(dbg_idx, pIK->printEq(lrel));
        linear( *this, a, v, IRT_EQ, lrel.offset, lin_cstr[lrel.group],
                ICL_BND );
        dbg_idx++;

        //cout << "Posted equality relation (x,y) = (" << lrel.xvar << "," << lrel.yvar << ") w/ offset " << lrel.offset << " for group " << lrel.group << std::endl;
        loop = pIK->GetNextEqualityPair( lrel );
    }

    // All inequality constraints
    loop = pIK->GetNextInequalityPair( lrel, true );
    while( loop ) {
        IntVarArgs v( 2 );
        v[0] = vars[lrel.xvar];
        v[1] = vars[lrel.yvar];
        IntArgs a( 2 );
        a[0] = 1;               // 'x' of x <= y + o
        a[1] = -1;              // 'y' of x <= y + o
        //pIK->SaveLinearConstraint(dbg_idx, pIK->printIneq(lrel));
        linear( *this, a, v, IRT_LQ, lrel.offset, lin_cstr[lrel.group],
                ICL_BND );
        //cout << "Posted inequality relation (x,y) = (" << lrel.xvar << "," << lrel.yvar << ") w/ offset " << lrel.offset << " for group " << lrel.group << std::endl;
        loop = pIK->GetNextInequalityPair( lrel );
    }

    // Post some channel constraints to enforce max-sat
    //IntVarArray boolchan(*this, pIK->GetNumLinearRelations(), 0, 1);
    //IntVarArray summation(*this, pIK->GetNumLinearRelations()+1,0, pIK->GetNumLinearRelations());
    IntVarArray boolchan( *this, pIK->GetNumLinearGroups(  ), 0, 1 );
    IntVarArray summation( *this, pIK->GetNumLinearGroups(  ) + 1 );    // ,0, pIK->GetNumLinearGroups());
    IntArgs coeffs( pIK->GetNumLinearGroups(  ) + 1 );
    unsigned long cidx;
    //for (cidx = 0; cidx < pIK->GetNumLinearRelations(); cidx++)
    for( cidx = 0; cidx < pIK->GetNumLinearGroups(  ); cidx++ ) {
        channel( *this, boolchan[cidx], lin_cstr[cidx] );
        coeffs[cidx] = 1;
        //summation[cidx].init( *this, 0, 1 );
		summation[cidx] = IntVar( *this, 0, 1 );
        summation[cidx] = boolchan[cidx];
    }
    coeffs[cidx] = -1;
    //summation[cidx].init( *this, 0, pIK->GetNumLinearGroups(  ) );
	summation[cidx] = IntVar( *this, 0, pIK->GetNumLinearGroups(  ) );
    summation[cidx] = sat_cstr[0];
    linear( *this, coeffs, summation, IRT_EQ, 0 );      // Sum up the channels

    dbg_idx = 0;
    // All duration constraints
    DurativeVariable dv;
    loop = pIK->GetNextDuration( dv, true );
    while( loop ) {
        IntVarArgs v( 1 );
        v[0] = vars[dv.xvar];
        IntArgs a( 1 );
        a[0] = 1;
        //pIK->SaveDurConstraint(dbg_idx++, pIK->printDur(dv));
        linear( *this, a, v, IRT_LQ, ( ( int ) max ) - dv.duration );   // don't run over the end
        ////cout << "Posted duration relation (x) = (" << dv.xvar << ") w/ length " << dv.duration << std::endl;
        loop = pIK->GetNextDuration( dv );
    }

    dbg_idx = 0;
    // All serialization constraints
    int ser_count = 0;
    std::vector < DurativeVariable > vdv;
    loop = pIK->GetNextSerialization( vdv, true );
    while( loop )               // && false)
    {
        IntVarArgs v( ( int ) vdv.size(  ) );
        IntVarArgs e( ( int ) vdv.size(  ) );
        IntArgs d( ( int ) vdv.size(  ) );

        //cout << "Serializing (x,d): ";
        for( unsigned int idx = 0; idx < vdv.size(  ); idx++ ) {
            v[idx] = vars[vdv[idx].xvar];
            d[idx] = ( int ) vdv[idx].duration;
            //e[idx].init( *this, min, max );
			e[idx] = IntVar( *this, min, max );
            //cout << "(" << vdv[idx].xvar << "," << vdv[idx].duration << "), ";
        }
        //cout << endl;

        IntArgs h( vdv.size(  ) ),
          m( vdv.size(  ) ),
          l( vdv.size(  ) );
        for( int idx = 0; idx < vdv.size(  ); idx++ ) {
            h[idx] = 1;
            m[idx] = 1;
            l[idx] = 1;
            //cout << m[idx] << " (" << vdv[idx].xvar << ") " << d[idx] << " (" << min << "," << max << ") " << h[idx] << " " << l[idx] << " " << true << endl;
        }
        cumulatives( *this, m, v, d, e, h, l, true );
        //pIK->SaveSerialConstraint(dbg_idx++, pIK->printSer(vdv));
        loop = pIK->GetNextSerialization( vdv );
        ser_count++;
    }

    // All dependency constraints
    LinearRelation ldc;
    loop = pIK->GetNextLocalDependency( ldc, true );
    while( loop ) {
        IntVarArgs v( 2 );
        v[0] = vars[ldc.xvar];
        v[1] = vars[ldc.yvar];
        IntArgs a( 2 );
        a[0] = 1;               // 'x' of x <= y + o
        a[1] = -1;              // 'y' of x <= y + o
        linear( *this, a, v, IRT_LQ, ldc.offset, lin_cstr[ldc.group],
                ICL_BND );
        //cout << "Posted local dependency (x,y) = (" << lrel.xvar << "," << lrel.yvar << ") w/ offset " << lrel.offset << std::endl;
        loop = pIK->GetNextLocalDependency( ldc );
    }

    dbg_idx = 0;
    // Instantiate latency variables here
    std::vector < LatencyBound > vlb;
    loop = pIK->GetNextLatencyBound( vlb, true );
    while( loop ) {

        std::vector < LatencyBound >::iterator pLB = vlb.begin(  );
        // First set up the range for the latency variables
        rel( *this, latencies[pLB->latvar], IRT_GQ, 1 );
        rel( *this, latencies[pLB->latvar], IRT_LQ, pLB->maxdist );

        // Then define the relationship between the instances
        // and the latencies
        int k = 0;
        BoolVarArgs bs( vlb.size(  ) );
        for( pLB = vlb.begin(  ); pLB != vlb.end(  ); pLB++ ) {
            BoolVar b( *this, 0, 1 );
            IntVarArgs v( 2 );
            v[0] = vars[pLB->rcvrvar];
            v[1] = vars[pLB->sendervar];
            IntArgs a( 2 );
            a[0] = 1;           // 'x' of x = y + o
            a[1] = -1;          // 'y' of x = y + o
            linear( *this, a, v, IRT_EQ, latencies[pLB->latvar], b );
            bs[k++] = b;
        }
        //pIK->SaveLatConstraint(dbg_idx++, pIK->printLat(vlb));
        linear( *this, bs, IRT_GR, 0 ); // Force at least one of the constraints in this collection to be true

        loop = pIK->GetNextLatencyBound( vlb );
    }

    // Dummy latency in case there are no latencies in the spec!
    // We have to branch on something!
    rel( *this, latencies[pIK->NumLatencies(  )], IRT_GQ, 1 );
    rel( *this, latencies[pIK->NumLatencies(  )], IRT_LQ, 1 );

    // Do branching
    branch( *this, sat_cstr, INT_VAR_NONE, INT_VAL_MAX );
    branch( *this, lin_cstr, INT_VAR_NONE, INT_VAL_MIN );
    branch( *this, vars, INT_VAR_REGRET_MAX_MAX, INT_VAL_MED );
    branch( *this, latencies, INT_VAR_SIZE_MIN, INT_VAL_SPLIT_MAX );    // start with max latency and work down
}

void CPEngine::constrain( CPEngine & cpe ) {
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
       BoolVar b(*this,0,1);
       IntVarArgs v(2);
       v[0] = vars[pLB->rcvrvar];
       v[1] = vars[pLB->sendervar];
       IntArgs a(2);
       a[0] = 1; // 'x' of x = y + o
       a[1] = -1; // 'y' of x = y + o
       linear( *this, a, v, IRT_EQ, latencies[pLB->latvar], b );
       bs[k++] = b;
       }
       linear( *this, bs, IRT_GR, 0 ); // Force only one of the constraints in this collection to be true

       loop = _pIK->GetNextLatencyBound( vlb );
       } */

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
       linear( *this, a, v, IRT_EQ, latencies[pLB->latvar], b );
       }

       loop = _pIK->GetNextLatencyBound( vlb );
       } */
    //branch(*this, vars, INT_VAR_NONE, INT_VAL_MED);
}

void CPEngine::CollectResults( InstanceKeeper * pIK, bool debug ) {
    unsigned int idx;
    if( !debug ) {
        for( idx = 0; idx < vars.size(  ); idx++ ) {
            pIK->SetResult( idx, vars[idx].val(  ) );
        }
    }
    else {
        for( idx = 0; idx < lin_cstr.size(  ); idx++ ) {
            cout << "Linear group " << idx << ": " << pIK->
                getLinearGroup( idx ) << " " <<
                ( ( lin_cstr[idx].val(  ) ==
                    0 ) ? string( "false" ) : string( "true" ) ) << endl;
            if( lin_cstr[idx].val(  ) == 1 ) {
                std::string lingroupname = pIK->getLinearGroup( idx );
                pIK->SaveConstraintResult( lingroupname );
            }
        }
        cout << "Satisfied constraints: " << sat_cstr[0].val(  ) << endl;
    }
}
