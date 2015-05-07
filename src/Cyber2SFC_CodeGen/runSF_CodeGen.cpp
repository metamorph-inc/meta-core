#include "runSF_CodeGen.h"

#include "SLSF.h"

#include "SFCUdmEngine.hpp"
#include "GenCyber2SFC.h"
//#include "InputFileRegistry.h"
#include "SFCprint.h"
#include "CPrinter.hpp"
#include "JPrinter.hpp"
#include "SFuncWrapperPrint.h"
#include "SfUtils.h"
//#include "transroot.hpp"
#include "ConfigKeeper.hpp"
#include "mfile2SFC.hpp"
#include <iostream>
#include <boost/filesystem.hpp>
#include <Uml.h>
#include <UmlExt.h>
#include "CGLog.h"

#include "UdmConsole.h"

#ifdef _WIN32
//UDM_USE_DOM
UDM_USE_MGA
#endif

typedef std::set< SFC::Program > ProgramSet;
typedef std::set<CyberComposition::Simulink::State> StateSet;
typedef std::set<CyberComposition::Simulink::SFState > SFStateSet;
typedef std::set<CyberComposition::Simulink::Transition > TransitionSet;
typedef std::set<CyberComposition::Simulink::SFTransition > SFTransitionSet;
typedef std::set<CyberComposition::Simulink::Data > DataSet;
typedef std::set<CyberComposition::Simulink::SFData > SFDataSet;
typedef std::set<CyberComposition::Simulink::Event > EventSet;
typedef std::set<CyberComposition::Simulink::SFEvent > SFEventSet;

// When this function is used as an exported .dll function by some other .dll 
// then we need to initialize the metas according to the following func.
void __declspec(dllexport) runCyberSF_CodeGen_dllimported(const CyberComposition::SimulinkWrapper &slmodel, std::string cyber_file, 
						Uml::Diagram &cyber_meta_diagram, std::string logFile, std::string directory)
{
	UdmDom::str_xsd_storage::RemoveXsd("CyberComposition.xsd");

	LINKS::Initialize();
	CyberComposition::Initialize(cyber_meta_diagram);
	
	// Collect Datanetwork to be passed to runCyPhy2SL_codeGen func
	Udm::DataNetwork * dn = slmodel.__impl()->__getdn();
	// Generate Code
	openLogFile(logFile, true);
	printLog("******Start model transformation(SF_CodeGen) for SimulinkWrapper model: "+(std::string)slmodel.name()+"******");
	runCyberSF_CodeGen(slmodel, cyber_file, dn, directory);
	printLog("******End model transformation(SF_CodeGen)******");
	closeLogFile();
}

void getStates(const CyberComposition::Simulink::Block &block, StateSet &states)
{
	set<CyberComposition::Simulink::State> bstates = block.State_kind_children();
	states.insert(bstates.begin(), bstates.end());
	if(Uml::IsDerivedFrom(block.type(),CyberComposition::Simulink::Subsystem::meta))
	{
		const CyberComposition::Simulink::Subsystem &subsys =CyberComposition::Simulink::Subsystem::Cast(block);
		set<CyberComposition::Simulink::Block> blocks = subsys.Block_kind_children();
		for(set<CyberComposition::Simulink::Block>::iterator bit=blocks.begin();bit!=blocks.end();++bit)
		{
			CyberComposition::Simulink::Block currBlock = *bit;
			getStates(currBlock, states);
		}
	}		
}

void getSFStates(const CyberComposition::Simulink::Block &block, SFStateSet &states)
{
	set<CyberComposition::Simulink::SFState> bstates = block.SFState_kind_children();
	states.insert(bstates.begin(), bstates.end());
	if(Uml::IsDerivedFrom(block.type(),CyberComposition::Simulink::Subsystem::meta))
	{
		const CyberComposition::Simulink::Subsystem &subsys =CyberComposition::Simulink::Subsystem::Cast(block);
		set<CyberComposition::Simulink::Block> blocks = subsys.Block_kind_children();
		for(set<CyberComposition::Simulink::Block>::iterator bit=blocks.begin();bit!=blocks.end();++bit)
		{
			CyberComposition::Simulink::Block currBlock = *bit;
			getSFStates(currBlock, states);
		}
	}		
}

void runCyberSF_CodeGen(const CyberComposition::SimulinkWrapper &slmodel, std::string cyber_file, Udm::DataNetwork* sdn_cyber, std::string directory)
{
	if (CyberComposition::SimulinkWrapper::meta == Udm::null)
		throw udm_exception("CyberComposition diagram is not initialized");
	set<CyberComposition::Simulink::Subsystem> syss = slmodel.Simulink_Subsystem_kind_children();
	for(set<CyberComposition::Simulink::Subsystem>::iterator it=syss.begin();it!=syss.end();++it)
	{
		CyberComposition::Simulink::Subsystem sys = *it;
		runCyberSF_CodeGen(sys, cyber_file, sdn_cyber, directory);
	}
}

void runCyberSF_CodeGen(const CyberComposition::Simulink::Subsystem &sys, std::string cyber_file, Udm::DataNetwork* sdn_cyber, std::string directory)
{
	std::string outputDirectory = directory;
	std::string projectName;
	if(!cyber_file.empty())
	{
		std::string inputFilename = cyber_file;
		projectName = inputFilename.substr(  0, inputFilename.rfind( "." )  );
	}
	else
	{
		Udm::Object sfparent = sys.parent();
		projectName = UdmUtil::ExtractName(sfparent);
	}
	if(outputDirectory.empty())
	{
		outputDirectory = boost::filesystem::current_path().string();		
	}
	try 
	{
		std::vector< Udm::StaticDataNetworkSpecifier> dnsvec;
		//Udm::StaticDataNetworkSpecifier sdns_cyphy(cyphy_file, sdn_cyphy);
		Udm::StaticDataNetworkSpecifier sdns_cyphy("", sdn_cyber);
		dnsvec.push_back( sdns_cyphy);

		// create SFC w
		UdmDom::DomDataNetwork &sdn_sfc = SFCUdmEngine::get_singleton().getDomDataNetwork();
		sdn_sfc.CreateNew("_sftemp.xml", "SFC.xsd", SFC::Project::meta );

		Udm::StaticDataNetworkSpecifier sdns_SFC("_sftemp.xml", &sdn_sfc);
		dnsvec.push_back( sdns_SFC);

		Udm::StaticUdmProject prj( dnsvec, Cyber2SFC::diagram);

		StateSet states;
		getStates(sys, states);

		SFStateSet sfstates;
		getSFStates( sys, sfstates );

		std::string sysName = UdmUtil::ExtractName(sys.GetParent());

		Packets_t projects;	
		SFC::Project rootSFC = SFC::Project::Cast(sdn_sfc.GetRootObject() );
		rootSFC.name() = projectName;
		projects.push_back(rootSFC);

		Packets_t rootFolder;
		CyberComposition::RootFolder cyber_rf = CyberComposition::RootFolder::Cast(sdn_cyber->GetRootObject());
		rootFolder.push_back(cyber_rf);

		// transformation
		// for each rootSFC ...
		for( StateSet::const_iterator it = states.begin() ; it!= states.end() ; ++it ) 
		{
			Packets_t oneRootState;
			oneRootState.push_back( *it );
			TL_0 tL_0;
			tL_0(rootFolder, oneRootState, projects);

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

			boost::filesystem::current_path( outputDirectory );

			ProgramSet programSet = rootSFC.Program_kind_children();
			for( ProgramSet::iterator pgsItr = programSet.begin() ; pgsItr != programSet.end() ; (void)++pgsItr )
			{
				pgsItr->filename() = SFUtils::convertToCPlusPlusName( stateName );
			}

			CPrinter::print( rootSFC);
				
			boost::filesystem::current_path(outputDirectory);

			sdn_sfc.SaveAs( outputDirectory + "/" + sysName + "_" + stateName + "_SFC.xml" );

			// Delete program
			std::set< SFC::Statement> statements = rootSFC.Statement_kind_children();
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
			std::set< SFC::Statement> statements2= rootSFC.Statement_kind_children();
			for( std::set< SFC::Statement>::const_iterator itS2= statements2.begin(); itS2!= statements2.end(); ++itS2) {
				std::cout << itS2->statementIndex() << endl;
			}
#endif
			// initialize program
			rootSFC.statementCount()= 0;
			rootSFC.statementIndex() = 0;
		}


		// Duplicate the above for SFStates
		// for each rootSFC ...
		for( SFStateSet::const_iterator it = sfstates.begin() ; it!= sfstates.end() ; ++it ) 
		{
			Packets_t oneRootState;
			oneRootState.push_back( *it );
			T2L_3761 tl2;
			tl2(rootFolder, oneRootState, projects);

			const SLSF::SFState& state = SLSF::SFState::Cast( *it);
			const std::string& stateName= state.name();
			const set<SLSF::SFConnectorRef>& crefs = state.referedbyConnectorRef();
			int ni=0;
			for( set< SLSF::SFConnectorRef >::const_iterator cri=crefs.begin() ; cri!=crefs.end() ; cri++ ) 
			{
				SLSF::SFConnectorRef cref = *cri;
				const set<SLSF::SFConnectorRef>& crefins = cref.Instances();
				ni += crefins.size();
			}

	//		boost::filesystem::path path = boost::filesystem::current_path();
			boost::filesystem::current_path( outputDirectory );
			//boost::filesystem::current_path(sysName);

			ProgramSet programSet = rootSFC.Program_kind_children();
			for( ProgramSet::iterator pgsItr = programSet.begin() ; pgsItr != programSet.end() ; (void)++pgsItr )
			{
				pgsItr->filename() = SFUtils::convertToCPlusPlusName( stateName );
			}

			CPrinter::print( rootSFC);
				
	//		boost::filesystem::current_path( path );
			boost::filesystem::current_path(outputDirectory);

			sdn_sfc.SaveAs( outputDirectory + "/" + sysName + "_" + stateName + "_SFC.xml" );

			// Delete program
			std::set< SFC::Statement> statements = rootSFC.Statement_kind_children();
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
			std::set< SFC::Statement> statements2= rootSFC.Statement_kind_children();
			for( std::set< SFC::Statement>::const_iterator itS2= statements2.begin(); itS2!= statements2.end(); ++itS2) {
				std::cout << itS2->statementIndex() << endl;
			}
#endif
			// initialize program
			rootSFC.statementCount()= 0;
			rootSFC.statementIndex() = 0;
		}

			// Close the project
	  		prj.Close();
			// Close SFC w
			sdn_sfc.CloseWithUpdate();

			boost::filesystem::remove( "_sftemp.xml" );
			boost::filesystem::remove( "_sftemp.mem" );
	}
	catch( udm_exception &e)
	{
		cout << e.what() << endl;
		printLog( e.what() );
		return;
	}
	catch( boost::filesystem::filesystem_error & fe )
	{
		printLog( "Filesystem error -- please check to see whether existing processes" );
		printLog( " (like Visual Studio) are using files in the generated files folder." );
		printLog( fe.what() );
	}
	catch(exception &e)
	{
		printLog(e.what());
		return;
	}
	catch(...)
	{
		printLog("Exception throw.");
		return;
	}	
	std::cout << "Done." << std::endl;
}