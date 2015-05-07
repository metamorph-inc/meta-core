/*** Included Header Files ***/
#include "motivate_process.h"
#include "templateAction.h"


/************************************************************************************/


std::map< std::string,std::string > Process::BuildParamMap( void ) {
	std::map< std::string,std::string > paramMap;
	// Get the root directory for the process
	std::string rootDirectory = this->_process->getAttribute( "RootDirectory" )->getValue();
	// Add it to the param map if there is a value
	if (rootDirectory.compare("") != 0) {
		paramMap.insert( std::make_pair( "ROOT_DIRECTORY", rootDirectory ) );
	}
	// Get all child constants
	std::set< BON::FCO > constants = this->_process->getChildFCOs("Constant");
	std::set< BON::FCO >::iterator constantIter = constants.begin();
	// Look at each one
	for (; constantIter != constants.end(); constantIter++) {
		// Get the constant's name
		std::string name = (*constantIter)->getName();
		// Get the constant's value
		std::string value = (*constantIter)->getAttribute("Value")->getValue();
		// Insert this into the paramMap
		paramMap.insert( std::make_pair( name, value ) );
	}
	// Return the newly build paramMap
	return paramMap;
}


bool Process::SetRootDirectory( const std::string &rootDirectory ) {
	// If root directory is present
	if ( rootDirectory.compare("") != 0 ) {
		// Make sure the root directory exists
		struct stat St;
		if( stat( rootDirectory.c_str(), &St ) != 0 ) {
			// Path does not exist
			std::string msg = "*** Invalid Root Directory Path: " + rootDirectory + " ***";
			Console::Out::WriteLine( msg.c_str() );
			return false;
		} else {
			// Build the command string
			std::string execStr = "os.chdir(\"" + rootDirectory + "\")\n";
//				Console::Out::WriteLine( execStr.c_str() );
			// Execute the command string
			try {
				// Change to the root directory
				PyRun_SimpleString( execStr.c_str() );
			} catch (error_already_set) {
				// PyErr_Print();
				Console::Out::WriteLine( "*** Motivate::Initialize -- Exception in os.chdir from PyRun_SimpleString ***" );
				return false;
			}
		}
	}
	// All is good
	return true;
}


/************************************************************************************/


Process::Process( const BON::Model &process, const std::vector< BON::Model > &actions ) :
	_process( process ), _actions(), _currentAction() {
	// Go through each BON::Model action and create a Motivate::Action
	std::vector< BON::Model >::const_iterator actionIter = actions.begin();
	for ( ; actionIter != actions.end(); actionIter++ ) {
		// Create a Motivate::Action
		TemplateAction* action = new TemplateAction( this, *actionIter );
		// Insert the action into the actions vector
		this->_actions.push_back( action );
	}
	// Set the current action
	this->_currentAction = this->_actions.begin();
}


Process::~Process() {
	// Need to delete all child actions
	while ( !this->_actions.empty() ) {
		// Delete the last action
		delete this->_actions.back();
		// Remove it from the vector
		this->_actions.pop_back();
	}
}


Action* Process::ExecuteStep( std::map< std::string,std::string > paramMap ) {
	int size = this->_actions.size();
	// Make sure we have not executed all actions
	if ( this->_currentAction == this->_actions.end() ) return NULL;
	// Get the local parameters
	std::map< std::string,std::string > selfParamMap = this->BuildParamMap();
	// Set the root directory
	this->SetRootDirectory( this->_process->getAttribute( "RootDirectory" )->getValue() );
	// Get the current action
	Action *action = *this->_currentAction;
	// Execute the current action
	if (action) action->Execute( paramMap );
	// Get the next action
	this->_currentAction++;
	// Return the action
	return action;
}


Action* Process::FindAction( const std::string &name ) {
	std::vector< Action* >::iterator actionIter = this->_actions.begin();
	// Go through each action and check the name
	for (; actionIter != this->_actions.end(); actionIter++) {
		if ( name.compare( (*actionIter)->Name() ) == 0 ) return *actionIter;
	}
	return NULL;
}


/************************************************************************************/


