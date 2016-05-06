#ifndef CYPHY2DESERT_H
#define CYPHY2DESERT_H

#include "CyPhyML.h"
#include "DesertIface.h"
#include "CyPhyUtil.h"

typedef map<Udm::Object, map<CyPhyML::DesignEntity, DesertIface::VariableProperty> > VPMap;

class CyPhy2Desert
{
public: 
	CyPhy2Desert():groupCount(0){};
	CyPhy2Desert(DesertIface::DesertSystem &desert_top);
	~CyPhy2Desert();

	void generateDesert(const CyPhyML::RootFolder &cyphy_rf);
	void generateDesert(const CyPhyML::DesignSpace &cyphy_ds);
	void generateDesert(const CyPhyML::DesignContainer &cyphy_dc);
	void getCom2ElemMap(map<CyPhyML::DesignEntity, DesertIface::Element> &retMap);
	void getConstraintMap(map<DesertIface::Constraint, CyPhyML::ConstraintBase> &retMap);
	set<std::string> getNatualParameters();
	set<std::string> getCustomParameters();
	set<std::string> getGroupConstraits();
	set<std::string> getDefaultConstraits();
	map<int, CyPhyML::DesignEntity> getMorphMatrixGroup();
	map<int, CyPhyML::DesignEntity> getMorphMatrixOption();

	std::string generateVisualConstraint(const CyPhyML::DesignContainer &cyphy_container, const CyPhyML::VisualConstraint &vcon);
	static std::string generateParameterConstraint(const CyPhyML::Parameter &param, const std::string &newParamName);
	static std::string generateParameterConstraint(const std::string &range, const std::string &newParamName, CyPhyML::Parameter parameter);
	static std::string getFirstNumber(std::string &str);
	static std::string increaseCounter();
	static std::string getDesertPCM(const std::string &method);
	static DesertIface::ConstraintSet constraintSet;
	static DesertIface::FormulaSet formulaSet; 
	static set<CyPhyML::Parameter> invalidParameters;
	static set<CyPhyML::CustomFormula> invalidCustomFormulas;
	static set<CyPhyML::DecisionGroup> invalidGroups;
	static set<CyPhyML::ComponentRef> nullComponentRefs;
	static map<CyPhyML::DesignEntity, int> originalIDs;
	static map<Udm::Object, std::string> originalNames;
	static map<std::string, int> connectorDefMap;
	static map<std::string, set<int> > connectorDefWildCardMap;
	static set<int> getConnectorDefValue(std::string &def);
	static std::string errorMsg;
private:
	CyPhyML::DesignContainer rootDC;
	DesertIface::DesertSystem dsystem;
	map<CyPhyML::DesignEntity, DesertIface::Element> com2elemMap;
	map<DesertIface::Constraint, CyPhyML::ConstraintBase> constraintMap;	
	map<DesertIface::CustomDomain, map<std::string, DesertIface::CustomMember> > custMemberMap;

	map<std::string, DesertIface::Domain> domainMap;
	map<DesertIface::NaturalDomain, map<double, DesertIface::NaturalMember> > natureMemberMap; 
	static int memberId;
	static int domainId;
	set<std::string> naturalParameters;
	
	DesertIface::Space space;
		
	set<std::string> customParameters;
	set<std::string> defaultConstraints;
	int groupCount;

	map<int, CyPhyML::DesignEntity> mgroupMap;
	map<int, CyPhyML::DesignEntity> moptionMap;

	//int formulaCount;
	map<Udm::Object, map<CyPhyML::DesignEntity, DesertIface::VariableProperty> > vpMap;
	map<CyPhyML::SimpleFormula, map<CyPhyML::DesignEntity, DesertIface::SimpleFormula> > sfMap;

private:
	void init();
	void initStructuralInterfaceMap(const CyPhyML::RootFolder &rf);
	void initStructuralInterfaceMap(const CyPhyML::DesignSpace &ds);
	void initStructuralInterfaceMap(const CyPhyML::DesignContainer &dc);
	void initStructuralInterfaceMap(const CyPhyML::DesignElement &de);
	//void initSITWildCardMap();
	
	void initConnectorDefMap(const CyPhyML::RootFolder &rf);
	void initConnectorDefMap(const CyPhyML::DesignSpace &ds);
	void initConnectorDefMap(const CyPhyML::DesignContainer &dc);
	void initConnectorDefMap(const CyPhyML::DesignElement &de);
	void initConnectorDefWildCardMap();

	template <class T> void traverseDesignSpace(const CyPhyML::DesignSpace &ds, T &element_parent);
	template <class T> void traverseContainer(const CyPhyML::DesignEntity &cyphy_elem, T &element_parent, CyPhyML::DesignEntity& currentGroup, bool isAlt=false);

	void addGroup(CyPhyML::DesignContainer& currentGroup);
	
	void processConstraints(const CyPhyML::DesignContainer &cyphy_container, DesertIface::Element &desert_elem);
	std::string generateConstraint(const CyPhyML::ImpliesEnd &iend, const Udm::Object &container);
	std::string generateConstraint(const CyPhyML::And_Or &and_or, const Udm::Object &container);
	std::string generateConstraint(const CyPhyML::DesignEntityRef &elem_ref, const Udm::Object &container);
	std::string generateConstraint(const CyPhyML::NullOptionRef &null_ref, const Udm::Object &container);
	std::string generateConstraint(const CyPhyML::DesignEntity &entity, const Udm::Object &container);
	std::string generateConstraint(const CyPhyML::PropertyConstraintRef &pcon_ref, const Udm::Object &container);
	std::string generateConstraint(const CyPhyML::PropertyConstraint &pcon, const Udm::Object &container);
	std::string generateImplConstraint(Udm::Object &entity, const Udm::Object &container, const std::string &impl);
	void generateConstraint(map<std::string, list<CyPhyML::DesignEntity> > &groups, const Udm::Object &container, DesertIface::Element &desert_elem);
	bool generateConstraint(map<CyPhyML::DesignElement, list<CyPhyML::DesignEntity> > &groups, const Udm::Object &container, DesertIface::Element &desert_elem);
	void generateConstraint(const CyPhyML::Parameter &parameter, DesertIface::Element &delem);

	std::string getPath(const Udm::Object &obj, const Udm::Object &container);
	void processProperties(const set<CyPhyML::Property> &properties, const CyPhyML::DesignContainer &celem, DesertIface::Element &delem);
	void processParameters(const set<CyPhyML::Parameter> &parameters, const CyPhyML::DesignContainer &celem, DesertIface::Element &delem, bool isAlt);	
	void processProperty(const CyPhyML::DesignEntity &cyphy_com, DesertIface::Element &desert_elem,
		                 const std::string &pname, const std::string &value);
	void processProperty(const CyPhyML::DesignEntity &cyphy_com, DesertIface::Element &desert_elem, Udm::Object &prop,set<CyPhyML::ValueFlow> &src_vfs);
	void processConstants(const set<CyPhyML::Constant> &constants, const CyPhyML::DesignEntity &celem, DesertIface::Element &delem);
	
	void pre_processValueFormulas(const set<CyPhyML::ValueFormula> &formulas, const CyPhyML::DesignEntity &celem, DesertIface::Element &delem);
	void processSimpleFormula(const CyPhyML::SimpleFormula &sformula, DesertIface::Element &delem);
	void processCustomFormula(const CyPhyML::CustomFormula &cformula, DesertIface::Element &delem);

	void flatternComponent(const CyPhyML::DesignEntity &celem, DesertIface::Element &delem, bool isAlt);

	void processAlternativeValueFlowEnds();
	bool getDomainWithMember(const std::string &dname, DesertIface::CustomDomain &domain, 
		                     const std::string &value, DesertIface::CustomMember &member);		
	void processProperty(const CyPhyML::DesignEntity &cyphy_com, DesertIface::Element &desert_elem,
		                 const std::string &pname, double value, const std::string &compType, Udm::Object cyphyProp,bool assignValue=true);	
	DesertIface::CustomDomain generateCustomDomain(const std::string &dname);
	bool getDomainWithMember(const std::string &dname, DesertIface::NaturalDomain &domain, double value, DesertIface::NaturalMember &member);
	DesertIface::NaturalDomain generateNaturalDomain(DesertIface::DesertSystem dsys, const std::string &dname, _int64 min=0, _int64 max=1000000000);
	double getPropertyNumberValue(CyPhyML::ValueFlowTarget &vft);

	double getAdditionValue(vector<double> args);
	double getMultiplicationValue(vector<double> args);
	double getMaximumValue(vector<double> args);
	double getMinimumValue(vector<double> args);
	double getArithMeanValue(vector<double> args);
	double getGeometriMeanValue(vector<double> args);

	void gatherGroupBasedOnName(set<CyPhyML::DesignContainer> &containers, map<std::string, list<CyPhyML::DesignEntity> > &groups);
	void gatherGroupBasedOnArchType(set<CyPhyML::DesignContainer> &containers, map<std::string, list<CyPhyML::DesignEntity> > &groups);
	void gatherGroupBasedOnRef(set<CyPhyML::DesignContainer> &containers, map<CyPhyML::DesignElement, list<CyPhyML::DesignEntity> > &groups);

	std::string getRelativePath(Udm::Object &obj, const CyPhyML::DesignEntity &cyphy_elem);
	DesertIface::Element getDesertElement(const CyPhyML::DesignEntity &obj);
	bool isAlternativeContainer(Udm::Object &obj);
	bool isConstantProperty(set<CyPhyML::ValueFlow> &src_vfs);

	set<CyPhyML::ValueFlowTarget> alt_vfends;

	DesertIface::VariableProperty getVariableProperty(Udm::Object &cyphy_obj, CyPhyML::DesignEntity &obj_parent);
	void updatevpMap(const DesertIface::VariableProperty &dvp, const Udm::Object &cyphy_obj, const CyPhyML::DesignEntity &obj_parent);
	void checkSourceValueFlows(set<CyPhyML::ValueFlow> &vfs, const CyPhyML::DesignContainer &dc);
	void processAlternativeParameters(CyPhyML::DesignContainer &altDC);
	void insertParameterMap(const CyPhyML::Parameter &param, map<std::string, set<std::string> > &paramMap);
	void preprocessParameters(set<CyPhyML::Parameter> &parameters);
	void preprocessParameters(CyPhyML::DesignContainer &dc);
	void postprocessParameters();
	bool validateParameter(CyPhyML::DesignEntity &de, const std::string &pname);
	map<CyPhyML::Parameter, std::string> parameterNameMap;

	DesertIface::VariableProperty createDesertVP_PCMNONE(DesertIface::Element &delem, std::string name, double value);
	void generateConstraint(CyPhyUtil::ComponentPort &port1, CyPhyUtil::ComponentPort &port2, const CyPhyML::DesignEntity &celem, bool isdefault=true);
	void processStructuralInterfaces(const CyPhyML::DesignContainer &dc,DesertIface::Element &delem);
//	void processStructuralPort_compound(const CyPhyML::StructuralPortType &sport, set<CyPhyUtil::ComponentPort> &connectedSports, const CyPhyML::DesignContainer &dc, DesertIface::Element &delem);
	void processStructuralPort_compound(const Udm::Object &sport, set<CyPhyUtil::ComponentPort> &connectedSports, const CyPhyML::DesignContainer &dc, DesertIface::Element &delem);
//	void processStructuralPort_alternative(const CyPhyML::StructuralPortType &sport, set<CyPhyUtil::ComponentPort> &connectedSports, const CyPhyML::DesignContainer &dc, DesertIface::Element &delem);
	void processStructuralPort_alternative(const Udm::Object &sport, set<CyPhyUtil::ComponentPort> &connectedSports, const CyPhyML::DesignContainer &dc, DesertIface::Element &delem);

	void processConnectors(const CyPhyML::DesignContainer &dc,DesertIface::Element &delem);
	
	bool isAltPort(const CyPhyUtil::ComponentPort &port, const Uml::Class &objType, std::string srcRole, std::string dstRole);
	bool isConnectedToAltPort(const CyPhyUtil::ComponentPort &port, const CyPhyML::DesignContainer &dc, 
                              const Uml::Class &objType, std::string srcRoleName, std::string dstRoleName);
	bool checkDesignElementLoop(const CyPhyML::DesignEntity &entity, set<int> ids, std::string &path);
};

class Com2DesertElement
{
public:
	Com2DesertElement(const CyPhyML::DesignElement &cyphy_de, const CyPhyML::ComponentRef &comref, 
					 DesertIface::Element &delem, bool isroot, bool isAlt);
	void flatternCA();
	map<Udm::Object, DesertIface::VariableProperty> getTopVpMap();
	map<DesertIface::VariableProperty, double> getVp2ValMap();
	map<DesertIface::VariableProperty, set<double> > getVp2ValsMap();
	void clearVp2ValMap();
	void clearVp2ValsMap();

private:
	CyPhyML::DesignElement _com;
	CyPhyML::DesignEntity _comref;
	DesertIface::Element _delem;
	bool _isroot;
	bool _isAlt;

	static set<std::string> _vpNameSet;
	static int _cnt;
	
	map<Udm::Object, DesertIface::VariableProperty> _topVpMap; //for connecting valueflow and move up 
	VPMap _innerVpMap;  //for connecting valueflow
	set<CyPhyML::ValueFlow> _vfs;
	static map<DesertIface::VariableProperty, double> _vp2ValMap;  //for Domain 
	static map<DesertIface::VariableProperty, set<double> > _vp2ValsMap;  //for Domain 
	void init();
	DesertIface::VariableProperty createDesertVP(const Udm::Object &cyphy_vp, double value, bool parametric);
//	void createDesertVP4StructuralInterfaceWildCard(const Udm::Object &si, set<int> vals);
	void createDesertVP4WildCard(const Udm::Object &si, set<int> vals);
	void processProperty(const CyPhyML::Property &prop);
	void processParameter(const CyPhyML::Parameter &param);
	void processConstant(const CyPhyML::Constant &constant);
	void processFormula(const CyPhyML::ValueFormula &formula);  //if it connected to SimpleFormula, create DesertIface::VariableProperty
	void processSimpleFormula(const CyPhyML::SimpleFormula &sformula);
	void processCustomFormula(const CyPhyML::CustomFormula &cformula);
	void processDirectValueFlow(const CyPhyML::ValueFlow &vf);
	void updateInnerVpMap(const CyPhyML::DesignEntity &cyphy_elem, map<Udm::Object, DesertIface::VariableProperty> &topVpMap);
	DesertIface::VariableProperty getVariableProperty(CyPhyML::ValueFlowTarget &cyphy_obj, CyPhyML::DesignEntity &obj_parent);
	bool isVariableParametric(const CyPhyML::ValueFlowTarget &var, CyPhyML::ValueFlowTarget &src_end, Udm::Object &src_end_parent);
};

#endif //CYPHY2DESERT_H