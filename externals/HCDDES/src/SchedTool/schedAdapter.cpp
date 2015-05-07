
#include "schedAdapter.h"

#include <iostream>
#include <vector>

using namespace std;

double __cdecl to_double( char *strval ) {
    std::string sv( strval );
    double val;

    if( from_string < double >( val, sv, std::dec ) )
        return val;
    else
        return -1.0;
}

void SchedAdapter::setResolution( double timeval, char *timeunits ) {
    double t = timeval * TimeU2Val( timeunits );

    //cout << "Global resolution: " << t << " secs" << endl;

    _pModel->SetResolution( t );
}

void SchedAdapter::AddNewProc( char *procname,
                               double freqval, char *frequnits,
                               double sndohd, char *sndohdunits,
                               double rcvohd, char *rcvohdunits ) {
    double f = freqval * FreqU2Val( frequnits );

    double s = sndohd * TimeU2Val( sndohdunits );

    double r = rcvohd * TimeU2Val( rcvohdunits );

    //cout << procname << ": ";
    //cout << f << " ";
    //cout << s << " ";
    //cout << r << endl;

    _pModel->AddProc( Proc( string( procname ), f, s, r ) );
}

void SchedAdapter::AddNewTask( char *procname, char *taskname,
                               char *frqspec, double freq, char *frequnits,
                               double dur, char *durunits ) {
    std::string errMsg;
    std::string task =
        std::string( procname ) + "/" + std::string( taskname );

    double f = freq * FreqU2Val( frequnits );

    double t = dur * TimeU2Val( durunits );

    if( t > 1 / f )
        errMsg =
            "  !! Task " + task + ": Duration is too large for frequency.";
    //cout << task << ": ";
    //cout << frqspec << f << " ";
    //cout << t << errMsg << endl;

    _pModel->AddTask( string( procname ),
                      Task( string( taskname ), string( procname ), f, t,
                            ( string( frqspec ) == "=" ) ? true : false ) );
}

void SchedAdapter::AddTaskOffset( char * procname, char * taskname, double offtime, char * offunits )
{

	std::string fullname =
		std::string( procname ) + "/" + std::string( taskname );
	
	double ot = offtime * TimeU2Val( offunits );
	//cout << "Rcvr " << name << " " << rcvtask << endl;

	Task t;

	if( _pModel->GetTaskByName( fullname, t ) ) {
		t.AddOffset( ot );
		_pModel->UpdateTask(t);
	}
}

void SchedAdapter::AddNewProcMsg( char *procname, char *msgname,
                                  double msgsize, char *sizeunits,
                                  char *sndtask ) {
    double s = msgsize * SizeU2Val( sizeunits );

    //cout << procname << "/" << msgname << ": ";
    //cout << s << " ";
    //cout << sndtask << endl;

    _pModel->AddLocalMsg( string( procname ),
                          Msg( string( msgname ), string( procname ), s,
                               string( sndtask ) ) );
}

void SchedAdapter::AddProcMsgRcvr( char *procname, char *msgname,
                                   char *rcvtask ) {
    string name = string( procname ) + "/" + string( msgname );

    //cout << "Rcvr " << name << " " << rcvtask << endl;

    Msg m;

    if( _pModel->GetMsgByName( name, m ) ) {
        m.AddRcvr( string( rcvtask ) );
        _pModel->UpdateMsg( m );
    }
}

void SchedAdapter::AddNewBus( char *busname,
                              double datarate, char *rateunits,
                              double timespec, char *timeunits ) {
    double r = datarate * RateU2Val( rateunits );
    double t = timespec * TimeU2Val( timeunits );

    //cout << busname << ": ";
    //cout << r << " ";
    //cout << t << endl;

    _pModel->AddBus( Bus( string( busname ), r, t ) );
}

void SchedAdapter::AddBusProc( char *busname, char *procname ) {
    Bus b;

    string bname( busname );
	string pname( procname );
    if( _pModel->GetBusByName( bname, b ) ) {
        b.AddBusProc( pname );
        _pModel->UpdateBus( b );
    }
}

void SchedAdapter::AddNewMsg( char *busname, char *msgname,
                              double msgsize, char *sizeunits,
                              char *sndtask ) {
    double s = msgsize * SizeU2Val( sizeunits );

    //cout << busname << "/" << msgname << ": ";
    //cout << s << " ";
    //cout << sndtask << endl;

    _pModel->AddMsg( string( busname ),
                     Msg( string( msgname ), string( busname ), s,
                          string( sndtask ) ) );
}

void SchedAdapter::AddMsgRcvr( char *busname, char *msgname, char *rcvtask ) {
    string name = string( busname ) + "/" + string( msgname );

    //cout << "Rcvr " << name << " " << rcvtask << endl;

    Msg m;

    if( _pModel->GetMsgByName( name, m ) ) {
        m.AddRcvr( string( rcvtask ) );
        _pModel->UpdateMsg( m );
    }
}

void SchedAdapter::AddMsgOffset( char * busname, char * msgname, double offtime, char * offunits )
{
	string name = string( busname ) + "/" + string( msgname );
	double ot = offtime * TimeU2Val( offunits );
	//cout << "Rcvr " << name << " " << rcvtask << endl;

	Msg m;

	if( _pModel->GetMsgByName( name, m ) ) {
		m.AddOffset( ot );
		_pModel->UpdateMsg(m);
	}
}

void SchedAdapter::AddNewSyncMsg( char *busname, char *msgname,
                                  double synctime, char *timeunits,
                                  double msgposmin, char *timeunitsmin ) {

    double t = synctime * TimeU2Val( timeunits );
    double tmin = msgposmin * TimeU2Val( timeunitsmin );

    //cout << busname << "/" << msgname << ": ";
    //cout << s << " ";
    //cout << sndtask << endl;

    _pModel->AddSyncMsg( string( busname ),
                         SyncMsg( string( msgname ), string( busname ), t,
                                  tmin, tmin ) );
}

void SchedAdapter::AddSyncMsgMax( char *busname, char *msgname,
                                  double msgposmax, char *timeunitsmax ) {

	string name = string( busname ) + "/" + string( msgname );
    SyncMsg m;

	double tmax = msgposmax * TimeU2Val( timeunitsmax );
	//cout << "SyncMax " << name << " " << tmax << endl;

	if ( _pModel->GetSyncMsgByName( name, m ) ) {
		m.SetPosMax( tmax );
		_pModel->UpdateSyncMsg( m );
	}

}

void SchedAdapter::AddLatency( double latency, char *timeunits, char *sndtask,
                               char *rcvtask ) {
    double l = latency * TimeU2Val( timeunits );

    //cout << "Latency " << l << " ";
    //cout << sndtask << " " << rcvtask << endl;

    _pModel->AddLatency( Latency( string( sndtask ), string( rcvtask ), l ) );
}

double SchedAdapter::FreqU2Val( char *frequnits ) {
    std::string units( frequnits );

    if( units == "Hz" )
        return 1.0;
    else if( units == "kHz" )
        return 1e3;
    else if( units == "MHz" )
        return 1e6;
    else if( units == "GHz" )
        return 1e9;

    return -1.0;
}

double SchedAdapter::TimeU2Val( char *timeunits ) {
    std::string units( timeunits );

    if( units == "s" )
        return 1.0;
    else if( units == "ms" )
        return 1e-3;
    else if( units == "us" )
        return 1e-6;
    else if( units == "ns" )
        return 1e-9;
    else if( units == "ps" )
        return 1e-12;
    else if( units == "fs" )
        return 1e-15;

    return -1.0;
}

double SchedAdapter::RateU2Val( char *rateunits ) {
    std::string units( rateunits );

    if( units == "b" )
        return 1.0;
    else if( units == "kb" )
        return 1e3;
    else if( units == "Mb" )
        return 1e6;
    else if( units == "Gb" )
        return 1e9;

    return -1.0;
}

double SchedAdapter::SizeU2Val( char *sizeunits ) {
    std::string units( sizeunits );

    if( units == "B" )
        return 8.0;
    else if( units == "kB" )
        return 8 * 1024.0;
    else if( units == "MB" )
        return 8 * 1024.0 * 1024.0;
    else if( units == "GB" )
        return 8 * 1024.0 * 1024.0 * 1024.0;

    return -1.0;
}

// Singleton implementation
SchedAdapter *SchedAdapter::pInstance = 0;
SchedAdapter *SchedAdapter::instance(  ) {
    if( pInstance == 0 ) {
        pInstance = new SchedAdapter;
    }
    return pInstance;
}

// Store a reference to the scheduling model
bool SchedAdapter::SetSchedModel( SchedModel * pModel ) {
    if( pModel ) {
        _pModel = pModel;
        return true;
    }

    return false;
}




