#ifndef __MOTIVATE_H__
#define __MOTIVATE_H__


/*** Included Header Files ***/
#include "BON2Component.h"
#include <list>
#include <Console.h>
#include <boost/python.hpp>


/*** Namespace Declarations ***/
using namespace GMEConsole;
using namespace boost::python;


/************************************************************************************/


namespace Motivate {


/*** Class Predefinitions ***/
class Action;
class Process;


// Static Helper Functions
std::map< std::string,BON::Model > MapProcesses( const BON::Project &project );
std::map< std::string,BON::Model > MapActions( const BON::Model &process );
std::vector< std::string > OrderProcesses( const std::map< std::string,BON::Model > &procMap );
std::vector< std::string > OrderActions( const std::map< std::string,BON::Model > &actionMap );
std::string RootDirectory( const BON::Model &action );
void RecurseActions( const BON::Model &action, std::vector< BON::Model > &actionVec );


class Conductor {
protected:
	// Some python stuff
	static bool								_init;
	static PyObject							*_mainMod, *_mainNamespace;
	static Process							*_currentProcess;

	// Hidden Methods
	static std::map< std::string,std::string > BuildParamMap( void );
	static bool Initialize( void );
	static bool Shutdown( void );
	// Deny access to Default constructor
	Conductor();
public:
	// Primary process execution method
	static bool Prepare( const BON::Model &process, const std::vector< BON::Model > &actions );
	static Action* ExecuteStep(void);
	static void Reset(void);
	static Process* CurrentProcess(void)	{ return Conductor::_currentProcess; }
};


}	// End of Motivate namespace


/*** Now we can declare using Motivate ***/
using namespace Motivate;


/************************************************************************************/


#endif //__MOTIVATE_H__

