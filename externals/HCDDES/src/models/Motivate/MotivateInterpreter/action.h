#ifndef __MOTIVATE_ACTION_H__
#define __MOTIVATE_ACTION_H__


/*** Included Header Files ***/
#include "motivate.h"


namespace Motivate {


/*** Class Predeclarations ***/
class Process;


/************************************************************************************/


// Enum to hold the status of the action
enum ActionStatus { Reset, Success, Failure };


class Action {
protected:
	BON::Model						_action;
	Process							*_process;
	std::string						_execStr;
	std::string						_results;
	ActionStatus					_status;
	// Deny access to default constructor
	Action();
public:
	// Constructors and Destructors
	Action( const BON::Model &action, Process *process ) :
	  _action(action), _process( process ), _execStr( "" ), _results( "" ), _status( Reset ) { }
	virtual ~Action()			{ }

	// Accessors
	inline BON::Model Model(void)		{ return this->_action; }
	inline std::string ExecutedString(void) { return this->_execStr; }
	inline std::string Results(void)	{ return this->_results; }
	inline ActionStatus Status(void)	{ return this->_status; }
	inline std::string Name(void)		{ return this->_action->getName(); }
	inline std::string Type(void)		{ return "TemplateAction"; }
	void SetStatus( const bool &status)	{ if ( status ) this->_status = Success; 
										  else this->_status = Failure; }
	void SetResults( const std::string &results)	{ this->_results = results; }

	// Virtual Functions
	virtual ActionStatus Execute( std::map< std::string,std::string > params )=0;
};


/************************************************************************************/


}	// End of Motivate namespace


#endif //__MOTIVATE_ACTION_H__

