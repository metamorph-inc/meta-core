#ifndef __MOTIVATE_TEMPLATEACTION_H__
#define __MOTIVATE_TEMPLATEACTION_H__


/*** Included Header Files ***/
#include "motivate.h"
#include "action.h"


namespace Motivate {


/************************************************************************************/


class TemplateAction : public Action {
protected:
	std::map< std::string,std::string >				_paramMap;
	// Build the local parameter values
	void BuildParameterMap(void);
	// Language Specific Execution Methods
	bool ExecutePython(const std::string &script);
	bool ExecuteCommandLine(const std::string &script);
	// Deny access to default constructor
	TemplateAction();
public:
	// Constructors and Destructors
	TemplateAction( Process *process, const BON::Model &action ) : ::Action( action, process ), _paramMap() { }
	virtual ~TemplateAction()						{ }

	// Virtual Functions
	virtual ActionStatus Execute( std::map< std::string,std::string > params );
};


/************************************************************************************/


}	// End of Motivate namespace


#endif //__MOTIVATE_TEMPLATEACTION_H__

