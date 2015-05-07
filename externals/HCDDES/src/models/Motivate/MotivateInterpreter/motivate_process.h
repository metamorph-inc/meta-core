#ifndef __MOTIVATE_PROCESS_H__
#define __MOTIVATE_PROCESS_H__


/*** Included Header Files ***/
#include "motivate.h"


namespace Motivate {


/*** Class Predefinitions ***/
class Action;


/************************************************************************************/


class Process {
protected:
	BON::Model							_process;
	std::vector< Action* >				_actions;
	std::vector< Action* >::iterator	_currentAction;
	// Private Methods
	std::map< std::string,std::string > BuildParamMap( void );
	bool SetRootDirectory( const std::string &rootDirectory );
	// Deny access to default constructor
	Process();
public:
	// Constructors and Destructors
	Process( const BON::Model &process, const std::vector< BON::Model > &actions );
	~Process();

	// Public Methods
	Action* ExecuteStep( std::map< std::string,std::string > paramMap );
	Action* CurrentAction(void)			{ return *this->_currentAction; }
	Action* FindAction( const std::string &name );
};


/************************************************************************************/


}	// End of Motivate namespace


#endif //__MOTIVATE_PROCESS_H__

