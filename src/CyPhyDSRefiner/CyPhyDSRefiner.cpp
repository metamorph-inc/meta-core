#ifndef CYPHYDSREFINER_CPP
#define CYPHYDSREFINER_CPP

#include "CyPhyDSRefiner.h"
#include "uml.h"
#include "UmlExt.h"
#include "CyPhyUtil.h"
#include <queue>
//#include "StatusDialog.h"

CyPhyDSRefiner::CyPhyDSRefiner(CyPhyML::Configurations &cfgs)
	:_cfgsModel(cfgs)
{
	_rootDS = _cfgsModel.DesignContainer_parent();
	_cwcs = _cfgsModel.CWC_kind_children();
}

CyPhyDSRefiner::CyPhyDSRefiner(CyPhyML::Configurations &cfgs, set<CyPhyML::CWC> &cwcs)
	:_cfgsModel(cfgs), _cwcs(cwcs)
{
	_rootDS = _cfgsModel.DesignContainer_parent();
}


void CyPhyDSRefiner::runDSRefiner()
{	
	//copy the original designspace
	prgDlg.SetProgress("Run UdmCopy from original DesignSpace...");
	int timer=0;
	while(timer<10)
	{
		prgDlg.SetProgress("Run UdmCopy from original DesignSpace...");
		timer++;
	}

	CyPhyML::DesignSpace dsFdr;
	Udm::Object dsparent = _rootDS.parent();
	while(dsparent.type()!=CyPhyML::DesignSpace::meta)
	{
		dsparent = dsparent.GetParent();
	}

	dsFdr = CyPhyML::DesignSpace::Cast(dsparent);

	std::map<Uml::Class, Uml::AssociationRole> refMemo;
	std::queue<Udm::Object> dsChildren;
	dsChildren.push(_rootDS);
	while (!dsChildren.empty())
	{
		Udm::Object child = dsChildren.front();
		dsChildren.pop();
		std::set<Udm::Object> childObjects = child.GetChildObjects();
		for (auto it = childObjects.begin(); it != childObjects.end(); it++)
		{
			dsChildren.push(*it);
		}
		Uml::Class type = child.type();
		if (type == CyPhyML::ComponentRef::meta)
		{
			Udm::Object comp = CyPhyML::ComponentRef::Cast(child).ref();
			if(comp==Udm::null)
				continue;
			_udmcopyMap[comp] = comp;
			std::set<Udm::Object> compChildren = comp.GetChildObjects();
			std::for_each(compChildren.begin(), compChildren.end(), [&](const Udm::Object& o) { _udmcopyMap[o] = o; });
		}
	}

	_newDS = CyPhyML::DesignContainer::Create(dsFdr);
	UdmUtil::CopyObjectHierarchy(_rootDS.__impl(), _newDS.__impl(),_rootDS.__impl()->__getdn(), _udmcopyMap);
	_newDS.name() = (std::string)_rootDS.name()+"_Refined";
	_udmcopyMap[_rootDS] = _newDS;

	preProcessDS(_newDS);
	processDecisionGroup(_rootDS);
	createConstraint();

	CyPhyML::DesignSpaceRef dsref = CyPhyML::DesignSpaceRef::Create(_newDS);
	dsref.ref() = _rootDS;
	dsref.name() = _rootDS.name();

	prgDlg.OnFinished();
}

CyPhyML::DesignContainer CyPhyDSRefiner::getRefinedDS()
{
	return _newDS;
}

void CyPhyDSRefiner::preProcessDS(CyPhyML::DesignContainer &dc)
{
	//delete some unwanted objects
	prgDlg.SetProgress("Remove redundant objects...");
	DOEVENTS();
	set<CyPhyML::ConstraintBase> cons = dc.ConstraintBase_kind_children();
	for(set<CyPhyML::ConstraintBase>::iterator it_con=cons.begin();it_con!=cons.end();++it_con)
	{
		CyPhyML::ConstraintBase con = *it_con;
		if(con.type()==CyPhyML::DecisionGroup::meta) continue;
		con.DeleteObject();
	}
	set<CyPhyML::Configurations> cfgss = dc.Configurations_kind_children();
	for(set<CyPhyML::Configurations>::iterator it_cfg=cfgss.begin();it_cfg!=cfgss.end();++it_cfg)
	{
		CyPhyML::Configurations cfgs = *it_cfg;
		cfgs.DeleteObject();
	}
	set<CyPhyML::CWCReference> cwcrefs = dc.CWCReference_kind_children();
	for(set<CyPhyML::CWCReference>::iterator it_cwcref=cwcrefs.begin();it_cwcref!=cwcrefs.end();++it_cwcref)
	{
		CyPhyML::CWCReference cwcref = *it_cwcref;
		cwcref.DeleteObject();
	}

	if((std::string)dc.ContainerType()=="Optional")
		_optionalContainers.insert(dc);

	set<CyPhyML::DesignContainer> containers = dc.DesignContainer_kind_children();
	for(set<CyPhyML::DesignContainer>::iterator it_ct=containers.begin();it_ct!=containers.end();++it_ct)
	{
		CyPhyML::DesignContainer container = *it_ct;
		preProcessDS(container);
	}
}

void CyPhyDSRefiner::processDecisionGroup(CyPhyML::DesignContainer &dc)
{
	prgDlg.SetProgress("Process DecisionGroup...");
	DOEVENTS();
	set<CyPhyML::DecisionGroup> dgs = dc.DecisionGroup_kind_children();	
	_allDecisionGroups.insert(dgs.begin(), dgs.end());

	for(set<CyPhyML::DecisionGroup>::iterator it_dg=dgs.begin();it_dg!=dgs.end();it_dg++)
	{
		prgDlg.SetProgress("Process DecisionGroup...");
		CyPhyML::DecisionGroup dg = *it_dg;
		set<CyPhyML::DesignEntityRef> refs = dg.DesignEntityRef_kind_children();
		if(refs.empty())
			continue;
		refs.erase(refs.begin());
		for(set<CyPhyML::DesignEntityRef>::iterator it_ref=refs.begin();it_ref!=refs.end();++it_ref)
		{
			CyPhyML::DesignEntityRef ref = *it_ref;
			_groupEntities.insert(ref.ref());
		}
	}
}

void CyPhyDSRefiner::createConstraint()
{
	set<CyPhyML::DesignEntity> allDesignEntities;
	analyseDS(_rootDS, allDesignEntities);

	//removed none-involed element
	for(set<CyPhyML::CWC>::iterator it=_cwcs.begin();it!=_cwcs.end();++it)
	{
		prgDlg.SetProgress("Create constraints...");
		DOEVENTS();
		set<CyPhyML::DesignEntityRef> refs = (*it).DesignEntityRef_kind_children();
		for(set<CyPhyML::DesignEntityRef>::iterator it_ref=refs.begin();it_ref!=refs.end();++it_ref)
		{
			CyPhyML::DesignEntity de = (*it_ref).ref();
			set<CyPhyML::DesignEntity>::iterator pos = allDesignEntities.find(de);
			if(pos!=allDesignEntities.end())
				allDesignEntities.erase(pos);
		}
	}
		
	for(set<CyPhyML::DesignEntity>::iterator it_de=allDesignEntities.begin();it_de!=allDesignEntities.end();++it_de)
	{
		DOEVENTS();
		CyPhyML::DesignEntity src_entity = *it_de;
		UdmUtil::copy_assoc_map::iterator pos=_udmcopyMap.find(src_entity);
		if(pos!=_udmcopyMap.end())
		{
			CyPhyML::DesignEntity dst_entity = CyPhyML::DesignEntity::Cast((*pos).second);
			Udm::Object entity_parent = dst_entity.parent();
			if(entity_parent.isInstance() || entity_parent.isSubtype())
			//	throw udm_exception((std::string)dst_entity.name()+" is instance/subtype.");
				continue;
			dst_entity.DeleteObject();
		}
	}
	cleanDecisionGroup();

	//create constraint: using "Or" operator between Cfg1, Cfg2
	//Using "And" operator for every components composed of each configuration
	//to be successfully refined, applied all the generated constraints, shoud get the same configuration set as it selects for refinning
	CyPhyML::DesignContainer dst_con = CyPhyML::DesignContainer::Cast(getMappedObject(_rootDS));
	CyPhyML::VisualConstraint vcon = CyPhyML::VisualConstraint::Create(dst_con);
	vcon.name() = "visualConstraint";
	CyPhyML::Or_operator or = CyPhyML::Or_operator::Create(vcon);
	or.name() = "Or";
	for(set<CyPhyML::CWC>::iterator it=_cwcs.begin();it!=_cwcs.end();++it)
	{
		set<CyPhyML::DesignEntityRef> refs = (*it).DesignEntityRef_kind_children();
		set<CyPhyML::NullOptionRef> nullrefs = (*it).NullOptionRef_kind_children();
		if(refs.empty() && nullrefs.empty())
			continue;

		CyPhyML::And_operator and = CyPhyML::And_operator::Create(or);
		and.name() = (std::string)(*it).name();
		for(set<CyPhyML::DesignEntityRef>::iterator it_ref=refs.begin();it_ref!=refs.end();++it_ref)
		{
			CyPhyML::DesignEntity de = (*it_ref).ref();
			if(_activeDesignEntities.find(de)==_activeDesignEntities.end())
				continue;
			CyPhyML::DesignEntity dst_de = CyPhyML::DesignEntity::Cast(getMappedObject(de));
			CyPhyML::DesignEntityRef dst_ref = CyPhyML::DesignEntityRef::Create(and);
			dst_ref.ref() = dst_de;
			dst_ref.name() = (std::string)dst_de.name();
		}
		
		for(set<CyPhyML::NullOptionRef>::iterator it_null=nullrefs.begin();it_null!=nullrefs.end();++it_null)
		{
			CyPhyML::DesignContainer ncon = (*it_null).ref();
			CyPhyML::DesignContainer dst_ncon = CyPhyML::DesignContainer::Cast(getMappedObject(ncon));
			CyPhyML::NullOptionRef dst_null = CyPhyML::NullOptionRef::Create(and);
			dst_null.ref() = dst_ncon;
			dst_null.name() = (std::string)ncon.name()+"_null";
		}
	}
}

void CyPhyDSRefiner::analyseDS(CyPhyML::DesignContainer &dc, set<CyPhyML::DesignEntity> &allDesignEntities)
{
	
	bool ignore = (_groupEntities.find(dc)!=_groupEntities.end());

	if((std::string)dc.ContainerType()!="Compound")
	{
		set<CyPhyML::DesignElement> delems = dc.DesignElement_kind_children();
		set<CyPhyML::ComponentRef> refs = dc.ComponentRef_kind_children();
		if(!ignore)
		{
			_activeDesignEntities.insert(delems.begin(), delems.end());
			_activeDesignEntities.insert(refs.begin(), refs.end());
		}
		allDesignEntities.insert(delems.begin(), delems.end());
		allDesignEntities.insert(refs.begin(), refs.end());
	}

	set<CyPhyML::DesignContainer> containers = dc.DesignContainer_kind_children();
	if(containers.empty())
	{
		Udm::Object dsParent = dc.parent();
		if(dsParent.type()==CyPhyML::DesignContainer::meta)
		{
			CyPhyML::DesignContainer dcp = CyPhyML::DesignContainer::Cast(dsParent);
			if((std::string)dcp.ContainerType()!="Compound" && !ignore)
				_activeDesignEntities.insert(dc);
		}
		return;
	}

	for(set<CyPhyML::DesignContainer>::iterator it=containers.begin();it!=containers.end();++it)
	{
		CyPhyML::DesignContainer con = *it;
		analyseDS(con, allDesignEntities);
	}
}

void CyPhyDSRefiner::cleanDecisionGroup()
{
	for(set<CyPhyML::DecisionGroup>::iterator it=_allDecisionGroups.begin();it!=_allDecisionGroups.end();++it)
	{
		CyPhyML::DecisionGroup src_dg = *it;
		CyPhyML::DecisionGroup dst_dg = CyPhyML::DecisionGroup::Cast(getMappedObject(src_dg));
		if(dst_dg==Udm::null)
			throw udm_exception("DecisionGroup:" + (std::string)src_dg.name()+" has no copy-mapped object.");
		set<CyPhyML::DesignEntityRef> refs = dst_dg.DesignEntityRef_kind_children();
		for(set<CyPhyML::DesignEntityRef>::iterator it_ref=refs.begin();it_ref!=refs.end();++it_ref)
		{
			CyPhyML::DesignEntityRef ref = *it_ref;
			CyPhyML::DesignEntity de = ref.ref();
			if(de == Udm::null)
				ref.DeleteObject();
		}
	}
}

Udm::Object CyPhyDSRefiner::getMappedObject(const Udm::Object &src_obj)
{
	Udm::Object ret;
	UdmUtil::copy_assoc_map::iterator pos=_udmcopyMap.find(src_obj);
	if(pos!=_udmcopyMap.end())
		ret = (*pos).second;

	return ret;
}

void CyPhyDSRefiner::startProgressBar()
{
	prgDlg.Create(IDD_PROGRESS_DIALOG);
	GetStatusDlg(&prgDlg);
	prgDlg.SetRange(100);
}

void CyPhyDSRefiner::endProgressBar()
{
	prgDlg.OnFinished();
}
#endif
