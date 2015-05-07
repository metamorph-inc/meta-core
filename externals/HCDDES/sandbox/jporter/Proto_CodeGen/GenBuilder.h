#ifndef _GEN_BUILDER_H_
#define _GEN_BUILDER_H_

#include <vector>

class AppConfig;
class NodeConfig;
class TaskConfig;
class PFGenerator;
class CompConfig;
class PortConfig;
class MsgConfig;

class GeneratorBase
{
public:
	virtual void AddAppNode( Node & n ) = 0;
	virtual void AddNodeTask( Node & n, Task & t ) = 0;
	virtual void AddTaskComponent( Task & t, Component & c ) = 0;
	virtual void AddComponentInPort( Component & c, InputPort & p ) = 0;
	virtual void AddComponentOutPort( Component & c, OutputPort & p ) = 0;
	virtual void AddTaskMessage( Task & t, Message & m ) = 0;
	
	virtual void AddBusMsg( Bus & b, Message & m ) = 0;
	virtual void MsgDependency( Task & st, Task & rt, Message & m ) = 0;
	virtual void AddLatency( Task & st, Task & rt, double latency ) = 0;

};

class GenBuilder : public virtual GeneratorBase
{
public:
	GenBuilder(std::string & name);
	virtual ~GenBuilder(void);

	// Note that these calls have a lot of state -- 
	// see the implementation for details
	virtual void AddAppNode( Node & n ); // Node in this application
	virtual void AddNodeTask( Node & n, Task & t ); // Task running on node
	virtual void AddTaskComponent( Task & t, Component & c ); // Component implementing task
	virtual void AddComponentInPort( Component & c, InputPort & p ); // Input port on component
	virtual void AddComponentOutPort( Component & c, OutputPort & p ); // Output port on component
	virtual void AddTaskMessage( Task & t, Message & m ); // Message implementing task comms
	
	virtual void AddBusMsg( Bus & b, Message & m ); // Message on bus
	virtual void MsgDependency( Task & st, Task & rt, Message & m ); // Sender, receiver, and msg
	virtual void AddLatency( Task & st, Task & rt, double latency ); // 1st/2nd task and latency

	void Generate();
	void Reset(); // Reset all of the state (start over)

protected:
	AppConfig * _app;
	NodeConfig * _ncfg;
	TaskConfig * _tcfg;
	CompConfig * _ccfg;
	PortConfig * _pcfg;
	MsgConfig * _mcfg;

	BusConfig * _bcfg;

	std::map< std::string, PortConfig * > _portbyname;
	//std::map<string, string> _type_map;

	std::vector<PFGenerator *> _pfgen; // Platform-specific generators (we may need a few)
};

// Actual class that does the generation -- 
// it may also keep Configuration that is non-standard (hence the GeneratorBase calls)
class PFGenerator : public virtual GeneratorBase
{
public:
	virtual void AddAppNode( Node & n ) { }
	virtual void AddNodeTask( Node & n, Task & t ) { }
	virtual void AddTaskComponent( Task & t, Component & c ) { }
	virtual void AddComponentInPort( Component & c, InputPort & p ) { }
	virtual void AddComponentOutPort( Component & c, OutputPort & p ) { }
	virtual void AddTaskMessage( Task & t, Message & m ) { }
	
	virtual void AddBusMsg( Bus & b, Message & m ) { }
	virtual void MsgDependency( Task & st, Task & rt, Message & m ) { }
	virtual void AddLatency( Task & st, Task & rt, double latency ) { } 

	virtual void Generate( ConfigKeeper * ck ) = 0;
	
protected:
	std::string _outputDir;
};


#endif // _GEN_BUILDER_H_