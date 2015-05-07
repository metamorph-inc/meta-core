#include "resource.h"
// Messages to be communicated between Observer and Notifier
class IMSG
{
public:
	UINT		fromCtrlId;				// Control Id of the Notifier = Resource Id Associated with the class's object
	CString		srcPortName;
	CString		srcPortType;
	CString		dstPortName;
	CString		dstPortType;
	//CString		linkType;
	enum		ActionType
	{									// RELEVANT Msg Attr:
				DEFAULT,
				ADD_PORT,				// srcPortName, srcPorType , fromCtrlId
				//FIND_PORT,				// srcPortName, fromCtrlId
				DELETE_PORT_LINK,		// srcPortName, dstPortName
				MODIFY_PORT_NAME,		// srcPortName, fromCtrlId
				MODIFY_PORT_TYPE,		// srcPortType, fromCtrlId
				MODIFY_PORT,			// srcPortName, srcPortType, fromCtrlId
				ADD_PORT_LINK			// linkType
	};

	ActionType	actionType;
	IMSG()
	{
		fromCtrlId = ID_DUMMY;
		srcPortName = _T("");
		srcPortType = _T("");
		dstPortName = _T("");
		dstPortType = _T("");
		actionType = DEFAULT;
	}
};


/*
MODIFY_PORT_TYPE:
									IMSG: 
									SRCPORTTYPE		new
									DSTPORTNAME		old
MODIFY_PORT_NAME:
									IMSG: 
									SRCPORTNAME		new
									DSTPORTNAME		old
									
ADD_PORT:
									IMSG: 
									SRCPORTNAME		new
									SRCPORTTYPE		new
*/