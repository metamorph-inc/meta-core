// Implementations for Graph Manager Class

#include "GraphMgr.h"

using namespace boost;

GraphMgr::GraphMgr()
{

}

void GraphMgr::AddTask(std::string &name, long length)
{
	SchedulableTask * st = new SchedulableTask( name, length );
	TaskGraph::vertex_descriptor v = add_vertex( *st, _tg );
	_tasks[ name ] = v;
}

void GraphMgr::AddMsg(std::string &name, long length)
{
	SchedulableMsg * sm = new SchedulableMsg( name, length );
	TaskGraph::vertex_descriptor  v = add_vertex( *sm, _tg );
	_msgs[ name ] = v;
}

void GraphMgr::AddSender(std::string &sndr, std::string &msg)
{
	TaskGraph::vertex_descriptor s = _tasks[ sndr ];
	TaskGraph::vertex_descriptor m = _msgs[ msg ];

	add_edge( s, m, _tg );
}

void GraphMgr::AddReceiver(std::string &msg, std::string &rcvr)
{
	TaskGraph::vertex_descriptor m = _msgs[ msg ];
	TaskGraph::vertex_descriptor r = _tasks[ rcvr ];

	add_edge( m, r, _tg );
}
