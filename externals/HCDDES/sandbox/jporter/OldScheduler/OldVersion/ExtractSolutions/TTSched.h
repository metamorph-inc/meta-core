#ifndef MOBIES_TTSCHED_H
#define MOBIES_TTSCHED_H
// header file TTSched.h generated from diagram TTSched
// generated on Thu Apr 12 20:29:49 2007

#ifndef MOBIES_UDMBASE_H
#include "UdmBase.h"
#endif

#ifdef min
#undef min
#endif

#ifdef max
#undef max
#endif
namespace TTSched {
	extern  Udm::UdmDiagram diagram;
	 void Initialize(const ::Uml::Diagram &dgr);
	 void Initialize();

		class  RangeConstraint;
		class  Assignment;
		class  Connectable;
		class  InstanceRef;
		class  PairConstraint;
		class  InequalityConstraint;
		class  Solution;
		class  Constraint;
		class  YPort;
		class  Member;
		class  XPort;
		class  ConstraintProblem;
		class  SerialConstraint;
		class  FDVar;
		class  SearchConfig;
		class  EqualityConstraint;
		class  TimeSync;
		class  Dependency;
		class  Task;
		class  Message;
		class  TTSystem;
		class  Latency;
		class  Sender;
		class  LatencySpec;
		class  EndMsg;
		class  StartMsg;
		class  Receiver;
		class  TaskRef;
		class  Processor;
		class  Link;
		class  Bus;
		class  MsgRef;
		class  Dependencies;
		class  MsgInstRef;
		class  ReceiverInst;
		class  TaskInstRef;
		class  SenderInst;
		class  InstanceModel;
		class  MsgInstances;
		class  TaskInstances;
		class  MsgInstance;
		class  TaskInstance;
		class  RootFolder;
		class  MgaObject;


		 void CreateMetaObjs();
		 void InitCrossNSInheritence();
		 void InitCrossNSCompositions();
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

			Udm::InstantiatedAttr< ::TTSched::RootFolder> Instances() { return Udm::InstantiatedAttr< ::TTSched::RootFolder>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::TTSched::RootFolder, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::TTSched::RootFolder, Pred>(impl);}

			Udm::DerivedAttr< ::TTSched::RootFolder> Derived() { return Udm::DerivedAttr< ::TTSched::RootFolder>(impl);}
			template <class Pred> Udm::DerivedAttr< ::TTSched::RootFolder, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::TTSched::RootFolder, Pred>(impl);}

			Udm::ArchetypeAttr< ::TTSched::RootFolder> Archetype() { return Udm::ArchetypeAttr< ::TTSched::RootFolder>(impl);}

			static ::Uml::Attribute meta_name;
			Udm::StringAttr name() const { return Udm::StringAttr(impl, meta_name); }

			static ::Uml::CompositionChildRole meta_TTSystem_children;
			Udm::ChildrenAttr< ::TTSched::TTSystem> TTSystem_children() const { return Udm::ChildrenAttr< ::TTSched::TTSystem>(impl, meta_TTSystem_children); }
			template <class Pred> Udm::ChildrenAttr< ::TTSched::TTSystem, Pred> TTSystem_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::TTSched::TTSystem, Pred>(impl, meta_TTSystem_children); }

			static ::Uml::CompositionChildRole meta_ConstraintProblem_children;
			Udm::ChildrenAttr< ::TTSched::ConstraintProblem> ConstraintProblem_children() const { return Udm::ChildrenAttr< ::TTSched::ConstraintProblem>(impl, meta_ConstraintProblem_children); }
			template <class Pred> Udm::ChildrenAttr< ::TTSched::ConstraintProblem, Pred> ConstraintProblem_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::TTSched::ConstraintProblem, Pred>(impl, meta_ConstraintProblem_children); }

			static ::Uml::CompositionChildRole meta_InstanceModel_children;
			Udm::ChildrenAttr< ::TTSched::InstanceModel> InstanceModel_children() const { return Udm::ChildrenAttr< ::TTSched::InstanceModel>(impl, meta_InstanceModel_children); }
			template <class Pred> Udm::ChildrenAttr< ::TTSched::InstanceModel, Pred> InstanceModel_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::TTSched::InstanceModel, Pred>(impl, meta_InstanceModel_children); }

			static ::Uml::CompositionChildRole meta_RootFolder_children;
			Udm::ChildrenAttr< ::TTSched::RootFolder> RootFolder_children() const { return Udm::ChildrenAttr< ::TTSched::RootFolder>(impl, meta_RootFolder_children); }
			template <class Pred> Udm::ChildrenAttr< ::TTSched::RootFolder, Pred> RootFolder_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::TTSched::RootFolder, Pred>(impl, meta_RootFolder_children); }

			Udm::ChildrenAttr< ::TTSched::ConstraintProblem> ConstraintProblem_kind_children() const { return Udm::ChildrenAttr< ::TTSched::ConstraintProblem>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::TTSched::ConstraintProblem, Pred> ConstraintProblem_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::TTSched::ConstraintProblem, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::TTSched::TTSystem> TTSystem_kind_children() const { return Udm::ChildrenAttr< ::TTSched::TTSystem>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::TTSched::TTSystem, Pred> TTSystem_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::TTSched::TTSystem, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::TTSched::InstanceModel> InstanceModel_kind_children() const { return Udm::ChildrenAttr< ::TTSched::InstanceModel>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::TTSched::InstanceModel, Pred> InstanceModel_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::TTSched::InstanceModel, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::TTSched::RootFolder> RootFolder_kind_children() const { return Udm::ChildrenAttr< ::TTSched::RootFolder>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::TTSched::RootFolder, Pred> RootFolder_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::TTSched::RootFolder, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::TTSched::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr< ::TTSched::MgaObject>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::TTSched::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::TTSched::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

			static ::Uml::CompositionParentRole meta_RootFolder_parent;
			Udm::ParentAttr< ::TTSched::RootFolder> RootFolder_parent() const { return Udm::ParentAttr< ::TTSched::RootFolder>(impl, meta_RootFolder_parent); }

			Udm::ParentAttr< ::TTSched::RootFolder> parent() const { return Udm::ParentAttr< ::TTSched::RootFolder>(impl, Udm::NULLPARENTROLE); }
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

			Udm::InstantiatedAttr< ::TTSched::MgaObject> Instances() { return Udm::InstantiatedAttr< ::TTSched::MgaObject>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::TTSched::MgaObject, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::TTSched::MgaObject, Pred>(impl);}

			Udm::DerivedAttr< ::TTSched::MgaObject> Derived() { return Udm::DerivedAttr< ::TTSched::MgaObject>(impl);}
			template <class Pred> Udm::DerivedAttr< ::TTSched::MgaObject, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::TTSched::MgaObject, Pred>(impl);}

			Udm::ArchetypeAttr< ::TTSched::MgaObject> Archetype() { return Udm::ArchetypeAttr< ::TTSched::MgaObject>(impl);}

			static ::Uml::Attribute meta_position;
			Udm::StringAttr position() const { return Udm::StringAttr(impl, meta_position); }

			static ::Uml::Attribute meta_name;
			Udm::StringAttr name() const { return Udm::StringAttr(impl, meta_name); }

			Udm::ParentAttr<Udm::Object> parent() const { return Udm::ParentAttr<Udm::Object>(impl, Udm::NULLPARENTROLE); }
		};

		class  Assignment :  public ::TTSched::MgaObject {
		public:
			static ::Uml::Class meta;

			Assignment() { }
			Assignment(Udm::ObjectImpl *impl) : ::TTSched::MgaObject(impl) { }
			Assignment(const Assignment &master) : ::TTSched::MgaObject(master) { }
			static Assignment Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static Assignment Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			Assignment CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			Assignment CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::TTSched::Assignment> Instances() { return Udm::InstantiatedAttr< ::TTSched::Assignment>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::TTSched::Assignment, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::TTSched::Assignment, Pred>(impl);}

			Udm::DerivedAttr< ::TTSched::Assignment> Derived() { return Udm::DerivedAttr< ::TTSched::Assignment>(impl);}
			template <class Pred> Udm::DerivedAttr< ::TTSched::Assignment, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::TTSched::Assignment, Pred>(impl);}

			Udm::ArchetypeAttr< ::TTSched::Assignment> Archetype() { return Udm::ArchetypeAttr< ::TTSched::Assignment>(impl);}

			static ::Uml::Attribute meta_Value;
			Udm::IntegerAttr Value() const { return Udm::IntegerAttr(impl, meta_Value); }

			static ::Uml::CompositionParentRole meta_Solution_parent;
			Udm::ParentAttr< ::TTSched::Solution> Solution_parent() const { return Udm::ParentAttr< ::TTSched::Solution>(impl, meta_Solution_parent); }

			Udm::ParentAttr< ::TTSched::MgaObject> parent() const { return Udm::ParentAttr< ::TTSched::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  Connectable :  virtual public ::TTSched::MgaObject {
		public:
			static ::Uml::Class meta;

			Connectable() { }
			Connectable(Udm::ObjectImpl *impl) : ::TTSched::MgaObject(impl) { }
			Connectable(const Connectable &master) : ::TTSched::MgaObject(master) { }
			static Connectable Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static Connectable Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			Connectable CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			Connectable CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::TTSched::Connectable> Instances() { return Udm::InstantiatedAttr< ::TTSched::Connectable>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::TTSched::Connectable, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::TTSched::Connectable, Pred>(impl);}

			Udm::DerivedAttr< ::TTSched::Connectable> Derived() { return Udm::DerivedAttr< ::TTSched::Connectable>(impl);}
			template <class Pred> Udm::DerivedAttr< ::TTSched::Connectable, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::TTSched::Connectable, Pred>(impl);}

			Udm::ArchetypeAttr< ::TTSched::Connectable> Archetype() { return Udm::ArchetypeAttr< ::TTSched::Connectable>(impl);}

			static ::Uml::AssociationRole meta_srcMember;
			static ::Uml::AssociationRole meta_srcMember_rev;
			Udm::AClassAssocAttr< ::TTSched::Member, ::TTSched::FDVar> srcMember() const { return Udm::AClassAssocAttr< ::TTSched::Member, ::TTSched::FDVar>(impl, meta_srcMember, meta_srcMember_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::TTSched::Member, ::TTSched::FDVar, Pred> srcMember_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::TTSched::Member, ::TTSched::FDVar, Pred>(impl, meta_srcMember, meta_srcMember_rev); }

			Udm::ParentAttr<Udm::Object> parent() const { return Udm::ParentAttr<Udm::Object>(impl, Udm::NULLPARENTROLE); }
		};

		class  InstanceRef :  public ::TTSched::MgaObject {
		public:
			static ::Uml::Class meta;

			InstanceRef() { }
			InstanceRef(Udm::ObjectImpl *impl) : ::TTSched::MgaObject(impl) { }
			InstanceRef(const InstanceRef &master) : ::TTSched::MgaObject(master) { }
			static InstanceRef Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static InstanceRef Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			InstanceRef CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			InstanceRef CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::TTSched::InstanceRef> Instances() { return Udm::InstantiatedAttr< ::TTSched::InstanceRef>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::TTSched::InstanceRef, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::TTSched::InstanceRef, Pred>(impl);}

			Udm::DerivedAttr< ::TTSched::InstanceRef> Derived() { return Udm::DerivedAttr< ::TTSched::InstanceRef>(impl);}
			template <class Pred> Udm::DerivedAttr< ::TTSched::InstanceRef, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::TTSched::InstanceRef, Pred>(impl);}

			Udm::ArchetypeAttr< ::TTSched::InstanceRef> Archetype() { return Udm::ArchetypeAttr< ::TTSched::InstanceRef>(impl);}

			static ::Uml::AssociationRole meta_ref;
			Udm::PointerAttr< ::TTSched::InstanceModel> ref() const { return Udm::PointerAttr< ::TTSched::InstanceModel>(impl, meta_ref); }

			static ::Uml::CompositionParentRole meta_ConstraintProblem_parent;
			Udm::ParentAttr< ::TTSched::ConstraintProblem> ConstraintProblem_parent() const { return Udm::ParentAttr< ::TTSched::ConstraintProblem>(impl, meta_ConstraintProblem_parent); }

			Udm::ParentAttr< ::TTSched::MgaObject> parent() const { return Udm::ParentAttr< ::TTSched::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  Solution :  public ::TTSched::MgaObject {
		public:
			static ::Uml::Class meta;

			Solution() { }
			Solution(Udm::ObjectImpl *impl) : ::TTSched::MgaObject(impl) { }
			Solution(const Solution &master) : ::TTSched::MgaObject(master) { }
			static Solution Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static Solution Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			Solution CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			Solution CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::TTSched::Solution> Instances() { return Udm::InstantiatedAttr< ::TTSched::Solution>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::TTSched::Solution, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::TTSched::Solution, Pred>(impl);}

			Udm::DerivedAttr< ::TTSched::Solution> Derived() { return Udm::DerivedAttr< ::TTSched::Solution>(impl);}
			template <class Pred> Udm::DerivedAttr< ::TTSched::Solution, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::TTSched::Solution, Pred>(impl);}

			Udm::ArchetypeAttr< ::TTSched::Solution> Archetype() { return Udm::ArchetypeAttr< ::TTSched::Solution>(impl);}

			static ::Uml::CompositionChildRole meta_Assignment_children;
			Udm::ChildrenAttr< ::TTSched::Assignment> Assignment_children() const { return Udm::ChildrenAttr< ::TTSched::Assignment>(impl, meta_Assignment_children); }
			template <class Pred> Udm::ChildrenAttr< ::TTSched::Assignment, Pred> Assignment_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::TTSched::Assignment, Pred>(impl, meta_Assignment_children); }

			Udm::ChildrenAttr< ::TTSched::Assignment> Assignment_kind_children() const { return Udm::ChildrenAttr< ::TTSched::Assignment>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::TTSched::Assignment, Pred> Assignment_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::TTSched::Assignment, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::TTSched::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr< ::TTSched::MgaObject>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::TTSched::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::TTSched::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

			static ::Uml::CompositionParentRole meta_ConstraintProblem_parent;
			Udm::ParentAttr< ::TTSched::ConstraintProblem> ConstraintProblem_parent() const { return Udm::ParentAttr< ::TTSched::ConstraintProblem>(impl, meta_ConstraintProblem_parent); }

			Udm::ParentAttr< ::TTSched::MgaObject> parent() const { return Udm::ParentAttr< ::TTSched::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  Constraint :  virtual public ::TTSched::MgaObject {
		public:
			static ::Uml::Class meta;

			Constraint() { }
			Constraint(Udm::ObjectImpl *impl) : ::TTSched::MgaObject(impl) { }
			Constraint(const Constraint &master) : ::TTSched::MgaObject(master) { }
			static Constraint Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static Constraint Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			Constraint CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			Constraint CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::TTSched::Constraint> Instances() { return Udm::InstantiatedAttr< ::TTSched::Constraint>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::TTSched::Constraint, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::TTSched::Constraint, Pred>(impl);}

			Udm::DerivedAttr< ::TTSched::Constraint> Derived() { return Udm::DerivedAttr< ::TTSched::Constraint>(impl);}
			template <class Pred> Udm::DerivedAttr< ::TTSched::Constraint, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::TTSched::Constraint, Pred>(impl);}

			Udm::ArchetypeAttr< ::TTSched::Constraint> Archetype() { return Udm::ArchetypeAttr< ::TTSched::Constraint>(impl);}

			static ::Uml::CompositionParentRole meta_ConstraintProblem_parent;
			Udm::ParentAttr< ::TTSched::ConstraintProblem> ConstraintProblem_parent() const { return Udm::ParentAttr< ::TTSched::ConstraintProblem>(impl, meta_ConstraintProblem_parent); }

			Udm::ParentAttr< ::TTSched::MgaObject> parent() const { return Udm::ParentAttr< ::TTSched::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  RangeConstraint :  public ::TTSched::Connectable, public ::TTSched::Constraint {
		public:
			static ::Uml::Class meta;

			RangeConstraint() { }
			RangeConstraint(Udm::ObjectImpl *impl) : ::TTSched::Connectable(impl),::TTSched::Constraint(impl), ::TTSched::MgaObject(impl) { }
			RangeConstraint(const RangeConstraint &master) : ::TTSched::Connectable(master),::TTSched::Constraint(master), ::TTSched::MgaObject(master) { }
			static RangeConstraint Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static RangeConstraint Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			RangeConstraint CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			RangeConstraint CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::TTSched::RangeConstraint> Instances() { return Udm::InstantiatedAttr< ::TTSched::RangeConstraint>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::TTSched::RangeConstraint, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::TTSched::RangeConstraint, Pred>(impl);}

			Udm::DerivedAttr< ::TTSched::RangeConstraint> Derived() { return Udm::DerivedAttr< ::TTSched::RangeConstraint>(impl);}
			template <class Pred> Udm::DerivedAttr< ::TTSched::RangeConstraint, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::TTSched::RangeConstraint, Pred>(impl);}

			Udm::ArchetypeAttr< ::TTSched::RangeConstraint> Archetype() { return Udm::ArchetypeAttr< ::TTSched::RangeConstraint>(impl);}

			static ::Uml::Attribute meta_UpperBound;
			Udm::IntegerAttr UpperBound() const { return Udm::IntegerAttr(impl, meta_UpperBound); }

			static ::Uml::Attribute meta_LowerBound;
			Udm::IntegerAttr LowerBound() const { return Udm::IntegerAttr(impl, meta_LowerBound); }

			Udm::ParentAttr< ::TTSched::MgaObject> parent() const { return Udm::ParentAttr< ::TTSched::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  PairConstraint :  public ::TTSched::Constraint {
		public:
			static ::Uml::Class meta;

			PairConstraint() { }
			PairConstraint(Udm::ObjectImpl *impl) : ::TTSched::Constraint(impl), ::TTSched::MgaObject(impl) { }
			PairConstraint(const PairConstraint &master) : ::TTSched::Constraint(master), ::TTSched::MgaObject(master) { }
			static PairConstraint Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static PairConstraint Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			PairConstraint CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			PairConstraint CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::TTSched::PairConstraint> Instances() { return Udm::InstantiatedAttr< ::TTSched::PairConstraint>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::TTSched::PairConstraint, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::TTSched::PairConstraint, Pred>(impl);}

			Udm::DerivedAttr< ::TTSched::PairConstraint> Derived() { return Udm::DerivedAttr< ::TTSched::PairConstraint>(impl);}
			template <class Pred> Udm::DerivedAttr< ::TTSched::PairConstraint, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::TTSched::PairConstraint, Pred>(impl);}

			Udm::ArchetypeAttr< ::TTSched::PairConstraint> Archetype() { return Udm::ArchetypeAttr< ::TTSched::PairConstraint>(impl);}

			static ::Uml::CompositionChildRole meta_YPort_children;
			Udm::ChildrenAttr< ::TTSched::YPort> YPort_children() const { return Udm::ChildrenAttr< ::TTSched::YPort>(impl, meta_YPort_children); }
			template <class Pred> Udm::ChildrenAttr< ::TTSched::YPort, Pred> YPort_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::TTSched::YPort, Pred>(impl, meta_YPort_children); }

			static ::Uml::CompositionChildRole meta_XPort_children;
			Udm::ChildrenAttr< ::TTSched::XPort> XPort_children() const { return Udm::ChildrenAttr< ::TTSched::XPort>(impl, meta_XPort_children); }
			template <class Pred> Udm::ChildrenAttr< ::TTSched::XPort, Pred> XPort_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::TTSched::XPort, Pred>(impl, meta_XPort_children); }

			Udm::ChildrenAttr< ::TTSched::Connectable> Connectable_kind_children() const { return Udm::ChildrenAttr< ::TTSched::Connectable>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::TTSched::Connectable, Pred> Connectable_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::TTSched::Connectable, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::TTSched::YPort> YPort_kind_children() const { return Udm::ChildrenAttr< ::TTSched::YPort>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::TTSched::YPort, Pred> YPort_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::TTSched::YPort, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::TTSched::XPort> XPort_kind_children() const { return Udm::ChildrenAttr< ::TTSched::XPort>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::TTSched::XPort, Pred> XPort_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::TTSched::XPort, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::TTSched::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr< ::TTSched::MgaObject>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::TTSched::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::TTSched::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ParentAttr< ::TTSched::MgaObject> parent() const { return Udm::ParentAttr< ::TTSched::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  InequalityConstraint :  public ::TTSched::PairConstraint {
		public:
			static ::Uml::Class meta;

			InequalityConstraint() { }
			InequalityConstraint(Udm::ObjectImpl *impl) : ::TTSched::PairConstraint(impl), ::TTSched::MgaObject(impl) { }
			InequalityConstraint(const InequalityConstraint &master) : ::TTSched::PairConstraint(master), ::TTSched::MgaObject(master) { }
			static InequalityConstraint Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static InequalityConstraint Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			InequalityConstraint CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			InequalityConstraint CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::TTSched::InequalityConstraint> Instances() { return Udm::InstantiatedAttr< ::TTSched::InequalityConstraint>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::TTSched::InequalityConstraint, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::TTSched::InequalityConstraint, Pred>(impl);}

			Udm::DerivedAttr< ::TTSched::InequalityConstraint> Derived() { return Udm::DerivedAttr< ::TTSched::InequalityConstraint>(impl);}
			template <class Pred> Udm::DerivedAttr< ::TTSched::InequalityConstraint, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::TTSched::InequalityConstraint, Pred>(impl);}

			Udm::ArchetypeAttr< ::TTSched::InequalityConstraint> Archetype() { return Udm::ArchetypeAttr< ::TTSched::InequalityConstraint>(impl);}

			static ::Uml::Attribute meta_Offset;
			Udm::IntegerAttr Offset() const { return Udm::IntegerAttr(impl, meta_Offset); }

			Udm::ParentAttr< ::TTSched::MgaObject> parent() const { return Udm::ParentAttr< ::TTSched::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  YPort :  public ::TTSched::Connectable {
		public:
			static ::Uml::Class meta;

			YPort() { }
			YPort(Udm::ObjectImpl *impl) : ::TTSched::Connectable(impl), ::TTSched::MgaObject(impl) { }
			YPort(const YPort &master) : ::TTSched::Connectable(master), ::TTSched::MgaObject(master) { }
			static YPort Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static YPort Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			YPort CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			YPort CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::TTSched::YPort> Instances() { return Udm::InstantiatedAttr< ::TTSched::YPort>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::TTSched::YPort, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::TTSched::YPort, Pred>(impl);}

			Udm::DerivedAttr< ::TTSched::YPort> Derived() { return Udm::DerivedAttr< ::TTSched::YPort>(impl);}
			template <class Pred> Udm::DerivedAttr< ::TTSched::YPort, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::TTSched::YPort, Pred>(impl);}

			Udm::ArchetypeAttr< ::TTSched::YPort> Archetype() { return Udm::ArchetypeAttr< ::TTSched::YPort>(impl);}

			static ::Uml::CompositionParentRole meta_PairConstraint_parent;
			Udm::ParentAttr< ::TTSched::PairConstraint> PairConstraint_parent() const { return Udm::ParentAttr< ::TTSched::PairConstraint>(impl, meta_PairConstraint_parent); }

			Udm::ParentAttr< ::TTSched::MgaObject> parent() const { return Udm::ParentAttr< ::TTSched::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  Member :  public ::TTSched::MgaObject {
		public:
			static ::Uml::Class meta;

			Member() { }
			Member(Udm::ObjectImpl *impl) : ::TTSched::MgaObject(impl) { }
			Member(const Member &master) : ::TTSched::MgaObject(master) { }
			static Member Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static Member Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			Member CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			Member CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::TTSched::Member> Instances() { return Udm::InstantiatedAttr< ::TTSched::Member>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::TTSched::Member, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::TTSched::Member, Pred>(impl);}

			Udm::DerivedAttr< ::TTSched::Member> Derived() { return Udm::DerivedAttr< ::TTSched::Member>(impl);}
			template <class Pred> Udm::DerivedAttr< ::TTSched::Member, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::TTSched::Member, Pred>(impl);}

			Udm::ArchetypeAttr< ::TTSched::Member> Archetype() { return Udm::ArchetypeAttr< ::TTSched::Member>(impl);}

			static ::Uml::CompositionParentRole meta_ConstraintProblem_parent;
			Udm::ParentAttr< ::TTSched::ConstraintProblem> ConstraintProblem_parent() const { return Udm::ParentAttr< ::TTSched::ConstraintProblem>(impl, meta_ConstraintProblem_parent); }

			Udm::ParentAttr< ::TTSched::MgaObject> parent() const { return Udm::ParentAttr< ::TTSched::MgaObject>(impl, Udm::NULLPARENTROLE); }
			static ::Uml::AssociationRole meta_dstMember_end_;
			Udm::AssocEndAttr< ::TTSched::Connectable> dstMember_end() const { return Udm::AssocEndAttr< ::TTSched::Connectable>(impl, meta_dstMember_end_); }

			static ::Uml::AssociationRole meta_srcMember_end_;
			Udm::AssocEndAttr< ::TTSched::FDVar> srcMember_end() const { return Udm::AssocEndAttr< ::TTSched::FDVar>(impl, meta_srcMember_end_); }

		};

		class  XPort :  public ::TTSched::Connectable {
		public:
			static ::Uml::Class meta;

			XPort() { }
			XPort(Udm::ObjectImpl *impl) : ::TTSched::Connectable(impl), ::TTSched::MgaObject(impl) { }
			XPort(const XPort &master) : ::TTSched::Connectable(master), ::TTSched::MgaObject(master) { }
			static XPort Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static XPort Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			XPort CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			XPort CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::TTSched::XPort> Instances() { return Udm::InstantiatedAttr< ::TTSched::XPort>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::TTSched::XPort, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::TTSched::XPort, Pred>(impl);}

			Udm::DerivedAttr< ::TTSched::XPort> Derived() { return Udm::DerivedAttr< ::TTSched::XPort>(impl);}
			template <class Pred> Udm::DerivedAttr< ::TTSched::XPort, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::TTSched::XPort, Pred>(impl);}

			Udm::ArchetypeAttr< ::TTSched::XPort> Archetype() { return Udm::ArchetypeAttr< ::TTSched::XPort>(impl);}

			static ::Uml::CompositionParentRole meta_PairConstraint_parent;
			Udm::ParentAttr< ::TTSched::PairConstraint> PairConstraint_parent() const { return Udm::ParentAttr< ::TTSched::PairConstraint>(impl, meta_PairConstraint_parent); }

			Udm::ParentAttr< ::TTSched::MgaObject> parent() const { return Udm::ParentAttr< ::TTSched::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  ConstraintProblem :  public ::TTSched::MgaObject {
		public:
			static ::Uml::Class meta;

			ConstraintProblem() { }
			ConstraintProblem(Udm::ObjectImpl *impl) : ::TTSched::MgaObject(impl) { }
			ConstraintProblem(const ConstraintProblem &master) : ::TTSched::MgaObject(master) { }
			static ConstraintProblem Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static ConstraintProblem Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			ConstraintProblem CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			ConstraintProblem CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::TTSched::ConstraintProblem> Instances() { return Udm::InstantiatedAttr< ::TTSched::ConstraintProblem>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::TTSched::ConstraintProblem, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::TTSched::ConstraintProblem, Pred>(impl);}

			Udm::DerivedAttr< ::TTSched::ConstraintProblem> Derived() { return Udm::DerivedAttr< ::TTSched::ConstraintProblem>(impl);}
			template <class Pred> Udm::DerivedAttr< ::TTSched::ConstraintProblem, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::TTSched::ConstraintProblem, Pred>(impl);}

			Udm::ArchetypeAttr< ::TTSched::ConstraintProblem> Archetype() { return Udm::ArchetypeAttr< ::TTSched::ConstraintProblem>(impl);}

			static ::Uml::Attribute meta_UpperBound;
			Udm::IntegerAttr UpperBound() const { return Udm::IntegerAttr(impl, meta_UpperBound); }

			static ::Uml::Attribute meta_LowerBound;
			Udm::IntegerAttr LowerBound() const { return Udm::IntegerAttr(impl, meta_LowerBound); }

			static ::Uml::CompositionChildRole meta_Solution_children;
			Udm::ChildrenAttr< ::TTSched::Solution> Solution_children() const { return Udm::ChildrenAttr< ::TTSched::Solution>(impl, meta_Solution_children); }
			template <class Pred> Udm::ChildrenAttr< ::TTSched::Solution, Pred> Solution_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::TTSched::Solution, Pred>(impl, meta_Solution_children); }

			static ::Uml::CompositionChildRole meta_SearchConfig_child;
			Udm::ChildAttr< ::TTSched::SearchConfig> SearchConfig_child() const { return Udm::ChildAttr< ::TTSched::SearchConfig>(impl, meta_SearchConfig_child); }

			static ::Uml::CompositionChildRole meta_InstanceRef_child;
			Udm::ChildAttr< ::TTSched::InstanceRef> InstanceRef_child() const { return Udm::ChildAttr< ::TTSched::InstanceRef>(impl, meta_InstanceRef_child); }

			static ::Uml::CompositionChildRole meta_FDVar_children;
			Udm::ChildrenAttr< ::TTSched::FDVar> FDVar_children() const { return Udm::ChildrenAttr< ::TTSched::FDVar>(impl, meta_FDVar_children); }
			template <class Pred> Udm::ChildrenAttr< ::TTSched::FDVar, Pred> FDVar_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::TTSched::FDVar, Pred>(impl, meta_FDVar_children); }

			static ::Uml::CompositionChildRole meta_Constraint_children;
			Udm::ChildrenAttr< ::TTSched::Constraint> Constraint_children() const { return Udm::ChildrenAttr< ::TTSched::Constraint>(impl, meta_Constraint_children); }
			template <class Pred> Udm::ChildrenAttr< ::TTSched::Constraint, Pred> Constraint_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::TTSched::Constraint, Pred>(impl, meta_Constraint_children); }

			static ::Uml::CompositionChildRole meta_Member_children;
			Udm::ChildrenAttr< ::TTSched::Member> Member_children() const { return Udm::ChildrenAttr< ::TTSched::Member>(impl, meta_Member_children); }
			template <class Pred> Udm::ChildrenAttr< ::TTSched::Member, Pred> Member_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::TTSched::Member, Pred>(impl, meta_Member_children); }

			Udm::ChildrenAttr< ::TTSched::RangeConstraint> RangeConstraint_kind_children() const { return Udm::ChildrenAttr< ::TTSched::RangeConstraint>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::TTSched::RangeConstraint, Pred> RangeConstraint_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::TTSched::RangeConstraint, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::TTSched::InstanceRef> InstanceRef_kind_children() const { return Udm::ChildrenAttr< ::TTSched::InstanceRef>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::TTSched::InstanceRef, Pred> InstanceRef_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::TTSched::InstanceRef, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::TTSched::PairConstraint> PairConstraint_kind_children() const { return Udm::ChildrenAttr< ::TTSched::PairConstraint>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::TTSched::PairConstraint, Pred> PairConstraint_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::TTSched::PairConstraint, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::TTSched::InequalityConstraint> InequalityConstraint_kind_children() const { return Udm::ChildrenAttr< ::TTSched::InequalityConstraint>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::TTSched::InequalityConstraint, Pred> InequalityConstraint_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::TTSched::InequalityConstraint, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::TTSched::Solution> Solution_kind_children() const { return Udm::ChildrenAttr< ::TTSched::Solution>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::TTSched::Solution, Pred> Solution_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::TTSched::Solution, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::TTSched::Constraint> Constraint_kind_children() const { return Udm::ChildrenAttr< ::TTSched::Constraint>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::TTSched::Constraint, Pred> Constraint_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::TTSched::Constraint, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::TTSched::Member> Member_kind_children() const { return Udm::ChildrenAttr< ::TTSched::Member>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::TTSched::Member, Pred> Member_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::TTSched::Member, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::TTSched::SerialConstraint> SerialConstraint_kind_children() const { return Udm::ChildrenAttr< ::TTSched::SerialConstraint>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::TTSched::SerialConstraint, Pred> SerialConstraint_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::TTSched::SerialConstraint, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::TTSched::FDVar> FDVar_kind_children() const { return Udm::ChildrenAttr< ::TTSched::FDVar>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::TTSched::FDVar, Pred> FDVar_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::TTSched::FDVar, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::TTSched::SearchConfig> SearchConfig_kind_children() const { return Udm::ChildrenAttr< ::TTSched::SearchConfig>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::TTSched::SearchConfig, Pred> SearchConfig_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::TTSched::SearchConfig, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::TTSched::EqualityConstraint> EqualityConstraint_kind_children() const { return Udm::ChildrenAttr< ::TTSched::EqualityConstraint>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::TTSched::EqualityConstraint, Pred> EqualityConstraint_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::TTSched::EqualityConstraint, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::TTSched::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr< ::TTSched::MgaObject>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::TTSched::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::TTSched::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

			static ::Uml::CompositionParentRole meta_RootFolder_parent;
			Udm::ParentAttr< ::TTSched::RootFolder> RootFolder_parent() const { return Udm::ParentAttr< ::TTSched::RootFolder>(impl, meta_RootFolder_parent); }

			Udm::ParentAttr< ::TTSched::RootFolder> parent() const { return Udm::ParentAttr< ::TTSched::RootFolder>(impl, Udm::NULLPARENTROLE); }
		};

		class  SerialConstraint :  public ::TTSched::Connectable, public ::TTSched::Constraint {
		public:
			static ::Uml::Class meta;

			SerialConstraint() { }
			SerialConstraint(Udm::ObjectImpl *impl) : ::TTSched::Connectable(impl),::TTSched::Constraint(impl), ::TTSched::MgaObject(impl) { }
			SerialConstraint(const SerialConstraint &master) : ::TTSched::Connectable(master),::TTSched::Constraint(master), ::TTSched::MgaObject(master) { }
			static SerialConstraint Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static SerialConstraint Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			SerialConstraint CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			SerialConstraint CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::TTSched::SerialConstraint> Instances() { return Udm::InstantiatedAttr< ::TTSched::SerialConstraint>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::TTSched::SerialConstraint, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::TTSched::SerialConstraint, Pred>(impl);}

			Udm::DerivedAttr< ::TTSched::SerialConstraint> Derived() { return Udm::DerivedAttr< ::TTSched::SerialConstraint>(impl);}
			template <class Pred> Udm::DerivedAttr< ::TTSched::SerialConstraint, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::TTSched::SerialConstraint, Pred>(impl);}

			Udm::ArchetypeAttr< ::TTSched::SerialConstraint> Archetype() { return Udm::ArchetypeAttr< ::TTSched::SerialConstraint>(impl);}

			Udm::ParentAttr< ::TTSched::MgaObject> parent() const { return Udm::ParentAttr< ::TTSched::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  FDVar :  public ::TTSched::MgaObject {
		public:
			static ::Uml::Class meta;

			FDVar() { }
			FDVar(Udm::ObjectImpl *impl) : ::TTSched::MgaObject(impl) { }
			FDVar(const FDVar &master) : ::TTSched::MgaObject(master) { }
			static FDVar Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static FDVar Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			FDVar CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			FDVar CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::TTSched::FDVar> Instances() { return Udm::InstantiatedAttr< ::TTSched::FDVar>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::TTSched::FDVar, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::TTSched::FDVar, Pred>(impl);}

			Udm::DerivedAttr< ::TTSched::FDVar> Derived() { return Udm::DerivedAttr< ::TTSched::FDVar>(impl);}
			template <class Pred> Udm::DerivedAttr< ::TTSched::FDVar, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::TTSched::FDVar, Pred>(impl);}

			Udm::ArchetypeAttr< ::TTSched::FDVar> Archetype() { return Udm::ArchetypeAttr< ::TTSched::FDVar>(impl);}

			static ::Uml::Attribute meta_UpperBound;
			Udm::IntegerAttr UpperBound() const { return Udm::IntegerAttr(impl, meta_UpperBound); }

			static ::Uml::Attribute meta_LowerBound;
			Udm::IntegerAttr LowerBound() const { return Udm::IntegerAttr(impl, meta_LowerBound); }

			static ::Uml::Attribute meta_Size;
			Udm::IntegerAttr Size() const { return Udm::IntegerAttr(impl, meta_Size); }

			static ::Uml::AssociationRole meta_dstMember;
			static ::Uml::AssociationRole meta_dstMember_rev;
			Udm::AClassAssocAttr< ::TTSched::Member, ::TTSched::Connectable> dstMember() const { return Udm::AClassAssocAttr< ::TTSched::Member, ::TTSched::Connectable>(impl, meta_dstMember, meta_dstMember_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::TTSched::Member, ::TTSched::Connectable, Pred> dstMember_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::TTSched::Member, ::TTSched::Connectable, Pred>(impl, meta_dstMember, meta_dstMember_rev); }

			static ::Uml::CompositionParentRole meta_ConstraintProblem_parent;
			Udm::ParentAttr< ::TTSched::ConstraintProblem> ConstraintProblem_parent() const { return Udm::ParentAttr< ::TTSched::ConstraintProblem>(impl, meta_ConstraintProblem_parent); }

			Udm::ParentAttr< ::TTSched::MgaObject> parent() const { return Udm::ParentAttr< ::TTSched::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  SearchConfig :  public ::TTSched::MgaObject {
		public:
			static ::Uml::Class meta;

			SearchConfig() { }
			SearchConfig(Udm::ObjectImpl *impl) : ::TTSched::MgaObject(impl) { }
			SearchConfig(const SearchConfig &master) : ::TTSched::MgaObject(master) { }
			static SearchConfig Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static SearchConfig Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			SearchConfig CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			SearchConfig CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::TTSched::SearchConfig> Instances() { return Udm::InstantiatedAttr< ::TTSched::SearchConfig>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::TTSched::SearchConfig, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::TTSched::SearchConfig, Pred>(impl);}

			Udm::DerivedAttr< ::TTSched::SearchConfig> Derived() { return Udm::DerivedAttr< ::TTSched::SearchConfig>(impl);}
			template <class Pred> Udm::DerivedAttr< ::TTSched::SearchConfig, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::TTSched::SearchConfig, Pred>(impl);}

			Udm::ArchetypeAttr< ::TTSched::SearchConfig> Archetype() { return Udm::ArchetypeAttr< ::TTSched::SearchConfig>(impl);}

			static ::Uml::Attribute meta_MaxRunTimeSec;
			Udm::IntegerAttr MaxRunTimeSec() const { return Udm::IntegerAttr(impl, meta_MaxRunTimeSec); }

			static ::Uml::Attribute meta_MaxFailures;
			Udm::IntegerAttr MaxFailures() const { return Udm::IntegerAttr(impl, meta_MaxFailures); }

			static ::Uml::Attribute meta_SearchEngine;
			Udm::StringAttr SearchEngine() const { return Udm::StringAttr(impl, meta_SearchEngine); }

			static ::Uml::Attribute meta_MaxSolutions;
			Udm::IntegerAttr MaxSolutions() const { return Udm::IntegerAttr(impl, meta_MaxSolutions); }

			static ::Uml::CompositionParentRole meta_ConstraintProblem_parent;
			Udm::ParentAttr< ::TTSched::ConstraintProblem> ConstraintProblem_parent() const { return Udm::ParentAttr< ::TTSched::ConstraintProblem>(impl, meta_ConstraintProblem_parent); }

			Udm::ParentAttr< ::TTSched::MgaObject> parent() const { return Udm::ParentAttr< ::TTSched::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  EqualityConstraint :  public ::TTSched::PairConstraint {
		public:
			static ::Uml::Class meta;

			EqualityConstraint() { }
			EqualityConstraint(Udm::ObjectImpl *impl) : ::TTSched::PairConstraint(impl), ::TTSched::MgaObject(impl) { }
			EqualityConstraint(const EqualityConstraint &master) : ::TTSched::PairConstraint(master), ::TTSched::MgaObject(master) { }
			static EqualityConstraint Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static EqualityConstraint Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			EqualityConstraint CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			EqualityConstraint CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::TTSched::EqualityConstraint> Instances() { return Udm::InstantiatedAttr< ::TTSched::EqualityConstraint>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::TTSched::EqualityConstraint, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::TTSched::EqualityConstraint, Pred>(impl);}

			Udm::DerivedAttr< ::TTSched::EqualityConstraint> Derived() { return Udm::DerivedAttr< ::TTSched::EqualityConstraint>(impl);}
			template <class Pred> Udm::DerivedAttr< ::TTSched::EqualityConstraint, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::TTSched::EqualityConstraint, Pred>(impl);}

			Udm::ArchetypeAttr< ::TTSched::EqualityConstraint> Archetype() { return Udm::ArchetypeAttr< ::TTSched::EqualityConstraint>(impl);}

			static ::Uml::Attribute meta_Offset;
			Udm::IntegerAttr Offset() const { return Udm::IntegerAttr(impl, meta_Offset); }

			Udm::ParentAttr< ::TTSched::MgaObject> parent() const { return Udm::ParentAttr< ::TTSched::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  TimeSync :  public ::TTSched::MgaObject {
		public:
			static ::Uml::Class meta;

			TimeSync() { }
			TimeSync(Udm::ObjectImpl *impl) : ::TTSched::MgaObject(impl) { }
			TimeSync(const TimeSync &master) : ::TTSched::MgaObject(master) { }
			static TimeSync Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static TimeSync Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			TimeSync CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			TimeSync CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::TTSched::TimeSync> Instances() { return Udm::InstantiatedAttr< ::TTSched::TimeSync>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::TTSched::TimeSync, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::TTSched::TimeSync, Pred>(impl);}

			Udm::DerivedAttr< ::TTSched::TimeSync> Derived() { return Udm::DerivedAttr< ::TTSched::TimeSync>(impl);}
			template <class Pred> Udm::DerivedAttr< ::TTSched::TimeSync, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::TTSched::TimeSync, Pred>(impl);}

			Udm::ArchetypeAttr< ::TTSched::TimeSync> Archetype() { return Udm::ArchetypeAttr< ::TTSched::TimeSync>(impl);}

			static ::Uml::Attribute meta_MsgLength;
			Udm::IntegerAttr MsgLength() const { return Udm::IntegerAttr(impl, meta_MsgLength); }

			static ::Uml::Attribute meta_Period;
			Udm::IntegerAttr Period() const { return Udm::IntegerAttr(impl, meta_Period); }

			static ::Uml::CompositionParentRole meta_Bus_parent;
			Udm::ParentAttr< ::TTSched::Bus> Bus_parent() const { return Udm::ParentAttr< ::TTSched::Bus>(impl, meta_Bus_parent); }

			Udm::ParentAttr< ::TTSched::MgaObject> parent() const { return Udm::ParentAttr< ::TTSched::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  Dependency :  public ::TTSched::MgaObject {
		public:
			static ::Uml::Class meta;

			Dependency() { }
			Dependency(Udm::ObjectImpl *impl) : ::TTSched::MgaObject(impl) { }
			Dependency(const Dependency &master) : ::TTSched::MgaObject(master) { }
			static Dependency Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static Dependency Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			Dependency CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			Dependency CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::TTSched::Dependency> Instances() { return Udm::InstantiatedAttr< ::TTSched::Dependency>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::TTSched::Dependency, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::TTSched::Dependency, Pred>(impl);}

			Udm::DerivedAttr< ::TTSched::Dependency> Derived() { return Udm::DerivedAttr< ::TTSched::Dependency>(impl);}
			template <class Pred> Udm::DerivedAttr< ::TTSched::Dependency, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::TTSched::Dependency, Pred>(impl);}

			Udm::ArchetypeAttr< ::TTSched::Dependency> Archetype() { return Udm::ArchetypeAttr< ::TTSched::Dependency>(impl);}

			static ::Uml::CompositionParentRole meta_LatencySpec_parent;
			Udm::ParentAttr< ::TTSched::LatencySpec> LatencySpec_parent() const { return Udm::ParentAttr< ::TTSched::LatencySpec>(impl, meta_LatencySpec_parent); }

			Udm::ParentAttr< ::TTSched::MgaObject> parent() const { return Udm::ParentAttr< ::TTSched::MgaObject>(impl, Udm::NULLPARENTROLE); }
			static ::Uml::AssociationRole meta_srcDependency_end_;
			Udm::AssocEndAttr< ::TTSched::MsgRef> srcDependency_end() const { return Udm::AssocEndAttr< ::TTSched::MsgRef>(impl, meta_srcDependency_end_); }

			static ::Uml::AssociationRole meta_dstDependency_end_;
			Udm::AssocEndAttr< ::TTSched::MsgRef> dstDependency_end() const { return Udm::AssocEndAttr< ::TTSched::MsgRef>(impl, meta_dstDependency_end_); }

		};

		class  Task :  public ::TTSched::MgaObject {
		public:
			static ::Uml::Class meta;

			Task() { }
			Task(Udm::ObjectImpl *impl) : ::TTSched::MgaObject(impl) { }
			Task(const Task &master) : ::TTSched::MgaObject(master) { }
			static Task Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static Task Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			Task CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			Task CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::TTSched::Task> Instances() { return Udm::InstantiatedAttr< ::TTSched::Task>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::TTSched::Task, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::TTSched::Task, Pred>(impl);}

			Udm::DerivedAttr< ::TTSched::Task> Derived() { return Udm::DerivedAttr< ::TTSched::Task>(impl);}
			template <class Pred> Udm::DerivedAttr< ::TTSched::Task, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::TTSched::Task, Pred>(impl);}

			Udm::ArchetypeAttr< ::TTSched::Task> Archetype() { return Udm::ArchetypeAttr< ::TTSched::Task>(impl);}

			static ::Uml::Attribute meta_WCExecTime;
			Udm::IntegerAttr WCExecTime() const { return Udm::IntegerAttr(impl, meta_WCExecTime); }

			static ::Uml::Attribute meta_Period;
			Udm::IntegerAttr Period() const { return Udm::IntegerAttr(impl, meta_Period); }

			static ::Uml::AssociationRole meta_referedbyTaskRef;
			Udm::AssocAttr< ::TTSched::TaskRef> referedbyTaskRef() const { return Udm::AssocAttr< ::TTSched::TaskRef>(impl, meta_referedbyTaskRef); }
			template <class Pred> Udm::AssocAttr< ::TTSched::TaskRef, Pred > referedbyTaskRef_sorted(const Pred &) const { return Udm::AssocAttr< ::TTSched::TaskRef, Pred>(impl, meta_referedbyTaskRef); }

			static ::Uml::CompositionParentRole meta_Processor_parent;
			Udm::ParentAttr< ::TTSched::Processor> Processor_parent() const { return Udm::ParentAttr< ::TTSched::Processor>(impl, meta_Processor_parent); }

			Udm::ParentAttr< ::TTSched::MgaObject> parent() const { return Udm::ParentAttr< ::TTSched::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  Message :  public ::TTSched::MgaObject {
		public:
			static ::Uml::Class meta;

			Message() { }
			Message(Udm::ObjectImpl *impl) : ::TTSched::MgaObject(impl) { }
			Message(const Message &master) : ::TTSched::MgaObject(master) { }
			static Message Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static Message Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			Message CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			Message CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::TTSched::Message> Instances() { return Udm::InstantiatedAttr< ::TTSched::Message>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::TTSched::Message, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::TTSched::Message, Pred>(impl);}

			Udm::DerivedAttr< ::TTSched::Message> Derived() { return Udm::DerivedAttr< ::TTSched::Message>(impl);}
			template <class Pred> Udm::DerivedAttr< ::TTSched::Message, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::TTSched::Message, Pred>(impl);}

			Udm::ArchetypeAttr< ::TTSched::Message> Archetype() { return Udm::ArchetypeAttr< ::TTSched::Message>(impl);}

			static ::Uml::Attribute meta_MsgLength;
			Udm::IntegerAttr MsgLength() const { return Udm::IntegerAttr(impl, meta_MsgLength); }

			static ::Uml::AssociationRole meta_srcSender;
			static ::Uml::AssociationRole meta_srcSender_rev;
			Udm::AClassPointerAttr< ::TTSched::Sender, ::TTSched::TaskRef> srcSender() const { return Udm::AClassPointerAttr< ::TTSched::Sender, ::TTSched::TaskRef>(impl, meta_srcSender, meta_srcSender_rev); }

			static ::Uml::AssociationRole meta_dstReceiver;
			static ::Uml::AssociationRole meta_dstReceiver_rev;
			Udm::AClassAssocAttr< ::TTSched::Receiver, ::TTSched::TaskRef> dstReceiver() const { return Udm::AClassAssocAttr< ::TTSched::Receiver, ::TTSched::TaskRef>(impl, meta_dstReceiver, meta_dstReceiver_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::TTSched::Receiver, ::TTSched::TaskRef, Pred> dstReceiver_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::TTSched::Receiver, ::TTSched::TaskRef, Pred>(impl, meta_dstReceiver, meta_dstReceiver_rev); }

			static ::Uml::AssociationRole meta_referedbyMsgRef;
			Udm::AssocAttr< ::TTSched::MsgRef> referedbyMsgRef() const { return Udm::AssocAttr< ::TTSched::MsgRef>(impl, meta_referedbyMsgRef); }
			template <class Pred> Udm::AssocAttr< ::TTSched::MsgRef, Pred > referedbyMsgRef_sorted(const Pred &) const { return Udm::AssocAttr< ::TTSched::MsgRef, Pred>(impl, meta_referedbyMsgRef); }

			static ::Uml::CompositionParentRole meta_Bus_parent;
			Udm::ParentAttr< ::TTSched::Bus> Bus_parent() const { return Udm::ParentAttr< ::TTSched::Bus>(impl, meta_Bus_parent); }

			Udm::ParentAttr< ::TTSched::MgaObject> parent() const { return Udm::ParentAttr< ::TTSched::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  TTSystem :  public ::TTSched::MgaObject {
		public:
			static ::Uml::Class meta;

			TTSystem() { }
			TTSystem(Udm::ObjectImpl *impl) : ::TTSched::MgaObject(impl) { }
			TTSystem(const TTSystem &master) : ::TTSched::MgaObject(master) { }
			static TTSystem Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static TTSystem Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			TTSystem CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			TTSystem CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::TTSched::TTSystem> Instances() { return Udm::InstantiatedAttr< ::TTSched::TTSystem>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::TTSched::TTSystem, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::TTSched::TTSystem, Pred>(impl);}

			Udm::DerivedAttr< ::TTSched::TTSystem> Derived() { return Udm::DerivedAttr< ::TTSched::TTSystem>(impl);}
			template <class Pred> Udm::DerivedAttr< ::TTSched::TTSystem, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::TTSched::TTSystem, Pred>(impl);}

			Udm::ArchetypeAttr< ::TTSched::TTSystem> Archetype() { return Udm::ArchetypeAttr< ::TTSched::TTSystem>(impl);}

			static ::Uml::CompositionChildRole meta_LatencySpec_children;
			Udm::ChildrenAttr< ::TTSched::LatencySpec> LatencySpec_children() const { return Udm::ChildrenAttr< ::TTSched::LatencySpec>(impl, meta_LatencySpec_children); }
			template <class Pred> Udm::ChildrenAttr< ::TTSched::LatencySpec, Pred> LatencySpec_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::TTSched::LatencySpec, Pred>(impl, meta_LatencySpec_children); }

			static ::Uml::CompositionChildRole meta_Link_children;
			Udm::ChildrenAttr< ::TTSched::Link> Link_children() const { return Udm::ChildrenAttr< ::TTSched::Link>(impl, meta_Link_children); }
			template <class Pred> Udm::ChildrenAttr< ::TTSched::Link, Pred> Link_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::TTSched::Link, Pred>(impl, meta_Link_children); }

			static ::Uml::CompositionChildRole meta_Processor_children;
			Udm::ChildrenAttr< ::TTSched::Processor> Processor_children() const { return Udm::ChildrenAttr< ::TTSched::Processor>(impl, meta_Processor_children); }
			template <class Pred> Udm::ChildrenAttr< ::TTSched::Processor, Pred> Processor_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::TTSched::Processor, Pred>(impl, meta_Processor_children); }

			static ::Uml::CompositionChildRole meta_Bus_children;
			Udm::ChildrenAttr< ::TTSched::Bus> Bus_children() const { return Udm::ChildrenAttr< ::TTSched::Bus>(impl, meta_Bus_children); }
			template <class Pred> Udm::ChildrenAttr< ::TTSched::Bus, Pred> Bus_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::TTSched::Bus, Pred>(impl, meta_Bus_children); }

			Udm::ChildrenAttr< ::TTSched::LatencySpec> LatencySpec_kind_children() const { return Udm::ChildrenAttr< ::TTSched::LatencySpec>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::TTSched::LatencySpec, Pred> LatencySpec_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::TTSched::LatencySpec, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::TTSched::Processor> Processor_kind_children() const { return Udm::ChildrenAttr< ::TTSched::Processor>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::TTSched::Processor, Pred> Processor_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::TTSched::Processor, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::TTSched::Link> Link_kind_children() const { return Udm::ChildrenAttr< ::TTSched::Link>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::TTSched::Link, Pred> Link_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::TTSched::Link, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::TTSched::Bus> Bus_kind_children() const { return Udm::ChildrenAttr< ::TTSched::Bus>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::TTSched::Bus, Pred> Bus_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::TTSched::Bus, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::TTSched::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr< ::TTSched::MgaObject>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::TTSched::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::TTSched::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

			static ::Uml::CompositionParentRole meta_RootFolder_parent;
			Udm::ParentAttr< ::TTSched::RootFolder> RootFolder_parent() const { return Udm::ParentAttr< ::TTSched::RootFolder>(impl, meta_RootFolder_parent); }

			Udm::ParentAttr< ::TTSched::RootFolder> parent() const { return Udm::ParentAttr< ::TTSched::RootFolder>(impl, Udm::NULLPARENTROLE); }
		};

		class  Latency :  public ::TTSched::MgaObject {
		public:
			static ::Uml::Class meta;

			Latency() { }
			Latency(Udm::ObjectImpl *impl) : ::TTSched::MgaObject(impl) { }
			Latency(const Latency &master) : ::TTSched::MgaObject(master) { }
			static Latency Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static Latency Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			Latency CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			Latency CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::TTSched::Latency> Instances() { return Udm::InstantiatedAttr< ::TTSched::Latency>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::TTSched::Latency, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::TTSched::Latency, Pred>(impl);}

			Udm::DerivedAttr< ::TTSched::Latency> Derived() { return Udm::DerivedAttr< ::TTSched::Latency>(impl);}
			template <class Pred> Udm::DerivedAttr< ::TTSched::Latency, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::TTSched::Latency, Pred>(impl);}

			Udm::ArchetypeAttr< ::TTSched::Latency> Archetype() { return Udm::ArchetypeAttr< ::TTSched::Latency>(impl);}

			static ::Uml::Attribute meta_MaxLength;
			Udm::IntegerAttr MaxLength() const { return Udm::IntegerAttr(impl, meta_MaxLength); }

			static ::Uml::AssociationRole meta_srcEndMsg;
			static ::Uml::AssociationRole meta_srcEndMsg_rev;
			Udm::AClassAssocAttr< ::TTSched::EndMsg, ::TTSched::MsgRef> srcEndMsg() const { return Udm::AClassAssocAttr< ::TTSched::EndMsg, ::TTSched::MsgRef>(impl, meta_srcEndMsg, meta_srcEndMsg_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::TTSched::EndMsg, ::TTSched::MsgRef, Pred> srcEndMsg_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::TTSched::EndMsg, ::TTSched::MsgRef, Pred>(impl, meta_srcEndMsg, meta_srcEndMsg_rev); }

			static ::Uml::AssociationRole meta_dstStartMsg;
			static ::Uml::AssociationRole meta_dstStartMsg_rev;
			Udm::AClassAssocAttr< ::TTSched::StartMsg, ::TTSched::MsgRef> dstStartMsg() const { return Udm::AClassAssocAttr< ::TTSched::StartMsg, ::TTSched::MsgRef>(impl, meta_dstStartMsg, meta_dstStartMsg_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::TTSched::StartMsg, ::TTSched::MsgRef, Pred> dstStartMsg_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::TTSched::StartMsg, ::TTSched::MsgRef, Pred>(impl, meta_dstStartMsg, meta_dstStartMsg_rev); }

			static ::Uml::CompositionParentRole meta_LatencySpec_parent;
			Udm::ParentAttr< ::TTSched::LatencySpec> LatencySpec_parent() const { return Udm::ParentAttr< ::TTSched::LatencySpec>(impl, meta_LatencySpec_parent); }

			Udm::ParentAttr< ::TTSched::MgaObject> parent() const { return Udm::ParentAttr< ::TTSched::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  Sender :  public ::TTSched::MgaObject {
		public:
			static ::Uml::Class meta;

			Sender() { }
			Sender(Udm::ObjectImpl *impl) : ::TTSched::MgaObject(impl) { }
			Sender(const Sender &master) : ::TTSched::MgaObject(master) { }
			static Sender Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static Sender Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			Sender CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			Sender CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::TTSched::Sender> Instances() { return Udm::InstantiatedAttr< ::TTSched::Sender>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::TTSched::Sender, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::TTSched::Sender, Pred>(impl);}

			Udm::DerivedAttr< ::TTSched::Sender> Derived() { return Udm::DerivedAttr< ::TTSched::Sender>(impl);}
			template <class Pred> Udm::DerivedAttr< ::TTSched::Sender, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::TTSched::Sender, Pred>(impl);}

			Udm::ArchetypeAttr< ::TTSched::Sender> Archetype() { return Udm::ArchetypeAttr< ::TTSched::Sender>(impl);}

			static ::Uml::CompositionParentRole meta_Bus_parent;
			Udm::ParentAttr< ::TTSched::Bus> Bus_parent() const { return Udm::ParentAttr< ::TTSched::Bus>(impl, meta_Bus_parent); }

			Udm::ParentAttr< ::TTSched::MgaObject> parent() const { return Udm::ParentAttr< ::TTSched::MgaObject>(impl, Udm::NULLPARENTROLE); }
			static ::Uml::AssociationRole meta_dstSender_end_;
			Udm::AssocEndAttr< ::TTSched::Message> dstSender_end() const { return Udm::AssocEndAttr< ::TTSched::Message>(impl, meta_dstSender_end_); }

			static ::Uml::AssociationRole meta_srcSender_end_;
			Udm::AssocEndAttr< ::TTSched::TaskRef> srcSender_end() const { return Udm::AssocEndAttr< ::TTSched::TaskRef>(impl, meta_srcSender_end_); }

		};

		class  LatencySpec :  public ::TTSched::MgaObject {
		public:
			static ::Uml::Class meta;

			LatencySpec() { }
			LatencySpec(Udm::ObjectImpl *impl) : ::TTSched::MgaObject(impl) { }
			LatencySpec(const LatencySpec &master) : ::TTSched::MgaObject(master) { }
			static LatencySpec Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static LatencySpec Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			LatencySpec CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			LatencySpec CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::TTSched::LatencySpec> Instances() { return Udm::InstantiatedAttr< ::TTSched::LatencySpec>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::TTSched::LatencySpec, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::TTSched::LatencySpec, Pred>(impl);}

			Udm::DerivedAttr< ::TTSched::LatencySpec> Derived() { return Udm::DerivedAttr< ::TTSched::LatencySpec>(impl);}
			template <class Pred> Udm::DerivedAttr< ::TTSched::LatencySpec, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::TTSched::LatencySpec, Pred>(impl);}

			Udm::ArchetypeAttr< ::TTSched::LatencySpec> Archetype() { return Udm::ArchetypeAttr< ::TTSched::LatencySpec>(impl);}

			static ::Uml::CompositionChildRole meta_Dependency_children;
			Udm::ChildrenAttr< ::TTSched::Dependency> Dependency_children() const { return Udm::ChildrenAttr< ::TTSched::Dependency>(impl, meta_Dependency_children); }
			template <class Pred> Udm::ChildrenAttr< ::TTSched::Dependency, Pred> Dependency_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::TTSched::Dependency, Pred>(impl, meta_Dependency_children); }

			static ::Uml::CompositionChildRole meta_Latency_children;
			Udm::ChildrenAttr< ::TTSched::Latency> Latency_children() const { return Udm::ChildrenAttr< ::TTSched::Latency>(impl, meta_Latency_children); }
			template <class Pred> Udm::ChildrenAttr< ::TTSched::Latency, Pred> Latency_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::TTSched::Latency, Pred>(impl, meta_Latency_children); }

			static ::Uml::CompositionChildRole meta_MsgRef_children;
			Udm::ChildrenAttr< ::TTSched::MsgRef> MsgRef_children() const { return Udm::ChildrenAttr< ::TTSched::MsgRef>(impl, meta_MsgRef_children); }
			template <class Pred> Udm::ChildrenAttr< ::TTSched::MsgRef, Pred> MsgRef_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::TTSched::MsgRef, Pred>(impl, meta_MsgRef_children); }

			static ::Uml::CompositionChildRole meta_StartMsg_children;
			Udm::ChildrenAttr< ::TTSched::StartMsg> StartMsg_children() const { return Udm::ChildrenAttr< ::TTSched::StartMsg>(impl, meta_StartMsg_children); }
			template <class Pred> Udm::ChildrenAttr< ::TTSched::StartMsg, Pred> StartMsg_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::TTSched::StartMsg, Pred>(impl, meta_StartMsg_children); }

			static ::Uml::CompositionChildRole meta_EndMsg_children;
			Udm::ChildrenAttr< ::TTSched::EndMsg> EndMsg_children() const { return Udm::ChildrenAttr< ::TTSched::EndMsg>(impl, meta_EndMsg_children); }
			template <class Pred> Udm::ChildrenAttr< ::TTSched::EndMsg, Pred> EndMsg_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::TTSched::EndMsg, Pred>(impl, meta_EndMsg_children); }

			Udm::ChildrenAttr< ::TTSched::Dependency> Dependency_kind_children() const { return Udm::ChildrenAttr< ::TTSched::Dependency>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::TTSched::Dependency, Pred> Dependency_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::TTSched::Dependency, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::TTSched::Latency> Latency_kind_children() const { return Udm::ChildrenAttr< ::TTSched::Latency>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::TTSched::Latency, Pred> Latency_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::TTSched::Latency, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::TTSched::EndMsg> EndMsg_kind_children() const { return Udm::ChildrenAttr< ::TTSched::EndMsg>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::TTSched::EndMsg, Pred> EndMsg_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::TTSched::EndMsg, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::TTSched::StartMsg> StartMsg_kind_children() const { return Udm::ChildrenAttr< ::TTSched::StartMsg>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::TTSched::StartMsg, Pred> StartMsg_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::TTSched::StartMsg, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::TTSched::MsgRef> MsgRef_kind_children() const { return Udm::ChildrenAttr< ::TTSched::MsgRef>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::TTSched::MsgRef, Pred> MsgRef_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::TTSched::MsgRef, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::TTSched::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr< ::TTSched::MgaObject>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::TTSched::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::TTSched::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

			static ::Uml::CompositionParentRole meta_TTSystem_parent;
			Udm::ParentAttr< ::TTSched::TTSystem> TTSystem_parent() const { return Udm::ParentAttr< ::TTSched::TTSystem>(impl, meta_TTSystem_parent); }

			Udm::ParentAttr< ::TTSched::MgaObject> parent() const { return Udm::ParentAttr< ::TTSched::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  EndMsg :  public ::TTSched::MgaObject {
		public:
			static ::Uml::Class meta;

			EndMsg() { }
			EndMsg(Udm::ObjectImpl *impl) : ::TTSched::MgaObject(impl) { }
			EndMsg(const EndMsg &master) : ::TTSched::MgaObject(master) { }
			static EndMsg Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static EndMsg Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			EndMsg CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			EndMsg CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::TTSched::EndMsg> Instances() { return Udm::InstantiatedAttr< ::TTSched::EndMsg>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::TTSched::EndMsg, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::TTSched::EndMsg, Pred>(impl);}

			Udm::DerivedAttr< ::TTSched::EndMsg> Derived() { return Udm::DerivedAttr< ::TTSched::EndMsg>(impl);}
			template <class Pred> Udm::DerivedAttr< ::TTSched::EndMsg, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::TTSched::EndMsg, Pred>(impl);}

			Udm::ArchetypeAttr< ::TTSched::EndMsg> Archetype() { return Udm::ArchetypeAttr< ::TTSched::EndMsg>(impl);}

			static ::Uml::CompositionParentRole meta_LatencySpec_parent;
			Udm::ParentAttr< ::TTSched::LatencySpec> LatencySpec_parent() const { return Udm::ParentAttr< ::TTSched::LatencySpec>(impl, meta_LatencySpec_parent); }

			Udm::ParentAttr< ::TTSched::MgaObject> parent() const { return Udm::ParentAttr< ::TTSched::MgaObject>(impl, Udm::NULLPARENTROLE); }
			static ::Uml::AssociationRole meta_dstEndMsg_end_;
			Udm::AssocEndAttr< ::TTSched::Latency> dstEndMsg_end() const { return Udm::AssocEndAttr< ::TTSched::Latency>(impl, meta_dstEndMsg_end_); }

			static ::Uml::AssociationRole meta_srcEndMsg_end_;
			Udm::AssocEndAttr< ::TTSched::MsgRef> srcEndMsg_end() const { return Udm::AssocEndAttr< ::TTSched::MsgRef>(impl, meta_srcEndMsg_end_); }

		};

		class  StartMsg :  public ::TTSched::MgaObject {
		public:
			static ::Uml::Class meta;

			StartMsg() { }
			StartMsg(Udm::ObjectImpl *impl) : ::TTSched::MgaObject(impl) { }
			StartMsg(const StartMsg &master) : ::TTSched::MgaObject(master) { }
			static StartMsg Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static StartMsg Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			StartMsg CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			StartMsg CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::TTSched::StartMsg> Instances() { return Udm::InstantiatedAttr< ::TTSched::StartMsg>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::TTSched::StartMsg, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::TTSched::StartMsg, Pred>(impl);}

			Udm::DerivedAttr< ::TTSched::StartMsg> Derived() { return Udm::DerivedAttr< ::TTSched::StartMsg>(impl);}
			template <class Pred> Udm::DerivedAttr< ::TTSched::StartMsg, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::TTSched::StartMsg, Pred>(impl);}

			Udm::ArchetypeAttr< ::TTSched::StartMsg> Archetype() { return Udm::ArchetypeAttr< ::TTSched::StartMsg>(impl);}

			static ::Uml::CompositionParentRole meta_LatencySpec_parent;
			Udm::ParentAttr< ::TTSched::LatencySpec> LatencySpec_parent() const { return Udm::ParentAttr< ::TTSched::LatencySpec>(impl, meta_LatencySpec_parent); }

			Udm::ParentAttr< ::TTSched::MgaObject> parent() const { return Udm::ParentAttr< ::TTSched::MgaObject>(impl, Udm::NULLPARENTROLE); }
			static ::Uml::AssociationRole meta_srcStartMsg_end_;
			Udm::AssocEndAttr< ::TTSched::Latency> srcStartMsg_end() const { return Udm::AssocEndAttr< ::TTSched::Latency>(impl, meta_srcStartMsg_end_); }

			static ::Uml::AssociationRole meta_dstStartMsg_end_;
			Udm::AssocEndAttr< ::TTSched::MsgRef> dstStartMsg_end() const { return Udm::AssocEndAttr< ::TTSched::MsgRef>(impl, meta_dstStartMsg_end_); }

		};

		class  Receiver :  public ::TTSched::MgaObject {
		public:
			static ::Uml::Class meta;

			Receiver() { }
			Receiver(Udm::ObjectImpl *impl) : ::TTSched::MgaObject(impl) { }
			Receiver(const Receiver &master) : ::TTSched::MgaObject(master) { }
			static Receiver Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static Receiver Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			Receiver CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			Receiver CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::TTSched::Receiver> Instances() { return Udm::InstantiatedAttr< ::TTSched::Receiver>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::TTSched::Receiver, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::TTSched::Receiver, Pred>(impl);}

			Udm::DerivedAttr< ::TTSched::Receiver> Derived() { return Udm::DerivedAttr< ::TTSched::Receiver>(impl);}
			template <class Pred> Udm::DerivedAttr< ::TTSched::Receiver, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::TTSched::Receiver, Pred>(impl);}

			Udm::ArchetypeAttr< ::TTSched::Receiver> Archetype() { return Udm::ArchetypeAttr< ::TTSched::Receiver>(impl);}

			static ::Uml::CompositionParentRole meta_Bus_parent;
			Udm::ParentAttr< ::TTSched::Bus> Bus_parent() const { return Udm::ParentAttr< ::TTSched::Bus>(impl, meta_Bus_parent); }

			Udm::ParentAttr< ::TTSched::MgaObject> parent() const { return Udm::ParentAttr< ::TTSched::MgaObject>(impl, Udm::NULLPARENTROLE); }
			static ::Uml::AssociationRole meta_srcReceiver_end_;
			Udm::AssocEndAttr< ::TTSched::Message> srcReceiver_end() const { return Udm::AssocEndAttr< ::TTSched::Message>(impl, meta_srcReceiver_end_); }

			static ::Uml::AssociationRole meta_dstReceiver_end_;
			Udm::AssocEndAttr< ::TTSched::TaskRef> dstReceiver_end() const { return Udm::AssocEndAttr< ::TTSched::TaskRef>(impl, meta_dstReceiver_end_); }

		};

		class  TaskRef :  public ::TTSched::MgaObject {
		public:
			static ::Uml::Class meta;

			TaskRef() { }
			TaskRef(Udm::ObjectImpl *impl) : ::TTSched::MgaObject(impl) { }
			TaskRef(const TaskRef &master) : ::TTSched::MgaObject(master) { }
			static TaskRef Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static TaskRef Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			TaskRef CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			TaskRef CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::TTSched::TaskRef> Instances() { return Udm::InstantiatedAttr< ::TTSched::TaskRef>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::TTSched::TaskRef, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::TTSched::TaskRef, Pred>(impl);}

			Udm::DerivedAttr< ::TTSched::TaskRef> Derived() { return Udm::DerivedAttr< ::TTSched::TaskRef>(impl);}
			template <class Pred> Udm::DerivedAttr< ::TTSched::TaskRef, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::TTSched::TaskRef, Pred>(impl);}

			Udm::ArchetypeAttr< ::TTSched::TaskRef> Archetype() { return Udm::ArchetypeAttr< ::TTSched::TaskRef>(impl);}

			static ::Uml::AssociationRole meta_ref;
			Udm::PointerAttr< ::TTSched::Task> ref() const { return Udm::PointerAttr< ::TTSched::Task>(impl, meta_ref); }

			static ::Uml::AssociationRole meta_dstSender;
			static ::Uml::AssociationRole meta_dstSender_rev;
			Udm::AClassAssocAttr< ::TTSched::Sender, ::TTSched::Message> dstSender() const { return Udm::AClassAssocAttr< ::TTSched::Sender, ::TTSched::Message>(impl, meta_dstSender, meta_dstSender_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::TTSched::Sender, ::TTSched::Message, Pred> dstSender_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::TTSched::Sender, ::TTSched::Message, Pred>(impl, meta_dstSender, meta_dstSender_rev); }

			static ::Uml::AssociationRole meta_srcReceiver;
			static ::Uml::AssociationRole meta_srcReceiver_rev;
			Udm::AClassAssocAttr< ::TTSched::Receiver, ::TTSched::Message> srcReceiver() const { return Udm::AClassAssocAttr< ::TTSched::Receiver, ::TTSched::Message>(impl, meta_srcReceiver, meta_srcReceiver_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::TTSched::Receiver, ::TTSched::Message, Pred> srcReceiver_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::TTSched::Receiver, ::TTSched::Message, Pred>(impl, meta_srcReceiver, meta_srcReceiver_rev); }

			static ::Uml::CompositionParentRole meta_TaskInstances_parent;
			Udm::ParentAttr< ::TTSched::TaskInstances> TaskInstances_parent() const { return Udm::ParentAttr< ::TTSched::TaskInstances>(impl, meta_TaskInstances_parent); }

			static ::Uml::CompositionParentRole meta_Bus_parent;
			Udm::ParentAttr< ::TTSched::Bus> Bus_parent() const { return Udm::ParentAttr< ::TTSched::Bus>(impl, meta_Bus_parent); }

			Udm::ParentAttr< ::TTSched::MgaObject> parent() const { return Udm::ParentAttr< ::TTSched::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  Processor :  public ::TTSched::MgaObject {
		public:
			static ::Uml::Class meta;

			Processor() { }
			Processor(Udm::ObjectImpl *impl) : ::TTSched::MgaObject(impl) { }
			Processor(const Processor &master) : ::TTSched::MgaObject(master) { }
			static Processor Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static Processor Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			Processor CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			Processor CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::TTSched::Processor> Instances() { return Udm::InstantiatedAttr< ::TTSched::Processor>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::TTSched::Processor, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::TTSched::Processor, Pred>(impl);}

			Udm::DerivedAttr< ::TTSched::Processor> Derived() { return Udm::DerivedAttr< ::TTSched::Processor>(impl);}
			template <class Pred> Udm::DerivedAttr< ::TTSched::Processor, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::TTSched::Processor, Pred>(impl);}

			Udm::ArchetypeAttr< ::TTSched::Processor> Archetype() { return Udm::ArchetypeAttr< ::TTSched::Processor>(impl);}

			static ::Uml::AssociationRole meta_dstLink;
			static ::Uml::AssociationRole meta_dstLink_rev;
			Udm::AClassAssocAttr< ::TTSched::Link, ::TTSched::Bus> dstLink() const { return Udm::AClassAssocAttr< ::TTSched::Link, ::TTSched::Bus>(impl, meta_dstLink, meta_dstLink_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::TTSched::Link, ::TTSched::Bus, Pred> dstLink_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::TTSched::Link, ::TTSched::Bus, Pred>(impl, meta_dstLink, meta_dstLink_rev); }

			static ::Uml::CompositionChildRole meta_TaskInstances_child;
			Udm::ChildAttr< ::TTSched::TaskInstances> TaskInstances_child() const { return Udm::ChildAttr< ::TTSched::TaskInstances>(impl, meta_TaskInstances_child); }

			static ::Uml::CompositionChildRole meta_Task_children;
			Udm::ChildrenAttr< ::TTSched::Task> Task_children() const { return Udm::ChildrenAttr< ::TTSched::Task>(impl, meta_Task_children); }
			template <class Pred> Udm::ChildrenAttr< ::TTSched::Task, Pred> Task_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::TTSched::Task, Pred>(impl, meta_Task_children); }

			Udm::ChildrenAttr< ::TTSched::Task> Task_kind_children() const { return Udm::ChildrenAttr< ::TTSched::Task>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::TTSched::Task, Pred> Task_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::TTSched::Task, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::TTSched::TaskInstances> TaskInstances_kind_children() const { return Udm::ChildrenAttr< ::TTSched::TaskInstances>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::TTSched::TaskInstances, Pred> TaskInstances_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::TTSched::TaskInstances, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::TTSched::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr< ::TTSched::MgaObject>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::TTSched::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::TTSched::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

			static ::Uml::CompositionParentRole meta_TTSystem_parent;
			Udm::ParentAttr< ::TTSched::TTSystem> TTSystem_parent() const { return Udm::ParentAttr< ::TTSched::TTSystem>(impl, meta_TTSystem_parent); }

			static ::Uml::CompositionParentRole meta_InstanceModel_parent;
			Udm::ParentAttr< ::TTSched::InstanceModel> InstanceModel_parent() const { return Udm::ParentAttr< ::TTSched::InstanceModel>(impl, meta_InstanceModel_parent); }

			Udm::ParentAttr< ::TTSched::MgaObject> parent() const { return Udm::ParentAttr< ::TTSched::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  Link :  public ::TTSched::MgaObject {
		public:
			static ::Uml::Class meta;

			Link() { }
			Link(Udm::ObjectImpl *impl) : ::TTSched::MgaObject(impl) { }
			Link(const Link &master) : ::TTSched::MgaObject(master) { }
			static Link Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static Link Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			Link CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			Link CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::TTSched::Link> Instances() { return Udm::InstantiatedAttr< ::TTSched::Link>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::TTSched::Link, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::TTSched::Link, Pred>(impl);}

			Udm::DerivedAttr< ::TTSched::Link> Derived() { return Udm::DerivedAttr< ::TTSched::Link>(impl);}
			template <class Pred> Udm::DerivedAttr< ::TTSched::Link, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::TTSched::Link, Pred>(impl);}

			Udm::ArchetypeAttr< ::TTSched::Link> Archetype() { return Udm::ArchetypeAttr< ::TTSched::Link>(impl);}

			static ::Uml::CompositionParentRole meta_TTSystem_parent;
			Udm::ParentAttr< ::TTSched::TTSystem> TTSystem_parent() const { return Udm::ParentAttr< ::TTSched::TTSystem>(impl, meta_TTSystem_parent); }

			Udm::ParentAttr< ::TTSched::MgaObject> parent() const { return Udm::ParentAttr< ::TTSched::MgaObject>(impl, Udm::NULLPARENTROLE); }
			static ::Uml::AssociationRole meta_dstLink_end_;
			Udm::AssocEndAttr< ::TTSched::Bus> dstLink_end() const { return Udm::AssocEndAttr< ::TTSched::Bus>(impl, meta_dstLink_end_); }

			static ::Uml::AssociationRole meta_srcLink_end_;
			Udm::AssocEndAttr< ::TTSched::Processor> srcLink_end() const { return Udm::AssocEndAttr< ::TTSched::Processor>(impl, meta_srcLink_end_); }

		};

		class  Bus :  public ::TTSched::MgaObject {
		public:
			static ::Uml::Class meta;

			Bus() { }
			Bus(Udm::ObjectImpl *impl) : ::TTSched::MgaObject(impl) { }
			Bus(const Bus &master) : ::TTSched::MgaObject(master) { }
			static Bus Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static Bus Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			Bus CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			Bus CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::TTSched::Bus> Instances() { return Udm::InstantiatedAttr< ::TTSched::Bus>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::TTSched::Bus, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::TTSched::Bus, Pred>(impl);}

			Udm::DerivedAttr< ::TTSched::Bus> Derived() { return Udm::DerivedAttr< ::TTSched::Bus>(impl);}
			template <class Pred> Udm::DerivedAttr< ::TTSched::Bus, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::TTSched::Bus, Pred>(impl);}

			Udm::ArchetypeAttr< ::TTSched::Bus> Archetype() { return Udm::ArchetypeAttr< ::TTSched::Bus>(impl);}

			static ::Uml::Attribute meta_Resolution;
			Udm::IntegerAttr Resolution() const { return Udm::IntegerAttr(impl, meta_Resolution); }

			static ::Uml::AssociationRole meta_srcLink;
			static ::Uml::AssociationRole meta_srcLink_rev;
			Udm::AClassAssocAttr< ::TTSched::Link, ::TTSched::Processor> srcLink() const { return Udm::AClassAssocAttr< ::TTSched::Link, ::TTSched::Processor>(impl, meta_srcLink, meta_srcLink_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::TTSched::Link, ::TTSched::Processor, Pred> srcLink_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::TTSched::Link, ::TTSched::Processor, Pred>(impl, meta_srcLink, meta_srcLink_rev); }

			static ::Uml::CompositionChildRole meta_Receiver_children;
			Udm::ChildrenAttr< ::TTSched::Receiver> Receiver_children() const { return Udm::ChildrenAttr< ::TTSched::Receiver>(impl, meta_Receiver_children); }
			template <class Pred> Udm::ChildrenAttr< ::TTSched::Receiver, Pred> Receiver_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::TTSched::Receiver, Pred>(impl, meta_Receiver_children); }

			static ::Uml::CompositionChildRole meta_Sender_children;
			Udm::ChildrenAttr< ::TTSched::Sender> Sender_children() const { return Udm::ChildrenAttr< ::TTSched::Sender>(impl, meta_Sender_children); }
			template <class Pred> Udm::ChildrenAttr< ::TTSched::Sender, Pred> Sender_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::TTSched::Sender, Pred>(impl, meta_Sender_children); }

			static ::Uml::CompositionChildRole meta_TimeSync_child;
			Udm::ChildAttr< ::TTSched::TimeSync> TimeSync_child() const { return Udm::ChildAttr< ::TTSched::TimeSync>(impl, meta_TimeSync_child); }

			static ::Uml::CompositionChildRole meta_Message_children;
			Udm::ChildrenAttr< ::TTSched::Message> Message_children() const { return Udm::ChildrenAttr< ::TTSched::Message>(impl, meta_Message_children); }
			template <class Pred> Udm::ChildrenAttr< ::TTSched::Message, Pred> Message_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::TTSched::Message, Pred>(impl, meta_Message_children); }

			static ::Uml::CompositionChildRole meta_TaskRef_children;
			Udm::ChildrenAttr< ::TTSched::TaskRef> TaskRef_children() const { return Udm::ChildrenAttr< ::TTSched::TaskRef>(impl, meta_TaskRef_children); }
			template <class Pred> Udm::ChildrenAttr< ::TTSched::TaskRef, Pred> TaskRef_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::TTSched::TaskRef, Pred>(impl, meta_TaskRef_children); }

			static ::Uml::CompositionChildRole meta_MsgInstances_child;
			Udm::ChildAttr< ::TTSched::MsgInstances> MsgInstances_child() const { return Udm::ChildAttr< ::TTSched::MsgInstances>(impl, meta_MsgInstances_child); }

			static ::Uml::CompositionChildRole meta_Dependencies_child;
			Udm::ChildAttr< ::TTSched::Dependencies> Dependencies_child() const { return Udm::ChildAttr< ::TTSched::Dependencies>(impl, meta_Dependencies_child); }

			Udm::ChildrenAttr< ::TTSched::TimeSync> TimeSync_kind_children() const { return Udm::ChildrenAttr< ::TTSched::TimeSync>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::TTSched::TimeSync, Pred> TimeSync_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::TTSched::TimeSync, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::TTSched::Message> Message_kind_children() const { return Udm::ChildrenAttr< ::TTSched::Message>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::TTSched::Message, Pred> Message_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::TTSched::Message, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::TTSched::Sender> Sender_kind_children() const { return Udm::ChildrenAttr< ::TTSched::Sender>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::TTSched::Sender, Pred> Sender_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::TTSched::Sender, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::TTSched::Receiver> Receiver_kind_children() const { return Udm::ChildrenAttr< ::TTSched::Receiver>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::TTSched::Receiver, Pred> Receiver_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::TTSched::Receiver, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::TTSched::TaskRef> TaskRef_kind_children() const { return Udm::ChildrenAttr< ::TTSched::TaskRef>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::TTSched::TaskRef, Pred> TaskRef_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::TTSched::TaskRef, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::TTSched::Dependencies> Dependencies_kind_children() const { return Udm::ChildrenAttr< ::TTSched::Dependencies>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::TTSched::Dependencies, Pred> Dependencies_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::TTSched::Dependencies, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::TTSched::MsgInstances> MsgInstances_kind_children() const { return Udm::ChildrenAttr< ::TTSched::MsgInstances>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::TTSched::MsgInstances, Pred> MsgInstances_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::TTSched::MsgInstances, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::TTSched::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr< ::TTSched::MgaObject>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::TTSched::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::TTSched::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

			static ::Uml::CompositionParentRole meta_TTSystem_parent;
			Udm::ParentAttr< ::TTSched::TTSystem> TTSystem_parent() const { return Udm::ParentAttr< ::TTSched::TTSystem>(impl, meta_TTSystem_parent); }

			static ::Uml::CompositionParentRole meta_InstanceModel_parent;
			Udm::ParentAttr< ::TTSched::InstanceModel> InstanceModel_parent() const { return Udm::ParentAttr< ::TTSched::InstanceModel>(impl, meta_InstanceModel_parent); }

			Udm::ParentAttr< ::TTSched::MgaObject> parent() const { return Udm::ParentAttr< ::TTSched::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  MsgRef :  public ::TTSched::MgaObject {
		public:
			static ::Uml::Class meta;

			MsgRef() { }
			MsgRef(Udm::ObjectImpl *impl) : ::TTSched::MgaObject(impl) { }
			MsgRef(const MsgRef &master) : ::TTSched::MgaObject(master) { }
			static MsgRef Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static MsgRef Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			MsgRef CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			MsgRef CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::TTSched::MsgRef> Instances() { return Udm::InstantiatedAttr< ::TTSched::MsgRef>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::TTSched::MsgRef, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::TTSched::MsgRef, Pred>(impl);}

			Udm::DerivedAttr< ::TTSched::MsgRef> Derived() { return Udm::DerivedAttr< ::TTSched::MsgRef>(impl);}
			template <class Pred> Udm::DerivedAttr< ::TTSched::MsgRef, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::TTSched::MsgRef, Pred>(impl);}

			Udm::ArchetypeAttr< ::TTSched::MsgRef> Archetype() { return Udm::ArchetypeAttr< ::TTSched::MsgRef>(impl);}

			static ::Uml::AssociationRole meta_ref;
			Udm::PointerAttr< ::TTSched::Message> ref() const { return Udm::PointerAttr< ::TTSched::Message>(impl, meta_ref); }

			static ::Uml::AssociationRole meta_dstEndMsg;
			static ::Uml::AssociationRole meta_dstEndMsg_rev;
			Udm::AClassAssocAttr< ::TTSched::EndMsg, ::TTSched::Latency> dstEndMsg() const { return Udm::AClassAssocAttr< ::TTSched::EndMsg, ::TTSched::Latency>(impl, meta_dstEndMsg, meta_dstEndMsg_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::TTSched::EndMsg, ::TTSched::Latency, Pred> dstEndMsg_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::TTSched::EndMsg, ::TTSched::Latency, Pred>(impl, meta_dstEndMsg, meta_dstEndMsg_rev); }

			static ::Uml::AssociationRole meta_srcStartMsg;
			static ::Uml::AssociationRole meta_srcStartMsg_rev;
			Udm::AClassAssocAttr< ::TTSched::StartMsg, ::TTSched::Latency> srcStartMsg() const { return Udm::AClassAssocAttr< ::TTSched::StartMsg, ::TTSched::Latency>(impl, meta_srcStartMsg, meta_srcStartMsg_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::TTSched::StartMsg, ::TTSched::Latency, Pred> srcStartMsg_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::TTSched::StartMsg, ::TTSched::Latency, Pred>(impl, meta_srcStartMsg, meta_srcStartMsg_rev); }

			static ::Uml::AssociationRole meta_dstDependency;
			static ::Uml::AssociationRole meta_dstDependency_rev;
			Udm::AClassAssocAttr< ::TTSched::Dependency, ::TTSched::MsgRef> dstDependency() const { return Udm::AClassAssocAttr< ::TTSched::Dependency, ::TTSched::MsgRef>(impl, meta_dstDependency, meta_dstDependency_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::TTSched::Dependency, ::TTSched::MsgRef, Pred> dstDependency_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::TTSched::Dependency, ::TTSched::MsgRef, Pred>(impl, meta_dstDependency, meta_dstDependency_rev); }

			static ::Uml::AssociationRole meta_srcDependency;
			static ::Uml::AssociationRole meta_srcDependency_rev;
			Udm::AClassAssocAttr< ::TTSched::Dependency, ::TTSched::MsgRef> srcDependency() const { return Udm::AClassAssocAttr< ::TTSched::Dependency, ::TTSched::MsgRef>(impl, meta_srcDependency, meta_srcDependency_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::TTSched::Dependency, ::TTSched::MsgRef, Pred> srcDependency_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::TTSched::Dependency, ::TTSched::MsgRef, Pred>(impl, meta_srcDependency, meta_srcDependency_rev); }

			static ::Uml::CompositionParentRole meta_LatencySpec_parent;
			Udm::ParentAttr< ::TTSched::LatencySpec> LatencySpec_parent() const { return Udm::ParentAttr< ::TTSched::LatencySpec>(impl, meta_LatencySpec_parent); }

			static ::Uml::CompositionParentRole meta_MsgInstances_parent;
			Udm::ParentAttr< ::TTSched::MsgInstances> MsgInstances_parent() const { return Udm::ParentAttr< ::TTSched::MsgInstances>(impl, meta_MsgInstances_parent); }

			Udm::ParentAttr< ::TTSched::MgaObject> parent() const { return Udm::ParentAttr< ::TTSched::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  Dependencies :  public ::TTSched::MgaObject {
		public:
			static ::Uml::Class meta;

			Dependencies() { }
			Dependencies(Udm::ObjectImpl *impl) : ::TTSched::MgaObject(impl) { }
			Dependencies(const Dependencies &master) : ::TTSched::MgaObject(master) { }
			static Dependencies Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static Dependencies Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			Dependencies CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			Dependencies CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::TTSched::Dependencies> Instances() { return Udm::InstantiatedAttr< ::TTSched::Dependencies>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::TTSched::Dependencies, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::TTSched::Dependencies, Pred>(impl);}

			Udm::DerivedAttr< ::TTSched::Dependencies> Derived() { return Udm::DerivedAttr< ::TTSched::Dependencies>(impl);}
			template <class Pred> Udm::DerivedAttr< ::TTSched::Dependencies, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::TTSched::Dependencies, Pred>(impl);}

			Udm::ArchetypeAttr< ::TTSched::Dependencies> Archetype() { return Udm::ArchetypeAttr< ::TTSched::Dependencies>(impl);}

			static ::Uml::CompositionChildRole meta_SenderInst_children;
			Udm::ChildrenAttr< ::TTSched::SenderInst> SenderInst_children() const { return Udm::ChildrenAttr< ::TTSched::SenderInst>(impl, meta_SenderInst_children); }
			template <class Pred> Udm::ChildrenAttr< ::TTSched::SenderInst, Pred> SenderInst_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::TTSched::SenderInst, Pred>(impl, meta_SenderInst_children); }

			static ::Uml::CompositionChildRole meta_TaskInstRef_children;
			Udm::ChildrenAttr< ::TTSched::TaskInstRef> TaskInstRef_children() const { return Udm::ChildrenAttr< ::TTSched::TaskInstRef>(impl, meta_TaskInstRef_children); }
			template <class Pred> Udm::ChildrenAttr< ::TTSched::TaskInstRef, Pred> TaskInstRef_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::TTSched::TaskInstRef, Pred>(impl, meta_TaskInstRef_children); }

			static ::Uml::CompositionChildRole meta_MsgInstRef_children;
			Udm::ChildrenAttr< ::TTSched::MsgInstRef> MsgInstRef_children() const { return Udm::ChildrenAttr< ::TTSched::MsgInstRef>(impl, meta_MsgInstRef_children); }
			template <class Pred> Udm::ChildrenAttr< ::TTSched::MsgInstRef, Pred> MsgInstRef_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::TTSched::MsgInstRef, Pred>(impl, meta_MsgInstRef_children); }

			static ::Uml::CompositionChildRole meta_ReceiverInst_children;
			Udm::ChildrenAttr< ::TTSched::ReceiverInst> ReceiverInst_children() const { return Udm::ChildrenAttr< ::TTSched::ReceiverInst>(impl, meta_ReceiverInst_children); }
			template <class Pred> Udm::ChildrenAttr< ::TTSched::ReceiverInst, Pred> ReceiverInst_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::TTSched::ReceiverInst, Pred>(impl, meta_ReceiverInst_children); }

			Udm::ChildrenAttr< ::TTSched::MsgInstRef> MsgInstRef_kind_children() const { return Udm::ChildrenAttr< ::TTSched::MsgInstRef>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::TTSched::MsgInstRef, Pred> MsgInstRef_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::TTSched::MsgInstRef, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::TTSched::ReceiverInst> ReceiverInst_kind_children() const { return Udm::ChildrenAttr< ::TTSched::ReceiverInst>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::TTSched::ReceiverInst, Pred> ReceiverInst_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::TTSched::ReceiverInst, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::TTSched::TaskInstRef> TaskInstRef_kind_children() const { return Udm::ChildrenAttr< ::TTSched::TaskInstRef>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::TTSched::TaskInstRef, Pred> TaskInstRef_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::TTSched::TaskInstRef, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::TTSched::SenderInst> SenderInst_kind_children() const { return Udm::ChildrenAttr< ::TTSched::SenderInst>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::TTSched::SenderInst, Pred> SenderInst_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::TTSched::SenderInst, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::TTSched::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr< ::TTSched::MgaObject>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::TTSched::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::TTSched::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

			static ::Uml::CompositionParentRole meta_Bus_parent;
			Udm::ParentAttr< ::TTSched::Bus> Bus_parent() const { return Udm::ParentAttr< ::TTSched::Bus>(impl, meta_Bus_parent); }

			Udm::ParentAttr< ::TTSched::MgaObject> parent() const { return Udm::ParentAttr< ::TTSched::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  MsgInstRef :  public ::TTSched::MgaObject {
		public:
			static ::Uml::Class meta;

			MsgInstRef() { }
			MsgInstRef(Udm::ObjectImpl *impl) : ::TTSched::MgaObject(impl) { }
			MsgInstRef(const MsgInstRef &master) : ::TTSched::MgaObject(master) { }
			static MsgInstRef Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static MsgInstRef Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			MsgInstRef CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			MsgInstRef CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::TTSched::MsgInstRef> Instances() { return Udm::InstantiatedAttr< ::TTSched::MsgInstRef>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::TTSched::MsgInstRef, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::TTSched::MsgInstRef, Pred>(impl);}

			Udm::DerivedAttr< ::TTSched::MsgInstRef> Derived() { return Udm::DerivedAttr< ::TTSched::MsgInstRef>(impl);}
			template <class Pred> Udm::DerivedAttr< ::TTSched::MsgInstRef, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::TTSched::MsgInstRef, Pred>(impl);}

			Udm::ArchetypeAttr< ::TTSched::MsgInstRef> Archetype() { return Udm::ArchetypeAttr< ::TTSched::MsgInstRef>(impl);}

			static ::Uml::AssociationRole meta_ref;
			Udm::PointerAttr< ::TTSched::MsgInstance> ref() const { return Udm::PointerAttr< ::TTSched::MsgInstance>(impl, meta_ref); }

			static ::Uml::AssociationRole meta_dstReceiverInst;
			static ::Uml::AssociationRole meta_dstReceiverInst_rev;
			Udm::AClassAssocAttr< ::TTSched::ReceiverInst, ::TTSched::TaskInstRef> dstReceiverInst() const { return Udm::AClassAssocAttr< ::TTSched::ReceiverInst, ::TTSched::TaskInstRef>(impl, meta_dstReceiverInst, meta_dstReceiverInst_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::TTSched::ReceiverInst, ::TTSched::TaskInstRef, Pred> dstReceiverInst_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::TTSched::ReceiverInst, ::TTSched::TaskInstRef, Pred>(impl, meta_dstReceiverInst, meta_dstReceiverInst_rev); }

			static ::Uml::AssociationRole meta_srcSenderInst;
			static ::Uml::AssociationRole meta_srcSenderInst_rev;
			Udm::AClassAssocAttr< ::TTSched::SenderInst, ::TTSched::TaskInstRef> srcSenderInst() const { return Udm::AClassAssocAttr< ::TTSched::SenderInst, ::TTSched::TaskInstRef>(impl, meta_srcSenderInst, meta_srcSenderInst_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::TTSched::SenderInst, ::TTSched::TaskInstRef, Pred> srcSenderInst_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::TTSched::SenderInst, ::TTSched::TaskInstRef, Pred>(impl, meta_srcSenderInst, meta_srcSenderInst_rev); }

			static ::Uml::CompositionParentRole meta_Dependencies_parent;
			Udm::ParentAttr< ::TTSched::Dependencies> Dependencies_parent() const { return Udm::ParentAttr< ::TTSched::Dependencies>(impl, meta_Dependencies_parent); }

			Udm::ParentAttr< ::TTSched::MgaObject> parent() const { return Udm::ParentAttr< ::TTSched::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  ReceiverInst :  public ::TTSched::MgaObject {
		public:
			static ::Uml::Class meta;

			ReceiverInst() { }
			ReceiverInst(Udm::ObjectImpl *impl) : ::TTSched::MgaObject(impl) { }
			ReceiverInst(const ReceiverInst &master) : ::TTSched::MgaObject(master) { }
			static ReceiverInst Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static ReceiverInst Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			ReceiverInst CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			ReceiverInst CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::TTSched::ReceiverInst> Instances() { return Udm::InstantiatedAttr< ::TTSched::ReceiverInst>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::TTSched::ReceiverInst, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::TTSched::ReceiverInst, Pred>(impl);}

			Udm::DerivedAttr< ::TTSched::ReceiverInst> Derived() { return Udm::DerivedAttr< ::TTSched::ReceiverInst>(impl);}
			template <class Pred> Udm::DerivedAttr< ::TTSched::ReceiverInst, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::TTSched::ReceiverInst, Pred>(impl);}

			Udm::ArchetypeAttr< ::TTSched::ReceiverInst> Archetype() { return Udm::ArchetypeAttr< ::TTSched::ReceiverInst>(impl);}

			static ::Uml::CompositionParentRole meta_Dependencies_parent;
			Udm::ParentAttr< ::TTSched::Dependencies> Dependencies_parent() const { return Udm::ParentAttr< ::TTSched::Dependencies>(impl, meta_Dependencies_parent); }

			Udm::ParentAttr< ::TTSched::MgaObject> parent() const { return Udm::ParentAttr< ::TTSched::MgaObject>(impl, Udm::NULLPARENTROLE); }
			static ::Uml::AssociationRole meta_srcReceiverInst_end_;
			Udm::AssocEndAttr< ::TTSched::MsgInstRef> srcReceiverInst_end() const { return Udm::AssocEndAttr< ::TTSched::MsgInstRef>(impl, meta_srcReceiverInst_end_); }

			static ::Uml::AssociationRole meta_dstReceiverInst_end_;
			Udm::AssocEndAttr< ::TTSched::TaskInstRef> dstReceiverInst_end() const { return Udm::AssocEndAttr< ::TTSched::TaskInstRef>(impl, meta_dstReceiverInst_end_); }

		};

		class  TaskInstRef :  public ::TTSched::MgaObject {
		public:
			static ::Uml::Class meta;

			TaskInstRef() { }
			TaskInstRef(Udm::ObjectImpl *impl) : ::TTSched::MgaObject(impl) { }
			TaskInstRef(const TaskInstRef &master) : ::TTSched::MgaObject(master) { }
			static TaskInstRef Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static TaskInstRef Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			TaskInstRef CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			TaskInstRef CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::TTSched::TaskInstRef> Instances() { return Udm::InstantiatedAttr< ::TTSched::TaskInstRef>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::TTSched::TaskInstRef, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::TTSched::TaskInstRef, Pred>(impl);}

			Udm::DerivedAttr< ::TTSched::TaskInstRef> Derived() { return Udm::DerivedAttr< ::TTSched::TaskInstRef>(impl);}
			template <class Pred> Udm::DerivedAttr< ::TTSched::TaskInstRef, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::TTSched::TaskInstRef, Pred>(impl);}

			Udm::ArchetypeAttr< ::TTSched::TaskInstRef> Archetype() { return Udm::ArchetypeAttr< ::TTSched::TaskInstRef>(impl);}

			static ::Uml::AssociationRole meta_srcReceiverInst;
			static ::Uml::AssociationRole meta_srcReceiverInst_rev;
			Udm::AClassAssocAttr< ::TTSched::ReceiverInst, ::TTSched::MsgInstRef> srcReceiverInst() const { return Udm::AClassAssocAttr< ::TTSched::ReceiverInst, ::TTSched::MsgInstRef>(impl, meta_srcReceiverInst, meta_srcReceiverInst_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::TTSched::ReceiverInst, ::TTSched::MsgInstRef, Pred> srcReceiverInst_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::TTSched::ReceiverInst, ::TTSched::MsgInstRef, Pred>(impl, meta_srcReceiverInst, meta_srcReceiverInst_rev); }

			static ::Uml::AssociationRole meta_dstSenderInst;
			static ::Uml::AssociationRole meta_dstSenderInst_rev;
			Udm::AClassAssocAttr< ::TTSched::SenderInst, ::TTSched::MsgInstRef> dstSenderInst() const { return Udm::AClassAssocAttr< ::TTSched::SenderInst, ::TTSched::MsgInstRef>(impl, meta_dstSenderInst, meta_dstSenderInst_rev); }
			template<class Pred> Udm::AClassAssocAttr< ::TTSched::SenderInst, ::TTSched::MsgInstRef, Pred> dstSenderInst_sorted(const Pred &) const { return Udm::AClassAssocAttr< ::TTSched::SenderInst, ::TTSched::MsgInstRef, Pred>(impl, meta_dstSenderInst, meta_dstSenderInst_rev); }

			static ::Uml::AssociationRole meta_ref;
			Udm::PointerAttr< ::TTSched::TaskInstance> ref() const { return Udm::PointerAttr< ::TTSched::TaskInstance>(impl, meta_ref); }

			static ::Uml::CompositionParentRole meta_Dependencies_parent;
			Udm::ParentAttr< ::TTSched::Dependencies> Dependencies_parent() const { return Udm::ParentAttr< ::TTSched::Dependencies>(impl, meta_Dependencies_parent); }

			Udm::ParentAttr< ::TTSched::MgaObject> parent() const { return Udm::ParentAttr< ::TTSched::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  SenderInst :  public ::TTSched::MgaObject {
		public:
			static ::Uml::Class meta;

			SenderInst() { }
			SenderInst(Udm::ObjectImpl *impl) : ::TTSched::MgaObject(impl) { }
			SenderInst(const SenderInst &master) : ::TTSched::MgaObject(master) { }
			static SenderInst Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static SenderInst Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			SenderInst CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			SenderInst CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::TTSched::SenderInst> Instances() { return Udm::InstantiatedAttr< ::TTSched::SenderInst>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::TTSched::SenderInst, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::TTSched::SenderInst, Pred>(impl);}

			Udm::DerivedAttr< ::TTSched::SenderInst> Derived() { return Udm::DerivedAttr< ::TTSched::SenderInst>(impl);}
			template <class Pred> Udm::DerivedAttr< ::TTSched::SenderInst, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::TTSched::SenderInst, Pred>(impl);}

			Udm::ArchetypeAttr< ::TTSched::SenderInst> Archetype() { return Udm::ArchetypeAttr< ::TTSched::SenderInst>(impl);}

			static ::Uml::CompositionParentRole meta_Dependencies_parent;
			Udm::ParentAttr< ::TTSched::Dependencies> Dependencies_parent() const { return Udm::ParentAttr< ::TTSched::Dependencies>(impl, meta_Dependencies_parent); }

			Udm::ParentAttr< ::TTSched::MgaObject> parent() const { return Udm::ParentAttr< ::TTSched::MgaObject>(impl, Udm::NULLPARENTROLE); }
			static ::Uml::AssociationRole meta_dstSenderInst_end_;
			Udm::AssocEndAttr< ::TTSched::MsgInstRef> dstSenderInst_end() const { return Udm::AssocEndAttr< ::TTSched::MsgInstRef>(impl, meta_dstSenderInst_end_); }

			static ::Uml::AssociationRole meta_srcSenderInst_end_;
			Udm::AssocEndAttr< ::TTSched::TaskInstRef> srcSenderInst_end() const { return Udm::AssocEndAttr< ::TTSched::TaskInstRef>(impl, meta_srcSenderInst_end_); }

		};

		class  InstanceModel :  public ::TTSched::MgaObject {
		public:
			static ::Uml::Class meta;

			InstanceModel() { }
			InstanceModel(Udm::ObjectImpl *impl) : ::TTSched::MgaObject(impl) { }
			InstanceModel(const InstanceModel &master) : ::TTSched::MgaObject(master) { }
			static InstanceModel Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static InstanceModel Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			InstanceModel CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			InstanceModel CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::TTSched::InstanceModel> Instances() { return Udm::InstantiatedAttr< ::TTSched::InstanceModel>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::TTSched::InstanceModel, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::TTSched::InstanceModel, Pred>(impl);}

			Udm::DerivedAttr< ::TTSched::InstanceModel> Derived() { return Udm::DerivedAttr< ::TTSched::InstanceModel>(impl);}
			template <class Pred> Udm::DerivedAttr< ::TTSched::InstanceModel, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::TTSched::InstanceModel, Pred>(impl);}

			Udm::ArchetypeAttr< ::TTSched::InstanceModel> Archetype() { return Udm::ArchetypeAttr< ::TTSched::InstanceModel>(impl);}

			static ::Uml::Attribute meta_HyperPeriod;
			Udm::IntegerAttr HyperPeriod() const { return Udm::IntegerAttr(impl, meta_HyperPeriod); }

			static ::Uml::AssociationRole meta_referedbyInstanceRef;
			Udm::AssocAttr< ::TTSched::InstanceRef> referedbyInstanceRef() const { return Udm::AssocAttr< ::TTSched::InstanceRef>(impl, meta_referedbyInstanceRef); }
			template <class Pred> Udm::AssocAttr< ::TTSched::InstanceRef, Pred > referedbyInstanceRef_sorted(const Pred &) const { return Udm::AssocAttr< ::TTSched::InstanceRef, Pred>(impl, meta_referedbyInstanceRef); }

			static ::Uml::CompositionChildRole meta_Bus_children;
			Udm::ChildrenAttr< ::TTSched::Bus> Bus_children() const { return Udm::ChildrenAttr< ::TTSched::Bus>(impl, meta_Bus_children); }
			template <class Pred> Udm::ChildrenAttr< ::TTSched::Bus, Pred> Bus_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::TTSched::Bus, Pred>(impl, meta_Bus_children); }

			static ::Uml::CompositionChildRole meta_Processor_children;
			Udm::ChildrenAttr< ::TTSched::Processor> Processor_children() const { return Udm::ChildrenAttr< ::TTSched::Processor>(impl, meta_Processor_children); }
			template <class Pred> Udm::ChildrenAttr< ::TTSched::Processor, Pred> Processor_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::TTSched::Processor, Pred>(impl, meta_Processor_children); }

			Udm::ChildrenAttr< ::TTSched::Processor> Processor_kind_children() const { return Udm::ChildrenAttr< ::TTSched::Processor>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::TTSched::Processor, Pred> Processor_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::TTSched::Processor, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::TTSched::Bus> Bus_kind_children() const { return Udm::ChildrenAttr< ::TTSched::Bus>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::TTSched::Bus, Pred> Bus_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::TTSched::Bus, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::TTSched::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr< ::TTSched::MgaObject>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::TTSched::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::TTSched::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

			static ::Uml::CompositionParentRole meta_RootFolder_parent;
			Udm::ParentAttr< ::TTSched::RootFolder> RootFolder_parent() const { return Udm::ParentAttr< ::TTSched::RootFolder>(impl, meta_RootFolder_parent); }

			Udm::ParentAttr< ::TTSched::RootFolder> parent() const { return Udm::ParentAttr< ::TTSched::RootFolder>(impl, Udm::NULLPARENTROLE); }
		};

		class  MsgInstances :  public ::TTSched::MgaObject {
		public:
			static ::Uml::Class meta;

			MsgInstances() { }
			MsgInstances(Udm::ObjectImpl *impl) : ::TTSched::MgaObject(impl) { }
			MsgInstances(const MsgInstances &master) : ::TTSched::MgaObject(master) { }
			static MsgInstances Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static MsgInstances Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			MsgInstances CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			MsgInstances CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::TTSched::MsgInstances> Instances() { return Udm::InstantiatedAttr< ::TTSched::MsgInstances>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::TTSched::MsgInstances, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::TTSched::MsgInstances, Pred>(impl);}

			Udm::DerivedAttr< ::TTSched::MsgInstances> Derived() { return Udm::DerivedAttr< ::TTSched::MsgInstances>(impl);}
			template <class Pred> Udm::DerivedAttr< ::TTSched::MsgInstances, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::TTSched::MsgInstances, Pred>(impl);}

			Udm::ArchetypeAttr< ::TTSched::MsgInstances> Archetype() { return Udm::ArchetypeAttr< ::TTSched::MsgInstances>(impl);}

			static ::Uml::CompositionChildRole meta_MsgRef_children;
			Udm::ChildrenAttr< ::TTSched::MsgRef> MsgRef_children() const { return Udm::ChildrenAttr< ::TTSched::MsgRef>(impl, meta_MsgRef_children); }
			template <class Pred> Udm::ChildrenAttr< ::TTSched::MsgRef, Pred> MsgRef_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::TTSched::MsgRef, Pred>(impl, meta_MsgRef_children); }

			static ::Uml::CompositionChildRole meta_MsgInstance_children;
			Udm::ChildrenAttr< ::TTSched::MsgInstance> MsgInstance_children() const { return Udm::ChildrenAttr< ::TTSched::MsgInstance>(impl, meta_MsgInstance_children); }
			template <class Pred> Udm::ChildrenAttr< ::TTSched::MsgInstance, Pred> MsgInstance_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::TTSched::MsgInstance, Pred>(impl, meta_MsgInstance_children); }

			Udm::ChildrenAttr< ::TTSched::MsgRef> MsgRef_kind_children() const { return Udm::ChildrenAttr< ::TTSched::MsgRef>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::TTSched::MsgRef, Pred> MsgRef_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::TTSched::MsgRef, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::TTSched::MsgInstance> MsgInstance_kind_children() const { return Udm::ChildrenAttr< ::TTSched::MsgInstance>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::TTSched::MsgInstance, Pred> MsgInstance_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::TTSched::MsgInstance, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::TTSched::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr< ::TTSched::MgaObject>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::TTSched::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::TTSched::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

			static ::Uml::CompositionParentRole meta_Bus_parent;
			Udm::ParentAttr< ::TTSched::Bus> Bus_parent() const { return Udm::ParentAttr< ::TTSched::Bus>(impl, meta_Bus_parent); }

			Udm::ParentAttr< ::TTSched::MgaObject> parent() const { return Udm::ParentAttr< ::TTSched::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  TaskInstances :  public ::TTSched::MgaObject {
		public:
			static ::Uml::Class meta;

			TaskInstances() { }
			TaskInstances(Udm::ObjectImpl *impl) : ::TTSched::MgaObject(impl) { }
			TaskInstances(const TaskInstances &master) : ::TTSched::MgaObject(master) { }
			static TaskInstances Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static TaskInstances Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			TaskInstances CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			TaskInstances CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::TTSched::TaskInstances> Instances() { return Udm::InstantiatedAttr< ::TTSched::TaskInstances>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::TTSched::TaskInstances, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::TTSched::TaskInstances, Pred>(impl);}

			Udm::DerivedAttr< ::TTSched::TaskInstances> Derived() { return Udm::DerivedAttr< ::TTSched::TaskInstances>(impl);}
			template <class Pred> Udm::DerivedAttr< ::TTSched::TaskInstances, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::TTSched::TaskInstances, Pred>(impl);}

			Udm::ArchetypeAttr< ::TTSched::TaskInstances> Archetype() { return Udm::ArchetypeAttr< ::TTSched::TaskInstances>(impl);}

			static ::Uml::CompositionChildRole meta_TaskRef_children;
			Udm::ChildrenAttr< ::TTSched::TaskRef> TaskRef_children() const { return Udm::ChildrenAttr< ::TTSched::TaskRef>(impl, meta_TaskRef_children); }
			template <class Pred> Udm::ChildrenAttr< ::TTSched::TaskRef, Pred> TaskRef_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::TTSched::TaskRef, Pred>(impl, meta_TaskRef_children); }

			static ::Uml::CompositionChildRole meta_TaskInstance_children;
			Udm::ChildrenAttr< ::TTSched::TaskInstance> TaskInstance_children() const { return Udm::ChildrenAttr< ::TTSched::TaskInstance>(impl, meta_TaskInstance_children); }
			template <class Pred> Udm::ChildrenAttr< ::TTSched::TaskInstance, Pred> TaskInstance_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::TTSched::TaskInstance, Pred>(impl, meta_TaskInstance_children); }

			Udm::ChildrenAttr< ::TTSched::TaskRef> TaskRef_kind_children() const { return Udm::ChildrenAttr< ::TTSched::TaskRef>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::TTSched::TaskRef, Pred> TaskRef_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::TTSched::TaskRef, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::TTSched::TaskInstance> TaskInstance_kind_children() const { return Udm::ChildrenAttr< ::TTSched::TaskInstance>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::TTSched::TaskInstance, Pred> TaskInstance_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::TTSched::TaskInstance, Pred>(impl, Udm::NULLCHILDROLE); }

			Udm::ChildrenAttr< ::TTSched::MgaObject> MgaObject_kind_children() const { return Udm::ChildrenAttr< ::TTSched::MgaObject>(impl, Udm::NULLCHILDROLE); }
			template<class Pred> Udm::ChildrenAttr< ::TTSched::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return Udm::ChildrenAttr< ::TTSched::MgaObject, Pred>(impl, Udm::NULLCHILDROLE); }

			static ::Uml::CompositionParentRole meta_Processor_parent;
			Udm::ParentAttr< ::TTSched::Processor> Processor_parent() const { return Udm::ParentAttr< ::TTSched::Processor>(impl, meta_Processor_parent); }

			Udm::ParentAttr< ::TTSched::MgaObject> parent() const { return Udm::ParentAttr< ::TTSched::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  MsgInstance :  public ::TTSched::MgaObject {
		public:
			static ::Uml::Class meta;

			MsgInstance() { }
			MsgInstance(Udm::ObjectImpl *impl) : ::TTSched::MgaObject(impl) { }
			MsgInstance(const MsgInstance &master) : ::TTSched::MgaObject(master) { }
			static MsgInstance Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static MsgInstance Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			MsgInstance CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			MsgInstance CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::TTSched::MsgInstance> Instances() { return Udm::InstantiatedAttr< ::TTSched::MsgInstance>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::TTSched::MsgInstance, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::TTSched::MsgInstance, Pred>(impl);}

			Udm::DerivedAttr< ::TTSched::MsgInstance> Derived() { return Udm::DerivedAttr< ::TTSched::MsgInstance>(impl);}
			template <class Pred> Udm::DerivedAttr< ::TTSched::MsgInstance, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::TTSched::MsgInstance, Pred>(impl);}

			Udm::ArchetypeAttr< ::TTSched::MsgInstance> Archetype() { return Udm::ArchetypeAttr< ::TTSched::MsgInstance>(impl);}

			static ::Uml::Attribute meta_RefName;
			Udm::StringAttr RefName() const { return Udm::StringAttr(impl, meta_RefName); }

			static ::Uml::Attribute meta_StartTime;
			Udm::IntegerAttr StartTime() const { return Udm::IntegerAttr(impl, meta_StartTime); }

			static ::Uml::Attribute meta_InstID;
			Udm::IntegerAttr InstID() const { return Udm::IntegerAttr(impl, meta_InstID); }

			static ::Uml::AssociationRole meta_referedbyMsgInstRef;
			Udm::AssocAttr< ::TTSched::MsgInstRef> referedbyMsgInstRef() const { return Udm::AssocAttr< ::TTSched::MsgInstRef>(impl, meta_referedbyMsgInstRef); }
			template <class Pred> Udm::AssocAttr< ::TTSched::MsgInstRef, Pred > referedbyMsgInstRef_sorted(const Pred &) const { return Udm::AssocAttr< ::TTSched::MsgInstRef, Pred>(impl, meta_referedbyMsgInstRef); }

			static ::Uml::CompositionParentRole meta_MsgInstances_parent;
			Udm::ParentAttr< ::TTSched::MsgInstances> MsgInstances_parent() const { return Udm::ParentAttr< ::TTSched::MsgInstances>(impl, meta_MsgInstances_parent); }

			Udm::ParentAttr< ::TTSched::MgaObject> parent() const { return Udm::ParentAttr< ::TTSched::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};

		class  TaskInstance :  public ::TTSched::MgaObject {
		public:
			static ::Uml::Class meta;

			TaskInstance() { }
			TaskInstance(Udm::ObjectImpl *impl) : ::TTSched::MgaObject(impl) { }
			TaskInstance(const TaskInstance &master) : ::TTSched::MgaObject(master) { }
			static TaskInstance Cast(const Udm::Object &a) { return __Cast(a, meta); }

			static TaskInstance Create(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role); }

			TaskInstance CreateInstance(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl); }

			TaskInstance CreateDerived(const Udm::Object &parent, const ::Uml::CompositionChildRole &role = Udm::NULLCHILDROLE) { return __Create(meta, parent, role, impl, true); }

			Udm::InstantiatedAttr< ::TTSched::TaskInstance> Instances() { return Udm::InstantiatedAttr< ::TTSched::TaskInstance>(impl);}
			template <class Pred> Udm::InstantiatedAttr< ::TTSched::TaskInstance, Pred> Instances_sorted(const Pred &) { return Udm::InstantiatedAttr< ::TTSched::TaskInstance, Pred>(impl);}

			Udm::DerivedAttr< ::TTSched::TaskInstance> Derived() { return Udm::DerivedAttr< ::TTSched::TaskInstance>(impl);}
			template <class Pred> Udm::DerivedAttr< ::TTSched::TaskInstance, Pred> Derived_sorted(const Pred &) { return Udm::DerivedAttr< ::TTSched::TaskInstance, Pred>(impl);}

			Udm::ArchetypeAttr< ::TTSched::TaskInstance> Archetype() { return Udm::ArchetypeAttr< ::TTSched::TaskInstance>(impl);}

			static ::Uml::Attribute meta_RefName;
			Udm::StringAttr RefName() const { return Udm::StringAttr(impl, meta_RefName); }

			static ::Uml::Attribute meta_StartTime;
			Udm::IntegerAttr StartTime() const { return Udm::IntegerAttr(impl, meta_StartTime); }

			static ::Uml::Attribute meta_InstID;
			Udm::IntegerAttr InstID() const { return Udm::IntegerAttr(impl, meta_InstID); }

			static ::Uml::AssociationRole meta_referedbyTaskInstRef;
			Udm::AssocAttr< ::TTSched::TaskInstRef> referedbyTaskInstRef() const { return Udm::AssocAttr< ::TTSched::TaskInstRef>(impl, meta_referedbyTaskInstRef); }
			template <class Pred> Udm::AssocAttr< ::TTSched::TaskInstRef, Pred > referedbyTaskInstRef_sorted(const Pred &) const { return Udm::AssocAttr< ::TTSched::TaskInstRef, Pred>(impl, meta_referedbyTaskInstRef); }

			static ::Uml::CompositionParentRole meta_TaskInstances_parent;
			Udm::ParentAttr< ::TTSched::TaskInstances> TaskInstances_parent() const { return Udm::ParentAttr< ::TTSched::TaskInstances>(impl, meta_TaskInstances_parent); }

			Udm::ParentAttr< ::TTSched::MgaObject> parent() const { return Udm::ParentAttr< ::TTSched::MgaObject>(impl, Udm::NULLPARENTROLE); }
		};



}

#endif //MOBIES_TTSCHED_H
