#include "stdafx.h"
#include <CommonUtilities.h>
#include <CreoPlugins/CreoPlugins.h>
#include <ISISConstants.h>

#include <ProNotify.h>
#include <AssembleUtils.h>
#include "cc_MiscellaneousFunctions.h"

#include "cc_WindowsFunctions.h"
#include <AssemblyEditingViaLink.h>
#include <MetaLinkHandler.h>
#include <Test_MetaLink.h>
#include <ISISVersionNumber.h>
#include "CADFactoryAbstract.h"
#include "CADFactoryCreo.h"
#include "CreoErrorCodes.h"
#include <AssembleUtils.h>

#include <boost/filesystem.hpp>
#include "cc_LoggerBoost.h"
#include <cc_CommonUtilities.h>
#include "GlobalModelData.h"
#include "AssembleUtils.h"
#include <boost/exception/all.hpp>


boost::atomic<bool> producerDone(false);
boost::atomic<bool> terminateProcess(false);

ProError ProTermAction(ProeTerminationStatus term_type)
{
    terminateProcess=true;
    if(isis::GlobalModelData::Instance.metalink_handler_ptr != 0)
    {
        isis::GlobalModelData::Instance.metalink_handler_ptr->interrupt();
    }
    return PRO_TK_NO_ERROR;
}

/**
* The callback to be used when a component or datum of a component is selected.
*	ProSelectionExtHighlightAct
*/
void informHightlightSelectionEvent(ProSelection selection, ProBoolean highlight, ProAppData ext_data)
{
    isis_LOG(lg, isis_FILE, isis_ERROR) << "caught event : "
                         << selection << " highlight-toggle: " << highlight << " external-data: " << ext_data;
}

/**
implements ProAsmcompActivatePreAction()
*/
ProError metaAsmcompActivatePreAction(ProAsmcomppath *active_path, ProSolid active_model)
{
    isis_LOG(lg, isis_FILE, isis_ERROR) << "caught event : "
                         << " active-path: " << active_path << " acive-model: " << active_model;
    return PRO_TK_NO_ERROR;
}

/**
implements ProMdlDisplayPostAction()
*/
ProError metaMdlDisplayPostAction(void)
{
    const std::string targetId = isis::GlobalModelData::Instance.designId;
    isis::GlobalModelData::Instance.metalink_handler_ptr->send_LocateSelectedRequest(isis::GlobalModelData::Instance.instanceId, targetId);
    return PRO_TK_NO_ERROR;
}

ProError metaParameterModifyAction(ProParameter *param, ProParamvalue *old_value)
{
    return PRO_TK_NO_ERROR;
}



int main(int argc, char *argv[])
{
    int ExitCode = 0;

    std::string			creoStartCommand;
    std::string			CADExtensionsDir;

    std::string			templateFile_PathAndFileName;
    std::stringstream	exceptionErrorStringStream;

	std::string			exeName = "CADCreoParametricMetaLink.exe";

    bool Logging_Set_Up = false;

    isis::MetaLinkInputArguments  programInputArguments;
    ::boost::filesystem::path    workingDir;

	std::stringstream inputLine;

    try
    {
		// Build string of input line
        for(int i = 0; i < argc; ++i)
        {
            inputLine << argv[i] << std::string(" ");
        }


        // Parse Input Arguments
        programInputArguments.ParseInputArguments(argc, argv);

        // Setup Boost logging
        //SetupLogging(programInputArguments.logFileName, programInputArguments.logVerbosity);
		SetupLogging("", programInputArguments.logFileName, true, false, programInputArguments.logVerbosity, isis_INFO );

        Logging_Set_Up = true;

        programInputArguments.ThrowException_If_InvalidInputArguments(argc, argv);

        // Must get the complete path to the working directory.  This is necessary because when
        // isis_ProDirectoryChange is called to change to a STEP directory, workingDir must be fully
        // defined so that isis_ProDirectoryChange can be called to return to the original directory.
        workingDir = isis::SetupWorkingDirectory(programInputArguments.workingDirectory);

		isis::GlobalModelData::Instance.instanceId = programInputArguments.instanceID;

        // Log CADCreoParametricCreateAssembly version information
        std::string programName_Version_TimeStamp;
        programName_Version_TimeStamp = "CADCreoParametricMetaLink " + std::string(ISIS_PRODUCT_VERSION_WITH_v_AND_DOTS);

        ///////////////////
        // Add Time Stamp
        ///////////////////

        programName_Version_TimeStamp += isis_CADCommon::GetDayMonthTimeYear();
        isis_LOG(lg, isis_FILE, isis_INFO) << programName_Version_TimeStamp;

        isis_LOG(lg, isis_FILE, isis_INFO) << "Notes: " << isis_EOL
                                     << "   1. The \"Component Instance ID\"s in this file equate to ComponentInstanceIDs in CyPhy."  << isis_EOL
                                     << "   2. To map \"Component Instance ID\"s in this file to AVM-IDs, see .\\log\\CyPhy2CAD.log." << isis_EOL;

        time_t time_start; /* calendar time */
        time_start=time(NULL); /* get current cal time */

        // Log input line and parameters
        isis_LOG(lg, isis_FILE, isis_INFO) << "Command line: " << inputLine.str();

		isis_LOG(lg, isis_FILE, isis_DEBUG) << "Input arguments (parsed): " << isis_EOL << programInputArguments;

        if(workingDir.generic_string().size() >= PRO_PATH_SIZE)      // PRO_PATH_SIZE   260
        {
            std::stringstream errorString;
            errorString << "WORKING_DIR string too long.  Maximum allowed number of characters: "  << PRO_PATH_SIZE - 1 << " WORKING_DIR string: " << workingDir;
            throw isis::application_exception(errorString);
        }

		isis::cad::CadFactoryAbstract::ptr cAD_Factory = isis::cad::creo::create();
		isis::cad::IEnvironment&           environment = cAD_Factory->getEnvironment();

		//environment.setupCADEnvironment(	isis::cad::OPENMETA_META_LINK,					// in
		//									workingDir.generic_string(),					// in 
		//									programInputArguments.auxiliaryCADDirectory,    // Auxiliary CAD Directory
		//									programInputArguments.graphicsModeOn,			// in
		//									programInputArguments.synchronizeWithCyPhy,		// in
		//									creoStartCommand,								// out
		//									CADExtensionsDir,								// out
		//									templateFile_PathAndFileName);					// out

		environment.setupCADEnvironment(	programInputArguments,
											creoStartCommand,								// out
											CADExtensionsDir,								// out
											templateFile_PathAndFileName);					// out
  


		//isis::SetCreoEnvirVariable_RetrieveSystemSettings(programInputArguments.graphicsModeOn,
        //       programInputArguments.synchronizeWithCyPhy,
        //        creoStartCommand,
        //        CADExtensionsDir,
        //        templateFile_PathAndFileName);

        std::map<std::string, isis::CADComponentData> CADComponentData_map;
        isis::CADAssemblies CADComponentAssemblies;

		//writeConfigProFile(workingDir, programInputArguments);

        /////////////////////////////
        /////// Start Pro/E /////////
        /////////////////////////////

        char* creoStartChar = const_cast<char*>(creoStartCommand.c_str());

        ::boost::filesystem::current_path(workingDir);
        std::string textPath = isis::META_PATH() + "\\bin\\CAD\\Creo\\plugins\\";
        isis::isis_ProEngineerStart(creoStartChar, const_cast<char*>(textPath.c_str()));

        ProTermFuncSet(ProTermAction);

        isis_LOG(lg, isis_FILE, isis_INFO) << "Creo-Parametric successfully started.";

        boost::asio::io_service ios;
        std::string delimiters(":");
        std::vector<std::string> parts;
        boost::split(parts, programInputArguments.syncConnectionString, boost::is_any_of(delimiters));
        std::string host = "127.0.0.1";  // localhost may be subject to firewall and DNS restrictions and not work
        std::string service = "15152";   // it looks like ISIS with a zero

        if(parts.size() > 1)
        {
            host = parts[0];
            service = parts[1];
            isis_LOG(lg, isis_FILE, isis_INFO) << "host: " << host << ", service: " << service;
        }
        else if(parts.size() > 0)
        {
            host = parts[0];
            isis_LOG(lg, isis_FILE, isis_INFO) << "host: " << host << ", service: " << service << "(default)";
        }
        else
        {
            isis_LOG(lg, isis_FILE, isis_INFO) << "host: " << host << "(default), service: " << service << "(default)";
        }

        SetupCreoPlugins();
        SetupCreoSelectPlugin();

        ProNotificationSet(PRO_PARAM_MODIFY_POST, (ProFunction)metaParameterModifyAction);


        isis::MetaLinkAssemblyEditor::Pointer assembler_ptr(new isis::MetaLinkAssemblyEditor(cAD_Factory, programInputArguments, isis::GlobalModelData::Instance.CadComponentData));

        boost::mutex eventloop_mutex;

        if(programInputArguments.is_passiveMode())
        {
            isis::GlobalModelData::Instance.designId = programInputArguments.designID;
        }

		isis::MetaLinkHandler metalink_handler(assembler_ptr, ios, host, service, programInputArguments.instanceID, programInputArguments.designID, eventloop_mutex);  // pass design ID pass in as argument

        isis::GlobalModelData::Instance.metalink_handler_ptr = &metalink_handler;
        isis::GlobalModelData::Instance.metalinkAssemblyEditorPtr = assembler_ptr;

        metalink_handler.change_m_operator("CAD");

        bool inputFileProcessed = false;

        while (true)
        {
            ProEventProcess();
            if (terminateProcess)
            {
                break;
            }
            ProErr err;
            if ((err = ProEngineerStatusGet()) != PRO_TK_NO_ERROR)
            {
				if (terminateProcess)
				{
					break;
				}
				exceptionErrorStringStream << "Creo exited abnormally: " << isis::ProToolKitError_string(err).c_str();
                ExitCode = -4;
                break;
            }
            if(programInputArguments.inputXmlFileName.size()!=0 && !inputFileProcessed)
            {
                metalink_handler.CreateAssembly(programInputArguments.inputXmlFileName);
                inputFileProcessed = true;
            }
            if(metalink_handler.m_eventQueue.size()>0)
            {
                boost::unique_lock< boost::mutex > guard(eventloop_mutex);
                isis::EditPointer edit = metalink_handler.m_eventQueue.front();
                metalink_handler.m_eventQueue.pop();
                bool result = metalink_handler.processEdit(edit);
                if(result)
                {
                    isis::EditPointer ack(new meta::Edit());
                    for(int i = 0; i < edit->topic_size(); i++)
                    {
		        // FIXME: need to set notice mode to ack?
                        *(ack->add_topic()) = edit->topic(i);
                    }
		    // FIXME: GME side doesn't do anything with this (ie who cares?)
                    // metalink_handler.send(ack);
                }
            }
        }

        metalink_handler.interrupt();
        metalink_handler.disconnect();

        ProEngineerEnd();


    } // END Try

	catch( boost::program_options::required_option& ex)
	{
        exceptionErrorStringStream  << "application error: Missing required option when invoking " << exeName << ", " << boost::diagnostic_information(ex);
		exceptionErrorStringStream << std::endl << "Input Line: " <<  inputLine;
        ExitCode = -1;
	}
	catch( boost::program_options::error& ex)
	{
        exceptionErrorStringStream  << "application error: Error with the options passed to " << exeName << ", " << boost::diagnostic_information(ex);
		exceptionErrorStringStream << std::endl << "Input Line: " <<  inputLine;
        ExitCode = -2;
	}
	catch (boost::exception &ex)
	{
		exceptionErrorStringStream  << "application error: " << boost::diagnostic_information(ex);
        ExitCode = -3;
	}

    catch(isis::application_exception& ex)
    {
        exceptionErrorStringStream  << "application error: " << ex.what();
        ExitCode = -4;
    }
    catch(std::exception& ex)
    {
        exceptionErrorStringStream << "general exception: " << ex.what();
        ExitCode = -5;
    }
    catch(...)
    {
        exceptionErrorStringStream << "unspecified error, caught with (...):  Please report this error to the help desk.";
        ExitCode = -6;
    }

    if(ExitCode != 0)
    {
		LogMainNonZeroExitCode( exeName, ExitCode, inputLine, Logging_Set_Up, programInputArguments.logFileName,  exceptionErrorStringStream );
    }
    else
    {
        if(Logging_Set_Up)
        {
            isis_LOG(lg, isis_FILE, isis_INFO) << "Normal exit" << std::endl;
        }
    }


    /*
    ::boost::filesystem::current_path(original_directory);

    // Cleanup - Delete the working dir after execution
    ::boost::system::error_code ec;
    if (isis::GlobalModelData::Instance.mode == isis::DESIGNEDIT)
    {
    	// Remove files one-by-one so if the directory removal fails still something is removed
    	::boost::filesystem::path deleteFile = workingDir / "*";
    	isis_LOG(lg, isis_FILE, isis_DEBUG) << workingDir / "*";
    	if (::boost::filesystem::exists(deleteFile) ) {
    		::boost::filesystem::remove_all(deleteFile, ec);
    		if (ec != 0)
    		{
    			isis_LOG(lg, isis_FILE, isis_ERROR) << "Failed to remove working directory, ec: " << ec;
    		}
    	}
    	deleteFile = workingDir;
    	isis_LOG(lg, isis_FILE, isis_DEBUG) << workingDir;
    	if (::boost::filesystem::exists(deleteFile) ) {
    		::boost::filesystem::remove_all(deleteFile, ec);
    		if (ec != 0)
    		{
    			isis_LOG(lg, isis_FILE, isis_ERROR) << "Failed to remove working directory, ec: " << ec;
    		}
    	}
    } else if (isis::GlobalModelData::Instance.mode == isis::COMPONENTEDIT)
    {
    	::boost::filesystem::path deleteFile = workingDir / "config.pro";
    	isis_LOG(lg, isis_FILE, isis_DEBUG) << workingDir / "config.pro";
    	if (::boost::filesystem::exists(deleteFile) ) {
    		::boost::filesystem::remove_all(deleteFile, ec);
    		if (ec != 0)
    		{
    			isis_LOG(lg, isis_FILE, isis_ERROR) << "Failed to remove working directory, ec: " << ec;
    		}
    	}
    	deleteFile = workingDir / "std.err";
    	isis_LOG(lg, isis_FILE, isis_DEBUG) << workingDir / "std.err";
    	if (::boost::filesystem::exists(deleteFile) ) {
    		::boost::filesystem::remove_all(deleteFile, ec);
    		if (ec != 0)
    		{
    			isis_LOG(lg, isis_FILE, isis_ERROR) << "Failed to remove working directory, ec: " << ec;
    		}
    	}
    	deleteFile = workingDir / "trail.txt.*";
    	isis_LOG(lg, isis_FILE, isis_DEBUG) << workingDir / "trail.txt.*";
    	if (::boost::filesystem::exists(deleteFile) ) {
    		::boost::filesystem::remove_all(deleteFile, ec);
    		if (ec != 0)
    		{
    			isis_LOG(lg, isis_FILE, isis_ERROR) << "Failed to remove working directory, ec: " << ec;
    		}
    	}
    }
    */
  

    exit(ExitCode);
}
