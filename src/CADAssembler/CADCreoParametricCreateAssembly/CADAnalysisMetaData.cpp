// cpp (meta datanetwork format) source file CADAnalysisMetaData.cpp
// generated from diagram CADAnalysisMetaData
// generated on Mon Dec 02 17:13:33 2013
#include "stdafx.h"

#include "CADAnalysisMetaData.h"
#include <UmlExt.h>
#include <UdmStatic.h>

#include <UdmDom.h>
#include "CADAnalysisMetaData_xsd.h"
using namespace std;

namespace CADAnalysisMetaData {

	Metric::Metric() {}
	Metric::Metric(::Udm::ObjectImpl *impl) : UDM_OBJECT(impl) {}
	Metric::Metric(const Metric &master) : UDM_OBJECT(master) {}

#ifdef UDM_RVALUE
	Metric::Metric(Metric &&master) : UDM_OBJECT(master) {};

	Metric Metric::Cast(::Udm::Object &&a) { return __Cast(std::move(a), meta); };
	Metric& Metric::operator=(Metric &&a) { ::Udm::Object::operator =(std::move(a)); return *this; };

#endif
	Metric Metric::Cast(const ::Udm::Object &a) { return __Cast(a, meta); }
	Metric Metric::Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role); }
	Metric Metric::CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl); }
	::Udm::InstantiatedAttr< Metric> Metric::Instances() { return ::Udm::InstantiatedAttr< Metric>(impl); }
	Metric Metric::CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl, true); }
	::Udm::DerivedAttr< Metric> Metric::Derived() { return ::Udm::DerivedAttr< Metric>(impl); }
	::Udm::ArchetypeAttr< Metric> Metric::Archetype() const { return ::Udm::ArchetypeAttr< Metric>(impl); }
	::Udm::StringAttr Metric::ConfigurationID() const { return ::Udm::StringAttr(impl, meta_ConfigurationID); }
	::Udm::StringAttr Metric::MetricType() const { return ::Udm::StringAttr(impl, meta_MetricType); }
	::Udm::StringAttr Metric::TopAssemblyComponentInstanceID() const { return ::Udm::StringAttr(impl, meta_TopAssemblyComponentInstanceID); }
	::Udm::StringAttr Metric::ComponenInstancetID() const { return ::Udm::StringAttr(impl, meta_ComponenInstancetID); }
	::Udm::StringAttr Metric::ComponentName() const { return ::Udm::StringAttr(impl, meta_ComponentName); }
	::Udm::StringAttr Metric::ComponentType() const { return ::Udm::StringAttr(impl, meta_ComponentType); }
	::Udm::StringAttr Metric::RequestedValueType() const { return ::Udm::StringAttr(impl, meta_RequestedValueType); }
	::Udm::StringAttr Metric::MetricID() const { return ::Udm::StringAttr(impl, meta_MetricID); }
	::Udm::StringAttr Metric::Details() const { return ::Udm::StringAttr(impl, meta_Details); }
	::Udm::ParentAttr< ::CADAnalysisMetaData::Metrics> Metric::Metrics_parent() const { return ::Udm::ParentAttr< ::CADAnalysisMetaData::Metrics>(impl, meta_Metrics_parent); }
	::Udm::ParentAttr< ::CADAnalysisMetaData::Metrics> Metric::parent() const { return ::Udm::ParentAttr< ::CADAnalysisMetaData::Metrics>(impl, ::Udm::NULLPARENTROLE); }

	::Uml::Class Metric::meta;
	::Uml::Attribute Metric::meta_ConfigurationID;
	::Uml::Attribute Metric::meta_MetricType;
	::Uml::Attribute Metric::meta_TopAssemblyComponentInstanceID;
	::Uml::Attribute Metric::meta_ComponenInstancetID;
	::Uml::Attribute Metric::meta_ComponentName;
	::Uml::Attribute Metric::meta_ComponentType;
	::Uml::Attribute Metric::meta_RequestedValueType;
	::Uml::Attribute Metric::meta_MetricID;
	::Uml::Attribute Metric::meta_Details;
	::Uml::CompositionParentRole Metric::meta_Metrics_parent;

	Metrics::Metrics() {}
	Metrics::Metrics(::Udm::ObjectImpl *impl) : UDM_OBJECT(impl) {}
	Metrics::Metrics(const Metrics &master) : UDM_OBJECT(master) {}

#ifdef UDM_RVALUE
	Metrics::Metrics(Metrics &&master) : UDM_OBJECT(master) {};

	Metrics Metrics::Cast(::Udm::Object &&a) { return __Cast(std::move(a), meta); };
	Metrics& Metrics::operator=(Metrics &&a) { ::Udm::Object::operator =(std::move(a)); return *this; };

#endif
	Metrics Metrics::Cast(const ::Udm::Object &a) { return __Cast(a, meta); }
	Metrics Metrics::Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role); }
	Metrics Metrics::CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl); }
	::Udm::InstantiatedAttr< Metrics> Metrics::Instances() { return ::Udm::InstantiatedAttr< Metrics>(impl); }
	Metrics Metrics::CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl, true); }
	::Udm::DerivedAttr< Metrics> Metrics::Derived() { return ::Udm::DerivedAttr< Metrics>(impl); }
	::Udm::ArchetypeAttr< Metrics> Metrics::Archetype() const { return ::Udm::ArchetypeAttr< Metrics>(impl); }
	::Udm::ChildrenAttr< ::CADAnalysisMetaData::Metric> Metrics::Metric_children() const { return ::Udm::ChildrenAttr< ::CADAnalysisMetaData::Metric>(impl, meta_Metric_children); }
	::Udm::ChildrenAttr< ::CADAnalysisMetaData::Metric> Metrics::Metric_kind_children() const { return ::Udm::ChildrenAttr< ::CADAnalysisMetaData::Metric>(impl, ::Udm::NULLCHILDROLE); }
	::Udm::ParentAttr< ::Udm::Object> Metrics::parent() const { return ::Udm::ParentAttr< ::Udm::Object>(impl, ::Udm::NULLPARENTROLE); }

	::Uml::Class Metrics::meta;
	::Uml::CompositionChildRole Metrics::meta_Metric_children;

	LifeCycle::LifeCycle() {}
	LifeCycle::LifeCycle(::Udm::ObjectImpl *impl) : UDM_OBJECT(impl) {}
	LifeCycle::LifeCycle(const LifeCycle &master) : UDM_OBJECT(master) {}

#ifdef UDM_RVALUE
	LifeCycle::LifeCycle(LifeCycle &&master) : UDM_OBJECT(master) {};

	LifeCycle LifeCycle::Cast(::Udm::Object &&a) { return __Cast(std::move(a), meta); };
	LifeCycle& LifeCycle::operator=(LifeCycle &&a) { ::Udm::Object::operator =(std::move(a)); return *this; };

#endif
	LifeCycle LifeCycle::Cast(const ::Udm::Object &a) { return __Cast(a, meta); }
	LifeCycle LifeCycle::Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role); }
	LifeCycle LifeCycle::CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl); }
	::Udm::InstantiatedAttr< LifeCycle> LifeCycle::Instances() { return ::Udm::InstantiatedAttr< LifeCycle>(impl); }
	LifeCycle LifeCycle::CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl, true); }
	::Udm::DerivedAttr< LifeCycle> LifeCycle::Derived() { return ::Udm::DerivedAttr< LifeCycle>(impl); }
	::Udm::ArchetypeAttr< LifeCycle> LifeCycle::Archetype() const { return ::Udm::ArchetypeAttr< LifeCycle>(impl); }
	::Udm::StringAttr LifeCycle::Duration() const { return ::Udm::StringAttr(impl, meta_Duration); }
	::Udm::IntegerAttr LifeCycle::NumberOfCycles() const { return ::Udm::IntegerAttr(impl, meta_NumberOfCycles); }
	::Udm::ParentAttr< ::CADAnalysisMetaData::AnalysisSupportingData> LifeCycle::AnalysisSupportingData_parent() const { return ::Udm::ParentAttr< ::CADAnalysisMetaData::AnalysisSupportingData>(impl, meta_AnalysisSupportingData_parent); }
	::Udm::ParentAttr< ::CADAnalysisMetaData::AnalysisSupportingData> LifeCycle::parent() const { return ::Udm::ParentAttr< ::CADAnalysisMetaData::AnalysisSupportingData>(impl, ::Udm::NULLPARENTROLE); }

	::Uml::Class LifeCycle::meta;
	::Uml::Attribute LifeCycle::meta_Duration;
	::Uml::Attribute LifeCycle::meta_NumberOfCycles;
	::Uml::CompositionParentRole LifeCycle::meta_AnalysisSupportingData_parent;

	AnalysisSupportingData::AnalysisSupportingData() {}
	AnalysisSupportingData::AnalysisSupportingData(::Udm::ObjectImpl *impl) : UDM_OBJECT(impl) {}
	AnalysisSupportingData::AnalysisSupportingData(const AnalysisSupportingData &master) : UDM_OBJECT(master) {}

#ifdef UDM_RVALUE
	AnalysisSupportingData::AnalysisSupportingData(AnalysisSupportingData &&master) : UDM_OBJECT(master) {};

	AnalysisSupportingData AnalysisSupportingData::Cast(::Udm::Object &&a) { return __Cast(std::move(a), meta); };
	AnalysisSupportingData& AnalysisSupportingData::operator=(AnalysisSupportingData &&a) { ::Udm::Object::operator =(std::move(a)); return *this; };

#endif
	AnalysisSupportingData AnalysisSupportingData::Cast(const ::Udm::Object &a) { return __Cast(a, meta); }
	AnalysisSupportingData AnalysisSupportingData::Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role); }
	AnalysisSupportingData AnalysisSupportingData::CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl); }
	::Udm::InstantiatedAttr< AnalysisSupportingData> AnalysisSupportingData::Instances() { return ::Udm::InstantiatedAttr< AnalysisSupportingData>(impl); }
	AnalysisSupportingData AnalysisSupportingData::CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl, true); }
	::Udm::DerivedAttr< AnalysisSupportingData> AnalysisSupportingData::Derived() { return ::Udm::DerivedAttr< AnalysisSupportingData>(impl); }
	::Udm::ArchetypeAttr< AnalysisSupportingData> AnalysisSupportingData::Archetype() const { return ::Udm::ArchetypeAttr< AnalysisSupportingData>(impl); }
	::Udm::StringAttr AnalysisSupportingData::AnalysisType() const { return ::Udm::StringAttr(impl, meta_AnalysisType); }
	::Udm::ChildAttr< ::CADAnalysisMetaData::LifeCycle> AnalysisSupportingData::LifeCycle_child() const { return ::Udm::ChildAttr< ::CADAnalysisMetaData::LifeCycle>(impl, meta_LifeCycle_child); }
	::Udm::ChildrenAttr< ::CADAnalysisMetaData::LifeCycle> AnalysisSupportingData::LifeCycle_kind_children() const { return ::Udm::ChildrenAttr< ::CADAnalysisMetaData::LifeCycle>(impl, ::Udm::NULLCHILDROLE); }
	::Udm::ParentAttr< ::CADAnalysisMetaData::CADAnalysisMetaData> AnalysisSupportingData::CADAnalysisMetaData_parent() const { return ::Udm::ParentAttr< ::CADAnalysisMetaData::CADAnalysisMetaData>(impl, meta_CADAnalysisMetaData_parent); }
	::Udm::ParentAttr< ::CADAnalysisMetaData::CADAnalysisMetaData> AnalysisSupportingData::parent() const { return ::Udm::ParentAttr< ::CADAnalysisMetaData::CADAnalysisMetaData>(impl, ::Udm::NULLPARENTROLE); }

	::Uml::Class AnalysisSupportingData::meta;
	::Uml::Attribute AnalysisSupportingData::meta_AnalysisType;
	::Uml::CompositionChildRole AnalysisSupportingData::meta_LifeCycle_child;
	::Uml::CompositionParentRole AnalysisSupportingData::meta_CADAnalysisMetaData_parent;

	AllowableBearingStress::AllowableBearingStress() {}
	AllowableBearingStress::AllowableBearingStress(::Udm::ObjectImpl *impl) : UDM_OBJECT(impl) {}
	AllowableBearingStress::AllowableBearingStress(const AllowableBearingStress &master) : UDM_OBJECT(master) {}

#ifdef UDM_RVALUE
	AllowableBearingStress::AllowableBearingStress(AllowableBearingStress &&master) : UDM_OBJECT(master) {};

	AllowableBearingStress AllowableBearingStress::Cast(::Udm::Object &&a) { return __Cast(std::move(a), meta); };
	AllowableBearingStress& AllowableBearingStress::operator=(AllowableBearingStress &&a) { ::Udm::Object::operator =(std::move(a)); return *this; };

#endif
	AllowableBearingStress AllowableBearingStress::Cast(const ::Udm::Object &a) { return __Cast(a, meta); }
	AllowableBearingStress AllowableBearingStress::Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role); }
	AllowableBearingStress AllowableBearingStress::CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl); }
	::Udm::InstantiatedAttr< AllowableBearingStress> AllowableBearingStress::Instances() { return ::Udm::InstantiatedAttr< AllowableBearingStress>(impl); }
	AllowableBearingStress AllowableBearingStress::CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl, true); }
	::Udm::DerivedAttr< AllowableBearingStress> AllowableBearingStress::Derived() { return ::Udm::DerivedAttr< AllowableBearingStress>(impl); }
	::Udm::ArchetypeAttr< AllowableBearingStress> AllowableBearingStress::Archetype() const { return ::Udm::ArchetypeAttr< AllowableBearingStress>(impl); }
	::Udm::RealAttr AllowableBearingStress::Value() const { return ::Udm::RealAttr(impl, meta_Value); }
	::Udm::StringAttr AllowableBearingStress::Units() const { return ::Udm::StringAttr(impl, meta_Units); }
	::Udm::StringAttr AllowableBearingStress::Source() const { return ::Udm::StringAttr(impl, meta_Source); }
	::Udm::ParentAttr< ::CADAnalysisMetaData::MaterialProperties> AllowableBearingStress::MaterialProperties_parent() const { return ::Udm::ParentAttr< ::CADAnalysisMetaData::MaterialProperties>(impl, meta_MaterialProperties_parent); }
	::Udm::ParentAttr< ::CADAnalysisMetaData::MaterialProperties> AllowableBearingStress::parent() const { return ::Udm::ParentAttr< ::CADAnalysisMetaData::MaterialProperties>(impl, ::Udm::NULLPARENTROLE); }

	::Uml::Class AllowableBearingStress::meta;
	::Uml::Attribute AllowableBearingStress::meta_Value;
	::Uml::Attribute AllowableBearingStress::meta_Units;
	::Uml::Attribute AllowableBearingStress::meta_Source;
	::Uml::CompositionParentRole AllowableBearingStress::meta_MaterialProperties_parent;

	AllowableShearStress::AllowableShearStress() {}
	AllowableShearStress::AllowableShearStress(::Udm::ObjectImpl *impl) : UDM_OBJECT(impl) {}
	AllowableShearStress::AllowableShearStress(const AllowableShearStress &master) : UDM_OBJECT(master) {}

#ifdef UDM_RVALUE
	AllowableShearStress::AllowableShearStress(AllowableShearStress &&master) : UDM_OBJECT(master) {};

	AllowableShearStress AllowableShearStress::Cast(::Udm::Object &&a) { return __Cast(std::move(a), meta); };
	AllowableShearStress& AllowableShearStress::operator=(AllowableShearStress &&a) { ::Udm::Object::operator =(std::move(a)); return *this; };

#endif
	AllowableShearStress AllowableShearStress::Cast(const ::Udm::Object &a) { return __Cast(a, meta); }
	AllowableShearStress AllowableShearStress::Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role); }
	AllowableShearStress AllowableShearStress::CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl); }
	::Udm::InstantiatedAttr< AllowableShearStress> AllowableShearStress::Instances() { return ::Udm::InstantiatedAttr< AllowableShearStress>(impl); }
	AllowableShearStress AllowableShearStress::CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl, true); }
	::Udm::DerivedAttr< AllowableShearStress> AllowableShearStress::Derived() { return ::Udm::DerivedAttr< AllowableShearStress>(impl); }
	::Udm::ArchetypeAttr< AllowableShearStress> AllowableShearStress::Archetype() const { return ::Udm::ArchetypeAttr< AllowableShearStress>(impl); }
	::Udm::RealAttr AllowableShearStress::Value() const { return ::Udm::RealAttr(impl, meta_Value); }
	::Udm::StringAttr AllowableShearStress::Units() const { return ::Udm::StringAttr(impl, meta_Units); }
	::Udm::StringAttr AllowableShearStress::Source() const { return ::Udm::StringAttr(impl, meta_Source); }
	::Udm::ParentAttr< ::CADAnalysisMetaData::MaterialProperties> AllowableShearStress::MaterialProperties_parent() const { return ::Udm::ParentAttr< ::CADAnalysisMetaData::MaterialProperties>(impl, meta_MaterialProperties_parent); }
	::Udm::ParentAttr< ::CADAnalysisMetaData::MaterialProperties> AllowableShearStress::parent() const { return ::Udm::ParentAttr< ::CADAnalysisMetaData::MaterialProperties>(impl, ::Udm::NULLPARENTROLE); }

	::Uml::Class AllowableShearStress::meta;
	::Uml::Attribute AllowableShearStress::meta_Value;
	::Uml::Attribute AllowableShearStress::meta_Units;
	::Uml::Attribute AllowableShearStress::meta_Source;
	::Uml::CompositionParentRole AllowableShearStress::meta_MaterialProperties_parent;

	AllowableTensileStress::AllowableTensileStress() {}
	AllowableTensileStress::AllowableTensileStress(::Udm::ObjectImpl *impl) : UDM_OBJECT(impl) {}
	AllowableTensileStress::AllowableTensileStress(const AllowableTensileStress &master) : UDM_OBJECT(master) {}

#ifdef UDM_RVALUE
	AllowableTensileStress::AllowableTensileStress(AllowableTensileStress &&master) : UDM_OBJECT(master) {};

	AllowableTensileStress AllowableTensileStress::Cast(::Udm::Object &&a) { return __Cast(std::move(a), meta); };
	AllowableTensileStress& AllowableTensileStress::operator=(AllowableTensileStress &&a) { ::Udm::Object::operator =(std::move(a)); return *this; };

#endif
	AllowableTensileStress AllowableTensileStress::Cast(const ::Udm::Object &a) { return __Cast(a, meta); }
	AllowableTensileStress AllowableTensileStress::Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role); }
	AllowableTensileStress AllowableTensileStress::CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl); }
	::Udm::InstantiatedAttr< AllowableTensileStress> AllowableTensileStress::Instances() { return ::Udm::InstantiatedAttr< AllowableTensileStress>(impl); }
	AllowableTensileStress AllowableTensileStress::CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl, true); }
	::Udm::DerivedAttr< AllowableTensileStress> AllowableTensileStress::Derived() { return ::Udm::DerivedAttr< AllowableTensileStress>(impl); }
	::Udm::ArchetypeAttr< AllowableTensileStress> AllowableTensileStress::Archetype() const { return ::Udm::ArchetypeAttr< AllowableTensileStress>(impl); }
	::Udm::RealAttr AllowableTensileStress::Value() const { return ::Udm::RealAttr(impl, meta_Value); }
	::Udm::StringAttr AllowableTensileStress::Units() const { return ::Udm::StringAttr(impl, meta_Units); }
	::Udm::StringAttr AllowableTensileStress::Source() const { return ::Udm::StringAttr(impl, meta_Source); }
	::Udm::ParentAttr< ::CADAnalysisMetaData::MaterialProperties> AllowableTensileStress::MaterialProperties_parent() const { return ::Udm::ParentAttr< ::CADAnalysisMetaData::MaterialProperties>(impl, meta_MaterialProperties_parent); }
	::Udm::ParentAttr< ::CADAnalysisMetaData::MaterialProperties> AllowableTensileStress::parent() const { return ::Udm::ParentAttr< ::CADAnalysisMetaData::MaterialProperties>(impl, ::Udm::NULLPARENTROLE); }

	::Uml::Class AllowableTensileStress::meta;
	::Uml::Attribute AllowableTensileStress::meta_Value;
	::Uml::Attribute AllowableTensileStress::meta_Units;
	::Uml::Attribute AllowableTensileStress::meta_Source;
	::Uml::CompositionParentRole AllowableTensileStress::meta_MaterialProperties_parent;

	Materials::Materials() {}
	Materials::Materials(::Udm::ObjectImpl *impl) : UDM_OBJECT(impl) {}
	Materials::Materials(const Materials &master) : UDM_OBJECT(master) {}

#ifdef UDM_RVALUE
	Materials::Materials(Materials &&master) : UDM_OBJECT(master) {};

	Materials Materials::Cast(::Udm::Object &&a) { return __Cast(std::move(a), meta); };
	Materials& Materials::operator=(Materials &&a) { ::Udm::Object::operator =(std::move(a)); return *this; };

#endif
	Materials Materials::Cast(const ::Udm::Object &a) { return __Cast(a, meta); }
	Materials Materials::Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role); }
	Materials Materials::CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl); }
	::Udm::InstantiatedAttr< Materials> Materials::Instances() { return ::Udm::InstantiatedAttr< Materials>(impl); }
	Materials Materials::CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl, true); }
	::Udm::DerivedAttr< Materials> Materials::Derived() { return ::Udm::DerivedAttr< Materials>(impl); }
	::Udm::ArchetypeAttr< Materials> Materials::Archetype() const { return ::Udm::ArchetypeAttr< Materials>(impl); }
	::Udm::ChildrenAttr< ::CADAnalysisMetaData::Material> Materials::Material_children() const { return ::Udm::ChildrenAttr< ::CADAnalysisMetaData::Material>(impl, meta_Material_children); }
	::Udm::ChildrenAttr< ::CADAnalysisMetaData::Material> Materials::Material_kind_children() const { return ::Udm::ChildrenAttr< ::CADAnalysisMetaData::Material>(impl, ::Udm::NULLCHILDROLE); }
	::Udm::ParentAttr< ::CADAnalysisMetaData::CADAnalysisMetaData> Materials::CADAnalysisMetaData_parent() const { return ::Udm::ParentAttr< ::CADAnalysisMetaData::CADAnalysisMetaData>(impl, meta_CADAnalysisMetaData_parent); }
	::Udm::ParentAttr< ::CADAnalysisMetaData::CADAnalysisMetaData> Materials::parent() const { return ::Udm::ParentAttr< ::CADAnalysisMetaData::CADAnalysisMetaData>(impl, ::Udm::NULLPARENTROLE); }

	::Uml::Class Materials::meta;
	::Uml::CompositionChildRole Materials::meta_Material_children;
	::Uml::CompositionParentRole Materials::meta_CADAnalysisMetaData_parent;

	Material::Material() {}
	Material::Material(::Udm::ObjectImpl *impl) : UDM_OBJECT(impl) {}
	Material::Material(const Material &master) : UDM_OBJECT(master) {}

#ifdef UDM_RVALUE
	Material::Material(Material &&master) : UDM_OBJECT(master) {};

	Material Material::Cast(::Udm::Object &&a) { return __Cast(std::move(a), meta); };
	Material& Material::operator=(Material &&a) { ::Udm::Object::operator =(std::move(a)); return *this; };

#endif
	Material Material::Cast(const ::Udm::Object &a) { return __Cast(a, meta); }
	Material Material::Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role); }
	Material Material::CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl); }
	::Udm::InstantiatedAttr< Material> Material::Instances() { return ::Udm::InstantiatedAttr< Material>(impl); }
	Material Material::CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl, true); }
	::Udm::DerivedAttr< Material> Material::Derived() { return ::Udm::DerivedAttr< Material>(impl); }
	::Udm::ArchetypeAttr< Material> Material::Archetype() const { return ::Udm::ArchetypeAttr< Material>(impl); }
	::Udm::StringAttr Material::MaterialID() const { return ::Udm::StringAttr(impl, meta_MaterialID); }
	::Udm::ChildrenAttr< ::CADAnalysisMetaData::MaterialProperties> Material::MaterialProperties_children() const { return ::Udm::ChildrenAttr< ::CADAnalysisMetaData::MaterialProperties>(impl, meta_MaterialProperties_children); }
	::Udm::ChildrenAttr< ::CADAnalysisMetaData::MaterialProperties> Material::MaterialProperties_kind_children() const { return ::Udm::ChildrenAttr< ::CADAnalysisMetaData::MaterialProperties>(impl, ::Udm::NULLCHILDROLE); }
	::Udm::ParentAttr< ::CADAnalysisMetaData::Materials> Material::Materials_parent() const { return ::Udm::ParentAttr< ::CADAnalysisMetaData::Materials>(impl, meta_Materials_parent); }
	::Udm::ParentAttr< ::CADAnalysisMetaData::Materials> Material::parent() const { return ::Udm::ParentAttr< ::CADAnalysisMetaData::Materials>(impl, ::Udm::NULLPARENTROLE); }

	::Uml::Class Material::meta;
	::Uml::Attribute Material::meta_MaterialID;
	::Uml::CompositionChildRole Material::meta_MaterialProperties_children;
	::Uml::CompositionParentRole Material::meta_Materials_parent;

	MaterialProperties::MaterialProperties() {}
	MaterialProperties::MaterialProperties(::Udm::ObjectImpl *impl) : UDM_OBJECT(impl) {}
	MaterialProperties::MaterialProperties(const MaterialProperties &master) : UDM_OBJECT(master) {}

#ifdef UDM_RVALUE
	MaterialProperties::MaterialProperties(MaterialProperties &&master) : UDM_OBJECT(master) {};

	MaterialProperties MaterialProperties::Cast(::Udm::Object &&a) { return __Cast(std::move(a), meta); };
	MaterialProperties& MaterialProperties::operator=(MaterialProperties &&a) { ::Udm::Object::operator =(std::move(a)); return *this; };

#endif
	MaterialProperties MaterialProperties::Cast(const ::Udm::Object &a) { return __Cast(a, meta); }
	MaterialProperties MaterialProperties::Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role); }
	MaterialProperties MaterialProperties::CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl); }
	::Udm::InstantiatedAttr< MaterialProperties> MaterialProperties::Instances() { return ::Udm::InstantiatedAttr< MaterialProperties>(impl); }
	MaterialProperties MaterialProperties::CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl, true); }
	::Udm::DerivedAttr< MaterialProperties> MaterialProperties::Derived() { return ::Udm::DerivedAttr< MaterialProperties>(impl); }
	::Udm::ArchetypeAttr< MaterialProperties> MaterialProperties::Archetype() const { return ::Udm::ArchetypeAttr< MaterialProperties>(impl); }
	::Udm::ChildAttr< ::CADAnalysisMetaData::AllowableBearingStress> MaterialProperties::AllowableBearingStress_child() const { return ::Udm::ChildAttr< ::CADAnalysisMetaData::AllowableBearingStress>(impl, meta_AllowableBearingStress_child); }
	::Udm::ChildAttr< ::CADAnalysisMetaData::AllowableShearStress> MaterialProperties::AllowableShearStress_child() const { return ::Udm::ChildAttr< ::CADAnalysisMetaData::AllowableShearStress>(impl, meta_AllowableShearStress_child); }
	::Udm::ChildAttr< ::CADAnalysisMetaData::AllowableTensileStress> MaterialProperties::AllowableTensileStress_child() const { return ::Udm::ChildAttr< ::CADAnalysisMetaData::AllowableTensileStress>(impl, meta_AllowableTensileStress_child); }
	::Udm::ChildrenAttr< ::CADAnalysisMetaData::AllowableBearingStress> MaterialProperties::AllowableBearingStress_kind_children() const { return ::Udm::ChildrenAttr< ::CADAnalysisMetaData::AllowableBearingStress>(impl, ::Udm::NULLCHILDROLE); }
	::Udm::ChildrenAttr< ::CADAnalysisMetaData::AllowableShearStress> MaterialProperties::AllowableShearStress_kind_children() const { return ::Udm::ChildrenAttr< ::CADAnalysisMetaData::AllowableShearStress>(impl, ::Udm::NULLCHILDROLE); }
	::Udm::ChildrenAttr< ::CADAnalysisMetaData::AllowableTensileStress> MaterialProperties::AllowableTensileStress_kind_children() const { return ::Udm::ChildrenAttr< ::CADAnalysisMetaData::AllowableTensileStress>(impl, ::Udm::NULLCHILDROLE); }
	::Udm::ParentAttr< ::CADAnalysisMetaData::Material> MaterialProperties::Material_parent() const { return ::Udm::ParentAttr< ::CADAnalysisMetaData::Material>(impl, meta_Material_parent); }
	::Udm::ParentAttr< ::CADAnalysisMetaData::Material> MaterialProperties::parent() const { return ::Udm::ParentAttr< ::CADAnalysisMetaData::Material>(impl, ::Udm::NULLPARENTROLE); }

	::Uml::Class MaterialProperties::meta;
	::Uml::CompositionChildRole MaterialProperties::meta_AllowableBearingStress_child;
	::Uml::CompositionChildRole MaterialProperties::meta_AllowableShearStress_child;
	::Uml::CompositionChildRole MaterialProperties::meta_AllowableTensileStress_child;
	::Uml::CompositionParentRole MaterialProperties::meta_Material_parent;

	Assemblies::Assemblies() {}
	Assemblies::Assemblies(::Udm::ObjectImpl *impl) : UDM_OBJECT(impl) {}
	Assemblies::Assemblies(const Assemblies &master) : UDM_OBJECT(master) {}

#ifdef UDM_RVALUE
	Assemblies::Assemblies(Assemblies &&master) : UDM_OBJECT(master) {};

	Assemblies Assemblies::Cast(::Udm::Object &&a) { return __Cast(std::move(a), meta); };
	Assemblies& Assemblies::operator=(Assemblies &&a) { ::Udm::Object::operator =(std::move(a)); return *this; };

#endif
	Assemblies Assemblies::Cast(const ::Udm::Object &a) { return __Cast(a, meta); }
	Assemblies Assemblies::Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role); }
	Assemblies Assemblies::CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl); }
	::Udm::InstantiatedAttr< Assemblies> Assemblies::Instances() { return ::Udm::InstantiatedAttr< Assemblies>(impl); }
	Assemblies Assemblies::CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl, true); }
	::Udm::DerivedAttr< Assemblies> Assemblies::Derived() { return ::Udm::DerivedAttr< Assemblies>(impl); }
	::Udm::ArchetypeAttr< Assemblies> Assemblies::Archetype() const { return ::Udm::ArchetypeAttr< Assemblies>(impl); }
	::Udm::ChildrenAttr< ::CADAnalysisMetaData::Assembly> Assemblies::Assembly_children() const { return ::Udm::ChildrenAttr< ::CADAnalysisMetaData::Assembly>(impl, meta_Assembly_children); }
	::Udm::ChildrenAttr< ::CADAnalysisMetaData::Assembly> Assemblies::Assembly_kind_children() const { return ::Udm::ChildrenAttr< ::CADAnalysisMetaData::Assembly>(impl, ::Udm::NULLCHILDROLE); }
	::Udm::ParentAttr< ::CADAnalysisMetaData::CADAnalysisMetaData> Assemblies::CADAnalysisMetaData_parent() const { return ::Udm::ParentAttr< ::CADAnalysisMetaData::CADAnalysisMetaData>(impl, meta_CADAnalysisMetaData_parent); }
	::Udm::ParentAttr< ::CADAnalysisMetaData::CADAnalysisMetaData> Assemblies::parent() const { return ::Udm::ParentAttr< ::CADAnalysisMetaData::CADAnalysisMetaData>(impl, ::Udm::NULLPARENTROLE); }

	::Uml::Class Assemblies::meta;
	::Uml::CompositionChildRole Assemblies::meta_Assembly_children;
	::Uml::CompositionParentRole Assemblies::meta_CADAnalysisMetaData_parent;

	Component::Component() {}
	Component::Component(::Udm::ObjectImpl *impl) : UDM_OBJECT(impl) {}
	Component::Component(const Component &master) : UDM_OBJECT(master) {}

#ifdef UDM_RVALUE
	Component::Component(Component &&master) : UDM_OBJECT(master) {};

	Component Component::Cast(::Udm::Object &&a) { return __Cast(std::move(a), meta); };
	Component& Component::operator=(Component &&a) { ::Udm::Object::operator =(std::move(a)); return *this; };

#endif
	Component Component::Cast(const ::Udm::Object &a) { return __Cast(a, meta); }
	Component Component::Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role); }
	Component Component::CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl); }
	::Udm::InstantiatedAttr< Component> Component::Instances() { return ::Udm::InstantiatedAttr< Component>(impl); }
	Component Component::CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl, true); }
	::Udm::DerivedAttr< Component> Component::Derived() { return ::Udm::DerivedAttr< Component>(impl); }
	::Udm::ArchetypeAttr< Component> Component::Archetype() const { return ::Udm::ArchetypeAttr< Component>(impl); }
	::Udm::StringAttr Component::Name() const { return ::Udm::StringAttr(impl, meta_Name); }
	::Udm::StringAttr Component::Type() const { return ::Udm::StringAttr(impl, meta_Type); }
	::Udm::StringAttr Component::MaterialID() const { return ::Udm::StringAttr(impl, meta_MaterialID); }
	::Udm::StringAttr Component::ComponentInstanceID() const { return ::Udm::StringAttr(impl, meta_ComponentInstanceID); }
	::Udm::StringAttr Component::FEAElementType() const { return ::Udm::StringAttr(impl, meta_FEAElementType); }
	::Udm::StringAttr Component::FEAElementID() const { return ::Udm::StringAttr(impl, meta_FEAElementID); }
	::Udm::ChildrenAttr< ::CADAnalysisMetaData::Component> Component::Component_children() const { return ::Udm::ChildrenAttr< ::CADAnalysisMetaData::Component>(impl, meta_Component_children); }
	::Udm::ChildrenAttr< ::CADAnalysisMetaData::Component> Component::Component_kind_children() const { return ::Udm::ChildrenAttr< ::CADAnalysisMetaData::Component>(impl, ::Udm::NULLCHILDROLE); }
	::Udm::ParentAttr< ::CADAnalysisMetaData::Component> Component::Component_parent() const { return ::Udm::ParentAttr< ::CADAnalysisMetaData::Component>(impl, meta_Component_parent); }
	::Udm::ParentAttr< ::CADAnalysisMetaData::Assembly> Component::Assembly_parent() const { return ::Udm::ParentAttr< ::CADAnalysisMetaData::Assembly>(impl, meta_Assembly_parent); }
	::Udm::ParentAttr< ::Udm::Object> Component::parent() const { return ::Udm::ParentAttr< ::Udm::Object>(impl, ::Udm::NULLPARENTROLE); }

	::Uml::Class Component::meta;
	::Uml::Attribute Component::meta_Name;
	::Uml::Attribute Component::meta_Type;
	::Uml::Attribute Component::meta_MaterialID;
	::Uml::Attribute Component::meta_ComponentInstanceID;
	::Uml::Attribute Component::meta_FEAElementType;
	::Uml::Attribute Component::meta_FEAElementID;
	::Uml::CompositionChildRole Component::meta_Component_children;
	::Uml::CompositionParentRole Component::meta_Component_parent;
	::Uml::CompositionParentRole Component::meta_Assembly_parent;

	CADAnalysisMetaData::CADAnalysisMetaData() {}
	CADAnalysisMetaData::CADAnalysisMetaData(::Udm::ObjectImpl *impl) : UDM_OBJECT(impl) {}
	CADAnalysisMetaData::CADAnalysisMetaData(const CADAnalysisMetaData &master) : UDM_OBJECT(master) {}

#ifdef UDM_RVALUE
	CADAnalysisMetaData::CADAnalysisMetaData(CADAnalysisMetaData &&master) : UDM_OBJECT(master) {};

	CADAnalysisMetaData CADAnalysisMetaData::Cast(::Udm::Object &&a) { return __Cast(std::move(a), meta); };
	CADAnalysisMetaData& CADAnalysisMetaData::operator=(CADAnalysisMetaData &&a) { ::Udm::Object::operator =(std::move(a)); return *this; };

#endif
	CADAnalysisMetaData CADAnalysisMetaData::Cast(const ::Udm::Object &a) { return __Cast(a, meta); }
	CADAnalysisMetaData CADAnalysisMetaData::Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role); }
	CADAnalysisMetaData CADAnalysisMetaData::CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl); }
	::Udm::InstantiatedAttr< CADAnalysisMetaData> CADAnalysisMetaData::Instances() { return ::Udm::InstantiatedAttr< CADAnalysisMetaData>(impl); }
	CADAnalysisMetaData CADAnalysisMetaData::CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl, true); }
	::Udm::DerivedAttr< CADAnalysisMetaData> CADAnalysisMetaData::Derived() { return ::Udm::DerivedAttr< CADAnalysisMetaData>(impl); }
	::Udm::ArchetypeAttr< CADAnalysisMetaData> CADAnalysisMetaData::Archetype() const { return ::Udm::ArchetypeAttr< CADAnalysisMetaData>(impl); }
	::Udm::ChildAttr< ::CADAnalysisMetaData::AnalysisSupportingData> CADAnalysisMetaData::AnalysisSupportingData_child() const { return ::Udm::ChildAttr< ::CADAnalysisMetaData::AnalysisSupportingData>(impl, meta_AnalysisSupportingData_child); }
	::Udm::ChildAttr< ::CADAnalysisMetaData::Materials> CADAnalysisMetaData::Materials_child() const { return ::Udm::ChildAttr< ::CADAnalysisMetaData::Materials>(impl, meta_Materials_child); }
	::Udm::ChildAttr< ::CADAnalysisMetaData::Assemblies> CADAnalysisMetaData::Assemblies_child() const { return ::Udm::ChildAttr< ::CADAnalysisMetaData::Assemblies>(impl, meta_Assemblies_child); }
	::Udm::ChildrenAttr< ::CADAnalysisMetaData::AnalysisSupportingData> CADAnalysisMetaData::AnalysisSupportingData_kind_children() const { return ::Udm::ChildrenAttr< ::CADAnalysisMetaData::AnalysisSupportingData>(impl, ::Udm::NULLCHILDROLE); }
	::Udm::ChildrenAttr< ::CADAnalysisMetaData::Materials> CADAnalysisMetaData::Materials_kind_children() const { return ::Udm::ChildrenAttr< ::CADAnalysisMetaData::Materials>(impl, ::Udm::NULLCHILDROLE); }
	::Udm::ChildrenAttr< ::CADAnalysisMetaData::Assemblies> CADAnalysisMetaData::Assemblies_kind_children() const { return ::Udm::ChildrenAttr< ::CADAnalysisMetaData::Assemblies>(impl, ::Udm::NULLCHILDROLE); }
	::Udm::ParentAttr< ::Udm::Object> CADAnalysisMetaData::parent() const { return ::Udm::ParentAttr< ::Udm::Object>(impl, ::Udm::NULLPARENTROLE); }

	::Uml::Class CADAnalysisMetaData::meta;
	::Uml::CompositionChildRole CADAnalysisMetaData::meta_AnalysisSupportingData_child;
	::Uml::CompositionChildRole CADAnalysisMetaData::meta_Materials_child;
	::Uml::CompositionChildRole CADAnalysisMetaData::meta_Assemblies_child;

	Assembly::Assembly() {}
	Assembly::Assembly(::Udm::ObjectImpl *impl) : UDM_OBJECT(impl) {}
	Assembly::Assembly(const Assembly &master) : UDM_OBJECT(master) {}

#ifdef UDM_RVALUE
	Assembly::Assembly(Assembly &&master) : UDM_OBJECT(master) {};

	Assembly Assembly::Cast(::Udm::Object &&a) { return __Cast(std::move(a), meta); };
	Assembly& Assembly::operator=(Assembly &&a) { ::Udm::Object::operator =(std::move(a)); return *this; };

#endif
	Assembly Assembly::Cast(const ::Udm::Object &a) { return __Cast(a, meta); }
	Assembly Assembly::Create(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role); }
	Assembly Assembly::CreateInstance(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl); }
	::Udm::InstantiatedAttr< Assembly> Assembly::Instances() { return ::Udm::InstantiatedAttr< Assembly>(impl); }
	Assembly Assembly::CreateDerived(const ::Udm::Object &parent, const ::Uml::CompositionChildRole &role) { return __Create(meta, parent, role, impl, true); }
	::Udm::DerivedAttr< Assembly> Assembly::Derived() { return ::Udm::DerivedAttr< Assembly>(impl); }
	::Udm::ArchetypeAttr< Assembly> Assembly::Archetype() const { return ::Udm::ArchetypeAttr< Assembly>(impl); }
	::Udm::StringAttr Assembly::ConfigurationID() const { return ::Udm::StringAttr(impl, meta_ConfigurationID); }
	::Udm::ChildrenAttr< ::CADAnalysisMetaData::Component> Assembly::Component_children() const { return ::Udm::ChildrenAttr< ::CADAnalysisMetaData::Component>(impl, meta_Component_children); }
	::Udm::ChildrenAttr< ::CADAnalysisMetaData::Component> Assembly::Component_kind_children() const { return ::Udm::ChildrenAttr< ::CADAnalysisMetaData::Component>(impl, ::Udm::NULLCHILDROLE); }
	::Udm::ParentAttr< ::CADAnalysisMetaData::Assemblies> Assembly::Assemblies_parent() const { return ::Udm::ParentAttr< ::CADAnalysisMetaData::Assemblies>(impl, meta_Assemblies_parent); }
	::Udm::ParentAttr< ::CADAnalysisMetaData::Assemblies> Assembly::parent() const { return ::Udm::ParentAttr< ::CADAnalysisMetaData::Assemblies>(impl, ::Udm::NULLPARENTROLE); }

	::Uml::Class Assembly::meta;
	::Uml::Attribute Assembly::meta_ConfigurationID;
	::Uml::CompositionChildRole Assembly::meta_Component_children;
	::Uml::CompositionParentRole Assembly::meta_Assemblies_parent;

	::Uml::Diagram meta;

	void CreateMeta() {
		// classes, with attributes, constraints and constraint definitions
		AllowableBearingStress::meta = ::Uml::Class::Create(meta);
		AllowableBearingStress::meta_Value = ::Uml::Attribute::Create(AllowableBearingStress::meta);
		AllowableBearingStress::meta_Units = ::Uml::Attribute::Create(AllowableBearingStress::meta);
		AllowableBearingStress::meta_Source = ::Uml::Attribute::Create(AllowableBearingStress::meta);

		AllowableShearStress::meta = ::Uml::Class::Create(meta);
		AllowableShearStress::meta_Value = ::Uml::Attribute::Create(AllowableShearStress::meta);
		AllowableShearStress::meta_Units = ::Uml::Attribute::Create(AllowableShearStress::meta);
		AllowableShearStress::meta_Source = ::Uml::Attribute::Create(AllowableShearStress::meta);

		AllowableTensileStress::meta = ::Uml::Class::Create(meta);
		AllowableTensileStress::meta_Value = ::Uml::Attribute::Create(AllowableTensileStress::meta);
		AllowableTensileStress::meta_Units = ::Uml::Attribute::Create(AllowableTensileStress::meta);
		AllowableTensileStress::meta_Source = ::Uml::Attribute::Create(AllowableTensileStress::meta);

		AnalysisSupportingData::meta = ::Uml::Class::Create(meta);
		AnalysisSupportingData::meta_AnalysisType = ::Uml::Attribute::Create(AnalysisSupportingData::meta);

		Assemblies::meta = ::Uml::Class::Create(meta);

		Assembly::meta = ::Uml::Class::Create(meta);
		Assembly::meta_ConfigurationID = ::Uml::Attribute::Create(Assembly::meta);

		CADAnalysisMetaData::meta = ::Uml::Class::Create(meta);

		Component::meta = ::Uml::Class::Create(meta);
		Component::meta_Name = ::Uml::Attribute::Create(Component::meta);
		Component::meta_Type = ::Uml::Attribute::Create(Component::meta);
		Component::meta_MaterialID = ::Uml::Attribute::Create(Component::meta);
		Component::meta_ComponentInstanceID = ::Uml::Attribute::Create(Component::meta);
		Component::meta_FEAElementType = ::Uml::Attribute::Create(Component::meta);
		Component::meta_FEAElementID = ::Uml::Attribute::Create(Component::meta);

		LifeCycle::meta = ::Uml::Class::Create(meta);
		LifeCycle::meta_Duration = ::Uml::Attribute::Create(LifeCycle::meta);
		LifeCycle::meta_NumberOfCycles = ::Uml::Attribute::Create(LifeCycle::meta);

		Material::meta = ::Uml::Class::Create(meta);
		Material::meta_MaterialID = ::Uml::Attribute::Create(Material::meta);

		MaterialProperties::meta = ::Uml::Class::Create(meta);

		Materials::meta = ::Uml::Class::Create(meta);

		Metric::meta = ::Uml::Class::Create(meta);
		Metric::meta_ConfigurationID = ::Uml::Attribute::Create(Metric::meta);
		Metric::meta_MetricType = ::Uml::Attribute::Create(Metric::meta);
		Metric::meta_TopAssemblyComponentInstanceID = ::Uml::Attribute::Create(Metric::meta);
		Metric::meta_ComponenInstancetID = ::Uml::Attribute::Create(Metric::meta);
		Metric::meta_ComponentName = ::Uml::Attribute::Create(Metric::meta);
		Metric::meta_ComponentType = ::Uml::Attribute::Create(Metric::meta);
		Metric::meta_RequestedValueType = ::Uml::Attribute::Create(Metric::meta);
		Metric::meta_MetricID = ::Uml::Attribute::Create(Metric::meta);
		Metric::meta_Details = ::Uml::Attribute::Create(Metric::meta);

		Metrics::meta = ::Uml::Class::Create(meta);

	}

	void InitMeta() {
		// classes, with attributes, constraints and constraint definitions
		::Uml::InitClassProps(AllowableBearingStress::meta, "AllowableBearingStress", false, NULL, NULL);
		::Uml::InitAttributeProps(AllowableBearingStress::meta_Value, "Value", "Real", false, false, 1, 1, false, "public", vector<string>());
		::Uml::InitAttributeProps(AllowableBearingStress::meta_Units, "Units", "String", false, false, 1, 1, false, "public", vector<string>());
		::Uml::InitAttributeProps(AllowableBearingStress::meta_Source, "Source", "String", false, false, 1, 1, false, "public", vector<string>());

		::Uml::InitClassProps(AllowableShearStress::meta, "AllowableShearStress", false, NULL, NULL);
		::Uml::InitAttributeProps(AllowableShearStress::meta_Value, "Value", "Real", false, false, 1, 1, false, "public", vector<string>());
		::Uml::InitAttributeProps(AllowableShearStress::meta_Units, "Units", "String", false, false, 1, 1, false, "public", vector<string>());
		::Uml::InitAttributeProps(AllowableShearStress::meta_Source, "Source", "String", false, false, 1, 1, false, "public", vector<string>());

		::Uml::InitClassProps(AllowableTensileStress::meta, "AllowableTensileStress", false, NULL, NULL);
		::Uml::InitAttributeProps(AllowableTensileStress::meta_Value, "Value", "Real", false, false, 1, 1, false, "public", vector<string>());
		::Uml::InitAttributeProps(AllowableTensileStress::meta_Units, "Units", "String", false, false, 1, 1, false, "public", vector<string>());
		::Uml::InitAttributeProps(AllowableTensileStress::meta_Source, "Source", "String", false, false, 1, 1, false, "public", vector<string>());

		::Uml::InitClassProps(AnalysisSupportingData::meta, "AnalysisSupportingData", false, NULL, NULL);
		::Uml::InitAttributeProps(AnalysisSupportingData::meta_AnalysisType, "AnalysisType", "String", false, false, 1, 1, false, "public", vector<string>());

		::Uml::InitClassProps(Assemblies::meta, "Assemblies", false, NULL, NULL);

		::Uml::InitClassProps(Assembly::meta, "Assembly", false, NULL, NULL);
		::Uml::InitAttributeProps(Assembly::meta_ConfigurationID, "ConfigurationID", "String", false, false, 1, 1, false, "public", vector<string>());

		::Uml::InitClassProps(CADAnalysisMetaData::meta, "CADAnalysisMetaData", false, NULL, NULL);

		::Uml::InitClassProps(Component::meta, "Component", false, NULL, NULL);
		::Uml::InitAttributeProps(Component::meta_Name, "Name", "String", false, false, 1, 1, false, "public", vector<string>());
		::Uml::InitAttributeProps(Component::meta_Type, "Type", "String", false, false, 1, 1, false, "public", vector<string>());
		::Uml::InitAttributeProps(Component::meta_MaterialID, "MaterialID", "String", false, false, 1, 1, false, "public", vector<string>());
		::Uml::InitAttributeProps(Component::meta_ComponentInstanceID, "ComponentInstanceID", "String", false, false, 1, 1, false, "public", vector<string>());
		::Uml::InitAttributeProps(Component::meta_FEAElementType, "FEAElementType", "String", false, false, 1, 1, false, "public", vector<string>());
		::Uml::InitAttributeProps(Component::meta_FEAElementID, "FEAElementID", "String", false, false, 1, 1, false, "public", vector<string>());

		::Uml::InitClassProps(LifeCycle::meta, "LifeCycle", false, NULL, NULL);
		::Uml::InitAttributeProps(LifeCycle::meta_Duration, "Duration", "String", false, false, 1, 1, false, "public", vector<string>());
		::Uml::InitAttributeProps(LifeCycle::meta_NumberOfCycles, "NumberOfCycles", "Integer", false, false, 1, 1, false, "public", vector<string>());

		::Uml::InitClassProps(Material::meta, "Material", false, NULL, NULL);
		::Uml::InitAttributeProps(Material::meta_MaterialID, "MaterialID", "String", false, false, 1, 1, false, "public", vector<string>());

		::Uml::InitClassProps(MaterialProperties::meta, "MaterialProperties", false, NULL, NULL);

		::Uml::InitClassProps(Materials::meta, "Materials", false, NULL, NULL);

		::Uml::InitClassProps(Metric::meta, "Metric", false, NULL, NULL);
		::Uml::InitAttributeProps(Metric::meta_ConfigurationID, "ConfigurationID", "String", false, false, 1, 1, false, "public", vector<string>());
		::Uml::InitAttributeProps(Metric::meta_MetricType, "MetricType", "String", false, false, 1, 1, false, "public", vector<string>());
		::Uml::InitAttributeProps(Metric::meta_TopAssemblyComponentInstanceID, "TopAssemblyComponentInstanceID", "String", false, false, 1, 1, false, "public", vector<string>());
		::Uml::InitAttributeProps(Metric::meta_ComponenInstancetID, "ComponenInstancetID", "String", false, false, 1, 1, false, "public", vector<string>());
		::Uml::InitAttributeProps(Metric::meta_ComponentName, "ComponentName", "String", false, false, 1, 1, false, "public", vector<string>());
		::Uml::InitAttributeProps(Metric::meta_ComponentType, "ComponentType", "String", false, false, 1, 1, false, "public", vector<string>());
		::Uml::InitAttributeProps(Metric::meta_RequestedValueType, "RequestedValueType", "String", false, false, 1, 1, false, "public", vector<string>());
		::Uml::InitAttributeProps(Metric::meta_MetricID, "MetricID", "String", false, false, 1, 1, false, "public", vector<string>());
		::Uml::InitAttributeProps(Metric::meta_Details, "Details", "String", false, false, 1, 1, false, "public", vector<string>());

		::Uml::InitClassProps(Metrics::meta, "Metrics", false, NULL, NULL);

		// compositions
		{
			::Uml::Composition comp = ::Uml::Composition::Create(meta);
			::Uml::InitCompositionProps(comp, "Composition", false);
			Metric::meta_Metrics_parent = ::Uml::CompositionParentRole::Create(comp);
			::Uml::InitCompositionParentRoleProps(Metric::meta_Metrics_parent, "Metrics_parent", true);
			Metrics::meta_Metric_children = ::Uml::CompositionChildRole::Create(comp);
			::Uml::InitCompositionChildRoleProps(Metrics::meta_Metric_children, "Metric", true, 0, -1);

		}
		{
			::Uml::Composition comp = ::Uml::Composition::Create(meta);
			::Uml::InitCompositionProps(comp, "Composition", false);
			LifeCycle::meta_AnalysisSupportingData_parent = ::Uml::CompositionParentRole::Create(comp);
			::Uml::InitCompositionParentRoleProps(LifeCycle::meta_AnalysisSupportingData_parent, "AnalysisSupportingData_parent", true);
			AnalysisSupportingData::meta_LifeCycle_child = ::Uml::CompositionChildRole::Create(comp);
			::Uml::InitCompositionChildRoleProps(AnalysisSupportingData::meta_LifeCycle_child, "LifeCycle", true, 0, 1);

		}
		{
			::Uml::Composition comp = ::Uml::Composition::Create(meta);
			::Uml::InitCompositionProps(comp, "Composition", false);
			Material::meta_Materials_parent = ::Uml::CompositionParentRole::Create(comp);
			::Uml::InitCompositionParentRoleProps(Material::meta_Materials_parent, "Materials_parent", true);
			Materials::meta_Material_children = ::Uml::CompositionChildRole::Create(comp);
			::Uml::InitCompositionChildRoleProps(Materials::meta_Material_children, "Material", true, 0, -1);

		}
		{
			::Uml::Composition comp = ::Uml::Composition::Create(meta);
			::Uml::InitCompositionProps(comp, "Composition", false);
			MaterialProperties::meta_Material_parent = ::Uml::CompositionParentRole::Create(comp);
			::Uml::InitCompositionParentRoleProps(MaterialProperties::meta_Material_parent, "Material_parent", true);
			Material::meta_MaterialProperties_children = ::Uml::CompositionChildRole::Create(comp);
			::Uml::InitCompositionChildRoleProps(Material::meta_MaterialProperties_children, "MaterialProperties", true, 0, -1);

		}
		{
			::Uml::Composition comp = ::Uml::Composition::Create(meta);
			::Uml::InitCompositionProps(comp, "Composition", false);
			AllowableBearingStress::meta_MaterialProperties_parent = ::Uml::CompositionParentRole::Create(comp);
			::Uml::InitCompositionParentRoleProps(AllowableBearingStress::meta_MaterialProperties_parent, "MaterialProperties_parent", true);
			MaterialProperties::meta_AllowableBearingStress_child = ::Uml::CompositionChildRole::Create(comp);
			::Uml::InitCompositionChildRoleProps(MaterialProperties::meta_AllowableBearingStress_child, "AllowableBearingStress", true, 0, 1);

		}
		{
			::Uml::Composition comp = ::Uml::Composition::Create(meta);
			::Uml::InitCompositionProps(comp, "Composition", false);
			AllowableShearStress::meta_MaterialProperties_parent = ::Uml::CompositionParentRole::Create(comp);
			::Uml::InitCompositionParentRoleProps(AllowableShearStress::meta_MaterialProperties_parent, "MaterialProperties_parent", true);
			MaterialProperties::meta_AllowableShearStress_child = ::Uml::CompositionChildRole::Create(comp);
			::Uml::InitCompositionChildRoleProps(MaterialProperties::meta_AllowableShearStress_child, "AllowableShearStress", true, 0, 1);

		}
		{
			::Uml::Composition comp = ::Uml::Composition::Create(meta);
			::Uml::InitCompositionProps(comp, "Composition", false);
			AllowableTensileStress::meta_MaterialProperties_parent = ::Uml::CompositionParentRole::Create(comp);
			::Uml::InitCompositionParentRoleProps(AllowableTensileStress::meta_MaterialProperties_parent, "MaterialProperties_parent", true);
			MaterialProperties::meta_AllowableTensileStress_child = ::Uml::CompositionChildRole::Create(comp);
			::Uml::InitCompositionChildRoleProps(MaterialProperties::meta_AllowableTensileStress_child, "AllowableTensileStress", true, 0, 1);

		}
		{
			::Uml::Composition comp = ::Uml::Composition::Create(meta);
			::Uml::InitCompositionProps(comp, "Composition", false);
			Assembly::meta_Assemblies_parent = ::Uml::CompositionParentRole::Create(comp);
			::Uml::InitCompositionParentRoleProps(Assembly::meta_Assemblies_parent, "Assemblies_parent", true);
			Assemblies::meta_Assembly_children = ::Uml::CompositionChildRole::Create(comp);
			::Uml::InitCompositionChildRoleProps(Assemblies::meta_Assembly_children, "Assembly", true, 0, -1);

		}
		{
			::Uml::Composition comp = ::Uml::Composition::Create(meta);
			::Uml::InitCompositionProps(comp, "Composition", false);
			Component::meta_Component_parent = ::Uml::CompositionParentRole::Create(comp);
			::Uml::InitCompositionParentRoleProps(Component::meta_Component_parent, "Component_parent", true);
			Component::meta_Component_children = ::Uml::CompositionChildRole::Create(comp);
			::Uml::InitCompositionChildRoleProps(Component::meta_Component_children, "Component", true, 0, -1);

		}
		{
			::Uml::Composition comp = ::Uml::Composition::Create(meta);
			::Uml::InitCompositionProps(comp, "Composition", false);
			AnalysisSupportingData::meta_CADAnalysisMetaData_parent = ::Uml::CompositionParentRole::Create(comp);
			::Uml::InitCompositionParentRoleProps(AnalysisSupportingData::meta_CADAnalysisMetaData_parent, "CADAnalysisMetaData_parent", true);
			CADAnalysisMetaData::meta_AnalysisSupportingData_child = ::Uml::CompositionChildRole::Create(comp);
			::Uml::InitCompositionChildRoleProps(CADAnalysisMetaData::meta_AnalysisSupportingData_child, "AnalysisSupportingData", true, 1, 1);

		}
		{
			::Uml::Composition comp = ::Uml::Composition::Create(meta);
			::Uml::InitCompositionProps(comp, "Composition", false);
			Materials::meta_CADAnalysisMetaData_parent = ::Uml::CompositionParentRole::Create(comp);
			::Uml::InitCompositionParentRoleProps(Materials::meta_CADAnalysisMetaData_parent, "CADAnalysisMetaData_parent", true);
			CADAnalysisMetaData::meta_Materials_child = ::Uml::CompositionChildRole::Create(comp);
			::Uml::InitCompositionChildRoleProps(CADAnalysisMetaData::meta_Materials_child, "Materials", true, 1, 1);

		}
		{
			::Uml::Composition comp = ::Uml::Composition::Create(meta);
			::Uml::InitCompositionProps(comp, "Composition", false);
			Assemblies::meta_CADAnalysisMetaData_parent = ::Uml::CompositionParentRole::Create(comp);
			::Uml::InitCompositionParentRoleProps(Assemblies::meta_CADAnalysisMetaData_parent, "CADAnalysisMetaData_parent", true);
			CADAnalysisMetaData::meta_Assemblies_child = ::Uml::CompositionChildRole::Create(comp);
			::Uml::InitCompositionChildRoleProps(CADAnalysisMetaData::meta_Assemblies_child, "Assemblies", true, 1, 1);

		}
		{
			::Uml::Composition comp = ::Uml::Composition::Create(meta);
			::Uml::InitCompositionProps(comp, "Composition", false);
			Component::meta_Assembly_parent = ::Uml::CompositionParentRole::Create(comp);
			::Uml::InitCompositionParentRoleProps(Component::meta_Assembly_parent, "Assembly_parent", true);
			Assembly::meta_Component_children = ::Uml::CompositionChildRole::Create(comp);
			::Uml::InitCompositionChildRoleProps(Assembly::meta_Component_children, "Component", true, 0, -1);

		}

	}

	void InitMetaLinks() {
		MaterialProperties::meta_AllowableBearingStress_child.target() = AllowableBearingStress::meta;

		MaterialProperties::meta_AllowableShearStress_child.target() = AllowableShearStress::meta;

		MaterialProperties::meta_AllowableTensileStress_child.target() = AllowableTensileStress::meta;

		LifeCycle::meta_AnalysisSupportingData_parent.target() = AnalysisSupportingData::meta;
		CADAnalysisMetaData::meta_AnalysisSupportingData_child.target() = AnalysisSupportingData::meta;

		Assembly::meta_Assemblies_parent.target() = Assemblies::meta;
		CADAnalysisMetaData::meta_Assemblies_child.target() = Assemblies::meta;

		Component::meta_Assembly_parent.target() = Assembly::meta;
		Assemblies::meta_Assembly_children.target() = Assembly::meta;

		AnalysisSupportingData::meta_CADAnalysisMetaData_parent.target() = CADAnalysisMetaData::meta;
		Materials::meta_CADAnalysisMetaData_parent.target() = CADAnalysisMetaData::meta;
		Assemblies::meta_CADAnalysisMetaData_parent.target() = CADAnalysisMetaData::meta;

		Component::meta_Component_parent.target() = Component::meta;
		Component::meta_Component_children.target() = Component::meta;
		Assembly::meta_Component_children.target() = Component::meta;

		AnalysisSupportingData::meta_LifeCycle_child.target() = LifeCycle::meta;

		MaterialProperties::meta_Material_parent.target() = Material::meta;
		Materials::meta_Material_children.target() = Material::meta;

		AllowableBearingStress::meta_MaterialProperties_parent.target() = MaterialProperties::meta;
		AllowableShearStress::meta_MaterialProperties_parent.target() = MaterialProperties::meta;
		AllowableTensileStress::meta_MaterialProperties_parent.target() = MaterialProperties::meta;
		Material::meta_MaterialProperties_children.target() = MaterialProperties::meta;

		Material::meta_Materials_parent.target() = Materials::meta;
		CADAnalysisMetaData::meta_Materials_child.target() = Materials::meta;

		Metrics::meta_Metric_children.target() = Metric::meta;

		Metric::meta_Metrics_parent.target() = Metrics::meta;

	}

	void InitMeta(const ::Uml::Diagram &parent) {
		// classes, with attributes, constraints and constraint definitions
		::Uml::SetClass(AllowableBearingStress::meta, parent, "AllowableBearingStress");
		::Uml::SetAttribute(AllowableBearingStress::meta_Value, AllowableBearingStress::meta, "Value");
		::Uml::SetAttribute(AllowableBearingStress::meta_Units, AllowableBearingStress::meta, "Units");
		::Uml::SetAttribute(AllowableBearingStress::meta_Source, AllowableBearingStress::meta, "Source");

		::Uml::SetClass(AllowableShearStress::meta, parent, "AllowableShearStress");
		::Uml::SetAttribute(AllowableShearStress::meta_Value, AllowableShearStress::meta, "Value");
		::Uml::SetAttribute(AllowableShearStress::meta_Units, AllowableShearStress::meta, "Units");
		::Uml::SetAttribute(AllowableShearStress::meta_Source, AllowableShearStress::meta, "Source");

		::Uml::SetClass(AllowableTensileStress::meta, parent, "AllowableTensileStress");
		::Uml::SetAttribute(AllowableTensileStress::meta_Value, AllowableTensileStress::meta, "Value");
		::Uml::SetAttribute(AllowableTensileStress::meta_Units, AllowableTensileStress::meta, "Units");
		::Uml::SetAttribute(AllowableTensileStress::meta_Source, AllowableTensileStress::meta, "Source");

		::Uml::SetClass(AnalysisSupportingData::meta, parent, "AnalysisSupportingData");
		::Uml::SetAttribute(AnalysisSupportingData::meta_AnalysisType, AnalysisSupportingData::meta, "AnalysisType");

		::Uml::SetClass(Assemblies::meta, parent, "Assemblies");

		::Uml::SetClass(Assembly::meta, parent, "Assembly");
		::Uml::SetAttribute(Assembly::meta_ConfigurationID, Assembly::meta, "ConfigurationID");

		::Uml::SetClass(CADAnalysisMetaData::meta, parent, "CADAnalysisMetaData");

		::Uml::SetClass(Component::meta, parent, "Component");
		::Uml::SetAttribute(Component::meta_Name, Component::meta, "Name");
		::Uml::SetAttribute(Component::meta_Type, Component::meta, "Type");
		::Uml::SetAttribute(Component::meta_MaterialID, Component::meta, "MaterialID");
		::Uml::SetAttribute(Component::meta_ComponentInstanceID, Component::meta, "ComponentInstanceID");
		::Uml::SetAttribute(Component::meta_FEAElementType, Component::meta, "FEAElementType");
		::Uml::SetAttribute(Component::meta_FEAElementID, Component::meta, "FEAElementID");

		::Uml::SetClass(LifeCycle::meta, parent, "LifeCycle");
		::Uml::SetAttribute(LifeCycle::meta_Duration, LifeCycle::meta, "Duration");
		::Uml::SetAttribute(LifeCycle::meta_NumberOfCycles, LifeCycle::meta, "NumberOfCycles");

		::Uml::SetClass(Material::meta, parent, "Material");
		::Uml::SetAttribute(Material::meta_MaterialID, Material::meta, "MaterialID");

		::Uml::SetClass(MaterialProperties::meta, parent, "MaterialProperties");

		::Uml::SetClass(Materials::meta, parent, "Materials");

		::Uml::SetClass(Metric::meta, parent, "Metric");
		::Uml::SetAttribute(Metric::meta_ConfigurationID, Metric::meta, "ConfigurationID");
		::Uml::SetAttribute(Metric::meta_MetricType, Metric::meta, "MetricType");
		::Uml::SetAttribute(Metric::meta_TopAssemblyComponentInstanceID, Metric::meta, "TopAssemblyComponentInstanceID");
		::Uml::SetAttribute(Metric::meta_ComponenInstancetID, Metric::meta, "ComponenInstancetID");
		::Uml::SetAttribute(Metric::meta_ComponentName, Metric::meta, "ComponentName");
		::Uml::SetAttribute(Metric::meta_ComponentType, Metric::meta, "ComponentType");
		::Uml::SetAttribute(Metric::meta_RequestedValueType, Metric::meta, "RequestedValueType");
		::Uml::SetAttribute(Metric::meta_MetricID, Metric::meta, "MetricID");
		::Uml::SetAttribute(Metric::meta_Details, Metric::meta, "Details");

		::Uml::SetClass(Metrics::meta, parent, "Metrics");

	}

	void InitMetaLinks(const ::Uml::Diagram &parent) {
		// classes
		::Uml::SetParentRole(AllowableBearingStress::meta_MaterialProperties_parent, AllowableBearingStress::meta, MaterialProperties::meta, "", "");

		::Uml::SetParentRole(AllowableShearStress::meta_MaterialProperties_parent, AllowableShearStress::meta, MaterialProperties::meta, "", "");

		::Uml::SetParentRole(AllowableTensileStress::meta_MaterialProperties_parent, AllowableTensileStress::meta, MaterialProperties::meta, "", "");

		::Uml::SetChildRole(AnalysisSupportingData::meta_LifeCycle_child, AnalysisSupportingData::meta, LifeCycle::meta, "", "");
		::Uml::SetParentRole(AnalysisSupportingData::meta_CADAnalysisMetaData_parent, AnalysisSupportingData::meta, CADAnalysisMetaData::meta, "", "");

		::Uml::SetChildRole(Assemblies::meta_Assembly_children, Assemblies::meta, Assembly::meta, "", "");
		::Uml::SetParentRole(Assemblies::meta_CADAnalysisMetaData_parent, Assemblies::meta, CADAnalysisMetaData::meta, "", "");

		::Uml::SetChildRole(Assembly::meta_Component_children, Assembly::meta, Component::meta, "", "");
		::Uml::SetParentRole(Assembly::meta_Assemblies_parent, Assembly::meta, Assemblies::meta, "", "");

		::Uml::SetChildRole(CADAnalysisMetaData::meta_AnalysisSupportingData_child, CADAnalysisMetaData::meta, AnalysisSupportingData::meta, "", "");
		::Uml::SetChildRole(CADAnalysisMetaData::meta_Materials_child, CADAnalysisMetaData::meta, Materials::meta, "", "");
		::Uml::SetChildRole(CADAnalysisMetaData::meta_Assemblies_child, CADAnalysisMetaData::meta, Assemblies::meta, "", "");

		::Uml::SetChildRole(Component::meta_Component_children, Component::meta, Component::meta, "", "");
		::Uml::SetParentRole(Component::meta_Component_parent, Component::meta, Component::meta, "", "");
		::Uml::SetParentRole(Component::meta_Assembly_parent, Component::meta, Assembly::meta, "", "");

		::Uml::SetParentRole(LifeCycle::meta_AnalysisSupportingData_parent, LifeCycle::meta, AnalysisSupportingData::meta, "", "");

		::Uml::SetChildRole(Material::meta_MaterialProperties_children, Material::meta, MaterialProperties::meta, "", "");
		::Uml::SetParentRole(Material::meta_Materials_parent, Material::meta, Materials::meta, "", "");

		::Uml::SetChildRole(MaterialProperties::meta_AllowableBearingStress_child, MaterialProperties::meta, AllowableBearingStress::meta, "", "");
		::Uml::SetChildRole(MaterialProperties::meta_AllowableShearStress_child, MaterialProperties::meta, AllowableShearStress::meta, "", "");
		::Uml::SetChildRole(MaterialProperties::meta_AllowableTensileStress_child, MaterialProperties::meta, AllowableTensileStress::meta, "", "");
		::Uml::SetParentRole(MaterialProperties::meta_Material_parent, MaterialProperties::meta, Material::meta, "", "");

		::Uml::SetChildRole(Materials::meta_Material_children, Materials::meta, Material::meta, "", "");
		::Uml::SetParentRole(Materials::meta_CADAnalysisMetaData_parent, Materials::meta, CADAnalysisMetaData::meta, "", "");

		::Uml::SetParentRole(Metric::meta_Metrics_parent, Metric::meta, Metrics::meta, "", "");

		::Uml::SetChildRole(Metrics::meta_Metric_children, Metrics::meta, Metric::meta, "", "");

	}

	void _SetXsdStorage()
	{
		UdmDom::str_xsd_storage::StoreXsd("CADAnalysisMetaData.xsd", CADAnalysisMetaData_xsd::getString());
	}

	void Initialize()
	{
		static bool first = true;
		if (!first) return;
		first = false;
		::Uml::Initialize();

	
		UDM_ASSERT( meta == ::Udm::null );

		::UdmStatic::StaticDataNetwork * meta_dn = new ::UdmStatic::StaticDataNetwork(::Uml::diagram);
		meta_dn->CreateNew("CADAnalysisMetaData.mem", "", ::Uml::Diagram::meta, ::Udm::CHANGES_LOST_DEFAULT);
		meta = ::Uml::Diagram::Cast(meta_dn->GetRootObject());

		::Uml::InitDiagramProps(meta, "CADAnalysisMetaData", "1.00");


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
			::Udm::MetaDepository::StoreDiagram("CADAnalysisMetaData", diagram);
		}
		~_regClass()
		{
			::Udm::MetaDepository::RemoveDiagram("CADAnalysisMetaData");
		}
	} __regUnUsed;

}

