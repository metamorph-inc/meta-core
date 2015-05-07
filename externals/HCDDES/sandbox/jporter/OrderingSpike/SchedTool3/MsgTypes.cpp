#include "MsgTypes.h"

using namespace std;

void BMsg::SetupParent( const std::string & sndproc, const std::string & sndtask, 
						const std::string & rcvproc, const std::string & rcvtask )
{
	_parent->AddMsg( shared_from_this() );
	_sending_task = _parent->getTaskByName( sndproc, sndtask );
	_receiving_tasks.insert( _parent->getTaskByName( rcvproc, rcvtask ) );
}

void BMsg::Print()
{
	cout << "        " << "BMsg " << Name() << endl;
	cout << "            " << " sender " << _sending_task->Name()  << " msg size " << getMsgSize() << " deadline " << getDeadline() << endl;
	cout << "            " << " receiver " << (*_receiving_tasks.begin())->Name() << " return msg size " << getReturnMsgSize() << endl;
}


