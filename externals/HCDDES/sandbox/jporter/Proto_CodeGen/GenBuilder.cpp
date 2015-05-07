#include "GenBuilder.h"
#include "ConfigKeeper.h"

// Macro to iterate over all of the platform-specific generators
#define APPLY_TO_PFG_LIST( callname, ... ) \
	for( std::vector<PFGenerator *>::iterator pPfg = _pfgen->begin(); pPfg != _pfgen->end(); pPfg++ ) \
		(*pPfg)->callname( __VA_ARGS__ )

GenBuilder::GenBuilder(std::string & name)
{
	// _type_map["None"] = "int";
	//_type_map["Bit"] = "char";
	//_type_map["Byte"] = "char";
	//_type_map["HalfWord"] = "short";
	//_type_map["Word"] = "int";
	//_type_map["DoubleWord"] = "long";
	_app->SetAppName( name );
}

GenBuilder::~GenBuilder(void)
{
	Reset();
}

GenBuilder::Generate(void)
{
	APPLY_TO_PFG_LIST( Generate, _app );
}

GenBuilder::Reset(void)
{
}



// Sequentially fill in the sections of the dictionary
void GenBuilder::AddAppNode( Node & n )
{
	_ncfg = _app->NewNode();
	_ncfg->SetNodeName( (const std::string &) n.name() );
	APPLY_TO_PFG_LIST( AddAppNode, n );
}

void GenBuilder::AddNodeTask( Node & n, Task & t )
{
	_tcfg = _ncfg->NewTask();
	_tcfg->SetTaskName( (const std::string &) t.name() );
	_tcfg->SetWCETus( (unsigned long long) t.WCET() );
	APPLY_TO_PFG_LIST( AddNodeTask, n, t );
}

void AddTaskComponent( Task & t, Component & c )
{
	_ccfg = _tcfg->NewComp();
	_ccfg->SetCompName( (const std::string &) c.name() );
	_ccfg->SetUniqueID( (const std::string &) c.uniqueID() );
	APPLY_TO_PFG_LIST( AddTaskComponent, t, c );
}

void AddComponentInPort( Component & c, InputPort & p )
{
	_pcfg = _ccfg->NewPort();
	_pcfg->SetPortName( (const std::string &) p.name() );
	//_pcfg->SetDataType( type_map[(const std::string &) p.DataType()] );
	_pcfg->SetDirection( true );
	_portbyname[(const std::string &) p.name()] = _pcfg;
	APPLY_TO_PFG_LIST( AddComponentInPort, c, p );
}

void AddComponentOutPort( Component & c, OutputPort & p )
{
	_pcfg = _ccfg->NewPort();
	_pcfg->SetPortName( (const std::string &) p.name() );
	//_pcfg->SetDataType( type_map[(const std::string &) p.DataType()] );
	_pcfg->SetDirection( false );
	_portbyname[(const std::string &) p.name()] = _pcfg;
	APPLY_TO_PFG_LIST( AddComponentOutPort, c, p );
}


void GenBuilder::AddBusMsg( Bus & b, Message & m )
{
	_mcfg = _ccfg->NewMsg();
	_mcfg->SetMsgName( (const std::string &) m.name() );
	APPLY_TO_PFG_LIST( AddBusMsg, b, m );
}

void GenBuilder::AddMsgPort(Message & m, Port & p )
{
	std::map< std::string, PortConfig * > pIter = _portbyname.find((const std::string &) p.name());
	if ( pIter != _portbyname.end() )
	{
		PortConfig * _p = *pIter;
		_p->SetMsgName( (const std::string &) m.name() );
		_p->SetDataType( (const std::string &) m.MsgDataType() );
	}
	APPLY_TO_PFG_LIST( AddMsgPort, m, p );
}

void GenBuilder::AddLatency( Task & st, Task & rt, double latency )
{
	APPLY_TO_PFG_LIST( AddLatency, st, rt, latency );
}
