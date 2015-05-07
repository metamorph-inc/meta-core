#ifndef _SCHED_VIZUALIZATION_H_
#define _SCHED_VIZUALIZATION_H_


/*** Included Header Files ***/
#include "Uml.h"
#include "ESMoL_Abstract\generated\Semantics.h"
#include <map>
#include <vector>
#include <sstream>
#include <iostream>
#include <fstream>


/*** Locally Defined Types ***/
class TraceViz;
class TVSuperblock;
class TVTrace;
// Typedefs for ESMoL_Abstract Paradigm
typedef std::set< Semantics::Deployment> DeploymentSet;
typedef std::set< Semantics::Node > NodeSet;
typedef std::set< Semantics::Task > TaskSet;
typedef std::set< Semantics::Device > DeviceSet;
typedef std::set< Semantics::IChan > IChanSet;
typedef std::set< Semantics::OChan > OChanSet;
typedef std::set< Semantics::CommMedium > BusSet;
typedef std::set< Semantics::TTBus > TTBusSet;
typedef std::set< Semantics::CANBus > CANBusSet;
typedef std::set< Semantics::ExecInfo > ExecInfoSet;
typedef std::set< Semantics::CommInterface > CommInterfaceSet;
typedef std::set< Semantics::BChan > BChanSet;
typedef std::set< Semantics::Transmits > TransmitsSet;
typedef std::set< Semantics::Receives > ReceivesSet;
typedef std::set< Semantics::Msg > MessageSet;


/***************************************************************************************/


class SchedViz {
private:
	bool				_debug;
	double				_hyperperiodSec;
	TraceViz*			_traceViz;

	// Private Methods
	SchedViz();													//!< Deny access to default constructor
public:
	// Constructors and Destructors	
	SchedViz( const std::string &abstractModel, const std::string &vizFilename, const bool &debug=false );
	~SchedViz() { }

	// Visitor Methods
	void Visit_RootFolder( const Semantics::RootFolder & rootFolder );
	void Visit_Deployment( const Semantics::Deployment & deployment );
	void Visit_Node( const Semantics::Node & node );
	void Visit_Task( const Semantics::Task & task, TVSuperblock * superblock );
	void Visit_Device( const Semantics::Device & device, TVSuperblock * superblock );
	void Visit_IChan( const Semantics::IChan & iChan, TVSuperblock * superblock );
	void Visit_OChan( const Semantics::OChan & oChan, TVSuperblock * superblock );
	void Visit_Bus( const Semantics::CommMedium & bus ); // There is no corresponding call in the superclass
	void Visit_Msg( const Semantics::Msg & message, TVTrace * trace );
};

class SchedVizException {
public:
	SchedVizException( std::string &reason ) : _reason(reason) { }
	std::string									_reason;
};

/***************************************************************************************/

#endif // _SCHED_VIZUALIZATION_H_

