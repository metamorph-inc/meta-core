#ifndef CYPHYDSREFINER_H
#define CYPHYDSREFINER_H

#include "CyPhyML.h"
#include "UdmUtil.h"
#include "StatusDialog.h"

class CyPhyDSRefiner
{
public:
	CyPhyDSRefiner();
	CyPhyDSRefiner(CyPhyML::Configurations &cfgs);
	CyPhyDSRefiner(CyPhyML::Configurations &cfgs, set<CyPhyML::CWC> &cwcs);
	//~CyPhyDSRefiner();

	void runDSRefiner();
	CyPhyML::DesignContainer getRefinedDS();
	void startProgressBar();
	void endProgressBar();

private:
	CyPhyML::Configurations _cfgsModel;
	CyPhyML::DesignContainer _rootDS;
	CyPhyML::DesignContainer _newDS;
	set<CyPhyML::CWC> _cwcs;
	CStatusDialog prgDlg;

	UdmUtil::copy_assoc_map _udmcopyMap;
//	map<CyPhyML::DesignEntity, set<CyPhyML::DesignEntityRef> > designElemRefMap;
	set<CyPhyML::DesignEntity> _activeDesignEntities;
	set<CyPhyML::DecisionGroup> _allDecisionGroups;
	set<CyPhyML::DesignEntity> _groupEntities;
	set<CyPhyML::DesignContainer> _optionalContainers;

	void preProcessDS(CyPhyML::DesignContainer &dc);
	void createConstraint();
	void analyseDS(CyPhyML::DesignContainer &dc, set<CyPhyML::DesignEntity> &allDesignEntities);
	void processDecisionGroup(CyPhyML::DesignContainer &dc);
	void cleanDecisionGroup();

	Udm::Object getMappedObject(const Udm::Object &src_obj);
};

#endif