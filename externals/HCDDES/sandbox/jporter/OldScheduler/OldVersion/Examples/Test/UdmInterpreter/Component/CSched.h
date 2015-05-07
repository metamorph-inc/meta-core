#ifndef MOBIES_CSCHED_H
#define MOBIES_CSCHED_H
// header file CSched.h generated from diagram CSched
// generated on Tue Nov 27 16:25:59 2007

#ifndef MOBIES_UDMBASE_H
#include "UdmBase.h"
#endif

#ifdef min
#undef min
#endif

#ifdef max
#undef max
#endif
namespace CSched {
	extern  Udm::UdmDiagram diagram;
	 void Initialize(const ::Uml::Diagram &dgr);
	 void Initialize();

		class  TTSystem;
		class  MgaObject;
		class  Processor;
		class  TaskRef;
		class  Bus;
		class  Message;
		class  Sender;
		class  Receiver;
		class  Link;
		class  Task;
		class  LatencySpec;
		class  MsgRef;
		class  Dependency;
		class  Latency;
		class  StartMsg;
		class  EndMsg;
		class  TimeSync;
		class  RootFolder;
		class  InstanceModel;
		class  MsgInstances;
		class  TaskInstances;
		class  MsgInstance;
		class  TaskInstance;
		class  ReceiverInst;
		class  SenderInst;
		class  Dependencies;
		class  MsgInstRef;
		class  TaskInstRef;
		class  EqualityConstraint;
		class  InequalityConstraint;
		class  Assignment;
		class  Member;
		class  RangeConstraint;
		class  InstanceRef;
		class  Solution;
		class  Connectable;
		class  YPort;
		class  XPort;
		class  PairConstraint;
		class  FDVar;
		class  ConstraintProblem;
		class  Constraint;
		class  SerialConstraint;
		class  SearchConfig;


		 void CreateMetaObjs();
		 void InitCrossNSInheritence();
		 void InitCrossNSCompositions();
		 void InitCrossNSAssociations();
		class  MgaObject :  public Udm::Object {
		public:
			static ::Uml::Class meta;

			MgaObject() { }
			MgaObject(Udm::ObjectImpl *impl) : UDM_OBJECT(impl) { }
			MgaObject(const MgaObject &master) : UDM_OBJECT(master) { }
			static MgaObject Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static MgaObject Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			MgaObject CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			MgaObject CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::CSched::MgaObject> Instances() { return Udm::InstantiatedAttr< ::CSched::MgaObject>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::CSched::MgaObject, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::CSched::MgaObject, Pred>(impl);}

			Udm::DerivedAttr< ::CSched::MgaObject> Derived() { return Udm::DerivedAttr< ::CSched::MgaObject>(impl);}
			template <class Pred> Udm::DerivedAttr< ::CSched::MgaObject, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::CSched::MgaObject, Pred>(impl);}

			Udm::ArchetypeAttr< ::CSched::MgaObject> Archetype() { return Udm::ArchetypeAttr< ::CSched::MgaObject>(impl);}

			static ::Uml::Attribute meta_position;
			Udm::StringAttr position() const { return Udm::StringAttr(impl, meta_position); }

			static ::Uml::Attribute meta_name;
			Udm::StringAttr name() const { return Udm::StringAttr(impl, meta_name); }

			Udm::ParentAttr<Udm::Object> parent() const { return Udm::ParentAttr<Udm::Object>(impl, Udm::NULLPARENTROLE); }
		};

		class  TTSystem :  public ::CSched::MgaObject {
		public:
			static ::Uml::Class meta;

			TTSystem() { }
			TTSystem(Udm::ObjectImpl *impl) : ::CSched::MgaObject(impl) { }
			TTSystem(const TTSystem &master) : ::CSched::MgaObject(master) { }
			static TTSystem Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static TTSystem Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			TTSystem CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			TTSystem CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::CSched::TTSystem> Instances() { return Udm::InstantiatedAttr< ::CSched::TTSystem>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::CSched::TTSystem, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::CSched::TTSystem, Pred>(impl);}

			Udm::DerivedAttr< ::CSched::TTSystem> Derived() { return Udm::DerivedAttr< ::CSched::TTSystem>(impl);}
			template <class Pred> Udm::DerivedAttr< ::CSched::TTSystem, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::CSched::TTSystem, Pred>(impl);}

			Udm::ArchetypeAttr< ::CSched::TTSystem> Archetype() { return Udm::ArchetypeAttr< ::CSched::TTSystem>(impl);}

			static ::Uml::CompositionChildRole meta_Processor_children;
			Udm::ChildrenAttr< ::CSched::Processor> Processor_children() const { return Udm::ChildrenAttr< ::CSched::Processor>(impl, meta_Processor_children); }
			template <class Pred> Udm::ChildrenAttr< ::CSched::Processor, Pred> Processor_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::CSched::Processor, Pred>(impl, meta_Processor_children); }

			static ::Uml::CompositionChildRole meta_LatencySpec_children;
			Udm::ChildrenAttr< ::CSched::LatencySpec> LatencySpec_children() const { return Udm::ChildrenAttr< ::CSched::LatencySpec>(impl, meta_LatencySpec_children); }
			template <class Pred> Udm::ChildrenAttr< ::CSched::LatencySpec, Pred> LatencySpec_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::CSched::LatencySpec, Pred>(impl, meta_LatencySpec_children); }

			static ::Uml::CompositionChildRole meta_Bus_children;
			Udm::ChildrenAttr< ::CSched::Bus> Bus_children() const { return Udm::ChildrenAttr< ::CSched::Bus>(impl, meta_Bus_children); }
			template <class Pred> Udm::ChildrenAttr< ::CSched::Bus, Pred> Bus_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::CSched::Bus, Pred>(impl, meta_Bus_children); }

			static ::Uml::CompositionChildRole meta_Link_children;
			Udm::ChildrenAttr< ::CSched::Link> Link_children() const { return Udm::ChildrenAttr< ::CSched::Link>(impl, meta_Link_children); }
			template <class Pred> Udm::ChildrenAttr< ::CSched::Link, Pred> Link_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::CSched::Link, Pred>(impl, meta_Link_children); }

			Udm::ChildrenAttr< ::CSched::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr< ::CSched::MgaObject>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::CSched::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::CSched::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::CSched::Processor> Processor_kind_children() const { return Udm::ChildrenAttr< ::CSched::Processor>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::CSched::Processor, Pred> Processor_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::CSched::Processor, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::CSched::Bus> Bus_kind_children() const { return Udm::ChildrenAttr< ::CSched::Bus>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::CSched::Bus, Pred> Bus_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::CSched::Bus, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::CSched::Link> Link_kind_children() const { return Udm::ChildrenAttr< ::CSched::Link>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::CSched::Link, Pred> Link_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::CSched::Link, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::CSched::LatencySpec> LatencySpec_kind_children() const { return Udm::ChildrenAttr< ::CSched::LatencySpec>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::CSched::LatencySpec, Pred> LatencySpec_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::CSched::LatencySpec, Pred>(impl, Udm::NULLCHILDROLE); }

			static ::Uml::CompositionParentRole meta_RootFolder_parent;
			Udm::ParentAttr< ::CSched::RootFolder> RootFolder_parent() const { return Udm::ParentAttr< ::CSched::RootFolder>(impl, meta_RootFolder_parent); }

			Udm::ParentAttr< ::CSched::RootFolder> parent() const { return Udm::ParentAttr< ::CSched::RootFolder>(impl, Udm::NULLPARENTROLE); }
		};

		class  Processor :  public ::CSched::MgaObject {
		public:
			static ::Uml::Class meta;

			Processor() { }
			Processor(Udm::ObjectImpl *impl) : ::CSched::MgaObject(impl) { }
			Processor(const Processor &master) : ::CSched::MgaObject(master) { }
			static Processor Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static Processor Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			Processor CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			Processor CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::CSched::Processor> Instances() { return Udm::InstantiatedAttr< ::CSched::Processor>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::CSched::Processor, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::CSched::Processor, Pred>(impl);}

			Udm::DerivedAttr< ::CSched::Processor> Derived() { return Udm::DerivedAttr< ::CSched::Processor>(impl);}
			template <class Pred> Udm::DerivedAttr< ::CSched::Processor, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::CSched::Processor, Pred>(impl);}

			Udm::ArchetypeAttr< ::CSched::Processor> Archetype() { return Udm::ArchetypeAttr< ::CSched::Processor>(impl);}

			static ::Uml::AssociationRole meta_dstLink;
			static ::Uml::AssociationRole meta_dstLink_rev;
			Udm::AClassAssocAttr< ::CSched::Link, ::CSched::Bus> dstLink() const { return Udm::AClassAssocAttr< ::CSched::Link, ::CSched::Bus>(impl, meta_dstLink, meta_dstLink_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::CSched::Link, ::CSched::Bus, Pred> dstLink_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::CSched::Link, ::CSched::Bus, Pred>(impl, meta_dstLink, meta_dstLink_rev); }

			static ::Uml::CompositionChildRole meta_TaskInstances_child;
			Udm::ChildAttr< ::CSched::TaskInstances> TaskInstances_child() const { return Udm::ChildAttr< ::CSched::TaskInstances>(impl, meta_TaskInstances_child); }

			static ::Uml::CompositionChildRole meta_Task_children;
			Udm::ChildrenAttr< ::CSched::Task> Task_children() const { return Udm::ChildrenAttr< ::CSched::Task>(impl, meta_Task_children); }
			template <class Pred> Udm::ChildrenAttr< ::CSched::Task, Pred> Task_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::CSched::Task, Pred>(impl, meta_Task_children); }

			Udm::ChildrenAttr< ::CSched::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr< ::CSched::MgaObject>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::CSched::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::CSched::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::CSched::Task> Task_kind_children() const { return Udm::ChildrenAttr< ::CSched::Task>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::CSched::Task, Pred> Task_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::CSched::Task, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::CSched::TaskInstances> TaskInstances_kind_children() const { return Udm::ChildrenAttr< ::CSched::TaskInstances>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::CSched::TaskInstances, Pred> TaskInstances_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::CSched::TaskInstances, Pred>(impl, Udm::NULLCHILDROLE); }

			static ::Uml::CompositionParentRole meta_TTSystem_parent;
			Udm::ParentAttr< ::CSched::TTSystem> TTSystem_parent() const { return Udm::ParentAttr< ::CSched::TTSystem>(impl, meta_TTSystem_parent); }

			static ::Uml::CompositionParentRole meta_InstanceModel_parent;
			Udm::ParentAttr< ::CSched::InstanceModel> InstanceModel_parent() const { return Udm::ParentAttr< ::CSched::InstanceModel>(impl, meta_InstanceModel_parent); }

			Udm::ParentAttr< ::CSched::MgaObject> parent() const { return Udm::ParentAttr< ::CSched::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  TaskRef :  public ::CSched::MgaObject {
		public:
			static ::Uml::Class meta;

			TaskRef() { }
			TaskRef(Udm::ObjectImpl *impl) : ::CSched::MgaObject(impl) { }
			TaskRef(const TaskRef &master) : ::CSched::MgaObject(master) { }
			static TaskRef Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static TaskRef Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			TaskRef CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			TaskRef CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::CSched::TaskRef> Instances() { return Udm::InstantiatedAttr< ::CSched::TaskRef>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::CSched::TaskRef, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::CSched::TaskRef, Pred>(impl);}

			Udm::DerivedAttr< ::CSched::TaskRef> Derived() { return Udm::DerivedAttr< ::CSched::TaskRef>(impl);}
			template <class Pred> Udm::DerivedAttr< ::CSched::TaskRef, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::CSched::TaskRef, Pred>(impl);}

			Udm::ArchetypeAttr< ::CSched::TaskRef> Archetype() { return Udm::ArchetypeAttr< ::CSched::TaskRef>(impl);}

			static ::Uml::AssociationRole meta_ref;
			Udm::PointerAttr< ::CSched::Task> ref() const { return Udm::PointerAttr< ::CSched::Task>(impl, meta_ref); }

			static ::Uml::AssociationRole meta_dstSender;
			static ::Uml::AssociationRole meta_dstSender_rev;
			Udm::AClassAssocAttr< ::CSched::Sender, ::CSched::Message> dstSender() const { return Udm::AClassAssocAttr< ::CSched::Sender, ::CSched::Message>(impl, meta_dstSender, meta_dstSender_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::CSched::Sender, ::CSched::Message, Pred> dstSender_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::CSched::Sender, ::CSched::Message, Pred>(impl, meta_dstSender, meta_dstSender_rev); }

			static ::Uml::AssociationRole meta_srcReceiver;
			static ::Uml::AssociationRole meta_srcReceiver_rev;
			Udm::AClassAssocAttr< ::CSched::Receiver, ::CSched::Message> srcReceiver() const { return Udm::AClassAssocAttr< ::CSched::Receiver, ::CSched::Message>(impl, meta_srcReceiver, meta_srcReceiver_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::CSched::Receiver, ::CSched::Message, Pred> srcReceiver_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::CSched::Receiver, ::CSched::Message, Pred>(impl, meta_srcReceiver, meta_srcReceiver_rev); }

			static ::Uml::CompositionParentRole meta_TaskInstances_parent;
			Udm::ParentAttr< ::CSched::TaskInstances> TaskInstances_parent() const { return Udm::ParentAttr< ::CSched::TaskInstances>(impl, meta_TaskInstances_parent); }

			static ::Uml::CompositionParentRole meta_Bus_parent;
			Udm::ParentAttr< ::CSched::Bus> Bus_parent() const { return Udm::ParentAttr< ::CSched::Bus>(impl, meta_Bus_parent); }

			Udm::ParentAttr< ::CSched::MgaObject> parent() const { return Udm::ParentAttr< ::CSched::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  Bus :  public ::CSched::MgaObject {
		public:
			static ::Uml::Class meta;

			Bus() { }
			Bus(Udm::ObjectImpl *impl) : ::CSched::MgaObject(impl) { }
			Bus(const Bus &master) : ::CSched::MgaObject(master) { }
			static Bus Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static Bus Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			Bus CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			Bus CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::CSched::Bus> Instances() { return Udm::InstantiatedAttr< ::CSched::Bus>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::CSched::Bus, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::CSched::Bus, Pred>(impl);}

			Udm::DerivedAttr< ::CSched::Bus> Derived() { return Udm::DerivedAttr< ::CSched::Bus>(impl);}
			template <class Pred> Udm::DerivedAttr< ::CSched::Bus, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::CSched::Bus, Pred>(impl);}

			Udm::ArchetypeAttr< ::CSched::Bus> Archetype() { return Udm::ArchetypeAttr< ::CSched::Bus>(impl);}

			static ::Uml::Attribute meta_Resolution;
			Udm::IntegerAttr Resolution() const { return Udm::IntegerAttr(impl, meta_Resolution); }

			static ::Uml::AssociationRole meta_srcLink;
			static ::Uml::AssociationRole meta_srcLink_rev;
			Udm::AClassAssocAttr< ::CSched::Link, ::CSched::Processor> srcLink() const { return Udm::AClassAssocAttr< ::CSched::Link, ::CSched::Processor>(impl, meta_srcLink, meta_srcLink_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::CSched::Link, ::CSched::Processor, Pred> srcLink_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::CSched::Link, ::CSched::Processor, Pred>(impl, meta_srcLink, meta_srcLink_rev); }

			static ::Uml::CompositionChildRole meta_Sender_children;
			Udm::ChildrenAttr< ::CSched::Sender> Sender_children() const { return Udm::ChildrenAttr< ::CSched::Sender>(impl, meta_Sender_children); }
			template <class Pred> Udm::ChildrenAttr< ::CSched::Sender, Pred> Sender_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::CSched::Sender, Pred>(impl, meta_Sender_children); }

			static ::Uml::CompositionChildRole meta_TimeSync_child;
			Udm::ChildAttr< ::CSched::TimeSync> TimeSync_child() const { return Udm::ChildAttr< ::CSched::TimeSync>(impl, meta_TimeSync_child); }

			static ::Uml::CompositionChildRole meta_Receiver_children;
			Udm::ChildrenAttr< ::CSched::Receiver> Receiver_children() const { return Udm::ChildrenAttr< ::CSched::Receiver>(impl, meta_Receiver_children); }
			template <class Pred> Udm::ChildrenAttr< ::CSched::Receiver, Pred> Receiver_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::CSched::Receiver, Pred>(impl, meta_Receiver_children); }

			static ::Uml::CompositionChildRole meta_TaskRef_children;
			Udm::ChildrenAttr< ::CSched::TaskRef> TaskRef_children() const { return Udm::ChildrenAttr< ::CSched::TaskRef>(impl, meta_TaskRef_children); }
			template <class Pred> Udm::ChildrenAttr< ::CSched::TaskRef, Pred> TaskRef_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::CSched::TaskRef, Pred>(impl, meta_TaskRef_children); }

			static ::Uml::CompositionChildRole meta_Message_children;
			Udm::ChildrenAttr< ::CSched::Message> Message_children() const { return Udm::ChildrenAttr< ::CSched::Message>(impl, meta_Message_children); }
			template <class Pred> Udm::ChildrenAttr< ::CSched::Message, Pred> Message_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::CSched::Message, Pred>(impl, meta_Message_children); }

			static ::Uml::CompositionChildRole meta_MsgInstances_child;
			Udm::ChildAttr< ::CSched::MsgInstances> MsgInstances_child() const { return Udm::ChildAttr< ::CSched::MsgInstances>(impl, meta_MsgInstances_child); }

			static ::Uml::CompositionChildRole meta_Dependencies_child;
			Udm::ChildAttr< ::CSched::Dependencies> Dependencies_child() const { return Udm::ChildAttr< ::CSched::Dependencies>(impl, meta_Dependencies_child); }

			Udm::ChildrenAttr< ::CSched::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr< ::CSched::MgaObject>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::CSched::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::CSched::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::CSched::TaskRef> TaskRef_kind_children() const { return Udm::ChildrenAttr< ::CSched::TaskRef>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::CSched::TaskRef, Pred> TaskRef_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::CSched::TaskRef, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::CSched::Message> Message_kind_children() const { return Udm::ChildrenAttr< ::CSched::Message>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::CSched::Message, Pred> Message_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::CSched::Message, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::CSched::Sender> Sender_kind_children() const { return Udm::ChildrenAttr< ::CSched::Sender>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::CSched::Sender, Pred> Sender_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::CSched::Sender, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::CSched::Receiver> Receiver_kind_children() const { return Udm::ChildrenAttr< ::CSched::Receiver>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::CSched::Receiver, Pred> Receiver_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::CSched::Receiver, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::CSched::TimeSync> TimeSync_kind_children() const { return Udm::ChildrenAttr< ::CSched::TimeSync>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::CSched::TimeSync, Pred> TimeSync_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::CSched::TimeSync, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::CSched::MsgInstances> MsgInstances_kind_children() const { return Udm::ChildrenAttr< ::CSched::MsgInstances>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::CSched::MsgInstances, Pred> MsgInstances_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::CSched::MsgInstances, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::CSched::Dependencies> Dependencies_kind_children() const { return Udm::ChildrenAttr< ::CSched::Dependencies>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::CSched::Dependencies, Pred> Dependencies_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::CSched::Dependencies, Pred>(impl, Udm::NULLCHILDROLE); }

			static ::Uml::CompositionParentRole meta_TTSystem_parent;
			Udm::ParentAttr< ::CSched::TTSystem> TTSystem_parent() const { return Udm::ParentAttr< ::CSched::TTSystem>(impl, meta_TTSystem_parent); }

			static ::Uml::CompositionParentRole meta_InstanceModel_parent;
			Udm::ParentAttr< ::CSched::InstanceModel> InstanceModel_parent() const { return Udm::ParentAttr< ::CSched::InstanceModel>(impl, meta_InstanceModel_parent); }

			Udm::ParentAttr< ::CSched::MgaObject> parent() const { return Udm::ParentAttr< ::CSched::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  Message :  public ::CSched::MgaObject {
		public:
			static ::Uml::Class meta;

			Message() { }
			Message(Udm::ObjectImpl *impl) : ::CSched::MgaObject(impl) { }
			Message(const Message &master) : ::CSched::MgaObject(master) { }
			static Message Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static Message Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			Message CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			Message CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::CSched::Message> Instances() { return Udm::InstantiatedAttr< ::CSched::Message>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::CSched::Message, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::CSched::Message, Pred>(impl);}

			Udm::DerivedAttr< ::CSched::Message> Derived() { return Udm::DerivedAttr< ::CSched::Message>(impl);}
			template <class Pred> Udm::DerivedAttr< ::CSched::Message, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::CSched::Message, Pred>(impl);}

			Udm::ArchetypeAttr< ::CSched::Message> Archetype() { return Udm::ArchetypeAttr< ::CSched::Message>(impl);}

			static ::Uml::Attribute meta_MsgLength;
			Udm::IntegerAttr MsgLength() const { return Udm::IntegerAttr(impl, meta_MsgLength); }

			static ::Uml::AssociationRole meta_srcSender;
			static ::Uml::AssociationRole meta_srcSender_rev;
			Udm::AClassPointerAttr< ::CSched::Sender, ::CSched::TaskRef> srcSender() const { return Udm::AClassPointerAttr< ::CSched::Sender, ::CSched::TaskRef>(impl, meta_srcSender, meta_srcSender_rev); }

			static ::Uml::AssociationRole meta_dstReceiver;
			static ::Uml::AssociationRole meta_dstReceiver_rev;
			Udm::AClassAssocAttr< ::CSched::Receiver, ::CSched::TaskRef> dstReceiver() const { return Udm::AClassAssocAttr< ::CSched::Receiver, ::CSched::TaskRef>(impl, meta_dstReceiver, meta_dstReceiver_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::CSched::Receiver, ::CSched::TaskRef, Pred> dstReceiver_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::CSched::Receiver, ::CSched::TaskRef, Pred>(impl, meta_dstReceiver, meta_dstReceiver_rev); }

			static ::Uml::AssociationRole meta_referedbyMsgRef;
			Udm::AssocAttr< ::CSched::MsgRef> referedbyMsgRef() const { return Udm::AssocAttr< ::CSched::MsgRef>(impl, meta_referedbyMsgRef); }
			template <class Pred> Udm::AssocAttr< ::CSched::MsgRef, Pred > referedbyMsgRef_sorted(const Pred &) const { return Udm::AssocAttr< ::CSched::MsgRef, Pred>(impl, meta_referedbyMsgRef); }

			static ::Uml::CompositionParentRole meta_Bus_parent;
			Udm::ParentAttr< ::CSched::Bus> Bus_parent() const { return Udm::ParentAttr< ::CSched::Bus>(impl, meta_Bus_parent); }

			Udm::ParentAttr< ::CSched::MgaObject> parent() const { return Udm::ParentAttr< ::CSched::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  Sender :  public ::CSched::MgaObject {
		public:
			static ::Uml::Class meta;

			Sender() { }
			Sender(Udm::ObjectImpl *impl) : ::CSched::MgaObject(impl) { }
			Sender(const Sender &master) : ::CSched::MgaObject(master) { }
			static Sender Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static Sender Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			Sender CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			Sender CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::CSched::Sender> Instances() { return Udm::InstantiatedAttr< ::CSched::Sender>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::CSched::Sender, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::CSched::Sender, Pred>(impl);}

			Udm::DerivedAttr< ::CSched::Sender> Derived() { return Udm::DerivedAttr< ::CSched::Sender>(impl);}
			template <class Pred> Udm::DerivedAttr< ::CSched::Sender, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::CSched::Sender, Pred>(impl);}

			Udm::ArchetypeAttr< ::CSched::Sender> Archetype() { return Udm::ArchetypeAttr< ::CSched::Sender>(impl);}

			static ::Uml::CompositionParentRole meta_Bus_parent;
			Udm::ParentAttr< ::CSched::Bus> Bus_parent() const { return Udm::ParentAttr< ::CSched::Bus>(impl, meta_Bus_parent); }

			Udm::ParentAttr< ::CSched::MgaObject> parent() const { return Udm::ParentAttr< ::CSched::MgaObject>(impl, Udm::NULLPARENTROLE); }
			static ::Uml::AssociationRole meta_srcSender_end_;
			Udm::AssocEndAttr< ::CSched::TaskRef> srcSender_end() const { return Udm::AssocEndAttr< ::CSched::TaskRef>(impl, meta_srcSender_end_); }

			static ::Uml::AssociationRole meta_dstSender_end_;
			Udm::AssocEndAttr< ::CSched::Message> dstSender_end() const { return Udm::AssocEndAttr< ::CSched::Message>(impl, meta_dstSender_end_); }

		};

		class  Receiver :  public ::CSched::MgaObject {
		public:
			static ::Uml::Class meta;

			Receiver() { }
			Receiver(Udm::ObjectImpl *impl) : ::CSched::MgaObject(impl) { }
			Receiver(const Receiver &master) : ::CSched::MgaObject(master) { }
			static Receiver Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static Receiver Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			Receiver CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			Receiver CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::CSched::Receiver> Instances() { return Udm::InstantiatedAttr< ::CSched::Receiver>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::CSched::Receiver, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::CSched::Receiver, Pred>(impl);}

			Udm::DerivedAttr< ::CSched::Receiver> Derived() { return Udm::DerivedAttr< ::CSched::Receiver>(impl);}
			template <class Pred> Udm::DerivedAttr< ::CSched::Receiver, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::CSched::Receiver, Pred>(impl);}

			Udm::ArchetypeAttr< ::CSched::Receiver> Archetype() { return Udm::ArchetypeAttr< ::CSched::Receiver>(impl);}

			static ::Uml::CompositionParentRole meta_Bus_parent;
			Udm::ParentAttr< ::CSched::Bus> Bus_parent() const { return Udm::ParentAttr< ::CSched::Bus>(impl, meta_Bus_parent); }

			Udm::ParentAttr< ::CSched::MgaObject> parent() const { return Udm::ParentAttr< ::CSched::MgaObject>(impl, Udm::NULLPARENTROLE); }
			static ::Uml::AssociationRole meta_dstReceiver_end_;
			Udm::AssocEndAttr< ::CSched::TaskRef> dstReceiver_end() const { return Udm::AssocEndAttr< ::CSched::TaskRef>(impl, meta_dstReceiver_end_); }

			static ::Uml::AssociationRole meta_srcReceiver_end_;
			Udm::AssocEndAttr< ::CSched::Message> srcReceiver_end() const { return Udm::AssocEndAttr< ::CSched::Message>(impl, meta_srcReceiver_end_); }

		};

		class  Link :  public ::CSched::MgaObject {
		public:
			static ::Uml::Class meta;

			Link() { }
			Link(Udm::ObjectImpl *impl) : ::CSched::MgaObject(impl) { }
			Link(const Link &master) : ::CSched::MgaObject(master) { }
			static Link Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static Link Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			Link CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			Link CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::CSched::Link> Instances() { return Udm::InstantiatedAttr< ::CSched::Link>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::CSched::Link, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::CSched::Link, Pred>(impl);}

			Udm::DerivedAttr< ::CSched::Link> Derived() { return Udm::DerivedAttr< ::CSched::Link>(impl);}
			template <class Pred> Udm::DerivedAttr< ::CSched::Link, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::CSched::Link, Pred>(impl);}

			Udm::ArchetypeAttr< ::CSched::Link> Archetype() { return Udm::ArchetypeAttr< ::CSched::Link>(impl);}

			static ::Uml::CompositionParentRole meta_TTSystem_parent;
			Udm::ParentAttr< ::CSched::TTSystem> TTSystem_parent() const { return Udm::ParentAttr< ::CSched::TTSystem>(impl, meta_TTSystem_parent); }

			Udm::ParentAttr< ::CSched::MgaObject> parent() const { return Udm::ParentAttr< ::CSched::MgaObject>(impl, Udm::NULLPARENTROLE); }
			static ::Uml::AssociationRole meta_dstLink_end_;
			Udm::AssocEndAttr< ::CSched::Bus> dstLink_end() const { return Udm::AssocEndAttr< ::CSched::Bus>(impl, meta_dstLink_end_); }

			static ::Uml::AssociationRole meta_srcLink_end_;
			Udm::AssocEndAttr< ::CSched::Processor> srcLink_end() const { return Udm::AssocEndAttr< ::CSched::Processor>(impl, meta_srcLink_end_); }

		};

		class  Task :  public ::CSched::MgaObject {
		public:
			static ::Uml::Class meta;

			Task() { }
			Task(Udm::ObjectImpl *impl) : ::CSched::MgaObject(impl) { }
			Task(const Task &master) : ::CSched::MgaObject(master) { }
			static Task Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static Task Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			Task CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			Task CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::CSched::Task> Instances() { return Udm::InstantiatedAttr< ::CSched::Task>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::CSched::Task, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::CSched::Task, Pred>(impl);}

			Udm::DerivedAttr< ::CSched::Task> Derived() { return Udm::DerivedAttr< ::CSched::Task>(impl);}
			template <class Pred> Udm::DerivedAttr< ::CSched::Task, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::CSched::Task, Pred>(impl);}

			Udm::ArchetypeAttr< ::CSched::Task> Archetype() { return Udm::ArchetypeAttr< ::CSched::Task>(impl);}

			static ::Uml::Attribute meta_WCExecTime;
			Udm::IntegerAttr WCExecTime() const { return Udm::IntegerAttr(impl, meta_WCExecTime); }

			static ::Uml::Attribute meta_Period;
			Udm::IntegerAttr Period() const { return Udm::IntegerAttr(impl, meta_Period); }

			static ::Uml::AssociationRole meta_referedbyTaskRef;
			Udm::AssocAttr< ::CSched::TaskRef> referedbyTaskRef() const { return Udm::AssocAttr< ::CSched::TaskRef>(impl, meta_referedbyTaskRef); }
			template <class Pred> Udm::AssocAttr< ::CSched::TaskRef, Pred > referedbyTaskRef_sorted(const Pred &) const { return Udm::AssocAttr< ::CSched::TaskRef, Pred>(impl, meta_referedbyTaskRef); }

			static ::Uml::CompositionParentRole meta_Processor_parent;
			Udm::ParentAttr< ::CSched::Processor> Processor_parent() const { return Udm::ParentAttr< ::CSched::Processor>(impl, meta_Processor_parent); }

			Udm::ParentAttr< ::CSched::MgaObject> parent() const { return Udm::ParentAttr< ::CSched::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  LatencySpec :  public ::CSched::MgaObject {
		public:
			static ::Uml::Class meta;

			LatencySpec() { }
			LatencySpec(Udm::ObjectImpl *impl) : ::CSched::MgaObject(impl) { }
			LatencySpec(const LatencySpec &master) : ::CSched::MgaObject(master) { }
			static LatencySpec Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static LatencySpec Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			LatencySpec CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			LatencySpec CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::CSched::LatencySpec> Instances() { return Udm::InstantiatedAttr< ::CSched::LatencySpec>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::CSched::LatencySpec, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::CSched::LatencySpec, Pred>(impl);}

			Udm::DerivedAttr< ::CSched::LatencySpec> Derived() { return Udm::DerivedAttr< ::CSched::LatencySpec>(impl);}
			template <class Pred> Udm::DerivedAttr< ::CSched::LatencySpec, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::CSched::LatencySpec, Pred>(impl);}

			Udm::ArchetypeAttr< ::CSched::LatencySpec> Archetype() { return Udm::ArchetypeAttr< ::CSched::LatencySpec>(impl);}

			static ::Uml::CompositionChildRole meta_Dependency_children;
			Udm::ChildrenAttr< ::CSched::Dependency> Dependency_children() const { return Udm::ChildrenAttr< ::CSched::Dependency>(impl, meta_Dependency_children); }
			template <class Pred> Udm::ChildrenAttr< ::CSched::Dependency, Pred> Dependency_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::CSched::Dependency, Pred>(impl, meta_Dependency_children); }

			static ::Uml::CompositionChildRole meta_MsgRef_children;
			Udm::ChildrenAttr< ::CSched::MsgRef> MsgRef_children() const { return Udm::ChildrenAttr< ::CSched::MsgRef>(impl, meta_MsgRef_children); }
			template <class Pred> Udm::ChildrenAttr< ::CSched::MsgRef, Pred> MsgRef_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::CSched::MsgRef, Pred>(impl, meta_MsgRef_children); }

			static ::Uml::CompositionChildRole meta_Latency_children;
			Udm::ChildrenAttr< ::CSched::Latency> Latency_children() const { return Udm::ChildrenAttr< ::CSched::Latency>(impl, meta_Latency_children); }
			template <class Pred> Udm::ChildrenAttr< ::CSched::Latency, Pred> Latency_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::CSched::Latency, Pred>(impl, meta_Latency_children); }

			static ::Uml::CompositionChildRole meta_StartMsg_children;
			Udm::ChildrenAttr< ::CSched::StartMsg> StartMsg_children() const { return Udm::ChildrenAttr< ::CSched::StartMsg>(impl, meta_StartMsg_children); }
			template <class Pred> Udm::ChildrenAttr< ::CSched::StartMsg, Pred> StartMsg_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::CSched::StartMsg, Pred>(impl, meta_StartMsg_children); }

			static ::Uml::CompositionChildRole meta_EndMsg_children;
			Udm::ChildrenAttr< ::CSched::EndMsg> EndMsg_children() const { return Udm::ChildrenAttr< ::CSched::EndMsg>(impl, meta_EndMsg_children); }
			template <class Pred> Udm::ChildrenAttr< ::CSched::EndMsg, Pred> EndMsg_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::CSched::EndMsg, Pred>(impl, meta_EndMsg_children); }

			Udm::ChildrenAttr< ::CSched::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr< ::CSched::MgaObject>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::CSched::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::CSched::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::CSched::MsgRef> MsgRef_kind_children() const { return Udm::ChildrenAttr< ::CSched::MsgRef>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::CSched::MsgRef, Pred> MsgRef_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::CSched::MsgRef, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::CSched::Dependency> Dependency_kind_children() const { return Udm::ChildrenAttr< ::CSched::Dependency>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::CSched::Dependency, Pred> Dependency_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::CSched::Dependency, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::CSched::Latency> Latency_kind_children() const { return Udm::ChildrenAttr< ::CSched::Latency>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::CSched::Latency, Pred> Latency_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::CSched::Latency, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::CSched::StartMsg> StartMsg_kind_children() const { return Udm::ChildrenAttr< ::CSched::StartMsg>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::CSched::StartMsg, Pred> StartMsg_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::CSched::StartMsg, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::CSched::EndMsg> EndMsg_kind_children() const { return Udm::ChildrenAttr< ::CSched::EndMsg>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::CSched::EndMsg, Pred> EndMsg_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::CSched::EndMsg, Pred>(impl, Udm::NULLCHILDROLE); }

			static ::Uml::CompositionParentRole meta_TTSystem_parent;
			Udm::ParentAttr< ::CSched::TTSystem> TTSystem_parent() const { return Udm::ParentAttr< ::CSched::TTSystem>(impl, meta_TTSystem_parent); }

			Udm::ParentAttr< ::CSched::MgaObject> parent() const { return Udm::ParentAttr< ::CSched::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  MsgRef :  public ::CSched::MgaObject {
		public:
			static ::Uml::Class meta;

			MsgRef() { }
			MsgRef(Udm::ObjectImpl *impl) : ::CSched::MgaObject(impl) { }
			MsgRef(const MsgRef &master) : ::CSched::MgaObject(master) { }
			static MsgRef Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static MsgRef Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			MsgRef CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			MsgRef CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::CSched::MsgRef> Instances() { return Udm::InstantiatedAttr< ::CSched::MsgRef>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::CSched::MsgRef, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::CSched::MsgRef, Pred>(impl);}

			Udm::DerivedAttr< ::CSched::MsgRef> Derived() { return Udm::DerivedAttr< ::CSched::MsgRef>(impl);}
			template <class Pred> Udm::DerivedAttr< ::CSched::MsgRef, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::CSched::MsgRef, Pred>(impl);}

			Udm::ArchetypeAttr< ::CSched::MsgRef> Archetype() { return Udm::ArchetypeAttr< ::CSched::MsgRef>(impl);}

			static ::Uml::AssociationRole meta_ref;
			Udm::PointerAttr< ::CSched::Message> ref() const { return Udm::PointerAttr< ::CSched::Message>(impl, meta_ref); }

			static ::Uml::AssociationRole meta_srcDependency;
			static ::Uml::AssociationRole meta_srcDependency_rev;
			Udm::AClassAssocAttr< ::CSched::Dependency, ::CSched::MsgRef> srcDependency() const { return Udm::AClassAssocAttr< ::CSched::Dependency, ::CSched::MsgRef>(impl, meta_srcDependency, meta_srcDependency_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::CSched::Dependency, ::CSched::MsgRef, Pred> srcDependency_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::CSched::Dependency, ::CSched::MsgRef, Pred>(impl, meta_srcDependency, meta_srcDependency_rev); }

			static ::Uml::AssociationRole meta_dstDependency;
			static ::Uml::AssociationRole meta_dstDependency_rev;
			Udm::AClassAssocAttr< ::CSched::Dependency, ::CSched::MsgRef> dstDependency() const { return Udm::AClassAssocAttr< ::CSched::Dependency, ::CSched::MsgRef>(impl, meta_dstDependency, meta_dstDependency_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::CSched::Dependency, ::CSched::MsgRef, Pred> dstDependency_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::CSched::Dependency, ::CSched::MsgRef, Pred>(impl, meta_dstDependency, meta_dstDependency_rev); }

			static ::Uml::AssociationRole meta_dstEndMsg;
			static ::Uml::AssociationRole meta_dstEndMsg_rev;
			Udm::AClassAssocAttr< ::CSched::EndMsg, ::CSched::Latency> dstEndMsg() const { return Udm::AClassAssocAttr< ::CSched::EndMsg, ::CSched::Latency>(impl, meta_dstEndMsg, meta_dstEndMsg_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::CSched::EndMsg, ::CSched::Latency, Pred> dstEndMsg_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::CSched::EndMsg, ::CSched::Latency, Pred>(impl, meta_dstEndMsg, meta_dstEndMsg_rev); }

			static ::Uml::AssociationRole meta_srcStartMsg;
			static ::Uml::AssociationRole meta_srcStartMsg_rev;
			Udm::AClassAssocAttr< ::CSched::StartMsg, ::CSched::Latency> srcStartMsg() const { return Udm::AClassAssocAttr< ::CSched::StartMsg, ::CSched::Latency>(impl, meta_srcStartMsg, meta_srcStartMsg_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::CSched::StartMsg, ::CSched::Latency, Pred> srcStartMsg_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::CSched::StartMsg, ::CSched::Latency, Pred>(impl, meta_srcStartMsg, meta_srcStartMsg_rev); }

			static ::Uml::CompositionParentRole meta_LatencySpec_parent;
			Udm::ParentAttr< ::CSched::LatencySpec> LatencySpec_parent() const { return Udm::ParentAttr< ::CSched::LatencySpec>(impl, meta_LatencySpec_parent); }

			static ::Uml::CompositionParentRole meta_MsgInstances_parent;
			Udm::ParentAttr< ::CSched::MsgInstances> MsgInstances_parent() const { return Udm::ParentAttr< ::CSched::MsgInstances>(impl, meta_MsgInstances_parent); }

			Udm::ParentAttr< ::CSched::MgaObject> parent() const { return Udm::ParentAttr< ::CSched::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  Dependency :  public ::CSched::MgaObject {
		public:
			static ::Uml::Class meta;

			Dependency() { }
			Dependency(Udm::ObjectImpl *impl) : ::CSched::MgaObject(impl) { }
			Dependency(const Dependency &master) : ::CSched::MgaObject(master) { }
			static Dependency Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static Dependency Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			Dependency CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			Dependency CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::CSched::Dependency> Instances() { return Udm::InstantiatedAttr< ::CSched::Dependency>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::CSched::Dependency, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::CSched::Dependency, Pred>(impl);}

			Udm::DerivedAttr< ::CSched::Dependency> Derived() { return Udm::DerivedAttr< ::CSched::Dependency>(impl);}
			template <class Pred> Udm::DerivedAttr< ::CSched::Dependency, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::CSched::Dependency, Pred>(impl);}

			Udm::ArchetypeAttr< ::CSched::Dependency> Archetype() { return Udm::ArchetypeAttr< ::CSched::Dependency>(impl);}

			static ::Uml::CompositionParentRole meta_LatencySpec_parent;
			Udm::ParentAttr< ::CSched::LatencySpec> LatencySpec_parent() const { return Udm::ParentAttr< ::CSched::LatencySpec>(impl, meta_LatencySpec_parent); }

			Udm::ParentAttr< ::CSched::MgaObject> parent() const { return Udm::ParentAttr< ::CSched::MgaObject>(impl, Udm::NULLPARENTROLE); }
			static ::Uml::AssociationRole meta_dstDependency_end_;
			Udm::AssocEndAttr< ::CSched::MsgRef> dstDependency_end() const { return Udm::AssocEndAttr< ::CSched::MsgRef>(impl, meta_dstDependency_end_); }

			static ::Uml::AssociationRole meta_srcDependency_end_;
			Udm::AssocEndAttr< ::CSched::MsgRef> srcDependency_end() const { return Udm::AssocEndAttr< ::CSched::MsgRef>(impl, meta_srcDependency_end_); }

		};

		class  Latency :  public ::CSched::MgaObject {
		public:
			static ::Uml::Class meta;

			Latency() { }
			Latency(Udm::ObjectImpl *impl) : ::CSched::MgaObject(impl) { }
			Latency(const Latency &master) : ::CSched::MgaObject(master) { }
			static Latency Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static Latency Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			Latency CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			Latency CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::CSched::Latency> Instances() { return Udm::InstantiatedAttr< ::CSched::Latency>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::CSched::Latency, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::CSched::Latency, Pred>(impl);}

			Udm::DerivedAttr< ::CSched::Latency> Derived() { return Udm::DerivedAttr< ::CSched::Latency>(impl);}
			template <class Pred> Udm::DerivedAttr< ::CSched::Latency, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::CSched::Latency, Pred>(impl);}

			Udm::ArchetypeAttr< ::CSched::Latency> Archetype() { return Udm::ArchetypeAttr< ::CSched::Latency>(impl);}

			static ::Uml::Attribute meta_MaxLength;
			Udm::IntegerAttr MaxLength() const { return Udm::IntegerAttr(impl, meta_MaxLength); }

			static ::Uml::AssociationRole meta_srcEndMsg;
			static ::Uml::AssociationRole meta_srcEndMsg_rev;
			Udm::AClassAssocAttr< ::CSched::EndMsg, ::CSched::MsgRef> srcEndMsg() const { return Udm::AClassAssocAttr< ::CSched::EndMsg, ::CSched::MsgRef>(impl, meta_srcEndMsg, meta_srcEndMsg_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::CSched::EndMsg, ::CSched::MsgRef, Pred> srcEndMsg_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::CSched::EndMsg, ::CSched::MsgRef, Pred>(impl, meta_srcEndMsg, meta_srcEndMsg_rev); }

			static ::Uml::AssociationRole meta_dstStartMsg;
			static ::Uml::AssociationRole meta_dstStartMsg_rev;
			Udm::AClassAssocAttr< ::CSched::StartMsg, ::CSched::MsgRef> dstStartMsg() const { return Udm::AClassAssocAttr< ::CSched::StartMsg, ::CSched::MsgRef>(impl, meta_dstStartMsg, meta_dstStartMsg_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::CSched::StartMsg, ::CSched::MsgRef, Pred> dstStartMsg_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::CSched::StartMsg, ::CSched::MsgRef, Pred>(impl, meta_dstStartMsg, meta_dstStartMsg_rev); }

			static ::Uml::CompositionParentRole meta_LatencySpec_parent;
			Udm::ParentAttr< ::CSched::LatencySpec> LatencySpec_parent() const { return Udm::ParentAttr< ::CSched::LatencySpec>(impl, meta_LatencySpec_parent); }

			Udm::ParentAttr< ::CSched::MgaObject> parent() const { return Udm::ParentAttr< ::CSched::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  StartMsg :  public ::CSched::MgaObject {
		public:
			static ::Uml::Class meta;

			StartMsg() { }
			StartMsg(Udm::ObjectImpl *impl) : ::CSched::MgaObject(impl) { }
			StartMsg(const StartMsg &master) : ::CSched::MgaObject(master) { }
			static StartMsg Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static StartMsg Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			StartMsg CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			StartMsg CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::CSched::StartMsg> Instances() { return Udm::InstantiatedAttr< ::CSched::StartMsg>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::CSched::StartMsg, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::CSched::StartMsg, Pred>(impl);}

			Udm::DerivedAttr< ::CSched::StartMsg> Derived() { return Udm::DerivedAttr< ::CSched::StartMsg>(impl);}
			template <class Pred> Udm::DerivedAttr< ::CSched::StartMsg, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::CSched::StartMsg, Pred>(impl);}

			Udm::ArchetypeAttr< ::CSched::StartMsg> Archetype() { return Udm::ArchetypeAttr< ::CSched::StartMsg>(impl);}

			static ::Uml::CompositionParentRole meta_LatencySpec_parent;
			Udm::ParentAttr< ::CSched::LatencySpec> LatencySpec_parent() const { return Udm::ParentAttr< ::CSched::LatencySpec>(impl, meta_LatencySpec_parent); }

			Udm::ParentAttr< ::CSched::MgaObject> parent() const { return Udm::ParentAttr< ::CSched::MgaObject>(impl, Udm::NULLPARENTROLE); }
			static ::Uml::AssociationRole meta_dstStartMsg_end_;
			Udm::AssocEndAttr< ::CSched::MsgRef> dstStartMsg_end() const { return Udm::AssocEndAttr< ::CSched::MsgRef>(impl, meta_dstStartMsg_end_); }

			static ::Uml::AssociationRole meta_srcStartMsg_end_;
			Udm::AssocEndAttr< ::CSched::Latency> srcStartMsg_end() const { return Udm::AssocEndAttr< ::CSched::Latency>(impl, meta_srcStartMsg_end_); }

		};

		class  EndMsg :  public ::CSched::MgaObject {
		public:
			static ::Uml::Class meta;

			EndMsg() { }
			EndMsg(Udm::ObjectImpl *impl) : ::CSched::MgaObject(impl) { }
			EndMsg(const EndMsg &master) : ::CSched::MgaObject(master) { }
			static EndMsg Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static EndMsg Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			EndMsg CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			EndMsg CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::CSched::EndMsg> Instances() { return Udm::InstantiatedAttr< ::CSched::EndMsg>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::CSched::EndMsg, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::CSched::EndMsg, Pred>(impl);}

			Udm::DerivedAttr< ::CSched::EndMsg> Derived() { return Udm::DerivedAttr< ::CSched::EndMsg>(impl);}
			template <class Pred> Udm::DerivedAttr< ::CSched::EndMsg, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::CSched::EndMsg, Pred>(impl);}

			Udm::ArchetypeAttr< ::CSched::EndMsg> Archetype() { return Udm::ArchetypeAttr< ::CSched::EndMsg>(impl);}

			static ::Uml::CompositionParentRole meta_LatencySpec_parent;
			Udm::ParentAttr< ::CSched::LatencySpec> LatencySpec_parent() const { return Udm::ParentAttr< ::CSched::LatencySpec>(impl, meta_LatencySpec_parent); }

			Udm::ParentAttr< ::CSched::MgaObject> parent() const { return Udm::ParentAttr< ::CSched::MgaObject>(impl, Udm::NULLPARENTROLE); }
			static ::Uml::AssociationRole meta_srcEndMsg_end_;
			Udm::AssocEndAttr< ::CSched::MsgRef> srcEndMsg_end() const { return Udm::AssocEndAttr< ::CSched::MsgRef>(impl, meta_srcEndMsg_end_); }

			static ::Uml::AssociationRole meta_dstEndMsg_end_;
			Udm::AssocEndAttr< ::CSched::Latency> dstEndMsg_end() const { return Udm::AssocEndAttr< ::CSched::Latency>(impl, meta_dstEndMsg_end_); }

		};

		class  TimeSync :  public ::CSched::MgaObject {
		public:
			static ::Uml::Class meta;

			TimeSync() { }
			TimeSync(Udm::ObjectImpl *impl) : ::CSched::MgaObject(impl) { }
			TimeSync(const TimeSync &master) : ::CSched::MgaObject(master) { }
			static TimeSync Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static TimeSync Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			TimeSync CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			TimeSync CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::CSched::TimeSync> Instances() { return Udm::InstantiatedAttr< ::CSched::TimeSync>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::CSched::TimeSync, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::CSched::TimeSync, Pred>(impl);}

			Udm::DerivedAttr< ::CSched::TimeSync> Derived() { return Udm::DerivedAttr< ::CSched::TimeSync>(impl);}
			template <class Pred> Udm::DerivedAttr< ::CSched::TimeSync, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::CSched::TimeSync, Pred>(impl);}

			Udm::ArchetypeAttr< ::CSched::TimeSync> Archetype() { return Udm::ArchetypeAttr< ::CSched::TimeSync>(impl);}

			static ::Uml::Attribute meta_MsgLength;
			Udm::IntegerAttr MsgLength() const { return Udm::IntegerAttr(impl, meta_MsgLength); }

			static ::Uml::Attribute meta_Period;
			Udm::IntegerAttr Period() const { return Udm::IntegerAttr(impl, meta_Period); }

			static ::Uml::CompositionParentRole meta_Bus_parent;
			Udm::ParentAttr< ::CSched::Bus> Bus_parent() const { return Udm::ParentAttr< ::CSched::Bus>(impl, meta_Bus_parent); }

			Udm::ParentAttr< ::CSched::MgaObject> parent() const { return Udm::ParentAttr< ::CSched::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  RootFolder :  public Udm::Object {
		public:
			static ::Uml::Class meta;

			RootFolder() { }
			RootFolder(Udm::ObjectImpl *impl) : UDM_OBJECT(impl) { }
			RootFolder(const RootFolder &master) : UDM_OBJECT(master) { }
			static RootFolder Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static RootFolder Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			RootFolder CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			RootFolder CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::CSched::RootFolder> Instances() { return Udm::InstantiatedAttr< ::CSched::RootFolder>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::CSched::RootFolder, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::CSched::RootFolder, Pred>(impl);}

			Udm::DerivedAttr< ::CSched::RootFolder> Derived() { return Udm::DerivedAttr< ::CSched::RootFolder>(impl);}
			template <class Pred> Udm::DerivedAttr< ::CSched::RootFolder, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::CSched::RootFolder, Pred>(impl);}

			Udm::ArchetypeAttr< ::CSched::RootFolder> Archetype() { return Udm::ArchetypeAttr< ::CSched::RootFolder>(impl);}

			static ::Uml::Attribute meta_name;
			Udm::StringAttr name() const { return Udm::StringAttr(impl, meta_name); }

			static ::Uml::CompositionChildRole meta_TTSystem_children;
			Udm::ChildrenAttr< ::CSched::TTSystem> TTSystem_children() const { return Udm::ChildrenAttr< ::CSched::TTSystem>(impl, meta_TTSystem_children); }
			template <class Pred> Udm::ChildrenAttr< ::CSched::TTSystem, Pred> TTSystem_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::CSched::TTSystem, Pred>(impl, meta_TTSystem_children); }

			static ::Uml::CompositionChildRole meta_InstanceModel_children;
			Udm::ChildrenAttr< ::CSched::InstanceModel> InstanceModel_children() const { return Udm::ChildrenAttr< ::CSched::InstanceModel>(impl, meta_InstanceModel_children); }
			template <class Pred> Udm::ChildrenAttr< ::CSched::InstanceModel, Pred> InstanceModel_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::CSched::InstanceModel, Pred>(impl, meta_InstanceModel_children); }

			static ::Uml::CompositionChildRole meta_ConstraintProblem_children;
			Udm::ChildrenAttr< ::CSched::ConstraintProblem> ConstraintProblem_children() const { return Udm::ChildrenAttr< ::CSched::ConstraintProblem>(impl, meta_ConstraintProblem_children); }
			template <class Pred> Udm::ChildrenAttr< ::CSched::ConstraintProblem, Pred> ConstraintProblem_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::CSched::ConstraintProblem, Pred>(impl, meta_ConstraintProblem_children); }

			static ::Uml::CompositionChildRole meta_RootFolder_children;
			Udm::ChildrenAttr< ::CSched::RootFolder> RootFolder_children() const { return Udm::ChildrenAttr< ::CSched::RootFolder>(impl, meta_RootFolder_children); }
			template <class Pred> Udm::ChildrenAttr< ::CSched::RootFolder, Pred> RootFolder_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::CSched::RootFolder, Pred>(impl, meta_RootFolder_children); }

			Udm::ChildrenAttr< ::CSched::TTSystem> TTSystem_kind_children() const { return Udm::ChildrenAttr< ::CSched::TTSystem>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::CSched::TTSystem, Pred> TTSystem_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::CSched::TTSystem, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::CSched::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr< ::CSched::MgaObject>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::CSched::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::CSched::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::CSched::RootFolder> RootFolder_kind_children() const { return Udm::ChildrenAttr< ::CSched::RootFolder>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::CSched::RootFolder, Pred> RootFolder_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::CSched::RootFolder, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::CSched::InstanceModel> InstanceModel_kind_children() const { return Udm::ChildrenAttr< ::CSched::InstanceModel>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::CSched::InstanceModel, Pred> InstanceModel_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::CSched::InstanceModel, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::CSched::ConstraintProblem> ConstraintProblem_kind_children() const { return Udm::ChildrenAttr< ::CSched::ConstraintProblem>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::CSched::ConstraintProblem, Pred> ConstraintProblem_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::CSched::ConstraintProblem, Pred>(impl, Udm::NULLCHILDROLE); }

			static ::Uml::CompositionParentRole meta_RootFolder_parent;
			Udm::ParentAttr< ::CSched::RootFolder> RootFolder_parent() const { return Udm::ParentAttr< ::CSched::RootFolder>(impl, meta_RootFolder_parent); }

			Udm::ParentAttr< ::CSched::RootFolder> parent() const { return Udm::ParentAttr< ::CSched::RootFolder>(impl, Udm::NULLPARENTROLE); }
		};

		class  InstanceModel :  public ::CSched::MgaObject {
		public:
			static ::Uml::Class meta;

			InstanceModel() { }
			InstanceModel(Udm::ObjectImpl *impl) : ::CSched::MgaObject(impl) { }
			InstanceModel(const InstanceModel &master) : ::CSched::MgaObject(master) { }
			static InstanceModel Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static InstanceModel Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			InstanceModel CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			InstanceModel CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::CSched::InstanceModel> Instances() { return Udm::InstantiatedAttr< ::CSched::InstanceModel>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::CSched::InstanceModel, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::CSched::InstanceModel, Pred>(impl);}

			Udm::DerivedAttr< ::CSched::InstanceModel> Derived() { return Udm::DerivedAttr< ::CSched::InstanceModel>(impl);}
			template <class Pred> Udm::DerivedAttr< ::CSched::InstanceModel, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::CSched::InstanceModel, Pred>(impl);}

			Udm::ArchetypeAttr< ::CSched::InstanceModel> Archetype() { return Udm::ArchetypeAttr< ::CSched::InstanceModel>(impl);}

			static ::Uml::Attribute meta_HyperPeriod;
			Udm::IntegerAttr HyperPeriod() const { return Udm::IntegerAttr(impl, meta_HyperPeriod); }

			static ::Uml::AssociationRole meta_referedbyInstanceRef;
			Udm::AssocAttr< ::CSched::InstanceRef> referedbyInstanceRef() const { return Udm::AssocAttr< ::CSched::InstanceRef>(impl, meta_referedbyInstanceRef); }
			template <class Pred> Udm::AssocAttr< ::CSched::InstanceRef, Pred > referedbyInstanceRef_sorted(const Pred &) const { return Udm::AssocAttr< ::CSched::InstanceRef, Pred>(impl, meta_referedbyInstanceRef); }

			static ::Uml::CompositionChildRole meta_Processor_children;
			Udm::ChildrenAttr< ::CSched::Processor> Processor_children() const { return Udm::ChildrenAttr< ::CSched::Processor>(impl, meta_Processor_children); }
			template <class Pred> Udm::ChildrenAttr< ::CSched::Processor, Pred> Processor_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::CSched::Processor, Pred>(impl, meta_Processor_children); }

			static ::Uml::CompositionChildRole meta_Bus_children;
			Udm::ChildrenAttr< ::CSched::Bus> Bus_children() const { return Udm::ChildrenAttr< ::CSched::Bus>(impl, meta_Bus_children); }
			template <class Pred> Udm::ChildrenAttr< ::CSched::Bus, Pred> Bus_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::CSched::Bus, Pred>(impl, meta_Bus_children); }

			Udm::ChildrenAttr< ::CSched::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr< ::CSched::MgaObject>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::CSched::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::CSched::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::CSched::Processor> Processor_kind_children() const { return Udm::ChildrenAttr< ::CSched::Processor>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::CSched::Processor, Pred> Processor_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::CSched::Processor, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::CSched::Bus> Bus_kind_children() const { return Udm::ChildrenAttr< ::CSched::Bus>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::CSched::Bus, Pred> Bus_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::CSched::Bus, Pred>(impl, Udm::NULLCHILDROLE); }

			static ::Uml::CompositionParentRole meta_RootFolder_parent;
			Udm::ParentAttr< ::CSched::RootFolder> RootFolder_parent() const { return Udm::ParentAttr< ::CSched::RootFolder>(impl, meta_RootFolder_parent); }

			Udm::ParentAttr< ::CSched::RootFolder> parent() const { return Udm::ParentAttr< ::CSched::RootFolder>(impl, Udm::NULLPARENTROLE); }
		};

		class  MsgInstances :  public ::CSched::MgaObject {
		public:
			static ::Uml::Class meta;

			MsgInstances() { }
			MsgInstances(Udm::ObjectImpl *impl) : ::CSched::MgaObject(impl) { }
			MsgInstances(const MsgInstances &master) : ::CSched::MgaObject(master) { }
			static MsgInstances Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static MsgInstances Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			MsgInstances CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			MsgInstances CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::CSched::MsgInstances> Instances() { return Udm::InstantiatedAttr< ::CSched::MsgInstances>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::CSched::MsgInstances, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::CSched::MsgInstances, Pred>(impl);}

			Udm::DerivedAttr< ::CSched::MsgInstances> Derived() { return Udm::DerivedAttr< ::CSched::MsgInstances>(impl);}
			template <class Pred> Udm::DerivedAttr< ::CSched::MsgInstances, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::CSched::MsgInstances, Pred>(impl);}

			Udm::ArchetypeAttr< ::CSched::MsgInstances> Archetype() { return Udm::ArchetypeAttr< ::CSched::MsgInstances>(impl);}

			static ::Uml::CompositionChildRole meta_MsgRef_children;
			Udm::ChildrenAttr< ::CSched::MsgRef> MsgRef_children() const { return Udm::ChildrenAttr< ::CSched::MsgRef>(impl, meta_MsgRef_children); }
			template <class Pred> Udm::ChildrenAttr< ::CSched::MsgRef, Pred> MsgRef_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::CSched::MsgRef, Pred>(impl, meta_MsgRef_children); }

			static ::Uml::CompositionChildRole meta_MsgInstance_children;
			Udm::ChildrenAttr< ::CSched::MsgInstance> MsgInstance_children() const { return Udm::ChildrenAttr< ::CSched::MsgInstance>(impl, meta_MsgInstance_children); }
			template <class Pred> Udm::ChildrenAttr< ::CSched::MsgInstance, Pred> MsgInstance_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::CSched::MsgInstance, Pred>(impl, meta_MsgInstance_children); }

			Udm::ChildrenAttr< ::CSched::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr< ::CSched::MgaObject>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::CSched::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::CSched::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::CSched::MsgRef> MsgRef_kind_children() const { return Udm::ChildrenAttr< ::CSched::MsgRef>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::CSched::MsgRef, Pred> MsgRef_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::CSched::MsgRef, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::CSched::MsgInstance> MsgInstance_kind_children() const { return Udm::ChildrenAttr< ::CSched::MsgInstance>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::CSched::MsgInstance, Pred> MsgInstance_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::CSched::MsgInstance, Pred>(impl, Udm::NULLCHILDROLE); }

			static ::Uml::CompositionParentRole meta_Bus_parent;
			Udm::ParentAttr< ::CSched::Bus> Bus_parent() const { return Udm::ParentAttr< ::CSched::Bus>(impl, meta_Bus_parent); }

			Udm::ParentAttr< ::CSched::MgaObject> parent() const { return Udm::ParentAttr< ::CSched::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  TaskInstances :  public ::CSched::MgaObject {
		public:
			static ::Uml::Class meta;

			TaskInstances() { }
			TaskInstances(Udm::ObjectImpl *impl) : ::CSched::MgaObject(impl) { }
			TaskInstances(const TaskInstances &master) : ::CSched::MgaObject(master) { }
			static TaskInstances Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static TaskInstances Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			TaskInstances CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			TaskInstances CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::CSched::TaskInstances> Instances() { return Udm::InstantiatedAttr< ::CSched::TaskInstances>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::CSched::TaskInstances, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::CSched::TaskInstances, Pred>(impl);}

			Udm::DerivedAttr< ::CSched::TaskInstances> Derived() { return Udm::DerivedAttr< ::CSched::TaskInstances>(impl);}
			template <class Pred> Udm::DerivedAttr< ::CSched::TaskInstances, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::CSched::TaskInstances, Pred>(impl);}

			Udm::ArchetypeAttr< ::CSched::TaskInstances> Archetype() { return Udm::ArchetypeAttr< ::CSched::TaskInstances>(impl);}

			static ::Uml::CompositionChildRole meta_TaskInstance_children;
			Udm::ChildrenAttr< ::CSched::TaskInstance> TaskInstance_children() const { return Udm::ChildrenAttr< ::CSched::TaskInstance>(impl, meta_TaskInstance_children); }
			template <class Pred> Udm::ChildrenAttr< ::CSched::TaskInstance, Pred> TaskInstance_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::CSched::TaskInstance, Pred>(impl, meta_TaskInstance_children); }

			static ::Uml::CompositionChildRole meta_TaskRef_children;
			Udm::ChildrenAttr< ::CSched::TaskRef> TaskRef_children() const { return Udm::ChildrenAttr< ::CSched::TaskRef>(impl, meta_TaskRef_children); }
			template <class Pred> Udm::ChildrenAttr< ::CSched::TaskRef, Pred> TaskRef_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::CSched::TaskRef, Pred>(impl, meta_TaskRef_children); }

			Udm::ChildrenAttr< ::CSched::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr< ::CSched::MgaObject>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::CSched::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::CSched::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::CSched::TaskRef> TaskRef_kind_children() const { return Udm::ChildrenAttr< ::CSched::TaskRef>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::CSched::TaskRef, Pred> TaskRef_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::CSched::TaskRef, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::CSched::TaskInstance> TaskInstance_kind_children() const { return Udm::ChildrenAttr< ::CSched::TaskInstance>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::CSched::TaskInstance, Pred> TaskInstance_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::CSched::TaskInstance, Pred>(impl, Udm::NULLCHILDROLE); }

			static ::Uml::CompositionParentRole meta_Processor_parent;
			Udm::ParentAttr< ::CSched::Processor> Processor_parent() const { return Udm::ParentAttr< ::CSched::Processor>(impl, meta_Processor_parent); }

			Udm::ParentAttr< ::CSched::MgaObject> parent() const { return Udm::ParentAttr< ::CSched::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  MsgInstance :  public ::CSched::MgaObject {
		public:
			static ::Uml::Class meta;

			MsgInstance() { }
			MsgInstance(Udm::ObjectImpl *impl) : ::CSched::MgaObject(impl) { }
			MsgInstance(const MsgInstance &master) : ::CSched::MgaObject(master) { }
			static MsgInstance Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static MsgInstance Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			MsgInstance CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			MsgInstance CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::CSched::MsgInstance> Instances() { return Udm::InstantiatedAttr< ::CSched::MsgInstance>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::CSched::MsgInstance, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::CSched::MsgInstance, Pred>(impl);}

			Udm::DerivedAttr< ::CSched::MsgInstance> Derived() { return Udm::DerivedAttr< ::CSched::MsgInstance>(impl);}
			template <class Pred> Udm::DerivedAttr< ::CSched::MsgInstance, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::CSched::MsgInstance, Pred>(impl);}

			Udm::ArchetypeAttr< ::CSched::MsgInstance> Archetype() { return Udm::ArchetypeAttr< ::CSched::MsgInstance>(impl);}

			static ::Uml::Attribute meta_RefName;
			Udm::StringAttr RefName() const { return Udm::StringAttr(impl, meta_RefName); }

			static ::Uml::Attribute meta_StartTime;
			Udm::IntegerAttr StartTime() const { return Udm::IntegerAttr(impl, meta_StartTime); }

			static ::Uml::Attribute meta_InstID;
			Udm::IntegerAttr InstID() const { return Udm::IntegerAttr(impl, meta_InstID); }

			static ::Uml::AssociationRole meta_referedbyMsgInstRef;
			Udm::AssocAttr< ::CSched::MsgInstRef> referedbyMsgInstRef() const { return Udm::AssocAttr< ::CSched::MsgInstRef>(impl, meta_referedbyMsgInstRef); }
			template <class Pred> Udm::AssocAttr< ::CSched::MsgInstRef, Pred > referedbyMsgInstRef_sorted(const Pred &) const { return Udm::AssocAttr< ::CSched::MsgInstRef, Pred>(impl, meta_referedbyMsgInstRef); }

			static ::Uml::CompositionParentRole meta_MsgInstances_parent;
			Udm::ParentAttr< ::CSched::MsgInstances> MsgInstances_parent() const { return Udm::ParentAttr< ::CSched::MsgInstances>(impl, meta_MsgInstances_parent); }

			Udm::ParentAttr< ::CSched::MgaObject> parent() const { return Udm::ParentAttr< ::CSched::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  TaskInstance :  public ::CSched::MgaObject {
		public:
			static ::Uml::Class meta;

			TaskInstance() { }
			TaskInstance(Udm::ObjectImpl *impl) : ::CSched::MgaObject(impl) { }
			TaskInstance(const TaskInstance &master) : ::CSched::MgaObject(master) { }
			static TaskInstance Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static TaskInstance Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			TaskInstance CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			TaskInstance CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::CSched::TaskInstance> Instances() { return Udm::InstantiatedAttr< ::CSched::TaskInstance>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::CSched::TaskInstance, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::CSched::TaskInstance, Pred>(impl);}

			Udm::DerivedAttr< ::CSched::TaskInstance> Derived() { return Udm::DerivedAttr< ::CSched::TaskInstance>(impl);}
			template <class Pred> Udm::DerivedAttr< ::CSched::TaskInstance, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::CSched::TaskInstance, Pred>(impl);}

			Udm::ArchetypeAttr< ::CSched::TaskInstance> Archetype() { return Udm::ArchetypeAttr< ::CSched::TaskInstance>(impl);}

			static ::Uml::Attribute meta_RefName;
			Udm::StringAttr RefName() const { return Udm::StringAttr(impl, meta_RefName); }

			static ::Uml::Attribute meta_StartTime;
			Udm::IntegerAttr StartTime() const { return Udm::IntegerAttr(impl, meta_StartTime); }

			static ::Uml::Attribute meta_InstID;
			Udm::IntegerAttr InstID() const { return Udm::IntegerAttr(impl, meta_InstID); }

			static ::Uml::AssociationRole meta_referedbyTaskInstRef;
			Udm::AssocAttr< ::CSched::TaskInstRef> referedbyTaskInstRef() const { return Udm::AssocAttr< ::CSched::TaskInstRef>(impl, meta_referedbyTaskInstRef); }
			template <class Pred> Udm::AssocAttr< ::CSched::TaskInstRef, Pred > referedbyTaskInstRef_sorted(const Pred &) const { return Udm::AssocAttr< ::CSched::TaskInstRef, Pred>(impl, meta_referedbyTaskInstRef); }

			static ::Uml::CompositionParentRole meta_TaskInstances_parent;
			Udm::ParentAttr< ::CSched::TaskInstances> TaskInstances_parent() const { return Udm::ParentAttr< ::CSched::TaskInstances>(impl, meta_TaskInstances_parent); }

			Udm::ParentAttr< ::CSched::MgaObject> parent() const { return Udm::ParentAttr< ::CSched::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  ReceiverInst :  public ::CSched::MgaObject {
		public:
			static ::Uml::Class meta;

			ReceiverInst() { }
			ReceiverInst(Udm::ObjectImpl *impl) : ::CSched::MgaObject(impl) { }
			ReceiverInst(const ReceiverInst &master) : ::CSched::MgaObject(master) { }
			static ReceiverInst Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static ReceiverInst Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			ReceiverInst CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			ReceiverInst CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::CSched::ReceiverInst> Instances() { return Udm::InstantiatedAttr< ::CSched::ReceiverInst>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::CSched::ReceiverInst, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::CSched::ReceiverInst, Pred>(impl);}

			Udm::DerivedAttr< ::CSched::ReceiverInst> Derived() { return Udm::DerivedAttr< ::CSched::ReceiverInst>(impl);}
			template <class Pred> Udm::DerivedAttr< ::CSched::ReceiverInst, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::CSched::ReceiverInst, Pred>(impl);}

			Udm::ArchetypeAttr< ::CSched::ReceiverInst> Archetype() { return Udm::ArchetypeAttr< ::CSched::ReceiverInst>(impl);}

			static ::Uml::CompositionParentRole meta_Dependencies_parent;
			Udm::ParentAttr< ::CSched::Dependencies> Dependencies_parent() const { return Udm::ParentAttr< ::CSched::Dependencies>(impl, meta_Dependencies_parent); }

			Udm::ParentAttr< ::CSched::MgaObject> parent() const { return Udm::ParentAttr< ::CSched::MgaObject>(impl, Udm::NULLPARENTROLE); }
			static ::Uml::AssociationRole meta_srcReceiverInst_end_;
			Udm::AssocEndAttr< ::CSched::MsgInstRef> srcReceiverInst_end() const { return Udm::AssocEndAttr< ::CSched::MsgInstRef>(impl, meta_srcReceiverInst_end_); }

			static ::Uml::AssociationRole meta_dstReceiverInst_end_;
			Udm::AssocEndAttr< ::CSched::TaskInstRef> dstReceiverInst_end() const { return Udm::AssocEndAttr< ::CSched::TaskInstRef>(impl, meta_dstReceiverInst_end_); }

		};

		class  SenderInst :  public ::CSched::MgaObject {
		public:
			static ::Uml::Class meta;

			SenderInst() { }
			SenderInst(Udm::ObjectImpl *impl) : ::CSched::MgaObject(impl) { }
			SenderInst(const SenderInst &master) : ::CSched::MgaObject(master) { }
			static SenderInst Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static SenderInst Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			SenderInst CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			SenderInst CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::CSched::SenderInst> Instances() { return Udm::InstantiatedAttr< ::CSched::SenderInst>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::CSched::SenderInst, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::CSched::SenderInst, Pred>(impl);}

			Udm::DerivedAttr< ::CSched::SenderInst> Derived() { return Udm::DerivedAttr< ::CSched::SenderInst>(impl);}
			template <class Pred> Udm::DerivedAttr< ::CSched::SenderInst, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::CSched::SenderInst, Pred>(impl);}

			Udm::ArchetypeAttr< ::CSched::SenderInst> Archetype() { return Udm::ArchetypeAttr< ::CSched::SenderInst>(impl);}

			static ::Uml::CompositionParentRole meta_Dependencies_parent;
			Udm::ParentAttr< ::CSched::Dependencies> Dependencies_parent() const { return Udm::ParentAttr< ::CSched::Dependencies>(impl, meta_Dependencies_parent); }

			Udm::ParentAttr< ::CSched::MgaObject> parent() const { return Udm::ParentAttr< ::CSched::MgaObject>(impl, Udm::NULLPARENTROLE); }
			static ::Uml::AssociationRole meta_dstSenderInst_end_;
			Udm::AssocEndAttr< ::CSched::MsgInstRef> dstSenderInst_end() const { return Udm::AssocEndAttr< ::CSched::MsgInstRef>(impl, meta_dstSenderInst_end_); }

			static ::Uml::AssociationRole meta_srcSenderInst_end_;
			Udm::AssocEndAttr< ::CSched::TaskInstRef> srcSenderInst_end() const { return Udm::AssocEndAttr< ::CSched::TaskInstRef>(impl, meta_srcSenderInst_end_); }

		};

		class  Dependencies :  public ::CSched::MgaObject {
		public:
			static ::Uml::Class meta;

			Dependencies() { }
			Dependencies(Udm::ObjectImpl *impl) : ::CSched::MgaObject(impl) { }
			Dependencies(const Dependencies &master) : ::CSched::MgaObject(master) { }
			static Dependencies Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static Dependencies Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			Dependencies CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			Dependencies CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::CSched::Dependencies> Instances() { return Udm::InstantiatedAttr< ::CSched::Dependencies>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::CSched::Dependencies, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::CSched::Dependencies, Pred>(impl);}

			Udm::DerivedAttr< ::CSched::Dependencies> Derived() { return Udm::DerivedAttr< ::CSched::Dependencies>(impl);}
			template <class Pred> Udm::DerivedAttr< ::CSched::Dependencies, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::CSched::Dependencies, Pred>(impl);}

			Udm::ArchetypeAttr< ::CSched::Dependencies> Archetype() { return Udm::ArchetypeAttr< ::CSched::Dependencies>(impl);}

			static ::Uml::CompositionChildRole meta_TaskInstRef_children;
			Udm::ChildrenAttr< ::CSched::TaskInstRef> TaskInstRef_children() const { return Udm::ChildrenAttr< ::CSched::TaskInstRef>(impl, meta_TaskInstRef_children); }
			template <class Pred> Udm::ChildrenAttr< ::CSched::TaskInstRef, Pred> TaskInstRef_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::CSched::TaskInstRef, Pred>(impl, meta_TaskInstRef_children); }

			static ::Uml::CompositionChildRole meta_MsgInstRef_children;
			Udm::ChildrenAttr< ::CSched::MsgInstRef> MsgInstRef_children() const { return Udm::ChildrenAttr< ::CSched::MsgInstRef>(impl, meta_MsgInstRef_children); }
			template <class Pred> Udm::ChildrenAttr< ::CSched::MsgInstRef, Pred> MsgInstRef_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::CSched::MsgInstRef, Pred>(impl, meta_MsgInstRef_children); }

			static ::Uml::CompositionChildRole meta_SenderInst_children;
			Udm::ChildrenAttr< ::CSched::SenderInst> SenderInst_children() const { return Udm::ChildrenAttr< ::CSched::SenderInst>(impl, meta_SenderInst_children); }
			template <class Pred> Udm::ChildrenAttr< ::CSched::SenderInst, Pred> SenderInst_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::CSched::SenderInst, Pred>(impl, meta_SenderInst_children); }

			static ::Uml::CompositionChildRole meta_ReceiverInst_children;
			Udm::ChildrenAttr< ::CSched::ReceiverInst> ReceiverInst_children() const { return Udm::ChildrenAttr< ::CSched::ReceiverInst>(impl, meta_ReceiverInst_children); }
			template <class Pred> Udm::ChildrenAttr< ::CSched::ReceiverInst, Pred> ReceiverInst_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::CSched::ReceiverInst, Pred>(impl, meta_ReceiverInst_children); }

			Udm::ChildrenAttr< ::CSched::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr< ::CSched::MgaObject>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::CSched::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::CSched::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::CSched::ReceiverInst> ReceiverInst_kind_children() const { return Udm::ChildrenAttr< ::CSched::ReceiverInst>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::CSched::ReceiverInst, Pred> ReceiverInst_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::CSched::ReceiverInst, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::CSched::SenderInst> SenderInst_kind_children() const { return Udm::ChildrenAttr< ::CSched::SenderInst>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::CSched::SenderInst, Pred> SenderInst_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::CSched::SenderInst, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::CSched::MsgInstRef> MsgInstRef_kind_children() const { return Udm::ChildrenAttr< ::CSched::MsgInstRef>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::CSched::MsgInstRef, Pred> MsgInstRef_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::CSched::MsgInstRef, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::CSched::TaskInstRef> TaskInstRef_kind_children() const { return Udm::ChildrenAttr< ::CSched::TaskInstRef>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::CSched::TaskInstRef, Pred> TaskInstRef_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::CSched::TaskInstRef, Pred>(impl, Udm::NULLCHILDROLE); }

			static ::Uml::CompositionParentRole meta_Bus_parent;
			Udm::ParentAttr< ::CSched::Bus> Bus_parent() const { return Udm::ParentAttr< ::CSched::Bus>(impl, meta_Bus_parent); }

			Udm::ParentAttr< ::CSched::MgaObject> parent() const { return Udm::ParentAttr< ::CSched::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  MsgInstRef :  public ::CSched::MgaObject {
		public:
			static ::Uml::Class meta;

			MsgInstRef() { }
			MsgInstRef(Udm::ObjectImpl *impl) : ::CSched::MgaObject(impl) { }
			MsgInstRef(const MsgInstRef &master) : ::CSched::MgaObject(master) { }
			static MsgInstRef Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static MsgInstRef Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			MsgInstRef CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			MsgInstRef CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::CSched::MsgInstRef> Instances() { return Udm::InstantiatedAttr< ::CSched::MsgInstRef>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::CSched::MsgInstRef, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::CSched::MsgInstRef, Pred>(impl);}

			Udm::DerivedAttr< ::CSched::MsgInstRef> Derived() { return Udm::DerivedAttr< ::CSched::MsgInstRef>(impl);}
			template <class Pred> Udm::DerivedAttr< ::CSched::MsgInstRef, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::CSched::MsgInstRef, Pred>(impl);}

			Udm::ArchetypeAttr< ::CSched::MsgInstRef> Archetype() { return Udm::ArchetypeAttr< ::CSched::MsgInstRef>(impl);}

			static ::Uml::AssociationRole meta_ref;
			Udm::PointerAttr< ::CSched::MsgInstance> ref() const { return Udm::PointerAttr< ::CSched::MsgInstance>(impl, meta_ref); }

			static ::Uml::AssociationRole meta_dstReceiverInst;
			static ::Uml::AssociationRole meta_dstReceiverInst_rev;
			Udm::AClassAssocAttr< ::CSched::ReceiverInst, ::CSched::TaskInstRef> dstReceiverInst() const { return Udm::AClassAssocAttr< ::CSched::ReceiverInst, ::CSched::TaskInstRef>(impl, meta_dstReceiverInst, meta_dstReceiverInst_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::CSched::ReceiverInst, ::CSched::TaskInstRef, Pred> dstReceiverInst_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::CSched::ReceiverInst, ::CSched::TaskInstRef, Pred>(impl, meta_dstReceiverInst, meta_dstReceiverInst_rev); }

			static ::Uml::AssociationRole meta_srcSenderInst;
			static ::Uml::AssociationRole meta_srcSenderInst_rev;
			Udm::AClassAssocAttr< ::CSched::SenderInst, ::CSched::TaskInstRef> srcSenderInst() const { return Udm::AClassAssocAttr< ::CSched::SenderInst, ::CSched::TaskInstRef>(impl, meta_srcSenderInst, meta_srcSenderInst_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::CSched::SenderInst, ::CSched::TaskInstRef, Pred> srcSenderInst_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::CSched::SenderInst, ::CSched::TaskInstRef, Pred>(impl, meta_srcSenderInst, meta_srcSenderInst_rev); }

			static ::Uml::CompositionParentRole meta_Dependencies_parent;
			Udm::ParentAttr< ::CSched::Dependencies> Dependencies_parent() const { return Udm::ParentAttr< ::CSched::Dependencies>(impl, meta_Dependencies_parent); }

			Udm::ParentAttr< ::CSched::MgaObject> parent() const { return Udm::ParentAttr< ::CSched::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  TaskInstRef :  public ::CSched::MgaObject {
		public:
			static ::Uml::Class meta;

			TaskInstRef() { }
			TaskInstRef(Udm::ObjectImpl *impl) : ::CSched::MgaObject(impl) { }
			TaskInstRef(const TaskInstRef &master) : ::CSched::MgaObject(master) { }
			static TaskInstRef Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static TaskInstRef Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			TaskInstRef CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			TaskInstRef CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::CSched::TaskInstRef> Instances() { return Udm::InstantiatedAttr< ::CSched::TaskInstRef>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::CSched::TaskInstRef, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::CSched::TaskInstRef, Pred>(impl);}

			Udm::DerivedAttr< ::CSched::TaskInstRef> Derived() { return Udm::DerivedAttr< ::CSched::TaskInstRef>(impl);}
			template <class Pred> Udm::DerivedAttr< ::CSched::TaskInstRef, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::CSched::TaskInstRef, Pred>(impl);}

			Udm::ArchetypeAttr< ::CSched::TaskInstRef> Archetype() { return Udm::ArchetypeAttr< ::CSched::TaskInstRef>(impl);}

			static ::Uml::AssociationRole meta_ref;
			Udm::PointerAttr< ::CSched::TaskInstance> ref() const { return Udm::PointerAttr< ::CSched::TaskInstance>(impl, meta_ref); }

			static ::Uml::AssociationRole meta_srcReceiverInst;
			static ::Uml::AssociationRole meta_srcReceiverInst_rev;
			Udm::AClassAssocAttr< ::CSched::ReceiverInst, ::CSched::MsgInstRef> srcReceiverInst() const { return Udm::AClassAssocAttr< ::CSched::ReceiverInst, ::CSched::MsgInstRef>(impl, meta_srcReceiverInst, meta_srcReceiverInst_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::CSched::ReceiverInst, ::CSched::MsgInstRef, Pred> srcReceiverInst_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::CSched::ReceiverInst, ::CSched::MsgInstRef, Pred>(impl, meta_srcReceiverInst, meta_srcReceiverInst_rev); }

			static ::Uml::AssociationRole meta_dstSenderInst;
			static ::Uml::AssociationRole meta_dstSenderInst_rev;
			Udm::AClassAssocAttr< ::CSched::SenderInst, ::CSched::MsgInstRef> dstSenderInst() const { return Udm::AClassAssocAttr< ::CSched::SenderInst, ::CSched::MsgInstRef>(impl, meta_dstSenderInst, meta_dstSenderInst_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::CSched::SenderInst, ::CSched::MsgInstRef, Pred> dstSenderInst_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::CSched::SenderInst, ::CSched::MsgInstRef, Pred>(impl, meta_dstSenderInst, meta_dstSenderInst_rev); }

			static ::Uml::CompositionParentRole meta_Dependencies_parent;
			Udm::ParentAttr< ::CSched::Dependencies> Dependencies_parent() const { return Udm::ParentAttr< ::CSched::Dependencies>(impl, meta_Dependencies_parent); }

			Udm::ParentAttr< ::CSched::MgaObject> parent() const { return Udm::ParentAttr< ::CSched::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  Assignment :  public ::CSched::MgaObject {
		public:
			static ::Uml::Class meta;

			Assignment() { }
			Assignment(Udm::ObjectImpl *impl) : ::CSched::MgaObject(impl) { }
			Assignment(const Assignment &master) : ::CSched::MgaObject(master) { }
			static Assignment Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static Assignment Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			Assignment CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			Assignment CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::CSched::Assignment> Instances() { return Udm::InstantiatedAttr< ::CSched::Assignment>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::CSched::Assignment, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::CSched::Assignment, Pred>(impl);}

			Udm::DerivedAttr< ::CSched::Assignment> Derived() { return Udm::DerivedAttr< ::CSched::Assignment>(impl);}
			template <class Pred> Udm::DerivedAttr< ::CSched::Assignment, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::CSched::Assignment, Pred>(impl);}

			Udm::ArchetypeAttr< ::CSched::Assignment> Archetype() { return Udm::ArchetypeAttr< ::CSched::Assignment>(impl);}

			static ::Uml::Attribute meta_Value;
			Udm::IntegerAttr Value() const { return Udm::IntegerAttr(impl, meta_Value); }

			static ::Uml::CompositionParentRole meta_Solution_parent;
			Udm::ParentAttr< ::CSched::Solution> Solution_parent() const { return Udm::ParentAttr< ::CSched::Solution>(impl, meta_Solution_parent); }

			Udm::ParentAttr< ::CSched::MgaObject> parent() const { return Udm::ParentAttr< ::CSched::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  Member :  public ::CSched::MgaObject {
		public:
			static ::Uml::Class meta;

			Member() { }
			Member(Udm::ObjectImpl *impl) : ::CSched::MgaObject(impl) { }
			Member(const Member &master) : ::CSched::MgaObject(master) { }
			static Member Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static Member Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			Member CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			Member CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::CSched::Member> Instances() { return Udm::InstantiatedAttr< ::CSched::Member>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::CSched::Member, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::CSched::Member, Pred>(impl);}

			Udm::DerivedAttr< ::CSched::Member> Derived() { return Udm::DerivedAttr< ::CSched::Member>(impl);}
			template <class Pred> Udm::DerivedAttr< ::CSched::Member, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::CSched::Member, Pred>(impl);}

			Udm::ArchetypeAttr< ::CSched::Member> Archetype() { return Udm::ArchetypeAttr< ::CSched::Member>(impl);}

			static ::Uml::CompositionParentRole meta_ConstraintProblem_parent;
			Udm::ParentAttr< ::CSched::ConstraintProblem> ConstraintProblem_parent() const { return Udm::ParentAttr< ::CSched::ConstraintProblem>(impl, meta_ConstraintProblem_parent); }

			Udm::ParentAttr< ::CSched::MgaObject> parent() const { return Udm::ParentAttr< ::CSched::MgaObject>(impl, Udm::NULLPARENTROLE); }
			static ::Uml::AssociationRole meta_dstMember_end_;
			Udm::AssocEndAttr< ::CSched::Connectable> dstMember_end() const { return Udm::AssocEndAttr< ::CSched::Connectable>(impl, meta_dstMember_end_); }

			static ::Uml::AssociationRole meta_srcMember_end_;
			Udm::AssocEndAttr< ::CSched::FDVar> srcMember_end() const { return Udm::AssocEndAttr< ::CSched::FDVar>(impl, meta_srcMember_end_); }

		};

		class  InstanceRef :  public ::CSched::MgaObject {
		public:
			static ::Uml::Class meta;

			InstanceRef() { }
			InstanceRef(Udm::ObjectImpl *impl) : ::CSched::MgaObject(impl) { }
			InstanceRef(const InstanceRef &master) : ::CSched::MgaObject(master) { }
			static InstanceRef Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static InstanceRef Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			InstanceRef CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			InstanceRef CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::CSched::InstanceRef> Instances() { return Udm::InstantiatedAttr< ::CSched::InstanceRef>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::CSched::InstanceRef, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::CSched::InstanceRef, Pred>(impl);}

			Udm::DerivedAttr< ::CSched::InstanceRef> Derived() { return Udm::DerivedAttr< ::CSched::InstanceRef>(impl);}
			template <class Pred> Udm::DerivedAttr< ::CSched::InstanceRef, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::CSched::InstanceRef, Pred>(impl);}

			Udm::ArchetypeAttr< ::CSched::InstanceRef> Archetype() { return Udm::ArchetypeAttr< ::CSched::InstanceRef>(impl);}

			static ::Uml::AssociationRole meta_ref;
			Udm::PointerAttr< ::CSched::InstanceModel> ref() const { return Udm::PointerAttr< ::CSched::InstanceModel>(impl, meta_ref); }

			static ::Uml::CompositionParentRole meta_ConstraintProblem_parent;
			Udm::ParentAttr< ::CSched::ConstraintProblem> ConstraintProblem_parent() const { return Udm::ParentAttr< ::CSched::ConstraintProblem>(impl, meta_ConstraintProblem_parent); }

			Udm::ParentAttr< ::CSched::MgaObject> parent() const { return Udm::ParentAttr< ::CSched::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  Solution :  public ::CSched::MgaObject {
		public:
			static ::Uml::Class meta;

			Solution() { }
			Solution(Udm::ObjectImpl *impl) : ::CSched::MgaObject(impl) { }
			Solution(const Solution &master) : ::CSched::MgaObject(master) { }
			static Solution Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static Solution Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			Solution CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			Solution CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::CSched::Solution> Instances() { return Udm::InstantiatedAttr< ::CSched::Solution>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::CSched::Solution, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::CSched::Solution, Pred>(impl);}

			Udm::DerivedAttr< ::CSched::Solution> Derived() { return Udm::DerivedAttr< ::CSched::Solution>(impl);}
			template <class Pred> Udm::DerivedAttr< ::CSched::Solution, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::CSched::Solution, Pred>(impl);}

			Udm::ArchetypeAttr< ::CSched::Solution> Archetype() { return Udm::ArchetypeAttr< ::CSched::Solution>(impl);}

			static ::Uml::CompositionChildRole meta_Assignment_children;
			Udm::ChildrenAttr< ::CSched::Assignment> Assignment_children() const { return Udm::ChildrenAttr< ::CSched::Assignment>(impl, meta_Assignment_children); }
			template <class Pred> Udm::ChildrenAttr< ::CSched::Assignment, Pred> Assignment_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::CSched::Assignment, Pred>(impl, meta_Assignment_children); }

			Udm::ChildrenAttr< ::CSched::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr< ::CSched::MgaObject>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::CSched::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::CSched::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::CSched::Assignment> Assignment_kind_children() const { return Udm::ChildrenAttr< ::CSched::Assignment>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::CSched::Assignment, Pred> Assignment_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::CSched::Assignment, Pred>(impl, Udm::NULLCHILDROLE); }

			static ::Uml::CompositionParentRole meta_ConstraintProblem_parent;
			Udm::ParentAttr< ::CSched::ConstraintProblem> ConstraintProblem_parent() const { return Udm::ParentAttr< ::CSched::ConstraintProblem>(impl, meta_ConstraintProblem_parent); }

			Udm::ParentAttr< ::CSched::MgaObject> parent() const { return Udm::ParentAttr< ::CSched::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  Connectable :  virtual public ::CSched::MgaObject {
		public:
			static ::Uml::Class meta;

			Connectable() { }
			Connectable(Udm::ObjectImpl *impl) : ::CSched::MgaObject(impl) { }
			Connectable(const Connectable &master) : ::CSched::MgaObject(master) { }
			static Connectable Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static Connectable Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			Connectable CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			Connectable CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::CSched::Connectable> Instances() { return Udm::InstantiatedAttr< ::CSched::Connectable>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::CSched::Connectable, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::CSched::Connectable, Pred>(impl);}

			Udm::DerivedAttr< ::CSched::Connectable> Derived() { return Udm::DerivedAttr< ::CSched::Connectable>(impl);}
			template <class Pred> Udm::DerivedAttr< ::CSched::Connectable, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::CSched::Connectable, Pred>(impl);}

			Udm::ArchetypeAttr< ::CSched::Connectable> Archetype() { return Udm::ArchetypeAttr< ::CSched::Connectable>(impl);}

			static ::Uml::AssociationRole meta_srcMember;
			static ::Uml::AssociationRole meta_srcMember_rev;
			Udm::AClassAssocAttr< ::CSched::Member, ::CSched::FDVar> srcMember() const { return Udm::AClassAssocAttr< ::CSched::Member, ::CSched::FDVar>(impl, meta_srcMember, meta_srcMember_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::CSched::Member, ::CSched::FDVar, Pred> srcMember_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::CSched::Member, ::CSched::FDVar, Pred>(impl, meta_srcMember, meta_srcMember_rev); }

			Udm::ParentAttr<Udm::Object> parent() const { return Udm::ParentAttr<Udm::Object>(impl, Udm::NULLPARENTROLE); }
		};

		class  YPort :  public ::CSched::Connectable {
		public:
			static ::Uml::Class meta;

			YPort() { }
			YPort(Udm::ObjectImpl *impl) : ::CSched::Connectable(impl), ::CSched::MgaObject(impl) { }
			YPort(const YPort &master) : ::CSched::Connectable(master), ::CSched::MgaObject(master) { }
			static YPort Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static YPort Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			YPort CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			YPort CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::CSched::YPort> Instances() { return Udm::InstantiatedAttr< ::CSched::YPort>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::CSched::YPort, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::CSched::YPort, Pred>(impl);}

			Udm::DerivedAttr< ::CSched::YPort> Derived() { return Udm::DerivedAttr< ::CSched::YPort>(impl);}
			template <class Pred> Udm::DerivedAttr< ::CSched::YPort, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::CSched::YPort, Pred>(impl);}

			Udm::ArchetypeAttr< ::CSched::YPort> Archetype() { return Udm::ArchetypeAttr< ::CSched::YPort>(impl);}

			static ::Uml::CompositionParentRole meta_PairConstraint_parent;
			Udm::ParentAttr< ::CSched::PairConstraint> PairConstraint_parent() const { return Udm::ParentAttr< ::CSched::PairConstraint>(impl, meta_PairConstraint_parent); }

			Udm::ParentAttr< ::CSched::MgaObject> parent() const { return Udm::ParentAttr< ::CSched::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  XPort :  public ::CSched::Connectable {
		public:
			static ::Uml::Class meta;

			XPort() { }
			XPort(Udm::ObjectImpl *impl) : ::CSched::Connectable(impl), ::CSched::MgaObject(impl) { }
			XPort(const XPort &master) : ::CSched::Connectable(master), ::CSched::MgaObject(master) { }
			static XPort Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static XPort Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			XPort CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			XPort CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::CSched::XPort> Instances() { return Udm::InstantiatedAttr< ::CSched::XPort>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::CSched::XPort, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::CSched::XPort, Pred>(impl);}

			Udm::DerivedAttr< ::CSched::XPort> Derived() { return Udm::DerivedAttr< ::CSched::XPort>(impl);}
			template <class Pred> Udm::DerivedAttr< ::CSched::XPort, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::CSched::XPort, Pred>(impl);}

			Udm::ArchetypeAttr< ::CSched::XPort> Archetype() { return Udm::ArchetypeAttr< ::CSched::XPort>(impl);}

			static ::Uml::CompositionParentRole meta_PairConstraint_parent;
			Udm::ParentAttr< ::CSched::PairConstraint> PairConstraint_parent() const { return Udm::ParentAttr< ::CSched::PairConstraint>(impl, meta_PairConstraint_parent); }

			Udm::ParentAttr< ::CSched::MgaObject> parent() const { return Udm::ParentAttr< ::CSched::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  FDVar :  public ::CSched::MgaObject {
		public:
			static ::Uml::Class meta;

			FDVar() { }
			FDVar(Udm::ObjectImpl *impl) : ::CSched::MgaObject(impl) { }
			FDVar(const FDVar &master) : ::CSched::MgaObject(master) { }
			static FDVar Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static FDVar Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			FDVar CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			FDVar CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::CSched::FDVar> Instances() { return Udm::InstantiatedAttr< ::CSched::FDVar>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::CSched::FDVar, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::CSched::FDVar, Pred>(impl);}

			Udm::DerivedAttr< ::CSched::FDVar> Derived() { return Udm::DerivedAttr< ::CSched::FDVar>(impl);}
			template <class Pred> Udm::DerivedAttr< ::CSched::FDVar, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::CSched::FDVar, Pred>(impl);}

			Udm::ArchetypeAttr< ::CSched::FDVar> Archetype() { return Udm::ArchetypeAttr< ::CSched::FDVar>(impl);}

			static ::Uml::Attribute meta_UpperBound;
			Udm::IntegerAttr UpperBound() const { return Udm::IntegerAttr(impl, meta_UpperBound); }

			static ::Uml::Attribute meta_LowerBound;
			Udm::IntegerAttr LowerBound() const { return Udm::IntegerAttr(impl, meta_LowerBound); }

			static ::Uml::Attribute meta_Size;
			Udm::IntegerAttr Size() const { return Udm::IntegerAttr(impl, meta_Size); }

			static ::Uml::AssociationRole meta_dstMember;
			static ::Uml::AssociationRole meta_dstMember_rev;
			Udm::AClassAssocAttr< ::CSched::Member, ::CSched::Connectable> dstMember() const { return Udm::AClassAssocAttr< ::CSched::Member, ::CSched::Connectable>(impl, meta_dstMember, meta_dstMember_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::CSched::Member, ::CSched::Connectable, Pred> dstMember_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::CSched::Member, ::CSched::Connectable, Pred>(impl, meta_dstMember, meta_dstMember_rev); }

			static ::Uml::CompositionParentRole meta_ConstraintProblem_parent;
			Udm::ParentAttr< ::CSched::ConstraintProblem> ConstraintProblem_parent() const { return Udm::ParentAttr< ::CSched::ConstraintProblem>(impl, meta_ConstraintProblem_parent); }

			Udm::ParentAttr< ::CSched::MgaObject> parent() const { return Udm::ParentAttr< ::CSched::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  ConstraintProblem :  public ::CSched::MgaObject {
		public:
			static ::Uml::Class meta;

			ConstraintProblem() { }
			ConstraintProblem(Udm::ObjectImpl *impl) : ::CSched::MgaObject(impl) { }
			ConstraintProblem(const ConstraintProblem &master) : ::CSched::MgaObject(master) { }
			static ConstraintProblem Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static ConstraintProblem Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			ConstraintProblem CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			ConstraintProblem CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::CSched::ConstraintProblem> Instances() { return Udm::InstantiatedAttr< ::CSched::ConstraintProblem>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::CSched::ConstraintProblem, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::CSched::ConstraintProblem, Pred>(impl);}

			Udm::DerivedAttr< ::CSched::ConstraintProblem> Derived() { return Udm::DerivedAttr< ::CSched::ConstraintProblem>(impl);}
			template <class Pred> Udm::DerivedAttr< ::CSched::ConstraintProblem, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::CSched::ConstraintProblem, Pred>(impl);}

			Udm::ArchetypeAttr< ::CSched::ConstraintProblem> Archetype() { return Udm::ArchetypeAttr< ::CSched::ConstraintProblem>(impl);}

			static ::Uml::Attribute meta_UpperBound;
			Udm::IntegerAttr UpperBound() const { return Udm::IntegerAttr(impl, meta_UpperBound); }

			static ::Uml::Attribute meta_LowerBound;
			Udm::IntegerAttr LowerBound() const { return Udm::IntegerAttr(impl, meta_LowerBound); }

			static ::Uml::CompositionChildRole meta_Member_children;
			Udm::ChildrenAttr< ::CSched::Member> Member_children() const { return Udm::ChildrenAttr< ::CSched::Member>(impl, meta_Member_children); }
			template <class Pred> Udm::ChildrenAttr< ::CSched::Member, Pred> Member_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::CSched::Member, Pred>(impl, meta_Member_children); }

			static ::Uml::CompositionChildRole meta_Solution_children;
			Udm::ChildrenAttr< ::CSched::Solution> Solution_children() const { return Udm::ChildrenAttr< ::CSched::Solution>(impl, meta_Solution_children); }
			template <class Pred> Udm::ChildrenAttr< ::CSched::Solution, Pred> Solution_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::CSched::Solution, Pred>(impl, meta_Solution_children); }

			static ::Uml::CompositionChildRole meta_SearchConfig_child;
			Udm::ChildAttr< ::CSched::SearchConfig> SearchConfig_child() const { return Udm::ChildAttr< ::CSched::SearchConfig>(impl, meta_SearchConfig_child); }

			static ::Uml::CompositionChildRole meta_Constraint_children;
			Udm::ChildrenAttr< ::CSched::Constraint> Constraint_children() const { return Udm::ChildrenAttr< ::CSched::Constraint>(impl, meta_Constraint_children); }
			template <class Pred> Udm::ChildrenAttr< ::CSched::Constraint, Pred> Constraint_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::CSched::Constraint, Pred>(impl, meta_Constraint_children); }

			static ::Uml::CompositionChildRole meta_FDVar_children;
			Udm::ChildrenAttr< ::CSched::FDVar> FDVar_children() const { return Udm::ChildrenAttr< ::CSched::FDVar>(impl, meta_FDVar_children); }
			template <class Pred> Udm::ChildrenAttr< ::CSched::FDVar, Pred> FDVar_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::CSched::FDVar, Pred>(impl, meta_FDVar_children); }

			static ::Uml::CompositionChildRole meta_InstanceRef_child;
			Udm::ChildAttr< ::CSched::InstanceRef> InstanceRef_child() const { return Udm::ChildAttr< ::CSched::InstanceRef>(impl, meta_InstanceRef_child); }

			Udm::ChildrenAttr< ::CSched::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr< ::CSched::MgaObject>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::CSched::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::CSched::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::CSched::EqualityConstraint> EqualityConstraint_kind_children() const { return Udm::ChildrenAttr< ::CSched::EqualityConstraint>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::CSched::EqualityConstraint, Pred> EqualityConstraint_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::CSched::EqualityConstraint, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::CSched::InequalityConstraint> InequalityConstraint_kind_children() const { return Udm::ChildrenAttr< ::CSched::InequalityConstraint>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::CSched::InequalityConstraint, Pred> InequalityConstraint_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::CSched::InequalityConstraint, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::CSched::Member> Member_kind_children() const { return Udm::ChildrenAttr< ::CSched::Member>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::CSched::Member, Pred> Member_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::CSched::Member, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::CSched::RangeConstraint> RangeConstraint_kind_children() const { return Udm::ChildrenAttr< ::CSched::RangeConstraint>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::CSched::RangeConstraint, Pred> RangeConstraint_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::CSched::RangeConstraint, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::CSched::InstanceRef> InstanceRef_kind_children() const { return Udm::ChildrenAttr< ::CSched::InstanceRef>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::CSched::InstanceRef, Pred> InstanceRef_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::CSched::InstanceRef, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::CSched::Solution> Solution_kind_children() const { return Udm::ChildrenAttr< ::CSched::Solution>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::CSched::Solution, Pred> Solution_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::CSched::Solution, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::CSched::PairConstraint> PairConstraint_kind_children() const { return Udm::ChildrenAttr< ::CSched::PairConstraint>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::CSched::PairConstraint, Pred> PairConstraint_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::CSched::PairConstraint, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::CSched::FDVar> FDVar_kind_children() const { return Udm::ChildrenAttr< ::CSched::FDVar>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::CSched::FDVar, Pred> FDVar_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::CSched::FDVar, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::CSched::Constraint> Constraint_kind_children() const { return Udm::ChildrenAttr< ::CSched::Constraint>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::CSched::Constraint, Pred> Constraint_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::CSched::Constraint, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::CSched::SerialConstraint> SerialConstraint_kind_children() const { return Udm::ChildrenAttr< ::CSched::SerialConstraint>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::CSched::SerialConstraint, Pred> SerialConstraint_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::CSched::SerialConstraint, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::CSched::SearchConfig> SearchConfig_kind_children() const { return Udm::ChildrenAttr< ::CSched::SearchConfig>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::CSched::SearchConfig, Pred> SearchConfig_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::CSched::SearchConfig, Pred>(impl, Udm::NULLCHILDROLE); }

			static ::Uml::CompositionParentRole meta_RootFolder_parent;
			Udm::ParentAttr< ::CSched::RootFolder> RootFolder_parent() const { return Udm::ParentAttr< ::CSched::RootFolder>(impl, meta_RootFolder_parent); }

			Udm::ParentAttr< ::CSched::RootFolder> parent() const { return Udm::ParentAttr< ::CSched::RootFolder>(impl, Udm::NULLPARENTROLE); }
		};

		class  Constraint :  virtual public ::CSched::MgaObject {
		public:
			static ::Uml::Class meta;

			Constraint() { }
			Constraint(Udm::ObjectImpl *impl) : ::CSched::MgaObject(impl) { }
			Constraint(const Constraint &master) : ::CSched::MgaObject(master) { }
			static Constraint Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static Constraint Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			Constraint CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			Constraint CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::CSched::Constraint> Instances() { return Udm::InstantiatedAttr< ::CSched::Constraint>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::CSched::Constraint, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::CSched::Constraint, Pred>(impl);}

			Udm::DerivedAttr< ::CSched::Constraint> Derived() { return Udm::DerivedAttr< ::CSched::Constraint>(impl);}
			template <class Pred> Udm::DerivedAttr< ::CSched::Constraint, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::CSched::Constraint, Pred>(impl);}

			Udm::ArchetypeAttr< ::CSched::Constraint> Archetype() { return Udm::ArchetypeAttr< ::CSched::Constraint>(impl);}

			static ::Uml::CompositionParentRole meta_ConstraintProblem_parent;
			Udm::ParentAttr< ::CSched::ConstraintProblem> ConstraintProblem_parent() const { return Udm::ParentAttr< ::CSched::ConstraintProblem>(impl, meta_ConstraintProblem_parent); }

			Udm::ParentAttr< ::CSched::MgaObject> parent() const { return Udm::ParentAttr< ::CSched::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  RangeConstraint :  public ::CSched::Connectable, public ::CSched::Constraint {
		public:
			static ::Uml::Class meta;

			RangeConstraint() { }
			RangeConstraint(Udm::ObjectImpl *impl) : ::CSched::Connectable(impl),::CSched::Constraint(impl), ::CSched::MgaObject(impl) { }
			RangeConstraint(const RangeConstraint &master) : ::CSched::Connectable(master),::CSched::Constraint(master), ::CSched::MgaObject(master) { }
			static RangeConstraint Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static RangeConstraint Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			RangeConstraint CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			RangeConstraint CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::CSched::RangeConstraint> Instances() { return Udm::InstantiatedAttr< ::CSched::RangeConstraint>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::CSched::RangeConstraint, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::CSched::RangeConstraint, Pred>(impl);}

			Udm::DerivedAttr< ::CSched::RangeConstraint> Derived() { return Udm::DerivedAttr< ::CSched::RangeConstraint>(impl);}
			template <class Pred> Udm::DerivedAttr< ::CSched::RangeConstraint, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::CSched::RangeConstraint, Pred>(impl);}

			Udm::ArchetypeAttr< ::CSched::RangeConstraint> Archetype() { return Udm::ArchetypeAttr< ::CSched::RangeConstraint>(impl);}

			static ::Uml::Attribute meta_UpperBound;
			Udm::IntegerAttr UpperBound() const { return Udm::IntegerAttr(impl, meta_UpperBound); }

			static ::Uml::Attribute meta_LowerBound;
			Udm::IntegerAttr LowerBound() const { return Udm::IntegerAttr(impl, meta_LowerBound); }

			Udm::ParentAttr< ::CSched::MgaObject> parent() const { return Udm::ParentAttr< ::CSched::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  PairConstraint :  public ::CSched::Constraint {
		public:
			static ::Uml::Class meta;

			PairConstraint() { }
			PairConstraint(Udm::ObjectImpl *impl) : ::CSched::Constraint(impl), ::CSched::MgaObject(impl) { }
			PairConstraint(const PairConstraint &master) : ::CSched::Constraint(master), ::CSched::MgaObject(master) { }
			static PairConstraint Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static PairConstraint Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			PairConstraint CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			PairConstraint CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::CSched::PairConstraint> Instances() { return Udm::InstantiatedAttr< ::CSched::PairConstraint>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::CSched::PairConstraint, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::CSched::PairConstraint, Pred>(impl);}

			Udm::DerivedAttr< ::CSched::PairConstraint> Derived() { return Udm::DerivedAttr< ::CSched::PairConstraint>(impl);}
			template <class Pred> Udm::DerivedAttr< ::CSched::PairConstraint, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::CSched::PairConstraint, Pred>(impl);}

			Udm::ArchetypeAttr< ::CSched::PairConstraint> Archetype() { return Udm::ArchetypeAttr< ::CSched::PairConstraint>(impl);}

			static ::Uml::CompositionChildRole meta_YPort_children;
			Udm::ChildrenAttr< ::CSched::YPort> YPort_children() const { return Udm::ChildrenAttr< ::CSched::YPort>(impl, meta_YPort_children); }
			template <class Pred> Udm::ChildrenAttr< ::CSched::YPort, Pred> YPort_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::CSched::YPort, Pred>(impl, meta_YPort_children); }

			static ::Uml::CompositionChildRole meta_XPort_children;
			Udm::ChildrenAttr< ::CSched::XPort> XPort_children() const { return Udm::ChildrenAttr< ::CSched::XPort>(impl, meta_XPort_children); }
			template <class Pred> Udm::ChildrenAttr< ::CSched::XPort, Pred> XPort_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::CSched::XPort, Pred>(impl, meta_XPort_children); }

			Udm::ChildrenAttr< ::CSched::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr< ::CSched::MgaObject>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::CSched::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::CSched::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::CSched::Connectable> Connectable_kind_children() const { return Udm::ChildrenAttr< ::CSched::Connectable>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::CSched::Connectable, Pred> Connectable_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::CSched::Connectable, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::CSched::YPort> YPort_kind_children() const { return Udm::ChildrenAttr< ::CSched::YPort>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::CSched::YPort, Pred> YPort_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::CSched::YPort, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::CSched::XPort> XPort_kind_children() const { return Udm::ChildrenAttr< ::CSched::XPort>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::CSched::XPort, Pred> XPort_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::CSched::XPort, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ParentAttr< ::CSched::MgaObject> parent() const { return Udm::ParentAttr< ::CSched::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  EqualityConstraint :  public ::CSched::PairConstraint {
		public:
			static ::Uml::Class meta;

			EqualityConstraint() { }
			EqualityConstraint(Udm::ObjectImpl *impl) : ::CSched::PairConstraint(impl), ::CSched::MgaObject(impl) { }
			EqualityConstraint(const EqualityConstraint &master) : ::CSched::PairConstraint(master), ::CSched::MgaObject(master) { }
			static EqualityConstraint Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static EqualityConstraint Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			EqualityConstraint CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			EqualityConstraint CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::CSched::EqualityConstraint> Instances() { return Udm::InstantiatedAttr< ::CSched::EqualityConstraint>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::CSched::EqualityConstraint, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::CSched::EqualityConstraint, Pred>(impl);}

			Udm::DerivedAttr< ::CSched::EqualityConstraint> Derived() { return Udm::DerivedAttr< ::CSched::EqualityConstraint>(impl);}
			template <class Pred> Udm::DerivedAttr< ::CSched::EqualityConstraint, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::CSched::EqualityConstraint, Pred>(impl);}

			Udm::ArchetypeAttr< ::CSched::EqualityConstraint> Archetype() { return Udm::ArchetypeAttr< ::CSched::EqualityConstraint>(impl);}

			static ::Uml::Attribute meta_Offset;
			Udm::IntegerAttr Offset() const { return Udm::IntegerAttr(impl, meta_Offset); }

			Udm::ParentAttr< ::CSched::MgaObject> parent() const { return Udm::ParentAttr< ::CSched::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  InequalityConstraint :  public ::CSched::PairConstraint {
		public:
			static ::Uml::Class meta;

			InequalityConstraint() { }
			InequalityConstraint(Udm::ObjectImpl *impl) : ::CSched::PairConstraint(impl), ::CSched::MgaObject(impl) { }
			InequalityConstraint(const InequalityConstraint &master) : ::CSched::PairConstraint(master), ::CSched::MgaObject(master) { }
			static InequalityConstraint Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static InequalityConstraint Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			InequalityConstraint CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			InequalityConstraint CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::CSched::InequalityConstraint> Instances() { return Udm::InstantiatedAttr< ::CSched::InequalityConstraint>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::CSched::InequalityConstraint, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::CSched::InequalityConstraint, Pred>(impl);}

			Udm::DerivedAttr< ::CSched::InequalityConstraint> Derived() { return Udm::DerivedAttr< ::CSched::InequalityConstraint>(impl);}
			template <class Pred> Udm::DerivedAttr< ::CSched::InequalityConstraint, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::CSched::InequalityConstraint, Pred>(impl);}

			Udm::ArchetypeAttr< ::CSched::InequalityConstraint> Archetype() { return Udm::ArchetypeAttr< ::CSched::InequalityConstraint>(impl);}

			static ::Uml::Attribute meta_Offset;
			Udm::IntegerAttr Offset() const { return Udm::IntegerAttr(impl, meta_Offset); }

			Udm::ParentAttr< ::CSched::MgaObject> parent() const { return Udm::ParentAttr< ::CSched::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  SerialConstraint :  public ::CSched::Connectable, public ::CSched::Constraint {
		public:
			static ::Uml::Class meta;

			SerialConstraint() { }
			SerialConstraint(Udm::ObjectImpl *impl) : ::CSched::Connectable(impl),::CSched::Constraint(impl), ::CSched::MgaObject(impl) { }
			SerialConstraint(const SerialConstraint &master) : ::CSched::Connectable(master),::CSched::Constraint(master), ::CSched::MgaObject(master) { }
			static SerialConstraint Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static SerialConstraint Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			SerialConstraint CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			SerialConstraint CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::CSched::SerialConstraint> Instances() { return Udm::InstantiatedAttr< ::CSched::SerialConstraint>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::CSched::SerialConstraint, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::CSched::SerialConstraint, Pred>(impl);}

			Udm::DerivedAttr< ::CSched::SerialConstraint> Derived() { return Udm::DerivedAttr< ::CSched::SerialConstraint>(impl);}
			template <class Pred> Udm::DerivedAttr< ::CSched::SerialConstraint, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::CSched::SerialConstraint, Pred>(impl);}

			Udm::ArchetypeAttr< ::CSched::SerialConstraint> Archetype() { return Udm::ArchetypeAttr< ::CSched::SerialConstraint>(impl);}

			Udm::ParentAttr< ::CSched::MgaObject> parent() const { return Udm::ParentAttr< ::CSched::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  SearchConfig :  public ::CSched::MgaObject {
		public:
			static ::Uml::Class meta;

			SearchConfig() { }
			SearchConfig(Udm::ObjectImpl *impl) : ::CSched::MgaObject(impl) { }
			SearchConfig(const SearchConfig &master) : ::CSched::MgaObject(master) { }
			static SearchConfig Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static SearchConfig Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			SearchConfig CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			SearchConfig CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::CSched::SearchConfig> Instances() { return Udm::InstantiatedAttr< ::CSched::SearchConfig>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::CSched::SearchConfig, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::CSched::SearchConfig, Pred>(impl);}

			Udm::DerivedAttr< ::CSched::SearchConfig> Derived() { return Udm::DerivedAttr< ::CSched::SearchConfig>(impl);}
			template <class Pred> Udm::DerivedAttr< ::CSched::SearchConfig, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::CSched::SearchConfig, Pred>(impl);}

			Udm::ArchetypeAttr< ::CSched::SearchConfig> Archetype() { return Udm::ArchetypeAttr< ::CSched::SearchConfig>(impl);}

			static ::Uml::Attribute meta_MaxRunTimeSec;
			Udm::IntegerAttr MaxRunTimeSec() const { return Udm::IntegerAttr(impl, meta_MaxRunTimeSec); }

			static ::Uml::Attribute meta_MaxFailures;
			Udm::IntegerAttr MaxFailures() const { return Udm::IntegerAttr(impl, meta_MaxFailures); }

			static ::Uml::Attribute meta_SearchEngine;
			Udm::StringAttr SearchEngine() const { return Udm::StringAttr(impl, meta_SearchEngine); }

			static ::Uml::Attribute meta_MaxSolutions;
			Udm::IntegerAttr MaxSolutions() const { return Udm::IntegerAttr(impl, meta_MaxSolutions); }

			static ::Uml::CompositionParentRole meta_ConstraintProblem_parent;
			Udm::ParentAttr< ::CSched::ConstraintProblem> ConstraintProblem_parent() const { return Udm::ParentAttr< ::CSched::ConstraintProblem>(impl, meta_ConstraintProblem_parent); }

			Udm::ParentAttr< ::CSched::MgaObject> parent() const { return Udm::ParentAttr< ::CSched::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};



}

#endif //MOBIES_CSCHED_H
