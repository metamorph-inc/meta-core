#include <stdafx.h>



#include <UdmBase.h>
#include <UdmDom.h>
#include <ErrHand.h>
#include "../iface/DesertIfaceBack.h"

#include <afxtempl.h>



#include <afxdlgs.h>
#include <afxcmn.h>



using namespace DesertIfaceBack;


//configurations
typedef map<long, Configuration> ConfigurationMapType_l;
typedef const pair<long const,Configuration> ConfigurationMapItemType_l;

typedef map<const char *, Configuration> ConfigurationMapType_c;
typedef const pair<const char * const,Configuration> ConfigurationMapItemType_c;

//elements
typedef map<long, DesertIfaceBack::Element> ElementMapType_l;
typedef const pair<long const,DesertIfaceBack::Element> ElementMapItemType_l;


//custommembers
typedef map<long, CustomMember> CustomMemberMapType_l;
typedef const pair<long const,CustomMember> CustomMemberMapItemType_l;

typedef map<long, CustomMember> CustomMemberMapType_c;
typedef const pair<long const,CustomMember> CustomMemberMapItemType_c;

//naturalmembers
typedef map<long, NaturalMember> NaturalMemberMapType_l;
typedef const pair<long const,NaturalMember> NaturalMemberMapItemType_l;


//var. properties
typedef map<const char *, VariableProperty> VariablePropertyMapType;
typedef const pair<const char * const,VariableProperty> VariablePropertyMapItemType;


//element's varproperties

typedef map<long, VariablePropertyMapType> ElementsVarPropMapType;
typedef const pair<long const, VariablePropertyMapType> ElementsVarPropMapItemType;



//configuration's assignments




//unfortunatly this needs to be global
ElementsVarPropMapType elem_vprop_map;


void GetSystem(const char *name)
{
	
	
	static bool already_created = false;
	if (already_created) return;

	ASSERT(name);
	//file open dialog
	CString def_file = 	CString(name) + CString("_back") + CString(".xml");
/*	CFileDialog SaveAs(	FALSE,									//construct a file save as dialog
						"xml",									//default file name extension
						(LPCTSTR)def_file,						//default file name
						OFN_OVERWRITEPROMPT |OFN_HIDEREADONLY , //default behaviour
						NULL,									//no filter
						NULL									//no parent windows
						);
	if (SaveAs.DoModal() != IDOK)
	{
		ASSERT(false);
	};*/
	//need to handle cancel here 
		
	DesertBackSystem dbs;
	UdmDom::DomDataNetwork  nw(diagram);

	nw.CreateNew(
		//(LPCTSTR)SaveAs.GetFileName(), 
		(LPCTSTR)def_file,
		"DesertIfaceBack",	
		DesertBackSystem.meta);	

	//creating the root object of the data network 
	dbs = DesertBackSystem::Cast(nw.GetRootObject());
	dbs.SystemName() = name;
	already_created = true;
	nw.CloseWithUpdate();
	
};







/*
bool GetElement(long id, long e_id, const char * name, DesertIfaceBack::Element &e)
{

	static ElementMapType_l element_map;
	ElementMapType_l::iterator i = element_map.find(id);
	if (i == element_map.end())
	{
		ASSERT(name);
		//DesertBackSystem dbs = 
		GetSystem(NULL);
		e = DesertIfaceBack::Element::Create(dbs);
		e.id() = id;
		e.externalID() = e_id;
		e.name() = name;
		//insert in map
		ElementMapItemType_l item(id, e);
		pair<ElementMapType_l::iterator, bool> ins_res = element_map.insert(item);
		ASSERT(ins_res.second);

		//create map for var properties
		VariablePropertyMapType * vpmap = new VariablePropertyMapType;
		ElementsVarPropMapItemType evpm_item (id, *vpmap);
		pair<ElementsVarPropMapType::iterator, bool> ins_res_1= elem_vprop_map.insert(evpm_item);
		ASSERT(ins_res_1.second);
		

		return false;
	}//eo if (i == element_map.end())
	else
	{
		e = (*i).second;
		return true;
	}	
};




bool GetCustomMember(long id, long e_id, const char * name, CustomMember &cm)
{

	static CustomMemberMapType_l CustomMember_map;
	CustomMemberMapType_l::iterator i = CustomMember_map.find(id);
	if (i == CustomMember_map.end())
	{
		ASSERT(name);
		//DesertBackSystem dbs = 
			GetSystem(NULL);
		cm = CustomMember::Create(dbs);
		cm.id() = id;
		cm.externalID() = e_id;
		cm.name() = name;
		//insert in map
		CustomMemberMapItemType_l item(id, cm);
		pair<CustomMemberMapType_l::iterator, bool> ins_res = CustomMember_map.insert(item);
		ASSERT(ins_res.second);
		return false;
	}//eo if (i == CustomMember_map.end())
	else
	{
		cm = (*i).second;
		return true;
	}	
};





bool GetNaturalMember(long value,  NaturalMember &nm)
{

	static NaturalMemberMapType_l NaturalMember_map;
	NaturalMemberMapType_l::iterator i = NaturalMember_map.find(value);
	if (i == NaturalMember_map.end())
	{

		//DesertBackSystem dbs = 
		GetSystem(NULL);
		nm = NaturalMember::Create(dbs);
		nm.value() = value;
		//insert in map
		NaturalMemberMapItemType_l item(value, nm);
		pair<NaturalMemberMapType_l::iterator, bool> ins_res = NaturalMember_map.insert(item);
		ASSERT(ins_res.second);
		return false;
	}//eo if (i == NaturalMember_map.end())
	else
	{
		nm = (*i).second;
		return true;
	}	
};


bool GetConfiguration(long id, const char * name, Configuration &conf)
{

	static ConfigurationMapType_l Configuration_map;
	ConfigurationMapType_l::iterator i = Configuration_map.find(id);
	if (i == Configuration_map.end())
	{
		//DesertBackSystem dbs = 
		GetSystem(NULL);
		conf = Configuration::Create(dbs);
		conf.name() = name;
		//insert in map
		ConfigurationMapItemType_l item(id, conf);
		pair<ConfigurationMapType_l::iterator, bool> ins_res = Configuration_map.insert(item);
		ASSERT(ins_res.second);
		return false;
	}//eo if (i == Configuration_map.end())
	else
	{
		conf = (*i).second;
		return true;
	}	
};



bool GetProperty(long element, const char * name, VariableProperty &vp)
{
	Element e;
	ASSERT(GetElement(element, -1, NULL, e));
	
	ElementsVarPropMapType::iterator i = elem_vprop_map.find(e.id());
	ASSERT(i != elem_vprop_map.end());

	VariablePropertyMapType vp_map = (*i).second;

	VariablePropertyMapType::iterator j = vp_map.find(name);
	if (j == vp_map.end())
	{
		ASSERT(name);
		//not found, create
		vp = VariableProperty::Create(e);
		vp.name() = name;
		VariablePropertyMapItemType vp_item(name, vp);
		pair<VariablePropertyMapType::iterator, bool> ins_res = vp_map.insert(vp_item);
		ASSERT(ins_res.second);
		return false;
	}
	vp = (*j).second;
	return true;
};
	
*/
namespace BackIfaceFunctions
{

	void CreateSystem(const char * name){	GetSystem(name);};
/*	bool CreateElement(long id, long e_id, const char * name){	Element dummy;	return GetElement(id, e_id, name, dummy);};
	bool CreateCustomMember(long id, long e_id, const char *name){	CustomMember dummy;	return GetCustomMember(id, e_id, name, dummy);};
	bool CreateNaturalMember(long value){	NaturalMember dummy;	return GetNaturalMember(value, dummy);};
	bool CreateConfiguration(long id, const char * name){	Configuration dummy;	return GetConfiguration(id, name, dummy);};
	long CreateProperty(long element, const char * name){	VariableProperty dummy;	return GetProperty(element, name, dummy);};


	void CreateAlternativeAssignment(long configuration, long alt, long alt_of, long id, long e_id, const char *name)
	{
		ASSERT(name);
		Configuration c;
		//configuratio should exist!
		ASSERT(GetConfiguration(configuration, NULL, c));
		Element alt_e, alt_of_e;
		//both alternative and alternative of elements shoudl exists!
		ASSERT(GetElement(alt, 0, NULL, alt_e));
		ASSERT(GetElement(alt_of, 0, NULL, alt_of_e));
		//create association
		AlternativeAssignment aa = AlternativeAssignment::Create(c);
		aa.alternative_end() = alt_e;
		aa.alternative_of_end() = alt_of_e;
		aa.id() = id;
		aa.externalID() = e_id;
		aa.name() = name;
	};

	void CreatePropertyMemberAssignment(long configuration, long element, const char * prop, long member, long id, long e_id, const char * name)
	{
		ASSERT(name);
		ASSERT(prop);
		Configuration c;
		VariableProperty vp;
		CustomMember cm;

		//configuratio should exist!
		ASSERT(GetConfiguration(configuration, NULL, c));
		//property should exist!
		ASSERT(GetProperty(element, prop, vp));
		//custommember should exist!
		ASSERT(GetCustomMember(member, 0, NULL, cm));

		PropertyAssignment pa = PropertyAssignment::Create(c);
		pa.value_of_end() = vp;
		pa.value_end() = cm;
		pa.id() = id;
		pa.externalID() = e_id;
		pa.name() = name;
	};

	void CreatePropertyValueAssignment(long configuration, long element, const char * prop, long value, long id, long e_id, const char * name)
	{
		ASSERT(name);
		ASSERT(prop);
		Configuration c;
		VariableProperty vp;
		NaturalMember nm;
		
		//configuration should exist!
		ASSERT(GetConfiguration(configuration, NULL, c));
		//property should exist!
		ASSERT(GetProperty(element, prop, vp));

		//naturalmember
		//is ok if it doesn't exists yet
		GetNaturalMember(value, nm);

		PropertyAssignment pa = PropertyAssignment::Create(c);
		pa.value_of_end() = vp;
		pa.value_end() = nm;
		pa.id() = id;
		pa.externalID() = e_id;
		pa.name() = name;

	};
	void FreeMaps()
	{
		ElementsVarPropMapType::iterator i = elem_vprop_map.begin();

		for (i = elem_vprop_map.begin(); i != elem_vprop_map.end(); i++)
		{
			VariablePropertyMapType vp = (*i).second;
			delete &vp;
		}
	};*/
}//END of namespace

