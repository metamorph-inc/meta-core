// CADCreoParametricDomainFunctionsDriver.cpp : Defines the entry point for the console application.
//
#define _USE_MATH_DEFINES
#include <cmath>

#include <czmq.h>
#include <ProToolkit.h> // must go first

#include <iosfwd>

#include <boost/thread.hpp>
#include <boost/atomic.hpp>
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/program_options.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>

#include <ProMdl.h>
#include <ProSolid.h>
#include <ProFeature.h>
#include <ProSelection.h>

#include <ProToolkitErrors.h>
#include <ProCore.h>
#include <UtilPtc.h>
#include <ProUtil.h>

#include "ExteriorShell.h"
#include "Disassembler.h"
#include <Joint.h>
#include <JointCreo.h>

#include <string>
#include <iostream>
#include <clocale>
#include <locale>
#include <vector>

#include "CommonDefinitions.h"
#include <log4cpp/Category.hh>
#include <log4cpp/OstreamAppender.hh>
#include <log4cpp/FileAppender.hh>
#include <log4cpp/PatternLayout.hh>

namespace ic = ::isis::cad;
namespace bpt = ::boost::property_tree;

template<class Ptree>
const Ptree &empty_ptree() {
	static Ptree pt;
	return pt;
}

struct Configuration {
public:
	::boost::filesystem::path working_dir;
	::boost::filesystem::path cad_dir;
};

Configuration parse_input(int in_argc, char* in_argv[]) {
	log4cpp::Category& log_cf = log4cpp::Category::getInstance(LOGCAT_CONSOLEANDLOGFILE);

	log_cf.infoStream() << "parsing input";

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

	log_cf.infoStream() << "parsed input: " << log4cpp::eol
		<< " working dir: " << config.working_dir << log4cpp::eol
		<< " cad dir: "     << config.cad_dir;

	return config;
}

extern "C" ProError otkAsyncFullMode_init ();
extern "C" ProError otkAsyncFullMode_run  ();

static void config_creo(Configuration config) {
	std::ofstream config_Pro;	
	::boost::filesystem::path configPro_PathAndFileName = config.working_dir / "config.pro";
	config_Pro.open( configPro_PathAndFileName.string() );
	
	config_Pro << "override_store_back YES"; 
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


/*
bool clear_message_frames( zmq::socket_t& distributor, zmq::message_t& last_frame) {
	if (! last_frame.more()) return true;

    zmq::message_t more_frame;
	do {
        distributor.recv(&more_frame);
	} while (more_frame.more());
	return true;
}
*/
	
class Driver {
private:
	Configuration config;

	log4cpp::Category& m_log_fo;
	log4cpp::Category& m_log_co;
	log4cpp::Category& m_log_cf; 

	void* m_distributor;
	std::string m_protocol_task;
	isis::hydrostatic::ExteriorShell m_ex_shell;
	isis::hydrostatic::PolatedSpace::ptr m_hydrostatic;

private:
	bool to_double( zframe_t* frame, double& out_value ) {
		byte* raw_payload = zframe_data( frame );
		int size = static_cast<int>(zframe_size( frame ));
		m_log_cf.infoStream() << "size " << size;

		out_value = boost::lexical_cast<double>(raw_payload, size);
		return true;
	}

public:
	Driver(const Configuration config)
		: m_log_fo(log4cpp::Category::getInstance(LOGCAT_LOGFILEONLY)),
		  m_log_co(log4cpp::Category::getInstance(LOGCAT_CONSOLEONLY)),
		  m_log_cf(log4cpp::Category::getInstance(LOGCAT_CONSOLEANDLOGFILE)),
		  m_distributor(NULL), m_protocol_task(""), 
		  m_ex_shell("default"), m_hydrostatic(NULL)
	{}

public:

	bool run() {
		m_log_cf.infoStream() << "current directory: " << ::boost::filesystem::current_path();
	
		int major_version, minor_version, patch_version;
		zmq_version(&major_version, &minor_version, &patch_version);
		m_log_cf.infoStream() << "0mq version: " << major_version << ":" << minor_version << ":" << patch_version;

		zctx_t* context = zctx_new();

		m_distributor = zsocket_new(context, ZMQ_ROUTER);
		zsocket_bind(m_distributor, "tcp://*:15150");

		std::string PROTOCOL_NAME = "META";

		/*
		make a thread to watch zmq.
		if there is work to do then
		issue an interrupt.
		*/
		/**
		The following loop will continue until 
		*/
		zmsg_t* msg = NULL;
		while( true ) {
			if (msg != NULL) {
				zmsg_destroy( &msg );
			}
			/* process creo events until a request is received */
			while( msg == NULL ) {
				ProEventProcess();
				msg = zmsg_recv_nowait( m_distributor );
				boost::this_thread::sleep( boost::posix_time::milliseconds(1000) );
			}

			m_log_cf.infoStream() << "frame count: " << zmsg_size( msg ) 
				<< " all: " << zmsg_content_size( msg );
			zmsg_fprint( msg, stdout );

			std::string protocol_id;
			{
				zframe_t* frame = zmsg_pop( msg );
				if (frame == NULL) {
					m_log_cf.warnStream() << "missing protocol frame[0]: ";
					continue;
				}
				zframe_fprint( frame, "proto-id:", stdout );
				if ( 1 > zframe_size( frame ) ) {
				   m_log_cf.warnStream() << "not a REQ protocol: ";
				   continue;
				}
				char* id = zframe_strhex( frame );
				protocol_id = id;
				free( id );
				if (! zframe_more( frame )) {
				   zframe_destroy( &frame );
				   m_log_cf.warnStream() << "received nothing following protocol type ";
				   continue;
				}
				zframe_destroy( &frame );
			} 
			m_log_cf.infoStream() << "proto-id " << protocol_id;

			std::string protocol_name;
			zframe_t* protocol_name_frame;
			{
				zframe_t* frame = zmsg_pop( msg );
				if (frame == NULL) {
					m_log_cf.warnStream() << "missing protocol frame[1]: ";
					continue;
				}
				zframe_fprint( frame, "proto-name:", stdout );

				char* name = zframe_as_cstr(frame, false);
				m_log_cf.infoStream() << "proto-name: " << name;

				if (PROTOCOL_NAME.compare(name) != 0) {
				   zframe_destroy( &frame );
				   m_log_cf.warnStream() << "received request but wrong protocol: " << name;
				   free( name );
				   continue;
				}
				protocol_name = name;
				free( name );

				if (! zframe_more( frame )) {
				   zframe_destroy( &frame );
				   m_log_cf.warnStream() << "received nothing following protocol name: " << protocol_name;
				   continue;
				}
				// zframe_destroy( &frame );
				protocol_name_frame = frame;
			} 

			std::string protocol_version;
			zframe_t* protocol_version_frame;
			{
				zframe_t* frame = zmsg_pop( msg );
				if (frame == NULL) {
					m_log_cf.warnStream() << "missing protocol frame[2]: ";
					continue;
				}
				zframe_fprint( frame, "pro-version:", stdout );

				char* version = zframe_as_cstr(frame);
				m_log_cf.infoStream() << "protocol version " << version;
				protocol_version = version;
				free( version );

				if (! zframe_more( frame )) {
				   zframe_destroy( &frame );
				   m_log_cf.warnStream() << "received nothing following protocol version: " << protocol_version;
				   continue;
				}
				// zframe_destroy( &frame );
				protocol_version_frame = frame;
			} 

			std::string protocol_task;
			zframe_t* protocol_task_frame;
			{
				zframe_t* frame = zmsg_pop( msg );
				if (frame == NULL) {
					m_log_cf.warnStream() << "missing protocol frame[3]: ";
					continue;
				}
				zframe_fprint( frame, "pro-task:", stdout );

				char* task = zframe_as_cstr(frame);
				m_log_cf.infoStream() << "protocol task: " << task;
				protocol_task = task;
				free( task );
				// zframe_destroy( &frame );
				protocol_task_frame = frame;
			} 
		
			/* send the response */
			zframe_send( &protocol_name_frame, m_distributor, ZFRAME_REUSE + ZFRAME_MORE );
			zframe_send( &protocol_version_frame, m_distributor, ZFRAME_REUSE + ZFRAME_MORE );
			zframe_send( &protocol_task_frame, m_distributor, ZFRAME_REUSE + ZFRAME_MORE );
			m_protocol_task = protocol_task;

			m_log_cf.infoStream() << " protocol: " << protocol_name << " : " << protocol_version;

			if( std::string("start-creo").compare(protocol_task) == 0) {
				start_creo( msg );
			} else
			if( std::string("stop-creo").compare(protocol_task) == 0) {
				stop_creo( msg );
			} else
			if( std::string("denormalize-assembly").compare(protocol_task) == 0) {
				denormalize( msg );
			} else
			if( std::string("disassemble").compare(protocol_task) == 0) {
				disassemble( msg );
			} else
			if( std::string("echo").compare(protocol_task) == 0) {
				echo_request( msg );
			} else
			if( std::string("dump-selected-feature").compare(protocol_task) == 0) {
				dump_selected_feature( msg );
			} else
			if( std::string("dump-selected-feature-xml").compare(protocol_task) == 0) {
				dump_selected_feature_xml( msg );
			} else
			if( std::string("dump-features").compare(protocol_task) == 0) {
				dump_features( msg );
			} else
			if( std::string("set-part-search-path").compare(protocol_task) == 0) {
				set_part_search_path( msg );
			} else 
			if( std::string("load-assembly").compare(protocol_task) == 0) {
				load_assembly( msg );
			} else
			if( std::string("init-shell").compare(protocol_task) == 0) {
				init_shell( msg );
			} else 
			if( std::string("create-shell").compare(protocol_task) == 0) {
				create_shell( msg );
			} else 
			if( std::string("set-trim").compare(protocol_task) == 0) {
				set_trim( msg );
			} else
			if( std::string("set-heel").compare(protocol_task) == 0) {
				set_heel( msg );
			} else
			if( std::string("set-displacement").compare(protocol_task) == 0) {
				set_displacement( msg );
			} else
			if( std::string("get-hydrostatic-result").compare(protocol_task) == 0) {
				get_hydrostatic_result( msg );
			} else
			if( std::string("get-hydrostatic-space").compare(protocol_task) == 0) {
				get_hydrostatic_space( msg );
			} else
			if( std::string("compute-displacement").compare(protocol_task) == 0) {
				compute_displacement( msg );
			} else
			if( std::string("extract-joint").compare(protocol_task) == 0) {
				extract_joint( msg );
			} else
			if( std::string("infer-joint").compare(protocol_task) == 0) {
				infer_joints( msg );
			} else
			{
				m_log_cf.warnStream() << "unknown protocol task type: " << protocol_task;
			}
			zstr_send( m_distributor, "processed" );

			zframe_destroy( &protocol_name_frame );
			zframe_destroy( &protocol_version_frame );
			zframe_destroy( &protocol_task_frame );
		}
		zctx_destroy( &context );
	}

private:
	
	bool start_creo( zmsg_t* msg ) {
		ProError status;
		config_creo(config);

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
		m_log_cf.infoStream() << "cmd: [" << strlen(creo_cmd_ca) << "] " << creo_cmd_ca;

		::boost::filesystem::path creo_msg_file = 
			creo_root_dir / "Common Files" / "M110" / "x86e_win64" / "obj" / "pro_comm_msg";
		creo_msg_file.make_preferred();
		std::string creo_msg_file_str = creo_msg_file.string<std::string>();
		char* creo_msg_ca = const_cast<char*>(creo_msg_file_str.c_str());
		m_log_cf.infoStream() << "msg: [" << strlen(creo_msg_ca) << "] " << creo_msg_ca;
		_putenv_s( "PRO_COMM_MSG_EXE", creo_msg_ca );

		::boost::filesystem::path text_path = program_file_path / "META" / "Proe ISIS Extensions" / "plugins";
		text_path.make_preferred();
		std::string text_path_str = text_path.string<std::string>();
		char* text_path_ca = const_cast<char*>(text_path_str.c_str());
		m_log_cf.infoStream() << "txt: [" << strlen(text_path_ca) << "] " << text_path_ca;

		try {
			switch( status = ProEngineerStart( creo_cmd_ca, text_path_ca ) ) {
			case PRO_TK_NO_ERROR: break;
			default:
			  m_log_cf.warnStream() << "Connection wasn't established" << log4cpp::eol
						<< "creo_cmd: " << creo_cmd << log4cpp::eol
						<< "text_path: " << text_path;
			  return false;
			}
		} catch (...) {
			m_log_cf.errorStream() << "could not connect to Creo";
			return false;
		}
		return true;
	}

	bool stop_creo( zmsg_t* msg ) {
		ProError status;
		try {
			switch( status = ProEngineerEnd( ) ) {
			case PRO_TK_NO_ERROR: break;
			default:
			  m_log_cf.warnStream() << "Connection wasn't established";
			  return false;
			}
		} catch (...) {
			m_log_cf.errorStream() << "could not connect to Creo";
			return false;
		}
		return true;
	}

	bool echo_request( zmsg_t* msg ) {
		std::string protocol_payload;
		zframe_t* protocol_payload_frame;
		{
			if ( zmsg_size( msg ) < 1 ) {
				m_log_cf.warnStream() << "received nothing following '" << m_protocol_task << "': " 
					<< zmsg_size( msg ); 
				return false;
			}
			zframe_t* frame = zmsg_pop( msg );
			if (frame == NULL) {
				m_log_cf.warnStream() << "missing protocol frame[" << m_protocol_task << ":0]: ";
				return false;
			}
			char* payload = zframe_as_cstr(frame);
			m_log_cf.infoStream() << m_protocol_task << " payload: " << payload;
			protocol_payload = payload;
			free( payload );
			// zframe_destroy( &frame );
			protocol_payload_frame = frame;
		}
		zframe_send( &protocol_payload_frame, m_distributor, ZFRAME_REUSE );
		zframe_destroy( &protocol_payload_frame );

		return true;
	}

		/* dump features as  */
	bool dump_selected_feature( zmsg_t* msg ) {
		write_selected_feature();
		return true;
	}

	bool dump_selected_feature_xml( zmsg_t* msg ) {
		std::string protocol_payload;
		zframe_t* protocol_payload_frame;
		{
			if ( zmsg_size( msg ) < 1 ) {
				m_log_cf.warnStream() << "received nothing following '" << m_protocol_task << "': " 
					<< zmsg_size( msg ) ;
				return false;
			}
			zframe_t* frame = zmsg_pop( msg );
			if (frame == NULL) {
				m_log_cf.warnStream() << "missing protocol frame[" << m_protocol_task << ":0]: ";
				return false;
			}
			char* payload = zframe_as_cstr(frame);
			m_log_cf.infoStream() << m_protocol_task << " payload: " << payload;
			protocol_payload = payload;
			free( payload );
			// zframe_destroy( &frame );
			protocol_payload_frame = frame;
		}

		write_selected_feature_as_xml( const_cast<char*>(protocol_payload.c_str()) );
		zframe_send( &protocol_payload_frame, m_distributor, ZFRAME_REUSE );
		zframe_destroy( &protocol_payload_frame );
		return true;
	}

	/* dump features as  */
	bool dump_features( zmsg_t* msg ) {
		std::string protocol_payload;
		zframe_t* protocol_payload_frame;
		{
			if ( zmsg_size( msg ) < 1 ) {
				m_log_cf.warnStream() << "received nothing following '" << m_protocol_task << "': " 
					<< zmsg_size( msg );
				return false;
			}
			zframe_t* frame = zmsg_pop( msg );
			if (frame == NULL) {
				m_log_cf.warnStream() << "missing protocol frame[" << m_protocol_task << ":0]: ";
				return false;
			}
			char* payload = zframe_as_cstr(frame);
			m_log_cf.infoStream() << m_protocol_task << " payload: " << payload;
			protocol_payload = payload;
			free( payload );
			// zframe_destroy( &frame );
			protocol_payload_frame = frame;
		}

		write_all_feature(protocol_payload);
		
		zframe_send( &protocol_payload_frame, m_distributor, ZFRAME_REUSE );
		zframe_destroy( &protocol_payload_frame );
		return true;
	}

	/* set the Creo part search path */
	bool set_part_search_path( zmsg_t* msg ) {
		while( zmsg_size( msg ) > 0 ) {
			zframe_t* frame = zmsg_pop( msg );
			if (frame == NULL) {
				m_log_cf.warnStream() << "missing protocol frame[echo:0]: ";
				continue;
			}
			char* payload = zframe_as_cstr(frame);
			m_log_cf.infoStream() << m_protocol_task << " payload: " << payload;
			free( payload );
			zframe_destroy( &frame );
			boost::filesystem::path path( payload );

			/*

			ProConfigoptSet()
			MultiFormatString searchPath_MultiFormat(searchPathString, PRO_PATH_SIZE - 1);
								wcscpy( optionName, L"search_path");
								wcscpy( optionValue, searchPath_MultiFormat);
								isis_ProConfigoptSet(optionName, optionValue  );
								m_logcat.infoStream() << "Added serach path: " << searchPathString;
								searchPaths.insert(searchPathString_uppercase);
			in_MetaLinkAssemblyEditor.AddSearchPaths(in_SearchPaths);
			
			*/
		}
		return true;
	} 
 
	/* set the Creo current directory */
	bool set_working_path( zmsg_t* msg ) {
		while( zmsg_size( msg ) > 0 ) {
			zframe_t* frame = zmsg_pop( msg );
			if (frame == NULL) {
				m_log_cf.warnStream() << "missing protocol frame[echo:0]: ";
				continue;
			}
			char* payload = zframe_as_cstr(frame);
			m_log_cf.infoStream() << m_protocol_task << " payload: " << payload;
			free( payload );
			zframe_destroy( &frame );
			boost::filesystem::path path( payload );
			ProName pro_path;
			ProDirectoryChange(ProStringToWstring(pro_path, payload));
		}
		return true;
	} 
 

	/** load a previously saved assembly */
	bool load_assembly( zmsg_t* msg ) {
		/*
			pfcModelDescriptor_ptr model_descriptor;
			model_descriptor->CreateFromFileName(in_path.generic_string().c_str());
			pfcModel_ptr model = session->RetrieveModel(model_descriptor);
			pfcModelType type = model->GetType();
			pfcSolid_ptr solid = pfcSolid::cast(model);
			if (solid->HasRetrievalErrors()) {
				m_log_cf.warnStream() << "retrieval errors";
			}
			return pfcAssembly::cast(solid);
		}
		*/
		return false;
	}

	/* 
	init shell 
	if no name is specified the current/active model/solid is assumed.
	*/
	bool init_shell( zmsg_t* msg ) {
		if ( zmsg_size( msg ) < 1 ) {
			m_log_cf.warnStream() 
				<< "received nothing following '" 
				<< m_protocol_task << "': " 
				<< zmsg_size( msg );
			m_ex_shell.glom_wrapped_solid();
			return true;
		} 
		std::string file_stem;
		{
			zframe_t* frame = zmsg_pop( msg );
			if (frame == NULL) {
				m_log_cf.warnStream() << "missing protocol frame[" << m_protocol_task << ":0]: ";
				return false;
			}
			char* payload = zframe_as_cstr(frame);
			m_log_cf.infoStream() << m_protocol_task << " payload: " << payload;
			file_stem = payload;
			free( payload );
			zframe_fprint( frame, "file stem: ", stdout);
			zframe_destroy( &frame );
		}
		std::string file_extension;
		{
			zframe_t* frame = zmsg_pop( msg );
			if (frame == NULL) {
				m_log_cf.warnStream() << "missing protocol frame[" << m_protocol_task << ":0]: ";
				return false;
			}
			char* payload = zframe_as_cstr(frame);
			m_log_cf.infoStream() << m_protocol_task << " payload: " << payload;
			file_extension = payload;
			free( payload );
			zframe_fprint( frame, "file extension: ", stdout);
			zframe_destroy( &frame );
		}
		m_ex_shell.set_name( file_stem );
		m_ex_shell.activate_model( file_stem,
			(file_extension == "asm" ? PRO_MDL_ASSEMBLY : PRO_MDL_PART ) );

		return true;
	} 

	/* shrink wrap the loaded assembly and save it to a file */
	bool create_shell( zmsg_t* msg ) {
		ProError status;
		if (! m_ex_shell.has_base_solid()) {
			m_log_cf.warnStream() << "no current model ";
			return false;
		}
		if ( zmsg_size( msg ) > 0 ) {
			std::string name;
			{
				zframe_t* frame = zmsg_pop( msg );
				if (frame == NULL) {
					m_log_cf.warnStream() << "missing protocol frame[" << m_protocol_task << ":name]: ";
					return false;
				}
				zframe_fprint( frame, "name: ", stdout);
				char* payload = zframe_as_cstr(frame);
				if( payload == NULL ) {
					m_log_cf.warnStream() << "bad name value" << name;
					return false;
				}
				name = payload;
				zframe_destroy( &frame );
			}
			switch( status = m_ex_shell.create_shrinkwrap(name) ) {
			case PRO_TK_NO_ERROR: break;
			default:
				m_log_cf.warnStream() << "something went wrong " << status;
			}
		}
		return true;
	} 

	/* set the trim angles to be used during the water-line computations */
	bool set_trim( zmsg_t* msg ) {
		while( zmsg_size( msg ) > 0 ) {
			zframe_t* frame = zmsg_pop( msg );
			if (frame == NULL) {
				m_log_cf.warnStream() << "missing protocol frame[set-trim:0]: ";
				continue;
			}
			zframe_fprint( frame, "trim: ", stdout);

			double payload;
			if(! to_double( frame, payload ) ) {
				m_log_cf.warnStream() << "bad trim value" << payload;
				continue;
			}

			zframe_destroy( &frame );
			m_ex_shell.addTrimAngle(payload);
		}
		return true;
	}

	/* set the heel angles */
	bool set_heel( zmsg_t* msg ) {
		while( zmsg_size( msg ) > 0 ) {
			zframe_t* frame = zmsg_pop( msg );
			if (frame == NULL) {
				m_log_cf.warnStream() << "missing protocol frame[set-heel:0]: ";
				continue;
			}
			zframe_fprint( frame, "heel: ", stdout);

			double payload;
			if(! to_double( frame, payload ) ) {
				m_log_cf.warnStream() << "bad heel value" << payload;
				continue;
			}
			zframe_fprint( frame, (m_protocol_task + ": ").c_str(), stdout);

			zframe_destroy( &frame );
			m_ex_shell.addHeelAngle(payload);
		}
		return true;
	}

	/* set the submersion displacements */
	bool set_displacement( zmsg_t* msg ) {
		while( zmsg_size( msg ) > 0 ) {
			zframe_t* frame = zmsg_pop( msg );
			if (frame == NULL) {
				m_log_cf.warnStream() << "missing protocol frame[set-displacement:0]: ";
				continue;
			}
			zframe_fprint( frame, "displacement: ", stdout);

			double payload;
			if(! to_double( frame, payload ) ) {
				m_log_cf.warnStream() << "bad displacement value" << payload;
				continue;
			}

			zframe_destroy( &frame );
			m_ex_shell.addDepth(payload);
		}
		return true;
	}

	/* GOAL: generate the hydrostatic information */
	bool get_hydrostatic_result( zmsg_t* msg ) {
		if (! m_ex_shell.has_wrapped_solid()) {
			m_log_cf.warnStream() << "no current wrapped model ";
			return false;
		}
		m_ex_shell.set_current_solid_to_wrapped();

		if ( zmsg_size( msg ) < 1 ) {
			return false;
		}

		double tolerance = 0.001;
		{
			zframe_t* frame = zmsg_pop( msg );
			if (frame == NULL) {
				m_log_cf.warnStream() << "missing protocol frame[" << m_protocol_task << ":tolerance]: ";
				return false;
			}
			zframe_fprint( frame, m_protocol_task.c_str(), stdout);
			if(! to_double( frame, tolerance ) ) {
				m_log_cf.warnStream() << "bad tolerance value" << tolerance;
				return false;
			}
			zframe_destroy( &frame );
		}

		double displaced = 100.0;
		{
			zframe_t* frame = zmsg_pop( msg );
			if (frame == NULL) {
				m_log_cf.warnStream() << "missing protocol frame[" << m_protocol_task << ":displaced]: ";
				return false;
			}
			zframe_fprint( frame, m_protocol_task.c_str(), stdout);
			if(! to_double( frame, displaced ) ) {
				m_log_cf.warnStream() << "bad displaced value" << displaced;
				return false;
			}
			zframe_destroy( &frame );
		}
		double heel = 0.0;
		{
			zframe_t* frame = zmsg_pop( msg );
			if (frame == NULL) {
				m_log_cf.warnStream() << "missing protocol frame[" << m_protocol_task << ":heel]: ";
				return false;
			}
			zframe_fprint( frame, m_protocol_task.c_str(), stdout);
			if(! to_double( frame, heel ) ) {
				m_log_cf.warnStream() << "bad heel value" << heel;
				return false;
			}
			zframe_destroy( &frame );
		}
		double trim = 0.0;
		{
			zframe_t* frame = zmsg_pop( msg );
			if (frame == NULL) {
				m_log_cf.warnStream() << "missing protocol frame[" << m_protocol_task << ":trim]: ";
				return false;
			}
			zframe_fprint( frame, m_protocol_task.c_str(), stdout);
			if(! to_double( frame, trim ) ) {
				m_log_cf.warnStream() << "bad trim value" << trim;
				return false;
			}
			zframe_destroy( &frame );
		}
		m_log_cf.infoStream() << m_protocol_task << ": " << log4cpp::eol
			<< " tolerance = " << tolerance << log4cpp::eol
			<< " displaced = " << displaced << log4cpp::eol
			<< " heel = " << heel << log4cpp::eol
			<< " trim = " << trim;
		isis::hydrostatic::Result result;
		m_ex_shell.computeHydrostatic(result, true, tolerance, displaced, heel, trim);

		m_log_cf.infoStream() << "hydrostatic result " << log4cpp::eol << result;
		
		return true;
	}

	/* using the hydrostatic object interpolate a single center of hydrostatic and depth */
	bool get_hydrostatic_space( zmsg_t* msg ) {
		if (! m_ex_shell.has_wrapped_solid()) {
			m_log_cf.warnStream() << "no current wrapped model ";
			return false;
		}
		m_ex_shell.set_current_solid_to_wrapped();

		isis::hydrostatic::PolatedSpace::ptr result_space;
		m_ex_shell.computeHydrostatic(result_space);
		m_log_cf.infoStream() << m_protocol_task << ": " << '/n' << result_space;

		if (m_hydrostatic == NULL) {
			m_log_cf.warnStream() << "hydrostaticshell value not set";
			return false;
		}
		return true;
	}

	/* compute a single center of hydrostatic */
	bool compute_displacement( zmsg_t* msg ) {
		if (m_hydrostatic == NULL) {
			m_log_cf.warnStream() << "hydrostaticshell value not set";
			return false;
		}
		return true;
	}

	bool extract_joint( zmsg_t* msg ) { 
		ProError rc;
		ProSelection* selection_array;
		int num_selected;
		switch( rc = ProSelect((char *)"feature", 1, NULL, NULL, NULL, NULL, 
			&selection_array, &num_selected) ) {
			case PRO_TK_NO_ERROR: break;
			default:
			return false; 
		}
		for( int ix=0; ix < num_selected; ++ix ) {
			ProFeature datum;
			switch( rc = ProSelectionModelitemGet(selection_array[ix], &datum) ) {
				case PRO_TK_NO_ERROR: break;
				default:
				continue;
			}
			isis::cad::Joint joint = isis::cad::creo::convert_datum(datum);
			m_log_cf.warnStream() 
				<< "selected joint : " << joint.toString() ;
		} 
		m_log_cf.warnStream() 
			<< "joint selection complete";
		return true;
	}

	/* infer a joint from a set of constraints */
	bool infer_joints( zmsg_t* msg ) {
		ic::Joint inferred;
		while( zmsg_size( msg ) > 0 ) {
			bpt::ptree pt;
			{
				zframe_t* frame = zmsg_pop( msg );
				if (frame == NULL) {
					m_log_cf.warnStream() << "missing protocol frame[" << m_protocol_task << ":tolerance]: ";
					return false;
				}
				// zframe_fprint( frame, m_protocol_task.c_str(), stdout);
				char* payload = zframe_as_cstr(frame);
				std::stringstream payload_ss;
				payload_ss << payload;
				bpt::read_json(payload_ss, pt);
				zframe_destroy( &frame );
			}

			typedef std::pair<std::string, bpt::ptree> kv_t;
			e3ga::vector locate;
			{
				double arr[] = {0.0, 0.0, 0.0};
				int ix = -1;
				for each( const kv_t node in pt.get_child("locate") ) {
					++ix;
					std::string coordinate_str = node.second.data();
					double coordinate =	boost::lexical_cast<double>(coordinate_str);
					arr[ix] = coordinate;
				}
				locate.set(e3ga::vector::coord_e1_e2_e3,
					arr[0], arr[1], arr[2] );
			}

			e3ga::vector orient;
			{
				double arr[] = {0.0, 0.0, 0.0};
				int ix = -1;
				for each( const kv_t node in pt.get_child("orient") ) {
					++ix;
					std::string coordinate_str = node.second.data();
					double coordinate =	boost::lexical_cast<double>(coordinate_str);
					arr[ix] = coordinate;
				}
				orient.set(e3ga::vector::coord_e1_e2_e3,
					arr[0], arr[1], arr[2] );
			}

			double rotate = pt.get<double>("rotate", 0.0) * M_PI * 2.0;

			std::string type_str = pt.get<std::string>("type","fff");
			ic::JointType type = ic::joint_type_map[ type_str ];

			ic::Joint joint( type, locate, orient, rotate, 0 );
			{
				std::ostringstream joint_ss;
				joint_ss << joint;
				m_log_cf.infoStream() << "input : " << joint_ss.str();
			}
			inferred = inferred.meld(joint);
			{
				std::ostringstream joint_ss;
				joint_ss << inferred;
				m_log_cf.infoStream() << "infer : " << joint_ss.str();
			}
			m_log_cf.infoStream() << log4cpp::eol;
		}
		std::ostringstream joint_ss;
		joint_ss << inferred;
		m_log_cf.infoStream() << "final : " << joint_ss.str();

		return EXIT_SUCCESS;
	}

	/**
	print out the components in the assembly
	*/
	bool disassemble( zmsg_t* msg ) {
		std::string protocol_payload;
		zframe_t* protocol_payload_frame;
		{
			if ( zmsg_size( msg ) < 1 ) {
				m_log_cf.warnStream() << "received nothing following '" << m_protocol_task << "': " 
					<< zmsg_size( msg ); 
				return false;
			}
			zframe_t* frame = zmsg_pop( msg );
			if (frame == NULL) {
				m_log_cf.warnStream() << "missing protocol frame[" << m_protocol_task << ":0]: ";
				return false;
			}
			char* payload = zframe_as_cstr(frame);
			m_log_cf.infoStream() << m_protocol_task << " payload: " << payload;
			protocol_payload = payload;
			free( payload );
			protocol_payload_frame = frame;
		}
			
		ProError rs;
		ProMdl assembly;

		switch( rs = ProMdlCurrentGet(&assembly) ) {
		case PRO_TK_NO_ERROR: break;
		default:
			m_log_cf.warnStream() << "no current model";
			return false;
		}
		::isis::creo::assembly::disassemble(static_cast<ProSolid>(assembly));

		zframe_send( &protocol_payload_frame, m_distributor, ZFRAME_REUSE );
		zframe_destroy( &protocol_payload_frame );
		return true;
	}

	/**
	denormalize all component files (no references)
	*/
	bool denormalize( zmsg_t* msg ) {
		ProError rs = PRO_TK_GENERAL_ERROR;
		/*
		ProMdl current_model;
				zframe_fprint( frame, "pro-task:", stdout );

		switch( rs = ProMdlCurrentGet(&current_model) ) {
		case PRO_TK_NO_ERROR: break;
		default:
			m_log_cf.warnStream() << "no current model";
			return false;
		}
		ProSolid assembly = static_cast<ProSolid>(current_model); 
		::isis::creo::denormal::denormalize( assembly, assembly );
		*/
		return false;
	}
};

bool setup_logging( boost::filesystem::path file_path ) {
	std::cout << "log file path: " << file_path << std::endl;

	if (! boost::filesystem::exists( file_path ) ) {
		boost::filesystem::remove_all( file_path );
	}
	boost::filesystem::create_directories( file_path.parent_path() );

	// Appender for logging to the console
	log4cpp::Appender *consoleappender1 = new log4cpp::OstreamAppender("console", &std::cout);
	consoleappender1->setLayout(new log4cpp::PatternLayout());

	// Appender for logging to the console
	log4cpp::Appender *consoleappender2 = new log4cpp::OstreamAppender("console", &std::cout);
	consoleappender2->setLayout(new log4cpp::PatternLayout());

	// Appender for logging to the log file
	log4cpp::Appender *fileappender1 = new log4cpp::FileAppender("logfile", file_path.string());
	fileappender1->setLayout(new log4cpp::PatternLayout());

	// Appender for logging to the log file
	log4cpp::Appender *fileappender2 = new log4cpp::FileAppender("logfile", file_path.string());
	fileappender2->setLayout(new log4cpp::PatternLayout());

	// Category for logging only to the log file
	log4cpp::Category& logcat_fileonly = log4cpp::Category::getInstance(LOGCAT_LOGFILEONLY);
	logcat_fileonly.setPriority(log4cpp::Priority::INFO);
	logcat_fileonly.addAppender(fileappender1);

	// Category for logging both to the log file and to the console
	log4cpp::Category& logcat_consoleandfile = log4cpp::Category::getInstance(LOGCAT_CONSOLEANDLOGFILE);
	logcat_consoleandfile.setPriority(log4cpp::Priority::INFO);
	logcat_consoleandfile.addAppender(consoleappender1);
	logcat_consoleandfile.addAppender(fileappender2);

	// Category for logging both to the log file and to the console
	log4cpp::Category& logcat_consoleonly = log4cpp::Category::getInstance(LOGCAT_CONSOLEONLY);
	logcat_consoleonly.setPriority(log4cpp::Priority::INFO);
	logcat_consoleonly.addAppender(consoleappender2);

	return true;
}

/**
The zmq should really wait until the creo connection is established.
*/
int main(int argc, char* argv[])
{
	Configuration config = parse_input(argc, argv);
	boost::filesystem::path log_path = config.working_dir / "log" / "metalink.log";
	setup_logging( log_path );
	Driver driver( config );
	driver.run();
	return 0;
}
