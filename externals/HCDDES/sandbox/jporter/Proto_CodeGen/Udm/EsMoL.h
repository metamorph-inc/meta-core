#ifndef MOBIES_ESMOL_H
#define MOBIES_ESMOL_H
// header file ESMoL.h generated from diagram ESMoL
// generated on Wed Sep 24 11:32:06 2008

#ifndef MOBIES_UDMBASE_H
#include "UdmBase.h"
#endif

#ifdef min
#undef min
#endif

#ifdef max
#undef max
#endif
namespace ESMoL {
	extern  Udm::UdmDiagram diagram;
	 void Initialize(const ::Uml::Diagram &dgr);
	 void Initialize();

		class  TTTask;
		class  TTBus;
		class  TTMessage;
		class  OldBus;
		class  OldTask;
		class  CCode;
		class  SubsystemRef;
		class  COutputPort;
		class  CInputPort;
		class  IOPortExp;
		class  IOPortInfo;
		class  IOPortInfoRef;
		class  IOPortAssignment;
		class  IOPortInfoBase;
		class  ArchitectureLibrary;
		class  System;
		class  DesignFolder;
		class  Connector;
		class  ComponentBase;
		class  Output;
		class  Input;
		class  Types;
		class  TypeStruct;
		class  Matrix;
		class  TypeBase;
		class  TypeBaseRef;
		class  TimingConstraint;
		class  RequirementsLibrary;
		class  TimingSheet;
		class  TaskRef;
		class  Primitive;
		class  Block;
		class  Parameter;
		class  Reference;
		class  Annotation;
		class  Line;
		class  Dataflow;
		class  Subsystem;
		class  OutputPort;
		class  OutPort;
		class  StatePort;
		class  Port;
		class  EnablePort;
		class  TriggerPort;
		class  InputPort;
		class  InPort;
		class  ActionPort;
		class  State;
		class  Transition;
		class  History;
		class  TransStart;
		class  Junction;
		class  ConnectorRef;
		class  Stateflow;
		class  TransConnector;
		class  Data;
		class  Event;
		class  StateDE;
		class  OS;
		class  Wire;
		class  HardwareUnit;
		class  PlatformLibrary;
		class  Router;
		class  Connectable;
		class  Capability;
		class  HWElement;
		class  IChan;
		class  OChan;
		class  BChan;
		class  Bus;
		class  Node;
		class  Channel;
		class  OutCommMapping;
		class  CommDst;
		class  CommMapping;
		class  InCommMapping;
		class  Component;
		class  Message;
		class  MessageRef;
		class  IOPort;
		class  MsgAssignment;
		class  TaskAssignment;
		class  DeploymentLibrary;
		class  Deployment;
		class  Task;
		class  NodeRef;
		class  ChannelRef;
		class  RootFolder;
		class  MgaObject;


		 void CreateMetaObjs();
		 void InitCrossNSInheritence();
		 void InitCrossNSCompositions();
		 void InitCrossNSAssociations();
		class  ArchitectureLibrary :  public Udm::Object {
		public:
			static ::Uml::Class meta;

			ArchitectureLibrary() { }
			ArchitectureLibrary(Udm::ObjectImpl *impl) : UDM_OBJECT(impl) { }
			ArchitectureLibrary(const ArchitectureLibrary &master) : UDM_OBJECT(master) { }
			static ArchitectureLibrary Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static ArchitectureLibrary Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			ArchitectureLibrary CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			ArchitectureLibrary CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::ESMoL::ArchitectureLibrary> Instances() { return Udm::InstantiatedAttr< ::ESMoL::ArchitectureLibrary>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::ESMoL::ArchitectureLibrary, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::ESMoL::ArchitectureLibrary, Pred>(impl);}

			Udm::DerivedAttr< ::ESMoL::ArchitectureLibrary> Derived() { return Udm::DerivedAttr< ::ESMoL::ArchitectureLibrary>(impl);}
			template <class Pred> Udm::DerivedAttr< ::ESMoL::ArchitectureLibrary, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::ESMoL::ArchitectureLibrary, Pred>(impl);}

			Udm::ArchetypeAttr< ::ESMoL::ArchitectureLibrary> Archetype() { return Udm::ArchetypeAttr< ::ESMoL::ArchitectureLibrary>(impl);}

			static ::Uml::Attribute meta_name;
			Udm::StringAttr name() const { return Udm::StringAttr(impl, meta_name); }

			static ::Uml::CompositionChildRole meta_System_children;
			Udm::ChildrenAttr< ::ESMoL::System> System_children() const { return Udm::ChildrenAttr< ::ESMoL::System>(impl, meta_System_children); }
			template <class Pred> Udm::ChildrenAttr< ::ESMoL::System, Pred> System_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::System, Pred>(impl, meta_System_children); }

			Udm::ChildrenAttr< ::ESMoL::System> System_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::System>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::System, Pred> System_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::System, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::ESMoL::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::MgaObject>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

			static ::Uml::CompositionParentRole meta_RootFolder_parent;
			Udm::ParentAttr< ::ESMoL::RootFolder> RootFolder_parent() const { return Udm::ParentAttr< ::ESMoL::RootFolder>(impl, meta_RootFolder_parent); }

			Udm::ParentAttr< ::ESMoL::RootFolder> parent() const { return Udm::ParentAttr< ::ESMoL::RootFolder>(impl, Udm::NULLPARENTROLE); }
		};

		class  DesignFolder :  public Udm::Object {
		public:
			static ::Uml::Class meta;

			DesignFolder() { }
			DesignFolder(Udm::ObjectImpl *impl) : UDM_OBJECT(impl) { }
			DesignFolder(const DesignFolder &master) : UDM_OBJECT(master) { }
			static DesignFolder Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static DesignFolder Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			DesignFolder CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			DesignFolder CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::ESMoL::DesignFolder> Instances() { return Udm::InstantiatedAttr< ::ESMoL::DesignFolder>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::ESMoL::DesignFolder, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::ESMoL::DesignFolder, Pred>(impl);}

			Udm::DerivedAttr< ::ESMoL::DesignFolder> Derived() { return Udm::DerivedAttr< ::ESMoL::DesignFolder>(impl);}
			template <class Pred> Udm::DerivedAttr< ::ESMoL::DesignFolder, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::ESMoL::DesignFolder, Pred>(impl);}

			Udm::ArchetypeAttr< ::ESMoL::DesignFolder> Archetype() { return Udm::ArchetypeAttr< ::ESMoL::DesignFolder>(impl);}

			static ::Uml::Attribute meta_name;
			Udm::StringAttr name() const { return Udm::StringAttr(impl, meta_name); }

			static ::Uml::CompositionChildRole meta_TimingSheet_children;
			Udm::ChildrenAttr< ::ESMoL::TimingSheet> TimingSheet_children() const { return Udm::ChildrenAttr< ::ESMoL::TimingSheet>(impl, meta_TimingSheet_children); }
			template <class Pred> Udm::ChildrenAttr< ::ESMoL::TimingSheet, Pred> TimingSheet_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::TimingSheet, Pred>(impl, meta_TimingSheet_children); }

			static ::Uml::CompositionChildRole meta_System_children;
			Udm::ChildrenAttr< ::ESMoL::System> System_children() const { return Udm::ChildrenAttr< ::ESMoL::System>(impl, meta_System_children); }
			template <class Pred> Udm::ChildrenAttr< ::ESMoL::System, Pred> System_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::System, Pred>(impl, meta_System_children); }

			static ::Uml::CompositionChildRole meta_Subsystem_children;
			Udm::ChildrenAttr< ::ESMoL::Subsystem> Subsystem_children() const { return Udm::ChildrenAttr< ::ESMoL::Subsystem>(impl, meta_Subsystem_children); }
			template <class Pred> Udm::ChildrenAttr< ::ESMoL::Subsystem, Pred> Subsystem_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::Subsystem, Pred>(impl, meta_Subsystem_children); }

			static ::Uml::CompositionChildRole meta_HardwareUnit_children;
			Udm::ChildrenAttr< ::ESMoL::HardwareUnit> HardwareUnit_children() const { return Udm::ChildrenAttr< ::ESMoL::HardwareUnit>(impl, meta_HardwareUnit_children); }
			template <class Pred> Udm::ChildrenAttr< ::ESMoL::HardwareUnit, Pred> HardwareUnit_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::HardwareUnit, Pred>(impl, meta_HardwareUnit_children); }

			static ::Uml::CompositionChildRole meta_Deployment_children;
			Udm::ChildrenAttr< ::ESMoL::Deployment> Deployment_children() const { return Udm::ChildrenAttr< ::ESMoL::Deployment>(impl, meta_Deployment_children); }
			template <class Pred> Udm::ChildrenAttr< ::ESMoL::Deployment, Pred> Deployment_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::Deployment, Pred>(impl, meta_Deployment_children); }

			Udm::ChildrenAttr< ::ESMoL::System> System_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::System>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::System, Pred> System_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::System, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::ESMoL::ComponentBase> ComponentBase_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::ComponentBase>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::ComponentBase, Pred> ComponentBase_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::ComponentBase, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::ESMoL::TimingSheet> TimingSheet_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::TimingSheet>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::TimingSheet, Pred> TimingSheet_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::TimingSheet, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::ESMoL::Block> Block_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::Block>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::Block, Pred> Block_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::Block, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::ESMoL::Subsystem> Subsystem_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::Subsystem>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::Subsystem, Pred> Subsystem_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::Subsystem, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::ESMoL::HardwareUnit> HardwareUnit_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::HardwareUnit>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::HardwareUnit, Pred> HardwareUnit_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::HardwareUnit, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::ESMoL::HWElement> HWElement_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::HWElement>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::HWElement, Pred> HWElement_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::HWElement, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::ESMoL::Deployment> Deployment_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::Deployment>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::Deployment, Pred> Deployment_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::Deployment, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::ESMoL::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::MgaObject>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

			static ::Uml::CompositionParentRole meta_RootFolder_parent;
			Udm::ParentAttr< ::ESMoL::RootFolder> RootFolder_parent() const { return Udm::ParentAttr< ::ESMoL::RootFolder>(impl, meta_RootFolder_parent); }

			Udm::ParentAttr< ::ESMoL::RootFolder> parent() const { return Udm::ParentAttr< ::ESMoL::RootFolder>(impl, Udm::NULLPARENTROLE); }
		};

		class  Types :  public Udm::Object {
		public:
			static ::Uml::Class meta;

			Types() { }
			Types(Udm::ObjectImpl *impl) : UDM_OBJECT(impl) { }
			Types(const Types &master) : UDM_OBJECT(master) { }
			static Types Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static Types Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			Types CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			Types CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::ESMoL::Types> Instances() { return Udm::InstantiatedAttr< ::ESMoL::Types>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::ESMoL::Types, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::ESMoL::Types, Pred>(impl);}

			Udm::DerivedAttr< ::ESMoL::Types> Derived() { return Udm::DerivedAttr< ::ESMoL::Types>(impl);}
			template <class Pred> Udm::DerivedAttr< ::ESMoL::Types, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::ESMoL::Types, Pred>(impl);}

			Udm::ArchetypeAttr< ::ESMoL::Types> Archetype() { return Udm::ArchetypeAttr< ::ESMoL::Types>(impl);}

			static ::Uml::Attribute meta_name;
			Udm::StringAttr name() const { return Udm::StringAttr(impl, meta_name); }

			static ::Uml::CompositionChildRole meta_TypeBase_children;
			Udm::ChildrenAttr< ::ESMoL::TypeBase> TypeBase_children() const { return Udm::ChildrenAttr< ::ESMoL::TypeBase>(impl, meta_TypeBase_children); }
			template <class Pred> Udm::ChildrenAttr< ::ESMoL::TypeBase, Pred> TypeBase_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::TypeBase, Pred>(impl, meta_TypeBase_children); }

			Udm::ChildrenAttr< ::ESMoL::TypeStruct> TypeStruct_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::TypeStruct>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::TypeStruct, Pred> TypeStruct_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::TypeStruct, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::ESMoL::Matrix> Matrix_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::Matrix>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::Matrix, Pred> Matrix_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::Matrix, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::ESMoL::TypeBase> TypeBase_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::TypeBase>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::TypeBase, Pred> TypeBase_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::TypeBase, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::ESMoL::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::MgaObject>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

			static ::Uml::CompositionParentRole meta_RootFolder_parent;
			Udm::ParentAttr< ::ESMoL::RootFolder> RootFolder_parent() const { return Udm::ParentAttr< ::ESMoL::RootFolder>(impl, meta_RootFolder_parent); }

			Udm::ParentAttr< ::ESMoL::RootFolder> parent() const { return Udm::ParentAttr< ::ESMoL::RootFolder>(impl, Udm::NULLPARENTROLE); }
		};

		class  RequirementsLibrary :  public Udm::Object {
		public:
			static ::Uml::Class meta;

			RequirementsLibrary() { }
			RequirementsLibrary(Udm::ObjectImpl *impl) : UDM_OBJECT(impl) { }
			RequirementsLibrary(const RequirementsLibrary &master) : UDM_OBJECT(master) { }
			static RequirementsLibrary Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static RequirementsLibrary Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			RequirementsLibrary CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			RequirementsLibrary CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::ESMoL::RequirementsLibrary> Instances() { return Udm::InstantiatedAttr< ::ESMoL::RequirementsLibrary>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::ESMoL::RequirementsLibrary, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::ESMoL::RequirementsLibrary, Pred>(impl);}

			Udm::DerivedAttr< ::ESMoL::RequirementsLibrary> Derived() { return Udm::DerivedAttr< ::ESMoL::RequirementsLibrary>(impl);}
			template <class Pred> Udm::DerivedAttr< ::ESMoL::RequirementsLibrary, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::ESMoL::RequirementsLibrary, Pred>(impl);}

			Udm::ArchetypeAttr< ::ESMoL::RequirementsLibrary> Archetype() { return Udm::ArchetypeAttr< ::ESMoL::RequirementsLibrary>(impl);}

			static ::Uml::Attribute meta_name;
			Udm::StringAttr name() const { return Udm::StringAttr(impl, meta_name); }

			static ::Uml::CompositionChildRole meta_TimingSheet_children;
			Udm::ChildrenAttr< ::ESMoL::TimingSheet> TimingSheet_children() const { return Udm::ChildrenAttr< ::ESMoL::TimingSheet>(impl, meta_TimingSheet_children); }
			template <class Pred> Udm::ChildrenAttr< ::ESMoL::TimingSheet, Pred> TimingSheet_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::TimingSheet, Pred>(impl, meta_TimingSheet_children); }

			Udm::ChildrenAttr< ::ESMoL::TimingSheet> TimingSheet_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::TimingSheet>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::TimingSheet, Pred> TimingSheet_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::TimingSheet, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::ESMoL::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::MgaObject>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

			static ::Uml::CompositionParentRole meta_RootFolder_parent;
			Udm::ParentAttr< ::ESMoL::RootFolder> RootFolder_parent() const { return Udm::ParentAttr< ::ESMoL::RootFolder>(impl, meta_RootFolder_parent); }

			Udm::ParentAttr< ::ESMoL::RootFolder> parent() const { return Udm::ParentAttr< ::ESMoL::RootFolder>(impl, Udm::NULLPARENTROLE); }
		};

		class  Dataflow :  public Udm::Object {
		public:
			static ::Uml::Class meta;

			Dataflow() { }
			Dataflow(Udm::ObjectImpl *impl) : UDM_OBJECT(impl) { }
			Dataflow(const Dataflow &master) : UDM_OBJECT(master) { }
			static Dataflow Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static Dataflow Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			Dataflow CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			Dataflow CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::ESMoL::Dataflow> Instances() { return Udm::InstantiatedAttr< ::ESMoL::Dataflow>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::ESMoL::Dataflow, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::ESMoL::Dataflow, Pred>(impl);}

			Udm::DerivedAttr< ::ESMoL::Dataflow> Derived() { return Udm::DerivedAttr< ::ESMoL::Dataflow>(impl);}
			template <class Pred> Udm::DerivedAttr< ::ESMoL::Dataflow, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::ESMoL::Dataflow, Pred>(impl);}

			Udm::ArchetypeAttr< ::ESMoL::Dataflow> Archetype() { return Udm::ArchetypeAttr< ::ESMoL::Dataflow>(impl);}

			static ::Uml::Attribute meta_name;
			Udm::StringAttr name() const { return Udm::StringAttr(impl, meta_name); }

			static ::Uml::CompositionChildRole meta_Subsystem_children;
			Udm::ChildrenAttr< ::ESMoL::Subsystem> Subsystem_children() const { return Udm::ChildrenAttr< ::ESMoL::Subsystem>(impl, meta_Subsystem_children); }
			template <class Pred> Udm::ChildrenAttr< ::ESMoL::Subsystem, Pred> Subsystem_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::Subsystem, Pred>(impl, meta_Subsystem_children); }

			Udm::ChildrenAttr< ::ESMoL::ComponentBase> ComponentBase_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::ComponentBase>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::ComponentBase, Pred> ComponentBase_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::ComponentBase, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::ESMoL::Block> Block_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::Block>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::Block, Pred> Block_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::Block, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::ESMoL::Subsystem> Subsystem_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::Subsystem>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::Subsystem, Pred> Subsystem_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::Subsystem, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::ESMoL::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::MgaObject>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

			static ::Uml::CompositionParentRole meta_RootFolder_parent;
			Udm::ParentAttr< ::ESMoL::RootFolder> RootFolder_parent() const { return Udm::ParentAttr< ::ESMoL::RootFolder>(impl, meta_RootFolder_parent); }

			Udm::ParentAttr< ::ESMoL::RootFolder> parent() const { return Udm::ParentAttr< ::ESMoL::RootFolder>(impl, Udm::NULLPARENTROLE); }
		};

		class  Stateflow :  public Udm::Object {
		public:
			static ::Uml::Class meta;

			Stateflow() { }
			Stateflow(Udm::ObjectImpl *impl) : UDM_OBJECT(impl) { }
			Stateflow(const Stateflow &master) : UDM_OBJECT(master) { }
			static Stateflow Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static Stateflow Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			Stateflow CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			Stateflow CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::ESMoL::Stateflow> Instances() { return Udm::InstantiatedAttr< ::ESMoL::Stateflow>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::ESMoL::Stateflow, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::ESMoL::Stateflow, Pred>(impl);}

			Udm::DerivedAttr< ::ESMoL::Stateflow> Derived() { return Udm::DerivedAttr< ::ESMoL::Stateflow>(impl);}
			template <class Pred> Udm::DerivedAttr< ::ESMoL::Stateflow, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::ESMoL::Stateflow, Pred>(impl);}

			Udm::ArchetypeAttr< ::ESMoL::Stateflow> Archetype() { return Udm::ArchetypeAttr< ::ESMoL::Stateflow>(impl);}

			static ::Uml::Attribute meta_name;
			Udm::StringAttr name() const { return Udm::StringAttr(impl, meta_name); }

			static ::Uml::CompositionChildRole meta_State_children;
			Udm::ChildrenAttr< ::ESMoL::State> State_children() const { return Udm::ChildrenAttr< ::ESMoL::State>(impl, meta_State_children); }
			template <class Pred> Udm::ChildrenAttr< ::ESMoL::State, Pred> State_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::State, Pred>(impl, meta_State_children); }

			Udm::ChildrenAttr< ::ESMoL::State> State_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::State>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::State, Pred> State_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::State, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::ESMoL::TransConnector> TransConnector_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::TransConnector>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::TransConnector, Pred> TransConnector_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::TransConnector, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::ESMoL::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::MgaObject>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

			static ::Uml::CompositionParentRole meta_RootFolder_parent;
			Udm::ParentAttr< ::ESMoL::RootFolder> RootFolder_parent() const { return Udm::ParentAttr< ::ESMoL::RootFolder>(impl, meta_RootFolder_parent); }

			Udm::ParentAttr< ::ESMoL::RootFolder> parent() const { return Udm::ParentAttr< ::ESMoL::RootFolder>(impl, Udm::NULLPARENTROLE); }
		};

		class  PlatformLibrary :  public Udm::Object {
		public:
			static ::Uml::Class meta;

			PlatformLibrary() { }
			PlatformLibrary(Udm::ObjectImpl *impl) : UDM_OBJECT(impl) { }
			PlatformLibrary(const PlatformLibrary &master) : UDM_OBJECT(master) { }
			static PlatformLibrary Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static PlatformLibrary Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			PlatformLibrary CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			PlatformLibrary CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::ESMoL::PlatformLibrary> Instances() { return Udm::InstantiatedAttr< ::ESMoL::PlatformLibrary>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::ESMoL::PlatformLibrary, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::ESMoL::PlatformLibrary, Pred>(impl);}

			Udm::DerivedAttr< ::ESMoL::PlatformLibrary> Derived() { return Udm::DerivedAttr< ::ESMoL::PlatformLibrary>(impl);}
			template <class Pred> Udm::DerivedAttr< ::ESMoL::PlatformLibrary, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::ESMoL::PlatformLibrary, Pred>(impl);}

			Udm::ArchetypeAttr< ::ESMoL::PlatformLibrary> Archetype() { return Udm::ArchetypeAttr< ::ESMoL::PlatformLibrary>(impl);}

			static ::Uml::Attribute meta_name;
			Udm::StringAttr name() const { return Udm::StringAttr(impl, meta_name); }

			static ::Uml::CompositionChildRole meta_HardwareUnit_children;
			Udm::ChildrenAttr< ::ESMoL::HardwareUnit> HardwareUnit_children() const { return Udm::ChildrenAttr< ::ESMoL::HardwareUnit>(impl, meta_HardwareUnit_children); }
			template <class Pred> Udm::ChildrenAttr< ::ESMoL::HardwareUnit, Pred> HardwareUnit_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::HardwareUnit, Pred>(impl, meta_HardwareUnit_children); }

			Udm::ChildrenAttr< ::ESMoL::HardwareUnit> HardwareUnit_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::HardwareUnit>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::HardwareUnit, Pred> HardwareUnit_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::HardwareUnit, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::ESMoL::HWElement> HWElement_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::HWElement>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::HWElement, Pred> HWElement_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::HWElement, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::ESMoL::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::MgaObject>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

			static ::Uml::CompositionParentRole meta_RootFolder_parent;
			Udm::ParentAttr< ::ESMoL::RootFolder> RootFolder_parent() const { return Udm::ParentAttr< ::ESMoL::RootFolder>(impl, meta_RootFolder_parent); }

			Udm::ParentAttr< ::ESMoL::RootFolder> parent() const { return Udm::ParentAttr< ::ESMoL::RootFolder>(impl, Udm::NULLPARENTROLE); }
		};

		class  DeploymentLibrary :  public Udm::Object {
		public:
			static ::Uml::Class meta;

			DeploymentLibrary() { }
			DeploymentLibrary(Udm::ObjectImpl *impl) : UDM_OBJECT(impl) { }
			DeploymentLibrary(const DeploymentLibrary &master) : UDM_OBJECT(master) { }
			static DeploymentLibrary Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static DeploymentLibrary Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			DeploymentLibrary CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			DeploymentLibrary CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::ESMoL::DeploymentLibrary> Instances() { return Udm::InstantiatedAttr< ::ESMoL::DeploymentLibrary>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::ESMoL::DeploymentLibrary, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::ESMoL::DeploymentLibrary, Pred>(impl);}

			Udm::DerivedAttr< ::ESMoL::DeploymentLibrary> Derived() { return Udm::DerivedAttr< ::ESMoL::DeploymentLibrary>(impl);}
			template <class Pred> Udm::DerivedAttr< ::ESMoL::DeploymentLibrary, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::ESMoL::DeploymentLibrary, Pred>(impl);}

			Udm::ArchetypeAttr< ::ESMoL::DeploymentLibrary> Archetype() { return Udm::ArchetypeAttr< ::ESMoL::DeploymentLibrary>(impl);}

			static ::Uml::Attribute meta_name;
			Udm::StringAttr name() const { return Udm::StringAttr(impl, meta_name); }

			static ::Uml::CompositionChildRole meta_Deployment_children;
			Udm::ChildrenAttr< ::ESMoL::Deployment> Deployment_children() const { return Udm::ChildrenAttr< ::ESMoL::Deployment>(impl, meta_Deployment_children); }
			template <class Pred> Udm::ChildrenAttr< ::ESMoL::Deployment, Pred> Deployment_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::Deployment, Pred>(impl, meta_Deployment_children); }

			Udm::ChildrenAttr< ::ESMoL::Deployment> Deployment_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::Deployment>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::Deployment, Pred> Deployment_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::Deployment, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::ESMoL::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::MgaObject>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

			static ::Uml::CompositionParentRole meta_RootFolder_parent;
			Udm::ParentAttr< ::ESMoL::RootFolder> RootFolder_parent() const { return Udm::ParentAttr< ::ESMoL::RootFolder>(impl, meta_RootFolder_parent); }

			Udm::ParentAttr< ::ESMoL::RootFolder> parent() const { return Udm::ParentAttr< ::ESMoL::RootFolder>(impl, Udm::NULLPARENTROLE); }
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

			Udm::InstantiatedAttr< ::ESMoL::RootFolder> Instances() { return Udm::InstantiatedAttr< ::ESMoL::RootFolder>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::ESMoL::RootFolder, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::ESMoL::RootFolder, Pred>(impl);}

			Udm::DerivedAttr< ::ESMoL::RootFolder> Derived() { return Udm::DerivedAttr< ::ESMoL::RootFolder>(impl);}
			template <class Pred> Udm::DerivedAttr< ::ESMoL::RootFolder, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::ESMoL::RootFolder, Pred>(impl);}

			Udm::ArchetypeAttr< ::ESMoL::RootFolder> Archetype() { return Udm::ArchetypeAttr< ::ESMoL::RootFolder>(impl);}

			static ::Uml::Attribute meta_name;
			Udm::StringAttr name() const { return Udm::StringAttr(impl, meta_name); }

			static ::Uml::CompositionChildRole meta_DesignFolder_children;
			Udm::ChildrenAttr< ::ESMoL::DesignFolder> DesignFolder_children() const { return Udm::ChildrenAttr< ::ESMoL::DesignFolder>(impl, meta_DesignFolder_children); }
			template <class Pred> Udm::ChildrenAttr< ::ESMoL::DesignFolder, Pred> DesignFolder_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::DesignFolder, Pred>(impl, meta_DesignFolder_children); }

			static ::Uml::CompositionChildRole meta_ArchitectureLibrary_children;
			Udm::ChildrenAttr< ::ESMoL::ArchitectureLibrary> ArchitectureLibrary_children() const { return Udm::ChildrenAttr< ::ESMoL::ArchitectureLibrary>(impl, meta_ArchitectureLibrary_children); }
			template <class Pred> Udm::ChildrenAttr< ::ESMoL::ArchitectureLibrary, Pred> ArchitectureLibrary_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::ArchitectureLibrary, Pred>(impl, meta_ArchitectureLibrary_children); }

			static ::Uml::CompositionChildRole meta_Types_children;
			Udm::ChildrenAttr< ::ESMoL::Types> Types_children() const { return Udm::ChildrenAttr< ::ESMoL::Types>(impl, meta_Types_children); }
			template <class Pred> Udm::ChildrenAttr< ::ESMoL::Types, Pred> Types_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::Types, Pred>(impl, meta_Types_children); }

			static ::Uml::CompositionChildRole meta_RequirementsLibrary_children;
			Udm::ChildrenAttr< ::ESMoL::RequirementsLibrary> RequirementsLibrary_children() const { return Udm::ChildrenAttr< ::ESMoL::RequirementsLibrary>(impl, meta_RequirementsLibrary_children); }
			template <class Pred> Udm::ChildrenAttr< ::ESMoL::RequirementsLibrary, Pred> RequirementsLibrary_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::RequirementsLibrary, Pred>(impl, meta_RequirementsLibrary_children); }

			static ::Uml::CompositionChildRole meta_Dataflow_children;
			Udm::ChildrenAttr< ::ESMoL::Dataflow> Dataflow_children() const { return Udm::ChildrenAttr< ::ESMoL::Dataflow>(impl, meta_Dataflow_children); }
			template <class Pred> Udm::ChildrenAttr< ::ESMoL::Dataflow, Pred> Dataflow_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::Dataflow, Pred>(impl, meta_Dataflow_children); }

			static ::Uml::CompositionChildRole meta_Stateflow_children;
			Udm::ChildrenAttr< ::ESMoL::Stateflow> Stateflow_children() const { return Udm::ChildrenAttr< ::ESMoL::Stateflow>(impl, meta_Stateflow_children); }
			template <class Pred> Udm::ChildrenAttr< ::ESMoL::Stateflow, Pred> Stateflow_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::Stateflow, Pred>(impl, meta_Stateflow_children); }

			static ::Uml::CompositionChildRole meta_PlatformLibrary_children;
			Udm::ChildrenAttr< ::ESMoL::PlatformLibrary> PlatformLibrary_children() const { return Udm::ChildrenAttr< ::ESMoL::PlatformLibrary>(impl, meta_PlatformLibrary_children); }
			template <class Pred> Udm::ChildrenAttr< ::ESMoL::PlatformLibrary, Pred> PlatformLibrary_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::PlatformLibrary, Pred>(impl, meta_PlatformLibrary_children); }

			static ::Uml::CompositionChildRole meta_DeploymentLibrary_children;
			Udm::ChildrenAttr< ::ESMoL::DeploymentLibrary> DeploymentLibrary_children() const { return Udm::ChildrenAttr< ::ESMoL::DeploymentLibrary>(impl, meta_DeploymentLibrary_children); }
			template <class Pred> Udm::ChildrenAttr< ::ESMoL::DeploymentLibrary, Pred> DeploymentLibrary_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::DeploymentLibrary, Pred>(impl, meta_DeploymentLibrary_children); }

			static ::Uml::CompositionChildRole meta_RootFolder_children;
			Udm::ChildrenAttr< ::ESMoL::RootFolder> RootFolder_children() const { return Udm::ChildrenAttr< ::ESMoL::RootFolder>(impl, meta_RootFolder_children); }
			template <class Pred> Udm::ChildrenAttr< ::ESMoL::RootFolder, Pred> RootFolder_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::RootFolder, Pred>(impl, meta_RootFolder_children); }

			Udm::ChildrenAttr< ::ESMoL::ArchitectureLibrary> ArchitectureLibrary_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::ArchitectureLibrary>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::ArchitectureLibrary, Pred> ArchitectureLibrary_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::ArchitectureLibrary, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::ESMoL::DesignFolder> DesignFolder_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::DesignFolder>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::DesignFolder, Pred> DesignFolder_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::DesignFolder, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::ESMoL::Types> Types_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::Types>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::Types, Pred> Types_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::Types, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::ESMoL::RequirementsLibrary> RequirementsLibrary_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::RequirementsLibrary>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::RequirementsLibrary, Pred> RequirementsLibrary_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::RequirementsLibrary, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::ESMoL::Dataflow> Dataflow_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::Dataflow>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::Dataflow, Pred> Dataflow_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::Dataflow, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::ESMoL::Stateflow> Stateflow_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::Stateflow>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::Stateflow, Pred> Stateflow_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::Stateflow, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::ESMoL::PlatformLibrary> PlatformLibrary_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::PlatformLibrary>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::PlatformLibrary, Pred> PlatformLibrary_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::PlatformLibrary, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::ESMoL::DeploymentLibrary> DeploymentLibrary_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::DeploymentLibrary>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::DeploymentLibrary, Pred> DeploymentLibrary_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::DeploymentLibrary, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::ESMoL::RootFolder> RootFolder_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::RootFolder>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::RootFolder, Pred> RootFolder_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::RootFolder, Pred>(impl, Udm::NULLCHILDROLE); }

			static ::Uml::CompositionParentRole meta_RootFolder_parent;
			Udm::ParentAttr< ::ESMoL::RootFolder> RootFolder_parent() const { return Udm::ParentAttr< ::ESMoL::RootFolder>(impl, meta_RootFolder_parent); }

			Udm::ParentAttr< ::ESMoL::RootFolder> parent() const { return Udm::ParentAttr< ::ESMoL::RootFolder>(impl, Udm::NULLPARENTROLE); }
		};

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

			Udm::InstantiatedAttr< ::ESMoL::MgaObject> Instances() { return Udm::InstantiatedAttr< ::ESMoL::MgaObject>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::ESMoL::MgaObject, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::ESMoL::MgaObject, Pred>(impl);}

			Udm::DerivedAttr< ::ESMoL::MgaObject> Derived() { return Udm::DerivedAttr< ::ESMoL::MgaObject>(impl);}
			template <class Pred> Udm::DerivedAttr< ::ESMoL::MgaObject, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::ESMoL::MgaObject, Pred>(impl);}

			Udm::ArchetypeAttr< ::ESMoL::MgaObject> Archetype() { return Udm::ArchetypeAttr< ::ESMoL::MgaObject>(impl);}

			static ::Uml::Attribute meta_position;
			Udm::StringAttr position() const { return Udm::StringAttr(impl, meta_position); }

			static ::Uml::Attribute meta_name;
			Udm::StringAttr name() const { return Udm::StringAttr(impl, meta_name); }

			Udm::ParentAttr<Udm::Object> parent() const { return Udm::ParentAttr<Udm::Object>(impl, Udm::NULLPARENTROLE); }
		};

		class  IOPortExp :  virtual public ::ESMoL::MgaObject {
		public:
			static ::Uml::Class meta;

			IOPortExp() { }
			IOPortExp(Udm::ObjectImpl *impl) : ::ESMoL::MgaObject(impl) { }
			IOPortExp(const IOPortExp &master) : ::ESMoL::MgaObject(master) { }
			static IOPortExp Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static IOPortExp Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			IOPortExp CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			IOPortExp CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::ESMoL::IOPortExp> Instances() { return Udm::InstantiatedAttr< ::ESMoL::IOPortExp>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::ESMoL::IOPortExp, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::ESMoL::IOPortExp, Pred>(impl);}

			Udm::DerivedAttr< ::ESMoL::IOPortExp> Derived() { return Udm::DerivedAttr< ::ESMoL::IOPortExp>(impl);}
			template <class Pred> Udm::DerivedAttr< ::ESMoL::IOPortExp, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::ESMoL::IOPortExp, Pred>(impl);}

			Udm::ArchetypeAttr< ::ESMoL::IOPortExp> Archetype() { return Udm::ArchetypeAttr< ::ESMoL::IOPortExp>(impl);}

			static ::Uml::AssociationRole meta_srcIOPortAssignment;
			static ::Uml::AssociationRole meta_srcIOPortAssignment_rev;
			Udm::AClassAssocAttr< ::ESMoL::IOPortAssignment, ::ESMoL::IOPortInfoBase> srcIOPortAssignment() const { return Udm::AClassAssocAttr< ::ESMoL::IOPortAssignment, ::ESMoL::IOPortInfoBase>(impl, meta_srcIOPortAssignment, meta_srcIOPortAssignment_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::ESMoL::IOPortAssignment, ::ESMoL::IOPortInfoBase, Pred> srcIOPortAssignment_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::ESMoL::IOPortAssignment, ::ESMoL::IOPortInfoBase, Pred>(impl, meta_srcIOPortAssignment, meta_srcIOPortAssignment_rev); }

			Udm::ParentAttr<Udm::Object> parent() const { return Udm::ParentAttr<Udm::Object>(impl, Udm::NULLPARENTROLE); }
		};

		class  IOPortAssignment :  public ::ESMoL::MgaObject {
		public:
			static ::Uml::Class meta;

			IOPortAssignment() { }
			IOPortAssignment(Udm::ObjectImpl *impl) : ::ESMoL::MgaObject(impl) { }
			IOPortAssignment(const IOPortAssignment &master) : ::ESMoL::MgaObject(master) { }
			static IOPortAssignment Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static IOPortAssignment Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			IOPortAssignment CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			IOPortAssignment CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::ESMoL::IOPortAssignment> Instances() { return Udm::InstantiatedAttr< ::ESMoL::IOPortAssignment>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::ESMoL::IOPortAssignment, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::ESMoL::IOPortAssignment, Pred>(impl);}

			Udm::DerivedAttr< ::ESMoL::IOPortAssignment> Derived() { return Udm::DerivedAttr< ::ESMoL::IOPortAssignment>(impl);}
			template <class Pred> Udm::DerivedAttr< ::ESMoL::IOPortAssignment, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::ESMoL::IOPortAssignment, Pred>(impl);}

			Udm::ArchetypeAttr< ::ESMoL::IOPortAssignment> Archetype() { return Udm::ArchetypeAttr< ::ESMoL::IOPortAssignment>(impl);}

			Udm::ParentAttr<Udm::Object> parent() const { return Udm::ParentAttr<Udm::Object>(impl, Udm::NULLPARENTROLE); }
			static ::Uml::AssociationRole meta_dstIOPortAssignment_end_;
			Udm::AssocEndAttr< ::ESMoL::IOPortExp> dstIOPortAssignment_end() const { return Udm::AssocEndAttr< ::ESMoL::IOPortExp>(impl, meta_dstIOPortAssignment_end_); }

			static ::Uml::AssociationRole meta_srcIOPortAssignment_end_;
			Udm::AssocEndAttr< ::ESMoL::IOPortInfoBase> srcIOPortAssignment_end() const { return Udm::AssocEndAttr< ::ESMoL::IOPortInfoBase>(impl, meta_srcIOPortAssignment_end_); }

		};

		class  IOPortInfoBase :  public ::ESMoL::MgaObject {
		public:
			static ::Uml::Class meta;

			IOPortInfoBase() { }
			IOPortInfoBase(Udm::ObjectImpl *impl) : ::ESMoL::MgaObject(impl) { }
			IOPortInfoBase(const IOPortInfoBase &master) : ::ESMoL::MgaObject(master) { }
			static IOPortInfoBase Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static IOPortInfoBase Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			IOPortInfoBase CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			IOPortInfoBase CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::ESMoL::IOPortInfoBase> Instances() { return Udm::InstantiatedAttr< ::ESMoL::IOPortInfoBase>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::ESMoL::IOPortInfoBase, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::ESMoL::IOPortInfoBase, Pred>(impl);}

			Udm::DerivedAttr< ::ESMoL::IOPortInfoBase> Derived() { return Udm::DerivedAttr< ::ESMoL::IOPortInfoBase>(impl);}
			template <class Pred> Udm::DerivedAttr< ::ESMoL::IOPortInfoBase, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::ESMoL::IOPortInfoBase, Pred>(impl);}

			Udm::ArchetypeAttr< ::ESMoL::IOPortInfoBase> Archetype() { return Udm::ArchetypeAttr< ::ESMoL::IOPortInfoBase>(impl);}

			static ::Uml::AssociationRole meta_dstIOPortAssignment;
			static ::Uml::AssociationRole meta_dstIOPortAssignment_rev;
			Udm::AClassAssocAttr< ::ESMoL::IOPortAssignment, ::ESMoL::IOPortExp> dstIOPortAssignment() const { return Udm::AClassAssocAttr< ::ESMoL::IOPortAssignment, ::ESMoL::IOPortExp>(impl, meta_dstIOPortAssignment, meta_dstIOPortAssignment_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::ESMoL::IOPortAssignment, ::ESMoL::IOPortExp, Pred> dstIOPortAssignment_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::ESMoL::IOPortAssignment, ::ESMoL::IOPortExp, Pred>(impl, meta_dstIOPortAssignment, meta_dstIOPortAssignment_rev); }

			Udm::ParentAttr<Udm::Object> parent() const { return Udm::ParentAttr<Udm::Object>(impl, Udm::NULLPARENTROLE); }
		};

		class  IOPortInfo :  public ::ESMoL::IOPortInfoBase {
		public:
			static ::Uml::Class meta;

			IOPortInfo() { }
			IOPortInfo(Udm::ObjectImpl *impl) : ::ESMoL::IOPortInfoBase(impl) { }
			IOPortInfo(const IOPortInfo &master) : ::ESMoL::IOPortInfoBase(master) { }
			static IOPortInfo Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static IOPortInfo Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			IOPortInfo CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			IOPortInfo CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::ESMoL::IOPortInfo> Instances() { return Udm::InstantiatedAttr< ::ESMoL::IOPortInfo>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::ESMoL::IOPortInfo, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::ESMoL::IOPortInfo, Pred>(impl);}

			Udm::DerivedAttr< ::ESMoL::IOPortInfo> Derived() { return Udm::DerivedAttr< ::ESMoL::IOPortInfo>(impl);}
			template <class Pred> Udm::DerivedAttr< ::ESMoL::IOPortInfo, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::ESMoL::IOPortInfo, Pred>(impl);}

			Udm::ArchetypeAttr< ::ESMoL::IOPortInfo> Archetype() { return Udm::ArchetypeAttr< ::ESMoL::IOPortInfo>(impl);}

			static ::Uml::Attribute meta_DataInit;
			Udm::StringAttr DataInit() const { return Udm::StringAttr(impl, meta_DataInit); }

			static ::Uml::Attribute meta_DataSize;
			Udm::StringAttr DataSize() const { return Udm::StringAttr(impl, meta_DataSize); }

			static ::Uml::Attribute meta_DataMin;
			Udm::StringAttr DataMin() const { return Udm::StringAttr(impl, meta_DataMin); }

			static ::Uml::Attribute meta_DataMax;
			Udm::StringAttr DataMax() const { return Udm::StringAttr(impl, meta_DataMax); }

			static ::Uml::Attribute meta_Scale;
			Udm::StringAttr Scale() const { return Udm::StringAttr(impl, meta_Scale); }

			static ::Uml::Attribute meta_Offset;
			Udm::StringAttr Offset() const { return Udm::StringAttr(impl, meta_Offset); }

			static ::Uml::AssociationRole meta_referedbyIOPortInfoRef;
			Udm::AssocAttr< ::ESMoL::IOPortInfoRef> referedbyIOPortInfoRef() const { return Udm::AssocAttr< ::ESMoL::IOPortInfoRef>(impl, meta_referedbyIOPortInfoRef); }
			template <class Pred> Udm::AssocAttr< ::ESMoL::IOPortInfoRef, Pred > referedbyIOPortInfoRef_sorted(const Pred &) const { return Udm::AssocAttr< ::ESMoL::IOPortInfoRef, Pred>(impl, meta_referedbyIOPortInfoRef); }

			Udm::ParentAttr<Udm::Object> parent() const { return Udm::ParentAttr<Udm::Object>(impl, Udm::NULLPARENTROLE); }
		};

		class  IOPortInfoRef :  public ::ESMoL::IOPortInfoBase {
		public:
			static ::Uml::Class meta;

			IOPortInfoRef() { }
			IOPortInfoRef(Udm::ObjectImpl *impl) : ::ESMoL::IOPortInfoBase(impl) { }
			IOPortInfoRef(const IOPortInfoRef &master) : ::ESMoL::IOPortInfoBase(master) { }
			static IOPortInfoRef Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static IOPortInfoRef Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			IOPortInfoRef CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			IOPortInfoRef CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::ESMoL::IOPortInfoRef> Instances() { return Udm::InstantiatedAttr< ::ESMoL::IOPortInfoRef>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::ESMoL::IOPortInfoRef, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::ESMoL::IOPortInfoRef, Pred>(impl);}

			Udm::DerivedAttr< ::ESMoL::IOPortInfoRef> Derived() { return Udm::DerivedAttr< ::ESMoL::IOPortInfoRef>(impl);}
			template <class Pred> Udm::DerivedAttr< ::ESMoL::IOPortInfoRef, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::ESMoL::IOPortInfoRef, Pred>(impl);}

			Udm::ArchetypeAttr< ::ESMoL::IOPortInfoRef> Archetype() { return Udm::ArchetypeAttr< ::ESMoL::IOPortInfoRef>(impl);}

			static ::Uml::AssociationRole meta_ref;
			Udm::PointerAttr< ::ESMoL::IOPortInfo> ref() const { return Udm::PointerAttr< ::ESMoL::IOPortInfo>(impl, meta_ref); }

			Udm::ParentAttr<Udm::Object> parent() const { return Udm::ParentAttr<Udm::Object>(impl, Udm::NULLPARENTROLE); }
		};

		class  System :  public ::ESMoL::MgaObject {
		public:
			static ::Uml::Class meta;

			System() { }
			System(Udm::ObjectImpl *impl) : ::ESMoL::MgaObject(impl) { }
			System(const System &master) : ::ESMoL::MgaObject(master) { }
			static System Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static System Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			System CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			System CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::ESMoL::System> Instances() { return Udm::InstantiatedAttr< ::ESMoL::System>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::ESMoL::System, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::ESMoL::System, Pred>(impl);}

			Udm::DerivedAttr< ::ESMoL::System> Derived() { return Udm::DerivedAttr< ::ESMoL::System>(impl);}
			template <class Pred> Udm::DerivedAttr< ::ESMoL::System, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::ESMoL::System, Pred>(impl);}

			Udm::ArchetypeAttr< ::ESMoL::System> Archetype() { return Udm::ArchetypeAttr< ::ESMoL::System>(impl);}

			static ::Uml::CompositionChildRole meta_ComponentBase_children;
			Udm::ChildrenAttr< ::ESMoL::ComponentBase> ComponentBase_children() const { return Udm::ChildrenAttr< ::ESMoL::ComponentBase>(impl, meta_ComponentBase_children); }
			template <class Pred> Udm::ChildrenAttr< ::ESMoL::ComponentBase, Pred> ComponentBase_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::ComponentBase, Pred>(impl, meta_ComponentBase_children); }

			static ::Uml::CompositionChildRole meta_Connector_children;
			Udm::ChildrenAttr< ::ESMoL::Connector> Connector_children() const { return Udm::ChildrenAttr< ::ESMoL::Connector>(impl, meta_Connector_children); }
			template <class Pred> Udm::ChildrenAttr< ::ESMoL::Connector, Pred> Connector_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::Connector, Pred>(impl, meta_Connector_children); }

			Udm::ChildrenAttr< ::ESMoL::CCode> CCode_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::CCode>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::CCode, Pred> CCode_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::CCode, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::ESMoL::SubsystemRef> SubsystemRef_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::SubsystemRef>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::SubsystemRef, Pred> SubsystemRef_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::SubsystemRef, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::ESMoL::Connector> Connector_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::Connector>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::Connector, Pred> Connector_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::Connector, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::ESMoL::ComponentBase> ComponentBase_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::ComponentBase>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::ComponentBase, Pred> ComponentBase_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::ComponentBase, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::ESMoL::Subsystem> Subsystem_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::Subsystem>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::Subsystem, Pred> Subsystem_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::Subsystem, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::ESMoL::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::MgaObject>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

			static ::Uml::CompositionParentRole meta_ArchitectureLibrary_parent;
			Udm::ParentAttr< ::ESMoL::ArchitectureLibrary> ArchitectureLibrary_parent() const { return Udm::ParentAttr< ::ESMoL::ArchitectureLibrary>(impl, meta_ArchitectureLibrary_parent); }

			static ::Uml::CompositionParentRole meta_DesignFolder_parent;
			Udm::ParentAttr< ::ESMoL::DesignFolder> DesignFolder_parent() const { return Udm::ParentAttr< ::ESMoL::DesignFolder>(impl, meta_DesignFolder_parent); }

			Udm::ParentAttr<Udm::Object> parent() const { return Udm::ParentAttr<Udm::Object>(impl, Udm::NULLPARENTROLE); }
		};

		class  Connector :  public ::ESMoL::MgaObject {
		public:
			static ::Uml::Class meta;

			Connector() { }
			Connector(Udm::ObjectImpl *impl) : ::ESMoL::MgaObject(impl) { }
			Connector(const Connector &master) : ::ESMoL::MgaObject(master) { }
			static Connector Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static Connector Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			Connector CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			Connector CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::ESMoL::Connector> Instances() { return Udm::InstantiatedAttr< ::ESMoL::Connector>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::ESMoL::Connector, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::ESMoL::Connector, Pred>(impl);}

			Udm::DerivedAttr< ::ESMoL::Connector> Derived() { return Udm::DerivedAttr< ::ESMoL::Connector>(impl);}
			template <class Pred> Udm::DerivedAttr< ::ESMoL::Connector, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::ESMoL::Connector, Pred>(impl);}

			Udm::ArchetypeAttr< ::ESMoL::Connector> Archetype() { return Udm::ArchetypeAttr< ::ESMoL::Connector>(impl);}

			static ::Uml::Attribute meta_ConnectionType;
			Udm::StringAttr ConnectionType() const { return Udm::StringAttr(impl, meta_ConnectionType); }

			static ::Uml::CompositionParentRole meta_System_parent;
			Udm::ParentAttr< ::ESMoL::System> System_parent() const { return Udm::ParentAttr< ::ESMoL::System>(impl, meta_System_parent); }

			static ::Uml::CompositionParentRole meta_Task_parent;
			Udm::ParentAttr< ::ESMoL::Task> Task_parent() const { return Udm::ParentAttr< ::ESMoL::Task>(impl, meta_Task_parent); }

			Udm::ParentAttr< ::ESMoL::MgaObject> parent() const { return Udm::ParentAttr< ::ESMoL::MgaObject>(impl, Udm::NULLPARENTROLE); }
			static ::Uml::AssociationRole meta_srcConnector_end_;
			Udm::AssocEndAttr< ::ESMoL::Output> srcConnector_end() const { return Udm::AssocEndAttr< ::ESMoL::Output>(impl, meta_srcConnector_end_); }

			static ::Uml::AssociationRole meta_dstConnector_end_;
			Udm::AssocEndAttr< ::ESMoL::Input> dstConnector_end() const { return Udm::AssocEndAttr< ::ESMoL::Input>(impl, meta_dstConnector_end_); }

		};

		class  ComponentBase :  virtual public ::ESMoL::MgaObject {
		public:
			static ::Uml::Class meta;

			ComponentBase() { }
			ComponentBase(Udm::ObjectImpl *impl) : ::ESMoL::MgaObject(impl) { }
			ComponentBase(const ComponentBase &master) : ::ESMoL::MgaObject(master) { }
			static ComponentBase Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static ComponentBase Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			ComponentBase CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			ComponentBase CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::ESMoL::ComponentBase> Instances() { return Udm::InstantiatedAttr< ::ESMoL::ComponentBase>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::ESMoL::ComponentBase, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::ESMoL::ComponentBase, Pred>(impl);}

			Udm::DerivedAttr< ::ESMoL::ComponentBase> Derived() { return Udm::DerivedAttr< ::ESMoL::ComponentBase>(impl);}
			template <class Pred> Udm::DerivedAttr< ::ESMoL::ComponentBase, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::ESMoL::ComponentBase, Pred>(impl);}

			Udm::ArchetypeAttr< ::ESMoL::ComponentBase> Archetype() { return Udm::ArchetypeAttr< ::ESMoL::ComponentBase>(impl);}

			static ::Uml::Attribute meta_Deadline;
			Udm::IntegerAttr Deadline() const { return Udm::IntegerAttr(impl, meta_Deadline); }

			static ::Uml::Attribute meta_Period;
			Udm::IntegerAttr Period() const { return Udm::IntegerAttr(impl, meta_Period); }

			static ::Uml::AssociationRole meta_referedbySystemRef;
			Udm::AssocAttr< ::ESMoL::Component> referedbySystemRef() const { return Udm::AssocAttr< ::ESMoL::Component>(impl, meta_referedbySystemRef); }
			template <class Pred> Udm::AssocAttr< ::ESMoL::Component, Pred > referedbySystemRef_sorted(const Pred &) const { return Udm::AssocAttr< ::ESMoL::Component, Pred>(impl, meta_referedbySystemRef); }

			static ::Uml::CompositionParentRole meta_System_parent;
			Udm::ParentAttr< ::ESMoL::System> System_parent() const { return Udm::ParentAttr< ::ESMoL::System>(impl, meta_System_parent); }

			Udm::ParentAttr< ::ESMoL::MgaObject> parent() const { return Udm::ParentAttr< ::ESMoL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  CCode :  public ::ESMoL::ComponentBase {
		public:
			static ::Uml::Class meta;

			CCode() { }
			CCode(Udm::ObjectImpl *impl) : ::ESMoL::ComponentBase(impl), ::ESMoL::MgaObject(impl) { }
			CCode(const CCode &master) : ::ESMoL::ComponentBase(master), ::ESMoL::MgaObject(master) { }
			static CCode Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static CCode Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			CCode CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			CCode CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::ESMoL::CCode> Instances() { return Udm::InstantiatedAttr< ::ESMoL::CCode>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::ESMoL::CCode, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::ESMoL::CCode, Pred>(impl);}

			Udm::DerivedAttr< ::ESMoL::CCode> Derived() { return Udm::DerivedAttr< ::ESMoL::CCode>(impl);}
			template <class Pred> Udm::DerivedAttr< ::ESMoL::CCode, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::ESMoL::CCode, Pred>(impl);}

			Udm::ArchetypeAttr< ::ESMoL::CCode> Archetype() { return Udm::ArchetypeAttr< ::ESMoL::CCode>(impl);}

			static ::Uml::CompositionChildRole meta_CInputPort_children;
			Udm::ChildrenAttr< ::ESMoL::CInputPort> CInputPort_children() const { return Udm::ChildrenAttr< ::ESMoL::CInputPort>(impl, meta_CInputPort_children); }
			template <class Pred> Udm::ChildrenAttr< ::ESMoL::CInputPort, Pred> CInputPort_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::CInputPort, Pred>(impl, meta_CInputPort_children); }

			static ::Uml::CompositionChildRole meta_COutputPort_children;
			Udm::ChildrenAttr< ::ESMoL::COutputPort> COutputPort_children() const { return Udm::ChildrenAttr< ::ESMoL::COutputPort>(impl, meta_COutputPort_children); }
			template <class Pred> Udm::ChildrenAttr< ::ESMoL::COutputPort, Pred> COutputPort_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::COutputPort, Pred>(impl, meta_COutputPort_children); }

			Udm::ChildrenAttr< ::ESMoL::COutputPort> COutputPort_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::COutputPort>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::COutputPort, Pred> COutputPort_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::COutputPort, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::ESMoL::CInputPort> CInputPort_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::CInputPort>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::CInputPort, Pred> CInputPort_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::CInputPort, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::ESMoL::IOPortExp> IOPortExp_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::IOPortExp>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::IOPortExp, Pred> IOPortExp_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::IOPortExp, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::ESMoL::Output> Output_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::Output>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::Output, Pred> Output_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::Output, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::ESMoL::Input> Input_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::Input>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::Input, Pred> Input_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::Input, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::ESMoL::IOPort> IOPort_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::IOPort>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::IOPort, Pred> IOPort_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::IOPort, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::ESMoL::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::MgaObject>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ParentAttr< ::ESMoL::MgaObject> parent() const { return Udm::ParentAttr< ::ESMoL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  SubsystemRef :  public ::ESMoL::ComponentBase {
		public:
			static ::Uml::Class meta;

			SubsystemRef() { }
			SubsystemRef(Udm::ObjectImpl *impl) : ::ESMoL::ComponentBase(impl), ::ESMoL::MgaObject(impl) { }
			SubsystemRef(const SubsystemRef &master) : ::ESMoL::ComponentBase(master), ::ESMoL::MgaObject(master) { }
			static SubsystemRef Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static SubsystemRef Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			SubsystemRef CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			SubsystemRef CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::ESMoL::SubsystemRef> Instances() { return Udm::InstantiatedAttr< ::ESMoL::SubsystemRef>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::ESMoL::SubsystemRef, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::ESMoL::SubsystemRef, Pred>(impl);}

			Udm::DerivedAttr< ::ESMoL::SubsystemRef> Derived() { return Udm::DerivedAttr< ::ESMoL::SubsystemRef>(impl);}
			template <class Pred> Udm::DerivedAttr< ::ESMoL::SubsystemRef, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::ESMoL::SubsystemRef, Pred>(impl);}

			Udm::ArchetypeAttr< ::ESMoL::SubsystemRef> Archetype() { return Udm::ArchetypeAttr< ::ESMoL::SubsystemRef>(impl);}

			static ::Uml::AssociationRole meta_ref;
			Udm::PointerAttr< ::ESMoL::Subsystem> ref() const { return Udm::PointerAttr< ::ESMoL::Subsystem>(impl, meta_ref); }

			Udm::ParentAttr< ::ESMoL::MgaObject> parent() const { return Udm::ParentAttr< ::ESMoL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  TypeBase :  public ::ESMoL::MgaObject {
		public:
			static ::Uml::Class meta;

			TypeBase() { }
			TypeBase(Udm::ObjectImpl *impl) : ::ESMoL::MgaObject(impl) { }
			TypeBase(const TypeBase &master) : ::ESMoL::MgaObject(master) { }
			static TypeBase Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static TypeBase Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			TypeBase CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			TypeBase CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::ESMoL::TypeBase> Instances() { return Udm::InstantiatedAttr< ::ESMoL::TypeBase>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::ESMoL::TypeBase, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::ESMoL::TypeBase, Pred>(impl);}

			Udm::DerivedAttr< ::ESMoL::TypeBase> Derived() { return Udm::DerivedAttr< ::ESMoL::TypeBase>(impl);}
			template <class Pred> Udm::DerivedAttr< ::ESMoL::TypeBase, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::ESMoL::TypeBase, Pred>(impl);}

			Udm::ArchetypeAttr< ::ESMoL::TypeBase> Archetype() { return Udm::ArchetypeAttr< ::ESMoL::TypeBase>(impl);}

			static ::Uml::AssociationRole meta_referedbyTypeStructRef;
			Udm::AssocAttr< ::ESMoL::TypeBaseRef> referedbyTypeStructRef() const { return Udm::AssocAttr< ::ESMoL::TypeBaseRef>(impl, meta_referedbyTypeStructRef); }
			template <class Pred> Udm::AssocAttr< ::ESMoL::TypeBaseRef, Pred > referedbyTypeStructRef_sorted(const Pred &) const { return Udm::AssocAttr< ::ESMoL::TypeBaseRef, Pred>(impl, meta_referedbyTypeStructRef); }

			static ::Uml::CompositionParentRole meta_Types_parent;
			Udm::ParentAttr< ::ESMoL::Types> Types_parent() const { return Udm::ParentAttr< ::ESMoL::Types>(impl, meta_Types_parent); }

			Udm::ParentAttr< ::ESMoL::Types> parent() const { return Udm::ParentAttr< ::ESMoL::Types>(impl, Udm::NULLPARENTROLE); }
		};

		class  TypeStruct :  public ::ESMoL::TypeBase {
		public:
			static ::Uml::Class meta;

			TypeStruct() { }
			TypeStruct(Udm::ObjectImpl *impl) : ::ESMoL::TypeBase(impl) { }
			TypeStruct(const TypeStruct &master) : ::ESMoL::TypeBase(master) { }
			static TypeStruct Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static TypeStruct Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			TypeStruct CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			TypeStruct CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::ESMoL::TypeStruct> Instances() { return Udm::InstantiatedAttr< ::ESMoL::TypeStruct>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::ESMoL::TypeStruct, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::ESMoL::TypeStruct, Pred>(impl);}

			Udm::DerivedAttr< ::ESMoL::TypeStruct> Derived() { return Udm::DerivedAttr< ::ESMoL::TypeStruct>(impl);}
			template <class Pred> Udm::DerivedAttr< ::ESMoL::TypeStruct, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::ESMoL::TypeStruct, Pred>(impl);}

			Udm::ArchetypeAttr< ::ESMoL::TypeStruct> Archetype() { return Udm::ArchetypeAttr< ::ESMoL::TypeStruct>(impl);}

			static ::Uml::Attribute meta_MemberCount;
			Udm::IntegerAttr MemberCount() const { return Udm::IntegerAttr(impl, meta_MemberCount); }

			static ::Uml::CompositionChildRole meta_TypeBaseRef_children;
			Udm::ChildrenAttr< ::ESMoL::TypeBaseRef> TypeBaseRef_children() const { return Udm::ChildrenAttr< ::ESMoL::TypeBaseRef>(impl, meta_TypeBaseRef_children); }
			template <class Pred> Udm::ChildrenAttr< ::ESMoL::TypeBaseRef, Pred> TypeBaseRef_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::TypeBaseRef, Pred>(impl, meta_TypeBaseRef_children); }

			Udm::ChildrenAttr< ::ESMoL::TypeBaseRef> TypeBaseRef_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::TypeBaseRef>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::TypeBaseRef, Pred> TypeBaseRef_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::TypeBaseRef, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::ESMoL::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::MgaObject>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ParentAttr< ::ESMoL::Types> parent() const { return Udm::ParentAttr< ::ESMoL::Types>(impl, Udm::NULLPARENTROLE); }
		};

		class  Matrix :  public ::ESMoL::TypeBase {
		public:
			static ::Uml::Class meta;

			Matrix() { }
			Matrix(Udm::ObjectImpl *impl) : ::ESMoL::TypeBase(impl) { }
			Matrix(const Matrix &master) : ::ESMoL::TypeBase(master) { }
			static Matrix Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static Matrix Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			Matrix CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			Matrix CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::ESMoL::Matrix> Instances() { return Udm::InstantiatedAttr< ::ESMoL::Matrix>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::ESMoL::Matrix, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::ESMoL::Matrix, Pred>(impl);}

			Udm::DerivedAttr< ::ESMoL::Matrix> Derived() { return Udm::DerivedAttr< ::ESMoL::Matrix>(impl);}
			template <class Pred> Udm::DerivedAttr< ::ESMoL::Matrix, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::ESMoL::Matrix, Pred>(impl);}

			Udm::ArchetypeAttr< ::ESMoL::Matrix> Archetype() { return Udm::ArchetypeAttr< ::ESMoL::Matrix>(impl);}

			static ::Uml::Attribute meta_rows;
			Udm::IntegerAttr rows() const { return Udm::IntegerAttr(impl, meta_rows); }

			static ::Uml::Attribute meta_Type;
			Udm::StringAttr Type() const { return Udm::StringAttr(impl, meta_Type); }

			static ::Uml::Attribute meta_columns;
			Udm::IntegerAttr columns() const { return Udm::IntegerAttr(impl, meta_columns); }

			Udm::ParentAttr< ::ESMoL::Types> parent() const { return Udm::ParentAttr< ::ESMoL::Types>(impl, Udm::NULLPARENTROLE); }
		};

		class  TypeBaseRef :  public ::ESMoL::MgaObject {
		public:
			static ::Uml::Class meta;

			TypeBaseRef() { }
			TypeBaseRef(Udm::ObjectImpl *impl) : ::ESMoL::MgaObject(impl) { }
			TypeBaseRef(const TypeBaseRef &master) : ::ESMoL::MgaObject(master) { }
			static TypeBaseRef Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static TypeBaseRef Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			TypeBaseRef CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			TypeBaseRef CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::ESMoL::TypeBaseRef> Instances() { return Udm::InstantiatedAttr< ::ESMoL::TypeBaseRef>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::ESMoL::TypeBaseRef, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::ESMoL::TypeBaseRef, Pred>(impl);}

			Udm::DerivedAttr< ::ESMoL::TypeBaseRef> Derived() { return Udm::DerivedAttr< ::ESMoL::TypeBaseRef>(impl);}
			template <class Pred> Udm::DerivedAttr< ::ESMoL::TypeBaseRef, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::ESMoL::TypeBaseRef, Pred>(impl);}

			Udm::ArchetypeAttr< ::ESMoL::TypeBaseRef> Archetype() { return Udm::ArchetypeAttr< ::ESMoL::TypeBaseRef>(impl);}

			static ::Uml::Attribute meta_MemberIndex;
			Udm::IntegerAttr MemberIndex() const { return Udm::IntegerAttr(impl, meta_MemberIndex); }

			static ::Uml::AssociationRole meta_ref;
			Udm::PointerAttr< ::ESMoL::TypeBase> ref() const { return Udm::PointerAttr< ::ESMoL::TypeBase>(impl, meta_ref); }

			static ::Uml::CompositionParentRole meta_TypeStruct_parent;
			Udm::ParentAttr< ::ESMoL::TypeStruct> TypeStruct_parent() const { return Udm::ParentAttr< ::ESMoL::TypeStruct>(impl, meta_TypeStruct_parent); }

			static ::Uml::CompositionParentRole meta_Port_parent;
			Udm::ParentAttr< ::ESMoL::Port> Port_parent() const { return Udm::ParentAttr< ::ESMoL::Port>(impl, meta_Port_parent); }

			static ::Uml::CompositionParentRole meta_StateDE_parent;
			Udm::ParentAttr< ::ESMoL::StateDE> StateDE_parent() const { return Udm::ParentAttr< ::ESMoL::StateDE>(impl, meta_StateDE_parent); }

			Udm::ParentAttr< ::ESMoL::MgaObject> parent() const { return Udm::ParentAttr< ::ESMoL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  TimingConstraint :  public ::ESMoL::MgaObject {
		public:
			static ::Uml::Class meta;

			TimingConstraint() { }
			TimingConstraint(Udm::ObjectImpl *impl) : ::ESMoL::MgaObject(impl) { }
			TimingConstraint(const TimingConstraint &master) : ::ESMoL::MgaObject(master) { }
			static TimingConstraint Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static TimingConstraint Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			TimingConstraint CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			TimingConstraint CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::ESMoL::TimingConstraint> Instances() { return Udm::InstantiatedAttr< ::ESMoL::TimingConstraint>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::ESMoL::TimingConstraint, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::ESMoL::TimingConstraint, Pred>(impl);}

			Udm::DerivedAttr< ::ESMoL::TimingConstraint> Derived() { return Udm::DerivedAttr< ::ESMoL::TimingConstraint>(impl);}
			template <class Pred> Udm::DerivedAttr< ::ESMoL::TimingConstraint, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::ESMoL::TimingConstraint, Pred>(impl);}

			Udm::ArchetypeAttr< ::ESMoL::TimingConstraint> Archetype() { return Udm::ArchetypeAttr< ::ESMoL::TimingConstraint>(impl);}

			static ::Uml::Attribute meta_Latency;
			Udm::StringAttr Latency() const { return Udm::StringAttr(impl, meta_Latency); }

			static ::Uml::CompositionParentRole meta_TimingSheet_parent;
			Udm::ParentAttr< ::ESMoL::TimingSheet> TimingSheet_parent() const { return Udm::ParentAttr< ::ESMoL::TimingSheet>(impl, meta_TimingSheet_parent); }

			Udm::ParentAttr< ::ESMoL::MgaObject> parent() const { return Udm::ParentAttr< ::ESMoL::MgaObject>(impl, Udm::NULLPARENTROLE); }
			static ::Uml::AssociationRole meta_dstTimingConstraint_end_;
			Udm::AssocEndAttr< ::ESMoL::TaskRef> dstTimingConstraint_end() const { return Udm::AssocEndAttr< ::ESMoL::TaskRef>(impl, meta_dstTimingConstraint_end_); }

			static ::Uml::AssociationRole meta_srcTimingConstraint_end_;
			Udm::AssocEndAttr< ::ESMoL::TaskRef> srcTimingConstraint_end() const { return Udm::AssocEndAttr< ::ESMoL::TaskRef>(impl, meta_srcTimingConstraint_end_); }

		};

		class  TimingSheet :  public ::ESMoL::MgaObject {
		public:
			static ::Uml::Class meta;

			TimingSheet() { }
			TimingSheet(Udm::ObjectImpl *impl) : ::ESMoL::MgaObject(impl) { }
			TimingSheet(const TimingSheet &master) : ::ESMoL::MgaObject(master) { }
			static TimingSheet Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static TimingSheet Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			TimingSheet CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			TimingSheet CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::ESMoL::TimingSheet> Instances() { return Udm::InstantiatedAttr< ::ESMoL::TimingSheet>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::ESMoL::TimingSheet, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::ESMoL::TimingSheet, Pred>(impl);}

			Udm::DerivedAttr< ::ESMoL::TimingSheet> Derived() { return Udm::DerivedAttr< ::ESMoL::TimingSheet>(impl);}
			template <class Pred> Udm::DerivedAttr< ::ESMoL::TimingSheet, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::ESMoL::TimingSheet, Pred>(impl);}

			Udm::ArchetypeAttr< ::ESMoL::TimingSheet> Archetype() { return Udm::ArchetypeAttr< ::ESMoL::TimingSheet>(impl);}

			static ::Uml::CompositionChildRole meta_TimingConstraint_children;
			Udm::ChildrenAttr< ::ESMoL::TimingConstraint> TimingConstraint_children() const { return Udm::ChildrenAttr< ::ESMoL::TimingConstraint>(impl, meta_TimingConstraint_children); }
			template <class Pred> Udm::ChildrenAttr< ::ESMoL::TimingConstraint, Pred> TimingConstraint_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::TimingConstraint, Pred>(impl, meta_TimingConstraint_children); }

			static ::Uml::CompositionChildRole meta_TaskRef_children;
			Udm::ChildrenAttr< ::ESMoL::TaskRef> TaskRef_children() const { return Udm::ChildrenAttr< ::ESMoL::TaskRef>(impl, meta_TaskRef_children); }
			template <class Pred> Udm::ChildrenAttr< ::ESMoL::TaskRef, Pred> TaskRef_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::TaskRef, Pred>(impl, meta_TaskRef_children); }

			Udm::ChildrenAttr< ::ESMoL::TimingConstraint> TimingConstraint_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::TimingConstraint>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::TimingConstraint, Pred> TimingConstraint_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::TimingConstraint, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::ESMoL::TaskRef> TaskRef_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::TaskRef>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::TaskRef, Pred> TaskRef_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::TaskRef, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::ESMoL::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::MgaObject>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

			static ::Uml::CompositionParentRole meta_DesignFolder_parent;
			Udm::ParentAttr< ::ESMoL::DesignFolder> DesignFolder_parent() const { return Udm::ParentAttr< ::ESMoL::DesignFolder>(impl, meta_DesignFolder_parent); }

			static ::Uml::CompositionParentRole meta_RequirementsLibrary_parent;
			Udm::ParentAttr< ::ESMoL::RequirementsLibrary> RequirementsLibrary_parent() const { return Udm::ParentAttr< ::ESMoL::RequirementsLibrary>(impl, meta_RequirementsLibrary_parent); }

			Udm::ParentAttr<Udm::Object> parent() const { return Udm::ParentAttr<Udm::Object>(impl, Udm::NULLPARENTROLE); }
		};

		class  TaskRef :  public ::ESMoL::MgaObject {
		public:
			static ::Uml::Class meta;

			TaskRef() { }
			TaskRef(Udm::ObjectImpl *impl) : ::ESMoL::MgaObject(impl) { }
			TaskRef(const TaskRef &master) : ::ESMoL::MgaObject(master) { }
			static TaskRef Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static TaskRef Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			TaskRef CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			TaskRef CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::ESMoL::TaskRef> Instances() { return Udm::InstantiatedAttr< ::ESMoL::TaskRef>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::ESMoL::TaskRef, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::ESMoL::TaskRef, Pred>(impl);}

			Udm::DerivedAttr< ::ESMoL::TaskRef> Derived() { return Udm::DerivedAttr< ::ESMoL::TaskRef>(impl);}
			template <class Pred> Udm::DerivedAttr< ::ESMoL::TaskRef, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::ESMoL::TaskRef, Pred>(impl);}

			Udm::ArchetypeAttr< ::ESMoL::TaskRef> Archetype() { return Udm::ArchetypeAttr< ::ESMoL::TaskRef>(impl);}

			static ::Uml::AssociationRole meta_srcTimingConstraint;
			static ::Uml::AssociationRole meta_srcTimingConstraint_rev;
			Udm::AClassAssocAttr< ::ESMoL::TimingConstraint, ::ESMoL::TaskRef> srcTimingConstraint() const { return Udm::AClassAssocAttr< ::ESMoL::TimingConstraint, ::ESMoL::TaskRef>(impl, meta_srcTimingConstraint, meta_srcTimingConstraint_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::ESMoL::TimingConstraint, ::ESMoL::TaskRef, Pred> srcTimingConstraint_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::ESMoL::TimingConstraint, ::ESMoL::TaskRef, Pred>(impl, meta_srcTimingConstraint, meta_srcTimingConstraint_rev); }

			static ::Uml::AssociationRole meta_dstTimingConstraint;
			static ::Uml::AssociationRole meta_dstTimingConstraint_rev;
			Udm::AClassAssocAttr< ::ESMoL::TimingConstraint, ::ESMoL::TaskRef> dstTimingConstraint() const { return Udm::AClassAssocAttr< ::ESMoL::TimingConstraint, ::ESMoL::TaskRef>(impl, meta_dstTimingConstraint, meta_dstTimingConstraint_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::ESMoL::TimingConstraint, ::ESMoL::TaskRef, Pred> dstTimingConstraint_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::ESMoL::TimingConstraint, ::ESMoL::TaskRef, Pred>(impl, meta_dstTimingConstraint, meta_dstTimingConstraint_rev); }

			static ::Uml::AssociationRole meta_ref;
			Udm::PointerAttr< ::ESMoL::Task> ref() const { return Udm::PointerAttr< ::ESMoL::Task>(impl, meta_ref); }

			static ::Uml::CompositionParentRole meta_TimingSheet_parent;
			Udm::ParentAttr< ::ESMoL::TimingSheet> TimingSheet_parent() const { return Udm::ParentAttr< ::ESMoL::TimingSheet>(impl, meta_TimingSheet_parent); }

			Udm::ParentAttr< ::ESMoL::MgaObject> parent() const { return Udm::ParentAttr< ::ESMoL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  Block :  virtual public ::ESMoL::MgaObject {
		public:
			static ::Uml::Class meta;

			Block() { }
			Block(Udm::ObjectImpl *impl) : ::ESMoL::MgaObject(impl) { }
			Block(const Block &master) : ::ESMoL::MgaObject(master) { }
			static Block Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static Block Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			Block CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			Block CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::ESMoL::Block> Instances() { return Udm::InstantiatedAttr< ::ESMoL::Block>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::ESMoL::Block, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::ESMoL::Block, Pred>(impl);}

			Udm::DerivedAttr< ::ESMoL::Block> Derived() { return Udm::DerivedAttr< ::ESMoL::Block>(impl);}
			template <class Pred> Udm::DerivedAttr< ::ESMoL::Block, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::ESMoL::Block, Pred>(impl);}

			Udm::ArchetypeAttr< ::ESMoL::Block> Archetype() { return Udm::ArchetypeAttr< ::ESMoL::Block>(impl);}

			static ::Uml::Attribute meta_Tag;
			Udm::StringAttr Tag() const { return Udm::StringAttr(impl, meta_Tag); }

			static ::Uml::Attribute meta_BlockType;
			Udm::StringAttr BlockType() const { return Udm::StringAttr(impl, meta_BlockType); }

			static ::Uml::Attribute meta_Name;
			Udm::StringAttr Name() const { return Udm::StringAttr(impl, meta_Name); }

			static ::Uml::Attribute meta_Description;
			Udm::StringAttr Description() const { return Udm::StringAttr(impl, meta_Description); }

			static ::Uml::Attribute meta_Priority;
			Udm::IntegerAttr Priority() const { return Udm::IntegerAttr(impl, meta_Priority); }

			static ::Uml::Attribute meta_SampleTime;
			Udm::RealAttr SampleTime() const { return Udm::RealAttr(impl, meta_SampleTime); }

			static ::Uml::Attribute meta_UserData;
			Udm::StringAttr UserData() const { return Udm::StringAttr(impl, meta_UserData); }

			static ::Uml::CompositionChildRole meta_Line;
			Udm::ChildrenAttr< ::ESMoL::Line> Line() const { return Udm::ChildrenAttr< ::ESMoL::Line>(impl, meta_Line); }
			template <class Pred> Udm::ChildrenAttr< ::ESMoL::Line, Pred> Line_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::Line, Pred>(impl, meta_Line); }

			static ::Uml::CompositionChildRole meta_Parameter;
			Udm::ChildrenAttr< ::ESMoL::Parameter> Parameter() const { return Udm::ChildrenAttr< ::ESMoL::Parameter>(impl, meta_Parameter); }
			template <class Pred> Udm::ChildrenAttr< ::ESMoL::Parameter, Pred> Parameter_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::Parameter, Pred>(impl, meta_Parameter); }

			static ::Uml::CompositionChildRole meta_Annotation_children;
			Udm::ChildrenAttr< ::ESMoL::Annotation> Annotation_children() const { return Udm::ChildrenAttr< ::ESMoL::Annotation>(impl, meta_Annotation_children); }
			template <class Pred> Udm::ChildrenAttr< ::ESMoL::Annotation, Pred> Annotation_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::Annotation, Pred>(impl, meta_Annotation_children); }

			static ::Uml::CompositionChildRole meta_ConnectorRef_children;
			Udm::ChildrenAttr< ::ESMoL::ConnectorRef> ConnectorRef_children() const { return Udm::ChildrenAttr< ::ESMoL::ConnectorRef>(impl, meta_ConnectorRef_children); }
			template <class Pred> Udm::ChildrenAttr< ::ESMoL::ConnectorRef, Pred> ConnectorRef_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::ConnectorRef, Pred>(impl, meta_ConnectorRef_children); }

			static ::Uml::CompositionChildRole meta_Port_children;
			Udm::ChildrenAttr< ::ESMoL::Port> Port_children() const { return Udm::ChildrenAttr< ::ESMoL::Port>(impl, meta_Port_children); }
			template <class Pred> Udm::ChildrenAttr< ::ESMoL::Port, Pred> Port_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::Port, Pred>(impl, meta_Port_children); }

			Udm::ChildrenAttr< ::ESMoL::Parameter> Parameter_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::Parameter>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::Parameter, Pred> Parameter_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::Parameter, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::ESMoL::Annotation> Annotation_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::Annotation>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::Annotation, Pred> Annotation_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::Annotation, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::ESMoL::Line> Line_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::Line>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::Line, Pred> Line_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::Line, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::ESMoL::OutputPort> OutputPort_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::OutputPort>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::OutputPort, Pred> OutputPort_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::OutputPort, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::ESMoL::OutPort> OutPort_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::OutPort>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::OutPort, Pred> OutPort_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::OutPort, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::ESMoL::StatePort> StatePort_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::StatePort>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::StatePort, Pred> StatePort_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::StatePort, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::ESMoL::Port> Port_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::Port>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::Port, Pred> Port_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::Port, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::ESMoL::EnablePort> EnablePort_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::EnablePort>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::EnablePort, Pred> EnablePort_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::EnablePort, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::ESMoL::TriggerPort> TriggerPort_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::TriggerPort>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::TriggerPort, Pred> TriggerPort_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::TriggerPort, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::ESMoL::InputPort> InputPort_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::InputPort>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::InputPort, Pred> InputPort_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::InputPort, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::ESMoL::InPort> InPort_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::InPort>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::InPort, Pred> InPort_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::InPort, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::ESMoL::ActionPort> ActionPort_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::ActionPort>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::ActionPort, Pred> ActionPort_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::ActionPort, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::ESMoL::ConnectorRef> ConnectorRef_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::ConnectorRef>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::ConnectorRef, Pred> ConnectorRef_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::ConnectorRef, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::ESMoL::TransConnector> TransConnector_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::TransConnector>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::TransConnector, Pred> TransConnector_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::TransConnector, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::ESMoL::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::MgaObject>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

			static ::Uml::CompositionParentRole meta_Subsystem_parent;
			Udm::ParentAttr< ::ESMoL::Subsystem> Subsystem_parent() const { return Udm::ParentAttr< ::ESMoL::Subsystem>(impl, meta_Subsystem_parent); }

			Udm::ParentAttr< ::ESMoL::MgaObject> parent() const { return Udm::ParentAttr< ::ESMoL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  Primitive :  public ::ESMoL::Block {
		public:
			static ::Uml::Class meta;

			Primitive() { }
			Primitive(Udm::ObjectImpl *impl) : ::ESMoL::Block(impl), ::ESMoL::MgaObject(impl) { }
			Primitive(const Primitive &master) : ::ESMoL::Block(master), ::ESMoL::MgaObject(master) { }
			static Primitive Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static Primitive Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			Primitive CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			Primitive CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::ESMoL::Primitive> Instances() { return Udm::InstantiatedAttr< ::ESMoL::Primitive>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::ESMoL::Primitive, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::ESMoL::Primitive, Pred>(impl);}

			Udm::DerivedAttr< ::ESMoL::Primitive> Derived() { return Udm::DerivedAttr< ::ESMoL::Primitive>(impl);}
			template <class Pred> Udm::DerivedAttr< ::ESMoL::Primitive, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::ESMoL::Primitive, Pred>(impl);}

			Udm::ArchetypeAttr< ::ESMoL::Primitive> Archetype() { return Udm::ArchetypeAttr< ::ESMoL::Primitive>(impl);}

			Udm::ParentAttr< ::ESMoL::MgaObject> parent() const { return Udm::ParentAttr< ::ESMoL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  Parameter :  public ::ESMoL::MgaObject {
		public:
			static ::Uml::Class meta;

			Parameter() { }
			Parameter(Udm::ObjectImpl *impl) : ::ESMoL::MgaObject(impl) { }
			Parameter(const Parameter &master) : ::ESMoL::MgaObject(master) { }
			static Parameter Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static Parameter Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			Parameter CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			Parameter CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::ESMoL::Parameter> Instances() { return Udm::InstantiatedAttr< ::ESMoL::Parameter>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::ESMoL::Parameter, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::ESMoL::Parameter, Pred>(impl);}

			Udm::DerivedAttr< ::ESMoL::Parameter> Derived() { return Udm::DerivedAttr< ::ESMoL::Parameter>(impl);}
			template <class Pred> Udm::DerivedAttr< ::ESMoL::Parameter, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::ESMoL::Parameter, Pred>(impl);}

			Udm::ArchetypeAttr< ::ESMoL::Parameter> Archetype() { return Udm::ArchetypeAttr< ::ESMoL::Parameter>(impl);}

			static ::Uml::Attribute meta_Value;
			Udm::StringAttr Value() const { return Udm::StringAttr(impl, meta_Value); }

			static ::Uml::CompositionParentRole meta_Parameter_Block_parent;
			Udm::ParentAttr< ::ESMoL::Block> Parameter_Block_parent() const { return Udm::ParentAttr< ::ESMoL::Block>(impl, meta_Parameter_Block_parent); }

			Udm::ParentAttr< ::ESMoL::MgaObject> parent() const { return Udm::ParentAttr< ::ESMoL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  Reference :  public ::ESMoL::Block {
		public:
			static ::Uml::Class meta;

			Reference() { }
			Reference(Udm::ObjectImpl *impl) : ::ESMoL::Block(impl), ::ESMoL::MgaObject(impl) { }
			Reference(const Reference &master) : ::ESMoL::Block(master), ::ESMoL::MgaObject(master) { }
			static Reference Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static Reference Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			Reference CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			Reference CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::ESMoL::Reference> Instances() { return Udm::InstantiatedAttr< ::ESMoL::Reference>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::ESMoL::Reference, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::ESMoL::Reference, Pred>(impl);}

			Udm::DerivedAttr< ::ESMoL::Reference> Derived() { return Udm::DerivedAttr< ::ESMoL::Reference>(impl);}
			template <class Pred> Udm::DerivedAttr< ::ESMoL::Reference, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::ESMoL::Reference, Pred>(impl);}

			Udm::ArchetypeAttr< ::ESMoL::Reference> Archetype() { return Udm::ArchetypeAttr< ::ESMoL::Reference>(impl);}

			static ::Uml::Attribute meta_SourceBlock;
			Udm::StringAttr SourceBlock() const { return Udm::StringAttr(impl, meta_SourceBlock); }

			static ::Uml::Attribute meta_SourceType;
			Udm::StringAttr SourceType() const { return Udm::StringAttr(impl, meta_SourceType); }

			Udm::ParentAttr< ::ESMoL::MgaObject> parent() const { return Udm::ParentAttr< ::ESMoL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  Annotation :  public ::ESMoL::MgaObject {
		public:
			static ::Uml::Class meta;

			Annotation() { }
			Annotation(Udm::ObjectImpl *impl) : ::ESMoL::MgaObject(impl) { }
			Annotation(const Annotation &master) : ::ESMoL::MgaObject(master) { }
			static Annotation Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static Annotation Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			Annotation CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			Annotation CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::ESMoL::Annotation> Instances() { return Udm::InstantiatedAttr< ::ESMoL::Annotation>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::ESMoL::Annotation, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::ESMoL::Annotation, Pred>(impl);}

			Udm::DerivedAttr< ::ESMoL::Annotation> Derived() { return Udm::DerivedAttr< ::ESMoL::Annotation>(impl);}
			template <class Pred> Udm::DerivedAttr< ::ESMoL::Annotation, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::ESMoL::Annotation, Pred>(impl);}

			Udm::ArchetypeAttr< ::ESMoL::Annotation> Archetype() { return Udm::ArchetypeAttr< ::ESMoL::Annotation>(impl);}

			static ::Uml::Attribute meta_Text;
			Udm::StringAttr Text() const { return Udm::StringAttr(impl, meta_Text); }

			static ::Uml::CompositionParentRole meta_Block_parent;
			Udm::ParentAttr< ::ESMoL::Block> Block_parent() const { return Udm::ParentAttr< ::ESMoL::Block>(impl, meta_Block_parent); }

			Udm::ParentAttr< ::ESMoL::MgaObject> parent() const { return Udm::ParentAttr< ::ESMoL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  Line :  public ::ESMoL::MgaObject {
		public:
			static ::Uml::Class meta;

			Line() { }
			Line(Udm::ObjectImpl *impl) : ::ESMoL::MgaObject(impl) { }
			Line(const Line &master) : ::ESMoL::MgaObject(master) { }
			static Line Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static Line Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			Line CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			Line CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::ESMoL::Line> Instances() { return Udm::InstantiatedAttr< ::ESMoL::Line>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::ESMoL::Line, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::ESMoL::Line, Pred>(impl);}

			Udm::DerivedAttr< ::ESMoL::Line> Derived() { return Udm::DerivedAttr< ::ESMoL::Line>(impl);}
			template <class Pred> Udm::DerivedAttr< ::ESMoL::Line, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::ESMoL::Line, Pred>(impl);}

			Udm::ArchetypeAttr< ::ESMoL::Line> Archetype() { return Udm::ArchetypeAttr< ::ESMoL::Line>(impl);}

			static ::Uml::Attribute meta_Name;
			Udm::StringAttr Name() const { return Udm::StringAttr(impl, meta_Name); }

			static ::Uml::CompositionParentRole meta_Line_Block_parent;
			Udm::ParentAttr< ::ESMoL::Block> Line_Block_parent() const { return Udm::ParentAttr< ::ESMoL::Block>(impl, meta_Line_Block_parent); }

			Udm::ParentAttr< ::ESMoL::MgaObject> parent() const { return Udm::ParentAttr< ::ESMoL::MgaObject>(impl, Udm::NULLPARENTROLE); }
			static ::Uml::AssociationRole meta_dstLine_end_;
			Udm::AssocEndAttr< ::ESMoL::Port> dstLine_end() const { return Udm::AssocEndAttr< ::ESMoL::Port>(impl, meta_dstLine_end_); }

			static ::Uml::AssociationRole meta_srcLine_end_;
			Udm::AssocEndAttr< ::ESMoL::Port> srcLine_end() const { return Udm::AssocEndAttr< ::ESMoL::Port>(impl, meta_srcLine_end_); }

		};

		class  Subsystem :  public ::ESMoL::ComponentBase, public ::ESMoL::Block {
		public:
			static ::Uml::Class meta;

			Subsystem() { }
			Subsystem(Udm::ObjectImpl *impl) : ::ESMoL::ComponentBase(impl),::ESMoL::Block(impl), ::ESMoL::MgaObject(impl) { }
			Subsystem(const Subsystem &master) : ::ESMoL::ComponentBase(master),::ESMoL::Block(master), ::ESMoL::MgaObject(master) { }
			static Subsystem Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static Subsystem Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			Subsystem CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			Subsystem CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::ESMoL::Subsystem> Instances() { return Udm::InstantiatedAttr< ::ESMoL::Subsystem>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::ESMoL::Subsystem, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::ESMoL::Subsystem, Pred>(impl);}

			Udm::DerivedAttr< ::ESMoL::Subsystem> Derived() { return Udm::DerivedAttr< ::ESMoL::Subsystem>(impl);}
			template <class Pred> Udm::DerivedAttr< ::ESMoL::Subsystem, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::ESMoL::Subsystem, Pred>(impl);}

			Udm::ArchetypeAttr< ::ESMoL::Subsystem> Archetype() { return Udm::ArchetypeAttr< ::ESMoL::Subsystem>(impl);}

			static ::Uml::AssociationRole meta_referedbySubsystemRef;
			Udm::AssocAttr< ::ESMoL::SubsystemRef> referedbySubsystemRef() const { return Udm::AssocAttr< ::ESMoL::SubsystemRef>(impl, meta_referedbySubsystemRef); }
			template <class Pred> Udm::AssocAttr< ::ESMoL::SubsystemRef, Pred > referedbySubsystemRef_sorted(const Pred &) const { return Udm::AssocAttr< ::ESMoL::SubsystemRef, Pred>(impl, meta_referedbySubsystemRef); }

			static ::Uml::CompositionChildRole meta_Block_children;
			Udm::ChildrenAttr< ::ESMoL::Block> Block_children() const { return Udm::ChildrenAttr< ::ESMoL::Block>(impl, meta_Block_children); }
			template <class Pred> Udm::ChildrenAttr< ::ESMoL::Block, Pred> Block_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::Block, Pred>(impl, meta_Block_children); }

			Udm::ChildrenAttr< ::ESMoL::Primitive> Primitive_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::Primitive>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::Primitive, Pred> Primitive_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::Primitive, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::ESMoL::Block> Block_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::Block>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::Block, Pred> Block_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::Block, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::ESMoL::Reference> Reference_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::Reference>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::Reference, Pred> Reference_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::Reference, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::ESMoL::Subsystem> Subsystem_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::Subsystem>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::Subsystem, Pred> Subsystem_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::Subsystem, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::ESMoL::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::MgaObject>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

			static ::Uml::CompositionParentRole meta_DesignFolder_parent;
			Udm::ParentAttr< ::ESMoL::DesignFolder> DesignFolder_parent() const { return Udm::ParentAttr< ::ESMoL::DesignFolder>(impl, meta_DesignFolder_parent); }

			static ::Uml::CompositionParentRole meta_Dataflow_parent;
			Udm::ParentAttr< ::ESMoL::Dataflow> Dataflow_parent() const { return Udm::ParentAttr< ::ESMoL::Dataflow>(impl, meta_Dataflow_parent); }

			Udm::ParentAttr<Udm::Object> parent() const { return Udm::ParentAttr<Udm::Object>(impl, Udm::NULLPARENTROLE); }
		};

		class  Port :  virtual public ::ESMoL::MgaObject {
		public:
			static ::Uml::Class meta;

			Port() { }
			Port(Udm::ObjectImpl *impl) : ::ESMoL::MgaObject(impl) { }
			Port(const Port &master) : ::ESMoL::MgaObject(master) { }
			static Port Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static Port Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			Port CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			Port CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::ESMoL::Port> Instances() { return Udm::InstantiatedAttr< ::ESMoL::Port>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::ESMoL::Port, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::ESMoL::Port, Pred>(impl);}

			Udm::DerivedAttr< ::ESMoL::Port> Derived() { return Udm::DerivedAttr< ::ESMoL::Port>(impl);}
			template <class Pred> Udm::DerivedAttr< ::ESMoL::Port, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::ESMoL::Port, Pred>(impl);}

			Udm::ArchetypeAttr< ::ESMoL::Port> Archetype() { return Udm::ArchetypeAttr< ::ESMoL::Port>(impl);}

			static ::Uml::AssociationRole meta_srcLine;
			static ::Uml::AssociationRole meta_srcLine_rev;
			Udm::AClassAssocAttr< ::ESMoL::Line, ::ESMoL::Port> srcLine() const { return Udm::AClassAssocAttr< ::ESMoL::Line, ::ESMoL::Port>(impl, meta_srcLine, meta_srcLine_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::ESMoL::Line, ::ESMoL::Port, Pred> srcLine_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::ESMoL::Line, ::ESMoL::Port, Pred>(impl, meta_srcLine, meta_srcLine_rev); }

			static ::Uml::AssociationRole meta_dstLine;
			static ::Uml::AssociationRole meta_dstLine_rev;
			Udm::AClassAssocAttr< ::ESMoL::Line, ::ESMoL::Port> dstLine() const { return Udm::AClassAssocAttr< ::ESMoL::Line, ::ESMoL::Port>(impl, meta_dstLine, meta_dstLine_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::ESMoL::Line, ::ESMoL::Port, Pred> dstLine_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::ESMoL::Line, ::ESMoL::Port, Pred>(impl, meta_dstLine, meta_dstLine_rev); }

			static ::Uml::CompositionChildRole meta_TypeBaseRef_child;
			Udm::ChildAttr< ::ESMoL::TypeBaseRef> TypeBaseRef_child() const { return Udm::ChildAttr< ::ESMoL::TypeBaseRef>(impl, meta_TypeBaseRef_child); }

			Udm::ChildrenAttr< ::ESMoL::TypeBaseRef> TypeBaseRef_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::TypeBaseRef>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::TypeBaseRef, Pred> TypeBaseRef_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::TypeBaseRef, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::ESMoL::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::MgaObject>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

			static ::Uml::CompositionParentRole meta_Block_parent;
			Udm::ParentAttr< ::ESMoL::Block> Block_parent() const { return Udm::ParentAttr< ::ESMoL::Block>(impl, meta_Block_parent); }

			Udm::ParentAttr< ::ESMoL::MgaObject> parent() const { return Udm::ParentAttr< ::ESMoL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  OutPort :  public ::ESMoL::Port {
		public:
			static ::Uml::Class meta;

			OutPort() { }
			OutPort(Udm::ObjectImpl *impl) : ::ESMoL::Port(impl), ::ESMoL::MgaObject(impl) { }
			OutPort(const OutPort &master) : ::ESMoL::Port(master), ::ESMoL::MgaObject(master) { }
			static OutPort Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static OutPort Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			OutPort CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			OutPort CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::ESMoL::OutPort> Instances() { return Udm::InstantiatedAttr< ::ESMoL::OutPort>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::ESMoL::OutPort, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::ESMoL::OutPort, Pred>(impl);}

			Udm::DerivedAttr< ::ESMoL::OutPort> Derived() { return Udm::DerivedAttr< ::ESMoL::OutPort>(impl);}
			template <class Pred> Udm::DerivedAttr< ::ESMoL::OutPort, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::ESMoL::OutPort, Pred>(impl);}

			Udm::ArchetypeAttr< ::ESMoL::OutPort> Archetype() { return Udm::ArchetypeAttr< ::ESMoL::OutPort>(impl);}

			Udm::ParentAttr< ::ESMoL::MgaObject> parent() const { return Udm::ParentAttr< ::ESMoL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  StatePort :  public ::ESMoL::OutPort {
		public:
			static ::Uml::Class meta;

			StatePort() { }
			StatePort(Udm::ObjectImpl *impl) : ::ESMoL::OutPort(impl), ::ESMoL::MgaObject(impl) { }
			StatePort(const StatePort &master) : ::ESMoL::OutPort(master), ::ESMoL::MgaObject(master) { }
			static StatePort Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static StatePort Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			StatePort CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			StatePort CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::ESMoL::StatePort> Instances() { return Udm::InstantiatedAttr< ::ESMoL::StatePort>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::ESMoL::StatePort, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::ESMoL::StatePort, Pred>(impl);}

			Udm::DerivedAttr< ::ESMoL::StatePort> Derived() { return Udm::DerivedAttr< ::ESMoL::StatePort>(impl);}
			template <class Pred> Udm::DerivedAttr< ::ESMoL::StatePort, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::ESMoL::StatePort, Pred>(impl);}

			Udm::ArchetypeAttr< ::ESMoL::StatePort> Archetype() { return Udm::ArchetypeAttr< ::ESMoL::StatePort>(impl);}

			Udm::ParentAttr< ::ESMoL::MgaObject> parent() const { return Udm::ParentAttr< ::ESMoL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  InPort :  public ::ESMoL::Port {
		public:
			static ::Uml::Class meta;

			InPort() { }
			InPort(Udm::ObjectImpl *impl) : ::ESMoL::Port(impl), ::ESMoL::MgaObject(impl) { }
			InPort(const InPort &master) : ::ESMoL::Port(master), ::ESMoL::MgaObject(master) { }
			static InPort Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static InPort Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			InPort CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			InPort CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::ESMoL::InPort> Instances() { return Udm::InstantiatedAttr< ::ESMoL::InPort>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::ESMoL::InPort, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::ESMoL::InPort, Pred>(impl);}

			Udm::DerivedAttr< ::ESMoL::InPort> Derived() { return Udm::DerivedAttr< ::ESMoL::InPort>(impl);}
			template <class Pred> Udm::DerivedAttr< ::ESMoL::InPort, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::ESMoL::InPort, Pred>(impl);}

			Udm::ArchetypeAttr< ::ESMoL::InPort> Archetype() { return Udm::ArchetypeAttr< ::ESMoL::InPort>(impl);}

			Udm::ParentAttr< ::ESMoL::MgaObject> parent() const { return Udm::ParentAttr< ::ESMoL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  EnablePort :  public ::ESMoL::InPort {
		public:
			static ::Uml::Class meta;

			EnablePort() { }
			EnablePort(Udm::ObjectImpl *impl) : ::ESMoL::InPort(impl), ::ESMoL::MgaObject(impl) { }
			EnablePort(const EnablePort &master) : ::ESMoL::InPort(master), ::ESMoL::MgaObject(master) { }
			static EnablePort Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static EnablePort Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			EnablePort CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			EnablePort CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::ESMoL::EnablePort> Instances() { return Udm::InstantiatedAttr< ::ESMoL::EnablePort>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::ESMoL::EnablePort, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::ESMoL::EnablePort, Pred>(impl);}

			Udm::DerivedAttr< ::ESMoL::EnablePort> Derived() { return Udm::DerivedAttr< ::ESMoL::EnablePort>(impl);}
			template <class Pred> Udm::DerivedAttr< ::ESMoL::EnablePort, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::ESMoL::EnablePort, Pred>(impl);}

			Udm::ArchetypeAttr< ::ESMoL::EnablePort> Archetype() { return Udm::ArchetypeAttr< ::ESMoL::EnablePort>(impl);}

			static ::Uml::Attribute meta_StatesWhenEnabling;
			Udm::StringAttr StatesWhenEnabling() const { return Udm::StringAttr(impl, meta_StatesWhenEnabling); }

			Udm::ParentAttr< ::ESMoL::MgaObject> parent() const { return Udm::ParentAttr< ::ESMoL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  TriggerPort :  public ::ESMoL::InPort {
		public:
			static ::Uml::Class meta;

			TriggerPort() { }
			TriggerPort(Udm::ObjectImpl *impl) : ::ESMoL::InPort(impl), ::ESMoL::MgaObject(impl) { }
			TriggerPort(const TriggerPort &master) : ::ESMoL::InPort(master), ::ESMoL::MgaObject(master) { }
			static TriggerPort Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static TriggerPort Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			TriggerPort CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			TriggerPort CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::ESMoL::TriggerPort> Instances() { return Udm::InstantiatedAttr< ::ESMoL::TriggerPort>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::ESMoL::TriggerPort, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::ESMoL::TriggerPort, Pred>(impl);}

			Udm::DerivedAttr< ::ESMoL::TriggerPort> Derived() { return Udm::DerivedAttr< ::ESMoL::TriggerPort>(impl);}
			template <class Pred> Udm::DerivedAttr< ::ESMoL::TriggerPort, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::ESMoL::TriggerPort, Pred>(impl);}

			Udm::ArchetypeAttr< ::ESMoL::TriggerPort> Archetype() { return Udm::ArchetypeAttr< ::ESMoL::TriggerPort>(impl);}

			static ::Uml::Attribute meta_TriggerType;
			Udm::StringAttr TriggerType() const { return Udm::StringAttr(impl, meta_TriggerType); }

			Udm::ParentAttr< ::ESMoL::MgaObject> parent() const { return Udm::ParentAttr< ::ESMoL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  ActionPort :  public ::ESMoL::InPort {
		public:
			static ::Uml::Class meta;

			ActionPort() { }
			ActionPort(Udm::ObjectImpl *impl) : ::ESMoL::InPort(impl), ::ESMoL::MgaObject(impl) { }
			ActionPort(const ActionPort &master) : ::ESMoL::InPort(master), ::ESMoL::MgaObject(master) { }
			static ActionPort Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static ActionPort Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			ActionPort CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			ActionPort CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::ESMoL::ActionPort> Instances() { return Udm::InstantiatedAttr< ::ESMoL::ActionPort>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::ESMoL::ActionPort, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::ESMoL::ActionPort, Pred>(impl);}

			Udm::DerivedAttr< ::ESMoL::ActionPort> Derived() { return Udm::DerivedAttr< ::ESMoL::ActionPort>(impl);}
			template <class Pred> Udm::DerivedAttr< ::ESMoL::ActionPort, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::ESMoL::ActionPort, Pred>(impl);}

			Udm::ArchetypeAttr< ::ESMoL::ActionPort> Archetype() { return Udm::ArchetypeAttr< ::ESMoL::ActionPort>(impl);}

			Udm::ParentAttr< ::ESMoL::MgaObject> parent() const { return Udm::ParentAttr< ::ESMoL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  Transition :  public ::ESMoL::MgaObject {
		public:
			static ::Uml::Class meta;

			Transition() { }
			Transition(Udm::ObjectImpl *impl) : ::ESMoL::MgaObject(impl) { }
			Transition(const Transition &master) : ::ESMoL::MgaObject(master) { }
			static Transition Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static Transition Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			Transition CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			Transition CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::ESMoL::Transition> Instances() { return Udm::InstantiatedAttr< ::ESMoL::Transition>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::ESMoL::Transition, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::ESMoL::Transition, Pred>(impl);}

			Udm::DerivedAttr< ::ESMoL::Transition> Derived() { return Udm::DerivedAttr< ::ESMoL::Transition>(impl);}
			template <class Pred> Udm::DerivedAttr< ::ESMoL::Transition, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::ESMoL::Transition, Pred>(impl);}

			Udm::ArchetypeAttr< ::ESMoL::Transition> Archetype() { return Udm::ArchetypeAttr< ::ESMoL::Transition>(impl);}

			static ::Uml::Attribute meta_Guard;
			Udm::StringAttr Guard() const { return Udm::StringAttr(impl, meta_Guard); }

			static ::Uml::Attribute meta_Trigger;
			Udm::StringAttr Trigger() const { return Udm::StringAttr(impl, meta_Trigger); }

			static ::Uml::Attribute meta_Action;
			Udm::StringAttr Action() const { return Udm::StringAttr(impl, meta_Action); }

			static ::Uml::Attribute meta_ConditionAction;
			Udm::StringAttr ConditionAction() const { return Udm::StringAttr(impl, meta_ConditionAction); }

			static ::Uml::Attribute meta_Order;
			Udm::StringAttr Order() const { return Udm::StringAttr(impl, meta_Order); }

			static ::Uml::CompositionParentRole meta_Transition_State_parent;
			Udm::ParentAttr< ::ESMoL::State> Transition_State_parent() const { return Udm::ParentAttr< ::ESMoL::State>(impl, meta_Transition_State_parent); }

			Udm::ParentAttr< ::ESMoL::MgaObject> parent() const { return Udm::ParentAttr< ::ESMoL::MgaObject>(impl, Udm::NULLPARENTROLE); }
			static ::Uml::AssociationRole meta_srcTransition_end_;
			Udm::AssocEndAttr< ::ESMoL::TransConnector> srcTransition_end() const { return Udm::AssocEndAttr< ::ESMoL::TransConnector>(impl, meta_srcTransition_end_); }

			static ::Uml::AssociationRole meta_dstTransition_end_;
			Udm::AssocEndAttr< ::ESMoL::TransConnector> dstTransition_end() const { return Udm::AssocEndAttr< ::ESMoL::TransConnector>(impl, meta_dstTransition_end_); }

		};

		class  TransConnector :  public ::ESMoL::MgaObject {
		public:
			static ::Uml::Class meta;

			TransConnector() { }
			TransConnector(Udm::ObjectImpl *impl) : ::ESMoL::MgaObject(impl) { }
			TransConnector(const TransConnector &master) : ::ESMoL::MgaObject(master) { }
			static TransConnector Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static TransConnector Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			TransConnector CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			TransConnector CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::ESMoL::TransConnector> Instances() { return Udm::InstantiatedAttr< ::ESMoL::TransConnector>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::ESMoL::TransConnector, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::ESMoL::TransConnector, Pred>(impl);}

			Udm::DerivedAttr< ::ESMoL::TransConnector> Derived() { return Udm::DerivedAttr< ::ESMoL::TransConnector>(impl);}
			template <class Pred> Udm::DerivedAttr< ::ESMoL::TransConnector, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::ESMoL::TransConnector, Pred>(impl);}

			Udm::ArchetypeAttr< ::ESMoL::TransConnector> Archetype() { return Udm::ArchetypeAttr< ::ESMoL::TransConnector>(impl);}

			static ::Uml::AssociationRole meta_referedbyConnectorRef;
			Udm::AssocAttr< ::ESMoL::ConnectorRef> referedbyConnectorRef() const { return Udm::AssocAttr< ::ESMoL::ConnectorRef>(impl, meta_referedbyConnectorRef); }
			template <class Pred> Udm::AssocAttr< ::ESMoL::ConnectorRef, Pred > referedbyConnectorRef_sorted(const Pred &) const { return Udm::AssocAttr< ::ESMoL::ConnectorRef, Pred>(impl, meta_referedbyConnectorRef); }

			static ::Uml::AssociationRole meta_dstTransition;
			static ::Uml::AssociationRole meta_dstTransition_rev;
			Udm::AClassAssocAttr< ::ESMoL::Transition, ::ESMoL::TransConnector> dstTransition() const { return Udm::AClassAssocAttr< ::ESMoL::Transition, ::ESMoL::TransConnector>(impl, meta_dstTransition, meta_dstTransition_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::ESMoL::Transition, ::ESMoL::TransConnector, Pred> dstTransition_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::ESMoL::Transition, ::ESMoL::TransConnector, Pred>(impl, meta_dstTransition, meta_dstTransition_rev); }

			static ::Uml::AssociationRole meta_srcTransition;
			static ::Uml::AssociationRole meta_srcTransition_rev;
			Udm::AClassAssocAttr< ::ESMoL::Transition, ::ESMoL::TransConnector> srcTransition() const { return Udm::AClassAssocAttr< ::ESMoL::Transition, ::ESMoL::TransConnector>(impl, meta_srcTransition, meta_srcTransition_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::ESMoL::Transition, ::ESMoL::TransConnector, Pred> srcTransition_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::ESMoL::Transition, ::ESMoL::TransConnector, Pred>(impl, meta_srcTransition, meta_srcTransition_rev); }

			static ::Uml::CompositionParentRole meta_State_parent;
			Udm::ParentAttr< ::ESMoL::State> State_parent() const { return Udm::ParentAttr< ::ESMoL::State>(impl, meta_State_parent); }

			Udm::ParentAttr< ::ESMoL::MgaObject> parent() const { return Udm::ParentAttr< ::ESMoL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  State :  public ::ESMoL::TransConnector {
		public:
			static ::Uml::Class meta;

			State() { }
			State(Udm::ObjectImpl *impl) : ::ESMoL::TransConnector(impl) { }
			State(const State &master) : ::ESMoL::TransConnector(master) { }
			static State Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static State Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			State CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			State CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::ESMoL::State> Instances() { return Udm::InstantiatedAttr< ::ESMoL::State>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::ESMoL::State, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::ESMoL::State, Pred>(impl);}

			Udm::DerivedAttr< ::ESMoL::State> Derived() { return Udm::DerivedAttr< ::ESMoL::State>(impl);}
			template <class Pred> Udm::DerivedAttr< ::ESMoL::State, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::ESMoL::State, Pred>(impl);}

			Udm::ArchetypeAttr< ::ESMoL::State> Archetype() { return Udm::ArchetypeAttr< ::ESMoL::State>(impl);}

			static ::Uml::Attribute meta_Name;
			Udm::StringAttr Name() const { return Udm::StringAttr(impl, meta_Name); }

			static ::Uml::Attribute meta_Decomposition;
			Udm::StringAttr Decomposition() const { return Udm::StringAttr(impl, meta_Decomposition); }

			static ::Uml::Attribute meta_EnterAction;
			Udm::StringAttr EnterAction() const { return Udm::StringAttr(impl, meta_EnterAction); }

			static ::Uml::Attribute meta_DuringAction;
			Udm::StringAttr DuringAction() const { return Udm::StringAttr(impl, meta_DuringAction); }

			static ::Uml::Attribute meta_ExitAction;
			Udm::StringAttr ExitAction() const { return Udm::StringAttr(impl, meta_ExitAction); }

			static ::Uml::Attribute meta_Order;
			Udm::StringAttr Order() const { return Udm::StringAttr(impl, meta_Order); }

			static ::Uml::CompositionChildRole meta_TransConnector_children;
			Udm::ChildrenAttr< ::ESMoL::TransConnector> TransConnector_children() const { return Udm::ChildrenAttr< ::ESMoL::TransConnector>(impl, meta_TransConnector_children); }
			template <class Pred> Udm::ChildrenAttr< ::ESMoL::TransConnector, Pred> TransConnector_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::TransConnector, Pred>(impl, meta_TransConnector_children); }

			static ::Uml::CompositionChildRole meta_Transition;
			Udm::ChildrenAttr< ::ESMoL::Transition> Transition() const { return Udm::ChildrenAttr< ::ESMoL::Transition>(impl, meta_Transition); }
			template <class Pred> Udm::ChildrenAttr< ::ESMoL::Transition, Pred> Transition_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::Transition, Pred>(impl, meta_Transition); }

			static ::Uml::CompositionChildRole meta_StateDE_children;
			Udm::ChildrenAttr< ::ESMoL::StateDE> StateDE_children() const { return Udm::ChildrenAttr< ::ESMoL::StateDE>(impl, meta_StateDE_children); }
			template <class Pred> Udm::ChildrenAttr< ::ESMoL::StateDE, Pred> StateDE_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::StateDE, Pred>(impl, meta_StateDE_children); }

			Udm::ChildrenAttr< ::ESMoL::State> State_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::State>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::State, Pred> State_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::State, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::ESMoL::Transition> Transition_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::Transition>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::Transition, Pred> Transition_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::Transition, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::ESMoL::History> History_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::History>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::History, Pred> History_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::History, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::ESMoL::TransStart> TransStart_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::TransStart>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::TransStart, Pred> TransStart_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::TransStart, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::ESMoL::Junction> Junction_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::Junction>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::Junction, Pred> Junction_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::Junction, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::ESMoL::ConnectorRef> ConnectorRef_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::ConnectorRef>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::ConnectorRef, Pred> ConnectorRef_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::ConnectorRef, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::ESMoL::TransConnector> TransConnector_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::TransConnector>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::TransConnector, Pred> TransConnector_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::TransConnector, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::ESMoL::Data> Data_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::Data>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::Data, Pred> Data_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::Data, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::ESMoL::Event> Event_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::Event>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::Event, Pred> Event_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::Event, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::ESMoL::StateDE> StateDE_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::StateDE>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::StateDE, Pred> StateDE_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::StateDE, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::ESMoL::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::MgaObject>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

			static ::Uml::CompositionParentRole meta_Stateflow_parent;
			Udm::ParentAttr< ::ESMoL::Stateflow> Stateflow_parent() const { return Udm::ParentAttr< ::ESMoL::Stateflow>(impl, meta_Stateflow_parent); }

			Udm::ParentAttr<Udm::Object> parent() const { return Udm::ParentAttr<Udm::Object>(impl, Udm::NULLPARENTROLE); }
		};

		class  History :  public ::ESMoL::TransConnector {
		public:
			static ::Uml::Class meta;

			History() { }
			History(Udm::ObjectImpl *impl) : ::ESMoL::TransConnector(impl) { }
			History(const History &master) : ::ESMoL::TransConnector(master) { }
			static History Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static History Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			History CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			History CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::ESMoL::History> Instances() { return Udm::InstantiatedAttr< ::ESMoL::History>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::ESMoL::History, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::ESMoL::History, Pred>(impl);}

			Udm::DerivedAttr< ::ESMoL::History> Derived() { return Udm::DerivedAttr< ::ESMoL::History>(impl);}
			template <class Pred> Udm::DerivedAttr< ::ESMoL::History, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::ESMoL::History, Pred>(impl);}

			Udm::ArchetypeAttr< ::ESMoL::History> Archetype() { return Udm::ArchetypeAttr< ::ESMoL::History>(impl);}

			Udm::ParentAttr< ::ESMoL::MgaObject> parent() const { return Udm::ParentAttr< ::ESMoL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  TransStart :  public ::ESMoL::TransConnector {
		public:
			static ::Uml::Class meta;

			TransStart() { }
			TransStart(Udm::ObjectImpl *impl) : ::ESMoL::TransConnector(impl) { }
			TransStart(const TransStart &master) : ::ESMoL::TransConnector(master) { }
			static TransStart Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static TransStart Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			TransStart CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			TransStart CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::ESMoL::TransStart> Instances() { return Udm::InstantiatedAttr< ::ESMoL::TransStart>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::ESMoL::TransStart, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::ESMoL::TransStart, Pred>(impl);}

			Udm::DerivedAttr< ::ESMoL::TransStart> Derived() { return Udm::DerivedAttr< ::ESMoL::TransStart>(impl);}
			template <class Pred> Udm::DerivedAttr< ::ESMoL::TransStart, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::ESMoL::TransStart, Pred>(impl);}

			Udm::ArchetypeAttr< ::ESMoL::TransStart> Archetype() { return Udm::ArchetypeAttr< ::ESMoL::TransStart>(impl);}

			Udm::ParentAttr< ::ESMoL::MgaObject> parent() const { return Udm::ParentAttr< ::ESMoL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  Junction :  public ::ESMoL::TransConnector {
		public:
			static ::Uml::Class meta;

			Junction() { }
			Junction(Udm::ObjectImpl *impl) : ::ESMoL::TransConnector(impl) { }
			Junction(const Junction &master) : ::ESMoL::TransConnector(master) { }
			static Junction Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static Junction Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			Junction CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			Junction CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::ESMoL::Junction> Instances() { return Udm::InstantiatedAttr< ::ESMoL::Junction>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::ESMoL::Junction, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::ESMoL::Junction, Pred>(impl);}

			Udm::DerivedAttr< ::ESMoL::Junction> Derived() { return Udm::DerivedAttr< ::ESMoL::Junction>(impl);}
			template <class Pred> Udm::DerivedAttr< ::ESMoL::Junction, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::ESMoL::Junction, Pred>(impl);}

			Udm::ArchetypeAttr< ::ESMoL::Junction> Archetype() { return Udm::ArchetypeAttr< ::ESMoL::Junction>(impl);}

			Udm::ParentAttr< ::ESMoL::MgaObject> parent() const { return Udm::ParentAttr< ::ESMoL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  ConnectorRef :  public ::ESMoL::TransConnector {
		public:
			static ::Uml::Class meta;

			ConnectorRef() { }
			ConnectorRef(Udm::ObjectImpl *impl) : ::ESMoL::TransConnector(impl) { }
			ConnectorRef(const ConnectorRef &master) : ::ESMoL::TransConnector(master) { }
			static ConnectorRef Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static ConnectorRef Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			ConnectorRef CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			ConnectorRef CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::ESMoL::ConnectorRef> Instances() { return Udm::InstantiatedAttr< ::ESMoL::ConnectorRef>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::ESMoL::ConnectorRef, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::ESMoL::ConnectorRef, Pred>(impl);}

			Udm::DerivedAttr< ::ESMoL::ConnectorRef> Derived() { return Udm::DerivedAttr< ::ESMoL::ConnectorRef>(impl);}
			template <class Pred> Udm::DerivedAttr< ::ESMoL::ConnectorRef, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::ESMoL::ConnectorRef, Pred>(impl);}

			Udm::ArchetypeAttr< ::ESMoL::ConnectorRef> Archetype() { return Udm::ArchetypeAttr< ::ESMoL::ConnectorRef>(impl);}

			static ::Uml::AssociationRole meta_ref;
			Udm::PointerAttr< ::ESMoL::TransConnector> ref() const { return Udm::PointerAttr< ::ESMoL::TransConnector>(impl, meta_ref); }

			static ::Uml::CompositionParentRole meta_Block_parent;
			Udm::ParentAttr< ::ESMoL::Block> Block_parent() const { return Udm::ParentAttr< ::ESMoL::Block>(impl, meta_Block_parent); }

			Udm::ParentAttr< ::ESMoL::MgaObject> parent() const { return Udm::ParentAttr< ::ESMoL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  StateDE :  public ::ESMoL::MgaObject {
		public:
			static ::Uml::Class meta;

			StateDE() { }
			StateDE(Udm::ObjectImpl *impl) : ::ESMoL::MgaObject(impl) { }
			StateDE(const StateDE &master) : ::ESMoL::MgaObject(master) { }
			static StateDE Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static StateDE Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			StateDE CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			StateDE CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::ESMoL::StateDE> Instances() { return Udm::InstantiatedAttr< ::ESMoL::StateDE>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::ESMoL::StateDE, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::ESMoL::StateDE, Pred>(impl);}

			Udm::DerivedAttr< ::ESMoL::StateDE> Derived() { return Udm::DerivedAttr< ::ESMoL::StateDE>(impl);}
			template <class Pred> Udm::DerivedAttr< ::ESMoL::StateDE, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::ESMoL::StateDE, Pred>(impl);}

			Udm::ArchetypeAttr< ::ESMoL::StateDE> Archetype() { return Udm::ArchetypeAttr< ::ESMoL::StateDE>(impl);}

			static ::Uml::CompositionChildRole meta_TypeBaseRef_child;
			Udm::ChildAttr< ::ESMoL::TypeBaseRef> TypeBaseRef_child() const { return Udm::ChildAttr< ::ESMoL::TypeBaseRef>(impl, meta_TypeBaseRef_child); }

			Udm::ChildrenAttr< ::ESMoL::TypeBaseRef> TypeBaseRef_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::TypeBaseRef>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::TypeBaseRef, Pred> TypeBaseRef_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::TypeBaseRef, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::ESMoL::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::MgaObject>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

			static ::Uml::CompositionParentRole meta_State_parent;
			Udm::ParentAttr< ::ESMoL::State> State_parent() const { return Udm::ParentAttr< ::ESMoL::State>(impl, meta_State_parent); }

			Udm::ParentAttr< ::ESMoL::MgaObject> parent() const { return Udm::ParentAttr< ::ESMoL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  Data :  public ::ESMoL::StateDE {
		public:
			static ::Uml::Class meta;

			Data() { }
			Data(Udm::ObjectImpl *impl) : ::ESMoL::StateDE(impl) { }
			Data(const Data &master) : ::ESMoL::StateDE(master) { }
			static Data Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static Data Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			Data CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			Data CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::ESMoL::Data> Instances() { return Udm::InstantiatedAttr< ::ESMoL::Data>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::ESMoL::Data, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::ESMoL::Data, Pred>(impl);}

			Udm::DerivedAttr< ::ESMoL::Data> Derived() { return Udm::DerivedAttr< ::ESMoL::Data>(impl);}
			template <class Pred> Udm::DerivedAttr< ::ESMoL::Data, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::ESMoL::Data, Pred>(impl);}

			Udm::ArchetypeAttr< ::ESMoL::Data> Archetype() { return Udm::ArchetypeAttr< ::ESMoL::Data>(impl);}

			static ::Uml::Attribute meta_Name;
			Udm::StringAttr Name() const { return Udm::StringAttr(impl, meta_Name); }

			static ::Uml::Attribute meta_Description;
			Udm::StringAttr Description() const { return Udm::StringAttr(impl, meta_Description); }

			static ::Uml::Attribute meta_DataType;
			Udm::StringAttr DataType() const { return Udm::StringAttr(impl, meta_DataType); }

			static ::Uml::Attribute meta_Port;
			Udm::IntegerAttr Port() const { return Udm::IntegerAttr(impl, meta_Port); }

			static ::Uml::Attribute meta_Units;
			Udm::StringAttr Units() const { return Udm::StringAttr(impl, meta_Units); }

			static ::Uml::Attribute meta_InitialValue;
			Udm::StringAttr InitialValue() const { return Udm::StringAttr(impl, meta_InitialValue); }

			static ::Uml::Attribute meta_Min;
			Udm::StringAttr Min() const { return Udm::StringAttr(impl, meta_Min); }

			static ::Uml::Attribute meta_Max;
			Udm::StringAttr Max() const { return Udm::StringAttr(impl, meta_Max); }

			static ::Uml::Attribute meta_ArraySize;
			Udm::StringAttr ArraySize() const { return Udm::StringAttr(impl, meta_ArraySize); }

			static ::Uml::Attribute meta_ArrayFirstIndex;
			Udm::StringAttr ArrayFirstIndex() const { return Udm::StringAttr(impl, meta_ArrayFirstIndex); }

			static ::Uml::Attribute meta_Scope;
			Udm::StringAttr Scope() const { return Udm::StringAttr(impl, meta_Scope); }

			Udm::ParentAttr< ::ESMoL::MgaObject> parent() const { return Udm::ParentAttr< ::ESMoL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  Event :  public ::ESMoL::StateDE {
		public:
			static ::Uml::Class meta;

			Event() { }
			Event(Udm::ObjectImpl *impl) : ::ESMoL::StateDE(impl) { }
			Event(const Event &master) : ::ESMoL::StateDE(master) { }
			static Event Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static Event Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			Event CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			Event CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::ESMoL::Event> Instances() { return Udm::InstantiatedAttr< ::ESMoL::Event>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::ESMoL::Event, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::ESMoL::Event, Pred>(impl);}

			Udm::DerivedAttr< ::ESMoL::Event> Derived() { return Udm::DerivedAttr< ::ESMoL::Event>(impl);}
			template <class Pred> Udm::DerivedAttr< ::ESMoL::Event, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::ESMoL::Event, Pred>(impl);}

			Udm::ArchetypeAttr< ::ESMoL::Event> Archetype() { return Udm::ArchetypeAttr< ::ESMoL::Event>(impl);}

			static ::Uml::Attribute meta_Name;
			Udm::StringAttr Name() const { return Udm::StringAttr(impl, meta_Name); }

			static ::Uml::Attribute meta_Description;
			Udm::StringAttr Description() const { return Udm::StringAttr(impl, meta_Description); }

			static ::Uml::Attribute meta_Scope;
			Udm::StringAttr Scope() const { return Udm::StringAttr(impl, meta_Scope); }

			static ::Uml::Attribute meta_Trigger;
			Udm::StringAttr Trigger() const { return Udm::StringAttr(impl, meta_Trigger); }

			static ::Uml::Attribute meta_Port;
			Udm::IntegerAttr Port() const { return Udm::IntegerAttr(impl, meta_Port); }

			Udm::ParentAttr< ::ESMoL::MgaObject> parent() const { return Udm::ParentAttr< ::ESMoL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  OS :  public ::ESMoL::MgaObject {
		public:
			static ::Uml::Class meta;

			OS() { }
			OS(Udm::ObjectImpl *impl) : ::ESMoL::MgaObject(impl) { }
			OS(const OS &master) : ::ESMoL::MgaObject(master) { }
			static OS Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static OS Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			OS CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			OS CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::ESMoL::OS> Instances() { return Udm::InstantiatedAttr< ::ESMoL::OS>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::ESMoL::OS, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::ESMoL::OS, Pred>(impl);}

			Udm::DerivedAttr< ::ESMoL::OS> Derived() { return Udm::DerivedAttr< ::ESMoL::OS>(impl);}
			template <class Pred> Udm::DerivedAttr< ::ESMoL::OS, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::ESMoL::OS, Pred>(impl);}

			Udm::ArchetypeAttr< ::ESMoL::OS> Archetype() { return Udm::ArchetypeAttr< ::ESMoL::OS>(impl);}

			static ::Uml::Attribute meta_SendOverheadTime;
			Udm::StringAttr SendOverheadTime() const { return Udm::StringAttr(impl, meta_SendOverheadTime); }

			static ::Uml::Attribute meta_ContextSwitchTime;
			Udm::StringAttr ContextSwitchTime() const { return Udm::StringAttr(impl, meta_ContextSwitchTime); }

			static ::Uml::Attribute meta_RecvOverheadTime;
			Udm::StringAttr RecvOverheadTime() const { return Udm::StringAttr(impl, meta_RecvOverheadTime); }

			static ::Uml::Attribute meta_TickResolution;
			Udm::StringAttr TickResolution() const { return Udm::StringAttr(impl, meta_TickResolution); }

			static ::Uml::CompositionParentRole meta_Node_parent;
			Udm::ParentAttr< ::ESMoL::Node> Node_parent() const { return Udm::ParentAttr< ::ESMoL::Node>(impl, meta_Node_parent); }

			Udm::ParentAttr< ::ESMoL::MgaObject> parent() const { return Udm::ParentAttr< ::ESMoL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  Wire :  public ::ESMoL::MgaObject {
		public:
			static ::Uml::Class meta;

			Wire() { }
			Wire(Udm::ObjectImpl *impl) : ::ESMoL::MgaObject(impl) { }
			Wire(const Wire &master) : ::ESMoL::MgaObject(master) { }
			static Wire Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static Wire Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			Wire CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			Wire CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::ESMoL::Wire> Instances() { return Udm::InstantiatedAttr< ::ESMoL::Wire>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::ESMoL::Wire, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::ESMoL::Wire, Pred>(impl);}

			Udm::DerivedAttr< ::ESMoL::Wire> Derived() { return Udm::DerivedAttr< ::ESMoL::Wire>(impl);}
			template <class Pred> Udm::DerivedAttr< ::ESMoL::Wire, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::ESMoL::Wire, Pred>(impl);}

			Udm::ArchetypeAttr< ::ESMoL::Wire> Archetype() { return Udm::ArchetypeAttr< ::ESMoL::Wire>(impl);}

			static ::Uml::CompositionParentRole meta_HardwareUnit_parent;
			Udm::ParentAttr< ::ESMoL::HardwareUnit> HardwareUnit_parent() const { return Udm::ParentAttr< ::ESMoL::HardwareUnit>(impl, meta_HardwareUnit_parent); }

			Udm::ParentAttr< ::ESMoL::MgaObject> parent() const { return Udm::ParentAttr< ::ESMoL::MgaObject>(impl, Udm::NULLPARENTROLE); }
			static ::Uml::AssociationRole meta_dstWire_end_;
			Udm::AssocEndAttr< ::ESMoL::Connectable> dstWire_end() const { return Udm::AssocEndAttr< ::ESMoL::Connectable>(impl, meta_dstWire_end_); }

			static ::Uml::AssociationRole meta_srcWire_end_;
			Udm::AssocEndAttr< ::ESMoL::Connectable> srcWire_end() const { return Udm::AssocEndAttr< ::ESMoL::Connectable>(impl, meta_srcWire_end_); }

		};

		class  Connectable :  virtual public ::ESMoL::MgaObject {
		public:
			static ::Uml::Class meta;

			Connectable() { }
			Connectable(Udm::ObjectImpl *impl) : ::ESMoL::MgaObject(impl) { }
			Connectable(const Connectable &master) : ::ESMoL::MgaObject(master) { }
			static Connectable Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static Connectable Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			Connectable CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			Connectable CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::ESMoL::Connectable> Instances() { return Udm::InstantiatedAttr< ::ESMoL::Connectable>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::ESMoL::Connectable, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::ESMoL::Connectable, Pred>(impl);}

			Udm::DerivedAttr< ::ESMoL::Connectable> Derived() { return Udm::DerivedAttr< ::ESMoL::Connectable>(impl);}
			template <class Pred> Udm::DerivedAttr< ::ESMoL::Connectable, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::ESMoL::Connectable, Pred>(impl);}

			Udm::ArchetypeAttr< ::ESMoL::Connectable> Archetype() { return Udm::ArchetypeAttr< ::ESMoL::Connectable>(impl);}

			static ::Uml::AssociationRole meta_srcWire;
			static ::Uml::AssociationRole meta_srcWire_rev;
			Udm::AClassAssocAttr< ::ESMoL::Wire, ::ESMoL::Connectable> srcWire() const { return Udm::AClassAssocAttr< ::ESMoL::Wire, ::ESMoL::Connectable>(impl, meta_srcWire, meta_srcWire_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::ESMoL::Wire, ::ESMoL::Connectable, Pred> srcWire_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::ESMoL::Wire, ::ESMoL::Connectable, Pred>(impl, meta_srcWire, meta_srcWire_rev); }

			static ::Uml::AssociationRole meta_dstWire;
			static ::Uml::AssociationRole meta_dstWire_rev;
			Udm::AClassAssocAttr< ::ESMoL::Wire, ::ESMoL::Connectable> dstWire() const { return Udm::AClassAssocAttr< ::ESMoL::Wire, ::ESMoL::Connectable>(impl, meta_dstWire, meta_dstWire_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::ESMoL::Wire, ::ESMoL::Connectable, Pred> dstWire_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::ESMoL::Wire, ::ESMoL::Connectable, Pred>(impl, meta_dstWire, meta_dstWire_rev); }

			Udm::ParentAttr<Udm::Object> parent() const { return Udm::ParentAttr<Udm::Object>(impl, Udm::NULLPARENTROLE); }
		};

		class  Capability :  public ::ESMoL::MgaObject {
		public:
			static ::Uml::Class meta;

			Capability() { }
			Capability(Udm::ObjectImpl *impl) : ::ESMoL::MgaObject(impl) { }
			Capability(const Capability &master) : ::ESMoL::MgaObject(master) { }
			static Capability Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static Capability Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			Capability CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			Capability CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::ESMoL::Capability> Instances() { return Udm::InstantiatedAttr< ::ESMoL::Capability>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::ESMoL::Capability, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::ESMoL::Capability, Pred>(impl);}

			Udm::DerivedAttr< ::ESMoL::Capability> Derived() { return Udm::DerivedAttr< ::ESMoL::Capability>(impl);}
			template <class Pred> Udm::DerivedAttr< ::ESMoL::Capability, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::ESMoL::Capability, Pred>(impl);}

			Udm::ArchetypeAttr< ::ESMoL::Capability> Archetype() { return Udm::ArchetypeAttr< ::ESMoL::Capability>(impl);}

			static ::Uml::CompositionParentRole meta_HWElement_parent;
			Udm::ParentAttr< ::ESMoL::HWElement> HWElement_parent() const { return Udm::ParentAttr< ::ESMoL::HWElement>(impl, meta_HWElement_parent); }

			Udm::ParentAttr< ::ESMoL::MgaObject> parent() const { return Udm::ParentAttr< ::ESMoL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  HWElement :  virtual public ::ESMoL::MgaObject {
		public:
			static ::Uml::Class meta;

			HWElement() { }
			HWElement(Udm::ObjectImpl *impl) : ::ESMoL::MgaObject(impl) { }
			HWElement(const HWElement &master) : ::ESMoL::MgaObject(master) { }
			static HWElement Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static HWElement Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			HWElement CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			HWElement CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::ESMoL::HWElement> Instances() { return Udm::InstantiatedAttr< ::ESMoL::HWElement>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::ESMoL::HWElement, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::ESMoL::HWElement, Pred>(impl);}

			Udm::DerivedAttr< ::ESMoL::HWElement> Derived() { return Udm::DerivedAttr< ::ESMoL::HWElement>(impl);}
			template <class Pred> Udm::DerivedAttr< ::ESMoL::HWElement, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::ESMoL::HWElement, Pred>(impl);}

			Udm::ArchetypeAttr< ::ESMoL::HWElement> Archetype() { return Udm::ArchetypeAttr< ::ESMoL::HWElement>(impl);}

			static ::Uml::Attribute meta_MetaData;
			Udm::StringAttr MetaData() const { return Udm::StringAttr(impl, meta_MetaData); }

			static ::Uml::CompositionChildRole meta_Capability_children;
			Udm::ChildrenAttr< ::ESMoL::Capability> Capability_children() const { return Udm::ChildrenAttr< ::ESMoL::Capability>(impl, meta_Capability_children); }
			template <class Pred> Udm::ChildrenAttr< ::ESMoL::Capability, Pred> Capability_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::Capability, Pred>(impl, meta_Capability_children); }

			Udm::ChildrenAttr< ::ESMoL::Capability> Capability_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::Capability>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::Capability, Pred> Capability_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::Capability, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::ESMoL::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::MgaObject>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

			static ::Uml::CompositionParentRole meta_HardwareUnit_parent;
			Udm::ParentAttr< ::ESMoL::HardwareUnit> HardwareUnit_parent() const { return Udm::ParentAttr< ::ESMoL::HardwareUnit>(impl, meta_HardwareUnit_parent); }

			Udm::ParentAttr< ::ESMoL::MgaObject> parent() const { return Udm::ParentAttr< ::ESMoL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  HardwareUnit :  public ::ESMoL::HWElement {
		public:
			static ::Uml::Class meta;

			HardwareUnit() { }
			HardwareUnit(Udm::ObjectImpl *impl) : ::ESMoL::HWElement(impl), ::ESMoL::MgaObject(impl) { }
			HardwareUnit(const HardwareUnit &master) : ::ESMoL::HWElement(master), ::ESMoL::MgaObject(master) { }
			static HardwareUnit Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static HardwareUnit Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			HardwareUnit CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			HardwareUnit CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::ESMoL::HardwareUnit> Instances() { return Udm::InstantiatedAttr< ::ESMoL::HardwareUnit>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::ESMoL::HardwareUnit, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::ESMoL::HardwareUnit, Pred>(impl);}

			Udm::DerivedAttr< ::ESMoL::HardwareUnit> Derived() { return Udm::DerivedAttr< ::ESMoL::HardwareUnit>(impl);}
			template <class Pred> Udm::DerivedAttr< ::ESMoL::HardwareUnit, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::ESMoL::HardwareUnit, Pred>(impl);}

			Udm::ArchetypeAttr< ::ESMoL::HardwareUnit> Archetype() { return Udm::ArchetypeAttr< ::ESMoL::HardwareUnit>(impl);}

			static ::Uml::CompositionChildRole meta_HWElement_children;
			Udm::ChildrenAttr< ::ESMoL::HWElement> HWElement_children() const { return Udm::ChildrenAttr< ::ESMoL::HWElement>(impl, meta_HWElement_children); }
			template <class Pred> Udm::ChildrenAttr< ::ESMoL::HWElement, Pred> HWElement_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::HWElement, Pred>(impl, meta_HWElement_children); }

			static ::Uml::CompositionChildRole meta_Wire_children;
			Udm::ChildrenAttr< ::ESMoL::Wire> Wire_children() const { return Udm::ChildrenAttr< ::ESMoL::Wire>(impl, meta_Wire_children); }
			template <class Pred> Udm::ChildrenAttr< ::ESMoL::Wire, Pred> Wire_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::Wire, Pred>(impl, meta_Wire_children); }

			static ::Uml::CompositionChildRole meta_Channel_children;
			Udm::ChildrenAttr< ::ESMoL::Channel> Channel_children() const { return Udm::ChildrenAttr< ::ESMoL::Channel>(impl, meta_Channel_children); }
			template <class Pred> Udm::ChildrenAttr< ::ESMoL::Channel, Pred> Channel_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::Channel, Pred>(impl, meta_Channel_children); }

			Udm::ChildrenAttr< ::ESMoL::TTBus> TTBus_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::TTBus>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::TTBus, Pred> TTBus_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::TTBus, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::ESMoL::OldBus> OldBus_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::OldBus>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::OldBus, Pred> OldBus_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::OldBus, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::ESMoL::Wire> Wire_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::Wire>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::Wire, Pred> Wire_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::Wire, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::ESMoL::HardwareUnit> HardwareUnit_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::HardwareUnit>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::HardwareUnit, Pred> HardwareUnit_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::HardwareUnit, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::ESMoL::Router> Router_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::Router>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::Router, Pred> Router_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::Router, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::ESMoL::Connectable> Connectable_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::Connectable>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::Connectable, Pred> Connectable_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::Connectable, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::ESMoL::HWElement> HWElement_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::HWElement>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::HWElement, Pred> HWElement_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::HWElement, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::ESMoL::IChan> IChan_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::IChan>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::IChan, Pred> IChan_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::IChan, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::ESMoL::OChan> OChan_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::OChan>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::OChan, Pred> OChan_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::OChan, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::ESMoL::BChan> BChan_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::BChan>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::BChan, Pred> BChan_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::BChan, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::ESMoL::Bus> Bus_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::Bus>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::Bus, Pred> Bus_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::Bus, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::ESMoL::Node> Node_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::Node>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::Node, Pred> Node_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::Node, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::ESMoL::Channel> Channel_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::Channel>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::Channel, Pred> Channel_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::Channel, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::ESMoL::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::MgaObject>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

			static ::Uml::CompositionParentRole meta_DesignFolder_parent;
			Udm::ParentAttr< ::ESMoL::DesignFolder> DesignFolder_parent() const { return Udm::ParentAttr< ::ESMoL::DesignFolder>(impl, meta_DesignFolder_parent); }

			static ::Uml::CompositionParentRole meta_PlatformLibrary_parent;
			Udm::ParentAttr< ::ESMoL::PlatformLibrary> PlatformLibrary_parent() const { return Udm::ParentAttr< ::ESMoL::PlatformLibrary>(impl, meta_PlatformLibrary_parent); }

			Udm::ParentAttr<Udm::Object> parent() const { return Udm::ParentAttr<Udm::Object>(impl, Udm::NULLPARENTROLE); }
		};

		class  Router :  public ::ESMoL::HWElement {
		public:
			static ::Uml::Class meta;

			Router() { }
			Router(Udm::ObjectImpl *impl) : ::ESMoL::HWElement(impl), ::ESMoL::MgaObject(impl) { }
			Router(const Router &master) : ::ESMoL::HWElement(master), ::ESMoL::MgaObject(master) { }
			static Router Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static Router Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			Router CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			Router CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::ESMoL::Router> Instances() { return Udm::InstantiatedAttr< ::ESMoL::Router>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::ESMoL::Router, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::ESMoL::Router, Pred>(impl);}

			Udm::DerivedAttr< ::ESMoL::Router> Derived() { return Udm::DerivedAttr< ::ESMoL::Router>(impl);}
			template <class Pred> Udm::DerivedAttr< ::ESMoL::Router, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::ESMoL::Router, Pred>(impl);}

			Udm::ArchetypeAttr< ::ESMoL::Router> Archetype() { return Udm::ArchetypeAttr< ::ESMoL::Router>(impl);}

			static ::Uml::CompositionChildRole meta_BChan_children;
			Udm::ChildrenAttr< ::ESMoL::BChan> BChan_children() const { return Udm::ChildrenAttr< ::ESMoL::BChan>(impl, meta_BChan_children); }
			template <class Pred> Udm::ChildrenAttr< ::ESMoL::BChan, Pred> BChan_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::BChan, Pred>(impl, meta_BChan_children); }

			Udm::ChildrenAttr< ::ESMoL::Connectable> Connectable_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::Connectable>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::Connectable, Pred> Connectable_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::Connectable, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::ESMoL::BChan> BChan_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::BChan>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::BChan, Pred> BChan_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::BChan, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::ESMoL::Channel> Channel_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::Channel>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::Channel, Pred> Channel_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::Channel, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::ESMoL::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::MgaObject>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ParentAttr< ::ESMoL::MgaObject> parent() const { return Udm::ParentAttr< ::ESMoL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  Bus :  public ::ESMoL::Connectable, public ::ESMoL::HWElement {
		public:
			static ::Uml::Class meta;

			Bus() { }
			Bus(Udm::ObjectImpl *impl) : ::ESMoL::Connectable(impl),::ESMoL::HWElement(impl), ::ESMoL::MgaObject(impl) { }
			Bus(const Bus &master) : ::ESMoL::Connectable(master),::ESMoL::HWElement(master), ::ESMoL::MgaObject(master) { }
			static Bus Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static Bus Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			Bus CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			Bus CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::ESMoL::Bus> Instances() { return Udm::InstantiatedAttr< ::ESMoL::Bus>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::ESMoL::Bus, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::ESMoL::Bus, Pred>(impl);}

			Udm::DerivedAttr< ::ESMoL::Bus> Derived() { return Udm::DerivedAttr< ::ESMoL::Bus>(impl);}
			template <class Pred> Udm::DerivedAttr< ::ESMoL::Bus, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::ESMoL::Bus, Pred>(impl);}

			Udm::ArchetypeAttr< ::ESMoL::Bus> Archetype() { return Udm::ArchetypeAttr< ::ESMoL::Bus>(impl);}

			static ::Uml::Attribute meta_OverheadTime;
			Udm::StringAttr OverheadTime() const { return Udm::StringAttr(impl, meta_OverheadTime); }

			static ::Uml::Attribute meta_DataRate;
			Udm::StringAttr DataRate() const { return Udm::StringAttr(impl, meta_DataRate); }

			static ::Uml::Attribute meta_MinFrameSize;
			Udm::StringAttr MinFrameSize() const { return Udm::StringAttr(impl, meta_MinFrameSize); }

			static ::Uml::Attribute meta_SwitchMemorySize;
			Udm::StringAttr SwitchMemorySize() const { return Udm::StringAttr(impl, meta_SwitchMemorySize); }

			Udm::ParentAttr< ::ESMoL::MgaObject> parent() const { return Udm::ParentAttr< ::ESMoL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  TTBus :  public ::ESMoL::Bus {
		public:
			static ::Uml::Class meta;

			TTBus() { }
			TTBus(Udm::ObjectImpl *impl) : ::ESMoL::Bus(impl), ::ESMoL::MgaObject(impl) { }
			TTBus(const TTBus &master) : ::ESMoL::Bus(master), ::ESMoL::MgaObject(master) { }
			static TTBus Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static TTBus Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			TTBus CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			TTBus CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::ESMoL::TTBus> Instances() { return Udm::InstantiatedAttr< ::ESMoL::TTBus>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::ESMoL::TTBus, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::ESMoL::TTBus, Pred>(impl);}

			Udm::DerivedAttr< ::ESMoL::TTBus> Derived() { return Udm::DerivedAttr< ::ESMoL::TTBus>(impl);}
			template <class Pred> Udm::DerivedAttr< ::ESMoL::TTBus, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::ESMoL::TTBus, Pred>(impl);}

			Udm::ArchetypeAttr< ::ESMoL::TTBus> Archetype() { return Udm::ArchetypeAttr< ::ESMoL::TTBus>(impl);}

			static ::Uml::Attribute meta_TTSetupTime;
			Udm::StringAttr TTSetupTime() const { return Udm::StringAttr(impl, meta_TTSetupTime); }

			static ::Uml::Attribute meta_Hyperperiod;
			Udm::StringAttr Hyperperiod() const { return Udm::StringAttr(impl, meta_Hyperperiod); }

			static ::Uml::Attribute meta_SlotSize;
			Udm::StringAttr SlotSize() const { return Udm::StringAttr(impl, meta_SlotSize); }

			Udm::ParentAttr< ::ESMoL::MgaObject> parent() const { return Udm::ParentAttr< ::ESMoL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  OldBus :  public ::ESMoL::Bus {
		public:
			static ::Uml::Class meta;

			OldBus() { }
			OldBus(Udm::ObjectImpl *impl) : ::ESMoL::Bus(impl), ::ESMoL::MgaObject(impl) { }
			OldBus(const OldBus &master) : ::ESMoL::Bus(master), ::ESMoL::MgaObject(master) { }
			static OldBus Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static OldBus Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			OldBus CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			OldBus CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::ESMoL::OldBus> Instances() { return Udm::InstantiatedAttr< ::ESMoL::OldBus>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::ESMoL::OldBus, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::ESMoL::OldBus, Pred>(impl);}

			Udm::DerivedAttr< ::ESMoL::OldBus> Derived() { return Udm::DerivedAttr< ::ESMoL::OldBus>(impl);}
			template <class Pred> Udm::DerivedAttr< ::ESMoL::OldBus, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::ESMoL::OldBus, Pred>(impl);}

			Udm::ArchetypeAttr< ::ESMoL::OldBus> Archetype() { return Udm::ArchetypeAttr< ::ESMoL::OldBus>(impl);}

			static ::Uml::Attribute meta_FrameSize;
			Udm::IntegerAttr FrameSize() const { return Udm::IntegerAttr(impl, meta_FrameSize); }

			static ::Uml::Attribute meta_Medium;
			Udm::StringAttr Medium() const { return Udm::StringAttr(impl, meta_Medium); }

			Udm::ParentAttr< ::ESMoL::MgaObject> parent() const { return Udm::ParentAttr< ::ESMoL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  Node :  public ::ESMoL::HWElement {
		public:
			static ::Uml::Class meta;

			Node() { }
			Node(Udm::ObjectImpl *impl) : ::ESMoL::HWElement(impl), ::ESMoL::MgaObject(impl) { }
			Node(const Node &master) : ::ESMoL::HWElement(master), ::ESMoL::MgaObject(master) { }
			static Node Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static Node Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			Node CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			Node CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::ESMoL::Node> Instances() { return Udm::InstantiatedAttr< ::ESMoL::Node>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::ESMoL::Node, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::ESMoL::Node, Pred>(impl);}

			Udm::DerivedAttr< ::ESMoL::Node> Derived() { return Udm::DerivedAttr< ::ESMoL::Node>(impl);}
			template <class Pred> Udm::DerivedAttr< ::ESMoL::Node, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::ESMoL::Node, Pred>(impl);}

			Udm::ArchetypeAttr< ::ESMoL::Node> Archetype() { return Udm::ArchetypeAttr< ::ESMoL::Node>(impl);}

			static ::Uml::Attribute meta_ROM;
			Udm::IntegerAttr ROM() const { return Udm::IntegerAttr(impl, meta_ROM); }

			static ::Uml::Attribute meta_Simulator;
			Udm::StringAttr Simulator() const { return Udm::StringAttr(impl, meta_Simulator); }

			static ::Uml::Attribute meta_RAM;
			Udm::IntegerAttr RAM() const { return Udm::IntegerAttr(impl, meta_RAM); }

			static ::Uml::Attribute meta_Speed;
			Udm::IntegerAttr Speed() const { return Udm::IntegerAttr(impl, meta_Speed); }

			static ::Uml::Attribute meta_CPU;
			Udm::StringAttr CPU() const { return Udm::StringAttr(impl, meta_CPU); }

			static ::Uml::AssociationRole meta_referedbyNodeRef;
			Udm::AssocAttr< ::ESMoL::NodeRef> referedbyNodeRef() const { return Udm::AssocAttr< ::ESMoL::NodeRef>(impl, meta_referedbyNodeRef); }
			template <class Pred> Udm::AssocAttr< ::ESMoL::NodeRef, Pred > referedbyNodeRef_sorted(const Pred &) const { return Udm::AssocAttr< ::ESMoL::NodeRef, Pred>(impl, meta_referedbyNodeRef); }

			static ::Uml::CompositionChildRole meta_Channel_children;
			Udm::ChildrenAttr< ::ESMoL::Channel> Channel_children() const { return Udm::ChildrenAttr< ::ESMoL::Channel>(impl, meta_Channel_children); }
			template <class Pred> Udm::ChildrenAttr< ::ESMoL::Channel, Pred> Channel_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::Channel, Pred>(impl, meta_Channel_children); }

			static ::Uml::CompositionChildRole meta_OS_child;
			Udm::ChildAttr< ::ESMoL::OS> OS_child() const { return Udm::ChildAttr< ::ESMoL::OS>(impl, meta_OS_child); }

			Udm::ChildrenAttr< ::ESMoL::OS> OS_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::OS>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::OS, Pred> OS_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::OS, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::ESMoL::Connectable> Connectable_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::Connectable>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::Connectable, Pred> Connectable_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::Connectable, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::ESMoL::IChan> IChan_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::IChan>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::IChan, Pred> IChan_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::IChan, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::ESMoL::OChan> OChan_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::OChan>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::OChan, Pred> OChan_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::OChan, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::ESMoL::BChan> BChan_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::BChan>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::BChan, Pred> BChan_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::BChan, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::ESMoL::Channel> Channel_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::Channel>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::Channel, Pred> Channel_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::Channel, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::ESMoL::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::MgaObject>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ParentAttr< ::ESMoL::MgaObject> parent() const { return Udm::ParentAttr< ::ESMoL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  Channel :  public ::ESMoL::Connectable {
		public:
			static ::Uml::Class meta;

			Channel() { }
			Channel(Udm::ObjectImpl *impl) : ::ESMoL::Connectable(impl), ::ESMoL::MgaObject(impl) { }
			Channel(const Channel &master) : ::ESMoL::Connectable(master), ::ESMoL::MgaObject(master) { }
			static Channel Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static Channel Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			Channel CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			Channel CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::ESMoL::Channel> Instances() { return Udm::InstantiatedAttr< ::ESMoL::Channel>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::ESMoL::Channel, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::ESMoL::Channel, Pred>(impl);}

			Udm::DerivedAttr< ::ESMoL::Channel> Derived() { return Udm::DerivedAttr< ::ESMoL::Channel>(impl);}
			template <class Pred> Udm::DerivedAttr< ::ESMoL::Channel, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::ESMoL::Channel, Pred>(impl);}

			Udm::ArchetypeAttr< ::ESMoL::Channel> Archetype() { return Udm::ArchetypeAttr< ::ESMoL::Channel>(impl);}

			static ::Uml::Attribute meta_InterruptNum;
			Udm::StringAttr InterruptNum() const { return Udm::StringAttr(impl, meta_InterruptNum); }

			static ::Uml::AssociationRole meta_referedbyChannelRef;
			Udm::AssocAttr< ::ESMoL::ChannelRef> referedbyChannelRef() const { return Udm::AssocAttr< ::ESMoL::ChannelRef>(impl, meta_referedbyChannelRef); }
			template <class Pred> Udm::AssocAttr< ::ESMoL::ChannelRef, Pred > referedbyChannelRef_sorted(const Pred &) const { return Udm::AssocAttr< ::ESMoL::ChannelRef, Pred>(impl, meta_referedbyChannelRef); }

			static ::Uml::CompositionParentRole meta_HardwareUnit_parent;
			Udm::ParentAttr< ::ESMoL::HardwareUnit> HardwareUnit_parent() const { return Udm::ParentAttr< ::ESMoL::HardwareUnit>(impl, meta_HardwareUnit_parent); }

			static ::Uml::CompositionParentRole meta_Node_parent;
			Udm::ParentAttr< ::ESMoL::Node> Node_parent() const { return Udm::ParentAttr< ::ESMoL::Node>(impl, meta_Node_parent); }

			Udm::ParentAttr< ::ESMoL::MgaObject> parent() const { return Udm::ParentAttr< ::ESMoL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  IChan :  public ::ESMoL::Channel {
		public:
			static ::Uml::Class meta;

			IChan() { }
			IChan(Udm::ObjectImpl *impl) : ::ESMoL::Channel(impl), ::ESMoL::MgaObject(impl) { }
			IChan(const IChan &master) : ::ESMoL::Channel(master), ::ESMoL::MgaObject(master) { }
			static IChan Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static IChan Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			IChan CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			IChan CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::ESMoL::IChan> Instances() { return Udm::InstantiatedAttr< ::ESMoL::IChan>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::ESMoL::IChan, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::ESMoL::IChan, Pred>(impl);}

			Udm::DerivedAttr< ::ESMoL::IChan> Derived() { return Udm::DerivedAttr< ::ESMoL::IChan>(impl);}
			template <class Pred> Udm::DerivedAttr< ::ESMoL::IChan, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::ESMoL::IChan, Pred>(impl);}

			Udm::ArchetypeAttr< ::ESMoL::IChan> Archetype() { return Udm::ArchetypeAttr< ::ESMoL::IChan>(impl);}

			Udm::ParentAttr< ::ESMoL::MgaObject> parent() const { return Udm::ParentAttr< ::ESMoL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  OChan :  public ::ESMoL::Channel {
		public:
			static ::Uml::Class meta;

			OChan() { }
			OChan(Udm::ObjectImpl *impl) : ::ESMoL::Channel(impl), ::ESMoL::MgaObject(impl) { }
			OChan(const OChan &master) : ::ESMoL::Channel(master), ::ESMoL::MgaObject(master) { }
			static OChan Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static OChan Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			OChan CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			OChan CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::ESMoL::OChan> Instances() { return Udm::InstantiatedAttr< ::ESMoL::OChan>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::ESMoL::OChan, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::ESMoL::OChan, Pred>(impl);}

			Udm::DerivedAttr< ::ESMoL::OChan> Derived() { return Udm::DerivedAttr< ::ESMoL::OChan>(impl);}
			template <class Pred> Udm::DerivedAttr< ::ESMoL::OChan, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::ESMoL::OChan, Pred>(impl);}

			Udm::ArchetypeAttr< ::ESMoL::OChan> Archetype() { return Udm::ArchetypeAttr< ::ESMoL::OChan>(impl);}

			Udm::ParentAttr< ::ESMoL::MgaObject> parent() const { return Udm::ParentAttr< ::ESMoL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  BChan :  public ::ESMoL::Channel {
		public:
			static ::Uml::Class meta;

			BChan() { }
			BChan(Udm::ObjectImpl *impl) : ::ESMoL::Channel(impl), ::ESMoL::MgaObject(impl) { }
			BChan(const BChan &master) : ::ESMoL::Channel(master), ::ESMoL::MgaObject(master) { }
			static BChan Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static BChan Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			BChan CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			BChan CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::ESMoL::BChan> Instances() { return Udm::InstantiatedAttr< ::ESMoL::BChan>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::ESMoL::BChan, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::ESMoL::BChan, Pred>(impl);}

			Udm::DerivedAttr< ::ESMoL::BChan> Derived() { return Udm::DerivedAttr< ::ESMoL::BChan>(impl);}
			template <class Pred> Udm::DerivedAttr< ::ESMoL::BChan, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::ESMoL::BChan, Pred>(impl);}

			Udm::ArchetypeAttr< ::ESMoL::BChan> Archetype() { return Udm::ArchetypeAttr< ::ESMoL::BChan>(impl);}

			static ::Uml::CompositionParentRole meta_Router_parent;
			Udm::ParentAttr< ::ESMoL::Router> Router_parent() const { return Udm::ParentAttr< ::ESMoL::Router>(impl, meta_Router_parent); }

			Udm::ParentAttr< ::ESMoL::MgaObject> parent() const { return Udm::ParentAttr< ::ESMoL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  CommDst :  public ::ESMoL::MgaObject {
		public:
			static ::Uml::Class meta;

			CommDst() { }
			CommDst(Udm::ObjectImpl *impl) : ::ESMoL::MgaObject(impl) { }
			CommDst(const CommDst &master) : ::ESMoL::MgaObject(master) { }
			static CommDst Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static CommDst Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			CommDst CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			CommDst CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::ESMoL::CommDst> Instances() { return Udm::InstantiatedAttr< ::ESMoL::CommDst>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::ESMoL::CommDst, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::ESMoL::CommDst, Pred>(impl);}

			Udm::DerivedAttr< ::ESMoL::CommDst> Derived() { return Udm::DerivedAttr< ::ESMoL::CommDst>(impl);}
			template <class Pred> Udm::DerivedAttr< ::ESMoL::CommDst, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::ESMoL::CommDst, Pred>(impl);}

			Udm::ArchetypeAttr< ::ESMoL::CommDst> Archetype() { return Udm::ArchetypeAttr< ::ESMoL::CommDst>(impl);}

			static ::Uml::AssociationRole meta_srcOutCommMapping;
			static ::Uml::AssociationRole meta_srcOutCommMapping_rev;
			Udm::AClassAssocAttr< ::ESMoL::OutCommMapping, ::ESMoL::Output> srcOutCommMapping() const { return Udm::AClassAssocAttr< ::ESMoL::OutCommMapping, ::ESMoL::Output>(impl, meta_srcOutCommMapping, meta_srcOutCommMapping_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::ESMoL::OutCommMapping, ::ESMoL::Output, Pred> srcOutCommMapping_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::ESMoL::OutCommMapping, ::ESMoL::Output, Pred>(impl, meta_srcOutCommMapping, meta_srcOutCommMapping_rev); }

			static ::Uml::AssociationRole meta_dstInCommMapping;
			static ::Uml::AssociationRole meta_dstInCommMapping_rev;
			Udm::AClassAssocAttr< ::ESMoL::InCommMapping, ::ESMoL::Input> dstInCommMapping() const { return Udm::AClassAssocAttr< ::ESMoL::InCommMapping, ::ESMoL::Input>(impl, meta_dstInCommMapping, meta_dstInCommMapping_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::ESMoL::InCommMapping, ::ESMoL::Input, Pred> dstInCommMapping_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::ESMoL::InCommMapping, ::ESMoL::Input, Pred>(impl, meta_dstInCommMapping, meta_dstInCommMapping_rev); }

			static ::Uml::AssociationRole meta_dstMsgAssignment;
			static ::Uml::AssociationRole meta_dstMsgAssignment_rev;
			Udm::AClassAssocAttr< ::ESMoL::MsgAssignment, ::ESMoL::ChannelRef> dstMsgAssignment() const { return Udm::AClassAssocAttr< ::ESMoL::MsgAssignment, ::ESMoL::ChannelRef>(impl, meta_dstMsgAssignment, meta_dstMsgAssignment_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::ESMoL::MsgAssignment, ::ESMoL::ChannelRef, Pred> dstMsgAssignment_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::ESMoL::MsgAssignment, ::ESMoL::ChannelRef, Pred>(impl, meta_dstMsgAssignment, meta_dstMsgAssignment_rev); }

			static ::Uml::CompositionParentRole meta_Task_parent;
			Udm::ParentAttr< ::ESMoL::Task> Task_parent() const { return Udm::ParentAttr< ::ESMoL::Task>(impl, meta_Task_parent); }

			Udm::ParentAttr< ::ESMoL::MgaObject> parent() const { return Udm::ParentAttr< ::ESMoL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  CommMapping :  public ::ESMoL::MgaObject {
		public:
			static ::Uml::Class meta;

			CommMapping() { }
			CommMapping(Udm::ObjectImpl *impl) : ::ESMoL::MgaObject(impl) { }
			CommMapping(const CommMapping &master) : ::ESMoL::MgaObject(master) { }
			static CommMapping Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static CommMapping Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			CommMapping CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			CommMapping CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::ESMoL::CommMapping> Instances() { return Udm::InstantiatedAttr< ::ESMoL::CommMapping>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::ESMoL::CommMapping, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::ESMoL::CommMapping, Pred>(impl);}

			Udm::DerivedAttr< ::ESMoL::CommMapping> Derived() { return Udm::DerivedAttr< ::ESMoL::CommMapping>(impl);}
			template <class Pred> Udm::DerivedAttr< ::ESMoL::CommMapping, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::ESMoL::CommMapping, Pred>(impl);}

			Udm::ArchetypeAttr< ::ESMoL::CommMapping> Archetype() { return Udm::ArchetypeAttr< ::ESMoL::CommMapping>(impl);}

			static ::Uml::CompositionParentRole meta_Task_parent;
			Udm::ParentAttr< ::ESMoL::Task> Task_parent() const { return Udm::ParentAttr< ::ESMoL::Task>(impl, meta_Task_parent); }

			Udm::ParentAttr< ::ESMoL::MgaObject> parent() const { return Udm::ParentAttr< ::ESMoL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  OutCommMapping :  public ::ESMoL::CommMapping {
		public:
			static ::Uml::Class meta;

			OutCommMapping() { }
			OutCommMapping(Udm::ObjectImpl *impl) : ::ESMoL::CommMapping(impl) { }
			OutCommMapping(const OutCommMapping &master) : ::ESMoL::CommMapping(master) { }
			static OutCommMapping Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static OutCommMapping Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			OutCommMapping CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			OutCommMapping CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::ESMoL::OutCommMapping> Instances() { return Udm::InstantiatedAttr< ::ESMoL::OutCommMapping>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::ESMoL::OutCommMapping, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::ESMoL::OutCommMapping, Pred>(impl);}

			Udm::DerivedAttr< ::ESMoL::OutCommMapping> Derived() { return Udm::DerivedAttr< ::ESMoL::OutCommMapping>(impl);}
			template <class Pred> Udm::DerivedAttr< ::ESMoL::OutCommMapping, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::ESMoL::OutCommMapping, Pred>(impl);}

			Udm::ArchetypeAttr< ::ESMoL::OutCommMapping> Archetype() { return Udm::ArchetypeAttr< ::ESMoL::OutCommMapping>(impl);}

			Udm::ParentAttr< ::ESMoL::MgaObject> parent() const { return Udm::ParentAttr< ::ESMoL::MgaObject>(impl, Udm::NULLPARENTROLE); }
			static ::Uml::AssociationRole meta_srcOutCommMapping_end_;
			Udm::AssocEndAttr< ::ESMoL::Output> srcOutCommMapping_end() const { return Udm::AssocEndAttr< ::ESMoL::Output>(impl, meta_srcOutCommMapping_end_); }

			static ::Uml::AssociationRole meta_dstOutCommMapping_end_;
			Udm::AssocEndAttr< ::ESMoL::CommDst> dstOutCommMapping_end() const { return Udm::AssocEndAttr< ::ESMoL::CommDst>(impl, meta_dstOutCommMapping_end_); }

		};

		class  InCommMapping :  public ::ESMoL::CommMapping {
		public:
			static ::Uml::Class meta;

			InCommMapping() { }
			InCommMapping(Udm::ObjectImpl *impl) : ::ESMoL::CommMapping(impl) { }
			InCommMapping(const InCommMapping &master) : ::ESMoL::CommMapping(master) { }
			static InCommMapping Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static InCommMapping Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			InCommMapping CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			InCommMapping CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::ESMoL::InCommMapping> Instances() { return Udm::InstantiatedAttr< ::ESMoL::InCommMapping>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::ESMoL::InCommMapping, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::ESMoL::InCommMapping, Pred>(impl);}

			Udm::DerivedAttr< ::ESMoL::InCommMapping> Derived() { return Udm::DerivedAttr< ::ESMoL::InCommMapping>(impl);}
			template <class Pred> Udm::DerivedAttr< ::ESMoL::InCommMapping, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::ESMoL::InCommMapping, Pred>(impl);}

			Udm::ArchetypeAttr< ::ESMoL::InCommMapping> Archetype() { return Udm::ArchetypeAttr< ::ESMoL::InCommMapping>(impl);}

			Udm::ParentAttr< ::ESMoL::MgaObject> parent() const { return Udm::ParentAttr< ::ESMoL::MgaObject>(impl, Udm::NULLPARENTROLE); }
			static ::Uml::AssociationRole meta_dstInCommMapping_end_;
			Udm::AssocEndAttr< ::ESMoL::Input> dstInCommMapping_end() const { return Udm::AssocEndAttr< ::ESMoL::Input>(impl, meta_dstInCommMapping_end_); }

			static ::Uml::AssociationRole meta_srcInCommMapping_end_;
			Udm::AssocEndAttr< ::ESMoL::CommDst> srcInCommMapping_end() const { return Udm::AssocEndAttr< ::ESMoL::CommDst>(impl, meta_srcInCommMapping_end_); }

		};

		class  Component :  public ::ESMoL::MgaObject {
		public:
			static ::Uml::Class meta;

			Component() { }
			Component(Udm::ObjectImpl *impl) : ::ESMoL::MgaObject(impl) { }
			Component(const Component &master) : ::ESMoL::MgaObject(master) { }
			static Component Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static Component Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			Component CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			Component CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::ESMoL::Component> Instances() { return Udm::InstantiatedAttr< ::ESMoL::Component>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::ESMoL::Component, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::ESMoL::Component, Pred>(impl);}

			Udm::DerivedAttr< ::ESMoL::Component> Derived() { return Udm::DerivedAttr< ::ESMoL::Component>(impl);}
			template <class Pred> Udm::DerivedAttr< ::ESMoL::Component, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::ESMoL::Component, Pred>(impl);}

			Udm::ArchetypeAttr< ::ESMoL::Component> Archetype() { return Udm::ArchetypeAttr< ::ESMoL::Component>(impl);}

			static ::Uml::Attribute meta_WCET;
			Udm::StringAttr WCET() const { return Udm::StringAttr(impl, meta_WCET); }

			static ::Uml::AssociationRole meta_ref;
			Udm::PointerAttr< ::ESMoL::ComponentBase> ref() const { return Udm::PointerAttr< ::ESMoL::ComponentBase>(impl, meta_ref); }

			static ::Uml::CompositionParentRole meta_Task_parent;
			Udm::ParentAttr< ::ESMoL::Task> Task_parent() const { return Udm::ParentAttr< ::ESMoL::Task>(impl, meta_Task_parent); }

			Udm::ParentAttr< ::ESMoL::MgaObject> parent() const { return Udm::ParentAttr< ::ESMoL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  Message :  public ::ESMoL::CommDst {
		public:
			static ::Uml::Class meta;

			Message() { }
			Message(Udm::ObjectImpl *impl) : ::ESMoL::CommDst(impl) { }
			Message(const Message &master) : ::ESMoL::CommDst(master) { }
			static Message Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static Message Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			Message CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			Message CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::ESMoL::Message> Instances() { return Udm::InstantiatedAttr< ::ESMoL::Message>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::ESMoL::Message, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::ESMoL::Message, Pred>(impl);}

			Udm::DerivedAttr< ::ESMoL::Message> Derived() { return Udm::DerivedAttr< ::ESMoL::Message>(impl);}
			template <class Pred> Udm::DerivedAttr< ::ESMoL::Message, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::ESMoL::Message, Pred>(impl);}

			Udm::ArchetypeAttr< ::ESMoL::Message> Archetype() { return Udm::ArchetypeAttr< ::ESMoL::Message>(impl);}

			static ::Uml::Attribute meta_MsgSize;
			Udm::StringAttr MsgSize() const { return Udm::StringAttr(impl, meta_MsgSize); }

			static ::Uml::Attribute meta_MsgMetaData;
			Udm::StringAttr MsgMetaData() const { return Udm::StringAttr(impl, meta_MsgMetaData); }

			static ::Uml::AssociationRole meta_referedbyBusMessageRef;
			Udm::AssocAttr< ::ESMoL::MessageRef> referedbyBusMessageRef() const { return Udm::AssocAttr< ::ESMoL::MessageRef>(impl, meta_referedbyBusMessageRef); }
			template <class Pred> Udm::AssocAttr< ::ESMoL::MessageRef, Pred > referedbyBusMessageRef_sorted(const Pred &) const { return Udm::AssocAttr< ::ESMoL::MessageRef, Pred>(impl, meta_referedbyBusMessageRef); }

			Udm::ParentAttr< ::ESMoL::MgaObject> parent() const { return Udm::ParentAttr< ::ESMoL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  TTMessage :  public ::ESMoL::Message {
		public:
			static ::Uml::Class meta;

			TTMessage() { }
			TTMessage(Udm::ObjectImpl *impl) : ::ESMoL::Message(impl) { }
			TTMessage(const TTMessage &master) : ::ESMoL::Message(master) { }
			static TTMessage Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static TTMessage Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			TTMessage CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			TTMessage CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::ESMoL::TTMessage> Instances() { return Udm::InstantiatedAttr< ::ESMoL::TTMessage>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::ESMoL::TTMessage, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::ESMoL::TTMessage, Pred>(impl);}

			Udm::DerivedAttr< ::ESMoL::TTMessage> Derived() { return Udm::DerivedAttr< ::ESMoL::TTMessage>(impl);}
			template <class Pred> Udm::DerivedAttr< ::ESMoL::TTMessage, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::ESMoL::TTMessage, Pred>(impl);}

			Udm::ArchetypeAttr< ::ESMoL::TTMessage> Archetype() { return Udm::ArchetypeAttr< ::ESMoL::TTMessage>(impl);}

			static ::Uml::Attribute meta_TTSchedule;
			Udm::StringAttr TTSchedule() const { return Udm::StringAttr(impl, meta_TTSchedule); }

			Udm::ParentAttr< ::ESMoL::MgaObject> parent() const { return Udm::ParentAttr< ::ESMoL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  MessageRef :  public ::ESMoL::CommDst {
		public:
			static ::Uml::Class meta;

			MessageRef() { }
			MessageRef(Udm::ObjectImpl *impl) : ::ESMoL::CommDst(impl) { }
			MessageRef(const MessageRef &master) : ::ESMoL::CommDst(master) { }
			static MessageRef Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static MessageRef Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			MessageRef CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			MessageRef CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::ESMoL::MessageRef> Instances() { return Udm::InstantiatedAttr< ::ESMoL::MessageRef>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::ESMoL::MessageRef, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::ESMoL::MessageRef, Pred>(impl);}

			Udm::DerivedAttr< ::ESMoL::MessageRef> Derived() { return Udm::DerivedAttr< ::ESMoL::MessageRef>(impl);}
			template <class Pred> Udm::DerivedAttr< ::ESMoL::MessageRef, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::ESMoL::MessageRef, Pred>(impl);}

			Udm::ArchetypeAttr< ::ESMoL::MessageRef> Archetype() { return Udm::ArchetypeAttr< ::ESMoL::MessageRef>(impl);}

			static ::Uml::AssociationRole meta_ref;
			Udm::PointerAttr< ::ESMoL::Message> ref() const { return Udm::PointerAttr< ::ESMoL::Message>(impl, meta_ref); }

			Udm::ParentAttr< ::ESMoL::MgaObject> parent() const { return Udm::ParentAttr< ::ESMoL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  IOPort :  virtual public ::ESMoL::MgaObject {
		public:
			static ::Uml::Class meta;

			IOPort() { }
			IOPort(Udm::ObjectImpl *impl) : ::ESMoL::MgaObject(impl) { }
			IOPort(const IOPort &master) : ::ESMoL::MgaObject(master) { }
			static IOPort Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static IOPort Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			IOPort CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			IOPort CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::ESMoL::IOPort> Instances() { return Udm::InstantiatedAttr< ::ESMoL::IOPort>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::ESMoL::IOPort, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::ESMoL::IOPort, Pred>(impl);}

			Udm::DerivedAttr< ::ESMoL::IOPort> Derived() { return Udm::DerivedAttr< ::ESMoL::IOPort>(impl);}
			template <class Pred> Udm::DerivedAttr< ::ESMoL::IOPort, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::ESMoL::IOPort, Pred>(impl);}

			Udm::ArchetypeAttr< ::ESMoL::IOPort> Archetype() { return Udm::ArchetypeAttr< ::ESMoL::IOPort>(impl);}

			Udm::ParentAttr<Udm::Object> parent() const { return Udm::ParentAttr<Udm::Object>(impl, Udm::NULLPARENTROLE); }
		};

		class  Output :  public ::ESMoL::IOPortExp, public ::ESMoL::IOPort {
		public:
			static ::Uml::Class meta;

			Output() { }
			Output(Udm::ObjectImpl *impl) : ::ESMoL::IOPortExp(impl),::ESMoL::IOPort(impl), ::ESMoL::MgaObject(impl) { }
			Output(const Output &master) : ::ESMoL::IOPortExp(master),::ESMoL::IOPort(master), ::ESMoL::MgaObject(master) { }
			static Output Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static Output Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			Output CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			Output CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::ESMoL::Output> Instances() { return Udm::InstantiatedAttr< ::ESMoL::Output>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::ESMoL::Output, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::ESMoL::Output, Pred>(impl);}

			Udm::DerivedAttr< ::ESMoL::Output> Derived() { return Udm::DerivedAttr< ::ESMoL::Output>(impl);}
			template <class Pred> Udm::DerivedAttr< ::ESMoL::Output, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::ESMoL::Output, Pred>(impl);}

			Udm::ArchetypeAttr< ::ESMoL::Output> Archetype() { return Udm::ArchetypeAttr< ::ESMoL::Output>(impl);}

			static ::Uml::AssociationRole meta_dstOutCommMapping;
			static ::Uml::AssociationRole meta_dstOutCommMapping_rev;
			Udm::AClassAssocAttr< ::ESMoL::OutCommMapping, ::ESMoL::CommDst> dstOutCommMapping() const { return Udm::AClassAssocAttr< ::ESMoL::OutCommMapping, ::ESMoL::CommDst>(impl, meta_dstOutCommMapping, meta_dstOutCommMapping_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::ESMoL::OutCommMapping, ::ESMoL::CommDst, Pred> dstOutCommMapping_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::ESMoL::OutCommMapping, ::ESMoL::CommDst, Pred>(impl, meta_dstOutCommMapping, meta_dstOutCommMapping_rev); }

			static ::Uml::AssociationRole meta_dstConnector;
			static ::Uml::AssociationRole meta_dstConnector_rev;
			Udm::AClassAssocAttr< ::ESMoL::Connector, ::ESMoL::Input> dstConnector() const { return Udm::AClassAssocAttr< ::ESMoL::Connector, ::ESMoL::Input>(impl, meta_dstConnector, meta_dstConnector_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::ESMoL::Connector, ::ESMoL::Input, Pred> dstConnector_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::ESMoL::Connector, ::ESMoL::Input, Pred>(impl, meta_dstConnector, meta_dstConnector_rev); }

			Udm::ParentAttr<Udm::Object> parent() const { return Udm::ParentAttr<Udm::Object>(impl, Udm::NULLPARENTROLE); }
		};

		class  COutputPort :  public ::ESMoL::Output {
		public:
			static ::Uml::Class meta;

			COutputPort() { }
			COutputPort(Udm::ObjectImpl *impl) : ::ESMoL::Output(impl), ::ESMoL::MgaObject(impl) { }
			COutputPort(const COutputPort &master) : ::ESMoL::Output(master), ::ESMoL::MgaObject(master) { }
			static COutputPort Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static COutputPort Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			COutputPort CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			COutputPort CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::ESMoL::COutputPort> Instances() { return Udm::InstantiatedAttr< ::ESMoL::COutputPort>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::ESMoL::COutputPort, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::ESMoL::COutputPort, Pred>(impl);}

			Udm::DerivedAttr< ::ESMoL::COutputPort> Derived() { return Udm::DerivedAttr< ::ESMoL::COutputPort>(impl);}
			template <class Pred> Udm::DerivedAttr< ::ESMoL::COutputPort, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::ESMoL::COutputPort, Pred>(impl);}

			Udm::ArchetypeAttr< ::ESMoL::COutputPort> Archetype() { return Udm::ArchetypeAttr< ::ESMoL::COutputPort>(impl);}

			static ::Uml::CompositionParentRole meta_CCode_parent;
			Udm::ParentAttr< ::ESMoL::CCode> CCode_parent() const { return Udm::ParentAttr< ::ESMoL::CCode>(impl, meta_CCode_parent); }

			Udm::ParentAttr< ::ESMoL::MgaObject> parent() const { return Udm::ParentAttr< ::ESMoL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  Input :  public ::ESMoL::IOPortExp, public ::ESMoL::IOPort {
		public:
			static ::Uml::Class meta;

			Input() { }
			Input(Udm::ObjectImpl *impl) : ::ESMoL::IOPortExp(impl),::ESMoL::IOPort(impl), ::ESMoL::MgaObject(impl) { }
			Input(const Input &master) : ::ESMoL::IOPortExp(master),::ESMoL::IOPort(master), ::ESMoL::MgaObject(master) { }
			static Input Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static Input Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			Input CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			Input CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::ESMoL::Input> Instances() { return Udm::InstantiatedAttr< ::ESMoL::Input>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::ESMoL::Input, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::ESMoL::Input, Pred>(impl);}

			Udm::DerivedAttr< ::ESMoL::Input> Derived() { return Udm::DerivedAttr< ::ESMoL::Input>(impl);}
			template <class Pred> Udm::DerivedAttr< ::ESMoL::Input, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::ESMoL::Input, Pred>(impl);}

			Udm::ArchetypeAttr< ::ESMoL::Input> Archetype() { return Udm::ArchetypeAttr< ::ESMoL::Input>(impl);}

			static ::Uml::AssociationRole meta_srcConnector;
			static ::Uml::AssociationRole meta_srcConnector_rev;
			Udm::AClassAssocAttr< ::ESMoL::Connector, ::ESMoL::Output> srcConnector() const { return Udm::AClassAssocAttr< ::ESMoL::Connector, ::ESMoL::Output>(impl, meta_srcConnector, meta_srcConnector_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::ESMoL::Connector, ::ESMoL::Output, Pred> srcConnector_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::ESMoL::Connector, ::ESMoL::Output, Pred>(impl, meta_srcConnector, meta_srcConnector_rev); }

			static ::Uml::AssociationRole meta_srcInCommMapping;
			static ::Uml::AssociationRole meta_srcInCommMapping_rev;
			Udm::AClassAssocAttr< ::ESMoL::InCommMapping, ::ESMoL::CommDst> srcInCommMapping() const { return Udm::AClassAssocAttr< ::ESMoL::InCommMapping, ::ESMoL::CommDst>(impl, meta_srcInCommMapping, meta_srcInCommMapping_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::ESMoL::InCommMapping, ::ESMoL::CommDst, Pred> srcInCommMapping_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::ESMoL::InCommMapping, ::ESMoL::CommDst, Pred>(impl, meta_srcInCommMapping, meta_srcInCommMapping_rev); }

			Udm::ParentAttr<Udm::Object> parent() const { return Udm::ParentAttr<Udm::Object>(impl, Udm::NULLPARENTROLE); }
		};

		class  CInputPort :  public ::ESMoL::Input {
		public:
			static ::Uml::Class meta;

			CInputPort() { }
			CInputPort(Udm::ObjectImpl *impl) : ::ESMoL::Input(impl), ::ESMoL::MgaObject(impl) { }
			CInputPort(const CInputPort &master) : ::ESMoL::Input(master), ::ESMoL::MgaObject(master) { }
			static CInputPort Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static CInputPort Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			CInputPort CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			CInputPort CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::ESMoL::CInputPort> Instances() { return Udm::InstantiatedAttr< ::ESMoL::CInputPort>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::ESMoL::CInputPort, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::ESMoL::CInputPort, Pred>(impl);}

			Udm::DerivedAttr< ::ESMoL::CInputPort> Derived() { return Udm::DerivedAttr< ::ESMoL::CInputPort>(impl);}
			template <class Pred> Udm::DerivedAttr< ::ESMoL::CInputPort, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::ESMoL::CInputPort, Pred>(impl);}

			Udm::ArchetypeAttr< ::ESMoL::CInputPort> Archetype() { return Udm::ArchetypeAttr< ::ESMoL::CInputPort>(impl);}

			static ::Uml::CompositionParentRole meta_CCode_parent;
			Udm::ParentAttr< ::ESMoL::CCode> CCode_parent() const { return Udm::ParentAttr< ::ESMoL::CCode>(impl, meta_CCode_parent); }

			Udm::ParentAttr< ::ESMoL::MgaObject> parent() const { return Udm::ParentAttr< ::ESMoL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  OutputPort :  public ::ESMoL::Output, public ::ESMoL::OutPort {
		public:
			static ::Uml::Class meta;

			OutputPort() { }
			OutputPort(Udm::ObjectImpl *impl) : ::ESMoL::Output(impl),::ESMoL::OutPort(impl), ::ESMoL::MgaObject(impl) { }
			OutputPort(const OutputPort &master) : ::ESMoL::Output(master),::ESMoL::OutPort(master), ::ESMoL::MgaObject(master) { }
			static OutputPort Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static OutputPort Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			OutputPort CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			OutputPort CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::ESMoL::OutputPort> Instances() { return Udm::InstantiatedAttr< ::ESMoL::OutputPort>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::ESMoL::OutputPort, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::ESMoL::OutputPort, Pred>(impl);}

			Udm::DerivedAttr< ::ESMoL::OutputPort> Derived() { return Udm::DerivedAttr< ::ESMoL::OutputPort>(impl);}
			template <class Pred> Udm::DerivedAttr< ::ESMoL::OutputPort, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::ESMoL::OutputPort, Pred>(impl);}

			Udm::ArchetypeAttr< ::ESMoL::OutputPort> Archetype() { return Udm::ArchetypeAttr< ::ESMoL::OutputPort>(impl);}

			static ::Uml::Attribute meta_Number;
			Udm::IntegerAttr Number() const { return Udm::IntegerAttr(impl, meta_Number); }

			Udm::ParentAttr< ::ESMoL::MgaObject> parent() const { return Udm::ParentAttr< ::ESMoL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  InputPort :  public ::ESMoL::Input, public ::ESMoL::InPort {
		public:
			static ::Uml::Class meta;

			InputPort() { }
			InputPort(Udm::ObjectImpl *impl) : ::ESMoL::Input(impl),::ESMoL::InPort(impl), ::ESMoL::MgaObject(impl) { }
			InputPort(const InputPort &master) : ::ESMoL::Input(master),::ESMoL::InPort(master), ::ESMoL::MgaObject(master) { }
			static InputPort Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static InputPort Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			InputPort CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			InputPort CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::ESMoL::InputPort> Instances() { return Udm::InstantiatedAttr< ::ESMoL::InputPort>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::ESMoL::InputPort, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::ESMoL::InputPort, Pred>(impl);}

			Udm::DerivedAttr< ::ESMoL::InputPort> Derived() { return Udm::DerivedAttr< ::ESMoL::InputPort>(impl);}
			template <class Pred> Udm::DerivedAttr< ::ESMoL::InputPort, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::ESMoL::InputPort, Pred>(impl);}

			Udm::ArchetypeAttr< ::ESMoL::InputPort> Archetype() { return Udm::ArchetypeAttr< ::ESMoL::InputPort>(impl);}

			static ::Uml::Attribute meta_Number;
			Udm::IntegerAttr Number() const { return Udm::IntegerAttr(impl, meta_Number); }

			Udm::ParentAttr< ::ESMoL::MgaObject> parent() const { return Udm::ParentAttr< ::ESMoL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  MsgAssignment :  public ::ESMoL::MgaObject {
		public:
			static ::Uml::Class meta;

			MsgAssignment() { }
			MsgAssignment(Udm::ObjectImpl *impl) : ::ESMoL::MgaObject(impl) { }
			MsgAssignment(const MsgAssignment &master) : ::ESMoL::MgaObject(master) { }
			static MsgAssignment Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static MsgAssignment Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			MsgAssignment CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			MsgAssignment CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::ESMoL::MsgAssignment> Instances() { return Udm::InstantiatedAttr< ::ESMoL::MsgAssignment>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::ESMoL::MsgAssignment, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::ESMoL::MsgAssignment, Pred>(impl);}

			Udm::DerivedAttr< ::ESMoL::MsgAssignment> Derived() { return Udm::DerivedAttr< ::ESMoL::MsgAssignment>(impl);}
			template <class Pred> Udm::DerivedAttr< ::ESMoL::MsgAssignment, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::ESMoL::MsgAssignment, Pred>(impl);}

			Udm::ArchetypeAttr< ::ESMoL::MsgAssignment> Archetype() { return Udm::ArchetypeAttr< ::ESMoL::MsgAssignment>(impl);}

			static ::Uml::CompositionParentRole meta_Task_parent;
			Udm::ParentAttr< ::ESMoL::Task> Task_parent() const { return Udm::ParentAttr< ::ESMoL::Task>(impl, meta_Task_parent); }

			Udm::ParentAttr< ::ESMoL::MgaObject> parent() const { return Udm::ParentAttr< ::ESMoL::MgaObject>(impl, Udm::NULLPARENTROLE); }
			static ::Uml::AssociationRole meta_srcMsgAssignment_end_;
			Udm::AssocEndAttr< ::ESMoL::CommDst> srcMsgAssignment_end() const { return Udm::AssocEndAttr< ::ESMoL::CommDst>(impl, meta_srcMsgAssignment_end_); }

			static ::Uml::AssociationRole meta_dstMsgAssignment_end_;
			Udm::AssocEndAttr< ::ESMoL::ChannelRef> dstMsgAssignment_end() const { return Udm::AssocEndAttr< ::ESMoL::ChannelRef>(impl, meta_dstMsgAssignment_end_); }

		};

		class  TaskAssignment :  public ::ESMoL::MgaObject {
		public:
			static ::Uml::Class meta;

			TaskAssignment() { }
			TaskAssignment(Udm::ObjectImpl *impl) : ::ESMoL::MgaObject(impl) { }
			TaskAssignment(const TaskAssignment &master) : ::ESMoL::MgaObject(master) { }
			static TaskAssignment Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static TaskAssignment Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			TaskAssignment CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			TaskAssignment CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::ESMoL::TaskAssignment> Instances() { return Udm::InstantiatedAttr< ::ESMoL::TaskAssignment>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::ESMoL::TaskAssignment, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::ESMoL::TaskAssignment, Pred>(impl);}

			Udm::DerivedAttr< ::ESMoL::TaskAssignment> Derived() { return Udm::DerivedAttr< ::ESMoL::TaskAssignment>(impl);}
			template <class Pred> Udm::DerivedAttr< ::ESMoL::TaskAssignment, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::ESMoL::TaskAssignment, Pred>(impl);}

			Udm::ArchetypeAttr< ::ESMoL::TaskAssignment> Archetype() { return Udm::ArchetypeAttr< ::ESMoL::TaskAssignment>(impl);}

			static ::Uml::CompositionParentRole meta_Deployment_parent;
			Udm::ParentAttr< ::ESMoL::Deployment> Deployment_parent() const { return Udm::ParentAttr< ::ESMoL::Deployment>(impl, meta_Deployment_parent); }

			Udm::ParentAttr< ::ESMoL::MgaObject> parent() const { return Udm::ParentAttr< ::ESMoL::MgaObject>(impl, Udm::NULLPARENTROLE); }
			static ::Uml::AssociationRole meta_srcTaskAssignment_end_;
			Udm::AssocEndAttr< ::ESMoL::Task> srcTaskAssignment_end() const { return Udm::AssocEndAttr< ::ESMoL::Task>(impl, meta_srcTaskAssignment_end_); }

			static ::Uml::AssociationRole meta_dstTaskAssignment_end_;
			Udm::AssocEndAttr< ::ESMoL::NodeRef> dstTaskAssignment_end() const { return Udm::AssocEndAttr< ::ESMoL::NodeRef>(impl, meta_dstTaskAssignment_end_); }

		};

		class  Deployment :  public ::ESMoL::MgaObject {
		public:
			static ::Uml::Class meta;

			Deployment() { }
			Deployment(Udm::ObjectImpl *impl) : ::ESMoL::MgaObject(impl) { }
			Deployment(const Deployment &master) : ::ESMoL::MgaObject(master) { }
			static Deployment Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static Deployment Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			Deployment CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			Deployment CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::ESMoL::Deployment> Instances() { return Udm::InstantiatedAttr< ::ESMoL::Deployment>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::ESMoL::Deployment, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::ESMoL::Deployment, Pred>(impl);}

			Udm::DerivedAttr< ::ESMoL::Deployment> Derived() { return Udm::DerivedAttr< ::ESMoL::Deployment>(impl);}
			template <class Pred> Udm::DerivedAttr< ::ESMoL::Deployment, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::ESMoL::Deployment, Pred>(impl);}

			Udm::ArchetypeAttr< ::ESMoL::Deployment> Archetype() { return Udm::ArchetypeAttr< ::ESMoL::Deployment>(impl);}

			static ::Uml::CompositionChildRole meta_Task_children;
			Udm::ChildrenAttr< ::ESMoL::Task> Task_children() const { return Udm::ChildrenAttr< ::ESMoL::Task>(impl, meta_Task_children); }
			template <class Pred> Udm::ChildrenAttr< ::ESMoL::Task, Pred> Task_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::Task, Pred>(impl, meta_Task_children); }

			static ::Uml::CompositionChildRole meta_TaskAssignment_children;
			Udm::ChildrenAttr< ::ESMoL::TaskAssignment> TaskAssignment_children() const { return Udm::ChildrenAttr< ::ESMoL::TaskAssignment>(impl, meta_TaskAssignment_children); }
			template <class Pred> Udm::ChildrenAttr< ::ESMoL::TaskAssignment, Pred> TaskAssignment_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::TaskAssignment, Pred>(impl, meta_TaskAssignment_children); }

			static ::Uml::CompositionChildRole meta_NodeRef_children;
			Udm::ChildrenAttr< ::ESMoL::NodeRef> NodeRef_children() const { return Udm::ChildrenAttr< ::ESMoL::NodeRef>(impl, meta_NodeRef_children); }
			template <class Pred> Udm::ChildrenAttr< ::ESMoL::NodeRef, Pred> NodeRef_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::NodeRef, Pred>(impl, meta_NodeRef_children); }

			Udm::ChildrenAttr< ::ESMoL::TTTask> TTTask_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::TTTask>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::TTTask, Pred> TTTask_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::TTTask, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::ESMoL::OldTask> OldTask_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::OldTask>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::OldTask, Pred> OldTask_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::OldTask, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::ESMoL::TaskAssignment> TaskAssignment_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::TaskAssignment>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::TaskAssignment, Pred> TaskAssignment_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::TaskAssignment, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::ESMoL::Task> Task_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::Task>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::Task, Pred> Task_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::Task, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::ESMoL::NodeRef> NodeRef_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::NodeRef>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::NodeRef, Pred> NodeRef_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::NodeRef, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::ESMoL::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::MgaObject>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

			static ::Uml::CompositionParentRole meta_DesignFolder_parent;
			Udm::ParentAttr< ::ESMoL::DesignFolder> DesignFolder_parent() const { return Udm::ParentAttr< ::ESMoL::DesignFolder>(impl, meta_DesignFolder_parent); }

			static ::Uml::CompositionParentRole meta_DeploymentLibrary_parent;
			Udm::ParentAttr< ::ESMoL::DeploymentLibrary> DeploymentLibrary_parent() const { return Udm::ParentAttr< ::ESMoL::DeploymentLibrary>(impl, meta_DeploymentLibrary_parent); }

			Udm::ParentAttr<Udm::Object> parent() const { return Udm::ParentAttr<Udm::Object>(impl, Udm::NULLPARENTROLE); }
		};

		class  Task :  public ::ESMoL::MgaObject {
		public:
			static ::Uml::Class meta;

			Task() { }
			Task(Udm::ObjectImpl *impl) : ::ESMoL::MgaObject(impl) { }
			Task(const Task &master) : ::ESMoL::MgaObject(master) { }
			static Task Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static Task Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			Task CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			Task CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::ESMoL::Task> Instances() { return Udm::InstantiatedAttr< ::ESMoL::Task>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::ESMoL::Task, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::ESMoL::Task, Pred>(impl);}

			Udm::DerivedAttr< ::ESMoL::Task> Derived() { return Udm::DerivedAttr< ::ESMoL::Task>(impl);}
			template <class Pred> Udm::DerivedAttr< ::ESMoL::Task, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::ESMoL::Task, Pred>(impl);}

			Udm::ArchetypeAttr< ::ESMoL::Task> Archetype() { return Udm::ArchetypeAttr< ::ESMoL::Task>(impl);}

			static ::Uml::AssociationRole meta_referedbyTaskRef;
			Udm::AssocAttr< ::ESMoL::TaskRef> referedbyTaskRef() const { return Udm::AssocAttr< ::ESMoL::TaskRef>(impl, meta_referedbyTaskRef); }
			template <class Pred> Udm::AssocAttr< ::ESMoL::TaskRef, Pred > referedbyTaskRef_sorted(const Pred &) const { return Udm::AssocAttr< ::ESMoL::TaskRef, Pred>(impl, meta_referedbyTaskRef); }

			static ::Uml::AssociationRole meta_dstTaskAssignment;
			static ::Uml::AssociationRole meta_dstTaskAssignment_rev;
			Udm::AClassAssocAttr< ::ESMoL::TaskAssignment, ::ESMoL::NodeRef> dstTaskAssignment() const { return Udm::AClassAssocAttr< ::ESMoL::TaskAssignment, ::ESMoL::NodeRef>(impl, meta_dstTaskAssignment, meta_dstTaskAssignment_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::ESMoL::TaskAssignment, ::ESMoL::NodeRef, Pred> dstTaskAssignment_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::ESMoL::TaskAssignment, ::ESMoL::NodeRef, Pred>(impl, meta_dstTaskAssignment, meta_dstTaskAssignment_rev); }

			static ::Uml::CompositionChildRole meta_Connector_children;
			Udm::ChildrenAttr< ::ESMoL::Connector> Connector_children() const { return Udm::ChildrenAttr< ::ESMoL::Connector>(impl, meta_Connector_children); }
			template <class Pred> Udm::ChildrenAttr< ::ESMoL::Connector, Pred> Connector_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::Connector, Pred>(impl, meta_Connector_children); }

			static ::Uml::CompositionChildRole meta_Component_children;
			Udm::ChildrenAttr< ::ESMoL::Component> Component_children() const { return Udm::ChildrenAttr< ::ESMoL::Component>(impl, meta_Component_children); }
			template <class Pred> Udm::ChildrenAttr< ::ESMoL::Component, Pred> Component_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::Component, Pred>(impl, meta_Component_children); }

			static ::Uml::CompositionChildRole meta_CommDst_children;
			Udm::ChildrenAttr< ::ESMoL::CommDst> CommDst_children() const { return Udm::ChildrenAttr< ::ESMoL::CommDst>(impl, meta_CommDst_children); }
			template <class Pred> Udm::ChildrenAttr< ::ESMoL::CommDst, Pred> CommDst_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::CommDst, Pred>(impl, meta_CommDst_children); }

			static ::Uml::CompositionChildRole meta_CommMapping_children;
			Udm::ChildrenAttr< ::ESMoL::CommMapping> CommMapping_children() const { return Udm::ChildrenAttr< ::ESMoL::CommMapping>(impl, meta_CommMapping_children); }
			template <class Pred> Udm::ChildrenAttr< ::ESMoL::CommMapping, Pred> CommMapping_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::CommMapping, Pred>(impl, meta_CommMapping_children); }

			static ::Uml::CompositionChildRole meta_MsgAssignment_children;
			Udm::ChildrenAttr< ::ESMoL::MsgAssignment> MsgAssignment_children() const { return Udm::ChildrenAttr< ::ESMoL::MsgAssignment>(impl, meta_MsgAssignment_children); }
			template <class Pred> Udm::ChildrenAttr< ::ESMoL::MsgAssignment, Pred> MsgAssignment_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::MsgAssignment, Pred>(impl, meta_MsgAssignment_children); }

			Udm::ChildrenAttr< ::ESMoL::TTMessage> TTMessage_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::TTMessage>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::TTMessage, Pred> TTMessage_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::TTMessage, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::ESMoL::Connector> Connector_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::Connector>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::Connector, Pred> Connector_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::Connector, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::ESMoL::OutCommMapping> OutCommMapping_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::OutCommMapping>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::OutCommMapping, Pred> OutCommMapping_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::OutCommMapping, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::ESMoL::CommDst> CommDst_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::CommDst>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::CommDst, Pred> CommDst_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::CommDst, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::ESMoL::CommMapping> CommMapping_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::CommMapping>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::CommMapping, Pred> CommMapping_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::CommMapping, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::ESMoL::InCommMapping> InCommMapping_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::InCommMapping>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::InCommMapping, Pred> InCommMapping_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::InCommMapping, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::ESMoL::Component> Component_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::Component>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::Component, Pred> Component_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::Component, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::ESMoL::Message> Message_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::Message>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::Message, Pred> Message_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::Message, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::ESMoL::MessageRef> MessageRef_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::MessageRef>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::MessageRef, Pred> MessageRef_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::MessageRef, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::ESMoL::MsgAssignment> MsgAssignment_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::MsgAssignment>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::MsgAssignment, Pred> MsgAssignment_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::MsgAssignment, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::ESMoL::ChannelRef> ChannelRef_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::ChannelRef>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::ChannelRef, Pred> ChannelRef_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::ChannelRef, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::ESMoL::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr< ::ESMoL::MgaObject>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::ESMoL::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::ESMoL::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

			static ::Uml::CompositionParentRole meta_Deployment_parent;
			Udm::ParentAttr< ::ESMoL::Deployment> Deployment_parent() const { return Udm::ParentAttr< ::ESMoL::Deployment>(impl, meta_Deployment_parent); }

			Udm::ParentAttr< ::ESMoL::MgaObject> parent() const { return Udm::ParentAttr< ::ESMoL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  TTTask :  public ::ESMoL::Task {
		public:
			static ::Uml::Class meta;

			TTTask() { }
			TTTask(Udm::ObjectImpl *impl) : ::ESMoL::Task(impl) { }
			TTTask(const TTTask &master) : ::ESMoL::Task(master) { }
			static TTTask Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static TTTask Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			TTTask CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			TTTask CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::ESMoL::TTTask> Instances() { return Udm::InstantiatedAttr< ::ESMoL::TTTask>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::ESMoL::TTTask, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::ESMoL::TTTask, Pred>(impl);}

			Udm::DerivedAttr< ::ESMoL::TTTask> Derived() { return Udm::DerivedAttr< ::ESMoL::TTTask>(impl);}
			template <class Pred> Udm::DerivedAttr< ::ESMoL::TTTask, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::ESMoL::TTTask, Pred>(impl);}

			Udm::ArchetypeAttr< ::ESMoL::TTTask> Archetype() { return Udm::ArchetypeAttr< ::ESMoL::TTTask>(impl);}

			static ::Uml::Attribute meta_TTFreq;
			Udm::StringAttr TTFreq() const { return Udm::StringAttr(impl, meta_TTFreq); }

			static ::Uml::Attribute meta_TTSchedule;
			Udm::StringAttr TTSchedule() const { return Udm::StringAttr(impl, meta_TTSchedule); }

			static ::Uml::Attribute meta_TTExecTime;
			Udm::StringAttr TTExecTime() const { return Udm::StringAttr(impl, meta_TTExecTime); }

			Udm::ParentAttr< ::ESMoL::MgaObject> parent() const { return Udm::ParentAttr< ::ESMoL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  OldTask :  public ::ESMoL::Task {
		public:
			static ::Uml::Class meta;

			OldTask() { }
			OldTask(Udm::ObjectImpl *impl) : ::ESMoL::Task(impl) { }
			OldTask(const OldTask &master) : ::ESMoL::Task(master) { }
			static OldTask Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static OldTask Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			OldTask CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			OldTask CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::ESMoL::OldTask> Instances() { return Udm::InstantiatedAttr< ::ESMoL::OldTask>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::ESMoL::OldTask, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::ESMoL::OldTask, Pred>(impl);}

			Udm::DerivedAttr< ::ESMoL::OldTask> Derived() { return Udm::DerivedAttr< ::ESMoL::OldTask>(impl);}
			template <class Pred> Udm::DerivedAttr< ::ESMoL::OldTask, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::ESMoL::OldTask, Pred>(impl);}

			Udm::ArchetypeAttr< ::ESMoL::OldTask> Archetype() { return Udm::ArchetypeAttr< ::ESMoL::OldTask>(impl);}

			static ::Uml::Attribute meta_Priority;
			Udm::IntegerAttr Priority() const { return Udm::IntegerAttr(impl, meta_Priority); }

			static ::Uml::Attribute meta_TaskType;
			Udm::StringAttr TaskType() const { return Udm::StringAttr(impl, meta_TaskType); }

			static ::Uml::Attribute meta_Activation;
			Udm::IntegerAttr Activation() const { return Udm::IntegerAttr(impl, meta_Activation); }

			static ::Uml::Attribute meta_AutoStart;
			Udm::BooleanAttr AutoStart() const { return Udm::BooleanAttr(impl, meta_AutoStart); }

			static ::Uml::Attribute meta_Preemption;
			Udm::StringAttr Preemption() const { return Udm::StringAttr(impl, meta_Preemption); }

			static ::Uml::Attribute meta_Cyclic;
			Udm::BooleanAttr Cyclic() const { return Udm::BooleanAttr(impl, meta_Cyclic); }

			static ::Uml::Attribute meta_CycleTime;
			Udm::IntegerAttr CycleTime() const { return Udm::IntegerAttr(impl, meta_CycleTime); }

			Udm::ParentAttr< ::ESMoL::MgaObject> parent() const { return Udm::ParentAttr< ::ESMoL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  NodeRef :  public ::ESMoL::MgaObject {
		public:
			static ::Uml::Class meta;

			NodeRef() { }
			NodeRef(Udm::ObjectImpl *impl) : ::ESMoL::MgaObject(impl) { }
			NodeRef(const NodeRef &master) : ::ESMoL::MgaObject(master) { }
			static NodeRef Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static NodeRef Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			NodeRef CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			NodeRef CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::ESMoL::NodeRef> Instances() { return Udm::InstantiatedAttr< ::ESMoL::NodeRef>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::ESMoL::NodeRef, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::ESMoL::NodeRef, Pred>(impl);}

			Udm::DerivedAttr< ::ESMoL::NodeRef> Derived() { return Udm::DerivedAttr< ::ESMoL::NodeRef>(impl);}
			template <class Pred> Udm::DerivedAttr< ::ESMoL::NodeRef, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::ESMoL::NodeRef, Pred>(impl);}

			Udm::ArchetypeAttr< ::ESMoL::NodeRef> Archetype() { return Udm::ArchetypeAttr< ::ESMoL::NodeRef>(impl);}

			static ::Uml::AssociationRole meta_ref;
			Udm::PointerAttr< ::ESMoL::Node> ref() const { return Udm::PointerAttr< ::ESMoL::Node>(impl, meta_ref); }

			static ::Uml::AssociationRole meta_srcTaskAssignment;
			static ::Uml::AssociationRole meta_srcTaskAssignment_rev;
			Udm::AClassAssocAttr< ::ESMoL::TaskAssignment, ::ESMoL::Task> srcTaskAssignment() const { return Udm::AClassAssocAttr< ::ESMoL::TaskAssignment, ::ESMoL::Task>(impl, meta_srcTaskAssignment, meta_srcTaskAssignment_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::ESMoL::TaskAssignment, ::ESMoL::Task, Pred> srcTaskAssignment_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::ESMoL::TaskAssignment, ::ESMoL::Task, Pred>(impl, meta_srcTaskAssignment, meta_srcTaskAssignment_rev); }

			static ::Uml::CompositionParentRole meta_Deployment_parent;
			Udm::ParentAttr< ::ESMoL::Deployment> Deployment_parent() const { return Udm::ParentAttr< ::ESMoL::Deployment>(impl, meta_Deployment_parent); }

			Udm::ParentAttr< ::ESMoL::MgaObject> parent() const { return Udm::ParentAttr< ::ESMoL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  ChannelRef :  public ::ESMoL::CommDst {
		public:
			static ::Uml::Class meta;

			ChannelRef() { }
			ChannelRef(Udm::ObjectImpl *impl) : ::ESMoL::CommDst(impl) { }
			ChannelRef(const ChannelRef &master) : ::ESMoL::CommDst(master) { }
			static ChannelRef Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static ChannelRef Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			ChannelRef CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			ChannelRef CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::ESMoL::ChannelRef> Instances() { return Udm::InstantiatedAttr< ::ESMoL::ChannelRef>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::ESMoL::ChannelRef, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::ESMoL::ChannelRef, Pred>(impl);}

			Udm::DerivedAttr< ::ESMoL::ChannelRef> Derived() { return Udm::DerivedAttr< ::ESMoL::ChannelRef>(impl);}
			template <class Pred> Udm::DerivedAttr< ::ESMoL::ChannelRef, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::ESMoL::ChannelRef, Pred>(impl);}

			Udm::ArchetypeAttr< ::ESMoL::ChannelRef> Archetype() { return Udm::ArchetypeAttr< ::ESMoL::ChannelRef>(impl);}

			static ::Uml::AssociationRole meta_ref;
			Udm::PointerAttr< ::ESMoL::Channel> ref() const { return Udm::PointerAttr< ::ESMoL::Channel>(impl, meta_ref); }

			static ::Uml::AssociationRole meta_srcMsgAssignment;
			static ::Uml::AssociationRole meta_srcMsgAssignment_rev;
			Udm::AClassAssocAttr< ::ESMoL::MsgAssignment, ::ESMoL::CommDst> srcMsgAssignment() const { return Udm::AClassAssocAttr< ::ESMoL::MsgAssignment, ::ESMoL::CommDst>(impl, meta_srcMsgAssignment, meta_srcMsgAssignment_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::ESMoL::MsgAssignment, ::ESMoL::CommDst, Pred> srcMsgAssignment_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::ESMoL::MsgAssignment, ::ESMoL::CommDst, Pred>(impl, meta_srcMsgAssignment, meta_srcMsgAssignment_rev); }

			Udm::ParentAttr< ::ESMoL::MgaObject> parent() const { return Udm::ParentAttr< ::ESMoL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};



}

#endif //MOBIES_ESMOL_H
