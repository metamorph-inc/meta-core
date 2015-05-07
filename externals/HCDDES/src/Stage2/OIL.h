#ifndef MOBIES_OIL_H
#define MOBIES_OIL_H

// header file OIL.h generated from diagram OIL
// generated with Udm version 3.33 on Wed Oct 29 11:21:54 2014

#include <UdmBase.h>

#if !defined(UDM_VERSION_MAJOR) || !defined(UDM_VERSION_MINOR)
#    error "Udm headers too old, they do not define UDM_VERSION"
#elif UDM_VERSION_MAJOR < 3
#    error "Udm headers too old, minimum version required 3.33"
#elif UDM_VERSION_MAJOR == 3 && UDM_VERSION_MINOR < 33
#    error "Udm headers too old, minimum version required 3.33"
#endif

#include <Uml.h>


#ifdef min
#undef min
#endif

#ifdef max
#undef max
#endif

namespace OIL {

	extern ::Uml::Diagram meta;
	class OILFile;
	class OIL_VERSION;
	class Include;
	class CPU;
	class OIL_OS;
	class Procedure;
	class ALARM;
	class EVENT;
	class TASK;
	class OIL_COM;

	void Initialize();
	void Initialize(const ::Uml::Diagram &dgr);

	extern  ::Udm::UdmDiagram diagram;

	class OILFile : public ::Udm::Object {
	public:
		OILFile();
		OILFile(::Udm::ObjectImpl *impl);
		OILFile(const OILFile &master);

#ifdef UDM_RVALUE
		OILFile(OILFile &&master);

		static OILFile Cast(::Udm::Object &&a);
		OILFile& operator=(OILFile &&a);

#endif
		static OILFile Cast(const ::Udm::Object &a);
		static OILFile Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		OILFile CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::InstantiatedAttr< OILFile> Instances();
		template <class Pred> ::Udm::InstantiatedAttr< OILFile, Pred> Instances_sorted(const Pred &) { return ::Udm::InstantiatedAttr< OILFile, Pred>(impl); };
		OILFile CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::DerivedAttr< OILFile> Derived();
		template <class Pred> ::Udm::DerivedAttr< OILFile, Pred> Derived_sorted(const Pred &) { return ::Udm::DerivedAttr< OILFile, Pred>(impl); };
		::Udm::ArchetypeAttr< OILFile> Archetype() const;
		::Udm::StringAttr filename() const;
		::Udm::ChildAttr< ::OIL::CPU> CPU_child() const;
		::Udm::ChildAttr< ::OIL::OIL_VERSION> OIL_VERSION_child() const;
		::Udm::ChildrenAttr< ::OIL::Include> Include_children() const;
		template <class Pred> ::Udm::ChildrenAttr< ::OIL::Include, Pred> Include_children_sorted(const Pred &) const { return ::Udm::ChildrenAttr< ::OIL::Include, Pred>(impl, meta_Include_children); };
		::Udm::ChildrenAttr< ::OIL::OIL_VERSION> OIL_VERSION_kind_children() const;
		template <class Pred> ::Udm::ChildrenAttr< ::OIL::OIL_VERSION, Pred> OIL_VERSION_kind_children_sorted(const Pred &) const { return ::Udm::ChildrenAttr< ::OIL::OIL_VERSION, Pred>(impl, ::Udm::NULLCHILDROLE); };
		::Udm::ChildrenAttr< ::OIL::Include> Include_kind_children() const;
		template <class Pred> ::Udm::ChildrenAttr< ::OIL::Include, Pred> Include_kind_children_sorted(const Pred &) const { return ::Udm::ChildrenAttr< ::OIL::Include, Pred>(impl, ::Udm::NULLCHILDROLE); };
		::Udm::ChildrenAttr< ::OIL::CPU> CPU_kind_children() const;
		template <class Pred> ::Udm::ChildrenAttr< ::OIL::CPU, Pred> CPU_kind_children_sorted(const Pred &) const { return ::Udm::ChildrenAttr< ::OIL::CPU, Pred>(impl, ::Udm::NULLCHILDROLE); };
		::Udm::ParentAttr< ::Udm::Object> parent() const;

		static ::Uml::Class meta;
		static ::Uml::Attribute meta_filename;
		static ::Uml::CompositionChildRole meta_CPU_child;
		static ::Uml::CompositionChildRole meta_OIL_VERSION_child;
		static ::Uml::CompositionChildRole meta_Include_children;

	};

	class OIL_VERSION : public ::Udm::Object {
	public:
		OIL_VERSION();
		OIL_VERSION(::Udm::ObjectImpl *impl);
		OIL_VERSION(const OIL_VERSION &master);

#ifdef UDM_RVALUE
		OIL_VERSION(OIL_VERSION &&master);

		static OIL_VERSION Cast(::Udm::Object &&a);
		OIL_VERSION& operator=(OIL_VERSION &&a);

#endif
		static OIL_VERSION Cast(const ::Udm::Object &a);
		static OIL_VERSION Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		OIL_VERSION CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::InstantiatedAttr< OIL_VERSION> Instances();
		template <class Pred> ::Udm::InstantiatedAttr< OIL_VERSION, Pred> Instances_sorted(const Pred &) { return ::Udm::InstantiatedAttr< OIL_VERSION, Pred>(impl); };
		OIL_VERSION CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::DerivedAttr< OIL_VERSION> Derived();
		template <class Pred> ::Udm::DerivedAttr< OIL_VERSION, Pred> Derived_sorted(const Pred &) { return ::Udm::DerivedAttr< OIL_VERSION, Pred>(impl); };
		::Udm::ArchetypeAttr< OIL_VERSION> Archetype() const;
		::Udm::StringAttr text() const;
		::Udm::ParentAttr< ::OIL::OILFile> OILFile_parent() const;
		::Udm::ParentAttr< ::OIL::OILFile> parent() const;

		static ::Uml::Class meta;
		static ::Uml::Attribute meta_text;
		static ::Uml::CompositionParentRole meta_OILFile_parent;

	};

	class Include : public ::Udm::Object {
	public:
		Include();
		Include(::Udm::ObjectImpl *impl);
		Include(const Include &master);

#ifdef UDM_RVALUE
		Include(Include &&master);

		static Include Cast(::Udm::Object &&a);
		Include& operator=(Include &&a);

#endif
		static Include Cast(const ::Udm::Object &a);
		static Include Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		Include CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::InstantiatedAttr< Include> Instances();
		template <class Pred> ::Udm::InstantiatedAttr< Include, Pred> Instances_sorted(const Pred &) { return ::Udm::InstantiatedAttr< Include, Pred>(impl); };
		Include CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::DerivedAttr< Include> Derived();
		template <class Pred> ::Udm::DerivedAttr< Include, Pred> Derived_sorted(const Pred &) { return ::Udm::DerivedAttr< Include, Pred>(impl); };
		::Udm::ArchetypeAttr< Include> Archetype() const;
		::Udm::StringAttr filename() const;
		::Udm::ParentAttr< ::OIL::OILFile> OILFile_parent() const;
		::Udm::ParentAttr< ::OIL::CPU> CPU_parent() const;
		::Udm::ParentAttr< ::Udm::Object> parent() const;

		static ::Uml::Class meta;
		static ::Uml::Attribute meta_filename;
		static ::Uml::CompositionParentRole meta_OILFile_parent;
		static ::Uml::CompositionParentRole meta_CPU_parent;

	};

	class CPU : public ::Udm::Object {
	public:
		CPU();
		CPU(::Udm::ObjectImpl *impl);
		CPU(const CPU &master);

#ifdef UDM_RVALUE
		CPU(CPU &&master);

		static CPU Cast(::Udm::Object &&a);
		CPU& operator=(CPU &&a);

#endif
		static CPU Cast(const ::Udm::Object &a);
		static CPU Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		CPU CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::InstantiatedAttr< CPU> Instances();
		template <class Pred> ::Udm::InstantiatedAttr< CPU, Pred> Instances_sorted(const Pred &) { return ::Udm::InstantiatedAttr< CPU, Pred>(impl); };
		CPU CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::DerivedAttr< CPU> Derived();
		template <class Pred> ::Udm::DerivedAttr< CPU, Pred> Derived_sorted(const Pred &) { return ::Udm::DerivedAttr< CPU, Pred>(impl); };
		::Udm::ArchetypeAttr< CPU> Archetype() const;
		::Udm::StringAttr name() const;
		::Udm::ChildrenAttr< ::OIL::Include> Include_children() const;
		template <class Pred> ::Udm::ChildrenAttr< ::OIL::Include, Pred> Include_children_sorted(const Pred &) const { return ::Udm::ChildrenAttr< ::OIL::Include, Pred>(impl, meta_Include_children); };
		::Udm::ChildrenAttr< ::OIL::ALARM> ALARM_children() const;
		template <class Pred> ::Udm::ChildrenAttr< ::OIL::ALARM, Pred> ALARM_children_sorted(const Pred &) const { return ::Udm::ChildrenAttr< ::OIL::ALARM, Pred>(impl, meta_ALARM_children); };
		::Udm::ChildrenAttr< ::OIL::EVENT> EVENT_children() const;
		template <class Pred> ::Udm::ChildrenAttr< ::OIL::EVENT, Pred> EVENT_children_sorted(const Pred &) const { return ::Udm::ChildrenAttr< ::OIL::EVENT, Pred>(impl, meta_EVENT_children); };
		::Udm::ChildrenAttr< ::OIL::TASK> TASK_children() const;
		template <class Pred> ::Udm::ChildrenAttr< ::OIL::TASK, Pred> TASK_children_sorted(const Pred &) const { return ::Udm::ChildrenAttr< ::OIL::TASK, Pred>(impl, meta_TASK_children); };
		::Udm::ChildAttr< ::OIL::OIL_COM> OIL_COM_child() const;
		::Udm::ChildAttr< ::OIL::OIL_OS> OIL_OS_child() const;
		::Udm::ChildrenAttr< ::OIL::Include> Include_kind_children() const;
		template <class Pred> ::Udm::ChildrenAttr< ::OIL::Include, Pred> Include_kind_children_sorted(const Pred &) const { return ::Udm::ChildrenAttr< ::OIL::Include, Pred>(impl, ::Udm::NULLCHILDROLE); };
		::Udm::ChildrenAttr< ::OIL::OIL_OS> OIL_OS_kind_children() const;
		template <class Pred> ::Udm::ChildrenAttr< ::OIL::OIL_OS, Pred> OIL_OS_kind_children_sorted(const Pred &) const { return ::Udm::ChildrenAttr< ::OIL::OIL_OS, Pred>(impl, ::Udm::NULLCHILDROLE); };
		::Udm::ChildrenAttr< ::OIL::ALARM> ALARM_kind_children() const;
		template <class Pred> ::Udm::ChildrenAttr< ::OIL::ALARM, Pred> ALARM_kind_children_sorted(const Pred &) const { return ::Udm::ChildrenAttr< ::OIL::ALARM, Pred>(impl, ::Udm::NULLCHILDROLE); };
		::Udm::ChildrenAttr< ::OIL::EVENT> EVENT_kind_children() const;
		template <class Pred> ::Udm::ChildrenAttr< ::OIL::EVENT, Pred> EVENT_kind_children_sorted(const Pred &) const { return ::Udm::ChildrenAttr< ::OIL::EVENT, Pred>(impl, ::Udm::NULLCHILDROLE); };
		::Udm::ChildrenAttr< ::OIL::TASK> TASK_kind_children() const;
		template <class Pred> ::Udm::ChildrenAttr< ::OIL::TASK, Pred> TASK_kind_children_sorted(const Pred &) const { return ::Udm::ChildrenAttr< ::OIL::TASK, Pred>(impl, ::Udm::NULLCHILDROLE); };
		::Udm::ChildrenAttr< ::OIL::OIL_COM> OIL_COM_kind_children() const;
		template <class Pred> ::Udm::ChildrenAttr< ::OIL::OIL_COM, Pred> OIL_COM_kind_children_sorted(const Pred &) const { return ::Udm::ChildrenAttr< ::OIL::OIL_COM, Pred>(impl, ::Udm::NULLCHILDROLE); };
		::Udm::ParentAttr< ::OIL::OILFile> OILFile_parent() const;
		::Udm::ParentAttr< ::OIL::OILFile> parent() const;

		static ::Uml::Class meta;
		static ::Uml::Attribute meta_name;
		static ::Uml::CompositionChildRole meta_Include_children;
		static ::Uml::CompositionChildRole meta_ALARM_children;
		static ::Uml::CompositionChildRole meta_EVENT_children;
		static ::Uml::CompositionChildRole meta_TASK_children;
		static ::Uml::CompositionChildRole meta_OIL_COM_child;
		static ::Uml::CompositionChildRole meta_OIL_OS_child;
		static ::Uml::CompositionParentRole meta_OILFile_parent;

	};

	class OIL_OS : public ::Udm::Object {
	public:
		OIL_OS();
		OIL_OS(::Udm::ObjectImpl *impl);
		OIL_OS(const OIL_OS &master);

#ifdef UDM_RVALUE
		OIL_OS(OIL_OS &&master);

		static OIL_OS Cast(::Udm::Object &&a);
		OIL_OS& operator=(OIL_OS &&a);

#endif
		static OIL_OS Cast(const ::Udm::Object &a);
		static OIL_OS Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		OIL_OS CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::InstantiatedAttr< OIL_OS> Instances();
		template <class Pred> ::Udm::InstantiatedAttr< OIL_OS, Pred> Instances_sorted(const Pred &) { return ::Udm::InstantiatedAttr< OIL_OS, Pred>(impl); };
		OIL_OS CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::DerivedAttr< OIL_OS> Derived();
		template <class Pred> ::Udm::DerivedAttr< OIL_OS, Pred> Derived_sorted(const Pred &) { return ::Udm::DerivedAttr< OIL_OS, Pred>(impl); };
		::Udm::ArchetypeAttr< OIL_OS> Archetype() const;
		::Udm::StringAttr name() const;
		::Udm::StringAttr comment() const;
		::Udm::StringAttr CC() const;
		::Udm::StringAttr STATUS() const;
		::Udm::StringAttr SCHEDULE() const;
		::Udm::BooleanAttr STARTUPHOOK() const;
		::Udm::BooleanAttr SHUTDOWNHOOK() const;
		::Udm::BooleanAttr ERRORHOOK() const;
		::Udm::BooleanAttr PRETASKHOOK() const;
		::Udm::BooleanAttr POSTTASKHOOK() const;
		::Udm::BooleanAttr USEGETSERVICEID() const;
		::Udm::BooleanAttr USEPARAMETERACCESS() const;
		::Udm::IntegerAttr TickTime() const;
		::Udm::ParentAttr< ::OIL::CPU> CPU_parent() const;
		::Udm::ParentAttr< ::OIL::CPU> parent() const;

		static ::Uml::Class meta;
		static ::Uml::Attribute meta_name;
		static ::Uml::Attribute meta_comment;
		static ::Uml::Attribute meta_CC;
		static ::Uml::Attribute meta_STATUS;
		static ::Uml::Attribute meta_SCHEDULE;
		static ::Uml::Attribute meta_STARTUPHOOK;
		static ::Uml::Attribute meta_SHUTDOWNHOOK;
		static ::Uml::Attribute meta_ERRORHOOK;
		static ::Uml::Attribute meta_PRETASKHOOK;
		static ::Uml::Attribute meta_POSTTASKHOOK;
		static ::Uml::Attribute meta_USEGETSERVICEID;
		static ::Uml::Attribute meta_USEPARAMETERACCESS;
		static ::Uml::Attribute meta_TickTime;
		static ::Uml::CompositionParentRole meta_CPU_parent;

	};

	class Procedure : public ::Udm::Object {
	public:
		Procedure();
		Procedure(::Udm::ObjectImpl *impl);
		Procedure(const Procedure &master);

#ifdef UDM_RVALUE
		Procedure(Procedure &&master);

		static Procedure Cast(::Udm::Object &&a);
		Procedure& operator=(Procedure &&a);

#endif
		static Procedure Cast(const ::Udm::Object &a);
		static Procedure Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		Procedure CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::InstantiatedAttr< Procedure> Instances();
		template <class Pred> ::Udm::InstantiatedAttr< Procedure, Pred> Instances_sorted(const Pred &) { return ::Udm::InstantiatedAttr< Procedure, Pred>(impl); };
		Procedure CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::DerivedAttr< Procedure> Derived();
		template <class Pred> ::Udm::DerivedAttr< Procedure, Pred> Derived_sorted(const Pred &) { return ::Udm::DerivedAttr< Procedure, Pred>(impl); };
		::Udm::ArchetypeAttr< Procedure> Archetype() const;
		::Udm::StringAttr name() const;
		::Udm::PointerAttr< EVENT> event() const;
		::Udm::ParentAttr< ::OIL::TASK> TASK_parent() const;
		::Udm::ParentAttr< ::OIL::TASK> parent() const;

		static ::Uml::Class meta;
		static ::Uml::Attribute meta_name;
		static ::Uml::AssociationRole meta_event;
		static ::Uml::CompositionParentRole meta_TASK_parent;

	};

	class ALARM : public ::Udm::Object {
	public:
		ALARM();
		ALARM(::Udm::ObjectImpl *impl);
		ALARM(const ALARM &master);

#ifdef UDM_RVALUE
		ALARM(ALARM &&master);

		static ALARM Cast(::Udm::Object &&a);
		ALARM& operator=(ALARM &&a);

#endif
		static ALARM Cast(const ::Udm::Object &a);
		static ALARM Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		ALARM CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::InstantiatedAttr< ALARM> Instances();
		template <class Pred> ::Udm::InstantiatedAttr< ALARM, Pred> Instances_sorted(const Pred &) { return ::Udm::InstantiatedAttr< ALARM, Pred>(impl); };
		ALARM CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::DerivedAttr< ALARM> Derived();
		template <class Pred> ::Udm::DerivedAttr< ALARM, Pred> Derived_sorted(const Pred &) { return ::Udm::DerivedAttr< ALARM, Pred>(impl); };
		::Udm::ArchetypeAttr< ALARM> Archetype() const;
		::Udm::StringAttr Component() const;
		::Udm::StringAttr name() const;
		::Udm::StringAttr COUNTER() const;
		::Udm::StringAttr ACTION() const;
		::Udm::BooleanAttr AUTOSTART() const;
		::Udm::IntegerAttr ALARMTIME() const;
		::Udm::IntegerAttr CYCLETIME() const;
		::Udm::StringAttr APPMODE() const;
		::Udm::StringAttr AlarmUnit() const;
		::Udm::BooleanAttr StaticAlarm() const;
		::Udm::PointerAttr< EVENT> event() const;
		::Udm::ParentAttr< ::OIL::CPU> CPU_parent() const;
		::Udm::ParentAttr< ::OIL::CPU> parent() const;

		static ::Uml::Class meta;
		static ::Uml::Attribute meta_Component;
		static ::Uml::Attribute meta_name;
		static ::Uml::Attribute meta_COUNTER;
		static ::Uml::Attribute meta_ACTION;
		static ::Uml::Attribute meta_AUTOSTART;
		static ::Uml::Attribute meta_ALARMTIME;
		static ::Uml::Attribute meta_CYCLETIME;
		static ::Uml::Attribute meta_APPMODE;
		static ::Uml::Attribute meta_AlarmUnit;
		static ::Uml::Attribute meta_StaticAlarm;
		static ::Uml::AssociationRole meta_event;
		static ::Uml::CompositionParentRole meta_CPU_parent;

	};

	class EVENT : public ::Udm::Object {
	public:
		EVENT();
		EVENT(::Udm::ObjectImpl *impl);
		EVENT(const EVENT &master);

#ifdef UDM_RVALUE
		EVENT(EVENT &&master);

		static EVENT Cast(::Udm::Object &&a);
		EVENT& operator=(EVENT &&a);

#endif
		static EVENT Cast(const ::Udm::Object &a);
		static EVENT Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		EVENT CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::InstantiatedAttr< EVENT> Instances();
		template <class Pred> ::Udm::InstantiatedAttr< EVENT, Pred> Instances_sorted(const Pred &) { return ::Udm::InstantiatedAttr< EVENT, Pred>(impl); };
		EVENT CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::DerivedAttr< EVENT> Derived();
		template <class Pred> ::Udm::DerivedAttr< EVENT, Pred> Derived_sorted(const Pred &) { return ::Udm::DerivedAttr< EVENT, Pred>(impl); };
		::Udm::ArchetypeAttr< EVENT> Archetype() const;
		::Udm::StringAttr Component() const;
		::Udm::StringAttr name() const;
		::Udm::StringAttr MASK() const;
		::Udm::PointerAttr< ALARM> alarm() const;
		::Udm::AssocAttr< Procedure> procs() const;
		template <class Pred> ::Udm::AssocAttr< Procedure, Pred> procs_sorted(const Pred &) const { return ::Udm::AssocAttr< Procedure, Pred>(impl, meta_procs); };
		::Udm::PointerAttr< TASK> ownerTask() const;
		::Udm::ParentAttr< ::OIL::CPU> CPU_parent() const;
		::Udm::ParentAttr< ::OIL::CPU> parent() const;

		static ::Uml::Class meta;
		static ::Uml::Attribute meta_Component;
		static ::Uml::Attribute meta_name;
		static ::Uml::Attribute meta_MASK;
		static ::Uml::AssociationRole meta_alarm;
		static ::Uml::AssociationRole meta_procs;
		static ::Uml::AssociationRole meta_ownerTask;
		static ::Uml::CompositionParentRole meta_CPU_parent;

	};

	class TASK : public ::Udm::Object {
	public:
		TASK();
		TASK(::Udm::ObjectImpl *impl);
		TASK(const TASK &master);

#ifdef UDM_RVALUE
		TASK(TASK &&master);

		static TASK Cast(::Udm::Object &&a);
		TASK& operator=(TASK &&a);

#endif
		static TASK Cast(const ::Udm::Object &a);
		static TASK Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		TASK CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::InstantiatedAttr< TASK> Instances();
		template <class Pred> ::Udm::InstantiatedAttr< TASK, Pred> Instances_sorted(const Pred &) { return ::Udm::InstantiatedAttr< TASK, Pred>(impl); };
		TASK CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::DerivedAttr< TASK> Derived();
		template <class Pred> ::Udm::DerivedAttr< TASK, Pred> Derived_sorted(const Pred &) { return ::Udm::DerivedAttr< TASK, Pred>(impl); };
		::Udm::ArchetypeAttr< TASK> Archetype() const;
		::Udm::StringAttr Component() const;
		::Udm::StringAttr name() const;
		::Udm::StringAttr TYPE() const;
		::Udm::StringAttr SCHEDULE() const;
		::Udm::IntegerAttr PRIORITY() const;
		::Udm::IntegerAttr ACTIVATION() const;
		::Udm::BooleanAttr AUTOSTART() const;
		::Udm::StringAttr APPMODE() const;
		::Udm::AssocAttr< EVENT> events() const;
		template <class Pred> ::Udm::AssocAttr< EVENT, Pred> events_sorted(const Pred &) const { return ::Udm::AssocAttr< EVENT, Pred>(impl, meta_events); };
		::Udm::ChildAttr< ::OIL::Procedure> Procedure_child() const;
		::Udm::ChildrenAttr< ::OIL::Procedure> Procedure_kind_children() const;
		template <class Pred> ::Udm::ChildrenAttr< ::OIL::Procedure, Pred> Procedure_kind_children_sorted(const Pred &) const { return ::Udm::ChildrenAttr< ::OIL::Procedure, Pred>(impl, ::Udm::NULLCHILDROLE); };
		::Udm::ParentAttr< ::OIL::CPU> CPU_parent() const;
		::Udm::ParentAttr< ::OIL::CPU> parent() const;

		static ::Uml::Class meta;
		static ::Uml::Attribute meta_Component;
		static ::Uml::Attribute meta_name;
		static ::Uml::Attribute meta_TYPE;
		static ::Uml::Attribute meta_SCHEDULE;
		static ::Uml::Attribute meta_PRIORITY;
		static ::Uml::Attribute meta_ACTIVATION;
		static ::Uml::Attribute meta_AUTOSTART;
		static ::Uml::Attribute meta_APPMODE;
		static ::Uml::AssociationRole meta_events;
		static ::Uml::CompositionChildRole meta_Procedure_child;
		static ::Uml::CompositionParentRole meta_CPU_parent;

	};

	class OIL_COM : public ::Udm::Object {
	public:
		OIL_COM();
		OIL_COM(::Udm::ObjectImpl *impl);
		OIL_COM(const OIL_COM &master);

#ifdef UDM_RVALUE
		OIL_COM(OIL_COM &&master);

		static OIL_COM Cast(::Udm::Object &&a);
		OIL_COM& operator=(OIL_COM &&a);

#endif
		static OIL_COM Cast(const ::Udm::Object &a);
		static OIL_COM Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		OIL_COM CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::InstantiatedAttr< OIL_COM> Instances();
		template <class Pred> ::Udm::InstantiatedAttr< OIL_COM, Pred> Instances_sorted(const Pred &) { return ::Udm::InstantiatedAttr< OIL_COM, Pred>(impl); };
		OIL_COM CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::DerivedAttr< OIL_COM> Derived();
		template <class Pred> ::Udm::DerivedAttr< OIL_COM, Pred> Derived_sorted(const Pred &) { return ::Udm::DerivedAttr< OIL_COM, Pred>(impl); };
		::Udm::ArchetypeAttr< OIL_COM> Archetype() const;
		::Udm::StringAttr name() const;
		::Udm::StringAttr comment() const;
		::Udm::BooleanAttr USEMESSAGERESOURCE() const;
		::Udm::BooleanAttr USEMESSAGESTATUS() const;
		::Udm::ParentAttr< ::OIL::CPU> CPU_parent() const;
		::Udm::ParentAttr< ::OIL::CPU> parent() const;

		static ::Uml::Class meta;
		static ::Uml::Attribute meta_name;
		static ::Uml::Attribute meta_comment;
		static ::Uml::Attribute meta_USEMESSAGERESOURCE;
		static ::Uml::Attribute meta_USEMESSAGESTATUS;
		static ::Uml::CompositionParentRole meta_CPU_parent;

	};

}

#endif // MOBIES_OIL_H
