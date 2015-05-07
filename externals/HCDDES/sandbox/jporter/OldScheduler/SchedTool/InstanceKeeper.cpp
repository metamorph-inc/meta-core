#include "InstanceKeeper.h"

#include <iostream>

using namespace std;

bool InstanceKeeper::FindName( unsigned int num, std::string & name )
{
	bool bSuccess = false;

	std::map< unsigned int, std::string >::const_iterator pVar = _var_instmap.find( num );
	if ( pVar != _var_instmap.end() )
	{
		name = pVar->second;
		bSuccess = true;
	}

	return bSuccess;
}

bool InstanceKeeper::FindLatencies( unsigned int num, std::vector< LatencyBound > & vlb )
{
	bool bSuccess = false;

	if ( num >= 0 && num < vlb.size() )
	{
		vlb = _Latencies[ num ];
		bSuccess = true;
	}

	return bSuccess;
}

bool InstanceKeeper::SerializeInstances( std::vector< std::string > & instnames, 
										std::vector< unsigned long long > & instsizes )
{
	bool bSuccess = true;
	unsigned int count;
	std::vector< DurativeVariable > vdv;

#if ( DBG_CONSTRAINTS == 1 )
	cout << "Serializing..." << endl; 
#endif

	for ( count = 0; count < instnames.size(); count++ )
	{
		DurativeVariable dv;
		std::map< std::string, unsigned int >::const_iterator pInst = _inst_varmap.find( instnames[count] );
		if ( pInst != _inst_varmap.end() )
		{
			dv.xvar = pInst->second;
			dv.duration = instsizes[ count ];
			vdv.push_back( dv );

#if ( DBG_CONSTRAINTS == 1 )
			cout << instnames[count] << " dur " << dv.duration << endl; 
#endif

		}
		else
		{
			bSuccess = false; // Didn't find that instance name
		}
	}

	if ( bSuccess )
	{
		_Serializations.push_back( vdv );
		_serial_constraints++;
	}

	return bSuccess;
}

bool InstanceKeeper::OrderInstances( std::vector< std::string > & instnames, 
									unsigned long long dist, unsigned long long dur, 
									bool strict )
{
	bool bSuccess = true;

	std::vector< LinearRelation > vlr;
	std::vector< std::string >::iterator pInst = instnames.begin();
	std::vector< std::string >::iterator pPrev = pInst;

#if ( DBG_CONSTRAINTS == 1 )
	cout << "Ordering..." << endl; 
#endif

	// Start on the second instance, and make pairs
	for ( pInst++; pInst != instnames.end(); pInst++ ) 
	{
		LinearRelation lr;
		std::map< std::string, unsigned int >::const_iterator pXVar = _inst_varmap.find( *pPrev );
		std::map< std::string, unsigned int >::const_iterator pYVar = _inst_varmap.find( *pInst );
		if ( pXVar != _inst_varmap.end() && pYVar != _inst_varmap.end() )
		{
			lr.xvar = pXVar->second;
			lr.yvar = pYVar->second;
			lr.offset = (int) -dist;
			lr.group = (int) _linear_groups;
#if ( DBG_CONSTRAINTS == 1 )
			cout << "x = " << *pPrev << " y = " << *pInst << " offset " << -lr.offset << endl; 
#endif
		}
		else
		{
			bSuccess = false;
			break;
		}

		vlr.push_back( lr );
		_linear_constraints++;
		pPrev = pInst;
	}


	if ( bSuccess )
	{
		// First, put a duration bound on the last variable
		DurativeVariable dv;
		dv.xvar = _inst_varmap[ *pPrev ];
		dv.duration = (unsigned int) dur;
		_Durations.push_back( dv );
		_duration_constraints++;

#if ( DBG_CONSTRAINTS == 1 )
		cout << "Duration bound..." << endl;
		cout << *pPrev << " dur " << dv.duration << endl; 
#endif

		// Then handle the ordering
		if ( strict )
		{
			_EqualityPairs.insert( _EqualityPairs.end(), vlr.begin(), vlr.end() );
		}
		else
		{
			// Handle wrap-around at the end of the hyperperiod (not necessary for eq constraints)
			LinearRelation lr;
			lr.xvar = _inst_varmap[ *pPrev ];
			lr.yvar = _inst_varmap[ *instnames.begin() ];
			lr.offset = (unsigned int) (_hyperperiod - dist);
			lr.group = _linear_groups++;
			vlr.push_back( lr );
			_InequalityPairs.insert( _InequalityPairs.end(), vlr.begin(), vlr.end() );

#if ( DBG_CONSTRAINTS == 1 )
			cout << "Handle wrap-around..." << endl;
			cout << "x = " << *pPrev << " y = " << *instnames.begin() << " offset " << -lr.offset<< endl; 
#endif
		}
		string desc = printOrderedInstances(instnames, dist, dur, strict);
		_lin_grp_desc[_linear_groups] = desc;
		_linear_groups++;
	}

	return bSuccess;
}

bool InstanceKeeper::AddLatencyBound( std::vector< std::string > & sndrnames, std::vector< std::string > & rcvrnames, unsigned long long maxdist )
{
	bool bSuccess = false;

	unsigned int count;
	std::vector< LatencyBound > vlb;
	cout << "Adding latency bound list #" << _Latencies.size() << endl;
	for ( count = 0; count < sndrnames.size(); count++ )
	{
		std::map< std::string, unsigned int >::const_iterator pSndr = _inst_varmap.find( sndrnames[count] );
		std::map< std::string, unsigned int >::const_iterator pRcvr = _inst_varmap.find( rcvrnames[count] );
		//cout << sndrnames[count] << " " << rcvrnames[count] << " " << maxdist << endl;
		if ( (pSndr != _inst_varmap.end()) && (pRcvr != _inst_varmap.end()) )
		{
			LatencyBound lb;
			lb.sendervar = pSndr->second;
			lb.rcvrvar = pRcvr->second;
			lb.maxdist = (int) maxdist;

			cout << pSndr->first << " " << pRcvr->first << " (" << lb.maxdist << " ticks)" << endl;

			lb.latvar = _Latencies.size();
			vlb.push_back( lb );

			bSuccess = true;
		}
	}

	if (bSuccess)
	{
		_Latencies.push_back( vlb );
		_latency_constraints++;
	}

	return bSuccess;
}

bool InstanceKeeper::GetNextEqualityPair( LinearRelation & lrel, bool reset )
{
	if ( reset )
		_pEqPairs = _EqualityPairs.begin();

	if ( _pEqPairs != _EqualityPairs.end() )
	{
		lrel = *_pEqPairs;
		_pEqPairs++;
		return true;
	}
	else
		return false;
}

bool InstanceKeeper::GetNextInequalityPair( LinearRelation & lrel, bool reset )
{
	if ( reset )
		_pIeqPairs = _InequalityPairs.begin();

	if ( _pIeqPairs != _InequalityPairs.end() )
	{
		lrel = *_pIeqPairs;
		_pIeqPairs++;
		return true;
	}
	else
		return false;
}

bool InstanceKeeper::GetNextDuration( DurativeVariable & dv, bool reset )
{
	if ( reset )
		_pDurations = _Durations.begin();

	if ( _pDurations != _Durations.end() )
	{
		dv = *_pDurations;
		_pDurations++;
		return true;
	}
	else
		return false;
}

bool InstanceKeeper::GetNextSerialization( std::vector< DurativeVariable > & vdv, bool reset )
{
	if ( reset )
		_pSerializations = _Serializations.begin();

	if ( _pSerializations != _Serializations.end() )
	{
		vdv = *_pSerializations;
		_pSerializations++;
		return true;
	}
	else
		return false;
}

bool InstanceKeeper::GetNextLatencyBound( std::vector< LatencyBound  > & vlb, bool reset )
{
	if ( reset )
		_pLatencies = _Latencies.begin();

	if ( _pLatencies != _Latencies.end() )
	{
		vlb = *_pLatencies;
		_pLatencies++;
		return true;
	}
	else
		return false;
}

std::string InstanceKeeper::printIneq( LinearRelation & lrel )
{
	ostringstream out;
	out << _var_instmap[lrel.xvar] << "(" << lrel.xvar << ")";
	out << " <= ";
	out << _var_instmap[lrel.yvar] << "(" << lrel.yvar << ")";
	out << " + ";
	out << lrel.offset;

	return out.str();
}

std::string InstanceKeeper::printEq( LinearRelation & lrel )
{
	ostringstream out;
	out << _var_instmap[lrel.xvar] << "(" << lrel.xvar << ")";
	out << " = ";
	out << _var_instmap[lrel.yvar] << "(" << lrel.yvar << ")";
	out << " + ";
	out << lrel.offset;

	return out.str();
}

std::string InstanceKeeper::printDur( DurativeVariable & dv )
{
	return string(""); // @todo: Fix this later!
}

std::string InstanceKeeper::printSer( std::vector< DurativeVariable > & vdv )
{
	return string(""); // @todo: Fix this later!
}

std::string InstanceKeeper::printLat( std::vector< LatencyBound > & vlb )
{
	//unsigned int count;

	/*ostringstream out;
	out << "Latency bound:";
	for ( count = 0; count < vlb.size(); count++ )
	{
		std::map< std::string, unsigned int >::const_iterator pSndr = _inst_varmap.find( sndrnames[count] );
		std::map< std::string, unsigned int >::const_iterator pRcvr = _inst_varmap.find( rcvrnames[count] );
		//cout << sndrnames[count] << " " << rcvrnames[count] << " " << maxdist << endl;
		if ( (pSndr != _inst_varmap.end()) && (pRcvr != _inst_varmap.end()) )
		{
			LatencyBound lb;
			lb.sendervar = pSndr->second;
			lb.rcvrvar = pRcvr->second;
			lb.maxdist = (int) maxdist;

			cout << pSndr->first << " " << pRcvr->first << " (" << lb.maxdist << " ticks)" << endl;

			lb.latvar = _Latencies.size();
			vlb.push_back( lb );

			bSuccess = true;
		}
	}*/
	return string(""); // @todo: Fix this later!
}

std::string InstanceKeeper::printOrderedInstances( std::vector< std::string > & instnames, 
												   unsigned long long dist, unsigned long long dur, bool strict)
{
	ostringstream out;

	out << "Linear group: ";

	for ( std::vector< std::string >::iterator pInst = instnames.begin(); pInst != instnames.end(); pInst++ )
	{
		out << *pInst << ",";
	}

	out << " dist " << dist << " dur " << dur << ((strict) ? string("eq =") : string("ineq <="));
	return out.str();
}
