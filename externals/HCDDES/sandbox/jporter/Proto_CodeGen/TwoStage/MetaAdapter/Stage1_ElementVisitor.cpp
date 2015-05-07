#include "Stage1_Visitor.h"
#include "utils.h"
#include "Uml.h"

#include "IndexKeeper.h"

#include <sstream>

#pragma warning(disable:4996)
#pragma warning(disable:4267)
#pragma warning(disable:4244)

using namespace std;

// portnum starts at 100000000 -- that should prevent conflicts (hopefully)
Stage1_ElementVisitor::Stage1_ElementVisitor(Semantics::RootFolder & target) : 
	Stage1_AbstractVisitor( target ), _hyperperiodsecs(0.0), _portnum(100000000)
{
	cout << "Translating individual model elements..." << endl;
}

// Add a string to a comma-separated list
void Stage1_ElementVisitor::addToFileList( std::string & fileList, const std::string & elt )
{
	if ( fileList.empty() )
	{
		fileList = elt;
	}
	else
	{
		fileList = fileList + string(",") + elt;
	}

}

void Stage1_ElementVisitor::Visit_Deployment(const ESMoL::Deployment& deploy)
{
	Semantics::Deployment dpl = Semantics::Deployment::Create( _RootFolder );
	_curDeployment = dpl;

	if (deploy==Udm::null) // @todo: throw an exception?
		return;

	IndexKeeper::inst()->store( deploy.uniqueId(), dpl.uniqueId(), dpl );

	string str;
	try
	{
		str = deploy.Hyperperiod();
		_hyperperiodsecs = timeStr2secs(str);
		cout << "Hyperperiod: " << _hyperperiodsecs << " secs" << endl;
	}
	catch ( conversionErr & err )
	{
		if ( str.empty() )
		{
			cout << _ct.getContext() << ": No hyperperiod specified!" << endl;
		}
		else
		{
			cout << _ct.getContext() << ":" << err._what << endl;
		}
	}

	// This is an odd circumstance, but we need to create a dummy plant
	Semantics::Plant p = Semantics::Plant::Create( dpl );
	p.id() = deploy.uniqueId();
	p.name() = string( "Plant" );

	Stage1_AbstractVisitor::Visit_Deployment( deploy );
}

// NodeRef semantics -- If two NodeRefs point to the same Node, then we 
// only instantiate the Node once in the semantics.
void Stage1_ElementVisitor::Visit_NodeRef(const ESMoL::NodeRef & nodeRef)
{
	ESMoL::Node nodeObj = nodeRef.ref();
	ESMoL::OS os = nodeObj.OS_child();

	if ( IndexKeeper::inst()->seenIdx( nodeObj.uniqueId() ) ) // only visit once
		return;

	Semantics::Node n = Semantics::Node::Create(_curDeployment);
	n.name() = nodeRef.name();
	n.id() = nodeObj.uniqueId();
	n.platformtype() = nodeObj.CPU();

	IndexKeeper::inst()->store( nodeObj.uniqueId(), n.uniqueId(), n );

	n.hyperperiodsecs() = _hyperperiodsecs;
	n.analog_inp_num() = 0;
	n.analog_outp_num() = 0;
	n.clockspeedhz() = 0.0;
	n.tickresolutionsecs() = 0.001;
	n.ctxtswitchsecs() = 0.0;
	n.sndohdsecs() = 0.0;
	n.rcvohdsecs() = 0.0;

	unsigned long long spd = nodeObj.Speed();
	n.clockspeedhz() = 1.0e6 * (double) spd;

	//get os specific values
	if (!(os ==Udm::null))
	{
		string str;		

		try 
		{
			str = os.TickResolution();
			n.tickresolutionsecs() = timeStr2secs(str);
		}
		catch ( conversionErr & err )
		{
			cout << _ct.getContext() << " Node resolution :" << err._what << endl;
		}

		try 
		{
			str = os.ContextSwitchTime();
			n.ctxtswitchsecs() = timeStr2secs(str);
		}
		catch ( conversionErr & err )
		{
			cout << _ct.getContext() << " Node context switch time :" << err._what << endl;
		}

		try 
		{
			str = os.SendOverheadTime();
			n.sndohdsecs() = timeStr2secs(str);
		}
		catch ( conversionErr & err )
		{
			cout << _ct.getContext() << " Node send overhead :" << err._what << endl;
		}

		try 
		{
			str = os.RecvOverheadTime();
			n.rcvohdsecs() = timeStr2secs(str);
		}
		catch ( conversionErr & err )
		{
			cout << _ct.getContext() << " Node receive overhead :" << err._what << endl;
		}
	}
}

void Stage1_ElementVisitor::Visit_TTTask(const ESMoL::TTTask & tttask)
{
	Semantics::TTTask t = Semantics::TTTask::Create( _curDeployment );
	t.name() = tttask.name();
	t.id() = tttask.uniqueId();

	IndexKeeper::inst()->store( tttask.uniqueId(), t.uniqueId(), t );
	try
	{
		string str = tttask.TTExecTime();
		t.wcetsecs() = timeStr2secs(str);
		str = tttask.TTFreq();
		double frq = freqStr2hz(str);
		if ( frq > 1e-6 )
			t.samplesecs() = 1.0/frq;
		else
			t.samplesecs() = 1.0; // Doesn't make sense! (but doesn't crash)
	}
	catch ( conversionErr & err )
	{
		cout << _ct.getContext() << ":" << err._what << endl;
	}

	t.schedule() = tttask.TTSchedule();
}

void Stage1_ElementVisitor::Visit_TTMessage(const ESMoL::TTMessage& ttmsg)
{
	// Make sure we only visit this once
	if ( !IndexKeeper::inst()->seenIdx( ttmsg.uniqueId() ) )
	{
		Semantics::TTMsg m = Semantics::TTMsg::Create(_curDeployment);
		m.name() = ttmsg.name();
		m.id() = ttmsg.uniqueId();

		IndexKeeper::inst()->store( ttmsg.uniqueId(), m.uniqueId(), m );
		//msgmap[ttmsg.uniqueId()] = m.uniqueId();

		string str = ttmsg.MsgSize();
		m.sizebytes() = sizeStr2bytes( str );

		m.schedule() = ttmsg.TTSchedule();
	}
}

void Stage1_ElementVisitor::Visit_MessageRef(const ESMoL::MessageRef & mref)
{
	ESMoL::Message m = mref.ref();  // We already checked for null...

	if ( m.type() == ESMoL::TTMessage::meta ) {
		ESMoL::TTMessage ttm = ESMoL::TTMessage::Cast( m );
		ttm.Accept( *this );
	}
}

// Component semantics -- Instances are explicit in the architecture model,
// so each reference relation (Component->ComponentBase) refers to a unique
// entity.  Hmmm... is that the best way to do it?  It will have to do for now.
void Stage1_ElementVisitor::Visit_Component(const ESMoL::Component & ecomp)
{
	Semantics::Component c = Semantics::Component::Create( _curDeployment );
	
	c.name() = ecomp.name();
	c.id() = ecomp.uniqueId();

	_portnum = 1;

	ESMoL::ComponentBase compBase = ecomp.ref();
	IndexKeeper::inst()->store( compBase.uniqueId(), c.uniqueId(), c );

	if (compBase.type() == ESMoL::SubsystemRef::meta) {
		compBase = ESMoL::SubsystemRef::Cast(compBase).ref();
	} // dereference once, if necessary

	if (compBase.type() == ESMoL::CCode::meta) {
		ESMoL::CCode ccode = ESMoL::CCode::Cast(compBase);
		
		std::string codeinfo = ccode.CodeInfo();
		if ( codeinfo.find( ".c" ) != std::string::npos )
		{
			string cfiles = c.cfiles();
			addToFileList( cfiles, codeinfo); // it's a filename
			c.cfiles() = cfiles;
		}
		else
		{
			c.ccode() = codeinfo; // it's a code block
		}
	}
	else if (compBase.type() == ESMoL::Subsystem::meta) 
	{
		ESMoL::Subsystem subSys = ESMoL::Subsystem::Cast(compBase);
	 
		c.msubsystem() = subSys.name();
		ostringstream cbasename;
		cbasename << subSys.name() << "_sl.";
		string cfiles = c.cfiles();
		string hfiles = c.hfiles();
		addToFileList( cfiles, cbasename.str() + string("c"));
		addToFileList( hfiles, cbasename.str() + string("h"));
		c.cfiles() = cfiles;
		c.hfiles() = hfiles;
	}
}

// Input port for C-implemented block
void Stage1_ElementVisitor::Visit_CInputPort(const ESMoL::CInputPort & eip)
{
	std::string name = _ctxtName + "_" + string(eip.name());
	Semantics::Signal s = Semantics::Signal::Create( _curDeployment );
	s.id() = eip.uniqueId();
	s.name() = name;
	s.num() = _portnum++;
	s.fnum() = 0;

	string ctype = string( eip.CPortType() );
	std::map< std::string, Semantics::SignalType >::iterator ittype 
		= _typesVisited.find( ctype );
	Semantics::SignalType st;
	if ( ittype == _typesVisited.end() )
	{
		// We'll need to make a new one
		st = Semantics::SignalType::Create( _curDeployment );
		st.name() = ctype;
		st.ctypestr() = ctype;
		_typesVisited[ ctype ] = st;
	}
	else
	{
		// We'll use the old one
		st = ittype->second;
	}

	IndexKeeper::inst()->store_pair( eip.uniqueId(), s.uniqueId(), s, st.uniqueId(), st );
}

// Output port for C-implemented block
void Stage1_ElementVisitor::Visit_COutputPort(const ESMoL::COutputPort & eop)
{
	std::string name = _ctxtName + "_" + string(eop.name());
	Semantics::Signal s = Semantics::Signal::Create( _curDeployment );
	s.id() = eop.uniqueId();
	s.name() = name;
	s.num() = _portnum++;

	string ctype = string( eop.CPortType() );
	std::map< std::string, Semantics::SignalType >::iterator ittype 
		= _typesVisited.find( ctype );
	Semantics::SignalType st;
	if ( ittype == _typesVisited.end() )
	{
		// We'll need to make a new one
		st = Semantics::SignalType::Create( _curDeployment );
		st.name() = ctype;
		st.ctypestr() = ctype;
		_typesVisited[ ctype ] = st;
	}
	else
	{
		// We'll use the old one
		st = ittype->second;
	}

	IndexKeeper::inst()->store_pair( eop.uniqueId(), s.uniqueId(), s, st.uniqueId(), st );
}

// Input port for Simulink-implemented block
void Stage1_ElementVisitor::Visit_InputPort(const ESMoL::InputPort & eip)
{
	std::string name = _ctxtName + "_" + string(eip.name());
	Semantics::Signal s = Semantics::Signal::Create( _curDeployment );
	s.id() = eip.uniqueId();
	s.name() = name;
	s.num() = eip.Number();

	ESMoL::TypeBaseRef tbr = eip.TypeBaseRef_child();
	ESMoL::TypeBase tb = tbr.ref();
	string mtype;
	if ( !tb )
	{
		mtype = "Unknown";
	}
	else
	{
		ESMoL::TypeBase tb = tbr.ref();
		mtype = tb.name();
	}

	std::map< std::string, Semantics::SignalType >::iterator ittype 
		= _typesVisited.find( mtype );
	Semantics::SignalType st;
	if ( ittype == _typesVisited.end() )
	{
		// We'll need to make a new one
		st = Semantics::SignalType::Create( _curDeployment );
		st.name() = mtype;
		st.mtypestr() = mtype;
		_typesVisited[ mtype ] = st;
	}
	else
	{
		// We'll use the old one
		st = ittype->second;
	}

	IndexKeeper::inst()->store_pair( eip.uniqueId(), s.uniqueId(), s, st.uniqueId(), st );
}

// Output port for Simulink-implemented block
void Stage1_ElementVisitor::Visit_OutputPort(const ESMoL::OutputPort & eop)
{
	std::string name = _ctxtName + "_" + string(eop.name());
	Semantics::Signal s = Semantics::Signal::Create( _curDeployment );
	s.id() = eop.uniqueId();
	s.name() = name;
	s.num() = eop.Number();

	ESMoL::TypeBaseRef tbr = eop.TypeBaseRef_child();
	ESMoL::TypeBase tb = tbr.ref();
	string mtype;
	if ( !tb )
	{
		mtype = "Unknown";
	}
	else
	{
		mtype = tb.name();
	}

	std::map< std::string, Semantics::SignalType >::iterator ittype 
		= _typesVisited.find( mtype );
	Semantics::SignalType st;
	if ( ittype == _typesVisited.end() )
	{
		// We'll need to make a new one
		st = Semantics::SignalType::Create( _curDeployment );
		st.name() = mtype;
		st.mtypestr() = mtype;
		_typesVisited[ mtype ] = st;
	}
	else
	{
		// We'll use the old one
		st = ittype->second;
	}

	IndexKeeper::inst()->store_pair( eop.uniqueId(), s.uniqueId(), s, st.uniqueId(), st );
}

void Stage1_ElementVisitor::Visit_TTBus(const ESMoL::TTBus& ttbus)
{
	
	if (IndexKeeper::inst()->seenIdx( ttbus.uniqueId() ) )  // if we've already been here!
		return;
	
	Semantics::TTBus b = Semantics::TTBus::Create( _curDeployment );

	b.id() = ttbus.uniqueId();
	b.name() = ttbus.name();
	// @todo: Fix the lookup!
	IndexKeeper::inst()->store( ttbus.uniqueId(), b.uniqueId(), b );
	//busmap[ttbus.uniqueId()] = b.uniqueId();
	
	b.hyperperiodsecs() = _hyperperiodsecs;

	string str = ttbus.DataRate();
	b.dataratebps() = rateStr2bps(str);
	
	str = ttbus.Hyperperiod();
	b.hyperperiodsecs() = timeStr2secs(str);
	
	str = ttbus.TTSetupTime();
	b.setupsecs() = timeStr2secs(str);

	str = ttbus.SwitchMemorySize();
	b.switchmembytes() = sizeStr2bytes(str);

	str = ttbus.MinFrameSize();
	b.minframebytes() = sizeStr2bytes(str);
	
	str = ttbus.SlotSize();
	b.busslotbytes() = sizeStr2bytes(str);
	
}

void Stage1_ElementVisitor::Visit_BChan(const ESMoL::BChan & bc)
{
	//Semantics::CommChan cc = Semantics::CommChan::Create(_curDeployment);

	Semantics::CommInterface ci = Semantics::CommInterface::Create(_curDeployment);
	Semantics::BChan sbc = Semantics::BChan::Create(_curDeployment);
	if (bc!=Udm::null)
	{
		string n1 = bc.name();
		n1 += "_dev";
		ci.name() = n1;
		string n2 = bc.name();
		n2 += "_chan";
		sbc.name() = n2;
		//cc.name() = bc.name();
		//cc.id() = bc.uniqueId();
		ci.id() = bc.uniqueId();
		sbc.id() = bc.uniqueId();
		IndexKeeper::inst()->store_pair( bc.uniqueId(), ci.uniqueId(),ci, sbc.uniqueId(), sbc);
	}
}

// The semantics here are tricky -- an IChan creates a task on its node.
// All references to the channel from other tasks really indicate communication
// with the IChan task.  If the other tasks have specified their own messages
// then we should use those (and make sure they are consistent).  If none of the
// other tasks have an explicit message (that is, they all connect the IChan
// directly to a component), then we need to create an implicit message.
//
// In either case, in the semantic model the IChan maps to a Task/Message pair.
// We create a default message for the pair, and then overwrite it if we find that
// one of the tasks has defined a message for it.  The assumption is that the 
// modeling tools will enforce the existence of no more than one actual message object
// among all the tasks that use this peripheral device (all of the others 
// must be references or simple chan->component connections).
// 
// The OChan semantics have the same issues.
void Stage1_ElementVisitor::Visit_IChan(const ESMoL::IChan& ic)
{
	//Semantics::TTTask t = Semantics::TTTask::Create( _curDeployment );
	//Semantics::TTMsg m = Semantics::TTMsg::Create( _curDeployment );

	Semantics::InputDevice id = Semantics::InputDevice::Create(_curDeployment);
	Semantics::IChan sic = Semantics::IChan::Create(_curDeployment);
	if ( ic != Udm::null)
	{
		string n1 = ic.name();
		n1 += "_dev";
		id.name() = n1;
		id.id() = ic.uniqueId();
		string n2 = ic.name();
		n2 += "_chan";
		sic.name() = n2;
		sic.id() = ic.uniqueId();

		sic.config() = ic.Configuration();
		//t.config() = ic.Configuration();
		id.devicetype() = ic.DeviceType();
		//t.devicetype() = ic.DeviceType();

		//IndexKeeper::inst()->store_pair( ic.uniqueId(), t.uniqueId(), t, m.uniqueId(), m );
		IndexKeeper::inst()->store_pair( ic.uniqueId(), id.uniqueId(), id, sic.uniqueId(), sic );
	}
}

// See the description for IChan
void Stage1_ElementVisitor::Visit_OChan(const ESMoL::OChan& oc)
{
	//Semantics::TTTask t = Semantics::TTTask::Create( _curDeployment );
	//Semantics::TTMsg m = Semantics::TTMsg::Create( _curDeployment );

	//if ( oc != Udm::null)
	//{
	//	string n1 = oc.name();
	//	n1 += "_task";
	//	t.name() = n1;
	//	t.id() = oc.uniqueId();
	//	string n2 = oc.name();
	//	n2 += "_msg";
	//	m.name() = n2;
	//	m.id() = oc.uniqueId();

	//	t.config() = oc.Configuration();
	//	t.devicetype() = oc.DeviceType();

	//	IndexKeeper::inst()->store_pair( oc.uniqueId(), t.uniqueId(), t, m.uniqueId(), m );
	//}
	Semantics::OutputDevice od = Semantics::OutputDevice::Create(_curDeployment);
	Semantics::OChan soc = Semantics::OChan::Create(_curDeployment);
	if ( oc != Udm::null)
	{
		string n1 = oc.name();
		n1 += "_dev";
		od.name() = n1;
		od.id() = oc.uniqueId();
		string n2 = oc.name();
		n2 += "_chan";
		soc.name() = n2;
		soc.id() = oc.uniqueId();

		soc.config() = oc.Configuration();
		od.devicetype() = oc.DeviceType();

		IndexKeeper::inst()->store_pair( oc.uniqueId(), od.uniqueId(), od, soc.uniqueId(), soc );
	}

}

// See whether we have the right type (IChan or OChan), 
// see whether we have an actual message object connected,
// and if so replace it in the semantic Task/Msg pair.
void Stage1_ElementVisitor::Visit_MsgAssignment(const ESMoL::MsgAssignment & ma)
{
	ESMoL::ChannelRef chanref = ma.dstMsgAssignment_end();
	ESMoL::Channel chan = chanref.ref();
	ESMoL::CommDst cd = ma.srcMsgAssignment_end();

	ESMoL::Message msg;
	if ( cd.type() == ESMoL::Message::meta || cd.type() == ESMoL::TTMessage::meta )
	{
		msg = ESMoL::Message::Cast(cd);
	}
	else if ( cd.type() == ESMoL::MsgPort::meta )
	{
		ESMoL::MsgPort mp = ESMoL::MsgPort::Cast(cd);
		msg = ESMoL::Message::Cast(mp.parent());
	}
	else if ( cd.type() == ESMoL::MessageRef::meta )
	{
		ESMoL::MessageRef mr = ESMoL::MessageRef::Cast(cd);
		msg = ESMoL::Message::Cast(mr.ref());
	}

	if ( msg == Udm::null )
	{
		cout << "Null message in message assignment (" << ma.uniqueId() << "... skipping." << endl;
		return;
	}

	Semantics::Msg m = IndexKeeper::inst()->lookup<Semantics::Msg>(msg.uniqueId());

	if ( chan != Udm::null )
	{	
		if ( chan.type() == ESMoL::BChan::meta )
		{
			m.messagetype() = string("Remote"); // set the right message type
		}

		// This lookup is OK because we've already finished creating the channels and devices in the traversal.
		// If the traversal order changes, this could break.
		pair< Semantics::Device, Semantics::Channel > pair1 =
		 IndexKeeper::inst()->lookup_pair< Semantics::Device, Semantics::Channel >( chan.uniqueId() );

		Semantics::Device dv = pair1.first;
		Semantics::Channel ch = pair1.second;

		IndexKeeper::inst()->store_pair( ma.uniqueId(), ch.uniqueId(), ch, m.uniqueId(), m );
	}
}

void Stage1_ElementVisitor::Visit_OutCommMapping(const ESMoL::OutCommMapping & ocm)
{
	ESMoL::Output outp = ocm.srcOutCommMapping_end();
	ESMoL::CommDst comdst = ocm.dstOutCommMapping_end();

	pair< Semantics::Signal, Semantics::SignalType > p
		= IndexKeeper::inst()->lookup_pair<Semantics::Signal, Semantics::SignalType>( outp.uniqueId() );

	Semantics::Signal s = p.first;

	if ( comdst.type() == ESMoL::MsgPort::meta )
	{
		ESMoL::MsgPort mp = ESMoL::MsgPort::Cast( comdst );
		if ( mp.MsgPortNum() != -1 )
		{
			s.fnum() = mp.MsgPortNum();
		}
	}	
}

void Stage1_ElementVisitor::Visit_InCommMapping(const ESMoL::InCommMapping & icm)
{
	ESMoL::Input inp = icm.dstInCommMapping_end();
	ESMoL::CommDst comdst = icm.srcInCommMapping_end();

	pair< Semantics::Signal, Semantics::SignalType > p
		= IndexKeeper::inst()->lookup_pair<Semantics::Signal, Semantics::SignalType>( inp.uniqueId() );

	Semantics::Signal s = p.first;

	if ( comdst.type() == ESMoL::MsgPort::meta )
	{
		ESMoL::MsgPort mp = ESMoL::MsgPort::Cast( comdst );
		if ( mp.MsgPortNum() != -1 )
		{
			s.fnum() = mp.MsgPortNum();
		}
	}
}

void little_function()
{
	// This prevents visual studio from doing dumb stuff when functions are folded in the editor.
}
