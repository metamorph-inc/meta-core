#ifndef MsgTypes_h__
#define MsgTypes_h__

#include "NWModel.h"

#include <string>
#include <iostream>

class BMsg;
typedef boost::shared_ptr< BMsg > BMsgPtr;
typedef std::set< BMsgPtr > BMsgSet;

class BMsg : public Msg
{
public:
	BMsg() { }
	BMsg( const std::string & name, BusPtr parent, unsigned long long msgsize,
			unsigned long long return_msgsize, double deadline ) : 
		Msg( name, parent, msgsize), _return_msg_size( return_msgsize ), 
			_deadline( deadline ) { }
	virtual ~BMsg() { }

	virtual void Print();
	virtual void SetupParent( const std::string & sndproc, const std::string & sndtask, 
								const std::string & rcvproc, const std::string & rcvtask );
	TaskPtr getReturnSender() { return *( _receiving_tasks.begin() ); }

	double getDeadline() { return _deadline; }
	unsigned long long getReturnMsgSize() { return _return_msg_size; }
	double getReturnMsgTransferTime() { return _parent->getMsgTransferTime( _return_msg_size ); }

protected:

	virtual void PrintOtherParameters() { }

	unsigned long long _return_msg_size;
	double _deadline;

private:
};

#endif // MsgTypes_h__