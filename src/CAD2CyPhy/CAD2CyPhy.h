#ifndef CAD2CYPHY_H
#define CAD2CYPHY_H

#include <string>
#include "MainDialog.h"
#include "CADMetrics.h"
#include "CyPhyML.h"
#include <UdmStatic.h>


#define MASS "Mass"
#define VOLUME "Volume"
#define DENSITY "Density"
#define SURF_AREA "SurfaceArea"
#define CENTER_GRAVITY "CG"


class CAD2CyPhy
{
public:		
	CAD2CyPhy();

	~CAD2CyPhy(){
		if (m_sdnMetrics)
		{
			if (m_sdnMetrics->isOpen())
				m_sdnMetrics->CloseNoUpdate();
			delete m_sdnMetrics;
			m_sdnMetrics = 0;
		}
	}

	typedef map<string, CADMetrics::MetricsBase> ComponentMetricsMap;	///< typedef map[dsID, MetricsBase]

	void UpdateMetrics(const CyPhyML::RootFolder&);	
	bool Initialize();
	bool ParseMetricsFile();
	void ProcessMetricsComponent(const CADMetrics::MetricComponents&);
	void ProcessMetricsAssemblies(const CADMetrics::Assemblies&);
	void ProcessMetricsAssembly(const CADMetrics::Assembly&, ComponentMetricsMap&);
	void ProcessMetricsCADComponent(const CADMetrics::CADComponent&, ComponentMetricsMap&);	
	void FindConfigurations(const CyPhyML::RootFolder&);
	void FindConfigurations(CyPhyML::ComponentAssembly&);
	void FindConfigurations(CyPhyML::ComponentAssemblies&);

	void UpdateConfigurations();										///< updates everything from metrics file
	void UpdateConfigurations(string configID);							///< updates a particular configuration specified by configID
	void UpdateComponentAssembly(CyPhyML::ComponentAssembly&, ComponentMetricsMap&);
	void UpdateMetrics(CyPhyML::Component&, ComponentMetricsMap&);
	void UpdateMetrics(CyPhyML::ComponentRef&, ComponentMetricsMap&);
	void UpdateMetrics(map<string, pair<string, string>>&, CyPhyML::DesignElement&);			//void UpdateMetrics(map<string, string>&, CyPhyML::DesignElement&);
	void UpdateMetrics(map<string, pair<string, string>>&, CyPhyML::ComponentRef&);			//void UpdateMetrics(map<string, string>&, CyPhyML::ComponentRef&);

	// helpers
	bool IsSize2FitParametric(const CyPhyML::Component&);
	void MakeMetricValuePairs(map<string, pair<string, string>>&, CADMetrics::MetricComponent&);		//void MakeMetricValuePairs(map<string, string>&, CADMetrics::MetricComponent&);

private:
	Udm::SmartDataNetwork* m_sdnMetrics;								///< smart data network of the metrics file
	string m_metricsFile;
	map<string, ComponentMetricsMap> m_configurationMetricsMap;			///< component's metric lookup map[configID, ComponentMetricsMap] for each configuration (configurationID is used as key)
	map<long, CADMetrics::MetricsBase> m_metricsLookup;					///< main metrics lookup map[mID, MetricsBase]
	map<string, CyPhyML::ComponentAssembly> m_cyPhyConfigurationMap;		///< configuration map - only doing this because the metric file could have metrics for multiple configurations!
	map<string, string> m_unitsLookup;
};

#endif                                           