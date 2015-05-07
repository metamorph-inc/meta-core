// cpp (meta datanetwork format) source file OIL.cpp
// generated from diagram OIL
// generated on Wed Oct 29 11:21:54 2014

#include "OIL.h"
#include <UmlExt.h>
#include <UdmStatic.h>

using namespace std;

namespace OIL {

	OILFile::OILFile() {}
	OILFile::OILFile(::Udm::ObjectImpl *impl) : UDM_OBJECT(impl) {}
	OILFile::OILFile(const OILFile &master) : UDM_OBJECT(master) {}

#ifdef UDM_RVALUE
	OILFile::OILFile(OILFile &&master) : UDM_OBJECT(master) {};

	OILFile OILFile::Cast(::Udm::Object &&a) { return __Cast(std::move(a), meta); };
	OILFile& OILFile::operator=(OILFile &&a) { ::Udm::Object::operator =(std::move(a)); return *this; };

#endif
	OILFile OILFile::Cast(const ::Udm::Object &a) { return __Cast(a, meta); }
	OILFile OILFile::Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role); }
	OILFile OILFile::CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl); }
	::Udm::InstantiatedAttr< OILFile> OILFile::Instances() { return ::Udm::InstantiatedAttr< OILFile>(impl); }
	OILFile OILFile::CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl, true); }
	::Udm::DerivedAttr< OILFile> OILFile::Derived() { return ::Udm::DerivedAttr< OILFile>(impl); }
	::Udm::ArchetypeAttr< OILFile> OILFile::Archetype() const { return ::Udm::ArchetypeAttr< OILFile>(impl); }
	::Udm::StringAttr OILFile::filename() const { return ::Udm::StringAttr(impl, meta_filename); }
	::Udm::ChildAttr< ::OIL::CPU> OILFile::CPU_child() const { return ::Udm::ChildAttr< ::OIL::CPU>(impl, meta_CPU_child); }
	::Udm::ChildAttr< ::OIL::OIL_VERSION> OILFile::OIL_VERSION_child() const { return ::Udm::ChildAttr< ::OIL::OIL_VERSION>(impl, meta_OIL_VERSION_child); }
	::Udm::ChildrenAttr< ::OIL::Include> OILFile::Include_children() const { return ::Udm::ChildrenAttr< ::OIL::Include>(impl, meta_Include_children); }
	::Udm::ChildrenAttr< ::OIL::OIL_VERSION> OILFile::OIL_VERSION_kind_children() const { return ::Udm::ChildrenAttr< ::OIL::OIL_VERSION>(impl, ::Udm::NULLCHILDROLE); }
	::Udm::ChildrenAttr< ::OIL::Include> OILFile::Include_kind_children() const { return ::Udm::ChildrenAttr< ::OIL::Include>(impl, ::Udm::NULLCHILDROLE); }
	::Udm::ChildrenAttr< ::OIL::CPU> OILFile::CPU_kind_children() const { return ::Udm::ChildrenAttr< ::OIL::CPU>(impl, ::Udm::NULLCHILDROLE); }
	::Udm::ParentAttr< ::Udm::Object> OILFile::parent() const { return ::Udm::ParentAttr< ::Udm::Object>(impl, ::Udm::NULLPARENTROLE); }

	::Uml::Class OILFile::meta;
	::Uml::Attribute OILFile::meta_filename;
	::Uml::CompositionChildRole OILFile::meta_CPU_child;
	::Uml::CompositionChildRole OILFile::meta_OIL_VERSION_child;
	::Uml::CompositionChildRole OILFile::meta_Include_children;

	OIL_VERSION::OIL_VERSION() {}
	OIL_VERSION::OIL_VERSION(::Udm::ObjectImpl *impl) : UDM_OBJECT(impl) {}
	OIL_VERSION::OIL_VERSION(const OIL_VERSION &master) : UDM_OBJECT(master) {}

#ifdef UDM_RVALUE
	OIL_VERSION::OIL_VERSION(OIL_VERSION &&master) : UDM_OBJECT(master) {};

	OIL_VERSION OIL_VERSION::Cast(::Udm::Object &&a) { return __Cast(std::move(a), meta); };
	OIL_VERSION& OIL_VERSION::operator=(OIL_VERSION &&a) { ::Udm::Object::operator =(std::move(a)); return *this; };

#endif
	OIL_VERSION OIL_VERSION::Cast(const ::Udm::Object &a) { return __Cast(a, meta); }
	OIL_VERSION OIL_VERSION::Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role); }
	OIL_VERSION OIL_VERSION::CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl); }
	::Udm::InstantiatedAttr< OIL_VERSION> OIL_VERSION::Instances() { return ::Udm::InstantiatedAttr< OIL_VERSION>(impl); }
	OIL_VERSION OIL_VERSION::CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl, true); }
	::Udm::DerivedAttr< OIL_VERSION> OIL_VERSION::Derived() { return ::Udm::DerivedAttr< OIL_VERSION>(impl); }
	::Udm::ArchetypeAttr< OIL_VERSION> OIL_VERSION::Archetype() const { return ::Udm::ArchetypeAttr< OIL_VERSION>(impl); }
	::Udm::StringAttr OIL_VERSION::text() const { return ::Udm::StringAttr(impl, meta_text); }
	::Udm::ParentAttr< ::OIL::OILFile> OIL_VERSION::OILFile_parent() const { return ::Udm::ParentAttr< ::OIL::OILFile>(impl, meta_OILFile_parent); }
	::Udm::ParentAttr< ::OIL::OILFile> OIL_VERSION::parent() const { return ::Udm::ParentAttr< ::OIL::OILFile>(impl, ::Udm::NULLPARENTROLE); }

	::Uml::Class OIL_VERSION::meta;
	::Uml::Attribute OIL_VERSION::meta_text;
	::Uml::CompositionParentRole OIL_VERSION::meta_OILFile_parent;

	Include::Include() {}
	Include::Include(::Udm::ObjectImpl *impl) : UDM_OBJECT(impl) {}
	Include::Include(const Include &master) : UDM_OBJECT(master) {}

#ifdef UDM_RVALUE
	Include::Include(Include &&master) : UDM_OBJECT(master) {};

	Include Include::Cast(::Udm::Object &&a) { return __Cast(std::move(a), meta); };
	Include& Include::operator=(Include &&a) { ::Udm::Object::operator =(std::move(a)); return *this; };

#endif
	Include Include::Cast(const ::Udm::Object &a) { return __Cast(a, meta); }
	Include Include::Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role); }
	Include Include::CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl); }
	::Udm::InstantiatedAttr< Include> Include::Instances() { return ::Udm::InstantiatedAttr< Include>(impl); }
	Include Include::CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl, true); }
	::Udm::DerivedAttr< Include> Include::Derived() { return ::Udm::DerivedAttr< Include>(impl); }
	::Udm::ArchetypeAttr< Include> Include::Archetype() const { return ::Udm::ArchetypeAttr< Include>(impl); }
	::Udm::StringAttr Include::filename() const { return ::Udm::StringAttr(impl, meta_filename); }
	::Udm::ParentAttr< ::OIL::OILFile> Include::OILFile_parent() const { return ::Udm::ParentAttr< ::OIL::OILFile>(impl, meta_OILFile_parent); }
	::Udm::ParentAttr< ::OIL::CPU> Include::CPU_parent() const { return ::Udm::ParentAttr< ::OIL::CPU>(impl, meta_CPU_parent); }
	::Udm::ParentAttr< ::Udm::Object> Include::parent() const { return ::Udm::ParentAttr< ::Udm::Object>(impl, ::Udm::NULLPARENTROLE); }

	::Uml::Class Include::meta;
	::Uml::Attribute Include::meta_filename;
	::Uml::CompositionParentRole Include::meta_OILFile_parent;
	::Uml::CompositionParentRole Include::meta_CPU_parent;

	CPU::CPU() {}
	CPU::CPU(::Udm::ObjectImpl *impl) : UDM_OBJECT(impl) {}
	CPU::CPU(const CPU &master) : UDM_OBJECT(master) {}

#ifdef UDM_RVALUE
	CPU::CPU(CPU &&master) : UDM_OBJECT(master) {};

	CPU CPU::Cast(::Udm::Object &&a) { return __Cast(std::move(a), meta); };
	CPU& CPU::operator=(CPU &&a) { ::Udm::Object::operator =(std::move(a)); return *this; };

#endif
	CPU CPU::Cast(const ::Udm::Object &a) { return __Cast(a, meta); }
	CPU CPU::Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role); }
	CPU CPU::CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl); }
	::Udm::InstantiatedAttr< CPU> CPU::Instances() { return ::Udm::InstantiatedAttr< CPU>(impl); }
	CPU CPU::CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl, true); }
	::Udm::DerivedAttr< CPU> CPU::Derived() { return ::Udm::DerivedAttr< CPU>(impl); }
	::Udm::ArchetypeAttr< CPU> CPU::Archetype() const { return ::Udm::ArchetypeAttr< CPU>(impl); }
	::Udm::StringAttr CPU::name() const { return ::Udm::StringAttr(impl, meta_name); }
	::Udm::ChildrenAttr< ::OIL::Include> CPU::Include_children() const { return ::Udm::ChildrenAttr< ::OIL::Include>(impl, meta_Include_children); }
	::Udm::ChildrenAttr< ::OIL::ALARM> CPU::ALARM_children() const { return ::Udm::ChildrenAttr< ::OIL::ALARM>(impl, meta_ALARM_children); }
	::Udm::ChildrenAttr< ::OIL::EVENT> CPU::EVENT_children() const { return ::Udm::ChildrenAttr< ::OIL::EVENT>(impl, meta_EVENT_children); }
	::Udm::ChildrenAttr< ::OIL::TASK> CPU::TASK_children() const { return ::Udm::ChildrenAttr< ::OIL::TASK>(impl, meta_TASK_children); }
	::Udm::ChildAttr< ::OIL::OIL_COM> CPU::OIL_COM_child() const { return ::Udm::ChildAttr< ::OIL::OIL_COM>(impl, meta_OIL_COM_child); }
	::Udm::ChildAttr< ::OIL::OIL_OS> CPU::OIL_OS_child() const { return ::Udm::ChildAttr< ::OIL::OIL_OS>(impl, meta_OIL_OS_child); }
	::Udm::ChildrenAttr< ::OIL::Include> CPU::Include_kind_children() const { return ::Udm::ChildrenAttr< ::OIL::Include>(impl, ::Udm::NULLCHILDROLE); }
	::Udm::ChildrenAttr< ::OIL::OIL_OS> CPU::OIL_OS_kind_children() const { return ::Udm::ChildrenAttr< ::OIL::OIL_OS>(impl, ::Udm::NULLCHILDROLE); }
	::Udm::ChildrenAttr< ::OIL::ALARM> CPU::ALARM_kind_children() const { return ::Udm::ChildrenAttr< ::OIL::ALARM>(impl, ::Udm::NULLCHILDROLE); }
	::Udm::ChildrenAttr< ::OIL::EVENT> CPU::EVENT_kind_children() const { return ::Udm::ChildrenAttr< ::OIL::EVENT>(impl, ::Udm::NULLCHILDROLE); }
	::Udm::ChildrenAttr< ::OIL::TASK> CPU::TASK_kind_children() const { return ::Udm::ChildrenAttr< ::OIL::TASK>(impl, ::Udm::NULLCHILDROLE); }
	::Udm::ChildrenAttr< ::OIL::OIL_COM> CPU::OIL_COM_kind_children() const { return ::Udm::ChildrenAttr< ::OIL::OIL_COM>(impl, ::Udm::NULLCHILDROLE); }
	::Udm::ParentAttr< ::OIL::OILFile> CPU::OILFile_parent() const { return ::Udm::ParentAttr< ::OIL::OILFile>(impl, meta_OILFile_parent); }
	::Udm::ParentAttr< ::OIL::OILFile> CPU::parent() const { return ::Udm::ParentAttr< ::OIL::OILFile>(impl, ::Udm::NULLPARENTROLE); }

	::Uml::Class CPU::meta;
	::Uml::Attribute CPU::meta_name;
	::Uml::CompositionChildRole CPU::meta_Include_children;
	::Uml::CompositionChildRole CPU::meta_ALARM_children;
	::Uml::CompositionChildRole CPU::meta_EVENT_children;
	::Uml::CompositionChildRole CPU::meta_TASK_children;
	::Uml::CompositionChildRole CPU::meta_OIL_COM_child;
	::Uml::CompositionChildRole CPU::meta_OIL_OS_child;
	::Uml::CompositionParentRole CPU::meta_OILFile_parent;

	OIL_OS::OIL_OS() {}
	OIL_OS::OIL_OS(::Udm::ObjectImpl *impl) : UDM_OBJECT(impl) {}
	OIL_OS::OIL_OS(const OIL_OS &master) : UDM_OBJECT(master) {}

#ifdef UDM_RVALUE
	OIL_OS::OIL_OS(OIL_OS &&master) : UDM_OBJECT(master) {};

	OIL_OS OIL_OS::Cast(::Udm::Object &&a) { return __Cast(std::move(a), meta); };
	OIL_OS& OIL_OS::operator=(OIL_OS &&a) { ::Udm::Object::operator =(std::move(a)); return *this; };

#endif
	OIL_OS OIL_OS::Cast(const ::Udm::Object &a) { return __Cast(a, meta); }
	OIL_OS OIL_OS::Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role); }
	OIL_OS OIL_OS::CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl); }
	::Udm::InstantiatedAttr< OIL_OS> OIL_OS::Instances() { return ::Udm::InstantiatedAttr< OIL_OS>(impl); }
	OIL_OS OIL_OS::CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl, true); }
	::Udm::DerivedAttr< OIL_OS> OIL_OS::Derived() { return ::Udm::DerivedAttr< OIL_OS>(impl); }
	::Udm::ArchetypeAttr< OIL_OS> OIL_OS::Archetype() const { return ::Udm::ArchetypeAttr< OIL_OS>(impl); }
	::Udm::StringAttr OIL_OS::name() const { return ::Udm::StringAttr(impl, meta_name); }
	::Udm::StringAttr OIL_OS::comment() const { return ::Udm::StringAttr(impl, meta_comment); }
	::Udm::StringAttr OIL_OS::CC() const { return ::Udm::StringAttr(impl, meta_CC); }
	::Udm::StringAttr OIL_OS::STATUS() const { return ::Udm::StringAttr(impl, meta_STATUS); }
	::Udm::StringAttr OIL_OS::SCHEDULE() const { return ::Udm::StringAttr(impl, meta_SCHEDULE); }
	::Udm::BooleanAttr OIL_OS::STARTUPHOOK() const { return ::Udm::BooleanAttr(impl, meta_STARTUPHOOK); }
	::Udm::BooleanAttr OIL_OS::SHUTDOWNHOOK() const { return ::Udm::BooleanAttr(impl, meta_SHUTDOWNHOOK); }
	::Udm::BooleanAttr OIL_OS::ERRORHOOK() const { return ::Udm::BooleanAttr(impl, meta_ERRORHOOK); }
	::Udm::BooleanAttr OIL_OS::PRETASKHOOK() const { return ::Udm::BooleanAttr(impl, meta_PRETASKHOOK); }
	::Udm::BooleanAttr OIL_OS::POSTTASKHOOK() const { return ::Udm::BooleanAttr(impl, meta_POSTTASKHOOK); }
	::Udm::BooleanAttr OIL_OS::USEGETSERVICEID() const { return ::Udm::BooleanAttr(impl, meta_USEGETSERVICEID); }
	::Udm::BooleanAttr OIL_OS::USEPARAMETERACCESS() const { return ::Udm::BooleanAttr(impl, meta_USEPARAMETERACCESS); }
	::Udm::IntegerAttr OIL_OS::TickTime() const { return ::Udm::IntegerAttr(impl, meta_TickTime); }
	::Udm::ParentAttr< ::OIL::CPU> OIL_OS::CPU_parent() const { return ::Udm::ParentAttr< ::OIL::CPU>(impl, meta_CPU_parent); }
	::Udm::ParentAttr< ::OIL::CPU> OIL_OS::parent() const { return ::Udm::ParentAttr< ::OIL::CPU>(impl, ::Udm::NULLPARENTROLE); }

	::Uml::Class OIL_OS::meta;
	::Uml::Attribute OIL_OS::meta_name;
	::Uml::Attribute OIL_OS::meta_comment;
	::Uml::Attribute OIL_OS::meta_CC;
	::Uml::Attribute OIL_OS::meta_STATUS;
	::Uml::Attribute OIL_OS::meta_SCHEDULE;
	::Uml::Attribute OIL_OS::meta_STARTUPHOOK;
	::Uml::Attribute OIL_OS::meta_SHUTDOWNHOOK;
	::Uml::Attribute OIL_OS::meta_ERRORHOOK;
	::Uml::Attribute OIL_OS::meta_PRETASKHOOK;
	::Uml::Attribute OIL_OS::meta_POSTTASKHOOK;
	::Uml::Attribute OIL_OS::meta_USEGETSERVICEID;
	::Uml::Attribute OIL_OS::meta_USEPARAMETERACCESS;
	::Uml::Attribute OIL_OS::meta_TickTime;
	::Uml::CompositionParentRole OIL_OS::meta_CPU_parent;

	Procedure::Procedure() {}
	Procedure::Procedure(::Udm::ObjectImpl *impl) : UDM_OBJECT(impl) {}
	Procedure::Procedure(const Procedure &master) : UDM_OBJECT(master) {}

#ifdef UDM_RVALUE
	Procedure::Procedure(Procedure &&master) : UDM_OBJECT(master) {};

	Procedure Procedure::Cast(::Udm::Object &&a) { return __Cast(std::move(a), meta); };
	Procedure& Procedure::operator=(Procedure &&a) { ::Udm::Object::operator =(std::move(a)); return *this; };

#endif
	Procedure Procedure::Cast(const ::Udm::Object &a) { return __Cast(a, meta); }
	Procedure Procedure::Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role); }
	Procedure Procedure::CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl); }
	::Udm::InstantiatedAttr< Procedure> Procedure::Instances() { return ::Udm::InstantiatedAttr< Procedure>(impl); }
	Procedure Procedure::CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl, true); }
	::Udm::DerivedAttr< Procedure> Procedure::Derived() { return ::Udm::DerivedAttr< Procedure>(impl); }
	::Udm::ArchetypeAttr< Procedure> Procedure::Archetype() const { return ::Udm::ArchetypeAttr< Procedure>(impl); }
	::Udm::StringAttr Procedure::name() const { return ::Udm::StringAttr(impl, meta_name); }
	::Udm::PointerAttr< EVENT> Procedure::event() const { return ::Udm::PointerAttr< EVENT>(impl, meta_event); }
	::Udm::ParentAttr< ::OIL::TASK> Procedure::TASK_parent() const { return ::Udm::ParentAttr< ::OIL::TASK>(impl, meta_TASK_parent); }
	::Udm::ParentAttr< ::OIL::TASK> Procedure::parent() const { return ::Udm::ParentAttr< ::OIL::TASK>(impl, ::Udm::NULLPARENTROLE); }

	::Uml::Class Procedure::meta;
	::Uml::Attribute Procedure::meta_name;
	::Uml::AssociationRole Procedure::meta_event;
	::Uml::CompositionParentRole Procedure::meta_TASK_parent;

	ALARM::ALARM() {}
	ALARM::ALARM(::Udm::ObjectImpl *impl) : UDM_OBJECT(impl) {}
	ALARM::ALARM(const ALARM &master) : UDM_OBJECT(master) {}

#ifdef UDM_RVALUE
	ALARM::ALARM(ALARM &&master) : UDM_OBJECT(master) {};

	ALARM ALARM::Cast(::Udm::Object &&a) { return __Cast(std::move(a), meta); };
	ALARM& ALARM::operator=(ALARM &&a) { ::Udm::Object::operator =(std::move(a)); return *this; };

#endif
	ALARM ALARM::Cast(const ::Udm::Object &a) { return __Cast(a, meta); }
	ALARM ALARM::Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role); }
	ALARM ALARM::CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl); }
	::Udm::InstantiatedAttr< ALARM> ALARM::Instances() { return ::Udm::InstantiatedAttr< ALARM>(impl); }
	ALARM ALARM::CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl, true); }
	::Udm::DerivedAttr< ALARM> ALARM::Derived() { return ::Udm::DerivedAttr< ALARM>(impl); }
	::Udm::ArchetypeAttr< ALARM> ALARM::Archetype() const { return ::Udm::ArchetypeAttr< ALARM>(impl); }
	::Udm::StringAttr ALARM::Component() const { return ::Udm::StringAttr(impl, meta_Component); }
	::Udm::StringAttr ALARM::name() const { return ::Udm::StringAttr(impl, meta_name); }
	::Udm::StringAttr ALARM::COUNTER() const { return ::Udm::StringAttr(impl, meta_COUNTER); }
	::Udm::StringAttr ALARM::ACTION() const { return ::Udm::StringAttr(impl, meta_ACTION); }
	::Udm::BooleanAttr ALARM::AUTOSTART() const { return ::Udm::BooleanAttr(impl, meta_AUTOSTART); }
	::Udm::IntegerAttr ALARM::ALARMTIME() const { return ::Udm::IntegerAttr(impl, meta_ALARMTIME); }
	::Udm::IntegerAttr ALARM::CYCLETIME() const { return ::Udm::IntegerAttr(impl, meta_CYCLETIME); }
	::Udm::StringAttr ALARM::APPMODE() const { return ::Udm::StringAttr(impl, meta_APPMODE); }
	::Udm::StringAttr ALARM::AlarmUnit() const { return ::Udm::StringAttr(impl, meta_AlarmUnit); }
	::Udm::BooleanAttr ALARM::StaticAlarm() const { return ::Udm::BooleanAttr(impl, meta_StaticAlarm); }
	::Udm::PointerAttr< EVENT> ALARM::event() const { return ::Udm::PointerAttr< EVENT>(impl, meta_event); }
	::Udm::ParentAttr< ::OIL::CPU> ALARM::CPU_parent() const { return ::Udm::ParentAttr< ::OIL::CPU>(impl, meta_CPU_parent); }
	::Udm::ParentAttr< ::OIL::CPU> ALARM::parent() const { return ::Udm::ParentAttr< ::OIL::CPU>(impl, ::Udm::NULLPARENTROLE); }

	::Uml::Class ALARM::meta;
	::Uml::Attribute ALARM::meta_Component;
	::Uml::Attribute ALARM::meta_name;
	::Uml::Attribute ALARM::meta_COUNTER;
	::Uml::Attribute ALARM::meta_ACTION;
	::Uml::Attribute ALARM::meta_AUTOSTART;
	::Uml::Attribute ALARM::meta_ALARMTIME;
	::Uml::Attribute ALARM::meta_CYCLETIME;
	::Uml::Attribute ALARM::meta_APPMODE;
	::Uml::Attribute ALARM::meta_AlarmUnit;
	::Uml::Attribute ALARM::meta_StaticAlarm;
	::Uml::AssociationRole ALARM::meta_event;
	::Uml::CompositionParentRole ALARM::meta_CPU_parent;

	EVENT::EVENT() {}
	EVENT::EVENT(::Udm::ObjectImpl *impl) : UDM_OBJECT(impl) {}
	EVENT::EVENT(const EVENT &master) : UDM_OBJECT(master) {}

#ifdef UDM_RVALUE
	EVENT::EVENT(EVENT &&master) : UDM_OBJECT(master) {};

	EVENT EVENT::Cast(::Udm::Object &&a) { return __Cast(std::move(a), meta); };
	EVENT& EVENT::operator=(EVENT &&a) { ::Udm::Object::operator =(std::move(a)); return *this; };

#endif
	EVENT EVENT::Cast(const ::Udm::Object &a) { return __Cast(a, meta); }
	EVENT EVENT::Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role); }
	EVENT EVENT::CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl); }
	::Udm::InstantiatedAttr< EVENT> EVENT::Instances() { return ::Udm::InstantiatedAttr< EVENT>(impl); }
	EVENT EVENT::CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl, true); }
	::Udm::DerivedAttr< EVENT> EVENT::Derived() { return ::Udm::DerivedAttr< EVENT>(impl); }
	::Udm::ArchetypeAttr< EVENT> EVENT::Archetype() const { return ::Udm::ArchetypeAttr< EVENT>(impl); }
	::Udm::StringAttr EVENT::Component() const { return ::Udm::StringAttr(impl, meta_Component); }
	::Udm::StringAttr EVENT::name() const { return ::Udm::StringAttr(impl, meta_name); }
	::Udm::StringAttr EVENT::MASK() const { return ::Udm::StringAttr(impl, meta_MASK); }
	::Udm::PointerAttr< ALARM> EVENT::alarm() const { return ::Udm::PointerAttr< ALARM>(impl, meta_alarm); }
	::Udm::AssocAttr< Procedure> EVENT::procs() const { return ::Udm::AssocAttr< Procedure>(impl, meta_procs); }
	::Udm::PointerAttr< TASK> EVENT::ownerTask() const { return ::Udm::PointerAttr< TASK>(impl, meta_ownerTask); }
	::Udm::ParentAttr< ::OIL::CPU> EVENT::CPU_parent() const { return ::Udm::ParentAttr< ::OIL::CPU>(impl, meta_CPU_parent); }
	::Udm::ParentAttr< ::OIL::CPU> EVENT::parent() const { return ::Udm::ParentAttr< ::OIL::CPU>(impl, ::Udm::NULLPARENTROLE); }

	::Uml::Class EVENT::meta;
	::Uml::Attribute EVENT::meta_Component;
	::Uml::Attribute EVENT::meta_name;
	::Uml::Attribute EVENT::meta_MASK;
	::Uml::AssociationRole EVENT::meta_alarm;
	::Uml::AssociationRole EVENT::meta_procs;
	::Uml::AssociationRole EVENT::meta_ownerTask;
	::Uml::CompositionParentRole EVENT::meta_CPU_parent;

	TASK::TASK() {}
	TASK::TASK(::Udm::ObjectImpl *impl) : UDM_OBJECT(impl) {}
	TASK::TASK(const TASK &master) : UDM_OBJECT(master) {}

#ifdef UDM_RVALUE
	TASK::TASK(TASK &&master) : UDM_OBJECT(master) {};

	TASK TASK::Cast(::Udm::Object &&a) { return __Cast(std::move(a), meta); };
	TASK& TASK::operator=(TASK &&a) { ::Udm::Object::operator =(std::move(a)); return *this; };

#endif
	TASK TASK::Cast(const ::Udm::Object &a) { return __Cast(a, meta); }
	TASK TASK::Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role); }
	TASK TASK::CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl); }
	::Udm::InstantiatedAttr< TASK> TASK::Instances() { return ::Udm::InstantiatedAttr< TASK>(impl); }
	TASK TASK::CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl, true); }
	::Udm::DerivedAttr< TASK> TASK::Derived() { return ::Udm::DerivedAttr< TASK>(impl); }
	::Udm::ArchetypeAttr< TASK> TASK::Archetype() const { return ::Udm::ArchetypeAttr< TASK>(impl); }
	::Udm::StringAttr TASK::Component() const { return ::Udm::StringAttr(impl, meta_Component); }
	::Udm::StringAttr TASK::name() const { return ::Udm::StringAttr(impl, meta_name); }
	::Udm::StringAttr TASK::TYPE() const { return ::Udm::StringAttr(impl, meta_TYPE); }
	::Udm::StringAttr TASK::SCHEDULE() const { return ::Udm::StringAttr(impl, meta_SCHEDULE); }
	::Udm::IntegerAttr TASK::PRIORITY() const { return ::Udm::IntegerAttr(impl, meta_PRIORITY); }
	::Udm::IntegerAttr TASK::ACTIVATION() const { return ::Udm::IntegerAttr(impl, meta_ACTIVATION); }
	::Udm::BooleanAttr TASK::AUTOSTART() const { return ::Udm::BooleanAttr(impl, meta_AUTOSTART); }
	::Udm::StringAttr TASK::APPMODE() const { return ::Udm::StringAttr(impl, meta_APPMODE); }
	::Udm::AssocAttr< EVENT> TASK::events() const { return ::Udm::AssocAttr< EVENT>(impl, meta_events); }
	::Udm::ChildAttr< ::OIL::Procedure> TASK::Procedure_child() const { return ::Udm::ChildAttr< ::OIL::Procedure>(impl, meta_Procedure_child); }
	::Udm::ChildrenAttr< ::OIL::Procedure> TASK::Procedure_kind_children() const { return ::Udm::ChildrenAttr< ::OIL::Procedure>(impl, ::Udm::NULLCHILDROLE); }
	::Udm::ParentAttr< ::OIL::CPU> TASK::CPU_parent() const { return ::Udm::ParentAttr< ::OIL::CPU>(impl, meta_CPU_parent); }
	::Udm::ParentAttr< ::OIL::CPU> TASK::parent() const { return ::Udm::ParentAttr< ::OIL::CPU>(impl, ::Udm::NULLPARENTROLE); }

	::Uml::Class TASK::meta;
	::Uml::Attribute TASK::meta_Component;
	::Uml::Attribute TASK::meta_name;
	::Uml::Attribute TASK::meta_TYPE;
	::Uml::Attribute TASK::meta_SCHEDULE;
	::Uml::Attribute TASK::meta_PRIORITY;
	::Uml::Attribute TASK::meta_ACTIVATION;
	::Uml::Attribute TASK::meta_AUTOSTART;
	::Uml::Attribute TASK::meta_APPMODE;
	::Uml::AssociationRole TASK::meta_events;
	::Uml::CompositionChildRole TASK::meta_Procedure_child;
	::Uml::CompositionParentRole TASK::meta_CPU_parent;

	OIL_COM::OIL_COM() {}
	OIL_COM::OIL_COM(::Udm::ObjectImpl *impl) : UDM_OBJECT(impl) {}
	OIL_COM::OIL_COM(const OIL_COM &master) : UDM_OBJECT(master) {}

#ifdef UDM_RVALUE
	OIL_COM::OIL_COM(OIL_COM &&master) : UDM_OBJECT(master) {};

	OIL_COM OIL_COM::Cast(::Udm::Object &&a) { return __Cast(std::move(a), meta); };
	OIL_COM& OIL_COM::operator=(OIL_COM &&a) { ::Udm::Object::operator =(std::move(a)); return *this; };

#endif
	OIL_COM OIL_COM::Cast(const ::Udm::Object &a) { return __Cast(a, meta); }
	OIL_COM OIL_COM::Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role); }
	OIL_COM OIL_COM::CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl); }
	::Udm::InstantiatedAttr< OIL_COM> OIL_COM::Instances() { return ::Udm::InstantiatedAttr< OIL_COM>(impl); }
	OIL_COM OIL_COM::CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl, true); }
	::Udm::DerivedAttr< OIL_COM> OIL_COM::Derived() { return ::Udm::DerivedAttr< OIL_COM>(impl); }
	::Udm::ArchetypeAttr< OIL_COM> OIL_COM::Archetype() const { return ::Udm::ArchetypeAttr< OIL_COM>(impl); }
	::Udm::StringAttr OIL_COM::name() const { return ::Udm::StringAttr(impl, meta_name); }
	::Udm::StringAttr OIL_COM::comment() const { return ::Udm::StringAttr(impl, meta_comment); }
	::Udm::BooleanAttr OIL_COM::USEMESSAGERESOURCE() const { return ::Udm::BooleanAttr(impl, meta_USEMESSAGERESOURCE); }
	::Udm::BooleanAttr OIL_COM::USEMESSAGESTATUS() const { return ::Udm::BooleanAttr(impl, meta_USEMESSAGESTATUS); }
	::Udm::ParentAttr< ::OIL::CPU> OIL_COM::CPU_parent() const { return ::Udm::ParentAttr< ::OIL::CPU>(impl, meta_CPU_parent); }
	::Udm::ParentAttr< ::OIL::CPU> OIL_COM::parent() const { return ::Udm::ParentAttr< ::OIL::CPU>(impl, ::Udm::NULLPARENTROLE); }

	::Uml::Class OIL_COM::meta;
	::Uml::Attribute OIL_COM::meta_name;
	::Uml::Attribute OIL_COM::meta_comment;
	::Uml::Attribute OIL_COM::meta_USEMESSAGERESOURCE;
	::Uml::Attribute OIL_COM::meta_USEMESSAGESTATUS;
	::Uml::CompositionParentRole OIL_COM::meta_CPU_parent;

	::Uml::Diagram meta;

	void CreateMeta() {
		// classes, with attributes, constraints and constraint definitions
		ALARM::meta = ::Uml::Class::Create(meta);
		ALARM::meta_Component = ::Uml::Attribute::Create(ALARM::meta);
		ALARM::meta_name = ::Uml::Attribute::Create(ALARM::meta);
		ALARM::meta_COUNTER = ::Uml::Attribute::Create(ALARM::meta);
		ALARM::meta_ACTION = ::Uml::Attribute::Create(ALARM::meta);
		ALARM::meta_AUTOSTART = ::Uml::Attribute::Create(ALARM::meta);
		ALARM::meta_ALARMTIME = ::Uml::Attribute::Create(ALARM::meta);
		ALARM::meta_CYCLETIME = ::Uml::Attribute::Create(ALARM::meta);
		ALARM::meta_APPMODE = ::Uml::Attribute::Create(ALARM::meta);
		ALARM::meta_AlarmUnit = ::Uml::Attribute::Create(ALARM::meta);
		ALARM::meta_StaticAlarm = ::Uml::Attribute::Create(ALARM::meta);

		CPU::meta = ::Uml::Class::Create(meta);
		CPU::meta_name = ::Uml::Attribute::Create(CPU::meta);

		EVENT::meta = ::Uml::Class::Create(meta);
		EVENT::meta_Component = ::Uml::Attribute::Create(EVENT::meta);
		EVENT::meta_name = ::Uml::Attribute::Create(EVENT::meta);
		EVENT::meta_MASK = ::Uml::Attribute::Create(EVENT::meta);

		Include::meta = ::Uml::Class::Create(meta);
		Include::meta_filename = ::Uml::Attribute::Create(Include::meta);

		OILFile::meta = ::Uml::Class::Create(meta);
		OILFile::meta_filename = ::Uml::Attribute::Create(OILFile::meta);

		OIL_COM::meta = ::Uml::Class::Create(meta);
		OIL_COM::meta_name = ::Uml::Attribute::Create(OIL_COM::meta);
		OIL_COM::meta_comment = ::Uml::Attribute::Create(OIL_COM::meta);
		OIL_COM::meta_USEMESSAGERESOURCE = ::Uml::Attribute::Create(OIL_COM::meta);
		OIL_COM::meta_USEMESSAGESTATUS = ::Uml::Attribute::Create(OIL_COM::meta);

		OIL_OS::meta = ::Uml::Class::Create(meta);
		OIL_OS::meta_name = ::Uml::Attribute::Create(OIL_OS::meta);
		OIL_OS::meta_comment = ::Uml::Attribute::Create(OIL_OS::meta);
		OIL_OS::meta_CC = ::Uml::Attribute::Create(OIL_OS::meta);
		OIL_OS::meta_STATUS = ::Uml::Attribute::Create(OIL_OS::meta);
		OIL_OS::meta_SCHEDULE = ::Uml::Attribute::Create(OIL_OS::meta);
		OIL_OS::meta_STARTUPHOOK = ::Uml::Attribute::Create(OIL_OS::meta);
		OIL_OS::meta_SHUTDOWNHOOK = ::Uml::Attribute::Create(OIL_OS::meta);
		OIL_OS::meta_ERRORHOOK = ::Uml::Attribute::Create(OIL_OS::meta);
		OIL_OS::meta_PRETASKHOOK = ::Uml::Attribute::Create(OIL_OS::meta);
		OIL_OS::meta_POSTTASKHOOK = ::Uml::Attribute::Create(OIL_OS::meta);
		OIL_OS::meta_USEGETSERVICEID = ::Uml::Attribute::Create(OIL_OS::meta);
		OIL_OS::meta_USEPARAMETERACCESS = ::Uml::Attribute::Create(OIL_OS::meta);
		OIL_OS::meta_TickTime = ::Uml::Attribute::Create(OIL_OS::meta);

		OIL_VERSION::meta = ::Uml::Class::Create(meta);
		OIL_VERSION::meta_text = ::Uml::Attribute::Create(OIL_VERSION::meta);

		Procedure::meta = ::Uml::Class::Create(meta);
		Procedure::meta_name = ::Uml::Attribute::Create(Procedure::meta);

		TASK::meta = ::Uml::Class::Create(meta);
		TASK::meta_Component = ::Uml::Attribute::Create(TASK::meta);
		TASK::meta_name = ::Uml::Attribute::Create(TASK::meta);
		TASK::meta_TYPE = ::Uml::Attribute::Create(TASK::meta);
		TASK::meta_SCHEDULE = ::Uml::Attribute::Create(TASK::meta);
		TASK::meta_PRIORITY = ::Uml::Attribute::Create(TASK::meta);
		TASK::meta_ACTIVATION = ::Uml::Attribute::Create(TASK::meta);
		TASK::meta_AUTOSTART = ::Uml::Attribute::Create(TASK::meta);
		TASK::meta_APPMODE = ::Uml::Attribute::Create(TASK::meta);

	}

	void InitMeta() {
		// classes, with attributes, constraints and constraint definitions
		::Uml::InitClassProps(ALARM::meta, "ALARM", false, NULL, NULL);
		::Uml::InitAttributeProps(ALARM::meta_Component, "Component", "String", false, false, 0, 1, false, "public", vector<string>());
		::Uml::InitAttributeProps(ALARM::meta_name, "name", "String", false, false, 1, 1, false, "public", vector<string>());
		::Uml::InitAttributeProps(ALARM::meta_COUNTER, "COUNTER", "String", false, false, 1, 1, false, "public", vector<string>());
		::Uml::InitAttributeProps(ALARM::meta_ACTION, "ACTION", "String", false, false, 1, 1, false, "public", vector<string>());
		::Uml::InitAttributeProps(ALARM::meta_AUTOSTART, "AUTOSTART", "Boolean", false, false, 1, 1, false, "public", vector<string>());
		::Uml::InitAttributeProps(ALARM::meta_ALARMTIME, "ALARMTIME", "Integer", false, false, 1, 1, false, "public", vector<string>());
		::Uml::InitAttributeProps(ALARM::meta_CYCLETIME, "CYCLETIME", "Integer", false, false, 1, 1, false, "public", vector<string>());
		::Uml::InitAttributeProps(ALARM::meta_APPMODE, "APPMODE", "String", false, false, 1, 1, false, "public", vector<string>());
		::Uml::InitAttributeProps(ALARM::meta_AlarmUnit, "AlarmUnit", "String", false, false, 1, 1, false, "public", vector<string>());
		::Uml::InitAttributeProps(ALARM::meta_StaticAlarm, "StaticAlarm", "Boolean", false, false, 1, 1, false, "public", vector<string>());

		::Uml::InitClassProps(CPU::meta, "CPU", false, NULL, NULL);
		::Uml::InitAttributeProps(CPU::meta_name, "name", "String", false, false, 1, 1, false, "public", vector<string>());

		::Uml::InitClassProps(EVENT::meta, "EVENT", false, NULL, NULL);
		::Uml::InitAttributeProps(EVENT::meta_Component, "Component", "String", false, false, 0, 1, false, "public", vector<string>());
		::Uml::InitAttributeProps(EVENT::meta_name, "name", "String", false, false, 1, 1, false, "public", vector<string>());
		::Uml::InitAttributeProps(EVENT::meta_MASK, "MASK", "String", false, false, 1, 1, false, "public", vector<string>());

		::Uml::InitClassProps(Include::meta, "Include", false, NULL, NULL);
		::Uml::InitAttributeProps(Include::meta_filename, "filename", "String", false, false, 1, 1, false, "public", vector<string>());

		::Uml::InitClassProps(OILFile::meta, "OILFile", false, NULL, NULL);
		::Uml::InitAttributeProps(OILFile::meta_filename, "filename", "String", false, false, 1, 1, false, "public", vector<string>());

		::Uml::InitClassProps(OIL_COM::meta, "OIL_COM", false, NULL, NULL);
		::Uml::InitAttributeProps(OIL_COM::meta_name, "name", "String", false, false, 1, 1, false, "public", vector<string>());
		::Uml::InitAttributeProps(OIL_COM::meta_comment, "comment", "String", false, false, 1, 1, false, "public", vector<string>());
		::Uml::InitAttributeProps(OIL_COM::meta_USEMESSAGERESOURCE, "USEMESSAGERESOURCE", "Boolean", false, false, 1, 1, false, "public", vector<string>());
		::Uml::InitAttributeProps(OIL_COM::meta_USEMESSAGESTATUS, "USEMESSAGESTATUS", "Boolean", false, false, 1, 1, false, "public", vector<string>());

		::Uml::InitClassProps(OIL_OS::meta, "OIL_OS", false, NULL, NULL);
		::Uml::InitAttributeProps(OIL_OS::meta_name, "name", "String", false, false, 1, 1, false, "public", vector<string>());
		::Uml::InitAttributeProps(OIL_OS::meta_comment, "comment", "String", false, false, 1, 1, false, "public", vector<string>());
		::Uml::InitAttributeProps(OIL_OS::meta_CC, "CC", "String", false, false, 1, 1, false, "public", vector<string>());
		::Uml::InitAttributeProps(OIL_OS::meta_STATUS, "STATUS", "String", false, false, 1, 1, false, "public", vector<string>());
		::Uml::InitAttributeProps(OIL_OS::meta_SCHEDULE, "SCHEDULE", "String", false, false, 1, 1, false, "public", vector<string>());
		::Uml::InitAttributeProps(OIL_OS::meta_STARTUPHOOK, "STARTUPHOOK", "Boolean", false, false, 1, 1, false, "public", vector<string>());
		::Uml::InitAttributeProps(OIL_OS::meta_SHUTDOWNHOOK, "SHUTDOWNHOOK", "Boolean", false, false, 1, 1, false, "public", vector<string>());
		::Uml::InitAttributeProps(OIL_OS::meta_ERRORHOOK, "ERRORHOOK", "Boolean", false, false, 1, 1, false, "public", vector<string>());
		::Uml::InitAttributeProps(OIL_OS::meta_PRETASKHOOK, "PRETASKHOOK", "Boolean", false, false, 1, 1, false, "public", vector<string>());
		::Uml::InitAttributeProps(OIL_OS::meta_POSTTASKHOOK, "POSTTASKHOOK", "Boolean", false, false, 1, 1, false, "public", vector<string>());
		::Uml::InitAttributeProps(OIL_OS::meta_USEGETSERVICEID, "USEGETSERVICEID", "Boolean", false, false, 1, 1, false, "public", vector<string>());
		::Uml::InitAttributeProps(OIL_OS::meta_USEPARAMETERACCESS, "USEPARAMETERACCESS", "Boolean", false, false, 1, 1, false, "public", vector<string>());
		::Uml::InitAttributeProps(OIL_OS::meta_TickTime, "TickTime", "Integer", false, false, 1, 1, false, "public", vector<string>());

		::Uml::InitClassProps(OIL_VERSION::meta, "OIL_VERSION", false, NULL, NULL);
		::Uml::InitAttributeProps(OIL_VERSION::meta_text, "text", "String", false, false, 1, 1, false, "public", vector<string>());

		::Uml::InitClassProps(Procedure::meta, "Procedure", false, NULL, NULL);
		::Uml::InitAttributeProps(Procedure::meta_name, "name", "String", false, false, 1, 1, false, "public", vector<string>());

		::Uml::InitClassProps(TASK::meta, "TASK", false, NULL, NULL);
		::Uml::InitAttributeProps(TASK::meta_Component, "Component", "String", false, false, 0, 1, false, "public", vector<string>());
		::Uml::InitAttributeProps(TASK::meta_name, "name", "String", false, false, 1, 1, false, "public", vector<string>());
		::Uml::InitAttributeProps(TASK::meta_TYPE, "TYPE", "String", false, false, 1, 1, false, "public", vector<string>());
		::Uml::InitAttributeProps(TASK::meta_SCHEDULE, "SCHEDULE", "String", false, false, 1, 1, false, "public", vector<string>());
		::Uml::InitAttributeProps(TASK::meta_PRIORITY, "PRIORITY", "Integer", false, false, 1, 1, false, "public", vector<string>());
		::Uml::InitAttributeProps(TASK::meta_ACTIVATION, "ACTIVATION", "Integer", false, false, 1, 1, false, "public", vector<string>());
		::Uml::InitAttributeProps(TASK::meta_AUTOSTART, "AUTOSTART", "Boolean", false, false, 1, 1, false, "public", vector<string>());
		::Uml::InitAttributeProps(TASK::meta_APPMODE, "APPMODE", "String", false, false, 1, 1, false, "public", vector<string>());

		// associations
		{
			::Uml::Association ass = ::Uml::Association::Create(meta);
			::Uml::InitAssociationProps(ass, "Association");
			ALARM::meta_event = ::Uml::AssociationRole::Create(ass);
			::Uml::InitAssociationRoleProps(ALARM::meta_event, "event", true, false, 0, 1);
			EVENT::meta_alarm = ::Uml::AssociationRole::Create(ass);
			::Uml::InitAssociationRoleProps(EVENT::meta_alarm, "alarm", true, false, 0, 1);

		}
		{
			::Uml::Association ass = ::Uml::Association::Create(meta);
			::Uml::InitAssociationProps(ass, "Association");
			EVENT::meta_procs = ::Uml::AssociationRole::Create(ass);
			::Uml::InitAssociationRoleProps(EVENT::meta_procs, "procs", true, false, 0, -1);
			Procedure::meta_event = ::Uml::AssociationRole::Create(ass);
			::Uml::InitAssociationRoleProps(Procedure::meta_event, "event", true, false, 1, 1);

		}
		{
			::Uml::Association ass = ::Uml::Association::Create(meta);
			::Uml::InitAssociationProps(ass, "Association");
			EVENT::meta_ownerTask = ::Uml::AssociationRole::Create(ass);
			::Uml::InitAssociationRoleProps(EVENT::meta_ownerTask, "ownerTask", true, false, 0, 1);
			TASK::meta_events = ::Uml::AssociationRole::Create(ass);
			::Uml::InitAssociationRoleProps(TASK::meta_events, "events", true, false, 0, -1);

		}

		// compositions
		{
			::Uml::Composition comp = ::Uml::Composition::Create(meta);
			::Uml::InitCompositionProps(comp, "Composition", false);
			CPU::meta_OILFile_parent = ::Uml::CompositionParentRole::Create(comp);
			::Uml::InitCompositionParentRoleProps(CPU::meta_OILFile_parent, "OILFile_parent", true);
			OILFile::meta_CPU_child = ::Uml::CompositionChildRole::Create(comp);
			::Uml::InitCompositionChildRoleProps(OILFile::meta_CPU_child, "CPU", true, 1, 1);

		}
		{
			::Uml::Composition comp = ::Uml::Composition::Create(meta);
			::Uml::InitCompositionProps(comp, "Composition", false);
			OIL_VERSION::meta_OILFile_parent = ::Uml::CompositionParentRole::Create(comp);
			::Uml::InitCompositionParentRoleProps(OIL_VERSION::meta_OILFile_parent, "OILFile_parent", true);
			OILFile::meta_OIL_VERSION_child = ::Uml::CompositionChildRole::Create(comp);
			::Uml::InitCompositionChildRoleProps(OILFile::meta_OIL_VERSION_child, "OIL_VERSION", true, 1, 1);

		}
		{
			::Uml::Composition comp = ::Uml::Composition::Create(meta);
			::Uml::InitCompositionProps(comp, "Composition", false);
			Include::meta_OILFile_parent = ::Uml::CompositionParentRole::Create(comp);
			::Uml::InitCompositionParentRoleProps(Include::meta_OILFile_parent, "OILFile_parent", true);
			OILFile::meta_Include_children = ::Uml::CompositionChildRole::Create(comp);
			::Uml::InitCompositionChildRoleProps(OILFile::meta_Include_children, "Include", true, 0, -1);

		}
		{
			::Uml::Composition comp = ::Uml::Composition::Create(meta);
			::Uml::InitCompositionProps(comp, "Composition", false);
			Include::meta_CPU_parent = ::Uml::CompositionParentRole::Create(comp);
			::Uml::InitCompositionParentRoleProps(Include::meta_CPU_parent, "CPU_parent", true);
			CPU::meta_Include_children = ::Uml::CompositionChildRole::Create(comp);
			::Uml::InitCompositionChildRoleProps(CPU::meta_Include_children, "Include", true, 0, -1);

		}
		{
			::Uml::Composition comp = ::Uml::Composition::Create(meta);
			::Uml::InitCompositionProps(comp, "Composition", false);
			ALARM::meta_CPU_parent = ::Uml::CompositionParentRole::Create(comp);
			::Uml::InitCompositionParentRoleProps(ALARM::meta_CPU_parent, "CPU_parent", true);
			CPU::meta_ALARM_children = ::Uml::CompositionChildRole::Create(comp);
			::Uml::InitCompositionChildRoleProps(CPU::meta_ALARM_children, "ALARM", true, 0, -1);

		}
		{
			::Uml::Composition comp = ::Uml::Composition::Create(meta);
			::Uml::InitCompositionProps(comp, "Composition", false);
			EVENT::meta_CPU_parent = ::Uml::CompositionParentRole::Create(comp);
			::Uml::InitCompositionParentRoleProps(EVENT::meta_CPU_parent, "CPU_parent", true);
			CPU::meta_EVENT_children = ::Uml::CompositionChildRole::Create(comp);
			::Uml::InitCompositionChildRoleProps(CPU::meta_EVENT_children, "EVENT", true, 0, -1);

		}
		{
			::Uml::Composition comp = ::Uml::Composition::Create(meta);
			::Uml::InitCompositionProps(comp, "Composition", false);
			TASK::meta_CPU_parent = ::Uml::CompositionParentRole::Create(comp);
			::Uml::InitCompositionParentRoleProps(TASK::meta_CPU_parent, "CPU_parent", true);
			CPU::meta_TASK_children = ::Uml::CompositionChildRole::Create(comp);
			::Uml::InitCompositionChildRoleProps(CPU::meta_TASK_children, "TASK", true, 0, -1);

		}
		{
			::Uml::Composition comp = ::Uml::Composition::Create(meta);
			::Uml::InitCompositionProps(comp, "Composition", false);
			OIL_COM::meta_CPU_parent = ::Uml::CompositionParentRole::Create(comp);
			::Uml::InitCompositionParentRoleProps(OIL_COM::meta_CPU_parent, "CPU_parent", true);
			CPU::meta_OIL_COM_child = ::Uml::CompositionChildRole::Create(comp);
			::Uml::InitCompositionChildRoleProps(CPU::meta_OIL_COM_child, "OIL_COM", true, 1, 1);

		}
		{
			::Uml::Composition comp = ::Uml::Composition::Create(meta);
			::Uml::InitCompositionProps(comp, "Composition", false);
			OIL_OS::meta_CPU_parent = ::Uml::CompositionParentRole::Create(comp);
			::Uml::InitCompositionParentRoleProps(OIL_OS::meta_CPU_parent, "CPU_parent", true);
			CPU::meta_OIL_OS_child = ::Uml::CompositionChildRole::Create(comp);
			::Uml::InitCompositionChildRoleProps(CPU::meta_OIL_OS_child, "OIL_OS", true, 1, 1);

		}
		{
			::Uml::Composition comp = ::Uml::Composition::Create(meta);
			::Uml::InitCompositionProps(comp, "Composition", false);
			Procedure::meta_TASK_parent = ::Uml::CompositionParentRole::Create(comp);
			::Uml::InitCompositionParentRoleProps(Procedure::meta_TASK_parent, "TASK_parent", true);
			TASK::meta_Procedure_child = ::Uml::CompositionChildRole::Create(comp);
			::Uml::InitCompositionChildRoleProps(TASK::meta_Procedure_child, "Procedure", true, 0, 1);

		}

	}

	void InitMetaLinks() {
		ALARM::meta_event.target() = EVENT::meta;
		CPU::meta_ALARM_children.target() = ALARM::meta;

		Include::meta_CPU_parent.target() = CPU::meta;
		ALARM::meta_CPU_parent.target() = CPU::meta;
		EVENT::meta_CPU_parent.target() = CPU::meta;
		TASK::meta_CPU_parent.target() = CPU::meta;
		OIL_COM::meta_CPU_parent.target() = CPU::meta;
		OIL_OS::meta_CPU_parent.target() = CPU::meta;
		OILFile::meta_CPU_child.target() = CPU::meta;

		EVENT::meta_alarm.target() = ALARM::meta;
		EVENT::meta_procs.target() = Procedure::meta;
		EVENT::meta_ownerTask.target() = TASK::meta;
		CPU::meta_EVENT_children.target() = EVENT::meta;

		OILFile::meta_Include_children.target() = Include::meta;
		CPU::meta_Include_children.target() = Include::meta;

		CPU::meta_OILFile_parent.target() = OILFile::meta;
		OIL_VERSION::meta_OILFile_parent.target() = OILFile::meta;
		Include::meta_OILFile_parent.target() = OILFile::meta;

		CPU::meta_OIL_COM_child.target() = OIL_COM::meta;

		CPU::meta_OIL_OS_child.target() = OIL_OS::meta;

		OILFile::meta_OIL_VERSION_child.target() = OIL_VERSION::meta;

		Procedure::meta_event.target() = EVENT::meta;
		TASK::meta_Procedure_child.target() = Procedure::meta;

		TASK::meta_events.target() = EVENT::meta;
		Procedure::meta_TASK_parent.target() = TASK::meta;
		CPU::meta_TASK_children.target() = TASK::meta;

	}

	void InitMeta(const ::Uml::Diagram &parent) {
		// classes, with attributes, constraints and constraint definitions
		::Uml::SetClass(ALARM::meta, parent, "ALARM");
		::Uml::SetAttribute(ALARM::meta_Component, ALARM::meta, "Component");
		::Uml::SetAttribute(ALARM::meta_name, ALARM::meta, "name");
		::Uml::SetAttribute(ALARM::meta_COUNTER, ALARM::meta, "COUNTER");
		::Uml::SetAttribute(ALARM::meta_ACTION, ALARM::meta, "ACTION");
		::Uml::SetAttribute(ALARM::meta_AUTOSTART, ALARM::meta, "AUTOSTART");
		::Uml::SetAttribute(ALARM::meta_ALARMTIME, ALARM::meta, "ALARMTIME");
		::Uml::SetAttribute(ALARM::meta_CYCLETIME, ALARM::meta, "CYCLETIME");
		::Uml::SetAttribute(ALARM::meta_APPMODE, ALARM::meta, "APPMODE");
		::Uml::SetAttribute(ALARM::meta_AlarmUnit, ALARM::meta, "AlarmUnit");
		::Uml::SetAttribute(ALARM::meta_StaticAlarm, ALARM::meta, "StaticAlarm");

		::Uml::SetClass(CPU::meta, parent, "CPU");
		::Uml::SetAttribute(CPU::meta_name, CPU::meta, "name");

		::Uml::SetClass(EVENT::meta, parent, "EVENT");
		::Uml::SetAttribute(EVENT::meta_Component, EVENT::meta, "Component");
		::Uml::SetAttribute(EVENT::meta_name, EVENT::meta, "name");
		::Uml::SetAttribute(EVENT::meta_MASK, EVENT::meta, "MASK");

		::Uml::SetClass(Include::meta, parent, "Include");
		::Uml::SetAttribute(Include::meta_filename, Include::meta, "filename");

		::Uml::SetClass(OILFile::meta, parent, "OILFile");
		::Uml::SetAttribute(OILFile::meta_filename, OILFile::meta, "filename");

		::Uml::SetClass(OIL_COM::meta, parent, "OIL_COM");
		::Uml::SetAttribute(OIL_COM::meta_name, OIL_COM::meta, "name");
		::Uml::SetAttribute(OIL_COM::meta_comment, OIL_COM::meta, "comment");
		::Uml::SetAttribute(OIL_COM::meta_USEMESSAGERESOURCE, OIL_COM::meta, "USEMESSAGERESOURCE");
		::Uml::SetAttribute(OIL_COM::meta_USEMESSAGESTATUS, OIL_COM::meta, "USEMESSAGESTATUS");

		::Uml::SetClass(OIL_OS::meta, parent, "OIL_OS");
		::Uml::SetAttribute(OIL_OS::meta_name, OIL_OS::meta, "name");
		::Uml::SetAttribute(OIL_OS::meta_comment, OIL_OS::meta, "comment");
		::Uml::SetAttribute(OIL_OS::meta_CC, OIL_OS::meta, "CC");
		::Uml::SetAttribute(OIL_OS::meta_STATUS, OIL_OS::meta, "STATUS");
		::Uml::SetAttribute(OIL_OS::meta_SCHEDULE, OIL_OS::meta, "SCHEDULE");
		::Uml::SetAttribute(OIL_OS::meta_STARTUPHOOK, OIL_OS::meta, "STARTUPHOOK");
		::Uml::SetAttribute(OIL_OS::meta_SHUTDOWNHOOK, OIL_OS::meta, "SHUTDOWNHOOK");
		::Uml::SetAttribute(OIL_OS::meta_ERRORHOOK, OIL_OS::meta, "ERRORHOOK");
		::Uml::SetAttribute(OIL_OS::meta_PRETASKHOOK, OIL_OS::meta, "PRETASKHOOK");
		::Uml::SetAttribute(OIL_OS::meta_POSTTASKHOOK, OIL_OS::meta, "POSTTASKHOOK");
		::Uml::SetAttribute(OIL_OS::meta_USEGETSERVICEID, OIL_OS::meta, "USEGETSERVICEID");
		::Uml::SetAttribute(OIL_OS::meta_USEPARAMETERACCESS, OIL_OS::meta, "USEPARAMETERACCESS");
		::Uml::SetAttribute(OIL_OS::meta_TickTime, OIL_OS::meta, "TickTime");

		::Uml::SetClass(OIL_VERSION::meta, parent, "OIL_VERSION");
		::Uml::SetAttribute(OIL_VERSION::meta_text, OIL_VERSION::meta, "text");

		::Uml::SetClass(Procedure::meta, parent, "Procedure");
		::Uml::SetAttribute(Procedure::meta_name, Procedure::meta, "name");

		::Uml::SetClass(TASK::meta, parent, "TASK");
		::Uml::SetAttribute(TASK::meta_Component, TASK::meta, "Component");
		::Uml::SetAttribute(TASK::meta_name, TASK::meta, "name");
		::Uml::SetAttribute(TASK::meta_TYPE, TASK::meta, "TYPE");
		::Uml::SetAttribute(TASK::meta_SCHEDULE, TASK::meta, "SCHEDULE");
		::Uml::SetAttribute(TASK::meta_PRIORITY, TASK::meta, "PRIORITY");
		::Uml::SetAttribute(TASK::meta_ACTIVATION, TASK::meta, "ACTIVATION");
		::Uml::SetAttribute(TASK::meta_AUTOSTART, TASK::meta, "AUTOSTART");
		::Uml::SetAttribute(TASK::meta_APPMODE, TASK::meta, "APPMODE");

	}

	void InitMetaLinks(const ::Uml::Diagram &parent) {
		// classes
		::Uml::SetAssocRole(ALARM::meta_event, ALARM::meta, EVENT::meta, "alarm");
		::Uml::SetParentRole(ALARM::meta_CPU_parent, ALARM::meta, CPU::meta, "", "");

		::Uml::SetChildRole(CPU::meta_Include_children, CPU::meta, Include::meta, "", "");
		::Uml::SetChildRole(CPU::meta_ALARM_children, CPU::meta, ALARM::meta, "", "");
		::Uml::SetChildRole(CPU::meta_EVENT_children, CPU::meta, EVENT::meta, "", "");
		::Uml::SetChildRole(CPU::meta_TASK_children, CPU::meta, TASK::meta, "", "");
		::Uml::SetChildRole(CPU::meta_OIL_COM_child, CPU::meta, OIL_COM::meta, "", "");
		::Uml::SetChildRole(CPU::meta_OIL_OS_child, CPU::meta, OIL_OS::meta, "", "");
		::Uml::SetParentRole(CPU::meta_OILFile_parent, CPU::meta, OILFile::meta, "", "");

		::Uml::SetAssocRole(EVENT::meta_alarm, EVENT::meta, ALARM::meta, "event");
		::Uml::SetAssocRole(EVENT::meta_procs, EVENT::meta, Procedure::meta, "event");
		::Uml::SetAssocRole(EVENT::meta_ownerTask, EVENT::meta, TASK::meta, "events");
		::Uml::SetParentRole(EVENT::meta_CPU_parent, EVENT::meta, CPU::meta, "", "");

		::Uml::SetParentRole(Include::meta_OILFile_parent, Include::meta, OILFile::meta, "", "");
		::Uml::SetParentRole(Include::meta_CPU_parent, Include::meta, CPU::meta, "", "");

		::Uml::SetChildRole(OILFile::meta_CPU_child, OILFile::meta, CPU::meta, "", "");
		::Uml::SetChildRole(OILFile::meta_OIL_VERSION_child, OILFile::meta, OIL_VERSION::meta, "", "");
		::Uml::SetChildRole(OILFile::meta_Include_children, OILFile::meta, Include::meta, "", "");

		::Uml::SetParentRole(OIL_COM::meta_CPU_parent, OIL_COM::meta, CPU::meta, "", "");

		::Uml::SetParentRole(OIL_OS::meta_CPU_parent, OIL_OS::meta, CPU::meta, "", "");

		::Uml::SetParentRole(OIL_VERSION::meta_OILFile_parent, OIL_VERSION::meta, OILFile::meta, "", "");

		::Uml::SetAssocRole(Procedure::meta_event, Procedure::meta, EVENT::meta, "procs");
		::Uml::SetParentRole(Procedure::meta_TASK_parent, Procedure::meta, TASK::meta, "", "");

		::Uml::SetAssocRole(TASK::meta_events, TASK::meta, EVENT::meta, "ownerTask");
		::Uml::SetChildRole(TASK::meta_Procedure_child, TASK::meta, Procedure::meta, "", "");
		::Uml::SetParentRole(TASK::meta_CPU_parent, TASK::meta, CPU::meta, "", "");

	}

	void Initialize()
	{
		static bool first = true;
		if (!first) return;
		first = false;
		::Uml::Initialize();

	
		UDM_ASSERT( meta == ::Udm::null );

		::UdmStatic::StaticDataNetwork * meta_dn = new ::UdmStatic::StaticDataNetwork(::Uml::diagram);
		meta_dn->CreateNew("OIL.mem", "", ::Uml::Diagram::meta, ::Udm::CHANGES_LOST_DEFAULT);
		meta = ::Uml::Diagram::Cast(meta_dn->GetRootObject());

		::Uml::InitDiagramProps(meta, "OIL", "1.00");


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
			::Udm::MetaDepository::StoreDiagram("OIL", diagram);
		}
		~_regClass()
		{
			::Udm::MetaDepository::RemoveDiagram("OIL");
		}
	} __regUnUsed;

}

