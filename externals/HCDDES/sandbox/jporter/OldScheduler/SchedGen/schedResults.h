#ifndef _SCHED_RESULTS_H_
#define _SCHED_RESULTS_H_


/*** Included Header Files ***/
#include <map>
#include <vector>


/***************************************************************************************/


class SchedKeeper
{
private:
	bool										_debug;
	std::string									_hyperperiodStr;
	double										_hyperperiodSec;
	std::map<std::string, std::string>			_elements;
	std::vector<std::string>					_harewareElements;

	// Private Methods
	SchedKeeper();													//!< Deny access to default constructor
	bool ParseResults(const std::string &resultFile);				//!< Parse the results file
	bool PopulateEsmol(const std::string &esmolModel);				//!< Populate tasks and messages in ESMoL model
	bool PopulateAbstract(const std::string &abstractModel);		//!< Populate tasks, messages, and bus in ESMoL_Abstract model
public:
	// Constructors and Destructors	
	SchedKeeper(const std::string &resultFile, const std::string &esmolModel, const std::string &abstractModel, const bool &debug=false);
	~SchedKeeper() { }
};


class SchedKeeperException
{
public:
	SchedKeeperException(std::string & reason ) : _reason(reason) { }
	std::string _reason;
};


/***************************************************************************************/


#endif // _SCHED_RESULTS_H_

