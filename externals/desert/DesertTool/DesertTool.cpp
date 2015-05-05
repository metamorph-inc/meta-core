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
using namespace DesertIface;
#include "DesMap.h"

#include <boost/crc.hpp>  // for boost::crc_32_type
#include <sstream>

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

std::string stripQuotes( const std::string& expr) {
	if ( expr.empty())
		return expr;
	std::string copyexpr= expr;
	if ( copyexpr[ 0] == '\"')
		copyexpr.erase(0, 1);
	if ( copyexpr.empty())
		return copyexpr;
	if ( copyexpr[ copyexpr.length()-1] == '\"')
		copyexpr.erase( copyexpr.length()-1, 1);
	return copyexpr;
}

std::string trimSpaces(std::string inStr)
{
	std::string outStr = inStr;
    std::stringstream trimmer;
    trimmer << outStr;
    outStr.clear();
    trimmer >> outStr;
	return outStr;
}

std::vector<std::string> splitConstraints(std::string _allCons)
{
    std::string allCons = trimSpaces(_allCons);
    
	std::vector<std::string> splittedCons;
	std::size_t lastColonPos = 0;
	std::size_t nextColonPos = allCons.find(":");
	while(nextColonPos != std::string::npos) {
		splittedCons.push_back(allCons.substr(lastColonPos,nextColonPos-lastColonPos));

		lastColonPos = nextColonPos + 1;
		nextColonPos = allCons.find(":", lastColonPos);
	}
	if(!allCons.empty()) {
	    // Add the last one
    	splittedCons.push_back(allCons.substr(lastColonPos));
	}

	return splittedCons;
}

void ReInitializeManager(DesertSystem* ds, UdmDesertMap* des_map, DesertUdmMap * inv_des_map)
{
		des_map->clear();
		inv_des_map->clear();
	//	desertlogFile = (std::string)ds.SystemName()+"_desert.log";
		DesertInit( ((string) ds->SystemName()).c_str());
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
				CreateDesertSpace(sp, dummy, *des_map, *inv_des_map, elements, true);
			}//eo for (sp_iterator)
		}//if (!spaces.empty())
	
		//create relations			
		CreateElementRelations(*ds, *des_map, *inv_des_map);

		//create constrains
		CreateConstraints(*ds, *des_map, *inv_des_map);

		//create natural domains
		CreateNaturalDomains(*ds, *des_map, *inv_des_map);

		//create custom domains
		CreateCustomDomains(*ds, *des_map, *inv_des_map, custom_members);

		//create custom domain relations
		CreateMemberRelations(*ds, *des_map, *inv_des_map);

		//create variable properties
		CreateVariableProperties(*des_map, *inv_des_map, elements);

		//create constant properties
		CreateConstantProperties(*des_map, *inv_des_map, elements, custom_members);
		
		CreateSimpleFormulas(*ds,*des_map,*inv_des_map);
		//create assignments for VariableProperties
		CreateAssignments(*des_map, *inv_des_map, elements, custom_members);
}

BOOL CDesertToolApp::InitInstance()
{
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif


	//this hack is offered gratiously by our very talented grad. student, Tihamer Levendovszky)

	SetRegistryKey("ISIS");
	CString str(m_pszHelpFilePath);
	WriteProfileString("Data","Path", str.Left(str.GetLength()-3) +CString("exe"));

	//end of the hack offered by our very gifted, talented, and I could continue... grad student

	//check for command line
	//if an argument is received, and actually ends up with stg. like
	// "xml", "mga", or "mem", we will use that one.
	bool command_arg_ok = false;
	bool cancel_input = false;
	bool cancel_output = false;
	bool isSilent= false;
	bool multiRun = false;
	CString consList;
	char** consGroupNames = NULL;
	char** consGroups = NULL;
	int numConsGroups = 0;
	char input_file[_MAX_PATH];
	char output_file[_MAX_PATH];
	std::string desert_config_info_file = "";
	FILE* fdDcif = 0;
	
	static char szFilterUDM[] = "XML Backend Files (*.xml)|*.xml|MEM Backend Files (*.mem)|*.mem|MGA Backend Files (*.mga)|*.mga|All Files (*.*)|*.*||";
	
	bool hasInputFile = false;
	bool hasOutputFile = false;
	
	if (m_lpCmdLine)
	{
		if (strlen(m_lpCmdLine))
		{
			if (strstr(m_lpCmdLine, "/?") || strstr(m_lpCmdLine, "/help") || strstr(m_lpCmdLine, "--help") || strstr(m_lpCmdLine, "/h"))
			{
				CString usage("Usage: deserttool.exe [<DesertInput xml>] [/o <Output Filename>] [/c \"constraint1:constraint2\"] [/m \"__CG__GroupA:constraint1:constraint2:__CG__GroupB:constraint1:constraint3\"\r\n\r\nArgument details:\r\n");
				usage.Append("DesertInput xml: input desert xml file\r\n");
				usage.Append("/c \"constraint1 : constraint2\": apply the constraint list directly without GUI shown up\r\n");
				usage.Append("/c \"applyAll\": apply all constraints directly without GUI shown up\r\n");
				usage.Append("/c \"none\": does not apply any constraint and computes total no. of configurations, without GUI shown up\r\n");
				usage.Append("/m : when used calls desert process for \"none\", \"applyAll\", and all given constraint groups (with names using '__CG__' prefix) one by one, without GUI shown up\r\n");
				std::cout << usage;
				AfxMessageBox(usage,MB_ICONINFORMATION);
				return TRUE;
			}
			if (strstr(m_lpCmdLine, ".xml") || strstr(m_lpCmdLine, ".mem") || strstr(m_lpCmdLine, ".mga")  
				|| strstr(m_lpCmdLine, ".XML") || strstr(m_lpCmdLine, ".MEM") || strstr(m_lpCmdLine, ".MGA"))
			{
				CWzdCommandLineInfo cmdInfo;
				ParseCommandLine(cmdInfo);

				//seems to be a valid file name
				command_arg_ok = true;
				strcpy(input_file, cmdInfo.desert_file);				
				hasInputFile = !(cmdInfo.desert_file).IsEmpty();

				strcpy(output_file, cmdInfo.desert_output_file);
				hasOutputFile = !(cmdInfo.desert_output_file).IsEmpty();
				
				isSilent = cmdInfo.silent;
				if (cmdInfo.applyCons)
					consList = cmdInfo.consList;

				if(cmdInfo.multiRun) {
					int ncgs = cmdInfo.consGroupNames.GetCount();
					numConsGroups = ncgs;
					if(ncgs > 0) {
						consGroupNames = new char* [ncgs];
						consGroups = new char* [ncgs];
					}
					POSITION p1 = cmdInfo.consGroupNames.GetHeadPosition();
					int cIdx = 0;
					while (p1 != NULL) {
						CString s = cmdInfo.consGroupNames.GetNext(p1);
						consGroupNames[cIdx++] = strdup(s);
					}

					POSITION p2 = cmdInfo.consGroups.GetHeadPosition();
					cIdx = 0;
					while (p2 != NULL) {
						CString s = cmdInfo.consGroups.GetNext(p2);
						consGroups[cIdx++] = strdup(s);
					}

					multiRun = true;
					isSilent = true;
				}
			}
		}//eo if (strlen(m_lpCmdLine))
	}//eo if (m_lpCmdLine)
	
	if (!command_arg_ok && !hasInputFile)
	{
		CFileDialog Open(	TRUE,									//construct a file open dialog
							"xml",									//default file name extension
							NULL,									//default file name
							OFN_FILEMUSTEXIST |OFN_HIDEREADONLY ,	//default behaviour
							szFilterUDM,							//no filter
							NULL									//no parent windows
							);
		if (Open.DoModal() == IDOK)
		{
			strcpy(input_file, Open.GetPathName());
		} 
		else 
		{
			cancel_input = true;
			return TRUE;
		}
	}
	
	std::string infile= stripQuotes( input_file);
	infile.copy( input_file, infile.length(), 0);
	input_file[ infile.length()]= 0;

	// If output_file name is given, use it, else tack an _back at the end
	// (i.e., output has the same name, but sufxed by _back.[xml|mga|mem])
	if(hasOutputFile) {
		std::string outFile = stripQuotes( output_file );
		outFile.copy( output_file, outFile.length(), 0);
		output_file[ outFile.length() ] = 0;

		std::string strOutFName = output_file;
		std::size_t dotPos = strOutFName.find_last_of('.');
		if(dotPos != std::string::npos) {
			desert_config_info_file = strOutFName.substr(0, dotPos) + "_configs.xml";
		} else {
			desert_config_info_file = strOutFName + "_configs.xml";
		}
		fdDcif = fopen(desert_config_info_file.c_str(), "w+");
	}
	else {
		strncpy(output_file, input_file, strlen(input_file)-4);
		*(output_file+strlen(input_file)-4)='\0';

		desert_config_info_file = output_file;
		desert_config_info_file += "_configs.xml";
		fdDcif = fopen(desert_config_info_file.c_str(), "w+");
	
		if (strstr(input_file, ".xml") || strstr(input_file, ".XML")) strcat(output_file, "_back.xml");
		if (strstr(input_file, ".mem") || strstr(input_file, ".MEM")) strcat(output_file, "_back.mem");
		if (strstr(input_file, ".mga") || strstr(input_file, ".MGA")) strcat(output_file, "_back.mga");
	}

	if (!cancel_input)
	{
		CStatusDlg s_dlg( 0, isSilent);
		s_dlg.Create(IDD_DIALOG1);
		GetStatusDlg(&s_dlg);

		
		DesertSystem ds;
		Udm::SmartDataNetwork nw(diagram);
		try
		{

			s_dlg.SetStatus(SD_INIT);
			s_dlg.SetStatus(SD_PARSE);

			nw.OpenExisting(input_file,"", 	Udm::CHANGES_PERSIST_ALWAYS);
			ds = DesertSystem::Cast(nw.GetRootObject());
			DesertInit( ((string)ds.SystemName()).c_str());
			
			s_dlg.SetStatus(SD_SPS);
			//spaces, elements
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
					TRACE("Creating space: %s\n", ((string)sp.name()).c_str());
					CreateDesertSpace(sp, dummy, des_map, inv_des_map, elements, true);
				}//eo for (sp_iterator)
			}//if (!spaces.empty())
			
			//create relations
			s_dlg.SetStatus(SD_ERS);
			CreateElementRelations(ds, des_map, inv_des_map);

			//create constrains
			s_dlg.SetStatus(SD_CTS);
			CreateConstraints(ds, des_map, inv_des_map);

			//create foumulas
			CreateCustomFormulas(ds, des_map, inv_des_map);

			//create natural domains
			s_dlg.SetStatus(SD_NDS);
			CreateNaturalDomains(ds, des_map, inv_des_map);

			//create custom domains
			s_dlg.SetStatus(SD_CDS);
			CreateCustomDomains(ds, des_map, inv_des_map, custom_members);

			//create custom domain relations
			s_dlg.SetStatus(SD_MRS);
			CreateMemberRelations(ds, des_map, inv_des_map);

			//create variable properties
			s_dlg.SetStatus(SD_VPS);
			CreateVariableProperties(des_map, inv_des_map, elements);

			//create constant properties
			s_dlg.SetStatus(SD_CPS);
			CreateConstantProperties(des_map, inv_des_map, elements, custom_members);
	
			//create simpleformula for properties
			CreateSimpleFormulas(ds, des_map, inv_des_map);
			//create assignments for VariableProperties
			s_dlg.SetStatus(SD_ASS);
			CreateAssignments(des_map, inv_des_map, elements, custom_members);
	
			//invoking Desert UI
			s_dlg.SetStatus(SD_GUI);

			using namespace BackIfaceFunctions;
		
			fprintf(fdDcif, "<?xml version=\"1.1\"?>\n");
			fprintf(fdDcif, "<DesertConfigurations>\n");

			if(multiRun) {
				// Initialize
				std::cout << "Starting Multirun" << std::endl;
				DesertFinitWithMultirun_Pre(numConsGroups, consGroupNames, consGroups);

				// First run with no constraints applied
				int numCfgs = DesertFinitWithMultirun_Exec(true, ((std::string) ds.SystemName()).c_str(), "NONE", "");
				std::cout << "Group: NONE\tConfigs: " << numCfgs << std::endl;
				fprintf(fdDcif, "\t<None NumConfigs=\"%d\"/>\n", numCfgs);
				ReInitializeManager(&ds, &des_map, &inv_des_map);

				// Next run with all constraints applied
				numCfgs = DesertFinitWithMultirun_Exec(true, ((std::string) ds.SystemName()).c_str(), "ALL", "applyAll");
				std::cout << "Group: NONE\tConfigs: " << numCfgs << std::endl;
				fprintf(fdDcif, "\t<All NumConfigs=\"%d\"/>\n", numCfgs);
				ReInitializeManager(&ds, &des_map, &inv_des_map);

				// Next run for all groups
				fprintf(fdDcif, "\t<Groups>\n");
				for(int j=0; j<numConsGroups; j++) {
					numCfgs = DesertFinitWithMultirun_Exec(true, ((std::string) ds.SystemName()).c_str(), consGroupNames[j], consGroups[j]);

					std::cout << "Group: NONE\tConfigs: " << numCfgs << std::endl;
					fprintf(fdDcif, "\t\t<Group Name=\"%s\" NumConfigs=\"%d\">\n", consGroupNames[j], numCfgs);
					std::vector<std::string> splittedCons = splitConstraints(consGroups[j]);
					for(std::vector<std::string>::iterator conIt = splittedCons.begin(); conIt != splittedCons.end(); ++conIt)
						fprintf(fdDcif, "\t\t\t<Constraint Name=\"%s\"/>\n", *conIt);
					fprintf(fdDcif, "\t\t</Group>\n");

					if(j<numConsGroups) {
						ReInitializeManager(&ds, &des_map, &inv_des_map);
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
				DBConfigurations * confs = (DBConfigurations * )DesertFinit(true, isSilent, consList == "" ? NULL : (LPCTSTR)consList);
				int numCfgs = (confs) ? confs->GetCount() : 0;
				
				std::string strConsList = trimSpaces(std::string(consList));
				if(strConsList.length() == 0 || strConsList.compare("None") == 0 || strConsList.compare("none") == 0) {
					fprintf(fdDcif, "\t<None NumConfigs=\"%d\"/>\n", numCfgs);
				} else if(strConsList.compare("applyAll") == 0) {
					fprintf(fdDcif, "\t<All NumConfigs=\"%d\"/>\n", numCfgs);
				} else {
					fprintf(fdDcif, "\t<ConstraintSet NumConfigs=\"%d\">\n", numCfgs);
					std::vector<std::string> allStrCons = splitConstraints(strConsList);
					for(std::vector<std::string>::iterator strCIt = allStrCons.begin(); strCIt != allStrCons.end(); ++strCIt)
						fprintf(fdDcif, "\t\t<Constraint Name=\"%s\"/>\n", *strCIt);
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
								"xml",									//default file name extension
								output_file,							//default file name
								OFN_HIDEREADONLY ,						//default behaviour
								szFilterUDM,							//no filter
								NULL									//no parent windows
								);
						if (SaveAs.DoModal() == IDOK)
						{
							strcpy(output_file, SaveAs.GetPathName());
						}
							else cancel_output = true;

					}
					if(!cancel_output)
					{
				
						//create data network
						Udm::SmartDataNetwork bw(DesertIfaceBack::diagram);
						DesertIfaceBack::DesertBackSystem dbs;
						bw.CreateNew(
							//(LPCTSTR)SaveAs.GetFileName(), 
							output_file,
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
							s_dlg.StepInState( (double)(((double)(++count))/((double)(confs->GetCount())))*100.0 );	//progress bar

						}//eo while(pos)
						pos = confs->GetHeadPosition();
						s_dlg.SetStatus(SD_BACK);
						count = 0;
						while(pos)
						{
							DBConfiguration * config = confs->GetNext(pos);
							if (config)
							{
								//create configuration
								DesertIfaceBack::Configuration dib_conf = DesertIfaceBack::Configuration::Create(dbs);
								CString s;s.Format("Conf. no: %d", config->id);
								dib_conf.name() = (LPCTSTR)s;
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
							
								TRACE("Got %d of configurations!\n", confs->GetCount());


								delete config;
							}//eo if (config)
							s_dlg.StepInState( (double)(((double)(++count))/((double)(confs->GetCount())))*100.0 );	//progress bar
						}//eo while(pos)
					
						delete confs;
			
						set<DesertIfaceBack::Configuration> cfgs = dbs.Configuration_kind_children();		
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
					}//eo if DoModal() == IDOK
				}//eo if (configs)	
		
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
			fflush(fdDcif);
			fclose(fdDcif);

		}//eo try
		catch(CDesertException *e)
		{
			fprintf(fdDcif, "</DesertConfigurations>\n");
			fflush(fdDcif);
			fclose(fdDcif);

			e->ReportError();
			//	throw e;
			return FALSE;
		}
		catch(udm_exception e)
		{
			fprintf(fdDcif, "</DesertConfigurations>\n");
			fflush(fdDcif);
			fclose(fdDcif);

			if (m_lpCmdLine && command_arg_ok)
				std::cerr << std::string("Udm exception: ") + e.what();
			else
				AfxMessageBox(CString("Udm exception: ") + CString(e.what()) );
			//	throw e;
			return FALSE;
		}
	
	}//eo if (Open.DoModal())

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return TRUE;
}

///////////////////////////////////////////////////////////////
// CWzdCommandLineInfo
CWzdCommandLineInfo::CWzdCommandLineInfo():
    desert_file (""), outputFileNeedsToBeRead(false), desert_output_file(""),
	silent(false), applyCons(false), consList(""), multiRun(false)
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

	int n = atoi((LPCTSTR)sArg);
	if(!outputFileNeedsToBeRead &&
		(sArg.Right(4) == ".xml" || sArg.Right(4) == ".mem" || sArg.Right(4) == ".mga"
			|| sArg.Right(4) == ".XML" || sArg.Right(4) == ".MEM" || sArg.Right(4) == ".MGA")
	  )
	{
		desert_file = sArg;
	}
	else if(sArg=="o")
	{
		outputFileNeedsToBeRead = true;
	}
	else if(outputFileNeedsToBeRead)
	{
		desert_output_file = sArg;
		outputFileNeedsToBeRead = false;
	}
	else if(sArg=="s") 
		silent=true;
	else if(sArg=="c")
	{
		silent = true;
		applyCons = true;
	}
	else if(applyCons)
	{
		consList = sArg;
	}
	else if(sArg=="m")
	{
		silent = true;
		multiRun = true;
	}
	else if(multiRun)
	{
		const char *ccstrConsGroups = (LPCTSTR) sArg;
		std::string strConsGroups = strdup(ccstrConsGroups);
		bool consGrpSpecError = false;

		std::string groupName = "";
		std::string constraintGroup = "";
		std::size_t posPrevGrpStart = strConsGroups.find("__CG__");
		std::size_t posPrevGrpEnd = 0;
		if(posPrevGrpStart != string::npos) {
			posPrevGrpEnd = strConsGroups.find(":");
			if(posPrevGrpEnd != string::npos) {
				groupName = strConsGroups.substr(posPrevGrpStart + 6, posPrevGrpEnd - posPrevGrpStart - 6);
			} else {
				consGrpSpecError = true;
			}
		}
		std::size_t posNextGrpStart = strConsGroups.find("__CG__", posPrevGrpEnd);
		while(!consGrpSpecError && posNextGrpStart != string::npos) {
			constraintGroup = strConsGroups.substr(posPrevGrpEnd + 1, posNextGrpStart - 1 - posPrevGrpEnd - 1);
			CString gn = groupName.c_str();
			consGroupNames.AddTail(gn);
			CString cg = constraintGroup.c_str();
			consGroups.AddTail(cg);

			std::size_t posNextGrpEnd = strConsGroups.find(":", posNextGrpStart);
			if(posNextGrpEnd != string::npos) {
				groupName = strConsGroups.substr(posNextGrpStart + 6, posNextGrpEnd - posNextGrpStart - 6);
				posPrevGrpStart = posNextGrpStart;
				posPrevGrpEnd = posNextGrpEnd;
				posNextGrpStart = strConsGroups.find("__CG__", posNextGrpEnd);
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