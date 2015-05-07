#include <vector>
#include <sstream>
#include <map>
#include <iostream>
#include <fstream>
#include <string>

#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <ctime>
#include <cmath>

#include "NetGraph.h"
#include "ConfigKeeper.h"
//#include "schedGen.h"

#include <boost/graph/graph_utility.hpp>
#include <boost/pending/disjoint_sets.hpp>
#include <boost/graph/incremental_components.hpp>

#include <boost/graph/erdos_renyi_generator.hpp>
#include <boost/random/linear_congruential.hpp>

#include <boost/graph/graphviz.hpp>

using namespace std;
using namespace boost;

NetGraph::NetGraph() : _ng( ConfigKeeper::inst()->getNetworks() + ConfigKeeper::inst()->getNodes())
{
	ConfigKeeper * cfg = ConfigKeeper::inst();
	// The general idea...
	// Create the proper number of networks and nodes
	unsigned long idx;

	cout << "Creating nodes and nets..." << endl;
	for ( idx = 0; idx < cfg->getNetworks(); idx++ )
	{
		_nets_nodes.push_back( new Net(idx) );
	}

	for ( ; idx < cfg->getNetworks() + cfg->getNodes(); idx++ )
	{
		_nets_nodes.push_back( new Node(idx) );
	}

	// Take the list of nodes and connect each one to at least one bus
	unsigned long nidx;
	cout << "Connecting nodes to nets..." << endl;
	if ( cfg->getNetworks() > 0 )
	{	
		for (nidx = cfg->getNetworks(); nidx < cfg->getNetworks() + cfg->getNodes(); nidx++)
		{
			cout << "Network " << nidx << ": ";
			// For the vertex, select a number of nets to connect to and
			// then make a list of them
			unsigned long numnets = _rnd.chooseInt(1, cfg->getMaxNetsPerNode()); 
			cout << numnets << " nets " << endl;
			std::vector< long > nets = _rnd.chooseInts(0, cfg->getNetworks()-1, numnets);
			for ( unsigned long eidx = 0; eidx < numnets; eidx++ )
			{
				cout << "   connecting (" << nidx << "," << nets[eidx] << ")" << endl;
				add_edge(nidx, nets[eidx], _ng);
			}
			cout << endl;
		}
		cout << endl;
	}

	// Continue adding node/bus edges until the whole graph is connected
	/*std::vector<size_type> rank(num_vertices(_ng));
	std::vector<NetworkVertex> parent(num_vertices(_ng));
	typedef size_type* Rank;
	typedef NetworkVertex* Parent;
	disjoint_sets<Rank, Parent> ds(&rank[0], &parent[0]);

	initialize_incremental_components(_ng, ds);
	incremental_components(_ng, ds);
	
	// Make a single connected component
	// @todo -- come back and look at cleaning this up:
	// The graph structure isn't using the type system.
	// This will not be quite as random as it should be.

	// For now we'll check and warn
	if ( ds.count_sets() > 1)
		cout << "The generated network is not connected!!! (" << ds.count_sets() << " comps)" << endl;

	/*while ( ds.count_sets() > 1 )
	{
		NetworkVertex vn1 = parent[ds.count_sets()-1];
		NetworkVertex vn2 = parent[ds.count_sets()-2];

		NetworkVertex 

		
	} */

	// Create a random number of tasks per processor
	cout << "Creating random tasks..." << endl;
	std::vector< double > dividers;
	dividers.push_back(1.0);
	dividers.push_back(2.0);
	dividers.push_back(4.0);
	unsigned long tidx = 0;
	// Counterintuitive syntax -- we're iterating over nodes
	for (nidx = cfg->getNetworks(); nidx < cfg->getNetworks() + cfg->getNodes(); nidx++)
	{
		unsigned long numtasks = _rnd.chooseInt(1, cfg->getMaxTasksPerNode());
		cout << "Node " << nidx << ": " << numtasks << endl;
		for ( idx = 0; idx < numtasks; idx++ )
		{
			Task t(tidx++, nidx);
			cout << "p";
			t._periodSecs = cfg->getSysPeriodSecs() / _rnd.chooseFromList(dividers);
			cout << "w";

			// Get a reasonable task exec length
			double res = cfg->getSysResolutionSecs();
			t._wcetSecs = max( t._periodSecs * _rnd.chooseDouble(cfg->getMinTaskUtil(), cfg->getMaxTaskUtil()),
								 res );
			cout << "c";
			Node * n = dynamic_cast<Node *>(_nets_nodes[nidx]);
			cout << "r";
			t._parent = n->id;
			cout << "t";
			n->_tasks.push_back(t);
			_tasks.push_back(t);

			cout << " added task " << t.id << " pd " << t._periodSecs << " et " << t._wcetSecs << " p " << t._parent << endl;
			//add_vertex(t.id, _tg);
		}
	}

	cout << "Creating subgraph structure..." << endl;
	_tg = new TaskGraph(_tasks.size());
	cout << "Creating subgraphs..." << endl;
	for ( unsigned long bidx = 0; bidx < cfg->getNetworks(); bidx++ )
	{
		_tg_subs.push_back(_tg->create_subgraph());
	}
	cout << "Finished creating subgraph structure." << endl;

	// Iterate over the buses...
	typedef graph_traits<NetworkGraph> GraphTraits;
	property_map<NetworkGraph, vertex_index_t>::type vindex = get(vertex_index, _ng);
	graph_traits<NetworkGraph>::adjacency_iterator ai;
	graph_traits<NetworkGraph>::adjacency_iterator ai_end;

	cout << "Creating task dependencies..."  << endl;
	unsigned long bidx;
	for ( bidx = 0; bidx < cfg->getNetworks(); bidx++ )
	{

		// Create a random graph between the tasks visible to that bus
		// (it could have almost any properties we want)
		
		//   Get the nodes for this bus
		//_tg_subs[bidx] = _tg.create_subgraph();
		for (tie(ai, ai_end) = adjacent_vertices(bidx, _ng); ai != ai_end; ++ai)
		{
			//   Get the tasks for this node
			Node * n = dynamic_cast<Node *>(_nets_nodes[vindex[*ai]]);
			for ( std::vector<Task>::iterator pT = n->_tasks.begin(); pT != n->_tasks.end(); pT++)
			{
				add_vertex( pT->id, _tg_subs[bidx] );
			}
		}

		//   Create a random graph using those tasks, and store a message length
		double p = cfg->getMsgProb();
		graph_traits<TaskGraph>::vertex_iterator vi, vi_end;
		graph_traits<TaskGraph>::vertex_iterator vi2, vi2_end;
		for ( tie(vi, vi_end) = vertices(_tg_subs[bidx]); vi != vi_end; ++vi)
		{
			for ( tie(vi2, vi2_end) = vertices(_tg_subs[bidx]); vi2 != vi2_end; ++vi2 )
			{
				if ( vi == vi2 )
					continue;

				if ( _rnd.choose(p) )
				{
					unsigned long msgln = _rnd.chooseInt(cfg->getMinMsgBytes(), cfg->getMaxMsgBytes());
					ostringstream msgtag;
					msgtag << bidx << "," << vindex[*vi] << "," << vindex[*vi2];
					cout << "Adding " << msgtag.str() << " with length " << msgln << endl;
					_tasks[vindex[*vi]]._msg_lengths[msgtag.str()] = msgln;
					add_edge(*vi, *vi2, _tg_subs[bidx]);
				}
			}
		}
	}

	cout << "Creation finished." << endl;
}

bool NetGraph::print_graph()
{
	ConfigKeeper * cfg = ConfigKeeper::inst();

	// Write it out to a file
	std::string filename(ConfigKeeper::inst()->getOutfileName());
	ofstream ofile( filename.c_str() );
	TaskGraph::const_children_iterator c_begin, c_end;
	unsigned long count = 0;
	for ( tie( c_begin, c_end ) = _tg->children(); c_begin != c_end; c_begin++ )
	{
		count++;
	}

	cout << " subgraph vector size: " << _tg_subs.size() << " children: " << count << endl;
	write_graphviz( ofile, *(_tg) );
	//for ( std::vector<TaskGraph>::iterator pSub = _tg_subs.begin(); pSub != _tg_subs.end(); pSub++ )
	//	write_graphviz( ofile, *(pSub) );

	ofile.close();
	
	return true;
}

// Output
bool NetGraph::print()
{
	cout << "Generating schedule file..." << endl;

	ConfigKeeper * cfg = ConfigKeeper::inst();
	ostringstream out;
	out << "% Schedule Generated by Tester" << endl;
	
	time_t tsecs;
	time( &tsecs );
	struct tm * timeinfo;
	timeinfo = localtime( &tsecs );

	char timestr[50];
	strftime(timestr, 99, "%H:%M:%S %a, %b %d, %Y", timeinfo);
	//out << "% at " << timestr << endl << endl;

	out << "Resolution " << fixed << cfg->getSysResolutionSecs() << "s" << endl << endl;

	std::vector< SystemObject * >::iterator nidx; 
	for ( nidx = _nets_nodes.begin() + cfg->getNetworks(); nidx != _nets_nodes.end(); nidx++ )
	{
		Node * n = dynamic_cast< Node * >(*nidx);
		out << "Proc P" << n->id << " 100MHz 0s 0s" << endl;

		std::vector< Task >::iterator pTsk;
		for ( pTsk = n->_tasks.begin(); pTsk != n->_tasks.end(); pTsk++ )
		{
			double freq = 1.0;
			if ( (*pTsk)._periodSecs > 1.0e-12 )
			{
				freq = 1.0 / (*pTsk)._periodSecs;
			}
			out << "Comp C" << (*pTsk).id << " =" << freq << "Hz " << (*pTsk)._wcetSecs << "s" << endl;
		}
		 
		out << endl;
	}
	
	unsigned long nnet, midx = 1;
	for ( nidx = _nets_nodes.begin(), nnet = 0; nnet < cfg->getNetworks(); nidx++, nnet++ )
	{
		Net * n = dynamic_cast< Net * >(*nidx);
		
		out << "Bus B" << n->id << " 1Mb 0s" << endl;

		// subgraph: _tg_subs[nnet]
		typedef property_map<TaskGraph, vertex_index_t>::type IndexMap;
		IndexMap index = get(vertex_index, _tg_subs[nnet]);

		graph_traits< TaskGraph >::edge_iterator ei, ei_end;
		for ( tie(ei, ei_end) = edges(_tg_subs[nnet]); ei != ei_end; ++ei )
		{
			unsigned long t1idx = index[source(*ei, _tg_subs[nnet])];
			unsigned long t2idx = index[target(*ei, _tg_subs[nnet])];
			Task * t1 = &_tasks[t1idx];
			Task * t2 = &_tasks[t2idx];
			ostringstream msgtag;

			msgtag << nnet << "," << t1idx << "," << t2idx;
			std::map< std::string, unsigned long >::iterator msgln = t1->_msg_lengths.find(msgtag.str());
			// @todo: This is a bad hack -- change to eliminate the subgraph structure.  It's not what we need.
			if ( msgln != (t1->_msg_lengths).end())
			{
				cout << "Retrieving tag " << msgtag.str() << " with length " << t1->_msg_lengths[msgtag.str()] << endl;
				out << "Msg M" << midx++ << " " << t1->_msg_lengths[msgtag.str()] << "B ";
				out << "P" << t1->_parent << "/" << "C" << t1->id << " ";
				out << "P" << t2->_parent << "/" << "C" << t2->id << " " << endl;
			}
		}

		out << endl;
	}

	// Write it out to a file
	std::string filename(ConfigKeeper::inst()->getOutfileName());
	ofstream ofile( filename.c_str() );
	ofile << out.str();
	ofile.close();

	//SchedGen sg(std::string("TestDesign"));
	cout << "File generated." << endl;

	return false;
}
