#ifndef MOBIES_CADDATUMEDITOR_H
#define MOBIES_CADDATUMEDITOR_H

// header file CADDatumEditor.h generated from diagram CADDatumEditor
// generated with Udm version 3.31 on Tue Dec 11 11:41:38 2012

#include <UdmBase.h>

#if !defined(UDM_VERSION_MAJOR) || !defined(UDM_VERSION_MINOR)
#    error "Udm headers too old, they do not define UDM_VERSION"
#elif UDM_VERSION_MAJOR < 3
#    error "Udm headers too old, minimum version required 3.31"
#elif UDM_VERSION_MAJOR == 3 && UDM_VERSION_MINOR < 31
#    error "Udm headers too old, minimum version required 3.31"
#endif

#include <Uml.h>


#ifdef min
#undef min
#endif

#ifdef max
#undef max
#endif

namespace CADDatumEditor {

	extern ::Uml::Diagram meta;
	class CADComponents;
	class Libraries;
	class CADComponent;
	class Library;
	class Delete;
	class Add;
	class DeleteDatum;
	class AddDatum;
	class CADDatumEditor;
	class Origin;
	class AddCoordinateSystems;
	class AddCoordinateSystem;
	class CoordinateBase;
	class DeleteDatums;
	class AddDatums;
	class XVector;
	class YVector;

	void Initialize();
	void Initialize(const ::Uml::Diagram &dgr);

	extern  ::Udm::UdmDiagram diagram;

	class CADComponents : public ::Udm::Object {
	public:
		CADComponents();
		CADComponents(::Udm::ObjectImpl *impl);
		CADComponents(const CADComponents &master);

#ifdef UDM_RVALUE
		CADComponents(CADComponents &&master);

		static CADComponents Cast(::Udm::Object &&a);
		CADComponents& operator=(CADComponents &&a);

#endif
		static CADComponents Cast(const ::Udm::Object &a);
		static CADComponents Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		CADComponents CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::InstantiatedAttr< CADComponents> Instances();
		template <class Pred> ::Udm::InstantiatedAttr< CADComponents, Pred> Instances_sorted(const Pred &) { return ::Udm::InstantiatedAttr< CADComponents, Pred>(impl); };
		CADComponents CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::DerivedAttr< CADComponents> Derived();
		template <class Pred> ::Udm::DerivedAttr< CADComponents, Pred> Derived_sorted(const Pred &) { return ::Udm::DerivedAttr< CADComponents, Pred>(impl); };
		::Udm::ArchetypeAttr< CADComponents> Archetype() const;
		::Udm::ChildrenAttr< ::CADDatumEditor::CADComponent> CADComponent_children() const;
		template <class Pred> ::Udm::ChildrenAttr< ::CADDatumEditor::CADComponent, Pred> CADComponent_children_sorted(const Pred &) const { return ::Udm::ChildrenAttr< ::CADDatumEditor::CADComponent, Pred>(impl, meta_CADComponent_children); };
		::Udm::ChildrenAttr< ::CADDatumEditor::CADComponent> CADComponent_kind_children() const;
		template <class Pred> ::Udm::ChildrenAttr< ::CADDatumEditor::CADComponent, Pred> CADComponent_kind_children_sorted(const Pred &) const { return ::Udm::ChildrenAttr< ::CADDatumEditor::CADComponent, Pred>(impl, ::Udm::NULLCHILDROLE); };
		::Udm::ParentAttr< ::CADDatumEditor::CADDatumEditor> CADDatumEditor_parent() const;
		::Udm::ParentAttr< ::CADDatumEditor::CADDatumEditor> parent() const;

		static ::Uml::Class meta;
		static ::Uml::CompositionChildRole meta_CADComponent_children;
		static ::Uml::CompositionParentRole meta_CADDatumEditor_parent;

	};

	class Libraries : public ::Udm::Object {
	public:
		Libraries();
		Libraries(::Udm::ObjectImpl *impl);
		Libraries(const Libraries &master);

#ifdef UDM_RVALUE
		Libraries(Libraries &&master);

		static Libraries Cast(::Udm::Object &&a);
		Libraries& operator=(Libraries &&a);

#endif
		static Libraries Cast(const ::Udm::Object &a);
		static Libraries Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		Libraries CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::InstantiatedAttr< Libraries> Instances();
		template <class Pred> ::Udm::InstantiatedAttr< Libraries, Pred> Instances_sorted(const Pred &) { return ::Udm::InstantiatedAttr< Libraries, Pred>(impl); };
		Libraries CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::DerivedAttr< Libraries> Derived();
		template <class Pred> ::Udm::DerivedAttr< Libraries, Pred> Derived_sorted(const Pred &) { return ::Udm::DerivedAttr< Libraries, Pred>(impl); };
		::Udm::ArchetypeAttr< Libraries> Archetype() const;
		::Udm::ChildrenAttr< ::CADDatumEditor::Library> Library_children() const;
		template <class Pred> ::Udm::ChildrenAttr< ::CADDatumEditor::Library, Pred> Library_children_sorted(const Pred &) const { return ::Udm::ChildrenAttr< ::CADDatumEditor::Library, Pred>(impl, meta_Library_children); };
		::Udm::ChildrenAttr< ::CADDatumEditor::Library> Library_kind_children() const;
		template <class Pred> ::Udm::ChildrenAttr< ::CADDatumEditor::Library, Pred> Library_kind_children_sorted(const Pred &) const { return ::Udm::ChildrenAttr< ::CADDatumEditor::Library, Pred>(impl, ::Udm::NULLCHILDROLE); };
		::Udm::ParentAttr< ::CADDatumEditor::CADDatumEditor> CADDatumEditor_parent() const;
		::Udm::ParentAttr< ::CADDatumEditor::CADDatumEditor> parent() const;

		static ::Uml::Class meta;
		static ::Uml::CompositionChildRole meta_Library_children;
		static ::Uml::CompositionParentRole meta_CADDatumEditor_parent;

	};

	class CADComponent : public ::Udm::Object {
	public:
		CADComponent();
		CADComponent(::Udm::ObjectImpl *impl);
		CADComponent(const CADComponent &master);

#ifdef UDM_RVALUE
		CADComponent(CADComponent &&master);

		static CADComponent Cast(::Udm::Object &&a);
		CADComponent& operator=(CADComponent &&a);

#endif
		static CADComponent Cast(const ::Udm::Object &a);
		static CADComponent Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		CADComponent CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::InstantiatedAttr< CADComponent> Instances();
		template <class Pred> ::Udm::InstantiatedAttr< CADComponent, Pred> Instances_sorted(const Pred &) { return ::Udm::InstantiatedAttr< CADComponent, Pred>(impl); };
		CADComponent CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::DerivedAttr< CADComponent> Derived();
		template <class Pred> ::Udm::DerivedAttr< CADComponent, Pred> Derived_sorted(const Pred &) { return ::Udm::DerivedAttr< CADComponent, Pred>(impl); };
		::Udm::ArchetypeAttr< CADComponent> Archetype() const;
		::Udm::StringAttr Name() const;
		::Udm::StringAttr Type() const;
		::Udm::StringAttr LibraryID() const;
		::Udm::StringAttr Format() const;
		::Udm::StringAttr File() const;
		::Udm::ChildAttr< ::CADDatumEditor::Delete> Delete_child() const;
		::Udm::ChildAttr< ::CADDatumEditor::Add> Add_child() const;
		::Udm::ChildrenAttr< ::CADDatumEditor::Delete> Delete_kind_children() const;
		template <class Pred> ::Udm::ChildrenAttr< ::CADDatumEditor::Delete, Pred> Delete_kind_children_sorted(const Pred &) const { return ::Udm::ChildrenAttr< ::CADDatumEditor::Delete, Pred>(impl, ::Udm::NULLCHILDROLE); };
		::Udm::ChildrenAttr< ::CADDatumEditor::Add> Add_kind_children() const;
		template <class Pred> ::Udm::ChildrenAttr< ::CADDatumEditor::Add, Pred> Add_kind_children_sorted(const Pred &) const { return ::Udm::ChildrenAttr< ::CADDatumEditor::Add, Pred>(impl, ::Udm::NULLCHILDROLE); };
		::Udm::ParentAttr< ::CADDatumEditor::CADComponents> CADComponents_parent() const;
		::Udm::ParentAttr< ::CADDatumEditor::CADComponents> parent() const;

		static ::Uml::Class meta;
		static ::Uml::Attribute meta_Name;
		static ::Uml::Attribute meta_Type;
		static ::Uml::Attribute meta_LibraryID;
		static ::Uml::Attribute meta_Format;
		static ::Uml::Attribute meta_File;
		static ::Uml::CompositionChildRole meta_Delete_child;
		static ::Uml::CompositionChildRole meta_Add_child;
		static ::Uml::CompositionParentRole meta_CADComponents_parent;

	};

	class Library : public ::Udm::Object {
	public:
		Library();
		Library(::Udm::ObjectImpl *impl);
		Library(const Library &master);

#ifdef UDM_RVALUE
		Library(Library &&master);

		static Library Cast(::Udm::Object &&a);
		Library& operator=(Library &&a);

#endif
		static Library Cast(const ::Udm::Object &a);
		static Library Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		Library CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::InstantiatedAttr< Library> Instances();
		template <class Pred> ::Udm::InstantiatedAttr< Library, Pred> Instances_sorted(const Pred &) { return ::Udm::InstantiatedAttr< Library, Pred>(impl); };
		Library CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::DerivedAttr< Library> Derived();
		template <class Pred> ::Udm::DerivedAttr< Library, Pred> Derived_sorted(const Pred &) { return ::Udm::DerivedAttr< Library, Pred>(impl); };
		::Udm::ArchetypeAttr< Library> Archetype() const;
		::Udm::StringAttr ID() const;
		::Udm::StringAttr DirectoryPath() const;
		::Udm::ParentAttr< ::CADDatumEditor::Libraries> Libraries_parent() const;
		::Udm::ParentAttr< ::CADDatumEditor::Libraries> parent() const;

		static ::Uml::Class meta;
		static ::Uml::Attribute meta_ID;
		static ::Uml::Attribute meta_DirectoryPath;
		static ::Uml::CompositionParentRole meta_Libraries_parent;

	};

	class Delete : public ::Udm::Object {
	public:
		Delete();
		Delete(::Udm::ObjectImpl *impl);
		Delete(const Delete &master);

#ifdef UDM_RVALUE
		Delete(Delete &&master);

		static Delete Cast(::Udm::Object &&a);
		Delete& operator=(Delete &&a);

#endif
		static Delete Cast(const ::Udm::Object &a);
		static Delete Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		Delete CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::InstantiatedAttr< Delete> Instances();
		template <class Pred> ::Udm::InstantiatedAttr< Delete, Pred> Instances_sorted(const Pred &) { return ::Udm::InstantiatedAttr< Delete, Pred>(impl); };
		Delete CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::DerivedAttr< Delete> Derived();
		template <class Pred> ::Udm::DerivedAttr< Delete, Pred> Derived_sorted(const Pred &) { return ::Udm::DerivedAttr< Delete, Pred>(impl); };
		::Udm::ArchetypeAttr< Delete> Archetype() const;
		::Udm::ChildAttr< ::CADDatumEditor::DeleteDatums> DeleteDatums_child() const;
		::Udm::ChildrenAttr< ::CADDatumEditor::DeleteDatums> DeleteDatums_kind_children() const;
		template <class Pred> ::Udm::ChildrenAttr< ::CADDatumEditor::DeleteDatums, Pred> DeleteDatums_kind_children_sorted(const Pred &) const { return ::Udm::ChildrenAttr< ::CADDatumEditor::DeleteDatums, Pred>(impl, ::Udm::NULLCHILDROLE); };
		::Udm::ParentAttr< ::CADDatumEditor::CADComponent> CADComponent_parent() const;
		::Udm::ParentAttr< ::CADDatumEditor::CADComponent> parent() const;

		static ::Uml::Class meta;
		static ::Uml::CompositionChildRole meta_DeleteDatums_child;
		static ::Uml::CompositionParentRole meta_CADComponent_parent;

	};

	class Add : public ::Udm::Object {
	public:
		Add();
		Add(::Udm::ObjectImpl *impl);
		Add(const Add &master);

#ifdef UDM_RVALUE
		Add(Add &&master);

		static Add Cast(::Udm::Object &&a);
		Add& operator=(Add &&a);

#endif
		static Add Cast(const ::Udm::Object &a);
		static Add Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		Add CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::InstantiatedAttr< Add> Instances();
		template <class Pred> ::Udm::InstantiatedAttr< Add, Pred> Instances_sorted(const Pred &) { return ::Udm::InstantiatedAttr< Add, Pred>(impl); };
		Add CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::DerivedAttr< Add> Derived();
		template <class Pred> ::Udm::DerivedAttr< Add, Pred> Derived_sorted(const Pred &) { return ::Udm::DerivedAttr< Add, Pred>(impl); };
		::Udm::ArchetypeAttr< Add> Archetype() const;
		::Udm::ChildAttr< ::CADDatumEditor::AddCoordinateSystems> AddCoordinateSystems_child() const;
		::Udm::ChildAttr< ::CADDatumEditor::AddDatums> AddDatums_child() const;
		::Udm::ChildrenAttr< ::CADDatumEditor::AddCoordinateSystems> AddCoordinateSystems_kind_children() const;
		template <class Pred> ::Udm::ChildrenAttr< ::CADDatumEditor::AddCoordinateSystems, Pred> AddCoordinateSystems_kind_children_sorted(const Pred &) const { return ::Udm::ChildrenAttr< ::CADDatumEditor::AddCoordinateSystems, Pred>(impl, ::Udm::NULLCHILDROLE); };
		::Udm::ChildrenAttr< ::CADDatumEditor::AddDatums> AddDatums_kind_children() const;
		template <class Pred> ::Udm::ChildrenAttr< ::CADDatumEditor::AddDatums, Pred> AddDatums_kind_children_sorted(const Pred &) const { return ::Udm::ChildrenAttr< ::CADDatumEditor::AddDatums, Pred>(impl, ::Udm::NULLCHILDROLE); };
		::Udm::ParentAttr< ::CADDatumEditor::CADComponent> CADComponent_parent() const;
		::Udm::ParentAttr< ::CADDatumEditor::CADComponent> parent() const;

		static ::Uml::Class meta;
		static ::Uml::CompositionChildRole meta_AddCoordinateSystems_child;
		static ::Uml::CompositionChildRole meta_AddDatums_child;
		static ::Uml::CompositionParentRole meta_CADComponent_parent;

	};

	class DeleteDatum : public ::Udm::Object {
	public:
		DeleteDatum();
		DeleteDatum(::Udm::ObjectImpl *impl);
		DeleteDatum(const DeleteDatum &master);

#ifdef UDM_RVALUE
		DeleteDatum(DeleteDatum &&master);

		static DeleteDatum Cast(::Udm::Object &&a);
		DeleteDatum& operator=(DeleteDatum &&a);

#endif
		static DeleteDatum Cast(const ::Udm::Object &a);
		static DeleteDatum Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		DeleteDatum CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::InstantiatedAttr< DeleteDatum> Instances();
		template <class Pred> ::Udm::InstantiatedAttr< DeleteDatum, Pred> Instances_sorted(const Pred &) { return ::Udm::InstantiatedAttr< DeleteDatum, Pred>(impl); };
		DeleteDatum CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::DerivedAttr< DeleteDatum> Derived();
		template <class Pred> ::Udm::DerivedAttr< DeleteDatum, Pred> Derived_sorted(const Pred &) { return ::Udm::DerivedAttr< DeleteDatum, Pred>(impl); };
		::Udm::ArchetypeAttr< DeleteDatum> Archetype() const;
		::Udm::StringAttr DatumName() const;
		::Udm::StringAttr DatumType() const;
		::Udm::ParentAttr< ::CADDatumEditor::DeleteDatums> DeleteDatums_parent() const;
		::Udm::ParentAttr< ::CADDatumEditor::DeleteDatums> parent() const;

		static ::Uml::Class meta;
		static ::Uml::Attribute meta_DatumName;
		static ::Uml::Attribute meta_DatumType;
		static ::Uml::CompositionParentRole meta_DeleteDatums_parent;

	};

	class AddDatum : public ::Udm::Object {
	public:
		AddDatum();
		AddDatum(::Udm::ObjectImpl *impl);
		AddDatum(const AddDatum &master);

#ifdef UDM_RVALUE
		AddDatum(AddDatum &&master);

		static AddDatum Cast(::Udm::Object &&a);
		AddDatum& operator=(AddDatum &&a);

#endif
		static AddDatum Cast(const ::Udm::Object &a);
		static AddDatum Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		AddDatum CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::InstantiatedAttr< AddDatum> Instances();
		template <class Pred> ::Udm::InstantiatedAttr< AddDatum, Pred> Instances_sorted(const Pred &) { return ::Udm::InstantiatedAttr< AddDatum, Pred>(impl); };
		AddDatum CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::DerivedAttr< AddDatum> Derived();
		template <class Pred> ::Udm::DerivedAttr< AddDatum, Pred> Derived_sorted(const Pred &) { return ::Udm::DerivedAttr< AddDatum, Pred>(impl); };
		::Udm::ArchetypeAttr< AddDatum> Archetype() const;
		::Udm::StringAttr DatumName() const;
		::Udm::StringAttr DatumType() const;
		::Udm::StringAttr ReplaceIfExists() const;
		::Udm::StringAttr CoordinateSystemName() const;
		::Udm::StringAttr CoordinateAlignment() const;
		::Udm::StringAttr FlipDatumPlaneDirection() const;
		::Udm::ParentAttr< ::CADDatumEditor::AddDatums> AddDatums_parent() const;
		::Udm::ParentAttr< ::CADDatumEditor::AddDatums> parent() const;

		static ::Uml::Class meta;
		static ::Uml::Attribute meta_DatumName;
		static ::Uml::Attribute meta_DatumType;
		static ::Uml::Attribute meta_ReplaceIfExists;
		static ::Uml::Attribute meta_CoordinateSystemName;
		static ::Uml::Attribute meta_CoordinateAlignment;
		static ::Uml::Attribute meta_FlipDatumPlaneDirection;
		static ::Uml::CompositionParentRole meta_AddDatums_parent;

	};

	class CADDatumEditor : public ::Udm::Object {
	public:
		CADDatumEditor();
		CADDatumEditor(::Udm::ObjectImpl *impl);
		CADDatumEditor(const CADDatumEditor &master);

#ifdef UDM_RVALUE
		CADDatumEditor(CADDatumEditor &&master);

		static CADDatumEditor Cast(::Udm::Object &&a);
		CADDatumEditor& operator=(CADDatumEditor &&a);

#endif
		static CADDatumEditor Cast(const ::Udm::Object &a);
		static CADDatumEditor Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		CADDatumEditor CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::InstantiatedAttr< CADDatumEditor> Instances();
		template <class Pred> ::Udm::InstantiatedAttr< CADDatumEditor, Pred> Instances_sorted(const Pred &) { return ::Udm::InstantiatedAttr< CADDatumEditor, Pred>(impl); };
		CADDatumEditor CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::DerivedAttr< CADDatumEditor> Derived();
		template <class Pred> ::Udm::DerivedAttr< CADDatumEditor, Pred> Derived_sorted(const Pred &) { return ::Udm::DerivedAttr< CADDatumEditor, Pred>(impl); };
		::Udm::ArchetypeAttr< CADDatumEditor> Archetype() const;
		::Udm::ChildrenAttr< ::CADDatumEditor::Libraries> Libraries_children() const;
		template <class Pred> ::Udm::ChildrenAttr< ::CADDatumEditor::Libraries, Pred> Libraries_children_sorted(const Pred &) const { return ::Udm::ChildrenAttr< ::CADDatumEditor::Libraries, Pred>(impl, meta_Libraries_children); };
		::Udm::ChildrenAttr< ::CADDatumEditor::CADComponents> CADComponents_children() const;
		template <class Pred> ::Udm::ChildrenAttr< ::CADDatumEditor::CADComponents, Pred> CADComponents_children_sorted(const Pred &) const { return ::Udm::ChildrenAttr< ::CADDatumEditor::CADComponents, Pred>(impl, meta_CADComponents_children); };
		::Udm::ChildrenAttr< ::CADDatumEditor::CADComponents> CADComponents_kind_children() const;
		template <class Pred> ::Udm::ChildrenAttr< ::CADDatumEditor::CADComponents, Pred> CADComponents_kind_children_sorted(const Pred &) const { return ::Udm::ChildrenAttr< ::CADDatumEditor::CADComponents, Pred>(impl, ::Udm::NULLCHILDROLE); };
		::Udm::ChildrenAttr< ::CADDatumEditor::Libraries> Libraries_kind_children() const;
		template <class Pred> ::Udm::ChildrenAttr< ::CADDatumEditor::Libraries, Pred> Libraries_kind_children_sorted(const Pred &) const { return ::Udm::ChildrenAttr< ::CADDatumEditor::Libraries, Pred>(impl, ::Udm::NULLCHILDROLE); };
		::Udm::ParentAttr< ::Udm::Object> parent() const;

		static ::Uml::Class meta;
		static ::Uml::CompositionChildRole meta_Libraries_children;
		static ::Uml::CompositionChildRole meta_CADComponents_children;

	};

	class AddCoordinateSystems : public ::Udm::Object {
	public:
		AddCoordinateSystems();
		AddCoordinateSystems(::Udm::ObjectImpl *impl);
		AddCoordinateSystems(const AddCoordinateSystems &master);

#ifdef UDM_RVALUE
		AddCoordinateSystems(AddCoordinateSystems &&master);

		static AddCoordinateSystems Cast(::Udm::Object &&a);
		AddCoordinateSystems& operator=(AddCoordinateSystems &&a);

#endif
		static AddCoordinateSystems Cast(const ::Udm::Object &a);
		static AddCoordinateSystems Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		AddCoordinateSystems CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::InstantiatedAttr< AddCoordinateSystems> Instances();
		template <class Pred> ::Udm::InstantiatedAttr< AddCoordinateSystems, Pred> Instances_sorted(const Pred &) { return ::Udm::InstantiatedAttr< AddCoordinateSystems, Pred>(impl); };
		AddCoordinateSystems CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::DerivedAttr< AddCoordinateSystems> Derived();
		template <class Pred> ::Udm::DerivedAttr< AddCoordinateSystems, Pred> Derived_sorted(const Pred &) { return ::Udm::DerivedAttr< AddCoordinateSystems, Pred>(impl); };
		::Udm::ArchetypeAttr< AddCoordinateSystems> Archetype() const;
		::Udm::ChildrenAttr< ::CADDatumEditor::AddCoordinateSystem> AddCoordinateSystem_children() const;
		template <class Pred> ::Udm::ChildrenAttr< ::CADDatumEditor::AddCoordinateSystem, Pred> AddCoordinateSystem_children_sorted(const Pred &) const { return ::Udm::ChildrenAttr< ::CADDatumEditor::AddCoordinateSystem, Pred>(impl, meta_AddCoordinateSystem_children); };
		::Udm::ChildrenAttr< ::CADDatumEditor::AddCoordinateSystem> AddCoordinateSystem_kind_children() const;
		template <class Pred> ::Udm::ChildrenAttr< ::CADDatumEditor::AddCoordinateSystem, Pred> AddCoordinateSystem_kind_children_sorted(const Pred &) const { return ::Udm::ChildrenAttr< ::CADDatumEditor::AddCoordinateSystem, Pred>(impl, ::Udm::NULLCHILDROLE); };
		::Udm::ParentAttr< ::CADDatumEditor::Add> Add_parent() const;
		::Udm::ParentAttr< ::CADDatumEditor::Add> parent() const;

		static ::Uml::Class meta;
		static ::Uml::CompositionChildRole meta_AddCoordinateSystem_children;
		static ::Uml::CompositionParentRole meta_Add_parent;

	};

	class AddCoordinateSystem : public ::Udm::Object {
	public:
		AddCoordinateSystem();
		AddCoordinateSystem(::Udm::ObjectImpl *impl);
		AddCoordinateSystem(const AddCoordinateSystem &master);

#ifdef UDM_RVALUE
		AddCoordinateSystem(AddCoordinateSystem &&master);

		static AddCoordinateSystem Cast(::Udm::Object &&a);
		AddCoordinateSystem& operator=(AddCoordinateSystem &&a);

#endif
		static AddCoordinateSystem Cast(const ::Udm::Object &a);
		static AddCoordinateSystem Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		AddCoordinateSystem CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::InstantiatedAttr< AddCoordinateSystem> Instances();
		template <class Pred> ::Udm::InstantiatedAttr< AddCoordinateSystem, Pred> Instances_sorted(const Pred &) { return ::Udm::InstantiatedAttr< AddCoordinateSystem, Pred>(impl); };
		AddCoordinateSystem CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::DerivedAttr< AddCoordinateSystem> Derived();
		template <class Pred> ::Udm::DerivedAttr< AddCoordinateSystem, Pred> Derived_sorted(const Pred &) { return ::Udm::DerivedAttr< AddCoordinateSystem, Pred>(impl); };
		::Udm::ArchetypeAttr< AddCoordinateSystem> Archetype() const;
		::Udm::StringAttr CoordinateSystemName() const;
		::Udm::StringAttr ReplaceIfExists() const;
		::Udm::ChildAttr< ::CADDatumEditor::Origin> Origin_child() const;
		::Udm::ChildAttr< ::CADDatumEditor::XVector> XVector_child() const;
		::Udm::ChildAttr< ::CADDatumEditor::YVector> YVector_child() const;
		::Udm::ChildrenAttr< ::CADDatumEditor::Origin> Origin_kind_children() const;
		template <class Pred> ::Udm::ChildrenAttr< ::CADDatumEditor::Origin, Pred> Origin_kind_children_sorted(const Pred &) const { return ::Udm::ChildrenAttr< ::CADDatumEditor::Origin, Pred>(impl, ::Udm::NULLCHILDROLE); };
		::Udm::ChildrenAttr< ::CADDatumEditor::CoordinateBase> CoordinateBase_kind_children() const;
		template <class Pred> ::Udm::ChildrenAttr< ::CADDatumEditor::CoordinateBase, Pred> CoordinateBase_kind_children_sorted(const Pred &) const { return ::Udm::ChildrenAttr< ::CADDatumEditor::CoordinateBase, Pred>(impl, ::Udm::NULLCHILDROLE); };
		::Udm::ChildrenAttr< ::CADDatumEditor::XVector> XVector_kind_children() const;
		template <class Pred> ::Udm::ChildrenAttr< ::CADDatumEditor::XVector, Pred> XVector_kind_children_sorted(const Pred &) const { return ::Udm::ChildrenAttr< ::CADDatumEditor::XVector, Pred>(impl, ::Udm::NULLCHILDROLE); };
		::Udm::ChildrenAttr< ::CADDatumEditor::YVector> YVector_kind_children() const;
		template <class Pred> ::Udm::ChildrenAttr< ::CADDatumEditor::YVector, Pred> YVector_kind_children_sorted(const Pred &) const { return ::Udm::ChildrenAttr< ::CADDatumEditor::YVector, Pred>(impl, ::Udm::NULLCHILDROLE); };
		::Udm::ParentAttr< ::CADDatumEditor::AddCoordinateSystems> AddCoordinateSystems_parent() const;
		::Udm::ParentAttr< ::CADDatumEditor::AddCoordinateSystems> parent() const;

		static ::Uml::Class meta;
		static ::Uml::Attribute meta_CoordinateSystemName;
		static ::Uml::Attribute meta_ReplaceIfExists;
		static ::Uml::CompositionChildRole meta_Origin_child;
		static ::Uml::CompositionChildRole meta_XVector_child;
		static ::Uml::CompositionChildRole meta_YVector_child;
		static ::Uml::CompositionParentRole meta_AddCoordinateSystems_parent;

	};

	class CoordinateBase : public ::Udm::Object {
	public:
		CoordinateBase();
		CoordinateBase(::Udm::ObjectImpl *impl);
		CoordinateBase(const CoordinateBase &master);

#ifdef UDM_RVALUE
		CoordinateBase(CoordinateBase &&master);

		static CoordinateBase Cast(::Udm::Object &&a);
		CoordinateBase& operator=(CoordinateBase &&a);

#endif
		static CoordinateBase Cast(const ::Udm::Object &a);
		static CoordinateBase Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		CoordinateBase CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::InstantiatedAttr< CoordinateBase> Instances();
		template <class Pred> ::Udm::InstantiatedAttr< CoordinateBase, Pred> Instances_sorted(const Pred &) { return ::Udm::InstantiatedAttr< CoordinateBase, Pred>(impl); };
		CoordinateBase CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::DerivedAttr< CoordinateBase> Derived();
		template <class Pred> ::Udm::DerivedAttr< CoordinateBase, Pred> Derived_sorted(const Pred &) { return ::Udm::DerivedAttr< CoordinateBase, Pred>(impl); };
		::Udm::ArchetypeAttr< CoordinateBase> Archetype() const;
		::Udm::RealAttr X() const;
		::Udm::RealAttr Y() const;
		::Udm::RealAttr Z() const;
		::Udm::ParentAttr< ::CADDatumEditor::AddCoordinateSystem> parent() const;

		static ::Uml::Class meta;
		static ::Uml::Attribute meta_X;
		static ::Uml::Attribute meta_Y;
		static ::Uml::Attribute meta_Z;

	};

	class Origin :  public CoordinateBase {
	public:
		Origin();
		Origin(::Udm::ObjectImpl *impl);
		Origin(const Origin &master);

#ifdef UDM_RVALUE
		Origin(Origin &&master);

		static Origin Cast(::Udm::Object &&a);
		Origin& operator=(Origin &&a);

#endif
		static Origin Cast(const ::Udm::Object &a);
		static Origin Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		Origin CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::InstantiatedAttr< Origin> Instances();
		template <class Pred> ::Udm::InstantiatedAttr< Origin, Pred> Instances_sorted(const Pred &) { return ::Udm::InstantiatedAttr< Origin, Pred>(impl); };
		Origin CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::DerivedAttr< Origin> Derived();
		template <class Pred> ::Udm::DerivedAttr< Origin, Pred> Derived_sorted(const Pred &) { return ::Udm::DerivedAttr< Origin, Pred>(impl); };
		::Udm::ArchetypeAttr< Origin> Archetype() const;
		::Udm::ParentAttr< ::CADDatumEditor::AddCoordinateSystem> AddCoordinateSystem_parent() const;
		::Udm::ParentAttr< ::CADDatumEditor::AddCoordinateSystem> parent() const;

		static ::Uml::Class meta;
		static ::Uml::CompositionParentRole meta_AddCoordinateSystem_parent;

	};

	class DeleteDatums : public ::Udm::Object {
	public:
		DeleteDatums();
		DeleteDatums(::Udm::ObjectImpl *impl);
		DeleteDatums(const DeleteDatums &master);

#ifdef UDM_RVALUE
		DeleteDatums(DeleteDatums &&master);

		static DeleteDatums Cast(::Udm::Object &&a);
		DeleteDatums& operator=(DeleteDatums &&a);

#endif
		static DeleteDatums Cast(const ::Udm::Object &a);
		static DeleteDatums Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		DeleteDatums CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::InstantiatedAttr< DeleteDatums> Instances();
		template <class Pred> ::Udm::InstantiatedAttr< DeleteDatums, Pred> Instances_sorted(const Pred &) { return ::Udm::InstantiatedAttr< DeleteDatums, Pred>(impl); };
		DeleteDatums CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::DerivedAttr< DeleteDatums> Derived();
		template <class Pred> ::Udm::DerivedAttr< DeleteDatums, Pred> Derived_sorted(const Pred &) { return ::Udm::DerivedAttr< DeleteDatums, Pred>(impl); };
		::Udm::ArchetypeAttr< DeleteDatums> Archetype() const;
		::Udm::ChildrenAttr< ::CADDatumEditor::DeleteDatum> DeleteDatum_children() const;
		template <class Pred> ::Udm::ChildrenAttr< ::CADDatumEditor::DeleteDatum, Pred> DeleteDatum_children_sorted(const Pred &) const { return ::Udm::ChildrenAttr< ::CADDatumEditor::DeleteDatum, Pred>(impl, meta_DeleteDatum_children); };
		::Udm::ChildrenAttr< ::CADDatumEditor::DeleteDatum> DeleteDatum_kind_children() const;
		template <class Pred> ::Udm::ChildrenAttr< ::CADDatumEditor::DeleteDatum, Pred> DeleteDatum_kind_children_sorted(const Pred &) const { return ::Udm::ChildrenAttr< ::CADDatumEditor::DeleteDatum, Pred>(impl, ::Udm::NULLCHILDROLE); };
		::Udm::ParentAttr< ::CADDatumEditor::Delete> Delete_parent() const;
		::Udm::ParentAttr< ::CADDatumEditor::Delete> parent() const;

		static ::Uml::Class meta;
		static ::Uml::CompositionChildRole meta_DeleteDatum_children;
		static ::Uml::CompositionParentRole meta_Delete_parent;

	};

	class AddDatums : public ::Udm::Object {
	public:
		AddDatums();
		AddDatums(::Udm::ObjectImpl *impl);
		AddDatums(const AddDatums &master);

#ifdef UDM_RVALUE
		AddDatums(AddDatums &&master);

		static AddDatums Cast(::Udm::Object &&a);
		AddDatums& operator=(AddDatums &&a);

#endif
		static AddDatums Cast(const ::Udm::Object &a);
		static AddDatums Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		AddDatums CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::InstantiatedAttr< AddDatums> Instances();
		template <class Pred> ::Udm::InstantiatedAttr< AddDatums, Pred> Instances_sorted(const Pred &) { return ::Udm::InstantiatedAttr< AddDatums, Pred>(impl); };
		AddDatums CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::DerivedAttr< AddDatums> Derived();
		template <class Pred> ::Udm::DerivedAttr< AddDatums, Pred> Derived_sorted(const Pred &) { return ::Udm::DerivedAttr< AddDatums, Pred>(impl); };
		::Udm::ArchetypeAttr< AddDatums> Archetype() const;
		::Udm::ChildrenAttr< ::CADDatumEditor::AddDatum> AddDatum_children() const;
		template <class Pred> ::Udm::ChildrenAttr< ::CADDatumEditor::AddDatum, Pred> AddDatum_children_sorted(const Pred &) const { return ::Udm::ChildrenAttr< ::CADDatumEditor::AddDatum, Pred>(impl, meta_AddDatum_children); };
		::Udm::ChildrenAttr< ::CADDatumEditor::AddDatum> AddDatum_kind_children() const;
		template <class Pred> ::Udm::ChildrenAttr< ::CADDatumEditor::AddDatum, Pred> AddDatum_kind_children_sorted(const Pred &) const { return ::Udm::ChildrenAttr< ::CADDatumEditor::AddDatum, Pred>(impl, ::Udm::NULLCHILDROLE); };
		::Udm::ParentAttr< ::CADDatumEditor::Add> Add_parent() const;
		::Udm::ParentAttr< ::CADDatumEditor::Add> parent() const;

		static ::Uml::Class meta;
		static ::Uml::CompositionChildRole meta_AddDatum_children;
		static ::Uml::CompositionParentRole meta_Add_parent;

	};

	class XVector :  public CoordinateBase {
	public:
		XVector();
		XVector(::Udm::ObjectImpl *impl);
		XVector(const XVector &master);

#ifdef UDM_RVALUE
		XVector(XVector &&master);

		static XVector Cast(::Udm::Object &&a);
		XVector& operator=(XVector &&a);

#endif
		static XVector Cast(const ::Udm::Object &a);
		static XVector Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		XVector CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::InstantiatedAttr< XVector> Instances();
		template <class Pred> ::Udm::InstantiatedAttr< XVector, Pred> Instances_sorted(const Pred &) { return ::Udm::InstantiatedAttr< XVector, Pred>(impl); };
		XVector CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::DerivedAttr< XVector> Derived();
		template <class Pred> ::Udm::DerivedAttr< XVector, Pred> Derived_sorted(const Pred &) { return ::Udm::DerivedAttr< XVector, Pred>(impl); };
		::Udm::ArchetypeAttr< XVector> Archetype() const;
		::Udm::ParentAttr< ::CADDatumEditor::AddCoordinateSystem> AddCoordinateSystem_parent() const;
		::Udm::ParentAttr< ::CADDatumEditor::AddCoordinateSystem> parent() const;

		static ::Uml::Class meta;
		static ::Uml::CompositionParentRole meta_AddCoordinateSystem_parent;

	};

	class YVector :  public CoordinateBase {
	public:
		YVector();
		YVector(::Udm::ObjectImpl *impl);
		YVector(const YVector &master);

#ifdef UDM_RVALUE
		YVector(YVector &&master);

		static YVector Cast(::Udm::Object &&a);
		YVector& operator=(YVector &&a);

#endif
		static YVector Cast(const ::Udm::Object &a);
		static YVector Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		YVector CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::InstantiatedAttr< YVector> Instances();
		template <class Pred> ::Udm::InstantiatedAttr< YVector, Pred> Instances_sorted(const Pred &) { return ::Udm::InstantiatedAttr< YVector, Pred>(impl); };
		YVector CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::DerivedAttr< YVector> Derived();
		template <class Pred> ::Udm::DerivedAttr< YVector, Pred> Derived_sorted(const Pred &) { return ::Udm::DerivedAttr< YVector, Pred>(impl); };
		::Udm::ArchetypeAttr< YVector> Archetype() const;
		::Udm::ParentAttr< ::CADDatumEditor::AddCoordinateSystem> AddCoordinateSystem_parent() const;
		::Udm::ParentAttr< ::CADDatumEditor::AddCoordinateSystem> parent() const;

		static ::Uml::Class meta;
		static ::Uml::CompositionParentRole meta_AddCoordinateSystem_parent;

	};

}

#endif // MOBIES_CADDATUMEDITOR_H
