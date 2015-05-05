#include "DesertManager.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "DesertIface.h"
#include "DesertIfaceBack.h"
#include <afxtempl.h>
#include "DesMap.h"
#include "DesBackMap.h"

#include <boost/crc.hpp>  // for boost::crc_32_type

using namespace DesertIface;

bool getElement(Element &elem_parent, Element &elem_ret, const std::string &elem_name)
{
	const set<Element> elem_set = elem_parent.Element_kind_children();
	for(set<Element>::const_iterator i=elem_set.begin();i!=elem_set.end();++i)
	{
		if((std::string)((*i).name()) == elem_name)
		{
			elem_ret = (*i);
			return true;
		}
	}
	return false;
}

DesertManager::DesertManager() : applyAll(false),cfgCount(0),constraints("") 
{
}

DesertManager::~DesertManager()
{
	if ( logFile.is_open())
		logFile.close();
}

void DesertManager::registerImporter(const std::string &in)
{
	if(!in.empty())
		importer = in;
	else
		throw Desert_exception("The importer is NULL. ");
}

void DesertManager::registerExporter(const std::string &ex)
{
	if(!ex.empty())
		exporter = ex;
	else
		throw Desert_exception("The exporter is NULL. ");
}

void DesertManager::importModel(const std::string &modelFile)
{
	if(modelFile.empty()) 
		throw Desert_exception("The input model is NULL. ");
	if(importer.empty()) 
		throw Desert_exception("The importer is NULL. ");

	inputModel = modelFile;

	if ( logFile.is_open())
		logFile.close();

	std::string logFileName = inputModel;
	desertIfaceFile = inputModel;
	size_t extPos = inputModel.find_last_of(".");
	if(extPos != std::string::npos)
	{
		desertIfaceFile.replace(extPos,4,"_Desert.xml");
		logFileName.replace(extPos,4,"_log.txt");
	}
	else
	{
		desertIfaceFile.append("_Desert.xml");
		logFileName.append("_log.txt");
	}

	logFile.open(logFileName.c_str());
	std::string cmd = importer+" "+inputModel;
	if(system(cmd.c_str()))
	{
		desertIfaceFile.clear();
		std::string err = "importModel error: <"+cmd+"> fails!";
		log(err);
		throw Desert_exception(err);
	}
	else
	{
		log("Input model: "+inputModel);
		log("Execute: <"+cmd+"> success!");
		log("Output: "+desertIfaceFile+"\n");
	}
}

void DesertManager::addConstraint(const std::string &name, const std::string &expr, const std::string &path)
{
	try
	{
		Udm::SmartDataNetwork dn(diagram);
		dn.OpenExisting(desertIfaceFile,"DesertIface",Udm::CHANGES_PERSIST_ALWAYS);
		DesertSystem dsys = DesertSystem::Cast(dn.GetRootObject());

		std::string fullpath = path;
		std::string elemname;
		size_t pos = fullpath.find_first_of("\\");
		if(pos != std::string::npos)
		{
			elemname = fullpath.substr(0, pos);
			fullpath = fullpath.substr(pos+1, fullpath.length()-pos);
		}
		else
		{
			elemname = fullpath;
			fullpath.clear();
		}
		
		Element rootelem;
		set<Space> spaces = dsys.Space_kind_children();
		set<Element> elems;
		for(set<Space>::iterator si=spaces.begin(); si!=spaces.end()&&rootelem==&Udm::_null; ++si)
		{
			elems = (*si).Element_kind_children();
			while(rootelem==&Udm::_null&&!elems.empty())
			{
				set<Element>::iterator i;
				for(i=elems.begin();i!=elems.end()&&rootelem==&Udm::_null;++i)
				{
					if((std::string)(*i).name()==elemname)
						rootelem = (*i);
				}
				if(rootelem!=&Udm::_null) break;
				set<Element> tmp_elems = elems;
				elems.clear();
				for(i=tmp_elems.begin();i!=tmp_elems.end();++i)
				{
					set<Element> child_elems = (*i).Element_kind_children();
					elems.insert(child_elems.begin(),child_elems.end());
				}
			}
		}

		if(rootelem!=&Udm::_null)
		{
			Element elem_ret = rootelem;
			while(!fullpath.empty())
			{
				elem_ret = &Udm::_null;
				pos = fullpath.find_first_of("\\");
				if(pos != std::string::npos)
				{
					elemname = fullpath.substr(0, pos);
					fullpath = fullpath.substr(pos+1, fullpath.length()-pos);
				}
				else
				{
					elemname = fullpath;
					fullpath.clear();
				}

				if(!getElement(rootelem, elem_ret, elemname)) break;
				else
					rootelem = elem_ret;		
			}

			if(elem_ret!=&Udm::_null)
			{
				ConstraintSet conset;
				set<ConstraintSet> consets = dsys.ConstraintSet_kind_children();
				if(consets.empty())
				{
					conset = ConstraintSet::Create(dsys);
					conset.name() = "constraints";
				}
				else
					conset = *(consets.begin());

				Constraint newcon = Constraint::Create(conset);
				newcon.id() = newcon.externalID() = newcon.uniqueId();
				newcon.name() = name;
				newcon.expression() = expr;
				newcon.context() = elem_ret;
				applyConstraint(name);
			}
			else
			{
				dn.CloseWithUpdate();
				std::string err = "Add Constraint error: The component with path: "+path+" cannot be found.";
				log(err);
				throw Desert_exception(err);
			}
		}
		else
		{
			dn.CloseWithUpdate();
			std::string err = "Add Constraint error: The component with path: "+path+" cannot be found.";
			log(err);
			throw Desert_exception(err);
		}
		log("Add Constraint: <"+expr+"> succees!\n");
	}
	catch(udm_exception &e)
	{
		std::string err = "AddConstraint error: "+(std::string)e.what();
		log(err);
		throw Desert_exception(err);
	}
}

void DesertManager::applyConstraint(const std::string &constraint_name)
{
	if(constraint_name.empty()) return;
	if(constraints.empty())
		constraints = constraint_name;
	else
		constraints.append(":"+constraint_name);
}

void DesertManager::applyConstraints(const std::string &constraints_list)
{
	constraints = constraints_list;
}

void DesertManager::applyAllConstraints()
{
	applyAll = true;
	constraints = "applyAll";
}

void DesertManager::exportModel(int cfgIndex)
{
	if(desertIfaceBackFile.empty()) 
	{
		std::string err("exportModel error: "+desertIfaceBackFile+" is null.");
		log(err);
		throw Desert_exception(err);
	}

	char id[16];
	itoa (cfgIndex, id, 10);
	if(cfgIndex>cfgCount || cfgIndex<1)
	{
		std::string err("exportModel error: configuration index "+(std::string)id+" is out of boundary.");
		log(err);
		throw Desert_exception(err);
	}


	std::string cmd = exporter+" "+desertIfaceBackFile+" "+inputModel+" "+id+" s";
	if(system(cmd.c_str()))
	{
		log("exportModel error: <"+cmd+"> fails!");
	}
	else
	{
		log("export model: <"+cmd+"> success!");
	}
}

void DesertManager::exportAll()
{
	for(int i=1; i<=cfgCount; ++i)
	{
		exportModel(i);
	}
}

int DesertManager::runDesert()
{
	desertIfaceBackFile = desertIfaceFile;
	size_t extPos = desertIfaceBackFile.find_last_of(".");
	if(extPos != std::string::npos)
	{
		desertIfaceBackFile.replace(extPos,4,"_Back.xml");
	}

	try
	{
		Udm::SmartDataNetwork ds_dn(diagram);
		ds_dn.OpenExisting(desertIfaceFile,"DesertIface",Udm::CHANGES_PERSIST_ALWAYS);
		DesertSystem ds = DesertSystem::Cast(ds_dn.GetRootObject());
		

		Udm::SmartDataNetwork dbs_dn(DesertIfaceBack::diagram);
		dbs_dn.CreateNew(desertIfaceBackFile,"DesertIfaceBack",DesertIfaceBack::DesertBackSystem::meta,Udm::CHANGES_PERSIST_ALWAYS);
		DesertIfaceBack::DesertBackSystem dbs = DesertIfaceBack::DesertBackSystem::Cast(dbs_dn.GetRootObject());

		DesertInit(((string)ds.SystemName()).c_str());
		set<Space> spaces = ds.Space_kind_children();
		set<Space>::iterator sp_iterator;

		ASSERT(spaces.begin() != spaces.end());	
		
		UdmDesertMap des_map;
		DesertUdmMap inv_des_map;


		UdmElementSet elements;
		UdmMemberSet custom_members;
					
		//Build spaces
		if (!spaces.empty())
		{
			Space sp;
			Element dummy;
			
			for (sp_iterator = spaces.begin(); sp_iterator != spaces.end(); sp_iterator ++)
			{
				Space sp = *(sp_iterator);
				//TRACE("Creating space: %s\n", ((string)sp.name()).c_str());
				CreateDesertSpace(sp, dummy, des_map, inv_des_map, elements, true);
			}//eo for (sp_iterator)
		}//if (!spaces.empty())
		
		//create relations
		//s_dlg.SetStatus(SD_ERS);
		CreateElementRelations(ds, des_map, inv_des_map);

		//create constrains
		//s_dlg.SetStatus(SD_CTS);
		CreateConstraints(ds, des_map, inv_des_map);

		//create natural domains
		//s_dlg.SetStatus(SD_NDS);
		CreateNaturalDomains(ds, des_map, inv_des_map);

		//create custom domains
		//s_dlg.SetStatus(SD_CDS);
		CreateCustomDomains(ds, des_map, inv_des_map, custom_members);

		//create custom domain relations
		//s_dlg.SetStatus(SD_MRS);
		CreateMemberRelations(ds, des_map, inv_des_map);

		//create variable properties
		//s_dlg.SetStatus(SD_VPS);
		CreateVariableProperties(des_map, inv_des_map, elements);

		//create constant properties
		//s_dlg.SetStatus(SD_CPS);
		CreateConstantProperties(des_map, inv_des_map, elements, custom_members);

		//create assignments for VariableProperties
		//s_dlg.SetStatus(SD_ASS);
		CreateAssignments(des_map, inv_des_map, elements, custom_members);

		//invoking Desert UI
		//s_dlg.SetStatus(SD_GUI);

		using namespace BackIfaceFunctions;
	
				
		DBConfigurations * confs = (DBConfigurations * )DesertFinitNoGui(true,true,constraints.c_str());
		
		if (confs)
		{
			dbs.SystemName() = ds.SystemName();

			//elements, custommembers and natural members will be created
			//on the fly, as it they are participating in an assignment

			int confn = confs->GetCount();
			POSITION pos = confs->GetHeadPosition();
			//s_dlg.SetStatus(SD_PREP);
			while(pos)
			{

				DBConfiguration * config = confs->GetNext(pos);
				if (config)
				{
					DesertIfaceBack::Configuration dummy;
					//check for the existence of elements, properties,
					//and values which will be associated then
					POSITION pos1 = config->assignments.GetHeadPosition();
					while (pos1)
					{
						long valid_ass = config->assignments.GetNext(pos1);				
						BackIfaceFunctions::CreatePropertyAssignment(dbs,dummy, inv_des_map, des_map, valid_ass, false);
					}
					POSITION pos2 = config->alt_assignments.GetStartPosition();
					while (pos2)
					{
						long alt, alt_of;
						config->alt_assignments.GetNextAssoc(pos2, alt_of, alt);
						BackIfaceFunctions::CreateAlternativeAssignment(dbs, dummy, inv_des_map, alt_of, alt, false);

					};
				}//eo if (config)	
				//s_dlg.StepInState( (double)(((double)(++count))/((double)(confs->GetCount())))*100.0 );	//progress bar
			}//eo while(pos)
			pos = confs->GetHeadPosition();
			//s_dlg.SetStatus(SD_BACK);
			while(pos)
			{
				DBConfiguration * config = confs->GetNext(pos);
				if (config)
				{
					//create configuration
					DesertIfaceBack::Configuration dib_conf = DesertIfaceBack::Configuration::Create(dbs);
					CString s;
					s.Format(_T("Conf. no: %d"),config->id);
					std::stringstream buf;
					CT2CA pszConvertedAnsiString (s);
					std::string temp (pszConvertedAnsiString);
					dib_conf.name() =temp.c_str(); 
					dib_conf.id() = config->id;


					POSITION pos1 = config->assignments.GetHeadPosition();
					while (pos1)
					{
						long valid_ass = config->assignments.GetNext(pos1);				
						BackIfaceFunctions::CreatePropertyAssignment(dbs,dib_conf, inv_des_map, des_map, valid_ass, true);
					}
					POSITION pos2 = config->alt_assignments.GetStartPosition();
					while (pos2)
					{
						long alt, alt_of;
						config->alt_assignments.GetNextAssoc(pos2, alt_of, alt);
						BackIfaceFunctions::CreateAlternativeAssignment(dbs, dib_conf, inv_des_map, alt_of, alt, true);

					};
					
					//TRACE("Got %d of configurations!\n", confs->GetCount());


					delete config;
				}//eo if (config)
				//s_dlg.StepInState( (double)(((double)(++count))/((double)(confs->GetCount())))*100.0 );	//progress bar
			}//eo while(pos)
			delete confs;
		}//eo if (configs)

		set<DesertIfaceBack::Configuration> cfgs = dbs.Configuration_kind_children();
		cfgCount = cfgs.size();
		for(set<DesertIfaceBack::Configuration>::iterator it=cfgs.begin();it!=cfgs.end();++it)
		{
			DesertIfaceBack::Configuration cfg = *it;
			std::string cfgid = cfg.cfgId();
			boost::crc_32_type id_hash;
			id_hash.process_bytes(cfgid.c_str(), cfgid.length());
			int uid = id_hash.checksum();
			std::stringstream sstream;
			sstream << std::hex <<uid;
			cfg.cfgId() = sstream.str();
		}

		ds_dn.CloseWithUpdate();
		dbs_dn.CloseWithUpdate();

		log("runDesert on "+desertIfaceFile+" success!");
		log("Output: "+desertIfaceBackFile+"\n");

	}//eo try
	catch(CDesertException *e)
	{
		std::string err("runDesert error: "+(std::string)((LPCTSTR)e->GetErrorMessage()));
		log(err);
		throw Desert_exception(err);
	}
	catch(udm_exception e)
	{
		std::string err("runDesert error: "+(std::string)e.what());
		log(err);
		throw Desert_exception(err);
	}
	return cfgCount;
}

void DesertManager::log(const std::string &info)
{
	if (logFile.is_open())
		logFile<<info<<std::endl;
}