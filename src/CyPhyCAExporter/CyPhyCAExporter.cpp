#ifndef CYPHYCAEXPORTER_CPP
#define CYPHYCAEXPORTER_CPP

#include "CyPhyCAExporter.h"
#include "Uml.h"
#include "UmlExt.h"
#include "UdmUtil.h"


CyPhyCAExporter::CyPhyCAExporter(CyPhyML::ComponentAssemblies &cyphy_cas, CyPhyML::CWC &cyphy_cwc, bool cyphy_flatten)
	:ca_folder(cyphy_cas), cwc(cyphy_cwc), flatten(cyphy_flatten)
{
	container2caMap.clear();
}

void CyPhyCAExporter::createComponentAssembly()
{
	//1. Create output ComponentAssembly model 
	//2. Put the selected Component in appopriate place
	//	2.1 if the ComponentAssembly is flattern, create all ComponentRefs directly inside
	//	2.2 if the ComponentAssembly is hierarchical, create a ComponentAssmbly for each CompoundContainer, 
	//		and put the generated ComponentRef to the top nearby ComponentAssembly.
	//3. Make the connection for the ComponentRef
	CyPhyML::ComponentAssemblyRef caref;
	try{
		CyPhyML::Configurations cfgs = cwc.Configurations_parent();
		rootDC = cfgs.DesignContainer_parent();	
		ca_model = CyPhyML::ComponentAssembly::Create(ca_folder);
		ca_model.ConfigurationUniqueID() = cwc.ID();
		ca_model.ID() = rootDC.ID();
		//ca_model.name() = (std::string)rootDC.name()+"_"+(std::string)cwc.name();
		std::string dcname = rootDC.name();
		std::string cwcname = cwc.name();
		ca_model.name() = dcname+"_"+cwcname;

		container2caMap[rootDC] = ca_model;

		caref = CyPhyML::ComponentAssemblyRef::Create(cfgs);
		caref.ref() = ca_model;
		caref.name() = ca_model.name();
		CyPhyML::Config2CA cfg2ca = CyPhyML::Config2CA::Create(cfgs);
		cfg2ca.srcConfig2CA_end() = cwc;
		cfg2ca.dstConfig2CA_end() = caref;

		init(rootDC, ca_model);
		//copy ports
		set<CyPhyML::Port> ports = rootDC.Port_kind_children();
		for(set<CyPhyML::Port>::iterator port_it=ports.begin();port_it!=ports.end();++port_it)
		{
			CyPhyML::Port port = *port_it;
			copyDesignContainerElement(port, ca_model);
		}

		containersMap[rootDC.uniqueId()]=1;
		if(flatten)
		{
			copyPropertyAndParameter(rootDC, ca_model);
		}

		set<CyPhyML::DesignEntityRef> comrefs = cwc.DesignEntityRef_kind_children();
		for(set<CyPhyML::DesignEntityRef>::iterator i=comrefs.begin();i!=comrefs.end();++i)
		{
			if (showGui)
			{
				DOEVENTS();
			}
			CyPhyML::DesignEntity entity = CyPhyML::DesignEntity::Cast((*i).ref());
			CyPhyML::ComponentRef comref;
			if(Uml::IsDerivedFrom(entity.type(), CyPhyML::DesignElement::meta))
			{
				CyPhyML::DesignElement elem = CyPhyML::DesignElement::Cast(entity);
				Udm::Object elem_parent = elem.parent();
				if(!Uml::IsDerivedFrom(elem_parent.type(), CyPhyML::DesignContainer::meta))
				{
					throw udm_exception("DesignElement: <"+std::string(elem.name())+"> doesn't have DesignContainer type parent.");
				}
				comref = createComponentRef(elem, CyPhyML::DesignContainer::Cast(elem_parent));
				copyMap[elem] = comref;
			}
			else if(Uml::IsDerivedFrom(entity.type(), CyPhyML::ComponentRef::meta))
			{
				CyPhyML::ComponentRef old_comref = CyPhyML::ComponentRef::Cast(entity);
				Udm::Object ref_parent = old_comref.parent();
				if(!Uml::IsDerivedFrom(ref_parent.type(), CyPhyML::DesignContainer::meta))
				{
					throw udm_exception("ComponentRef: <"+std::string(comref.name())+"> doesn't have DesignContainer type parent.");
				}

				comref = createComponentRef(old_comref, CyPhyML::DesignContainer::Cast(ref_parent));
				copyMap[old_comref] = comref;
			}
			else if(Uml::IsDerivedFrom(entity.type(), CyPhyML::DesignContainer::meta))
			{
				// Check to see if a ComponentAssembly has been generated for this.
				// If so, create a link in the CWC.
				CyPhyML::DesignContainer dc = CyPhyML::DesignContainer::Cast(entity);
				CyPhyML::ComponentAssembly ca = container2caMap[dc];
				if (ca != Udm::null)
				{
					CyPhyML::BuiltDesignEntityRef bder = CyPhyML::BuiltDesignEntityRef::Create(cwc);
					bder.name() = (*i).name();
					bder.ref() = ca;
					CyPhyML::Built b = CyPhyML::Built::Create(cwc);
					b.srcBuilt_end() = *i;
					b.dstBuilt_end() = bder;
				}
			}

			// If a ComponentRef was generated, create a link in the CWC.
			if (comref != Udm::null)
			{
				CyPhyML::BuiltDesignEntityRef bder = CyPhyML::BuiltDesignEntityRef::Create(cwc);
				bder.name() = (*i).name();
				bder.ref() = comref;
				CyPhyML::Built b = CyPhyML::Built::Create(cwc);
				b.srcBuilt_end() = *i;
				b.dstBuilt_end() = bder;
			}
		}

		if(!flatten)
			createConnections(rootDC);
		else
			createFlattenedConnections(rootDC);

		postProcessComponentAssembly(ca_model);
	}
	catch(udm_exception &exc)
	{
		caref.DeleteObject();
		throw exc;
	}
}

void CyPhyCAExporter::init(CyPhyML::DesignContainer &container,CyPhyML::ComponentAssembly &ca)
{
	if(flatten)
	{
		set<CyPhyML::ValueFormula> vfs = container.ValueFormula_kind_children();
		for(set<CyPhyML::ValueFormula>::iterator it_vf=vfs.begin();it_vf!=vfs.end();++it_vf)
		{
			copyDesignContainerElement(CyPhyML::ValueFormula::Cast(*it_vf), ca);
		}
	}
	
	set<CyPhyML::DesignContainer> dcons = container.DesignContainer_kind_children();
	for(set<CyPhyML::DesignContainer>::iterator i=dcons.begin();i!=dcons.end();++i)
	{
		CyPhyML::DesignContainer dcon = *i;
		std::string conType = dcon.ContainerType();
		if(conType=="Compound") 
		{
			containersMap[dcon.uniqueId()]=1;
			std::string genSemantics = dcon.GenerationSemantics();
			if((genSemantics == "Normal" && !flatten) || genSemantics == "AsCA")
			{
				CyPhyML::ComponentAssembly newca = CyPhyML::ComponentAssembly::Create(ca);
				newca.name() = dcon.name();
				newca.ID() = dcon.ID();
				newca.position() = dcon.position();
				container2caMap[dcon] = newca;
				copyPropertyAndParameter(dcon, newca);
				init(dcon, newca);
			}
			else
			{
				container2caMap[dcon] = ca;
				init(dcon, ca);
			}
		}
		else if(conType=="Alternative") 
		{
			if(isSelected(dcon))
				containersMap[dcon.uniqueId()]=1;
			else
				containersMap[dcon.uniqueId()]=0;
			
			container2caMap[dcon] = ca;
			init(dcon, ca);
		}
		else //Optional
		{
			if(isSelected(dcon))
				containersMap[dcon.uniqueId()]=2;
			else
				containersMap[dcon.uniqueId()]=3;

			container2caMap[dcon] = ca;
			init(dcon, ca);
		}
	}

	//copy geometry model
	set<CyPhyML::GeometryBase> geos = container.GeometryBase_kind_children();
	for(auto geo_it=geos.begin();geo_it!=geos.end();++geo_it)
	{
		CyPhyML::GeometryBase geo = *geo_it;
		copyDesignContainerElement(geo, ca);
	}
}

CyPhyML::ComponentAssembly CyPhyCAExporter::getComponentAssembly()
{
	return ca_model;
}

void CyPhyCAExporter::copyDesignContainerElement(Udm::Object &from_obj, CyPhyML::ComponentAssembly &to_parent)
{
	if(to_parent==Udm::null) return;
	Udm::Object from_parent = from_obj.GetParent();
	Udm::Object to_obj;
	Uml::Class objType = from_obj.type();
	set<Uml::CompositionParentRole> parentRoles = objType.parentRoles();
	if((from_obj.isInstance() || from_obj.isSubtype()) &&!parentRoles.empty() && !to_parent.isInstance())
	{
		int arch_level = 0;
		Udm::Object arch_obj = from_obj.archetype();
		map<int, Udm::Object> fromArchId2FromObj;
		while(arch_obj.isInstance())
		{
			arch_obj = arch_obj.archetype();
			arch_level++;
		}
		makeArch2InstMap(from_obj, fromArchId2FromObj, arch_level);		
		try{
			to_obj = to_parent.CreateObject(objType, arch_obj, from_obj.isSubtype());
			copyUdmInstanceObjAttributes(from_obj, to_obj, fromArchId2FromObj, true);
		}
		catch(udm_exception &e)
		{
			to_obj = to_parent.CreateObject(objType);
			UdmUtil::CopyObjectHierarchy(from_obj.__impl(), to_obj.__impl(), to_obj.__impl()->__getdn(), copyMap);
		}
	}
	else
	{
		to_obj = to_parent.CreateObject(objType);
		UdmUtil::CopyObjectHierarchy(from_obj.__impl(), to_obj.__impl(), to_obj.__impl()->__getdn(), copyMap);
	}
	copyMap[from_obj] = to_obj;	

	//Property/Parameter
	if(from_obj.type()==CyPhyML::Property::meta)
	{
		CyPhyML::Property from_prop = CyPhyML::Property::Cast(from_obj);
		CyPhyML::Property to_prop = CyPhyML::Property::Cast(to_obj);
		to_prop.ref() = from_prop.ref();
	}
	else if(from_obj.type()==CyPhyML::Parameter::meta)
	{
		CyPhyML::Parameter from_para = CyPhyML::Parameter::Cast(from_obj);
		CyPhyML::Parameter to_para = CyPhyML::Parameter::Cast(to_obj);
		to_para.ref() = from_para.ref();
	}
}

void CyPhyCAExporter::makeArch2InstMap(Udm::Object &obj, map<int, Udm::Object> &fromArchId2FromObj, int arch_level)
{
	int al = arch_level;
	Udm::Object arch_obj = obj.archetype();
	al--;
	while(al>0)
	{
		arch_obj = arch_obj.archetype();
		al--;
	}
	fromArchId2FromObj[arch_obj.uniqueId()]=obj;

	set<Udm::Object> &objs = obj.GetChildObjects();
	for(set<Udm::Object>::iterator i=objs.begin();i!=objs.end();++i)
	{
		makeArch2InstMap(Udm::Object(*i), fromArchId2FromObj, arch_level);
	}
}

void CyPhyCAExporter::copyDesignElementInstance(CyPhyML::DesignElement &from_elem, Udm::Object &to_parent)
{
	if(to_parent==Udm::null)
	{
		CyPhyML::DesignContainer container = CyPhyML::DesignContainer::Cast(from_elem.parent());
		map<CyPhyML::DesignContainer, CyPhyML::ComponentAssembly>::iterator pos = container2caMap.find(container);
		if(pos!=container2caMap.end())
			to_parent = (*pos).second;
	}

	ASSERT(to_parent!=Udm::null);
	map<int, Udm::Object> fromArchId2FromObj;
	int arch_level = 0;
	Uml::Class objType = from_elem.type();
	if(from_elem.isInstance() || from_elem.isSubtype())
	{
		Udm::Object arch_com = from_elem.archetype();
		arch_level++;
		while(arch_com.isInstance())
		{
			arch_com = arch_com.archetype();
			arch_level++;
		}
		makeArch2InstMap(from_elem, fromArchId2FromObj, arch_level);
		Udm::Object to_com = to_parent.CreateObject(objType, arch_com, true);
		copyMap[from_elem] = to_com;
		copyUdmInstanceObjAttributes(from_elem, to_com, fromArchId2FromObj, true);
	}
	else
	{
		std::string aa = UdmUtil::ExtractName(from_elem);
		Udm::Object to_com = to_parent.CreateObject(objType);
		UdmUtil::CopyObjectHierarchy(from_elem.__impl(), to_com.__impl(), from_elem.__impl()->__getdn(), copyMap);
	}	
}

void CyPhyCAExporter::copyUdmInstanceObjAttributes(const Udm::Object &from, Udm::Object &to, map<int, Udm::Object> &fromArchId2FromObj, bool makemap)
{
//	copyMap[from] = to;
	to.__impl()->CopyAttributesFrom(from.__impl());
	set<Udm::Object> &from_objs = from.GetChildObjects();
	set<Udm::Object> &to_objs = to.GetChildObjects();
	ASSERT(from_objs.size()==to_objs.size());
	for(set<Udm::Object>::iterator i=to_objs.begin();i!=to_objs.end();++i)
	{
		Udm::Object to_obj = *i;
		Udm::Object to_arch = to_obj.archetype();
		Udm::Object from_obj;
		map<int, Udm::Object>::iterator pos = fromArchId2FromObj.find(to_arch.uniqueId());
		if(pos!=fromArchId2FromObj.end())
		{
			from_obj = (*pos).second;
		}
		if(from_obj!=Udm::null)
		{
			if(makemap)
			{
				const Uml::Class &objType = to_obj.type();
				if(!Uml::IsAssocClass(objType))
					copyMap[from_obj]=to_obj;
			}
			copyUdmInstanceObjAttributes(from_obj, to_obj, fromArchId2FromObj);
		}

		//for(set<Udm::Object>::iterator fi=from_objs.begin();fi!=from_objs.end();++fi)
		//{
		//	const Udm::Object &from_obj = *fi;
		//	if(from_obj.type() != to_obj.type()) continue;
		//	set<Udm::Object> &from_objs_1 = from_obj.GetChildObjects();
		//	set<Udm::Object> &to_objs_1 = to_obj.GetChildObjects();
		//	if(from_objs_1.size()!=to_objs_1.size()) continue;
		//	Udm::Object to_arch = to_obj.archetype();
		//	Udm::Object from_arch = from_obj.archetype();
		//	while(from_arch.isInstance())
		//		from_arch = from_arch.archetype();
		//	if(to_arch.uniqueId()!=from_arch.uniqueId()) continue;
	
		//	if(makemap)
		//	{
		//		const Uml::Class &objType = to_obj.type();
		//		if(!Uml::IsAssocClass(objType))
		//			copyMap[from_obj]=to_obj;
		//	}
		//	//copy attribute
		//	copyUdmInstanceObjAttributes(from_obj, to_obj, fromArchId2FromObj);

		//	from_objs.erase(*fi);
		//	break;
		//}
	}
}

Udm::Object CyPhyCAExporter::getMappedInstanceObject(const Udm::Object &arcObj, Udm::Object &instanceParent)
{
	set<Udm::Object> childObjs = instanceParent.GetChildObjects();
	Udm::Object ret;
	for(set<Udm::Object>::iterator i=childObjs.begin();i!=childObjs.end();++i)
	{
		if((*i).archetype()==arcObj)
			return (*i);
	}
	return ret;
}

void CyPhyCAExporter::createFlattenedConnections(CyPhyML::DesignContainer &dcontainer)
{
	map<int, int>::iterator mpos=containersMap.find(dcontainer.uniqueId());
	ASSERT(mpos!=containersMap.end());
	if((*mpos).second==0) return;

	map<CyPhyML::DesignContainer, CyPhyML::ComponentAssembly>::iterator ca_pos = container2caMap.find(dcontainer);
	if(ca_pos==container2caMap.end())
		return;
	CyPhyML::ComponentAssembly to_ca = (*ca_pos).second;


	set<Udm::Object> connSet;
	set<CyPhyML::Composition> compositions = dcontainer.Composition_kind_children();
	connSet.insert(compositions.begin(), compositions.end());
	set<CyPhyML::ValueFlow> vfs = dcontainer.ValueFlow_kind_children();
	connSet.insert(vfs.begin(), vfs.end());
	set<CyPhyML::Geometry2Forwarder> gfs = dcontainer.Geometry2Forwarder_kind_children();
	connSet.insert(gfs.begin(), gfs.end());
	set<CyPhyML::Cylinder2Forwarder> cfs = dcontainer.Cylinder2Forwarder_kind_children();
	connSet.insert(cfs.begin(), cfs.end());
	set<CyPhyML::Sphere2Forwarder> sfs = dcontainer.Sphere2Forwarder_kind_children();
	connSet.insert(sfs.begin(), sfs.end());
	for(set<Udm::Object>::iterator i=connSet.begin();i!=connSet.end();++i)
	{
		const Udm::Object &childObj = *i;
		const Uml::Class &objType = childObj.type();

		if (showGui)
		{
			DOEVENTS();
		}
		std::string assocName = objType.name();
	
		if(connsTraversed.find(childObj)!=connsTraversed.end())
			continue;
		connsTraversed.insert(childObj);

		CyPhyUtil::ComponentPortPair endpair = CyPhyUtil::getConnectionEnds(objType, childObj);
		CyPhyUtil::ComponentPort end1 = endpair.src;
		CyPhyUtil::ComponentPort end2 = endpair.dst;

		Udm::Object::AssociationInfo assocInfo1(objType);
		assocInfo1.strSrcRoleName = endpair.srcRoleName;
		assocInfo1.strDstRoleName = endpair.dstRoleName;
	
		Udm::Object::AssociationInfo assocInfo2(objType);
		assocInfo2.strSrcRoleName = endpair.dstRoleName;
		assocInfo2.strDstRoleName = endpair.srcRoleName;

		list<CyPhyUtil::ComponentPort> to_end1s, to_end2s;

		getFlattenedAssociationEnds_for_ComRef(assocInfo2, end1, to_end1s, end2, dcontainer, true, true);
		getFlattenedAssociationEnds_for_ComRef(assocInfo1, end2, to_end2s, end1, dcontainer, false, true);

		if(to_end1s.empty() || to_end2s.empty())
			continue;

		//reconstructConnections(assocInfo1.clsAssociation, to_ca, to_end1s, to_end2s);
		reconstructConnections(childObj, to_ca, to_end1s, to_end2s);
	}

	set<CyPhyML::DesignContainer> child_containers = dcontainer.DesignContainer_kind_children();
	for(set<CyPhyML::DesignContainer>::iterator ci=child_containers.begin();ci!=child_containers.end();++ci)
	{
		CyPhyML::DesignContainer cc = *ci;
		createFlattenedConnections(cc);
	}

	//AssignJoinData
	set<CyPhyML::AssignJoinData> ajds = dcontainer.AssignJoinData_kind_children();
	for(auto it_ajd = ajds.begin();it_ajd!=ajds.end();++it_ajd)
	{
		CyPhyML::AssignJoinData ajd = *it_ajd;
		CyPhyML::JoinData jd = ajd.srcAssignJoinData_end();
		
		CyPhyML::Connector sport = ajd.dstAssignJoinData_end();
		CyPhyML::AssignJoinData_dstAssignJoinData_RPContainer_Base sport_ref_parent = ajd.dstAssignJoinData_refport_parent();
		list<Connector_Pair> to_sports;
		set<CyPhyML::ConnectorComposition> traversedConns;
		getConnectors(sport, sport_ref_parent, to_sports, traversedConns, true);

		if(to_sports.empty()) continue;

		map<Udm::Object, Udm::Object>::iterator pos = copyMap.find(jd);
		if(pos==copyMap.end())
			copyDesignContainerElement(jd, to_ca);

		CyPhyML::JoinData to_jd;
		pos = copyMap.find(jd);
		if(pos!=copyMap.end())
			to_jd = CyPhyML::JoinData::Cast(copyMap[jd]);
		else
			continue;
		for(auto it_list=to_sports.begin();it_list!=to_sports.end();++it_list)
		{
			CyPhyUtil::ComponentPort to_end;
			to_end.port = (*it_list).first;
			to_end.port_ref_parent = (*it_list).second;
			CyPhyUtil::ComponentPort to_sport = getMappingComponentPort(to_end);
			CyPhyML::AssignJoinData newajd = CyPhyML::AssignJoinData::Create(to_ca);
			newajd.srcAssignJoinData_end() = to_jd;
			if(to_sport.port_ref_parent)
				newajd.dstAssignJoinData_refport_parent() = CyPhyML::AssignJoinData_dstAssignJoinData_RPContainer_Base::Cast(to_sport.port_ref_parent);
			newajd.dstAssignJoinData_end() = CyPhyML::Connector::Cast(to_sport.port);
		}
	}
}

void CyPhyCAExporter::createConnections(CyPhyML::DesignContainer &dcontainer)
{
	//From the root DesignContainer, iterate each connection .
	//For each connection, start with two ends, traverse down till the Component/ComponentRef port or CompoundContainer port
	//Find the corresponding mapping port in the ComponentAssembly, 
	//If not found, create one if the port parent is DesignContainer
	//If both sides mapping port found or created, re-construct the same
	map<int, int>::iterator mpos=containersMap.find(dcontainer.uniqueId());
	ASSERT(mpos!=containersMap.end());
	if((*mpos).second==0) 
		return;

	map<CyPhyML::DesignContainer, CyPhyML::ComponentAssembly>::iterator ca_pos = container2caMap.find(dcontainer);
	if(ca_pos==container2caMap.end())
		return;
	CyPhyML::ComponentAssembly to_ca = (*ca_pos).second;

	//if((std::string)dcontainer.ContainerType()=="Compound" || dcontainer==rootDC)
	{	
		set<Udm::Object> connSet;
		set<CyPhyML::Composition> compositions = dcontainer.Composition_kind_children();
		connSet.insert(compositions.begin(), compositions.end());
		set<CyPhyML::ValueFlow> vfs = dcontainer.ValueFlow_kind_children();
		connSet.insert(vfs.begin(), vfs.end());
		set<CyPhyML::Geometry2Forwarder> gfs = dcontainer.Geometry2Forwarder_kind_children();
		connSet.insert(gfs.begin(), gfs.end());
		set<CyPhyML::Cylinder2Forwarder> cfs = dcontainer.Cylinder2Forwarder_kind_children();
		connSet.insert(cfs.begin(), cfs.end());
		set<CyPhyML::Sphere2Forwarder> sfs = dcontainer.Sphere2Forwarder_kind_children();
		connSet.insert(sfs.begin(), sfs.end());
		for(set<Udm::Object>::iterator i=connSet.begin();i!=connSet.end();++i)
		{
			const Udm::Object &childObj = *i;
			const Uml::Class &objType = childObj.type();   //connection
			
			if (showGui)
			{
				DOEVENTS();
			}

			std::string assocName = objType.name();
			if(connsTraversed.find(childObj)!=connsTraversed.end())
				continue;
			connsTraversed.insert(childObj);
			
			CyPhyUtil::ComponentPortPair endpair = CyPhyUtil::getConnectionEnds(objType, childObj);
			CyPhyUtil::ComponentPort end1 = endpair.src;
			CyPhyUtil::ComponentPort end2 = endpair.dst;

			//if alternative container, it could have short-circuit connection
			bool ignore = true;
			if((std::string)dcontainer.ContainerType()=="Compound" || dcontainer==rootDC)
				ignore = false;
			else if((std::string)dcontainer.ContainerType()!="Compound")
			{
				if(end1.port_ref_parent && end2.port_ref_parent && end1.port_ref_parent==end2.port_ref_parent)
					ignore = false;
				else if(!end1.port_ref_parent && !end2.port_ref_parent)
				{
					Udm::Object port1_parent = end1.port.GetParent();
					Udm::Object port2_parent = end2.port.GetParent();
					if(port1_parent==port2_parent && port1_parent.uniqueId()!=dcontainer.uniqueId())
						ignore = false;
					else if(Uml::IsDerivedFrom(port1_parent.type(), CyPhyML::GeometryBase::meta) 
						   ||Uml::IsDerivedFrom(port2_parent.type(), CyPhyML::GeometryBase::meta))
						ignore = false;
				}
			}

			if(!ignore)
			{
				Udm::Object::AssociationInfo assocInfo1(objType);
				assocInfo1.strSrcRoleName = endpair.srcRoleName;
				assocInfo1.strDstRoleName = endpair.dstRoleName;
	
				Udm::Object::AssociationInfo assocInfo2(objType);
				assocInfo2.strSrcRoleName = endpair.dstRoleName;
				assocInfo2.strDstRoleName = endpair.srcRoleName;

				list<CyPhyUtil::ComponentPort> to_end1s, to_end2s;
				//getNearestEndSet_for_ComRef(assocInfo2, end1, to_end1s, end2, true,  assocName=="JoinStructures");
				//getNearestEndSet_for_ComRef(assocInfo1, end2, to_end2s, end1, false,  assocName=="JoinStructures");
				
				getNearestEndSet_for_ComRef(assocInfo2, end1, to_end1s, end2, dcontainer, true,  true);
				getNearestEndSet_for_ComRef(assocInfo1, end2, to_end2s, end1, dcontainer, false,  true);

				if(to_end1s.empty() || to_end2s.empty())
					continue;
		
				//reconstructConnections(assocInfo1.clsAssociation, to_ca, to_end1s, to_end2s);
				reconstructConnections(childObj, to_ca, to_end1s, to_end2s);
			}
		}
	}

	set<CyPhyML::DesignContainer> child_containers = dcontainer.DesignContainer_kind_children();
	for(set<CyPhyML::DesignContainer>::iterator ci=child_containers.begin();ci!=child_containers.end();++ci)
	{
		CyPhyML::DesignContainer cc = *ci;
		createConnections(cc);
	}

	//AssignJoinData
	set<CyPhyML::AssignJoinData> ajds = dcontainer.AssignJoinData_kind_children();
	for(auto it_ajd = ajds.begin();it_ajd!=ajds.end();++it_ajd)
	{
		CyPhyML::AssignJoinData ajd = *it_ajd;
		CyPhyML::JoinData jd = ajd.srcAssignJoinData_end();
		
		CyPhyML::Connector sport = ajd.dstAssignJoinData_end();
		CyPhyML::AssignJoinData_dstAssignJoinData_RPContainer_Base sport_ref_parent = ajd.dstAssignJoinData_refport_parent();
		list<Connector_Pair> to_sports;
		set<CyPhyML::ConnectorComposition> traversedConns;
		getConnectors(sport, sport_ref_parent, to_sports, traversedConns);

		if(to_sports.empty()) continue;

		map<Udm::Object, Udm::Object>::iterator pos = copyMap.find(jd);
		if(pos==copyMap.end())
			copyDesignContainerElement(jd, to_ca);

		CyPhyML::JoinData to_jd;
		pos = copyMap.find(jd);
		if(pos!=copyMap.end())
			to_jd = CyPhyML::JoinData::Cast(copyMap[jd]);
		else
			continue;

		for(auto it_list=to_sports.begin();it_list!=to_sports.end();++it_list)
		{
			CyPhyUtil::ComponentPort to_end;
			to_end.port = (*it_list).first;
			to_end.port_ref_parent = (*it_list).second;
			CyPhyUtil::ComponentPort to_sport = getMappingComponentPort(to_end);
			CyPhyML::AssignJoinData newajd = CyPhyML::AssignJoinData::Create(to_ca);
			newajd.srcAssignJoinData_end() = to_jd;
			if(to_sport.port_ref_parent)
				newajd.dstAssignJoinData_refport_parent() = CyPhyML::AssignJoinData_dstAssignJoinData_RPContainer_Base::Cast(to_sport.port_ref_parent);
			newajd.dstAssignJoinData_end() = CyPhyML::Connector::Cast(to_sport.port);
		}
	}
}

void CyPhyCAExporter::getConnectors(CyPhyML::Connector &sport, Udm::Object &sport_ref_parent,
										list<Connector_Pair> &ends, set<CyPhyML::ConnectorComposition> &traversedConns, bool flattened)
{
	Connector_Pair end;
	end.first = sport;
	end.second = sport_ref_parent;

	Udm::Object src_parent = sport_ref_parent;
	if (!src_parent)
		src_parent = sport.GetParent();
	
	// If the parent is NOT a DesignContainer, do this.
	if (!Uml::IsDerivedFrom(src_parent.type(), CyPhyML::DesignContainer::meta))
	{
		map<Udm::Object, Udm::Object>::iterator pos = copyMap.find(src_parent);
		if(pos!=copyMap.end())
			ends.push_back(end);
		return;
	}
	else // Otherwise, do this.
	{
		CyPhyML::DesignContainer container = CyPhyML::DesignContainer::Cast(src_parent);
		int containerInfo = -1;
		map<int,int>::iterator mpos=containersMap.find(src_parent.uniqueId());
		if(mpos!=containersMap.end())
			containerInfo = (*mpos).second;

		if(containerInfo <= 0) return;

		map<Udm::Object, Udm::Object>::iterator pos = copyMap.find(sport);
		if((std::string)container.ContainerType() == "Compound" && !flattened)
		{
			if(pos!=copyMap.end())
				ends.push_back(end);
			return;
		}
		else
		{
			Udm::Object src_parent_parent = src_parent.GetParent();
			set<CyPhyML::ConnectorComposition> src_conns = sport.srcConnectorComposition();
			for (auto it_conn=src_conns.begin();it_conn!=src_conns.end();++it_conn)
			{
				CyPhyML::ConnectorComposition curr_conn = *it_conn;
				if(traversedConns.find(curr_conn)!=traversedConns.end())
					continue;
				traversedConns.insert(curr_conn);

				Udm::Object conn_parent = curr_conn.GetParent();
				if(conn_parent==src_parent_parent)
					continue;
				CyPhyML::Connector src_end = curr_conn.srcConnectorComposition_end();
				CyPhyML::ConnectorComposition_srcConnectorComposition_RPContainer_Base src_ref_parent = curr_conn.srcConnectorComposition_refport_parent();
				getConnectors(src_end, src_ref_parent, ends, traversedConns,flattened);
			}
			
			set<CyPhyML::ConnectorComposition> dst_conns = sport.dstConnectorComposition();
			for(auto it_conn=dst_conns.begin();it_conn!=dst_conns.end();++it_conn)
			{
				CyPhyML::ConnectorComposition curr_conn = *it_conn;
				if(traversedConns.find(curr_conn)!=traversedConns.end())
					continue;
				traversedConns.insert(curr_conn);

				Udm::Object conn_parent = curr_conn.GetParent();
				if(conn_parent==src_parent_parent)
					continue;

				CyPhyML::Connector dst_end = curr_conn.dstConnectorComposition_end();
				CyPhyML::ConnectorComposition_dstConnectorComposition_RPContainer_Base dst_ref_parent = curr_conn.dstConnectorComposition_refport_parent();
				getConnectors(dst_end, dst_ref_parent, ends, traversedConns, flattened);
			}
		}
	}
}

Udm::Object CyPhyCAExporter::getMappingObject(const Udm::Object &obj)
{
	Udm::Object ret;
	map<Udm::Object, Udm::Object>::iterator pos = copyMap.find(obj);
	if(pos!=copyMap.end())
		ret = (*pos).second;
	return ret;
}

bool CyPhyCAExporter::isSelected(const CyPhyML::DesignElement &com)
{
	set<CyPhyML::DesignEntityRef> refs = com.referedbyDesignEntityRef();
	for(set<CyPhyML::DesignEntityRef>::iterator i=refs.begin();i!=refs.end();++i)
	{
		CyPhyML::DesignEntityRef ref = *i;
		Udm::Object refParent = ref.GetParent();
		if(Uml::IsDerivedFrom(refParent.type(), CyPhyML::CWC::meta))
		{
			if(CyPhyML::CWC::Cast(refParent)==cwc) return true;
		}
	}
	return false;
}

bool CyPhyCAExporter::isSelected(const CyPhyML::ComponentRef &comref)
{
	set<CyPhyML::DesignEntityRef> refs = comref.referedbyDesignEntityRef();
	for(set<CyPhyML::DesignEntityRef>::iterator i=refs.begin();i!=refs.end();++i)
	{
		CyPhyML::DesignEntityRef ref = *i;
		Udm::Object refParent = ref.GetParent();
		if(Uml::IsDerivedFrom(refParent.type(), CyPhyML::CWC::meta))
		{
			if(CyPhyML::CWC::Cast(refParent)==cwc) return true;
		}
	}
	return false;
}

bool CyPhyCAExporter::isSelected(const CyPhyML::DesignContainer &container)
{
	set<CyPhyML::DesignElement> elems = container.DesignElement_kind_children();
	for(set<CyPhyML::DesignElement>::iterator it_elem=elems.begin();it_elem!=elems.end();++it_elem)
	{
		if(isSelected(*it_elem))
			return true;
	}
	set<CyPhyML::ComponentRef> refs = container.ComponentRef_kind_children();
	for(set<CyPhyML::ComponentRef>::iterator it_ref=refs.begin();it_ref!=refs.end();++it_ref)
	{
		if(isSelected(*it_ref))
			return true;
	}
	
	set<CyPhyML::DesignContainer> childContainers = container.DesignContainer_kind_children();
	for(set<CyPhyML::DesignContainer>::iterator it_con=childContainers.begin();it_con!=childContainers.end();++it_con)
	{
		if(isSelected(*it_con)) return true;
	}
	return false;
}

bool CyPhyCAExporter::isOptionalContainer(const Udm::Object &obj, bool &isNullSelected)
{
	if(!Uml::IsDerivedFrom(obj.type(), CyPhyML::DesignContainer::meta)) return false;
	CyPhyML::DesignContainer container = CyPhyML::DesignContainer::Cast(obj);
	if( (std::string)container.ContainerType() !="Optional") return false;

	isNullSelected = !isSelected(container);

	return true;
}

void CyPhyCAExporter::postProcessComponentAssembly(CyPhyML::ComponentAssembly &ca)
{
	//Delete standalone port
	if(ca.isInstance() || ca.isSubtype()) 
		return;

	//set<CyPhyML::Port> ports = ca.Port_kind_children();
	//set<Udm::Object> connSet;
	//set<CyPhyML::Composition> compositions = ca.Composition_kind_children();
	//connSet.insert(compositions.begin(), compositions.end());
	//set<CyPhyML::SignalConnectionType> sigConns = ca.SignalConnectionType_kind_children();
	//connSet.insert(sigConns.begin(), sigConns.end());

	//for(set<Udm::Object>::iterator cit=connSet.begin();cit!=connSet.end();++cit)
	//{
	//	const Udm::Object &conn = *cit;
	//	const Uml::Class &objType = conn.type();   //connection
	//		
	//	CyPhyUtil::ComponentPortPair endpair = CyPhyUtil::getConnectionEnds(objType, conn);
	//	CyPhyUtil::ComponentPort end1 = endpair.src;
	//	CyPhyUtil::ComponentPort end2 = endpair.dst;
	//	if(end1.port_ref_parent==Udm::null && Uml::IsDerivedFrom(end1.port.type(), CyPhyML::Port::meta))
	//	{
	//		set<CyPhyML::Port>::iterator pos1=ports.find(CyPhyML::Port::Cast(end1.port));
	//		if(pos1!=ports.end())
	//			ports.erase(pos1);
	//	}
	//	if(end2.port_ref_parent==Udm::null && Uml::IsDerivedFrom(end2.port.type(), CyPhyML::Port::meta))
	//	{
	//		set<CyPhyML::Port>::iterator pos2=ports.find(CyPhyML::Port::Cast(end2.port));
	//		if(pos2!=ports.end())
	//			ports.erase(pos2);
	//	}
	//}

	//for(set<CyPhyML::Port>::iterator pit=ports.begin();pit!=ports.end();++pit)
	//{
	//	CyPhyML::Port port = *pit;
	//	port.DeleteObject();
	//}

	set<CyPhyML::ComponentAssembly> cas = ca.ComponentAssembly_kind_children();
	set<CyPhyML::ComponentRef> comrefs = ca.ComponentRef_kind_children();
	if(comrefs.empty() && cas.empty())
	{
		ca.DeleteObject();
		return;
	}

	for(set<CyPhyML::ComponentAssembly>::iterator i=cas.begin();i!=cas.end();++i)
	{
		postProcessComponentAssembly(CyPhyML::ComponentAssembly(*i));		
	}	
	
	cas = ca.ComponentAssembly_kind_children();
	comrefs = ca.ComponentRef_kind_children();
	if(comrefs.empty() && cas.empty())
		ca.DeleteObject();
}

CyPhyML::ComponentRef CyPhyCAExporter::createComponentRef(CyPhyML::DesignElement &from_com, CyPhyML::DesignContainer &from_parent)
{
	CyPhyML::ComponentRef comref;
	CyPhyML::ComponentAssembly to_parent = ca_model;
//	CyPhyML::DesignContainer container = CyPhyML::DesignContainer::Cast(from_com.parent());
	map<CyPhyML::DesignContainer, CyPhyML::ComponentAssembly>::iterator pos = container2caMap.find(from_parent);
	if(pos!=container2caMap.end())
		to_parent = (*pos).second;
	
	ASSERT(to_parent!=Udm::null);
	comref = CyPhyML::ComponentRef::Create(to_parent);

	
	if ((std::string)from_parent.ContainerType()=="Alternative")
    {
        // Use Alternative container name and position for all selected
        // component name in the componentAssembly
		comref.name() = from_parent.name();
        comref.position() = from_parent.position();
    }
	else
    {
        // if the parent is Optional or Compound
		comref.name() = from_com.name();
        comref.position() = from_com.position();
    }
	
	comref.ref() = from_com;
	
	return comref;
}

CyPhyML::ComponentRef CyPhyCAExporter::createComponentRef(CyPhyML::ComponentRef &from_ref, CyPhyML::DesignContainer &from_parent)
{
	CyPhyML::DesignElement from_com = from_ref.ref();

	CyPhyML::ComponentRef comref;
	CyPhyML::ComponentAssembly to_parent = ca_model;
	map<CyPhyML::DesignContainer, CyPhyML::ComponentAssembly>::iterator pos = container2caMap.find(from_parent);
	if(pos!=container2caMap.end())
		to_parent = (*pos).second;
	
	ASSERT(to_parent!=Udm::null);
	comref = CyPhyML::ComponentRef::Create(to_parent);

	
	if ((std::string)from_parent.ContainerType() == "Alternative" ||
        (std::string)from_parent.ContainerType() == "Optional")
    {
        // Use Alternative container name and position for all selected
        // component name in the componentAssembly
        comref.name() = std::string(from_parent.name()) + "__" + std::string(from_com.name());
        comref.position() = from_parent.position();
    }
	else
    {
        // if the parent is Optional or Compound
        // keep the name as it is in the design space
		comref.name() = from_ref.name();
        comref.position() = from_ref.position();
    }
	
	comref.ref() = from_com;
	
	return comref;
}

void CyPhyCAExporter::getNearestEndSet_for_ComRef(const Udm::Object::AssociationInfo &assocInfo, CyPhyUtil::ComponentPort &src_end, 
		                             list<CyPhyUtil::ComponentPort> &src_ends, CyPhyUtil::ComponentPort &peer_end, 
									 Udm::Object &currParentObj, bool isSrc, bool bidirect)
//assocInfo: the type of the association
//src_end: the src end of the association
//src_ends: the return found src_end set traverse start from the src_end
//peer_end: the dst end of the association
//currParentObj: the parent object of the first association, this is useful for bidirection check
//isSrc: mark as the traverse direction
//bidirect: whether traverse the association biditectional(currently this value always true)
{
	map<Udm::Object, Udm::Object>::iterator pos;

	Udm::Object src_parent = src_end.port_ref_parent;
	if(src_parent==Udm::null)
		src_parent = src_end.port.GetParent();

	if(!Uml::IsDerivedFrom(src_parent.type(), CyPhyML::DesignContainer::meta))
	{
		pos = copyMap.find(src_parent);
		if(pos!=copyMap.end())
			src_ends.push_back(src_end);
		else {
			if (Uml::IsDerivedFrom(src_parent.type(), CyPhyML::Connector::meta) && Uml::IsDerivedFrom(src_parent.GetParent().type(), CyPhyML::DesignContainer::meta))
			{
				auto dstContainer = container2caMap.find(CyPhyML::DesignContainer::Cast(currParentObj));
				if (dstContainer != container2caMap.end() && Uml::IsDerivedFrom(dstContainer->second.type(), CyPhyML::ComponentAssembly::meta))
				{
					copyDesignContainerElement(src_parent, dstContainer->second);
					pos = copyMap.find(src_parent);
					if(pos != copyMap.end())
						src_ends.push_back(src_end);
					else if (IsDebuggerPresent())
						DebugBreak();
				}
			}
		}
		return;
	}
	else
	{
		CyPhyML::DesignContainer container = CyPhyML::DesignContainer::Cast(src_parent);
		if(container==rootDC)
		{
			src_ends.push_back(src_end);
			return;
		}
		int containerInfo = -1;
		map<int,int>::iterator mpos=containersMap.find(src_parent.uniqueId());	
		if(mpos!=containersMap.end())
		{
			containerInfo = (*mpos).second;
		}

		if(containerInfo <= 0) return;
		
		map<Udm::Object, Udm::Object>::iterator pos = copyMap.find(src_end.port);
		if((std::string)container.ContainerType()=="Compound")
		{
			if(pos==copyMap.end())
			{
				map<CyPhyML::DesignContainer, CyPhyML::ComponentAssembly>::iterator cpos = container2caMap.find(container);
				//ASSERT(cpos!=container2caMap.end());
				if(cpos==container2caMap.end())
				{
					throw udm_exception("There is no ComponentAssembly generated for DesignContainer: "+(std::string)container.name()+".");
				}
				CyPhyML::ComponentAssembly to_parent = (*cpos).second;
				copyDesignContainerElement(src_end.port, to_parent);
			}						
			src_ends.push_back(src_end);
			return;
		}
		else
		{
			int connectedAssocs = 1;
			set<Udm::Object> assocs = (src_end.port).GetAssociationClassObjects(NULL, assocInfo);
			connectedAssocs = assocs.size();

			for(set<Udm::Object>::iterator ci=assocs.begin();ci!=assocs.end();++ci)
			{
				Udm::Object assoc = *ci;	
				
				if(src_parent.GetParent()==assoc.GetParent())  //stop here, only down traverse
					continue;

				CyPhyUtil::ComponentPortPair portPair = CyPhyUtil::getConnectionEnds(assoc.type(), assoc);
				//ComponentPort dst_end = isSamePort(portPair.src, src_end) ? portPair.dst : portPair.src;
				CyPhyUtil::ComponentPort dst_end = isSrc ? portPair.src : portPair.dst;
				if(containerInfo<2)
				{
					getNearestEndSet_for_ComRef(assocInfo, dst_end, src_ends, src_end, currParentObj, isSrc, bidirect);
					continue;
				}
				//for optional container
				Udm::Object dst_parent = dst_end.port_ref_parent;
				if(dst_parent == Udm::null)
					dst_parent = dst_end.port.GetParent();

				if(dst_parent == src_parent && assoc.GetParent() == src_parent)
				{	
					if(containerInfo==3)
						getNearestEndSet_for_ComRef(assocInfo, dst_end, src_ends, src_end, currParentObj, isSrc, bidirect);
					else
						continue;
				}
				else if(assoc.GetParent()==src_parent && dst_parent != src_parent)
				{
					if(containerInfo==2)
						getNearestEndSet_for_ComRef(assocInfo, dst_end, src_ends, src_end, currParentObj, isSrc, bidirect);
					else
						continue;
				}
				else  //could do more optimazition here
				{
					getNearestEndSet_for_ComRef(assocInfo, dst_end, src_ends, src_end, currParentObj, isSrc, bidirect);
				}
			}			
			if(bidirect)
			{
				Udm::Object::AssociationInfo assocInfo1(assocInfo.clsAssociation);
				assocInfo1.strSrcRoleName = assocInfo.strDstRoleName;
				assocInfo1.strDstRoleName = assocInfo.strSrcRoleName;
				set<Udm::Object> assocs1 = src_end.port.GetAssociationClassObjects(NULL, assocInfo1);
				connectedAssocs += assocs1.size();
				for(set<Udm::Object>::iterator i=assocs1.begin();i!=assocs1.end();++i)
				{		
					Udm::Object assoc1 = *i;

					if(assoc1.GetParent()==currParentObj || src_parent.GetParent()==assoc1.GetParent()) continue;
					
					CyPhyUtil::ComponentPortPair portPair = CyPhyUtil::getConnectionEnds(assoc1.type(), assoc1);
					CyPhyUtil::ComponentPort dst_end =  isSrc ? portPair.dst : portPair.src;
					if(isSamePort(dst_end,peer_end)) continue;

					if(containerInfo<2)
					{
						getNearestEndSet_for_ComRef(assocInfo1, dst_end, src_ends, src_end, currParentObj, !isSrc, bidirect);
						continue;
					}
					//for optional container
					Udm::Object dst_parent = dst_end.port_ref_parent;
					if(dst_parent == Udm::null)
						dst_parent = dst_end.port.GetParent();

					if(dst_parent == src_parent && assoc1.GetParent()==src_parent)
					{	
						if(containerInfo==3)
							getNearestEndSet_for_ComRef(assocInfo1, dst_end, src_ends, src_end, currParentObj, !isSrc, bidirect);
						else
							continue;
					}
					else if(assoc1.GetParent()==src_parent && dst_parent!= src_parent)
					{
						if(containerInfo==2)
							getNearestEndSet_for_ComRef(assocInfo1, dst_end, src_ends, src_end, currParentObj, !isSrc, bidirect);
						else
							continue;
					}
					else  //could do more optimazition here
					{
						getNearestEndSet_for_ComRef(assocInfo1, dst_end, src_ends, src_end, currParentObj, !isSrc, bidirect);
					}
				}
			}
			if(connectedAssocs < 2)
			{
				//Geometry2Forwarder,Cylinder2Forwarder,Sphere2Forwarder-->portComposition
				if(Uml::IsDerivedFrom(src_end.port.type(), CyPhyML::GeometryBase::meta))
					//check whether the geometry model
					src_ends.push_back(src_end);
			}
		}
	}
}

void CyPhyCAExporter::getFlattenedAssociationEnds_for_ComRef(const Udm::Object::AssociationInfo &assocInfo, CyPhyUtil::ComponentPort &src_end, 
															list<CyPhyUtil::ComponentPort> &ends, CyPhyUtil::ComponentPort &peer_end, 
															Udm::Object &currParentObj, bool isSrc, bool bidirect)
{
	map<Udm::Object, Udm::Object>::iterator pos = copyMap.find(src_end.port);
	if(pos!=copyMap.end())
	{
		ends.push_back(src_end);
		return;
	}

	Udm::Object src_parent = src_end.port_ref_parent;
	if(src_parent==Udm::null)
		src_parent = src_end.port.GetParent();

	if(Uml::IsDerivedFrom(src_parent.type(), CyPhyML::DesignElement::meta) || Uml::IsDerivedFrom(src_parent.type(), CyPhyML::ComponentRef::meta))
	{
		map<Udm::Object, Udm::Object>::iterator pos = copyMap.find(src_parent);
		if(pos!=copyMap.end())
			ends.push_back(src_end);
		return;
	}	
	else if(Uml::IsDerivedFrom(src_parent.type(), CyPhyML::DesignContainer::meta))
	{
		CyPhyML::DesignContainer container = CyPhyML::DesignContainer::Cast(src_parent);
		if((std::string)container.ContainerType()=="Compound")
		{
			ends.push_back(src_end);
			return;
		}
		else
		{
			CyPhyML::DesignContainer container = CyPhyML::DesignContainer::Cast(src_parent);
			int containerInfo = -1;
			map<int,int>::iterator mpos=containersMap.find(src_parent.uniqueId());	
			if(mpos!=containersMap.end())
			{
				containerInfo = (*mpos).second;
			}

			if(containerInfo <= 0) return;
			bool isNullSelected = (containerInfo==3);
	
			if(containerInfo==1)
			{
				set<Udm::Object> assocs = (src_end.port).GetAssociationClassObjects(NULL, assocInfo);
				for(set<Udm::Object>::iterator i=assocs.begin();i!=assocs.end();++i)
				{
					Udm::Object assoc = *i;
					connsTraversed.insert(assoc);
					CyPhyUtil::ComponentPortPair portPair = CyPhyUtil::getConnectionEnds(assoc.type(), assoc);
					CyPhyUtil::ComponentPort dst_end = isSrc ? portPair.src : portPair.dst;
					//Udm::Object dst_end = assoc.GetAssociationEnd(assocInfo.strDstRoleName);
					getFlattenedAssociationEnds_for_ComRef(assocInfo, dst_end, ends,  isSrc ? portPair.dst : portPair.src, currParentObj, isSrc, bidirect);
				}	
				if(bidirect)
				{
					Udm::Object::AssociationInfo assocInfo1(assocInfo.clsAssociation);
					assocInfo1.strSrcRoleName = assocInfo.strDstRoleName;
					assocInfo1.strDstRoleName = assocInfo.strSrcRoleName;
					set<Udm::Object> assocs1 = (src_end.port).GetAssociationClassObjects(NULL, assocInfo1);
					for(set<Udm::Object>::iterator i=assocs1.begin();i!=assocs1.end();++i)
					{
						Udm::Object assoc1 = *i;
						if(assoc1.GetParent()==currParentObj) continue;
						if(connsTraversed.find(assoc1)!=connsTraversed.end()) continue;
						connsTraversed.insert(assoc1);

						CyPhyUtil::ComponentPortPair portPair = CyPhyUtil::getConnectionEnds(assoc1.type(), assoc1);
						CyPhyUtil::ComponentPort dst_end =  isSrc ? portPair.dst : portPair.src;
						if(isSamePort(dst_end,peer_end)) continue;
						//Udm::Object dst_end = assoc1.GetAssociationEnd(assocInfo1.strDstRoleName);

						getFlattenedAssociationEnds_for_ComRef(assocInfo1, dst_end, ends, isSrc ? portPair.src : portPair.dst, currParentObj, !isSrc, bidirect);
					}
				}
			}
			else
			{
				set<Udm::Object> assocs = (src_end.port).GetAssociationClassObjects(NULL, assocInfo);
				for(set<Udm::Object>::iterator i=assocs.begin();i!=assocs.end();++i)
				{
					Udm::Object assoc = *i;
				//	if(connsTraversed.find(assoc)!=connsTraversed.end()) continue;
					connsTraversed.insert(assoc);
				
					CyPhyUtil::ComponentPortPair portPair = CyPhyUtil::getConnectionEnds(assoc.type(), assoc);
					CyPhyUtil::ComponentPort dst_end = isSrc ? portPair.src : portPair.dst;
			
					Udm::Object dst_parent = dst_end.port_ref_parent;
					if(dst_parent==Udm::null)
						dst_parent = (dst_end.port).GetParent();
					//Udm::Object assoc_parent = assoc.GetParent();
					if(dst_parent == src_parent && assoc.GetParent() == src_parent)
					{	
						if(isNullSelected)
							getFlattenedAssociationEnds_for_ComRef(assocInfo, dst_end, ends, isSrc ? portPair.dst : portPair.src, currParentObj, isSrc, bidirect);
						else
							continue;
					}
					else if(assoc.GetParent()==src_parent && dst_parent != src_parent)
					{
						if(!isNullSelected)
							getFlattenedAssociationEnds_for_ComRef(assocInfo, dst_end, ends, isSrc ? portPair.dst : portPair.src, currParentObj, isSrc, bidirect);
						else
							continue;
					}
					else  //could do more optimazition here
					{
						getFlattenedAssociationEnds_for_ComRef(assocInfo, dst_end, ends, isSrc ? portPair.dst : portPair.src, currParentObj, isSrc, bidirect);
					}
				}	
				if(bidirect)
				{
					Udm::Object::AssociationInfo assocInfo1(assocInfo.clsAssociation);
					assocInfo1.strSrcRoleName = assocInfo.strDstRoleName;
					assocInfo1.strDstRoleName = assocInfo.strSrcRoleName;
					set<Udm::Object> assocs1 = (src_end.port).GetAssociationClassObjects(NULL, assocInfo1);
					for(set<Udm::Object>::iterator i=assocs1.begin();i!=assocs1.end();++i)
					{
						Udm::Object assoc1 = *i;

						if(assoc1.GetParent()==currParentObj) continue;

						if(connsTraversed.find(assoc1)!=connsTraversed.end()) continue;
						connsTraversed.insert(assoc1);

						CyPhyUtil::ComponentPortPair portPair = CyPhyUtil::getConnectionEnds(assoc1.type(), assoc1);
						CyPhyUtil::ComponentPort dst_end = isSrc ? portPair.src : portPair.dst;
						if(isSamePort(dst_end,peer_end)) continue;

						Udm::Object dst_parent = dst_end.port_ref_parent;
						if(dst_parent==Udm::null)
							dst_parent = (dst_end.port).GetParent();
					
						if(dst_parent == src_parent && assoc1.GetParent()==src_parent)
						{	
							if(isNullSelected)
								getFlattenedAssociationEnds_for_ComRef(assocInfo1, dst_end, ends, isSrc ? portPair.dst : portPair.src, currParentObj, !isSrc, bidirect);
							else
								continue;
						}
						else if(assoc1.GetParent()==src_parent && dst_parent!= src_parent)
						{
							if(!isNullSelected)
								getFlattenedAssociationEnds_for_ComRef(assocInfo1, dst_end, ends, isSrc ? portPair.dst : portPair.src, currParentObj, !isSrc, bidirect);
							else
								continue;
						}
						else
						{
							getFlattenedAssociationEnds_for_ComRef(assocInfo, dst_end, ends, isSrc ? portPair.dst : portPair.src, currParentObj, !isSrc, bidirect);
						}
					}
				}	
			}
		}
	}
}

bool CyPhyCAExporter::isRefPort(Udm::Object &obj, CyPhyML::ComponentRef &comref)
{
	map<Udm::Object, Udm::Object>::iterator pos = copyMap.find(obj);
	if(pos!=copyMap.end())
	{
		Udm::Object toObj = (*pos).second;
		if(!Uml::IsDerivedFrom(toObj.type(), CyPhyML::DesignElement::meta))
			return false;
		else
			return true; // FIXME: KMS: is this right?
	}
	else
	{
		Udm::Object objParent = obj.GetParent();
		pos = copyMap.find(objParent);
		ASSERT(pos!=copyMap.end());
		Udm::Object toParent = (*pos).second;
		if(Uml::IsDerivedFrom(toParent.type(), CyPhyML::ComponentRef::meta))
		{
			comref = CyPhyML::ComponentRef::Cast(toParent);
			return true;
		}
		return false;
	}
}

void CyPhyCAExporter::copyPropertyAndParameter(CyPhyML::DesignContainer &container, CyPhyML::ComponentAssembly &ca)
{
	set<CyPhyML::Property> properties = container.Property_kind_children();
	for(set<CyPhyML::Property>::iterator it_prop=properties.begin();it_prop!=properties.end();++it_prop)
	{
		CyPhyML::Property from_prop = *it_prop;
		copyDesignContainerElement(from_prop, ca);
	}
	set<CyPhyML::Parameter> parameters = container.Parameter_kind_children();
	for(set<CyPhyML::Parameter>::iterator it_para=parameters.begin();it_para!=parameters.end();++it_para)
	{
		CyPhyML::Parameter from_para = *it_para;
		copyDesignContainerElement(from_para, ca);
	}
	set<CyPhyML::Constant> constants = container.Constant_kind_children();
	for(auto it_con=constants.begin();it_con!=constants.end();++it_con)
	{
		CyPhyML::Constant from_con = *it_con;
		copyDesignContainerElement(from_con, ca);
	}
}

CyPhyUtil::ComponentPort CyPhyCAExporter::getMappingComponentPort(CyPhyUtil::ComponentPort &from_port)
{
	CyPhyUtil::ComponentPort ret;
	
	if(from_port.port_ref_parent!=Udm::null)
	{
		ret.port = from_port.port;
		ret.port_ref_parent = getMappingObject(from_port.port_ref_parent);			
	}
	else
	{
		Udm::Object port_parent = from_port.port.GetParent();
		if(Uml::IsDerivedFrom(port_parent.type(), CyPhyML::DesignContainer::meta))
		{
			ret.port = getMappingObject(from_port.port);
		}
		else if(Uml::IsDerivedFrom(port_parent.type(), CyPhyML::DesignElement::meta))
		{
			ret.port = from_port.port;
			ret.port_ref_parent = getMappingObject(port_parent);
		}
		else
		{
			ret.port = getMappingObject(from_port.port);
		}
	}

	return ret;
}

//void CyPhyCAExporter::reconstructConnections(const Uml::Class &type, CyPhyML::ComponentAssembly &parent_ca, list<CyPhyUtil::ComponentPort> &end1s, list<CyPhyUtil::ComponentPort> &end2s)
void CyPhyCAExporter::reconstructConnections(const Udm::Object &from_assoc, CyPhyML::ComponentAssembly &parent_ca, list<CyPhyUtil::ComponentPort> &end1s, list<CyPhyUtil::ComponentPort> &end2s)
{
	for(list<CyPhyUtil::ComponentPort>::iterator it1=end1s.begin();it1!=end1s.end();++it1)
	{
		CyPhyUtil::ComponentPort to_end1 = getMappingComponentPort(*it1);
#if 0
		std::string name = from_assoc ? UdmUtil::ExtractName(from_assoc) : "null";
		std::string name2 = to_end1.port ? UdmUtil::ExtractName(to_end1.port) : "null";
		std::string name3 = it1->port ? UdmUtil::ExtractName(it1->port) : "null";
#endif
		CyPhyML::ComponentRef comref1;
		if(to_end1.port && to_end1.port_ref_parent && Uml::IsDerivedFrom(to_end1.port_ref_parent.type(), CyPhyML::ComponentRef::meta))
			comref1 = CyPhyML::ComponentRef::Cast(to_end1.port_ref_parent);
		else if(!to_end1.port && (*it1).port)
		{
			copyDesignContainerElement((*it1).port, parent_ca);
			to_end1 = getMappingComponentPort(*it1);
		}

		for(list<CyPhyUtil::ComponentPort>::iterator it2=end2s.begin();it2!=end2s.end();++it2)
		{
			CyPhyUtil::ComponentPort to_end2 = getMappingComponentPort(*it2);
			CyPhyML::ComponentRef comref2;
			if(to_end2.port && to_end2.port_ref_parent && Uml::IsDerivedFrom(to_end2.port_ref_parent.type(), CyPhyML::ComponentRef::meta))
				comref2 = CyPhyML::ComponentRef::Cast(to_end2.port_ref_parent);
			else if(!to_end2.port && (*it2).port)
			{
				copyDesignContainerElement((*it2).port, parent_ca);
				to_end2 = getMappingComponentPort(*it2);
			}	
			if(to_end1.port!=Udm::null && to_end2.port!=Udm::null && to_end1.port!=to_end2.port)
			{
				Udm::Object new_assoc = CyPhyUtil::reconstructConnection(from_assoc.type(), parent_ca, to_end1.port, comref1, to_end2.port, comref2, from_assoc);					
				new_assoc.__impl()->CopyAttributesFrom(from_assoc.__impl());
			}
		}
	}

}

#endif