#ifndef __MOTIVATE_COMMANDLINEACTION_H__
#define __MOTIVATE_COMMANDLINEACTION_H__


/*** Included Header Files ***/
#include "motivate.h"
#include "action.h"


namespace Motivate {


/************************************************************************************/


class CommandLineAction : public Action {
protected:
	// Deny access to default constructor
	CommandLineAction();
public:
	// Constructors and Destructors
	CommandLineAction( const BON::Model &action, Process *process ) : ::Action( action, process ) { }
	virtual ~CommandLineAction()			{ }
	// Virtual Functions
	virtual ActionStatus Execute( std::map< std::string,std::string > params );
};


/************************************************************************************/


}	// End of Motivate namespace


#endif //__MOTIVATE_COMMANDLINEACTION_H__

