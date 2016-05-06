#ifndef DES_MAP_CPP
#define DES_MAP_CPP
// FIXME: this file is largely copy-pasted from DesertTool/DesMap.cpp, but this file may be out-of-date
#include "DesertIface.h"

#include "uml.h"

//status dlg

#include <algorithm>
#include <iterator>


#include <afxcmn.h>
#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions



using namespace DesertIface;

#include "DesMap.h"
#include <stdio.h>

void DoMap(DesertBase &db, UdmDesertMap & _map, DesertUdmMap &inv_des_map,long id)
{
	//the main mapping function
	//of UDM objects to Desert IDs
	//in a global map

	pair<UdmDesertMap::iterator, bool> insert_res;
	
	UdmDesertMapItem item(db, id);
	Uml::Class db_meta = db.meta;
	insert_res = _map.insert(item);
	ASSERT(insert_res.second);

	pair<DesertUdmMap::iterator, bool> insert_res_1;
	DesertUdmMapItem item_1(id, db);
	insert_res_1 = inv_des_map.insert(item_1);
	ASSERT(insert_res_1.second);


};

long GetID(DesertBase &db, UdmDesertMap & _map)
{
	//find a UDM object in a global map
	UdmDesertMap::iterator i = _map.find(db);
	ASSERT(i != _map.end());
	return (*i).second;
};

DesertBase &GetObject(long id, DesertUdmMap & _map)
{
	//find a UDM object in a global map
	DesertUdmMap::iterator i = _map.find(id);
	ASSERT(i != _map.end());
	return (*i).second;
};


bool CreateDesertSpace(Space &sp, Element &e, UdmDesertMap &des_map, DesertUdmMap &inv_des_map, UdmElementSet &elements, bool root)
{
	static long parent;			//id of the parent element
	long old_parent;			//save old parent
	static long space;			//id of the space
	short decomposition;		//0:leaf, 1: and, 2: or
	//element.decomposition():	
	//			has no children: leaf
	//			has children and decomposition is false:	or node
	//			has children and decomposition is true:		and node

	set<Element> ev_set;

	//static and non-static variables
	//for progress indication 
	//(this is weird in case of a recursive function)
	//CStatusDlg * st_dlg = GetStatusDlg(NULL);
	static short percent_done;
	static short percent_to_do;
	short percent_to_do_save;
	if (root) 
	{
		percent_done = 0;
		percent_to_do = 100;
	}

	
	if (root)
	{
		
		//compute decomposition value
		//mapping from boolean decomposition value
		//to desert style short decomposition
		set<Element> e_children = sp.Element_kind_children();
		set<Element>::iterator i;
		
		if (e_children.empty()) decomposition = 0;
		else
		{
			if (sp.decomposition()) decomposition  = 1;
			else decomposition = 2;
		}

		//debug trace
		TRACE ("CreateDesertSpace invoked: parent: %d, Space: %s , decomposition: %d\n", parent, ((string)sp.name()).c_str(), decomposition);

		//
		//create the space & the root element in the space
		//
		space =  CreateSpace(((string)sp.name()).c_str());
		//space =  CreateSpace(((string)sp.name()).c_str(), sp.id(), sp.externalID());
		
/*
		parent = CreateElement(
			((string)sp.name()).c_str(), 
			space, 
			decomposition, 
			-1, 
			sp.id(),
			sp.externalID());
*/
				
		parent = CreateElement(
			((string)sp.name()).c_str(), 
			space, 
			decomposition, 
			-1, 
			sp.externalID());

		//map the UDM, desert object pairs
		
		DoMap(sp, des_map, inv_des_map, parent);

		
		//recursion here
		
		//update progress bar
		if (e_children.empty())
		{
			//leaf node
			percent_done += percent_to_do;
			//st_dlg->StepInState(percent_done);
		}//eo if (e_children.empty())

		else
		{

			//recursive hack for overall performance
			//percent to done is always smaller
			int no_of_children = e_children.size();
			percent_to_do_save = percent_to_do;
			percent_to_do = (short)((float)percent_to_do / (float)no_of_children);
				

			//recursive step
			for (i = e_children.begin(); i != e_children.end(); i++)
			{
				Element new_e = *i;
				CreateDesertSpace(sp, new_e, des_map, inv_des_map, elements, false);
			};//eo for (i...)

			//still part of the recursive hack
			//set back the percent_to_do when a recursive loop is done

			percent_to_do = percent_to_do_save;
		}//eo else if (e_children.begin() == e_children.end() )
		
	}//eo if (root)
	else
	{
		
		//compute decomposition value
		//mapping boolean decomposition value
		//to desert style decomposition value
		set<Element> e_children = e.Element_kind_children();
		set<Element>::iterator i;
		
		if (e_children.empty()) decomposition = 0;
		else
		{
			if (e.decomposition()) decomposition  = 1;
			else decomposition = 2;
		}

		//debug trace
		TRACE ("CreateDesertSpace invoked: parent: %d, Element: %s \n", parent, ((string)e.name()).c_str());

		//create new elenent
		long new_parent = CreateElement(
			((string)e.name()).c_str(),
			space,
			decomposition,
			parent,
			e.externalID());

	/*	long new_parent = CreateElement(
			((string)e.name()).c_str(),
			space,
			decomposition,
			parent,
			e.id(),
			e.externalID());*/

		//insert association in the map
		DoMap(e, des_map, inv_des_map, new_parent);
		//2nd set of elements, we need when mapping associations
		elements.insert(e);

		
		//recursion here
		
		if (e_children.empty() )
		{
			//leaf node
			percent_done += percent_to_do;
			//st_dlg->StepInState(percent_done);
		}//eo if (e_children.empty())
		else
		{

			//recursive hack for overall performance
			//percent to done is always smaller
			int no_of_children = e_children.size();
			percent_to_do_save = percent_to_do;
			percent_to_do = (short)((float)percent_to_do / (float)no_of_children);

			//saving old parent and setting the new parent for Create() calls 
			//when invoking myself recursively
			old_parent = parent;
			parent = new_parent;

			//check recursively all the children
		
			for (i = e_children.begin(); i != e_children.end(); i++)
			{
				Element new_e = *i;
				CreateDesertSpace(sp, new_e, des_map, inv_des_map, elements, false);
			};//eo for (i...)
			//recursive & static hack
			//setting back parent&percent_to_do

			parent = old_parent;
			percent_to_do = percent_to_do_save;

		}//eo else if (e_children.empty())
		
		
	}//eo else if (root) 
	return true;
};



bool CreateCustomDomain(CustomDomain &cd, CustomMember &mb, UdmDesertMap &des_map, DesertUdmMap &inv_des_map, UdmMemberSet &mb_set, bool root)
{
	static long parent;			//id of the parent element
	long old_parent;		//save old parent
	static long cd_id;			//id of the custom domain

	//static and non-static variables
	//for progress indication 
	//(this is weird in case of a recursive function)
	//CStatusDlg * st_dlg = GetStatusDlg(NULL);
	static short percent_done;
	static short percent_to_do;
	short percent_to_do_save;
	if (root) 
	{
		percent_done = 0;
		percent_to_do = 100;
	}





	if (root)
	{

		//recursion here
		set<CustomMember> cm_set = cd.CustomMember_kind_children();
		set<CustomMember>::iterator cm_iterator;
		
		cd_id = GetID(cd, des_map);
		parent = -1;

		if (cm_set.empty())
		{
			//leaf node
			percent_done += percent_to_do;
			//st_dlg->StepInState(percent_done);

		}//eo if cm_set.empty()
		else
		{
			
			//recursive hack for overall performance
			//percent to done is always smaller
			int no_of_children = cm_set.size();
			percent_to_do_save = percent_to_do;
			percent_to_do = (short)((float)percent_to_do / (float)no_of_children);

			for (cm_iterator = cm_set.begin(); cm_iterator != cm_set.end(); cm_iterator++)
			{
				CustomMember cm = *cm_iterator;
				CreateCustomDomain(cd, cm, des_map, inv_des_map, mb_set,  false);
			};//eo for (iterator)

			//still part of the recursive hack
			percent_to_do = percent_to_do_save;


		}//eo if (cm_set.empty())
		
	}//eo if (root)
	else
	{
		long decomposition;
		set<CustomMember> cm_set = mb.CustomMember_kind_children();
		set<CustomMember>::iterator cm_iterator;

		//compute decomposition value
		if (cm_set.empty()) decomposition = 0;
		else
		{
			if (mb.decomposition()) decomposition  = 1;
			else decomposition = 2;
		}

		long new_parent = CreateElement(
			((string)mb.name()).c_str(),
			cd_id, 
			decomposition,
			parent,
			mb.externalID());
/*	
		long new_parent = CreateElement(
			((string)mb.name()).c_str(),
			cd_id, 
			decomposition,
			parent,
			mb.id(),
			mb.externalID());
*/
		DoMap(mb, des_map, inv_des_map, new_parent);
		mb_set.insert(mb);

		if (cm_set.empty())
		{
			percent_done += percent_to_do;
			//st_dlg->StepInState(percent_done);

		}//eo if (cm_set.empty())
		else
		{
			//recursive hack for overall performance
			//percent to done is always smaller
			int no_of_children = cm_set.size();
			percent_to_do_save = percent_to_do;
			percent_to_do = (short)((float)percent_to_do / (float)no_of_children);
			//saving old parent and setting the new parent
			//this is because of static variables
			//still better then keeping on the stack
			old_parent = parent;
			parent = new_parent;
		
			//recursion here
		
			for (cm_iterator = cm_set.begin(); cm_iterator != cm_set.end(); cm_iterator++)
			{
				CustomMember cm = *cm_iterator;
				CreateCustomDomain(cd, cm, des_map, inv_des_map, mb_set, false);
			};//eo for (cm_iterator)

			percent_to_do = percent_to_do_save;
			parent = old_parent;
		}//eo else if (cm_set.empty())
	}//eo else if (root) 
	return true;
};


struct DepthSort {
	bool operator()(const Udm::Object& a, const Udm::Object& b) {
		int aDepth = depth(a);
		int bDepth = depth(b);
		if (aDepth == bDepth) {
			return a < b;
		}
		return aDepth < bDepth;
	}
	static int depth(const Udm::Object& prop) {
		Udm::Object o = prop;
		int depth = 0;
		while (o) {
			depth++;
			o = o.GetParent();
		}
		return depth;
	}
};

bool CreateVariableProperties(UdmDesertMap& des_map, DesertUdmMap &inv_des_map, UdmElementSet& elements)
{
	//progress bar indication
	CStatusDlg * st_dlg = GetStatusDlg(NULL);
	int pos = 0;
	std::set<DesertIface::Element, DepthSort> sortedElements;
	std::copy(elements.begin(), elements.end(), std::inserter(sortedElements, sortedElements.begin()));
	DepthSort sort;
	
	for (auto i = sortedElements.begin(); i != sortedElements.end(); i++)
	{
		Element e= *i;
		set<VariableProperty> vp_set = e.VariableProperty_kind_children();
		set<VariableProperty>::iterator vp_i;
		for (vp_i = vp_set.begin(); vp_i != vp_set.end(); vp_i++)
		{
			VariableProperty vp = *vp_i;
			if(vp.parametric())
			{
				continue;
			}
			long owner_id = GetID(e, des_map);

			Domain vp_d = vp.domain();
			long domain_id = GetID(vp_d, des_map);

			string cpfn;
			if ( strcmp(((string)vp.PCM_STR()).c_str() , "PCM_CUST") )
			{
				//no match
				cpfn = ((string)vp.PCM_STR());
			}
			else cpfn = ((string)vp.CUSTName()).c_str();
			
			//const char* cpfn_char = (LPCTSTR)cpfn;
			long vp_id = CreateVariableProperty(
				((string)vp.name()).c_str(),	//const char *name, 
				cpfn.c_str(),					//const char *cpfn
				owner_id,						//long owner, 
				domain_id );					//long domain);
			
			/*
			
			long vp_id = CreateVariableProperty(
				((string)vp.name()).c_str(),	//const char *name, 
				(LPCTSTR)cpfn,					//const char *cpfn
				owner_id,						//long owner, 
				domain_id,						//long domain);
				vp.id(),
				vp.externalID());
			*/


			DoMap(vp, des_map, inv_des_map, vp_id);
			TRACE("Added VariableProperty: (name %s, owner: %d, domain: %d) :%d\n", 
					((string)vp.name()).c_str(),
					owner_id,
					domain_id,
					long(vp_id));


		};//eo for(vp_i)
		//progress bar status update
		pos++;
		//st_dlg->StepInState((short) (float)pos *100.00 /(float)elements.size());

	};//end for(i)

	return true;
};


bool CreateConstantProperties(UdmDesertMap& des_map, DesertUdmMap &inv_des_map, UdmElementSet& elements, UdmMemberSet &c_members)
{
	//progress bar indication
	//CStatusDlg * st_dlg = GetStatusDlg(NULL);
	int pos = 0;
	
	UdmElementSet::iterator i;
	for (i = elements.begin(); i != elements.end(); i++)
	{
		Element e= *i;
		set<ConstantProperty> cp_set = e.ConstantProperty_kind_children();
		set<ConstantProperty>::iterator cp_i;
		for (cp_i = cp_set.begin(); cp_i != cp_set.end(); cp_i++)
		{
			ConstantProperty cp = *cp_i;
			long owner_id = GetID(e, des_map);

			Domain cp_d = cp.domain();
			long domain_id = GetID(cp_d, des_map);

			string cpfn;
			if ( strcmp(((string)cp.PCM_STR()).c_str() , "PCM_CUST") )
			{
				//no match
				cpfn = ((string)cp.PCM_STR());
			}
			else cpfn = ((string)cp.CUSTName());
			
			int value;
			Member cp_value = cp.value();
			UdmMemberSet::iterator find_res = c_members.find(cp_value);

			if (find_res != c_members.end())
			{
				//a custom member is being assigned
				//since this is really an object
				//the value will be the id of the CustomMember
				//assigned to this property
				CustomMember cm = CustomMember::Cast(cp_value);
				value = GetID(cm, des_map);			
			}
			else
			{
				//a naturalmember is being assigned
				//value will be it's value
				NaturalMember nm = NaturalMember::Cast(cp_value);
				value = nm.value();
			}

			
				long cp_id = CreateConstantProperty(
				((string)cp.name()).c_str(),	//const char *name, 
				cpfn.c_str(),					//const char *cpfn
				owner_id,						//long owner, 
				domain_id,						//long domain);
				value);							//int value
			/*
			long cp_id = CreateConstantProperty(
				((string)cp.name()).c_str(),	//const char *name, 
				(LPCTSTR)cpfn,					//const char *cpfn
				owner_id,						//long owner, 
				domain_id,						//long domain);
				value,							//int value
				cp.id(),
				cp.externalID());*/

			DoMap(cp, des_map, inv_des_map, cp_id);
			
			TRACE("Added ConstantProperty: (name %s, owner: %d, domain: %d, value: %d) :%d\n", 
					((string)cp.name()).c_str(),
					owner_id,
					domain_id,
					value,
					long(cp_id));
			


		};//eo for(cp_i)
		//progress bar status update
		pos++;
	//	st_dlg->StepInState((short) (float)pos *100.00 /(float)elements.size());

	};//end for(i)

	return true;
};



bool CreateAssignments(UdmDesertMap& des_map, DesertUdmMap &inv_des_map, UdmElementSet& elements, UdmMemberSet &c_members)
{
	//progress bar indication
//	CStatusDlg * st_dlg = GetStatusDlg(NULL);
	int pos = 0;
	
	UdmElementSet::iterator i;
	for (i = elements.begin(); i != elements.end(); i++)
	{
		Element e= *i;
		set<AssignedValues> av_set = e.AssignedValues_kind_children();
		set<AssignedValues>::iterator av_i;
		for (av_i = av_set.begin(); av_i != av_set.end(); av_i++)
		{
			AssignedValues av = *av_i;
		
			long owner_id = GetID(e, des_map);
			
			Property av_prop = av.properties_end();
			long prop_id = GetID(av_prop, des_map);

			Domain av_dom = av_prop.domain();
			long d_id = GetID(av_dom, des_map);

			Member av_memb = av.values_end();
			UdmMemberSet::iterator find_res = c_members.find(av_memb);

#ifndef DOUBLE_MTBDD
			long value;
#else
			double value;
#endif
			long av_id ;
			if (find_res != c_members.end())
			{
				//a custom member is being assigned
				//since this is really an object
				//the value will be the id of the CustomMember
				//assigned to this property
				CustomMember cm = CustomMember::Cast(av_memb);
				value = GetID(cm, des_map);			
			}
			else
			{
				// a naturalmember is being assigned
				//value will be it's value
				NaturalMember nm = NaturalMember::Cast(av_memb);
				value = nm.value();
			}

			//av_id = AddtoVariableProperty(prop_id, owner_id, value, d_id);
			//av_id = AddtoVariableProperty(prop_id, ((string)av.name()).c_str(), owner_id, value, d_id, av.id(), av.externalID());
			av_id = AddtoVariableProperty(prop_id, ((string)av.name()).c_str(), owner_id, value, d_id);
			DoMap(av, des_map, inv_des_map,  av_id);


		};//eo for(vp_i)
		//progress bar indication
		pos++;
		//st_dlg->StepInState((short) (float)pos *100.00 /(float)elements.size());

	};//end for(i)
	return true;

};




bool CreateDesertConstraintSet(ConstraintSet &cs, UdmDesertMap &des_map, DesertUdmMap &inv_des_map )
{
	Constraint ct;
	Element owner;
	long cts_id;
	set<Constraint> ct_set = cs.Constraint_children();
	set<Constraint>::iterator ct_iterator;

	ASSERT(!ct_set.empty());
	//create constraint set
	cts_id = CreateConstraintSet(((string)cs.name()).c_str());
	DoMap(cs, des_map, inv_des_map, cts_id);


	for(ct_iterator = ct_set.begin(); ct_iterator != ct_set.end(); ct_iterator++)
	{
		//get owner element
		ct = *(ct_iterator);
		owner = ct.context();
		
		//lookup in the map 
		long owner_id = GetID(owner, des_map);

		//create constraint
		
		long ct_id = CreateConstraint(((string)ct.name()).c_str(), 
						cts_id, 
						owner_id, 
						((string)ct.expression()).c_str());
		/*
		long ct_id = CreateConstraint(((string)ct.name()).c_str(), 
						cts_id, 
						owner_id, 
						((string)ct.expression()).c_str(),
						ct.id(),
						ct.externalID());
		*/

		DoMap(ct, des_map, inv_des_map,  ct_id);

	}//eo for (ct_iterator)
	return true;
};//eo bool CreateDesertConstrainSet

bool CreateConstraints(DesertSystem &ds, UdmDesertMap &des_map, DesertUdmMap &inv_des_map)
{

	//constrainsets, constrainst
	set<ConstraintSet> c_sets = ds.ConstraintSet_kind_children();
	set<ConstraintSet>::iterator cts_iterator;


	if (!c_sets.empty())
	{
		ConstraintSet cs;
		for (cts_iterator = c_sets.begin(); cts_iterator != c_sets.end(); cts_iterator++)
		{
			cs = *(cts_iterator);
			set<Constraint> ct_set = cs.Constraint_kind_children();

			if (ct_set.empty()) continue;
			CreateDesertConstraintSet(cs, des_map, inv_des_map);

		}//eo for cts_iterator
	}//eo if (!c_sets.empty())
	return true;
};

bool CreateElementRelations(DesertSystem &ds, UdmDesertMap &des_map, DesertUdmMap &inv_des_map)
{
	//progress bar indication
	//CStatusDlg * st_dlg = GetStatusDlg(NULL);
	int pos = 0;

		//ElementRelations
		//
		set<ElementRelation> er_set = ds.ElementRelation_kind_children();
		set<ElementRelation>::iterator  e_iterator;
		if (!er_set.empty())
		{
			ElementRelation er;
			for(e_iterator = er_set.begin(); e_iterator != er_set.end(); e_iterator ++)
			{
				er = *e_iterator;
				long src_id, dst_id, par_id;
				Element src = er.src_end();
				Element dst = er.dst_end();
				DesertBase par1 = DesertBase::Cast(src.parent());
				DesertBase par2 = DesertBase::Cast(dst.parent());
				
				//enforce same parent
				ASSERT( (long)par1.id() == (long)par2.id());
					
				UdmDesertMap::iterator i;
				
				src_id = GetID(src, des_map);
				dst_id = GetID(dst, des_map);
				par_id = GetID(par1, des_map);

				TRACE("%s relation: src_id: %d, dst_id: %d, context: %d\n",
					((string)er.name()).c_str(),
					src_id, 
					dst_id,
					par_id);

				long rel_id = CreateRelation(par_id, src_id, dst_id);
				//long rel_id = CreateRelation(par_id, src_id, dst_id, er.id(), er.externalID());
				DoMap(er, des_map, inv_des_map,  rel_id);
				
				//progress bar indication
				pos++;
				//st_dlg->StepInState((short) (float)pos *100.00 /(float)er_set.size());


			}//eo for (e_iterator)

		}//if (!er_set.empty())

		return true;
};


bool CreateMemberRelations(DesertSystem &ds, UdmDesertMap &des_map, DesertUdmMap &inv_des_map)
{

		//MemberRelations
		//
		set<MemberRelation> mr_set = ds.MemberRelation_kind_children();
		set<MemberRelation>::iterator  m_iterator;
		if (!mr_set.empty())
		{
			MemberRelation mr;
			//progress bar indication
		//	CStatusDlg * st_dlg = GetStatusDlg(NULL);
			int pos = 0;

			for(m_iterator = mr_set.begin(); m_iterator != mr_set.end(); m_iterator ++)
			{
				mr = *m_iterator;
				long src_id, dst_id, par_id;
				CustomMember src = mr.src_end();
				CustomMember dst = mr.dst_end();

				DesertBase par1 = DesertBase::Cast(src.parent());
				DesertBase par2 = DesertBase::Cast(dst.parent());
				
				//enforce same parent
				ASSERT( (long)par1.id() == (long)par2.id());
					
				UdmDesertMap::iterator i;
				
				src_id = GetID(src, des_map);
				dst_id = GetID(dst, des_map);
				par_id = GetID(par1, des_map);

				TRACE("%s member relation: src_id: %d, dst_id: %d, context: %d\n",
					((string)mr.name()).c_str(),
					src_id, 
					dst_id,
					par_id);

				long rel_id = CreateRelation(par_id, src_id, dst_id);
				//long rel_id = CreateRelation(par_id, src_id, dst_id, mr.id(), mr.externalID());
				DoMap(mr, des_map, inv_des_map,  rel_id);

				//progress bar indication
				pos++;
				//st_dlg->StepInState((short) (float)pos *100.00 /(float)mr_set.size());

			}//eo for (e_iterator)

		}//if (!mr_set.empty())

		return true;
};


bool CreateNaturalDomains(DesertSystem& ds, UdmDesertMap& des_map, DesertUdmMap &inv_des_map)
{
		//natural domains
		set<NaturalDomain> nd_set = ds.NaturalDomain_kind_children();
		set<NaturalDomain>::iterator  nd_iterator;
		

		if (!nd_set.empty())
		{
			//progress bar indication
		//	CStatusDlg * st_dlg = GetStatusDlg(NULL);
			int pos = 0;

			NaturalDomain nd;
			
			for (nd_iterator = nd_set.begin(); nd_iterator != nd_set.end(); nd_iterator++)
			{
				nd = *(nd_iterator);
				
				long d_id = CreateNaturalDomain( 
					(((string)nd.name()).c_str()),
					nd.maximum(),
					nd.minimum());
					
				/*
				long d_id = CreateNaturalDomain( 
					(((string)nd.name()).c_str()),
					nd.maximum(),
					nd.minimum(),
					nd.id(),
					nd.externalID());
				*/

				TRACE("Created NaturalDomain %s, min: %d, max: %d, id: %d\n",
					((string)nd.name()).c_str(),
					(long)nd.minimum(),
					(long)nd.maximum(),
					(long)d_id);

				DoMap(nd, des_map,inv_des_map,  d_id);
				//progress bar indication
				pos++;
			//	st_dlg->StepInState((short) (float)pos *100.00 /(float)nd_set.size());


			}//eo nd_iterator

		}//eo if (!nd_set.empty())
		return true;
};


bool CreateCustomDomains(DesertSystem& ds, UdmDesertMap& des_map, DesertUdmMap &inv_des_map,  UdmMemberSet &mb_set)
{
		//custom domains
		set<CustomDomain> cd_set = ds.CustomDomain_kind_children();
		set<CustomDomain>::iterator  cd_iterator;
		

		if (!cd_set.empty())
		{
			CustomDomain cd;

			
			for (cd_iterator = cd_set.begin(); cd_iterator != cd_set.end(); cd_iterator++)
			{
				cd = *(cd_iterator);
				CustomMember dummy;
				//
				//create the custom domain 
				//

				long cd_id =  CreateCustomDomain(((string)cd.name()).c_str());
				//long cd_id =  CreateCustomDomain(((string)cd.name()).c_str(), cd.id(), cd.externalID());
		
				//inserting custom domain in map
				DoMap(cd,des_map, inv_des_map, cd_id);
				TRACE("Created customdomain %s: %d\n", ((string)cd.name()).c_str(), (long)cd_id);
		
				
				
				CreateCustomDomain(cd, dummy, des_map, inv_des_map, mb_set, true);
			}//eo cd_iterator

		}//eo if (!cd_set.empty())
		return true;
};
#endif