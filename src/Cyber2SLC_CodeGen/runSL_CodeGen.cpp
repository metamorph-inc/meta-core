#include "runSL_CodeGen.h"

#include "SLSF.h"

#include "GenCyber2SLC.h"
#include "InputFileRegistry.h"
#include "SFCUdmEngine.hpp"

#include "CPrinter.hpp"
#include "JPrinter.hpp"
#include "utils.h"
//#include "transroot.hpp"
#include "ConfigKeeper.hpp"
#include "CodeGenerator.hpp"
#include "mfile2SFC.hpp"
#include <Uml.h>

//ADDED by AKSHAY AGRAWAL
#include "UdmConsole.h"

#include <boost/filesystem.hpp>

#define NOMINMAX

#include <iostream>
#include <algorithm>
#include <boost/format.hpp>
#include <boost/tokenizer.hpp>
#include <boost/filesystem.hpp>

#include "FMUCodeGen.h"
#include "CGLog.h"
#include "utils.h"
//UDM_USE_DOM // COMMENTED OUT BECAUSE OF INCLUDE OF "SFCUdmEngine.hpp"

#ifdef _WIN32
UDM_USE_MGA
#endif

CyberComposition::SimulinkWrapper SFUtils::rootSFModel;

//typedef std::set< SLSF::Dataflow > DataflowSet;

// When this function is used as an exported .dll function by some other .dll 
// then we need to initialize the metas according to the following func.
bool __declspec(dllexport) runCyberSL_CodeGen_dllimported(const CyberComposition::SimulinkWrapper &slmodel, std::string cyber_file, Uml::Diagram cyber_meta_diagram, std::string directory)
{
	UdmDom::str_xsd_storage::RemoveXsd("CyberComposition.xsd");

	LINKS::Initialize();
	CyberComposition::Initialize(cyber_meta_diagram);
	
	// Collect Datanetwork to be passed to runCyPhy2SL_codeGen func
	Udm::DataNetwork * dn = slmodel.__impl()->__getdn();
	// Generate Code
	return runCyberSL_CodeGen(slmodel, cyber_file, dn, directory);
}

bool runCyberSL_CodeGen(const CyberComposition::SimulinkWrapper &slmodel, std::string cyber_file, Udm::DataNetwork* sdn_cyber, std::string directory)
{
	//UdmDom::str_xsd_storage::RemoveXsd("CyberComposition.xsd");

	//LINKS::Initialize();
	//CyberComposition::Initialize(cyber_meta_diagram);
	//
	//// Collect Datanetwork to be passed to runCyPhy2SL_codeGen func
	//Udm::DataNetwork * sdn_cyber = slmodel.__impl()->__getdn();
	//// Generate Code

	bool ret = true;

	// Set the output directory
	SFUtils::rootSFModel = slmodel;
	std::string outputDirectory = directory;
	std::string projectName;
	if(!cyber_file.empty())
	{
		std::string inputFilename = cyber_file;
		projectName = inputFilename.substr(  0, inputFilename.rfind( "." )  );
		{
			std::string::size_type lastSlashPos = projectName.rfind( "/" );
			std::string::size_type lastBackslashPos = projectName.rfind( "\\" );

			if ( lastSlashPos == std::string::npos ) lastSlashPos = lastBackslashPos;
			if ( lastBackslashPos == std::string::npos ) lastBackslashPos = lastSlashPos;

			std::string::size_type delimPos = std::min( lastSlashPos, lastBackslashPos );
			if ( delimPos != std::string::npos ) projectName = projectName.substr( delimPos + 1 );
		}
	}
	else
	{
		//Udm::Object sfparent = slmodel.parent();
		//projectName = UdmUtil::ExtractName(sfparent);
		projectName = slmodel.name();
	}
	//try {

		// Open SLSF r
		std::vector< Udm::StaticDataNetworkSpecifier> dnsvec;
		Udm::StaticDataNetworkSpecifier sdns_cyphy("", sdn_cyber);
		dnsvec.push_back( sdns_cyphy);

		std::string slName = slmodel.name();
		std::string slcFilename = outputDirectory + "/" + slName + "_SLC.xml";

		UdmDom::DomDataNetwork &sdn_sfc = SFCUdmEngine::get_singleton().getDomDataNetwork();
		sdn_sfc.CreateNew(slcFilename, "SFC.xsd", SFC::Project::meta );

		Udm::StaticDataNetworkSpecifier sdns_SFC(slcFilename, &sdn_sfc);
		dnsvec.push_back( sdns_SFC);

		Udm::StaticUdmProject prj( dnsvec, Cyber2SLC::diagram);
		Packets_t projects;
		Packets_t rootDataflows;

		SFC::Project rootSFC = SFC::Project::Cast(sdn_sfc.GetRootObject() );
		rootSFC.name() = projectName;

		projects.push_back( rootSFC);
		rootDataflows.push_back(slmodel);

		set<CyberComposition::Simulink::Subsystem> top_level_subs = slmodel.Simulink_Subsystem_kind_children();
		for ( set<CyberComposition::Simulink::Subsystem>::iterator subIter = top_level_subs.begin(); subIter != top_level_subs.end(); subIter++ )
		{
			// Check for cycles in the graph before getting underway with the translation - throw an exception if a cycle is found
			SFUtils::TopLevelTopologicalSort( *subIter );
		}

		printLog("******Start model transformation(SL_CodeGen) for SignalFlow model: "+(std::string)slmodel.name()+"******");
		TL_0 tL_0;
		tL_0(rootDataflows, projects);
		
		if(SFUtils::getErrorReported())
		{
			ret = false;
			printLog("******SL_CodeGen fails******");
		}
		else
		{
			printLog("******End model transformation(SL_CodeGen)******");
			boost::filesystem::current_path( outputDirectory );
			try{
				CPrinter::print( rootSFC );
			}
			catch(udm_exception &e)
			{
				printLog(e.what());
				prj.Close();
				sdn_sfc.CloseWithUpdate();
				return false;
			}
			boost::filesystem::current_path(outputDirectory);
	
			//get the top subsystem name
			set<CyberComposition::Simulink::Subsystem> subs = slmodel.Simulink_Subsystem_kind_children();
			if(!subs.empty())
			{
				CyberComposition::Simulink::Subsystem topSub = *(subs.begin());
				FMUCodeGen fmu(rootSFC, (std::string)topSub.name(), outputDirectory);
				fmu.gen();
			}
		}
		//get the top class name and args, generate fmi.xml and fmu.c files

		prj.Close();
		// Close SFC w
		sdn_sfc.CloseWithUpdate();

		if ( SFUtils::getPortTypeError() ) 
		{
			if(!boost::filesystem::remove( slcFilename ))
			{
				std::string err = "boost::filesystem::remove(\""+slcFilename+"\") fails";
				throw std::exception(err.c_str());
			}
		}

		return ret;
}

void logGeneratedFiles(std::string workDir)
{
	if(workDir.empty())
		workDir = boost::filesystem::current_path().string();

	list<std::string> fmufiles;	
	if( boost::filesystem::exists(workDir) )
	{
		printLog("generated source files: ");
		boost::filesystem::directory_iterator end;
		for(boost::filesystem::directory_iterator iter(workDir) ; iter != end ; ++iter )
		{
			if ( !boost::filesystem::is_directory( *iter ) )
			{
				std::string fname = iter->path().filename().string();
				size_t pos1 = fname.find("_fmu.c");
				size_t pos2 = fname.find("_Description.xml");
				if(pos1!=std::string::npos || pos2!=std::string::npos)
					fmufiles.push_back(fname);
				else
					printLog("\t\t"+fname);
			}
		}
	}
	if(!fmufiles.empty())
	{
		printLog("generated fmu files: ");
		while(!fmufiles.empty())
		{
			printLog("\t\t"+*(fmufiles.begin()));
			fmufiles.pop_front();
		}
	}
}

ComponentCodeGen::ComponentCodeGen(CyberComposition::SimulinkWrapper &slmodel, const std::string &outputDirectory, const std::string &comname)
	:_slmodel(slmodel), _directory(outputDirectory), errorReported(false)
{
	if(comname.empty())
		_slname = _slmodel.name();
}

void ComponentCodeGen::runCodeGen(const std::string &logFile)
{
	boost::filesystem::current_path(_directory);
	boost::filesystem::create_directory(_slname);
	printLog("create folder: "+_slname);
	std::string curr_workDir = _directory + "\\"+ _slname;
	boost::filesystem::current_path(curr_workDir);

	try { // Handle exceptions from filesystem::remove

		//remove existing files
		if( boost::filesystem::exists( curr_workDir ) )
		{
			boost::filesystem::directory_iterator end;
			for(boost::filesystem::directory_iterator iter(curr_workDir) ; iter != end ; ++iter )
			{
				if ( !boost::filesystem::is_directory( *iter ) )
				{
					boost::filesystem::path fpath = iter->path();
					boost::filesystem::remove(fpath);
				}
			}
		}
	}
	catch ( boost::filesystem::filesystem_error & e )
	{
		string errmsg = "\nThe file system reported an error.  Please check to see whether\nfiles in ";
		errmsg += "the generated code directory are open in other programs\nsuch as Visual Studio.\n\n";
		errmsg += string(e.what() );

		throw std::exception( errmsg.c_str() );
	}

//	for(auto it=sfmodels.begin();it!=sfmodels.end();it++)
	{
		//CyPhyML::SignalFlowModel sfmodel = *it;
		//std::string sfname = sfmodel.name();

		//printLog("******Start model transformation(SF_CodeGen) for SignalFlow model: "+sfname+"******");
		closeLogFile();
		runCyberSF_CodeGen_dllimported(_slmodel, "", CyberComposition::meta, logFile, curr_workDir);
		
		openLogFile(logFile, true);
		if(!runCyberSL_CodeGen(_slmodel,"", _slmodel.__impl()->__getdn(), curr_workDir))
			setError();

		logGeneratedFiles(curr_workDir);

		try {

			if( boost::filesystem::exists( curr_workDir ) )
			{
				boost::filesystem::directory_iterator end;
				for(boost::filesystem::directory_iterator iter(curr_workDir) ; iter != end ; ++iter )
				{
					if ( !boost::filesystem::is_directory( *iter ) )
					{
						boost::filesystem::path fpath = iter->path();
						boost::filesystem::path fpath_ext = fpath.extension();
						std::string filepath_str = fpath_ext.string();
						if(filepath_str == ".h")
							_hfiles.insert(fpath.string());
						else if(filepath_str == ".c")
						{
							std::string fname = fpath.filename().string();
							size_t slpos = fname.find("_fmu.c");
							if(slpos==std::string::npos)
								_cfiles.insert(fpath.string());
						}
					}
				}
			}
		}
		catch ( boost::filesystem::filesystem_error & e )
		{
			string errmsg = "\nThe file system reported an error.  Please check to see whether\nfiles in ";
			errmsg += "the generated code directory are open in other programs\nsuch as Visual Studio.\n\n";
			errmsg += string(e.what() );

			throw std::exception( errmsg.c_str() );
		}
	}
}

set<std::string> ComponentCodeGen::getGenerated_H_Files()
{
	return _hfiles;
}

set<std::string> ComponentCodeGen::getGenerated_C_Files()
{
	return _cfiles;
}