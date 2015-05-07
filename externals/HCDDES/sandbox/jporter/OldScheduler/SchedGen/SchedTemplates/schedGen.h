#ifndef _SCHED_GEN_H_
#define _SCHED_GEN_H_

#include <vector>
#include <map>
#include <set>
#include <sstream>

class EltInfo
{
public:
	std::string id;
};

class NodeInfo : public virtual EltInfo
{
public:
	std::string speed;
	std::string sendohd;
	std::string recvohd;
};

class TaskInfo : public virtual EltInfo
{
public:
	std::string basename;
	std::string freq;
	std::string wcexectime;
};

class BusInfo : public virtual EltInfo
{
public:
	std::string rate;
	std::string setuptime;
};

class MsgInfo : public virtual EltInfo
{
public:
	std::string basename;
	std::string msgsize;
	std::string sender;
	std::string receivers;
};

typedef struct
{
	std::string latency;
	std::string sender;
	std::string receiver;
} LatencyInfo;

class SchedGen 
{
public:
	SchedGen() { }
	SchedGen( const std::string & designname );
	virtual ~SchedGen() { }

	void AddNode( const std::string & id, 
				  const std::string & node, const std::string & freq, 
				  const std::string & sndoverhead, const std::string & rcvoverhead );
	void AddTask( const std::string & id, 
				  const std::string & node, const std::string & task, 
				  const std::string & frq, const std::string & wcexectime );

	void AddBus( const std::string & id, const std::string & name, 
				 const std::string & rate, const std::string & setuptime );
	void AddMsg( const std::string & id, const std::string & bus, 
				 const std::string & msg, const std::string & msgsize);
	void AddMsgSndr( const std::string & bus, const std::string & msg, 
					 const std::string & sndnode, const std::string & sndtask );

	void AddMsgRcvr( const std::string & bus, const std::string & msg, 
				     const std::string & rcvnode, const std::string & rcvtask );

	void AddLatency( const std::string & sndnode, const std::string & sndtask,
					 const std::string & rcvnode, const std::string & rcvtask, 
					 const std::string & latency );

	void GenerateSched();  // filenames are based on the design name
	void GenerateIDs();

protected:
	void CheckUnits( std::string & str, const std::string & unitstr );

	std::string _designname;
	std::string _gentime;
	double _max_tick;

	std::multimap<std::string, std::string> _nodetasks;
	std::map< std::string, NodeInfo > _nodeinfo;
	std::map< std::string, TaskInfo > _taskinfo;

	std::map< std::string, std::set< std::string > > _busmsgs;
	std::map< std::string, BusInfo > _businfo;
	std::map< std::string, MsgInfo > _msginfo;

	std::vector< LatencyInfo > _latencyinfo;

	std::string _logstr;
};

#endif // _SCHED_GEN_H_
