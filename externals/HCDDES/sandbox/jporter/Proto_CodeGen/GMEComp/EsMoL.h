#ifndef MOBIES_ESMOL_H
#define MOBIES_ESMOL_H
// header file EsMoL.h generated from diagram EsMoL
// generated on Tue Jul 22 11:51:46 2008

#ifndef MOBIES_UDMBASE_H
#include "UdmBase.h"
#endif

#ifdef min
#undef min
#endif

#ifdef max
#undef max
#endif
namespace EsMoL {
	extern  Udm::UdmDiagram diagram;
	 void Initialize(const ::Uml::Diagram &dgr);
	 void Initialize();

		class  Channel;
		class  Node;
		class  PlatformLibrary;
		class  HWElement;
		class  IChan;
		class  Bus;
		class  HardwareUnit;
		class  Connectable;
		class  BChan;
		class  Wire;
		class  OS;
		class  Router;
		class  Capability;
		class  OChan;
		class  ActionPort;
		class  EnablePort;
		class  Port;
		class  InPort;
		class  TriggerPort;
		class  Subsystem;
		class  Dataflow;
		class  Primitive;
		class  Block;
		class  Parameter;
		class  SLReference;
		class  SLAnnotation;
		class  Line;
		class  OutPort;
		class  StatePort;
		class  OutputPort;
		class  InputPort;
		class  Component;
		class  Deployment;
		class  TaskAssignment;
		class  Task;
		class  CommDst;
		class  MessageRef;
		class  NodeRef;
		class  MsgAssignment;
		class  CommMapping;
		class  Message;
		class  InCommMapping;
		class  IOPort;
		class  ChannelRef;
		class  OutCommMapping;
		class  DeploymentLibrary;
		class  TypeBase;
		class  Matrix;
		class  Types;
		class  TypeStruct;
		class  TypeBaseRef;
		class  IOPortExp;
		class  IOPortInfo;
		class  IOPortInfoRef;
		class  IOPortAssignment;
		class  IOPortInfoBase;
		class  System;
		class  ArchitectureLibrary;
		class  SubsystemRef;
		class  CCode;
		class  DesignFolder;
		class  Connector;
		class  ComponentBase;
		class  TimingConstraint;
		class  RequirementsLibrary;
		class  TimingSheet;
		class  TaskRef;
		class  Stateflow;
		class  ConnectorRef;
		class  TransConnector;
		class  History;
		class  Event;
		class  StateDE;
		class  Data;
		class  Transition;
		class  State;
		class  TransStart;
		class  Junction;
		class  RootFolder;
		class  MgaObject;


		 void CreateMetaObjs();
		 void InitCrossNSInheritence();
		 void InitCrossNSCompositions();
		 void InitCrossNSAssociations();
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

			Udm::InstantiatedAttr< ::EsMoL::PlatformLibrary> Instances() { return Udm::InstantiatedAttr< ::EsMoL::PlatformLibrary>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::EsMoL::PlatformLibrary, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::EsMoL::PlatformLibrary, Pred>(impl);}

			Udm::DerivedAttr< ::EsMoL::PlatformLibrary> Derived() { return Udm::DerivedAttr< ::EsMoL::PlatformLibrary>(impl);}
			template <class Pred> Udm::DerivedAttr< ::EsMoL::PlatformLibrary, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::EsMoL::PlatformLibrary, Pred>(impl);}

			Udm::ArchetypeAttr< ::EsMoL::PlatformLibrary> Archetype() { return Udm::ArchetypeAttr< ::EsMoL::PlatformLibrary>(impl);}

			static ::Uml::Attribute meta_name;
			Udm::StringAttr name() const { return Udm::StringAttr(impl, meta_name); }

			static ::Uml::CompositionChildRole meta_HardwareUnit_children;
			Udm::ChildrenAttr< ::EsMoL::HardwareUnit> HardwareUnit_children() const { return Udm::ChildrenAttr< ::EsMoL::HardwareUnit>(impl, meta_HardwareUnit_children); }
			template <class Pred> Udm::ChildrenAttr< ::EsMoL::HardwareUnit, Pred> HardwareUnit_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::HardwareUnit, Pred>(impl, meta_HardwareUnit_children); }

			Udm::ChildrenAttr< ::EsMoL::HWElement> HWElement_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::HWElement>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::HWElement, Pred> HWElement_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::HWElement, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::EsMoL::HardwareUnit> HardwareUnit_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::HardwareUnit>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::HardwareUnit, Pred> HardwareUnit_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::HardwareUnit, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::EsMoL::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::MgaObject>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

			static ::Uml::CompositionParentRole meta_RootFolder_parent;
			Udm::ParentAttr< ::EsMoL::RootFolder> RootFolder_parent() const { return Udm::ParentAttr< ::EsMoL::RootFolder>(impl, meta_RootFolder_parent); }

			Udm::ParentAttr< ::EsMoL::RootFolder> parent() const { return Udm::ParentAttr< ::EsMoL::RootFolder>(impl, Udm::NULLPARENTROLE); }
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

			Udm::InstantiatedAttr< ::EsMoL::Dataflow> Instances() { return Udm::InstantiatedAttr< ::EsMoL::Dataflow>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::EsMoL::Dataflow, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::EsMoL::Dataflow, Pred>(impl);}

			Udm::DerivedAttr< ::EsMoL::Dataflow> Derived() { return Udm::DerivedAttr< ::EsMoL::Dataflow>(impl);}
			template <class Pred> Udm::DerivedAttr< ::EsMoL::Dataflow, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::EsMoL::Dataflow, Pred>(impl);}

			Udm::ArchetypeAttr< ::EsMoL::Dataflow> Archetype() { return Udm::ArchetypeAttr< ::EsMoL::Dataflow>(impl);}

			static ::Uml::Attribute meta_name;
			Udm::StringAttr name() const { return Udm::StringAttr(impl, meta_name); }

			static ::Uml::CompositionChildRole meta_Subsystem_children;
			Udm::ChildrenAttr< ::EsMoL::Subsystem> Subsystem_children() const { return Udm::ChildrenAttr< ::EsMoL::Subsystem>(impl, meta_Subsystem_children); }
			template <class Pred> Udm::ChildrenAttr< ::EsMoL::Subsystem, Pred> Subsystem_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::Subsystem, Pred>(impl, meta_Subsystem_children); }

			Udm::ChildrenAttr< ::EsMoL::Subsystem> Subsystem_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::Subsystem>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::Subsystem, Pred> Subsystem_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::Subsystem, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::EsMoL::Block> Block_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::Block>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::Block, Pred> Block_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::Block, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::EsMoL::ComponentBase> ComponentBase_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::ComponentBase>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::ComponentBase, Pred> ComponentBase_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::ComponentBase, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::EsMoL::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::MgaObject>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

			static ::Uml::CompositionParentRole meta_RootFolder_parent;
			Udm::ParentAttr< ::EsMoL::RootFolder> RootFolder_parent() const { return Udm::ParentAttr< ::EsMoL::RootFolder>(impl, meta_RootFolder_parent); }

			Udm::ParentAttr< ::EsMoL::RootFolder> parent() const { return Udm::ParentAttr< ::EsMoL::RootFolder>(impl, Udm::NULLPARENTROLE); }
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

			Udm::InstantiatedAttr< ::EsMoL::DeploymentLibrary> Instances() { return Udm::InstantiatedAttr< ::EsMoL::DeploymentLibrary>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::EsMoL::DeploymentLibrary, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::EsMoL::DeploymentLibrary, Pred>(impl);}

			Udm::DerivedAttr< ::EsMoL::DeploymentLibrary> Derived() { return Udm::DerivedAttr< ::EsMoL::DeploymentLibrary>(impl);}
			template <class Pred> Udm::DerivedAttr< ::EsMoL::DeploymentLibrary, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::EsMoL::DeploymentLibrary, Pred>(impl);}

			Udm::ArchetypeAttr< ::EsMoL::DeploymentLibrary> Archetype() { return Udm::ArchetypeAttr< ::EsMoL::DeploymentLibrary>(impl);}

			static ::Uml::Attribute meta_name;
			Udm::StringAttr name() const { return Udm::StringAttr(impl, meta_name); }

			static ::Uml::CompositionChildRole meta_Deployment_children;
			Udm::ChildrenAttr< ::EsMoL::Deployment> Deployment_children() const { return Udm::ChildrenAttr< ::EsMoL::Deployment>(impl, meta_Deployment_children); }
			template <class Pred> Udm::ChildrenAttr< ::EsMoL::Deployment, Pred> Deployment_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::Deployment, Pred>(impl, meta_Deployment_children); }

			Udm::ChildrenAttr< ::EsMoL::Deployment> Deployment_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::Deployment>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::Deployment, Pred> Deployment_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::Deployment, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::EsMoL::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::MgaObject>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

			static ::Uml::CompositionParentRole meta_RootFolder_parent;
			Udm::ParentAttr< ::EsMoL::RootFolder> RootFolder_parent() const { return Udm::ParentAttr< ::EsMoL::RootFolder>(impl, meta_RootFolder_parent); }

			Udm::ParentAttr< ::EsMoL::RootFolder> parent() const { return Udm::ParentAttr< ::EsMoL::RootFolder>(impl, Udm::NULLPARENTROLE); }
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

			Udm::InstantiatedAttr< ::EsMoL::Types> Instances() { return Udm::InstantiatedAttr< ::EsMoL::Types>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::EsMoL::Types, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::EsMoL::Types, Pred>(impl);}

			Udm::DerivedAttr< ::EsMoL::Types> Derived() { return Udm::DerivedAttr< ::EsMoL::Types>(impl);}
			template <class Pred> Udm::DerivedAttr< ::EsMoL::Types, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::EsMoL::Types, Pred>(impl);}

			Udm::ArchetypeAttr< ::EsMoL::Types> Archetype() { return Udm::ArchetypeAttr< ::EsMoL::Types>(impl);}

			static ::Uml::Attribute meta_name;
			Udm::StringAttr name() const { return Udm::StringAttr(impl, meta_name); }

			static ::Uml::CompositionChildRole meta_TypeBase_children;
			Udm::ChildrenAttr< ::EsMoL::TypeBase> TypeBase_children() const { return Udm::ChildrenAttr< ::EsMoL::TypeBase>(impl, meta_TypeBase_children); }
			template <class Pred> Udm::ChildrenAttr< ::EsMoL::TypeBase, Pred> TypeBase_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::TypeBase, Pred>(impl, meta_TypeBase_children); }

			Udm::ChildrenAttr< ::EsMoL::TypeBase> TypeBase_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::TypeBase>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::TypeBase, Pred> TypeBase_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::TypeBase, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::EsMoL::Matrix> Matrix_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::Matrix>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::Matrix, Pred> Matrix_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::Matrix, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::EsMoL::TypeStruct> TypeStruct_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::TypeStruct>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::TypeStruct, Pred> TypeStruct_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::TypeStruct, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::EsMoL::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::MgaObject>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

			static ::Uml::CompositionParentRole meta_RootFolder_parent;
			Udm::ParentAttr< ::EsMoL::RootFolder> RootFolder_parent() const { return Udm::ParentAttr< ::EsMoL::RootFolder>(impl, meta_RootFolder_parent); }

			Udm::ParentAttr< ::EsMoL::RootFolder> parent() const { return Udm::ParentAttr< ::EsMoL::RootFolder>(impl, Udm::NULLPARENTROLE); }
		};

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

			Udm::InstantiatedAttr< ::EsMoL::ArchitectureLibrary> Instances() { return Udm::InstantiatedAttr< ::EsMoL::ArchitectureLibrary>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::EsMoL::ArchitectureLibrary, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::EsMoL::ArchitectureLibrary, Pred>(impl);}

			Udm::DerivedAttr< ::EsMoL::ArchitectureLibrary> Derived() { return Udm::DerivedAttr< ::EsMoL::ArchitectureLibrary>(impl);}
			template <class Pred> Udm::DerivedAttr< ::EsMoL::ArchitectureLibrary, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::EsMoL::ArchitectureLibrary, Pred>(impl);}

			Udm::ArchetypeAttr< ::EsMoL::ArchitectureLibrary> Archetype() { return Udm::ArchetypeAttr< ::EsMoL::ArchitectureLibrary>(impl);}

			static ::Uml::Attribute meta_name;
			Udm::StringAttr name() const { return Udm::StringAttr(impl, meta_name); }

			static ::Uml::CompositionChildRole meta_System_children;
			Udm::ChildrenAttr< ::EsMoL::System> System_children() const { return Udm::ChildrenAttr< ::EsMoL::System>(impl, meta_System_children); }
			template <class Pred> Udm::ChildrenAttr< ::EsMoL::System, Pred> System_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::System, Pred>(impl, meta_System_children); }

			Udm::ChildrenAttr< ::EsMoL::System> System_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::System>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::System, Pred> System_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::System, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::EsMoL::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::MgaObject>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

			static ::Uml::CompositionParentRole meta_RootFolder_parent;
			Udm::ParentAttr< ::EsMoL::RootFolder> RootFolder_parent() const { return Udm::ParentAttr< ::EsMoL::RootFolder>(impl, meta_RootFolder_parent); }

			Udm::ParentAttr< ::EsMoL::RootFolder> parent() const { return Udm::ParentAttr< ::EsMoL::RootFolder>(impl, Udm::NULLPARENTROLE); }
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

			Udm::InstantiatedAttr< ::EsMoL::DesignFolder> Instances() { return Udm::InstantiatedAttr< ::EsMoL::DesignFolder>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::EsMoL::DesignFolder, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::EsMoL::DesignFolder, Pred>(impl);}

			Udm::DerivedAttr< ::EsMoL::DesignFolder> Derived() { return Udm::DerivedAttr< ::EsMoL::DesignFolder>(impl);}
			template <class Pred> Udm::DerivedAttr< ::EsMoL::DesignFolder, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::EsMoL::DesignFolder, Pred>(impl);}

			Udm::ArchetypeAttr< ::EsMoL::DesignFolder> Archetype() { return Udm::ArchetypeAttr< ::EsMoL::DesignFolder>(impl);}

			static ::Uml::Attribute meta_name;
			Udm::StringAttr name() const { return Udm::StringAttr(impl, meta_name); }

			static ::Uml::CompositionChildRole meta_HardwareUnit_children;
			Udm::ChildrenAttr< ::EsMoL::HardwareUnit> HardwareUnit_children() const { return Udm::ChildrenAttr< ::EsMoL::HardwareUnit>(impl, meta_HardwareUnit_children); }
			template <class Pred> Udm::ChildrenAttr< ::EsMoL::HardwareUnit, Pred> HardwareUnit_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::HardwareUnit, Pred>(impl, meta_HardwareUnit_children); }

			static ::Uml::CompositionChildRole meta_System_children;
			Udm::ChildrenAttr< ::EsMoL::System> System_children() const { return Udm::ChildrenAttr< ::EsMoL::System>(impl, meta_System_children); }
			template <class Pred> Udm::ChildrenAttr< ::EsMoL::System, Pred> System_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::System, Pred>(impl, meta_System_children); }

			static ::Uml::CompositionChildRole meta_Subsystem_children;
			Udm::ChildrenAttr< ::EsMoL::Subsystem> Subsystem_children() const { return Udm::ChildrenAttr< ::EsMoL::Subsystem>(impl, meta_Subsystem_children); }
			template <class Pred> Udm::ChildrenAttr< ::EsMoL::Subsystem, Pred> Subsystem_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::Subsystem, Pred>(impl, meta_Subsystem_children); }

			static ::Uml::CompositionChildRole meta_Deployment_children;
			Udm::ChildrenAttr< ::EsMoL::Deployment> Deployment_children() const { return Udm::ChildrenAttr< ::EsMoL::Deployment>(impl, meta_Deployment_children); }
			template <class Pred> Udm::ChildrenAttr< ::EsMoL::Deployment, Pred> Deployment_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::Deployment, Pred>(impl, meta_Deployment_children); }

			static ::Uml::CompositionChildRole meta_TimingSheet_children;
			Udm::ChildrenAttr< ::EsMoL::TimingSheet> TimingSheet_children() const { return Udm::ChildrenAttr< ::EsMoL::TimingSheet>(impl, meta_TimingSheet_children); }
			template <class Pred> Udm::ChildrenAttr< ::EsMoL::TimingSheet, Pred> TimingSheet_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::TimingSheet, Pred>(impl, meta_TimingSheet_children); }

			Udm::ChildrenAttr< ::EsMoL::HWElement> HWElement_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::HWElement>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::HWElement, Pred> HWElement_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::HWElement, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::EsMoL::HardwareUnit> HardwareUnit_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::HardwareUnit>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::HardwareUnit, Pred> HardwareUnit_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::HardwareUnit, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::EsMoL::Subsystem> Subsystem_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::Subsystem>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::Subsystem, Pred> Subsystem_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::Subsystem, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::EsMoL::Block> Block_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::Block>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::Block, Pred> Block_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::Block, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::EsMoL::Deployment> Deployment_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::Deployment>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::Deployment, Pred> Deployment_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::Deployment, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::EsMoL::System> System_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::System>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::System, Pred> System_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::System, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::EsMoL::ComponentBase> ComponentBase_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::ComponentBase>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::ComponentBase, Pred> ComponentBase_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::ComponentBase, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::EsMoL::TimingSheet> TimingSheet_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::TimingSheet>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::TimingSheet, Pred> TimingSheet_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::TimingSheet, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::EsMoL::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::MgaObject>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

			static ::Uml::CompositionParentRole meta_RootFolder_parent;
			Udm::ParentAttr< ::EsMoL::RootFolder> RootFolder_parent() const { return Udm::ParentAttr< ::EsMoL::RootFolder>(impl, meta_RootFolder_parent); }

			Udm::ParentAttr< ::EsMoL::RootFolder> parent() const { return Udm::ParentAttr< ::EsMoL::RootFolder>(impl, Udm::NULLPARENTROLE); }
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

			Udm::InstantiatedAttr< ::EsMoL::RequirementsLibrary> Instances() { return Udm::InstantiatedAttr< ::EsMoL::RequirementsLibrary>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::EsMoL::RequirementsLibrary, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::EsMoL::RequirementsLibrary, Pred>(impl);}

			Udm::DerivedAttr< ::EsMoL::RequirementsLibrary> Derived() { return Udm::DerivedAttr< ::EsMoL::RequirementsLibrary>(impl);}
			template <class Pred> Udm::DerivedAttr< ::EsMoL::RequirementsLibrary, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::EsMoL::RequirementsLibrary, Pred>(impl);}

			Udm::ArchetypeAttr< ::EsMoL::RequirementsLibrary> Archetype() { return Udm::ArchetypeAttr< ::EsMoL::RequirementsLibrary>(impl);}

			static ::Uml::Attribute meta_name;
			Udm::StringAttr name() const { return Udm::StringAttr(impl, meta_name); }

			static ::Uml::CompositionChildRole meta_TimingSheet_children;
			Udm::ChildrenAttr< ::EsMoL::TimingSheet> TimingSheet_children() const { return Udm::ChildrenAttr< ::EsMoL::TimingSheet>(impl, meta_TimingSheet_children); }
			template <class Pred> Udm::ChildrenAttr< ::EsMoL::TimingSheet, Pred> TimingSheet_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::TimingSheet, Pred>(impl, meta_TimingSheet_children); }

			Udm::ChildrenAttr< ::EsMoL::TimingSheet> TimingSheet_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::TimingSheet>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::TimingSheet, Pred> TimingSheet_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::TimingSheet, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::EsMoL::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::MgaObject>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

			static ::Uml::CompositionParentRole meta_RootFolder_parent;
			Udm::ParentAttr< ::EsMoL::RootFolder> RootFolder_parent() const { return Udm::ParentAttr< ::EsMoL::RootFolder>(impl, meta_RootFolder_parent); }

			Udm::ParentAttr< ::EsMoL::RootFolder> parent() const { return Udm::ParentAttr< ::EsMoL::RootFolder>(impl, Udm::NULLPARENTROLE); }
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

			Udm::InstantiatedAttr< ::EsMoL::Stateflow> Instances() { return Udm::InstantiatedAttr< ::EsMoL::Stateflow>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::EsMoL::Stateflow, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::EsMoL::Stateflow, Pred>(impl);}

			Udm::DerivedAttr< ::EsMoL::Stateflow> Derived() { return Udm::DerivedAttr< ::EsMoL::Stateflow>(impl);}
			template <class Pred> Udm::DerivedAttr< ::EsMoL::Stateflow, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::EsMoL::Stateflow, Pred>(impl);}

			Udm::ArchetypeAttr< ::EsMoL::Stateflow> Archetype() { return Udm::ArchetypeAttr< ::EsMoL::Stateflow>(impl);}

			static ::Uml::Attribute meta_name;
			Udm::StringAttr name() const { return Udm::StringAttr(impl, meta_name); }

			static ::Uml::CompositionChildRole meta_State_children;
			Udm::ChildrenAttr< ::EsMoL::State> State_children() const { return Udm::ChildrenAttr< ::EsMoL::State>(impl, meta_State_children); }
			template <class Pred> Udm::ChildrenAttr< ::EsMoL::State, Pred> State_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::State, Pred>(impl, meta_State_children); }

			Udm::ChildrenAttr< ::EsMoL::TransConnector> TransConnector_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::TransConnector>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::TransConnector, Pred> TransConnector_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::TransConnector, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::EsMoL::State> State_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::State>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::State, Pred> State_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::State, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::EsMoL::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::MgaObject>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

			static ::Uml::CompositionParentRole meta_RootFolder_parent;
			Udm::ParentAttr< ::EsMoL::RootFolder> RootFolder_parent() const { return Udm::ParentAttr< ::EsMoL::RootFolder>(impl, meta_RootFolder_parent); }

			Udm::ParentAttr< ::EsMoL::RootFolder> parent() const { return Udm::ParentAttr< ::EsMoL::RootFolder>(impl, Udm::NULLPARENTROLE); }
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

			Udm::InstantiatedAttr< ::EsMoL::RootFolder> Instances() { return Udm::InstantiatedAttr< ::EsMoL::RootFolder>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::EsMoL::RootFolder, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::EsMoL::RootFolder, Pred>(impl);}

			Udm::DerivedAttr< ::EsMoL::RootFolder> Derived() { return Udm::DerivedAttr< ::EsMoL::RootFolder>(impl);}
			template <class Pred> Udm::DerivedAttr< ::EsMoL::RootFolder, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::EsMoL::RootFolder, Pred>(impl);}

			Udm::ArchetypeAttr< ::EsMoL::RootFolder> Archetype() { return Udm::ArchetypeAttr< ::EsMoL::RootFolder>(impl);}

			static ::Uml::Attribute meta_name;
			Udm::StringAttr name() const { return Udm::StringAttr(impl, meta_name); }

			static ::Uml::CompositionChildRole meta_Dataflow_children;
			Udm::ChildrenAttr< ::EsMoL::Dataflow> Dataflow_children() const { return Udm::ChildrenAttr< ::EsMoL::Dataflow>(impl, meta_Dataflow_children); }
			template <class Pred> Udm::ChildrenAttr< ::EsMoL::Dataflow, Pred> Dataflow_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::Dataflow, Pred>(impl, meta_Dataflow_children); }

			static ::Uml::CompositionChildRole meta_PlatformLibrary_children;
			Udm::ChildrenAttr< ::EsMoL::PlatformLibrary> PlatformLibrary_children() const { return Udm::ChildrenAttr< ::EsMoL::PlatformLibrary>(impl, meta_PlatformLibrary_children); }
			template <class Pred> Udm::ChildrenAttr< ::EsMoL::PlatformLibrary, Pred> PlatformLibrary_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::PlatformLibrary, Pred>(impl, meta_PlatformLibrary_children); }

			static ::Uml::CompositionChildRole meta_DeploymentLibrary_children;
			Udm::ChildrenAttr< ::EsMoL::DeploymentLibrary> DeploymentLibrary_children() const { return Udm::ChildrenAttr< ::EsMoL::DeploymentLibrary>(impl, meta_DeploymentLibrary_children); }
			template <class Pred> Udm::ChildrenAttr< ::EsMoL::DeploymentLibrary, Pred> DeploymentLibrary_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::DeploymentLibrary, Pred>(impl, meta_DeploymentLibrary_children); }

			static ::Uml::CompositionChildRole meta_Types_children;
			Udm::ChildrenAttr< ::EsMoL::Types> Types_children() const { return Udm::ChildrenAttr< ::EsMoL::Types>(impl, meta_Types_children); }
			template <class Pred> Udm::ChildrenAttr< ::EsMoL::Types, Pred> Types_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::Types, Pred>(impl, meta_Types_children); }

			static ::Uml::CompositionChildRole meta_ArchitectureLibrary_children;
			Udm::ChildrenAttr< ::EsMoL::ArchitectureLibrary> ArchitectureLibrary_children() const { return Udm::ChildrenAttr< ::EsMoL::ArchitectureLibrary>(impl, meta_ArchitectureLibrary_children); }
			template <class Pred> Udm::ChildrenAttr< ::EsMoL::ArchitectureLibrary, Pred> ArchitectureLibrary_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::ArchitectureLibrary, Pred>(impl, meta_ArchitectureLibrary_children); }

			static ::Uml::CompositionChildRole meta_DesignFolder_children;
			Udm::ChildrenAttr< ::EsMoL::DesignFolder> DesignFolder_children() const { return Udm::ChildrenAttr< ::EsMoL::DesignFolder>(impl, meta_DesignFolder_children); }
			template <class Pred> Udm::ChildrenAttr< ::EsMoL::DesignFolder, Pred> DesignFolder_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::DesignFolder, Pred>(impl, meta_DesignFolder_children); }

			static ::Uml::CompositionChildRole meta_RequirementsLibrary_children;
			Udm::ChildrenAttr< ::EsMoL::RequirementsLibrary> RequirementsLibrary_children() const { return Udm::ChildrenAttr< ::EsMoL::RequirementsLibrary>(impl, meta_RequirementsLibrary_children); }
			template <class Pred> Udm::ChildrenAttr< ::EsMoL::RequirementsLibrary, Pred> RequirementsLibrary_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::RequirementsLibrary, Pred>(impl, meta_RequirementsLibrary_children); }

			static ::Uml::CompositionChildRole meta_Stateflow_children;
			Udm::ChildrenAttr< ::EsMoL::Stateflow> Stateflow_children() const { return Udm::ChildrenAttr< ::EsMoL::Stateflow>(impl, meta_Stateflow_children); }
			template <class Pred> Udm::ChildrenAttr< ::EsMoL::Stateflow, Pred> Stateflow_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::Stateflow, Pred>(impl, meta_Stateflow_children); }

			static ::Uml::CompositionChildRole meta_RootFolder_children;
			Udm::ChildrenAttr< ::EsMoL::RootFolder> RootFolder_children() const { return Udm::ChildrenAttr< ::EsMoL::RootFolder>(impl, meta_RootFolder_children); }
			template <class Pred> Udm::ChildrenAttr< ::EsMoL::RootFolder, Pred> RootFolder_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::RootFolder, Pred>(impl, meta_RootFolder_children); }

			Udm::ChildrenAttr< ::EsMoL::PlatformLibrary> PlatformLibrary_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::PlatformLibrary>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::PlatformLibrary, Pred> PlatformLibrary_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::PlatformLibrary, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::EsMoL::Dataflow> Dataflow_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::Dataflow>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::Dataflow, Pred> Dataflow_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::Dataflow, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::EsMoL::DeploymentLibrary> DeploymentLibrary_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::DeploymentLibrary>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::DeploymentLibrary, Pred> DeploymentLibrary_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::DeploymentLibrary, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::EsMoL::Types> Types_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::Types>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::Types, Pred> Types_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::Types, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::EsMoL::ArchitectureLibrary> ArchitectureLibrary_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::ArchitectureLibrary>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::ArchitectureLibrary, Pred> ArchitectureLibrary_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::ArchitectureLibrary, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::EsMoL::DesignFolder> DesignFolder_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::DesignFolder>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::DesignFolder, Pred> DesignFolder_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::DesignFolder, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::EsMoL::RequirementsLibrary> RequirementsLibrary_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::RequirementsLibrary>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::RequirementsLibrary, Pred> RequirementsLibrary_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::RequirementsLibrary, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::EsMoL::Stateflow> Stateflow_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::Stateflow>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::Stateflow, Pred> Stateflow_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::Stateflow, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::EsMoL::RootFolder> RootFolder_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::RootFolder>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::RootFolder, Pred> RootFolder_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::RootFolder, Pred>(impl, Udm::NULLCHILDROLE); }

			static ::Uml::CompositionParentRole meta_RootFolder_parent;
			Udm::ParentAttr< ::EsMoL::RootFolder> RootFolder_parent() const { return Udm::ParentAttr< ::EsMoL::RootFolder>(impl, meta_RootFolder_parent); }

			Udm::ParentAttr< ::EsMoL::RootFolder> parent() const { return Udm::ParentAttr< ::EsMoL::RootFolder>(impl, Udm::NULLPARENTROLE); }
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

			Udm::InstantiatedAttr< ::EsMoL::MgaObject> Instances() { return Udm::InstantiatedAttr< ::EsMoL::MgaObject>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::EsMoL::MgaObject, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::EsMoL::MgaObject, Pred>(impl);}

			Udm::DerivedAttr< ::EsMoL::MgaObject> Derived() { return Udm::DerivedAttr< ::EsMoL::MgaObject>(impl);}
			template <class Pred> Udm::DerivedAttr< ::EsMoL::MgaObject, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::EsMoL::MgaObject, Pred>(impl);}

			Udm::ArchetypeAttr< ::EsMoL::MgaObject> Archetype() { return Udm::ArchetypeAttr< ::EsMoL::MgaObject>(impl);}

			static ::Uml::Attribute meta_position;
			Udm::StringAttr position() const { return Udm::StringAttr(impl, meta_position); }

			static ::Uml::Attribute meta_name;
			Udm::StringAttr name() const { return Udm::StringAttr(impl, meta_name); }

			Udm::ParentAttr<Udm::Object> parent() const { return Udm::ParentAttr<Udm::Object>(impl, Udm::NULLPARENTROLE); }
		};

		class  HWElement :  virtual public ::EsMoL::MgaObject {
		public:
			static ::Uml::Class meta;

			HWElement() { }
			HWElement(Udm::ObjectImpl *impl) : ::EsMoL::MgaObject(impl) { }
			HWElement(const HWElement &master) : ::EsMoL::MgaObject(master) { }
			static HWElement Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static HWElement Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			HWElement CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			HWElement CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::EsMoL::HWElement> Instances() { return Udm::InstantiatedAttr< ::EsMoL::HWElement>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::EsMoL::HWElement, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::EsMoL::HWElement, Pred>(impl);}

			Udm::DerivedAttr< ::EsMoL::HWElement> Derived() { return Udm::DerivedAttr< ::EsMoL::HWElement>(impl);}
			template <class Pred> Udm::DerivedAttr< ::EsMoL::HWElement, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::EsMoL::HWElement, Pred>(impl);}

			Udm::ArchetypeAttr< ::EsMoL::HWElement> Archetype() { return Udm::ArchetypeAttr< ::EsMoL::HWElement>(impl);}

			static ::Uml::Attribute meta_MetaData;
			Udm::StringAttr MetaData() const { return Udm::StringAttr(impl, meta_MetaData); }

			static ::Uml::CompositionChildRole meta_Capability_children;
			Udm::ChildrenAttr< ::EsMoL::Capability> Capability_children() const { return Udm::ChildrenAttr< ::EsMoL::Capability>(impl, meta_Capability_children); }
			template <class Pred> Udm::ChildrenAttr< ::EsMoL::Capability, Pred> Capability_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::Capability, Pred>(impl, meta_Capability_children); }

			Udm::ChildrenAttr< ::EsMoL::Capability> Capability_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::Capability>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::Capability, Pred> Capability_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::Capability, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::EsMoL::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::MgaObject>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

			static ::Uml::CompositionParentRole meta_HardwareUnit_parent;
			Udm::ParentAttr< ::EsMoL::HardwareUnit> HardwareUnit_parent() const { return Udm::ParentAttr< ::EsMoL::HardwareUnit>(impl, meta_HardwareUnit_parent); }

			Udm::ParentAttr< ::EsMoL::MgaObject> parent() const { return Udm::ParentAttr< ::EsMoL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  Node :  public ::EsMoL::HWElement {
		public:
			static ::Uml::Class meta;

			Node() { }
			Node(Udm::ObjectImpl *impl) : ::EsMoL::HWElement(impl), ::EsMoL::MgaObject(impl) { }
			Node(const Node &master) : ::EsMoL::HWElement(master), ::EsMoL::MgaObject(master) { }
			static Node Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static Node Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			Node CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			Node CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::EsMoL::Node> Instances() { return Udm::InstantiatedAttr< ::EsMoL::Node>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::EsMoL::Node, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::EsMoL::Node, Pred>(impl);}

			Udm::DerivedAttr< ::EsMoL::Node> Derived() { return Udm::DerivedAttr< ::EsMoL::Node>(impl);}
			template <class Pred> Udm::DerivedAttr< ::EsMoL::Node, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::EsMoL::Node, Pred>(impl);}

			Udm::ArchetypeAttr< ::EsMoL::Node> Archetype() { return Udm::ArchetypeAttr< ::EsMoL::Node>(impl);}

			static ::Uml::Attribute meta_Simulator;
			Udm::StringAttr Simulator() const { return Udm::StringAttr(impl, meta_Simulator); }

			static ::Uml::Attribute meta_CName;
			Udm::StringAttr CName() const { return Udm::StringAttr(impl, meta_CName); }

			static ::Uml::Attribute meta_ROM;
			Udm::IntegerAttr ROM() const { return Udm::IntegerAttr(impl, meta_ROM); }

			static ::Uml::Attribute meta_RAM;
			Udm::IntegerAttr RAM() const { return Udm::IntegerAttr(impl, meta_RAM); }

			static ::Uml::Attribute meta_Speed;
			Udm::IntegerAttr Speed() const { return Udm::IntegerAttr(impl, meta_Speed); }

			static ::Uml::Attribute meta_CPU;
			Udm::StringAttr CPU() const { return Udm::StringAttr(impl, meta_CPU); }

			static ::Uml::AssociationRole meta_referedbyNodeRef;
			Udm::AssocAttr< ::EsMoL::NodeRef> referedbyNodeRef() const { return Udm::AssocAttr< ::EsMoL::NodeRef>(impl, meta_referedbyNodeRef); }
			template <class Pred> Udm::AssocAttr< ::EsMoL::NodeRef, Pred > referedbyNodeRef_sorted(const Pred &) const { return Udm::AssocAttr< ::EsMoL::NodeRef, Pred>(impl, meta_referedbyNodeRef); }

			static ::Uml::CompositionChildRole meta_OS_child;
			Udm::ChildAttr< ::EsMoL::OS> OS_child() const { return Udm::ChildAttr< ::EsMoL::OS>(impl, meta_OS_child); }

			static ::Uml::CompositionChildRole meta_Channel_children;
			Udm::ChildrenAttr< ::EsMoL::Channel> Channel_children() const { return Udm::ChildrenAttr< ::EsMoL::Channel>(impl, meta_Channel_children); }
			template <class Pred> Udm::ChildrenAttr< ::EsMoL::Channel, Pred> Channel_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::Channel, Pred>(impl, meta_Channel_children); }

			Udm::ChildrenAttr< ::EsMoL::Channel> Channel_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::Channel>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::Channel, Pred> Channel_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::Channel, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::EsMoL::IChan> IChan_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::IChan>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::IChan, Pred> IChan_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::IChan, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::EsMoL::Connectable> Connectable_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::Connectable>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::Connectable, Pred> Connectable_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::Connectable, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::EsMoL::BChan> BChan_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::BChan>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::BChan, Pred> BChan_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::BChan, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::EsMoL::OS> OS_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::OS>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::OS, Pred> OS_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::OS, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::EsMoL::OChan> OChan_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::OChan>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::OChan, Pred> OChan_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::OChan, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::EsMoL::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::MgaObject>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ParentAttr< ::EsMoL::MgaObject> parent() const { return Udm::ParentAttr< ::EsMoL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  HardwareUnit :  public ::EsMoL::HWElement {
		public:
			static ::Uml::Class meta;

			HardwareUnit() { }
			HardwareUnit(Udm::ObjectImpl *impl) : ::EsMoL::HWElement(impl), ::EsMoL::MgaObject(impl) { }
			HardwareUnit(const HardwareUnit &master) : ::EsMoL::HWElement(master), ::EsMoL::MgaObject(master) { }
			static HardwareUnit Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static HardwareUnit Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			HardwareUnit CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			HardwareUnit CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::EsMoL::HardwareUnit> Instances() { return Udm::InstantiatedAttr< ::EsMoL::HardwareUnit>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::EsMoL::HardwareUnit, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::EsMoL::HardwareUnit, Pred>(impl);}

			Udm::DerivedAttr< ::EsMoL::HardwareUnit> Derived() { return Udm::DerivedAttr< ::EsMoL::HardwareUnit>(impl);}
			template <class Pred> Udm::DerivedAttr< ::EsMoL::HardwareUnit, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::EsMoL::HardwareUnit, Pred>(impl);}

			Udm::ArchetypeAttr< ::EsMoL::HardwareUnit> Archetype() { return Udm::ArchetypeAttr< ::EsMoL::HardwareUnit>(impl);}

			static ::Uml::CompositionChildRole meta_HWElement_children;
			Udm::ChildrenAttr< ::EsMoL::HWElement> HWElement_children() const { return Udm::ChildrenAttr< ::EsMoL::HWElement>(impl, meta_HWElement_children); }
			template <class Pred> Udm::ChildrenAttr< ::EsMoL::HWElement, Pred> HWElement_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::HWElement, Pred>(impl, meta_HWElement_children); }

			static ::Uml::CompositionChildRole meta_Wire_children;
			Udm::ChildrenAttr< ::EsMoL::Wire> Wire_children() const { return Udm::ChildrenAttr< ::EsMoL::Wire>(impl, meta_Wire_children); }
			template <class Pred> Udm::ChildrenAttr< ::EsMoL::Wire, Pred> Wire_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::Wire, Pred>(impl, meta_Wire_children); }

			static ::Uml::CompositionChildRole meta_Channel_children;
			Udm::ChildrenAttr< ::EsMoL::Channel> Channel_children() const { return Udm::ChildrenAttr< ::EsMoL::Channel>(impl, meta_Channel_children); }
			template <class Pred> Udm::ChildrenAttr< ::EsMoL::Channel, Pred> Channel_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::Channel, Pred>(impl, meta_Channel_children); }

			Udm::ChildrenAttr< ::EsMoL::Channel> Channel_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::Channel>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::Channel, Pred> Channel_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::Channel, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::EsMoL::Node> Node_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::Node>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::Node, Pred> Node_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::Node, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::EsMoL::HWElement> HWElement_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::HWElement>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::HWElement, Pred> HWElement_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::HWElement, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::EsMoL::IChan> IChan_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::IChan>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::IChan, Pred> IChan_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::IChan, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::EsMoL::Bus> Bus_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::Bus>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::Bus, Pred> Bus_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::Bus, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::EsMoL::HardwareUnit> HardwareUnit_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::HardwareUnit>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::HardwareUnit, Pred> HardwareUnit_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::HardwareUnit, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::EsMoL::Connectable> Connectable_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::Connectable>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::Connectable, Pred> Connectable_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::Connectable, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::EsMoL::BChan> BChan_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::BChan>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::BChan, Pred> BChan_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::BChan, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::EsMoL::Wire> Wire_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::Wire>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::Wire, Pred> Wire_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::Wire, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::EsMoL::Router> Router_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::Router>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::Router, Pred> Router_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::Router, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::EsMoL::OChan> OChan_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::OChan>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::OChan, Pred> OChan_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::OChan, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::EsMoL::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::MgaObject>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

			static ::Uml::CompositionParentRole meta_PlatformLibrary_parent;
			Udm::ParentAttr< ::EsMoL::PlatformLibrary> PlatformLibrary_parent() const { return Udm::ParentAttr< ::EsMoL::PlatformLibrary>(impl, meta_PlatformLibrary_parent); }

			static ::Uml::CompositionParentRole meta_DesignFolder_parent;
			Udm::ParentAttr< ::EsMoL::DesignFolder> DesignFolder_parent() const { return Udm::ParentAttr< ::EsMoL::DesignFolder>(impl, meta_DesignFolder_parent); }

			Udm::ParentAttr<Udm::Object> parent() const { return Udm::ParentAttr<Udm::Object>(impl, Udm::NULLPARENTROLE); }
		};

		class  Connectable :  virtual public ::EsMoL::MgaObject {
		public:
			static ::Uml::Class meta;

			Connectable() { }
			Connectable(Udm::ObjectImpl *impl) : ::EsMoL::MgaObject(impl) { }
			Connectable(const Connectable &master) : ::EsMoL::MgaObject(master) { }
			static Connectable Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static Connectable Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			Connectable CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			Connectable CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::EsMoL::Connectable> Instances() { return Udm::InstantiatedAttr< ::EsMoL::Connectable>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::EsMoL::Connectable, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::EsMoL::Connectable, Pred>(impl);}

			Udm::DerivedAttr< ::EsMoL::Connectable> Derived() { return Udm::DerivedAttr< ::EsMoL::Connectable>(impl);}
			template <class Pred> Udm::DerivedAttr< ::EsMoL::Connectable, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::EsMoL::Connectable, Pred>(impl);}

			Udm::ArchetypeAttr< ::EsMoL::Connectable> Archetype() { return Udm::ArchetypeAttr< ::EsMoL::Connectable>(impl);}

			static ::Uml::AssociationRole meta_srcWire;
			static ::Uml::AssociationRole meta_srcWire_rev;
			Udm::AClassAssocAttr< ::EsMoL::Wire, ::EsMoL::Connectable> srcWire() const { return Udm::AClassAssocAttr< ::EsMoL::Wire, ::EsMoL::Connectable>(impl, meta_srcWire, meta_srcWire_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::EsMoL::Wire, ::EsMoL::Connectable, Pred> srcWire_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::EsMoL::Wire, ::EsMoL::Connectable, Pred>(impl, meta_srcWire, meta_srcWire_rev); }

			static ::Uml::AssociationRole meta_dstWire;
			static ::Uml::AssociationRole meta_dstWire_rev;
			Udm::AClassAssocAttr< ::EsMoL::Wire, ::EsMoL::Connectable> dstWire() const { return Udm::AClassAssocAttr< ::EsMoL::Wire, ::EsMoL::Connectable>(impl, meta_dstWire, meta_dstWire_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::EsMoL::Wire, ::EsMoL::Connectable, Pred> dstWire_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::EsMoL::Wire, ::EsMoL::Connectable, Pred>(impl, meta_dstWire, meta_dstWire_rev); }

			Udm::ParentAttr<Udm::Object> parent() const { return Udm::ParentAttr<Udm::Object>(impl, Udm::NULLPARENTROLE); }
		};

		class  Channel :  public ::EsMoL::Connectable {
		public:
			static ::Uml::Class meta;

			Channel() { }
			Channel(Udm::ObjectImpl *impl) : ::EsMoL::Connectable(impl), ::EsMoL::MgaObject(impl) { }
			Channel(const Channel &master) : ::EsMoL::Connectable(master), ::EsMoL::MgaObject(master) { }
			static Channel Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static Channel Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			Channel CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			Channel CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::EsMoL::Channel> Instances() { return Udm::InstantiatedAttr< ::EsMoL::Channel>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::EsMoL::Channel, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::EsMoL::Channel, Pred>(impl);}

			Udm::DerivedAttr< ::EsMoL::Channel> Derived() { return Udm::DerivedAttr< ::EsMoL::Channel>(impl);}
			template <class Pred> Udm::DerivedAttr< ::EsMoL::Channel, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::EsMoL::Channel, Pred>(impl);}

			Udm::ArchetypeAttr< ::EsMoL::Channel> Archetype() { return Udm::ArchetypeAttr< ::EsMoL::Channel>(impl);}

			static ::Uml::Attribute meta_InterruptNum;
			Udm::StringAttr InterruptNum() const { return Udm::StringAttr(impl, meta_InterruptNum); }

			static ::Uml::Attribute meta_CName;
			Udm::StringAttr CName() const { return Udm::StringAttr(impl, meta_CName); }

			static ::Uml::AssociationRole meta_referedbyChannelRef;
			Udm::AssocAttr< ::EsMoL::ChannelRef> referedbyChannelRef() const { return Udm::AssocAttr< ::EsMoL::ChannelRef>(impl, meta_referedbyChannelRef); }
			template <class Pred> Udm::AssocAttr< ::EsMoL::ChannelRef, Pred > referedbyChannelRef_sorted(const Pred &) const { return Udm::AssocAttr< ::EsMoL::ChannelRef, Pred>(impl, meta_referedbyChannelRef); }

			static ::Uml::CompositionParentRole meta_HardwareUnit_parent;
			Udm::ParentAttr< ::EsMoL::HardwareUnit> HardwareUnit_parent() const { return Udm::ParentAttr< ::EsMoL::HardwareUnit>(impl, meta_HardwareUnit_parent); }

			static ::Uml::CompositionParentRole meta_Node_parent;
			Udm::ParentAttr< ::EsMoL::Node> Node_parent() const { return Udm::ParentAttr< ::EsMoL::Node>(impl, meta_Node_parent); }

			Udm::ParentAttr< ::EsMoL::MgaObject> parent() const { return Udm::ParentAttr< ::EsMoL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  IChan :  public ::EsMoL::Channel {
		public:
			static ::Uml::Class meta;

			IChan() { }
			IChan(Udm::ObjectImpl *impl) : ::EsMoL::Channel(impl), ::EsMoL::MgaObject(impl) { }
			IChan(const IChan &master) : ::EsMoL::Channel(master), ::EsMoL::MgaObject(master) { }
			static IChan Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static IChan Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			IChan CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			IChan CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::EsMoL::IChan> Instances() { return Udm::InstantiatedAttr< ::EsMoL::IChan>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::EsMoL::IChan, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::EsMoL::IChan, Pred>(impl);}

			Udm::DerivedAttr< ::EsMoL::IChan> Derived() { return Udm::DerivedAttr< ::EsMoL::IChan>(impl);}
			template <class Pred> Udm::DerivedAttr< ::EsMoL::IChan, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::EsMoL::IChan, Pred>(impl);}

			Udm::ArchetypeAttr< ::EsMoL::IChan> Archetype() { return Udm::ArchetypeAttr< ::EsMoL::IChan>(impl);}

			Udm::ParentAttr< ::EsMoL::MgaObject> parent() const { return Udm::ParentAttr< ::EsMoL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  Bus :  public ::EsMoL::HWElement, public ::EsMoL::Connectable {
		public:
			static ::Uml::Class meta;

			Bus() { }
			Bus(Udm::ObjectImpl *impl) : ::EsMoL::HWElement(impl),::EsMoL::Connectable(impl), ::EsMoL::MgaObject(impl) { }
			Bus(const Bus &master) : ::EsMoL::HWElement(master),::EsMoL::Connectable(master), ::EsMoL::MgaObject(master) { }
			static Bus Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static Bus Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			Bus CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			Bus CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::EsMoL::Bus> Instances() { return Udm::InstantiatedAttr< ::EsMoL::Bus>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::EsMoL::Bus, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::EsMoL::Bus, Pred>(impl);}

			Udm::DerivedAttr< ::EsMoL::Bus> Derived() { return Udm::DerivedAttr< ::EsMoL::Bus>(impl);}
			template <class Pred> Udm::DerivedAttr< ::EsMoL::Bus, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::EsMoL::Bus, Pred>(impl);}

			Udm::ArchetypeAttr< ::EsMoL::Bus> Archetype() { return Udm::ArchetypeAttr< ::EsMoL::Bus>(impl);}

			static ::Uml::Attribute meta_FrameSize;
			Udm::IntegerAttr FrameSize() const { return Udm::IntegerAttr(impl, meta_FrameSize); }

			static ::Uml::Attribute meta_Medium;
			Udm::StringAttr Medium() const { return Udm::StringAttr(impl, meta_Medium); }

			static ::Uml::Attribute meta_BitRate;
			Udm::IntegerAttr BitRate() const { return Udm::IntegerAttr(impl, meta_BitRate); }

			static ::Uml::Attribute meta_CName;
			Udm::StringAttr CName() const { return Udm::StringAttr(impl, meta_CName); }

			Udm::ParentAttr< ::EsMoL::MgaObject> parent() const { return Udm::ParentAttr< ::EsMoL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  BChan :  public ::EsMoL::Channel {
		public:
			static ::Uml::Class meta;

			BChan() { }
			BChan(Udm::ObjectImpl *impl) : ::EsMoL::Channel(impl), ::EsMoL::MgaObject(impl) { }
			BChan(const BChan &master) : ::EsMoL::Channel(master), ::EsMoL::MgaObject(master) { }
			static BChan Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static BChan Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			BChan CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			BChan CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::EsMoL::BChan> Instances() { return Udm::InstantiatedAttr< ::EsMoL::BChan>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::EsMoL::BChan, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::EsMoL::BChan, Pred>(impl);}

			Udm::DerivedAttr< ::EsMoL::BChan> Derived() { return Udm::DerivedAttr< ::EsMoL::BChan>(impl);}
			template <class Pred> Udm::DerivedAttr< ::EsMoL::BChan, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::EsMoL::BChan, Pred>(impl);}

			Udm::ArchetypeAttr< ::EsMoL::BChan> Archetype() { return Udm::ArchetypeAttr< ::EsMoL::BChan>(impl);}

			static ::Uml::CompositionParentRole meta_Router_parent;
			Udm::ParentAttr< ::EsMoL::Router> Router_parent() const { return Udm::ParentAttr< ::EsMoL::Router>(impl, meta_Router_parent); }

			Udm::ParentAttr< ::EsMoL::MgaObject> parent() const { return Udm::ParentAttr< ::EsMoL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  Wire :  public ::EsMoL::MgaObject {
		public:
			static ::Uml::Class meta;

			Wire() { }
			Wire(Udm::ObjectImpl *impl) : ::EsMoL::MgaObject(impl) { }
			Wire(const Wire &master) : ::EsMoL::MgaObject(master) { }
			static Wire Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static Wire Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			Wire CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			Wire CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::EsMoL::Wire> Instances() { return Udm::InstantiatedAttr< ::EsMoL::Wire>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::EsMoL::Wire, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::EsMoL::Wire, Pred>(impl);}

			Udm::DerivedAttr< ::EsMoL::Wire> Derived() { return Udm::DerivedAttr< ::EsMoL::Wire>(impl);}
			template <class Pred> Udm::DerivedAttr< ::EsMoL::Wire, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::EsMoL::Wire, Pred>(impl);}

			Udm::ArchetypeAttr< ::EsMoL::Wire> Archetype() { return Udm::ArchetypeAttr< ::EsMoL::Wire>(impl);}

			static ::Uml::CompositionParentRole meta_HardwareUnit_parent;
			Udm::ParentAttr< ::EsMoL::HardwareUnit> HardwareUnit_parent() const { return Udm::ParentAttr< ::EsMoL::HardwareUnit>(impl, meta_HardwareUnit_parent); }

			Udm::ParentAttr< ::EsMoL::MgaObject> parent() const { return Udm::ParentAttr< ::EsMoL::MgaObject>(impl, Udm::NULLPARENTROLE); }
			static ::Uml::AssociationRole meta_dstWire_end_;
			Udm::AssocEndAttr< ::EsMoL::Connectable> dstWire_end() const { return Udm::AssocEndAttr< ::EsMoL::Connectable>(impl, meta_dstWire_end_); }

			static ::Uml::AssociationRole meta_srcWire_end_;
			Udm::AssocEndAttr< ::EsMoL::Connectable> srcWire_end() const { return Udm::AssocEndAttr< ::EsMoL::Connectable>(impl, meta_srcWire_end_); }

		};

		class  OS :  public ::EsMoL::MgaObject {
		public:
			static ::Uml::Class meta;

			OS() { }
			OS(Udm::ObjectImpl *impl) : ::EsMoL::MgaObject(impl) { }
			OS(const OS &master) : ::EsMoL::MgaObject(master) { }
			static OS Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static OS Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			OS CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			OS CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::EsMoL::OS> Instances() { return Udm::InstantiatedAttr< ::EsMoL::OS>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::EsMoL::OS, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::EsMoL::OS, Pred>(impl);}

			Udm::DerivedAttr< ::EsMoL::OS> Derived() { return Udm::DerivedAttr< ::EsMoL::OS>(impl);}
			template <class Pred> Udm::DerivedAttr< ::EsMoL::OS, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::EsMoL::OS, Pred>(impl);}

			Udm::ArchetypeAttr< ::EsMoL::OS> Archetype() { return Udm::ArchetypeAttr< ::EsMoL::OS>(impl);}

			static ::Uml::Attribute meta_CName;
			Udm::StringAttr CName() const { return Udm::StringAttr(impl, meta_CName); }

			static ::Uml::Attribute meta_TickTime;
			Udm::IntegerAttr TickTime() const { return Udm::IntegerAttr(impl, meta_TickTime); }

			static ::Uml::CompositionParentRole meta_Node_parent;
			Udm::ParentAttr< ::EsMoL::Node> Node_parent() const { return Udm::ParentAttr< ::EsMoL::Node>(impl, meta_Node_parent); }

			Udm::ParentAttr< ::EsMoL::MgaObject> parent() const { return Udm::ParentAttr< ::EsMoL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  Router :  public ::EsMoL::HWElement {
		public:
			static ::Uml::Class meta;

			Router() { }
			Router(Udm::ObjectImpl *impl) : ::EsMoL::HWElement(impl), ::EsMoL::MgaObject(impl) { }
			Router(const Router &master) : ::EsMoL::HWElement(master), ::EsMoL::MgaObject(master) { }
			static Router Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static Router Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			Router CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			Router CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::EsMoL::Router> Instances() { return Udm::InstantiatedAttr< ::EsMoL::Router>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::EsMoL::Router, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::EsMoL::Router, Pred>(impl);}

			Udm::DerivedAttr< ::EsMoL::Router> Derived() { return Udm::DerivedAttr< ::EsMoL::Router>(impl);}
			template <class Pred> Udm::DerivedAttr< ::EsMoL::Router, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::EsMoL::Router, Pred>(impl);}

			Udm::ArchetypeAttr< ::EsMoL::Router> Archetype() { return Udm::ArchetypeAttr< ::EsMoL::Router>(impl);}

			static ::Uml::CompositionChildRole meta_BChan_children;
			Udm::ChildrenAttr< ::EsMoL::BChan> BChan_children() const { return Udm::ChildrenAttr< ::EsMoL::BChan>(impl, meta_BChan_children); }
			template <class Pred> Udm::ChildrenAttr< ::EsMoL::BChan, Pred> BChan_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::BChan, Pred>(impl, meta_BChan_children); }

			Udm::ChildrenAttr< ::EsMoL::Channel> Channel_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::Channel>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::Channel, Pred> Channel_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::Channel, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::EsMoL::Connectable> Connectable_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::Connectable>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::Connectable, Pred> Connectable_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::Connectable, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::EsMoL::BChan> BChan_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::BChan>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::BChan, Pred> BChan_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::BChan, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::EsMoL::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::MgaObject>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ParentAttr< ::EsMoL::MgaObject> parent() const { return Udm::ParentAttr< ::EsMoL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  Capability :  public ::EsMoL::MgaObject {
		public:
			static ::Uml::Class meta;

			Capability() { }
			Capability(Udm::ObjectImpl *impl) : ::EsMoL::MgaObject(impl) { }
			Capability(const Capability &master) : ::EsMoL::MgaObject(master) { }
			static Capability Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static Capability Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			Capability CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			Capability CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::EsMoL::Capability> Instances() { return Udm::InstantiatedAttr< ::EsMoL::Capability>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::EsMoL::Capability, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::EsMoL::Capability, Pred>(impl);}

			Udm::DerivedAttr< ::EsMoL::Capability> Derived() { return Udm::DerivedAttr< ::EsMoL::Capability>(impl);}
			template <class Pred> Udm::DerivedAttr< ::EsMoL::Capability, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::EsMoL::Capability, Pred>(impl);}

			Udm::ArchetypeAttr< ::EsMoL::Capability> Archetype() { return Udm::ArchetypeAttr< ::EsMoL::Capability>(impl);}

			static ::Uml::CompositionParentRole meta_HWElement_parent;
			Udm::ParentAttr< ::EsMoL::HWElement> HWElement_parent() const { return Udm::ParentAttr< ::EsMoL::HWElement>(impl, meta_HWElement_parent); }

			Udm::ParentAttr< ::EsMoL::MgaObject> parent() const { return Udm::ParentAttr< ::EsMoL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  OChan :  public ::EsMoL::Channel {
		public:
			static ::Uml::Class meta;

			OChan() { }
			OChan(Udm::ObjectImpl *impl) : ::EsMoL::Channel(impl), ::EsMoL::MgaObject(impl) { }
			OChan(const OChan &master) : ::EsMoL::Channel(master), ::EsMoL::MgaObject(master) { }
			static OChan Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static OChan Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			OChan CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			OChan CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::EsMoL::OChan> Instances() { return Udm::InstantiatedAttr< ::EsMoL::OChan>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::EsMoL::OChan, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::EsMoL::OChan, Pred>(impl);}

			Udm::DerivedAttr< ::EsMoL::OChan> Derived() { return Udm::DerivedAttr< ::EsMoL::OChan>(impl);}
			template <class Pred> Udm::DerivedAttr< ::EsMoL::OChan, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::EsMoL::OChan, Pred>(impl);}

			Udm::ArchetypeAttr< ::EsMoL::OChan> Archetype() { return Udm::ArchetypeAttr< ::EsMoL::OChan>(impl);}

			Udm::ParentAttr< ::EsMoL::MgaObject> parent() const { return Udm::ParentAttr< ::EsMoL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  Port :  virtual public ::EsMoL::MgaObject {
		public:
			static ::Uml::Class meta;

			Port() { }
			Port(Udm::ObjectImpl *impl) : ::EsMoL::MgaObject(impl) { }
			Port(const Port &master) : ::EsMoL::MgaObject(master) { }
			static Port Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static Port Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			Port CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			Port CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::EsMoL::Port> Instances() { return Udm::InstantiatedAttr< ::EsMoL::Port>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::EsMoL::Port, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::EsMoL::Port, Pred>(impl);}

			Udm::DerivedAttr< ::EsMoL::Port> Derived() { return Udm::DerivedAttr< ::EsMoL::Port>(impl);}
			template <class Pred> Udm::DerivedAttr< ::EsMoL::Port, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::EsMoL::Port, Pred>(impl);}

			Udm::ArchetypeAttr< ::EsMoL::Port> Archetype() { return Udm::ArchetypeAttr< ::EsMoL::Port>(impl);}

			static ::Uml::AssociationRole meta_srcLine;
			static ::Uml::AssociationRole meta_srcLine_rev;
			Udm::AClassAssocAttr< ::EsMoL::Line, ::EsMoL::Port> srcLine() const { return Udm::AClassAssocAttr< ::EsMoL::Line, ::EsMoL::Port>(impl, meta_srcLine, meta_srcLine_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::EsMoL::Line, ::EsMoL::Port, Pred> srcLine_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::EsMoL::Line, ::EsMoL::Port, Pred>(impl, meta_srcLine, meta_srcLine_rev); }

			static ::Uml::AssociationRole meta_dstLine;
			static ::Uml::AssociationRole meta_dstLine_rev;
			Udm::AClassAssocAttr< ::EsMoL::Line, ::EsMoL::Port> dstLine() const { return Udm::AClassAssocAttr< ::EsMoL::Line, ::EsMoL::Port>(impl, meta_dstLine, meta_dstLine_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::EsMoL::Line, ::EsMoL::Port, Pred> dstLine_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::EsMoL::Line, ::EsMoL::Port, Pred>(impl, meta_dstLine, meta_dstLine_rev); }

			static ::Uml::AssociationRole meta_dstConnector;
			static ::Uml::AssociationRole meta_dstConnector_rev;
			Udm::AClassAssocAttr< ::EsMoL::Connector, ::EsMoL::Port> dstConnector() const { return Udm::AClassAssocAttr< ::EsMoL::Connector, ::EsMoL::Port>(impl, meta_dstConnector, meta_dstConnector_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::EsMoL::Connector, ::EsMoL::Port, Pred> dstConnector_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::EsMoL::Connector, ::EsMoL::Port, Pred>(impl, meta_dstConnector, meta_dstConnector_rev); }

			static ::Uml::AssociationRole meta_srcConnector;
			static ::Uml::AssociationRole meta_srcConnector_rev;
			Udm::AClassAssocAttr< ::EsMoL::Connector, ::EsMoL::Port> srcConnector() const { return Udm::AClassAssocAttr< ::EsMoL::Connector, ::EsMoL::Port>(impl, meta_srcConnector, meta_srcConnector_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::EsMoL::Connector, ::EsMoL::Port, Pred> srcConnector_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::EsMoL::Connector, ::EsMoL::Port, Pred>(impl, meta_srcConnector, meta_srcConnector_rev); }

			static ::Uml::CompositionChildRole meta_TypeBaseRef_child;
			Udm::ChildAttr< ::EsMoL::TypeBaseRef> TypeBaseRef_child() const { return Udm::ChildAttr< ::EsMoL::TypeBaseRef>(impl, meta_TypeBaseRef_child); }

			Udm::ChildrenAttr< ::EsMoL::TypeBaseRef> TypeBaseRef_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::TypeBaseRef>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::TypeBaseRef, Pred> TypeBaseRef_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::TypeBaseRef, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::EsMoL::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::MgaObject>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

			static ::Uml::CompositionParentRole meta_CCode_parent;
			Udm::ParentAttr< ::EsMoL::CCode> CCode_parent() const { return Udm::ParentAttr< ::EsMoL::CCode>(impl, meta_CCode_parent); }

			static ::Uml::CompositionParentRole meta_Block_parent;
			Udm::ParentAttr< ::EsMoL::Block> Block_parent() const { return Udm::ParentAttr< ::EsMoL::Block>(impl, meta_Block_parent); }

			Udm::ParentAttr< ::EsMoL::MgaObject> parent() const { return Udm::ParentAttr< ::EsMoL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  InPort :  public ::EsMoL::Port {
		public:
			static ::Uml::Class meta;

			InPort() { }
			InPort(Udm::ObjectImpl *impl) : ::EsMoL::Port(impl), ::EsMoL::MgaObject(impl) { }
			InPort(const InPort &master) : ::EsMoL::Port(master), ::EsMoL::MgaObject(master) { }
			static InPort Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static InPort Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			InPort CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			InPort CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::EsMoL::InPort> Instances() { return Udm::InstantiatedAttr< ::EsMoL::InPort>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::EsMoL::InPort, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::EsMoL::InPort, Pred>(impl);}

			Udm::DerivedAttr< ::EsMoL::InPort> Derived() { return Udm::DerivedAttr< ::EsMoL::InPort>(impl);}
			template <class Pred> Udm::DerivedAttr< ::EsMoL::InPort, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::EsMoL::InPort, Pred>(impl);}

			Udm::ArchetypeAttr< ::EsMoL::InPort> Archetype() { return Udm::ArchetypeAttr< ::EsMoL::InPort>(impl);}

			Udm::ParentAttr< ::EsMoL::MgaObject> parent() const { return Udm::ParentAttr< ::EsMoL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  ActionPort :  public ::EsMoL::InPort {
		public:
			static ::Uml::Class meta;

			ActionPort() { }
			ActionPort(Udm::ObjectImpl *impl) : ::EsMoL::InPort(impl), ::EsMoL::MgaObject(impl) { }
			ActionPort(const ActionPort &master) : ::EsMoL::InPort(master), ::EsMoL::MgaObject(master) { }
			static ActionPort Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static ActionPort Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			ActionPort CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			ActionPort CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::EsMoL::ActionPort> Instances() { return Udm::InstantiatedAttr< ::EsMoL::ActionPort>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::EsMoL::ActionPort, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::EsMoL::ActionPort, Pred>(impl);}

			Udm::DerivedAttr< ::EsMoL::ActionPort> Derived() { return Udm::DerivedAttr< ::EsMoL::ActionPort>(impl);}
			template <class Pred> Udm::DerivedAttr< ::EsMoL::ActionPort, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::EsMoL::ActionPort, Pred>(impl);}

			Udm::ArchetypeAttr< ::EsMoL::ActionPort> Archetype() { return Udm::ArchetypeAttr< ::EsMoL::ActionPort>(impl);}

			Udm::ParentAttr< ::EsMoL::MgaObject> parent() const { return Udm::ParentAttr< ::EsMoL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  EnablePort :  public ::EsMoL::InPort {
		public:
			static ::Uml::Class meta;

			EnablePort() { }
			EnablePort(Udm::ObjectImpl *impl) : ::EsMoL::InPort(impl), ::EsMoL::MgaObject(impl) { }
			EnablePort(const EnablePort &master) : ::EsMoL::InPort(master), ::EsMoL::MgaObject(master) { }
			static EnablePort Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static EnablePort Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			EnablePort CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			EnablePort CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::EsMoL::EnablePort> Instances() { return Udm::InstantiatedAttr< ::EsMoL::EnablePort>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::EsMoL::EnablePort, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::EsMoL::EnablePort, Pred>(impl);}

			Udm::DerivedAttr< ::EsMoL::EnablePort> Derived() { return Udm::DerivedAttr< ::EsMoL::EnablePort>(impl);}
			template <class Pred> Udm::DerivedAttr< ::EsMoL::EnablePort, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::EsMoL::EnablePort, Pred>(impl);}

			Udm::ArchetypeAttr< ::EsMoL::EnablePort> Archetype() { return Udm::ArchetypeAttr< ::EsMoL::EnablePort>(impl);}

			static ::Uml::Attribute meta_StatesWhenEnabling;
			Udm::StringAttr StatesWhenEnabling() const { return Udm::StringAttr(impl, meta_StatesWhenEnabling); }

			Udm::ParentAttr< ::EsMoL::MgaObject> parent() const { return Udm::ParentAttr< ::EsMoL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  TriggerPort :  public ::EsMoL::InPort {
		public:
			static ::Uml::Class meta;

			TriggerPort() { }
			TriggerPort(Udm::ObjectImpl *impl) : ::EsMoL::InPort(impl), ::EsMoL::MgaObject(impl) { }
			TriggerPort(const TriggerPort &master) : ::EsMoL::InPort(master), ::EsMoL::MgaObject(master) { }
			static TriggerPort Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static TriggerPort Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			TriggerPort CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			TriggerPort CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::EsMoL::TriggerPort> Instances() { return Udm::InstantiatedAttr< ::EsMoL::TriggerPort>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::EsMoL::TriggerPort, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::EsMoL::TriggerPort, Pred>(impl);}

			Udm::DerivedAttr< ::EsMoL::TriggerPort> Derived() { return Udm::DerivedAttr< ::EsMoL::TriggerPort>(impl);}
			template <class Pred> Udm::DerivedAttr< ::EsMoL::TriggerPort, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::EsMoL::TriggerPort, Pred>(impl);}

			Udm::ArchetypeAttr< ::EsMoL::TriggerPort> Archetype() { return Udm::ArchetypeAttr< ::EsMoL::TriggerPort>(impl);}

			static ::Uml::Attribute meta_TriggerType;
			Udm::StringAttr TriggerType() const { return Udm::StringAttr(impl, meta_TriggerType); }

			Udm::ParentAttr< ::EsMoL::MgaObject> parent() const { return Udm::ParentAttr< ::EsMoL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  Block :  virtual public ::EsMoL::MgaObject {
		public:
			static ::Uml::Class meta;

			Block() { }
			Block(Udm::ObjectImpl *impl) : ::EsMoL::MgaObject(impl) { }
			Block(const Block &master) : ::EsMoL::MgaObject(master) { }
			static Block Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static Block Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			Block CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			Block CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::EsMoL::Block> Instances() { return Udm::InstantiatedAttr< ::EsMoL::Block>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::EsMoL::Block, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::EsMoL::Block, Pred>(impl);}

			Udm::DerivedAttr< ::EsMoL::Block> Derived() { return Udm::DerivedAttr< ::EsMoL::Block>(impl);}
			template <class Pred> Udm::DerivedAttr< ::EsMoL::Block, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::EsMoL::Block, Pred>(impl);}

			Udm::ArchetypeAttr< ::EsMoL::Block> Archetype() { return Udm::ArchetypeAttr< ::EsMoL::Block>(impl);}

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

			static ::Uml::CompositionChildRole meta_Parameter;
			Udm::ChildrenAttr< ::EsMoL::Parameter> Parameter() const { return Udm::ChildrenAttr< ::EsMoL::Parameter>(impl, meta_Parameter); }
			template <class Pred> Udm::ChildrenAttr< ::EsMoL::Parameter, Pred> Parameter_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::Parameter, Pred>(impl, meta_Parameter); }

			static ::Uml::CompositionChildRole meta_SLAnnotation_children;
			Udm::ChildrenAttr< ::EsMoL::SLAnnotation> SLAnnotation_children() const { return Udm::ChildrenAttr< ::EsMoL::SLAnnotation>(impl, meta_SLAnnotation_children); }
			template <class Pred> Udm::ChildrenAttr< ::EsMoL::SLAnnotation, Pred> SLAnnotation_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::SLAnnotation, Pred>(impl, meta_SLAnnotation_children); }

			static ::Uml::CompositionChildRole meta_ConnectorRef_children;
			Udm::ChildrenAttr< ::EsMoL::ConnectorRef> ConnectorRef_children() const { return Udm::ChildrenAttr< ::EsMoL::ConnectorRef>(impl, meta_ConnectorRef_children); }
			template <class Pred> Udm::ChildrenAttr< ::EsMoL::ConnectorRef, Pred> ConnectorRef_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::ConnectorRef, Pred>(impl, meta_ConnectorRef_children); }

			static ::Uml::CompositionChildRole meta_Port_children;
			Udm::ChildrenAttr< ::EsMoL::Port> Port_children() const { return Udm::ChildrenAttr< ::EsMoL::Port>(impl, meta_Port_children); }
			template <class Pred> Udm::ChildrenAttr< ::EsMoL::Port, Pred> Port_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::Port, Pred>(impl, meta_Port_children); }

			static ::Uml::CompositionChildRole meta_Line;
			Udm::ChildrenAttr< ::EsMoL::Line> Line() const { return Udm::ChildrenAttr< ::EsMoL::Line>(impl, meta_Line); }
			template <class Pred> Udm::ChildrenAttr< ::EsMoL::Line, Pred> Line_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::Line, Pred>(impl, meta_Line); }

			Udm::ChildrenAttr< ::EsMoL::ActionPort> ActionPort_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::ActionPort>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::ActionPort, Pred> ActionPort_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::ActionPort, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::EsMoL::EnablePort> EnablePort_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::EnablePort>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::EnablePort, Pred> EnablePort_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::EnablePort, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::EsMoL::Port> Port_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::Port>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::Port, Pred> Port_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::Port, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::EsMoL::InPort> InPort_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::InPort>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::InPort, Pred> InPort_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::InPort, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::EsMoL::TriggerPort> TriggerPort_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::TriggerPort>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::TriggerPort, Pred> TriggerPort_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::TriggerPort, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::EsMoL::Parameter> Parameter_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::Parameter>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::Parameter, Pred> Parameter_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::Parameter, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::EsMoL::SLAnnotation> SLAnnotation_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::SLAnnotation>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::SLAnnotation, Pred> SLAnnotation_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::SLAnnotation, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::EsMoL::Line> Line_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::Line>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::Line, Pred> Line_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::Line, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::EsMoL::OutPort> OutPort_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::OutPort>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::OutPort, Pred> OutPort_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::OutPort, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::EsMoL::StatePort> StatePort_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::StatePort>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::StatePort, Pred> StatePort_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::StatePort, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::EsMoL::OutputPort> OutputPort_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::OutputPort>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::OutputPort, Pred> OutputPort_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::OutputPort, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::EsMoL::InputPort> InputPort_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::InputPort>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::InputPort, Pred> InputPort_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::InputPort, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::EsMoL::ConnectorRef> ConnectorRef_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::ConnectorRef>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::ConnectorRef, Pred> ConnectorRef_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::ConnectorRef, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::EsMoL::TransConnector> TransConnector_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::TransConnector>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::TransConnector, Pred> TransConnector_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::TransConnector, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::EsMoL::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::MgaObject>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

			static ::Uml::CompositionParentRole meta_Subsystem_parent;
			Udm::ParentAttr< ::EsMoL::Subsystem> Subsystem_parent() const { return Udm::ParentAttr< ::EsMoL::Subsystem>(impl, meta_Subsystem_parent); }

			Udm::ParentAttr< ::EsMoL::MgaObject> parent() const { return Udm::ParentAttr< ::EsMoL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  Primitive :  public ::EsMoL::Block {
		public:
			static ::Uml::Class meta;

			Primitive() { }
			Primitive(Udm::ObjectImpl *impl) : ::EsMoL::Block(impl), ::EsMoL::MgaObject(impl) { }
			Primitive(const Primitive &master) : ::EsMoL::Block(master), ::EsMoL::MgaObject(master) { }
			static Primitive Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static Primitive Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			Primitive CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			Primitive CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::EsMoL::Primitive> Instances() { return Udm::InstantiatedAttr< ::EsMoL::Primitive>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::EsMoL::Primitive, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::EsMoL::Primitive, Pred>(impl);}

			Udm::DerivedAttr< ::EsMoL::Primitive> Derived() { return Udm::DerivedAttr< ::EsMoL::Primitive>(impl);}
			template <class Pred> Udm::DerivedAttr< ::EsMoL::Primitive, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::EsMoL::Primitive, Pred>(impl);}

			Udm::ArchetypeAttr< ::EsMoL::Primitive> Archetype() { return Udm::ArchetypeAttr< ::EsMoL::Primitive>(impl);}

			Udm::ParentAttr< ::EsMoL::MgaObject> parent() const { return Udm::ParentAttr< ::EsMoL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  Parameter :  public ::EsMoL::MgaObject {
		public:
			static ::Uml::Class meta;

			Parameter() { }
			Parameter(Udm::ObjectImpl *impl) : ::EsMoL::MgaObject(impl) { }
			Parameter(const Parameter &master) : ::EsMoL::MgaObject(master) { }
			static Parameter Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static Parameter Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			Parameter CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			Parameter CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::EsMoL::Parameter> Instances() { return Udm::InstantiatedAttr< ::EsMoL::Parameter>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::EsMoL::Parameter, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::EsMoL::Parameter, Pred>(impl);}

			Udm::DerivedAttr< ::EsMoL::Parameter> Derived() { return Udm::DerivedAttr< ::EsMoL::Parameter>(impl);}
			template <class Pred> Udm::DerivedAttr< ::EsMoL::Parameter, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::EsMoL::Parameter, Pred>(impl);}

			Udm::ArchetypeAttr< ::EsMoL::Parameter> Archetype() { return Udm::ArchetypeAttr< ::EsMoL::Parameter>(impl);}

			static ::Uml::Attribute meta_Value;
			Udm::StringAttr Value() const { return Udm::StringAttr(impl, meta_Value); }

			static ::Uml::CompositionParentRole meta_Parameter_Block_parent;
			Udm::ParentAttr< ::EsMoL::Block> Parameter_Block_parent() const { return Udm::ParentAttr< ::EsMoL::Block>(impl, meta_Parameter_Block_parent); }

			Udm::ParentAttr< ::EsMoL::MgaObject> parent() const { return Udm::ParentAttr< ::EsMoL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  SLReference :  public ::EsMoL::Block {
		public:
			static ::Uml::Class meta;

			SLReference() { }
			SLReference(Udm::ObjectImpl *impl) : ::EsMoL::Block(impl), ::EsMoL::MgaObject(impl) { }
			SLReference(const SLReference &master) : ::EsMoL::Block(master), ::EsMoL::MgaObject(master) { }
			static SLReference Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static SLReference Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			SLReference CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			SLReference CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::EsMoL::SLReference> Instances() { return Udm::InstantiatedAttr< ::EsMoL::SLReference>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::EsMoL::SLReference, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::EsMoL::SLReference, Pred>(impl);}

			Udm::DerivedAttr< ::EsMoL::SLReference> Derived() { return Udm::DerivedAttr< ::EsMoL::SLReference>(impl);}
			template <class Pred> Udm::DerivedAttr< ::EsMoL::SLReference, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::EsMoL::SLReference, Pred>(impl);}

			Udm::ArchetypeAttr< ::EsMoL::SLReference> Archetype() { return Udm::ArchetypeAttr< ::EsMoL::SLReference>(impl);}

			static ::Uml::Attribute meta_SourceBlock;
			Udm::StringAttr SourceBlock() const { return Udm::StringAttr(impl, meta_SourceBlock); }

			static ::Uml::Attribute meta_SourceType;
			Udm::StringAttr SourceType() const { return Udm::StringAttr(impl, meta_SourceType); }

			Udm::ParentAttr< ::EsMoL::MgaObject> parent() const { return Udm::ParentAttr< ::EsMoL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  SLAnnotation :  public ::EsMoL::MgaObject {
		public:
			static ::Uml::Class meta;

			SLAnnotation() { }
			SLAnnotation(Udm::ObjectImpl *impl) : ::EsMoL::MgaObject(impl) { }
			SLAnnotation(const SLAnnotation &master) : ::EsMoL::MgaObject(master) { }
			static SLAnnotation Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static SLAnnotation Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			SLAnnotation CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			SLAnnotation CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::EsMoL::SLAnnotation> Instances() { return Udm::InstantiatedAttr< ::EsMoL::SLAnnotation>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::EsMoL::SLAnnotation, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::EsMoL::SLAnnotation, Pred>(impl);}

			Udm::DerivedAttr< ::EsMoL::SLAnnotation> Derived() { return Udm::DerivedAttr< ::EsMoL::SLAnnotation>(impl);}
			template <class Pred> Udm::DerivedAttr< ::EsMoL::SLAnnotation, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::EsMoL::SLAnnotation, Pred>(impl);}

			Udm::ArchetypeAttr< ::EsMoL::SLAnnotation> Archetype() { return Udm::ArchetypeAttr< ::EsMoL::SLAnnotation>(impl);}

			static ::Uml::Attribute meta_Text;
			Udm::StringAttr Text() const { return Udm::StringAttr(impl, meta_Text); }

			static ::Uml::CompositionParentRole meta_Block_parent;
			Udm::ParentAttr< ::EsMoL::Block> Block_parent() const { return Udm::ParentAttr< ::EsMoL::Block>(impl, meta_Block_parent); }

			Udm::ParentAttr< ::EsMoL::MgaObject> parent() const { return Udm::ParentAttr< ::EsMoL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  Line :  public ::EsMoL::MgaObject {
		public:
			static ::Uml::Class meta;

			Line() { }
			Line(Udm::ObjectImpl *impl) : ::EsMoL::MgaObject(impl) { }
			Line(const Line &master) : ::EsMoL::MgaObject(master) { }
			static Line Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static Line Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			Line CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			Line CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::EsMoL::Line> Instances() { return Udm::InstantiatedAttr< ::EsMoL::Line>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::EsMoL::Line, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::EsMoL::Line, Pred>(impl);}

			Udm::DerivedAttr< ::EsMoL::Line> Derived() { return Udm::DerivedAttr< ::EsMoL::Line>(impl);}
			template <class Pred> Udm::DerivedAttr< ::EsMoL::Line, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::EsMoL::Line, Pred>(impl);}

			Udm::ArchetypeAttr< ::EsMoL::Line> Archetype() { return Udm::ArchetypeAttr< ::EsMoL::Line>(impl);}

			static ::Uml::Attribute meta_Name;
			Udm::StringAttr Name() const { return Udm::StringAttr(impl, meta_Name); }

			static ::Uml::CompositionParentRole meta_Line_Block_parent;
			Udm::ParentAttr< ::EsMoL::Block> Line_Block_parent() const { return Udm::ParentAttr< ::EsMoL::Block>(impl, meta_Line_Block_parent); }

			Udm::ParentAttr< ::EsMoL::MgaObject> parent() const { return Udm::ParentAttr< ::EsMoL::MgaObject>(impl, Udm::NULLPARENTROLE); }
			static ::Uml::AssociationRole meta_dstLine_end_;
			Udm::AssocEndAttr< ::EsMoL::Port> dstLine_end() const { return Udm::AssocEndAttr< ::EsMoL::Port>(impl, meta_dstLine_end_); }

			static ::Uml::AssociationRole meta_srcLine_end_;
			Udm::AssocEndAttr< ::EsMoL::Port> srcLine_end() const { return Udm::AssocEndAttr< ::EsMoL::Port>(impl, meta_srcLine_end_); }

		};

		class  OutPort :  public ::EsMoL::Port {
		public:
			static ::Uml::Class meta;

			OutPort() { }
			OutPort(Udm::ObjectImpl *impl) : ::EsMoL::Port(impl), ::EsMoL::MgaObject(impl) { }
			OutPort(const OutPort &master) : ::EsMoL::Port(master), ::EsMoL::MgaObject(master) { }
			static OutPort Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static OutPort Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			OutPort CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			OutPort CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::EsMoL::OutPort> Instances() { return Udm::InstantiatedAttr< ::EsMoL::OutPort>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::EsMoL::OutPort, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::EsMoL::OutPort, Pred>(impl);}

			Udm::DerivedAttr< ::EsMoL::OutPort> Derived() { return Udm::DerivedAttr< ::EsMoL::OutPort>(impl);}
			template <class Pred> Udm::DerivedAttr< ::EsMoL::OutPort, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::EsMoL::OutPort, Pred>(impl);}

			Udm::ArchetypeAttr< ::EsMoL::OutPort> Archetype() { return Udm::ArchetypeAttr< ::EsMoL::OutPort>(impl);}

			Udm::ParentAttr< ::EsMoL::MgaObject> parent() const { return Udm::ParentAttr< ::EsMoL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  StatePort :  public ::EsMoL::OutPort {
		public:
			static ::Uml::Class meta;

			StatePort() { }
			StatePort(Udm::ObjectImpl *impl) : ::EsMoL::OutPort(impl), ::EsMoL::MgaObject(impl) { }
			StatePort(const StatePort &master) : ::EsMoL::OutPort(master), ::EsMoL::MgaObject(master) { }
			static StatePort Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static StatePort Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			StatePort CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			StatePort CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::EsMoL::StatePort> Instances() { return Udm::InstantiatedAttr< ::EsMoL::StatePort>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::EsMoL::StatePort, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::EsMoL::StatePort, Pred>(impl);}

			Udm::DerivedAttr< ::EsMoL::StatePort> Derived() { return Udm::DerivedAttr< ::EsMoL::StatePort>(impl);}
			template <class Pred> Udm::DerivedAttr< ::EsMoL::StatePort, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::EsMoL::StatePort, Pred>(impl);}

			Udm::ArchetypeAttr< ::EsMoL::StatePort> Archetype() { return Udm::ArchetypeAttr< ::EsMoL::StatePort>(impl);}

			Udm::ParentAttr< ::EsMoL::MgaObject> parent() const { return Udm::ParentAttr< ::EsMoL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  Component :  public ::EsMoL::MgaObject {
		public:
			static ::Uml::Class meta;

			Component() { }
			Component(Udm::ObjectImpl *impl) : ::EsMoL::MgaObject(impl) { }
			Component(const Component &master) : ::EsMoL::MgaObject(master) { }
			static Component Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static Component Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			Component CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			Component CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::EsMoL::Component> Instances() { return Udm::InstantiatedAttr< ::EsMoL::Component>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::EsMoL::Component, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::EsMoL::Component, Pred>(impl);}

			Udm::DerivedAttr< ::EsMoL::Component> Derived() { return Udm::DerivedAttr< ::EsMoL::Component>(impl);}
			template <class Pred> Udm::DerivedAttr< ::EsMoL::Component, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::EsMoL::Component, Pred>(impl);}

			Udm::ArchetypeAttr< ::EsMoL::Component> Archetype() { return Udm::ArchetypeAttr< ::EsMoL::Component>(impl);}

			static ::Uml::Attribute meta_WCET;
			Udm::StringAttr WCET() const { return Udm::StringAttr(impl, meta_WCET); }

			static ::Uml::AssociationRole meta_ref;
			Udm::PointerAttr< ::EsMoL::ComponentBase> ref() const { return Udm::PointerAttr< ::EsMoL::ComponentBase>(impl, meta_ref); }

			static ::Uml::CompositionParentRole meta_Task_parent;
			Udm::ParentAttr< ::EsMoL::Task> Task_parent() const { return Udm::ParentAttr< ::EsMoL::Task>(impl, meta_Task_parent); }

			Udm::ParentAttr< ::EsMoL::MgaObject> parent() const { return Udm::ParentAttr< ::EsMoL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  Deployment :  public ::EsMoL::MgaObject {
		public:
			static ::Uml::Class meta;

			Deployment() { }
			Deployment(Udm::ObjectImpl *impl) : ::EsMoL::MgaObject(impl) { }
			Deployment(const Deployment &master) : ::EsMoL::MgaObject(master) { }
			static Deployment Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static Deployment Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			Deployment CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			Deployment CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::EsMoL::Deployment> Instances() { return Udm::InstantiatedAttr< ::EsMoL::Deployment>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::EsMoL::Deployment, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::EsMoL::Deployment, Pred>(impl);}

			Udm::DerivedAttr< ::EsMoL::Deployment> Derived() { return Udm::DerivedAttr< ::EsMoL::Deployment>(impl);}
			template <class Pred> Udm::DerivedAttr< ::EsMoL::Deployment, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::EsMoL::Deployment, Pred>(impl);}

			Udm::ArchetypeAttr< ::EsMoL::Deployment> Archetype() { return Udm::ArchetypeAttr< ::EsMoL::Deployment>(impl);}

			static ::Uml::CompositionChildRole meta_TaskAssignment_children;
			Udm::ChildrenAttr< ::EsMoL::TaskAssignment> TaskAssignment_children() const { return Udm::ChildrenAttr< ::EsMoL::TaskAssignment>(impl, meta_TaskAssignment_children); }
			template <class Pred> Udm::ChildrenAttr< ::EsMoL::TaskAssignment, Pred> TaskAssignment_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::TaskAssignment, Pred>(impl, meta_TaskAssignment_children); }

			static ::Uml::CompositionChildRole meta_NodeRef_children;
			Udm::ChildrenAttr< ::EsMoL::NodeRef> NodeRef_children() const { return Udm::ChildrenAttr< ::EsMoL::NodeRef>(impl, meta_NodeRef_children); }
			template <class Pred> Udm::ChildrenAttr< ::EsMoL::NodeRef, Pred> NodeRef_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::NodeRef, Pred>(impl, meta_NodeRef_children); }

			static ::Uml::CompositionChildRole meta_Task_children;
			Udm::ChildrenAttr< ::EsMoL::Task> Task_children() const { return Udm::ChildrenAttr< ::EsMoL::Task>(impl, meta_Task_children); }
			template <class Pred> Udm::ChildrenAttr< ::EsMoL::Task, Pred> Task_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::Task, Pred>(impl, meta_Task_children); }

			Udm::ChildrenAttr< ::EsMoL::TaskAssignment> TaskAssignment_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::TaskAssignment>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::TaskAssignment, Pred> TaskAssignment_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::TaskAssignment, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::EsMoL::Task> Task_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::Task>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::Task, Pred> Task_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::Task, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::EsMoL::NodeRef> NodeRef_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::NodeRef>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::NodeRef, Pred> NodeRef_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::NodeRef, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::EsMoL::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::MgaObject>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

			static ::Uml::CompositionParentRole meta_DesignFolder_parent;
			Udm::ParentAttr< ::EsMoL::DesignFolder> DesignFolder_parent() const { return Udm::ParentAttr< ::EsMoL::DesignFolder>(impl, meta_DesignFolder_parent); }

			static ::Uml::CompositionParentRole meta_DeploymentLibrary_parent;
			Udm::ParentAttr< ::EsMoL::DeploymentLibrary> DeploymentLibrary_parent() const { return Udm::ParentAttr< ::EsMoL::DeploymentLibrary>(impl, meta_DeploymentLibrary_parent); }

			Udm::ParentAttr<Udm::Object> parent() const { return Udm::ParentAttr<Udm::Object>(impl, Udm::NULLPARENTROLE); }
		};

		class  TaskAssignment :  public ::EsMoL::MgaObject {
		public:
			static ::Uml::Class meta;

			TaskAssignment() { }
			TaskAssignment(Udm::ObjectImpl *impl) : ::EsMoL::MgaObject(impl) { }
			TaskAssignment(const TaskAssignment &master) : ::EsMoL::MgaObject(master) { }
			static TaskAssignment Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static TaskAssignment Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			TaskAssignment CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			TaskAssignment CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::EsMoL::TaskAssignment> Instances() { return Udm::InstantiatedAttr< ::EsMoL::TaskAssignment>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::EsMoL::TaskAssignment, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::EsMoL::TaskAssignment, Pred>(impl);}

			Udm::DerivedAttr< ::EsMoL::TaskAssignment> Derived() { return Udm::DerivedAttr< ::EsMoL::TaskAssignment>(impl);}
			template <class Pred> Udm::DerivedAttr< ::EsMoL::TaskAssignment, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::EsMoL::TaskAssignment, Pred>(impl);}

			Udm::ArchetypeAttr< ::EsMoL::TaskAssignment> Archetype() { return Udm::ArchetypeAttr< ::EsMoL::TaskAssignment>(impl);}

			static ::Uml::CompositionParentRole meta_Deployment_parent;
			Udm::ParentAttr< ::EsMoL::Deployment> Deployment_parent() const { return Udm::ParentAttr< ::EsMoL::Deployment>(impl, meta_Deployment_parent); }

			Udm::ParentAttr< ::EsMoL::MgaObject> parent() const { return Udm::ParentAttr< ::EsMoL::MgaObject>(impl, Udm::NULLPARENTROLE); }
			static ::Uml::AssociationRole meta_srcTaskAssignment_end_;
			Udm::AssocEndAttr< ::EsMoL::Task> srcTaskAssignment_end() const { return Udm::AssocEndAttr< ::EsMoL::Task>(impl, meta_srcTaskAssignment_end_); }

			static ::Uml::AssociationRole meta_dstTaskAssignment_end_;
			Udm::AssocEndAttr< ::EsMoL::NodeRef> dstTaskAssignment_end() const { return Udm::AssocEndAttr< ::EsMoL::NodeRef>(impl, meta_dstTaskAssignment_end_); }

		};

		class  Task :  public ::EsMoL::MgaObject {
		public:
			static ::Uml::Class meta;

			Task() { }
			Task(Udm::ObjectImpl *impl) : ::EsMoL::MgaObject(impl) { }
			Task(const Task &master) : ::EsMoL::MgaObject(master) { }
			static Task Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static Task Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			Task CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			Task CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::EsMoL::Task> Instances() { return Udm::InstantiatedAttr< ::EsMoL::Task>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::EsMoL::Task, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::EsMoL::Task, Pred>(impl);}

			Udm::DerivedAttr< ::EsMoL::Task> Derived() { return Udm::DerivedAttr< ::EsMoL::Task>(impl);}
			template <class Pred> Udm::DerivedAttr< ::EsMoL::Task, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::EsMoL::Task, Pred>(impl);}

			Udm::ArchetypeAttr< ::EsMoL::Task> Archetype() { return Udm::ArchetypeAttr< ::EsMoL::Task>(impl);}

			static ::Uml::Attribute meta_Type;
			Udm::StringAttr Type() const { return Udm::StringAttr(impl, meta_Type); }

			static ::Uml::Attribute meta_Comment;
			Udm::StringAttr Comment() const { return Udm::StringAttr(impl, meta_Comment); }

			static ::Uml::Attribute meta_Priority;
			Udm::IntegerAttr Priority() const { return Udm::IntegerAttr(impl, meta_Priority); }

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

			static ::Uml::AssociationRole meta_referedbyTaskRef;
			Udm::AssocAttr< ::EsMoL::TaskRef> referedbyTaskRef() const { return Udm::AssocAttr< ::EsMoL::TaskRef>(impl, meta_referedbyTaskRef); }
			template <class Pred> Udm::AssocAttr< ::EsMoL::TaskRef, Pred > referedbyTaskRef_sorted(const Pred &) const { return Udm::AssocAttr< ::EsMoL::TaskRef, Pred>(impl, meta_referedbyTaskRef); }

			static ::Uml::AssociationRole meta_dstTaskAssignment;
			static ::Uml::AssociationRole meta_dstTaskAssignment_rev;
			Udm::AClassAssocAttr< ::EsMoL::TaskAssignment, ::EsMoL::NodeRef> dstTaskAssignment() const { return Udm::AClassAssocAttr< ::EsMoL::TaskAssignment, ::EsMoL::NodeRef>(impl, meta_dstTaskAssignment, meta_dstTaskAssignment_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::EsMoL::TaskAssignment, ::EsMoL::NodeRef, Pred> dstTaskAssignment_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::EsMoL::TaskAssignment, ::EsMoL::NodeRef, Pred>(impl, meta_dstTaskAssignment, meta_dstTaskAssignment_rev); }

			static ::Uml::CompositionChildRole meta_Connector_children;
			Udm::ChildrenAttr< ::EsMoL::Connector> Connector_children() const { return Udm::ChildrenAttr< ::EsMoL::Connector>(impl, meta_Connector_children); }
			template <class Pred> Udm::ChildrenAttr< ::EsMoL::Connector, Pred> Connector_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::Connector, Pred>(impl, meta_Connector_children); }

			static ::Uml::CompositionChildRole meta_Component_children;
			Udm::ChildrenAttr< ::EsMoL::Component> Component_children() const { return Udm::ChildrenAttr< ::EsMoL::Component>(impl, meta_Component_children); }
			template <class Pred> Udm::ChildrenAttr< ::EsMoL::Component, Pred> Component_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::Component, Pred>(impl, meta_Component_children); }

			static ::Uml::CompositionChildRole meta_CommDst_children;
			Udm::ChildrenAttr< ::EsMoL::CommDst> CommDst_children() const { return Udm::ChildrenAttr< ::EsMoL::CommDst>(impl, meta_CommDst_children); }
			template <class Pred> Udm::ChildrenAttr< ::EsMoL::CommDst, Pred> CommDst_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::CommDst, Pred>(impl, meta_CommDst_children); }

			static ::Uml::CompositionChildRole meta_CommMapping_children;
			Udm::ChildrenAttr< ::EsMoL::CommMapping> CommMapping_children() const { return Udm::ChildrenAttr< ::EsMoL::CommMapping>(impl, meta_CommMapping_children); }
			template <class Pred> Udm::ChildrenAttr< ::EsMoL::CommMapping, Pred> CommMapping_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::CommMapping, Pred>(impl, meta_CommMapping_children); }

			static ::Uml::CompositionChildRole meta_MsgAssignment_children;
			Udm::ChildrenAttr< ::EsMoL::MsgAssignment> MsgAssignment_children() const { return Udm::ChildrenAttr< ::EsMoL::MsgAssignment>(impl, meta_MsgAssignment_children); }
			template <class Pred> Udm::ChildrenAttr< ::EsMoL::MsgAssignment, Pred> MsgAssignment_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::MsgAssignment, Pred>(impl, meta_MsgAssignment_children); }

			Udm::ChildrenAttr< ::EsMoL::Component> Component_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::Component>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::Component, Pred> Component_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::Component, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::EsMoL::CommDst> CommDst_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::CommDst>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::CommDst, Pred> CommDst_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::CommDst, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::EsMoL::MessageRef> MessageRef_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::MessageRef>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::MessageRef, Pred> MessageRef_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::MessageRef, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::EsMoL::MsgAssignment> MsgAssignment_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::MsgAssignment>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::MsgAssignment, Pred> MsgAssignment_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::MsgAssignment, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::EsMoL::CommMapping> CommMapping_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::CommMapping>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::CommMapping, Pred> CommMapping_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::CommMapping, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::EsMoL::Message> Message_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::Message>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::Message, Pred> Message_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::Message, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::EsMoL::InCommMapping> InCommMapping_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::InCommMapping>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::InCommMapping, Pred> InCommMapping_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::InCommMapping, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::EsMoL::ChannelRef> ChannelRef_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::ChannelRef>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::ChannelRef, Pred> ChannelRef_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::ChannelRef, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::EsMoL::OutCommMapping> OutCommMapping_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::OutCommMapping>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::OutCommMapping, Pred> OutCommMapping_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::OutCommMapping, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::EsMoL::Connector> Connector_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::Connector>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::Connector, Pred> Connector_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::Connector, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::EsMoL::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::MgaObject>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

			static ::Uml::CompositionParentRole meta_Deployment_parent;
			Udm::ParentAttr< ::EsMoL::Deployment> Deployment_parent() const { return Udm::ParentAttr< ::EsMoL::Deployment>(impl, meta_Deployment_parent); }

			Udm::ParentAttr< ::EsMoL::MgaObject> parent() const { return Udm::ParentAttr< ::EsMoL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  CommDst :  public ::EsMoL::MgaObject {
		public:
			static ::Uml::Class meta;

			CommDst() { }
			CommDst(Udm::ObjectImpl *impl) : ::EsMoL::MgaObject(impl) { }
			CommDst(const CommDst &master) : ::EsMoL::MgaObject(master) { }
			static CommDst Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static CommDst Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			CommDst CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			CommDst CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::EsMoL::CommDst> Instances() { return Udm::InstantiatedAttr< ::EsMoL::CommDst>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::EsMoL::CommDst, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::EsMoL::CommDst, Pred>(impl);}

			Udm::DerivedAttr< ::EsMoL::CommDst> Derived() { return Udm::DerivedAttr< ::EsMoL::CommDst>(impl);}
			template <class Pred> Udm::DerivedAttr< ::EsMoL::CommDst, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::EsMoL::CommDst, Pred>(impl);}

			Udm::ArchetypeAttr< ::EsMoL::CommDst> Archetype() { return Udm::ArchetypeAttr< ::EsMoL::CommDst>(impl);}

			static ::Uml::AssociationRole meta_srcOutCommMapping;
			static ::Uml::AssociationRole meta_srcOutCommMapping_rev;
			Udm::AClassAssocAttr< ::EsMoL::OutCommMapping, ::EsMoL::OutputPort> srcOutCommMapping() const { return Udm::AClassAssocAttr< ::EsMoL::OutCommMapping, ::EsMoL::OutputPort>(impl, meta_srcOutCommMapping, meta_srcOutCommMapping_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::EsMoL::OutCommMapping, ::EsMoL::OutputPort, Pred> srcOutCommMapping_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::EsMoL::OutCommMapping, ::EsMoL::OutputPort, Pred>(impl, meta_srcOutCommMapping, meta_srcOutCommMapping_rev); }

			static ::Uml::AssociationRole meta_dstInCommMapping;
			static ::Uml::AssociationRole meta_dstInCommMapping_rev;
			Udm::AClassAssocAttr< ::EsMoL::InCommMapping, ::EsMoL::InputPort> dstInCommMapping() const { return Udm::AClassAssocAttr< ::EsMoL::InCommMapping, ::EsMoL::InputPort>(impl, meta_dstInCommMapping, meta_dstInCommMapping_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::EsMoL::InCommMapping, ::EsMoL::InputPort, Pred> dstInCommMapping_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::EsMoL::InCommMapping, ::EsMoL::InputPort, Pred>(impl, meta_dstInCommMapping, meta_dstInCommMapping_rev); }

			static ::Uml::AssociationRole meta_dstMsgAssignment;
			static ::Uml::AssociationRole meta_dstMsgAssignment_rev;
			Udm::AClassAssocAttr< ::EsMoL::MsgAssignment, ::EsMoL::ChannelRef> dstMsgAssignment() const { return Udm::AClassAssocAttr< ::EsMoL::MsgAssignment, ::EsMoL::ChannelRef>(impl, meta_dstMsgAssignment, meta_dstMsgAssignment_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::EsMoL::MsgAssignment, ::EsMoL::ChannelRef, Pred> dstMsgAssignment_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::EsMoL::MsgAssignment, ::EsMoL::ChannelRef, Pred>(impl, meta_dstMsgAssignment, meta_dstMsgAssignment_rev); }

			static ::Uml::CompositionParentRole meta_Task_parent;
			Udm::ParentAttr< ::EsMoL::Task> Task_parent() const { return Udm::ParentAttr< ::EsMoL::Task>(impl, meta_Task_parent); }

			Udm::ParentAttr< ::EsMoL::MgaObject> parent() const { return Udm::ParentAttr< ::EsMoL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  MessageRef :  public ::EsMoL::CommDst {
		public:
			static ::Uml::Class meta;

			MessageRef() { }
			MessageRef(Udm::ObjectImpl *impl) : ::EsMoL::CommDst(impl) { }
			MessageRef(const MessageRef &master) : ::EsMoL::CommDst(master) { }
			static MessageRef Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static MessageRef Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			MessageRef CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			MessageRef CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::EsMoL::MessageRef> Instances() { return Udm::InstantiatedAttr< ::EsMoL::MessageRef>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::EsMoL::MessageRef, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::EsMoL::MessageRef, Pred>(impl);}

			Udm::DerivedAttr< ::EsMoL::MessageRef> Derived() { return Udm::DerivedAttr< ::EsMoL::MessageRef>(impl);}
			template <class Pred> Udm::DerivedAttr< ::EsMoL::MessageRef, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::EsMoL::MessageRef, Pred>(impl);}

			Udm::ArchetypeAttr< ::EsMoL::MessageRef> Archetype() { return Udm::ArchetypeAttr< ::EsMoL::MessageRef>(impl);}

			static ::Uml::AssociationRole meta_ref;
			Udm::PointerAttr< ::EsMoL::Message> ref() const { return Udm::PointerAttr< ::EsMoL::Message>(impl, meta_ref); }

			Udm::ParentAttr< ::EsMoL::MgaObject> parent() const { return Udm::ParentAttr< ::EsMoL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  NodeRef :  public ::EsMoL::MgaObject {
		public:
			static ::Uml::Class meta;

			NodeRef() { }
			NodeRef(Udm::ObjectImpl *impl) : ::EsMoL::MgaObject(impl) { }
			NodeRef(const NodeRef &master) : ::EsMoL::MgaObject(master) { }
			static NodeRef Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static NodeRef Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			NodeRef CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			NodeRef CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::EsMoL::NodeRef> Instances() { return Udm::InstantiatedAttr< ::EsMoL::NodeRef>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::EsMoL::NodeRef, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::EsMoL::NodeRef, Pred>(impl);}

			Udm::DerivedAttr< ::EsMoL::NodeRef> Derived() { return Udm::DerivedAttr< ::EsMoL::NodeRef>(impl);}
			template <class Pred> Udm::DerivedAttr< ::EsMoL::NodeRef, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::EsMoL::NodeRef, Pred>(impl);}

			Udm::ArchetypeAttr< ::EsMoL::NodeRef> Archetype() { return Udm::ArchetypeAttr< ::EsMoL::NodeRef>(impl);}

			static ::Uml::AssociationRole meta_ref;
			Udm::PointerAttr< ::EsMoL::Node> ref() const { return Udm::PointerAttr< ::EsMoL::Node>(impl, meta_ref); }

			static ::Uml::AssociationRole meta_srcTaskAssignment;
			static ::Uml::AssociationRole meta_srcTaskAssignment_rev;
			Udm::AClassAssocAttr< ::EsMoL::TaskAssignment, ::EsMoL::Task> srcTaskAssignment() const { return Udm::AClassAssocAttr< ::EsMoL::TaskAssignment, ::EsMoL::Task>(impl, meta_srcTaskAssignment, meta_srcTaskAssignment_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::EsMoL::TaskAssignment, ::EsMoL::Task, Pred> srcTaskAssignment_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::EsMoL::TaskAssignment, ::EsMoL::Task, Pred>(impl, meta_srcTaskAssignment, meta_srcTaskAssignment_rev); }

			static ::Uml::CompositionParentRole meta_Deployment_parent;
			Udm::ParentAttr< ::EsMoL::Deployment> Deployment_parent() const { return Udm::ParentAttr< ::EsMoL::Deployment>(impl, meta_Deployment_parent); }

			Udm::ParentAttr< ::EsMoL::MgaObject> parent() const { return Udm::ParentAttr< ::EsMoL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  MsgAssignment :  public ::EsMoL::MgaObject {
		public:
			static ::Uml::Class meta;

			MsgAssignment() { }
			MsgAssignment(Udm::ObjectImpl *impl) : ::EsMoL::MgaObject(impl) { }
			MsgAssignment(const MsgAssignment &master) : ::EsMoL::MgaObject(master) { }
			static MsgAssignment Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static MsgAssignment Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			MsgAssignment CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			MsgAssignment CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::EsMoL::MsgAssignment> Instances() { return Udm::InstantiatedAttr< ::EsMoL::MsgAssignment>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::EsMoL::MsgAssignment, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::EsMoL::MsgAssignment, Pred>(impl);}

			Udm::DerivedAttr< ::EsMoL::MsgAssignment> Derived() { return Udm::DerivedAttr< ::EsMoL::MsgAssignment>(impl);}
			template <class Pred> Udm::DerivedAttr< ::EsMoL::MsgAssignment, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::EsMoL::MsgAssignment, Pred>(impl);}

			Udm::ArchetypeAttr< ::EsMoL::MsgAssignment> Archetype() { return Udm::ArchetypeAttr< ::EsMoL::MsgAssignment>(impl);}

			static ::Uml::CompositionParentRole meta_Task_parent;
			Udm::ParentAttr< ::EsMoL::Task> Task_parent() const { return Udm::ParentAttr< ::EsMoL::Task>(impl, meta_Task_parent); }

			Udm::ParentAttr< ::EsMoL::MgaObject> parent() const { return Udm::ParentAttr< ::EsMoL::MgaObject>(impl, Udm::NULLPARENTROLE); }
			static ::Uml::AssociationRole meta_srcMsgAssignment_end_;
			Udm::AssocEndAttr< ::EsMoL::CommDst> srcMsgAssignment_end() const { return Udm::AssocEndAttr< ::EsMoL::CommDst>(impl, meta_srcMsgAssignment_end_); }

			static ::Uml::AssociationRole meta_dstMsgAssignment_end_;
			Udm::AssocEndAttr< ::EsMoL::ChannelRef> dstMsgAssignment_end() const { return Udm::AssocEndAttr< ::EsMoL::ChannelRef>(impl, meta_dstMsgAssignment_end_); }

		};

		class  CommMapping :  public ::EsMoL::MgaObject {
		public:
			static ::Uml::Class meta;

			CommMapping() { }
			CommMapping(Udm::ObjectImpl *impl) : ::EsMoL::MgaObject(impl) { }
			CommMapping(const CommMapping &master) : ::EsMoL::MgaObject(master) { }
			static CommMapping Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static CommMapping Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			CommMapping CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			CommMapping CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::EsMoL::CommMapping> Instances() { return Udm::InstantiatedAttr< ::EsMoL::CommMapping>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::EsMoL::CommMapping, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::EsMoL::CommMapping, Pred>(impl);}

			Udm::DerivedAttr< ::EsMoL::CommMapping> Derived() { return Udm::DerivedAttr< ::EsMoL::CommMapping>(impl);}
			template <class Pred> Udm::DerivedAttr< ::EsMoL::CommMapping, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::EsMoL::CommMapping, Pred>(impl);}

			Udm::ArchetypeAttr< ::EsMoL::CommMapping> Archetype() { return Udm::ArchetypeAttr< ::EsMoL::CommMapping>(impl);}

			static ::Uml::CompositionParentRole meta_Task_parent;
			Udm::ParentAttr< ::EsMoL::Task> Task_parent() const { return Udm::ParentAttr< ::EsMoL::Task>(impl, meta_Task_parent); }

			Udm::ParentAttr< ::EsMoL::MgaObject> parent() const { return Udm::ParentAttr< ::EsMoL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  Message :  public ::EsMoL::CommDst {
		public:
			static ::Uml::Class meta;

			Message() { }
			Message(Udm::ObjectImpl *impl) : ::EsMoL::CommDst(impl) { }
			Message(const Message &master) : ::EsMoL::CommDst(master) { }
			static Message Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static Message Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			Message CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			Message CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::EsMoL::Message> Instances() { return Udm::InstantiatedAttr< ::EsMoL::Message>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::EsMoL::Message, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::EsMoL::Message, Pred>(impl);}

			Udm::DerivedAttr< ::EsMoL::Message> Derived() { return Udm::DerivedAttr< ::EsMoL::Message>(impl);}
			template <class Pred> Udm::DerivedAttr< ::EsMoL::Message, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::EsMoL::Message, Pred>(impl);}

			Udm::ArchetypeAttr< ::EsMoL::Message> Archetype() { return Udm::ArchetypeAttr< ::EsMoL::Message>(impl);}

			static ::Uml::AssociationRole meta_referedbyBusMessageRef;
			Udm::AssocAttr< ::EsMoL::MessageRef> referedbyBusMessageRef() const { return Udm::AssocAttr< ::EsMoL::MessageRef>(impl, meta_referedbyBusMessageRef); }
			template <class Pred> Udm::AssocAttr< ::EsMoL::MessageRef, Pred > referedbyBusMessageRef_sorted(const Pred &) const { return Udm::AssocAttr< ::EsMoL::MessageRef, Pred>(impl, meta_referedbyBusMessageRef); }

			Udm::ParentAttr< ::EsMoL::MgaObject> parent() const { return Udm::ParentAttr< ::EsMoL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  InCommMapping :  public ::EsMoL::CommMapping {
		public:
			static ::Uml::Class meta;

			InCommMapping() { }
			InCommMapping(Udm::ObjectImpl *impl) : ::EsMoL::CommMapping(impl) { }
			InCommMapping(const InCommMapping &master) : ::EsMoL::CommMapping(master) { }
			static InCommMapping Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static InCommMapping Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			InCommMapping CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			InCommMapping CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::EsMoL::InCommMapping> Instances() { return Udm::InstantiatedAttr< ::EsMoL::InCommMapping>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::EsMoL::InCommMapping, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::EsMoL::InCommMapping, Pred>(impl);}

			Udm::DerivedAttr< ::EsMoL::InCommMapping> Derived() { return Udm::DerivedAttr< ::EsMoL::InCommMapping>(impl);}
			template <class Pred> Udm::DerivedAttr< ::EsMoL::InCommMapping, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::EsMoL::InCommMapping, Pred>(impl);}

			Udm::ArchetypeAttr< ::EsMoL::InCommMapping> Archetype() { return Udm::ArchetypeAttr< ::EsMoL::InCommMapping>(impl);}

			Udm::ParentAttr< ::EsMoL::MgaObject> parent() const { return Udm::ParentAttr< ::EsMoL::MgaObject>(impl, Udm::NULLPARENTROLE); }
			static ::Uml::AssociationRole meta_dstInCommMapping_end_;
			Udm::AssocEndAttr< ::EsMoL::InputPort> dstInCommMapping_end() const { return Udm::AssocEndAttr< ::EsMoL::InputPort>(impl, meta_dstInCommMapping_end_); }

			static ::Uml::AssociationRole meta_srcInCommMapping_end_;
			Udm::AssocEndAttr< ::EsMoL::CommDst> srcInCommMapping_end() const { return Udm::AssocEndAttr< ::EsMoL::CommDst>(impl, meta_srcInCommMapping_end_); }

		};

		class  IOPort :  virtual public ::EsMoL::MgaObject {
		public:
			static ::Uml::Class meta;

			IOPort() { }
			IOPort(Udm::ObjectImpl *impl) : ::EsMoL::MgaObject(impl) { }
			IOPort(const IOPort &master) : ::EsMoL::MgaObject(master) { }
			static IOPort Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static IOPort Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			IOPort CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			IOPort CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::EsMoL::IOPort> Instances() { return Udm::InstantiatedAttr< ::EsMoL::IOPort>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::EsMoL::IOPort, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::EsMoL::IOPort, Pred>(impl);}

			Udm::DerivedAttr< ::EsMoL::IOPort> Derived() { return Udm::DerivedAttr< ::EsMoL::IOPort>(impl);}
			template <class Pred> Udm::DerivedAttr< ::EsMoL::IOPort, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::EsMoL::IOPort, Pred>(impl);}

			Udm::ArchetypeAttr< ::EsMoL::IOPort> Archetype() { return Udm::ArchetypeAttr< ::EsMoL::IOPort>(impl);}

			Udm::ParentAttr<Udm::Object> parent() const { return Udm::ParentAttr<Udm::Object>(impl, Udm::NULLPARENTROLE); }
		};

		class  ChannelRef :  public ::EsMoL::CommDst {
		public:
			static ::Uml::Class meta;

			ChannelRef() { }
			ChannelRef(Udm::ObjectImpl *impl) : ::EsMoL::CommDst(impl) { }
			ChannelRef(const ChannelRef &master) : ::EsMoL::CommDst(master) { }
			static ChannelRef Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static ChannelRef Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			ChannelRef CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			ChannelRef CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::EsMoL::ChannelRef> Instances() { return Udm::InstantiatedAttr< ::EsMoL::ChannelRef>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::EsMoL::ChannelRef, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::EsMoL::ChannelRef, Pred>(impl);}

			Udm::DerivedAttr< ::EsMoL::ChannelRef> Derived() { return Udm::DerivedAttr< ::EsMoL::ChannelRef>(impl);}
			template <class Pred> Udm::DerivedAttr< ::EsMoL::ChannelRef, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::EsMoL::ChannelRef, Pred>(impl);}

			Udm::ArchetypeAttr< ::EsMoL::ChannelRef> Archetype() { return Udm::ArchetypeAttr< ::EsMoL::ChannelRef>(impl);}

			static ::Uml::AssociationRole meta_ref;
			Udm::PointerAttr< ::EsMoL::Channel> ref() const { return Udm::PointerAttr< ::EsMoL::Channel>(impl, meta_ref); }

			static ::Uml::AssociationRole meta_srcMsgAssignment;
			static ::Uml::AssociationRole meta_srcMsgAssignment_rev;
			Udm::AClassAssocAttr< ::EsMoL::MsgAssignment, ::EsMoL::CommDst> srcMsgAssignment() const { return Udm::AClassAssocAttr< ::EsMoL::MsgAssignment, ::EsMoL::CommDst>(impl, meta_srcMsgAssignment, meta_srcMsgAssignment_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::EsMoL::MsgAssignment, ::EsMoL::CommDst, Pred> srcMsgAssignment_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::EsMoL::MsgAssignment, ::EsMoL::CommDst, Pred>(impl, meta_srcMsgAssignment, meta_srcMsgAssignment_rev); }

			Udm::ParentAttr< ::EsMoL::MgaObject> parent() const { return Udm::ParentAttr< ::EsMoL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  OutCommMapping :  public ::EsMoL::CommMapping {
		public:
			static ::Uml::Class meta;

			OutCommMapping() { }
			OutCommMapping(Udm::ObjectImpl *impl) : ::EsMoL::CommMapping(impl) { }
			OutCommMapping(const OutCommMapping &master) : ::EsMoL::CommMapping(master) { }
			static OutCommMapping Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static OutCommMapping Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			OutCommMapping CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			OutCommMapping CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::EsMoL::OutCommMapping> Instances() { return Udm::InstantiatedAttr< ::EsMoL::OutCommMapping>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::EsMoL::OutCommMapping, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::EsMoL::OutCommMapping, Pred>(impl);}

			Udm::DerivedAttr< ::EsMoL::OutCommMapping> Derived() { return Udm::DerivedAttr< ::EsMoL::OutCommMapping>(impl);}
			template <class Pred> Udm::DerivedAttr< ::EsMoL::OutCommMapping, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::EsMoL::OutCommMapping, Pred>(impl);}

			Udm::ArchetypeAttr< ::EsMoL::OutCommMapping> Archetype() { return Udm::ArchetypeAttr< ::EsMoL::OutCommMapping>(impl);}

			Udm::ParentAttr< ::EsMoL::MgaObject> parent() const { return Udm::ParentAttr< ::EsMoL::MgaObject>(impl, Udm::NULLPARENTROLE); }
			static ::Uml::AssociationRole meta_dstOutCommMapping_end_;
			Udm::AssocEndAttr< ::EsMoL::CommDst> dstOutCommMapping_end() const { return Udm::AssocEndAttr< ::EsMoL::CommDst>(impl, meta_dstOutCommMapping_end_); }

			static ::Uml::AssociationRole meta_srcOutCommMapping_end_;
			Udm::AssocEndAttr< ::EsMoL::OutputPort> srcOutCommMapping_end() const { return Udm::AssocEndAttr< ::EsMoL::OutputPort>(impl, meta_srcOutCommMapping_end_); }

		};

		class  TypeBase :  public ::EsMoL::MgaObject {
		public:
			static ::Uml::Class meta;

			TypeBase() { }
			TypeBase(Udm::ObjectImpl *impl) : ::EsMoL::MgaObject(impl) { }
			TypeBase(const TypeBase &master) : ::EsMoL::MgaObject(master) { }
			static TypeBase Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static TypeBase Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			TypeBase CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			TypeBase CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::EsMoL::TypeBase> Instances() { return Udm::InstantiatedAttr< ::EsMoL::TypeBase>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::EsMoL::TypeBase, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::EsMoL::TypeBase, Pred>(impl);}

			Udm::DerivedAttr< ::EsMoL::TypeBase> Derived() { return Udm::DerivedAttr< ::EsMoL::TypeBase>(impl);}
			template <class Pred> Udm::DerivedAttr< ::EsMoL::TypeBase, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::EsMoL::TypeBase, Pred>(impl);}

			Udm::ArchetypeAttr< ::EsMoL::TypeBase> Archetype() { return Udm::ArchetypeAttr< ::EsMoL::TypeBase>(impl);}

			static ::Uml::AssociationRole meta_referedbyTypeStructRef;
			Udm::AssocAttr< ::EsMoL::TypeBaseRef> referedbyTypeStructRef() const { return Udm::AssocAttr< ::EsMoL::TypeBaseRef>(impl, meta_referedbyTypeStructRef); }
			template <class Pred> Udm::AssocAttr< ::EsMoL::TypeBaseRef, Pred > referedbyTypeStructRef_sorted(const Pred &) const { return Udm::AssocAttr< ::EsMoL::TypeBaseRef, Pred>(impl, meta_referedbyTypeStructRef); }

			static ::Uml::CompositionParentRole meta_Types_parent;
			Udm::ParentAttr< ::EsMoL::Types> Types_parent() const { return Udm::ParentAttr< ::EsMoL::Types>(impl, meta_Types_parent); }

			Udm::ParentAttr< ::EsMoL::Types> parent() const { return Udm::ParentAttr< ::EsMoL::Types>(impl, Udm::NULLPARENTROLE); }
		};

		class  Matrix :  public ::EsMoL::TypeBase {
		public:
			static ::Uml::Class meta;

			Matrix() { }
			Matrix(Udm::ObjectImpl *impl) : ::EsMoL::TypeBase(impl) { }
			Matrix(const Matrix &master) : ::EsMoL::TypeBase(master) { }
			static Matrix Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static Matrix Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			Matrix CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			Matrix CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::EsMoL::Matrix> Instances() { return Udm::InstantiatedAttr< ::EsMoL::Matrix>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::EsMoL::Matrix, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::EsMoL::Matrix, Pred>(impl);}

			Udm::DerivedAttr< ::EsMoL::Matrix> Derived() { return Udm::DerivedAttr< ::EsMoL::Matrix>(impl);}
			template <class Pred> Udm::DerivedAttr< ::EsMoL::Matrix, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::EsMoL::Matrix, Pred>(impl);}

			Udm::ArchetypeAttr< ::EsMoL::Matrix> Archetype() { return Udm::ArchetypeAttr< ::EsMoL::Matrix>(impl);}

			static ::Uml::Attribute meta_rows;
			Udm::IntegerAttr rows() const { return Udm::IntegerAttr(impl, meta_rows); }

			static ::Uml::Attribute meta_Type;
			Udm::StringAttr Type() const { return Udm::StringAttr(impl, meta_Type); }

			static ::Uml::Attribute meta_columns;
			Udm::IntegerAttr columns() const { return Udm::IntegerAttr(impl, meta_columns); }

			Udm::ParentAttr< ::EsMoL::Types> parent() const { return Udm::ParentAttr< ::EsMoL::Types>(impl, Udm::NULLPARENTROLE); }
		};

		class  TypeStruct :  public ::EsMoL::TypeBase {
		public:
			static ::Uml::Class meta;

			TypeStruct() { }
			TypeStruct(Udm::ObjectImpl *impl) : ::EsMoL::TypeBase(impl) { }
			TypeStruct(const TypeStruct &master) : ::EsMoL::TypeBase(master) { }
			static TypeStruct Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static TypeStruct Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			TypeStruct CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			TypeStruct CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::EsMoL::TypeStruct> Instances() { return Udm::InstantiatedAttr< ::EsMoL::TypeStruct>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::EsMoL::TypeStruct, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::EsMoL::TypeStruct, Pred>(impl);}

			Udm::DerivedAttr< ::EsMoL::TypeStruct> Derived() { return Udm::DerivedAttr< ::EsMoL::TypeStruct>(impl);}
			template <class Pred> Udm::DerivedAttr< ::EsMoL::TypeStruct, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::EsMoL::TypeStruct, Pred>(impl);}

			Udm::ArchetypeAttr< ::EsMoL::TypeStruct> Archetype() { return Udm::ArchetypeAttr< ::EsMoL::TypeStruct>(impl);}

			static ::Uml::Attribute meta_MemberCount;
			Udm::IntegerAttr MemberCount() const { return Udm::IntegerAttr(impl, meta_MemberCount); }

			static ::Uml::CompositionChildRole meta_TypeBaseRef_children;
			Udm::ChildrenAttr< ::EsMoL::TypeBaseRef> TypeBaseRef_children() const { return Udm::ChildrenAttr< ::EsMoL::TypeBaseRef>(impl, meta_TypeBaseRef_children); }
			template <class Pred> Udm::ChildrenAttr< ::EsMoL::TypeBaseRef, Pred> TypeBaseRef_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::TypeBaseRef, Pred>(impl, meta_TypeBaseRef_children); }

			Udm::ChildrenAttr< ::EsMoL::TypeBaseRef> TypeBaseRef_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::TypeBaseRef>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::TypeBaseRef, Pred> TypeBaseRef_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::TypeBaseRef, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::EsMoL::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::MgaObject>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ParentAttr< ::EsMoL::Types> parent() const { return Udm::ParentAttr< ::EsMoL::Types>(impl, Udm::NULLPARENTROLE); }
		};

		class  TypeBaseRef :  public ::EsMoL::MgaObject {
		public:
			static ::Uml::Class meta;

			TypeBaseRef() { }
			TypeBaseRef(Udm::ObjectImpl *impl) : ::EsMoL::MgaObject(impl) { }
			TypeBaseRef(const TypeBaseRef &master) : ::EsMoL::MgaObject(master) { }
			static TypeBaseRef Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static TypeBaseRef Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			TypeBaseRef CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			TypeBaseRef CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::EsMoL::TypeBaseRef> Instances() { return Udm::InstantiatedAttr< ::EsMoL::TypeBaseRef>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::EsMoL::TypeBaseRef, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::EsMoL::TypeBaseRef, Pred>(impl);}

			Udm::DerivedAttr< ::EsMoL::TypeBaseRef> Derived() { return Udm::DerivedAttr< ::EsMoL::TypeBaseRef>(impl);}
			template <class Pred> Udm::DerivedAttr< ::EsMoL::TypeBaseRef, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::EsMoL::TypeBaseRef, Pred>(impl);}

			Udm::ArchetypeAttr< ::EsMoL::TypeBaseRef> Archetype() { return Udm::ArchetypeAttr< ::EsMoL::TypeBaseRef>(impl);}

			static ::Uml::Attribute meta_MemberIndex;
			Udm::IntegerAttr MemberIndex() const { return Udm::IntegerAttr(impl, meta_MemberIndex); }

			static ::Uml::AssociationRole meta_ref;
			Udm::PointerAttr< ::EsMoL::TypeBase> ref() const { return Udm::PointerAttr< ::EsMoL::TypeBase>(impl, meta_ref); }

			static ::Uml::CompositionParentRole meta_Port_parent;
			Udm::ParentAttr< ::EsMoL::Port> Port_parent() const { return Udm::ParentAttr< ::EsMoL::Port>(impl, meta_Port_parent); }

			static ::Uml::CompositionParentRole meta_TypeStruct_parent;
			Udm::ParentAttr< ::EsMoL::TypeStruct> TypeStruct_parent() const { return Udm::ParentAttr< ::EsMoL::TypeStruct>(impl, meta_TypeStruct_parent); }

			static ::Uml::CompositionParentRole meta_StateDE_parent;
			Udm::ParentAttr< ::EsMoL::StateDE> StateDE_parent() const { return Udm::ParentAttr< ::EsMoL::StateDE>(impl, meta_StateDE_parent); }

			Udm::ParentAttr< ::EsMoL::MgaObject> parent() const { return Udm::ParentAttr< ::EsMoL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  IOPortExp :  virtual public ::EsMoL::MgaObject {
		public:
			static ::Uml::Class meta;

			IOPortExp() { }
			IOPortExp(Udm::ObjectImpl *impl) : ::EsMoL::MgaObject(impl) { }
			IOPortExp(const IOPortExp &master) : ::EsMoL::MgaObject(master) { }
			static IOPortExp Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static IOPortExp Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			IOPortExp CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			IOPortExp CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::EsMoL::IOPortExp> Instances() { return Udm::InstantiatedAttr< ::EsMoL::IOPortExp>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::EsMoL::IOPortExp, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::EsMoL::IOPortExp, Pred>(impl);}

			Udm::DerivedAttr< ::EsMoL::IOPortExp> Derived() { return Udm::DerivedAttr< ::EsMoL::IOPortExp>(impl);}
			template <class Pred> Udm::DerivedAttr< ::EsMoL::IOPortExp, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::EsMoL::IOPortExp, Pred>(impl);}

			Udm::ArchetypeAttr< ::EsMoL::IOPortExp> Archetype() { return Udm::ArchetypeAttr< ::EsMoL::IOPortExp>(impl);}

			static ::Uml::AssociationRole meta_srcIOPortAssignment;
			static ::Uml::AssociationRole meta_srcIOPortAssignment_rev;
			Udm::AClassAssocAttr< ::EsMoL::IOPortAssignment, ::EsMoL::IOPortInfoBase> srcIOPortAssignment() const { return Udm::AClassAssocAttr< ::EsMoL::IOPortAssignment, ::EsMoL::IOPortInfoBase>(impl, meta_srcIOPortAssignment, meta_srcIOPortAssignment_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::EsMoL::IOPortAssignment, ::EsMoL::IOPortInfoBase, Pred> srcIOPortAssignment_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::EsMoL::IOPortAssignment, ::EsMoL::IOPortInfoBase, Pred>(impl, meta_srcIOPortAssignment, meta_srcIOPortAssignment_rev); }

			Udm::ParentAttr<Udm::Object> parent() const { return Udm::ParentAttr<Udm::Object>(impl, Udm::NULLPARENTROLE); }
		};

		class  OutputPort :  public ::EsMoL::OutPort, public ::EsMoL::IOPort, public ::EsMoL::IOPortExp {
		public:
			static ::Uml::Class meta;

			OutputPort() { }
			OutputPort(Udm::ObjectImpl *impl) : ::EsMoL::OutPort(impl),::EsMoL::IOPort(impl),::EsMoL::IOPortExp(impl), ::EsMoL::MgaObject(impl) { }
			OutputPort(const OutputPort &master) : ::EsMoL::OutPort(master),::EsMoL::IOPort(master),::EsMoL::IOPortExp(master), ::EsMoL::MgaObject(master) { }
			static OutputPort Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static OutputPort Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			OutputPort CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			OutputPort CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::EsMoL::OutputPort> Instances() { return Udm::InstantiatedAttr< ::EsMoL::OutputPort>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::EsMoL::OutputPort, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::EsMoL::OutputPort, Pred>(impl);}

			Udm::DerivedAttr< ::EsMoL::OutputPort> Derived() { return Udm::DerivedAttr< ::EsMoL::OutputPort>(impl);}
			template <class Pred> Udm::DerivedAttr< ::EsMoL::OutputPort, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::EsMoL::OutputPort, Pred>(impl);}

			Udm::ArchetypeAttr< ::EsMoL::OutputPort> Archetype() { return Udm::ArchetypeAttr< ::EsMoL::OutputPort>(impl);}

			static ::Uml::Attribute meta_Number;
			Udm::IntegerAttr Number() const { return Udm::IntegerAttr(impl, meta_Number); }

			static ::Uml::AssociationRole meta_dstOutCommMapping;
			static ::Uml::AssociationRole meta_dstOutCommMapping_rev;
			Udm::AClassAssocAttr< ::EsMoL::OutCommMapping, ::EsMoL::CommDst> dstOutCommMapping() const { return Udm::AClassAssocAttr< ::EsMoL::OutCommMapping, ::EsMoL::CommDst>(impl, meta_dstOutCommMapping, meta_dstOutCommMapping_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::EsMoL::OutCommMapping, ::EsMoL::CommDst, Pred> dstOutCommMapping_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::EsMoL::OutCommMapping, ::EsMoL::CommDst, Pred>(impl, meta_dstOutCommMapping, meta_dstOutCommMapping_rev); }

			Udm::ParentAttr< ::EsMoL::MgaObject> parent() const { return Udm::ParentAttr< ::EsMoL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  InputPort :  public ::EsMoL::InPort, public ::EsMoL::IOPort, public ::EsMoL::IOPortExp {
		public:
			static ::Uml::Class meta;

			InputPort() { }
			InputPort(Udm::ObjectImpl *impl) : ::EsMoL::InPort(impl),::EsMoL::IOPort(impl),::EsMoL::IOPortExp(impl), ::EsMoL::MgaObject(impl) { }
			InputPort(const InputPort &master) : ::EsMoL::InPort(master),::EsMoL::IOPort(master),::EsMoL::IOPortExp(master), ::EsMoL::MgaObject(master) { }
			static InputPort Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static InputPort Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			InputPort CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			InputPort CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::EsMoL::InputPort> Instances() { return Udm::InstantiatedAttr< ::EsMoL::InputPort>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::EsMoL::InputPort, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::EsMoL::InputPort, Pred>(impl);}

			Udm::DerivedAttr< ::EsMoL::InputPort> Derived() { return Udm::DerivedAttr< ::EsMoL::InputPort>(impl);}
			template <class Pred> Udm::DerivedAttr< ::EsMoL::InputPort, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::EsMoL::InputPort, Pred>(impl);}

			Udm::ArchetypeAttr< ::EsMoL::InputPort> Archetype() { return Udm::ArchetypeAttr< ::EsMoL::InputPort>(impl);}

			static ::Uml::Attribute meta_Number;
			Udm::IntegerAttr Number() const { return Udm::IntegerAttr(impl, meta_Number); }

			static ::Uml::AssociationRole meta_srcInCommMapping;
			static ::Uml::AssociationRole meta_srcInCommMapping_rev;
			Udm::AClassAssocAttr< ::EsMoL::InCommMapping, ::EsMoL::CommDst> srcInCommMapping() const { return Udm::AClassAssocAttr< ::EsMoL::InCommMapping, ::EsMoL::CommDst>(impl, meta_srcInCommMapping, meta_srcInCommMapping_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::EsMoL::InCommMapping, ::EsMoL::CommDst, Pred> srcInCommMapping_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::EsMoL::InCommMapping, ::EsMoL::CommDst, Pred>(impl, meta_srcInCommMapping, meta_srcInCommMapping_rev); }

			Udm::ParentAttr< ::EsMoL::MgaObject> parent() const { return Udm::ParentAttr< ::EsMoL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  IOPortAssignment :  public ::EsMoL::MgaObject {
		public:
			static ::Uml::Class meta;

			IOPortAssignment() { }
			IOPortAssignment(Udm::ObjectImpl *impl) : ::EsMoL::MgaObject(impl) { }
			IOPortAssignment(const IOPortAssignment &master) : ::EsMoL::MgaObject(master) { }
			static IOPortAssignment Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static IOPortAssignment Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			IOPortAssignment CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			IOPortAssignment CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::EsMoL::IOPortAssignment> Instances() { return Udm::InstantiatedAttr< ::EsMoL::IOPortAssignment>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::EsMoL::IOPortAssignment, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::EsMoL::IOPortAssignment, Pred>(impl);}

			Udm::DerivedAttr< ::EsMoL::IOPortAssignment> Derived() { return Udm::DerivedAttr< ::EsMoL::IOPortAssignment>(impl);}
			template <class Pred> Udm::DerivedAttr< ::EsMoL::IOPortAssignment, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::EsMoL::IOPortAssignment, Pred>(impl);}

			Udm::ArchetypeAttr< ::EsMoL::IOPortAssignment> Archetype() { return Udm::ArchetypeAttr< ::EsMoL::IOPortAssignment>(impl);}

			Udm::ParentAttr<Udm::Object> parent() const { return Udm::ParentAttr<Udm::Object>(impl, Udm::NULLPARENTROLE); }
			static ::Uml::AssociationRole meta_dstIOPortAssignment_end_;
			Udm::AssocEndAttr< ::EsMoL::IOPortExp> dstIOPortAssignment_end() const { return Udm::AssocEndAttr< ::EsMoL::IOPortExp>(impl, meta_dstIOPortAssignment_end_); }

			static ::Uml::AssociationRole meta_srcIOPortAssignment_end_;
			Udm::AssocEndAttr< ::EsMoL::IOPortInfoBase> srcIOPortAssignment_end() const { return Udm::AssocEndAttr< ::EsMoL::IOPortInfoBase>(impl, meta_srcIOPortAssignment_end_); }

		};

		class  IOPortInfoBase :  public ::EsMoL::MgaObject {
		public:
			static ::Uml::Class meta;

			IOPortInfoBase() { }
			IOPortInfoBase(Udm::ObjectImpl *impl) : ::EsMoL::MgaObject(impl) { }
			IOPortInfoBase(const IOPortInfoBase &master) : ::EsMoL::MgaObject(master) { }
			static IOPortInfoBase Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static IOPortInfoBase Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			IOPortInfoBase CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			IOPortInfoBase CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::EsMoL::IOPortInfoBase> Instances() { return Udm::InstantiatedAttr< ::EsMoL::IOPortInfoBase>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::EsMoL::IOPortInfoBase, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::EsMoL::IOPortInfoBase, Pred>(impl);}

			Udm::DerivedAttr< ::EsMoL::IOPortInfoBase> Derived() { return Udm::DerivedAttr< ::EsMoL::IOPortInfoBase>(impl);}
			template <class Pred> Udm::DerivedAttr< ::EsMoL::IOPortInfoBase, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::EsMoL::IOPortInfoBase, Pred>(impl);}

			Udm::ArchetypeAttr< ::EsMoL::IOPortInfoBase> Archetype() { return Udm::ArchetypeAttr< ::EsMoL::IOPortInfoBase>(impl);}

			static ::Uml::AssociationRole meta_dstIOPortAssignment;
			static ::Uml::AssociationRole meta_dstIOPortAssignment_rev;
			Udm::AClassAssocAttr< ::EsMoL::IOPortAssignment, ::EsMoL::IOPortExp> dstIOPortAssignment() const { return Udm::AClassAssocAttr< ::EsMoL::IOPortAssignment, ::EsMoL::IOPortExp>(impl, meta_dstIOPortAssignment, meta_dstIOPortAssignment_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::EsMoL::IOPortAssignment, ::EsMoL::IOPortExp, Pred> dstIOPortAssignment_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::EsMoL::IOPortAssignment, ::EsMoL::IOPortExp, Pred>(impl, meta_dstIOPortAssignment, meta_dstIOPortAssignment_rev); }

			Udm::ParentAttr<Udm::Object> parent() const { return Udm::ParentAttr<Udm::Object>(impl, Udm::NULLPARENTROLE); }
		};

		class  IOPortInfo :  public ::EsMoL::IOPortInfoBase {
		public:
			static ::Uml::Class meta;

			IOPortInfo() { }
			IOPortInfo(Udm::ObjectImpl *impl) : ::EsMoL::IOPortInfoBase(impl) { }
			IOPortInfo(const IOPortInfo &master) : ::EsMoL::IOPortInfoBase(master) { }
			static IOPortInfo Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static IOPortInfo Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			IOPortInfo CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			IOPortInfo CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::EsMoL::IOPortInfo> Instances() { return Udm::InstantiatedAttr< ::EsMoL::IOPortInfo>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::EsMoL::IOPortInfo, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::EsMoL::IOPortInfo, Pred>(impl);}

			Udm::DerivedAttr< ::EsMoL::IOPortInfo> Derived() { return Udm::DerivedAttr< ::EsMoL::IOPortInfo>(impl);}
			template <class Pred> Udm::DerivedAttr< ::EsMoL::IOPortInfo, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::EsMoL::IOPortInfo, Pred>(impl);}

			Udm::ArchetypeAttr< ::EsMoL::IOPortInfo> Archetype() { return Udm::ArchetypeAttr< ::EsMoL::IOPortInfo>(impl);}

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
			Udm::AssocAttr< ::EsMoL::IOPortInfoRef> referedbyIOPortInfoRef() const { return Udm::AssocAttr< ::EsMoL::IOPortInfoRef>(impl, meta_referedbyIOPortInfoRef); }
			template <class Pred> Udm::AssocAttr< ::EsMoL::IOPortInfoRef, Pred > referedbyIOPortInfoRef_sorted(const Pred &) const { return Udm::AssocAttr< ::EsMoL::IOPortInfoRef, Pred>(impl, meta_referedbyIOPortInfoRef); }

			Udm::ParentAttr<Udm::Object> parent() const { return Udm::ParentAttr<Udm::Object>(impl, Udm::NULLPARENTROLE); }
		};

		class  IOPortInfoRef :  public ::EsMoL::IOPortInfoBase {
		public:
			static ::Uml::Class meta;

			IOPortInfoRef() { }
			IOPortInfoRef(Udm::ObjectImpl *impl) : ::EsMoL::IOPortInfoBase(impl) { }
			IOPortInfoRef(const IOPortInfoRef &master) : ::EsMoL::IOPortInfoBase(master) { }
			static IOPortInfoRef Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static IOPortInfoRef Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			IOPortInfoRef CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			IOPortInfoRef CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::EsMoL::IOPortInfoRef> Instances() { return Udm::InstantiatedAttr< ::EsMoL::IOPortInfoRef>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::EsMoL::IOPortInfoRef, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::EsMoL::IOPortInfoRef, Pred>(impl);}

			Udm::DerivedAttr< ::EsMoL::IOPortInfoRef> Derived() { return Udm::DerivedAttr< ::EsMoL::IOPortInfoRef>(impl);}
			template <class Pred> Udm::DerivedAttr< ::EsMoL::IOPortInfoRef, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::EsMoL::IOPortInfoRef, Pred>(impl);}

			Udm::ArchetypeAttr< ::EsMoL::IOPortInfoRef> Archetype() { return Udm::ArchetypeAttr< ::EsMoL::IOPortInfoRef>(impl);}

			static ::Uml::AssociationRole meta_ref;
			Udm::PointerAttr< ::EsMoL::IOPortInfo> ref() const { return Udm::PointerAttr< ::EsMoL::IOPortInfo>(impl, meta_ref); }

			Udm::ParentAttr<Udm::Object> parent() const { return Udm::ParentAttr<Udm::Object>(impl, Udm::NULLPARENTROLE); }
		};

		class  System :  public ::EsMoL::MgaObject {
		public:
			static ::Uml::Class meta;

			System() { }
			System(Udm::ObjectImpl *impl) : ::EsMoL::MgaObject(impl) { }
			System(const System &master) : ::EsMoL::MgaObject(master) { }
			static System Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static System Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			System CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			System CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::EsMoL::System> Instances() { return Udm::InstantiatedAttr< ::EsMoL::System>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::EsMoL::System, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::EsMoL::System, Pred>(impl);}

			Udm::DerivedAttr< ::EsMoL::System> Derived() { return Udm::DerivedAttr< ::EsMoL::System>(impl);}
			template <class Pred> Udm::DerivedAttr< ::EsMoL::System, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::EsMoL::System, Pred>(impl);}

			Udm::ArchetypeAttr< ::EsMoL::System> Archetype() { return Udm::ArchetypeAttr< ::EsMoL::System>(impl);}

			static ::Uml::CompositionChildRole meta_Connector_children;
			Udm::ChildrenAttr< ::EsMoL::Connector> Connector_children() const { return Udm::ChildrenAttr< ::EsMoL::Connector>(impl, meta_Connector_children); }
			template <class Pred> Udm::ChildrenAttr< ::EsMoL::Connector, Pred> Connector_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::Connector, Pred>(impl, meta_Connector_children); }

			static ::Uml::CompositionChildRole meta_ComponentBase_children;
			Udm::ChildrenAttr< ::EsMoL::ComponentBase> ComponentBase_children() const { return Udm::ChildrenAttr< ::EsMoL::ComponentBase>(impl, meta_ComponentBase_children); }
			template <class Pred> Udm::ChildrenAttr< ::EsMoL::ComponentBase, Pred> ComponentBase_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::ComponentBase, Pred>(impl, meta_ComponentBase_children); }

			Udm::ChildrenAttr< ::EsMoL::Subsystem> Subsystem_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::Subsystem>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::Subsystem, Pred> Subsystem_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::Subsystem, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::EsMoL::SubsystemRef> SubsystemRef_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::SubsystemRef>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::SubsystemRef, Pred> SubsystemRef_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::SubsystemRef, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::EsMoL::CCode> CCode_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::CCode>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::CCode, Pred> CCode_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::CCode, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::EsMoL::Connector> Connector_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::Connector>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::Connector, Pred> Connector_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::Connector, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::EsMoL::ComponentBase> ComponentBase_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::ComponentBase>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::ComponentBase, Pred> ComponentBase_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::ComponentBase, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::EsMoL::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::MgaObject>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

			static ::Uml::CompositionParentRole meta_DesignFolder_parent;
			Udm::ParentAttr< ::EsMoL::DesignFolder> DesignFolder_parent() const { return Udm::ParentAttr< ::EsMoL::DesignFolder>(impl, meta_DesignFolder_parent); }

			static ::Uml::CompositionParentRole meta_ArchitectureLibrary_parent;
			Udm::ParentAttr< ::EsMoL::ArchitectureLibrary> ArchitectureLibrary_parent() const { return Udm::ParentAttr< ::EsMoL::ArchitectureLibrary>(impl, meta_ArchitectureLibrary_parent); }

			Udm::ParentAttr<Udm::Object> parent() const { return Udm::ParentAttr<Udm::Object>(impl, Udm::NULLPARENTROLE); }
		};

		class  Connector :  public ::EsMoL::MgaObject {
		public:
			static ::Uml::Class meta;

			Connector() { }
			Connector(Udm::ObjectImpl *impl) : ::EsMoL::MgaObject(impl) { }
			Connector(const Connector &master) : ::EsMoL::MgaObject(master) { }
			static Connector Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static Connector Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			Connector CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			Connector CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::EsMoL::Connector> Instances() { return Udm::InstantiatedAttr< ::EsMoL::Connector>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::EsMoL::Connector, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::EsMoL::Connector, Pred>(impl);}

			Udm::DerivedAttr< ::EsMoL::Connector> Derived() { return Udm::DerivedAttr< ::EsMoL::Connector>(impl);}
			template <class Pred> Udm::DerivedAttr< ::EsMoL::Connector, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::EsMoL::Connector, Pred>(impl);}

			Udm::ArchetypeAttr< ::EsMoL::Connector> Archetype() { return Udm::ArchetypeAttr< ::EsMoL::Connector>(impl);}

			static ::Uml::Attribute meta_ConnectionType;
			Udm::StringAttr ConnectionType() const { return Udm::StringAttr(impl, meta_ConnectionType); }

			static ::Uml::CompositionParentRole meta_Task_parent;
			Udm::ParentAttr< ::EsMoL::Task> Task_parent() const { return Udm::ParentAttr< ::EsMoL::Task>(impl, meta_Task_parent); }

			static ::Uml::CompositionParentRole meta_System_parent;
			Udm::ParentAttr< ::EsMoL::System> System_parent() const { return Udm::ParentAttr< ::EsMoL::System>(impl, meta_System_parent); }

			Udm::ParentAttr< ::EsMoL::MgaObject> parent() const { return Udm::ParentAttr< ::EsMoL::MgaObject>(impl, Udm::NULLPARENTROLE); }
			static ::Uml::AssociationRole meta_srcConnector_end_;
			Udm::AssocEndAttr< ::EsMoL::Port> srcConnector_end() const { return Udm::AssocEndAttr< ::EsMoL::Port>(impl, meta_srcConnector_end_); }

			static ::Uml::AssociationRole meta_dstConnector_end_;
			Udm::AssocEndAttr< ::EsMoL::Port> dstConnector_end() const { return Udm::AssocEndAttr< ::EsMoL::Port>(impl, meta_dstConnector_end_); }

		};

		class  ComponentBase :  virtual public ::EsMoL::MgaObject {
		public:
			static ::Uml::Class meta;

			ComponentBase() { }
			ComponentBase(Udm::ObjectImpl *impl) : ::EsMoL::MgaObject(impl) { }
			ComponentBase(const ComponentBase &master) : ::EsMoL::MgaObject(master) { }
			static ComponentBase Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static ComponentBase Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			ComponentBase CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			ComponentBase CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::EsMoL::ComponentBase> Instances() { return Udm::InstantiatedAttr< ::EsMoL::ComponentBase>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::EsMoL::ComponentBase, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::EsMoL::ComponentBase, Pred>(impl);}

			Udm::DerivedAttr< ::EsMoL::ComponentBase> Derived() { return Udm::DerivedAttr< ::EsMoL::ComponentBase>(impl);}
			template <class Pred> Udm::DerivedAttr< ::EsMoL::ComponentBase, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::EsMoL::ComponentBase, Pred>(impl);}

			Udm::ArchetypeAttr< ::EsMoL::ComponentBase> Archetype() { return Udm::ArchetypeAttr< ::EsMoL::ComponentBase>(impl);}

			static ::Uml::Attribute meta_Deadline;
			Udm::IntegerAttr Deadline() const { return Udm::IntegerAttr(impl, meta_Deadline); }

			static ::Uml::Attribute meta_Period;
			Udm::IntegerAttr Period() const { return Udm::IntegerAttr(impl, meta_Period); }

			static ::Uml::AssociationRole meta_referedbySystemRef;
			Udm::AssocAttr< ::EsMoL::Component> referedbySystemRef() const { return Udm::AssocAttr< ::EsMoL::Component>(impl, meta_referedbySystemRef); }
			template <class Pred> Udm::AssocAttr< ::EsMoL::Component, Pred > referedbySystemRef_sorted(const Pred &) const { return Udm::AssocAttr< ::EsMoL::Component, Pred>(impl, meta_referedbySystemRef); }

			static ::Uml::CompositionParentRole meta_System_parent;
			Udm::ParentAttr< ::EsMoL::System> System_parent() const { return Udm::ParentAttr< ::EsMoL::System>(impl, meta_System_parent); }

			Udm::ParentAttr< ::EsMoL::MgaObject> parent() const { return Udm::ParentAttr< ::EsMoL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  Subsystem :  public ::EsMoL::Block, public ::EsMoL::ComponentBase {
		public:
			static ::Uml::Class meta;

			Subsystem() { }
			Subsystem(Udm::ObjectImpl *impl) : ::EsMoL::Block(impl),::EsMoL::ComponentBase(impl), ::EsMoL::MgaObject(impl) { }
			Subsystem(const Subsystem &master) : ::EsMoL::Block(master),::EsMoL::ComponentBase(master), ::EsMoL::MgaObject(master) { }
			static Subsystem Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static Subsystem Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			Subsystem CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			Subsystem CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::EsMoL::Subsystem> Instances() { return Udm::InstantiatedAttr< ::EsMoL::Subsystem>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::EsMoL::Subsystem, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::EsMoL::Subsystem, Pred>(impl);}

			Udm::DerivedAttr< ::EsMoL::Subsystem> Derived() { return Udm::DerivedAttr< ::EsMoL::Subsystem>(impl);}
			template <class Pred> Udm::DerivedAttr< ::EsMoL::Subsystem, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::EsMoL::Subsystem, Pred>(impl);}

			Udm::ArchetypeAttr< ::EsMoL::Subsystem> Archetype() { return Udm::ArchetypeAttr< ::EsMoL::Subsystem>(impl);}

			static ::Uml::AssociationRole meta_referedbySubsystemRef;
			Udm::AssocAttr< ::EsMoL::SubsystemRef> referedbySubsystemRef() const { return Udm::AssocAttr< ::EsMoL::SubsystemRef>(impl, meta_referedbySubsystemRef); }
			template <class Pred> Udm::AssocAttr< ::EsMoL::SubsystemRef, Pred > referedbySubsystemRef_sorted(const Pred &) const { return Udm::AssocAttr< ::EsMoL::SubsystemRef, Pred>(impl, meta_referedbySubsystemRef); }

			static ::Uml::CompositionChildRole meta_Block_children;
			Udm::ChildrenAttr< ::EsMoL::Block> Block_children() const { return Udm::ChildrenAttr< ::EsMoL::Block>(impl, meta_Block_children); }
			template <class Pred> Udm::ChildrenAttr< ::EsMoL::Block, Pred> Block_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::Block, Pred>(impl, meta_Block_children); }

			Udm::ChildrenAttr< ::EsMoL::Subsystem> Subsystem_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::Subsystem>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::Subsystem, Pred> Subsystem_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::Subsystem, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::EsMoL::Primitive> Primitive_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::Primitive>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::Primitive, Pred> Primitive_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::Primitive, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::EsMoL::Block> Block_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::Block>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::Block, Pred> Block_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::Block, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::EsMoL::SLReference> SLReference_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::SLReference>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::SLReference, Pred> SLReference_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::SLReference, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::EsMoL::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::MgaObject>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

			static ::Uml::CompositionParentRole meta_DesignFolder_parent;
			Udm::ParentAttr< ::EsMoL::DesignFolder> DesignFolder_parent() const { return Udm::ParentAttr< ::EsMoL::DesignFolder>(impl, meta_DesignFolder_parent); }

			static ::Uml::CompositionParentRole meta_Dataflow_parent;
			Udm::ParentAttr< ::EsMoL::Dataflow> Dataflow_parent() const { return Udm::ParentAttr< ::EsMoL::Dataflow>(impl, meta_Dataflow_parent); }

			Udm::ParentAttr<Udm::Object> parent() const { return Udm::ParentAttr<Udm::Object>(impl, Udm::NULLPARENTROLE); }
		};

		class  SubsystemRef :  public ::EsMoL::ComponentBase {
		public:
			static ::Uml::Class meta;

			SubsystemRef() { }
			SubsystemRef(Udm::ObjectImpl *impl) : ::EsMoL::ComponentBase(impl), ::EsMoL::MgaObject(impl) { }
			SubsystemRef(const SubsystemRef &master) : ::EsMoL::ComponentBase(master), ::EsMoL::MgaObject(master) { }
			static SubsystemRef Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static SubsystemRef Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			SubsystemRef CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			SubsystemRef CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::EsMoL::SubsystemRef> Instances() { return Udm::InstantiatedAttr< ::EsMoL::SubsystemRef>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::EsMoL::SubsystemRef, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::EsMoL::SubsystemRef, Pred>(impl);}

			Udm::DerivedAttr< ::EsMoL::SubsystemRef> Derived() { return Udm::DerivedAttr< ::EsMoL::SubsystemRef>(impl);}
			template <class Pred> Udm::DerivedAttr< ::EsMoL::SubsystemRef, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::EsMoL::SubsystemRef, Pred>(impl);}

			Udm::ArchetypeAttr< ::EsMoL::SubsystemRef> Archetype() { return Udm::ArchetypeAttr< ::EsMoL::SubsystemRef>(impl);}

			static ::Uml::AssociationRole meta_ref;
			Udm::PointerAttr< ::EsMoL::Subsystem> ref() const { return Udm::PointerAttr< ::EsMoL::Subsystem>(impl, meta_ref); }

			Udm::ParentAttr< ::EsMoL::MgaObject> parent() const { return Udm::ParentAttr< ::EsMoL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  CCode :  public ::EsMoL::ComponentBase {
		public:
			static ::Uml::Class meta;

			CCode() { }
			CCode(Udm::ObjectImpl *impl) : ::EsMoL::ComponentBase(impl), ::EsMoL::MgaObject(impl) { }
			CCode(const CCode &master) : ::EsMoL::ComponentBase(master), ::EsMoL::MgaObject(master) { }
			static CCode Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static CCode Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			CCode CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			CCode CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::EsMoL::CCode> Instances() { return Udm::InstantiatedAttr< ::EsMoL::CCode>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::EsMoL::CCode, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::EsMoL::CCode, Pred>(impl);}

			Udm::DerivedAttr< ::EsMoL::CCode> Derived() { return Udm::DerivedAttr< ::EsMoL::CCode>(impl);}
			template <class Pred> Udm::DerivedAttr< ::EsMoL::CCode, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::EsMoL::CCode, Pred>(impl);}

			Udm::ArchetypeAttr< ::EsMoL::CCode> Archetype() { return Udm::ArchetypeAttr< ::EsMoL::CCode>(impl);}

			static ::Uml::Attribute meta_Source;
			Udm::StringAttr Source() const { return Udm::StringAttr(impl, meta_Source); }

			static ::Uml::CompositionChildRole meta_Port_children;
			Udm::ChildrenAttr< ::EsMoL::Port> Port_children() const { return Udm::ChildrenAttr< ::EsMoL::Port>(impl, meta_Port_children); }
			template <class Pred> Udm::ChildrenAttr< ::EsMoL::Port, Pred> Port_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::Port, Pred>(impl, meta_Port_children); }

			Udm::ChildrenAttr< ::EsMoL::ActionPort> ActionPort_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::ActionPort>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::ActionPort, Pred> ActionPort_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::ActionPort, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::EsMoL::EnablePort> EnablePort_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::EnablePort>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::EnablePort, Pred> EnablePort_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::EnablePort, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::EsMoL::Port> Port_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::Port>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::Port, Pred> Port_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::Port, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::EsMoL::InPort> InPort_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::InPort>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::InPort, Pred> InPort_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::InPort, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::EsMoL::TriggerPort> TriggerPort_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::TriggerPort>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::TriggerPort, Pred> TriggerPort_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::TriggerPort, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::EsMoL::OutPort> OutPort_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::OutPort>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::OutPort, Pred> OutPort_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::OutPort, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::EsMoL::StatePort> StatePort_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::StatePort>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::StatePort, Pred> StatePort_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::StatePort, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::EsMoL::OutputPort> OutputPort_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::OutputPort>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::OutputPort, Pred> OutputPort_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::OutputPort, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::EsMoL::InputPort> InputPort_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::InputPort>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::InputPort, Pred> InputPort_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::InputPort, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::EsMoL::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::MgaObject>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ParentAttr< ::EsMoL::MgaObject> parent() const { return Udm::ParentAttr< ::EsMoL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  TimingConstraint :  public ::EsMoL::MgaObject {
		public:
			static ::Uml::Class meta;

			TimingConstraint() { }
			TimingConstraint(Udm::ObjectImpl *impl) : ::EsMoL::MgaObject(impl) { }
			TimingConstraint(const TimingConstraint &master) : ::EsMoL::MgaObject(master) { }
			static TimingConstraint Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static TimingConstraint Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			TimingConstraint CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			TimingConstraint CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::EsMoL::TimingConstraint> Instances() { return Udm::InstantiatedAttr< ::EsMoL::TimingConstraint>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::EsMoL::TimingConstraint, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::EsMoL::TimingConstraint, Pred>(impl);}

			Udm::DerivedAttr< ::EsMoL::TimingConstraint> Derived() { return Udm::DerivedAttr< ::EsMoL::TimingConstraint>(impl);}
			template <class Pred> Udm::DerivedAttr< ::EsMoL::TimingConstraint, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::EsMoL::TimingConstraint, Pred>(impl);}

			Udm::ArchetypeAttr< ::EsMoL::TimingConstraint> Archetype() { return Udm::ArchetypeAttr< ::EsMoL::TimingConstraint>(impl);}

			static ::Uml::Attribute meta_Latency;
			Udm::StringAttr Latency() const { return Udm::StringAttr(impl, meta_Latency); }

			static ::Uml::CompositionParentRole meta_TimingSheet_parent;
			Udm::ParentAttr< ::EsMoL::TimingSheet> TimingSheet_parent() const { return Udm::ParentAttr< ::EsMoL::TimingSheet>(impl, meta_TimingSheet_parent); }

			Udm::ParentAttr< ::EsMoL::MgaObject> parent() const { return Udm::ParentAttr< ::EsMoL::MgaObject>(impl, Udm::NULLPARENTROLE); }
			static ::Uml::AssociationRole meta_dstTimingConstraint_end_;
			Udm::AssocEndAttr< ::EsMoL::TaskRef> dstTimingConstraint_end() const { return Udm::AssocEndAttr< ::EsMoL::TaskRef>(impl, meta_dstTimingConstraint_end_); }

			static ::Uml::AssociationRole meta_srcTimingConstraint_end_;
			Udm::AssocEndAttr< ::EsMoL::TaskRef> srcTimingConstraint_end() const { return Udm::AssocEndAttr< ::EsMoL::TaskRef>(impl, meta_srcTimingConstraint_end_); }

		};

		class  TimingSheet :  public ::EsMoL::MgaObject {
		public:
			static ::Uml::Class meta;

			TimingSheet() { }
			TimingSheet(Udm::ObjectImpl *impl) : ::EsMoL::MgaObject(impl) { }
			TimingSheet(const TimingSheet &master) : ::EsMoL::MgaObject(master) { }
			static TimingSheet Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static TimingSheet Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			TimingSheet CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			TimingSheet CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::EsMoL::TimingSheet> Instances() { return Udm::InstantiatedAttr< ::EsMoL::TimingSheet>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::EsMoL::TimingSheet, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::EsMoL::TimingSheet, Pred>(impl);}

			Udm::DerivedAttr< ::EsMoL::TimingSheet> Derived() { return Udm::DerivedAttr< ::EsMoL::TimingSheet>(impl);}
			template <class Pred> Udm::DerivedAttr< ::EsMoL::TimingSheet, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::EsMoL::TimingSheet, Pred>(impl);}

			Udm::ArchetypeAttr< ::EsMoL::TimingSheet> Archetype() { return Udm::ArchetypeAttr< ::EsMoL::TimingSheet>(impl);}

			static ::Uml::CompositionChildRole meta_TimingConstraint_children;
			Udm::ChildrenAttr< ::EsMoL::TimingConstraint> TimingConstraint_children() const { return Udm::ChildrenAttr< ::EsMoL::TimingConstraint>(impl, meta_TimingConstraint_children); }
			template <class Pred> Udm::ChildrenAttr< ::EsMoL::TimingConstraint, Pred> TimingConstraint_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::TimingConstraint, Pred>(impl, meta_TimingConstraint_children); }

			static ::Uml::CompositionChildRole meta_TaskRef_children;
			Udm::ChildrenAttr< ::EsMoL::TaskRef> TaskRef_children() const { return Udm::ChildrenAttr< ::EsMoL::TaskRef>(impl, meta_TaskRef_children); }
			template <class Pred> Udm::ChildrenAttr< ::EsMoL::TaskRef, Pred> TaskRef_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::TaskRef, Pred>(impl, meta_TaskRef_children); }

			Udm::ChildrenAttr< ::EsMoL::TimingConstraint> TimingConstraint_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::TimingConstraint>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::TimingConstraint, Pred> TimingConstraint_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::TimingConstraint, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::EsMoL::TaskRef> TaskRef_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::TaskRef>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::TaskRef, Pred> TaskRef_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::TaskRef, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::EsMoL::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::MgaObject>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

			static ::Uml::CompositionParentRole meta_DesignFolder_parent;
			Udm::ParentAttr< ::EsMoL::DesignFolder> DesignFolder_parent() const { return Udm::ParentAttr< ::EsMoL::DesignFolder>(impl, meta_DesignFolder_parent); }

			static ::Uml::CompositionParentRole meta_RequirementsLibrary_parent;
			Udm::ParentAttr< ::EsMoL::RequirementsLibrary> RequirementsLibrary_parent() const { return Udm::ParentAttr< ::EsMoL::RequirementsLibrary>(impl, meta_RequirementsLibrary_parent); }

			Udm::ParentAttr<Udm::Object> parent() const { return Udm::ParentAttr<Udm::Object>(impl, Udm::NULLPARENTROLE); }
		};

		class  TaskRef :  public ::EsMoL::MgaObject {
		public:
			static ::Uml::Class meta;

			TaskRef() { }
			TaskRef(Udm::ObjectImpl *impl) : ::EsMoL::MgaObject(impl) { }
			TaskRef(const TaskRef &master) : ::EsMoL::MgaObject(master) { }
			static TaskRef Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static TaskRef Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			TaskRef CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			TaskRef CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::EsMoL::TaskRef> Instances() { return Udm::InstantiatedAttr< ::EsMoL::TaskRef>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::EsMoL::TaskRef, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::EsMoL::TaskRef, Pred>(impl);}

			Udm::DerivedAttr< ::EsMoL::TaskRef> Derived() { return Udm::DerivedAttr< ::EsMoL::TaskRef>(impl);}
			template <class Pred> Udm::DerivedAttr< ::EsMoL::TaskRef, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::EsMoL::TaskRef, Pred>(impl);}

			Udm::ArchetypeAttr< ::EsMoL::TaskRef> Archetype() { return Udm::ArchetypeAttr< ::EsMoL::TaskRef>(impl);}

			static ::Uml::AssociationRole meta_ref;
			Udm::PointerAttr< ::EsMoL::Task> ref() const { return Udm::PointerAttr< ::EsMoL::Task>(impl, meta_ref); }

			static ::Uml::AssociationRole meta_srcTimingConstraint;
			static ::Uml::AssociationRole meta_srcTimingConstraint_rev;
			Udm::AClassAssocAttr< ::EsMoL::TimingConstraint, ::EsMoL::TaskRef> srcTimingConstraint() const { return Udm::AClassAssocAttr< ::EsMoL::TimingConstraint, ::EsMoL::TaskRef>(impl, meta_srcTimingConstraint, meta_srcTimingConstraint_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::EsMoL::TimingConstraint, ::EsMoL::TaskRef, Pred> srcTimingConstraint_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::EsMoL::TimingConstraint, ::EsMoL::TaskRef, Pred>(impl, meta_srcTimingConstraint, meta_srcTimingConstraint_rev); }

			static ::Uml::AssociationRole meta_dstTimingConstraint;
			static ::Uml::AssociationRole meta_dstTimingConstraint_rev;
			Udm::AClassAssocAttr< ::EsMoL::TimingConstraint, ::EsMoL::TaskRef> dstTimingConstraint() const { return Udm::AClassAssocAttr< ::EsMoL::TimingConstraint, ::EsMoL::TaskRef>(impl, meta_dstTimingConstraint, meta_dstTimingConstraint_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::EsMoL::TimingConstraint, ::EsMoL::TaskRef, Pred> dstTimingConstraint_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::EsMoL::TimingConstraint, ::EsMoL::TaskRef, Pred>(impl, meta_dstTimingConstraint, meta_dstTimingConstraint_rev); }

			static ::Uml::CompositionParentRole meta_TimingSheet_parent;
			Udm::ParentAttr< ::EsMoL::TimingSheet> TimingSheet_parent() const { return Udm::ParentAttr< ::EsMoL::TimingSheet>(impl, meta_TimingSheet_parent); }

			Udm::ParentAttr< ::EsMoL::MgaObject> parent() const { return Udm::ParentAttr< ::EsMoL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  TransConnector :  public ::EsMoL::MgaObject {
		public:
			static ::Uml::Class meta;

			TransConnector() { }
			TransConnector(Udm::ObjectImpl *impl) : ::EsMoL::MgaObject(impl) { }
			TransConnector(const TransConnector &master) : ::EsMoL::MgaObject(master) { }
			static TransConnector Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static TransConnector Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			TransConnector CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			TransConnector CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::EsMoL::TransConnector> Instances() { return Udm::InstantiatedAttr< ::EsMoL::TransConnector>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::EsMoL::TransConnector, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::EsMoL::TransConnector, Pred>(impl);}

			Udm::DerivedAttr< ::EsMoL::TransConnector> Derived() { return Udm::DerivedAttr< ::EsMoL::TransConnector>(impl);}
			template <class Pred> Udm::DerivedAttr< ::EsMoL::TransConnector, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::EsMoL::TransConnector, Pred>(impl);}

			Udm::ArchetypeAttr< ::EsMoL::TransConnector> Archetype() { return Udm::ArchetypeAttr< ::EsMoL::TransConnector>(impl);}

			static ::Uml::AssociationRole meta_referedbyConnectorRef;
			Udm::AssocAttr< ::EsMoL::ConnectorRef> referedbyConnectorRef() const { return Udm::AssocAttr< ::EsMoL::ConnectorRef>(impl, meta_referedbyConnectorRef); }
			template <class Pred> Udm::AssocAttr< ::EsMoL::ConnectorRef, Pred > referedbyConnectorRef_sorted(const Pred &) const { return Udm::AssocAttr< ::EsMoL::ConnectorRef, Pred>(impl, meta_referedbyConnectorRef); }

			static ::Uml::AssociationRole meta_dstTransition;
			static ::Uml::AssociationRole meta_dstTransition_rev;
			Udm::AClassAssocAttr< ::EsMoL::Transition, ::EsMoL::TransConnector> dstTransition() const { return Udm::AClassAssocAttr< ::EsMoL::Transition, ::EsMoL::TransConnector>(impl, meta_dstTransition, meta_dstTransition_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::EsMoL::Transition, ::EsMoL::TransConnector, Pred> dstTransition_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::EsMoL::Transition, ::EsMoL::TransConnector, Pred>(impl, meta_dstTransition, meta_dstTransition_rev); }

			static ::Uml::AssociationRole meta_srcTransition;
			static ::Uml::AssociationRole meta_srcTransition_rev;
			Udm::AClassAssocAttr< ::EsMoL::Transition, ::EsMoL::TransConnector> srcTransition() const { return Udm::AClassAssocAttr< ::EsMoL::Transition, ::EsMoL::TransConnector>(impl, meta_srcTransition, meta_srcTransition_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::EsMoL::Transition, ::EsMoL::TransConnector, Pred> srcTransition_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::EsMoL::Transition, ::EsMoL::TransConnector, Pred>(impl, meta_srcTransition, meta_srcTransition_rev); }

			static ::Uml::CompositionParentRole meta_State_parent;
			Udm::ParentAttr< ::EsMoL::State> State_parent() const { return Udm::ParentAttr< ::EsMoL::State>(impl, meta_State_parent); }

			Udm::ParentAttr< ::EsMoL::MgaObject> parent() const { return Udm::ParentAttr< ::EsMoL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  ConnectorRef :  public ::EsMoL::TransConnector {
		public:
			static ::Uml::Class meta;

			ConnectorRef() { }
			ConnectorRef(Udm::ObjectImpl *impl) : ::EsMoL::TransConnector(impl) { }
			ConnectorRef(const ConnectorRef &master) : ::EsMoL::TransConnector(master) { }
			static ConnectorRef Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static ConnectorRef Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			ConnectorRef CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			ConnectorRef CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::EsMoL::ConnectorRef> Instances() { return Udm::InstantiatedAttr< ::EsMoL::ConnectorRef>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::EsMoL::ConnectorRef, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::EsMoL::ConnectorRef, Pred>(impl);}

			Udm::DerivedAttr< ::EsMoL::ConnectorRef> Derived() { return Udm::DerivedAttr< ::EsMoL::ConnectorRef>(impl);}
			template <class Pred> Udm::DerivedAttr< ::EsMoL::ConnectorRef, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::EsMoL::ConnectorRef, Pred>(impl);}

			Udm::ArchetypeAttr< ::EsMoL::ConnectorRef> Archetype() { return Udm::ArchetypeAttr< ::EsMoL::ConnectorRef>(impl);}

			static ::Uml::AssociationRole meta_ref;
			Udm::PointerAttr< ::EsMoL::TransConnector> ref() const { return Udm::PointerAttr< ::EsMoL::TransConnector>(impl, meta_ref); }

			static ::Uml::CompositionParentRole meta_Block_parent;
			Udm::ParentAttr< ::EsMoL::Block> Block_parent() const { return Udm::ParentAttr< ::EsMoL::Block>(impl, meta_Block_parent); }

			Udm::ParentAttr< ::EsMoL::MgaObject> parent() const { return Udm::ParentAttr< ::EsMoL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  History :  public ::EsMoL::TransConnector {
		public:
			static ::Uml::Class meta;

			History() { }
			History(Udm::ObjectImpl *impl) : ::EsMoL::TransConnector(impl) { }
			History(const History &master) : ::EsMoL::TransConnector(master) { }
			static History Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static History Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			History CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			History CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::EsMoL::History> Instances() { return Udm::InstantiatedAttr< ::EsMoL::History>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::EsMoL::History, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::EsMoL::History, Pred>(impl);}

			Udm::DerivedAttr< ::EsMoL::History> Derived() { return Udm::DerivedAttr< ::EsMoL::History>(impl);}
			template <class Pred> Udm::DerivedAttr< ::EsMoL::History, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::EsMoL::History, Pred>(impl);}

			Udm::ArchetypeAttr< ::EsMoL::History> Archetype() { return Udm::ArchetypeAttr< ::EsMoL::History>(impl);}

			Udm::ParentAttr< ::EsMoL::MgaObject> parent() const { return Udm::ParentAttr< ::EsMoL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  StateDE :  public ::EsMoL::MgaObject {
		public:
			static ::Uml::Class meta;

			StateDE() { }
			StateDE(Udm::ObjectImpl *impl) : ::EsMoL::MgaObject(impl) { }
			StateDE(const StateDE &master) : ::EsMoL::MgaObject(master) { }
			static StateDE Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static StateDE Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			StateDE CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			StateDE CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::EsMoL::StateDE> Instances() { return Udm::InstantiatedAttr< ::EsMoL::StateDE>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::EsMoL::StateDE, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::EsMoL::StateDE, Pred>(impl);}

			Udm::DerivedAttr< ::EsMoL::StateDE> Derived() { return Udm::DerivedAttr< ::EsMoL::StateDE>(impl);}
			template <class Pred> Udm::DerivedAttr< ::EsMoL::StateDE, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::EsMoL::StateDE, Pred>(impl);}

			Udm::ArchetypeAttr< ::EsMoL::StateDE> Archetype() { return Udm::ArchetypeAttr< ::EsMoL::StateDE>(impl);}

			static ::Uml::CompositionChildRole meta_TypeBaseRef_child;
			Udm::ChildAttr< ::EsMoL::TypeBaseRef> TypeBaseRef_child() const { return Udm::ChildAttr< ::EsMoL::TypeBaseRef>(impl, meta_TypeBaseRef_child); }

			Udm::ChildrenAttr< ::EsMoL::TypeBaseRef> TypeBaseRef_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::TypeBaseRef>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::TypeBaseRef, Pred> TypeBaseRef_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::TypeBaseRef, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::EsMoL::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::MgaObject>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

			static ::Uml::CompositionParentRole meta_State_parent;
			Udm::ParentAttr< ::EsMoL::State> State_parent() const { return Udm::ParentAttr< ::EsMoL::State>(impl, meta_State_parent); }

			Udm::ParentAttr< ::EsMoL::MgaObject> parent() const { return Udm::ParentAttr< ::EsMoL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  Event :  public ::EsMoL::StateDE {
		public:
			static ::Uml::Class meta;

			Event() { }
			Event(Udm::ObjectImpl *impl) : ::EsMoL::StateDE(impl) { }
			Event(const Event &master) : ::EsMoL::StateDE(master) { }
			static Event Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static Event Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			Event CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			Event CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::EsMoL::Event> Instances() { return Udm::InstantiatedAttr< ::EsMoL::Event>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::EsMoL::Event, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::EsMoL::Event, Pred>(impl);}

			Udm::DerivedAttr< ::EsMoL::Event> Derived() { return Udm::DerivedAttr< ::EsMoL::Event>(impl);}
			template <class Pred> Udm::DerivedAttr< ::EsMoL::Event, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::EsMoL::Event, Pred>(impl);}

			Udm::ArchetypeAttr< ::EsMoL::Event> Archetype() { return Udm::ArchetypeAttr< ::EsMoL::Event>(impl);}

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

			Udm::ParentAttr< ::EsMoL::MgaObject> parent() const { return Udm::ParentAttr< ::EsMoL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  Data :  public ::EsMoL::StateDE {
		public:
			static ::Uml::Class meta;

			Data() { }
			Data(Udm::ObjectImpl *impl) : ::EsMoL::StateDE(impl) { }
			Data(const Data &master) : ::EsMoL::StateDE(master) { }
			static Data Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static Data Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			Data CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			Data CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::EsMoL::Data> Instances() { return Udm::InstantiatedAttr< ::EsMoL::Data>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::EsMoL::Data, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::EsMoL::Data, Pred>(impl);}

			Udm::DerivedAttr< ::EsMoL::Data> Derived() { return Udm::DerivedAttr< ::EsMoL::Data>(impl);}
			template <class Pred> Udm::DerivedAttr< ::EsMoL::Data, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::EsMoL::Data, Pred>(impl);}

			Udm::ArchetypeAttr< ::EsMoL::Data> Archetype() { return Udm::ArchetypeAttr< ::EsMoL::Data>(impl);}

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

			Udm::ParentAttr< ::EsMoL::MgaObject> parent() const { return Udm::ParentAttr< ::EsMoL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  Transition :  public ::EsMoL::MgaObject {
		public:
			static ::Uml::Class meta;

			Transition() { }
			Transition(Udm::ObjectImpl *impl) : ::EsMoL::MgaObject(impl) { }
			Transition(const Transition &master) : ::EsMoL::MgaObject(master) { }
			static Transition Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static Transition Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			Transition CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			Transition CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::EsMoL::Transition> Instances() { return Udm::InstantiatedAttr< ::EsMoL::Transition>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::EsMoL::Transition, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::EsMoL::Transition, Pred>(impl);}

			Udm::DerivedAttr< ::EsMoL::Transition> Derived() { return Udm::DerivedAttr< ::EsMoL::Transition>(impl);}
			template <class Pred> Udm::DerivedAttr< ::EsMoL::Transition, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::EsMoL::Transition, Pred>(impl);}

			Udm::ArchetypeAttr< ::EsMoL::Transition> Archetype() { return Udm::ArchetypeAttr< ::EsMoL::Transition>(impl);}

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
			Udm::ParentAttr< ::EsMoL::State> Transition_State_parent() const { return Udm::ParentAttr< ::EsMoL::State>(impl, meta_Transition_State_parent); }

			Udm::ParentAttr< ::EsMoL::MgaObject> parent() const { return Udm::ParentAttr< ::EsMoL::MgaObject>(impl, Udm::NULLPARENTROLE); }
			static ::Uml::AssociationRole meta_srcTransition_end_;
			Udm::AssocEndAttr< ::EsMoL::TransConnector> srcTransition_end() const { return Udm::AssocEndAttr< ::EsMoL::TransConnector>(impl, meta_srcTransition_end_); }

			static ::Uml::AssociationRole meta_dstTransition_end_;
			Udm::AssocEndAttr< ::EsMoL::TransConnector> dstTransition_end() const { return Udm::AssocEndAttr< ::EsMoL::TransConnector>(impl, meta_dstTransition_end_); }

		};

		class  State :  public ::EsMoL::TransConnector {
		public:
			static ::Uml::Class meta;

			State() { }
			State(Udm::ObjectImpl *impl) : ::EsMoL::TransConnector(impl) { }
			State(const State &master) : ::EsMoL::TransConnector(master) { }
			static State Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static State Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			State CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			State CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::EsMoL::State> Instances() { return Udm::InstantiatedAttr< ::EsMoL::State>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::EsMoL::State, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::EsMoL::State, Pred>(impl);}

			Udm::DerivedAttr< ::EsMoL::State> Derived() { return Udm::DerivedAttr< ::EsMoL::State>(impl);}
			template <class Pred> Udm::DerivedAttr< ::EsMoL::State, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::EsMoL::State, Pred>(impl);}

			Udm::ArchetypeAttr< ::EsMoL::State> Archetype() { return Udm::ArchetypeAttr< ::EsMoL::State>(impl);}

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
			Udm::ChildrenAttr< ::EsMoL::TransConnector> TransConnector_children() const { return Udm::ChildrenAttr< ::EsMoL::TransConnector>(impl, meta_TransConnector_children); }
			template <class Pred> Udm::ChildrenAttr< ::EsMoL::TransConnector, Pred> TransConnector_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::TransConnector, Pred>(impl, meta_TransConnector_children); }

			static ::Uml::CompositionChildRole meta_StateDE_children;
			Udm::ChildrenAttr< ::EsMoL::StateDE> StateDE_children() const { return Udm::ChildrenAttr< ::EsMoL::StateDE>(impl, meta_StateDE_children); }
			template <class Pred> Udm::ChildrenAttr< ::EsMoL::StateDE, Pred> StateDE_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::StateDE, Pred>(impl, meta_StateDE_children); }

			static ::Uml::CompositionChildRole meta_Transition;
			Udm::ChildrenAttr< ::EsMoL::Transition> Transition() const { return Udm::ChildrenAttr< ::EsMoL::Transition>(impl, meta_Transition); }
			template <class Pred> Udm::ChildrenAttr< ::EsMoL::Transition, Pred> Transition_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::Transition, Pred>(impl, meta_Transition); }

			Udm::ChildrenAttr< ::EsMoL::ConnectorRef> ConnectorRef_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::ConnectorRef>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::ConnectorRef, Pred> ConnectorRef_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::ConnectorRef, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::EsMoL::TransConnector> TransConnector_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::TransConnector>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::TransConnector, Pred> TransConnector_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::TransConnector, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::EsMoL::History> History_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::History>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::History, Pred> History_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::History, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::EsMoL::Event> Event_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::Event>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::Event, Pred> Event_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::Event, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::EsMoL::StateDE> StateDE_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::StateDE>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::StateDE, Pred> StateDE_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::StateDE, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::EsMoL::Data> Data_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::Data>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::Data, Pred> Data_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::Data, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::EsMoL::Transition> Transition_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::Transition>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::Transition, Pred> Transition_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::Transition, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::EsMoL::State> State_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::State>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::State, Pred> State_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::State, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::EsMoL::TransStart> TransStart_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::TransStart>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::TransStart, Pred> TransStart_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::TransStart, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::EsMoL::Junction> Junction_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::Junction>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::Junction, Pred> Junction_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::Junction, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::EsMoL::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr< ::EsMoL::MgaObject>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::EsMoL::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::EsMoL::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

			static ::Uml::CompositionParentRole meta_Stateflow_parent;
			Udm::ParentAttr< ::EsMoL::Stateflow> Stateflow_parent() const { return Udm::ParentAttr< ::EsMoL::Stateflow>(impl, meta_Stateflow_parent); }

			Udm::ParentAttr<Udm::Object> parent() const { return Udm::ParentAttr<Udm::Object>(impl, Udm::NULLPARENTROLE); }
		};

		class  TransStart :  public ::EsMoL::TransConnector {
		public:
			static ::Uml::Class meta;

			TransStart() { }
			TransStart(Udm::ObjectImpl *impl) : ::EsMoL::TransConnector(impl) { }
			TransStart(const TransStart &master) : ::EsMoL::TransConnector(master) { }
			static TransStart Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static TransStart Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			TransStart CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			TransStart CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::EsMoL::TransStart> Instances() { return Udm::InstantiatedAttr< ::EsMoL::TransStart>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::EsMoL::TransStart, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::EsMoL::TransStart, Pred>(impl);}

			Udm::DerivedAttr< ::EsMoL::TransStart> Derived() { return Udm::DerivedAttr< ::EsMoL::TransStart>(impl);}
			template <class Pred> Udm::DerivedAttr< ::EsMoL::TransStart, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::EsMoL::TransStart, Pred>(impl);}

			Udm::ArchetypeAttr< ::EsMoL::TransStart> Archetype() { return Udm::ArchetypeAttr< ::EsMoL::TransStart>(impl);}

			Udm::ParentAttr< ::EsMoL::MgaObject> parent() const { return Udm::ParentAttr< ::EsMoL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  Junction :  public ::EsMoL::TransConnector {
		public:
			static ::Uml::Class meta;

			Junction() { }
			Junction(Udm::ObjectImpl *impl) : ::EsMoL::TransConnector(impl) { }
			Junction(const Junction &master) : ::EsMoL::TransConnector(master) { }
			static Junction Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static Junction Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			Junction CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			Junction CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::EsMoL::Junction> Instances() { return Udm::InstantiatedAttr< ::EsMoL::Junction>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::EsMoL::Junction, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::EsMoL::Junction, Pred>(impl);}

			Udm::DerivedAttr< ::EsMoL::Junction> Derived() { return Udm::DerivedAttr< ::EsMoL::Junction>(impl);}
			template <class Pred> Udm::DerivedAttr< ::EsMoL::Junction, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::EsMoL::Junction, Pred>(impl);}

			Udm::ArchetypeAttr< ::EsMoL::Junction> Archetype() { return Udm::ArchetypeAttr< ::EsMoL::Junction>(impl);}

			Udm::ParentAttr< ::EsMoL::MgaObject> parent() const { return Udm::ParentAttr< ::EsMoL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};



}

#endif //MOBIES_ESMOL_H
