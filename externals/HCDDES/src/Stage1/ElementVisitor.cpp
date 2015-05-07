/*** Included Header Files ***/
#include "Visitor.h"
#include "Uml.h"
#include "ConfigKeeper.h"
#include "IndexKeeper.h"
#include "MessageKeeper.h"
#include "DepGraph.h"

/*** Compiler Warning Disables ***/
#pragma warning(disable:4996)
#pragma warning(disable:4267)
#pragma warning(disable:4244)

#include <ctype.h>

namespace {
	struct mangle : std::unary_function<char, char> {
		char operator()(const char& c) {
			if (!(islower(c) || isupper(c) || c == '_' || isdigit(c))) {
				return '_';
			}
			return c;
		}
	};

	void SanitizeName(std::string& name) {
		if (!(islower(name[0]) || isupper(name[0]) || name[0] == '_')) {
			name = std::string("_") + name;
		}
		std::transform(name.begin(), name.end(), name.begin(), mangle());
	}
}

/***************************************************************************************/

ElementVisitor::ElementVisitor( Semantics::RootFolder &target ) : 
	AbstractVisitor( target ), _hyperperiodSecs(0.0), _portNum(0), _msgNum(0), _devNum(0) {

	_contextTracker.showContext( ConfigKeeper::Inst().GetContextEnabled() );
	std::cout << "Translating individual model elements..." << std::endl;
}

// Add a string to a comma-separated list
void ElementVisitor::AddToFileList( std::string &fileList, const std::string &element ) {
	if ( fileList.empty() ) {
		fileList = element;
	} else {
		fileList = fileList + string(",") + element;
	}
}

void ElementVisitor::HandleOSEKExecInfo( ESMoL::ExecutionInfo & execinfo, Semantics::ExecInfo & sei )
{
	set< ESMoL::OSEKExecInfo > oeiChildren = execinfo.OSEKExecInfo_kind_children();

	if ( oeiChildren.size() > 0 )
	{
		ESMoL::OSEKExecInfo osekinfo = *oeiChildren.begin();

		Semantics::OsekExecParameters parameters = Semantics::OsekExecParameters::Create( sei );
		Semantics::Preemption preemption = Semantics::Preemption::Create( sei );
		Semantics::Priority priority = Semantics::Priority::Create( sei );

		priority.priority() = osekinfo.Priority();
		preemption.preemption() = (osekinfo.Preemption() == "NON") ? false : true;
		parameters.taskType() = osekinfo.Type();
		parameters.activation() = osekinfo.Activation();
		parameters.autoStart() = osekinfo.AutoStart();
	}

}


void ElementVisitor::Visit_HardwareUnit( const ESMoL::HardwareUnit & hwunit )
{
	// Basic idea:  Collect all IChans and OChans and make sure they each have unique ChanNum values

	set< ESMoL::Node > collect_nodes;
	GetNodes( hwunit, collect_nodes );  // collect all nodes within this hardware design

	// Collect the IChans, OChans, and channel numbers for each
	set< ESMoL::IChan > collect_ichans;
	set< ESMoL::OChan > collect_ochans;
	long long max_ichan_num = 0, max_ochan_num = 0;
	for ( set< ESMoL::Node >::iterator nodeIter = collect_nodes.begin(); nodeIter != collect_nodes.end(); nodeIter++ )
	{
		set< ESMoL::IChan > ichans = (*nodeIter).IChan_kind_children();
		for ( set< ESMoL::IChan >::iterator ichanIter = ichans.begin(); ichanIter != ichans.end(); ichanIter++ )
		{
			collect_ichans.insert( *ichanIter );
			if ( (*ichanIter).ChanNum() > max_ichan_num )
				max_ichan_num = (*ichanIter).ChanNum();
		}
		
		set< ESMoL::OChan > ochans = (*nodeIter).OChan_kind_children();
		for ( set< ESMoL::OChan >::iterator ochanIter = ochans.begin(); ochanIter != ochans.end(); ochanIter++ )
		{
			collect_ochans.insert( *ochanIter );
			if ( (*ochanIter).ChanNum() > max_ochan_num )
				max_ochan_num = (*ochanIter).ChanNum();
		}
	}

	// Go through each set of channel objects and assign unassigned channels
	long long index = max_ichan_num + 1;
	for ( set< ESMoL::IChan >::iterator ichanIter = collect_ichans.begin(); ichanIter != collect_ichans.end(); ichanIter++ )
	{
		if ( (*ichanIter).ChanNum() == -1 )
		{
			(*ichanIter).ChanNum() = index++;
		}
	}

	index = max_ochan_num + 1;
	for ( set< ESMoL::OChan >::iterator ochanIter = collect_ochans.begin(); ochanIter != collect_ochans.end(); ochanIter++ )
	{
		if ( (*ochanIter).ChanNum() == -1 )
		{
			(*ochanIter).ChanNum() = index++;
		}
	}
}


void ElementVisitor::Visit_System( const ESMoL::System & sys ) {

	Semantics::Deployment sDeployment = Semantics::Deployment::Create( _rootFolder );
	_currentDeployment = sDeployment;
	sDeployment.name() = sys.name(); // copy through the name
	IndexKeeper::Inst()->Store( sys.uniqueId(), sDeployment.uniqueId(), sDeployment );

	// Start traversal
	AbstractVisitor::Visit_System( sys );
}

// NodeRef semantics -- If two NodeRefs point to the same Node, then we 
// only instantiate the Node once in the semantics.
void ElementVisitor::Visit_NodeRef( const ESMoL::NodeRef & nodeRef ) {

}

void ElementVisitor::Visit_Node( const ESMoL::Node & eNode ) {

	// Only visit once
	if ( IndexKeeper::Inst()->SeenIndex( eNode.uniqueId() ) ) return;

	Semantics::Node sNode = Semantics::Node::Create( _currentDeployment );
	sNode.name() = eNode.name();
	sNode.id() = eNode.uniqueId();
	sNode.processormodel() = eNode.CPU();
	
	sNode.platformtype() = eNode.PlatformType();
	IndexKeeper::Inst()->Store( eNode.uniqueId(), sNode.uniqueId(), sNode );

	set< ESMoL::TTExecContext > ttecs = eNode.TTExecContext_kind_children();
	if ( ttecs.size() > 0 ) 
	{
		ESMoL::TTExecContext ttec = *ttecs.begin();
		std::string hp = ttec.Hyperperiod();
		_hyperperiodSecs = TimeStr2secs( hp );
	}

	sNode.hyperperiodsecs() = _hyperperiodSecs;

	// Set defaults for things
	sNode.analog_inp_num() = 0;
	sNode.analog_outp_num() = 0;
	sNode.clockspeedhz() = 0.0;
	sNode.tickresolutionsecs() = 0.001;
	sNode.ctxtswitchsecs() = 0.0;
	sNode.sndohdsecs() = 0.0;
	sNode.rcvohdsecs() = 0.0;

	unsigned long long spd = eNode.Speed();
	sNode.clockspeedhz() = 1.0e6 * (double) spd;

	_curSNode = sNode;
}

void ElementVisitor::Visit_OS( const ESMoL::OS & os ) {

	// STRONG ASSUMPTION -- we have already visited the parent and created a semantics entry for it.
	Udm::Object obj = os.parent();
	Semantics::Node sNode = IndexKeeper::Inst()->Lookup< Semantics::Node >( obj.uniqueId() );
	//get os specific values
	std::string str;		
	try {
		str = os.TickResolution();
		sNode.tickresolutionsecs() = TimeStr2secs( str );
	}
	catch ( ConversionErr &err ) {
		std::cout << _contextTracker.GetContext() << " Node resolution :" << err._what << std::endl;
	}
	try {
		str = os.ContextSwitchTime();
		sNode.ctxtswitchsecs() = TimeStr2secs(str);
	}
	catch ( ConversionErr &err ) {
		std::cout << _contextTracker.GetContext() << " Node context switch time :" << err._what << std::endl;
	}
	try {
		str = os.ISROverheadTime();
		sNode.isrohdsecs() = TimeStr2secs(str);
	}
	catch ( ConversionErr &err ) {
		std::cout << _contextTracker.GetContext() << " ISR overhead time :" << err._what << std::endl;
	}
	try {
		str = os.SendOverheadTime();
		sNode.sndohdsecs() = TimeStr2secs(str);
	}
	catch ( ConversionErr &err ) {
		std::cout << _contextTracker.GetContext() << " Node send overhead :" << err._what << std::endl;
	}
	try {
		str = os.RecvOverheadTime();
		sNode.rcvohdsecs() = TimeStr2secs(str);
	}
	catch ( ConversionErr &err ) {
		std::cout << _contextTracker.GetContext() << " Node receive overhead :" << err._what << std::endl;
	}

	sNode.maxtasknum() = os.MaxTaskNumber();
	sNode.schedalgorithm() = os.SchedulingAlgorithm();

	// Adding functionality for OSEK OS
	if (os.type() == ESMoL::OSEK::meta)
	{
		ESMoL::OSEK osek = ESMoL::OSEK::Cast(os);
		Semantics::OSEKParameters parameters = Semantics::OSEKParameters::Create(sNode);

		parameters.Compiler() = osek.Compiler();
		parameters.Conformance() = osek.Conformance();
		parameters.Schedule() = osek.Schedule();
		parameters.Status() = osek.Status();

		ESMoL::Node parent = ESMoL::Node::Cast(os.parent());
		set<ESMoL::OSEKCOM> coms =  parent.OSEKCOM_kind_children();
		if (coms.size() > 0) {
			ESMoL::OSEKCOM com = *(coms.begin());
			parameters.GenerateTask() = com.GenerateTask();
			parameters.CycleTime() = com.CycleTime();
			parameters.HandleNM() = com.HandleNM();
			parameters.HandleCCL() = com.HandleCCL();
			parameters.HandleRX() = com.HandleRX();
			parameters.HandleTX() = com.HandleTX();
		}
	}
}

void ElementVisitor::Visit_BChan( const ESMoL::BChan & bc ) {

	Semantics::CommInterface ci = Semantics::CommInterface::Create( _currentDeployment );
	Semantics::BChan sbc = Semantics::BChan::Create( _currentDeployment );

	string n1 = bc.name();
	n1 += "_dev";
	ci.name() = n1;
	string n2 = bc.name();
	n2 += "_chan";
	sbc.name() = n2;

	ci.id() = bc.uniqueId();
	sbc.id() = bc.uniqueId();
	sbc.chanIndex() = bc.ChanNum();
	ci.localid() = ++_devNum;
	IndexKeeper::Inst()->StorePair( bc.uniqueId(), ci.uniqueId(),ci, sbc.uniqueId(), sbc);

}

void ElementVisitor::Visit_TTBus( const ESMoL::TTBus &ttbus ) {	

	// if we've already been here!
	if (IndexKeeper::Inst()->SeenIndex( ttbus.uniqueId() ) ) return;
	
	Semantics::TTBus b = Semantics::TTBus::Create( _currentDeployment );
	b.id() = ttbus.uniqueId();
	b.name() = ttbus.name();
	IndexKeeper::Inst()->Store( ttbus.uniqueId(), b.uniqueId(), b );

	b.hyperperiodsecs() = _hyperperiodSecs;

	string str;
	try {
		str = ttbus.DataRate();
		b.dataratebps() = RateStr2bps(str);
	}
	catch ( ConversionErr &err ) {
		std::cout << _contextTracker.GetContext();
		std::cout << " Error converting bus data rate (bits per second) from string :" << err._what << std::endl;
	}
	try {
		str = ttbus.Hyperperiod();
		b.hyperperiodsecs() = TimeStr2secs(str);
	}
	catch ( ConversionErr &err ) {
		std::cout << _contextTracker.GetContext();
		std::cout << " Error converting bus hyperperiod (seconds) from string :" << err._what << std::endl;
	}
	try {
		str = ttbus.TTSetupTime();
		b.setupsecs() = TimeStr2secs(str);
	}
	catch ( ConversionErr &err ) {
		std::cout << _contextTracker.GetContext();
		std::cout << " Error converting bus setup time (seconds) from string :" << err._what << std::endl;
	}
	try {
		str = ttbus.SwitchMemorySize();
		b.switchmembytes() = SizeStr2bytes(str);
	}
	catch ( ConversionErr &err ) {
		std::cout << _contextTracker.GetContext();
		std::cout << " Error converting bus memory size (bytes) from string :" << err._what << std::endl;
	}
	try {
		str = ttbus.MinFrameSize();
		b.minframebytes() = SizeStr2bytes(str);
	}
	catch ( ConversionErr &err ) {
		std::cout << _contextTracker.GetContext();
		std::cout << " Error converting bus min frame size (bytes) from string :" << err._what << std::endl;
	}
	try {
		str = ttbus.SlotTime();
		b.busslottimesecs() = TimeStr2secs(str);
	}
	catch ( ConversionErr &err ) {
		std::cout << _contextTracker.GetContext();
		std::cout << " Error converting bus slot time (seconds) from string :" << err._what << std::endl;
	}
	
}

void ElementVisitor::Visit_CANBus( const ESMoL::CANBus & canbus ) {	

	// if we've already been here!
	if (IndexKeeper::Inst()->SeenIndex( canbus.uniqueId() ) ) return;

	Semantics::CANBus b = Semantics::CANBus::Create( _currentDeployment );
	b.id() = canbus.uniqueId();
	b.name() = canbus.name();
	IndexKeeper::Inst()->Store( canbus.uniqueId(), b.uniqueId(), b );

	b.hyperperiodsecs() = _hyperperiodSecs;

	string str;
	try {
		str = canbus.DataRate();
		b.dataratebps() = RateStr2bps(str);
	}
	catch ( ConversionErr &err ) {
		std::cout << _contextTracker.GetContext();
		std::cout << " Error converting bus data rate (bits per second) from string :" << err._what << std::endl;
	}
	try {
		str = canbus.Hyperperiod();
		b.hyperperiodsecs() = TimeStr2secs(str);
	}
	catch ( ConversionErr &err ) {
		std::cout << _contextTracker.GetContext();
		std::cout << " Error converting bus hyperperiod (seconds) from string :" << err._what << std::endl;
	}
	try {
		str = canbus.SetupTime();
		b.setupsecs() = TimeStr2secs(str);
	}
	catch ( ConversionErr &err ) {
		std::cout << _contextTracker.GetContext();
		std::cout << " Error converting bus setup time (seconds) from string :" << err._what << std::endl;
	}
	try {
		str = canbus.SwitchMemorySize();
		b.switchmembytes() = SizeStr2bytes(str);
	}
	catch ( ConversionErr &err ) {
		std::cout << _contextTracker.GetContext();
		std::cout << " Error converting bus memory size (bytes) from string :" << err._what << std::endl;
	}
	try {
		str = canbus.MinFrameSize();
		b.minframebytes() = SizeStr2bytes(str);
	}
	catch ( ConversionErr &err ) {
		std::cout << _contextTracker.GetContext();
		std::cout << " Error converting bus min frame size (bytes) from string :" << err._what << std::endl;
	}

}

void ElementVisitor::Visit_SerialLink( const ESMoL::SerialLink & sl )
{
	// if we've already been here!
	if (IndexKeeper::Inst()->SeenIndex( sl.uniqueId() ) ) return;

	Semantics::SerialLink s = Semantics::SerialLink::Create( _currentDeployment );
	s.id() = sl.uniqueId();
	s.name() = sl.name();
	IndexKeeper::Inst()->Store( sl.uniqueId(), s.uniqueId(), s );

	s.databits() = sl.DataBits();
	s.parity() = sl.Parity();
	s.stopbits() = sl.StopBits();
	s.flowcontrol() = sl.FlowControl();

	string str;
	try {
		str = sl.DataRate();
		s.dataratebps() = RateStr2bps(str);
	}
	catch ( ConversionErr &err ) {
		std::cout << _contextTracker.GetContext();
		std::cout << " Error converting bus data rate (bits per second) from string :" << err._what << std::endl;
	}
	try {
		str = sl.SetupTime();
		s.setupsecs() = TimeStr2secs(str);
	}
	catch ( ConversionErr &err ) {
		std::cout << _contextTracker.GetContext();
		std::cout << " Error converting bus setup time (seconds) from string :" << err._what << std::endl;
	}
	try {
		str = sl.SwitchMemorySize();
		s.switchmembytes() = SizeStr2bytes(str);
	}
	catch ( ConversionErr &err ) {
		std::cout << _contextTracker.GetContext();
		std::cout << " Error converting bus memory size (bytes) from string :" << err._what << std::endl;
	}
	try {
		str = sl.MinFrameSize();
		s.minframebytes() = SizeStr2bytes(str);
	}
	catch ( ConversionErr &err ) {
		std::cout << _contextTracker.GetContext();
		std::cout << " Error converting bus min frame size (bytes) from string :" << err._what << std::endl;
	}
}

void ElementVisitor::Visit_IChan( const ESMoL::IChan & ic ) {

	Semantics::IChan sic = Semantics::IChan::Create( _currentDeployment );
	sic.name() = ic.name();
	sic.id() = ic.uniqueId();
	sic.config() = ic.Configuration();
	sic.chanIndex() = ic.ChanNum();
	IndexKeeper::Inst()->Store( ic.uniqueId(), sic.uniqueId(), sic );
	_inputDevice = true;

	_curSNode.analog_inp_num() = _curSNode.analog_inp_num() + 1;
}

void ElementVisitor::Visit_OChan( const ESMoL::OChan &oc ) {

	Semantics::OChan soc = Semantics::OChan::Create( _currentDeployment );
	soc.name() = oc.name();
	soc.id() = oc.uniqueId();
	soc.config() = oc.Configuration();
	soc.chanIndex() = oc.ChanNum();
	IndexKeeper::Inst()->Store( oc.uniqueId(), soc.uniqueId(), soc );
	_inputDevice = false;

	_curSNode.analog_outp_num() = _curSNode.analog_outp_num() + 1;
}

void ElementVisitor::Visit_IODevice( const ESMoL::IODevice & iod ) {

	if ( IndexKeeper::Inst()->SeenIndex( iod.uniqueId() ) ) 
	{
		//cout << "Already seen IODev " << iod.name() << endl;
		return; // Only visit once
	}

	if ( _inputDevice )
	{
		Semantics::InputDevice id = Semantics::InputDevice::Create( _currentDeployment );
		id.name() = iod.name();
		id.devicetype() = iod.DeviceType();
		id.config() = iod.Configuration();
		id.id() = iod.uniqueId();
		id.localid() = ++_devNum;
		IndexKeeper::Inst()->Store( iod.uniqueId(), id.uniqueId(), id );
		//cout << "Created input device " << iod.name() << endl;
	}
	else
	{
		Semantics::OutputDevice od = Semantics::OutputDevice::Create( _currentDeployment );
		od.name() = iod.name();
		od.devicetype() = iod.DeviceType();
		od.config() = iod.Configuration();
		od.id() = iod.uniqueId();
		od.localid() = ++_devNum;
		IndexKeeper::Inst()->Store( iod.uniqueId(), od.uniqueId(), od );
		//cout << "Created output device " << iod.name() << endl;
	}
}

void ElementVisitor::Visit_Plant( const ESMoL::Plant & epl ) {

	if ( IndexKeeper::Inst()->SeenIndex( epl.uniqueId() ) ) return; // Only visit once
	Semantics::Plant p = Semantics::Plant::Create( _currentDeployment );
	p.name() = epl.name();
	p.id() = epl.uniqueId();
	IndexKeeper::Inst()->Store( epl.uniqueId(), p.uniqueId(), p );
}

// A ComponentRef is an instance of the Component type, so we create one
// semantic object per reference
void ElementVisitor::Visit_ComponentRef( const ESMoL::ComponentRef & cref ) {

	Semantics::Component c = Semantics::Component::Create( _currentDeployment );
	std::string cref_name = cref.name();
	SanitizeName(cref_name);
	c.name() = cref_name;
	c.id() = cref.uniqueId();
	_portNum = 1;
	IndexKeeper::Inst()->Store( cref.uniqueId(), c.uniqueId(), c );

	_curSComp = c;
	_curECompRef = cref;
	
}

void ElementVisitor::Visit_Component( const ESMoL::Component & ecomp ) {
	// Nothing to see here!

	
}


// We don't create semantic message objects here, because they must be
// unique to a communication transaction.  See Visit_Dependency, below.

// One exception -- a totally message with no dependencies still needs to 
// have a semantic message created...
void ElementVisitor::Visit_MessageRef( const ESMoL::MessageRef & mref ) {

	
}


void ElementVisitor::Visit_Message( const ESMoL::Message & msg ) {


//	std::string msgName = msg.name();
//	std::cout << "Message: " << msgName << std::endl;
	// Create a map for the message ports and their msgIndex
	std::set< ESMoL::MsgPort > msgPortSet = msg.MsgPort_children();
	int index = msgPortSet.size() + 1;
	std::map< int, ESMoL::MsgPort > msgPortMap;
	// Iterate through all msgports
	std::set< ESMoL::MsgPort >::iterator msgPortIter = msgPortSet.begin();
	for ( ; msgPortIter != msgPortSet.end(); msgPortIter++ ) {
		// Determine correct msgIndex value - use existing if between 1 and # of msg ports
		int msgIndex = msgPortIter->getIntegerAttr( ESMoL::MsgPort::meta_MsgPortNum );
		if ( msgIndex < 0 || msgIndex > (int)msgPortSet.size() )
			msgIndex = index++;
		// Add msgPorts into the map using their msgIndex value as key
		msgPortMap.insert( std::make_pair( msgIndex, *msgPortIter ) );
	}
	// Now set actual msgIndex values using order in the map
	std::map< int, ESMoL::MsgPort >::iterator mapIter = msgPortMap.begin();
	index = 1;
	for ( ; mapIter != msgPortMap.end(); mapIter++) {
		// Do some debugging output
//		std::string portName = mapIter->second.name();
//		std::cout << "\tMsgPort " << portName << "(" << index << ")\n";
		// Set the msgIndex value for this msgPort
		mapIter->second.setIntegerAttr( ESMoL::MsgPort::meta_MsgPortNum, index++ );
	}

	// If we have not already visited this msg type declaration, create a new MessageType object for it
	if ( !IndexKeeper::Inst()->SeenIndex( msg.uniqueId() ) )
	{
		Semantics::MsgType mt = Semantics::MsgType::Create( _currentDeployment );
		mt.name() = msg.name();
		mt.id() = msg.uniqueId();

		set< ESMoL::MsgPort > msgPorts = msg.MsgPort_children();
		for ( set< ESMoL::MsgPort >::iterator msgIter = msgPorts.begin(); msgIter != msgPorts.end(); msgIter++ )
		{
			Semantics::MsgField f = Semantics::MsgField::Create( _currentDeployment );
			f.id() = (*msgIter).uniqueId();
			f.name() = (*msgIter).name();
			f.msgindex() = (*msgIter).MsgPortNum();

			f.dataType() = (*msgIter).DataType();
			f.dataSign() = (*msgIter).DataSign();
			f.dataScale() = (*msgIter).DataScale();
			f.dataOffset() = (*msgIter).DataOffset();
			f.min() = (*msgIter).Min();
			f.max() = (*msgIter).Max();

			set< ESMoL::FieldDataType > efdt = (*msgIter).FieldDataType_children();
			if ( efdt.size() > 0 )
			{
				ESMoL::FieldDataType fdt = *(efdt.begin());
				f.numbits() = fdt.NumBits();
				f.initialvalue() = std::stod(fdt.InitialValue());
			}
			else
			{
				f.numbits() = (*msgIter).DataSize();
				f.initialvalue() = (*msgIter).DataInit();
			}

			f.fieldMsg() = mt;
			mt.msgFields() += f;
		}

		IndexKeeper::Inst()->Store( msg.uniqueId(), mt.uniqueId(), mt );
	}
}


void ElementVisitor::Visit_MsgPort( const ESMoL::MsgPort & mport ) {
}


void ElementVisitor::Visit_CCode( const ESMoL::CCode & ccode ) {
	// Get the code info block
	std::string codeinfo = ccode.CodeInfo();
	// See if code block is code or name of .c file
	if ( codeinfo.find( ".c" ) != std::string::npos ) {
		// Add to list of c files
		std::string cfiles = _curSComp.cfiles();
		// It's a filename
		AddToFileList( cfiles, codeinfo);
		// Set the new list of c files
		_curSComp.cfiles() = cfiles;
	}
	// Otherwise, we will be generating the code as a component using C_CodeGen
	else {
		// Get the name of the component
		std::string compName = ccode.name();
		// Set the block of code
		_curSComp.ccode() = codeinfo; // it's a code block
		// Set the hfiles name
		std::string hfiles = _curSComp.hfiles();
		AddToFileList( hfiles, compName + ".h" );
		_curSComp.hfiles() = hfiles;
		// Set the cfiles name
		std::string cfiles = _curSComp.cfiles();
		AddToFileList( cfiles, compName + ".c" );
		_curSComp.cfiles() = cfiles;
		// Since we actually call things using msubsystem, we need to fill it in
		_curSComp.msubsystem() = compName;
	}
}


// Input port for C-implemented block
void ElementVisitor::Visit_CInputPort( const ESMoL::CInputPort & cip ) {

	std::string name = string(_curSComp.name()) + "_" + std::string( cip.name() );
	Semantics::Signal s = Semantics::Signal::Create( _currentDeployment );
	s.id() = cip.uniqueId();
	s.name() = name;
	s.callIndex() = _portNum++;
	s.msgIndex() = 0;

	std::string ctype = std::string( cip.CPortType() );
	std::map< std::string, Semantics::SignalType >::iterator ittype 
		= _typesVisited.find( ctype );
	Semantics::SignalType st;
	if ( ittype == _typesVisited.end() ) {
		// We'll need to make a new one
		st = Semantics::SignalType::Create( _currentDeployment );
		st.name() = ctype;
		st.ctypestr() = ctype;
		_typesVisited[ ctype ] = st;
	} else {
		// We'll use the old one
		st = ittype->second;
	}
	IndexKeeper::Inst()->StorePair( MakeCombinedID(_curECompRef.uniqueId(), cip.uniqueId()), s.uniqueId(), s, st.uniqueId(), st );
}

// Output port for C-implemented block
void ElementVisitor::Visit_COutputPort( const ESMoL::COutputPort & cop ) {

	std::string name = string(_curSComp.name()) + "_" + std::string( cop.name() );
	Semantics::Signal s = Semantics::Signal::Create( _currentDeployment );
	s.id() = cop.uniqueId();
	s.name() = name;
	s.callIndex() = _portNum++;

	std::string ctype = std::string( cop.CPortType() );
	std::map< std::string, Semantics::SignalType >::iterator ittype 
		= _typesVisited.find( ctype );
	Semantics::SignalType st;
	if ( ittype == _typesVisited.end() ) {
		// We'll need to make a new one
		st = Semantics::SignalType::Create( _currentDeployment );
		st.name() = ctype;
		st.ctypestr() = ctype;
		_typesVisited[ ctype ] = st;
	} else {
		// We'll use the old one
		st = ittype->second;
	}
	IndexKeeper::Inst()->StorePair( MakeCombinedID(_curECompRef.uniqueId(), cop.uniqueId()), s.uniqueId(), s, st.uniqueId(), st );
}

void ElementVisitor::Visit_SubsystemRef(const ESMoL::SubsystemRef &) {

}

void ElementVisitor::Visit_Subsystem( const ESMoL::Subsystem & subSys )
{
	_curSComp.msubsystem() = subSys.name();
	ostringstream cbasename;
	cbasename << subSys.name() << "_sl.";
	std::string cfiles = _curSComp.cfiles();
	std::string hfiles = _curSComp.hfiles();
	AddToFileList( cfiles, cbasename.str() + string("c"));
	AddToFileList( hfiles, cbasename.str() + string("h"));
	_curSComp.cfiles() = cfiles;
	_curSComp.hfiles() = hfiles;
}

// Input port for Simulink-implemented block
void ElementVisitor::Visit_InputPort( const ESMoL::InputPort & eip ) {

	std::string name = string(_curSComp.name()) + "_" + std::string( eip.name() );
	SanitizeName(name);
	Semantics::Signal s = Semantics::Signal::Create( _currentDeployment );
	s.id() = eip.uniqueId();
	s.name() = name;
	s.callIndex() = eip.Number();
	ESMoL::TypeBaseRef tbr = eip.TypeBaseRef_child();
	ESMoL::TypeBase tb = tbr.ref();
	string mtype;
	if ( !tb ) {
		mtype = "Unknown";
	} else {
		ESMoL::TypeBase tb = tbr.ref();
		mtype = tb.name();
	}

	std::map< std::string, Semantics::SignalType >::iterator ittype 
		= _typesVisited.find( mtype );
	Semantics::SignalType st;
	if ( ittype == _typesVisited.end() ) {
		// We'll need to make a new one
		st = Semantics::SignalType::Create( _currentDeployment );
		st.name() = mtype;
		st.mtypestr() = mtype;
		_typesVisited[ mtype ] = st;
	} else {
		// We'll use the old one
		st = ittype->second;
	}
	IndexKeeper::Inst()->StorePair( MakeCombinedID(_curECompRef.uniqueId(), eip.uniqueId()), s.uniqueId(), s, st.uniqueId(), st );
}


// Output port for Simulink-implemented block
void ElementVisitor::Visit_OutputPort( const ESMoL::OutputPort & eop ) {

	std::string name = string(_curSComp.name()) + "_" + std::string( eop.name() );
	SanitizeName(name);
	Semantics::Signal s = Semantics::Signal::Create( _currentDeployment );
	s.id() = eop.uniqueId();
	s.name() = name;
	s.callIndex() = eop.Number();
	ESMoL::TypeBaseRef tbr = eop.TypeBaseRef_child();
	ESMoL::TypeBase tb = tbr.ref();
	string mtype;
	if ( !tb ) {
		mtype = "Unknown";
	} else {
		mtype = tb.name();
	}
	std::map< std::string, Semantics::SignalType >::iterator ittype 
		= _typesVisited.find( mtype );
	Semantics::SignalType st;
	if ( ittype == _typesVisited.end() ) {
		// We'll need to make a new one
		st = Semantics::SignalType::Create( _currentDeployment );
		st.name() = mtype;
		st.mtypestr() = mtype;
		_typesVisited[ mtype ] = st;
	} else {
		// We'll use the old one
		st = ittype->second;
	}
	IndexKeeper::Inst()->StorePair( MakeCombinedID(_curECompRef.uniqueId(), eop.uniqueId()), s.uniqueId(), s, st.uniqueId(), st );
}

void ElementVisitor::Visit_Connector( const ESMoL::Connector & conn) {

	IndexKeeper * ik = IndexKeeper::Inst();
	ESMoL::Output outp = conn.srcConnector_end();
	ESMoL::Input inp = conn.dstConnector_end();

	if ( outp.type() == ESMoL::MsgPort::meta )
	{
		ESMoL::MsgPort mp = ESMoL::MsgPort::Cast( outp );
		ID_TYPE cid = MakeCombinedID( _curECompRef.uniqueId(), inp.uniqueId() );
		pair< Semantics::Signal, Semantics::SignalType > p = ik->LookupPair< Semantics::Signal, Semantics::SignalType >( cid );
		Semantics::Signal s = p.first;
		if ( mp.MsgPortNum() != -1 ) {
			s.msgIndex() = mp.MsgPortNum();
		}
	}
	else if ( inp.type() == ESMoL::MsgPort::meta )
	{
		ESMoL::MsgPort mp = ESMoL::MsgPort::Cast( inp );
		ID_TYPE cid = MakeCombinedID( _curECompRef.uniqueId(), outp.uniqueId() );
		pair< Semantics::Signal, Semantics::SignalType > p = ik->LookupPair< Semantics::Signal, Semantics::SignalType  >( cid );
		Semantics::Signal s = p.first;
		if ( mp.MsgPortNum() != -1 ) {
			s.msgIndex() = mp.MsgPortNum();
		}
	}
}

void ElementVisitor::Visit_Dependency( const ESMoL::Dependency & dep ) {

	DependencyKeeper * dk = DependencyKeeper::Inst();

	ESMoL::MessageRef smr = dep.srcDependency_end();
	ESMoL::ComponentRef scr = dep.srcDependency__rp_helper();
	ESMoL::MessageRef dmr = dep.dstDependency_end();
	ESMoL::ComponentRef dcr = dep.dstDependency__rp_helper();

	MessageKeeper * mk = MessageKeeper::Inst();
	Semantics::Msg msg;
	ID_TYPE mid;
	if ( !mk->SeenMessage( scr.uniqueId(), smr.uniqueId() ) )
	{
		// Create a new one
		msg = Semantics::Msg::Create( _currentDeployment );
		
		mid = MakeCombinedID( scr.uniqueId(), smr.uniqueId() );
		IndexKeeper::Inst()->Store( mid, msg.uniqueId(), msg );
		mk->AddMessage( scr.uniqueId(), smr.uniqueId(), mid );

		ostringstream sname;
		sname << scr.name() << "." << smr.name(); // << "_" << ++_msgNum;
		msg.name() = sname.str();

		ESMoL::MessageRef_RefersTo_Base mrtb = smr.ref();  // we already checked for null
		ESMoL::Message emsg = ESMoL::Message::Cast( mrtb ); 

		try {
			string str = emsg.MsgSize();
			msg.sizebytes() = (double) SizeStr2bytes( str );
		}
		catch ( ConversionErr & err )
		{
			std::cout << _contextTracker.GetContext() << ":";
			std::cout << "Failed to convert message size from string ";
			std::cout << err._what << endl;
			msg.sizebytes() = 0;
		}
	}
	else
	{
		// Use the old one
		mid = mk->GetMessage( scr.uniqueId(), smr.uniqueId() );
		msg = IndexKeeper::Inst()->Lookup< Semantics::Msg >( mid );
	}

	// Hook up the other end of the dependency to the message, if necessary
	if ( !mk->SeenMessage( dcr.uniqueId(), dmr.uniqueId() ) )
	{
		mk->AddMessage( dcr.uniqueId(), dmr.uniqueId(), mid );
	}

	dk->AddDependency( scr, smr, dcr, dmr );
}

void ElementVisitor::Visit_CommMapping( const ESMoL::CommMapping & ecomm ) {

	IndexKeeper * ik = IndexKeeper::Inst();
	MessageKeeper * mk = MessageKeeper::Inst();

	bool inputconn = false;
	ESMoL::CommMapping_Members_Base secmb = ecomm.srcCommMapping_end();
	ESMoL::CommMapping_Members_Base decmb = ecomm.dstCommMapping_end();

	ESMoL::MessageRef mr_end;
	ESMoL::Channel chan_end;
	ESMoL::ComponentRef cref;
	ESMoL::Node node;

	if ( secmb.type() == ESMoL::MessageRef::meta )
	{
		mr_end = ESMoL::MessageRef::Cast( secmb );
		chan_end = ESMoL::Channel::Cast( decmb );
		cref = ESMoL::ComponentRef::Cast(ecomm.srcCommMapping__rp_helper());
		inputconn = false;  // which direction is the data going?
	}
	else
	{
		mr_end = ESMoL::MessageRef::Cast( decmb );
		chan_end = ESMoL::Channel::Cast( secmb );
		cref = ESMoL::ComponentRef::Cast(ecomm.dstCommMapping__rp_helper());
		inputconn = true;
	}

	node = ESMoL::Node::Cast( chan_end.parent() );

	//cout << "CommMapping endpoints -- chan parent: " << node.name() << " message_ref parent " << cref.name() << endl;
	//cout << " chan name: " << chan_end.name() << " message_ref name: " << mr_end.name() << endl;

	// Handle the two missing message cases: IChan, OChan (no dependencies)
	if( ( chan_end.type() == ESMoL::IChan::meta ) || ( chan_end.type() == ESMoL::OChan::meta ) )
	{
		ID_TYPE mid;
		if ( !mk->SeenMessage( cref.uniqueId(), mr_end.uniqueId() ) )
		{
			// Create a new one
			Semantics::Msg msg = Semantics::Msg::Create( _currentDeployment );
			mid = MakeCombinedID( cref.uniqueId(), mr_end.uniqueId() );
			IndexKeeper::Inst()->Store( mid, msg.uniqueId(), msg );
			mk->AddMessage( cref.uniqueId(), mr_end.uniqueId(), mid );

			ostringstream sname;
			sname << cref.name() << "." << mr_end.name(); // << "_" << ++_msgNum;
			msg.name() = sname.str();

			ESMoL::MessageRef_RefersTo_Base mrtb = mr_end.ref();  // we already checked for null
			ESMoL::Message emsg = ESMoL::Message::Cast( mrtb ); 

			try {
				string str = emsg.MsgSize();
				msg.sizebytes() = SizeStr2bytes( str );
			}
			catch ( ConversionErr & err )
			{
				std::cout << _contextTracker.GetContext() << ":";
				std::cout << "Failed to convert message size from string ";
				std::cout << err._what << endl;
				msg.sizebytes() = 0;
			}
		}
	}
	// Check to see if CommMapping is BChan
	else if ( chan_end.type() == ESMoL::BChan::meta ) {
		// Get the combined ID for the component and message
		ID_TYPE mid = MakeCombinedID( cref.uniqueId(), mr_end.uniqueId() );
		try {
			// Try to lookup the ID
			Semantics::Msg msg = ik->Lookup< Semantics::Msg >( mid );
			// Set the message type to remote
			msg.messagetype() = "Remote";

		} catch (...) {
			// Catch nothing for now
			//exit(1);
		}
	}
}

void ElementVisitor::Visit_TTExecInfo( const ESMoL::TTExecInfo & ttei ) {

	Semantics::ExecInfo sei = Semantics::ExecInfo::Create( _currentDeployment );
	Semantics::SamplePeriod spd = Semantics::SamplePeriod::Create( sei );
	Semantics::TTSchedule tts = Semantics::TTSchedule::Create( sei );
	Semantics::MaxDuration dur = Semantics::MaxDuration::Create( sei );
	Semantics::Deadline dead = Semantics::Deadline::Create( sei );

	sei.name() = ttei.name();
	tts.sched() = ttei.TTSchedule();

	string str;
	try {

		str = ttei.ExecPeriod();
		spd.periodsecs() = TimeStr2secs( str );

		str = ttei.DesiredOffset();
		if ( str.empty() || str.length() == 0 )
		{
			spd.desiredoffsetsecs() = -1.0;
		}
		else
		{
			double off = TimeStr2secs( str );
			spd.desiredoffsetsecs() = off;
		}
	}
	catch ( ConversionErr & err ) {

		std::cout << _contextTracker.GetContext();
		std::cout << " Error reading execution period -- ";
		std::cout << err._what << std::endl;
	}

	try {

		str = ttei.WCDuration();
		cout << ttei.name() << " " << str << " " << TimeStr2secs(str) << endl;
		dur.exectimesecs() = TimeStr2secs( str );
	}
	catch ( ConversionErr & err ) {

		std::cout << _contextTracker.GetContext();
		std::cout << ": error converting string " << err._what;
		std::cout << " to duration value." << std::endl;
	}

	try {

		ESMoL::ExecutionInfo einfo = ESMoL::ExecutionInfo::Cast( ttei );
		
		str = ttei.RelDeadline();
		if ( !str.empty() && str.length() > 0 )
		{
			cout << ttei.name() << " " << str << " " << TimeStr2secs(str) << endl;
			dead.deadlinesecs() = TimeStr2secs( str );
		}
		else
		{
			dead.deadlinesecs() = -1.0;
		}
	}
	catch ( ConversionErr & err ) {

		std::cout << _contextTracker.GetContext();
		std::cout << ": error converting string " << err._what;
		std::cout << " to deadline value." << std::endl;
	}

	HandleOSEKExecInfo( ESMoL::ExecutionInfo::Cast( ttei ), sei );

	IndexKeeper::Inst()->Store( ttei.uniqueId(), sei.uniqueId(), sei );
}

void ElementVisitor::Visit_AsyncPeriodicExecInfo(const ESMoL::AsyncPeriodicExecInfo & apei ) {

	Semantics::ExecInfo sei = Semantics::ExecInfo::Create( _currentDeployment );
	Semantics::SamplePeriod spd = Semantics::SamplePeriod::Create( sei );
	Semantics::MaxDuration dur = Semantics::MaxDuration::Create( sei );
	Semantics::Deadline dead = Semantics::Deadline::Create( sei );

	sei.name() = apei.name();

	string str;
	try {

		str = apei.ExecPeriod();
		spd.periodsecs() = TimeStr2secs( str );
	}
	catch ( ConversionErr & err ) {

		std::cout << _contextTracker.GetContext();
		std::cout << " Reading execution period -- ";
		std::cout << err._what << std::endl;
	}

	try {

		str = apei.TerminationTime();
		spd.terminationsecs() = TimeStr2secs( str );
	}
	catch ( ConversionErr & err ) {

		std::cout << _contextTracker.GetContext();
		std::cout << " Error reading termination time -- ";
		std::cout << err._what << std::endl;
	}
	
	try {

		str = apei.WCDuration();
		cout << apei.name() << " " << str << " " << TimeStr2secs(str) << endl;
		dur.exectimesecs() = TimeStr2secs( str );
	}
	catch ( ConversionErr & err ) {

		std::cout << _contextTracker.GetContext();
		std::cout << ": error converting string " << err._what;
		std::cout << " to duration value." << std::endl;
	}

	try {

		str = apei.RelDeadline();
		cout << apei.name() << " " << str << " " << TimeStr2secs(str) << endl;
		dead.deadlinesecs() = TimeStr2secs( str );
	}
	catch ( ConversionErr & err ) {

		std::cout << _contextTracker.GetContext();
		std::cout << ": error converting string " << err._what;
		std::cout << " to deadline value." << std::endl;
	}

	HandleOSEKExecInfo( ESMoL::ExecutionInfo::Cast( apei ), sei );

	IndexKeeper::Inst()->Store( apei.uniqueId(), sei.uniqueId(), sei );
}

void ElementVisitor::Visit_SporadicExecInfo(const ESMoL::SporadicExecInfo & spei) {

	Semantics::ExecInfo sei = Semantics::ExecInfo::Create( _currentDeployment );
	Semantics::MinimumPeriod smp = Semantics::MinimumPeriod::Create( sei );
	Semantics::SamplePeriod spd = Semantics::SamplePeriod::Create( sei );
	Semantics::MaxDuration dur = Semantics::MaxDuration::Create( sei );
	Semantics::Deadline dead = Semantics::Deadline::Create( sei );

	sei.name() = spei.name();

	string str;
	try {

		str = spei.MinimumPeriod();
		smp.periodsecs() = TimeStr2secs( str );
	}
	catch ( ConversionErr & err ) {

		std::cout << _contextTracker.GetContext();
		std::cout << " Error reading minimum period -- ";
		std::cout << err._what << std::endl;
	}

	try {

		str = spei.WCDuration();
		cout << spei.name() << " " << str << " " << TimeStr2secs(str) << endl;
		dur.exectimesecs() = TimeStr2secs( str );
	}
	catch ( ConversionErr & err ) {

		std::cout << _contextTracker.GetContext();
		std::cout << ": error converting string " << err._what;
		std::cout << " to duration value." << std::endl;
	}

	try {

		str = spei.RelDeadline();
		cout << spei.name() << " " << str << " " << TimeStr2secs(str) << endl;
		dead.deadlinesecs() = TimeStr2secs( str );
	}
	catch ( ConversionErr & err ) {

		std::cout << _contextTracker.GetContext();
		std::cout << ": error converting string " << err._what;
		std::cout << " to deadline value." << std::endl;
	}

	HandleOSEKExecInfo( ESMoL::ExecutionInfo::Cast( spei ), sei );

	IndexKeeper::Inst()->Store( spei.uniqueId(), sei.uniqueId(), sei );
}

void ElementVisitor::Visit_ExecutionAssignment( const ESMoL::ExecutionAssignment & exa ) {

	ESMoL::ExecutionInfo ei = exa.srcExecutionAssignment_end();
	ESMoL::Executable exc = exa.dstExecutionAssignment_end();

	if ( exc.type() == ESMoL::ComponentRef::meta )
	{
		// Then create a task, but only one per execution info object
		ESMoL::ComponentRef cref = ESMoL::ComponentRef::Cast( exc );

		if (!IndexKeeper::Inst()->SeenPairIndex( ei.uniqueId() ) )
		{
			// It's OK to create a Task
			Semantics::Task t = Semantics::Task::Create( _currentDeployment );
			ostringstream nm;
			nm << cref.name(); // << "_" << ei.name() << "_Task";
			t.name() = nm.str();

			Semantics::ExecInfo sei = IndexKeeper::Inst()->Lookup< Semantics::ExecInfo >( ei.uniqueId() );
			//IndexKeeper::Inst()->StorePair( cref.uniqueId(), t.uniqueId(), t, sei.uniqueId(), sei );
			IndexKeeper::Inst()->StorePair( ei.uniqueId(), t.uniqueId(), t, sei.uniqueId(), sei );
		}
	}
}

// This is a little bit hacky, because it creates all of the relations and everything
// here in the element visitor -- decide what to do about it later
void ElementVisitor::Visit_Unused_MsgRefs() {

	// Loop over the remaining msg refs:
	//   Create a local dependency for each one
	//   Look up the connector from the Msg type (to get the input/output orientation)
	//   Hook it up to the proper end of the local dependency

	IndexKeeper * ik = IndexKeeper::Inst();
	MessageKeeper * mk = MessageKeeper::Inst();

	std::map< ESMoL::MessageRef, ESMoL::ComponentRef >::iterator mrefIter;
	for ( mrefIter = _msgRefsLeft.begin(); mrefIter != _msgRefsLeft.end(); mrefIter++ )
	{
		enum Direction { Inbound, Outbound, NA } portDir = NA;
		Semantics::LocalDependency ld = Semantics::LocalDependency::Create( _currentDeployment );
		Semantics::Msg smsg = Semantics::Msg::Create( _currentDeployment );
		string msgname = string( (mrefIter->second).name() ) + string(".") + string( (mrefIter->first).name() );
		smsg.name() = msgname;
		smsg.messagetype() = string("Local");
		try {
			ESMoL::MessageRef_RefersTo_Base mrrtb = (mrefIter->first).ref();
			ESMoL::Message msg = ESMoL::Message::Cast( mrrtb );
			string str = msg.MsgSize();
			smsg.sizebytes() = (double) SizeStr2bytes( str );
		}
		catch ( ConversionErr & err )
		{
			std::cout << "Failed to convert message size from string ";
			std::cout << err._what << endl;
			smsg.sizebytes() = 0;
		}

		ld.depMsg() = smsg;
		smsg.msgDep() = ld;

		ESMoL::MessageRef emsgref = mrefIter->first;
		ESMoL::MessageRef_RefersTo_Base mrrtb = emsgref.ref();
		ESMoL::Message emsg = ESMoL::Message::Cast( mrrtb );
		
		ESMoL::ComponentRef ecref = mrefIter->second;
		ESMoL::ComponentRef_RefersTo_Base ecrrtb = ecref.ref();
		ESMoL::Component ecomp = ESMoL::Component::Cast( ecrrtb );

		ID_TYPE mid = MakeCombinedID( (mrefIter->second).uniqueId(), emsgref.uniqueId() );
		mk->AddMessage( ecref.uniqueId(), emsgref.uniqueId(), mid );
		ik->Store< Semantics::Msg >( mid, smsg.uniqueId(), smsg );

		set< ESMoL::Connector > check_conns = ecomp.Connector_children();
		set< ESMoL::Connector >::iterator connIter;
		for ( connIter = check_conns.begin(); connIter != check_conns.end(); connIter++ )
		{
			ESMoL::Output outp = (*connIter).srcConnector_end();
			ESMoL::Input inp = (*connIter).dstConnector_end();

			if ( inp.type() == ESMoL::MessageRef::meta ) {
				// we're inbound, check to see if this is the msgref
				if ( inp.uniqueId() == emsgref.uniqueId() )
					portDir = Outbound;

			}
			else if ( inp.type() == ESMoL::MsgPort::meta ) {

				// we're inbound, check to see if the parent is the msgref
				ESMoL::MessageRef compare = ESMoL::MessageRef::Cast((*connIter).dstConnector__rp_helper());
				if ( compare.uniqueId() == emsgref.uniqueId() )
					portDir = Outbound;
			}
			else if ( outp.type() == ESMoL::MessageRef::meta ) {
				
				// we're outbound, check to see if this is the msgref
				if (outp.uniqueId() == emsgref.uniqueId() )
					portDir = Inbound;
			}
			else if ( outp.type() == ESMoL::MsgPort::meta ) {

				// we're outbound, check to see if the parent is the msgref
				ESMoL::MessageRef compare = ESMoL::MessageRef::Cast((*connIter).srcConnector__rp_helper());
				if ( compare.uniqueId() == emsgref.uniqueId() )
					portDir = Inbound;
			}

			// Now get the task from the component ref
			set< ESMoL::ExecutionAssignment > exassgns = ecref.srcExecutionAssignment();
			ESMoL::ExecutionAssignment exa = *(exassgns.begin());
			ESMoL::ExecutionInfo einf = exa.srcExecutionAssignment_end();

			pair< Semantics::Task, Semantics::ExecInfo > lkupRslt;
			lkupRslt = ik->LookupPair< Semantics::Task, Semantics::ExecInfo >( einf.uniqueId() );
			Semantics::Task stsk = lkupRslt.first;
			Semantics::ExecInfo seinf = lkupRslt.second;

			// Wire in the node attribute for the dependency
			Semantics::Node n = ld.localDepNode();
			if ( n == Udm::null )
			{
				// This is tricky to get to, because none of the proper relations have been
				// created yet in the semantic domain
				set< ESMoL::ComponentAssignment > cas = ecref.dstComponentAssignment();
				ESMoL::ComponentAssignment ca = *(cas.begin());
				ESMoL::NodeRef enoderef = ca.dstComponentAssignment_end();
				ESMoL::Node enode = enoderef.ref();
				n = ik->Lookup< Semantics::Node >( enode.uniqueId() );
				ld.localDepNode() = n;
				n.nodeLocalDeps() += ld;					
			}

			if ( portDir == Inbound ) {
			
				// This is a receiver context
				ld.localDepReceivingTasks() += stsk;
			}
			else if ( portDir == Outbound ) {

				// This is a sender context
				ld.localDepSendingTask() = stsk;

			}
		}
	}

	// Make sure the FRODO visitor handles local dependencies with dangling ends
}

/***************************************************************************************/

