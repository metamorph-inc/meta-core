/*** Warning Workaround ***/
#pragma warning( disable: 4251 )


// This is the first pass only

/*** Included Header Files ***/
#include "TTVisitor.h"
#include "ConfigKeeper.h"
#include "Uml.h"
#include <boost/Filesystem.hpp>


using namespace Semantics;


/***************************************************************************************/
void TrueTimeSimulinkVisitor::DetermineAnalogIO( const Semantics::Node & node, 
													spec_list_type & input_spec,
													spec_list_type & output_spec )
												 //std::vector< unsigned int > & input_spec, 
												 //std::vector< unsigned int > & output_spec,
												 //std::vector< unsigned int > & input_chan_nums, 
												 //std::vector< unsigned int > & output_chan_nums )
{	
	input_spec.clear();
	output_spec.clear();

	unsigned int inp_idx_counter = 1;
	unsigned int outp_idx_counter = 1;

	// Get all devices -- for the devices, collect all of the IChans and OChans
	SortedIChan_ByChanIndex_Set ichans;
	SortedOChan_ByChanIndex_Set ochans;

	DeviceSet deviceSet = node.integrates();
	DeviceSet::iterator deviceIter = deviceSet.begin();
	for ( ; deviceIter != deviceSet.end(); deviceIter++ ) {
		// See if device is input
		if ( Semantics::InputDevice::meta == deviceIter->type() ) {
			// Cast to an InputDevice
			Semantics::InputDevice inputDevice = Semantics::InputDevice::Cast( *deviceIter );
			// Get the associated IChans
			SortedIChan_ByChanIndex_Set iChanSet = inputDevice.inputChannels_sorted( IChanIndexSorter() );
			ichans.insert( iChanSet.begin(), iChanSet.end() );
		}
		// See if device is output
		else if ( Semantics::OutputDevice::meta == deviceIter->type() ) {
			// Cast to an InputDevice
			Semantics::OutputDevice outputDevice = Semantics::OutputDevice::Cast( *deviceIter );
			// Get the associated OChans
			SortedOChan_ByChanIndex_Set oChanSet = outputDevice.outputChannels_sorted( OChanIndexSorter() );
			ochans.insert( oChanSet.begin(), oChanSet.end() );
		}
	}
			
	// Visit the IChans
	for ( SortedIChan_ByChanIndex_Set::iterator iChanIter = ichans.begin(); iChanIter != ichans.end(); iChanIter++ ) {
		// Get the acquisition
		Semantics::Acquisition acquisition = iChanIter->channelAcquisition();
		// Make sure there is an acquisition
		if ( acquisition != Udm::null ) {
			// Get the message
			Semantics::Msg msg = acquisition.acquiredMsg();
			// Get the set of signals
			SortedSignal_ByMsgIndex_Set signalSet = msg.carries_sorted( SignalMsgIndexSorter() );
			// Go through each signal
			SortedSignal_ByMsgIndex_Set::iterator signalIter = signalSet.begin();
			for ( ; signalIter != signalSet.end(); signalIter++ ) {
				// Get the signal type
				Semantics::SignalType signalType = signalIter->isOfType();
				std::string signalTypeStr = signalType.getStringAttr( Semantics::SignalType::meta_name );
				// Add signal set size to inputs

				ioport_struct inp;
				inp.sigsize = SignalSize( signalTypeStr );
				inp.sigIdx = (_metamode) ? (*iChanIter).chanIndex() : inp_idx_counter++;
				ostringstream chan_str;
				//chan_str << (*iChanIter).name() << "_" << msg.name() << "_" << (*signalIter).name();
				chan_str << (*signalIter).name();
				inp.portname = chan_str.str();

				input_spec.push_back(inp);
			}
		}
	}	// IChan

	// Visit the OChans
	for ( SortedOChan_ByChanIndex_Set::iterator oChanIter = ochans.begin(); oChanIter != ochans.end(); oChanIter++ ) {
		// Get the actuators
		ActuationSet actuationSet = oChanIter->channelActuations();
		ActuationSet::iterator actuationIter = actuationSet.begin();
		for ( ; actuationIter != actuationSet.end(); actuationIter++ ) { 
			// Get the message
			Semantics::Msg msg = actuationIter->actuatingMsg();
			// Get the set of signals
			SortedSignal_ByMsgIndex_Set signalSet = msg.carries_sorted( SignalMsgIndexSorter() );
			// Go through each signal
			unsigned long output_sigs = 0;
			SortedSignal_ByMsgIndex_Set::iterator signalIter = signalSet.begin();
			for ( ; signalIter != signalSet.end(); signalIter++ ) {
				// Get the signal type
				Semantics::SignalType signalType = signalIter->isOfType();
				std::string signalTypeStr = signalType.getStringAttr( Semantics::SignalType::meta_mtypestr );

				ioport_struct outp;
				outp.sigsize = SignalSize( signalTypeStr );
				outp.sigIdx = (_metamode) ? (*oChanIter).chanIndex() : outp_idx_counter++;
				ostringstream chan_str;
				//chan_str << (*oChanIter).name() << "_" << msg.name() << "_" << (*signalIter).name();
				chan_str << (*signalIter).name();
				outp.portname = chan_str.str();

				output_spec.push_back(outp);
			}

		}	// Actuation
	}	// OChan
}

/***************************************************************************************/

TrueTimeSimulinkVisitor::TrueTimeSimulinkVisitor( ) : 
		TrueTimeVisitor(), _metamode( ConfigKeeper::inst().MetaMode() ) {

		_directoryName = ConfigKeeper::inst().GetDirectoryName();
		_baseName = ConfigKeeper::inst().GetOutfileName();
}

void TrueTimeSimulinkVisitor::SetUpDictionary() {

	ClearDictionary();
	GetMainDictionary().SetValue( "SYSTEMNAME", _baseName );
}

void TrueTimeSimulinkVisitor::WriteTemplate( std::string & template_filename, std::string & output_filename ) {
	
	// Initialize the template 
	ctemplate::Template * googleTemplate = ctemplate::Template::GetTemplate( template_filename, ctemplate::DO_NOT_STRIP );
	std::string output;
	// Expand the output into a string
	googleTemplate->Expand( &output, &GetMainDictionary() );
	
	// Only work on directory if not .
	if ( _directoryName.compare( "" ) != 0 ) {
		// Create a path
		boost::filesystem::path outPath( _directoryName );
		// See if the directory exists
		if (! boost::filesystem::exists( _directoryName ) ) {
			// If not, create the directory
			boost::filesystem::create_directory( _directoryName );
		}
	}
	ofstream outputFile( output_filename.c_str() );
	// Write the generated code out to the file and close
	outputFile << output;
	outputFile.close();
}

void TrueTimeSimulinkVisitor::Gen_Deployment_Script( NodeVector & nodeVector, BusSet & buses )
{
	SetUpDictionary();  // Prepare the dictionary for this deployment model

	// Iterate over the nodes and buses, and fill in the blanks

	for ( BusSet::iterator busIter = buses.begin(); busIter != buses.end(); busIter++ )
	{
		AddSectionDictionary( "NETWORK_SECTION" );
		GetTemplateDictionary().SetValue( "BUSNAME", string((*busIter).name()) );
		PopSectionDictionary();
	}

	for ( NodeVector::iterator nodeIter = nodeVector.begin(); nodeIter != nodeVector.end(); nodeIter++ )
	{
		AddSectionDictionary( "TTKERNEL_SECTION" );
		GetTemplateDictionary().SetValue( "KERNELNAME", string((*nodeIter).name()) );
		PopSectionDictionary();
	}

	// Get the template file name
	std::string templateName = ConfigKeeper::inst().GetTemplatePath() + "\\truetime_gen.tpl";
	// Get the name of the output file
	std::string output_filename = _directoryName + "\\" + _baseName + "_gen.m";
	WriteTemplate( templateName, output_filename );

	// Now execute the matlab script to create the model itself
	std::string execStr = "matlab -r \"" + _baseName + "_gen; quit;\"";
	//	system( execStr.c_str() );
}


void TrueTimeSimulinkVisitor::Visit_RootFolder( const Semantics::RootFolder & rootFolder ) {

	/*** Pass 1 - Generate the _gen.m file ***/
	std::cout << "-- Starting First Pass:\n";
	
	// Get the set of deployment children
	DeploymentVector deploymentVector = rootFolder.Deployment_children();
	// Visit each deployment to start off
	DeploymentVector::iterator deploymentIter = deploymentVector.begin();
	for ( ; deploymentIter != deploymentVector.end(); deploymentIter++) {
		deploymentIter->Accept( *this );
	}
}


void TrueTimeSimulinkVisitor::Visit_Deployment( const Semantics::Deployment & deployment ) {
		
        // Get set of TTBUs children - create TT Networks
		TTBusVector ttbusVector = deployment.TTBus_kind_children();
		// Visit them all
		TTBusVector::iterator ttbusIter = ttbusVector.begin();
		for ( ; ttbusIter != ttbusVector.end(); ttbusIter++ ) {
			// Visit the TTBus
			ttbusIter->Accept( *this );
		}

		// Get set of CANBus children - create CAN networks
		CANBusVector canbusVector = deployment.CANBus_kind_children();
		CANBusVector::iterator canbusIter = canbusVector.begin();
		for ( ; canbusIter != canbusVector.end(); canbusIter++ )
		{
			canbusIter->Accept( *this );
		}

		// Get set of Node children - create TT Kernels
		NodeVector nodeVector = deployment.Node_kind_children();
		// Visit them all
		NodeVector::iterator nodeIter = nodeVector.begin();
		for ( ; nodeIter != nodeVector.end(); nodeIter++ ) {
			// Visit the node
			nodeIter->Accept( *this );
		}

		BusSet buses;
		buses.insert( ttbusVector.begin(), ttbusVector.end() );
		buses.insert( canbusVector.begin(), canbusVector.end() );

		Gen_Deployment_Script( nodeVector, buses );
}


void TrueTimeSimulinkVisitor::Visit_Node( const Semantics::Node & node ) {

	SetUpDictionary();

	// Setup the node name
	std::string kernelName = node.name();
	std::string kernelInitName = kernelName + "_init";
	DEBUGOUT( "\tNode: " << kernelName << std::endl );

	// Add a TTKernel for the node
	AddSectionDictionary( "TTKERNEL_SECTION" );
	GetTemplateDictionary().SetValue( "KERNEL_NAME", kernelName );
	GetTemplateDictionary().SetValue( "KERNEL_INITFUNC", kernelInitName );
	GetTemplateDictionary().SetValue( "KERNEL_ARGS", "[1]" );
	spec_list_type input_spec, output_spec;
	DetermineAnalogIO( node, input_spec, output_spec );
	// Set the number of input and output analog values
	GetTemplateDictionary().SetFormattedValue( "KERNEL_IOPORTS", "[%d %d]", sum_sig_sizes(input_spec), sum_sig_sizes(output_spec) );

	// Determine network node number
	std::string netNodeStr = "[";
	DeviceSet deviceSet = node.integrates();
	DeviceSet::iterator deviceIter = deviceSet.begin();
	for ( ; deviceIter != deviceSet.end(); deviceIter++ ) {
		// Make sure we are only looking at commInterfaces
		if ( Semantics::CommInterface::meta == deviceIter->type() ) {
			// Cast to a commInterface
			Semantics::CommInterface commInterface = Semantics::CommInterface::Cast( *deviceIter );
			// Get the associated CommMedium (i.e. TTNetwork)
			Semantics::CommMedium medium = commInterface.commMedium();
			// Get network name
			std::string networkName = medium.name();
			// Find the network in the map
			std::map< std::string, TTNetwork >::iterator netIter = _networkMap.find( networkName );
			TTNetwork network = netIter->second;
			// Make sure the node is not already in this network (i.e. multi BChan to one TTBus)
			if ( network.nodes->find( kernelName ) == network.nodes->end() ) { 
				// Build the netNode String
				int nodeNum = network.nodes->size() + 1;
				// If nodeNum
				netNodeStr += ";" + boost::lexical_cast<std::string>( network.id );
				netNodeStr += " " + boost::lexical_cast<std::string>( nodeNum );
				// Insert the node into the networkMap
				network.nodes->insert( std::make_pair( kernelName, nodeNum ) );
			}
		}
	}
	// Close the network-node string
	netNodeStr += "]";
	// Remove leading ;
	if ( netNodeStr.find_first_of( ";" ) != string::npos ) {
		netNodeStr.erase( netNodeStr.find_first_of(";"), 1 );
	}
	GetTemplateDictionary().SetValue( "KERNEL_NETNODENUM", netNodeStr );
	// Finally, set the schedule scope port on the kernel
	if ( input_spec.size() == 0 || output_spec.size() == 0 )
		GetTemplateDictionary().SetIntValue( "KERNEL_SCOPE_PORT", 1 );
	else
		GetTemplateDictionary().SetIntValue( "KERNEL_SCOPE_PORT", 2 );

	// Build the mux for the A/D if necessary
	if ( input_spec.size() > 0 )
	{
		AddSectionDictionary( "TTKERNEL_AD_SECTION" );
		GetTemplateDictionary().SetIntValue( "KERNEL_AD_PORT", 1 );
		GetTemplateDictionary().SetValue( "KERNEL_AD_SIZE_VECTOR", spec_size_str( input_spec ) );
		for ( unsigned int idx = 0; idx < input_spec.size(); idx++ )
		{
			AddSectionDictionary( "INPUT_PORT_SECTION" );
			GetTemplateDictionary().SetValue( "INPUT_PORT_NAME", input_spec[idx].portname );
			GetTemplateDictionary().SetIntValue( "INPUT_PORT_NUM", input_spec[idx].sigIdx );
			GetTemplateDictionary().SetIntValue( "INPUT_MUX_PORT_NUM", idx + 1 );
			PopSectionDictionary();
		}
		PopSectionDictionary();
	}

	// Build the demux for the D/A if necessary
	if ( output_spec.size() > 0 )
	{
		AddSectionDictionary( "TTKERNEL_DA_SECTION" );
		GetTemplateDictionary().SetIntValue( "KERNEL_DA_PORT", 1 );
		GetTemplateDictionary().SetValue( "KERNEL_DA_SIZE_VECTOR", spec_size_str( output_spec ) );
		for ( unsigned int idx = 0; idx < output_spec.size(); idx++ )
		{
			AddSectionDictionary( "OUTPUT_PORT_SECTION" );
			GetTemplateDictionary().SetValue( "OUTPUT_PORT_NAME", output_spec[idx].portname );
			GetTemplateDictionary().SetIntValue( "OUTPUT_PORT_NUM", output_spec[idx].sigIdx );
			GetTemplateDictionary().SetIntValue( "OUTPUT_MUX_PORT_NUM", idx + 1 );
			PopSectionDictionary();
		}
		PopSectionDictionary();
	}

	// Done with section dictionary
	PopSectionDictionary();

	// Get the template file name
	std::string templateName = ConfigKeeper::inst().GetTemplatePath() + "\\truetime_gen_node.tpl";
	// Get the name of the output file
	std::string output_filename = _directoryName + "\\" + _baseName + "_gen_" + string(node.name()) +  ".m";
	WriteTemplate( templateName, output_filename );
}

void TrueTimeSimulinkVisitor::Visit_TTBus( const Semantics::TTBus & bus ) {

	SetUpDictionary();

	// Get the bus/network name
	std::string networkName = bus.name();
	double datarate = bus.dataratebps();
	int id = _networkMap.size() + 1;
	DEBUGOUT( "\tTTBus(" << id << "): " << networkName << std::endl );
	// Insert network into NetworkMap
	TTNetwork network = { id, new std::map< std::string, int >() };
	_networkMap.insert( std::make_pair( networkName, network ) );
	// Add a TTKernel for the node
	AddSectionDictionary( "NETWORK_SECTION" );
	GetTemplateDictionary().SetValue( "NETWORK_NAME", networkName );
	GetTemplateDictionary().SetValue( "NETWORK_TYPE", "TDMA" );
	GetTemplateDictionary().SetIntValue( "NETWORK_NUMBER", id );
	GetTemplateDictionary().SetFormattedValue( "NETWORK_DATARATE", "%4.0f", datarate );
	GetTemplateDictionary().SetValue( "NETWORK_FRAMESIZE", "512");
	GetTemplateDictionary().SetValue( "NETWORK_LOSSPROB", "0");
	GetTemplateDictionary().SetValue( "NETWORK_SEED", "0");
	// Determine the number of nodes attached to the bus
	int numNodes = 0;
	// First get all of the comm interfaces attached to the bus
	IfaceSet commInterfaceSet = bus.interfaces();
	IfaceSet::iterator commInterfaceIter = commInterfaceSet.begin();
	// Loop through the comm interfaces
	for ( ; commInterfaceIter != commInterfaceSet.end(); commInterfaceIter++ ) {
		// Get the bChan associated with each comm interface
		BChanSet bChanSet = commInterfaceIter->commChannels();
		// Add the number of bChans to the num nodes
		numNodes += bChanSet.size();
	}
	// Now build the schedule string
	std::string netSchedule = "[ ";
	for ( int i=0; i < numNodes; i++ ) {
		// Make sure to add 1 to i since TT network nodes are 1-indexed
		netSchedule = netSchedule + boost::lexical_cast<std::string>(i+1) + std::string(" ");
	}
	netSchedule += "]";
	AddSectionDictionary( "TT_SECTION" );
	GetTemplateDictionary().SetValue( "NETWORK_SCHEDULE", netSchedule );
	PopSectionDictionary();
	GetTemplateDictionary().SetIntValue( "NETWORK_NUMNODES", numNodes );
	// Done with section dictionary
	PopSectionDictionary();


	// Get the template file name
	std::string templateName = ConfigKeeper::inst().GetTemplatePath() + "\\truetime_gen_network.tpl";
	// Get the name of the output file
	std::string output_filename = _directoryName + "\\" + _baseName + "_gen_" + string(bus.name()) +  ".m";
	WriteTemplate( templateName, output_filename );
}

void TrueTimeSimulinkVisitor::Visit_CANBus( const Semantics::CANBus & bus ) {

	SetUpDictionary();

	// Get the bus/network name
	std::string networkName = bus.name();
	double datarate = bus.dataratebps();
	int id = _networkMap.size() + 1;
	DEBUGOUT( "\tCANBus(" << id << "): " << networkName << std::endl );
	// Insert network into NetworkMap
	TTNetwork network = { id, new std::map< std::string, int >() };
	_networkMap.insert( std::make_pair( networkName, network ) );
	// Add a TTKernel for the node
	AddSectionDictionary( "NETWORK_SECTION" );
	GetTemplateDictionary().SetValue( "NETWORK_NAME", networkName );
	GetTemplateDictionary().SetValue( "NETWORK_TYPE", "CSMA/AMP (CAN)" );
	GetTemplateDictionary().SetIntValue( "NETWORK_NUMBER", id );
	GetTemplateDictionary().SetFormattedValue( "NETWORK_DATARATE", "%4.0f", datarate );
	GetTemplateDictionary().SetValue( "NETWORK_FRAMESIZE", "512");
	GetTemplateDictionary().SetValue( "NETWORK_LOSSPROB", "0");
	GetTemplateDictionary().SetValue( "NETWORK_SEED", "0");
	// Determine the number of nodes attached to the bus
	int numNodes = 0;
	// First get all of the comm interfaces attached to the bus
	IfaceSet commInterfaceSet = bus.interfaces();
	IfaceSet::iterator commInterfaceIter = commInterfaceSet.begin();
	// Loop through the comm interfaces
	for ( ; commInterfaceIter != commInterfaceSet.end(); commInterfaceIter++ ) {
		// Get the bChan associated with each comm interface
		BChanSet bChanSet = commInterfaceIter->commChannels();
		// Add the number of bChans to the num nodes
		numNodes += bChanSet.size();
	}
	GetTemplateDictionary().SetIntValue( "NETWORK_NUMNODES", numNodes );
	// Done with section dictionary
	PopSectionDictionary();

	// Get the template file name
	std::string templateName = ConfigKeeper::inst().GetTemplatePath() + "\\truetime_gen_network.tpl";
	// Get the name of the output file
	std::string output_filename = _directoryName + "\\" + _baseName + "_gen_" + string(bus.name()) +  ".m";
	WriteTemplate( templateName, output_filename );
}

/***************************************************************************************/
