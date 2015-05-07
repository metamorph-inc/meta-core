#include "DiscTaskTypes.h"

#include <iostream>

using namespace std;

void DiscTTask::Print()
{
	cout << "    Discrete TTask " << _ref->Name() << " period " << _period_ticks << " ticks," 
		<< " duration " << _duration_ticks << " ticks" << endl;
}

void DiscTTask::DiscretizeTask( double res )
{
	DiscTask::DiscretizeTask( res );
}

void DiscPTask::Print()
{
	cout << "    Discrete PTask " << _ref->Name() << " period " << _period_ticks << " ticks," 
		<< " duration " << _duration_ticks << " ticks, deadline " << _deadline_ticks << " ticks" << endl;
}

void DiscPTask::DiscretizeTask( double res )
{
	DiscTask::DiscretizeTask( res );

	double deadline = _ptask_ref->getDeadline();
	_deadline_ticks = DiscProblem::Time2Ticks( deadline, res );
}

void DiscATask::Print()
{
	cout << "    Discrete ATask " << _ref->Name() << " period " << _period_ticks << " ticks," 
		<< " duration " << _duration_ticks << " ticks, deadline " << _deadline_ticks << " ticks" << endl;
}

void DiscATask::DiscretizeTask( double res )
{
	DiscTask::DiscretizeTask( res );

	double deadline = _atask_ref->getDeadline();
	_deadline_ticks = DiscProblem::Time2Ticks( deadline, res );
}



