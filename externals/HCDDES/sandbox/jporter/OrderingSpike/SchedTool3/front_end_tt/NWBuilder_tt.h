#ifndef NWBuilder_h__
#define NWBuilder_h__

#include "NWModel.h"
#include "TaskTypes.h"
#include "MsgTypes.h"
#include "LatencyTypes.h"

#include <string>
#include <map>

class NWBuilder_tt
{
public:
	NWBuilder_tt( const std::string & filename );
	virtual ~NWBuilder_tt(void);

	virtual ProblemPtr getProblem() { return _problem; }

protected:
	static void SetCurrentNWBuilder( NWBuilder_tt * nwb );

	ProblemPtr		_problem;
	ProcPtr			_cur_proc;
	BusPtr			_cur_bus;
	ProcMsgPtr		_cur_pmsg;
	MsgPtr			_cur_msg;
	LatencyChainPtr _cur_latency_chain;

	std::map< std::string, SyncMsgPtr > _sync_msgs;

public:
	void Print() { _problem->Print(); }
	void setName( char * name );
	void setResolution( double timeval, char * units );
	void newProc( char * procname, double tickres, char * timeunits );
	void addSchedType( char * procname, char * schedtype );
	void newTTask( char * procname, char * taskname, char * freqspec, double freq, char * frequnits, double dur, char * timeunits );
	void newPTask( char * procname, char * taskname, char * freqspec, double freq, char * frequnits, double dur, char * timeunits, double deadline, char * dtimeunits );
	void newATask( char * procname, char * taskname, char * freqspec, double freq, char * frequnits, double dur, char * timeunits, double deadline, char * dtimeunits );
	void newSyncMsgRef( char * procname, char * msgname );
	void newProcMsg( char * procname, char * msgname, unsigned long long msgsize, char * sizeunits, char * sndtask );
	void addProcMsgRcvr( char * procname, char * msgname, char * rcvtask );
	void newTBus( char * busname, double datarate, char * rateunits, double setuptime, char * timeunits );
	void newTTBus( char * busname, double datarate, char * rateunits, double setuptime, char * timeunits, double slottime, char * stimeunits );
	void newAFDXBus( char * busname, double datarate, char * rateunits, double chanrate, char * crateunits, unsigned long long numchans );
	void newEthBus( char * busname, double datarate, char * rateunits );
	void newTTEBus( char * busname, double datarate, char * rateunits, double setuptime, char * timeunits, double slottime, char * stimeunits );
	void addBusProc( char * busname, char * procname );
	void newSyncMsg( char * busname, char * msgname, unsigned long long msgsize, char * sizeunits );
	void addSyncBounds( char * busname, char * msgname, double msgposmin, char * timeunitsmin, double msgposmax, char * timeunitsmax );
	void newMsg( char * busname, char * msgname, unsigned long long msgsize, char * sizeunits, char * sndtask );
	void addMsgRcvr( char * busname, char * msgname, char* rcvtask );
	void newBMsg( char * busname, char * msgname, unsigned long long msgsize, char * sizeunits, unsigned long long ret_msgsize, char * ret_sizeunits, double deadlinetime, char * dtimeunits, char * sndtask, char * rcvtask );
	void newLatencyBound( double latency, char * timeunits, char * sndtask, char * rcvtask );
	void newLatencyChain( double latency, char * timeunits );
	void addFirstChainElement( char * start_task );
	void addChainElement( char * intermed_elt );
	void addLastChainElement( char * end_task );
};

#endif // NWBuilder_h__
