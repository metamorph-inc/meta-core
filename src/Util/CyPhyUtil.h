#ifndef CYPHYUTIL_H
#define CYPHYUTIL_H

#include "Uml.h"
#include "UdmUtil.h"
#include <objbase.h>
#include "CyPhyML.h"

#define DOEVENTS() \
{\
    MSG msg;\
    long sts;\
    do {\
    if (sts = PeekMessage(&msg, (HWND) NULL, 0, 0, PM_REMOVE)) {\
            TranslateMessage(&msg);\
            DispatchMessage(&msg);\
        }\
    } while (sts);\
};

namespace CyPhyUtil
{
	struct ComponentPort{
		Udm::Object port;
		Udm::Object port_ref_parent;
		
		bool operator<(const ComponentPort& rhs) const
		{
			if(!port_ref_parent && !rhs.port_ref_parent)
				return port.uniqueId() < rhs.port.uniqueId();
			else if(port_ref_parent && rhs.port_ref_parent)
			{
				if(port_ref_parent.uniqueId() < rhs.port_ref_parent.uniqueId())
					return true;
				else if(port_ref_parent.uniqueId() > rhs.port_ref_parent.uniqueId())
					return false;
				else //==
					return port.uniqueId() < rhs.port.uniqueId();
			}
			else if(!port_ref_parent && rhs.port_ref_parent)
				return true;
			else if(port_ref_parent && !rhs.port_ref_parent)
				return false;
			return port.uniqueId() < rhs.port.uniqueId(); // should be unreachable
		}

		bool operator==(const ComponentPort& rhs) const
		{
			return (port==rhs.port && port_ref_parent==rhs.port_ref_parent);
		}
	};

	typedef struct {
		ComponentPort src;
		std::string srcRoleName;
		ComponentPort dst;
		std::string dstRoleName;
	} ComponentPortPair;

	ComponentPortPair getConnectionEnds(const Uml::Class &type, const Udm::Object &conn);
	ComponentPortPair getCompositionEnds(const Uml::Class &type, const Udm::Object &conn);
	bool isSamePort(ComponentPort &port1, ComponentPort &port2);

	Udm::Object reconstructConnection(const Uml::Class &type,  const Udm::Object& parent, 
							   const Udm::Object &end1, const CyPhyML::ComponentRef &end1_comref, 
							   const Udm::Object &end2, const CyPhyML::ComponentRef &end2_comref, const Udm::Object& origAssocObj);
	/*Udm::Object createComposition(const Uml::Class &type,const CyPhyML::Port& src,const CyPhyML::Port& dst,
						   const Udm::Object& parent,const Udm::Object& srcRefParent,const Udm::Object& dstRefParent);*/
	Udm::Object createComposition(const Uml::Class &type,const Udm::Object& src,const Udm::Object& dst,
						   const Udm::Object& parent,const Udm::Object& srcRefParent,const Udm::Object& dstRefParent);
	Udm::Object createSignalConnectionType(const Uml::Class &type,const Udm::Object& src,const Udm::Object& dst,
									const Udm::Object& parent, const Udm::Object& srcRefParent,const Udm::Object& dstRefParent);
};

#endif