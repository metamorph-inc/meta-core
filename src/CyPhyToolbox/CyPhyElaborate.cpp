#include "CyPhyElaborate.h"
#include "Uml.h"
#include "UmlExt.h"
#include "UdmUtil.h"
#include "Mga.h"
#include "atlbase.h"
#include "atlcom.h"
#include "UdmGmeUtil.h"

#include <iterator>
#include <algorithm>

using namespace CyPhyML;
using namespace Udm;

static std::string _ExtractName(Udm::Object o){
 return o == null ? "null" : UdmUtil::ExtractName(o);
}


#define REGNODENAME L"ElaboratedModel"

CComPtr<IMgaFCO> Udm2MgaFCO(Udm::Object& o)
{
	o.isLibObject(); // will throw if o is Udm::null
	CComPtr<IUnknown> pUnk;
	pUnk.Attach(UdmGme::Udm2Gme(o));
	CComQIPtr<IMgaFCO> pFCO = pUnk;
	return pFCO.p;
}

void Store(Udm::Object storage_obj, Udm::Object target)
{
	CComPtr<IMgaFCO> ptr_storage_obj = Udm2MgaFCO(storage_obj);

	CComPtr<IMgaFCO> ptr_target = Udm2MgaFCO(target);

	if (ptr_storage_obj && ptr_target)
	{
		CComBSTR id;
		ptr_target->get_ID(&id);
		if (id)
		{
			HRESULT hr = ptr_storage_obj->put_RegistryValue(CComBSTR(REGNODENAME), id);
			ASSERT(SUCCEEDED(hr));
		}
	}
}

CyPhyML::MgaObject CyPhyElaborate::Retrieve(
	CyPhyML::MgaObject storage_obj)
{
  CComQIPtr<IMgaFCO> ptr_storage_obj = Udm2MgaFCO(storage_obj);
	CComBSTR name;
	
	Udm::Object o;

	if (ptr_storage_obj)
	{
		CComBSTR id;
		COMTHROW(ptr_storage_obj->get_Name(&name));
		CComBSTR value;
		HRESULT result = ptr_storage_obj->get_RegistryValue(CComBSTR(REGNODENAME), &id);
		// check bad ptr
		if (result == S_OK)
		{
			if (id)
			{
				//IMgaFCO *ca_original;

				//IMgaProject *project;
				//tb->get_Project(&project);

				//result = project->GetFCOByID(id, &ca_original);

				if (result == S_OK)
				{
					Udm_VS10::Object::uniqueId_type uid;
					uid = UdmGme::GmeId2UdmId(string(_bstr_t(id)).c_str());

					Udm::Object object;
					object = storage_obj.__impl()->__getdn()->ObjectById(uid);

					// return with the object
					return CyPhyML::MgaObject::Cast(object);
				}
			}
		}
  }
	return CyPhyML::MgaObject::Cast(Udm::null);
}
ComponentAssembly CyPhyElaborate::expand(const ComponentAssembly& ca) {
	return expand(ca, "", std::set<Udm::Object>());
}

void CyPhyElaborate::addAllObjectsToTraceability(Object start) {
	addCopy(start, (start).archetype());
	set<Object> children = start.GetChildObjects();
	for (auto childIt = children.begin(); childIt != children.end(); childIt++)
	{
		// traceability: save all child objects of a component
		addAllObjectsToTraceability(*childIt);
	}

}

ComponentAssembly CyPhyElaborate::expand(const ComponentAssembly& ca, string ID_Prefix, std::set<Udm::Object> expandedInThisStack) {
	log("expand(CA)",ca.getPath());
	expandedInThisStack.insert(ca);

	set<ComponentAssembly> caKids = ca.ComponentAssembly_kind_children();
	for (set<ComponentAssembly>::const_iterator i = caKids.begin(); i != caKids.end(); i++) {
		if (i->isSubtype() || i->isInstance())
		{
			DetachFromArchetype(*i);
		}
		expand(*i, "", expandedInThisStack);
	}
	
	set<ComponentRef> crToElaborate = ca.ComponentRef_kind_children();
	// For each of these, replace the reference with an instance. Then replicate all connections.
	for (set<ComponentRef>::const_iterator i = crToElaborate.begin(); i != crToElaborate.end(); i++) {
		ComponentRef cri(*i);
		ComponentAssembly parent = cri.ComponentAssembly_parent();
		
		DesignElement cType = DesignElement::Cast(cri.ref());
		
		if (cType != Udm::null)
		{
			DesignElement instance;
			if (cType.type() == Component::meta)
			{
				instance = Component::Cast(cType).CreateInstance(parent);
				// traceability: save this component
				addCopy(instance, cType);
				addCopy(instance, cri);
				
				// Copy InstanceGUID
				string newInstanceGUID = ID_Prefix;
				newInstanceGUID.append(cri.InstanceGUID());
				(Component::Cast(instance)).InstanceGUID() = newInstanceGUID;

				set<Object> children = instance.GetChildObjects();
				for (auto childIt = children.begin(); childIt != children.end(); childIt++)
				{
					// traceability: save all child objects of a component
					addCopy(*childIt, (*childIt).archetype());
					addAllObjectsToTraceability(*childIt);

					SwitchConnections(childIt->archetype(), *childIt, cri, ca);
				}
				// TODO: cannot test this, since connections of this type is not possible
				SwitchConnections(cri, instance, Udm::null, parent);

				// DY: 9/9/11 - copy registry value from ref to instance
				this->copyCADPropertyRegistry2Inst(cri, CyPhyML::Component::Cast(instance));
			}
			else if (expandedInThisStack.find(cType) == expandedInThisStack.end())
			{
				map<Object, Object> cam;
				deque<Object> queue;
				queue.push_back(cType);
				while (queue.size() > 0)
				{
					Object o = queue.front();
					queue.pop_front();
                    Udm::Object referred = getReferredOrNull(o);
					if (referred)
					{
						set<Object> refCompChildren = referred.GetChildObjects();
						for_each(refCompChildren.begin(), refCompChildren.end(), [&](const Object& o) { cam[o] = o; });
					}
					set<Object> qChildren = o.GetChildObjects();
					std::copy(qChildren.begin(), qChildren.end(), back_inserter(queue));
				}
				// instance = ComponentAssembly::Create(parent);
				instance = DesignElement::Cast(parent.CreateObject(cType.type()));
				UdmUtil::CopyObjectHierarchy(cType.__impl(), instance.__impl(), parent.__impl()->__getdn(), cam);

				set<Object> children = cType.GetChildObjects();
				for (auto childIt = children.begin(); childIt != children.end(); childIt++)
				{
					auto to = cam.find(*childIt);
					if (to == cam.end())
						throw udm_exception();
					//if (_ExtractName(to->second) != _ExtractName(*childIt))
					//	throw udm_exception();
					SwitchConnections(*childIt, to->second, cri, parent);
				}
				addCopies(cam);
				// TODO: cannot test this, since connections of this type is not possible
				//SwitchConnections(cri, instance, Udm::null, parent);				
				expand(ComponentAssembly::Cast(instance), ID_Prefix + static_cast<std::string>(cri.InstanceGUID()), expandedInThisStack);
			} else {
				throw udm_exception(std::string("ComponentAssembly '") + static_cast<std::string>(cType.name())
					+ "' cannot contain a ComponentRef to itself");
			}
			instance.name() = cri.name();
			// Copy layout information as well.
			string pos = cri.position();
			if (pos == "")
				instance.position() = "0,0";
			else
				instance.position() = pos;

			// If "ID" is non-zero, copy it too.
			if (cri.ID() != 0)
				instance.ID() = cri.ID();

			cri.DeleteObject();
			// cri.name() = std::string(cri.name()) + "tobedeleted";
		}
		else
		{
			cr_null_ref_set.insert(cri);
		}
	}

	return ca;
};

void CyPhyElaborate::collapse_recursive(const ComponentAssembly& ca) {
	log("collapse(CA)",ca.getPath());

	toggleGMEOverhead(false,ca);
	
	set<DesignElement> cToCollapse;
	set<Component> cKids = ca.Component_kind_children();
	for (set<Component>::const_iterator i = cKids.begin(); i != cKids.end(); i++)
		cToCollapse.insert(*i);
	
	set<ComponentAssembly> caKids = ca.ComponentAssembly_kind_children();
	for (set<ComponentAssembly>::const_iterator i = caKids.begin(); i != caKids.end(); i++) {
		// If it's an INSTANCE, we need to collapse it here.
		// Otherwise, it's a standalone Component Assembly and we'll traverse it.
		if ((*i).isInstance()) {
			cToCollapse.insert(*i);
		}
		else {
			collapse(*i);
		}
	}

	map<Object,Object> c_to_cr_map;
	// For each of these, replace the instance with a reference. Then replicate all connections.
	for (set<DesignElement>::const_iterator i = cToCollapse.begin(); i != cToCollapse.end(); i++) {
		DesignElement ci(*i);
		ComponentAssembly parent = ComponentAssembly::Cast(ci.parent());
		// Get type
		DesignElement cType = ci.Archetype();

		ComponentRef reference = ComponentRef::Create(parent);
		reference.name() = ci.name();
		reference.ref() = cType;
		// Copy layout information as well
		string pos = ci.position();
		if (pos == "")
			reference.position() = "0,0";
		else
			reference.position() = pos;
		
		// If ID is non-zero, copy it as well
		if (ci.ID() != 0)
			reference.ID() = ci.ID();

		// Copy InstanceGUID
		if (ci.type() == Component::meta)
			reference.InstanceGUID() = Component::Cast(ci).InstanceGUID();

		c_to_cr_map[ci] = reference;

		// DY: 9/9/11 - copy registry values 
		if (Component::meta == ci.type())
			this->copyCADPropertyRegistry2Ref(CyPhyML::Component::Cast(ci), reference);
	}

	// Connection swap
	SwitchConnections(ca, c_to_cr_map);

	c_to_cr_map.clear();
	ComponentAssembly tempCA = ComponentAssembly::Create(ca);
	// Delete original instances. Not sure why we need to use this hacky workaround to delete.
	for (set<DesignElement>::const_iterator i = cToCollapse.begin(); i != cToCollapse.end(); i++)
		(*i).parent() = tempCA;
	tempCA.parent() = NULL;
	
	toggleGMEOverhead(true,ca);
};

TestBenchType CyPhyElaborate::expand(const TestBenchType& tb) {
	log("elaborate(TB)",tb.getPath());

	set<ComponentAssembly> caKids = tb.ComponentAssembly_kind_children();
	for (set<ComponentAssembly>::const_iterator i = caKids.begin(); i != caKids.end(); i++) {
		if (i->isSubtype() || i->isInstance())
		{
			DetachFromArchetype(*i);
		}
		expand(*i, "", std::set<Udm::Object>());
	}
	
    set<TopLevelSystemUnderTest> crToElaborate = tb.TopLevelSystemUnderTest_kind_children();
	// For each of these, replace the reference with an instance. Then replicate all connections.
	for (set<TopLevelSystemUnderTest>::const_iterator i = crToElaborate.begin(); i != crToElaborate.end(); i++) {
		TopLevelSystemUnderTest cri(*i);
		TestBenchType parent = cri.TestBenchType_parent();

		if (!Uml::IsDerivedFrom(static_cast<DesignEntity>(cri.ref()).type(), DesignElement::meta))
			continue;
		DesignElement cType = DesignElement::Cast(cri.ref());
		
		if (cType != Udm::null)
		{
			DesignElement instance;
			if (Uml::IsDerivedFrom(cType.type(), CyPhyML::ComponentType::meta))
			{
				if (cType.type() == CyPhyML::TestComponent::meta)
				{
					instance = DesignElement::Cast(Udm::Object::Create(cType.type(), parent, CyPhyML::TestBenchType::meta_TopLevelSystemUnderTest, cType.__impl(), false));
				}
				else
					instance = Component::Cast(cType).CreateInstance(parent);
				// traceability: save the instance to the map
				addCopy(instance, cType);

				set<Object> children = instance.GetChildObjects();
				for (auto childIt = children.begin(); childIt != children.end(); childIt++)
				{
					// traceability: save all child objects of a component
					addCopy(*childIt, (*childIt).archetype());
					SwitchConnections(childIt->archetype(), *childIt, cri, tb);
				}
				// TODO: cannot test this, since connections of this type is not possible
				//SwitchConnections(cri, instance, Udm::null, parent);

			}
			else
			{
				map<Object, Object> cam;
				deque<Object> queue;
				queue.push_back(cType);
				while (queue.size() > 0)
				{
					Object o = queue.front();
					queue.pop_front();
                    Udm::Object referred = getReferredOrNull(o);
					if (referred)
					{
						set<Object> refCompChildren = referred.GetChildObjects();
						for_each(refCompChildren.begin(), refCompChildren.end(), [&](const Object& o) { cam[o] = o; });
					}
					set<Object> qChildren = o.GetChildObjects();
					std::copy(qChildren.begin(), qChildren.end(), back_inserter(queue));
				}
				// instance = ComponentAssembly::Create(parent);
				// std::string d1 = cType.type().name();
				instance = DesignElement::Cast(parent.CreateObject(cType.type()));
				Store(instance, cType);
				UdmUtil::CopyObjectHierarchy(cType.__impl(), instance.__impl(), parent.__impl()->__getdn(), cam);

				set<Object> children = cType.GetChildObjects();
				for (auto childIt = children.begin(); childIt != children.end(); childIt++)
				{
					auto to = cam.find(*childIt);
					if (to == cam.end())
						throw udm_exception();
					//if (_ExtractName(to->second) != _ExtractName(*childIt))
					//	throw udm_exception();
					SwitchConnections(*childIt, to->second, cri, parent);
				}
				addCopies(cam);
				// TODO: cannot test this, since connections of this type is not possible
				//SwitchConnections(cri, instance, Udm::null, parent);
				expand(ComponentAssembly::Cast(instance));
				addCopy(instance, cType);
			}
			instance.name() = cri.name();
			// Copy layout information as well.
			string pos = cri.position();
			if (pos == "")
				instance.position() = "0,0";
			else
				instance.position() = pos;

			cri.DeleteObject();
			//cri.name() = std::string(cri.name()) + "tobedeleted";
		}
		else
		{
			cr_null_ref_set.insert(cri);			
		}
	}

	if (Uml::IsDerivedFrom(tb.type(), CyPhyML::BallisticTestBench::meta))
	{
		CyPhyElaborate::SwitchReference<CyPhyML::BallisticTarget>(tb);
		CyPhyElaborate::SwitchReference<CyPhyML::CriticalComponent>(tb);
	}
	CyPhyElaborate::SwitchReference<CyPhyML::TestInjectionPoint>(tb);

	return tb;
}

template<typename CHILD_CLASS>
void CyPhyElaborate::SwitchReference(CyPhyML::TestBenchType tb)
{
	set<Udm::Object> bTargets = tb.GetChildObjects(CHILD_CLASS::meta);
	for (auto bTargetIt = bTargets.begin(); bTargetIt != bTargets.end(); bTargetIt++)
	{
		CHILD_CLASS bTarget = CHILD_CLASS::Cast(*bTargetIt);
		//Udm::Object bTargetCopy = btb.__impl()->createChild(Udm::NULLCHILDROLE, *referenceMeta);
		CHILD_CLASS bTargetCopy = CHILD_CLASS::Create(tb);

		TIPRefBase base;
		auto copyIt = originalObjectToCopies.find(bTarget.ref());
		if (copyIt == originalObjectToCopies.end())
		{
			throw udm_exception("Bug: could not find BallisticTarget copy");
		}
		while (copyIt != originalObjectToCopies.end())
		{
			base = TIPRefBase::Cast(copyIt->second);
			copyIt = originalObjectToCopies.find(base);
		}
		bTargetCopy.ref() = TIPRefBase::Cast(base);
		bTargetCopy.__impl()->CopyAttributesFrom(bTarget.__impl());

		// Debugging
		// std::string d1 = bTarget.getPath("/");
		// std::string d2 = bTargetCopy.getPath("/");
		//std::string d3 = TIPRefBase::Cast(base).getPath("/");
		// std::string target_type = static_cast<Udm::Object>(bTarget.ref()).type().name();
		// OutputDebugStringA("\n\n");
		// for (auto it = originalObjectToCopies.begin(); it != originalObjectToCopies.end(); ++it)
		// {
		// 	OutputDebugStringA((UdmUtil::ExtractName(it->first) + "\t" + UdmGme::UdmId2GmeId(it->first) + "\t" + UdmUtil::ExtractName(it->second) + "\n").c_str());
		// }

		SwitchConnections(bTarget, bTargetCopy, Udm::null, bTarget.GetParent());
		Udm::Object eventualTarget = bTarget;
		while (!!getReferredOrNull(eventualTarget))
		{
			eventualTarget = getReferredOrNull(eventualTarget);
		}
		// std::string d4 = eventualTarget.getPath("/");
		// std::string d5 = eventualTarget.type().name();
		// std::string d6 = getReferredOrNull(bTargetCopy).getPath("/");
		// std::string d22 = UdmGme::UdmId2GmeId(bTargetCopy.uniqueId());
		// std::string d22ref = UdmGme::UdmId2GmeId(Udm::Object(bTargetCopy.ref()).uniqueId());
		
		if (!!eventualTarget && Uml::IsDerivedFrom(eventualTarget.type(), CyPhyML::Component::meta))
			// Components are created as instances, so the RelIDs are the same
			SwitchRefportConnections(bTarget, bTargetCopy, map<Udm::Object, Udm::Object>(), *dynamic_cast<UdmGme::GmeDataNetwork*>(bTarget.__impl()->__getdn()));
		else
			// ComponentAssemblys are created with CopyObjectHierarchy, so the originalObjectToCopies map is correct
			SwitchRefportConnections(bTarget, bTargetCopy, originalObjectToCopies, *dynamic_cast<UdmGme::GmeDataNetwork*>(bTarget.__impl()->__getdn()));
		bTarget.DeleteObject();
		//bTarget.name() = std::string(bTarget.name()) + "tobedeleted";
		//std::string d7 = getReferredOrNull(bTargetCopy).getPath("/");
		//std::string d6_2 = getReferredOrNull(bTargetCopy).getPath("/");
		//std::string d22_2 = UdmGme::UdmId2GmeId(bTargetCopy.uniqueId());
		//std::string d22ref_2 = UdmGme::UdmId2GmeId(Udm::Object(bTargetCopy.ref()).uniqueId());
	}
}

void CyPhyElaborate::collapse_recursive(const TestBenchType& tb) {
	log("collapse(TB)",tb.getPath());

	toggleGMEOverhead(false,tb);

	DesignElement tlsut_localcopy;
	set<ComponentAssembly> caSet = tb.ComponentAssembly_kind_children();
	for (set<ComponentAssembly>::const_iterator i = caSet.begin(); i != caSet.end() && !tlsut_localcopy; i++) {
		if ( GetChildRole(*i).find("TopLevelSystemUnderTest") != string::npos )
			tlsut_localcopy = *i;
	}

	set<Component> caSetComp = tb.Component_kind_children();
	for (set<Component>::const_iterator i = caSetComp.begin(); i != caSetComp.end() && !tlsut_localcopy; i++) {
		if ( GetChildRole(*i).find("TopLevelSystemUnderTest") != string::npos )
			tlsut_localcopy = *i;
	}

	// TODO: what if    tlsut_localcopy is Udm::null

	// Revert TestInjectionPoints to point back to original referent
	set<TestInjectionPoint> tip_set = tb.TestInjectionPoint_children();
	for (set<TestInjectionPoint>::const_iterator i = tip_set.begin(); i != tip_set.end(); i++) {
		TestInjectionPoint tip(*i);
		CyPhyML::MgaObject original_referent = Retrieve(tip);
		if (original_referent == Udm::null)
			return;
		RedirectReference(tip,original_referent);
	}

	// Find referent of original TopLevelSystemUnderTest reference (find the design model)
	DesignElement tlsut_original = DesignElement::Cast(tbToOriginalSUT[tb]);
	if (!tlsut_original) {
		// Cannot collapse using map, no record of previous TLSUT object
		// May be stored in current TLSUT object

		// Get the ID of stored in the regnode of
		CyPhyML::MgaObject object = Retrieve(tlsut_localcopy);
		if (object == Udm::null)
		{
			return;
		}
		string name = object.name();
		string path = object.getPath2("/");
		tlsut_original = CyPhyML::DesignElement::Cast(object); // Udm::null or valid element
	}
	
	TopLevelSystemUnderTest tlsut = TopLevelSystemUnderTest::Create(tb);
	tlsut.ref() = tlsut_original;
	string pos = tlsut_localcopy.position();
	tlsut.name() = tlsut_original.name();
	if (pos == "")
		tlsut.position() = "0,0";
	else 
		tlsut.position() = pos;

	// Now we need to wire it up.
	map<Object,Object> c_to_cr_map;
	c_to_cr_map[tlsut_localcopy] = tlsut;

	SwitchConnections(tb, c_to_cr_map);

	tlsut_localcopy.parent() = NULL;

	toggleGMEOverhead(true,tb);
};


// DY: 9/9/11
// ComponentRef's CADPROPERTIES registry values to Component instance's CADProperty objects
void CyPhyElaborate::copyCADPropertyRegistry2Inst(CyPhyML::ComponentRef& src, CyPhyML::Component& dst) 
{
	PropertyRegistryMap valueUnitPairs;
	string srcName = src.name();

	CComPtr<IMgaFCO> isrcRef = Udm2MgaFCO(src);
	CComPtr<IMgaFCO> idstRef = Udm2MgaFCO(dst);

	if (isrcRef && idstRef)
	{
		copyCADRegistryValues(isrcRef, idstRef, valueUnitPairs);
			
		//set<CyPhyML::CADProperty> cadProp_Set = dst.CADProperty_kind_children();
		//for (set<CyPhyML::CADProperty>::iterator i = cadProp_Set.begin(); i != cadProp_Set.end(); i++)
		//{
		//	CyPhyML::CADProperty prop(*i);

		//	PropertyRegistryMap::const_iterator ci = valueUnitPairs.find(prop.ParameterName());
		//	if (ci != valueUnitPairs.end())
		//	{
		//		prop.Value() = ci->second.first;		// Value
		//		prop.UnitOfMeasurement() = ci->second.second;		// Unit
		//	}
		//}
	}
}

// DY: 9/9/11
// Copy value of Component instance's CADProperty objects to ComponentRef's registry
void CyPhyElaborate::copyCADPropertyRegistry2Ref(Component& src, ComponentRef& dst)
{		
	CComQIPtr<IMgaFCO> srcRef = Udm2MgaFCO(src);
	CComQIPtr<IMgaFCO> dstRef = Udm2MgaFCO(dst);
	PropertyRegistryMap valueUnitPairs;

	if (srcRef && dstRef)
	{
		copyCADRegistryValues(srcRef, dstRef, valueUnitPairs);

		//set<CyPhyML::CADProperty> cadProp_Set = src.CADProperty_kind_children();
		//for (set<CyPhyML::CADProperty>::const_iterator ci = cadProp_Set.begin(); ci != cadProp_Set.end(); ci++)
		//{
		//	CyPhyML::CADProperty prop(*ci);
		//	string pName = prop.ParameterName(), pValue = prop.Value(), pUnit = prop.UnitOfMeasurement();

		//	string path = "CADPROPERTIES/" + pName;
		//	string valuePath = path + "/Value";
		//	string unitPath = path + "/Unit";
		//	HRESULT result = dstRef->put_RegistryValue(CComBSTR(valuePath.c_str()), CComBSTR(pValue.c_str()));
		//	result = dstRef->put_RegistryValue(CComBSTR(unitPath.c_str()), CComBSTR(pUnit.c_str()));	
		//	ASSERT(SUCCEEDED(result));
		//}
	}
}

// DY: 9/9/11
// Copies CADProperties registry value between a source and a destination IMgaFCO
void CyPhyElaborate::copyCADRegistryValues(CComPtr<IMgaFCO>& src, CComPtr<IMgaFCO>& dst, map<string, pair<string, string>>& valueUnitPairs)
{
	CComQIPtr<IMgaRegNode> cadNode;
	src->get_RegistryNode(CComBSTR("CADPROPERTIES"), &cadNode);
	if (cadNode)
	{
		CComQIPtr<IMgaRegNodes> subNodes;
		cadNode->get_SubNodes(VARIANT_FALSE, &subNodes);
		long count;
		subNodes->get_Count(&count);
		for (long i = 1; i <= count; ++i)
		{
			_bstr_t name, value, unit;
			string strName, strValue, strUnit;
			CComQIPtr<IMgaRegNode> node, valueNode, unitNode;

			subNodes->get_Item(i, &node);
			node->get_Name(name.GetAddress());
			node->get_SubNodeByName(CComBSTR("Value"), &valueNode);		// Value
			node->get_SubNodeByName(CComBSTR("Unit"), &unitNode);		// Unit

			if (valueNode)
				valueNode->get_Value(value.GetAddress());
				
			if (unitNode)
				unitNode->get_Value(unit.GetAddress());

			strName = name;
			strValue = value.length() ? value : "";
			strUnit = unit.length() ? unit : "";

			string valuePath = "CADPROPERTIES/" + strName + "/Value";
			string unitPath = "CADPROPERTIES/" + strName + "/Unit";
			HRESULT result = dst->put_RegistryValue(CComBSTR(valuePath.c_str()), CComBSTR(strValue.c_str()));
			result = dst->put_RegistryValue(CComBSTR(unitPath.c_str()), CComBSTR(strUnit.c_str()));			

			valueUnitPairs[strName] = make_pair(strValue, strUnit);
		}
	}
}

