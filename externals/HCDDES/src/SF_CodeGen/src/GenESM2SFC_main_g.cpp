#include "GenESM2SFC_main_g.hpp"
#include <set>

#ifdef _WIN32
//UDM_USE_DOM
UDM_USE_MGA
#endif


typedef std::map< int, SLSF::ModelsFolder > ModelsFolderMap;
typedef std::set< SFC::Program > ProgramSet;
typedef std::set< SLSF::DesignFolder > DesignFolderSet;
typedef std::set< SLSF::ModelsFolder > ModelsFolderSet;
typedef std::set< SLSF::Stateflow > StateflowSet;

ModelsFolderMap getModelsFolderMap( Udm::DataNetwork &dataNetwork ) {
	ModelsFolderMap modelsFolderMap;

	SLSF::RootFolder rootFolder = SLSF::RootFolder::Cast( dataNetwork.GetRootObject() );
	DesignFolderSet designFolderSet = rootFolder.DesignFolder_children();
	if ( designFolderSet.empty() ) return modelsFolderMap;
	SLSF::DesignFolder designFolder = *designFolderSet.begin();

	ModelsFolderSet modelsFolderSet = designFolder.ModelsFolder_children();
	for( ModelsFolderSet::iterator mfsItr = modelsFolderSet.begin() ; mfsItr != modelsFolderSet.end() ; ++mfsItr ) {
		modelsFolderMap.insert(  std::make_pair( mfsItr->uniqueId(), *mfsItr )  );
	}
	return modelsFolderMap;
}

SLSF::Stateflow getStateflow( SLSF::ModelsFolder &modelsFolder ) {
	StateflowSet stateflowSet = modelsFolder.Stateflow_children();
	return stateflowSet.empty() ? SLSF::Stateflow() : *stateflowSet.begin();
}

const std::string SFCG_VERSION = "1.1";

void printVer() {
	std::cerr << "Stateflow Code Generator v" << SFCG_VERSION << " \n(c) 2003-2005 Vanderbilt University\nInstitute for Software Integrated Systems\n\n";
}

int main( int argc, char* argv[]) {
	printVer();
	// Try to parse the command line
	if ( !ConfigKeeper::getSingleton().processCommandLineArguments( argc, argv ) ) {
		return 0;
	}
	// See about the other command line options
	if (  ConfigKeeper::getSingleton().getCount( "help" )  ) {
		std::cerr << "Usage: SF_CodeGen.exe ( <SLSF_model.mga> | <SLSF_model.xml> ) [options]" << std::endl;
		std::cerr << ConfigKeeper::getSingleton().getDescription();
		return 0;
	}
	if (  !ConfigKeeper::getSingleton().getCount( "inputFile" )  ) {
		std::cerr << "Usage: SF_CodeGen.exe ( <SLSF_model.mga> | <SLSF_model.xml> ) [options]" << std::endl;
		std::cerr << ConfigKeeper::getSingleton().getDescription();
		return 1;
	}

	std::cout << "Stateflow Code Generator is running..." << std::endl;

	bool generateC = true;
	if (  ConfigKeeper::getSingleton().getCount( "java" )  ) generateC = false;
	if (  ConfigKeeper::getSingleton().getCount( "c" )  ) generateC = true;
	bool annotations = ConfigKeeper::getSingleton().getCount( "annotations" );

	bool generateStatusFunction = !generateC && ConfigKeeper::getSingleton().getCount( "status" );

	std::string outputDirectory;
	if (  ConfigKeeper::getSingleton().getCount( "directory" )  ) {
		outputDirectory = ConfigKeeper::getSingleton().getStringValue( "directory" );
	}

	DirectoryVector directoryVector;
	if (  ConfigKeeper::getSingleton().getCount( "libdir" )  ) {
		directoryVector = ConfigKeeper::getSingleton().getStringVector( "libdir" );
	}

	addMFileDirectories( directoryVector );

	std::string inputFilename = ConfigKeeper::getSingleton().getStringValue( "inputFile" );
	std::string projectName = inputFilename.substr(  0, inputFilename.rfind( "." )  );

	std::string arg1 = std::string( SLSF_PARADIGM_NAME " = \"" ) + inputFilename + "\" ";
//  std::string arg2 = "SFC = \"" + std::string(argv[2]) + string(".xml") + "\" !\"SFC.xsd\" ";

	InputFileRegistry ifr;
	ifr.registerFile( arg1 );
	try {

		// Open SLSF r
		Udm::SmartDataNetwork sdnSLSF_2575( ESMoL::diagram );
		sdnSLSF_2575.OpenExisting(argv[1], "ESMoL", Udm::CHANGES_LOST_DEFAULT);
		Udm::StaticDataNetworkSpecifier sdns_SLSF_2576(  ifr.getFileName( SLSF_PARADIGM_NAME ), &sdnSLSF_2575  );

		ModelsFolderMap modelsFolderMap = getModelsFolderMap( sdnSLSF_2575 );
		for( ModelsFolderMap::iterator mfmItr = modelsFolderMap.begin() ; mfmItr != modelsFolderMap.end() ; ++mfmItr ) {

			int modelsFolderID = mfmItr->first;
			std::string modelsFolderName = mfmItr->second.name();
			if ( outputDirectory.empty() ) outputDirectory = modelsFolderName;

			std::vector< Udm::StaticDataNetworkSpecifier> dnsvec;
			dnsvec.push_back( sdns_SLSF_2576 );

			if (
			 !boost::filesystem::create_directory( outputDirectory ) &&
			 !boost::filesystem::is_directory( outputDirectory )
			) {
				std::cerr << "ERROR:  Could not create \"" + outputDirectory + "\" directory." << std::endl;
				continue;
			}

			std::string arg2= "SFC = \"" + std::string( "_sftemp.xml" ) + "\" !\"SFC.xsd\" ";
			ifr.registerFile( arg2 );

			// Open SFC w
			UdmDom::DomDataNetwork &sdnSFC_2578 = SFCUdmEngine::get_singleton().getDomDataNetwork();
			sdnSFC_2578.CreateNew(
			 ifr.getFileName( "SFC" ),
			 UseXSD()(  ifr.getFileName( "SFC" )  ) ? ifr.getXsdName( "SFC" ) : "SFC",
			 SFC::Project::meta,
			 Udm::CHANGES_LOST_DEFAULT
			);

			Udm::StaticDataNetworkSpecifier sdns_SFC_2579(  ifr.getFileName( "SFC" ), &sdnSFC_2578  );
			dnsvec.push_back( sdns_SFC_2579);

			// Create the project
			Udm::StaticUdmProject prj( dnsvec, ESM2SFC::diagram);
			Udm::DataNetwork& SLSF_ref_2577= prj.GetDataNetwork(  ifr.getFileName( SLSF_PARADIGM_NAME )  );
			Udm::DataNetwork& sFC_ref_257a= prj.GetDataNetwork(  ifr.getFileName( "SFC" )  );

			Packets_t projects_1;
			Packets_t rootStates_3;

			// get objects from DNs
			ModelsFolderMap modelsFolderMap2 = getModelsFolderMap( SLSF_ref_2577 );
			ModelsFolderMap::iterator mfmItr2 = modelsFolderMap2.find( modelsFolderID );
			if ( mfmItr2 == modelsFolderMap2.end() ) {
				std::cerr << "ERROR:  Could not find ModelsFolder with id \"" << modelsFolderID << "\"" << std::endl;
				continue;
			}

			SLSF::Stateflow stateflow = getStateflow( mfmItr2->second );
			if ( stateflow == Udm::null ) continue;

			std::set< SLSF::State> states_257e = stateflow.State_kind_children();
			for(
				std::set< SLSF::State >::const_iterator itstates_2718 = states_257e.begin() ;
				itstates_2718 != states_257e.end() ;
				++itstates_2718
			) {
				const SLSF::State& currState_2719 = *itstates_2718;
				std::set< SLSF::State > states_271a = currState_2719.State_kind_children();
				rootStates_3.insert( rootStates_3.end(), states_271a.begin(), states_271a.end() );
			}

			SFC::Project rootSFC_257f = SFC::Project::Cast( sFC_ref_257a.GetRootObject() );
			rootSFC_257f.name() = projectName;

			projects_1.push_back( rootSFC_257f );

			// transformation

			// for each rootSFC ...
			for( Packets_t::const_iterator it = rootStates_3.begin() ; it != rootStates_3.end() ; ++it ) 
			{
				Packets_t oneRootState;
				oneRootState.push_back( *it );
				TransClass tL_0;
	//			tL_0( projects_1, oneRootState );
				tL_0( oneRootState, projects_1 );

				const SLSF::State& state = SLSF::State::Cast( *it);
				const std::string& stateName= state.name();
				const set<SLSF::ConnectorRef>& crefs = state.referedbyConnectorRef();
				int ni=0;
				for( set< SLSF::ConnectorRef >::const_iterator cri=crefs.begin() ; cri!=crefs.end() ; cri++ ) 
				{
					SLSF::ConnectorRef cref = *cri;
					const set<SLSF::ConnectorRef>& crefins = cref.Instances();
					ni += crefins.size();
				}

				boost::filesystem::path path = boost::filesystem::current_path();
				boost::filesystem::current_path( outputDirectory );

				ProgramSet programSet = rootSFC_257f.Program_kind_children();
				for( ProgramSet::iterator pgsItr = programSet.begin() ; pgsItr != programSet.end() ; (void)++pgsItr )
				{
					pgsItr->filename() = SFUtils::convertToCPlusPlusName( stateName );
//						SFuncWrapperPrint::printWrapper(  SFUtils::convertToCPlusPlusName( stateName ), *pgsItr  );
				}

				if ( generateC ) CPrinter::print( rootSFC_257f );
				else             JPrinter::printProject( rootSFC_257f, annotations, generateStatusFunction );

				boost::filesystem::current_path( path );

				sdnSFC_2578.SaveAs( outputDirectory + "/" + modelsFolderName + "_" + stateName + "_SFC.xml" );

				// Delete program
				std::set< SFC::Statement> statements = rootSFC_257f.Statement_kind_children();
				for(
					std::set< SFC::Statement >::iterator itS = statements.begin() ;
					itS != statements.end() ;
					++itS
				) 
					{
					SFC::Statement ob = *itS;
					ob.DeleteObject();
				}

#ifdef _DEBUG
				std::set< SFC::Statement> statements2= rootSFC_257f.Statement_kind_children();
				for( std::set< SFC::Statement>::const_iterator itS2= statements2.begin(); itS2!= statements2.end(); ++itS2) {
					std::cout << itS2->statementIndex() << endl;
				}
#endif
				// initialize program
				rootSFC_257f.statementCount()= 0;
				rootSFC_257f.statementIndex() = 0;
			}

			// Close the project
	  		prj.Close();
			// Close SFC w
			sdnSFC_2578.CloseWithUpdate();

	//		boost::filesystem::remove( "_sftemp.xml" );
			boost::filesystem::remove( "_sftemp.mem" );
		}

		// Close SLSF r
		sdnSLSF_2575.CloseNoUpdate();
	}
	catch( udm_exception &e)
	{
		cout << e.what() << endl;
	}

	std::cout << "Done." << std::endl;

	return 0;
}

