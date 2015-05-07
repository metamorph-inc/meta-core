#ifndef MOBIES_SEMANTICS_H
#define MOBIES_SEMANTICS_H

// header file Semantics.h generated from diagram Semantics
// generated with Udm version 3.26 on Thu Nov 11 14:16:28 2010

#include <UdmBase.h>

#if !defined(UDM_VERSION_MAJOR) || !defined(UDM_VERSION_MINOR)
#    error "Udm headers too old, they do not define UDM_VERSION"
#elif UDM_VERSION_MAJOR < 3
#    error "Udm headers too old, minimum version required 3.26"
#elif UDM_VERSION_MAJOR == 3 && UDM_VERSION_MINOR < 26
#    error "Udm headers too old, minimum version required 3.26"
#endif

#include <Uml.h>


#ifdef min
#undef min
#endif

#ifdef max
#undef max
#endif

namespace Semantics {

	extern ::Uml::Diagram meta;
	class TTSchedule;
	class Schedule;
	class Executable;
	class Channel;
	class Device;
	class Msg;
	class Task;
	class MaxDuration;
	class Duration;
	class SamplePeriod;
	class Periodicity;
	class MinimumPeriod;
	class DummyDependency;
	class CommRelations;
	class LocalDependency;
	class TestPoint;
	class Sink;
	class Source;
	class OutputDevice;
	class InputDevice;
	class CommInterface;
	class Receives;
	class Transmits;
	class Actuation;
	class Acquisition;
	class OChan;
	class BChan;
	class IChan;
	class TTBus;
	class CommMedium;
	class ExecInfo;
	class RootFolder;
	class Deployment;
	class SignalType;
	class Plant;
	class Signal;
	class Component;
	class Node;
	class TimingConstraint;

	class Visitor : public Udm::BaseVisitor {
	public:
		virtual ~Visitor() {}

		virtual void Visit_TTSchedule(const TTSchedule &) {}
		virtual void Visit_Channel(const Channel &) {}
		virtual void Visit_Device(const Device &) {}
		virtual void Visit_Msg(const Msg &) {}
		virtual void Visit_Task(const Task &) {}
		virtual void Visit_MaxDuration(const MaxDuration &) {}
		virtual void Visit_SamplePeriod(const SamplePeriod &) {}
		virtual void Visit_MinimumPeriod(const MinimumPeriod &) {}
		virtual void Visit_DummyDependency(const DummyDependency &) {}
		virtual void Visit_CommRelations(const CommRelations &) {}
		virtual void Visit_LocalDependency(const LocalDependency &) {}
		virtual void Visit_TestPoint(const TestPoint &) {}
		virtual void Visit_Sink(const Sink &) {}
		virtual void Visit_Source(const Source &) {}
		virtual void Visit_OutputDevice(const OutputDevice &) {}
		virtual void Visit_InputDevice(const InputDevice &) {}
		virtual void Visit_CommInterface(const CommInterface &) {}
		virtual void Visit_Receives(const Receives &) {}
		virtual void Visit_Transmits(const Transmits &) {}
		virtual void Visit_Actuation(const Actuation &) {}
		virtual void Visit_Acquisition(const Acquisition &) {}
		virtual void Visit_OChan(const OChan &) {}
		virtual void Visit_BChan(const BChan &) {}
		virtual void Visit_IChan(const IChan &) {}
		virtual void Visit_TTBus(const TTBus &) {}
		virtual void Visit_CommMedium(const CommMedium &) {}
		virtual void Visit_ExecInfo(const ExecInfo &) {}
		virtual void Visit_RootFolder(const RootFolder &) {}
		virtual void Visit_Deployment(const Deployment &) {}
		virtual void Visit_SignalType(const SignalType &) {}
		virtual void Visit_Plant(const Plant &) {}
		virtual void Visit_Signal(const Signal &) {}
		virtual void Visit_Component(const Component &) {}
		virtual void Visit_Node(const Node &) {}
		virtual void Visit_TimingConstraint(const TimingConstraint &) {}
		virtual void Visit_Object(const Udm::Object &) {}

	};

	void Initialize();
	void Initialize(const ::Uml::Diagram &dgr);

	extern Udm::UdmDiagram diagram;

	class Schedule : public Udm::Object {
	public:
		Schedule() {}
		Schedule(Udm::ObjectImpl *impl) : UDM_OBJECT(impl) {}
		Schedule(const Schedule &master) : UDM_OBJECT(master) {}
		#ifdef UDM_RVALUE
		Schedule(Schedule &&master) : UDM_OBJECT(master) {}

		static Schedule Cast(Udm::Object &&a) { return __Cast(a, meta); }
		Schedule& operator=(Schedule &&a) { Udm::Object::operator =(a); return *this; }
		#endif
		static Schedule Cast(const Udm::Object &a) { return __Cast(a, meta); }
		static Schedule Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }
		Schedule CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }
		Udm::InstantiatedAttr<Schedule> Instances() { return Udm::InstantiatedAttr<Schedule>(impl); }
		template <class Pred> Udm::InstantiatedAttr<Schedule, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<Schedule, Pred>(impl); }
		Schedule CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }
		Udm::DerivedAttr<Schedule> Derived() { return Udm::DerivedAttr<Schedule>(impl); }
		template <class Pred> Udm::DerivedAttr<Schedule, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<Schedule, Pred>(impl); }
		Udm::ArchetypeAttr<Schedule> Archetype() const { return Udm::ArchetypeAttr<Schedule>(impl); }
		Udm::StringAttr sched() const { return Udm::StringAttr(impl, meta_sched); }
		Udm::ParentAttr< ::Semantics::ExecInfo> ExecInfo_parent() const { return Udm::ParentAttr< ::Semantics::ExecInfo>(impl, meta_ExecInfo_parent); }
		Udm::ParentAttr< ::Semantics::ExecInfo> parent() const { return Udm::ParentAttr< ::Semantics::ExecInfo>(impl, Udm::NULLPARENTROLE); }

		static ::Uml::Class meta;
		static ::Uml::Attribute meta_sched;
		static ::Uml::CompositionParentRole meta_ExecInfo_parent;

	};

	class TTSchedule :  public Schedule {
	public:
		TTSchedule() {}
		TTSchedule(Udm::ObjectImpl *impl) : Schedule(impl) {}
		TTSchedule(const TTSchedule &master) : Schedule(master) {}
		#ifdef UDM_RVALUE
		TTSchedule(TTSchedule &&master) : Schedule(master) {}

		static TTSchedule Cast(Udm::Object &&a) { return __Cast(a, meta); }
		TTSchedule& operator=(TTSchedule &&a) { Udm::Object::operator =(a); return *this; }
		#endif
		static TTSchedule Cast(const Udm::Object &a) { return __Cast(a, meta); }
		static TTSchedule Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }
		TTSchedule CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }
		Udm::InstantiatedAttr<TTSchedule> Instances() { return Udm::InstantiatedAttr<TTSchedule>(impl); }
		template <class Pred> Udm::InstantiatedAttr<TTSchedule, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<TTSchedule, Pred>(impl); }
		TTSchedule CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }
		Udm::DerivedAttr<TTSchedule> Derived() { return Udm::DerivedAttr<TTSchedule>(impl); }
		template <class Pred> Udm::DerivedAttr<TTSchedule, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<TTSchedule, Pred>(impl); }
		Udm::ArchetypeAttr<TTSchedule> Archetype() const { return Udm::ArchetypeAttr<TTSchedule>(impl); }
		Udm::StringAttr ticksched() const { return Udm::StringAttr(impl, meta_ticksched); }
		Udm::ParentAttr< ::Semantics::ExecInfo> parent() const { return Udm::ParentAttr< ::Semantics::ExecInfo>(impl, Udm::NULLPARENTROLE); }
		void Accept(Visitor &v) { v.Visit_TTSchedule(*this); }

		static ::Uml::Class meta;
		static ::Uml::Attribute meta_ticksched;

	};

	class Executable : public Udm::Object {
	public:
		Executable() {}
		Executable(Udm::ObjectImpl *impl) : UDM_OBJECT(impl) {}
		Executable(const Executable &master) : UDM_OBJECT(master) {}
		#ifdef UDM_RVALUE
		Executable(Executable &&master) : UDM_OBJECT(master) {}

		static Executable Cast(Udm::Object &&a) { return __Cast(a, meta); }
		Executable& operator=(Executable &&a) { Udm::Object::operator =(a); return *this; }
		#endif
		static Executable Cast(const Udm::Object &a) { return __Cast(a, meta); }
		static Executable Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }
		Executable CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }
		Udm::InstantiatedAttr<Executable> Instances() { return Udm::InstantiatedAttr<Executable>(impl); }
		template <class Pred> Udm::InstantiatedAttr<Executable, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<Executable, Pred>(impl); }
		Executable CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }
		Udm::DerivedAttr<Executable> Derived() { return Udm::DerivedAttr<Executable>(impl); }
		template <class Pred> Udm::DerivedAttr<Executable, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<Executable, Pred>(impl); }
		Udm::ArchetypeAttr<Executable> Archetype() const { return Udm::ArchetypeAttr<Executable>(impl); }
		Udm::AssocAttr< ExecInfo> info() const { return Udm::AssocAttr< ExecInfo>(impl, meta_info); }
		template <class Pred> Udm::AssocAttr< ExecInfo, Pred> info_sorted(const Pred &) const { return Udm::AssocAttr< ExecInfo, Pred>(impl, meta_info); }
		Udm::ParentAttr<Udm::Object> parent() const { return Udm::ParentAttr<Udm::Object>(impl, Udm::NULLPARENTROLE); }

		static ::Uml::Class meta;
		static ::Uml::AssociationRole meta_info;

	};

	class Channel :  public Executable {
	public:
		Channel() {}
		Channel(Udm::ObjectImpl *impl) : Executable(impl) {}
		Channel(const Channel &master) : Executable(master) {}
		#ifdef UDM_RVALUE
		Channel(Channel &&master) : Executable(master) {}

		static Channel Cast(Udm::Object &&a) { return __Cast(a, meta); }
		Channel& operator=(Channel &&a) { Udm::Object::operator =(a); return *this; }
		#endif
		static Channel Cast(const Udm::Object &a) { return __Cast(a, meta); }
		static Channel Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }
		Channel CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }
		Udm::InstantiatedAttr<Channel> Instances() { return Udm::InstantiatedAttr<Channel>(impl); }
		template <class Pred> Udm::InstantiatedAttr<Channel, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<Channel, Pred>(impl); }
		Channel CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }
		Udm::DerivedAttr<Channel> Derived() { return Udm::DerivedAttr<Channel>(impl); }
		template <class Pred> Udm::DerivedAttr<Channel, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<Channel, Pred>(impl); }
		Udm::ArchetypeAttr<Channel> Archetype() const { return Udm::ArchetypeAttr<Channel>(impl); }
		Udm::StringAttr name() const { return Udm::StringAttr(impl, meta_name); }
		Udm::IntegerAttr id() const { return Udm::IntegerAttr(impl, meta_id); }
		Udm::StringAttr config() const { return Udm::StringAttr(impl, meta_config); }
		Udm::ParentAttr< ::Semantics::Deployment> Deployment_parent() const { return Udm::ParentAttr< ::Semantics::Deployment>(impl, meta_Deployment_parent); }
		Udm::ParentAttr< ::Semantics::Deployment> parent() const { return Udm::ParentAttr< ::Semantics::Deployment>(impl, Udm::NULLPARENTROLE); }
		void Accept(Visitor &v) { v.Visit_Channel(*this); }

		static ::Uml::Class meta;
		static ::Uml::Attribute meta_name;
		static ::Uml::Attribute meta_id;
		static ::Uml::Attribute meta_config;
		static ::Uml::CompositionParentRole meta_Deployment_parent;

	};

	class Device :  public Executable {
	public:
		Device() {}
		Device(Udm::ObjectImpl *impl) : Executable(impl) {}
		Device(const Device &master) : Executable(master) {}
		#ifdef UDM_RVALUE
		Device(Device &&master) : Executable(master) {}

		static Device Cast(Udm::Object &&a) { return __Cast(a, meta); }
		Device& operator=(Device &&a) { Udm::Object::operator =(a); return *this; }
		#endif
		static Device Cast(const Udm::Object &a) { return __Cast(a, meta); }
		static Device Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }
		Device CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }
		Udm::InstantiatedAttr<Device> Instances() { return Udm::InstantiatedAttr<Device>(impl); }
		template <class Pred> Udm::InstantiatedAttr<Device, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<Device, Pred>(impl); }
		Device CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }
		Udm::DerivedAttr<Device> Derived() { return Udm::DerivedAttr<Device>(impl); }
		template <class Pred> Udm::DerivedAttr<Device, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<Device, Pred>(impl); }
		Udm::ArchetypeAttr<Device> Archetype() const { return Udm::ArchetypeAttr<Device>(impl); }
		Udm::StringAttr name() const { return Udm::StringAttr(impl, meta_name); }
		Udm::IntegerAttr id() const { return Udm::IntegerAttr(impl, meta_id); }
		Udm::StringAttr devicetype() const { return Udm::StringAttr(impl, meta_devicetype); }
		Udm::StringAttr config() const { return Udm::StringAttr(impl, meta_config); }
		Udm::PointerAttr< Node> integratedBy() const { return Udm::PointerAttr< Node>(impl, meta_integratedBy); }
		Udm::ParentAttr< ::Semantics::Deployment> Deployment_parent() const { return Udm::ParentAttr< ::Semantics::Deployment>(impl, meta_Deployment_parent); }
		Udm::ParentAttr< ::Semantics::Deployment> parent() const { return Udm::ParentAttr< ::Semantics::Deployment>(impl, Udm::NULLPARENTROLE); }
		void Accept(Visitor &v) { v.Visit_Device(*this); }

		static ::Uml::Class meta;
		static ::Uml::Attribute meta_name;
		static ::Uml::Attribute meta_id;
		static ::Uml::Attribute meta_devicetype;
		static ::Uml::Attribute meta_config;
		static ::Uml::AssociationRole meta_integratedBy;
		static ::Uml::CompositionParentRole meta_Deployment_parent;

	};

	class Msg :  public Executable {
	public:
		Msg() {}
		Msg(Udm::ObjectImpl *impl) : Executable(impl) {}
		Msg(const Msg &master) : Executable(master) {}
		#ifdef UDM_RVALUE
		Msg(Msg &&master) : Executable(master) {}

		static Msg Cast(Udm::Object &&a) { return __Cast(a, meta); }
		Msg& operator=(Msg &&a) { Udm::Object::operator =(a); return *this; }
		#endif
		static Msg Cast(const Udm::Object &a) { return __Cast(a, meta); }
		static Msg Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }
		Msg CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }
		Udm::InstantiatedAttr<Msg> Instances() { return Udm::InstantiatedAttr<Msg>(impl); }
		template <class Pred> Udm::InstantiatedAttr<Msg, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<Msg, Pred>(impl); }
		Msg CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }
		Udm::DerivedAttr<Msg> Derived() { return Udm::DerivedAttr<Msg>(impl); }
		template <class Pred> Udm::DerivedAttr<Msg, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<Msg, Pred>(impl); }
		Udm::ArchetypeAttr<Msg> Archetype() const { return Udm::ArchetypeAttr<Msg>(impl); }
		Udm::StringAttr name() const { return Udm::StringAttr(impl, meta_name); }
		Udm::IntegerAttr id() const { return Udm::IntegerAttr(impl, meta_id); }
		Udm::RealAttr sizebytes() const { return Udm::RealAttr(impl, meta_sizebytes); }
		Udm::StringAttr messagetype() const { return Udm::StringAttr(impl, meta_messagetype); }
		Udm::AssocAttr< DummyDependency> dummyMsgDeps() const { return Udm::AssocAttr< DummyDependency>(impl, meta_dummyMsgDeps); }
		template <class Pred> Udm::AssocAttr< DummyDependency, Pred> dummyMsgDeps_sorted(const Pred &) const { return Udm::AssocAttr< DummyDependency, Pred>(impl, meta_dummyMsgDeps); }
		Udm::PointerAttr< LocalDependency> msgDep() const { return Udm::PointerAttr< LocalDependency>(impl, meta_msgDep); }
		Udm::AssocAttr< Receives> msgListeners() const { return Udm::AssocAttr< Receives>(impl, meta_msgListeners); }
		template <class Pred> Udm::AssocAttr< Receives, Pred> msgListeners_sorted(const Pred &) const { return Udm::AssocAttr< Receives, Pred>(impl, meta_msgListeners); }
		Udm::PointerAttr< Transmits> msgTransmitter() const { return Udm::PointerAttr< Transmits>(impl, meta_msgTransmitter); }
		Udm::PointerAttr< Actuation> msgActuation() const { return Udm::PointerAttr< Actuation>(impl, meta_msgActuation); }
		Udm::AssocAttr< Acquisition> msgAcquisitions() const { return Udm::AssocAttr< Acquisition>(impl, meta_msgAcquisitions); }
		template <class Pred> Udm::AssocAttr< Acquisition, Pred> msgAcquisitions_sorted(const Pred &) const { return Udm::AssocAttr< Acquisition, Pred>(impl, meta_msgAcquisitions); }
		Udm::PointerAttr< Node> dummyMsgNode() const { return Udm::PointerAttr< Node>(impl, meta_dummyMsgNode); }
		Udm::AssocAttr< Signal> carries() const { return Udm::AssocAttr< Signal>(impl, meta_carries); }
		template <class Pred> Udm::AssocAttr< Signal, Pred> carries_sorted(const Pred &) const { return Udm::AssocAttr< Signal, Pred>(impl, meta_carries); }
		Udm::ParentAttr< ::Semantics::Deployment> Deployment_parent() const { return Udm::ParentAttr< ::Semantics::Deployment>(impl, meta_Deployment_parent); }
		Udm::ParentAttr< ::Semantics::Deployment> parent() const { return Udm::ParentAttr< ::Semantics::Deployment>(impl, Udm::NULLPARENTROLE); }
		void Accept(Visitor &v) { v.Visit_Msg(*this); }

		static ::Uml::Class meta;
		static ::Uml::Attribute meta_name;
		static ::Uml::Attribute meta_id;
		static ::Uml::Attribute meta_sizebytes;
		static ::Uml::Attribute meta_messagetype;
		static ::Uml::AssociationRole meta_dummyMsgDeps;
		static ::Uml::AssociationRole meta_msgDep;
		static ::Uml::AssociationRole meta_msgListeners;
		static ::Uml::AssociationRole meta_msgTransmitter;
		static ::Uml::AssociationRole meta_msgActuation;
		static ::Uml::AssociationRole meta_msgAcquisitions;
		static ::Uml::AssociationRole meta_dummyMsgNode;
		static ::Uml::AssociationRole meta_carries;
		static ::Uml::CompositionParentRole meta_Deployment_parent;

	};

	class Task :  public Executable {
	public:
		Task() {}
		Task(Udm::ObjectImpl *impl) : Executable(impl) {}
		Task(const Task &master) : Executable(master) {}
		#ifdef UDM_RVALUE
		Task(Task &&master) : Executable(master) {}

		static Task Cast(Udm::Object &&a) { return __Cast(a, meta); }
		Task& operator=(Task &&a) { Udm::Object::operator =(a); return *this; }
		#endif
		static Task Cast(const Udm::Object &a) { return __Cast(a, meta); }
		static Task Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }
		Task CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }
		Udm::InstantiatedAttr<Task> Instances() { return Udm::InstantiatedAttr<Task>(impl); }
		template <class Pred> Udm::InstantiatedAttr<Task, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<Task, Pred>(impl); }
		Task CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }
		Udm::DerivedAttr<Task> Derived() { return Udm::DerivedAttr<Task>(impl); }
		template <class Pred> Udm::DerivedAttr<Task, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<Task, Pred>(impl); }
		Udm::ArchetypeAttr<Task> Archetype() const { return Udm::ArchetypeAttr<Task>(impl); }
		Udm::StringAttr name() const { return Udm::StringAttr(impl, meta_name); }
		Udm::IntegerAttr id() const { return Udm::IntegerAttr(impl, meta_id); }
		Udm::AssocAttr< TimingConstraint> latencyAsSndr() const { return Udm::AssocAttr< TimingConstraint>(impl, meta_latencyAsSndr); }
		template <class Pred> Udm::AssocAttr< TimingConstraint, Pred> latencyAsSndr_sorted(const Pred &) const { return Udm::AssocAttr< TimingConstraint, Pred>(impl, meta_latencyAsSndr); }
		Udm::AssocAttr< DummyDependency> dummyDeps() const { return Udm::AssocAttr< DummyDependency>(impl, meta_dummyDeps); }
		template <class Pred> Udm::AssocAttr< DummyDependency, Pred> dummyDeps_sorted(const Pred &) const { return Udm::AssocAttr< DummyDependency, Pred>(impl, meta_dummyDeps); }
		Udm::AssocAttr< LocalDependency> localReceivingTaskDeps() const { return Udm::AssocAttr< LocalDependency>(impl, meta_localReceivingTaskDeps); }
		template <class Pred> Udm::AssocAttr< LocalDependency, Pred> localReceivingTaskDeps_sorted(const Pred &) const { return Udm::AssocAttr< LocalDependency, Pred>(impl, meta_localReceivingTaskDeps); }
		Udm::AssocAttr< LocalDependency> localSendingTaskDeps() const { return Udm::AssocAttr< LocalDependency>(impl, meta_localSendingTaskDeps); }
		template <class Pred> Udm::AssocAttr< LocalDependency, Pred> localSendingTaskDeps_sorted(const Pred &) const { return Udm::AssocAttr< LocalDependency, Pred>(impl, meta_localSendingTaskDeps); }
		Udm::AssocAttr< Receives> incomingData() const { return Udm::AssocAttr< Receives>(impl, meta_incomingData); }
		template <class Pred> Udm::AssocAttr< Receives, Pred> incomingData_sorted(const Pred &) const { return Udm::AssocAttr< Receives, Pred>(impl, meta_incomingData); }
		Udm::AssocAttr< Actuation> taskActuations() const { return Udm::AssocAttr< Actuation>(impl, meta_taskActuations); }
		template <class Pred> Udm::AssocAttr< Actuation, Pred> taskActuations_sorted(const Pred &) const { return Udm::AssocAttr< Actuation, Pred>(impl, meta_taskActuations); }
		Udm::AssocAttr< Acquisition> taskAcquisitions() const { return Udm::AssocAttr< Acquisition>(impl, meta_taskAcquisitions); }
		template <class Pred> Udm::AssocAttr< Acquisition, Pred> taskAcquisitions_sorted(const Pred &) const { return Udm::AssocAttr< Acquisition, Pred>(impl, meta_taskAcquisitions); }
		Udm::PointerAttr< Node> executedBy() const { return Udm::PointerAttr< Node>(impl, meta_executedBy); }
		Udm::AssocAttr< Component> invokes() const { return Udm::AssocAttr< Component>(impl, meta_invokes); }
		template <class Pred> Udm::AssocAttr< Component, Pred> invokes_sorted(const Pred &) const { return Udm::AssocAttr< Component, Pred>(impl, meta_invokes); }
		Udm::AssocAttr< Transmits> outboundData() const { return Udm::AssocAttr< Transmits>(impl, meta_outboundData); }
		template <class Pred> Udm::AssocAttr< Transmits, Pred> outboundData_sorted(const Pred &) const { return Udm::AssocAttr< Transmits, Pred>(impl, meta_outboundData); }
		Udm::AssocAttr< TimingConstraint> latencyAsRcvr() const { return Udm::AssocAttr< TimingConstraint>(impl, meta_latencyAsRcvr); }
		template <class Pred> Udm::AssocAttr< TimingConstraint, Pred> latencyAsRcvr_sorted(const Pred &) const { return Udm::AssocAttr< TimingConstraint, Pred>(impl, meta_latencyAsRcvr); }
		Udm::ParentAttr< ::Semantics::Deployment> Deployment_parent() const { return Udm::ParentAttr< ::Semantics::Deployment>(impl, meta_Deployment_parent); }
		Udm::ParentAttr< ::Semantics::Deployment> parent() const { return Udm::ParentAttr< ::Semantics::Deployment>(impl, Udm::NULLPARENTROLE); }
		void Accept(Visitor &v) { v.Visit_Task(*this); }

		static ::Uml::Class meta;
		static ::Uml::Attribute meta_name;
		static ::Uml::Attribute meta_id;
		static ::Uml::AssociationRole meta_latencyAsSndr;
		static ::Uml::AssociationRole meta_dummyDeps;
		static ::Uml::AssociationRole meta_localReceivingTaskDeps;
		static ::Uml::AssociationRole meta_localSendingTaskDeps;
		static ::Uml::AssociationRole meta_incomingData;
		static ::Uml::AssociationRole meta_taskActuations;
		static ::Uml::AssociationRole meta_taskAcquisitions;
		static ::Uml::AssociationRole meta_executedBy;
		static ::Uml::AssociationRole meta_invokes;
		static ::Uml::AssociationRole meta_outboundData;
		static ::Uml::AssociationRole meta_latencyAsRcvr;
		static ::Uml::CompositionParentRole meta_Deployment_parent;

	};

	class Duration : public Udm::Object {
	public:
		Duration() {}
		Duration(Udm::ObjectImpl *impl) : UDM_OBJECT(impl) {}
		Duration(const Duration &master) : UDM_OBJECT(master) {}
		#ifdef UDM_RVALUE
		Duration(Duration &&master) : UDM_OBJECT(master) {}

		static Duration Cast(Udm::Object &&a) { return __Cast(a, meta); }
		Duration& operator=(Duration &&a) { Udm::Object::operator =(a); return *this; }
		#endif
		static Duration Cast(const Udm::Object &a) { return __Cast(a, meta); }
		static Duration Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }
		Duration CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }
		Udm::InstantiatedAttr<Duration> Instances() { return Udm::InstantiatedAttr<Duration>(impl); }
		template <class Pred> Udm::InstantiatedAttr<Duration, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<Duration, Pred>(impl); }
		Duration CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }
		Udm::DerivedAttr<Duration> Derived() { return Udm::DerivedAttr<Duration>(impl); }
		template <class Pred> Udm::DerivedAttr<Duration, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<Duration, Pred>(impl); }
		Udm::ArchetypeAttr<Duration> Archetype() const { return Udm::ArchetypeAttr<Duration>(impl); }
		Udm::RealAttr exectimesecs() const { return Udm::RealAttr(impl, meta_exectimesecs); }
		Udm::ParentAttr< ::Semantics::ExecInfo> ExecInfo_parent() const { return Udm::ParentAttr< ::Semantics::ExecInfo>(impl, meta_ExecInfo_parent); }
		Udm::ParentAttr< ::Semantics::ExecInfo> parent() const { return Udm::ParentAttr< ::Semantics::ExecInfo>(impl, Udm::NULLPARENTROLE); }

		static ::Uml::Class meta;
		static ::Uml::Attribute meta_exectimesecs;
		static ::Uml::CompositionParentRole meta_ExecInfo_parent;

	};

	class MaxDuration :  public Duration {
	public:
		MaxDuration() {}
		MaxDuration(Udm::ObjectImpl *impl) : Duration(impl) {}
		MaxDuration(const MaxDuration &master) : Duration(master) {}
		#ifdef UDM_RVALUE
		MaxDuration(MaxDuration &&master) : Duration(master) {}

		static MaxDuration Cast(Udm::Object &&a) { return __Cast(a, meta); }
		MaxDuration& operator=(MaxDuration &&a) { Udm::Object::operator =(a); return *this; }
		#endif
		static MaxDuration Cast(const Udm::Object &a) { return __Cast(a, meta); }
		static MaxDuration Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }
		MaxDuration CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }
		Udm::InstantiatedAttr<MaxDuration> Instances() { return Udm::InstantiatedAttr<MaxDuration>(impl); }
		template <class Pred> Udm::InstantiatedAttr<MaxDuration, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<MaxDuration, Pred>(impl); }
		MaxDuration CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }
		Udm::DerivedAttr<MaxDuration> Derived() { return Udm::DerivedAttr<MaxDuration>(impl); }
		template <class Pred> Udm::DerivedAttr<MaxDuration, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<MaxDuration, Pred>(impl); }
		Udm::ArchetypeAttr<MaxDuration> Archetype() const { return Udm::ArchetypeAttr<MaxDuration>(impl); }
		Udm::ParentAttr< ::Semantics::ExecInfo> parent() const { return Udm::ParentAttr< ::Semantics::ExecInfo>(impl, Udm::NULLPARENTROLE); }
		void Accept(Visitor &v) { v.Visit_MaxDuration(*this); }

		static ::Uml::Class meta;

	};

	class Periodicity : public Udm::Object {
	public:
		Periodicity() {}
		Periodicity(Udm::ObjectImpl *impl) : UDM_OBJECT(impl) {}
		Periodicity(const Periodicity &master) : UDM_OBJECT(master) {}
		#ifdef UDM_RVALUE
		Periodicity(Periodicity &&master) : UDM_OBJECT(master) {}

		static Periodicity Cast(Udm::Object &&a) { return __Cast(a, meta); }
		Periodicity& operator=(Periodicity &&a) { Udm::Object::operator =(a); return *this; }
		#endif
		static Periodicity Cast(const Udm::Object &a) { return __Cast(a, meta); }
		static Periodicity Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }
		Periodicity CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }
		Udm::InstantiatedAttr<Periodicity> Instances() { return Udm::InstantiatedAttr<Periodicity>(impl); }
		template <class Pred> Udm::InstantiatedAttr<Periodicity, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<Periodicity, Pred>(impl); }
		Periodicity CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }
		Udm::DerivedAttr<Periodicity> Derived() { return Udm::DerivedAttr<Periodicity>(impl); }
		template <class Pred> Udm::DerivedAttr<Periodicity, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<Periodicity, Pred>(impl); }
		Udm::ArchetypeAttr<Periodicity> Archetype() const { return Udm::ArchetypeAttr<Periodicity>(impl); }
		Udm::RealAttr periodsecs() const { return Udm::RealAttr(impl, meta_periodsecs); }
		Udm::ParentAttr< ::Semantics::ExecInfo> ExecInfo_parent() const { return Udm::ParentAttr< ::Semantics::ExecInfo>(impl, meta_ExecInfo_parent); }
		Udm::ParentAttr< ::Semantics::ExecInfo> parent() const { return Udm::ParentAttr< ::Semantics::ExecInfo>(impl, Udm::NULLPARENTROLE); }

		static ::Uml::Class meta;
		static ::Uml::Attribute meta_periodsecs;
		static ::Uml::CompositionParentRole meta_ExecInfo_parent;

	};

	class SamplePeriod :  public Periodicity {
	public:
		SamplePeriod() {}
		SamplePeriod(Udm::ObjectImpl *impl) : Periodicity(impl) {}
		SamplePeriod(const SamplePeriod &master) : Periodicity(master) {}
		#ifdef UDM_RVALUE
		SamplePeriod(SamplePeriod &&master) : Periodicity(master) {}

		static SamplePeriod Cast(Udm::Object &&a) { return __Cast(a, meta); }
		SamplePeriod& operator=(SamplePeriod &&a) { Udm::Object::operator =(a); return *this; }
		#endif
		static SamplePeriod Cast(const Udm::Object &a) { return __Cast(a, meta); }
		static SamplePeriod Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }
		SamplePeriod CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }
		Udm::InstantiatedAttr<SamplePeriod> Instances() { return Udm::InstantiatedAttr<SamplePeriod>(impl); }
		template <class Pred> Udm::InstantiatedAttr<SamplePeriod, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<SamplePeriod, Pred>(impl); }
		SamplePeriod CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }
		Udm::DerivedAttr<SamplePeriod> Derived() { return Udm::DerivedAttr<SamplePeriod>(impl); }
		template <class Pred> Udm::DerivedAttr<SamplePeriod, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<SamplePeriod, Pred>(impl); }
		Udm::ArchetypeAttr<SamplePeriod> Archetype() const { return Udm::ArchetypeAttr<SamplePeriod>(impl); }
		Udm::ParentAttr< ::Semantics::ExecInfo> parent() const { return Udm::ParentAttr< ::Semantics::ExecInfo>(impl, Udm::NULLPARENTROLE); }
		void Accept(Visitor &v) { v.Visit_SamplePeriod(*this); }

		static ::Uml::Class meta;

	};

	class MinimumPeriod :  public Periodicity {
	public:
		MinimumPeriod() {}
		MinimumPeriod(Udm::ObjectImpl *impl) : Periodicity(impl) {}
		MinimumPeriod(const MinimumPeriod &master) : Periodicity(master) {}
		#ifdef UDM_RVALUE
		MinimumPeriod(MinimumPeriod &&master) : Periodicity(master) {}

		static MinimumPeriod Cast(Udm::Object &&a) { return __Cast(a, meta); }
		MinimumPeriod& operator=(MinimumPeriod &&a) { Udm::Object::operator =(a); return *this; }
		#endif
		static MinimumPeriod Cast(const Udm::Object &a) { return __Cast(a, meta); }
		static MinimumPeriod Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }
		MinimumPeriod CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }
		Udm::InstantiatedAttr<MinimumPeriod> Instances() { return Udm::InstantiatedAttr<MinimumPeriod>(impl); }
		template <class Pred> Udm::InstantiatedAttr<MinimumPeriod, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<MinimumPeriod, Pred>(impl); }
		MinimumPeriod CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }
		Udm::DerivedAttr<MinimumPeriod> Derived() { return Udm::DerivedAttr<MinimumPeriod>(impl); }
		template <class Pred> Udm::DerivedAttr<MinimumPeriod, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<MinimumPeriod, Pred>(impl); }
		Udm::ArchetypeAttr<MinimumPeriod> Archetype() const { return Udm::ArchetypeAttr<MinimumPeriod>(impl); }
		Udm::ParentAttr< ::Semantics::ExecInfo> parent() const { return Udm::ParentAttr< ::Semantics::ExecInfo>(impl, Udm::NULLPARENTROLE); }
		void Accept(Visitor &v) { v.Visit_MinimumPeriod(*this); }

		static ::Uml::Class meta;

	};

	class CommRelations : public Udm::Object {
	public:
		CommRelations() {}
		CommRelations(Udm::ObjectImpl *impl) : UDM_OBJECT(impl) {}
		CommRelations(const CommRelations &master) : UDM_OBJECT(master) {}
		#ifdef UDM_RVALUE
		CommRelations(CommRelations &&master) : UDM_OBJECT(master) {}

		static CommRelations Cast(Udm::Object &&a) { return __Cast(a, meta); }
		CommRelations& operator=(CommRelations &&a) { Udm::Object::operator =(a); return *this; }
		#endif
		static CommRelations Cast(const Udm::Object &a) { return __Cast(a, meta); }
		static CommRelations Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }
		CommRelations CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }
		Udm::InstantiatedAttr<CommRelations> Instances() { return Udm::InstantiatedAttr<CommRelations>(impl); }
		template <class Pred> Udm::InstantiatedAttr<CommRelations, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<CommRelations, Pred>(impl); }
		CommRelations CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }
		Udm::DerivedAttr<CommRelations> Derived() { return Udm::DerivedAttr<CommRelations>(impl); }
		template <class Pred> Udm::DerivedAttr<CommRelations, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<CommRelations, Pred>(impl); }
		Udm::ArchetypeAttr<CommRelations> Archetype() const { return Udm::ArchetypeAttr<CommRelations>(impl); }
		Udm::IntegerAttr id() const { return Udm::IntegerAttr(impl, meta_id); }
		Udm::ParentAttr< ::Semantics::Deployment> Deployment_parent() const { return Udm::ParentAttr< ::Semantics::Deployment>(impl, meta_Deployment_parent); }
		Udm::ParentAttr< ::Semantics::Deployment> parent() const { return Udm::ParentAttr< ::Semantics::Deployment>(impl, Udm::NULLPARENTROLE); }
		void Accept(Visitor &v) { v.Visit_CommRelations(*this); }

		static ::Uml::Class meta;
		static ::Uml::Attribute meta_id;
		static ::Uml::CompositionParentRole meta_Deployment_parent;

	};

	class DummyDependency :  public CommRelations {
	public:
		DummyDependency() {}
		DummyDependency(Udm::ObjectImpl *impl) : CommRelations(impl) {}
		DummyDependency(const DummyDependency &master) : CommRelations(master) {}
		#ifdef UDM_RVALUE
		DummyDependency(DummyDependency &&master) : CommRelations(master) {}

		static DummyDependency Cast(Udm::Object &&a) { return __Cast(a, meta); }
		DummyDependency& operator=(DummyDependency &&a) { Udm::Object::operator =(a); return *this; }
		#endif
		static DummyDependency Cast(const Udm::Object &a) { return __Cast(a, meta); }
		static DummyDependency Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }
		DummyDependency CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }
		Udm::InstantiatedAttr<DummyDependency> Instances() { return Udm::InstantiatedAttr<DummyDependency>(impl); }
		template <class Pred> Udm::InstantiatedAttr<DummyDependency, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<DummyDependency, Pred>(impl); }
		DummyDependency CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }
		Udm::DerivedAttr<DummyDependency> Derived() { return Udm::DerivedAttr<DummyDependency>(impl); }
		template <class Pred> Udm::DerivedAttr<DummyDependency, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<DummyDependency, Pred>(impl); }
		Udm::ArchetypeAttr<DummyDependency> Archetype() const { return Udm::ArchetypeAttr<DummyDependency>(impl); }
		Udm::PointerAttr< Task> sendingTask() const { return Udm::PointerAttr< Task>(impl, meta_sendingTask); }
		Udm::PointerAttr< Msg> dummyMsg() const { return Udm::PointerAttr< Msg>(impl, meta_dummyMsg); }
		Udm::PointerAttr< Node> dummyDepNode() const { return Udm::PointerAttr< Node>(impl, meta_dummyDepNode); }
		Udm::ParentAttr< ::Semantics::Deployment> parent() const { return Udm::ParentAttr< ::Semantics::Deployment>(impl, Udm::NULLPARENTROLE); }
		void Accept(Visitor &v) { v.Visit_DummyDependency(*this); }

		static ::Uml::Class meta;
		static ::Uml::AssociationRole meta_sendingTask;
		static ::Uml::AssociationRole meta_dummyMsg;
		static ::Uml::AssociationRole meta_dummyDepNode;

	};

	class LocalDependency :  public CommRelations {
	public:
		LocalDependency() {}
		LocalDependency(Udm::ObjectImpl *impl) : CommRelations(impl) {}
		LocalDependency(const LocalDependency &master) : CommRelations(master) {}
		#ifdef UDM_RVALUE
		LocalDependency(LocalDependency &&master) : CommRelations(master) {}

		static LocalDependency Cast(Udm::Object &&a) { return __Cast(a, meta); }
		LocalDependency& operator=(LocalDependency &&a) { Udm::Object::operator =(a); return *this; }
		#endif
		static LocalDependency Cast(const Udm::Object &a) { return __Cast(a, meta); }
		static LocalDependency Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }
		LocalDependency CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }
		Udm::InstantiatedAttr<LocalDependency> Instances() { return Udm::InstantiatedAttr<LocalDependency>(impl); }
		template <class Pred> Udm::InstantiatedAttr<LocalDependency, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<LocalDependency, Pred>(impl); }
		LocalDependency CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }
		Udm::DerivedAttr<LocalDependency> Derived() { return Udm::DerivedAttr<LocalDependency>(impl); }
		template <class Pred> Udm::DerivedAttr<LocalDependency, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<LocalDependency, Pred>(impl); }
		Udm::ArchetypeAttr<LocalDependency> Archetype() const { return Udm::ArchetypeAttr<LocalDependency>(impl); }
		Udm::PointerAttr< Node> localDepNode() const { return Udm::PointerAttr< Node>(impl, meta_localDepNode); }
		Udm::AssocAttr< Task> localDepReceivingTasks() const { return Udm::AssocAttr< Task>(impl, meta_localDepReceivingTasks); }
		template <class Pred> Udm::AssocAttr< Task, Pred> localDepReceivingTasks_sorted(const Pred &) const { return Udm::AssocAttr< Task, Pred>(impl, meta_localDepReceivingTasks); }
		Udm::PointerAttr< Task> localDepSendingTask() const { return Udm::PointerAttr< Task>(impl, meta_localDepSendingTask); }
		Udm::PointerAttr< Msg> depMsg() const { return Udm::PointerAttr< Msg>(impl, meta_depMsg); }
		Udm::ParentAttr< ::Semantics::Deployment> parent() const { return Udm::ParentAttr< ::Semantics::Deployment>(impl, Udm::NULLPARENTROLE); }
		void Accept(Visitor &v) { v.Visit_LocalDependency(*this); }

		static ::Uml::Class meta;
		static ::Uml::AssociationRole meta_localDepNode;
		static ::Uml::AssociationRole meta_localDepReceivingTasks;
		static ::Uml::AssociationRole meta_localDepSendingTask;
		static ::Uml::AssociationRole meta_depMsg;

	};

	class TestPoint : public Udm::Object {
	public:
		TestPoint() {}
		TestPoint(Udm::ObjectImpl *impl) : UDM_OBJECT(impl) {}
		TestPoint(const TestPoint &master) : UDM_OBJECT(master) {}
		#ifdef UDM_RVALUE
		TestPoint(TestPoint &&master) : UDM_OBJECT(master) {}

		static TestPoint Cast(Udm::Object &&a) { return __Cast(a, meta); }
		TestPoint& operator=(TestPoint &&a) { Udm::Object::operator =(a); return *this; }
		#endif
		static TestPoint Cast(const Udm::Object &a) { return __Cast(a, meta); }
		static TestPoint Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }
		TestPoint CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }
		Udm::InstantiatedAttr<TestPoint> Instances() { return Udm::InstantiatedAttr<TestPoint>(impl); }
		template <class Pred> Udm::InstantiatedAttr<TestPoint, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<TestPoint, Pred>(impl); }
		TestPoint CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }
		Udm::DerivedAttr<TestPoint> Derived() { return Udm::DerivedAttr<TestPoint>(impl); }
		template <class Pred> Udm::DerivedAttr<TestPoint, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<TestPoint, Pred>(impl); }
		Udm::ArchetypeAttr<TestPoint> Archetype() const { return Udm::ArchetypeAttr<TestPoint>(impl); }
		Udm::StringAttr name() const { return Udm::StringAttr(impl, meta_name); }
		Udm::IntegerAttr id() const { return Udm::IntegerAttr(impl, meta_id); }
		Udm::StringAttr config() const { return Udm::StringAttr(impl, meta_config); }
		Udm::ParentAttr< ::Semantics::Deployment> Deployment_parent() const { return Udm::ParentAttr< ::Semantics::Deployment>(impl, meta_Deployment_parent); }
		Udm::ParentAttr< ::Semantics::Deployment> parent() const { return Udm::ParentAttr< ::Semantics::Deployment>(impl, Udm::NULLPARENTROLE); }
		void Accept(Visitor &v) { v.Visit_TestPoint(*this); }

		static ::Uml::Class meta;
		static ::Uml::Attribute meta_name;
		static ::Uml::Attribute meta_id;
		static ::Uml::Attribute meta_config;
		static ::Uml::CompositionParentRole meta_Deployment_parent;

	};

	class Sink :  public TestPoint {
	public:
		Sink() {}
		Sink(Udm::ObjectImpl *impl) : TestPoint(impl) {}
		Sink(const Sink &master) : TestPoint(master) {}
		#ifdef UDM_RVALUE
		Sink(Sink &&master) : TestPoint(master) {}

		static Sink Cast(Udm::Object &&a) { return __Cast(a, meta); }
		Sink& operator=(Sink &&a) { Udm::Object::operator =(a); return *this; }
		#endif
		static Sink Cast(const Udm::Object &a) { return __Cast(a, meta); }
		static Sink Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }
		Sink CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }
		Udm::InstantiatedAttr<Sink> Instances() { return Udm::InstantiatedAttr<Sink>(impl); }
		template <class Pred> Udm::InstantiatedAttr<Sink, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<Sink, Pred>(impl); }
		Sink CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }
		Udm::DerivedAttr<Sink> Derived() { return Udm::DerivedAttr<Sink>(impl); }
		template <class Pred> Udm::DerivedAttr<Sink, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<Sink, Pred>(impl); }
		Udm::ArchetypeAttr<Sink> Archetype() const { return Udm::ArchetypeAttr<Sink>(impl); }
		Udm::AssocAttr< Signal> measures() const { return Udm::AssocAttr< Signal>(impl, meta_measures); }
		template <class Pred> Udm::AssocAttr< Signal, Pred> measures_sorted(const Pred &) const { return Udm::AssocAttr< Signal, Pred>(impl, meta_measures); }
		Udm::ParentAttr< ::Semantics::Deployment> parent() const { return Udm::ParentAttr< ::Semantics::Deployment>(impl, Udm::NULLPARENTROLE); }
		void Accept(Visitor &v) { v.Visit_Sink(*this); }

		static ::Uml::Class meta;
		static ::Uml::AssociationRole meta_measures;

	};

	class Source :  public TestPoint {
	public:
		Source() {}
		Source(Udm::ObjectImpl *impl) : TestPoint(impl) {}
		Source(const Source &master) : TestPoint(master) {}
		#ifdef UDM_RVALUE
		Source(Source &&master) : TestPoint(master) {}

		static Source Cast(Udm::Object &&a) { return __Cast(a, meta); }
		Source& operator=(Source &&a) { Udm::Object::operator =(a); return *this; }
		#endif
		static Source Cast(const Udm::Object &a) { return __Cast(a, meta); }
		static Source Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }
		Source CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }
		Udm::InstantiatedAttr<Source> Instances() { return Udm::InstantiatedAttr<Source>(impl); }
		template <class Pred> Udm::InstantiatedAttr<Source, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<Source, Pred>(impl); }
		Source CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }
		Udm::DerivedAttr<Source> Derived() { return Udm::DerivedAttr<Source>(impl); }
		template <class Pred> Udm::DerivedAttr<Source, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<Source, Pred>(impl); }
		Udm::ArchetypeAttr<Source> Archetype() const { return Udm::ArchetypeAttr<Source>(impl); }
		Udm::AssocAttr< Signal> creates() const { return Udm::AssocAttr< Signal>(impl, meta_creates); }
		template <class Pred> Udm::AssocAttr< Signal, Pred> creates_sorted(const Pred &) const { return Udm::AssocAttr< Signal, Pred>(impl, meta_creates); }
		Udm::ParentAttr< ::Semantics::Deployment> parent() const { return Udm::ParentAttr< ::Semantics::Deployment>(impl, Udm::NULLPARENTROLE); }
		void Accept(Visitor &v) { v.Visit_Source(*this); }

		static ::Uml::Class meta;
		static ::Uml::AssociationRole meta_creates;

	};

	class OutputDevice :  public Device {
	public:
		OutputDevice() {}
		OutputDevice(Udm::ObjectImpl *impl) : Device(impl) {}
		OutputDevice(const OutputDevice &master) : Device(master) {}
		#ifdef UDM_RVALUE
		OutputDevice(OutputDevice &&master) : Device(master) {}

		static OutputDevice Cast(Udm::Object &&a) { return __Cast(a, meta); }
		OutputDevice& operator=(OutputDevice &&a) { Udm::Object::operator =(a); return *this; }
		#endif
		static OutputDevice Cast(const Udm::Object &a) { return __Cast(a, meta); }
		static OutputDevice Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }
		OutputDevice CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }
		Udm::InstantiatedAttr<OutputDevice> Instances() { return Udm::InstantiatedAttr<OutputDevice>(impl); }
		template <class Pred> Udm::InstantiatedAttr<OutputDevice, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<OutputDevice, Pred>(impl); }
		OutputDevice CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }
		Udm::DerivedAttr<OutputDevice> Derived() { return Udm::DerivedAttr<OutputDevice>(impl); }
		template <class Pred> Udm::DerivedAttr<OutputDevice, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<OutputDevice, Pred>(impl); }
		Udm::ArchetypeAttr<OutputDevice> Archetype() const { return Udm::ArchetypeAttr<OutputDevice>(impl); }
		Udm::PointerAttr< Plant> outputEnvironment() const { return Udm::PointerAttr< Plant>(impl, meta_outputEnvironment); }
		Udm::AssocAttr< OChan> outputChannels() const { return Udm::AssocAttr< OChan>(impl, meta_outputChannels); }
		template <class Pred> Udm::AssocAttr< OChan, Pred> outputChannels_sorted(const Pred &) const { return Udm::AssocAttr< OChan, Pred>(impl, meta_outputChannels); }
		Udm::ParentAttr< ::Semantics::Deployment> parent() const { return Udm::ParentAttr< ::Semantics::Deployment>(impl, Udm::NULLPARENTROLE); }
		void Accept(Visitor &v) { v.Visit_OutputDevice(*this); }

		static ::Uml::Class meta;
		static ::Uml::AssociationRole meta_outputEnvironment;
		static ::Uml::AssociationRole meta_outputChannels;

	};

	class InputDevice :  public Device {
	public:
		InputDevice() {}
		InputDevice(Udm::ObjectImpl *impl) : Device(impl) {}
		InputDevice(const InputDevice &master) : Device(master) {}
		#ifdef UDM_RVALUE
		InputDevice(InputDevice &&master) : Device(master) {}

		static InputDevice Cast(Udm::Object &&a) { return __Cast(a, meta); }
		InputDevice& operator=(InputDevice &&a) { Udm::Object::operator =(a); return *this; }
		#endif
		static InputDevice Cast(const Udm::Object &a) { return __Cast(a, meta); }
		static InputDevice Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }
		InputDevice CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }
		Udm::InstantiatedAttr<InputDevice> Instances() { return Udm::InstantiatedAttr<InputDevice>(impl); }
		template <class Pred> Udm::InstantiatedAttr<InputDevice, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<InputDevice, Pred>(impl); }
		InputDevice CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }
		Udm::DerivedAttr<InputDevice> Derived() { return Udm::DerivedAttr<InputDevice>(impl); }
		template <class Pred> Udm::DerivedAttr<InputDevice, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<InputDevice, Pred>(impl); }
		Udm::ArchetypeAttr<InputDevice> Archetype() const { return Udm::ArchetypeAttr<InputDevice>(impl); }
		Udm::PointerAttr< Plant> inputEnvironment() const { return Udm::PointerAttr< Plant>(impl, meta_inputEnvironment); }
		Udm::AssocAttr< IChan> inputChannels() const { return Udm::AssocAttr< IChan>(impl, meta_inputChannels); }
		template <class Pred> Udm::AssocAttr< IChan, Pred> inputChannels_sorted(const Pred &) const { return Udm::AssocAttr< IChan, Pred>(impl, meta_inputChannels); }
		Udm::ParentAttr< ::Semantics::Deployment> parent() const { return Udm::ParentAttr< ::Semantics::Deployment>(impl, Udm::NULLPARENTROLE); }
		void Accept(Visitor &v) { v.Visit_InputDevice(*this); }

		static ::Uml::Class meta;
		static ::Uml::AssociationRole meta_inputEnvironment;
		static ::Uml::AssociationRole meta_inputChannels;

	};

	class CommInterface :  public Device {
	public:
		CommInterface() {}
		CommInterface(Udm::ObjectImpl *impl) : Device(impl) {}
		CommInterface(const CommInterface &master) : Device(master) {}
		#ifdef UDM_RVALUE
		CommInterface(CommInterface &&master) : Device(master) {}

		static CommInterface Cast(Udm::Object &&a) { return __Cast(a, meta); }
		CommInterface& operator=(CommInterface &&a) { Udm::Object::operator =(a); return *this; }
		#endif
		static CommInterface Cast(const Udm::Object &a) { return __Cast(a, meta); }
		static CommInterface Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }
		CommInterface CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }
		Udm::InstantiatedAttr<CommInterface> Instances() { return Udm::InstantiatedAttr<CommInterface>(impl); }
		template <class Pred> Udm::InstantiatedAttr<CommInterface, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<CommInterface, Pred>(impl); }
		CommInterface CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }
		Udm::DerivedAttr<CommInterface> Derived() { return Udm::DerivedAttr<CommInterface>(impl); }
		template <class Pred> Udm::DerivedAttr<CommInterface, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<CommInterface, Pred>(impl); }
		Udm::ArchetypeAttr<CommInterface> Archetype() const { return Udm::ArchetypeAttr<CommInterface>(impl); }
		Udm::PointerAttr< CommMedium> commMedium() const { return Udm::PointerAttr< CommMedium>(impl, meta_commMedium); }
		Udm::AssocAttr< BChan> commChannels() const { return Udm::AssocAttr< BChan>(impl, meta_commChannels); }
		template <class Pred> Udm::AssocAttr< BChan, Pred> commChannels_sorted(const Pred &) const { return Udm::AssocAttr< BChan, Pred>(impl, meta_commChannels); }
		Udm::ParentAttr< ::Semantics::Deployment> parent() const { return Udm::ParentAttr< ::Semantics::Deployment>(impl, Udm::NULLPARENTROLE); }
		void Accept(Visitor &v) { v.Visit_CommInterface(*this); }

		static ::Uml::Class meta;
		static ::Uml::AssociationRole meta_commMedium;
		static ::Uml::AssociationRole meta_commChannels;

	};

	class Receives :  public CommRelations {
	public:
		Receives() {}
		Receives(Udm::ObjectImpl *impl) : CommRelations(impl) {}
		Receives(const Receives &master) : CommRelations(master) {}
		#ifdef UDM_RVALUE
		Receives(Receives &&master) : CommRelations(master) {}

		static Receives Cast(Udm::Object &&a) { return __Cast(a, meta); }
		Receives& operator=(Receives &&a) { Udm::Object::operator =(a); return *this; }
		#endif
		static Receives Cast(const Udm::Object &a) { return __Cast(a, meta); }
		static Receives Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }
		Receives CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }
		Udm::InstantiatedAttr<Receives> Instances() { return Udm::InstantiatedAttr<Receives>(impl); }
		template <class Pred> Udm::InstantiatedAttr<Receives, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<Receives, Pred>(impl); }
		Receives CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }
		Udm::DerivedAttr<Receives> Derived() { return Udm::DerivedAttr<Receives>(impl); }
		template <class Pred> Udm::DerivedAttr<Receives, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<Receives, Pred>(impl); }
		Udm::ArchetypeAttr<Receives> Archetype() const { return Udm::ArchetypeAttr<Receives>(impl); }
		Udm::PointerAttr< Msg> receivedMsg() const { return Udm::PointerAttr< Msg>(impl, meta_receivedMsg); }
		Udm::PointerAttr< BChan> receivingChannel() const { return Udm::PointerAttr< BChan>(impl, meta_receivingChannel); }
		Udm::PointerAttr< Task> receivingTask() const { return Udm::PointerAttr< Task>(impl, meta_receivingTask); }
		Udm::ParentAttr< ::Semantics::Deployment> parent() const { return Udm::ParentAttr< ::Semantics::Deployment>(impl, Udm::NULLPARENTROLE); }
		void Accept(Visitor &v) { v.Visit_Receives(*this); }

		static ::Uml::Class meta;
		static ::Uml::AssociationRole meta_receivedMsg;
		static ::Uml::AssociationRole meta_receivingChannel;
		static ::Uml::AssociationRole meta_receivingTask;

	};

	class Transmits :  public CommRelations {
	public:
		Transmits() {}
		Transmits(Udm::ObjectImpl *impl) : CommRelations(impl) {}
		Transmits(const Transmits &master) : CommRelations(master) {}
		#ifdef UDM_RVALUE
		Transmits(Transmits &&master) : CommRelations(master) {}

		static Transmits Cast(Udm::Object &&a) { return __Cast(a, meta); }
		Transmits& operator=(Transmits &&a) { Udm::Object::operator =(a); return *this; }
		#endif
		static Transmits Cast(const Udm::Object &a) { return __Cast(a, meta); }
		static Transmits Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }
		Transmits CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }
		Udm::InstantiatedAttr<Transmits> Instances() { return Udm::InstantiatedAttr<Transmits>(impl); }
		template <class Pred> Udm::InstantiatedAttr<Transmits, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<Transmits, Pred>(impl); }
		Transmits CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }
		Udm::DerivedAttr<Transmits> Derived() { return Udm::DerivedAttr<Transmits>(impl); }
		template <class Pred> Udm::DerivedAttr<Transmits, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<Transmits, Pred>(impl); }
		Udm::ArchetypeAttr<Transmits> Archetype() const { return Udm::ArchetypeAttr<Transmits>(impl); }
		Udm::PointerAttr< Msg> transmittedMsg() const { return Udm::PointerAttr< Msg>(impl, meta_transmittedMsg); }
		Udm::PointerAttr< BChan> sendingChan() const { return Udm::PointerAttr< BChan>(impl, meta_sendingChan); }
		Udm::PointerAttr< Task> sendingTask() const { return Udm::PointerAttr< Task>(impl, meta_sendingTask); }
		Udm::ParentAttr< ::Semantics::Deployment> parent() const { return Udm::ParentAttr< ::Semantics::Deployment>(impl, Udm::NULLPARENTROLE); }
		void Accept(Visitor &v) { v.Visit_Transmits(*this); }

		static ::Uml::Class meta;
		static ::Uml::AssociationRole meta_transmittedMsg;
		static ::Uml::AssociationRole meta_sendingChan;
		static ::Uml::AssociationRole meta_sendingTask;

	};

	class Actuation :  public CommRelations {
	public:
		Actuation() {}
		Actuation(Udm::ObjectImpl *impl) : CommRelations(impl) {}
		Actuation(const Actuation &master) : CommRelations(master) {}
		#ifdef UDM_RVALUE
		Actuation(Actuation &&master) : CommRelations(master) {}

		static Actuation Cast(Udm::Object &&a) { return __Cast(a, meta); }
		Actuation& operator=(Actuation &&a) { Udm::Object::operator =(a); return *this; }
		#endif
		static Actuation Cast(const Udm::Object &a) { return __Cast(a, meta); }
		static Actuation Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }
		Actuation CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }
		Udm::InstantiatedAttr<Actuation> Instances() { return Udm::InstantiatedAttr<Actuation>(impl); }
		template <class Pred> Udm::InstantiatedAttr<Actuation, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<Actuation, Pred>(impl); }
		Actuation CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }
		Udm::DerivedAttr<Actuation> Derived() { return Udm::DerivedAttr<Actuation>(impl); }
		template <class Pred> Udm::DerivedAttr<Actuation, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<Actuation, Pred>(impl); }
		Udm::ArchetypeAttr<Actuation> Archetype() const { return Udm::ArchetypeAttr<Actuation>(impl); }
		Udm::PointerAttr< OChan> actuatingChannel() const { return Udm::PointerAttr< OChan>(impl, meta_actuatingChannel); }
		Udm::PointerAttr< Msg> actuatingMsg() const { return Udm::PointerAttr< Msg>(impl, meta_actuatingMsg); }
		Udm::PointerAttr< Task> actuatingTask() const { return Udm::PointerAttr< Task>(impl, meta_actuatingTask); }
		Udm::ParentAttr< ::Semantics::Deployment> parent() const { return Udm::ParentAttr< ::Semantics::Deployment>(impl, Udm::NULLPARENTROLE); }
		void Accept(Visitor &v) { v.Visit_Actuation(*this); }

		static ::Uml::Class meta;
		static ::Uml::AssociationRole meta_actuatingChannel;
		static ::Uml::AssociationRole meta_actuatingMsg;
		static ::Uml::AssociationRole meta_actuatingTask;

	};

	class Acquisition :  public CommRelations {
	public:
		Acquisition() {}
		Acquisition(Udm::ObjectImpl *impl) : CommRelations(impl) {}
		Acquisition(const Acquisition &master) : CommRelations(master) {}
		#ifdef UDM_RVALUE
		Acquisition(Acquisition &&master) : CommRelations(master) {}

		static Acquisition Cast(Udm::Object &&a) { return __Cast(a, meta); }
		Acquisition& operator=(Acquisition &&a) { Udm::Object::operator =(a); return *this; }
		#endif
		static Acquisition Cast(const Udm::Object &a) { return __Cast(a, meta); }
		static Acquisition Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }
		Acquisition CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }
		Udm::InstantiatedAttr<Acquisition> Instances() { return Udm::InstantiatedAttr<Acquisition>(impl); }
		template <class Pred> Udm::InstantiatedAttr<Acquisition, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<Acquisition, Pred>(impl); }
		Acquisition CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }
		Udm::DerivedAttr<Acquisition> Derived() { return Udm::DerivedAttr<Acquisition>(impl); }
		template <class Pred> Udm::DerivedAttr<Acquisition, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<Acquisition, Pred>(impl); }
		Udm::ArchetypeAttr<Acquisition> Archetype() const { return Udm::ArchetypeAttr<Acquisition>(impl); }
		Udm::AssocAttr< Task> acquiringTasks() const { return Udm::AssocAttr< Task>(impl, meta_acquiringTasks); }
		template <class Pred> Udm::AssocAttr< Task, Pred> acquiringTasks_sorted(const Pred &) const { return Udm::AssocAttr< Task, Pred>(impl, meta_acquiringTasks); }
		Udm::PointerAttr< Msg> acquiredMsg() const { return Udm::PointerAttr< Msg>(impl, meta_acquiredMsg); }
		Udm::PointerAttr< IChan> acquiringChannel() const { return Udm::PointerAttr< IChan>(impl, meta_acquiringChannel); }
		Udm::ParentAttr< ::Semantics::Deployment> parent() const { return Udm::ParentAttr< ::Semantics::Deployment>(impl, Udm::NULLPARENTROLE); }
		void Accept(Visitor &v) { v.Visit_Acquisition(*this); }

		static ::Uml::Class meta;
		static ::Uml::AssociationRole meta_acquiringTasks;
		static ::Uml::AssociationRole meta_acquiredMsg;
		static ::Uml::AssociationRole meta_acquiringChannel;

	};

	class OChan :  public Channel {
	public:
		OChan() {}
		OChan(Udm::ObjectImpl *impl) : Channel(impl) {}
		OChan(const OChan &master) : Channel(master) {}
		#ifdef UDM_RVALUE
		OChan(OChan &&master) : Channel(master) {}

		static OChan Cast(Udm::Object &&a) { return __Cast(a, meta); }
		OChan& operator=(OChan &&a) { Udm::Object::operator =(a); return *this; }
		#endif
		static OChan Cast(const Udm::Object &a) { return __Cast(a, meta); }
		static OChan Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }
		OChan CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }
		Udm::InstantiatedAttr<OChan> Instances() { return Udm::InstantiatedAttr<OChan>(impl); }
		template <class Pred> Udm::InstantiatedAttr<OChan, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<OChan, Pred>(impl); }
		OChan CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }
		Udm::DerivedAttr<OChan> Derived() { return Udm::DerivedAttr<OChan>(impl); }
		template <class Pred> Udm::DerivedAttr<OChan, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<OChan, Pred>(impl); }
		Udm::ArchetypeAttr<OChan> Archetype() const { return Udm::ArchetypeAttr<OChan>(impl); }
		Udm::AssocAttr< Actuation> channelActuations() const { return Udm::AssocAttr< Actuation>(impl, meta_channelActuations); }
		template <class Pred> Udm::AssocAttr< Actuation, Pred> channelActuations_sorted(const Pred &) const { return Udm::AssocAttr< Actuation, Pred>(impl, meta_channelActuations); }
		Udm::PointerAttr< OutputDevice> ochanDevice() const { return Udm::PointerAttr< OutputDevice>(impl, meta_ochanDevice); }
		Udm::ParentAttr< ::Semantics::Deployment> parent() const { return Udm::ParentAttr< ::Semantics::Deployment>(impl, Udm::NULLPARENTROLE); }
		void Accept(Visitor &v) { v.Visit_OChan(*this); }

		static ::Uml::Class meta;
		static ::Uml::AssociationRole meta_channelActuations;
		static ::Uml::AssociationRole meta_ochanDevice;

	};

	class BChan :  public Channel {
	public:
		BChan() {}
		BChan(Udm::ObjectImpl *impl) : Channel(impl) {}
		BChan(const BChan &master) : Channel(master) {}
		#ifdef UDM_RVALUE
		BChan(BChan &&master) : Channel(master) {}

		static BChan Cast(Udm::Object &&a) { return __Cast(a, meta); }
		BChan& operator=(BChan &&a) { Udm::Object::operator =(a); return *this; }
		#endif
		static BChan Cast(const Udm::Object &a) { return __Cast(a, meta); }
		static BChan Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }
		BChan CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }
		Udm::InstantiatedAttr<BChan> Instances() { return Udm::InstantiatedAttr<BChan>(impl); }
		template <class Pred> Udm::InstantiatedAttr<BChan, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<BChan, Pred>(impl); }
		BChan CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }
		Udm::DerivedAttr<BChan> Derived() { return Udm::DerivedAttr<BChan>(impl); }
		template <class Pred> Udm::DerivedAttr<BChan, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<BChan, Pred>(impl); }
		Udm::ArchetypeAttr<BChan> Archetype() const { return Udm::ArchetypeAttr<BChan>(impl); }
		Udm::AssocAttr< Receives> channelListeners() const { return Udm::AssocAttr< Receives>(impl, meta_channelListeners); }
		template <class Pred> Udm::AssocAttr< Receives, Pred> channelListeners_sorted(const Pred &) const { return Udm::AssocAttr< Receives, Pred>(impl, meta_channelListeners); }
		Udm::AssocAttr< Transmits> channelTransmitters() const { return Udm::AssocAttr< Transmits>(impl, meta_channelTransmitters); }
		template <class Pred> Udm::AssocAttr< Transmits, Pred> channelTransmitters_sorted(const Pred &) const { return Udm::AssocAttr< Transmits, Pred>(impl, meta_channelTransmitters); }
		Udm::PointerAttr< CommInterface> chanInterface() const { return Udm::PointerAttr< CommInterface>(impl, meta_chanInterface); }
		Udm::ParentAttr< ::Semantics::Deployment> parent() const { return Udm::ParentAttr< ::Semantics::Deployment>(impl, Udm::NULLPARENTROLE); }
		void Accept(Visitor &v) { v.Visit_BChan(*this); }

		static ::Uml::Class meta;
		static ::Uml::AssociationRole meta_channelListeners;
		static ::Uml::AssociationRole meta_channelTransmitters;
		static ::Uml::AssociationRole meta_chanInterface;

	};

	class IChan :  public Channel {
	public:
		IChan() {}
		IChan(Udm::ObjectImpl *impl) : Channel(impl) {}
		IChan(const IChan &master) : Channel(master) {}
		#ifdef UDM_RVALUE
		IChan(IChan &&master) : Channel(master) {}

		static IChan Cast(Udm::Object &&a) { return __Cast(a, meta); }
		IChan& operator=(IChan &&a) { Udm::Object::operator =(a); return *this; }
		#endif
		static IChan Cast(const Udm::Object &a) { return __Cast(a, meta); }
		static IChan Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }
		IChan CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }
		Udm::InstantiatedAttr<IChan> Instances() { return Udm::InstantiatedAttr<IChan>(impl); }
		template <class Pred> Udm::InstantiatedAttr<IChan, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<IChan, Pred>(impl); }
		IChan CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }
		Udm::DerivedAttr<IChan> Derived() { return Udm::DerivedAttr<IChan>(impl); }
		template <class Pred> Udm::DerivedAttr<IChan, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<IChan, Pred>(impl); }
		Udm::ArchetypeAttr<IChan> Archetype() const { return Udm::ArchetypeAttr<IChan>(impl); }
		Udm::PointerAttr< Acquisition> channelAcquisition() const { return Udm::PointerAttr< Acquisition>(impl, meta_channelAcquisition); }
		Udm::PointerAttr< InputDevice> ichanDevice() const { return Udm::PointerAttr< InputDevice>(impl, meta_ichanDevice); }
		Udm::ParentAttr< ::Semantics::Deployment> parent() const { return Udm::ParentAttr< ::Semantics::Deployment>(impl, Udm::NULLPARENTROLE); }
		void Accept(Visitor &v) { v.Visit_IChan(*this); }

		static ::Uml::Class meta;
		static ::Uml::AssociationRole meta_channelAcquisition;
		static ::Uml::AssociationRole meta_ichanDevice;

	};

	class CommMedium : public Udm::Object {
	public:
		CommMedium() {}
		CommMedium(Udm::ObjectImpl *impl) : UDM_OBJECT(impl) {}
		CommMedium(const CommMedium &master) : UDM_OBJECT(master) {}
		#ifdef UDM_RVALUE
		CommMedium(CommMedium &&master) : UDM_OBJECT(master) {}

		static CommMedium Cast(Udm::Object &&a) { return __Cast(a, meta); }
		CommMedium& operator=(CommMedium &&a) { Udm::Object::operator =(a); return *this; }
		#endif
		static CommMedium Cast(const Udm::Object &a) { return __Cast(a, meta); }
		static CommMedium Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }
		CommMedium CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }
		Udm::InstantiatedAttr<CommMedium> Instances() { return Udm::InstantiatedAttr<CommMedium>(impl); }
		template <class Pred> Udm::InstantiatedAttr<CommMedium, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<CommMedium, Pred>(impl); }
		CommMedium CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }
		Udm::DerivedAttr<CommMedium> Derived() { return Udm::DerivedAttr<CommMedium>(impl); }
		template <class Pred> Udm::DerivedAttr<CommMedium, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<CommMedium, Pred>(impl); }
		Udm::ArchetypeAttr<CommMedium> Archetype() const { return Udm::ArchetypeAttr<CommMedium>(impl); }
		Udm::StringAttr name() const { return Udm::StringAttr(impl, meta_name); }
		Udm::IntegerAttr id() const { return Udm::IntegerAttr(impl, meta_id); }
		Udm::RealAttr dataratebps() const { return Udm::RealAttr(impl, meta_dataratebps); }
		Udm::AssocAttr< CommInterface> interfaces() const { return Udm::AssocAttr< CommInterface>(impl, meta_interfaces); }
		template <class Pred> Udm::AssocAttr< CommInterface, Pred> interfaces_sorted(const Pred &) const { return Udm::AssocAttr< CommInterface, Pred>(impl, meta_interfaces); }
		Udm::ParentAttr< ::Semantics::Deployment> Deployment_parent() const { return Udm::ParentAttr< ::Semantics::Deployment>(impl, meta_Deployment_parent); }
		Udm::ParentAttr< ::Semantics::Deployment> parent() const { return Udm::ParentAttr< ::Semantics::Deployment>(impl, Udm::NULLPARENTROLE); }
		void Accept(Visitor &v) { v.Visit_CommMedium(*this); }

		static ::Uml::Class meta;
		static ::Uml::Attribute meta_name;
		static ::Uml::Attribute meta_id;
		static ::Uml::Attribute meta_dataratebps;
		static ::Uml::AssociationRole meta_interfaces;
		static ::Uml::CompositionParentRole meta_Deployment_parent;

	};

	class TTBus :  public CommMedium {
	public:
		TTBus() {}
		TTBus(Udm::ObjectImpl *impl) : CommMedium(impl) {}
		TTBus(const TTBus &master) : CommMedium(master) {}
		#ifdef UDM_RVALUE
		TTBus(TTBus &&master) : CommMedium(master) {}

		static TTBus Cast(Udm::Object &&a) { return __Cast(a, meta); }
		TTBus& operator=(TTBus &&a) { Udm::Object::operator =(a); return *this; }
		#endif
		static TTBus Cast(const Udm::Object &a) { return __Cast(a, meta); }
		static TTBus Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }
		TTBus CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }
		Udm::InstantiatedAttr<TTBus> Instances() { return Udm::InstantiatedAttr<TTBus>(impl); }
		template <class Pred> Udm::InstantiatedAttr<TTBus, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<TTBus, Pred>(impl); }
		TTBus CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }
		Udm::DerivedAttr<TTBus> Derived() { return Udm::DerivedAttr<TTBus>(impl); }
		template <class Pred> Udm::DerivedAttr<TTBus, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<TTBus, Pred>(impl); }
		Udm::ArchetypeAttr<TTBus> Archetype() const { return Udm::ArchetypeAttr<TTBus>(impl); }
		Udm::IntegerAttr hyperperiodticks() const { return Udm::IntegerAttr(impl, meta_hyperperiodticks); }
		Udm::RealAttr hyperperiodsecs() const { return Udm::RealAttr(impl, meta_hyperperiodsecs); }
		Udm::RealAttr setupsecs() const { return Udm::RealAttr(impl, meta_setupsecs); }
		Udm::IntegerAttr switchmembytes() const { return Udm::IntegerAttr(impl, meta_switchmembytes); }
		Udm::IntegerAttr minframebytes() const { return Udm::IntegerAttr(impl, meta_minframebytes); }
		Udm::IntegerAttr busslotbytes() const { return Udm::IntegerAttr(impl, meta_busslotbytes); }
		Udm::ParentAttr< ::Semantics::Deployment> parent() const { return Udm::ParentAttr< ::Semantics::Deployment>(impl, Udm::NULLPARENTROLE); }
		void Accept(Visitor &v) { v.Visit_TTBus(*this); }

		static ::Uml::Class meta;
		static ::Uml::Attribute meta_hyperperiodticks;
		static ::Uml::Attribute meta_hyperperiodsecs;
		static ::Uml::Attribute meta_setupsecs;
		static ::Uml::Attribute meta_switchmembytes;
		static ::Uml::Attribute meta_minframebytes;
		static ::Uml::Attribute meta_busslotbytes;

	};

	class ExecInfo : public Udm::Object {
	public:
		ExecInfo() {}
		ExecInfo(Udm::ObjectImpl *impl) : UDM_OBJECT(impl) {}
		ExecInfo(const ExecInfo &master) : UDM_OBJECT(master) {}
		#ifdef UDM_RVALUE
		ExecInfo(ExecInfo &&master) : UDM_OBJECT(master) {}

		static ExecInfo Cast(Udm::Object &&a) { return __Cast(a, meta); }
		ExecInfo& operator=(ExecInfo &&a) { Udm::Object::operator =(a); return *this; }
		#endif
		static ExecInfo Cast(const Udm::Object &a) { return __Cast(a, meta); }
		static ExecInfo Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }
		ExecInfo CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }
		Udm::InstantiatedAttr<ExecInfo> Instances() { return Udm::InstantiatedAttr<ExecInfo>(impl); }
		template <class Pred> Udm::InstantiatedAttr<ExecInfo, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<ExecInfo, Pred>(impl); }
		ExecInfo CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }
		Udm::DerivedAttr<ExecInfo> Derived() { return Udm::DerivedAttr<ExecInfo>(impl); }
		template <class Pred> Udm::DerivedAttr<ExecInfo, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<ExecInfo, Pred>(impl); }
		Udm::ArchetypeAttr<ExecInfo> Archetype() const { return Udm::ArchetypeAttr<ExecInfo>(impl); }
		Udm::StringAttr name() const { return Udm::StringAttr(impl, meta_name); }
		Udm::AssocAttr< Executable> executor() const { return Udm::AssocAttr< Executable>(impl, meta_executor); }
		template <class Pred> Udm::AssocAttr< Executable, Pred> executor_sorted(const Pred &) const { return Udm::AssocAttr< Executable, Pred>(impl, meta_executor); }
		Udm::ChildAttr< ::Semantics::Schedule> Schedule_child() const { return Udm::ChildAttr< ::Semantics::Schedule>(impl, meta_Schedule_child); }
		Udm::ChildrenAttr< ::Semantics::Periodicity> Periodicity_children() const { return Udm::ChildrenAttr< ::Semantics::Periodicity>(impl, meta_Periodicity_children); }
		template <class Pred> Udm::ChildrenAttr< ::Semantics::Periodicity, Pred> Periodicity_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::Semantics::Periodicity, Pred>(impl, meta_Periodicity_children); }
		Udm::ChildAttr< ::Semantics::Duration> Duration_child() const { return Udm::ChildAttr< ::Semantics::Duration>(impl, meta_Duration_child); }
		Udm::ChildrenAttr< ::Semantics::TTSchedule> TTSchedule_kind_children() const { return Udm::ChildrenAttr< ::Semantics::TTSchedule>(impl, Udm::NULLCHILDROLE); }
		template <class Pred> Udm::ChildrenAttr< ::Semantics::TTSchedule, Pred> TTSchedule_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::Semantics::TTSchedule, Pred>(impl, Udm::NULLCHILDROLE); }
		Udm::ChildrenAttr< ::Semantics::Schedule> Schedule_kind_children() const { return Udm::ChildrenAttr< ::Semantics::Schedule>(impl, Udm::NULLCHILDROLE); }
		template <class Pred> Udm::ChildrenAttr< ::Semantics::Schedule, Pred> Schedule_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::Semantics::Schedule, Pred>(impl, Udm::NULLCHILDROLE); }
		Udm::ChildrenAttr< ::Semantics::MaxDuration> MaxDuration_kind_children() const { return Udm::ChildrenAttr< ::Semantics::MaxDuration>(impl, Udm::NULLCHILDROLE); }
		template <class Pred> Udm::ChildrenAttr< ::Semantics::MaxDuration, Pred> MaxDuration_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::Semantics::MaxDuration, Pred>(impl, Udm::NULLCHILDROLE); }
		Udm::ChildrenAttr< ::Semantics::Duration> Duration_kind_children() const { return Udm::ChildrenAttr< ::Semantics::Duration>(impl, Udm::NULLCHILDROLE); }
		template <class Pred> Udm::ChildrenAttr< ::Semantics::Duration, Pred> Duration_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::Semantics::Duration, Pred>(impl, Udm::NULLCHILDROLE); }
		Udm::ChildrenAttr< ::Semantics::SamplePeriod> SamplePeriod_kind_children() const { return Udm::ChildrenAttr< ::Semantics::SamplePeriod>(impl, Udm::NULLCHILDROLE); }
		template <class Pred> Udm::ChildrenAttr< ::Semantics::SamplePeriod, Pred> SamplePeriod_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::Semantics::SamplePeriod, Pred>(impl, Udm::NULLCHILDROLE); }
		Udm::ChildrenAttr< ::Semantics::Periodicity> Periodicity_kind_children() const { return Udm::ChildrenAttr< ::Semantics::Periodicity>(impl, Udm::NULLCHILDROLE); }
		template <class Pred> Udm::ChildrenAttr< ::Semantics::Periodicity, Pred> Periodicity_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::Semantics::Periodicity, Pred>(impl, Udm::NULLCHILDROLE); }
		Udm::ChildrenAttr< ::Semantics::MinimumPeriod> MinimumPeriod_kind_children() const { return Udm::ChildrenAttr< ::Semantics::MinimumPeriod>(impl, Udm::NULLCHILDROLE); }
		template <class Pred> Udm::ChildrenAttr< ::Semantics::MinimumPeriod, Pred> MinimumPeriod_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::Semantics::MinimumPeriod, Pred>(impl, Udm::NULLCHILDROLE); }
		Udm::ParentAttr< ::Semantics::Deployment> Deployment_parent() const { return Udm::ParentAttr< ::Semantics::Deployment>(impl, meta_Deployment_parent); }
		Udm::ParentAttr< ::Semantics::Deployment> parent() const { return Udm::ParentAttr< ::Semantics::Deployment>(impl, Udm::NULLPARENTROLE); }
		void Accept(Visitor &v) { v.Visit_ExecInfo(*this); }

		static ::Uml::Class meta;
		static ::Uml::Attribute meta_name;
		static ::Uml::AssociationRole meta_executor;
		static ::Uml::CompositionChildRole meta_Schedule_child;
		static ::Uml::CompositionChildRole meta_Periodicity_children;
		static ::Uml::CompositionChildRole meta_Duration_child;
		static ::Uml::CompositionParentRole meta_Deployment_parent;

	};

	class RootFolder : public Udm::Object {
	public:
		RootFolder() {}
		RootFolder(Udm::ObjectImpl *impl) : UDM_OBJECT(impl) {}
		RootFolder(const RootFolder &master) : UDM_OBJECT(master) {}
		#ifdef UDM_RVALUE
		RootFolder(RootFolder &&master) : UDM_OBJECT(master) {}

		static RootFolder Cast(Udm::Object &&a) { return __Cast(a, meta); }
		RootFolder& operator=(RootFolder &&a) { Udm::Object::operator =(a); return *this; }
		#endif
		static RootFolder Cast(const Udm::Object &a) { return __Cast(a, meta); }
		static RootFolder Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }
		RootFolder CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }
		Udm::InstantiatedAttr<RootFolder> Instances() { return Udm::InstantiatedAttr<RootFolder>(impl); }
		template <class Pred> Udm::InstantiatedAttr<RootFolder, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<RootFolder, Pred>(impl); }
		RootFolder CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }
		Udm::DerivedAttr<RootFolder> Derived() { return Udm::DerivedAttr<RootFolder>(impl); }
		template <class Pred> Udm::DerivedAttr<RootFolder, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<RootFolder, Pred>(impl); }
		Udm::ArchetypeAttr<RootFolder> Archetype() const { return Udm::ArchetypeAttr<RootFolder>(impl); }
		Udm::StringAttr name() const { return Udm::StringAttr(impl, meta_name); }
		Udm::ChildrenAttr< ::Semantics::RootFolder> RootFolder_children() const { return Udm::ChildrenAttr< ::Semantics::RootFolder>(impl, meta_RootFolder_children); }
		template <class Pred> Udm::ChildrenAttr< ::Semantics::RootFolder, Pred> RootFolder_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::Semantics::RootFolder, Pred>(impl, meta_RootFolder_children); }
		Udm::ChildrenAttr< ::Semantics::Deployment> Deployment_children() const { return Udm::ChildrenAttr< ::Semantics::Deployment>(impl, meta_Deployment_children); }
		template <class Pred> Udm::ChildrenAttr< ::Semantics::Deployment, Pred> Deployment_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::Semantics::Deployment, Pred>(impl, meta_Deployment_children); }
		Udm::ChildrenAttr< ::Semantics::RootFolder> RootFolder_kind_children() const { return Udm::ChildrenAttr< ::Semantics::RootFolder>(impl, Udm::NULLCHILDROLE); }
		template <class Pred> Udm::ChildrenAttr< ::Semantics::RootFolder, Pred> RootFolder_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::Semantics::RootFolder, Pred>(impl, Udm::NULLCHILDROLE); }
		Udm::ChildrenAttr< ::Semantics::Deployment> Deployment_kind_children() const { return Udm::ChildrenAttr< ::Semantics::Deployment>(impl, Udm::NULLCHILDROLE); }
		template <class Pred> Udm::ChildrenAttr< ::Semantics::Deployment, Pred> Deployment_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::Semantics::Deployment, Pred>(impl, Udm::NULLCHILDROLE); }
		Udm::ParentAttr< ::Semantics::RootFolder> RootFolder_parent() const { return Udm::ParentAttr< ::Semantics::RootFolder>(impl, meta_RootFolder_parent); }
		Udm::ParentAttr< ::Semantics::RootFolder> parent() const { return Udm::ParentAttr< ::Semantics::RootFolder>(impl, Udm::NULLPARENTROLE); }
		void Accept(Visitor &v) { v.Visit_RootFolder(*this); }

		static ::Uml::Class meta;
		static ::Uml::Attribute meta_name;
		static ::Uml::CompositionChildRole meta_RootFolder_children;
		static ::Uml::CompositionChildRole meta_Deployment_children;
		static ::Uml::CompositionParentRole meta_RootFolder_parent;

	};

	class Deployment : public Udm::Object {
	public:
		Deployment() {}
		Deployment(Udm::ObjectImpl *impl) : UDM_OBJECT(impl) {}
		Deployment(const Deployment &master) : UDM_OBJECT(master) {}
		#ifdef UDM_RVALUE
		Deployment(Deployment &&master) : UDM_OBJECT(master) {}

		static Deployment Cast(Udm::Object &&a) { return __Cast(a, meta); }
		Deployment& operator=(Deployment &&a) { Udm::Object::operator =(a); return *this; }
		#endif
		static Deployment Cast(const Udm::Object &a) { return __Cast(a, meta); }
		static Deployment Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }
		Deployment CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }
		Udm::InstantiatedAttr<Deployment> Instances() { return Udm::InstantiatedAttr<Deployment>(impl); }
		template <class Pred> Udm::InstantiatedAttr<Deployment, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<Deployment, Pred>(impl); }
		Deployment CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }
		Udm::DerivedAttr<Deployment> Derived() { return Udm::DerivedAttr<Deployment>(impl); }
		template <class Pred> Udm::DerivedAttr<Deployment, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<Deployment, Pred>(impl); }
		Udm::ArchetypeAttr<Deployment> Archetype() const { return Udm::ArchetypeAttr<Deployment>(impl); }
		Udm::IntegerAttr id() const { return Udm::IntegerAttr(impl, meta_id); }
		Udm::StringAttr name() const { return Udm::StringAttr(impl, meta_name); }
		Udm::ChildrenAttr< ::Semantics::SignalType> SignalType_children() const { return Udm::ChildrenAttr< ::Semantics::SignalType>(impl, meta_SignalType_children); }
		template <class Pred> Udm::ChildrenAttr< ::Semantics::SignalType, Pred> SignalType_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::Semantics::SignalType, Pred>(impl, meta_SignalType_children); }
		Udm::ChildrenAttr< ::Semantics::TestPoint> TestPoint_children() const { return Udm::ChildrenAttr< ::Semantics::TestPoint>(impl, meta_TestPoint_children); }
		template <class Pred> Udm::ChildrenAttr< ::Semantics::TestPoint, Pred> TestPoint_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::Semantics::TestPoint, Pred>(impl, meta_TestPoint_children); }
		Udm::ChildrenAttr< ::Semantics::CommRelations> CommRelations_children() const { return Udm::ChildrenAttr< ::Semantics::CommRelations>(impl, meta_CommRelations_children); }
		template <class Pred> Udm::ChildrenAttr< ::Semantics::CommRelations, Pred> CommRelations_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::Semantics::CommRelations, Pred>(impl, meta_CommRelations_children); }
		Udm::ChildrenAttr< ::Semantics::Channel> Channel_children() const { return Udm::ChildrenAttr< ::Semantics::Channel>(impl, meta_Channel_children); }
		template <class Pred> Udm::ChildrenAttr< ::Semantics::Channel, Pred> Channel_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::Semantics::Channel, Pred>(impl, meta_Channel_children); }
		Udm::ChildrenAttr< ::Semantics::Plant> Plant_children() const { return Udm::ChildrenAttr< ::Semantics::Plant>(impl, meta_Plant_children); }
		template <class Pred> Udm::ChildrenAttr< ::Semantics::Plant, Pred> Plant_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::Semantics::Plant, Pred>(impl, meta_Plant_children); }
		Udm::ChildrenAttr< ::Semantics::Msg> Msg_children() const { return Udm::ChildrenAttr< ::Semantics::Msg>(impl, meta_Msg_children); }
		template <class Pred> Udm::ChildrenAttr< ::Semantics::Msg, Pred> Msg_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::Semantics::Msg, Pred>(impl, meta_Msg_children); }
		Udm::ChildrenAttr< ::Semantics::CommMedium> CommMedium_children() const { return Udm::ChildrenAttr< ::Semantics::CommMedium>(impl, meta_CommMedium_children); }
		template <class Pred> Udm::ChildrenAttr< ::Semantics::CommMedium, Pred> CommMedium_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::Semantics::CommMedium, Pred>(impl, meta_CommMedium_children); }
		Udm::ChildrenAttr< ::Semantics::Task> Task_children() const { return Udm::ChildrenAttr< ::Semantics::Task>(impl, meta_Task_children); }
		template <class Pred> Udm::ChildrenAttr< ::Semantics::Task, Pred> Task_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::Semantics::Task, Pred>(impl, meta_Task_children); }
		Udm::ChildrenAttr< ::Semantics::Signal> Signal_children() const { return Udm::ChildrenAttr< ::Semantics::Signal>(impl, meta_Signal_children); }
		template <class Pred> Udm::ChildrenAttr< ::Semantics::Signal, Pred> Signal_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::Semantics::Signal, Pred>(impl, meta_Signal_children); }
		Udm::ChildrenAttr< ::Semantics::Device> Device_children() const { return Udm::ChildrenAttr< ::Semantics::Device>(impl, meta_Device_children); }
		template <class Pred> Udm::ChildrenAttr< ::Semantics::Device, Pred> Device_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::Semantics::Device, Pred>(impl, meta_Device_children); }
		Udm::ChildrenAttr< ::Semantics::Component> Component_children() const { return Udm::ChildrenAttr< ::Semantics::Component>(impl, meta_Component_children); }
		template <class Pred> Udm::ChildrenAttr< ::Semantics::Component, Pred> Component_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::Semantics::Component, Pred>(impl, meta_Component_children); }
		Udm::ChildrenAttr< ::Semantics::Node> Node_children() const { return Udm::ChildrenAttr< ::Semantics::Node>(impl, meta_Node_children); }
		template <class Pred> Udm::ChildrenAttr< ::Semantics::Node, Pred> Node_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::Semantics::Node, Pred>(impl, meta_Node_children); }
		Udm::ChildrenAttr< ::Semantics::ExecInfo> ExecInfo_children() const { return Udm::ChildrenAttr< ::Semantics::ExecInfo>(impl, meta_ExecInfo_children); }
		template <class Pred> Udm::ChildrenAttr< ::Semantics::ExecInfo, Pred> ExecInfo_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::Semantics::ExecInfo, Pred>(impl, meta_ExecInfo_children); }
		Udm::ChildrenAttr< ::Semantics::TimingConstraint> TimingConstraint_children() const { return Udm::ChildrenAttr< ::Semantics::TimingConstraint>(impl, meta_TimingConstraint_children); }
		template <class Pred> Udm::ChildrenAttr< ::Semantics::TimingConstraint, Pred> TimingConstraint_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::Semantics::TimingConstraint, Pred>(impl, meta_TimingConstraint_children); }
		Udm::ChildrenAttr< ::Semantics::Executable> Executable_kind_children() const { return Udm::ChildrenAttr< ::Semantics::Executable>(impl, Udm::NULLCHILDROLE); }
		template <class Pred> Udm::ChildrenAttr< ::Semantics::Executable, Pred> Executable_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::Semantics::Executable, Pred>(impl, Udm::NULLCHILDROLE); }
		Udm::ChildrenAttr< ::Semantics::Channel> Channel_kind_children() const { return Udm::ChildrenAttr< ::Semantics::Channel>(impl, Udm::NULLCHILDROLE); }
		template <class Pred> Udm::ChildrenAttr< ::Semantics::Channel, Pred> Channel_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::Semantics::Channel, Pred>(impl, Udm::NULLCHILDROLE); }
		Udm::ChildrenAttr< ::Semantics::Device> Device_kind_children() const { return Udm::ChildrenAttr< ::Semantics::Device>(impl, Udm::NULLCHILDROLE); }
		template <class Pred> Udm::ChildrenAttr< ::Semantics::Device, Pred> Device_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::Semantics::Device, Pred>(impl, Udm::NULLCHILDROLE); }
		Udm::ChildrenAttr< ::Semantics::Msg> Msg_kind_children() const { return Udm::ChildrenAttr< ::Semantics::Msg>(impl, Udm::NULLCHILDROLE); }
		template <class Pred> Udm::ChildrenAttr< ::Semantics::Msg, Pred> Msg_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::Semantics::Msg, Pred>(impl, Udm::NULLCHILDROLE); }
		Udm::ChildrenAttr< ::Semantics::Task> Task_kind_children() const { return Udm::ChildrenAttr< ::Semantics::Task>(impl, Udm::NULLCHILDROLE); }
		template <class Pred> Udm::ChildrenAttr< ::Semantics::Task, Pred> Task_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::Semantics::Task, Pred>(impl, Udm::NULLCHILDROLE); }
		Udm::ChildrenAttr< ::Semantics::DummyDependency> DummyDependency_kind_children() const { return Udm::ChildrenAttr< ::Semantics::DummyDependency>(impl, Udm::NULLCHILDROLE); }
		template <class Pred> Udm::ChildrenAttr< ::Semantics::DummyDependency, Pred> DummyDependency_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::Semantics::DummyDependency, Pred>(impl, Udm::NULLCHILDROLE); }
		Udm::ChildrenAttr< ::Semantics::CommRelations> CommRelations_kind_children() const { return Udm::ChildrenAttr< ::Semantics::CommRelations>(impl, Udm::NULLCHILDROLE); }
		template <class Pred> Udm::ChildrenAttr< ::Semantics::CommRelations, Pred> CommRelations_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::Semantics::CommRelations, Pred>(impl, Udm::NULLCHILDROLE); }
		Udm::ChildrenAttr< ::Semantics::LocalDependency> LocalDependency_kind_children() const { return Udm::ChildrenAttr< ::Semantics::LocalDependency>(impl, Udm::NULLCHILDROLE); }
		template <class Pred> Udm::ChildrenAttr< ::Semantics::LocalDependency, Pred> LocalDependency_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::Semantics::LocalDependency, Pred>(impl, Udm::NULLCHILDROLE); }
		Udm::ChildrenAttr< ::Semantics::TestPoint> TestPoint_kind_children() const { return Udm::ChildrenAttr< ::Semantics::TestPoint>(impl, Udm::NULLCHILDROLE); }
		template <class Pred> Udm::ChildrenAttr< ::Semantics::TestPoint, Pred> TestPoint_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::Semantics::TestPoint, Pred>(impl, Udm::NULLCHILDROLE); }
		Udm::ChildrenAttr< ::Semantics::Sink> Sink_kind_children() const { return Udm::ChildrenAttr< ::Semantics::Sink>(impl, Udm::NULLCHILDROLE); }
		template <class Pred> Udm::ChildrenAttr< ::Semantics::Sink, Pred> Sink_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::Semantics::Sink, Pred>(impl, Udm::NULLCHILDROLE); }
		Udm::ChildrenAttr< ::Semantics::Source> Source_kind_children() const { return Udm::ChildrenAttr< ::Semantics::Source>(impl, Udm::NULLCHILDROLE); }
		template <class Pred> Udm::ChildrenAttr< ::Semantics::Source, Pred> Source_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::Semantics::Source, Pred>(impl, Udm::NULLCHILDROLE); }
		Udm::ChildrenAttr< ::Semantics::OutputDevice> OutputDevice_kind_children() const { return Udm::ChildrenAttr< ::Semantics::OutputDevice>(impl, Udm::NULLCHILDROLE); }
		template <class Pred> Udm::ChildrenAttr< ::Semantics::OutputDevice, Pred> OutputDevice_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::Semantics::OutputDevice, Pred>(impl, Udm::NULLCHILDROLE); }
		Udm::ChildrenAttr< ::Semantics::InputDevice> InputDevice_kind_children() const { return Udm::ChildrenAttr< ::Semantics::InputDevice>(impl, Udm::NULLCHILDROLE); }
		template <class Pred> Udm::ChildrenAttr< ::Semantics::InputDevice, Pred> InputDevice_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::Semantics::InputDevice, Pred>(impl, Udm::NULLCHILDROLE); }
		Udm::ChildrenAttr< ::Semantics::CommInterface> CommInterface_kind_children() const { return Udm::ChildrenAttr< ::Semantics::CommInterface>(impl, Udm::NULLCHILDROLE); }
		template <class Pred> Udm::ChildrenAttr< ::Semantics::CommInterface, Pred> CommInterface_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::Semantics::CommInterface, Pred>(impl, Udm::NULLCHILDROLE); }
		Udm::ChildrenAttr< ::Semantics::Receives> Receives_kind_children() const { return Udm::ChildrenAttr< ::Semantics::Receives>(impl, Udm::NULLCHILDROLE); }
		template <class Pred> Udm::ChildrenAttr< ::Semantics::Receives, Pred> Receives_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::Semantics::Receives, Pred>(impl, Udm::NULLCHILDROLE); }
		Udm::ChildrenAttr< ::Semantics::Transmits> Transmits_kind_children() const { return Udm::ChildrenAttr< ::Semantics::Transmits>(impl, Udm::NULLCHILDROLE); }
		template <class Pred> Udm::ChildrenAttr< ::Semantics::Transmits, Pred> Transmits_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::Semantics::Transmits, Pred>(impl, Udm::NULLCHILDROLE); }
		Udm::ChildrenAttr< ::Semantics::Actuation> Actuation_kind_children() const { return Udm::ChildrenAttr< ::Semantics::Actuation>(impl, Udm::NULLCHILDROLE); }
		template <class Pred> Udm::ChildrenAttr< ::Semantics::Actuation, Pred> Actuation_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::Semantics::Actuation, Pred>(impl, Udm::NULLCHILDROLE); }
		Udm::ChildrenAttr< ::Semantics::Acquisition> Acquisition_kind_children() const { return Udm::ChildrenAttr< ::Semantics::Acquisition>(impl, Udm::NULLCHILDROLE); }
		template <class Pred> Udm::ChildrenAttr< ::Semantics::Acquisition, Pred> Acquisition_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::Semantics::Acquisition, Pred>(impl, Udm::NULLCHILDROLE); }
		Udm::ChildrenAttr< ::Semantics::OChan> OChan_kind_children() const { return Udm::ChildrenAttr< ::Semantics::OChan>(impl, Udm::NULLCHILDROLE); }
		template <class Pred> Udm::ChildrenAttr< ::Semantics::OChan, Pred> OChan_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::Semantics::OChan, Pred>(impl, Udm::NULLCHILDROLE); }
		Udm::ChildrenAttr< ::Semantics::BChan> BChan_kind_children() const { return Udm::ChildrenAttr< ::Semantics::BChan>(impl, Udm::NULLCHILDROLE); }
		template <class Pred> Udm::ChildrenAttr< ::Semantics::BChan, Pred> BChan_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::Semantics::BChan, Pred>(impl, Udm::NULLCHILDROLE); }
		Udm::ChildrenAttr< ::Semantics::IChan> IChan_kind_children() const { return Udm::ChildrenAttr< ::Semantics::IChan>(impl, Udm::NULLCHILDROLE); }
		template <class Pred> Udm::ChildrenAttr< ::Semantics::IChan, Pred> IChan_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::Semantics::IChan, Pred>(impl, Udm::NULLCHILDROLE); }
		Udm::ChildrenAttr< ::Semantics::TTBus> TTBus_kind_children() const { return Udm::ChildrenAttr< ::Semantics::TTBus>(impl, Udm::NULLCHILDROLE); }
		template <class Pred> Udm::ChildrenAttr< ::Semantics::TTBus, Pred> TTBus_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::Semantics::TTBus, Pred>(impl, Udm::NULLCHILDROLE); }
		Udm::ChildrenAttr< ::Semantics::CommMedium> CommMedium_kind_children() const { return Udm::ChildrenAttr< ::Semantics::CommMedium>(impl, Udm::NULLCHILDROLE); }
		template <class Pred> Udm::ChildrenAttr< ::Semantics::CommMedium, Pred> CommMedium_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::Semantics::CommMedium, Pred>(impl, Udm::NULLCHILDROLE); }
		Udm::ChildrenAttr< ::Semantics::ExecInfo> ExecInfo_kind_children() const { return Udm::ChildrenAttr< ::Semantics::ExecInfo>(impl, Udm::NULLCHILDROLE); }
		template <class Pred> Udm::ChildrenAttr< ::Semantics::ExecInfo, Pred> ExecInfo_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::Semantics::ExecInfo, Pred>(impl, Udm::NULLCHILDROLE); }
		Udm::ChildrenAttr< ::Semantics::SignalType> SignalType_kind_children() const { return Udm::ChildrenAttr< ::Semantics::SignalType>(impl, Udm::NULLCHILDROLE); }
		template <class Pred> Udm::ChildrenAttr< ::Semantics::SignalType, Pred> SignalType_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::Semantics::SignalType, Pred>(impl, Udm::NULLCHILDROLE); }
		Udm::ChildrenAttr< ::Semantics::Plant> Plant_kind_children() const { return Udm::ChildrenAttr< ::Semantics::Plant>(impl, Udm::NULLCHILDROLE); }
		template <class Pred> Udm::ChildrenAttr< ::Semantics::Plant, Pred> Plant_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::Semantics::Plant, Pred>(impl, Udm::NULLCHILDROLE); }
		Udm::ChildrenAttr< ::Semantics::Signal> Signal_kind_children() const { return Udm::ChildrenAttr< ::Semantics::Signal>(impl, Udm::NULLCHILDROLE); }
		template <class Pred> Udm::ChildrenAttr< ::Semantics::Signal, Pred> Signal_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::Semantics::Signal, Pred>(impl, Udm::NULLCHILDROLE); }
		Udm::ChildrenAttr< ::Semantics::Component> Component_kind_children() const { return Udm::ChildrenAttr< ::Semantics::Component>(impl, Udm::NULLCHILDROLE); }
		template <class Pred> Udm::ChildrenAttr< ::Semantics::Component, Pred> Component_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::Semantics::Component, Pred>(impl, Udm::NULLCHILDROLE); }
		Udm::ChildrenAttr< ::Semantics::Node> Node_kind_children() const { return Udm::ChildrenAttr< ::Semantics::Node>(impl, Udm::NULLCHILDROLE); }
		template <class Pred> Udm::ChildrenAttr< ::Semantics::Node, Pred> Node_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::Semantics::Node, Pred>(impl, Udm::NULLCHILDROLE); }
		Udm::ChildrenAttr< ::Semantics::TimingConstraint> TimingConstraint_kind_children() const { return Udm::ChildrenAttr< ::Semantics::TimingConstraint>(impl, Udm::NULLCHILDROLE); }
		template <class Pred> Udm::ChildrenAttr< ::Semantics::TimingConstraint, Pred> TimingConstraint_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::Semantics::TimingConstraint, Pred>(impl, Udm::NULLCHILDROLE); }
		Udm::ParentAttr< ::Semantics::RootFolder> RootFolder_parent() const { return Udm::ParentAttr< ::Semantics::RootFolder>(impl, meta_RootFolder_parent); }
		Udm::ParentAttr< ::Semantics::RootFolder> parent() const { return Udm::ParentAttr< ::Semantics::RootFolder>(impl, Udm::NULLPARENTROLE); }
		void Accept(Visitor &v) { v.Visit_Deployment(*this); }

		static ::Uml::Class meta;
		static ::Uml::Attribute meta_id;
		static ::Uml::Attribute meta_name;
		static ::Uml::CompositionChildRole meta_SignalType_children;
		static ::Uml::CompositionChildRole meta_TestPoint_children;
		static ::Uml::CompositionChildRole meta_CommRelations_children;
		static ::Uml::CompositionChildRole meta_Channel_children;
		static ::Uml::CompositionChildRole meta_Plant_children;
		static ::Uml::CompositionChildRole meta_Msg_children;
		static ::Uml::CompositionChildRole meta_CommMedium_children;
		static ::Uml::CompositionChildRole meta_Task_children;
		static ::Uml::CompositionChildRole meta_Signal_children;
		static ::Uml::CompositionChildRole meta_Device_children;
		static ::Uml::CompositionChildRole meta_Component_children;
		static ::Uml::CompositionChildRole meta_Node_children;
		static ::Uml::CompositionChildRole meta_ExecInfo_children;
		static ::Uml::CompositionChildRole meta_TimingConstraint_children;
		static ::Uml::CompositionParentRole meta_RootFolder_parent;

	};

	class SignalType : public Udm::Object {
	public:
		SignalType() {}
		SignalType(Udm::ObjectImpl *impl) : UDM_OBJECT(impl) {}
		SignalType(const SignalType &master) : UDM_OBJECT(master) {}
		#ifdef UDM_RVALUE
		SignalType(SignalType &&master) : UDM_OBJECT(master) {}

		static SignalType Cast(Udm::Object &&a) { return __Cast(a, meta); }
		SignalType& operator=(SignalType &&a) { Udm::Object::operator =(a); return *this; }
		#endif
		static SignalType Cast(const Udm::Object &a) { return __Cast(a, meta); }
		static SignalType Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }
		SignalType CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }
		Udm::InstantiatedAttr<SignalType> Instances() { return Udm::InstantiatedAttr<SignalType>(impl); }
		template <class Pred> Udm::InstantiatedAttr<SignalType, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<SignalType, Pred>(impl); }
		SignalType CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }
		Udm::DerivedAttr<SignalType> Derived() { return Udm::DerivedAttr<SignalType>(impl); }
		template <class Pred> Udm::DerivedAttr<SignalType, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<SignalType, Pred>(impl); }
		Udm::ArchetypeAttr<SignalType> Archetype() const { return Udm::ArchetypeAttr<SignalType>(impl); }
		Udm::StringAttr name() const { return Udm::StringAttr(impl, meta_name); }
		Udm::IntegerAttr id() const { return Udm::IntegerAttr(impl, meta_id); }
		Udm::StringAttr mtypestr() const { return Udm::StringAttr(impl, meta_mtypestr); }
		Udm::StringAttr ctypestr() const { return Udm::StringAttr(impl, meta_ctypestr); }
		Udm::StringAttr rows() const { return Udm::StringAttr(impl, meta_rows); }
		Udm::StringAttr columns() const { return Udm::StringAttr(impl, meta_columns); }
		Udm::BooleanAttr isStruct() const { return Udm::BooleanAttr(impl, meta_isStruct); }
		Udm::AssocAttr< Signal> signalsOfType() const { return Udm::AssocAttr< Signal>(impl, meta_signalsOfType); }
		template <class Pred> Udm::AssocAttr< Signal, Pred> signalsOfType_sorted(const Pred &) const { return Udm::AssocAttr< Signal, Pred>(impl, meta_signalsOfType); }
		Udm::ParentAttr< ::Semantics::Deployment> Deployment_parent() const { return Udm::ParentAttr< ::Semantics::Deployment>(impl, meta_Deployment_parent); }
		Udm::ParentAttr< ::Semantics::Deployment> parent() const { return Udm::ParentAttr< ::Semantics::Deployment>(impl, Udm::NULLPARENTROLE); }
		void Accept(Visitor &v) { v.Visit_SignalType(*this); }

		static ::Uml::Class meta;
		static ::Uml::Attribute meta_name;
		static ::Uml::Attribute meta_id;
		static ::Uml::Attribute meta_mtypestr;
		static ::Uml::Attribute meta_ctypestr;
		static ::Uml::Attribute meta_rows;
		static ::Uml::Attribute meta_columns;
		static ::Uml::Attribute meta_isStruct;
		static ::Uml::AssociationRole meta_signalsOfType;
		static ::Uml::CompositionParentRole meta_Deployment_parent;

	};

	class Plant : public Udm::Object {
	public:
		Plant() {}
		Plant(Udm::ObjectImpl *impl) : UDM_OBJECT(impl) {}
		Plant(const Plant &master) : UDM_OBJECT(master) {}
		#ifdef UDM_RVALUE
		Plant(Plant &&master) : UDM_OBJECT(master) {}

		static Plant Cast(Udm::Object &&a) { return __Cast(a, meta); }
		Plant& operator=(Plant &&a) { Udm::Object::operator =(a); return *this; }
		#endif
		static Plant Cast(const Udm::Object &a) { return __Cast(a, meta); }
		static Plant Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }
		Plant CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }
		Udm::InstantiatedAttr<Plant> Instances() { return Udm::InstantiatedAttr<Plant>(impl); }
		template <class Pred> Udm::InstantiatedAttr<Plant, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<Plant, Pred>(impl); }
		Plant CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }
		Udm::DerivedAttr<Plant> Derived() { return Udm::DerivedAttr<Plant>(impl); }
		template <class Pred> Udm::DerivedAttr<Plant, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<Plant, Pred>(impl); }
		Udm::ArchetypeAttr<Plant> Archetype() const { return Udm::ArchetypeAttr<Plant>(impl); }
		Udm::StringAttr name() const { return Udm::StringAttr(impl, meta_name); }
		Udm::IntegerAttr id() const { return Udm::IntegerAttr(impl, meta_id); }
		Udm::StringAttr config() const { return Udm::StringAttr(impl, meta_config); }
		Udm::AssocAttr< OutputDevice> actuationDevices() const { return Udm::AssocAttr< OutputDevice>(impl, meta_actuationDevices); }
		template <class Pred> Udm::AssocAttr< OutputDevice, Pred> actuationDevices_sorted(const Pred &) const { return Udm::AssocAttr< OutputDevice, Pred>(impl, meta_actuationDevices); }
		Udm::AssocAttr< InputDevice> acquisitionDevices() const { return Udm::AssocAttr< InputDevice>(impl, meta_acquisitionDevices); }
		template <class Pred> Udm::AssocAttr< InputDevice, Pred> acquisitionDevices_sorted(const Pred &) const { return Udm::AssocAttr< InputDevice, Pred>(impl, meta_acquisitionDevices); }
		Udm::ParentAttr< ::Semantics::Deployment> Deployment_parent() const { return Udm::ParentAttr< ::Semantics::Deployment>(impl, meta_Deployment_parent); }
		Udm::ParentAttr< ::Semantics::Deployment> parent() const { return Udm::ParentAttr< ::Semantics::Deployment>(impl, Udm::NULLPARENTROLE); }
		void Accept(Visitor &v) { v.Visit_Plant(*this); }

		static ::Uml::Class meta;
		static ::Uml::Attribute meta_name;
		static ::Uml::Attribute meta_id;
		static ::Uml::Attribute meta_config;
		static ::Uml::AssociationRole meta_actuationDevices;
		static ::Uml::AssociationRole meta_acquisitionDevices;
		static ::Uml::CompositionParentRole meta_Deployment_parent;

	};

	class Signal : public Udm::Object {
	public:
		Signal() {}
		Signal(Udm::ObjectImpl *impl) : UDM_OBJECT(impl) {}
		Signal(const Signal &master) : UDM_OBJECT(master) {}
		#ifdef UDM_RVALUE
		Signal(Signal &&master) : UDM_OBJECT(master) {}

		static Signal Cast(Udm::Object &&a) { return __Cast(a, meta); }
		Signal& operator=(Signal &&a) { Udm::Object::operator =(a); return *this; }
		#endif
		static Signal Cast(const Udm::Object &a) { return __Cast(a, meta); }
		static Signal Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }
		Signal CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }
		Udm::InstantiatedAttr<Signal> Instances() { return Udm::InstantiatedAttr<Signal>(impl); }
		template <class Pred> Udm::InstantiatedAttr<Signal, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<Signal, Pred>(impl); }
		Signal CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }
		Udm::DerivedAttr<Signal> Derived() { return Udm::DerivedAttr<Signal>(impl); }
		template <class Pred> Udm::DerivedAttr<Signal, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<Signal, Pred>(impl); }
		Udm::ArchetypeAttr<Signal> Archetype() const { return Udm::ArchetypeAttr<Signal>(impl); }
		Udm::IntegerAttr callIndex() const { return Udm::IntegerAttr(impl, meta_callIndex); }
		Udm::IntegerAttr id() const { return Udm::IntegerAttr(impl, meta_id); }
		Udm::IntegerAttr msgIndex() const { return Udm::IntegerAttr(impl, meta_msgIndex); }
		Udm::StringAttr name() const { return Udm::StringAttr(impl, meta_name); }
		Udm::AssocAttr< Source> createdBy() const { return Udm::AssocAttr< Source>(impl, meta_createdBy); }
		template <class Pred> Udm::AssocAttr< Source, Pred> createdBy_sorted(const Pred &) const { return Udm::AssocAttr< Source, Pred>(impl, meta_createdBy); }
		Udm::AssocAttr< Sink> measuredBy() const { return Udm::AssocAttr< Sink>(impl, meta_measuredBy); }
		template <class Pred> Udm::AssocAttr< Sink, Pred> measuredBy_sorted(const Pred &) const { return Udm::AssocAttr< Sink, Pred>(impl, meta_measuredBy); }
		Udm::AssocAttr< Msg> carriedBy() const { return Udm::AssocAttr< Msg>(impl, meta_carriedBy); }
		template <class Pred> Udm::AssocAttr< Msg, Pred> carriedBy_sorted(const Pred &) const { return Udm::AssocAttr< Msg, Pred>(impl, meta_carriedBy); }
		Udm::PointerAttr< SignalType> isOfType() const { return Udm::PointerAttr< SignalType>(impl, meta_isOfType); }
		Udm::AssocAttr< Component> consumedBy() const { return Udm::AssocAttr< Component>(impl, meta_consumedBy); }
		template <class Pred> Udm::AssocAttr< Component, Pred> consumedBy_sorted(const Pred &) const { return Udm::AssocAttr< Component, Pred>(impl, meta_consumedBy); }
		Udm::PointerAttr< Component> generatedBy() const { return Udm::PointerAttr< Component>(impl, meta_generatedBy); }
		Udm::ParentAttr< ::Semantics::Deployment> Deployment_parent() const { return Udm::ParentAttr< ::Semantics::Deployment>(impl, meta_Deployment_parent); }
		Udm::ParentAttr< ::Semantics::Deployment> parent() const { return Udm::ParentAttr< ::Semantics::Deployment>(impl, Udm::NULLPARENTROLE); }
		void Accept(Visitor &v) { v.Visit_Signal(*this); }

		static ::Uml::Class meta;
		static ::Uml::Attribute meta_callIndex;
		static ::Uml::Attribute meta_id;
		static ::Uml::Attribute meta_msgIndex;
		static ::Uml::Attribute meta_name;
		static ::Uml::AssociationRole meta_createdBy;
		static ::Uml::AssociationRole meta_measuredBy;
		static ::Uml::AssociationRole meta_carriedBy;
		static ::Uml::AssociationRole meta_isOfType;
		static ::Uml::AssociationRole meta_consumedBy;
		static ::Uml::AssociationRole meta_generatedBy;
		static ::Uml::CompositionParentRole meta_Deployment_parent;

	};

	class Component : public Udm::Object {
	public:
		Component() {}
		Component(Udm::ObjectImpl *impl) : UDM_OBJECT(impl) {}
		Component(const Component &master) : UDM_OBJECT(master) {}
		#ifdef UDM_RVALUE
		Component(Component &&master) : UDM_OBJECT(master) {}

		static Component Cast(Udm::Object &&a) { return __Cast(a, meta); }
		Component& operator=(Component &&a) { Udm::Object::operator =(a); return *this; }
		#endif
		static Component Cast(const Udm::Object &a) { return __Cast(a, meta); }
		static Component Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }
		Component CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }
		Udm::InstantiatedAttr<Component> Instances() { return Udm::InstantiatedAttr<Component>(impl); }
		template <class Pred> Udm::InstantiatedAttr<Component, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<Component, Pred>(impl); }
		Component CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }
		Udm::DerivedAttr<Component> Derived() { return Udm::DerivedAttr<Component>(impl); }
		template <class Pred> Udm::DerivedAttr<Component, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<Component, Pred>(impl); }
		Udm::ArchetypeAttr<Component> Archetype() const { return Udm::ArchetypeAttr<Component>(impl); }
		Udm::StringAttr name() const { return Udm::StringAttr(impl, meta_name); }
		Udm::IntegerAttr id() const { return Udm::IntegerAttr(impl, meta_id); }
		Udm::StringAttr msubsystem() const { return Udm::StringAttr(impl, meta_msubsystem); }
		Udm::StringAttr ccode() const { return Udm::StringAttr(impl, meta_ccode); }
		Udm::StringAttr hfiles() const { return Udm::StringAttr(impl, meta_hfiles); }
		Udm::StringAttr cfiles() const { return Udm::StringAttr(impl, meta_cfiles); }
		Udm::AssocAttr< Signal> consumes() const { return Udm::AssocAttr< Signal>(impl, meta_consumes); }
		template <class Pred> Udm::AssocAttr< Signal, Pred> consumes_sorted(const Pred &) const { return Udm::AssocAttr< Signal, Pred>(impl, meta_consumes); }
		Udm::PointerAttr< Task> invokedBy() const { return Udm::PointerAttr< Task>(impl, meta_invokedBy); }
		Udm::AssocAttr< Signal> generates() const { return Udm::AssocAttr< Signal>(impl, meta_generates); }
		template <class Pred> Udm::AssocAttr< Signal, Pred> generates_sorted(const Pred &) const { return Udm::AssocAttr< Signal, Pred>(impl, meta_generates); }
		Udm::ParentAttr< ::Semantics::Deployment> Deployment_parent() const { return Udm::ParentAttr< ::Semantics::Deployment>(impl, meta_Deployment_parent); }
		Udm::ParentAttr< ::Semantics::Deployment> parent() const { return Udm::ParentAttr< ::Semantics::Deployment>(impl, Udm::NULLPARENTROLE); }
		void Accept(Visitor &v) { v.Visit_Component(*this); }

		static ::Uml::Class meta;
		static ::Uml::Attribute meta_name;
		static ::Uml::Attribute meta_id;
		static ::Uml::Attribute meta_msubsystem;
		static ::Uml::Attribute meta_ccode;
		static ::Uml::Attribute meta_hfiles;
		static ::Uml::Attribute meta_cfiles;
		static ::Uml::AssociationRole meta_consumes;
		static ::Uml::AssociationRole meta_invokedBy;
		static ::Uml::AssociationRole meta_generates;
		static ::Uml::CompositionParentRole meta_Deployment_parent;

	};

	class Node : public Udm::Object {
	public:
		Node() {}
		Node(Udm::ObjectImpl *impl) : UDM_OBJECT(impl) {}
		Node(const Node &master) : UDM_OBJECT(master) {}
		#ifdef UDM_RVALUE
		Node(Node &&master) : UDM_OBJECT(master) {}

		static Node Cast(Udm::Object &&a) { return __Cast(a, meta); }
		Node& operator=(Node &&a) { Udm::Object::operator =(a); return *this; }
		#endif
		static Node Cast(const Udm::Object &a) { return __Cast(a, meta); }
		static Node Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }
		Node CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }
		Udm::InstantiatedAttr<Node> Instances() { return Udm::InstantiatedAttr<Node>(impl); }
		template <class Pred> Udm::InstantiatedAttr<Node, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<Node, Pred>(impl); }
		Node CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }
		Udm::DerivedAttr<Node> Derived() { return Udm::DerivedAttr<Node>(impl); }
		template <class Pred> Udm::DerivedAttr<Node, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<Node, Pred>(impl); }
		Udm::ArchetypeAttr<Node> Archetype() const { return Udm::ArchetypeAttr<Node>(impl); }
		Udm::StringAttr name() const { return Udm::StringAttr(impl, meta_name); }
		Udm::IntegerAttr id() const { return Udm::IntegerAttr(impl, meta_id); }
		Udm::StringAttr platformtype() const { return Udm::StringAttr(impl, meta_platformtype); }
		Udm::StringAttr processormodel() const { return Udm::StringAttr(impl, meta_processormodel); }
		Udm::RealAttr hyperperiodsecs() const { return Udm::RealAttr(impl, meta_hyperperiodsecs); }
		Udm::IntegerAttr hyperperiodticks() const { return Udm::IntegerAttr(impl, meta_hyperperiodticks); }
		Udm::RealAttr clockspeedhz() const { return Udm::RealAttr(impl, meta_clockspeedhz); }
		Udm::RealAttr tickresolutionsecs() const { return Udm::RealAttr(impl, meta_tickresolutionsecs); }
		Udm::RealAttr ctxtswitchsecs() const { return Udm::RealAttr(impl, meta_ctxtswitchsecs); }
		Udm::RealAttr sndohdsecs() const { return Udm::RealAttr(impl, meta_sndohdsecs); }
		Udm::RealAttr rcvohdsecs() const { return Udm::RealAttr(impl, meta_rcvohdsecs); }
		Udm::IntegerAttr analog_inp_num() const { return Udm::IntegerAttr(impl, meta_analog_inp_num); }
		Udm::IntegerAttr analog_outp_num() const { return Udm::IntegerAttr(impl, meta_analog_outp_num); }
		Udm::StringAttr pp_directives() const { return Udm::StringAttr(impl, meta_pp_directives); }
		Udm::AssocAttr< DummyDependency> nodeDummyDeps() const { return Udm::AssocAttr< DummyDependency>(impl, meta_nodeDummyDeps); }
		template <class Pred> Udm::AssocAttr< DummyDependency, Pred> nodeDummyDeps_sorted(const Pred &) const { return Udm::AssocAttr< DummyDependency, Pred>(impl, meta_nodeDummyDeps); }
		Udm::AssocAttr< LocalDependency> nodeLocalDeps() const { return Udm::AssocAttr< LocalDependency>(impl, meta_nodeLocalDeps); }
		template <class Pred> Udm::AssocAttr< LocalDependency, Pred> nodeLocalDeps_sorted(const Pred &) const { return Udm::AssocAttr< LocalDependency, Pred>(impl, meta_nodeLocalDeps); }
		Udm::PointerAttr< Msg> nodeDummyMsg() const { return Udm::PointerAttr< Msg>(impl, meta_nodeDummyMsg); }
		Udm::AssocAttr< Task> executes() const { return Udm::AssocAttr< Task>(impl, meta_executes); }
		template <class Pred> Udm::AssocAttr< Task, Pred> executes_sorted(const Pred &) const { return Udm::AssocAttr< Task, Pred>(impl, meta_executes); }
		Udm::AssocAttr< Device> integrates() const { return Udm::AssocAttr< Device>(impl, meta_integrates); }
		template <class Pred> Udm::AssocAttr< Device, Pred> integrates_sorted(const Pred &) const { return Udm::AssocAttr< Device, Pred>(impl, meta_integrates); }
		Udm::ParentAttr< ::Semantics::Deployment> Deployment_parent() const { return Udm::ParentAttr< ::Semantics::Deployment>(impl, meta_Deployment_parent); }
		Udm::ParentAttr< ::Semantics::Deployment> parent() const { return Udm::ParentAttr< ::Semantics::Deployment>(impl, Udm::NULLPARENTROLE); }
		void Accept(Visitor &v) { v.Visit_Node(*this); }

		static ::Uml::Class meta;
		static ::Uml::Attribute meta_name;
		static ::Uml::Attribute meta_id;
		static ::Uml::Attribute meta_platformtype;
		static ::Uml::Attribute meta_processormodel;
		static ::Uml::Attribute meta_hyperperiodsecs;
		static ::Uml::Attribute meta_hyperperiodticks;
		static ::Uml::Attribute meta_clockspeedhz;
		static ::Uml::Attribute meta_tickresolutionsecs;
		static ::Uml::Attribute meta_ctxtswitchsecs;
		static ::Uml::Attribute meta_sndohdsecs;
		static ::Uml::Attribute meta_rcvohdsecs;
		static ::Uml::Attribute meta_analog_inp_num;
		static ::Uml::Attribute meta_analog_outp_num;
		static ::Uml::Attribute meta_pp_directives;
		static ::Uml::AssociationRole meta_nodeDummyDeps;
		static ::Uml::AssociationRole meta_nodeLocalDeps;
		static ::Uml::AssociationRole meta_nodeDummyMsg;
		static ::Uml::AssociationRole meta_executes;
		static ::Uml::AssociationRole meta_integrates;
		static ::Uml::CompositionParentRole meta_Deployment_parent;

	};

	class TimingConstraint : public Udm::Object {
	public:
		TimingConstraint() {}
		TimingConstraint(Udm::ObjectImpl *impl) : UDM_OBJECT(impl) {}
		TimingConstraint(const TimingConstraint &master) : UDM_OBJECT(master) {}
		#ifdef UDM_RVALUE
		TimingConstraint(TimingConstraint &&master) : UDM_OBJECT(master) {}

		static TimingConstraint Cast(Udm::Object &&a) { return __Cast(a, meta); }
		TimingConstraint& operator=(TimingConstraint &&a) { Udm::Object::operator =(a); return *this; }
		#endif
		static TimingConstraint Cast(const Udm::Object &a) { return __Cast(a, meta); }
		static TimingConstraint Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }
		TimingConstraint CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }
		Udm::InstantiatedAttr<TimingConstraint> Instances() { return Udm::InstantiatedAttr<TimingConstraint>(impl); }
		template <class Pred> Udm::InstantiatedAttr<TimingConstraint, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr<TimingConstraint, Pred>(impl); }
		TimingConstraint CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }
		Udm::DerivedAttr<TimingConstraint> Derived() { return Udm::DerivedAttr<TimingConstraint>(impl); }
		template <class Pred> Udm::DerivedAttr<TimingConstraint, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr<TimingConstraint, Pred>(impl); }
		Udm::ArchetypeAttr<TimingConstraint> Archetype() const { return Udm::ArchetypeAttr<TimingConstraint>(impl); }
		Udm::RealAttr timeboundsecs() const { return Udm::RealAttr(impl, meta_timeboundsecs); }
		Udm::StringAttr id() const { return Udm::StringAttr(impl, meta_id); }
		Udm::PointerAttr< Task> sndTask() const { return Udm::PointerAttr< Task>(impl, meta_sndTask); }
		Udm::PointerAttr< Task> rcvTask() const { return Udm::PointerAttr< Task>(impl, meta_rcvTask); }
		Udm::ParentAttr< ::Semantics::Deployment> Deployment_parent() const { return Udm::ParentAttr< ::Semantics::Deployment>(impl, meta_Deployment_parent); }
		Udm::ParentAttr< ::Semantics::Deployment> parent() const { return Udm::ParentAttr< ::Semantics::Deployment>(impl, Udm::NULLPARENTROLE); }
		void Accept(Visitor &v) { v.Visit_TimingConstraint(*this); }

		static ::Uml::Class meta;
		static ::Uml::Attribute meta_timeboundsecs;
		static ::Uml::Attribute meta_id;
		static ::Uml::AssociationRole meta_sndTask;
		static ::Uml::AssociationRole meta_rcvTask;
		static ::Uml::CompositionParentRole meta_Deployment_parent;

	};

}

#endif // MOBIES_SEMANTICS_H
