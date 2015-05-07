/*** Warning Workaround ***/
#pragma warning( disable: 4251 )


/*** Included Header Files ***/
#include "traceViz.h"


/*** Locally Define Values ***/
#define MAXPERIODS	5000


// Easy global variables
TraceViz* __globalViz = NULL;
unsigned int TVSchedulable::_colorID = 0;


/***************************************************************************************/


std::string GetColorFromID( unsigned int &ID ) {
	// Increment ID mod on 8
	ID = ( ID + 1 ) % 8;
	// Find the color
	switch( ID ) {
		case 0: return "white";
		case 1: return "rgb(100,100,180)";
		case 2: return "green";
		case 3: return "grey";
		case 4: return "yellow";
		case 5: return "silver";
		case 6: return "aqua";
		case 7: return "teal";
		default: return "brown";
	}
}


/***************************************************************************************/


TVSchedulable::TVSchedulable( const TraceType &type, const std::string &name, const bool inKey, const std::string &color ) :
	_type( type ), _name ( name ), _color( GetColorFromID( TVSchedulable::_colorID ) ) {
	// Add to the global key if appropriate
	if ( inKey ) {
		__globalViz->AddKey( this->_name, this->_color );
	}
}


TVSchedulable::~TVSchedulable() {
	// Loop until all instances are deleted
	while ( !this->_instances.empty() ) {
		// Delete the instance
		delete this->_instances.front();
		// Remove it from the list
		this->_instances.pop_front();
	}
}


TVInstance* TVSchedulable::AddInstance( TVTrace* trace, const unsigned int &period, const double &start, const double &duration ) {
	// Create the instance
	TVInstance* instance = new TVInstance;
	instance->schedulable = this;
	instance->trace = trace;
	instance->start = start;
	instance->duration = duration;
	instance->period = period;
	instance->preDep = NULL;
	instance->postDep = NULL;

	// Add the instance to the schedulable
	this->_instances.push_back( instance );
	// Add the instance to the trace
	unsigned int pos = trace->AddInstance( period, instance );
	// Return the instance
	return instance;
}


TVInstance* TVSchedulable::AddInstance( TVTrace* trace, const double &start, const double &duration ) {
	// Create the instance
	TVInstance* instance = new TVInstance;
	instance->schedulable = this;
	instance->trace = trace;
	instance->start = start;
	instance->duration = duration;
	instance->preDep = NULL;
	instance->postDep = NULL;

	// Add the instance to the schedulable
	this->_instances.push_back( instance );
	// Add the instance to the trace
	unsigned int pos = trace->AddInstance( instance );
	// Return the instance
	return instance;
}


void TVSchedulable::AddDependenceArrow( TVInstance* instance, TVTrace* preDependency, TVTrace* postDependency ) {
}


/***************************************************************************************/


unsigned int TVTrace::Render( ctemplate::TemplateDictionary *dictionary, unsigned int &superX,
							  unsigned int &superY, const unsigned int &width, const double &ratio ) {
	// Add a section for this trace
	ctemplate::TemplateDictionary *dict = dictionary->AddSectionDictionary( "TRACEBLOCK" );
	dict->SetValue( "TRACE_NAME", this->_name );
	dict->SetIntValue( "TRACE_HSTROKE_Y", superY + 80 );
	dict->SetIntValue( "TRACE_LABEL_Y", superY + 45 );
	// Determine a period width
	dict->SetIntValue( "TRACE_WIDTH", width );

	// Render all of the periods
	unsigned int periodStart = 150;
	std::vector< std::map< double, TVInstance* > >::iterator periodIter = this->_instances.begin();
	unsigned int i = 0;
	while ( i <= this->_maxPeriod ) {
		// Add the period block
		ctemplate::TemplateDictionary *period = dict->AddSectionDictionary( "PERIODBLOCK" );
		dict->SetIntValue( "PERIOD_VSTROKE_X", periodStart );
		dict->SetIntValue( "PERIOD_VSTROKE_Y", superY + 5 );
		// In this period get the instances
		std::map< double, TVInstance* >::iterator instanceIter = periodIter->begin();
		// Go through all instances
		for ( ; instanceIter != periodIter->end(); instanceIter++ ) {
			// Add an instance block
			ctemplate::TemplateDictionary *msgDict = dict->AddSectionDictionary( "INSTANCEBLOCK" );
			double factor = ratio * 25000.0;
			// Determine the start value and width
			unsigned int start = periodStart + (unsigned int)( instanceIter->second->start * factor );
			unsigned int width = (unsigned int)( instanceIter->second->duration * factor );
			width = std::max( (unsigned int)1, width );
			msgDict->SetIntValue( "INSTANCE_X", start );
			msgDict->SetIntValue( "INSTANCE_Y", superY + 52 );
			msgDict->SetIntValue( "INSTANCE_WIDTH", width );
			msgDict->SetValue( "INSTANCE_COLOR", instanceIter->second->schedulable->Color() );
			msgDict->SetIntValue( "INSTANCE_LABELX", start + 2 );
			msgDict->SetIntValue( "INSTANCE_LABELY", superY + 68 );
			msgDict->SetFormattedValue( "INSTANCE_TIME", "%4.1f", instanceIter->first * 1000 );
		}
		// Move to the next period
		periodStart += 150;
		// Increment i and the periodIter
		i++;
		periodIter++;
	}
	// Increment the superY
	superY += 75;
	// Return the number of periods
	return this->_maxPeriod;
}


TVTrace::TVTrace( const unsigned int &order, const std::string &name ) : 
	_order( order), _name( name ), _currentPeriod( 0 ), _maxPeriod( 0 ), _instances( MAXPERIODS ) {
	// Nothing else for now...
}


unsigned int TVTrace::AddInstance( const unsigned int &period, TVInstance *instance ) {
	// One quick assert
	assert( period >= MAXPERIODS );
	// Set currentPeriod to this update's period
	this->_currentPeriod = period;
	this->_maxPeriod = std::max( this->_maxPeriod, period );
	// Add instance to the correct period
	this->_instances.at( this->_currentPeriod ).insert( std::make_pair( instance->start, instance ) );
	// Return the number of instances
	return this->_instances.at( this->_currentPeriod ).size();
}


unsigned int TVTrace::AddInstance( TVInstance *instance ) {
	// Add instance to the current period
	this->_instances.at( this->_currentPeriod ).insert( std::make_pair( instance->start, instance ) );
	// Return the number of instances
	return this->_instances.at( this->_currentPeriod ).size();
}


/***************************************************************************************/


unsigned int TVSuperblock::Render( ctemplate::TemplateDictionary *dictionary, unsigned int &superX,
								   unsigned int &superY, const double &ratio ) {
	// Need to create a superblock
	ctemplate::TemplateDictionary *dict = dictionary->AddSectionDictionary( "SUPERBLOCK" );
	dict->SetValue( "SUPERBLOCK_NAME", this->_name );
	dict->SetValue( "SUPERBLOCK_COLOR", "rgb(140,140,140)" );
	// See if there should be a hyperperiod lists
	if ( !this->_traces.empty() ) {
		// Create the hyperperiod block
		ctemplate::TemplateDictionary *hyperperiodDict = dict->AddSectionDictionary( "HYPERPERIODBLOCK" );
		hyperperiodDict->SetIntValue( "HYPERPERIOD_Y", superY + 30 );
		hyperperiodDict->SetFormattedValue( "HYPERPERIOD", "%3.1fms", this->_hyperperiodSec * 1000.0 );
	}
	unsigned int taskCount = this->_traces.size();
	unsigned int height = std::max( 75 * taskCount + 10, (unsigned int)25 );

	// Set all of the super block parameters
	dict->SetIntValue( "SUPERBLOCK_Y", superY );
	dict->SetIntValue( "SUPERBLOCK_HEIGHT", height );
	dict->SetIntValue( "SUPERBLOCK_TITLE_Y", superY + 20 );

	// Render all of the child traces
	unsigned int childX = 30;
	unsigned int childY = superY;
	// Determine width of a period ( 20ms period = 500pix if ratio == 1 )
	unsigned int width = (unsigned int)( 25000.0 * ratio * this->_hyperperiodSec );
	unsigned int maxPeriods = 1;
	std::list< TVTrace* >::iterator traceIter = this->_traces.begin();
	for ( ; traceIter != this->_traces.end(); traceIter++ ) {
		// Render the child traces
		maxPeriods = std::max( maxPeriods, (*traceIter)->Render( dict, childX, childY, width, ratio ) );
	}
	// Determine the width of the superblock
	width = width * maxPeriods + 150;
	dict->SetIntValue( "SUPERBLOCK_WIDTH", width );
	// Update the superX and superY values
	superY += height + 10;
	// All is good
	return width;
}


TVSuperblock::TVSuperblock( const unsigned int &order, const std::string &name, const double &hyperperiodSec ) :
	_order( order ), _name( name ), _hyperperiodSec( hyperperiodSec ) {
	// Nothing else to do for now
}


TVSuperblock::~TVSuperblock() {
	// Loop until all traces are deleted
	while ( !this->_traces.empty() ) {
		// Delete the trace
		delete this->_traces.front();
		// Remove it from the list
		this->_traces.pop_front();
	}
}


TVTrace* TVSuperblock::AddTrace( const std::string &name ) {
	// Create the new trace
	TVTrace* trace = new TVTrace( this->_traces.size(), name );
	// Add the trace to the registry
	this->_traces.push_back( trace );
	// Return the trace
	return trace;
}


/***************************************************************************************/


void TraceViz::AddKey( const std::string &name, const std::string &color ) {
	// For now just insert into the key
	this->_key.insert( std::make_pair( name, color ) );
}


unsigned int TraceViz::RenderKey( ctemplate::TemplateDictionary* dictionary,
	unsigned int &superX, unsigned int &superY ) {
	// First, add a keyblock
	ctemplate::TemplateDictionary *dict = dictionary->AddSectionDictionary( "KEYBLOCK" );
	// Set the keyblock parameter
	dict->SetIntValue( "KEY_Y", superY + 15 );
	// Advance the superY beyond the header
	superY += 20;
	// Loop through all of the key objects
	std::map< std::string, std::string >::iterator mapIter = this->_key.begin();
	for ( ; mapIter != this->_key.end(); mapIter++ ) {
		// Add a keyblock
		ctemplate::TemplateDictionary *keyDict = dict->AddSectionDictionary( "KEY_INST" );
		// Set the parameters
		keyDict->SetIntValue( "KEY_INSTY", superY );
		keyDict->SetValue( "KEY_COLOR", mapIter->second );
		keyDict->SetIntValue( "KEY_LABELY", superY + 17 );
		keyDict->SetValue( "KEY_NAME", mapIter->first );
		// Advance the superY
		superY += 30;
	}
	// Return the number of key items
	return this->_key.size();
}


TraceViz::TraceViz( const std::string &traceName, const std::string &filename, const std::string &templateFile  ) :
	_name( traceName ), _filename( filename ), _templateFile( templateFile ), _maxHyperperiod( 0.0 ) {
	// Set the global TraceViz variable
	__globalViz = this;
}


TraceViz::~TraceViz() {
	// Loop until all superblocks are deleted
	while ( !this->_superblocks.empty() ) {
		// Delete the superblock
		delete this->_superblocks.front();
		// Remove it from the list
		this->_superblocks.pop_front();
	}
	// Set the globalViz to NULL
	__globalViz = NULL;
}


TVSuperblock* TraceViz::AddSuperblock( const std::string &name, const double &hyperperiodSec ) {
	// Create the superblock
	TVSuperblock* block = new TVSuperblock( this->_superblocks.size(), name, hyperperiodSec );
	// Add the block int the registry
	this->_superblocks.push_back( block );
	// See if this is a new max hyperperiod
	this->_maxHyperperiod = std::max( this->_maxHyperperiod, hyperperiodSec );
	// Return the block
	return block;
}


bool TraceViz::Render(void) {
	// Initialize the template 
	ctemplate::Template *googleTemplate = ctemplate::Template::GetTemplate( this->_templateFile, ctemplate::DO_NOT_STRIP );
	std::string output;
	// Initialize the main ctemplate dictionary
	ctemplate::TemplateDictionary *dictionary = new ctemplate::TemplateDictionary( "MAIN" );
	dictionary->SetValue( "TRACEVIZ_NAME", this->_name );
	dictionary->SetValue( "TRACEVIZ_FONT", "Tahoma" );

	// Render superblocks into the dictionary
	unsigned int subX = 10, subY = 70;
	unsigned int maxWidth = 650;
	double ratio = 0.020 / this->_maxHyperperiod;
	std::list< TVSuperblock* >::iterator blockIter = this->_superblocks.begin();
	for ( ; blockIter != this->_superblocks.end(); blockIter++) {
		// Render the superblock
		maxWidth = std::max( maxWidth, (*blockIter)->Render( dictionary, subX, subY, ratio ) );
	}
	// Set the overall image width
	dictionary->SetIntValue( "TRACEVIZ_WIDTH", maxWidth + 20 );

	// Now render the key if there are items in it
	if ( !this->_key.empty() )
		this->RenderKey( dictionary, subX, subY );

	// Expand the output into a string
	googleTemplate->Expand( &output, dictionary );
	// Get the name of the output file
	std::ofstream outputFile( this->_filename.c_str() );
	// Write the generated code out to the file and close
	outputFile << output;
	outputFile.close();
	return true;
}


/***************************************************************************************/

