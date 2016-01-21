// UdmApp.cpp: implementation of the CUdmApp class.
// This file was automatically generated as UdmApp.cpp
// by UDM Interpreter Wizard on Thursday, March 27, 2008 10:37:31
#include "stdafx.h"
#include "UdmApp.h"
#include "UdmConfig.h"
#include "Uml.h"

#include "UdmConsole.h"
#include "time.h"
#include "UdmBase.h"

#include "UmlExt.h"
#include "UdmStatic.h"

#include "CyPhyDSRefiner.h"


using namespace Uml;
using namespace Udm;

using namespace CyPhyML;

CString CUdmApp::mgaPath = "";

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
	try{
		//1. get the set of configurations for refiner
		if(focusObject==Udm::null){
			AfxMessageBox(L"Please open a Configurations model to invoke the interpreter.");
			return;
		}

		if(!Uml::IsDerivedFrom(focusObject.type(), CyPhyML::Configurations::meta)){
			AfxMessageBox(L"Please open a Configurations model to invoke the interpreter.");
			return;
		}
		
		CyPhyML::Configurations cfgs = CyPhyML::Configurations::Cast(focusObject);
		set<CyPhyML::CWC> cwcs;
		if(selectedObjects.empty())
		{
			cwcs = CyPhyML::Configurations::Cast(focusObject).CWC_kind_children();
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
			}
		}

		if(cwcs.empty())
		{
			AfxMessageBox(L"There is no CWC model selected.");
			return;
		}
		//2. run the refiner
		CyPhyDSRefiner refiner(cfgs, cwcs);
		refiner.startProgressBar();
		refiner.runDSRefiner();
		CyPhyML::DesignContainer refinedDS = refiner.getRefinedDS();
		std::string addlog("CyPhyDSRefiner for "+appendObjLink(cfgs.parent())+" is created: "+appendObjLink(refinedDS));
		GMEConsole::Console::Out::writeLine(addlog);
		refiner.endProgressBar();
		std::string msg("Refined DesignSpace: "+ (std::string)refinedDS.name() + " is created for "+ UdmUtil::ExtractName(cfgs.parent())+".");
		MessageBox(NULL, CString(msg.c_str()), L"CyPhyDSRefiner", MB_ICONINFORMATION);
	}
	catch(udm_exception &exc)
	{
		AfxMessageBox(CString(exc.what()));
		std::string addlog("CyPhyDSRefiner for "+appendObjLink(focusObject)+" is failed: "+(std::string)exc.what());
		GMEConsole::Console::Out::writeLine(addlog);
		throw exc;
	}
	catch(...)
	{
		throw;
	}
}