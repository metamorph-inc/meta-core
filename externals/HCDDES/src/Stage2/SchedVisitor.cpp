/*** Warning Workaround ***/
#pragma warning( disable: 4251 )


/*** Included Header Files ***/
#include "SchedVisitor.h"
#include "ConfigKeeper.h"
#include "Uml.h"

#include <sstream>

using namespace std;

/*** Automatic registration of Sched interpreter ***/
bool SchedVisitor::_initialized = 
	VisitorFactory::RegisterInstantiator( std::string("Sched"), &SchedVisitor::GetSchedVisitor );


/***************************************************************************************/


std::string SchedVisitor::GetQualifiedName( const Semantics::Task &task ) {
	
	if ( task != Udm::null )
	{
		ostringstream out;
		Semantics::Node n = task.executedBy();
		out << n.name() << "/" << task.name();
		return out.str();
	}
	return string("");
}


SchedVisitor::SchedVisitor( Semantics::RootFolder &rootFolder, TimeStamp &timeStamp) : 
	AbstractVisitor(rootFolder, timeStamp) {
}


AbstractVisitor * GetSchedVisitor( Semantics::RootFolder &rootFolder, TimeStamp &timeStamp ) { 
	return new SchedVisitor( rootFolder, timeStamp );
}


void SchedVisitor::Visit_RootFolder( const Semantics::RootFolder &rootFolder ) {
	this->GetMainDictionary().SetValue( "APPNAME", std::string( rootFolder.name() ) );
	this->GetMainDictionary().SetValue( "GENTIME", this->_datetime.GetDateTime() );
	this->GetMainDictionary().SetValue( "RESOLUTION", ConfigKeeper::inst().GetResolution() );	

	DeploymentVector deps = rootFolder.Deployment_children();
	for (DeploymentVector::iterator idep = deps.begin(); idep != deps.end(); idep++) {
		(*idep).Accept(*this);
	}

	

	std::string tmpl_fname = ConfigKeeper::inst().GetTemplatePath() + "\\sched.tpl";
	ifstream inputfile( tmpl_fname.c_str() );
	if ( !inputfile )
	{
		throw std::exception( "Unable to find the template file sched.tpl.  Please make sure HCDDES_CONFIG_PATH is set." );
	}
	inputfile.close();
	ctemplate::Template *googleTemplate = ctemplate::Template::GetTemplate( tmpl_fname, ctemplate::DO_NOT_STRIP );
	std::string output;
	googleTemplate->Expand( &output, &this->GetMainDictionary() );

	// Get the name of the output file
	ConfigKeeper &cfg = ConfigKeeper::inst();
	std::string outputFileName = cfg.GetOutfileName();
	ofstream outputFile( outputFileName.c_str() );
	outputFile << output;
	outputFile.close();
}


void SchedVisitor::Visit_Deployment( const Semantics::Deployment &dep ) {
	NodeVector nodes = dep.Node_children();
	for (NodeVector::iterator inode = nodes.begin(); inode != nodes.end(); inode++) {
		//std::cout << "Node " << (*inode).name() << std::endl;
		_curNode = *inode;

		// Do the stuff
		this->AddSectionDictionary( "HOST_SECTION" );
		this->GetTemplateDictionary().SetValue( "NODENAME", std::string((*inode).name()) );
		this->GetTemplateDictionary().SetValue( "NODEFREQ", Hz2freqStr((*inode).clockspeedhz()) );
		this->GetTemplateDictionary().SetValue( "SENDOHD", Secs2timeStr((*inode).sndohdsecs()) );
		this->GetTemplateDictionary().SetValue( "RECVOHD", Secs2timeStr((*inode).rcvohdsecs()) );

		// Visit the tasks
		TaskSet taskSet = (*inode).executes();
		TaskVector tasks( taskSet.begin(), taskSet.end() );
		for ( TaskVector::iterator taskIter = tasks.begin(); taskIter != tasks.end(); taskIter++ ) {

			// Process tasks that have a TT schedule object
			if ( HasTTSched( *taskIter ) ) {
				//std::cout << "Task " << (*taskIter).name() << std::endl;
				(*taskIter).Accept(*this);
			}	
		}

		// Visit the devices and such
		DeviceSet devs = (*inode).integrates();
		for ( DeviceSet::iterator devIter = devs.begin(); devIter != devs.end(); devIter++ )
		{
			//std::cout << "Device " << (*devIter).name() << std::endl;
			// Process devices that have a TT schedule object
			if ( HasTTSched( *devIter ) )
			{
				if ( (*devIter).type() == Semantics::InputDevice::meta )
				{
					Semantics::InputDevice id = Semantics::InputDevice::Cast( *devIter );
					_curInputDevice = id;
					id.Accept(*this);

					IChanSet ichans = id.inputChannels();
					for ( IChanSet::iterator ichanIter = ichans.begin(); 
						  ichanIter != ichans.end(); ichanIter++ )
					{
						Semantics::Acquisition acq = (*ichanIter).channelAcquisition();
						if ( acq != Udm::null )
						{
							acq.Accept(*this);
						}
					}
				}
				else if ( (*devIter).type() == Semantics::OutputDevice::meta )
				{
					Semantics::OutputDevice od = Semantics::OutputDevice::Cast( *devIter );
					_curOutputDevice = od;
					od.Accept(*this);

					OChanSet ochans = od.outputChannels();
					for ( OChanSet::iterator ochanIter = ochans.begin();
						  ochanIter != ochans.end(); ochanIter++ )
					{
						ActuationSet actsSet = (*ochanIter).channelActuations();
						ActuationVector acts( actsSet.begin(), actsSet.end() );
						for ( ActuationVector::iterator actIter = acts.begin(); 
							  actIter != acts.end(); actIter++ )
						{
							(*actIter).Accept(*this);
						}
					}
				}
			}
		}

		// Local dependencies
		LocalDependencySet localdepsSet = (*inode).nodeLocalDeps();
		LocalDependencyVector localdeps( localdepsSet.begin(), localdepsSet.end() );
		for ( LocalDependencyVector::iterator ldepIter = localdeps.begin(); ldepIter != localdeps.end(); ldepIter++ )
		{
			// TODO: visit the dependency and create the local message to go with it
			(*ldepIter).Accept(*this);
		}

		this->PopSectionDictionary();
	}

	BusSet buses;
	TTBusVector ttbuses = dep.TTBus_kind_children();
	for ( TTBusVector::iterator ttbIter = ttbuses.begin(); ttbIter != ttbuses.end(); ttbIter++ )
	{
		if ( *ttbIter != Udm::null )
			buses.insert( Semantics::CommMedium::Cast( *ttbIter ) );
	}

	CANBusVector canbuses = dep.CANBus_kind_children();
	for ( CANBusVector::iterator canbIter = canbuses.begin(); canbIter != canbuses.end(); canbIter++ )
	{
		if ( *canbIter != Udm::null )
			buses.insert( Semantics::CommMedium::Cast( *canbIter ) );
	}
	
	//for ( TTBusVector::iterator busIter = buses.begin(); busIter != buses.end(); busIter++ ) {
	for ( BusSet::iterator busIter = buses.begin(); busIter != buses.end(); busIter++ ) {
		//std::cout << "Bus " << (*busIter).name() << std::endl;

		this->AddSectionDictionary( "BUS_SECTION" );
		this->GetTemplateDictionary().SetValue( "BUSNAME", std::string( (*busIter).name() ) );
		this->GetTemplateDictionary().SetValue( "BUSRATE", Bps2rateStr( (*busIter).dataratebps() ) );
		this->GetTemplateDictionary().SetValue( "SETUPTIME", Secs2timeStr( (*busIter).setupsecs() ) );

		CommInterfaceSet cifaces = (*busIter).interfaces();
		if ( !cifaces.empty() )
		{
			for ( CommInterfaceSet::iterator cifaceIter = cifaces.begin(); cifaceIter != cifaces.end(); cifaceIter++ )
			{
				this->AddSectionDictionary( "BUS_HOST_SECTION" );
				Semantics::Node n = (*cifaceIter).integratedBy();
				this->GetTemplateDictionary().SetValue( "NODENAME", std::string( n.name() ) );
				this->PopSectionDictionary();	
			}	
		}

		if ( (*busIter).type() == Semantics::TTBus::meta )
		{
			AddSectionDictionary("TT_SECTION");
			string syncname = string((*busIter).name()) + string("Sync");
			GetTemplateDictionary().SetValue( "SYNCMSGNAME", syncname );
			PopSectionDictionary();
		}
		
		// Messages
		MsgVector msgs;
		IfaceSet ifaces = (*busIter).interfaces();
		for ( IfaceSet::iterator ifaceIter = ifaces.begin(); ifaceIter != ifaces.end(); ifaceIter++ ) {
			// Then loop over the channels and accumulate all of the messages
			//cout << "Interface " << (ifaceIter->name()) << endl;
			BChanSet bchans = (*ifaceIter).commChannels();
			for ( BChanSet::iterator bChanIter = bchans.begin(); bChanIter != bchans.end(); bChanIter++ ) {

				//cout << "Bus chan " << (*bChanIter).name();
				TransmitsSet trans = (*bChanIter).channelTransmitters();
				for ( TransmitsSet::iterator transmitsIter = trans.begin(); transmitsIter != trans.end(); transmitsIter++ ) {
					//Semantics::Task st = (*transmitsIter).sendingTask();
					//Semantics::Msg m = (*transmitsIter).transmittedMsg();
					//cout << "Transmitter " << st.name() << " msg " << m.name() << endl;
					msgs.push_back( (*transmitsIter).transmittedMsg() );
				}
			}
		}
		
		// Finally iterate over the messages and process those with TT messages
		for ( MsgVector::iterator msgIter = msgs.begin(); msgIter != msgs.end(); msgIter++ ) {

			if ( HasTTSched( *msgIter ) ) {
				//std::cout << "Msg " << (*msgIter).name() << std::endl;
				(*msgIter).Accept(*this);
			}
		}
	}
}

void SchedVisitor::Visit_Task( const Semantics::Task & ttTask ) {
	this->AddSectionDictionary( "TASK_SECTION" );
	this->GetTemplateDictionary().SetValue( "TASKNAME", std::string( ttTask.name() ) );
	double period = GetSampPeriod( ttTask );
	double freq = (period < 1.0e-12) ? 0.0 : (1.0/period);
	this->GetTemplateDictionary().SetValue( "FREQUENCY", Hz2freqStr( freq ) );
	this->GetTemplateDictionary().SetValue( "WCEXECTIME", Secs2timeStr( GetMaxDuration(ttTask) ) );
	double off = GetDesiredOffset( ttTask );

	if ( off >= 0.0 )
	{
		AddSectionDictionary( "OFFSET_SECTION" );
		GetTemplateDictionary().SetValue( "OFFSETTIME", Secs2timeStr(off) );
		PopSectionDictionary();
	}

	this->PopSectionDictionary();
}

void SchedVisitor::Visit_InputDevice( const Semantics::InputDevice & id ) {

	this->AddSectionDictionary( "TASK_SECTION" );
	this->GetTemplateDictionary().SetValue( "TASKNAME", std::string( id.name() ) );
	double period = GetSampPeriod( id );
	double freq = (period < 1.0e-12) ? 0.0 : (1.0/period);
	this->GetTemplateDictionary().SetValue( "FREQUENCY", Hz2freqStr( freq ) );
	this->GetTemplateDictionary().SetValue( "WCEXECTIME", Secs2timeStr( GetMaxDuration( id ) ) );
	this->PopSectionDictionary();
}

void SchedVisitor::Visit_OutputDevice( const Semantics::OutputDevice & od ) {

	this->AddSectionDictionary( "TASK_SECTION" );
	this->GetTemplateDictionary().SetValue( "TASKNAME", std::string( od.name() ) );
	double period = GetSampPeriod( od );
	double freq = (period < 1.0e-12) ? 0.0 : (1.0/period);
	this->GetTemplateDictionary().SetValue( "FREQUENCY", Hz2freqStr( freq ) );
	this->GetTemplateDictionary().SetValue( "WCEXECTIME", Secs2timeStr( GetMaxDuration( od ) ) );
	this->PopSectionDictionary();
}

void SchedVisitor::Visit_Msg(const Semantics::Msg & ttMsg ) {

	this->AddSectionDictionary( "MSG_SECTION" );
	this->GetTemplateDictionary().SetValue( "MSGNAME", std::string( ttMsg.name() ) );
	this->GetTemplateDictionary().SetValue( "MSGSIZE", Bytes2sizeStr( ttMsg.sizebytes() ) );

	Semantics::Transmits trans = ttMsg.msgTransmitter();
	Semantics::Task tsk = trans.sendingTask();
	this->GetTemplateDictionary().SetValue( "SENDTASK", this->GetQualifiedName( tsk ) );
	ostringstream rcvrs;
	ReceivesSet rcvrset = ttMsg.msgListeners();

	for ( ReceivesSet::iterator receivesIter = rcvrset.begin(); receivesIter != rcvrset.end(); receivesIter++ ) {
		tsk = (*receivesIter).receivingTask();
		rcvrs << this->GetQualifiedName(tsk) << " ";
	}
	this->GetTemplateDictionary().SetValue( "RECVTASKS", rcvrs.str() );	

	double off = GetDesiredOffset( ttMsg );

	if ( off >= 0.0 )
	{
		AddSectionDictionary( "OFFSET_SECTION" );
		GetTemplateDictionary().SetValue( "OFFSETTIME", Secs2timeStr(off) );
		PopSectionDictionary();
	}

	this->PopSectionDictionary();
}

void SchedVisitor::Visit_Acquisition(const Semantics::Acquisition & acq )
{
	// use _curInputDevice
	Semantics::Msg m = acq.acquiredMsg();
	TaskSet taskSet = acq.acquiringTasks(); // device is sender, tasks are receivers
	TaskVector tasks( taskSet.begin(), taskSet.end() );
	this->AddSectionDictionary( "LOCAL_MSG_SECTION" );
	this->GetTemplateDictionary().SetValue( "MSGNAME", std::string( m.name() ) );
	this->GetTemplateDictionary().SetValue( "MSGSIZE", Bytes2sizeStr( m.sizebytes() ) );

	ostringstream sndrname;
	sndrname << _curNode.name() << "/" << _curInputDevice.name();
	this->GetTemplateDictionary().SetValue( "SENDTASK", sndrname.str() );

	ostringstream rcvrs;
	for ( TaskVector::iterator receivesIter = tasks.begin(); receivesIter != tasks.end(); receivesIter++ ) {
		rcvrs << this->GetQualifiedName(*receivesIter) << " ";
	}
	this->GetTemplateDictionary().SetValue( "RECVTASKS", rcvrs.str() );	

	this->PopSectionDictionary();
}

void SchedVisitor::Visit_Actuation(const Semantics::Actuation & act )
{
	Semantics::Msg m = act.actuatingMsg();
	Semantics::Task t = act.actuatingTask(); // task is sender, device is receiver
	this->AddSectionDictionary( "LOCAL_MSG_SECTION" );
	
	this->GetTemplateDictionary().SetValue( "MSGNAME", std::string( m.name() ) );
	this->GetTemplateDictionary().SetValue( "MSGSIZE", Bytes2sizeStr( m.sizebytes() ) );
	this->GetTemplateDictionary().SetValue( "SENDTASK", this->GetQualifiedName( t ) );

	ostringstream rcvrname;
	rcvrname << _curNode.name() << "/" << _curOutputDevice.name();
	this->GetTemplateDictionary().SetValue( "RECVTASKS", rcvrname.str() );	
	
	this->PopSectionDictionary();
}

void SchedVisitor::Visit_LocalDependency( const Semantics::LocalDependency & ldep )
{
	Semantics::Msg m = ldep.depMsg();
	Semantics::Task st = ldep.localDepSendingTask();

	ostringstream rcvrs;
	TaskSet taskSet = ldep.localDepReceivingTasks();
	TaskVector rtasks( taskSet.begin(), taskSet.end() );
	for ( TaskVector::iterator rtIter = rtasks.begin(); rtIter != rtasks.end(); rtIter++ )
	{
		rcvrs << this->GetQualifiedName(*rtIter) << " ";
	}

	// If no sender or no receivers, then do not generate a local dependency entry
	if ( st != Udm::null  && rtasks.size() > 0 )
	{
		this->AddSectionDictionary( "LOCAL_MSG_SECTION" );
		this->GetTemplateDictionary().SetValue( "MSGNAME", std::string( m.name() ) );
		this->GetTemplateDictionary().SetValue( "MSGSIZE", Bytes2sizeStr( m.sizebytes() ) );
		this->GetTemplateDictionary().SetValue( "SENDTASK", this->GetQualifiedName( st ) );
		this->GetTemplateDictionary().SetValue( "RECVTASKS", rcvrs.str() );	
		this->PopSectionDictionary();
	}
}


/***************************************************************************************/

/*  Original code from schedGen

ctemplate::TemplateDictionary * d2 = d1->AddSectionDictionary( "MSG_SECTION" );
d2->SetValue( "MSGNAME", m.basename );
d2->SetValue( "MSGSIZE", m.msgsize );
d2->SetValue( "SENDTASK", m.sender );
d2->SetValue( "RECVTASKS", m.receivers );
}
}

// Latencies
if ( _latencyinfo.size() > 0 )
{
std::vector< LatencyInfo >::iterator pLatency;
for ( pLatency = _latencyinfo.begin(); pLatency != _latencyinfo.end(); pLatency++ )
{
ctemplate::TemplateDictionary * d1 = dict.AddSectionDictionary( "LATENCY_SECTION" );
d1->SetValue( "LATENCY", pLatency->latency );
d1->SetValue( "SENDTASK", pLatency->sender );
d1->SetValue( "RECVTASK", pLatency->receiver );
}
}

// Now write everything out...
ctemplate::Template * tpl = ctemplate::Template::GetTemplate( "SchedTemplates\\sched.tpl", ctemplate::DO_NOT_STRIP );

std::string output;
tpl->Expand( &output, &dict );

std::string filename(LOCAL_OUTPUT_PATH);
filename += "\\" + _designname + "_" + _gentime + ".scs";
ofstream ofile( filename.c_str() );
ofile << output;
ofile.close();

std::string logfilename(LOCAL_OUTPUT_PATH);
logfilename += "\\SchedGenLog.txt";
ofstream logfile( logfilename.c_str() );
logfile << _logstr;
logfile.close();
}

void SchedGen::GenerateSched()
{
if ( _mkdir( LOCAL_OUTPUT_PATH ) != 0 )
if ( errno != EEXIST )
return;

x ctemplate::TemplateDictionary dict( "sched" );

x // The good stuff happens here!
x dict.SetValue( "APPNAME", _designname );
x dict.SetValue( "GENTIME", _gentime );
x dict.SetValue( "RESOLUTION", "1 us" );

// Nodes and tasks
std::map< std::string, NodeInfo >::iterator pNode;
for ( pNode = _nodeinfo.begin(); pNode != _nodeinfo.end(); pNode++ )
{
ctemplate::TemplateDictionary * d1 = dict.AddSectionDictionary( "HOST_SECTION" );
d1->SetValue( "NODENAME", pNode->first );
d1->SetValue( "NODEFREQ", (pNode->second).speed );
d1->SetValue( "SENDOHD", (pNode->second).sendohd );
d1->SetValue( "RECVOHD", (pNode->second).recvohd );

pair<std::multimap<std::string, std::string>::iterator,std::multimap<std::string, std::string>::iterator > range;
std::multimap<std::string, std::string>::iterator pPair;

range = _nodetasks.equal_range( pNode->first ); // All tasks for this node

for ( pPair = range.first; pPair != range.second; pPair++ )
{
TaskInfo t = _taskinfo[ pPair->second ];
ctemplate::TemplateDictionary * d2 = d1->AddSectionDictionary( "TASK_SECTION" );
d2->SetValue( "TASKNAME", t.basename );
d2->SetValue( "FREQUENCY", t.freq );
d2->SetValue( "WCEXECTIME", t.wcexectime );
}
}

// Buses and messages
std::map< std::string, BusInfo >::iterator pBus;
for ( pBus = _businfo.begin(); pBus != _businfo.end(); pBus++ )
{
ctemplate::TemplateDictionary * d1 = dict.AddSectionDictionary( "BUS_SECTION" );
d1->SetValue( "BUSNAME", pBus->first );
d1->SetValue( "BUSRATE", pBus->second.rate );
d1->SetValue( "SETUPTIME", pBus->second.setuptime );

// All msgs for this bus
std::set< std::string > busMsgs = _busmsgs[pBus->first];
std::set< std::string >::iterator pMsgs;
for ( pMsgs = busMsgs.begin(); pMsgs != busMsgs.end(); pMsgs++ )
{
MsgInfo m = _msginfo[ *pMsgs ];

*/
