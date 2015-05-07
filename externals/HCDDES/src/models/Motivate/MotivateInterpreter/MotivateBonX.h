#ifndef MOTIVATEBONX_H
#define MOTIVATEBONX_H

#include "BON.h"
#include "BONImpl.h"
#include "Extensions.h"

///BUP
// add your include files/class definitions here
///EUP

namespace Motivate_BON {    DECLARE_BONEXTENSION( BON::Folder, ProcessFolderImpl, ProcessFolder ); }
namespace Motivate_BON {    DECLARE_ABSTRACT_BONEXTENSION( BON::Model, FragmentImpl, Fragment ); }
namespace Motivate_BON {    DECLARE_BONEXTENSION( Fragment, AbstractModelImpl, AbstractModel ); }
namespace Motivate_BON {    DECLARE_BONEXTENSION( Fragment, CodeImpl, Code ); }
namespace Motivate_BON {    DECLARE_BONEXTENSION( Fragment, ESMoLModelImpl, ESMoLModel ); }
namespace Motivate_BON {    DECLARE_BONEXTENSION( Fragment, SchedInputImpl, SchedInput ); }
namespace Motivate_BON {    DECLARE_BONEXTENSION( Fragment, SchedOutputImpl, SchedOutput ); }
namespace Motivate_BON {    DECLARE_BONEXTENSION( Fragment, SimulinkModelImpl, SimulinkModel ); }
namespace Motivate_BON {    DECLARE_ABSTRACT_BONEXTENSION( BON::Model, ActionImpl, Action ); }
namespace Motivate_BON {    DECLARE_BONEXTENSION( Action, CommandLineActionImpl, CommandLineAction ); }
namespace Motivate_BON {    DECLARE_BONEXTENSION( BON::Model, ProcessImpl, Process ); }
namespace Motivate_BON {    DECLARE_BONEXTENSION( BON::Model, TestImpl, Test ); }
namespace Motivate_BON {    DECLARE_BONEXTENSION( BON::Model, ViewerImpl, Viewer ); }
namespace Motivate_BON {    DECLARE_ABSTRACT_BONEXTENSION( BON::Atom, ChannelImpl, Channel ); }
namespace Motivate_BON {    DECLARE_BONEXTENSION( Channel, InputPortImpl, InputPort ); }
namespace Motivate_BON {    DECLARE_BONEXTENSION( Channel, OutputPortImpl, OutputPort ); }
namespace Motivate_BON {    DECLARE_BONEXTENSION( BON::Connection, ActionSequenceImpl, ActionSequence ); }
namespace Motivate_BON {    DECLARE_BONEXTENSION( BON::Connection, InputConnectionImpl, InputConnection ); }
namespace Motivate_BON {    DECLARE_BONEXTENSION( BON::Connection, OutputConnectionImpl, OutputConnection ); }
namespace Motivate_BON {    DECLARE_BONEXTENSION( BON::Connection, ViewerUseageImpl, ViewerUseage ); }


namespace Motivate_BON
{
//*******************************************************************
//   C  L  A  S  S   ProcessFolderImpl
//*******************************************************************
class ProcessFolderImpl :
	  public BON::FolderImpl
{
public:
	virtual void        initialize() { };
	virtual void        finalize() { };
	virtual void        accept( BON::Visitor *pVisitor);
	//
	// kind and subfolder getters
	virtual std::set<Motivate_BON::Process> getProcess();

	///BUP
	// add your own members here
	///EUP
}; // class
}  // namespace


namespace Motivate_BON
{
//*******************************************************************
//   C  L  A  S  S   FragmentImpl
//*******************************************************************
class FragmentImpl :
	  virtual public BON::ModelImpl
{
public:
	virtual void        initialize() { };
	virtual void        finalize() { };
	virtual void        accept( BON::Visitor *pVisitor);

	//
	// connectionEnd getters
	virtual std::set<Motivate_BON::OutputConnection>  getInOutputConnectionLinks();
	virtual std::multiset<Motivate_BON::InputPort>    getInputConnectionDsts();
	virtual std::set<Motivate_BON::InputConnection>   getOutInputConnectionLinks();
	virtual std::set<Motivate_BON::ViewerUseage>      getOutViewerUseageLinks();
	virtual std::multiset<Motivate_BON::OutputPort>   getOutputConnectionSrcs();
	virtual std::multiset<Motivate_BON::Viewer>       getViewerUseageDsts();
	//
	// attribute getters and setters
	virtual std::string getdateModified() ;
	virtual std::string getfullpath() ;
	virtual void        setdateModified( const std::string& val);
	virtual void        setfullpath( const std::string& val);

	///BUP
	// add your own members here
	///EUP
}; // class
}  // namespace


namespace Motivate_BON
{
//*******************************************************************
//   C  L  A  S  S   AbstractModelImpl
//*******************************************************************
class AbstractModelImpl :
	  public FragmentImpl
{
public:
	virtual void        initialize() { };
	virtual void        finalize() { };
	virtual void        accept( BON::Visitor *pVisitor);


	///BUP
	// add your own members here
	///EUP
}; // class
}  // namespace


namespace Motivate_BON
{
//*******************************************************************
//   C  L  A  S  S   CodeImpl
//*******************************************************************
class CodeImpl :
	  public FragmentImpl
{
public:
	virtual void        initialize() { };
	virtual void        finalize() { };
	virtual void        accept( BON::Visitor *pVisitor);


	///BUP
	// add your own members here
	///EUP
}; // class
}  // namespace


namespace Motivate_BON
{
//*******************************************************************
//   C  L  A  S  S   ESMoLModelImpl
//*******************************************************************
class ESMoLModelImpl :
	  public FragmentImpl
{
public:
	virtual void        initialize() { };
	virtual void        finalize() { };
	virtual void        accept( BON::Visitor *pVisitor);


	///BUP
	// add your own members here
	///EUP
}; // class
}  // namespace


namespace Motivate_BON
{
//*******************************************************************
//   C  L  A  S  S   SchedInputImpl
//*******************************************************************
class SchedInputImpl :
	  public FragmentImpl
{
public:
	virtual void        initialize() { };
	virtual void        finalize() { };
	virtual void        accept( BON::Visitor *pVisitor);


	///BUP
	// add your own members here
	///EUP
}; // class
}  // namespace


namespace Motivate_BON
{
//*******************************************************************
//   C  L  A  S  S   SchedOutputImpl
//*******************************************************************
class SchedOutputImpl :
	  public FragmentImpl
{
public:
	virtual void        initialize() { };
	virtual void        finalize() { };
	virtual void        accept( BON::Visitor *pVisitor);


	///BUP
	// add your own members here
	///EUP
}; // class
}  // namespace


namespace Motivate_BON
{
//*******************************************************************
//   C  L  A  S  S   SimulinkModelImpl
//*******************************************************************
class SimulinkModelImpl :
	  public FragmentImpl
{
public:
	virtual void        initialize() { };
	virtual void        finalize() { };
	virtual void        accept( BON::Visitor *pVisitor);


	///BUP
	// add your own members here
	///EUP
}; // class
}  // namespace


namespace Motivate_BON
{
//*******************************************************************
//   C  L  A  S  S   ActionImpl
//*******************************************************************
class ActionImpl :
	  virtual public BON::ModelImpl
{
public:
	virtual void        initialize() { };
	virtual void        finalize() { };
	virtual void        accept( BON::Visitor *pVisitor);

	//
	// connectionEnd getters
	virtual std::multiset<Motivate_BON::Action>       getActionSequenceDsts();
	virtual std::set<Motivate_BON::ActionSequence>    getActionSequenceLinks();
	virtual std::multiset<Motivate_BON::Action>       getActionSequenceSrcs();
	virtual std::set<Motivate_BON::ActionSequence>    getInActionSequenceLinks();
	virtual std::set<Motivate_BON::ActionSequence>    getOutActionSequenceLinks();
	//
	// kind and role getters
	virtual std::set<Motivate_BON::Channel> getChannel();
	virtual std::set<Motivate_BON::CommandLineAction> getCommandLineAction();
	virtual std::set<Motivate_BON::InputPort>         getInputPort();
	virtual std::set<Motivate_BON::OutputPort>        getOutputPort();

	///BUP
	// add your own members here
	///EUP
}; // class
}  // namespace


namespace Motivate_BON
{
//*******************************************************************
//   C  L  A  S  S   CommandLineActionImpl
//*******************************************************************
class CommandLineActionImpl :
	  public ActionImpl
{
public:
	virtual void        initialize() { };
	virtual void        finalize() { };
	virtual void        accept( BON::Visitor *pVisitor);


	///BUP
	// add your own members here
	///EUP
}; // class
}  // namespace


namespace Motivate_BON
{
//*******************************************************************
//   C  L  A  S  S   ProcessImpl
//*******************************************************************
class ProcessImpl :
	  virtual public BON::ModelImpl
{
public:
	virtual void        initialize() { };
	virtual void        finalize() { };
	virtual void        accept( BON::Visitor *pVisitor);

	//
	// attribute getters and setters
	virtual std::string getWorkingDirectory() ;
	virtual void        setWorkingDirectory( const std::string& val);
	//
	// kind and role getters
	virtual std::set<Motivate_BON::AbstractModel>     getAbstractModel();
	virtual std::set<Motivate_BON::ActionSequence>    getActionSequence();
	virtual std::set<Motivate_BON::Code>    getCode();
	virtual std::set<Motivate_BON::CommandLineAction> getCommandLineAction();
	virtual std::set<Motivate_BON::ESMoLModel>        getESMoLModel();
	virtual std::set<Motivate_BON::Fragment>          getFragment();
	virtual std::set<Motivate_BON::InputConnection>   getInputConnection();
	virtual std::set<Motivate_BON::OutputConnection>  getOutputConnection();
	virtual std::set<Motivate_BON::SchedInput>        getSchedInput();
	virtual std::set<Motivate_BON::SchedOutput>       getSchedOutput();
	virtual std::set<Motivate_BON::SimulinkModel>     getSimulinkModel();
	virtual std::set<Motivate_BON::Test>    getTest();
	virtual std::set<Motivate_BON::Viewer>  getViewer();
	virtual std::set<Motivate_BON::ViewerUseage>      getViewerUseage();

	///BUP
	// add your own members here
	///EUP
}; // class
}  // namespace


namespace Motivate_BON
{
//*******************************************************************
//   C  L  A  S  S   TestImpl
//*******************************************************************
class TestImpl :
	  virtual public BON::ModelImpl
{
public:
	virtual void        initialize() { };
	virtual void        finalize() { };
	virtual void        accept( BON::Visitor *pVisitor);


	///BUP
	// add your own members here
	///EUP
}; // class
}  // namespace


namespace Motivate_BON
{
//*******************************************************************
//   C  L  A  S  S   ViewerImpl
//*******************************************************************
class ViewerImpl :
	  virtual public BON::ModelImpl
{
public:
	virtual void        initialize() { };
	virtual void        finalize() { };
	virtual void        accept( BON::Visitor *pVisitor);

	//
	// connectionEnd getters
	virtual std::set<Motivate_BON::ViewerUseage>      getInViewerUseageLinks();
	virtual std::multiset<Motivate_BON::Fragment>     getViewerUseageSrcs();

	///BUP
	// add your own members here
	///EUP
}; // class
}  // namespace


namespace Motivate_BON
{
//*******************************************************************
//   C  L  A  S  S   ChannelImpl
//*******************************************************************
class ChannelImpl :
	  virtual public BON::AtomImpl
{
public:
	virtual void        initialize() { };
	virtual void        finalize() { };
	virtual void        accept( BON::Visitor *pVisitor);

	//
	// attribute getters and setters
	virtual std::string getfiletype() ;
	virtual void        setfiletype( const std::string& val);

	///BUP
	// add your own members here
	///EUP
}; // class
}  // namespace


namespace Motivate_BON
{
//*******************************************************************
//   C  L  A  S  S   InputPortImpl
//*******************************************************************
class InputPortImpl :
	  public ChannelImpl
{
public:
	virtual void        initialize() { };
	virtual void        finalize() { };
	virtual void        accept( BON::Visitor *pVisitor);

	//
	// connectionEnd getters
	virtual std::set<Motivate_BON::InputConnection>   getInInputConnectionLinks();
	virtual std::multiset<Motivate_BON::Fragment>     getInputConnectionSrcs();

	///BUP
	// add your own members here
	///EUP
}; // class
}  // namespace


namespace Motivate_BON
{
//*******************************************************************
//   C  L  A  S  S   OutputPortImpl
//*******************************************************************
class OutputPortImpl :
	  public ChannelImpl
{
public:
	virtual void        initialize() { };
	virtual void        finalize() { };
	virtual void        accept( BON::Visitor *pVisitor);

	//
	// connectionEnd getters
	virtual std::set<Motivate_BON::OutputConnection>  getOutOutputConnectionLinks();
	virtual std::multiset<Motivate_BON::Fragment>     getOutputConnectionDsts();

	///BUP
	// add your own members here
	///EUP
}; // class
}  // namespace


namespace Motivate_BON
{
//*******************************************************************
//   C  L  A  S  S   ActionSequenceImpl
//*******************************************************************
class ActionSequenceImpl :
	  virtual public BON::ConnectionImpl
{
public:
	virtual void        initialize() { };
	virtual void        finalize() { };
	virtual void        accept( BON::Visitor *pVisitor);

	//
	// connectionEnd getters
	virtual Motivate_BON::Action            getDst();
	virtual Motivate_BON::Action            getSrc();

	///BUP
	// add your own members here
	///EUP
}; // class
}  // namespace


namespace Motivate_BON
{
//*******************************************************************
//   C  L  A  S  S   InputConnectionImpl
//*******************************************************************
class InputConnectionImpl :
	  virtual public BON::ConnectionImpl
{
public:
	virtual void        initialize() { };
	virtual void        finalize() { };
	virtual void        accept( BON::Visitor *pVisitor);

	//
	// connectionEnd getters
	virtual Motivate_BON::InputPort         getDst();
	virtual Motivate_BON::Fragment          getSrc();

	///BUP
	// add your own members here
	///EUP
}; // class
}  // namespace


namespace Motivate_BON
{
//*******************************************************************
//   C  L  A  S  S   OutputConnectionImpl
//*******************************************************************
class OutputConnectionImpl :
	  virtual public BON::ConnectionImpl
{
public:
	virtual void        initialize() { };
	virtual void        finalize() { };
	virtual void        accept( BON::Visitor *pVisitor);

	//
	// connectionEnd getters
	virtual Motivate_BON::Fragment          getDst();
	virtual Motivate_BON::OutputPort        getSrc();

	///BUP
	// add your own members here
	///EUP
}; // class
}  // namespace


namespace Motivate_BON
{
//*******************************************************************
//   C  L  A  S  S   ViewerUseageImpl
//*******************************************************************
class ViewerUseageImpl :
	  virtual public BON::ConnectionImpl
{
public:
	virtual void        initialize() { };
	virtual void        finalize() { };
	virtual void        accept( BON::Visitor *pVisitor);

	//
	// connectionEnd getters
	virtual Motivate_BON::Viewer            getDst();
	virtual Motivate_BON::Fragment          getSrc();

	///BUP
	// add your own members here
	///EUP
}; // class
}  // namespace


///BUP
// add your additional class definitions here
///EUP

#endif // MOTIVATEBONX_H
