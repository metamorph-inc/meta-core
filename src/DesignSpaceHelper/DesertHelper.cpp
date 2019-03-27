#include "DesertHelper.h"
#include "CyPhy2desert.h"
#include "Desert2Cyphy.h"

#include "UdmUtil.h"
#include "Uml.h"
#include "UdmGme.h"

#include <fstream>
#include <sstream>

#include <codecvt>
#include <locale>

#include "StatusDialog.h"
#include "DesertStatusDlg.h"

#include <direct.h>
#include "UdmConsole.h"
#include "CyPhyUtil.h"
#include "time.h"
#include "DesertThread.h"

using namespace DesertIface;

//typedef map<DesertIfaceBack::Configuration, set<CyPhyML::DesignEntity>*> MorphMatrix;
//MorphMatrix morphMatrix;
//set<CyPhyML::DesignEntity> allEntities;
//string mmFilename = "configuration_list";
//int runID = 0;
//FILE* mmfd = 0;
//bool morphMatrixInitialized1 = false;
std::string capitalizeFirstLetter(const std::string& str)
{
	std::string ret=str;

	if (isalpha(str[0]))
		ret[0] = toupper(str[0]);

	int i=1;
	while (str[i])
	{
		if (isalpha(str[i]))
			ret[i] = tolower(str[i]);
		i++;
	}
	return ret;
}

std::string dropExtension( const std::string& filename)
{
	std::string nameNoExt( filename);
	std::replace(nameNoExt.begin(), nameNoExt.end(), '/', '\\');
	std::string::size_type findExt= nameNoExt.rfind( '.');
	if ( findExt != std::string::npos)
		if( *(nameNoExt.begin()+ findExt+ 1) != '\\')
			nameNoExt.erase( nameNoExt.begin()+ findExt, nameNoExt.end());	// delete '.' too.
	return nameNoExt;
}

std::string dropPath( const std::string& filepath)
{
	std::string filename( filepath);
	std::replace(filename.begin(), filename.end(), '/', '\\');
	std::string::size_type findPath= filename.rfind( '\\');
	if ( findPath != std::string::npos)
		filename.erase( filename.begin(), filename.begin()+ ++findPath);	// delete '\' too.
	return filename;
}

std::string getPath( const std::string& filepath, bool deleteTerminatingBackSlash)
{
	std::string filename( filepath);
	std::replace(filename.begin(), filename.end(), '/', '\\');
	std::string::size_type findPath= filename.rfind( '\\');
	if ( findPath != std::string::npos)
		filename.erase( filename.begin()+ ( deleteTerminatingBackSlash ? findPath : ++findPath), filename.end());	// do not delete terminating '\'.
	else
		filename.erase();
	return filename;
}

string wstringToString(const std::wstring& wstr)
{
	using convert_typeX = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_typeX, wchar_t> converterX;

	return converterX.to_bytes(wstr);
}

wstring stringToWstring(const std::string& str)
{
	using convert_typeX = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_typeX, wchar_t> converterX;

	return converterX.from_bytes(str);
}

void DesertHelper::morphMatrixInitialize1() {
	if(morphMatrixInitialized1) {
		return;
	}

	// MorphMatrix initialize
	morphMatrix.clear();
	allEntities.clear();
	stringstream newMMFilenameSS;
	if(runID > 0) {
		newMMFilenameSS << mmFilename << runID << ".txt";
	} else {
		newMMFilenameSS << mmFilename << ".txt";
	}
	mmfd = fopen(newMMFilenameSS.str().c_str(), "w+");

	// runID++;
	morphMatrixInitialized1 = true;
}

void DesertHelper::morphMatrixFinalize1() {
	if(!morphMatrixInitialized1) {
		return;
	}

	morphMatrixInitialized1 = false;

	// MorphMatrix finalize
	fprintf(mmfd, "Configurations/Options");
	for(set<CyPhyML::DesignEntity>::iterator aePos = allEntities.begin(); aePos != allEntities.end(); ++aePos) {
		CyPhyML::DesignEntity e = *aePos;
		long eID = e.ID();
		fprintf(mmfd, "\t%ld", eID);
	}
	fprintf(mmfd, "\n");
	for(set<CyPhyML::DesignEntity>::iterator aePos1 = allEntities.begin(); aePos1 != allEntities.end(); ++aePos1) {
		CyPhyML::DesignEntity e = *aePos1;
		string eName = e.name();
		fprintf(mmfd, "\t%s", eName.c_str());
	}
	fprintf(mmfd, "\n");
	for(MorphMatrix::iterator mmPos = morphMatrix.begin(); mmPos != morphMatrix.end(); ++mmPos) {
		MorphMatrix::value_type& mmRow = *mmPos;
		Configuration cfg = mmRow.first;
		auto& selectedEntities = mmRow.second;
		long cfgID = cfg.uniqueId();
		string cfgName = cfg.name();
		cfgName = "cfg" + cfgName.substr(10);
		fprintf(mmfd, "%ld_%s", cfgID, cfgName.c_str());

		set<CyPhyML::DesignEntity>::iterator aePos = allEntities.begin();
		while(aePos != allEntities.end()) {
			CyPhyML::DesignEntity e = *aePos;
			set<CyPhyML::DesignEntity>::iterator sePos = selectedEntities->find(e);
			if(sePos != selectedEntities->end()) {
				// Selected
				fprintf(mmfd, "\t1");

			} else {
				// Not selected
				fprintf(mmfd, "\t0");
			}

//////////////////////////////////////////////////////////////////////////////////
// Himanshu: Commenting out the new format
//////////////////////////////////////////////////////////////////////////////////
//
//			std::string info;
//			map<int, CyPhyML::DesignEntity>::iterator mopos = moptionMap.find(e.ID());
//			if(mopos!=moptionMap.end())
//				info = "`@`" + (std::string)(*mopos).second.name();
//
//			map<int, CyPhyML::DesignEntity>::iterator mgpos = mgroupMap.find(e.ID());
//			if(mgpos!=mgroupMap.end())
//				info = info + "`@`" + (std::string)(*mgpos).second.name();
//
//			if(!info.empty())
//				fprintf(mmfd,"%s",info.c_str());
//////////////////////////////////////////////////////////////////////////////////

			++aePos;
		}
		fprintf(mmfd, "\n");
	}
	fflush(mmfd);
	fclose(mmfd);
	mmfd = 0;
	morphMatrix.clear();
	allEntities.clear();
}

DesertHelper::DesertHelper(const std::string &mgaFile, Udm::DataNetwork& cyphyDN, Udm::Object focusObject)
	: cyphy_dn(cyphyDN), currObj(focusObject),desertFinit_2_fail(false), showGui(true)
{
	std::string currPath = getPath(mgaFile, false);
	std::string logPath = currPath + "log\\";
	CreateDirectoryA(logPath.c_str(), NULL);
	currPath += "tmp\\";
	CreateDirectoryA(currPath.c_str(), NULL);

	std::string fileBase = dropExtension(dropPath(mgaFile));

#ifdef IFACE_FILE_FORMAT_XML
	desertIfaceFile = currPath + fileBase + "_Desert_tmp.xml";
	desertIfaceBackFile = currPath + fileBase +"_Desert_back_tmp.xml";
#else
	desertIfaceFile = currPath + fileBase + "_Desert_tmp.mem";
	desertIfaceBackFile = currPath + fileBase + "_Desert_back_tmp.mem";
#endif
	desertlogFile = logPath + fileBase;
	mmFilename = "configuration_list";
	runID = 0;
	mmfd = 0;
	morphMatrixInitialized1 = false;
}

DesertHelper::~DesertHelper()
{
	closeDesertLogFile();

	closeDesertManageInstace();
}

void DesertHelper::close()
{
	//reverse the DesignEntity ID
	for(auto it = CyPhy2Desert::originalIDs.begin(); it != CyPhy2Desert::originalIDs.end(); ++it)
	{
		(*it).first.ID() = (*it).second;
	}
	CyPhy2Desert::originalIDs.clear();

	if(cfg_models!=Udm::null)
	{
		set<CyPhyML::CWC> cwcs = cfg_models.CWC_kind_children();
		if(cwcs.empty())
			cfg_models.DeleteObject();
	}

	//removed unsaved new visualconstraints
	for(map<int, DesertIface::Constraint>::iterator it=newConstraintMap.begin(); it!=newConstraintMap.end();++it)
	{
		DesertIface::Constraint dcon = (*it).second;
		map<DesertIface::Constraint, CyPhyML::ConstraintBase>::iterator map_pos = dcon2CyphyConMap.find(dcon);
		if(map_pos!=dcon2CyphyConMap.end())
		{
			CyPhyML::ConstraintBase ccon = (*map_pos).second;
			ccon.DeleteObject();
			dcon2CyphyConMap.erase(map_pos);
		}
	}
	newConstraintMap.clear();

	closeDesertIfaceDN();
	closeDesertIfaceBackDN();
}

void DesertHelper::closeDesertIfaceDN()
{
	//close and delete ds_dn
	if (ds_dn)
	{
		if (ds_dn->isOpen())
#ifndef SAVE_IFACE_FILE
			ds_dn->CloseWithUpdate();
#else
			ds_dn->CloseNoUpdate();
#endif
	}
}

void DesertHelper::closeDesertIfaceBackDN()
{
	//close and delete dbs_dn
	if (dbs_dn)
	{
		if (dbs_dn->isOpen())
		{
			dbs_dn->CloseWithUpdate();
		}
	}

	dbs_dn.reset(nullptr);
}

bool DesertHelper::runCyPhy2Desert()
{
	DesertIface::DesertSystem dummy;
	long configCount = -1;
	CDesertStatusDlg s_dlg(dummy, "", des_map, inv_des_map, NULL, showGui == false, configCount);
	m_realConfigCount = configCount;

	if (showGui)
	{
		s_dlg.Create(IDD_DESERTSTATUSDLG);
		GetStatusDlg(&s_dlg);

		s_dlg.SetStatus(_T("Initializing"),0);
	}

	CyPhyML::RootFolder cyphy_rf = CyPhyML::RootFolder::Cast(cyphy_dn.GetRootObject());

	ds_dn = unique_ptr<Udm::SmartDataNetwork>(new Udm::SmartDataNetwork(DesertIface::diagram));
	ds_dn->CreateNew(desertIfaceFile, "DesertIface.xsd", DesertIface::DesertSystem::meta,Udm::CHANGES_LOST_DEFAULT);
	DesertIface::DesertSystem desert_top = DesertIface::DesertSystem::Cast(ds_dn->GetRootObject());
	if (showGui)
	{
		s_dlg.SetStatus(_T("Execute CyPhy2Desert"), 40);
	}
	/*if(startObj!=Udm::null && (Uml::IsDerivedFrom(startObj.type(), CyPhyML::DesignContainer::meta)))
	{
		Udm::Object pobj = startObj.GetParent();
		while(!Uml::IsDerivedFrom(pobj.type(), CyPhyML::DesignSpace::meta))
		{
			pobj = pobj.GetParent();
		}*/
	CyPhy2Desert c2d(desert_top);
	try{
		if(rootDC!=Udm::null)
			c2d.generateDesert(rootDC);//, desert_top, com2elemMap, dcon2CyphyConMap);
		else if(rootDS!=Udm::null)
			c2d.generateDesert(rootDS);
		else
			c2d.generateDesert(cyphy_rf);
	}
	catch(udm_exception &exc)
	{
		if(!CyPhy2Desert::errorMsg.empty())
			GMEConsole::Console::Error::writeLine(CyPhy2Desert::errorMsg);
		throw;
	}

	if(!CyPhy2Desert::invalidParameters.empty())
	{
		GMEConsole::Console::Warning::writeLine("No constraints generated for the following invalid parameters:");
		for(auto param_it=CyPhy2Desert::invalidParameters.begin();param_it!=CyPhy2Desert::invalidParameters.end();++param_it)
		{
			CyPhyML::Parameter param = *param_it;
			std::string err = "Parameter: <"+appendObjLink(param) + "> contains invalid syntax in Range: "+(std::string)param.Range();
			GMEConsole::Console::Warning::writeLine(err);
		}
	}
	if(!CyPhy2Desert::invalidCustomFormulas.empty())
	{
		GMEConsole::Console::Error::writeLine("The following CustomFormulas are invalid:");
		for(auto cf_it=CyPhy2Desert::invalidCustomFormulas.begin();cf_it!=CyPhy2Desert::invalidCustomFormulas.end();++cf_it)
		{
			CyPhyML::CustomFormula cf = *cf_it;
			std::string err = "CustomFormula: <"+appendObjLink(cf) + "> contains invalid syntax in expression: "+(std::string)cf.Expression();
			GMEConsole::Console::Error::writeLine(err);
		}
		if (showGui)
		{
			s_dlg.OnFinished();
		}
		return false;
	}

	if(!CyPhy2Desert::invalidGroups.empty())
	{
		GMEConsole::Console::Error::writeLine("The following DecisionGroup are invalid:");
		std::string info("DecisionGroup: ");
		for(auto dg_it=CyPhy2Desert::invalidGroups.begin();dg_it!=CyPhy2Desert::invalidGroups.end();++dg_it)
		{
			CyPhyML::DecisionGroup dg = *dg_it;
			string err = "DecisionGroup: <"+appendObjLink(dg) + "> contains the alternatives which do not match. ";
			info = info + (std::string)dg.name()+" ";
			GMEConsole::Console::Error::writeLine(err);
		}
		info = info + "contain the Alternatives which do not match. \nDo you want to proceed(with ignoring the DecisionGroup constraint altogether)?";
		if(AfxMessageBox(utf82cstring(info), MB_YESNO) == IDNO)
		{
			if (showGui)
			{
				s_dlg.OnFinished();
			}
			return false;
		}
	}

	if(!CyPhy2Desert::nullComponentRefs.empty())
	{
		GMEConsole::Console::Error::writeLine("The following ComponentRef are null:");
		std::string info("ComponentRef: ");
		for(auto ref_it=CyPhy2Desert::nullComponentRefs.begin();ref_it!=CyPhy2Desert::nullComponentRefs.end();++ref_it)
		{
			CyPhyML::ComponentRef ref = *ref_it;
			std::string err = "ComponentRef: <"+appendObjLink(ref) + "> refers to null object. ";
			info = info + (std::string)ref.name()+" ";
			GMEConsole::Console::Error::writeLine(err);
		}
	}

	mgroupMap = c2d.getMorphMatrixGroup();
	moptionMap = c2d.getMorphMatrixOption();

	if (showGui)
	{
		s_dlg.SetStatus(_T("Execute CyPhy2Desert"), 60);
	}
	c2d.getCom2ElemMap(com2elemMap);
	c2d.getConstraintMap(dcon2CyphyConMap);
	naturalParameters = c2d.getNatualParameters();
	customParameters = c2d.getCustomParameters();
	if (showGui)
	{
		s_dlg.SetStatus(_T("Write out Desert XML file"), 80);
	}

	//build up the oldconstraintMap
	set<std::string> groupConstraints = c2d.getGroupConstraits();
	int tmpCnt = 0;
	for(set<std::string>::iterator string_it=groupConstraints.begin();string_it!=groupConstraints.end();++string_it)
	{
		defaultConstraints.append(*string_it);
		tmpCnt++;
		if(tmpCnt != groupConstraints.size())
			defaultConstraints.append(":");
	}

	constraintCount = 0;
	set<DesertIface::ConstraintSet> csets = desert_top.ConstraintSet_kind_children();
	for(set<DesertIface::ConstraintSet>::iterator i=csets.begin();i!=csets.end();++i)
	{
		set<DesertIface::Constraint> cons = (*i).Constraint_kind_children();
		for(set<DesertIface::Constraint>::iterator j=cons.begin();j!=cons.end();++j)
		{
			if(groupConstraints.find((std::string)(*j).name())==groupConstraints.end())
			{
				oldConstraintMap[constraintCount] = (*j);
				makeConstraintDomainMap(constraintCount, (DesertIface::Constraint)(*j));
				constraintCount++;
			}
		}
	}

	if (showGui)
	{
		s_dlg.OnFinished();
	}

	//generate desertIfaceElemMap
	desertIfaceElemMap.clear();
	set<DesertIface::Space> spaces = desert_top.Space_kind_children();
	//DesertIface::Space space;

	if(s_dlg.m_cancel || spaces.empty()) return false;

	if(!csets.empty())
		dconstraintSet = *(csets.begin());

	DesertIface::Space space = *(spaces.begin());
	set<DesertIface::Element> elems = space.Element_kind_children();
	makeDesertIfaceElemMap(elems);   //Null can exist in Desert, but not in Cyphy

	for(map<CyPhyML::DesignEntity, DesertIface::Element>::iterator mit=com2elemMap.begin();mit!=com2elemMap.end();++mit)
	{
		CyPhyML::DesignEntity cd = (*mit).first;
		cyPhyDesignElemMap[cd.ID()] = cd;
	}

	return (!s_dlg.m_cancel);
	//return true;
}

void DesertHelper::makeDesertIfaceElemMap(const set<DesertIface::Element> &elems)
{
	for(set<DesertIface::Element>::const_iterator it_elem=elems.begin();it_elem!=elems.end();++it_elem)
	{
		DesertIface::Element elem = *it_elem;
		desertIfaceElemMap[elem.externalID()] = elem;
		set<DesertIface::Element> c_elems = elem.Element_kind_children();
		makeDesertIfaceElemMap(c_elems);
	}
}

void DesertHelper::addConstraint(int listIndex, int cyphyObjId, const std::string &name, const std::string &expr, std::string &context)
{
	Udm::Object obj = cyphy_dn.ObjectById(cyphyObjId);
	if(obj == Udm::null) return;
	if(!Uml::IsDerivedFrom(obj.type(), CyPhyML::DesignEntity::meta)) return;

	CyPhyML::DesignEntity scom = CyPhyML::DesignEntity::Cast(obj);
	map<CyPhyML::DesignEntity, DesertIface::Element>::iterator pos = com2elemMap.find(CyPhyML::DesignEntity::Cast(scom));
	if(pos == com2elemMap.end()) return;

	DesertIface::Element elem = (*pos).second;
	context = getElementFullPath(elem);

	DesertIface::DesertSystem desert_top = DesertIface::DesertSystem::Cast(ds_dn->GetRootObject());
	DesertIface::ConstraintSet conset;
	set<DesertIface::ConstraintSet> csets = desert_top.ConstraintSet_kind_children();
	if(csets.empty())
	{
		conset = ConstraintSet::Create(desert_top);
		conset.name() = "constraints";
	}
	else
		conset = *(csets.begin());

	DesertIface::Constraint newcon = DesertIface::Constraint::Create(conset);
	newcon.id() = newcon.externalID() = newcon.uniqueId();
	newcon.name() = name;
	newcon.expression() = expr;
	newcon.context() = elem;

	newConstraintMap[listIndex] = newcon;
	makeConstraintDomainMap(listIndex, newcon, "OCL");
}

bool DesertHelper::getConstraint(int idx, std::string &name, std::string &context, std::string &expression, std::string &ctype, std::string &cdomain)
{
	bool isnew = true;
	map<int, DesertIface::Constraint>::iterator pos = newConstraintMap.find(idx);
	if(pos == newConstraintMap.end())
	{
		pos = oldConstraintMap.find(idx);
		if(pos == oldConstraintMap.end()) return false;
		isnew = false;
	}
	DesertIface::Constraint con = (*pos).second;
	name = con.name();
	context = getElementFullPath(con.context());
	expression = con.expression();

	map<DesertIface::Constraint, CyPhyML::ConstraintBase>::iterator c2c_pos = dcon2CyphyConMap.find(con);
	if(c2c_pos!=dcon2CyphyConMap.end())
	{
		CyPhyML::ConstraintBase cbase = (*c2c_pos).second;
		std::string cbasetype = cbase.type().name();
		if(cbasetype=="Constraint")
			ctype = "OCL";
		else if(cbasetype=="VisualConstraint" && !isnew)
			ctype = "Visual Constraint";
		else if(cbasetype=="VisualConstraint" && isnew)
			ctype = "Visual Constraint(from selelctor)";
		else if(cbasetype=="PropertyConstraint")
			ctype = "Property Constraint";

		cdomain = cbase.Domain();
	}
	else
	{
		ctype = "Parameter Constraint";
	}

	return true;
}

void DesertHelper::updateConstraint(int idx, const std::string &name, const std::string &expression)
{
	map<int, DesertIface::Constraint>::iterator pos = oldConstraintMap.find(idx);
	if(pos == oldConstraintMap.end())
	{
		pos = newConstraintMap.find(idx);
		if(pos == newConstraintMap.end()) return;
	}
	DesertIface::Constraint con = (*pos).second;
	con.name() = name;
	con.expression() = expression;
}

void DesertHelper::updateCyPhy()
{
	map<int, DesertIface::Constraint>::iterator pos;
	for(pos=oldConstraintMap.begin();pos!=oldConstraintMap.end();++pos)
	{
		DesertIface::Constraint dcon = (*pos).second;
		map<DesertIface::Constraint, CyPhyML::ConstraintBase>::iterator mit = dcon2CyphyConMap.find(dcon);

		if(mit==dcon2CyphyConMap.end()) continue;
		CyPhyML::ConstraintBase conObj = (*mit).second;
		if(conObj.type()==CyPhyML::Constraint::meta)
		{
			CyPhyML::Constraint scon = CyPhyML::Constraint::Cast(conObj);
			scon.name() = dcon.name();
			scon.Context() = dcon.expression();
		}
	}
	//for(pos=newConstraintMap.begin();pos!=newConstraintMap.end();++pos)
	//{
	//	DesertIface::Constraint dcon = (*pos).second;
	//	DesertIface::Element elem = dcon.context();
	//	Udm::Object comObj = cyphy_dn.ObjectById(elem.externalID());
	//	if(comObj==Udm::null) continue;
	//	CyPhyML::Constraint scon = CyPhyML::Constraint::Create(CyPhyML::DesignContainer::Cast(comObj));
	//	dcon.id() = dcon.externalID() = scon.uniqueId();
	//	scon.name() = dcon.name();
	//	scon.Context() = dcon.expression();
	//	std::string addlog("Add new constraint: "+appendObjLink(scon));
	//	GMEConsole::Console::Out::writeLine(addlog);
	//}
	//for(set<CyPhyML::Constraint>::iterator it=deleteConstraitSet.begin();it!=deleteConstraitSet.end();++it)
	//{
	//	CyPhyML::Constraint con = *it;
	//	std::string conname = con.name();
	//	con.DeleteObject();
	//	std::string addlog("Delete constraint: "+conname);
	//	GMEConsole::Console::Out::writeLine(addlog);
	//}
	oldConstraintMap.insert(newConstraintMap.begin(), newConstraintMap.end());
	newConstraintMap.clear();
}

int DesertHelper::getConstraintSize()
{
	return oldConstraintMap.size()+newConstraintMap.size();
}

std::string DesertHelper::getElementFullPath(const DesertIface::Element &elem)
{
	std::string ret = elem.name();
	Udm::Object parent = elem.GetParent();
	while(parent.type() == DesertIface::Element::meta)
	{
		ret = UdmUtil::ExtractName(parent) + "\\" + ret;
		parent = parent.GetParent();
	}
	return ret;
}

std::string DesertHelper::getCurrObjType()
{
	if(currObj == Udm::null)
		return "NullObj";
	else
		return (std::string)currObj.type().name();
}

std::string DesertHelper::getCyPhyObjType(int id)
{
	Udm::Object obj = cyphy_dn.ObjectById(id);
	if(obj == Udm::null)
		return "NullObj";
	else
		return (std::string)obj.type().name();
}

bool DesertHelper::isFocusObject(int id)
{
	if(currObj == Udm::null) return false;
	else return(id==currObj.uniqueId());
}

void DesertHelper::applyConstraints(set<int> constraints_list, bool dsRefresh)
{
	conId_list.clear();
	applyAll = false;
	//std::string conslist="";
	std::string conslist=defaultConstraints;
//	set<int> conId_list;
	set<CyPhyML::ConstraintBase> cyphyCons;
	for(set<int>::iterator it=constraints_list.begin();it!=constraints_list.end();++it)
	{
		int idx = *it;
		map<int, DesertIface::Constraint>::iterator pos = newConstraintMap.find(idx);
		if(pos == newConstraintMap.end())
		{
			pos = oldConstraintMap.find(idx);
			if(pos == oldConstraintMap.end()) continue;
		}
		DesertIface::Constraint con = (*pos).second;

		if(conslist.empty())
			conslist = con.name();
		else
			conslist.append(":"+(std::string)con.name());

		map<DesertIface::Constraint, CyPhyML::ConstraintBase>::iterator cpos = dcon2CyphyConMap.find(con);
		if(cpos != dcon2CyphyConMap.end())
		{
			CyPhyML::ConstraintBase cyphyCon = (*cpos).second;
			conId_list.insert(cyphyCon.ID());
			cyphyCons.insert(cyphyCon);
		}
	}

	// debugging:
	//ds_dn->SaveAs("tmp\\tmp.mem");
	runDesertFinit_1(conslist, dsRefresh);
}

void DesertHelper::applyAllConstraints(bool dsRefresh)
{
	conId_list.clear();
	applyAll = true;

	// debugging:
	//ds_dn->SaveAs("tmp1.mem");
	runDesertFinit_1("applyAll", dsRefresh);
}

void DesertHelper::buildDesertSystem(DesertSystem& ds, bool refresh)
{
	using namespace DesertIface;

	ds = DesertSystem::Cast(ds_dn->GetRootObject());

	if(refresh)
	{
		des_map.clear();
		inv_des_map.clear();
		//	desertlogFile = (std::string)ds.SystemName()+"_desert.log";
		DesertInit(utf82cstring(desertlogFile), true);
		set<Space> spaces = ds.Space_kind_children();
		set<Space>::iterator sp_iterator;

		ASSERT(spaces.begin() != spaces.end());
		BackIfaceFunctions::ClearMap();

		UdmElementSet elements;
		UdmMemberSet custom_members;
		std::function<void(short)> noop = [](short) {};

		//Build spaces
		if (!spaces.empty())
		{
			Space sp;
			DesertIface::Element dummy;

			for (sp_iterator = spaces.begin(); sp_iterator != spaces.end(); sp_iterator ++)
			{
				Space sp = *(sp_iterator);
				//TRACE("Creating space: %s\n", ((string)sp.name()).c_str());
				CreateDesertSpace(sp, dummy, des_map, inv_des_map, elements, true, noop);
			}//eo for (sp_iterator)
		}//if (!spaces.empty())

		//create relations
		CreateElementRelations(ds, des_map, inv_des_map, noop);

		//create constrains
		CreateConstraints(ds, des_map, inv_des_map);

		//create natural domains
		CreateNaturalDomains(ds, des_map, inv_des_map, noop);

		//create custom domains
		CreateCustomDomains(ds, des_map, inv_des_map, custom_members, noop);

		//create custom domain relations
		CreateMemberRelations(ds, des_map, inv_des_map, noop);

		//create variable properties
		CreateVariableProperties(des_map, inv_des_map, elements, noop);

		//create constant properties
		CreateConstantProperties(des_map, inv_des_map, elements, custom_members, noop);

		CreateSimpleFormulas(ds,des_map,inv_des_map);
		//create assignments for VariableProperties
		CreateAssignments(des_map, inv_des_map, elements, custom_members, noop);
	}
}

void DesertHelper::runDesertFinit_1(const std::string &constraints, bool refresh)
{
	DesertSystem ds;
	buildDesertSystem(ds, refresh);

	if (!defaultConstraints.empty())
	{
		if (DesertFinit_preApply())
			DesertFinit_Apply(_T(""));
		else
			throw new CDesertException(CString("invalid Constraint"));
	}

	long configCount = -1;
	CDesertStatusDlg s_dlg(ds, constraints, des_map, inv_des_map, NULL, false, configCount);
	CString invalidConstraint;
	if (IsDebuggerPresent()) {
		ds_dn->SaveAs(desertIfaceFile);
	}
	if (showGui)
	{
		s_dlg.DoModal();
		if (s_dlg.m_fatal)
		{
			invalidConstraint = s_dlg.m_invalidConstraint;
		}
	}
	if (!showGui)
	{
		BaseNotify notify(150);
		DesertThread m_thrd(ds, constraints, des_map, inv_des_map, &notify, 1, configCount);
		m_thrd.RunDesert();
		invalidConstraint = notify.m_invalidConstraint;
	}

	if(invalidConstraint.GetLength())
	{
		throw new CDesertException(CString("invalid Constraint"), invalidConstraint);
	}
	if(configCount!=-1) //may result 0 due to the invalid constraint
		m_realConfigCount = configCount;
}

int DesertHelper::runDesertFinit_2()
{
	if(dbs_dn)
		return cfgCount;

	dbs_dn = std::unique_ptr<Udm::SmartDataNetwork>(new Udm::SmartDataNetwork(DesertIfaceBack::diagram));
	dbs_dn->CreateNew(desertIfaceBackFile,"DesertIfaceBack",DesertIfaceBack::DesertBackSystem::meta, Udm::CHANGES_LOST_DEFAULT);
	DesertIfaceBack::DesertBackSystem dbs = DesertIfaceBack::DesertBackSystem::Cast(dbs_dn->GetRootObject());

	DesertSystem ds = DesertSystem::Cast(ds_dn->GetRootObject());
	dbs.SystemName() = ds.SystemName();
	long localConfigCount = -1;
	bool cancel;
	CDesertStatusDlg s_dlg(dbs, des_map, inv_des_map, NULL, false, localConfigCount);
	if (showGui)
	{
		s_dlg.DoModal();
		cancel = s_dlg.m_cancel;
	}
	else
	{
		BaseNotify notify(150);
		DesertThread m_thrd(dbs, des_map, inv_des_map, &notify, 2, localConfigCount);
		m_thrd.RunDesert();
		cancel = notify.m_cancel;
	}
	m_realConfigCount = localConfigCount;

	cfgCount = 0;
	configMap.clear();
	configElemMap.clear();
	if(!s_dlg.m_cancel)
	{
		set<DesertIfaceBack::Configuration> cfgs = dbs.Configuration_kind_children();
		if (cfgs.size() != m_realConfigCount) {
			dbs_dn->SaveAs(desertIfaceBackFile);
			throw udm_exception("Internal error: desert returned the wrong number of configurations");
		}
		for(set<DesertIfaceBack::Configuration>::iterator cfg_it=cfgs.begin();cfg_it!=cfgs.end();++cfg_it)
		{
			DesertIfaceBack::Configuration cfg = *cfg_it;
			int id = cfg.id();
			configMap[id-1] = cfg;
			std::string n = cfg.name();
			cfgCount++;
			makeConfigElemMap(cfg);
		}

		//generate desertIfaceBackElemMap
		desertIfaceBackElemMap.clear();
		set<DesertIfaceBack::Element> elems = dbs.Element_kind_children();
		for(set<DesertIfaceBack::Element>::iterator it_elem=elems.begin();it_elem!=elems.end();++it_elem)
		{
			DesertIfaceBack::Element elem = *it_elem;
			desertIfaceBackElemMap[elem.externalID()] = elem;
		}
		desertFinit_2_fail = false;

		char cnt_buff[10];
		_itoa(cfgCount, cnt_buff, 10);
		std::string cfgSizeStr = (std::string)cnt_buff;

		updateNumAssociatedConfigs(rootDC, cfgSizeStr);
		// Also, set the NumAssociatedConfigs attribute value for the root container, see META-1686
		rootDC.NumAssociatedConfigs() = cfgSizeStr + "/" + cfgSizeStr;


		// Update actual no. of configurations
		cfgCount = localConfigCount;
	}
	else
	{
		desertFinit_2_fail = true;
		closeDesertIfaceBackDN();
		cfgCount = -1;
	}

	return cfgCount;
}

_int64 DesertHelper::getDesignSpaceSize()
{
	return getDesertSize();
}

//void DesertHelper::getDesignSpaceSize(double& dspSize, long& repSize)
//{
//	getDesertSize(dspSize, repSize);
//}

void DesertHelper::getAppliedConstraintSet(set<std::string> &consSet)
{
	std::set<tstring> newSet;
	for each (auto s in consSet)
	{
		newSet.insert(tstring(utf82cstring(s)));
	}
	getDesertAppliedConstraintSet(newSet);
}

int DesertHelper::runDesert(const std::string &constraints)
{
	//CDesertStatusDlg s_dlg(0, false);
	//s_dlg.Create(IDD_DESERTSTATUSDLG);
	//GetStatusDlg(&s_dlg);

	//using namespace DesertIface;

	///*s_dlg.SetStatus(SD_INIT);
	//s_dlg.SetStatus(SD_PARSE);*/
	//std::string ds_copy = desertIfaceFile;
	//size_t extPos = ds_copy.find(".xml");
	//ds_copy.replace(extPos,4,"_copy.xml");
	//ds_dn->SaveAs(ds_copy);

	//Udm::SmartDataNetwork ds_dn_copy(DesertIface::diagram);
	//ds_dn_copy.OpenExisting(ds_copy, "DesertIface.xsd", Udm::CHANGES_LOST_DEFAULT);
	//DesertSystem ds = DesertSystem::Cast(ds_dn_copy.GetRootObject());

	//dbs_dn = new Udm::SmartDataNetwork(DesertIfaceBack::diagram);
	//dbs_dn->CreateNew(desertIfaceBackFile,"DesertIfaceBack",DesertIfaceBack::DesertBackSystem::meta, Udm::CHANGES_LOST_DEFAULT);
	//DesertIfaceBack::DesertBackSystem dbs = DesertIfaceBack::DesertBackSystem::Cast(dbs_dn->GetRootObject());
	//
	//CDesertStatusDlg s_dlg(ds, dbs, constraints);
	//s_dlg.DoModal();

	//cfgCount = 0;
	//configMap.clear();
	//configElemMap.clear();
	//if(!s_dlg.m_cancel)
	//{
	//	set<DesertIfaceBack::Configuration> cfgs = dbs.Configuration_kind_children();
	//	for(set<DesertIfaceBack::Configuration>::iterator cfg_it=cfgs.begin();cfg_it!=cfgs.end();++cfg_it)
	//	{
	//		DesertIfaceBack::Configuration cfg = *cfg_it;
	//		int id = cfg.id();
	//		configMap[id-1] = cfg;
	//		std::string n = cfg.name();
	//		cfgCount++;
	//		makeConfigElemMap(cfg);
	//	}

	//	ds_dn_copy.CloseWithUpdate();
	//	remove(ds_copy.c_str());

	//	//generate desertIfaceBackElemMap
	//	desertIfaceBackElemMap.clear();
	//	set<DesertIfaceBack::Element> elems = dbs.Element_kind_children();
	//	for(set<DesertIfaceBack::Element>::iterator it_elem=elems.begin();it_elem!=elems.end();++it_elem)
	//	{
	//		DesertIfaceBack::Element elem = *it_elem;
	//		desertIfaceBackElemMap[elem.externalID()] = elem;
	//	}
	//}
	//else
	//{
	//	ds_dn_copy.CloseNoUpdate();
	//	remove(ds_copy.c_str());
	//	closeDesertIfaceBackDN();
	//}

	return cfgCount;
}

void DesertHelper::createNewConfigurations()
{
	createdCfgIds.clear();
	if(rootDC!=Udm::null)
		cfg_models = CyPhyML::Configurations::Create(rootDC);
	else if(rootDS!=Udm::null)
		cfg_models = CyPhyML::Configurations::Create(rootDS);
	else
		return;

	char currtime[128];
	time_t now = time(0);
	struct tm* tm = localtime(&now);

	sprintf(currtime,"%02d-%02d--%02d-%02d-%02d", (tm->tm_mon + 1), tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec);
	cfg_models.name() = "Exported-Configurations-at--"+(std::string)currtime+"";

	cfg_models.applyAll() = applyAll;
	cfg_models.isDirty() = false;
	cfg_models.configSize() = cfgCount;
}

void DesertHelper::exportModel_ex(set<int> cfgIds)
{
	if(cfg_models==Udm::null) return;

	//if(cfg_models==Udm::null)
	//{
	//	if(rootDC!=Udm::null)
	//		cfg_models = CyPhyML::Configurations::Create(rootDC);
	//	else if(rootDS!=Udm::null)
	//		cfg_models = CyPhyML::Configurations::Create(rootDS);
	//	else
	//		return;
	//}

	//cfg_models.name() = "configuration models";
	//cfg_models.applyAll() = applyAll;
	//cfg_models.isDirty() = false;
	//cfg_models.configSize() = cfgCount;

	CStatusDialog prgDlg;
	if (showGui)
	{
		prgDlg.Create(IDD_PROGRESS_DIALOG);
		GetStatusDlg(&prgDlg);

		int idCnt = cfgIds.size();
		//std::string fnbase;
		prgDlg.SetRange((1 + idCnt)*10); // processPos += 10;

		prgDlg.SetProgress("Preparing to export models...");
	}

	CyPhyML::RootFolder cyphy_rf = CyPhyML::RootFolder::Cast(cyphy_dn.GetRootObject());
	DesertIfaceBack::DesertBackSystem desertBack_top = DesertIfaceBack::DesertBackSystem::Cast(dbs_dn->GetRootObject());

//	morphMatrixInitialize1();

	for(set<int>::iterator i=cfgIds.begin();i!=cfgIds.end();++i)
	{
		set<int>::iterator int_it = createdCfgIds.find(*i);
		if(int_it!=createdCfgIds.end())
			continue;

		createdCfgIds.insert(*i);

		if (showGui)
		{
			DOEVENTS();
			if(prgDlg.m_cancel)
			{
				morphMatrixFinalize1();
				return;
			}
		}
		DesertIfaceBack::Configuration config = configMap[*i];
		int cfgid = config.id();
		desertBack_top.selConfig() = config.id();

		char buffer [33];
		_itoa (cfgid,buffer,10);
		std::string msg ="create output model for cfg:"+(std::string)buffer+"...";
		if (showGui)
		{
			prgDlg.SetProgress(CString(msg.c_str()));
		}
		try{
			std::string newCWCname = "cfg"+(std::string)buffer;
			map<int, std::string>::iterator it = cfgId2NameMap.find(cfgid);
			if(it!=cfgId2NameMap.end())
				newCWCname = (*it).second;
			CyPhyML::CWC newcwc =generateCyPhy(cyphy_rf, cfg_models, desertBack_top, config, morphMatrix, allEntities);
			if(newcwc!=Udm::null)
			{
				newcwc.name() = newCWCname;
				newcwc.CfgID() = config.cfgId();
			}
		}
		catch(udm_exception &exc)
		{
			morphMatrixFinalize1();
			if (showGui)
			{
				AfxMessageBox(utf82cstring(exc.what()));
				prgDlg.OnFinished();
			}
			throw;
		}
	}

//	morphMatrixFinalize1();
	if (showGui)
	{
		prgDlg.OnFinished();
	}
}

void DesertHelper::exportModel_ForMorphMatrix(set<int> cfgIds)
{
	if(cfg_models==Udm::null)
	{
		if(rootDC!=Udm::null)
			cfg_models = CyPhyML::Configurations::Create(rootDC);
		else if(rootDS!=Udm::null)
			cfg_models = CyPhyML::Configurations::Create(rootDS);
		else
			return;
	}

	cfg_models.name() = "configuration models";
	cfg_models.applyAll() = applyAll;
	cfg_models.isDirty() = false;
	cfg_models.configSize() = cfgCount;

	CStatusDialog prgDlg;
	prgDlg.Create(IDD_PROGRESS_DIALOG);
	GetStatusDlg(&prgDlg);

	int idCnt = cfgIds.size();
	//std::string fnbase;
	prgDlg.SetRange(25*idCnt);

	prgDlg.SetProgress("Preparing for export models for MorphMatrix...");

	CyPhyML::RootFolder cyphy_rf = CyPhyML::RootFolder::Cast(cyphy_dn.GetRootObject());
	DesertIfaceBack::DesertBackSystem desertBack_top = DesertIfaceBack::DesertBackSystem::Cast(dbs_dn->GetRootObject());

	morphMatrixInitialize1();

	for(set<int>::iterator i=cfgIds.begin();i!=cfgIds.end();++i)
	{
		DOEVENTS();
		if(prgDlg.m_cancel)
		{
			morphMatrixFinalize1();
			return;
		}
		DesertIfaceBack::Configuration config = configMap[*i];
		int cfgid = config.id();
		desertBack_top.selConfig() = config.id();

		char buffer [33];
		_itoa (cfgid,buffer,10);
		std::string msg ="create output model for cfg:"+(std::string)buffer+"...";
		prgDlg.SetProgress(CString(msg.c_str()));
		try{
			generateMorphMatrixCfgsForGivenConfigs(cyphy_rf, cfg_models, desertBack_top, config, morphMatrix, allEntities);
		}
		catch(udm_exception &exc)
		{
			AfxMessageBox(utf82cstring(exc.what()));
			morphMatrixFinalize1();
			prgDlg.OnFinished();
			return;
		}
	}

	morphMatrixFinalize1();
	prgDlg.OnFinished();
}


bool DesertHelper::getConfiguration(int idx, std::string &name, std::string &cfg_no, std::string &cfg_id)
{
	map<int, DesertIfaceBack::Configuration>::iterator pos = configMap.find(idx);
	if(pos==configMap.end()) return false;
	DesertIfaceBack::Configuration cfg = (*pos).second;
	name = cfg.name();
	char buffer [33];
	_itoa ((int)cfg.id(),buffer,10);
	cfg_no = buffer;
	cfg_id = cfg.cfgId();
	return true;
}

bool DesertHelper::getConfigurationUniqueId(int idx, std::string &id)
{
	map<int, DesertIfaceBack::Configuration>::iterator pos = configMap.find(idx);
	if(pos==configMap.end()) return false;
	DesertIfaceBack::Configuration cfg = (*pos).second;
	char buffer [80];
	_ltoa ((long)cfg.uniqueId(),buffer,10);
	id = buffer;
	return true;
}



//void DesertHelper::createExportFolder()
//{
//	//remove the previous folders
////	cfgDCMap.clear();
//	log.clear();
//	//if(dcfolder!=Udm::null)
//	//{
//	//	dcfolder.DeleteObject();
//	//	dcfolder = 0;
//	//}
//
//	//generate the new one
//	CyPhyML::RootFolder cyphy_rf = CyPhyML::RootFolder::Cast(cyphy_dn.GetRootObject());
//	/*dcfolder = CyPhyML::DeviceConfigurationFolder::Create(cyphy_rf);
//	dcfolder.name() = "DeviceConfigurationFolder for Cfgs:";*/
//}

std::string DesertHelper::generateOutputFileName(DesertIfaceBack::Configuration &config, const std::string &outputFdr)
{
	std::string outputFile = desertIfaceFile;
	return outputFile;
}

std::string DesertHelper::getFileNameNoExt(const std::string &fullpath)
{
	std::string ret = fullpath;
	size_t pos = ret.rfind("\\");
	if(pos!=string::npos)
		ret = ret.substr(pos+1);
	pos = ret.find(".");
	if(pos!=string::npos)
		ret = ret.substr(0, pos);
	return ret;
}

void DesertHelper::getDesertIfaceSpace(DesertIface::Space &space)
{
	DesertIface::DesertSystem desert_top = DesertIface::DesertSystem::Cast(ds_dn->GetRootObject());
	set<DesertIface::Space> spaces = desert_top.Space_kind_children();
	if(!spaces.empty())
		space = *(spaces.begin());
}

bool DesertHelper::isElementExist(int elemId)
{
	map<int, DesertIfaceBack::Element>::iterator pos = desertIfaceBackElemMap.find(elemId);
	if(pos==desertIfaceBackElemMap.end())
	{
		map<int, DesertIface::Element>::iterator dit = desertIfaceElemMap.find(elemId);
		if(dit==desertIfaceElemMap.end())
			return false;
		else
		{
			DesertIface::Element elem = (*dit).second;
			//if the component is under Compound, then return true
			Udm::Object parentObj = elem.parent();
			if(parentObj.type()==DesertIface::Element::meta)
			{
				DesertIface::Element parentElem = DesertIface::Element::Cast(parentObj);
				if(!parentElem.decomposition())
					return false;
				while(parentElem.decomposition())
				{
					/*parentObj = parentElem.parent();
					if(parentObj.type()!=DesertIface::Element::meta) return true;
					parentElem = DesertIface::Element::Cast(parentObj);*/
					return isElementExist(parentElem.externalID());
				}
				// FIXME: need to return a value here or throw an exception
			}
			else return true;
		}
	}

	else return true;
}

bool DesertHelper::isElementSelected(int cfgId, int elemId, bool forNull)
{
	//only consider the Component
	map<int, DesertIface::Element>::iterator diPos = desertIfaceElemMap.find(elemId);
	if(diPos==desertIfaceElemMap.end()) return false;
	DesertIface::Element diElem = (*diPos).second;
	std::string nn=diElem.name();
	if(!forNull)
	{
		set<DesertIface::Element> elems = diElem.Element_kind_children();
		if(!elems.empty()) return false;
	}

	map<int, DesertIfaceBack::Configuration>::iterator pos = configMap.find(cfgId);
	if(pos==configMap.end()) return false;
	DesertIfaceBack::Configuration config = (*pos).second;
	map<DesertIfaceBack::Configuration, set<int> >::iterator elemPos = configElemMap.find(config);
	if(elemPos==configElemMap.end()) return false;

	set<int> ids = (*elemPos).second;
	if(ids.find(elemId)!=ids.end())
	{
		return true;
	}

	//if the component is under Compound, then return true
	Udm::Object parentObj = diElem.parent();
	if(parentObj.type()==DesertIface::Element::meta)
	{
		DesertIface::Element parentElem = DesertIface::Element::Cast(parentObj);
		if(!parentElem.decomposition())
		{
			return false;
		}
		while(parentElem.decomposition())
		{
			if(ids.find(parentElem.externalID())!=ids.end()) return true;
			parentObj = parentElem.parent();
			if(parentObj.type()!=DesertIface::Element::meta) return true;
			parentElem = DesertIface::Element::Cast(parentObj);
		}
	}

	return false;
}

void DesertHelper::makeConfigElemMap(const DesertIfaceBack::Configuration &config)
{
	set<int> ids;
	const set<DesertIfaceBack::AlternativeAssignment> &alts = config.AlternativeAssignment_kind_children();
	for(set<DesertIfaceBack::AlternativeAssignment>::const_iterator i=alts.begin();i!=alts.end();++i)
	{
		const DesertIfaceBack::AlternativeAssignment &alt = *i;
		const DesertIfaceBack::Element &alt_end = alt.alternative_end();
		ids.insert(alt_end.id());
	}
	configElemMap[config] = ids;
}

bool DesertHelper::checkConstraints_1()
{
	bool valid = VerifyConstraints(_T("applyAll"));
	desertFinit_2_fail = true;
	return valid;
}

bool DesertHelper::checkConstraints()
{
	using namespace DesertIface;

	std::string ds_copy = desertIfaceFile;
	size_t extPos;
#ifdef IFACE_FILE_FORMAT_XML
	extPos = ds_copy.find(".xml");
	ds_copy.replace(extPos,4,"_copy.xml");
#else
		extPos = ds_copy.find(".mem");
			ds_copy.replace(extPos,4,"_copy.mem");
#endif
//	size_t extPos = ds_copy.find(".mem");
//	ds_copy.replace(extPos,4,"_copy.mem");
	ds_dn->SaveAs(ds_copy);

	Udm::SmartDataNetwork ds_dn_copy(DesertIface::diagram);
	ds_dn_copy.OpenExisting(ds_copy, "DesertIface.xsd", Udm::CHANGES_LOST_DEFAULT);
	DesertSystem ds = DesertSystem::Cast(ds_dn_copy.GetRootObject());

	//dbs_dn = new Udm::SmartDataNetwork(DesertIfaceBack::diagram);
	//dbs_dn->CreateNew(desertIfaceBackFile,"DesertIfaceBack",DesertIfaceBack::DesertBackSystem::meta, Udm::CHANGES_LOST_DEFAULT);
	//DesertIfaceBack::DesertBackSystem dbs = DesertIfaceBack::DesertBackSystem::Cast(dbs_dn->GetRootObject());
	//

	DesertInit(utf82cstring(desertlogFile), true);
	set<Space> spaces = ds.Space_kind_children();
	set<Space>::iterator sp_iterator;

	ASSERT(spaces.begin() != spaces.end());

	UdmDesertMap des_map;
	DesertUdmMap inv_des_map;

	BackIfaceFunctions::ClearMap();

	UdmElementSet elements;
	UdmMemberSet custom_members;
	std::function<void(short)> noop = [](short) {};

	//Build spaces
	if (!spaces.empty())
	{
		Space sp;
		DesertIface::Element dummy;

		for (sp_iterator = spaces.begin(); sp_iterator != spaces.end(); sp_iterator ++)
		{
			Space sp = *(sp_iterator);
			//TRACE("Creating space: %s\n", ((string)sp.name()).c_str());
			CreateDesertSpace(sp, dummy, des_map, inv_des_map, elements, true, noop);
		}//eo for (sp_iterator)
	}//if (!spaces.empty())

	//create relations
	CreateElementRelations(ds, des_map, inv_des_map, noop);

	//create constrains
	bool ret = CreateConstraints(ds, des_map, inv_des_map);

	//create natural domains
	CreateNaturalDomains(ds, des_map, inv_des_map, noop);

	//create custom domains
	CreateCustomDomains(ds, des_map, inv_des_map, custom_members, noop);

	//create custom domain relations
	CreateMemberRelations(ds, des_map, inv_des_map, noop);

	//create variable properties
	CreateVariableProperties(des_map, inv_des_map, elements, noop);

	//create constant properties
	CreateConstantProperties(des_map, inv_des_map, elements, custom_members, noop);

	//create assignments for VariableProperties
	CreateAssignments(des_map, inv_des_map, elements, custom_members, noop);

	bool valid = VerifyConstraints(_T("applyAll"));
	ds_dn_copy.CloseNoUpdate();
	desertFinit_2_fail = true;
	closeDesertManageInstace();
	return valid&&ret;
}

//void DesertHelper::removeConstraint(int idx)
//{
//	//find the corresponding scamla constraint to be removed
//	map<int, DesertIface::Constraint>::iterator pos = oldConstraintMap.find(idx);
//	if(pos == oldConstraintMap.end())
//	{
//		pos = newConstraintMap.find(idx);
//		if(pos != newConstraintMap.end())
//		{
//			DesertIface::Constraint con = (*pos).second;
//			con.DeleteObject();
//		}
//	}
//	else
//	{
//		DesertIface::Constraint con = (*pos).second;
//		Udm::Object conObj = cyphy_dn.ObjectById(con.externalID());
//		ASSERT(conObj.type()==CyPhyML::Constraint::meta);
//		deleteConstraitSet.insert(CyPhyML::Constraint::Cast(conObj));
//		con.DeleteObject();
//	}
//
//	//	update map<int, DesertIface::Constraint> oldConstraintMap;
//	map<int, DesertIface::Constraint> tmp;
//	for(pos=oldConstraintMap.begin();pos!=oldConstraintMap.end();++pos)
//	{
//		if((*pos).first == idx) continue;
//		if((*pos).first < idx)
//			tmp[(*pos).first] = (*pos).second;
//		else
//			tmp[(*pos).first-1] = (*pos).second;
//	}
//	oldConstraintMap.clear();
//	oldConstraintMap.insert(tmp.begin(), tmp.end());
//
//	//	update map<int, DesertIface::Constraint> newConstraintMap;
//	tmp.clear();
//	for(pos=newConstraintMap.begin();pos!=newConstraintMap.end();++pos)
//	{
//		if((*pos).first == idx) continue;
//		if((*pos).first < idx)
//			tmp[(*pos).first] = (*pos).second;
//		else
//			tmp[(*pos).first-1] = (*pos).second;
//	}
//	newConstraintMap.clear();
//	newConstraintMap.insert(tmp.begin(), tmp.end());
//}

std::string DesertHelper::appendObjLink(const Udm::Object &obj)
{
	if (!obj)
		return "null";
	return ("<A HREF=\"mga:"+ UdmGme::UdmId2GmeId(obj.uniqueId()) + "\">" + UdmUtil::ExtractName(obj) + "</A>");
}

void DesertHelper::writeLog()
{
	if(log.empty()) return;
	GMEConsole::Console::Out::writeLine("The result of last apply constraints: "+apply_constraints+" ->");
	for(list<std::string>::iterator i=log.begin();i!=log.end();++i)
		GMEConsole::Console::Out::writeLine(*i);
}

void DesertHelper::updateNumAssociatedConfigs(CyPhyML::DesignContainer &container, const std::string &cfgSizeStr)
{
	set<CyPhyML::DesignElement> elems = container.DesignElement_kind_children();
	for(set<CyPhyML::DesignElement>::iterator it=elems.begin();it!=elems.end();++it)
	{
		CyPhyML::DesignElement elem = *it;
		int cnt = getElementInConfigsCount(elem.ID());
		char buffer[10];
		_itoa(cnt, buffer, 10);
		std::string size_str = (std::string)buffer+"/"+cfgSizeStr;
		elem.NumAssociatedConfigs() = size_str;
		cfgSizeInfoMap[elem.ID()] = size_str;
	}

	set<CyPhyML::DesignContainer> cons = container.DesignContainer_kind_children();
	for(set<CyPhyML::DesignContainer>::iterator cit=cons.begin();cit!=cons.end();++cit)
	{
		CyPhyML::DesignContainer con = *cit;
		int cnt = getElementInConfigsCount(con.ID());
		char buffer[10];
		_itoa(cnt, buffer, 10);
		std::string size_str = (cnt == 0 && ((std::string)con.ContainerType()!="Compound"))? "" : (std::string)buffer+"/"+cfgSizeStr;
		con.NumAssociatedConfigs() = size_str;
		cfgSizeInfoMap[con.ID()] = size_str;
		updateNumAssociatedConfigs(con, cfgSizeStr);
	}

	set<CyPhyML::ComponentRef> refs = container.ComponentRef_kind_children();
	for(set<CyPhyML::ComponentRef>::iterator rit=refs.begin();rit!=refs.end();++rit)
	{
		CyPhyML::ComponentRef ref = *rit;
		int cnt = getElementInConfigsCount(ref.ID());
		char buffer[10];
		_itoa(cnt, buffer, 10);
		std::string size_str = (std::string)buffer+"/"+cfgSizeStr;
		ref.NumAssociatedConfigs() = size_str;
		cfgSizeInfoMap[ref.ID()] = size_str;
	}
}
void DesertHelper::executeAll(bool applyConstraints)
{
	int cfgs = 0;
	if(allRootDCs.empty())
	{
		runCyPhy2Desert();
		if (applyConstraints) {
			applyAllConstraints(true);
		} else {
			this->applyConstraints(std::set<int>(), true);
		}
		cfgs = runDesertFinit_2();
		if (cfgs)
		{
			createNewConfigurations();
			set<int> ids;
			// int cnt = m_cfglist.GetItemCount();
			for(int i=0; i < cfgs; ++i)
			{
				ids.insert(i);
			}

			exportModel_ex(ids);
		}
	}
	else
	{
		for(set<CyPhyML::DesignContainer>::iterator it=allRootDCs.begin();it!=allRootDCs.end();++it)
		{
			rootDC = *it;
			runCyPhy2Desert();
			if (applyConstraints)
				applyAllConstraints(true);
			cfgs = runDesertFinit_2();
		}
	}
}

void DesertHelper::exportXml(std::wstring exportPath)
{
	desertIfaceFile = wstringToString(exportPath);
	runCyPhy2Desert();
}

std::wstring DesertHelper::importConfigsFromXml(std::wstring desertXmlPath, std::wstring desertBackXmlPath)
{
	ds_dn = std::unique_ptr<Udm::SmartDataNetwork>(new Udm::SmartDataNetwork(DesertIface::diagram));
	ds_dn->OpenExisting(wstringToString(desertXmlPath), "DesertIface.xsd", Udm::CHANGES_LOST_DEFAULT);

	dbs_dn = std::unique_ptr<Udm::SmartDataNetwork>(new Udm::SmartDataNetwork(DesertIfaceBack::diagram));
	dbs_dn->OpenExisting(wstringToString(desertBackXmlPath), "DesertIfaceBack.xsd", Udm::CHANGES_LOST_DEFAULT);



	DesertSystem ds;
	buildDesertSystem(ds, true);

	DesertBackSystem dbs = DesertBackSystem::Cast(dbs_dn->GetRootObject());

	remapAlternativeElementIds(ds, rootDC);

	//generate desertIfaceElemMap
	desertIfaceElemMap.clear();
	set<DesertIface::Space> spaces = ds.Space_kind_children();

	DesertIface::Space space = *(spaces.begin());
	set<DesertIface::Element> dselems = space.Element_kind_children();
	makeDesertIfaceElemMap(dselems);   //Null can exist in Desert, but not in Cyphy

	for (map<CyPhyML::DesignEntity, DesertIface::Element>::iterator mit = com2elemMap.begin(); mit != com2elemMap.end(); ++mit)
	{
		CyPhyML::DesignEntity cd = (*mit).first;
		cyPhyDesignElemMap[cd.ID()] = cd;
	}


	cfgCount = 0;
	configMap.clear();
	configElemMap.clear();

	set<DesertIfaceBack::Configuration> cfgs = dbs.Configuration_kind_children();
	for (set<DesertIfaceBack::Configuration>::iterator cfg_it = cfgs.begin(); cfg_it != cfgs.end(); ++cfg_it)
	{
		DesertIfaceBack::Configuration cfg = *cfg_it;
		int id = cfg.id();
		configMap[id - 1] = cfg;
		std::string n = cfg.name();
		cfgCount++;
		makeConfigElemMap(cfg);
	}

	//generate desertIfaceBackElemMap
	desertIfaceBackElemMap.clear();
	set<DesertIfaceBack::Element> elems = dbs.Element_kind_children();
	for (set<DesertIfaceBack::Element>::iterator it_elem = elems.begin(); it_elem != elems.end(); ++it_elem)
	{
		DesertIfaceBack::Element elem = *it_elem;
		desertIfaceBackElemMap[elem.externalID()] = elem;
	}
	desertFinit_2_fail = false;

	char cnt_buff[10];
	_itoa(cfgCount, cnt_buff, 10);
	std::string cfgSizeStr = (std::string)cnt_buff;

	updateNumAssociatedConfigs(rootDC, cfgSizeStr);
	// Also, set the NumAssociatedConfigs attribute value for the root container, see META-1686
	rootDC.NumAssociatedConfigs() = cfgSizeStr + "/" + cfgSizeStr;

	createNewConfigurations();
	set<int> ids;
	// int cnt = m_cfglist.GetItemCount();
	for (int i = 0; i < cfgCount; ++i)
	{
		ids.insert(i);
	}

	morphMatrixInitialize1();

	exportModel_ex(ids);

	return stringToWstring(cfg_models.name());
}

void DesertHelper::remapAlternativeElementIds(const DesertSystem &ds, DesignContainer &rootDC)
{
	// Find the root of the DESERT system
	set<Space> spaces = ds.Space_kind_children();

	if(spaces.empty())
	{
		throw udm_exception("DESERT system is missing space");
	}

	Space space = *(spaces.begin());

	set<DesertIface::Element> elements = space.Element_children();

	if(elements.empty())
	{
		throw udm_exception("DESERT system is missing root element");
	}

	DesertIface::Element rootElement = *(elements.begin());

	//Verify that the roots appear to be the same element
	if(string(rootElement.name()) != string(rootDC.name()))
	{
		stringstream s;
		s << "Root element name mismatch: DESERT: " << rootElement.name() << ", CyPhy: " << rootDC.name();
		throw udm_exception(s.str().c_str());
	}

	remapAlternativeElementIdsInner(rootElement, rootDC);
}

void DesertHelper::remapAlternativeElementIdsInner(DesertIface::Element &desertElement, DesignEntity &cyPhyElement)
{
	// Make sure we don't overwrite the original ID if it already exists in this map somehow
	// (i.e. it's already been changed once)
	if(CyPhy2Desert::originalIDs.count(cyPhyElement) == 0)
	{
		CyPhy2Desert::originalIDs[cyPhyElement] = cyPhyElement.ID();
	}

	cyPhyElement.ID() = desertElement.externalID();

	set<DesertIface::Element> children = desertElement.Element_children();

	if (Uml::IsDerivedFrom(cyPhyElement.type(), DesignContainer::meta)) {
		DesignContainer dc = DesignContainer::Cast(cyPhyElement);

		set<DesignEntity> cyPhyChildren = dc.DesignEntity_kind_children();

		for (auto it = children.begin(); it != children.end(); it++)
		{
			// Find matching cyphy element child
			string name = it->name();

			if(name == "null")
			{
				continue;
			}

			auto matchingElement = find_if(cyPhyChildren.begin(), cyPhyChildren.end(), [name](DesignEntity cpc)->bool { return name == string(cpc.name()); });
			if(matchingElement == cyPhyChildren.end())
			{
				stringstream s;
				s << "Couldn't find matching CyPhy element for " << name;
				throw udm_exception(s.str().c_str());
			}

			DesertIface::Element nextElement = *it;
			DesignEntity nextEntity = *matchingElement;

			remapAlternativeElementIdsInner(nextElement, nextEntity);
		}
	} else {
		if(!children.empty())
		{
			stringstream s;
			s << "DESERT element " << desertElement.name() << " should not have children";
			throw udm_exception(s.str().c_str());
		}
	}
}

void DesertHelper::checkConfigurationsModel(set<int> &constraintId_list, bool applyAll)
{
	set<CyPhyML::Configurations> cfgs;
	if(rootDC!=Udm::null)
		cfgs = rootDC.Configurations_kind_children();
	else if(rootDS!=Udm::null)
		cfgs = rootDS.Configurations_kind_children();
	else
		return;

	CyPhyML::Configurations cfg;

	for(set<CyPhyML::Configurations>::iterator it=cfgs.begin();it!=cfgs.end();++it)
	{
		CyPhyML::Configurations currcfg = *it;
		if(currcfg.isArchived()) continue;
		if(applyAll)
		{
			if(currcfg.applyAll())
			{
				cfg = currcfg;
				break;
			}
		}
		else
		{
			set<int> conIdlist;
			set<CyPhyML::ConstraintRef> conrefs = currcfg.ConstraintRef_kind_children();
			for(set<CyPhyML::ConstraintRef>::iterator i=conrefs.begin();i!=conrefs.end();++i)
			{
				CyPhyML::ConstraintBase conBase = (*i).ref();
				conIdlist.insert(conBase.ID());
			}
			if(conIdlist == constraintId_list)
			{
				cfg = currcfg;
		//		old_cfgs_model = cfg;
				break;
			}
		}
	}

	if(cfg!=Udm::null)
	{
	//	set<CyPhyML::CWCReference> cfgrefs = cfg.referedbyCWCReference();
	//	for(set<CyPhyML::CWCReference>::iterator ref_it=cfgrefs.begin();ref_it!=cfgrefs.end();++ref_it)
	//	{
	//		CyPhyML::CWCReference cwcref = *ref_it;
	//		cwcref.DeleteObject();
	//	}
		cfg.DeleteObject();
	}
}

bool DesertHelper::isOptionalElement(const DesertIface::Element &elem)
{
	set<DesertIface::Element> elems = elem.Element_kind_children();
	for(set<DesertIface::Element>::iterator i=elems.begin();i!=elems.end();++i)
	{
		DesertIface::Element currElem = *i;
		if((std::string)currElem.name()=="null" && currElem.externalID()<=0)
			return true;
	}
	return false;
}

set<std::string> DesertHelper::getNaturalParameters()
{
	return naturalParameters;
}

set<std::string> DesertHelper::getCustomParameters()
{
	return customParameters;
}

void DesertHelper::removeCfgsIfEmpty()
{
	if(cfg_models!=Udm::null)
	{
		set<CyPhyML::CWC> cwcs = cfg_models.CWC_kind_children();
		if(cwcs.empty())
		{
			cfg_models.DeleteObject();
			cfg_models = 0;
		}
	}
}

bool DesertHelper::isBackNavigable()
{
	return isDesertBackNavigable();
}

bool DesertHelper::isForwardNavigable()
{
	return isDesertForwardNavigable();
}

void DesertHelper::goBack()
{
	Desert_goBack();
	updateRealNoOfConfigurationsCount();
}

void DesertHelper::goForward()
{
	Desert_goForward();
	updateRealNoOfConfigurationsCount();
}

long DesertHelper::getRealConfigCount()
{
	return m_realConfigCount;
}

void DesertHelper::updateRealNoOfConfigurationsCount()
{
	try{
		this->m_realConfigCount = Desert_getRealNoOfConfigurations();
	}
	catch(CDesertException *e)
	{
		return;
	}
}

bool DesertHelper::isLastDesertFinit_2_fail()
{
	return desertFinit_2_fail;
}

int DesertHelper::getElementInConfigsCount(int elemId)
{
	//add code here:
	map<int, DesertIfaceBack::Element>::iterator pos = desertIfaceBackElemMap.find(elemId);
	if(pos!=desertIfaceBackElemMap.end())
	{
		DesertIfaceBack::Element elem = (*pos).second;
		set<DesertIfaceBack::AlternativeAssignment> alts = elem.alternative_of();
		if(alts.empty())
			alts = elem.alternative();
		return alts.size();
	}

	map<int, DesertIface::Element>::iterator ipos = desertIfaceElemMap.find(elemId);
	if(ipos==desertIfaceElemMap.end())  //for the root designspace
		return cfgCount;

	DesertIface::Element ielem = (*ipos).second;

	Udm::Object parent_obj = ielem.parent();
	if(Uml::IsDerivedFrom(parent_obj.type(), DesertIface::Element::meta)) //&& elem_parent.decomposition())
	{
		DesertIface::Element elem_parent = DesertIface::Element::Cast(parent_obj);
		if(elem_parent.decomposition())
		{
			return getElementInConfigsCount(elem_parent.externalID());
		}
		else
		{
			//map<int, DesertIfaceBack::Element>::iterator apos = desertIfaceBackElemMap.find(elem_parent.externalID());
			//if(apos!=desertIfaceBackElemMap.end())  //it should not execute here
			//{
			//	set<DesertIfaceBack::AlternativeAssignment> alts = ((*apos).second).alternative();
			//	return alts.size();
			//}
			//else
				return 0;
		}
	}

	return cfgCount;
}

std::string DesertHelper::getElementInConfigsString(int elemId)
{
	std::string ret;
	map<int, std::string>::iterator pos = cfgSizeInfoMap.find(elemId);
	if(pos!=cfgSizeInfoMap.end())
		ret = (*pos).second;
	return ret;
}

bool DesertHelper::isCfgModelsNull()
{
	return (cfg_models==Udm::null);
}

void DesertHelper::removeCfgs()
{
	if(cfg_models!=Udm::null)
	{
		cfg_models.DeleteObject();
		cfg_models = 0;
	}
}

std::string DesertHelper::getElementType(int externalId)
{
	map<int, DesertIface::Element>::iterator dit = desertIfaceElemMap.find(externalId);
	if(dit==desertIfaceElemMap.end())
		return "";
	else
	{
		DesertIface::Element elem = (*dit).second;
		if(elem.decomposition())
			return "Compound";
		else
		{
			set<DesertIface::Element> child_elems = elem.Element_kind_children();
			if(!child_elems.empty())
			{
				if(isOptionalElement(elem))
					return "Optional";
				else
					return "Alternative";
			}
			else
				return "Component";
		}
	}
}

set<int> DesertHelper::getConfigurationsfromElement(int externalId)
{
	set<int> ret;
	map<int, DesertIfaceBack::Element>::iterator pos = desertIfaceBackElemMap.find(externalId);
	if(pos!=desertIfaceBackElemMap.end())
	{
		DesertIfaceBack::Element delem = (*pos).second;

		set<DesertIfaceBack::AlternativeAssignment> alts = delem.alternative_of();
		set<DesertIfaceBack::AlternativeAssignment> alt_of_ends = delem.alternative();
		alts.insert(alt_of_ends.begin(), alt_of_ends.end());
		for(set<DesertIfaceBack::AlternativeAssignment>::iterator it=alts.begin();it!=alts.end();++it)
		{
			DesertIfaceBack::AlternativeAssignment alt = *it;
			DesertIfaceBack::Configuration cfg = alt.parent();
			ret.insert((int)cfg.id()-1);
		}
		return ret;
	}

	map<int, DesertIface::Element>::iterator ipos = desertIfaceElemMap.find(externalId);
	if(ipos==desertIfaceElemMap.end())  //for the root designspace
		return ret;

	DesertIface::Element ielem = (*ipos).second;

	Udm::Object parent_obj = ielem.parent();
	if(Uml::IsDerivedFrom(parent_obj.type(), DesertIface::Element::meta)) //&& elem_parent.decomposition())
	{
		DesertIface::Element elem_parent = DesertIface::Element::Cast(parent_obj);
		return getConfigurationsfromElement(elem_parent.externalID());
	}

	for(int i=0;i<cfgCount;++i)
		ret.insert(i);

	return ret;
}

int DesertHelper::getConfigurationExternalId(int cfgId)
{
	int eid = 0;
	map<int, DesertIfaceBack::Configuration>::iterator pos = configMap.find(cfgId);
	if(pos!=configMap.end())
	{
		DesertIfaceBack::Configuration cfg = (*pos).second;
		eid = cfg.uniqueId();
	}
	return eid;
}

void DesertHelper::clearCfgId2NameMap()
{
	cfgId2NameMap.clear();
}

void DesertHelper::updateCfgId2NameMap(int id, const std::string &name)
{
	cfgId2NameMap[id] = name;
}

CyPhyML::Or_operator DesertHelper::getOrFromDesignContainer(CyPhyML::DesignContainer &dc)
{
	CyPhyML::Or_operator or;
	map<CyPhyML::DesignContainer, CyPhyML::Or_operator>::iterator or_it = dc2orMap.find(dc);
	if(or_it!=dc2orMap.end())
		or = (*or_it).second;
	else
	{
		CyPhyML::VisualConstraint newvc = CyPhyML::VisualConstraint::Create(dc);
		char buffer [80];
		_ltoa ((long) newvc.uniqueId(),buffer,10);
		newvc.name() = (std::string)dc.name()+"_selective_VC_"+buffer;

		or = CyPhyML::Or_operator::Create(newvc);
		or.name() = "Or";
		dc2orMap[dc] = or;
	}
	return or;
}

void DesertHelper::generateSelectionConstraints(set<int> &selectElemIds)
{
	set<CyPhyML::Or_operator> ors;
	for(set<int>::iterator i=selectElemIds.begin();i!=selectElemIds.end();++i)
	{
		set<int>::iterator set_it = existedElemId_in_VC.find(*i);
		if(set_it!=existedElemId_in_VC.end())
			continue;

		existedElemId_in_VC.insert(*i);

		map<int, CyPhyML::DesignEntity>::iterator pos = cyPhyDesignElemMap.find(*i);
		if(pos!=cyPhyDesignElemMap.end())
		{
			CyPhyML::DesignEntity dentity = (*pos).second;
			Udm::Object dparent = dentity.parent();
			if(Uml::IsDerivedFrom(dparent.type(), CyPhyML::DesignContainer::meta))
			{
				CyPhyML::DesignContainer dc = CyPhyML::DesignContainer::Cast(dparent);
				if((std::string)dc.ContainerType()=="Compound")
					continue;

				CyPhyML::Or_operator or = getOrFromDesignContainer(dc);
				ors.insert(or);
				CyPhyML::DesignEntityRef ref = CyPhyML::DesignEntityRef::Create(or);
				ref.ref() = dentity;
				ref.name() = dentity.name();
			}
		}
		else
		{
			map<int, DesertIface::Element>::iterator dit = desertIfaceElemMap.find(*i);
			if(dit!=desertIfaceElemMap.end())
			{
				DesertIface::Element delem = (*dit).second;
				if((std::string)delem.name()=="null" && (*i)<=0)
				{
					DesertIface::Element delem_parent = delem.Element_parent();
					pos = cyPhyDesignElemMap.find(delem_parent.externalID());
					if(pos!=cyPhyDesignElemMap.end())
					{
						CyPhyML::DesignEntity dentity = (*pos).second;
						if(Uml::IsDerivedFrom(dentity.type(), CyPhyML::DesignContainer::meta))
						{
							CyPhyML::DesignContainer dc = CyPhyML::DesignContainer::Cast(dentity);
							ASSERT((std::string)dc.ContainerType()=="Optional");

							CyPhyML::Or_operator or = getOrFromDesignContainer(dc);
							ors.insert(or);
							CyPhyML::NullOptionRef nullref = CyPhyML::NullOptionRef::Create(or);
							nullref.ref() = dc;
							nullref.name() = dentity.name();
						}
					}
				}
			}
		}
	}

	//generate the constraint in Desert
	if(dconstraintSet==Udm::null)
	{
		DesertIface::DesertSystem desert_top = DesertIface::DesertSystem::Cast(ds_dn->GetRootObject());
		dconstraintSet = DesertIface::ConstraintSet::Create(desert_top);
	}

	CyPhy2Desert c2d;
	for(set<CyPhyML::Or_operator>::iterator or_it=ors.begin();or_it!=ors.end();++or_it)
	{
		DesertIface::Constraint dsConstraint;
		CyPhyML::VisualConstraint vcon = (*or_it).VisualConstraint_parent();
		CyPhyML::DesignContainer dcon = vcon.parent();
		map<CyPhyML::DesignEntity, DesertIface::Element>::iterator c2d_it = com2elemMap.find(dcon);
		if(c2d_it == com2elemMap.end())
			continue;

		std::string expr = c2d.generateVisualConstraint(dcon, vcon);
		if(expr.empty()) continue;

		map<CyPhyML::Or_operator, DesertIface::Constraint>::iterator o2c_it = or2ConstraintMap.find(*or_it);
		if(o2c_it==or2ConstraintMap.end())
		{
			//create the constraint in desert
			dsConstraint = DesertIface::Constraint::Create(dconstraintSet);
			dsConstraint.name() = vcon.name();
			dsConstraint.id() = dsConstraint.externalID() = vcon.ID();
			dsConstraint.context() = (*c2d_it).second;
			newConstraintMap[constraintCount] = dsConstraint;
			dcon2CyphyConMap[dsConstraint]=vcon;
			or2ConstraintMap[*or_it]=dsConstraint;
			makeConstraintDomainMap(constraintCount, dsConstraint, "Visual Constraints");
			constraintCount++;
		}
		else
			dsConstraint = (*o2c_it).second;

		dsConstraint.expression() = "constraint "+ (std::string)vcon.name()+"() {\n" + expr+"\n}";
	}
}

void DesertHelper::makeConstraintDomainMap(int idx, DesertIface::Constraint &con, std::string ctype)
{
	std::string cdomain;
	CyPhyML::ConstraintBase cbase;
	map<DesertIface::Constraint, CyPhyML::ConstraintBase>::iterator c2c_pos = dcon2CyphyConMap.find(con);
	if(c2c_pos!=dcon2CyphyConMap.end())
	{
		cbase = (*c2c_pos).second;
		std::string cbasetype = cbase.type().name();
		if(cbasetype=="Constraint")
			ctype = "OCL";
		else if(cbasetype=="VisualConstraint")
			ctype = "Visual Constraints";
		else if(cbasetype=="PropertyConstraint")
			ctype = "Property Constraints";

		cdomain = cbase.Domain();
	}
	else
	{
		if(ctype.empty())
			ctype = "Parameter Constraints";
	}

	set<int> conset;
	map<std::string, set<int> >::iterator pos = constraintDomainMap.find(ctype);
	if(pos != constraintDomainMap.end())
		conset = (*pos).second;
	conset.insert(idx);
	constraintDomainMap[ctype] = conset;
	if(!cdomain.empty())
	{
		set<std::string> domains;
		std::string delimiter = ";";

		size_t dpos = 0;
		std::string token;
		while ((dpos = cdomain.find(delimiter)) != std::string::npos)
		{
			token = capitalizeFirstLetter(cdomain.substr(0, dpos));
			domains.insert(token);
			cdomain.erase(0, dpos + delimiter.length());
		}
		if(!cdomain.empty())
			domains.insert(capitalizeFirstLetter(cdomain));

		if(cbase!=Udm::null)
		{
			std::string sort_domains;
			set<std::string>::iterator it=domains.begin();
			sort_domains = (*it);
			for(it++;it!=domains.end();++it)
			{
				sort_domains = sort_domains+";"+(*it);
			}
			cbase.Domain() = sort_domains;
		}

		for(set<std::string>::iterator it=domains.begin();it!=domains.end();++it)
		{
			pos = constraintDomainMap.find(*it);
			set<int> domain_conset;
			if(pos!=constraintDomainMap.end())
				domain_conset = (*pos).second;
			domain_conset.insert(idx);
			constraintDomainMap[*it] = domain_conset;
		}
	}
}

set<std::string> DesertHelper::getDomainSet()
{
	set<std::string> ret;
	for(auto pos=constraintDomainMap.begin();pos!=constraintDomainMap.end();++pos)
	{
		ret.insert((*pos).first);
	}
	return ret;
}

set<int>  DesertHelper::getDesertConstraintlist(const std::string &domain)
{
	set<int> ret;
	map<std::string, set<int> >::iterator pos = constraintDomainMap.find(domain);
	if(pos!=constraintDomainMap.end())
	{
		return (*pos).second;
	}
	return ret;
}
