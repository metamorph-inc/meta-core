// CADCreoParametricDomainFunctionsDriver.cpp : Defines the entry point for the console application.
//
#include <ProToolkit.h> // must go first
#include "../CADCreoParametricCommonFunctions/ExteriorShell.h"

#include "zmq_helper.h"

#include <boost/thread.hpp>
#include <boost/filesystem.hpp>s
#include <boost/lexical_cast.hpp>
#include <boost/program_options.hpp>

#include <pfcSession.h>
#include <pfcModel.h>

#include <ciplib.h>

#include <pfcGlobal.h>
#include <pfcFeature.h>
#include <wfcElementTree.h>
#include <wfcCollection.h>
#include <wfcElemIds.h>
#include <wfcFeature.h>
#include <wfcSolid.h>
#include <pfcSelect.h>
#include <wfcSession.h>
#include <pfcArgument.h>
#include <wfcFeatureInstructions.h>

#include <ProMdl.h>
#include <ProFeature.h>
#include <ProSelection.h>
#include <wfcGlobal.h>

#include <ProToolkitErrors.h>
#include <pfcAsyncConnection.h>
#include <pfcCommand.h>

#include <ProCore.h>
#include <pfcModel.h>
#include <pfcSession.h>
#include <ciplib.h>
#include <pfcExport.h>

#include "creo_save_feature_cpp.h"


void set_part_search_path(::boost::filesystem::path in_path)  {
    /*
      MultiFormatString searchPath_MultiFormat(searchPathString, PRO_PATH_SIZE - 1);
                        wcscpy( optionName, L"search_path");
                        wcscpy( optionValue, searchPath_MultiFormat);
                        isis_ProConfigoptSet(optionName, optionValue  );
                        m_logcat.infoStream() << "Added serach path: " << searchPathString;
                        searchPaths.insert(searchPathString_uppercase);
    in_MetaLinkAssemblyEditor.AddSearchPaths(in_SearchPaths);
    */
}

/** load a previously saved assembly */
pfcAssembly_ptr load_assembly(pfcSession_ptr session, const ::boost::filesystem::path in_path) {
    pfcModelDescriptor_ptr model_descriptor;
    model_descriptor->CreateFromFileName(in_path.generic_string().c_str());
    pfcModel_ptr model = session->RetrieveModel(model_descriptor);
    pfcModelType type = model->GetType();
    pfcSolid_ptr solid = pfcSolid::cast(model);
    if (solid->HasRetrievalErrors()) {
        std::cout << "retrieval errors" << std::endl;
    }
    return pfcAssembly::cast(solid);
}

struct Configuration {
public:
	::boost::filesystem::path working_dir;
	::boost::filesystem::path cad_dir;
};

Configuration parse_input(int in_argc, char* in_argv[]) {

	std::cout << "parsing input" << std::endl;

    namespace po = ::boost::program_options;
	po::options_description visible_options("Allowed options");

	visible_options.add_options()
			("w", po::value<std::string>(), " Working directory.  The directory where the generated files would be persisted.")
			("c", po::value<std::string>(), " CAD directory.  This directory contains Creo part files.") ;

	po::variables_map vm;
	po::store(po::command_line_parser(in_argc, in_argv).options(visible_options).
			style(po::command_line_style::default_style | 
			po::command_line_style::case_insensitive | 
			po::command_line_style::allow_long_disguise).run(), vm);

	Configuration config;
	config.working_dir = (vm.count("w") < 1) ? ::boost::filesystem::current_path() : vm["w"].as<std::string>();
	config.cad_dir = (vm.count("c") < 1) ? ::boost::filesystem::current_path() : vm["c"].as<std::string>();

	std::cout << "parsed input: " 
		<< " working dir: " << config.working_dir << std::endl
		<< " cad dir: "     << config.cad_dir << std::endl;

	return config;
}

extern "C" ProError otkAsyncFullMode_init ();
extern "C" ProError otkAsyncFullMode_run  ();
// extern "C" ProError otkAsyncSimpleMode_init ();
// extern "C" ProError otkAsyncSimpleMode_run ();

static void config_creo(Configuration config) {
	ofstream config_Pro;	
	::boost::filesystem::path configPro_PathAndFileName = config.working_dir / "config.pro";
	config_Pro.open( configPro_PathAndFileName.string() );
	
	config_Pro << "override_store_back YES" << std::endl; 
	config_Pro << "enable_sociallink NO" << std::endl; 
	config_Pro << "search_path" << " " << "C:\\Users\\feisele\\Desktop\\meta-data\\Hull_and_Hook\\Creo files" << std::endl;
	config_Pro << "toolkit_registry_file" << std::endl;
	
	::boost::filesystem::path searchMetaFileName = "./search_META.pro";
	if ( ::boost::filesystem::exists( searchMetaFileName )) {
		config_Pro << "search_path_file " << "\"" << searchMetaFileName.string() << "\"";
	}

	::boost::filesystem::path cadPartsLibDir = config.cad_dir;
	if ( ::boost::filesystem::exists( cadPartsLibDir )) {
		config_Pro << std::endl << "search_path " << "\"" << cadPartsLibDir.string() << "\"" << std::endl;
	}	

	config_Pro.close();
}

bool clear_message_frames( zmq::socket_t& responder, zmq::message_t& last_frame) {
	if (! last_frame.more()) return true;

    zmq::message_t more_frame;
	do {
        responder.recv(&more_frame);
	} while (more_frame.more());
	return true;
}

/**
The zmq should really wait until the creo connection is established.
*/
int main(int argc, char* argv[])
{
	ProError status;
	std::cout << "current directory: " << ::boost::filesystem::current_path() << std::endl;

	Configuration config = parse_input(argc, argv);
	config_creo(config);

    pfcAssembly_ptr assembly;
    pfcModel_ptr shell_model;
    isis::hydrostatic::ExteriorShell_ptr m_shell_ptr;
    isis::hydrostatic::Hydrostatic_ptr m_hydrostatic_ptr;

    ::boost::filesystem::path program_file_path = "C:/Program Files";
    /*
    — Turn off the graphics display.
    - Causes Creo Parametric to expect input from this asynchronous application only.
    */
	::boost::filesystem::path creo_root_dir = program_file_path / "PTC" / "Creo 2.0";

    // ::boost::filesystem::path creo_exe 
	// ::boost::filesystem::path creo_exe = "C:/PROGRA~1/PTC/CREO2~1.0/PARAME~1/bin/parametric.exe";
	::boost::filesystem::path creo_exe = creo_root_dir / "Parametric" / "bin" / "parametric.exe";
	creo_exe.make_preferred();
	std::string creo_exe_str = creo_exe.string<std::string>();
	char* creo_exe_ca = const_cast<char*>(creo_exe_str.c_str());

    std::string creo_args = ""; // "-g:no_graphics -i:rpc_input";
    std::string creo_cmd;
	creo_cmd.append(creo_exe_ca).append(" ").append(creo_args);
	char* creo_cmd_ca = const_cast<char*>(creo_cmd.c_str());
	std::cout << "cmd: [" << strlen(creo_cmd_ca) << "] " << creo_cmd_ca << std::endl;

	::boost::filesystem::path creo_msg_file = 
		creo_root_dir / "Common Files" / "M110" / "x86e_win64" / "obj" / "pro_comm_msg";
	creo_msg_file.make_preferred();
	std::string creo_msg_file_str = creo_msg_file.string<std::string>();
	char* creo_msg_ca = const_cast<char*>(creo_msg_file_str.c_str());
	std::cout << "msg: [" << strlen(creo_msg_ca) << "] " << creo_msg_ca << std::endl;
	_putenv_s( "PRO_COMM_MSG_EXE", creo_msg_ca );

    ::boost::filesystem::path text_path = program_file_path / "META" / "Proe ISIS Extensions" / "plugins";
	text_path.make_preferred();
	std::string text_path_str = text_path.string<std::string>();
	char* text_path_ca = const_cast<char*>(text_path_str.c_str());
	std::cout << "txt: [" << strlen(text_path_ca) << "] " << text_path_ca << std::endl;

	/*
	switch( status = ProEngineerStart(creo_cmd_ca, text_path_ca) ) {
	case PRO_TK_NO_ERROR:
		break;
	default:
      std::cout << "Connection wasn't established " << status << std::endl
                << "creo_cmd: " << creo_cmd_ca << std::endl
                << "text_path: " << text_path_ca << std::endl;
      return -1;
    }

	ProTermFuncSet(ProTermAction);
	// while(1) {
	// ProEventProcess();
	// }ProAsynchronousEventLoop();
	*/

    pfcAsyncConnection_ptr connection = pfcAsyncConnection::Start( creo_cmd_ca, text_path_ca );
    if (!connection) {
      std::cout << "Connection wasn't established" << std::endl
                << "creo_cmd: " << creo_cmd << std::endl
                << "text_path: " << text_path << std::endl;
      return -1;
    }
    pfcSession_ptr session = connection->GetSession();
    // int pfc_version = session->GetReleaseNumericVersion();

    // int status;
    // status = otkAsyncSimpleMode_init();
    // status = otkAsyncSimpleMode_run();

	
    int major_version, minor_version, patch_version;
    zmq::version(&major_version, &minor_version, &patch_version);
    std::cout << "0mq version: " << major_version << ":" << minor_version << ":" << patch_version << std::endl;

    zmq::context_t context(1);
    zmq::socket_t responder(context, ZMQ_REP);
    responder.bind("tcp://*:15150");

    std::string PROTOCOL_NAME = "META";

    while (true) {

        /* receive request */
        std::string protocol;
		try {
			zmq::message_t protocol_frame;
			responder.recv(&protocol_frame);
			protocol = to_string(protocol_frame);

			if (PROTOCOL_NAME.compare(protocol) != 0) {
			   std::cout << "received request but wrong protocol: " << protocol << std::endl;
			   clear_message_frames( responder, protocol_frame);
			   continue;
			}

			if (!protocol_frame.more()) {
			   std::cout << "received nothing following protocol: " << protocol << std::endl;
			   continue;
			}
		} catch (zmq::error_t ex) {
			std::cout << "error " << ex.what() << std::endl;
			continue;
		}

        std::string protocol_version;
		try {
			zmq::message_t version_frame;
			responder.recv(&version_frame);
			protocol_version = to_string(version_frame);
			std::cout << "protocol version " << protocol_version << std::endl;

			/* do the work */
			if (! version_frame.more()) {
			   std::cout << "received nothing following version: " << protocol << " : " << protocol_version << std::endl;
			   continue;
			}
		} catch (zmq::error_t ex) {
			std::cout << "error " << ex.what() << std::endl;
			continue;
		}

		std::string work;
		bool has_more; 
		try {
			zmq::message_t work_frame;
			responder.recv(&work_frame);
			has_more = work_frame.more();
			work = to_string(work_frame);
		} catch (zmq::error_t ex) {
			std::cout << "error " << ex.what() << std::endl;
			continue;
		}

		std::cout << "work: " << work << std::endl;

		/* echo  */
		if( std::string("echo").compare(work) == 0) {
			try {
				if (! has_more) {
				   std::cout << "received nothing following 'echo': " 
					   << protocol << " : " << protocol_version << std::endl;
				   continue;
				}
    			std::string msg;
				{
					zmq::message_t msg_frame;
    				responder.recv(&msg_frame);
    				msg = to_string(msg_frame);

					clear_message_frames( responder, msg_frame);
				}

				/* send the response */
				responder.send(PROTOCOL_NAME.c_str(), PROTOCOL_NAME.size(), ZMQ_SNDMORE);
				responder.send(protocol_version.c_str(), protocol_version.size(), ZMQ_SNDMORE);
				responder.send(work.c_str(), work.size(), ZMQ_SNDMORE);
				responder.send(msg.c_str(), msg.size(), ZMQ_SNDMORE);

				/* do the work */
				zmq::message_t reply_frame = to_zmessage("processed");
				responder.send(reply_frame);
				continue;
			} catch (zmq::error_t ex) {
				std::cout << "error " << ex.what() << std::endl;
				continue;
			}
		}
		/* dump features as  */
		if( std::string("dump-selected-feature").compare(work) == 0) {
			zmq::message_t feature_frame;
			responder.recv(&feature_frame);
			std::string feature = to_string(feature_frame);
			std::string cpp = creo_save_feature_selected_as_cpp();

			/* send the response */
			zmq::message_t reply = to_zmessage(cpp);
			responder.send(PROTOCOL_NAME.c_str(), PROTOCOL_NAME.size(), ZMQ_SNDMORE);
			responder.send(protocol_version.c_str(), protocol_version.size(), ZMQ_SNDMORE);
			responder.send(work.c_str(), work.size(), ZMQ_SNDMORE);
			responder.send(reply);
			continue;
		}
		/* dump features as  */
		if( std::string("dump-feature").compare(work) == 0) {
			if (! has_more ) {
				std::string result = creo_save_feature_all_as_cpp();

				/* send the response */
				responder.send(PROTOCOL_NAME.c_str(), PROTOCOL_NAME.size(), ZMQ_SNDMORE);
			    responder.send(protocol_version.c_str(), protocol_version.size(), ZMQ_SNDMORE);
				responder.send(work.c_str(), work.size(), ZMQ_SNDMORE);
				zmq::message_t reply = to_zmessage(result);
				responder.send(reply);
				continue;
			} 
			zmq::message_t feature_frame;
			responder.recv(&feature_frame);
		    int feature_id = to_int(feature_frame);
			std::string cpp = creo_save_feature_id_as_cpp(feature_id);

			/* send the response */
			zmq::message_t reply = to_zmessage(cpp);
			responder.send(PROTOCOL_NAME.c_str(), PROTOCOL_NAME.size(), ZMQ_SNDMORE);
			responder.send(protocol_version.c_str(), protocol_version.size(), ZMQ_SNDMORE);
			responder.send(work.c_str(), work.size(), ZMQ_SNDMORE);
			responder.send(reply);
			continue;
		}
		/* set the Creo part search path */
		if( std::string("set-part-search-path").compare(work) == 0) {
			while(true) {
				zmq::message_t path_frame;
				responder.recv(&path_frame);
				boost::filesystem::path path = to_file_path(path_frame);
				set_part_search_path(path);

				if (! path_frame.more()) {
					break;
				}
			}
			
			/* send the response */
			zmq::message_t reply = to_zmessage(std::string("processed"));
			responder.send(reply);
			continue;
		}
		/* load the source assembly */
		if( std::string("load-assembly").compare(work) == 0) {

			zmq::message_t path_frame;
			responder.recv(&path_frame);
			boost::filesystem::path path = to_file_path(path_frame);

			assembly = load_assembly(session, path);
			
			/* send the response */
			zmq::message_t reply = to_zmessage(std::string("processed"));
			responder.send(reply);
			continue;
		}
		/* init shell */
		if( std::string("init-shell").compare(work) == 0) {

			zmq::message_t name_frame;
			responder.recv(&name_frame);
			std::string name = to_string(name_frame);

			m_shell_ptr = isis::hydrostatic::ExteriorShell::create(name);
			
			/* send the response */
			zmq::message_t reply = to_zmessage(std::string("processed"));
			responder.send(reply);
			continue;
		} 
		/* shrink wrap the loaded assembly and save it to a file */
		if( std::string("create-shell").compare(work) == 0) {

			zmq::message_t name_frame;
			responder.recv(&name_frame);
			std::string name = to_string(name_frame);

			isis::hydrostatic::ExteriorShell_ptr shell_ptr = isis::hydrostatic::ExteriorShell::create(name);
			shell_ptr->make(session, assembly);
			m_shell_ptr = shell_ptr;
			
			/* send the response */
			zmq::message_t reply = to_zmessage(std::string("processed"));
			responder.send(reply);
			continue;
		} 
		/* load the shrink-wrap shell */
		if( std::string("load-shell").compare(work) == 0) {
			zmq::message_t path_frame;
			responder.recv(&path_frame);
			boost::filesystem::path path = to_file_path(path_frame);

			isis::hydrostatic::ExteriorShell_ptr shell_ptr = isis::hydrostatic::ExteriorShell::create(path.stem().generic_string());
			shell_ptr->load(session, path);
			m_shell_ptr = shell_ptr;
			
			/* send the response */
			zmq::message_t reply = to_zmessage(std::string("processed"));
			responder.send(reply);
			continue;
		}
		/* set the trim angles to be used during the water-line computations */
		if( std::string("set-trim").compare(work) == 0) {
			std::cout << "shell value: " << m_shell_ptr << std::endl;
			if (m_shell_ptr == NULL) {
			    std::cout << "shell value not set" << std::endl;
				m_shell_ptr = isis::hydrostatic::ExteriorShell::create("defaulted");
			}
			while(true) {
				zmq::message_t trim_frame;
				responder.recv(&trim_frame);
				double trim = to_double(trim_frame);
				m_shell_ptr->addTrimAngle(trim);

				if (! trim_frame.more()) {
					break;
				}
			}
			
			/* send the response */
			zmq::message_t reply = to_zmessage(std::string("processed"));
			responder.send(reply);
			continue;
		}
		/* set the heel angles */
		if( std::string("set-heel").compare(work) == 0) {
			if (m_shell_ptr == NULL) {
			    std::cout << "shell value not set" << std::endl;
				m_shell_ptr = isis::hydrostatic::ExteriorShell::create("defaulted");
			}
			while(true) {
				zmq::message_t trim_frame;
				responder.recv(&trim_frame);
				double trim = to_double(trim_frame);
				m_shell_ptr->addTrimAngle(trim);

				if (! trim_frame.more()) {
					break;
				}
			}
			
			/* send the response */
			zmq::message_t reply = to_zmessage(std::string("processed"));
			responder.send(reply);
			continue;
		}
		/* set the submersion displacements */
		if( std::string("set-displacement").compare(work) == 0) {
			if (m_shell_ptr == NULL) {
			    std::cout << "shell value not set" << std::endl;
				m_shell_ptr = isis::hydrostatic::ExteriorShell::create("defaulted");
			}
			while(true) {
				zmq::message_t displacement_frame;
				responder.recv(&displacement_frame);
				double displacement = to_double(displacement_frame);
				m_shell_ptr->addDisplacement(displacement);

				if (! displacement_frame.more()) {
					break;
				}
			}
			
			/* send the response */
			zmq::message_t reply = to_zmessage(std::string("processed"));
			responder.send(reply);
			continue;
		}
		/* GOAL: generate the hydrostatic information */

		if( std::string("compute-hydrostatic").compare(work) == 0) {
			if (m_shell_ptr == NULL) {
			    std::cout << "shell value not set" << std::endl;
				m_shell_ptr = isis::hydrostatic::ExteriorShell::create("defaulted");
			}
			zmq::message_t tolerance_frame;
			responder.recv(&tolerance_frame);
			double tolerance = to_double(tolerance_frame);

			m_hydrostatic_ptr = m_shell_ptr->computeHydrostatic(tolerance);
			
			/* send the response */
			zmq::message_t reply = to_zmessage(std::string("processed"));
			responder.send(reply);
			continue;
		}
		/* using the hydrostatic object interpolate a single center of hydrostatic and depth */
		if( std::string("get-hydrostatic").compare(work) == 0) {
			if (m_hydrostatic_ptr == NULL) {
			    std::cout << "hydrostaticshell value not set" << std::endl;
				continue;
			}

			zmq::message_t displacement_frame;
			responder.recv(&displacement_frame);
			double displacement = to_double(displacement_frame);

			zmq::message_t heel_frame;
			responder.recv(&heel_frame);
			double heel = to_double(heel_frame);

			zmq::message_t trim_frame;
			responder.recv(&trim_frame);
			double trim = to_double(trim_frame);

			::boost::tuple< isis::hydrostatic::cob_type, isis::hydrostatic::depth_type > value 
				= m_hydrostatic_ptr->interpolate(displacement, heel, trim);
			std::cout << "hydrostatic " << value.get<0>() << " : " << value.get<1>() << std::endl;
			
			/* send the response */
			zmq::message_t reply = to_zmessage(std::string("processed"));
			responder.send(reply);
			continue;
		}
		/* compute a single center of hydrostatic */
		if( std::string("compute-displacement").compare(work) == 0) {

			zmq::message_t depth_frame;
			responder.recv(&depth_frame);
			double depth = to_double(depth_frame);

			zmq::message_t heel_frame;
			responder.recv(&heel_frame);
			double heel = to_double(heel_frame);

			zmq::message_t trim_frame;
			responder.recv(&trim_frame);
			double trim = to_double(trim_frame);

			isis::hydrostatic::ExteriorShell_ptr shell_ptr = ::isis::hydrostatic::ExteriorShell::create("hydrostatic");
            ::boost::tuple< double, ::isis::hydrostatic::cob_type > buoy = shell_ptr->computeHydrostatic(depth, heel, trim);
            std::cout << "depth " << buoy.get<0>() << std::endl;
			std::cout << "center of hydrostatic " << buoy.get<1>() << std::endl;
			
			/* send the response */
			zmq::message_t reply = to_zmessage(std::string("processed"));
			responder.send(reply);
			continue;
		}
		{
			std::cout << "unknown work type: " << work << std::endl;
		}
    }
    responder.close();
    return 0;
}

