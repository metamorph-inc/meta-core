#ifndef _MESSAGE_KEEPER_H_
#define _MESSAGE_KEEPER_H_

#include "IndexMerge.h"

#include <map>

/* Here's the approach:
   We basically need something that can take a ComponentRef / MessageRef pair,
   and look up a representative object that has related information -- most
   importantly, an index for an attached semantic message (well, we can look
   it up for them just as easily). */

typedef std::pair< ID_TYPE, ID_TYPE > IDX_TYPE;
typedef std::map< IDX_TYPE, ID_TYPE > MSG_MAP_TYPE;

// KMS: GetMessage is defined to GetMessageA in windows.h.
// #undef GetMessage

class MessageKeeper
{
protected:
	// (comprefid, msgrefid) -> msgid 
	MSG_MAP_TYPE _msgidx;

	// Singleton pattern
	MessageKeeper(void)				{ }
	static MessageKeeper				*_inst;

public:

	virtual ~MessageKeeper() { }
	static MessageKeeper *Inst();

	void AddMessage( ID_TYPE comprefid, ID_TYPE msgrefid, ID_TYPE msgid );
	bool SeenMessage( ID_TYPE comprefid, ID_TYPE msgrefid );
	ID_TYPE GetMessage( ID_TYPE comprefid, ID_TYPE msgrefid );

};

#endif // _MESSAGE_KEEPER_H_