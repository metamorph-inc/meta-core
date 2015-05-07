// Scheduler parser tool adapter -- interface ANTLR parser with C++ backend

#ifndef _SCHED_ADAPTER_H_
#define _SCHED_ADAPTER_H_

#include "schedModel.h"

#include <sstream>

// Utility Functions

template <class T>
bool from_string(T& t, 
				 const std::string& s, 
				 std::ios_base& (*f)(std::ios_base&))
{
	std::istringstream iss(s);
	return !(iss >> f >> t).fail();
}

// String conversion stuff
double __cdecl to_double( char * strval );


class SchedAdapter
{
public:
	void setResolution( double timeval, char * timeunits );

	void AddNewProc( char * procname, 
					 double freqval, char * frequnits, 
					 double sndohd, char * sndohdunits, 
					 double rcvohd, char * rcvohdunits );

	void AddNewTask( char * procname, char * taskname, 
					 char * frqspec, double freq, char * frequnits, 
					 double dur, char * durunits );

	void AddNewBus( char * busname, 
					double datarate, char * rateunits, 
					double timespec, char * timeunits );

	void AddNewMsg( char * busname, char * msgname, 
					double msgsize, char * sizeunits, 
					char * sndtask );

	void AddMsgRcvr( char * busname, char * msgname, char * rcvtask );

	void AddLatency( double latency, char * timeunits, 
					 char * sndtask, char * rcvtask );

	// Singleton stuff
	static SchedAdapter * instance();
	bool SetSchedModel( SchedModel * pModel ); 
	void RunSchedModel() { _pModel->Run(); }

protected:

	double FreqU2Val( char * frequnits );
	double TimeU2Val( char * timeunits );
	double RateU2Val( char * rateunits );
	double SizeU2Val( char * sizeunits );

	// Hidden constructors
	SchedAdapter() { }
	SchedAdapter( const SchedAdapter & ) { }
	SchedAdapter &operator= ( const SchedAdapter & ) { }

private:
	static SchedAdapter * pInstance;
	SchedModel * _pModel;

};

#endif // _SCHED_ADAPTER_H_