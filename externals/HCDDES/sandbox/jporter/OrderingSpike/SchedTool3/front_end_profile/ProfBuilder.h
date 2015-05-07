#ifndef ProfBuilder_h__
#define ProfBuilder_h__



class ProfBuilder
{
protected:
	static void SetCurrentProfBuilder( ProfBuilder * pfb );

public:
	ProfBuilder(void);
	virtual ~ProfBuilder(void);
	void setGlobals( double hypval, char * hyptimeunits, double resval, char * restimeunits );
	void newNodeTTaskProfile( char * nodename, char * typestr , char * profname );
	void addTTaskExtensibility( char * profname, long long extval );
	void addTaskInterval( char * taskname, long long start, long long end );
	void addTTaskProfileIFace( char * nodename, char * typestr,  char * profname, char * inodename,  char * itypestr, char * iprofname );
	void addTTaskIFaceParams( char * profname, long long iperiod, long long ideadline );
	void addTTaskIFaceInterval( char * taskname, long long start_tick, long long end_tick );
	void newNodePTaskProfile( char * nodename, char * typestr ,  char * profname );
	void addPTaskExtensibility( char * profname, double period, double capacity );
	void addPTaskParams( char * taskname, long long period, long long deadline );
	void addPTaskProfileIFace( char * nodename, char * typestr,  char * profname, char * inodename,  char * itypestr, char * iprofname );
	void addPTaskIFaceParams( char * profname, long long iperiod, long long ideadline );
	void newNodeATaskProfile( char * nodename, char * typestr,  char * profname );
	void addATaskExtensibility( char * profname, double period, double capacity );
	void addATaskParams( char * taskname, long long period, long long deadline );
	void addATaskProfileIFace( char * nodename, char * typestr,  char * profname, char * inodename,  char * itypestr, char * iprofname, long long iperiod, long long ideadline );
	void addATaskIFaceParams( char * profname, long long iperiod, long long ideadline );
	void newBusProfile( char * busname, char * typestr,  char * profname );
	void addBusExtensibility( char * profname, long long extval );
	void addMsg( char * msgname, long long durationticks );
	void addMsgWindowInterval( char * msgname, long long start_tick, long long end_tick );
};



#endif // ProfBuilder_h__
