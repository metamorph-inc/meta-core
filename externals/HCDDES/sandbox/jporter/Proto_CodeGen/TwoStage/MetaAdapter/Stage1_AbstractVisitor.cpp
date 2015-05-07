#include "Stage1_Visitor.h"
#include "utils.h"
#include "Uml.h"

#include "IndexKeeper.h"

#include <sstream>

#pragma warning(disable:4996)
#pragma warning(disable:4267)
#pragma warning(disable:4244)

using namespace std;

Stage1_AbstractVisitor::Stage1_AbstractVisitor(Semantics::RootFolder & target)
{
	_RootFolder = target;
}

void Stage1_AbstractVisitor::Visit_RootFolder(const ESMoL::RootFolder & rf)
{
	ctxt rfc( _ct, rf.name() );
	set<ESMoL::DesignFolder> setDF = rf.DesignFolder_children();

	for(set<ESMoL::DesignFolder>::iterator it = setDF.begin(); it!= setDF.end(); it++)
	{
		ESMoL::DesignFolder df = (*it);
		ctxt dfc( _ct, df.name() );

		set<ESMoL::Deployment> setDeploys = df.Deployment_children();
		for(set<ESMoL::Deployment>::iterator itd = setDeploys.begin(); itd != setDeploys.end(); itd++)
		{
			ctxt dpc( _ct, (*itd).name() );
			(*itd).Accept(*this);
		}

		set<ESMoL::TimingSheet> setTimingSheets = df.TimingSheet_children();
		for(set<ESMoL::TimingSheet>::iterator itt = setTimingSheets.begin(); itt != setTimingSheets.end(); itt++)
		{
			ctxt tsc( _ct, (*itt).name() );
			(*itt).Accept(*this);
		}
	}
}

void Stage1_AbstractVisitor::Visit_Deployment(const ESMoL::Deployment& deploy)
{
	// Any instantiation/setup was done already by the specialized subclass

	//visit all noderefs and build the information pertaining to all the TTBuses (networks) involved
	set<ESMoL::NodeRef> setNR = deploy.NodeRef_children();
	for(set<ESMoL::NodeRef>::iterator itnr= setNR.begin(); itnr != setNR.end(); itnr++)
	{
		ESMoL::NodeRef nr = (*itnr);
		ESMoL::Node node = nr.ref();
		if (node == Udm::null)
		{
			cout << _ct.getContext() << ": Dangling NodeRef (points nowhere) " << nr.name() << endl;
			//probably print an error message
			continue;
		}

		ctxt ndc( _ct, node.name() );
		//visit noderef
		nr.Accept(*this);

		// Process the channel elements
		set<ESMoL::BChan> setBC = node.BChan_kind_children();
		for(set<ESMoL::BChan>::iterator itbc = setBC.begin(); itbc != setBC.end(); itbc++)
		{
			ESMoL::BChan bc = (*itbc);
			ESMoL::TTBus b;

			ctxt bcc( _ct, bc.name() );
			bc.Accept(*this);

			set<ESMoL::BChan> nodesVisited;
			nodesVisited.clear();
			nodesVisited.insert(bc);

			if (getBus(bc,nodesVisited,b))
			{
				ctxt bsc(_ct, b.name());
				b.Accept(*this);
			}
		}

		// Process the ichan nodes
		set<ESMoL::IChan> setIC = node.IChan_kind_children();
		for(set<ESMoL::IChan>::iterator itic = setIC.begin(); itic != setIC.end(); itic++)
		{
			// Create a task and message
			ctxt icc( _ct, (*itic).name() );
			(*itic).Accept(*this);
		}

		// Process the ochan nodes
		set<ESMoL::OChan> setOC = node.OChan_kind_children();
		for(set<ESMoL::OChan>::iterator itoc = setOC.begin(); itoc != setOC.end(); itoc++)
		{
			// Create a task and message
			ctxt occ( _ct, (*itoc).name() );
			(*itoc).Accept(*this);
		}
	}

	set<ESMoL::Task> setTasks = deploy.Task_children();
	for ( set<ESMoL::Task>::iterator itt = setTasks.begin(); itt != setTasks.end(); itt++)
	{
		// @todo:  visit the Components, Messages, and then all types of connectors between things.
		// look at how Nag/Peter handled the InCommMapping/OutCommMapping in the translation to
		// signal entities.
		ctxt tsc( _ct, (*itt).name() );

		(*itt).Accept( *this ); // First, visit the parent class
		// Then visit the subclasses
		if ((*itt).type() == ESMoL::TTTask::meta )
		{
			ESMoL::TTTask tt = ESMoL::TTTask::Cast( *itt );
			tt.Accept( *this ); // Time-triggered task
		}

		set<ESMoL::Component> setComp = itt->Component_children();
		for ( set<ESMoL::Component>::iterator icmp = setComp.begin(); icmp != setComp.end(); icmp++)
		{
			_ctxtName = string((*itt).name()) + "_" + string((*icmp).name()) + "_"; // provide unique names for signals
			ctxt cmc( _ct, (*icmp).name());
			(*icmp).Accept(*this);

			ESMoL::ComponentBase cb = (*icmp).ref();
			if ( cb )
			{
				if (cb.type() == ESMoL::SubsystemRef::meta) {
					cb = ESMoL::SubsystemRef::Cast(cb).ref();
				} // dereference once, if necessary

				if (cb.type() == ESMoL::CCode::meta) {
					ESMoL::CCode ccode = ESMoL::CCode::Cast(cb);
					
					if ( ccode )
					{
						set< ESMoL::CInputPort > setInPorts = ccode.CInputPort_children();
						for ( set< ESMoL::CInputPort >::iterator iip = setInPorts.begin(); iip != setInPorts.end(); iip++)
						{
							(*iip).Accept(*this);
						}

						set< ESMoL::COutputPort > setOutPorts = ccode.COutputPort_children();
						for ( set< ESMoL::COutputPort >::iterator iop = setOutPorts.begin(); iop != setOutPorts.end(); iop++)
						{
							(*iop).Accept(*this);
						}
					}
				}
				else if (cb.type() == ESMoL::Subsystem::meta) 
				{
					ESMoL::Subsystem subSys = ESMoL::Subsystem::Cast(cb);
					
					if ( cb )
					{
						set< ESMoL::InputPort > setInPorts = subSys.InputPort_kind_children();
						for ( set< ESMoL::InputPort >::iterator iip = setInPorts.begin(); iip != setInPorts.end(); iip++)
						{
							(*iip).Accept(*this);
						}

						set< ESMoL::OutputPort > setOutPorts = subSys.OutputPort_kind_children();
						for ( set< ESMoL::OutputPort >::iterator iop = setOutPorts.begin(); iop != setOutPorts.end(); iop++)
						{
							(*iop).Accept(*this);
						}
					}
				}
			}
		}

		std::set< ESMoL::MessageRef > msgrefs = itt->MessageRef_kind_children();
		for ( std::set< ESMoL::MessageRef >::iterator mrf = msgrefs.begin(); mrf != msgrefs.end(); mrf++ )
		{
			ostringstream out;
			out << (*mrf).name() << "(ref)";
			ctxt mrc( _ct, out.str());
			(*mrf).Accept(*this);
		}

		// The parent class (Message) is abstract, so we handle the children directly.
		std::set< ESMoL::TTMessage > ttmsgs = itt->TTMessage_kind_children();
		for ( set< ESMoL::TTMessage >::iterator ttm = ttmsgs.begin(); ttm != ttmsgs.end(); ttm++ )
		{
			ctxt mc( _ct, (*ttm).name() );
			(*ttm).Accept( *this );
		}

		std::set< ESMoL::ChannelRef > crefs = (*itt).ChannelRef_kind_children();
		for ( set< ESMoL::ChannelRef >::iterator icr = crefs.begin(); icr != crefs.end(); icr++ )
		{
			ctxt crc( _ct, (*icr).name() );
			(*icr).Accept(*this);
		}

		set< ESMoL::MsgAssignment > msgasgn = (*itt).MsgAssignment_children();
		for ( set< ESMoL::MsgAssignment >::iterator ima = msgasgn.begin(); ima != msgasgn.end(); ima++ )
		{
			std::ostringstream ctname;
			ctname << "MsgAssignment" << (*ima).uniqueId();
			ctxt mac(_ct, ctname.str() );
			(*ima).Accept(*this);
		}

		set< ESMoL::InCommMapping > inmaps = (*itt).InCommMapping_kind_children();
		for ( set< ESMoL::InCommMapping >::iterator iicm = inmaps.begin(); iicm != inmaps.end(); iicm++ )
		{
			std::ostringstream ctname;
			ctname << "InCommMapping" << (*iicm).uniqueId();
			ctxt imc( _ct, ctname.str() );
			(*iicm).Accept(*this);
		}

		set< ESMoL::OutCommMapping > outmaps = (*itt).OutCommMapping_kind_children();
		for ( set< ESMoL::OutCommMapping >::iterator iocm = outmaps.begin(); iocm != outmaps.end(); iocm++ )
		{
			std::ostringstream ctname;
			ctname << "OutCommMapping" << (*iocm).uniqueId();
			ctxt omc( _ct, ctname.str() );
			(*iocm).Accept(*this);
		}
	}

	set< ESMoL::TaskAssignment > taskasgn = deploy.TaskAssignment_children();
	for ( set< ESMoL::TaskAssignment >::iterator pTA = taskasgn.begin(); pTA != taskasgn.end(); pTA++ )
	{
		std::ostringstream ctxtname;
		ctxtname << "TaskAssignment" << (*pTA).uniqueId();
		ctxt tac( _ct, ctxtname.str() );
		(*pTA).Accept(*this);
	}

	
}

void Stage1_AbstractVisitor::Visit_TimingSheet(const ESMoL::TimingSheet&)
{
	// iterate and get all of the constraints and the connected tasks
}

bool Stage1_AbstractVisitor::getBus(ESMoL::BChan bchan, set<ESMoL::BChan>& nodesVisited, ESMoL::TTBus& bus)
{
	//function invoked recursively to identify the bus to which a BChan node/port is associated.

	//source connections
	set<ESMoL::Wire> srcWire = bchan.srcWire();
	for(set<ESMoL::Wire>::iterator its = srcWire.begin(); its !=srcWire.end(); its++)
	{
		ESMoL::Connectable con = (*its).srcWire_end();
		if (con.type() == ESMoL::TTBus::meta)
		{
			bus = ESMoL::TTBus::Cast(con);
			return true;
		}
		if (con.type() == ESMoL::BChan::meta)
		{
			ESMoL::BChan nbchan = ESMoL::BChan::Cast(con);
			if (nodesVisited.find(nbchan) != nodesVisited.end())
				continue;
			nodesVisited.insert(nbchan);
			getBus(nbchan,nodesVisited,bus);
		}
	}


	//destination connections
	set<ESMoL::Wire> dstWire = bchan.dstWire();
	for(set<ESMoL::Wire>::iterator itd = dstWire.begin(); itd != dstWire.end(); itd++)
	{
		ESMoL::Connectable con = (*itd).dstWire_end();
		if (con.type() == ESMoL::TTBus::meta)
		{
			bus = ESMoL::TTBus::Cast(con);
			return true;
		}
		if (con.type() == ESMoL::BChan::meta)
		{
			ESMoL::BChan nbchan = ESMoL::BChan::Cast(con);
			if (nodesVisited.find(nbchan) != nodesVisited.end())
				continue;
			nodesVisited.insert(nbchan);
			getBus(nbchan,nodesVisited,bus);
		}
	}

	return false;
}

ESMoL::Message Stage1_AbstractVisitor::getMsg( ESMoL::CommDst & cd )
{
	if ( cd.type() == ESMoL::Message::meta || cd.type() == ESMoL::TTMessage::meta )
	{
		ESMoL::Message m = ESMoL::Message::Cast( cd );
		return m;
	}

	if ( cd.type() == ESMoL::MessageRef::meta )
	{
		ESMoL::MessageRef mr = ESMoL::MessageRef::Cast( cd );
		return ESMoL::Message::Cast(mr.ref());
	}

	if ( cd.type() == ESMoL::MsgPort::meta )
	{
		ESMoL::MsgPort mp = ESMoL::MsgPort::Cast( cd );
		return ESMoL::Message::Cast(mp.parent());
	}

	// @todo What about the ChannelRef case?  Decide what to do with it.
	// It should probably indicate an inferred message in the task.

	return ESMoL::Message::Cast(Udm::null);
}

