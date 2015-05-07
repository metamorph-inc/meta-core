/*** Included Header Files ***/
#include "templateAction.h"
#include <ctemplate/template.h>


/*** Namespace Declaration ***/
using namespace boost::python;


/************************************************************************************/


//	std::vector< Action* >::iterator execIter = this->_actions.begin();
//	for (; execIter != actions.end(); execIter++) {
//		// Get the BON action and MITemplateAction objects
//		BON::Model bonAction = *execIter;
//		TemplateAction action( bonAction );
//		// Check to see if this action is isolated or uses the default Python environment
//		bool isolate = bonAction->getAttribute( "Isolate" )->getValue();
//		PyThreadState *state = NULL;
//		if ( isolate ) state = Py_NewInterpreter();
//
//		// Exec the action and save the status
//		bool results = action.Execute( paramMap );
//		statusVec.push_back( results );
//
//		// See if we need to clean up an isolated Python environment
//		if ( isolate ) Py_EndInterpreter( state );
//	}
//	// Concatenate both paramMaps
//	return NULL;


bool TemplateAction::ExecutePython( const std::string &script ) {
	// Try executing the script
	try {
		int retVal = PyRun_SimpleString( script.c_str() );
		// Save the executed string
		this->_execStr = script;
	} catch(error_already_set) {
		Console::Out::WriteLine( "*** TemplateAction::ExecutePython -- Exception caught in PyRun_SimpleString ***" );
		return false;
	}
	// Otherwise, all is good
	return true;
}


bool TemplateAction::ExecuteCommandLine( const std::string &script ) {
	return true;
}


void TemplateAction::BuildParameterMap(void) {
	// Get all child constants
	std::set< BON::FCO > constants = this->_action->getChildFCOs( "Constant" );
	std::set< BON::FCO >::iterator constantIter = constants.begin();
	// Look at each one
	for (; constantIter != constants.end(); constantIter++) {
		// Get the constant's name
		std::string name = (*constantIter)->getName();
		// Get the constant's value
		std::string value = (*constantIter)->getAttribute("Value")->getValue();
		// Insert this into the paramMap
		this->_paramMap.insert( std::make_pair( name, value ) );
	}

	// Process all ParamPorts
	std::set< BON::FCO > paramPorts = this->_action->getChildFCOs( "ParamPort" );
	std::set< BON::FCO >::iterator portIter = paramPorts.begin();
	// Look at each one
	for (; portIter != paramPorts.end(); portIter++) {
		// Get the port's name
		std::string name = (*portIter)->getName();

		// Get the in-bound fragment this port is connected to
		std::multiset< BON::ConnectionEnd > connectionSet = (*portIter)->getConnEnds( "FragmentToPort", "src", false );
		std::multiset< BON::ConnectionEnd >::iterator connSetIter = connectionSet.begin();
		// Visit each fragment
		for (; connSetIter != connectionSet.end(); connSetIter++) {
			// Cast the connection end into a model (of a fragment)
			BON::Model fragment( (*connSetIter) );
			// Get the fragment's name (this is the value!)
			std::string value = fragment->getName();
			// Insert this into the paramMap
			this->_paramMap.insert( std::make_pair( name, value ) );
		}

		// Get the out-bound fragment this port is connected to
		connectionSet = (*portIter)->getConnEnds( "PortToFragment", "dst", false );
		connSetIter = connectionSet.begin();
		// Visit each fragment
		for (; connSetIter != connectionSet.end(); connSetIter++) {
			// Cast the connection end into a model (of a fragment)
			BON::Model fragment( (*connSetIter) );
			// Get the fragment's name (this is the value!)
			std::string value = fragment->getName();
			// Insert this into the paramMap
			this->_paramMap.insert( std::make_pair( name, value ) );
		}
	}
	// paramMap should be built at this point
	// ...
}


ActionStatus TemplateAction::Execute( std::map< std::string,std::string > paramMap ) {
	// Set the paramMap
	this->_paramMap = paramMap;
	// Create primary dictionary for the template
	ctemplate::TemplateDictionary dict( this->_action->getName() );
	Console::Out::WriteLine( this->_action->getName().c_str() );

	// Get the template string
	BON::Attribute templateAttribute = this->_action->getAttribute( "TemplateCode" );
	std::string actionTemplate = templateAttribute->getValue();
	// Create the ctemplate using the provided code
	ctemplate::Template* tpl = ctemplate::Template::StringToTemplate( actionTemplate, ctemplate::DO_NOT_STRIP); 

	// Get map of parameters and values
	this->BuildParameterMap();
	// Load the dictionary with the parameters and their values
	std::map< std::string,std::string >::iterator paramMapIter = this->_paramMap.begin();
	for(; paramMapIter != this->_paramMap.end(); paramMapIter++) {
		// Show what you have - if in debug
		if (true) {
			std::string caption = "------" + (*paramMapIter).first + ": " + (*paramMapIter).second;
			Console::Out::WriteLine( caption.c_str() );
		}
		// Insert away!!!
		dict.SetValue( (*paramMapIter).first, (*paramMapIter).second );
	}

	// Generate the output from the template and values...
	std::string output;
	try {
		tpl->Expand(&output, &dict);
		// Show me the money!
		Console::Out::WriteLine( output.c_str() );
		// See what type of template this is
		std::string language = this->_action->getAttribute("Language")->getValue();
		// Is it Python?
		if ( language.compare( "Python" ) == 0 ) {
			// Set the status to success (until we know better)
			this->_status = Success;
			// Execute that statement
			bool retVal = this->ExecutePython( output );
			// See what is returned
			if (!retVal) {
				Console::Out::WriteLine( "*** An error occured running this action ***" );
				this->_status = Failure;
			}
		}
		// Or is it command line?
		else if ( language.compare( "CommandLine" ) == 0 ) {
		}
		// Not sure what to do here
		else {
			Console::Out::WriteLine( "*** Not able to determine template action language ***");
		}
	}
	// Occasionally, errors happen in the ctemplate stuff...
	catch (...) {
		Console::Out::WriteLine( "*** An error has occured in your template.  Please double check it an run this interpreter again. ***" );
		this->_status = Failure;
	}

	// What about outbound parameters?
	//...

	// If we get here, all is good
	return this->_status;
}


/************************************************************************************/

