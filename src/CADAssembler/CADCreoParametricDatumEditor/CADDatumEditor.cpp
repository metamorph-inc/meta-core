// cpp (meta datanetwork format) source file CADDatumEditor.cpp
// generated from diagram CADDatumEditor
// generated on Tue Dec 11 11:41:38 2012

#include "CADDatumEditor.h"
#include <UmlExt.h>
#include <UdmStatic.h>

#include <UdmDom.h>
#include "CADDatumEditor_xsd.h"
using namespace std;

namespace CADDatumEditor {

	CADComponents::CADComponents() {}
	CADComponents::CADComponents(::Udm::ObjectImpl *impl) : UDM_OBJECT(impl) {}
	CADComponents::CADComponents(const CADComponents &master) : UDM_OBJECT(master) {}

#ifdef UDM_RVALUE
	CADComponents::CADComponents(CADComponents &&master) : UDM_OBJECT(master) {};

	CADComponents CADComponents::Cast(::Udm::Object &&a) { return __Cast(std::move(a), meta); };
	CADComponents& CADComponents::operator=(CADComponents &&a) { ::Udm::Object::operator =(std::move(a)); return *this; };

#endif
	CADComponents CADComponents::Cast(const ::Udm::Object &a) { return __Cast(a, meta); }
	CADComponents CADComponents::Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role); }
	CADComponents CADComponents::CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl); }
	::Udm::InstantiatedAttr< CADComponents> CADComponents::Instances() { return ::Udm::InstantiatedAttr< CADComponents>(impl); }
	CADComponents CADComponents::CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl, true); }
	::Udm::DerivedAttr< CADComponents> CADComponents::Derived() { return ::Udm::DerivedAttr< CADComponents>(impl); }
	::Udm::ArchetypeAttr< CADComponents> CADComponents::Archetype() const { return ::Udm::ArchetypeAttr< CADComponents>(impl); }
	::Udm::ChildrenAttr< ::CADDatumEditor::CADComponent> CADComponents::CADComponent_children() const { return ::Udm::ChildrenAttr< ::CADDatumEditor::CADComponent>(impl, meta_CADComponent_children); }
	::Udm::ChildrenAttr< ::CADDatumEditor::CADComponent> CADComponents::CADComponent_kind_children() const { return ::Udm::ChildrenAttr< ::CADDatumEditor::CADComponent>(impl, ::Udm::NULLCHILDROLE); }
	::Udm::ParentAttr< ::CADDatumEditor::CADDatumEditor> CADComponents::CADDatumEditor_parent() const { return ::Udm::ParentAttr< ::CADDatumEditor::CADDatumEditor>(impl, meta_CADDatumEditor_parent); }
	::Udm::ParentAttr< ::CADDatumEditor::CADDatumEditor> CADComponents::parent() const { return ::Udm::ParentAttr< ::CADDatumEditor::CADDatumEditor>(impl, ::Udm::NULLPARENTROLE); }

	::Uml::Class CADComponents::meta;
	::Uml::CompositionChildRole CADComponents::meta_CADComponent_children;
	::Uml::CompositionParentRole CADComponents::meta_CADDatumEditor_parent;

	Libraries::Libraries() {}
	Libraries::Libraries(::Udm::ObjectImpl *impl) : UDM_OBJECT(impl) {}
	Libraries::Libraries(const Libraries &master) : UDM_OBJECT(master) {}

#ifdef UDM_RVALUE
	Libraries::Libraries(Libraries &&master) : UDM_OBJECT(master) {};

	Libraries Libraries::Cast(::Udm::Object &&a) { return __Cast(std::move(a), meta); };
	Libraries& Libraries::operator=(Libraries &&a) { ::Udm::Object::operator =(std::move(a)); return *this; };

#endif
	Libraries Libraries::Cast(const ::Udm::Object &a) { return __Cast(a, meta); }
	Libraries Libraries::Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role); }
	Libraries Libraries::CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl); }
	::Udm::InstantiatedAttr< Libraries> Libraries::Instances() { return ::Udm::InstantiatedAttr< Libraries>(impl); }
	Libraries Libraries::CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl, true); }
	::Udm::DerivedAttr< Libraries> Libraries::Derived() { return ::Udm::DerivedAttr< Libraries>(impl); }
	::Udm::ArchetypeAttr< Libraries> Libraries::Archetype() const { return ::Udm::ArchetypeAttr< Libraries>(impl); }
	::Udm::ChildrenAttr< ::CADDatumEditor::Library> Libraries::Library_children() const { return ::Udm::ChildrenAttr< ::CADDatumEditor::Library>(impl, meta_Library_children); }
	::Udm::ChildrenAttr< ::CADDatumEditor::Library> Libraries::Library_kind_children() const { return ::Udm::ChildrenAttr< ::CADDatumEditor::Library>(impl, ::Udm::NULLCHILDROLE); }
	::Udm::ParentAttr< ::CADDatumEditor::CADDatumEditor> Libraries::CADDatumEditor_parent() const { return ::Udm::ParentAttr< ::CADDatumEditor::CADDatumEditor>(impl, meta_CADDatumEditor_parent); }
	::Udm::ParentAttr< ::CADDatumEditor::CADDatumEditor> Libraries::parent() const { return ::Udm::ParentAttr< ::CADDatumEditor::CADDatumEditor>(impl, ::Udm::NULLPARENTROLE); }

	::Uml::Class Libraries::meta;
	::Uml::CompositionChildRole Libraries::meta_Library_children;
	::Uml::CompositionParentRole Libraries::meta_CADDatumEditor_parent;

	CADComponent::CADComponent() {}
	CADComponent::CADComponent(::Udm::ObjectImpl *impl) : UDM_OBJECT(impl) {}
	CADComponent::CADComponent(const CADComponent &master) : UDM_OBJECT(master) {}

#ifdef UDM_RVALUE
	CADComponent::CADComponent(CADComponent &&master) : UDM_OBJECT(master) {};

	CADComponent CADComponent::Cast(::Udm::Object &&a) { return __Cast(std::move(a), meta); };
	CADComponent& CADComponent::operator=(CADComponent &&a) { ::Udm::Object::operator =(std::move(a)); return *this; };

#endif
	CADComponent CADComponent::Cast(const ::Udm::Object &a) { return __Cast(a, meta); }
	CADComponent CADComponent::Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role); }
	CADComponent CADComponent::CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl); }
	::Udm::InstantiatedAttr< CADComponent> CADComponent::Instances() { return ::Udm::InstantiatedAttr< CADComponent>(impl); }
	CADComponent CADComponent::CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl, true); }
	::Udm::DerivedAttr< CADComponent> CADComponent::Derived() { return ::Udm::DerivedAttr< CADComponent>(impl); }
	::Udm::ArchetypeAttr< CADComponent> CADComponent::Archetype() const { return ::Udm::ArchetypeAttr< CADComponent>(impl); }
	::Udm::StringAttr CADComponent::Name() const { return ::Udm::StringAttr(impl, meta_Name); }
	::Udm::StringAttr CADComponent::Type() const { return ::Udm::StringAttr(impl, meta_Type); }
	::Udm::StringAttr CADComponent::LibraryID() const { return ::Udm::StringAttr(impl, meta_LibraryID); }
	::Udm::StringAttr CADComponent::Format() const { return ::Udm::StringAttr(impl, meta_Format); }
	::Udm::StringAttr CADComponent::File() const { return ::Udm::StringAttr(impl, meta_File); }
	::Udm::ChildAttr< ::CADDatumEditor::Delete> CADComponent::Delete_child() const { return ::Udm::ChildAttr< ::CADDatumEditor::Delete>(impl, meta_Delete_child); }
	::Udm::ChildAttr< ::CADDatumEditor::Add> CADComponent::Add_child() const { return ::Udm::ChildAttr< ::CADDatumEditor::Add>(impl, meta_Add_child); }
	::Udm::ChildrenAttr< ::CADDatumEditor::Delete> CADComponent::Delete_kind_children() const { return ::Udm::ChildrenAttr< ::CADDatumEditor::Delete>(impl, ::Udm::NULLCHILDROLE); }
	::Udm::ChildrenAttr< ::CADDatumEditor::Add> CADComponent::Add_kind_children() const { return ::Udm::ChildrenAttr< ::CADDatumEditor::Add>(impl, ::Udm::NULLCHILDROLE); }
	::Udm::ParentAttr< ::CADDatumEditor::CADComponents> CADComponent::CADComponents_parent() const { return ::Udm::ParentAttr< ::CADDatumEditor::CADComponents>(impl, meta_CADComponents_parent); }
	::Udm::ParentAttr< ::CADDatumEditor::CADComponents> CADComponent::parent() const { return ::Udm::ParentAttr< ::CADDatumEditor::CADComponents>(impl, ::Udm::NULLPARENTROLE); }

	::Uml::Class CADComponent::meta;
	::Uml::Attribute CADComponent::meta_Name;
	::Uml::Attribute CADComponent::meta_Type;
	::Uml::Attribute CADComponent::meta_LibraryID;
	::Uml::Attribute CADComponent::meta_Format;
	::Uml::Attribute CADComponent::meta_File;
	::Uml::CompositionChildRole CADComponent::meta_Delete_child;
	::Uml::CompositionChildRole CADComponent::meta_Add_child;
	::Uml::CompositionParentRole CADComponent::meta_CADComponents_parent;

	Library::Library() {}
	Library::Library(::Udm::ObjectImpl *impl) : UDM_OBJECT(impl) {}
	Library::Library(const Library &master) : UDM_OBJECT(master) {}

#ifdef UDM_RVALUE
	Library::Library(Library &&master) : UDM_OBJECT(master) {};

	Library Library::Cast(::Udm::Object &&a) { return __Cast(std::move(a), meta); };
	Library& Library::operator=(Library &&a) { ::Udm::Object::operator =(std::move(a)); return *this; };

#endif
	Library Library::Cast(const ::Udm::Object &a) { return __Cast(a, meta); }
	Library Library::Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role); }
	Library Library::CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl); }
	::Udm::InstantiatedAttr< Library> Library::Instances() { return ::Udm::InstantiatedAttr< Library>(impl); }
	Library Library::CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl, true); }
	::Udm::DerivedAttr< Library> Library::Derived() { return ::Udm::DerivedAttr< Library>(impl); }
	::Udm::ArchetypeAttr< Library> Library::Archetype() const { return ::Udm::ArchetypeAttr< Library>(impl); }
	::Udm::StringAttr Library::ID() const { return ::Udm::StringAttr(impl, meta_ID); }
	::Udm::StringAttr Library::DirectoryPath() const { return ::Udm::StringAttr(impl, meta_DirectoryPath); }
	::Udm::ParentAttr< ::CADDatumEditor::Libraries> Library::Libraries_parent() const { return ::Udm::ParentAttr< ::CADDatumEditor::Libraries>(impl, meta_Libraries_parent); }
	::Udm::ParentAttr< ::CADDatumEditor::Libraries> Library::parent() const { return ::Udm::ParentAttr< ::CADDatumEditor::Libraries>(impl, ::Udm::NULLPARENTROLE); }

	::Uml::Class Library::meta;
	::Uml::Attribute Library::meta_ID;
	::Uml::Attribute Library::meta_DirectoryPath;
	::Uml::CompositionParentRole Library::meta_Libraries_parent;

	Delete::Delete() {}
	Delete::Delete(::Udm::ObjectImpl *impl) : UDM_OBJECT(impl) {}
	Delete::Delete(const Delete &master) : UDM_OBJECT(master) {}

#ifdef UDM_RVALUE
	Delete::Delete(Delete &&master) : UDM_OBJECT(master) {};

	Delete Delete::Cast(::Udm::Object &&a) { return __Cast(std::move(a), meta); };
	Delete& Delete::operator=(Delete &&a) { ::Udm::Object::operator =(std::move(a)); return *this; };

#endif
	Delete Delete::Cast(const ::Udm::Object &a) { return __Cast(a, meta); }
	Delete Delete::Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role); }
	Delete Delete::CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl); }
	::Udm::InstantiatedAttr< Delete> Delete::Instances() { return ::Udm::InstantiatedAttr< Delete>(impl); }
	Delete Delete::CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl, true); }
	::Udm::DerivedAttr< Delete> Delete::Derived() { return ::Udm::DerivedAttr< Delete>(impl); }
	::Udm::ArchetypeAttr< Delete> Delete::Archetype() const { return ::Udm::ArchetypeAttr< Delete>(impl); }
	::Udm::ChildAttr< ::CADDatumEditor::DeleteDatums> Delete::DeleteDatums_child() const { return ::Udm::ChildAttr< ::CADDatumEditor::DeleteDatums>(impl, meta_DeleteDatums_child); }
	::Udm::ChildrenAttr< ::CADDatumEditor::DeleteDatums> Delete::DeleteDatums_kind_children() const { return ::Udm::ChildrenAttr< ::CADDatumEditor::DeleteDatums>(impl, ::Udm::NULLCHILDROLE); }
	::Udm::ParentAttr< ::CADDatumEditor::CADComponent> Delete::CADComponent_parent() const { return ::Udm::ParentAttr< ::CADDatumEditor::CADComponent>(impl, meta_CADComponent_parent); }
	::Udm::ParentAttr< ::CADDatumEditor::CADComponent> Delete::parent() const { return ::Udm::ParentAttr< ::CADDatumEditor::CADComponent>(impl, ::Udm::NULLPARENTROLE); }

	::Uml::Class Delete::meta;
	::Uml::CompositionChildRole Delete::meta_DeleteDatums_child;
	::Uml::CompositionParentRole Delete::meta_CADComponent_parent;

	Add::Add() {}
	Add::Add(::Udm::ObjectImpl *impl) : UDM_OBJECT(impl) {}
	Add::Add(const Add &master) : UDM_OBJECT(master) {}

#ifdef UDM_RVALUE
	Add::Add(Add &&master) : UDM_OBJECT(master) {};

	Add Add::Cast(::Udm::Object &&a) { return __Cast(std::move(a), meta); };
	Add& Add::operator=(Add &&a) { ::Udm::Object::operator =(std::move(a)); return *this; };

#endif
	Add Add::Cast(const ::Udm::Object &a) { return __Cast(a, meta); }
	Add Add::Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role); }
	Add Add::CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl); }
	::Udm::InstantiatedAttr< Add> Add::Instances() { return ::Udm::InstantiatedAttr< Add>(impl); }
	Add Add::CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl, true); }
	::Udm::DerivedAttr< Add> Add::Derived() { return ::Udm::DerivedAttr< Add>(impl); }
	::Udm::ArchetypeAttr< Add> Add::Archetype() const { return ::Udm::ArchetypeAttr< Add>(impl); }
	::Udm::ChildAttr< ::CADDatumEditor::AddCoordinateSystems> Add::AddCoordinateSystems_child() const { return ::Udm::ChildAttr< ::CADDatumEditor::AddCoordinateSystems>(impl, meta_AddCoordinateSystems_child); }
	::Udm::ChildAttr< ::CADDatumEditor::AddDatums> Add::AddDatums_child() const { return ::Udm::ChildAttr< ::CADDatumEditor::AddDatums>(impl, meta_AddDatums_child); }
	::Udm::ChildrenAttr< ::CADDatumEditor::AddCoordinateSystems> Add::AddCoordinateSystems_kind_children() const { return ::Udm::ChildrenAttr< ::CADDatumEditor::AddCoordinateSystems>(impl, ::Udm::NULLCHILDROLE); }
	::Udm::ChildrenAttr< ::CADDatumEditor::AddDatums> Add::AddDatums_kind_children() const { return ::Udm::ChildrenAttr< ::CADDatumEditor::AddDatums>(impl, ::Udm::NULLCHILDROLE); }
	::Udm::ParentAttr< ::CADDatumEditor::CADComponent> Add::CADComponent_parent() const { return ::Udm::ParentAttr< ::CADDatumEditor::CADComponent>(impl, meta_CADComponent_parent); }
	::Udm::ParentAttr< ::CADDatumEditor::CADComponent> Add::parent() const { return ::Udm::ParentAttr< ::CADDatumEditor::CADComponent>(impl, ::Udm::NULLPARENTROLE); }

	::Uml::Class Add::meta;
	::Uml::CompositionChildRole Add::meta_AddCoordinateSystems_child;
	::Uml::CompositionChildRole Add::meta_AddDatums_child;
	::Uml::CompositionParentRole Add::meta_CADComponent_parent;

	DeleteDatum::DeleteDatum() {}
	DeleteDatum::DeleteDatum(::Udm::ObjectImpl *impl) : UDM_OBJECT(impl) {}
	DeleteDatum::DeleteDatum(const DeleteDatum &master) : UDM_OBJECT(master) {}

#ifdef UDM_RVALUE
	DeleteDatum::DeleteDatum(DeleteDatum &&master) : UDM_OBJECT(master) {};

	DeleteDatum DeleteDatum::Cast(::Udm::Object &&a) { return __Cast(std::move(a), meta); };
	DeleteDatum& DeleteDatum::operator=(DeleteDatum &&a) { ::Udm::Object::operator =(std::move(a)); return *this; };

#endif
	DeleteDatum DeleteDatum::Cast(const ::Udm::Object &a) { return __Cast(a, meta); }
	DeleteDatum DeleteDatum::Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role); }
	DeleteDatum DeleteDatum::CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl); }
	::Udm::InstantiatedAttr< DeleteDatum> DeleteDatum::Instances() { return ::Udm::InstantiatedAttr< DeleteDatum>(impl); }
	DeleteDatum DeleteDatum::CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl, true); }
	::Udm::DerivedAttr< DeleteDatum> DeleteDatum::Derived() { return ::Udm::DerivedAttr< DeleteDatum>(impl); }
	::Udm::ArchetypeAttr< DeleteDatum> DeleteDatum::Archetype() const { return ::Udm::ArchetypeAttr< DeleteDatum>(impl); }
	::Udm::StringAttr DeleteDatum::DatumName() const { return ::Udm::StringAttr(impl, meta_DatumName); }
	::Udm::StringAttr DeleteDatum::DatumType() const { return ::Udm::StringAttr(impl, meta_DatumType); }
	::Udm::ParentAttr< ::CADDatumEditor::DeleteDatums> DeleteDatum::DeleteDatums_parent() const { return ::Udm::ParentAttr< ::CADDatumEditor::DeleteDatums>(impl, meta_DeleteDatums_parent); }
	::Udm::ParentAttr< ::CADDatumEditor::DeleteDatums> DeleteDatum::parent() const { return ::Udm::ParentAttr< ::CADDatumEditor::DeleteDatums>(impl, ::Udm::NULLPARENTROLE); }

	::Uml::Class DeleteDatum::meta;
	::Uml::Attribute DeleteDatum::meta_DatumName;
	::Uml::Attribute DeleteDatum::meta_DatumType;
	::Uml::CompositionParentRole DeleteDatum::meta_DeleteDatums_parent;

	AddDatum::AddDatum() {}
	AddDatum::AddDatum(::Udm::ObjectImpl *impl) : UDM_OBJECT(impl) {}
	AddDatum::AddDatum(const AddDatum &master) : UDM_OBJECT(master) {}

#ifdef UDM_RVALUE
	AddDatum::AddDatum(AddDatum &&master) : UDM_OBJECT(master) {};

	AddDatum AddDatum::Cast(::Udm::Object &&a) { return __Cast(std::move(a), meta); };
	AddDatum& AddDatum::operator=(AddDatum &&a) { ::Udm::Object::operator =(std::move(a)); return *this; };

#endif
	AddDatum AddDatum::Cast(const ::Udm::Object &a) { return __Cast(a, meta); }
	AddDatum AddDatum::Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role); }
	AddDatum AddDatum::CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl); }
	::Udm::InstantiatedAttr< AddDatum> AddDatum::Instances() { return ::Udm::InstantiatedAttr< AddDatum>(impl); }
	AddDatum AddDatum::CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl, true); }
	::Udm::DerivedAttr< AddDatum> AddDatum::Derived() { return ::Udm::DerivedAttr< AddDatum>(impl); }
	::Udm::ArchetypeAttr< AddDatum> AddDatum::Archetype() const { return ::Udm::ArchetypeAttr< AddDatum>(impl); }
	::Udm::StringAttr AddDatum::DatumName() const { return ::Udm::StringAttr(impl, meta_DatumName); }
	::Udm::StringAttr AddDatum::DatumType() const { return ::Udm::StringAttr(impl, meta_DatumType); }
	::Udm::StringAttr AddDatum::ReplaceIfExists() const { return ::Udm::StringAttr(impl, meta_ReplaceIfExists); }
	::Udm::StringAttr AddDatum::CoordinateSystemName() const { return ::Udm::StringAttr(impl, meta_CoordinateSystemName); }
	::Udm::StringAttr AddDatum::CoordinateAlignment() const { return ::Udm::StringAttr(impl, meta_CoordinateAlignment); }
	::Udm::StringAttr AddDatum::FlipDatumPlaneDirection() const { return ::Udm::StringAttr(impl, meta_FlipDatumPlaneDirection); }
	::Udm::ParentAttr< ::CADDatumEditor::AddDatums> AddDatum::AddDatums_parent() const { return ::Udm::ParentAttr< ::CADDatumEditor::AddDatums>(impl, meta_AddDatums_parent); }
	::Udm::ParentAttr< ::CADDatumEditor::AddDatums> AddDatum::parent() const { return ::Udm::ParentAttr< ::CADDatumEditor::AddDatums>(impl, ::Udm::NULLPARENTROLE); }

	::Uml::Class AddDatum::meta;
	::Uml::Attribute AddDatum::meta_DatumName;
	::Uml::Attribute AddDatum::meta_DatumType;
	::Uml::Attribute AddDatum::meta_ReplaceIfExists;
	::Uml::Attribute AddDatum::meta_CoordinateSystemName;
	::Uml::Attribute AddDatum::meta_CoordinateAlignment;
	::Uml::Attribute AddDatum::meta_FlipDatumPlaneDirection;
	::Uml::CompositionParentRole AddDatum::meta_AddDatums_parent;

	CADDatumEditor::CADDatumEditor() {}
	CADDatumEditor::CADDatumEditor(::Udm::ObjectImpl *impl) : UDM_OBJECT(impl) {}
	CADDatumEditor::CADDatumEditor(const CADDatumEditor &master) : UDM_OBJECT(master) {}

#ifdef UDM_RVALUE
	CADDatumEditor::CADDatumEditor(CADDatumEditor &&master) : UDM_OBJECT(master) {};

	CADDatumEditor CADDatumEditor::Cast(::Udm::Object &&a) { return __Cast(std::move(a), meta); };
	CADDatumEditor& CADDatumEditor::operator=(CADDatumEditor &&a) { ::Udm::Object::operator =(std::move(a)); return *this; };

#endif
	CADDatumEditor CADDatumEditor::Cast(const ::Udm::Object &a) { return __Cast(a, meta); }
	CADDatumEditor CADDatumEditor::Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role); }
	CADDatumEditor CADDatumEditor::CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl); }
	::Udm::InstantiatedAttr< CADDatumEditor> CADDatumEditor::Instances() { return ::Udm::InstantiatedAttr< CADDatumEditor>(impl); }
	CADDatumEditor CADDatumEditor::CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl, true); }
	::Udm::DerivedAttr< CADDatumEditor> CADDatumEditor::Derived() { return ::Udm::DerivedAttr< CADDatumEditor>(impl); }
	::Udm::ArchetypeAttr< CADDatumEditor> CADDatumEditor::Archetype() const { return ::Udm::ArchetypeAttr< CADDatumEditor>(impl); }
	::Udm::ChildrenAttr< ::CADDatumEditor::Libraries> CADDatumEditor::Libraries_children() const { return ::Udm::ChildrenAttr< ::CADDatumEditor::Libraries>(impl, meta_Libraries_children); }
	::Udm::ChildrenAttr< ::CADDatumEditor::CADComponents> CADDatumEditor::CADComponents_children() const { return ::Udm::ChildrenAttr< ::CADDatumEditor::CADComponents>(impl, meta_CADComponents_children); }
	::Udm::ChildrenAttr< ::CADDatumEditor::CADComponents> CADDatumEditor::CADComponents_kind_children() const { return ::Udm::ChildrenAttr< ::CADDatumEditor::CADComponents>(impl, ::Udm::NULLCHILDROLE); }
	::Udm::ChildrenAttr< ::CADDatumEditor::Libraries> CADDatumEditor::Libraries_kind_children() const { return ::Udm::ChildrenAttr< ::CADDatumEditor::Libraries>(impl, ::Udm::NULLCHILDROLE); }
	::Udm::ParentAttr< ::Udm::Object> CADDatumEditor::parent() const { return ::Udm::ParentAttr< ::Udm::Object>(impl, ::Udm::NULLPARENTROLE); }

	::Uml::Class CADDatumEditor::meta;
	::Uml::CompositionChildRole CADDatumEditor::meta_Libraries_children;
	::Uml::CompositionChildRole CADDatumEditor::meta_CADComponents_children;

	Origin::Origin() {}
	Origin::Origin(::Udm::ObjectImpl *impl) : CoordinateBase(impl) {}
	Origin::Origin(const Origin &master) : CoordinateBase(master) {}

#ifdef UDM_RVALUE
	Origin::Origin(Origin &&master) : CoordinateBase(master) {};

	Origin Origin::Cast(::Udm::Object &&a) { return __Cast(std::move(a), meta); };
	Origin& Origin::operator=(Origin &&a) { ::Udm::Object::operator =(std::move(a)); return *this; };

#endif
	Origin Origin::Cast(const ::Udm::Object &a) { return __Cast(a, meta); }
	Origin Origin::Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role); }
	Origin Origin::CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl); }
	::Udm::InstantiatedAttr< Origin> Origin::Instances() { return ::Udm::InstantiatedAttr< Origin>(impl); }
	Origin Origin::CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl, true); }
	::Udm::DerivedAttr< Origin> Origin::Derived() { return ::Udm::DerivedAttr< Origin>(impl); }
	::Udm::ArchetypeAttr< Origin> Origin::Archetype() const { return ::Udm::ArchetypeAttr< Origin>(impl); }
	::Udm::ParentAttr< ::CADDatumEditor::AddCoordinateSystem> Origin::AddCoordinateSystem_parent() const { return ::Udm::ParentAttr< ::CADDatumEditor::AddCoordinateSystem>(impl, meta_AddCoordinateSystem_parent); }
	::Udm::ParentAttr< ::CADDatumEditor::AddCoordinateSystem> Origin::parent() const { return ::Udm::ParentAttr< ::CADDatumEditor::AddCoordinateSystem>(impl, ::Udm::NULLPARENTROLE); }

	::Uml::Class Origin::meta;
	::Uml::CompositionParentRole Origin::meta_AddCoordinateSystem_parent;

	AddCoordinateSystems::AddCoordinateSystems() {}
	AddCoordinateSystems::AddCoordinateSystems(::Udm::ObjectImpl *impl) : UDM_OBJECT(impl) {}
	AddCoordinateSystems::AddCoordinateSystems(const AddCoordinateSystems &master) : UDM_OBJECT(master) {}

#ifdef UDM_RVALUE
	AddCoordinateSystems::AddCoordinateSystems(AddCoordinateSystems &&master) : UDM_OBJECT(master) {};

	AddCoordinateSystems AddCoordinateSystems::Cast(::Udm::Object &&a) { return __Cast(std::move(a), meta); };
	AddCoordinateSystems& AddCoordinateSystems::operator=(AddCoordinateSystems &&a) { ::Udm::Object::operator =(std::move(a)); return *this; };

#endif
	AddCoordinateSystems AddCoordinateSystems::Cast(const ::Udm::Object &a) { return __Cast(a, meta); }
	AddCoordinateSystems AddCoordinateSystems::Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role); }
	AddCoordinateSystems AddCoordinateSystems::CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl); }
	::Udm::InstantiatedAttr< AddCoordinateSystems> AddCoordinateSystems::Instances() { return ::Udm::InstantiatedAttr< AddCoordinateSystems>(impl); }
	AddCoordinateSystems AddCoordinateSystems::CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl, true); }
	::Udm::DerivedAttr< AddCoordinateSystems> AddCoordinateSystems::Derived() { return ::Udm::DerivedAttr< AddCoordinateSystems>(impl); }
	::Udm::ArchetypeAttr< AddCoordinateSystems> AddCoordinateSystems::Archetype() const { return ::Udm::ArchetypeAttr< AddCoordinateSystems>(impl); }
	::Udm::ChildrenAttr< ::CADDatumEditor::AddCoordinateSystem> AddCoordinateSystems::AddCoordinateSystem_children() const { return ::Udm::ChildrenAttr< ::CADDatumEditor::AddCoordinateSystem>(impl, meta_AddCoordinateSystem_children); }
	::Udm::ChildrenAttr< ::CADDatumEditor::AddCoordinateSystem> AddCoordinateSystems::AddCoordinateSystem_kind_children() const { return ::Udm::ChildrenAttr< ::CADDatumEditor::AddCoordinateSystem>(impl, ::Udm::NULLCHILDROLE); }
	::Udm::ParentAttr< ::CADDatumEditor::Add> AddCoordinateSystems::Add_parent() const { return ::Udm::ParentAttr< ::CADDatumEditor::Add>(impl, meta_Add_parent); }
	::Udm::ParentAttr< ::CADDatumEditor::Add> AddCoordinateSystems::parent() const { return ::Udm::ParentAttr< ::CADDatumEditor::Add>(impl, ::Udm::NULLPARENTROLE); }

	::Uml::Class AddCoordinateSystems::meta;
	::Uml::CompositionChildRole AddCoordinateSystems::meta_AddCoordinateSystem_children;
	::Uml::CompositionParentRole AddCoordinateSystems::meta_Add_parent;

	AddCoordinateSystem::AddCoordinateSystem() {}
	AddCoordinateSystem::AddCoordinateSystem(::Udm::ObjectImpl *impl) : UDM_OBJECT(impl) {}
	AddCoordinateSystem::AddCoordinateSystem(const AddCoordinateSystem &master) : UDM_OBJECT(master) {}

#ifdef UDM_RVALUE
	AddCoordinateSystem::AddCoordinateSystem(AddCoordinateSystem &&master) : UDM_OBJECT(master) {};

	AddCoordinateSystem AddCoordinateSystem::Cast(::Udm::Object &&a) { return __Cast(std::move(a), meta); };
	AddCoordinateSystem& AddCoordinateSystem::operator=(AddCoordinateSystem &&a) { ::Udm::Object::operator =(std::move(a)); return *this; };

#endif
	AddCoordinateSystem AddCoordinateSystem::Cast(const ::Udm::Object &a) { return __Cast(a, meta); }
	AddCoordinateSystem AddCoordinateSystem::Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role); }
	AddCoordinateSystem AddCoordinateSystem::CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl); }
	::Udm::InstantiatedAttr< AddCoordinateSystem> AddCoordinateSystem::Instances() { return ::Udm::InstantiatedAttr< AddCoordinateSystem>(impl); }
	AddCoordinateSystem AddCoordinateSystem::CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl, true); }
	::Udm::DerivedAttr< AddCoordinateSystem> AddCoordinateSystem::Derived() { return ::Udm::DerivedAttr< AddCoordinateSystem>(impl); }
	::Udm::ArchetypeAttr< AddCoordinateSystem> AddCoordinateSystem::Archetype() const { return ::Udm::ArchetypeAttr< AddCoordinateSystem>(impl); }
	::Udm::StringAttr AddCoordinateSystem::CoordinateSystemName() const { return ::Udm::StringAttr(impl, meta_CoordinateSystemName); }
	::Udm::StringAttr AddCoordinateSystem::ReplaceIfExists() const { return ::Udm::StringAttr(impl, meta_ReplaceIfExists); }
	::Udm::ChildAttr< ::CADDatumEditor::Origin> AddCoordinateSystem::Origin_child() const { return ::Udm::ChildAttr< ::CADDatumEditor::Origin>(impl, meta_Origin_child); }
	::Udm::ChildAttr< ::CADDatumEditor::XVector> AddCoordinateSystem::XVector_child() const { return ::Udm::ChildAttr< ::CADDatumEditor::XVector>(impl, meta_XVector_child); }
	::Udm::ChildAttr< ::CADDatumEditor::YVector> AddCoordinateSystem::YVector_child() const { return ::Udm::ChildAttr< ::CADDatumEditor::YVector>(impl, meta_YVector_child); }
	::Udm::ChildrenAttr< ::CADDatumEditor::Origin> AddCoordinateSystem::Origin_kind_children() const { return ::Udm::ChildrenAttr< ::CADDatumEditor::Origin>(impl, ::Udm::NULLCHILDROLE); }
	::Udm::ChildrenAttr< ::CADDatumEditor::CoordinateBase> AddCoordinateSystem::CoordinateBase_kind_children() const { return ::Udm::ChildrenAttr< ::CADDatumEditor::CoordinateBase>(impl, ::Udm::NULLCHILDROLE); }
	::Udm::ChildrenAttr< ::CADDatumEditor::XVector> AddCoordinateSystem::XVector_kind_children() const { return ::Udm::ChildrenAttr< ::CADDatumEditor::XVector>(impl, ::Udm::NULLCHILDROLE); }
	::Udm::ChildrenAttr< ::CADDatumEditor::YVector> AddCoordinateSystem::YVector_kind_children() const { return ::Udm::ChildrenAttr< ::CADDatumEditor::YVector>(impl, ::Udm::NULLCHILDROLE); }
	::Udm::ParentAttr< ::CADDatumEditor::AddCoordinateSystems> AddCoordinateSystem::AddCoordinateSystems_parent() const { return ::Udm::ParentAttr< ::CADDatumEditor::AddCoordinateSystems>(impl, meta_AddCoordinateSystems_parent); }
	::Udm::ParentAttr< ::CADDatumEditor::AddCoordinateSystems> AddCoordinateSystem::parent() const { return ::Udm::ParentAttr< ::CADDatumEditor::AddCoordinateSystems>(impl, ::Udm::NULLPARENTROLE); }

	::Uml::Class AddCoordinateSystem::meta;
	::Uml::Attribute AddCoordinateSystem::meta_CoordinateSystemName;
	::Uml::Attribute AddCoordinateSystem::meta_ReplaceIfExists;
	::Uml::CompositionChildRole AddCoordinateSystem::meta_Origin_child;
	::Uml::CompositionChildRole AddCoordinateSystem::meta_XVector_child;
	::Uml::CompositionChildRole AddCoordinateSystem::meta_YVector_child;
	::Uml::CompositionParentRole AddCoordinateSystem::meta_AddCoordinateSystems_parent;

	CoordinateBase::CoordinateBase() {}
	CoordinateBase::CoordinateBase(::Udm::ObjectImpl *impl) : UDM_OBJECT(impl) {}
	CoordinateBase::CoordinateBase(const CoordinateBase &master) : UDM_OBJECT(master) {}

#ifdef UDM_RVALUE
	CoordinateBase::CoordinateBase(CoordinateBase &&master) : UDM_OBJECT(master) {};

	CoordinateBase CoordinateBase::Cast(::Udm::Object &&a) { return __Cast(std::move(a), meta); };
	CoordinateBase& CoordinateBase::operator=(CoordinateBase &&a) { ::Udm::Object::operator =(std::move(a)); return *this; };

#endif
	CoordinateBase CoordinateBase::Cast(const ::Udm::Object &a) { return __Cast(a, meta); }
	CoordinateBase CoordinateBase::Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role); }
	CoordinateBase CoordinateBase::CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl); }
	::Udm::InstantiatedAttr< CoordinateBase> CoordinateBase::Instances() { return ::Udm::InstantiatedAttr< CoordinateBase>(impl); }
	CoordinateBase CoordinateBase::CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl, true); }
	::Udm::DerivedAttr< CoordinateBase> CoordinateBase::Derived() { return ::Udm::DerivedAttr< CoordinateBase>(impl); }
	::Udm::ArchetypeAttr< CoordinateBase> CoordinateBase::Archetype() const { return ::Udm::ArchetypeAttr< CoordinateBase>(impl); }
	::Udm::RealAttr CoordinateBase::X() const { return ::Udm::RealAttr(impl, meta_X); }
	::Udm::RealAttr CoordinateBase::Y() const { return ::Udm::RealAttr(impl, meta_Y); }
	::Udm::RealAttr CoordinateBase::Z() const { return ::Udm::RealAttr(impl, meta_Z); }
	::Udm::ParentAttr< ::CADDatumEditor::AddCoordinateSystem> CoordinateBase::parent() const { return ::Udm::ParentAttr< ::CADDatumEditor::AddCoordinateSystem>(impl, ::Udm::NULLPARENTROLE); }

	::Uml::Class CoordinateBase::meta;
	::Uml::Attribute CoordinateBase::meta_X;
	::Uml::Attribute CoordinateBase::meta_Y;
	::Uml::Attribute CoordinateBase::meta_Z;

	DeleteDatums::DeleteDatums() {}
	DeleteDatums::DeleteDatums(::Udm::ObjectImpl *impl) : UDM_OBJECT(impl) {}
	DeleteDatums::DeleteDatums(const DeleteDatums &master) : UDM_OBJECT(master) {}

#ifdef UDM_RVALUE
	DeleteDatums::DeleteDatums(DeleteDatums &&master) : UDM_OBJECT(master) {};

	DeleteDatums DeleteDatums::Cast(::Udm::Object &&a) { return __Cast(std::move(a), meta); };
	DeleteDatums& DeleteDatums::operator=(DeleteDatums &&a) { ::Udm::Object::operator =(std::move(a)); return *this; };

#endif
	DeleteDatums DeleteDatums::Cast(const ::Udm::Object &a) { return __Cast(a, meta); }
	DeleteDatums DeleteDatums::Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role); }
	DeleteDatums DeleteDatums::CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl); }
	::Udm::InstantiatedAttr< DeleteDatums> DeleteDatums::Instances() { return ::Udm::InstantiatedAttr< DeleteDatums>(impl); }
	DeleteDatums DeleteDatums::CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl, true); }
	::Udm::DerivedAttr< DeleteDatums> DeleteDatums::Derived() { return ::Udm::DerivedAttr< DeleteDatums>(impl); }
	::Udm::ArchetypeAttr< DeleteDatums> DeleteDatums::Archetype() const { return ::Udm::ArchetypeAttr< DeleteDatums>(impl); }
	::Udm::ChildrenAttr< ::CADDatumEditor::DeleteDatum> DeleteDatums::DeleteDatum_children() const { return ::Udm::ChildrenAttr< ::CADDatumEditor::DeleteDatum>(impl, meta_DeleteDatum_children); }
	::Udm::ChildrenAttr< ::CADDatumEditor::DeleteDatum> DeleteDatums::DeleteDatum_kind_children() const { return ::Udm::ChildrenAttr< ::CADDatumEditor::DeleteDatum>(impl, ::Udm::NULLCHILDROLE); }
	::Udm::ParentAttr< ::CADDatumEditor::Delete> DeleteDatums::Delete_parent() const { return ::Udm::ParentAttr< ::CADDatumEditor::Delete>(impl, meta_Delete_parent); }
	::Udm::ParentAttr< ::CADDatumEditor::Delete> DeleteDatums::parent() const { return ::Udm::ParentAttr< ::CADDatumEditor::Delete>(impl, ::Udm::NULLPARENTROLE); }

	::Uml::Class DeleteDatums::meta;
	::Uml::CompositionChildRole DeleteDatums::meta_DeleteDatum_children;
	::Uml::CompositionParentRole DeleteDatums::meta_Delete_parent;

	AddDatums::AddDatums() {}
	AddDatums::AddDatums(::Udm::ObjectImpl *impl) : UDM_OBJECT(impl) {}
	AddDatums::AddDatums(const AddDatums &master) : UDM_OBJECT(master) {}

#ifdef UDM_RVALUE
	AddDatums::AddDatums(AddDatums &&master) : UDM_OBJECT(master) {};

	AddDatums AddDatums::Cast(::Udm::Object &&a) { return __Cast(std::move(a), meta); };
	AddDatums& AddDatums::operator=(AddDatums &&a) { ::Udm::Object::operator =(std::move(a)); return *this; };

#endif
	AddDatums AddDatums::Cast(const ::Udm::Object &a) { return __Cast(a, meta); }
	AddDatums AddDatums::Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role); }
	AddDatums AddDatums::CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl); }
	::Udm::InstantiatedAttr< AddDatums> AddDatums::Instances() { return ::Udm::InstantiatedAttr< AddDatums>(impl); }
	AddDatums AddDatums::CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl, true); }
	::Udm::DerivedAttr< AddDatums> AddDatums::Derived() { return ::Udm::DerivedAttr< AddDatums>(impl); }
	::Udm::ArchetypeAttr< AddDatums> AddDatums::Archetype() const { return ::Udm::ArchetypeAttr< AddDatums>(impl); }
	::Udm::ChildrenAttr< ::CADDatumEditor::AddDatum> AddDatums::AddDatum_children() const { return ::Udm::ChildrenAttr< ::CADDatumEditor::AddDatum>(impl, meta_AddDatum_children); }
	::Udm::ChildrenAttr< ::CADDatumEditor::AddDatum> AddDatums::AddDatum_kind_children() const { return ::Udm::ChildrenAttr< ::CADDatumEditor::AddDatum>(impl, ::Udm::NULLCHILDROLE); }
	::Udm::ParentAttr< ::CADDatumEditor::Add> AddDatums::Add_parent() const { return ::Udm::ParentAttr< ::CADDatumEditor::Add>(impl, meta_Add_parent); }
	::Udm::ParentAttr< ::CADDatumEditor::Add> AddDatums::parent() const { return ::Udm::ParentAttr< ::CADDatumEditor::Add>(impl, ::Udm::NULLPARENTROLE); }

	::Uml::Class AddDatums::meta;
	::Uml::CompositionChildRole AddDatums::meta_AddDatum_children;
	::Uml::CompositionParentRole AddDatums::meta_Add_parent;

	XVector::XVector() {}
	XVector::XVector(::Udm::ObjectImpl *impl) : CoordinateBase(impl) {}
	XVector::XVector(const XVector &master) : CoordinateBase(master) {}

#ifdef UDM_RVALUE
	XVector::XVector(XVector &&master) : CoordinateBase(master) {};

	XVector XVector::Cast(::Udm::Object &&a) { return __Cast(std::move(a), meta); };
	XVector& XVector::operator=(XVector &&a) { ::Udm::Object::operator =(std::move(a)); return *this; };

#endif
	XVector XVector::Cast(const ::Udm::Object &a) { return __Cast(a, meta); }
	XVector XVector::Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role); }
	XVector XVector::CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl); }
	::Udm::InstantiatedAttr< XVector> XVector::Instances() { return ::Udm::InstantiatedAttr< XVector>(impl); }
	XVector XVector::CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl, true); }
	::Udm::DerivedAttr< XVector> XVector::Derived() { return ::Udm::DerivedAttr< XVector>(impl); }
	::Udm::ArchetypeAttr< XVector> XVector::Archetype() const { return ::Udm::ArchetypeAttr< XVector>(impl); }
	::Udm::ParentAttr< ::CADDatumEditor::AddCoordinateSystem> XVector::AddCoordinateSystem_parent() const { return ::Udm::ParentAttr< ::CADDatumEditor::AddCoordinateSystem>(impl, meta_AddCoordinateSystem_parent); }
	::Udm::ParentAttr< ::CADDatumEditor::AddCoordinateSystem> XVector::parent() const { return ::Udm::ParentAttr< ::CADDatumEditor::AddCoordinateSystem>(impl, ::Udm::NULLPARENTROLE); }

	::Uml::Class XVector::meta;
	::Uml::CompositionParentRole XVector::meta_AddCoordinateSystem_parent;

	YVector::YVector() {}
	YVector::YVector(::Udm::ObjectImpl *impl) : CoordinateBase(impl) {}
	YVector::YVector(const YVector &master) : CoordinateBase(master) {}

#ifdef UDM_RVALUE
	YVector::YVector(YVector &&master) : CoordinateBase(master) {};

	YVector YVector::Cast(::Udm::Object &&a) { return __Cast(std::move(a), meta); };
	YVector& YVector::operator=(YVector &&a) { ::Udm::Object::operator =(std::move(a)); return *this; };

#endif
	YVector YVector::Cast(const ::Udm::Object &a) { return __Cast(a, meta); }
	YVector YVector::Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role); }
	YVector YVector::CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl); }
	::Udm::InstantiatedAttr< YVector> YVector::Instances() { return ::Udm::InstantiatedAttr< YVector>(impl); }
	YVector YVector::CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl, true); }
	::Udm::DerivedAttr< YVector> YVector::Derived() { return ::Udm::DerivedAttr< YVector>(impl); }
	::Udm::ArchetypeAttr< YVector> YVector::Archetype() const { return ::Udm::ArchetypeAttr< YVector>(impl); }
	::Udm::ParentAttr< ::CADDatumEditor::AddCoordinateSystem> YVector::AddCoordinateSystem_parent() const { return ::Udm::ParentAttr< ::CADDatumEditor::AddCoordinateSystem>(impl, meta_AddCoordinateSystem_parent); }
	::Udm::ParentAttr< ::CADDatumEditor::AddCoordinateSystem> YVector::parent() const { return ::Udm::ParentAttr< ::CADDatumEditor::AddCoordinateSystem>(impl, ::Udm::NULLPARENTROLE); }

	::Uml::Class YVector::meta;
	::Uml::CompositionParentRole YVector::meta_AddCoordinateSystem_parent;

	::Uml::Diagram meta;

	void CreateMeta() {
		// classes, with attributes, constraints and constraint definitions
		Add::meta = ::Uml::Class::Create(meta);

		AddCoordinateSystem::meta = ::Uml::Class::Create(meta);
		AddCoordinateSystem::meta_CoordinateSystemName = ::Uml::Attribute::Create(AddCoordinateSystem::meta);
		AddCoordinateSystem::meta_ReplaceIfExists = ::Uml::Attribute::Create(AddCoordinateSystem::meta);

		AddCoordinateSystems::meta = ::Uml::Class::Create(meta);

		AddDatum::meta = ::Uml::Class::Create(meta);
		AddDatum::meta_DatumName = ::Uml::Attribute::Create(AddDatum::meta);
		AddDatum::meta_DatumType = ::Uml::Attribute::Create(AddDatum::meta);
		AddDatum::meta_ReplaceIfExists = ::Uml::Attribute::Create(AddDatum::meta);
		AddDatum::meta_CoordinateSystemName = ::Uml::Attribute::Create(AddDatum::meta);
		AddDatum::meta_CoordinateAlignment = ::Uml::Attribute::Create(AddDatum::meta);
		AddDatum::meta_FlipDatumPlaneDirection = ::Uml::Attribute::Create(AddDatum::meta);

		AddDatums::meta = ::Uml::Class::Create(meta);

		CADComponent::meta = ::Uml::Class::Create(meta);
		CADComponent::meta_Name = ::Uml::Attribute::Create(CADComponent::meta);
		CADComponent::meta_Type = ::Uml::Attribute::Create(CADComponent::meta);
		CADComponent::meta_LibraryID = ::Uml::Attribute::Create(CADComponent::meta);
		CADComponent::meta_Format = ::Uml::Attribute::Create(CADComponent::meta);
		CADComponent::meta_File = ::Uml::Attribute::Create(CADComponent::meta);

		CADComponents::meta = ::Uml::Class::Create(meta);

		CADDatumEditor::meta = ::Uml::Class::Create(meta);

		CoordinateBase::meta = ::Uml::Class::Create(meta);
		CoordinateBase::meta_X = ::Uml::Attribute::Create(CoordinateBase::meta);
		CoordinateBase::meta_Y = ::Uml::Attribute::Create(CoordinateBase::meta);
		CoordinateBase::meta_Z = ::Uml::Attribute::Create(CoordinateBase::meta);

		Delete::meta = ::Uml::Class::Create(meta);

		DeleteDatum::meta = ::Uml::Class::Create(meta);
		DeleteDatum::meta_DatumName = ::Uml::Attribute::Create(DeleteDatum::meta);
		DeleteDatum::meta_DatumType = ::Uml::Attribute::Create(DeleteDatum::meta);

		DeleteDatums::meta = ::Uml::Class::Create(meta);

		Libraries::meta = ::Uml::Class::Create(meta);

		Library::meta = ::Uml::Class::Create(meta);
		Library::meta_ID = ::Uml::Attribute::Create(Library::meta);
		Library::meta_DirectoryPath = ::Uml::Attribute::Create(Library::meta);

		Origin::meta = ::Uml::Class::Create(meta);

		XVector::meta = ::Uml::Class::Create(meta);

		YVector::meta = ::Uml::Class::Create(meta);

	}

	void InitMeta() {
		// classes, with attributes, constraints and constraint definitions
		::Uml::InitClassProps(Add::meta, "Add", false, NULL, NULL);

		::Uml::InitClassProps(AddCoordinateSystem::meta, "AddCoordinateSystem", false, NULL, NULL);
		::Uml::InitAttributeProps(AddCoordinateSystem::meta_CoordinateSystemName, "CoordinateSystemName", "String", false, false, 1, 1, false, "public", vector<string>());
		::Uml::InitAttributeProps(AddCoordinateSystem::meta_ReplaceIfExists, "ReplaceIfExists", "String", false, false, 1, 1, false, "public", vector<string>());

		::Uml::InitClassProps(AddCoordinateSystems::meta, "AddCoordinateSystems", false, NULL, NULL);

		::Uml::InitClassProps(AddDatum::meta, "AddDatum", false, NULL, NULL);
		::Uml::InitAttributeProps(AddDatum::meta_DatumName, "DatumName", "String", false, false, 1, 1, false, "public", vector<string>());
		::Uml::InitAttributeProps(AddDatum::meta_DatumType, "DatumType", "String", false, false, 1, 1, false, "public", vector<string>());
		::Uml::InitAttributeProps(AddDatum::meta_ReplaceIfExists, "ReplaceIfExists", "String", false, false, 1, 1, false, "public", vector<string>());
		::Uml::InitAttributeProps(AddDatum::meta_CoordinateSystemName, "CoordinateSystemName", "String", false, false, 1, 1, false, "public", vector<string>());
		::Uml::InitAttributeProps(AddDatum::meta_CoordinateAlignment, "CoordinateAlignment", "String", false, false, 1, 1, false, "public", vector<string>());
		::Uml::InitAttributeProps(AddDatum::meta_FlipDatumPlaneDirection, "FlipDatumPlaneDirection", "String", false, false, 1, 1, false, "public", vector<string>());

		::Uml::InitClassProps(AddDatums::meta, "AddDatums", false, NULL, NULL);

		::Uml::InitClassProps(CADComponent::meta, "CADComponent", false, NULL, NULL);
		::Uml::InitAttributeProps(CADComponent::meta_Name, "Name", "String", false, false, 1, 1, false, "public", vector<string>());
		::Uml::InitAttributeProps(CADComponent::meta_Type, "Type", "String", false, false, 1, 1, false, "public", vector<string>());
		::Uml::InitAttributeProps(CADComponent::meta_LibraryID, "LibraryID", "String", false, false, 1, 1, false, "public", vector<string>());
		::Uml::InitAttributeProps(CADComponent::meta_Format, "Format", "String", false, false, 1, 1, false, "public", vector<string>());
		::Uml::InitAttributeProps(CADComponent::meta_File, "File", "String", false, false, 1, 1, false, "public", vector<string>());

		::Uml::InitClassProps(CADComponents::meta, "CADComponents", false, NULL, NULL);

		::Uml::InitClassProps(CADDatumEditor::meta, "CADDatumEditor", false, NULL, NULL);

		::Uml::InitClassProps(CoordinateBase::meta, "CoordinateBase", true, NULL, NULL);
		::Uml::InitAttributeProps(CoordinateBase::meta_X, "X", "Real", false, false, 1, 1, false, "public", vector<string>());
		::Uml::InitAttributeProps(CoordinateBase::meta_Y, "Y", "Real", false, false, 1, 1, false, "public", vector<string>());
		::Uml::InitAttributeProps(CoordinateBase::meta_Z, "Z", "Real", false, false, 1, 1, false, "public", vector<string>());

		::Uml::InitClassProps(Delete::meta, "Delete", false, NULL, NULL);

		::Uml::InitClassProps(DeleteDatum::meta, "DeleteDatum", false, NULL, NULL);
		::Uml::InitAttributeProps(DeleteDatum::meta_DatumName, "DatumName", "String", false, false, 1, 1, false, "public", vector<string>());
		::Uml::InitAttributeProps(DeleteDatum::meta_DatumType, "DatumType", "String", false, false, 1, 1, false, "public", vector<string>());

		::Uml::InitClassProps(DeleteDatums::meta, "DeleteDatums", false, NULL, NULL);

		::Uml::InitClassProps(Libraries::meta, "Libraries", false, NULL, NULL);

		::Uml::InitClassProps(Library::meta, "Library", false, NULL, NULL);
		::Uml::InitAttributeProps(Library::meta_ID, "ID", "String", false, false, 1, 1, false, "public", vector<string>());
		::Uml::InitAttributeProps(Library::meta_DirectoryPath, "DirectoryPath", "String", false, false, 1, 1, false, "public", vector<string>());

		::Uml::InitClassProps(Origin::meta, "Origin", false, NULL, NULL);

		::Uml::InitClassProps(XVector::meta, "XVector", false, NULL, NULL);

		::Uml::InitClassProps(YVector::meta, "YVector", false, NULL, NULL);

		// compositions
		{
			::Uml::Composition comp = ::Uml::Composition::Create(meta);
			::Uml::InitCompositionProps(comp, "Composition", false);
			CADComponent::meta_CADComponents_parent = ::Uml::CompositionParentRole::Create(comp);
			::Uml::InitCompositionParentRoleProps(CADComponent::meta_CADComponents_parent, "CADComponents_parent", true);
			CADComponents::meta_CADComponent_children = ::Uml::CompositionChildRole::Create(comp);
			::Uml::InitCompositionChildRoleProps(CADComponents::meta_CADComponent_children, "CADComponent", true, 0, -1);

		}
		{
			::Uml::Composition comp = ::Uml::Composition::Create(meta);
			::Uml::InitCompositionProps(comp, "Composition", false);
			Library::meta_Libraries_parent = ::Uml::CompositionParentRole::Create(comp);
			::Uml::InitCompositionParentRoleProps(Library::meta_Libraries_parent, "Libraries_parent", true);
			Libraries::meta_Library_children = ::Uml::CompositionChildRole::Create(comp);
			::Uml::InitCompositionChildRoleProps(Libraries::meta_Library_children, "Library", true, 0, -1);

		}
		{
			::Uml::Composition comp = ::Uml::Composition::Create(meta);
			::Uml::InitCompositionProps(comp, "Composition", false);
			Delete::meta_CADComponent_parent = ::Uml::CompositionParentRole::Create(comp);
			::Uml::InitCompositionParentRoleProps(Delete::meta_CADComponent_parent, "CADComponent_parent", true);
			CADComponent::meta_Delete_child = ::Uml::CompositionChildRole::Create(comp);
			::Uml::InitCompositionChildRoleProps(CADComponent::meta_Delete_child, "Delete", true, 0, 1);

		}
		{
			::Uml::Composition comp = ::Uml::Composition::Create(meta);
			::Uml::InitCompositionProps(comp, "Composition", false);
			Add::meta_CADComponent_parent = ::Uml::CompositionParentRole::Create(comp);
			::Uml::InitCompositionParentRoleProps(Add::meta_CADComponent_parent, "CADComponent_parent", true);
			CADComponent::meta_Add_child = ::Uml::CompositionChildRole::Create(comp);
			::Uml::InitCompositionChildRoleProps(CADComponent::meta_Add_child, "Add", true, 0, 1);

		}
		{
			::Uml::Composition comp = ::Uml::Composition::Create(meta);
			::Uml::InitCompositionProps(comp, "Composition", false);
			DeleteDatums::meta_Delete_parent = ::Uml::CompositionParentRole::Create(comp);
			::Uml::InitCompositionParentRoleProps(DeleteDatums::meta_Delete_parent, "Delete_parent", true);
			Delete::meta_DeleteDatums_child = ::Uml::CompositionChildRole::Create(comp);
			::Uml::InitCompositionChildRoleProps(Delete::meta_DeleteDatums_child, "DeleteDatums", true, 0, 1);

		}
		{
			::Uml::Composition comp = ::Uml::Composition::Create(meta);
			::Uml::InitCompositionProps(comp, "Composition", false);
			AddCoordinateSystems::meta_Add_parent = ::Uml::CompositionParentRole::Create(comp);
			::Uml::InitCompositionParentRoleProps(AddCoordinateSystems::meta_Add_parent, "Add_parent", true);
			Add::meta_AddCoordinateSystems_child = ::Uml::CompositionChildRole::Create(comp);
			::Uml::InitCompositionChildRoleProps(Add::meta_AddCoordinateSystems_child, "AddCoordinateSystems", true, 0, 1);

		}
		{
			::Uml::Composition comp = ::Uml::Composition::Create(meta);
			::Uml::InitCompositionProps(comp, "Composition", false);
			AddDatums::meta_Add_parent = ::Uml::CompositionParentRole::Create(comp);
			::Uml::InitCompositionParentRoleProps(AddDatums::meta_Add_parent, "Add_parent", true);
			Add::meta_AddDatums_child = ::Uml::CompositionChildRole::Create(comp);
			::Uml::InitCompositionChildRoleProps(Add::meta_AddDatums_child, "AddDatums", true, 0, 1);

		}
		{
			::Uml::Composition comp = ::Uml::Composition::Create(meta);
			::Uml::InitCompositionProps(comp, "Composition", false);
			Libraries::meta_CADDatumEditor_parent = ::Uml::CompositionParentRole::Create(comp);
			::Uml::InitCompositionParentRoleProps(Libraries::meta_CADDatumEditor_parent, "CADDatumEditor_parent", true);
			CADDatumEditor::meta_Libraries_children = ::Uml::CompositionChildRole::Create(comp);
			::Uml::InitCompositionChildRoleProps(CADDatumEditor::meta_Libraries_children, "Libraries", true, 1, -1);

		}
		{
			::Uml::Composition comp = ::Uml::Composition::Create(meta);
			::Uml::InitCompositionProps(comp, "Composition", false);
			CADComponents::meta_CADDatumEditor_parent = ::Uml::CompositionParentRole::Create(comp);
			::Uml::InitCompositionParentRoleProps(CADComponents::meta_CADDatumEditor_parent, "CADDatumEditor_parent", true);
			CADDatumEditor::meta_CADComponents_children = ::Uml::CompositionChildRole::Create(comp);
			::Uml::InitCompositionChildRoleProps(CADDatumEditor::meta_CADComponents_children, "CADComponents", true, 1, -1);

		}
		{
			::Uml::Composition comp = ::Uml::Composition::Create(meta);
			::Uml::InitCompositionProps(comp, "Composition", false);
			AddCoordinateSystem::meta_AddCoordinateSystems_parent = ::Uml::CompositionParentRole::Create(comp);
			::Uml::InitCompositionParentRoleProps(AddCoordinateSystem::meta_AddCoordinateSystems_parent, "AddCoordinateSystems_parent", true);
			AddCoordinateSystems::meta_AddCoordinateSystem_children = ::Uml::CompositionChildRole::Create(comp);
			::Uml::InitCompositionChildRoleProps(AddCoordinateSystems::meta_AddCoordinateSystem_children, "AddCoordinateSystem", true, 1, -1);

		}
		{
			::Uml::Composition comp = ::Uml::Composition::Create(meta);
			::Uml::InitCompositionProps(comp, "Composition", false);
			Origin::meta_AddCoordinateSystem_parent = ::Uml::CompositionParentRole::Create(comp);
			::Uml::InitCompositionParentRoleProps(Origin::meta_AddCoordinateSystem_parent, "AddCoordinateSystem_parent", true);
			AddCoordinateSystem::meta_Origin_child = ::Uml::CompositionChildRole::Create(comp);
			::Uml::InitCompositionChildRoleProps(AddCoordinateSystem::meta_Origin_child, "Origin", true, 1, 1);

		}
		{
			::Uml::Composition comp = ::Uml::Composition::Create(meta);
			::Uml::InitCompositionProps(comp, "Composition", false);
			XVector::meta_AddCoordinateSystem_parent = ::Uml::CompositionParentRole::Create(comp);
			::Uml::InitCompositionParentRoleProps(XVector::meta_AddCoordinateSystem_parent, "AddCoordinateSystem_parent", true);
			AddCoordinateSystem::meta_XVector_child = ::Uml::CompositionChildRole::Create(comp);
			::Uml::InitCompositionChildRoleProps(AddCoordinateSystem::meta_XVector_child, "XVector", true, 1, 1);

		}
		{
			::Uml::Composition comp = ::Uml::Composition::Create(meta);
			::Uml::InitCompositionProps(comp, "Composition", false);
			YVector::meta_AddCoordinateSystem_parent = ::Uml::CompositionParentRole::Create(comp);
			::Uml::InitCompositionParentRoleProps(YVector::meta_AddCoordinateSystem_parent, "AddCoordinateSystem_parent", true);
			AddCoordinateSystem::meta_YVector_child = ::Uml::CompositionChildRole::Create(comp);
			::Uml::InitCompositionChildRoleProps(AddCoordinateSystem::meta_YVector_child, "YVector", true, 1, 1);

		}
		{
			::Uml::Composition comp = ::Uml::Composition::Create(meta);
			::Uml::InitCompositionProps(comp, "Composition", false);
			DeleteDatum::meta_DeleteDatums_parent = ::Uml::CompositionParentRole::Create(comp);
			::Uml::InitCompositionParentRoleProps(DeleteDatum::meta_DeleteDatums_parent, "DeleteDatums_parent", true);
			DeleteDatums::meta_DeleteDatum_children = ::Uml::CompositionChildRole::Create(comp);
			::Uml::InitCompositionChildRoleProps(DeleteDatums::meta_DeleteDatum_children, "DeleteDatum", true, 1, -1);

		}
		{
			::Uml::Composition comp = ::Uml::Composition::Create(meta);
			::Uml::InitCompositionProps(comp, "Composition", false);
			AddDatum::meta_AddDatums_parent = ::Uml::CompositionParentRole::Create(comp);
			::Uml::InitCompositionParentRoleProps(AddDatum::meta_AddDatums_parent, "AddDatums_parent", true);
			AddDatums::meta_AddDatum_children = ::Uml::CompositionChildRole::Create(comp);
			::Uml::InitCompositionChildRoleProps(AddDatums::meta_AddDatum_children, "AddDatum", true, 1, -1);

		}

	}

	void InitMetaLinks() {
		AddCoordinateSystems::meta_Add_parent.target() = Add::meta;
		AddDatums::meta_Add_parent.target() = Add::meta;
		CADComponent::meta_Add_child.target() = Add::meta;

		Origin::meta_AddCoordinateSystem_parent.target() = AddCoordinateSystem::meta;
		XVector::meta_AddCoordinateSystem_parent.target() = AddCoordinateSystem::meta;
		YVector::meta_AddCoordinateSystem_parent.target() = AddCoordinateSystem::meta;
		AddCoordinateSystems::meta_AddCoordinateSystem_children.target() = AddCoordinateSystem::meta;

		AddCoordinateSystem::meta_AddCoordinateSystems_parent.target() = AddCoordinateSystems::meta;
		Add::meta_AddCoordinateSystems_child.target() = AddCoordinateSystems::meta;

		AddDatums::meta_AddDatum_children.target() = AddDatum::meta;

		AddDatum::meta_AddDatums_parent.target() = AddDatums::meta;
		Add::meta_AddDatums_child.target() = AddDatums::meta;

		Delete::meta_CADComponent_parent.target() = CADComponent::meta;
		Add::meta_CADComponent_parent.target() = CADComponent::meta;
		CADComponents::meta_CADComponent_children.target() = CADComponent::meta;

		CADComponent::meta_CADComponents_parent.target() = CADComponents::meta;
		CADDatumEditor::meta_CADComponents_children.target() = CADComponents::meta;

		Libraries::meta_CADDatumEditor_parent.target() = CADDatumEditor::meta;
		CADComponents::meta_CADDatumEditor_parent.target() = CADDatumEditor::meta;

		CoordinateBase::meta.subTypes() += Origin::meta;
		CoordinateBase::meta.subTypes() += XVector::meta;
		CoordinateBase::meta.subTypes() += YVector::meta;

		DeleteDatums::meta_Delete_parent.target() = Delete::meta;
		CADComponent::meta_Delete_child.target() = Delete::meta;

		DeleteDatums::meta_DeleteDatum_children.target() = DeleteDatum::meta;

		DeleteDatum::meta_DeleteDatums_parent.target() = DeleteDatums::meta;
		Delete::meta_DeleteDatums_child.target() = DeleteDatums::meta;

		Library::meta_Libraries_parent.target() = Libraries::meta;
		CADDatumEditor::meta_Libraries_children.target() = Libraries::meta;

		Libraries::meta_Library_children.target() = Library::meta;

		AddCoordinateSystem::meta_Origin_child.target() = Origin::meta;

		AddCoordinateSystem::meta_XVector_child.target() = XVector::meta;

		AddCoordinateSystem::meta_YVector_child.target() = YVector::meta;

	}

	void InitMeta(const ::Uml::Diagram &parent) {
		// classes, with attributes, constraints and constraint definitions
		::Uml::SetClass(Add::meta, parent, "Add");

		::Uml::SetClass(AddCoordinateSystem::meta, parent, "AddCoordinateSystem");
		::Uml::SetAttribute(AddCoordinateSystem::meta_CoordinateSystemName, AddCoordinateSystem::meta, "CoordinateSystemName");
		::Uml::SetAttribute(AddCoordinateSystem::meta_ReplaceIfExists, AddCoordinateSystem::meta, "ReplaceIfExists");

		::Uml::SetClass(AddCoordinateSystems::meta, parent, "AddCoordinateSystems");

		::Uml::SetClass(AddDatum::meta, parent, "AddDatum");
		::Uml::SetAttribute(AddDatum::meta_DatumName, AddDatum::meta, "DatumName");
		::Uml::SetAttribute(AddDatum::meta_DatumType, AddDatum::meta, "DatumType");
		::Uml::SetAttribute(AddDatum::meta_ReplaceIfExists, AddDatum::meta, "ReplaceIfExists");
		::Uml::SetAttribute(AddDatum::meta_CoordinateSystemName, AddDatum::meta, "CoordinateSystemName");
		::Uml::SetAttribute(AddDatum::meta_CoordinateAlignment, AddDatum::meta, "CoordinateAlignment");
		::Uml::SetAttribute(AddDatum::meta_FlipDatumPlaneDirection, AddDatum::meta, "FlipDatumPlaneDirection");

		::Uml::SetClass(AddDatums::meta, parent, "AddDatums");

		::Uml::SetClass(CADComponent::meta, parent, "CADComponent");
		::Uml::SetAttribute(CADComponent::meta_Name, CADComponent::meta, "Name");
		::Uml::SetAttribute(CADComponent::meta_Type, CADComponent::meta, "Type");
		::Uml::SetAttribute(CADComponent::meta_LibraryID, CADComponent::meta, "LibraryID");
		::Uml::SetAttribute(CADComponent::meta_Format, CADComponent::meta, "Format");
		::Uml::SetAttribute(CADComponent::meta_File, CADComponent::meta, "File");

		::Uml::SetClass(CADComponents::meta, parent, "CADComponents");

		::Uml::SetClass(CADDatumEditor::meta, parent, "CADDatumEditor");

		::Uml::SetClass(CoordinateBase::meta, parent, "CoordinateBase");
		::Uml::SetAttribute(CoordinateBase::meta_X, CoordinateBase::meta, "X");
		::Uml::SetAttribute(CoordinateBase::meta_Y, CoordinateBase::meta, "Y");
		::Uml::SetAttribute(CoordinateBase::meta_Z, CoordinateBase::meta, "Z");

		::Uml::SetClass(Delete::meta, parent, "Delete");

		::Uml::SetClass(DeleteDatum::meta, parent, "DeleteDatum");
		::Uml::SetAttribute(DeleteDatum::meta_DatumName, DeleteDatum::meta, "DatumName");
		::Uml::SetAttribute(DeleteDatum::meta_DatumType, DeleteDatum::meta, "DatumType");

		::Uml::SetClass(DeleteDatums::meta, parent, "DeleteDatums");

		::Uml::SetClass(Libraries::meta, parent, "Libraries");

		::Uml::SetClass(Library::meta, parent, "Library");
		::Uml::SetAttribute(Library::meta_ID, Library::meta, "ID");
		::Uml::SetAttribute(Library::meta_DirectoryPath, Library::meta, "DirectoryPath");

		::Uml::SetClass(Origin::meta, parent, "Origin");

		::Uml::SetClass(XVector::meta, parent, "XVector");

		::Uml::SetClass(YVector::meta, parent, "YVector");

	}

	void InitMetaLinks(const ::Uml::Diagram &parent) {
		// classes
		::Uml::SetChildRole(Add::meta_AddCoordinateSystems_child, Add::meta, AddCoordinateSystems::meta, "", "");
		::Uml::SetChildRole(Add::meta_AddDatums_child, Add::meta, AddDatums::meta, "", "");
		::Uml::SetParentRole(Add::meta_CADComponent_parent, Add::meta, CADComponent::meta, "", "");

		::Uml::SetChildRole(AddCoordinateSystem::meta_Origin_child, AddCoordinateSystem::meta, Origin::meta, "", "");
		::Uml::SetChildRole(AddCoordinateSystem::meta_XVector_child, AddCoordinateSystem::meta, XVector::meta, "", "");
		::Uml::SetChildRole(AddCoordinateSystem::meta_YVector_child, AddCoordinateSystem::meta, YVector::meta, "", "");
		::Uml::SetParentRole(AddCoordinateSystem::meta_AddCoordinateSystems_parent, AddCoordinateSystem::meta, AddCoordinateSystems::meta, "", "");

		::Uml::SetChildRole(AddCoordinateSystems::meta_AddCoordinateSystem_children, AddCoordinateSystems::meta, AddCoordinateSystem::meta, "", "");
		::Uml::SetParentRole(AddCoordinateSystems::meta_Add_parent, AddCoordinateSystems::meta, Add::meta, "", "");

		::Uml::SetParentRole(AddDatum::meta_AddDatums_parent, AddDatum::meta, AddDatums::meta, "", "");

		::Uml::SetChildRole(AddDatums::meta_AddDatum_children, AddDatums::meta, AddDatum::meta, "", "");
		::Uml::SetParentRole(AddDatums::meta_Add_parent, AddDatums::meta, Add::meta, "", "");

		::Uml::SetChildRole(CADComponent::meta_Delete_child, CADComponent::meta, Delete::meta, "", "");
		::Uml::SetChildRole(CADComponent::meta_Add_child, CADComponent::meta, Add::meta, "", "");
		::Uml::SetParentRole(CADComponent::meta_CADComponents_parent, CADComponent::meta, CADComponents::meta, "", "");

		::Uml::SetChildRole(CADComponents::meta_CADComponent_children, CADComponents::meta, CADComponent::meta, "", "");
		::Uml::SetParentRole(CADComponents::meta_CADDatumEditor_parent, CADComponents::meta, CADDatumEditor::meta, "", "");

		::Uml::SetChildRole(CADDatumEditor::meta_Libraries_children, CADDatumEditor::meta, Libraries::meta, "", "");
		::Uml::SetChildRole(CADDatumEditor::meta_CADComponents_children, CADDatumEditor::meta, CADComponents::meta, "", "");

		::Uml::SetChildRole(Delete::meta_DeleteDatums_child, Delete::meta, DeleteDatums::meta, "", "");
		::Uml::SetParentRole(Delete::meta_CADComponent_parent, Delete::meta, CADComponent::meta, "", "");

		::Uml::SetParentRole(DeleteDatum::meta_DeleteDatums_parent, DeleteDatum::meta, DeleteDatums::meta, "", "");

		::Uml::SetChildRole(DeleteDatums::meta_DeleteDatum_children, DeleteDatums::meta, DeleteDatum::meta, "", "");
		::Uml::SetParentRole(DeleteDatums::meta_Delete_parent, DeleteDatums::meta, Delete::meta, "", "");

		::Uml::SetChildRole(Libraries::meta_Library_children, Libraries::meta, Library::meta, "", "");
		::Uml::SetParentRole(Libraries::meta_CADDatumEditor_parent, Libraries::meta, CADDatumEditor::meta, "", "");

		::Uml::SetParentRole(Library::meta_Libraries_parent, Library::meta, Libraries::meta, "", "");

		::Uml::SetParentRole(Origin::meta_AddCoordinateSystem_parent, Origin::meta, AddCoordinateSystem::meta, "", "");

		::Uml::SetParentRole(XVector::meta_AddCoordinateSystem_parent, XVector::meta, AddCoordinateSystem::meta, "", "");

		::Uml::SetParentRole(YVector::meta_AddCoordinateSystem_parent, YVector::meta, AddCoordinateSystem::meta, "", "");

	}

	void _SetXsdStorage()
	{
		UdmDom::str_xsd_storage::StoreXsd("CADDatumEditor.xsd", CADDatumEditor_xsd::getString());
	}

	void Initialize()
	{
		static bool first = true;
		if (!first) return;
		first = false;
		::Uml::Initialize();

	
		UDM_ASSERT( meta == ::Udm::null );

		::UdmStatic::StaticDataNetwork * meta_dn = new ::UdmStatic::StaticDataNetwork(::Uml::diagram);
		meta_dn->CreateNew("CADDatumEditor.mem", "", ::Uml::Diagram::meta, ::Udm::CHANGES_LOST_DEFAULT);
		meta = ::Uml::Diagram::Cast(meta_dn->GetRootObject());

		::Uml::InitDiagramProps(meta, "CADDatumEditor", "1.00");


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


	 ::Udm::UdmDiagram diagram = { &meta, Initialize };
	static struct _regClass
	{
		_regClass()
		{
			::Udm::MetaDepository::StoreDiagram("CADDatumEditor", diagram);
		}
		~_regClass()
		{
			::Udm::MetaDepository::RemoveDiagram("CADDatumEditor");
		}
	} __regUnUsed;

}

