#ifndef _SCHED_RESULTS_H_
#define _SCHED_RESULTS_H_


/*** Included Header Files ***/
#include "Uml.h"
#include "ESMoL\generated\ESMoL.h"
#include "ESMoL_Abstract\generated\Semantics.h"
#include <map>
#include <vector>
#include <sstream>
#include <iostream>
#include <fstream>


/*** Locally Defined Types ***/
typedef enum {FIND_HYPERPERIOD, FIND_ELEMENTS} ParserState;
// Typedefs for ESMoL Paradigm
typedef std::set< ESMoL::DesignFolder > DesignFolderSet;
typedef std::set< ESMoL::System > SystemSet;
typedef std::vector< ESMoL::TTExecInfo > TTExecInfoVector;
typedef std::set< ESMoL::ComponentRef > ComponentRefSet;
typedef std::set< ESMoL::ComponentAssignment > ComponentAssignmentSet;
typedef std::set< ESMoL::ExecutionAssignment > ExecutionAssignmentSet;
typedef std::set< ESMoL::NodeRef > NodeRefSet;
typedef std::set< ESMoL::IODevice > IODeviceSet;
// Typedefs for ESMoL_Abstract Paradigm
typedef std::set< Semantics::Deployment > AbstractDeploymentSet;
typedef std::vector< Semantics::CommMedium > AbstractBusVector;
typedef std::set< Semantics::Node > AbstractNodeSet;


/***************************************************************************************/


class SchedResults {
private:
	bool										_debug;
	std::string									_hyperperiodStr;
	double										_hyperperiodSec;
	double										_unitsFactor;
	std::map<std::string, std::string>			_elements;
	std::map<std::string, std::vector< double > > _values;
	std::vector<std::string>					_harewareElements;

	// Private Methods
	SchedResults();													//!< Deny access to default constructor
	bool ParseResults( const std::string &resultFile );				//!< Parse the results file
	bool PopulateEsmol( const std::string &esmolModel );			//!< Populate components and messages in ESMoL model
	bool PopulateAbstract( const std::string &abstractModel );		//!< Populate components, messages, and busses in ESMoL_Abstract model
public:
	// Constructors and Destructors	
	SchedResults( const std::string &resultFile, const std::string &esmolModel,
		const std::string &abstractModel, const bool &debug=false );
	~SchedResults() { }
};


class SchedResultsException {
public:
	SchedResultsException( std::string &reason ) : _reason(reason) { }
	std::string									_reason;
};


/***************************************************************************************/


#endif // _SCHED_RESULTS_H_

