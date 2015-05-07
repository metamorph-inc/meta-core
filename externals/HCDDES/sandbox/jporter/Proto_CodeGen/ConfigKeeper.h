#ifndef _CONFIG_KEEPER_H_
#define _CONFIG_KEEPER_H_

#include <ctime>
#include <list>

#include <google/template.h>

class NodeConfig;
class TaskConfig;
class CompConfig;
class MsgConfig;
class ArgConfig;
class PortConfig;
class BusConfig;

class ConfigKeeper
{
public:
	ConfigKeeper();
	ConfigKeeper( google::TemplateDictionary * d ) : _dict(d) { }
	virtual ~ConfigKeeper();

	virtual google::TemplateDictionary * GetConfig() { return _dict; }
	virtual google::TemplateDictionary * NewSection( std::string & name ) { return _dict->AddSectionDictionary( name ); }
	virtual google::TemplateDictionary * NewSection( const char * name ) { return _dict->AddSectionDictionary( name ); }

protected:
	google::TemplateDictionary * _dict;
};

class AppConfig : public virtual ConfigKeeper
{
public:
	AppConfig(void) { _dict = new google::TemplateDictionary( "App" ); }
	virtual ~AppConfig(void) {}

	void SetAppName( std::string & name ) { _dict->SetValue( "APPNAME", name ); }
	void SetTime() { std::time_t ltime; std::time( &ltime ); _dict->SetValue( "GENTIME", std::ctime( &ltime )); }
	NodeConfig * NewNode();
};


class NodeConfig : public virtual ConfigKeeper
{
public:
	NodeConfig( google::TemplateDictionary * d ) : ConfigKeeper( d ) { }

	void SetNodeName( std::string & name ) { _dict->SetValue( "NODENAME", name ); }
	void SetCPUName( std::string & name ) { _dict->SetValue( "CPUNAME", name ); }
	TaskConfig * NewTask();
};

class TaskConfig : public virtual ConfigKeeper
{
public:
	TaskConfig( google::TemplateDictionary * d ) : ConfigKeeper( d ) { }

	void SetTaskName( std::string & name ) { _dict->SetValue( "TASKNAME", name ); }
	void SetWCETus( unsigned long long wcet ) { _dict->SetIntValue( "WCEXECTIME", wcet ); }
	void SetFrequency( double )
	CompConfig * NewComp();
};

class CompConfig : public virtual ConfigKeeper
{
public:
	CompConfig( google::TemplateDictionary * d ) : ConfigKeeper( d ) { }

	void SetCompName( std::string & name ) { _dict->SetValue( "COMPNAME", name ); }
	void SetCompID( std::string & uniqueID ) { _dict->SetValue( "COMPID", uniqueID ); }
	//void AddInputArgument( std::string & name ) { }
	//void AddOutputArgument( std::string & name ) { }
	MsgConfig * NewMsg();
	PortConfig * NewPort();

protected:
	std::list< std::pair< std::string, std::string > > _arglist;
};

class ArgConfig : public virtual ConfigKeeper
{
public:
	ArgConfig( google::TemplateDictionary * d ) : ConfigKeeper( d ) { }

	
};

class MsgConfig : public virtual ConfigKeeper
{
public:
	MsgConfig( google::TemplateDictionary * d ) : ConfigKeeper( d ) { }

	void SetMsgName( std::string & name ) { _dict->SetValue( "MSGNAME", name ); }

	// For now these have no fields, so we don't do anything special for them.
	// When we need particular fields, then we can expand them.
	void NewRecvMsg() { this->NewSection( "RECEIVEDMSG_SECTION" ); }
	void NewSentMsg() { this->NewSection( "SENTMSG_SECTION" ); }
	void NewIoMsg() { this->NewSection( "IOMSG_SECTION" ); }
};

class PortConfig : public virtual ConfigKeeper
{
public:
	PortConfig( google::TemplateDictionary * d ) : ConfigKeeper( d ) { }

	void SetPortName( std::string & name ) { _dict->SetValue( "PORTNAME", name ); }
	void SetDataType( std::string & dt ) { _dict->SetValue( "DATATYPE", dt ); }
	void SetMsgName( std::string & name ) { _dict->SetValue( "MSGNAME", name ); }
	void SetDirection( bool input ) { }
};

class BusConfig : public virtual ConfigKeeper
{
public:
	BusConfig( google::TemplateDictionary * d ) : ConfigKeeper( d ) { }
	void SetBusName( std::string & name ) { _dict->SetValue( "BUSNAME", name ); }
};

#endif // _CONFIG_KEEPER_H_
