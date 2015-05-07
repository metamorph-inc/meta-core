// cpp (meta datanetwork format) source file Semantics.cpp
// generated from diagram Semantics
// generated on Thu Nov 11 14:16:28 2010

#include "Semantics.h"
#include <UmlExt.h>
#include <UdmStatic.h>

#include <UdmDom.h>
#include "Semantics_xsd.h"

namespace Semantics {

	::Uml::Class TTSchedule::meta;
	::Uml::Attribute TTSchedule::meta_ticksched;

	::Uml::Class Schedule::meta;
	::Uml::Attribute Schedule::meta_sched;
	::Uml::CompositionParentRole Schedule::meta_ExecInfo_parent;

	::Uml::Class Executable::meta;
	::Uml::AssociationRole Executable::meta_info;

	::Uml::Class Channel::meta;
	::Uml::Attribute Channel::meta_name;
	::Uml::Attribute Channel::meta_id;
	::Uml::Attribute Channel::meta_config;
	::Uml::CompositionParentRole Channel::meta_Deployment_parent;

	::Uml::Class Device::meta;
	::Uml::Attribute Device::meta_name;
	::Uml::Attribute Device::meta_id;
	::Uml::Attribute Device::meta_devicetype;
	::Uml::Attribute Device::meta_config;
	::Uml::AssociationRole Device::meta_integratedBy;
	::Uml::CompositionParentRole Device::meta_Deployment_parent;

	::Uml::Class Msg::meta;
	::Uml::Attribute Msg::meta_name;
	::Uml::Attribute Msg::meta_id;
	::Uml::Attribute Msg::meta_sizebytes;
	::Uml::Attribute Msg::meta_messagetype;
	::Uml::AssociationRole Msg::meta_dummyMsgDeps;
	::Uml::AssociationRole Msg::meta_msgDep;
	::Uml::AssociationRole Msg::meta_msgListeners;
	::Uml::AssociationRole Msg::meta_msgTransmitter;
	::Uml::AssociationRole Msg::meta_msgActuation;
	::Uml::AssociationRole Msg::meta_msgAcquisitions;
	::Uml::AssociationRole Msg::meta_dummyMsgNode;
	::Uml::AssociationRole Msg::meta_carries;
	::Uml::CompositionParentRole Msg::meta_Deployment_parent;

	::Uml::Class Task::meta;
	::Uml::Attribute Task::meta_name;
	::Uml::Attribute Task::meta_id;
	::Uml::AssociationRole Task::meta_latencyAsSndr;
	::Uml::AssociationRole Task::meta_dummyDeps;
	::Uml::AssociationRole Task::meta_localReceivingTaskDeps;
	::Uml::AssociationRole Task::meta_localSendingTaskDeps;
	::Uml::AssociationRole Task::meta_incomingData;
	::Uml::AssociationRole Task::meta_taskActuations;
	::Uml::AssociationRole Task::meta_taskAcquisitions;
	::Uml::AssociationRole Task::meta_executedBy;
	::Uml::AssociationRole Task::meta_invokes;
	::Uml::AssociationRole Task::meta_outboundData;
	::Uml::AssociationRole Task::meta_latencyAsRcvr;
	::Uml::CompositionParentRole Task::meta_Deployment_parent;

	::Uml::Class MaxDuration::meta;

	::Uml::Class Duration::meta;
	::Uml::Attribute Duration::meta_exectimesecs;
	::Uml::CompositionParentRole Duration::meta_ExecInfo_parent;

	::Uml::Class SamplePeriod::meta;

	::Uml::Class Periodicity::meta;
	::Uml::Attribute Periodicity::meta_periodsecs;
	::Uml::CompositionParentRole Periodicity::meta_ExecInfo_parent;

	::Uml::Class MinimumPeriod::meta;

	::Uml::Class DummyDependency::meta;
	::Uml::AssociationRole DummyDependency::meta_sendingTask;
	::Uml::AssociationRole DummyDependency::meta_dummyMsg;
	::Uml::AssociationRole DummyDependency::meta_dummyDepNode;

	::Uml::Class CommRelations::meta;
	::Uml::Attribute CommRelations::meta_id;
	::Uml::CompositionParentRole CommRelations::meta_Deployment_parent;

	::Uml::Class LocalDependency::meta;
	::Uml::AssociationRole LocalDependency::meta_localDepNode;
	::Uml::AssociationRole LocalDependency::meta_localDepReceivingTasks;
	::Uml::AssociationRole LocalDependency::meta_localDepSendingTask;
	::Uml::AssociationRole LocalDependency::meta_depMsg;

	::Uml::Class TestPoint::meta;
	::Uml::Attribute TestPoint::meta_name;
	::Uml::Attribute TestPoint::meta_id;
	::Uml::Attribute TestPoint::meta_config;
	::Uml::CompositionParentRole TestPoint::meta_Deployment_parent;

	::Uml::Class Sink::meta;
	::Uml::AssociationRole Sink::meta_measures;

	::Uml::Class Source::meta;
	::Uml::AssociationRole Source::meta_creates;

	::Uml::Class OutputDevice::meta;
	::Uml::AssociationRole OutputDevice::meta_outputEnvironment;
	::Uml::AssociationRole OutputDevice::meta_outputChannels;

	::Uml::Class InputDevice::meta;
	::Uml::AssociationRole InputDevice::meta_inputEnvironment;
	::Uml::AssociationRole InputDevice::meta_inputChannels;

	::Uml::Class CommInterface::meta;
	::Uml::AssociationRole CommInterface::meta_commMedium;
	::Uml::AssociationRole CommInterface::meta_commChannels;

	::Uml::Class Receives::meta;
	::Uml::AssociationRole Receives::meta_receivedMsg;
	::Uml::AssociationRole Receives::meta_receivingChannel;
	::Uml::AssociationRole Receives::meta_receivingTask;

	::Uml::Class Transmits::meta;
	::Uml::AssociationRole Transmits::meta_transmittedMsg;
	::Uml::AssociationRole Transmits::meta_sendingChan;
	::Uml::AssociationRole Transmits::meta_sendingTask;

	::Uml::Class Actuation::meta;
	::Uml::AssociationRole Actuation::meta_actuatingChannel;
	::Uml::AssociationRole Actuation::meta_actuatingMsg;
	::Uml::AssociationRole Actuation::meta_actuatingTask;

	::Uml::Class Acquisition::meta;
	::Uml::AssociationRole Acquisition::meta_acquiringTasks;
	::Uml::AssociationRole Acquisition::meta_acquiredMsg;
	::Uml::AssociationRole Acquisition::meta_acquiringChannel;

	::Uml::Class OChan::meta;
	::Uml::AssociationRole OChan::meta_channelActuations;
	::Uml::AssociationRole OChan::meta_ochanDevice;

	::Uml::Class BChan::meta;
	::Uml::AssociationRole BChan::meta_channelListeners;
	::Uml::AssociationRole BChan::meta_channelTransmitters;
	::Uml::AssociationRole BChan::meta_chanInterface;

	::Uml::Class IChan::meta;
	::Uml::AssociationRole IChan::meta_channelAcquisition;
	::Uml::AssociationRole IChan::meta_ichanDevice;

	::Uml::Class TTBus::meta;
	::Uml::Attribute TTBus::meta_hyperperiodticks;
	::Uml::Attribute TTBus::meta_hyperperiodsecs;
	::Uml::Attribute TTBus::meta_setupsecs;
	::Uml::Attribute TTBus::meta_switchmembytes;
	::Uml::Attribute TTBus::meta_minframebytes;
	::Uml::Attribute TTBus::meta_busslotbytes;

	::Uml::Class CommMedium::meta;
	::Uml::Attribute CommMedium::meta_name;
	::Uml::Attribute CommMedium::meta_id;
	::Uml::Attribute CommMedium::meta_dataratebps;
	::Uml::AssociationRole CommMedium::meta_interfaces;
	::Uml::CompositionParentRole CommMedium::meta_Deployment_parent;

	::Uml::Class ExecInfo::meta;
	::Uml::Attribute ExecInfo::meta_name;
	::Uml::AssociationRole ExecInfo::meta_executor;
	::Uml::CompositionChildRole ExecInfo::meta_Schedule_child;
	::Uml::CompositionChildRole ExecInfo::meta_Periodicity_children;
	::Uml::CompositionChildRole ExecInfo::meta_Duration_child;
	::Uml::CompositionParentRole ExecInfo::meta_Deployment_parent;

	::Uml::Class RootFolder::meta;
	::Uml::Attribute RootFolder::meta_name;
	::Uml::CompositionChildRole RootFolder::meta_RootFolder_children;
	::Uml::CompositionChildRole RootFolder::meta_Deployment_children;
	::Uml::CompositionParentRole RootFolder::meta_RootFolder_parent;

	::Uml::Class Deployment::meta;
	::Uml::Attribute Deployment::meta_id;
	::Uml::Attribute Deployment::meta_name;
	::Uml::CompositionChildRole Deployment::meta_SignalType_children;
	::Uml::CompositionChildRole Deployment::meta_TestPoint_children;
	::Uml::CompositionChildRole Deployment::meta_CommRelations_children;
	::Uml::CompositionChildRole Deployment::meta_Channel_children;
	::Uml::CompositionChildRole Deployment::meta_Plant_children;
	::Uml::CompositionChildRole Deployment::meta_Msg_children;
	::Uml::CompositionChildRole Deployment::meta_CommMedium_children;
	::Uml::CompositionChildRole Deployment::meta_Task_children;
	::Uml::CompositionChildRole Deployment::meta_Signal_children;
	::Uml::CompositionChildRole Deployment::meta_Device_children;
	::Uml::CompositionChildRole Deployment::meta_Component_children;
	::Uml::CompositionChildRole Deployment::meta_Node_children;
	::Uml::CompositionChildRole Deployment::meta_ExecInfo_children;
	::Uml::CompositionChildRole Deployment::meta_TimingConstraint_children;
	::Uml::CompositionParentRole Deployment::meta_RootFolder_parent;

	::Uml::Class SignalType::meta;
	::Uml::Attribute SignalType::meta_name;
	::Uml::Attribute SignalType::meta_id;
	::Uml::Attribute SignalType::meta_mtypestr;
	::Uml::Attribute SignalType::meta_ctypestr;
	::Uml::Attribute SignalType::meta_rows;
	::Uml::Attribute SignalType::meta_columns;
	::Uml::Attribute SignalType::meta_isStruct;
	::Uml::AssociationRole SignalType::meta_signalsOfType;
	::Uml::CompositionParentRole SignalType::meta_Deployment_parent;

	::Uml::Class Plant::meta;
	::Uml::Attribute Plant::meta_name;
	::Uml::Attribute Plant::meta_id;
	::Uml::Attribute Plant::meta_config;
	::Uml::AssociationRole Plant::meta_actuationDevices;
	::Uml::AssociationRole Plant::meta_acquisitionDevices;
	::Uml::CompositionParentRole Plant::meta_Deployment_parent;

	::Uml::Class Signal::meta;
	::Uml::Attribute Signal::meta_callIndex;
	::Uml::Attribute Signal::meta_id;
	::Uml::Attribute Signal::meta_msgIndex;
	::Uml::Attribute Signal::meta_name;
	::Uml::AssociationRole Signal::meta_createdBy;
	::Uml::AssociationRole Signal::meta_measuredBy;
	::Uml::AssociationRole Signal::meta_carriedBy;
	::Uml::AssociationRole Signal::meta_isOfType;
	::Uml::AssociationRole Signal::meta_consumedBy;
	::Uml::AssociationRole Signal::meta_generatedBy;
	::Uml::CompositionParentRole Signal::meta_Deployment_parent;

	::Uml::Class Component::meta;
	::Uml::Attribute Component::meta_name;
	::Uml::Attribute Component::meta_id;
	::Uml::Attribute Component::meta_msubsystem;
	::Uml::Attribute Component::meta_ccode;
	::Uml::Attribute Component::meta_hfiles;
	::Uml::Attribute Component::meta_cfiles;
	::Uml::AssociationRole Component::meta_consumes;
	::Uml::AssociationRole Component::meta_invokedBy;
	::Uml::AssociationRole Component::meta_generates;
	::Uml::CompositionParentRole Component::meta_Deployment_parent;

	::Uml::Class Node::meta;
	::Uml::Attribute Node::meta_name;
	::Uml::Attribute Node::meta_id;
	::Uml::Attribute Node::meta_platformtype;
	::Uml::Attribute Node::meta_processormodel;
	::Uml::Attribute Node::meta_hyperperiodsecs;
	::Uml::Attribute Node::meta_hyperperiodticks;
	::Uml::Attribute Node::meta_clockspeedhz;
	::Uml::Attribute Node::meta_tickresolutionsecs;
	::Uml::Attribute Node::meta_ctxtswitchsecs;
	::Uml::Attribute Node::meta_sndohdsecs;
	::Uml::Attribute Node::meta_rcvohdsecs;
	::Uml::Attribute Node::meta_analog_inp_num;
	::Uml::Attribute Node::meta_analog_outp_num;
	::Uml::Attribute Node::meta_pp_directives;
	::Uml::AssociationRole Node::meta_nodeDummyDeps;
	::Uml::AssociationRole Node::meta_nodeLocalDeps;
	::Uml::AssociationRole Node::meta_nodeDummyMsg;
	::Uml::AssociationRole Node::meta_executes;
	::Uml::AssociationRole Node::meta_integrates;
	::Uml::CompositionParentRole Node::meta_Deployment_parent;

	::Uml::Class TimingConstraint::meta;
	::Uml::Attribute TimingConstraint::meta_timeboundsecs;
	::Uml::Attribute TimingConstraint::meta_id;
	::Uml::AssociationRole TimingConstraint::meta_sndTask;
	::Uml::AssociationRole TimingConstraint::meta_rcvTask;
	::Uml::CompositionParentRole TimingConstraint::meta_Deployment_parent;

	::Uml::Diagram meta;

	void CreateMeta() {
		// classes, with attributes, constraints and constraint definitions
		Acquisition::meta = ::Uml::Class::Create(meta);

		Actuation::meta = ::Uml::Class::Create(meta);

		BChan::meta = ::Uml::Class::Create(meta);

		Channel::meta = ::Uml::Class::Create(meta);
		Channel::meta_name = ::Uml::Attribute::Create(Channel::meta);
		Channel::meta_id = ::Uml::Attribute::Create(Channel::meta);
		Channel::meta_config = ::Uml::Attribute::Create(Channel::meta);

		CommInterface::meta = ::Uml::Class::Create(meta);

		CommMedium::meta = ::Uml::Class::Create(meta);
		CommMedium::meta_name = ::Uml::Attribute::Create(CommMedium::meta);
		CommMedium::meta_id = ::Uml::Attribute::Create(CommMedium::meta);
		CommMedium::meta_dataratebps = ::Uml::Attribute::Create(CommMedium::meta);

		CommRelations::meta = ::Uml::Class::Create(meta);
		CommRelations::meta_id = ::Uml::Attribute::Create(CommRelations::meta);

		Component::meta = ::Uml::Class::Create(meta);
		Component::meta_name = ::Uml::Attribute::Create(Component::meta);
		Component::meta_id = ::Uml::Attribute::Create(Component::meta);
		Component::meta_msubsystem = ::Uml::Attribute::Create(Component::meta);
		Component::meta_ccode = ::Uml::Attribute::Create(Component::meta);
		Component::meta_hfiles = ::Uml::Attribute::Create(Component::meta);
		Component::meta_cfiles = ::Uml::Attribute::Create(Component::meta);

		Deployment::meta = ::Uml::Class::Create(meta);
		Deployment::meta_id = ::Uml::Attribute::Create(Deployment::meta);
		Deployment::meta_name = ::Uml::Attribute::Create(Deployment::meta);

		Device::meta = ::Uml::Class::Create(meta);
		Device::meta_name = ::Uml::Attribute::Create(Device::meta);
		Device::meta_id = ::Uml::Attribute::Create(Device::meta);
		Device::meta_devicetype = ::Uml::Attribute::Create(Device::meta);
		Device::meta_config = ::Uml::Attribute::Create(Device::meta);

		DummyDependency::meta = ::Uml::Class::Create(meta);

		Duration::meta = ::Uml::Class::Create(meta);
		Duration::meta_exectimesecs = ::Uml::Attribute::Create(Duration::meta);

		ExecInfo::meta = ::Uml::Class::Create(meta);
		ExecInfo::meta_name = ::Uml::Attribute::Create(ExecInfo::meta);

		Executable::meta = ::Uml::Class::Create(meta);

		IChan::meta = ::Uml::Class::Create(meta);

		InputDevice::meta = ::Uml::Class::Create(meta);

		LocalDependency::meta = ::Uml::Class::Create(meta);

		MaxDuration::meta = ::Uml::Class::Create(meta);

		MinimumPeriod::meta = ::Uml::Class::Create(meta);

		Msg::meta = ::Uml::Class::Create(meta);
		Msg::meta_name = ::Uml::Attribute::Create(Msg::meta);
		Msg::meta_id = ::Uml::Attribute::Create(Msg::meta);
		Msg::meta_sizebytes = ::Uml::Attribute::Create(Msg::meta);
		Msg::meta_messagetype = ::Uml::Attribute::Create(Msg::meta);

		Node::meta = ::Uml::Class::Create(meta);
		Node::meta_name = ::Uml::Attribute::Create(Node::meta);
		Node::meta_id = ::Uml::Attribute::Create(Node::meta);
		Node::meta_platformtype = ::Uml::Attribute::Create(Node::meta);
		Node::meta_processormodel = ::Uml::Attribute::Create(Node::meta);
		Node::meta_hyperperiodsecs = ::Uml::Attribute::Create(Node::meta);
		Node::meta_hyperperiodticks = ::Uml::Attribute::Create(Node::meta);
		Node::meta_clockspeedhz = ::Uml::Attribute::Create(Node::meta);
		Node::meta_tickresolutionsecs = ::Uml::Attribute::Create(Node::meta);
		Node::meta_ctxtswitchsecs = ::Uml::Attribute::Create(Node::meta);
		Node::meta_sndohdsecs = ::Uml::Attribute::Create(Node::meta);
		Node::meta_rcvohdsecs = ::Uml::Attribute::Create(Node::meta);
		Node::meta_analog_inp_num = ::Uml::Attribute::Create(Node::meta);
		Node::meta_analog_outp_num = ::Uml::Attribute::Create(Node::meta);
		Node::meta_pp_directives = ::Uml::Attribute::Create(Node::meta);

		OChan::meta = ::Uml::Class::Create(meta);

		OutputDevice::meta = ::Uml::Class::Create(meta);

		Periodicity::meta = ::Uml::Class::Create(meta);
		Periodicity::meta_periodsecs = ::Uml::Attribute::Create(Periodicity::meta);

		Plant::meta = ::Uml::Class::Create(meta);
		Plant::meta_name = ::Uml::Attribute::Create(Plant::meta);
		Plant::meta_id = ::Uml::Attribute::Create(Plant::meta);
		Plant::meta_config = ::Uml::Attribute::Create(Plant::meta);

		Receives::meta = ::Uml::Class::Create(meta);

		RootFolder::meta = ::Uml::Class::Create(meta);
		RootFolder::meta_name = ::Uml::Attribute::Create(RootFolder::meta);

		SamplePeriod::meta = ::Uml::Class::Create(meta);

		Schedule::meta = ::Uml::Class::Create(meta);
		Schedule::meta_sched = ::Uml::Attribute::Create(Schedule::meta);

		Signal::meta = ::Uml::Class::Create(meta);
		Signal::meta_callIndex = ::Uml::Attribute::Create(Signal::meta);
		Signal::meta_id = ::Uml::Attribute::Create(Signal::meta);
		Signal::meta_msgIndex = ::Uml::Attribute::Create(Signal::meta);
		Signal::meta_name = ::Uml::Attribute::Create(Signal::meta);

		SignalType::meta = ::Uml::Class::Create(meta);
		SignalType::meta_name = ::Uml::Attribute::Create(SignalType::meta);
		SignalType::meta_id = ::Uml::Attribute::Create(SignalType::meta);
		SignalType::meta_mtypestr = ::Uml::Attribute::Create(SignalType::meta);
		SignalType::meta_ctypestr = ::Uml::Attribute::Create(SignalType::meta);
		SignalType::meta_rows = ::Uml::Attribute::Create(SignalType::meta);
		SignalType::meta_columns = ::Uml::Attribute::Create(SignalType::meta);
		SignalType::meta_isStruct = ::Uml::Attribute::Create(SignalType::meta);

		Sink::meta = ::Uml::Class::Create(meta);

		Source::meta = ::Uml::Class::Create(meta);

		TTBus::meta = ::Uml::Class::Create(meta);
		TTBus::meta_hyperperiodticks = ::Uml::Attribute::Create(TTBus::meta);
		TTBus::meta_hyperperiodsecs = ::Uml::Attribute::Create(TTBus::meta);
		TTBus::meta_setupsecs = ::Uml::Attribute::Create(TTBus::meta);
		TTBus::meta_switchmembytes = ::Uml::Attribute::Create(TTBus::meta);
		TTBus::meta_minframebytes = ::Uml::Attribute::Create(TTBus::meta);
		TTBus::meta_busslotbytes = ::Uml::Attribute::Create(TTBus::meta);

		TTSchedule::meta = ::Uml::Class::Create(meta);
		TTSchedule::meta_ticksched = ::Uml::Attribute::Create(TTSchedule::meta);

		Task::meta = ::Uml::Class::Create(meta);
		Task::meta_name = ::Uml::Attribute::Create(Task::meta);
		Task::meta_id = ::Uml::Attribute::Create(Task::meta);

		TestPoint::meta = ::Uml::Class::Create(meta);
		TestPoint::meta_name = ::Uml::Attribute::Create(TestPoint::meta);
		TestPoint::meta_id = ::Uml::Attribute::Create(TestPoint::meta);
		TestPoint::meta_config = ::Uml::Attribute::Create(TestPoint::meta);

		TimingConstraint::meta = ::Uml::Class::Create(meta);
		TimingConstraint::meta_timeboundsecs = ::Uml::Attribute::Create(TimingConstraint::meta);
		TimingConstraint::meta_id = ::Uml::Attribute::Create(TimingConstraint::meta);

		Transmits::meta = ::Uml::Class::Create(meta);

	}

	void InitMeta() {
		// classes, with attributes, constraints and constraint definitions
		::Uml::InitClassProps(Acquisition::meta, "Acquisition", false, NULL, NULL);

		::Uml::InitClassProps(Actuation::meta, "Actuation", false, NULL, NULL);

		::Uml::InitClassProps(BChan::meta, "BChan", false, NULL, NULL);

		::Uml::InitClassProps(Channel::meta, "Channel", false, NULL, NULL);
		::Uml::InitAttributeProps(Channel::meta_name, "name", "String", false, false, 1, 1, false, "public", vector<string>());
		::Uml::InitAttributeProps(Channel::meta_id, "id", "Integer", false, false, 1, 1, false, "public", vector<string>());
		::Uml::InitAttributeProps(Channel::meta_config, "config", "String", false, false, 1, 1, false, "public", vector<string>());

		::Uml::InitClassProps(CommInterface::meta, "CommInterface", false, NULL, NULL);

		::Uml::InitClassProps(CommMedium::meta, "CommMedium", false, NULL, NULL);
		::Uml::InitAttributeProps(CommMedium::meta_name, "name", "String", false, false, 1, 1, false, "public", vector<string>());
		::Uml::InitAttributeProps(CommMedium::meta_id, "id", "Integer", false, false, 1, 1, false, "public", vector<string>());
		::Uml::InitAttributeProps(CommMedium::meta_dataratebps, "dataratebps", "Real", false, false, 1, 1, false, "public", vector<string>());

		::Uml::InitClassProps(CommRelations::meta, "CommRelations", false, NULL, NULL);
		::Uml::InitAttributeProps(CommRelations::meta_id, "id", "Integer", false, false, 1, 1, false, "public", vector<string>());

		::Uml::InitClassProps(Component::meta, "Component", false, NULL, NULL);
		::Uml::InitAttributeProps(Component::meta_name, "name", "String", false, false, 1, 1, false, "public", vector<string>());
		::Uml::InitAttributeProps(Component::meta_id, "id", "Integer", false, false, 1, 1, false, "public", vector<string>());
		::Uml::InitAttributeProps(Component::meta_msubsystem, "msubsystem", "String", false, false, 1, 1, false, "public", vector<string>());
		::Uml::InitAttributeProps(Component::meta_ccode, "ccode", "String", false, false, 1, 1, false, "public", vector<string>());
		::Uml::InitAttributeProps(Component::meta_hfiles, "hfiles", "String", false, false, 1, 1, false, "public", vector<string>());
		::Uml::InitAttributeProps(Component::meta_cfiles, "cfiles", "String", false, false, 1, 1, false, "public", vector<string>());

		::Uml::InitClassProps(Deployment::meta, "Deployment", false, "Model", NULL);
		::Uml::InitAttributeProps(Deployment::meta_id, "id", "Integer", false, false, 1, 1, false, "public", vector<string>());
		::Uml::InitAttributeProps(Deployment::meta_name, "name", "String", false, false, 0, 1, false, "public", vector<string>());

		::Uml::InitClassProps(Device::meta, "Device", false, NULL, NULL);
		::Uml::InitAttributeProps(Device::meta_name, "name", "String", false, false, 1, 1, false, "public", vector<string>());
		::Uml::InitAttributeProps(Device::meta_id, "id", "Integer", false, false, 1, 1, false, "public", vector<string>());
		vector<string> Device_devicetype_dva;
		Device_devicetype_dva.push_back("none");
		::Uml::InitAttributeProps(Device::meta_devicetype, "devicetype", "String", false, false, 1, 1, false, "public", Device_devicetype_dva);
		::Uml::InitAttributeProps(Device::meta_config, "config", "String", false, false, 1, 1, false, "public", vector<string>());

		::Uml::InitClassProps(DummyDependency::meta, "DummyDependency", false, NULL, NULL);

		::Uml::InitClassProps(Duration::meta, "Duration", true, NULL, NULL);
		::Uml::InitAttributeProps(Duration::meta_exectimesecs, "exectimesecs", "Real", false, false, 1, 1, false, "public", vector<string>());

		::Uml::InitClassProps(ExecInfo::meta, "ExecInfo", false, NULL, NULL);
		::Uml::InitAttributeProps(ExecInfo::meta_name, "name", "String", false, false, 1, 1, false, "public", vector<string>());

		::Uml::InitClassProps(Executable::meta, "Executable", true, NULL, NULL);

		::Uml::InitClassProps(IChan::meta, "IChan", false, NULL, NULL);

		::Uml::InitClassProps(InputDevice::meta, "InputDevice", false, NULL, NULL);

		::Uml::InitClassProps(LocalDependency::meta, "LocalDependency", false, NULL, NULL);

		::Uml::InitClassProps(MaxDuration::meta, "MaxDuration", false, NULL, NULL);

		::Uml::InitClassProps(MinimumPeriod::meta, "MinimumPeriod", false, NULL, NULL);

		::Uml::InitClassProps(Msg::meta, "Msg", false, NULL, NULL);
		::Uml::InitAttributeProps(Msg::meta_name, "name", "String", false, false, 1, 1, false, "public", vector<string>());
		::Uml::InitAttributeProps(Msg::meta_id, "id", "Integer", false, false, 1, 1, false, "public", vector<string>());
		::Uml::InitAttributeProps(Msg::meta_sizebytes, "sizebytes", "Real", false, false, 1, 1, false, "public", vector<string>());
		vector<string> Msg_messagetype_dva;
		Msg_messagetype_dva.push_back("Local");
		::Uml::InitAttributeProps(Msg::meta_messagetype, "messagetype", "String", false, false, 1, 1, false, "public", Msg_messagetype_dva);

		::Uml::InitClassProps(Node::meta, "Node", false, NULL, NULL);
		::Uml::InitAttributeProps(Node::meta_name, "name", "String", false, false, 1, 1, false, "public", vector<string>());
		::Uml::InitAttributeProps(Node::meta_id, "id", "Integer", false, false, 1, 1, false, "public", vector<string>());
		::Uml::InitAttributeProps(Node::meta_platformtype, "platformtype", "String", false, false, 1, 1, false, "public", vector<string>());
		::Uml::InitAttributeProps(Node::meta_processormodel, "processormodel", "String", false, false, 1, 1, false, "public", vector<string>());
		::Uml::InitAttributeProps(Node::meta_hyperperiodsecs, "hyperperiodsecs", "Real", false, false, 1, 1, false, "public", vector<string>());
		::Uml::InitAttributeProps(Node::meta_hyperperiodticks, "hyperperiodticks", "Integer", false, false, 1, 1, false, "public", vector<string>());
		::Uml::InitAttributeProps(Node::meta_clockspeedhz, "clockspeedhz", "Real", false, false, 1, 1, false, "public", vector<string>());
		::Uml::InitAttributeProps(Node::meta_tickresolutionsecs, "tickresolutionsecs", "Real", false, false, 1, 1, false, "public", vector<string>());
		::Uml::InitAttributeProps(Node::meta_ctxtswitchsecs, "ctxtswitchsecs", "Real", false, false, 1, 1, false, "public", vector<string>());
		::Uml::InitAttributeProps(Node::meta_sndohdsecs, "sndohdsecs", "Real", false, false, 1, 1, false, "public", vector<string>());
		::Uml::InitAttributeProps(Node::meta_rcvohdsecs, "rcvohdsecs", "Real", false, false, 1, 1, false, "public", vector<string>());
		::Uml::InitAttributeProps(Node::meta_analog_inp_num, "analog_inp_num", "Integer", false, false, 1, 1, false, "public", vector<string>());
		::Uml::InitAttributeProps(Node::meta_analog_outp_num, "analog_outp_num", "Integer", false, false, 1, 1, false, "public", vector<string>());
		::Uml::InitAttributeProps(Node::meta_pp_directives, "pp_directives", "String", false, false, 1, 1, false, "public", vector<string>());

		::Uml::InitClassProps(OChan::meta, "OChan", false, NULL, NULL);

		::Uml::InitClassProps(OutputDevice::meta, "OutputDevice", false, NULL, NULL);

		::Uml::InitClassProps(Periodicity::meta, "Periodicity", true, NULL, NULL);
		::Uml::InitAttributeProps(Periodicity::meta_periodsecs, "periodsecs", "Real", false, false, 1, 1, false, "public", vector<string>());

		::Uml::InitClassProps(Plant::meta, "Plant", false, NULL, NULL);
		::Uml::InitAttributeProps(Plant::meta_name, "name", "String", false, false, 1, 1, false, "public", vector<string>());
		::Uml::InitAttributeProps(Plant::meta_id, "id", "Integer", false, false, 1, 1, false, "public", vector<string>());
		::Uml::InitAttributeProps(Plant::meta_config, "config", "String", false, false, 1, 1, false, "public", vector<string>());

		::Uml::InitClassProps(Receives::meta, "Receives", false, NULL, NULL);

		::Uml::InitClassProps(RootFolder::meta, "RootFolder", false, "Folder", NULL);
		::Uml::InitAttributeProps(RootFolder::meta_name, "name", "String", false, false, 0, 1, false, "public", vector<string>());

		::Uml::InitClassProps(SamplePeriod::meta, "SamplePeriod", false, NULL, NULL);

		::Uml::InitClassProps(Schedule::meta, "Schedule", true, NULL, NULL);
		::Uml::InitAttributeProps(Schedule::meta_sched, "sched", "String", false, false, 1, 1, false, "public", vector<string>());

		::Uml::InitClassProps(Signal::meta, "Signal", false, NULL, NULL);
		::Uml::InitAttributeProps(Signal::meta_callIndex, "callIndex", "Integer", false, false, 1, 1, false, "public", vector<string>());
		::Uml::InitAttributeProps(Signal::meta_id, "id", "Integer", false, false, 1, 1, false, "public", vector<string>());
		::Uml::InitAttributeProps(Signal::meta_msgIndex, "msgIndex", "Integer", false, false, 1, 1, false, "public", vector<string>());
		::Uml::InitAttributeProps(Signal::meta_name, "name", "String", false, false, 1, 1, false, "public", vector<string>());

		::Uml::InitClassProps(SignalType::meta, "SignalType", false, NULL, NULL);
		::Uml::InitAttributeProps(SignalType::meta_name, "name", "String", false, false, 1, 1, false, "public", vector<string>());
		::Uml::InitAttributeProps(SignalType::meta_id, "id", "Integer", false, false, 1, 1, false, "public", vector<string>());
		::Uml::InitAttributeProps(SignalType::meta_mtypestr, "mtypestr", "String", false, false, 1, 1, false, "public", vector<string>());
		::Uml::InitAttributeProps(SignalType::meta_ctypestr, "ctypestr", "String", false, false, 1, 1, false, "public", vector<string>());
		::Uml::InitAttributeProps(SignalType::meta_rows, "rows", "String", false, false, 1, 1, false, "public", vector<string>());
		::Uml::InitAttributeProps(SignalType::meta_columns, "columns", "String", false, false, 1, 1, false, "public", vector<string>());
		::Uml::InitAttributeProps(SignalType::meta_isStruct, "isStruct", "Boolean", false, false, 1, 1, false, "public", vector<string>());

		::Uml::InitClassProps(Sink::meta, "Sink", false, NULL, NULL);

		::Uml::InitClassProps(Source::meta, "Source", false, NULL, NULL);

		::Uml::InitClassProps(TTBus::meta, "TTBus", false, NULL, NULL);
		::Uml::InitAttributeProps(TTBus::meta_hyperperiodticks, "hyperperiodticks", "Integer", false, false, 1, 1, false, "public", vector<string>());
		::Uml::InitAttributeProps(TTBus::meta_hyperperiodsecs, "hyperperiodsecs", "Real", false, false, 1, 1, false, "public", vector<string>());
		::Uml::InitAttributeProps(TTBus::meta_setupsecs, "setupsecs", "Real", false, false, 1, 1, false, "public", vector<string>());
		::Uml::InitAttributeProps(TTBus::meta_switchmembytes, "switchmembytes", "Integer", false, false, 1, 1, false, "public", vector<string>());
		::Uml::InitAttributeProps(TTBus::meta_minframebytes, "minframebytes", "Integer", false, false, 1, 1, false, "public", vector<string>());
		::Uml::InitAttributeProps(TTBus::meta_busslotbytes, "busslotbytes", "Integer", false, false, 1, 1, false, "public", vector<string>());

		::Uml::InitClassProps(TTSchedule::meta, "TTSchedule", false, NULL, NULL);
		::Uml::InitAttributeProps(TTSchedule::meta_ticksched, "ticksched", "String", false, false, 1, 1, false, "public", vector<string>());

		::Uml::InitClassProps(Task::meta, "Task", false, NULL, NULL);
		::Uml::InitAttributeProps(Task::meta_name, "name", "String", false, false, 1, 1, false, "public", vector<string>());
		::Uml::InitAttributeProps(Task::meta_id, "id", "Integer", false, false, 1, 1, false, "public", vector<string>());

		::Uml::InitClassProps(TestPoint::meta, "TestPoint", false, NULL, NULL);
		::Uml::InitAttributeProps(TestPoint::meta_name, "name", "String", false, false, 1, 1, false, "public", vector<string>());
		::Uml::InitAttributeProps(TestPoint::meta_id, "id", "Integer", false, false, 1, 1, false, "public", vector<string>());
		::Uml::InitAttributeProps(TestPoint::meta_config, "config", "String", false, false, 1, 1, false, "public", vector<string>());

		::Uml::InitClassProps(TimingConstraint::meta, "TimingConstraint", false, NULL, NULL);
		::Uml::InitAttributeProps(TimingConstraint::meta_timeboundsecs, "timeboundsecs", "Real", false, false, 1, 1, false, "public", vector<string>());
		::Uml::InitAttributeProps(TimingConstraint::meta_id, "id", "String", false, false, 1, 1, false, "public", vector<string>());

		::Uml::InitClassProps(Transmits::meta, "Transmits", false, NULL, NULL);

		// associations
		{
			::Uml::Association ass = ::Uml::Association::Create(meta);
			::Uml::InitAssociationProps(ass, "Association");
			Executable::meta_info = ::Uml::AssociationRole::Create(ass);
			::Uml::InitAssociationRoleProps(Executable::meta_info, "info", true, false, 0, -1);
			ExecInfo::meta_executor = ::Uml::AssociationRole::Create(ass);
			::Uml::InitAssociationRoleProps(ExecInfo::meta_executor, "executor", true, false, 0, -1);

		}
		{
			::Uml::Association ass = ::Uml::Association::Create(meta);
			::Uml::InitAssociationProps(ass, "Association");
			Device::meta_integratedBy = ::Uml::AssociationRole::Create(ass);
			::Uml::InitAssociationRoleProps(Device::meta_integratedBy, "integratedBy", true, false, 0, 1);
			Node::meta_integrates = ::Uml::AssociationRole::Create(ass);
			::Uml::InitAssociationRoleProps(Node::meta_integrates, "integrates", true, false, 0, -1);

		}
		{
			::Uml::Association ass = ::Uml::Association::Create(meta);
			::Uml::InitAssociationProps(ass, "Association");
			DummyDependency::meta_dummyMsg = ::Uml::AssociationRole::Create(ass);
			::Uml::InitAssociationRoleProps(DummyDependency::meta_dummyMsg, "dummyMsg", true, false, 0, 1);
			Msg::meta_dummyMsgDeps = ::Uml::AssociationRole::Create(ass);
			::Uml::InitAssociationRoleProps(Msg::meta_dummyMsgDeps, "dummyMsgDeps", true, false, 0, -1);

		}
		{
			::Uml::Association ass = ::Uml::Association::Create(meta);
			::Uml::InitAssociationProps(ass, "Association");
			LocalDependency::meta_depMsg = ::Uml::AssociationRole::Create(ass);
			::Uml::InitAssociationRoleProps(LocalDependency::meta_depMsg, "depMsg", true, false, 0, 1);
			Msg::meta_msgDep = ::Uml::AssociationRole::Create(ass);
			::Uml::InitAssociationRoleProps(Msg::meta_msgDep, "msgDep", true, false, 0, 1);

		}
		{
			::Uml::Association ass = ::Uml::Association::Create(meta);
			::Uml::InitAssociationProps(ass, "Association");
			Receives::meta_receivedMsg = ::Uml::AssociationRole::Create(ass);
			::Uml::InitAssociationRoleProps(Receives::meta_receivedMsg, "receivedMsg", true, false, 0, 1);
			Msg::meta_msgListeners = ::Uml::AssociationRole::Create(ass);
			::Uml::InitAssociationRoleProps(Msg::meta_msgListeners, "msgListeners", true, false, 0, -1);

		}
		{
			::Uml::Association ass = ::Uml::Association::Create(meta);
			::Uml::InitAssociationProps(ass, "Association");
			Transmits::meta_transmittedMsg = ::Uml::AssociationRole::Create(ass);
			::Uml::InitAssociationRoleProps(Transmits::meta_transmittedMsg, "transmittedMsg", true, false, 0, 1);
			Msg::meta_msgTransmitter = ::Uml::AssociationRole::Create(ass);
			::Uml::InitAssociationRoleProps(Msg::meta_msgTransmitter, "msgTransmitter", true, false, 0, 1);

		}
		{
			::Uml::Association ass = ::Uml::Association::Create(meta);
			::Uml::InitAssociationProps(ass, "Association");
			Actuation::meta_actuatingMsg = ::Uml::AssociationRole::Create(ass);
			::Uml::InitAssociationRoleProps(Actuation::meta_actuatingMsg, "actuatingMsg", true, false, 0, 1);
			Msg::meta_msgActuation = ::Uml::AssociationRole::Create(ass);
			::Uml::InitAssociationRoleProps(Msg::meta_msgActuation, "msgActuation", true, false, 0, 1);

		}
		{
			::Uml::Association ass = ::Uml::Association::Create(meta);
			::Uml::InitAssociationProps(ass, "Association");
			Acquisition::meta_acquiredMsg = ::Uml::AssociationRole::Create(ass);
			::Uml::InitAssociationRoleProps(Acquisition::meta_acquiredMsg, "acquiredMsg", true, false, 0, 1);
			Msg::meta_msgAcquisitions = ::Uml::AssociationRole::Create(ass);
			::Uml::InitAssociationRoleProps(Msg::meta_msgAcquisitions, "msgAcquisitions", true, false, 0, -1);

		}
		{
			::Uml::Association ass = ::Uml::Association::Create(meta);
			::Uml::InitAssociationProps(ass, "Association");
			Node::meta_nodeDummyMsg = ::Uml::AssociationRole::Create(ass);
			::Uml::InitAssociationRoleProps(Node::meta_nodeDummyMsg, "nodeDummyMsg", true, false, 0, 1);
			Msg::meta_dummyMsgNode = ::Uml::AssociationRole::Create(ass);
			::Uml::InitAssociationRoleProps(Msg::meta_dummyMsgNode, "dummyMsgNode", true, false, 0, 1);

		}
		{
			::Uml::Association ass = ::Uml::Association::Create(meta);
			::Uml::InitAssociationProps(ass, "Association");
			Msg::meta_carries = ::Uml::AssociationRole::Create(ass);
			::Uml::InitAssociationRoleProps(Msg::meta_carries, "carries", true, false, 0, -1);
			Signal::meta_carriedBy = ::Uml::AssociationRole::Create(ass);
			::Uml::InitAssociationRoleProps(Signal::meta_carriedBy, "carriedBy", true, false, 0, -1);

		}
		{
			::Uml::Association ass = ::Uml::Association::Create(meta);
			::Uml::InitAssociationProps(ass, "Association");
			TimingConstraint::meta_sndTask = ::Uml::AssociationRole::Create(ass);
			::Uml::InitAssociationRoleProps(TimingConstraint::meta_sndTask, "sndTask", true, false, 0, 1);
			Task::meta_latencyAsSndr = ::Uml::AssociationRole::Create(ass);
			::Uml::InitAssociationRoleProps(Task::meta_latencyAsSndr, "latencyAsSndr", true, false, 0, -1);

		}
		{
			::Uml::Association ass = ::Uml::Association::Create(meta);
			::Uml::InitAssociationProps(ass, "Association");
			DummyDependency::meta_sendingTask = ::Uml::AssociationRole::Create(ass);
			::Uml::InitAssociationRoleProps(DummyDependency::meta_sendingTask, "sendingTask", true, false, 0, 1);
			Task::meta_dummyDeps = ::Uml::AssociationRole::Create(ass);
			::Uml::InitAssociationRoleProps(Task::meta_dummyDeps, "dummyDeps", true, false, 0, -1);

		}
		{
			::Uml::Association ass = ::Uml::Association::Create(meta);
			::Uml::InitAssociationProps(ass, "Association");
			LocalDependency::meta_localDepReceivingTasks = ::Uml::AssociationRole::Create(ass);
			::Uml::InitAssociationRoleProps(LocalDependency::meta_localDepReceivingTasks, "localDepReceivingTasks", true, false, 0, -1);
			Task::meta_localReceivingTaskDeps = ::Uml::AssociationRole::Create(ass);
			::Uml::InitAssociationRoleProps(Task::meta_localReceivingTaskDeps, "localReceivingTaskDeps", true, false, 0, -1);

		}
		{
			::Uml::Association ass = ::Uml::Association::Create(meta);
			::Uml::InitAssociationProps(ass, "Association");
			LocalDependency::meta_localDepSendingTask = ::Uml::AssociationRole::Create(ass);
			::Uml::InitAssociationRoleProps(LocalDependency::meta_localDepSendingTask, "localDepSendingTask", true, false, 0, 1);
			Task::meta_localSendingTaskDeps = ::Uml::AssociationRole::Create(ass);
			::Uml::InitAssociationRoleProps(Task::meta_localSendingTaskDeps, "localSendingTaskDeps", true, false, 0, -1);

		}
		{
			::Uml::Association ass = ::Uml::Association::Create(meta);
			::Uml::InitAssociationProps(ass, "Association");
			Receives::meta_receivingTask = ::Uml::AssociationRole::Create(ass);
			::Uml::InitAssociationRoleProps(Receives::meta_receivingTask, "receivingTask", true, false, 0, 1);
			Task::meta_incomingData = ::Uml::AssociationRole::Create(ass);
			::Uml::InitAssociationRoleProps(Task::meta_incomingData, "incomingData", true, false, 0, -1);

		}
		{
			::Uml::Association ass = ::Uml::Association::Create(meta);
			::Uml::InitAssociationProps(ass, "Association");
			Actuation::meta_actuatingTask = ::Uml::AssociationRole::Create(ass);
			::Uml::InitAssociationRoleProps(Actuation::meta_actuatingTask, "actuatingTask", true, false, 0, 1);
			Task::meta_taskActuations = ::Uml::AssociationRole::Create(ass);
			::Uml::InitAssociationRoleProps(Task::meta_taskActuations, "taskActuations", true, false, 0, -1);

		}
		{
			::Uml::Association ass = ::Uml::Association::Create(meta);
			::Uml::InitAssociationProps(ass, "Association");
			Acquisition::meta_acquiringTasks = ::Uml::AssociationRole::Create(ass);
			::Uml::InitAssociationRoleProps(Acquisition::meta_acquiringTasks, "acquiringTasks", true, false, 0, -1);
			Task::meta_taskAcquisitions = ::Uml::AssociationRole::Create(ass);
			::Uml::InitAssociationRoleProps(Task::meta_taskAcquisitions, "taskAcquisitions", true, false, 0, -1);

		}
		{
			::Uml::Association ass = ::Uml::Association::Create(meta);
			::Uml::InitAssociationProps(ass, "Association");
			Node::meta_executes = ::Uml::AssociationRole::Create(ass);
			::Uml::InitAssociationRoleProps(Node::meta_executes, "executes", true, false, 0, -1);
			Task::meta_executedBy = ::Uml::AssociationRole::Create(ass);
			::Uml::InitAssociationRoleProps(Task::meta_executedBy, "executedBy", true, false, 0, 1);

		}
		{
			::Uml::Association ass = ::Uml::Association::Create(meta);
			::Uml::InitAssociationProps(ass, "Association");
			Task::meta_invokes = ::Uml::AssociationRole::Create(ass);
			::Uml::InitAssociationRoleProps(Task::meta_invokes, "invokes", true, false, 0, -1);
			Component::meta_invokedBy = ::Uml::AssociationRole::Create(ass);
			::Uml::InitAssociationRoleProps(Component::meta_invokedBy, "invokedBy", true, false, 0, 1);

		}
		{
			::Uml::Association ass = ::Uml::Association::Create(meta);
			::Uml::InitAssociationProps(ass, "Association");
			Task::meta_outboundData = ::Uml::AssociationRole::Create(ass);
			::Uml::InitAssociationRoleProps(Task::meta_outboundData, "outboundData", true, false, 0, -1);
			Transmits::meta_sendingTask = ::Uml::AssociationRole::Create(ass);
			::Uml::InitAssociationRoleProps(Transmits::meta_sendingTask, "sendingTask", true, false, 0, 1);

		}
		{
			::Uml::Association ass = ::Uml::Association::Create(meta);
			::Uml::InitAssociationProps(ass, "Association");
			Task::meta_latencyAsRcvr = ::Uml::AssociationRole::Create(ass);
			::Uml::InitAssociationRoleProps(Task::meta_latencyAsRcvr, "latencyAsRcvr", true, false, 0, -1);
			TimingConstraint::meta_rcvTask = ::Uml::AssociationRole::Create(ass);
			::Uml::InitAssociationRoleProps(TimingConstraint::meta_rcvTask, "rcvTask", true, false, 0, 1);

		}
		{
			::Uml::Association ass = ::Uml::Association::Create(meta);
			::Uml::InitAssociationProps(ass, "Association");
			DummyDependency::meta_dummyDepNode = ::Uml::AssociationRole::Create(ass);
			::Uml::InitAssociationRoleProps(DummyDependency::meta_dummyDepNode, "dummyDepNode", true, false, 0, 1);
			Node::meta_nodeDummyDeps = ::Uml::AssociationRole::Create(ass);
			::Uml::InitAssociationRoleProps(Node::meta_nodeDummyDeps, "nodeDummyDeps", true, false, 0, -1);

		}
		{
			::Uml::Association ass = ::Uml::Association::Create(meta);
			::Uml::InitAssociationProps(ass, "Association");
			LocalDependency::meta_localDepNode = ::Uml::AssociationRole::Create(ass);
			::Uml::InitAssociationRoleProps(LocalDependency::meta_localDepNode, "localDepNode", true, false, 0, 1);
			Node::meta_nodeLocalDeps = ::Uml::AssociationRole::Create(ass);
			::Uml::InitAssociationRoleProps(Node::meta_nodeLocalDeps, "nodeLocalDeps", true, false, 0, -1);

		}
		{
			::Uml::Association ass = ::Uml::Association::Create(meta);
			::Uml::InitAssociationProps(ass, "Association");
			Sink::meta_measures = ::Uml::AssociationRole::Create(ass);
			::Uml::InitAssociationRoleProps(Sink::meta_measures, "measures", true, false, 0, -1);
			Signal::meta_measuredBy = ::Uml::AssociationRole::Create(ass);
			::Uml::InitAssociationRoleProps(Signal::meta_measuredBy, "measuredBy", true, false, 0, -1);

		}
		{
			::Uml::Association ass = ::Uml::Association::Create(meta);
			::Uml::InitAssociationProps(ass, "Association");
			Source::meta_creates = ::Uml::AssociationRole::Create(ass);
			::Uml::InitAssociationRoleProps(Source::meta_creates, "creates", true, false, 0, -1);
			Signal::meta_createdBy = ::Uml::AssociationRole::Create(ass);
			::Uml::InitAssociationRoleProps(Signal::meta_createdBy, "createdBy", true, false, 0, -1);

		}
		{
			::Uml::Association ass = ::Uml::Association::Create(meta);
			::Uml::InitAssociationProps(ass, "Association");
			OutputDevice::meta_outputEnvironment = ::Uml::AssociationRole::Create(ass);
			::Uml::InitAssociationRoleProps(OutputDevice::meta_outputEnvironment, "outputEnvironment", true, false, 0, 1);
			Plant::meta_actuationDevices = ::Uml::AssociationRole::Create(ass);
			::Uml::InitAssociationRoleProps(Plant::meta_actuationDevices, "actuationDevices", true, false, 0, -1);

		}
		{
			::Uml::Association ass = ::Uml::Association::Create(meta);
			::Uml::InitAssociationProps(ass, "Association");
			OChan::meta_ochanDevice = ::Uml::AssociationRole::Create(ass);
			::Uml::InitAssociationRoleProps(OChan::meta_ochanDevice, "ochanDevice", true, false, 0, 1);
			OutputDevice::meta_outputChannels = ::Uml::AssociationRole::Create(ass);
			::Uml::InitAssociationRoleProps(OutputDevice::meta_outputChannels, "outputChannels", true, false, 0, -1);

		}
		{
			::Uml::Association ass = ::Uml::Association::Create(meta);
			::Uml::InitAssociationProps(ass, "Association");
			InputDevice::meta_inputEnvironment = ::Uml::AssociationRole::Create(ass);
			::Uml::InitAssociationRoleProps(InputDevice::meta_inputEnvironment, "inputEnvironment", true, false, 0, 1);
			Plant::meta_acquisitionDevices = ::Uml::AssociationRole::Create(ass);
			::Uml::InitAssociationRoleProps(Plant::meta_acquisitionDevices, "acquisitionDevices", true, false, 0, -1);

		}
		{
			::Uml::Association ass = ::Uml::Association::Create(meta);
			::Uml::InitAssociationProps(ass, "Association");
			IChan::meta_ichanDevice = ::Uml::AssociationRole::Create(ass);
			::Uml::InitAssociationRoleProps(IChan::meta_ichanDevice, "ichanDevice", true, false, 0, 1);
			InputDevice::meta_inputChannels = ::Uml::AssociationRole::Create(ass);
			::Uml::InitAssociationRoleProps(InputDevice::meta_inputChannels, "inputChannels", true, false, 0, -1);

		}
		{
			::Uml::Association ass = ::Uml::Association::Create(meta);
			::Uml::InitAssociationProps(ass, "Association");
			CommInterface::meta_commMedium = ::Uml::AssociationRole::Create(ass);
			::Uml::InitAssociationRoleProps(CommInterface::meta_commMedium, "commMedium", true, false, 0, 1);
			CommMedium::meta_interfaces = ::Uml::AssociationRole::Create(ass);
			::Uml::InitAssociationRoleProps(CommMedium::meta_interfaces, "interfaces", true, false, 0, -1);

		}
		{
			::Uml::Association ass = ::Uml::Association::Create(meta);
			::Uml::InitAssociationProps(ass, "Association");
			BChan::meta_chanInterface = ::Uml::AssociationRole::Create(ass);
			::Uml::InitAssociationRoleProps(BChan::meta_chanInterface, "chanInterface", true, false, 0, 1);
			CommInterface::meta_commChannels = ::Uml::AssociationRole::Create(ass);
			::Uml::InitAssociationRoleProps(CommInterface::meta_commChannels, "commChannels", true, false, 0, -1);

		}
		{
			::Uml::Association ass = ::Uml::Association::Create(meta);
			::Uml::InitAssociationProps(ass, "Association");
			Receives::meta_receivingChannel = ::Uml::AssociationRole::Create(ass);
			::Uml::InitAssociationRoleProps(Receives::meta_receivingChannel, "receivingChannel", true, false, 0, 1);
			BChan::meta_channelListeners = ::Uml::AssociationRole::Create(ass);
			::Uml::InitAssociationRoleProps(BChan::meta_channelListeners, "channelListeners", true, false, 0, -1);

		}
		{
			::Uml::Association ass = ::Uml::Association::Create(meta);
			::Uml::InitAssociationProps(ass, "Association");
			Transmits::meta_sendingChan = ::Uml::AssociationRole::Create(ass);
			::Uml::InitAssociationRoleProps(Transmits::meta_sendingChan, "sendingChan", true, false, 0, 1);
			BChan::meta_channelTransmitters = ::Uml::AssociationRole::Create(ass);
			::Uml::InitAssociationRoleProps(BChan::meta_channelTransmitters, "channelTransmitters", true, false, 0, -1);

		}
		{
			::Uml::Association ass = ::Uml::Association::Create(meta);
			::Uml::InitAssociationProps(ass, "Association");
			Actuation::meta_actuatingChannel = ::Uml::AssociationRole::Create(ass);
			::Uml::InitAssociationRoleProps(Actuation::meta_actuatingChannel, "actuatingChannel", true, false, 0, 1);
			OChan::meta_channelActuations = ::Uml::AssociationRole::Create(ass);
			::Uml::InitAssociationRoleProps(OChan::meta_channelActuations, "channelActuations", true, false, 0, -1);

		}
		{
			::Uml::Association ass = ::Uml::Association::Create(meta);
			::Uml::InitAssociationProps(ass, "Association");
			Acquisition::meta_acquiringChannel = ::Uml::AssociationRole::Create(ass);
			::Uml::InitAssociationRoleProps(Acquisition::meta_acquiringChannel, "acquiringChannel", true, false, 0, 1);
			IChan::meta_channelAcquisition = ::Uml::AssociationRole::Create(ass);
			::Uml::InitAssociationRoleProps(IChan::meta_channelAcquisition, "channelAcquisition", true, false, 0, 1);

		}
		{
			::Uml::Association ass = ::Uml::Association::Create(meta);
			::Uml::InitAssociationProps(ass, "Association");
			Signal::meta_isOfType = ::Uml::AssociationRole::Create(ass);
			::Uml::InitAssociationRoleProps(Signal::meta_isOfType, "isOfType", true, false, 0, 1);
			SignalType::meta_signalsOfType = ::Uml::AssociationRole::Create(ass);
			::Uml::InitAssociationRoleProps(SignalType::meta_signalsOfType, "signalsOfType", true, false, 0, -1);

		}
		{
			::Uml::Association ass = ::Uml::Association::Create(meta);
			::Uml::InitAssociationProps(ass, "Association");
			Signal::meta_consumedBy = ::Uml::AssociationRole::Create(ass);
			::Uml::InitAssociationRoleProps(Signal::meta_consumedBy, "consumedBy", true, false, 0, -1);
			Component::meta_consumes = ::Uml::AssociationRole::Create(ass);
			::Uml::InitAssociationRoleProps(Component::meta_consumes, "consumes", true, false, 0, -1);

		}
		{
			::Uml::Association ass = ::Uml::Association::Create(meta);
			::Uml::InitAssociationProps(ass, "Association");
			Component::meta_generates = ::Uml::AssociationRole::Create(ass);
			::Uml::InitAssociationRoleProps(Component::meta_generates, "generates", true, false, 0, -1);
			Signal::meta_generatedBy = ::Uml::AssociationRole::Create(ass);
			::Uml::InitAssociationRoleProps(Signal::meta_generatedBy, "generatedBy", true, false, 0, 1);

		}

		// compositions
		{
			::Uml::Composition comp = ::Uml::Composition::Create(meta);
			::Uml::InitCompositionProps(comp, "Composition", false);
			Schedule::meta_ExecInfo_parent = ::Uml::CompositionParentRole::Create(comp);
			::Uml::InitCompositionParentRoleProps(Schedule::meta_ExecInfo_parent, "ExecInfo_parent", true);
			ExecInfo::meta_Schedule_child = ::Uml::CompositionChildRole::Create(comp);
			::Uml::InitCompositionChildRoleProps(ExecInfo::meta_Schedule_child, "Schedule", true, 0, 1);

		}
		{
			::Uml::Composition comp = ::Uml::Composition::Create(meta);
			::Uml::InitCompositionProps(comp, "Composition", false);
			Periodicity::meta_ExecInfo_parent = ::Uml::CompositionParentRole::Create(comp);
			::Uml::InitCompositionParentRoleProps(Periodicity::meta_ExecInfo_parent, "ExecInfo_parent", true);
			ExecInfo::meta_Periodicity_children = ::Uml::CompositionChildRole::Create(comp);
			::Uml::InitCompositionChildRoleProps(ExecInfo::meta_Periodicity_children, "Periodicity", true, 0, -1);

		}
		{
			::Uml::Composition comp = ::Uml::Composition::Create(meta);
			::Uml::InitCompositionProps(comp, "Composition", false);
			Duration::meta_ExecInfo_parent = ::Uml::CompositionParentRole::Create(comp);
			::Uml::InitCompositionParentRoleProps(Duration::meta_ExecInfo_parent, "ExecInfo_parent", true);
			ExecInfo::meta_Duration_child = ::Uml::CompositionChildRole::Create(comp);
			::Uml::InitCompositionChildRoleProps(ExecInfo::meta_Duration_child, "Duration", true, 0, 1);

		}
		{
			::Uml::Composition comp = ::Uml::Composition::Create(meta);
			::Uml::InitCompositionProps(comp, "Composition", false);
			RootFolder::meta_RootFolder_parent = ::Uml::CompositionParentRole::Create(comp);
			::Uml::InitCompositionParentRoleProps(RootFolder::meta_RootFolder_parent, "RootFolder_parent", true);
			RootFolder::meta_RootFolder_children = ::Uml::CompositionChildRole::Create(comp);
			::Uml::InitCompositionChildRoleProps(RootFolder::meta_RootFolder_children, "RootFolder", true, 0, -1);

		}
		{
			::Uml::Composition comp = ::Uml::Composition::Create(meta);
			::Uml::InitCompositionProps(comp, "Composition", false);
			Deployment::meta_RootFolder_parent = ::Uml::CompositionParentRole::Create(comp);
			::Uml::InitCompositionParentRoleProps(Deployment::meta_RootFolder_parent, "RootFolder_parent", true);
			RootFolder::meta_Deployment_children = ::Uml::CompositionChildRole::Create(comp);
			::Uml::InitCompositionChildRoleProps(RootFolder::meta_Deployment_children, "Deployment", true, 0, -1);

		}
		{
			::Uml::Composition comp = ::Uml::Composition::Create(meta);
			::Uml::InitCompositionProps(comp, "Composition", false);
			SignalType::meta_Deployment_parent = ::Uml::CompositionParentRole::Create(comp);
			::Uml::InitCompositionParentRoleProps(SignalType::meta_Deployment_parent, "Deployment_parent", true);
			Deployment::meta_SignalType_children = ::Uml::CompositionChildRole::Create(comp);
			::Uml::InitCompositionChildRoleProps(Deployment::meta_SignalType_children, "SignalType", true, 0, -1);

		}
		{
			::Uml::Composition comp = ::Uml::Composition::Create(meta);
			::Uml::InitCompositionProps(comp, "Composition", false);
			TestPoint::meta_Deployment_parent = ::Uml::CompositionParentRole::Create(comp);
			::Uml::InitCompositionParentRoleProps(TestPoint::meta_Deployment_parent, "Deployment_parent", true);
			Deployment::meta_TestPoint_children = ::Uml::CompositionChildRole::Create(comp);
			::Uml::InitCompositionChildRoleProps(Deployment::meta_TestPoint_children, "TestPoint", true, 0, -1);

		}
		{
			::Uml::Composition comp = ::Uml::Composition::Create(meta);
			::Uml::InitCompositionProps(comp, "Composition", false);
			CommRelations::meta_Deployment_parent = ::Uml::CompositionParentRole::Create(comp);
			::Uml::InitCompositionParentRoleProps(CommRelations::meta_Deployment_parent, "Deployment_parent", true);
			Deployment::meta_CommRelations_children = ::Uml::CompositionChildRole::Create(comp);
			::Uml::InitCompositionChildRoleProps(Deployment::meta_CommRelations_children, "CommRelations", true, 0, -1);

		}
		{
			::Uml::Composition comp = ::Uml::Composition::Create(meta);
			::Uml::InitCompositionProps(comp, "Composition", false);
			Channel::meta_Deployment_parent = ::Uml::CompositionParentRole::Create(comp);
			::Uml::InitCompositionParentRoleProps(Channel::meta_Deployment_parent, "Deployment_parent", true);
			Deployment::meta_Channel_children = ::Uml::CompositionChildRole::Create(comp);
			::Uml::InitCompositionChildRoleProps(Deployment::meta_Channel_children, "Channel", true, 0, -1);

		}
		{
			::Uml::Composition comp = ::Uml::Composition::Create(meta);
			::Uml::InitCompositionProps(comp, "Composition", false);
			Plant::meta_Deployment_parent = ::Uml::CompositionParentRole::Create(comp);
			::Uml::InitCompositionParentRoleProps(Plant::meta_Deployment_parent, "Deployment_parent", true);
			Deployment::meta_Plant_children = ::Uml::CompositionChildRole::Create(comp);
			::Uml::InitCompositionChildRoleProps(Deployment::meta_Plant_children, "Plant", true, 0, -1);

		}
		{
			::Uml::Composition comp = ::Uml::Composition::Create(meta);
			::Uml::InitCompositionProps(comp, "Composition", false);
			Msg::meta_Deployment_parent = ::Uml::CompositionParentRole::Create(comp);
			::Uml::InitCompositionParentRoleProps(Msg::meta_Deployment_parent, "Deployment_parent", true);
			Deployment::meta_Msg_children = ::Uml::CompositionChildRole::Create(comp);
			::Uml::InitCompositionChildRoleProps(Deployment::meta_Msg_children, "Msg", true, 0, -1);

		}
		{
			::Uml::Composition comp = ::Uml::Composition::Create(meta);
			::Uml::InitCompositionProps(comp, "Composition", false);
			CommMedium::meta_Deployment_parent = ::Uml::CompositionParentRole::Create(comp);
			::Uml::InitCompositionParentRoleProps(CommMedium::meta_Deployment_parent, "Deployment_parent", true);
			Deployment::meta_CommMedium_children = ::Uml::CompositionChildRole::Create(comp);
			::Uml::InitCompositionChildRoleProps(Deployment::meta_CommMedium_children, "CommMedium", true, 0, -1);

		}
		{
			::Uml::Composition comp = ::Uml::Composition::Create(meta);
			::Uml::InitCompositionProps(comp, "Composition", false);
			Task::meta_Deployment_parent = ::Uml::CompositionParentRole::Create(comp);
			::Uml::InitCompositionParentRoleProps(Task::meta_Deployment_parent, "Deployment_parent", true);
			Deployment::meta_Task_children = ::Uml::CompositionChildRole::Create(comp);
			::Uml::InitCompositionChildRoleProps(Deployment::meta_Task_children, "Task", true, 0, -1);

		}
		{
			::Uml::Composition comp = ::Uml::Composition::Create(meta);
			::Uml::InitCompositionProps(comp, "Composition", false);
			Signal::meta_Deployment_parent = ::Uml::CompositionParentRole::Create(comp);
			::Uml::InitCompositionParentRoleProps(Signal::meta_Deployment_parent, "Deployment_parent", true);
			Deployment::meta_Signal_children = ::Uml::CompositionChildRole::Create(comp);
			::Uml::InitCompositionChildRoleProps(Deployment::meta_Signal_children, "Signal", true, 0, -1);

		}
		{
			::Uml::Composition comp = ::Uml::Composition::Create(meta);
			::Uml::InitCompositionProps(comp, "Composition", false);
			Device::meta_Deployment_parent = ::Uml::CompositionParentRole::Create(comp);
			::Uml::InitCompositionParentRoleProps(Device::meta_Deployment_parent, "Deployment_parent", true);
			Deployment::meta_Device_children = ::Uml::CompositionChildRole::Create(comp);
			::Uml::InitCompositionChildRoleProps(Deployment::meta_Device_children, "Device", true, 0, -1);

		}
		{
			::Uml::Composition comp = ::Uml::Composition::Create(meta);
			::Uml::InitCompositionProps(comp, "Composition", false);
			Component::meta_Deployment_parent = ::Uml::CompositionParentRole::Create(comp);
			::Uml::InitCompositionParentRoleProps(Component::meta_Deployment_parent, "Deployment_parent", true);
			Deployment::meta_Component_children = ::Uml::CompositionChildRole::Create(comp);
			::Uml::InitCompositionChildRoleProps(Deployment::meta_Component_children, "Component", true, 0, -1);

		}
		{
			::Uml::Composition comp = ::Uml::Composition::Create(meta);
			::Uml::InitCompositionProps(comp, "Composition", false);
			Node::meta_Deployment_parent = ::Uml::CompositionParentRole::Create(comp);
			::Uml::InitCompositionParentRoleProps(Node::meta_Deployment_parent, "Deployment_parent", true);
			Deployment::meta_Node_children = ::Uml::CompositionChildRole::Create(comp);
			::Uml::InitCompositionChildRoleProps(Deployment::meta_Node_children, "Node", true, 0, -1);

		}
		{
			::Uml::Composition comp = ::Uml::Composition::Create(meta);
			::Uml::InitCompositionProps(comp, "Composition", false);
			ExecInfo::meta_Deployment_parent = ::Uml::CompositionParentRole::Create(comp);
			::Uml::InitCompositionParentRoleProps(ExecInfo::meta_Deployment_parent, "Deployment_parent", true);
			Deployment::meta_ExecInfo_children = ::Uml::CompositionChildRole::Create(comp);
			::Uml::InitCompositionChildRoleProps(Deployment::meta_ExecInfo_children, "ExecInfo", true, 0, -1);

		}
		{
			::Uml::Composition comp = ::Uml::Composition::Create(meta);
			::Uml::InitCompositionProps(comp, "Composition", false);
			TimingConstraint::meta_Deployment_parent = ::Uml::CompositionParentRole::Create(comp);
			::Uml::InitCompositionParentRoleProps(TimingConstraint::meta_Deployment_parent, "Deployment_parent", true);
			Deployment::meta_TimingConstraint_children = ::Uml::CompositionChildRole::Create(comp);
			::Uml::InitCompositionChildRoleProps(Deployment::meta_TimingConstraint_children, "TimingConstraint", true, 0, -1);

		}

	}

	void InitMetaLinks() {
		Acquisition::meta_acquiringTasks.target() = Task::meta;
		Acquisition::meta_acquiredMsg.target() = Msg::meta;
		Acquisition::meta_acquiringChannel.target() = IChan::meta;

		Actuation::meta_actuatingChannel.target() = OChan::meta;
		Actuation::meta_actuatingMsg.target() = Msg::meta;
		Actuation::meta_actuatingTask.target() = Task::meta;

		BChan::meta_channelListeners.target() = Receives::meta;
		BChan::meta_channelTransmitters.target() = Transmits::meta;
		BChan::meta_chanInterface.target() = CommInterface::meta;

		Deployment::meta_Channel_children.target() = Channel::meta;
		Channel::meta.subTypes() += OChan::meta;
		Channel::meta.subTypes() += BChan::meta;
		Channel::meta.subTypes() += IChan::meta;

		CommInterface::meta_commMedium.target() = CommMedium::meta;
		CommInterface::meta_commChannels.target() = BChan::meta;

		CommMedium::meta_interfaces.target() = CommInterface::meta;
		Deployment::meta_CommMedium_children.target() = CommMedium::meta;
		CommMedium::meta.subTypes() += TTBus::meta;

		Deployment::meta_CommRelations_children.target() = CommRelations::meta;
		CommRelations::meta.subTypes() += DummyDependency::meta;
		CommRelations::meta.subTypes() += LocalDependency::meta;
		CommRelations::meta.subTypes() += Receives::meta;
		CommRelations::meta.subTypes() += Transmits::meta;
		CommRelations::meta.subTypes() += Actuation::meta;
		CommRelations::meta.subTypes() += Acquisition::meta;

		Component::meta_consumes.target() = Signal::meta;
		Component::meta_invokedBy.target() = Task::meta;
		Component::meta_generates.target() = Signal::meta;
		Deployment::meta_Component_children.target() = Component::meta;

		SignalType::meta_Deployment_parent.target() = Deployment::meta;
		TestPoint::meta_Deployment_parent.target() = Deployment::meta;
		CommRelations::meta_Deployment_parent.target() = Deployment::meta;
		Channel::meta_Deployment_parent.target() = Deployment::meta;
		Plant::meta_Deployment_parent.target() = Deployment::meta;
		Msg::meta_Deployment_parent.target() = Deployment::meta;
		CommMedium::meta_Deployment_parent.target() = Deployment::meta;
		Task::meta_Deployment_parent.target() = Deployment::meta;
		Signal::meta_Deployment_parent.target() = Deployment::meta;
		Device::meta_Deployment_parent.target() = Deployment::meta;
		Component::meta_Deployment_parent.target() = Deployment::meta;
		Node::meta_Deployment_parent.target() = Deployment::meta;
		ExecInfo::meta_Deployment_parent.target() = Deployment::meta;
		TimingConstraint::meta_Deployment_parent.target() = Deployment::meta;
		RootFolder::meta_Deployment_children.target() = Deployment::meta;

		Device::meta_integratedBy.target() = Node::meta;
		Deployment::meta_Device_children.target() = Device::meta;
		Device::meta.subTypes() += OutputDevice::meta;
		Device::meta.subTypes() += InputDevice::meta;
		Device::meta.subTypes() += CommInterface::meta;

		DummyDependency::meta_sendingTask.target() = Task::meta;
		DummyDependency::meta_dummyMsg.target() = Msg::meta;
		DummyDependency::meta_dummyDepNode.target() = Node::meta;

		ExecInfo::meta_Duration_child.target() = Duration::meta;
		Duration::meta.subTypes() += MaxDuration::meta;

		ExecInfo::meta_executor.target() = Executable::meta;
		Schedule::meta_ExecInfo_parent.target() = ExecInfo::meta;
		Periodicity::meta_ExecInfo_parent.target() = ExecInfo::meta;
		Duration::meta_ExecInfo_parent.target() = ExecInfo::meta;
		Deployment::meta_ExecInfo_children.target() = ExecInfo::meta;

		Executable::meta_info.target() = ExecInfo::meta;
		Executable::meta.subTypes() += Channel::meta;
		Executable::meta.subTypes() += Device::meta;
		Executable::meta.subTypes() += Msg::meta;
		Executable::meta.subTypes() += Task::meta;

		IChan::meta_channelAcquisition.target() = Acquisition::meta;
		IChan::meta_ichanDevice.target() = InputDevice::meta;

		InputDevice::meta_inputEnvironment.target() = Plant::meta;
		InputDevice::meta_inputChannels.target() = IChan::meta;

		LocalDependency::meta_localDepNode.target() = Node::meta;
		LocalDependency::meta_localDepReceivingTasks.target() = Task::meta;
		LocalDependency::meta_localDepSendingTask.target() = Task::meta;
		LocalDependency::meta_depMsg.target() = Msg::meta;

		Msg::meta_dummyMsgDeps.target() = DummyDependency::meta;
		Msg::meta_msgDep.target() = LocalDependency::meta;
		Msg::meta_msgListeners.target() = Receives::meta;
		Msg::meta_msgTransmitter.target() = Transmits::meta;
		Msg::meta_msgActuation.target() = Actuation::meta;
		Msg::meta_msgAcquisitions.target() = Acquisition::meta;
		Msg::meta_dummyMsgNode.target() = Node::meta;
		Msg::meta_carries.target() = Signal::meta;
		Deployment::meta_Msg_children.target() = Msg::meta;

		Node::meta_nodeDummyDeps.target() = DummyDependency::meta;
		Node::meta_nodeLocalDeps.target() = LocalDependency::meta;
		Node::meta_nodeDummyMsg.target() = Msg::meta;
		Node::meta_executes.target() = Task::meta;
		Node::meta_integrates.target() = Device::meta;
		Deployment::meta_Node_children.target() = Node::meta;

		OChan::meta_channelActuations.target() = Actuation::meta;
		OChan::meta_ochanDevice.target() = OutputDevice::meta;

		OutputDevice::meta_outputEnvironment.target() = Plant::meta;
		OutputDevice::meta_outputChannels.target() = OChan::meta;

		ExecInfo::meta_Periodicity_children.target() = Periodicity::meta;
		Periodicity::meta.subTypes() += SamplePeriod::meta;
		Periodicity::meta.subTypes() += MinimumPeriod::meta;

		Plant::meta_actuationDevices.target() = OutputDevice::meta;
		Plant::meta_acquisitionDevices.target() = InputDevice::meta;
		Deployment::meta_Plant_children.target() = Plant::meta;

		Receives::meta_receivedMsg.target() = Msg::meta;
		Receives::meta_receivingChannel.target() = BChan::meta;
		Receives::meta_receivingTask.target() = Task::meta;

		RootFolder::meta_RootFolder_parent.target() = RootFolder::meta;
		Deployment::meta_RootFolder_parent.target() = RootFolder::meta;
		RootFolder::meta_RootFolder_children.target() = RootFolder::meta;

		ExecInfo::meta_Schedule_child.target() = Schedule::meta;
		Schedule::meta.subTypes() += TTSchedule::meta;

		Signal::meta_createdBy.target() = Source::meta;
		Signal::meta_measuredBy.target() = Sink::meta;
		Signal::meta_carriedBy.target() = Msg::meta;
		Signal::meta_isOfType.target() = SignalType::meta;
		Signal::meta_consumedBy.target() = Component::meta;
		Signal::meta_generatedBy.target() = Component::meta;
		Deployment::meta_Signal_children.target() = Signal::meta;

		SignalType::meta_signalsOfType.target() = Signal::meta;
		Deployment::meta_SignalType_children.target() = SignalType::meta;

		Sink::meta_measures.target() = Signal::meta;

		Source::meta_creates.target() = Signal::meta;

		Task::meta_latencyAsSndr.target() = TimingConstraint::meta;
		Task::meta_dummyDeps.target() = DummyDependency::meta;
		Task::meta_localReceivingTaskDeps.target() = LocalDependency::meta;
		Task::meta_localSendingTaskDeps.target() = LocalDependency::meta;
		Task::meta_incomingData.target() = Receives::meta;
		Task::meta_taskActuations.target() = Actuation::meta;
		Task::meta_taskAcquisitions.target() = Acquisition::meta;
		Task::meta_executedBy.target() = Node::meta;
		Task::meta_invokes.target() = Component::meta;
		Task::meta_outboundData.target() = Transmits::meta;
		Task::meta_latencyAsRcvr.target() = TimingConstraint::meta;
		Deployment::meta_Task_children.target() = Task::meta;

		Deployment::meta_TestPoint_children.target() = TestPoint::meta;
		TestPoint::meta.subTypes() += Sink::meta;
		TestPoint::meta.subTypes() += Source::meta;

		TimingConstraint::meta_sndTask.target() = Task::meta;
		TimingConstraint::meta_rcvTask.target() = Task::meta;
		Deployment::meta_TimingConstraint_children.target() = TimingConstraint::meta;

		Transmits::meta_transmittedMsg.target() = Msg::meta;
		Transmits::meta_sendingChan.target() = BChan::meta;
		Transmits::meta_sendingTask.target() = Task::meta;

	}

	void InitMeta(const ::Uml::Diagram &parent) {
		// classes, with attributes, constraints and constraint definitions
		::Uml::SetClass(Acquisition::meta, parent, "Acquisition");

		::Uml::SetClass(Actuation::meta, parent, "Actuation");

		::Uml::SetClass(BChan::meta, parent, "BChan");

		::Uml::SetClass(Channel::meta, parent, "Channel");
		::Uml::SetAttribute(Channel::meta_name, Channel::meta, "name");
		::Uml::SetAttribute(Channel::meta_id, Channel::meta, "id");
		::Uml::SetAttribute(Channel::meta_config, Channel::meta, "config");

		::Uml::SetClass(CommInterface::meta, parent, "CommInterface");

		::Uml::SetClass(CommMedium::meta, parent, "CommMedium");
		::Uml::SetAttribute(CommMedium::meta_name, CommMedium::meta, "name");
		::Uml::SetAttribute(CommMedium::meta_id, CommMedium::meta, "id");
		::Uml::SetAttribute(CommMedium::meta_dataratebps, CommMedium::meta, "dataratebps");

		::Uml::SetClass(CommRelations::meta, parent, "CommRelations");
		::Uml::SetAttribute(CommRelations::meta_id, CommRelations::meta, "id");

		::Uml::SetClass(Component::meta, parent, "Component");
		::Uml::SetAttribute(Component::meta_name, Component::meta, "name");
		::Uml::SetAttribute(Component::meta_id, Component::meta, "id");
		::Uml::SetAttribute(Component::meta_msubsystem, Component::meta, "msubsystem");
		::Uml::SetAttribute(Component::meta_ccode, Component::meta, "ccode");
		::Uml::SetAttribute(Component::meta_hfiles, Component::meta, "hfiles");
		::Uml::SetAttribute(Component::meta_cfiles, Component::meta, "cfiles");

		::Uml::SetClass(Deployment::meta, parent, "Deployment");
		::Uml::SetAttribute(Deployment::meta_id, Deployment::meta, "id");
		::Uml::SetAttribute(Deployment::meta_name, Deployment::meta, "name");

		::Uml::SetClass(Device::meta, parent, "Device");
		::Uml::SetAttribute(Device::meta_name, Device::meta, "name");
		::Uml::SetAttribute(Device::meta_id, Device::meta, "id");
		::Uml::SetAttribute(Device::meta_devicetype, Device::meta, "devicetype");
		::Uml::SetAttribute(Device::meta_config, Device::meta, "config");

		::Uml::SetClass(DummyDependency::meta, parent, "DummyDependency");

		::Uml::SetClass(Duration::meta, parent, "Duration");
		::Uml::SetAttribute(Duration::meta_exectimesecs, Duration::meta, "exectimesecs");

		::Uml::SetClass(ExecInfo::meta, parent, "ExecInfo");
		::Uml::SetAttribute(ExecInfo::meta_name, ExecInfo::meta, "name");

		::Uml::SetClass(Executable::meta, parent, "Executable");

		::Uml::SetClass(IChan::meta, parent, "IChan");

		::Uml::SetClass(InputDevice::meta, parent, "InputDevice");

		::Uml::SetClass(LocalDependency::meta, parent, "LocalDependency");

		::Uml::SetClass(MaxDuration::meta, parent, "MaxDuration");

		::Uml::SetClass(MinimumPeriod::meta, parent, "MinimumPeriod");

		::Uml::SetClass(Msg::meta, parent, "Msg");
		::Uml::SetAttribute(Msg::meta_name, Msg::meta, "name");
		::Uml::SetAttribute(Msg::meta_id, Msg::meta, "id");
		::Uml::SetAttribute(Msg::meta_sizebytes, Msg::meta, "sizebytes");
		::Uml::SetAttribute(Msg::meta_messagetype, Msg::meta, "messagetype");

		::Uml::SetClass(Node::meta, parent, "Node");
		::Uml::SetAttribute(Node::meta_name, Node::meta, "name");
		::Uml::SetAttribute(Node::meta_id, Node::meta, "id");
		::Uml::SetAttribute(Node::meta_platformtype, Node::meta, "platformtype");
		::Uml::SetAttribute(Node::meta_processormodel, Node::meta, "processormodel");
		::Uml::SetAttribute(Node::meta_hyperperiodsecs, Node::meta, "hyperperiodsecs");
		::Uml::SetAttribute(Node::meta_hyperperiodticks, Node::meta, "hyperperiodticks");
		::Uml::SetAttribute(Node::meta_clockspeedhz, Node::meta, "clockspeedhz");
		::Uml::SetAttribute(Node::meta_tickresolutionsecs, Node::meta, "tickresolutionsecs");
		::Uml::SetAttribute(Node::meta_ctxtswitchsecs, Node::meta, "ctxtswitchsecs");
		::Uml::SetAttribute(Node::meta_sndohdsecs, Node::meta, "sndohdsecs");
		::Uml::SetAttribute(Node::meta_rcvohdsecs, Node::meta, "rcvohdsecs");
		::Uml::SetAttribute(Node::meta_analog_inp_num, Node::meta, "analog_inp_num");
		::Uml::SetAttribute(Node::meta_analog_outp_num, Node::meta, "analog_outp_num");
		::Uml::SetAttribute(Node::meta_pp_directives, Node::meta, "pp_directives");

		::Uml::SetClass(OChan::meta, parent, "OChan");

		::Uml::SetClass(OutputDevice::meta, parent, "OutputDevice");

		::Uml::SetClass(Periodicity::meta, parent, "Periodicity");
		::Uml::SetAttribute(Periodicity::meta_periodsecs, Periodicity::meta, "periodsecs");

		::Uml::SetClass(Plant::meta, parent, "Plant");
		::Uml::SetAttribute(Plant::meta_name, Plant::meta, "name");
		::Uml::SetAttribute(Plant::meta_id, Plant::meta, "id");
		::Uml::SetAttribute(Plant::meta_config, Plant::meta, "config");

		::Uml::SetClass(Receives::meta, parent, "Receives");

		::Uml::SetClass(RootFolder::meta, parent, "RootFolder");
		::Uml::SetAttribute(RootFolder::meta_name, RootFolder::meta, "name");

		::Uml::SetClass(SamplePeriod::meta, parent, "SamplePeriod");

		::Uml::SetClass(Schedule::meta, parent, "Schedule");
		::Uml::SetAttribute(Schedule::meta_sched, Schedule::meta, "sched");

		::Uml::SetClass(Signal::meta, parent, "Signal");
		::Uml::SetAttribute(Signal::meta_callIndex, Signal::meta, "callIndex");
		::Uml::SetAttribute(Signal::meta_id, Signal::meta, "id");
		::Uml::SetAttribute(Signal::meta_msgIndex, Signal::meta, "msgIndex");
		::Uml::SetAttribute(Signal::meta_name, Signal::meta, "name");

		::Uml::SetClass(SignalType::meta, parent, "SignalType");
		::Uml::SetAttribute(SignalType::meta_name, SignalType::meta, "name");
		::Uml::SetAttribute(SignalType::meta_id, SignalType::meta, "id");
		::Uml::SetAttribute(SignalType::meta_mtypestr, SignalType::meta, "mtypestr");
		::Uml::SetAttribute(SignalType::meta_ctypestr, SignalType::meta, "ctypestr");
		::Uml::SetAttribute(SignalType::meta_rows, SignalType::meta, "rows");
		::Uml::SetAttribute(SignalType::meta_columns, SignalType::meta, "columns");
		::Uml::SetAttribute(SignalType::meta_isStruct, SignalType::meta, "isStruct");

		::Uml::SetClass(Sink::meta, parent, "Sink");

		::Uml::SetClass(Source::meta, parent, "Source");

		::Uml::SetClass(TTBus::meta, parent, "TTBus");
		::Uml::SetAttribute(TTBus::meta_hyperperiodticks, TTBus::meta, "hyperperiodticks");
		::Uml::SetAttribute(TTBus::meta_hyperperiodsecs, TTBus::meta, "hyperperiodsecs");
		::Uml::SetAttribute(TTBus::meta_setupsecs, TTBus::meta, "setupsecs");
		::Uml::SetAttribute(TTBus::meta_switchmembytes, TTBus::meta, "switchmembytes");
		::Uml::SetAttribute(TTBus::meta_minframebytes, TTBus::meta, "minframebytes");
		::Uml::SetAttribute(TTBus::meta_busslotbytes, TTBus::meta, "busslotbytes");

		::Uml::SetClass(TTSchedule::meta, parent, "TTSchedule");
		::Uml::SetAttribute(TTSchedule::meta_ticksched, TTSchedule::meta, "ticksched");

		::Uml::SetClass(Task::meta, parent, "Task");
		::Uml::SetAttribute(Task::meta_name, Task::meta, "name");
		::Uml::SetAttribute(Task::meta_id, Task::meta, "id");

		::Uml::SetClass(TestPoint::meta, parent, "TestPoint");
		::Uml::SetAttribute(TestPoint::meta_name, TestPoint::meta, "name");
		::Uml::SetAttribute(TestPoint::meta_id, TestPoint::meta, "id");
		::Uml::SetAttribute(TestPoint::meta_config, TestPoint::meta, "config");

		::Uml::SetClass(TimingConstraint::meta, parent, "TimingConstraint");
		::Uml::SetAttribute(TimingConstraint::meta_timeboundsecs, TimingConstraint::meta, "timeboundsecs");
		::Uml::SetAttribute(TimingConstraint::meta_id, TimingConstraint::meta, "id");

		::Uml::SetClass(Transmits::meta, parent, "Transmits");

	}

	void InitMetaLinks(const ::Uml::Diagram &parent) {
		// classes
		::Uml::SetAssocRole(Acquisition::meta_acquiringTasks, Acquisition::meta, Task::meta, "taskAcquisitions");
		::Uml::SetAssocRole(Acquisition::meta_acquiredMsg, Acquisition::meta, Msg::meta, "msgAcquisitions");
		::Uml::SetAssocRole(Acquisition::meta_acquiringChannel, Acquisition::meta, IChan::meta, "channelAcquisition");

		::Uml::SetAssocRole(Actuation::meta_actuatingChannel, Actuation::meta, OChan::meta, "channelActuations");
		::Uml::SetAssocRole(Actuation::meta_actuatingMsg, Actuation::meta, Msg::meta, "msgActuation");
		::Uml::SetAssocRole(Actuation::meta_actuatingTask, Actuation::meta, Task::meta, "taskActuations");

		::Uml::SetAssocRole(BChan::meta_channelListeners, BChan::meta, Receives::meta, "receivingChannel");
		::Uml::SetAssocRole(BChan::meta_channelTransmitters, BChan::meta, Transmits::meta, "sendingChan");
		::Uml::SetAssocRole(BChan::meta_chanInterface, BChan::meta, CommInterface::meta, "commChannels");

		::Uml::SetParentRole(Channel::meta_Deployment_parent, Channel::meta, Deployment::meta, "", "");

		::Uml::SetAssocRole(CommInterface::meta_commMedium, CommInterface::meta, CommMedium::meta, "interfaces");
		::Uml::SetAssocRole(CommInterface::meta_commChannels, CommInterface::meta, BChan::meta, "chanInterface");

		::Uml::SetAssocRole(CommMedium::meta_interfaces, CommMedium::meta, CommInterface::meta, "commMedium");
		::Uml::SetParentRole(CommMedium::meta_Deployment_parent, CommMedium::meta, Deployment::meta, "", "");

		::Uml::SetParentRole(CommRelations::meta_Deployment_parent, CommRelations::meta, Deployment::meta, "", "");

		::Uml::SetAssocRole(Component::meta_consumes, Component::meta, Signal::meta, "consumedBy");
		::Uml::SetAssocRole(Component::meta_invokedBy, Component::meta, Task::meta, "invokes");
		::Uml::SetAssocRole(Component::meta_generates, Component::meta, Signal::meta, "generatedBy");
		::Uml::SetParentRole(Component::meta_Deployment_parent, Component::meta, Deployment::meta, "", "");

		::Uml::SetChildRole(Deployment::meta_SignalType_children, Deployment::meta, SignalType::meta, "", "");
		::Uml::SetChildRole(Deployment::meta_TestPoint_children, Deployment::meta, TestPoint::meta, "", "");
		::Uml::SetChildRole(Deployment::meta_CommRelations_children, Deployment::meta, CommRelations::meta, "", "");
		::Uml::SetChildRole(Deployment::meta_Channel_children, Deployment::meta, Channel::meta, "", "");
		::Uml::SetChildRole(Deployment::meta_Plant_children, Deployment::meta, Plant::meta, "", "");
		::Uml::SetChildRole(Deployment::meta_Msg_children, Deployment::meta, Msg::meta, "", "");
		::Uml::SetChildRole(Deployment::meta_CommMedium_children, Deployment::meta, CommMedium::meta, "", "");
		::Uml::SetChildRole(Deployment::meta_Task_children, Deployment::meta, Task::meta, "", "");
		::Uml::SetChildRole(Deployment::meta_Signal_children, Deployment::meta, Signal::meta, "", "");
		::Uml::SetChildRole(Deployment::meta_Device_children, Deployment::meta, Device::meta, "", "");
		::Uml::SetChildRole(Deployment::meta_Component_children, Deployment::meta, Component::meta, "", "");
		::Uml::SetChildRole(Deployment::meta_Node_children, Deployment::meta, Node::meta, "", "");
		::Uml::SetChildRole(Deployment::meta_ExecInfo_children, Deployment::meta, ExecInfo::meta, "", "");
		::Uml::SetChildRole(Deployment::meta_TimingConstraint_children, Deployment::meta, TimingConstraint::meta, "", "");
		::Uml::SetParentRole(Deployment::meta_RootFolder_parent, Deployment::meta, RootFolder::meta, "", "");

		::Uml::SetAssocRole(Device::meta_integratedBy, Device::meta, Node::meta, "integrates");
		::Uml::SetParentRole(Device::meta_Deployment_parent, Device::meta, Deployment::meta, "", "");

		::Uml::SetAssocRole(DummyDependency::meta_sendingTask, DummyDependency::meta, Task::meta, "dummyDeps");
		::Uml::SetAssocRole(DummyDependency::meta_dummyMsg, DummyDependency::meta, Msg::meta, "dummyMsgDeps");
		::Uml::SetAssocRole(DummyDependency::meta_dummyDepNode, DummyDependency::meta, Node::meta, "nodeDummyDeps");

		::Uml::SetParentRole(Duration::meta_ExecInfo_parent, Duration::meta, ExecInfo::meta, "", "");

		::Uml::SetAssocRole(ExecInfo::meta_executor, ExecInfo::meta, Executable::meta, "info");
		::Uml::SetChildRole(ExecInfo::meta_Schedule_child, ExecInfo::meta, Schedule::meta, "", "");
		::Uml::SetChildRole(ExecInfo::meta_Periodicity_children, ExecInfo::meta, Periodicity::meta, "", "");
		::Uml::SetChildRole(ExecInfo::meta_Duration_child, ExecInfo::meta, Duration::meta, "", "");
		::Uml::SetParentRole(ExecInfo::meta_Deployment_parent, ExecInfo::meta, Deployment::meta, "", "");

		::Uml::SetAssocRole(Executable::meta_info, Executable::meta, ExecInfo::meta, "executor");

		::Uml::SetAssocRole(IChan::meta_channelAcquisition, IChan::meta, Acquisition::meta, "acquiringChannel");
		::Uml::SetAssocRole(IChan::meta_ichanDevice, IChan::meta, InputDevice::meta, "inputChannels");

		::Uml::SetAssocRole(InputDevice::meta_inputEnvironment, InputDevice::meta, Plant::meta, "acquisitionDevices");
		::Uml::SetAssocRole(InputDevice::meta_inputChannels, InputDevice::meta, IChan::meta, "ichanDevice");

		::Uml::SetAssocRole(LocalDependency::meta_localDepNode, LocalDependency::meta, Node::meta, "nodeLocalDeps");
		::Uml::SetAssocRole(LocalDependency::meta_localDepReceivingTasks, LocalDependency::meta, Task::meta, "localReceivingTaskDeps");
		::Uml::SetAssocRole(LocalDependency::meta_localDepSendingTask, LocalDependency::meta, Task::meta, "localSendingTaskDeps");
		::Uml::SetAssocRole(LocalDependency::meta_depMsg, LocalDependency::meta, Msg::meta, "msgDep");

		::Uml::SetAssocRole(Msg::meta_dummyMsgDeps, Msg::meta, DummyDependency::meta, "dummyMsg");
		::Uml::SetAssocRole(Msg::meta_msgDep, Msg::meta, LocalDependency::meta, "depMsg");
		::Uml::SetAssocRole(Msg::meta_msgListeners, Msg::meta, Receives::meta, "receivedMsg");
		::Uml::SetAssocRole(Msg::meta_msgTransmitter, Msg::meta, Transmits::meta, "transmittedMsg");
		::Uml::SetAssocRole(Msg::meta_msgActuation, Msg::meta, Actuation::meta, "actuatingMsg");
		::Uml::SetAssocRole(Msg::meta_msgAcquisitions, Msg::meta, Acquisition::meta, "acquiredMsg");
		::Uml::SetAssocRole(Msg::meta_dummyMsgNode, Msg::meta, Node::meta, "nodeDummyMsg");
		::Uml::SetAssocRole(Msg::meta_carries, Msg::meta, Signal::meta, "carriedBy");
		::Uml::SetParentRole(Msg::meta_Deployment_parent, Msg::meta, Deployment::meta, "", "");

		::Uml::SetAssocRole(Node::meta_nodeDummyDeps, Node::meta, DummyDependency::meta, "dummyDepNode");
		::Uml::SetAssocRole(Node::meta_nodeLocalDeps, Node::meta, LocalDependency::meta, "localDepNode");
		::Uml::SetAssocRole(Node::meta_nodeDummyMsg, Node::meta, Msg::meta, "dummyMsgNode");
		::Uml::SetAssocRole(Node::meta_executes, Node::meta, Task::meta, "executedBy");
		::Uml::SetAssocRole(Node::meta_integrates, Node::meta, Device::meta, "integratedBy");
		::Uml::SetParentRole(Node::meta_Deployment_parent, Node::meta, Deployment::meta, "", "");

		::Uml::SetAssocRole(OChan::meta_channelActuations, OChan::meta, Actuation::meta, "actuatingChannel");
		::Uml::SetAssocRole(OChan::meta_ochanDevice, OChan::meta, OutputDevice::meta, "outputChannels");

		::Uml::SetAssocRole(OutputDevice::meta_outputEnvironment, OutputDevice::meta, Plant::meta, "actuationDevices");
		::Uml::SetAssocRole(OutputDevice::meta_outputChannels, OutputDevice::meta, OChan::meta, "ochanDevice");

		::Uml::SetParentRole(Periodicity::meta_ExecInfo_parent, Periodicity::meta, ExecInfo::meta, "", "");

		::Uml::SetAssocRole(Plant::meta_actuationDevices, Plant::meta, OutputDevice::meta, "outputEnvironment");
		::Uml::SetAssocRole(Plant::meta_acquisitionDevices, Plant::meta, InputDevice::meta, "inputEnvironment");
		::Uml::SetParentRole(Plant::meta_Deployment_parent, Plant::meta, Deployment::meta, "", "");

		::Uml::SetAssocRole(Receives::meta_receivedMsg, Receives::meta, Msg::meta, "msgListeners");
		::Uml::SetAssocRole(Receives::meta_receivingChannel, Receives::meta, BChan::meta, "channelListeners");
		::Uml::SetAssocRole(Receives::meta_receivingTask, Receives::meta, Task::meta, "incomingData");

		::Uml::SetChildRole(RootFolder::meta_RootFolder_children, RootFolder::meta, RootFolder::meta, "", "");
		::Uml::SetChildRole(RootFolder::meta_Deployment_children, RootFolder::meta, Deployment::meta, "", "");
		::Uml::SetParentRole(RootFolder::meta_RootFolder_parent, RootFolder::meta, RootFolder::meta, "", "");

		::Uml::SetParentRole(Schedule::meta_ExecInfo_parent, Schedule::meta, ExecInfo::meta, "", "");

		::Uml::SetAssocRole(Signal::meta_createdBy, Signal::meta, Source::meta, "creates");
		::Uml::SetAssocRole(Signal::meta_measuredBy, Signal::meta, Sink::meta, "measures");
		::Uml::SetAssocRole(Signal::meta_carriedBy, Signal::meta, Msg::meta, "carries");
		::Uml::SetAssocRole(Signal::meta_isOfType, Signal::meta, SignalType::meta, "signalsOfType");
		::Uml::SetAssocRole(Signal::meta_consumedBy, Signal::meta, Component::meta, "consumes");
		::Uml::SetAssocRole(Signal::meta_generatedBy, Signal::meta, Component::meta, "generates");
		::Uml::SetParentRole(Signal::meta_Deployment_parent, Signal::meta, Deployment::meta, "", "");

		::Uml::SetAssocRole(SignalType::meta_signalsOfType, SignalType::meta, Signal::meta, "isOfType");
		::Uml::SetParentRole(SignalType::meta_Deployment_parent, SignalType::meta, Deployment::meta, "", "");

		::Uml::SetAssocRole(Sink::meta_measures, Sink::meta, Signal::meta, "measuredBy");

		::Uml::SetAssocRole(Source::meta_creates, Source::meta, Signal::meta, "createdBy");

		::Uml::SetAssocRole(Task::meta_latencyAsSndr, Task::meta, TimingConstraint::meta, "sndTask");
		::Uml::SetAssocRole(Task::meta_dummyDeps, Task::meta, DummyDependency::meta, "sendingTask");
		::Uml::SetAssocRole(Task::meta_localReceivingTaskDeps, Task::meta, LocalDependency::meta, "localDepReceivingTasks");
		::Uml::SetAssocRole(Task::meta_localSendingTaskDeps, Task::meta, LocalDependency::meta, "localDepSendingTask");
		::Uml::SetAssocRole(Task::meta_incomingData, Task::meta, Receives::meta, "receivingTask");
		::Uml::SetAssocRole(Task::meta_taskActuations, Task::meta, Actuation::meta, "actuatingTask");
		::Uml::SetAssocRole(Task::meta_taskAcquisitions, Task::meta, Acquisition::meta, "acquiringTasks");
		::Uml::SetAssocRole(Task::meta_executedBy, Task::meta, Node::meta, "executes");
		::Uml::SetAssocRole(Task::meta_invokes, Task::meta, Component::meta, "invokedBy");
		::Uml::SetAssocRole(Task::meta_outboundData, Task::meta, Transmits::meta, "sendingTask");
		::Uml::SetAssocRole(Task::meta_latencyAsRcvr, Task::meta, TimingConstraint::meta, "rcvTask");
		::Uml::SetParentRole(Task::meta_Deployment_parent, Task::meta, Deployment::meta, "", "");

		::Uml::SetParentRole(TestPoint::meta_Deployment_parent, TestPoint::meta, Deployment::meta, "", "");

		::Uml::SetAssocRole(TimingConstraint::meta_sndTask, TimingConstraint::meta, Task::meta, "latencyAsSndr");
		::Uml::SetAssocRole(TimingConstraint::meta_rcvTask, TimingConstraint::meta, Task::meta, "latencyAsRcvr");
		::Uml::SetParentRole(TimingConstraint::meta_Deployment_parent, TimingConstraint::meta, Deployment::meta, "", "");

		::Uml::SetAssocRole(Transmits::meta_transmittedMsg, Transmits::meta, Msg::meta, "msgTransmitter");
		::Uml::SetAssocRole(Transmits::meta_sendingChan, Transmits::meta, BChan::meta, "channelTransmitters");
		::Uml::SetAssocRole(Transmits::meta_sendingTask, Transmits::meta, Task::meta, "outboundData");

	}

	void _SetXsdStorage()
	{
		UdmDom::str_xsd_storage::StoreXsd("Semantics.xsd", Semantics_xsd::getString());
	}

	void Initialize()
	{
		static bool first = true;
		if (!first) return;
		first = false;
		::Uml::Initialize();

	
		ASSERT( meta == Udm::null );

		UdmStatic::StaticDataNetwork * meta_dn = new UdmStatic::StaticDataNetwork(::Uml::diagram);
		meta_dn->CreateNew("Semantics.mem", "", ::Uml::Diagram::meta, Udm::CHANGES_LOST_DEFAULT);
		meta = ::Uml::Diagram::Cast(meta_dn->GetRootObject());

		::Uml::InitDiagramProps(meta, "Semantics", "1.00");


		CreateMeta();
		InitMeta();
		InitMetaLinks();

		_SetXsdStorage();

	}

	void Initialize(const ::Uml::Diagram &dgr)
	{
		
		if (meta == dgr) return;
		meta = dgr;

		InitMeta(dgr);
		InitMetaLinks(dgr);

		
		_SetXsdStorage();
	}


	Udm::UdmDiagram diagram = { &meta, Initialize };
	static struct _regClass
	{
		_regClass()
		{
			Udm::MetaDepository::StoreDiagram("Semantics", diagram);
		}
		~_regClass()
		{
			Udm::MetaDepository::RemoveDiagram("Semantics");
		}
	} __regUnUsed;

}

