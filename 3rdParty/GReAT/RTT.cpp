// cpp (meta datanetwork format) source file RTT.cpp
// generated from diagram RTT
// generated on Wed Jul 27 11:42:41 2016

#include "RTT.h"
#include <UmlExt.h>
#include <UdmStatic.h>

using namespace std;

namespace RTT {

	DataNetwork::DataNetwork() {}
	DataNetwork::DataNetwork(::Udm::ObjectImpl *impl) : MgaObject(impl) {}
	DataNetwork::DataNetwork(const DataNetwork &master) : MgaObject(master) {}

#ifdef UDM_RVALUE
	DataNetwork::DataNetwork(DataNetwork &&master) : MgaObject(master) {};

	DataNetwork DataNetwork::Cast(::Udm::Object &&a) { return __Cast(std::move(a), meta); };
	DataNetwork& DataNetwork::operator=(DataNetwork &&a) { ::Udm::Object::operator =(std::move(a)); return *this; };
	DataNetwork& DataNetwork::operator=(const DataNetwork &a) { ::Udm::Object::operator =(a); return *this; };

#endif
	DataNetwork DataNetwork::Cast(const ::Udm::Object &a) { return __Cast(a, meta); }
	DataNetwork DataNetwork::Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role); }
	DataNetwork DataNetwork::CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl); }
	::Udm::InstantiatedAttr< DataNetwork> DataNetwork::Instances() { return ::Udm::InstantiatedAttr< DataNetwork>(impl); }
	DataNetwork DataNetwork::CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl, true); }
	::Udm::DerivedAttr< DataNetwork> DataNetwork::Derived() { return ::Udm::DerivedAttr< DataNetwork>(impl); }
	::Udm::ArchetypeAttr< DataNetwork> DataNetwork::Archetype() const { return ::Udm::ArchetypeAttr< DataNetwork>(impl); }
	::Udm::StringAttr DataNetwork::filePath() const { return ::Udm::StringAttr(impl, meta_filePath); }
	::Udm::IntegerAttr DataNetwork::dataNetworkId() const { return ::Udm::IntegerAttr(impl, meta_dataNetworkId); }
	::Udm::ParentAttr< ::RTT::RTTRoot> DataNetwork::RTTRoot_parent() const { return ::Udm::ParentAttr< ::RTT::RTTRoot>(impl, meta_RTTRoot_parent); }
	::Udm::ParentAttr< ::RTT::RTTRoot> DataNetwork::parent() const { return ::Udm::ParentAttr< ::RTT::RTTRoot>(impl, ::Udm::NULLPARENTROLE); }

	::Uml::Class DataNetwork::meta;
	::Uml::Attribute DataNetwork::meta_filePath;
	::Uml::Attribute DataNetwork::meta_dataNetworkId;
	::Uml::CompositionParentRole DataNetwork::meta_RTTRoot_parent;

	MgaObject::MgaObject() {}
	MgaObject::MgaObject(::Udm::ObjectImpl *impl) : UDM_OBJECT(impl) {}
	MgaObject::MgaObject(const MgaObject &master) : UDM_OBJECT(master) {}

#ifdef UDM_RVALUE
	MgaObject::MgaObject(MgaObject &&master) : UDM_OBJECT(master) {};

	MgaObject MgaObject::Cast(::Udm::Object &&a) { return __Cast(std::move(a), meta); };
	MgaObject& MgaObject::operator=(MgaObject &&a) { ::Udm::Object::operator =(std::move(a)); return *this; };
	MgaObject& MgaObject::operator=(const MgaObject &a) { ::Udm::Object::operator =(a); return *this; };

#endif
	MgaObject MgaObject::Cast(const ::Udm::Object &a) { return __Cast(a, meta); }
	MgaObject MgaObject::Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role); }
	MgaObject MgaObject::CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl); }
	::Udm::InstantiatedAttr< MgaObject> MgaObject::Instances() { return ::Udm::InstantiatedAttr< MgaObject>(impl); }
	MgaObject MgaObject::CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl, true); }
	::Udm::DerivedAttr< MgaObject> MgaObject::Derived() { return ::Udm::DerivedAttr< MgaObject>(impl); }
	::Udm::ArchetypeAttr< MgaObject> MgaObject::Archetype() const { return ::Udm::ArchetypeAttr< MgaObject>(impl); }
	::Udm::StringAttr MgaObject::position() const { return ::Udm::StringAttr(impl, meta_position); }
	::Udm::StringAttr MgaObject::name() const { return ::Udm::StringAttr(impl, meta_name); }
	::Udm::ParentAttr< ::Udm::Object> MgaObject::parent() const { return ::Udm::ParentAttr< ::Udm::Object>(impl, ::Udm::NULLPARENTROLE); }

	::Uml::Class MgaObject::meta;
	::Uml::Attribute MgaObject::meta_position;
	::Uml::Attribute MgaObject::meta_name;

	RTTRoot::RTTRoot() {}
	RTTRoot::RTTRoot(::Udm::ObjectImpl *impl) : MgaObject(impl) {}
	RTTRoot::RTTRoot(const RTTRoot &master) : MgaObject(master) {}

#ifdef UDM_RVALUE
	RTTRoot::RTTRoot(RTTRoot &&master) : MgaObject(master) {};

	RTTRoot RTTRoot::Cast(::Udm::Object &&a) { return __Cast(std::move(a), meta); };
	RTTRoot& RTTRoot::operator=(RTTRoot &&a) { ::Udm::Object::operator =(std::move(a)); return *this; };
	RTTRoot& RTTRoot::operator=(const RTTRoot &a) { ::Udm::Object::operator =(a); return *this; };

#endif
	RTTRoot RTTRoot::Cast(const ::Udm::Object &a) { return __Cast(a, meta); }
	RTTRoot RTTRoot::Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role); }
	RTTRoot RTTRoot::CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl); }
	::Udm::InstantiatedAttr< RTTRoot> RTTRoot::Instances() { return ::Udm::InstantiatedAttr< RTTRoot>(impl); }
	RTTRoot RTTRoot::CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl, true); }
	::Udm::DerivedAttr< RTTRoot> RTTRoot::Derived() { return ::Udm::DerivedAttr< RTTRoot>(impl); }
	::Udm::ArchetypeAttr< RTTRoot> RTTRoot::Archetype() const { return ::Udm::ArchetypeAttr< RTTRoot>(impl); }
	::Udm::ChildrenAttr< ::RTT::DataNetwork> RTTRoot::DataNetwork_children() const { return ::Udm::ChildrenAttr< ::RTT::DataNetwork>(impl, meta_DataNetwork_children); }
	::Udm::ChildrenAttr< ::RTT::ExecuteRule> RTTRoot::ExecuteRule_children() const { return ::Udm::ChildrenAttr< ::RTT::ExecuteRule>(impl, meta_ExecuteRule_children); }
	::Udm::ChildrenAttr< ::RTT::Sequence> RTTRoot::Sequence_children() const { return ::Udm::ChildrenAttr< ::RTT::Sequence>(impl, meta_Sequence_children); }
	::Udm::ChildrenAttr< ::RTT::DataNetwork> RTTRoot::DataNetwork_kind_children() const { return ::Udm::ChildrenAttr< ::RTT::DataNetwork>(impl, ::Udm::NULLCHILDROLE); }
	::Udm::ChildrenAttr< ::RTT::MgaObject> RTTRoot::MgaObject_kind_children() const { return ::Udm::ChildrenAttr< ::RTT::MgaObject>(impl, ::Udm::NULLCHILDROLE); }
	::Udm::ChildrenAttr< ::RTT::ExecuteRule> RTTRoot::ExecuteRule_kind_children() const { return ::Udm::ChildrenAttr< ::RTT::ExecuteRule>(impl, ::Udm::NULLCHILDROLE); }
	::Udm::ChildrenAttr< ::RTT::Sequence> RTTRoot::Sequence_kind_children() const { return ::Udm::ChildrenAttr< ::RTT::Sequence>(impl, ::Udm::NULLCHILDROLE); }
	::Udm::ParentAttr< ::RTT::RootFolder> RTTRoot::RootFolder_parent() const { return ::Udm::ParentAttr< ::RTT::RootFolder>(impl, meta_RootFolder_parent); }
	::Udm::ParentAttr< ::RTT::RootFolder> RTTRoot::parent() const { return ::Udm::ParentAttr< ::RTT::RootFolder>(impl, ::Udm::NULLPARENTROLE); }

	::Uml::Class RTTRoot::meta;
	::Uml::CompositionChildRole RTTRoot::meta_DataNetwork_children;
	::Uml::CompositionChildRole RTTRoot::meta_ExecuteRule_children;
	::Uml::CompositionChildRole RTTRoot::meta_Sequence_children;
	::Uml::CompositionParentRole RTTRoot::meta_RootFolder_parent;

	ExecuteRule::ExecuteRule() {}
	ExecuteRule::ExecuteRule(::Udm::ObjectImpl *impl) : MgaObject(impl) {}
	ExecuteRule::ExecuteRule(const ExecuteRule &master) : MgaObject(master) {}

#ifdef UDM_RVALUE
	ExecuteRule::ExecuteRule(ExecuteRule &&master) : MgaObject(master) {};

	ExecuteRule ExecuteRule::Cast(::Udm::Object &&a) { return __Cast(std::move(a), meta); };
	ExecuteRule& ExecuteRule::operator=(ExecuteRule &&a) { ::Udm::Object::operator =(std::move(a)); return *this; };
	ExecuteRule& ExecuteRule::operator=(const ExecuteRule &a) { ::Udm::Object::operator =(a); return *this; };

#endif
	ExecuteRule ExecuteRule::Cast(const ::Udm::Object &a) { return __Cast(a, meta); }
	ExecuteRule ExecuteRule::Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role); }
	ExecuteRule ExecuteRule::CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl); }
	::Udm::InstantiatedAttr< ExecuteRule> ExecuteRule::Instances() { return ::Udm::InstantiatedAttr< ExecuteRule>(impl); }
	ExecuteRule ExecuteRule::CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl, true); }
	::Udm::DerivedAttr< ExecuteRule> ExecuteRule::Derived() { return ::Udm::DerivedAttr< ExecuteRule>(impl); }
	::Udm::ArchetypeAttr< ExecuteRule> ExecuteRule::Archetype() const { return ::Udm::ArchetypeAttr< ExecuteRule>(impl); }
	::Udm::IntegerAttr ExecuteRule::uniqueObjId() const { return ::Udm::IntegerAttr(impl, meta_uniqueObjId); }
	::Udm::AClassAssocAttr< Sequence, ExecuteRule> ExecuteRule::dstSequence() const { return ::Udm::AClassAssocAttr< Sequence, ExecuteRule>(impl, meta_dstSequence, meta_dstSequence_rev); }
	::Udm::AClassAssocAttr< Sequence, ExecuteRule> ExecuteRule::srcSequence() const { return ::Udm::AClassAssocAttr< Sequence, ExecuteRule>(impl, meta_srcSequence, meta_srcSequence_rev); }
	::Udm::ChildrenAttr< ::RTT::UdmObject> ExecuteRule::UdmObject_children() const { return ::Udm::ChildrenAttr< ::RTT::UdmObject>(impl, meta_UdmObject_children); }
	::Udm::ChildrenAttr< ::RTT::Group> ExecuteRule::Group_children() const { return ::Udm::ChildrenAttr< ::RTT::Group>(impl, meta_Group_children); }
	::Udm::ChildrenAttr< ::RTT::TracesTo> ExecuteRule::TracesTo_children() const { return ::Udm::ChildrenAttr< ::RTT::TracesTo>(impl, meta_TracesTo_children); }
	::Udm::ChildrenAttr< ::RTT::MgaObject> ExecuteRule::MgaObject_kind_children() const { return ::Udm::ChildrenAttr< ::RTT::MgaObject>(impl, ::Udm::NULLCHILDROLE); }
	::Udm::ChildrenAttr< ::RTT::Group> ExecuteRule::Group_kind_children() const { return ::Udm::ChildrenAttr< ::RTT::Group>(impl, ::Udm::NULLCHILDROLE); }
	::Udm::ChildrenAttr< ::RTT::UdmObject> ExecuteRule::UdmObject_kind_children() const { return ::Udm::ChildrenAttr< ::RTT::UdmObject>(impl, ::Udm::NULLCHILDROLE); }
	::Udm::ChildrenAttr< ::RTT::TracesTo> ExecuteRule::TracesTo_kind_children() const { return ::Udm::ChildrenAttr< ::RTT::TracesTo>(impl, ::Udm::NULLCHILDROLE); }
	::Udm::ParentAttr< ::RTT::RTTRoot> ExecuteRule::RTTRoot_parent() const { return ::Udm::ParentAttr< ::RTT::RTTRoot>(impl, meta_RTTRoot_parent); }
	::Udm::ParentAttr< ::RTT::RTTRoot> ExecuteRule::parent() const { return ::Udm::ParentAttr< ::RTT::RTTRoot>(impl, ::Udm::NULLPARENTROLE); }

	::Uml::Class ExecuteRule::meta;
	::Uml::Attribute ExecuteRule::meta_uniqueObjId;
	::Uml::AssociationRole ExecuteRule::meta_dstSequence;
	::Uml::AssociationRole ExecuteRule::meta_dstSequence_rev;
	::Uml::AssociationRole ExecuteRule::meta_srcSequence;
	::Uml::AssociationRole ExecuteRule::meta_srcSequence_rev;
	::Uml::CompositionChildRole ExecuteRule::meta_UdmObject_children;
	::Uml::CompositionChildRole ExecuteRule::meta_Group_children;
	::Uml::CompositionChildRole ExecuteRule::meta_TracesTo_children;
	::Uml::CompositionParentRole ExecuteRule::meta_RTTRoot_parent;

	Sequence::Sequence() {}
	Sequence::Sequence(::Udm::ObjectImpl *impl) : MgaObject(impl) {}
	Sequence::Sequence(const Sequence &master) : MgaObject(master) {}

#ifdef UDM_RVALUE
	Sequence::Sequence(Sequence &&master) : MgaObject(master) {};

	Sequence Sequence::Cast(::Udm::Object &&a) { return __Cast(std::move(a), meta); };
	Sequence& Sequence::operator=(Sequence &&a) { ::Udm::Object::operator =(std::move(a)); return *this; };
	Sequence& Sequence::operator=(const Sequence &a) { ::Udm::Object::operator =(a); return *this; };

#endif
	Sequence Sequence::Cast(const ::Udm::Object &a) { return __Cast(a, meta); }
	Sequence Sequence::Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role); }
	Sequence Sequence::CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl); }
	::Udm::InstantiatedAttr< Sequence> Sequence::Instances() { return ::Udm::InstantiatedAttr< Sequence>(impl); }
	Sequence Sequence::CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl, true); }
	::Udm::DerivedAttr< Sequence> Sequence::Derived() { return ::Udm::DerivedAttr< Sequence>(impl); }
	::Udm::ArchetypeAttr< Sequence> Sequence::Archetype() const { return ::Udm::ArchetypeAttr< Sequence>(impl); }
	::Udm::ParentAttr< ::RTT::RTTRoot> Sequence::RTTRoot_parent() const { return ::Udm::ParentAttr< ::RTT::RTTRoot>(impl, meta_RTTRoot_parent); }
	::Udm::ParentAttr< ::RTT::RTTRoot> Sequence::parent() const { return ::Udm::ParentAttr< ::RTT::RTTRoot>(impl, ::Udm::NULLPARENTROLE); }
	::Udm::AssocEndAttr< ::RTT::ExecuteRule> Sequence::srcSequence_end() const { return ::Udm::AssocEndAttr< ::RTT::ExecuteRule>(impl, meta_srcSequence_end_); }
	::Udm::AssocEndAttr< ::RTT::ExecuteRule> Sequence::dstSequence_end() const { return ::Udm::AssocEndAttr< ::RTT::ExecuteRule>(impl, meta_dstSequence_end_); }

	::Uml::Class Sequence::meta;
	::Uml::CompositionParentRole Sequence::meta_RTTRoot_parent;
	::Uml::AssociationRole Sequence::meta_srcSequence_end_;
	::Uml::AssociationRole Sequence::meta_dstSequence_end_;

	Group::Group() {}
	Group::Group(::Udm::ObjectImpl *impl) : MgaObject(impl) {}
	Group::Group(const Group &master) : MgaObject(master) {}

#ifdef UDM_RVALUE
	Group::Group(Group &&master) : MgaObject(master) {};

	Group Group::Cast(::Udm::Object &&a) { return __Cast(std::move(a), meta); };
	Group& Group::operator=(Group &&a) { ::Udm::Object::operator =(std::move(a)); return *this; };
	Group& Group::operator=(const Group &a) { ::Udm::Object::operator =(a); return *this; };

#endif
	Group Group::Cast(const ::Udm::Object &a) { return __Cast(a, meta); }
	Group Group::Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role); }
	Group Group::CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl); }
	::Udm::InstantiatedAttr< Group> Group::Instances() { return ::Udm::InstantiatedAttr< Group>(impl); }
	Group Group::CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl, true); }
	::Udm::DerivedAttr< Group> Group::Derived() { return ::Udm::DerivedAttr< Group>(impl); }
	::Udm::ArchetypeAttr< Group> Group::Archetype() const { return ::Udm::ArchetypeAttr< Group>(impl); }
	::Udm::AssocAttr< UdmObject> Group::members() const { return ::Udm::AssocAttr< UdmObject>(impl, meta_members); }
	::Udm::ParentAttr< ::RTT::ExecuteRule> Group::ExecuteRule_parent() const { return ::Udm::ParentAttr< ::RTT::ExecuteRule>(impl, meta_ExecuteRule_parent); }
	::Udm::ParentAttr< ::RTT::ExecuteRule> Group::parent() const { return ::Udm::ParentAttr< ::RTT::ExecuteRule>(impl, ::Udm::NULLPARENTROLE); }

	::Uml::Class Group::meta;
	::Uml::AssociationRole Group::meta_members;
	::Uml::CompositionParentRole Group::meta_ExecuteRule_parent;

	UdmObject::UdmObject() {}
	UdmObject::UdmObject(::Udm::ObjectImpl *impl) : MgaObject(impl) {}
	UdmObject::UdmObject(const UdmObject &master) : MgaObject(master) {}

#ifdef UDM_RVALUE
	UdmObject::UdmObject(UdmObject &&master) : MgaObject(master) {};

	UdmObject UdmObject::Cast(::Udm::Object &&a) { return __Cast(std::move(a), meta); };
	UdmObject& UdmObject::operator=(UdmObject &&a) { ::Udm::Object::operator =(std::move(a)); return *this; };
	UdmObject& UdmObject::operator=(const UdmObject &a) { ::Udm::Object::operator =(a); return *this; };

#endif
	UdmObject UdmObject::Cast(const ::Udm::Object &a) { return __Cast(a, meta); }
	UdmObject UdmObject::Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role); }
	UdmObject UdmObject::CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl); }
	::Udm::InstantiatedAttr< UdmObject> UdmObject::Instances() { return ::Udm::InstantiatedAttr< UdmObject>(impl); }
	UdmObject UdmObject::CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl, true); }
	::Udm::DerivedAttr< UdmObject> UdmObject::Derived() { return ::Udm::DerivedAttr< UdmObject>(impl); }
	::Udm::ArchetypeAttr< UdmObject> UdmObject::Archetype() const { return ::Udm::ArchetypeAttr< UdmObject>(impl); }
	::Udm::IntegerAttr UdmObject::dataNetworkId() const { return ::Udm::IntegerAttr(impl, meta_dataNetworkId); }
	::Udm::IntegerAttr UdmObject::uniqueObjId() const { return ::Udm::IntegerAttr(impl, meta_uniqueObjId); }
	::Udm::AClassAssocAttr< TracesTo, UdmObject> UdmObject::dstTracesTo() const { return ::Udm::AClassAssocAttr< TracesTo, UdmObject>(impl, meta_dstTracesTo, meta_dstTracesTo_rev); }
	::Udm::AClassAssocAttr< TracesTo, UdmObject> UdmObject::srcTracesTo() const { return ::Udm::AClassAssocAttr< TracesTo, UdmObject>(impl, meta_srcTracesTo, meta_srcTracesTo_rev); }
	::Udm::AssocAttr< Group> UdmObject::setGroup() const { return ::Udm::AssocAttr< Group>(impl, meta_setGroup); }
	::Udm::ParentAttr< ::RTT::ExecuteRule> UdmObject::ExecuteRule_parent() const { return ::Udm::ParentAttr< ::RTT::ExecuteRule>(impl, meta_ExecuteRule_parent); }
	::Udm::ParentAttr< ::RTT::ExecuteRule> UdmObject::parent() const { return ::Udm::ParentAttr< ::RTT::ExecuteRule>(impl, ::Udm::NULLPARENTROLE); }

	::Uml::Class UdmObject::meta;
	::Uml::Attribute UdmObject::meta_dataNetworkId;
	::Uml::Attribute UdmObject::meta_uniqueObjId;
	::Uml::AssociationRole UdmObject::meta_dstTracesTo;
	::Uml::AssociationRole UdmObject::meta_dstTracesTo_rev;
	::Uml::AssociationRole UdmObject::meta_srcTracesTo;
	::Uml::AssociationRole UdmObject::meta_srcTracesTo_rev;
	::Uml::AssociationRole UdmObject::meta_setGroup;
	::Uml::CompositionParentRole UdmObject::meta_ExecuteRule_parent;

	TracesTo::TracesTo() {}
	TracesTo::TracesTo(::Udm::ObjectImpl *impl) : MgaObject(impl) {}
	TracesTo::TracesTo(const TracesTo &master) : MgaObject(master) {}

#ifdef UDM_RVALUE
	TracesTo::TracesTo(TracesTo &&master) : MgaObject(master) {};

	TracesTo TracesTo::Cast(::Udm::Object &&a) { return __Cast(std::move(a), meta); };
	TracesTo& TracesTo::operator=(TracesTo &&a) { ::Udm::Object::operator =(std::move(a)); return *this; };
	TracesTo& TracesTo::operator=(const TracesTo &a) { ::Udm::Object::operator =(a); return *this; };

#endif
	TracesTo TracesTo::Cast(const ::Udm::Object &a) { return __Cast(a, meta); }
	TracesTo TracesTo::Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role); }
	TracesTo TracesTo::CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl); }
	::Udm::InstantiatedAttr< TracesTo> TracesTo::Instances() { return ::Udm::InstantiatedAttr< TracesTo>(impl); }
	TracesTo TracesTo::CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl, true); }
	::Udm::DerivedAttr< TracesTo> TracesTo::Derived() { return ::Udm::DerivedAttr< TracesTo>(impl); }
	::Udm::ArchetypeAttr< TracesTo> TracesTo::Archetype() const { return ::Udm::ArchetypeAttr< TracesTo>(impl); }
	::Udm::ParentAttr< ::RTT::ExecuteRule> TracesTo::ExecuteRule_parent() const { return ::Udm::ParentAttr< ::RTT::ExecuteRule>(impl, meta_ExecuteRule_parent); }
	::Udm::ParentAttr< ::RTT::ExecuteRule> TracesTo::parent() const { return ::Udm::ParentAttr< ::RTT::ExecuteRule>(impl, ::Udm::NULLPARENTROLE); }
	::Udm::AssocEndAttr< ::RTT::UdmObject> TracesTo::srcTracesTo_end() const { return ::Udm::AssocEndAttr< ::RTT::UdmObject>(impl, meta_srcTracesTo_end_); }
	::Udm::AssocEndAttr< ::RTT::UdmObject> TracesTo::dstTracesTo_end() const { return ::Udm::AssocEndAttr< ::RTT::UdmObject>(impl, meta_dstTracesTo_end_); }

	::Uml::Class TracesTo::meta;
	::Uml::CompositionParentRole TracesTo::meta_ExecuteRule_parent;
	::Uml::AssociationRole TracesTo::meta_srcTracesTo_end_;
	::Uml::AssociationRole TracesTo::meta_dstTracesTo_end_;

	RootFolder::RootFolder() {}
	RootFolder::RootFolder(::Udm::ObjectImpl *impl) : UDM_OBJECT(impl) {}
	RootFolder::RootFolder(const RootFolder &master) : UDM_OBJECT(master) {}

#ifdef UDM_RVALUE
	RootFolder::RootFolder(RootFolder &&master) : UDM_OBJECT(master) {};

	RootFolder RootFolder::Cast(::Udm::Object &&a) { return __Cast(std::move(a), meta); };
	RootFolder& RootFolder::operator=(RootFolder &&a) { ::Udm::Object::operator =(std::move(a)); return *this; };
	RootFolder& RootFolder::operator=(const RootFolder &a) { ::Udm::Object::operator =(a); return *this; };

#endif
	RootFolder RootFolder::Cast(const ::Udm::Object &a) { return __Cast(a, meta); }
	RootFolder RootFolder::Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role); }
	RootFolder RootFolder::CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl); }
	::Udm::InstantiatedAttr< RootFolder> RootFolder::Instances() { return ::Udm::InstantiatedAttr< RootFolder>(impl); }
	RootFolder RootFolder::CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl, true); }
	::Udm::DerivedAttr< RootFolder> RootFolder::Derived() { return ::Udm::DerivedAttr< RootFolder>(impl); }
	::Udm::ArchetypeAttr< RootFolder> RootFolder::Archetype() const { return ::Udm::ArchetypeAttr< RootFolder>(impl); }
	::Udm::StringAttr RootFolder::name() const { return ::Udm::StringAttr(impl, meta_name); }
	::Udm::ChildrenAttr< ::RTT::RTTRoot> RootFolder::RTTRoot_children() const { return ::Udm::ChildrenAttr< ::RTT::RTTRoot>(impl, meta_RTTRoot_children); }
	::Udm::ChildrenAttr< ::RTT::RootFolder> RootFolder::RootFolder_children() const { return ::Udm::ChildrenAttr< ::RTT::RootFolder>(impl, meta_RootFolder_children); }
	::Udm::ChildrenAttr< ::RTT::MgaObject> RootFolder::MgaObject_kind_children() const { return ::Udm::ChildrenAttr< ::RTT::MgaObject>(impl, ::Udm::NULLCHILDROLE); }
	::Udm::ChildrenAttr< ::RTT::RTTRoot> RootFolder::RTTRoot_kind_children() const { return ::Udm::ChildrenAttr< ::RTT::RTTRoot>(impl, ::Udm::NULLCHILDROLE); }
	::Udm::ChildrenAttr< ::RTT::RootFolder> RootFolder::RootFolder_kind_children() const { return ::Udm::ChildrenAttr< ::RTT::RootFolder>(impl, ::Udm::NULLCHILDROLE); }
	::Udm::ParentAttr< ::RTT::RootFolder> RootFolder::RootFolder_parent() const { return ::Udm::ParentAttr< ::RTT::RootFolder>(impl, meta_RootFolder_parent); }
	::Udm::ParentAttr< ::RTT::RootFolder> RootFolder::parent() const { return ::Udm::ParentAttr< ::RTT::RootFolder>(impl, ::Udm::NULLPARENTROLE); }

	::Uml::Class RootFolder::meta;
	::Uml::Attribute RootFolder::meta_name;
	::Uml::CompositionChildRole RootFolder::meta_RTTRoot_children;
	::Uml::CompositionChildRole RootFolder::meta_RootFolder_children;
	::Uml::CompositionParentRole RootFolder::meta_RootFolder_parent;

	::Uml::Diagram meta;

	void CreateMeta() {
		// classes, with attributes, constraints and constraint definitions
		DataNetwork::meta = ::Uml::Class::Create(meta);
		DataNetwork::meta_filePath = ::Uml::Attribute::Create(DataNetwork::meta);
		DataNetwork::meta_dataNetworkId = ::Uml::Attribute::Create(DataNetwork::meta);

		ExecuteRule::meta = ::Uml::Class::Create(meta);
		ExecuteRule::meta_uniqueObjId = ::Uml::Attribute::Create(ExecuteRule::meta);

		Group::meta = ::Uml::Class::Create(meta);

		MgaObject::meta = ::Uml::Class::Create(meta);
		MgaObject::meta_position = ::Uml::Attribute::Create(MgaObject::meta);
		MgaObject::meta_name = ::Uml::Attribute::Create(MgaObject::meta);

		RTTRoot::meta = ::Uml::Class::Create(meta);

		RootFolder::meta = ::Uml::Class::Create(meta);
		RootFolder::meta_name = ::Uml::Attribute::Create(RootFolder::meta);

		Sequence::meta = ::Uml::Class::Create(meta);

		TracesTo::meta = ::Uml::Class::Create(meta);

		UdmObject::meta = ::Uml::Class::Create(meta);
		UdmObject::meta_dataNetworkId = ::Uml::Attribute::Create(UdmObject::meta);
		UdmObject::meta_uniqueObjId = ::Uml::Attribute::Create(UdmObject::meta);

	}

	void InitMeta() {
		// classes, with attributes, constraints and constraint definitions
		::Uml::InitClassProps(DataNetwork::meta, "DataNetwork", false, "Atom", NULL);
		::Uml::InitAttributeProps(DataNetwork::meta_filePath, "filePath", "String", false, false, 1, 1, false, "public", vector<string>());
		::Uml::InitAttributeProps(DataNetwork::meta_dataNetworkId, "dataNetworkId", "Integer", false, false, 1, 1, false, "public", vector<string>());

		::Uml::InitClassProps(ExecuteRule::meta, "ExecuteRule", false, "Model", NULL);
		::Uml::InitAttributeProps(ExecuteRule::meta_uniqueObjId, "uniqueObjId", "Integer", false, false, 1, 1, false, "public", vector<string>());

		::Uml::InitClassProps(Group::meta, "Group", false, "Set", NULL);

		::Uml::InitClassProps(MgaObject::meta, "MgaObject", true, NULL, NULL);
		::Uml::InitAttributeProps(MgaObject::meta_position, "position", "String", false, false, 0, 1, false, "public", vector<string>());
		::Uml::InitAttributeProps(MgaObject::meta_name, "name", "String", false, false, 0, 1, false, "public", vector<string>());

		::Uml::InitClassProps(RTTRoot::meta, "RTTRoot", false, "Model", NULL);

		::Uml::InitClassProps(RootFolder::meta, "RootFolder", false, "Folder", NULL);
		::Uml::InitAttributeProps(RootFolder::meta_name, "name", "String", false, false, 0, 1, false, "public", vector<string>());

		::Uml::InitClassProps(Sequence::meta, "Sequence", false, "Connection", NULL);

		::Uml::InitClassProps(TracesTo::meta, "TracesTo", false, "Connection", NULL);

		::Uml::InitClassProps(UdmObject::meta, "UdmObject", false, "Atom", NULL);
		::Uml::InitAttributeProps(UdmObject::meta_dataNetworkId, "dataNetworkId", "Integer", false, false, 1, 1, false, "public", vector<string>());
		::Uml::InitAttributeProps(UdmObject::meta_uniqueObjId, "uniqueObjId", "Integer", false, false, 1, 1, false, "public", vector<string>());

		// associations
		{
			::Uml::Association ass = ::Uml::Association::Create(meta);
			::Uml::InitAssociationProps(ass, "Sequence");
			ExecuteRule::meta_dstSequence = ::Uml::AssociationRole::Create(ass);
			::Uml::InitAssociationRoleProps(ExecuteRule::meta_dstSequence, "dstSequence", true, false, 0, -1);
			ExecuteRule::meta_srcSequence = ::Uml::AssociationRole::Create(ass);
			::Uml::InitAssociationRoleProps(ExecuteRule::meta_srcSequence, "srcSequence", true, false, 0, -1);

		}
		{
			::Uml::Association ass = ::Uml::Association::Create(meta);
			::Uml::InitAssociationProps(ass, "TracesTo");
			UdmObject::meta_dstTracesTo = ::Uml::AssociationRole::Create(ass);
			::Uml::InitAssociationRoleProps(UdmObject::meta_dstTracesTo, "dstTracesTo", true, false, 0, -1);
			UdmObject::meta_srcTracesTo = ::Uml::AssociationRole::Create(ass);
			::Uml::InitAssociationRoleProps(UdmObject::meta_srcTracesTo, "srcTracesTo", true, false, 0, -1);

		}
		{
			::Uml::Association ass = ::Uml::Association::Create(meta);
			::Uml::InitAssociationProps(ass, "");
			UdmObject::meta_setGroup = ::Uml::AssociationRole::Create(ass);
			::Uml::InitAssociationRoleProps(UdmObject::meta_setGroup, "setGroup", true, false, 0, -1);
			Group::meta_members = ::Uml::AssociationRole::Create(ass);
			::Uml::InitAssociationRoleProps(Group::meta_members, "members", true, false, 0, -1);

		}

		// compositions
		{
			::Uml::Composition comp = ::Uml::Composition::Create(meta);
			::Uml::InitCompositionProps(comp, "", false);
			DataNetwork::meta_RTTRoot_parent = ::Uml::CompositionParentRole::Create(comp);
			::Uml::InitCompositionParentRoleProps(DataNetwork::meta_RTTRoot_parent, "RTTRoot_parent", true);
			RTTRoot::meta_DataNetwork_children = ::Uml::CompositionChildRole::Create(comp);
			::Uml::InitCompositionChildRoleProps(RTTRoot::meta_DataNetwork_children, "DataNetwork", true, 0, -1);

		}
		{
			::Uml::Composition comp = ::Uml::Composition::Create(meta);
			::Uml::InitCompositionProps(comp, "", false);
			ExecuteRule::meta_RTTRoot_parent = ::Uml::CompositionParentRole::Create(comp);
			::Uml::InitCompositionParentRoleProps(ExecuteRule::meta_RTTRoot_parent, "RTTRoot_parent", true);
			RTTRoot::meta_ExecuteRule_children = ::Uml::CompositionChildRole::Create(comp);
			::Uml::InitCompositionChildRoleProps(RTTRoot::meta_ExecuteRule_children, "ExecuteRule", true, 0, -1);

		}
		{
			::Uml::Composition comp = ::Uml::Composition::Create(meta);
			::Uml::InitCompositionProps(comp, "", false);
			Sequence::meta_RTTRoot_parent = ::Uml::CompositionParentRole::Create(comp);
			::Uml::InitCompositionParentRoleProps(Sequence::meta_RTTRoot_parent, "RTTRoot_parent", true);
			RTTRoot::meta_Sequence_children = ::Uml::CompositionChildRole::Create(comp);
			::Uml::InitCompositionChildRoleProps(RTTRoot::meta_Sequence_children, "Sequence", true, 0, -1);

		}
		{
			::Uml::Composition comp = ::Uml::Composition::Create(meta);
			::Uml::InitCompositionProps(comp, "", false);
			UdmObject::meta_ExecuteRule_parent = ::Uml::CompositionParentRole::Create(comp);
			::Uml::InitCompositionParentRoleProps(UdmObject::meta_ExecuteRule_parent, "ExecuteRule_parent", true);
			ExecuteRule::meta_UdmObject_children = ::Uml::CompositionChildRole::Create(comp);
			::Uml::InitCompositionChildRoleProps(ExecuteRule::meta_UdmObject_children, "UdmObject", true, 0, -1);

		}
		{
			::Uml::Composition comp = ::Uml::Composition::Create(meta);
			::Uml::InitCompositionProps(comp, "", false);
			Group::meta_ExecuteRule_parent = ::Uml::CompositionParentRole::Create(comp);
			::Uml::InitCompositionParentRoleProps(Group::meta_ExecuteRule_parent, "ExecuteRule_parent", true);
			ExecuteRule::meta_Group_children = ::Uml::CompositionChildRole::Create(comp);
			::Uml::InitCompositionChildRoleProps(ExecuteRule::meta_Group_children, "Group", true, 0, -1);

		}
		{
			::Uml::Composition comp = ::Uml::Composition::Create(meta);
			::Uml::InitCompositionProps(comp, "", false);
			TracesTo::meta_ExecuteRule_parent = ::Uml::CompositionParentRole::Create(comp);
			::Uml::InitCompositionParentRoleProps(TracesTo::meta_ExecuteRule_parent, "ExecuteRule_parent", true);
			ExecuteRule::meta_TracesTo_children = ::Uml::CompositionChildRole::Create(comp);
			::Uml::InitCompositionChildRoleProps(ExecuteRule::meta_TracesTo_children, "TracesTo", true, 0, -1);

		}
		{
			::Uml::Composition comp = ::Uml::Composition::Create(meta);
			::Uml::InitCompositionProps(comp, "", false);
			RTTRoot::meta_RootFolder_parent = ::Uml::CompositionParentRole::Create(comp);
			::Uml::InitCompositionParentRoleProps(RTTRoot::meta_RootFolder_parent, "RootFolder_parent", true);
			RootFolder::meta_RTTRoot_children = ::Uml::CompositionChildRole::Create(comp);
			::Uml::InitCompositionChildRoleProps(RootFolder::meta_RTTRoot_children, "RTTRoot", true, 0, -1);

		}
		{
			::Uml::Composition comp = ::Uml::Composition::Create(meta);
			::Uml::InitCompositionProps(comp, "", false);
			RootFolder::meta_RootFolder_parent = ::Uml::CompositionParentRole::Create(comp);
			::Uml::InitCompositionParentRoleProps(RootFolder::meta_RootFolder_parent, "RootFolder_parent", true);
			RootFolder::meta_RootFolder_children = ::Uml::CompositionChildRole::Create(comp);
			::Uml::InitCompositionChildRoleProps(RootFolder::meta_RootFolder_children, "RootFolder", true, 0, -1);

		}

	}

	void InitMetaLinks() {
		RTTRoot::meta_DataNetwork_children.target() = DataNetwork::meta;

		ExecuteRule::meta_dstSequence.target() = ExecuteRule::meta;
		ExecuteRule::meta_srcSequence.target() = ExecuteRule::meta;
		UdmObject::meta_ExecuteRule_parent.target() = ExecuteRule::meta;
		Group::meta_ExecuteRule_parent.target() = ExecuteRule::meta;
		TracesTo::meta_ExecuteRule_parent.target() = ExecuteRule::meta;
		RTTRoot::meta_ExecuteRule_children.target() = ExecuteRule::meta;

		Group::meta_members.target() = UdmObject::meta;
		ExecuteRule::meta_Group_children.target() = Group::meta;

		MgaObject::meta.subTypes() += DataNetwork::meta;
		MgaObject::meta.subTypes() += RTTRoot::meta;
		MgaObject::meta.subTypes() += ExecuteRule::meta;
		MgaObject::meta.subTypes() += Sequence::meta;
		MgaObject::meta.subTypes() += Group::meta;
		MgaObject::meta.subTypes() += UdmObject::meta;
		MgaObject::meta.subTypes() += TracesTo::meta;

		DataNetwork::meta_RTTRoot_parent.target() = RTTRoot::meta;
		ExecuteRule::meta_RTTRoot_parent.target() = RTTRoot::meta;
		Sequence::meta_RTTRoot_parent.target() = RTTRoot::meta;
		RootFolder::meta_RTTRoot_children.target() = RTTRoot::meta;

		RTTRoot::meta_RootFolder_parent.target() = RootFolder::meta;
		RootFolder::meta_RootFolder_parent.target() = RootFolder::meta;
		RootFolder::meta_RootFolder_children.target() = RootFolder::meta;

		RTTRoot::meta_Sequence_children.target() = Sequence::meta;
		Sequence::meta.association() = ExecuteRule::meta_dstSequence.parent();
		Sequence::meta_dstSequence_end_ = ExecuteRule::meta_srcSequence_rev = ExecuteRule::meta_dstSequence;
		Sequence::meta_srcSequence_end_ = ExecuteRule::meta_dstSequence_rev = ExecuteRule::meta_srcSequence;

		ExecuteRule::meta_TracesTo_children.target() = TracesTo::meta;
		TracesTo::meta.association() = UdmObject::meta_dstTracesTo.parent();
		TracesTo::meta_dstTracesTo_end_ = UdmObject::meta_srcTracesTo_rev = UdmObject::meta_dstTracesTo;
		TracesTo::meta_srcTracesTo_end_ = UdmObject::meta_dstTracesTo_rev = UdmObject::meta_srcTracesTo;

		UdmObject::meta_dstTracesTo.target() = UdmObject::meta;
		UdmObject::meta_srcTracesTo.target() = UdmObject::meta;
		UdmObject::meta_setGroup.target() = Group::meta;
		ExecuteRule::meta_UdmObject_children.target() = UdmObject::meta;

	}

	void InitMeta(const ::Uml::Diagram &parent) {
		// classes, with attributes, constraints and constraint definitions
		::Uml::SetClass(DataNetwork::meta, parent, "DataNetwork");
		::Uml::SetAttribute(DataNetwork::meta_filePath, DataNetwork::meta, "filePath");
		::Uml::SetAttribute(DataNetwork::meta_dataNetworkId, DataNetwork::meta, "dataNetworkId");

		::Uml::SetClass(ExecuteRule::meta, parent, "ExecuteRule");
		::Uml::SetAttribute(ExecuteRule::meta_uniqueObjId, ExecuteRule::meta, "uniqueObjId");

		::Uml::SetClass(Group::meta, parent, "Group");

		::Uml::SetClass(MgaObject::meta, parent, "MgaObject");
		::Uml::SetAttribute(MgaObject::meta_position, MgaObject::meta, "position");
		::Uml::SetAttribute(MgaObject::meta_name, MgaObject::meta, "name");

		::Uml::SetClass(RTTRoot::meta, parent, "RTTRoot");

		::Uml::SetClass(RootFolder::meta, parent, "RootFolder");
		::Uml::SetAttribute(RootFolder::meta_name, RootFolder::meta, "name");

		::Uml::SetClass(Sequence::meta, parent, "Sequence");

		::Uml::SetClass(TracesTo::meta, parent, "TracesTo");

		::Uml::SetClass(UdmObject::meta, parent, "UdmObject");
		::Uml::SetAttribute(UdmObject::meta_dataNetworkId, UdmObject::meta, "dataNetworkId");
		::Uml::SetAttribute(UdmObject::meta_uniqueObjId, UdmObject::meta, "uniqueObjId");

	}

	void InitMetaLinks(const ::Uml::Diagram &parent) {
		// classes
		::Uml::SetParentRole(DataNetwork::meta_RTTRoot_parent, DataNetwork::meta, RTTRoot::meta, "", "");

		::Uml::SetAssocRole(ExecuteRule::meta_dstSequence, ExecuteRule::meta, ExecuteRule::meta, "srcSequence");
		Sequence::meta_dstSequence_end_ = ExecuteRule::meta_srcSequence_rev = ExecuteRule::meta_dstSequence;
		::Uml::SetAssocRole(ExecuteRule::meta_srcSequence, ExecuteRule::meta, ExecuteRule::meta, "dstSequence");
		Sequence::meta_srcSequence_end_ = ExecuteRule::meta_dstSequence_rev = ExecuteRule::meta_srcSequence;
		::Uml::SetChildRole(ExecuteRule::meta_UdmObject_children, ExecuteRule::meta, UdmObject::meta, "", "");
		::Uml::SetChildRole(ExecuteRule::meta_Group_children, ExecuteRule::meta, Group::meta, "", "");
		::Uml::SetChildRole(ExecuteRule::meta_TracesTo_children, ExecuteRule::meta, TracesTo::meta, "", "");
		::Uml::SetParentRole(ExecuteRule::meta_RTTRoot_parent, ExecuteRule::meta, RTTRoot::meta, "", "");

		::Uml::SetAssocRole(Group::meta_members, Group::meta, UdmObject::meta, "setGroup");
		::Uml::SetParentRole(Group::meta_ExecuteRule_parent, Group::meta, ExecuteRule::meta, "", "");

		::Uml::SetChildRole(RTTRoot::meta_DataNetwork_children, RTTRoot::meta, DataNetwork::meta, "", "");
		::Uml::SetChildRole(RTTRoot::meta_ExecuteRule_children, RTTRoot::meta, ExecuteRule::meta, "", "");
		::Uml::SetChildRole(RTTRoot::meta_Sequence_children, RTTRoot::meta, Sequence::meta, "", "");
		::Uml::SetParentRole(RTTRoot::meta_RootFolder_parent, RTTRoot::meta, RootFolder::meta, "", "");

		::Uml::SetChildRole(RootFolder::meta_RTTRoot_children, RootFolder::meta, RTTRoot::meta, "", "");
		::Uml::SetChildRole(RootFolder::meta_RootFolder_children, RootFolder::meta, RootFolder::meta, "", "");
		::Uml::SetParentRole(RootFolder::meta_RootFolder_parent, RootFolder::meta, RootFolder::meta, "", "");

		::Uml::SetParentRole(Sequence::meta_RTTRoot_parent, Sequence::meta, RTTRoot::meta, "", "");

		::Uml::SetParentRole(TracesTo::meta_ExecuteRule_parent, TracesTo::meta, ExecuteRule::meta, "", "");

		::Uml::SetAssocRole(UdmObject::meta_dstTracesTo, UdmObject::meta, UdmObject::meta, "srcTracesTo");
		TracesTo::meta_dstTracesTo_end_ = UdmObject::meta_srcTracesTo_rev = UdmObject::meta_dstTracesTo;
		::Uml::SetAssocRole(UdmObject::meta_srcTracesTo, UdmObject::meta, UdmObject::meta, "dstTracesTo");
		TracesTo::meta_srcTracesTo_end_ = UdmObject::meta_dstTracesTo_rev = UdmObject::meta_srcTracesTo;
		::Uml::SetAssocRole(UdmObject::meta_setGroup, UdmObject::meta, Group::meta, "members");
		::Uml::SetParentRole(UdmObject::meta_ExecuteRule_parent, UdmObject::meta, ExecuteRule::meta, "", "");

	}

	void Initialize()
	{
		static bool first = true;
		if (!first) return;
		first = false;
		::Uml::Initialize();

	
		UDM_ASSERT( meta == ::Udm::null );

		::UdmStatic::StaticDataNetwork * meta_dn = new ::UdmStatic::StaticDataNetwork(::Uml::diagram);
		meta_dn->CreateNew("RTT.mem", "", ::Uml::Diagram::meta, ::Udm::CHANGES_LOST_DEFAULT);
		meta = ::Uml::Diagram::Cast(meta_dn->GetRootObject());

		::Uml::InitDiagramProps(meta, "RTT", "1.00");


		CreateMeta();
		InitMeta();
		InitMetaLinks();

	}

	void Initialize(const ::Uml::Diagram &dgr)
	{
		
		if (meta == dgr) return;
		meta = dgr;

		InitMeta(dgr);
		InitMetaLinks(dgr);

		
		
	}


	 ::Udm::UdmDiagram diagram = { &meta, Initialize };
	static struct _regClass
	{
		_regClass()
		{
			::Udm::MetaDepository::StoreDiagram("RTT", diagram);
		}
		~_regClass()
		{
			::Udm::MetaDepository::RemoveDiagram("RTT");
		}
	} __regUnUsed;

}

