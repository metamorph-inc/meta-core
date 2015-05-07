#include "stdafx.h"
#include "MotivateBonX.h"


namespace BON
{

IMPLEMENT_BONEXTENSION( Motivate_BON::ProcessFolder, "ProcessFolder" );
IMPLEMENT_ABSTRACT_BONEXTENSION( Motivate_BON::Fragment );
IMPLEMENT_BONEXTENSION( Motivate_BON::AbstractModel, "AbstractModel" );
IMPLEMENT_BONEXTENSION( Motivate_BON::Code, "Code" );
IMPLEMENT_BONEXTENSION( Motivate_BON::ESMoLModel, "ESMoLModel" );
IMPLEMENT_BONEXTENSION( Motivate_BON::SchedInput, "SchedInput" );
IMPLEMENT_BONEXTENSION( Motivate_BON::SchedOutput, "SchedOutput" );
IMPLEMENT_BONEXTENSION( Motivate_BON::SimulinkModel, "SimulinkModel" );
IMPLEMENT_ABSTRACT_BONEXTENSION( Motivate_BON::Action );
IMPLEMENT_BONEXTENSION( Motivate_BON::CommandLineAction, "CommandLineAction" );
IMPLEMENT_BONEXTENSION( Motivate_BON::Process, "Process" );
IMPLEMENT_BONEXTENSION( Motivate_BON::Test, "Test" );
IMPLEMENT_BONEXTENSION( Motivate_BON::Viewer, "Viewer" );
IMPLEMENT_ABSTRACT_BONEXTENSION( Motivate_BON::Channel );
IMPLEMENT_BONEXTENSION( Motivate_BON::InputPort, "InputPort" );
IMPLEMENT_BONEXTENSION( Motivate_BON::OutputPort, "OutputPort" );
IMPLEMENT_BONEXTENSION( Motivate_BON::ActionSequence, "ActionSequence" );
IMPLEMENT_BONEXTENSION( Motivate_BON::InputConnection, "InputConnection" );
IMPLEMENT_BONEXTENSION( Motivate_BON::OutputConnection, "OutputConnection" );
IMPLEMENT_BONEXTENSION( Motivate_BON::ViewerUseage, "ViewerUseage" );


} // namespace BON

//********************************************************************************
// 
//********************************************************************************
void Motivate_BON::ProcessFolderImpl::accept( BON::Visitor *pVisitor)
{
	// visit the Folder
	pVisitor->visitFolder( BON::Folder( this));

}


//********************************************************************************
// getter for kind "Process"
//********************************************************************************
std::set<Motivate_BON::Process> Motivate_BON::ProcessFolderImpl::getProcess()
{
	std::set<Motivate_BON::Process> res;
	std::set<BON::Object> kinds = FolderImpl::getChildObjects("Process");
	for( std::set<BON::Object>::iterator i = kinds.begin(); i != kinds.end(); ++i)
	{
		Motivate_BON::Process elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// 
//********************************************************************************
void Motivate_BON::FragmentImpl::accept( BON::Visitor *pVisitor)
{
	// visit the Model
	pVisitor->visitModel( BON::Model( this));

}


//********************************************************************************
// 
//********************************************************************************
std::set<Motivate_BON::OutputConnection> Motivate_BON::FragmentImpl::getInOutputConnectionLinks()
{
	std::set<Motivate_BON::OutputConnection> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getInConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		Motivate_BON::OutputConnection c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
// returns dst Motivate_BON::InputPorts
//********************************************************************************
std::multiset<Motivate_BON::InputPort> Motivate_BON::FragmentImpl::getInputConnectionDsts()
{
	std::multiset<Motivate_BON::InputPort> res;
	{
		std::multiset<BON::ConnectionEnd> out_ends = BON::ConnectionEndImpl::getOutConnEnds("InputConnection");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = out_ends.begin() ; cit != out_ends.end() ; ++cit )
		{
			Motivate_BON::InputPort dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
// 
//********************************************************************************
std::set<Motivate_BON::InputConnection> Motivate_BON::FragmentImpl::getOutInputConnectionLinks()
{
	std::set<Motivate_BON::InputConnection> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getOutConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		Motivate_BON::InputConnection c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
// 
//********************************************************************************
std::set<Motivate_BON::ViewerUseage> Motivate_BON::FragmentImpl::getOutViewerUseageLinks()
{
	std::set<Motivate_BON::ViewerUseage> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getOutConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		Motivate_BON::ViewerUseage c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
// returns src Motivate_BON::OutputPorts
//********************************************************************************
std::multiset<Motivate_BON::OutputPort> Motivate_BON::FragmentImpl::getOutputConnectionSrcs()
{
	std::multiset<Motivate_BON::OutputPort> res;
	{
		std::multiset<BON::ConnectionEnd> in_ends = BON::ConnectionEndImpl::getInConnEnds("OutputConnection");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = in_ends.begin() ; cit != in_ends.end() ; ++cit )
		{
			Motivate_BON::OutputPort dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
// returns dst Motivate_BON::Viewers
//********************************************************************************
std::multiset<Motivate_BON::Viewer> Motivate_BON::FragmentImpl::getViewerUseageDsts()
{
	std::multiset<Motivate_BON::Viewer> res;
	{
		std::multiset<BON::ConnectionEnd> out_ends = BON::ConnectionEndImpl::getOutConnEnds("ViewerUseage");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = out_ends.begin() ; cit != out_ends.end() ; ++cit )
		{
			Motivate_BON::Viewer dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
// 
//********************************************************************************
std::string Motivate_BON::FragmentImpl::getdateModified() 
{
	return FCOImpl::getAttribute("dateModified")->getStringValue();
}


//********************************************************************************
// 
//********************************************************************************
std::string Motivate_BON::FragmentImpl::getfullpath() 
{
	return FCOImpl::getAttribute("fullpath")->getStringValue();
}


//********************************************************************************
// 
//********************************************************************************
void Motivate_BON::FragmentImpl::setdateModified( const std::string& val)
{
	FCOImpl::getAttribute("dateModified")->setStringValue( val);
}


//********************************************************************************
// 
//********************************************************************************
void Motivate_BON::FragmentImpl::setfullpath( const std::string& val)
{
	FCOImpl::getAttribute("fullpath")->setStringValue( val);
}


//********************************************************************************
// 
//********************************************************************************
void Motivate_BON::AbstractModelImpl::accept( BON::Visitor *pVisitor)
{
	// visit the Model
	pVisitor->visitModel( BON::Model( this));

}


//********************************************************************************
// 
//********************************************************************************
void Motivate_BON::CodeImpl::accept( BON::Visitor *pVisitor)
{
	// visit the Model
	pVisitor->visitModel( BON::Model( this));

}


//********************************************************************************
// 
//********************************************************************************
void Motivate_BON::ESMoLModelImpl::accept( BON::Visitor *pVisitor)
{
	// visit the Model
	pVisitor->visitModel( BON::Model( this));

}


//********************************************************************************
// 
//********************************************************************************
void Motivate_BON::SchedInputImpl::accept( BON::Visitor *pVisitor)
{
	// visit the Model
	pVisitor->visitModel( BON::Model( this));

}


//********************************************************************************
// 
//********************************************************************************
void Motivate_BON::SchedOutputImpl::accept( BON::Visitor *pVisitor)
{
	// visit the Model
	pVisitor->visitModel( BON::Model( this));

}


//********************************************************************************
// 
//********************************************************************************
void Motivate_BON::SimulinkModelImpl::accept( BON::Visitor *pVisitor)
{
	// visit the Model
	pVisitor->visitModel( BON::Model( this));

}


//********************************************************************************
// 
//********************************************************************************
void Motivate_BON::ActionImpl::accept( BON::Visitor *pVisitor)
{
	// visit the Model
	pVisitor->visitModel( BON::Model( this));

}


//********************************************************************************
// returns dst Motivate_BON::Actions
//********************************************************************************
std::multiset<Motivate_BON::Action> Motivate_BON::ActionImpl::getActionSequenceDsts()
{
	std::multiset<Motivate_BON::Action> res;
	{
		std::multiset<BON::ConnectionEnd> out_ends = BON::ConnectionEndImpl::getOutConnEnds("ActionSequence");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = out_ends.begin() ; cit != out_ends.end() ; ++cit )
		{
			Motivate_BON::Action dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
// 
//********************************************************************************
std::set<Motivate_BON::ActionSequence> Motivate_BON::ActionImpl::getActionSequenceLinks()
{
	std::set<Motivate_BON::ActionSequence> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		Motivate_BON::ActionSequence c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
// returns src Motivate_BON::Actions
//********************************************************************************
std::multiset<Motivate_BON::Action> Motivate_BON::ActionImpl::getActionSequenceSrcs()
{
	std::multiset<Motivate_BON::Action> res;
	{
		std::multiset<BON::ConnectionEnd> in_ends = BON::ConnectionEndImpl::getInConnEnds("ActionSequence");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = in_ends.begin() ; cit != in_ends.end() ; ++cit )
		{
			Motivate_BON::Action dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
// 
//********************************************************************************
std::set<Motivate_BON::ActionSequence> Motivate_BON::ActionImpl::getInActionSequenceLinks()
{
	std::set<Motivate_BON::ActionSequence> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getInConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		Motivate_BON::ActionSequence c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
// 
//********************************************************************************
std::set<Motivate_BON::ActionSequence> Motivate_BON::ActionImpl::getOutActionSequenceLinks()
{
	std::set<Motivate_BON::ActionSequence> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getOutConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		Motivate_BON::ActionSequence c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
// aggregated getter for role "Motivate_BON::" among "Motivate_BON::Channel"s and its descendants
//********************************************************************************
std::set<Motivate_BON::Channel> Motivate_BON::ActionImpl::getChannel()
{
	std::set<Motivate_BON::Channel> res;
	const int len = 2;
	std::set<BON::FCO> roles_vec[ len];
	roles_vec[0] = ModelImpl::getChildFCOsAs("InputPort");
	roles_vec[1] = ModelImpl::getChildFCOsAs("OutputPort");
	for( int k = 0; k < len; ++k)
		for( std::set<BON::FCO>::iterator i = roles_vec[k].begin(); i != roles_vec[k].end(); ++i)
		{
			Motivate_BON::Channel elem(*i);
			ASSERT(elem);
			res.insert(elem);
		}
	return res;
}


//********************************************************************************
// getter for role "CommandLineAction" among "Motivate_BON::Action"s
//********************************************************************************
std::set<Motivate_BON::CommandLineAction> Motivate_BON::ActionImpl::getCommandLineAction()
{
	std::set<Motivate_BON::CommandLineAction> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("CommandLineAction");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Motivate_BON::CommandLineAction elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "InputPort" among "Channel"s and its descendants
//********************************************************************************
std::set<Motivate_BON::InputPort> Motivate_BON::ActionImpl::getInputPort()
{
	std::set<Motivate_BON::InputPort> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("InputPort");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Motivate_BON::InputPort elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "OutputPort" among "Channel"s and its descendants
//********************************************************************************
std::set<Motivate_BON::OutputPort> Motivate_BON::ActionImpl::getOutputPort()
{
	std::set<Motivate_BON::OutputPort> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("OutputPort");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Motivate_BON::OutputPort elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// 
//********************************************************************************
void Motivate_BON::CommandLineActionImpl::accept( BON::Visitor *pVisitor)
{
	// visit the Model
	pVisitor->visitModel( BON::Model( this));

}


//********************************************************************************
// 
//********************************************************************************
void Motivate_BON::ProcessImpl::accept( BON::Visitor *pVisitor)
{
	// visit the Model
	pVisitor->visitModel( BON::Model( this));

}


//********************************************************************************
// 
//********************************************************************************
std::string Motivate_BON::ProcessImpl::getWorkingDirectory() 
{
	return FCOImpl::getAttribute("WorkingDirectory")->getStringValue();
}


//********************************************************************************
// 
//********************************************************************************
void Motivate_BON::ProcessImpl::setWorkingDirectory( const std::string& val)
{
	FCOImpl::getAttribute("WorkingDirectory")->setStringValue( val);
}


//********************************************************************************
// getter for role "AbstractModel" among "Fragment"s and its descendants
//********************************************************************************
std::set<Motivate_BON::AbstractModel> Motivate_BON::ProcessImpl::getAbstractModel()
{
	std::set<Motivate_BON::AbstractModel> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("AbstractModel");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Motivate_BON::AbstractModel elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "ActionSequence" among "Motivate_BON::ActionSequence"s
//********************************************************************************
std::set<Motivate_BON::ActionSequence> Motivate_BON::ProcessImpl::getActionSequence()
{
	std::set<Motivate_BON::ActionSequence> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("ActionSequence");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Motivate_BON::ActionSequence elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "Code" among "Fragment"s and its descendants
//********************************************************************************
std::set<Motivate_BON::Code> Motivate_BON::ProcessImpl::getCode()
{
	std::set<Motivate_BON::Code> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("Code");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Motivate_BON::Code elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "CommandLineAction" among "Motivate_BON::Action"s
//********************************************************************************
std::set<Motivate_BON::CommandLineAction> Motivate_BON::ProcessImpl::getCommandLineAction()
{
	std::set<Motivate_BON::CommandLineAction> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("CommandLineAction");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Motivate_BON::CommandLineAction elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "ESMoLModel" among "Fragment"s and its descendants
//********************************************************************************
std::set<Motivate_BON::ESMoLModel> Motivate_BON::ProcessImpl::getESMoLModel()
{
	std::set<Motivate_BON::ESMoLModel> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("ESMoLModel");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Motivate_BON::ESMoLModel elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// aggregated getter for role "Motivate_BON::" among "Motivate_BON::Fragment"s and its descendants
//********************************************************************************
std::set<Motivate_BON::Fragment> Motivate_BON::ProcessImpl::getFragment()
{
	std::set<Motivate_BON::Fragment> res;
	const int len = 6;
	std::set<BON::FCO> roles_vec[ len];
	roles_vec[0] = ModelImpl::getChildFCOsAs("AbstractModel");
	roles_vec[1] = ModelImpl::getChildFCOsAs("Code");
	roles_vec[2] = ModelImpl::getChildFCOsAs("ESMoLModel");
	roles_vec[3] = ModelImpl::getChildFCOsAs("SchedInput");
	roles_vec[4] = ModelImpl::getChildFCOsAs("SchedOutput");
	roles_vec[5] = ModelImpl::getChildFCOsAs("SimulinkModel");
	for( int k = 0; k < len; ++k)
		for( std::set<BON::FCO>::iterator i = roles_vec[k].begin(); i != roles_vec[k].end(); ++i)
		{
			Motivate_BON::Fragment elem(*i);
			ASSERT(elem);
			res.insert(elem);
		}
	return res;
}


//********************************************************************************
// getter for role "InputConnection" among "Motivate_BON::InputConnection"s
//********************************************************************************
std::set<Motivate_BON::InputConnection> Motivate_BON::ProcessImpl::getInputConnection()
{
	std::set<Motivate_BON::InputConnection> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("InputConnection");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Motivate_BON::InputConnection elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "OutputConnection" among "Motivate_BON::OutputConnection"s
//********************************************************************************
std::set<Motivate_BON::OutputConnection> Motivate_BON::ProcessImpl::getOutputConnection()
{
	std::set<Motivate_BON::OutputConnection> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("OutputConnection");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Motivate_BON::OutputConnection elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "SchedInput" among "Fragment"s and its descendants
//********************************************************************************
std::set<Motivate_BON::SchedInput> Motivate_BON::ProcessImpl::getSchedInput()
{
	std::set<Motivate_BON::SchedInput> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("SchedInput");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Motivate_BON::SchedInput elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "SchedOutput" among "Fragment"s and its descendants
//********************************************************************************
std::set<Motivate_BON::SchedOutput> Motivate_BON::ProcessImpl::getSchedOutput()
{
	std::set<Motivate_BON::SchedOutput> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("SchedOutput");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Motivate_BON::SchedOutput elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "SimulinkModel" among "Fragment"s and its descendants
//********************************************************************************
std::set<Motivate_BON::SimulinkModel> Motivate_BON::ProcessImpl::getSimulinkModel()
{
	std::set<Motivate_BON::SimulinkModel> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("SimulinkModel");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Motivate_BON::SimulinkModel elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "Test" among "Motivate_BON::Test"s
//********************************************************************************
std::set<Motivate_BON::Test> Motivate_BON::ProcessImpl::getTest()
{
	std::set<Motivate_BON::Test> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("Test");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Motivate_BON::Test elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "Viewer" among "Motivate_BON::Viewer"s
//********************************************************************************
std::set<Motivate_BON::Viewer> Motivate_BON::ProcessImpl::getViewer()
{
	std::set<Motivate_BON::Viewer> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("Viewer");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Motivate_BON::Viewer elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "ViewerUseage" among "Motivate_BON::ViewerUseage"s
//********************************************************************************
std::set<Motivate_BON::ViewerUseage> Motivate_BON::ProcessImpl::getViewerUseage()
{
	std::set<Motivate_BON::ViewerUseage> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("ViewerUseage");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Motivate_BON::ViewerUseage elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// 
//********************************************************************************
void Motivate_BON::TestImpl::accept( BON::Visitor *pVisitor)
{
	// visit the Model
	pVisitor->visitModel( BON::Model( this));

}


//********************************************************************************
// 
//********************************************************************************
void Motivate_BON::ViewerImpl::accept( BON::Visitor *pVisitor)
{
	// visit the Model
	pVisitor->visitModel( BON::Model( this));

}


//********************************************************************************
// 
//********************************************************************************
std::set<Motivate_BON::ViewerUseage> Motivate_BON::ViewerImpl::getInViewerUseageLinks()
{
	std::set<Motivate_BON::ViewerUseage> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getInConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		Motivate_BON::ViewerUseage c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
// returns src Motivate_BON::Fragments
//********************************************************************************
std::multiset<Motivate_BON::Fragment> Motivate_BON::ViewerImpl::getViewerUseageSrcs()
{
	std::multiset<Motivate_BON::Fragment> res;
	{
		std::multiset<BON::ConnectionEnd> in_ends = BON::ConnectionEndImpl::getInConnEnds("ViewerUseage");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = in_ends.begin() ; cit != in_ends.end() ; ++cit )
		{
			Motivate_BON::Fragment dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
// 
//********************************************************************************
void Motivate_BON::ChannelImpl::accept( BON::Visitor *pVisitor)
{
	// visit the Atom
	pVisitor->visitAtom( BON::Atom( this));

}


//********************************************************************************
// 
//********************************************************************************
std::string Motivate_BON::ChannelImpl::getfiletype() 
{
	return FCOImpl::getAttribute("filetype")->getStringValue();
}


//********************************************************************************
// 
//********************************************************************************
void Motivate_BON::ChannelImpl::setfiletype( const std::string& val)
{
	FCOImpl::getAttribute("filetype")->setStringValue( val);
}


//********************************************************************************
// 
//********************************************************************************
void Motivate_BON::InputPortImpl::accept( BON::Visitor *pVisitor)
{
	// visit the Atom
	pVisitor->visitAtom( BON::Atom( this));

}


//********************************************************************************
// 
//********************************************************************************
std::set<Motivate_BON::InputConnection> Motivate_BON::InputPortImpl::getInInputConnectionLinks()
{
	std::set<Motivate_BON::InputConnection> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getInConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		Motivate_BON::InputConnection c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
// returns src Motivate_BON::Fragments
//********************************************************************************
std::multiset<Motivate_BON::Fragment> Motivate_BON::InputPortImpl::getInputConnectionSrcs()
{
	std::multiset<Motivate_BON::Fragment> res;
	{
		std::multiset<BON::ConnectionEnd> in_ends = BON::ConnectionEndImpl::getInConnEnds("InputConnection");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = in_ends.begin() ; cit != in_ends.end() ; ++cit )
		{
			Motivate_BON::Fragment dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
// 
//********************************************************************************
void Motivate_BON::OutputPortImpl::accept( BON::Visitor *pVisitor)
{
	// visit the Atom
	pVisitor->visitAtom( BON::Atom( this));

}


//********************************************************************************
// 
//********************************************************************************
std::set<Motivate_BON::OutputConnection> Motivate_BON::OutputPortImpl::getOutOutputConnectionLinks()
{
	std::set<Motivate_BON::OutputConnection> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getOutConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		Motivate_BON::OutputConnection c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
// returns dst Motivate_BON::Fragments
//********************************************************************************
std::multiset<Motivate_BON::Fragment> Motivate_BON::OutputPortImpl::getOutputConnectionDsts()
{
	std::multiset<Motivate_BON::Fragment> res;
	{
		std::multiset<BON::ConnectionEnd> out_ends = BON::ConnectionEndImpl::getOutConnEnds("OutputConnection");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = out_ends.begin() ; cit != out_ends.end() ; ++cit )
		{
			Motivate_BON::Fragment dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
// 
//********************************************************************************
void Motivate_BON::ActionSequenceImpl::accept( BON::Visitor *pVisitor)
{
	// visit the Connection
	pVisitor->visitConnection( BON::Connection( this));

}


//********************************************************************************
// getDst() return value is a ConnectionEnd casted to Motivate_BON::Action
//********************************************************************************
Motivate_BON::Action Motivate_BON::ActionSequenceImpl::getDst()
{
	BON::ConnectionEnd ce = ConnectionImpl::getDst();
	Motivate_BON::Action sp( ce);
	if ( sp)
		return sp;

	Motivate_BON::Action empty;
	return empty;
}


//********************************************************************************
// getSrc() return value is a ConnectionEnd casted to Motivate_BON::Action
//********************************************************************************
Motivate_BON::Action Motivate_BON::ActionSequenceImpl::getSrc()
{
	BON::ConnectionEnd ce = ConnectionImpl::getSrc();
	Motivate_BON::Action sp( ce);
	if ( sp)
		return sp;

	Motivate_BON::Action empty;
	return empty;
}


//********************************************************************************
// 
//********************************************************************************
void Motivate_BON::InputConnectionImpl::accept( BON::Visitor *pVisitor)
{
	// visit the Connection
	pVisitor->visitConnection( BON::Connection( this));

}


//********************************************************************************
// getDst() return value is a ConnectionEnd casted to Motivate_BON::InputPort
//********************************************************************************
Motivate_BON::InputPort Motivate_BON::InputConnectionImpl::getDst()
{
	BON::ConnectionEnd ce = ConnectionImpl::getDst();
	Motivate_BON::InputPort sp( ce);
	if ( sp)
		return sp;

	Motivate_BON::InputPort empty;
	return empty;
}


//********************************************************************************
// getSrc() return value is a ConnectionEnd casted to Motivate_BON::Fragment
//********************************************************************************
Motivate_BON::Fragment Motivate_BON::InputConnectionImpl::getSrc()
{
	BON::ConnectionEnd ce = ConnectionImpl::getSrc();
	Motivate_BON::Fragment sp( ce);
	if ( sp)
		return sp;

	Motivate_BON::Fragment empty;
	return empty;
}


//********************************************************************************
// 
//********************************************************************************
void Motivate_BON::OutputConnectionImpl::accept( BON::Visitor *pVisitor)
{
	// visit the Connection
	pVisitor->visitConnection( BON::Connection( this));

}


//********************************************************************************
// getDst() return value is a ConnectionEnd casted to Motivate_BON::Fragment
//********************************************************************************
Motivate_BON::Fragment Motivate_BON::OutputConnectionImpl::getDst()
{
	BON::ConnectionEnd ce = ConnectionImpl::getDst();
	Motivate_BON::Fragment sp( ce);
	if ( sp)
		return sp;

	Motivate_BON::Fragment empty;
	return empty;
}


//********************************************************************************
// getSrc() return value is a ConnectionEnd casted to Motivate_BON::OutputPort
//********************************************************************************
Motivate_BON::OutputPort Motivate_BON::OutputConnectionImpl::getSrc()
{
	BON::ConnectionEnd ce = ConnectionImpl::getSrc();
	Motivate_BON::OutputPort sp( ce);
	if ( sp)
		return sp;

	Motivate_BON::OutputPort empty;
	return empty;
}


//********************************************************************************
// 
//********************************************************************************
void Motivate_BON::ViewerUseageImpl::accept( BON::Visitor *pVisitor)
{
	// visit the Connection
	pVisitor->visitConnection( BON::Connection( this));

}


//********************************************************************************
// getDst() return value is a ConnectionEnd casted to Motivate_BON::Viewer
//********************************************************************************
Motivate_BON::Viewer Motivate_BON::ViewerUseageImpl::getDst()
{
	BON::ConnectionEnd ce = ConnectionImpl::getDst();
	Motivate_BON::Viewer sp( ce);
	if ( sp)
		return sp;

	Motivate_BON::Viewer empty;
	return empty;
}


//********************************************************************************
// getSrc() return value is a ConnectionEnd casted to Motivate_BON::Fragment
//********************************************************************************
Motivate_BON::Fragment Motivate_BON::ViewerUseageImpl::getSrc()
{
	BON::ConnectionEnd ce = ConnectionImpl::getSrc();
	Motivate_BON::Fragment sp( ce);
	if ( sp)
		return sp;

	Motivate_BON::Fragment empty;
	return empty;
}


