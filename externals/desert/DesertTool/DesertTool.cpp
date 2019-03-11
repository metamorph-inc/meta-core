// DesertTool.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "DesertTool.h"

//UDM stuff
#include "UdmStatic.h"
#include "UdmBase.h"
#include "UdmDom.h"
#include "Uml.h"
#include "ErrHand.h"

//DesertIface stuff
#include "..\iface\DesertIface.h"
#include "common\Error.h"
using namespace DesertIface;
#include "DesMap.h"

#include <boost/crc.hpp>  // for boost::crc_32_type
#include <sstream>

#include <boost/dynamic_bitset.hpp>

//we need MFC CList....
#include <afxtempl.h>
//DesertIfaceBack stuff
#include "..\iface\DesertIfaceBack.h"
#include "..\DesertDll\BackIface.h"

#include "DesBackMap.h"

//desert dll stuff
#include "..\desertdll\desertdll.h"


//status dlg
#include <afxcmn.h>
#include "StatusDlg.h"

//splash screen
#include "splash.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void silentModeLogger(const TCHAR *msg, const TCHAR *loc, int level) {
	_ftprintf(stderr, _T("<%s>: %s\n"), loc, msg);
}


/////////////////////////////////////////////////////////////////////////////
// CDesertToolApp

BEGIN_MESSAGE_MAP(CDesertToolApp, CWinApp)
	//{{AFX_MSG_MAP(CDesertToolApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDesertToolApp construction

CDesertToolApp::CDesertToolApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CDesertToolApp object

CDesertToolApp theApp;
static int argc = 0;

/////////////////////////////////////////////////////////////////////////////
// CDesertToolApp initialization

tstring stripQuotes( const tstring& expr) {
	if ( expr.empty())
		return expr;
	tstring copyexpr= expr;
	if ( copyexpr[ 0] == '\"')
		copyexpr.erase(0, 1);
	if ( copyexpr.empty())
		return copyexpr;
	if ( copyexpr[ copyexpr.length()-1] == '\"')
		copyexpr.erase( copyexpr.length()-1, 1);
	return copyexpr;
}

tstring trimSpaces(tstring inStr)
{
	tstring outStr = inStr;
    tstringstream trimmer;
    trimmer << outStr;
    outStr.clear();
    trimmer >> outStr;
	return outStr;
}

std::vector<tstring> splitConstraints(tstring _allCons)
{
    tstring allCons = trimSpaces(_allCons);
    
	std::vector<tstring> splittedCons;
	std::size_t lastColonPos = 0;
	std::size_t nextColonPos = allCons.find(_T(":"));
	while(nextColonPos != std::wstring::npos) {
		splittedCons.push_back(allCons.substr(lastColonPos,nextColonPos-lastColonPos));

		lastColonPos = nextColonPos + 1;
		nextColonPos = allCons.find(_T(":"), lastColonPos);
	}
	if(!allCons.empty()) {
	    // Add the last one
    	splittedCons.push_back(allCons.substr(lastColonPos));
	}

	return splittedCons;
}

void ReInitializeManager(DesertSystem* ds, UdmDesertMap* des_map, DesertUdmMap * inv_des_map, std::function<void(short)> UpdateStatus)
{
		des_map->clear();
		inv_des_map->clear();
	//	desertlogFile = (std::string)ds.SystemName()+"_desert.log";
		DesertInit(CString(((string) ds->SystemName()).c_str()));
		set<Space> spaces = ds->Space_kind_children();
		set<Space>::iterator sp_iterator;

		ASSERT(spaces.begin() != spaces.end());
		// BackIfaceFunctions::ClearMap();

		UdmElementSet elements;
		UdmMemberSet custom_members;

		//Build spaces
		if (!spaces.empty())
		{
			Space sp;
			DesertIface::Element dummy;
		
			for (sp_iterator = spaces.begin(); sp_iterator != spaces.end(); sp_iterator ++)
			{
				Space sp = *(sp_iterator);
				//TRACE("Creating space: %s\n", ((string)sp.name()).c_str());
				CreateDesertSpace(sp, dummy, *des_map, *inv_des_map, elements, true, UpdateStatus);
			}//eo for (sp_iterator)
		}//if (!spaces.empty())
	
		//create relations			
		CreateElementRelations(*ds, *des_map, *inv_des_map, UpdateStatus);

		//create constrains
		CreateConstraints(*ds, *des_map, *inv_des_map);

		//create natural domains
		CreateNaturalDomains(*ds, *des_map, *inv_des_map, UpdateStatus);

		//create custom domains
		CreateCustomDomains(*ds, *des_map, *inv_des_map, custom_members, UpdateStatus);

		//create custom domain relations
		CreateMemberRelations(*ds, *des_map, *inv_des_map, UpdateStatus);

		//create variable properties
		CreateVariableProperties(*des_map, *inv_des_map, elements, UpdateStatus);

		//create constant properties
		CreateConstantProperties(*des_map, *inv_des_map, elements, custom_members, UpdateStatus);
		
		CreateSimpleFormulas(*ds,*des_map,*inv_des_map);
		//create assignments for VariableProperties
		CreateAssignments(*des_map, *inv_des_map, elements, custom_members, UpdateStatus);
}

int CDesertToolApp::ExitInstance()
{
	__super::ExitInstance();
	return returnCode;
}

BOOL CDesertToolApp::InitInstance()
{
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

	//this hack is offered gratiously by our very talented grad. student, Tihamer Levendovszky)

	SetRegistryKey(_T("ISIS"));
	CString str(m_pszHelpFilePath);
	WriteProfileString(_T("Data"),_T("Path"), str.Left(str.GetLength()-3) +CString(_T("exe")));

	//end of the hack offered by our very gifted, talented, and I could continue... grad student

	//check for command line
	//if an argument is received, and actually ends up with stg. like
	// "xml", "mga", or "mem", we will use that one.
	bool command_arg_ok = false;
	bool cancel_input = false;
	bool cancel_output = false;
	bool isSilent= false;
	uint64_t maxConfigs = -1;
	bool multiRun = false;
	bool countMode = false;
	CString consList;
	TCHAR** consGroupNames = NULL;
	TCHAR** consGroups = NULL;
	int numConsGroups = 0;
	TCHAR input_file[_MAX_PATH];
	TCHAR output_file[_MAX_PATH];
	tstring desert_config_info_file = _T("");
	FILE* fdDcif = 0;
	
	static TCHAR szFilterUDM[] = _T("XML Backend Files (*.xml)|*.xml|MEM Backend Files (*.mem)|*.mem|MGA Backend Files (*.mga)|*.mga|All Files (*.*)|*.*||");
	
	bool hasInputFile = false;
	bool hasOutputFile = false;
	
	if (m_lpCmdLine)
	{
		if (_tcslen(m_lpCmdLine))
		{
			if (_tcsstr(m_lpCmdLine, _T("/?")) || _tcsstr(m_lpCmdLine, _T("/help")) || _tcsstr(m_lpCmdLine, _T("--help")) || _tcsstr(m_lpCmdLine, _T("/h")))
			{
				CString usage(_T("Usage: deserttool.exe [<DesertInput xml>] [/o <Output Filename>] [/M <maximum configs>] [/c \"constraint1:constraint2\"] [/m \"__CG__GroupA:constraint1:constraint2:__CG__GroupB:constraint1:constraint3\"\r\n\r\nArgument details:\r\n"));
				usage.Append(_T("DesertInput xml: input desert xml file\r\n"));
				usage.Append(_T("/c \"constraint1 : constraint2\": apply the constraint list directly without GUI shown up\r\n"));
				usage.Append(_T("/c \"applyAll\": apply all constraints directly without GUI shown up\r\n"));
				usage.Append(_T("/c \"none\": does not apply any constraint and computes total no. of configurations, without GUI shown up\r\n"));
				usage.Append(_T("/m : when used, calls desert process for \"none\", \"applyAll\", and all given constraint groups (with names using '__CG__' prefix) one by one, without GUI shown up\r\n"));
				usage.Append(_T("/C : Outputs the number of times an element is used in all valid configurations\r\n"));
				usage.Append(_T("/M number : maximum number of configs to write\r\n"));
				std::cout << usage;
				AfxMessageBox(usage,MB_ICONINFORMATION);
				returnCode = 1;
				return TRUE;
			}
			if (_tcsstr(m_lpCmdLine, _T(".xml")) || _tcsstr(m_lpCmdLine, _T(".mem")) || _tcsstr(m_lpCmdLine, _T(".mga"))  
				|| _tcsstr(m_lpCmdLine, _T(".XML")) || _tcsstr(m_lpCmdLine, _T(".MEM")) || _tcsstr(m_lpCmdLine, _T(".MGA")))
			{
				CWzdCommandLineInfo cmdInfo;
				try
				{
					ParseCommandLine(cmdInfo);
				}
				catch (const std::exception& e)
				{
					fprintf(stderr, "%s\n", e.what());
					returnCode = 2;
					return FALSE;
				}

				//seems to be a valid file name
				command_arg_ok = true;
				_tcscpy(input_file, cmdInfo.desert_file);				
				hasInputFile = !(cmdInfo.desert_file).IsEmpty();

				_tcscpy(output_file, cmdInfo.desert_output_file);
				hasOutputFile = !(cmdInfo.desert_output_file).IsEmpty();
				
				isSilent = cmdInfo.silent;
				maxConfigs = cmdInfo.maxConfigs;
				consList = cmdInfo.consList;

				if (cmdInfo.countMode) {
					countMode = true;
					isSilent = true;
				}
				else if (cmdInfo.multiRun) {
					int ncgs = cmdInfo.consGroupNames.GetCount();
					numConsGroups = ncgs;
					if(ncgs > 0) {
						consGroupNames = new TCHAR* [ncgs];
						consGroups = new TCHAR* [ncgs];
					}
					POSITION p1 = cmdInfo.consGroupNames.GetHeadPosition();
					int cIdx = 0;
					while (p1 != NULL) {
						CString s = cmdInfo.consGroupNames.GetNext(p1);
						consGroupNames[cIdx++] = _tcsdup(s);
					}

					POSITION p2 = cmdInfo.consGroups.GetHeadPosition();
					cIdx = 0;
					while (p2 != NULL) {
						CString s = cmdInfo.consGroups.GetNext(p2);
						consGroups[cIdx++] = _tcsdup(s);
					}

					multiRun = true;
					isSilent = true;
				}
			}
		}//eo if (_tcslen(m_lpCmdLine))
	}//eo if (m_lpCmdLine)

	if (isSilent) {
		SetLoggerHandler(silentModeLogger);
	}
	if (!command_arg_ok && !hasInputFile)
	{
		CFileDialog Open(	TRUE,									//construct a file open dialog
							_T("xml"),									//default file name extension
							NULL,									//default file name
							OFN_FILEMUSTEXIST |OFN_HIDEREADONLY ,	//default behaviour
							szFilterUDM,							//no filter
							NULL									//no parent windows
							);
		if (Open.DoModal() == IDOK)
		{
			_tcscpy(input_file, Open.GetPathName());
		} 
		else 
		{
			cancel_input = true;
			returnCode = 0;
			return TRUE;
		}
	}
	
	tstring infile= stripQuotes( input_file);
	infile.copy( input_file, infile.length(), 0);
	input_file[ infile.length()]= 0;

	// If output_file name is given, use it, else tack an _back at the end
	// (i.e., output has the same name, but sufxed by _back.[xml|mga|mem])
	if(hasOutputFile) {
		tstring outFile = stripQuotes( output_file );
		outFile.copy( output_file, outFile.length(), 0);
		output_file[ outFile.length() ] = 0;

		tstring strOutFName = output_file;
		std::size_t dotPos = strOutFName.find_last_of('.');
		if(dotPos != std::string::npos) {
			desert_config_info_file = strOutFName.substr(0, dotPos) + _T("_configs.xml");
		} else {
			desert_config_info_file = strOutFName + _T("_configs.xml");
		}
		fdDcif = _tfopen(desert_config_info_file.c_str(), _T("w+"));
	}
	else {
		_tcsncpy(output_file, input_file, _tcslen(input_file)-4);
		*(output_file+_tcslen(input_file)-4)='\0';

		desert_config_info_file = output_file;
		desert_config_info_file += _T("_configs.xml");
		fdDcif = _tfopen(desert_config_info_file.c_str(), _T("w+"));
	
		if (_tcsstr(input_file, _T(".xml")) || _tcsstr(input_file, _T(".XML"))) _tcscat(output_file, _T("_back.xml"));
		if (_tcsstr(input_file, _T(".mem")) || _tcsstr(input_file, _T(".MEM"))) _tcscat(output_file, _T("_back.mem"));
		if (_tcsstr(input_file, _T(".mga")) || _tcsstr(input_file, _T(".MGA"))) _tcscat(output_file, _T("_back.mga"));
	}
	if (fdDcif == NULL) {
		char buf[512];
		strerror_s(buf, errno);
		std::cerr << "Could not open '" << desert_config_info_file.c_str() << "': " << buf << std::endl;
		returnCode = 5;
		return FALSE;
	}

	if (!cancel_input)
	{
		CStatusDlg s_dlg( 0, isSilent);
		if (!isSilent) {
			s_dlg.Create(IDD_DIALOG1);
		}
		CStatusDlg* st_dlg = &s_dlg;

		
		DesertSystem ds;
		Udm::SmartDataNetwork nw(diagram);
		try
		{

			s_dlg.SetStatus(SD_INIT);
			s_dlg.SetStatus(SD_PARSE);

			nw.OpenExisting(std::string(CStringA(input_file)), "", Udm::CHANGES_LOST_DEFAULT);
			ds = DesertSystem::Cast(nw.GetRootObject());
			DesertInit(CString(((std::string)ds.SystemName()).c_str()));
			
			s_dlg.SetStatus(SD_SPS);
			//spaces, elements
			set<Space> spaces = ds.Space_kind_children();
			set<Space>::iterator sp_iterator;

			ASSERT(spaces.begin() != spaces.end());	
			
			UdmDesertMap des_map;
			DesertUdmMap inv_des_map;


			UdmElementSet elements;
			UdmMemberSet custom_members;
						
			std::function<void(short)> UpdateStatus = [&st_dlg](short status) { st_dlg->StepInState(status); };
	
			//Build spaces
			if (!spaces.empty())
			{
				Space sp;
				Element dummy;
				
				for (sp_iterator = spaces.begin(); sp_iterator != spaces.end(); sp_iterator++)
				{
					Space sp = *(sp_iterator);
					TRACE("Creating space: %s\n", ((string)sp.name()).c_str());
					CreateDesertSpace(sp, dummy, des_map, inv_des_map, elements, true, UpdateStatus);
				}//eo for (sp_iterator)
			}//if (!spaces.empty())
			
			//create relations
			s_dlg.SetStatus(SD_ERS);
			CreateElementRelations(ds, des_map, inv_des_map, UpdateStatus);

			//create constrains
			s_dlg.SetStatus(SD_CTS);
			bool success = CreateConstraints(ds, des_map, inv_des_map);
			if (!success && isSilent) {
				returnCode = 5;
				return FALSE;
			}

			//create foumulas
			CreateCustomFormulas(ds, des_map, inv_des_map);
			if (!success && isSilent) {
				returnCode = 6;
				return FALSE;
			}

			//create natural domains
			s_dlg.SetStatus(SD_NDS);
			CreateNaturalDomains(ds, des_map, inv_des_map, UpdateStatus);

			//create custom domains
			s_dlg.SetStatus(SD_CDS);
			CreateCustomDomains(ds, des_map, inv_des_map, custom_members, UpdateStatus);

			//create custom domain relations
			s_dlg.SetStatus(SD_MRS);
			CreateMemberRelations(ds, des_map, inv_des_map, UpdateStatus);

			//create variable properties
			s_dlg.SetStatus(SD_VPS);
			CreateVariableProperties(des_map, inv_des_map, elements, UpdateStatus);

			//create constant properties
			s_dlg.SetStatus(SD_CPS);
			CreateConstantProperties(des_map, inv_des_map, elements, custom_members, UpdateStatus);
	
			//create simpleformula for properties
			CreateSimpleFormulas(ds, des_map, inv_des_map);
			//create assignments for VariableProperties
			s_dlg.SetStatus(SD_ASS);
			CreateAssignments(des_map, inv_des_map, elements, custom_members, UpdateStatus);
	
			//invoking Desert UI
			s_dlg.SetStatus(SD_GUI);

			using namespace BackIfaceFunctions;
		
			fprintf(fdDcif, "<?xml version=\"1.0\"?>\n");
			fprintf(fdDcif, "<DesertConfigurations>\n");

			if (countMode) {
				std::map<__int64, __int64> counts;
				int numCfgs = 0;
				auto cb = [&](const BackIfaceFunctions::DBConfiguration& config) {
					numCfgs++;
					POSITION pos2 = config.alt_assignments.GetStartPosition();
					while (pos2)
					{
						long alt, alt_of;
						config.alt_assignments.GetNextAssoc(pos2, alt_of, alt);

						// auto alt_of_input = inv_des_map.find(alt_of);
						auto alt_input = inv_des_map.find(alt);
						// fprintf(fdDcif, "%s\n", static_cast<const std::string>(alt_of_input->second.name()).c_str());
						// fprintf(fdDcif, "%s\n", static_cast<const std::string>(alt_input->second.name()).c_str());
						// std::string type = alt_input->second.type().name();
						// auto x = alt_input->second.name();
						// auto x2 = (__int64)alt_input->second.externalID();

						auto countIt = counts.find(alt_input->second.id());
						if (countIt == counts.end()) {
							counts[alt_input->second.id()] = 1;
						}
						else {
							counts[alt_input->second.id()] = countIt->second + 1;
						}
					}
					return 0;
				};
				typedef decltype(cb) cb_t;
				auto cbVoid = [](void* cb_, const BackIfaceFunctions::DBConfiguration& config) {
					return (*((cb_t*)cb_))(config);
				};
				typedef int(*BuildConfigurationsCallbackFunction_t)(void*, const BackIfaceFunctions::DBConfiguration&);
				DBConfigurations * confs = (DBConfigurations *)DesertFinit(true, isSilent, consList == "" ? NULL : static_cast<LPCTSTR>(consList),
					(BuildConfigurationsCallbackFunction_t)cbVoid, &cb);
				fprintf(fdDcif, "  <NumberOfConfigurations count=\"%d\"/>\n", numCfgs);
				for (auto countsIt = counts.begin(); countsIt != counts.end(); ++countsIt) {
					fprintf(fdDcif, "  <Count id=\"%I64d\" count=\"%I64d\"/>\n", countsIt->first, countsIt->second);
				}
			}
			else if (multiRun) {
				// Initialize
				std::cout << "Starting Multirun" << std::endl;
				DesertFinitWithMultirun_Pre(numConsGroups, consGroupNames, consGroups);

				// First run with no constraints applied
				int numCfgs = DesertFinitWithMultirun_Exec(true, CString(((std::string) ds.SystemName()).c_str()), _T("NONE"), _T(""));
				std::cout << "Group: NONE\tConfigs: " << numCfgs << std::endl;
				fprintf(fdDcif, "\t<None NumConfigs=\"%d\"/>\n", numCfgs);
				ReInitializeManager(&ds, &des_map, &inv_des_map, UpdateStatus);

				// Next run with all constraints applied
				numCfgs = DesertFinitWithMultirun_Exec(true, CString(((std::string) ds.SystemName()).c_str()), _T("ALL"), _T("applyAll"));
				std::cout << "Group: NONE\tConfigs: " << numCfgs << std::endl;
				fprintf(fdDcif, "\t<All NumConfigs=\"%d\"/>\n", numCfgs);
				ReInitializeManager(&ds, &des_map, &inv_des_map, UpdateStatus);

				// Next run for all groups
				fprintf(fdDcif, "\t<Groups>\n");
				for (int j = 0; j < numConsGroups; j++) {
					numCfgs = DesertFinitWithMultirun_Exec(true, CString(((std::string) ds.SystemName()).c_str()), consGroupNames[j], consGroups[j]);

					std::cout << "Group: NONE\tConfigs: " << numCfgs << std::endl;
					fprintf(fdDcif, "\t\t<Group Name=\"" PRINTF_TSTRING "\" NumConfigs=\"%d\">\n", consGroupNames[j], numCfgs);
					std::vector<tstring> splittedCons = splitConstraints(consGroups[j]);
					for (std::vector<tstring>::iterator conIt = splittedCons.begin(); conIt != splittedCons.end(); ++conIt)
						fprintf(fdDcif, "\t\t\t<Constraint Name=\"" PRINTF_TSTRING "\"/>\n", conIt->c_str());
					fprintf(fdDcif, "\t\t</Group>\n");

					if (j < numConsGroups) {
						ReInitializeManager(&ds, &des_map, &inv_des_map, UpdateStatus);
					}
				}
				fprintf(fdDcif, "\t</Groups>\n");

				// Finalize
				DesertFinitWithMultirun_Post();
				std::cout << "Finished Multirun" << std::endl;
				std::cout.flush();
			}
			else {

				// AfxMessageBox(CString("Calling DESERT Finit w/:") + consList );
				std::cout << "Running with constraint set: \t" << consList << std::endl;
				DBConfigurations * confs = (DBConfigurations *)DesertFinit(true, isSilent, consList == "" ? NULL : (LPCTSTR)consList);
				int numCfgs = (confs) ? confs->GetCount() : 0;
				
				tstring strConsList = trimSpaces(tstring(consList));
				if (strConsList.length() == 0 || strConsList.compare(_T("None")) == 0 || strConsList.compare(_T("none")) == 0) {
					fprintf(fdDcif, "\t<None NumConfigs=\"%d\"/>\n", numCfgs);
				} else if(strConsList.compare(_T("applyAll")) == 0) {
					fprintf(fdDcif, "\t<All NumConfigs=\"%d\"/>\n", numCfgs);
				} else {
					fprintf(fdDcif, "\t<ConstraintSet NumConfigs=\"%d\">\n", numCfgs);
					std::vector<tstring> allStrCons = splitConstraints(strConsList);
					for (std::vector<tstring>::iterator strCIt = allStrCons.begin(); strCIt != allStrCons.end(); ++strCIt)
						fprintf(fdDcif, "\t\t<Constraint Name=\"" PRINTF_TSTRING "\"/>\n", strCIt->c_str());
					fprintf(fdDcif, "\t</ConstraintSet>\n");
				}
				std::cout << "Configs: " << numCfgs << std::endl;
				std::cout.flush();
			
				if (confs)
				{
					//open file dialog to write out configurations
					if (!command_arg_ok || !isSilent)
					{
						CFileDialog SaveAs(
								FALSE,									//construct a file open dialog
								_T("xml"),									//default file name extension
								output_file,							//default file name
								OFN_HIDEREADONLY ,						//default behaviour
								szFilterUDM,							//no filter
								NULL									//no parent windows
								);
						if (SaveAs.DoModal() == IDOK)
						{
							_tcscpy(output_file, SaveAs.GetPathName());
						}
							else cancel_output = true;

					}
					if (!cancel_output)
					{
				
						//create data network
						Udm::SmartDataNetwork bw(DesertIfaceBack::diagram);
						DesertIfaceBack::DesertBackSystem dbs;
						bw.CreateNew(
							//(LPCTSTR)SaveAs.GetFileName(), 
							std::string(CStringA(output_file)),
							"DesertIfaceBack",	
							DesertIfaceBack::DesertBackSystem::meta,
							Udm::CHANGES_PERSIST_ALWAYS);	

						//get&take care of the root object
						dbs = DesertIfaceBack::DesertBackSystem::Cast(bw.GetRootObject());
						dbs.SystemName() = ds.SystemName();

						//elements, custommembers and natural members will be created
						//on the fly, as it they are participating in an assignment

						int confn = confs->GetCount();
						POSITION pos = confs->GetHeadPosition();
						int count = 0;
						s_dlg.SetStatus(SD_PREP);
						while (pos)
						{
		
							DBConfiguration * config = confs->GetNext(pos);
							// config->id starts at 1
							if (config && config->id - 1 >= maxConfigs)
							{
								break;
							}
							if (config)
							{
								DesertIfaceBack::Configuration dummy;
								//check for the existence of elements, properties,
								//and values which will be associated then
								POSITION pos1 = config->assignments.GetHeadPosition();
								while (pos1)
								{
									long valid_ass = config->assignments.GetNext(pos1);				
									BackIfaceFunctions::CreatePropertyAssignment(dbs, dummy, inv_des_map, des_map, valid_ass, false);
								}
								POSITION pos2 = config->alt_assignments.GetStartPosition();
								while (pos2)
								{
									long alt, alt_of;
									config->alt_assignments.GetNextAssoc(pos2, alt_of, alt);
									BackIfaceFunctions::CreateAlternativeAssignment(dbs, dummy, inv_des_map, alt_of, alt, false);

								};
							}//eo if (config)	
							s_dlg.StepInState((double)(((double)(++count)) / ((double)(confs->GetCount())))*100.0);	//progress bar

						}//eo while(pos)
						pos = confs->GetHeadPosition();
						s_dlg.SetStatus(SD_BACK);
						count = 0;
						while (pos)
						{
							DBConfiguration * config = confs->GetNext(pos);
							// config->id starts at 1
							if (config && config->id - 1 >= maxConfigs)
							{
								break;
							}
							if (config)
							{
								//create configuration
								DesertIfaceBack::Configuration dib_conf = DesertIfaceBack::Configuration::Create(dbs);
								CString s;
								s.Format(_T("Conf. no: %d"), config->id);
								dib_conf.name() = static_cast<LPCSTR>(CStringA(s));
								dib_conf.id() = config->id;
							

								POSITION pos1 = config->assignments.GetHeadPosition();
								while (pos1)
								{
									long valid_ass = config->assignments.GetNext(pos1);				
									BackIfaceFunctions::CreatePropertyAssignment(dbs, dib_conf, inv_des_map, des_map, valid_ass, true);
								}
								POSITION pos2 = config->alt_assignments.GetStartPosition();
								while (pos2)
								{
									long alt, alt_of;
									config->alt_assignments.GetNextAssoc(pos2, alt_of, alt);
									BackIfaceFunctions::CreateAlternativeAssignment(dbs, dib_conf, inv_des_map, alt_of, alt, true);
								};
							
								TRACE("Got %d of configurations!\n", confs->GetCount());


								delete config;
							}//eo if (config)
							s_dlg.StepInState((double)(((double)(++count)) / ((double)(confs->GetCount())))*100.0);	//progress bar
						}//eo while(pos)
					
						delete confs;
			
						set<DesertIfaceBack::Configuration> cfgs = dbs.Configuration_kind_children();		
						for (set<DesertIfaceBack::Configuration>::iterator it = cfgs.begin(); it != cfgs.end(); ++it)
						{
							DesertIfaceBack::Configuration cfg = *it;
							std::string cfgid = cfg.cfgId();
							boost::crc_32_type id_hash;
							id_hash.process_bytes(cfgid.c_str(), cfgid.length());
							int uid = id_hash.checksum();
							std::stringstream sstream;
							sstream << std::hex << uid;
							cfg.cfgId() = sstream.str();
						}
					}//eo if DoModal() == IDOK
				}//eo if (configs)
				else if (isSilent) {
					Udm::SmartDataNetwork bw(DesertIfaceBack::diagram);
					DesertIfaceBack::DesertBackSystem dbs;
					bw.CreateNew(
						//(LPCTSTR)SaveAs.GetFileName(),
						std::string(CStringA(output_file)),
						"DesertIfaceBack",
						DesertIfaceBack::DesertBackSystem::meta,
						Udm::CHANGES_LOST_DEFAULT);
					bw.SaveAs(std::string(CStringA(output_file)));
				}

				//all done
				s_dlg.SetStatus(SD_FINIT);
			
				/*{
					//debug test
					set<ConstraintSet> ts_set = ds.ConstraintSet_kind_children();
				
					set<ConstraintSet>::iterator i = ts_set.begin();
					ConstraintSet cts = *i;
				
					Constraint ct = Constraint::Create(cts);
				
					ct.id() = 1234567890;
					ct.externalID() = -1234567890;
					ct.name() = "fuck name";
					ct.expression() = "fuck expression";
				

					set<Space> sp_set = ds.Space_kind_children();
					set<Space> :: iterator j = sp_set.begin();
					Space sp = *j;
				
					set<Element> e_set = sp.Element_kind_children();
					set<Element> :: iterator k = e_set.begin();
					Element e = *k;
				
					ct.context() = e;
				


				}*/

			} // eo multiRun

			fprintf(fdDcif, "</DesertConfigurations>\n");
			fclose(fdDcif);

		}//eo try
		// catch (CMemoryException *e)
		catch (CSimpleException *e)
		{
			if (fdDcif)
			{
				if (ftell(fdDcif))
				{
					fprintf(fdDcif, "</DesertConfigurations>\n");
				}
				fclose(fdDcif);
			}
			if (isSilent) {
				TCHAR errorMessage[1024];
				e->GetErrorMessage(errorMessage, _countof(errorMessage), 0);
				_ftprintf(stderr, _T("%s\n"), errorMessage);
			}
			else {
				e->ReportError();
			}
			//	throw e;
			returnCode = 1;
			return FALSE;
		}
		catch (CDesertException *e)
		{
			if (fdDcif)
			{
				if (ftell(fdDcif))
				{
					fprintf(fdDcif, "</DesertConfigurations>\n");
				}
				fclose(fdDcif);
			}
			if (isSilent) {
				_ftprintf(stderr, _T("%s\n"), static_cast<const TCHAR*>(e->GetErrorMessage()));
			}
			else {
				e->ReportError();
			}
			//	throw e;
			returnCode = 1;
			return FALSE;
		}
		catch (const udm_exception& e)
		{
			if (fdDcif)
			{
				if (ftell(fdDcif))
				{
					fprintf(fdDcif, "</DesertConfigurations>\n");
				}
				fclose(fdDcif);
			}
			if (m_lpCmdLine && command_arg_ok)
				// FIXME: we are /SUBSYSTEM:WINDOWS; this likely won't go anywhere
				std::cerr << std::string("Udm exception: ") + e.what() << std::endl;
			else
				AfxMessageBox(CString("Udm exception: ") + CString(e.what()));
			//	throw e;
			returnCode = 2;
			return FALSE;
		}
	
	}//eo if (Open.DoModal())

	returnCode = 0;
	if (isSilent) {
		return FALSE;
	}
	else {
		return TRUE;
	}
}

///////////////////////////////////////////////////////////////
// CWzdCommandLineInfo
CWzdCommandLineInfo::CWzdCommandLineInfo():
    desert_file (""), outputFileNeedsToBeRead(false), desert_output_file(""),
	silent(false), constraintListNeedsToBeRead(false), consList(""), multiRun(false),
	maxConfigs(-1), maxConfigsNeedsToBeRead(false), countMode(false)
{
}

CWzdCommandLineInfo::~CWzdCommandLineInfo()
{
}

///////////////////////////////////////////////////////////////
void CWzdCommandLineInfo::ParseParam(const TCHAR* pszParam, BOOL bFlag,
    BOOL bLast)
{
	argc++;
    CString sArg(pszParam);

	int n = _ttoi((LPCTSTR)sArg);
	if(!outputFileNeedsToBeRead && !maxConfigsNeedsToBeRead &&
		(sArg.Right(4) == ".xml" || sArg.Right(4) == ".mem" || sArg.Right(4) == ".mga"
			|| sArg.Right(4) == ".XML" || sArg.Right(4) == ".MEM" || sArg.Right(4) == ".MGA")
	  )
	{
		desert_file = sArg;
	}
	else if (maxConfigsNeedsToBeRead)
	{
		TCHAR* endPtr;
		maxConfigs = _tcstoull(static_cast<const TCHAR*>(sArg),
			&endPtr, 10);
		maxConfigsNeedsToBeRead = false;
	}
	else if (sArg == "M")
	{
		maxConfigsNeedsToBeRead = true;
		if (bLast) {
			throw runtime_error("/M requires an argument");
		}
	}
	else if(sArg=="o")
	{
		outputFileNeedsToBeRead = true;
		if (bLast) {
			throw runtime_error("/o requires an argument");
		}
	}
	else if(outputFileNeedsToBeRead)
	{
		desert_output_file = sArg;
		outputFileNeedsToBeRead = false;
	}
	else if (sArg == "s")
	{
		silent = true;
	}
	else if(sArg=="c")
	{
		silent = true;
		constraintListNeedsToBeRead = true;
		if (bLast) {
			throw runtime_error("/c requires an argument");
		}
	}
	else if(constraintListNeedsToBeRead)
	{
		consList = sArg;
		constraintListNeedsToBeRead = false;
	}
	else if (sArg == "C")
	{
		silent = true;
		countMode = true;
	}
	else if(sArg=="m")
	{
		silent = true;
		multiRun = true;
		if (bLast) {
			throw runtime_error("/m requires an argument");
		}
	}
	else if(multiRun)
	{
		const TCHAR *ccstrConsGroups = (LPCTSTR) sArg;
		tstring strConsGroups = ccstrConsGroups;
		bool consGrpSpecError = false;

		tstring groupName = _T("");
		tstring constraintGroup = _T("");
		std::size_t posPrevGrpStart = strConsGroups.find(_T("__CG__"));
		std::size_t posPrevGrpEnd = 0;
		if(posPrevGrpStart != tstring::npos) {
			posPrevGrpEnd = strConsGroups.find(_T(":"));
			if(posPrevGrpEnd != tstring::npos) {
				groupName = strConsGroups.substr(posPrevGrpStart + 6, posPrevGrpEnd - posPrevGrpStart - 6);
			} else {
				consGrpSpecError = true;
			}
		}
		std::size_t posNextGrpStart = strConsGroups.find(_T("__CG__"), posPrevGrpEnd);
		while(!consGrpSpecError && posNextGrpStart != tstring::npos) {
			constraintGroup = strConsGroups.substr(posPrevGrpEnd + 1, posNextGrpStart - 1 - posPrevGrpEnd - 1);
			CString gn = groupName.c_str();
			consGroupNames.AddTail(gn);
			CString cg = constraintGroup.c_str();
			consGroups.AddTail(cg);

			std::size_t posNextGrpEnd = strConsGroups.find(_T(":"), posNextGrpStart);
			if(posNextGrpEnd != string::npos) {
				groupName = strConsGroups.substr(posNextGrpStart + 6, posNextGrpEnd - posNextGrpStart - 6);
				posPrevGrpStart = posNextGrpStart;
				posPrevGrpEnd = posNextGrpEnd;
				posNextGrpStart = strConsGroups.find(_T("__CG__"), posNextGrpEnd);
			} else {
				consGrpSpecError = true;
			}
		}
		if(!consGrpSpecError && posPrevGrpStart != string::npos && posNextGrpStart == string::npos) {
			// Add the last constraint group
			constraintGroup = strConsGroups.substr(posPrevGrpEnd + 1);
			CString gn = groupName.c_str();
			consGroupNames.AddTail(gn);
			CString cg = constraintGroup.c_str();
			consGroups.AddTail(cg);
		}
	}
    CCommandLineInfo::ParseParam(pszParam,bFlag,bLast);
}
/////////////////////////////////////////////////////////////////////////////