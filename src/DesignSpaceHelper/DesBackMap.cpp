#ifndef DES_BACK_MAP_CPP
#define DES_BACK_MAP_CPP

#include "DesBackMap.h"

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
	void ClearMap()
	{
		back_cm_map.clear();
		back_elements_map.clear();
		back_nm_map.clear();
		back_properties_map.clear();
	}

	bool GetElement(DesertIfaceBack::DesertBackSystem &dbs,
					DesertUdmMap & _map,
					long desertID,
					DesertIfaceBack::Element &ret,
					std::string &element_path)
	{
//		static DesertUdmBackElemMap back_elements_map;
		DesertUdmMap::iterator j = _map.find(desertID);
		//..and it defintely must be there
		ASSERT(j != _map.end());
		//cast must work
		DesertIface::Element e = DesertIface::Element::Cast((*j).second);
		element_path = e.getPath("//",false,false,"name",true);

		DesertUdmBackElemMap::iterator i = back_elements_map.find(desertID);
		if (i == back_elements_map.end())
		{
			//need to insert it
			//for this, we have to fetch the orig. element from the map
			//create the copy of the Element in the back interface
			ret = DesertIfaceBack::Element::Create(dbs);
			//copy the attributes
			ret.name() = e.name();
			ret.externalID() = e.externalID();
			ret.id() = e.id();
			//create an item, and insert in our static map
			DesertUdmBackElemItem item(desertID, ret);
			pair<DesertUdmBackElemMap::iterator, bool> ins_res = back_elements_map.insert(item);
			//insert must succeed!
			ASSERT(ins_res.second);
			//we return false so to indicate that a new object was created
			return false;
		}//eoif (i == back_elements_map.end())
		else
		{
			//found in our static map, so let's just return it
			ret = (*i).second;
			//we return true so to indicate that we found it in cache
			return true;
		}//eoif (i != back_elements_map.end())
	};//eo bool GetElement()

	bool GetCustomMember(
					DesertIfaceBack::DesertBackSystem &dbs,
					DesertUdmMap & _map,
					long desertID,
					DesertIfaceBack::CustomMember &ret)
	{
	//	static DesertUdmBackCmMap back_cm_map;
		DesertUdmBackCmMap::iterator i = back_cm_map.find(desertID);
		if (i == back_cm_map.end())
		{
			//need to insert it
			//for this, we have to fetch the orig. element from the map
			DesertUdmMap::iterator j = _map.find(desertID);
			//..and it defintely must be there
			ASSERT(j != _map.end());
			//cast must work
			DesertIface::CustomMember cm = DesertIface::CustomMember::Cast((*j).second);
			//create the copy of the CustomMember in the back interface
			DesertIfaceBack::CustomMember ret = DesertIfaceBack::CustomMember::Create(dbs);
			//copy the attributes
			ret.name() = cm.name();
			ret.externalID() = cm.externalID();
			ret.id() = cm.id();
			//create an item, and insert in our static map
			DesertUdmBackCmItem item(desertID, ret);
			pair<DesertUdmBackCmMap::iterator, bool> ins_res = back_cm_map.insert(item);
			//insert must succeed!
			ASSERT(ins_res.second);
			//we return false so to indicate that a new object was created
			return false;
		}//eoif (i == back_cm_map.end())
		else
		{
			//found in our static map, so let's just return it
			ret = (*i).second;
			//we return true so to indicate that we found it in cache
			return true;
		}//eoif (i != back_elements_map.end())

	};//eo bool GetCustomMember()


	bool GetNaturalMember(
					DesertIfaceBack::DesertBackSystem &dbs,
					long value,
					DesertIfaceBack::NaturalMember &ret)
	{
	//	static DesertUdmBackNmMap back_nm_map;
		DesertUdmBackNmMap::iterator i = back_nm_map.find(value);
		if (i == back_nm_map.end())
		{
			DesertIfaceBack::NaturalMember ret = DesertIfaceBack::NaturalMember::Create(dbs);
			//copy the attributes
			ret.value() = value;
			//create an item, and insert in our static map
			DesertUdmBackNmItem item(value, ret);
			pair<DesertUdmBackNmMap::iterator, bool> ins_res = back_nm_map.insert(item);
			//insert must succeed!
			ASSERT(ins_res.second);
			//we return false so to indicate that a new object was created
			return false;
		}//eoif (i == back_nm_map.end())
		else
		{
			//found in our static map, so let's just return it
			ret = (*i).second;
			//we return true so to indicate that we found it in cache
			return true;
		}//eoif (i != back_elements_map.end())

	};//eo bool GetNaturalMember()

	




	bool GetElementProperty(
		DesertIfaceBack::DesertBackSystem &dbs,
		DesertUdmMap & _map,
		UdmDesertMap &_map_dir,
		long desertID,
		DesertIfaceBack::VariableProperty &ret)
	{
	//	static DesertUdmBackVpMap back_properties_map;
		DesertUdmBackVpMap::iterator i = back_properties_map.find(desertID);
		if (i == back_properties_map.end())
		{
			//not found...
			//first, fetch the orig. variable property from map
			DesertUdmMap::iterator j = _map.find(desertID);
			//must be there!
			ASSERT(j != _map.end());
			//type cast also should be OK
			DesertIface::VariableProperty vp = DesertIface::VariableProperty::Cast((*j).second);
			//our maps are not quite good where, we need actually 2 finds here ...
			// lookup the owner element of vp, that will be a DesertIface::Element
			DesertIface::Element e = vp.owner();
			// lookup up it's desertid, that will be a long
			UdmDesertMap::iterator k = _map_dir.find(e);
			//it must be there
			ASSERT(k != _map_dir.end());
			long elem_desertID = (*k).second;
			// finally invoke it's get element
			DesertIfaceBack::Element be;
			std::string be_path;
			GetElement(dbs, _map, elem_desertID, be, be_path);
			//now we can can create the VariableProperty in DesertBackIface!
			//(it was a long way to get here ;-), or this line in British English:
			//(it was a long way to get here, wasn't it ? ;-)))
			DesertIfaceBack::VariableProperty ret = DesertIfaceBack::VariableProperty::Create(be);
			//copy data
			ret.name() = vp.name();
			//insert into our static map
			DesertUdmBackVpItem item(desertID, ret);
			pair<DesertUdmBackVpMap::iterator, bool> ins_res = back_properties_map.insert(item);
			ASSERT(ins_res.second);
			return false;

		}//eo if (i == back_elements_map.end())
		else
		{
			ret = DesertIfaceBack::VariableProperty::Cast((*i).second);
			return true;
		}//eo else if (i == back_elements_map.end())

	};//eo bool GetElementProperty


	void CreateAlternativeAssignment(
		DesertIfaceBack::DesertBackSystem &dbs,
		DesertIfaceBack::Configuration &conf,
		DesertUdmMap &_map,
		long alt_of_desertID,
		long alt_desertID,
		bool create_ass)			//or just check only the requirements
	{
		//create or get the corresponding elements in BackInterface
		DesertIfaceBack::Element alt_e, alt_of_e;
		std::string alt_e_path, alt_of_e_path;
		GetElement(dbs, _map, alt_desertID, alt_e, alt_e_path);
		GetElement(dbs, _map, alt_of_desertID, alt_of_e, alt_of_e_path);
		if (create_ass)
		{
			//create assignemnt in the current config.
			DesertIfaceBack::AlternativeAssignment aa = DesertIfaceBack::AlternativeAssignment::Create(conf);
			aa.alternative_end() = alt_e;
			aa.alternative_of_end() = alt_of_e;
			std::string cfgid = conf.cfgId();
			if(cfgid.empty())
				conf.cfgId() = alt_e_path;
			else
				conf.cfgId() = cfgid+";"+alt_e_path;
			//get id's
			//this has no special id's
			//it's fully identified by the external ID's of the two end
			aa.id() = -1;
			aa.externalID() = -1;
			aa.name() = "alternative assignment";
		}

		//that's it!
	};//eo bool CreateAlternativeAssignment()

	void CreatePropertyAssignment(
		DesertIfaceBack::DesertBackSystem &dbs,
		DesertIfaceBack::Configuration &conf,
		DesertUdmMap &_map,
		UdmDesertMap &_map_1,
		long assignment_desertID,
		bool create_ass
		)
	{
		//first, get the corresponding AssignedValues
		DesertUdmMap::iterator i = _map.find(assignment_desertID);
		
		//must be there!
		ASSERT(i != _map.end());
		
		//cast should be allright...
		DesertIface::AssignedValues av = DesertIface::AssignedValues::Cast((*i).second);
		
		//get the property 
		DesertIface::VariableProperty vp = av.properties_end();
		
		//get the Desert ID's for vp:
		UdmDesertMap::iterator j;

		j = _map_1.find(vp);
		ASSERT(j != _map_1.end());
		long vp_desertID = (*j).second;

		//create the back property
		DesertIfaceBack::VariableProperty  bvp;
		GetElementProperty(dbs, _map,_map_1, vp_desertID, bvp); 

		
		//get the value
		DesertIface::Member v = av.values_end();

		DesertIfaceBack::Value back_value;
		//decide upon kind of member

		if (v.type() == DesertIface::CustomMember::meta)
		{
	
			j = _map_1.find(v);
			ASSERT(j != _map_1.end());
			long cm_desertID = (*j).second;
			//get the back custom member
			DesertIfaceBack::CustomMember temp_cm_back;
			GetCustomMember(dbs, _map, cm_desertID, temp_cm_back);
			back_value = temp_cm_back;

		}
		else
		{
			DesertIface::NaturalMember temp_nm = DesertIface::NaturalMember::Cast(v);
			long value = temp_nm.value();
			//get the back natural member
			DesertIfaceBack::NaturalMember temp_nm_back;
			GetNaturalMember(dbs, value, temp_nm_back);
			back_value = temp_nm_back;
		}
		if (create_ass)
		{
			//create property assignment
			DesertIfaceBack::PropertyAssignment pa = DesertIfaceBack::PropertyAssignment::Create(conf);
			pa.value_of_end() = bvp;
			pa.value_end() = back_value;
			pa.id() = av.id();
			pa.externalID() = av.externalID();
			pa.name() = av.name();
		}

		//ufff, finally

	};



};


#endif DES_BACK_MAP_CPP