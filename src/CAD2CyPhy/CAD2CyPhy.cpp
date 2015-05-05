#include "CAD2CyPhy.h"

//#include "StdAfx.h"
#include "AssemblyInterface.h"
#include <UdmDom.h>
#include <UdmConsole.h>
#include "string_utils.h"

#include <UmlExt.h>
#include "UdmGme.h"
#include "Mga.h"

CAD2CyPhy::CAD2CyPhy()
		:m_sdnMetrics(0)
{
	// initialize 	map<string, string> m_unitsLookup;
}

void CAD2CyPhy::UpdateMetrics(const CyPhyML::RootFolder& rf)
{
	if (Initialize())
	{
		if (ParseMetricsFile())
		{
			if (m_configurationMetricsMap.size() > 0 && m_metricsLookup.size() > 0)
			{
				int mSize = m_metricsLookup.size();
				int size = m_configurationMetricsMap.size();
				for (map<string, ComponentMetricsMap>::const_iterator ci = m_configurationMetricsMap.begin(); ci != m_configurationMetricsMap.end(); ci++)
				{
					string cID = ci->first;
					int metricsMapSize = ci->second.size();
				}

				FindConfigurations(rf);
				UpdateConfigurations();
			}
		}
	}
	return;
}

bool CAD2CyPhy::Initialize()
{
	MainDialog dialog = MainDialog();
	//quit if user cancels
	if(dialog.DoModal() != IDOK) 
		return 0;

	string tmp = dialog.GetMetricsFile();
	if (tmp.find(".xml") == string::npos)
		return 0;

	m_metricsFile = tmp;
	return 1;
}

bool CAD2CyPhy::ParseMetricsFile()
{
	//	UdmDom::DomDataNetwork dn(CADMetrics::diagram);
	//	m_metricsFile = "C:\\Users\\yaod\\Desktop\\CADMetric_TEST.xml";
	
	m_sdnMetrics = new Udm::SmartDataNetwork(CADMetrics::diagram);
	m_sdnMetrics->OpenExisting(m_metricsFile, "CADMetrics", Udm::CHANGES_LOST_DEFAULT);
	CADMetrics::CADMetricRoot root = CADMetrics::CADMetricRoot::Cast(m_sdnMetrics->GetRootObject());

	if (root == Udm::null)
		return 0;

	// Creates metrics lookup map<mID, Metrics>
	CADMetrics::MetricComponents metricsComponent = root.MetricComponents_child();
	ProcessMetricsComponent(metricsComponent);

	// Creates cadComponent metrics lookup map<cID, map<mID, Metrics>>
	CADMetrics::Assemblies assemblies = root.Assemblies_child();
	ProcessMetricsAssemblies(assemblies);
	return 1;
}

void CAD2CyPhy::ProcessMetricsComponent(const CADMetrics::MetricComponents& mc)
{
	set<CADMetrics::MetricComponent> mc_Set = mc.MetricComponent_kind_children();
	for (set<CADMetrics::MetricComponent>::const_iterator ci = mc_Set.begin(); ci != mc_Set.end(); ci++)
	{
		CADMetrics::MetricComponent mc(*ci);
		m_metricsLookup[mc.MetricID()] = mc;
	}
}

void CAD2CyPhy::ProcessMetricsAssemblies(const CADMetrics::Assemblies& assemblies)
{
	set<CADMetrics::Assembly> assembly_Set = assemblies.Assembly_kind_children(); 
	for (set<CADMetrics::Assembly>::const_iterator ci = assembly_Set.begin(); ci != assembly_Set.end(); ci++)
	{
		CADMetrics::Assembly assembly (*ci);
		string configID = assembly.ConfigurationID();

		ComponentMetricsMap configMetricMap;
		ProcessMetricsAssembly(assembly, configMetricMap);
		m_configurationMetricsMap[configID] = configMetricMap;
	}
}

void CAD2CyPhy::ProcessMetricsAssembly(const CADMetrics::Assembly& assembly, ComponentMetricsMap& metricsMap)
{
	set<CADMetrics::CADComponent> component_Set = assembly.CADComponent_kind_children();
	for (set<CADMetrics::CADComponent>::const_iterator ci = component_Set.begin(); ci != component_Set.end(); ci++)
	{
		CADMetrics::CADComponent component(*ci);
		ProcessMetricsCADComponent(component, metricsMap);
	}
}

void CAD2CyPhy::ProcessMetricsCADComponent(const CADMetrics::CADComponent& component, ComponentMetricsMap& metricsMap)
{
	string cID = component.ComponentID();
	long mID = component.MetricID();
	
	map<long, CADMetrics::MetricsBase>::iterator i = m_metricsLookup.find(mID);
	if (i != m_metricsLookup.end())
		metricsMap[cID] = i->second;

	set<CADMetrics::CADComponent> componentChildren_Set = component.CADComponent_kind_children();
	for (set<CADMetrics::CADComponent>::const_iterator ci = componentChildren_Set.begin(); ci != componentChildren_Set.end(); ci++)
	{
		CADMetrics::CADComponent compChild(*ci);
		ProcessMetricsCADComponent(compChild, metricsMap);
	}
}

void CAD2CyPhy::UpdateConfigurations()
{		
	// we have map<string,  ComponentMetricsMap> m_componentMetricsMap made
	// we find the corresponding configuration in GME
	// traverse through each configuration
	//			for each Component/ComponentAssembly
	//				find it in componentMetricsMap
	//				getMetrics from componentMetricsMap
	//				update metrics in the Component/ComponentAssembly

	for (map<string, ComponentMetricsMap>::const_iterator ci = m_configurationMetricsMap.begin(); ci != m_configurationMetricsMap.end(); ci++)
	{		
		string configID = ci->first;
		ComponentMetricsMap componentMetricsMap = ci->second;

		map<string, CyPhyML::ComponentAssembly>::iterator i = m_cyPhyConfigurationMap.find(configID);		
		if (i != m_cyPhyConfigurationMap.end())
		{
			CyPhyML::ComponentAssembly assembly2update = i->second;
			UpdateComponentAssembly(assembly2update, componentMetricsMap);
		}
	}
}

void CAD2CyPhy::UpdateConfigurations(string configID)
{
	map<string, ComponentMetricsMap>::const_iterator ci = m_configurationMetricsMap.find(configID);
	if (ci != m_configurationMetricsMap.end())
	{
		ComponentMetricsMap componentMetricsMap = ci->second;
		map<string, CyPhyML::ComponentAssembly>::iterator i = m_cyPhyConfigurationMap.find(configID);		
		if (i != m_cyPhyConfigurationMap.end())
		{
			CyPhyML::ComponentAssembly assembly2update = i->second;
			UpdateComponentAssembly(assembly2update, componentMetricsMap);
		}
	}
}

void CAD2CyPhy::UpdateComponentAssembly(CyPhyML::ComponentAssembly& assembly, ComponentMetricsMap& metricsMap)
{
	int aID = assembly.ID();
	string aID_str;
	to_string(aID_str, aID);

	ComponentMetricsMap::const_iterator ci = metricsMap.find(aID_str);
	if (ci != metricsMap.end())
	{
		if (ci->second.type() == CADMetrics::MetricComponent::meta)
		{
			CADMetrics::MetricComponent metric = CADMetrics::MetricComponent::Cast(ci->second);
			map<string, pair<string, string>> metricValuePairs;
			MakeMetricValuePairs(metricValuePairs, metric);
			UpdateMetrics(metricValuePairs, assembly);
		}
	}	


	set<CyPhyML::ComponentAssembly> compAsm_Set = assembly.ComponentAssembly_kind_children();
	for (set<CyPhyML::ComponentAssembly>::iterator i = compAsm_Set.begin(); i != compAsm_Set.end(); i++)
	{
		CyPhyML::ComponentAssembly assembly(*i);
		UpdateComponentAssembly(assembly, metricsMap);
	}
	
	set<CyPhyML::ComponentRef> componentRef_Set = assembly.ComponentRef_kind_children();
	for (set<CyPhyML::ComponentRef>::iterator i = componentRef_Set.begin(); i != componentRef_Set.end(); i++)
	{
		CyPhyML::ComponentRef cRef(*i);
		UpdateMetrics(cRef, metricsMap);
	}
	
	set<CyPhyML::Component> component_Set = assembly.Component_kind_children();
	for (set<CyPhyML::Component>::iterator i = component_Set.begin(); i != component_Set.end(); i++)
	{
		CyPhyML::Component component(*i);
		UpdateMetrics(component, metricsMap);
	}
}

void CAD2CyPhy::UpdateMetrics(CyPhyML::Component& component, ComponentMetricsMap& metricsMap)
{
	// metrics only need to be updated for size2fit components
	if (IsSize2FitParametric(component))
	{
		int cID = component.ID();
		string cID_str;
		to_string(cID_str, cID);

		ComponentMetricsMap::const_iterator ci = metricsMap.find(cID_str);
		if (ci != metricsMap.end())
		{
			CADMetrics::MetricsBase metric = ci->second;

			if (metric.type() == CADMetrics::MetricComponent::meta)
			{
				CADMetrics::MetricComponent metricComponent = CADMetrics::MetricComponent::Cast(metric);
				map<string, pair<string, string>> metricValuePairs;		//map<string, string> metricValuePairs;
				MakeMetricValuePairs(metricValuePairs, metricComponent);
				UpdateMetrics(metricValuePairs, component);				
				
			}
		}
	}
}

void CAD2CyPhy::UpdateMetrics(CyPhyML::ComponentRef& componentRef, ComponentMetricsMap& metricsMap)
{
	// only deal with reference to Components - even though ComponentRef references DesignElement
	Udm::Object objRef = componentRef.getReferencedObject();
	if (objRef.type() == CyPhyML::Component::meta)
	{
		CyPhyML::Component component = CyPhyML::Component::Cast(objRef);
		if (IsSize2FitParametric(component))
		{
			int cID = component.ID();
			string cID_str;
			to_string(cID_str, cID);

			ComponentMetricsMap::const_iterator ci = metricsMap.find(cID_str);

			if (ci != metricsMap.end())
			{
				CADMetrics::MetricsBase metric = ci->second;

				if (metric.type() == CADMetrics::MetricComponent::meta)
				{
					CADMetrics::MetricComponent metricComponent = CADMetrics::MetricComponent::Cast(metric);
					map<string, pair<string, string>> metricValuePairs;			//map<string, string> metricValuePairs;
					MakeMetricValuePairs(metricValuePairs, metricComponent);
					UpdateMetrics(metricValuePairs, componentRef);					
				}
			}
		}
	}
}

void CAD2CyPhy::UpdateMetrics(map<string, pair<string, string>>& metricValuePairs, CyPhyML::DesignElement& de)
{
	for (map<string, pair<string, string>>::iterator i = metricValuePairs.begin(); i != metricValuePairs.end(); i++)
	{
		string mParamName = i->first;
		pair<string, string> val_unit = i->second;
		
		string mValue = val_unit.first, mUnit = val_unit.second;

		CyPhyML::CADProperty cProp;

		set<CyPhyML::CADProperty> cadProperty_Set = de.CADProperty_kind_children();
		for (set<CyPhyML::CADProperty>::iterator i = cadProperty_Set.begin(); i != cadProperty_Set.end(); i++)
		{
			if (i->ParameterName() == mParamName)
			{
				cProp = *i;
				break;
			}
		}

		if (cProp == Udm::null)
		{
			cProp = CyPhyML::CADProperty::Create(de);	
			cProp.ParameterName() = mParamName;
			cProp.name() = mParamName;
		}

		cProp.UnitOfMeasurement() = mUnit;
		cProp.Value() = mValue;
	}
}

void CAD2CyPhy::UpdateMetrics(map<string, pair<string, string>>& metricValuePairs, CyPhyML::ComponentRef& compRef)
{
	// convert objRef to CComQIPtr
	// create/update registry values
	CComQIPtr<IMgaFCO> icompRef =  UdmGme::Udm2Gme(compRef);
	if (icompRef)
	{
		for (map<string, pair<string, string>>::iterator i = metricValuePairs.begin(); i != metricValuePairs.end(); i++)
		{
			pair<string, string> val_unit = i->second;

			/*
			CComPtr<IMgaRegNode> valueNode;
			CComPtr<IMgaRegNode> unitNode;

			string valuePath = "CADPROPERTIES/" + i->first + "/Value";
			string unitPath = "CADPROPERTIES/" + i->first + "/Unit";

			icompRef->get_RegistryNode(CComBSTR(valuePath.c_str()), &valueNode);
			icompRef->get_RegistryNode(CComBSTR(unitPath.c_str()), &unitNode);

			if (valueNode != NULL)
				valueNode->put_Value(CComBSTR(val_unit.first.c_str()));
			
			if (unitNode != NULL)
				unitNode->put_Value(CComBSTR(val_unit.second.c_str()));
	*/

		
			string path = "CADPROPERTIES/" + i->first;
			string valuePath = path + "/Value";
			string unitPath = path + "/Unit";
			HRESULT result = icompRef->put_RegistryValue(CComBSTR(valuePath.c_str()), CComBSTR(val_unit.first.c_str()));
			result = icompRef->put_RegistryValue(CComBSTR(unitPath.c_str()), CComBSTR(val_unit.second.c_str()));
		

			if (result != S_OK)
				GMEConsole::Console::writeLine("Couldn't assign registry value for ComponentRef" + UdmGme::UdmId2GmeId(compRef.uniqueId()), MSG_ERROR);
			
		}
	}
}

// map<ParamName, pair<Value, Unit>>
void CAD2CyPhy::MakeMetricValuePairs(map<string, pair<string, string>>& metricValueMap, CADMetrics::MetricComponent& metricComponent)
{
	string val, tmp;
	CADMetrics::Scalars scalars = metricComponent.Scalars_child();

	set<CADMetrics::Scalar> scalar_Set = scalars.Scalar_kind_children();
	for (set<CADMetrics::Scalar>::const_iterator ci = scalar_Set.begin(); ci != scalar_Set.end(); ci++)
	{
		CADMetrics::Scalar scalar(*ci);		
		to_string(val, (double)scalar.Value());
		metricValueMap[scalar.Name()] = make_pair(val, scalar.Unit());
	}

	val.clear();
	CADMetrics::CG centerGravity = metricComponent.CG_child();	

	to_string(tmp, (double)centerGravity.X());	
	val += tmp + ":"; tmp.clear();
	to_string(tmp, (double)centerGravity.Y());	
	val += tmp +":"; tmp.clear();	
	to_string(tmp, (double)centerGravity.Z());
	val += tmp;

	CADMetrics::Units metricUnits = metricComponent.Units_child();	
	metricValueMap[CENTER_GRAVITY] = make_pair(val, metricUnits.Distance());
}



#if 0
void CAD2CyPhy::UpdateMetrics(map<string, string>& metricValuePairs, CyPhyML::ComponentRef& compRef)
{			
	// convert objRef to CComQIPtr
	// create/update registry values
	CComQIPtr<IMgaFCO> icompRef =  UdmGme::Udm2Gme(compRef);
	if (icompRef)
	{
		for (map<string, string>::iterator i = metricValuePairs.begin(); i != metricValuePairs.end(); i++)
		{
			string path = "CADPROPERTIES/" + i->first;
			HRESULT result = icompRef->put_RegistryValue(CComBSTR(path.c_str()), CComBSTR(i->second.c_str()));

			if (result != S_OK)
				GMEConsole::Console::writeLine("Couldn't assign registry value for ComponentRef" + UdmGme::UdmId2GmeId(compRef.uniqueId()), MSG_ERROR);
		}
	}
}


void CAD2CyPhy::UpdateMetrics(map<string, string>& metricValuePairs, CyPhyML::DesignElement& de)
{
	for (map<string, string>::iterator i = metricValuePairs.begin(); i != metricValuePairs.end(); i++)
	{
		string mParamName = i->first;
		string mValue = i->second;

		CyPhyML::CADProperty cProp;

		set<CyPhyML::CADProperty> cadProperty_Set = de.CADProperty_kind_children();
		for (set<CyPhyML::CADProperty>::iterator i = cadProperty_Set.begin(); i != cadProperty_Set.end(); i++)
		{
			if (i->ParameterName() == mParamName)
			{
				cProp = *i;
				break;
			}
		}

		if (cProp == Udm::null)
		{
			cProp = CyPhyML::CADProperty::Create(de);	
			cProp.ParameterName() = mParamName;
			cProp.name() = mParamName;
		}

		cProp.Value() = mValue;
	}
}



void CAD2CyPhy::MakeMetricValuePairs(map<string, string>& metricValuePairs, CADMetrics::MetricComponent& metricComponent)
{		


	/*
	string val;

	to_string(val, (double)scalar.SurfaceArea());
	metricValuePairs[SURF_AREA] = val;
	val.clear();

	to_string(val, (double)scalar.Volume());
	scalar.Volume();
	metricValuePairs[VOLUME] = val;		
	val.clear();

	to_string(val, (double)scalar.Density());
	metricValuePairs[DENSITY] = val;			
	val.clear();

	to_string(val, (double)scalar.Mass());
	metricValuePairs[MASS] = val;				
	val.clear();
	*/

	CADMetrics::CG centerGravity = metricComponent.CG_child();	
	to_string(val, (double)centerGravity.X() + (double)centerGravity.Y() + (double)centerGravity.Z());
	metricValuePairs[CENTER_GRAVITY] = val;
}
#endif


void CAD2CyPhy::FindConfigurations(const CyPhyML::RootFolder& rf)
{
//	CyPhyML::ComponentAssembly configOfInterest;
	set<CyPhyML::Components> components_Set = rf.Components_kind_children();
	for (set<CyPhyML::Components>::const_iterator ci = components_Set.begin(); ci != components_Set.end(); ci++)
	{
		CyPhyML::Components componentsFolder(*ci);
		set<CyPhyML::ComponentAssemblies> assemblies_Set = componentsFolder.ComponentAssemblies_kind_children();
		for (set<CyPhyML::ComponentAssemblies>::const_iterator di = assemblies_Set.begin(); di != assemblies_Set.end(); di++)
		{
			CyPhyML::ComponentAssemblies assemblies(*di);
			FindConfigurations(assemblies);
		}
	}

	return;
}


void CAD2CyPhy::FindConfigurations(CyPhyML::ComponentAssembly& assembly)
{
	long configID = assembly.ConfigurationUniqueID();
	string name = assembly.name();
	if (configID > 0)
	{
		string tmp;
		to_string(tmp, configID);
		m_cyPhyConfigurationMap[tmp] = assembly;
	}
}


void CAD2CyPhy::FindConfigurations(CyPhyML::ComponentAssemblies& assemblies)
{	
	set<CyPhyML::ComponentAssembly> assembly_Set = assemblies.ComponentAssembly_kind_children();
	for (set<CyPhyML::ComponentAssembly>::const_iterator ci = assembly_Set.begin(); ci != assembly_Set.end(); ci++)
	{
		CyPhyML::ComponentAssembly c_assembly(*ci);        string name = c_assembly.name();
		FindConfigurations(c_assembly);
	}

	set<CyPhyML::ComponentAssemblies> assemblyFolder_Set = assemblies.ComponentAssemblies_kind_children();
	for (set<CyPhyML::ComponentAssemblies>::const_iterator i = assemblyFolder_Set.begin(); i != assemblyFolder_Set.end(); i++)
	{
		CyPhyML::ComponentAssemblies c_assemblies(*i);		string name = c_assemblies.name();
		FindConfigurations(c_assemblies);
	}
	return;
}

// helper
bool CAD2CyPhy::IsSize2FitParametric(const CyPhyML::Component& component)
{
	set<CyPhyML::SizedToFit> s2f_Set = component.SizedToFit_kind_children();
	set<CyPhyML::CADParameter> CADParam_Set = component.CADParameter_kind_children(); 
	return (s2f_Set.size() > 0 || CADParam_Set.size() > 0);
}