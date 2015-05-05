#ifndef DES_BACK_MAP_H
#define DES_BACK_MAP_H
#include "DesMap.h"


typedef map<DesertIface::Element , DesertIfaceBack::Element>  UDMForwardMap;
typedef map<DesertIface::Element , DesertIfaceBack::Element>  UDMBackMap;

typedef map<long, DesertIfaceBack::Element> DesertUdmBackElemMap;
typedef pair<long const, DesertIfaceBack::Element> DesertUdmBackElemItem;

typedef map<long, DesertIfaceBack::VariableProperty> DesertUdmBackVpMap;
typedef pair<long const, DesertIfaceBack::VariableProperty> DesertUdmBackVpItem;

typedef map<long, DesertIfaceBack::CustomMember> DesertUdmBackCmMap;
typedef pair<long const, DesertIfaceBack::CustomMember> DesertUdmBackCmItem;

typedef map<long, DesertIfaceBack::NaturalMember> DesertUdmBackNmMap;
typedef pair<long const, DesertIfaceBack::NaturalMember> DesertUdmBackNmItem;


namespace BackIfaceFunctions {

	/*
		Creates an element in the DesertBackIface datanetwork
		from a DesertUdmMap and a desertID of the Element.

	*/
	static DesertUdmBackElemMap back_elements_map;
	static DesertUdmBackCmMap back_cm_map;
	static DesertUdmBackNmMap back_nm_map;
	static DesertUdmBackVpMap back_properties_map;

	void ClearMap();

	bool GetElement(DesertIfaceBack::DesertBackSystem &dbs,
					DesertUdmMap & _map,
					long desertID,
					DesertIfaceBack::Element &ret,
					std::string &element_path);
	

	bool GetCustomMember(
					DesertIfaceBack::DesertBackSystem &dbs,
					DesertUdmMap & _map,
					long desertID,
					DesertIfaceBack::CustomMember &ret);


	bool GetNaturalMember(
					DesertIfaceBack::DesertBackSystem &dbs,
					long value,
					DesertIfaceBack::NaturalMember &ret);

	bool GetElementProperty(
		DesertIfaceBack::DesertBackSystem &dbs,
		DesertUdmMap & _map,
		UdmDesertMap &_map_dir,
		long desertID,
		DesertIfaceBack::VariableProperty &ret);

	void CreateAlternativeAssignment(
		DesertIfaceBack::DesertBackSystem &dbs,
		DesertIfaceBack::Configuration &conf,
		DesertUdmMap &_map,
		long alt_of_desertID,
		long alt_desertID,
		bool create_ass);			//or just check only the requirements
	
	void CreatePropertyAssignment(
		DesertIfaceBack::DesertBackSystem &dbs,
		DesertIfaceBack::Configuration &conf,
		DesertUdmMap &_map,
		UdmDesertMap &_map_1,
		long assignment_desertID,
		bool create_ass
		);

};


#endif DES_BACK_MAP_H