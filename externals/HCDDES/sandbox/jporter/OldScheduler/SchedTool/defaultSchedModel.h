
#ifndef _DEFAULT_SCHED_MODEL_H_
#define _DEFAULT_SCHED_MODEL_H_

#include "SchedModel.h"

class DefaultSchedResults : public virtual SchedResults
{
public:
	std::map< std::string, std::map< double, std::string > > schedule;
};

class DefaultSchedModel :
	public virtual SchedModel
{
public:
	DefaultSchedModel();
	virtual ~DefaultSchedModel();

	virtual SchedResults * GetResultsObject() { return (SchedResults *) new DefaultSchedResults(); }

	// Template Pattern: calling Run() invokes each of these virtual methods in sequence
	virtual void CalcHyperperiod();
	virtual void BuildConstraints();
	virtual void InvokeSolver();
	virtual bool CheckResults();
	virtual void CollectResults( SchedResults * rslt );
	virtual void OutputResults( SchedResults * rslt );

protected:
	std::map< std::string, std::string > _inst_taskmap;
	std::map< std::string, std::string > _inst_busmap;
	std::map< std::string, unsigned int > _num_inst;

	bool _foundresults;
};

#endif // _DEFAULT_SCHED_MODEL_H_