#pragma once
#ifndef MOBIES_RTT_H
#define MOBIES_RTT_H

// header file RTT.h generated from diagram RTT
// generated with Udm version 3.34 on Wed Jul 27 11:42:41 2016

#include <UdmBase.h>

#if !defined(UDM_VERSION_MAJOR) || !defined(UDM_VERSION_MINOR)
#    error "Udm headers too old, they do not define UDM_VERSION"
#elif UDM_VERSION_MAJOR < 3
#    error "Udm headers too old, minimum version required 3.34"
#elif UDM_VERSION_MAJOR == 3 && UDM_VERSION_MINOR < 34
#    error "Udm headers too old, minimum version required 3.34"
#endif

#include <Uml.h>


#ifdef min
#undef min
#endif

#ifdef max
#undef max
#endif

namespace RTT {

	extern ::Uml::Diagram meta;
	class DataNetwork;
	class MgaObject;
	class RTTRoot;
	class ExecuteRule;
	class Sequence;
	class Group;
	class UdmObject;
	class TracesTo;
	class RootFolder;

	void Initialize();
	void Initialize(const ::Uml::Diagram &dgr);

	extern  ::Udm::UdmDiagram diagram;

	class MgaObject : public ::Udm::Object {
	public:
		MgaObject();
		MgaObject(::Udm::ObjectImpl *impl);
		MgaObject(const MgaObject &master);

#ifdef UDM_RVALUE
		MgaObject(MgaObject &&master);

		static MgaObject Cast(::Udm::Object &&a);
		MgaObject& operator=(MgaObject &&a);
		MgaObject& operator=(const MgaObject &a);

#endif
		static MgaObject Cast(const ::Udm::Object &a);
		static MgaObject Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		MgaObject CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::InstantiatedAttr< MgaObject> Instances();
		template <class Pred> ::Udm::InstantiatedAttr< MgaObject, Pred> Instances_sorted(const Pred &) { return ::Udm::InstantiatedAttr< MgaObject, Pred>(impl); };
		MgaObject CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::DerivedAttr< MgaObject> Derived();
		template <class Pred> ::Udm::DerivedAttr< MgaObject, Pred> Derived_sorted(const Pred &) { return ::Udm::DerivedAttr< MgaObject, Pred>(impl); };
		::Udm::ArchetypeAttr< MgaObject> Archetype() const;
		::Udm::StringAttr position() const;
		::Udm::StringAttr name() const;
		::Udm::ParentAttr< ::Udm::Object> parent() const;

		static ::Uml::Class meta;
		static ::Uml::Attribute meta_position;
		static ::Uml::Attribute meta_name;

	};

	class DataNetwork :  public MgaObject {
	public:
		DataNetwork();
		DataNetwork(::Udm::ObjectImpl *impl);
		DataNetwork(const DataNetwork &master);

#ifdef UDM_RVALUE
		DataNetwork(DataNetwork &&master);

		static DataNetwork Cast(::Udm::Object &&a);
		DataNetwork& operator=(DataNetwork &&a);
		DataNetwork& operator=(const DataNetwork &a);

#endif
		static DataNetwork Cast(const ::Udm::Object &a);
		static DataNetwork Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		DataNetwork CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::InstantiatedAttr< DataNetwork> Instances();
		template <class Pred> ::Udm::InstantiatedAttr< DataNetwork, Pred> Instances_sorted(const Pred &) { return ::Udm::InstantiatedAttr< DataNetwork, Pred>(impl); };
		DataNetwork CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::DerivedAttr< DataNetwork> Derived();
		template <class Pred> ::Udm::DerivedAttr< DataNetwork, Pred> Derived_sorted(const Pred &) { return ::Udm::DerivedAttr< DataNetwork, Pred>(impl); };
		::Udm::ArchetypeAttr< DataNetwork> Archetype() const;
		::Udm::StringAttr filePath() const;
		::Udm::IntegerAttr dataNetworkId() const;
		::Udm::ParentAttr< ::RTT::RTTRoot> RTTRoot_parent() const;
		::Udm::ParentAttr< ::RTT::RTTRoot> parent() const;

		static ::Uml::Class meta;
		static ::Uml::Attribute meta_filePath;
		static ::Uml::Attribute meta_dataNetworkId;
		static ::Uml::CompositionParentRole meta_RTTRoot_parent;

	};

	class RTTRoot :  public MgaObject {
	public:
		RTTRoot();
		RTTRoot(::Udm::ObjectImpl *impl);
		RTTRoot(const RTTRoot &master);

#ifdef UDM_RVALUE
		RTTRoot(RTTRoot &&master);

		static RTTRoot Cast(::Udm::Object &&a);
		RTTRoot& operator=(RTTRoot &&a);
		RTTRoot& operator=(const RTTRoot &a);

#endif
		static RTTRoot Cast(const ::Udm::Object &a);
		static RTTRoot Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		RTTRoot CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::InstantiatedAttr< RTTRoot> Instances();
		template <class Pred> ::Udm::InstantiatedAttr< RTTRoot, Pred> Instances_sorted(const Pred &) { return ::Udm::InstantiatedAttr< RTTRoot, Pred>(impl); };
		RTTRoot CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::DerivedAttr< RTTRoot> Derived();
		template <class Pred> ::Udm::DerivedAttr< RTTRoot, Pred> Derived_sorted(const Pred &) { return ::Udm::DerivedAttr< RTTRoot, Pred>(impl); };
		::Udm::ArchetypeAttr< RTTRoot> Archetype() const;
		::Udm::ChildrenAttr< ::RTT::DataNetwork> DataNetwork_children() const;
		template <class Pred> ::Udm::ChildrenAttr< ::RTT::DataNetwork, Pred> DataNetwork_children_sorted(const Pred &) const { return ::Udm::ChildrenAttr< ::RTT::DataNetwork, Pred>(impl, meta_DataNetwork_children); };
		::Udm::ChildrenAttr< ::RTT::ExecuteRule> ExecuteRule_children() const;
		template <class Pred> ::Udm::ChildrenAttr< ::RTT::ExecuteRule, Pred> ExecuteRule_children_sorted(const Pred &) const { return ::Udm::ChildrenAttr< ::RTT::ExecuteRule, Pred>(impl, meta_ExecuteRule_children); };
		::Udm::ChildrenAttr< ::RTT::Sequence> Sequence_children() const;
		template <class Pred> ::Udm::ChildrenAttr< ::RTT::Sequence, Pred> Sequence_children_sorted(const Pred &) const { return ::Udm::ChildrenAttr< ::RTT::Sequence, Pred>(impl, meta_Sequence_children); };
		::Udm::ChildrenAttr< ::RTT::DataNetwork> DataNetwork_kind_children() const;
		template <class Pred> ::Udm::ChildrenAttr< ::RTT::DataNetwork, Pred> DataNetwork_kind_children_sorted(const Pred &) const { return ::Udm::ChildrenAttr< ::RTT::DataNetwork, Pred>(impl, ::Udm::NULLCHILDROLE); };
		::Udm::ChildrenAttr< ::RTT::MgaObject> MgaObject_kind_children() const;
		template <class Pred> ::Udm::ChildrenAttr< ::RTT::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return ::Udm::ChildrenAttr< ::RTT::MgaObject, Pred>(impl, ::Udm::NULLCHILDROLE); };
		::Udm::ChildrenAttr< ::RTT::ExecuteRule> ExecuteRule_kind_children() const;
		template <class Pred> ::Udm::ChildrenAttr< ::RTT::ExecuteRule, Pred> ExecuteRule_kind_children_sorted(const Pred &) const { return ::Udm::ChildrenAttr< ::RTT::ExecuteRule, Pred>(impl, ::Udm::NULLCHILDROLE); };
		::Udm::ChildrenAttr< ::RTT::Sequence> Sequence_kind_children() const;
		template <class Pred> ::Udm::ChildrenAttr< ::RTT::Sequence, Pred> Sequence_kind_children_sorted(const Pred &) const { return ::Udm::ChildrenAttr< ::RTT::Sequence, Pred>(impl, ::Udm::NULLCHILDROLE); };
		::Udm::ParentAttr< ::RTT::RootFolder> RootFolder_parent() const;
		::Udm::ParentAttr< ::RTT::RootFolder> parent() const;

		static ::Uml::Class meta;
		static ::Uml::CompositionChildRole meta_DataNetwork_children;
		static ::Uml::CompositionChildRole meta_ExecuteRule_children;
		static ::Uml::CompositionChildRole meta_Sequence_children;
		static ::Uml::CompositionParentRole meta_RootFolder_parent;

	};

	class ExecuteRule :  public MgaObject {
	public:
		ExecuteRule();
		ExecuteRule(::Udm::ObjectImpl *impl);
		ExecuteRule(const ExecuteRule &master);

#ifdef UDM_RVALUE
		ExecuteRule(ExecuteRule &&master);

		static ExecuteRule Cast(::Udm::Object &&a);
		ExecuteRule& operator=(ExecuteRule &&a);
		ExecuteRule& operator=(const ExecuteRule &a);

#endif
		static ExecuteRule Cast(const ::Udm::Object &a);
		static ExecuteRule Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		ExecuteRule CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::InstantiatedAttr< ExecuteRule> Instances();
		template <class Pred> ::Udm::InstantiatedAttr< ExecuteRule, Pred> Instances_sorted(const Pred &) { return ::Udm::InstantiatedAttr< ExecuteRule, Pred>(impl); };
		ExecuteRule CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::DerivedAttr< ExecuteRule> Derived();
		template <class Pred> ::Udm::DerivedAttr< ExecuteRule, Pred> Derived_sorted(const Pred &) { return ::Udm::DerivedAttr< ExecuteRule, Pred>(impl); };
		::Udm::ArchetypeAttr< ExecuteRule> Archetype() const;
		::Udm::IntegerAttr uniqueObjId() const;
		::Udm::AClassAssocAttr< Sequence, ExecuteRule> dstSequence() const;
		template <class Pred> ::Udm::AClassAssocAttr< Sequence, ExecuteRule, Pred> dstSequence_sorted(const Pred &) const { return ::Udm::AClassAssocAttr< Sequence, ExecuteRule, Pred>(impl, meta_dstSequence, meta_dstSequence_rev); };
		::Udm::AClassAssocAttr< Sequence, ExecuteRule> srcSequence() const;
		template <class Pred> ::Udm::AClassAssocAttr< Sequence, ExecuteRule, Pred> srcSequence_sorted(const Pred &) const { return ::Udm::AClassAssocAttr< Sequence, ExecuteRule, Pred>(impl, meta_srcSequence, meta_srcSequence_rev); };
		::Udm::ChildrenAttr< ::RTT::UdmObject> UdmObject_children() const;
		template <class Pred> ::Udm::ChildrenAttr< ::RTT::UdmObject, Pred> UdmObject_children_sorted(const Pred &) const { return ::Udm::ChildrenAttr< ::RTT::UdmObject, Pred>(impl, meta_UdmObject_children); };
		::Udm::ChildrenAttr< ::RTT::Group> Group_children() const;
		template <class Pred> ::Udm::ChildrenAttr< ::RTT::Group, Pred> Group_children_sorted(const Pred &) const { return ::Udm::ChildrenAttr< ::RTT::Group, Pred>(impl, meta_Group_children); };
		::Udm::ChildrenAttr< ::RTT::TracesTo> TracesTo_children() const;
		template <class Pred> ::Udm::ChildrenAttr< ::RTT::TracesTo, Pred> TracesTo_children_sorted(const Pred &) const { return ::Udm::ChildrenAttr< ::RTT::TracesTo, Pred>(impl, meta_TracesTo_children); };
		::Udm::ChildrenAttr< ::RTT::MgaObject> MgaObject_kind_children() const;
		template <class Pred> ::Udm::ChildrenAttr< ::RTT::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return ::Udm::ChildrenAttr< ::RTT::MgaObject, Pred>(impl, ::Udm::NULLCHILDROLE); };
		::Udm::ChildrenAttr< ::RTT::Group> Group_kind_children() const;
		template <class Pred> ::Udm::ChildrenAttr< ::RTT::Group, Pred> Group_kind_children_sorted(const Pred &) const { return ::Udm::ChildrenAttr< ::RTT::Group, Pred>(impl, ::Udm::NULLCHILDROLE); };
		::Udm::ChildrenAttr< ::RTT::UdmObject> UdmObject_kind_children() const;
		template <class Pred> ::Udm::ChildrenAttr< ::RTT::UdmObject, Pred> UdmObject_kind_children_sorted(const Pred &) const { return ::Udm::ChildrenAttr< ::RTT::UdmObject, Pred>(impl, ::Udm::NULLCHILDROLE); };
		::Udm::ChildrenAttr< ::RTT::TracesTo> TracesTo_kind_children() const;
		template <class Pred> ::Udm::ChildrenAttr< ::RTT::TracesTo, Pred> TracesTo_kind_children_sorted(const Pred &) const { return ::Udm::ChildrenAttr< ::RTT::TracesTo, Pred>(impl, ::Udm::NULLCHILDROLE); };
		::Udm::ParentAttr< ::RTT::RTTRoot> RTTRoot_parent() const;
		::Udm::ParentAttr< ::RTT::RTTRoot> parent() const;

		static ::Uml::Class meta;
		static ::Uml::Attribute meta_uniqueObjId;
		static ::Uml::AssociationRole meta_dstSequence;
		static ::Uml::AssociationRole meta_dstSequence_rev;
		static ::Uml::AssociationRole meta_srcSequence;
		static ::Uml::AssociationRole meta_srcSequence_rev;
		static ::Uml::CompositionChildRole meta_UdmObject_children;
		static ::Uml::CompositionChildRole meta_Group_children;
		static ::Uml::CompositionChildRole meta_TracesTo_children;
		static ::Uml::CompositionParentRole meta_RTTRoot_parent;

	};

	class Sequence :  public MgaObject {
	public:
		Sequence();
		Sequence(::Udm::ObjectImpl *impl);
		Sequence(const Sequence &master);

#ifdef UDM_RVALUE
		Sequence(Sequence &&master);

		static Sequence Cast(::Udm::Object &&a);
		Sequence& operator=(Sequence &&a);
		Sequence& operator=(const Sequence &a);

#endif
		static Sequence Cast(const ::Udm::Object &a);
		static Sequence Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		Sequence CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::InstantiatedAttr< Sequence> Instances();
		template <class Pred> ::Udm::InstantiatedAttr< Sequence, Pred> Instances_sorted(const Pred &) { return ::Udm::InstantiatedAttr< Sequence, Pred>(impl); };
		Sequence CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::DerivedAttr< Sequence> Derived();
		template <class Pred> ::Udm::DerivedAttr< Sequence, Pred> Derived_sorted(const Pred &) { return ::Udm::DerivedAttr< Sequence, Pred>(impl); };
		::Udm::ArchetypeAttr< Sequence> Archetype() const;
		::Udm::ParentAttr< ::RTT::RTTRoot> RTTRoot_parent() const;
		::Udm::ParentAttr< ::RTT::RTTRoot> parent() const;
		::Udm::AssocEndAttr< ::RTT::ExecuteRule> srcSequence_end() const;
		::Udm::AssocEndAttr< ::RTT::ExecuteRule> dstSequence_end() const;

		static ::Uml::Class meta;
		static ::Uml::CompositionParentRole meta_RTTRoot_parent;
		static ::Uml::AssociationRole meta_srcSequence_end_;
		static ::Uml::AssociationRole meta_dstSequence_end_;

	};

	class Group :  public MgaObject {
	public:
		Group();
		Group(::Udm::ObjectImpl *impl);
		Group(const Group &master);

#ifdef UDM_RVALUE
		Group(Group &&master);

		static Group Cast(::Udm::Object &&a);
		Group& operator=(Group &&a);
		Group& operator=(const Group &a);

#endif
		static Group Cast(const ::Udm::Object &a);
		static Group Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		Group CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::InstantiatedAttr< Group> Instances();
		template <class Pred> ::Udm::InstantiatedAttr< Group, Pred> Instances_sorted(const Pred &) { return ::Udm::InstantiatedAttr< Group, Pred>(impl); };
		Group CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::DerivedAttr< Group> Derived();
		template <class Pred> ::Udm::DerivedAttr< Group, Pred> Derived_sorted(const Pred &) { return ::Udm::DerivedAttr< Group, Pred>(impl); };
		::Udm::ArchetypeAttr< Group> Archetype() const;
		::Udm::AssocAttr< UdmObject> members() const;
		template <class Pred> ::Udm::AssocAttr< UdmObject, Pred> members_sorted(const Pred &) const { return ::Udm::AssocAttr< UdmObject, Pred>(impl, meta_members); };
		::Udm::ParentAttr< ::RTT::ExecuteRule> ExecuteRule_parent() const;
		::Udm::ParentAttr< ::RTT::ExecuteRule> parent() const;

		static ::Uml::Class meta;
		static ::Uml::AssociationRole meta_members;
		static ::Uml::CompositionParentRole meta_ExecuteRule_parent;

	};

	class UdmObject :  public MgaObject {
	public:
		UdmObject();
		UdmObject(::Udm::ObjectImpl *impl);
		UdmObject(const UdmObject &master);

#ifdef UDM_RVALUE
		UdmObject(UdmObject &&master);

		static UdmObject Cast(::Udm::Object &&a);
		UdmObject& operator=(UdmObject &&a);
		UdmObject& operator=(const UdmObject &a);

#endif
		static UdmObject Cast(const ::Udm::Object &a);
		static UdmObject Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		UdmObject CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::InstantiatedAttr< UdmObject> Instances();
		template <class Pred> ::Udm::InstantiatedAttr< UdmObject, Pred> Instances_sorted(const Pred &) { return ::Udm::InstantiatedAttr< UdmObject, Pred>(impl); };
		UdmObject CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::DerivedAttr< UdmObject> Derived();
		template <class Pred> ::Udm::DerivedAttr< UdmObject, Pred> Derived_sorted(const Pred &) { return ::Udm::DerivedAttr< UdmObject, Pred>(impl); };
		::Udm::ArchetypeAttr< UdmObject> Archetype() const;
		::Udm::IntegerAttr dataNetworkId() const;
		::Udm::IntegerAttr uniqueObjId() const;
		::Udm::AClassAssocAttr< TracesTo, UdmObject> dstTracesTo() const;
		template <class Pred> ::Udm::AClassAssocAttr< TracesTo, UdmObject, Pred> dstTracesTo_sorted(const Pred &) const { return ::Udm::AClassAssocAttr< TracesTo, UdmObject, Pred>(impl, meta_dstTracesTo, meta_dstTracesTo_rev); };
		::Udm::AClassAssocAttr< TracesTo, UdmObject> srcTracesTo() const;
		template <class Pred> ::Udm::AClassAssocAttr< TracesTo, UdmObject, Pred> srcTracesTo_sorted(const Pred &) const { return ::Udm::AClassAssocAttr< TracesTo, UdmObject, Pred>(impl, meta_srcTracesTo, meta_srcTracesTo_rev); };
		::Udm::AssocAttr< Group> setGroup() const;
		template <class Pred> ::Udm::AssocAttr< Group, Pred> setGroup_sorted(const Pred &) const { return ::Udm::AssocAttr< Group, Pred>(impl, meta_setGroup); };
		::Udm::ParentAttr< ::RTT::ExecuteRule> ExecuteRule_parent() const;
		::Udm::ParentAttr< ::RTT::ExecuteRule> parent() const;

		static ::Uml::Class meta;
		static ::Uml::Attribute meta_dataNetworkId;
		static ::Uml::Attribute meta_uniqueObjId;
		static ::Uml::AssociationRole meta_dstTracesTo;
		static ::Uml::AssociationRole meta_dstTracesTo_rev;
		static ::Uml::AssociationRole meta_srcTracesTo;
		static ::Uml::AssociationRole meta_srcTracesTo_rev;
		static ::Uml::AssociationRole meta_setGroup;
		static ::Uml::CompositionParentRole meta_ExecuteRule_parent;

	};

	class TracesTo :  public MgaObject {
	public:
		TracesTo();
		TracesTo(::Udm::ObjectImpl *impl);
		TracesTo(const TracesTo &master);

#ifdef UDM_RVALUE
		TracesTo(TracesTo &&master);

		static TracesTo Cast(::Udm::Object &&a);
		TracesTo& operator=(TracesTo &&a);
		TracesTo& operator=(const TracesTo &a);

#endif
		static TracesTo Cast(const ::Udm::Object &a);
		static TracesTo Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		TracesTo CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::InstantiatedAttr< TracesTo> Instances();
		template <class Pred> ::Udm::InstantiatedAttr< TracesTo, Pred> Instances_sorted(const Pred &) { return ::Udm::InstantiatedAttr< TracesTo, Pred>(impl); };
		TracesTo CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::DerivedAttr< TracesTo> Derived();
		template <class Pred> ::Udm::DerivedAttr< TracesTo, Pred> Derived_sorted(const Pred &) { return ::Udm::DerivedAttr< TracesTo, Pred>(impl); };
		::Udm::ArchetypeAttr< TracesTo> Archetype() const;
		::Udm::ParentAttr< ::RTT::ExecuteRule> ExecuteRule_parent() const;
		::Udm::ParentAttr< ::RTT::ExecuteRule> parent() const;
		::Udm::AssocEndAttr< ::RTT::UdmObject> srcTracesTo_end() const;
		::Udm::AssocEndAttr< ::RTT::UdmObject> dstTracesTo_end() const;

		static ::Uml::Class meta;
		static ::Uml::CompositionParentRole meta_ExecuteRule_parent;
		static ::Uml::AssociationRole meta_srcTracesTo_end_;
		static ::Uml::AssociationRole meta_dstTracesTo_end_;

	};

	class RootFolder : public ::Udm::Object {
	public:
		RootFolder();
		RootFolder(::Udm::ObjectImpl *impl);
		RootFolder(const RootFolder &master);

#ifdef UDM_RVALUE
		RootFolder(RootFolder &&master);

		static RootFolder Cast(::Udm::Object &&a);
		RootFolder& operator=(RootFolder &&a);
		RootFolder& operator=(const RootFolder &a);

#endif
		static RootFolder Cast(const ::Udm::Object &a);
		static RootFolder Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		RootFolder CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::InstantiatedAttr< RootFolder> Instances();
		template <class Pred> ::Udm::InstantiatedAttr< RootFolder, Pred> Instances_sorted(const Pred &) { return ::Udm::InstantiatedAttr< RootFolder, Pred>(impl); };
		RootFolder CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::DerivedAttr< RootFolder> Derived();
		template <class Pred> ::Udm::DerivedAttr< RootFolder, Pred> Derived_sorted(const Pred &) { return ::Udm::DerivedAttr< RootFolder, Pred>(impl); };
		::Udm::ArchetypeAttr< RootFolder> Archetype() const;
		::Udm::StringAttr name() const;
		::Udm::ChildrenAttr< ::RTT::RTTRoot> RTTRoot_children() const;
		template <class Pred> ::Udm::ChildrenAttr< ::RTT::RTTRoot, Pred> RTTRoot_children_sorted(const Pred &) const { return ::Udm::ChildrenAttr< ::RTT::RTTRoot, Pred>(impl, meta_RTTRoot_children); };
		::Udm::ChildrenAttr< ::RTT::RootFolder> RootFolder_children() const;
		template <class Pred> ::Udm::ChildrenAttr< ::RTT::RootFolder, Pred> RootFolder_children_sorted(const Pred &) const { return ::Udm::ChildrenAttr< ::RTT::RootFolder, Pred>(impl, meta_RootFolder_children); };
		::Udm::ChildrenAttr< ::RTT::MgaObject> MgaObject_kind_children() const;
		template <class Pred> ::Udm::ChildrenAttr< ::RTT::MgaObject, Pred> MgaObject_kind_children_sorted(const Pred &) const { return ::Udm::ChildrenAttr< ::RTT::MgaObject, Pred>(impl, ::Udm::NULLCHILDROLE); };
		::Udm::ChildrenAttr< ::RTT::RTTRoot> RTTRoot_kind_children() const;
		template <class Pred> ::Udm::ChildrenAttr< ::RTT::RTTRoot, Pred> RTTRoot_kind_children_sorted(const Pred &) const { return ::Udm::ChildrenAttr< ::RTT::RTTRoot, Pred>(impl, ::Udm::NULLCHILDROLE); };
		::Udm::ChildrenAttr< ::RTT::RootFolder> RootFolder_kind_children() const;
		template <class Pred> ::Udm::ChildrenAttr< ::RTT::RootFolder, Pred> RootFolder_kind_children_sorted(const Pred &) const { return ::Udm::ChildrenAttr< ::RTT::RootFolder, Pred>(impl, ::Udm::NULLCHILDROLE); };
		::Udm::ParentAttr< ::RTT::RootFolder> RootFolder_parent() const;
		::Udm::ParentAttr< ::RTT::RootFolder> parent() const;

		static ::Uml::Class meta;
		static ::Uml::Attribute meta_name;
		static ::Uml::CompositionChildRole meta_RTTRoot_children;
		static ::Uml::CompositionChildRole meta_RootFolder_children;
		static ::Uml::CompositionParentRole meta_RootFolder_parent;

	};

}

#endif // MOBIES_RTT_H
