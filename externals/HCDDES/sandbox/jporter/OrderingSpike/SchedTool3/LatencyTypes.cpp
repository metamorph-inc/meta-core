#include "LatencyTypes.h"

using namespace std;

void LatencyChain::AddFirstElement( const std::string & procname, const std::string & first_task )
{
	_elements.push_back( _parent->getTaskByName( procname, first_task ) );
	_next_element = LATENCY_MSG;
}

void LatencyChain::AddIntermediateElement( const std::string & hw_resource_name, const std::string & element_name )
{
	if ( _next_element == LATENCY_MSG )
	{
		_elements.push_back( _parent->getMsgByName( hw_resource_name, element_name ) );
		_next_element = LATENCY_TASK;
	}
	else if ( _next_element == LATENCY_TASK )
	{
		_elements.push_back( _parent->getTaskByName( hw_resource_name, element_name ) );
		_next_element = LATENCY_MSG;
	}
}

void LatencyChain::AddLastElement( const std::string & procname, const std::string & last_task )
{
	// _next_element should be LATENCY_TASK at this point, or else...
	_elements.push_back( _parent->getTaskByName( procname, last_task ) );
}

void LatencyBound::Print()
{
	cout << "        " << "LatencyBound " << _latency; 
	cout << " from task " << _sending_task->Name();
	cout << " to task " << _receiving_task->Name() << endl;
}

void LatencyChain::Print()
{
	cout << "        "  << "LatencyChain " << _latency << endl;
	unsigned long idx;
	for ( idx = 0; idx < _elements.size(); idx++ )
	{
		cout << "            " << _elements[idx]->Name() << endl;
	}

}

