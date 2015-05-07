
#ifndef _DEBUG_SCHED_MODEL_H_
#define _DEBUG_SCHED_MODEL_H_

#include "schedModel.h"
#include "defaultSchedModel.h"
#include <vector>
#include <map>
#include <set>

class DebugSchedResults : public virtual SchedResults
{
public:
	std::vector< std::string > rslts;
	bool feasible;
	bool empty;
};

// This version should look for unsatisfiable partial models
class DebugSchedModel :
	public virtual DefaultSchedModel
{
public:
	DebugSchedModel();
	virtual ~DebugSchedModel(void);

	virtual SchedResults * GetResultsObject() { return (SchedResults *) new DebugSchedResults(); }

	// Template Pattern: calling Run() invokes each of these virtual methods in sequence
	//virtual void CalcHyperperiod();
	virtual void BuildConstraints();
	virtual void InvokeSolver();
	virtual bool CheckResults();
	virtual void CollectResults( SchedResults * rslt );
	virtual void OutputResults( SchedResults * rslt );

protected:
	
	std::vector< std::string > _serial_constraints;
	std::vector< std::string > _order_constraints;
	std::vector< std::string > _latency_constraints;

	std::set< std::string > _debugList;
	bool _debugAll;
};

#endif // _DEBUG_SCHED_MODEL_H_
