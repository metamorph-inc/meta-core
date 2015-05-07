#include "DiscMsgTypes.h"

#include <iostream>

using namespace std;

void DiscBMsg::Print()
{
	cout << "    Discrete Blocking Message " << _ref->Name() << " length " << _msgsize_ticks << " ticks," << endl;
	cout << "        return msg length " << _return_msgsize_ticks << " ticks, deadline " << _deadline_ticks << " ticks" << endl;
}


void DiscBMsg::DiscretizeMsg( double res )
{
	DiscMsg::DiscretizeMsg( res );

	Ticks bus_granularity = _parent->getGranularityTicks();
	double return_msg_time = _bmsg_ref->getReturnMsgTransferTime();

	_return_msgsize_ticks = DiscProblem::Time2Ticks( return_msg_time, res, bus_granularity );
	_return_msgsize_ticks = std::max( _return_msgsize_ticks, (Ticks) 1 );

	double deadline = _bmsg_ref->getDeadline();
	_deadline_ticks = DiscProblem::Time2Ticks( deadline, res );
}
