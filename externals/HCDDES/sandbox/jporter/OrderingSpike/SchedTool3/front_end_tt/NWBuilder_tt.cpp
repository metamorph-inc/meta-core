#include "NWBuilder_tt.h"
#include "NWModel.h"
#include "TaskTypes.h"
#include "BusTypes.h"
#include "MsgTypes.h"
#include "LatencyTypes.h"
#include "ParseUtils.h"

#include <boost/make_shared.hpp>

#include <exception>
#include <iostream>

using namespace std;

NWBuilder_tt * _current_nwb;

extern "C"
{
	#include <antlr3.h>

	#include "schedmodelLexer.h"
	#include "schedmodelParser.h"
};


NWBuilder_tt::NWBuilder_tt( const string & filename )
{
	pANTLR3_INPUT_STREAM pIStream = antlr3AsciiFileStreamNew((pANTLR3_UINT8) filename.c_str() );
	// Make sure there is no error opening the file
	if ((pANTLR3_INPUT_STREAM)ANTLR3_ERR_NOFILE == pIStream)
	{
		std::cout << "File " << filename << " not found. " << std::endl;
		exit(-1);
	}

	if ((pANTLR3_INPUT_STREAM)ANTLR3_ERR_NOFILE == pIStream)
	{
		std::cout << "Insufficient memory to open " << filename << "." << std::endl;
		exit(-1);
	}

	if ( !pIStream )
	{
		std::cout << "Unknown error while opening " << filename << "." << std::endl;
		exit(-1);
	}

	// Make sure we have something to call back to before we start!
	NWBuilder_tt::SetCurrentNWBuilder( this );
	pschedmodelLexer pLexer = schedmodelLexerNew( pIStream );

	pANTLR3_COMMON_TOKEN_STREAM pTokens = antlr3CommonTokenStreamSourceNew( 0, TOKENSOURCE(pLexer) );
	pschedmodelParser pParser = schedmodelParserNew( pTokens );
	pParser->specs(pParser);

	_problem->Setup();

	//_problem->Print();
}

NWBuilder_tt::~NWBuilder_tt(void)
{
}

void NWBuilder_tt::SetCurrentNWBuilder(NWBuilder_tt * nwb)
{
	_current_nwb = nwb;
}

void NWBuilder_tt::setName( char * name )
{
	// Create the problem instance first and give it a name
	_problem = boost::make_shared< Problem >( string( name ) );
}

void NWBuilder_tt::setResolution( double timeval, char * units )
{
	cout << "Resolution " << timeval << units << endl;
	double resolution = timeval * TimeU2Val( units );
	_problem->setMinimumResolution( resolution );
}

void NWBuilder_tt::newProc( char * procname, double tickres, char * timeunits )
{
	cout << "Proc " << procname<< " " << tickres << timeunits << endl;
	ProcPtr proc = boost::make_shared< Proc >( string( procname ), _problem );
	proc->SetupParent();  // required because the shared_from_this() in the callback doesn't work until make_shared is finished
	proc->setResolution( tickres * TimeU2Val(timeunits) );
	_cur_proc = proc;
}

void NWBuilder_tt::addSchedType( char * procname, char * schedtype )
{
	cout << "    " << schedtype << endl;
	_cur_proc->setSchedType( string( schedtype ) );
}

void NWBuilder_tt::newTTask( char * procname, char * taskname, char * freqspec, 
							double freq, char * frequnits, 
							double dur, char * timeunits )
{
	cout << "TTask " << taskname << freqspec << " " << freq << frequnits << " " << dur << timeunits << endl;
	TTaskPtr ttask = boost::make_shared< TTask >( string( taskname ), _cur_proc, 
													freq * FreqU2Val(frequnits), 
													dur * TimeU2Val(timeunits) );
	ttask->SetupParent();  // required because the shared_from_this() in the callback doesn't work until make_shared is finished

}

void NWBuilder_tt::newPTask( char * procname, char * taskname, char * freqspec, 
							double freq, char * frequnits, 
							double dur, char * timeunits, 
							double deadline, char * dtimeunits )
{
	PTaskPtr ptask = boost::make_shared< PTask >( string( taskname ), _cur_proc, 
													freq * FreqU2Val(frequnits), 
													dur * TimeU2Val(timeunits),
													deadline * TimeU2Val( dtimeunits ));
	ptask->SetupParent();  // required because the shared_from_this() in the callback doesn't work until make_shared is finished
}

void NWBuilder_tt::newATask( char * procname, char * taskname, char * freqspec, 
							double freq, char * frequnits, double dur, char * timeunits, 
							double deadline, char * dtimeunits )
{
	ATaskPtr atask = boost::make_shared< ATask >( string( taskname ), _cur_proc, 
													freq * FreqU2Val(frequnits), 
													dur * TimeU2Val(timeunits),
													deadline * TimeU2Val( dtimeunits ));
	atask->SetupParent();  // required because the shared_from_this() in the callback doesn't work until make_shared is finished
}

void NWBuilder_tt::newSyncMsgRef( char * procname, char * msgname )
{
	cout << "Sync Msg Ref proc " << procname << "(" << _cur_proc->Name() << ") msg " << msgname << endl;
	map< string, SyncMsgPtr >::iterator rslt = _sync_msgs.find( string( msgname ) ); // Have we already seen this sync msg name?
	if ( rslt == _sync_msgs.end() )  // if not, make a new one
	{
		SyncMsgPtr syncmsg = boost::make_shared< SyncMsg >( string( msgname ) );
		_cur_proc->AddSyncMsg( syncmsg );
		_sync_msgs.insert( make_pair( string( msgname ), syncmsg ) );
	}
	else // if so, make sure the current proc knows about the existing sync msg
	{
		_cur_proc->AddSyncMsg( rslt->second );
	}

}

void NWBuilder_tt::newProcMsg( char * procname, char * msgname, unsigned long long msgsize, char * sizeunits, char * sndtask )
{
	cout << "ProcMsg " << msgname << " " << msgsize << sizeunits << " " << sndtask << endl;
	ProcMsgPtr procmsg = boost::make_shared< ProcMsg >( string( msgname ), _cur_proc, 
															msgsize * SizeU2Val( sizeunits ) );
	procmsg->SetupParent( string( sndtask ) );  // required because the shared_from_this() 
												// in the callback doesn't work until 
												// make_shared is finished
	_cur_pmsg = procmsg;
}

void NWBuilder_tt::addProcMsgRcvr( char * procname, char * msgname, char * rcvtask )
{
	cout << "    " << rcvtask << endl;
	_cur_pmsg->AddReceiver( string( rcvtask ) );

}

void NWBuilder_tt::newTBus( char * busname, double datarate, char * rateunits, 
							double setuptime, char * timeunits )
{
	cout << "TBus " << busname << " " << datarate << rateunits << " " << setuptime << timeunits << endl;
	TBusPtr tbus = boost::make_shared< TBus >( string( busname ), _problem, 
												datarate * RateU2Val( rateunits ), 
												setuptime * TimeU2Val( timeunits ) );
	tbus->SetupParent();
	_cur_bus = tbus;
}

void NWBuilder_tt::newTTBus( char * busname, double datarate, char * rateunits, 
							double setuptime, char * timeunits, 
							double slottime, char * stimeunits )
{
	TTBusPtr ttbus = boost::make_shared< TTBus >( string( busname ), _problem, 
													datarate * RateU2Val( rateunits ), 
													setuptime * TimeU2Val( timeunits ),
													slottime * TimeU2Val( stimeunits ) );
	ttbus->SetupParent();
	_cur_bus = ttbus;
}

void NWBuilder_tt::newAFDXBus( char * busname, double datarate, char * rateunits, 
							double chanrate, char * crateunits, unsigned long long numchans )
{
	cout << "AFDXBus " << busname << " " << datarate << rateunits << " " << chanrate << crateunits << " " << numchans << endl;	
	AFDXBusPtr afdxbus = boost::make_shared< AFDXBus >( string( busname ), _problem, 
														datarate * RateU2Val( rateunits ),
														chanrate * RateU2Val( crateunits ),
														numchans );	
	afdxbus->SetupParent();
	_cur_bus = afdxbus;
}

void NWBuilder_tt::newEthBus( char * busname, double datarate, char * rateunits )
{
	EthBusPtr ethbus = boost::make_shared< EthBus >( string( busname ), _problem,
														datarate * RateU2Val( rateunits ) );
	ethbus->SetupParent();
	_cur_bus = ethbus;
}

void NWBuilder_tt::newTTEBus( char * busname, double datarate, char * rateunits, 
							double setuptime, char * timeunits, 
							double slottime, char * stimeunits )
{
	TTEBusPtr ttebus = boost::make_shared< TTEBus >( string( busname ), _problem, 
														datarate * RateU2Val( rateunits ), 
														setuptime * TimeU2Val( timeunits ),
														slottime * TimeU2Val( stimeunits ) );
	ttebus->SetupParent();
	_cur_bus = ttebus;
}

void NWBuilder_tt::addBusProc( char * busname, char * procname )
{
	_cur_bus->AddBusProc( string( procname ) );
}

void NWBuilder_tt::newSyncMsg( char * busname, char * msgname, 
							unsigned long long msgsize, char * sizeunits )
{
	
	cout << "SyncMsg " << msgname << " " << msgsize << sizeunits << " bus " << busname << "(" << _cur_bus->Name() << ")" << endl;
	map< string, SyncMsgPtr >::iterator rslt = _sync_msgs.find( string( msgname ) );
	if ( rslt != _sync_msgs.end() )
	{
		SyncMsgPtr syncmsg( rslt->second );

		syncmsg->SetupParent( _cur_bus );
		syncmsg->setSize( msgsize * SizeU2Val( sizeunits ) );
		cout << "Checking parent pointer: " << syncmsg->parent()->Name() << endl;;
	}
	else
		cout << "Error: unable to find sync msg!!!" << endl;
	
}

void NWBuilder_tt::addSyncBounds( char * busname, char * msgname, 
								double msgposmin, char * timeunitsmin, 
								double msgposmax, char * timeunitsmax )
{
	SyncMsgPtr syncmsg = _sync_msgs[ string( msgname ) ];
	syncmsg->AddBounds( msgposmin * TimeU2Val( timeunitsmin ), 
						msgposmax * TimeU2Val( timeunitsmax ) );
}

void NWBuilder_tt::newMsg( char * busname, char * msgname, 
						unsigned long long msgsize, char * sizeunits, char * sndtask )
{
	cout << "Msg " << msgname << " " << msgsize << sizeunits << " " << sndtask << endl;
	string procname, sending_task;
	split_string_path( string( sndtask ), procname, sending_task );
	

	MsgPtr msg = boost::make_shared< Msg >( string( msgname ), _cur_bus, 
											msgsize * SizeU2Val( sizeunits ) );

	msg->SetupParent( procname, sending_task ); // the parent can not know about this object
	                                            // until after make_shared has completed
	_cur_msg = msg;
}

void NWBuilder_tt::addMsgRcvr( char * busname, char * msgname, char* rcvtask )
{
	cout << "    " << rcvtask << endl;
	string procname, receiving_task;
	split_string_path( string( rcvtask ), procname, receiving_task );
	_cur_msg->AddReceivingTask( procname, receiving_task );
}

void NWBuilder_tt::newBMsg( char * busname, char * msgname, 
							unsigned long long msgsize, char * sizeunits, 
							unsigned long long ret_msgsize, char * ret_sizeunits, 
							double deadlinetime, char * dtimeunits, 
							char * sndtask, char * rcvtask )
{
	string sending_proc, sending_task, receiving_proc, receiving_task;
	split_string_path( string( sndtask ), sending_proc, sending_task );
	split_string_path( string( rcvtask ), receiving_proc, receiving_task );
	BMsgPtr bmsg = boost::make_shared< BMsg >( string( msgname ), _cur_bus,
												msgsize * SizeU2Val( sizeunits ),
												ret_msgsize * SizeU2Val( ret_sizeunits ),
												deadlinetime * TimeU2Val( dtimeunits ) );

	bmsg->SetupParent( sending_proc, sending_task, receiving_proc, receiving_task );
}

void NWBuilder_tt::newLatencyBound( double latency, char * timeunits, char * sndtask, char * rcvtask )
{
	string sending_proc, sending_task, receiving_proc, receiving_task;
	split_string_path( string( sndtask ), sending_proc, sending_task );
	split_string_path( string( rcvtask ), receiving_proc, receiving_task );
	LatencyBoundPtr latency_obj = boost::make_shared< LatencyBound >( _problem, latency * TimeU2Val( timeunits ),
																	sending_proc, sending_task, 
																	receiving_proc, receiving_task );	

	latency_obj->SetupParent();
}

void NWBuilder_tt::newLatencyChain( double latency, char * timeunits )
{
	LatencyChainPtr latency_obj = boost::make_shared< LatencyChain >( _problem, latency * TimeU2Val( timeunits ) );

	latency_obj->SetupParent();
	_cur_latency_chain = latency_obj;
}

void NWBuilder_tt::addFirstChainElement( char * start_task )
{
	string procname, first_task;
	split_string_path( string( start_task ), procname, first_task );

	_cur_latency_chain->AddFirstElement( procname, first_task );
}

void NWBuilder_tt::addChainElement( char * intermed_elt )
{
	string procname, element_name;
	split_string_path( string( intermed_elt ), procname, element_name );

	_cur_latency_chain->AddIntermediateElement( procname, element_name );
}

void NWBuilder_tt::addLastChainElement( char * end_task )
{
	string procname, last_task;
	split_string_path( string( end_task ), procname, last_task );

	_cur_latency_chain->AddLastElement( procname, last_task );	
}

// Below this are the C callback functions which are exported for the ANTLR parser

#ifndef __cdecl
#define __cdecl
#endif

extern "C" {

	int __cdecl setName( pANTLR3_STRING name )
	{
		_current_nwb->setName( (char *) name->to8(name)->chars );
		return 0;
	}

	int __cdecl setResolution( pANTLR3_STRING timeval,
		pANTLR3_STRING timeunits )
	{
		_current_nwb->setResolution( 
			to_double( (char *) timeval->to8(timeval)->chars ),
			(char *) timeunits->to8(timeunits)->chars 
		);
		return 0;
	}

	int __cdecl newProc( pANTLR3_STRING procname,
		pANTLR3_STRING tickresolutionval,
		pANTLR3_STRING ticktimeunits )
	{
		_current_nwb->newProc( 
			(char *) procname->to8(procname)->chars, 
			to_double( (char *) tickresolutionval->to8(tickresolutionval)->chars ),
			(char *) ticktimeunits->to8(ticktimeunits)->chars 
			);
		return 0;
	}

	int __cdecl addSchedType( pANTLR3_STRING procname,
		pANTLR3_STRING scheduletypestr )
	{
		_current_nwb->addSchedType(
			(char *) procname->to8(procname)->chars,
			(char *) scheduletypestr->to8(scheduletypestr)->chars
			);
		return 0;
	}

	int __cdecl newTTask( pANTLR3_STRING procname,
		pANTLR3_STRING taskname,
		pANTLR3_STRING freqspecchar,
		pANTLR3_STRING taskfreq,
		pANTLR3_STRING frequnits,
		pANTLR3_STRING taskdur,
		pANTLR3_STRING durunits )
	{
		_current_nwb->newTTask( 
			(char *) procname->to8(procname)->chars,
			(char *) taskname->to8(taskname)->chars,
			(char *) freqspecchar->to8(freqspecchar)->chars,
			to_double( (char *) taskfreq->to8(taskfreq)->chars ),
			(char *) frequnits->to8(frequnits)->chars,
			to_double( (char *) taskdur->to8(taskdur)->chars ),
			(char *) durunits->to8(durunits)->chars
			);
		return 0;
	}

	int __cdecl newPTask( pANTLR3_STRING procname,
		pANTLR3_STRING taskname,
		pANTLR3_STRING freqspecchar,
		pANTLR3_STRING taskfreq,
		pANTLR3_STRING frequnits,
		pANTLR3_STRING taskdur,
		pANTLR3_STRING durunits,
		pANTLR3_STRING taskdeadline,
		pANTLR3_STRING deadlineunits )
	{
		_current_nwb->newPTask( 
			(char *) procname->to8(procname)->chars,
			(char *) taskname->to8(taskname)->chars,
			(char *) freqspecchar->to8(freqspecchar)->chars,
			to_double( (char *) taskfreq->to8(taskfreq)->chars ),
			(char *) frequnits->to8(frequnits)->chars,
			to_double( (char *) taskdur->to8(taskdur)->chars ),
			(char *) durunits->to8(durunits)->chars,
			to_double( (char *) taskdeadline->to8(taskdeadline)->chars ),
			(char *) deadlineunits->to8(deadlineunits)->chars
			);
		return 0;
	}

	int __cdecl newATask( pANTLR3_STRING procname,
		pANTLR3_STRING taskname,
		pANTLR3_STRING freqspecchar,
		pANTLR3_STRING taskfreq,
		pANTLR3_STRING frequnits,
		pANTLR3_STRING taskdur,
		pANTLR3_STRING durunits,
		pANTLR3_STRING taskdeadline,
		pANTLR3_STRING deadlineunits )
	{
		_current_nwb->newATask( 
			(char *) procname->to8(procname)->chars,
			(char *) taskname->to8(taskname)->chars,
			(char *) freqspecchar->to8(freqspecchar)->chars,
			to_double( (char *) taskfreq->to8(taskfreq)->chars ),
			(char *) frequnits->to8(frequnits)->chars,
			to_double( (char *) taskdur->to8(taskdur)->chars ),
			(char *) durunits->to8(durunits)->chars,
			to_double( (char *) taskdeadline->to8(taskdeadline)->chars ),
			(char *) deadlineunits->to8(deadlineunits)->chars
			);
		return 0;
	}

	int __cdecl newSyncMsgRef( pANTLR3_STRING procname, pANTLR3_STRING msgname )
	{
		_current_nwb->newSyncMsgRef( 
			(char *) procname->to8(procname)->chars,
			(char *) msgname->to8(msgname)->chars
			);
		return 0;
	}

	int __cdecl newProcMsg( pANTLR3_STRING procname,
		pANTLR3_STRING msgname,
		pANTLR3_STRING msgsize,
		pANTLR3_STRING sizeunits,
		pANTLR3_STRING sndtask )
	{
		_current_nwb->newProcMsg(
			(char *) procname->to8(procname)->chars,
			(char *) msgname->to8(msgname)->chars,
			to_integer( (char *) msgsize->to8(msgsize)->chars ),
			(char *) sizeunits->to8(sizeunits)->chars,
			(char *) sndtask->to8(sndtask)->chars
			);
		return 0;
	}

	int __cdecl addProcMsgRcvr( pANTLR3_STRING procname,
		pANTLR3_STRING msgname,
		pANTLR3_STRING rcvtask )
	{
		_current_nwb->addProcMsgRcvr(
			(char *) procname->to8(procname)->chars,
			(char *) msgname->to8(msgname)->chars,
			(char *) rcvtask->to8(rcvtask)->chars
			);
		return 0;
	}

	int __cdecl newTBus( pANTLR3_STRING busname,
		pANTLR3_STRING datarate,
		pANTLR3_STRING rateunits,
		pANTLR3_STRING setuptime,
		pANTLR3_STRING timeunits )
	{
		_current_nwb->newTBus(
			(char *) busname->to8(busname)->chars,
			to_double( (char *) datarate->to8(datarate)->chars ),
			(char *) rateunits->to8(rateunits)->chars,
			to_double( (char *) setuptime->to8(setuptime)->chars ),
			(char *) timeunits->to8(timeunits)->chars
			);
		return 0;
	}

	int __cdecl newTTBus( pANTLR3_STRING busname,
		pANTLR3_STRING datarate,
		pANTLR3_STRING rateunits,
		pANTLR3_STRING setuptime,
		pANTLR3_STRING timeunits,
		pANTLR3_STRING slottime,
		pANTLR3_STRING stimeunits )
	{
		_current_nwb->newTTBus(
			(char *) busname->to8(busname)->chars,
			to_double( (char *) datarate->to8(datarate)->chars ),
			(char *) rateunits->to8(rateunits)->chars,
			to_double( (char *) setuptime->to8(setuptime)->chars ),
			(char *) timeunits->to8(timeunits)->chars,
			to_double( (char *) slottime->to8(slottime)->chars ),
			(char *) stimeunits->to8(stimeunits)->chars
			);
		return 0;
	}

	int __cdecl newAFDXBus( pANTLR3_STRING busname,
		pANTLR3_STRING datarate,
		pANTLR3_STRING rateunits,
		pANTLR3_STRING channelrate,
		pANTLR3_STRING crateunits,
		pANTLR3_STRING numchannels )
	{
		_current_nwb->newAFDXBus(
			(char *) busname->to8(busname)->chars,
			to_double( (char *) datarate->to8(datarate)->chars ),
			(char *) rateunits->to8(rateunits)->chars,
			to_double( (char *) channelrate->to8(channelrate)->chars ),
			(char *) crateunits->to8(crateunits)->chars,
			to_integer( (char *) numchannels->to8(numchannels)->chars  )
			);
		return 0;
	}

	int __cdecl newEthBus( pANTLR3_STRING busname,
		pANTLR3_STRING datarate,
		pANTLR3_STRING rateunits )
	{
		_current_nwb->newEthBus(
			(char *) busname->to8(busname)->chars,
			to_double( (char *) datarate->to8(datarate)->chars ),
			(char *) rateunits->to8(rateunits)->chars
			);
		return 0;
	}

	int __cdecl newTTEBus( pANTLR3_STRING busname,
		pANTLR3_STRING datarate,
		pANTLR3_STRING rateunits,
		pANTLR3_STRING setuptime,
		pANTLR3_STRING timeunits,
		pANTLR3_STRING slottime,
		pANTLR3_STRING stimeunits )
	{
		_current_nwb->newTTEBus(
			(char *) busname->to8(busname)->chars,
			to_double( (char *) datarate->to8(datarate)->chars ),
			(char *) rateunits->to8(rateunits)->chars,
			to_double( (char *) setuptime->to8(setuptime)->chars ),
			(char *) timeunits->to8(timeunits)->chars,
			to_double( (char *) slottime->to8(slottime)->chars ),
			(char *) stimeunits->to8(stimeunits)->chars
			);
		return 0;
	}

	int __cdecl addBusProc( pANTLR3_STRING busname,
		pANTLR3_STRING procname )
	{
		_current_nwb->addBusProc(
			(char *) busname->to8(busname)->chars,
			(char *) procname->to8(procname)->chars
			);
		return 0;
	}

	int __cdecl newSyncMsg( pANTLR3_STRING busname,
		pANTLR3_STRING msgname,
		pANTLR3_STRING msgsize,
		pANTLR3_STRING sizeunits )
	{
		_current_nwb->newSyncMsg(
			(char *) busname->to8(busname)->chars,
			(char *) msgname->to8(msgname)->chars,
			to_integer( (char *) msgsize->to8(msgsize)->chars ),
			(char *) sizeunits->to8(sizeunits)->chars
			);
		return 0;
	}

	int __cdecl addSyncBounds( pANTLR3_STRING busname,
		pANTLR3_STRING msgname,
		pANTLR3_STRING msgposmin,
		pANTLR3_STRING timeunitsmin,
		pANTLR3_STRING msgposmax,
		pANTLR3_STRING timeunitsmax )
	{
		_current_nwb->addSyncBounds(
			(char *) busname->to8(busname)->chars,
			(char *) msgname->to8(msgname)->chars,
			to_double( (char *) msgposmin->to8(msgposmin)->chars ),
			(char *) timeunitsmin->to8(timeunitsmin)->chars,
			to_double( (char *) msgposmax->to8(msgposmax)->chars ),
			(char *) timeunitsmax->to8(timeunitsmax)->chars
			);
		return 0;
	}

	int __cdecl newMsg( pANTLR3_STRING busname,
		pANTLR3_STRING msgname,
		pANTLR3_STRING msgsize,
		pANTLR3_STRING sizeunits,
		pANTLR3_STRING sndtask )
	{
		_current_nwb->newMsg(
			(char *) busname->to8(busname)->chars,
			(char *) msgname->to8(msgname)->chars,
			to_integer( (char *) msgsize->to8(msgsize)->chars ),
			(char *) sizeunits->to8(sizeunits)->chars,
			(char *) sndtask->to8(sndtask)->chars
			);
		return 0;
	}

	int __cdecl addMsgRcvr( pANTLR3_STRING busname,
		pANTLR3_STRING msgname,
		pANTLR3_STRING rcvtask )
	{
		_current_nwb->addMsgRcvr(
			(char *) busname->to8(busname)->chars,
			(char *) msgname->to8(msgname)->chars,
			(char *) rcvtask->to8(rcvtask)->chars
			);
		return 0;
	}

	int __cdecl newBMsg( pANTLR3_STRING busname,
		pANTLR3_STRING msgname,
		pANTLR3_STRING msgsize,
		pANTLR3_STRING sizeunits,
		pANTLR3_STRING msg2size,
		pANTLR3_STRING sizeunits2,
		pANTLR3_STRING deadlinetime,
		pANTLR3_STRING timeunits,
		pANTLR3_STRING sndtask,
		pANTLR3_STRING rcvtask )
	{
		_current_nwb->newBMsg(
			(char *) busname->to8(busname)->chars,
			(char *) msgname->to8(msgname)->chars,
			to_integer( (char *) msgsize->to8(msgsize)->chars ),
			(char *) sizeunits->to8(sizeunits)->chars,
			to_integer( (char *) msg2size->to8(msg2size)->chars ),
			(char *) sizeunits2->to8(sizeunits2)->chars,
			to_double( (char *) deadlinetime->to8(deadlinetime)->chars ),
			(char *) timeunits->to8(timeunits)->chars,
			(char *) sndtask->to8(sndtask)->chars,
			(char *) rcvtask->to8(rcvtask)->chars
			);
		return 0;
	}

	int __cdecl newLatencyBound( pANTLR3_STRING timespec,
		pANTLR3_STRING timeunits,
		pANTLR3_STRING sndtask,
		pANTLR3_STRING rcvtask )
	{
		_current_nwb->newLatencyBound(
			to_double( (char *) timespec->to8(timespec)->chars ),
			(char *) timeunits->to8(timeunits)->chars,
			(char *) sndtask->to8(sndtask)->chars,
			(char *) rcvtask->to8(rcvtask)->chars
			);
		return 0;
	}

	int __cdecl newLatencyChain( pANTLR3_STRING timespec,
		pANTLR3_STRING timeunits )
	{
		_current_nwb->newLatencyChain(
			to_double( (char *) timespec->to8(timespec)->chars ),
			(char *) timeunits->to8(timeunits)->chars
			);
		return 0;
	}

	int __cdecl addFirstChainElement( pANTLR3_STRING start_task )
	{
		_current_nwb->addFirstChainElement(
			(char *) start_task->to8(start_task)->chars
			);
		return 0;
	}

	int __cdecl addChainElement( pANTLR3_STRING intermed_elt )
	{
		_current_nwb->addChainElement(
			(char *) intermed_elt->to8(intermed_elt)->chars
			);
		return 0;
	}

	int __cdecl addLastChainElement( pANTLR3_STRING end_task )
	{
		_current_nwb->addLastChainElement(
			(char *) end_task->to8(end_task)->chars
			);
		return 0;
	}
}

