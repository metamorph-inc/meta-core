#ifndef CYPHYDSECONVERTER_CPP
#define CYPHYDSECONVERTER_CPP

#include "CyPhyDSEConverter.h"
#include "Uml.h"
#include "UmlExt.h"
#include <queue>
#include "Mga.h"
#include "atlbase.h"
#include "atlcom.h"
#include <ComHelp.h>
#include <UdmGme.h>

extern bool Automation;
extern string RefactoredTypeOrAction;
extern wstring ExceptionMessage;


CyPhyDSEConverter::CyPhyDSEConverter(CyPhyML::RootFolder &_root):rootFolder(_root),genCA(false)
{
	//std::map<Uml::Class, Uml::AssociationRole> refMemo;
}

void CyPhyDSEConverter::initCopyMap()
{
	//for the partial copy of the heirarchical tree, it is possible the reference association if cross the boundary,
	//in order to reconstruct the association, it needs to build the initial copymap for the Componentref
	std::queue<Udm::Object> dsChildren;
	dsChildren.push(rootFolder);
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
			if (comp)
			{
				copyMap[comp] = comp;
				std::set<Udm::Object> compChildren = comp.GetChildObjects();
				std::for_each(compChildren.begin(), compChildren.end(), [&](const Udm::Object& o) 
				{ 
					copyMap[o] = o; 
				}
				);
			}
		}
	}
}

void CyPhyDSEConverter::initCopyMap(const Udm::Object &rootObj)
{
	//Please refer JIRA issue 512 and 1545
	//for the partial copy of the heirarchical tree, it is possible the reference association if cross the boundary,
	//in order to reconstruct the association, it needs to build the initial copymap for the Componentref
	std::queue<Udm::Object> dsChildren;
	dsChildren.push(rootObj);
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
			if (comp)
			{
				//check if comp is inside the rootObj
				bool makeCopy = true;
				Udm::Object comp_parent = comp.GetParent();
				while(!Uml::IsDerivedFrom(comp_parent.type(), CyPhyML::RootFolder::meta))
				{
					if(comp_parent==rootObj)
					{
						makeCopy = false;
						break;
					}
					comp_parent = comp_parent.GetParent();
				}
				if(makeCopy)
				{
					copyMap[comp] = comp;
					std::set<Udm::Object> compChildren = comp.GetChildObjects();
					std::for_each(compChildren.begin(), compChildren.end(), [&](const Udm::Object& o) 
					{ 
						copyMap[o] = o; 
					}
					);
				}
			}
		}
	}
}

CyPhyML::DesignContainer CyPhyDSEConverter::createNewDesignContainer(CyPhyML::DesignSpace &dsFolder, const std::string &name)
{
	new_dc = CyPhyML::DesignContainer::Create(dsFolder);
	new_dc.name() = name;

	updateDesignContainerTypeAndIcon(new_dc, false);

	return new_dc;
}

CyPhyML::DesignContainer CyPhyDSEConverter::createNewDesignContainer(CyPhyML::DesignContainer &dc, const std::string &name, bool isAlt)
{
	new_dc = CyPhyML::DesignContainer::Create(dc);
	new_dc.name() = name;

	updateDesignContainerTypeAndIcon(new_dc, isAlt);

	return new_dc;
}

void CyPhyDSEConverter::updateDesignContainerTypeAndIcon(CyPhyML::DesignContainer &new_dc, bool isAlt)
{
	CComPtr<IMgaFCO> new_dc_com_ptr;
	CComPtr<IUnknown> new_dc_iunknown = UdmGme::Udm2Gme(new_dc);
	new_dc_iunknown.QueryInterface(&new_dc_com_ptr);

	if(isAlt)
	{
		new_dc.ContainerType() = "Alternative";
		new_dc_com_ptr->put_RegistryValue((CBstrIn)"icon", (CBstrIn)"alternative_ds.png");
	}
	else
	{
		new_dc.ContainerType() = "Compound";
		new_dc_com_ptr->put_RegistryValue((CBstrIn)"icon", (CBstrIn)"mandatory_ds.png");
	}
}


CyPhyML::ComponentAssembly CyPhyDSEConverter::createNewCA(Udm::Object &parentObj, const std::string &name)
{
	if(Uml::IsDerivedFrom(parentObj.type(), CyPhyML::ComponentAssembly::meta))
		new_ca = CyPhyML::ComponentAssembly::Create(CyPhyML::ComponentAssembly::Cast(parentObj));
	else if(Uml::IsDerivedFrom(parentObj.type(), CyPhyML::DesignContainer::meta))
		new_ca = CyPhyML::ComponentAssembly::Create(CyPhyML::DesignContainer::Cast(parentObj));
	else if(Uml::IsDerivedFrom(parentObj.type(), CyPhyML::TestBench::meta))
	{
		CyPhyML::TestBench tb = CyPhyML::TestBench::Cast(parentObj);
		new_ca = CyPhyML::ComponentAssembly::Create(tb);
	}
	else
		return new_ca;

	new_ca.name() = name;
	genCA = true;
	return new_ca;
}

void CyPhyDSEConverter::setNewDesignContainer(CyPhyML::DesignContainer &dc)
{
	new_dc = dc;
	genCA = false;
}

void CyPhyDSEConverter::setNewCA(CyPhyML::ComponentAssembly &ca)
{
	new_ca = ca;
	genCA = true;
}

void CyPhyDSEConverter::createNewObject(const Udm::Object &from_obj, bool delObj)
{
	if(!Uml::IsDerivedFrom(from_obj.type(), CyPhyML::DesignEntity::meta))
		return;
	
	if(delObj)
		tobeDelObjs.insert(from_obj);
	if(!genCA)
		createNewObject(from_obj, new_dc);
	else
		createNewObject(from_obj, new_ca);
}

void CyPhyDSEConverter::createNewObject(const Udm::Object &from_obj, Udm::Object &to_parent)
{
	std::string status = "Create new object for: "+UdmUtil::ExtractName(from_obj); 
	if(!Automation)
		prgDlg.SetProgress(status.c_str());

	Uml::Class objType = from_obj.type();

	if(objType == CyPhyML::ComponentRef::meta)
	{
		CyPhyML::ComponentRef from_ref = CyPhyML::ComponentRef::Cast(from_obj);
		CyPhyML::ComponentRef new_ref = CyPhyML::ComponentRef::Create(to_parent);
		new_ref.name() = from_ref.name();
		new_ref.ref() = from_ref.ref();
		new_ref.position() = from_ref.position();
		copyMap[from_ref] = new_ref;
	}
	else
	{
		Udm::Object to_obj;
		map<int, Udm::Object> fromArchId2FromObj;
		int arch_level = 0;	
		if(from_obj.isInstance()|| from_obj.isSubtype())
		{
			Udm::Object arch_com = from_obj.archetype();
			arch_level++;
			while(arch_com.isInstance())
			{
				arch_com = arch_com.archetype();
				arch_level++;
			}
			makeArch2InstMap(from_obj, fromArchId2FromObj, arch_level);
			to_obj = to_parent.CreateObject(objType, arch_com, from_obj.isSubtype());
			copyMap[from_obj] = to_obj;
			updateUdmCopySubtype(from_obj, to_obj);
			copyUdmObjAttributes(from_obj, to_obj, fromArchId2FromObj, true);

			if(Uml::IsDerivedFrom(to_obj.type(), CyPhyML::DesignContainer::meta)) {
				CyPhyML::DesignContainer to_obj_dc = CyPhyML::DesignContainer::Cast(to_obj);
				bool isAltCon = (((std::string) to_obj_dc.ContainerType()) == "Alternative");
				updateDesignContainerTypeAndIcon(to_obj_dc, isAltCon);
			}
		}
		else
		{
			to_obj = to_parent.CreateObject(objType);
			copyMap[from_obj] = to_obj;
			//UdmUtil::copy_assoc_map _tmpMap;
			UdmUtil::CopyObjectHierarchy(from_obj.__impl(), to_obj.__impl(), from_obj.__impl()->__getdn(), copyMap);
			to_obj.__impl()->CopyAttributesFrom(from_obj.__impl());

			if(Uml::IsDerivedFrom(to_obj.type(), CyPhyML::DesignContainer::meta)) {
				CyPhyML::DesignContainer to_obj_dc = CyPhyML::DesignContainer::Cast(to_obj);
				bool isAltCon = (((std::string) to_obj_dc.ContainerType()) == "Alternative");
				updateDesignContainerTypeAndIcon(to_obj_dc, isAltCon);
			}
		}
	}
}

void CyPhyDSEConverter::makeArch2InstMap(const Udm::Object &obj, map<int, Udm::Object> &fromArchId2FromObj, int arch_level)
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

void CyPhyDSEConverter::copyUdmObjAttributes(const Udm::Object &from, Udm::Object &to, map<int, Udm::Object> &fromArchId2FromObj, bool makemap)
{
	std::string status = "Copy attributes from: "+UdmUtil::ExtractName(from); 
	if(!Automation)
		prgDlg.SetProgress(status.c_str());

	to.__impl()->CopyAttributesFrom(from.__impl());
	set<Udm::Object> &from_objs = from.GetChildObjects();
	set<Udm::Object> &to_objs = to.GetChildObjects();
//	ASSERT(from_objs.size()==to_objs.size());
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
			copyUdmObjAttributes(from_obj, to_obj, fromArchId2FromObj);
		}
	}
}

void CyPhyDSEConverter::gatherConnections_tobe_reconstruce(const Udm::Object &obj, const Udm::Object &focusObj)
{
	set<Udm::Object> childObjs = focusObj.GetChildObjects();
	for(set<Udm::Object>::iterator i=childObjs.begin();i!=childObjs.end();++i)
	{
		Udm::Object from_obj = *i;
		Uml::Class objType = from_obj.type();
		if(!Uml::IsAssocClass(objType))
			continue;

		std::string status = "Collect to-be-reconstruct association in: "+UdmUtil::ExtractName(focusObj); 
		if(!Automation)
			prgDlg.SetProgress(status.c_str());

		CyPhyUtil::ComponentPortPair endpair = CyPhyUtil::getConnectionEnds(objType, from_obj);
		CyPhyUtil::ComponentPort fromEnd1 = endpair.src;
		Udm::Object end1_parent = fromEnd1.port.GetParent();
		if(end1_parent==obj)
		{
			tobeReconnecteLinks.insert(from_obj);
			continue;
		}

		CyPhyUtil::ComponentPort fromEnd2 = endpair.dst;
		Udm::Object end2_parent = fromEnd2.port.GetParent();
		if(end2_parent==obj)
		{
			tobeReconnecteLinks.insert(from_obj);
		}
	}
}

void CyPhyDSEConverter::create_CA_DC_copy(const CyPhyML::DesignEntity &cyphy_ca, Udm::Object &to_parent)
{
	//since UdmCopy cannot be applied here, it will copy the non-association objects
	//then copy the association by locating the mapping connetion end objects
	std::string status = "Processing: "+(std::string)cyphy_ca.name(); 
	if(!Automation)
		prgDlg.SetProgress(status.c_str());
	set<Udm::Object> assocs;
	set<Udm::Object> childObjs = cyphy_ca.GetChildObjects();
	for(set<Udm::Object>::iterator i=childObjs.begin();i!=childObjs.end();++i)
	{
		Udm::Object from_obj = *i;
		Uml::Class objType = from_obj.type();
		if(Uml::IsAssocClass(objType))
			assocs.insert(from_obj);
		else
		{
			//if(from_obj.isInstance() || from_obj.isSubtype())
				createNewObject(from_obj, to_parent);
		/*	else
			{
				if(Uml::IsDerivedFrom(objType, CyPhyML::ComponentAssembly::meta))
				{
					Udm::Object to_obj = to_parent.CreateObject(objType);
					copyMap[from_obj] = to_obj;
					to_obj.__impl()->CopyAttributesFrom(from_obj.__impl());
					create_CA_DC_copy(CyPhyML::DesignElement::Cast(from_obj), to_obj);
				}
				else
					createNewObject(from_obj, to_parent);
			}*/
		}
	}
	for(set<Udm::Object>::iterator ai=assocs.begin();ai!=assocs.end();++ai)
	{
		Udm::Object fromAssoc = *ai;
		copyAssociation(fromAssoc, to_parent);
	}
}

void CyPhyDSEConverter::reconstructExtractAssociations(Udm::Object &focusObj)
{
	for(auto it=tobeReconnecteLinks.begin();it!=tobeReconnecteLinks.end();++it)
	{
		Udm::Object from_obj = *it;
		Uml::Class objType = from_obj.type();

		CyPhyUtil::ComponentPortPair endpair = CyPhyUtil::getConnectionEnds(objType, from_obj);
		CyPhyUtil::ComponentPort fromEnd1 = endpair.src;
		CyPhyUtil::ComponentPort fromEnd2 = endpair.dst;
		
		Udm::Object toEnd1, toEnd2;
		CyPhyML::ComponentRef ref_parent1, ref_parent2;

		Udm::Object end1_parent = fromEnd1.port.GetParent();
		Udm::Object end2_parent = fromEnd2.port.GetParent();
		if(end1_parent==focusObj)
		{
			toEnd1 = fromEnd1.port;
			toEnd2 = getMappingObject(fromEnd2.port);
		}
		else if(end2_parent==focusObj)
		{
			toEnd2 = fromEnd2.port;
			toEnd1 = getMappingObject(fromEnd1.port);
		}
		else
		{
			toEnd1 = getMappingObject(fromEnd1.port);
			if(!toEnd1)
			{
				toEnd1 = fromEnd1.port;
				if(fromEnd1.port_ref_parent)
					ref_parent1 = CyPhyML::ComponentRef::Cast(fromEnd1.port_ref_parent);
			}
			toEnd2 = getMappingObject(fromEnd2.port);
			if(!toEnd2)
			{
				toEnd2 = fromEnd2.port;
				if(fromEnd2.port_ref_parent)
					ref_parent2 = CyPhyML::ComponentRef::Cast(fromEnd2.port_ref_parent);
			}
		}

		CyPhyUtil::reconstructConnection(objType, focusObj, toEnd1, ref_parent1, toEnd2, ref_parent2, from_obj);
	}
}

void CyPhyDSEConverter::createDSFromCA(const CyPhyML::DesignElement &cyphy_ca)
{
	if(cyphy_ca.isInstance() || cyphy_ca.isSubtype() )
	{
		CyPhyML::ComponentAssembly newca = CyPhyML::ComponentAssembly::Create(new_dc);
		newca.name() = (std::string)cyphy_ca.name()+" DesignContainer";
		createNewObject(cyphy_ca, newca);
	}		
	else
		create_CA_DC_copy(cyphy_ca, new_dc);
}

void CyPhyDSEConverter::copyAssociation(Udm::Object &fromAssoc, Udm::Object &toParent)
{
	Uml::Class assocType = fromAssoc.type();
	std::string status = "Create association in: "+UdmUtil::ExtractName(toParent); 
	if(!Automation)
		prgDlg.SetProgress(status.c_str());

	CyPhyUtil::ComponentPortPair endpair = CyPhyUtil::getConnectionEnds(assocType, fromAssoc);
	CyPhyUtil::ComponentPort fromEnd1 = endpair.src;
	CyPhyUtil::ComponentPort fromEnd2 = endpair.dst;
	CyPhyUtil::ComponentPort toEnd1 = getMappingComponentPort(endpair.src);
	CyPhyUtil::ComponentPort toEnd2 = getMappingComponentPort(endpair.dst);
	CyPhyML::ComponentRef comref1, comref2;
	if(toEnd1.port_ref_parent && Uml::IsDerivedFrom(toEnd1.port_ref_parent.type(), CyPhyML::ComponentRef::meta))
		comref1 = CyPhyML::ComponentRef::Cast(toEnd1.port_ref_parent);
	if(toEnd2.port_ref_parent && Uml::IsDerivedFrom(toEnd2.port_ref_parent.type(), CyPhyML::ComponentRef::meta))
		comref2 = CyPhyML::ComponentRef::Cast(toEnd2.port_ref_parent);
	
	if(toEnd1.port==Udm::null || toEnd2.port==Udm::null)
		return;
	else 
		Udm::Object new_assoc = CyPhyUtil::reconstructConnection(assocType, toParent, toEnd1.port, comref1, toEnd2.port, comref2, fromAssoc);
}

void CyPhyDSEConverter::createAssociation(Udm::Object &assocParent, Uml::Class &assocType,
										  std::string &roleName1, Udm::Object &end1,
										  std::string &roleName2, Udm::Object &end2)
{
	Udm::Object newAssoc = assocParent.CreateObject(assocType);
	Udm::Object::AssociationInfo assocInfo(assocType);
	assocInfo.strSrcRoleName = roleName1;
	assocInfo.strDstRoleName = roleName2;
	end1.CreateLink(newAssoc, assocInfo);
	assocInfo.strSrcRoleName = roleName2;
	assocInfo.strDstRoleName = roleName1;
	end2.CreateLink(newAssoc, assocInfo);	
}

void CyPhyDSEConverter::reconstructAssociations(Udm::Object &parentObj)
{
	Udm::Object new_parent;
	if(genCA)
		new_parent = new_ca;
	else
		new_parent = new_dc;

	//set<Udm::Object> childObjs = dc.GetChildObjects();
	set<Udm::Object> childObjs = parentObj.GetChildObjects();
	for(set<Udm::Object>::iterator i=childObjs.begin();i!=childObjs.end();++i)
	{
		Udm::Object from_obj = *i;
		Uml::Class objType = from_obj.type();
		if(!Uml::IsAssocClass(objType))
			continue;

		std::string status = "Create association in: "+UdmUtil::ExtractName(parentObj); 
		if(!Automation)
			prgDlg.SetProgress(status.c_str());

		CyPhyUtil::ComponentPortPair endpair = CyPhyUtil::getConnectionEnds(objType, from_obj);
		CyPhyUtil::ComponentPort fromEnd1 = endpair.src;
		CyPhyUtil::ComponentPort fromEnd2 = endpair.dst;
		CyPhyUtil::ComponentPort toEnd1 = getMappingComponentPort(endpair.src);
		CyPhyUtil::ComponentPort toEnd2 = getMappingComponentPort(endpair.dst);
		CyPhyML::ComponentRef comref1, comref2;
		if(toEnd1.port_ref_parent && Uml::IsDerivedFrom(toEnd1.port_ref_parent.type(), CyPhyML::ComponentRef::meta))
			comref1 = CyPhyML::ComponentRef::Cast(toEnd1.port_ref_parent);
		if(toEnd2.port_ref_parent && Uml::IsDerivedFrom(toEnd2.port_ref_parent.type(), CyPhyML::ComponentRef::meta))
			comref2 = CyPhyML::ComponentRef::Cast(toEnd2.port_ref_parent);
		
		if(toEnd1.port==Udm::null && toEnd2.port==Udm::null)
			continue;
		else if(toEnd1.port!=Udm::null  && toEnd2.port!=Udm::null)
		{
			CyPhyUtil::reconstructConnection(objType, new_parent, toEnd1.port, comref1, toEnd2.port, comref2, from_obj);
		}
		else if(toEnd1.port!=Udm::null  && toEnd2.port==Udm::null)
		{
			if(toEnd2.port_ref_parent==Udm::null && fromEnd2.port_ref_parent && Uml::IsDerivedFrom(fromEnd2.port_ref_parent.type(), CyPhyML::ComponentRef::meta))
				comref2 = CyPhyML::ComponentRef::Cast(fromEnd2.port_ref_parent);

			Udm::Object toEnd1_parent = toEnd1.port_ref_parent;
			if(toEnd1_parent==Udm::null)
				toEnd1_parent  = toEnd1.port.GetParent();
			
			CyPhyML::ComponentRef port_ref;
			Udm::Object toEnd1_port;
			if(toEnd1_parent != new_parent)
			{
				if(componentPortMap.find(fromEnd1)!=componentPortMap.end())
					toEnd1_port = componentPortMap[fromEnd1];	
				else
				{
					toEnd1_port = createNewPort(fromEnd1, new_parent, UdmUtil::ExtractName(fromEnd2.port)+"_"+UdmUtil::ExtractName(toEnd1_parent));
					componentPortMap[fromEnd1] = toEnd1_port;

					Udm::Object new_assoc1;
					if(objType==CyPhyML::AssignJoinData::meta)
					{
						// For this type, there's no analagous connection in the new sub-container
						new_assoc1 = CyPhyUtil::reconstructConnection(CyPhyML::ConnectorComposition::meta, new_parent, toEnd1.port, comref1, toEnd1_port, port_ref, from_obj);
					}
					else
						new_assoc1 = CyPhyUtil::reconstructConnection(objType, new_parent, toEnd1.port, comref1, toEnd1_port, port_ref, from_obj);
				}
			}
			else
				toEnd1_port = toEnd1.port;
			Udm::Object new_assoc2 = CyPhyUtil::reconstructConnection(objType, parentObj, toEnd1_port, port_ref, fromEnd2.port, comref2, from_obj);
		}
		else if(toEnd1.port==Udm::null  && toEnd2.port!=Udm::null)
		{
			if(toEnd1.port_ref_parent==Udm::null && fromEnd1.port_ref_parent && Uml::IsDerivedFrom(fromEnd1.port_ref_parent.type(), CyPhyML::ComponentRef::meta))
				comref1 = CyPhyML::ComponentRef::Cast(fromEnd1.port_ref_parent);

			Udm::Object toEnd2_parent = toEnd2.port_ref_parent;
			if(toEnd2_parent==Udm::null)
				toEnd2_parent  = toEnd2.port.GetParent();
			
			CyPhyML::ComponentRef port_ref;
			Udm::Object toEnd2_port;
			if(toEnd2_parent != new_parent)
			{
				if(componentPortMap.find(fromEnd2)!=componentPortMap.end())
				{
					toEnd2_port = componentPortMap[fromEnd2];
					//CyPhyUtil::reconstructConnection(objType,new_parent, toEnd2_port,port_ref,toEnd2.port,comref2, from_obj);
				}
				else
				{
					toEnd2_port = createNewPort(fromEnd2, new_parent, UdmUtil::ExtractName(fromEnd2.port)+"_"+UdmUtil::ExtractName(toEnd2_parent));
					componentPortMap[fromEnd2] = toEnd2_port;

					Udm::Object new_assoc1;
					if(objType==CyPhyML::AssignJoinData::meta)
					{
						// For this type, there's no analagous connection in the new sub-container
						new_assoc1 = CyPhyUtil::reconstructConnection(CyPhyML::ConnectorComposition::meta, new_parent, toEnd2_port, port_ref, toEnd2.port, comref2, from_obj);
					}
					else
						new_assoc1 = CyPhyUtil::reconstructConnection(objType, new_parent, toEnd2_port, port_ref, toEnd2.port, comref2, from_obj);
				}
			}
			else
				toEnd2_port = toEnd2.port;
			CyPhyML::ComponentRef fromEnd1_ref;
			if(fromEnd1.port_ref_parent && Uml::IsDerivedFrom(fromEnd1.port_ref_parent.type(), CyPhyML::ComponentRef::meta))
				fromEnd1_ref = CyPhyML::ComponentRef::Cast(fromEnd1.port_ref_parent);
			Udm::Object new_assoc2 = CyPhyUtil::reconstructConnection(objType, parentObj, fromEnd1.port, fromEnd1_ref, toEnd2_port, port_ref, from_obj);
		}
	}
}

Udm::Object CyPhyDSEConverter::createNewPort(const CyPhyUtil::ComponentPort &from, Udm::Object &to_parent, std::string port_name)
{
	long portId = from.port.uniqueId();
	char buffer [33];
	_itoa_s(portId, buffer, 10);
	if(port_name.empty())
#if _DEBUG
     // Good for developers
	 port_name = UdmUtil::ExtractName(from.port)+"_"+buffer;
#else
		port_name = UdmUtil::ExtractName(from.port);
#endif
	
	Uml::Class objType = from.port.type();

	Udm::Object to;
	map<int, Udm::Object> fromArchId2FromObj;
	int arch_level = 0;	
	if(from.port.isInstance()|| from.port.isSubtype())
	{
		Udm::Object arch_com = from.port.archetype();
		arch_level++;
		while(arch_com.isInstance())
		{
			arch_com = arch_com.archetype();
			arch_level++;
		}
		makeArch2InstMap(from.port, fromArchId2FromObj, arch_level);
		to = to_parent.CreateObject(objType, arch_com, from.port.isSubtype());
		
		updateUdmCopySubtype(from.port, to);
		copyUdmObjAttributes(from.port, to, fromArchId2FromObj, true);
	}
	else
	{
		to = to_parent.CreateObject(objType);
		UdmUtil::copy_assoc_map _tmpcopyMap;
		UdmUtil::CopyObjectHierarchy(from.port.__impl(), to.__impl(), from.port.__impl()->__getdn(), _tmpcopyMap);
		to.__impl()->CopyAttributesFrom(from.port.__impl());
	}

	to.SetStrValue("name", port_name);
	componentPortMap[from] = to;	
	return to;
}

void CyPhyDSEConverter::deleteConvertedObjs()
{
	for(set<Udm::Object>::iterator di=tobeDelObjs.begin();di!=tobeDelObjs.end();++di)
	{
		Udm::Object delObj = *di;
		if(delObj) {
			try {
				delObj.DeleteObject();
			} catch(...) {
				// Ignore, probably a subtype or instance model
			}
		}
	}
}

void CyPhyDSEConverter::startProgressBar()
{
	prgDlg.Create(IDD_PROGRESS_DIALOG);
	GetStatusDlg(&prgDlg);
	prgDlg.SetRange(100);
}

void CyPhyDSEConverter::endProgressBar()
{
	prgDlg.OnFinished();
}

Udm::Object CyPhyDSEConverter::getMappingObject(const Udm::Object &obj)
{
	Udm::Object ret;
	map<Udm::Object, Udm::Object>::iterator pos = copyMap.find(obj);
	if(pos!=copyMap.end())
	{
		ret = (*pos).second;
	}
	return ret;
}

CyPhyUtil::ComponentPort CyPhyDSEConverter::getMappingComponentPort(CyPhyUtil::ComponentPort &from_port)
{
	CyPhyUtil::ComponentPort ret;
	
	if(from_port.port_ref_parent!=Udm::null)
	{
		ret.port_ref_parent = getMappingObject(from_port.port_ref_parent);
		if(ret.port_ref_parent!=Udm::null)
		{
			if(Uml::IsDerivedFrom(ret.port_ref_parent.type(), CyPhyML::ComponentRef::meta))
				ret.port = from_port.port;
			else   //for example, replace the componentRef with its referred ComponentAssembly
				ret.port = getMappingObject(from_port.port);
		}
	}
	else
	{
		ret.port = getMappingObject(from_port.port);
	}

	return ret;
}

void CyPhyDSEConverter::updateUdmCopySubtype(const Udm::Object &from, Udm::Object &to)
{
	set<Udm::Object> from_objs = from.GetChildObjects();
	set<Udm::Object> to_objs = to.GetChildObjects();
	
	if(from_objs.size()==to_objs.size()) return;

	for(auto i=from_objs.begin();i!=from_objs.end();++i)
	{
		Udm::Object currObj = *i;
		if(currObj.isInstance() || currObj.isSubtype())
			continue;
		createNewObject(currObj, to);
	}
}

void CyPhyDSEConverter::reconstructReference(CyPhyML::DesignContainer &from_dc)
{
	set<CyPhyML::VisualConstraint> vcs = from_dc.VisualConstraint_kind_children();
	for(set<CyPhyML::VisualConstraint>::iterator it_vc = vcs.begin();it_vc!=vcs.end();++it_vc)
	{
		set<CyPhyML::ImpliesEnd> iends = (*it_vc).ImpliesEnd_kind_children();
		set<CyPhyML::ImpliesEnd>::iterator it_im;
		while(!iends.empty())
		{
			it_im = iends.begin();
			CyPhyML::ImpliesEnd iend = *it_im;
			iends.erase(it_im);
			if(Uml::IsDerivedFrom(iend.type(), CyPhyML::And_Or::meta))
			{
				set<CyPhyML::ImpliesEnd> and_or_iends = (CyPhyML::And_Or::Cast(iend)).ImpliesEnd_kind_children();
				iends.insert(and_or_iends.begin(), and_or_iends.end());
			}
			else
			{
				Udm::Object to_ref_obj = getMappingObject(iend);
				if(to_ref_obj==Udm::null) continue;

				if(Uml::IsDerivedFrom(iend.type(), CyPhyML::DesignEntityRef::meta))
				{				
					CyPhyML::DesignEntityRef from_dref = CyPhyML::DesignEntityRef::Cast(iend);
					CyPhyML::DesignEntityRef to_dref = CyPhyML::DesignEntityRef::Cast(to_ref_obj);
					CyPhyML::DesignEntity from_de = from_dref.ref();
					Udm::Object to_de_obj = getMappingObject(from_de);
					if(to_de_obj!=Udm::null)
						to_dref.ref() = CyPhyML::DesignEntity::Cast(to_de_obj);
					else
						to_dref.ref() = from_de;
				}
				else if(Uml::IsDerivedFrom(iend.type(), CyPhyML::NullOptionRef::meta))
				{
					CyPhyML::NullOptionRef from_nref = CyPhyML::NullOptionRef::Cast(iend);
					CyPhyML::NullOptionRef to_nref = CyPhyML::NullOptionRef::Cast(to_ref_obj);
					CyPhyML::DesignContainer from_dc = from_nref.ref();
					Udm::Object to_dc_obj = getMappingObject(from_dc);
					if(to_dc_obj!=Udm::null)
						to_nref.ref() = CyPhyML::DesignContainer::Cast(to_dc_obj);
					else
						to_nref.ref() = from_dc;
				}
				else if(Uml::IsDerivedFrom(iend.type(), CyPhyML::PropertyConstraintRef::meta))
				{
					CyPhyML::PropertyConstraintRef from_pref = CyPhyML::PropertyConstraintRef::Cast(iend);
					CyPhyML::PropertyConstraintRef to_pref = CyPhyML::PropertyConstraintRef::Cast(to_ref_obj);
					CyPhyML::PropertyConstraint from_prop = from_pref.ref();
					Udm::Object to_pc_obj = getMappingObject(from_prop);
					if(to_pc_obj!=Udm::null)
						to_pref.ref() = CyPhyML::PropertyConstraint::Cast(from_prop);
					else
						to_pref.ref() = from_prop;
				}
			}
		}
	}

	set<CyPhyML::DesignContainer> dcs = from_dc.DesignContainer_kind_children();
	for(set<CyPhyML::DesignContainer>::iterator it_dc=dcs.begin();it_dc!=dcs.end();++it_dc)
	{
		CyPhyML::DesignContainer dc = *it_dc;
		reconstructReference(dc);
	}
}

void CyPhyDSEConverter::reconstructReference(CyPhyML::DesignElement &delem)
{
	Udm::Object copyElem = getMappingObject(delem);
	if(!copyElem)
		return;
	if(!Uml::IsDerivedFrom(copyElem.type(), CyPhyML::DesignElement::meta))
		return;
	CyPhyML::DesignElement copy_delem = CyPhyML::DesignElement::Cast(copyElem);
	set<CyPhyML::ComponentRef> comRefs = delem.referedbyComponentRef();
	for(auto it=comRefs.begin();it!=comRefs.end();++it)
	{
		(*it).ref() = copy_delem;
	}
}

void CyPhyDSEConverter::addCopyMapInput(Udm::Object &from, Udm::Object &to)
{
	copyMap[from] = to;
}

#endif