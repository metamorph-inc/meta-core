#include <MessageKeeper.h>

using namespace std;

/*** Static Member Initialization ***/
MessageKeeper* MessageKeeper::_inst = NULL;


/***************************************************************************************/


MessageKeeper* MessageKeeper::Inst() {
	if ( MessageKeeper::_inst == NULL ) {
		// We don't catch the exception, since we're dead if this fails.
		MessageKeeper::_inst = new MessageKeeper();
	}
	return MessageKeeper::_inst;
}


/***************************************************************************************/

void MessageKeeper::AddMessage( ID_TYPE comprefid, ID_TYPE msgrefid, ID_TYPE msgid ) {

	IDX_TYPE idx = make_pair( comprefid, msgrefid );
	_msgidx[ idx ] = msgid;
}

bool MessageKeeper::SeenMessage( ID_TYPE comprefid, ID_TYPE msgrefid ) {

	MSG_MAP_TYPE::iterator mmIter = _msgidx.find( make_pair( comprefid, msgrefid ) );
	return ( mmIter != _msgidx.end() );
}

ID_TYPE MessageKeeper::GetMessage( ID_TYPE comprefid, ID_TYPE msgrefid ) {

	MSG_MAP_TYPE::iterator mmIter = _msgidx.find( make_pair( comprefid, msgrefid ) );
	if ( mmIter != _msgidx.end() ) {

		return mmIter->second;
	}

	return (unsigned long long) 0;
}

