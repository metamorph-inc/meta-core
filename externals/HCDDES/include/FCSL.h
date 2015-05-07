#ifndef MOBIES_FCSL_H
#define MOBIES_FCSL_H
// header file FCSL.h generated from diagram FCSL
// generated on Mon Dec 10 10:56:36 2007

#ifndef MOBIES_UDMBASE_H
#include "UdmBase.h"
#endif

#ifdef min
#undef min
#endif

#ifdef max
#undef max
#endif
namespace FCSL {
	extern  Udm::UdmDiagram diagram;
	 void Initialize(const ::Uml::Diagram &dgr);
	 void Initialize();

		class  EventPortMapping_Members_Base;
		class  CommMapping_Members_Base;
		class  PortMapping_Members_Base;
		class  ScheduleItem;
		class  Partition;
		class  CommMapping;
		class  Process;
		class  OperatingMode;
		class  ProcessingModule;
		class  DevicePort;
		class  Bus;
		class  BusPort;
		class  HWElement;
		class  Wire;
		class  HardwareModels;
		class  CommElement;
		class  HardwareSheet;
		class  Port2Channel;
		class  Channel2Port;
		class  EventPort;
		class  Port;
		class  Channel;
		class  EventChannel;
		class  ComponentSheet;
		class  Component;
		class  EventPortMapping;
		class  OutputPort;
		class  OE2EC;
		class  EC2IE;
		class  ComponentModels;
		class  BitTestRef;
		class  PortMapping;
		class  InputEvent;
		class  SystemRef;
		class  OutputEvent;
		class  InputPort;
		class  ComponentShortcut;
		class  RootFolder;
		class  MgaObject;


		 void CreateMetaObjs();
		 void InitCrossNSInheritence();
		 void InitCrossNSCompositions();
		 void InitCrossNSAssociations();
	namespace FM {
		class  TestModels;
		class  PlatformModels;
		class  UnitModels;
		class  FM2DOP;
		class  Device;
		class  Sig2Dataport;
		class  Device2Surface;
		class  DiscrepancyPort;
		class  FailureMode;
		class  Assembly;
		class  DIP2D;
		class  Discrepancy;
		class  FM2D;
		class  DiscrepancyOutPort;
		class  SignalflowConn;
		class  DOP2T;
		class  Surface;
		class  DataOut;
		class  Ctrl2Device;
		class  FaultModelElementPort;
		class  DataIn;
		class  Test;
		class  TestContainer;
		class  TestGroup;
		class  D2Test;
		class  Control;
		class  FaultModelElement;
		class  FailureModeRef;
		class  Platform;
		class  DOP2D;
		class  D2DOP;
		class  Entity;
		class  Unit;
		class  DiscrepancyInPort;
		class  DataPort;
		class  FailureProp;
		class  DIP2DOP;
		class  TestInstance;
		class  Signal;
		class  TestRef;

		 extern ::Uml::Namespace meta;

		 void CreateMetaObjs();
		 void InitCrossNSInheritence();
		 void InitCrossNSCompositions();
		 void InitCrossNSAssociations();
		 void InitializeNS();
		 void InitializeNS2();
		 void InitializeNSSetupMeta(const ::Uml::Namespace &ns);

	}
	namespace SLSF {
		class  Dataflow;
		class  Types;
		class  Stateflow;
		class  Block;
		class  EnablePort;
		class  OutPort;
		class  Annotation;
		class  OutputPort;
		class  Parameter;
		class  InputPort;
		class  Line;
		class  InPort;
		class  Primitive;
		class  StatePort;
		class  TriggerPort;
		class  Port;
		class  System;
		class  Reference;
		class  TypeBase;
		class  Matrix;
		class  TypeBaseRef;
		class  TypeStruct;
		class  Data;
		class  ConnectorRef;
		class  State;
		class  Event;
		class  Transition;
		class  TransStart;
		class  Junction;
		class  TransConnector;
		class  History;

		 extern ::Uml::Namespace meta;

		 void CreateMetaObjs();
		 void InitCrossNSInheritence();
		 void InitCrossNSCompositions();
		 void InitCrossNSAssociations();
		 void InitializeNS();
		 void InitializeNS2();
		 void InitializeNSSetupMeta(const ::Uml::Namespace &ns);

	}
		class  EventPortMapping_Members_Base :  virtual public Udm::Object {
		public:
			static ::Uml::Class meta;

			EventPortMapping_Members_Base() { }
			EventPortMapping_Members_Base(Udm::ObjectImpl *impl) : UDM_OBJECT(impl) { }
			EventPortMapping_Members_Base(const EventPortMapping_Members_Base &master) : UDM_OBJECT(master) { }
			static EventPortMapping_Members_Base Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static EventPortMapping_Members_Base Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			EventPortMapping_Members_Base CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			EventPortMapping_Members_Base CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::FCSL::EventPortMapping_Members_Base> Instances() { return Udm::InstantiatedAttr< ::FCSL::EventPortMapping_Members_Base>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::FCSL::EventPortMapping_Members_Base, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::FCSL::EventPortMapping_Members_Base, Pred>(impl);}

			Udm::DerivedAttr< ::FCSL::EventPortMapping_Members_Base> Derived() { return Udm::DerivedAttr< ::FCSL::EventPortMapping_Members_Base>(impl);}
			template <class Pred> Udm::DerivedAttr< ::FCSL::EventPortMapping_Members_Base, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::FCSL::EventPortMapping_Members_Base, Pred>(impl);}

			Udm::ArchetypeAttr< ::FCSL::EventPortMapping_Members_Base> Archetype() const { return Udm::ArchetypeAttr< ::FCSL::EventPortMapping_Members_Base>(impl);}

			static ::Uml::AssociationRole meta_srcEventPortMapping;
			static ::Uml::AssociationRole meta_srcEventPortMapping_rev;
			Udm::AClassAssocAttr< ::FCSL::EventPortMapping, ::FCSL::EventPortMapping_Members_Base> srcEventPortMapping() const { return Udm::AClassAssocAttr< ::FCSL::EventPortMapping, ::FCSL::EventPortMapping_Members_Base>(impl, meta_srcEventPortMapping, meta_srcEventPortMapping_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::FCSL::EventPortMapping, ::FCSL::EventPortMapping_Members_Base, Pred> srcEventPortMapping_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::FCSL::EventPortMapping, ::FCSL::EventPortMapping_Members_Base, Pred>(impl, meta_srcEventPortMapping, meta_srcEventPortMapping_rev); }

			static ::Uml::AssociationRole meta_dstEventPortMapping;
			static ::Uml::AssociationRole meta_dstEventPortMapping_rev;
			Udm::AClassAssocAttr< ::FCSL::EventPortMapping, ::FCSL::EventPortMapping_Members_Base> dstEventPortMapping() const { return Udm::AClassAssocAttr< ::FCSL::EventPortMapping, ::FCSL::EventPortMapping_Members_Base>(impl, meta_dstEventPortMapping, meta_dstEventPortMapping_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::FCSL::EventPortMapping, ::FCSL::EventPortMapping_Members_Base, Pred> dstEventPortMapping_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::FCSL::EventPortMapping, ::FCSL::EventPortMapping_Members_Base, Pred>(impl, meta_dstEventPortMapping, meta_dstEventPortMapping_rev); }

			Udm::ParentAttr<Udm::Object> parent() const { return Udm::ParentAttr<Udm::Object>(impl, Udm::NULLPARENTROLE); }
		};

		class  CommMapping_Members_Base :  virtual public Udm::Object {
		public:
			static ::Uml::Class meta;

			CommMapping_Members_Base() { }
			CommMapping_Members_Base(Udm::ObjectImpl *impl) : UDM_OBJECT(impl) { }
			CommMapping_Members_Base(const CommMapping_Members_Base &master) : UDM_OBJECT(master) { }
			static CommMapping_Members_Base Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static CommMapping_Members_Base Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			CommMapping_Members_Base CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			CommMapping_Members_Base CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::FCSL::CommMapping_Members_Base> Instances() { return Udm::InstantiatedAttr< ::FCSL::CommMapping_Members_Base>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::FCSL::CommMapping_Members_Base, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::FCSL::CommMapping_Members_Base, Pred>(impl);}

			Udm::DerivedAttr< ::FCSL::CommMapping_Members_Base> Derived() { return Udm::DerivedAttr< ::FCSL::CommMapping_Members_Base>(impl);}
			template <class Pred> Udm::DerivedAttr< ::FCSL::CommMapping_Members_Base, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::FCSL::CommMapping_Members_Base, Pred>(impl);}

			Udm::ArchetypeAttr< ::FCSL::CommMapping_Members_Base> Archetype() const { return Udm::ArchetypeAttr< ::FCSL::CommMapping_Members_Base>(impl);}

			static ::Uml::AssociationRole meta_srcCommMapping;
			static ::Uml::AssociationRole meta_srcCommMapping_rev;
			Udm::AClassAssocAttr< ::FCSL::CommMapping, ::FCSL::CommMapping_Members_Base> srcCommMapping() const { return Udm::AClassAssocAttr< ::FCSL::CommMapping, ::FCSL::CommMapping_Members_Base>(impl, meta_srcCommMapping, meta_srcCommMapping_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::FCSL::CommMapping, ::FCSL::CommMapping_Members_Base, Pred> srcCommMapping_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::FCSL::CommMapping, ::FCSL::CommMapping_Members_Base, Pred>(impl, meta_srcCommMapping, meta_srcCommMapping_rev); }

			static ::Uml::AssociationRole meta_dstCommMapping;
			static ::Uml::AssociationRole meta_dstCommMapping_rev;
			Udm::AClassAssocAttr< ::FCSL::CommMapping, ::FCSL::CommMapping_Members_Base> dstCommMapping() const { return Udm::AClassAssocAttr< ::FCSL::CommMapping, ::FCSL::CommMapping_Members_Base>(impl, meta_dstCommMapping, meta_dstCommMapping_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::FCSL::CommMapping, ::FCSL::CommMapping_Members_Base, Pred> dstCommMapping_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::FCSL::CommMapping, ::FCSL::CommMapping_Members_Base, Pred>(impl, meta_dstCommMapping, meta_dstCommMapping_rev); }

			Udm::ParentAttr<Udm::Object> parent() const { return Udm::ParentAttr<Udm::Object>(impl, Udm::NULLPARENTROLE); }
		};

		class  PortMapping_Members_Base :  virtual public Udm::Object {
		public:
			static ::Uml::Class meta;

			PortMapping_Members_Base() { }
			PortMapping_Members_Base(Udm::ObjectImpl *impl) : UDM_OBJECT(impl) { }
			PortMapping_Members_Base(const PortMapping_Members_Base &master) : UDM_OBJECT(master) { }
			static PortMapping_Members_Base Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static PortMapping_Members_Base Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			PortMapping_Members_Base CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			PortMapping_Members_Base CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::FCSL::PortMapping_Members_Base> Instances() { return Udm::InstantiatedAttr< ::FCSL::PortMapping_Members_Base>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::FCSL::PortMapping_Members_Base, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::FCSL::PortMapping_Members_Base, Pred>(impl);}

			Udm::DerivedAttr< ::FCSL::PortMapping_Members_Base> Derived() { return Udm::DerivedAttr< ::FCSL::PortMapping_Members_Base>(impl);}
			template <class Pred> Udm::DerivedAttr< ::FCSL::PortMapping_Members_Base, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::FCSL::PortMapping_Members_Base, Pred>(impl);}

			Udm::ArchetypeAttr< ::FCSL::PortMapping_Members_Base> Archetype() const { return Udm::ArchetypeAttr< ::FCSL::PortMapping_Members_Base>(impl);}

			static ::Uml::AssociationRole meta_srcInPortMapping;
			static ::Uml::AssociationRole meta_srcInPortMapping_rev;
			Udm::AClassAssocAttr< ::FCSL::PortMapping, ::FCSL::PortMapping_Members_Base> srcInPortMapping() const { return Udm::AClassAssocAttr< ::FCSL::PortMapping, ::FCSL::PortMapping_Members_Base>(impl, meta_srcInPortMapping, meta_srcInPortMapping_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::FCSL::PortMapping, ::FCSL::PortMapping_Members_Base, Pred> srcInPortMapping_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::FCSL::PortMapping, ::FCSL::PortMapping_Members_Base, Pred>(impl, meta_srcInPortMapping, meta_srcInPortMapping_rev); }

			static ::Uml::AssociationRole meta_dstInPortMapping;
			static ::Uml::AssociationRole meta_dstInPortMapping_rev;
			Udm::AClassAssocAttr< ::FCSL::PortMapping, ::FCSL::PortMapping_Members_Base> dstInPortMapping() const { return Udm::AClassAssocAttr< ::FCSL::PortMapping, ::FCSL::PortMapping_Members_Base>(impl, meta_dstInPortMapping, meta_dstInPortMapping_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::FCSL::PortMapping, ::FCSL::PortMapping_Members_Base, Pred> dstInPortMapping_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::FCSL::PortMapping, ::FCSL::PortMapping_Members_Base, Pred>(impl, meta_dstInPortMapping, meta_dstInPortMapping_rev); }

			Udm::ParentAttr<Udm::Object> parent() const { return Udm::ParentAttr<Udm::Object>(impl, Udm::NULLPARENTROLE); }
		};

		class  HardwareModels :  public Udm::Object {
		public:
			static ::Uml::Class meta;

			HardwareModels() { }
			HardwareModels(Udm::ObjectImpl *impl) : UDM_OBJECT(impl) { }
			HardwareModels(const HardwareModels &master) : UDM_OBJECT(master) { }
			static HardwareModels Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static HardwareModels Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			HardwareModels CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			HardwareModels CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::FCSL::HardwareModels> Instances() { return Udm::InstantiatedAttr< ::FCSL::HardwareModels>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::FCSL::HardwareModels, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::FCSL::HardwareModels, Pred>(impl);}

			Udm::DerivedAttr< ::FCSL::HardwareModels> Derived() { return Udm::DerivedAttr< ::FCSL::HardwareModels>(impl);}
			template <class Pred> Udm::DerivedAttr< ::FCSL::HardwareModels, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::FCSL::HardwareModels, Pred>(impl);}

			Udm::ArchetypeAttr< ::FCSL::HardwareModels> Archetype() const { return Udm::ArchetypeAttr< ::FCSL::HardwareModels>(impl);}

			static ::Uml::Attribute meta_name;
			Udm::StringAttr name() const { return Udm::StringAttr(impl, meta_name); }

			static ::Uml::CompositionChildRole meta_HardwareSheet_children;
			Udm::ChildrenAttr< ::FCSL::HardwareSheet> HardwareSheet_children() const { return Udm::ChildrenAttr< ::FCSL::HardwareSheet>(impl, meta_HardwareSheet_children); }
			template <class Pred> Udm::ChildrenAttr< ::FCSL::HardwareSheet, Pred> HardwareSheet_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::HardwareSheet, Pred>(impl, meta_HardwareSheet_children); }

			Udm::ChildrenAttr< ::FCSL::HardwareSheet> HardwareSheet_kind_children() const { return Udm::ChildrenAttr< ::FCSL::HardwareSheet>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::HardwareSheet, Pred> HardwareSheet_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::HardwareSheet, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr< ::FCSL::MgaObject>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

			static ::Uml::CompositionParentRole meta_RootFolder_parent;
			Udm::ParentAttr< ::FCSL::RootFolder> RootFolder_parent() const { return Udm::ParentAttr< ::FCSL::RootFolder>(impl, meta_RootFolder_parent); }

			Udm::ParentAttr< ::FCSL::RootFolder> parent() const { return Udm::ParentAttr< ::FCSL::RootFolder>(impl, Udm::NULLPARENTROLE); }
		};

		class  ComponentModels :  public Udm::Object {
		public:
			static ::Uml::Class meta;

			ComponentModels() { }
			ComponentModels(Udm::ObjectImpl *impl) : UDM_OBJECT(impl) { }
			ComponentModels(const ComponentModels &master) : UDM_OBJECT(master) { }
			static ComponentModels Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static ComponentModels Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			ComponentModels CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			ComponentModels CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::FCSL::ComponentModels> Instances() { return Udm::InstantiatedAttr< ::FCSL::ComponentModels>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::FCSL::ComponentModels, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::FCSL::ComponentModels, Pred>(impl);}

			Udm::DerivedAttr< ::FCSL::ComponentModels> Derived() { return Udm::DerivedAttr< ::FCSL::ComponentModels>(impl);}
			template <class Pred> Udm::DerivedAttr< ::FCSL::ComponentModels, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::FCSL::ComponentModels, Pred>(impl);}

			Udm::ArchetypeAttr< ::FCSL::ComponentModels> Archetype() const { return Udm::ArchetypeAttr< ::FCSL::ComponentModels>(impl);}

			static ::Uml::Attribute meta_name;
			Udm::StringAttr name() const { return Udm::StringAttr(impl, meta_name); }

			static ::Uml::CompositionChildRole meta_ComponentSheet_children;
			Udm::ChildrenAttr< ::FCSL::ComponentSheet> ComponentSheet_children() const { return Udm::ChildrenAttr< ::FCSL::ComponentSheet>(impl, meta_ComponentSheet_children); }
			template <class Pred> Udm::ChildrenAttr< ::FCSL::ComponentSheet, Pred> ComponentSheet_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::ComponentSheet, Pred>(impl, meta_ComponentSheet_children); }

			Udm::ChildrenAttr< ::FCSL::ComponentSheet> ComponentSheet_kind_children() const { return Udm::ChildrenAttr< ::FCSL::ComponentSheet>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::ComponentSheet, Pred> ComponentSheet_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::ComponentSheet, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr< ::FCSL::MgaObject>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

			static ::Uml::CompositionParentRole meta_RootFolder_parent;
			Udm::ParentAttr< ::FCSL::RootFolder> RootFolder_parent() const { return Udm::ParentAttr< ::FCSL::RootFolder>(impl, meta_RootFolder_parent); }

			Udm::ParentAttr< ::FCSL::RootFolder> parent() const { return Udm::ParentAttr< ::FCSL::RootFolder>(impl, Udm::NULLPARENTROLE); }
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

			Udm::InstantiatedAttr< ::FCSL::RootFolder> Instances() { return Udm::InstantiatedAttr< ::FCSL::RootFolder>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::FCSL::RootFolder, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::FCSL::RootFolder, Pred>(impl);}

			Udm::DerivedAttr< ::FCSL::RootFolder> Derived() { return Udm::DerivedAttr< ::FCSL::RootFolder>(impl);}
			template <class Pred> Udm::DerivedAttr< ::FCSL::RootFolder, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::FCSL::RootFolder, Pred>(impl);}

			Udm::ArchetypeAttr< ::FCSL::RootFolder> Archetype() const { return Udm::ArchetypeAttr< ::FCSL::RootFolder>(impl);}

			static ::Uml::Attribute meta_name;
			Udm::StringAttr name() const { return Udm::StringAttr(impl, meta_name); }

			static ::Uml::CompositionChildRole meta_ComponentModels_children;
			Udm::ChildrenAttr< ::FCSL::ComponentModels> ComponentModels_children() const { return Udm::ChildrenAttr< ::FCSL::ComponentModels>(impl, meta_ComponentModels_children); }
			template <class Pred> Udm::ChildrenAttr< ::FCSL::ComponentModels, Pred> ComponentModels_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::ComponentModels, Pred>(impl, meta_ComponentModels_children); }

			static ::Uml::CompositionChildRole meta_HardwareModels_children;
			Udm::ChildrenAttr< ::FCSL::HardwareModels> HardwareModels_children() const { return Udm::ChildrenAttr< ::FCSL::HardwareModels>(impl, meta_HardwareModels_children); }
			template <class Pred> Udm::ChildrenAttr< ::FCSL::HardwareModels, Pred> HardwareModels_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::HardwareModels, Pred>(impl, meta_HardwareModels_children); }

			static ::Uml::CompositionChildRole meta_FM_TestModels_children;
			Udm::ChildrenAttr< ::FCSL::FM::TestModels> FM_TestModels_children() const { return Udm::ChildrenAttr< ::FCSL::FM::TestModels>(impl, meta_FM_TestModels_children); }
			template <class Pred> Udm::ChildrenAttr< ::FCSL::FM::TestModels, Pred> FM_TestModels_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::FM::TestModels, Pred>(impl, meta_FM_TestModels_children); }

			static ::Uml::CompositionChildRole meta_FM_PlatformModels_children;
			Udm::ChildrenAttr< ::FCSL::FM::PlatformModels> FM_PlatformModels_children() const { return Udm::ChildrenAttr< ::FCSL::FM::PlatformModels>(impl, meta_FM_PlatformModels_children); }
			template <class Pred> Udm::ChildrenAttr< ::FCSL::FM::PlatformModels, Pred> FM_PlatformModels_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::FM::PlatformModels, Pred>(impl, meta_FM_PlatformModels_children); }

			static ::Uml::CompositionChildRole meta_FM_UnitModels_children;
			Udm::ChildrenAttr< ::FCSL::FM::UnitModels> FM_UnitModels_children() const { return Udm::ChildrenAttr< ::FCSL::FM::UnitModels>(impl, meta_FM_UnitModels_children); }
			template <class Pred> Udm::ChildrenAttr< ::FCSL::FM::UnitModels, Pred> FM_UnitModels_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::FM::UnitModels, Pred>(impl, meta_FM_UnitModels_children); }

			static ::Uml::CompositionChildRole meta_SLSF_Dataflow_children;
			Udm::ChildrenAttr< ::FCSL::SLSF::Dataflow> SLSF_Dataflow_children() const { return Udm::ChildrenAttr< ::FCSL::SLSF::Dataflow>(impl, meta_SLSF_Dataflow_children); }
			template <class Pred> Udm::ChildrenAttr< ::FCSL::SLSF::Dataflow, Pred> SLSF_Dataflow_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::SLSF::Dataflow, Pred>(impl, meta_SLSF_Dataflow_children); }

			static ::Uml::CompositionChildRole meta_SLSF_Types_children;
			Udm::ChildrenAttr< ::FCSL::SLSF::Types> SLSF_Types_children() const { return Udm::ChildrenAttr< ::FCSL::SLSF::Types>(impl, meta_SLSF_Types_children); }
			template <class Pred> Udm::ChildrenAttr< ::FCSL::SLSF::Types, Pred> SLSF_Types_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::SLSF::Types, Pred>(impl, meta_SLSF_Types_children); }

			static ::Uml::CompositionChildRole meta_SLSF_Stateflow_children;
			Udm::ChildrenAttr< ::FCSL::SLSF::Stateflow> SLSF_Stateflow_children() const { return Udm::ChildrenAttr< ::FCSL::SLSF::Stateflow>(impl, meta_SLSF_Stateflow_children); }
			template <class Pred> Udm::ChildrenAttr< ::FCSL::SLSF::Stateflow, Pred> SLSF_Stateflow_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::SLSF::Stateflow, Pred>(impl, meta_SLSF_Stateflow_children); }

			static ::Uml::CompositionChildRole meta_RootFolder_children;
			Udm::ChildrenAttr< ::FCSL::RootFolder> RootFolder_children() const { return Udm::ChildrenAttr< ::FCSL::RootFolder>(impl, meta_RootFolder_children); }
			template <class Pred> Udm::ChildrenAttr< ::FCSL::RootFolder, Pred> RootFolder_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::RootFolder, Pred>(impl, meta_RootFolder_children); }

			Udm::ChildrenAttr< ::FCSL::HardwareModels> HardwareModels_kind_children() const { return Udm::ChildrenAttr< ::FCSL::HardwareModels>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::HardwareModels, Pred> HardwareModels_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::HardwareModels, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::ComponentModels> ComponentModels_kind_children() const { return Udm::ChildrenAttr< ::FCSL::ComponentModels>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::ComponentModels, Pred> ComponentModels_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::ComponentModels, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::RootFolder> RootFolder_kind_children() const { return Udm::ChildrenAttr< ::FCSL::RootFolder>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::RootFolder, Pred> RootFolder_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::RootFolder, Pred>(impl, Udm::NULLCHILDROLE); }

			static ::Uml::CompositionParentRole meta_RootFolder_parent;
			Udm::ParentAttr< ::FCSL::RootFolder> RootFolder_parent() const { return Udm::ParentAttr< ::FCSL::RootFolder>(impl, meta_RootFolder_parent); }

			Udm::ParentAttr< ::FCSL::RootFolder> parent() const { return Udm::ParentAttr< ::FCSL::RootFolder>(impl, Udm::NULLPARENTROLE); }
		};

		class  MgaObject :  virtual public Udm::Object {
		public:
			static ::Uml::Class meta;

			MgaObject() { }
			MgaObject(Udm::ObjectImpl *impl) : UDM_OBJECT(impl) { }
			MgaObject(const MgaObject &master) : UDM_OBJECT(master) { }
			static MgaObject Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static MgaObject Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			MgaObject CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			MgaObject CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::FCSL::MgaObject> Instances() { return Udm::InstantiatedAttr< ::FCSL::MgaObject>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::FCSL::MgaObject, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::FCSL::MgaObject, Pred>(impl);}

			Udm::DerivedAttr< ::FCSL::MgaObject> Derived() { return Udm::DerivedAttr< ::FCSL::MgaObject>(impl);}
			template <class Pred> Udm::DerivedAttr< ::FCSL::MgaObject, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::FCSL::MgaObject, Pred>(impl);}

			Udm::ArchetypeAttr< ::FCSL::MgaObject> Archetype() const { return Udm::ArchetypeAttr< ::FCSL::MgaObject>(impl);}

			static ::Uml::Attribute meta_position;
			Udm::StringAttr position() const { return Udm::StringAttr(impl, meta_position); }

			static ::Uml::Attribute meta_name;
			Udm::StringAttr name() const { return Udm::StringAttr(impl, meta_name); }

			Udm::ParentAttr<Udm::Object> parent() const { return Udm::ParentAttr<Udm::Object>(impl, Udm::NULLPARENTROLE); }
		};

		class  ScheduleItem :  public ::FCSL::MgaObject {
		public:
			static ::Uml::Class meta;

			ScheduleItem() { }
			ScheduleItem(Udm::ObjectImpl *impl) : ::FCSL::MgaObject(impl), UDM_OBJECT(impl) { }
			ScheduleItem(const ScheduleItem &master) : ::FCSL::MgaObject(master), UDM_OBJECT(master) { }
			static ScheduleItem Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static ScheduleItem Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			ScheduleItem CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			ScheduleItem CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::FCSL::ScheduleItem> Instances() { return Udm::InstantiatedAttr< ::FCSL::ScheduleItem>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::FCSL::ScheduleItem, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::FCSL::ScheduleItem, Pred>(impl);}

			Udm::DerivedAttr< ::FCSL::ScheduleItem> Derived() { return Udm::DerivedAttr< ::FCSL::ScheduleItem>(impl);}
			template <class Pred> Udm::DerivedAttr< ::FCSL::ScheduleItem, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::FCSL::ScheduleItem, Pred>(impl);}

			Udm::ArchetypeAttr< ::FCSL::ScheduleItem> Archetype() const { return Udm::ArchetypeAttr< ::FCSL::ScheduleItem>(impl);}

			static ::Uml::Attribute meta_Duration;
			Udm::IntegerAttr Duration() const { return Udm::IntegerAttr(impl, meta_Duration); }

			static ::Uml::Attribute meta_activationDelay;
			Udm::IntegerAttr activationDelay() const { return Udm::IntegerAttr(impl, meta_activationDelay); }

			static ::Uml::Attribute meta_activationPeriod;
			Udm::IntegerAttr activationPeriod() const { return Udm::IntegerAttr(impl, meta_activationPeriod); }

			static ::Uml::AssociationRole meta_ref;
			Udm::PointerAttr< ::FCSL::Partition> ref() const { return Udm::PointerAttr< ::FCSL::Partition>(impl, meta_ref); }

			static ::Uml::AssociationRole meta_setOperatingMode;
			Udm::AssocAttr< ::FCSL::OperatingMode> setOperatingMode() const { return Udm::AssocAttr< ::FCSL::OperatingMode>(impl, meta_setOperatingMode); }
			template <class Pred> Udm::AssocAttr< ::FCSL::OperatingMode, Pred > setOperatingMode_sorted(const Pred &) const { return Udm::AssocAttr< ::FCSL::OperatingMode, Pred>(impl, meta_setOperatingMode); }

			static ::Uml::CompositionParentRole meta_ProcessingModule_parent;
			Udm::ParentAttr< ::FCSL::ProcessingModule> ProcessingModule_parent() const { return Udm::ParentAttr< ::FCSL::ProcessingModule>(impl, meta_ProcessingModule_parent); }

			Udm::ParentAttr< ::FCSL::MgaObject> parent() const { return Udm::ParentAttr< ::FCSL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  Partition :  public ::FCSL::MgaObject {
		public:
			static ::Uml::Class meta;

			Partition() { }
			Partition(Udm::ObjectImpl *impl) : ::FCSL::MgaObject(impl), UDM_OBJECT(impl) { }
			Partition(const Partition &master) : ::FCSL::MgaObject(master), UDM_OBJECT(master) { }
			static Partition Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static Partition Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			Partition CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			Partition CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::FCSL::Partition> Instances() { return Udm::InstantiatedAttr< ::FCSL::Partition>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::FCSL::Partition, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::FCSL::Partition, Pred>(impl);}

			Udm::DerivedAttr< ::FCSL::Partition> Derived() { return Udm::DerivedAttr< ::FCSL::Partition>(impl);}
			template <class Pred> Udm::DerivedAttr< ::FCSL::Partition, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::FCSL::Partition, Pred>(impl);}

			Udm::ArchetypeAttr< ::FCSL::Partition> Archetype() const { return Udm::ArchetypeAttr< ::FCSL::Partition>(impl);}

			static ::Uml::Attribute meta_ID;
			Udm::StringAttr ID() const { return Udm::StringAttr(impl, meta_ID); }

			static ::Uml::Attribute meta_Name;
			Udm::StringAttr Name() const { return Udm::StringAttr(impl, meta_Name); }

			static ::Uml::Attribute meta_PartitionMemory;
			Udm::IntegerAttr PartitionMemory() const { return Udm::IntegerAttr(impl, meta_PartitionMemory); }

			static ::Uml::Attribute meta_SystemPartition;
			Udm::BooleanAttr SystemPartition() const { return Udm::BooleanAttr(impl, meta_SystemPartition); }

			static ::Uml::Attribute meta_Criticality;
			Udm::StringAttr Criticality() const { return Udm::StringAttr(impl, meta_Criticality); }

			static ::Uml::Attribute meta_startupOrder;
			Udm::IntegerAttr startupOrder() const { return Udm::IntegerAttr(impl, meta_startupOrder); }

			static ::Uml::AssociationRole meta_referedbyScheduleItem;
			Udm::AssocAttr< ::FCSL::ScheduleItem> referedbyScheduleItem() const { return Udm::AssocAttr< ::FCSL::ScheduleItem>(impl, meta_referedbyScheduleItem); }
			template <class Pred> Udm::AssocAttr< ::FCSL::ScheduleItem, Pred > referedbyScheduleItem_sorted(const Pred &) const { return Udm::AssocAttr< ::FCSL::ScheduleItem, Pred>(impl, meta_referedbyScheduleItem); }

			static ::Uml::AssociationRole meta_members;
			Udm::AssocAttr< ::FCSL::Process> members() const { return Udm::AssocAttr< ::FCSL::Process>(impl, meta_members); }
			template <class Pred> Udm::AssocAttr< ::FCSL::Process, Pred > members_sorted(const Pred &) const { return Udm::AssocAttr< ::FCSL::Process, Pred>(impl, meta_members); }

			static ::Uml::CompositionParentRole meta_ProcessingModule_parent;
			Udm::ParentAttr< ::FCSL::ProcessingModule> ProcessingModule_parent() const { return Udm::ParentAttr< ::FCSL::ProcessingModule>(impl, meta_ProcessingModule_parent); }

			Udm::ParentAttr< ::FCSL::MgaObject> parent() const { return Udm::ParentAttr< ::FCSL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  CommMapping :  public ::FCSL::MgaObject {
		public:
			static ::Uml::Class meta;

			CommMapping() { }
			CommMapping(Udm::ObjectImpl *impl) : ::FCSL::MgaObject(impl), UDM_OBJECT(impl) { }
			CommMapping(const CommMapping &master) : ::FCSL::MgaObject(master), UDM_OBJECT(master) { }
			static CommMapping Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static CommMapping Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			CommMapping CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			CommMapping CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::FCSL::CommMapping> Instances() { return Udm::InstantiatedAttr< ::FCSL::CommMapping>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::FCSL::CommMapping, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::FCSL::CommMapping, Pred>(impl);}

			Udm::DerivedAttr< ::FCSL::CommMapping> Derived() { return Udm::DerivedAttr< ::FCSL::CommMapping>(impl);}
			template <class Pred> Udm::DerivedAttr< ::FCSL::CommMapping, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::FCSL::CommMapping, Pred>(impl);}

			Udm::ArchetypeAttr< ::FCSL::CommMapping> Archetype() const { return Udm::ArchetypeAttr< ::FCSL::CommMapping>(impl);}

			static ::Uml::CompositionParentRole meta_ProcessingModule_parent;
			Udm::ParentAttr< ::FCSL::ProcessingModule> ProcessingModule_parent() const { return Udm::ParentAttr< ::FCSL::ProcessingModule>(impl, meta_ProcessingModule_parent); }

			Udm::ParentAttr< ::FCSL::MgaObject> parent() const { return Udm::ParentAttr< ::FCSL::MgaObject>(impl, Udm::NULLPARENTROLE); }
			static ::Uml::AssociationRole meta_dstCommMapping_end_;
			Udm::AssocEndAttr< ::FCSL::CommMapping_Members_Base> dstCommMapping_end() const { return Udm::AssocEndAttr< ::FCSL::CommMapping_Members_Base>(impl, meta_dstCommMapping_end_); }

			static ::Uml::AssociationRole meta_srcCommMapping_end_;
			Udm::AssocEndAttr< ::FCSL::CommMapping_Members_Base> srcCommMapping_end() const { return Udm::AssocEndAttr< ::FCSL::CommMapping_Members_Base>(impl, meta_srcCommMapping_end_); }

		};

		class  Process :  public ::FCSL::MgaObject {
		public:
			static ::Uml::Class meta;

			Process() { }
			Process(Udm::ObjectImpl *impl) : ::FCSL::MgaObject(impl), UDM_OBJECT(impl) { }
			Process(const Process &master) : ::FCSL::MgaObject(master), UDM_OBJECT(master) { }
			static Process Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static Process Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			Process CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			Process CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::FCSL::Process> Instances() { return Udm::InstantiatedAttr< ::FCSL::Process>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::FCSL::Process, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::FCSL::Process, Pred>(impl);}

			Udm::DerivedAttr< ::FCSL::Process> Derived() { return Udm::DerivedAttr< ::FCSL::Process>(impl);}
			template <class Pred> Udm::DerivedAttr< ::FCSL::Process, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::FCSL::Process, Pred>(impl);}

			Udm::ArchetypeAttr< ::FCSL::Process> Archetype() const { return Udm::ArchetypeAttr< ::FCSL::Process>(impl);}

			static ::Uml::Attribute meta_TimeCapacity;
			Udm::IntegerAttr TimeCapacity() const { return Udm::IntegerAttr(impl, meta_TimeCapacity); }

			static ::Uml::Attribute meta_Name;
			Udm::StringAttr Name() const { return Udm::StringAttr(impl, meta_Name); }

			static ::Uml::Attribute meta_kindOfDeadline;
			Udm::StringAttr kindOfDeadline() const { return Udm::StringAttr(impl, meta_kindOfDeadline); }

			static ::Uml::Attribute meta_stackSize;
			Udm::IntegerAttr stackSize() const { return Udm::IntegerAttr(impl, meta_stackSize); }

			static ::Uml::Attribute meta_startOffset;
			Udm::IntegerAttr startOffset() const { return Udm::IntegerAttr(impl, meta_startOffset); }

			static ::Uml::Attribute meta_basePriority;
			Udm::IntegerAttr basePriority() const { return Udm::IntegerAttr(impl, meta_basePriority); }

			static ::Uml::Attribute meta_activationPeriod;
			Udm::IntegerAttr activationPeriod() const { return Udm::IntegerAttr(impl, meta_activationPeriod); }

			static ::Uml::Attribute meta_isAperiodic;
			Udm::BooleanAttr isAperiodic() const { return Udm::BooleanAttr(impl, meta_isAperiodic); }

			static ::Uml::Attribute meta_entryPoint;
			Udm::StringAttr entryPoint() const { return Udm::StringAttr(impl, meta_entryPoint); }

			static ::Uml::Attribute meta_infiniteTimeCapacity;
			Udm::BooleanAttr infiniteTimeCapacity() const { return Udm::BooleanAttr(impl, meta_infiniteTimeCapacity); }

			static ::Uml::Attribute meta_isHealthMonitor;
			Udm::BooleanAttr isHealthMonitor() const { return Udm::BooleanAttr(impl, meta_isHealthMonitor); }

			static ::Uml::AssociationRole meta_setPartition;
			Udm::AssocAttr< ::FCSL::Partition> setPartition() const { return Udm::AssocAttr< ::FCSL::Partition>(impl, meta_setPartition); }
			template <class Pred> Udm::AssocAttr< ::FCSL::Partition, Pred > setPartition_sorted(const Pred &) const { return Udm::AssocAttr< ::FCSL::Partition, Pred>(impl, meta_setPartition); }

			static ::Uml::AssociationRole meta_ref;
			Udm::PointerAttr< ::FCSL::Component> ref() const { return Udm::PointerAttr< ::FCSL::Component>(impl, meta_ref); }

			static ::Uml::CompositionParentRole meta_ProcessingModule_parent;
			Udm::ParentAttr< ::FCSL::ProcessingModule> ProcessingModule_parent() const { return Udm::ParentAttr< ::FCSL::ProcessingModule>(impl, meta_ProcessingModule_parent); }

			Udm::ParentAttr< ::FCSL::MgaObject> parent() const { return Udm::ParentAttr< ::FCSL::MgaObject>(impl, Udm::NULLPARENTROLE); }
			static ::Uml::Constraint meta_ProcessAssignedtoExactlyOnePartition;
		};

		class  OperatingMode :  public ::FCSL::MgaObject {
		public:
			static ::Uml::Class meta;

			OperatingMode() { }
			OperatingMode(Udm::ObjectImpl *impl) : ::FCSL::MgaObject(impl), UDM_OBJECT(impl) { }
			OperatingMode(const OperatingMode &master) : ::FCSL::MgaObject(master), UDM_OBJECT(master) { }
			static OperatingMode Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static OperatingMode Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			OperatingMode CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			OperatingMode CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::FCSL::OperatingMode> Instances() { return Udm::InstantiatedAttr< ::FCSL::OperatingMode>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::FCSL::OperatingMode, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::FCSL::OperatingMode, Pred>(impl);}

			Udm::DerivedAttr< ::FCSL::OperatingMode> Derived() { return Udm::DerivedAttr< ::FCSL::OperatingMode>(impl);}
			template <class Pred> Udm::DerivedAttr< ::FCSL::OperatingMode, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::FCSL::OperatingMode, Pred>(impl);}

			Udm::ArchetypeAttr< ::FCSL::OperatingMode> Archetype() const { return Udm::ArchetypeAttr< ::FCSL::OperatingMode>(impl);}

			static ::Uml::Attribute meta_Name;
			Udm::StringAttr Name() const { return Udm::StringAttr(impl, meta_Name); }

			static ::Uml::AssociationRole meta_members;
			Udm::AssocAttr< ::FCSL::ScheduleItem> members() const { return Udm::AssocAttr< ::FCSL::ScheduleItem>(impl, meta_members); }
			template <class Pred> Udm::AssocAttr< ::FCSL::ScheduleItem, Pred > members_sorted(const Pred &) const { return Udm::AssocAttr< ::FCSL::ScheduleItem, Pred>(impl, meta_members); }

			static ::Uml::CompositionParentRole meta_ProcessingModule_parent;
			Udm::ParentAttr< ::FCSL::ProcessingModule> ProcessingModule_parent() const { return Udm::ParentAttr< ::FCSL::ProcessingModule>(impl, meta_ProcessingModule_parent); }

			Udm::ParentAttr< ::FCSL::MgaObject> parent() const { return Udm::ParentAttr< ::FCSL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  DevicePort :  public ::FCSL::CommMapping_Members_Base, virtual public ::FCSL::MgaObject {
		public:
			static ::Uml::Class meta;

			DevicePort() { }
			DevicePort(Udm::ObjectImpl *impl) : ::FCSL::CommMapping_Members_Base(impl),::FCSL::MgaObject(impl), UDM_OBJECT(impl) { }
			DevicePort(const DevicePort &master) : ::FCSL::CommMapping_Members_Base(master),::FCSL::MgaObject(master), UDM_OBJECT(master) { }
			static DevicePort Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static DevicePort Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			DevicePort CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			DevicePort CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::FCSL::DevicePort> Instances() { return Udm::InstantiatedAttr< ::FCSL::DevicePort>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::FCSL::DevicePort, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::FCSL::DevicePort, Pred>(impl);}

			Udm::DerivedAttr< ::FCSL::DevicePort> Derived() { return Udm::DerivedAttr< ::FCSL::DevicePort>(impl);}
			template <class Pred> Udm::DerivedAttr< ::FCSL::DevicePort, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::FCSL::DevicePort, Pred>(impl);}

			Udm::ArchetypeAttr< ::FCSL::DevicePort> Archetype() const { return Udm::ArchetypeAttr< ::FCSL::DevicePort>(impl);}

			static ::Uml::Attribute meta_PhysicalAddress;
			Udm::StringAttr PhysicalAddress() const { return Udm::StringAttr(impl, meta_PhysicalAddress); }

			static ::Uml::Attribute meta_Procedure;
			Udm::StringAttr Procedure() const { return Udm::StringAttr(impl, meta_Procedure); }

			static ::Uml::CompositionParentRole meta_ProcessingModule_parent;
			Udm::ParentAttr< ::FCSL::ProcessingModule> ProcessingModule_parent() const { return Udm::ParentAttr< ::FCSL::ProcessingModule>(impl, meta_ProcessingModule_parent); }

			Udm::ParentAttr< ::FCSL::MgaObject> parent() const { return Udm::ParentAttr< ::FCSL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  HWElement :  virtual public ::FCSL::MgaObject {
		public:
			static ::Uml::Class meta;

			HWElement() { }
			HWElement(Udm::ObjectImpl *impl) : ::FCSL::MgaObject(impl), UDM_OBJECT(impl) { }
			HWElement(const HWElement &master) : ::FCSL::MgaObject(master), UDM_OBJECT(master) { }
			static HWElement Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static HWElement Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			HWElement CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			HWElement CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::FCSL::HWElement> Instances() { return Udm::InstantiatedAttr< ::FCSL::HWElement>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::FCSL::HWElement, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::FCSL::HWElement, Pred>(impl);}

			Udm::DerivedAttr< ::FCSL::HWElement> Derived() { return Udm::DerivedAttr< ::FCSL::HWElement>(impl);}
			template <class Pred> Udm::DerivedAttr< ::FCSL::HWElement, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::FCSL::HWElement, Pred>(impl);}

			Udm::ArchetypeAttr< ::FCSL::HWElement> Archetype() const { return Udm::ArchetypeAttr< ::FCSL::HWElement>(impl);}

			static ::Uml::CompositionParentRole meta_HardwareSheet_parent;
			Udm::ParentAttr< ::FCSL::HardwareSheet> HardwareSheet_parent() const { return Udm::ParentAttr< ::FCSL::HardwareSheet>(impl, meta_HardwareSheet_parent); }

			Udm::ParentAttr< ::FCSL::MgaObject> parent() const { return Udm::ParentAttr< ::FCSL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  ProcessingModule :  public ::FCSL::HWElement {
		public:
			static ::Uml::Class meta;

			ProcessingModule() { }
			ProcessingModule(Udm::ObjectImpl *impl) : ::FCSL::HWElement(impl), ::FCSL::MgaObject(impl), UDM_OBJECT(impl) { }
			ProcessingModule(const ProcessingModule &master) : ::FCSL::HWElement(master), ::FCSL::MgaObject(master), UDM_OBJECT(master) { }
			static ProcessingModule Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static ProcessingModule Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			ProcessingModule CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			ProcessingModule CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::FCSL::ProcessingModule> Instances() { return Udm::InstantiatedAttr< ::FCSL::ProcessingModule>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::FCSL::ProcessingModule, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::FCSL::ProcessingModule, Pred>(impl);}

			Udm::DerivedAttr< ::FCSL::ProcessingModule> Derived() { return Udm::DerivedAttr< ::FCSL::ProcessingModule>(impl);}
			template <class Pred> Udm::DerivedAttr< ::FCSL::ProcessingModule, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::FCSL::ProcessingModule, Pred>(impl);}

			Udm::ArchetypeAttr< ::FCSL::ProcessingModule> Archetype() const { return Udm::ArchetypeAttr< ::FCSL::ProcessingModule>(impl);}

			static ::Uml::Attribute meta_SPECint_rate2000;
			Udm::IntegerAttr SPECint_rate2000() const { return Udm::IntegerAttr(impl, meta_SPECint_rate2000); }

			static ::Uml::Attribute meta_Name;
			Udm::StringAttr Name() const { return Udm::StringAttr(impl, meta_Name); }

			static ::Uml::Attribute meta_CPUType;
			Udm::StringAttr CPUType() const { return Udm::StringAttr(impl, meta_CPUType); }

			static ::Uml::Attribute meta_Cores;
			Udm::IntegerAttr Cores() const { return Udm::IntegerAttr(impl, meta_Cores); }

			static ::Uml::Attribute meta_SPECfp_rate2000;
			Udm::IntegerAttr SPECfp_rate2000() const { return Udm::IntegerAttr(impl, meta_SPECfp_rate2000); }

			static ::Uml::Attribute meta_Memory;
			Udm::IntegerAttr Memory() const { return Udm::IntegerAttr(impl, meta_Memory); }

			static ::Uml::CompositionChildRole meta_Process_children;
			Udm::ChildrenAttr< ::FCSL::Process> Process_children() const { return Udm::ChildrenAttr< ::FCSL::Process>(impl, meta_Process_children); }
			template <class Pred> Udm::ChildrenAttr< ::FCSL::Process, Pred> Process_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::Process, Pred>(impl, meta_Process_children); }

			static ::Uml::CompositionChildRole meta_DevicePort_children;
			Udm::ChildrenAttr< ::FCSL::DevicePort> DevicePort_children() const { return Udm::ChildrenAttr< ::FCSL::DevicePort>(impl, meta_DevicePort_children); }
			template <class Pred> Udm::ChildrenAttr< ::FCSL::DevicePort, Pred> DevicePort_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::DevicePort, Pred>(impl, meta_DevicePort_children); }

			static ::Uml::CompositionChildRole meta_OperatingMode_children;
			Udm::ChildrenAttr< ::FCSL::OperatingMode> OperatingMode_children() const { return Udm::ChildrenAttr< ::FCSL::OperatingMode>(impl, meta_OperatingMode_children); }
			template <class Pred> Udm::ChildrenAttr< ::FCSL::OperatingMode, Pred> OperatingMode_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::OperatingMode, Pred>(impl, meta_OperatingMode_children); }

			static ::Uml::CompositionChildRole meta_ScheduleItem_children;
			Udm::ChildrenAttr< ::FCSL::ScheduleItem> ScheduleItem_children() const { return Udm::ChildrenAttr< ::FCSL::ScheduleItem>(impl, meta_ScheduleItem_children); }
			template <class Pred> Udm::ChildrenAttr< ::FCSL::ScheduleItem, Pred> ScheduleItem_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::ScheduleItem, Pred>(impl, meta_ScheduleItem_children); }

			static ::Uml::CompositionChildRole meta_CommMapping_children;
			Udm::ChildrenAttr< ::FCSL::CommMapping> CommMapping_children() const { return Udm::ChildrenAttr< ::FCSL::CommMapping>(impl, meta_CommMapping_children); }
			template <class Pred> Udm::ChildrenAttr< ::FCSL::CommMapping, Pred> CommMapping_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::CommMapping, Pred>(impl, meta_CommMapping_children); }

			static ::Uml::CompositionChildRole meta_Partition_children;
			Udm::ChildrenAttr< ::FCSL::Partition> Partition_children() const { return Udm::ChildrenAttr< ::FCSL::Partition>(impl, meta_Partition_children); }
			template <class Pred> Udm::ChildrenAttr< ::FCSL::Partition, Pred> Partition_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::Partition, Pred>(impl, meta_Partition_children); }

			Udm::ChildrenAttr< ::FCSL::CommMapping_Members_Base> CommMapping_Members_Base_kind_children() const { return Udm::ChildrenAttr< ::FCSL::CommMapping_Members_Base>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::CommMapping_Members_Base, Pred> CommMapping_Members_Base_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::CommMapping_Members_Base, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::ScheduleItem> ScheduleItem_kind_children() const { return Udm::ChildrenAttr< ::FCSL::ScheduleItem>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::ScheduleItem, Pred> ScheduleItem_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::ScheduleItem, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::Partition> Partition_kind_children() const { return Udm::ChildrenAttr< ::FCSL::Partition>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::Partition, Pred> Partition_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::Partition, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::CommMapping> CommMapping_kind_children() const { return Udm::ChildrenAttr< ::FCSL::CommMapping>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::CommMapping, Pred> CommMapping_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::CommMapping, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::Process> Process_kind_children() const { return Udm::ChildrenAttr< ::FCSL::Process>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::Process, Pred> Process_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::Process, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::OperatingMode> OperatingMode_kind_children() const { return Udm::ChildrenAttr< ::FCSL::OperatingMode>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::OperatingMode, Pred> OperatingMode_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::OperatingMode, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::DevicePort> DevicePort_kind_children() const { return Udm::ChildrenAttr< ::FCSL::DevicePort>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::DevicePort, Pred> DevicePort_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::DevicePort, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::BusPort> BusPort_kind_children() const { return Udm::ChildrenAttr< ::FCSL::BusPort>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::BusPort, Pred> BusPort_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::BusPort, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr< ::FCSL::MgaObject>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ParentAttr< ::FCSL::MgaObject> parent() const { return Udm::ParentAttr< ::FCSL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  Wire :  public ::FCSL::HWElement {
		public:
			static ::Uml::Class meta;

			Wire() { }
			Wire(Udm::ObjectImpl *impl) : ::FCSL::HWElement(impl), ::FCSL::MgaObject(impl), UDM_OBJECT(impl) { }
			Wire(const Wire &master) : ::FCSL::HWElement(master), ::FCSL::MgaObject(master), UDM_OBJECT(master) { }
			static Wire Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static Wire Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			Wire CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			Wire CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::FCSL::Wire> Instances() { return Udm::InstantiatedAttr< ::FCSL::Wire>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::FCSL::Wire, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::FCSL::Wire, Pred>(impl);}

			Udm::DerivedAttr< ::FCSL::Wire> Derived() { return Udm::DerivedAttr< ::FCSL::Wire>(impl);}
			template <class Pred> Udm::DerivedAttr< ::FCSL::Wire, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::FCSL::Wire, Pred>(impl);}

			Udm::ArchetypeAttr< ::FCSL::Wire> Archetype() const { return Udm::ArchetypeAttr< ::FCSL::Wire>(impl);}

			Udm::ParentAttr< ::FCSL::MgaObject> parent() const { return Udm::ParentAttr< ::FCSL::MgaObject>(impl, Udm::NULLPARENTROLE); }
			static ::Uml::AssociationRole meta_dstWire_end_;
			Udm::AssocEndAttr< ::FCSL::CommElement> dstWire_end() const { return Udm::AssocEndAttr< ::FCSL::CommElement>(impl, meta_dstWire_end_); }

			static ::Uml::AssociationRole meta_srcWire_end_;
			Udm::AssocEndAttr< ::FCSL::CommElement> srcWire_end() const { return Udm::AssocEndAttr< ::FCSL::CommElement>(impl, meta_srcWire_end_); }

		};

		class  CommElement :  virtual public ::FCSL::MgaObject {
		public:
			static ::Uml::Class meta;

			CommElement() { }
			CommElement(Udm::ObjectImpl *impl) : ::FCSL::MgaObject(impl), UDM_OBJECT(impl) { }
			CommElement(const CommElement &master) : ::FCSL::MgaObject(master), UDM_OBJECT(master) { }
			static CommElement Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static CommElement Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			CommElement CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			CommElement CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::FCSL::CommElement> Instances() { return Udm::InstantiatedAttr< ::FCSL::CommElement>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::FCSL::CommElement, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::FCSL::CommElement, Pred>(impl);}

			Udm::DerivedAttr< ::FCSL::CommElement> Derived() { return Udm::DerivedAttr< ::FCSL::CommElement>(impl);}
			template <class Pred> Udm::DerivedAttr< ::FCSL::CommElement, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::FCSL::CommElement, Pred>(impl);}

			Udm::ArchetypeAttr< ::FCSL::CommElement> Archetype() const { return Udm::ArchetypeAttr< ::FCSL::CommElement>(impl);}

			static ::Uml::AssociationRole meta_srcWire;
			static ::Uml::AssociationRole meta_srcWire_rev;
			Udm::AClassAssocAttr< ::FCSL::Wire, ::FCSL::CommElement> srcWire() const { return Udm::AClassAssocAttr< ::FCSL::Wire, ::FCSL::CommElement>(impl, meta_srcWire, meta_srcWire_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::FCSL::Wire, ::FCSL::CommElement, Pred> srcWire_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::FCSL::Wire, ::FCSL::CommElement, Pred>(impl, meta_srcWire, meta_srcWire_rev); }

			static ::Uml::AssociationRole meta_dstWire;
			static ::Uml::AssociationRole meta_dstWire_rev;
			Udm::AClassAssocAttr< ::FCSL::Wire, ::FCSL::CommElement> dstWire() const { return Udm::AClassAssocAttr< ::FCSL::Wire, ::FCSL::CommElement>(impl, meta_dstWire, meta_dstWire_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::FCSL::Wire, ::FCSL::CommElement, Pred> dstWire_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::FCSL::Wire, ::FCSL::CommElement, Pred>(impl, meta_dstWire, meta_dstWire_rev); }

			Udm::ParentAttr<Udm::Object> parent() const { return Udm::ParentAttr<Udm::Object>(impl, Udm::NULLPARENTROLE); }
		};

		class  Bus :  public ::FCSL::HWElement, public ::FCSL::CommElement {
		public:
			static ::Uml::Class meta;

			Bus() { }
			Bus(Udm::ObjectImpl *impl) : ::FCSL::HWElement(impl),::FCSL::CommElement(impl), ::FCSL::MgaObject(impl), UDM_OBJECT(impl) { }
			Bus(const Bus &master) : ::FCSL::HWElement(master),::FCSL::CommElement(master), ::FCSL::MgaObject(master), UDM_OBJECT(master) { }
			static Bus Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static Bus Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			Bus CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			Bus CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::FCSL::Bus> Instances() { return Udm::InstantiatedAttr< ::FCSL::Bus>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::FCSL::Bus, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::FCSL::Bus, Pred>(impl);}

			Udm::DerivedAttr< ::FCSL::Bus> Derived() { return Udm::DerivedAttr< ::FCSL::Bus>(impl);}
			template <class Pred> Udm::DerivedAttr< ::FCSL::Bus, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::FCSL::Bus, Pred>(impl);}

			Udm::ArchetypeAttr< ::FCSL::Bus> Archetype() const { return Udm::ArchetypeAttr< ::FCSL::Bus>(impl);}

			static ::Uml::Attribute meta_Medium;
			Udm::StringAttr Medium() const { return Udm::StringAttr(impl, meta_Medium); }

			static ::Uml::Attribute meta_Name;
			Udm::StringAttr Name() const { return Udm::StringAttr(impl, meta_Name); }

			static ::Uml::Attribute meta_FrameSize;
			Udm::IntegerAttr FrameSize() const { return Udm::IntegerAttr(impl, meta_FrameSize); }

			static ::Uml::Attribute meta_BusSpeed;
			Udm::IntegerAttr BusSpeed() const { return Udm::IntegerAttr(impl, meta_BusSpeed); }

			static ::Uml::Attribute meta_MajorCycle;
			Udm::StringAttr MajorCycle() const { return Udm::StringAttr(impl, meta_MajorCycle); }

			Udm::ParentAttr< ::FCSL::MgaObject> parent() const { return Udm::ParentAttr< ::FCSL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  BusPort :  public ::FCSL::DevicePort, public ::FCSL::CommElement {
		public:
			static ::Uml::Class meta;

			BusPort() { }
			BusPort(Udm::ObjectImpl *impl) : ::FCSL::DevicePort(impl),::FCSL::CommElement(impl), ::FCSL::MgaObject(impl), UDM_OBJECT(impl) { }
			BusPort(const BusPort &master) : ::FCSL::DevicePort(master),::FCSL::CommElement(master), ::FCSL::MgaObject(master), UDM_OBJECT(master) { }
			static BusPort Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static BusPort Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			BusPort CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			BusPort CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::FCSL::BusPort> Instances() { return Udm::InstantiatedAttr< ::FCSL::BusPort>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::FCSL::BusPort, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::FCSL::BusPort, Pred>(impl);}

			Udm::DerivedAttr< ::FCSL::BusPort> Derived() { return Udm::DerivedAttr< ::FCSL::BusPort>(impl);}
			template <class Pred> Udm::DerivedAttr< ::FCSL::BusPort, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::FCSL::BusPort, Pred>(impl);}

			Udm::ArchetypeAttr< ::FCSL::BusPort> Archetype() const { return Udm::ArchetypeAttr< ::FCSL::BusPort>(impl);}

			Udm::ParentAttr< ::FCSL::MgaObject> parent() const { return Udm::ParentAttr< ::FCSL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  HardwareSheet :  public ::FCSL::MgaObject {
		public:
			static ::Uml::Class meta;

			HardwareSheet() { }
			HardwareSheet(Udm::ObjectImpl *impl) : ::FCSL::MgaObject(impl), UDM_OBJECT(impl) { }
			HardwareSheet(const HardwareSheet &master) : ::FCSL::MgaObject(master), UDM_OBJECT(master) { }
			static HardwareSheet Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static HardwareSheet Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			HardwareSheet CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			HardwareSheet CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::FCSL::HardwareSheet> Instances() { return Udm::InstantiatedAttr< ::FCSL::HardwareSheet>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::FCSL::HardwareSheet, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::FCSL::HardwareSheet, Pred>(impl);}

			Udm::DerivedAttr< ::FCSL::HardwareSheet> Derived() { return Udm::DerivedAttr< ::FCSL::HardwareSheet>(impl);}
			template <class Pred> Udm::DerivedAttr< ::FCSL::HardwareSheet, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::FCSL::HardwareSheet, Pred>(impl);}

			Udm::ArchetypeAttr< ::FCSL::HardwareSheet> Archetype() const { return Udm::ArchetypeAttr< ::FCSL::HardwareSheet>(impl);}

			static ::Uml::CompositionChildRole meta_HWElement_children;
			Udm::ChildrenAttr< ::FCSL::HWElement> HWElement_children() const { return Udm::ChildrenAttr< ::FCSL::HWElement>(impl, meta_HWElement_children); }
			template <class Pred> Udm::ChildrenAttr< ::FCSL::HWElement, Pred> HWElement_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::HWElement, Pred>(impl, meta_HWElement_children); }

			Udm::ChildrenAttr< ::FCSL::ProcessingModule> ProcessingModule_kind_children() const { return Udm::ChildrenAttr< ::FCSL::ProcessingModule>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::ProcessingModule, Pred> ProcessingModule_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::ProcessingModule, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::Bus> Bus_kind_children() const { return Udm::ChildrenAttr< ::FCSL::Bus>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::Bus, Pred> Bus_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::Bus, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::HWElement> HWElement_kind_children() const { return Udm::ChildrenAttr< ::FCSL::HWElement>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::HWElement, Pred> HWElement_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::HWElement, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::Wire> Wire_kind_children() const { return Udm::ChildrenAttr< ::FCSL::Wire>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::Wire, Pred> Wire_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::Wire, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr< ::FCSL::MgaObject>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

			static ::Uml::CompositionParentRole meta_HardwareModels_parent;
			Udm::ParentAttr< ::FCSL::HardwareModels> HardwareModels_parent() const { return Udm::ParentAttr< ::FCSL::HardwareModels>(impl, meta_HardwareModels_parent); }

			Udm::ParentAttr< ::FCSL::HardwareModels> parent() const { return Udm::ParentAttr< ::FCSL::HardwareModels>(impl, Udm::NULLPARENTROLE); }
		};

		class  Port2Channel :  public ::FCSL::MgaObject {
		public:
			static ::Uml::Class meta;

			Port2Channel() { }
			Port2Channel(Udm::ObjectImpl *impl) : ::FCSL::MgaObject(impl), UDM_OBJECT(impl) { }
			Port2Channel(const Port2Channel &master) : ::FCSL::MgaObject(master), UDM_OBJECT(master) { }
			static Port2Channel Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static Port2Channel Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			Port2Channel CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			Port2Channel CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::FCSL::Port2Channel> Instances() { return Udm::InstantiatedAttr< ::FCSL::Port2Channel>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::FCSL::Port2Channel, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::FCSL::Port2Channel, Pred>(impl);}

			Udm::DerivedAttr< ::FCSL::Port2Channel> Derived() { return Udm::DerivedAttr< ::FCSL::Port2Channel>(impl);}
			template <class Pred> Udm::DerivedAttr< ::FCSL::Port2Channel, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::FCSL::Port2Channel, Pred>(impl);}

			Udm::ArchetypeAttr< ::FCSL::Port2Channel> Archetype() const { return Udm::ArchetypeAttr< ::FCSL::Port2Channel>(impl);}

			static ::Uml::CompositionParentRole meta_ComponentSheet_parent;
			Udm::ParentAttr< ::FCSL::ComponentSheet> ComponentSheet_parent() const { return Udm::ParentAttr< ::FCSL::ComponentSheet>(impl, meta_ComponentSheet_parent); }

			Udm::ParentAttr< ::FCSL::MgaObject> parent() const { return Udm::ParentAttr< ::FCSL::MgaObject>(impl, Udm::NULLPARENTROLE); }
			static ::Uml::AssociationRole meta_dstPort2Channel_end_;
			Udm::AssocEndAttr< ::FCSL::Channel> dstPort2Channel_end() const { return Udm::AssocEndAttr< ::FCSL::Channel>(impl, meta_dstPort2Channel_end_); }

			static ::Uml::AssociationRole meta_srcPort2Channel_end_;
			Udm::AssocEndAttr< ::FCSL::OutputPort> srcPort2Channel_end() const { return Udm::AssocEndAttr< ::FCSL::OutputPort>(impl, meta_srcPort2Channel_end_); }

		};

		class  Channel2Port :  public ::FCSL::MgaObject {
		public:
			static ::Uml::Class meta;

			Channel2Port() { }
			Channel2Port(Udm::ObjectImpl *impl) : ::FCSL::MgaObject(impl), UDM_OBJECT(impl) { }
			Channel2Port(const Channel2Port &master) : ::FCSL::MgaObject(master), UDM_OBJECT(master) { }
			static Channel2Port Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static Channel2Port Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			Channel2Port CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			Channel2Port CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::FCSL::Channel2Port> Instances() { return Udm::InstantiatedAttr< ::FCSL::Channel2Port>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::FCSL::Channel2Port, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::FCSL::Channel2Port, Pred>(impl);}

			Udm::DerivedAttr< ::FCSL::Channel2Port> Derived() { return Udm::DerivedAttr< ::FCSL::Channel2Port>(impl);}
			template <class Pred> Udm::DerivedAttr< ::FCSL::Channel2Port, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::FCSL::Channel2Port, Pred>(impl);}

			Udm::ArchetypeAttr< ::FCSL::Channel2Port> Archetype() const { return Udm::ArchetypeAttr< ::FCSL::Channel2Port>(impl);}

			static ::Uml::CompositionParentRole meta_ComponentSheet_parent;
			Udm::ParentAttr< ::FCSL::ComponentSheet> ComponentSheet_parent() const { return Udm::ParentAttr< ::FCSL::ComponentSheet>(impl, meta_ComponentSheet_parent); }

			Udm::ParentAttr< ::FCSL::MgaObject> parent() const { return Udm::ParentAttr< ::FCSL::MgaObject>(impl, Udm::NULLPARENTROLE); }
			static ::Uml::AssociationRole meta_srcChannel2Port_end_;
			Udm::AssocEndAttr< ::FCSL::Channel> srcChannel2Port_end() const { return Udm::AssocEndAttr< ::FCSL::Channel>(impl, meta_srcChannel2Port_end_); }

			static ::Uml::AssociationRole meta_dstChannel2Port_end_;
			Udm::AssocEndAttr< ::FCSL::InputPort> dstChannel2Port_end() const { return Udm::AssocEndAttr< ::FCSL::InputPort>(impl, meta_dstChannel2Port_end_); }

		};

		class  EventPort :  public ::FCSL::EventPortMapping_Members_Base, public ::FCSL::MgaObject {
		public:
			static ::Uml::Class meta;

			EventPort() { }
			EventPort(Udm::ObjectImpl *impl) : ::FCSL::EventPortMapping_Members_Base(impl),::FCSL::MgaObject(impl), UDM_OBJECT(impl) { }
			EventPort(const EventPort &master) : ::FCSL::EventPortMapping_Members_Base(master),::FCSL::MgaObject(master), UDM_OBJECT(master) { }
			static EventPort Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static EventPort Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			EventPort CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			EventPort CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::FCSL::EventPort> Instances() { return Udm::InstantiatedAttr< ::FCSL::EventPort>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::FCSL::EventPort, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::FCSL::EventPort, Pred>(impl);}

			Udm::DerivedAttr< ::FCSL::EventPort> Derived() { return Udm::DerivedAttr< ::FCSL::EventPort>(impl);}
			template <class Pred> Udm::DerivedAttr< ::FCSL::EventPort, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::FCSL::EventPort, Pred>(impl);}

			Udm::ArchetypeAttr< ::FCSL::EventPort> Archetype() const { return Udm::ArchetypeAttr< ::FCSL::EventPort>(impl);}

			static ::Uml::Attribute meta_Name;
			Udm::StringAttr Name() const { return Udm::StringAttr(impl, meta_Name); }

			static ::Uml::CompositionParentRole meta_Component_parent;
			Udm::ParentAttr< ::FCSL::Component> Component_parent() const { return Udm::ParentAttr< ::FCSL::Component>(impl, meta_Component_parent); }

			Udm::ParentAttr< ::FCSL::MgaObject> parent() const { return Udm::ParentAttr< ::FCSL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  Port :  public ::FCSL::CommMapping_Members_Base, public ::FCSL::PortMapping_Members_Base, public ::FCSL::MgaObject {
		public:
			static ::Uml::Class meta;

			Port() { }
			Port(Udm::ObjectImpl *impl) : ::FCSL::CommMapping_Members_Base(impl),::FCSL::PortMapping_Members_Base(impl),::FCSL::MgaObject(impl), UDM_OBJECT(impl) { }
			Port(const Port &master) : ::FCSL::CommMapping_Members_Base(master),::FCSL::PortMapping_Members_Base(master),::FCSL::MgaObject(master), UDM_OBJECT(master) { }
			static Port Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static Port Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			Port CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			Port CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::FCSL::Port> Instances() { return Udm::InstantiatedAttr< ::FCSL::Port>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::FCSL::Port, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::FCSL::Port, Pred>(impl);}

			Udm::DerivedAttr< ::FCSL::Port> Derived() { return Udm::DerivedAttr< ::FCSL::Port>(impl);}
			template <class Pred> Udm::DerivedAttr< ::FCSL::Port, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::FCSL::Port, Pred>(impl);}

			Udm::ArchetypeAttr< ::FCSL::Port> Archetype() const { return Udm::ArchetypeAttr< ::FCSL::Port>(impl);}

			static ::Uml::Attribute meta_MaxMessageSize;
			Udm::StringAttr MaxMessageSize() const { return Udm::StringAttr(impl, meta_MaxMessageSize); }

			static ::Uml::Attribute meta_Name;
			Udm::StringAttr Name() const { return Udm::StringAttr(impl, meta_Name); }

			static ::Uml::Attribute meta_RefreshRateSeconds;
			Udm::IntegerAttr RefreshRateSeconds() const { return Udm::IntegerAttr(impl, meta_RefreshRateSeconds); }

			static ::Uml::Attribute meta_MaxNbMessages;
			Udm::IntegerAttr MaxNbMessages() const { return Udm::IntegerAttr(impl, meta_MaxNbMessages); }

			static ::Uml::Attribute meta_Type;
			Udm::StringAttr Type() const { return Udm::StringAttr(impl, meta_Type); }

			static ::Uml::CompositionParentRole meta_Component_parent;
			Udm::ParentAttr< ::FCSL::Component> Component_parent() const { return Udm::ParentAttr< ::FCSL::Component>(impl, meta_Component_parent); }

			Udm::ParentAttr< ::FCSL::MgaObject> parent() const { return Udm::ParentAttr< ::FCSL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  Channel :  public ::FCSL::MgaObject {
		public:
			static ::Uml::Class meta;

			Channel() { }
			Channel(Udm::ObjectImpl *impl) : ::FCSL::MgaObject(impl), UDM_OBJECT(impl) { }
			Channel(const Channel &master) : ::FCSL::MgaObject(master), UDM_OBJECT(master) { }
			static Channel Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static Channel Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			Channel CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			Channel CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::FCSL::Channel> Instances() { return Udm::InstantiatedAttr< ::FCSL::Channel>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::FCSL::Channel, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::FCSL::Channel, Pred>(impl);}

			Udm::DerivedAttr< ::FCSL::Channel> Derived() { return Udm::DerivedAttr< ::FCSL::Channel>(impl);}
			template <class Pred> Udm::DerivedAttr< ::FCSL::Channel, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::FCSL::Channel, Pred>(impl);}

			Udm::ArchetypeAttr< ::FCSL::Channel> Archetype() const { return Udm::ArchetypeAttr< ::FCSL::Channel>(impl);}

			static ::Uml::Attribute meta_ID;
			Udm::StringAttr ID() const { return Udm::StringAttr(impl, meta_ID); }

			static ::Uml::Attribute meta_Name;
			Udm::StringAttr Name() const { return Udm::StringAttr(impl, meta_Name); }

			static ::Uml::Attribute meta_Type;
			Udm::StringAttr Type() const { return Udm::StringAttr(impl, meta_Type); }

			static ::Uml::AssociationRole meta_srcPort2Channel;
			static ::Uml::AssociationRole meta_srcPort2Channel_rev;
			Udm::AClassAssocAttr< ::FCSL::Port2Channel, ::FCSL::OutputPort> srcPort2Channel() const { return Udm::AClassAssocAttr< ::FCSL::Port2Channel, ::FCSL::OutputPort>(impl, meta_srcPort2Channel, meta_srcPort2Channel_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::FCSL::Port2Channel, ::FCSL::OutputPort, Pred> srcPort2Channel_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::FCSL::Port2Channel, ::FCSL::OutputPort, Pred>(impl, meta_srcPort2Channel, meta_srcPort2Channel_rev); }

			static ::Uml::AssociationRole meta_dstChannel2Port;
			static ::Uml::AssociationRole meta_dstChannel2Port_rev;
			Udm::AClassAssocAttr< ::FCSL::Channel2Port, ::FCSL::InputPort> dstChannel2Port() const { return Udm::AClassAssocAttr< ::FCSL::Channel2Port, ::FCSL::InputPort>(impl, meta_dstChannel2Port, meta_dstChannel2Port_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::FCSL::Channel2Port, ::FCSL::InputPort, Pred> dstChannel2Port_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::FCSL::Channel2Port, ::FCSL::InputPort, Pred>(impl, meta_dstChannel2Port, meta_dstChannel2Port_rev); }

			static ::Uml::CompositionParentRole meta_ComponentSheet_parent;
			Udm::ParentAttr< ::FCSL::ComponentSheet> ComponentSheet_parent() const { return Udm::ParentAttr< ::FCSL::ComponentSheet>(impl, meta_ComponentSheet_parent); }

			Udm::ParentAttr< ::FCSL::MgaObject> parent() const { return Udm::ParentAttr< ::FCSL::MgaObject>(impl, Udm::NULLPARENTROLE); }
			static ::Uml::Constraint meta_PortChannelTypeConsistency;
		};

		class  EventChannel :  public ::FCSL::MgaObject {
		public:
			static ::Uml::Class meta;

			EventChannel() { }
			EventChannel(Udm::ObjectImpl *impl) : ::FCSL::MgaObject(impl), UDM_OBJECT(impl) { }
			EventChannel(const EventChannel &master) : ::FCSL::MgaObject(master), UDM_OBJECT(master) { }
			static EventChannel Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static EventChannel Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			EventChannel CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			EventChannel CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::FCSL::EventChannel> Instances() { return Udm::InstantiatedAttr< ::FCSL::EventChannel>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::FCSL::EventChannel, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::FCSL::EventChannel, Pred>(impl);}

			Udm::DerivedAttr< ::FCSL::EventChannel> Derived() { return Udm::DerivedAttr< ::FCSL::EventChannel>(impl);}
			template <class Pred> Udm::DerivedAttr< ::FCSL::EventChannel, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::FCSL::EventChannel, Pred>(impl);}

			Udm::ArchetypeAttr< ::FCSL::EventChannel> Archetype() const { return Udm::ArchetypeAttr< ::FCSL::EventChannel>(impl);}

			static ::Uml::Attribute meta_QueuingDiscipline;
			Udm::StringAttr QueuingDiscipline() const { return Udm::StringAttr(impl, meta_QueuingDiscipline); }

			static ::Uml::Attribute meta_Name;
			Udm::StringAttr Name() const { return Udm::StringAttr(impl, meta_Name); }

			static ::Uml::Attribute meta_ChannelType;
			Udm::StringAttr ChannelType() const { return Udm::StringAttr(impl, meta_ChannelType); }

			static ::Uml::Attribute meta_Count;
			Udm::IntegerAttr Count() const { return Udm::IntegerAttr(impl, meta_Count); }

			static ::Uml::AssociationRole meta_srcOE2EC;
			static ::Uml::AssociationRole meta_srcOE2EC_rev;
			Udm::AClassAssocAttr< ::FCSL::OE2EC, ::FCSL::OutputEvent> srcOE2EC() const { return Udm::AClassAssocAttr< ::FCSL::OE2EC, ::FCSL::OutputEvent>(impl, meta_srcOE2EC, meta_srcOE2EC_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::FCSL::OE2EC, ::FCSL::OutputEvent, Pred> srcOE2EC_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::FCSL::OE2EC, ::FCSL::OutputEvent, Pred>(impl, meta_srcOE2EC, meta_srcOE2EC_rev); }

			static ::Uml::AssociationRole meta_dstEC2IE;
			static ::Uml::AssociationRole meta_dstEC2IE_rev;
			Udm::AClassAssocAttr< ::FCSL::EC2IE, ::FCSL::InputEvent> dstEC2IE() const { return Udm::AClassAssocAttr< ::FCSL::EC2IE, ::FCSL::InputEvent>(impl, meta_dstEC2IE, meta_dstEC2IE_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::FCSL::EC2IE, ::FCSL::InputEvent, Pred> dstEC2IE_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::FCSL::EC2IE, ::FCSL::InputEvent, Pred>(impl, meta_dstEC2IE, meta_dstEC2IE_rev); }

			static ::Uml::CompositionParentRole meta_ComponentSheet_parent;
			Udm::ParentAttr< ::FCSL::ComponentSheet> ComponentSheet_parent() const { return Udm::ParentAttr< ::FCSL::ComponentSheet>(impl, meta_ComponentSheet_parent); }

			Udm::ParentAttr< ::FCSL::MgaObject> parent() const { return Udm::ParentAttr< ::FCSL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  ComponentSheet :  public ::FCSL::MgaObject {
		public:
			static ::Uml::Class meta;

			ComponentSheet() { }
			ComponentSheet(Udm::ObjectImpl *impl) : ::FCSL::MgaObject(impl), UDM_OBJECT(impl) { }
			ComponentSheet(const ComponentSheet &master) : ::FCSL::MgaObject(master), UDM_OBJECT(master) { }
			static ComponentSheet Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static ComponentSheet Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			ComponentSheet CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			ComponentSheet CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::FCSL::ComponentSheet> Instances() { return Udm::InstantiatedAttr< ::FCSL::ComponentSheet>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::FCSL::ComponentSheet, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::FCSL::ComponentSheet, Pred>(impl);}

			Udm::DerivedAttr< ::FCSL::ComponentSheet> Derived() { return Udm::DerivedAttr< ::FCSL::ComponentSheet>(impl);}
			template <class Pred> Udm::DerivedAttr< ::FCSL::ComponentSheet, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::FCSL::ComponentSheet, Pred>(impl);}

			Udm::ArchetypeAttr< ::FCSL::ComponentSheet> Archetype() const { return Udm::ArchetypeAttr< ::FCSL::ComponentSheet>(impl);}

			static ::Uml::CompositionChildRole meta_Component_children;
			Udm::ChildrenAttr< ::FCSL::Component> Component_children() const { return Udm::ChildrenAttr< ::FCSL::Component>(impl, meta_Component_children); }
			template <class Pred> Udm::ChildrenAttr< ::FCSL::Component, Pred> Component_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::Component, Pred>(impl, meta_Component_children); }

			static ::Uml::CompositionChildRole meta_EC2IE_children;
			Udm::ChildrenAttr< ::FCSL::EC2IE> EC2IE_children() const { return Udm::ChildrenAttr< ::FCSL::EC2IE>(impl, meta_EC2IE_children); }
			template <class Pred> Udm::ChildrenAttr< ::FCSL::EC2IE, Pred> EC2IE_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::EC2IE, Pred>(impl, meta_EC2IE_children); }

			static ::Uml::CompositionChildRole meta_OE2EC_children;
			Udm::ChildrenAttr< ::FCSL::OE2EC> OE2EC_children() const { return Udm::ChildrenAttr< ::FCSL::OE2EC>(impl, meta_OE2EC_children); }
			template <class Pred> Udm::ChildrenAttr< ::FCSL::OE2EC, Pred> OE2EC_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::OE2EC, Pred>(impl, meta_OE2EC_children); }

			static ::Uml::CompositionChildRole meta_ComponentShortcut_children;
			Udm::ChildrenAttr< ::FCSL::ComponentShortcut> ComponentShortcut_children() const { return Udm::ChildrenAttr< ::FCSL::ComponentShortcut>(impl, meta_ComponentShortcut_children); }
			template <class Pred> Udm::ChildrenAttr< ::FCSL::ComponentShortcut, Pred> ComponentShortcut_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::ComponentShortcut, Pred>(impl, meta_ComponentShortcut_children); }

			static ::Uml::CompositionChildRole meta_Channel2Port_children;
			Udm::ChildrenAttr< ::FCSL::Channel2Port> Channel2Port_children() const { return Udm::ChildrenAttr< ::FCSL::Channel2Port>(impl, meta_Channel2Port_children); }
			template <class Pred> Udm::ChildrenAttr< ::FCSL::Channel2Port, Pred> Channel2Port_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::Channel2Port, Pred>(impl, meta_Channel2Port_children); }

			static ::Uml::CompositionChildRole meta_EventChannel_children;
			Udm::ChildrenAttr< ::FCSL::EventChannel> EventChannel_children() const { return Udm::ChildrenAttr< ::FCSL::EventChannel>(impl, meta_EventChannel_children); }
			template <class Pred> Udm::ChildrenAttr< ::FCSL::EventChannel, Pred> EventChannel_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::EventChannel, Pred>(impl, meta_EventChannel_children); }

			static ::Uml::CompositionChildRole meta_Port2Channel_children;
			Udm::ChildrenAttr< ::FCSL::Port2Channel> Port2Channel_children() const { return Udm::ChildrenAttr< ::FCSL::Port2Channel>(impl, meta_Port2Channel_children); }
			template <class Pred> Udm::ChildrenAttr< ::FCSL::Port2Channel, Pred> Port2Channel_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::Port2Channel, Pred>(impl, meta_Port2Channel_children); }

			static ::Uml::CompositionChildRole meta_Channel_children;
			Udm::ChildrenAttr< ::FCSL::Channel> Channel_children() const { return Udm::ChildrenAttr< ::FCSL::Channel>(impl, meta_Channel_children); }
			template <class Pred> Udm::ChildrenAttr< ::FCSL::Channel, Pred> Channel_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::Channel, Pred>(impl, meta_Channel_children); }

			Udm::ChildrenAttr< ::FCSL::Port2Channel> Port2Channel_kind_children() const { return Udm::ChildrenAttr< ::FCSL::Port2Channel>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::Port2Channel, Pred> Port2Channel_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::Port2Channel, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::Channel2Port> Channel2Port_kind_children() const { return Udm::ChildrenAttr< ::FCSL::Channel2Port>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::Channel2Port, Pred> Channel2Port_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::Channel2Port, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::Channel> Channel_kind_children() const { return Udm::ChildrenAttr< ::FCSL::Channel>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::Channel, Pred> Channel_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::Channel, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::EventChannel> EventChannel_kind_children() const { return Udm::ChildrenAttr< ::FCSL::EventChannel>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::EventChannel, Pred> EventChannel_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::EventChannel, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::Component> Component_kind_children() const { return Udm::ChildrenAttr< ::FCSL::Component>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::Component, Pred> Component_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::Component, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::OE2EC> OE2EC_kind_children() const { return Udm::ChildrenAttr< ::FCSL::OE2EC>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::OE2EC, Pred> OE2EC_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::OE2EC, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::EC2IE> EC2IE_kind_children() const { return Udm::ChildrenAttr< ::FCSL::EC2IE>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::EC2IE, Pred> EC2IE_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::EC2IE, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::ComponentShortcut> ComponentShortcut_kind_children() const { return Udm::ChildrenAttr< ::FCSL::ComponentShortcut>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::ComponentShortcut, Pred> ComponentShortcut_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::ComponentShortcut, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr< ::FCSL::MgaObject>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

			static ::Uml::CompositionParentRole meta_ComponentModels_parent;
			Udm::ParentAttr< ::FCSL::ComponentModels> ComponentModels_parent() const { return Udm::ParentAttr< ::FCSL::ComponentModels>(impl, meta_ComponentModels_parent); }

			Udm::ParentAttr< ::FCSL::ComponentModels> parent() const { return Udm::ParentAttr< ::FCSL::ComponentModels>(impl, Udm::NULLPARENTROLE); }
		};

		class  Component :  public ::FCSL::MgaObject {
		public:
			static ::Uml::Class meta;

			Component() { }
			Component(Udm::ObjectImpl *impl) : ::FCSL::MgaObject(impl), UDM_OBJECT(impl) { }
			Component(const Component &master) : ::FCSL::MgaObject(master), UDM_OBJECT(master) { }
			static Component Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static Component Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			Component CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			Component CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::FCSL::Component> Instances() { return Udm::InstantiatedAttr< ::FCSL::Component>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::FCSL::Component, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::FCSL::Component, Pred>(impl);}

			Udm::DerivedAttr< ::FCSL::Component> Derived() { return Udm::DerivedAttr< ::FCSL::Component>(impl);}
			template <class Pred> Udm::DerivedAttr< ::FCSL::Component, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::FCSL::Component, Pred>(impl);}

			Udm::ArchetypeAttr< ::FCSL::Component> Archetype() const { return Udm::ArchetypeAttr< ::FCSL::Component>(impl);}

			static ::Uml::Attribute meta_Name;
			Udm::StringAttr Name() const { return Udm::StringAttr(impl, meta_Name); }

			static ::Uml::AssociationRole meta_referedbyProcess;
			Udm::AssocAttr< ::FCSL::Process> referedbyProcess() const { return Udm::AssocAttr< ::FCSL::Process>(impl, meta_referedbyProcess); }
			template <class Pred> Udm::AssocAttr< ::FCSL::Process, Pred > referedbyProcess_sorted(const Pred &) const { return Udm::AssocAttr< ::FCSL::Process, Pred>(impl, meta_referedbyProcess); }

			static ::Uml::AssociationRole meta_referedbyComponentShortcut;
			Udm::AssocAttr< ::FCSL::ComponentShortcut> referedbyComponentShortcut() const { return Udm::AssocAttr< ::FCSL::ComponentShortcut>(impl, meta_referedbyComponentShortcut); }
			template <class Pred> Udm::AssocAttr< ::FCSL::ComponentShortcut, Pred > referedbyComponentShortcut_sorted(const Pred &) const { return Udm::AssocAttr< ::FCSL::ComponentShortcut, Pred>(impl, meta_referedbyComponentShortcut); }

			static ::Uml::CompositionChildRole meta_EventPortMapping_children;
			Udm::ChildrenAttr< ::FCSL::EventPortMapping> EventPortMapping_children() const { return Udm::ChildrenAttr< ::FCSL::EventPortMapping>(impl, meta_EventPortMapping_children); }
			template <class Pred> Udm::ChildrenAttr< ::FCSL::EventPortMapping, Pred> EventPortMapping_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::EventPortMapping, Pred>(impl, meta_EventPortMapping_children); }

			static ::Uml::CompositionChildRole meta_Port_children;
			Udm::ChildrenAttr< ::FCSL::Port> Port_children() const { return Udm::ChildrenAttr< ::FCSL::Port>(impl, meta_Port_children); }
			template <class Pred> Udm::ChildrenAttr< ::FCSL::Port, Pred> Port_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::Port, Pred>(impl, meta_Port_children); }

			static ::Uml::CompositionChildRole meta_SystemRef_child;
			Udm::ChildAttr< ::FCSL::SystemRef> SystemRef_child() const { return Udm::ChildAttr< ::FCSL::SystemRef>(impl, meta_SystemRef_child); }

			static ::Uml::CompositionChildRole meta_BitTestRef_child;
			Udm::ChildAttr< ::FCSL::BitTestRef> BitTestRef_child() const { return Udm::ChildAttr< ::FCSL::BitTestRef>(impl, meta_BitTestRef_child); }

			static ::Uml::CompositionChildRole meta_PortMapping_children;
			Udm::ChildrenAttr< ::FCSL::PortMapping> PortMapping_children() const { return Udm::ChildrenAttr< ::FCSL::PortMapping>(impl, meta_PortMapping_children); }
			template <class Pred> Udm::ChildrenAttr< ::FCSL::PortMapping, Pred> PortMapping_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::PortMapping, Pred>(impl, meta_PortMapping_children); }

			static ::Uml::CompositionChildRole meta_EventPort_children;
			Udm::ChildrenAttr< ::FCSL::EventPort> EventPort_children() const { return Udm::ChildrenAttr< ::FCSL::EventPort>(impl, meta_EventPort_children); }
			template <class Pred> Udm::ChildrenAttr< ::FCSL::EventPort, Pred> EventPort_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::EventPort, Pred>(impl, meta_EventPort_children); }

			Udm::ChildrenAttr< ::FCSL::EventPortMapping_Members_Base> EventPortMapping_Members_Base_kind_children() const { return Udm::ChildrenAttr< ::FCSL::EventPortMapping_Members_Base>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::EventPortMapping_Members_Base, Pred> EventPortMapping_Members_Base_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::EventPortMapping_Members_Base, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::CommMapping_Members_Base> CommMapping_Members_Base_kind_children() const { return Udm::ChildrenAttr< ::FCSL::CommMapping_Members_Base>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::CommMapping_Members_Base, Pred> CommMapping_Members_Base_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::CommMapping_Members_Base, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::PortMapping_Members_Base> PortMapping_Members_Base_kind_children() const { return Udm::ChildrenAttr< ::FCSL::PortMapping_Members_Base>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::PortMapping_Members_Base, Pred> PortMapping_Members_Base_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::PortMapping_Members_Base, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::EventPort> EventPort_kind_children() const { return Udm::ChildrenAttr< ::FCSL::EventPort>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::EventPort, Pred> EventPort_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::EventPort, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::Port> Port_kind_children() const { return Udm::ChildrenAttr< ::FCSL::Port>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::Port, Pred> Port_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::Port, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::EventPortMapping> EventPortMapping_kind_children() const { return Udm::ChildrenAttr< ::FCSL::EventPortMapping>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::EventPortMapping, Pred> EventPortMapping_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::EventPortMapping, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::OutputPort> OutputPort_kind_children() const { return Udm::ChildrenAttr< ::FCSL::OutputPort>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::OutputPort, Pred> OutputPort_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::OutputPort, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::BitTestRef> BitTestRef_kind_children() const { return Udm::ChildrenAttr< ::FCSL::BitTestRef>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::BitTestRef, Pred> BitTestRef_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::BitTestRef, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::PortMapping> PortMapping_kind_children() const { return Udm::ChildrenAttr< ::FCSL::PortMapping>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::PortMapping, Pred> PortMapping_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::PortMapping, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::InputEvent> InputEvent_kind_children() const { return Udm::ChildrenAttr< ::FCSL::InputEvent>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::InputEvent, Pred> InputEvent_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::InputEvent, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::SystemRef> SystemRef_kind_children() const { return Udm::ChildrenAttr< ::FCSL::SystemRef>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::SystemRef, Pred> SystemRef_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::SystemRef, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::OutputEvent> OutputEvent_kind_children() const { return Udm::ChildrenAttr< ::FCSL::OutputEvent>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::OutputEvent, Pred> OutputEvent_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::OutputEvent, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::InputPort> InputPort_kind_children() const { return Udm::ChildrenAttr< ::FCSL::InputPort>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::InputPort, Pred> InputPort_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::InputPort, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr< ::FCSL::MgaObject>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

			static ::Uml::CompositionParentRole meta_ComponentSheet_parent;
			Udm::ParentAttr< ::FCSL::ComponentSheet> ComponentSheet_parent() const { return Udm::ParentAttr< ::FCSL::ComponentSheet>(impl, meta_ComponentSheet_parent); }

			Udm::ParentAttr< ::FCSL::MgaObject> parent() const { return Udm::ParentAttr< ::FCSL::MgaObject>(impl, Udm::NULLPARENTROLE); }
			static ::Uml::Constraint meta_ExactlyOneRef;
		};

		class  EventPortMapping :  public ::FCSL::MgaObject {
		public:
			static ::Uml::Class meta;

			EventPortMapping() { }
			EventPortMapping(Udm::ObjectImpl *impl) : ::FCSL::MgaObject(impl), UDM_OBJECT(impl) { }
			EventPortMapping(const EventPortMapping &master) : ::FCSL::MgaObject(master), UDM_OBJECT(master) { }
			static EventPortMapping Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static EventPortMapping Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			EventPortMapping CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			EventPortMapping CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::FCSL::EventPortMapping> Instances() { return Udm::InstantiatedAttr< ::FCSL::EventPortMapping>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::FCSL::EventPortMapping, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::FCSL::EventPortMapping, Pred>(impl);}

			Udm::DerivedAttr< ::FCSL::EventPortMapping> Derived() { return Udm::DerivedAttr< ::FCSL::EventPortMapping>(impl);}
			template <class Pred> Udm::DerivedAttr< ::FCSL::EventPortMapping, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::FCSL::EventPortMapping, Pred>(impl);}

			Udm::ArchetypeAttr< ::FCSL::EventPortMapping> Archetype() const { return Udm::ArchetypeAttr< ::FCSL::EventPortMapping>(impl);}

			static ::Uml::Attribute meta_StorageClass;
			Udm::StringAttr StorageClass() const { return Udm::StringAttr(impl, meta_StorageClass); }

			static ::Uml::CompositionParentRole meta_Component_parent;
			Udm::ParentAttr< ::FCSL::Component> Component_parent() const { return Udm::ParentAttr< ::FCSL::Component>(impl, meta_Component_parent); }

			Udm::ParentAttr< ::FCSL::MgaObject> parent() const { return Udm::ParentAttr< ::FCSL::MgaObject>(impl, Udm::NULLPARENTROLE); }
			static ::Uml::AssociationRole meta_dstEventPortMapping_end_;
			Udm::AssocEndAttr< ::FCSL::EventPortMapping_Members_Base> dstEventPortMapping_end() const { return Udm::AssocEndAttr< ::FCSL::EventPortMapping_Members_Base>(impl, meta_dstEventPortMapping_end_); }

			static ::Uml::AssociationRole meta_srcEventPortMapping_end_;
			Udm::AssocEndAttr< ::FCSL::EventPortMapping_Members_Base> srcEventPortMapping_end() const { return Udm::AssocEndAttr< ::FCSL::EventPortMapping_Members_Base>(impl, meta_srcEventPortMapping_end_); }

		};

		class  OutputPort :  public ::FCSL::Port {
		public:
			static ::Uml::Class meta;

			OutputPort() { }
			OutputPort(Udm::ObjectImpl *impl) : ::FCSL::Port(impl), UDM_OBJECT(impl) { }
			OutputPort(const OutputPort &master) : ::FCSL::Port(master), UDM_OBJECT(master) { }
			static OutputPort Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static OutputPort Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			OutputPort CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			OutputPort CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::FCSL::OutputPort> Instances() { return Udm::InstantiatedAttr< ::FCSL::OutputPort>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::FCSL::OutputPort, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::FCSL::OutputPort, Pred>(impl);}

			Udm::DerivedAttr< ::FCSL::OutputPort> Derived() { return Udm::DerivedAttr< ::FCSL::OutputPort>(impl);}
			template <class Pred> Udm::DerivedAttr< ::FCSL::OutputPort, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::FCSL::OutputPort, Pred>(impl);}

			Udm::ArchetypeAttr< ::FCSL::OutputPort> Archetype() const { return Udm::ArchetypeAttr< ::FCSL::OutputPort>(impl);}

			static ::Uml::AssociationRole meta_dstPort2Channel;
			static ::Uml::AssociationRole meta_dstPort2Channel_rev;
			Udm::AClassAssocAttr< ::FCSL::Port2Channel, ::FCSL::Channel> dstPort2Channel() const { return Udm::AClassAssocAttr< ::FCSL::Port2Channel, ::FCSL::Channel>(impl, meta_dstPort2Channel, meta_dstPort2Channel_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::FCSL::Port2Channel, ::FCSL::Channel, Pred> dstPort2Channel_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::FCSL::Port2Channel, ::FCSL::Channel, Pred>(impl, meta_dstPort2Channel, meta_dstPort2Channel_rev); }

			Udm::ParentAttr< ::FCSL::MgaObject> parent() const { return Udm::ParentAttr< ::FCSL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  OE2EC :  public ::FCSL::MgaObject {
		public:
			static ::Uml::Class meta;

			OE2EC() { }
			OE2EC(Udm::ObjectImpl *impl) : ::FCSL::MgaObject(impl), UDM_OBJECT(impl) { }
			OE2EC(const OE2EC &master) : ::FCSL::MgaObject(master), UDM_OBJECT(master) { }
			static OE2EC Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static OE2EC Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			OE2EC CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			OE2EC CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::FCSL::OE2EC> Instances() { return Udm::InstantiatedAttr< ::FCSL::OE2EC>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::FCSL::OE2EC, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::FCSL::OE2EC, Pred>(impl);}

			Udm::DerivedAttr< ::FCSL::OE2EC> Derived() { return Udm::DerivedAttr< ::FCSL::OE2EC>(impl);}
			template <class Pred> Udm::DerivedAttr< ::FCSL::OE2EC, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::FCSL::OE2EC, Pred>(impl);}

			Udm::ArchetypeAttr< ::FCSL::OE2EC> Archetype() const { return Udm::ArchetypeAttr< ::FCSL::OE2EC>(impl);}

			static ::Uml::CompositionParentRole meta_ComponentSheet_parent;
			Udm::ParentAttr< ::FCSL::ComponentSheet> ComponentSheet_parent() const { return Udm::ParentAttr< ::FCSL::ComponentSheet>(impl, meta_ComponentSheet_parent); }

			Udm::ParentAttr< ::FCSL::MgaObject> parent() const { return Udm::ParentAttr< ::FCSL::MgaObject>(impl, Udm::NULLPARENTROLE); }
			static ::Uml::AssociationRole meta_dstOE2EC_end_;
			Udm::AssocEndAttr< ::FCSL::EventChannel> dstOE2EC_end() const { return Udm::AssocEndAttr< ::FCSL::EventChannel>(impl, meta_dstOE2EC_end_); }

			static ::Uml::AssociationRole meta_srcOE2EC_end_;
			Udm::AssocEndAttr< ::FCSL::OutputEvent> srcOE2EC_end() const { return Udm::AssocEndAttr< ::FCSL::OutputEvent>(impl, meta_srcOE2EC_end_); }

		};

		class  EC2IE :  public ::FCSL::MgaObject {
		public:
			static ::Uml::Class meta;

			EC2IE() { }
			EC2IE(Udm::ObjectImpl *impl) : ::FCSL::MgaObject(impl), UDM_OBJECT(impl) { }
			EC2IE(const EC2IE &master) : ::FCSL::MgaObject(master), UDM_OBJECT(master) { }
			static EC2IE Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static EC2IE Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			EC2IE CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			EC2IE CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::FCSL::EC2IE> Instances() { return Udm::InstantiatedAttr< ::FCSL::EC2IE>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::FCSL::EC2IE, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::FCSL::EC2IE, Pred>(impl);}

			Udm::DerivedAttr< ::FCSL::EC2IE> Derived() { return Udm::DerivedAttr< ::FCSL::EC2IE>(impl);}
			template <class Pred> Udm::DerivedAttr< ::FCSL::EC2IE, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::FCSL::EC2IE, Pred>(impl);}

			Udm::ArchetypeAttr< ::FCSL::EC2IE> Archetype() const { return Udm::ArchetypeAttr< ::FCSL::EC2IE>(impl);}

			static ::Uml::CompositionParentRole meta_ComponentSheet_parent;
			Udm::ParentAttr< ::FCSL::ComponentSheet> ComponentSheet_parent() const { return Udm::ParentAttr< ::FCSL::ComponentSheet>(impl, meta_ComponentSheet_parent); }

			Udm::ParentAttr< ::FCSL::MgaObject> parent() const { return Udm::ParentAttr< ::FCSL::MgaObject>(impl, Udm::NULLPARENTROLE); }
			static ::Uml::AssociationRole meta_srcEC2IE_end_;
			Udm::AssocEndAttr< ::FCSL::EventChannel> srcEC2IE_end() const { return Udm::AssocEndAttr< ::FCSL::EventChannel>(impl, meta_srcEC2IE_end_); }

			static ::Uml::AssociationRole meta_dstEC2IE_end_;
			Udm::AssocEndAttr< ::FCSL::InputEvent> dstEC2IE_end() const { return Udm::AssocEndAttr< ::FCSL::InputEvent>(impl, meta_dstEC2IE_end_); }

		};

		class  BitTestRef :  public ::FCSL::MgaObject {
		public:
			static ::Uml::Class meta;

			BitTestRef() { }
			BitTestRef(Udm::ObjectImpl *impl) : ::FCSL::MgaObject(impl), UDM_OBJECT(impl) { }
			BitTestRef(const BitTestRef &master) : ::FCSL::MgaObject(master), UDM_OBJECT(master) { }
			static BitTestRef Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static BitTestRef Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			BitTestRef CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			BitTestRef CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::FCSL::BitTestRef> Instances() { return Udm::InstantiatedAttr< ::FCSL::BitTestRef>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::FCSL::BitTestRef, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::FCSL::BitTestRef, Pred>(impl);}

			Udm::DerivedAttr< ::FCSL::BitTestRef> Derived() { return Udm::DerivedAttr< ::FCSL::BitTestRef>(impl);}
			template <class Pred> Udm::DerivedAttr< ::FCSL::BitTestRef, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::FCSL::BitTestRef, Pred>(impl);}

			Udm::ArchetypeAttr< ::FCSL::BitTestRef> Archetype() const { return Udm::ArchetypeAttr< ::FCSL::BitTestRef>(impl);}

			static ::Uml::AssociationRole meta_ref;
			Udm::PointerAttr< ::FCSL::FM::Test> ref() const { return Udm::PointerAttr< ::FCSL::FM::Test>(impl, meta_ref); }

			static ::Uml::CompositionParentRole meta_Component_parent;
			Udm::ParentAttr< ::FCSL::Component> Component_parent() const { return Udm::ParentAttr< ::FCSL::Component>(impl, meta_Component_parent); }

			Udm::ParentAttr< ::FCSL::MgaObject> parent() const { return Udm::ParentAttr< ::FCSL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  PortMapping :  public ::FCSL::MgaObject {
		public:
			static ::Uml::Class meta;

			PortMapping() { }
			PortMapping(Udm::ObjectImpl *impl) : ::FCSL::MgaObject(impl), UDM_OBJECT(impl) { }
			PortMapping(const PortMapping &master) : ::FCSL::MgaObject(master), UDM_OBJECT(master) { }
			static PortMapping Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static PortMapping Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			PortMapping CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			PortMapping CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::FCSL::PortMapping> Instances() { return Udm::InstantiatedAttr< ::FCSL::PortMapping>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::FCSL::PortMapping, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::FCSL::PortMapping, Pred>(impl);}

			Udm::DerivedAttr< ::FCSL::PortMapping> Derived() { return Udm::DerivedAttr< ::FCSL::PortMapping>(impl);}
			template <class Pred> Udm::DerivedAttr< ::FCSL::PortMapping, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::FCSL::PortMapping, Pred>(impl);}

			Udm::ArchetypeAttr< ::FCSL::PortMapping> Archetype() const { return Udm::ArchetypeAttr< ::FCSL::PortMapping>(impl);}

			static ::Uml::Attribute meta_StorageClass;
			Udm::StringAttr StorageClass() const { return Udm::StringAttr(impl, meta_StorageClass); }

			static ::Uml::CompositionParentRole meta_Component_parent;
			Udm::ParentAttr< ::FCSL::Component> Component_parent() const { return Udm::ParentAttr< ::FCSL::Component>(impl, meta_Component_parent); }

			Udm::ParentAttr< ::FCSL::MgaObject> parent() const { return Udm::ParentAttr< ::FCSL::MgaObject>(impl, Udm::NULLPARENTROLE); }
			static ::Uml::AssociationRole meta_dstInPortMapping_end_;
			Udm::AssocEndAttr< ::FCSL::PortMapping_Members_Base> dstInPortMapping_end() const { return Udm::AssocEndAttr< ::FCSL::PortMapping_Members_Base>(impl, meta_dstInPortMapping_end_); }

			static ::Uml::AssociationRole meta_srcInPortMapping_end_;
			Udm::AssocEndAttr< ::FCSL::PortMapping_Members_Base> srcInPortMapping_end() const { return Udm::AssocEndAttr< ::FCSL::PortMapping_Members_Base>(impl, meta_srcInPortMapping_end_); }

		};

		class  InputEvent :  public ::FCSL::EventPort {
		public:
			static ::Uml::Class meta;

			InputEvent() { }
			InputEvent(Udm::ObjectImpl *impl) : ::FCSL::EventPort(impl), UDM_OBJECT(impl) { }
			InputEvent(const InputEvent &master) : ::FCSL::EventPort(master), UDM_OBJECT(master) { }
			static InputEvent Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static InputEvent Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			InputEvent CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			InputEvent CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::FCSL::InputEvent> Instances() { return Udm::InstantiatedAttr< ::FCSL::InputEvent>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::FCSL::InputEvent, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::FCSL::InputEvent, Pred>(impl);}

			Udm::DerivedAttr< ::FCSL::InputEvent> Derived() { return Udm::DerivedAttr< ::FCSL::InputEvent>(impl);}
			template <class Pred> Udm::DerivedAttr< ::FCSL::InputEvent, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::FCSL::InputEvent, Pred>(impl);}

			Udm::ArchetypeAttr< ::FCSL::InputEvent> Archetype() const { return Udm::ArchetypeAttr< ::FCSL::InputEvent>(impl);}

			static ::Uml::AssociationRole meta_srcEC2IE;
			static ::Uml::AssociationRole meta_srcEC2IE_rev;
			Udm::AClassAssocAttr< ::FCSL::EC2IE, ::FCSL::EventChannel> srcEC2IE() const { return Udm::AClassAssocAttr< ::FCSL::EC2IE, ::FCSL::EventChannel>(impl, meta_srcEC2IE, meta_srcEC2IE_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::FCSL::EC2IE, ::FCSL::EventChannel, Pred> srcEC2IE_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::FCSL::EC2IE, ::FCSL::EventChannel, Pred>(impl, meta_srcEC2IE, meta_srcEC2IE_rev); }

			Udm::ParentAttr< ::FCSL::MgaObject> parent() const { return Udm::ParentAttr< ::FCSL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  SystemRef :  public ::FCSL::MgaObject {
		public:
			static ::Uml::Class meta;

			SystemRef() { }
			SystemRef(Udm::ObjectImpl *impl) : ::FCSL::MgaObject(impl), UDM_OBJECT(impl) { }
			SystemRef(const SystemRef &master) : ::FCSL::MgaObject(master), UDM_OBJECT(master) { }
			static SystemRef Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static SystemRef Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			SystemRef CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			SystemRef CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::FCSL::SystemRef> Instances() { return Udm::InstantiatedAttr< ::FCSL::SystemRef>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::FCSL::SystemRef, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::FCSL::SystemRef, Pred>(impl);}

			Udm::DerivedAttr< ::FCSL::SystemRef> Derived() { return Udm::DerivedAttr< ::FCSL::SystemRef>(impl);}
			template <class Pred> Udm::DerivedAttr< ::FCSL::SystemRef, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::FCSL::SystemRef, Pred>(impl);}

			Udm::ArchetypeAttr< ::FCSL::SystemRef> Archetype() const { return Udm::ArchetypeAttr< ::FCSL::SystemRef>(impl);}

			static ::Uml::AssociationRole meta_ref;
			Udm::PointerAttr< ::FCSL::SLSF::System> ref() const { return Udm::PointerAttr< ::FCSL::SLSF::System>(impl, meta_ref); }

			static ::Uml::CompositionParentRole meta_Component_parent;
			Udm::ParentAttr< ::FCSL::Component> Component_parent() const { return Udm::ParentAttr< ::FCSL::Component>(impl, meta_Component_parent); }

			Udm::ParentAttr< ::FCSL::MgaObject> parent() const { return Udm::ParentAttr< ::FCSL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  OutputEvent :  public ::FCSL::EventPort {
		public:
			static ::Uml::Class meta;

			OutputEvent() { }
			OutputEvent(Udm::ObjectImpl *impl) : ::FCSL::EventPort(impl), UDM_OBJECT(impl) { }
			OutputEvent(const OutputEvent &master) : ::FCSL::EventPort(master), UDM_OBJECT(master) { }
			static OutputEvent Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static OutputEvent Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			OutputEvent CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			OutputEvent CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::FCSL::OutputEvent> Instances() { return Udm::InstantiatedAttr< ::FCSL::OutputEvent>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::FCSL::OutputEvent, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::FCSL::OutputEvent, Pred>(impl);}

			Udm::DerivedAttr< ::FCSL::OutputEvent> Derived() { return Udm::DerivedAttr< ::FCSL::OutputEvent>(impl);}
			template <class Pred> Udm::DerivedAttr< ::FCSL::OutputEvent, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::FCSL::OutputEvent, Pred>(impl);}

			Udm::ArchetypeAttr< ::FCSL::OutputEvent> Archetype() const { return Udm::ArchetypeAttr< ::FCSL::OutputEvent>(impl);}

			static ::Uml::AssociationRole meta_dstOE2EC;
			static ::Uml::AssociationRole meta_dstOE2EC_rev;
			Udm::AClassAssocAttr< ::FCSL::OE2EC, ::FCSL::EventChannel> dstOE2EC() const { return Udm::AClassAssocAttr< ::FCSL::OE2EC, ::FCSL::EventChannel>(impl, meta_dstOE2EC, meta_dstOE2EC_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::FCSL::OE2EC, ::FCSL::EventChannel, Pred> dstOE2EC_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::FCSL::OE2EC, ::FCSL::EventChannel, Pred>(impl, meta_dstOE2EC, meta_dstOE2EC_rev); }

			Udm::ParentAttr< ::FCSL::MgaObject> parent() const { return Udm::ParentAttr< ::FCSL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  InputPort :  public ::FCSL::Port {
		public:
			static ::Uml::Class meta;

			InputPort() { }
			InputPort(Udm::ObjectImpl *impl) : ::FCSL::Port(impl), UDM_OBJECT(impl) { }
			InputPort(const InputPort &master) : ::FCSL::Port(master), UDM_OBJECT(master) { }
			static InputPort Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static InputPort Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			InputPort CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			InputPort CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::FCSL::InputPort> Instances() { return Udm::InstantiatedAttr< ::FCSL::InputPort>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::FCSL::InputPort, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::FCSL::InputPort, Pred>(impl);}

			Udm::DerivedAttr< ::FCSL::InputPort> Derived() { return Udm::DerivedAttr< ::FCSL::InputPort>(impl);}
			template <class Pred> Udm::DerivedAttr< ::FCSL::InputPort, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::FCSL::InputPort, Pred>(impl);}

			Udm::ArchetypeAttr< ::FCSL::InputPort> Archetype() const { return Udm::ArchetypeAttr< ::FCSL::InputPort>(impl);}

			static ::Uml::AssociationRole meta_srcChannel2Port;
			static ::Uml::AssociationRole meta_srcChannel2Port_rev;
			Udm::AClassAssocAttr< ::FCSL::Channel2Port, ::FCSL::Channel> srcChannel2Port() const { return Udm::AClassAssocAttr< ::FCSL::Channel2Port, ::FCSL::Channel>(impl, meta_srcChannel2Port, meta_srcChannel2Port_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::FCSL::Channel2Port, ::FCSL::Channel, Pred> srcChannel2Port_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::FCSL::Channel2Port, ::FCSL::Channel, Pred>(impl, meta_srcChannel2Port, meta_srcChannel2Port_rev); }

			Udm::ParentAttr< ::FCSL::MgaObject> parent() const { return Udm::ParentAttr< ::FCSL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  ComponentShortcut :  public ::FCSL::MgaObject {
		public:
			static ::Uml::Class meta;

			ComponentShortcut() { }
			ComponentShortcut(Udm::ObjectImpl *impl) : ::FCSL::MgaObject(impl), UDM_OBJECT(impl) { }
			ComponentShortcut(const ComponentShortcut &master) : ::FCSL::MgaObject(master), UDM_OBJECT(master) { }
			static ComponentShortcut Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static ComponentShortcut Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			ComponentShortcut CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			ComponentShortcut CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::FCSL::ComponentShortcut> Instances() { return Udm::InstantiatedAttr< ::FCSL::ComponentShortcut>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::FCSL::ComponentShortcut, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::FCSL::ComponentShortcut, Pred>(impl);}

			Udm::DerivedAttr< ::FCSL::ComponentShortcut> Derived() { return Udm::DerivedAttr< ::FCSL::ComponentShortcut>(impl);}
			template <class Pred> Udm::DerivedAttr< ::FCSL::ComponentShortcut, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::FCSL::ComponentShortcut, Pred>(impl);}

			Udm::ArchetypeAttr< ::FCSL::ComponentShortcut> Archetype() const { return Udm::ArchetypeAttr< ::FCSL::ComponentShortcut>(impl);}

			static ::Uml::AssociationRole meta_ref;
			Udm::PointerAttr< ::FCSL::Component> ref() const { return Udm::PointerAttr< ::FCSL::Component>(impl, meta_ref); }

			static ::Uml::CompositionParentRole meta_ComponentSheet_parent;
			Udm::ParentAttr< ::FCSL::ComponentSheet> ComponentSheet_parent() const { return Udm::ParentAttr< ::FCSL::ComponentSheet>(impl, meta_ComponentSheet_parent); }

			Udm::ParentAttr< ::FCSL::MgaObject> parent() const { return Udm::ParentAttr< ::FCSL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};



	namespace FM {
		class  TestModels :  public Udm::Object {
		public:
			static ::Uml::Class meta;

			TestModels() { }
			TestModels(Udm::ObjectImpl *impl) : UDM_OBJECT(impl) { }
			TestModels(const TestModels &master) : UDM_OBJECT(master) { }
			static TestModels Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static TestModels Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			TestModels CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			TestModels CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::FCSL::FM::TestModels> Instances() { return Udm::InstantiatedAttr< ::FCSL::FM::TestModels>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::FCSL::FM::TestModels, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::FCSL::FM::TestModels, Pred>(impl);}

			Udm::DerivedAttr< ::FCSL::FM::TestModels> Derived() { return Udm::DerivedAttr< ::FCSL::FM::TestModels>(impl);}
			template <class Pred> Udm::DerivedAttr< ::FCSL::FM::TestModels, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::FCSL::FM::TestModels, Pred>(impl);}

			Udm::ArchetypeAttr< ::FCSL::FM::TestModels> Archetype() const { return Udm::ArchetypeAttr< ::FCSL::FM::TestModels>(impl);}

			static ::Uml::Attribute meta_name;
			Udm::StringAttr name() const { return Udm::StringAttr(impl, meta_name); }

			static ::Uml::CompositionChildRole meta_TestGroup_children;
			Udm::ChildrenAttr< ::FCSL::FM::TestGroup> TestGroup_children() const { return Udm::ChildrenAttr< ::FCSL::FM::TestGroup>(impl, meta_TestGroup_children); }
			template <class Pred> Udm::ChildrenAttr< ::FCSL::FM::TestGroup, Pred> TestGroup_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::FM::TestGroup, Pred>(impl, meta_TestGroup_children); }

			static ::Uml::CompositionChildRole meta_TestContainer_children;
			Udm::ChildrenAttr< ::FCSL::FM::TestContainer> TestContainer_children() const { return Udm::ChildrenAttr< ::FCSL::FM::TestContainer>(impl, meta_TestContainer_children); }
			template <class Pred> Udm::ChildrenAttr< ::FCSL::FM::TestContainer, Pred> TestContainer_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::FM::TestContainer, Pred>(impl, meta_TestContainer_children); }

			Udm::ChildrenAttr< ::FCSL::FM::TestContainer> TestContainer_kind_children() const { return Udm::ChildrenAttr< ::FCSL::FM::TestContainer>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::FM::TestContainer, Pred> TestContainer_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::FM::TestContainer, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::FM::TestGroup> TestGroup_kind_children() const { return Udm::ChildrenAttr< ::FCSL::FM::TestGroup>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::FM::TestGroup, Pred> TestGroup_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::FM::TestGroup, Pred>(impl, Udm::NULLCHILDROLE); }

			static ::Uml::CompositionParentRole meta_RootFolder_parent;
			Udm::ParentAttr< ::FCSL::RootFolder> RootFolder_parent() const { return Udm::ParentAttr< ::FCSL::RootFolder>(impl, meta_RootFolder_parent); }

			Udm::ParentAttr< ::FCSL::RootFolder> parent() const { return Udm::ParentAttr< ::FCSL::RootFolder>(impl, Udm::NULLPARENTROLE); }
		};

		class  PlatformModels :  public Udm::Object {
		public:
			static ::Uml::Class meta;

			PlatformModels() { }
			PlatformModels(Udm::ObjectImpl *impl) : UDM_OBJECT(impl) { }
			PlatformModels(const PlatformModels &master) : UDM_OBJECT(master) { }
			static PlatformModels Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static PlatformModels Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			PlatformModels CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			PlatformModels CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::FCSL::FM::PlatformModels> Instances() { return Udm::InstantiatedAttr< ::FCSL::FM::PlatformModels>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::FCSL::FM::PlatformModels, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::FCSL::FM::PlatformModels, Pred>(impl);}

			Udm::DerivedAttr< ::FCSL::FM::PlatformModels> Derived() { return Udm::DerivedAttr< ::FCSL::FM::PlatformModels>(impl);}
			template <class Pred> Udm::DerivedAttr< ::FCSL::FM::PlatformModels, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::FCSL::FM::PlatformModels, Pred>(impl);}

			Udm::ArchetypeAttr< ::FCSL::FM::PlatformModels> Archetype() const { return Udm::ArchetypeAttr< ::FCSL::FM::PlatformModels>(impl);}

			static ::Uml::Attribute meta_name;
			Udm::StringAttr name() const { return Udm::StringAttr(impl, meta_name); }

			static ::Uml::CompositionChildRole meta_Platform_children;
			Udm::ChildrenAttr< ::FCSL::FM::Platform> Platform_children() const { return Udm::ChildrenAttr< ::FCSL::FM::Platform>(impl, meta_Platform_children); }
			template <class Pred> Udm::ChildrenAttr< ::FCSL::FM::Platform, Pred> Platform_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::FM::Platform, Pred>(impl, meta_Platform_children); }

			Udm::ChildrenAttr< ::FCSL::FM::Platform> Platform_kind_children() const { return Udm::ChildrenAttr< ::FCSL::FM::Platform>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::FM::Platform, Pred> Platform_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::FM::Platform, Pred>(impl, Udm::NULLCHILDROLE); }

			static ::Uml::CompositionParentRole meta_RootFolder_parent;
			Udm::ParentAttr< ::FCSL::RootFolder> RootFolder_parent() const { return Udm::ParentAttr< ::FCSL::RootFolder>(impl, meta_RootFolder_parent); }

			Udm::ParentAttr< ::FCSL::RootFolder> parent() const { return Udm::ParentAttr< ::FCSL::RootFolder>(impl, Udm::NULLPARENTROLE); }
		};

		class  UnitModels :  public Udm::Object {
		public:
			static ::Uml::Class meta;

			UnitModels() { }
			UnitModels(Udm::ObjectImpl *impl) : UDM_OBJECT(impl) { }
			UnitModels(const UnitModels &master) : UDM_OBJECT(master) { }
			static UnitModels Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static UnitModels Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			UnitModels CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			UnitModels CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::FCSL::FM::UnitModels> Instances() { return Udm::InstantiatedAttr< ::FCSL::FM::UnitModels>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::FCSL::FM::UnitModels, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::FCSL::FM::UnitModels, Pred>(impl);}

			Udm::DerivedAttr< ::FCSL::FM::UnitModels> Derived() { return Udm::DerivedAttr< ::FCSL::FM::UnitModels>(impl);}
			template <class Pred> Udm::DerivedAttr< ::FCSL::FM::UnitModels, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::FCSL::FM::UnitModels, Pred>(impl);}

			Udm::ArchetypeAttr< ::FCSL::FM::UnitModels> Archetype() const { return Udm::ArchetypeAttr< ::FCSL::FM::UnitModels>(impl);}

			static ::Uml::Attribute meta_name;
			Udm::StringAttr name() const { return Udm::StringAttr(impl, meta_name); }

			static ::Uml::CompositionChildRole meta_Unit_children;
			Udm::ChildrenAttr< ::FCSL::FM::Unit> Unit_children() const { return Udm::ChildrenAttr< ::FCSL::FM::Unit>(impl, meta_Unit_children); }
			template <class Pred> Udm::ChildrenAttr< ::FCSL::FM::Unit, Pred> Unit_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::FM::Unit, Pred>(impl, meta_Unit_children); }

			Udm::ChildrenAttr< ::FCSL::FM::Device> Device_kind_children() const { return Udm::ChildrenAttr< ::FCSL::FM::Device>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::FM::Device, Pred> Device_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::FM::Device, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::FM::Surface> Surface_kind_children() const { return Udm::ChildrenAttr< ::FCSL::FM::Surface>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::FM::Surface, Pred> Surface_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::FM::Surface, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::FM::Control> Control_kind_children() const { return Udm::ChildrenAttr< ::FCSL::FM::Control>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::FM::Control, Pred> Control_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::FM::Control, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::FM::Entity> Entity_kind_children() const { return Udm::ChildrenAttr< ::FCSL::FM::Entity>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::FM::Entity, Pred> Entity_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::FM::Entity, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::FM::Unit> Unit_kind_children() const { return Udm::ChildrenAttr< ::FCSL::FM::Unit>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::FM::Unit, Pred> Unit_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::FM::Unit, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::FM::Signal> Signal_kind_children() const { return Udm::ChildrenAttr< ::FCSL::FM::Signal>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::FM::Signal, Pred> Signal_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::FM::Signal, Pred>(impl, Udm::NULLCHILDROLE); }

			static ::Uml::CompositionParentRole meta_RootFolder_parent;
			Udm::ParentAttr< ::FCSL::RootFolder> RootFolder_parent() const { return Udm::ParentAttr< ::FCSL::RootFolder>(impl, meta_RootFolder_parent); }

			Udm::ParentAttr< ::FCSL::RootFolder> parent() const { return Udm::ParentAttr< ::FCSL::RootFolder>(impl, Udm::NULLPARENTROLE); }
		};

		class  Sig2Dataport :  public ::FCSL::MgaObject {
		public:
			static ::Uml::Class meta;

			Sig2Dataport() { }
			Sig2Dataport(Udm::ObjectImpl *impl) : ::FCSL::MgaObject(impl), UDM_OBJECT(impl) { }
			Sig2Dataport(const Sig2Dataport &master) : ::FCSL::MgaObject(master), UDM_OBJECT(master) { }
			static Sig2Dataport Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static Sig2Dataport Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			Sig2Dataport CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			Sig2Dataport CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::FCSL::FM::Sig2Dataport> Instances() { return Udm::InstantiatedAttr< ::FCSL::FM::Sig2Dataport>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::FCSL::FM::Sig2Dataport, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::FCSL::FM::Sig2Dataport, Pred>(impl);}

			Udm::DerivedAttr< ::FCSL::FM::Sig2Dataport> Derived() { return Udm::DerivedAttr< ::FCSL::FM::Sig2Dataport>(impl);}
			template <class Pred> Udm::DerivedAttr< ::FCSL::FM::Sig2Dataport, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::FCSL::FM::Sig2Dataport, Pred>(impl);}

			Udm::ArchetypeAttr< ::FCSL::FM::Sig2Dataport> Archetype() const { return Udm::ArchetypeAttr< ::FCSL::FM::Sig2Dataport>(impl);}

			static ::Uml::CompositionParentRole meta_Device_parent;
			Udm::ParentAttr< ::FCSL::FM::Device> Device_parent() const { return Udm::ParentAttr< ::FCSL::FM::Device>(impl, meta_Device_parent); }

			Udm::ParentAttr< ::FCSL::MgaObject> parent() const { return Udm::ParentAttr< ::FCSL::MgaObject>(impl, Udm::NULLPARENTROLE); }
			static ::Uml::AssociationRole meta_dstSig2Dataport_end_;
			Udm::AssocEndAttr< ::FCSL::FM::DataPort> dstSig2Dataport_end() const { return Udm::AssocEndAttr< ::FCSL::FM::DataPort>(impl, meta_dstSig2Dataport_end_); }

			static ::Uml::AssociationRole meta_srcSig2Dataport_end_;
			Udm::AssocEndAttr< ::FCSL::FM::Signal> srcSig2Dataport_end() const { return Udm::AssocEndAttr< ::FCSL::FM::Signal>(impl, meta_srcSig2Dataport_end_); }

		};

		class  Device2Surface :  public ::FCSL::MgaObject {
		public:
			static ::Uml::Class meta;

			Device2Surface() { }
			Device2Surface(Udm::ObjectImpl *impl) : ::FCSL::MgaObject(impl), UDM_OBJECT(impl) { }
			Device2Surface(const Device2Surface &master) : ::FCSL::MgaObject(master), UDM_OBJECT(master) { }
			static Device2Surface Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static Device2Surface Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			Device2Surface CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			Device2Surface CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::FCSL::FM::Device2Surface> Instances() { return Udm::InstantiatedAttr< ::FCSL::FM::Device2Surface>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::FCSL::FM::Device2Surface, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::FCSL::FM::Device2Surface, Pred>(impl);}

			Udm::DerivedAttr< ::FCSL::FM::Device2Surface> Derived() { return Udm::DerivedAttr< ::FCSL::FM::Device2Surface>(impl);}
			template <class Pred> Udm::DerivedAttr< ::FCSL::FM::Device2Surface, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::FCSL::FM::Device2Surface, Pred>(impl);}

			Udm::ArchetypeAttr< ::FCSL::FM::Device2Surface> Archetype() const { return Udm::ArchetypeAttr< ::FCSL::FM::Device2Surface>(impl);}

			static ::Uml::CompositionParentRole meta_Assembly_parent;
			Udm::ParentAttr< ::FCSL::FM::Assembly> Assembly_parent() const { return Udm::ParentAttr< ::FCSL::FM::Assembly>(impl, meta_Assembly_parent); }

			Udm::ParentAttr< ::FCSL::MgaObject> parent() const { return Udm::ParentAttr< ::FCSL::MgaObject>(impl, Udm::NULLPARENTROLE); }
			static ::Uml::AssociationRole meta_srcDevice2Surface_end_;
			Udm::AssocEndAttr< ::FCSL::FM::Device> srcDevice2Surface_end() const { return Udm::AssocEndAttr< ::FCSL::FM::Device>(impl, meta_srcDevice2Surface_end_); }

			static ::Uml::AssociationRole meta_dstDevice2Surface_end_;
			Udm::AssocEndAttr< ::FCSL::FM::Surface> dstDevice2Surface_end() const { return Udm::AssocEndAttr< ::FCSL::FM::Surface>(impl, meta_dstDevice2Surface_end_); }

		};

		class  Ctrl2Device :  public ::FCSL::MgaObject {
		public:
			static ::Uml::Class meta;

			Ctrl2Device() { }
			Ctrl2Device(Udm::ObjectImpl *impl) : ::FCSL::MgaObject(impl), UDM_OBJECT(impl) { }
			Ctrl2Device(const Ctrl2Device &master) : ::FCSL::MgaObject(master), UDM_OBJECT(master) { }
			static Ctrl2Device Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static Ctrl2Device Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			Ctrl2Device CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			Ctrl2Device CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::FCSL::FM::Ctrl2Device> Instances() { return Udm::InstantiatedAttr< ::FCSL::FM::Ctrl2Device>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::FCSL::FM::Ctrl2Device, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::FCSL::FM::Ctrl2Device, Pred>(impl);}

			Udm::DerivedAttr< ::FCSL::FM::Ctrl2Device> Derived() { return Udm::DerivedAttr< ::FCSL::FM::Ctrl2Device>(impl);}
			template <class Pred> Udm::DerivedAttr< ::FCSL::FM::Ctrl2Device, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::FCSL::FM::Ctrl2Device, Pred>(impl);}

			Udm::ArchetypeAttr< ::FCSL::FM::Ctrl2Device> Archetype() const { return Udm::ArchetypeAttr< ::FCSL::FM::Ctrl2Device>(impl);}

			static ::Uml::CompositionParentRole meta_Assembly_parent;
			Udm::ParentAttr< ::FCSL::FM::Assembly> Assembly_parent() const { return Udm::ParentAttr< ::FCSL::FM::Assembly>(impl, meta_Assembly_parent); }

			Udm::ParentAttr< ::FCSL::MgaObject> parent() const { return Udm::ParentAttr< ::FCSL::MgaObject>(impl, Udm::NULLPARENTROLE); }
			static ::Uml::AssociationRole meta_dstCtrl2Device_end_;
			Udm::AssocEndAttr< ::FCSL::FM::Device> dstCtrl2Device_end() const { return Udm::AssocEndAttr< ::FCSL::FM::Device>(impl, meta_dstCtrl2Device_end_); }

			static ::Uml::AssociationRole meta_srcCtrl2Device_end_;
			Udm::AssocEndAttr< ::FCSL::FM::Control> srcCtrl2Device_end() const { return Udm::AssocEndAttr< ::FCSL::FM::Control>(impl, meta_srcCtrl2Device_end_); }

		};

		class  FaultModelElementPort :  public ::FCSL::MgaObject {
		public:
			static ::Uml::Class meta;

			FaultModelElementPort() { }
			FaultModelElementPort(Udm::ObjectImpl *impl) : ::FCSL::MgaObject(impl), UDM_OBJECT(impl) { }
			FaultModelElementPort(const FaultModelElementPort &master) : ::FCSL::MgaObject(master), UDM_OBJECT(master) { }
			static FaultModelElementPort Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static FaultModelElementPort Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			FaultModelElementPort CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			FaultModelElementPort CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::FCSL::FM::FaultModelElementPort> Instances() { return Udm::InstantiatedAttr< ::FCSL::FM::FaultModelElementPort>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::FCSL::FM::FaultModelElementPort, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::FCSL::FM::FaultModelElementPort, Pred>(impl);}

			Udm::DerivedAttr< ::FCSL::FM::FaultModelElementPort> Derived() { return Udm::DerivedAttr< ::FCSL::FM::FaultModelElementPort>(impl);}
			template <class Pred> Udm::DerivedAttr< ::FCSL::FM::FaultModelElementPort, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::FCSL::FM::FaultModelElementPort, Pred>(impl);}

			Udm::ArchetypeAttr< ::FCSL::FM::FaultModelElementPort> Archetype() const { return Udm::ArchetypeAttr< ::FCSL::FM::FaultModelElementPort>(impl);}

			static ::Uml::CompositionParentRole meta_Assembly_parent;
			Udm::ParentAttr< ::FCSL::FM::Assembly> Assembly_parent() const { return Udm::ParentAttr< ::FCSL::FM::Assembly>(impl, meta_Assembly_parent); }

			static ::Uml::CompositionParentRole meta_Unit_parent;
			Udm::ParentAttr< ::FCSL::FM::Unit> Unit_parent() const { return Udm::ParentAttr< ::FCSL::FM::Unit>(impl, meta_Unit_parent); }

			Udm::ParentAttr< ::FCSL::MgaObject> parent() const { return Udm::ParentAttr< ::FCSL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  DiscrepancyPort :  public ::FCSL::FM::FaultModelElementPort {
		public:
			static ::Uml::Class meta;

			DiscrepancyPort() { }
			DiscrepancyPort(Udm::ObjectImpl *impl) : ::FCSL::FM::FaultModelElementPort(impl), UDM_OBJECT(impl) { }
			DiscrepancyPort(const DiscrepancyPort &master) : ::FCSL::FM::FaultModelElementPort(master), UDM_OBJECT(master) { }
			static DiscrepancyPort Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static DiscrepancyPort Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			DiscrepancyPort CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			DiscrepancyPort CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::FCSL::FM::DiscrepancyPort> Instances() { return Udm::InstantiatedAttr< ::FCSL::FM::DiscrepancyPort>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::FCSL::FM::DiscrepancyPort, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::FCSL::FM::DiscrepancyPort, Pred>(impl);}

			Udm::DerivedAttr< ::FCSL::FM::DiscrepancyPort> Derived() { return Udm::DerivedAttr< ::FCSL::FM::DiscrepancyPort>(impl);}
			template <class Pred> Udm::DerivedAttr< ::FCSL::FM::DiscrepancyPort, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::FCSL::FM::DiscrepancyPort, Pred>(impl);}

			Udm::ArchetypeAttr< ::FCSL::FM::DiscrepancyPort> Archetype() const { return Udm::ArchetypeAttr< ::FCSL::FM::DiscrepancyPort>(impl);}

			static ::Uml::AssociationRole meta_srcDIP2DOP;
			static ::Uml::AssociationRole meta_srcDIP2DOP_rev;
			Udm::AClassAssocAttr< ::FCSL::FM::DIP2DOP, ::FCSL::FM::DiscrepancyPort> srcDIP2DOP() const { return Udm::AClassAssocAttr< ::FCSL::FM::DIP2DOP, ::FCSL::FM::DiscrepancyPort>(impl, meta_srcDIP2DOP, meta_srcDIP2DOP_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::FCSL::FM::DIP2DOP, ::FCSL::FM::DiscrepancyPort, Pred> srcDIP2DOP_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::FCSL::FM::DIP2DOP, ::FCSL::FM::DiscrepancyPort, Pred>(impl, meta_srcDIP2DOP, meta_srcDIP2DOP_rev); }

			static ::Uml::AssociationRole meta_dstDIP2DOP;
			static ::Uml::AssociationRole meta_dstDIP2DOP_rev;
			Udm::AClassAssocAttr< ::FCSL::FM::DIP2DOP, ::FCSL::FM::DiscrepancyPort> dstDIP2DOP() const { return Udm::AClassAssocAttr< ::FCSL::FM::DIP2DOP, ::FCSL::FM::DiscrepancyPort>(impl, meta_dstDIP2DOP, meta_dstDIP2DOP_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::FCSL::FM::DIP2DOP, ::FCSL::FM::DiscrepancyPort, Pred> dstDIP2DOP_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::FCSL::FM::DIP2DOP, ::FCSL::FM::DiscrepancyPort, Pred>(impl, meta_dstDIP2DOP, meta_dstDIP2DOP_rev); }

			Udm::ParentAttr< ::FCSL::MgaObject> parent() const { return Udm::ParentAttr< ::FCSL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  DiscrepancyOutPort :  public ::FCSL::FM::DiscrepancyPort {
		public:
			static ::Uml::Class meta;

			DiscrepancyOutPort() { }
			DiscrepancyOutPort(Udm::ObjectImpl *impl) : ::FCSL::FM::DiscrepancyPort(impl), UDM_OBJECT(impl) { }
			DiscrepancyOutPort(const DiscrepancyOutPort &master) : ::FCSL::FM::DiscrepancyPort(master), UDM_OBJECT(master) { }
			static DiscrepancyOutPort Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static DiscrepancyOutPort Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			DiscrepancyOutPort CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			DiscrepancyOutPort CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::FCSL::FM::DiscrepancyOutPort> Instances() { return Udm::InstantiatedAttr< ::FCSL::FM::DiscrepancyOutPort>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::FCSL::FM::DiscrepancyOutPort, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::FCSL::FM::DiscrepancyOutPort, Pred>(impl);}

			Udm::DerivedAttr< ::FCSL::FM::DiscrepancyOutPort> Derived() { return Udm::DerivedAttr< ::FCSL::FM::DiscrepancyOutPort>(impl);}
			template <class Pred> Udm::DerivedAttr< ::FCSL::FM::DiscrepancyOutPort, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::FCSL::FM::DiscrepancyOutPort, Pred>(impl);}

			Udm::ArchetypeAttr< ::FCSL::FM::DiscrepancyOutPort> Archetype() const { return Udm::ArchetypeAttr< ::FCSL::FM::DiscrepancyOutPort>(impl);}

			static ::Uml::AssociationRole meta_srcFM2DOP;
			static ::Uml::AssociationRole meta_srcFM2DOP_rev;
			Udm::AClassAssocAttr< ::FCSL::FM::FM2DOP, ::FCSL::FM::FailureMode> srcFM2DOP() const { return Udm::AClassAssocAttr< ::FCSL::FM::FM2DOP, ::FCSL::FM::FailureMode>(impl, meta_srcFM2DOP, meta_srcFM2DOP_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::FCSL::FM::FM2DOP, ::FCSL::FM::FailureMode, Pred> srcFM2DOP_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::FCSL::FM::FM2DOP, ::FCSL::FM::FailureMode, Pred>(impl, meta_srcFM2DOP, meta_srcFM2DOP_rev); }

			static ::Uml::AssociationRole meta_srcD2DOP;
			static ::Uml::AssociationRole meta_srcD2DOP_rev;
			Udm::AClassAssocAttr< ::FCSL::FM::D2DOP, ::FCSL::FM::Discrepancy> srcD2DOP() const { return Udm::AClassAssocAttr< ::FCSL::FM::D2DOP, ::FCSL::FM::Discrepancy>(impl, meta_srcD2DOP, meta_srcD2DOP_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::FCSL::FM::D2DOP, ::FCSL::FM::Discrepancy, Pred> srcD2DOP_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::FCSL::FM::D2DOP, ::FCSL::FM::Discrepancy, Pred>(impl, meta_srcD2DOP, meta_srcD2DOP_rev); }

			static ::Uml::AssociationRole meta_dstDOP2D;
			static ::Uml::AssociationRole meta_dstDOP2D_rev;
			Udm::AClassAssocAttr< ::FCSL::FM::DOP2D, ::FCSL::FM::Discrepancy> dstDOP2D() const { return Udm::AClassAssocAttr< ::FCSL::FM::DOP2D, ::FCSL::FM::Discrepancy>(impl, meta_dstDOP2D, meta_dstDOP2D_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::FCSL::FM::DOP2D, ::FCSL::FM::Discrepancy, Pred> dstDOP2D_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::FCSL::FM::DOP2D, ::FCSL::FM::Discrepancy, Pred>(impl, meta_dstDOP2D, meta_dstDOP2D_rev); }

			static ::Uml::AssociationRole meta_dstDOP2T;
			static ::Uml::AssociationRole meta_dstDOP2T_rev;
			Udm::AClassAssocAttr< ::FCSL::FM::DOP2T, ::FCSL::FM::TestInstance> dstDOP2T() const { return Udm::AClassAssocAttr< ::FCSL::FM::DOP2T, ::FCSL::FM::TestInstance>(impl, meta_dstDOP2T, meta_dstDOP2T_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::FCSL::FM::DOP2T, ::FCSL::FM::TestInstance, Pred> dstDOP2T_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::FCSL::FM::DOP2T, ::FCSL::FM::TestInstance, Pred>(impl, meta_dstDOP2T, meta_dstDOP2T_rev); }

			Udm::ParentAttr< ::FCSL::MgaObject> parent() const { return Udm::ParentAttr< ::FCSL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  Test :  public ::FCSL::MgaObject {
		public:
			static ::Uml::Class meta;

			Test() { }
			Test(Udm::ObjectImpl *impl) : ::FCSL::MgaObject(impl), UDM_OBJECT(impl) { }
			Test(const Test &master) : ::FCSL::MgaObject(master), UDM_OBJECT(master) { }
			static Test Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static Test Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			Test CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			Test CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::FCSL::FM::Test> Instances() { return Udm::InstantiatedAttr< ::FCSL::FM::Test>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::FCSL::FM::Test, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::FCSL::FM::Test, Pred>(impl);}

			Udm::DerivedAttr< ::FCSL::FM::Test> Derived() { return Udm::DerivedAttr< ::FCSL::FM::Test>(impl);}
			template <class Pred> Udm::DerivedAttr< ::FCSL::FM::Test, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::FCSL::FM::Test, Pred>(impl);}

			Udm::ArchetypeAttr< ::FCSL::FM::Test> Archetype() const { return Udm::ArchetypeAttr< ::FCSL::FM::Test>(impl);}

			static ::Uml::AssociationRole meta_referedbyBitTestRef;
			Udm::AssocAttr< ::FCSL::BitTestRef> referedbyBitTestRef() const { return Udm::AssocAttr< ::FCSL::BitTestRef>(impl, meta_referedbyBitTestRef); }
			template <class Pred> Udm::AssocAttr< ::FCSL::BitTestRef, Pred > referedbyBitTestRef_sorted(const Pred &) const { return Udm::AssocAttr< ::FCSL::BitTestRef, Pred>(impl, meta_referedbyBitTestRef); }

			static ::Uml::AssociationRole meta_referedbyTestRef;
			Udm::AssocAttr< ::FCSL::FM::TestRef> referedbyTestRef() const { return Udm::AssocAttr< ::FCSL::FM::TestRef>(impl, meta_referedbyTestRef); }
			template <class Pred> Udm::AssocAttr< ::FCSL::FM::TestRef, Pred > referedbyTestRef_sorted(const Pred &) const { return Udm::AssocAttr< ::FCSL::FM::TestRef, Pred>(impl, meta_referedbyTestRef); }

			static ::Uml::AssociationRole meta_referedbyTestInstance;
			Udm::AssocAttr< ::FCSL::FM::TestInstance> referedbyTestInstance() const { return Udm::AssocAttr< ::FCSL::FM::TestInstance>(impl, meta_referedbyTestInstance); }
			template <class Pred> Udm::AssocAttr< ::FCSL::FM::TestInstance, Pred > referedbyTestInstance_sorted(const Pred &) const { return Udm::AssocAttr< ::FCSL::FM::TestInstance, Pred>(impl, meta_referedbyTestInstance); }

			static ::Uml::CompositionChildRole meta_FailureModeRef_children;
			Udm::ChildrenAttr< ::FCSL::FM::FailureModeRef> FailureModeRef_children() const { return Udm::ChildrenAttr< ::FCSL::FM::FailureModeRef>(impl, meta_FailureModeRef_children); }
			template <class Pred> Udm::ChildrenAttr< ::FCSL::FM::FailureModeRef, Pred> FailureModeRef_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::FM::FailureModeRef, Pred>(impl, meta_FailureModeRef_children); }

			Udm::ChildrenAttr< ::FCSL::FM::FailureModeRef> FailureModeRef_kind_children() const { return Udm::ChildrenAttr< ::FCSL::FM::FailureModeRef>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::FM::FailureModeRef, Pred> FailureModeRef_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::FM::FailureModeRef, Pred>(impl, Udm::NULLCHILDROLE); }

			static ::Uml::CompositionParentRole meta_TestContainer_parent;
			Udm::ParentAttr< ::FCSL::FM::TestContainer> TestContainer_parent() const { return Udm::ParentAttr< ::FCSL::FM::TestContainer>(impl, meta_TestContainer_parent); }

			Udm::ParentAttr< ::FCSL::MgaObject> parent() const { return Udm::ParentAttr< ::FCSL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  TestContainer :  public ::FCSL::MgaObject {
		public:
			static ::Uml::Class meta;

			TestContainer() { }
			TestContainer(Udm::ObjectImpl *impl) : ::FCSL::MgaObject(impl), UDM_OBJECT(impl) { }
			TestContainer(const TestContainer &master) : ::FCSL::MgaObject(master), UDM_OBJECT(master) { }
			static TestContainer Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static TestContainer Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			TestContainer CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			TestContainer CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::FCSL::FM::TestContainer> Instances() { return Udm::InstantiatedAttr< ::FCSL::FM::TestContainer>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::FCSL::FM::TestContainer, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::FCSL::FM::TestContainer, Pred>(impl);}

			Udm::DerivedAttr< ::FCSL::FM::TestContainer> Derived() { return Udm::DerivedAttr< ::FCSL::FM::TestContainer>(impl);}
			template <class Pred> Udm::DerivedAttr< ::FCSL::FM::TestContainer, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::FCSL::FM::TestContainer, Pred>(impl);}

			Udm::ArchetypeAttr< ::FCSL::FM::TestContainer> Archetype() const { return Udm::ArchetypeAttr< ::FCSL::FM::TestContainer>(impl);}

			static ::Uml::CompositionChildRole meta_Test_children;
			Udm::ChildrenAttr< ::FCSL::FM::Test> Test_children() const { return Udm::ChildrenAttr< ::FCSL::FM::Test>(impl, meta_Test_children); }
			template <class Pred> Udm::ChildrenAttr< ::FCSL::FM::Test, Pred> Test_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::FM::Test, Pred>(impl, meta_Test_children); }

			Udm::ChildrenAttr< ::FCSL::FM::Test> Test_kind_children() const { return Udm::ChildrenAttr< ::FCSL::FM::Test>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::FM::Test, Pred> Test_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::FM::Test, Pred>(impl, Udm::NULLCHILDROLE); }

			static ::Uml::CompositionParentRole meta_TestModels_parent;
			Udm::ParentAttr< ::FCSL::FM::TestModels> TestModels_parent() const { return Udm::ParentAttr< ::FCSL::FM::TestModels>(impl, meta_TestModels_parent); }

			Udm::ParentAttr< ::FCSL::FM::TestModels> parent() const { return Udm::ParentAttr< ::FCSL::FM::TestModels>(impl, Udm::NULLPARENTROLE); }
		};

		class  TestGroup :  public ::FCSL::MgaObject {
		public:
			static ::Uml::Class meta;

			TestGroup() { }
			TestGroup(Udm::ObjectImpl *impl) : ::FCSL::MgaObject(impl), UDM_OBJECT(impl) { }
			TestGroup(const TestGroup &master) : ::FCSL::MgaObject(master), UDM_OBJECT(master) { }
			static TestGroup Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static TestGroup Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			TestGroup CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			TestGroup CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::FCSL::FM::TestGroup> Instances() { return Udm::InstantiatedAttr< ::FCSL::FM::TestGroup>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::FCSL::FM::TestGroup, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::FCSL::FM::TestGroup, Pred>(impl);}

			Udm::DerivedAttr< ::FCSL::FM::TestGroup> Derived() { return Udm::DerivedAttr< ::FCSL::FM::TestGroup>(impl);}
			template <class Pred> Udm::DerivedAttr< ::FCSL::FM::TestGroup, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::FCSL::FM::TestGroup, Pred>(impl);}

			Udm::ArchetypeAttr< ::FCSL::FM::TestGroup> Archetype() const { return Udm::ArchetypeAttr< ::FCSL::FM::TestGroup>(impl);}

			static ::Uml::CompositionChildRole meta_TestRef_children;
			Udm::ChildrenAttr< ::FCSL::FM::TestRef> TestRef_children() const { return Udm::ChildrenAttr< ::FCSL::FM::TestRef>(impl, meta_TestRef_children); }
			template <class Pred> Udm::ChildrenAttr< ::FCSL::FM::TestRef, Pred> TestRef_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::FM::TestRef, Pred>(impl, meta_TestRef_children); }

			Udm::ChildrenAttr< ::FCSL::FM::TestRef> TestRef_kind_children() const { return Udm::ChildrenAttr< ::FCSL::FM::TestRef>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::FM::TestRef, Pred> TestRef_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::FM::TestRef, Pred>(impl, Udm::NULLCHILDROLE); }

			static ::Uml::CompositionParentRole meta_TestModels_parent;
			Udm::ParentAttr< ::FCSL::FM::TestModels> TestModels_parent() const { return Udm::ParentAttr< ::FCSL::FM::TestModels>(impl, meta_TestModels_parent); }

			Udm::ParentAttr< ::FCSL::FM::TestModels> parent() const { return Udm::ParentAttr< ::FCSL::FM::TestModels>(impl, Udm::NULLPARENTROLE); }
		};

		class  FaultModelElement :  public ::FCSL::MgaObject {
		public:
			static ::Uml::Class meta;

			FaultModelElement() { }
			FaultModelElement(Udm::ObjectImpl *impl) : ::FCSL::MgaObject(impl), UDM_OBJECT(impl) { }
			FaultModelElement(const FaultModelElement &master) : ::FCSL::MgaObject(master), UDM_OBJECT(master) { }
			static FaultModelElement Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static FaultModelElement Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			FaultModelElement CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			FaultModelElement CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::FCSL::FM::FaultModelElement> Instances() { return Udm::InstantiatedAttr< ::FCSL::FM::FaultModelElement>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::FCSL::FM::FaultModelElement, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::FCSL::FM::FaultModelElement, Pred>(impl);}

			Udm::DerivedAttr< ::FCSL::FM::FaultModelElement> Derived() { return Udm::DerivedAttr< ::FCSL::FM::FaultModelElement>(impl);}
			template <class Pred> Udm::DerivedAttr< ::FCSL::FM::FaultModelElement, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::FCSL::FM::FaultModelElement, Pred>(impl);}

			Udm::ArchetypeAttr< ::FCSL::FM::FaultModelElement> Archetype() const { return Udm::ArchetypeAttr< ::FCSL::FM::FaultModelElement>(impl);}

			static ::Uml::CompositionParentRole meta_Assembly_parent;
			Udm::ParentAttr< ::FCSL::FM::Assembly> Assembly_parent() const { return Udm::ParentAttr< ::FCSL::FM::Assembly>(impl, meta_Assembly_parent); }

			static ::Uml::CompositionParentRole meta_Unit_parent;
			Udm::ParentAttr< ::FCSL::FM::Unit> Unit_parent() const { return Udm::ParentAttr< ::FCSL::FM::Unit>(impl, meta_Unit_parent); }

			Udm::ParentAttr< ::FCSL::MgaObject> parent() const { return Udm::ParentAttr< ::FCSL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  FM2DOP :  public ::FCSL::FM::FaultModelElement {
		public:
			static ::Uml::Class meta;

			FM2DOP() { }
			FM2DOP(Udm::ObjectImpl *impl) : ::FCSL::FM::FaultModelElement(impl), UDM_OBJECT(impl) { }
			FM2DOP(const FM2DOP &master) : ::FCSL::FM::FaultModelElement(master), UDM_OBJECT(master) { }
			static FM2DOP Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static FM2DOP Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			FM2DOP CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			FM2DOP CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::FCSL::FM::FM2DOP> Instances() { return Udm::InstantiatedAttr< ::FCSL::FM::FM2DOP>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::FCSL::FM::FM2DOP, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::FCSL::FM::FM2DOP, Pred>(impl);}

			Udm::DerivedAttr< ::FCSL::FM::FM2DOP> Derived() { return Udm::DerivedAttr< ::FCSL::FM::FM2DOP>(impl);}
			template <class Pred> Udm::DerivedAttr< ::FCSL::FM::FM2DOP, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::FCSL::FM::FM2DOP, Pred>(impl);}

			Udm::ArchetypeAttr< ::FCSL::FM::FM2DOP> Archetype() const { return Udm::ArchetypeAttr< ::FCSL::FM::FM2DOP>(impl);}

			Udm::ParentAttr< ::FCSL::MgaObject> parent() const { return Udm::ParentAttr< ::FCSL::MgaObject>(impl, Udm::NULLPARENTROLE); }
			static ::Uml::AssociationRole meta_srcFM2DOP_end_;
			Udm::AssocEndAttr< ::FCSL::FM::FailureMode> srcFM2DOP_end() const { return Udm::AssocEndAttr< ::FCSL::FM::FailureMode>(impl, meta_srcFM2DOP_end_); }

			static ::Uml::AssociationRole meta_dstFM2DOP_end_;
			Udm::AssocEndAttr< ::FCSL::FM::DiscrepancyOutPort> dstFM2DOP_end() const { return Udm::AssocEndAttr< ::FCSL::FM::DiscrepancyOutPort>(impl, meta_dstFM2DOP_end_); }

		};

		class  FailureMode :  public ::FCSL::FM::FaultModelElement {
		public:
			static ::Uml::Class meta;

			FailureMode() { }
			FailureMode(Udm::ObjectImpl *impl) : ::FCSL::FM::FaultModelElement(impl), UDM_OBJECT(impl) { }
			FailureMode(const FailureMode &master) : ::FCSL::FM::FaultModelElement(master), UDM_OBJECT(master) { }
			static FailureMode Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static FailureMode Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			FailureMode CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			FailureMode CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::FCSL::FM::FailureMode> Instances() { return Udm::InstantiatedAttr< ::FCSL::FM::FailureMode>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::FCSL::FM::FailureMode, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::FCSL::FM::FailureMode, Pred>(impl);}

			Udm::DerivedAttr< ::FCSL::FM::FailureMode> Derived() { return Udm::DerivedAttr< ::FCSL::FM::FailureMode>(impl);}
			template <class Pred> Udm::DerivedAttr< ::FCSL::FM::FailureMode, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::FCSL::FM::FailureMode, Pred>(impl);}

			Udm::ArchetypeAttr< ::FCSL::FM::FailureMode> Archetype() const { return Udm::ArchetypeAttr< ::FCSL::FM::FailureMode>(impl);}

			static ::Uml::AssociationRole meta_dstFM2DOP;
			static ::Uml::AssociationRole meta_dstFM2DOP_rev;
			Udm::AClassAssocAttr< ::FCSL::FM::FM2DOP, ::FCSL::FM::DiscrepancyOutPort> dstFM2DOP() const { return Udm::AClassAssocAttr< ::FCSL::FM::FM2DOP, ::FCSL::FM::DiscrepancyOutPort>(impl, meta_dstFM2DOP, meta_dstFM2DOP_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::FCSL::FM::FM2DOP, ::FCSL::FM::DiscrepancyOutPort, Pred> dstFM2DOP_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::FCSL::FM::FM2DOP, ::FCSL::FM::DiscrepancyOutPort, Pred>(impl, meta_dstFM2DOP, meta_dstFM2DOP_rev); }

			static ::Uml::AssociationRole meta_dstFM2D;
			static ::Uml::AssociationRole meta_dstFM2D_rev;
			Udm::AClassAssocAttr< ::FCSL::FM::FM2D, ::FCSL::FM::Discrepancy> dstFM2D() const { return Udm::AClassAssocAttr< ::FCSL::FM::FM2D, ::FCSL::FM::Discrepancy>(impl, meta_dstFM2D, meta_dstFM2D_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::FCSL::FM::FM2D, ::FCSL::FM::Discrepancy, Pred> dstFM2D_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::FCSL::FM::FM2D, ::FCSL::FM::Discrepancy, Pred>(impl, meta_dstFM2D, meta_dstFM2D_rev); }

			static ::Uml::AssociationRole meta_referedbyFailureModeRef;
			Udm::AssocAttr< ::FCSL::FM::FailureModeRef> referedbyFailureModeRef() const { return Udm::AssocAttr< ::FCSL::FM::FailureModeRef>(impl, meta_referedbyFailureModeRef); }
			template <class Pred> Udm::AssocAttr< ::FCSL::FM::FailureModeRef, Pred > referedbyFailureModeRef_sorted(const Pred &) const { return Udm::AssocAttr< ::FCSL::FM::FailureModeRef, Pred>(impl, meta_referedbyFailureModeRef); }

			Udm::ParentAttr< ::FCSL::MgaObject> parent() const { return Udm::ParentAttr< ::FCSL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  DIP2D :  public ::FCSL::FM::FaultModelElement {
		public:
			static ::Uml::Class meta;

			DIP2D() { }
			DIP2D(Udm::ObjectImpl *impl) : ::FCSL::FM::FaultModelElement(impl), UDM_OBJECT(impl) { }
			DIP2D(const DIP2D &master) : ::FCSL::FM::FaultModelElement(master), UDM_OBJECT(master) { }
			static DIP2D Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static DIP2D Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			DIP2D CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			DIP2D CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::FCSL::FM::DIP2D> Instances() { return Udm::InstantiatedAttr< ::FCSL::FM::DIP2D>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::FCSL::FM::DIP2D, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::FCSL::FM::DIP2D, Pred>(impl);}

			Udm::DerivedAttr< ::FCSL::FM::DIP2D> Derived() { return Udm::DerivedAttr< ::FCSL::FM::DIP2D>(impl);}
			template <class Pred> Udm::DerivedAttr< ::FCSL::FM::DIP2D, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::FCSL::FM::DIP2D, Pred>(impl);}

			Udm::ArchetypeAttr< ::FCSL::FM::DIP2D> Archetype() const { return Udm::ArchetypeAttr< ::FCSL::FM::DIP2D>(impl);}

			Udm::ParentAttr< ::FCSL::MgaObject> parent() const { return Udm::ParentAttr< ::FCSL::MgaObject>(impl, Udm::NULLPARENTROLE); }
			static ::Uml::AssociationRole meta_dstDIP2D_end_;
			Udm::AssocEndAttr< ::FCSL::FM::Discrepancy> dstDIP2D_end() const { return Udm::AssocEndAttr< ::FCSL::FM::Discrepancy>(impl, meta_dstDIP2D_end_); }

			static ::Uml::AssociationRole meta_srcDIP2D_end_;
			Udm::AssocEndAttr< ::FCSL::FM::DiscrepancyInPort> srcDIP2D_end() const { return Udm::AssocEndAttr< ::FCSL::FM::DiscrepancyInPort>(impl, meta_srcDIP2D_end_); }

		};

		class  Discrepancy :  public ::FCSL::FM::FaultModelElement {
		public:
			static ::Uml::Class meta;

			Discrepancy() { }
			Discrepancy(Udm::ObjectImpl *impl) : ::FCSL::FM::FaultModelElement(impl), UDM_OBJECT(impl) { }
			Discrepancy(const Discrepancy &master) : ::FCSL::FM::FaultModelElement(master), UDM_OBJECT(master) { }
			static Discrepancy Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static Discrepancy Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			Discrepancy CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			Discrepancy CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::FCSL::FM::Discrepancy> Instances() { return Udm::InstantiatedAttr< ::FCSL::FM::Discrepancy>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::FCSL::FM::Discrepancy, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::FCSL::FM::Discrepancy, Pred>(impl);}

			Udm::DerivedAttr< ::FCSL::FM::Discrepancy> Derived() { return Udm::DerivedAttr< ::FCSL::FM::Discrepancy>(impl);}
			template <class Pred> Udm::DerivedAttr< ::FCSL::FM::Discrepancy, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::FCSL::FM::Discrepancy, Pred>(impl);}

			Udm::ArchetypeAttr< ::FCSL::FM::Discrepancy> Archetype() const { return Udm::ArchetypeAttr< ::FCSL::FM::Discrepancy>(impl);}

			static ::Uml::Attribute meta_DiscrepancyType;
			Udm::StringAttr DiscrepancyType() const { return Udm::StringAttr(impl, meta_DiscrepancyType); }

			static ::Uml::AssociationRole meta_srcFM2D;
			static ::Uml::AssociationRole meta_srcFM2D_rev;
			Udm::AClassAssocAttr< ::FCSL::FM::FM2D, ::FCSL::FM::FailureMode> srcFM2D() const { return Udm::AClassAssocAttr< ::FCSL::FM::FM2D, ::FCSL::FM::FailureMode>(impl, meta_srcFM2D, meta_srcFM2D_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::FCSL::FM::FM2D, ::FCSL::FM::FailureMode, Pred> srcFM2D_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::FCSL::FM::FM2D, ::FCSL::FM::FailureMode, Pred>(impl, meta_srcFM2D, meta_srcFM2D_rev); }

			static ::Uml::AssociationRole meta_dstD2Test;
			static ::Uml::AssociationRole meta_dstD2Test_rev;
			Udm::AClassAssocAttr< ::FCSL::FM::D2Test, ::FCSL::FM::TestInstance> dstD2Test() const { return Udm::AClassAssocAttr< ::FCSL::FM::D2Test, ::FCSL::FM::TestInstance>(impl, meta_dstD2Test, meta_dstD2Test_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::FCSL::FM::D2Test, ::FCSL::FM::TestInstance, Pred> dstD2Test_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::FCSL::FM::D2Test, ::FCSL::FM::TestInstance, Pred>(impl, meta_dstD2Test, meta_dstD2Test_rev); }

			static ::Uml::AssociationRole meta_dstD2DOP;
			static ::Uml::AssociationRole meta_dstD2DOP_rev;
			Udm::AClassAssocAttr< ::FCSL::FM::D2DOP, ::FCSL::FM::DiscrepancyOutPort> dstD2DOP() const { return Udm::AClassAssocAttr< ::FCSL::FM::D2DOP, ::FCSL::FM::DiscrepancyOutPort>(impl, meta_dstD2DOP, meta_dstD2DOP_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::FCSL::FM::D2DOP, ::FCSL::FM::DiscrepancyOutPort, Pred> dstD2DOP_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::FCSL::FM::D2DOP, ::FCSL::FM::DiscrepancyOutPort, Pred>(impl, meta_dstD2DOP, meta_dstD2DOP_rev); }

			static ::Uml::AssociationRole meta_dstFailureProp;
			static ::Uml::AssociationRole meta_dstFailureProp_rev;
			Udm::AClassAssocAttr< ::FCSL::FM::FailureProp, ::FCSL::FM::Discrepancy> dstFailureProp() const { return Udm::AClassAssocAttr< ::FCSL::FM::FailureProp, ::FCSL::FM::Discrepancy>(impl, meta_dstFailureProp, meta_dstFailureProp_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::FCSL::FM::FailureProp, ::FCSL::FM::Discrepancy, Pred> dstFailureProp_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::FCSL::FM::FailureProp, ::FCSL::FM::Discrepancy, Pred>(impl, meta_dstFailureProp, meta_dstFailureProp_rev); }

			static ::Uml::AssociationRole meta_srcFailureProp;
			static ::Uml::AssociationRole meta_srcFailureProp_rev;
			Udm::AClassAssocAttr< ::FCSL::FM::FailureProp, ::FCSL::FM::Discrepancy> srcFailureProp() const { return Udm::AClassAssocAttr< ::FCSL::FM::FailureProp, ::FCSL::FM::Discrepancy>(impl, meta_srcFailureProp, meta_srcFailureProp_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::FCSL::FM::FailureProp, ::FCSL::FM::Discrepancy, Pred> srcFailureProp_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::FCSL::FM::FailureProp, ::FCSL::FM::Discrepancy, Pred>(impl, meta_srcFailureProp, meta_srcFailureProp_rev); }

			static ::Uml::AssociationRole meta_srcDOP2D;
			static ::Uml::AssociationRole meta_srcDOP2D_rev;
			Udm::AClassAssocAttr< ::FCSL::FM::DOP2D, ::FCSL::FM::DiscrepancyOutPort> srcDOP2D() const { return Udm::AClassAssocAttr< ::FCSL::FM::DOP2D, ::FCSL::FM::DiscrepancyOutPort>(impl, meta_srcDOP2D, meta_srcDOP2D_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::FCSL::FM::DOP2D, ::FCSL::FM::DiscrepancyOutPort, Pred> srcDOP2D_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::FCSL::FM::DOP2D, ::FCSL::FM::DiscrepancyOutPort, Pred>(impl, meta_srcDOP2D, meta_srcDOP2D_rev); }

			static ::Uml::AssociationRole meta_srcDIP2D;
			static ::Uml::AssociationRole meta_srcDIP2D_rev;
			Udm::AClassAssocAttr< ::FCSL::FM::DIP2D, ::FCSL::FM::DiscrepancyInPort> srcDIP2D() const { return Udm::AClassAssocAttr< ::FCSL::FM::DIP2D, ::FCSL::FM::DiscrepancyInPort>(impl, meta_srcDIP2D, meta_srcDIP2D_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::FCSL::FM::DIP2D, ::FCSL::FM::DiscrepancyInPort, Pred> srcDIP2D_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::FCSL::FM::DIP2D, ::FCSL::FM::DiscrepancyInPort, Pred>(impl, meta_srcDIP2D, meta_srcDIP2D_rev); }

			Udm::ParentAttr< ::FCSL::MgaObject> parent() const { return Udm::ParentAttr< ::FCSL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  FM2D :  public ::FCSL::FM::FaultModelElement {
		public:
			static ::Uml::Class meta;

			FM2D() { }
			FM2D(Udm::ObjectImpl *impl) : ::FCSL::FM::FaultModelElement(impl), UDM_OBJECT(impl) { }
			FM2D(const FM2D &master) : ::FCSL::FM::FaultModelElement(master), UDM_OBJECT(master) { }
			static FM2D Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static FM2D Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			FM2D CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			FM2D CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::FCSL::FM::FM2D> Instances() { return Udm::InstantiatedAttr< ::FCSL::FM::FM2D>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::FCSL::FM::FM2D, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::FCSL::FM::FM2D, Pred>(impl);}

			Udm::DerivedAttr< ::FCSL::FM::FM2D> Derived() { return Udm::DerivedAttr< ::FCSL::FM::FM2D>(impl);}
			template <class Pred> Udm::DerivedAttr< ::FCSL::FM::FM2D, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::FCSL::FM::FM2D, Pred>(impl);}

			Udm::ArchetypeAttr< ::FCSL::FM::FM2D> Archetype() const { return Udm::ArchetypeAttr< ::FCSL::FM::FM2D>(impl);}

			Udm::ParentAttr< ::FCSL::MgaObject> parent() const { return Udm::ParentAttr< ::FCSL::MgaObject>(impl, Udm::NULLPARENTROLE); }
			static ::Uml::AssociationRole meta_srcFM2D_end_;
			Udm::AssocEndAttr< ::FCSL::FM::FailureMode> srcFM2D_end() const { return Udm::AssocEndAttr< ::FCSL::FM::FailureMode>(impl, meta_srcFM2D_end_); }

			static ::Uml::AssociationRole meta_dstFM2D_end_;
			Udm::AssocEndAttr< ::FCSL::FM::Discrepancy> dstFM2D_end() const { return Udm::AssocEndAttr< ::FCSL::FM::Discrepancy>(impl, meta_dstFM2D_end_); }

		};

		class  SignalflowConn :  public ::FCSL::FM::FaultModelElement {
		public:
			static ::Uml::Class meta;

			SignalflowConn() { }
			SignalflowConn(Udm::ObjectImpl *impl) : ::FCSL::FM::FaultModelElement(impl), UDM_OBJECT(impl) { }
			SignalflowConn(const SignalflowConn &master) : ::FCSL::FM::FaultModelElement(master), UDM_OBJECT(master) { }
			static SignalflowConn Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static SignalflowConn Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			SignalflowConn CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			SignalflowConn CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::FCSL::FM::SignalflowConn> Instances() { return Udm::InstantiatedAttr< ::FCSL::FM::SignalflowConn>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::FCSL::FM::SignalflowConn, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::FCSL::FM::SignalflowConn, Pred>(impl);}

			Udm::DerivedAttr< ::FCSL::FM::SignalflowConn> Derived() { return Udm::DerivedAttr< ::FCSL::FM::SignalflowConn>(impl);}
			template <class Pred> Udm::DerivedAttr< ::FCSL::FM::SignalflowConn, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::FCSL::FM::SignalflowConn, Pred>(impl);}

			Udm::ArchetypeAttr< ::FCSL::FM::SignalflowConn> Archetype() const { return Udm::ArchetypeAttr< ::FCSL::FM::SignalflowConn>(impl);}

			static ::Uml::CompositionParentRole meta_Platform_parent;
			Udm::ParentAttr< ::FCSL::FM::Platform> Platform_parent() const { return Udm::ParentAttr< ::FCSL::FM::Platform>(impl, meta_Platform_parent); }

			Udm::ParentAttr< ::FCSL::MgaObject> parent() const { return Udm::ParentAttr< ::FCSL::MgaObject>(impl, Udm::NULLPARENTROLE); }
			static ::Uml::AssociationRole meta_dstSignalflowConn_end_;
			Udm::AssocEndAttr< ::FCSL::FM::DataPort> dstSignalflowConn_end() const { return Udm::AssocEndAttr< ::FCSL::FM::DataPort>(impl, meta_dstSignalflowConn_end_); }

			static ::Uml::AssociationRole meta_srcSignalflowConn_end_;
			Udm::AssocEndAttr< ::FCSL::FM::DataPort> srcSignalflowConn_end() const { return Udm::AssocEndAttr< ::FCSL::FM::DataPort>(impl, meta_srcSignalflowConn_end_); }

		};

		class  DOP2T :  public ::FCSL::FM::FaultModelElement {
		public:
			static ::Uml::Class meta;

			DOP2T() { }
			DOP2T(Udm::ObjectImpl *impl) : ::FCSL::FM::FaultModelElement(impl), UDM_OBJECT(impl) { }
			DOP2T(const DOP2T &master) : ::FCSL::FM::FaultModelElement(master), UDM_OBJECT(master) { }
			static DOP2T Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static DOP2T Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			DOP2T CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			DOP2T CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::FCSL::FM::DOP2T> Instances() { return Udm::InstantiatedAttr< ::FCSL::FM::DOP2T>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::FCSL::FM::DOP2T, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::FCSL::FM::DOP2T, Pred>(impl);}

			Udm::DerivedAttr< ::FCSL::FM::DOP2T> Derived() { return Udm::DerivedAttr< ::FCSL::FM::DOP2T>(impl);}
			template <class Pred> Udm::DerivedAttr< ::FCSL::FM::DOP2T, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::FCSL::FM::DOP2T, Pred>(impl);}

			Udm::ArchetypeAttr< ::FCSL::FM::DOP2T> Archetype() const { return Udm::ArchetypeAttr< ::FCSL::FM::DOP2T>(impl);}

			Udm::ParentAttr< ::FCSL::MgaObject> parent() const { return Udm::ParentAttr< ::FCSL::MgaObject>(impl, Udm::NULLPARENTROLE); }
			static ::Uml::AssociationRole meta_srcDOP2T_end_;
			Udm::AssocEndAttr< ::FCSL::FM::DiscrepancyOutPort> srcDOP2T_end() const { return Udm::AssocEndAttr< ::FCSL::FM::DiscrepancyOutPort>(impl, meta_srcDOP2T_end_); }

			static ::Uml::AssociationRole meta_dstDOP2T_end_;
			Udm::AssocEndAttr< ::FCSL::FM::TestInstance> dstDOP2T_end() const { return Udm::AssocEndAttr< ::FCSL::FM::TestInstance>(impl, meta_dstDOP2T_end_); }

		};

		class  D2Test :  public ::FCSL::FM::FaultModelElement {
		public:
			static ::Uml::Class meta;

			D2Test() { }
			D2Test(Udm::ObjectImpl *impl) : ::FCSL::FM::FaultModelElement(impl), UDM_OBJECT(impl) { }
			D2Test(const D2Test &master) : ::FCSL::FM::FaultModelElement(master), UDM_OBJECT(master) { }
			static D2Test Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static D2Test Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			D2Test CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			D2Test CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::FCSL::FM::D2Test> Instances() { return Udm::InstantiatedAttr< ::FCSL::FM::D2Test>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::FCSL::FM::D2Test, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::FCSL::FM::D2Test, Pred>(impl);}

			Udm::DerivedAttr< ::FCSL::FM::D2Test> Derived() { return Udm::DerivedAttr< ::FCSL::FM::D2Test>(impl);}
			template <class Pred> Udm::DerivedAttr< ::FCSL::FM::D2Test, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::FCSL::FM::D2Test, Pred>(impl);}

			Udm::ArchetypeAttr< ::FCSL::FM::D2Test> Archetype() const { return Udm::ArchetypeAttr< ::FCSL::FM::D2Test>(impl);}

			Udm::ParentAttr< ::FCSL::MgaObject> parent() const { return Udm::ParentAttr< ::FCSL::MgaObject>(impl, Udm::NULLPARENTROLE); }
			static ::Uml::AssociationRole meta_srcD2Test_end_;
			Udm::AssocEndAttr< ::FCSL::FM::Discrepancy> srcD2Test_end() const { return Udm::AssocEndAttr< ::FCSL::FM::Discrepancy>(impl, meta_srcD2Test_end_); }

			static ::Uml::AssociationRole meta_dstD2Test_end_;
			Udm::AssocEndAttr< ::FCSL::FM::TestInstance> dstD2Test_end() const { return Udm::AssocEndAttr< ::FCSL::FM::TestInstance>(impl, meta_dstD2Test_end_); }

		};

		class  FailureModeRef :  public ::FCSL::MgaObject {
		public:
			static ::Uml::Class meta;

			FailureModeRef() { }
			FailureModeRef(Udm::ObjectImpl *impl) : ::FCSL::MgaObject(impl), UDM_OBJECT(impl) { }
			FailureModeRef(const FailureModeRef &master) : ::FCSL::MgaObject(master), UDM_OBJECT(master) { }
			static FailureModeRef Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static FailureModeRef Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			FailureModeRef CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			FailureModeRef CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::FCSL::FM::FailureModeRef> Instances() { return Udm::InstantiatedAttr< ::FCSL::FM::FailureModeRef>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::FCSL::FM::FailureModeRef, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::FCSL::FM::FailureModeRef, Pred>(impl);}

			Udm::DerivedAttr< ::FCSL::FM::FailureModeRef> Derived() { return Udm::DerivedAttr< ::FCSL::FM::FailureModeRef>(impl);}
			template <class Pred> Udm::DerivedAttr< ::FCSL::FM::FailureModeRef, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::FCSL::FM::FailureModeRef, Pred>(impl);}

			Udm::ArchetypeAttr< ::FCSL::FM::FailureModeRef> Archetype() const { return Udm::ArchetypeAttr< ::FCSL::FM::FailureModeRef>(impl);}

			static ::Uml::AssociationRole meta_ref;
			Udm::PointerAttr< ::FCSL::FM::FailureMode> ref() const { return Udm::PointerAttr< ::FCSL::FM::FailureMode>(impl, meta_ref); }

			static ::Uml::CompositionParentRole meta_Test_parent;
			Udm::ParentAttr< ::FCSL::FM::Test> Test_parent() const { return Udm::ParentAttr< ::FCSL::FM::Test>(impl, meta_Test_parent); }

			Udm::ParentAttr< ::FCSL::MgaObject> parent() const { return Udm::ParentAttr< ::FCSL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  Platform :  public ::FCSL::MgaObject {
		public:
			static ::Uml::Class meta;

			Platform() { }
			Platform(Udm::ObjectImpl *impl) : ::FCSL::MgaObject(impl), UDM_OBJECT(impl) { }
			Platform(const Platform &master) : ::FCSL::MgaObject(master), UDM_OBJECT(master) { }
			static Platform Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static Platform Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			Platform CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			Platform CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::FCSL::FM::Platform> Instances() { return Udm::InstantiatedAttr< ::FCSL::FM::Platform>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::FCSL::FM::Platform, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::FCSL::FM::Platform, Pred>(impl);}

			Udm::DerivedAttr< ::FCSL::FM::Platform> Derived() { return Udm::DerivedAttr< ::FCSL::FM::Platform>(impl);}
			template <class Pred> Udm::DerivedAttr< ::FCSL::FM::Platform, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::FCSL::FM::Platform, Pred>(impl);}

			Udm::ArchetypeAttr< ::FCSL::FM::Platform> Archetype() const { return Udm::ArchetypeAttr< ::FCSL::FM::Platform>(impl);}

			static ::Uml::CompositionChildRole meta_SignalflowConn_children;
			Udm::ChildrenAttr< ::FCSL::FM::SignalflowConn> SignalflowConn_children() const { return Udm::ChildrenAttr< ::FCSL::FM::SignalflowConn>(impl, meta_SignalflowConn_children); }
			template <class Pred> Udm::ChildrenAttr< ::FCSL::FM::SignalflowConn, Pred> SignalflowConn_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::FM::SignalflowConn, Pred>(impl, meta_SignalflowConn_children); }

			static ::Uml::CompositionChildRole meta_DIP2DOP_children;
			Udm::ChildrenAttr< ::FCSL::FM::DIP2DOP> DIP2DOP_children() const { return Udm::ChildrenAttr< ::FCSL::FM::DIP2DOP>(impl, meta_DIP2DOP_children); }
			template <class Pred> Udm::ChildrenAttr< ::FCSL::FM::DIP2DOP, Pred> DIP2DOP_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::FM::DIP2DOP, Pred>(impl, meta_DIP2DOP_children); }

			static ::Uml::CompositionChildRole meta_Entity_children;
			Udm::ChildrenAttr< ::FCSL::FM::Entity> Entity_children() const { return Udm::ChildrenAttr< ::FCSL::FM::Entity>(impl, meta_Entity_children); }
			template <class Pred> Udm::ChildrenAttr< ::FCSL::FM::Entity, Pred> Entity_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::FM::Entity, Pred>(impl, meta_Entity_children); }

			Udm::ChildrenAttr< ::FCSL::FM::Device> Device_kind_children() const { return Udm::ChildrenAttr< ::FCSL::FM::Device>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::FM::Device, Pred> Device_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::FM::Device, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::FM::Assembly> Assembly_kind_children() const { return Udm::ChildrenAttr< ::FCSL::FM::Assembly>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::FM::Assembly, Pred> Assembly_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::FM::Assembly, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::FM::SignalflowConn> SignalflowConn_kind_children() const { return Udm::ChildrenAttr< ::FCSL::FM::SignalflowConn>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::FM::SignalflowConn, Pred> SignalflowConn_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::FM::SignalflowConn, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::FM::Surface> Surface_kind_children() const { return Udm::ChildrenAttr< ::FCSL::FM::Surface>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::FM::Surface, Pred> Surface_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::FM::Surface, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::FM::Control> Control_kind_children() const { return Udm::ChildrenAttr< ::FCSL::FM::Control>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::FM::Control, Pred> Control_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::FM::Control, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::FM::FaultModelElement> FaultModelElement_kind_children() const { return Udm::ChildrenAttr< ::FCSL::FM::FaultModelElement>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::FM::FaultModelElement, Pred> FaultModelElement_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::FM::FaultModelElement, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::FM::Entity> Entity_kind_children() const { return Udm::ChildrenAttr< ::FCSL::FM::Entity>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::FM::Entity, Pred> Entity_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::FM::Entity, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::FM::Unit> Unit_kind_children() const { return Udm::ChildrenAttr< ::FCSL::FM::Unit>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::FM::Unit, Pred> Unit_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::FM::Unit, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::FM::DIP2DOP> DIP2DOP_kind_children() const { return Udm::ChildrenAttr< ::FCSL::FM::DIP2DOP>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::FM::DIP2DOP, Pred> DIP2DOP_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::FM::DIP2DOP, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::FM::Signal> Signal_kind_children() const { return Udm::ChildrenAttr< ::FCSL::FM::Signal>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::FM::Signal, Pred> Signal_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::FM::Signal, Pred>(impl, Udm::NULLCHILDROLE); }

			static ::Uml::CompositionParentRole meta_PlatformModels_parent;
			Udm::ParentAttr< ::FCSL::FM::PlatformModels> PlatformModels_parent() const { return Udm::ParentAttr< ::FCSL::FM::PlatformModels>(impl, meta_PlatformModels_parent); }

			Udm::ParentAttr< ::FCSL::FM::PlatformModels> parent() const { return Udm::ParentAttr< ::FCSL::FM::PlatformModels>(impl, Udm::NULLPARENTROLE); }
		};

		class  DOP2D :  public ::FCSL::FM::FaultModelElement {
		public:
			static ::Uml::Class meta;

			DOP2D() { }
			DOP2D(Udm::ObjectImpl *impl) : ::FCSL::FM::FaultModelElement(impl), UDM_OBJECT(impl) { }
			DOP2D(const DOP2D &master) : ::FCSL::FM::FaultModelElement(master), UDM_OBJECT(master) { }
			static DOP2D Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static DOP2D Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			DOP2D CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			DOP2D CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::FCSL::FM::DOP2D> Instances() { return Udm::InstantiatedAttr< ::FCSL::FM::DOP2D>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::FCSL::FM::DOP2D, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::FCSL::FM::DOP2D, Pred>(impl);}

			Udm::DerivedAttr< ::FCSL::FM::DOP2D> Derived() { return Udm::DerivedAttr< ::FCSL::FM::DOP2D>(impl);}
			template <class Pred> Udm::DerivedAttr< ::FCSL::FM::DOP2D, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::FCSL::FM::DOP2D, Pred>(impl);}

			Udm::ArchetypeAttr< ::FCSL::FM::DOP2D> Archetype() const { return Udm::ArchetypeAttr< ::FCSL::FM::DOP2D>(impl);}

			Udm::ParentAttr< ::FCSL::MgaObject> parent() const { return Udm::ParentAttr< ::FCSL::MgaObject>(impl, Udm::NULLPARENTROLE); }
			static ::Uml::AssociationRole meta_dstDOP2D_end_;
			Udm::AssocEndAttr< ::FCSL::FM::Discrepancy> dstDOP2D_end() const { return Udm::AssocEndAttr< ::FCSL::FM::Discrepancy>(impl, meta_dstDOP2D_end_); }

			static ::Uml::AssociationRole meta_srcDOP2D_end_;
			Udm::AssocEndAttr< ::FCSL::FM::DiscrepancyOutPort> srcDOP2D_end() const { return Udm::AssocEndAttr< ::FCSL::FM::DiscrepancyOutPort>(impl, meta_srcDOP2D_end_); }

		};

		class  D2DOP :  public ::FCSL::FM::FaultModelElement {
		public:
			static ::Uml::Class meta;

			D2DOP() { }
			D2DOP(Udm::ObjectImpl *impl) : ::FCSL::FM::FaultModelElement(impl), UDM_OBJECT(impl) { }
			D2DOP(const D2DOP &master) : ::FCSL::FM::FaultModelElement(master), UDM_OBJECT(master) { }
			static D2DOP Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static D2DOP Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			D2DOP CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			D2DOP CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::FCSL::FM::D2DOP> Instances() { return Udm::InstantiatedAttr< ::FCSL::FM::D2DOP>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::FCSL::FM::D2DOP, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::FCSL::FM::D2DOP, Pred>(impl);}

			Udm::DerivedAttr< ::FCSL::FM::D2DOP> Derived() { return Udm::DerivedAttr< ::FCSL::FM::D2DOP>(impl);}
			template <class Pred> Udm::DerivedAttr< ::FCSL::FM::D2DOP, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::FCSL::FM::D2DOP, Pred>(impl);}

			Udm::ArchetypeAttr< ::FCSL::FM::D2DOP> Archetype() const { return Udm::ArchetypeAttr< ::FCSL::FM::D2DOP>(impl);}

			Udm::ParentAttr< ::FCSL::MgaObject> parent() const { return Udm::ParentAttr< ::FCSL::MgaObject>(impl, Udm::NULLPARENTROLE); }
			static ::Uml::AssociationRole meta_srcD2DOP_end_;
			Udm::AssocEndAttr< ::FCSL::FM::Discrepancy> srcD2DOP_end() const { return Udm::AssocEndAttr< ::FCSL::FM::Discrepancy>(impl, meta_srcD2DOP_end_); }

			static ::Uml::AssociationRole meta_dstD2DOP_end_;
			Udm::AssocEndAttr< ::FCSL::FM::DiscrepancyOutPort> dstD2DOP_end() const { return Udm::AssocEndAttr< ::FCSL::FM::DiscrepancyOutPort>(impl, meta_dstD2DOP_end_); }

		};

		class  Entity :  public ::FCSL::MgaObject {
		public:
			static ::Uml::Class meta;

			Entity() { }
			Entity(Udm::ObjectImpl *impl) : ::FCSL::MgaObject(impl), UDM_OBJECT(impl) { }
			Entity(const Entity &master) : ::FCSL::MgaObject(master), UDM_OBJECT(master) { }
			static Entity Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static Entity Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			Entity CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			Entity CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::FCSL::FM::Entity> Instances() { return Udm::InstantiatedAttr< ::FCSL::FM::Entity>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::FCSL::FM::Entity, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::FCSL::FM::Entity, Pred>(impl);}

			Udm::DerivedAttr< ::FCSL::FM::Entity> Derived() { return Udm::DerivedAttr< ::FCSL::FM::Entity>(impl);}
			template <class Pred> Udm::DerivedAttr< ::FCSL::FM::Entity, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::FCSL::FM::Entity, Pred>(impl);}

			Udm::ArchetypeAttr< ::FCSL::FM::Entity> Archetype() const { return Udm::ArchetypeAttr< ::FCSL::FM::Entity>(impl);}

			static ::Uml::CompositionParentRole meta_Platform_parent;
			Udm::ParentAttr< ::FCSL::FM::Platform> Platform_parent() const { return Udm::ParentAttr< ::FCSL::FM::Platform>(impl, meta_Platform_parent); }

			Udm::ParentAttr< ::FCSL::MgaObject> parent() const { return Udm::ParentAttr< ::FCSL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  Assembly :  public ::FCSL::FM::Entity {
		public:
			static ::Uml::Class meta;

			Assembly() { }
			Assembly(Udm::ObjectImpl *impl) : ::FCSL::FM::Entity(impl), UDM_OBJECT(impl) { }
			Assembly(const Assembly &master) : ::FCSL::FM::Entity(master), UDM_OBJECT(master) { }
			static Assembly Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static Assembly Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			Assembly CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			Assembly CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::FCSL::FM::Assembly> Instances() { return Udm::InstantiatedAttr< ::FCSL::FM::Assembly>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::FCSL::FM::Assembly, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::FCSL::FM::Assembly, Pred>(impl);}

			Udm::DerivedAttr< ::FCSL::FM::Assembly> Derived() { return Udm::DerivedAttr< ::FCSL::FM::Assembly>(impl);}
			template <class Pred> Udm::DerivedAttr< ::FCSL::FM::Assembly, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::FCSL::FM::Assembly, Pred>(impl);}

			Udm::ArchetypeAttr< ::FCSL::FM::Assembly> Archetype() const { return Udm::ArchetypeAttr< ::FCSL::FM::Assembly>(impl);}

			static ::Uml::CompositionChildRole meta_DataPort_children;
			Udm::ChildrenAttr< ::FCSL::FM::DataPort> DataPort_children() const { return Udm::ChildrenAttr< ::FCSL::FM::DataPort>(impl, meta_DataPort_children); }
			template <class Pred> Udm::ChildrenAttr< ::FCSL::FM::DataPort, Pred> DataPort_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::FM::DataPort, Pred>(impl, meta_DataPort_children); }

			static ::Uml::CompositionChildRole meta_Device2Surface_children;
			Udm::ChildrenAttr< ::FCSL::FM::Device2Surface> Device2Surface_children() const { return Udm::ChildrenAttr< ::FCSL::FM::Device2Surface>(impl, meta_Device2Surface_children); }
			template <class Pred> Udm::ChildrenAttr< ::FCSL::FM::Device2Surface, Pred> Device2Surface_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::FM::Device2Surface, Pred>(impl, meta_Device2Surface_children); }

			static ::Uml::CompositionChildRole meta_Ctrl2Device_children;
			Udm::ChildrenAttr< ::FCSL::FM::Ctrl2Device> Ctrl2Device_children() const { return Udm::ChildrenAttr< ::FCSL::FM::Ctrl2Device>(impl, meta_Ctrl2Device_children); }
			template <class Pred> Udm::ChildrenAttr< ::FCSL::FM::Ctrl2Device, Pred> Ctrl2Device_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::FM::Ctrl2Device, Pred>(impl, meta_Ctrl2Device_children); }

			static ::Uml::CompositionChildRole meta_Assembly_children;
			Udm::ChildrenAttr< ::FCSL::FM::Assembly> Assembly_children() const { return Udm::ChildrenAttr< ::FCSL::FM::Assembly>(impl, meta_Assembly_children); }
			template <class Pred> Udm::ChildrenAttr< ::FCSL::FM::Assembly, Pred> Assembly_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::FM::Assembly, Pred>(impl, meta_Assembly_children); }

			static ::Uml::CompositionChildRole meta_Unit_children;
			Udm::ChildrenAttr< ::FCSL::FM::Unit> Unit_children() const { return Udm::ChildrenAttr< ::FCSL::FM::Unit>(impl, meta_Unit_children); }
			template <class Pred> Udm::ChildrenAttr< ::FCSL::FM::Unit, Pred> Unit_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::FM::Unit, Pred>(impl, meta_Unit_children); }

			static ::Uml::CompositionChildRole meta_FaultModelElementPort_children;
			Udm::ChildrenAttr< ::FCSL::FM::FaultModelElementPort> FaultModelElementPort_children() const { return Udm::ChildrenAttr< ::FCSL::FM::FaultModelElementPort>(impl, meta_FaultModelElementPort_children); }
			template <class Pred> Udm::ChildrenAttr< ::FCSL::FM::FaultModelElementPort, Pred> FaultModelElementPort_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::FM::FaultModelElementPort, Pred>(impl, meta_FaultModelElementPort_children); }

			static ::Uml::CompositionChildRole meta_FaultModelElement_children;
			Udm::ChildrenAttr< ::FCSL::FM::FaultModelElement> FaultModelElement_children() const { return Udm::ChildrenAttr< ::FCSL::FM::FaultModelElement>(impl, meta_FaultModelElement_children); }
			template <class Pred> Udm::ChildrenAttr< ::FCSL::FM::FaultModelElement, Pred> FaultModelElement_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::FM::FaultModelElement, Pred>(impl, meta_FaultModelElement_children); }

			Udm::ChildrenAttr< ::FCSL::FM::FM2DOP> FM2DOP_kind_children() const { return Udm::ChildrenAttr< ::FCSL::FM::FM2DOP>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::FM::FM2DOP, Pred> FM2DOP_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::FM::FM2DOP, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::FM::Device> Device_kind_children() const { return Udm::ChildrenAttr< ::FCSL::FM::Device>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::FM::Device, Pred> Device_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::FM::Device, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::FM::Device2Surface> Device2Surface_kind_children() const { return Udm::ChildrenAttr< ::FCSL::FM::Device2Surface>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::FM::Device2Surface, Pred> Device2Surface_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::FM::Device2Surface, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::FM::DiscrepancyPort> DiscrepancyPort_kind_children() const { return Udm::ChildrenAttr< ::FCSL::FM::DiscrepancyPort>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::FM::DiscrepancyPort, Pred> DiscrepancyPort_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::FM::DiscrepancyPort, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::FM::FailureMode> FailureMode_kind_children() const { return Udm::ChildrenAttr< ::FCSL::FM::FailureMode>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::FM::FailureMode, Pred> FailureMode_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::FM::FailureMode, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::FM::Assembly> Assembly_kind_children() const { return Udm::ChildrenAttr< ::FCSL::FM::Assembly>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::FM::Assembly, Pred> Assembly_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::FM::Assembly, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::FM::DIP2D> DIP2D_kind_children() const { return Udm::ChildrenAttr< ::FCSL::FM::DIP2D>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::FM::DIP2D, Pred> DIP2D_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::FM::DIP2D, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::FM::Discrepancy> Discrepancy_kind_children() const { return Udm::ChildrenAttr< ::FCSL::FM::Discrepancy>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::FM::Discrepancy, Pred> Discrepancy_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::FM::Discrepancy, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::FM::FM2D> FM2D_kind_children() const { return Udm::ChildrenAttr< ::FCSL::FM::FM2D>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::FM::FM2D, Pred> FM2D_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::FM::FM2D, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::FM::DiscrepancyOutPort> DiscrepancyOutPort_kind_children() const { return Udm::ChildrenAttr< ::FCSL::FM::DiscrepancyOutPort>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::FM::DiscrepancyOutPort, Pred> DiscrepancyOutPort_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::FM::DiscrepancyOutPort, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::FM::SignalflowConn> SignalflowConn_kind_children() const { return Udm::ChildrenAttr< ::FCSL::FM::SignalflowConn>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::FM::SignalflowConn, Pred> SignalflowConn_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::FM::SignalflowConn, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::FM::DOP2T> DOP2T_kind_children() const { return Udm::ChildrenAttr< ::FCSL::FM::DOP2T>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::FM::DOP2T, Pred> DOP2T_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::FM::DOP2T, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::FM::Surface> Surface_kind_children() const { return Udm::ChildrenAttr< ::FCSL::FM::Surface>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::FM::Surface, Pred> Surface_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::FM::Surface, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::FM::DataOut> DataOut_kind_children() const { return Udm::ChildrenAttr< ::FCSL::FM::DataOut>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::FM::DataOut, Pred> DataOut_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::FM::DataOut, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::FM::Ctrl2Device> Ctrl2Device_kind_children() const { return Udm::ChildrenAttr< ::FCSL::FM::Ctrl2Device>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::FM::Ctrl2Device, Pred> Ctrl2Device_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::FM::Ctrl2Device, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::FM::FaultModelElementPort> FaultModelElementPort_kind_children() const { return Udm::ChildrenAttr< ::FCSL::FM::FaultModelElementPort>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::FM::FaultModelElementPort, Pred> FaultModelElementPort_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::FM::FaultModelElementPort, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::FM::DataIn> DataIn_kind_children() const { return Udm::ChildrenAttr< ::FCSL::FM::DataIn>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::FM::DataIn, Pred> DataIn_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::FM::DataIn, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::FM::D2Test> D2Test_kind_children() const { return Udm::ChildrenAttr< ::FCSL::FM::D2Test>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::FM::D2Test, Pred> D2Test_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::FM::D2Test, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::FM::Control> Control_kind_children() const { return Udm::ChildrenAttr< ::FCSL::FM::Control>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::FM::Control, Pred> Control_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::FM::Control, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::FM::FaultModelElement> FaultModelElement_kind_children() const { return Udm::ChildrenAttr< ::FCSL::FM::FaultModelElement>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::FM::FaultModelElement, Pred> FaultModelElement_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::FM::FaultModelElement, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::FM::DOP2D> DOP2D_kind_children() const { return Udm::ChildrenAttr< ::FCSL::FM::DOP2D>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::FM::DOP2D, Pred> DOP2D_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::FM::DOP2D, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::FM::D2DOP> D2DOP_kind_children() const { return Udm::ChildrenAttr< ::FCSL::FM::D2DOP>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::FM::D2DOP, Pred> D2DOP_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::FM::D2DOP, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::FM::Entity> Entity_kind_children() const { return Udm::ChildrenAttr< ::FCSL::FM::Entity>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::FM::Entity, Pred> Entity_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::FM::Entity, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::FM::Unit> Unit_kind_children() const { return Udm::ChildrenAttr< ::FCSL::FM::Unit>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::FM::Unit, Pred> Unit_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::FM::Unit, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::FM::DiscrepancyInPort> DiscrepancyInPort_kind_children() const { return Udm::ChildrenAttr< ::FCSL::FM::DiscrepancyInPort>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::FM::DiscrepancyInPort, Pred> DiscrepancyInPort_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::FM::DiscrepancyInPort, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::FM::DataPort> DataPort_kind_children() const { return Udm::ChildrenAttr< ::FCSL::FM::DataPort>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::FM::DataPort, Pred> DataPort_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::FM::DataPort, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::FM::FailureProp> FailureProp_kind_children() const { return Udm::ChildrenAttr< ::FCSL::FM::FailureProp>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::FM::FailureProp, Pred> FailureProp_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::FM::FailureProp, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::FM::DIP2DOP> DIP2DOP_kind_children() const { return Udm::ChildrenAttr< ::FCSL::FM::DIP2DOP>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::FM::DIP2DOP, Pred> DIP2DOP_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::FM::DIP2DOP, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::FM::TestInstance> TestInstance_kind_children() const { return Udm::ChildrenAttr< ::FCSL::FM::TestInstance>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::FM::TestInstance, Pred> TestInstance_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::FM::TestInstance, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::FM::Signal> Signal_kind_children() const { return Udm::ChildrenAttr< ::FCSL::FM::Signal>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::FM::Signal, Pred> Signal_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::FM::Signal, Pred>(impl, Udm::NULLCHILDROLE); }

			static ::Uml::CompositionParentRole meta_Assembly_parent;
			Udm::ParentAttr< ::FCSL::FM::Assembly> Assembly_parent() const { return Udm::ParentAttr< ::FCSL::FM::Assembly>(impl, meta_Assembly_parent); }

			Udm::ParentAttr< ::FCSL::MgaObject> parent() const { return Udm::ParentAttr< ::FCSL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  Unit :  public ::FCSL::FM::Entity {
		public:
			static ::Uml::Class meta;

			Unit() { }
			Unit(Udm::ObjectImpl *impl) : ::FCSL::FM::Entity(impl), UDM_OBJECT(impl) { }
			Unit(const Unit &master) : ::FCSL::FM::Entity(master), UDM_OBJECT(master) { }
			static Unit Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static Unit Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			Unit CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			Unit CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::FCSL::FM::Unit> Instances() { return Udm::InstantiatedAttr< ::FCSL::FM::Unit>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::FCSL::FM::Unit, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::FCSL::FM::Unit, Pred>(impl);}

			Udm::DerivedAttr< ::FCSL::FM::Unit> Derived() { return Udm::DerivedAttr< ::FCSL::FM::Unit>(impl);}
			template <class Pred> Udm::DerivedAttr< ::FCSL::FM::Unit, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::FCSL::FM::Unit, Pred>(impl);}

			Udm::ArchetypeAttr< ::FCSL::FM::Unit> Archetype() const { return Udm::ArchetypeAttr< ::FCSL::FM::Unit>(impl);}

			static ::Uml::CompositionChildRole meta_FaultModelElement_children;
			Udm::ChildrenAttr< ::FCSL::FM::FaultModelElement> FaultModelElement_children() const { return Udm::ChildrenAttr< ::FCSL::FM::FaultModelElement>(impl, meta_FaultModelElement_children); }
			template <class Pred> Udm::ChildrenAttr< ::FCSL::FM::FaultModelElement, Pred> FaultModelElement_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::FM::FaultModelElement, Pred>(impl, meta_FaultModelElement_children); }

			static ::Uml::CompositionChildRole meta_FaultModelElementPort_children;
			Udm::ChildrenAttr< ::FCSL::FM::FaultModelElementPort> FaultModelElementPort_children() const { return Udm::ChildrenAttr< ::FCSL::FM::FaultModelElementPort>(impl, meta_FaultModelElementPort_children); }
			template <class Pred> Udm::ChildrenAttr< ::FCSL::FM::FaultModelElementPort, Pred> FaultModelElementPort_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::FM::FaultModelElementPort, Pred>(impl, meta_FaultModelElementPort_children); }

			Udm::ChildrenAttr< ::FCSL::FM::FM2DOP> FM2DOP_kind_children() const { return Udm::ChildrenAttr< ::FCSL::FM::FM2DOP>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::FM::FM2DOP, Pred> FM2DOP_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::FM::FM2DOP, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::FM::DiscrepancyPort> DiscrepancyPort_kind_children() const { return Udm::ChildrenAttr< ::FCSL::FM::DiscrepancyPort>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::FM::DiscrepancyPort, Pred> DiscrepancyPort_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::FM::DiscrepancyPort, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::FM::FailureMode> FailureMode_kind_children() const { return Udm::ChildrenAttr< ::FCSL::FM::FailureMode>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::FM::FailureMode, Pred> FailureMode_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::FM::FailureMode, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::FM::DIP2D> DIP2D_kind_children() const { return Udm::ChildrenAttr< ::FCSL::FM::DIP2D>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::FM::DIP2D, Pred> DIP2D_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::FM::DIP2D, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::FM::Discrepancy> Discrepancy_kind_children() const { return Udm::ChildrenAttr< ::FCSL::FM::Discrepancy>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::FM::Discrepancy, Pred> Discrepancy_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::FM::Discrepancy, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::FM::FM2D> FM2D_kind_children() const { return Udm::ChildrenAttr< ::FCSL::FM::FM2D>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::FM::FM2D, Pred> FM2D_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::FM::FM2D, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::FM::DiscrepancyOutPort> DiscrepancyOutPort_kind_children() const { return Udm::ChildrenAttr< ::FCSL::FM::DiscrepancyOutPort>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::FM::DiscrepancyOutPort, Pred> DiscrepancyOutPort_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::FM::DiscrepancyOutPort, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::FM::SignalflowConn> SignalflowConn_kind_children() const { return Udm::ChildrenAttr< ::FCSL::FM::SignalflowConn>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::FM::SignalflowConn, Pred> SignalflowConn_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::FM::SignalflowConn, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::FM::DOP2T> DOP2T_kind_children() const { return Udm::ChildrenAttr< ::FCSL::FM::DOP2T>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::FM::DOP2T, Pred> DOP2T_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::FM::DOP2T, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::FM::FaultModelElementPort> FaultModelElementPort_kind_children() const { return Udm::ChildrenAttr< ::FCSL::FM::FaultModelElementPort>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::FM::FaultModelElementPort, Pred> FaultModelElementPort_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::FM::FaultModelElementPort, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::FM::D2Test> D2Test_kind_children() const { return Udm::ChildrenAttr< ::FCSL::FM::D2Test>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::FM::D2Test, Pred> D2Test_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::FM::D2Test, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::FM::FaultModelElement> FaultModelElement_kind_children() const { return Udm::ChildrenAttr< ::FCSL::FM::FaultModelElement>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::FM::FaultModelElement, Pred> FaultModelElement_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::FM::FaultModelElement, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::FM::DOP2D> DOP2D_kind_children() const { return Udm::ChildrenAttr< ::FCSL::FM::DOP2D>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::FM::DOP2D, Pred> DOP2D_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::FM::DOP2D, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::FM::D2DOP> D2DOP_kind_children() const { return Udm::ChildrenAttr< ::FCSL::FM::D2DOP>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::FM::D2DOP, Pred> D2DOP_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::FM::D2DOP, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::FM::DiscrepancyInPort> DiscrepancyInPort_kind_children() const { return Udm::ChildrenAttr< ::FCSL::FM::DiscrepancyInPort>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::FM::DiscrepancyInPort, Pred> DiscrepancyInPort_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::FM::DiscrepancyInPort, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::FM::FailureProp> FailureProp_kind_children() const { return Udm::ChildrenAttr< ::FCSL::FM::FailureProp>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::FM::FailureProp, Pred> FailureProp_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::FM::FailureProp, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::FM::DIP2DOP> DIP2DOP_kind_children() const { return Udm::ChildrenAttr< ::FCSL::FM::DIP2DOP>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::FM::DIP2DOP, Pred> DIP2DOP_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::FM::DIP2DOP, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::FM::TestInstance> TestInstance_kind_children() const { return Udm::ChildrenAttr< ::FCSL::FM::TestInstance>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::FM::TestInstance, Pred> TestInstance_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::FM::TestInstance, Pred>(impl, Udm::NULLCHILDROLE); }

			static ::Uml::CompositionParentRole meta_UnitModels_parent;
			Udm::ParentAttr< ::FCSL::FM::UnitModels> UnitModels_parent() const { return Udm::ParentAttr< ::FCSL::FM::UnitModels>(impl, meta_UnitModels_parent); }

			static ::Uml::CompositionParentRole meta_Assembly_parent;
			Udm::ParentAttr< ::FCSL::FM::Assembly> Assembly_parent() const { return Udm::ParentAttr< ::FCSL::FM::Assembly>(impl, meta_Assembly_parent); }

			Udm::ParentAttr<Udm::Object> parent() const { return Udm::ParentAttr<Udm::Object>(impl, Udm::NULLPARENTROLE); }
		};

		class  Device :  public ::FCSL::FM::Unit {
		public:
			static ::Uml::Class meta;

			Device() { }
			Device(Udm::ObjectImpl *impl) : ::FCSL::FM::Unit(impl), UDM_OBJECT(impl) { }
			Device(const Device &master) : ::FCSL::FM::Unit(master), UDM_OBJECT(master) { }
			static Device Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static Device Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			Device CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			Device CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::FCSL::FM::Device> Instances() { return Udm::InstantiatedAttr< ::FCSL::FM::Device>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::FCSL::FM::Device, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::FCSL::FM::Device, Pred>(impl);}

			Udm::DerivedAttr< ::FCSL::FM::Device> Derived() { return Udm::DerivedAttr< ::FCSL::FM::Device>(impl);}
			template <class Pred> Udm::DerivedAttr< ::FCSL::FM::Device, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::FCSL::FM::Device, Pred>(impl);}

			Udm::ArchetypeAttr< ::FCSL::FM::Device> Archetype() const { return Udm::ArchetypeAttr< ::FCSL::FM::Device>(impl);}

			static ::Uml::AssociationRole meta_srcCtrl2Device;
			static ::Uml::AssociationRole meta_srcCtrl2Device_rev;
			Udm::AClassAssocAttr< ::FCSL::FM::Ctrl2Device, ::FCSL::FM::Control> srcCtrl2Device() const { return Udm::AClassAssocAttr< ::FCSL::FM::Ctrl2Device, ::FCSL::FM::Control>(impl, meta_srcCtrl2Device, meta_srcCtrl2Device_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::FCSL::FM::Ctrl2Device, ::FCSL::FM::Control, Pred> srcCtrl2Device_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::FCSL::FM::Ctrl2Device, ::FCSL::FM::Control, Pred>(impl, meta_srcCtrl2Device, meta_srcCtrl2Device_rev); }

			static ::Uml::AssociationRole meta_dstDevice2Surface;
			static ::Uml::AssociationRole meta_dstDevice2Surface_rev;
			Udm::AClassAssocAttr< ::FCSL::FM::Device2Surface, ::FCSL::FM::Surface> dstDevice2Surface() const { return Udm::AClassAssocAttr< ::FCSL::FM::Device2Surface, ::FCSL::FM::Surface>(impl, meta_dstDevice2Surface, meta_dstDevice2Surface_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::FCSL::FM::Device2Surface, ::FCSL::FM::Surface, Pred> dstDevice2Surface_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::FCSL::FM::Device2Surface, ::FCSL::FM::Surface, Pred>(impl, meta_dstDevice2Surface, meta_dstDevice2Surface_rev); }

			static ::Uml::CompositionChildRole meta_Sig2Dataport_children;
			Udm::ChildrenAttr< ::FCSL::FM::Sig2Dataport> Sig2Dataport_children() const { return Udm::ChildrenAttr< ::FCSL::FM::Sig2Dataport>(impl, meta_Sig2Dataport_children); }
			template <class Pred> Udm::ChildrenAttr< ::FCSL::FM::Sig2Dataport, Pred> Sig2Dataport_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::FM::Sig2Dataport, Pred>(impl, meta_Sig2Dataport_children); }

			static ::Uml::CompositionChildRole meta_Signal_children;
			Udm::ChildrenAttr< ::FCSL::FM::Signal> Signal_children() const { return Udm::ChildrenAttr< ::FCSL::FM::Signal>(impl, meta_Signal_children); }
			template <class Pred> Udm::ChildrenAttr< ::FCSL::FM::Signal, Pred> Signal_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::FM::Signal, Pred>(impl, meta_Signal_children); }

			static ::Uml::CompositionChildRole meta_DataPort_children;
			Udm::ChildrenAttr< ::FCSL::FM::DataPort> DataPort_children() const { return Udm::ChildrenAttr< ::FCSL::FM::DataPort>(impl, meta_DataPort_children); }
			template <class Pred> Udm::ChildrenAttr< ::FCSL::FM::DataPort, Pred> DataPort_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::FM::DataPort, Pred>(impl, meta_DataPort_children); }

			Udm::ChildrenAttr< ::FCSL::FM::Sig2Dataport> Sig2Dataport_kind_children() const { return Udm::ChildrenAttr< ::FCSL::FM::Sig2Dataport>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::FM::Sig2Dataport, Pred> Sig2Dataport_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::FM::Sig2Dataport, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::FM::DataOut> DataOut_kind_children() const { return Udm::ChildrenAttr< ::FCSL::FM::DataOut>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::FM::DataOut, Pred> DataOut_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::FM::DataOut, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::FM::DataIn> DataIn_kind_children() const { return Udm::ChildrenAttr< ::FCSL::FM::DataIn>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::FM::DataIn, Pred> DataIn_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::FM::DataIn, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::FM::Entity> Entity_kind_children() const { return Udm::ChildrenAttr< ::FCSL::FM::Entity>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::FM::Entity, Pred> Entity_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::FM::Entity, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::FM::Unit> Unit_kind_children() const { return Udm::ChildrenAttr< ::FCSL::FM::Unit>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::FM::Unit, Pred> Unit_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::FM::Unit, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::FM::DataPort> DataPort_kind_children() const { return Udm::ChildrenAttr< ::FCSL::FM::DataPort>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::FM::DataPort, Pred> DataPort_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::FM::DataPort, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::FM::Signal> Signal_kind_children() const { return Udm::ChildrenAttr< ::FCSL::FM::Signal>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::FM::Signal, Pred> Signal_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::FM::Signal, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ParentAttr<Udm::Object> parent() const { return Udm::ParentAttr<Udm::Object>(impl, Udm::NULLPARENTROLE); }
		};

		class  Surface :  public ::FCSL::FM::Unit {
		public:
			static ::Uml::Class meta;

			Surface() { }
			Surface(Udm::ObjectImpl *impl) : ::FCSL::FM::Unit(impl), UDM_OBJECT(impl) { }
			Surface(const Surface &master) : ::FCSL::FM::Unit(master), UDM_OBJECT(master) { }
			static Surface Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static Surface Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			Surface CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			Surface CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::FCSL::FM::Surface> Instances() { return Udm::InstantiatedAttr< ::FCSL::FM::Surface>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::FCSL::FM::Surface, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::FCSL::FM::Surface, Pred>(impl);}

			Udm::DerivedAttr< ::FCSL::FM::Surface> Derived() { return Udm::DerivedAttr< ::FCSL::FM::Surface>(impl);}
			template <class Pred> Udm::DerivedAttr< ::FCSL::FM::Surface, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::FCSL::FM::Surface, Pred>(impl);}

			Udm::ArchetypeAttr< ::FCSL::FM::Surface> Archetype() const { return Udm::ArchetypeAttr< ::FCSL::FM::Surface>(impl);}

			static ::Uml::AssociationRole meta_srcDevice2Surface;
			static ::Uml::AssociationRole meta_srcDevice2Surface_rev;
			Udm::AClassAssocAttr< ::FCSL::FM::Device2Surface, ::FCSL::FM::Device> srcDevice2Surface() const { return Udm::AClassAssocAttr< ::FCSL::FM::Device2Surface, ::FCSL::FM::Device>(impl, meta_srcDevice2Surface, meta_srcDevice2Surface_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::FCSL::FM::Device2Surface, ::FCSL::FM::Device, Pred> srcDevice2Surface_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::FCSL::FM::Device2Surface, ::FCSL::FM::Device, Pred>(impl, meta_srcDevice2Surface, meta_srcDevice2Surface_rev); }

			Udm::ParentAttr<Udm::Object> parent() const { return Udm::ParentAttr<Udm::Object>(impl, Udm::NULLPARENTROLE); }
		};

		class  Control :  public ::FCSL::FM::Unit {
		public:
			static ::Uml::Class meta;

			Control() { }
			Control(Udm::ObjectImpl *impl) : ::FCSL::FM::Unit(impl), UDM_OBJECT(impl) { }
			Control(const Control &master) : ::FCSL::FM::Unit(master), UDM_OBJECT(master) { }
			static Control Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static Control Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			Control CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			Control CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::FCSL::FM::Control> Instances() { return Udm::InstantiatedAttr< ::FCSL::FM::Control>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::FCSL::FM::Control, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::FCSL::FM::Control, Pred>(impl);}

			Udm::DerivedAttr< ::FCSL::FM::Control> Derived() { return Udm::DerivedAttr< ::FCSL::FM::Control>(impl);}
			template <class Pred> Udm::DerivedAttr< ::FCSL::FM::Control, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::FCSL::FM::Control, Pred>(impl);}

			Udm::ArchetypeAttr< ::FCSL::FM::Control> Archetype() const { return Udm::ArchetypeAttr< ::FCSL::FM::Control>(impl);}

			static ::Uml::AssociationRole meta_dstCtrl2Device;
			static ::Uml::AssociationRole meta_dstCtrl2Device_rev;
			Udm::AClassAssocAttr< ::FCSL::FM::Ctrl2Device, ::FCSL::FM::Device> dstCtrl2Device() const { return Udm::AClassAssocAttr< ::FCSL::FM::Ctrl2Device, ::FCSL::FM::Device>(impl, meta_dstCtrl2Device, meta_dstCtrl2Device_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::FCSL::FM::Ctrl2Device, ::FCSL::FM::Device, Pred> dstCtrl2Device_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::FCSL::FM::Ctrl2Device, ::FCSL::FM::Device, Pred>(impl, meta_dstCtrl2Device, meta_dstCtrl2Device_rev); }

			Udm::ParentAttr<Udm::Object> parent() const { return Udm::ParentAttr<Udm::Object>(impl, Udm::NULLPARENTROLE); }
		};

		class  DiscrepancyInPort :  public ::FCSL::FM::DiscrepancyPort {
		public:
			static ::Uml::Class meta;

			DiscrepancyInPort() { }
			DiscrepancyInPort(Udm::ObjectImpl *impl) : ::FCSL::FM::DiscrepancyPort(impl), UDM_OBJECT(impl) { }
			DiscrepancyInPort(const DiscrepancyInPort &master) : ::FCSL::FM::DiscrepancyPort(master), UDM_OBJECT(master) { }
			static DiscrepancyInPort Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static DiscrepancyInPort Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			DiscrepancyInPort CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			DiscrepancyInPort CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::FCSL::FM::DiscrepancyInPort> Instances() { return Udm::InstantiatedAttr< ::FCSL::FM::DiscrepancyInPort>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::FCSL::FM::DiscrepancyInPort, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::FCSL::FM::DiscrepancyInPort, Pred>(impl);}

			Udm::DerivedAttr< ::FCSL::FM::DiscrepancyInPort> Derived() { return Udm::DerivedAttr< ::FCSL::FM::DiscrepancyInPort>(impl);}
			template <class Pred> Udm::DerivedAttr< ::FCSL::FM::DiscrepancyInPort, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::FCSL::FM::DiscrepancyInPort, Pred>(impl);}

			Udm::ArchetypeAttr< ::FCSL::FM::DiscrepancyInPort> Archetype() const { return Udm::ArchetypeAttr< ::FCSL::FM::DiscrepancyInPort>(impl);}

			static ::Uml::AssociationRole meta_dstDIP2D;
			static ::Uml::AssociationRole meta_dstDIP2D_rev;
			Udm::AClassAssocAttr< ::FCSL::FM::DIP2D, ::FCSL::FM::Discrepancy> dstDIP2D() const { return Udm::AClassAssocAttr< ::FCSL::FM::DIP2D, ::FCSL::FM::Discrepancy>(impl, meta_dstDIP2D, meta_dstDIP2D_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::FCSL::FM::DIP2D, ::FCSL::FM::Discrepancy, Pred> dstDIP2D_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::FCSL::FM::DIP2D, ::FCSL::FM::Discrepancy, Pred>(impl, meta_dstDIP2D, meta_dstDIP2D_rev); }

			Udm::ParentAttr< ::FCSL::MgaObject> parent() const { return Udm::ParentAttr< ::FCSL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  DataPort :  public ::FCSL::MgaObject {
		public:
			static ::Uml::Class meta;

			DataPort() { }
			DataPort(Udm::ObjectImpl *impl) : ::FCSL::MgaObject(impl), UDM_OBJECT(impl) { }
			DataPort(const DataPort &master) : ::FCSL::MgaObject(master), UDM_OBJECT(master) { }
			static DataPort Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static DataPort Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			DataPort CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			DataPort CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::FCSL::FM::DataPort> Instances() { return Udm::InstantiatedAttr< ::FCSL::FM::DataPort>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::FCSL::FM::DataPort, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::FCSL::FM::DataPort, Pred>(impl);}

			Udm::DerivedAttr< ::FCSL::FM::DataPort> Derived() { return Udm::DerivedAttr< ::FCSL::FM::DataPort>(impl);}
			template <class Pred> Udm::DerivedAttr< ::FCSL::FM::DataPort, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::FCSL::FM::DataPort, Pred>(impl);}

			Udm::ArchetypeAttr< ::FCSL::FM::DataPort> Archetype() const { return Udm::ArchetypeAttr< ::FCSL::FM::DataPort>(impl);}

			static ::Uml::AssociationRole meta_srcSignalflowConn;
			static ::Uml::AssociationRole meta_srcSignalflowConn_rev;
			Udm::AClassAssocAttr< ::FCSL::FM::SignalflowConn, ::FCSL::FM::DataPort> srcSignalflowConn() const { return Udm::AClassAssocAttr< ::FCSL::FM::SignalflowConn, ::FCSL::FM::DataPort>(impl, meta_srcSignalflowConn, meta_srcSignalflowConn_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::FCSL::FM::SignalflowConn, ::FCSL::FM::DataPort, Pred> srcSignalflowConn_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::FCSL::FM::SignalflowConn, ::FCSL::FM::DataPort, Pred>(impl, meta_srcSignalflowConn, meta_srcSignalflowConn_rev); }

			static ::Uml::AssociationRole meta_dstSignalflowConn;
			static ::Uml::AssociationRole meta_dstSignalflowConn_rev;
			Udm::AClassAssocAttr< ::FCSL::FM::SignalflowConn, ::FCSL::FM::DataPort> dstSignalflowConn() const { return Udm::AClassAssocAttr< ::FCSL::FM::SignalflowConn, ::FCSL::FM::DataPort>(impl, meta_dstSignalflowConn, meta_dstSignalflowConn_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::FCSL::FM::SignalflowConn, ::FCSL::FM::DataPort, Pred> dstSignalflowConn_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::FCSL::FM::SignalflowConn, ::FCSL::FM::DataPort, Pred>(impl, meta_dstSignalflowConn, meta_dstSignalflowConn_rev); }

			static ::Uml::AssociationRole meta_srcSig2Dataport;
			static ::Uml::AssociationRole meta_srcSig2Dataport_rev;
			Udm::AClassAssocAttr< ::FCSL::FM::Sig2Dataport, ::FCSL::FM::Signal> srcSig2Dataport() const { return Udm::AClassAssocAttr< ::FCSL::FM::Sig2Dataport, ::FCSL::FM::Signal>(impl, meta_srcSig2Dataport, meta_srcSig2Dataport_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::FCSL::FM::Sig2Dataport, ::FCSL::FM::Signal, Pred> srcSig2Dataport_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::FCSL::FM::Sig2Dataport, ::FCSL::FM::Signal, Pred>(impl, meta_srcSig2Dataport, meta_srcSig2Dataport_rev); }

			static ::Uml::CompositionParentRole meta_Device_parent;
			Udm::ParentAttr< ::FCSL::FM::Device> Device_parent() const { return Udm::ParentAttr< ::FCSL::FM::Device>(impl, meta_Device_parent); }

			static ::Uml::CompositionParentRole meta_Assembly_parent;
			Udm::ParentAttr< ::FCSL::FM::Assembly> Assembly_parent() const { return Udm::ParentAttr< ::FCSL::FM::Assembly>(impl, meta_Assembly_parent); }

			Udm::ParentAttr< ::FCSL::MgaObject> parent() const { return Udm::ParentAttr< ::FCSL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  DataOut :  public ::FCSL::FM::DataPort {
		public:
			static ::Uml::Class meta;

			DataOut() { }
			DataOut(Udm::ObjectImpl *impl) : ::FCSL::FM::DataPort(impl), UDM_OBJECT(impl) { }
			DataOut(const DataOut &master) : ::FCSL::FM::DataPort(master), UDM_OBJECT(master) { }
			static DataOut Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static DataOut Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			DataOut CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			DataOut CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::FCSL::FM::DataOut> Instances() { return Udm::InstantiatedAttr< ::FCSL::FM::DataOut>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::FCSL::FM::DataOut, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::FCSL::FM::DataOut, Pred>(impl);}

			Udm::DerivedAttr< ::FCSL::FM::DataOut> Derived() { return Udm::DerivedAttr< ::FCSL::FM::DataOut>(impl);}
			template <class Pred> Udm::DerivedAttr< ::FCSL::FM::DataOut, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::FCSL::FM::DataOut, Pred>(impl);}

			Udm::ArchetypeAttr< ::FCSL::FM::DataOut> Archetype() const { return Udm::ArchetypeAttr< ::FCSL::FM::DataOut>(impl);}

			Udm::ParentAttr< ::FCSL::MgaObject> parent() const { return Udm::ParentAttr< ::FCSL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  DataIn :  public ::FCSL::FM::DataPort {
		public:
			static ::Uml::Class meta;

			DataIn() { }
			DataIn(Udm::ObjectImpl *impl) : ::FCSL::FM::DataPort(impl), UDM_OBJECT(impl) { }
			DataIn(const DataIn &master) : ::FCSL::FM::DataPort(master), UDM_OBJECT(master) { }
			static DataIn Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static DataIn Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			DataIn CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			DataIn CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::FCSL::FM::DataIn> Instances() { return Udm::InstantiatedAttr< ::FCSL::FM::DataIn>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::FCSL::FM::DataIn, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::FCSL::FM::DataIn, Pred>(impl);}

			Udm::DerivedAttr< ::FCSL::FM::DataIn> Derived() { return Udm::DerivedAttr< ::FCSL::FM::DataIn>(impl);}
			template <class Pred> Udm::DerivedAttr< ::FCSL::FM::DataIn, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::FCSL::FM::DataIn, Pred>(impl);}

			Udm::ArchetypeAttr< ::FCSL::FM::DataIn> Archetype() const { return Udm::ArchetypeAttr< ::FCSL::FM::DataIn>(impl);}

			Udm::ParentAttr< ::FCSL::MgaObject> parent() const { return Udm::ParentAttr< ::FCSL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  FailureProp :  public ::FCSL::FM::FaultModelElement {
		public:
			static ::Uml::Class meta;

			FailureProp() { }
			FailureProp(Udm::ObjectImpl *impl) : ::FCSL::FM::FaultModelElement(impl), UDM_OBJECT(impl) { }
			FailureProp(const FailureProp &master) : ::FCSL::FM::FaultModelElement(master), UDM_OBJECT(master) { }
			static FailureProp Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static FailureProp Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			FailureProp CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			FailureProp CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::FCSL::FM::FailureProp> Instances() { return Udm::InstantiatedAttr< ::FCSL::FM::FailureProp>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::FCSL::FM::FailureProp, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::FCSL::FM::FailureProp, Pred>(impl);}

			Udm::DerivedAttr< ::FCSL::FM::FailureProp> Derived() { return Udm::DerivedAttr< ::FCSL::FM::FailureProp>(impl);}
			template <class Pred> Udm::DerivedAttr< ::FCSL::FM::FailureProp, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::FCSL::FM::FailureProp, Pred>(impl);}

			Udm::ArchetypeAttr< ::FCSL::FM::FailureProp> Archetype() const { return Udm::ArchetypeAttr< ::FCSL::FM::FailureProp>(impl);}

			Udm::ParentAttr< ::FCSL::MgaObject> parent() const { return Udm::ParentAttr< ::FCSL::MgaObject>(impl, Udm::NULLPARENTROLE); }
			static ::Uml::AssociationRole meta_srcFailureProp_end_;
			Udm::AssocEndAttr< ::FCSL::FM::Discrepancy> srcFailureProp_end() const { return Udm::AssocEndAttr< ::FCSL::FM::Discrepancy>(impl, meta_srcFailureProp_end_); }

			static ::Uml::AssociationRole meta_dstFailureProp_end_;
			Udm::AssocEndAttr< ::FCSL::FM::Discrepancy> dstFailureProp_end() const { return Udm::AssocEndAttr< ::FCSL::FM::Discrepancy>(impl, meta_dstFailureProp_end_); }

		};

		class  DIP2DOP :  public ::FCSL::FM::FaultModelElement {
		public:
			static ::Uml::Class meta;

			DIP2DOP() { }
			DIP2DOP(Udm::ObjectImpl *impl) : ::FCSL::FM::FaultModelElement(impl), UDM_OBJECT(impl) { }
			DIP2DOP(const DIP2DOP &master) : ::FCSL::FM::FaultModelElement(master), UDM_OBJECT(master) { }
			static DIP2DOP Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static DIP2DOP Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			DIP2DOP CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			DIP2DOP CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::FCSL::FM::DIP2DOP> Instances() { return Udm::InstantiatedAttr< ::FCSL::FM::DIP2DOP>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::FCSL::FM::DIP2DOP, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::FCSL::FM::DIP2DOP, Pred>(impl);}

			Udm::DerivedAttr< ::FCSL::FM::DIP2DOP> Derived() { return Udm::DerivedAttr< ::FCSL::FM::DIP2DOP>(impl);}
			template <class Pred> Udm::DerivedAttr< ::FCSL::FM::DIP2DOP, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::FCSL::FM::DIP2DOP, Pred>(impl);}

			Udm::ArchetypeAttr< ::FCSL::FM::DIP2DOP> Archetype() const { return Udm::ArchetypeAttr< ::FCSL::FM::DIP2DOP>(impl);}

			static ::Uml::CompositionParentRole meta_Platform_parent;
			Udm::ParentAttr< ::FCSL::FM::Platform> Platform_parent() const { return Udm::ParentAttr< ::FCSL::FM::Platform>(impl, meta_Platform_parent); }

			Udm::ParentAttr< ::FCSL::MgaObject> parent() const { return Udm::ParentAttr< ::FCSL::MgaObject>(impl, Udm::NULLPARENTROLE); }
			static ::Uml::AssociationRole meta_dstDIP2DOP_end_;
			Udm::AssocEndAttr< ::FCSL::FM::DiscrepancyPort> dstDIP2DOP_end() const { return Udm::AssocEndAttr< ::FCSL::FM::DiscrepancyPort>(impl, meta_dstDIP2DOP_end_); }

			static ::Uml::AssociationRole meta_srcDIP2DOP_end_;
			Udm::AssocEndAttr< ::FCSL::FM::DiscrepancyPort> srcDIP2DOP_end() const { return Udm::AssocEndAttr< ::FCSL::FM::DiscrepancyPort>(impl, meta_srcDIP2DOP_end_); }

		};

		class  TestInstance :  public ::FCSL::FM::FaultModelElement {
		public:
			static ::Uml::Class meta;

			TestInstance() { }
			TestInstance(Udm::ObjectImpl *impl) : ::FCSL::FM::FaultModelElement(impl), UDM_OBJECT(impl) { }
			TestInstance(const TestInstance &master) : ::FCSL::FM::FaultModelElement(master), UDM_OBJECT(master) { }
			static TestInstance Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static TestInstance Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			TestInstance CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			TestInstance CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::FCSL::FM::TestInstance> Instances() { return Udm::InstantiatedAttr< ::FCSL::FM::TestInstance>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::FCSL::FM::TestInstance, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::FCSL::FM::TestInstance, Pred>(impl);}

			Udm::DerivedAttr< ::FCSL::FM::TestInstance> Derived() { return Udm::DerivedAttr< ::FCSL::FM::TestInstance>(impl);}
			template <class Pred> Udm::DerivedAttr< ::FCSL::FM::TestInstance, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::FCSL::FM::TestInstance, Pred>(impl);}

			Udm::ArchetypeAttr< ::FCSL::FM::TestInstance> Archetype() const { return Udm::ArchetypeAttr< ::FCSL::FM::TestInstance>(impl);}

			static ::Uml::AssociationRole meta_srcD2Test;
			static ::Uml::AssociationRole meta_srcD2Test_rev;
			Udm::AClassAssocAttr< ::FCSL::FM::D2Test, ::FCSL::FM::Discrepancy> srcD2Test() const { return Udm::AClassAssocAttr< ::FCSL::FM::D2Test, ::FCSL::FM::Discrepancy>(impl, meta_srcD2Test, meta_srcD2Test_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::FCSL::FM::D2Test, ::FCSL::FM::Discrepancy, Pred> srcD2Test_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::FCSL::FM::D2Test, ::FCSL::FM::Discrepancy, Pred>(impl, meta_srcD2Test, meta_srcD2Test_rev); }

			static ::Uml::AssociationRole meta_srcDOP2T;
			static ::Uml::AssociationRole meta_srcDOP2T_rev;
			Udm::AClassAssocAttr< ::FCSL::FM::DOP2T, ::FCSL::FM::DiscrepancyOutPort> srcDOP2T() const { return Udm::AClassAssocAttr< ::FCSL::FM::DOP2T, ::FCSL::FM::DiscrepancyOutPort>(impl, meta_srcDOP2T, meta_srcDOP2T_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::FCSL::FM::DOP2T, ::FCSL::FM::DiscrepancyOutPort, Pred> srcDOP2T_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::FCSL::FM::DOP2T, ::FCSL::FM::DiscrepancyOutPort, Pred>(impl, meta_srcDOP2T, meta_srcDOP2T_rev); }

			static ::Uml::AssociationRole meta_ref;
			Udm::PointerAttr< ::FCSL::FM::Test> ref() const { return Udm::PointerAttr< ::FCSL::FM::Test>(impl, meta_ref); }

			Udm::ParentAttr< ::FCSL::MgaObject> parent() const { return Udm::ParentAttr< ::FCSL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  Signal :  public ::FCSL::FM::Unit {
		public:
			static ::Uml::Class meta;

			Signal() { }
			Signal(Udm::ObjectImpl *impl) : ::FCSL::FM::Unit(impl), UDM_OBJECT(impl) { }
			Signal(const Signal &master) : ::FCSL::FM::Unit(master), UDM_OBJECT(master) { }
			static Signal Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static Signal Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			Signal CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			Signal CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::FCSL::FM::Signal> Instances() { return Udm::InstantiatedAttr< ::FCSL::FM::Signal>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::FCSL::FM::Signal, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::FCSL::FM::Signal, Pred>(impl);}

			Udm::DerivedAttr< ::FCSL::FM::Signal> Derived() { return Udm::DerivedAttr< ::FCSL::FM::Signal>(impl);}
			template <class Pred> Udm::DerivedAttr< ::FCSL::FM::Signal, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::FCSL::FM::Signal, Pred>(impl);}

			Udm::ArchetypeAttr< ::FCSL::FM::Signal> Archetype() const { return Udm::ArchetypeAttr< ::FCSL::FM::Signal>(impl);}

			static ::Uml::AssociationRole meta_dstSig2Dataport;
			static ::Uml::AssociationRole meta_dstSig2Dataport_rev;
			Udm::AClassAssocAttr< ::FCSL::FM::Sig2Dataport, ::FCSL::FM::DataPort> dstSig2Dataport() const { return Udm::AClassAssocAttr< ::FCSL::FM::Sig2Dataport, ::FCSL::FM::DataPort>(impl, meta_dstSig2Dataport, meta_dstSig2Dataport_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::FCSL::FM::Sig2Dataport, ::FCSL::FM::DataPort, Pred> dstSig2Dataport_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::FCSL::FM::Sig2Dataport, ::FCSL::FM::DataPort, Pred>(impl, meta_dstSig2Dataport, meta_dstSig2Dataport_rev); }

			static ::Uml::CompositionParentRole meta_Device_parent;
			Udm::ParentAttr< ::FCSL::FM::Device> Device_parent() const { return Udm::ParentAttr< ::FCSL::FM::Device>(impl, meta_Device_parent); }

			Udm::ParentAttr<Udm::Object> parent() const { return Udm::ParentAttr<Udm::Object>(impl, Udm::NULLPARENTROLE); }
		};

		class  TestRef :  public ::FCSL::MgaObject {
		public:
			static ::Uml::Class meta;

			TestRef() { }
			TestRef(Udm::ObjectImpl *impl) : ::FCSL::MgaObject(impl), UDM_OBJECT(impl) { }
			TestRef(const TestRef &master) : ::FCSL::MgaObject(master), UDM_OBJECT(master) { }
			static TestRef Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static TestRef Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			TestRef CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			TestRef CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::FCSL::FM::TestRef> Instances() { return Udm::InstantiatedAttr< ::FCSL::FM::TestRef>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::FCSL::FM::TestRef, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::FCSL::FM::TestRef, Pred>(impl);}

			Udm::DerivedAttr< ::FCSL::FM::TestRef> Derived() { return Udm::DerivedAttr< ::FCSL::FM::TestRef>(impl);}
			template <class Pred> Udm::DerivedAttr< ::FCSL::FM::TestRef, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::FCSL::FM::TestRef, Pred>(impl);}

			Udm::ArchetypeAttr< ::FCSL::FM::TestRef> Archetype() const { return Udm::ArchetypeAttr< ::FCSL::FM::TestRef>(impl);}

			static ::Uml::AssociationRole meta_ref;
			Udm::PointerAttr< ::FCSL::FM::Test> ref() const { return Udm::PointerAttr< ::FCSL::FM::Test>(impl, meta_ref); }

			static ::Uml::CompositionParentRole meta_TestGroup_parent;
			Udm::ParentAttr< ::FCSL::FM::TestGroup> TestGroup_parent() const { return Udm::ParentAttr< ::FCSL::FM::TestGroup>(impl, meta_TestGroup_parent); }

			Udm::ParentAttr< ::FCSL::MgaObject> parent() const { return Udm::ParentAttr< ::FCSL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

	}


	namespace SLSF {
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

			Udm::InstantiatedAttr< ::FCSL::SLSF::Dataflow> Instances() { return Udm::InstantiatedAttr< ::FCSL::SLSF::Dataflow>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::FCSL::SLSF::Dataflow, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::FCSL::SLSF::Dataflow, Pred>(impl);}

			Udm::DerivedAttr< ::FCSL::SLSF::Dataflow> Derived() { return Udm::DerivedAttr< ::FCSL::SLSF::Dataflow>(impl);}
			template <class Pred> Udm::DerivedAttr< ::FCSL::SLSF::Dataflow, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::FCSL::SLSF::Dataflow, Pred>(impl);}

			Udm::ArchetypeAttr< ::FCSL::SLSF::Dataflow> Archetype() const { return Udm::ArchetypeAttr< ::FCSL::SLSF::Dataflow>(impl);}

			static ::Uml::Attribute meta_name;
			Udm::StringAttr name() const { return Udm::StringAttr(impl, meta_name); }

			static ::Uml::CompositionChildRole meta_System_children;
			Udm::ChildrenAttr< ::FCSL::SLSF::System> System_children() const { return Udm::ChildrenAttr< ::FCSL::SLSF::System>(impl, meta_System_children); }
			template <class Pred> Udm::ChildrenAttr< ::FCSL::SLSF::System, Pred> System_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::SLSF::System, Pred>(impl, meta_System_children); }

			Udm::ChildrenAttr< ::FCSL::SLSF::Block> Block_kind_children() const { return Udm::ChildrenAttr< ::FCSL::SLSF::Block>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::SLSF::Block, Pred> Block_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::SLSF::Block, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::SLSF::System> System_kind_children() const { return Udm::ChildrenAttr< ::FCSL::SLSF::System>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::SLSF::System, Pred> System_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::SLSF::System, Pred>(impl, Udm::NULLCHILDROLE); }

			static ::Uml::CompositionParentRole meta_RootFolder_parent;
			Udm::ParentAttr< ::FCSL::RootFolder> RootFolder_parent() const { return Udm::ParentAttr< ::FCSL::RootFolder>(impl, meta_RootFolder_parent); }

			Udm::ParentAttr< ::FCSL::RootFolder> parent() const { return Udm::ParentAttr< ::FCSL::RootFolder>(impl, Udm::NULLPARENTROLE); }
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

			Udm::InstantiatedAttr< ::FCSL::SLSF::Types> Instances() { return Udm::InstantiatedAttr< ::FCSL::SLSF::Types>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::FCSL::SLSF::Types, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::FCSL::SLSF::Types, Pred>(impl);}

			Udm::DerivedAttr< ::FCSL::SLSF::Types> Derived() { return Udm::DerivedAttr< ::FCSL::SLSF::Types>(impl);}
			template <class Pred> Udm::DerivedAttr< ::FCSL::SLSF::Types, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::FCSL::SLSF::Types, Pred>(impl);}

			Udm::ArchetypeAttr< ::FCSL::SLSF::Types> Archetype() const { return Udm::ArchetypeAttr< ::FCSL::SLSF::Types>(impl);}

			static ::Uml::Attribute meta_name;
			Udm::StringAttr name() const { return Udm::StringAttr(impl, meta_name); }

			static ::Uml::CompositionChildRole meta_TypeBase_children;
			Udm::ChildrenAttr< ::FCSL::SLSF::TypeBase> TypeBase_children() const { return Udm::ChildrenAttr< ::FCSL::SLSF::TypeBase>(impl, meta_TypeBase_children); }
			template <class Pred> Udm::ChildrenAttr< ::FCSL::SLSF::TypeBase, Pred> TypeBase_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::SLSF::TypeBase, Pred>(impl, meta_TypeBase_children); }

			Udm::ChildrenAttr< ::FCSL::SLSF::TypeBase> TypeBase_kind_children() const { return Udm::ChildrenAttr< ::FCSL::SLSF::TypeBase>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::SLSF::TypeBase, Pred> TypeBase_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::SLSF::TypeBase, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::SLSF::Matrix> Matrix_kind_children() const { return Udm::ChildrenAttr< ::FCSL::SLSF::Matrix>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::SLSF::Matrix, Pred> Matrix_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::SLSF::Matrix, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::SLSF::TypeStruct> TypeStruct_kind_children() const { return Udm::ChildrenAttr< ::FCSL::SLSF::TypeStruct>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::SLSF::TypeStruct, Pred> TypeStruct_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::SLSF::TypeStruct, Pred>(impl, Udm::NULLCHILDROLE); }

			static ::Uml::CompositionParentRole meta_RootFolder_parent;
			Udm::ParentAttr< ::FCSL::RootFolder> RootFolder_parent() const { return Udm::ParentAttr< ::FCSL::RootFolder>(impl, meta_RootFolder_parent); }

			Udm::ParentAttr< ::FCSL::RootFolder> parent() const { return Udm::ParentAttr< ::FCSL::RootFolder>(impl, Udm::NULLPARENTROLE); }
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

			Udm::InstantiatedAttr< ::FCSL::SLSF::Stateflow> Instances() { return Udm::InstantiatedAttr< ::FCSL::SLSF::Stateflow>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::FCSL::SLSF::Stateflow, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::FCSL::SLSF::Stateflow, Pred>(impl);}

			Udm::DerivedAttr< ::FCSL::SLSF::Stateflow> Derived() { return Udm::DerivedAttr< ::FCSL::SLSF::Stateflow>(impl);}
			template <class Pred> Udm::DerivedAttr< ::FCSL::SLSF::Stateflow, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::FCSL::SLSF::Stateflow, Pred>(impl);}

			Udm::ArchetypeAttr< ::FCSL::SLSF::Stateflow> Archetype() const { return Udm::ArchetypeAttr< ::FCSL::SLSF::Stateflow>(impl);}

			static ::Uml::Attribute meta_name;
			Udm::StringAttr name() const { return Udm::StringAttr(impl, meta_name); }

			static ::Uml::CompositionChildRole meta_State_children;
			Udm::ChildrenAttr< ::FCSL::SLSF::State> State_children() const { return Udm::ChildrenAttr< ::FCSL::SLSF::State>(impl, meta_State_children); }
			template <class Pred> Udm::ChildrenAttr< ::FCSL::SLSF::State, Pred> State_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::SLSF::State, Pred>(impl, meta_State_children); }

			Udm::ChildrenAttr< ::FCSL::SLSF::State> State_kind_children() const { return Udm::ChildrenAttr< ::FCSL::SLSF::State>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::SLSF::State, Pred> State_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::SLSF::State, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::SLSF::TransConnector> TransConnector_kind_children() const { return Udm::ChildrenAttr< ::FCSL::SLSF::TransConnector>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::SLSF::TransConnector, Pred> TransConnector_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::SLSF::TransConnector, Pred>(impl, Udm::NULLCHILDROLE); }

			static ::Uml::CompositionParentRole meta_RootFolder_parent;
			Udm::ParentAttr< ::FCSL::RootFolder> RootFolder_parent() const { return Udm::ParentAttr< ::FCSL::RootFolder>(impl, meta_RootFolder_parent); }

			Udm::ParentAttr< ::FCSL::RootFolder> parent() const { return Udm::ParentAttr< ::FCSL::RootFolder>(impl, Udm::NULLPARENTROLE); }
		};

		class  Block :  public ::FCSL::MgaObject {
		public:
			static ::Uml::Class meta;

			Block() { }
			Block(Udm::ObjectImpl *impl) : ::FCSL::MgaObject(impl), UDM_OBJECT(impl) { }
			Block(const Block &master) : ::FCSL::MgaObject(master), UDM_OBJECT(master) { }
			static Block Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static Block Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			Block CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			Block CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::FCSL::SLSF::Block> Instances() { return Udm::InstantiatedAttr< ::FCSL::SLSF::Block>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::FCSL::SLSF::Block, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::FCSL::SLSF::Block, Pred>(impl);}

			Udm::DerivedAttr< ::FCSL::SLSF::Block> Derived() { return Udm::DerivedAttr< ::FCSL::SLSF::Block>(impl);}
			template <class Pred> Udm::DerivedAttr< ::FCSL::SLSF::Block, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::FCSL::SLSF::Block, Pred>(impl);}

			Udm::ArchetypeAttr< ::FCSL::SLSF::Block> Archetype() const { return Udm::ArchetypeAttr< ::FCSL::SLSF::Block>(impl);}

			static ::Uml::Attribute meta_Priority;
			Udm::IntegerAttr Priority() const { return Udm::IntegerAttr(impl, meta_Priority); }

			static ::Uml::Attribute meta_SampleTime;
			Udm::RealAttr SampleTime() const { return Udm::RealAttr(impl, meta_SampleTime); }

			static ::Uml::Attribute meta_Description;
			Udm::StringAttr Description() const { return Udm::StringAttr(impl, meta_Description); }

			static ::Uml::Attribute meta_Name;
			Udm::StringAttr Name() const { return Udm::StringAttr(impl, meta_Name); }

			static ::Uml::Attribute meta_Tag;
			Udm::StringAttr Tag() const { return Udm::StringAttr(impl, meta_Tag); }

			static ::Uml::Attribute meta_BlockType;
			Udm::StringAttr BlockType() const { return Udm::StringAttr(impl, meta_BlockType); }

			static ::Uml::CompositionChildRole meta_Annotation_children;
			Udm::ChildrenAttr< ::FCSL::SLSF::Annotation> Annotation_children() const { return Udm::ChildrenAttr< ::FCSL::SLSF::Annotation>(impl, meta_Annotation_children); }
			template <class Pred> Udm::ChildrenAttr< ::FCSL::SLSF::Annotation, Pred> Annotation_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::SLSF::Annotation, Pred>(impl, meta_Annotation_children); }

			static ::Uml::CompositionChildRole meta_Parameter;
			Udm::ChildrenAttr< ::FCSL::SLSF::Parameter> Parameter() const { return Udm::ChildrenAttr< ::FCSL::SLSF::Parameter>(impl, meta_Parameter); }
			template <class Pred> Udm::ChildrenAttr< ::FCSL::SLSF::Parameter, Pred> Parameter_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::SLSF::Parameter, Pred>(impl, meta_Parameter); }

			static ::Uml::CompositionChildRole meta_Line;
			Udm::ChildrenAttr< ::FCSL::SLSF::Line> Line() const { return Udm::ChildrenAttr< ::FCSL::SLSF::Line>(impl, meta_Line); }
			template <class Pred> Udm::ChildrenAttr< ::FCSL::SLSF::Line, Pred> Line_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::SLSF::Line, Pred>(impl, meta_Line); }

			static ::Uml::CompositionChildRole meta_Port_children;
			Udm::ChildrenAttr< ::FCSL::SLSF::Port> Port_children() const { return Udm::ChildrenAttr< ::FCSL::SLSF::Port>(impl, meta_Port_children); }
			template <class Pred> Udm::ChildrenAttr< ::FCSL::SLSF::Port, Pred> Port_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::SLSF::Port, Pred>(impl, meta_Port_children); }

			static ::Uml::CompositionChildRole meta_ConnectorRef_children;
			Udm::ChildrenAttr< ::FCSL::SLSF::ConnectorRef> ConnectorRef_children() const { return Udm::ChildrenAttr< ::FCSL::SLSF::ConnectorRef>(impl, meta_ConnectorRef_children); }
			template <class Pred> Udm::ChildrenAttr< ::FCSL::SLSF::ConnectorRef, Pred> ConnectorRef_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::SLSF::ConnectorRef, Pred>(impl, meta_ConnectorRef_children); }

			Udm::ChildrenAttr< ::FCSL::SLSF::EnablePort> EnablePort_kind_children() const { return Udm::ChildrenAttr< ::FCSL::SLSF::EnablePort>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::SLSF::EnablePort, Pred> EnablePort_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::SLSF::EnablePort, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::SLSF::OutPort> OutPort_kind_children() const { return Udm::ChildrenAttr< ::FCSL::SLSF::OutPort>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::SLSF::OutPort, Pred> OutPort_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::SLSF::OutPort, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::SLSF::Annotation> Annotation_kind_children() const { return Udm::ChildrenAttr< ::FCSL::SLSF::Annotation>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::SLSF::Annotation, Pred> Annotation_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::SLSF::Annotation, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::SLSF::OutputPort> OutputPort_kind_children() const { return Udm::ChildrenAttr< ::FCSL::SLSF::OutputPort>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::SLSF::OutputPort, Pred> OutputPort_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::SLSF::OutputPort, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::SLSF::Parameter> Parameter_kind_children() const { return Udm::ChildrenAttr< ::FCSL::SLSF::Parameter>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::SLSF::Parameter, Pred> Parameter_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::SLSF::Parameter, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::SLSF::InputPort> InputPort_kind_children() const { return Udm::ChildrenAttr< ::FCSL::SLSF::InputPort>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::SLSF::InputPort, Pred> InputPort_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::SLSF::InputPort, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::SLSF::Line> Line_kind_children() const { return Udm::ChildrenAttr< ::FCSL::SLSF::Line>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::SLSF::Line, Pred> Line_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::SLSF::Line, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::SLSF::InPort> InPort_kind_children() const { return Udm::ChildrenAttr< ::FCSL::SLSF::InPort>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::SLSF::InPort, Pred> InPort_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::SLSF::InPort, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::SLSF::StatePort> StatePort_kind_children() const { return Udm::ChildrenAttr< ::FCSL::SLSF::StatePort>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::SLSF::StatePort, Pred> StatePort_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::SLSF::StatePort, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::SLSF::TriggerPort> TriggerPort_kind_children() const { return Udm::ChildrenAttr< ::FCSL::SLSF::TriggerPort>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::SLSF::TriggerPort, Pred> TriggerPort_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::SLSF::TriggerPort, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::SLSF::Port> Port_kind_children() const { return Udm::ChildrenAttr< ::FCSL::SLSF::Port>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::SLSF::Port, Pred> Port_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::SLSF::Port, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::SLSF::ConnectorRef> ConnectorRef_kind_children() const { return Udm::ChildrenAttr< ::FCSL::SLSF::ConnectorRef>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::SLSF::ConnectorRef, Pred> ConnectorRef_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::SLSF::ConnectorRef, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::SLSF::TransConnector> TransConnector_kind_children() const { return Udm::ChildrenAttr< ::FCSL::SLSF::TransConnector>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::SLSF::TransConnector, Pred> TransConnector_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::SLSF::TransConnector, Pred>(impl, Udm::NULLCHILDROLE); }

			static ::Uml::CompositionParentRole meta_System_parent;
			Udm::ParentAttr< ::FCSL::SLSF::System> System_parent() const { return Udm::ParentAttr< ::FCSL::SLSF::System>(impl, meta_System_parent); }

			Udm::ParentAttr< ::FCSL::MgaObject> parent() const { return Udm::ParentAttr< ::FCSL::MgaObject>(impl, Udm::NULLPARENTROLE); }
			static ::Uml::Constraint meta_UniqueConnectorNames;
		};

		class  Annotation :  public ::FCSL::MgaObject {
		public:
			static ::Uml::Class meta;

			Annotation() { }
			Annotation(Udm::ObjectImpl *impl) : ::FCSL::MgaObject(impl), UDM_OBJECT(impl) { }
			Annotation(const Annotation &master) : ::FCSL::MgaObject(master), UDM_OBJECT(master) { }
			static Annotation Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static Annotation Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			Annotation CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			Annotation CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::FCSL::SLSF::Annotation> Instances() { return Udm::InstantiatedAttr< ::FCSL::SLSF::Annotation>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::FCSL::SLSF::Annotation, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::FCSL::SLSF::Annotation, Pred>(impl);}

			Udm::DerivedAttr< ::FCSL::SLSF::Annotation> Derived() { return Udm::DerivedAttr< ::FCSL::SLSF::Annotation>(impl);}
			template <class Pred> Udm::DerivedAttr< ::FCSL::SLSF::Annotation, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::FCSL::SLSF::Annotation, Pred>(impl);}

			Udm::ArchetypeAttr< ::FCSL::SLSF::Annotation> Archetype() const { return Udm::ArchetypeAttr< ::FCSL::SLSF::Annotation>(impl);}

			static ::Uml::Attribute meta_Text;
			Udm::StringAttr Text() const { return Udm::StringAttr(impl, meta_Text); }

			static ::Uml::CompositionParentRole meta_Block_parent;
			Udm::ParentAttr< ::FCSL::SLSF::Block> Block_parent() const { return Udm::ParentAttr< ::FCSL::SLSF::Block>(impl, meta_Block_parent); }

			Udm::ParentAttr< ::FCSL::MgaObject> parent() const { return Udm::ParentAttr< ::FCSL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  Parameter :  public ::FCSL::MgaObject {
		public:
			static ::Uml::Class meta;

			Parameter() { }
			Parameter(Udm::ObjectImpl *impl) : ::FCSL::MgaObject(impl), UDM_OBJECT(impl) { }
			Parameter(const Parameter &master) : ::FCSL::MgaObject(master), UDM_OBJECT(master) { }
			static Parameter Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static Parameter Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			Parameter CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			Parameter CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::FCSL::SLSF::Parameter> Instances() { return Udm::InstantiatedAttr< ::FCSL::SLSF::Parameter>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::FCSL::SLSF::Parameter, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::FCSL::SLSF::Parameter, Pred>(impl);}

			Udm::DerivedAttr< ::FCSL::SLSF::Parameter> Derived() { return Udm::DerivedAttr< ::FCSL::SLSF::Parameter>(impl);}
			template <class Pred> Udm::DerivedAttr< ::FCSL::SLSF::Parameter, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::FCSL::SLSF::Parameter, Pred>(impl);}

			Udm::ArchetypeAttr< ::FCSL::SLSF::Parameter> Archetype() const { return Udm::ArchetypeAttr< ::FCSL::SLSF::Parameter>(impl);}

			static ::Uml::Attribute meta_Value;
			Udm::StringAttr Value() const { return Udm::StringAttr(impl, meta_Value); }

			static ::Uml::CompositionParentRole meta_Parameter_Block_parent;
			Udm::ParentAttr< ::FCSL::SLSF::Block> Parameter_Block_parent() const { return Udm::ParentAttr< ::FCSL::SLSF::Block>(impl, meta_Parameter_Block_parent); }

			Udm::ParentAttr< ::FCSL::MgaObject> parent() const { return Udm::ParentAttr< ::FCSL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  Line :  public ::FCSL::MgaObject {
		public:
			static ::Uml::Class meta;

			Line() { }
			Line(Udm::ObjectImpl *impl) : ::FCSL::MgaObject(impl), UDM_OBJECT(impl) { }
			Line(const Line &master) : ::FCSL::MgaObject(master), UDM_OBJECT(master) { }
			static Line Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static Line Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			Line CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			Line CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::FCSL::SLSF::Line> Instances() { return Udm::InstantiatedAttr< ::FCSL::SLSF::Line>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::FCSL::SLSF::Line, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::FCSL::SLSF::Line, Pred>(impl);}

			Udm::DerivedAttr< ::FCSL::SLSF::Line> Derived() { return Udm::DerivedAttr< ::FCSL::SLSF::Line>(impl);}
			template <class Pred> Udm::DerivedAttr< ::FCSL::SLSF::Line, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::FCSL::SLSF::Line, Pred>(impl);}

			Udm::ArchetypeAttr< ::FCSL::SLSF::Line> Archetype() const { return Udm::ArchetypeAttr< ::FCSL::SLSF::Line>(impl);}

			static ::Uml::Attribute meta_Name;
			Udm::StringAttr Name() const { return Udm::StringAttr(impl, meta_Name); }

			static ::Uml::CompositionParentRole meta_Line_Block_parent;
			Udm::ParentAttr< ::FCSL::SLSF::Block> Line_Block_parent() const { return Udm::ParentAttr< ::FCSL::SLSF::Block>(impl, meta_Line_Block_parent); }

			Udm::ParentAttr< ::FCSL::MgaObject> parent() const { return Udm::ParentAttr< ::FCSL::MgaObject>(impl, Udm::NULLPARENTROLE); }
			static ::Uml::AssociationRole meta_dstLine_end_;
			Udm::AssocEndAttr< ::FCSL::SLSF::Port> dstLine_end() const { return Udm::AssocEndAttr< ::FCSL::SLSF::Port>(impl, meta_dstLine_end_); }

			static ::Uml::AssociationRole meta_srcLine_end_;
			Udm::AssocEndAttr< ::FCSL::SLSF::Port> srcLine_end() const { return Udm::AssocEndAttr< ::FCSL::SLSF::Port>(impl, meta_srcLine_end_); }

		};

		class  Primitive :  public ::FCSL::SLSF::Block {
		public:
			static ::Uml::Class meta;

			Primitive() { }
			Primitive(Udm::ObjectImpl *impl) : ::FCSL::SLSF::Block(impl), UDM_OBJECT(impl) { }
			Primitive(const Primitive &master) : ::FCSL::SLSF::Block(master), UDM_OBJECT(master) { }
			static Primitive Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static Primitive Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			Primitive CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			Primitive CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::FCSL::SLSF::Primitive> Instances() { return Udm::InstantiatedAttr< ::FCSL::SLSF::Primitive>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::FCSL::SLSF::Primitive, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::FCSL::SLSF::Primitive, Pred>(impl);}

			Udm::DerivedAttr< ::FCSL::SLSF::Primitive> Derived() { return Udm::DerivedAttr< ::FCSL::SLSF::Primitive>(impl);}
			template <class Pred> Udm::DerivedAttr< ::FCSL::SLSF::Primitive, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::FCSL::SLSF::Primitive, Pred>(impl);}

			Udm::ArchetypeAttr< ::FCSL::SLSF::Primitive> Archetype() const { return Udm::ArchetypeAttr< ::FCSL::SLSF::Primitive>(impl);}

			static ::Uml::Attribute meta_Deadline;
			Udm::IntegerAttr Deadline() const { return Udm::IntegerAttr(impl, meta_Deadline); }

			static ::Uml::Attribute meta_Period;
			Udm::IntegerAttr Period() const { return Udm::IntegerAttr(impl, meta_Period); }

			static ::Uml::Attribute meta_ExecutionTime;
			Udm::IntegerAttr ExecutionTime() const { return Udm::IntegerAttr(impl, meta_ExecutionTime); }

			Udm::ParentAttr< ::FCSL::MgaObject> parent() const { return Udm::ParentAttr< ::FCSL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  Port :  public ::FCSL::EventPortMapping_Members_Base, public ::FCSL::PortMapping_Members_Base, public ::FCSL::MgaObject {
		public:
			static ::Uml::Class meta;

			Port() { }
			Port(Udm::ObjectImpl *impl) : ::FCSL::EventPortMapping_Members_Base(impl),::FCSL::PortMapping_Members_Base(impl),::FCSL::MgaObject(impl), UDM_OBJECT(impl) { }
			Port(const Port &master) : ::FCSL::EventPortMapping_Members_Base(master),::FCSL::PortMapping_Members_Base(master),::FCSL::MgaObject(master), UDM_OBJECT(master) { }
			static Port Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static Port Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			Port CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			Port CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::FCSL::SLSF::Port> Instances() { return Udm::InstantiatedAttr< ::FCSL::SLSF::Port>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::FCSL::SLSF::Port, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::FCSL::SLSF::Port, Pred>(impl);}

			Udm::DerivedAttr< ::FCSL::SLSF::Port> Derived() { return Udm::DerivedAttr< ::FCSL::SLSF::Port>(impl);}
			template <class Pred> Udm::DerivedAttr< ::FCSL::SLSF::Port, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::FCSL::SLSF::Port, Pred>(impl);}

			Udm::ArchetypeAttr< ::FCSL::SLSF::Port> Archetype() const { return Udm::ArchetypeAttr< ::FCSL::SLSF::Port>(impl);}

			static ::Uml::AssociationRole meta_srcLine;
			static ::Uml::AssociationRole meta_srcLine_rev;
			Udm::AClassAssocAttr< ::FCSL::SLSF::Line, ::FCSL::SLSF::Port> srcLine() const { return Udm::AClassAssocAttr< ::FCSL::SLSF::Line, ::FCSL::SLSF::Port>(impl, meta_srcLine, meta_srcLine_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::FCSL::SLSF::Line, ::FCSL::SLSF::Port, Pred> srcLine_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::FCSL::SLSF::Line, ::FCSL::SLSF::Port, Pred>(impl, meta_srcLine, meta_srcLine_rev); }

			static ::Uml::AssociationRole meta_dstLine;
			static ::Uml::AssociationRole meta_dstLine_rev;
			Udm::AClassAssocAttr< ::FCSL::SLSF::Line, ::FCSL::SLSF::Port> dstLine() const { return Udm::AClassAssocAttr< ::FCSL::SLSF::Line, ::FCSL::SLSF::Port>(impl, meta_dstLine, meta_dstLine_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::FCSL::SLSF::Line, ::FCSL::SLSF::Port, Pred> dstLine_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::FCSL::SLSF::Line, ::FCSL::SLSF::Port, Pred>(impl, meta_dstLine, meta_dstLine_rev); }

			static ::Uml::CompositionChildRole meta_TypeBaseRef_child;
			Udm::ChildAttr< ::FCSL::SLSF::TypeBaseRef> TypeBaseRef_child() const { return Udm::ChildAttr< ::FCSL::SLSF::TypeBaseRef>(impl, meta_TypeBaseRef_child); }

			Udm::ChildrenAttr< ::FCSL::SLSF::TypeBaseRef> TypeBaseRef_kind_children() const { return Udm::ChildrenAttr< ::FCSL::SLSF::TypeBaseRef>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::SLSF::TypeBaseRef, Pred> TypeBaseRef_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::SLSF::TypeBaseRef, Pred>(impl, Udm::NULLCHILDROLE); }

			static ::Uml::CompositionParentRole meta_Block_parent;
			Udm::ParentAttr< ::FCSL::SLSF::Block> Block_parent() const { return Udm::ParentAttr< ::FCSL::SLSF::Block>(impl, meta_Block_parent); }

			Udm::ParentAttr< ::FCSL::MgaObject> parent() const { return Udm::ParentAttr< ::FCSL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  OutPort :  public ::FCSL::SLSF::Port {
		public:
			static ::Uml::Class meta;

			OutPort() { }
			OutPort(Udm::ObjectImpl *impl) : ::FCSL::SLSF::Port(impl), UDM_OBJECT(impl) { }
			OutPort(const OutPort &master) : ::FCSL::SLSF::Port(master), UDM_OBJECT(master) { }
			static OutPort Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static OutPort Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			OutPort CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			OutPort CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::FCSL::SLSF::OutPort> Instances() { return Udm::InstantiatedAttr< ::FCSL::SLSF::OutPort>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::FCSL::SLSF::OutPort, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::FCSL::SLSF::OutPort, Pred>(impl);}

			Udm::DerivedAttr< ::FCSL::SLSF::OutPort> Derived() { return Udm::DerivedAttr< ::FCSL::SLSF::OutPort>(impl);}
			template <class Pred> Udm::DerivedAttr< ::FCSL::SLSF::OutPort, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::FCSL::SLSF::OutPort, Pred>(impl);}

			Udm::ArchetypeAttr< ::FCSL::SLSF::OutPort> Archetype() const { return Udm::ArchetypeAttr< ::FCSL::SLSF::OutPort>(impl);}

			Udm::ParentAttr< ::FCSL::MgaObject> parent() const { return Udm::ParentAttr< ::FCSL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  OutputPort :  public ::FCSL::SLSF::OutPort {
		public:
			static ::Uml::Class meta;

			OutputPort() { }
			OutputPort(Udm::ObjectImpl *impl) : ::FCSL::SLSF::OutPort(impl), UDM_OBJECT(impl) { }
			OutputPort(const OutputPort &master) : ::FCSL::SLSF::OutPort(master), UDM_OBJECT(master) { }
			static OutputPort Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static OutputPort Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			OutputPort CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			OutputPort CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::FCSL::SLSF::OutputPort> Instances() { return Udm::InstantiatedAttr< ::FCSL::SLSF::OutputPort>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::FCSL::SLSF::OutputPort, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::FCSL::SLSF::OutputPort, Pred>(impl);}

			Udm::DerivedAttr< ::FCSL::SLSF::OutputPort> Derived() { return Udm::DerivedAttr< ::FCSL::SLSF::OutputPort>(impl);}
			template <class Pred> Udm::DerivedAttr< ::FCSL::SLSF::OutputPort, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::FCSL::SLSF::OutputPort, Pred>(impl);}

			Udm::ArchetypeAttr< ::FCSL::SLSF::OutputPort> Archetype() const { return Udm::ArchetypeAttr< ::FCSL::SLSF::OutputPort>(impl);}

			static ::Uml::Attribute meta_Number;
			Udm::IntegerAttr Number() const { return Udm::IntegerAttr(impl, meta_Number); }

			Udm::ParentAttr< ::FCSL::MgaObject> parent() const { return Udm::ParentAttr< ::FCSL::MgaObject>(impl, Udm::NULLPARENTROLE); }
			static ::Uml::Constraint meta_NestedOPtoOPorNestedOPtoIP;
		};

		class  InPort :  public ::FCSL::SLSF::Port {
		public:
			static ::Uml::Class meta;

			InPort() { }
			InPort(Udm::ObjectImpl *impl) : ::FCSL::SLSF::Port(impl), UDM_OBJECT(impl) { }
			InPort(const InPort &master) : ::FCSL::SLSF::Port(master), UDM_OBJECT(master) { }
			static InPort Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static InPort Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			InPort CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			InPort CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::FCSL::SLSF::InPort> Instances() { return Udm::InstantiatedAttr< ::FCSL::SLSF::InPort>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::FCSL::SLSF::InPort, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::FCSL::SLSF::InPort, Pred>(impl);}

			Udm::DerivedAttr< ::FCSL::SLSF::InPort> Derived() { return Udm::DerivedAttr< ::FCSL::SLSF::InPort>(impl);}
			template <class Pred> Udm::DerivedAttr< ::FCSL::SLSF::InPort, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::FCSL::SLSF::InPort, Pred>(impl);}

			Udm::ArchetypeAttr< ::FCSL::SLSF::InPort> Archetype() const { return Udm::ArchetypeAttr< ::FCSL::SLSF::InPort>(impl);}

			Udm::ParentAttr< ::FCSL::MgaObject> parent() const { return Udm::ParentAttr< ::FCSL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  EnablePort :  public ::FCSL::SLSF::InPort {
		public:
			static ::Uml::Class meta;

			EnablePort() { }
			EnablePort(Udm::ObjectImpl *impl) : ::FCSL::SLSF::InPort(impl), UDM_OBJECT(impl) { }
			EnablePort(const EnablePort &master) : ::FCSL::SLSF::InPort(master), UDM_OBJECT(master) { }
			static EnablePort Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static EnablePort Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			EnablePort CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			EnablePort CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::FCSL::SLSF::EnablePort> Instances() { return Udm::InstantiatedAttr< ::FCSL::SLSF::EnablePort>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::FCSL::SLSF::EnablePort, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::FCSL::SLSF::EnablePort, Pred>(impl);}

			Udm::DerivedAttr< ::FCSL::SLSF::EnablePort> Derived() { return Udm::DerivedAttr< ::FCSL::SLSF::EnablePort>(impl);}
			template <class Pred> Udm::DerivedAttr< ::FCSL::SLSF::EnablePort, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::FCSL::SLSF::EnablePort, Pred>(impl);}

			Udm::ArchetypeAttr< ::FCSL::SLSF::EnablePort> Archetype() const { return Udm::ArchetypeAttr< ::FCSL::SLSF::EnablePort>(impl);}

			static ::Uml::Attribute meta_StatesWhenEnabling;
			Udm::StringAttr StatesWhenEnabling() const { return Udm::StringAttr(impl, meta_StatesWhenEnabling); }

			Udm::ParentAttr< ::FCSL::MgaObject> parent() const { return Udm::ParentAttr< ::FCSL::MgaObject>(impl, Udm::NULLPARENTROLE); }
			static ::Uml::Constraint meta_EPParentIsSubsystemOrSystem;
			static ::Uml::Constraint meta_EPInSubsystemOnlyConnectToEPInSystem;
			static ::Uml::Constraint meta_EPConnectedByPortInParentParent;
			static ::Uml::Constraint meta_EPtoNestedIPTPEPBPOnly;
			static ::Uml::Constraint meta_EPConnectedAsDstAtLeastOnce;
			static ::Uml::Constraint meta_OnlyOneEP;
			static ::Uml::Constraint meta_EnableParentInSubsystemMustConnectDownToSystem;
		};

		class  InputPort :  public ::FCSL::SLSF::InPort {
		public:
			static ::Uml::Class meta;

			InputPort() { }
			InputPort(Udm::ObjectImpl *impl) : ::FCSL::SLSF::InPort(impl), UDM_OBJECT(impl) { }
			InputPort(const InputPort &master) : ::FCSL::SLSF::InPort(master), UDM_OBJECT(master) { }
			static InputPort Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static InputPort Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			InputPort CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			InputPort CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::FCSL::SLSF::InputPort> Instances() { return Udm::InstantiatedAttr< ::FCSL::SLSF::InputPort>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::FCSL::SLSF::InputPort, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::FCSL::SLSF::InputPort, Pred>(impl);}

			Udm::DerivedAttr< ::FCSL::SLSF::InputPort> Derived() { return Udm::DerivedAttr< ::FCSL::SLSF::InputPort>(impl);}
			template <class Pred> Udm::DerivedAttr< ::FCSL::SLSF::InputPort, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::FCSL::SLSF::InputPort, Pred>(impl);}

			Udm::ArchetypeAttr< ::FCSL::SLSF::InputPort> Archetype() const { return Udm::ArchetypeAttr< ::FCSL::SLSF::InputPort>(impl);}

			static ::Uml::Attribute meta_Number;
			Udm::IntegerAttr Number() const { return Udm::IntegerAttr(impl, meta_Number); }

			Udm::ParentAttr< ::FCSL::MgaObject> parent() const { return Udm::ParentAttr< ::FCSL::MgaObject>(impl, Udm::NULLPARENTROLE); }
			static ::Uml::Constraint meta_IPtoNestedIPTPEPBPOnly;
			static ::Uml::Constraint meta_IPInSubsystemOnlyConnectToIPInSystem;
		};

		class  StatePort :  public ::FCSL::SLSF::OutPort {
		public:
			static ::Uml::Class meta;

			StatePort() { }
			StatePort(Udm::ObjectImpl *impl) : ::FCSL::SLSF::OutPort(impl), UDM_OBJECT(impl) { }
			StatePort(const StatePort &master) : ::FCSL::SLSF::OutPort(master), UDM_OBJECT(master) { }
			static StatePort Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static StatePort Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			StatePort CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			StatePort CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::FCSL::SLSF::StatePort> Instances() { return Udm::InstantiatedAttr< ::FCSL::SLSF::StatePort>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::FCSL::SLSF::StatePort, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::FCSL::SLSF::StatePort, Pred>(impl);}

			Udm::DerivedAttr< ::FCSL::SLSF::StatePort> Derived() { return Udm::DerivedAttr< ::FCSL::SLSF::StatePort>(impl);}
			template <class Pred> Udm::DerivedAttr< ::FCSL::SLSF::StatePort, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::FCSL::SLSF::StatePort, Pred>(impl);}

			Udm::ArchetypeAttr< ::FCSL::SLSF::StatePort> Archetype() const { return Udm::ArchetypeAttr< ::FCSL::SLSF::StatePort>(impl);}

			Udm::ParentAttr< ::FCSL::MgaObject> parent() const { return Udm::ParentAttr< ::FCSL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  TriggerPort :  public ::FCSL::SLSF::InPort {
		public:
			static ::Uml::Class meta;

			TriggerPort() { }
			TriggerPort(Udm::ObjectImpl *impl) : ::FCSL::SLSF::InPort(impl), UDM_OBJECT(impl) { }
			TriggerPort(const TriggerPort &master) : ::FCSL::SLSF::InPort(master), UDM_OBJECT(master) { }
			static TriggerPort Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static TriggerPort Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			TriggerPort CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			TriggerPort CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::FCSL::SLSF::TriggerPort> Instances() { return Udm::InstantiatedAttr< ::FCSL::SLSF::TriggerPort>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::FCSL::SLSF::TriggerPort, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::FCSL::SLSF::TriggerPort, Pred>(impl);}

			Udm::DerivedAttr< ::FCSL::SLSF::TriggerPort> Derived() { return Udm::DerivedAttr< ::FCSL::SLSF::TriggerPort>(impl);}
			template <class Pred> Udm::DerivedAttr< ::FCSL::SLSF::TriggerPort, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::FCSL::SLSF::TriggerPort, Pred>(impl);}

			Udm::ArchetypeAttr< ::FCSL::SLSF::TriggerPort> Archetype() const { return Udm::ArchetypeAttr< ::FCSL::SLSF::TriggerPort>(impl);}

			static ::Uml::Attribute meta_TriggerType;
			Udm::StringAttr TriggerType() const { return Udm::StringAttr(impl, meta_TriggerType); }

			Udm::ParentAttr< ::FCSL::MgaObject> parent() const { return Udm::ParentAttr< ::FCSL::MgaObject>(impl, Udm::NULLPARENTROLE); }
			static ::Uml::Constraint meta_TPParentIsSubsystemOrSystem;
			static ::Uml::Constraint meta_OnlyOneTP;
			static ::Uml::Constraint meta_TPConnectedByPortInParentParent;
			static ::Uml::Constraint meta_TPtoNestedIPTPEPBPOnly;
			static ::Uml::Constraint meta_TPConnectedAsDstAtLeastOnce;
			static ::Uml::Constraint meta_TPInSubsystemOnlyConnectToTPInSystem;
			static ::Uml::Constraint meta_TriggerParentInSubsystemMustConnectDownToSystem;
		};

		class  System :  public ::FCSL::SLSF::Block {
		public:
			static ::Uml::Class meta;

			System() { }
			System(Udm::ObjectImpl *impl) : ::FCSL::SLSF::Block(impl), UDM_OBJECT(impl) { }
			System(const System &master) : ::FCSL::SLSF::Block(master), UDM_OBJECT(master) { }
			static System Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static System Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			System CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			System CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::FCSL::SLSF::System> Instances() { return Udm::InstantiatedAttr< ::FCSL::SLSF::System>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::FCSL::SLSF::System, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::FCSL::SLSF::System, Pred>(impl);}

			Udm::DerivedAttr< ::FCSL::SLSF::System> Derived() { return Udm::DerivedAttr< ::FCSL::SLSF::System>(impl);}
			template <class Pred> Udm::DerivedAttr< ::FCSL::SLSF::System, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::FCSL::SLSF::System, Pred>(impl);}

			Udm::ArchetypeAttr< ::FCSL::SLSF::System> Archetype() const { return Udm::ArchetypeAttr< ::FCSL::SLSF::System>(impl);}

			static ::Uml::AssociationRole meta_referedbySystemRef;
			Udm::AssocAttr< ::FCSL::SystemRef> referedbySystemRef() const { return Udm::AssocAttr< ::FCSL::SystemRef>(impl, meta_referedbySystemRef); }
			template <class Pred> Udm::AssocAttr< ::FCSL::SystemRef, Pred > referedbySystemRef_sorted(const Pred &) const { return Udm::AssocAttr< ::FCSL::SystemRef, Pred>(impl, meta_referedbySystemRef); }

			static ::Uml::CompositionChildRole meta_Block_children;
			Udm::ChildrenAttr< ::FCSL::SLSF::Block> Block_children() const { return Udm::ChildrenAttr< ::FCSL::SLSF::Block>(impl, meta_Block_children); }
			template <class Pred> Udm::ChildrenAttr< ::FCSL::SLSF::Block, Pred> Block_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::SLSF::Block, Pred>(impl, meta_Block_children); }

			Udm::ChildrenAttr< ::FCSL::SLSF::Block> Block_kind_children() const { return Udm::ChildrenAttr< ::FCSL::SLSF::Block>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::SLSF::Block, Pred> Block_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::SLSF::Block, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::SLSF::Primitive> Primitive_kind_children() const { return Udm::ChildrenAttr< ::FCSL::SLSF::Primitive>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::SLSF::Primitive, Pred> Primitive_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::SLSF::Primitive, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::SLSF::System> System_kind_children() const { return Udm::ChildrenAttr< ::FCSL::SLSF::System>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::SLSF::System, Pred> System_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::SLSF::System, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::SLSF::Reference> Reference_kind_children() const { return Udm::ChildrenAttr< ::FCSL::SLSF::Reference>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::SLSF::Reference, Pred> Reference_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::SLSF::Reference, Pred>(impl, Udm::NULLCHILDROLE); }

			static ::Uml::CompositionParentRole meta_Dataflow_parent;
			Udm::ParentAttr< ::FCSL::SLSF::Dataflow> Dataflow_parent() const { return Udm::ParentAttr< ::FCSL::SLSF::Dataflow>(impl, meta_Dataflow_parent); }

			Udm::ParentAttr<Udm::Object> parent() const { return Udm::ParentAttr<Udm::Object>(impl, Udm::NULLPARENTROLE); }
			static ::Uml::Constraint meta_UniqueConnectorNames1;
		};

		class  Reference :  public ::FCSL::SLSF::Block {
		public:
			static ::Uml::Class meta;

			Reference() { }
			Reference(Udm::ObjectImpl *impl) : ::FCSL::SLSF::Block(impl), UDM_OBJECT(impl) { }
			Reference(const Reference &master) : ::FCSL::SLSF::Block(master), UDM_OBJECT(master) { }
			static Reference Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static Reference Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			Reference CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			Reference CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::FCSL::SLSF::Reference> Instances() { return Udm::InstantiatedAttr< ::FCSL::SLSF::Reference>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::FCSL::SLSF::Reference, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::FCSL::SLSF::Reference, Pred>(impl);}

			Udm::DerivedAttr< ::FCSL::SLSF::Reference> Derived() { return Udm::DerivedAttr< ::FCSL::SLSF::Reference>(impl);}
			template <class Pred> Udm::DerivedAttr< ::FCSL::SLSF::Reference, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::FCSL::SLSF::Reference, Pred>(impl);}

			Udm::ArchetypeAttr< ::FCSL::SLSF::Reference> Archetype() const { return Udm::ArchetypeAttr< ::FCSL::SLSF::Reference>(impl);}

			static ::Uml::Attribute meta_SourceType;
			Udm::StringAttr SourceType() const { return Udm::StringAttr(impl, meta_SourceType); }

			static ::Uml::Attribute meta_SourceBlock;
			Udm::StringAttr SourceBlock() const { return Udm::StringAttr(impl, meta_SourceBlock); }

			Udm::ParentAttr< ::FCSL::MgaObject> parent() const { return Udm::ParentAttr< ::FCSL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  TypeBase :  public ::FCSL::MgaObject {
		public:
			static ::Uml::Class meta;

			TypeBase() { }
			TypeBase(Udm::ObjectImpl *impl) : ::FCSL::MgaObject(impl), UDM_OBJECT(impl) { }
			TypeBase(const TypeBase &master) : ::FCSL::MgaObject(master), UDM_OBJECT(master) { }
			static TypeBase Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static TypeBase Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			TypeBase CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			TypeBase CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::FCSL::SLSF::TypeBase> Instances() { return Udm::InstantiatedAttr< ::FCSL::SLSF::TypeBase>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::FCSL::SLSF::TypeBase, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::FCSL::SLSF::TypeBase, Pred>(impl);}

			Udm::DerivedAttr< ::FCSL::SLSF::TypeBase> Derived() { return Udm::DerivedAttr< ::FCSL::SLSF::TypeBase>(impl);}
			template <class Pred> Udm::DerivedAttr< ::FCSL::SLSF::TypeBase, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::FCSL::SLSF::TypeBase, Pred>(impl);}

			Udm::ArchetypeAttr< ::FCSL::SLSF::TypeBase> Archetype() const { return Udm::ArchetypeAttr< ::FCSL::SLSF::TypeBase>(impl);}

			static ::Uml::AssociationRole meta_referedbyTypeStructRef;
			Udm::AssocAttr< ::FCSL::SLSF::TypeBaseRef> referedbyTypeStructRef() const { return Udm::AssocAttr< ::FCSL::SLSF::TypeBaseRef>(impl, meta_referedbyTypeStructRef); }
			template <class Pred> Udm::AssocAttr< ::FCSL::SLSF::TypeBaseRef, Pred > referedbyTypeStructRef_sorted(const Pred &) const { return Udm::AssocAttr< ::FCSL::SLSF::TypeBaseRef, Pred>(impl, meta_referedbyTypeStructRef); }

			static ::Uml::CompositionParentRole meta_Types_parent;
			Udm::ParentAttr< ::FCSL::SLSF::Types> Types_parent() const { return Udm::ParentAttr< ::FCSL::SLSF::Types>(impl, meta_Types_parent); }

			Udm::ParentAttr< ::FCSL::SLSF::Types> parent() const { return Udm::ParentAttr< ::FCSL::SLSF::Types>(impl, Udm::NULLPARENTROLE); }
		};

		class  Matrix :  public ::FCSL::SLSF::TypeBase {
		public:
			static ::Uml::Class meta;

			Matrix() { }
			Matrix(Udm::ObjectImpl *impl) : ::FCSL::SLSF::TypeBase(impl), UDM_OBJECT(impl) { }
			Matrix(const Matrix &master) : ::FCSL::SLSF::TypeBase(master), UDM_OBJECT(master) { }
			static Matrix Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static Matrix Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			Matrix CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			Matrix CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::FCSL::SLSF::Matrix> Instances() { return Udm::InstantiatedAttr< ::FCSL::SLSF::Matrix>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::FCSL::SLSF::Matrix, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::FCSL::SLSF::Matrix, Pred>(impl);}

			Udm::DerivedAttr< ::FCSL::SLSF::Matrix> Derived() { return Udm::DerivedAttr< ::FCSL::SLSF::Matrix>(impl);}
			template <class Pred> Udm::DerivedAttr< ::FCSL::SLSF::Matrix, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::FCSL::SLSF::Matrix, Pred>(impl);}

			Udm::ArchetypeAttr< ::FCSL::SLSF::Matrix> Archetype() const { return Udm::ArchetypeAttr< ::FCSL::SLSF::Matrix>(impl);}

			static ::Uml::Attribute meta_rows;
			Udm::IntegerAttr rows() const { return Udm::IntegerAttr(impl, meta_rows); }

			static ::Uml::Attribute meta_columns;
			Udm::IntegerAttr columns() const { return Udm::IntegerAttr(impl, meta_columns); }

			static ::Uml::Attribute meta_Type;
			Udm::StringAttr Type() const { return Udm::StringAttr(impl, meta_Type); }

			Udm::ParentAttr< ::FCSL::SLSF::Types> parent() const { return Udm::ParentAttr< ::FCSL::SLSF::Types>(impl, Udm::NULLPARENTROLE); }
		};

		class  TypeBaseRef :  public ::FCSL::MgaObject {
		public:
			static ::Uml::Class meta;

			TypeBaseRef() { }
			TypeBaseRef(Udm::ObjectImpl *impl) : ::FCSL::MgaObject(impl), UDM_OBJECT(impl) { }
			TypeBaseRef(const TypeBaseRef &master) : ::FCSL::MgaObject(master), UDM_OBJECT(master) { }
			static TypeBaseRef Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static TypeBaseRef Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			TypeBaseRef CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			TypeBaseRef CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::FCSL::SLSF::TypeBaseRef> Instances() { return Udm::InstantiatedAttr< ::FCSL::SLSF::TypeBaseRef>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::FCSL::SLSF::TypeBaseRef, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::FCSL::SLSF::TypeBaseRef, Pred>(impl);}

			Udm::DerivedAttr< ::FCSL::SLSF::TypeBaseRef> Derived() { return Udm::DerivedAttr< ::FCSL::SLSF::TypeBaseRef>(impl);}
			template <class Pred> Udm::DerivedAttr< ::FCSL::SLSF::TypeBaseRef, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::FCSL::SLSF::TypeBaseRef, Pred>(impl);}

			Udm::ArchetypeAttr< ::FCSL::SLSF::TypeBaseRef> Archetype() const { return Udm::ArchetypeAttr< ::FCSL::SLSF::TypeBaseRef>(impl);}

			static ::Uml::Attribute meta_MemberIndex;
			Udm::IntegerAttr MemberIndex() const { return Udm::IntegerAttr(impl, meta_MemberIndex); }

			static ::Uml::AssociationRole meta_ref;
			Udm::PointerAttr< ::FCSL::SLSF::TypeBase> ref() const { return Udm::PointerAttr< ::FCSL::SLSF::TypeBase>(impl, meta_ref); }

			static ::Uml::CompositionParentRole meta_Port_parent;
			Udm::ParentAttr< ::FCSL::SLSF::Port> Port_parent() const { return Udm::ParentAttr< ::FCSL::SLSF::Port>(impl, meta_Port_parent); }

			static ::Uml::CompositionParentRole meta_TypeStruct_parent;
			Udm::ParentAttr< ::FCSL::SLSF::TypeStruct> TypeStruct_parent() const { return Udm::ParentAttr< ::FCSL::SLSF::TypeStruct>(impl, meta_TypeStruct_parent); }

			Udm::ParentAttr< ::FCSL::MgaObject> parent() const { return Udm::ParentAttr< ::FCSL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  TypeStruct :  public ::FCSL::SLSF::TypeBase {
		public:
			static ::Uml::Class meta;

			TypeStruct() { }
			TypeStruct(Udm::ObjectImpl *impl) : ::FCSL::SLSF::TypeBase(impl), UDM_OBJECT(impl) { }
			TypeStruct(const TypeStruct &master) : ::FCSL::SLSF::TypeBase(master), UDM_OBJECT(master) { }
			static TypeStruct Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static TypeStruct Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			TypeStruct CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			TypeStruct CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::FCSL::SLSF::TypeStruct> Instances() { return Udm::InstantiatedAttr< ::FCSL::SLSF::TypeStruct>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::FCSL::SLSF::TypeStruct, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::FCSL::SLSF::TypeStruct, Pred>(impl);}

			Udm::DerivedAttr< ::FCSL::SLSF::TypeStruct> Derived() { return Udm::DerivedAttr< ::FCSL::SLSF::TypeStruct>(impl);}
			template <class Pred> Udm::DerivedAttr< ::FCSL::SLSF::TypeStruct, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::FCSL::SLSF::TypeStruct, Pred>(impl);}

			Udm::ArchetypeAttr< ::FCSL::SLSF::TypeStruct> Archetype() const { return Udm::ArchetypeAttr< ::FCSL::SLSF::TypeStruct>(impl);}

			static ::Uml::Attribute meta_MemberCount;
			Udm::IntegerAttr MemberCount() const { return Udm::IntegerAttr(impl, meta_MemberCount); }

			static ::Uml::CompositionChildRole meta_TypeBaseRef_children;
			Udm::ChildrenAttr< ::FCSL::SLSF::TypeBaseRef> TypeBaseRef_children() const { return Udm::ChildrenAttr< ::FCSL::SLSF::TypeBaseRef>(impl, meta_TypeBaseRef_children); }
			template <class Pred> Udm::ChildrenAttr< ::FCSL::SLSF::TypeBaseRef, Pred> TypeBaseRef_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::SLSF::TypeBaseRef, Pred>(impl, meta_TypeBaseRef_children); }

			Udm::ChildrenAttr< ::FCSL::SLSF::TypeBaseRef> TypeBaseRef_kind_children() const { return Udm::ChildrenAttr< ::FCSL::SLSF::TypeBaseRef>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::SLSF::TypeBaseRef, Pred> TypeBaseRef_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::SLSF::TypeBaseRef, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ParentAttr< ::FCSL::SLSF::Types> parent() const { return Udm::ParentAttr< ::FCSL::SLSF::Types>(impl, Udm::NULLPARENTROLE); }
		};

		class  Data :  public ::FCSL::MgaObject {
		public:
			static ::Uml::Class meta;

			Data() { }
			Data(Udm::ObjectImpl *impl) : ::FCSL::MgaObject(impl), UDM_OBJECT(impl) { }
			Data(const Data &master) : ::FCSL::MgaObject(master), UDM_OBJECT(master) { }
			static Data Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static Data Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			Data CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			Data CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::FCSL::SLSF::Data> Instances() { return Udm::InstantiatedAttr< ::FCSL::SLSF::Data>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::FCSL::SLSF::Data, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::FCSL::SLSF::Data, Pred>(impl);}

			Udm::DerivedAttr< ::FCSL::SLSF::Data> Derived() { return Udm::DerivedAttr< ::FCSL::SLSF::Data>(impl);}
			template <class Pred> Udm::DerivedAttr< ::FCSL::SLSF::Data, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::FCSL::SLSF::Data, Pred>(impl);}

			Udm::ArchetypeAttr< ::FCSL::SLSF::Data> Archetype() const { return Udm::ArchetypeAttr< ::FCSL::SLSF::Data>(impl);}

			static ::Uml::Attribute meta_Description;
			Udm::StringAttr Description() const { return Udm::StringAttr(impl, meta_Description); }

			static ::Uml::Attribute meta_Port;
			Udm::IntegerAttr Port() const { return Udm::IntegerAttr(impl, meta_Port); }

			static ::Uml::Attribute meta_Scope;
			Udm::StringAttr Scope() const { return Udm::StringAttr(impl, meta_Scope); }

			static ::Uml::Attribute meta_Name;
			Udm::StringAttr Name() const { return Udm::StringAttr(impl, meta_Name); }

			static ::Uml::Attribute meta_DataType;
			Udm::StringAttr DataType() const { return Udm::StringAttr(impl, meta_DataType); }

			static ::Uml::Attribute meta_Units;
			Udm::StringAttr Units() const { return Udm::StringAttr(impl, meta_Units); }

			static ::Uml::Attribute meta_Max;
			Udm::StringAttr Max() const { return Udm::StringAttr(impl, meta_Max); }

			static ::Uml::Attribute meta_Min;
			Udm::StringAttr Min() const { return Udm::StringAttr(impl, meta_Min); }

			static ::Uml::Attribute meta_InitialValue;
			Udm::StringAttr InitialValue() const { return Udm::StringAttr(impl, meta_InitialValue); }

			static ::Uml::Attribute meta_ArrayFirstIndex;
			Udm::IntegerAttr ArrayFirstIndex() const { return Udm::IntegerAttr(impl, meta_ArrayFirstIndex); }

			static ::Uml::Attribute meta_ArraySize;
			Udm::IntegerAttr ArraySize() const { return Udm::IntegerAttr(impl, meta_ArraySize); }

			static ::Uml::CompositionParentRole meta_State_parent;
			Udm::ParentAttr< ::FCSL::SLSF::State> State_parent() const { return Udm::ParentAttr< ::FCSL::SLSF::State>(impl, meta_State_parent); }

			Udm::ParentAttr< ::FCSL::MgaObject> parent() const { return Udm::ParentAttr< ::FCSL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  Event :  public ::FCSL::MgaObject {
		public:
			static ::Uml::Class meta;

			Event() { }
			Event(Udm::ObjectImpl *impl) : ::FCSL::MgaObject(impl), UDM_OBJECT(impl) { }
			Event(const Event &master) : ::FCSL::MgaObject(master), UDM_OBJECT(master) { }
			static Event Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static Event Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			Event CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			Event CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::FCSL::SLSF::Event> Instances() { return Udm::InstantiatedAttr< ::FCSL::SLSF::Event>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::FCSL::SLSF::Event, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::FCSL::SLSF::Event, Pred>(impl);}

			Udm::DerivedAttr< ::FCSL::SLSF::Event> Derived() { return Udm::DerivedAttr< ::FCSL::SLSF::Event>(impl);}
			template <class Pred> Udm::DerivedAttr< ::FCSL::SLSF::Event, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::FCSL::SLSF::Event, Pred>(impl);}

			Udm::ArchetypeAttr< ::FCSL::SLSF::Event> Archetype() const { return Udm::ArchetypeAttr< ::FCSL::SLSF::Event>(impl);}

			static ::Uml::Attribute meta_Description;
			Udm::StringAttr Description() const { return Udm::StringAttr(impl, meta_Description); }

			static ::Uml::Attribute meta_Port;
			Udm::IntegerAttr Port() const { return Udm::IntegerAttr(impl, meta_Port); }

			static ::Uml::Attribute meta_Scope;
			Udm::StringAttr Scope() const { return Udm::StringAttr(impl, meta_Scope); }

			static ::Uml::Attribute meta_Name;
			Udm::StringAttr Name() const { return Udm::StringAttr(impl, meta_Name); }

			static ::Uml::Attribute meta_Trigger;
			Udm::StringAttr Trigger() const { return Udm::StringAttr(impl, meta_Trigger); }

			static ::Uml::CompositionParentRole meta_State_parent;
			Udm::ParentAttr< ::FCSL::SLSF::State> State_parent() const { return Udm::ParentAttr< ::FCSL::SLSF::State>(impl, meta_State_parent); }

			Udm::ParentAttr< ::FCSL::MgaObject> parent() const { return Udm::ParentAttr< ::FCSL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  Transition :  public ::FCSL::MgaObject {
		public:
			static ::Uml::Class meta;

			Transition() { }
			Transition(Udm::ObjectImpl *impl) : ::FCSL::MgaObject(impl), UDM_OBJECT(impl) { }
			Transition(const Transition &master) : ::FCSL::MgaObject(master), UDM_OBJECT(master) { }
			static Transition Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static Transition Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			Transition CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			Transition CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::FCSL::SLSF::Transition> Instances() { return Udm::InstantiatedAttr< ::FCSL::SLSF::Transition>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::FCSL::SLSF::Transition, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::FCSL::SLSF::Transition, Pred>(impl);}

			Udm::DerivedAttr< ::FCSL::SLSF::Transition> Derived() { return Udm::DerivedAttr< ::FCSL::SLSF::Transition>(impl);}
			template <class Pred> Udm::DerivedAttr< ::FCSL::SLSF::Transition, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::FCSL::SLSF::Transition, Pred>(impl);}

			Udm::ArchetypeAttr< ::FCSL::SLSF::Transition> Archetype() const { return Udm::ArchetypeAttr< ::FCSL::SLSF::Transition>(impl);}

			static ::Uml::Attribute meta_ConditionAction;
			Udm::StringAttr ConditionAction() const { return Udm::StringAttr(impl, meta_ConditionAction); }

			static ::Uml::Attribute meta_Order;
			Udm::StringAttr Order() const { return Udm::StringAttr(impl, meta_Order); }

			static ::Uml::Attribute meta_Action;
			Udm::StringAttr Action() const { return Udm::StringAttr(impl, meta_Action); }

			static ::Uml::Attribute meta_Guard;
			Udm::StringAttr Guard() const { return Udm::StringAttr(impl, meta_Guard); }

			static ::Uml::Attribute meta_Trigger;
			Udm::StringAttr Trigger() const { return Udm::StringAttr(impl, meta_Trigger); }

			static ::Uml::CompositionParentRole meta_Transition_State_parent;
			Udm::ParentAttr< ::FCSL::SLSF::State> Transition_State_parent() const { return Udm::ParentAttr< ::FCSL::SLSF::State>(impl, meta_Transition_State_parent); }

			Udm::ParentAttr< ::FCSL::MgaObject> parent() const { return Udm::ParentAttr< ::FCSL::MgaObject>(impl, Udm::NULLPARENTROLE); }
			static ::Uml::AssociationRole meta_srcTransition_end_;
			Udm::AssocEndAttr< ::FCSL::SLSF::TransConnector> srcTransition_end() const { return Udm::AssocEndAttr< ::FCSL::SLSF::TransConnector>(impl, meta_srcTransition_end_); }

			static ::Uml::AssociationRole meta_dstTransition_end_;
			Udm::AssocEndAttr< ::FCSL::SLSF::TransConnector> dstTransition_end() const { return Udm::AssocEndAttr< ::FCSL::SLSF::TransConnector>(impl, meta_dstTransition_end_); }

		};

		class  TransConnector :  public ::FCSL::MgaObject {
		public:
			static ::Uml::Class meta;

			TransConnector() { }
			TransConnector(Udm::ObjectImpl *impl) : ::FCSL::MgaObject(impl), UDM_OBJECT(impl) { }
			TransConnector(const TransConnector &master) : ::FCSL::MgaObject(master), UDM_OBJECT(master) { }
			static TransConnector Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static TransConnector Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			TransConnector CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			TransConnector CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::FCSL::SLSF::TransConnector> Instances() { return Udm::InstantiatedAttr< ::FCSL::SLSF::TransConnector>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::FCSL::SLSF::TransConnector, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::FCSL::SLSF::TransConnector, Pred>(impl);}

			Udm::DerivedAttr< ::FCSL::SLSF::TransConnector> Derived() { return Udm::DerivedAttr< ::FCSL::SLSF::TransConnector>(impl);}
			template <class Pred> Udm::DerivedAttr< ::FCSL::SLSF::TransConnector, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::FCSL::SLSF::TransConnector, Pred>(impl);}

			Udm::ArchetypeAttr< ::FCSL::SLSF::TransConnector> Archetype() const { return Udm::ArchetypeAttr< ::FCSL::SLSF::TransConnector>(impl);}

			static ::Uml::AssociationRole meta_referedbyConnectorRef;
			Udm::AssocAttr< ::FCSL::SLSF::ConnectorRef> referedbyConnectorRef() const { return Udm::AssocAttr< ::FCSL::SLSF::ConnectorRef>(impl, meta_referedbyConnectorRef); }
			template <class Pred> Udm::AssocAttr< ::FCSL::SLSF::ConnectorRef, Pred > referedbyConnectorRef_sorted(const Pred &) const { return Udm::AssocAttr< ::FCSL::SLSF::ConnectorRef, Pred>(impl, meta_referedbyConnectorRef); }

			static ::Uml::AssociationRole meta_dstTransition;
			static ::Uml::AssociationRole meta_dstTransition_rev;
			Udm::AClassAssocAttr< ::FCSL::SLSF::Transition, ::FCSL::SLSF::TransConnector> dstTransition() const { return Udm::AClassAssocAttr< ::FCSL::SLSF::Transition, ::FCSL::SLSF::TransConnector>(impl, meta_dstTransition, meta_dstTransition_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::FCSL::SLSF::Transition, ::FCSL::SLSF::TransConnector, Pred> dstTransition_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::FCSL::SLSF::Transition, ::FCSL::SLSF::TransConnector, Pred>(impl, meta_dstTransition, meta_dstTransition_rev); }

			static ::Uml::AssociationRole meta_srcTransition;
			static ::Uml::AssociationRole meta_srcTransition_rev;
			Udm::AClassAssocAttr< ::FCSL::SLSF::Transition, ::FCSL::SLSF::TransConnector> srcTransition() const { return Udm::AClassAssocAttr< ::FCSL::SLSF::Transition, ::FCSL::SLSF::TransConnector>(impl, meta_srcTransition, meta_srcTransition_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::FCSL::SLSF::Transition, ::FCSL::SLSF::TransConnector, Pred> srcTransition_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::FCSL::SLSF::Transition, ::FCSL::SLSF::TransConnector, Pred>(impl, meta_srcTransition, meta_srcTransition_rev); }

			static ::Uml::CompositionParentRole meta_State_parent;
			Udm::ParentAttr< ::FCSL::SLSF::State> State_parent() const { return Udm::ParentAttr< ::FCSL::SLSF::State>(impl, meta_State_parent); }

			Udm::ParentAttr< ::FCSL::MgaObject> parent() const { return Udm::ParentAttr< ::FCSL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  ConnectorRef :  public ::FCSL::SLSF::TransConnector {
		public:
			static ::Uml::Class meta;

			ConnectorRef() { }
			ConnectorRef(Udm::ObjectImpl *impl) : ::FCSL::SLSF::TransConnector(impl), UDM_OBJECT(impl) { }
			ConnectorRef(const ConnectorRef &master) : ::FCSL::SLSF::TransConnector(master), UDM_OBJECT(master) { }
			static ConnectorRef Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static ConnectorRef Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			ConnectorRef CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			ConnectorRef CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::FCSL::SLSF::ConnectorRef> Instances() { return Udm::InstantiatedAttr< ::FCSL::SLSF::ConnectorRef>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::FCSL::SLSF::ConnectorRef, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::FCSL::SLSF::ConnectorRef, Pred>(impl);}

			Udm::DerivedAttr< ::FCSL::SLSF::ConnectorRef> Derived() { return Udm::DerivedAttr< ::FCSL::SLSF::ConnectorRef>(impl);}
			template <class Pred> Udm::DerivedAttr< ::FCSL::SLSF::ConnectorRef, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::FCSL::SLSF::ConnectorRef, Pred>(impl);}

			Udm::ArchetypeAttr< ::FCSL::SLSF::ConnectorRef> Archetype() const { return Udm::ArchetypeAttr< ::FCSL::SLSF::ConnectorRef>(impl);}

			static ::Uml::AssociationRole meta_ref;
			Udm::PointerAttr< ::FCSL::SLSF::TransConnector> ref() const { return Udm::PointerAttr< ::FCSL::SLSF::TransConnector>(impl, meta_ref); }

			static ::Uml::CompositionParentRole meta_Block_parent;
			Udm::ParentAttr< ::FCSL::SLSF::Block> Block_parent() const { return Udm::ParentAttr< ::FCSL::SLSF::Block>(impl, meta_Block_parent); }

			Udm::ParentAttr< ::FCSL::MgaObject> parent() const { return Udm::ParentAttr< ::FCSL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  State :  public ::FCSL::SLSF::TransConnector {
		public:
			static ::Uml::Class meta;

			State() { }
			State(Udm::ObjectImpl *impl) : ::FCSL::SLSF::TransConnector(impl), UDM_OBJECT(impl) { }
			State(const State &master) : ::FCSL::SLSF::TransConnector(master), UDM_OBJECT(master) { }
			static State Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static State Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			State CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			State CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::FCSL::SLSF::State> Instances() { return Udm::InstantiatedAttr< ::FCSL::SLSF::State>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::FCSL::SLSF::State, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::FCSL::SLSF::State, Pred>(impl);}

			Udm::DerivedAttr< ::FCSL::SLSF::State> Derived() { return Udm::DerivedAttr< ::FCSL::SLSF::State>(impl);}
			template <class Pred> Udm::DerivedAttr< ::FCSL::SLSF::State, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::FCSL::SLSF::State, Pred>(impl);}

			Udm::ArchetypeAttr< ::FCSL::SLSF::State> Archetype() const { return Udm::ArchetypeAttr< ::FCSL::SLSF::State>(impl);}

			static ::Uml::Attribute meta_ExitAction;
			Udm::StringAttr ExitAction() const { return Udm::StringAttr(impl, meta_ExitAction); }

			static ::Uml::Attribute meta_Order;
			Udm::StringAttr Order() const { return Udm::StringAttr(impl, meta_Order); }

			static ::Uml::Attribute meta_DuringAction;
			Udm::StringAttr DuringAction() const { return Udm::StringAttr(impl, meta_DuringAction); }

			static ::Uml::Attribute meta_EnterAction;
			Udm::StringAttr EnterAction() const { return Udm::StringAttr(impl, meta_EnterAction); }

			static ::Uml::Attribute meta_Name;
			Udm::StringAttr Name() const { return Udm::StringAttr(impl, meta_Name); }

			static ::Uml::Attribute meta_Decomposition;
			Udm::StringAttr Decomposition() const { return Udm::StringAttr(impl, meta_Decomposition); }

			static ::Uml::CompositionChildRole meta_Event_children;
			Udm::ChildrenAttr< ::FCSL::SLSF::Event> Event_children() const { return Udm::ChildrenAttr< ::FCSL::SLSF::Event>(impl, meta_Event_children); }
			template <class Pred> Udm::ChildrenAttr< ::FCSL::SLSF::Event, Pred> Event_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::SLSF::Event, Pred>(impl, meta_Event_children); }

			static ::Uml::CompositionChildRole meta_TransConnector_children;
			Udm::ChildrenAttr< ::FCSL::SLSF::TransConnector> TransConnector_children() const { return Udm::ChildrenAttr< ::FCSL::SLSF::TransConnector>(impl, meta_TransConnector_children); }
			template <class Pred> Udm::ChildrenAttr< ::FCSL::SLSF::TransConnector, Pred> TransConnector_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::SLSF::TransConnector, Pred>(impl, meta_TransConnector_children); }

			static ::Uml::CompositionChildRole meta_Transition;
			Udm::ChildrenAttr< ::FCSL::SLSF::Transition> Transition() const { return Udm::ChildrenAttr< ::FCSL::SLSF::Transition>(impl, meta_Transition); }
			template <class Pred> Udm::ChildrenAttr< ::FCSL::SLSF::Transition, Pred> Transition_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::SLSF::Transition, Pred>(impl, meta_Transition); }

			static ::Uml::CompositionChildRole meta_Data_children;
			Udm::ChildrenAttr< ::FCSL::SLSF::Data> Data_children() const { return Udm::ChildrenAttr< ::FCSL::SLSF::Data>(impl, meta_Data_children); }
			template <class Pred> Udm::ChildrenAttr< ::FCSL::SLSF::Data, Pred> Data_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::SLSF::Data, Pred>(impl, meta_Data_children); }

			Udm::ChildrenAttr< ::FCSL::SLSF::Data> Data_kind_children() const { return Udm::ChildrenAttr< ::FCSL::SLSF::Data>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::SLSF::Data, Pred> Data_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::SLSF::Data, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::SLSF::ConnectorRef> ConnectorRef_kind_children() const { return Udm::ChildrenAttr< ::FCSL::SLSF::ConnectorRef>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::SLSF::ConnectorRef, Pred> ConnectorRef_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::SLSF::ConnectorRef, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::SLSF::State> State_kind_children() const { return Udm::ChildrenAttr< ::FCSL::SLSF::State>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::SLSF::State, Pred> State_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::SLSF::State, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::SLSF::Event> Event_kind_children() const { return Udm::ChildrenAttr< ::FCSL::SLSF::Event>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::SLSF::Event, Pred> Event_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::SLSF::Event, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::SLSF::Transition> Transition_kind_children() const { return Udm::ChildrenAttr< ::FCSL::SLSF::Transition>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::SLSF::Transition, Pred> Transition_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::SLSF::Transition, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::SLSF::TransStart> TransStart_kind_children() const { return Udm::ChildrenAttr< ::FCSL::SLSF::TransStart>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::SLSF::TransStart, Pred> TransStart_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::SLSF::TransStart, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::SLSF::Junction> Junction_kind_children() const { return Udm::ChildrenAttr< ::FCSL::SLSF::Junction>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::SLSF::Junction, Pred> Junction_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::SLSF::Junction, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::SLSF::TransConnector> TransConnector_kind_children() const { return Udm::ChildrenAttr< ::FCSL::SLSF::TransConnector>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::SLSF::TransConnector, Pred> TransConnector_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::SLSF::TransConnector, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::FCSL::SLSF::History> History_kind_children() const { return Udm::ChildrenAttr< ::FCSL::SLSF::History>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::FCSL::SLSF::History, Pred> History_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::FCSL::SLSF::History, Pred>(impl, Udm::NULLCHILDROLE); }

			static ::Uml::CompositionParentRole meta_Stateflow_parent;
			Udm::ParentAttr< ::FCSL::SLSF::Stateflow> Stateflow_parent() const { return Udm::ParentAttr< ::FCSL::SLSF::Stateflow>(impl, meta_Stateflow_parent); }

			Udm::ParentAttr<Udm::Object> parent() const { return Udm::ParentAttr<Udm::Object>(impl, Udm::NULLPARENTROLE); }
			static ::Uml::Constraint meta_SingleHistory;
		};

		class  TransStart :  public ::FCSL::SLSF::TransConnector {
		public:
			static ::Uml::Class meta;

			TransStart() { }
			TransStart(Udm::ObjectImpl *impl) : ::FCSL::SLSF::TransConnector(impl), UDM_OBJECT(impl) { }
			TransStart(const TransStart &master) : ::FCSL::SLSF::TransConnector(master), UDM_OBJECT(master) { }
			static TransStart Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static TransStart Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			TransStart CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			TransStart CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::FCSL::SLSF::TransStart> Instances() { return Udm::InstantiatedAttr< ::FCSL::SLSF::TransStart>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::FCSL::SLSF::TransStart, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::FCSL::SLSF::TransStart, Pred>(impl);}

			Udm::DerivedAttr< ::FCSL::SLSF::TransStart> Derived() { return Udm::DerivedAttr< ::FCSL::SLSF::TransStart>(impl);}
			template <class Pred> Udm::DerivedAttr< ::FCSL::SLSF::TransStart, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::FCSL::SLSF::TransStart, Pred>(impl);}

			Udm::ArchetypeAttr< ::FCSL::SLSF::TransStart> Archetype() const { return Udm::ArchetypeAttr< ::FCSL::SLSF::TransStart>(impl);}

			Udm::ParentAttr< ::FCSL::MgaObject> parent() const { return Udm::ParentAttr< ::FCSL::MgaObject>(impl, Udm::NULLPARENTROLE); }
			static ::Uml::Constraint meta_TransStartRestrict;
		};

		class  Junction :  public ::FCSL::SLSF::TransConnector {
		public:
			static ::Uml::Class meta;

			Junction() { }
			Junction(Udm::ObjectImpl *impl) : ::FCSL::SLSF::TransConnector(impl), UDM_OBJECT(impl) { }
			Junction(const Junction &master) : ::FCSL::SLSF::TransConnector(master), UDM_OBJECT(master) { }
			static Junction Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static Junction Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			Junction CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			Junction CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::FCSL::SLSF::Junction> Instances() { return Udm::InstantiatedAttr< ::FCSL::SLSF::Junction>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::FCSL::SLSF::Junction, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::FCSL::SLSF::Junction, Pred>(impl);}

			Udm::DerivedAttr< ::FCSL::SLSF::Junction> Derived() { return Udm::DerivedAttr< ::FCSL::SLSF::Junction>(impl);}
			template <class Pred> Udm::DerivedAttr< ::FCSL::SLSF::Junction, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::FCSL::SLSF::Junction, Pred>(impl);}

			Udm::ArchetypeAttr< ::FCSL::SLSF::Junction> Archetype() const { return Udm::ArchetypeAttr< ::FCSL::SLSF::Junction>(impl);}

			Udm::ParentAttr< ::FCSL::MgaObject> parent() const { return Udm::ParentAttr< ::FCSL::MgaObject>(impl, Udm::NULLPARENTROLE); }
			static ::Uml::Constraint meta_JunctNotToSelf;
		};

		class  History :  public ::FCSL::SLSF::TransConnector {
		public:
			static ::Uml::Class meta;

			History() { }
			History(Udm::ObjectImpl *impl) : ::FCSL::SLSF::TransConnector(impl), UDM_OBJECT(impl) { }
			History(const History &master) : ::FCSL::SLSF::TransConnector(master), UDM_OBJECT(master) { }
			static History Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static History Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			History CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			History CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::FCSL::SLSF::History> Instances() { return Udm::InstantiatedAttr< ::FCSL::SLSF::History>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::FCSL::SLSF::History, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::FCSL::SLSF::History, Pred>(impl);}

			Udm::DerivedAttr< ::FCSL::SLSF::History> Derived() { return Udm::DerivedAttr< ::FCSL::SLSF::History>(impl);}
			template <class Pred> Udm::DerivedAttr< ::FCSL::SLSF::History, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::FCSL::SLSF::History, Pred>(impl);}

			Udm::ArchetypeAttr< ::FCSL::SLSF::History> Archetype() const { return Udm::ArchetypeAttr< ::FCSL::SLSF::History>(impl);}

			Udm::ParentAttr< ::FCSL::MgaObject> parent() const { return Udm::ParentAttr< ::FCSL::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

	}


}

#endif //MOBIES_FCSL_H
