#ifndef CYPHY2DESERT_CPP
#define CYPHY2DESERT_CPP

#include "uml.h"
#include <objbase.h>
#include "UmlExt.h"
#include "UdmUtil.h"
#include "CyPhy2Desert.h"
#include <cmath>
#include<math.h>
#include <stdio.h>

#include "DesertFormulaUtil.h"
#include "UdmGme.h"

#include <sstream>
#include <string.h>
#include <iterator>
#include <algorithm>
#include <regex>

using namespace CyPhyML;
using namespace DesertIface;

std::string checkInvalidName(std::string name)
{
	if (name.empty())
		return "";
	std::string badtoken = " ;.,#:'\"?<>-+=*&^$!";
	std::string converted;
	// Spaces and '-' are translated to '_'.
	for( std::string::const_iterator i= name.begin(); i!= name.end(); ++i)
	{
		if(badtoken.find(*i)!=std::string::npos)
		//if ( (*i== '-') || (*i==' '))
			converted+= '_';
		else
			converted+= *i;
	}
	return converted;
};

set<CyPhyML::DesignEntity> allOptions;
typedef map<CyPhyML::DesignEntity, std::unique_ptr<set<CyPhyML::DesignEntity> > > GroupOptions;
GroupOptions allGroupOptions;
const char* mmGroupsFilename = "groups_list.txt";
const char* mmOptionsFilename = "options_list.txt";
FILE* mmfd1 = 0;
FILE* mmfd2 = 0;
bool morphMatrixInitialized2 = false;

void morphMatrixInitialize2() {
	if(morphMatrixInitialized2)
		return;

	// MorphMatrix initialize
	allGroupOptions.clear();
	allOptions.clear();
	mmfd1 = fopen(mmGroupsFilename, "w+");
	mmfd2 = fopen(mmOptionsFilename, "w+");

	morphMatrixInitialized2 = true;
}

void morphMatrixFinalize2() {
	if(!morphMatrixInitialized2)
		return;

	morphMatrixInitialized2 = false;

	// MorphMatrix finalize
	for(GroupOptions::iterator mmPos = allGroupOptions.begin(); mmPos != allGroupOptions.end(); ++mmPos) {
		GroupOptions::value_type& mmRow = *mmPos;
		CyPhyML::DesignEntity group = mmRow.first;

		auto& groupOptions = mmRow.second;
		string groupName = group.name();
		long groupID = group.ID();
		CyPhyML::DesignContainer groupContainer = CyPhyML::DesignContainer::Cast(group);
		string groupType = groupContainer.ContainerType();
		fprintf(mmfd1, "%s\t%ld\t%s", groupName.c_str(), groupID, groupType.c_str());
		for(set<CyPhyML::DesignEntity>::iterator aePos = groupOptions->begin(); aePos != groupOptions->end(); ++aePos) {
			CyPhyML::DesignEntity e = *aePos;
			string optionName = e.name();
			long optionID = e.ID();
			fprintf(mmfd1, "\t%s\t%ld", optionName.c_str(), optionID);
		}
		fprintf(mmfd1, "\n");
	}
	fprintf(mmfd2, "Option_ID\tOption_Name\n");
	for(set<CyPhyML::DesignEntity>::iterator aePos = allOptions.begin(); aePos != allOptions.end(); ++aePos) {
		CyPhyML::DesignEntity e = *aePos;
		string eName = e.name();
		long eID = e.ID();
		fprintf(mmfd2, "%ld\t%s\n", eID, eName.c_str());
	}
	fflush(mmfd1);
	fflush(mmfd2);
	fclose(mmfd1);
	fclose(mmfd2);
	mmfd1 = 0;
	mmfd2 = 0;
	allGroupOptions.clear();
	allOptions.clear();
}

int CyPhy2Desert::domainId = 0;
DesertIface::ConstraintSet CyPhy2Desert::constraintSet;
DesertIface::FormulaSet CyPhy2Desert::formulaSet;
set<CyPhyML::Parameter> CyPhy2Desert::invalidParameters;
set<CyPhyML::CustomFormula> CyPhy2Desert::invalidCustomFormulas;
set<CyPhyML::DecisionGroup> CyPhy2Desert::invalidGroups;
set<CyPhyML::ComponentRef> CyPhy2Desert::nullComponentRefs;
map<CyPhyML::DesignEntity, int> CyPhy2Desert::originalIDs;
map<Udm::Object, std::string> CyPhy2Desert::originalNames;
map<std::string, int>  CyPhy2Desert::connectorDefMap;
map<std::string, set<int> >  CyPhy2Desert::connectorDefWildCardMap;
std::string CyPhy2Desert::errorMsg = "";

CyPhy2Desert::CyPhy2Desert(DesertIface::DesertSystem &desert_top) : dsystem(desert_top),groupCount(0)
{
//	morphMatrixInitialize2();
}

CyPhy2Desert::~CyPhy2Desert()
{
//	morphMatrixFinalize2();
}

void CyPhy2Desert::generateDesert(const CyPhyML::RootFolder &cyphy_rf)
{
	init();
//	initSITWildCardMap();

	initConnectorDefMap(cyphy_rf);
	initConnectorDefWildCardMap();

	const set<CyPhyML::DesignSpace> &dss = cyphy_rf.DesignSpace_kind_children();
	for(set<CyPhyML::DesignSpace>::const_iterator i=dss.begin();i!=dss.end();++i)
	{
		CyPhyML::DesignSpace ds = *i;
		traverseDesignSpace(ds, space);
	}

	postprocessParameters();
}

void CyPhy2Desert::generateDesert(const CyPhyML::DesignSpace &cyphy_ds)
{
	init();
//	initSITWildCardMap();

	initConnectorDefMap(cyphy_ds);
	initConnectorDefWildCardMap();

	traverseDesignSpace(cyphy_ds, space);
	postprocessParameters();
}

void CyPhy2Desert::generateDesert(const CyPhyML::DesignContainer &cyphy_dc)
{
	rootDC = cyphy_dc;
	set<int> ids;
	std::string path;
	if(!checkDesignElementLoop(rootDC, ids, path))
	{
		errorMsg = "The cyclic reference loop is found: <br>"+path;
		throw udm_exception("The cyclic reference loop is found. Please see the details in console window.");
	}
	init();

	initConnectorDefMap(cyphy_dc);
	initConnectorDefWildCardMap();


	traverseContainer(cyphy_dc, space, CyPhyML::DesignEntity::Cast(cyphy_dc));
	postprocessParameters();
}

void CyPhy2Desert::getCom2ElemMap(map<CyPhyML::DesignEntity, DesertIface::Element> &retMap)
{
	retMap = com2elemMap;
}

void CyPhy2Desert::getConstraintMap(map<DesertIface::Constraint, CyPhyML::ConstraintBase> &retMap)
{
	retMap = constraintMap;
}

_int64 string2int(const std::string &value)
{
	double num = atof(value.c_str());
	return (_int64)ceil(num);
}

void CyPhy2Desert::init()
{
	space = DesertIface::Space::Create(dsystem);
	space.name() = "DesignSpace";
	space.id() = 1000;
	space.externalID() = 1000;
	space.decomposition() = true;

	constraintSet = DesertIface::ConstraintSet::Create(dsystem);
	constraintSet.name() = "constraints";
	constraintSet.id() = 1003;
	constraintSet.externalID() = 1003;

	formulaSet = DesertIface::FormulaSet::Create(dsystem);
	formulaSet.name() = "formulaSet";
	formulaSet.id() = 1005;
	formulaSet.externalID() = 1005;

	domainId = 1004;

	domainMap.clear();
	parameterNameMap.clear();
	invalidParameters.clear();
	invalidCustomFormulas.clear();
	nullComponentRefs.clear();
	invalidGroups.clear();
	originalIDs.clear();
	//structuralInterfaceTypeMap.clear();
	//sitWildCardMap.clear();
	//structuralInterfaceTypeMap["88888888"] = 0;
	originalNames.clear();

}


//void CyPhy2Desert::initSITWildCardMap()
//{
//	 map<std::string, set<int> > tmp_sitWildCardMap;
//	for(auto pos=sitWildCardMap.begin(); pos!=sitWildCardMap.end(); ++pos)
//	{
//		std::string si_wc = (*pos).first;
//		set<int> vals = (*pos).second;
//		int length = si_wc.size();
//		for(auto pos1=structuralInterfaceTypeMap.begin(); pos1!=structuralInterfaceTypeMap.end(); ++pos1)
//		{
//			std::string this_si = (*pos1).first;
//			if(this_si.size()!=length)
//				continue;
//			bool ismatch = true;
//			for(std::string::size_type i = 0; i<length; ++i)
//			{
//				if(si_wc[i]=='*')
//					continue;
//				else if(si_wc[i]!=this_si[i])
//				{
//					ismatch = false;
//					break;
//				}
//			}
//
//			if(ismatch)
//				vals.insert((*pos1).second);
//		}
//		if(!vals.empty())
//			tmp_sitWildCardMap[si_wc] = vals;
//		else
//		{
//			int si_val = structuralInterfaceTypeMap.size()+1;
//			structuralInterfaceTypeMap[si_wc] = si_val;
//		}
//	}
//
//	sitWildCardMap.clear();
//	sitWildCardMap.insert(tmp_sitWildCardMap.begin(), tmp_sitWildCardMap.end());
//}

void CyPhy2Desert::initConnectorDefMap(const CyPhyML::RootFolder &rf)
{
	const set<CyPhyML::DesignSpace> &dsfdrs = rf.DesignSpace_kind_children();
	for(set<CyPhyML::DesignSpace>::const_iterator di=dsfdrs.begin();di!=dsfdrs.end();++di)
	{
		initConnectorDefMap(*di);
	}
}

void CyPhy2Desert::initConnectorDefMap(const CyPhyML::DesignSpace &ds)
{
	const set<CyPhyML::DesignSpace> &dsfdrs = ds.DesignSpace_kind_children();
	for(set<CyPhyML::DesignSpace>::const_iterator di=dsfdrs.begin();di!=dsfdrs.end();++di)
	{
		initConnectorDefMap(*di);
	}

	const set<CyPhyML::DesignContainer> &containers = ds.DesignContainer_kind_children();
	for(set<CyPhyML::DesignContainer>::const_iterator ci=containers.begin();ci!=containers.end();++ci)
	{
		initConnectorDefMap(*ci);
	}
}

void CyPhy2Desert::initConnectorDefMap(const CyPhyML::DesignContainer &dc)
{
	set<CyPhyML::DesignContainer> dcs = dc.DesignContainer_kind_children();
	for(auto it_dc=dcs.begin(); it_dc!=dcs.end(); ++it_dc)
		initConnectorDefMap(*it_dc);

	set<CyPhyML::ComponentRef> comrefs = dc.ComponentRef_kind_children();
	for(auto it_ref=comrefs.begin(); it_ref!=comrefs.end(); ++it_ref)
	{
		CyPhyML::ComponentRef comref = *it_ref;
		CyPhyML::DesignElement de = comref.ref();
		if(de!=Udm::null)
			initConnectorDefMap(de);
		else
		{
			nullComponentRefs.insert(comref);
		}
	}

	set<CyPhyML::ComponentAssembly> cas = dc.ComponentAssembly_kind_children();
	for(auto it_ca=cas.begin(); it_ca!=cas.end(); ++it_ca)
		initConnectorDefMap(*it_ca);

	set<CyPhyML::Component> coms = dc.Component_kind_children();
	for(auto it_com=coms.begin(); it_com!=coms.end(); ++it_com)
		initConnectorDefMap(*it_com);
}

void CyPhy2Desert::initConnectorDefMap(const CyPhyML::DesignElement &de)
{
	set<CyPhyML::Connector> connectors = de.Connector_kind_children();
	for(auto it_si=connectors.begin(); it_si!=connectors.end(); ++it_si)
	{
		CyPhyML::Connector si = *it_si;
		std::string def = si.Definition();
		if(def.find("*")==std::string::npos) //non-wildcard
		{
			map<std::string, int>::iterator map_pos = connectorDefMap.find(def);
			if(map_pos == connectorDefMap.end())
			{
				int def_val = connectorDefMap.size()+1;
				connectorDefMap[def] = def_val;
			}
		}
		else  //wildcard
		{
			map<std::string, set<int> >::iterator map_pos1 = connectorDefWildCardMap.find(def);
			if(map_pos1 == connectorDefWildCardMap.end())
			{
				set<int> tmp;
				connectorDefWildCardMap[def] = tmp;
			}
		}
	}

	if(Uml::IsDerivedFrom(de.type(), CyPhyML::ComponentAssembly::meta))
	{
		CyPhyML::ComponentAssembly ca = CyPhyML::ComponentAssembly::Cast(de);
		set<CyPhyML::DesignElement> des = ca.DesignElement_kind_children();
		for(auto it_de = des.begin();it_de!=des.end();++it_de)
		{
			initConnectorDefMap(*it_de);
		}
		set<CyPhyML::ComponentRef> comrefs = ca.ComponentRef_kind_children();
		for(auto it_ref=comrefs.begin();it_ref!=comrefs.end();++it_ref)
		{
			const CyPhyML::DesignElement ref_de = (*it_ref).ref();
			if(ref_de!=Udm::null)
				initConnectorDefMap(ref_de);
		}
	}
}

void CyPhy2Desert::initConnectorDefWildCardMap()
{
	 map<std::string, set<int> > tmp_defWildCardMap;
	for(auto pos=connectorDefWildCardMap.begin(); pos!=connectorDefWildCardMap.end(); ++pos)
	{
		std::string si_wc = (*pos).first;
		set<int> vals = (*pos).second;
		std::string::size_type length = si_wc.size();
		for(auto pos1=connectorDefMap.begin(); pos1!=connectorDefMap.end(); ++pos1)
		{
			std::string this_si = (*pos1).first;
			if(this_si.size()!=length)
				continue;
			bool ismatch = true;
			for(std::string::size_type i = 0; i<length; ++i)
			{
				if(si_wc[i]=='*')
					continue;
				else if(si_wc[i]!=this_si[i])
				{
					ismatch = false;
					break;
				}
			}

			if(ismatch)
				vals.insert((*pos1).second);
		}
		if(!vals.empty())
			tmp_defWildCardMap[si_wc] = vals;
		else
		{
			int si_val = connectorDefMap.size()+1;
			connectorDefMap[si_wc] = si_val;
		}
	}

	connectorDefWildCardMap.clear();
	connectorDefWildCardMap.insert(tmp_defWildCardMap.begin(), tmp_defWildCardMap.end());
}

DesertIface::NaturalDomain CyPhy2Desert::generateNaturalDomain(DesertIface::DesertSystem dsys, const std::string &dname, _int64 min, _int64 max)
{
	domainId++;
	DesertIface::NaturalDomain ndomain = DesertIface::NaturalDomain::Create(dsys);
	ndomain.name() = dname;
	ndomain.id() = ndomain.externalID() = domainId;
	ndomain.minimum() = min;
	ndomain.maximum() = max;
	domainMap[dname] = ndomain;
	naturalParameters.insert(dname);
	return ndomain;
}

DesertIface::CustomDomain CyPhy2Desert::generateCustomDomain(const std::string &dname)
{
	domainId++;
	DesertIface::CustomDomain cdomain = DesertIface::CustomDomain::Create(dsystem);
	cdomain.name() = dname+"s";
	cdomain.id() = cdomain.externalID() = domainId;
	cdomain.decomposition() = true;
	domainMap[dname] = cdomain;
	customParameters.insert(dname);
	return cdomain;
}

template <class T> void CyPhy2Desert::traverseDesignSpace(const CyPhyML::DesignSpace &ds, T &element_parent)
{
	Element element = Element::Create(element_parent);
	element.name() = ds.name();
	element.id() = element.externalID() = ds.uniqueId();
	element.decomposition() = true;

	const set<CyPhyML::DesignSpace> &dsfdrs = ds.DesignSpace_kind_children();
	for(set<CyPhyML::DesignSpace>::const_iterator di=dsfdrs.begin();di!=dsfdrs.end();++di)
	{
		traverseDesignSpace(*di, element);
		DOEVENTS();
	}

	const set<CyPhyML::DesignContainer> &containers = ds.DesignContainer_kind_children();
	if(containers.empty()) return;
//	rootContainer = *(containers.begin());
	for(set<CyPhyML::DesignContainer>::const_iterator ci=containers.begin();ci!=containers.end();++ci)
	{
		rootDC = *ci;
		set<int> ids;
		std::string path;
		if(!checkDesignElementLoop(rootDC, ids, path))
			throw udm_exception("The cyclic reference loop is found: \\n"+path);
		traverseContainer(*ci, element, CyPhyML::DesignEntity::Cast(*ci));
		DOEVENTS();
	}
}

void CyPhy2Desert::addGroup(CyPhyML::DesignContainer& currentGroup)
{
	CyPhyML::DesignContainer groupBaseType = currentGroup;
	if(currentGroup.isInstance() || currentGroup.isSubtype()) {
		groupBaseType = currentGroup.Archetype();
		while(groupBaseType.isInstance() || groupBaseType.isSubtype()) {
			groupBaseType =groupBaseType.Archetype();
		}
	}
	allOptions.insert(groupBaseType);

	GroupOptions::iterator goItr = allGroupOptions.find(groupBaseType);
	if(goItr == allGroupOptions.end()) {
		// New group
		auto groupOptions = new set<CyPhyML::DesignEntity>();
		allGroupOptions.emplace(groupBaseType, unique_ptr<set<CyPhyML::DesignEntity> >(groupOptions));

		set<CyPhyML::DesignEntity> groupChildren = groupBaseType.DesignEntity_kind_children();
		for(set<CyPhyML::DesignEntity>::iterator it = groupChildren.begin(); it != groupChildren.end(); ++it) {
			CyPhyML::DesignEntity aChild = *it;
			allOptions.insert(aChild);
			groupOptions->insert(aChild);

			mgroupMap[aChild.ID()] = groupBaseType;
			moptionMap[aChild.ID()] = aChild;
		}

		moptionMap[groupBaseType.ID()] = groupBaseType;
		mgroupMap[groupBaseType.ID()] = groupBaseType;
	}
}

void CyPhy2Desert::processConstants(const set<CyPhyML::Constant> &constants, const CyPhyML::DesignEntity &celem, DesertIface::Element &delem)
{
	for(set<CyPhyML::Constant>::iterator i=constants.begin();i!=constants.end();++i)
	{
		CyPhyML::Constant constant = *i;

		std::string cname = constant.name();
		double value = constant.ConstantValue();

		DesertIface::NaturalDomain domain;
		DesertIface::NaturalMember member;
		if(!getDomainWithMember(cname, domain, value, member))
			throw udm_exception("Constant: "+cname+" is not NaturalDomain.");
		DesertIface::VariableProperty vp = DesertIface::VariableProperty::Create(delem);
		updatevpMap(vp, constant, celem);
		vp.parametric() = false;
		vp.name() = vp.CUSTName() = cname;
		vp.id() = vp.externalID() = celem.ID()+5000;
		vp.PCM_STR() = "PCM_NONE";
		vp.Max() = 100000;
		vp.domain() =domain;

		DesertIface::AssignedValues av = DesertIface::AssignedValues::Create(delem);
		av.id() = av.externalID() = 2000;
		av.properties_end() = vp;
		av.values_end() = member;
	}
}

void CyPhy2Desert::processProperties(const set<CyPhyML::Property> &properties, const CyPhyML::DesignContainer &celem,
									DesertIface::Element &delem, set<CyPhyML::ValueFlowTarget>& alt_vfends)
{
	for(set<CyPhyML::Property>::iterator i=properties.begin();i!=properties.end();++i)
	{
		CyPhyML::Property propty = *i;
		if(propty.Disable()) continue;
		std::string pname = propty.name();
		set<CyPhyML::ValueFlow> src_vfs = propty.srcValueFlow();

		checkSourceValueFlows(src_vfs, celem);

		if(!src_vfs.empty())  //this property is parametric
		{
			processProperty(celem, delem, propty, src_vfs, alt_vfends);
		}  //for non-parametric property
		else
		{
			if((std::string)propty.ComputationType()!="Cust")
			{
				double value = getPropertyNumberValue(propty);
				processProperty(celem,delem,pname,value,propty.ComputationType(),propty,(src_vfs.empty()||Uml::IsDerivedFrom(celem.type(), CyPhyML::ComponentAssembly::meta)));
			}
		}
	}
}

void CyPhy2Desert::processParameters(const set<CyPhyML::Parameter> &parameters, const CyPhyML::DesignContainer &celem, DesertIface::Element &delem, bool isAlt, set<CyPhyML::ValueFlowTarget>& alt_vfends)
{
	for(set<CyPhyML::Parameter>::iterator pi=parameters.begin();pi!=parameters.end();++pi)
	{
		CyPhyML::Parameter parameter = *pi;

		std::string pname = parameter.name();

		if(!parameter.Disable() && !isAlt)
			generateConstraint(parameter, delem);

		set<CyPhyML::ValueFlow> src_vfs = parameter.srcValueFlow();

		checkSourceValueFlows(src_vfs, celem);

		if(!src_vfs.empty())
		{
			processProperty(celem, delem, parameter, src_vfs, alt_vfends);
		}
		else
		{
			if((std::string)parameter.ComputationType()!="Cust")
			{
				double value = getPropertyNumberValue(parameter);
				processProperty(celem,delem,pname,value,parameter.ComputationType(),parameter,(src_vfs.empty()||Uml::IsDerivedFrom(celem.type(), CyPhyML::ComponentAssembly::meta)));
			}
		}
	}
}

std::string CyPhy2Desert::generateParameterConstraint(const CyPhyML::Parameter &parameter, const std::string &newParamName)
{
	std::string range = parameter.Range();
	return generateParameterConstraint(range, newParamName, parameter);
}

std::string CyPhy2Desert::generateParameterConstraint(const std::string &range, const std::string &newParamName, CyPhyML::Parameter parameter)
{
/*
Syntax: 0..2
or -inf..inf
or -inf..2
Ranges are inclusive
*/
	if(range.empty())
		return "";

	size_t not_pos = range.find_first_not_of(" [(-0123456789infINF]).,");
	if(not_pos!=std::string::npos)
	{
		if (parameter) {
			invalidParameters.insert(parameter);
		}
		return "";
	}

	if(range.find("-inf..inf")!=std::string::npos)
		return "";
	if(range.find("-inf,inf")!=std::string::npos) // compatibility
		return "";

	std::string cleanRange;
	std::copy_if(range.begin(), range.end(), std::back_inserter(cleanRange), [](std::string::value_type ch) { return strchr("[]() ", ch) == NULL; }); // compatibility
	cleanRange = std::regex_replace(cleanRange, std::regex(","), std::string(".."));

	size_t dots;
	dots = cleanRange.rfind("..");
	if (dots == std::string::npos)
	{
		if (parameter) {
			invalidParameters.insert(parameter);
		}
		return "";
	}
	std::string lower;
	if (dots > 0) {
		lower = cleanRange.substr(0, dots);
	}
	std::string upper = cleanRange.substr(dots + 2);

	std::string expr;
	expr = "(";
	if (lower.empty() == false && lower != "-inf")
	{
		expr = expr + "(" + newParamName + "() >= " + lower + ")";
	}
	if (upper.empty() == false && upper != "inf")
	{
		if (expr != "(")
			expr += " and ";
		expr = expr + "(" + newParamName + "() <= " + upper + ")";
	}
	expr += ")";

	if (expr == "()") {
		expr.clear();
	}

	return expr;
}

void CyPhy2Desert::generateConstraint(const CyPhyML::Parameter &parameter, DesertIface::Element &delem)
{
	std::string expr = generateParameterConstraint(parameter, parameter.name());
	if(expr=="()")
		return;

	if(expr.empty()) return;

	std::string pname = parameter.name();

	DesertIface::Constraint dsConstraint = DesertIface::Constraint::Create(constraintSet);
	//int pid = parameter.uniqueId();
	//char buffer[65];
	//_itoa(pid, buffer, 10);
	std::string pcon_name = pname+"_"+increaseCounter()+"_constraint";
	dsConstraint.name() = pcon_name;
	dsConstraint.expression() = "constraint "+ pcon_name+"() {\n" + expr+"\n}";
	dsConstraint.context() = delem;

	return;
}

std::string CyPhy2Desert::getFirstNumber(std::string &str)
{
	std::string ret;
	string tokens = "[(-0123456789infINF])";
	size_t pos = str.find_first_of(tokens);
	str.erase(0, pos);
	pos = 0;
	size_t end_pos = str.length();
	size_t pos2 = str.find("..");
	if(pos2!=std::string::npos)
		end_pos = pos2;
	else
	{
		pos2 = str.find_first_of(",");
		if(pos2!=std::string::npos)
			end_pos = pos2;
	}

	ret = str.substr(pos, end_pos-pos);
	str.erase(0, end_pos);

	return ret;
}

void CyPhy2Desert::processAlternativeValueFlowEnds(const set<CyPhyML::ValueFlowTarget>& alt_vfends) {
	map<CyPhyML::DesignEntity, DesertIface::Element>::iterator pos;
	for(set<CyPhyML::ValueFlowTarget>::iterator it=alt_vfends.begin();it!=alt_vfends.end();++it)
	{
		CyPhyML::ValueFlowTarget vfend = *it;
		if(Uml::IsDerivedFrom(vfend.type(), CyPhyML::ValueFormula::meta))
			continue;

		set<CyPhyML::ValueFlow> src_vfs = vfend.srcValueFlow();
		if(src_vfs.empty())
			continue;

		Udm::Object vfend_owner =vfend.GetParent();
		ASSERT(Uml::IsDerivedFrom(vfend_owner.type(), CyPhyML::DesignEntity::meta));
		CyPhyML::DesignEntity celem = CyPhyML::DesignEntity::Cast(vfend_owner);

		DesertIface::Element delem;
		pos = com2elemMap.find(celem);
		if(pos!=com2elemMap.end())
			delem = (*pos).second;
		else
			continue;

		DesertIface::SimpleFormula dsf = DesertIface::SimpleFormula::Create(formulaSet);
		dsf.ComputationType() = "PCM_OR";
		dsf.context() = delem;
		dsf.name() = "SimpleFormula";
		dsf.id() = dsf.externalID() = vfend.uniqueId();

		DesertIface::VariableProperty dvp = getVariableProperty(vfend,celem);
		if(dvp == Udm::null)
		{
			dvp = DesertIface::VariableProperty::Create(delem);
			updatevpMap(dvp, vfend, celem);
			dvp.name() = dvp.CUSTName() = UdmUtil::ExtractName(vfend);
			dvp.id() = dvp.externalID() = celem.ID()+5000;
		}
		dvp.parametric() = true;
		dvp.PCM_STR() = "";
		dsf.property_end() += dvp;

		for(set<CyPhyML::ValueFlow>::iterator it_vf=src_vfs.begin();it_vf!=src_vfs.end();++it_vf)
		{
			CyPhyML::ValueFlowTarget src_end = (*it_vf).srcValueFlow_end();
			if (Uml::IsDerivedFrom(src_end.type(), CyPhyML::Parameter::meta)
				|| Uml::IsDerivedFrom(src_end.type(), CyPhyML::Property::meta)
				|| Uml::IsDerivedFrom(src_end.type(), CyPhyML::Constant::meta)
				)
			{
				Udm::Object src_end_parent = (*it_vf).srcValueFlow_refport_parent();
				if(src_end_parent==Udm::null)
					src_end_parent = src_end.GetParent();

				if(!Uml::IsDerivedFrom(src_end_parent.type(), CyPhyML::DesignEntity::meta))
					continue;

				CyPhyML::DesignEntity src_parent = CyPhyML::DesignEntity::Cast(src_end_parent);
				DesertIface::VariableProperty src_dvp = getVariableProperty(src_end, src_parent);
				// std::string src_parent_name = src_parent.name();
				// std::string src_end_name = src_end.name();
				if(src_dvp==Udm::null)
				{
					DesertIface::Element vpParent;
					pos = com2elemMap.find(src_parent);
					if(pos!=com2elemMap.end())
						vpParent = (*pos).second;
					else
						continue;

					src_dvp = DesertIface::VariableProperty::Create(vpParent);
					updatevpMap(src_dvp, src_end, src_parent);
					int uid = src_end.uniqueId();
				//	char buffer[65];
				//	_itoa(uid, buffer, 10);
					src_dvp.id() = src_dvp.externalID() = src_end.uniqueId()+5000;
				}
				dsf.srcProperty() += src_dvp;
			}
		}
	}
}

DesertIface::Element CyPhy2Desert::getDesertElement(const CyPhyML::DesignEntity &obj)
{
	DesertIface::Element ret;
	map<CyPhyML::DesignEntity, DesertIface::Element>::iterator pos = com2elemMap.find(obj);
	if(pos!=com2elemMap.end())
		ret = (*pos).second;

	return ret;
}

template <class T> void CyPhy2Desert::traverseContainer(const CyPhyML::DesignEntity &cyphy_elem, T &element_parent, CyPhyML::DesignEntity& currentGroup,bool isAlt)
{
	DOEVENTS();
	std::string tmp = cyphy_elem.getPath2();
	addGroup(CyPhyML::DesignContainer::Cast(currentGroup));

	Element element = Element::Create(element_parent);
	com2elemMap[cyphy_elem] = element;
	element.name() = cyphy_elem.name();

	originalIDs[cyphy_elem] = (int)cyphy_elem.ID();  //reverse it back later
	cyphy_elem.ID() = cyphy_elem.uniqueId();  //using uniqueID to avoid negative integer(maybe caused by overflow)
	int elemID = (int)cyphy_elem.ID();

	element.id() = element.externalID() = elemID;

	if(!Uml::IsDerivedFrom(cyphy_elem.type(), DesignContainer::meta))
	{
		flatternComponent(cyphy_elem, element, isAlt);
	}
	else  //DesignContainer
	{
		DesignContainer container = DesignContainer::Cast(cyphy_elem);
		set<CyPhyML::ValueFlowTarget> alt_vfends;

		// First make sure that the design container contains at least 1 design element in it (as per the designed/supported use-case)
		set<DesignEntity> childrenDesignEntities = container.DesignEntity_kind_children();
		if(childrenDesignEntities.empty()) {
			int errConId = container.uniqueId();
			std::stringstream ss;
			ss << "The design container doesn't contain any design element in it:" << std::endl;
			ss << "Name: " << container.name() << std::endl;
			ss << "ID: " << UdmGme::UdmId2GmeId(errConId) << std::endl;
			throw udm_exception(ss.str());
		}

		preprocessParameters(container);

		bool dcAlt = ((std::string)container.ContainerType() != "Compound");

		bool procSF = true;

		const set<CyPhyML::Property> properties = container.Property_kind_children();
		processProperties(properties, container, element, alt_vfends);

		set<CyPhyML::Parameter> parameters = container.Parameter_kind_children();
		processParameters(parameters, container, element, false, alt_vfends);

		const set<CyPhyML::Constant> constants = container.Constant_kind_children();
		processConstants(constants, container, element);

		//const set<CyPhyML::SimpleFormula> sfs = container.SimpleFormula_kind_children();
		//processSimpleFormulas(sfs, container, element);
		const set<CyPhyML::ValueFormula> formulas = container.ValueFormula_kind_children();
		pre_processValueFormulas(formulas, container, element);

		if(!dcAlt)
			element.decomposition() = true;

		else
		{
			set<CyPhyML::ValueFlowTarget> alt_vfends;

			element.decomposition() = false;
			if(container.ContainerType() == "Optional")
			{
				Element nullelem = Element::Create(element);
				nullelem.name() = "null";
				//nullelem.id() = 0;
				nullelem.id() = nullelem.externalID() = -1*container.ID();
				//processProperties
				{
					for(set<CyPhyML::Property>::iterator nit=properties.begin();nit!=properties.end();++nit)
					{
						CyPhyML::Property propty = *nit;
						set<CyPhyML::ValueFlow> dstvfs = propty.dstValueFlow();
						if(dstvfs.empty()) continue;
						CyPhyML::SimpleFormula sf;
						for(set<CyPhyML::ValueFlow>::iterator vf_it=dstvfs.begin();vf_it!=dstvfs.end();++vf_it)
						{
							CyPhyML::ValueFlowTarget vftarget = (*vf_it).dstValueFlow_end();
							if(Uml::IsDerivedFrom(vftarget.type(), CyPhyML::SimpleFormula::meta))
							{
								sf = CyPhyML::SimpleFormula::Cast(vftarget);
								break;
							}
						}
						double nullVal = 0;
						if(sf!=Udm::null)
						{
							std::string method = sf.Method();
							if(method=="Multiplication")
								nullVal = 1;
							else if(method=="Maximum")
								nullVal = 0;
							else if(method=="Minimum")
								nullVal = 214748000;
							else if(method=="GeometricMean")
								nullVal = 1;
						}
						std::string pname = propty.name();
						DesertIface::NaturalDomain domain;
						DesertIface::NaturalMember member;
						getDomainWithMember(pname, domain, nullVal, member);
						DesertIface::VariableProperty vp = DesertIface::VariableProperty::Create(nullelem);
						vp.parametric() = false;
						vp.name() = vp.CUSTName() = pname;
						vp.id() = vp.externalID() = container.ID()+5100;
						vp.PCM_STR() = "PCM_NONE";
						vp.domain() =domain;
						vp.Max() = 214748000;
						DesertIface::AssignedValues av = DesertIface::AssignedValues::Create(nullelem);
						av.id() = av.externalID() = 2000;
						av.properties_end() = vp;
						av.values_end() = member;
					}
				}
			}
		}
		processConstraints(container, element);

		// Update Morph Matrix
		// Add this container as an option to current group (if there is one)
		// allOptions.insert(container);
//		addGroup(CyPhyML::DesignContainer::Cast(currentGroup));

		// Add this container as a new group (if not already there) before iterating over its children
		addGroup(container);

		set<CyPhyML::DesignEntity> entities = container.DesignEntity_kind_children();
		for(set<CyPhyML::DesignEntity>::iterator i=entities.begin();i!=entities.end();++i)
			 traverseContainer(*i, element, container,dcAlt);

		processAlternativeValueFlowEnds(alt_vfends);

		for(auto it_f=formulas.begin();it_f!=formulas.end();++it_f)
		{
			CyPhyML::ValueFormula formula = *it_f;
			//processSimpleFormulas
			if(Uml::IsDerivedFrom(formula.type(), CyPhyML::SimpleFormula::meta))
				processSimpleFormula(CyPhyML::SimpleFormula::Cast(formula), element);
			else if(Uml::IsDerivedFrom(formula.type(), CyPhyML::CustomFormula::meta))
				processCustomFormula(CyPhyML::CustomFormula::Cast(formula), element, isAlt);
		}

		processConnectors(container, element);

		if(dcAlt)
			processAlternativeParameters(container);
	}
}


void CyPhy2Desert::processConnectors(const CyPhyML::DesignContainer &dc,DesertIface::Element &delem)
{
	bool isAlt = ((std::string)dc.ContainerType() != "Compound");

	set<CyPhyML::ConnectorComposition> ccs = dc.ConnectorComposition_kind_children();
	set<CyPhyML::Connector> connectors = dc.Connector_kind_children();
	set<std::string> uniqueConnectorNames;
	for(auto it_sp=connectors.begin();it_sp!=connectors.end();++it_sp)
	{
		CyPhyML::Connector connector = *it_sp;
		std::string c_name = connector.name();
		std::string c_valid_name = checkInvalidName(c_name);
		if(uniqueConnectorNames.find(c_valid_name)!=uniqueConnectorNames.end())
		{
			int cnt = uniqueConnectorNames.size();
			char buffer[65];
			_itoa_s(cnt, buffer, 10);
			c_valid_name = c_valid_name+"_"+(std::string)buffer;
		}
		uniqueConnectorNames.insert(c_valid_name);
		if(c_valid_name!=c_name)
		{
			connector.name() = c_valid_name;
			CyPhy2Desert::originalNames[connector] = c_name;
		}

		set<CyPhyUtil::ComponentPort> connectedPorts;
		set<CyPhyML::ConnectorComposition> src_ccs =connector.srcConnectorComposition();
		for(auto it_src=src_ccs.begin();it_src!=src_ccs.end();++it_src)
		{
			CyPhyML::ConnectorComposition src_cc = *it_src;
			if(ccs.find(src_cc)!=ccs.end())
			{
				CyPhyUtil::ComponentPort src_port;
				src_port.port = src_cc.srcConnectorComposition_end();
				src_port.port_ref_parent = src_cc.srcConnectorComposition_refport_parent();
				connectedPorts.insert(src_port);
				ccs.erase(src_cc);
			}
		}
		set<CyPhyML::ConnectorComposition> dst_ccs = connector.dstConnectorComposition();
		for(auto it_dst=dst_ccs.begin();it_dst!=dst_ccs.end();++it_dst)
		{
			CyPhyML::ConnectorComposition dst_cc = *it_dst;
			if(ccs.find(dst_cc)!=ccs.end())
			{
				CyPhyUtil::ComponentPort dst_port;
				dst_port.port = dst_cc.dstConnectorComposition_end();
				dst_port.port_ref_parent = dst_cc.dstConnectorComposition_refport_parent();
				connectedPorts.insert(dst_port);
				ccs.erase(dst_cc);
			}
		}
	}

	if(isAlt) return;

	for(auto it_js=ccs.begin();it_js!=ccs.end();++it_js)
	{
		CyPhyML::ConnectorComposition js = *it_js;
		CyPhyUtil::ComponentPort src_port, dst_port;
		src_port.port = js.srcConnectorComposition_end();
		src_port.port_ref_parent = js.srcConnectorComposition_refport_parent();
		dst_port.port = js.dstConnectorComposition_end();
		dst_port.port_ref_parent = js.dstConnectorComposition_refport_parent();
		Udm::Object src_port_parent = src_port.port.GetParent();

		if(isAltPort(src_port, CyPhyML::ConnectorComposition::meta, "srcConnectorComposition", "dstConnectorComposition")
			|| isAltPort(dst_port, CyPhyML::ConnectorComposition::meta, "srcConnectorComposition", "dstConnectorComposition"))
			generateConstraint(src_port, dst_port, dc);
	}
}

bool CyPhy2Desert::isAltPort(const CyPhyUtil::ComponentPort &port, const Uml::Class &objType, std::string srcRoleName, std::string dstRoleName)
{
	Udm::Object port_parent;
	if(port.port_ref_parent)
	{
		if(Uml::IsDerivedFrom(port.port_ref_parent.type(), CyPhyML::ComponentRef::meta))
			port_parent = CyPhyML::ComponentRef::Cast(port.port_ref_parent).ref();
	}
	else
		port_parent = port.port.GetParent();

	if(!port_parent)
		return false;

	if(Uml::IsDerivedFrom(port_parent.type(), CyPhyML::DesignElement::meta))
		return false;

	if(Uml::IsDerivedFrom(port_parent.type(), CyPhyML::DesignContainer::meta))
	{
		CyPhyML::DesignContainer dc = CyPhyML::DesignContainer::Cast(port_parent);
		if((std::string)dc.ContainerType()!="Compound")
			return true;

		Udm::Object::AssociationInfo assocInfo1(objType);
		assocInfo1.strSrcRoleName = srcRoleName;
		assocInfo1.strDstRoleName = dstRoleName;

		set<Udm::Object> assocs = Udm::Object(port.port).GetAssociationClassObjects(NULL, assocInfo1);
		assocInfo1.strSrcRoleName = dstRoleName;
		assocInfo1.strDstRoleName = srcRoleName;
		set<Udm::Object> assocs1 = Udm::Object(port.port).GetAssociationClassObjects(NULL, assocInfo1);
		assocs.insert(assocs1.begin(), assocs1.end());

		for(auto it=assocs.begin();it!=assocs.end();++it)
		{
			Udm::Object assoc = *it;
			Udm::Object assoc_parent = assoc.GetParent();
			if(assoc_parent!=dc)
				continue;
			CyPhyUtil::ComponentPortPair endpair = CyPhyUtil::getConnectionEnds(objType, assoc);
			CyPhyUtil::ComponentPort peer_port = endpair.src == port? endpair.dst : endpair.src;
			if(isAltPort(peer_port, objType, srcRoleName, dstRoleName))
				return true;
		}
	}
	return false;
}

bool CyPhy2Desert::isConnectedToAltPort(const CyPhyUtil::ComponentPort &port, const CyPhyML::DesignContainer &dc,
                                        const Uml::Class &objType, std::string srcRoleName, std::string dstRoleName)
{
	std::string tmp1=port.port.getPath2();
	std::string tmp2=dc.getPath2();
	if((std::string)dc.ContainerType()!="Compound")
		return true;

	Udm::Object port_parent;
	if(port.port_ref_parent)
	{
		if(Uml::IsDerivedFrom(port.port_ref_parent.type(), CyPhyML::ComponentRef::meta))
			port_parent = CyPhyML::ComponentRef::Cast(port.port_ref_parent).ref();
	}
	else
		port_parent = port.port.GetParent();

	if(!port_parent)
		return false;

	if(Uml::IsDerivedFrom(port_parent.type(), CyPhyML::DesignElement::meta))
		return false;

	if(!Uml::IsDerivedFrom(port_parent.type(), CyPhyML::DesignContainer::meta))
		return false;

	if(port_parent!=dc)
		return isAltPort(port, objType, srcRoleName, dstRoleName);

	CyPhyML::DesignContainer dc_parent;
	Udm::Object dc_parent_obj = port_parent.GetParent();
	if (Uml::IsDerivedFrom(dc_parent_obj.type(), CyPhyML::DesignContainer::meta))
		dc_parent = CyPhyML::DesignContainer::Cast(dc_parent_obj);
	else
		return false;

	Udm::Object::AssociationInfo assocInfo1(objType);
	assocInfo1.strSrcRoleName = srcRoleName;
	assocInfo1.strDstRoleName = dstRoleName;

	set<Udm::Object> assocs = Udm::Object(port.port).GetAssociationClassObjects(NULL, assocInfo1);
	assocInfo1.strSrcRoleName = dstRoleName;
	assocInfo1.strDstRoleName = srcRoleName;
	set<Udm::Object> assocs1 = Udm::Object(port.port).GetAssociationClassObjects(NULL, assocInfo1);
	assocs.insert(assocs1.begin(), assocs1.end());

	for(auto it=assocs.begin();it!=assocs.end();++it)
	{
		Udm::Object assoc = *it;
		Udm::Object assoc_parent = assoc.GetParent();
		CyPhyUtil::ComponentPortPair endpair = CyPhyUtil::getConnectionEnds(objType, assoc);
		CyPhyUtil::ComponentPort peer_port = endpair.src == port? endpair.dst : endpair.src;
		if(assoc_parent==dc_parent)
		{
			if(isConnectedToAltPort(peer_port, dc_parent, objType, srcRoleName, dstRoleName))
				return true;
		}
		else if(assoc_parent==dc)
		{
		     if(isAltPort(peer_port,objType, srcRoleName, dstRoleName))
		            return true;
		}
	}
	return false;
}


DesertIface::VariableProperty CyPhy2Desert::createDesertVP_PCMNONE(DesertIface::Element &delem, std::string name, double value)
{
	DesertIface::NaturalDomain domain;
	DesertIface::NaturalMember member;
	if(!getDomainWithMember(name, domain, value, member))
		throw udm_exception("Property/Parameter: "+name+" of component: "+(std::string)delem.name()+" is not NaturalDomain. Please check the ComputationType.");
	DesertIface::VariableProperty vp = DesertIface::VariableProperty::Create(delem);
//	updatevpMap(vp, cyphyProp, cyphy_com);
	vp.parametric() = false;
	vp.name() = vp.CUSTName() = name;
	vp.id() = vp.externalID() = delem.externalID()+5000;
	vp.PCM_STR() = "PCM_NONE";
	vp.Max() = 10000000000000;
	vp.domain() =domain;

	DesertIface::AssignedValues av = DesertIface::AssignedValues::Create(delem);
	av.id() = av.externalID() = 2000;
	av.properties_end() = vp;
	av.values_end() = member;
	return vp;
}

void CyPhy2Desert::generateConstraint(CyPhyUtil::ComponentPort &port1, CyPhyUtil::ComponentPort &port2, const CyPhyML::DesignEntity &celem,bool isdefault)
{
	DesertIface::Element delem = getDesertElement(celem);
	if(!delem)
		throw udm_exception(std::string(celem.name())+" cannot find the desert element.");

	Udm::Object port1_parent = port1.port_ref_parent;
	if(!port1_parent)
		port1_parent = port1.port.GetParent();
	if(!Uml::IsDerivedFrom(port1_parent.type(), CyPhyML::DesignEntity::meta))
		return;
	DesertIface::VariableProperty dvp1 = getVariableProperty(port1.port, CyPhyML::DesignEntity::Cast(port1_parent));
	if(!dvp1) return;

	Udm::Object port2_parent = port2.port_ref_parent;
	if(!port2_parent)
		port2_parent = port2.port.GetParent();
	if(!Uml::IsDerivedFrom(port2_parent.type(), CyPhyML::DesignEntity::meta))
		return;
	DesertIface::VariableProperty dvp2 = getVariableProperty(port2.port, CyPhyML::DesignEntity::Cast(port2_parent));
	if(!dvp2) return;

	std::string port1_path = DFUtil::getRelativePath((Udm::Object)port1.port, port1_parent, celem)+UdmUtil::ExtractName(port1.port);
	std::string port2_path = DFUtil::getRelativePath((Udm::Object)port2.port, port2_parent, celem)+UdmUtil::ExtractName(port2.port);
	std::string expr ="("+ port1_path+"()="+port2_path+"()"+")";
	expr += ("\nor\n("+port1_path+"()=0)");
	expr += ("\nor\n("+port2_path+"()=0)");


	DesertIface::Constraint dconstraint = DesertIface::Constraint::Create(constraintSet);
	std::string con_name = UdmUtil::ExtractName(port1.port)+"_"+UdmUtil::ExtractName(port2.port)+CyPhy2Desert::increaseCounter()+"_constraint";
	dconstraint.name() = con_name;
	dconstraint.context() = delem;
	dconstraint.expression() = "constraint "+ con_name+"() {\n" + expr+"\n}";
	if(isdefault)
	{
		defaultConstraints.insert(con_name);
	}
}

bool CyPhy2Desert::isConstantProperty(set<CyPhyML::ValueFlow> &src_vfs)
{
	bool ret = true;
	for(set<CyPhyML::ValueFlow>::iterator i=src_vfs.begin();i!=src_vfs.end();++i)
	{
		CyPhyML::ValueFlowTarget src_vf = (*i).srcValueFlow_end();
		set<CyPhyML::ValueFlow> in_vfs = src_vf.srcValueFlow();
		//need filter out the componentref
		if (Uml::IsDerivedFrom(src_vf.type(), CyPhyML::Parameter::meta) || Uml::IsDerivedFrom(src_vf.type(), CyPhyML::Property::meta))
		{
			if(!in_vfs.empty())
				return false;
		}
		else
			if(!isConstantProperty(in_vfs)) return false;
	}
	return ret;
}

//process Parametric Property(direct valueflow, dst of Customformula/SimpleFormula)
void CyPhy2Desert::processProperty(const CyPhyML::DesignEntity &cyphy_com, DesertIface::Element &desert_elem, Udm::Object &prop,set<CyPhyML::ValueFlow> &src_vfs,
	set<CyPhyML::ValueFlowTarget>& alt_vfends)
{
	if(cyphy_com.type()==CyPhyML::DesignContainer::meta)
	{
		CyPhyML::DesignContainer dc = CyPhyML::DesignContainer::Cast(cyphy_com);
		if((std::string)dc.ContainerType()!="Compound" && src_vfs.size()>1)
		{
			if(Uml::IsDerivedFrom(prop.type(), CyPhyML::ValueFlowTarget::meta))
				alt_vfends.insert(CyPhyML::ValueFlowTarget::Cast(prop));
			return;
		}
	}
	std::string propname = UdmUtil::ExtractName(prop);

	if (src_vfs.size()>1)
		throw udm_exception("<a href=\"mga:" + UdmGme::UdmId2GmeId(prop.uniqueId()) + "\">" + propname + "</a> cannot have more than one source ValueFlow if it has one end of Property/Parameter.");

	DesertIface::VariableProperty vp = DesertIface::VariableProperty::Create(desert_elem);
	updatevpMap(vp, prop, cyphy_com);
	vp.parametric() = true;
	vp.name() = vp.CUSTName() = propname;
	vp.id() = vp.externalID() = cyphy_com.ID()+5000;

	CyPhyML::ValueFlow src_vf = *(src_vfs.begin());
	CyPhyML::ValueFlowTarget src_vf_end = src_vf.srcValueFlow_end();
	Udm::Object src_vf_end_parent = src_vf.srcValueFlow_refport_parent();
	if(src_vf_end_parent==Udm::null)
		src_vf_end_parent = src_vf_end.parent();

	//for direct ValueFlow
	if (Uml::IsDerivedFrom(src_vf_end.type(), CyPhyML::Parameter::meta)
		|| Uml::IsDerivedFrom(src_vf_end.type(), CyPhyML::Property::meta)
		|| Uml::IsDerivedFrom(src_vf_end.type(), CyPhyML::Constant::meta)
		)
	{
		DesertIface::CustomFormula dcf = DesertIface::CustomFormula::Create(formulaSet);
		dcf.name() = propname;

		std::string expr = "constraint formula_" + increaseCounter() + "_" + propname + " () {"
			+ DFUtil::getRelativePath(src_vf_end, src_vf_end_parent, cyphy_com) + std::string(src_vf_end.name()) + "()";
		if (src_vf.GetParent().type() == CyPhyML::DesignContainer::meta)
		{
			CyPhyML::DesignContainer dc = CyPhyML::DesignContainer::Cast(src_vf.GetParent());
			if ((std::string)dc.ContainerType() == "Optional") {
				expr += " or " +
					DFUtil::getRelativePath(src_vf_end, src_vf_end_parent.GetParent(), cyphy_com) +
					std::string("children(\"null\").") + std::string(src_vf_end.name()) + "()";
			}
			else if ((std::string)dc.ContainerType() == "Alternative") {
				// FIXME tbd
			}
		}
		expr += "}";
		dcf.expression() = expr;

		dcf.property_end() += vp;
		dcf.context() = desert_elem;
	}
	//if the property is the dst end of CustomFormula/SimpleFormula, it will be processed in processCustomFormula/processSimpleformula
}

//process non-parametric Parameter(natual number)
void CyPhy2Desert::processProperty(const CyPhyML::DesignEntity &cyphy_com, DesertIface::Element &desert_elem,
	                               const std::string &pname, double value, const std::string &compType, Udm::Object cyphyProp, bool assignValue)
{
	DesertIface::NaturalDomain domain;
	DesertIface::NaturalMember member;
	if(!getDomainWithMember(pname, domain, value, member))
		throw udm_exception("Property/Parameter: "+pname+" of component: "+(std::string)cyphy_com.name()+" is not NaturalDomain. Please check the ComputationType.");
	DesertIface::VariableProperty vp = DesertIface::VariableProperty::Create(desert_elem);
	updatevpMap(vp, cyphyProp, cyphy_com);
	vp.parametric() = false;
	vp.name() = vp.CUSTName() = pname;
	vp.id() = vp.externalID() = cyphy_com.ID()+5000;

	vp.PCM_STR() = "PCM_NONE";
	if(isAlternativeContainer(CyPhyML::DesignEntity(cyphy_com)) && Uml::IsDerivedFrom(cyphyProp.type(), CyPhyML::ValueFlowTarget::meta))
	{
		CyPhyML::ValueFlowTarget vftarget = CyPhyML::ValueFlowTarget::Cast(cyphyProp);
		set<CyPhyML::ValueFlow> srcvfs = vftarget.srcValueFlow();
		set<CyPhyML::ValueFlow> dstvfs = vftarget.dstValueFlow();
		if(srcvfs.empty() && dstvfs.empty())
			vp.PCM_STR() = "PCM_ADD";
		else if(!srcvfs.empty())
		{
			CyPhyML::ValueFlow vf = *(srcvfs.begin());
			Udm::Object  vf_parent = vf.parent();
			if(vf_parent==cyphy_com)
				vp.PCM_STR() = "PCM_ADD";
		}
		else if(!dstvfs.empty())
		{
			for(set<CyPhyML::ValueFlow>::iterator it=dstvfs.begin();it!=dstvfs.end();++it)
			{
				CyPhyML::ValueFlow dstvf = *it;
				Udm::Object  vf_parent = dstvf.parent();
				if(vf_parent!=cyphy_com)
				{
					vp.PCM_STR() = "PCM_ADD";
					break;
				}
			}
		}

	}

	vp.Max() = 100000;
	vp.domain() =domain;

	if(assignValue)
	{
		DesertIface::AssignedValues av = DesertIface::AssignedValues::Create(desert_elem);
		av.id() = av.externalID() = 2000;
		av.properties_end() = vp;
		av.values_end() = member;
	}
}

//process non-parametric Parameter(string type value)
void CyPhy2Desert::processProperty(const CyPhyML::DesignEntity &cyphy_com, DesertIface::Element &desert_elem,
		                           const std::string &pname, const std::string &value)
{
	DesertIface::CustomDomain domain;
	DesertIface::CustomMember member;
	if(!getDomainWithMember(pname,domain,value,member))
		throw udm_exception("Property/Parameter: "+pname+" of component: "+(std::string)cyphy_com.name()+" is not CustomDomain. Please check the ComputationType.");
	DesertIface::VariableProperty vp = DesertIface::VariableProperty::Create(desert_elem);
	vp.parametric() = false;
	vp.name() = vp.CUSTName() = pname;
	vp.id() = vp.externalID() = cyphy_com.uniqueId()+1;
	vp.PCM_STR() = "PCM_NOP";
	vp.domain() = domain;

	DesertIface::AssignedValues av = DesertIface::AssignedValues::Create(desert_elem);
	av.id() = av.externalID() = 2000;
	av.properties_end() = vp;
	av.values_end() = member;
}

double CyPhy2Desert::getPropertyNumberValue(CyPhyML::ValueFlowTarget &vft)
{
	if(vft.type()!=CyPhyML::Property::meta && vft.type()!=CyPhyML::Parameter::meta)
		return 0;

	double ret = 0;
	set<CyPhyML::ValueFlow> srcfs = vft.srcValueFlow();

	set<CyPhyML::SimpleFormula> sfs;
	for(set<CyPhyML::ValueFlow>::iterator i=srcfs.begin();i!=srcfs.end();++i)
	{
		CyPhyML::ValueFlowTarget dvft = (*i).srcValueFlow_end();
		if(dvft.type()==CyPhyML::SimpleFormula::meta)
			sfs.insert(CyPhyML::SimpleFormula::Cast(dvft));
	}

	if(srcfs.empty() || sfs.empty())
	{
		std::string svalue;
		if(vft.type()==CyPhyML::Property::meta)
			svalue = (CyPhyML::Property::Cast(vft)).Value();
		else if(vft.type()==CyPhyML::Parameter::meta)
			svalue = (CyPhyML::Parameter::Cast(vft)).Value();
		ret = atof(svalue.c_str());
		return ret;
	}

	if(sfs.size()>1)
		return 0;

	CyPhyML::SimpleFormula sf = *(sfs.begin());
	std::string method = sf.Method();
	double dret;
	vector<double> args;
	set<CyPhyML::ValueFlow> sf_srcs = sf.srcValueFlow();
	for(set<CyPhyML::ValueFlow>::iterator si=sf_srcs.begin();si!=sf_srcs.end();++si)
	{
		CyPhyML::ValueFlowTarget src_arg = (*si).srcValueFlow_end();
		if(src_arg.type()!=CyPhyML::Property::meta && src_arg.type()!=CyPhyML::Parameter::meta)
			continue;
		std::string strval;
		if(src_arg.type()==CyPhyML::Property::meta)
		{
			CyPhyML::Property cprop = CyPhyML::Property::Cast(src_arg);
			std::string ct = cprop.ComputationType();
			//if(ct=="None" || ct=="Custom")
			if(ct=="Custom")
				continue;
			strval= cprop.Value();
		}
		else
		{
			CyPhyML::Parameter cpara = CyPhyML::Parameter::Cast(src_arg);
			std::string ct = cpara.ComputationType();
			//if(ct=="None" || ct=="Custom")
			if(ct=="Custom")
				continue;
			strval= cpara.Value();
		}
		args.push_back(atof(strval.c_str()));
	}

	if(method=="Addition")
		dret = getAdditionValue(args);
	else if(method=="Multiplication")
		dret = getMultiplicationValue(args);
	else if(method=="Maximum")
		dret = getMaximumValue(args);
	else if(method=="Minimum")
		dret = getMinimumValue(args);
	else if(method=="ArithmeticMean")
		dret = getArithMeanValue(args);
	else if(method=="GeometricMean")
		dret = getGeometriMeanValue(args);

	return dret;
}

bool CyPhy2Desert::getDomainWithMember(const std::string &dname, DesertIface::NaturalDomain &domain,
	                                   double value, DesertIface::NaturalMember &member)
{
	map<std::string, DesertIface::Domain>::iterator pos = domainMap.find(dname);
	if(pos==domainMap.end())
	{
		domain = generateNaturalDomain(dsystem, dname);
		domainMap[dname] = domain;
	}
	else
	{
		DesertIface::Domain domain_base = (*pos).second;
		if(domain_base.type()!=DesertIface::NaturalDomain::meta)
			return false;   //this error may be cuased by its parent container which also contains this property/parameter, but incorrect computationType
		domain = DesertIface::NaturalDomain::Cast(domain_base);
	}

	if(domain.maximum()<value)
		domain.maximum() = value+1;
	if(domain.minimum()>value)
		domain.minimum() = value-1;

	map<double, DesertIface::NaturalMember> memberMap;
	map<DesertIface::NaturalDomain, map<double, DesertIface::NaturalMember> >::iterator mpos = natureMemberMap.find(domain);
	if(mpos != natureMemberMap.end())
	{
		memberMap = (*mpos).second;
	}

	map<double, DesertIface::NaturalMember>::iterator mempos = memberMap.find(value);
	if(mempos==memberMap.end())
	{
		static int memberId = 0;
		member = DesertIface::NaturalMember::Create(domain);
		member.id() = member.externalID() = memberId;
		memberId++;
		member.value() = value;
		memberMap[value] = member;
		natureMemberMap[domain] = memberMap;
	}
	else
		member = (*mempos).second;

	return true;
}

bool CyPhy2Desert::getDomainWithMember(const std::string &dname, DesertIface::CustomDomain &domain,
		                               const std::string &value, DesertIface::CustomMember &member)
{
	map<std::string, DesertIface::Domain>::iterator pos = domainMap.find(dname);
	if(pos==domainMap.end())
	{
		domain = generateCustomDomain(dname);
		domainMap[dname] = domain;
	}
	else
	{
		DesertIface::Domain domain_base = (*pos).second;
		if(domain_base.type()!=DesertIface::CustomDomain::meta)
			return false;
		domain = DesertIface::CustomDomain::Cast(domain_base);
	}

	map<std::string, DesertIface::CustomMember> memberMap;
	map<DesertIface::CustomDomain, map<std::string, DesertIface::CustomMember> >::iterator mpos = custMemberMap.find(domain);
	if(mpos != custMemberMap.end())
	{
		memberMap = (*mpos).second;
	}

	map<std::string, DesertIface::CustomMember>::iterator mempos = memberMap.find(value);
	if(mempos==memberMap.end())
	{
		static int memberId = 0;
		member = DesertIface::CustomMember::Create(domain);
		member.id() = member.externalID() = memberId;
		memberId++;
		member.name() = value;
		memberMap[value] = member;
		custMemberMap[domain] = memberMap;
	}
	else
		member = (*mempos).second;

	return true;
}

void CyPhy2Desert::processConstraints(const CyPhyML::DesignContainer &cyphy_container, DesertIface::Element &desert_elem)
{
	DOEVENTS();
	const set<CyPhyML::Constraint> constraints = cyphy_container.Constraint_kind_children();
	for(set<CyPhyML::Constraint>::iterator ci=constraints.begin();ci!=constraints.end();++ci)
	{
		CyPhyML::Constraint constraint = *ci;
		if(constraint.Inactive()) continue;
		if(std::string(constraint.Context()).empty()) continue;
		DesertIface::Constraint dsConstraint = DesertIface::Constraint::Create(constraintSet);
		dsConstraint.name() = constraint.name();
		dsConstraint.id() = dsConstraint.externalID() = constraint.ID();
		dsConstraint.expression() = constraint.Context();
		dsConstraint.context() = desert_elem;
		constraintMap[dsConstraint] = constraint;
	}

	const set<CyPhyML::VisualConstraint> vcons = cyphy_container.VisualConstraint_kind_children();
	for(set<CyPhyML::VisualConstraint>::iterator vci=vcons.begin();vci!=vcons.end();++vci)
	{
		CyPhyML::VisualConstraint vcon = *vci;
		if(vcon.Inactive()) continue;

		std::string expr = generateVisualConstraint(cyphy_container, vcon);
		if(expr.empty()) continue;

		//create the constraint in desert
		DesertIface::Constraint dsConstraint = DesertIface::Constraint::Create(constraintSet);
		dsConstraint.name() = vcon.name();
		dsConstraint.id() = dsConstraint.externalID() = vcon.ID();
		dsConstraint.expression() = "constraint "+ (std::string)vcon.name()+"() {\n" + expr+"\n}";
		dsConstraint.context() = desert_elem;
		constraintMap[dsConstraint] = vcon;
	}

	const set<CyPhyML::DecisionGroup> groups = cyphy_container.DecisionGroup_kind_children();
	for(set<CyPhyML::DecisionGroup>::iterator git=groups.begin();git!=groups.end();++git)
	{
		CyPhyML::DecisionGroup group = *git;
		const set<CyPhyML::DesignEntityRef> refs = group.DesignEntityRef_kind_children();
		set<CyPhyML::DesignContainer> groupContainers;
		for(set<CyPhyML::DesignEntityRef>::iterator ref_it=refs.begin();ref_it!=refs.end();++ref_it)
		{
			CyPhyML::DesignEntity dentiti = (*ref_it).ref();
			if (!dentiti)
			{
				throw udm_exception("Invalid Decision Group reference " + ref_it->getPath("/", false, false));
			}
			if(Uml::IsDerivedFrom(dentiti.type(), CyPhyML::DesignContainer::meta))
			{
				CyPhyML::DesignContainer container = CyPhyML::DesignContainer::Cast(dentiti);
				if((std::string)container.ContainerType()!="Compound")
					groupContainers.insert(container);
			}
		}

		if(groupContainers.size()>1)
		{
			// map<std::string, list<CyPhyML::DesignEntity> > groups;
			// if((std::string)group.DecisionCriteria()=="SameName")
			//	gatherGroupBasedOnName(groupContainers, groups);
			// else
			//	gatherGroupBasedOnArchType(groupContainers, groups);

			//generateConstraint(groups, cyphy_container, desert_elem);

			map<CyPhyML::DesignElement, list<CyPhyML::DesignEntity> > groups;
			gatherGroupBasedOnRef(groupContainers, groups);
			if(!generateConstraint(groups, cyphy_container, desert_elem))
				invalidGroups.insert(group);
		}
	}

	const set<CyPhyML::PropertyConstraint> pcons = cyphy_container.PropertyConstraint_kind_children();
	for(set<CyPhyML::PropertyConstraint>::iterator pit=pcons.begin();pit!=pcons.end();++pit)
	{
		CyPhyML::PropertyConstraint pcon = *pit;
		if(pcon.Inactive()) continue;

		set<CyPhyML::PropertyConstraintRef> pcon_refs = pcon.referedbyPropertyConstraintRef();
		if(!pcon_refs.empty()) continue;

		std::string expr;
		std::string rtype = pcon.TargetType();
		if(rtype == "Must Exceed")
			expr = " > ";
		else if(rtype == "Must Not Exceed")
			expr = " <= ";
		else if(rtype == "Must Equal")
			expr = " = ";
		else if(rtype == "Must Equal or Exceed")
			expr = " >= ";
		else if(rtype == "Must Be Less Than")
			expr = " < ";

		char limit[64];
		double dv = pcon.TargetValue();
		sprintf_s(limit,"%f",dv);

		expr += (std::string)limit;

		set<CyPhyML::PropertyConstraintBinding> bindings = pcon.dstPropertyConstraintBinding();
		for(set<CyPhyML::PropertyConstraintBinding>::iterator bit=bindings.begin();bit!=bindings.end();++bit)
		{
			std::string currExpr;
			CyPhyML::Property propertCon = (*bit).dstPropertyConstraintBinding_end();
			//create the constraint in desert
			std::string dsconName = (std::string)cyphy_container.name()+"_"+(std::string)pcon.name()+"_constraint";

			Udm::Object propParent = propertCon.parent();
			if(cyphy_container == propParent)
				currExpr = (std::string)propertCon.name() + "()" + expr;
			else
				currExpr = "children(\""+UdmUtil::ExtractName(propParent)+"\")."+(std::string)propertCon.name() + "()" + expr;
			if (Uml::IsDerivedFrom(cyphy_container.type(), CyPhyML::DesignContainer::meta))
			{
				auto dc = CyPhyML::DesignContainer::Cast(cyphy_container);
				if (dc.ContainerType() == "Optional")
				{
					// if optional is not selected, constraint doesn't matter
					currExpr = std::string("implementedBy() = children(\"null\") or ") + currExpr;
				}
			}

			DesertIface::Constraint dsConstraint = DesertIface::Constraint::Create(constraintSet);
			dsConstraint.name() = dsconName;
			dsConstraint.id() = dsConstraint.externalID() = pcon.ID();
			dsConstraint.expression() = "constraint "+ dsconName+"() {\n" + currExpr+"\n}";
			dsConstraint.context() = desert_elem;
			constraintMap[dsConstraint] = pcon;
		}
	}
}

std::string CyPhy2Desert::generateConstraint(const CyPhyML::ImpliesEnd &iend, const Udm::Object &container)
{
	if(Uml::IsDerivedFrom(iend.type(), CyPhyML::And_Or::meta))
		return generateConstraint(CyPhyML::And_Or::Cast(iend), container);
	else if(Uml::IsDerivedFrom(iend.type(), CyPhyML::DesignEntityRef::meta))
		return generateConstraint(CyPhyML::DesignEntityRef::Cast(iend), container);
	else if(Uml::IsDerivedFrom(iend.type(), CyPhyML::NullOptionRef::meta))
		return generateConstraint(CyPhyML::NullOptionRef::Cast(iend), container);
	else if(Uml::IsDerivedFrom(iend.type(), CyPhyML::PropertyConstraintRef::meta))
		return generateConstraint(CyPhyML::PropertyConstraintRef::Cast(iend), container);
	return "";
}

std::string CyPhy2Desert::generateConstraint(const CyPhyML::And_Or &and_or, const Udm::Object &container)
{
	std::string ret;

	std::string op;
	op = Uml::IsDerivedFrom(and_or.type(), CyPhyML::Or_operator::meta) ? " or " : " and ";  //for or_operator: or; for and and not operator: and

	set<CyPhyML::ImpliesEnd> implyends = and_or.ImpliesEnd_kind_children();
	bool mul = implyends.size()>1 ? true : false;
	set<CyPhyML::ImpliesEnd>::iterator i=implyends.begin();
	while(i!=implyends.end())
	{
		CyPhyML::ImpliesEnd currend = *i;
		std::string context;
		if(Uml::IsDerivedFrom(currend.type(), CyPhyML::DesignEntityRef::meta))
			context = generateConstraint(CyPhyML::DesignEntityRef::Cast(currend), container);
		else if(Uml::IsDerivedFrom(currend.type(), CyPhyML::And_Or::meta))
			context = generateConstraint(CyPhyML::And_Or::Cast(currend), container);
		else if(Uml::IsDerivedFrom(currend.type(), CyPhyML::NullOptionRef::meta))
			context = generateConstraint(CyPhyML::NullOptionRef::Cast(currend), container);
		else if(Uml::IsDerivedFrom(currend.type(), CyPhyML::PropertyConstraintRef::meta))
			context = generateConstraint(CyPhyML::PropertyConstraintRef::Cast(currend), container);

		if(Uml::IsDerivedFrom(and_or.type(), CyPhyML::Not_operator::meta))
			context = "not("+context+")";

		ret += context;
		i++;
		if(mul && i!=implyends.end())
			ret = ret + "\n" + op + "\n";
	}

	if(!ret.empty())
		ret = "(" + ret + ")\n";

	return ret;
}

std::string CyPhy2Desert::generateConstraint(const CyPhyML::DesignEntityRef &elem_ref, const Udm::Object &container)
{
	CyPhyML::DesignEntity entity = CyPhyML::DesignEntity::Cast(elem_ref.ref());
	return generateConstraint(entity, container);
}

std::string CyPhy2Desert::generateConstraint(const CyPhyML::NullOptionRef &null_ref, const Udm::Object &container)
{
	CyPhyML::DesignContainer optionCon = null_ref.ref();
	if(optionCon==Udm::null) return "";
	return generateImplConstraint(optionCon, container, "null");
}

std::string CyPhy2Desert::generateConstraint(const CyPhyML::PropertyConstraintRef &pcon_ref, const Udm::Object &container)
{
	CyPhyML::PropertyConstraint pcon = pcon_ref.ref();
	if(pcon==Udm::null) return "";
	return generateConstraint(pcon, container);

}

std::string CyPhy2Desert::generateConstraint(const CyPhyML::PropertyConstraint &pcon, const Udm::Object &container)
{
	set<CyPhyML::PropertyConstraintBinding> bindings = pcon.dstPropertyConstraintBinding();
	if(bindings.empty()) return "";

	std::string ret;
	std::string expr;
	std::string rtype = pcon.TargetType();
	if(rtype == "Must Exceed")
		expr = " > ";
	else if(rtype == "Must Not Exceed")
		expr = " <= ";
	else if(rtype == "Must Equal")
		expr = " = ";
	else if(rtype == "Must Equal or Exceed")
		expr = " >= ";
	else if(rtype == "Must Be Less Than")
		expr = " < ";

	char limit[64];
	double dv = pcon.TargetValue();
	sprintf_s(limit,"%f",dv);

	expr += (std::string)limit;

	set<CyPhyML::PropertyConstraintBinding>::iterator bit;
	while(!bindings.empty())
	{
		bit = bindings.begin();
		CyPhyML::Property prop = (*bit).dstPropertyConstraintBinding_end();

		Udm::Object propParent = prop.parent();
		std::string path = getPath(propParent, container);

		ret = ret + "("+path+(std::string)prop.name() + "()" + expr+")";

		bindings.erase(bit);
		if(!bindings.empty())
			ret += " and ";
	}

	return ret;
}

std::string CyPhy2Desert::getPath(const Udm::Object &obj, const Udm::Object &container)
{
	std::string path("self.");

	Udm::Object pobj = obj;
	list<Udm::Object> childrenlist;
	while(pobj!=container && Uml::IsDerivedFrom(pobj.type(), CyPhyML::DesignContainer::meta))
	{
		childrenlist.push_front(pobj);
		pobj = pobj.GetParent();
	}

	if(pobj == container)
	{
		for(list<Udm::Object>::iterator i=childrenlist.begin();i!=childrenlist.end();++i)
		{
			path = path+"children(\""+UdmUtil::ExtractName(*i)+"\").";
		}
	}
	else if(Uml::IsDerivedFrom(pobj.type(), CyPhyML::RootFolder::meta) || Uml::IsDerivedFrom(pobj.type(), CyPhyML::DesignSpace::meta))
	{
		pobj = container.GetParent();
		int cnt = 1;
		bool found = false;
		list<Udm::Object>::iterator i;
		while(Uml::IsDerivedFrom(pobj.type(), CyPhyML::DesignContainer::meta))
		{
			for(i=childrenlist.begin();i!=childrenlist.end();++i)
			{
				if(*i == pobj)
				{
					found = true;
					break;
				}
			}

			if(!found)
			{
				pobj = pobj.GetParent();
				cnt++;
			}
			else
			{
				if(i==childrenlist.begin())
					childrenlist.erase(childrenlist.begin());
				else
					childrenlist.erase(childrenlist.begin(),i);
				break;
			}
		}

		for(int t=0;t<cnt;++t)
			path += "parent().";
		for(i=childrenlist.begin();i!=childrenlist.end();++i)
			path = path+"children(\""+UdmUtil::ExtractName(*i)+"\").";
	}

	return path;
}

std::string CyPhy2Desert::generateImplConstraint(Udm::Object &pobj, const Udm::Object &container, const std::string &impl)
{
	std::string ret;
	std::string path = getPath(pobj, container);
	ret = path + "implementedBy() = " + path+"children(\""+impl+"\")";

	if(!ret.empty())
		ret = "(" + ret + ")\n";

	return ret;
}

std::string CyPhy2Desert::generateConstraint(const CyPhyML::DesignEntity &entity, const Udm::Object &container)
{
	if(entity==Udm::null) return "";
	Udm::Object pobj = entity.parent();
	return generateImplConstraint(pobj, container, (std::string)entity.name());
}

set<std::string> CyPhy2Desert::getNatualParameters()
{
	return naturalParameters;
}

set<std::string> CyPhy2Desert::getCustomParameters()
{
	return customParameters;
}

set<std::string> CyPhy2Desert::getGroupConstraits()
{
	return defaultConstraints;
}

set<std::string> CyPhy2Desert::getDefaultConstraits()
{
	return defaultConstraints;
}

double CyPhy2Desert::getAdditionValue(vector<double> args)
{
	double ret = 0;
	for(vector<double>::iterator i=args.begin();i!=args.end();++i)
	{
		ret += *i;
	}
	return ret;
}

double CyPhy2Desert::getMultiplicationValue(vector<double> args)
{
	double ret = 1;
	for(vector<double>::iterator i=args.begin();i!=args.end();++i)
	{
		ret *= *i;    //for temporary use, because the model integer is too large
	}
	return ret;
}

double CyPhy2Desert::getMaximumValue(vector<double> args)
{
	if(args.empty()) return 0;

	double ret = *(args.begin());
	for(vector<double>::iterator i=args.begin();i!=args.end();++i)
	{
		if(*i > ret) ret = *i;
	}
	return ret;
}

double CyPhy2Desert::getMinimumValue(vector<double> args)
{
	if(args.empty()) return 0;

	double ret = *(args.begin());
	for(vector<double>::iterator i=args.begin();i!=args.end();++i)
	{
		if(*i < ret) ret = *i;
	}
	return ret;
}

double CyPhy2Desert::getArithMeanValue(vector<double> args)
{
	double ret = 0;
	for(vector<double>::iterator i=args.begin();i!=args.end();++i)
	{
		ret += *i;
	}
	if(!args.empty())
		ret = ret / (args.size());
	return ret;
}

double CyPhy2Desert::getGeometriMeanValue(vector<double> args)
{
	double ret = 1;
	for(vector<double>::iterator i=args.begin();i!=args.end();++i)
	{
		ret *= *i;
	}

	if(!args.empty())
	{
		double x = 1/args.size();
		ret = pow(ret, x);
	}
	return ret;
}

void CyPhy2Desert::gatherGroupBasedOnName(set<CyPhyML::DesignContainer> &containers, map<std::string, list<CyPhyML::DesignEntity> > &groups)
{
	for(set<CyPhyML::DesignContainer>::iterator it=containers.begin();it!=containers.end();++it)
	{
		set<CyPhyML::DesignEntity> entities = (*it).DesignEntity_kind_children();
		for(set<CyPhyML::DesignEntity>::iterator eit=entities.begin();eit!=entities.end();++eit)
		{
			std::string ename = (*eit).name();
			list<CyPhyML::DesignEntity> tmp;
			map<std::string, list<CyPhyML::DesignEntity> >::iterator pos = groups.find(ename);
			if(pos!=groups.end())
				tmp = (*pos).second;
			tmp.push_back(*eit);
			groups[ename] = tmp;
		}
	}
}

void CyPhy2Desert::gatherGroupBasedOnRef(set<CyPhyML::DesignContainer> &containers, map<CyPhyML::DesignElement, list<CyPhyML::DesignEntity> > &groups)
{
	for(set<CyPhyML::DesignContainer>::iterator it=containers.begin();it!=containers.end();++it)
	{
		set<CyPhyML::DesignEntity> entities = (*it).DesignEntity_kind_children();
		for(set<CyPhyML::DesignEntity>::iterator eit=entities.begin();eit!=entities.end();++eit)
		{
			CyPhyML::DesignEntity curr_entity = *eit;
			if(Uml::IsDerivedFrom(curr_entity.type(), CyPhyML::ComponentRef::meta))
			{
				CyPhyML::DesignElement de = (CyPhyML::ComponentRef::Cast(curr_entity)).ref();
				if(de==Udm::null)
				{
					nullComponentRefs.insert(CyPhyML::ComponentRef::Cast(curr_entity));
					continue;
				}
				list<CyPhyML::DesignEntity> tmp;
				map<CyPhyML::DesignElement, list<CyPhyML::DesignEntity> >::iterator pos = groups.find(de);
				if(pos!=groups.end())
					tmp = (*pos).second;
				tmp.push_back(curr_entity);
				groups[de] = tmp;
			}
		}
	}
}
void CyPhy2Desert::gatherGroupBasedOnArchType(set<CyPhyML::DesignContainer> &containers, map<std::string, list<CyPhyML::DesignEntity> > &groups)
{
	if(containers.size() < 2) return;

	set<CyPhyML::DesignContainer>::iterator it = containers.begin();
	CyPhyML::DesignContainer firstContainer = *it;

}

void CyPhy2Desert::generateConstraint(map<std::string, list<CyPhyML::DesignEntity> > &groups, const Udm::Object &container, DesertIface::Element &desert_elem)
{
	for(map<std::string, list<CyPhyML::DesignEntity> >::iterator pos=groups.begin();pos!=groups.end();++pos)
	{
		list<CyPhyML::DesignEntity> entities = (*pos).second;
		CyPhyML::DesignEntity firstEntity = entities.front();
		entities.pop_front();
		std::string expr = generateConstraint(firstEntity, container)+"implies\n(";
		while(!entities.empty())
		{
			CyPhyML::DesignEntity currEntity = entities.front();
			entities.pop_front();
			if(!entities.empty())
				expr = expr + generateConstraint(currEntity, container) +"and\n";
			else
				expr = expr + generateConstraint(currEntity, container) +"";
		}
		expr += ")";

		groupCount++;
		char buffer[10];
		_itoa_s(groupCount, buffer, 10);
		std::string constraintName = "group_constraint"+std::string(buffer);
		defaultConstraints.insert(constraintName);

		DesertIface::Constraint dsConstraint = DesertIface::Constraint::Create(constraintSet);
		dsConstraint.name() = constraintName;
		/*dsConstraint.id() = dsConstraint.externalID() = vcon.ID();*/
		dsConstraint.expression() = "constraint "+ constraintName+"() {\n" + expr+"\n}";
		dsConstraint.context() = desert_elem;
	}
}

bool CyPhy2Desert::generateConstraint(map<CyPhyML::DesignElement, list<CyPhyML::DesignEntity> > &groups, const Udm::Object &container, DesertIface::Element &desert_elem)
{
	bool ret = true;
	for(map<CyPhyML::DesignElement, list<CyPhyML::DesignEntity> >::iterator pos1=groups.begin();pos1!=groups.end();++pos1)
	{
		list<CyPhyML::DesignEntity> entities = (*pos1).second;
		if(entities.size()<2)
		{
			return false;
		}
	}
	for(map<CyPhyML::DesignElement, list<CyPhyML::DesignEntity> >::iterator pos=groups.begin();pos!=groups.end();++pos)
	{
		list<CyPhyML::DesignEntity> entities = (*pos).second;
		CyPhyML::DesignEntity firstEntity = entities.front();
		entities.pop_front();
		std::string expr = generateConstraint(firstEntity, container)+"implies\n(";
		while(!entities.empty())
		{
			CyPhyML::DesignEntity currEntity = entities.front();
			entities.pop_front();
			if(!entities.empty())
				expr = expr + generateConstraint(currEntity, container) +"and\n";
			else
				expr = expr + generateConstraint(currEntity, container) +"";
		}
		expr += ")";

		groupCount++;
		char buffer[10];
		_itoa_s(groupCount, buffer, 10);
		std::string constraintName = "group_constraint"+std::string(buffer);
		defaultConstraints.insert(constraintName);

		DesertIface::Constraint dsConstraint = DesertIface::Constraint::Create(constraintSet);
		dsConstraint.name() = constraintName;
		/*dsConstraint.id() = dsConstraint.externalID() = vcon.ID();*/
		dsConstraint.expression() = "constraint "+ constraintName+"() {\n" + expr+"\n}";
		dsConstraint.context() = desert_elem;
	}
	return ret;
}

map<int, CyPhyML::DesignEntity> CyPhy2Desert::getMorphMatrixGroup()
{
	return mgroupMap;
}

map<int, CyPhyML::DesignEntity> CyPhy2Desert::getMorphMatrixOption()
{
	return moptionMap;
}

std::string CyPhy2Desert::getRelativePath(Udm::Object &obj, const CyPhyML::DesignEntity &cyphy_elem)
{
	std::string ret;
	if(obj == Udm::null) return ret;

	Udm::Object pobj = obj.GetParent();
	if(pobj == cyphy_elem)
		ret = "";
	else
	{
		Udm::Object ppobj = pobj.GetParent();
		if(ppobj == cyphy_elem)
		{
			ret = "children(\""+UdmUtil::ExtractName(pobj)+"\").";
		}
		else
		{
			Udm::Object cobj = cyphy_elem.parent();
			if(cobj == pobj)
				ret = "parent().";
			else if(cobj == ppobj)
				ret = "parent().children(\""+UdmUtil::ExtractName(pobj)+"\").";
			else
			{
				if(Uml::IsDerivedFrom(cobj.type(), CyPhyML::DesignEntity::meta))
					ret = "parent()."+getRelativePath(obj, CyPhyML::DesignEntity::Cast(cobj));
			}
		}
	}
	return ret;
}

std::string CyPhy2Desert::increaseCounter()
{
	static int c2dCount = 0;
	c2dCount++;
	char buffer[65];
	_itoa_s(c2dCount, buffer, 10);
	return std::string(buffer);
}

std::string CyPhy2Desert::getDesertPCM(const std::string &method)
{
	std::string pcm("PCM_NOP");

	if(method == "Addition")
		pcm = "PCM_ADD";
	else if(method == "Multiplication")
		pcm = "PCM_MUL";
	else if(method == "ArithmeticMean")
		pcm = "PCM_AMED";
	else if(method == "GeometricMean")
		pcm = "PCM_GMED";
	else if(method == "Maximum")
		pcm = "PCM_MAX";
	else if(method == "Minimum")
		pcm = "PCM_MIN";
	else if(method == "None")
		pcm = "PCM_NONE";
	else //custom
		pcm = "PCM_CUST";

	return pcm;
}

bool CyPhy2Desert::isAlternativeContainer(Udm::Object &obj)
{
	if(Uml::IsDerivedFrom(obj.type(), CyPhyML::DesignContainer::meta))
	{
		CyPhyML::DesignContainer dc = CyPhyML::DesignContainer::Cast(obj);
		if((std::string)dc.ContainerType()!="Compound")
			return true;
		else
			return false;
	}
	else
		return false;
}

DesertIface::VariableProperty CyPhy2Desert::getVariableProperty(Udm::Object &cyphy_obj, CyPhyML::DesignEntity &obj_parent)
{
	DesertIface::VariableProperty dvp;
	map<Udm::Object, map<CyPhyML::DesignEntity, DesertIface::VariableProperty> >::iterator vpMap_pos = vpMap.find(cyphy_obj);
	if(vpMap_pos!=vpMap.end())
	{
		const map<CyPhyML::DesignEntity, DesertIface::VariableProperty>& tmpMap = (*vpMap_pos).second;
		map<CyPhyML::DesignEntity, DesertIface::VariableProperty>::const_iterator tmpMap_pos = tmpMap.find(obj_parent);
		if (tmpMap_pos!=tmpMap.end())
			dvp = (*tmpMap_pos).second;
	}
	return dvp;
}

void CyPhy2Desert::updatevpMap(const DesertIface::VariableProperty &dvp, const Udm::Object &cyphy_obj, const CyPhyML::DesignEntity &obj_parent)
{
	auto vpMap_pos = vpMap.insert(std::make_pair(cyphy_obj, map<CyPhyML::DesignEntity, DesertIface::VariableProperty>()));
	vpMap_pos.first->second[obj_parent] = dvp;
}

void CyPhy2Desert::checkSourceValueFlows(set<CyPhyML::ValueFlow> &vfs, const CyPhyML::DesignContainer &dc)
{
	//check whether the incoming valueflow is outside of the root designContainer,
	//some design explorer may start in lower level designContainer
	set<CyPhyML::ValueFlow> tmpvfs;
	Udm::Object rootDC_parent = rootDC.parent();
	Udm::Object dc_parent = dc.parent();
	for(set<CyPhyML::ValueFlow>::iterator i=vfs.begin();i!=vfs.end();++i)
	{
		CyPhyML::ValueFlow vf = *i;
		Udm::Object vf_parent = vf.GetParent();
		if(vf_parent==rootDC_parent) continue;
		if(vf_parent==dc || vf_parent==dc_parent)
			tmpvfs.insert(vf);
	}
	vfs.clear();
	vfs.insert(tmpvfs.begin(), tmpvfs.end());
}

void CyPhy2Desert::processAlternativeParameters(CyPhyML::DesignContainer &altDC)
{
	//collect all parameters
	map<std::string, set<std::string> > parameterMap;
	std::set<std::string> propertyNames;
	auto addPropertyNames = [&propertyNames](const std::set<CyPhyML::Property>& props) {
		std::for_each(props.begin(), props.end(), [&](const CyPhyML::Property prop) {
			propertyNames.insert(prop.name());
		});
	};
	set<CyPhyML::DesignEntity> des = altDC.DesignEntity_kind_children();
	for(set<CyPhyML::DesignEntity>::iterator it=des.begin();it!=des.end();++it)
	{
		CyPhyML::DesignEntity de = *it;

		if(Uml::IsDerivedFrom(de.type(), CyPhyML::DesignElement::meta))
		{
			CyPhyML::DesignElement celem = CyPhyML::DesignElement::Cast(de);
			set<CyPhyML::Parameter> params = celem.Parameter_kind_children();
			for(set<CyPhyML::Parameter>::iterator pit=params.begin();pit!=params.end();++pit)
				insertParameterMap(*pit, parameterMap);
			addPropertyNames(celem.Property_children());
		}
		else if(Uml::IsDerivedFrom(de.type(), CyPhyML::ComponentRef::meta))
		{
			CyPhyML::ComponentRef comref = CyPhyML::ComponentRef::Cast(de);
			CyPhyML::DesignElement celem = comref.ref();
			if(celem!=Udm::null)
			{
				set<CyPhyML::Parameter> params = celem.Parameter_kind_children();
				for(set<CyPhyML::Parameter>::iterator pit=params.begin();pit!=params.end();++pit)
					insertParameterMap(*pit, parameterMap);
				addPropertyNames(celem.Property_children());
			}
			else
				nullComponentRefs.insert(comref);
		}
		else if(Uml::IsDerivedFrom(de.type(), CyPhyML::DesignContainer::meta))
		{
			CyPhyML::DesignContainer cdc = CyPhyML::DesignContainer::Cast(de);
			set<CyPhyML::Parameter> params = cdc.Parameter_kind_children();
			for(set<CyPhyML::Parameter>::iterator pit=params.begin();pit!=params.end();++pit)
				insertParameterMap(*pit, parameterMap);
			addPropertyNames(cdc.Property_children());
		}
	}

	DesertIface::Element alt_delem = com2elemMap[altDC];
	ASSERT(alt_delem!=Udm::null);
	//generate the parameter constraint for the alternative container
	for(map<std::string, set<std::string> >::iterator pos=parameterMap.begin();pos!=parameterMap.end();++pos)
	{
		if (propertyNames.find(pos->first) != propertyNames.end()) {
			// Propertys are unconstrained
			continue;
		}
		std::string constraint_expr;
		set<std::string> ranges = (*pos).second;
		for(set<std::string>::iterator rit=ranges.begin();rit!=ranges.end();++rit)
		{
			if (constraint_expr.length() > 0) {
				constraint_expr += "\nor\n";
			}
			std::string parameterConstraint = generateParameterConstraint(*rit, pos->first, CyPhyML::Parameter::Cast(Udm::null));
			if (parameterConstraint.length() == 0) {
				// unconstrained
				constraint_expr = "";
				break;
			}
			constraint_expr = constraint_expr + parameterConstraint;
		}

		if (constraint_expr.length() == 0) {
			continue;
		}

		DesertIface::NaturalDomain domain;
		DesertIface::NaturalMember member;
		if(!getDomainWithMember((*pos).first, domain, 0, member))
			throw udm_exception("Property/Parameter:  is not NaturalDomain. Please check the ComputationType.");
		DesertIface::VariableProperty vp = DesertIface::VariableProperty::Create(alt_delem);
		vp.parametric() = false;
		vp.name() = vp.CUSTName() = (*pos).first;
		vp.id() = vp.externalID() = altDC.ID()+5000;
		vp.PCM_STR() = "PCM_ADD";
		vp.Max() = 100000;
		vp.domain() = domain;

		bool validate = true;
		for(set<CyPhyML::DesignEntity>::iterator it=des.begin(); it!=des.end(); ++it)
		{
			CyPhyML::DesignEntity de = *it;
			if(!validateParameter(de, (*pos).first))
				validate = false;
		}

		if (!validate) {
			if (constraint_expr.length() > 0) {
				constraint_expr += "\nor\n";
			}
			constraint_expr = constraint_expr + "(" +  pos->first + "() = -1)";

		}

		DesertIface::Constraint dsConstraint = DesertIface::Constraint::Create(constraintSet);
		std::string pcon_name = pos->first + "_" + increaseCounter() + "_constraint";
		dsConstraint.name() = pcon_name;
		dsConstraint.expression() = "constraint " + pcon_name + "() {\n" + constraint_expr + "\n}";
		dsConstraint.context() = alt_delem;

	}
}

void CyPhy2Desert::insertParameterMap(const CyPhyML::Parameter &param, map<std::string, set<std::string> > &paramMap)
{
	std::string pname = param.name();
	std::string prange = param.Range();
	set<std::string> ranges;
	map<std::string, set<std::string> >::iterator pos = paramMap.find(pname);
	if(pos!=paramMap.end())
	{
		ranges = (*pos).second;
		if(ranges.find(prange)==ranges.end())
			ranges.insert(prange);
	}
	else
		ranges.insert(prange);
	paramMap[pname] = ranges;
}

void CyPhy2Desert::preprocessParameters(CyPhyML::DesignContainer &dc)
{
	set<CyPhyML::DesignContainer> dcs = dc.DesignContainer_kind_children();
	for (auto it = dcs.begin(); it != dcs.end(); ++it)
	{
		CyPhyML::DesignContainer dc_child = *it;
		if ((std::string)(dc_child.ContainerType()) == "Compound")
			continue;
		set<CyPhyML::Parameter> parameters = dc_child.Parameter_kind_children();
		preprocessParameters(parameters);
	}

	set<CyPhyML::Property> properties = dc.Property_children();
	set<std::string> propertyNames;
	for (auto it = properties.begin(); it != properties.end(); ++it)
	{
		if (propertyNames.insert(it->name()).second != true)
		{
			std::string hyperlink = "<a href=\"mga:" + UdmGme::UdmId2GmeId(it->uniqueId()) + "\">" + static_cast<std::string>(it->name()) + "</a>";

			errorMsg = std::string("Duplicate Property name \"") + hyperlink + "\"";
			throw udm_exception(std::string("Duplicate Property name \"") + static_cast<std::string>(it->name()) + "\"");
		}
	}
}

void CyPhy2Desert::preprocessParameters(set<CyPhyML::Parameter> &parameters)
{
	for(set<CyPhyML::Parameter>::iterator i=parameters.begin();i!=parameters.end();++i)
	{
		CyPhyML::Parameter currParameter = *i;
		set<CyPhyML::ValueFlow> srcvfs = currParameter.srcValueFlow();
		set<CyPhyML::ValueFlow> dstvfs = currParameter.dstValueFlow();

		if(srcvfs.empty() && dstvfs.empty())
			continue;

		std::string pname = currParameter.name();
		parameterNameMap[currParameter] = pname;
		int pid = currParameter.uniqueId();
		char buffer[65];
		_itoa_s(pid, buffer, 10);
		currParameter.name() = pname+(std::string)buffer;
	}
}

void CyPhy2Desert::postprocessParameters()
{
	for(map<CyPhyML::Parameter, std::string>::iterator it=parameterNameMap.begin();it!=parameterNameMap.end();++it)
	{
		(*it).first.name() = (*it).second;
	}
}

bool CyPhy2Desert::validateParameter(CyPhyML::DesignEntity &de, const std::string &pname)
{
	DesertIface::Element delem = com2elemMap[de];
	ASSERT(de!=Udm::null);

	set<DesertIface::VariableProperty> vps = delem.VariableProperty_kind_children();
	for(set<DesertIface::VariableProperty>::iterator it=vps.begin();it!=vps.end();++it)
	{
		std::string vpname = (*it).name();
		if(vpname == pname)
			return false;
	}

	DesertIface::NaturalDomain domain;
	DesertIface::NaturalMember member;
	if(!getDomainWithMember(pname, domain, -1, member))
		throw udm_exception("Property/Parameter:  is not NaturalDomain. Please check the ComputationType.");
	DesertIface::VariableProperty vp = DesertIface::VariableProperty::Create(delem);
	vp.parametric() = false;
	vp.name() = vp.CUSTName() = pname;
	vp.id() = vp.externalID() = de.ID()+5000;
	vp.PCM_STR() = "PCM_NONE";
	vp.Max() = 100000;
	vp.domain() =domain;

	DesertIface::AssignedValues av = DesertIface::AssignedValues::Create(delem);
	av.id() = av.externalID() = 2000;
	av.properties_end() = vp;
	av.values_end() = member;
	return true;
}

std::string CyPhy2Desert::generateVisualConstraint(const CyPhyML::DesignContainer &cyphy_container, const CyPhyML::VisualConstraint &vcon)
{
	std::string expr;

	set<CyPhyML::ImpliesEnd> implyends = vcon.ImpliesEnd_kind_children();
	map<CyPhyML::ImpliesEnd, std::string> visual2string;
	set<CyPhyML::ImpliesEnd> singleEnds;
	for(set<CyPhyML::ImpliesEnd>::iterator ei=implyends.begin();ei!=implyends.end();++ei)
	{
		CyPhyML::ImpliesEnd iend = *ei;
		std::string context = generateConstraint(*ei, cyphy_container);

		if(context.empty()) continue;

		visual2string[iend] = context;
		set<CyPhyML::Implies> ims_src = iend.dstImplies();
		set<CyPhyML::Implies> ims_dst = iend.srcImplies();
		if(ims_src.empty() && ims_dst.empty())
			singleEnds.insert(iend);
	}

	set<CyPhyML::Implies> implies = vcon.Implies_kind_children();
	bool mul = implies.size()>1 ? true : false;
	set<CyPhyML::Implies>::iterator imply_it=implies.begin();
	while(imply_it!=implies.end())
	{
		CyPhyML::ImpliesEnd src = CyPhyML::ImpliesEnd::Cast((*imply_it).srcImplies_end());
		CyPhyML::ImpliesEnd dst = CyPhyML::ImpliesEnd::Cast((*imply_it).dstImplies_end());
		expr += "(";
		expr = expr + "(" + visual2string[src] + ")\nimplies\n" +"(" + visual2string[dst] + "))";
		imply_it++;
		if(mul && imply_it!=implies.end())
			expr += "\n and \n";
	}
	if(!singleEnds.empty() && !implies.empty())
		expr += "\n and \n";
	set<CyPhyML::ImpliesEnd>::iterator implyend_it = singleEnds.begin();
	while(implyend_it!=singleEnds.end())
	{
		expr =  expr + "(" + visual2string[*implyend_it] + ")";
		implyend_it++;
		if(implyend_it!=singleEnds.end())
			expr += "\n and \n";
	}

	return expr;
}

//check whether CusttomFormula/SimpleFormula is the source of SimpleFormula,
//if it is, then create DesertIface::VariableProperty for transferring the value in desert
void CyPhy2Desert::pre_processValueFormulas(const set<CyPhyML::ValueFormula> &formulas, const CyPhyML::DesignEntity &celem, DesertIface::Element &delem)
{
	for(auto it_f=formulas.begin();it_f!=formulas.end();++it_f)
	{
		CyPhyML::ValueFormula formula = *it_f;

		set<CyPhyML::ValueFlow> dst_vfs = formula.dstValueFlow();
		for(auto it_dst=dst_vfs.begin();it_dst!=dst_vfs.end();++it_dst)
		{
			CyPhyML::ValueFlowTarget dst_end = (*it_dst).dstValueFlow_end();
			if(Uml::IsDerivedFrom(dst_end.type(), CyPhyML::ValueFormula::meta))
			{
				if(Uml::IsDerivedFrom(dst_end.type(), CyPhyML::CustomFormula::meta))
					throw udm_exception("CustomFormula/SimpleFormula cannot be conneted to CustomFormula.");

				//create parametric VariableProperty
				DesertIface::VariableProperty dvp = DesertIface::VariableProperty::Create(delem);
				dvp.id() = dvp.externalID() = formula.uniqueId()+5000;
				dvp.name() = dvp.CUSTName() = UdmUtil::ExtractName(formula)+"_"+increaseCounter();
				dvp.parametric() = true;
				updatevpMap(dvp, formula, celem);
			}
		}
	}
}

void CyPhy2Desert::processSimpleFormula(const CyPhyML::SimpleFormula &sformula, DesertIface::Element &delem)
{
	//create Desert SimpleFormula
	DesertIface::SimpleFormula dsf = DesertIface::SimpleFormula::Create(CyPhy2Desert::formulaSet);
	dsf.ComputationType() = CyPhy2Desert::getDesertPCM((std::string)sformula.Method());
	dsf.context() = delem;
	dsf.name() = "SimpleFormula";
	dsf.id() = dsf.externalID() = sformula.uniqueId();

	set<CyPhyML::ValueFlow> src_vfs = sformula.srcValueFlow();
	for(auto it_src=src_vfs.begin();it_src!=src_vfs.end();++it_src)
	{
		CyPhyML::ValueFlowTarget src_end = (*it_src).srcValueFlow_end();
		Udm::Object src_end_parent = (*it_src).srcValueFlow_refport_parent();
		if(src_end_parent==Udm::null)
			src_end_parent = src_end.GetParent();

		if(!Uml::IsDerivedFrom(src_end_parent.type(), CyPhyML::DesignEntity::meta))
			continue;

		CyPhyML::DesignEntity src_end_entity = CyPhyML::DesignEntity::Cast(src_end_parent);
		DesertIface::VariableProperty dvp = getVariableProperty(src_end, src_end_entity);
		if(dvp==Udm::null)
		{
			DesertIface::Element delem_parent;
			map<CyPhyML::DesignEntity, DesertIface::Element>::iterator pos = com2elemMap.find(src_end_entity);
			if(pos!=com2elemMap.end())
				delem_parent = (*pos).second;
			else
				continue;
			dvp = DesertIface::VariableProperty::Create(delem_parent);
			updatevpMap(dvp, src_end, src_end_entity);
			dvp.name() = dvp.CUSTName() = src_end.name();;
			dvp.id() = dvp.externalID() = src_end_entity.ID()+5000;
		}
		//ASSERT(dvp!=Udm::null);// || Uml::IsDerivedFrom(src_end.type(), CyPhyML::ValueFormula::meta));
		if(dvp!=Udm::null)
			dsf.srcProperty() += dvp;
	}

	set<CyPhyML::ValueFlow> dst_vfs = sformula.dstValueFlow();
	for(auto it_dst=dst_vfs.begin();it_dst!=dst_vfs.end();++it_dst)
	{
		CyPhyML::ValueFlowTarget dst_end = (*it_dst).dstValueFlow_end();
		Udm::Object dst_end_parent = (*it_dst).dstValueFlow_refport_parent();
		if(dst_end_parent==Udm::null)
			dst_end_parent = dst_end.GetParent();

		if(!Uml::IsDerivedFrom(dst_end_parent.type(), CyPhyML::DesignEntity::meta))
			continue;

		DesertIface::VariableProperty dvp;

		if(Uml::IsDerivedFrom(dst_end.type(), CyPhyML::ValueFormula::meta))
			dvp = getVariableProperty((CyPhyML::SimpleFormula)sformula, CyPhyML::DesignEntity::Cast(dst_end_parent));
		else
			dvp = getVariableProperty(dst_end, CyPhyML::DesignEntity::Cast(dst_end_parent));

		ASSERT(dvp!=Udm::null);// || Uml::IsDerivedFrom(dst_end.type(), CyPhyML::ValueFormula::meta));
		if(dvp!=Udm::null)
		{
			dsf.property_end() += dvp;
			dvp.parametric() = true;
		}
	}
}

void CyPhy2Desert::processCustomFormula(const CyPhyML::CustomFormula &cformula, DesertIface::Element &delem, bool isAlt)
{
	string expression = cformula.Expression();
	if(expression.empty()) return;
	if(!DFUtil::IsValidExpression(expression))
		CyPhy2Desert::invalidCustomFormulas.insert(cformula);

	map<string, string> VariablePathMap;

	set<CyPhyML::ValueFlow> inputVFs = cformula.srcValueFlow();
	if (inputVFs.empty())
		return;

	for (set<CyPhyML::ValueFlow>::const_iterator ci = inputVFs.begin(); ci != inputVFs.end(); ci++)
	{
		CyPhyML::ValueFlowTarget src_vfend = ci->srcValueFlow_end();
		Udm::Object src_parent = ci->srcValueFlow_refport_parent();
		if(src_parent==Udm::null)
			src_parent = src_vfend.parent();

		std::string src_vfend_name = src_vfend.name();
		std::string FormulaVariableName = ci->FormulaVariableName();

		DesertIface::VariableProperty dvp = getVariableProperty(src_vfend, CyPhyML::DesignEntity::Cast(src_parent));
		std::string dvp_name = (std::string)dvp.name();
		if (isAlt == false && src_parent != ci->parent()) {
			std::string src_parent_name = MgaObject::Cast(src_parent).name();
			dvp_name = "children(\"" + src_parent_name + "\")." + dvp_name;
		}
		VariablePathMap[(FormulaVariableName == "" ? src_vfend_name : FormulaVariableName)] = dvp_name + "()";;
	}

	std::string expr = DFUtil::ConvertExpression(expression, VariablePathMap);
	if(expr.empty()) return;

	set<CyPhyML::ValueFlow> dst_vfs = cformula.dstValueFlow();
	for(auto it_dst=dst_vfs.begin();it_dst!=dst_vfs.end();++it_dst)
	{
		CyPhyML::ValueFlowTarget dst_end = (*it_dst).dstValueFlow_end();
		Udm::Object dst_end_parent = (*it_dst).dstValueFlow_refport_parent();
		if(dst_end_parent==Udm::null)
			dst_end_parent = dst_end.GetParent();

		if(!Uml::IsDerivedFrom(dst_end_parent.type(), CyPhyML::DesignEntity::meta))
			continue;

		DesertIface::VariableProperty dvp;
		if(Uml::IsDerivedFrom(dst_end.type(), CyPhyML::ValueFormula::meta))
			dvp = getVariableProperty((CyPhyML::CustomFormula)cformula, CyPhyML::DesignEntity::Cast(dst_end_parent));
		else
			dvp = getVariableProperty(dst_end, CyPhyML::DesignEntity::Cast(dst_end_parent));

		ASSERT(dvp!=Udm::null);

		if(dvp!=Udm::null)
		{
			DesertIface::CustomFormula dcf = DesertIface::CustomFormula::Create(CyPhy2Desert::formulaSet);
			std::string fname = (std::string)dvp.name();
			dcf.name() = fname;
			dcf.expression() = "constraint formula_"+CyPhy2Desert::increaseCounter()+"_"+fname+" () {"+expr+"}";
			dcf.property_end() += dvp;
			dcf.context() = delem;
			dvp.parametric() = true;
		}
	}
}

void CyPhy2Desert::flatternComponent(const CyPhyML::DesignEntity &celem, DesertIface::Element &delem, bool isAlt)
{
	if(Uml::IsDerivedFrom(celem.type(), CyPhyML::DesignContainer::meta))
		return;

	CyPhyML::DesignElement cyphy_de;

	CyPhyML::ComponentRef comref;
	if(Uml::IsDerivedFrom(celem.type(), CyPhyML::ComponentRef::meta))
	{
		comref = CyPhyML::ComponentRef::Cast(celem);
		cyphy_de = comref.ref();
		if(cyphy_de==Udm::null)
		{
			nullComponentRefs.insert(comref);
			return;
		}
	}
	else
		cyphy_de = CyPhyML::DesignElement::Cast(celem);

	Com2DesertElement comflattern(cyphy_de, comref, delem, true, isAlt);
	comflattern.flatternCA();

	//update the vpMap
	map<Udm::Object, DesertIface::VariableProperty>& topVpMap = comflattern.getTopVpMap();
	for(map<Udm::Object, DesertIface::VariableProperty>::iterator pos=topVpMap.begin(); pos!=topVpMap.end();++pos)
	{
		Udm::Object pobj = (*pos).first;
		DesertIface::VariableProperty dvp = (*pos).second;

		map<CyPhyML::DesignEntity, DesertIface::VariableProperty> tmpMap;
		vpMap.insert(make_pair(pobj, std::move(tmpMap))).first->second[celem] = dvp;
	}

	//update NatureDomain/CustomDomain
	map<DesertIface::VariableProperty, double>& vp2ValMap = comflattern.getVp2ValMap();
	for(map<DesertIface::VariableProperty, double>::iterator pos_d=vp2ValMap.begin();pos_d!=vp2ValMap.end();++pos_d)
	{
		DesertIface::VariableProperty dvp = (*pos_d).first;
		double val = (*pos_d).second;

		DesertIface::NaturalDomain domain;
		DesertIface::NaturalMember member;
		getDomainWithMember((std::string)dvp.name(), domain, val, member);
		dvp.domain() = domain;

		DesertIface::Element vp_elem = dvp.owner();
		DesertIface::AssignedValues av = DesertIface::AssignedValues::Create(vp_elem);
		av.id() = av.externalID() = 2000;
		av.properties_end() = dvp;
		av.values_end() = member;
	}

	map<DesertIface::VariableProperty, set<double> >& vp2ValsMap = comflattern.getVp2ValsMap();
	for(map<DesertIface::VariableProperty, set<double> >::iterator pos_ds=vp2ValsMap.begin();pos_ds!=vp2ValsMap.end();++pos_ds)
	{
		DesertIface::VariableProperty dvp = (*pos_ds).first;
		std::string dvp_name = dvp.name();
		set<double>& vals = (*pos_ds).second;
		for(set<double>::iterator d_it=vals.begin();d_it!=vals.end();++d_it)
		{
			double val = *d_it;
			DesertIface::NaturalDomain domain;
			DesertIface::NaturalMember member;
			getDomainWithMember(dvp_name, domain, val, member);
			dvp.domain() = domain;

			DesertIface::Element vp_elem = dvp.owner();
			DesertIface::AssignedValues av = DesertIface::AssignedValues::Create(vp_elem);
			av.id() = av.externalID() = 2000;
			av.properties_end() = dvp;
			av.values_end() = member;
		}
	}
}

bool CyPhy2Desert::checkDesignElementLoop(const CyPhyML::DesignEntity &entity, set<int> ids, std::string &path)
{
	int id = entity.uniqueId();
	std::string hyperlink = "<A HREF=\"mga:"+ UdmGme::UdmId2GmeId(id) + "\">" + (std::string)entity.name() + "</A>";
	set<int>::iterator pos = ids.find(id);
	if(pos!=ids.end())
	{
		path = hyperlink;
		return false;
	}
	ids.insert(id);

	set<CyPhyML::ComponentRef> comrefs;
	set<std::string> componentRefNames;
	set<CyPhyML::DesignContainer> dcs;
	set<CyPhyML::ComponentAssembly> cas;
	set<std::string> containerNames;
	if(Uml::IsDerivedFrom(entity.type(), CyPhyML::DesignContainer::meta))
	{
		CyPhyML::DesignContainer dc = CyPhyML::DesignContainer::Cast(entity);
		comrefs = dc.ComponentRef_kind_children();
		dcs = dc.DesignContainer_kind_children();
		cas = dc.ComponentAssembly_kind_children();
	}
	else if(Uml::IsDerivedFrom(entity.type(), CyPhyML::ComponentAssembly::meta))
	{
		CyPhyML::ComponentAssembly ca = CyPhyML::ComponentAssembly::Cast(entity);
		comrefs = ca.ComponentRef_kind_children();
		cas = ca.ComponentAssembly_kind_children();
	}
	else if(Uml::IsDerivedFrom(entity.type(), CyPhyML::ComponentRef::meta))
	{
		CyPhyML::ComponentRef comref = CyPhyML::ComponentRef::Cast(entity);
		comrefs.insert(comref);
	}

	for(auto ref_it=comrefs.begin();ref_it!=comrefs.end();++ref_it)
	{
		const CyPhyML::ComponentRef& this_ref = *ref_it;
		CyPhyML::DesignElement de = this_ref.ref();
		if (!de)
		{
			throw udm_exception("Illegal null reference " + this_ref.getPath("/"));
		}
		if(!checkDesignElementLoop(de, ids,path))
		{
			path = hyperlink + "-->" + path;
			return false;
		}
		if (componentRefNames.insert(this_ref.name()).second == false) {
			throw udm_exception(std::string("Illegal duplicate name ") + static_cast<const std::string>(this_ref.name()) + " in " + entity.getPath("/"));
		}
	}
	for(auto dc_it=dcs.begin();dc_it!=dcs.end();++dc_it)
	{
		const CyPhyML::DesignContainer& this_dc = *dc_it;
		if(!checkDesignElementLoop(this_dc, ids,path))
		{
			path = hyperlink + "-->" + path;
			return false;
		}
		if (containerNames.insert(this_dc.name()).second == false) {
			throw udm_exception(std::string("Illegal duplicate name ") + static_cast<const std::string>(this_dc.name()) + " in " + entity.getPath("/"));
		}
	}
	for(auto ca_it=cas.begin();ca_it!=cas.end();++ca_it)
	{
		const CyPhyML::ComponentAssembly& this_ca = *ca_it;
		if(!checkDesignElementLoop(this_ca, ids,path))
		{
			path = hyperlink + "-->" + path;
			return false;
		}
		if (containerNames.insert(this_ca.name()).second == false) {
			throw udm_exception(std::string("Illegal duplicate name ") + static_cast<const std::string>(this_ca.name()) + " in " + entity.getPath("/"));
		}
	}
	return true;
}

int Com2DesertElement::_cnt = 0;
map<DesertIface::VariableProperty, double> Com2DesertElement::_vp2ValMap;
map<DesertIface::VariableProperty, set<double> > Com2DesertElement::_vp2ValsMap;
set<std::string> Com2DesertElement::_vpNameSet;

Com2DesertElement::Com2DesertElement(const CyPhyML::DesignElement &cyphy_de, const CyPhyML::ComponentRef &comref,
									DesertIface::Element &delem, bool isroot, bool isAlt)
									:_com(cyphy_de), _comref(comref), _delem(delem), _isroot(isroot), _isAlt(isAlt)
{
	_vfs = _com.ValueFlow_kind_children();
	if(_isroot)
		init();
}

void Com2DesertElement::flatternCA()
{
	set<CyPhyML::Property> properties = _com.Property_kind_children();
	for(auto it_prop=properties.begin(); it_prop!=properties.end(); ++it_prop)
	{
		processProperty(*it_prop);
	}

	set<CyPhyML::Parameter> parameters = _com.Parameter_kind_children();
	for(auto it_para=parameters.begin(); it_para!=parameters.end();++it_para)
	{
		processParameter(*it_para);
	}

	set<CyPhyML::Constant> constants = _com.Constant_kind_children();
	for(auto it_con=constants.begin(); it_con!=constants.end(); ++it_con)
	{
		processConstant(*it_con);
	}

	if(Uml::IsDerivedFrom(_com.type(), CyPhyML::ComponentAssembly::meta))
	{
		CyPhyML::ComponentAssembly ca = CyPhyML::ComponentAssembly::Cast(_com);
		set<CyPhyML::Component> coms = ca.Component_kind_children();
		for(auto it_com=coms.begin(); it_com!=coms.end(); ++it_com)
		{
			CyPhyML::ComponentRef cref;
			Com2DesertElement com2d(*it_com, cref, _delem, false, _isAlt);
			com2d.flatternCA();
			updateInnerVpMap(*it_com, com2d.getTopVpMap());
		}

		set<CyPhyML::ComponentRef> comrefs = ca.ComponentRef_kind_children();
		for(auto it_comref=comrefs.begin(); it_comref!=comrefs.end(); ++it_comref)
		{
			CyPhyML::ComponentRef cref = *it_comref;
			Com2DesertElement comref2d(cref.ref(), cref, _delem, false, _isAlt);
			comref2d.flatternCA();
			updateInnerVpMap(cref, comref2d.getTopVpMap());
		}

		set<CyPhyML::ComponentAssembly> cas = ca.ComponentAssembly_kind_children();
		for(auto it_ca=cas.begin(); it_ca!=cas.end(); ++it_ca)
		{
			CyPhyML::ComponentRef cref;
			Com2DesertElement ca2d(*it_ca, cref, _delem, false, !_isAlt);
			ca2d.flatternCA();
			updateInnerVpMap(*it_ca, ca2d.getTopVpMap());
		}
	}

	//pre-process SimpleFormula/CustomFormula to see where it is the src for another SimpleFormula,
	//if it is, create DesertIface::VariableProperty
	set<CyPhyML::CustomFormula> cformulas = _com.CustomFormula_kind_children();
	for(auto it_cf=cformulas.begin();it_cf!=cformulas.end();++it_cf)
	{
		processFormula(*it_cf);
	}

	set<CyPhyML::SimpleFormula> sformulas = _com.SimpleFormula_kind_children();
	for(auto it_sf=sformulas.begin();it_sf!=sformulas.end();++it_sf)
	{
		processFormula(*it_sf);
	}

	//processSimpleFormulas and other direct ValueFlows
	for(auto it_cf=cformulas.begin();it_cf!=cformulas.end();++it_cf)
	{
		processCustomFormula(*it_cf);
	}
	for(auto it_sf=sformulas.begin();it_sf!=sformulas.end();++it_sf)
	{
		processSimpleFormula(*it_sf);
	}
	for(auto it_vf=_vfs.begin(); it_vf!=_vfs.end(); ++it_vf)
	{
		processDirectValueFlow(*it_vf);
	}

	if(!_isroot) return;


	//process Connectors
	set<CyPhyML::Connector> connectors = _com.Connector_kind_children();
	set<std::string> uniqueConnectorNames;
	for(auto it_si=connectors.begin();it_si!=connectors.end();++it_si)
	{
		CyPhyML::Connector connector= *it_si;
		std::string si_name = connector.name();
		std::string si_valid_name = checkInvalidName(si_name);
		if(uniqueConnectorNames.find(si_valid_name)!=uniqueConnectorNames.end())
		{
			int cnt = uniqueConnectorNames.size();
			char buffer[65];
			_itoa_s(cnt, buffer, 10);
			si_valid_name = si_valid_name+"_"+(std::string)buffer;
		}
		uniqueConnectorNames.insert(si_valid_name);
		if(si_valid_name!=si_name)
		{
			connector.name() = si_valid_name;
			CyPhy2Desert::originalNames[connector] = si_name;
		}
		std::string def = connector.Definition();
		set<int> def_vals = CyPhy2Desert::getConnectorDefValue(def);
		if(def_vals.size()==1)
		{
			int def_val = *(def_vals.begin());
			DesertIface::VariableProperty dvp = createDesertVP(connector, def_val, false);
		}
		else if(def_vals.size()>1)
		{
			createDesertVP4WildCard(connector, def_vals);
		}
	}
}

map<Udm::Object, DesertIface::VariableProperty>& Com2DesertElement::getTopVpMap()
{
	return _topVpMap;
}

map<DesertIface::VariableProperty, double>& Com2DesertElement::getVp2ValMap()
{
	return _vp2ValMap;
}

map<DesertIface::VariableProperty, set<double> >& Com2DesertElement::getVp2ValsMap()
{
	return _vp2ValsMap;
}

void Com2DesertElement::clearVp2ValMap()
{
	_vp2ValMap.clear();
}
void Com2DesertElement::clearVp2ValsMap()
{
	_vp2ValsMap.clear();
}

void Com2DesertElement::init()
{
	_cnt = 0;
	_vp2ValMap.clear();
	_vpNameSet.clear();
}

DesertIface::VariableProperty Com2DesertElement::createDesertVP(const Udm::Object &cyphy_vp, double value, bool parametric)
{
	std::string pname = UdmUtil::ExtractName(cyphy_vp);

	set<std::string>::iterator it = _vpNameSet.find(pname);
	if(it!=_vpNameSet.end())
	{
		char buffer[65];
		_itoa_s(_cnt++, buffer, 10);
		pname = pname + (std::string)buffer;
	}
	_vpNameSet.insert(pname);

	int pid = cyphy_vp.uniqueId();

	DesertIface::VariableProperty newdvp = DesertIface::VariableProperty::Create(_delem);

	newdvp.id() = newdvp.externalID() = pid+5000;
	newdvp.name() = newdvp.CUSTName() = pname;
	newdvp.PCM_STR() = "PCM_NONE";
	newdvp.Max() = value+100000;
	newdvp.parametric() = parametric;

	if(!parametric)
	{
		Com2DesertElement::_vp2ValMap[newdvp] = value;
	}
	_topVpMap[cyphy_vp] = newdvp;

	return newdvp;
}

void Com2DesertElement::createDesertVP4WildCard(const Udm::Object &si, set<int> vals)
{
	std::string pname = UdmUtil::ExtractName(si);
	set<std::string>::iterator it = _vpNameSet.find(pname);
	if(it!=_vpNameSet.end())
	{
		char buffer[65];
		_itoa_s(_cnt++, buffer, 10);
		pname = pname + (std::string)buffer;
	}
	_vpNameSet.insert(pname);

	int pid = si.uniqueId();
	DesertIface::VariableProperty newdvp = DesertIface::VariableProperty::Create(_delem);
	newdvp.id() = newdvp.externalID() = pid+5000;
	newdvp.name() = newdvp.CUSTName() = pname;
	newdvp.PCM_STR() = "";
	newdvp.parametric() = false;
	_topVpMap[si] = newdvp;
	set<double> tmp_vals;
	for(set<int>::iterator it=vals.begin();it!=vals.end();++it)
		tmp_vals.insert(*it);
	Com2DesertElement::_vp2ValsMap[newdvp] = tmp_vals;
}

void Com2DesertElement::processProperty(const CyPhyML::Property &prop)
{
	//set<CyPhyML::ValueFlow> src_vfs = prop.srcValueFlow();
	//bool parametric = src_vfs.empty() ? false : true;
	//not complete here, need more consideration, even if src_vfs is not empty,
	//it may come from somewhere else due to the reference port case
	CyPhyML::ValueFlowTarget src_end;
	Udm::Object src_end_parent;
	bool parametric = isVariableParametric(prop, src_end, src_end_parent);

	set<CyPhyML::ValueFlow> dst_vfs = prop.dstValueFlow();
	if(!_isroot && !parametric && dst_vfs.empty())
		return;          //stand alone property

	if((std::string)prop.ComputationType()=="Cust")
		return;

	std::string val_str = prop.Value();
	double value = atof(val_str.c_str());
	DesertIface::VariableProperty dvp = createDesertVP(prop, value, parametric);

	if(src_end==Udm::null) return;

	if (Uml::IsDerivedFrom(src_end.type(), CyPhyML::Parameter::meta)
		|| Uml::IsDerivedFrom(src_end.type(), CyPhyML::Property::meta)
		|| Uml::IsDerivedFrom(src_end.type(), CyPhyML::Constant::meta)
		)
	{
		DesertIface::CustomFormula dcf = DesertIface::CustomFormula::Create(CyPhy2Desert::formulaSet);
		std::string fname = dvp.name();
		dcf.name() = fname;
		dcf.expression() = "constraint formula_"+CyPhy2Desert::increaseCounter()+"_"+fname+" () {"+DFUtil::getRelativePath(src_end, src_end_parent, _comref!=Udm::null?_comref:_com)+std::string(src_end.name())+"() }";
		dcf.property_end() += dvp;
		dcf.context() = _delem;
	}
}

void Com2DesertElement::processParameter(const CyPhyML::Parameter &param)
{
	//set<CyPhyML::ValueFlow> src_vfs = param.srcValueFlow();
	//bool parametric = src_vfs.empty() ? false : true;
	CyPhyML::ValueFlowTarget src_end;
	Udm::Object src_end_parent;
	bool parametric = isVariableParametric(param, src_end, src_end_parent);

	set<CyPhyML::ValueFlow> dst_vfs = param.dstValueFlow();
	if(!_isroot && !parametric && dst_vfs.empty() && param.Disable())
		return;

	std::string val_str = param.Value();
	double value = atof(val_str.c_str());
	DesertIface::VariableProperty dvp = createDesertVP(param, value, parametric);
	std::string newParamName = dvp.name();

	if(_isroot && !param.Disable() && !_isAlt)
	{
		//generate constraints
		std::string expr = CyPhy2Desert::generateParameterConstraint(param, newParamName);
		if(!(expr=="()" || expr.empty()))
		{
			DesertIface::Constraint dsConstraint = DesertIface::Constraint::Create(CyPhy2Desert::constraintSet);
			std::string pcon_name = newParamName +"_"+CyPhy2Desert::increaseCounter()+"_constraint";
			dsConstraint.name() = pcon_name;
			dsConstraint.expression() = "constraint "+ pcon_name+"() {\n" + expr+"\n}";
			dsConstraint.context() = _delem;
		}
	}

	if(src_end==Udm::null) return;

	if (Uml::IsDerivedFrom(src_end.type(), CyPhyML::Parameter::meta)
		|| Uml::IsDerivedFrom(src_end.type(), CyPhyML::Property::meta)
		|| Uml::IsDerivedFrom(src_end.type(), CyPhyML::Constant::meta)
		)
	{
		DesertIface::CustomFormula dcf = DesertIface::CustomFormula::Create(CyPhy2Desert::formulaSet);
		std::string fname = dvp.name();
		dcf.name() = fname;
		dcf.expression() = "constraint formula_"+CyPhy2Desert::increaseCounter()+"_"+fname+" () {"+DFUtil::getRelativePath(src_end, src_end_parent, _comref!=Udm::null?_comref:_com)+std::string(src_end.name())+"() }";
		dcf.property_end() += dvp;
		dcf.context() = _delem;
	}
}

void Com2DesertElement::processConstant(const CyPhyML::Constant &constant)
{
	set<CyPhyML::ValueFlow> src_vfs = constant.srcValueFlow(); //shoud not have incoming valueflows
	set<CyPhyML::ValueFlow> dst_vfs = constant.dstValueFlow();
	if(!_isroot && src_vfs.empty() && dst_vfs.empty())
		return;

	double value = constant.ConstantValue();
	DesertIface::VariableProperty dvp = createDesertVP(constant, value, false);
}

void Com2DesertElement::processFormula(const CyPhyML::ValueFormula &formula)
{
	set<CyPhyML::ValueFlow> dst_vfs = formula.dstValueFlow();
	for(auto it_dst=dst_vfs.begin();it_dst!=dst_vfs.end();++it_dst)
	{
		CyPhyML::ValueFlowTarget dst_end = (*it_dst).dstValueFlow_end();
		if(Uml::IsDerivedFrom(dst_end.type(), CyPhyML::ValueFormula::meta))
		{
			if(Uml::IsDerivedFrom(dst_end.type(), CyPhyML::CustomFormula::meta))
				throw udm_exception("CustomFormula/SimpleFormula cannot be conneted to CustomFormula.");
			DesertIface::VariableProperty dvp = createDesertVP(formula, 0, true);
			return;
		}
	}
}

void Com2DesertElement::processSimpleFormula(const CyPhyML::SimpleFormula &sformula)
{
	//create Desert SimpleFormula
	DesertIface::SimpleFormula dsf = DesertIface::SimpleFormula::Create(CyPhy2Desert::formulaSet);
	dsf.ComputationType() = CyPhy2Desert::getDesertPCM((std::string)sformula.Method());
	dsf.context() = _delem;
	dsf.name() = "SimpleFormula";
	dsf.id() = dsf.externalID() = sformula.uniqueId();

	set<CyPhyML::ValueFlow> src_vfs = sformula.srcValueFlow();
	for(auto it_src=src_vfs.begin();it_src!=src_vfs.end();++it_src)
	{
		set<CyPhyML::ValueFlow>::iterator it_vf = _vfs.find(*it_src);
		if(it_vf!=_vfs.end())
			_vfs.erase(it_vf);

		CyPhyML::ValueFlowTarget src_end = (*it_src).srcValueFlow_end();
		Udm::Object src_end_parent = (*it_src).srcValueFlow_refport_parent();
		if(src_end_parent==Udm::null)
			src_end_parent = src_end.GetParent();

		if(!Uml::IsDerivedFrom(src_end_parent.type(), CyPhyML::DesignEntity::meta))
			continue;

		DesertIface::VariableProperty dvp = getVariableProperty(src_end, CyPhyML::DesignEntity::Cast(src_end_parent));
		ASSERT(dvp!=Udm::null);// || Uml::IsDerivedFrom(src_end.type(), CyPhyML::ValueFormula::meta));
		if(dvp!=Udm::null)
			dsf.srcProperty() += dvp;
	}

	set<CyPhyML::ValueFlow> dst_vfs = sformula.dstValueFlow();
	for(auto it_dst=dst_vfs.begin();it_dst!=dst_vfs.end();++it_dst)
	{
		set<CyPhyML::ValueFlow>::iterator it_vf = _vfs.find(*it_dst);
		if(it_vf!=_vfs.end())
			_vfs.erase(it_vf);

		CyPhyML::ValueFlowTarget dst_end = (*it_dst).dstValueFlow_end();
		Udm::Object dst_end_parent = (*it_dst).dstValueFlow_refport_parent();
		if(dst_end_parent==Udm::null)
			dst_end_parent = dst_end.GetParent();

		if(!Uml::IsDerivedFrom(dst_end_parent.type(), CyPhyML::DesignEntity::meta))
			continue;

		DesertIface::VariableProperty dvp;

		if(Uml::IsDerivedFrom(dst_end.type(), CyPhyML::ValueFormula::meta))
			dvp = getVariableProperty((CyPhyML::SimpleFormula)sformula, CyPhyML::DesignEntity::Cast(dst_end_parent));
		else
			dvp = getVariableProperty(dst_end, CyPhyML::DesignEntity::Cast(dst_end_parent));

		ASSERT(dvp!=Udm::null);// || Uml::IsDerivedFrom(dst_end.type(), CyPhyML::ValueFormula::meta));
		if(dvp!=Udm::null)
		{
			dsf.property_end() += dvp;
			dvp.parametric() = true;
		}
	}
}

void Com2DesertElement::processCustomFormula(const CyPhyML::CustomFormula &cformula)
{
	string expression = cformula.Expression();
	if(!DFUtil::IsValidExpression(expression))
		CyPhy2Desert::invalidCustomFormulas.insert(cformula);
	map<string, string> VariablePathMap;

	set<CyPhyML::ValueFlow> inputVFs = cformula.srcValueFlow();

	if (inputVFs.empty())
		return;

	for (set<CyPhyML::ValueFlow>::const_iterator ci = inputVFs.begin(); ci != inputVFs.end(); ci++)
	{
		set<CyPhyML::ValueFlow>::iterator it_vf = _vfs.find(*ci);
		if(it_vf!=_vfs.end())
			_vfs.erase(it_vf);

		CyPhyML::ValueFlowTarget src_vfend = ci->srcValueFlow_end();
		Udm::Object src_parent = ci->srcValueFlow_refport_parent();
		if(src_parent==Udm::null)
			src_parent = src_vfend.parent();

		std::string src_vfend_name = src_vfend.name();
		std::string FormulaVariableName = ci->FormulaVariableName();

		DesertIface::VariableProperty dvp = getVariableProperty(src_vfend, CyPhyML::DesignEntity::Cast(src_parent));
		VariablePathMap[(FormulaVariableName == "" ? src_vfend_name : FormulaVariableName)] = (std::string)dvp.name()+"()";;
	}

	std::string expr = DFUtil::ConvertExpression(expression, VariablePathMap);
	if(expr.empty()) return;

	set<CyPhyML::ValueFlow> dst_vfs = cformula.dstValueFlow();
	for(auto it_dst=dst_vfs.begin();it_dst!=dst_vfs.end();++it_dst)
	{
		set<CyPhyML::ValueFlow>::iterator it_vf = _vfs.find(*it_dst);
		if(it_vf!=_vfs.end())
			_vfs.erase(it_vf);

	//	if(expr.empty()) continue;

		CyPhyML::ValueFlowTarget dst_end = (*it_dst).dstValueFlow_end();
		Udm::Object dst_end_parent = (*it_dst).dstValueFlow_refport_parent();
		if(dst_end_parent==Udm::null)
			dst_end_parent = dst_end.GetParent();

		if(!Uml::IsDerivedFrom(dst_end_parent.type(), CyPhyML::DesignEntity::meta))
			continue;

		DesertIface::VariableProperty dvp;
		if(Uml::IsDerivedFrom(dst_end.type(), CyPhyML::ValueFormula::meta))
			dvp = getVariableProperty((CyPhyML::CustomFormula)cformula, CyPhyML::DesignEntity::Cast(dst_end_parent));
		else
			dvp = getVariableProperty(dst_end, CyPhyML::DesignEntity::Cast(dst_end_parent));

		ASSERT(dvp!=Udm::null);

		if(dvp!=Udm::null)
		{
			DesertIface::CustomFormula dcf = DesertIface::CustomFormula::Create(CyPhy2Desert::formulaSet);
			std::string fname = (std::string)dvp.name();
			dcf.name() = fname;
			dcf.expression() = "constraint formula_"+CyPhy2Desert::increaseCounter()+"_"+fname+" () {"+expr+"}";
			dcf.property_end() += dvp;
			dcf.context() = _delem;
			dvp.parametric() = true;
		}
	}
}

void Com2DesertElement::processDirectValueFlow(const CyPhyML::ValueFlow &vf)
{
	CyPhyML::ValueFlowTarget dst_end = vf.dstValueFlow_end();
	Udm::Object dst_end_parent = vf.dstValueFlow_refport_parent();
	if(dst_end_parent==Udm::null)
		dst_end_parent = dst_end.GetParent();

	DesertIface::VariableProperty dst_dvp = getVariableProperty(dst_end, CyPhyML::DesignEntity::Cast(dst_end_parent));
	ASSERT(dst_dvp!=Udm::null);

	CyPhyML::ValueFlowTarget src_end = vf.srcValueFlow_end();
	Udm::Object src_end_parent = vf.srcValueFlow_refport_parent();
	if(src_end_parent==Udm::null)
		src_end_parent = src_end.GetParent();

	DesertIface::VariableProperty src_dvp = getVariableProperty(src_end, CyPhyML::DesignEntity::Cast(src_end_parent));
	ASSERT(src_dvp!=Udm::null);

	if(dst_dvp!=Udm::null && src_dvp!=Udm::null)
	{
		DesertIface::CustomFormula dcf = DesertIface::CustomFormula::Create(CyPhy2Desert::formulaSet);
		std::string fname = (std::string)dst_dvp.name();
		dcf.name() = fname;
		dcf.expression() = "constraint formula_"+CyPhy2Desert::increaseCounter()+"_"+fname+" () {"+(std::string)src_dvp.name()+"()}";
		dcf.property_end() += dst_dvp;
		dcf.context() = _delem;
		dst_dvp.parametric() = true;
	}
}

void Com2DesertElement::updateInnerVpMap(const CyPhyML::DesignEntity &cyphy_elem, map<Udm::Object, DesertIface::VariableProperty> &topVpMap)
{
	for(map<Udm::Object, DesertIface::VariableProperty>::iterator pos = topVpMap.begin(); pos != topVpMap.end(); ++pos)
	{
		const Udm::Object& pobj = (*pos).first;

		if(Uml::IsDerivedFrom(pobj.type(), CyPhyML::ValueFormula::meta))
			continue;

		DesertIface::VariableProperty dvp = (*pos).second;

		map<CyPhyML::DesignEntity, DesertIface::VariableProperty> tmpMap;
		_innerVpMap.insert(make_pair(pobj, std::move(tmpMap))).first->second[cyphy_elem] = dvp;
	}
}

DesertIface::VariableProperty Com2DesertElement::getVariableProperty(CyPhyML::ValueFlowTarget &cyphy_obj, CyPhyML::DesignEntity &obj_parent)
{
	DesertIface::VariableProperty dvp;
	VPMap::iterator vpMap_pos = _innerVpMap.find(cyphy_obj);
	if(vpMap_pos!=_innerVpMap.end())
	{
		map<CyPhyML::DesignEntity, DesertIface::VariableProperty>& tmpMap = (*vpMap_pos).second;
		map<CyPhyML::DesignEntity, DesertIface::VariableProperty>::iterator tmpMap_pos = tmpMap.find(obj_parent);
		if(tmpMap_pos!=tmpMap.end())
			dvp = (*tmpMap_pos).second;
	}
	else if(obj_parent == _com)
	{
		map<Udm::Object, DesertIface::VariableProperty>::iterator pos = _topVpMap.find(cyphy_obj);
		if(pos!=_topVpMap.end())
			dvp = (*pos).second;
	}
	return dvp;
}

bool Com2DesertElement::isVariableParametric(const CyPhyML::ValueFlowTarget &var, CyPhyML::ValueFlowTarget &src_end, Udm::Object &src_end_parent)
{
	set<CyPhyML::ValueFlow> src_vfs = var.srcValueFlow();
	if(src_vfs.empty()) return false;

	set<CyPhyML::ValueFlow> real_vfs;
	for(auto it=src_vfs.begin();it!=src_vfs.end();++it)
	{
		CyPhyML::ValueFlow src_vf = *it;
		CyPhyML::ValueFlowTarget dst_end = src_vf.dstValueFlow_end();
		CyPhyML::ValueFlow_dstValueFlow_RPContainer_Base dst_end_refParent = src_vf.dstValueFlow_refport_parent();
		if(dst_end_refParent!=Udm::null && _comref!=Udm::null && dst_end_refParent.uniqueId()==_comref.uniqueId())
		{
			real_vfs.insert(src_vf);
		}
		else if(dst_end_refParent==Udm::null && _com.uniqueId() == dst_end.GetParent().uniqueId())// && _comref==Udm::null)
		{
			 //same component
			real_vfs.insert(src_vf);
		}

	}

	if(real_vfs.empty()) return false;

	if(real_vfs.size()>1)
		throw udm_exception("Property/Parameter: "+(std::string)var.name()+" cannot have more than one source ValueFlow.");

	CyPhyML::ValueFlow real_vf = *(real_vfs.begin());

	Udm::Object topParent;
	if(_comref!=Udm::null)
		topParent = _comref.GetParent();
	else
		topParent = _com.GetParent();

	if(_isroot && real_vf.GetParent()==topParent)
	{
		src_end = real_vf.srcValueFlow_end();
		src_end_parent = real_vf.srcValueFlow_refport_parent();
	}

	return true;
}

//set<int> CyPhy2Desert::getStructuralInterfaceTypeValue(std::string &si_type)
//{
//	set<int> ret;
//	map<std::string, int>::iterator pos = structuralInterfaceTypeMap.find(si_type);
//	if(pos!=structuralInterfaceTypeMap.end())
//	{
//		ret.insert((*pos).second);
//	}
//	else
//	{
//		map<std::string, set<int> >::iterator pos1 = sitWildCardMap.find(si_type);
//		if(pos1!=sitWildCardMap.end())
//			ret = (*pos1).second;
//	}
//	return ret;
//}

set<int> CyPhy2Desert::getConnectorDefValue(std::string &def)
{
	set<int> ret;
	map<std::string, int>::iterator pos = connectorDefMap.find(def);
	if(pos!=connectorDefMap.end())
	{
		ret.insert((*pos).second);
	}
	else
	{
		map<std::string, set<int> >::iterator pos1 = connectorDefWildCardMap.find(def);
		if(pos1!=connectorDefWildCardMap.end())
			ret = (*pos1).second;
	}
	return ret;
}
#endif