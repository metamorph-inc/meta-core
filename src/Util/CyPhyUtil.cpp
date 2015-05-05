#ifndef CYPHYUTIL_CPP
#define CYPHYUTIL_CPP

#include "UmlExt.h"
#include "CyPhyUtil.h"


namespace CyPhyUtil
{
	using namespace CyPhyML;
	ComponentPortPair getConnectionEnds(const Uml::Class &type, const Udm::Object &conn)
	{
		if(Uml::IsDerivedFrom(type, CyPhyML::Composition::meta))
		{
			return getCompositionEnds(type, conn);
		}
		else if(Uml::IsDerivedFrom(type, CyPhyML::ValueFlow::meta))
		{
			ValueFlow vf = ValueFlow::Cast(conn);
			ComponentPortPair portPair;
			portPair.srcRoleName = "srcValueFlow";
			portPair.src.port = vf.srcValueFlow_end();
			portPair.src.port_ref_parent = vf.srcValueFlow_refport_parent();
			portPair.dstRoleName = "dstValueFlow";
			portPair.dst.port = vf.dstValueFlow_end();
			portPair.dst.port_ref_parent = vf.dstValueFlow_refport_parent();
			return portPair;
		}
		else if(Uml::IsDerivedFrom(type, CyPhyML::AssignJoinData::meta))
		{
			CyPhyML::AssignJoinData ajd = CyPhyML::AssignJoinData::Cast(conn);
			ComponentPortPair portPair;
			portPair.srcRoleName = "srcAssignJoinData";
			portPair.src.port = ajd.srcAssignJoinData_end();
			portPair.dstRoleName = "dstAssignJoinData";
			portPair.dst.port =ajd.dstAssignJoinData_end();
			portPair.dst.port_ref_parent = ajd.dstAssignJoinData_refport_parent();
			return portPair;
		}
		else if(Uml::IsDerivedFrom(type, CyPhyML::Geometry2Forwarder::meta))
		{
			CyPhyML::Geometry2Forwarder geo = CyPhyML::Geometry2Forwarder::Cast(conn);
			ComponentPortPair portPair;
			portPair.srcRoleName = "srcGeometry2Forwarder";
			portPair.src.port = geo.srcGeometry2Forwarder_end();
			portPair.src.port_ref_parent = geo.srcGeometry2Forwarder_refport_parent();
			portPair.dstRoleName = "dstGeometry2Forwarder";
			portPair.dst.port =geo.dstGeometry2Forwarder_end();
			portPair.dst.port_ref_parent = geo.dstGeometry2Forwarder_refport_parent();
			return portPair;
		}
		else if(Uml::IsDerivedFrom(type, CyPhyML::Cylinder2Forwarder::meta))
		{
			CyPhyML::Cylinder2Forwarder c2f = CyPhyML::Cylinder2Forwarder::Cast(conn);
			ComponentPortPair portPair;
			portPair.srcRoleName = "srcExtrusion2Forwarder";
			portPair.src.port = c2f.srcExtrusion2Forwarder_end();
			portPair.src.port_ref_parent = c2f.srcExtrusion2Forwarder_refport_parent();
			portPair.dstRoleName = "dstExtrusion2Forwarder";
			portPair.dst.port = c2f.dstExtrusion2Forwarder_end();
			portPair.dst.port_ref_parent = c2f.dstExtrusion2Forwarder_refport_parent();
			return portPair;
		}
		else if(Uml::IsDerivedFrom(type, CyPhyML::Sphere2Forwarder::meta))
		{
			CyPhyML::Sphere2Forwarder s2f = CyPhyML::Sphere2Forwarder::Cast(conn);
			ComponentPortPair portPair;
			portPair.srcRoleName = "srcSphere2Forwarder";
			portPair.src.port = s2f.srcSphere2Forwarder_end();
			portPair.src.port_ref_parent = s2f.srcSphere2Forwarder_refport_parent();
			portPair.dstRoleName = "dstSphere2Forwarder";
			portPair.dst.port = s2f.dstSphere2Forwarder_end();
			portPair.dst.port_ref_parent = s2f.dstSphere2Forwarder_refport_parent();
			return portPair;
		}
		else if(Uml::IsDerivedFrom(type, CyPhyML::PropertyConstraintBinding::meta))
		{
			CyPhyML::PropertyConstraintBinding pcb = CyPhyML::PropertyConstraintBinding::Cast(conn);
			ComponentPortPair portPair;
			portPair.srcRoleName = "srcPropertyConstraint";
			portPair.src.port = pcb.srcPropertyConstraintBinding_end();
			portPair.dstRoleName = "dstProperty";
			portPair.dst.port = pcb.dstPropertyConstraintBinding_end();
			portPair.dst.port_ref_parent = pcb.dstPropertyConstraintBinding_refport_parent();
			return portPair;
		}
		else
			throw udm_exception(std::string("Unknown connection type ") + static_cast<std::string>(type.name()));
	};

	ComponentPortPair getCompositionEnds(const Uml::Class &type, const Udm::Object &conn)
	{
		ComponentPortPair portPair;
		
		if (type == RefCoordSystem2RefCoordSystem::meta)
		{
			RefCoordSystem2RefCoordSystem assoc = RefCoordSystem2RefCoordSystem::Cast(conn);
			portPair.srcRoleName = "srcRefCoordSystem2RefCoordSystem";
			portPair.src.port = assoc.srcRefCoordSystem2RefCoordSystem_end();
			portPair.src.port_ref_parent = assoc.srcRefCoordSystem2RefCoordSystem_refport_parent();
			portPair.dstRoleName = "dstRefCoordSystem2RefCoordSystem";
			portPair.dst.port = assoc.dstRefCoordSystem2RefCoordSystem_end();
			portPair.dst.port_ref_parent = assoc.dstRefCoordSystem2RefCoordSystem_refport_parent();
		}		
		else if(type == CyPhyML::AnalysisPointMap::meta)
		{
			AnalysisPointMap assoc = AnalysisPointMap::Cast(conn);
			portPair.srcRoleName = "srcAnalysisPointMap";
			portPair.src.port = assoc.srcAnalysisPointMap_end();
			portPair.src.port_ref_parent = assoc.srcAnalysisPointMap_refport_parent();
			portPair.dstRoleName = "dstAnalysisPointMap";
			portPair.dst.port = assoc.dstAnalysisPointMap_end();
			portPair.dst.port_ref_parent = assoc.dstAnalysisPointMap_refport_parent();		
		}
		else if(type == CyPhyML::PortComposition::meta)
		{
			PortComposition assoc = PortComposition::Cast(conn);
			portPair.srcRoleName = "srcPortComposition";
			portPair.src.port = assoc.srcPortComposition_end();
			portPair.src.port_ref_parent = assoc.srcPortComposition_refport_parent();
			portPair.dstRoleName = "dstPortComposition";
			portPair.dst.port = assoc.dstPortComposition_end();
			portPair.dst.port_ref_parent = assoc.dstPortComposition_refport_parent();	
		}
		else if(type == CyPhyML::ConnectorComposition::meta)
		{
			ConnectorComposition assoc = ConnectorComposition::Cast(conn);
			portPair.srcRoleName = "srcConnectorComposition";
			portPair.src.port = assoc.srcConnectorComposition_end();
			portPair.src.port_ref_parent = assoc.srcConnectorComposition_refport_parent();
			portPair.dstRoleName = "dstConnectorComposition";
			portPair.dst.port = assoc.dstConnectorComposition_end();
			portPair.dst.port_ref_parent = assoc.dstConnectorComposition_refport_parent();	
		}
		else if(type == CyPhyML::PropertyConstraintBinding::meta)
		{
			PropertyConstraintBinding assoc = PropertyConstraintBinding::Cast(conn);
			portPair.srcRoleName = "srcPropertyConstraintBinding";
			portPair.src.port = assoc.srcPropertyConstraintBinding_end();
			portPair.dstRoleName = "dstPropertyConstraintBinding";
			portPair.dst.port = assoc.dstPropertyConstraintBinding_end();
			portPair.dst.port_ref_parent = assoc.dstPropertyConstraintBinding_refport_parent();	
		}
		else
			throw udm_exception(std::string("Unknown composition type ") + static_cast<std::string>(type.name()));

		return portPair;
	};

	bool isSamePort(ComponentPort &port1, ComponentPort &port2)
	{
		return (port1.port==port2.port && port1.port_ref_parent==port2.port_ref_parent);
	};


	Udm::Object reconstructConnection(const Uml::Class &type,  const Udm::Object& parent, 
									  const Udm::Object &end1, const CyPhyML::ComponentRef &end1_comref, 
									  const Udm::Object &end2, const CyPhyML::ComponentRef &end2_comref, const Udm::Object& origAssocObj)
	{
		if(Uml::IsDerivedFrom(type, CyPhyML::Composition::meta))
		{
			return createComposition(type,end1,end2,parent,end1_comref, end2_comref);
		}
		else if(Uml::IsDerivedFrom(type, CyPhyML::ValueFlow::meta))
		{
			ValueFlow newVF = ValueFlow::Create(parent);
			if (end1_comref)
				newVF.srcValueFlow_refport_parent() = ValueFlow_srcValueFlow_RPContainer_Base::Cast(end1_comref);
			if (end2_comref)
				newVF.dstValueFlow_refport_parent() = ValueFlow_dstValueFlow_RPContainer_Base::Cast(end2_comref);
			newVF.srcValueFlow_end() = ValueFlowTarget::Cast(end1);
			newVF.dstValueFlow_end() = ValueFlowTarget::Cast(end2);

			newVF.__impl()->CopyAttributesFrom(origAssocObj.__impl());

			return newVF;
		}
		else if(Uml::IsDerivedFrom(type, CyPhyML::AssignJoinData::meta))
		{
			CyPhyML::AssignJoinData newajd = CyPhyML::AssignJoinData::Create(parent);
			if (end2_comref)
				newajd.dstAssignJoinData_refport_parent() = CyPhyML::AssignJoinData_dstAssignJoinData_RPContainer_Base::Cast(end2_comref);
			newajd.srcAssignJoinData_end() = JoinData::Cast(end1);
			newajd.dstAssignJoinData_end() = Connector::Cast(end2);
			return newajd;
		}
		else if(Uml::IsDerivedFrom(type, CyPhyML::Geometry2Forwarder::meta))
		{
			CyPhyML::Geometry2Forwarder newageo = CyPhyML::Geometry2Forwarder::Create(parent);
			if (end1_comref)
				newageo.srcGeometry2Forwarder_refport_parent() = CyPhyML::Geometry2Forwarder_srcGeometry2Forwarder_RPContainer_Base::Cast(end1_comref);
			if (end2_comref)
				newageo.dstGeometry2Forwarder_refport_parent() = CyPhyML::Geometry2Forwarder_dstGeometry2Forwarder_RPContainer_Base::Cast(end2_comref);
			newageo.srcGeometry2Forwarder_end() = GeometryBase::Cast(end1);
			newageo.dstGeometry2Forwarder_end() = GeometryBase::Cast(end2);
			return newageo;
		}
		else if(Uml::IsDerivedFrom(type, CyPhyML::Cylinder2Forwarder::meta))
		{
			CyPhyML::Cylinder2Forwarder newconn = CyPhyML::Cylinder2Forwarder::Create(parent);
			if (end1_comref)
				newconn.srcExtrusion2Forwarder_refport_parent() = CyPhyML::Cylinder2Forwarder_srcExtrusion2Forwarder_RPContainer_Base::Cast(end1_comref);
			if (end2_comref)
				newconn.dstExtrusion2Forwarder_refport_parent() = CyPhyML::Cylinder2Forwarder_dstExtrusion2Forwarder_RPContainer_Base::Cast(end2_comref);
			newconn.srcExtrusion2Forwarder_end() = CyPhyML::CylinderGeometryType::Cast(end1);
			newconn.dstExtrusion2Forwarder_end() = CyPhyML::CylinderGeometryType::Cast(end2);
			return newconn;
		}
		else if(Uml::IsDerivedFrom(type, CyPhyML::Sphere2Forwarder::meta))
		{
			CyPhyML::Sphere2Forwarder newconn = CyPhyML::Sphere2Forwarder::Create(parent);
			if (end1_comref)
				newconn.srcSphere2Forwarder_refport_parent() = CyPhyML::Sphere2Forwarder_srcSphere2Forwarder_RPContainer_Base::Cast(end1_comref);
			if (end2_comref)
				newconn.dstSphere2Forwarder_refport_parent() = CyPhyML::Sphere2Forwarder_dstSphere2Forwarder_RPContainer_Base::Cast(end2_comref);
			newconn.srcSphere2Forwarder_end() = CyPhyML::SphereGeometryType::Cast(end1);
			newconn.dstSphere2Forwarder_end() = CyPhyML::SphereGeometryType::Cast(end2);
			return newconn;
		}
		else if(Uml::IsDerivedFrom(type, CyPhyML::PropertyConstraintBinding::meta))
		{
			CyPhyML::PropertyConstraintBinding newconn = CyPhyML::PropertyConstraintBinding::Create(parent);
			if (end2_comref)
				newconn.dstPropertyConstraintBinding_refport_parent() = CyPhyML::PropertyConstraintBinding_dstPropertyConstraintBinding_RPContainer_Base::Cast(end2_comref);
			newconn.srcPropertyConstraintBinding_end() = CyPhyML::PropertyConstraint::Cast(end1);
			newconn.dstPropertyConstraintBinding_end() = CyPhyML::Property::Cast(end2);
			return newconn;
		}
		else
			throw udm_exception(std::string("Unknown connection type ") + static_cast<std::string>(type.name()));
	};

	//Udm::Object createComposition(const Uml::Class &type,const CyPhyML::Port& src,const CyPhyML::Port& dst,
	//							  const Udm::Object& parent,const Udm::Object& srcRefParent,const Udm::Object& dstRefParent)
	
	Udm::Object createComposition(const Uml::Class &type,const Udm::Object& src,const Udm::Object& dst,
								  const Udm::Object& parent,const Udm::Object& srcRefParent,const Udm::Object& dstRefParent)
	{
		if (type == RefCoordSystem2RefCoordSystem::meta) {
			RefCoordSystem2RefCoordSystem assoc = RefCoordSystem2RefCoordSystem::Create(parent);
			if (srcRefParent)
				assoc.srcRefCoordSystem2RefCoordSystem_refport_parent() = RefCoordSystem2RefCoordSystem_srcRefCoordSystem2RefCoordSystem_RPContainer_Base::Cast(srcRefParent);
			if (dstRefParent)
				assoc.dstRefCoordSystem2RefCoordSystem_refport_parent() = RefCoordSystem2RefCoordSystem_dstRefCoordSystem2RefCoordSystem_RPContainer_Base::Cast(dstRefParent);
			assoc.srcRefCoordSystem2RefCoordSystem_end() = ReferenceCoordinateSystem::Cast(src);
			assoc.dstRefCoordSystem2RefCoordSystem_end() = ReferenceCoordinateSystem::Cast(dst);
			return assoc;
		}
		else if(type == CyPhyML::AnalysisPointMap::meta)
		{
			AnalysisPointMap assoc = AnalysisPointMap::Create(parent);
			if (srcRefParent)
				assoc.srcAnalysisPointMap_refport_parent() = CyPhyML::AnalysisPointMap_srcAnalysisPointMap_RPContainer_Base::Cast(srcRefParent);
			if (dstRefParent)
				assoc.dstAnalysisPointMap_refport_parent() = CyPhyML::AnalysisPointMap_dstAnalysisPointMap_RPContainer_Base::Cast(dstRefParent);
			assoc.srcAnalysisPointMap_end() = AnalysisFeatures::Cast(src);
			assoc.dstAnalysisPointMap_end() = AnalysisFeatures::Cast(dst);
			return assoc;
		}
		else if(type == CyPhyML::PortComposition::meta)
		{
			PortComposition assoc = PortComposition::Create(parent);
			if(srcRefParent)
				assoc.srcPortComposition_refport_parent() = CyPhyML::PortComposition_srcPortComposition_RPContainer_Base::Cast(srcRefParent);
			if(dstRefParent)
				assoc.dstPortComposition_refport_parent() = CyPhyML::PortComposition_dstPortComposition_RPContainer_Base::Cast(dstRefParent);
			assoc.srcPortComposition_end() = CyPhyML::Port::Cast(src);
			assoc.dstPortComposition_end() = CyPhyML::Port::Cast(dst);
			return assoc;
		}
		else if(type == CyPhyML::ConnectorComposition::meta)
		{
			ConnectorComposition assoc = ConnectorComposition::Create(parent);
			if(srcRefParent)
				assoc.srcConnectorComposition_refport_parent() = CyPhyML::ConnectorComposition_srcConnectorComposition_RPContainer_Base::Cast(srcRefParent);
			if(dstRefParent)
				assoc.dstConnectorComposition_refport_parent() = CyPhyML::ConnectorComposition_dstConnectorComposition_RPContainer_Base::Cast(dstRefParent);
			assoc.srcConnectorComposition_end() = CyPhyML::Connector::Cast(src);
			assoc.dstConnectorComposition_end() = CyPhyML::Connector::Cast(dst);
			return assoc;
		}
		else if(type == CyPhyML::PropertyConstraintBinding::meta)
		{
			PropertyConstraintBinding assoc = PropertyConstraintBinding::Create(parent);
			if(dstRefParent)
				assoc.dstPropertyConstraintBinding_refport_parent() = CyPhyML::PropertyConstraintBinding_dstPropertyConstraintBinding_RPContainer_Base::Cast(dstRefParent);
			assoc.srcPropertyConstraintBinding_end() = CyPhyML::PropertyConstraint::Cast(src);
			assoc.dstPropertyConstraintBinding_end() = CyPhyML::Property::Cast(dst);
			return assoc;
		}
		throw udm_exception(std::string("Unknown composition type ") + static_cast<std::string>(type.name()));	
	};
};

#endif