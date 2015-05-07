#ifndef LatencyTypes_h__
#define LatencyTypes_h__

#include "NWModel.h"

#include <string>
#include <iostream>
#include <vector>

class LatencyBound;
typedef boost::shared_ptr< LatencyBound > LatencyBoundPtr;
typedef std::set< LatencyBoundPtr > LatencyBoundSet;

class LatencyBound : public Latency
{
public:
	LatencyBound() { }
	LatencyBound( ProblemPtr parent, double latency,
					const std::string & sndproc, const std::string & sndtask ,
					const std::string & rcvproc, const std::string & rcvtask ) : 
		Latency( parent, latency ), 
		_sending_task( _parent->getTaskByName( sndproc, sndtask ) ),
		_receiving_task( _parent->getTaskByName( rcvproc, rcvtask ) ) { }
	virtual ~LatencyBound() { }

	virtual void Print();

protected:

	virtual void PrintOtherParameters() { }

	TaskPtr _sending_task;
	TaskPtr _receiving_task;

private:
};

class LatencyChain;
typedef boost::shared_ptr< LatencyChain > LatencyChainPtr;
typedef std::set< LatencyChainPtr > LatencyChainSet;

class LatencyChain : public Latency
{
public:
	LatencyChain() { }
	LatencyChain( ProblemPtr parent, double latency ) : 
		Latency( parent, latency ), _next_element( LATENCY_TASK ) { }
	virtual ~LatencyChain() { }

	typedef enum { LATENCY_TASK, LATENCY_MSG } elementType;
	

	virtual void Print();
	void AddFirstElement( const std::string & procname, const std::string & first_task );
	void AddIntermediateElement( const std::string & hw_resource_name, const std::string & element_name );
	void AddLastElement( const std::string & procname, const std::string & last_task );

protected:

	virtual void PrintOtherParameters() { }

	std::vector< SoftwareElementPtr > _elements;

private:
	elementType _next_element;
};

#endif // LatencyTypes_h__