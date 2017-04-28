#include "stdafx.h"
#include <ProSelbuffer.h>
#include "MetaLinkHandler.h"
#include "ParametricParameters.h"
#include "google/protobuf/text_format.h"
#include "CreoPlugins\CreoPluginFunctions.h"
#include "GlobalModelData.h"


namespace meta = edu::vanderbilt::isis::meta;
namespace pb = google::protobuf;

namespace isis
{


	const std::string CAD_OPERATOR("CAD");

#pragma warning(disable: 4355)

	MetaLinkHandler::MetaLinkHandler(isis::MetaLinkAssemblyEditor::Pointer assembler_ptr,
		boost::asio::io_service& io_service,
		std::string host,
		std::string service,
		const std::string &in_InstanceID,
		const std::string &in_TargetID,
		boost::mutex &in_events_mutex):
		//m_logcat(::log4cpp::Category::getInstance(LOGCAT_LOGFILEONLY)),
		m_operator(CAD_OPERATOR), m_sequence(1), m_ready(false),
		m_assembler(assembler_ptr), m_target_id(in_TargetID),
		m_client(io_service, host, service,
		boost::bind(&isis::MetaLinkHandler::interest, this, _1, in_InstanceID, in_TargetID),
		boost::bind(&isis::MetaLinkHandler::process, this, _1)),
		m_network_thread(boost::bind(&boost::asio::io_service::run, &io_service)),
		m_ioservice(io_service),
		m_events_mutex(in_events_mutex)
	{
	}

	void MetaLinkHandler::interrupt()
	{
		m_client.disconnect();
		m_ioservice.stop();
		m_network_thread.interrupt();
	}

	void MetaLinkHandler::disconnect()
	{
		m_network_thread.join();
	}

	/**
	Send the selected component instance information out via MetaLink.
	*/
	bool MetaLinkHandler::send_LocateSelectedRequest(const std::string &in_InstanceID , const std::string &in_TargetID)
	{
		std::vector<isis::CADCreateAssemblyError> errorList;
		std::string component_guid = m_assembler->LocateComponentOfAssembly(errorList);
		if(component_guid.empty())
		{
			isis_LOG(lg, isis_FILE, isis_WARN) << "MetaLinkHandler::send_LocateSelectedRequest(): empty component guid";
			return false;
		}
		isis_LOG(lg, isis_FILE, isis_DEBUG) << "MetaLinkHandler::send_LocateSelectedRequest(): component guid: " << component_guid;

		isis::EditPointer editPtr(new meta::Edit());
		editPtr->add_mode(meta::Edit_EditMode_POST);

		boost::uuids::uuid guid = boost::uuids::random_generator()();
		editPtr->set_guid(boost::uuids::to_string(guid));

		meta::Action *action = editPtr->add_actions();
		action->set_subjectid(in_TargetID);
		action->set_actionmode(meta::Action_ActionMode_SELECT_CYPHY_COMPONENT);
		meta::Payload* payload = action->mutable_payload();
		editPtr->add_topic(in_InstanceID);
		
		switch(GlobalModelData::Instance.mode)
		{
		case COMPONENTEDIT:
			{
				meta::DatumType* datum = payload->add_datums();
				datum->set_type("SURFACE");
				datum->set_name("DTM1");
			}
			break;
		case DESIGNEDIT:
			{
				action->set_actionmode(meta::Action_ActionMode_SELECT_CYPHY_COMPONENT);
				meta::CADComponentType* comp = payload->add_components();
				comp->set_componentid(component_guid);
			}
			break;
		case UNDEFINEDMODE:
			return true;

		}	
			
		editPtr->add_origin(m_operator);

		isis_LOG(lg, isis_FILE, isis_DEBUG) << "MetaLinkHandler::send_LocateSelectedRequest(): posting locate request";
		m_client.send(editPtr);
		return true;
	}

	/**
	Express an interest in certain types of information by topic name.
	*/
	bool  MetaLinkHandler::interest(isis::BridgeClient* client, const std::string &in_InstanceID, const std::string &in_TargetID)
	{
		isis_LOG(lg, isis_FILE, isis_INFO) << "MetaLinkHandler::interest(): " << in_InstanceID << isis_EOL;

		isis::EditPointer editPtr(new meta::Edit());
		editPtr->add_mode(meta::Edit_EditMode_INTEREST);
		editPtr->set_editmode(meta::Edit_EditMode_INTEREST);

		boost::uuids::uuid guid = boost::uuids::random_generator()();
		editPtr->set_guid(boost::uuids::to_string(guid));
		editPtr->add_topic(in_InstanceID);

		editPtr->add_origin(m_operator);

		// 8/9/2013 try leaving out zzzz editPtr->set_sequence(m_sequence++);  sequence number should't be needed for an interest

		isis_LOG(lg, isis_FILE, isis_DEBUG) << "MetaLinkHandler::interest(): posting interest";
		client->send(editPtr);

		m_ready = true;
		return true;
	}

	/**
	The work of handling the events in a Creo specific way happens here.
	This is the method which extracts items from the message queue.
	The lock need only happen here.
	*/
	bool MetaLinkHandler::process(isis::EditPointer edit)
	{
		boost::unique_lock< boost::mutex > guard(m_events_mutex);
		meta::Edit* newEdit = new meta::Edit(*edit);
		m_eventQueue.push(EditPointer(newEdit));
		return false;
	}

	bool MetaLinkHandler::processEdit(isis::EditPointer edit)
	{
		isis_LOG(lg, isis_FILE, isis_DEBUG) << "MetaLinkHandler::processEdit()";
		std::string baseTopic = edit->topic(0);
		meta::Edit_EditMode mode = edit->mode(edit->mode_size()-1);

		switch(mode)
		{
		case meta::Edit_EditMode_POST:
			return process_AssemblyDesignPost(edit);
				// a passive post must specify the "origin" correctly.
//				return process_PassivePost(edit);
			isis_LOG(lg, isis_FILE, isis_WARN) << "MetaLinkHandler::processEdit(): unknown topic " << baseTopic;
			break;

		case meta::Edit_EditMode_DISINTEREST:
			isis_LOG(lg, isis_FILE, isis_WARN) << "MetaLinkHandler::processEdit(): ignoring DISINTEREST";
			break;

		case meta::Edit_EditMode_INTEREST:
			isis_LOG(lg, isis_FILE, isis_WARN) << "MetaLinkHandler::processEdit(): ignoring INTEREST";
			break;

		case meta::Edit_EditMode_NOTICE:
			isis_LOG(lg, isis_FILE, isis_WARN) << "MetaLinkHandler::processEdit(): ignoring NOTICE:" << edit->notices(0).msg();
			break;

		default:
			isis_LOG(lg, isis_FILE, isis_WARN) << "MetaLinkHandler::processEdit(): ignoring " << mode;
		}

		return false;
	}

	/**
	The passive topic "ISIS.METALINK.CAD.PASSIVE".

	This topic indicates that the CADAssembler is waiting to be given some work to perform.
	The protobuf schema to carries arguments, "mode" and an "identifer".
	It will also use an "action" -> START as well.
	The CADAssembler responds by posting an INTEREST for
	the STARTed topic and a DISINTEREST in "ISIS.METALINK.CAD.PASSIVE".
	There is also a STOP action for the other modes.
	The STOP action drops the current work product and
	expressing DISINTEREST in :
	1) ISIS.METALINK.CADASSEMBLY + <identifier>, or
	2) ISIS.METALINK.COMPONENT.UPDATE  + <component id>) and
	express INTEREST in "ISIS.METALINK.CAD.PASSIVE".
	*/
#if 0
	bool MetaLinkHandler::process_PassivePost(isis::EditPointer edit)
	{
		// 0 field indicates CAD passive topic
		if(edit->topic_size() < 1)
		{
			isis_LOG(lg, isis_FILE, isis_WARN) << "MetaLinkHandler::process_PassivePost(): insufficient topic for passive, topic size: " << edit->topic_size();
			return false;
		}
		// std::string passiveTopic = edit->topic(0);

		isis::EditPointer editPtr(new meta::Edit());
		editPtr->add_mode(meta::Edit_EditMode_INTEREST);
		editPtr->set_editmode(meta::Edit_EditMode_INTEREST);

		boost::uuids::uuid guid = boost::uuids::random_generator()();
		editPtr->set_guid(boost::uuids::to_string(guid));
		editPtr->add_origin(m_operator);

		for(int actionIx=0; actionIx < edit->actions_size(); ++actionIx)
		{
			std::string majorMode;
			meta::Action* action = edit->mutable_actions(actionIx);
			if(! action->has_actionmode())
			{
				isis_LOG(lg, isis_FILE, isis_WARN) << "MetaLinkHandler::process_PassivePost():no action mode specified";
				continue;
			}
			if(action->actionmode() != meta::Action_ActionMode_SWITCH)
			{
				isis_LOG(lg, isis_FILE, isis_WARN) << "MetaLinkHandler::process_PassivePost(): wrong action mode specified, action mode: " << action->actionmode();
				continue;
			}

			/*if(! action->has_interest())
			{
				isis_LOG(lg, isis_FILE, isis_WARN) << "MetaLinkHandler::process_PassivePost(): switch action has no interest";
				continue;
			}
			meta::Interest interest = action->interest();
			if(interest.topic_size() < 1)
			{
				isis_LOG(lg, isis_FILE, isis_WARN) << "MetaLinkHandler::process_PassivePost(): switch action has no topic";
				continue;
			}
			if(interest.uid_size() < 1)
			{
				isis_LOG(lg, isis_FILE, isis_WARN) << "MetaLinkHandler::process_PassivePost(): switch action has no identifier";
				continue;
			}

			for(int topix=0; topix < interest.topic_size(); ++topix)
			{
				editPtr->add_topic(interest.topic(topix));
			}

			editPtr->add_topic(interest.uid(0));

			isis_LOG(lg, isis_FILE, isis_DEBUG) << "MetaLinkHandler::process_PassivePost(): posting interest";
			m_client.send(editPtr);*/
		}
		return true;
	}
#endif

		bool MetaLinkHandler::process_AvmComponentPost_select(isis::EditPointer in_Edit, int in_ActionIx, meta::Action *in_Action)
		{
			isis_LOG(lg, isis_FILE, isis_DEBUG) << "MetaLinkHandler::process_AvmComponentPost_select()";
			bool exp = false;
			/**
			* Handle selecting the datums.
			*/
			if(! in_Action->has_payload())
			{
				isis_LOG(lg, isis_FILE, isis_WARN) << "missing payload on select";
				return false;
			}
			meta::Payload payload = in_Action->payload();
			std::vector<isis::CADCreateAssemblyError> out_ErrorList;
			for(int datumIx=0; datumIx < payload.datums_size(); ++datumIx)
			{
				meta::DatumType datum = payload.datums().Get(datumIx);
				std::string datumType = datum.type();
				std::string datumName = datum.name();

				try
				{
					m_assembler->SelectDatumOfComponent(
						datumType,
						datumName,
						out_ErrorList);
				}
				catch(isis::application_exception &ex)
				{
					// Don't freak out this is not the end of the world
				}
			}
			return true;
		}

		bool MetaLinkHandler::process_AvmComponentPost_insert(isis::EditPointer in_Edit, int in_ActionIx, meta::Action *in_Action)
		{
			isis_LOG(lg, isis_FILE, isis_DEBUG) << "MetaLinkHandler::process_AvmComponentPost_insert()";
			/**
			* Handle setting the environment.
			*/
			for(int jx=0; jx < in_Action->environment_size(); ++jx)
			{
				meta::Environment env = in_Action->environment(jx);

				std::list<std::string> searchPaths;
				if(isis::ConvertToUpperCase(env.name()) == "SEARCHPATH")
				{
					for(int kx=0; kx < env.value_size(); ++kx)
					{
						searchPaths.push_back(env.value(kx));
					}
					process_SearchPaths(searchPaths);
					if(env.value_size() > 0)
					{
						isis::MultiFormatString newDir(env.value(0));
						ProError err = ProDirectoryChange(const_cast<wchar_t*>((const wchar_t*)newDir));
						if(err != PRO_TK_NO_ERROR)
						{
							isis_LOG(lg, isis_FILE, isis_WARN) << "Unable to set new working directory to " << newDir;
						}
					}
				}
			}

			/**
			* Handle alien/foreign encodings
			*/
			if(in_Action->has_alien())
			{
				meta::Alien alien = in_Action->alien();
				const std::string payloadBytes = alien.encoded();

				switch(alien.encodingmode())
				{
				case  meta::Alien_EncodingMode_XML:
					m_assembler->UpdateAvmComponentViaXML(payloadBytes);
					break;
				case  meta::Alien_EncodingMode_JSON:
					isis_LOG(lg, isis_FILE, isis_WARN) << "no JSON messages recognized for topic ";
					break;

				default:
					/** Print the edit */
					std::string display;
					pb::TextFormat::PrintToString(alien, &display);
					isis_LOG(lg, isis_FILE, isis_INFO) << "alien payload [" << in_ActionIx << "]\n" << display;
				}
			}

			/**
			* Handle setting the payload.
			*/
			if(in_Action->has_payload())
			{
				meta::Payload payload = in_Action->payload();
				for(int jx=0; jx < payload.components_size(); ++jx)
				{
					meta::CADComponentType component = payload.components(jx);
					meta::Notice notice = process_AvmComponentInitialize(in_Action->subjectid(), component);
					*(in_Action->add_notices()) = notice;
				}
			}
			else
			{
				isis_LOG(lg, isis_FILE, isis_DEBUG)<< "MetaLinkHandler::process_AvmComponentPost_insert(): No payload, assuming component without existing CAD part.";
				GlobalModelData::Instance.ComponentEdit.avmId = in_Action->subjectid();
				isis::GlobalModelData::Instance.mode = isis::COMPONENTEDIT;
			}
			return true;
		}

		/**
		Process a POST edit message, probably from CyPhy, which indicates
		a change to the design assembly indicated by the TargetID specified when the program was started.
		*/
		bool MetaLinkHandler::process_AssemblyDesignPost(isis::EditPointer edit)
		{
			isis_LOG(lg, isis_FILE, isis_DEBUG) << "MetaLinkHandler::process_AssemblyDesignPost()";

			/**
			* The origin and sequence together provide data for NACK
			*/
			edit->origin();
			edit->sequence();

			/**
			* Process the notices.
			*/
			pb::RepeatedPtrField< meta::Notice > notes = edit->notices();


			/*
			* Error message
			*/
			edit->add_mode(meta::Edit_EditMode_NOTICE);
			edit->set_editmode(meta::Edit_EditMode_NOTICE);
			edit->add_origin(m_operator);

			/*
			Perform any preparation based on the presence of certain actions.
			This is where the default behavior of clearing all highlights happens.
			*/
			for(int actionIx=0; actionIx < edit->actions_size(); ++actionIx)
			{
				meta::Action* action = edit->mutable_actions(actionIx);
				switch(action->actionmode())
				{
				case meta::Action_ActionMode_SELECT:
					try
					{
						m_assembler->ClearSelection();
					}
					catch(isis::application_exception &ex)
					{
						// Ignore this, there's a log message
					}
					break;
				}
			}

			bool error = false; // if exp == true, then send exception notification message to Cyphy.
			for(int actionIx=0; actionIx < edit->actions_size(); ++actionIx)
			{
				meta::Action* action = edit->mutable_actions(actionIx);
				switch(action->actionmode())
				{
				case meta::Action_ActionMode_REQUEST_COMPONENT_LIST:
					error = processResponse_ComponentList(edit);
					break;
				case meta::Action_ActionMode_SELECT:
					if (GlobalModelData::Instance.mode == COMPONENTEDIT)
						error = process_AvmComponentPost_select(edit, actionIx, action);
					else
						error = process_AssemblyDesignPost_select(edit, actionIx, action);
					break;
				case meta::Action_ActionMode_CREATE_CYPHY_DESIGN:
					error = process_AssemblyDesignPost_insert(edit, actionIx, action);
					break;
				case meta::Action_ActionMode_CLEAR:
					if (GlobalModelData::Instance.mode == COMPONENTEDIT)
						error = process_AvmComponentPost_clear(edit, actionIx, action);
					else
						error = process_AssemblyDesignPost_clear(edit, actionIx, action);
					break;
				case meta::Action_ActionMode_UPDATE_CYPHY_DESIGN:
					error = process_AssemblyDesignPost_update(edit, actionIx, action);
					break;
				case meta::Action_ActionMode_ADD_COMPONENT_TO_CAD_DESIGN:
					error = process_AssemblyDesignPost_insert(edit, actionIx, action);
					break;
				case meta::Action_ActionMode_CONSTRAIN_COMPONENTS_CAD:
					error = process_AssemblyDesignPost_insert(edit, actionIx, action);
					break;
				case meta::Action_ActionMode_UPDATE_CAD_COMPONENT:
					error = process_AvmComponentPost_insert(edit, actionIx, action);
					break;
				case meta::Action_ActionMode_INSERT:
					error = process_AvmComponentPost_insert(edit, actionIx, action);
					break;
				default:
					isis_LOG(lg, isis_FILE, isis_WARN) << "MetaLinkHandler::process_AvmComponentPost() no explicit process for the action type: " << action->actionmode() << " [" << __FILE__ << ": " << __LINE__ << "]";
				}

			}

			if(error)
			{
				send(edit);
			}

		
		return true;
	}

	//////////////////////////////////////////////
	/**

	editMode: POST
	topic: "ISIS.METALINK.CADASSEMBLY"
	topic: "13a4c47c-39ce-44e0-8732-b946e8821ec7"
	origin: "GME"
	sequence: 29
	actions {
	actionMode: SELECT
	payload {
	components {
	ComponentId: "{b190fe97-65ad-4597-b644-9fb99cce41bd}"
	}
	}
	}

	By the time this method is called the first part of the edit has been processed.
	This method is responsible for processing a SELECT action.
	*/
	bool MetaLinkHandler::process_AssemblyDesignPost_select(isis::EditPointer in_Edit, int in_ActionIx, meta::Action *in_Action)
	{
		isis_LOG(lg, isis_FILE, isis_DEBUG) << "MetaLinkHandler::process_AssemblyDesignPost_select()";
		/**
		* Handle selecting the datums.
		*/
		if(! in_Action->has_payload())
		{
			isis_LOG(lg, isis_FILE, isis_INFO) << "missing payload on select";
			return false;
		}
		meta::Payload payload = in_Action->payload();
		std::vector<isis::CADCreateAssemblyError> out_ErrorList;
		for(int componentIx=0; componentIx < payload.components_size(); ++componentIx)
		{
			meta::CADComponentType component = payload.components().Get(componentIx);
			std::string componentInstanceId = component.componentid();

			try
			{
				m_assembler->SelectComponentOfAssembly(
					componentInstanceId,
					out_ErrorList);
			}
			catch(isis::application_exception &ex)
			{
				isis_LOG(lg, isis_FILE, isis_WARN)
					<< "Couldn't select component: " << componentInstanceId;
			}
		}
		return true;
	}
	//////////////////////////////////////////////
	bool MetaLinkHandler::process_AssemblyDesignPost_insert(isis::EditPointer edit, int actionIx, meta::Action *action)
	{
		isis_LOG(lg, isis_FILE, isis_DEBUG) << "MetaLinkHandler::process_AssemblyDesignPost_insert()";
		bool exp = false;
		/**
		* Handle setting the environment.
		*/
		for(int envIx=0; envIx < action->environment_size(); ++envIx)
		{
			meta::Environment env = action->environment(envIx);

			std::list<std::string> searchPaths;
			if(isis::ConvertToUpperCase(env.name()) == "SEARCHPATH")
			{
				for(int kx=0; kx < env.value_size(); ++kx)
				{
					searchPaths.push_back(env.value(kx));
				}
				process_SearchPaths(searchPaths);
			}
		}

		/**
		* Handle alien/foreign encodings
		*/
		if(action->has_alien())
		{
			meta::Alien alien = action->alien();
			const std::string payloadBytes = alien.encoded();

			switch(alien.encodingmode())
			{
			case  meta::Alien_EncodingMode_XML:
				{
					meta::Notice temp = process_CreateAssembly(payloadBytes);
					exp = true;
					meta::Notice* add = action->add_notices();
					*add = temp;
					break;
				}
			case  meta::Alien_EncodingMode_JSON:
				isis_LOG(lg, isis_FILE, isis_WARN) << "no JSON messages recognized for topic ";
				break;

			default:
				/** Print the edit */
				std::string display;
				pb::TextFormat::PrintToString(alien, &display);
				isis_LOG(lg, isis_FILE, isis_WARN) << "alien payload [" << actionIx << "]\n" << display;
			}
		}

		/**
		* Handle setting the payload.
		*/
		if(action->has_payload())
		{
			meta::Payload payload = action->payload();
			for(int jx=0; jx < payload.components_size(); ++jx)
			{
				meta::CADComponentType component = payload.components(jx);
				meta::Notice temp = process_ComponentAdded(component);
				if(temp.code().compare("C00000") != 0)
				{
					exp = true;
					meta::Notice* add = action->add_notices();
					*add = temp;
				}
			}
			for(int jx=0; jx < payload.constraints_size(); ++jx)
			{
				meta::ConstraintType constraint = payload.constraints(jx);
				meta::Notice temp = process_ConstraintAdded(constraint);
				if(temp.code().compare("C00000") != 0)
				{
					exp = true;
					meta::Notice* add = action->add_notices();
					*add = temp;
				}
			}
			for(int jx=0; jx < payload.parameters_size(); ++jx)
			{
				meta::ParametricParametersType parameter = payload.parameters(jx);
				meta::Notice temp = process_ParametersModified(parameter);
				if(temp.code().compare("C00000") != 0)
				{
					exp = true;
					meta::Notice* add = action->add_notices();
					*add = temp;
				}
			}
		}
		return exp;
	}

	//////////////////////////////////////////////
	// This currently only handles name change
	bool MetaLinkHandler::process_AssemblyDesignPost_update(isis::EditPointer edit,  int actionIx, meta::Action *action)
	{
		isis_LOG(lg, isis_FILE, isis_DEBUG) << "MetaLinkHandler::process_AssemblyDesignPost_update()";
		bool exp = false;
		try
		{
			/**
			* Handle setting the payload.
			*/
			if(action->has_payload())
			{
				meta::Payload payload = action->payload();
				for(int jx=0; jx < payload.components_size(); ++jx)
				{
					meta::CADComponentType component = payload.components(jx);
					meta::Notice temp = process_ComponentModified(component);
					if(temp.code().compare("C00000") != 0)
					{
						exp = true;
						meta::Notice* add = action->add_notices();
						*add = temp;
					}
				}
			}
		}
		catch(isis::application_exception& ex)
		{
			isis_LOG(lg, isis_FILE, isis_ERROR) << "ERROR, Function: MetaLinkHandler::process_AssemblyDesignPost_update,  application_exception: " << ex;
		}
		return false;
	}

	//////////////////////////////////////////////
	bool MetaLinkHandler::process_AssemblyDesignPost_clear(isis::EditPointer edit,  int actionIx, meta::Action *action)
	{
		isis_LOG(lg, isis_FILE, isis_DEBUG) << "MetaLinkHandler::process_AssemblyDesignPost_clear()";
		bool exp = false;
		try
		{
			return m_assembler->Clear();
		}
		catch(isis::application_exception& ex)
		{
			isis_LOG(lg, isis_FILE, isis_ERROR) << "ERROR, Function: MetaLinkHandler::process_AssemblyDesignPost_clear,  application_exception: " << ex;
		}
		return false;
	}

	//////////////////////////////////////////////
	bool MetaLinkHandler::process_AvmComponentPost_clear(isis::EditPointer in_Edit, int in_ActionIx, meta::Action *in_Action)
	{
		bool exp = false;
		try
		{
			return m_assembler->Clear();
		}
		catch(isis::application_exception& ex)
		{
			isis_LOG(lg, isis_FILE, isis_ERROR) << "ERROR, Function: MetaLinkHandler::process_AssemblyDesignPost_clear,  application_exception: " << ex;
		}
		return false;
	}


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	meta::Notice MetaLinkHandler::process_ComponentModified(meta::CADComponentType &in_component) throw(isis::application_exception)
	{
		isis_LOG(lg, isis_FILE, isis_DEBUG) << "MetaLinkHandler::process_ComponentModified()";

		try
		{
			m_assembler->UpdateComponentName(in_component.componentid(), in_component.displayname());
		}
		catch(isis::application_exception &ex)
		{
			meta::Notice notice;
			notice.set_noticemode(meta::Notice_NoticeMode_FAULT);
			notice.set_msg(ex.what());
			notice.set_code("C00000");
			return notice;
		}

		meta::Notice notice;
		notice.set_noticemode(meta::Notice_NoticeMode_DONE);
		notice.set_msg("No exception.");
		notice.set_code("C00000");
		return notice;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	meta::Notice MetaLinkHandler::process_ComponentAdded(meta::CADComponentType &in_component) throw(isis::application_exception)
	{
		isis_LOG(lg, isis_FILE, isis_INFO) << "MetaLinkHandler::process_ComponentAdded()";
		isis_LOG(lg, isis_FILE, isis_INFO) << "**************** Begin Recieved Message Add Component **********************";
		isis_LOG(lg, isis_FILE, isis_INFO) << "************ Begin Message Content *********************";

		isis_LOG(lg, isis_FILE, isis_INFO) << "Received a component added message, component name:  " << in_component.name();

		try
		{
			isis_LOG(lg, isis_FILE, isis_WARN)	<< isis_EOL << "MetaLinkHandler::process_ComponentAdded. Received message:"
				<< isis_EOL << "component.componentid():                "	<<	in_component.componentid()
				<< isis_EOL << "component.name():                       "	<<  in_component.name()
				<< isis_EOL << "component.type():                       "  <<  in_component.type()
				<< isis_EOL << "component.materialid()                  "  <<  in_component.materialid()
				<< isis_EOL << "component.specialinstruction()          "  <<  in_component.specialinstruction();

			const std::string					creoModelName =			in_component.name();
			const std::string					componentInstanceID =	in_component.componentid();
			ProMdlType							creoModelType =			isis::ProMdlType_enum(in_component.type());
			const std::string					materialID =			in_component.materialid();
			isis::e_CADSpecialInstruction		specialInstruction =	isis::SpecialInstruction_enum(in_component.specialinstruction());

			if(componentInstanceID.size() == 0)
			{
				// A message without a componentInstanceID should be ignored.
				isis_LOG(lg, isis_FILE, isis_WARN)
					<< "MetaLinkHandler::process_ComponentAdded. Received a message with a null componentid. Ignoring the message.";
				meta::Notice notice;
				notice.set_noticemode(meta::Notice_NoticeMode_DONE);
				notice.set_msg("No exception.");
				notice.set_code("C00000");
				return notice;
			}


			std::vector<isis::CADParameter> cADParameters;

			::google::protobuf::RepeatedPtrField< ::edu::vanderbilt::isis::meta::CADParameterType > parameters;

			if(in_component.has_parametricparameters())
			{
				parameters = in_component.parametricparameters().cadparameter();

				std::vector<isis::CADParameter>	cADParameters(parameters.size()) ;

				for(int jx=0; jx < parameters.size(); ++jx)
				{
					meta::CADParameterType parameter = parameters.Get(jx);
					isis::CADParameter cADParameter;

					isis_LOG(lg, isis_FILE, isis_WARN) << "Parameter: ";
					isis_LOG(lg, isis_FILE, isis_WARN) << "   parameter.name():       " << parameter.name();
					isis_LOG(lg, isis_FILE, isis_WARN) << "   parameter.type():       " << parameter.type();
					isis_LOG(lg, isis_FILE, isis_WARN) << "   parameter.value():      " << parameter.value();

					cADParameter.name =		parameter.name();
					cADParameter.type =		isis::CADParameterType_enum(parameter.type());
					cADParameter.value =	parameter.value();
					cADParameters.push_back(cADParameter);
				}
			}
			else
			{
				isis_LOG(lg, isis_FILE, isis_INFO) << "No parameters";
			}

			// Add connectors belonging to this component
			std::vector<isis::CADComponentConnector> connectorlist;
			for(int i = 0; i < in_component.connectors().size(); i++)
			{
				isis::CADComponentConnector connector;
				connector.id = in_component.connectors(i).id();
				connector.displayName = in_component.connectors(i).displayname();
				connector.parentid = componentInstanceID;
				connectorlist.push_back(connector);
			}

			isis_LOG(lg, isis_FILE, isis_WARN) << "************** End Message Content *********************";
			isis_LOG(lg, isis_FILE, isis_WARN) << "****************** End Recieved Message Add Component **********************";
			std::vector<isis::CADCreateAssemblyError> errorList;
			m_assembler->AddComponentToAssembly(
				componentInstanceID,
				creoModelName,
				creoModelType,
				materialID,
				in_component.displayname(),
				specialInstruction,
				cADParameters,
				errorList,
				connectorlist
				);

		} // END try
		catch(isis::application_exception& ex)
		{
			ex.setComponentInfo(in_component.name());
			isis_LOG(lg, isis_FILE, isis_ERROR) << "ERROR, Function: MetaLinkHandler::process_ComponentAdded, application exception : " << ex;
			meta::Notice notice;
			notice.set_noticemode(meta::Notice_NoticeMode_FAULT);
			if(stricmp(ex.get_error_code(), "C00000") != 0)
			{
				notice.set_msg(ex.tostring());
				notice.set_code(ex.get_error_code());
			}
			else
			{
				notice.set_msg(ex.tostring());
				notice.set_code("C00003");
			}
			return notice;
		}
		catch(std::exception& ex)
		{
			isis_LOG(lg, isis_FILE, isis_ERROR) << "ERROR, Function: MetaLinkHandler::process_ComponentAdded, std::exception : " << ex.what();
			meta::Notice notice;
			notice.set_noticemode(meta::Notice_NoticeMode_FAULT);
			notice.set_msg(ex.what());
			notice.set_code("C00001");
			return notice;

		}
		catch(...)
		{
			isis_LOG(lg, isis_FILE, isis_ERROR) << "ERROR: Function: MetaLinkHandler::process_ComponentAdded, caught exception (...).  Please report the error to the help desk.";
			meta::Notice notice;
			notice.set_noticemode(meta::Notice_NoticeMode_FAULT);
			notice.set_msg("Unknown exception.");
			notice.set_code("C00002");
			return notice;
		}

		meta::Notice notice;
		notice.set_noticemode(meta::Notice_NoticeMode_DONE);
		notice.set_msg("No exception.");
		notice.set_code("C00000");
		return notice;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	meta::Notice MetaLinkHandler::process_ConstraintAdded(meta::ConstraintType  &in_constraints) throw(isis::application_exception)
	{
		isis_LOG(lg, isis_FILE, isis_INFO) << "MetaLinkHandler::process_ConstraintAdded()";
		try
		{
			isis_LOG(lg, isis_FILE, isis_INFO) << "**************** Begin Recieved Message Constraints **********************";
			isis_LOG(lg, isis_FILE, isis_INFO) << "************ Begin Message Content *********************";

			std::string componentInstanceID = in_constraints.componentid();

			isis_LOG(lg, isis_FILE, isis_INFO) << "componentInstanceID: " << componentInstanceID;
			isis_LOG(lg, isis_FILE, isis_INFO) << "Received a constraint added message, component instance ID:  " << componentInstanceID;

			::google::protobuf::RepeatedPtrField< ::edu::vanderbilt::isis::meta::PairType > contraintPairs = in_constraints.pair();

			std::vector< isis::ConstraintPair>  creoConstraintPairs;

			for(int iy=0; iy < contraintPairs.size(); ++iy)
			{
				meta::PairType contraintPair = contraintPairs.Get(iy);

				isis::ConstraintPair  creoConstraintPair;

				isis_LOG(lg, isis_FILE, isis_INFO)
					<< "contraintPair.featurealignmenttype(): " <<	 contraintPair.featurealignmenttype()
					<< isis_EOL << "contraintPair.featuregeometrytype():  " <<	contraintPair.featuregeometrytype()
					<< isis_EOL << "contraintPair.featureinterfacetype(): " <<	contraintPair.featureinterfacetype();

				//creoConstraintPair.featureAlignmentType = isis::ProAsmcompConstrType_enum(contraintPair.featurealignmenttype());
				creoConstraintPair.featureAlignmentType = isis::CADAssemblyConstraintType_enum(contraintPair.featurealignmenttype());
				// creoConstraintPair.featureGeometryType  = isis::FeatureGeometryType_enum(contraintPair.featuregeometrytype());
				creoConstraintPair.featureGeometryType  = isis::CADFeatureGeometryType_enum(contraintPair.featuregeometrytype());
				creoConstraintPair.featureInterfaceType = isis::FeatureInterfaceType_enum(contraintPair.featureinterfacetype());

				edu::vanderbilt::isis::meta::ConstraintFeatureType constraintFeature_A = contraintPair.constraintfeaturea();
				edu::vanderbilt::isis::meta::ConstraintFeatureType constraintFeature_B = contraintPair.constraintfeatureb();

				isis_LOG(lg, isis_FILE, isis_INFO)
					<< "constraintFeature_A.componentid(): "				<< constraintFeature_A.componentid()
					<< isis_EOL << "constraintFeature_A.featurename(): "				<< constraintFeature_A.featurename()
					<< isis_EOL << "constraintFeature_A.featureorientationtype(): "	<< constraintFeature_A.featureorientationtype();

				isis_LOG(lg, isis_FILE, isis_INFO)
					<< "constraintFeature_B.componentid(): "				<< constraintFeature_B.componentid()
					<< isis_EOL << "constraintFeature_B.featurename(): "				<< constraintFeature_B.featurename()
					<< isis_EOL << "constraintFeature_B.featureorientationtype(): "	<< constraintFeature_B.featureorientationtype();

				isis::ConstraintFeature  creoConstraintFeature_A;
				creoConstraintFeature_A.componentInstanceID	 =  constraintFeature_A.componentid();
				creoConstraintFeature_A.featureName			 =  constraintFeature_A.featurename();
				creoConstraintFeature_A.featureOrientationType =  isis::ProDatumside_enum(constraintFeature_A.featureorientationtype());
				creoConstraintPair.constraintFeatures.push_back(creoConstraintFeature_A);

				isis::ConstraintFeature  creoConstraintFeature_B;
				creoConstraintFeature_B.componentInstanceID	 =  constraintFeature_B.componentid();
				creoConstraintFeature_B.featureName			 =  constraintFeature_B.featurename();
				creoConstraintFeature_B.featureOrientationType =  isis::ProDatumside_enum(constraintFeature_B.featureorientationtype());
				creoConstraintPair.constraintFeatures.push_back(creoConstraintFeature_B);

				creoConstraintPairs.push_back(creoConstraintPair);

			}
			isis_LOG(lg, isis_FILE, isis_INFO) << "************** End Message Content *********************"
				<< isis_EOL << "****************** End Recieved Message Constraints **********************";
			m_assembler->ConstrainComponent(componentInstanceID, creoConstraintPairs);

		}  // END Try
		catch(isis::application_exception& ex)
		{
			ex.setComponentInfo(in_constraints.componentid());
			isis_LOG(lg, isis_FILE, isis_ERROR) << "ERROR, Function: MetaLinkHandler::process_ConstraintAdded,  " << ex;
			meta::Notice notice;
			notice.set_noticemode(meta::Notice_NoticeMode_FAULT);
			if(stricmp(ex.get_error_code(), "C00000") != 0)
			{
				notice.set_msg(ex.tostring());
				notice.set_code(ex.get_error_code());
			}
			else
			{
				notice.set_msg(ex.tostring());
				notice.set_code("C00003");
			}
			return notice;
		}
		catch(std::exception& ex)
		{
			isis_LOG(lg, isis_FILE, isis_ERROR) << "ERROR, Function: MetaLinkHandler::process_ConstraintAdded,  " << ex.what();
			meta::Notice notice;
			notice.set_noticemode(meta::Notice_NoticeMode_FAULT);
			notice.set_msg(ex.what());
			notice.set_code("C00001");
			return notice;

		}
		catch(...)
		{
			isis_LOG(lg, isis_FILE, isis_ERROR) << "ERROR: Function: MetaLinkHandler::process_ConstraintAdded caught exception (...).  Please report the error to the help desk.";
			meta::Notice notice;
			notice.set_noticemode(meta::Notice_NoticeMode_FAULT);
			notice.set_msg("Unknown exception.");
			notice.set_code("C00002");
			return notice;
		}

		meta::Notice notice;
		notice.set_noticemode(meta::Notice_NoticeMode_DONE);
		notice.set_msg("No exception.");
		notice.set_code("C00000");
		return notice;
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	meta::Notice MetaLinkHandler::process_ParametersModified(meta::ParametricParametersType &in_parameters) throw(isis::application_exception)
	{
		isis_LOG(lg, isis_FILE, isis_DEBUG) << "MetaLinkHandler::process_ParametersModified()";
		try
		{
			isis_LOG(lg, isis_FILE, isis_INFO)
				<< isis_EOL << "**************** Begin Recieved Message Parameters **********************"
				<< isis_EOL << "************ Begin Message Content *********************";

			std::string componentInstanceID = in_parameters.componentid();

			::google::protobuf::RepeatedPtrField< ::edu::vanderbilt::isis::meta::CADParameterType > cADParameters = in_parameters.cadparameter();

			std::vector<isis::CADParameter> creoParameters;

			isis::CADParameter creoParameter;

			isis_LOG(lg, isis_FILE, isis_INFO) << "componentInstanceID: " << componentInstanceID
				<< isis_EOL << "Received a parameter modified message, component instance ID:  " << componentInstanceID;

			for(int iy=0; iy < cADParameters.size(); ++iy)
			{
				meta::CADParameterType parameter = cADParameters.Get(iy);
				isis_LOG(lg, isis_FILE, isis_INFO)  << "parameter.name():  " << parameter.name()
					<< isis_EOL << "parameter.value(): " << parameter.value()
					<< isis_EOL << "parameter.type():  " <<  isis::CADParameterType_enum(parameter.type());

				creoParameter.name = parameter.name();
				creoParameter.value = parameter.value();
				creoParameter.type =   isis::CADParameterType_enum(parameter.type());

				creoParameters.push_back(creoParameter);
			}
			isis_LOG(lg, isis_FILE, isis_INFO) << "************** End Message Content *********************";
			isis_LOG(lg, isis_FILE, isis_INFO) << "****************** End Recieved Message Parameters **********************";
			m_assembler->ModifyParameters(componentInstanceID, creoParameters);
		} // END try
		catch(isis::application_exception& ex)
		{
			ex.setComponentInfo(in_parameters.componentid());
			isis_LOG(lg, isis_FILE, isis_ERROR) << "ERROR, Function: MetaLinkHandler::process_ParametersModified,  " << ex;
			meta::Notice notice;
			notice.set_noticemode(meta::Notice_NoticeMode_FAULT);
			if(stricmp(ex.get_error_code(), "C00000") != 0)
			{
				notice.set_msg(ex.tostring());
				notice.set_code(ex.get_error_code());
			}
			else
			{
				notice.set_msg(ex.tostring());
				notice.set_code("C00003");
			}
			return notice;
		}
		catch(std::exception& ex)
		{
			isis_LOG(lg, isis_FILE, isis_ERROR) << "ERROR, Function: MetaLinkHandler::process_ParametersModified,  " << ex.what();
			meta::Notice notice;
			notice.set_noticemode(meta::Notice_NoticeMode_FAULT);
			notice.set_msg(ex.what());
			notice.set_code("C00001");
			return notice;

		}
		catch(...)
		{
			isis_LOG(lg, isis_FILE, isis_ERROR) << "ERROR: Function: MetaLinkHandler::process_ParametersModified caught exception (...).  Please report the error to the help desk.";
			meta::Notice notice;
			notice.set_noticemode(meta::Notice_NoticeMode_FAULT);
			notice.set_msg("Unknown exception.");
			notice.set_code("C00002");
			return notice;
		}

		meta::Notice notice;
		notice.set_noticemode(meta::Notice_NoticeMode_DONE);
		notice.set_msg("No exception.");
		notice.set_code("C00000");
		return notice;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	meta::Notice MetaLinkHandler::process_CreateAssembly(const std::string &in_AssemblyXML) throw(isis::application_exception)
	{
		isis_LOG(lg, isis_FILE, isis_DEBUG) << "MetaLinkHandler::process_CreateAssembly()";
		try
		{
			m_assembler->CreateAssembly(in_AssemblyXML);
		}
		catch(isis::application_exception& ex)
		{
			isis::GlobalModelData::Instance.mode = UNDEFINEDMODE;
			isis_LOG(lg, isis_FILE, isis_ERROR)  << "ERROR, Function: MetaLinkHandler::process_CreateAssembly,  application_exception: " << ex;
			meta::Notice notice;
			notice.set_noticemode(meta::Notice_NoticeMode_FAULT);
			if(stricmp(ex.get_error_code(), "C00000") != 0)
			{
				notice.set_msg(ex.tostring());
				notice.set_code(ex.get_error_code());
			}
			else
			{
				notice.set_msg(ex.tostring());
				notice.set_code("C00003");
			}
			return notice;
		}
		catch(std::exception& ex)
		{
			isis_LOG(lg, isis_FILE, isis_ERROR) << "ERROR, Function: MetaLinkHandler::process_CreateAssembly,  std::exception: " << ex.what();
			meta::Notice notice;
			notice.set_noticemode(meta::Notice_NoticeMode_FAULT);
			notice.set_msg(ex.what());
			notice.set_code("C00001");
			return notice;

		}
		catch(...)
		{
			isis_LOG(lg, isis_FILE, isis_ERROR)  << "ERROR: Function: MetaLinkHandler::process_CreateAssembly : caught exception (...).  Please report the error to the help desk.";
			meta::Notice notice;
			notice.set_noticemode(meta::Notice_NoticeMode_FAULT);
			notice.set_msg("Unknown exception.");
			notice.set_code("C00002");
			return notice;
		}

		meta::Notice notice;
		notice.set_noticemode(meta::Notice_NoticeMode_DONE);
		notice.set_msg("No exception.");
		notice.set_code("C00000");
		return notice;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// If in_SearchPaths.size() == 0 then no action taken.
	meta::Notice MetaLinkHandler::process_SearchPaths(const std::list<std::string> &in_SearchPaths)throw(isis::application_exception)
	{
		isis_LOG(lg, isis_FILE, isis_DEBUG) << "MetaLinkAssemblyEditor::process_SearchPaths()";
		if(in_SearchPaths.size() == 0)
		{
			isis_LOG(lg, isis_FILE, isis_WARN)  << "Function MetaLinkHandler::process_SearchPaths received an empty in_SearchPaths.";
		}

		try
		{
			m_assembler->AddSearchPaths(in_SearchPaths);
		}
		catch(isis::application_exception& ex)
		{
			isis_LOG(lg, isis_FILE, isis_ERROR) << "ERROR, Function: MetaLinkHandler::process_SearchPaths, application exception " << ex;
			meta::Notice notice;
			notice.set_noticemode(meta::Notice_NoticeMode_FAULT);
			if(stricmp(ex.get_error_code(), "C00000") != 0)
			{
				notice.set_msg(ex.tostring());
				notice.set_code(ex.get_error_code());
			}
			else
			{
				notice.set_msg(ex.tostring());
				notice.set_code("C00003");
			}
			return notice;
		}
		catch(std::exception& ex)
		{
			isis_LOG(lg, isis_FILE, isis_ERROR) << "ERROR, Function: MetaLinkHandler::process_SearchPaths, std::exception " << ex.what() << isis_EOL;
			meta::Notice notice;
			notice.set_noticemode(meta::Notice_NoticeMode_FAULT);
			notice.set_msg(ex.what());
			notice.set_code("C00001");
			return notice;
		}
		catch(...)
		{
			isis_LOG(lg, isis_FILE, isis_ERROR) << "ERROR: Function: MetaLinkHandler::process_SearchPaths, caught exception(...). " << "Please report the error to the help desk." << isis_EOL;
			meta::Notice notice;
			notice.set_noticemode(meta::Notice_NoticeMode_FAULT);
			notice.set_msg("Unknown exception.");
			notice.set_code("C00002");
			return notice;
		}
		meta::Notice notice;
		notice.set_noticemode(meta::Notice_NoticeMode_DONE);
		notice.set_msg("No exception.");
		notice.set_code("C00000");
		return notice;
	}

	// Set the CyPhy name of the datums for a component
	static ProError DatumNameVisit(ProFeature* p_feature, ProError status, ProAppData app_data)
	{
		
		map<string,string> *datumnamemap = (map<string,string>*)app_data;
		ProFeattype type;
		ProFeatureTypeGet(p_feature, &type);
		if(type == PRO_FEAT_DATUM || type == PRO_FEAT_DATUM_AXIS || type == PRO_FEAT_DATUM_POINT || type == PRO_FEAT_CSYS)
		{
			ProElempath path;
			ProValue value;
			ProValueData  value_data;
			ProElempathItem constr_type[3];

			constr_type[0].type = PRO_ELEM_PATH_ITEM_TYPE_ID;
			constr_type[0].path_item.elem_id = PRO_E_STD_FEATURE_NAME;

			ProElempathAlloc(&path);
			ProElempathDataSet(path, constr_type, 1);
			ProError error = ProFeatureElemValueGet(p_feature, path, &value);
			ProValueDataGet(value, &value_data);
			MultiFormatString datumname(value_data.v.w);
			string cyphyname = datumnamemap->operator[]((string)datumname);
			if(cyphyname.size()>0)
			{
				ProParamvalue v;
				v.type = PRO_PARAM_STRING;
				wcscpy(v.value.s_val, wstring(cyphyname.begin(),cyphyname.end()).c_str());
				ProParameter param;
				ProError err = ProParameterCreate(p_feature, const_cast<wchar_t*>(wstring(CYPHY_NAME.begin(),CYPHY_NAME.end()).c_str()), &v, &param);
				if(err != PRO_TK_NO_ERROR && err != PRO_TK_E_FOUND)
				{
					isis_LOG(lg, isis_FILE, isis_WARN) << "Can't set CYPHY_NAME for " << datumname << ", error: " << err;

				}
				else if(err == PRO_TK_E_FOUND)
				{
					err = ProParameterInit(p_feature, const_cast<wchar_t*>(wstring(CYPHY_NAME.begin(),CYPHY_NAME.end()).c_str()), &param);
					if(err != PRO_TK_NO_ERROR)
					{
						isis_LOG(lg, isis_FILE, isis_WARN) << "Can't set CYPHY_NAME for " << datumname << ", error: " << err;
					}
					else
					{
						err = ProParameterValueSet(&param, &v);
						if(err != PRO_TK_NO_ERROR)
						{
							isis_LOG(lg, isis_FILE, isis_WARN) << "Can't set CYPHY_NAME for " << datumname << ", error: " << err;
						}
					}
				}

			}
		}
		return PRO_TK_NO_ERROR;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	meta::Notice MetaLinkHandler::process_AvmComponentInitialize(const string& in_AvmComponentID, meta::CADComponentType &in_component) throw (isis::application_exception)
	{
		isis_LOG(lg, isis_FILE, isis_INFO)  << "**************** Begin Recieved Message Init AVM Component **********************";
		isis_LOG(lg, isis_FILE, isis_INFO)  << "************ Begin Message Content *********************";

		isis_LOG(lg, isis_FILE, isis_INFO)  << "Received an AVM component init message, name:  " << in_component.name();

		try
		{
			isis_LOG(lg, isis_FILE, isis_INFO)	<< isis_EOL << "MetaLinkHandler::defineComponent. Received message:"
				<< isis_EOL << "AVM componentInstanceId:        "  <<  in_AvmComponentID
				// << isis_EOL << "component.model():              "	<<  in_component.model()
				<< isis_EOL << "component.name():               "	<<  in_component.name()
				<< isis_EOL << "component.type():               "  <<  in_component.type()
				<< isis_EOL;

			const std::string					creoModelName; //  =	        in_component.model()
			const std::string					creoAvmComponentName =	in_component.name();
			ProMdlType							creoModelType =			isis::ProMdlType_enum(in_component.type());

			isis_LOG(lg, isis_FILE, isis_INFO) << "************** End Message Content *********************";
			isis_LOG(lg, isis_FILE, isis_INFO) << "****************** End Received Message Init Component **********************";

			m_assembler->InitAvmComponent(in_AvmComponentID, creoModelName, creoAvmComponentName, creoModelType);
			GlobalModelData::Instance.ComponentEdit.resourceId = in_component.cadmodelid();

			// Set up connector information for the model
			GlobalModelData::Instance.ComponentEdit.connectors.clear();
			for(int i = 0; i < in_component.connectors().size(); i++)
			{
				CADComponentConnector connector;
				connector.id = in_component.connectors(i).id();
				connector.displayName = in_component.connectors(i).displayname();
				GlobalModelData::Instance.ComponentEdit.connectors.push_back(connector);
			}

			map<string,string> datumnamemap;

			for(int i = 0; i < in_component.datums().size(); i++)
			{
				string idupper = boost::to_upper_copy(in_component.datums(i).id());
				datumnamemap[idupper] = in_component.datums(i).displayname();
			}
			ProSolidFeatVisit(ProMdlToSolid(GlobalModelData::Instance.ComponentEdit.mdl), DatumNameVisit, NULL, &datumnamemap);

			ProError status = ProTreetoolRefresh(GlobalModelData::Instance.ComponentEdit.mdl);

			// Refresh the model tree component
			switch(status)
			{
			case PRO_TK_NO_ERROR:
				{
					isis_LOG(lg, isis_FILE, isis_INFO) << "ProTreetoolRefresh(): Successfully refreshed model tree.";
					break;
				}
			case PRO_TK_BAD_INPUTS:
				{
					isis_LOG(lg, isis_FILE, isis_WARN) << "ProTreetoolRefresh(): Invalid model for UI tree refresh.";
					break;
				}
			case PRO_TK_E_NOT_FOUND:
				{
					isis_LOG(lg, isis_FILE, isis_WARN) << "ProTreetoolRefresh(): Model tree is not present for UI tree refresh.";
					break;
				}
			}



		} // END try
		// This is a fatal error. In case of failure we want Creo to exit with an error
		catch(isis::application_exception& ex)
		{
			ex.setComponentInfo(in_component.name());
			throw;
		}
		catch(std::exception& ex)
		{
			throw;
		}
		catch(...)
		{
			throw;
		}

		meta::Notice notice;
		notice.set_noticemode(meta::Notice_NoticeMode_DONE);
		notice.set_msg("No exception.");
		notice.set_code("C00000");
		return notice;
	}

	void MetaLinkHandler::CreateAssembly(std::string xmlfile)
	{
		ifstream xmlistream(xmlfile);
		stringstream s;
		s << xmlistream.rdbuf();
		m_assembler->CreateAssembly(s.str());
	}
	//////////////////////////////////////////////

} // namespace isis

