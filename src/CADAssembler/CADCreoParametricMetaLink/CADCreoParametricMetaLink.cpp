#include "stdafx.h"
#include <CommonUtilities.h>
#include <CreoPlugins/CreoPlugins.h>
#include <ISISConstants.h>

#include <ProNotify.h>
#include <AssembleUtils.h>
#include "MiscellaneousFunctions.h"

#include "CADEnvironmentSettings.h"
#include "WindowsFunctions.h"
#include <AssemblyEditingViaLink.h>
#include <MetaLinkHandler.h>
#include <Test_MetaLink.h>
#include <ISISVersionNumber.h>
#include "CADFactoryAbstract.h"
#include "CADFactoryCreo.h"
#include <AssembleUtils.h>

#include <boost/filesystem.hpp>

#include <log4cpp/Category.hh>
#include <log4cpp/PropertyConfigurator.hh>
#include <log4cpp/BasicConfigurator.hh>
#include <log4cpp/PatternLayout.hh>
#include <log4cpp/OstreamAppender.hh>
#include <log4cpp/FileAppender.hh>
#include "CommonDefinitions.h"

#include "EventLoopMonitor.h"

boost::atomic<bool> producerDone (false);
boost::atomic<bool> terminateProcess (false);

ProError ProTermAction( ProeTerminationStatus term_type )
{
    terminateProcess=true;
    if (isis::GlobalModelData::Instance.metalink_handler_ptr != 0)
        isis::GlobalModelData::Instance.metalink_handler_ptr->interrupt();
    return PRO_TK_NO_ERROR;
}

/**
* The callback to be used when a component or datum of a component is selected.
*	ProSelectionExtHighlightAct
*/
void informHightlightSelectionEvent( ProSelection selection, ProBoolean highlight, ProAppData ext_data) {
    log4cpp::Category& logcat = log4cpp::Category::getInstance(LOGCAT_LOGFILEONLY);
    logcat.errorStream() << "caught event : "
                         << selection << " highlight-toggle: " << highlight << " external-data: " << ext_data;
}

/**
implements ProAsmcompActivatePreAction()
*/
ProError metaAsmcompActivatePreAction(ProAsmcomppath *active_path, ProSolid active_model) {
    log4cpp::Category& logcat = log4cpp::Category::getInstance(LOGCAT_LOGFILEONLY);
    logcat.errorStream() << "caught event : "
                         << " active-path: " << active_path << " acive-model: " << active_model;
    return PRO_TK_NO_ERROR;
}

/**
implements ProMdlDisplayPostAction()
*/
ProError metaMdlDisplayPostAction(void) {
    const isis::MetaLinkMode majorMode = isis::GlobalModelData::Instance.mode;
    const std::string targetId = isis::GlobalModelData::Instance.designId;
    isis::GlobalModelData::Instance.metalink_handler_ptr->send_LocateSelectedRequest(majorMode, targetId);
    return PRO_TK_NO_ERROR;
}

ProError metaParameterModifyAction(ProParameter *param, ProParamvalue *old_value)
{
    return PRO_TK_NO_ERROR;
}

void SetupLogging(const std::string logfilename, const log4cpp::Priority::Value level)
{
    isis::isis_DeleteFile(logfilename);

    log4cpp::PatternLayout *layout1 = new log4cpp::PatternLayout();
    layout1->setConversionPattern(log4cpp::PatternLayout::SIMPLE_CONVERSION_PATTERN);
    log4cpp::PatternLayout *layout2 = new log4cpp::PatternLayout();
    layout2->setConversionPattern(log4cpp::PatternLayout::SIMPLE_CONVERSION_PATTERN);
    log4cpp::PatternLayout *layout3 = new log4cpp::PatternLayout();
    layout3->setConversionPattern(log4cpp::PatternLayout::SIMPLE_CONVERSION_PATTERN);

    // Appender for logging to the console
    log4cpp::Appender *consoleappender2 = new log4cpp::OstreamAppender("console", &std::cout);
    consoleappender2->setLayout(layout1);

    // Appender for logging to the log file
    log4cpp::Appender *fileappender1 = new log4cpp::FileAppender("logfile", logfilename);
    fileappender1->setLayout(layout2);

    // Appender for logging to the log file
    log4cpp::Appender *fileappender2 = new log4cpp::FileAppender("logfile", logfilename);
    fileappender2->setLayout(layout3);

    // Category for logging only to the log file
    log4cpp::Category& logcat_fileonly = log4cpp::Category::getInstance(LOGCAT_LOGFILEONLY);
    logcat_fileonly.setPriority(level);
    logcat_fileonly.addAppender(fileappender1);

    // Category for logging both to the log file and to the console (in Meta-Link it's only to the file)
    log4cpp::Category& logcat_consoleandfile = log4cpp::Category::getInstance(LOGCAT_CONSOLEANDLOGFILE);
    logcat_consoleandfile.setPriority(level);
    logcat_consoleandfile.addAppender(fileappender2);

    // Category for logging only to the console
    log4cpp::Category& logcat_consoleonly = log4cpp::Category::getInstance(LOGCAT_CONSOLEONLY);
    logcat_consoleonly.setPriority(level);
    logcat_consoleonly.addAppender(consoleappender2);
}

int main( int argc, char *argv[] )
{
    const std::string ASSEMBLE_PTC_VERSION = ISIS_PRODUCT_VERSION_WITH_v_AND_DOTS;

    ::boost::filesystem::path original_directory = ::boost::filesystem::current_path();

    int ExitCode = 0;

    std::string			creoStartCommand;
    std::string			proeIsisExtensionsDir;

    std::string			templateFile_PathAndFileName;
    std::stringstream	exceptionErrorStringStream;

    bool Pro_E_Running = false;
    bool Template_Copied = false;

    bool Logging_Set_Up = false;

    isis::ProgramInputArguments  programInputArguments;
    ::boost::filesystem::path    workingDir;

    try
    {
        bool regenerationSucceeded_ForAllAssemblies = true;

        // Parse Input Arguments
        isis::ParseInputArguments(argc, argv, programInputArguments);
        //std::cout << std::endl << programInputArguments;

        // Set log4cpp to use user-specified logfilename
        SetupLogging(programInputArguments.logFileName, programInputArguments.logVerbosity);

        log4cpp::Category& logcat_fileonly = log4cpp::Category::getInstance(LOGCAT_LOGFILEONLY);

        Logging_Set_Up = true;


        isis::ThrowExecption_If_InvalidInputArguments(argc, argv, programInputArguments);

        // Must get the complete path to the working directory.  This is necessary because when
        // isis_ProDirectoryChange is called to change to a STEP directory, workingDir must be fully
        // defined so that isis_ProDirectoryChange can be called to return to the original directory.
        workingDir = isis::SetupWorkingDirectory( programInputArguments.workingDirectory );

        //cADPartsLibDir = programInputArguments.auxiliaryCADDirectory;
        //xmlInputFile_PathAndFileName = programInputArguments.inputXmlFileName;

        // Log CADCreoParametricCreateAssembly version information
        std::string programName_Version_TimeStamp;
        programName_Version_TimeStamp = "CADCreoParametricCreateAssembly " + ASSEMBLE_PTC_VERSION + "      ";

        ///////////////////
        // Add Time Stamp
        ///////////////////

        programName_Version_TimeStamp += isis_CADCommon::GetDayMonthTimeYear();
        logcat_fileonly.infoStream() << programName_Version_TimeStamp;

        logcat_fileonly.infoStream() << "Notes: " << ::log4cpp::eol
                                     << "   1. The \"Component Instance ID\"s in this file equate to ComponentInstanceIDs in CyPhy."  << ::log4cpp::eol
                                     << "   2. To map \"Component Instance ID\"s in this file to AVM-IDs, see .\\log\\CyPhy2CAD.log." << ::log4cpp::eol;

        time_t time_start; /* calendar time */
        time_start=time(NULL); /* get current cal time */

        // Log input line and parameters
        std::ostringstream inputLine;
        for ( int i = 0; i < argc; ++i) inputLine << argv[i] << std::string(" ");
        logcat_fileonly.infoStream()
                << "************** Begin Input Line *****************" << ::log4cpp::eol
                << inputLine.str() << ::log4cpp::eol
                << "************** End Input Line *****************";

        logcat_fileonly.infoStream()
                << "************** Begin Input Arguments *****************" << ::log4cpp::eol
                << programInputArguments << ::log4cpp::eol
                << "************** End Input Arguments *******************";

        if ( workingDir.generic_string().size() >= PRO_PATH_SIZE )  // PRO_PATH_SIZE   260
        {
            std::stringstream errorString;
            errorString << "WORKING_DIR string too long.  Maximum allowed number of characters: "  << PRO_PATH_SIZE - 1 << " WORKING_DIR string: " << workingDir;
            throw isis::application_exception(errorString);
        }
        logcat_fileonly.infoStream()
                << ::log4cpp::eol << "************** Begin Directory Settings *****************"
                << ::log4cpp::eol << "workingDir:                    "	<< workingDir
                << ::log4cpp::eol << "inputXmlFileName:              "	<< programInputArguments.inputXmlFileName
                << ::log4cpp::eol << "logFileName:                   "	<< programInputArguments.logFileName
                << ::log4cpp::eol << "*************** End Directory Settings ******************";

        bool graphicsModeOn = false;
        bool creoExceptInputFromThisProgramAndCreoUI = false;

        if (programInputArguments.graphicsModeOn) graphicsModeOn = true;
        if (programInputArguments.synchronizeWithCyPhy) creoExceptInputFromThisProgramAndCreoUI = true;

        isis::SetCreoEnvirVariable_RetrieveSystemSettings(	graphicsModeOn,
                creoExceptInputFromThisProgramAndCreoUI,
                creoStartCommand,
                proeIsisExtensionsDir,
                templateFile_PathAndFileName );

        std::map<std::string, isis::CADComponentData> CADComponentData_map;
        isis::CADAssemblies CADComponentAssemblies;

        logcat_fileonly.infoStream() << "running in MetaLink live mode";

        ////////////////////////////////////////////////
        // Write config.pro
        ///////////////////////////////////////////////

        ofstream config_Pro;
        ::boost::filesystem::path configPro_PathAndFileName = workingDir / "config.pro";
        config_Pro.open( configPro_PathAndFileName.string() );
        config_Pro << "override_store_back yes\n";
        config_Pro << "enable_sociallink NO\n";

        ::boost::filesystem::path searchMetaFileName = "./search_META.pro";
        if ( ::boost::filesystem::exists( searchMetaFileName )) {
            config_Pro << "search_path_file " <<  searchMetaFileName.string();
        }

        ::boost::filesystem::path cadPartsLibDir = programInputArguments.auxiliaryCADDirectory;
        if ( ::boost::filesystem::exists( cadPartsLibDir )) {
            config_Pro << std::endl << "search_path " << "\"" << cadPartsLibDir.string() << "\"" << std::endl;
        }

        config_Pro << std::endl << "pro_material_dir " << isis::CreoMaterialMTLFilesDir_Path();

        // protk.dat configuration information
        const char* proeIsisExt = std::getenv("PROE_ISIS_EXTENSIONS");
        const char* hudatExt = std::getenv("HUDAT_INSTALLDIR");
        if (proeIsisExt == NULL) {
            std::string msg = "the PROE_ISIS_EXTENSIONS environment parameter is not set";
            throw isis::application_exception(msg);
        }
        ::boost::filesystem::path proeIsisExtPath(proeIsisExt);
        if (! ::boost::filesystem::is_directory(proeIsisExtPath)) {
            std::stringstream msg;
            msg << "the PROE_ISIS_EXTENSIONS environment parameter set but no such directory exists: "
                << proeIsisExtPath.generic_string();
            throw isis::application_exception(msg);
        }

        ::boost::filesystem::path protkPath;
        if (hudatExt != NULL) {
            logcat_fileonly.infoStream() << " HuDat present : using custom protk_hudat.dat (" << hudatExt << ")" << ::log4cpp::eol;
            protkPath = proeIsisExtPath / "plugins" / "protk_hudat.dat";
            if (! ::boost::filesystem::is_regular_file(protkPath)) {
                std::stringstream msg;
                msg << "the \"protk.dat\" file has a problem (existance?): "
                    << protkPath.string();
                throw isis::application_exception(msg);
            }
            config_Pro << std::endl << "toolkit_registry_file  " << "" << protkPath.string() << "" << std::endl;
        }

        string treecfgfile = programInputArguments.is_designMode()?"tree_design_edit.cfg":"tree_component_edit.cfg";

        // only add the following line to the config when in design mode
        ::boost::filesystem::path modelTreeConfigPath = proeIsisExtPath / "plugins" / treecfgfile;
        if (! ::boost::filesystem::is_regular_file(modelTreeConfigPath)) {
            logcat_fileonly.warnStream() << "the model tree config file file has a problem (existance?): "
                                         << modelTreeConfigPath.string();
            config_Pro << std::endl << "# ";
        } else {
            config_Pro << std::endl;
        }
        // config_Pro << "mdl_tree_cfg_file $PROE_ISIS_EXTENSIONS\plugins\tree.cfg" << std::endl;
        config_Pro << "mdl_tree_cfg_file  " << "" << modelTreeConfigPath.string() << "" << std::endl;

        if (programInputArguments.configPro.length()>0)
        {
            ifstream is(programInputArguments.configPro);
            config_Pro << is.rdbuf();
            config_Pro << std::endl;
        }

        config_Pro.close();

        /////////////////////////////
        /////// Start Pro/E /////////
        /////////////////////////////
        logcat_fileonly.infoStream()
                << "CADCreoParametricCreateAssembly "<< ASSEMBLE_PTC_VERSION << ::log4cpp::eol
                << "Starting Creo-Parametric, this takes about 10 seconds...";

        char* creoStartChar = const_cast<char*>(creoStartCommand.c_str());

        ::boost::filesystem::current_path(workingDir);
        std::string textPath=std::string(proeIsisExt)+"plugins\\";
        logcat_fileonly.warnStream()
                << " command: " << creoStartChar << " text: " << textPath.c_str();
        isis::isis_ProEngineerStart(creoStartChar, const_cast<char*>(textPath.c_str()));

        ProTermFuncSet(ProTermAction);

        Pro_E_Running = true;
        logcat_fileonly.infoStream() << "Creo-Parametric successfully started.";

        /**
        The following establishes the MetaLink.
        It uses an asynchronous, producer/comsumer model.
        */

        boost::asio::io_service ios;

        /**
        Pull out the host and service from the command line
        */
        std::string delimiters(":");
        std::vector<std::string> parts;
        boost::split(parts, programInputArguments.syncConnectionString, boost::is_any_of(delimiters));

        std::string host = "127.0.0.1";  // localhost may be subject to firewall and DNS restrictions and not work
        std::string service = "15152";   // it looks like ISIS with a zero

        if (parts.size() > 1) {
            host = parts[0];
            service = parts[1];
            logcat_fileonly.infoStream() << "host: " << host << ", service: " << service;
        } else if (parts.size() > 0) {
            host = parts[0];
            logcat_fileonly.infoStream() << "host: " << host << ", service: " << service << "(default)";
        } else {
            logcat_fileonly.infoStream() << "host: " << host << "(default), service: " << service << "(default)";
        }

        SetupCreoPlugins();
        SetupCreoSelectPlugin();

        ProNotificationSet(PRO_PARAM_MODIFY_POST, (ProFunction)metaParameterModifyAction);

        isis::cad::CadFactoryAbstract::ptr cad_factory = isis::cad::creo::create();

        isis::MetaLinkAssemblyEditor::Pointer assembler_ptr(new isis::MetaLinkAssemblyEditor( cad_factory, programInputArguments, isis::GlobalModelData::Instance.CadComponentData));

        isis::EventLoopMonitor eventLoopMonitor;

        if (programInputArguments.is_passiveMode())
        {
            isis::GlobalModelData::Instance.designId = programInputArguments.designID;
        }

        isis::MetaLinkHandler metalink_handler(assembler_ptr, ios, host, service,
                                               programInputArguments.majorMode, programInputArguments.designID, eventLoopMonitor);  // pass design ID pass in as argument

        isis::GlobalModelData::Instance.metalink_handler_ptr = &metalink_handler;
        isis::GlobalModelData::Instance.metalinkAssemblyEditorPtr = assembler_ptr;

        metalink_handler.change_m_operator("creo parameteric create assembly");

		bool inputFileProcessed = false;


        while(!terminateProcess) {
            eventLoopMonitor.CreoEventLoop();
            if (metalink_handler.m_eventQueue.size()>0)
            {
                boost::unique_lock< boost::mutex > guard(eventLoopMonitor.m_mutex);
                isis::EditPointer edit = metalink_handler.m_eventQueue.front();
                metalink_handler.m_eventQueue.pop();
				if (programInputArguments.inputXmlFileName.size()!=0 && !inputFileProcessed)
				{
					metalink_handler.CreateAssembly(programInputArguments.inputXmlFileName);
					inputFileProcessed = true;
				}
                bool result = metalink_handler.processEdit(edit);
                if (result)
                {
                    isis::EditPointer ack(new meta::Edit());
                    ack->set_guid(edit->guid());
                    for (int i = 0; i < edit->topic_size(); i++)
                    {
                        *(ack->add_topic()) = edit->topic(i);
                    }
                    metalink_handler.send(ack);
                }
            }
        }

		metalink_handler.interrupt();
        metalink_handler.disconnect();

        ProEngineerEnd();


    } // END Try
    catch ( isis::application_exception& ex )
    {
        exceptionErrorStringStream  << "application error: " << ex.what();
        ExitCode = -1;
    }
    catch ( std::exception& ex )
    {
        exceptionErrorStringStream << "general exception: " << ex.what();
        ExitCode = -2;
    }
    catch ( ... )
    {
        exceptionErrorStringStream << "unspecified throwable (...):  Please report the error to the help desk.";
        ExitCode = -3;
    }

    if ( ExitCode != 0 )
    {
        // Write to _FAILED.txt
        std::string failedTxtFileName = "_FAILED.txt";
        bool addLineFeed = false;
        if ( isis::FileExists( failedTxtFileName.c_str() )) addLineFeed = true;

        ofstream failedTxtFileStream;
        failedTxtFileStream.open (failedTxtFileName, ios::app );
        if ( failedTxtFileStream.is_open() )
        {
            if ( addLineFeed ) failedTxtFileStream << std::endl;
            failedTxtFileStream <<  isis_CADCommon::GetDayMonthTimeYear() << ", CADCreoParametricCreateAssembly.exe error code: " << ExitCode << ".  For additional information, scroll to the bottom of " << programInputArguments.logFileName;
            failedTxtFileStream.close();
        }

        if (Logging_Set_Up)
        {
            log4cpp::Category& logcat_fileonly = log4cpp::Category::getInstance(LOGCAT_LOGFILEONLY);
            logcat_fileonly.errorStream() << exceptionErrorStringStream.str();
        } else {
            std::cerr << std::endl << std::endl << exceptionErrorStringStream.str() << std::endl << std::endl;
        }

    }


    // Delete the copied template assembly file if it exists.
    // Note - Added "if ( Template_Copied )" because the function call was returning a message to the
    // console if the file did not exist.
    if ( Template_Copied )
    {
        ::boost::filesystem::path deleteFile = workingDir / (isis::TEMPLATE_MODEL_NAME_METRIC + isis::TEMPLATE_MODEL_NAME_METRIC_SUFFIX);
        if (::boost::filesystem::exists(deleteFile) ) {
            ::boost::filesystem::remove(deleteFile);
        }
        // isis::IfFileExists_DeleteFile( deleteFile.generic_string() );
    }



    /*log4cpp::Category& logcat_fileonly = log4cpp::Category::getInstance(LOGCAT_LOGFILEONLY);
    ::boost::filesystem::current_path(original_directory);

    // Cleanup - Delete the working dir after execution
    ::boost::system::error_code ec;
    if (isis::GlobalModelData::Instance.mode == isis::DESIGNEDIT)
    {
    	// Remove files one-by-one so if the directory removal fails still something is removed
    	::boost::filesystem::path deleteFile = workingDir / "*";
    	logcat_fileonly.debugStream() << workingDir / "*";
    	if (::boost::filesystem::exists(deleteFile) ) {
    		::boost::filesystem::remove_all(deleteFile, ec);
    		if (ec != 0)
    		{
    			logcat_fileonly.errorStream() << "Failed to remove working directory, ec: " << ec;
    		}
    	}
    	deleteFile = workingDir;
    	logcat_fileonly.debugStream() << workingDir;
    	if (::boost::filesystem::exists(deleteFile) ) {
    		::boost::filesystem::remove_all(deleteFile, ec);
    		if (ec != 0)
    		{
    			logcat_fileonly.errorStream() << "Failed to remove working directory, ec: " << ec;
    		}
    	}
    } else if (isis::GlobalModelData::Instance.mode == isis::COMPONENTEDIT)
    {
    	::boost::filesystem::path deleteFile = workingDir / "config.pro";
    	logcat_fileonly.debugStream() << workingDir / "config.pro";
    	if (::boost::filesystem::exists(deleteFile) ) {
    		::boost::filesystem::remove_all(deleteFile, ec);
    		if (ec != 0)
    		{
    			logcat_fileonly.errorStream() << "Failed to remove working directory, ec: " << ec;
    		}
    	}
    	deleteFile = workingDir / "std.err";
    	logcat_fileonly.debugStream() << workingDir / "std.err";
    	if (::boost::filesystem::exists(deleteFile) ) {
    		::boost::filesystem::remove_all(deleteFile, ec);
    		if (ec != 0)
    		{
    			logcat_fileonly.errorStream() << "Failed to remove working directory, ec: " << ec;
    		}
    	}
    	deleteFile = workingDir / "trail.txt.*";
    	logcat_fileonly.debugStream() << workingDir / "trail.txt.*";
    	if (::boost::filesystem::exists(deleteFile) ) {
    		::boost::filesystem::remove_all(deleteFile, ec);
    		if (ec != 0)
    		{
    			logcat_fileonly.errorStream() << "Failed to remove working directory, ec: " << ec;
    		}
    	}
    }
    */
    log4cpp::Category::shutdown();
	
    exit(ExitCode);
}
