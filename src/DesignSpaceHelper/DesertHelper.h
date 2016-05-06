#ifndef DESERTHELPER_H
#define DESERTHELPER_H

#include "DesertIface.h"
#include "DesertIfaceBack.h"
#include "CyPhyML.h"

#include "DesMap.h"
#include "DesBackMap.h"

class DesertHelper
{
public:
	DesertHelper(const std::string &scaFile, Udm::DataNetwork& cyphyDN, Udm::Object focusObject);
	~DesertHelper();
	
	void setRootDS(const CyPhyML::DesignSpace &ds) {rootDS = ds;};
	void setRootDC(const CyPhyML::DesignContainer &dc) {rootDC = dc;};
	void setAllRootDCs(const set<CyPhyML::DesignContainer> &dcs) {allRootDCs = dcs;};

	void close();
	void closeDesertIfaceDN();
	void closeDesertIfaceBackDN();

	void addConstraint(int listIndex, int cyphyObjId, const std::string &name, const std::string &expr, std::string &context);

//	void removeConstraint(int idx);
	bool checkConstraints();
	bool checkConstraints_1();
	void updateConstraint(int idx, const std::string &name, const std::string &expression);
	void updateCyPhy();

	bool runCyPhy2Desert();
	set<std::string> getNaturalParameters();
	set<std::string> getCustomParameters();

//	int applyConstraints(set<int> constraints_list);
//	int applyAllConstraints();
	void applyConstraints(set<int> constraints_list, bool dsRefresh);
	void applyAllConstraints(bool dsRefresh);
	void runDesertFinit_1(const std::string &constraints, bool refresh);
	int runDesertFinit_2();
	//void getDesignSpaceSize(double& dspSize, long& repSize);
	_int64 getDesignSpaceSize();
	int runDesert(const std::string &constraints);
	bool isBackNavigable();
	bool isForwardNavigable();
	void goBack();
	void goForward();
	void getAppliedConstraintSet(set<std::string> &consSet);
	void exportModel_ex(set<int> cfgIds);
	void exportModel_ForMorphMatrix(set<int> cfgIds);

	int getConstraintSize();
	std::string getCurrObjType();
	std::string getCyPhyObjType(int id);
	bool isFocusObject(int id);

	bool getConstraint(int idx, std::string &name, std::string &context, std::string &expression, std::string &ctype, std::string &cdomain);
	bool getConfiguration(int idx, std::string &name, std::string &cfg_no, std::string &cfg_id);	
	bool DesertHelper::getConfigurationUniqueId(int idx, std::string &id);
	void getDesertIfaceSpace(DesertIface::Space &space);
	bool isElementExist(int elemId);
	bool isElementSelected(int cfgId, int elemId, bool forNull=false);
	bool isOptionalElement(const DesertIface::Element &elem);
	void writeLog();

	void executeAll(bool applyConstraints);  //automatically excute all tools with all constraints applied
	void removeCfgsIfEmpty();

	bool isLastDesertFinit_2_fail();
	int getElementInConfigsCount(int elemId);
	std::string getElementInConfigsString(int elemId);

	bool isCfgModelsNull();
	void removeCfgs();
	std::string getElementType(int externalId);
	set<int> getConfigurationsfromElement(int externalId);
	int getConfigurationExternalId(int cfgId);
	void updateNumAssociatedConfigs(CyPhyML::DesignContainer &container, const std::string &cfgSizeStr);

	void clearCfgId2NameMap();
	void updateCfgId2NameMap(int id, const std::string &name);
	void generateSelectionConstraints(set<int> &selectElemIds);
	CyPhyML::Or_operator getOrFromDesignContainer(CyPhyML::DesignContainer &dc);
	void createNewConfigurations();
	set<std::string> getDomainSet();
	set<int> getDesertConstraintlist(const std::string &domain);

	bool showGui;
protected:
	std::unique_ptr<Udm::SmartDataNetwork> ds_dn;
	std::unique_ptr<Udm::SmartDataNetwork> dbs_dn;
	Udm::DataNetwork& cyphy_dn;
	Udm::Object currObj;
	CyPhyML::DesignSpace rootDS;
	CyPhyML::DesignContainer rootDC;
	set<CyPhyML::DesignContainer> allRootDCs;
	std::string cyphyFile;
	std::string desertIfaceFile;
	std::string desertIfaceBackFile;
	std::string desertlogFile;
	int cfgCount;
	int constraintCount;
	
	CyPhyML::Configurations cfg_models;
	set<int> createdCfgIds;

	map<int, DesertIface::Constraint> oldConstraintMap;
	map<int, DesertIface::Constraint> newConstraintMap;
	set<CyPhyML::Constraint> deleteConstraitSet;
	map<CyPhyML::DesignEntity, DesertIface::Element> com2elemMap;
	map<DesertIface::Constraint, CyPhyML::ConstraintBase> dcon2CyphyConMap;
	map<std::string, set<int> > constraintDomainMap;
	map<int, DesertIfaceBack::Configuration> configMap;
	
	map<int, DesertIfaceBack::Element> desertIfaceBackElemMap;
	map<DesertIfaceBack::Configuration, set<int> > configElemMap;
//	map<int, CyPhyML::DeviceConfiguration> cfgDCMap;
//	CyPhyML::DeviceConfigurationFolder dcfolder;

	map<int, std::string> cfgSizeInfoMap;

	list<std::string> log;

	set<int> conId_list;
	bool applyAll;
	std::string apply_constraints;
	UdmDesertMap des_map;
	DesertUdmMap inv_des_map;

	set<std::string> naturalParameters;
	set<std::string> customParameters;

	bool desertFinit_2_fail;

	std::string defaultConstraints;

	typedef map<DesertIfaceBack::Configuration, std::unique_ptr<set<CyPhyML::DesignEntity> > > MorphMatrix;
	MorphMatrix morphMatrix;
	set<CyPhyML::DesignEntity> allEntities;
	string mmFilename;
	int runID;
	FILE* mmfd;
	bool morphMatrixInitialized1;
	map<int, CyPhyML::DesignEntity> mgroupMap;
	map<int, CyPhyML::DesignEntity> moptionMap;

	map<int, std::string> cfgId2NameMap;

	map<int, DesertIface::Element> desertIfaceElemMap;
	map<int, CyPhyML::DesignEntity> cyPhyDesignElemMap;
	DesertIface::ConstraintSet dconstraintSet;
	set<int> existedElemId_in_VC;
	map<CyPhyML::DesignContainer, CyPhyML::Or_operator> dc2orMap;
	map<CyPhyML::Or_operator, DesertIface::Constraint> or2ConstraintMap;

private:
	long m_realConfigCount;
public:
	long getRealConfigCount();
	void updateRealNoOfConfigurationsCount();
	
private:
	std::string getElementFullPath(const DesertIface::Element &elem);
	std::string generateOutputFileName(DesertIfaceBack::Configuration &config, const std::string &outputFdr);
	std::string getFileNameNoExt(const std::string &fullpath);
	void makeDesertIfaceElemMap(const set<DesertIface::Element> &elems);
	void makeConfigElemMap(const DesertIfaceBack::Configuration &cfg);
	std::string appendObjLink(const Udm::Object &obj);

	int applyConstraints(const std::string &constraints_list);
	void checkConfigurationsModel(set<int> &constraintId_list, bool applyAll=false);

	void morphMatrixInitialize1();
	void morphMatrixFinalize1();

	void makeConstraintDomainMap(int index, DesertIface::Constraint &con, std::string ctype="");
	
};

#endif