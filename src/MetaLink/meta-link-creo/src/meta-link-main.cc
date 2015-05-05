/**
 * meta-link library test
 * Copyright (c) 2013 Fred Eisele (phreed at gmail dot com)
 *
 * Distributed under the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include <cstdlib>
#include <deque>
#include <iostream>
#include <fstream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/program_options.hpp>
#include <boost/uuid/uuid.hpp>            // uuid class
#include <boost/uuid/uuid_generators.hpp> // generators
#include <boost/uuid/uuid_io.hpp>
#include <google/protobuf/text_format.h>

#include "gen/MetaLinkMsg.pb.h"
#include "BridgeClient.h"
#include "BridgeClientST.h"

#include <Windows.h>


namespace isis {

	/** 
	This serves as a rough approximation to the real MetaLink assembler.
	*/
	class MetaLinkAssembler {
	public:
		typedef boost::shared_ptr<MetaLinkAssembler> Pointer;

		MetaLinkAssembler() {
		}
		void InitializeAssembly (	
				const std::string	&in_AssemblyModel ) 
		{
			std::cout << "initialize assembly : " << in_AssemblyModel << std::endl;
		}

		void SetEnvironment (	
				const std::string	&in_ParameterName,
				const std::list< std::string >  &in_ParameterValue )
		{
			std::cout << "set environment : " << in_ParameterName << " : " << in_ParameterValue.size() <<  std::endl;
		}

		void AddComponentToAssembly (	
				const std::string	&in_ParentAssemblyInstanceID,
				const std::string 	&in_ComponentInstanceID,
				const std::string 	&in_CreoModelName,
				const std::string	&in_CreoModelType )
		{
			std::cout << "add component to assembly : " 
				<< in_ParentAssemblyInstanceID << " : " 
				<< in_ComponentInstanceID << " , "
				<< in_CreoModelName << " , " 
				<< in_CreoModelType <<  std::endl;
		}

		// Needs Contract
		void ConstrainComponent (const std::vector< std::string >  &in_ConstraintPairs ) {
		}

		// Needs Contract
		void ModifyParameter (  const std::string  &in_ComponentInstanceID,
								const std::vector<std::string> &in_Parameters) {
		}	
	};

	/**
	 * A sample implementation of the MetaLink Event Handler
	 * The real implementation can be found in...
	 *  <META_BRANCH>\src\CADAssembler\CADCreoParametricCreateAssembly\MetaLinkHandler.cpp
	 */
	class MetaLinkHandler {
	public:
		typedef boost::shared_ptr<MetaLinkHandler> Pointer;

		MetaLinkHandler(MetaLinkAssembler::Pointer assembler_ptr) 
		 : m_operator("<meta link handler>"), m_sequence(1), m_assembler(assembler_ptr) 
		{}

		boost::atomic<int> m_sequence;

		void setOperator(std::string operate) {
			m_operator = operate;
		}

		bool interest(BridgeClient* client) {
			isis::EditPointer editPtr(new meta::Edit());
			editPtr->add_mode(meta::Edit_EditMode_INTEREST);

			boost::uuids::uuid guid = boost::uuids::random_generator()();
			editPtr->set_guid(boost::uuids::to_string(guid));

			editPtr->add_topic("edu.vanderbilt.isis.metalink.manifest.component");

			editPtr->add_origin(m_operator);
			editPtr->set_sequence(m_sequence++);

			std::cout << "posting interest" << std::endl;
			client->send(editPtr);
			return true;
		}

		/**
		 * Process an edit.
		 * Remember an edit may have many raw payload elements.
		 */
		bool process(EditPointer edit_ptr) {
			std::cout << "Process" << std::endl;
			pb::RepeatedPtrField< meta::Action > prl = edit_ptr->actions();

			for (int kx=0; kx < prl.size(); ++kx) {
				meta::Action action = prl.Get(kx);

				std::cout << "Action:" << action.actionmode() << std::endl;
				for (int i = 0; i < action.manifest_size(); i++)
				{
					action.manifest(i).nodemode();
					action.manifest(i).name();
					action.manifest(i).guid();
					action.manifest(i).cyphyparentid();
				}

				for (int ix=0; ix < action.environment_size(); ++ix) {
					{
						meta::Environment env = action.environment(ix);
						pb::RepeatedPtrField<std::string> values = env.value();
						std::list<std::string> value_list(values.size());
						for (int jx=0; jx < values.size(); ++jx) {
							const std::string value = values.Get(jx);
							value_list.push_back(value);
						}
						m_assembler->SetEnvironment(env.name(), value_list);
						break;
					}
				}

				if (action.has_payload()) {
					meta::Payload payload = action.payload();
					for (int ix=0; ix < payload.components_size(); ++ix) {
						meta::CADComponentType component = payload.components(ix);
						upsertComponent(edit_ptr, component);
					}
					for (int ix=0; ix < payload.constraints_size(); ++ix) {
						meta::ConstraintType constraint = payload.constraints(ix);
						upsertConstraint(edit_ptr, constraint);
					}
					for (int ix=0; ix < payload.parameters_size(); ++ix) {
						meta::ParametricParametersType parameter = payload.parameters(ix);
						upsertParameter(edit_ptr, parameter);
					}
				}

				if (action.has_alien()) {
					meta::Alien alien = action.alien();
					std::string alienEncoded = alien.encoded();
					switch (alien.encodingmode()) {
					case meta::Alien_EncodingMode_JSON:
						{
							std::cout << "alien json [" << kx << "]\n" << alienEncoded << std::endl;
							break;
						}
					case meta::Alien_EncodingMode_XML:
						{
							m_assembler->InitializeAssembly( alienEncoded );
							break;
						}
					default:
						{
							std::cout << "alien " << alien.encodingmode() << " [" << kx << "]\n" << alienEncoded << std::endl;
						}
					}
				}
			}
			return true;
		}

		typedef pb::RepeatedPtrField< meta::CADComponentType > ComponentRepeated;
		typedef ComponentRepeated::iterator ComponentIterator;

	private:

		void upsertConstraint(EditPointer edit, meta::ConstraintType& component) {
			std::cout << "upsertComponent" << std::endl;
		}

		void upsertParameter(EditPointer edit, meta::ParametricParametersType parameter) {
			std::cout << "upsertComponent" << std::endl;
		}

		void upsertComponent(EditPointer edit, meta::CADComponentType& component) {
				std::cout << "upsertComponent" << std::endl;
				const std::string in_AssemblyInstanceId = edit->topic(1);
				const std::string in_ComponentInstanceID = component.componentid();
				const std::string in_CreoModelName = component.name();
				const std::string in_CreoModelType = component.type();
		    
				m_assembler->AddComponentToAssembly (
					in_AssemblyInstanceId,
					in_ComponentInstanceID,
					in_CreoModelName,
					in_CreoModelType );
		}
	
		MetaLinkAssembler::Pointer m_assembler;
		std::string m_operator;
	};

	
} // isis namespace


enum Mode {LOOPER, SYNCHRONOUS};

std::istream& operator>>(std::istream& in, Mode& mode)
{
    std::string token;
    in >> token;
    if (token == "looper")
        mode = LOOPER;
    else if (token == "synchronous")
        mode = SYNCHRONOUS;
	else if (token == "single")
        mode = SYNCHRONOUS;
    else 
		mode = LOOPER;
    return in;
}
/**
	* Read lines from standard input and write them to the socket on the main thread.
	* Create a secondary thread (task) to receive messages from the server.
	*
	*/
int main(int argc, char* argv[]) {
	namespace po = boost::program_options;

	try {
		// Declare the supported options.
		po::options_description odesc("Allowed options");
		std::string host;
		std::string service;
		Mode mode = SYNCHRONOUS;

		// @formatter off
		odesc.add_options()
				("help", "produce help message")
				("host", po::value<std::string>(&host)->default_value("localhost"), "set host name (or ip addr)")
				("service", po::value<std::string>(&service)->default_value("15150"), "set port number or service name")
				("initfile", po::value<std::string>(), "initial load")
				("mode", po::value<Mode>(&mode), "which mode [looper synchronous]")
				("stdin", "initial load via standard input (only processed if initfile not present)")
				;

		 po::options_description options("Program options");

		// @formater on

		po::variables_map varmap;
		po::parsed_options parsedOptions = po::parse_command_line(argc, argv, odesc);
		po::store(parsedOptions, varmap);
		po::notify(varmap);

		if (varmap.count("help")) {
			std::cout << odesc << std::endl;
			return 1;
		}

		std::istream *instream;
		if (varmap.count("initfile")) {
			std::string file = varmap["file"].as<std::string>();
			std::cout << "initialization file name was set to " << varmap["initfile"].as<std::string>()
					<< "." << std::endl;
			std::ifstream ifs;
			ifs.open(file.c_str(), std::ios::in | std::ios::binary);
			instream = &ifs;
		} if (varmap.count("stdin")) {
			std::cout << "standard input being for initialization" << std::endl;
			instream = &std::cin;
		} else {
			std::cout << "no initialization stream provided" << std::endl;
			instream = NULL;
		}


		switch (mode) {

		// Test the single-threaded part
		case SYNCHRONOUS:		
		{
			isis::BridgeClientST client(host, service);
			client.connect();



			isis::EditPointer edit(new meta::Edit());
			edit->add_mode(meta::Edit_EditMode_INTEREST);

			boost::uuids::uuid guid = boost::uuids::random_generator()();
			edit->set_guid(boost::uuids::to_string(guid));

			edit->add_topic("ISIS.METALINK.COMPONENT.MANIFEST");

			edit->add_origin("CreoAddComponent");
			edit->set_sequence(1);

			client.send(edit);

			while (true){
				try{
					isis::EditPointer result = client.receive(2000);

					pb::RepeatedPtrField<meta::Action> prl = result->actions();

					for (int kx=0; kx < prl.size(); ++kx) {
						meta::Action action = prl.Get(kx);
						std::cout << "Received: " << action.actionmode() << std::endl;
					}
				} catch (isis::BridgeClientST::TimeoutException ex){
					std::cout << "Receive timed out" << ex << std::endl;
				}
			}
			break;
		}
		case LOOPER:
		default:
			{
			std::cout << "running looper mode" << std::endl;
			/**
			The following example code should be used in any
			use of the MetaLink where blocking is unacceptable.
			(And blocking is generally unacceptable.)
			*/
			boost::asio::io_service ios;
		
			isis::MetaLinkAssembler::Pointer assembler_ptr( new isis::MetaLinkAssembler() );
			isis::MetaLinkHandler::Pointer 
				handler_ptr( new isis::MetaLinkHandler( assembler_ptr) );
			handler_ptr->setOperator("meta link c++ test");

			isis::BridgeClient client(ios, host, service,
				boost::bind(&isis::MetaLinkHandler::interest, handler_ptr, _1),
				boost::bind(&isis::MetaLinkHandler::process, handler_ptr, _1));
			std::cout << "connection requested" << std::endl;

			ios.run();
			break;
			}
		}


	} catch (std::exception& ex) {
		std::cerr << "Exception: " << ex.what() << "\n";
	}

	return 0;
}
