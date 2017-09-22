// UdmApp.cpp: implementation of the CUdmApp class.
// This file was automatically generated as UdmApp.cpp
// by UDM Interpreter Wizard on Thursday, March 27, 2008 10:37:31
#include "stdafx.h"
#include "UdmApp.h"
#include "UdmConfig.h"
#include "Uml.h"
#include "CyPhyML.h"
#include "UdmUtil.h"
#include "CyPhyCAExporter.h"
#include "ConfigExportDialog.h"
#include "UdmConsole.h"
#include "StatusDialog.h"
#include "time.h"
#include "UdmBase.h"

#include "UmlExt.h"
#include "UdmStatic.h"

#include "../CyPhyElaborate/CyPhyElaborateTraceability.h"

using namespace Uml;
using namespace Udm;

using namespace CyPhyML;

std::string getFileNameNoExt(const std::string &fullpath)
{
	std::string ret = fullpath;
	size_t pos = ret.rfind("\\");
	if(pos!=string::npos)
		ret = ret.substr(pos+1);
	pos = ret.find(".");
	if(pos!=string::npos)
		ret = ret.substr(0, pos);
	return ret;
}

std::string appendObjLink(const Udm::Object &obj)
{
	if (!obj)
		return "null";
	return ("<A HREF=\"mga:"+ UdmGme::UdmId2GmeId(obj.uniqueId()) + "\">" + UdmUtil::ExtractName(obj) + "</A>");
}

void udmCopy(ObjectImpl *p_srcRoot, ObjectImpl *p_dstRoot, DataNetwork *p_dstBackend, UdmUtil::copy_assoc_map &cam, const string &m_backend_ext, bool inside_lib = false)
{
	vector<ObjectImpl*> children = p_srcRoot->getChildren(NULL, p_srcRoot->type());
	for (vector<ObjectImpl*>::const_iterator i = children.begin(); i != children.end(); i++) 
	{

		ObjectImpl *p_srcChild = *i;

		string lib_name;
		if (!p_srcChild->getLibraryName(lib_name)) {
			p_srcChild->release();
			continue;
		}

		if (lib_name.length() == 0) 
		{ // a library with an empty lib name
			Udm::Object srcChild = p_srcChild->clone();
			lib_name = UdmUtil::ExtractName(srcChild);
			if (lib_name.length() == 0 || lib_name == "<empty string>" || lib_name == "<no name specified>")
				lib_name = "lib";
		}

		// remove file extension if it looks like a
		// backend designation
		if (lib_name.length() > 4 && lib_name[ lib_name.length() - 4 ] == '.')
			lib_name = lib_name.substr(0, lib_name.length() - 4);

		string new_lib_name = lib_name + "." + m_backend_ext;

		//new_lib_name could contain the path which is not exist

		// create datanetwork for standalone library and build map
		// from source object to standalone library object
		Udm::SmartDataNetwork libDN(CyPhyML::diagram);
		const ::Uml::Class & safe_type = ::Uml::SafeTypeContainer::GetSafeType(p_srcChild->type());
		libDN.CreateNew(new_lib_name, "CyPhyML", safe_type, Udm::CHANGES_LOST_DEFAULT);
		Object p_root = libDN.GetRootObject();
		ObjectImpl *p_libRoot = p_root.__impl();

		// copy, including nested libraries
		UdmUtil::copy_assoc_map lib_cam;
		udmCopy(p_srcChild, p_libRoot, &libDN, lib_cam, m_backend_ext, true);
		lib_cam.insert( make_pair(p_srcChild->clone(), p_libRoot->clone()) );
		libDN.SaveAs(new_lib_name);


		// attach library to destination host and build map
		// from standalone library object to private copy object
		// (the map already has a mapping from src root to dst root)
		Udm::t_lib_to_copy_impl_map lib_copy_impl_cam;
		ObjectImpl *p_newLibRoot = p_dstRoot->AttachLibrary(p_libRoot, new_lib_name, &lib_copy_impl_cam);

		// we need a new map with Object elements instead of ObjectImpl* elements
		UdmUtil::copy_assoc_map lib_copy_cam;
		for (Udm::t_lib_to_copy_impl_map::const_iterator i = lib_copy_impl_cam.begin(); i != lib_copy_impl_cam.end(); i++) {
			lib_copy_cam.insert( make_pair(i->first, i->second) );
		}


		// map from source to library private copy
		for (UdmUtil::copy_assoc_map::const_iterator i = lib_cam.begin(); i != lib_cam.end(); i++) {
			UdmUtil::copy_assoc_map::const_iterator fi = lib_copy_cam.find(i->second);
			if (fi == lib_copy_cam.end())
				throw udm_exception("object not found in map");
			cam.insert( make_pair(i->first, fi->second) );
		}

		libDN.CloseNoUpdate();

		cam.insert( make_pair(p_srcChild, p_newLibRoot) );
	}

	UdmUtil::CopyOpts opts = { true, inside_lib };
	UdmUtil::CopyObjectHierarchy(p_srcRoot, p_dstRoot, p_dstBackend, cam, opts);
}

std::string getTimeNow()
{
	char currtime[128];
	time_t now = time(0);
	struct tm* tm = localtime(&now);
	sprintf_s(currtime,"%02d-%02d-%04d  %02d:%02d:%02d", (tm->tm_mon + 1), tm->tm_mday, (tm->tm_year+1900), tm->tm_hour, tm->tm_min, tm->tm_sec);
	return currtime;
}

/*********************************************************************************/
/* Initialization function. The framework calls it before preparing the backend. */
/* Initialize here the settings in the config global object.					  */
/* Return 0 if successful.														  */
/*********************************************************************************/
int CUdmApp::Initialize()
{

	// TODO: Your initialization code comes here...
	return 0;
}



/* 
Remarks to CUdmApp::UdmMain(...):
0.	The p_backend points to an already open backend, and the framework 
	closes it automatically. DO NOT OPEN OR CLOSE IT!
	To commit changes use p_backend->CommitEditSequence().
	To abort changes use p_backend->AbortEditSequence().
	To save changes to a different file use p_backend->SaveAs() or p_backend->CloseAs().

1.	Focus is the currently open model.

2.	The possible values for param (from GME DecoratorLib.h component_startmode_enum):
	GME_MAIN_START			=   0,
	GME_BROWSER_START		=   1,
	GME_CONTEXT_START		=   2,
	GME_EMBEDDED_START		=   3,
	GME_MENU_START			=  16,
	GME_BGCONTEXT_START		=  18,
	GME_ICON_START			=  32,
	METAMODEL_CHECK_SYNTAX	= 101

 3. The framework catches all the exceptions and reports the error in a message box,
	clean up and close the transactions aborting the changes. You can override this 
	behavior by catching udm_exception. Use udm_exception::what() to form an error 
	message.
*/

/***********************************************/
/* Main entry point for Udm-based Interpreter  */
/***********************************************/
void CUdmApp::UdmMain(
					 Udm::DataNetwork* p_backend,		// Backend pointer(already open!)
					 Udm::Object focusObject,			// Focus object
					 set<Udm::Object> selectedObjects,	// Selected objects
					 long param,						// Parameters
					 CComPtr<IMgaProject> mgaProject)
{
	/*char currtime[128];
	time_t now = time(0);
	struct tm* tm = localtime(&now);
	sprintf(currtime,"%02d-%02d-%04d  %02d:%02d:%02d", (tm->tm_mon + 1), tm->tm_mday, (tm->tm_year+1900), tm->tm_hour, tm->tm_min, tm->tm_sec);
	std::string ct(currtime);
*/
	std::string start_time = getTimeNow();
	try{
		CyPhyML::RootFolder cyphy_rf = CyPhyML::RootFolder::Cast(p_backend->GetRootObject());
		set<CyPhyML::CWC> cwcs;
		if(	focusObject!=Udm::null 
			&& (Uml::IsDerivedFrom(focusObject.type(), CyPhyML::CWC::meta)))
		{	
			cwcs.insert(CyPhyML::CWC::Cast(focusObject));
		}
		else
		{
			for(set<Udm::Object>::iterator i=selectedObjects.begin();i!=selectedObjects.end();++i)
			{
				Udm::Object currObj = *i;
				if(Uml::IsDerivedFrom(currObj.type(), CyPhyML::CWC::meta))
				{
					cwcs.insert(CyPhyML::CWC::Cast(currObj));					
				}

				// User has selected a Configurations container -- elaborate all CWC within.
				if(Uml::IsDerivedFrom(currObj.type(), CyPhyML::Configurations::meta))
				{
					set<CWC> s_cwc = Configurations::Cast(currObj).CWC_children();
					for (set<CWC>::const_iterator i = s_cwc.begin(); i != s_cwc.end(); i++)
					{
						cwcs.insert(*i);
					}
				}
			}
		}

		if(cwcs.empty()){
			AfxMessageBox(L"Please open a CWC model or select CWC model(s) to invoke the interpreter.");
			return;
		}

		CyPhyML::Configurations cfg_model = (*(cwcs.begin())).Configurations_parent();
		CyPhyML::DesignContainer rootDC = cfg_model.DesignContainer_parent();

		CyPhyML::Configurations cfgModel;
		bool useCurrent = true;
		bool flatten = false;
		std::string outFdr;
		
		// Hide dialog box in silent mode OR if user isn't pressing CTRL
		if (param == GME_SILENT_MODE ||
			!((GetKeyState(VK_CONTROL) & 0x8000)))
		{
			param = GME_SILENT_MODE;
		}
		if (param == GME_SILENT_MODE)
		{
			useCurrent = true;
			flatten = false;
			outFdr = "notused";
		}
		else
		{
			CConfigExportDialog cfgExDlg;
			if (cfgExDlg.DoModal()==IDOK)
			{
				useCurrent = cfgExDlg.m_useCurrent;
				flatten = cfgExDlg.m_flatten;
				outFdr = (LPCSTR)((CStringA)cfgExDlg.m_outputfdr);
			}
			else 
				return;
		}

		int idCnt = cwcs.size();	
		CStatusDialog prgDlg;
		if (param != GME_SILENT_MODE)
		{
			prgDlg.Create(IDD_PROGRESS_DIALOG);
			GetStatusDlg(&prgDlg);
			prgDlg.SetProgress("Preparing to export models...");
		}
		
		if(!useCurrent)
		{
			if (param != GME_SILENT_MODE)
			{
				prgDlg.SetRange(40*idCnt);
			}
			std::string outfileBase = outFdr+"\\"+getFileNameNoExt((LPCSTR)((CStringA)mgaPath));
			for(set<CyPhyML::CWC>::iterator i=cwcs.begin();i!=cwcs.end();++i)
			{
				CyPhyML::CWC cwc = *i;
				std::string outfile = outfileBase+"_"+(std::string)cwc.name()+".mga";					
				std::string msg ="create new CyPhy model for: "+(std::string)cwc.name()+"...";
				if (param != GME_SILENT_MODE)
				{
					prgDlg.SetProgress(CString(msg.c_str()));
				}

				Udm::SmartDataNetwork outDN(CyPhyML::diagram);
				outDN.CreateNew(outfile,"CyPhyML", CyPhyML::RootFolder::meta, Udm::CHANGES_LOST_DEFAULT);
				CyPhyML::RootFolder out_rf = CyPhyML::RootFolder::Cast(outDN.GetRootObject());
				UdmUtil::copy_assoc_map copymap;
				udmCopy(p_backend->GetRootObject().__impl(), outDN.GetRootObject().__impl(),&outDN,copymap,"mga");
			//	UdmUtil::CopyObjectHierarchy(p_backend->GetRootObject().__impl(), outDN.GetRootObject().__impl(), &outDN, copymap);		
				UdmUtil::copy_assoc_map::iterator map_it = copymap.find(cwc);
				ASSERT(map_it!=copymap.end());
				Udm::Object to_cwc_obj = (*map_it).second;
				ASSERT(to_cwc_obj.type() == CyPhyML::CWC::meta);

				if (param != GME_SILENT_MODE)
				{
					prgDlg.SetProgress(CString(msg.c_str()));
				}

				//CyPhyML::Components coms = CyPhyML::Components::Create(out_rf);
				//coms.name() = "Configurations ("+start_time+")";
				CyPhyML::ComponentAssemblies ca_rootFdr = CyPhyML::ComponentAssemblies::Create(out_rf);
				ca_rootFdr.name() = "Generated configurations";
				CyPhyML::ComponentAssemblies ca_folder_dc = CyPhyML::ComponentAssemblies::Create(ca_rootFdr);
				ca_folder_dc.name() = rootDC.name();
				CyPhyML::ComponentAssemblies ca_folder = CyPhyML::ComponentAssemblies::Create(ca_folder_dc);
				ca_folder.name() = "Configurations ("+start_time+")";
				CyPhyCAExporter dexport(ca_folder, CyPhyML::CWC::Cast(to_cwc_obj), flatten);
				dexport.showGui = param != GME_SILENT_MODE;
				dexport.createComponentAssembly();
				traceability.Attach(new CyPhyElaborateTraceability(std::unique_ptr<std::map<Udm::Object, Udm::Object> >(std::move(dexport.copiedObjectsToOriginals))));

				//outDN.SaveAs(outfile);
				outDN.CloseWithUpdate();
				
				std::string addlog("CyPhy model \"<A HREF=\""+outfile+ "\">"+outfile+"\"</A> generated for "+appendObjLink(cwc));
				GMEConsole::Console::Out::writeLine(addlog);
			}			
		}
		else
		{
			if (param != GME_SILENT_MODE)
			{
				prgDlg.SetRange(25*idCnt);
			}
			
			set<CyPhyML::ComponentAssemblies>::iterator cafdr_it;
			CyPhyML::ComponentAssemblies ca_rootFdr;
			set<CyPhyML::ComponentAssemblies> ca_fdrs = cyphy_rf.ComponentAssemblies_kind_children();
			for(cafdr_it=ca_fdrs.begin();cafdr_it!=ca_fdrs.end();++cafdr_it)
			{
				CyPhyML::ComponentAssemblies ca_fdr = *cafdr_it;
				if((std::string)ca_fdr.name()=="Generated configurations")
				{
					ca_rootFdr = ca_fdr;
					break;
				}
			}
			if(ca_rootFdr==Udm::null)
			{
				ca_rootFdr = CyPhyML::ComponentAssemblies::Create(cyphy_rf);
				ca_rootFdr.name() = "Generated configurations";
			}
		
			CyPhyML::ComponentAssemblies ca_folder_dc;
			std::string rootDC_name = rootDC.name();
			set<CyPhyML::ComponentAssemblies> ca_fdr_fdrs = ca_rootFdr.ComponentAssemblies_kind_children();
			for(cafdr_it=ca_fdr_fdrs.begin();cafdr_it!=ca_fdr_fdrs.end();++cafdr_it)
			{
				CyPhyML::ComponentAssemblies ca_fdr = *cafdr_it;
				if((std::string)ca_fdr.name()==rootDC_name)
				{
					ca_folder_dc = ca_fdr;
					break;
				}
			}
			if(ca_folder_dc==Udm::null)
			{
				ca_folder_dc = CyPhyML::ComponentAssemblies::Create(ca_rootFdr);
				ca_folder_dc.name() = rootDC_name;
			}

			CyPhyML::ComponentAssemblies ca_folder = CyPhyML::ComponentAssemblies::Create(ca_folder_dc);
			ca_folder.name() = "Configurations ("+start_time+")";

			int txCount = 0;
			for(set<CyPhyML::CWC>::iterator i=cwcs.begin();i!=cwcs.end();++i)
			{
				if (idCnt > 40 && txCount++ == 20)
				{
					txCount = 0;
					COMTHROW(mgaProject->CommitTransaction());
					COMTHROW(mgaProject->FlushUndoQueue());
					CComPtr<IMgaTerritory> terr;
					COMTHROW(mgaProject->BeginTransactionInNewTerr(TRANSACTION_GENERAL, &terr));
				}
				CyPhyML::CWC cwc = *i;
				std::string msg ="create ComponentAssembly for:"+(std::string)cwc.name()+"...";
				if (param != GME_SILENT_MODE)
				{
					prgDlg.SetProgress(CString(msg.c_str()));
				}
				try
				{
					CyPhyCAExporter dexport(ca_folder, cwc, flatten);
					dexport.showGui = param != GME_SILENT_MODE;
					dexport.createComponentAssembly();
					std::string addlog("ComponentAssembly "+appendObjLink(dexport.getComponentAssembly())+"  generated for "+appendObjLink(cwc));
					GMEConsole::Console::Out::writeLine(addlog);
					traceability.Attach(new CyPhyElaborateTraceability(std::unique_ptr<std::map<Udm::Object, Udm::Object> >(std::move(dexport.copiedObjectsToOriginals))));

								
//#ifdef _DEBUG
//					std::string end_time = getTimeNow();
//					coms.name() = "Configurations ("+start_time+"-"+end_time+")";
//#else
//					coms.name() = "Configurations ("+start_time+")";
//#endif
				}
				catch(udm_exception &exc)
				{
					ca_folder.DeleteObject();
				//	coms.DeleteObject();
					std::string addlog("ComponentAssembly for "+appendObjLink(cwc)+" has failed: "+(std::string)exc.what());
					GMEConsole::Console::Out::writeLine(addlog);
				}
				catch(...)
				{
					std::string addlog("ComponentAssembly for "+appendObjLink(cwc)+" has failed.");
					GMEConsole::Console::Out::writeLine(addlog);
				}				
			}

		}
		if (param != GME_SILENT_MODE)
		{
			prgDlg.OnFinished();
		}
	}
	catch(udm_exception &exc)
	{
		AfxMessageBox(CString(exc.what()));
		throw exc;
	}
	catch(...)
	{
		throw;
	}
}