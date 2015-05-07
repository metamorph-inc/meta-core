#ifndef DiscMsgTypes_h__
#define DiscMsgTypes_h__

#include "MsgTypes.h"
#include "DiscNWModel.h"

class DiscBMsg;

typedef boost::shared_ptr< DiscBMsg > DiscBMsgPtr;
typedef std::set< DiscBMsgPtr > DiscBMsgSet;

class DiscBMsg : public DiscMsg
{
public:

	DiscBMsg() { }
	DiscBMsg( BMsgPtr m, DiscBusPtr parent, DiscTaskPtr dt, DiscTaskPtr rdt ) :
		DiscMsg( m, parent, dt ), _bmsg_ref( m ), _return_sending_task( rdt ) { }
	virtual ~DiscBMsg() { }

	virtual const DiscBusPtr parent() { return _parent; }

	virtual void Print();

	virtual Ticks getReturnMsgSizeTicks() { return _return_msgsize_ticks; }
	virtual Ticks getDeadlineTicks() { return _deadline_ticks; }

	virtual void DiscretizeMsg( double res );

protected:

	Ticks _return_msgsize_ticks;
	Ticks _deadline_ticks;

	BMsgPtr	_bmsg_ref;
	DiscTaskPtr _return_sending_task;

private:
	
};




#endif // DiscMsgTypes_h__