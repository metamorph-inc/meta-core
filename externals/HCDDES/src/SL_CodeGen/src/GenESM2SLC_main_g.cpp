/* GenESM2SLC_main.cpp generated on Fri Mar 23 14:49:21 2007
 */

#include "SLSF.h"

#include "GenESM2SLC.h"
#include "InputFileRegistry.h"
#include "SFCUdmEngine.hpp"

#include "CPrinter.hpp"
#include "JPrinter.hpp"
#include "utils.h"
#include "transroot.hpp"
#include "ConfigKeeper.hpp"
#include "CodeGenerator.hpp"
#include "mfile2SFC.hpp"
#include <Uml.h>

#define NOMINMAX

#include <iostream>
#include <algorithm>
#include <boost/format.hpp>
#include <boost/tokenizer.hpp>
#include <boost/filesystem.hpp>

//UDM_USE_DOM // COMMENTED OUT BECAUSE OF INCLUDE OF "SFCUdmEngine.hpp"

#ifdef _WIN32
UDM_USE_MGA
#endif

typedef std::map< int, SLSF::ModelsFolder > ModelsFolderMap;
typedef std::set< SLSF::DesignFolder > DesignFolderSet;
typedef std::set< SLSF::ModelsFolder > ModelsFolderSet;
typedef std::set< SLSF::Dataflow > DataflowSet;

ModelsFolderMap getModelsFolderMap( Udm::DataNetwork &dataNetwork ) {
	ModelsFolderMap modelsFolderMap;

	SLSF::RootFolder rootFolder = SLSF::RootFolder::Cast( dataNetwork.GetRootObject() );
	DesignFolderSet designFolderSet = rootFolder.DesignFolder_children();
	if ( designFolderSet.empty() ) return modelsFolderMap;
	for ( DesignFolderSet::iterator designFolder = designFolderSet.begin(); designFolder != designFolderSet.end(); designFolder++ )
	{
		ModelsFolderSet modelsFolderSet = (*designFolder).ModelsFolder_children();
		for( ModelsFolderSet::iterator mfsItr = modelsFolderSet.begin() ; mfsItr != modelsFolderSet.end() ; ++mfsItr ) {
			modelsFolderMap.insert(  std::make_pair( mfsItr->uniqueId(), *mfsItr )  );
		}
	}
	return modelsFolderMap;
}

SLSF::Dataflow getDataflow( SLSF::ModelsFolder &modelsFolder ) {
	DataflowSet dataflowSet = modelsFolder.Dataflow_children();
	return dataflowSet.empty() ? SLSF::Dataflow() : *dataflowSet.begin();
}


int main( int argc, char* argv[] ) {
	// Try to process the command line
	if ( !ConfigKeeper::getSingleton().processCommandLineArguments( argc, argv ) ) {
		std::cout << ConfigKeeper::getSingleton().getDescription();
		return 0;
	}
	// See if help was requested
	if (  ConfigKeeper::getSingleton().getCount( "help" )  ) {
		std::cout << ConfigKeeper::getSingleton().getDescription();
		return 0;
	}
	// Make sure an input was specified
	if (  !ConfigKeeper::getSingleton().getCount( "slsf-file" )  ) {
		std::cerr << "Input file must be specified." << std::endl;
		std::cerr << ConfigKeeper::getSingleton().getDescription();
		return 1;
	}
	// Set the generation language
	bool generateC = true;
	if (  ConfigKeeper::getSingleton().getCount( "java" )  ) generateC = false;
	if (  ConfigKeeper::getSingleton().getCount( "c" )  ) generateC = true;
	bool annotations = ConfigKeeper::getSingleton().getCount( "annotations" );

	// Set the output directory
	std::string outputDirectory;
	if (  ConfigKeeper::getSingleton().getCount( "output-directory" )  ) {
		outputDirectory = ConfigKeeper::getSingleton().getStringValue( "output-directory" );
	}

	DirectoryVector directoryVector;
	if (  ConfigKeeper::getSingleton().getCount( "libdir" )  ) {
		directoryVector = ConfigKeeper::getSingleton().getStringVector( "libdir" );
	}

	addMFileDirectories( directoryVector );

	// Determine the input file name
	std::string inputFilename = ConfigKeeper::getSingleton().getStringValue( "slsf-file" );
	std::string projectName = inputFilename.substr(  0, inputFilename.rfind( "." )  );
	{
		std::string::size_type lastSlashPos = projectName.rfind( "/" );
		std::string::size_type lastBackslashPos = projectName.rfind( "\\" );

		if ( lastSlashPos == std::string::npos ) lastSlashPos = lastBackslashPos;
		if ( lastBackslashPos == std::string::npos ) lastBackslashPos = lastSlashPos;

		std::string::size_type delimPos = std::min( lastSlashPos, lastBackslashPos );
		if ( delimPos != std::string::npos ) projectName = projectName.substr( delimPos + 1 );
	}

	std::string arg1 = SLSF_PARADIGM_NAME " = \"" + inputFilename + "\" ";
//  std::string arg2 = "SFC = \"" + std::string(argv[2]) + string(".xml") + "\" !\"SFC.xsd\" ";

	InputFileRegistry ifr;
	ifr.registerFile( arg1 );
	try {

		// Open SLSF r
		Udm::SmartDataNetwork sdnSLSF_fb0( SLSF::diagram );
		sdnSLSF_fb0.OpenExisting(
		 ifr.getFileName( SLSF_PARADIGM_NAME ),
		 UseXSD()(  ifr.getFileName( SLSF_PARADIGM_NAME )  ) ? ifr.getXsdName( SLSF_PARADIGM_NAME ) : SLSF_PARADIGM_NAME,
		 Udm::CHANGES_LOST_DEFAULT
		);
		Udm::StaticDataNetworkSpecifier sdns_SLSF_fb1(  ifr.getFileName( SLSF_PARADIGM_NAME ), &sdnSLSF_fb0  );

		ModelsFolderMap modelsFolderMap = getModelsFolderMap( sdnSLSF_fb0 );
		for( ModelsFolderMap::iterator mfmItr = modelsFolderMap.begin() ; mfmItr != modelsFolderMap.end() ; ++mfmItr ) {

			int modelsFolderID = mfmItr->first;
			std::string modelsFolderName = mfmItr->second.name();
			if ( outputDirectory.empty() ) outputDirectory = modelsFolderName;

			std::vector< Udm::StaticDataNetworkSpecifier > dnsvec;
			dnsvec.push_back( sdns_SLSF_fb1 );

			if (  !boost::filesystem::create_directories( outputDirectory )  ) {
				if (  !boost::filesystem::is_directory( outputDirectory )  ) {
					std::cerr << "ERROR:  Could not create \"" + outputDirectory + "\" directory." << std::endl;
					continue;
				}
			}

			std::string sfcFilename = outputDirectory + "/" + modelsFolderName + "_SFC.xml";
			std::string arg2= "SFC = \"" + sfcFilename + "\" !\"SFC.xsd\" ";
			ifr.registerFile( arg2 );

			// Open SFC w
			UdmDom::DomDataNetwork &sdnSFC_fb3 = SFCUdmEngine::get_singleton().getDomDataNetwork();
			sdnSFC_fb3.CreateNew(
			 ifr.getFileName( "SFC" ),
			 UseXSD()(  ifr.getFileName( "SFC" )  ) ? ifr.getXsdName( "SFC" ) : "SFC",
			 SFC::Project::meta,
			 Udm::CHANGES_LOST_DEFAULT
			);

			Udm::StaticDataNetworkSpecifier sdns_SFC_fb4(  ifr.getFileName( "SFC" ), &sdnSFC_fb3  );
			dnsvec.push_back( sdns_SFC_fb4 );

			// Create the project
			Udm::StaticUdmProject prj( dnsvec, ESM2SLC::diagram );
			Udm::DataNetwork& SLSF_ref_fb2= prj.GetDataNetwork(  ifr.getFileName( SLSF_PARADIGM_NAME )  );
			Udm::DataNetwork& sFC_ref_fb5= prj.GetDataNetwork(  ifr.getFileName( "SFC" )  );

			Packets_t projects_1;
			Packets_t rootDataflows_3;

			// get objects from DNs
			ModelsFolderMap modelsFolderMap2 = getModelsFolderMap( SLSF_ref_fb2 );
			ModelsFolderMap::iterator mfmItr2 = modelsFolderMap2.find( modelsFolderID );
			if ( mfmItr2 == modelsFolderMap2.end() ) {
				std::cerr << "ERROR:  Could not find ModelsFolder with id \"" << modelsFolderID << "\"" << std::endl;
			} else {
				SLSF::Dataflow dataflow = getDataflow( mfmItr2->second );

				rootDataflows_3.push_back( dataflow );

				// Get access to temporary root object(s).
				// transformation
				SFC::Project rootSFC_257f = SFC::Project::Cast( sFC_ref_fb5.GetRootObject() );
				rootSFC_257f.name() = projectName;

				projects_1.push_back( rootSFC_257f );

				for( Packets_t::const_iterator it = rootDataflows_3.begin() ; it != rootDataflows_3.end() ; ++it ) {
					Packets_t oneRootState;
					oneRootState.push_back( *it );
					TransClass tL_0;
		//			tL_0( projects_1, oneRootState);
					tL_0( oneRootState, projects_1 );

		//			rootSFC_257f.numInstance() = ni>0 ? ni : 1;
					boost::filesystem::path path = boost::filesystem::current_path();
					boost::filesystem::current_path( outputDirectory );
					if ( generateC ) CPrinter::print( rootSFC_257f );
					else             JPrinter::printProject( rootSFC_257f, annotations );
					boost::filesystem::current_path( path );

					// Print the S-Function wrapper
		//			printSFuncWrapper(  SFUtils::convertToCPlusPlusName( stateName ), rootSFC_257f  );
					// Delete program 

				}
			}

			// Close the project
			prj.Close();
			// Close SFC w
			sdnSFC_fb3.CloseWithUpdate();

			if ( SFUtils::getPortTypeError() ) boost::filesystem::remove( sfcFilename );
		}
		modelsFolderMap.clear();

		// Close SLSF r
		sdnSLSF_fb0.CloseNoUpdate();

	} catch( udm_exception &e ) {
		cout << e.what() << endl;
		return 2;
	} catch( std::exception &e ) {
		std::cout << e.what() << std::endl;
		return 3;
	} catch( ... ) {
		std::cout << "Unknown exception (sorry)" << std::endl;
		return 4;
	}

	if ( CodeGenerator::getSingleton().getError() ) return 5;

	return 0;
}

