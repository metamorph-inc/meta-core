#ifndef DESERT2CYPHY_CPP
#define DESERT2CYPHY_CPP

#include "desert2cyphy.h"
#include "uml.h"
#include "UmlExt.h"
#include "UdmUtil.h"

#define DEVICECONFIG "DeviceConfiguration"

RootFolder cyphy_rootfolder;
DesertBackSystem desert_system;
//DeviceConfigurationFolder dcfolder;
//DeviceConfiguration dcd;
//CyPhyML::DesignContainer cfg_dc;
CyPhyML::Configurations cfgs_model;
CyPhyML::Configurations old_cfgs_model;
CyPhyML::CWC cfg_model;
CyPhyML::ComponentAssembly comassem;
Configuration config;
map<int, int> alternativeMap;
set<int> comIdSet;
map<CyPhyML::DesignElement, CyPhyML::DesignElement> elemMap;
map<Udm::Object, Udm::Object> copyMap;

void initD2C();
void makeAlternativeMap();
void traversDesignSpace(const CyPhyML::DesignSpace &ds, MorphMatrix& morphMatrix, set<CyPhyML::DesignEntity>& allEntities);
void traverseContainer(CyPhyML::DesignContainer &container, MorphMatrix& morphMatrix, set<CyPhyML::DesignEntity>& allEntities);
void createCWCReference(CyPhyML::DesignContainer &container);


// MorphMatrix only (doesn't export to GME)
void clearAllMaps();
CyPhyML::DesignContainer& initializeMorphMatrixCreation(RootFolder &cyphy_rf, CyPhyML::Configurations &cfgModels, DesertBackSystem &desert_top, Configuration &cfg);
void traversDesignSpaceForMorphMatrix(const CyPhyML::DesignSpace &ds, MorphMatrix& morphMatrix, set<CyPhyML::DesignEntity>& allEntities);
void traverseContainerForMorphMatrix(CyPhyML::DesignContainer &container, MorphMatrix& morphMatrix, set<CyPhyML::DesignEntity>& allEntities);

void generateCyPhy(RootFolder &cyphy_rf,
					DesertBackSystem &desert_top,
				//	DeviceConfigurationFolder &dcfdr,
					Configuration &cfg, MorphMatrix& morphMatrix, set<CyPhyML::DesignEntity>& allEntities)
{
	//dcfolder = dcfdr;
	config = cfg;
	generateCyPhy(cyphy_rf, desert_top, morphMatrix, allEntities);
}

string GetGMEGUID(Udm::Object &object)
{
	string guid = "";
	CComQIPtr<IMgaObject> cObj;
	CComPtr<IUnknown> punk;
	punk.Attach(UdmGme::Udm2Gme(object));
	punk.QueryInterface(&cObj);

	if (cObj)
	{
		_bstr_t cGuid;
		HRESULT res = cObj->GetGuidDisp(cGuid.GetAddress());
		if (res == S_OK)
			guid = cGuid;
	}

	size_t begin = guid.find('{');
	size_t end = guid.find('}');

	if (begin != string::npos)
		guid = guid.substr(begin+1);

	if (end != string::npos)
		guid = guid.substr(0, end-1);

	return guid;
}

CyPhyML::CWC generateCyPhy(RootFolder &cyphy_rf, CyPhyML::Configurations &cfgModels, DesertBackSystem &desert_top,
				   Configuration &cfg, MorphMatrix& morphMatrix, set<CyPhyML::DesignEntity>& allEntities)
{
	alternativeMap.clear();
	elemMap.clear();
	copyMap.clear();

	cyphy_rootfolder = cyphy_rf;
	desert_system = desert_top;
	config = cfg;
	cfgs_model = cfgModels;

	int cfgId = desert_system.selConfig();
	char buffer[65];
	_itoa(cfgId, buffer, 10);

	if(config == Udm::null)
	{
		const set<Configuration> &cfgs  = desert_system.Configuration_kind_children();
		for(set<Configuration>::const_iterator i=cfgs.begin();i!=cfgs.end();++i)
		{
			if((*i).id() == cfgId)
			{
				config = *i;
				break;
			}
		}
	}

	if(config == Udm::null) return cfg_model;

	makeAlternativeMap();

	cfg_model = CyPhyML::CWC::Create(cfgs_model);
	//cfg_model.name() = "cfg" + (std::string)buffer;
//	cfg_model.name() = "cfg: "+cfg_id;
	cfg_model.ID() = GetGMEGUID(cfg_model);
//	cfg_model.CfgID() = cfg_id;

	CyPhyML::DesignContainer cfg_top = cfgModels.DesignContainer_parent();
	//if(Uml::IsDerivedFrom(cfg_top.type(), CyPhyML::DesignContainer::meta))
	traverseContainer(cfg_top, morphMatrix, allEntities);
	//else
	//	traversDesignSpace(CyPhyML::DesignSpace::Cast(cfg_top));

	alternativeMap.clear();
	elemMap.clear();
	copyMap.clear();

	return cfg_model;
}

void initD2C()
{
	alternativeMap.clear();
	elemMap.clear();
	copyMap.clear();

	//cyphy_rootfolder = cyphy_rf;
	//desert_system = desert_top;
	//Create SoftwareAssembly model with configuration #

	int cfgId = desert_system.selConfig();
	char buffer[65];
	_itoa(cfgId, buffer, 10);

	if(config == Udm::null)
	{
		const set<Configuration> &cfgs  = desert_system.Configuration_kind_children();
		for(set<Configuration>::const_iterator i=cfgs.begin();i!=cfgs.end();++i)
		{
			if((*i).id() == cfgId)
			{
				config = *i;
				break;
			}
		}
	}

	if(config == Udm::null) return;

	makeAlternativeMap();
}

void generateCyPhy(RootFolder &cyphy_rf, DesertBackSystem &desert_top, MorphMatrix& morphMatrix, set<CyPhyML::DesignEntity>& allEntities)
{
	cyphy_rootfolder = cyphy_rf;
	desert_system = desert_top;

	initD2C();

	const set<DesignSpace> &dss = cyphy_rf.DesignSpace_kind_children();
	for(set<DesignSpace>::const_iterator di=dss.begin();di!=dss.end(); ++di)
	{
		traversDesignSpace(*di, morphMatrix, allEntities);
	}

	//const set<CyPhyML::DesignContainer> &dcs = cyphy_rf.DesignContainer_kind_children();
	//for(set<CyPhyML::DesignContainer>::const_iterator i=dcs.begin();i!=dcs.end(); ++i)
	//{
	//	CyPhyML::DesignContainer dc = *i;
	//	traverseContainer(dc);
	//}
//	createConnections();

	alternativeMap.clear();
	elemMap.clear();
	copyMap.clear();
}

void makeAlternativeMap()
{
	std::string n = config.name();
	const set<AlternativeAssignment> &alts = config.AlternativeAssignment_kind_children();
	for(set<AlternativeAssignment>::const_iterator i=alts.begin();i!=alts.end();++i)
	{
		const AlternativeAssignment &alt = *i;
		const Element &alt_of_end = alt.alternative_of_end();
		const Element &alt_end = alt.alternative_end();
		alternativeMap[alt_of_end.id()] = alt_end.id();
		comIdSet.insert(alt_end.id());
	}
}

void traversDesignSpace(const CyPhyML::DesignSpace &ds, MorphMatrix& morphMatrix, set<CyPhyML::DesignEntity>& allEntities)
{
	const set<CyPhyML::DesignSpace> &dsfdrs = ds.DesignSpace_kind_children();
	for(set<CyPhyML::DesignSpace>::const_iterator di=dsfdrs.begin();di!=dsfdrs.end();++di)
	{
		traversDesignSpace(*di, morphMatrix, allEntities);
	}

	const set<CyPhyML::DesignContainer> &containers = ds.DesignContainer_kind_children();
	for(set<CyPhyML::DesignContainer>::iterator ci=containers.begin();ci!=containers.end();++ci)
	{
		CyPhyML::DesignContainer container = *ci;
		traverseContainer(container, morphMatrix, allEntities);
	}

}

bool componentFollowsFromInTypeInstanceHierarchy(const CyPhyML::DesignEntity& selectedCom, const CyPhyML::DesignEntity& rootChild)
{
	if(selectedCom == rootChild)
		return true;

	if(selectedCom.isInstance() || selectedCom.isSubtype()) {
		CyPhyML::DesignEntity parentType = selectedCom.Archetype();
		return componentFollowsFromInTypeInstanceHierarchy(parentType, rootChild);
	}

	return false;
}

void addSelectedEntity(set<CyPhyML::DesignEntity>& allEntities, set<CyPhyML::DesignEntity>* curEntities, const CyPhyML::DesignEntity& selectedCom, const CyPhyML::DesignContainer& parent)
{
	CyPhyML::DesignContainer groupBaseType = parent;
	if(groupBaseType.isInstance() || groupBaseType.isSubtype()) {
		groupBaseType = CyPhyML::DesignContainer::Cast(groupBaseType.Archetype());
		while(groupBaseType.isInstance() || groupBaseType.isSubtype()) {
			groupBaseType = CyPhyML::DesignContainer::Cast(groupBaseType.Archetype());
		}
	}
	curEntities->insert(groupBaseType);
	allEntities.insert(groupBaseType);
	set<CyPhyML::DesignEntity> rootChildren = groupBaseType.DesignEntity_kind_children();
	for(set<CyPhyML::DesignEntity>::iterator rootChIt = rootChildren.begin(); rootChIt != rootChildren.end(); ++rootChIt) {
		CyPhyML::DesignEntity rootChild = *rootChIt;
		if(componentFollowsFromInTypeInstanceHierarchy(selectedCom, rootChild)) {
			curEntities->insert(rootChild);
			allEntities.insert(rootChild);
		}
	}
}

void traverseContainer(CyPhyML::DesignContainer &container, MorphMatrix& morphMatrix, set<CyPhyML::DesignEntity>& allEntities)
{
	set<CyPhyML::DesignEntity> entities = container.DesignEntity_kind_children();

	// First, populate the Morph Matrix
	set<CyPhyML::DesignEntity>* curEntities = 0;
	MorphMatrix::iterator pos;
	try {
		pos = morphMatrix.find(config);
	} catch (...) {
		// shouldn't normally occur
		pos = morphMatrix.end();
	}
	if(pos==morphMatrix.end()) {
		curEntities = new set<CyPhyML::DesignEntity>();
		auto inserted = morphMatrix.insert(MorphMatrix::value_type(config, std::unique_ptr<set<CyPhyML::DesignEntity> >(curEntities)));
	} else {
		curEntities = morphMatrix[config].get();
	}


	// Continue with configuration models creation
	if((std::string)container.ContainerType()!="Compound")
	{
		map<int, int>::iterator pos = alternativeMap.find(container.ID());
		if(pos==alternativeMap.end())
			return;

		//get object by id
		int altId = (*pos).second;
		if(altId<=0)
		{
			if((std::string)container.ContainerType()=="Optional")
			{
				CyPhyML::NullOptionRef null_ref = CyPhyML::NullOptionRef::Create(cfg_model);
				null_ref.name() = std::string(container.name())+"_null_ref";
				null_ref.ref() = container;
			}
			return;
		}

		CyPhyML::DesignEntity selectedCom;
		for(set<CyPhyML::DesignEntity>::iterator i=entities.begin();i!=entities.end();++i)
		{
			if((*i).ID() == altId)
			{
				selectedCom = *i;
				break;
			}
		}
		if(selectedCom == Udm::null)
		{
			char buffer[10];
			_itoa(altId, buffer, 10);
			std::string err = "Cannot find the Component with ID: "+(std::string)buffer+" in DesignContainer: "+(std::string)container.name();
			throw udm_exception(err.c_str());
		}
		CyPhyML::DesignEntityRef com_ref = CyPhyML::DesignEntityRef::Create(cfg_model);
		com_ref.name() = std::string(selectedCom.name())+"_ref";
		com_ref.ref() = selectedCom;
		addSelectedEntity(allEntities, curEntities, selectedCom, container);

		createCWCReference(container);

		//if(Uml::IsDerivedFrom(selectedCom.type(), CyPhyML::DesignContainer::meta))
		if((std::string)selectedCom.type().name()=="DesignContainer")
		{
			traverseContainer(CyPhyML::DesignContainer::Cast(selectedCom), morphMatrix, allEntities);
		}
	}
	else
	{
		createCWCReference(container);
		for(set<CyPhyML::DesignEntity>::iterator i=entities.begin();i!=entities.end();++i)
		{
			const CyPhyML::DesignEntity &com = *i;
			addSelectedEntity(allEntities, curEntities, com, container);
			std::string comtype = com.type().name();
			if(Uml::IsDerivedFrom(com.type(), CyPhyML::DesignElement::meta)||Uml::IsDerivedFrom(com.type(), CyPhyML::ComponentRef::meta))
			{
				CyPhyML::DesignEntityRef com_ref = CyPhyML::DesignEntityRef::Create(cfg_model);
				com_ref.name() = std::string(com.name())+"_ref";
				com_ref.ref() = com;
			}
			//else if(Uml::IsDerivedFrom(com.type(), CyPhyML::DesignContainer::meta))
			else if(comtype == "DesignContainer")
			{
				traverseContainer(CyPhyML::DesignContainer::Cast(com), morphMatrix, allEntities);
			}
		}
	}
}

void createCWCReference(CyPhyML::DesignContainer &container)
{
	if(container == cfgs_model.parent()) return;

	set<CyPhyML::CWCReference> cwcrefs = container.CWCReference_kind_children();
	for(set<CyPhyML::CWCReference>::iterator i=cwcrefs.begin();i!=cwcrefs.end();++i)
	{
		CyPhyML::Configurations cfgmodels = (*i).ref();
		if(cfgmodels == cfgs_model)
			return;
	}

	string tmp = container.name();
	if(!container.isInstance())
	{
		CyPhyML::CWCReference cwc_ref = CyPhyML::CWCReference::Create(container);
		cwc_ref.name() = "configurations ref";
		cwc_ref.ref() = cfgs_model;
	}
	else
	{
		cfg_model.DeleteObject();
		throw udm_exception((string)container.name()+" cannot be instance model.");
	}
}

void clearAllMaps()
{
	alternativeMap.clear();
	elemMap.clear();
	copyMap.clear();
}

void initializeMorphMatrixCreation(RootFolder &cyphy_rf, CyPhyML::Configurations &cfgModels, DesertBackSystem &desert_top, Configuration &cfg, CyPhyML::DesignContainer& cfg_top)
{
	clearAllMaps();

	cyphy_rootfolder = cyphy_rf;
	desert_system = desert_top;
	config = cfg;
	cfgs_model = cfgModels;

	int cfgId = desert_system.selConfig();
	char buffer[65];
	_itoa(cfgId, buffer, 10);

	if(config == Udm::null)
	{
		const set<Configuration> &cfgs  = desert_system.Configuration_kind_children();
		for(set<Configuration>::const_iterator i=cfgs.begin();i!=cfgs.end();++i)
		{
			if((*i).id() == cfgId)
			{
				config = *i;
				break;
			}
		}
	}

	if(config != Udm::null) {

		makeAlternativeMap();

		if(cfgModels) {
			cfg_top = cfgModels.DesignContainer_parent();
		}
	}
}

void traversDesignSpaceForMorphMatrix(const CyPhyML::DesignSpace &ds, MorphMatrix& morphMatrix, set<CyPhyML::DesignEntity>& allEntities)
{
	const set<CyPhyML::DesignSpace> &dsfdrs = ds.DesignSpace_kind_children();
	for(set<CyPhyML::DesignSpace>::const_iterator di=dsfdrs.begin();di!=dsfdrs.end();++di)
	{
		traversDesignSpaceForMorphMatrix(*di, morphMatrix, allEntities);
	}

	const set<CyPhyML::DesignContainer> &containers = ds.DesignContainer_kind_children();
	for(set<CyPhyML::DesignContainer>::iterator ci=containers.begin();ci!=containers.end();++ci)
	{
		CyPhyML::DesignContainer container = *ci;
		traverseContainerForMorphMatrix(container, morphMatrix, allEntities);
	}

}

void traverseContainerForMorphMatrix(CyPhyML::DesignContainer &container, MorphMatrix& morphMatrix, set<CyPhyML::DesignEntity>& allEntities)
{
	set<CyPhyML::DesignEntity> entities = container.DesignEntity_kind_children();

	// First, populate the Morph Matrix
	set<CyPhyML::DesignEntity>* curEntities = 0;
	MorphMatrix::iterator pos;
	try {
		pos = morphMatrix.find(config);
	} catch (...) {
		// shouldn't normally occur
		pos = morphMatrix.end();
	}
	if(pos==morphMatrix.end()) {
		curEntities = new set<CyPhyML::DesignEntity>();
		morphMatrix.insert(MorphMatrix::value_type(config, std::unique_ptr<set<CyPhyML::DesignEntity> >(curEntities)));
	} else {
		curEntities = morphMatrix[config].get();
	}

	// Continue with configurations
	if((std::string)container.ContainerType()!="Compound")
	{
		map<int, int>::iterator pos = alternativeMap.find(container.ID());
		if(pos==alternativeMap.end())
			return;

		//get object by id
		int altId = (*pos).second;
		if(altId<=0) return;

		CyPhyML::DesignEntity selectedCom;
		for(set<CyPhyML::DesignEntity>::iterator i=entities.begin();i!=entities.end();++i)
		{
			if((*i).ID() == altId)
			{
				selectedCom = *i;
				break;
			}
		}
		if(selectedCom == Udm::null)
		{
			char buffer[10];
			_itoa(altId, buffer, 10);
			std::string err = "Cannot find the Component with ID: "+(std::string)buffer+" in DesignContainer: "+(std::string)container.name();
			throw udm_exception(err.c_str());
		}

		addSelectedEntity(allEntities, curEntities, selectedCom, container);

		//if(Uml::IsDerivedFrom(selectedCom.type(), CyPhyML::DesignContainer::meta))
		if((std::string)selectedCom.type().name()=="DesignContainer")
		{
			traverseContainerForMorphMatrix(CyPhyML::DesignContainer::Cast(selectedCom), morphMatrix, allEntities);
		}
	}
	else
	{
		for(set<CyPhyML::DesignEntity>::iterator i=entities.begin();i!=entities.end();++i)
		{
			const CyPhyML::DesignEntity &com = *i;
			addSelectedEntity(allEntities, curEntities, com, container);
			//if(Uml::IsDerivedFrom(com.type(), CyPhyML::DesignContainer::meta))
			if((std::string)com.type().name()=="DesignContainer")
			{
				traverseContainerForMorphMatrix(CyPhyML::DesignContainer::Cast(com), morphMatrix, allEntities);
			}
		}
	}
}

void generateMorphMatrixCfgsForGivenDesignSpace(RootFolder &cyphy_rf, DesertBackSystem &desert_top, Configuration &cfg, MorphMatrix& morphMatrix, set<CyPhyML::DesignEntity>& allEntities)
{
	config = cfg;
	generateMorphMatrixCfgsForDesignSpace(cyphy_rf, desert_top, morphMatrix, allEntities);
}
void generateMorphMatrixCfgsForDesignSpace(RootFolder &cyphy_rf, DesertBackSystem &desert_top, MorphMatrix& morphMatrix, set<CyPhyML::DesignEntity>& allEntities)
{
	Configuration cfg;
	CyPhyML::Configurations cfgModels;
	CyPhyML::DesignContainer cfg_top;
	initializeMorphMatrixCreation(cyphy_rf, cfgModels, desert_top, cfg, cfg_top);

	const set<DesignSpace> &dss = cyphy_rf.DesignSpace_kind_children();
	for(set<DesignSpace>::const_iterator di=dss.begin();di!=dss.end(); ++di)
	{
		traversDesignSpaceForMorphMatrix(*di, morphMatrix, allEntities);
	}

	clearAllMaps();
}
void generateMorphMatrixCfgsForGivenConfigs(RootFolder &cyphy_rf, CyPhyML::Configurations &cfgModels, DesertBackSystem &desert_top, Configuration &cfg, MorphMatrix& morphMatrix, set<CyPhyML::DesignEntity>& allEntities)
{
	CyPhyML::DesignContainer cfg_top;
	initializeMorphMatrixCreation(cyphy_rf, cfgModels, desert_top, cfg, cfg_top);

	if(cfg_top) {
		traverseContainerForMorphMatrix(cfg_top, morphMatrix, allEntities);
	}

	clearAllMaps();
}


#endif
