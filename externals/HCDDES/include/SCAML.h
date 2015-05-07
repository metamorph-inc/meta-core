#ifndef MOBIES_SCAML_H
#define MOBIES_SCAML_H
// header file SCAML.h generated from diagram SCAML
// generated on Mon Oct 29 12:27:19 2007

#ifndef MOBIES_UDMBASE_H
#include "UdmBase.h"
#endif

#ifdef min
#undef min
#endif

#ifdef max
#undef max
#endif
namespace SCAML {
	extern  Udm::UdmDiagram diagram;
	 void Initialize(const ::Uml::Diagram &dgr);
	 void Initialize();

		class  connectdestBase;
		class  componentplacement_Members_Base;
		class  usestodestConn;
		class  externaltodestConn;
		class  externalport;
		class  externaltodestBase;
		class  componentsupportedinterface;
		class  freestandingtoprovidedConn;
		class  usestofreestandingConn;
		class  thisPort;
		class  log;
		class  filemanager;
		class  namingservice;
		class  FreestandingPort;
		class  deviceusedbythiscomponentref;
		class  service;
		class  eventchannel;
		class  devicethatloadedthiscomponentref;
		class  Event;
		class  ControlFlow;
		class  CFSrc;
		class  ArgRef;
		class  Arg;
		class  SimulinkEntrypoint;
		class  Branch;
		class  CFDst;
		class  ThenFlow;
		class  ElseFlow;
		class  controllertocompinst;
		class  assemblycontroller;
		class  componentresourcefactorytopropertiesConn;
		class  findcomponentConn;
		class  componentresourcefactoryref;
		class  propertyProxy;
		class  componentpropertiesConn;
		class  componentproperties;
		class  componentplacementConn;
		class  hostcollocation;
		class  InterfaceFolder;
		class  usesPort;
		class  providesPort;
		class  supports;
		class  inheritsfrom;
		class  Interface;
		class  interfaces;
		class  port;
		class  componentfeatures;
		class  devicepkgref;
		class  hwdeviceregistration;
		class  childhwdevice;
		class  structBase;
		class  simpleBase;
		class  structvalue;
		class  simpleref;
		class  structref;
		class  structsequencePropertyDescriptor;
		class  structPropertyDescriptor;
		class  testPropertyDescriptor;
		class  kind;
		class  simplesequencePropertyDescriptor;
		class  property;
		class  simplePropertyDescriptor;
		class  enumeration;
		class  devicepkgfile;
		class  filesystemname;
		class  componentfile;
		class  compositepartofdevice;
		class  deployondevice;
		class  componentplacement;
		class  devconfigcomponentfile;
		class  devicemanagersoftpkg;
		class  usesImpl;
		class  dependency;
		class  usesdeviceref;
		class  usesdevice;
		class  propertyref;
		class  implref;
		class  simplepropertyref;
		class  softpkgref;
		class  runtime;
		class  processor;
		class  os;
		class  humanlanguage;
		class  compiler;
		class  programminglanguage;
		class  code;
		class  author;
		class  implementation;
		class  SimulinkSystem;
		class  DataInport;
		class  DataOutport;
		class  DataFlow;
		class  DFFCO;
		class  Var;
		class  Const;
		class  Function;
		class  SimulinkImplementation;
		class  DataPort;
		class  DataMapping;
		class  WrapperPart;
		class  SimulinkVars;
		class  propertyfile;
		class  SoftPkgRef;
		class  SoftPkgFolder;
		class  SoftwareComponentRef;
		class  SoftwareComponentFolder;
		class  DevicePkgRef;
		class  DevicePackageFolder;
		class  DeviceConfigurationFolder;
		class  PropertiesFolder;
		class  SoftwarePackageRef;
		class  SoftwareComponent;
		class  Properties;
		class  SoftPkg;
		class  DevicePkg;
		class  DomainProfile;
		class  DomainManagerConfiguration;
		class  SoftwareAssembly;
		class  DeviceConfiguration;
		class  RootFolder;
		class  MgaObject;


		 void CreateMetaObjs();
		 void InitCrossNSInheritence();
		 void InitCrossNSCompositions();
		 void InitCrossNSAssociations();
		class  componentplacement_Members_Base :  virtual public Udm::Object {
		public:
			static ::Uml::Class meta;

			componentplacement_Members_Base() { }
			componentplacement_Members_Base(Udm::ObjectImpl *impl) : UDM_OBJECT(impl) { }
			componentplacement_Members_Base(const componentplacement_Members_Base &master) : UDM_OBJECT(master) { }
			static componentplacement_Members_Base Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static componentplacement_Members_Base Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			componentplacement_Members_Base CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			componentplacement_Members_Base CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::SCAML::componentplacement_Members_Base> Instances() { return Udm::InstantiatedAttr< ::SCAML::componentplacement_Members_Base>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::SCAML::componentplacement_Members_Base, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::SCAML::componentplacement_Members_Base, Pred>(impl);}

			Udm::DerivedAttr< ::SCAML::componentplacement_Members_Base> Derived() { return Udm::DerivedAttr< ::SCAML::componentplacement_Members_Base>(impl);}
			template <class Pred> Udm::DerivedAttr< ::SCAML::componentplacement_Members_Base, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::SCAML::componentplacement_Members_Base, Pred>(impl);}

			Udm::ArchetypeAttr< ::SCAML::componentplacement_Members_Base> Archetype() const { return Udm::ArchetypeAttr< ::SCAML::componentplacement_Members_Base>(impl);}

			static ::Uml::AssociationRole meta_setcomponentplacement;
			Udm::AssocAttr< ::SCAML::componentplacement> setcomponentplacement() const { return Udm::AssocAttr< ::SCAML::componentplacement>(impl, meta_setcomponentplacement); }
			template <class Pred> Udm::AssocAttr< ::SCAML::componentplacement, Pred > setcomponentplacement_sorted(const Pred &) const { return Udm::AssocAttr< ::SCAML::componentplacement, Pred>(impl, meta_setcomponentplacement); }

			Udm::ParentAttr<Udm::Object> parent() const { return Udm::ParentAttr<Udm::Object>(impl, Udm::NULLPARENTROLE); }
		};

		class  InterfaceFolder :  public Udm::Object {
		public:
			static ::Uml::Class meta;

			InterfaceFolder() { }
			InterfaceFolder(Udm::ObjectImpl *impl) : UDM_OBJECT(impl) { }
			InterfaceFolder(const InterfaceFolder &master) : UDM_OBJECT(master) { }
			static InterfaceFolder Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static InterfaceFolder Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			InterfaceFolder CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			InterfaceFolder CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::SCAML::InterfaceFolder> Instances() { return Udm::InstantiatedAttr< ::SCAML::InterfaceFolder>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::SCAML::InterfaceFolder, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::SCAML::InterfaceFolder, Pred>(impl);}

			Udm::DerivedAttr< ::SCAML::InterfaceFolder> Derived() { return Udm::DerivedAttr< ::SCAML::InterfaceFolder>(impl);}
			template <class Pred> Udm::DerivedAttr< ::SCAML::InterfaceFolder, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::SCAML::InterfaceFolder, Pred>(impl);}

			Udm::ArchetypeAttr< ::SCAML::InterfaceFolder> Archetype() const { return Udm::ArchetypeAttr< ::SCAML::InterfaceFolder>(impl);}

			static ::Uml::Attribute meta_name;
			Udm::StringAttr name() const { return Udm::StringAttr(impl, meta_name); }

			static ::Uml::CompositionChildRole meta_interfaces_children;
			Udm::ChildrenAttr< ::SCAML::interfaces> interfaces_children() const { return Udm::ChildrenAttr< ::SCAML::interfaces>(impl, meta_interfaces_children); }
			template <class Pred> Udm::ChildrenAttr< ::SCAML::interfaces, Pred> interfaces_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::interfaces, Pred>(impl, meta_interfaces_children); }

			Udm::ChildrenAttr< ::SCAML::interfaces> interfaces_kind_children() const { return Udm::ChildrenAttr< ::SCAML::interfaces>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::interfaces, Pred> interfaces_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::interfaces, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr< ::SCAML::MgaObject>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

			static ::Uml::CompositionParentRole meta_RootFolder_parent;
			Udm::ParentAttr< ::SCAML::RootFolder> RootFolder_parent() const { return Udm::ParentAttr< ::SCAML::RootFolder>(impl, meta_RootFolder_parent); }

			Udm::ParentAttr< ::SCAML::RootFolder> parent() const { return Udm::ParentAttr< ::SCAML::RootFolder>(impl, Udm::NULLPARENTROLE); }
		};

		class  SoftPkgFolder :  public Udm::Object {
		public:
			static ::Uml::Class meta;

			SoftPkgFolder() { }
			SoftPkgFolder(Udm::ObjectImpl *impl) : UDM_OBJECT(impl) { }
			SoftPkgFolder(const SoftPkgFolder &master) : UDM_OBJECT(master) { }
			static SoftPkgFolder Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static SoftPkgFolder Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			SoftPkgFolder CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			SoftPkgFolder CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::SCAML::SoftPkgFolder> Instances() { return Udm::InstantiatedAttr< ::SCAML::SoftPkgFolder>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::SCAML::SoftPkgFolder, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::SCAML::SoftPkgFolder, Pred>(impl);}

			Udm::DerivedAttr< ::SCAML::SoftPkgFolder> Derived() { return Udm::DerivedAttr< ::SCAML::SoftPkgFolder>(impl);}
			template <class Pred> Udm::DerivedAttr< ::SCAML::SoftPkgFolder, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::SCAML::SoftPkgFolder, Pred>(impl);}

			Udm::ArchetypeAttr< ::SCAML::SoftPkgFolder> Archetype() const { return Udm::ArchetypeAttr< ::SCAML::SoftPkgFolder>(impl);}

			static ::Uml::Attribute meta_name;
			Udm::StringAttr name() const { return Udm::StringAttr(impl, meta_name); }

			static ::Uml::CompositionChildRole meta_SoftPkg_children;
			Udm::ChildrenAttr< ::SCAML::SoftPkg> SoftPkg_children() const { return Udm::ChildrenAttr< ::SCAML::SoftPkg>(impl, meta_SoftPkg_children); }
			template <class Pred> Udm::ChildrenAttr< ::SCAML::SoftPkg, Pred> SoftPkg_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::SoftPkg, Pred>(impl, meta_SoftPkg_children); }

			Udm::ChildrenAttr< ::SCAML::SoftPkg> SoftPkg_kind_children() const { return Udm::ChildrenAttr< ::SCAML::SoftPkg>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::SoftPkg, Pred> SoftPkg_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::SoftPkg, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr< ::SCAML::MgaObject>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

			static ::Uml::CompositionParentRole meta_RootFolder_parent;
			Udm::ParentAttr< ::SCAML::RootFolder> RootFolder_parent() const { return Udm::ParentAttr< ::SCAML::RootFolder>(impl, meta_RootFolder_parent); }

			static ::Uml::CompositionParentRole meta_DomainProfile_parent;
			Udm::ParentAttr< ::SCAML::DomainProfile> DomainProfile_parent() const { return Udm::ParentAttr< ::SCAML::DomainProfile>(impl, meta_DomainProfile_parent); }

			Udm::ParentAttr<Udm::Object> parent() const { return Udm::ParentAttr<Udm::Object>(impl, Udm::NULLPARENTROLE); }
		};

		class  SoftwareComponentFolder :  public Udm::Object {
		public:
			static ::Uml::Class meta;

			SoftwareComponentFolder() { }
			SoftwareComponentFolder(Udm::ObjectImpl *impl) : UDM_OBJECT(impl) { }
			SoftwareComponentFolder(const SoftwareComponentFolder &master) : UDM_OBJECT(master) { }
			static SoftwareComponentFolder Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static SoftwareComponentFolder Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			SoftwareComponentFolder CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			SoftwareComponentFolder CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::SCAML::SoftwareComponentFolder> Instances() { return Udm::InstantiatedAttr< ::SCAML::SoftwareComponentFolder>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::SCAML::SoftwareComponentFolder, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::SCAML::SoftwareComponentFolder, Pred>(impl);}

			Udm::DerivedAttr< ::SCAML::SoftwareComponentFolder> Derived() { return Udm::DerivedAttr< ::SCAML::SoftwareComponentFolder>(impl);}
			template <class Pred> Udm::DerivedAttr< ::SCAML::SoftwareComponentFolder, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::SCAML::SoftwareComponentFolder, Pred>(impl);}

			Udm::ArchetypeAttr< ::SCAML::SoftwareComponentFolder> Archetype() const { return Udm::ArchetypeAttr< ::SCAML::SoftwareComponentFolder>(impl);}

			static ::Uml::Attribute meta_name;
			Udm::StringAttr name() const { return Udm::StringAttr(impl, meta_name); }

			static ::Uml::CompositionChildRole meta_SoftwareComponent_children;
			Udm::ChildrenAttr< ::SCAML::SoftwareComponent> SoftwareComponent_children() const { return Udm::ChildrenAttr< ::SCAML::SoftwareComponent>(impl, meta_SoftwareComponent_children); }
			template <class Pred> Udm::ChildrenAttr< ::SCAML::SoftwareComponent, Pred> SoftwareComponent_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::SoftwareComponent, Pred>(impl, meta_SoftwareComponent_children); }

			Udm::ChildrenAttr< ::SCAML::componentplacement_Members_Base> componentplacement_Members_Base_kind_children() const { return Udm::ChildrenAttr< ::SCAML::componentplacement_Members_Base>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::componentplacement_Members_Base, Pred> componentplacement_Members_Base_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::componentplacement_Members_Base, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::SimulinkImplementation> SimulinkImplementation_kind_children() const { return Udm::ChildrenAttr< ::SCAML::SimulinkImplementation>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::SimulinkImplementation, Pred> SimulinkImplementation_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::SimulinkImplementation, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::SoftwareComponent> SoftwareComponent_kind_children() const { return Udm::ChildrenAttr< ::SCAML::SoftwareComponent>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::SoftwareComponent, Pred> SoftwareComponent_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::SoftwareComponent, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr< ::SCAML::MgaObject>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

			static ::Uml::CompositionParentRole meta_RootFolder_parent;
			Udm::ParentAttr< ::SCAML::RootFolder> RootFolder_parent() const { return Udm::ParentAttr< ::SCAML::RootFolder>(impl, meta_RootFolder_parent); }

			static ::Uml::CompositionParentRole meta_DomainProfile_parent;
			Udm::ParentAttr< ::SCAML::DomainProfile> DomainProfile_parent() const { return Udm::ParentAttr< ::SCAML::DomainProfile>(impl, meta_DomainProfile_parent); }

			Udm::ParentAttr<Udm::Object> parent() const { return Udm::ParentAttr<Udm::Object>(impl, Udm::NULLPARENTROLE); }
		};

		class  DevicePackageFolder :  public Udm::Object {
		public:
			static ::Uml::Class meta;

			DevicePackageFolder() { }
			DevicePackageFolder(Udm::ObjectImpl *impl) : UDM_OBJECT(impl) { }
			DevicePackageFolder(const DevicePackageFolder &master) : UDM_OBJECT(master) { }
			static DevicePackageFolder Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static DevicePackageFolder Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			DevicePackageFolder CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			DevicePackageFolder CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::SCAML::DevicePackageFolder> Instances() { return Udm::InstantiatedAttr< ::SCAML::DevicePackageFolder>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::SCAML::DevicePackageFolder, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::SCAML::DevicePackageFolder, Pred>(impl);}

			Udm::DerivedAttr< ::SCAML::DevicePackageFolder> Derived() { return Udm::DerivedAttr< ::SCAML::DevicePackageFolder>(impl);}
			template <class Pred> Udm::DerivedAttr< ::SCAML::DevicePackageFolder, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::SCAML::DevicePackageFolder, Pred>(impl);}

			Udm::ArchetypeAttr< ::SCAML::DevicePackageFolder> Archetype() const { return Udm::ArchetypeAttr< ::SCAML::DevicePackageFolder>(impl);}

			static ::Uml::Attribute meta_name;
			Udm::StringAttr name() const { return Udm::StringAttr(impl, meta_name); }

			static ::Uml::CompositionChildRole meta_DevicePkg_children;
			Udm::ChildrenAttr< ::SCAML::DevicePkg> DevicePkg_children() const { return Udm::ChildrenAttr< ::SCAML::DevicePkg>(impl, meta_DevicePkg_children); }
			template <class Pred> Udm::ChildrenAttr< ::SCAML::DevicePkg, Pred> DevicePkg_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::DevicePkg, Pred>(impl, meta_DevicePkg_children); }

			Udm::ChildrenAttr< ::SCAML::DevicePkg> DevicePkg_kind_children() const { return Udm::ChildrenAttr< ::SCAML::DevicePkg>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::DevicePkg, Pred> DevicePkg_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::DevicePkg, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr< ::SCAML::MgaObject>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

			static ::Uml::CompositionParentRole meta_RootFolder_parent;
			Udm::ParentAttr< ::SCAML::RootFolder> RootFolder_parent() const { return Udm::ParentAttr< ::SCAML::RootFolder>(impl, meta_RootFolder_parent); }

			static ::Uml::CompositionParentRole meta_DomainProfile_parent;
			Udm::ParentAttr< ::SCAML::DomainProfile> DomainProfile_parent() const { return Udm::ParentAttr< ::SCAML::DomainProfile>(impl, meta_DomainProfile_parent); }

			Udm::ParentAttr<Udm::Object> parent() const { return Udm::ParentAttr<Udm::Object>(impl, Udm::NULLPARENTROLE); }
		};

		class  DeviceConfigurationFolder :  public Udm::Object {
		public:
			static ::Uml::Class meta;

			DeviceConfigurationFolder() { }
			DeviceConfigurationFolder(Udm::ObjectImpl *impl) : UDM_OBJECT(impl) { }
			DeviceConfigurationFolder(const DeviceConfigurationFolder &master) : UDM_OBJECT(master) { }
			static DeviceConfigurationFolder Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static DeviceConfigurationFolder Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			DeviceConfigurationFolder CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			DeviceConfigurationFolder CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::SCAML::DeviceConfigurationFolder> Instances() { return Udm::InstantiatedAttr< ::SCAML::DeviceConfigurationFolder>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::SCAML::DeviceConfigurationFolder, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::SCAML::DeviceConfigurationFolder, Pred>(impl);}

			Udm::DerivedAttr< ::SCAML::DeviceConfigurationFolder> Derived() { return Udm::DerivedAttr< ::SCAML::DeviceConfigurationFolder>(impl);}
			template <class Pred> Udm::DerivedAttr< ::SCAML::DeviceConfigurationFolder, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::SCAML::DeviceConfigurationFolder, Pred>(impl);}

			Udm::ArchetypeAttr< ::SCAML::DeviceConfigurationFolder> Archetype() const { return Udm::ArchetypeAttr< ::SCAML::DeviceConfigurationFolder>(impl);}

			static ::Uml::Attribute meta_name;
			Udm::StringAttr name() const { return Udm::StringAttr(impl, meta_name); }

			static ::Uml::CompositionChildRole meta_DeviceConfiguration_children;
			Udm::ChildrenAttr< ::SCAML::DeviceConfiguration> DeviceConfiguration_children() const { return Udm::ChildrenAttr< ::SCAML::DeviceConfiguration>(impl, meta_DeviceConfiguration_children); }
			template <class Pred> Udm::ChildrenAttr< ::SCAML::DeviceConfiguration, Pred> DeviceConfiguration_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::DeviceConfiguration, Pred>(impl, meta_DeviceConfiguration_children); }

			Udm::ChildrenAttr< ::SCAML::DeviceConfiguration> DeviceConfiguration_kind_children() const { return Udm::ChildrenAttr< ::SCAML::DeviceConfiguration>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::DeviceConfiguration, Pred> DeviceConfiguration_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::DeviceConfiguration, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr< ::SCAML::MgaObject>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

			static ::Uml::CompositionParentRole meta_RootFolder_parent;
			Udm::ParentAttr< ::SCAML::RootFolder> RootFolder_parent() const { return Udm::ParentAttr< ::SCAML::RootFolder>(impl, meta_RootFolder_parent); }

			static ::Uml::CompositionParentRole meta_DomainProfile_parent;
			Udm::ParentAttr< ::SCAML::DomainProfile> DomainProfile_parent() const { return Udm::ParentAttr< ::SCAML::DomainProfile>(impl, meta_DomainProfile_parent); }

			Udm::ParentAttr<Udm::Object> parent() const { return Udm::ParentAttr<Udm::Object>(impl, Udm::NULLPARENTROLE); }
		};

		class  PropertiesFolder :  public Udm::Object {
		public:
			static ::Uml::Class meta;

			PropertiesFolder() { }
			PropertiesFolder(Udm::ObjectImpl *impl) : UDM_OBJECT(impl) { }
			PropertiesFolder(const PropertiesFolder &master) : UDM_OBJECT(master) { }
			static PropertiesFolder Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static PropertiesFolder Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			PropertiesFolder CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			PropertiesFolder CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::SCAML::PropertiesFolder> Instances() { return Udm::InstantiatedAttr< ::SCAML::PropertiesFolder>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::SCAML::PropertiesFolder, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::SCAML::PropertiesFolder, Pred>(impl);}

			Udm::DerivedAttr< ::SCAML::PropertiesFolder> Derived() { return Udm::DerivedAttr< ::SCAML::PropertiesFolder>(impl);}
			template <class Pred> Udm::DerivedAttr< ::SCAML::PropertiesFolder, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::SCAML::PropertiesFolder, Pred>(impl);}

			Udm::ArchetypeAttr< ::SCAML::PropertiesFolder> Archetype() const { return Udm::ArchetypeAttr< ::SCAML::PropertiesFolder>(impl);}

			static ::Uml::Attribute meta_name;
			Udm::StringAttr name() const { return Udm::StringAttr(impl, meta_name); }

			static ::Uml::CompositionChildRole meta_Properties_children;
			Udm::ChildrenAttr< ::SCAML::Properties> Properties_children() const { return Udm::ChildrenAttr< ::SCAML::Properties>(impl, meta_Properties_children); }
			template <class Pred> Udm::ChildrenAttr< ::SCAML::Properties, Pred> Properties_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::Properties, Pred>(impl, meta_Properties_children); }

			Udm::ChildrenAttr< ::SCAML::Properties> Properties_kind_children() const { return Udm::ChildrenAttr< ::SCAML::Properties>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::Properties, Pred> Properties_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::Properties, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr< ::SCAML::MgaObject>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

			static ::Uml::CompositionParentRole meta_RootFolder_parent;
			Udm::ParentAttr< ::SCAML::RootFolder> RootFolder_parent() const { return Udm::ParentAttr< ::SCAML::RootFolder>(impl, meta_RootFolder_parent); }

			static ::Uml::CompositionParentRole meta_DomainProfile_parent;
			Udm::ParentAttr< ::SCAML::DomainProfile> DomainProfile_parent() const { return Udm::ParentAttr< ::SCAML::DomainProfile>(impl, meta_DomainProfile_parent); }

			Udm::ParentAttr<Udm::Object> parent() const { return Udm::ParentAttr<Udm::Object>(impl, Udm::NULLPARENTROLE); }
		};

		class  DomainProfile :  public Udm::Object {
		public:
			static ::Uml::Class meta;

			DomainProfile() { }
			DomainProfile(Udm::ObjectImpl *impl) : UDM_OBJECT(impl) { }
			DomainProfile(const DomainProfile &master) : UDM_OBJECT(master) { }
			static DomainProfile Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static DomainProfile Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			DomainProfile CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			DomainProfile CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::SCAML::DomainProfile> Instances() { return Udm::InstantiatedAttr< ::SCAML::DomainProfile>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::SCAML::DomainProfile, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::SCAML::DomainProfile, Pred>(impl);}

			Udm::DerivedAttr< ::SCAML::DomainProfile> Derived() { return Udm::DerivedAttr< ::SCAML::DomainProfile>(impl);}
			template <class Pred> Udm::DerivedAttr< ::SCAML::DomainProfile, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::SCAML::DomainProfile, Pred>(impl);}

			Udm::ArchetypeAttr< ::SCAML::DomainProfile> Archetype() const { return Udm::ArchetypeAttr< ::SCAML::DomainProfile>(impl);}

			static ::Uml::Attribute meta_name;
			Udm::StringAttr name() const { return Udm::StringAttr(impl, meta_name); }

			static ::Uml::CompositionChildRole meta_SoftwareComponentFolder_children;
			Udm::ChildrenAttr< ::SCAML::SoftwareComponentFolder> SoftwareComponentFolder_children() const { return Udm::ChildrenAttr< ::SCAML::SoftwareComponentFolder>(impl, meta_SoftwareComponentFolder_children); }
			template <class Pred> Udm::ChildrenAttr< ::SCAML::SoftwareComponentFolder, Pred> SoftwareComponentFolder_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::SoftwareComponentFolder, Pred>(impl, meta_SoftwareComponentFolder_children); }

			static ::Uml::CompositionChildRole meta_SoftPkgFolder_children;
			Udm::ChildrenAttr< ::SCAML::SoftPkgFolder> SoftPkgFolder_children() const { return Udm::ChildrenAttr< ::SCAML::SoftPkgFolder>(impl, meta_SoftPkgFolder_children); }
			template <class Pred> Udm::ChildrenAttr< ::SCAML::SoftPkgFolder, Pred> SoftPkgFolder_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::SoftPkgFolder, Pred>(impl, meta_SoftPkgFolder_children); }

			static ::Uml::CompositionChildRole meta_DevicePackageFolder_children;
			Udm::ChildrenAttr< ::SCAML::DevicePackageFolder> DevicePackageFolder_children() const { return Udm::ChildrenAttr< ::SCAML::DevicePackageFolder>(impl, meta_DevicePackageFolder_children); }
			template <class Pred> Udm::ChildrenAttr< ::SCAML::DevicePackageFolder, Pred> DevicePackageFolder_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::DevicePackageFolder, Pred>(impl, meta_DevicePackageFolder_children); }

			static ::Uml::CompositionChildRole meta_DeviceConfigurationFolder_children;
			Udm::ChildrenAttr< ::SCAML::DeviceConfigurationFolder> DeviceConfigurationFolder_children() const { return Udm::ChildrenAttr< ::SCAML::DeviceConfigurationFolder>(impl, meta_DeviceConfigurationFolder_children); }
			template <class Pred> Udm::ChildrenAttr< ::SCAML::DeviceConfigurationFolder, Pred> DeviceConfigurationFolder_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::DeviceConfigurationFolder, Pred>(impl, meta_DeviceConfigurationFolder_children); }

			static ::Uml::CompositionChildRole meta_PropertiesFolder_children;
			Udm::ChildrenAttr< ::SCAML::PropertiesFolder> PropertiesFolder_children() const { return Udm::ChildrenAttr< ::SCAML::PropertiesFolder>(impl, meta_PropertiesFolder_children); }
			template <class Pred> Udm::ChildrenAttr< ::SCAML::PropertiesFolder, Pred> PropertiesFolder_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::PropertiesFolder, Pred>(impl, meta_PropertiesFolder_children); }

			static ::Uml::CompositionChildRole meta_SoftwareAssembly_children;
			Udm::ChildrenAttr< ::SCAML::SoftwareAssembly> SoftwareAssembly_children() const { return Udm::ChildrenAttr< ::SCAML::SoftwareAssembly>(impl, meta_SoftwareAssembly_children); }
			template <class Pred> Udm::ChildrenAttr< ::SCAML::SoftwareAssembly, Pred> SoftwareAssembly_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::SoftwareAssembly, Pred>(impl, meta_SoftwareAssembly_children); }

			static ::Uml::CompositionChildRole meta_DomainManagerConfiguration_children;
			Udm::ChildrenAttr< ::SCAML::DomainManagerConfiguration> DomainManagerConfiguration_children() const { return Udm::ChildrenAttr< ::SCAML::DomainManagerConfiguration>(impl, meta_DomainManagerConfiguration_children); }
			template <class Pred> Udm::ChildrenAttr< ::SCAML::DomainManagerConfiguration, Pred> DomainManagerConfiguration_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::DomainManagerConfiguration, Pred>(impl, meta_DomainManagerConfiguration_children); }

			Udm::ChildrenAttr< ::SCAML::SoftPkgFolder> SoftPkgFolder_kind_children() const { return Udm::ChildrenAttr< ::SCAML::SoftPkgFolder>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::SoftPkgFolder, Pred> SoftPkgFolder_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::SoftPkgFolder, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::SoftwareComponentFolder> SoftwareComponentFolder_kind_children() const { return Udm::ChildrenAttr< ::SCAML::SoftwareComponentFolder>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::SoftwareComponentFolder, Pred> SoftwareComponentFolder_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::SoftwareComponentFolder, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::DevicePackageFolder> DevicePackageFolder_kind_children() const { return Udm::ChildrenAttr< ::SCAML::DevicePackageFolder>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::DevicePackageFolder, Pred> DevicePackageFolder_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::DevicePackageFolder, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::DeviceConfigurationFolder> DeviceConfigurationFolder_kind_children() const { return Udm::ChildrenAttr< ::SCAML::DeviceConfigurationFolder>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::DeviceConfigurationFolder, Pred> DeviceConfigurationFolder_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::DeviceConfigurationFolder, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::PropertiesFolder> PropertiesFolder_kind_children() const { return Udm::ChildrenAttr< ::SCAML::PropertiesFolder>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::PropertiesFolder, Pred> PropertiesFolder_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::PropertiesFolder, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::DomainManagerConfiguration> DomainManagerConfiguration_kind_children() const { return Udm::ChildrenAttr< ::SCAML::DomainManagerConfiguration>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::DomainManagerConfiguration, Pred> DomainManagerConfiguration_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::DomainManagerConfiguration, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::SoftwareAssembly> SoftwareAssembly_kind_children() const { return Udm::ChildrenAttr< ::SCAML::SoftwareAssembly>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::SoftwareAssembly, Pred> SoftwareAssembly_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::SoftwareAssembly, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr< ::SCAML::MgaObject>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

			static ::Uml::CompositionParentRole meta_RootFolder_parent;
			Udm::ParentAttr< ::SCAML::RootFolder> RootFolder_parent() const { return Udm::ParentAttr< ::SCAML::RootFolder>(impl, meta_RootFolder_parent); }

			Udm::ParentAttr< ::SCAML::RootFolder> parent() const { return Udm::ParentAttr< ::SCAML::RootFolder>(impl, Udm::NULLPARENTROLE); }
			static ::Uml::Constraint meta_OnlyOneSAD;
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

			Udm::InstantiatedAttr< ::SCAML::RootFolder> Instances() { return Udm::InstantiatedAttr< ::SCAML::RootFolder>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::SCAML::RootFolder, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::SCAML::RootFolder, Pred>(impl);}

			Udm::DerivedAttr< ::SCAML::RootFolder> Derived() { return Udm::DerivedAttr< ::SCAML::RootFolder>(impl);}
			template <class Pred> Udm::DerivedAttr< ::SCAML::RootFolder, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::SCAML::RootFolder, Pred>(impl);}

			Udm::ArchetypeAttr< ::SCAML::RootFolder> Archetype() const { return Udm::ArchetypeAttr< ::SCAML::RootFolder>(impl);}

			static ::Uml::Attribute meta_name;
			Udm::StringAttr name() const { return Udm::StringAttr(impl, meta_name); }

			static ::Uml::CompositionChildRole meta_SoftPkgFolder_children;
			Udm::ChildrenAttr< ::SCAML::SoftPkgFolder> SoftPkgFolder_children() const { return Udm::ChildrenAttr< ::SCAML::SoftPkgFolder>(impl, meta_SoftPkgFolder_children); }
			template <class Pred> Udm::ChildrenAttr< ::SCAML::SoftPkgFolder, Pred> SoftPkgFolder_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::SoftPkgFolder, Pred>(impl, meta_SoftPkgFolder_children); }

			static ::Uml::CompositionChildRole meta_InterfaceFolder_children;
			Udm::ChildrenAttr< ::SCAML::InterfaceFolder> InterfaceFolder_children() const { return Udm::ChildrenAttr< ::SCAML::InterfaceFolder>(impl, meta_InterfaceFolder_children); }
			template <class Pred> Udm::ChildrenAttr< ::SCAML::InterfaceFolder, Pred> InterfaceFolder_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::InterfaceFolder, Pred>(impl, meta_InterfaceFolder_children); }

			static ::Uml::CompositionChildRole meta_SoftwareComponentFolder_children;
			Udm::ChildrenAttr< ::SCAML::SoftwareComponentFolder> SoftwareComponentFolder_children() const { return Udm::ChildrenAttr< ::SCAML::SoftwareComponentFolder>(impl, meta_SoftwareComponentFolder_children); }
			template <class Pred> Udm::ChildrenAttr< ::SCAML::SoftwareComponentFolder, Pred> SoftwareComponentFolder_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::SoftwareComponentFolder, Pred>(impl, meta_SoftwareComponentFolder_children); }

			static ::Uml::CompositionChildRole meta_DevicePackageFolder_children;
			Udm::ChildrenAttr< ::SCAML::DevicePackageFolder> DevicePackageFolder_children() const { return Udm::ChildrenAttr< ::SCAML::DevicePackageFolder>(impl, meta_DevicePackageFolder_children); }
			template <class Pred> Udm::ChildrenAttr< ::SCAML::DevicePackageFolder, Pred> DevicePackageFolder_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::DevicePackageFolder, Pred>(impl, meta_DevicePackageFolder_children); }

			static ::Uml::CompositionChildRole meta_DeviceConfigurationFolder_children;
			Udm::ChildrenAttr< ::SCAML::DeviceConfigurationFolder> DeviceConfigurationFolder_children() const { return Udm::ChildrenAttr< ::SCAML::DeviceConfigurationFolder>(impl, meta_DeviceConfigurationFolder_children); }
			template <class Pred> Udm::ChildrenAttr< ::SCAML::DeviceConfigurationFolder, Pred> DeviceConfigurationFolder_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::DeviceConfigurationFolder, Pred>(impl, meta_DeviceConfigurationFolder_children); }

			static ::Uml::CompositionChildRole meta_PropertiesFolder_children;
			Udm::ChildrenAttr< ::SCAML::PropertiesFolder> PropertiesFolder_children() const { return Udm::ChildrenAttr< ::SCAML::PropertiesFolder>(impl, meta_PropertiesFolder_children); }
			template <class Pred> Udm::ChildrenAttr< ::SCAML::PropertiesFolder, Pred> PropertiesFolder_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::PropertiesFolder, Pred>(impl, meta_PropertiesFolder_children); }

			static ::Uml::CompositionChildRole meta_DomainProfile_children;
			Udm::ChildrenAttr< ::SCAML::DomainProfile> DomainProfile_children() const { return Udm::ChildrenAttr< ::SCAML::DomainProfile>(impl, meta_DomainProfile_children); }
			template <class Pred> Udm::ChildrenAttr< ::SCAML::DomainProfile, Pred> DomainProfile_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::DomainProfile, Pred>(impl, meta_DomainProfile_children); }

			static ::Uml::CompositionChildRole meta_RootFolder_children;
			Udm::ChildrenAttr< ::SCAML::RootFolder> RootFolder_children() const { return Udm::ChildrenAttr< ::SCAML::RootFolder>(impl, meta_RootFolder_children); }
			template <class Pred> Udm::ChildrenAttr< ::SCAML::RootFolder, Pred> RootFolder_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::RootFolder, Pred>(impl, meta_RootFolder_children); }

			Udm::ChildrenAttr< ::SCAML::InterfaceFolder> InterfaceFolder_kind_children() const { return Udm::ChildrenAttr< ::SCAML::InterfaceFolder>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::InterfaceFolder, Pred> InterfaceFolder_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::InterfaceFolder, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::SoftPkgFolder> SoftPkgFolder_kind_children() const { return Udm::ChildrenAttr< ::SCAML::SoftPkgFolder>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::SoftPkgFolder, Pred> SoftPkgFolder_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::SoftPkgFolder, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::SoftwareComponentFolder> SoftwareComponentFolder_kind_children() const { return Udm::ChildrenAttr< ::SCAML::SoftwareComponentFolder>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::SoftwareComponentFolder, Pred> SoftwareComponentFolder_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::SoftwareComponentFolder, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::DevicePackageFolder> DevicePackageFolder_kind_children() const { return Udm::ChildrenAttr< ::SCAML::DevicePackageFolder>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::DevicePackageFolder, Pred> DevicePackageFolder_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::DevicePackageFolder, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::DeviceConfigurationFolder> DeviceConfigurationFolder_kind_children() const { return Udm::ChildrenAttr< ::SCAML::DeviceConfigurationFolder>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::DeviceConfigurationFolder, Pred> DeviceConfigurationFolder_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::DeviceConfigurationFolder, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::PropertiesFolder> PropertiesFolder_kind_children() const { return Udm::ChildrenAttr< ::SCAML::PropertiesFolder>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::PropertiesFolder, Pred> PropertiesFolder_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::PropertiesFolder, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::DomainProfile> DomainProfile_kind_children() const { return Udm::ChildrenAttr< ::SCAML::DomainProfile>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::DomainProfile, Pred> DomainProfile_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::DomainProfile, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::RootFolder> RootFolder_kind_children() const { return Udm::ChildrenAttr< ::SCAML::RootFolder>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::RootFolder, Pred> RootFolder_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::RootFolder, Pred>(impl, Udm::NULLCHILDROLE); }

			static ::Uml::CompositionParentRole meta_RootFolder_parent;
			Udm::ParentAttr< ::SCAML::RootFolder> RootFolder_parent() const { return Udm::ParentAttr< ::SCAML::RootFolder>(impl, meta_RootFolder_parent); }

			Udm::ParentAttr< ::SCAML::RootFolder> parent() const { return Udm::ParentAttr< ::SCAML::RootFolder>(impl, Udm::NULLPARENTROLE); }
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

			Udm::InstantiatedAttr< ::SCAML::MgaObject> Instances() { return Udm::InstantiatedAttr< ::SCAML::MgaObject>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::SCAML::MgaObject, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::SCAML::MgaObject, Pred>(impl);}

			Udm::DerivedAttr< ::SCAML::MgaObject> Derived() { return Udm::DerivedAttr< ::SCAML::MgaObject>(impl);}
			template <class Pred> Udm::DerivedAttr< ::SCAML::MgaObject, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::SCAML::MgaObject, Pred>(impl);}

			Udm::ArchetypeAttr< ::SCAML::MgaObject> Archetype() const { return Udm::ArchetypeAttr< ::SCAML::MgaObject>(impl);}

			static ::Uml::Attribute meta_position;
			Udm::StringAttr position() const { return Udm::StringAttr(impl, meta_position); }

			static ::Uml::Attribute meta_name;
			Udm::StringAttr name() const { return Udm::StringAttr(impl, meta_name); }

			Udm::ParentAttr<Udm::Object> parent() const { return Udm::ParentAttr<Udm::Object>(impl, Udm::NULLPARENTROLE); }
		};

		class  connectdestBase :  virtual public ::SCAML::MgaObject {
		public:
			static ::Uml::Class meta;

			connectdestBase() { }
			connectdestBase(Udm::ObjectImpl *impl) : ::SCAML::MgaObject(impl), UDM_OBJECT(impl) { }
			connectdestBase(const connectdestBase &master) : ::SCAML::MgaObject(master), UDM_OBJECT(master) { }
			static connectdestBase Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static connectdestBase Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			connectdestBase CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			connectdestBase CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::SCAML::connectdestBase> Instances() { return Udm::InstantiatedAttr< ::SCAML::connectdestBase>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::SCAML::connectdestBase, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::SCAML::connectdestBase, Pred>(impl);}

			Udm::DerivedAttr< ::SCAML::connectdestBase> Derived() { return Udm::DerivedAttr< ::SCAML::connectdestBase>(impl);}
			template <class Pred> Udm::DerivedAttr< ::SCAML::connectdestBase, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::SCAML::connectdestBase, Pred>(impl);}

			Udm::ArchetypeAttr< ::SCAML::connectdestBase> Archetype() const { return Udm::ArchetypeAttr< ::SCAML::connectdestBase>(impl);}

			static ::Uml::AssociationRole meta_srcusestodestConn;
			static ::Uml::AssociationRole meta_srcusestodestConn_rev;
			Udm::AClassAssocAttr< ::SCAML::usestodestConn, ::SCAML::usesPort> srcusestodestConn() const { return Udm::AClassAssocAttr< ::SCAML::usestodestConn, ::SCAML::usesPort>(impl, meta_srcusestodestConn, meta_srcusestodestConn_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::SCAML::usestodestConn, ::SCAML::usesPort, Pred> srcusestodestConn_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::SCAML::usestodestConn, ::SCAML::usesPort, Pred>(impl, meta_srcusestodestConn, meta_srcusestodestConn_rev); }

			Udm::ParentAttr<Udm::Object> parent() const { return Udm::ParentAttr<Udm::Object>(impl, Udm::NULLPARENTROLE); }
		};

		class  usestodestConn :  public ::SCAML::MgaObject {
		public:
			static ::Uml::Class meta;

			usestodestConn() { }
			usestodestConn(Udm::ObjectImpl *impl) : ::SCAML::MgaObject(impl), UDM_OBJECT(impl) { }
			usestodestConn(const usestodestConn &master) : ::SCAML::MgaObject(master), UDM_OBJECT(master) { }
			static usestodestConn Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static usestodestConn Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			usestodestConn CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			usestodestConn CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::SCAML::usestodestConn> Instances() { return Udm::InstantiatedAttr< ::SCAML::usestodestConn>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::SCAML::usestodestConn, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::SCAML::usestodestConn, Pred>(impl);}

			Udm::DerivedAttr< ::SCAML::usestodestConn> Derived() { return Udm::DerivedAttr< ::SCAML::usestodestConn>(impl);}
			template <class Pred> Udm::DerivedAttr< ::SCAML::usestodestConn, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::SCAML::usestodestConn, Pred>(impl);}

			Udm::ArchetypeAttr< ::SCAML::usestodestConn> Archetype() const { return Udm::ArchetypeAttr< ::SCAML::usestodestConn>(impl);}

			static ::Uml::CompositionParentRole meta_DeviceConfiguration_parent;
			Udm::ParentAttr< ::SCAML::DeviceConfiguration> DeviceConfiguration_parent() const { return Udm::ParentAttr< ::SCAML::DeviceConfiguration>(impl, meta_DeviceConfiguration_parent); }

			static ::Uml::CompositionParentRole meta_SoftwareAssembly_parent;
			Udm::ParentAttr< ::SCAML::SoftwareAssembly> SoftwareAssembly_parent() const { return Udm::ParentAttr< ::SCAML::SoftwareAssembly>(impl, meta_SoftwareAssembly_parent); }

			Udm::ParentAttr< ::SCAML::MgaObject> parent() const { return Udm::ParentAttr< ::SCAML::MgaObject>(impl, Udm::NULLPARENTROLE); }
			static ::Uml::AssociationRole meta_dstusestodestConn_end_;
			Udm::AssocEndAttr< ::SCAML::connectdestBase> dstusestodestConn_end() const { return Udm::AssocEndAttr< ::SCAML::connectdestBase>(impl, meta_dstusestodestConn_end_); }

			static ::Uml::AssociationRole meta_srcusestodestConn_end_;
			Udm::AssocEndAttr< ::SCAML::usesPort> srcusestodestConn_end() const { return Udm::AssocEndAttr< ::SCAML::usesPort>(impl, meta_srcusestodestConn_end_); }

		};

		class  externaltodestConn :  public ::SCAML::MgaObject {
		public:
			static ::Uml::Class meta;

			externaltodestConn() { }
			externaltodestConn(Udm::ObjectImpl *impl) : ::SCAML::MgaObject(impl), UDM_OBJECT(impl) { }
			externaltodestConn(const externaltodestConn &master) : ::SCAML::MgaObject(master), UDM_OBJECT(master) { }
			static externaltodestConn Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static externaltodestConn Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			externaltodestConn CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			externaltodestConn CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::SCAML::externaltodestConn> Instances() { return Udm::InstantiatedAttr< ::SCAML::externaltodestConn>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::SCAML::externaltodestConn, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::SCAML::externaltodestConn, Pred>(impl);}

			Udm::DerivedAttr< ::SCAML::externaltodestConn> Derived() { return Udm::DerivedAttr< ::SCAML::externaltodestConn>(impl);}
			template <class Pred> Udm::DerivedAttr< ::SCAML::externaltodestConn, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::SCAML::externaltodestConn, Pred>(impl);}

			Udm::ArchetypeAttr< ::SCAML::externaltodestConn> Archetype() const { return Udm::ArchetypeAttr< ::SCAML::externaltodestConn>(impl);}

			static ::Uml::CompositionParentRole meta_SoftwareAssembly_parent;
			Udm::ParentAttr< ::SCAML::SoftwareAssembly> SoftwareAssembly_parent() const { return Udm::ParentAttr< ::SCAML::SoftwareAssembly>(impl, meta_SoftwareAssembly_parent); }

			Udm::ParentAttr< ::SCAML::MgaObject> parent() const { return Udm::ParentAttr< ::SCAML::MgaObject>(impl, Udm::NULLPARENTROLE); }
			static ::Uml::AssociationRole meta_dstexternaltodestConn_end_;
			Udm::AssocEndAttr< ::SCAML::externaltodestBase> dstexternaltodestConn_end() const { return Udm::AssocEndAttr< ::SCAML::externaltodestBase>(impl, meta_dstexternaltodestConn_end_); }

			static ::Uml::AssociationRole meta_srcexternaltodestConn_end_;
			Udm::AssocEndAttr< ::SCAML::externalport> srcexternaltodestConn_end() const { return Udm::AssocEndAttr< ::SCAML::externalport>(impl, meta_srcexternaltodestConn_end_); }

		};

		class  externalport :  public ::SCAML::MgaObject {
		public:
			static ::Uml::Class meta;

			externalport() { }
			externalport(Udm::ObjectImpl *impl) : ::SCAML::MgaObject(impl), UDM_OBJECT(impl) { }
			externalport(const externalport &master) : ::SCAML::MgaObject(master), UDM_OBJECT(master) { }
			static externalport Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static externalport Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			externalport CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			externalport CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::SCAML::externalport> Instances() { return Udm::InstantiatedAttr< ::SCAML::externalport>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::SCAML::externalport, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::SCAML::externalport, Pred>(impl);}

			Udm::DerivedAttr< ::SCAML::externalport> Derived() { return Udm::DerivedAttr< ::SCAML::externalport>(impl);}
			template <class Pred> Udm::DerivedAttr< ::SCAML::externalport, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::SCAML::externalport, Pred>(impl);}

			Udm::ArchetypeAttr< ::SCAML::externalport> Archetype() const { return Udm::ArchetypeAttr< ::SCAML::externalport>(impl);}

			static ::Uml::AssociationRole meta_dstexternaltodestConn;
			static ::Uml::AssociationRole meta_dstexternaltodestConn_rev;
			Udm::AClassAssocAttr< ::SCAML::externaltodestConn, ::SCAML::externaltodestBase> dstexternaltodestConn() const { return Udm::AClassAssocAttr< ::SCAML::externaltodestConn, ::SCAML::externaltodestBase>(impl, meta_dstexternaltodestConn, meta_dstexternaltodestConn_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::SCAML::externaltodestConn, ::SCAML::externaltodestBase, Pred> dstexternaltodestConn_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::SCAML::externaltodestConn, ::SCAML::externaltodestBase, Pred>(impl, meta_dstexternaltodestConn, meta_dstexternaltodestConn_rev); }

			static ::Uml::CompositionParentRole meta_SoftwareAssembly_parent;
			Udm::ParentAttr< ::SCAML::SoftwareAssembly> SoftwareAssembly_parent() const { return Udm::ParentAttr< ::SCAML::SoftwareAssembly>(impl, meta_SoftwareAssembly_parent); }

			Udm::ParentAttr< ::SCAML::MgaObject> parent() const { return Udm::ParentAttr< ::SCAML::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  externaltodestBase :  virtual public ::SCAML::MgaObject {
		public:
			static ::Uml::Class meta;

			externaltodestBase() { }
			externaltodestBase(Udm::ObjectImpl *impl) : ::SCAML::MgaObject(impl), UDM_OBJECT(impl) { }
			externaltodestBase(const externaltodestBase &master) : ::SCAML::MgaObject(master), UDM_OBJECT(master) { }
			static externaltodestBase Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static externaltodestBase Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			externaltodestBase CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			externaltodestBase CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::SCAML::externaltodestBase> Instances() { return Udm::InstantiatedAttr< ::SCAML::externaltodestBase>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::SCAML::externaltodestBase, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::SCAML::externaltodestBase, Pred>(impl);}

			Udm::DerivedAttr< ::SCAML::externaltodestBase> Derived() { return Udm::DerivedAttr< ::SCAML::externaltodestBase>(impl);}
			template <class Pred> Udm::DerivedAttr< ::SCAML::externaltodestBase, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::SCAML::externaltodestBase, Pred>(impl);}

			Udm::ArchetypeAttr< ::SCAML::externaltodestBase> Archetype() const { return Udm::ArchetypeAttr< ::SCAML::externaltodestBase>(impl);}

			static ::Uml::AssociationRole meta_srcexternaltodestConn;
			static ::Uml::AssociationRole meta_srcexternaltodestConn_rev;
			Udm::AClassAssocAttr< ::SCAML::externaltodestConn, ::SCAML::externalport> srcexternaltodestConn() const { return Udm::AClassAssocAttr< ::SCAML::externaltodestConn, ::SCAML::externalport>(impl, meta_srcexternaltodestConn, meta_srcexternaltodestConn_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::SCAML::externaltodestConn, ::SCAML::externalport, Pred> srcexternaltodestConn_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::SCAML::externaltodestConn, ::SCAML::externalport, Pred>(impl, meta_srcexternaltodestConn, meta_srcexternaltodestConn_rev); }

			Udm::ParentAttr<Udm::Object> parent() const { return Udm::ParentAttr<Udm::Object>(impl, Udm::NULLPARENTROLE); }
		};

		class  componentsupportedinterface :  public ::SCAML::connectdestBase, public ::SCAML::externaltodestBase {
		public:
			static ::Uml::Class meta;

			componentsupportedinterface() { }
			componentsupportedinterface(Udm::ObjectImpl *impl) : ::SCAML::connectdestBase(impl),::SCAML::externaltodestBase(impl), ::SCAML::MgaObject(impl), UDM_OBJECT(impl) { }
			componentsupportedinterface(const componentsupportedinterface &master) : ::SCAML::connectdestBase(master),::SCAML::externaltodestBase(master), ::SCAML::MgaObject(master), UDM_OBJECT(master) { }
			static componentsupportedinterface Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static componentsupportedinterface Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			componentsupportedinterface CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			componentsupportedinterface CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::SCAML::componentsupportedinterface> Instances() { return Udm::InstantiatedAttr< ::SCAML::componentsupportedinterface>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::SCAML::componentsupportedinterface, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::SCAML::componentsupportedinterface, Pred>(impl);}

			Udm::DerivedAttr< ::SCAML::componentsupportedinterface> Derived() { return Udm::DerivedAttr< ::SCAML::componentsupportedinterface>(impl);}
			template <class Pred> Udm::DerivedAttr< ::SCAML::componentsupportedinterface, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::SCAML::componentsupportedinterface, Pred>(impl);}

			Udm::ArchetypeAttr< ::SCAML::componentsupportedinterface> Archetype() const { return Udm::ArchetypeAttr< ::SCAML::componentsupportedinterface>(impl);}

			static ::Uml::AssociationRole meta_ref;
			Udm::PointerAttr< ::SCAML::supports> ref() const { return Udm::PointerAttr< ::SCAML::supports>(impl, meta_ref); }

			static ::Uml::CompositionParentRole meta_SoftwareAssembly_parent;
			Udm::ParentAttr< ::SCAML::SoftwareAssembly> SoftwareAssembly_parent() const { return Udm::ParentAttr< ::SCAML::SoftwareAssembly>(impl, meta_SoftwareAssembly_parent); }

			Udm::ParentAttr< ::SCAML::MgaObject> parent() const { return Udm::ParentAttr< ::SCAML::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  freestandingtoprovidedConn :  public ::SCAML::MgaObject {
		public:
			static ::Uml::Class meta;

			freestandingtoprovidedConn() { }
			freestandingtoprovidedConn(Udm::ObjectImpl *impl) : ::SCAML::MgaObject(impl), UDM_OBJECT(impl) { }
			freestandingtoprovidedConn(const freestandingtoprovidedConn &master) : ::SCAML::MgaObject(master), UDM_OBJECT(master) { }
			static freestandingtoprovidedConn Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static freestandingtoprovidedConn Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			freestandingtoprovidedConn CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			freestandingtoprovidedConn CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::SCAML::freestandingtoprovidedConn> Instances() { return Udm::InstantiatedAttr< ::SCAML::freestandingtoprovidedConn>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::SCAML::freestandingtoprovidedConn, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::SCAML::freestandingtoprovidedConn, Pred>(impl);}

			Udm::DerivedAttr< ::SCAML::freestandingtoprovidedConn> Derived() { return Udm::DerivedAttr< ::SCAML::freestandingtoprovidedConn>(impl);}
			template <class Pred> Udm::DerivedAttr< ::SCAML::freestandingtoprovidedConn, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::SCAML::freestandingtoprovidedConn, Pred>(impl);}

			Udm::ArchetypeAttr< ::SCAML::freestandingtoprovidedConn> Archetype() const { return Udm::ArchetypeAttr< ::SCAML::freestandingtoprovidedConn>(impl);}

			static ::Uml::CompositionParentRole meta_DeviceConfiguration_parent;
			Udm::ParentAttr< ::SCAML::DeviceConfiguration> DeviceConfiguration_parent() const { return Udm::ParentAttr< ::SCAML::DeviceConfiguration>(impl, meta_DeviceConfiguration_parent); }

			static ::Uml::CompositionParentRole meta_SoftwareAssembly_parent;
			Udm::ParentAttr< ::SCAML::SoftwareAssembly> SoftwareAssembly_parent() const { return Udm::ParentAttr< ::SCAML::SoftwareAssembly>(impl, meta_SoftwareAssembly_parent); }

			Udm::ParentAttr< ::SCAML::MgaObject> parent() const { return Udm::ParentAttr< ::SCAML::MgaObject>(impl, Udm::NULLPARENTROLE); }
			static ::Uml::AssociationRole meta_srcfreestandingtoprovidedConn_end_;
			Udm::AssocEndAttr< ::SCAML::FreestandingPort> srcfreestandingtoprovidedConn_end() const { return Udm::AssocEndAttr< ::SCAML::FreestandingPort>(impl, meta_srcfreestandingtoprovidedConn_end_); }

			static ::Uml::AssociationRole meta_dstfreestandingtoprovidedConn_end_;
			Udm::AssocEndAttr< ::SCAML::providesPort> dstfreestandingtoprovidedConn_end() const { return Udm::AssocEndAttr< ::SCAML::providesPort>(impl, meta_dstfreestandingtoprovidedConn_end_); }

		};

		class  usestofreestandingConn :  public ::SCAML::MgaObject {
		public:
			static ::Uml::Class meta;

			usestofreestandingConn() { }
			usestofreestandingConn(Udm::ObjectImpl *impl) : ::SCAML::MgaObject(impl), UDM_OBJECT(impl) { }
			usestofreestandingConn(const usestofreestandingConn &master) : ::SCAML::MgaObject(master), UDM_OBJECT(master) { }
			static usestofreestandingConn Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static usestofreestandingConn Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			usestofreestandingConn CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			usestofreestandingConn CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::SCAML::usestofreestandingConn> Instances() { return Udm::InstantiatedAttr< ::SCAML::usestofreestandingConn>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::SCAML::usestofreestandingConn, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::SCAML::usestofreestandingConn, Pred>(impl);}

			Udm::DerivedAttr< ::SCAML::usestofreestandingConn> Derived() { return Udm::DerivedAttr< ::SCAML::usestofreestandingConn>(impl);}
			template <class Pred> Udm::DerivedAttr< ::SCAML::usestofreestandingConn, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::SCAML::usestofreestandingConn, Pred>(impl);}

			Udm::ArchetypeAttr< ::SCAML::usestofreestandingConn> Archetype() const { return Udm::ArchetypeAttr< ::SCAML::usestofreestandingConn>(impl);}

			static ::Uml::CompositionParentRole meta_DeviceConfiguration_parent;
			Udm::ParentAttr< ::SCAML::DeviceConfiguration> DeviceConfiguration_parent() const { return Udm::ParentAttr< ::SCAML::DeviceConfiguration>(impl, meta_DeviceConfiguration_parent); }

			static ::Uml::CompositionParentRole meta_SoftwareAssembly_parent;
			Udm::ParentAttr< ::SCAML::SoftwareAssembly> SoftwareAssembly_parent() const { return Udm::ParentAttr< ::SCAML::SoftwareAssembly>(impl, meta_SoftwareAssembly_parent); }

			Udm::ParentAttr< ::SCAML::MgaObject> parent() const { return Udm::ParentAttr< ::SCAML::MgaObject>(impl, Udm::NULLPARENTROLE); }
			static ::Uml::AssociationRole meta_dstusestofreestandingConn_end_;
			Udm::AssocEndAttr< ::SCAML::FreestandingPort> dstusestofreestandingConn_end() const { return Udm::AssocEndAttr< ::SCAML::FreestandingPort>(impl, meta_dstusestofreestandingConn_end_); }

			static ::Uml::AssociationRole meta_srcusestofreestandingConn_end_;
			Udm::AssocEndAttr< ::SCAML::usesPort> srcusestofreestandingConn_end() const { return Udm::AssocEndAttr< ::SCAML::usesPort>(impl, meta_srcusestofreestandingConn_end_); }

		};

		class  FreestandingPort :  public ::SCAML::MgaObject {
		public:
			static ::Uml::Class meta;

			FreestandingPort() { }
			FreestandingPort(Udm::ObjectImpl *impl) : ::SCAML::MgaObject(impl), UDM_OBJECT(impl) { }
			FreestandingPort(const FreestandingPort &master) : ::SCAML::MgaObject(master), UDM_OBJECT(master) { }
			static FreestandingPort Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static FreestandingPort Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			FreestandingPort CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			FreestandingPort CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::SCAML::FreestandingPort> Instances() { return Udm::InstantiatedAttr< ::SCAML::FreestandingPort>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::SCAML::FreestandingPort, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::SCAML::FreestandingPort, Pred>(impl);}

			Udm::DerivedAttr< ::SCAML::FreestandingPort> Derived() { return Udm::DerivedAttr< ::SCAML::FreestandingPort>(impl);}
			template <class Pred> Udm::DerivedAttr< ::SCAML::FreestandingPort, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::SCAML::FreestandingPort, Pred>(impl);}

			Udm::ArchetypeAttr< ::SCAML::FreestandingPort> Archetype() const { return Udm::ArchetypeAttr< ::SCAML::FreestandingPort>(impl);}

			static ::Uml::AssociationRole meta_srcusestofreestandingConn;
			static ::Uml::AssociationRole meta_srcusestofreestandingConn_rev;
			Udm::AClassAssocAttr< ::SCAML::usestofreestandingConn, ::SCAML::usesPort> srcusestofreestandingConn() const { return Udm::AClassAssocAttr< ::SCAML::usestofreestandingConn, ::SCAML::usesPort>(impl, meta_srcusestofreestandingConn, meta_srcusestofreestandingConn_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::SCAML::usestofreestandingConn, ::SCAML::usesPort, Pred> srcusestofreestandingConn_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::SCAML::usestofreestandingConn, ::SCAML::usesPort, Pred>(impl, meta_srcusestofreestandingConn, meta_srcusestofreestandingConn_rev); }

			static ::Uml::AssociationRole meta_dstfreestandingtoprovidedConn;
			static ::Uml::AssociationRole meta_dstfreestandingtoprovidedConn_rev;
			Udm::AClassAssocAttr< ::SCAML::freestandingtoprovidedConn, ::SCAML::providesPort> dstfreestandingtoprovidedConn() const { return Udm::AClassAssocAttr< ::SCAML::freestandingtoprovidedConn, ::SCAML::providesPort>(impl, meta_dstfreestandingtoprovidedConn, meta_dstfreestandingtoprovidedConn_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::SCAML::freestandingtoprovidedConn, ::SCAML::providesPort, Pred> dstfreestandingtoprovidedConn_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::SCAML::freestandingtoprovidedConn, ::SCAML::providesPort, Pred>(impl, meta_dstfreestandingtoprovidedConn, meta_dstfreestandingtoprovidedConn_rev); }

			static ::Uml::CompositionParentRole meta_SoftwareAssembly_parent;
			Udm::ParentAttr< ::SCAML::SoftwareAssembly> SoftwareAssembly_parent() const { return Udm::ParentAttr< ::SCAML::SoftwareAssembly>(impl, meta_SoftwareAssembly_parent); }

			Udm::ParentAttr< ::SCAML::MgaObject> parent() const { return Udm::ParentAttr< ::SCAML::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  thisPort :  public ::SCAML::FreestandingPort {
		public:
			static ::Uml::Class meta;

			thisPort() { }
			thisPort(Udm::ObjectImpl *impl) : ::SCAML::FreestandingPort(impl), UDM_OBJECT(impl) { }
			thisPort(const thisPort &master) : ::SCAML::FreestandingPort(master), UDM_OBJECT(master) { }
			static thisPort Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static thisPort Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			thisPort CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			thisPort CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::SCAML::thisPort> Instances() { return Udm::InstantiatedAttr< ::SCAML::thisPort>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::SCAML::thisPort, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::SCAML::thisPort, Pred>(impl);}

			Udm::DerivedAttr< ::SCAML::thisPort> Derived() { return Udm::DerivedAttr< ::SCAML::thisPort>(impl);}
			template <class Pred> Udm::DerivedAttr< ::SCAML::thisPort, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::SCAML::thisPort, Pred>(impl);}

			Udm::ArchetypeAttr< ::SCAML::thisPort> Archetype() const { return Udm::ArchetypeAttr< ::SCAML::thisPort>(impl);}

			static ::Uml::CompositionParentRole meta_DeviceConfiguration_parent;
			Udm::ParentAttr< ::SCAML::DeviceConfiguration> DeviceConfiguration_parent() const { return Udm::ParentAttr< ::SCAML::DeviceConfiguration>(impl, meta_DeviceConfiguration_parent); }

			Udm::ParentAttr< ::SCAML::MgaObject> parent() const { return Udm::ParentAttr< ::SCAML::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  deviceusedbythiscomponentref :  public ::SCAML::FreestandingPort {
		public:
			static ::Uml::Class meta;

			deviceusedbythiscomponentref() { }
			deviceusedbythiscomponentref(Udm::ObjectImpl *impl) : ::SCAML::FreestandingPort(impl), UDM_OBJECT(impl) { }
			deviceusedbythiscomponentref(const deviceusedbythiscomponentref &master) : ::SCAML::FreestandingPort(master), UDM_OBJECT(master) { }
			static deviceusedbythiscomponentref Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static deviceusedbythiscomponentref Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			deviceusedbythiscomponentref CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			deviceusedbythiscomponentref CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::SCAML::deviceusedbythiscomponentref> Instances() { return Udm::InstantiatedAttr< ::SCAML::deviceusedbythiscomponentref>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::SCAML::deviceusedbythiscomponentref, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::SCAML::deviceusedbythiscomponentref, Pred>(impl);}

			Udm::DerivedAttr< ::SCAML::deviceusedbythiscomponentref> Derived() { return Udm::DerivedAttr< ::SCAML::deviceusedbythiscomponentref>(impl);}
			template <class Pred> Udm::DerivedAttr< ::SCAML::deviceusedbythiscomponentref, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::SCAML::deviceusedbythiscomponentref, Pred>(impl);}

			Udm::ArchetypeAttr< ::SCAML::deviceusedbythiscomponentref> Archetype() const { return Udm::ArchetypeAttr< ::SCAML::deviceusedbythiscomponentref>(impl);}

			static ::Uml::Attribute meta_usesrefid;
			Udm::StringAttr usesrefid() const { return Udm::StringAttr(impl, meta_usesrefid); }

			Udm::ParentAttr< ::SCAML::MgaObject> parent() const { return Udm::ParentAttr< ::SCAML::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  service :  public ::SCAML::FreestandingPort {
		public:
			static ::Uml::Class meta;

			service() { }
			service(Udm::ObjectImpl *impl) : ::SCAML::FreestandingPort(impl), UDM_OBJECT(impl) { }
			service(const service &master) : ::SCAML::FreestandingPort(master), UDM_OBJECT(master) { }
			static service Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static service Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			service CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			service CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::SCAML::service> Instances() { return Udm::InstantiatedAttr< ::SCAML::service>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::SCAML::service, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::SCAML::service, Pred>(impl);}

			Udm::DerivedAttr< ::SCAML::service> Derived() { return Udm::DerivedAttr< ::SCAML::service>(impl);}
			template <class Pred> Udm::DerivedAttr< ::SCAML::service, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::SCAML::service, Pred>(impl);}

			Udm::ArchetypeAttr< ::SCAML::service> Archetype() const { return Udm::ArchetypeAttr< ::SCAML::service>(impl);}

			static ::Uml::CompositionParentRole meta_DeviceConfiguration_parent;
			Udm::ParentAttr< ::SCAML::DeviceConfiguration> DeviceConfiguration_parent() const { return Udm::ParentAttr< ::SCAML::DeviceConfiguration>(impl, meta_DeviceConfiguration_parent); }

			Udm::ParentAttr< ::SCAML::MgaObject> parent() const { return Udm::ParentAttr< ::SCAML::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  log :  public ::SCAML::service {
		public:
			static ::Uml::Class meta;

			log() { }
			log(Udm::ObjectImpl *impl) : ::SCAML::service(impl), UDM_OBJECT(impl) { }
			log(const log &master) : ::SCAML::service(master), UDM_OBJECT(master) { }
			static log Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static log Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			log CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			log CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::SCAML::log> Instances() { return Udm::InstantiatedAttr< ::SCAML::log>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::SCAML::log, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::SCAML::log, Pred>(impl);}

			Udm::DerivedAttr< ::SCAML::log> Derived() { return Udm::DerivedAttr< ::SCAML::log>(impl);}
			template <class Pred> Udm::DerivedAttr< ::SCAML::log, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::SCAML::log, Pred>(impl);}

			Udm::ArchetypeAttr< ::SCAML::log> Archetype() const { return Udm::ArchetypeAttr< ::SCAML::log>(impl);}

			Udm::ParentAttr< ::SCAML::MgaObject> parent() const { return Udm::ParentAttr< ::SCAML::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  filemanager :  public ::SCAML::service {
		public:
			static ::Uml::Class meta;

			filemanager() { }
			filemanager(Udm::ObjectImpl *impl) : ::SCAML::service(impl), UDM_OBJECT(impl) { }
			filemanager(const filemanager &master) : ::SCAML::service(master), UDM_OBJECT(master) { }
			static filemanager Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static filemanager Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			filemanager CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			filemanager CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::SCAML::filemanager> Instances() { return Udm::InstantiatedAttr< ::SCAML::filemanager>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::SCAML::filemanager, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::SCAML::filemanager, Pred>(impl);}

			Udm::DerivedAttr< ::SCAML::filemanager> Derived() { return Udm::DerivedAttr< ::SCAML::filemanager>(impl);}
			template <class Pred> Udm::DerivedAttr< ::SCAML::filemanager, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::SCAML::filemanager, Pred>(impl);}

			Udm::ArchetypeAttr< ::SCAML::filemanager> Archetype() const { return Udm::ArchetypeAttr< ::SCAML::filemanager>(impl);}

			static ::Uml::Attribute meta_usagename;
			Udm::StringAttr usagename() const { return Udm::StringAttr(impl, meta_usagename); }

			Udm::ParentAttr< ::SCAML::MgaObject> parent() const { return Udm::ParentAttr< ::SCAML::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  namingservice :  public ::SCAML::service {
		public:
			static ::Uml::Class meta;

			namingservice() { }
			namingservice(Udm::ObjectImpl *impl) : ::SCAML::service(impl), UDM_OBJECT(impl) { }
			namingservice(const namingservice &master) : ::SCAML::service(master), UDM_OBJECT(master) { }
			static namingservice Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static namingservice Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			namingservice CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			namingservice CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::SCAML::namingservice> Instances() { return Udm::InstantiatedAttr< ::SCAML::namingservice>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::SCAML::namingservice, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::SCAML::namingservice, Pred>(impl);}

			Udm::DerivedAttr< ::SCAML::namingservice> Derived() { return Udm::DerivedAttr< ::SCAML::namingservice>(impl);}
			template <class Pred> Udm::DerivedAttr< ::SCAML::namingservice, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::SCAML::namingservice, Pred>(impl);}

			Udm::ArchetypeAttr< ::SCAML::namingservice> Archetype() const { return Udm::ArchetypeAttr< ::SCAML::namingservice>(impl);}

			Udm::ParentAttr< ::SCAML::MgaObject> parent() const { return Udm::ParentAttr< ::SCAML::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  eventchannel :  public ::SCAML::FreestandingPort {
		public:
			static ::Uml::Class meta;

			eventchannel() { }
			eventchannel(Udm::ObjectImpl *impl) : ::SCAML::FreestandingPort(impl), UDM_OBJECT(impl) { }
			eventchannel(const eventchannel &master) : ::SCAML::FreestandingPort(master), UDM_OBJECT(master) { }
			static eventchannel Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static eventchannel Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			eventchannel CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			eventchannel CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::SCAML::eventchannel> Instances() { return Udm::InstantiatedAttr< ::SCAML::eventchannel>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::SCAML::eventchannel, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::SCAML::eventchannel, Pred>(impl);}

			Udm::DerivedAttr< ::SCAML::eventchannel> Derived() { return Udm::DerivedAttr< ::SCAML::eventchannel>(impl);}
			template <class Pred> Udm::DerivedAttr< ::SCAML::eventchannel, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::SCAML::eventchannel, Pred>(impl);}

			Udm::ArchetypeAttr< ::SCAML::eventchannel> Archetype() const { return Udm::ArchetypeAttr< ::SCAML::eventchannel>(impl);}

			Udm::ParentAttr< ::SCAML::MgaObject> parent() const { return Udm::ParentAttr< ::SCAML::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  devicethatloadedthiscomponentref :  public ::SCAML::FreestandingPort {
		public:
			static ::Uml::Class meta;

			devicethatloadedthiscomponentref() { }
			devicethatloadedthiscomponentref(Udm::ObjectImpl *impl) : ::SCAML::FreestandingPort(impl), UDM_OBJECT(impl) { }
			devicethatloadedthiscomponentref(const devicethatloadedthiscomponentref &master) : ::SCAML::FreestandingPort(master), UDM_OBJECT(master) { }
			static devicethatloadedthiscomponentref Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static devicethatloadedthiscomponentref Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			devicethatloadedthiscomponentref CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			devicethatloadedthiscomponentref CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::SCAML::devicethatloadedthiscomponentref> Instances() { return Udm::InstantiatedAttr< ::SCAML::devicethatloadedthiscomponentref>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::SCAML::devicethatloadedthiscomponentref, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::SCAML::devicethatloadedthiscomponentref, Pred>(impl);}

			Udm::DerivedAttr< ::SCAML::devicethatloadedthiscomponentref> Derived() { return Udm::DerivedAttr< ::SCAML::devicethatloadedthiscomponentref>(impl);}
			template <class Pred> Udm::DerivedAttr< ::SCAML::devicethatloadedthiscomponentref, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::SCAML::devicethatloadedthiscomponentref, Pred>(impl);}

			Udm::ArchetypeAttr< ::SCAML::devicethatloadedthiscomponentref> Archetype() const { return Udm::ArchetypeAttr< ::SCAML::devicethatloadedthiscomponentref>(impl);}

			Udm::ParentAttr< ::SCAML::MgaObject> parent() const { return Udm::ParentAttr< ::SCAML::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  CFSrc :  virtual public ::SCAML::MgaObject {
		public:
			static ::Uml::Class meta;

			CFSrc() { }
			CFSrc(Udm::ObjectImpl *impl) : ::SCAML::MgaObject(impl), UDM_OBJECT(impl) { }
			CFSrc(const CFSrc &master) : ::SCAML::MgaObject(master), UDM_OBJECT(master) { }
			static CFSrc Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static CFSrc Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			CFSrc CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			CFSrc CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::SCAML::CFSrc> Instances() { return Udm::InstantiatedAttr< ::SCAML::CFSrc>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::SCAML::CFSrc, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::SCAML::CFSrc, Pred>(impl);}

			Udm::DerivedAttr< ::SCAML::CFSrc> Derived() { return Udm::DerivedAttr< ::SCAML::CFSrc>(impl);}
			template <class Pred> Udm::DerivedAttr< ::SCAML::CFSrc, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::SCAML::CFSrc, Pred>(impl);}

			Udm::ArchetypeAttr< ::SCAML::CFSrc> Archetype() const { return Udm::ArchetypeAttr< ::SCAML::CFSrc>(impl);}

			static ::Uml::AssociationRole meta_dstControlFlow;
			static ::Uml::AssociationRole meta_dstControlFlow_rev;
			Udm::AClassAssocAttr< ::SCAML::ControlFlow, ::SCAML::CFDst> dstControlFlow() const { return Udm::AClassAssocAttr< ::SCAML::ControlFlow, ::SCAML::CFDst>(impl, meta_dstControlFlow, meta_dstControlFlow_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::SCAML::ControlFlow, ::SCAML::CFDst, Pred> dstControlFlow_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::SCAML::ControlFlow, ::SCAML::CFDst, Pred>(impl, meta_dstControlFlow, meta_dstControlFlow_rev); }

			Udm::ParentAttr<Udm::Object> parent() const { return Udm::ParentAttr<Udm::Object>(impl, Udm::NULLPARENTROLE); }
		};

		class  ArgRef :  virtual public ::SCAML::MgaObject {
		public:
			static ::Uml::Class meta;

			ArgRef() { }
			ArgRef(Udm::ObjectImpl *impl) : ::SCAML::MgaObject(impl), UDM_OBJECT(impl) { }
			ArgRef(const ArgRef &master) : ::SCAML::MgaObject(master), UDM_OBJECT(master) { }
			static ArgRef Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static ArgRef Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			ArgRef CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			ArgRef CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::SCAML::ArgRef> Instances() { return Udm::InstantiatedAttr< ::SCAML::ArgRef>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::SCAML::ArgRef, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::SCAML::ArgRef, Pred>(impl);}

			Udm::DerivedAttr< ::SCAML::ArgRef> Derived() { return Udm::DerivedAttr< ::SCAML::ArgRef>(impl);}
			template <class Pred> Udm::DerivedAttr< ::SCAML::ArgRef, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::SCAML::ArgRef, Pred>(impl);}

			Udm::ArchetypeAttr< ::SCAML::ArgRef> Archetype() const { return Udm::ArchetypeAttr< ::SCAML::ArgRef>(impl);}

			static ::Uml::AssociationRole meta_referedbyArg;
			Udm::AssocAttr< ::SCAML::Arg> referedbyArg() const { return Udm::AssocAttr< ::SCAML::Arg>(impl, meta_referedbyArg); }
			template <class Pred> Udm::AssocAttr< ::SCAML::Arg, Pred > referedbyArg_sorted(const Pred &) const { return Udm::AssocAttr< ::SCAML::Arg, Pred>(impl, meta_referedbyArg); }

			Udm::ParentAttr<Udm::Object> parent() const { return Udm::ParentAttr<Udm::Object>(impl, Udm::NULLPARENTROLE); }
		};

		class  Arg :  public ::SCAML::MgaObject {
		public:
			static ::Uml::Class meta;

			Arg() { }
			Arg(Udm::ObjectImpl *impl) : ::SCAML::MgaObject(impl), UDM_OBJECT(impl) { }
			Arg(const Arg &master) : ::SCAML::MgaObject(master), UDM_OBJECT(master) { }
			static Arg Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static Arg Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			Arg CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			Arg CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::SCAML::Arg> Instances() { return Udm::InstantiatedAttr< ::SCAML::Arg>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::SCAML::Arg, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::SCAML::Arg, Pred>(impl);}

			Udm::DerivedAttr< ::SCAML::Arg> Derived() { return Udm::DerivedAttr< ::SCAML::Arg>(impl);}
			template <class Pred> Udm::DerivedAttr< ::SCAML::Arg, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::SCAML::Arg, Pred>(impl);}

			Udm::ArchetypeAttr< ::SCAML::Arg> Archetype() const { return Udm::ArchetypeAttr< ::SCAML::Arg>(impl);}

			static ::Uml::AssociationRole meta_ref;
			Udm::PointerAttr< ::SCAML::ArgRef> ref() const { return Udm::PointerAttr< ::SCAML::ArgRef>(impl, meta_ref); }

			static ::Uml::CompositionParentRole meta_SimulinkEntrypoint_parent;
			Udm::ParentAttr< ::SCAML::SimulinkEntrypoint> SimulinkEntrypoint_parent() const { return Udm::ParentAttr< ::SCAML::SimulinkEntrypoint>(impl, meta_SimulinkEntrypoint_parent); }

			Udm::ParentAttr< ::SCAML::MgaObject> parent() const { return Udm::ParentAttr< ::SCAML::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  CFDst :  virtual public ::SCAML::MgaObject {
		public:
			static ::Uml::Class meta;

			CFDst() { }
			CFDst(Udm::ObjectImpl *impl) : ::SCAML::MgaObject(impl), UDM_OBJECT(impl) { }
			CFDst(const CFDst &master) : ::SCAML::MgaObject(master), UDM_OBJECT(master) { }
			static CFDst Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static CFDst Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			CFDst CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			CFDst CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::SCAML::CFDst> Instances() { return Udm::InstantiatedAttr< ::SCAML::CFDst>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::SCAML::CFDst, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::SCAML::CFDst, Pred>(impl);}

			Udm::DerivedAttr< ::SCAML::CFDst> Derived() { return Udm::DerivedAttr< ::SCAML::CFDst>(impl);}
			template <class Pred> Udm::DerivedAttr< ::SCAML::CFDst, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::SCAML::CFDst, Pred>(impl);}

			Udm::ArchetypeAttr< ::SCAML::CFDst> Archetype() const { return Udm::ArchetypeAttr< ::SCAML::CFDst>(impl);}

			static ::Uml::AssociationRole meta_srcControlFlow;
			static ::Uml::AssociationRole meta_srcControlFlow_rev;
			Udm::AClassAssocAttr< ::SCAML::ControlFlow, ::SCAML::CFSrc> srcControlFlow() const { return Udm::AClassAssocAttr< ::SCAML::ControlFlow, ::SCAML::CFSrc>(impl, meta_srcControlFlow, meta_srcControlFlow_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::SCAML::ControlFlow, ::SCAML::CFSrc, Pred> srcControlFlow_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::SCAML::ControlFlow, ::SCAML::CFSrc, Pred>(impl, meta_srcControlFlow, meta_srcControlFlow_rev); }

			static ::Uml::AssociationRole meta_srcElseFlow;
			static ::Uml::AssociationRole meta_srcElseFlow_rev;
			Udm::AClassAssocAttr< ::SCAML::ElseFlow, ::SCAML::Branch> srcElseFlow() const { return Udm::AClassAssocAttr< ::SCAML::ElseFlow, ::SCAML::Branch>(impl, meta_srcElseFlow, meta_srcElseFlow_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::SCAML::ElseFlow, ::SCAML::Branch, Pred> srcElseFlow_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::SCAML::ElseFlow, ::SCAML::Branch, Pred>(impl, meta_srcElseFlow, meta_srcElseFlow_rev); }

			static ::Uml::AssociationRole meta_srcThenFlow;
			static ::Uml::AssociationRole meta_srcThenFlow_rev;
			Udm::AClassAssocAttr< ::SCAML::ThenFlow, ::SCAML::Branch> srcThenFlow() const { return Udm::AClassAssocAttr< ::SCAML::ThenFlow, ::SCAML::Branch>(impl, meta_srcThenFlow, meta_srcThenFlow_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::SCAML::ThenFlow, ::SCAML::Branch, Pred> srcThenFlow_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::SCAML::ThenFlow, ::SCAML::Branch, Pred>(impl, meta_srcThenFlow, meta_srcThenFlow_rev); }

			Udm::ParentAttr<Udm::Object> parent() const { return Udm::ParentAttr<Udm::Object>(impl, Udm::NULLPARENTROLE); }
		};

		class  SimulinkEntrypoint :  public ::SCAML::CFSrc, public ::SCAML::CFDst {
		public:
			static ::Uml::Class meta;

			SimulinkEntrypoint() { }
			SimulinkEntrypoint(Udm::ObjectImpl *impl) : ::SCAML::CFSrc(impl),::SCAML::CFDst(impl), ::SCAML::MgaObject(impl), UDM_OBJECT(impl) { }
			SimulinkEntrypoint(const SimulinkEntrypoint &master) : ::SCAML::CFSrc(master),::SCAML::CFDst(master), ::SCAML::MgaObject(master), UDM_OBJECT(master) { }
			static SimulinkEntrypoint Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static SimulinkEntrypoint Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			SimulinkEntrypoint CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			SimulinkEntrypoint CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::SCAML::SimulinkEntrypoint> Instances() { return Udm::InstantiatedAttr< ::SCAML::SimulinkEntrypoint>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::SCAML::SimulinkEntrypoint, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::SCAML::SimulinkEntrypoint, Pred>(impl);}

			Udm::DerivedAttr< ::SCAML::SimulinkEntrypoint> Derived() { return Udm::DerivedAttr< ::SCAML::SimulinkEntrypoint>(impl);}
			template <class Pred> Udm::DerivedAttr< ::SCAML::SimulinkEntrypoint, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::SCAML::SimulinkEntrypoint, Pred>(impl);}

			Udm::ArchetypeAttr< ::SCAML::SimulinkEntrypoint> Archetype() const { return Udm::ArchetypeAttr< ::SCAML::SimulinkEntrypoint>(impl);}

			static ::Uml::CompositionChildRole meta_Arg_children;
			Udm::ChildrenAttr< ::SCAML::Arg> Arg_children() const { return Udm::ChildrenAttr< ::SCAML::Arg>(impl, meta_Arg_children); }
			template <class Pred> Udm::ChildrenAttr< ::SCAML::Arg, Pred> Arg_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::Arg, Pred>(impl, meta_Arg_children); }

			Udm::ChildrenAttr< ::SCAML::Arg> Arg_kind_children() const { return Udm::ChildrenAttr< ::SCAML::Arg>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::Arg, Pred> Arg_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::Arg, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr< ::SCAML::MgaObject>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

			static ::Uml::CompositionParentRole meta_SimulinkSystem_parent;
			Udm::ParentAttr< ::SCAML::SimulinkSystem> SimulinkSystem_parent() const { return Udm::ParentAttr< ::SCAML::SimulinkSystem>(impl, meta_SimulinkSystem_parent); }

			Udm::ParentAttr< ::SCAML::MgaObject> parent() const { return Udm::ParentAttr< ::SCAML::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  controllertocompinst :  public ::SCAML::MgaObject {
		public:
			static ::Uml::Class meta;

			controllertocompinst() { }
			controllertocompinst(Udm::ObjectImpl *impl) : ::SCAML::MgaObject(impl), UDM_OBJECT(impl) { }
			controllertocompinst(const controllertocompinst &master) : ::SCAML::MgaObject(master), UDM_OBJECT(master) { }
			static controllertocompinst Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static controllertocompinst Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			controllertocompinst CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			controllertocompinst CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::SCAML::controllertocompinst> Instances() { return Udm::InstantiatedAttr< ::SCAML::controllertocompinst>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::SCAML::controllertocompinst, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::SCAML::controllertocompinst, Pred>(impl);}

			Udm::DerivedAttr< ::SCAML::controllertocompinst> Derived() { return Udm::DerivedAttr< ::SCAML::controllertocompinst>(impl);}
			template <class Pred> Udm::DerivedAttr< ::SCAML::controllertocompinst, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::SCAML::controllertocompinst, Pred>(impl);}

			Udm::ArchetypeAttr< ::SCAML::controllertocompinst> Archetype() const { return Udm::ArchetypeAttr< ::SCAML::controllertocompinst>(impl);}

			static ::Uml::CompositionParentRole meta_SoftwareAssembly_parent;
			Udm::ParentAttr< ::SCAML::SoftwareAssembly> SoftwareAssembly_parent() const { return Udm::ParentAttr< ::SCAML::SoftwareAssembly>(impl, meta_SoftwareAssembly_parent); }

			Udm::ParentAttr< ::SCAML::MgaObject> parent() const { return Udm::ParentAttr< ::SCAML::MgaObject>(impl, Udm::NULLPARENTROLE); }
			static ::Uml::AssociationRole meta_dstcontrollertocompinst_end_;
			Udm::AssocEndAttr< ::SCAML::SoftwareComponent> dstcontrollertocompinst_end() const { return Udm::AssocEndAttr< ::SCAML::SoftwareComponent>(impl, meta_dstcontrollertocompinst_end_); }

			static ::Uml::AssociationRole meta_srccontrollertocompinst_end_;
			Udm::AssocEndAttr< ::SCAML::assemblycontroller> srccontrollertocompinst_end() const { return Udm::AssocEndAttr< ::SCAML::assemblycontroller>(impl, meta_srccontrollertocompinst_end_); }

		};

		class  assemblycontroller :  public ::SCAML::MgaObject {
		public:
			static ::Uml::Class meta;

			assemblycontroller() { }
			assemblycontroller(Udm::ObjectImpl *impl) : ::SCAML::MgaObject(impl), UDM_OBJECT(impl) { }
			assemblycontroller(const assemblycontroller &master) : ::SCAML::MgaObject(master), UDM_OBJECT(master) { }
			static assemblycontroller Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static assemblycontroller Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			assemblycontroller CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			assemblycontroller CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::SCAML::assemblycontroller> Instances() { return Udm::InstantiatedAttr< ::SCAML::assemblycontroller>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::SCAML::assemblycontroller, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::SCAML::assemblycontroller, Pred>(impl);}

			Udm::DerivedAttr< ::SCAML::assemblycontroller> Derived() { return Udm::DerivedAttr< ::SCAML::assemblycontroller>(impl);}
			template <class Pred> Udm::DerivedAttr< ::SCAML::assemblycontroller, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::SCAML::assemblycontroller, Pred>(impl);}

			Udm::ArchetypeAttr< ::SCAML::assemblycontroller> Archetype() const { return Udm::ArchetypeAttr< ::SCAML::assemblycontroller>(impl);}

			static ::Uml::AssociationRole meta_dstcontrollertocompinst;
			static ::Uml::AssociationRole meta_dstcontrollertocompinst_rev;
			Udm::AClassAssocAttr< ::SCAML::controllertocompinst, ::SCAML::SoftwareComponent> dstcontrollertocompinst() const { return Udm::AClassAssocAttr< ::SCAML::controllertocompinst, ::SCAML::SoftwareComponent>(impl, meta_dstcontrollertocompinst, meta_dstcontrollertocompinst_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::SCAML::controllertocompinst, ::SCAML::SoftwareComponent, Pred> dstcontrollertocompinst_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::SCAML::controllertocompinst, ::SCAML::SoftwareComponent, Pred>(impl, meta_dstcontrollertocompinst, meta_dstcontrollertocompinst_rev); }

			static ::Uml::CompositionParentRole meta_SoftwareAssembly_parent;
			Udm::ParentAttr< ::SCAML::SoftwareAssembly> SoftwareAssembly_parent() const { return Udm::ParentAttr< ::SCAML::SoftwareAssembly>(impl, meta_SoftwareAssembly_parent); }

			Udm::ParentAttr< ::SCAML::MgaObject> parent() const { return Udm::ParentAttr< ::SCAML::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  componentresourcefactorytopropertiesConn :  public ::SCAML::MgaObject {
		public:
			static ::Uml::Class meta;

			componentresourcefactorytopropertiesConn() { }
			componentresourcefactorytopropertiesConn(Udm::ObjectImpl *impl) : ::SCAML::MgaObject(impl), UDM_OBJECT(impl) { }
			componentresourcefactorytopropertiesConn(const componentresourcefactorytopropertiesConn &master) : ::SCAML::MgaObject(master), UDM_OBJECT(master) { }
			static componentresourcefactorytopropertiesConn Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static componentresourcefactorytopropertiesConn Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			componentresourcefactorytopropertiesConn CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			componentresourcefactorytopropertiesConn CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::SCAML::componentresourcefactorytopropertiesConn> Instances() { return Udm::InstantiatedAttr< ::SCAML::componentresourcefactorytopropertiesConn>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::SCAML::componentresourcefactorytopropertiesConn, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::SCAML::componentresourcefactorytopropertiesConn, Pred>(impl);}

			Udm::DerivedAttr< ::SCAML::componentresourcefactorytopropertiesConn> Derived() { return Udm::DerivedAttr< ::SCAML::componentresourcefactorytopropertiesConn>(impl);}
			template <class Pred> Udm::DerivedAttr< ::SCAML::componentresourcefactorytopropertiesConn, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::SCAML::componentresourcefactorytopropertiesConn, Pred>(impl);}

			Udm::ArchetypeAttr< ::SCAML::componentresourcefactorytopropertiesConn> Archetype() const { return Udm::ArchetypeAttr< ::SCAML::componentresourcefactorytopropertiesConn>(impl);}

			static ::Uml::CompositionParentRole meta_SoftwareAssembly_parent;
			Udm::ParentAttr< ::SCAML::SoftwareAssembly> SoftwareAssembly_parent() const { return Udm::ParentAttr< ::SCAML::SoftwareAssembly>(impl, meta_SoftwareAssembly_parent); }

			Udm::ParentAttr< ::SCAML::MgaObject> parent() const { return Udm::ParentAttr< ::SCAML::MgaObject>(impl, Udm::NULLPARENTROLE); }
			static ::Uml::AssociationRole meta_srccomponentresourcefactorytopropertiesConn_end_;
			Udm::AssocEndAttr< ::SCAML::componentresourcefactoryref> srccomponentresourcefactorytopropertiesConn_end() const { return Udm::AssocEndAttr< ::SCAML::componentresourcefactoryref>(impl, meta_srccomponentresourcefactorytopropertiesConn_end_); }

			static ::Uml::AssociationRole meta_dstcomponentresourcefactorytopropertiesConn_end_;
			Udm::AssocEndAttr< ::SCAML::componentproperties> dstcomponentresourcefactorytopropertiesConn_end() const { return Udm::AssocEndAttr< ::SCAML::componentproperties>(impl, meta_dstcomponentresourcefactorytopropertiesConn_end_); }

		};

		class  findcomponentConn :  public ::SCAML::MgaObject {
		public:
			static ::Uml::Class meta;

			findcomponentConn() { }
			findcomponentConn(Udm::ObjectImpl *impl) : ::SCAML::MgaObject(impl), UDM_OBJECT(impl) { }
			findcomponentConn(const findcomponentConn &master) : ::SCAML::MgaObject(master), UDM_OBJECT(master) { }
			static findcomponentConn Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static findcomponentConn Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			findcomponentConn CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			findcomponentConn CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::SCAML::findcomponentConn> Instances() { return Udm::InstantiatedAttr< ::SCAML::findcomponentConn>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::SCAML::findcomponentConn, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::SCAML::findcomponentConn, Pred>(impl);}

			Udm::DerivedAttr< ::SCAML::findcomponentConn> Derived() { return Udm::DerivedAttr< ::SCAML::findcomponentConn>(impl);}
			template <class Pred> Udm::DerivedAttr< ::SCAML::findcomponentConn, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::SCAML::findcomponentConn, Pred>(impl);}

			Udm::ArchetypeAttr< ::SCAML::findcomponentConn> Archetype() const { return Udm::ArchetypeAttr< ::SCAML::findcomponentConn>(impl);}

			static ::Uml::CompositionParentRole meta_SoftwareAssembly_parent;
			Udm::ParentAttr< ::SCAML::SoftwareAssembly> SoftwareAssembly_parent() const { return Udm::ParentAttr< ::SCAML::SoftwareAssembly>(impl, meta_SoftwareAssembly_parent); }

			Udm::ParentAttr< ::SCAML::MgaObject> parent() const { return Udm::ParentAttr< ::SCAML::MgaObject>(impl, Udm::NULLPARENTROLE); }
			static ::Uml::AssociationRole meta_srcfindcomponentConn_end_;
			Udm::AssocEndAttr< ::SCAML::componentresourcefactoryref> srcfindcomponentConn_end() const { return Udm::AssocEndAttr< ::SCAML::componentresourcefactoryref>(impl, meta_srcfindcomponentConn_end_); }

			static ::Uml::AssociationRole meta_dstfindcomponentConn_end_;
			Udm::AssocEndAttr< ::SCAML::SoftwareComponent> dstfindcomponentConn_end() const { return Udm::AssocEndAttr< ::SCAML::SoftwareComponent>(impl, meta_dstfindcomponentConn_end_); }

		};

		class  componentresourcefactoryref :  public ::SCAML::MgaObject {
		public:
			static ::Uml::Class meta;

			componentresourcefactoryref() { }
			componentresourcefactoryref(Udm::ObjectImpl *impl) : ::SCAML::MgaObject(impl), UDM_OBJECT(impl) { }
			componentresourcefactoryref(const componentresourcefactoryref &master) : ::SCAML::MgaObject(master), UDM_OBJECT(master) { }
			static componentresourcefactoryref Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static componentresourcefactoryref Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			componentresourcefactoryref CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			componentresourcefactoryref CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::SCAML::componentresourcefactoryref> Instances() { return Udm::InstantiatedAttr< ::SCAML::componentresourcefactoryref>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::SCAML::componentresourcefactoryref, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::SCAML::componentresourcefactoryref, Pred>(impl);}

			Udm::DerivedAttr< ::SCAML::componentresourcefactoryref> Derived() { return Udm::DerivedAttr< ::SCAML::componentresourcefactoryref>(impl);}
			template <class Pred> Udm::DerivedAttr< ::SCAML::componentresourcefactoryref, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::SCAML::componentresourcefactoryref, Pred>(impl);}

			Udm::ArchetypeAttr< ::SCAML::componentresourcefactoryref> Archetype() const { return Udm::ArchetypeAttr< ::SCAML::componentresourcefactoryref>(impl);}

			static ::Uml::AssociationRole meta_dstfindcomponentConn;
			static ::Uml::AssociationRole meta_dstfindcomponentConn_rev;
			Udm::AClassAssocAttr< ::SCAML::findcomponentConn, ::SCAML::SoftwareComponent> dstfindcomponentConn() const { return Udm::AClassAssocAttr< ::SCAML::findcomponentConn, ::SCAML::SoftwareComponent>(impl, meta_dstfindcomponentConn, meta_dstfindcomponentConn_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::SCAML::findcomponentConn, ::SCAML::SoftwareComponent, Pred> dstfindcomponentConn_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::SCAML::findcomponentConn, ::SCAML::SoftwareComponent, Pred>(impl, meta_dstfindcomponentConn, meta_dstfindcomponentConn_rev); }

			static ::Uml::AssociationRole meta_dstcomponentresourcefactorytopropertiesConn;
			static ::Uml::AssociationRole meta_dstcomponentresourcefactorytopropertiesConn_rev;
			Udm::AClassAssocAttr< ::SCAML::componentresourcefactorytopropertiesConn, ::SCAML::componentproperties> dstcomponentresourcefactorytopropertiesConn() const { return Udm::AClassAssocAttr< ::SCAML::componentresourcefactorytopropertiesConn, ::SCAML::componentproperties>(impl, meta_dstcomponentresourcefactorytopropertiesConn, meta_dstcomponentresourcefactorytopropertiesConn_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::SCAML::componentresourcefactorytopropertiesConn, ::SCAML::componentproperties, Pred> dstcomponentresourcefactorytopropertiesConn_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::SCAML::componentresourcefactorytopropertiesConn, ::SCAML::componentproperties, Pred>(impl, meta_dstcomponentresourcefactorytopropertiesConn, meta_dstcomponentresourcefactorytopropertiesConn_rev); }

			static ::Uml::AssociationRole meta_ref;
			Udm::PointerAttr< ::SCAML::SoftwareComponent> ref() const { return Udm::PointerAttr< ::SCAML::SoftwareComponent>(impl, meta_ref); }

			static ::Uml::CompositionParentRole meta_SoftwareAssembly_parent;
			Udm::ParentAttr< ::SCAML::SoftwareAssembly> SoftwareAssembly_parent() const { return Udm::ParentAttr< ::SCAML::SoftwareAssembly>(impl, meta_SoftwareAssembly_parent); }

			Udm::ParentAttr< ::SCAML::MgaObject> parent() const { return Udm::ParentAttr< ::SCAML::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  propertyProxy :  virtual public ::SCAML::MgaObject {
		public:
			static ::Uml::Class meta;

			propertyProxy() { }
			propertyProxy(Udm::ObjectImpl *impl) : ::SCAML::MgaObject(impl), UDM_OBJECT(impl) { }
			propertyProxy(const propertyProxy &master) : ::SCAML::MgaObject(master), UDM_OBJECT(master) { }
			static propertyProxy Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static propertyProxy Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			propertyProxy CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			propertyProxy CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::SCAML::propertyProxy> Instances() { return Udm::InstantiatedAttr< ::SCAML::propertyProxy>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::SCAML::propertyProxy, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::SCAML::propertyProxy, Pred>(impl);}

			Udm::DerivedAttr< ::SCAML::propertyProxy> Derived() { return Udm::DerivedAttr< ::SCAML::propertyProxy>(impl);}
			template <class Pred> Udm::DerivedAttr< ::SCAML::propertyProxy, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::SCAML::propertyProxy, Pred>(impl);}

			Udm::ArchetypeAttr< ::SCAML::propertyProxy> Archetype() const { return Udm::ArchetypeAttr< ::SCAML::propertyProxy>(impl);}

			static ::Uml::AssociationRole meta_referedbycomponentproperties;
			Udm::AssocAttr< ::SCAML::componentproperties> referedbycomponentproperties() const { return Udm::AssocAttr< ::SCAML::componentproperties>(impl, meta_referedbycomponentproperties); }
			template <class Pred> Udm::AssocAttr< ::SCAML::componentproperties, Pred > referedbycomponentproperties_sorted(const Pred &) const { return Udm::AssocAttr< ::SCAML::componentproperties, Pred>(impl, meta_referedbycomponentproperties); }

			Udm::ParentAttr<Udm::Object> parent() const { return Udm::ParentAttr<Udm::Object>(impl, Udm::NULLPARENTROLE); }
		};

		class  componentpropertiesConn :  public ::SCAML::MgaObject {
		public:
			static ::Uml::Class meta;

			componentpropertiesConn() { }
			componentpropertiesConn(Udm::ObjectImpl *impl) : ::SCAML::MgaObject(impl), UDM_OBJECT(impl) { }
			componentpropertiesConn(const componentpropertiesConn &master) : ::SCAML::MgaObject(master), UDM_OBJECT(master) { }
			static componentpropertiesConn Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static componentpropertiesConn Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			componentpropertiesConn CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			componentpropertiesConn CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::SCAML::componentpropertiesConn> Instances() { return Udm::InstantiatedAttr< ::SCAML::componentpropertiesConn>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::SCAML::componentpropertiesConn, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::SCAML::componentpropertiesConn, Pred>(impl);}

			Udm::DerivedAttr< ::SCAML::componentpropertiesConn> Derived() { return Udm::DerivedAttr< ::SCAML::componentpropertiesConn>(impl);}
			template <class Pred> Udm::DerivedAttr< ::SCAML::componentpropertiesConn, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::SCAML::componentpropertiesConn, Pred>(impl);}

			Udm::ArchetypeAttr< ::SCAML::componentpropertiesConn> Archetype() const { return Udm::ArchetypeAttr< ::SCAML::componentpropertiesConn>(impl);}

			static ::Uml::CompositionParentRole meta_DeviceConfiguration_parent;
			Udm::ParentAttr< ::SCAML::DeviceConfiguration> DeviceConfiguration_parent() const { return Udm::ParentAttr< ::SCAML::DeviceConfiguration>(impl, meta_DeviceConfiguration_parent); }

			static ::Uml::CompositionParentRole meta_SoftwareAssembly_parent;
			Udm::ParentAttr< ::SCAML::SoftwareAssembly> SoftwareAssembly_parent() const { return Udm::ParentAttr< ::SCAML::SoftwareAssembly>(impl, meta_SoftwareAssembly_parent); }

			Udm::ParentAttr< ::SCAML::MgaObject> parent() const { return Udm::ParentAttr< ::SCAML::MgaObject>(impl, Udm::NULLPARENTROLE); }
			static ::Uml::AssociationRole meta_dstcomponentpropertiesConn_end_;
			Udm::AssocEndAttr< ::SCAML::componentproperties> dstcomponentpropertiesConn_end() const { return Udm::AssocEndAttr< ::SCAML::componentproperties>(impl, meta_dstcomponentpropertiesConn_end_); }

			static ::Uml::AssociationRole meta_srccomponentpropertiesConn_end_;
			Udm::AssocEndAttr< ::SCAML::SoftwareComponent> srccomponentpropertiesConn_end() const { return Udm::AssocEndAttr< ::SCAML::SoftwareComponent>(impl, meta_srccomponentpropertiesConn_end_); }

		};

		class  componentproperties :  public ::SCAML::componentplacement_Members_Base, public ::SCAML::MgaObject {
		public:
			static ::Uml::Class meta;

			componentproperties() { }
			componentproperties(Udm::ObjectImpl *impl) : ::SCAML::componentplacement_Members_Base(impl),::SCAML::MgaObject(impl), UDM_OBJECT(impl) { }
			componentproperties(const componentproperties &master) : ::SCAML::componentplacement_Members_Base(master),::SCAML::MgaObject(master), UDM_OBJECT(master) { }
			static componentproperties Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static componentproperties Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			componentproperties CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			componentproperties CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::SCAML::componentproperties> Instances() { return Udm::InstantiatedAttr< ::SCAML::componentproperties>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::SCAML::componentproperties, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::SCAML::componentproperties, Pred>(impl);}

			Udm::DerivedAttr< ::SCAML::componentproperties> Derived() { return Udm::DerivedAttr< ::SCAML::componentproperties>(impl);}
			template <class Pred> Udm::DerivedAttr< ::SCAML::componentproperties, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::SCAML::componentproperties, Pred>(impl);}

			Udm::ArchetypeAttr< ::SCAML::componentproperties> Archetype() const { return Udm::ArchetypeAttr< ::SCAML::componentproperties>(impl);}

			static ::Uml::Attribute meta_value_s;
			Udm::StringAttr value_s() const { return Udm::StringAttr(impl, meta_value_s); }

			static ::Uml::AssociationRole meta_srccomponentresourcefactorytopropertiesConn;
			static ::Uml::AssociationRole meta_srccomponentresourcefactorytopropertiesConn_rev;
			Udm::AClassAssocAttr< ::SCAML::componentresourcefactorytopropertiesConn, ::SCAML::componentresourcefactoryref> srccomponentresourcefactorytopropertiesConn() const { return Udm::AClassAssocAttr< ::SCAML::componentresourcefactorytopropertiesConn, ::SCAML::componentresourcefactoryref>(impl, meta_srccomponentresourcefactorytopropertiesConn, meta_srccomponentresourcefactorytopropertiesConn_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::SCAML::componentresourcefactorytopropertiesConn, ::SCAML::componentresourcefactoryref, Pred> srccomponentresourcefactorytopropertiesConn_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::SCAML::componentresourcefactorytopropertiesConn, ::SCAML::componentresourcefactoryref, Pred>(impl, meta_srccomponentresourcefactorytopropertiesConn, meta_srccomponentresourcefactorytopropertiesConn_rev); }

			static ::Uml::AssociationRole meta_ref;
			Udm::PointerAttr< ::SCAML::propertyProxy> ref() const { return Udm::PointerAttr< ::SCAML::propertyProxy>(impl, meta_ref); }

			static ::Uml::AssociationRole meta_srccomponentpropertiesConn;
			static ::Uml::AssociationRole meta_srccomponentpropertiesConn_rev;
			Udm::AClassAssocAttr< ::SCAML::componentpropertiesConn, ::SCAML::SoftwareComponent> srccomponentpropertiesConn() const { return Udm::AClassAssocAttr< ::SCAML::componentpropertiesConn, ::SCAML::SoftwareComponent>(impl, meta_srccomponentpropertiesConn, meta_srccomponentpropertiesConn_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::SCAML::componentpropertiesConn, ::SCAML::SoftwareComponent, Pred> srccomponentpropertiesConn_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::SCAML::componentpropertiesConn, ::SCAML::SoftwareComponent, Pred>(impl, meta_srccomponentpropertiesConn, meta_srccomponentpropertiesConn_rev); }

			static ::Uml::CompositionParentRole meta_DeviceConfiguration_parent;
			Udm::ParentAttr< ::SCAML::DeviceConfiguration> DeviceConfiguration_parent() const { return Udm::ParentAttr< ::SCAML::DeviceConfiguration>(impl, meta_DeviceConfiguration_parent); }

			static ::Uml::CompositionParentRole meta_SoftwareAssembly_parent;
			Udm::ParentAttr< ::SCAML::SoftwareAssembly> SoftwareAssembly_parent() const { return Udm::ParentAttr< ::SCAML::SoftwareAssembly>(impl, meta_SoftwareAssembly_parent); }

			Udm::ParentAttr< ::SCAML::MgaObject> parent() const { return Udm::ParentAttr< ::SCAML::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  componentplacementConn :  public ::SCAML::MgaObject {
		public:
			static ::Uml::Class meta;

			componentplacementConn() { }
			componentplacementConn(Udm::ObjectImpl *impl) : ::SCAML::MgaObject(impl), UDM_OBJECT(impl) { }
			componentplacementConn(const componentplacementConn &master) : ::SCAML::MgaObject(master), UDM_OBJECT(master) { }
			static componentplacementConn Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static componentplacementConn Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			componentplacementConn CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			componentplacementConn CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::SCAML::componentplacementConn> Instances() { return Udm::InstantiatedAttr< ::SCAML::componentplacementConn>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::SCAML::componentplacementConn, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::SCAML::componentplacementConn, Pred>(impl);}

			Udm::DerivedAttr< ::SCAML::componentplacementConn> Derived() { return Udm::DerivedAttr< ::SCAML::componentplacementConn>(impl);}
			template <class Pred> Udm::DerivedAttr< ::SCAML::componentplacementConn, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::SCAML::componentplacementConn, Pred>(impl);}

			Udm::ArchetypeAttr< ::SCAML::componentplacementConn> Archetype() const { return Udm::ArchetypeAttr< ::SCAML::componentplacementConn>(impl);}

			static ::Uml::CompositionParentRole meta_SoftwareAssembly_parent;
			Udm::ParentAttr< ::SCAML::SoftwareAssembly> SoftwareAssembly_parent() const { return Udm::ParentAttr< ::SCAML::SoftwareAssembly>(impl, meta_SoftwareAssembly_parent); }

			Udm::ParentAttr< ::SCAML::MgaObject> parent() const { return Udm::ParentAttr< ::SCAML::MgaObject>(impl, Udm::NULLPARENTROLE); }
			static ::Uml::AssociationRole meta_srccomponentplacementConn_end_;
			Udm::AssocEndAttr< ::SCAML::componentfile> srccomponentplacementConn_end() const { return Udm::AssocEndAttr< ::SCAML::componentfile>(impl, meta_srccomponentplacementConn_end_); }

			static ::Uml::AssociationRole meta_dstcomponentplacementConn_end_;
			Udm::AssocEndAttr< ::SCAML::SoftwareComponent> dstcomponentplacementConn_end() const { return Udm::AssocEndAttr< ::SCAML::SoftwareComponent>(impl, meta_dstcomponentplacementConn_end_); }

		};

		class  hostcollocation :  public ::SCAML::MgaObject {
		public:
			static ::Uml::Class meta;

			hostcollocation() { }
			hostcollocation(Udm::ObjectImpl *impl) : ::SCAML::MgaObject(impl), UDM_OBJECT(impl) { }
			hostcollocation(const hostcollocation &master) : ::SCAML::MgaObject(master), UDM_OBJECT(master) { }
			static hostcollocation Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static hostcollocation Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			hostcollocation CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			hostcollocation CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::SCAML::hostcollocation> Instances() { return Udm::InstantiatedAttr< ::SCAML::hostcollocation>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::SCAML::hostcollocation, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::SCAML::hostcollocation, Pred>(impl);}

			Udm::DerivedAttr< ::SCAML::hostcollocation> Derived() { return Udm::DerivedAttr< ::SCAML::hostcollocation>(impl);}
			template <class Pred> Udm::DerivedAttr< ::SCAML::hostcollocation, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::SCAML::hostcollocation, Pred>(impl);}

			Udm::ArchetypeAttr< ::SCAML::hostcollocation> Archetype() const { return Udm::ArchetypeAttr< ::SCAML::hostcollocation>(impl);}

			static ::Uml::AssociationRole meta_members;
			Udm::AssocAttr< ::SCAML::SoftwareComponent> members() const { return Udm::AssocAttr< ::SCAML::SoftwareComponent>(impl, meta_members); }
			template <class Pred> Udm::AssocAttr< ::SCAML::SoftwareComponent, Pred > members_sorted(const Pred &) const { return Udm::AssocAttr< ::SCAML::SoftwareComponent, Pred>(impl, meta_members); }

			static ::Uml::CompositionParentRole meta_SoftwareAssembly_parent;
			Udm::ParentAttr< ::SCAML::SoftwareAssembly> SoftwareAssembly_parent() const { return Udm::ParentAttr< ::SCAML::SoftwareAssembly>(impl, meta_SoftwareAssembly_parent); }

			Udm::ParentAttr< ::SCAML::MgaObject> parent() const { return Udm::ParentAttr< ::SCAML::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  inheritsfrom :  public ::SCAML::MgaObject {
		public:
			static ::Uml::Class meta;

			inheritsfrom() { }
			inheritsfrom(Udm::ObjectImpl *impl) : ::SCAML::MgaObject(impl), UDM_OBJECT(impl) { }
			inheritsfrom(const inheritsfrom &master) : ::SCAML::MgaObject(master), UDM_OBJECT(master) { }
			static inheritsfrom Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static inheritsfrom Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			inheritsfrom CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			inheritsfrom CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::SCAML::inheritsfrom> Instances() { return Udm::InstantiatedAttr< ::SCAML::inheritsfrom>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::SCAML::inheritsfrom, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::SCAML::inheritsfrom, Pred>(impl);}

			Udm::DerivedAttr< ::SCAML::inheritsfrom> Derived() { return Udm::DerivedAttr< ::SCAML::inheritsfrom>(impl);}
			template <class Pred> Udm::DerivedAttr< ::SCAML::inheritsfrom, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::SCAML::inheritsfrom, Pred>(impl);}

			Udm::ArchetypeAttr< ::SCAML::inheritsfrom> Archetype() const { return Udm::ArchetypeAttr< ::SCAML::inheritsfrom>(impl);}

			static ::Uml::CompositionParentRole meta_interfaces_parent;
			Udm::ParentAttr< ::SCAML::interfaces> interfaces_parent() const { return Udm::ParentAttr< ::SCAML::interfaces>(impl, meta_interfaces_parent); }

			Udm::ParentAttr< ::SCAML::MgaObject> parent() const { return Udm::ParentAttr< ::SCAML::MgaObject>(impl, Udm::NULLPARENTROLE); }
			static ::Uml::AssociationRole meta_srcinheritsfrom_end_;
			Udm::AssocEndAttr< ::SCAML::Interface> srcinheritsfrom_end() const { return Udm::AssocEndAttr< ::SCAML::Interface>(impl, meta_srcinheritsfrom_end_); }

			static ::Uml::AssociationRole meta_dstinheritsfrom_end_;
			Udm::AssocEndAttr< ::SCAML::Interface> dstinheritsfrom_end() const { return Udm::AssocEndAttr< ::SCAML::Interface>(impl, meta_dstinheritsfrom_end_); }

		};

		class  Interface :  public ::SCAML::MgaObject {
		public:
			static ::Uml::Class meta;

			Interface() { }
			Interface(Udm::ObjectImpl *impl) : ::SCAML::MgaObject(impl), UDM_OBJECT(impl) { }
			Interface(const Interface &master) : ::SCAML::MgaObject(master), UDM_OBJECT(master) { }
			static Interface Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static Interface Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			Interface CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			Interface CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::SCAML::Interface> Instances() { return Udm::InstantiatedAttr< ::SCAML::Interface>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::SCAML::Interface, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::SCAML::Interface, Pred>(impl);}

			Udm::DerivedAttr< ::SCAML::Interface> Derived() { return Udm::DerivedAttr< ::SCAML::Interface>(impl);}
			template <class Pred> Udm::DerivedAttr< ::SCAML::Interface, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::SCAML::Interface, Pred>(impl);}

			Udm::ArchetypeAttr< ::SCAML::Interface> Archetype() const { return Udm::ArchetypeAttr< ::SCAML::Interface>(impl);}

			static ::Uml::Attribute meta_repid;
			Udm::StringAttr repid() const { return Udm::StringAttr(impl, meta_repid); }

			static ::Uml::AssociationRole meta_referedbyusesPort;
			Udm::AssocAttr< ::SCAML::usesPort> referedbyusesPort() const { return Udm::AssocAttr< ::SCAML::usesPort>(impl, meta_referedbyusesPort); }
			template <class Pred> Udm::AssocAttr< ::SCAML::usesPort, Pred > referedbyusesPort_sorted(const Pred &) const { return Udm::AssocAttr< ::SCAML::usesPort, Pred>(impl, meta_referedbyusesPort); }

			static ::Uml::AssociationRole meta_referedbyprovidesPort;
			Udm::AssocAttr< ::SCAML::providesPort> referedbyprovidesPort() const { return Udm::AssocAttr< ::SCAML::providesPort>(impl, meta_referedbyprovidesPort); }
			template <class Pred> Udm::AssocAttr< ::SCAML::providesPort, Pred > referedbyprovidesPort_sorted(const Pred &) const { return Udm::AssocAttr< ::SCAML::providesPort, Pred>(impl, meta_referedbyprovidesPort); }

			static ::Uml::AssociationRole meta_referedbysupports;
			Udm::AssocAttr< ::SCAML::supports> referedbysupports() const { return Udm::AssocAttr< ::SCAML::supports>(impl, meta_referedbysupports); }
			template <class Pred> Udm::AssocAttr< ::SCAML::supports, Pred > referedbysupports_sorted(const Pred &) const { return Udm::AssocAttr< ::SCAML::supports, Pred>(impl, meta_referedbysupports); }

			static ::Uml::AssociationRole meta_dstinheritsfrom;
			static ::Uml::AssociationRole meta_dstinheritsfrom_rev;
			Udm::AClassAssocAttr< ::SCAML::inheritsfrom, ::SCAML::Interface> dstinheritsfrom() const { return Udm::AClassAssocAttr< ::SCAML::inheritsfrom, ::SCAML::Interface>(impl, meta_dstinheritsfrom, meta_dstinheritsfrom_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::SCAML::inheritsfrom, ::SCAML::Interface, Pred> dstinheritsfrom_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::SCAML::inheritsfrom, ::SCAML::Interface, Pred>(impl, meta_dstinheritsfrom, meta_dstinheritsfrom_rev); }

			static ::Uml::AssociationRole meta_srcinheritsfrom;
			static ::Uml::AssociationRole meta_srcinheritsfrom_rev;
			Udm::AClassAssocAttr< ::SCAML::inheritsfrom, ::SCAML::Interface> srcinheritsfrom() const { return Udm::AClassAssocAttr< ::SCAML::inheritsfrom, ::SCAML::Interface>(impl, meta_srcinheritsfrom, meta_srcinheritsfrom_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::SCAML::inheritsfrom, ::SCAML::Interface, Pred> srcinheritsfrom_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::SCAML::inheritsfrom, ::SCAML::Interface, Pred>(impl, meta_srcinheritsfrom, meta_srcinheritsfrom_rev); }

			static ::Uml::CompositionParentRole meta_interfaces_parent;
			Udm::ParentAttr< ::SCAML::interfaces> interfaces_parent() const { return Udm::ParentAttr< ::SCAML::interfaces>(impl, meta_interfaces_parent); }

			Udm::ParentAttr< ::SCAML::MgaObject> parent() const { return Udm::ParentAttr< ::SCAML::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  interfaces :  public ::SCAML::MgaObject {
		public:
			static ::Uml::Class meta;

			interfaces() { }
			interfaces(Udm::ObjectImpl *impl) : ::SCAML::MgaObject(impl), UDM_OBJECT(impl) { }
			interfaces(const interfaces &master) : ::SCAML::MgaObject(master), UDM_OBJECT(master) { }
			static interfaces Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static interfaces Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			interfaces CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			interfaces CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::SCAML::interfaces> Instances() { return Udm::InstantiatedAttr< ::SCAML::interfaces>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::SCAML::interfaces, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::SCAML::interfaces, Pred>(impl);}

			Udm::DerivedAttr< ::SCAML::interfaces> Derived() { return Udm::DerivedAttr< ::SCAML::interfaces>(impl);}
			template <class Pred> Udm::DerivedAttr< ::SCAML::interfaces, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::SCAML::interfaces, Pred>(impl);}

			Udm::ArchetypeAttr< ::SCAML::interfaces> Archetype() const { return Udm::ArchetypeAttr< ::SCAML::interfaces>(impl);}

			static ::Uml::CompositionChildRole meta_Interface_children;
			Udm::ChildrenAttr< ::SCAML::Interface> Interface_children() const { return Udm::ChildrenAttr< ::SCAML::Interface>(impl, meta_Interface_children); }
			template <class Pred> Udm::ChildrenAttr< ::SCAML::Interface, Pred> Interface_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::Interface, Pred>(impl, meta_Interface_children); }

			static ::Uml::CompositionChildRole meta_inheritsfrom_children;
			Udm::ChildrenAttr< ::SCAML::inheritsfrom> inheritsfrom_children() const { return Udm::ChildrenAttr< ::SCAML::inheritsfrom>(impl, meta_inheritsfrom_children); }
			template <class Pred> Udm::ChildrenAttr< ::SCAML::inheritsfrom, Pred> inheritsfrom_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::inheritsfrom, Pred>(impl, meta_inheritsfrom_children); }

			Udm::ChildrenAttr< ::SCAML::inheritsfrom> inheritsfrom_kind_children() const { return Udm::ChildrenAttr< ::SCAML::inheritsfrom>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::inheritsfrom, Pred> inheritsfrom_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::inheritsfrom, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::Interface> Interface_kind_children() const { return Udm::ChildrenAttr< ::SCAML::Interface>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::Interface, Pred> Interface_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::Interface, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr< ::SCAML::MgaObject>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

			static ::Uml::CompositionParentRole meta_InterfaceFolder_parent;
			Udm::ParentAttr< ::SCAML::InterfaceFolder> InterfaceFolder_parent() const { return Udm::ParentAttr< ::SCAML::InterfaceFolder>(impl, meta_InterfaceFolder_parent); }

			Udm::ParentAttr< ::SCAML::InterfaceFolder> parent() const { return Udm::ParentAttr< ::SCAML::InterfaceFolder>(impl, Udm::NULLPARENTROLE); }
		};

		class  componentfeatures :  virtual public ::SCAML::MgaObject {
		public:
			static ::Uml::Class meta;

			componentfeatures() { }
			componentfeatures(Udm::ObjectImpl *impl) : ::SCAML::MgaObject(impl), UDM_OBJECT(impl) { }
			componentfeatures(const componentfeatures &master) : ::SCAML::MgaObject(master), UDM_OBJECT(master) { }
			static componentfeatures Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static componentfeatures Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			componentfeatures CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			componentfeatures CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::SCAML::componentfeatures> Instances() { return Udm::InstantiatedAttr< ::SCAML::componentfeatures>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::SCAML::componentfeatures, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::SCAML::componentfeatures, Pred>(impl);}

			Udm::DerivedAttr< ::SCAML::componentfeatures> Derived() { return Udm::DerivedAttr< ::SCAML::componentfeatures>(impl);}
			template <class Pred> Udm::DerivedAttr< ::SCAML::componentfeatures, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::SCAML::componentfeatures, Pred>(impl);}

			Udm::ArchetypeAttr< ::SCAML::componentfeatures> Archetype() const { return Udm::ArchetypeAttr< ::SCAML::componentfeatures>(impl);}

			Udm::ParentAttr<Udm::Object> parent() const { return Udm::ParentAttr<Udm::Object>(impl, Udm::NULLPARENTROLE); }
		};

		class  supports :  public ::SCAML::componentfeatures {
		public:
			static ::Uml::Class meta;

			supports() { }
			supports(Udm::ObjectImpl *impl) : ::SCAML::componentfeatures(impl), ::SCAML::MgaObject(impl), UDM_OBJECT(impl) { }
			supports(const supports &master) : ::SCAML::componentfeatures(master), ::SCAML::MgaObject(master), UDM_OBJECT(master) { }
			static supports Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static supports Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			supports CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			supports CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::SCAML::supports> Instances() { return Udm::InstantiatedAttr< ::SCAML::supports>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::SCAML::supports, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::SCAML::supports, Pred>(impl);}

			Udm::DerivedAttr< ::SCAML::supports> Derived() { return Udm::DerivedAttr< ::SCAML::supports>(impl);}
			template <class Pred> Udm::DerivedAttr< ::SCAML::supports, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::SCAML::supports, Pred>(impl);}

			Udm::ArchetypeAttr< ::SCAML::supports> Archetype() const { return Udm::ArchetypeAttr< ::SCAML::supports>(impl);}

			static ::Uml::Attribute meta_supportsname;
			Udm::StringAttr supportsname() const { return Udm::StringAttr(impl, meta_supportsname); }

			static ::Uml::AssociationRole meta_referedbycomponentsupportedinterface;
			Udm::AssocAttr< ::SCAML::componentsupportedinterface> referedbycomponentsupportedinterface() const { return Udm::AssocAttr< ::SCAML::componentsupportedinterface>(impl, meta_referedbycomponentsupportedinterface); }
			template <class Pred> Udm::AssocAttr< ::SCAML::componentsupportedinterface, Pred > referedbycomponentsupportedinterface_sorted(const Pred &) const { return Udm::AssocAttr< ::SCAML::componentsupportedinterface, Pred>(impl, meta_referedbycomponentsupportedinterface); }

			static ::Uml::AssociationRole meta_ref;
			Udm::PointerAttr< ::SCAML::Interface> ref() const { return Udm::PointerAttr< ::SCAML::Interface>(impl, meta_ref); }

			static ::Uml::CompositionParentRole meta_SoftwareComponent_parent;
			Udm::ParentAttr< ::SCAML::SoftwareComponent> SoftwareComponent_parent() const { return Udm::ParentAttr< ::SCAML::SoftwareComponent>(impl, meta_SoftwareComponent_parent); }

			Udm::ParentAttr< ::SCAML::componentplacement_Members_Base> componentplacement_Members_Base_parent() const { return Udm::ParentAttr< ::SCAML::componentplacement_Members_Base>(impl, Udm::NULLPARENTROLE); }
			Udm::ParentAttr< ::SCAML::MgaObject> MgaObject_parent() const { return Udm::ParentAttr< ::SCAML::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  devicepkgref :  public ::SCAML::MgaObject {
		public:
			static ::Uml::Class meta;

			devicepkgref() { }
			devicepkgref(Udm::ObjectImpl *impl) : ::SCAML::MgaObject(impl), UDM_OBJECT(impl) { }
			devicepkgref(const devicepkgref &master) : ::SCAML::MgaObject(master), UDM_OBJECT(master) { }
			static devicepkgref Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static devicepkgref Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			devicepkgref CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			devicepkgref CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::SCAML::devicepkgref> Instances() { return Udm::InstantiatedAttr< ::SCAML::devicepkgref>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::SCAML::devicepkgref, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::SCAML::devicepkgref, Pred>(impl);}

			Udm::DerivedAttr< ::SCAML::devicepkgref> Derived() { return Udm::DerivedAttr< ::SCAML::devicepkgref>(impl);}
			template <class Pred> Udm::DerivedAttr< ::SCAML::devicepkgref, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::SCAML::devicepkgref, Pred>(impl);}

			Udm::ArchetypeAttr< ::SCAML::devicepkgref> Archetype() const { return Udm::ArchetypeAttr< ::SCAML::devicepkgref>(impl);}

			static ::Uml::AssociationRole meta_ref;
			Udm::PointerAttr< ::SCAML::DevicePkg> ref() const { return Udm::PointerAttr< ::SCAML::DevicePkg>(impl, meta_ref); }

			static ::Uml::CompositionParentRole meta_hwdeviceregistration_parent;
			Udm::ParentAttr< ::SCAML::hwdeviceregistration> hwdeviceregistration_parent() const { return Udm::ParentAttr< ::SCAML::hwdeviceregistration>(impl, meta_hwdeviceregistration_parent); }

			Udm::ParentAttr< ::SCAML::MgaObject> parent() const { return Udm::ParentAttr< ::SCAML::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  childhwdevice :  public ::SCAML::MgaObject {
		public:
			static ::Uml::Class meta;

			childhwdevice() { }
			childhwdevice(Udm::ObjectImpl *impl) : ::SCAML::MgaObject(impl), UDM_OBJECT(impl) { }
			childhwdevice(const childhwdevice &master) : ::SCAML::MgaObject(master), UDM_OBJECT(master) { }
			static childhwdevice Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static childhwdevice Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			childhwdevice CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			childhwdevice CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::SCAML::childhwdevice> Instances() { return Udm::InstantiatedAttr< ::SCAML::childhwdevice>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::SCAML::childhwdevice, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::SCAML::childhwdevice, Pred>(impl);}

			Udm::DerivedAttr< ::SCAML::childhwdevice> Derived() { return Udm::DerivedAttr< ::SCAML::childhwdevice>(impl);}
			template <class Pred> Udm::DerivedAttr< ::SCAML::childhwdevice, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::SCAML::childhwdevice, Pred>(impl);}

			Udm::ArchetypeAttr< ::SCAML::childhwdevice> Archetype() const { return Udm::ArchetypeAttr< ::SCAML::childhwdevice>(impl);}

			static ::Uml::CompositionParentRole meta_hwdeviceregistration_parent;
			Udm::ParentAttr< ::SCAML::hwdeviceregistration> hwdeviceregistration_parent() const { return Udm::ParentAttr< ::SCAML::hwdeviceregistration>(impl, meta_hwdeviceregistration_parent); }

			Udm::ParentAttr< ::SCAML::MgaObject> parent() const { return Udm::ParentAttr< ::SCAML::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  hwdeviceregistration :  public ::SCAML::childhwdevice {
		public:
			static ::Uml::Class meta;

			hwdeviceregistration() { }
			hwdeviceregistration(Udm::ObjectImpl *impl) : ::SCAML::childhwdevice(impl), UDM_OBJECT(impl) { }
			hwdeviceregistration(const hwdeviceregistration &master) : ::SCAML::childhwdevice(master), UDM_OBJECT(master) { }
			static hwdeviceregistration Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static hwdeviceregistration Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			hwdeviceregistration CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			hwdeviceregistration CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::SCAML::hwdeviceregistration> Instances() { return Udm::InstantiatedAttr< ::SCAML::hwdeviceregistration>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::SCAML::hwdeviceregistration, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::SCAML::hwdeviceregistration, Pred>(impl);}

			Udm::DerivedAttr< ::SCAML::hwdeviceregistration> Derived() { return Udm::DerivedAttr< ::SCAML::hwdeviceregistration>(impl);}
			template <class Pred> Udm::DerivedAttr< ::SCAML::hwdeviceregistration, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::SCAML::hwdeviceregistration, Pred>(impl);}

			Udm::ArchetypeAttr< ::SCAML::hwdeviceregistration> Archetype() const { return Udm::ArchetypeAttr< ::SCAML::hwdeviceregistration>(impl);}

			static ::Uml::Attribute meta_modelnumber;
			Udm::StringAttr modelnumber() const { return Udm::StringAttr(impl, meta_modelnumber); }

			static ::Uml::Attribute meta_id;
			Udm::StringAttr id() const { return Udm::StringAttr(impl, meta_id); }

			static ::Uml::Attribute meta_version;
			Udm::StringAttr version() const { return Udm::StringAttr(impl, meta_version); }

			static ::Uml::Attribute meta_description;
			Udm::StringAttr description() const { return Udm::StringAttr(impl, meta_description); }

			static ::Uml::Attribute meta_deviceclass;
			Udm::StringAttr deviceclass() const { return Udm::StringAttr(impl, meta_deviceclass); }

			static ::Uml::Attribute meta_manufacturer;
			Udm::StringAttr manufacturer() const { return Udm::StringAttr(impl, meta_manufacturer); }

			static ::Uml::CompositionChildRole meta_devicepkgref_children;
			Udm::ChildrenAttr< ::SCAML::devicepkgref> devicepkgref_children() const { return Udm::ChildrenAttr< ::SCAML::devicepkgref>(impl, meta_devicepkgref_children); }
			template <class Pred> Udm::ChildrenAttr< ::SCAML::devicepkgref, Pred> devicepkgref_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::devicepkgref, Pred>(impl, meta_devicepkgref_children); }

			static ::Uml::CompositionChildRole meta_propertyfile_children;
			Udm::ChildrenAttr< ::SCAML::propertyfile> propertyfile_children() const { return Udm::ChildrenAttr< ::SCAML::propertyfile>(impl, meta_propertyfile_children); }
			template <class Pred> Udm::ChildrenAttr< ::SCAML::propertyfile, Pred> propertyfile_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::propertyfile, Pred>(impl, meta_propertyfile_children); }

			static ::Uml::CompositionChildRole meta_childhwdevice_children;
			Udm::ChildrenAttr< ::SCAML::childhwdevice> childhwdevice_children() const { return Udm::ChildrenAttr< ::SCAML::childhwdevice>(impl, meta_childhwdevice_children); }
			template <class Pred> Udm::ChildrenAttr< ::SCAML::childhwdevice, Pred> childhwdevice_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::childhwdevice, Pred>(impl, meta_childhwdevice_children); }

			Udm::ChildrenAttr< ::SCAML::devicepkgref> devicepkgref_kind_children() const { return Udm::ChildrenAttr< ::SCAML::devicepkgref>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::devicepkgref, Pred> devicepkgref_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::devicepkgref, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::hwdeviceregistration> hwdeviceregistration_kind_children() const { return Udm::ChildrenAttr< ::SCAML::hwdeviceregistration>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::hwdeviceregistration, Pred> hwdeviceregistration_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::hwdeviceregistration, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::childhwdevice> childhwdevice_kind_children() const { return Udm::ChildrenAttr< ::SCAML::childhwdevice>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::childhwdevice, Pred> childhwdevice_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::childhwdevice, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::propertyfile> propertyfile_kind_children() const { return Udm::ChildrenAttr< ::SCAML::propertyfile>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::propertyfile, Pred> propertyfile_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::propertyfile, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr< ::SCAML::MgaObject>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

			static ::Uml::CompositionParentRole meta_DevicePkg_parent;
			Udm::ParentAttr< ::SCAML::DevicePkg> DevicePkg_parent() const { return Udm::ParentAttr< ::SCAML::DevicePkg>(impl, meta_DevicePkg_parent); }

			Udm::ParentAttr< ::SCAML::MgaObject> parent() const { return Udm::ParentAttr< ::SCAML::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  structvalue :  public ::SCAML::MgaObject {
		public:
			static ::Uml::Class meta;

			structvalue() { }
			structvalue(Udm::ObjectImpl *impl) : ::SCAML::MgaObject(impl), UDM_OBJECT(impl) { }
			structvalue(const structvalue &master) : ::SCAML::MgaObject(master), UDM_OBJECT(master) { }
			static structvalue Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static structvalue Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			structvalue CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			structvalue CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::SCAML::structvalue> Instances() { return Udm::InstantiatedAttr< ::SCAML::structvalue>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::SCAML::structvalue, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::SCAML::structvalue, Pred>(impl);}

			Udm::DerivedAttr< ::SCAML::structvalue> Derived() { return Udm::DerivedAttr< ::SCAML::structvalue>(impl);}
			template <class Pred> Udm::DerivedAttr< ::SCAML::structvalue, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::SCAML::structvalue, Pred>(impl);}

			Udm::ArchetypeAttr< ::SCAML::structvalue> Archetype() const { return Udm::ArchetypeAttr< ::SCAML::structvalue>(impl);}

			static ::Uml::CompositionChildRole meta_simpleref_children;
			Udm::ChildrenAttr< ::SCAML::simpleref> simpleref_children() const { return Udm::ChildrenAttr< ::SCAML::simpleref>(impl, meta_simpleref_children); }
			template <class Pred> Udm::ChildrenAttr< ::SCAML::simpleref, Pred> simpleref_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::simpleref, Pred>(impl, meta_simpleref_children); }

			Udm::ChildrenAttr< ::SCAML::simpleref> simpleref_kind_children() const { return Udm::ChildrenAttr< ::SCAML::simpleref>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::simpleref, Pred> simpleref_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::simpleref, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr< ::SCAML::MgaObject>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

			static ::Uml::CompositionParentRole meta_structsequencePropertyDescriptor_parent;
			Udm::ParentAttr< ::SCAML::structsequencePropertyDescriptor> structsequencePropertyDescriptor_parent() const { return Udm::ParentAttr< ::SCAML::structsequencePropertyDescriptor>(impl, meta_structsequencePropertyDescriptor_parent); }

			Udm::ParentAttr< ::SCAML::MgaObject> parent() const { return Udm::ParentAttr< ::SCAML::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  simpleref :  public ::SCAML::MgaObject {
		public:
			static ::Uml::Class meta;

			simpleref() { }
			simpleref(Udm::ObjectImpl *impl) : ::SCAML::MgaObject(impl), UDM_OBJECT(impl) { }
			simpleref(const simpleref &master) : ::SCAML::MgaObject(master), UDM_OBJECT(master) { }
			static simpleref Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static simpleref Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			simpleref CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			simpleref CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::SCAML::simpleref> Instances() { return Udm::InstantiatedAttr< ::SCAML::simpleref>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::SCAML::simpleref, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::SCAML::simpleref, Pred>(impl);}

			Udm::DerivedAttr< ::SCAML::simpleref> Derived() { return Udm::DerivedAttr< ::SCAML::simpleref>(impl);}
			template <class Pred> Udm::DerivedAttr< ::SCAML::simpleref, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::SCAML::simpleref, Pred>(impl);}

			Udm::ArchetypeAttr< ::SCAML::simpleref> Archetype() const { return Udm::ArchetypeAttr< ::SCAML::simpleref>(impl);}

			static ::Uml::Attribute meta_value;
			Udm::StringAttr value() const { return Udm::StringAttr(impl, meta_value); }

			static ::Uml::AssociationRole meta_ref;
			Udm::PointerAttr< ::SCAML::simplePropertyDescriptor> ref() const { return Udm::PointerAttr< ::SCAML::simplePropertyDescriptor>(impl, meta_ref); }

			static ::Uml::CompositionParentRole meta_structvalue_parent;
			Udm::ParentAttr< ::SCAML::structvalue> structvalue_parent() const { return Udm::ParentAttr< ::SCAML::structvalue>(impl, meta_structvalue_parent); }

			Udm::ParentAttr< ::SCAML::MgaObject> parent() const { return Udm::ParentAttr< ::SCAML::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  structref :  public ::SCAML::MgaObject {
		public:
			static ::Uml::Class meta;

			structref() { }
			structref(Udm::ObjectImpl *impl) : ::SCAML::MgaObject(impl), UDM_OBJECT(impl) { }
			structref(const structref &master) : ::SCAML::MgaObject(master), UDM_OBJECT(master) { }
			static structref Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static structref Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			structref CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			structref CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::SCAML::structref> Instances() { return Udm::InstantiatedAttr< ::SCAML::structref>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::SCAML::structref, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::SCAML::structref, Pred>(impl);}

			Udm::DerivedAttr< ::SCAML::structref> Derived() { return Udm::DerivedAttr< ::SCAML::structref>(impl);}
			template <class Pred> Udm::DerivedAttr< ::SCAML::structref, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::SCAML::structref, Pred>(impl);}

			Udm::ArchetypeAttr< ::SCAML::structref> Archetype() const { return Udm::ArchetypeAttr< ::SCAML::structref>(impl);}

			static ::Uml::AssociationRole meta_ref;
			Udm::PointerAttr< ::SCAML::structPropertyDescriptor> ref() const { return Udm::PointerAttr< ::SCAML::structPropertyDescriptor>(impl, meta_ref); }

			static ::Uml::CompositionParentRole meta_structref_structsequencePropertyDescriptor_parent;
			Udm::ParentAttr< ::SCAML::structsequencePropertyDescriptor> structref_structsequencePropertyDescriptor_parent() const { return Udm::ParentAttr< ::SCAML::structsequencePropertyDescriptor>(impl, meta_structref_structsequencePropertyDescriptor_parent); }

			Udm::ParentAttr< ::SCAML::MgaObject> parent() const { return Udm::ParentAttr< ::SCAML::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  kind :  public ::SCAML::MgaObject {
		public:
			static ::Uml::Class meta;

			kind() { }
			kind(Udm::ObjectImpl *impl) : ::SCAML::MgaObject(impl), UDM_OBJECT(impl) { }
			kind(const kind &master) : ::SCAML::MgaObject(master), UDM_OBJECT(master) { }
			static kind Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static kind Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			kind CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			kind CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::SCAML::kind> Instances() { return Udm::InstantiatedAttr< ::SCAML::kind>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::SCAML::kind, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::SCAML::kind, Pred>(impl);}

			Udm::DerivedAttr< ::SCAML::kind> Derived() { return Udm::DerivedAttr< ::SCAML::kind>(impl);}
			template <class Pred> Udm::DerivedAttr< ::SCAML::kind, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::SCAML::kind, Pred>(impl);}

			Udm::ArchetypeAttr< ::SCAML::kind> Archetype() const { return Udm::ArchetypeAttr< ::SCAML::kind>(impl);}

			static ::Uml::Attribute meta_kindtype;
			Udm::StringAttr kindtype() const { return Udm::StringAttr(impl, meta_kindtype); }

			static ::Uml::CompositionParentRole meta_simpleBase_parent;
			Udm::ParentAttr< ::SCAML::simpleBase> simpleBase_parent() const { return Udm::ParentAttr< ::SCAML::simpleBase>(impl, meta_simpleBase_parent); }

			Udm::ParentAttr< ::SCAML::MgaObject> parent() const { return Udm::ParentAttr< ::SCAML::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  enumeration :  public ::SCAML::MgaObject {
		public:
			static ::Uml::Class meta;

			enumeration() { }
			enumeration(Udm::ObjectImpl *impl) : ::SCAML::MgaObject(impl), UDM_OBJECT(impl) { }
			enumeration(const enumeration &master) : ::SCAML::MgaObject(master), UDM_OBJECT(master) { }
			static enumeration Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static enumeration Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			enumeration CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			enumeration CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::SCAML::enumeration> Instances() { return Udm::InstantiatedAttr< ::SCAML::enumeration>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::SCAML::enumeration, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::SCAML::enumeration, Pred>(impl);}

			Udm::DerivedAttr< ::SCAML::enumeration> Derived() { return Udm::DerivedAttr< ::SCAML::enumeration>(impl);}
			template <class Pred> Udm::DerivedAttr< ::SCAML::enumeration, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::SCAML::enumeration, Pred>(impl);}

			Udm::ArchetypeAttr< ::SCAML::enumeration> Archetype() const { return Udm::ArchetypeAttr< ::SCAML::enumeration>(impl);}

			static ::Uml::Attribute meta_value;
			Udm::StringAttr value() const { return Udm::StringAttr(impl, meta_value); }

			static ::Uml::Attribute meta_label;
			Udm::StringAttr label() const { return Udm::StringAttr(impl, meta_label); }

			static ::Uml::CompositionParentRole meta_enumerations_simplePropertyDescriptor_parent;
			Udm::ParentAttr< ::SCAML::simplePropertyDescriptor> enumerations_simplePropertyDescriptor_parent() const { return Udm::ParentAttr< ::SCAML::simplePropertyDescriptor>(impl, meta_enumerations_simplePropertyDescriptor_parent); }

			Udm::ParentAttr< ::SCAML::MgaObject> parent() const { return Udm::ParentAttr< ::SCAML::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  devicepkgfile :  public ::SCAML::componentplacement_Members_Base, public ::SCAML::MgaObject {
		public:
			static ::Uml::Class meta;

			devicepkgfile() { }
			devicepkgfile(Udm::ObjectImpl *impl) : ::SCAML::componentplacement_Members_Base(impl),::SCAML::MgaObject(impl), UDM_OBJECT(impl) { }
			devicepkgfile(const devicepkgfile &master) : ::SCAML::componentplacement_Members_Base(master),::SCAML::MgaObject(master), UDM_OBJECT(master) { }
			static devicepkgfile Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static devicepkgfile Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			devicepkgfile CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			devicepkgfile CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::SCAML::devicepkgfile> Instances() { return Udm::InstantiatedAttr< ::SCAML::devicepkgfile>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::SCAML::devicepkgfile, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::SCAML::devicepkgfile, Pred>(impl);}

			Udm::DerivedAttr< ::SCAML::devicepkgfile> Derived() { return Udm::DerivedAttr< ::SCAML::devicepkgfile>(impl);}
			template <class Pred> Udm::DerivedAttr< ::SCAML::devicepkgfile, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::SCAML::devicepkgfile, Pred>(impl);}

			Udm::ArchetypeAttr< ::SCAML::devicepkgfile> Archetype() const { return Udm::ArchetypeAttr< ::SCAML::devicepkgfile>(impl);}

			static ::Uml::Attribute meta_type;
			Udm::StringAttr type() const { return Udm::StringAttr(impl, meta_type); }

			static ::Uml::AssociationRole meta_ref;
			Udm::PointerAttr< ::SCAML::DevicePkg> ref() const { return Udm::PointerAttr< ::SCAML::DevicePkg>(impl, meta_ref); }

			static ::Uml::CompositionParentRole meta_DeviceConfiguration_parent;
			Udm::ParentAttr< ::SCAML::DeviceConfiguration> DeviceConfiguration_parent() const { return Udm::ParentAttr< ::SCAML::DeviceConfiguration>(impl, meta_DeviceConfiguration_parent); }

			Udm::ParentAttr< ::SCAML::MgaObject> parent() const { return Udm::ParentAttr< ::SCAML::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  filesystemname :  public ::SCAML::MgaObject {
		public:
			static ::Uml::Class meta;

			filesystemname() { }
			filesystemname(Udm::ObjectImpl *impl) : ::SCAML::MgaObject(impl), UDM_OBJECT(impl) { }
			filesystemname(const filesystemname &master) : ::SCAML::MgaObject(master), UDM_OBJECT(master) { }
			static filesystemname Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static filesystemname Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			filesystemname CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			filesystemname CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::SCAML::filesystemname> Instances() { return Udm::InstantiatedAttr< ::SCAML::filesystemname>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::SCAML::filesystemname, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::SCAML::filesystemname, Pred>(impl);}

			Udm::DerivedAttr< ::SCAML::filesystemname> Derived() { return Udm::DerivedAttr< ::SCAML::filesystemname>(impl);}
			template <class Pred> Udm::DerivedAttr< ::SCAML::filesystemname, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::SCAML::filesystemname, Pred>(impl);}

			Udm::ArchetypeAttr< ::SCAML::filesystemname> Archetype() const { return Udm::ArchetypeAttr< ::SCAML::filesystemname>(impl);}

			static ::Uml::Attribute meta_deviceid;
			Udm::StringAttr deviceid() const { return Udm::StringAttr(impl, meta_deviceid); }

			static ::Uml::Attribute meta_mountname;
			Udm::StringAttr mountname() const { return Udm::StringAttr(impl, meta_mountname); }

			static ::Uml::CompositionParentRole meta_DeviceConfiguration_parent;
			Udm::ParentAttr< ::SCAML::DeviceConfiguration> DeviceConfiguration_parent() const { return Udm::ParentAttr< ::SCAML::DeviceConfiguration>(impl, meta_DeviceConfiguration_parent); }

			Udm::ParentAttr< ::SCAML::MgaObject> parent() const { return Udm::ParentAttr< ::SCAML::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  componentfile :  public ::SCAML::componentplacement_Members_Base, public ::SCAML::MgaObject {
		public:
			static ::Uml::Class meta;

			componentfile() { }
			componentfile(Udm::ObjectImpl *impl) : ::SCAML::componentplacement_Members_Base(impl),::SCAML::MgaObject(impl), UDM_OBJECT(impl) { }
			componentfile(const componentfile &master) : ::SCAML::componentplacement_Members_Base(master),::SCAML::MgaObject(master), UDM_OBJECT(master) { }
			static componentfile Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static componentfile Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			componentfile CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			componentfile CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::SCAML::componentfile> Instances() { return Udm::InstantiatedAttr< ::SCAML::componentfile>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::SCAML::componentfile, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::SCAML::componentfile, Pred>(impl);}

			Udm::DerivedAttr< ::SCAML::componentfile> Derived() { return Udm::DerivedAttr< ::SCAML::componentfile>(impl);}
			template <class Pred> Udm::DerivedAttr< ::SCAML::componentfile, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::SCAML::componentfile, Pred>(impl);}

			Udm::ArchetypeAttr< ::SCAML::componentfile> Archetype() const { return Udm::ArchetypeAttr< ::SCAML::componentfile>(impl);}

			static ::Uml::AssociationRole meta_dstcomponentplacementConn;
			static ::Uml::AssociationRole meta_dstcomponentplacementConn_rev;
			Udm::AClassAssocAttr< ::SCAML::componentplacementConn, ::SCAML::SoftwareComponent> dstcomponentplacementConn() const { return Udm::AClassAssocAttr< ::SCAML::componentplacementConn, ::SCAML::SoftwareComponent>(impl, meta_dstcomponentplacementConn, meta_dstcomponentplacementConn_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::SCAML::componentplacementConn, ::SCAML::SoftwareComponent, Pred> dstcomponentplacementConn_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::SCAML::componentplacementConn, ::SCAML::SoftwareComponent, Pred>(impl, meta_dstcomponentplacementConn, meta_dstcomponentplacementConn_rev); }

			static ::Uml::AssociationRole meta_ref;
			Udm::PointerAttr< ::SCAML::SoftPkg> ref() const { return Udm::PointerAttr< ::SCAML::SoftPkg>(impl, meta_ref); }

			static ::Uml::CompositionParentRole meta_DeviceConfiguration_parent;
			Udm::ParentAttr< ::SCAML::DeviceConfiguration> DeviceConfiguration_parent() const { return Udm::ParentAttr< ::SCAML::DeviceConfiguration>(impl, meta_DeviceConfiguration_parent); }

			static ::Uml::CompositionParentRole meta_SoftwareAssembly_parent;
			Udm::ParentAttr< ::SCAML::SoftwareAssembly> SoftwareAssembly_parent() const { return Udm::ParentAttr< ::SCAML::SoftwareAssembly>(impl, meta_SoftwareAssembly_parent); }

			Udm::ParentAttr< ::SCAML::MgaObject> parent() const { return Udm::ParentAttr< ::SCAML::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  compositepartofdevice :  public ::SCAML::componentplacement_Members_Base, public ::SCAML::MgaObject {
		public:
			static ::Uml::Class meta;

			compositepartofdevice() { }
			compositepartofdevice(Udm::ObjectImpl *impl) : ::SCAML::componentplacement_Members_Base(impl),::SCAML::MgaObject(impl), UDM_OBJECT(impl) { }
			compositepartofdevice(const compositepartofdevice &master) : ::SCAML::componentplacement_Members_Base(master),::SCAML::MgaObject(master), UDM_OBJECT(master) { }
			static compositepartofdevice Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static compositepartofdevice Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			compositepartofdevice CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			compositepartofdevice CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::SCAML::compositepartofdevice> Instances() { return Udm::InstantiatedAttr< ::SCAML::compositepartofdevice>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::SCAML::compositepartofdevice, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::SCAML::compositepartofdevice, Pred>(impl);}

			Udm::DerivedAttr< ::SCAML::compositepartofdevice> Derived() { return Udm::DerivedAttr< ::SCAML::compositepartofdevice>(impl);}
			template <class Pred> Udm::DerivedAttr< ::SCAML::compositepartofdevice, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::SCAML::compositepartofdevice, Pred>(impl);}

			Udm::ArchetypeAttr< ::SCAML::compositepartofdevice> Archetype() const { return Udm::ArchetypeAttr< ::SCAML::compositepartofdevice>(impl);}

			static ::Uml::AssociationRole meta_ref;
			Udm::PointerAttr< ::SCAML::SoftwareComponent> ref() const { return Udm::PointerAttr< ::SCAML::SoftwareComponent>(impl, meta_ref); }

			static ::Uml::CompositionParentRole meta_DeviceConfiguration_parent;
			Udm::ParentAttr< ::SCAML::DeviceConfiguration> DeviceConfiguration_parent() const { return Udm::ParentAttr< ::SCAML::DeviceConfiguration>(impl, meta_DeviceConfiguration_parent); }

			Udm::ParentAttr< ::SCAML::MgaObject> parent() const { return Udm::ParentAttr< ::SCAML::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  deployondevice :  public ::SCAML::componentplacement_Members_Base, public ::SCAML::MgaObject {
		public:
			static ::Uml::Class meta;

			deployondevice() { }
			deployondevice(Udm::ObjectImpl *impl) : ::SCAML::componentplacement_Members_Base(impl),::SCAML::MgaObject(impl), UDM_OBJECT(impl) { }
			deployondevice(const deployondevice &master) : ::SCAML::componentplacement_Members_Base(master),::SCAML::MgaObject(master), UDM_OBJECT(master) { }
			static deployondevice Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static deployondevice Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			deployondevice CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			deployondevice CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::SCAML::deployondevice> Instances() { return Udm::InstantiatedAttr< ::SCAML::deployondevice>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::SCAML::deployondevice, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::SCAML::deployondevice, Pred>(impl);}

			Udm::DerivedAttr< ::SCAML::deployondevice> Derived() { return Udm::DerivedAttr< ::SCAML::deployondevice>(impl);}
			template <class Pred> Udm::DerivedAttr< ::SCAML::deployondevice, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::SCAML::deployondevice, Pred>(impl);}

			Udm::ArchetypeAttr< ::SCAML::deployondevice> Archetype() const { return Udm::ArchetypeAttr< ::SCAML::deployondevice>(impl);}

			static ::Uml::AssociationRole meta_ref;
			Udm::PointerAttr< ::SCAML::SoftwareComponent> ref() const { return Udm::PointerAttr< ::SCAML::SoftwareComponent>(impl, meta_ref); }

			static ::Uml::CompositionParentRole meta_DeviceConfiguration_parent;
			Udm::ParentAttr< ::SCAML::DeviceConfiguration> DeviceConfiguration_parent() const { return Udm::ParentAttr< ::SCAML::DeviceConfiguration>(impl, meta_DeviceConfiguration_parent); }

			Udm::ParentAttr< ::SCAML::MgaObject> parent() const { return Udm::ParentAttr< ::SCAML::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  componentplacement :  public ::SCAML::MgaObject {
		public:
			static ::Uml::Class meta;

			componentplacement() { }
			componentplacement(Udm::ObjectImpl *impl) : ::SCAML::MgaObject(impl), UDM_OBJECT(impl) { }
			componentplacement(const componentplacement &master) : ::SCAML::MgaObject(master), UDM_OBJECT(master) { }
			static componentplacement Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static componentplacement Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			componentplacement CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			componentplacement CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::SCAML::componentplacement> Instances() { return Udm::InstantiatedAttr< ::SCAML::componentplacement>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::SCAML::componentplacement, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::SCAML::componentplacement, Pred>(impl);}

			Udm::DerivedAttr< ::SCAML::componentplacement> Derived() { return Udm::DerivedAttr< ::SCAML::componentplacement>(impl);}
			template <class Pred> Udm::DerivedAttr< ::SCAML::componentplacement, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::SCAML::componentplacement, Pred>(impl);}

			Udm::ArchetypeAttr< ::SCAML::componentplacement> Archetype() const { return Udm::ArchetypeAttr< ::SCAML::componentplacement>(impl);}

			static ::Uml::AssociationRole meta_members;
			Udm::AssocAttr< ::SCAML::componentplacement_Members_Base> members() const { return Udm::AssocAttr< ::SCAML::componentplacement_Members_Base>(impl, meta_members); }
			template <class Pred> Udm::AssocAttr< ::SCAML::componentplacement_Members_Base, Pred > members_sorted(const Pred &) const { return Udm::AssocAttr< ::SCAML::componentplacement_Members_Base, Pred>(impl, meta_members); }

			static ::Uml::CompositionParentRole meta_DeviceConfiguration_parent;
			Udm::ParentAttr< ::SCAML::DeviceConfiguration> DeviceConfiguration_parent() const { return Udm::ParentAttr< ::SCAML::DeviceConfiguration>(impl, meta_DeviceConfiguration_parent); }

			Udm::ParentAttr< ::SCAML::MgaObject> parent() const { return Udm::ParentAttr< ::SCAML::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  devconfigcomponentfile :  public ::SCAML::componentplacement_Members_Base, public ::SCAML::MgaObject {
		public:
			static ::Uml::Class meta;

			devconfigcomponentfile() { }
			devconfigcomponentfile(Udm::ObjectImpl *impl) : ::SCAML::componentplacement_Members_Base(impl),::SCAML::MgaObject(impl), UDM_OBJECT(impl) { }
			devconfigcomponentfile(const devconfigcomponentfile &master) : ::SCAML::componentplacement_Members_Base(master),::SCAML::MgaObject(master), UDM_OBJECT(master) { }
			static devconfigcomponentfile Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static devconfigcomponentfile Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			devconfigcomponentfile CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			devconfigcomponentfile CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::SCAML::devconfigcomponentfile> Instances() { return Udm::InstantiatedAttr< ::SCAML::devconfigcomponentfile>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::SCAML::devconfigcomponentfile, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::SCAML::devconfigcomponentfile, Pred>(impl);}

			Udm::DerivedAttr< ::SCAML::devconfigcomponentfile> Derived() { return Udm::DerivedAttr< ::SCAML::devconfigcomponentfile>(impl);}
			template <class Pred> Udm::DerivedAttr< ::SCAML::devconfigcomponentfile, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::SCAML::devconfigcomponentfile, Pred>(impl);}

			Udm::ArchetypeAttr< ::SCAML::devconfigcomponentfile> Archetype() const { return Udm::ArchetypeAttr< ::SCAML::devconfigcomponentfile>(impl);}

			static ::Uml::AssociationRole meta_ref;
			Udm::PointerAttr< ::SCAML::SoftPkg> ref() const { return Udm::PointerAttr< ::SCAML::SoftPkg>(impl, meta_ref); }

			static ::Uml::CompositionParentRole meta_DeviceConfiguration_parent;
			Udm::ParentAttr< ::SCAML::DeviceConfiguration> DeviceConfiguration_parent() const { return Udm::ParentAttr< ::SCAML::DeviceConfiguration>(impl, meta_DeviceConfiguration_parent); }

			Udm::ParentAttr< ::SCAML::MgaObject> parent() const { return Udm::ParentAttr< ::SCAML::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  devicemanagersoftpkg :  public ::SCAML::MgaObject {
		public:
			static ::Uml::Class meta;

			devicemanagersoftpkg() { }
			devicemanagersoftpkg(Udm::ObjectImpl *impl) : ::SCAML::MgaObject(impl), UDM_OBJECT(impl) { }
			devicemanagersoftpkg(const devicemanagersoftpkg &master) : ::SCAML::MgaObject(master), UDM_OBJECT(master) { }
			static devicemanagersoftpkg Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static devicemanagersoftpkg Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			devicemanagersoftpkg CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			devicemanagersoftpkg CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::SCAML::devicemanagersoftpkg> Instances() { return Udm::InstantiatedAttr< ::SCAML::devicemanagersoftpkg>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::SCAML::devicemanagersoftpkg, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::SCAML::devicemanagersoftpkg, Pred>(impl);}

			Udm::DerivedAttr< ::SCAML::devicemanagersoftpkg> Derived() { return Udm::DerivedAttr< ::SCAML::devicemanagersoftpkg>(impl);}
			template <class Pred> Udm::DerivedAttr< ::SCAML::devicemanagersoftpkg, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::SCAML::devicemanagersoftpkg, Pred>(impl);}

			Udm::ArchetypeAttr< ::SCAML::devicemanagersoftpkg> Archetype() const { return Udm::ArchetypeAttr< ::SCAML::devicemanagersoftpkg>(impl);}

			static ::Uml::AssociationRole meta_ref;
			Udm::PointerAttr< ::SCAML::SoftPkg> ref() const { return Udm::PointerAttr< ::SCAML::SoftPkg>(impl, meta_ref); }

			static ::Uml::CompositionParentRole meta_DeviceConfiguration_parent;
			Udm::ParentAttr< ::SCAML::DeviceConfiguration> DeviceConfiguration_parent() const { return Udm::ParentAttr< ::SCAML::DeviceConfiguration>(impl, meta_DeviceConfiguration_parent); }

			Udm::ParentAttr< ::SCAML::MgaObject> parent() const { return Udm::ParentAttr< ::SCAML::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  usesImpl :  public ::SCAML::MgaObject {
		public:
			static ::Uml::Class meta;

			usesImpl() { }
			usesImpl(Udm::ObjectImpl *impl) : ::SCAML::MgaObject(impl), UDM_OBJECT(impl) { }
			usesImpl(const usesImpl &master) : ::SCAML::MgaObject(master), UDM_OBJECT(master) { }
			static usesImpl Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static usesImpl Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			usesImpl CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			usesImpl CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::SCAML::usesImpl> Instances() { return Udm::InstantiatedAttr< ::SCAML::usesImpl>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::SCAML::usesImpl, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::SCAML::usesImpl, Pred>(impl);}

			Udm::DerivedAttr< ::SCAML::usesImpl> Derived() { return Udm::DerivedAttr< ::SCAML::usesImpl>(impl);}
			template <class Pred> Udm::DerivedAttr< ::SCAML::usesImpl, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::SCAML::usesImpl, Pred>(impl);}

			Udm::ArchetypeAttr< ::SCAML::usesImpl> Archetype() const { return Udm::ArchetypeAttr< ::SCAML::usesImpl>(impl);}

			static ::Uml::CompositionParentRole meta_implementation_parent;
			Udm::ParentAttr< ::SCAML::implementation> implementation_parent() const { return Udm::ParentAttr< ::SCAML::implementation>(impl, meta_implementation_parent); }

			Udm::ParentAttr< ::SCAML::MgaObject> parent() const { return Udm::ParentAttr< ::SCAML::MgaObject>(impl, Udm::NULLPARENTROLE); }
			static ::Uml::AssociationRole meta_dstusesImpl_end_;
			Udm::AssocEndAttr< ::SCAML::implref> dstusesImpl_end() const { return Udm::AssocEndAttr< ::SCAML::implref>(impl, meta_dstusesImpl_end_); }

			static ::Uml::AssociationRole meta_srcusesImpl_end_;
			Udm::AssocEndAttr< ::SCAML::softpkgref> srcusesImpl_end() const { return Udm::AssocEndAttr< ::SCAML::softpkgref>(impl, meta_srcusesImpl_end_); }

		};

		class  dependency :  public ::SCAML::MgaObject {
		public:
			static ::Uml::Class meta;

			dependency() { }
			dependency(Udm::ObjectImpl *impl) : ::SCAML::MgaObject(impl), UDM_OBJECT(impl) { }
			dependency(const dependency &master) : ::SCAML::MgaObject(master), UDM_OBJECT(master) { }
			static dependency Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static dependency Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			dependency CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			dependency CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::SCAML::dependency> Instances() { return Udm::InstantiatedAttr< ::SCAML::dependency>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::SCAML::dependency, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::SCAML::dependency, Pred>(impl);}

			Udm::DerivedAttr< ::SCAML::dependency> Derived() { return Udm::DerivedAttr< ::SCAML::dependency>(impl);}
			template <class Pred> Udm::DerivedAttr< ::SCAML::dependency, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::SCAML::dependency, Pred>(impl);}

			Udm::ArchetypeAttr< ::SCAML::dependency> Archetype() const { return Udm::ArchetypeAttr< ::SCAML::dependency>(impl);}

			static ::Uml::Attribute meta_type;
			Udm::StringAttr type() const { return Udm::StringAttr(impl, meta_type); }

			static ::Uml::CompositionParentRole meta_implementation_parent;
			Udm::ParentAttr< ::SCAML::implementation> implementation_parent() const { return Udm::ParentAttr< ::SCAML::implementation>(impl, meta_implementation_parent); }

			Udm::ParentAttr< ::SCAML::MgaObject> parent() const { return Udm::ParentAttr< ::SCAML::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  usesdeviceref :  public ::SCAML::MgaObject {
		public:
			static ::Uml::Class meta;

			usesdeviceref() { }
			usesdeviceref(Udm::ObjectImpl *impl) : ::SCAML::MgaObject(impl), UDM_OBJECT(impl) { }
			usesdeviceref(const usesdeviceref &master) : ::SCAML::MgaObject(master), UDM_OBJECT(master) { }
			static usesdeviceref Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static usesdeviceref Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			usesdeviceref CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			usesdeviceref CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::SCAML::usesdeviceref> Instances() { return Udm::InstantiatedAttr< ::SCAML::usesdeviceref>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::SCAML::usesdeviceref, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::SCAML::usesdeviceref, Pred>(impl);}

			Udm::DerivedAttr< ::SCAML::usesdeviceref> Derived() { return Udm::DerivedAttr< ::SCAML::usesdeviceref>(impl);}
			template <class Pred> Udm::DerivedAttr< ::SCAML::usesdeviceref, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::SCAML::usesdeviceref, Pred>(impl);}

			Udm::ArchetypeAttr< ::SCAML::usesdeviceref> Archetype() const { return Udm::ArchetypeAttr< ::SCAML::usesdeviceref>(impl);}

			static ::Uml::AssociationRole meta_ref;
			Udm::PointerAttr< ::SCAML::DevicePkg> ref() const { return Udm::PointerAttr< ::SCAML::DevicePkg>(impl, meta_ref); }

			static ::Uml::CompositionParentRole meta_usesdevice_parent;
			Udm::ParentAttr< ::SCAML::usesdevice> usesdevice_parent() const { return Udm::ParentAttr< ::SCAML::usesdevice>(impl, meta_usesdevice_parent); }

			Udm::ParentAttr< ::SCAML::MgaObject> parent() const { return Udm::ParentAttr< ::SCAML::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  usesdevice :  public ::SCAML::MgaObject {
		public:
			static ::Uml::Class meta;

			usesdevice() { }
			usesdevice(Udm::ObjectImpl *impl) : ::SCAML::MgaObject(impl), UDM_OBJECT(impl) { }
			usesdevice(const usesdevice &master) : ::SCAML::MgaObject(master), UDM_OBJECT(master) { }
			static usesdevice Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static usesdevice Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			usesdevice CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			usesdevice CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::SCAML::usesdevice> Instances() { return Udm::InstantiatedAttr< ::SCAML::usesdevice>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::SCAML::usesdevice, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::SCAML::usesdevice, Pred>(impl);}

			Udm::DerivedAttr< ::SCAML::usesdevice> Derived() { return Udm::DerivedAttr< ::SCAML::usesdevice>(impl);}
			template <class Pred> Udm::DerivedAttr< ::SCAML::usesdevice, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::SCAML::usesdevice, Pred>(impl);}

			Udm::ArchetypeAttr< ::SCAML::usesdevice> Archetype() const { return Udm::ArchetypeAttr< ::SCAML::usesdevice>(impl);}

			static ::Uml::Attribute meta_type;
			Udm::StringAttr type() const { return Udm::StringAttr(impl, meta_type); }

			static ::Uml::CompositionChildRole meta_propertyref_children;
			Udm::ChildrenAttr< ::SCAML::propertyref> propertyref_children() const { return Udm::ChildrenAttr< ::SCAML::propertyref>(impl, meta_propertyref_children); }
			template <class Pred> Udm::ChildrenAttr< ::SCAML::propertyref, Pred> propertyref_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::propertyref, Pred>(impl, meta_propertyref_children); }

			static ::Uml::CompositionChildRole meta_usesdeviceref_children;
			Udm::ChildrenAttr< ::SCAML::usesdeviceref> usesdeviceref_children() const { return Udm::ChildrenAttr< ::SCAML::usesdeviceref>(impl, meta_usesdeviceref_children); }
			template <class Pred> Udm::ChildrenAttr< ::SCAML::usesdeviceref, Pred> usesdeviceref_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::usesdeviceref, Pred>(impl, meta_usesdeviceref_children); }

			Udm::ChildrenAttr< ::SCAML::usesdeviceref> usesdeviceref_kind_children() const { return Udm::ChildrenAttr< ::SCAML::usesdeviceref>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::usesdeviceref, Pred> usesdeviceref_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::usesdeviceref, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::propertyref> propertyref_kind_children() const { return Udm::ChildrenAttr< ::SCAML::propertyref>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::propertyref, Pred> propertyref_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::propertyref, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr< ::SCAML::MgaObject>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

			static ::Uml::CompositionParentRole meta_SoftPkg_parent;
			Udm::ParentAttr< ::SCAML::SoftPkg> SoftPkg_parent() const { return Udm::ParentAttr< ::SCAML::SoftPkg>(impl, meta_SoftPkg_parent); }

			static ::Uml::CompositionParentRole meta_implementation_parent;
			Udm::ParentAttr< ::SCAML::implementation> implementation_parent() const { return Udm::ParentAttr< ::SCAML::implementation>(impl, meta_implementation_parent); }

			Udm::ParentAttr< ::SCAML::MgaObject> parent() const { return Udm::ParentAttr< ::SCAML::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  propertyref :  public ::SCAML::MgaObject {
		public:
			static ::Uml::Class meta;

			propertyref() { }
			propertyref(Udm::ObjectImpl *impl) : ::SCAML::MgaObject(impl), UDM_OBJECT(impl) { }
			propertyref(const propertyref &master) : ::SCAML::MgaObject(master), UDM_OBJECT(master) { }
			static propertyref Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static propertyref Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			propertyref CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			propertyref CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::SCAML::propertyref> Instances() { return Udm::InstantiatedAttr< ::SCAML::propertyref>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::SCAML::propertyref, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::SCAML::propertyref, Pred>(impl);}

			Udm::DerivedAttr< ::SCAML::propertyref> Derived() { return Udm::DerivedAttr< ::SCAML::propertyref>(impl);}
			template <class Pred> Udm::DerivedAttr< ::SCAML::propertyref, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::SCAML::propertyref, Pred>(impl);}

			Udm::ArchetypeAttr< ::SCAML::propertyref> Archetype() const { return Udm::ArchetypeAttr< ::SCAML::propertyref>(impl);}

			static ::Uml::AssociationRole meta_ref;
			Udm::PointerAttr< ::SCAML::Properties> ref() const { return Udm::PointerAttr< ::SCAML::Properties>(impl, meta_ref); }

			static ::Uml::CompositionParentRole meta_usesdevice_parent;
			Udm::ParentAttr< ::SCAML::usesdevice> usesdevice_parent() const { return Udm::ParentAttr< ::SCAML::usesdevice>(impl, meta_usesdevice_parent); }

			Udm::ParentAttr< ::SCAML::MgaObject> parent() const { return Udm::ParentAttr< ::SCAML::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  implref :  public ::SCAML::MgaObject {
		public:
			static ::Uml::Class meta;

			implref() { }
			implref(Udm::ObjectImpl *impl) : ::SCAML::MgaObject(impl), UDM_OBJECT(impl) { }
			implref(const implref &master) : ::SCAML::MgaObject(master), UDM_OBJECT(master) { }
			static implref Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static implref Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			implref CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			implref CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::SCAML::implref> Instances() { return Udm::InstantiatedAttr< ::SCAML::implref>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::SCAML::implref, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::SCAML::implref, Pred>(impl);}

			Udm::DerivedAttr< ::SCAML::implref> Derived() { return Udm::DerivedAttr< ::SCAML::implref>(impl);}
			template <class Pred> Udm::DerivedAttr< ::SCAML::implref, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::SCAML::implref, Pred>(impl);}

			Udm::ArchetypeAttr< ::SCAML::implref> Archetype() const { return Udm::ArchetypeAttr< ::SCAML::implref>(impl);}

			static ::Uml::AssociationRole meta_srcusesImpl;
			static ::Uml::AssociationRole meta_srcusesImpl_rev;
			Udm::AClassAssocAttr< ::SCAML::usesImpl, ::SCAML::softpkgref> srcusesImpl() const { return Udm::AClassAssocAttr< ::SCAML::usesImpl, ::SCAML::softpkgref>(impl, meta_srcusesImpl, meta_srcusesImpl_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::SCAML::usesImpl, ::SCAML::softpkgref, Pred> srcusesImpl_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::SCAML::usesImpl, ::SCAML::softpkgref, Pred>(impl, meta_srcusesImpl, meta_srcusesImpl_rev); }

			static ::Uml::AssociationRole meta_ref;
			Udm::PointerAttr< ::SCAML::implementation> ref() const { return Udm::PointerAttr< ::SCAML::implementation>(impl, meta_ref); }

			static ::Uml::CompositionParentRole meta_implementation_parent;
			Udm::ParentAttr< ::SCAML::implementation> implementation_parent() const { return Udm::ParentAttr< ::SCAML::implementation>(impl, meta_implementation_parent); }

			Udm::ParentAttr< ::SCAML::MgaObject> parent() const { return Udm::ParentAttr< ::SCAML::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  simplepropertyref :  public ::SCAML::dependency {
		public:
			static ::Uml::Class meta;

			simplepropertyref() { }
			simplepropertyref(Udm::ObjectImpl *impl) : ::SCAML::dependency(impl), UDM_OBJECT(impl) { }
			simplepropertyref(const simplepropertyref &master) : ::SCAML::dependency(master), UDM_OBJECT(master) { }
			static simplepropertyref Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static simplepropertyref Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			simplepropertyref CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			simplepropertyref CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::SCAML::simplepropertyref> Instances() { return Udm::InstantiatedAttr< ::SCAML::simplepropertyref>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::SCAML::simplepropertyref, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::SCAML::simplepropertyref, Pred>(impl);}

			Udm::DerivedAttr< ::SCAML::simplepropertyref> Derived() { return Udm::DerivedAttr< ::SCAML::simplepropertyref>(impl);}
			template <class Pred> Udm::DerivedAttr< ::SCAML::simplepropertyref, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::SCAML::simplepropertyref, Pred>(impl);}

			Udm::ArchetypeAttr< ::SCAML::simplepropertyref> Archetype() const { return Udm::ArchetypeAttr< ::SCAML::simplepropertyref>(impl);}

			static ::Uml::Attribute meta_value;
			Udm::StringAttr value() const { return Udm::StringAttr(impl, meta_value); }

			static ::Uml::AssociationRole meta_ref;
			Udm::PointerAttr< ::SCAML::simplePropertyDescriptor> ref() const { return Udm::PointerAttr< ::SCAML::simplePropertyDescriptor>(impl, meta_ref); }

			Udm::ParentAttr< ::SCAML::MgaObject> parent() const { return Udm::ParentAttr< ::SCAML::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  softpkgref :  public ::SCAML::dependency {
		public:
			static ::Uml::Class meta;

			softpkgref() { }
			softpkgref(Udm::ObjectImpl *impl) : ::SCAML::dependency(impl), UDM_OBJECT(impl) { }
			softpkgref(const softpkgref &master) : ::SCAML::dependency(master), UDM_OBJECT(master) { }
			static softpkgref Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static softpkgref Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			softpkgref CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			softpkgref CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::SCAML::softpkgref> Instances() { return Udm::InstantiatedAttr< ::SCAML::softpkgref>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::SCAML::softpkgref, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::SCAML::softpkgref, Pred>(impl);}

			Udm::DerivedAttr< ::SCAML::softpkgref> Derived() { return Udm::DerivedAttr< ::SCAML::softpkgref>(impl);}
			template <class Pred> Udm::DerivedAttr< ::SCAML::softpkgref, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::SCAML::softpkgref, Pred>(impl);}

			Udm::ArchetypeAttr< ::SCAML::softpkgref> Archetype() const { return Udm::ArchetypeAttr< ::SCAML::softpkgref>(impl);}

			static ::Uml::AssociationRole meta_dstusesImpl;
			static ::Uml::AssociationRole meta_dstusesImpl_rev;
			Udm::AClassPointerAttr< ::SCAML::usesImpl, ::SCAML::implref> dstusesImpl() const { return Udm::AClassPointerAttr< ::SCAML::usesImpl, ::SCAML::implref>(impl, meta_dstusesImpl, meta_dstusesImpl_rev); }

			static ::Uml::AssociationRole meta_ref;
			Udm::PointerAttr< ::SCAML::SoftPkg> ref() const { return Udm::PointerAttr< ::SCAML::SoftPkg>(impl, meta_ref); }

			Udm::ParentAttr< ::SCAML::MgaObject> parent() const { return Udm::ParentAttr< ::SCAML::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  runtime :  public ::SCAML::MgaObject {
		public:
			static ::Uml::Class meta;

			runtime() { }
			runtime(Udm::ObjectImpl *impl) : ::SCAML::MgaObject(impl), UDM_OBJECT(impl) { }
			runtime(const runtime &master) : ::SCAML::MgaObject(master), UDM_OBJECT(master) { }
			static runtime Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static runtime Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			runtime CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			runtime CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::SCAML::runtime> Instances() { return Udm::InstantiatedAttr< ::SCAML::runtime>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::SCAML::runtime, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::SCAML::runtime, Pred>(impl);}

			Udm::DerivedAttr< ::SCAML::runtime> Derived() { return Udm::DerivedAttr< ::SCAML::runtime>(impl);}
			template <class Pred> Udm::DerivedAttr< ::SCAML::runtime, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::SCAML::runtime, Pred>(impl);}

			Udm::ArchetypeAttr< ::SCAML::runtime> Archetype() const { return Udm::ArchetypeAttr< ::SCAML::runtime>(impl);}

			static ::Uml::Attribute meta_version;
			Udm::StringAttr version() const { return Udm::StringAttr(impl, meta_version); }

			static ::Uml::CompositionParentRole meta_implementation_parent;
			Udm::ParentAttr< ::SCAML::implementation> implementation_parent() const { return Udm::ParentAttr< ::SCAML::implementation>(impl, meta_implementation_parent); }

			Udm::ParentAttr< ::SCAML::MgaObject> parent() const { return Udm::ParentAttr< ::SCAML::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  processor :  public ::SCAML::MgaObject {
		public:
			static ::Uml::Class meta;

			processor() { }
			processor(Udm::ObjectImpl *impl) : ::SCAML::MgaObject(impl), UDM_OBJECT(impl) { }
			processor(const processor &master) : ::SCAML::MgaObject(master), UDM_OBJECT(master) { }
			static processor Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static processor Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			processor CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			processor CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::SCAML::processor> Instances() { return Udm::InstantiatedAttr< ::SCAML::processor>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::SCAML::processor, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::SCAML::processor, Pred>(impl);}

			Udm::DerivedAttr< ::SCAML::processor> Derived() { return Udm::DerivedAttr< ::SCAML::processor>(impl);}
			template <class Pred> Udm::DerivedAttr< ::SCAML::processor, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::SCAML::processor, Pred>(impl);}

			Udm::ArchetypeAttr< ::SCAML::processor> Archetype() const { return Udm::ArchetypeAttr< ::SCAML::processor>(impl);}

			static ::Uml::CompositionParentRole meta_implementation_parent;
			Udm::ParentAttr< ::SCAML::implementation> implementation_parent() const { return Udm::ParentAttr< ::SCAML::implementation>(impl, meta_implementation_parent); }

			Udm::ParentAttr< ::SCAML::MgaObject> parent() const { return Udm::ParentAttr< ::SCAML::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  os :  public ::SCAML::MgaObject {
		public:
			static ::Uml::Class meta;

			os() { }
			os(Udm::ObjectImpl *impl) : ::SCAML::MgaObject(impl), UDM_OBJECT(impl) { }
			os(const os &master) : ::SCAML::MgaObject(master), UDM_OBJECT(master) { }
			static os Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static os Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			os CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			os CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::SCAML::os> Instances() { return Udm::InstantiatedAttr< ::SCAML::os>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::SCAML::os, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::SCAML::os, Pred>(impl);}

			Udm::DerivedAttr< ::SCAML::os> Derived() { return Udm::DerivedAttr< ::SCAML::os>(impl);}
			template <class Pred> Udm::DerivedAttr< ::SCAML::os, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::SCAML::os, Pred>(impl);}

			Udm::ArchetypeAttr< ::SCAML::os> Archetype() const { return Udm::ArchetypeAttr< ::SCAML::os>(impl);}

			static ::Uml::Attribute meta_version;
			Udm::StringAttr version() const { return Udm::StringAttr(impl, meta_version); }

			static ::Uml::CompositionParentRole meta_implementation_parent;
			Udm::ParentAttr< ::SCAML::implementation> implementation_parent() const { return Udm::ParentAttr< ::SCAML::implementation>(impl, meta_implementation_parent); }

			Udm::ParentAttr< ::SCAML::MgaObject> parent() const { return Udm::ParentAttr< ::SCAML::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  humanlanguage :  public ::SCAML::MgaObject {
		public:
			static ::Uml::Class meta;

			humanlanguage() { }
			humanlanguage(Udm::ObjectImpl *impl) : ::SCAML::MgaObject(impl), UDM_OBJECT(impl) { }
			humanlanguage(const humanlanguage &master) : ::SCAML::MgaObject(master), UDM_OBJECT(master) { }
			static humanlanguage Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static humanlanguage Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			humanlanguage CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			humanlanguage CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::SCAML::humanlanguage> Instances() { return Udm::InstantiatedAttr< ::SCAML::humanlanguage>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::SCAML::humanlanguage, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::SCAML::humanlanguage, Pred>(impl);}

			Udm::DerivedAttr< ::SCAML::humanlanguage> Derived() { return Udm::DerivedAttr< ::SCAML::humanlanguage>(impl);}
			template <class Pred> Udm::DerivedAttr< ::SCAML::humanlanguage, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::SCAML::humanlanguage, Pred>(impl);}

			Udm::ArchetypeAttr< ::SCAML::humanlanguage> Archetype() const { return Udm::ArchetypeAttr< ::SCAML::humanlanguage>(impl);}

			static ::Uml::CompositionParentRole meta_implementation_parent;
			Udm::ParentAttr< ::SCAML::implementation> implementation_parent() const { return Udm::ParentAttr< ::SCAML::implementation>(impl, meta_implementation_parent); }

			Udm::ParentAttr< ::SCAML::MgaObject> parent() const { return Udm::ParentAttr< ::SCAML::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  compiler :  public ::SCAML::MgaObject {
		public:
			static ::Uml::Class meta;

			compiler() { }
			compiler(Udm::ObjectImpl *impl) : ::SCAML::MgaObject(impl), UDM_OBJECT(impl) { }
			compiler(const compiler &master) : ::SCAML::MgaObject(master), UDM_OBJECT(master) { }
			static compiler Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static compiler Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			compiler CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			compiler CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::SCAML::compiler> Instances() { return Udm::InstantiatedAttr< ::SCAML::compiler>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::SCAML::compiler, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::SCAML::compiler, Pred>(impl);}

			Udm::DerivedAttr< ::SCAML::compiler> Derived() { return Udm::DerivedAttr< ::SCAML::compiler>(impl);}
			template <class Pred> Udm::DerivedAttr< ::SCAML::compiler, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::SCAML::compiler, Pred>(impl);}

			Udm::ArchetypeAttr< ::SCAML::compiler> Archetype() const { return Udm::ArchetypeAttr< ::SCAML::compiler>(impl);}

			static ::Uml::Attribute meta_version;
			Udm::StringAttr version() const { return Udm::StringAttr(impl, meta_version); }

			static ::Uml::CompositionParentRole meta_implementation_parent;
			Udm::ParentAttr< ::SCAML::implementation> implementation_parent() const { return Udm::ParentAttr< ::SCAML::implementation>(impl, meta_implementation_parent); }

			Udm::ParentAttr< ::SCAML::MgaObject> parent() const { return Udm::ParentAttr< ::SCAML::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  programminglanguage :  public ::SCAML::MgaObject {
		public:
			static ::Uml::Class meta;

			programminglanguage() { }
			programminglanguage(Udm::ObjectImpl *impl) : ::SCAML::MgaObject(impl), UDM_OBJECT(impl) { }
			programminglanguage(const programminglanguage &master) : ::SCAML::MgaObject(master), UDM_OBJECT(master) { }
			static programminglanguage Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static programminglanguage Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			programminglanguage CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			programminglanguage CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::SCAML::programminglanguage> Instances() { return Udm::InstantiatedAttr< ::SCAML::programminglanguage>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::SCAML::programminglanguage, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::SCAML::programminglanguage, Pred>(impl);}

			Udm::DerivedAttr< ::SCAML::programminglanguage> Derived() { return Udm::DerivedAttr< ::SCAML::programminglanguage>(impl);}
			template <class Pred> Udm::DerivedAttr< ::SCAML::programminglanguage, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::SCAML::programminglanguage, Pred>(impl);}

			Udm::ArchetypeAttr< ::SCAML::programminglanguage> Archetype() const { return Udm::ArchetypeAttr< ::SCAML::programminglanguage>(impl);}

			static ::Uml::Attribute meta_version;
			Udm::StringAttr version() const { return Udm::StringAttr(impl, meta_version); }

			static ::Uml::CompositionParentRole meta_implementation_parent;
			Udm::ParentAttr< ::SCAML::implementation> implementation_parent() const { return Udm::ParentAttr< ::SCAML::implementation>(impl, meta_implementation_parent); }

			Udm::ParentAttr< ::SCAML::MgaObject> parent() const { return Udm::ParentAttr< ::SCAML::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  code :  public ::SCAML::MgaObject {
		public:
			static ::Uml::Class meta;

			code() { }
			code(Udm::ObjectImpl *impl) : ::SCAML::MgaObject(impl), UDM_OBJECT(impl) { }
			code(const code &master) : ::SCAML::MgaObject(master), UDM_OBJECT(master) { }
			static code Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static code Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			code CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			code CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::SCAML::code> Instances() { return Udm::InstantiatedAttr< ::SCAML::code>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::SCAML::code, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::SCAML::code, Pred>(impl);}

			Udm::DerivedAttr< ::SCAML::code> Derived() { return Udm::DerivedAttr< ::SCAML::code>(impl);}
			template <class Pred> Udm::DerivedAttr< ::SCAML::code, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::SCAML::code, Pred>(impl);}

			Udm::ArchetypeAttr< ::SCAML::code> Archetype() const { return Udm::ArchetypeAttr< ::SCAML::code>(impl);}

			static ::Uml::Attribute meta_entrypoint;
			Udm::StringAttr entrypoint() const { return Udm::StringAttr(impl, meta_entrypoint); }

			static ::Uml::Attribute meta_localfile;
			Udm::StringAttr localfile() const { return Udm::StringAttr(impl, meta_localfile); }

			static ::Uml::Attribute meta_stacksize;
			Udm::StringAttr stacksize() const { return Udm::StringAttr(impl, meta_stacksize); }

			static ::Uml::Attribute meta_priority;
			Udm::StringAttr priority() const { return Udm::StringAttr(impl, meta_priority); }

			static ::Uml::Attribute meta_type;
			Udm::StringAttr type() const { return Udm::StringAttr(impl, meta_type); }

			static ::Uml::CompositionParentRole meta_implementation_parent;
			Udm::ParentAttr< ::SCAML::implementation> implementation_parent() const { return Udm::ParentAttr< ::SCAML::implementation>(impl, meta_implementation_parent); }

			Udm::ParentAttr< ::SCAML::MgaObject> parent() const { return Udm::ParentAttr< ::SCAML::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  author :  public ::SCAML::MgaObject {
		public:
			static ::Uml::Class meta;

			author() { }
			author(Udm::ObjectImpl *impl) : ::SCAML::MgaObject(impl), UDM_OBJECT(impl) { }
			author(const author &master) : ::SCAML::MgaObject(master), UDM_OBJECT(master) { }
			static author Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static author Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			author CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			author CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::SCAML::author> Instances() { return Udm::InstantiatedAttr< ::SCAML::author>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::SCAML::author, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::SCAML::author, Pred>(impl);}

			Udm::DerivedAttr< ::SCAML::author> Derived() { return Udm::DerivedAttr< ::SCAML::author>(impl);}
			template <class Pred> Udm::DerivedAttr< ::SCAML::author, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::SCAML::author, Pred>(impl);}

			Udm::ArchetypeAttr< ::SCAML::author> Archetype() const { return Udm::ArchetypeAttr< ::SCAML::author>(impl);}

			static ::Uml::Attribute meta_company;
			Udm::StringAttr company() const { return Udm::StringAttr(impl, meta_company); }

			static ::Uml::Attribute meta_names;
			Udm::StringAttr names() const { return Udm::StringAttr(impl, meta_names); }

			static ::Uml::Attribute meta_webpage;
			Udm::StringAttr webpage() const { return Udm::StringAttr(impl, meta_webpage); }

			static ::Uml::CompositionParentRole meta_DevicePkg_parent;
			Udm::ParentAttr< ::SCAML::DevicePkg> DevicePkg_parent() const { return Udm::ParentAttr< ::SCAML::DevicePkg>(impl, meta_DevicePkg_parent); }

			static ::Uml::CompositionParentRole meta_SoftPkg_parent;
			Udm::ParentAttr< ::SCAML::SoftPkg> SoftPkg_parent() const { return Udm::ParentAttr< ::SCAML::SoftPkg>(impl, meta_SoftPkg_parent); }

			Udm::ParentAttr< ::SCAML::MgaObject> parent() const { return Udm::ParentAttr< ::SCAML::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  implementation :  virtual public ::SCAML::MgaObject {
		public:
			static ::Uml::Class meta;

			implementation() { }
			implementation(Udm::ObjectImpl *impl) : ::SCAML::MgaObject(impl), UDM_OBJECT(impl) { }
			implementation(const implementation &master) : ::SCAML::MgaObject(master), UDM_OBJECT(master) { }
			static implementation Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static implementation Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			implementation CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			implementation CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::SCAML::implementation> Instances() { return Udm::InstantiatedAttr< ::SCAML::implementation>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::SCAML::implementation, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::SCAML::implementation, Pred>(impl);}

			Udm::DerivedAttr< ::SCAML::implementation> Derived() { return Udm::DerivedAttr< ::SCAML::implementation>(impl);}
			template <class Pred> Udm::DerivedAttr< ::SCAML::implementation, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::SCAML::implementation, Pred>(impl);}

			Udm::ArchetypeAttr< ::SCAML::implementation> Archetype() const { return Udm::ArchetypeAttr< ::SCAML::implementation>(impl);}

			static ::Uml::Attribute meta_aepcompliance;
			Udm::StringAttr aepcompliance() const { return Udm::StringAttr(impl, meta_aepcompliance); }

			static ::Uml::Attribute meta_description;
			Udm::StringAttr description() const { return Udm::StringAttr(impl, meta_description); }

			static ::Uml::Attribute meta_id;
			Udm::StringAttr id() const { return Udm::StringAttr(impl, meta_id); }

			static ::Uml::AssociationRole meta_referedbyimplref;
			Udm::AssocAttr< ::SCAML::implref> referedbyimplref() const { return Udm::AssocAttr< ::SCAML::implref>(impl, meta_referedbyimplref); }
			template <class Pred> Udm::AssocAttr< ::SCAML::implref, Pred > referedbyimplref_sorted(const Pred &) const { return Udm::AssocAttr< ::SCAML::implref, Pred>(impl, meta_referedbyimplref); }

			static ::Uml::CompositionChildRole meta_implref_children;
			Udm::ChildrenAttr< ::SCAML::implref> implref_children() const { return Udm::ChildrenAttr< ::SCAML::implref>(impl, meta_implref_children); }
			template <class Pred> Udm::ChildrenAttr< ::SCAML::implref, Pred> implref_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::implref, Pred>(impl, meta_implref_children); }

			static ::Uml::CompositionChildRole meta_usesImpl_children;
			Udm::ChildrenAttr< ::SCAML::usesImpl> usesImpl_children() const { return Udm::ChildrenAttr< ::SCAML::usesImpl>(impl, meta_usesImpl_children); }
			template <class Pred> Udm::ChildrenAttr< ::SCAML::usesImpl, Pred> usesImpl_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::usesImpl, Pred>(impl, meta_usesImpl_children); }

			static ::Uml::CompositionChildRole meta_dependency_children;
			Udm::ChildrenAttr< ::SCAML::dependency> dependency_children() const { return Udm::ChildrenAttr< ::SCAML::dependency>(impl, meta_dependency_children); }
			template <class Pred> Udm::ChildrenAttr< ::SCAML::dependency, Pred> dependency_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::dependency, Pred>(impl, meta_dependency_children); }

			static ::Uml::CompositionChildRole meta_code_child;
			Udm::ChildAttr< ::SCAML::code> code_child() const { return Udm::ChildAttr< ::SCAML::code>(impl, meta_code_child); }

			static ::Uml::CompositionChildRole meta_humanlanguage_child;
			Udm::ChildAttr< ::SCAML::humanlanguage> humanlanguage_child() const { return Udm::ChildAttr< ::SCAML::humanlanguage>(impl, meta_humanlanguage_child); }

			static ::Uml::CompositionChildRole meta_runtime_child;
			Udm::ChildAttr< ::SCAML::runtime> runtime_child() const { return Udm::ChildAttr< ::SCAML::runtime>(impl, meta_runtime_child); }

			static ::Uml::CompositionChildRole meta_compiler_child;
			Udm::ChildAttr< ::SCAML::compiler> compiler_child() const { return Udm::ChildAttr< ::SCAML::compiler>(impl, meta_compiler_child); }

			static ::Uml::CompositionChildRole meta_processor_children;
			Udm::ChildrenAttr< ::SCAML::processor> processor_children() const { return Udm::ChildrenAttr< ::SCAML::processor>(impl, meta_processor_children); }
			template <class Pred> Udm::ChildrenAttr< ::SCAML::processor, Pred> processor_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::processor, Pred>(impl, meta_processor_children); }

			static ::Uml::CompositionChildRole meta_os_children;
			Udm::ChildrenAttr< ::SCAML::os> os_children() const { return Udm::ChildrenAttr< ::SCAML::os>(impl, meta_os_children); }
			template <class Pred> Udm::ChildrenAttr< ::SCAML::os, Pred> os_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::os, Pred>(impl, meta_os_children); }

			static ::Uml::CompositionChildRole meta_propertyfile_children;
			Udm::ChildrenAttr< ::SCAML::propertyfile> propertyfile_children() const { return Udm::ChildrenAttr< ::SCAML::propertyfile>(impl, meta_propertyfile_children); }
			template <class Pred> Udm::ChildrenAttr< ::SCAML::propertyfile, Pred> propertyfile_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::propertyfile, Pred>(impl, meta_propertyfile_children); }

			static ::Uml::CompositionChildRole meta_usesdevice_children;
			Udm::ChildrenAttr< ::SCAML::usesdevice> usesdevice_children() const { return Udm::ChildrenAttr< ::SCAML::usesdevice>(impl, meta_usesdevice_children); }
			template <class Pred> Udm::ChildrenAttr< ::SCAML::usesdevice, Pred> usesdevice_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::usesdevice, Pred>(impl, meta_usesdevice_children); }

			static ::Uml::CompositionChildRole meta_programminglanguage_child;
			Udm::ChildAttr< ::SCAML::programminglanguage> programminglanguage_child() const { return Udm::ChildAttr< ::SCAML::programminglanguage>(impl, meta_programminglanguage_child); }

			Udm::ChildrenAttr< ::SCAML::usesImpl> usesImpl_kind_children() const { return Udm::ChildrenAttr< ::SCAML::usesImpl>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::usesImpl, Pred> usesImpl_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::usesImpl, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::dependency> dependency_kind_children() const { return Udm::ChildrenAttr< ::SCAML::dependency>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::dependency, Pred> dependency_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::dependency, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::usesdevice> usesdevice_kind_children() const { return Udm::ChildrenAttr< ::SCAML::usesdevice>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::usesdevice, Pred> usesdevice_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::usesdevice, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::implref> implref_kind_children() const { return Udm::ChildrenAttr< ::SCAML::implref>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::implref, Pred> implref_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::implref, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::simplepropertyref> simplepropertyref_kind_children() const { return Udm::ChildrenAttr< ::SCAML::simplepropertyref>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::simplepropertyref, Pred> simplepropertyref_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::simplepropertyref, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::softpkgref> softpkgref_kind_children() const { return Udm::ChildrenAttr< ::SCAML::softpkgref>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::softpkgref, Pred> softpkgref_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::softpkgref, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::runtime> runtime_kind_children() const { return Udm::ChildrenAttr< ::SCAML::runtime>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::runtime, Pred> runtime_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::runtime, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::processor> processor_kind_children() const { return Udm::ChildrenAttr< ::SCAML::processor>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::processor, Pred> processor_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::processor, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::os> os_kind_children() const { return Udm::ChildrenAttr< ::SCAML::os>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::os, Pred> os_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::os, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::humanlanguage> humanlanguage_kind_children() const { return Udm::ChildrenAttr< ::SCAML::humanlanguage>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::humanlanguage, Pred> humanlanguage_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::humanlanguage, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::compiler> compiler_kind_children() const { return Udm::ChildrenAttr< ::SCAML::compiler>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::compiler, Pred> compiler_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::compiler, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::programminglanguage> programminglanguage_kind_children() const { return Udm::ChildrenAttr< ::SCAML::programminglanguage>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::programminglanguage, Pred> programminglanguage_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::programminglanguage, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::code> code_kind_children() const { return Udm::ChildrenAttr< ::SCAML::code>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::code, Pred> code_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::code, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::propertyfile> propertyfile_kind_children() const { return Udm::ChildrenAttr< ::SCAML::propertyfile>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::propertyfile, Pred> propertyfile_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::propertyfile, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr< ::SCAML::MgaObject>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

			static ::Uml::CompositionParentRole meta_SoftPkg_parent;
			Udm::ParentAttr< ::SCAML::SoftPkg> SoftPkg_parent() const { return Udm::ParentAttr< ::SCAML::SoftPkg>(impl, meta_SoftPkg_parent); }

			Udm::ParentAttr< ::SCAML::MgaObject> parent() const { return Udm::ParentAttr< ::SCAML::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  SimulinkSystem :  public ::SCAML::MgaObject {
		public:
			static ::Uml::Class meta;

			SimulinkSystem() { }
			SimulinkSystem(Udm::ObjectImpl *impl) : ::SCAML::MgaObject(impl), UDM_OBJECT(impl) { }
			SimulinkSystem(const SimulinkSystem &master) : ::SCAML::MgaObject(master), UDM_OBJECT(master) { }
			static SimulinkSystem Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static SimulinkSystem Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			SimulinkSystem CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			SimulinkSystem CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::SCAML::SimulinkSystem> Instances() { return Udm::InstantiatedAttr< ::SCAML::SimulinkSystem>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::SCAML::SimulinkSystem, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::SCAML::SimulinkSystem, Pred>(impl);}

			Udm::DerivedAttr< ::SCAML::SimulinkSystem> Derived() { return Udm::DerivedAttr< ::SCAML::SimulinkSystem>(impl);}
			template <class Pred> Udm::DerivedAttr< ::SCAML::SimulinkSystem, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::SCAML::SimulinkSystem, Pred>(impl);}

			Udm::ArchetypeAttr< ::SCAML::SimulinkSystem> Archetype() const { return Udm::ArchetypeAttr< ::SCAML::SimulinkSystem>(impl);}

			static ::Uml::CompositionChildRole meta_DataPort_children;
			Udm::ChildrenAttr< ::SCAML::DataPort> DataPort_children() const { return Udm::ChildrenAttr< ::SCAML::DataPort>(impl, meta_DataPort_children); }
			template <class Pred> Udm::ChildrenAttr< ::SCAML::DataPort, Pred> DataPort_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::DataPort, Pred>(impl, meta_DataPort_children); }

			static ::Uml::CompositionChildRole meta_SimulinkVars_children;
			Udm::ChildrenAttr< ::SCAML::SimulinkVars> SimulinkVars_children() const { return Udm::ChildrenAttr< ::SCAML::SimulinkVars>(impl, meta_SimulinkVars_children); }
			template <class Pred> Udm::ChildrenAttr< ::SCAML::SimulinkVars, Pred> SimulinkVars_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::SimulinkVars, Pred>(impl, meta_SimulinkVars_children); }

			static ::Uml::CompositionChildRole meta_SimulinkEntrypoint_children;
			Udm::ChildrenAttr< ::SCAML::SimulinkEntrypoint> SimulinkEntrypoint_children() const { return Udm::ChildrenAttr< ::SCAML::SimulinkEntrypoint>(impl, meta_SimulinkEntrypoint_children); }
			template <class Pred> Udm::ChildrenAttr< ::SCAML::SimulinkEntrypoint, Pred> SimulinkEntrypoint_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::SimulinkEntrypoint, Pred>(impl, meta_SimulinkEntrypoint_children); }

			Udm::ChildrenAttr< ::SCAML::CFSrc> CFSrc_kind_children() const { return Udm::ChildrenAttr< ::SCAML::CFSrc>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::CFSrc, Pred> CFSrc_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::CFSrc, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::ArgRef> ArgRef_kind_children() const { return Udm::ChildrenAttr< ::SCAML::ArgRef>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::ArgRef, Pred> ArgRef_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::ArgRef, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::SimulinkEntrypoint> SimulinkEntrypoint_kind_children() const { return Udm::ChildrenAttr< ::SCAML::SimulinkEntrypoint>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::SimulinkEntrypoint, Pred> SimulinkEntrypoint_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::SimulinkEntrypoint, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::CFDst> CFDst_kind_children() const { return Udm::ChildrenAttr< ::SCAML::CFDst>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::CFDst, Pred> CFDst_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::CFDst, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::DataInport> DataInport_kind_children() const { return Udm::ChildrenAttr< ::SCAML::DataInport>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::DataInport, Pred> DataInport_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::DataInport, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::DataOutport> DataOutport_kind_children() const { return Udm::ChildrenAttr< ::SCAML::DataOutport>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::DataOutport, Pred> DataOutport_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::DataOutport, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::DFFCO> DFFCO_kind_children() const { return Udm::ChildrenAttr< ::SCAML::DFFCO>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::DFFCO, Pred> DFFCO_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::DFFCO, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::DataPort> DataPort_kind_children() const { return Udm::ChildrenAttr< ::SCAML::DataPort>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::DataPort, Pred> DataPort_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::DataPort, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::SimulinkVars> SimulinkVars_kind_children() const { return Udm::ChildrenAttr< ::SCAML::SimulinkVars>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::SimulinkVars, Pred> SimulinkVars_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::SimulinkVars, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr< ::SCAML::MgaObject>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

			static ::Uml::CompositionParentRole meta_SimulinkImplementation_parent;
			Udm::ParentAttr< ::SCAML::SimulinkImplementation> SimulinkImplementation_parent() const { return Udm::ParentAttr< ::SCAML::SimulinkImplementation>(impl, meta_SimulinkImplementation_parent); }

			Udm::ParentAttr< ::SCAML::componentplacement_Members_Base> componentplacement_Members_Base_parent() const { return Udm::ParentAttr< ::SCAML::componentplacement_Members_Base>(impl, Udm::NULLPARENTROLE); }
			Udm::ParentAttr< ::SCAML::MgaObject> MgaObject_parent() const { return Udm::ParentAttr< ::SCAML::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  DFFCO :  virtual public ::SCAML::MgaObject {
		public:
			static ::Uml::Class meta;

			DFFCO() { }
			DFFCO(Udm::ObjectImpl *impl) : ::SCAML::MgaObject(impl), UDM_OBJECT(impl) { }
			DFFCO(const DFFCO &master) : ::SCAML::MgaObject(master), UDM_OBJECT(master) { }
			static DFFCO Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static DFFCO Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			DFFCO CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			DFFCO CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::SCAML::DFFCO> Instances() { return Udm::InstantiatedAttr< ::SCAML::DFFCO>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::SCAML::DFFCO, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::SCAML::DFFCO, Pred>(impl);}

			Udm::DerivedAttr< ::SCAML::DFFCO> Derived() { return Udm::DerivedAttr< ::SCAML::DFFCO>(impl);}
			template <class Pred> Udm::DerivedAttr< ::SCAML::DFFCO, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::SCAML::DFFCO, Pred>(impl);}

			Udm::ArchetypeAttr< ::SCAML::DFFCO> Archetype() const { return Udm::ArchetypeAttr< ::SCAML::DFFCO>(impl);}

			static ::Uml::AssociationRole meta_srcDataFlow;
			static ::Uml::AssociationRole meta_srcDataFlow_rev;
			Udm::AClassAssocAttr< ::SCAML::DataFlow, ::SCAML::DFFCO> srcDataFlow() const { return Udm::AClassAssocAttr< ::SCAML::DataFlow, ::SCAML::DFFCO>(impl, meta_srcDataFlow, meta_srcDataFlow_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::SCAML::DataFlow, ::SCAML::DFFCO, Pred> srcDataFlow_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::SCAML::DataFlow, ::SCAML::DFFCO, Pred>(impl, meta_srcDataFlow, meta_srcDataFlow_rev); }

			static ::Uml::AssociationRole meta_dstDataFlow;
			static ::Uml::AssociationRole meta_dstDataFlow_rev;
			Udm::AClassAssocAttr< ::SCAML::DataFlow, ::SCAML::DFFCO> dstDataFlow() const { return Udm::AClassAssocAttr< ::SCAML::DataFlow, ::SCAML::DFFCO>(impl, meta_dstDataFlow, meta_dstDataFlow_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::SCAML::DataFlow, ::SCAML::DFFCO, Pred> dstDataFlow_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::SCAML::DataFlow, ::SCAML::DFFCO, Pred>(impl, meta_dstDataFlow, meta_dstDataFlow_rev); }

			Udm::ParentAttr<Udm::Object> parent() const { return Udm::ParentAttr<Udm::Object>(impl, Udm::NULLPARENTROLE); }
		};

		class  port :  public ::SCAML::CFSrc, public ::SCAML::CFDst, public ::SCAML::componentfeatures, public ::SCAML::DFFCO {
		public:
			static ::Uml::Class meta;

			port() { }
			port(Udm::ObjectImpl *impl) : ::SCAML::CFSrc(impl),::SCAML::CFDst(impl),::SCAML::componentfeatures(impl),::SCAML::DFFCO(impl), ::SCAML::MgaObject(impl), UDM_OBJECT(impl) { }
			port(const port &master) : ::SCAML::CFSrc(master),::SCAML::CFDst(master),::SCAML::componentfeatures(master),::SCAML::DFFCO(master), ::SCAML::MgaObject(master), UDM_OBJECT(master) { }
			static port Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static port Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			port CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			port CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::SCAML::port> Instances() { return Udm::InstantiatedAttr< ::SCAML::port>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::SCAML::port, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::SCAML::port, Pred>(impl);}

			Udm::DerivedAttr< ::SCAML::port> Derived() { return Udm::DerivedAttr< ::SCAML::port>(impl);}
			template <class Pred> Udm::DerivedAttr< ::SCAML::port, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::SCAML::port, Pred>(impl);}

			Udm::ArchetypeAttr< ::SCAML::port> Archetype() const { return Udm::ArchetypeAttr< ::SCAML::port>(impl);}

			static ::Uml::Attribute meta_isControl;
			Udm::BooleanAttr isControl() const { return Udm::BooleanAttr(impl, meta_isControl); }

			static ::Uml::Attribute meta_isData;
			Udm::BooleanAttr isData() const { return Udm::BooleanAttr(impl, meta_isData); }

			static ::Uml::Attribute meta_isResponses;
			Udm::BooleanAttr isResponses() const { return Udm::BooleanAttr(impl, meta_isResponses); }

			static ::Uml::Attribute meta_isTest;
			Udm::BooleanAttr isTest() const { return Udm::BooleanAttr(impl, meta_isTest); }

			Udm::ParentAttr<Udm::Object> parent() const { return Udm::ParentAttr<Udm::Object>(impl, Udm::NULLPARENTROLE); }
		};

		class  usesPort :  public ::SCAML::externaltodestBase, public ::SCAML::port {
		public:
			static ::Uml::Class meta;

			usesPort() { }
			usesPort(Udm::ObjectImpl *impl) : ::SCAML::externaltodestBase(impl),::SCAML::port(impl), ::SCAML::MgaObject(impl), UDM_OBJECT(impl) { }
			usesPort(const usesPort &master) : ::SCAML::externaltodestBase(master),::SCAML::port(master), ::SCAML::MgaObject(master), UDM_OBJECT(master) { }
			static usesPort Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static usesPort Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			usesPort CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			usesPort CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::SCAML::usesPort> Instances() { return Udm::InstantiatedAttr< ::SCAML::usesPort>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::SCAML::usesPort, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::SCAML::usesPort, Pred>(impl);}

			Udm::DerivedAttr< ::SCAML::usesPort> Derived() { return Udm::DerivedAttr< ::SCAML::usesPort>(impl);}
			template <class Pred> Udm::DerivedAttr< ::SCAML::usesPort, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::SCAML::usesPort, Pred>(impl);}

			Udm::ArchetypeAttr< ::SCAML::usesPort> Archetype() const { return Udm::ArchetypeAttr< ::SCAML::usesPort>(impl);}

			static ::Uml::Attribute meta_usesname;
			Udm::StringAttr usesname() const { return Udm::StringAttr(impl, meta_usesname); }

			static ::Uml::AssociationRole meta_dstusestodestConn;
			static ::Uml::AssociationRole meta_dstusestodestConn_rev;
			Udm::AClassAssocAttr< ::SCAML::usestodestConn, ::SCAML::connectdestBase> dstusestodestConn() const { return Udm::AClassAssocAttr< ::SCAML::usestodestConn, ::SCAML::connectdestBase>(impl, meta_dstusestodestConn, meta_dstusestodestConn_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::SCAML::usestodestConn, ::SCAML::connectdestBase, Pred> dstusestodestConn_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::SCAML::usestodestConn, ::SCAML::connectdestBase, Pred>(impl, meta_dstusestodestConn, meta_dstusestodestConn_rev); }

			static ::Uml::AssociationRole meta_dstusestofreestandingConn;
			static ::Uml::AssociationRole meta_dstusestofreestandingConn_rev;
			Udm::AClassAssocAttr< ::SCAML::usestofreestandingConn, ::SCAML::FreestandingPort> dstusestofreestandingConn() const { return Udm::AClassAssocAttr< ::SCAML::usestofreestandingConn, ::SCAML::FreestandingPort>(impl, meta_dstusestofreestandingConn, meta_dstusestofreestandingConn_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::SCAML::usestofreestandingConn, ::SCAML::FreestandingPort, Pred> dstusestofreestandingConn_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::SCAML::usestofreestandingConn, ::SCAML::FreestandingPort, Pred>(impl, meta_dstusestofreestandingConn, meta_dstusestofreestandingConn_rev); }

			static ::Uml::AssociationRole meta_ref;
			Udm::PointerAttr< ::SCAML::Interface> ref() const { return Udm::PointerAttr< ::SCAML::Interface>(impl, meta_ref); }

			static ::Uml::CompositionParentRole meta_uses_SoftwareComponent_parent;
			Udm::ParentAttr< ::SCAML::SoftwareComponent> uses_SoftwareComponent_parent() const { return Udm::ParentAttr< ::SCAML::SoftwareComponent>(impl, meta_uses_SoftwareComponent_parent); }

			Udm::ParentAttr< ::SCAML::componentplacement_Members_Base> componentplacement_Members_Base_parent() const { return Udm::ParentAttr< ::SCAML::componentplacement_Members_Base>(impl, Udm::NULLPARENTROLE); }
			Udm::ParentAttr< ::SCAML::MgaObject> MgaObject_parent() const { return Udm::ParentAttr< ::SCAML::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  providesPort :  public ::SCAML::connectdestBase, public ::SCAML::externaltodestBase, public ::SCAML::port {
		public:
			static ::Uml::Class meta;

			providesPort() { }
			providesPort(Udm::ObjectImpl *impl) : ::SCAML::connectdestBase(impl),::SCAML::externaltodestBase(impl),::SCAML::port(impl), ::SCAML::MgaObject(impl), UDM_OBJECT(impl) { }
			providesPort(const providesPort &master) : ::SCAML::connectdestBase(master),::SCAML::externaltodestBase(master),::SCAML::port(master), ::SCAML::MgaObject(master), UDM_OBJECT(master) { }
			static providesPort Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static providesPort Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			providesPort CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			providesPort CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::SCAML::providesPort> Instances() { return Udm::InstantiatedAttr< ::SCAML::providesPort>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::SCAML::providesPort, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::SCAML::providesPort, Pred>(impl);}

			Udm::DerivedAttr< ::SCAML::providesPort> Derived() { return Udm::DerivedAttr< ::SCAML::providesPort>(impl);}
			template <class Pred> Udm::DerivedAttr< ::SCAML::providesPort, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::SCAML::providesPort, Pred>(impl);}

			Udm::ArchetypeAttr< ::SCAML::providesPort> Archetype() const { return Udm::ArchetypeAttr< ::SCAML::providesPort>(impl);}

			static ::Uml::Attribute meta_providesname;
			Udm::StringAttr providesname() const { return Udm::StringAttr(impl, meta_providesname); }

			static ::Uml::AssociationRole meta_srcfreestandingtoprovidedConn;
			static ::Uml::AssociationRole meta_srcfreestandingtoprovidedConn_rev;
			Udm::AClassAssocAttr< ::SCAML::freestandingtoprovidedConn, ::SCAML::FreestandingPort> srcfreestandingtoprovidedConn() const { return Udm::AClassAssocAttr< ::SCAML::freestandingtoprovidedConn, ::SCAML::FreestandingPort>(impl, meta_srcfreestandingtoprovidedConn, meta_srcfreestandingtoprovidedConn_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::SCAML::freestandingtoprovidedConn, ::SCAML::FreestandingPort, Pred> srcfreestandingtoprovidedConn_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::SCAML::freestandingtoprovidedConn, ::SCAML::FreestandingPort, Pred>(impl, meta_srcfreestandingtoprovidedConn, meta_srcfreestandingtoprovidedConn_rev); }

			static ::Uml::AssociationRole meta_ref;
			Udm::PointerAttr< ::SCAML::Interface> ref() const { return Udm::PointerAttr< ::SCAML::Interface>(impl, meta_ref); }

			static ::Uml::CompositionParentRole meta_provides_SoftwareComponent_parent;
			Udm::ParentAttr< ::SCAML::SoftwareComponent> provides_SoftwareComponent_parent() const { return Udm::ParentAttr< ::SCAML::SoftwareComponent>(impl, meta_provides_SoftwareComponent_parent); }

			Udm::ParentAttr< ::SCAML::componentplacement_Members_Base> componentplacement_Members_Base_parent() const { return Udm::ParentAttr< ::SCAML::componentplacement_Members_Base>(impl, Udm::NULLPARENTROLE); }
			Udm::ParentAttr< ::SCAML::MgaObject> MgaObject_parent() const { return Udm::ParentAttr< ::SCAML::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  property :  public ::SCAML::CFSrc, public ::SCAML::DFFCO {
		public:
			static ::Uml::Class meta;

			property() { }
			property(Udm::ObjectImpl *impl) : ::SCAML::CFSrc(impl),::SCAML::DFFCO(impl), ::SCAML::MgaObject(impl), UDM_OBJECT(impl) { }
			property(const property &master) : ::SCAML::CFSrc(master),::SCAML::DFFCO(master), ::SCAML::MgaObject(master), UDM_OBJECT(master) { }
			static property Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static property Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			property CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			property CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::SCAML::property> Instances() { return Udm::InstantiatedAttr< ::SCAML::property>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::SCAML::property, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::SCAML::property, Pred>(impl);}

			Udm::DerivedAttr< ::SCAML::property> Derived() { return Udm::DerivedAttr< ::SCAML::property>(impl);}
			template <class Pred> Udm::DerivedAttr< ::SCAML::property, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::SCAML::property, Pred>(impl);}

			Udm::ArchetypeAttr< ::SCAML::property> Archetype() const { return Udm::ArchetypeAttr< ::SCAML::property>(impl);}

			static ::Uml::Attribute meta_id;
			Udm::StringAttr id() const { return Udm::StringAttr(impl, meta_id); }

			static ::Uml::Attribute meta_description;
			Udm::StringAttr description() const { return Udm::StringAttr(impl, meta_description); }

			static ::Uml::CompositionParentRole meta_properties_Properties_parent;
			Udm::ParentAttr< ::SCAML::Properties> properties_Properties_parent() const { return Udm::ParentAttr< ::SCAML::Properties>(impl, meta_properties_Properties_parent); }

			Udm::ParentAttr< ::SCAML::MgaObject> parent() const { return Udm::ParentAttr< ::SCAML::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  structBase :  public ::SCAML::property {
		public:
			static ::Uml::Class meta;

			structBase() { }
			structBase(Udm::ObjectImpl *impl) : ::SCAML::property(impl), ::SCAML::MgaObject(impl), UDM_OBJECT(impl) { }
			structBase(const structBase &master) : ::SCAML::property(master), ::SCAML::MgaObject(master), UDM_OBJECT(master) { }
			static structBase Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static structBase Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			structBase CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			structBase CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::SCAML::structBase> Instances() { return Udm::InstantiatedAttr< ::SCAML::structBase>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::SCAML::structBase, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::SCAML::structBase, Pred>(impl);}

			Udm::DerivedAttr< ::SCAML::structBase> Derived() { return Udm::DerivedAttr< ::SCAML::structBase>(impl);}
			template <class Pred> Udm::DerivedAttr< ::SCAML::structBase, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::SCAML::structBase, Pred>(impl);}

			Udm::ArchetypeAttr< ::SCAML::structBase> Archetype() const { return Udm::ArchetypeAttr< ::SCAML::structBase>(impl);}

			static ::Uml::Attribute meta_mode;
			Udm::StringAttr mode() const { return Udm::StringAttr(impl, meta_mode); }

			static ::Uml::Attribute meta_configurationkind;
			Udm::StringAttr configurationkind() const { return Udm::StringAttr(impl, meta_configurationkind); }

			Udm::ParentAttr< ::SCAML::MgaObject> parent() const { return Udm::ParentAttr< ::SCAML::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  simpleBase :  public ::SCAML::property {
		public:
			static ::Uml::Class meta;

			simpleBase() { }
			simpleBase(Udm::ObjectImpl *impl) : ::SCAML::property(impl), ::SCAML::MgaObject(impl), UDM_OBJECT(impl) { }
			simpleBase(const simpleBase &master) : ::SCAML::property(master), ::SCAML::MgaObject(master), UDM_OBJECT(master) { }
			static simpleBase Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static simpleBase Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			simpleBase CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			simpleBase CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::SCAML::simpleBase> Instances() { return Udm::InstantiatedAttr< ::SCAML::simpleBase>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::SCAML::simpleBase, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::SCAML::simpleBase, Pred>(impl);}

			Udm::DerivedAttr< ::SCAML::simpleBase> Derived() { return Udm::DerivedAttr< ::SCAML::simpleBase>(impl);}
			template <class Pred> Udm::DerivedAttr< ::SCAML::simpleBase, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::SCAML::simpleBase, Pred>(impl);}

			Udm::ArchetypeAttr< ::SCAML::simpleBase> Archetype() const { return Udm::ArchetypeAttr< ::SCAML::simpleBase>(impl);}

			static ::Uml::Attribute meta_range_min;
			Udm::StringAttr range_min() const { return Udm::StringAttr(impl, meta_range_min); }

			static ::Uml::Attribute meta_units;
			Udm::StringAttr units() const { return Udm::StringAttr(impl, meta_units); }

			static ::Uml::Attribute meta_range_max;
			Udm::StringAttr range_max() const { return Udm::StringAttr(impl, meta_range_max); }

			static ::Uml::Attribute meta_action;
			Udm::StringAttr action() const { return Udm::StringAttr(impl, meta_action); }

			static ::Uml::Attribute meta_mode;
			Udm::StringAttr mode() const { return Udm::StringAttr(impl, meta_mode); }

			static ::Uml::Attribute meta_type;
			Udm::StringAttr type() const { return Udm::StringAttr(impl, meta_type); }

			static ::Uml::CompositionChildRole meta_kind_children;
			Udm::ChildrenAttr< ::SCAML::kind> kind_children() const { return Udm::ChildrenAttr< ::SCAML::kind>(impl, meta_kind_children); }
			template <class Pred> Udm::ChildrenAttr< ::SCAML::kind, Pred> kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::kind, Pred>(impl, meta_kind_children); }

			Udm::ChildrenAttr< ::SCAML::kind> kind_kind_children() const { return Udm::ChildrenAttr< ::SCAML::kind>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::kind, Pred> kind_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::kind, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr< ::SCAML::MgaObject>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ParentAttr< ::SCAML::MgaObject> parent() const { return Udm::ParentAttr< ::SCAML::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  structsequencePropertyDescriptor :  public ::SCAML::propertyProxy, public ::SCAML::structBase {
		public:
			static ::Uml::Class meta;

			structsequencePropertyDescriptor() { }
			structsequencePropertyDescriptor(Udm::ObjectImpl *impl) : ::SCAML::propertyProxy(impl),::SCAML::structBase(impl), ::SCAML::MgaObject(impl), UDM_OBJECT(impl) { }
			structsequencePropertyDescriptor(const structsequencePropertyDescriptor &master) : ::SCAML::propertyProxy(master),::SCAML::structBase(master), ::SCAML::MgaObject(master), UDM_OBJECT(master) { }
			static structsequencePropertyDescriptor Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static structsequencePropertyDescriptor Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			structsequencePropertyDescriptor CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			structsequencePropertyDescriptor CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::SCAML::structsequencePropertyDescriptor> Instances() { return Udm::InstantiatedAttr< ::SCAML::structsequencePropertyDescriptor>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::SCAML::structsequencePropertyDescriptor, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::SCAML::structsequencePropertyDescriptor, Pred>(impl);}

			Udm::DerivedAttr< ::SCAML::structsequencePropertyDescriptor> Derived() { return Udm::DerivedAttr< ::SCAML::structsequencePropertyDescriptor>(impl);}
			template <class Pred> Udm::DerivedAttr< ::SCAML::structsequencePropertyDescriptor, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::SCAML::structsequencePropertyDescriptor, Pred>(impl);}

			Udm::ArchetypeAttr< ::SCAML::structsequencePropertyDescriptor> Archetype() const { return Udm::ArchetypeAttr< ::SCAML::structsequencePropertyDescriptor>(impl);}

			static ::Uml::CompositionChildRole meta_structref;
			Udm::ChildAttr< ::SCAML::structref> structref() const { return Udm::ChildAttr< ::SCAML::structref>(impl, meta_structref); }

			static ::Uml::CompositionChildRole meta_structvalue_children;
			Udm::ChildrenAttr< ::SCAML::structvalue> structvalue_children() const { return Udm::ChildrenAttr< ::SCAML::structvalue>(impl, meta_structvalue_children); }
			template <class Pred> Udm::ChildrenAttr< ::SCAML::structvalue, Pred> structvalue_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::structvalue, Pred>(impl, meta_structvalue_children); }

			Udm::ChildrenAttr< ::SCAML::structvalue> structvalue_kind_children() const { return Udm::ChildrenAttr< ::SCAML::structvalue>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::structvalue, Pred> structvalue_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::structvalue, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::structref> structref_kind_children() const { return Udm::ChildrenAttr< ::SCAML::structref>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::structref, Pred> structref_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::structref, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr< ::SCAML::MgaObject>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ParentAttr< ::SCAML::MgaObject> parent() const { return Udm::ParentAttr< ::SCAML::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  structPropertyDescriptor :  public ::SCAML::propertyProxy, public ::SCAML::structBase {
		public:
			static ::Uml::Class meta;

			structPropertyDescriptor() { }
			structPropertyDescriptor(Udm::ObjectImpl *impl) : ::SCAML::propertyProxy(impl),::SCAML::structBase(impl), ::SCAML::MgaObject(impl), UDM_OBJECT(impl) { }
			structPropertyDescriptor(const structPropertyDescriptor &master) : ::SCAML::propertyProxy(master),::SCAML::structBase(master), ::SCAML::MgaObject(master), UDM_OBJECT(master) { }
			static structPropertyDescriptor Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static structPropertyDescriptor Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			structPropertyDescriptor CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			structPropertyDescriptor CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::SCAML::structPropertyDescriptor> Instances() { return Udm::InstantiatedAttr< ::SCAML::structPropertyDescriptor>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::SCAML::structPropertyDescriptor, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::SCAML::structPropertyDescriptor, Pred>(impl);}

			Udm::DerivedAttr< ::SCAML::structPropertyDescriptor> Derived() { return Udm::DerivedAttr< ::SCAML::structPropertyDescriptor>(impl);}
			template <class Pred> Udm::DerivedAttr< ::SCAML::structPropertyDescriptor, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::SCAML::structPropertyDescriptor, Pred>(impl);}

			Udm::ArchetypeAttr< ::SCAML::structPropertyDescriptor> Archetype() const { return Udm::ArchetypeAttr< ::SCAML::structPropertyDescriptor>(impl);}

			static ::Uml::AssociationRole meta_referedbystructref;
			Udm::AssocAttr< ::SCAML::structref> referedbystructref() const { return Udm::AssocAttr< ::SCAML::structref>(impl, meta_referedbystructref); }
			template <class Pred> Udm::AssocAttr< ::SCAML::structref, Pred > referedbystructref_sorted(const Pred &) const { return Udm::AssocAttr< ::SCAML::structref, Pred>(impl, meta_referedbystructref); }

			static ::Uml::CompositionChildRole meta_simple;
			Udm::ChildrenAttr< ::SCAML::simplePropertyDescriptor> simple() const { return Udm::ChildrenAttr< ::SCAML::simplePropertyDescriptor>(impl, meta_simple); }
			template <class Pred> Udm::ChildrenAttr< ::SCAML::simplePropertyDescriptor, Pred> simple_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::simplePropertyDescriptor, Pred>(impl, meta_simple); }

			Udm::ChildrenAttr< ::SCAML::CFSrc> CFSrc_kind_children() const { return Udm::ChildrenAttr< ::SCAML::CFSrc>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::CFSrc, Pred> CFSrc_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::CFSrc, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::propertyProxy> propertyProxy_kind_children() const { return Udm::ChildrenAttr< ::SCAML::propertyProxy>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::propertyProxy, Pred> propertyProxy_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::propertyProxy, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::simpleBase> simpleBase_kind_children() const { return Udm::ChildrenAttr< ::SCAML::simpleBase>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::simpleBase, Pred> simpleBase_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::simpleBase, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::property> property_kind_children() const { return Udm::ChildrenAttr< ::SCAML::property>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::property, Pred> property_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::property, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::simplePropertyDescriptor> simplePropertyDescriptor_kind_children() const { return Udm::ChildrenAttr< ::SCAML::simplePropertyDescriptor>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::simplePropertyDescriptor, Pred> simplePropertyDescriptor_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::simplePropertyDescriptor, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::DFFCO> DFFCO_kind_children() const { return Udm::ChildrenAttr< ::SCAML::DFFCO>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::DFFCO, Pred> DFFCO_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::DFFCO, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr< ::SCAML::MgaObject>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ParentAttr< ::SCAML::MgaObject> parent() const { return Udm::ParentAttr< ::SCAML::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  testPropertyDescriptor :  public ::SCAML::property {
		public:
			static ::Uml::Class meta;

			testPropertyDescriptor() { }
			testPropertyDescriptor(Udm::ObjectImpl *impl) : ::SCAML::property(impl), ::SCAML::MgaObject(impl), UDM_OBJECT(impl) { }
			testPropertyDescriptor(const testPropertyDescriptor &master) : ::SCAML::property(master), ::SCAML::MgaObject(master), UDM_OBJECT(master) { }
			static testPropertyDescriptor Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static testPropertyDescriptor Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			testPropertyDescriptor CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			testPropertyDescriptor CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::SCAML::testPropertyDescriptor> Instances() { return Udm::InstantiatedAttr< ::SCAML::testPropertyDescriptor>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::SCAML::testPropertyDescriptor, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::SCAML::testPropertyDescriptor, Pred>(impl);}

			Udm::DerivedAttr< ::SCAML::testPropertyDescriptor> Derived() { return Udm::DerivedAttr< ::SCAML::testPropertyDescriptor>(impl);}
			template <class Pred> Udm::DerivedAttr< ::SCAML::testPropertyDescriptor, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::SCAML::testPropertyDescriptor, Pred>(impl);}

			Udm::ArchetypeAttr< ::SCAML::testPropertyDescriptor> Archetype() const { return Udm::ArchetypeAttr< ::SCAML::testPropertyDescriptor>(impl);}

			static ::Uml::CompositionChildRole meta_inputvalue;
			Udm::ChildrenAttr< ::SCAML::simplePropertyDescriptor> inputvalue() const { return Udm::ChildrenAttr< ::SCAML::simplePropertyDescriptor>(impl, meta_inputvalue); }
			template <class Pred> Udm::ChildrenAttr< ::SCAML::simplePropertyDescriptor, Pred> inputvalue_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::simplePropertyDescriptor, Pred>(impl, meta_inputvalue); }

			static ::Uml::CompositionChildRole meta_resultvalue;
			Udm::ChildrenAttr< ::SCAML::simplePropertyDescriptor> resultvalue() const { return Udm::ChildrenAttr< ::SCAML::simplePropertyDescriptor>(impl, meta_resultvalue); }
			template <class Pred> Udm::ChildrenAttr< ::SCAML::simplePropertyDescriptor, Pred> resultvalue_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::simplePropertyDescriptor, Pred>(impl, meta_resultvalue); }

			Udm::ChildrenAttr< ::SCAML::CFSrc> CFSrc_kind_children() const { return Udm::ChildrenAttr< ::SCAML::CFSrc>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::CFSrc, Pred> CFSrc_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::CFSrc, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::propertyProxy> propertyProxy_kind_children() const { return Udm::ChildrenAttr< ::SCAML::propertyProxy>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::propertyProxy, Pred> propertyProxy_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::propertyProxy, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::simpleBase> simpleBase_kind_children() const { return Udm::ChildrenAttr< ::SCAML::simpleBase>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::simpleBase, Pred> simpleBase_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::simpleBase, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::property> property_kind_children() const { return Udm::ChildrenAttr< ::SCAML::property>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::property, Pred> property_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::property, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::simplePropertyDescriptor> simplePropertyDescriptor_kind_children() const { return Udm::ChildrenAttr< ::SCAML::simplePropertyDescriptor>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::simplePropertyDescriptor, Pred> simplePropertyDescriptor_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::simplePropertyDescriptor, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::DFFCO> DFFCO_kind_children() const { return Udm::ChildrenAttr< ::SCAML::DFFCO>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::DFFCO, Pred> DFFCO_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::DFFCO, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr< ::SCAML::MgaObject>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ParentAttr< ::SCAML::MgaObject> parent() const { return Udm::ParentAttr< ::SCAML::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  simplesequencePropertyDescriptor :  public ::SCAML::propertyProxy, public ::SCAML::simpleBase {
		public:
			static ::Uml::Class meta;

			simplesequencePropertyDescriptor() { }
			simplesequencePropertyDescriptor(Udm::ObjectImpl *impl) : ::SCAML::propertyProxy(impl),::SCAML::simpleBase(impl), ::SCAML::MgaObject(impl), UDM_OBJECT(impl) { }
			simplesequencePropertyDescriptor(const simplesequencePropertyDescriptor &master) : ::SCAML::propertyProxy(master),::SCAML::simpleBase(master), ::SCAML::MgaObject(master), UDM_OBJECT(master) { }
			static simplesequencePropertyDescriptor Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static simplesequencePropertyDescriptor Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			simplesequencePropertyDescriptor CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			simplesequencePropertyDescriptor CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::SCAML::simplesequencePropertyDescriptor> Instances() { return Udm::InstantiatedAttr< ::SCAML::simplesequencePropertyDescriptor>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::SCAML::simplesequencePropertyDescriptor, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::SCAML::simplesequencePropertyDescriptor, Pred>(impl);}

			Udm::DerivedAttr< ::SCAML::simplesequencePropertyDescriptor> Derived() { return Udm::DerivedAttr< ::SCAML::simplesequencePropertyDescriptor>(impl);}
			template <class Pred> Udm::DerivedAttr< ::SCAML::simplesequencePropertyDescriptor, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::SCAML::simplesequencePropertyDescriptor, Pred>(impl);}

			Udm::ArchetypeAttr< ::SCAML::simplesequencePropertyDescriptor> Archetype() const { return Udm::ArchetypeAttr< ::SCAML::simplesequencePropertyDescriptor>(impl);}

			static ::Uml::Attribute meta_values;
			Udm::StringAttr values() const { return Udm::StringAttr(impl, meta_values); }

			Udm::ParentAttr< ::SCAML::MgaObject> parent() const { return Udm::ParentAttr< ::SCAML::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  simplePropertyDescriptor :  public ::SCAML::propertyProxy, public ::SCAML::simpleBase {
		public:
			static ::Uml::Class meta;

			simplePropertyDescriptor() { }
			simplePropertyDescriptor(Udm::ObjectImpl *impl) : ::SCAML::propertyProxy(impl),::SCAML::simpleBase(impl), ::SCAML::MgaObject(impl), UDM_OBJECT(impl) { }
			simplePropertyDescriptor(const simplePropertyDescriptor &master) : ::SCAML::propertyProxy(master),::SCAML::simpleBase(master), ::SCAML::MgaObject(master), UDM_OBJECT(master) { }
			static simplePropertyDescriptor Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static simplePropertyDescriptor Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			simplePropertyDescriptor CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			simplePropertyDescriptor CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::SCAML::simplePropertyDescriptor> Instances() { return Udm::InstantiatedAttr< ::SCAML::simplePropertyDescriptor>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::SCAML::simplePropertyDescriptor, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::SCAML::simplePropertyDescriptor, Pred>(impl);}

			Udm::DerivedAttr< ::SCAML::simplePropertyDescriptor> Derived() { return Udm::DerivedAttr< ::SCAML::simplePropertyDescriptor>(impl);}
			template <class Pred> Udm::DerivedAttr< ::SCAML::simplePropertyDescriptor, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::SCAML::simplePropertyDescriptor, Pred>(impl);}

			Udm::ArchetypeAttr< ::SCAML::simplePropertyDescriptor> Archetype() const { return Udm::ArchetypeAttr< ::SCAML::simplePropertyDescriptor>(impl);}

			static ::Uml::Attribute meta_value;
			Udm::StringAttr value() const { return Udm::StringAttr(impl, meta_value); }

			static ::Uml::AssociationRole meta_referedbystructvalue;
			Udm::AssocAttr< ::SCAML::simpleref> referedbystructvalue() const { return Udm::AssocAttr< ::SCAML::simpleref>(impl, meta_referedbystructvalue); }
			template <class Pred> Udm::AssocAttr< ::SCAML::simpleref, Pred > referedbystructvalue_sorted(const Pred &) const { return Udm::AssocAttr< ::SCAML::simpleref, Pred>(impl, meta_referedbystructvalue); }

			static ::Uml::AssociationRole meta_referedbysimplepropertyref;
			Udm::AssocAttr< ::SCAML::simplepropertyref> referedbysimplepropertyref() const { return Udm::AssocAttr< ::SCAML::simplepropertyref>(impl, meta_referedbysimplepropertyref); }
			template <class Pred> Udm::AssocAttr< ::SCAML::simplepropertyref, Pred > referedbysimplepropertyref_sorted(const Pred &) const { return Udm::AssocAttr< ::SCAML::simplepropertyref, Pred>(impl, meta_referedbysimplepropertyref); }

			static ::Uml::CompositionChildRole meta_enumerations;
			Udm::ChildrenAttr< ::SCAML::enumeration> enumerations() const { return Udm::ChildrenAttr< ::SCAML::enumeration>(impl, meta_enumerations); }
			template <class Pred> Udm::ChildrenAttr< ::SCAML::enumeration, Pred> enumerations_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::enumeration, Pred>(impl, meta_enumerations); }

			Udm::ChildrenAttr< ::SCAML::enumeration> enumeration_kind_children() const { return Udm::ChildrenAttr< ::SCAML::enumeration>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::enumeration, Pred> enumeration_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::enumeration, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr< ::SCAML::MgaObject>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

			static ::Uml::CompositionParentRole meta_resultvalue_testPropertyDescriptor_parent;
			Udm::ParentAttr< ::SCAML::testPropertyDescriptor> resultvalue_testPropertyDescriptor_parent() const { return Udm::ParentAttr< ::SCAML::testPropertyDescriptor>(impl, meta_resultvalue_testPropertyDescriptor_parent); }

			static ::Uml::CompositionParentRole meta_simple_structPropertyDescriptor_parent;
			Udm::ParentAttr< ::SCAML::structPropertyDescriptor> simple_structPropertyDescriptor_parent() const { return Udm::ParentAttr< ::SCAML::structPropertyDescriptor>(impl, meta_simple_structPropertyDescriptor_parent); }

			static ::Uml::CompositionParentRole meta_inputvalue_testPropertyDescriptor_parent;
			Udm::ParentAttr< ::SCAML::testPropertyDescriptor> inputvalue_testPropertyDescriptor_parent() const { return Udm::ParentAttr< ::SCAML::testPropertyDescriptor>(impl, meta_inputvalue_testPropertyDescriptor_parent); }

			Udm::ParentAttr< ::SCAML::MgaObject> parent() const { return Udm::ParentAttr< ::SCAML::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  DataPort :  public ::SCAML::ArgRef, public ::SCAML::DFFCO {
		public:
			static ::Uml::Class meta;

			DataPort() { }
			DataPort(Udm::ObjectImpl *impl) : ::SCAML::ArgRef(impl),::SCAML::DFFCO(impl), ::SCAML::MgaObject(impl), UDM_OBJECT(impl) { }
			DataPort(const DataPort &master) : ::SCAML::ArgRef(master),::SCAML::DFFCO(master), ::SCAML::MgaObject(master), UDM_OBJECT(master) { }
			static DataPort Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static DataPort Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			DataPort CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			DataPort CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::SCAML::DataPort> Instances() { return Udm::InstantiatedAttr< ::SCAML::DataPort>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::SCAML::DataPort, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::SCAML::DataPort, Pred>(impl);}

			Udm::DerivedAttr< ::SCAML::DataPort> Derived() { return Udm::DerivedAttr< ::SCAML::DataPort>(impl);}
			template <class Pred> Udm::DerivedAttr< ::SCAML::DataPort, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::SCAML::DataPort, Pred>(impl);}

			Udm::ArchetypeAttr< ::SCAML::DataPort> Archetype() const { return Udm::ArchetypeAttr< ::SCAML::DataPort>(impl);}

			static ::Uml::CompositionParentRole meta_SimulinkSystem_parent;
			Udm::ParentAttr< ::SCAML::SimulinkSystem> SimulinkSystem_parent() const { return Udm::ParentAttr< ::SCAML::SimulinkSystem>(impl, meta_SimulinkSystem_parent); }

			Udm::ParentAttr< ::SCAML::MgaObject> parent() const { return Udm::ParentAttr< ::SCAML::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  DataInport :  public ::SCAML::DataPort {
		public:
			static ::Uml::Class meta;

			DataInport() { }
			DataInport(Udm::ObjectImpl *impl) : ::SCAML::DataPort(impl), ::SCAML::MgaObject(impl), UDM_OBJECT(impl) { }
			DataInport(const DataInport &master) : ::SCAML::DataPort(master), ::SCAML::MgaObject(master), UDM_OBJECT(master) { }
			static DataInport Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static DataInport Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			DataInport CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			DataInport CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::SCAML::DataInport> Instances() { return Udm::InstantiatedAttr< ::SCAML::DataInport>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::SCAML::DataInport, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::SCAML::DataInport, Pred>(impl);}

			Udm::DerivedAttr< ::SCAML::DataInport> Derived() { return Udm::DerivedAttr< ::SCAML::DataInport>(impl);}
			template <class Pred> Udm::DerivedAttr< ::SCAML::DataInport, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::SCAML::DataInport, Pred>(impl);}

			Udm::ArchetypeAttr< ::SCAML::DataInport> Archetype() const { return Udm::ArchetypeAttr< ::SCAML::DataInport>(impl);}

			Udm::ParentAttr< ::SCAML::MgaObject> parent() const { return Udm::ParentAttr< ::SCAML::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  DataOutport :  public ::SCAML::DataPort {
		public:
			static ::Uml::Class meta;

			DataOutport() { }
			DataOutport(Udm::ObjectImpl *impl) : ::SCAML::DataPort(impl), ::SCAML::MgaObject(impl), UDM_OBJECT(impl) { }
			DataOutport(const DataOutport &master) : ::SCAML::DataPort(master), ::SCAML::MgaObject(master), UDM_OBJECT(master) { }
			static DataOutport Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static DataOutport Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			DataOutport CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			DataOutport CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::SCAML::DataOutport> Instances() { return Udm::InstantiatedAttr< ::SCAML::DataOutport>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::SCAML::DataOutport, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::SCAML::DataOutport, Pred>(impl);}

			Udm::DerivedAttr< ::SCAML::DataOutport> Derived() { return Udm::DerivedAttr< ::SCAML::DataOutport>(impl);}
			template <class Pred> Udm::DerivedAttr< ::SCAML::DataOutport, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::SCAML::DataOutport, Pred>(impl);}

			Udm::ArchetypeAttr< ::SCAML::DataOutport> Archetype() const { return Udm::ArchetypeAttr< ::SCAML::DataOutport>(impl);}

			Udm::ParentAttr< ::SCAML::MgaObject> parent() const { return Udm::ParentAttr< ::SCAML::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  WrapperPart :  virtual public ::SCAML::MgaObject {
		public:
			static ::Uml::Class meta;

			WrapperPart() { }
			WrapperPart(Udm::ObjectImpl *impl) : ::SCAML::MgaObject(impl), UDM_OBJECT(impl) { }
			WrapperPart(const WrapperPart &master) : ::SCAML::MgaObject(master), UDM_OBJECT(master) { }
			static WrapperPart Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static WrapperPart Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			WrapperPart CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			WrapperPart CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::SCAML::WrapperPart> Instances() { return Udm::InstantiatedAttr< ::SCAML::WrapperPart>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::SCAML::WrapperPart, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::SCAML::WrapperPart, Pred>(impl);}

			Udm::DerivedAttr< ::SCAML::WrapperPart> Derived() { return Udm::DerivedAttr< ::SCAML::WrapperPart>(impl);}
			template <class Pred> Udm::DerivedAttr< ::SCAML::WrapperPart, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::SCAML::WrapperPart, Pred>(impl);}

			Udm::ArchetypeAttr< ::SCAML::WrapperPart> Archetype() const { return Udm::ArchetypeAttr< ::SCAML::WrapperPart>(impl);}

			static ::Uml::CompositionParentRole meta_SimulinkImplementation_parent;
			Udm::ParentAttr< ::SCAML::SimulinkImplementation> SimulinkImplementation_parent() const { return Udm::ParentAttr< ::SCAML::SimulinkImplementation>(impl, meta_SimulinkImplementation_parent); }

			Udm::ParentAttr< ::SCAML::componentplacement_Members_Base> componentplacement_Members_Base_parent() const { return Udm::ParentAttr< ::SCAML::componentplacement_Members_Base>(impl, Udm::NULLPARENTROLE); }
			Udm::ParentAttr< ::SCAML::MgaObject> MgaObject_parent() const { return Udm::ParentAttr< ::SCAML::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  Event :  public ::SCAML::CFSrc, public ::SCAML::WrapperPart {
		public:
			static ::Uml::Class meta;

			Event() { }
			Event(Udm::ObjectImpl *impl) : ::SCAML::CFSrc(impl),::SCAML::WrapperPart(impl), ::SCAML::MgaObject(impl), UDM_OBJECT(impl) { }
			Event(const Event &master) : ::SCAML::CFSrc(master),::SCAML::WrapperPart(master), ::SCAML::MgaObject(master), UDM_OBJECT(master) { }
			static Event Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static Event Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			Event CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			Event CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::SCAML::Event> Instances() { return Udm::InstantiatedAttr< ::SCAML::Event>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::SCAML::Event, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::SCAML::Event, Pred>(impl);}

			Udm::DerivedAttr< ::SCAML::Event> Derived() { return Udm::DerivedAttr< ::SCAML::Event>(impl);}
			template <class Pred> Udm::DerivedAttr< ::SCAML::Event, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::SCAML::Event, Pred>(impl);}

			Udm::ArchetypeAttr< ::SCAML::Event> Archetype() const { return Udm::ArchetypeAttr< ::SCAML::Event>(impl);}

			Udm::ParentAttr< ::SCAML::componentplacement_Members_Base> componentplacement_Members_Base_parent() const { return Udm::ParentAttr< ::SCAML::componentplacement_Members_Base>(impl, Udm::NULLPARENTROLE); }
			Udm::ParentAttr< ::SCAML::MgaObject> MgaObject_parent() const { return Udm::ParentAttr< ::SCAML::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  ControlFlow :  public ::SCAML::WrapperPart {
		public:
			static ::Uml::Class meta;

			ControlFlow() { }
			ControlFlow(Udm::ObjectImpl *impl) : ::SCAML::WrapperPart(impl), ::SCAML::MgaObject(impl), UDM_OBJECT(impl) { }
			ControlFlow(const ControlFlow &master) : ::SCAML::WrapperPart(master), ::SCAML::MgaObject(master), UDM_OBJECT(master) { }
			static ControlFlow Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static ControlFlow Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			ControlFlow CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			ControlFlow CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::SCAML::ControlFlow> Instances() { return Udm::InstantiatedAttr< ::SCAML::ControlFlow>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::SCAML::ControlFlow, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::SCAML::ControlFlow, Pred>(impl);}

			Udm::DerivedAttr< ::SCAML::ControlFlow> Derived() { return Udm::DerivedAttr< ::SCAML::ControlFlow>(impl);}
			template <class Pred> Udm::DerivedAttr< ::SCAML::ControlFlow, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::SCAML::ControlFlow, Pred>(impl);}

			Udm::ArchetypeAttr< ::SCAML::ControlFlow> Archetype() const { return Udm::ArchetypeAttr< ::SCAML::ControlFlow>(impl);}

			Udm::ParentAttr< ::SCAML::componentplacement_Members_Base> componentplacement_Members_Base_parent() const { return Udm::ParentAttr< ::SCAML::componentplacement_Members_Base>(impl, Udm::NULLPARENTROLE); }
			Udm::ParentAttr< ::SCAML::MgaObject> MgaObject_parent() const { return Udm::ParentAttr< ::SCAML::MgaObject>(impl, Udm::NULLPARENTROLE); }
			static ::Uml::AssociationRole meta_dstControlFlow_end_;
			Udm::AssocEndAttr< ::SCAML::CFDst> dstControlFlow_end() const { return Udm::AssocEndAttr< ::SCAML::CFDst>(impl, meta_dstControlFlow_end_); }

			static ::Uml::AssociationRole meta_srcControlFlow_end_;
			Udm::AssocEndAttr< ::SCAML::CFSrc> srcControlFlow_end() const { return Udm::AssocEndAttr< ::SCAML::CFSrc>(impl, meta_srcControlFlow_end_); }

		};

		class  Branch :  public ::SCAML::CFDst, public ::SCAML::WrapperPart {
		public:
			static ::Uml::Class meta;

			Branch() { }
			Branch(Udm::ObjectImpl *impl) : ::SCAML::CFDst(impl),::SCAML::WrapperPart(impl), ::SCAML::MgaObject(impl), UDM_OBJECT(impl) { }
			Branch(const Branch &master) : ::SCAML::CFDst(master),::SCAML::WrapperPart(master), ::SCAML::MgaObject(master), UDM_OBJECT(master) { }
			static Branch Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static Branch Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			Branch CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			Branch CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::SCAML::Branch> Instances() { return Udm::InstantiatedAttr< ::SCAML::Branch>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::SCAML::Branch, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::SCAML::Branch, Pred>(impl);}

			Udm::DerivedAttr< ::SCAML::Branch> Derived() { return Udm::DerivedAttr< ::SCAML::Branch>(impl);}
			template <class Pred> Udm::DerivedAttr< ::SCAML::Branch, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::SCAML::Branch, Pred>(impl);}

			Udm::ArchetypeAttr< ::SCAML::Branch> Archetype() const { return Udm::ArchetypeAttr< ::SCAML::Branch>(impl);}

			static ::Uml::AssociationRole meta_dstElseFlow;
			static ::Uml::AssociationRole meta_dstElseFlow_rev;
			Udm::AClassAssocAttr< ::SCAML::ElseFlow, ::SCAML::CFDst> dstElseFlow() const { return Udm::AClassAssocAttr< ::SCAML::ElseFlow, ::SCAML::CFDst>(impl, meta_dstElseFlow, meta_dstElseFlow_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::SCAML::ElseFlow, ::SCAML::CFDst, Pred> dstElseFlow_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::SCAML::ElseFlow, ::SCAML::CFDst, Pred>(impl, meta_dstElseFlow, meta_dstElseFlow_rev); }

			static ::Uml::AssociationRole meta_dstThenFlow;
			static ::Uml::AssociationRole meta_dstThenFlow_rev;
			Udm::AClassAssocAttr< ::SCAML::ThenFlow, ::SCAML::CFDst> dstThenFlow() const { return Udm::AClassAssocAttr< ::SCAML::ThenFlow, ::SCAML::CFDst>(impl, meta_dstThenFlow, meta_dstThenFlow_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::SCAML::ThenFlow, ::SCAML::CFDst, Pred> dstThenFlow_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::SCAML::ThenFlow, ::SCAML::CFDst, Pred>(impl, meta_dstThenFlow, meta_dstThenFlow_rev); }

			Udm::ParentAttr< ::SCAML::componentplacement_Members_Base> componentplacement_Members_Base_parent() const { return Udm::ParentAttr< ::SCAML::componentplacement_Members_Base>(impl, Udm::NULLPARENTROLE); }
			Udm::ParentAttr< ::SCAML::MgaObject> MgaObject_parent() const { return Udm::ParentAttr< ::SCAML::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  ThenFlow :  public ::SCAML::WrapperPart {
		public:
			static ::Uml::Class meta;

			ThenFlow() { }
			ThenFlow(Udm::ObjectImpl *impl) : ::SCAML::WrapperPart(impl), ::SCAML::MgaObject(impl), UDM_OBJECT(impl) { }
			ThenFlow(const ThenFlow &master) : ::SCAML::WrapperPart(master), ::SCAML::MgaObject(master), UDM_OBJECT(master) { }
			static ThenFlow Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static ThenFlow Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			ThenFlow CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			ThenFlow CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::SCAML::ThenFlow> Instances() { return Udm::InstantiatedAttr< ::SCAML::ThenFlow>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::SCAML::ThenFlow, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::SCAML::ThenFlow, Pred>(impl);}

			Udm::DerivedAttr< ::SCAML::ThenFlow> Derived() { return Udm::DerivedAttr< ::SCAML::ThenFlow>(impl);}
			template <class Pred> Udm::DerivedAttr< ::SCAML::ThenFlow, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::SCAML::ThenFlow, Pred>(impl);}

			Udm::ArchetypeAttr< ::SCAML::ThenFlow> Archetype() const { return Udm::ArchetypeAttr< ::SCAML::ThenFlow>(impl);}

			Udm::ParentAttr< ::SCAML::componentplacement_Members_Base> componentplacement_Members_Base_parent() const { return Udm::ParentAttr< ::SCAML::componentplacement_Members_Base>(impl, Udm::NULLPARENTROLE); }
			Udm::ParentAttr< ::SCAML::MgaObject> MgaObject_parent() const { return Udm::ParentAttr< ::SCAML::MgaObject>(impl, Udm::NULLPARENTROLE); }
			static ::Uml::AssociationRole meta_srcThenFlow_end_;
			Udm::AssocEndAttr< ::SCAML::Branch> srcThenFlow_end() const { return Udm::AssocEndAttr< ::SCAML::Branch>(impl, meta_srcThenFlow_end_); }

			static ::Uml::AssociationRole meta_dstThenFlow_end_;
			Udm::AssocEndAttr< ::SCAML::CFDst> dstThenFlow_end() const { return Udm::AssocEndAttr< ::SCAML::CFDst>(impl, meta_dstThenFlow_end_); }

		};

		class  ElseFlow :  public ::SCAML::WrapperPart {
		public:
			static ::Uml::Class meta;

			ElseFlow() { }
			ElseFlow(Udm::ObjectImpl *impl) : ::SCAML::WrapperPart(impl), ::SCAML::MgaObject(impl), UDM_OBJECT(impl) { }
			ElseFlow(const ElseFlow &master) : ::SCAML::WrapperPart(master), ::SCAML::MgaObject(master), UDM_OBJECT(master) { }
			static ElseFlow Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static ElseFlow Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			ElseFlow CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			ElseFlow CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::SCAML::ElseFlow> Instances() { return Udm::InstantiatedAttr< ::SCAML::ElseFlow>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::SCAML::ElseFlow, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::SCAML::ElseFlow, Pred>(impl);}

			Udm::DerivedAttr< ::SCAML::ElseFlow> Derived() { return Udm::DerivedAttr< ::SCAML::ElseFlow>(impl);}
			template <class Pred> Udm::DerivedAttr< ::SCAML::ElseFlow, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::SCAML::ElseFlow, Pred>(impl);}

			Udm::ArchetypeAttr< ::SCAML::ElseFlow> Archetype() const { return Udm::ArchetypeAttr< ::SCAML::ElseFlow>(impl);}

			Udm::ParentAttr< ::SCAML::componentplacement_Members_Base> componentplacement_Members_Base_parent() const { return Udm::ParentAttr< ::SCAML::componentplacement_Members_Base>(impl, Udm::NULLPARENTROLE); }
			Udm::ParentAttr< ::SCAML::MgaObject> MgaObject_parent() const { return Udm::ParentAttr< ::SCAML::MgaObject>(impl, Udm::NULLPARENTROLE); }
			static ::Uml::AssociationRole meta_srcElseFlow_end_;
			Udm::AssocEndAttr< ::SCAML::Branch> srcElseFlow_end() const { return Udm::AssocEndAttr< ::SCAML::Branch>(impl, meta_srcElseFlow_end_); }

			static ::Uml::AssociationRole meta_dstElseFlow_end_;
			Udm::AssocEndAttr< ::SCAML::CFDst> dstElseFlow_end() const { return Udm::AssocEndAttr< ::SCAML::CFDst>(impl, meta_dstElseFlow_end_); }

		};

		class  DataFlow :  public ::SCAML::WrapperPart {
		public:
			static ::Uml::Class meta;

			DataFlow() { }
			DataFlow(Udm::ObjectImpl *impl) : ::SCAML::WrapperPart(impl), ::SCAML::MgaObject(impl), UDM_OBJECT(impl) { }
			DataFlow(const DataFlow &master) : ::SCAML::WrapperPart(master), ::SCAML::MgaObject(master), UDM_OBJECT(master) { }
			static DataFlow Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static DataFlow Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			DataFlow CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			DataFlow CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::SCAML::DataFlow> Instances() { return Udm::InstantiatedAttr< ::SCAML::DataFlow>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::SCAML::DataFlow, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::SCAML::DataFlow, Pred>(impl);}

			Udm::DerivedAttr< ::SCAML::DataFlow> Derived() { return Udm::DerivedAttr< ::SCAML::DataFlow>(impl);}
			template <class Pred> Udm::DerivedAttr< ::SCAML::DataFlow, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::SCAML::DataFlow, Pred>(impl);}

			Udm::ArchetypeAttr< ::SCAML::DataFlow> Archetype() const { return Udm::ArchetypeAttr< ::SCAML::DataFlow>(impl);}

			Udm::ParentAttr< ::SCAML::componentplacement_Members_Base> componentplacement_Members_Base_parent() const { return Udm::ParentAttr< ::SCAML::componentplacement_Members_Base>(impl, Udm::NULLPARENTROLE); }
			Udm::ParentAttr< ::SCAML::MgaObject> MgaObject_parent() const { return Udm::ParentAttr< ::SCAML::MgaObject>(impl, Udm::NULLPARENTROLE); }
			static ::Uml::AssociationRole meta_dstDataFlow_end_;
			Udm::AssocEndAttr< ::SCAML::DFFCO> dstDataFlow_end() const { return Udm::AssocEndAttr< ::SCAML::DFFCO>(impl, meta_dstDataFlow_end_); }

			static ::Uml::AssociationRole meta_srcDataFlow_end_;
			Udm::AssocEndAttr< ::SCAML::DFFCO> srcDataFlow_end() const { return Udm::AssocEndAttr< ::SCAML::DFFCO>(impl, meta_srcDataFlow_end_); }

		};

		class  Var :  public ::SCAML::WrapperPart {
		public:
			static ::Uml::Class meta;

			Var() { }
			Var(Udm::ObjectImpl *impl) : ::SCAML::WrapperPart(impl), ::SCAML::MgaObject(impl), UDM_OBJECT(impl) { }
			Var(const Var &master) : ::SCAML::WrapperPart(master), ::SCAML::MgaObject(master), UDM_OBJECT(master) { }
			static Var Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static Var Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			Var CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			Var CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::SCAML::Var> Instances() { return Udm::InstantiatedAttr< ::SCAML::Var>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::SCAML::Var, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::SCAML::Var, Pred>(impl);}

			Udm::DerivedAttr< ::SCAML::Var> Derived() { return Udm::DerivedAttr< ::SCAML::Var>(impl);}
			template <class Pred> Udm::DerivedAttr< ::SCAML::Var, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::SCAML::Var, Pred>(impl);}

			Udm::ArchetypeAttr< ::SCAML::Var> Archetype() const { return Udm::ArchetypeAttr< ::SCAML::Var>(impl);}

			Udm::ParentAttr< ::SCAML::componentplacement_Members_Base> componentplacement_Members_Base_parent() const { return Udm::ParentAttr< ::SCAML::componentplacement_Members_Base>(impl, Udm::NULLPARENTROLE); }
			Udm::ParentAttr< ::SCAML::MgaObject> MgaObject_parent() const { return Udm::ParentAttr< ::SCAML::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  Const :  public ::SCAML::WrapperPart {
		public:
			static ::Uml::Class meta;

			Const() { }
			Const(Udm::ObjectImpl *impl) : ::SCAML::WrapperPart(impl), ::SCAML::MgaObject(impl), UDM_OBJECT(impl) { }
			Const(const Const &master) : ::SCAML::WrapperPart(master), ::SCAML::MgaObject(master), UDM_OBJECT(master) { }
			static Const Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static Const Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			Const CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			Const CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::SCAML::Const> Instances() { return Udm::InstantiatedAttr< ::SCAML::Const>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::SCAML::Const, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::SCAML::Const, Pred>(impl);}

			Udm::DerivedAttr< ::SCAML::Const> Derived() { return Udm::DerivedAttr< ::SCAML::Const>(impl);}
			template <class Pred> Udm::DerivedAttr< ::SCAML::Const, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::SCAML::Const, Pred>(impl);}

			Udm::ArchetypeAttr< ::SCAML::Const> Archetype() const { return Udm::ArchetypeAttr< ::SCAML::Const>(impl);}

			Udm::ParentAttr< ::SCAML::componentplacement_Members_Base> componentplacement_Members_Base_parent() const { return Udm::ParentAttr< ::SCAML::componentplacement_Members_Base>(impl, Udm::NULLPARENTROLE); }
			Udm::ParentAttr< ::SCAML::MgaObject> MgaObject_parent() const { return Udm::ParentAttr< ::SCAML::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  Function :  public ::SCAML::WrapperPart {
		public:
			static ::Uml::Class meta;

			Function() { }
			Function(Udm::ObjectImpl *impl) : ::SCAML::WrapperPart(impl), ::SCAML::MgaObject(impl), UDM_OBJECT(impl) { }
			Function(const Function &master) : ::SCAML::WrapperPart(master), ::SCAML::MgaObject(master), UDM_OBJECT(master) { }
			static Function Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static Function Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			Function CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			Function CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::SCAML::Function> Instances() { return Udm::InstantiatedAttr< ::SCAML::Function>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::SCAML::Function, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::SCAML::Function, Pred>(impl);}

			Udm::DerivedAttr< ::SCAML::Function> Derived() { return Udm::DerivedAttr< ::SCAML::Function>(impl);}
			template <class Pred> Udm::DerivedAttr< ::SCAML::Function, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::SCAML::Function, Pred>(impl);}

			Udm::ArchetypeAttr< ::SCAML::Function> Archetype() const { return Udm::ArchetypeAttr< ::SCAML::Function>(impl);}

			Udm::ParentAttr< ::SCAML::componentplacement_Members_Base> componentplacement_Members_Base_parent() const { return Udm::ParentAttr< ::SCAML::componentplacement_Members_Base>(impl, Udm::NULLPARENTROLE); }
			Udm::ParentAttr< ::SCAML::MgaObject> MgaObject_parent() const { return Udm::ParentAttr< ::SCAML::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  DataMapping :  public ::SCAML::CFSrc, public ::SCAML::CFDst, public ::SCAML::DFFCO, public ::SCAML::WrapperPart {
		public:
			static ::Uml::Class meta;

			DataMapping() { }
			DataMapping(Udm::ObjectImpl *impl) : ::SCAML::CFSrc(impl),::SCAML::CFDst(impl),::SCAML::DFFCO(impl),::SCAML::WrapperPart(impl), ::SCAML::MgaObject(impl), UDM_OBJECT(impl) { }
			DataMapping(const DataMapping &master) : ::SCAML::CFSrc(master),::SCAML::CFDst(master),::SCAML::DFFCO(master),::SCAML::WrapperPart(master), ::SCAML::MgaObject(master), UDM_OBJECT(master) { }
			static DataMapping Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static DataMapping Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			DataMapping CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			DataMapping CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::SCAML::DataMapping> Instances() { return Udm::InstantiatedAttr< ::SCAML::DataMapping>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::SCAML::DataMapping, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::SCAML::DataMapping, Pred>(impl);}

			Udm::DerivedAttr< ::SCAML::DataMapping> Derived() { return Udm::DerivedAttr< ::SCAML::DataMapping>(impl);}
			template <class Pred> Udm::DerivedAttr< ::SCAML::DataMapping, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::SCAML::DataMapping, Pred>(impl);}

			Udm::ArchetypeAttr< ::SCAML::DataMapping> Archetype() const { return Udm::ArchetypeAttr< ::SCAML::DataMapping>(impl);}

			Udm::ParentAttr< ::SCAML::componentplacement_Members_Base> componentplacement_Members_Base_parent() const { return Udm::ParentAttr< ::SCAML::componentplacement_Members_Base>(impl, Udm::NULLPARENTROLE); }
			Udm::ParentAttr< ::SCAML::MgaObject> MgaObject_parent() const { return Udm::ParentAttr< ::SCAML::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  SimulinkVars :  public ::SCAML::ArgRef {
		public:
			static ::Uml::Class meta;

			SimulinkVars() { }
			SimulinkVars(Udm::ObjectImpl *impl) : ::SCAML::ArgRef(impl), ::SCAML::MgaObject(impl), UDM_OBJECT(impl) { }
			SimulinkVars(const SimulinkVars &master) : ::SCAML::ArgRef(master), ::SCAML::MgaObject(master), UDM_OBJECT(master) { }
			static SimulinkVars Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static SimulinkVars Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			SimulinkVars CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			SimulinkVars CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::SCAML::SimulinkVars> Instances() { return Udm::InstantiatedAttr< ::SCAML::SimulinkVars>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::SCAML::SimulinkVars, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::SCAML::SimulinkVars, Pred>(impl);}

			Udm::DerivedAttr< ::SCAML::SimulinkVars> Derived() { return Udm::DerivedAttr< ::SCAML::SimulinkVars>(impl);}
			template <class Pred> Udm::DerivedAttr< ::SCAML::SimulinkVars, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::SCAML::SimulinkVars, Pred>(impl);}

			Udm::ArchetypeAttr< ::SCAML::SimulinkVars> Archetype() const { return Udm::ArchetypeAttr< ::SCAML::SimulinkVars>(impl);}

			static ::Uml::CompositionParentRole meta_SimulinkSystem_parent;
			Udm::ParentAttr< ::SCAML::SimulinkSystem> SimulinkSystem_parent() const { return Udm::ParentAttr< ::SCAML::SimulinkSystem>(impl, meta_SimulinkSystem_parent); }

			Udm::ParentAttr< ::SCAML::MgaObject> parent() const { return Udm::ParentAttr< ::SCAML::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  propertyfile :  public ::SCAML::MgaObject {
		public:
			static ::Uml::Class meta;

			propertyfile() { }
			propertyfile(Udm::ObjectImpl *impl) : ::SCAML::MgaObject(impl), UDM_OBJECT(impl) { }
			propertyfile(const propertyfile &master) : ::SCAML::MgaObject(master), UDM_OBJECT(master) { }
			static propertyfile Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static propertyfile Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			propertyfile CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			propertyfile CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::SCAML::propertyfile> Instances() { return Udm::InstantiatedAttr< ::SCAML::propertyfile>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::SCAML::propertyfile, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::SCAML::propertyfile, Pred>(impl);}

			Udm::DerivedAttr< ::SCAML::propertyfile> Derived() { return Udm::DerivedAttr< ::SCAML::propertyfile>(impl);}
			template <class Pred> Udm::DerivedAttr< ::SCAML::propertyfile, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::SCAML::propertyfile, Pred>(impl);}

			Udm::ArchetypeAttr< ::SCAML::propertyfile> Archetype() const { return Udm::ArchetypeAttr< ::SCAML::propertyfile>(impl);}

			static ::Uml::Attribute meta_type;
			Udm::StringAttr type() const { return Udm::StringAttr(impl, meta_type); }

			static ::Uml::AssociationRole meta_ref;
			Udm::PointerAttr< ::SCAML::Properties> ref() const { return Udm::PointerAttr< ::SCAML::Properties>(impl, meta_ref); }

			static ::Uml::CompositionParentRole meta_hwdeviceregistration_parent;
			Udm::ParentAttr< ::SCAML::hwdeviceregistration> hwdeviceregistration_parent() const { return Udm::ParentAttr< ::SCAML::hwdeviceregistration>(impl, meta_hwdeviceregistration_parent); }

			static ::Uml::CompositionParentRole meta_implementation_parent;
			Udm::ParentAttr< ::SCAML::implementation> implementation_parent() const { return Udm::ParentAttr< ::SCAML::implementation>(impl, meta_implementation_parent); }

			static ::Uml::CompositionParentRole meta_SoftwareComponent_parent;
			Udm::ParentAttr< ::SCAML::SoftwareComponent> SoftwareComponent_parent() const { return Udm::ParentAttr< ::SCAML::SoftwareComponent>(impl, meta_SoftwareComponent_parent); }

			static ::Uml::CompositionParentRole meta_SoftPkg_parent;
			Udm::ParentAttr< ::SCAML::SoftPkg> SoftPkg_parent() const { return Udm::ParentAttr< ::SCAML::SoftPkg>(impl, meta_SoftPkg_parent); }

			Udm::ParentAttr< ::SCAML::MgaObject> parent() const { return Udm::ParentAttr< ::SCAML::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  SoftPkgRef :  public ::SCAML::MgaObject {
		public:
			static ::Uml::Class meta;

			SoftPkgRef() { }
			SoftPkgRef(Udm::ObjectImpl *impl) : ::SCAML::MgaObject(impl), UDM_OBJECT(impl) { }
			SoftPkgRef(const SoftPkgRef &master) : ::SCAML::MgaObject(master), UDM_OBJECT(master) { }
			static SoftPkgRef Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static SoftPkgRef Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			SoftPkgRef CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			SoftPkgRef CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::SCAML::SoftPkgRef> Instances() { return Udm::InstantiatedAttr< ::SCAML::SoftPkgRef>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::SCAML::SoftPkgRef, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::SCAML::SoftPkgRef, Pred>(impl);}

			Udm::DerivedAttr< ::SCAML::SoftPkgRef> Derived() { return Udm::DerivedAttr< ::SCAML::SoftPkgRef>(impl);}
			template <class Pred> Udm::DerivedAttr< ::SCAML::SoftPkgRef, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::SCAML::SoftPkgRef, Pred>(impl);}

			Udm::ArchetypeAttr< ::SCAML::SoftPkgRef> Archetype() const { return Udm::ArchetypeAttr< ::SCAML::SoftPkgRef>(impl);}

			static ::Uml::AssociationRole meta_ref;
			Udm::PointerAttr< ::SCAML::SoftPkg> ref() const { return Udm::PointerAttr< ::SCAML::SoftPkg>(impl, meta_ref); }

			static ::Uml::CompositionParentRole meta_DomainManagerConfiguration_parent;
			Udm::ParentAttr< ::SCAML::DomainManagerConfiguration> DomainManagerConfiguration_parent() const { return Udm::ParentAttr< ::SCAML::DomainManagerConfiguration>(impl, meta_DomainManagerConfiguration_parent); }

			Udm::ParentAttr< ::SCAML::MgaObject> parent() const { return Udm::ParentAttr< ::SCAML::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  SoftwareComponentRef :  public ::SCAML::MgaObject {
		public:
			static ::Uml::Class meta;

			SoftwareComponentRef() { }
			SoftwareComponentRef(Udm::ObjectImpl *impl) : ::SCAML::MgaObject(impl), UDM_OBJECT(impl) { }
			SoftwareComponentRef(const SoftwareComponentRef &master) : ::SCAML::MgaObject(master), UDM_OBJECT(master) { }
			static SoftwareComponentRef Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static SoftwareComponentRef Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			SoftwareComponentRef CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			SoftwareComponentRef CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::SCAML::SoftwareComponentRef> Instances() { return Udm::InstantiatedAttr< ::SCAML::SoftwareComponentRef>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::SCAML::SoftwareComponentRef, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::SCAML::SoftwareComponentRef, Pred>(impl);}

			Udm::DerivedAttr< ::SCAML::SoftwareComponentRef> Derived() { return Udm::DerivedAttr< ::SCAML::SoftwareComponentRef>(impl);}
			template <class Pred> Udm::DerivedAttr< ::SCAML::SoftwareComponentRef, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::SCAML::SoftwareComponentRef, Pred>(impl);}

			Udm::ArchetypeAttr< ::SCAML::SoftwareComponentRef> Archetype() const { return Udm::ArchetypeAttr< ::SCAML::SoftwareComponentRef>(impl);}

			static ::Uml::AssociationRole meta_ref;
			Udm::PointerAttr< ::SCAML::SoftwareComponent> ref() const { return Udm::PointerAttr< ::SCAML::SoftwareComponent>(impl, meta_ref); }

			static ::Uml::CompositionParentRole meta_DeviceConfiguration_parent;
			Udm::ParentAttr< ::SCAML::DeviceConfiguration> DeviceConfiguration_parent() const { return Udm::ParentAttr< ::SCAML::DeviceConfiguration>(impl, meta_DeviceConfiguration_parent); }

			static ::Uml::CompositionParentRole meta_descriptor_SoftPkg_parent;
			Udm::ParentAttr< ::SCAML::SoftPkg> descriptor_SoftPkg_parent() const { return Udm::ParentAttr< ::SCAML::SoftPkg>(impl, meta_descriptor_SoftPkg_parent); }

			Udm::ParentAttr< ::SCAML::MgaObject> parent() const { return Udm::ParentAttr< ::SCAML::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  DevicePkgRef :  public ::SCAML::MgaObject {
		public:
			static ::Uml::Class meta;

			DevicePkgRef() { }
			DevicePkgRef(Udm::ObjectImpl *impl) : ::SCAML::MgaObject(impl), UDM_OBJECT(impl) { }
			DevicePkgRef(const DevicePkgRef &master) : ::SCAML::MgaObject(master), UDM_OBJECT(master) { }
			static DevicePkgRef Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static DevicePkgRef Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			DevicePkgRef CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			DevicePkgRef CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::SCAML::DevicePkgRef> Instances() { return Udm::InstantiatedAttr< ::SCAML::DevicePkgRef>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::SCAML::DevicePkgRef, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::SCAML::DevicePkgRef, Pred>(impl);}

			Udm::DerivedAttr< ::SCAML::DevicePkgRef> Derived() { return Udm::DerivedAttr< ::SCAML::DevicePkgRef>(impl);}
			template <class Pred> Udm::DerivedAttr< ::SCAML::DevicePkgRef, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::SCAML::DevicePkgRef, Pred>(impl);}

			Udm::ArchetypeAttr< ::SCAML::DevicePkgRef> Archetype() const { return Udm::ArchetypeAttr< ::SCAML::DevicePkgRef>(impl);}

			static ::Uml::AssociationRole meta_ref;
			Udm::PointerAttr< ::SCAML::DevicePkg> ref() const { return Udm::PointerAttr< ::SCAML::DevicePkg>(impl, meta_ref); }

			static ::Uml::CompositionParentRole meta_DeviceConfiguration_parent;
			Udm::ParentAttr< ::SCAML::DeviceConfiguration> DeviceConfiguration_parent() const { return Udm::ParentAttr< ::SCAML::DeviceConfiguration>(impl, meta_DeviceConfiguration_parent); }

			Udm::ParentAttr< ::SCAML::MgaObject> parent() const { return Udm::ParentAttr< ::SCAML::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  SoftwarePackageRef :  public ::SCAML::MgaObject {
		public:
			static ::Uml::Class meta;

			SoftwarePackageRef() { }
			SoftwarePackageRef(Udm::ObjectImpl *impl) : ::SCAML::MgaObject(impl), UDM_OBJECT(impl) { }
			SoftwarePackageRef(const SoftwarePackageRef &master) : ::SCAML::MgaObject(master), UDM_OBJECT(master) { }
			static SoftwarePackageRef Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static SoftwarePackageRef Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			SoftwarePackageRef CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			SoftwarePackageRef CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::SCAML::SoftwarePackageRef> Instances() { return Udm::InstantiatedAttr< ::SCAML::SoftwarePackageRef>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::SCAML::SoftwarePackageRef, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::SCAML::SoftwarePackageRef, Pred>(impl);}

			Udm::DerivedAttr< ::SCAML::SoftwarePackageRef> Derived() { return Udm::DerivedAttr< ::SCAML::SoftwarePackageRef>(impl);}
			template <class Pred> Udm::DerivedAttr< ::SCAML::SoftwarePackageRef, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::SCAML::SoftwarePackageRef, Pred>(impl);}

			Udm::ArchetypeAttr< ::SCAML::SoftwarePackageRef> Archetype() const { return Udm::ArchetypeAttr< ::SCAML::SoftwarePackageRef>(impl);}

			static ::Uml::AssociationRole meta_ref;
			Udm::PointerAttr< ::SCAML::SoftPkg> ref() const { return Udm::PointerAttr< ::SCAML::SoftPkg>(impl, meta_ref); }

			static ::Uml::CompositionParentRole meta_DeviceConfiguration_parent;
			Udm::ParentAttr< ::SCAML::DeviceConfiguration> DeviceConfiguration_parent() const { return Udm::ParentAttr< ::SCAML::DeviceConfiguration>(impl, meta_DeviceConfiguration_parent); }

			static ::Uml::CompositionParentRole meta_SoftwareAssembly_parent;
			Udm::ParentAttr< ::SCAML::SoftwareAssembly> SoftwareAssembly_parent() const { return Udm::ParentAttr< ::SCAML::SoftwareAssembly>(impl, meta_SoftwareAssembly_parent); }

			Udm::ParentAttr< ::SCAML::MgaObject> parent() const { return Udm::ParentAttr< ::SCAML::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  SoftwareComponent :  public ::SCAML::componentplacement_Members_Base, virtual public ::SCAML::MgaObject {
		public:
			static ::Uml::Class meta;

			SoftwareComponent() { }
			SoftwareComponent(Udm::ObjectImpl *impl) : ::SCAML::componentplacement_Members_Base(impl),::SCAML::MgaObject(impl), UDM_OBJECT(impl) { }
			SoftwareComponent(const SoftwareComponent &master) : ::SCAML::componentplacement_Members_Base(master),::SCAML::MgaObject(master), UDM_OBJECT(master) { }
			static SoftwareComponent Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static SoftwareComponent Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			SoftwareComponent CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			SoftwareComponent CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::SCAML::SoftwareComponent> Instances() { return Udm::InstantiatedAttr< ::SCAML::SoftwareComponent>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::SCAML::SoftwareComponent, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::SCAML::SoftwareComponent, Pred>(impl);}

			Udm::DerivedAttr< ::SCAML::SoftwareComponent> Derived() { return Udm::DerivedAttr< ::SCAML::SoftwareComponent>(impl);}
			template <class Pred> Udm::DerivedAttr< ::SCAML::SoftwareComponent, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::SCAML::SoftwareComponent, Pred>(impl);}

			Udm::ArchetypeAttr< ::SCAML::SoftwareComponent> Archetype() const { return Udm::ArchetypeAttr< ::SCAML::SoftwareComponent>(impl);}

			static ::Uml::Attribute meta_componentrepid;
			Udm::StringAttr componentrepid() const { return Udm::StringAttr(impl, meta_componentrepid); }

			static ::Uml::Attribute meta_corbaversion;
			Udm::StringAttr corbaversion() const { return Udm::StringAttr(impl, meta_corbaversion); }

			static ::Uml::Attribute meta_componenttype;
			Udm::StringAttr componenttype() const { return Udm::StringAttr(impl, meta_componenttype); }

			static ::Uml::Attribute meta_usagename;
			Udm::StringAttr usagename() const { return Udm::StringAttr(impl, meta_usagename); }

			static ::Uml::AssociationRole meta_srccontrollertocompinst;
			static ::Uml::AssociationRole meta_srccontrollertocompinst_rev;
			Udm::AClassAssocAttr< ::SCAML::controllertocompinst, ::SCAML::assemblycontroller> srccontrollertocompinst() const { return Udm::AClassAssocAttr< ::SCAML::controllertocompinst, ::SCAML::assemblycontroller>(impl, meta_srccontrollertocompinst, meta_srccontrollertocompinst_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::SCAML::controllertocompinst, ::SCAML::assemblycontroller, Pred> srccontrollertocompinst_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::SCAML::controllertocompinst, ::SCAML::assemblycontroller, Pred>(impl, meta_srccontrollertocompinst, meta_srccontrollertocompinst_rev); }

			static ::Uml::AssociationRole meta_srcfindcomponentConn;
			static ::Uml::AssociationRole meta_srcfindcomponentConn_rev;
			Udm::AClassAssocAttr< ::SCAML::findcomponentConn, ::SCAML::componentresourcefactoryref> srcfindcomponentConn() const { return Udm::AClassAssocAttr< ::SCAML::findcomponentConn, ::SCAML::componentresourcefactoryref>(impl, meta_srcfindcomponentConn, meta_srcfindcomponentConn_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::SCAML::findcomponentConn, ::SCAML::componentresourcefactoryref, Pred> srcfindcomponentConn_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::SCAML::findcomponentConn, ::SCAML::componentresourcefactoryref, Pred>(impl, meta_srcfindcomponentConn, meta_srcfindcomponentConn_rev); }

			static ::Uml::AssociationRole meta_referedbycomponentresourcefactoryref;
			Udm::AssocAttr< ::SCAML::componentresourcefactoryref> referedbycomponentresourcefactoryref() const { return Udm::AssocAttr< ::SCAML::componentresourcefactoryref>(impl, meta_referedbycomponentresourcefactoryref); }
			template <class Pred> Udm::AssocAttr< ::SCAML::componentresourcefactoryref, Pred > referedbycomponentresourcefactoryref_sorted(const Pred &) const { return Udm::AssocAttr< ::SCAML::componentresourcefactoryref, Pred>(impl, meta_referedbycomponentresourcefactoryref); }

			static ::Uml::AssociationRole meta_dstcomponentpropertiesConn;
			static ::Uml::AssociationRole meta_dstcomponentpropertiesConn_rev;
			Udm::AClassAssocAttr< ::SCAML::componentpropertiesConn, ::SCAML::componentproperties> dstcomponentpropertiesConn() const { return Udm::AClassAssocAttr< ::SCAML::componentpropertiesConn, ::SCAML::componentproperties>(impl, meta_dstcomponentpropertiesConn, meta_dstcomponentpropertiesConn_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::SCAML::componentpropertiesConn, ::SCAML::componentproperties, Pred> dstcomponentpropertiesConn_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::SCAML::componentpropertiesConn, ::SCAML::componentproperties, Pred>(impl, meta_dstcomponentpropertiesConn, meta_dstcomponentpropertiesConn_rev); }

			static ::Uml::AssociationRole meta_sethostcollocation;
			Udm::AssocAttr< ::SCAML::hostcollocation> sethostcollocation() const { return Udm::AssocAttr< ::SCAML::hostcollocation>(impl, meta_sethostcollocation); }
			template <class Pred> Udm::AssocAttr< ::SCAML::hostcollocation, Pred > sethostcollocation_sorted(const Pred &) const { return Udm::AssocAttr< ::SCAML::hostcollocation, Pred>(impl, meta_sethostcollocation); }

			static ::Uml::AssociationRole meta_srccomponentplacementConn;
			static ::Uml::AssociationRole meta_srccomponentplacementConn_rev;
			Udm::AClassAssocAttr< ::SCAML::componentplacementConn, ::SCAML::componentfile> srccomponentplacementConn() const { return Udm::AClassAssocAttr< ::SCAML::componentplacementConn, ::SCAML::componentfile>(impl, meta_srccomponentplacementConn, meta_srccomponentplacementConn_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::SCAML::componentplacementConn, ::SCAML::componentfile, Pred> srccomponentplacementConn_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::SCAML::componentplacementConn, ::SCAML::componentfile, Pred>(impl, meta_srccomponentplacementConn, meta_srccomponentplacementConn_rev); }

			static ::Uml::AssociationRole meta_referedbycompositepartofdevice;
			Udm::AssocAttr< ::SCAML::compositepartofdevice> referedbycompositepartofdevice() const { return Udm::AssocAttr< ::SCAML::compositepartofdevice>(impl, meta_referedbycompositepartofdevice); }
			template <class Pred> Udm::AssocAttr< ::SCAML::compositepartofdevice, Pred > referedbycompositepartofdevice_sorted(const Pred &) const { return Udm::AssocAttr< ::SCAML::compositepartofdevice, Pred>(impl, meta_referedbycompositepartofdevice); }

			static ::Uml::AssociationRole meta_referedbydeployondevice;
			Udm::AssocAttr< ::SCAML::deployondevice> referedbydeployondevice() const { return Udm::AssocAttr< ::SCAML::deployondevice>(impl, meta_referedbydeployondevice); }
			template <class Pred> Udm::AssocAttr< ::SCAML::deployondevice, Pred > referedbydeployondevice_sorted(const Pred &) const { return Udm::AssocAttr< ::SCAML::deployondevice, Pred>(impl, meta_referedbydeployondevice); }

			static ::Uml::AssociationRole meta_referedbySoftwareComponentRef;
			Udm::AssocAttr< ::SCAML::SoftwareComponentRef> referedbySoftwareComponentRef() const { return Udm::AssocAttr< ::SCAML::SoftwareComponentRef>(impl, meta_referedbySoftwareComponentRef); }
			template <class Pred> Udm::AssocAttr< ::SCAML::SoftwareComponentRef, Pred > referedbySoftwareComponentRef_sorted(const Pred &) const { return Udm::AssocAttr< ::SCAML::SoftwareComponentRef, Pred>(impl, meta_referedbySoftwareComponentRef); }

			static ::Uml::CompositionChildRole meta_supports_children;
			Udm::ChildrenAttr< ::SCAML::supports> supports_children() const { return Udm::ChildrenAttr< ::SCAML::supports>(impl, meta_supports_children); }
			template <class Pred> Udm::ChildrenAttr< ::SCAML::supports, Pred> supports_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::supports, Pred>(impl, meta_supports_children); }

			static ::Uml::CompositionChildRole meta_propertyfile_child;
			Udm::ChildAttr< ::SCAML::propertyfile> propertyfile_child() const { return Udm::ChildAttr< ::SCAML::propertyfile>(impl, meta_propertyfile_child); }

			static ::Uml::CompositionChildRole meta_uses;
			Udm::ChildrenAttr< ::SCAML::usesPort> uses() const { return Udm::ChildrenAttr< ::SCAML::usesPort>(impl, meta_uses); }
			template <class Pred> Udm::ChildrenAttr< ::SCAML::usesPort, Pred> uses_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::usesPort, Pred>(impl, meta_uses); }

			static ::Uml::CompositionChildRole meta_provides;
			Udm::ChildrenAttr< ::SCAML::providesPort> provides() const { return Udm::ChildrenAttr< ::SCAML::providesPort>(impl, meta_provides); }
			template <class Pred> Udm::ChildrenAttr< ::SCAML::providesPort, Pred> provides_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::providesPort, Pred>(impl, meta_provides); }

			Udm::ChildrenAttr< ::SCAML::connectdestBase> connectdestBase_kind_children() const { return Udm::ChildrenAttr< ::SCAML::connectdestBase>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::connectdestBase, Pred> connectdestBase_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::connectdestBase, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::externaltodestBase> externaltodestBase_kind_children() const { return Udm::ChildrenAttr< ::SCAML::externaltodestBase>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::externaltodestBase, Pred> externaltodestBase_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::externaltodestBase, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::CFSrc> CFSrc_kind_children() const { return Udm::ChildrenAttr< ::SCAML::CFSrc>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::CFSrc, Pred> CFSrc_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::CFSrc, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::CFDst> CFDst_kind_children() const { return Udm::ChildrenAttr< ::SCAML::CFDst>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::CFDst, Pred> CFDst_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::CFDst, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::usesPort> usesPort_kind_children() const { return Udm::ChildrenAttr< ::SCAML::usesPort>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::usesPort, Pred> usesPort_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::usesPort, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::providesPort> providesPort_kind_children() const { return Udm::ChildrenAttr< ::SCAML::providesPort>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::providesPort, Pred> providesPort_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::providesPort, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::supports> supports_kind_children() const { return Udm::ChildrenAttr< ::SCAML::supports>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::supports, Pred> supports_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::supports, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::port> port_kind_children() const { return Udm::ChildrenAttr< ::SCAML::port>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::port, Pred> port_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::port, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::componentfeatures> componentfeatures_kind_children() const { return Udm::ChildrenAttr< ::SCAML::componentfeatures>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::componentfeatures, Pred> componentfeatures_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::componentfeatures, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::DFFCO> DFFCO_kind_children() const { return Udm::ChildrenAttr< ::SCAML::DFFCO>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::DFFCO, Pred> DFFCO_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::DFFCO, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::propertyfile> propertyfile_kind_children() const { return Udm::ChildrenAttr< ::SCAML::propertyfile>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::propertyfile, Pred> propertyfile_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::propertyfile, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr< ::SCAML::MgaObject>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

			static ::Uml::CompositionParentRole meta_SoftwareComponentFolder_parent;
			Udm::ParentAttr< ::SCAML::SoftwareComponentFolder> SoftwareComponentFolder_parent() const { return Udm::ParentAttr< ::SCAML::SoftwareComponentFolder>(impl, meta_SoftwareComponentFolder_parent); }

			static ::Uml::CompositionParentRole meta_SoftwareAssembly_parent;
			Udm::ParentAttr< ::SCAML::SoftwareAssembly> SoftwareAssembly_parent() const { return Udm::ParentAttr< ::SCAML::SoftwareAssembly>(impl, meta_SoftwareAssembly_parent); }

			static ::Uml::CompositionParentRole meta_DeviceConfiguration_parent;
			Udm::ParentAttr< ::SCAML::DeviceConfiguration> DeviceConfiguration_parent() const { return Udm::ParentAttr< ::SCAML::DeviceConfiguration>(impl, meta_DeviceConfiguration_parent); }

			Udm::ParentAttr<Udm::Object> parent() const { return Udm::ParentAttr<Udm::Object>(impl, Udm::NULLPARENTROLE); }
		};

		class  SimulinkImplementation :  public ::SCAML::implementation, public ::SCAML::SoftwareComponent {
		public:
			static ::Uml::Class meta;

			SimulinkImplementation() { }
			SimulinkImplementation(Udm::ObjectImpl *impl) : ::SCAML::implementation(impl),::SCAML::SoftwareComponent(impl), ::SCAML::MgaObject(impl), UDM_OBJECT(impl) { }
			SimulinkImplementation(const SimulinkImplementation &master) : ::SCAML::implementation(master),::SCAML::SoftwareComponent(master), ::SCAML::MgaObject(master), UDM_OBJECT(master) { }
			static SimulinkImplementation Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static SimulinkImplementation Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			SimulinkImplementation CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			SimulinkImplementation CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::SCAML::SimulinkImplementation> Instances() { return Udm::InstantiatedAttr< ::SCAML::SimulinkImplementation>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::SCAML::SimulinkImplementation, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::SCAML::SimulinkImplementation, Pred>(impl);}

			Udm::DerivedAttr< ::SCAML::SimulinkImplementation> Derived() { return Udm::DerivedAttr< ::SCAML::SimulinkImplementation>(impl);}
			template <class Pred> Udm::DerivedAttr< ::SCAML::SimulinkImplementation, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::SCAML::SimulinkImplementation, Pred>(impl);}

			Udm::ArchetypeAttr< ::SCAML::SimulinkImplementation> Archetype() const { return Udm::ArchetypeAttr< ::SCAML::SimulinkImplementation>(impl);}

			static ::Uml::CompositionChildRole meta_WrapperPart_children;
			Udm::ChildrenAttr< ::SCAML::WrapperPart> WrapperPart_children() const { return Udm::ChildrenAttr< ::SCAML::WrapperPart>(impl, meta_WrapperPart_children); }
			template <class Pred> Udm::ChildrenAttr< ::SCAML::WrapperPart, Pred> WrapperPart_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::WrapperPart, Pred>(impl, meta_WrapperPart_children); }

			static ::Uml::CompositionChildRole meta_SimulinkSystem_children;
			Udm::ChildrenAttr< ::SCAML::SimulinkSystem> SimulinkSystem_children() const { return Udm::ChildrenAttr< ::SCAML::SimulinkSystem>(impl, meta_SimulinkSystem_children); }
			template <class Pred> Udm::ChildrenAttr< ::SCAML::SimulinkSystem, Pred> SimulinkSystem_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::SimulinkSystem, Pred>(impl, meta_SimulinkSystem_children); }

			Udm::ChildrenAttr< ::SCAML::Event> Event_kind_children() const { return Udm::ChildrenAttr< ::SCAML::Event>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::Event, Pred> Event_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::Event, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::ControlFlow> ControlFlow_kind_children() const { return Udm::ChildrenAttr< ::SCAML::ControlFlow>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::ControlFlow, Pred> ControlFlow_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::ControlFlow, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::Branch> Branch_kind_children() const { return Udm::ChildrenAttr< ::SCAML::Branch>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::Branch, Pred> Branch_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::Branch, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::ThenFlow> ThenFlow_kind_children() const { return Udm::ChildrenAttr< ::SCAML::ThenFlow>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::ThenFlow, Pred> ThenFlow_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::ThenFlow, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::ElseFlow> ElseFlow_kind_children() const { return Udm::ChildrenAttr< ::SCAML::ElseFlow>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::ElseFlow, Pred> ElseFlow_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::ElseFlow, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::SimulinkSystem> SimulinkSystem_kind_children() const { return Udm::ChildrenAttr< ::SCAML::SimulinkSystem>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::SimulinkSystem, Pred> SimulinkSystem_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::SimulinkSystem, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::DataFlow> DataFlow_kind_children() const { return Udm::ChildrenAttr< ::SCAML::DataFlow>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::DataFlow, Pred> DataFlow_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::DataFlow, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::Var> Var_kind_children() const { return Udm::ChildrenAttr< ::SCAML::Var>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::Var, Pred> Var_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::Var, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::Const> Const_kind_children() const { return Udm::ChildrenAttr< ::SCAML::Const>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::Const, Pred> Const_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::Const, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::Function> Function_kind_children() const { return Udm::ChildrenAttr< ::SCAML::Function>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::Function, Pred> Function_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::Function, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::DataMapping> DataMapping_kind_children() const { return Udm::ChildrenAttr< ::SCAML::DataMapping>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::DataMapping, Pred> DataMapping_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::DataMapping, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::WrapperPart> WrapperPart_kind_children() const { return Udm::ChildrenAttr< ::SCAML::WrapperPart>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::WrapperPart, Pred> WrapperPart_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::WrapperPart, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr< ::SCAML::MgaObject>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ParentAttr<Udm::Object> parent() const { return Udm::ParentAttr<Udm::Object>(impl, Udm::NULLPARENTROLE); }
		};

		class  Properties :  public ::SCAML::MgaObject {
		public:
			static ::Uml::Class meta;

			Properties() { }
			Properties(Udm::ObjectImpl *impl) : ::SCAML::MgaObject(impl), UDM_OBJECT(impl) { }
			Properties(const Properties &master) : ::SCAML::MgaObject(master), UDM_OBJECT(master) { }
			static Properties Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static Properties Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			Properties CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			Properties CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::SCAML::Properties> Instances() { return Udm::InstantiatedAttr< ::SCAML::Properties>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::SCAML::Properties, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::SCAML::Properties, Pred>(impl);}

			Udm::DerivedAttr< ::SCAML::Properties> Derived() { return Udm::DerivedAttr< ::SCAML::Properties>(impl);}
			template <class Pred> Udm::DerivedAttr< ::SCAML::Properties, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::SCAML::Properties, Pred>(impl);}

			Udm::ArchetypeAttr< ::SCAML::Properties> Archetype() const { return Udm::ArchetypeAttr< ::SCAML::Properties>(impl);}

			static ::Uml::AssociationRole meta_referedbypropertyref;
			Udm::AssocAttr< ::SCAML::propertyref> referedbypropertyref() const { return Udm::AssocAttr< ::SCAML::propertyref>(impl, meta_referedbypropertyref); }
			template <class Pred> Udm::AssocAttr< ::SCAML::propertyref, Pred > referedbypropertyref_sorted(const Pred &) const { return Udm::AssocAttr< ::SCAML::propertyref, Pred>(impl, meta_referedbypropertyref); }

			static ::Uml::AssociationRole meta_referedbypropertyfile;
			Udm::AssocAttr< ::SCAML::propertyfile> referedbypropertyfile() const { return Udm::AssocAttr< ::SCAML::propertyfile>(impl, meta_referedbypropertyfile); }
			template <class Pred> Udm::AssocAttr< ::SCAML::propertyfile, Pred > referedbypropertyfile_sorted(const Pred &) const { return Udm::AssocAttr< ::SCAML::propertyfile, Pred>(impl, meta_referedbypropertyfile); }

			static ::Uml::CompositionChildRole meta_properties;
			Udm::ChildrenAttr< ::SCAML::property> properties() const { return Udm::ChildrenAttr< ::SCAML::property>(impl, meta_properties); }
			template <class Pred> Udm::ChildrenAttr< ::SCAML::property, Pred> properties_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::property, Pred>(impl, meta_properties); }

			Udm::ChildrenAttr< ::SCAML::CFSrc> CFSrc_kind_children() const { return Udm::ChildrenAttr< ::SCAML::CFSrc>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::CFSrc, Pred> CFSrc_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::CFSrc, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::structBase> structBase_kind_children() const { return Udm::ChildrenAttr< ::SCAML::structBase>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::structBase, Pred> structBase_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::structBase, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::simpleBase> simpleBase_kind_children() const { return Udm::ChildrenAttr< ::SCAML::simpleBase>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::simpleBase, Pred> simpleBase_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::simpleBase, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::structsequencePropertyDescriptor> structsequencePropertyDescriptor_kind_children() const { return Udm::ChildrenAttr< ::SCAML::structsequencePropertyDescriptor>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::structsequencePropertyDescriptor, Pred> structsequencePropertyDescriptor_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::structsequencePropertyDescriptor, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::structPropertyDescriptor> structPropertyDescriptor_kind_children() const { return Udm::ChildrenAttr< ::SCAML::structPropertyDescriptor>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::structPropertyDescriptor, Pred> structPropertyDescriptor_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::structPropertyDescriptor, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::testPropertyDescriptor> testPropertyDescriptor_kind_children() const { return Udm::ChildrenAttr< ::SCAML::testPropertyDescriptor>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::testPropertyDescriptor, Pred> testPropertyDescriptor_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::testPropertyDescriptor, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::simplesequencePropertyDescriptor> simplesequencePropertyDescriptor_kind_children() const { return Udm::ChildrenAttr< ::SCAML::simplesequencePropertyDescriptor>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::simplesequencePropertyDescriptor, Pred> simplesequencePropertyDescriptor_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::simplesequencePropertyDescriptor, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::property> property_kind_children() const { return Udm::ChildrenAttr< ::SCAML::property>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::property, Pred> property_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::property, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::simplePropertyDescriptor> simplePropertyDescriptor_kind_children() const { return Udm::ChildrenAttr< ::SCAML::simplePropertyDescriptor>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::simplePropertyDescriptor, Pred> simplePropertyDescriptor_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::simplePropertyDescriptor, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::DFFCO> DFFCO_kind_children() const { return Udm::ChildrenAttr< ::SCAML::DFFCO>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::DFFCO, Pred> DFFCO_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::DFFCO, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr< ::SCAML::MgaObject>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

			static ::Uml::CompositionParentRole meta_PropertiesFolder_parent;
			Udm::ParentAttr< ::SCAML::PropertiesFolder> PropertiesFolder_parent() const { return Udm::ParentAttr< ::SCAML::PropertiesFolder>(impl, meta_PropertiesFolder_parent); }

			Udm::ParentAttr< ::SCAML::PropertiesFolder> parent() const { return Udm::ParentAttr< ::SCAML::PropertiesFolder>(impl, Udm::NULLPARENTROLE); }
		};

		class  SoftPkg :  public ::SCAML::MgaObject {
		public:
			static ::Uml::Class meta;

			SoftPkg() { }
			SoftPkg(Udm::ObjectImpl *impl) : ::SCAML::MgaObject(impl), UDM_OBJECT(impl) { }
			SoftPkg(const SoftPkg &master) : ::SCAML::MgaObject(master), UDM_OBJECT(master) { }
			static SoftPkg Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static SoftPkg Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			SoftPkg CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			SoftPkg CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::SCAML::SoftPkg> Instances() { return Udm::InstantiatedAttr< ::SCAML::SoftPkg>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::SCAML::SoftPkg, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::SCAML::SoftPkg, Pred>(impl);}

			Udm::DerivedAttr< ::SCAML::SoftPkg> Derived() { return Udm::DerivedAttr< ::SCAML::SoftPkg>(impl);}
			template <class Pred> Udm::DerivedAttr< ::SCAML::SoftPkg, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::SCAML::SoftPkg, Pred>(impl);}

			Udm::ArchetypeAttr< ::SCAML::SoftPkg> Archetype() const { return Udm::ArchetypeAttr< ::SCAML::SoftPkg>(impl);}

			static ::Uml::Attribute meta_description;
			Udm::StringAttr description() const { return Udm::StringAttr(impl, meta_description); }

			static ::Uml::Attribute meta_title;
			Udm::StringAttr title() const { return Udm::StringAttr(impl, meta_title); }

			static ::Uml::Attribute meta_type;
			Udm::StringAttr type() const { return Udm::StringAttr(impl, meta_type); }

			static ::Uml::Attribute meta_version;
			Udm::StringAttr version() const { return Udm::StringAttr(impl, meta_version); }

			static ::Uml::Attribute meta_id;
			Udm::StringAttr id() const { return Udm::StringAttr(impl, meta_id); }

			static ::Uml::AssociationRole meta_referedbycomponentfile;
			Udm::AssocAttr< ::SCAML::componentfile> referedbycomponentfile() const { return Udm::AssocAttr< ::SCAML::componentfile>(impl, meta_referedbycomponentfile); }
			template <class Pred> Udm::AssocAttr< ::SCAML::componentfile, Pred > referedbycomponentfile_sorted(const Pred &) const { return Udm::AssocAttr< ::SCAML::componentfile, Pred>(impl, meta_referedbycomponentfile); }

			static ::Uml::AssociationRole meta_referedbydevconfigcomponentfile;
			Udm::AssocAttr< ::SCAML::devconfigcomponentfile> referedbydevconfigcomponentfile() const { return Udm::AssocAttr< ::SCAML::devconfigcomponentfile>(impl, meta_referedbydevconfigcomponentfile); }
			template <class Pred> Udm::AssocAttr< ::SCAML::devconfigcomponentfile, Pred > referedbydevconfigcomponentfile_sorted(const Pred &) const { return Udm::AssocAttr< ::SCAML::devconfigcomponentfile, Pred>(impl, meta_referedbydevconfigcomponentfile); }

			static ::Uml::AssociationRole meta_referedbydevicemanagersoftpkg;
			Udm::AssocAttr< ::SCAML::devicemanagersoftpkg> referedbydevicemanagersoftpkg() const { return Udm::AssocAttr< ::SCAML::devicemanagersoftpkg>(impl, meta_referedbydevicemanagersoftpkg); }
			template <class Pred> Udm::AssocAttr< ::SCAML::devicemanagersoftpkg, Pred > referedbydevicemanagersoftpkg_sorted(const Pred &) const { return Udm::AssocAttr< ::SCAML::devicemanagersoftpkg, Pred>(impl, meta_referedbydevicemanagersoftpkg); }

			static ::Uml::AssociationRole meta_referedbysoftpkgref;
			Udm::AssocAttr< ::SCAML::softpkgref> referedbysoftpkgref() const { return Udm::AssocAttr< ::SCAML::softpkgref>(impl, meta_referedbysoftpkgref); }
			template <class Pred> Udm::AssocAttr< ::SCAML::softpkgref, Pred > referedbysoftpkgref_sorted(const Pred &) const { return Udm::AssocAttr< ::SCAML::softpkgref, Pred>(impl, meta_referedbysoftpkgref); }

			static ::Uml::AssociationRole meta_referedbySoftPkgRef;
			Udm::AssocAttr< ::SCAML::SoftPkgRef> referedbySoftPkgRef() const { return Udm::AssocAttr< ::SCAML::SoftPkgRef>(impl, meta_referedbySoftPkgRef); }
			template <class Pred> Udm::AssocAttr< ::SCAML::SoftPkgRef, Pred > referedbySoftPkgRef_sorted(const Pred &) const { return Udm::AssocAttr< ::SCAML::SoftPkgRef, Pred>(impl, meta_referedbySoftPkgRef); }

			static ::Uml::AssociationRole meta_referedbySoftwarePackageRef;
			Udm::AssocAttr< ::SCAML::SoftwarePackageRef> referedbySoftwarePackageRef() const { return Udm::AssocAttr< ::SCAML::SoftwarePackageRef>(impl, meta_referedbySoftwarePackageRef); }
			template <class Pred> Udm::AssocAttr< ::SCAML::SoftwarePackageRef, Pred > referedbySoftwarePackageRef_sorted(const Pred &) const { return Udm::AssocAttr< ::SCAML::SoftwarePackageRef, Pred>(impl, meta_referedbySoftwarePackageRef); }

			static ::Uml::CompositionChildRole meta_propertyfile_child;
			Udm::ChildAttr< ::SCAML::propertyfile> propertyfile_child() const { return Udm::ChildAttr< ::SCAML::propertyfile>(impl, meta_propertyfile_child); }

			static ::Uml::CompositionChildRole meta_descriptor;
			Udm::ChildAttr< ::SCAML::SoftwareComponentRef> descriptor() const { return Udm::ChildAttr< ::SCAML::SoftwareComponentRef>(impl, meta_descriptor); }

			static ::Uml::CompositionChildRole meta_author_children;
			Udm::ChildrenAttr< ::SCAML::author> author_children() const { return Udm::ChildrenAttr< ::SCAML::author>(impl, meta_author_children); }
			template <class Pred> Udm::ChildrenAttr< ::SCAML::author, Pred> author_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::author, Pred>(impl, meta_author_children); }

			static ::Uml::CompositionChildRole meta_usesdevice_children;
			Udm::ChildrenAttr< ::SCAML::usesdevice> usesdevice_children() const { return Udm::ChildrenAttr< ::SCAML::usesdevice>(impl, meta_usesdevice_children); }
			template <class Pred> Udm::ChildrenAttr< ::SCAML::usesdevice, Pred> usesdevice_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::usesdevice, Pred>(impl, meta_usesdevice_children); }

			static ::Uml::CompositionChildRole meta_implementation_children;
			Udm::ChildrenAttr< ::SCAML::implementation> implementation_children() const { return Udm::ChildrenAttr< ::SCAML::implementation>(impl, meta_implementation_children); }
			template <class Pred> Udm::ChildrenAttr< ::SCAML::implementation, Pred> implementation_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::implementation, Pred>(impl, meta_implementation_children); }

			Udm::ChildrenAttr< ::SCAML::usesdevice> usesdevice_kind_children() const { return Udm::ChildrenAttr< ::SCAML::usesdevice>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::usesdevice, Pred> usesdevice_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::usesdevice, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::author> author_kind_children() const { return Udm::ChildrenAttr< ::SCAML::author>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::author, Pred> author_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::author, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::implementation> implementation_kind_children() const { return Udm::ChildrenAttr< ::SCAML::implementation>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::implementation, Pred> implementation_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::implementation, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::SimulinkImplementation> SimulinkImplementation_kind_children() const { return Udm::ChildrenAttr< ::SCAML::SimulinkImplementation>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::SimulinkImplementation, Pred> SimulinkImplementation_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::SimulinkImplementation, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::propertyfile> propertyfile_kind_children() const { return Udm::ChildrenAttr< ::SCAML::propertyfile>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::propertyfile, Pred> propertyfile_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::propertyfile, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::SoftwareComponentRef> SoftwareComponentRef_kind_children() const { return Udm::ChildrenAttr< ::SCAML::SoftwareComponentRef>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::SoftwareComponentRef, Pred> SoftwareComponentRef_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::SoftwareComponentRef, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr< ::SCAML::MgaObject>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

			static ::Uml::CompositionParentRole meta_SoftPkgFolder_parent;
			Udm::ParentAttr< ::SCAML::SoftPkgFolder> SoftPkgFolder_parent() const { return Udm::ParentAttr< ::SCAML::SoftPkgFolder>(impl, meta_SoftPkgFolder_parent); }

			Udm::ParentAttr< ::SCAML::SoftPkgFolder> parent() const { return Udm::ParentAttr< ::SCAML::SoftPkgFolder>(impl, Udm::NULLPARENTROLE); }
			static ::Uml::Constraint meta_OnlyOneSCD;
		};

		class  DevicePkg :  public ::SCAML::MgaObject {
		public:
			static ::Uml::Class meta;

			DevicePkg() { }
			DevicePkg(Udm::ObjectImpl *impl) : ::SCAML::MgaObject(impl), UDM_OBJECT(impl) { }
			DevicePkg(const DevicePkg &master) : ::SCAML::MgaObject(master), UDM_OBJECT(master) { }
			static DevicePkg Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static DevicePkg Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			DevicePkg CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			DevicePkg CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::SCAML::DevicePkg> Instances() { return Udm::InstantiatedAttr< ::SCAML::DevicePkg>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::SCAML::DevicePkg, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::SCAML::DevicePkg, Pred>(impl);}

			Udm::DerivedAttr< ::SCAML::DevicePkg> Derived() { return Udm::DerivedAttr< ::SCAML::DevicePkg>(impl);}
			template <class Pred> Udm::DerivedAttr< ::SCAML::DevicePkg, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::SCAML::DevicePkg, Pred>(impl);}

			Udm::ArchetypeAttr< ::SCAML::DevicePkg> Archetype() const { return Udm::ArchetypeAttr< ::SCAML::DevicePkg>(impl);}

			static ::Uml::Attribute meta_version;
			Udm::StringAttr version() const { return Udm::StringAttr(impl, meta_version); }

			static ::Uml::Attribute meta_id;
			Udm::StringAttr id() const { return Udm::StringAttr(impl, meta_id); }

			static ::Uml::Attribute meta_title;
			Udm::StringAttr title() const { return Udm::StringAttr(impl, meta_title); }

			static ::Uml::Attribute meta_description;
			Udm::StringAttr description() const { return Udm::StringAttr(impl, meta_description); }

			static ::Uml::AssociationRole meta_referedbydevicepkgref;
			Udm::AssocAttr< ::SCAML::devicepkgref> referedbydevicepkgref() const { return Udm::AssocAttr< ::SCAML::devicepkgref>(impl, meta_referedbydevicepkgref); }
			template <class Pred> Udm::AssocAttr< ::SCAML::devicepkgref, Pred > referedbydevicepkgref_sorted(const Pred &) const { return Udm::AssocAttr< ::SCAML::devicepkgref, Pred>(impl, meta_referedbydevicepkgref); }

			static ::Uml::AssociationRole meta_referedbydevicepkgfile;
			Udm::AssocAttr< ::SCAML::devicepkgfile> referedbydevicepkgfile() const { return Udm::AssocAttr< ::SCAML::devicepkgfile>(impl, meta_referedbydevicepkgfile); }
			template <class Pred> Udm::AssocAttr< ::SCAML::devicepkgfile, Pred > referedbydevicepkgfile_sorted(const Pred &) const { return Udm::AssocAttr< ::SCAML::devicepkgfile, Pred>(impl, meta_referedbydevicepkgfile); }

			static ::Uml::AssociationRole meta_referedbyusesdeviceref;
			Udm::AssocAttr< ::SCAML::usesdeviceref> referedbyusesdeviceref() const { return Udm::AssocAttr< ::SCAML::usesdeviceref>(impl, meta_referedbyusesdeviceref); }
			template <class Pred> Udm::AssocAttr< ::SCAML::usesdeviceref, Pred > referedbyusesdeviceref_sorted(const Pred &) const { return Udm::AssocAttr< ::SCAML::usesdeviceref, Pred>(impl, meta_referedbyusesdeviceref); }

			static ::Uml::AssociationRole meta_referedbyDevicePkgRef;
			Udm::AssocAttr< ::SCAML::DevicePkgRef> referedbyDevicePkgRef() const { return Udm::AssocAttr< ::SCAML::DevicePkgRef>(impl, meta_referedbyDevicePkgRef); }
			template <class Pred> Udm::AssocAttr< ::SCAML::DevicePkgRef, Pred > referedbyDevicePkgRef_sorted(const Pred &) const { return Udm::AssocAttr< ::SCAML::DevicePkgRef, Pred>(impl, meta_referedbyDevicePkgRef); }

			static ::Uml::CompositionChildRole meta_author_children;
			Udm::ChildrenAttr< ::SCAML::author> author_children() const { return Udm::ChildrenAttr< ::SCAML::author>(impl, meta_author_children); }
			template <class Pred> Udm::ChildrenAttr< ::SCAML::author, Pred> author_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::author, Pred>(impl, meta_author_children); }

			static ::Uml::CompositionChildRole meta_hwdeviceregistration_children;
			Udm::ChildrenAttr< ::SCAML::hwdeviceregistration> hwdeviceregistration_children() const { return Udm::ChildrenAttr< ::SCAML::hwdeviceregistration>(impl, meta_hwdeviceregistration_children); }
			template <class Pred> Udm::ChildrenAttr< ::SCAML::hwdeviceregistration, Pred> hwdeviceregistration_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::hwdeviceregistration, Pred>(impl, meta_hwdeviceregistration_children); }

			Udm::ChildrenAttr< ::SCAML::hwdeviceregistration> hwdeviceregistration_kind_children() const { return Udm::ChildrenAttr< ::SCAML::hwdeviceregistration>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::hwdeviceregistration, Pred> hwdeviceregistration_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::hwdeviceregistration, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::childhwdevice> childhwdevice_kind_children() const { return Udm::ChildrenAttr< ::SCAML::childhwdevice>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::childhwdevice, Pred> childhwdevice_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::childhwdevice, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::author> author_kind_children() const { return Udm::ChildrenAttr< ::SCAML::author>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::author, Pred> author_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::author, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr< ::SCAML::MgaObject>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

			static ::Uml::CompositionParentRole meta_DevicePackageFolder_parent;
			Udm::ParentAttr< ::SCAML::DevicePackageFolder> DevicePackageFolder_parent() const { return Udm::ParentAttr< ::SCAML::DevicePackageFolder>(impl, meta_DevicePackageFolder_parent); }

			Udm::ParentAttr< ::SCAML::DevicePackageFolder> parent() const { return Udm::ParentAttr< ::SCAML::DevicePackageFolder>(impl, Udm::NULLPARENTROLE); }
		};

		class  DomainManagerConfiguration :  public ::SCAML::MgaObject {
		public:
			static ::Uml::Class meta;

			DomainManagerConfiguration() { }
			DomainManagerConfiguration(Udm::ObjectImpl *impl) : ::SCAML::MgaObject(impl), UDM_OBJECT(impl) { }
			DomainManagerConfiguration(const DomainManagerConfiguration &master) : ::SCAML::MgaObject(master), UDM_OBJECT(master) { }
			static DomainManagerConfiguration Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static DomainManagerConfiguration Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			DomainManagerConfiguration CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			DomainManagerConfiguration CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::SCAML::DomainManagerConfiguration> Instances() { return Udm::InstantiatedAttr< ::SCAML::DomainManagerConfiguration>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::SCAML::DomainManagerConfiguration, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::SCAML::DomainManagerConfiguration, Pred>(impl);}

			Udm::DerivedAttr< ::SCAML::DomainManagerConfiguration> Derived() { return Udm::DerivedAttr< ::SCAML::DomainManagerConfiguration>(impl);}
			template <class Pred> Udm::DerivedAttr< ::SCAML::DomainManagerConfiguration, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::SCAML::DomainManagerConfiguration, Pred>(impl);}

			Udm::ArchetypeAttr< ::SCAML::DomainManagerConfiguration> Archetype() const { return Udm::ArchetypeAttr< ::SCAML::DomainManagerConfiguration>(impl);}

			static ::Uml::Attribute meta_description;
			Udm::StringAttr description() const { return Udm::StringAttr(impl, meta_description); }

			static ::Uml::CompositionChildRole meta_SoftPkgRef_children;
			Udm::ChildrenAttr< ::SCAML::SoftPkgRef> SoftPkgRef_children() const { return Udm::ChildrenAttr< ::SCAML::SoftPkgRef>(impl, meta_SoftPkgRef_children); }
			template <class Pred> Udm::ChildrenAttr< ::SCAML::SoftPkgRef, Pred> SoftPkgRef_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::SoftPkgRef, Pred>(impl, meta_SoftPkgRef_children); }

			Udm::ChildrenAttr< ::SCAML::SoftPkgRef> SoftPkgRef_kind_children() const { return Udm::ChildrenAttr< ::SCAML::SoftPkgRef>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::SoftPkgRef, Pred> SoftPkgRef_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::SoftPkgRef, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr< ::SCAML::MgaObject>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

			static ::Uml::CompositionParentRole meta_DomainProfile_parent;
			Udm::ParentAttr< ::SCAML::DomainProfile> DomainProfile_parent() const { return Udm::ParentAttr< ::SCAML::DomainProfile>(impl, meta_DomainProfile_parent); }

			Udm::ParentAttr< ::SCAML::DomainProfile> parent() const { return Udm::ParentAttr< ::SCAML::DomainProfile>(impl, Udm::NULLPARENTROLE); }
		};

		class  SoftwareAssembly :  public ::SCAML::MgaObject {
		public:
			static ::Uml::Class meta;

			SoftwareAssembly() { }
			SoftwareAssembly(Udm::ObjectImpl *impl) : ::SCAML::MgaObject(impl), UDM_OBJECT(impl) { }
			SoftwareAssembly(const SoftwareAssembly &master) : ::SCAML::MgaObject(master), UDM_OBJECT(master) { }
			static SoftwareAssembly Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static SoftwareAssembly Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			SoftwareAssembly CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			SoftwareAssembly CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::SCAML::SoftwareAssembly> Instances() { return Udm::InstantiatedAttr< ::SCAML::SoftwareAssembly>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::SCAML::SoftwareAssembly, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::SCAML::SoftwareAssembly, Pred>(impl);}

			Udm::DerivedAttr< ::SCAML::SoftwareAssembly> Derived() { return Udm::DerivedAttr< ::SCAML::SoftwareAssembly>(impl);}
			template <class Pred> Udm::DerivedAttr< ::SCAML::SoftwareAssembly, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::SCAML::SoftwareAssembly, Pred>(impl);}

			Udm::ArchetypeAttr< ::SCAML::SoftwareAssembly> Archetype() const { return Udm::ArchetypeAttr< ::SCAML::SoftwareAssembly>(impl);}

			static ::Uml::Attribute meta_description;
			Udm::StringAttr description() const { return Udm::StringAttr(impl, meta_description); }

			static ::Uml::Attribute meta_version;
			Udm::StringAttr version() const { return Udm::StringAttr(impl, meta_version); }

			static ::Uml::CompositionChildRole meta_componentsupportedinterface_children;
			Udm::ChildrenAttr< ::SCAML::componentsupportedinterface> componentsupportedinterface_children() const { return Udm::ChildrenAttr< ::SCAML::componentsupportedinterface>(impl, meta_componentsupportedinterface_children); }
			template <class Pred> Udm::ChildrenAttr< ::SCAML::componentsupportedinterface, Pred> componentsupportedinterface_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::componentsupportedinterface, Pred>(impl, meta_componentsupportedinterface_children); }

			static ::Uml::CompositionChildRole meta_SoftwarePackageRef_children;
			Udm::ChildrenAttr< ::SCAML::SoftwarePackageRef> SoftwarePackageRef_children() const { return Udm::ChildrenAttr< ::SCAML::SoftwarePackageRef>(impl, meta_SoftwarePackageRef_children); }
			template <class Pred> Udm::ChildrenAttr< ::SCAML::SoftwarePackageRef, Pred> SoftwarePackageRef_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::SoftwarePackageRef, Pred>(impl, meta_SoftwarePackageRef_children); }

			static ::Uml::CompositionChildRole meta_usestodestConn_children;
			Udm::ChildrenAttr< ::SCAML::usestodestConn> usestodestConn_children() const { return Udm::ChildrenAttr< ::SCAML::usestodestConn>(impl, meta_usestodestConn_children); }
			template <class Pred> Udm::ChildrenAttr< ::SCAML::usestodestConn, Pred> usestodestConn_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::usestodestConn, Pred>(impl, meta_usestodestConn_children); }

			static ::Uml::CompositionChildRole meta_usestofreestandingConn_children;
			Udm::ChildrenAttr< ::SCAML::usestofreestandingConn> usestofreestandingConn_children() const { return Udm::ChildrenAttr< ::SCAML::usestofreestandingConn>(impl, meta_usestofreestandingConn_children); }
			template <class Pred> Udm::ChildrenAttr< ::SCAML::usestofreestandingConn, Pred> usestofreestandingConn_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::usestofreestandingConn, Pred>(impl, meta_usestofreestandingConn_children); }

			static ::Uml::CompositionChildRole meta_freestandingtoprovidedConn_children;
			Udm::ChildrenAttr< ::SCAML::freestandingtoprovidedConn> freestandingtoprovidedConn_children() const { return Udm::ChildrenAttr< ::SCAML::freestandingtoprovidedConn>(impl, meta_freestandingtoprovidedConn_children); }
			template <class Pred> Udm::ChildrenAttr< ::SCAML::freestandingtoprovidedConn, Pred> freestandingtoprovidedConn_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::freestandingtoprovidedConn, Pred>(impl, meta_freestandingtoprovidedConn_children); }

			static ::Uml::CompositionChildRole meta_externaltodestConn_children;
			Udm::ChildrenAttr< ::SCAML::externaltodestConn> externaltodestConn_children() const { return Udm::ChildrenAttr< ::SCAML::externaltodestConn>(impl, meta_externaltodestConn_children); }
			template <class Pred> Udm::ChildrenAttr< ::SCAML::externaltodestConn, Pred> externaltodestConn_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::externaltodestConn, Pred>(impl, meta_externaltodestConn_children); }

			static ::Uml::CompositionChildRole meta_externalport_children;
			Udm::ChildrenAttr< ::SCAML::externalport> externalport_children() const { return Udm::ChildrenAttr< ::SCAML::externalport>(impl, meta_externalport_children); }
			template <class Pred> Udm::ChildrenAttr< ::SCAML::externalport, Pred> externalport_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::externalport, Pred>(impl, meta_externalport_children); }

			static ::Uml::CompositionChildRole meta_FreestandingPort_children;
			Udm::ChildrenAttr< ::SCAML::FreestandingPort> FreestandingPort_children() const { return Udm::ChildrenAttr< ::SCAML::FreestandingPort>(impl, meta_FreestandingPort_children); }
			template <class Pred> Udm::ChildrenAttr< ::SCAML::FreestandingPort, Pred> FreestandingPort_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::FreestandingPort, Pred>(impl, meta_FreestandingPort_children); }

			static ::Uml::CompositionChildRole meta_controllertocompinst_children;
			Udm::ChildrenAttr< ::SCAML::controllertocompinst> controllertocompinst_children() const { return Udm::ChildrenAttr< ::SCAML::controllertocompinst>(impl, meta_controllertocompinst_children); }
			template <class Pred> Udm::ChildrenAttr< ::SCAML::controllertocompinst, Pred> controllertocompinst_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::controllertocompinst, Pred>(impl, meta_controllertocompinst_children); }

			static ::Uml::CompositionChildRole meta_assemblycontroller_children;
			Udm::ChildrenAttr< ::SCAML::assemblycontroller> assemblycontroller_children() const { return Udm::ChildrenAttr< ::SCAML::assemblycontroller>(impl, meta_assemblycontroller_children); }
			template <class Pred> Udm::ChildrenAttr< ::SCAML::assemblycontroller, Pred> assemblycontroller_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::assemblycontroller, Pred>(impl, meta_assemblycontroller_children); }

			static ::Uml::CompositionChildRole meta_SoftwareComponent_children;
			Udm::ChildrenAttr< ::SCAML::SoftwareComponent> SoftwareComponent_children() const { return Udm::ChildrenAttr< ::SCAML::SoftwareComponent>(impl, meta_SoftwareComponent_children); }
			template <class Pred> Udm::ChildrenAttr< ::SCAML::SoftwareComponent, Pred> SoftwareComponent_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::SoftwareComponent, Pred>(impl, meta_SoftwareComponent_children); }

			static ::Uml::CompositionChildRole meta_componentfile_children;
			Udm::ChildrenAttr< ::SCAML::componentfile> componentfile_children() const { return Udm::ChildrenAttr< ::SCAML::componentfile>(impl, meta_componentfile_children); }
			template <class Pred> Udm::ChildrenAttr< ::SCAML::componentfile, Pred> componentfile_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::componentfile, Pred>(impl, meta_componentfile_children); }

			static ::Uml::CompositionChildRole meta_componentresourcefactoryref_children;
			Udm::ChildrenAttr< ::SCAML::componentresourcefactoryref> componentresourcefactoryref_children() const { return Udm::ChildrenAttr< ::SCAML::componentresourcefactoryref>(impl, meta_componentresourcefactoryref_children); }
			template <class Pred> Udm::ChildrenAttr< ::SCAML::componentresourcefactoryref, Pred> componentresourcefactoryref_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::componentresourcefactoryref, Pred>(impl, meta_componentresourcefactoryref_children); }

			static ::Uml::CompositionChildRole meta_componentproperties_children;
			Udm::ChildrenAttr< ::SCAML::componentproperties> componentproperties_children() const { return Udm::ChildrenAttr< ::SCAML::componentproperties>(impl, meta_componentproperties_children); }
			template <class Pred> Udm::ChildrenAttr< ::SCAML::componentproperties, Pred> componentproperties_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::componentproperties, Pred>(impl, meta_componentproperties_children); }

			static ::Uml::CompositionChildRole meta_findcomponentConn_children;
			Udm::ChildrenAttr< ::SCAML::findcomponentConn> findcomponentConn_children() const { return Udm::ChildrenAttr< ::SCAML::findcomponentConn>(impl, meta_findcomponentConn_children); }
			template <class Pred> Udm::ChildrenAttr< ::SCAML::findcomponentConn, Pred> findcomponentConn_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::findcomponentConn, Pred>(impl, meta_findcomponentConn_children); }

			static ::Uml::CompositionChildRole meta_componentresourcefactorytopropertiesConn_children;
			Udm::ChildrenAttr< ::SCAML::componentresourcefactorytopropertiesConn> componentresourcefactorytopropertiesConn_children() const { return Udm::ChildrenAttr< ::SCAML::componentresourcefactorytopropertiesConn>(impl, meta_componentresourcefactorytopropertiesConn_children); }
			template <class Pred> Udm::ChildrenAttr< ::SCAML::componentresourcefactorytopropertiesConn, Pred> componentresourcefactorytopropertiesConn_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::componentresourcefactorytopropertiesConn, Pred>(impl, meta_componentresourcefactorytopropertiesConn_children); }

			static ::Uml::CompositionChildRole meta_componentpropertiesConn_children;
			Udm::ChildrenAttr< ::SCAML::componentpropertiesConn> componentpropertiesConn_children() const { return Udm::ChildrenAttr< ::SCAML::componentpropertiesConn>(impl, meta_componentpropertiesConn_children); }
			template <class Pred> Udm::ChildrenAttr< ::SCAML::componentpropertiesConn, Pred> componentpropertiesConn_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::componentpropertiesConn, Pred>(impl, meta_componentpropertiesConn_children); }

			static ::Uml::CompositionChildRole meta_componentplacementConn_children;
			Udm::ChildrenAttr< ::SCAML::componentplacementConn> componentplacementConn_children() const { return Udm::ChildrenAttr< ::SCAML::componentplacementConn>(impl, meta_componentplacementConn_children); }
			template <class Pred> Udm::ChildrenAttr< ::SCAML::componentplacementConn, Pred> componentplacementConn_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::componentplacementConn, Pred>(impl, meta_componentplacementConn_children); }

			static ::Uml::CompositionChildRole meta_hostcollocation_children;
			Udm::ChildrenAttr< ::SCAML::hostcollocation> hostcollocation_children() const { return Udm::ChildrenAttr< ::SCAML::hostcollocation>(impl, meta_hostcollocation_children); }
			template <class Pred> Udm::ChildrenAttr< ::SCAML::hostcollocation, Pred> hostcollocation_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::hostcollocation, Pred>(impl, meta_hostcollocation_children); }

			Udm::ChildrenAttr< ::SCAML::connectdestBase> connectdestBase_kind_children() const { return Udm::ChildrenAttr< ::SCAML::connectdestBase>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::connectdestBase, Pred> connectdestBase_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::connectdestBase, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::componentplacement_Members_Base> componentplacement_Members_Base_kind_children() const { return Udm::ChildrenAttr< ::SCAML::componentplacement_Members_Base>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::componentplacement_Members_Base, Pred> componentplacement_Members_Base_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::componentplacement_Members_Base, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::usestodestConn> usestodestConn_kind_children() const { return Udm::ChildrenAttr< ::SCAML::usestodestConn>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::usestodestConn, Pred> usestodestConn_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::usestodestConn, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::externaltodestConn> externaltodestConn_kind_children() const { return Udm::ChildrenAttr< ::SCAML::externaltodestConn>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::externaltodestConn, Pred> externaltodestConn_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::externaltodestConn, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::externalport> externalport_kind_children() const { return Udm::ChildrenAttr< ::SCAML::externalport>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::externalport, Pred> externalport_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::externalport, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::externaltodestBase> externaltodestBase_kind_children() const { return Udm::ChildrenAttr< ::SCAML::externaltodestBase>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::externaltodestBase, Pred> externaltodestBase_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::externaltodestBase, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::componentsupportedinterface> componentsupportedinterface_kind_children() const { return Udm::ChildrenAttr< ::SCAML::componentsupportedinterface>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::componentsupportedinterface, Pred> componentsupportedinterface_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::componentsupportedinterface, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::freestandingtoprovidedConn> freestandingtoprovidedConn_kind_children() const { return Udm::ChildrenAttr< ::SCAML::freestandingtoprovidedConn>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::freestandingtoprovidedConn, Pred> freestandingtoprovidedConn_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::freestandingtoprovidedConn, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::usestofreestandingConn> usestofreestandingConn_kind_children() const { return Udm::ChildrenAttr< ::SCAML::usestofreestandingConn>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::usestofreestandingConn, Pred> usestofreestandingConn_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::usestofreestandingConn, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::thisPort> thisPort_kind_children() const { return Udm::ChildrenAttr< ::SCAML::thisPort>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::thisPort, Pred> thisPort_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::thisPort, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::log> log_kind_children() const { return Udm::ChildrenAttr< ::SCAML::log>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::log, Pred> log_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::log, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::filemanager> filemanager_kind_children() const { return Udm::ChildrenAttr< ::SCAML::filemanager>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::filemanager, Pred> filemanager_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::filemanager, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::namingservice> namingservice_kind_children() const { return Udm::ChildrenAttr< ::SCAML::namingservice>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::namingservice, Pred> namingservice_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::namingservice, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::FreestandingPort> FreestandingPort_kind_children() const { return Udm::ChildrenAttr< ::SCAML::FreestandingPort>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::FreestandingPort, Pred> FreestandingPort_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::FreestandingPort, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::deviceusedbythiscomponentref> deviceusedbythiscomponentref_kind_children() const { return Udm::ChildrenAttr< ::SCAML::deviceusedbythiscomponentref>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::deviceusedbythiscomponentref, Pred> deviceusedbythiscomponentref_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::deviceusedbythiscomponentref, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::service> service_kind_children() const { return Udm::ChildrenAttr< ::SCAML::service>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::service, Pred> service_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::service, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::eventchannel> eventchannel_kind_children() const { return Udm::ChildrenAttr< ::SCAML::eventchannel>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::eventchannel, Pred> eventchannel_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::eventchannel, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::devicethatloadedthiscomponentref> devicethatloadedthiscomponentref_kind_children() const { return Udm::ChildrenAttr< ::SCAML::devicethatloadedthiscomponentref>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::devicethatloadedthiscomponentref, Pred> devicethatloadedthiscomponentref_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::devicethatloadedthiscomponentref, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::controllertocompinst> controllertocompinst_kind_children() const { return Udm::ChildrenAttr< ::SCAML::controllertocompinst>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::controllertocompinst, Pred> controllertocompinst_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::controllertocompinst, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::assemblycontroller> assemblycontroller_kind_children() const { return Udm::ChildrenAttr< ::SCAML::assemblycontroller>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::assemblycontroller, Pred> assemblycontroller_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::assemblycontroller, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::componentresourcefactorytopropertiesConn> componentresourcefactorytopropertiesConn_kind_children() const { return Udm::ChildrenAttr< ::SCAML::componentresourcefactorytopropertiesConn>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::componentresourcefactorytopropertiesConn, Pred> componentresourcefactorytopropertiesConn_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::componentresourcefactorytopropertiesConn, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::findcomponentConn> findcomponentConn_kind_children() const { return Udm::ChildrenAttr< ::SCAML::findcomponentConn>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::findcomponentConn, Pred> findcomponentConn_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::findcomponentConn, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::componentresourcefactoryref> componentresourcefactoryref_kind_children() const { return Udm::ChildrenAttr< ::SCAML::componentresourcefactoryref>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::componentresourcefactoryref, Pred> componentresourcefactoryref_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::componentresourcefactoryref, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::componentpropertiesConn> componentpropertiesConn_kind_children() const { return Udm::ChildrenAttr< ::SCAML::componentpropertiesConn>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::componentpropertiesConn, Pred> componentpropertiesConn_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::componentpropertiesConn, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::componentproperties> componentproperties_kind_children() const { return Udm::ChildrenAttr< ::SCAML::componentproperties>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::componentproperties, Pred> componentproperties_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::componentproperties, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::componentplacementConn> componentplacementConn_kind_children() const { return Udm::ChildrenAttr< ::SCAML::componentplacementConn>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::componentplacementConn, Pred> componentplacementConn_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::componentplacementConn, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::hostcollocation> hostcollocation_kind_children() const { return Udm::ChildrenAttr< ::SCAML::hostcollocation>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::hostcollocation, Pred> hostcollocation_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::hostcollocation, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::componentfile> componentfile_kind_children() const { return Udm::ChildrenAttr< ::SCAML::componentfile>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::componentfile, Pred> componentfile_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::componentfile, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::SimulinkImplementation> SimulinkImplementation_kind_children() const { return Udm::ChildrenAttr< ::SCAML::SimulinkImplementation>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::SimulinkImplementation, Pred> SimulinkImplementation_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::SimulinkImplementation, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::SoftwarePackageRef> SoftwarePackageRef_kind_children() const { return Udm::ChildrenAttr< ::SCAML::SoftwarePackageRef>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::SoftwarePackageRef, Pred> SoftwarePackageRef_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::SoftwarePackageRef, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::SoftwareComponent> SoftwareComponent_kind_children() const { return Udm::ChildrenAttr< ::SCAML::SoftwareComponent>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::SoftwareComponent, Pred> SoftwareComponent_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::SoftwareComponent, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr< ::SCAML::MgaObject>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

			static ::Uml::CompositionParentRole meta_DomainProfile_parent;
			Udm::ParentAttr< ::SCAML::DomainProfile> DomainProfile_parent() const { return Udm::ParentAttr< ::SCAML::DomainProfile>(impl, meta_DomainProfile_parent); }

			Udm::ParentAttr< ::SCAML::DomainProfile> parent() const { return Udm::ParentAttr< ::SCAML::DomainProfile>(impl, Udm::NULLPARENTROLE); }
		};

		class  DeviceConfiguration :  public ::SCAML::MgaObject {
		public:
			static ::Uml::Class meta;

			DeviceConfiguration() { }
			DeviceConfiguration(Udm::ObjectImpl *impl) : ::SCAML::MgaObject(impl), UDM_OBJECT(impl) { }
			DeviceConfiguration(const DeviceConfiguration &master) : ::SCAML::MgaObject(master), UDM_OBJECT(master) { }
			static DeviceConfiguration Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static DeviceConfiguration Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			DeviceConfiguration CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			DeviceConfiguration CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::SCAML::DeviceConfiguration> Instances() { return Udm::InstantiatedAttr< ::SCAML::DeviceConfiguration>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::SCAML::DeviceConfiguration, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::SCAML::DeviceConfiguration, Pred>(impl);}

			Udm::DerivedAttr< ::SCAML::DeviceConfiguration> Derived() { return Udm::DerivedAttr< ::SCAML::DeviceConfiguration>(impl);}
			template <class Pred> Udm::DerivedAttr< ::SCAML::DeviceConfiguration, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::SCAML::DeviceConfiguration, Pred>(impl);}

			Udm::ArchetypeAttr< ::SCAML::DeviceConfiguration> Archetype() const { return Udm::ArchetypeAttr< ::SCAML::DeviceConfiguration>(impl);}

			static ::Uml::Attribute meta_id;
			Udm::StringAttr id() const { return Udm::StringAttr(impl, meta_id); }

			static ::Uml::Attribute meta_description;
			Udm::StringAttr description() const { return Udm::StringAttr(impl, meta_description); }

			static ::Uml::Attribute meta_domainmanager;
			Udm::StringAttr domainmanager() const { return Udm::StringAttr(impl, meta_domainmanager); }

			static ::Uml::CompositionChildRole meta_SoftwarePackageRef_children;
			Udm::ChildrenAttr< ::SCAML::SoftwarePackageRef> SoftwarePackageRef_children() const { return Udm::ChildrenAttr< ::SCAML::SoftwarePackageRef>(impl, meta_SoftwarePackageRef_children); }
			template <class Pred> Udm::ChildrenAttr< ::SCAML::SoftwarePackageRef, Pred> SoftwarePackageRef_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::SoftwarePackageRef, Pred>(impl, meta_SoftwarePackageRef_children); }

			static ::Uml::CompositionChildRole meta_DevicePkgRef_children;
			Udm::ChildrenAttr< ::SCAML::DevicePkgRef> DevicePkgRef_children() const { return Udm::ChildrenAttr< ::SCAML::DevicePkgRef>(impl, meta_DevicePkgRef_children); }
			template <class Pred> Udm::ChildrenAttr< ::SCAML::DevicePkgRef, Pred> DevicePkgRef_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::DevicePkgRef, Pred>(impl, meta_DevicePkgRef_children); }

			static ::Uml::CompositionChildRole meta_devicepkgfile_children;
			Udm::ChildrenAttr< ::SCAML::devicepkgfile> devicepkgfile_children() const { return Udm::ChildrenAttr< ::SCAML::devicepkgfile>(impl, meta_devicepkgfile_children); }
			template <class Pred> Udm::ChildrenAttr< ::SCAML::devicepkgfile, Pred> devicepkgfile_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::devicepkgfile, Pred>(impl, meta_devicepkgfile_children); }

			static ::Uml::CompositionChildRole meta_thisPort_children;
			Udm::ChildrenAttr< ::SCAML::thisPort> thisPort_children() const { return Udm::ChildrenAttr< ::SCAML::thisPort>(impl, meta_thisPort_children); }
			template <class Pred> Udm::ChildrenAttr< ::SCAML::thisPort, Pred> thisPort_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::thisPort, Pred>(impl, meta_thisPort_children); }

			static ::Uml::CompositionChildRole meta_SoftwareComponent_children;
			Udm::ChildrenAttr< ::SCAML::SoftwareComponent> SoftwareComponent_children() const { return Udm::ChildrenAttr< ::SCAML::SoftwareComponent>(impl, meta_SoftwareComponent_children); }
			template <class Pred> Udm::ChildrenAttr< ::SCAML::SoftwareComponent, Pred> SoftwareComponent_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::SoftwareComponent, Pred>(impl, meta_SoftwareComponent_children); }

			static ::Uml::CompositionChildRole meta_filesystemname_children;
			Udm::ChildrenAttr< ::SCAML::filesystemname> filesystemname_children() const { return Udm::ChildrenAttr< ::SCAML::filesystemname>(impl, meta_filesystemname_children); }
			template <class Pred> Udm::ChildrenAttr< ::SCAML::filesystemname, Pred> filesystemname_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::filesystemname, Pred>(impl, meta_filesystemname_children); }

			static ::Uml::CompositionChildRole meta_componentpropertiesConn_children;
			Udm::ChildrenAttr< ::SCAML::componentpropertiesConn> componentpropertiesConn_children() const { return Udm::ChildrenAttr< ::SCAML::componentpropertiesConn>(impl, meta_componentpropertiesConn_children); }
			template <class Pred> Udm::ChildrenAttr< ::SCAML::componentpropertiesConn, Pred> componentpropertiesConn_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::componentpropertiesConn, Pred>(impl, meta_componentpropertiesConn_children); }

			static ::Uml::CompositionChildRole meta_componentproperties_children;
			Udm::ChildrenAttr< ::SCAML::componentproperties> componentproperties_children() const { return Udm::ChildrenAttr< ::SCAML::componentproperties>(impl, meta_componentproperties_children); }
			template <class Pred> Udm::ChildrenAttr< ::SCAML::componentproperties, Pred> componentproperties_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::componentproperties, Pred>(impl, meta_componentproperties_children); }

			static ::Uml::CompositionChildRole meta_componentfile_children;
			Udm::ChildrenAttr< ::SCAML::componentfile> componentfile_children() const { return Udm::ChildrenAttr< ::SCAML::componentfile>(impl, meta_componentfile_children); }
			template <class Pred> Udm::ChildrenAttr< ::SCAML::componentfile, Pred> componentfile_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::componentfile, Pred>(impl, meta_componentfile_children); }

			static ::Uml::CompositionChildRole meta_service_children;
			Udm::ChildrenAttr< ::SCAML::service> service_children() const { return Udm::ChildrenAttr< ::SCAML::service>(impl, meta_service_children); }
			template <class Pred> Udm::ChildrenAttr< ::SCAML::service, Pred> service_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::service, Pred>(impl, meta_service_children); }

			static ::Uml::CompositionChildRole meta_freestandingtoprovidedConn_children;
			Udm::ChildrenAttr< ::SCAML::freestandingtoprovidedConn> freestandingtoprovidedConn_children() const { return Udm::ChildrenAttr< ::SCAML::freestandingtoprovidedConn>(impl, meta_freestandingtoprovidedConn_children); }
			template <class Pred> Udm::ChildrenAttr< ::SCAML::freestandingtoprovidedConn, Pred> freestandingtoprovidedConn_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::freestandingtoprovidedConn, Pred>(impl, meta_freestandingtoprovidedConn_children); }

			static ::Uml::CompositionChildRole meta_usestofreestandingConn_children;
			Udm::ChildrenAttr< ::SCAML::usestofreestandingConn> usestofreestandingConn_children() const { return Udm::ChildrenAttr< ::SCAML::usestofreestandingConn>(impl, meta_usestofreestandingConn_children); }
			template <class Pred> Udm::ChildrenAttr< ::SCAML::usestofreestandingConn, Pred> usestofreestandingConn_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::usestofreestandingConn, Pred>(impl, meta_usestofreestandingConn_children); }

			static ::Uml::CompositionChildRole meta_usestodestConn_children;
			Udm::ChildrenAttr< ::SCAML::usestodestConn> usestodestConn_children() const { return Udm::ChildrenAttr< ::SCAML::usestodestConn>(impl, meta_usestodestConn_children); }
			template <class Pred> Udm::ChildrenAttr< ::SCAML::usestodestConn, Pred> usestodestConn_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::usestodestConn, Pred>(impl, meta_usestodestConn_children); }

			static ::Uml::CompositionChildRole meta_SoftwareComponentRef_children;
			Udm::ChildrenAttr< ::SCAML::SoftwareComponentRef> SoftwareComponentRef_children() const { return Udm::ChildrenAttr< ::SCAML::SoftwareComponentRef>(impl, meta_SoftwareComponentRef_children); }
			template <class Pred> Udm::ChildrenAttr< ::SCAML::SoftwareComponentRef, Pred> SoftwareComponentRef_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::SoftwareComponentRef, Pred>(impl, meta_SoftwareComponentRef_children); }

			static ::Uml::CompositionChildRole meta_compositepartofdevice_children;
			Udm::ChildrenAttr< ::SCAML::compositepartofdevice> compositepartofdevice_children() const { return Udm::ChildrenAttr< ::SCAML::compositepartofdevice>(impl, meta_compositepartofdevice_children); }
			template <class Pred> Udm::ChildrenAttr< ::SCAML::compositepartofdevice, Pred> compositepartofdevice_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::compositepartofdevice, Pred>(impl, meta_compositepartofdevice_children); }

			static ::Uml::CompositionChildRole meta_deployondevice_children;
			Udm::ChildrenAttr< ::SCAML::deployondevice> deployondevice_children() const { return Udm::ChildrenAttr< ::SCAML::deployondevice>(impl, meta_deployondevice_children); }
			template <class Pred> Udm::ChildrenAttr< ::SCAML::deployondevice, Pred> deployondevice_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::deployondevice, Pred>(impl, meta_deployondevice_children); }

			static ::Uml::CompositionChildRole meta_componentplacement_children;
			Udm::ChildrenAttr< ::SCAML::componentplacement> componentplacement_children() const { return Udm::ChildrenAttr< ::SCAML::componentplacement>(impl, meta_componentplacement_children); }
			template <class Pred> Udm::ChildrenAttr< ::SCAML::componentplacement, Pred> componentplacement_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::componentplacement, Pred>(impl, meta_componentplacement_children); }

			static ::Uml::CompositionChildRole meta_devconfigcomponentfile_children;
			Udm::ChildrenAttr< ::SCAML::devconfigcomponentfile> devconfigcomponentfile_children() const { return Udm::ChildrenAttr< ::SCAML::devconfigcomponentfile>(impl, meta_devconfigcomponentfile_children); }
			template <class Pred> Udm::ChildrenAttr< ::SCAML::devconfigcomponentfile, Pred> devconfigcomponentfile_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::devconfigcomponentfile, Pred>(impl, meta_devconfigcomponentfile_children); }

			static ::Uml::CompositionChildRole meta_devicemanagersoftpkg_children;
			Udm::ChildrenAttr< ::SCAML::devicemanagersoftpkg> devicemanagersoftpkg_children() const { return Udm::ChildrenAttr< ::SCAML::devicemanagersoftpkg>(impl, meta_devicemanagersoftpkg_children); }
			template <class Pred> Udm::ChildrenAttr< ::SCAML::devicemanagersoftpkg, Pred> devicemanagersoftpkg_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::devicemanagersoftpkg, Pred>(impl, meta_devicemanagersoftpkg_children); }

			Udm::ChildrenAttr< ::SCAML::componentplacement_Members_Base> componentplacement_Members_Base_kind_children() const { return Udm::ChildrenAttr< ::SCAML::componentplacement_Members_Base>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::componentplacement_Members_Base, Pred> componentplacement_Members_Base_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::componentplacement_Members_Base, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::usestodestConn> usestodestConn_kind_children() const { return Udm::ChildrenAttr< ::SCAML::usestodestConn>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::usestodestConn, Pred> usestodestConn_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::usestodestConn, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::freestandingtoprovidedConn> freestandingtoprovidedConn_kind_children() const { return Udm::ChildrenAttr< ::SCAML::freestandingtoprovidedConn>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::freestandingtoprovidedConn, Pred> freestandingtoprovidedConn_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::freestandingtoprovidedConn, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::usestofreestandingConn> usestofreestandingConn_kind_children() const { return Udm::ChildrenAttr< ::SCAML::usestofreestandingConn>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::usestofreestandingConn, Pred> usestofreestandingConn_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::usestofreestandingConn, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::thisPort> thisPort_kind_children() const { return Udm::ChildrenAttr< ::SCAML::thisPort>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::thisPort, Pred> thisPort_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::thisPort, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::log> log_kind_children() const { return Udm::ChildrenAttr< ::SCAML::log>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::log, Pred> log_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::log, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::filemanager> filemanager_kind_children() const { return Udm::ChildrenAttr< ::SCAML::filemanager>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::filemanager, Pred> filemanager_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::filemanager, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::namingservice> namingservice_kind_children() const { return Udm::ChildrenAttr< ::SCAML::namingservice>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::namingservice, Pred> namingservice_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::namingservice, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::FreestandingPort> FreestandingPort_kind_children() const { return Udm::ChildrenAttr< ::SCAML::FreestandingPort>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::FreestandingPort, Pred> FreestandingPort_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::FreestandingPort, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::service> service_kind_children() const { return Udm::ChildrenAttr< ::SCAML::service>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::service, Pred> service_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::service, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::componentpropertiesConn> componentpropertiesConn_kind_children() const { return Udm::ChildrenAttr< ::SCAML::componentpropertiesConn>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::componentpropertiesConn, Pred> componentpropertiesConn_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::componentpropertiesConn, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::componentproperties> componentproperties_kind_children() const { return Udm::ChildrenAttr< ::SCAML::componentproperties>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::componentproperties, Pred> componentproperties_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::componentproperties, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::devicepkgfile> devicepkgfile_kind_children() const { return Udm::ChildrenAttr< ::SCAML::devicepkgfile>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::devicepkgfile, Pred> devicepkgfile_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::devicepkgfile, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::filesystemname> filesystemname_kind_children() const { return Udm::ChildrenAttr< ::SCAML::filesystemname>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::filesystemname, Pred> filesystemname_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::filesystemname, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::componentfile> componentfile_kind_children() const { return Udm::ChildrenAttr< ::SCAML::componentfile>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::componentfile, Pred> componentfile_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::componentfile, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::compositepartofdevice> compositepartofdevice_kind_children() const { return Udm::ChildrenAttr< ::SCAML::compositepartofdevice>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::compositepartofdevice, Pred> compositepartofdevice_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::compositepartofdevice, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::deployondevice> deployondevice_kind_children() const { return Udm::ChildrenAttr< ::SCAML::deployondevice>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::deployondevice, Pred> deployondevice_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::deployondevice, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::componentplacement> componentplacement_kind_children() const { return Udm::ChildrenAttr< ::SCAML::componentplacement>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::componentplacement, Pred> componentplacement_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::componentplacement, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::devconfigcomponentfile> devconfigcomponentfile_kind_children() const { return Udm::ChildrenAttr< ::SCAML::devconfigcomponentfile>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::devconfigcomponentfile, Pred> devconfigcomponentfile_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::devconfigcomponentfile, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::devicemanagersoftpkg> devicemanagersoftpkg_kind_children() const { return Udm::ChildrenAttr< ::SCAML::devicemanagersoftpkg>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::devicemanagersoftpkg, Pred> devicemanagersoftpkg_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::devicemanagersoftpkg, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::SimulinkImplementation> SimulinkImplementation_kind_children() const { return Udm::ChildrenAttr< ::SCAML::SimulinkImplementation>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::SimulinkImplementation, Pred> SimulinkImplementation_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::SimulinkImplementation, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::SoftwareComponentRef> SoftwareComponentRef_kind_children() const { return Udm::ChildrenAttr< ::SCAML::SoftwareComponentRef>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::SoftwareComponentRef, Pred> SoftwareComponentRef_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::SoftwareComponentRef, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::DevicePkgRef> DevicePkgRef_kind_children() const { return Udm::ChildrenAttr< ::SCAML::DevicePkgRef>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::DevicePkgRef, Pred> DevicePkgRef_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::DevicePkgRef, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::SoftwarePackageRef> SoftwarePackageRef_kind_children() const { return Udm::ChildrenAttr< ::SCAML::SoftwarePackageRef>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::SoftwarePackageRef, Pred> SoftwarePackageRef_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::SoftwarePackageRef, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::SoftwareComponent> SoftwareComponent_kind_children() const { return Udm::ChildrenAttr< ::SCAML::SoftwareComponent>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::SoftwareComponent, Pred> SoftwareComponent_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::SoftwareComponent, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::SCAML::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr< ::SCAML::MgaObject>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::SCAML::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::SCAML::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

			static ::Uml::CompositionParentRole meta_DeviceConfigurationFolder_parent;
			Udm::ParentAttr< ::SCAML::DeviceConfigurationFolder> DeviceConfigurationFolder_parent() const { return Udm::ParentAttr< ::SCAML::DeviceConfigurationFolder>(impl, meta_DeviceConfigurationFolder_parent); }

			Udm::ParentAttr< ::SCAML::DeviceConfigurationFolder> parent() const { return Udm::ParentAttr< ::SCAML::DeviceConfigurationFolder>(impl, Udm::NULLPARENTROLE); }
		};



}

#endif //MOBIES_SCAML_H
