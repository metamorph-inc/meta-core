#ifndef MOBIES_CADANALYSISMETADATA_H
#define MOBIES_CADANALYSISMETADATA_H

// header file CADAnalysisMetaData.h generated from diagram CADAnalysisMetaData
// generated with Udm version 3.31 on Mon Dec 02 17:13:33 2013

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

namespace CADAnalysisMetaData {

	extern ::Uml::Diagram meta;
	class Metric;
	class Metrics;
	class LifeCycle;
	class AnalysisSupportingData;
	class AllowableBearingStress;
	class AllowableShearStress;
	class AllowableTensileStress;
	class Materials;
	class Material;
	class MaterialProperties;
	class Assemblies;
	class Component;
	class CADAnalysisMetaData;
	class Assembly;

	void Initialize();
	void Initialize(const ::Uml::Diagram &dgr);

	extern  ::Udm::UdmDiagram diagram;

	class Metric : public ::Udm::Object {
	public:
		Metric();
		Metric(::Udm::ObjectImpl *impl);
		Metric(const Metric &master);

#ifdef UDM_RVALUE
		Metric(Metric &&master);

		static Metric Cast(::Udm::Object &&a);
		Metric& operator=(Metric &&a);

#endif
		static Metric Cast(const ::Udm::Object &a);
		static Metric Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		Metric CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::InstantiatedAttr< Metric> Instances();
		template <class Pred> ::Udm::InstantiatedAttr< Metric, Pred> Instances_sorted(const Pred &) { return ::Udm::InstantiatedAttr< Metric, Pred>(impl); };
		Metric CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::DerivedAttr< Metric> Derived();
		template <class Pred> ::Udm::DerivedAttr< Metric, Pred> Derived_sorted(const Pred &) { return ::Udm::DerivedAttr< Metric, Pred>(impl); };
		::Udm::ArchetypeAttr< Metric> Archetype() const;
		::Udm::StringAttr ConfigurationID() const;
		::Udm::StringAttr MetricType() const;
		::Udm::StringAttr TopAssemblyComponentInstanceID() const;
		::Udm::StringAttr ComponenInstancetID() const;
		::Udm::StringAttr ComponentName() const;
		::Udm::StringAttr ComponentType() const;
		::Udm::StringAttr RequestedValueType() const;
		::Udm::StringAttr MetricID() const;
		::Udm::StringAttr Details() const;
		::Udm::ParentAttr< ::CADAnalysisMetaData::Metrics> Metrics_parent() const;
		::Udm::ParentAttr< ::CADAnalysisMetaData::Metrics> parent() const;

		static ::Uml::Class meta;
		static ::Uml::Attribute meta_ConfigurationID;
		static ::Uml::Attribute meta_MetricType;
		static ::Uml::Attribute meta_TopAssemblyComponentInstanceID;
		static ::Uml::Attribute meta_ComponenInstancetID;
		static ::Uml::Attribute meta_ComponentName;
		static ::Uml::Attribute meta_ComponentType;
		static ::Uml::Attribute meta_RequestedValueType;
		static ::Uml::Attribute meta_MetricID;
		static ::Uml::Attribute meta_Details;
		static ::Uml::CompositionParentRole meta_Metrics_parent;

	};

	class Metrics : public ::Udm::Object {
	public:
		Metrics();
		Metrics(::Udm::ObjectImpl *impl);
		Metrics(const Metrics &master);

#ifdef UDM_RVALUE
		Metrics(Metrics &&master);

		static Metrics Cast(::Udm::Object &&a);
		Metrics& operator=(Metrics &&a);

#endif
		static Metrics Cast(const ::Udm::Object &a);
		static Metrics Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		Metrics CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::InstantiatedAttr< Metrics> Instances();
		template <class Pred> ::Udm::InstantiatedAttr< Metrics, Pred> Instances_sorted(const Pred &) { return ::Udm::InstantiatedAttr< Metrics, Pred>(impl); };
		Metrics CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::DerivedAttr< Metrics> Derived();
		template <class Pred> ::Udm::DerivedAttr< Metrics, Pred> Derived_sorted(const Pred &) { return ::Udm::DerivedAttr< Metrics, Pred>(impl); };
		::Udm::ArchetypeAttr< Metrics> Archetype() const;
		::Udm::ChildrenAttr< ::CADAnalysisMetaData::Metric> Metric_children() const;
		template <class Pred> ::Udm::ChildrenAttr< ::CADAnalysisMetaData::Metric, Pred> Metric_children_sorted(const Pred &) const { return ::Udm::ChildrenAttr< ::CADAnalysisMetaData::Metric, Pred>(impl, meta_Metric_children); };
		::Udm::ChildrenAttr< ::CADAnalysisMetaData::Metric> Metric_kind_children() const;
		template <class Pred> ::Udm::ChildrenAttr< ::CADAnalysisMetaData::Metric, Pred> Metric_kind_children_sorted(const Pred &) const { return ::Udm::ChildrenAttr< ::CADAnalysisMetaData::Metric, Pred>(impl, ::Udm::NULLCHILDROLE); };
		::Udm::ParentAttr< ::Udm::Object> parent() const;

		static ::Uml::Class meta;
		static ::Uml::CompositionChildRole meta_Metric_children;

	};

	class LifeCycle : public ::Udm::Object {
	public:
		LifeCycle();
		LifeCycle(::Udm::ObjectImpl *impl);
		LifeCycle(const LifeCycle &master);

#ifdef UDM_RVALUE
		LifeCycle(LifeCycle &&master);

		static LifeCycle Cast(::Udm::Object &&a);
		LifeCycle& operator=(LifeCycle &&a);

#endif
		static LifeCycle Cast(const ::Udm::Object &a);
		static LifeCycle Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		LifeCycle CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::InstantiatedAttr< LifeCycle> Instances();
		template <class Pred> ::Udm::InstantiatedAttr< LifeCycle, Pred> Instances_sorted(const Pred &) { return ::Udm::InstantiatedAttr< LifeCycle, Pred>(impl); };
		LifeCycle CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::DerivedAttr< LifeCycle> Derived();
		template <class Pred> ::Udm::DerivedAttr< LifeCycle, Pred> Derived_sorted(const Pred &) { return ::Udm::DerivedAttr< LifeCycle, Pred>(impl); };
		::Udm::ArchetypeAttr< LifeCycle> Archetype() const;
		::Udm::StringAttr Duration() const;
		::Udm::IntegerAttr NumberOfCycles() const;
		::Udm::ParentAttr< ::CADAnalysisMetaData::AnalysisSupportingData> AnalysisSupportingData_parent() const;
		::Udm::ParentAttr< ::CADAnalysisMetaData::AnalysisSupportingData> parent() const;

		static ::Uml::Class meta;
		static ::Uml::Attribute meta_Duration;
		static ::Uml::Attribute meta_NumberOfCycles;
		static ::Uml::CompositionParentRole meta_AnalysisSupportingData_parent;

	};

	class AnalysisSupportingData : public ::Udm::Object {
	public:
		AnalysisSupportingData();
		AnalysisSupportingData(::Udm::ObjectImpl *impl);
		AnalysisSupportingData(const AnalysisSupportingData &master);

#ifdef UDM_RVALUE
		AnalysisSupportingData(AnalysisSupportingData &&master);

		static AnalysisSupportingData Cast(::Udm::Object &&a);
		AnalysisSupportingData& operator=(AnalysisSupportingData &&a);

#endif
		static AnalysisSupportingData Cast(const ::Udm::Object &a);
		static AnalysisSupportingData Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		AnalysisSupportingData CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::InstantiatedAttr< AnalysisSupportingData> Instances();
		template <class Pred> ::Udm::InstantiatedAttr< AnalysisSupportingData, Pred> Instances_sorted(const Pred &) { return ::Udm::InstantiatedAttr< AnalysisSupportingData, Pred>(impl); };
		AnalysisSupportingData CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::DerivedAttr< AnalysisSupportingData> Derived();
		template <class Pred> ::Udm::DerivedAttr< AnalysisSupportingData, Pred> Derived_sorted(const Pred &) { return ::Udm::DerivedAttr< AnalysisSupportingData, Pred>(impl); };
		::Udm::ArchetypeAttr< AnalysisSupportingData> Archetype() const;
		::Udm::StringAttr AnalysisType() const;
		::Udm::ChildAttr< ::CADAnalysisMetaData::LifeCycle> LifeCycle_child() const;
		::Udm::ChildrenAttr< ::CADAnalysisMetaData::LifeCycle> LifeCycle_kind_children() const;
		template <class Pred> ::Udm::ChildrenAttr< ::CADAnalysisMetaData::LifeCycle, Pred> LifeCycle_kind_children_sorted(const Pred &) const { return ::Udm::ChildrenAttr< ::CADAnalysisMetaData::LifeCycle, Pred>(impl, ::Udm::NULLCHILDROLE); };
		::Udm::ParentAttr< ::CADAnalysisMetaData::CADAnalysisMetaData> CADAnalysisMetaData_parent() const;
		::Udm::ParentAttr< ::CADAnalysisMetaData::CADAnalysisMetaData> parent() const;

		static ::Uml::Class meta;
		static ::Uml::Attribute meta_AnalysisType;
		static ::Uml::CompositionChildRole meta_LifeCycle_child;
		static ::Uml::CompositionParentRole meta_CADAnalysisMetaData_parent;

	};

	class AllowableBearingStress : public ::Udm::Object {
	public:
		AllowableBearingStress();
		AllowableBearingStress(::Udm::ObjectImpl *impl);
		AllowableBearingStress(const AllowableBearingStress &master);

#ifdef UDM_RVALUE
		AllowableBearingStress(AllowableBearingStress &&master);

		static AllowableBearingStress Cast(::Udm::Object &&a);
		AllowableBearingStress& operator=(AllowableBearingStress &&a);

#endif
		static AllowableBearingStress Cast(const ::Udm::Object &a);
		static AllowableBearingStress Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		AllowableBearingStress CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::InstantiatedAttr< AllowableBearingStress> Instances();
		template <class Pred> ::Udm::InstantiatedAttr< AllowableBearingStress, Pred> Instances_sorted(const Pred &) { return ::Udm::InstantiatedAttr< AllowableBearingStress, Pred>(impl); };
		AllowableBearingStress CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::DerivedAttr< AllowableBearingStress> Derived();
		template <class Pred> ::Udm::DerivedAttr< AllowableBearingStress, Pred> Derived_sorted(const Pred &) { return ::Udm::DerivedAttr< AllowableBearingStress, Pred>(impl); };
		::Udm::ArchetypeAttr< AllowableBearingStress> Archetype() const;
		::Udm::RealAttr Value() const;
		::Udm::StringAttr Units() const;
		::Udm::StringAttr Source() const;
		::Udm::ParentAttr< ::CADAnalysisMetaData::MaterialProperties> MaterialProperties_parent() const;
		::Udm::ParentAttr< ::CADAnalysisMetaData::MaterialProperties> parent() const;

		static ::Uml::Class meta;
		static ::Uml::Attribute meta_Value;
		static ::Uml::Attribute meta_Units;
		static ::Uml::Attribute meta_Source;
		static ::Uml::CompositionParentRole meta_MaterialProperties_parent;

	};

	class AllowableShearStress : public ::Udm::Object {
	public:
		AllowableShearStress();
		AllowableShearStress(::Udm::ObjectImpl *impl);
		AllowableShearStress(const AllowableShearStress &master);

#ifdef UDM_RVALUE
		AllowableShearStress(AllowableShearStress &&master);

		static AllowableShearStress Cast(::Udm::Object &&a);
		AllowableShearStress& operator=(AllowableShearStress &&a);

#endif
		static AllowableShearStress Cast(const ::Udm::Object &a);
		static AllowableShearStress Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		AllowableShearStress CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::InstantiatedAttr< AllowableShearStress> Instances();
		template <class Pred> ::Udm::InstantiatedAttr< AllowableShearStress, Pred> Instances_sorted(const Pred &) { return ::Udm::InstantiatedAttr< AllowableShearStress, Pred>(impl); };
		AllowableShearStress CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::DerivedAttr< AllowableShearStress> Derived();
		template <class Pred> ::Udm::DerivedAttr< AllowableShearStress, Pred> Derived_sorted(const Pred &) { return ::Udm::DerivedAttr< AllowableShearStress, Pred>(impl); };
		::Udm::ArchetypeAttr< AllowableShearStress> Archetype() const;
		::Udm::RealAttr Value() const;
		::Udm::StringAttr Units() const;
		::Udm::StringAttr Source() const;
		::Udm::ParentAttr< ::CADAnalysisMetaData::MaterialProperties> MaterialProperties_parent() const;
		::Udm::ParentAttr< ::CADAnalysisMetaData::MaterialProperties> parent() const;

		static ::Uml::Class meta;
		static ::Uml::Attribute meta_Value;
		static ::Uml::Attribute meta_Units;
		static ::Uml::Attribute meta_Source;
		static ::Uml::CompositionParentRole meta_MaterialProperties_parent;

	};

	class AllowableTensileStress : public ::Udm::Object {
	public:
		AllowableTensileStress();
		AllowableTensileStress(::Udm::ObjectImpl *impl);
		AllowableTensileStress(const AllowableTensileStress &master);

#ifdef UDM_RVALUE
		AllowableTensileStress(AllowableTensileStress &&master);

		static AllowableTensileStress Cast(::Udm::Object &&a);
		AllowableTensileStress& operator=(AllowableTensileStress &&a);

#endif
		static AllowableTensileStress Cast(const ::Udm::Object &a);
		static AllowableTensileStress Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		AllowableTensileStress CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::InstantiatedAttr< AllowableTensileStress> Instances();
		template <class Pred> ::Udm::InstantiatedAttr< AllowableTensileStress, Pred> Instances_sorted(const Pred &) { return ::Udm::InstantiatedAttr< AllowableTensileStress, Pred>(impl); };
		AllowableTensileStress CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::DerivedAttr< AllowableTensileStress> Derived();
		template <class Pred> ::Udm::DerivedAttr< AllowableTensileStress, Pred> Derived_sorted(const Pred &) { return ::Udm::DerivedAttr< AllowableTensileStress, Pred>(impl); };
		::Udm::ArchetypeAttr< AllowableTensileStress> Archetype() const;
		::Udm::RealAttr Value() const;
		::Udm::StringAttr Units() const;
		::Udm::StringAttr Source() const;
		::Udm::ParentAttr< ::CADAnalysisMetaData::MaterialProperties> MaterialProperties_parent() const;
		::Udm::ParentAttr< ::CADAnalysisMetaData::MaterialProperties> parent() const;

		static ::Uml::Class meta;
		static ::Uml::Attribute meta_Value;
		static ::Uml::Attribute meta_Units;
		static ::Uml::Attribute meta_Source;
		static ::Uml::CompositionParentRole meta_MaterialProperties_parent;

	};

	class Materials : public ::Udm::Object {
	public:
		Materials();
		Materials(::Udm::ObjectImpl *impl);
		Materials(const Materials &master);

#ifdef UDM_RVALUE
		Materials(Materials &&master);

		static Materials Cast(::Udm::Object &&a);
		Materials& operator=(Materials &&a);

#endif
		static Materials Cast(const ::Udm::Object &a);
		static Materials Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		Materials CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::InstantiatedAttr< Materials> Instances();
		template <class Pred> ::Udm::InstantiatedAttr< Materials, Pred> Instances_sorted(const Pred &) { return ::Udm::InstantiatedAttr< Materials, Pred>(impl); };
		Materials CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::DerivedAttr< Materials> Derived();
		template <class Pred> ::Udm::DerivedAttr< Materials, Pred> Derived_sorted(const Pred &) { return ::Udm::DerivedAttr< Materials, Pred>(impl); };
		::Udm::ArchetypeAttr< Materials> Archetype() const;
		::Udm::ChildrenAttr< ::CADAnalysisMetaData::Material> Material_children() const;
		template <class Pred> ::Udm::ChildrenAttr< ::CADAnalysisMetaData::Material, Pred> Material_children_sorted(const Pred &) const { return ::Udm::ChildrenAttr< ::CADAnalysisMetaData::Material, Pred>(impl, meta_Material_children); };
		::Udm::ChildrenAttr< ::CADAnalysisMetaData::Material> Material_kind_children() const;
		template <class Pred> ::Udm::ChildrenAttr< ::CADAnalysisMetaData::Material, Pred> Material_kind_children_sorted(const Pred &) const { return ::Udm::ChildrenAttr< ::CADAnalysisMetaData::Material, Pred>(impl, ::Udm::NULLCHILDROLE); };
		::Udm::ParentAttr< ::CADAnalysisMetaData::CADAnalysisMetaData> CADAnalysisMetaData_parent() const;
		::Udm::ParentAttr< ::CADAnalysisMetaData::CADAnalysisMetaData> parent() const;

		static ::Uml::Class meta;
		static ::Uml::CompositionChildRole meta_Material_children;
		static ::Uml::CompositionParentRole meta_CADAnalysisMetaData_parent;

	};

	class Material : public ::Udm::Object {
	public:
		Material();
		Material(::Udm::ObjectImpl *impl);
		Material(const Material &master);

#ifdef UDM_RVALUE
		Material(Material &&master);

		static Material Cast(::Udm::Object &&a);
		Material& operator=(Material &&a);

#endif
		static Material Cast(const ::Udm::Object &a);
		static Material Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		Material CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::InstantiatedAttr< Material> Instances();
		template <class Pred> ::Udm::InstantiatedAttr< Material, Pred> Instances_sorted(const Pred &) { return ::Udm::InstantiatedAttr< Material, Pred>(impl); };
		Material CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::DerivedAttr< Material> Derived();
		template <class Pred> ::Udm::DerivedAttr< Material, Pred> Derived_sorted(const Pred &) { return ::Udm::DerivedAttr< Material, Pred>(impl); };
		::Udm::ArchetypeAttr< Material> Archetype() const;
		::Udm::StringAttr MaterialID() const;
		::Udm::ChildrenAttr< ::CADAnalysisMetaData::MaterialProperties> MaterialProperties_children() const;
		template <class Pred> ::Udm::ChildrenAttr< ::CADAnalysisMetaData::MaterialProperties, Pred> MaterialProperties_children_sorted(const Pred &) const { return ::Udm::ChildrenAttr< ::CADAnalysisMetaData::MaterialProperties, Pred>(impl, meta_MaterialProperties_children); };
		::Udm::ChildrenAttr< ::CADAnalysisMetaData::MaterialProperties> MaterialProperties_kind_children() const;
		template <class Pred> ::Udm::ChildrenAttr< ::CADAnalysisMetaData::MaterialProperties, Pred> MaterialProperties_kind_children_sorted(const Pred &) const { return ::Udm::ChildrenAttr< ::CADAnalysisMetaData::MaterialProperties, Pred>(impl, ::Udm::NULLCHILDROLE); };
		::Udm::ParentAttr< ::CADAnalysisMetaData::Materials> Materials_parent() const;
		::Udm::ParentAttr< ::CADAnalysisMetaData::Materials> parent() const;

		static ::Uml::Class meta;
		static ::Uml::Attribute meta_MaterialID;
		static ::Uml::CompositionChildRole meta_MaterialProperties_children;
		static ::Uml::CompositionParentRole meta_Materials_parent;

	};

	class MaterialProperties : public ::Udm::Object {
	public:
		MaterialProperties();
		MaterialProperties(::Udm::ObjectImpl *impl);
		MaterialProperties(const MaterialProperties &master);

#ifdef UDM_RVALUE
		MaterialProperties(MaterialProperties &&master);

		static MaterialProperties Cast(::Udm::Object &&a);
		MaterialProperties& operator=(MaterialProperties &&a);

#endif
		static MaterialProperties Cast(const ::Udm::Object &a);
		static MaterialProperties Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		MaterialProperties CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::InstantiatedAttr< MaterialProperties> Instances();
		template <class Pred> ::Udm::InstantiatedAttr< MaterialProperties, Pred> Instances_sorted(const Pred &) { return ::Udm::InstantiatedAttr< MaterialProperties, Pred>(impl); };
		MaterialProperties CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::DerivedAttr< MaterialProperties> Derived();
		template <class Pred> ::Udm::DerivedAttr< MaterialProperties, Pred> Derived_sorted(const Pred &) { return ::Udm::DerivedAttr< MaterialProperties, Pred>(impl); };
		::Udm::ArchetypeAttr< MaterialProperties> Archetype() const;
		::Udm::ChildAttr< ::CADAnalysisMetaData::AllowableBearingStress> AllowableBearingStress_child() const;
		::Udm::ChildAttr< ::CADAnalysisMetaData::AllowableShearStress> AllowableShearStress_child() const;
		::Udm::ChildAttr< ::CADAnalysisMetaData::AllowableTensileStress> AllowableTensileStress_child() const;
		::Udm::ChildrenAttr< ::CADAnalysisMetaData::AllowableBearingStress> AllowableBearingStress_kind_children() const;
		template <class Pred> ::Udm::ChildrenAttr< ::CADAnalysisMetaData::AllowableBearingStress, Pred> AllowableBearingStress_kind_children_sorted(const Pred &) const { return ::Udm::ChildrenAttr< ::CADAnalysisMetaData::AllowableBearingStress, Pred>(impl, ::Udm::NULLCHILDROLE); };
		::Udm::ChildrenAttr< ::CADAnalysisMetaData::AllowableShearStress> AllowableShearStress_kind_children() const;
		template <class Pred> ::Udm::ChildrenAttr< ::CADAnalysisMetaData::AllowableShearStress, Pred> AllowableShearStress_kind_children_sorted(const Pred &) const { return ::Udm::ChildrenAttr< ::CADAnalysisMetaData::AllowableShearStress, Pred>(impl, ::Udm::NULLCHILDROLE); };
		::Udm::ChildrenAttr< ::CADAnalysisMetaData::AllowableTensileStress> AllowableTensileStress_kind_children() const;
		template <class Pred> ::Udm::ChildrenAttr< ::CADAnalysisMetaData::AllowableTensileStress, Pred> AllowableTensileStress_kind_children_sorted(const Pred &) const { return ::Udm::ChildrenAttr< ::CADAnalysisMetaData::AllowableTensileStress, Pred>(impl, ::Udm::NULLCHILDROLE); };
		::Udm::ParentAttr< ::CADAnalysisMetaData::Material> Material_parent() const;
		::Udm::ParentAttr< ::CADAnalysisMetaData::Material> parent() const;

		static ::Uml::Class meta;
		static ::Uml::CompositionChildRole meta_AllowableBearingStress_child;
		static ::Uml::CompositionChildRole meta_AllowableShearStress_child;
		static ::Uml::CompositionChildRole meta_AllowableTensileStress_child;
		static ::Uml::CompositionParentRole meta_Material_parent;

	};

	class Assemblies : public ::Udm::Object {
	public:
		Assemblies();
		Assemblies(::Udm::ObjectImpl *impl);
		Assemblies(const Assemblies &master);

#ifdef UDM_RVALUE
		Assemblies(Assemblies &&master);

		static Assemblies Cast(::Udm::Object &&a);
		Assemblies& operator=(Assemblies &&a);

#endif
		static Assemblies Cast(const ::Udm::Object &a);
		static Assemblies Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		Assemblies CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::InstantiatedAttr< Assemblies> Instances();
		template <class Pred> ::Udm::InstantiatedAttr< Assemblies, Pred> Instances_sorted(const Pred &) { return ::Udm::InstantiatedAttr< Assemblies, Pred>(impl); };
		Assemblies CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::DerivedAttr< Assemblies> Derived();
		template <class Pred> ::Udm::DerivedAttr< Assemblies, Pred> Derived_sorted(const Pred &) { return ::Udm::DerivedAttr< Assemblies, Pred>(impl); };
		::Udm::ArchetypeAttr< Assemblies> Archetype() const;
		::Udm::ChildrenAttr< ::CADAnalysisMetaData::Assembly> Assembly_children() const;
		template <class Pred> ::Udm::ChildrenAttr< ::CADAnalysisMetaData::Assembly, Pred> Assembly_children_sorted(const Pred &) const { return ::Udm::ChildrenAttr< ::CADAnalysisMetaData::Assembly, Pred>(impl, meta_Assembly_children); };
		::Udm::ChildrenAttr< ::CADAnalysisMetaData::Assembly> Assembly_kind_children() const;
		template <class Pred> ::Udm::ChildrenAttr< ::CADAnalysisMetaData::Assembly, Pred> Assembly_kind_children_sorted(const Pred &) const { return ::Udm::ChildrenAttr< ::CADAnalysisMetaData::Assembly, Pred>(impl, ::Udm::NULLCHILDROLE); };
		::Udm::ParentAttr< ::CADAnalysisMetaData::CADAnalysisMetaData> CADAnalysisMetaData_parent() const;
		::Udm::ParentAttr< ::CADAnalysisMetaData::CADAnalysisMetaData> parent() const;

		static ::Uml::Class meta;
		static ::Uml::CompositionChildRole meta_Assembly_children;
		static ::Uml::CompositionParentRole meta_CADAnalysisMetaData_parent;

	};

	class Component : public ::Udm::Object {
	public:
		Component();
		Component(::Udm::ObjectImpl *impl);
		Component(const Component &master);

#ifdef UDM_RVALUE
		Component(Component &&master);

		static Component Cast(::Udm::Object &&a);
		Component& operator=(Component &&a);

#endif
		static Component Cast(const ::Udm::Object &a);
		static Component Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		Component CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::InstantiatedAttr< Component> Instances();
		template <class Pred> ::Udm::InstantiatedAttr< Component, Pred> Instances_sorted(const Pred &) { return ::Udm::InstantiatedAttr< Component, Pred>(impl); };
		Component CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::DerivedAttr< Component> Derived();
		template <class Pred> ::Udm::DerivedAttr< Component, Pred> Derived_sorted(const Pred &) { return ::Udm::DerivedAttr< Component, Pred>(impl); };
		::Udm::ArchetypeAttr< Component> Archetype() const;
		::Udm::StringAttr Name() const;
		::Udm::StringAttr Type() const;
		::Udm::StringAttr MaterialID() const;
		::Udm::StringAttr ComponentInstanceID() const;
		::Udm::StringAttr FEAElementType() const;
		::Udm::StringAttr FEAElementID() const;
		::Udm::ChildrenAttr< ::CADAnalysisMetaData::Component> Component_children() const;
		template <class Pred> ::Udm::ChildrenAttr< ::CADAnalysisMetaData::Component, Pred> Component_children_sorted(const Pred &) const { return ::Udm::ChildrenAttr< ::CADAnalysisMetaData::Component, Pred>(impl, meta_Component_children); };
		::Udm::ChildrenAttr< ::CADAnalysisMetaData::Component> Component_kind_children() const;
		template <class Pred> ::Udm::ChildrenAttr< ::CADAnalysisMetaData::Component, Pred> Component_kind_children_sorted(const Pred &) const { return ::Udm::ChildrenAttr< ::CADAnalysisMetaData::Component, Pred>(impl, ::Udm::NULLCHILDROLE); };
		::Udm::ParentAttr< ::CADAnalysisMetaData::Component> Component_parent() const;
		::Udm::ParentAttr< ::CADAnalysisMetaData::Assembly> Assembly_parent() const;
		::Udm::ParentAttr< ::Udm::Object> parent() const;

		static ::Uml::Class meta;
		static ::Uml::Attribute meta_Name;
		static ::Uml::Attribute meta_Type;
		static ::Uml::Attribute meta_MaterialID;
		static ::Uml::Attribute meta_ComponentInstanceID;
		static ::Uml::Attribute meta_FEAElementType;
		static ::Uml::Attribute meta_FEAElementID;
		static ::Uml::CompositionChildRole meta_Component_children;
		static ::Uml::CompositionParentRole meta_Component_parent;
		static ::Uml::CompositionParentRole meta_Assembly_parent;

	};

	class CADAnalysisMetaData : public ::Udm::Object {
	public:
		CADAnalysisMetaData();
		CADAnalysisMetaData(::Udm::ObjectImpl *impl);
		CADAnalysisMetaData(const CADAnalysisMetaData &master);

#ifdef UDM_RVALUE
		CADAnalysisMetaData(CADAnalysisMetaData &&master);

		static CADAnalysisMetaData Cast(::Udm::Object &&a);
		CADAnalysisMetaData& operator=(CADAnalysisMetaData &&a);

#endif
		static CADAnalysisMetaData Cast(const ::Udm::Object &a);
		static CADAnalysisMetaData Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		CADAnalysisMetaData CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::InstantiatedAttr< CADAnalysisMetaData> Instances();
		template <class Pred> ::Udm::InstantiatedAttr< CADAnalysisMetaData, Pred> Instances_sorted(const Pred &) { return ::Udm::InstantiatedAttr< CADAnalysisMetaData, Pred>(impl); };
		CADAnalysisMetaData CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::DerivedAttr< CADAnalysisMetaData> Derived();
		template <class Pred> ::Udm::DerivedAttr< CADAnalysisMetaData, Pred> Derived_sorted(const Pred &) { return ::Udm::DerivedAttr< CADAnalysisMetaData, Pred>(impl); };
		::Udm::ArchetypeAttr< CADAnalysisMetaData> Archetype() const;
		::Udm::ChildAttr< ::CADAnalysisMetaData::AnalysisSupportingData> AnalysisSupportingData_child() const;
		::Udm::ChildAttr< ::CADAnalysisMetaData::Materials> Materials_child() const;
		::Udm::ChildAttr< ::CADAnalysisMetaData::Assemblies> Assemblies_child() const;
		::Udm::ChildrenAttr< ::CADAnalysisMetaData::AnalysisSupportingData> AnalysisSupportingData_kind_children() const;
		template <class Pred> ::Udm::ChildrenAttr< ::CADAnalysisMetaData::AnalysisSupportingData, Pred> AnalysisSupportingData_kind_children_sorted(const Pred &) const { return ::Udm::ChildrenAttr< ::CADAnalysisMetaData::AnalysisSupportingData, Pred>(impl, ::Udm::NULLCHILDROLE); };
		::Udm::ChildrenAttr< ::CADAnalysisMetaData::Materials> Materials_kind_children() const;
		template <class Pred> ::Udm::ChildrenAttr< ::CADAnalysisMetaData::Materials, Pred> Materials_kind_children_sorted(const Pred &) const { return ::Udm::ChildrenAttr< ::CADAnalysisMetaData::Materials, Pred>(impl, ::Udm::NULLCHILDROLE); };
		::Udm::ChildrenAttr< ::CADAnalysisMetaData::Assemblies> Assemblies_kind_children() const;
		template <class Pred> ::Udm::ChildrenAttr< ::CADAnalysisMetaData::Assemblies, Pred> Assemblies_kind_children_sorted(const Pred &) const { return ::Udm::ChildrenAttr< ::CADAnalysisMetaData::Assemblies, Pred>(impl, ::Udm::NULLCHILDROLE); };
		::Udm::ParentAttr< ::Udm::Object> parent() const;

		static ::Uml::Class meta;
		static ::Uml::CompositionChildRole meta_AnalysisSupportingData_child;
		static ::Uml::CompositionChildRole meta_Materials_child;
		static ::Uml::CompositionChildRole meta_Assemblies_child;

	};

	class Assembly : public ::Udm::Object {
	public:
		Assembly();
		Assembly(::Udm::ObjectImpl *impl);
		Assembly(const Assembly &master);

#ifdef UDM_RVALUE
		Assembly(Assembly &&master);

		static Assembly Cast(::Udm::Object &&a);
		Assembly& operator=(Assembly &&a);

#endif
		static Assembly Cast(const ::Udm::Object &a);
		static Assembly Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		Assembly CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::InstantiatedAttr< Assembly> Instances();
		template <class Pred> ::Udm::InstantiatedAttr< Assembly, Pred> Instances_sorted(const Pred &) { return ::Udm::InstantiatedAttr< Assembly, Pred>(impl); };
		Assembly CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role = ::Udm::NULLCHILDROLE);
		::Udm::DerivedAttr< Assembly> Derived();
		template <class Pred> ::Udm::DerivedAttr< Assembly, Pred> Derived_sorted(const Pred &) { return ::Udm::DerivedAttr< Assembly, Pred>(impl); };
		::Udm::ArchetypeAttr< Assembly> Archetype() const;
		::Udm::StringAttr ConfigurationID() const;
		::Udm::ChildrenAttr< ::CADAnalysisMetaData::Component> Component_children() const;
		template <class Pred> ::Udm::ChildrenAttr< ::CADAnalysisMetaData::Component, Pred> Component_children_sorted(const Pred &) const { return ::Udm::ChildrenAttr< ::CADAnalysisMetaData::Component, Pred>(impl, meta_Component_children); };
		::Udm::ChildrenAttr< ::CADAnalysisMetaData::Component> Component_kind_children() const;
		template <class Pred> ::Udm::ChildrenAttr< ::CADAnalysisMetaData::Component, Pred> Component_kind_children_sorted(const Pred &) const { return ::Udm::ChildrenAttr< ::CADAnalysisMetaData::Component, Pred>(impl, ::Udm::NULLCHILDROLE); };
		::Udm::ParentAttr< ::CADAnalysisMetaData::Assemblies> Assemblies_parent() const;
		::Udm::ParentAttr< ::CADAnalysisMetaData::Assemblies> parent() const;

		static ::Uml::Class meta;
		static ::Uml::Attribute meta_ConfigurationID;
		static ::Uml::CompositionChildRole meta_Component_children;
		static ::Uml::CompositionParentRole meta_Assemblies_parent;

	};

}

#endif // MOBIES_CADANALYSISMETADATA_H
