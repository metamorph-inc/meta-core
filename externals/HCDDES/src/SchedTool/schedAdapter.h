// Scheduler parser tool adapter -- interface ANTLR parser with C++ backend

#ifndef _SCHED_ADAPTER_H_
#define _SCHED_ADAPTER_H_

#include "schedModel.h"

#include <sstream>

// Utility Functions

template < class T >
    bool from_string( T & t,
                      const std::string & s,
                      std::ios_base & ( *f ) ( std::ios_base & ) ) {
    std::istringstream iss( s );
    return !( iss >> f >> t ).fail(  );
}

// String conversion stuff
#ifndef __cdecl
#define __cdecl
#endif
double __cdecl to_double( char *strval );


class SchedAdapter {
  public:
    void setResolution( double timeval, char *timeunits );

    void AddNewProc( char *procname,
                     double freqval, char *frequnits,
                     double sndohd, char *sndohdunits,
                     double rcvohd, char *rcvohdunits );

    void AddNewTask( char *procname, char *taskname,
                     char *frqspec, double freq, char *frequnits,
                     double dur, char *durunits );

	void AddTaskOffset( char * procname, char * taskname, 
						double offtime, char * offunits );

    void AddNewProcMsg( char *procname, char *msgname,
                        double msgsize, char *sizeunits, char *sndtask );

    void AddProcMsgRcvr( char *procname, char *msgname, char *rcvtask );

    void AddNewBus( char *busname,
                    double datarate, char *rateunits,
                    double timespec, char *timeunits );

    void AddBusProc( char *busname, char *procname );

    void AddNewMsg( char *busname, char *msgname,
                    double msgsize, char *sizeunits, char *sndtask );

    void AddMsgRcvr( char *busname, char *msgname, char *rcvtask );

	void AddMsgOffset( char * procname, char * taskname, 
					   double offtime, char * offunits );

    void AddNewSyncMsg( char *busname, char *msgname,
                        double synctime, char *timeunits,
                        double msgposmin, char *timeunitsmin );

	void AddSyncMsgMax( char *busname, char *msgname, 
						double msgposmax, char *timeunitsmax );

    void AddLatency( double latency, char *timeunits,
                     char *sndtask, char *rcvtask );

    // Singleton stuff
    static SchedAdapter *instance(  );
    bool SetSchedModel( SchedModel * pModel );
    int RunSchedModel(  ) {
        return _pModel->Run(  );
    }
protected:

    double FreqU2Val( char *frequnits );
    double TimeU2Val( char *timeunits );
    double RateU2Val( char *rateunits );
    double SizeU2Val( char *sizeunits );

    // Hidden constructors
    SchedAdapter(  ) { }
    SchedAdapter( const SchedAdapter & ) { }
    SchedAdapter & operator=( const SchedAdapter & ) { }
	
	

  private:
    static SchedAdapter *pInstance;
    SchedModel *_pModel;

};

#endif // _SCHED_ADAPTER_H_
