// desert.cpp : Defines the initialization routines for the DLL.
//



#include "stdafx.h"
#include "desert.h"
#include "CSetErrDialog.h"
#include <set>

#include "common/Error.h"
#pragma comment(linker, "/EXPORT:?SetLoggerHandler@@YAXP6AXPB_W0H@Z@Z")
#pragma comment(linker, "/EXPORT:?SetExitHandler@@YAXP6AXH@Z@Z")

#ifdef _DEBUG
//#define new DEBUG_NEW
#undef THIS_FILE
#define WIDE2(x) L##x
#define WIDE1(x) WIDE2(x)
static TCHAR WTHIS_FILE[] = WIDE1(__FILE__);
static char THIS_FILE[] = __FILE__;
#endif



//
//	Note!
//
//		If this DLL is dynamically linked against the MFC
//		DLLs, any functions exported from this DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

/////////////////////////////////////////////////////////////////////////////
// CDesertApp

BEGIN_MESSAGE_MAP(CDesertApp, CWinApp)
	//{{AFX_MSG_MAP(CDesertApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDesertApp construction

CDesertApp::CDesertApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CDesertApp object

CDesertApp theApp;

/////////////////////////////////////////////////////////////////////////////
// Functions exported from this dll

#include "common/error.h"
#include "core/coredefs.h"
#include "core/dynamic.h"
#include "core/manager.h"
#include "desertui.h"


#define DLL_DECL __declspec (dllexport)

CString projectName;

DLL_DECL void 
DesertInit(const TCHAR *prjName, bool append)
{
	if(CManager::theInstance)
	{
		delete CManager::theInstance;
		CManager::theInstance = NULL;
	}
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	ASSERT_EX( CManager::theInstance == NULL, _T("CoreInit"), _T("CManager::theInstance is NOT null"));
	CManager::theInstance = new CManager();
	CManager::unique = 1;
	projectName = prjName;

	// Start error logging
	CString log = projectName + _T("_desert.log");
	StartLogging(log, append);
}

int GroupRunWithinMultirun(const TCHAR* groupName, const TCHAR* constraintsToApply)
{
	CStringList cNames;
	bool applyAll = _tcsncmp(constraintsToApply, _T("applyAll"), 8) == 0;
	if (!applyAll)
	{
		TCHAR *cons = _tcsdup(constraintsToApply);
		TCHAR *cName = _tcstok( cons, _T(":") );
		while(cName)
		{
			cNames.AddTail( cName );
			cName = _tcstok( NULL, _T(":") );
		}
	}
	CDynConstraintSet *set = new CDynConstraintSet(0);
	set->RemoveAll();
	CDynConstraintSetList & setlist = CManager::theInstance->GetConstraintSets();

	POSITION sl_pos = setlist.GetHeadPosition();
	while (sl_pos)
	{
		CDynConstraintSet * setlist_i = setlist.GetNext(sl_pos);
		CDynConstraintList& list = setlist_i->GetConstraints();
		POSITION pos1 = list.GetHeadPosition();
		while(pos1)
		{
			CDynConstraint *cur = list.GetNext(pos1);
			const CString& nm  = cur->GetName();
			  
			if (applyAll || cNames.Find(nm))
			{
				Info(_T("DesertFinit"), _T("Applying Constraint: %s"), nm);
				cur->SetApplied();
				set->InsertConstraint(cur);
			}
		}
	}
	  
	// prune & generate next hierarchy
	double dspSize;
	long repSize;
	long clockTime;
	try
	{
		CManager::theInstance->GetSizeInfo(dspSize, repSize, clockTime, set);
		CManager::theInstance->GenerateNextHierarchy();
	}
	catch(CDesertException *e)
	{
		set->RemoveAll();
		delete set;
		delete CManager::theInstance;
		CManager::theInstance = NULL;
		StopLogging();
		throw e;
	}
	  
	Info(_T("DesertFinit"), _T("Design Space Size Info: %f %d %d"), dspSize, repSize, clockTime);
	set->RemoveAll();
	delete set;

	int numCfgs = 0;
	try
	{
		numCfgs = CManager::theInstance->CalcRealNoOfConfigurations();
		Info(_T("DesertFinitMultirun"), _T("For constraint group: %s with constraints:\r\n\t%s\r\n\tDesign Space Size Info: %f %d %d"), groupName, constraintsToApply, dspSize, repSize, clockTime);
		Info(_T("DesertFinitMultirun"), _T("No. of configurations with last set of constraints is %d"), numCfgs);
	}
	catch(CDesertException *e)
	{
		delete CManager::theInstance;
		CManager::theInstance = NULL;
		StopLogging();
		throw e;
	}

	return numCfgs;
}

DLL_DECL void
DesertFinitWithMultirun_Pre(int numConsGroups, TCHAR ** consGroupNames, TCHAR** consGroups)
{
  AFX_MANAGE_STATE(AfxGetStaticModuleState());
  ASSERT_EX( CManager::theInstance, _T("CoreInit"), _T("CManager::theInstance IS null"));
 
  for(int i=0; i<numConsGroups; i++) {
	  Info(_T("DesertFinitWithMultirun"), _T("Constraint Group: %s\r\n\tConstraints: %s"), consGroupNames[i], consGroups[i]);
  }
}

DLL_DECL int
DesertFinitWithMultirun_Exec(bool noError, const TCHAR* prjName, TCHAR * consGroupName, TCHAR* consGroups)
{
  AFX_MANAGE_STATE(AfxGetStaticModuleState());
  ASSERT_EX( CManager::theInstance, _T("CoreInit"), _T("CManager::theInstance IS null"));
 
  CCSetErrDialog cse_dialog;
  CManager::theInstance->VerifyConstraints(&cse_dialog);
  CManager::theInstance->AnalyseConstraints();
  CManager::theInstance->GenerateNextHierarchy();
  
  if (noError)
  {
	  // Run with given (or no) constraints
	  int numCfgs = GroupRunWithinMultirun(consGroupName, consGroups);
	  return numCfgs;
  }
  return -1;
}

DLL_DECL void
DesertFinitWithMultirun_Post()
{
  // delete the manager instance
  delete CManager::theInstance;
  CManager::theInstance = NULL;

  Info(_T("DesertFinit"), _T("Generated Configurations"));

  // stop logging
  StopLogging();
}

static void*
DesertFinit(bool noError, bool isSilent, const TCHAR *applyConstraints, CManager::fBuildConfigurations BuildConfigurations)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	ASSERT_EX(CManager::theInstance, _T("CoreInit"), _T("CManager::theInstance IS null"));

	Info(_T("DesertFinit"), _T("Constraint List: %s"), applyConstraints);
	CCSetErrDialog cse_dialog;

	CManager::theInstance->VerifyConstraints(&cse_dialog);

	if (!isSilent && cse_dialog.IsAlive())
		if (cse_dialog.DoModal() != IDOK)
		{
			delete CManager::theInstance;
			CManager::theInstance = NULL;
			return NULL;
		}

	CManager::theInstance->AnalyseConstraints();
	CManager::theInstance->GenerateNextHierarchy();

	void * retval = NULL;
	if (noError)
	{
		if (CManager::theInstance->HasConstraints())
		{
			if (isSilent && !applyConstraints) {
				applyConstraints = _T("");
			}
			if (applyConstraints)
			{
				CStringList cNames;
				bool applyAll = _tcsncmp(applyConstraints, _T("applyAll"), 8) == 0;
				if (!applyAll)
				{
					TCHAR *cons = _tcsdup(applyConstraints);
					TCHAR *cName = _tcstok(cons, _T(":"));
					while (cName)
					{
						cNames.AddTail(cName);
						cName = _tcstok(NULL, _T(":"));
					}
				}
				CDynConstraintSet *set = new CDynConstraintSet(0);
				set->RemoveAll();
				CDynConstraintSetList & setlist = CManager::theInstance->GetConstraintSets();

				POSITION sl_pos = setlist.GetHeadPosition();
				while (sl_pos)
				{
					CDynConstraintSet * setlist_i = setlist.GetNext(sl_pos);
					CDynConstraintList& list = setlist_i->GetConstraints();
					POSITION pos1 = list.GetHeadPosition();
					while (pos1)
					{
						CDynConstraint *cur = list.GetNext(pos1);
						const CString& nm = cur->GetName();

						if (applyAll || cNames.Find(nm))
						{
							Info(_T("DesertFinit"), _T("Applying Constraint: %s"), nm);
							cur->SetApplied();
							set->InsertConstraint(cur);
						}
					}
				}
				// prune & generate next hierarchy
				double dspSize;
				long repSize;
				long clockTime;
				try {
					CManager::theInstance->GetSizeInfo(dspSize, repSize, clockTime, set);
					CManager::theInstance->GenerateNextHierarchy();
				}
				catch (CDesertException *e)
				{
					set->RemoveAll();
					delete set;
					delete CManager::theInstance;
					CManager::theInstance = NULL;
					StopLogging();
					throw e;
				}
				Info(_T("DesertFinit"), _T("Design Space Size Info: %f %d %d"), dspSize, repSize, clockTime);
				set->RemoveAll();
				delete set;
			}
			else
			{
				if (!isSilent) {
					CDesertUi ui(_T("Design Space Exploration Tool"));
					ui.DoModal(); // launch the ui
				}
			}
		}
		//#ifdef DO_STORE_CONFIGURATIONS
		// dump the configurations
		CString fname = projectName + _T(".cfg");
		tstring errmsg;
		try {
			retval = CManager::theInstance->StoreConfigurations(fname, errmsg, BuildConfigurations);
		}
		catch (CDesertException *e)
		{
			delete CManager::theInstance;
			CManager::theInstance = NULL;
			StopLogging();
			throw e;
		}
		//#endif
	}

	// delete the manager instance
	delete CManager::theInstance;
	CManager::theInstance = NULL;

	Info(_T("DesertFinit"), _T("Generated Configurations"));

	// stop logging
	StopLogging();
	return retval;
}

DLL_DECL void *
DesertFinit(bool noError, bool isSilent, const TCHAR *applyConstraints, int(*BuildConfigurationsCallbackFunction)(void* callbackArg, const BackIfaceFunctions::DBConfiguration&), void* callbackArg)
{
	CManager::theInstance->BuildConfigurationsCallbackFunctionArg = callbackArg;
	CManager::theInstance->BuildConfigurationsCallbackFunction = BuildConfigurationsCallbackFunction;
	return DesertFinit(noError, isSilent, applyConstraints, &CManager::BuildConfigurationsCallback);
}

DLL_DECL void *
DesertFinit(bool noError, bool isSilent, const TCHAR *applyConstraints)
{
	return DesertFinit(noError, isSilent, applyConstraints, &CManager::BuildConfigurations);
}

DLL_DECL long
CreateSpace(const TCHAR *n)
{
  AFX_MANAGE_STATE(AfxGetStaticModuleState());
  ASSERT_EX( CManager::theInstance, WTHIS_FILE, _T("manager not instantiated") );
  return CManager::theInstance->CreateSpace(n);
}



DLL_DECL long
CreateElement(const TCHAR *n, long s, short d, long p, long e)
{
  AFX_MANAGE_STATE(AfxGetStaticModuleState());
  ASSERT_EX( CManager::theInstance, WTHIS_FILE, _T("manager not instantiated") );
  return CManager::theInstance->CreateElement(n, s, d, p, e);
}
/*
DLL_DECL long
CreateElement(const TCHAR *n, long s, short d, long p,  long id, long e_id)
{
  AFX_MANAGE_STATE(AfxGetStaticModuleState());
  ASSERT_EX( CManager::theInstance, WTHIS_FILE, _T("manager not instantiated") );
  return CManager::theInstance->CreateElement(n, s, d, p, id, e_id);
}*/


DLL_DECL long
CreateNaturalDomain(const TCHAR *n, int mx, int mn)
{
  AFX_MANAGE_STATE(AfxGetStaticModuleState());
  ASSERT_EX( CManager::theInstance, WTHIS_FILE, _T("manager not instantiated") );
  return CManager::theInstance->CreateNaturalDomain(n, mx, mn);
}


/*
DLL_DECL long
CreateNaturalDomain(const TCHAR *n, int mx, int mn, long id, long e_id)
{
  AFX_MANAGE_STATE(AfxGetStaticModuleState());
  ASSERT_EX( CManager::theInstance, WTHIS_FILE, _T("manager not instantiated") );
  return CManager::theInstance->CreateNaturalDomain(id, e_id, n, mx, mn);
}*/

DLL_DECL long
CreateCustomDomain(const TCHAR *n)
{
  AFX_MANAGE_STATE(AfxGetStaticModuleState());
  ASSERT_EX( CManager::theInstance, WTHIS_FILE, _T("manager not instantiated") );
  return CManager::theInstance->CreateCustomDomain(n);
}
/*
DLL_DECL long
CreateCustomDomain(const TCHAR *n, long id, long e_id)
{
  AFX_MANAGE_STATE(AfxGetStaticModuleState());
  ASSERT_EX( CManager::theInstance, WTHIS_FILE, _T("manager not instantiated") );
  return CManager::theInstance->CreateCustomDomain(n, id, e_id);
}*/

#ifdef __OLD_INTERFACE
DLL_DECL long
CreateElement(const TCHAR *n, long d, long e)
{
  AFX_MANAGE_STATE(AfxGetStaticModuleState());
  ASSERT_EX( CManager::theInstance, WTHIS_FILE, _T("manager not instantiated") );
  return CManager::theInstance->CreateElement(n, d, e);
}
#endif

DLL_DECL long
CreateConstantProperty(const TCHAR *n, const TCHAR *pcm_fn, long o, long d, int v)
{
  AFX_MANAGE_STATE(AfxGetStaticModuleState());
  ASSERT_EX( CManager::theInstance, WTHIS_FILE, _T("manager not instantiated") );
  return CManager::theInstance->CreateConstantProperty(n, pcm_fn, o, d, v);
}

/*
DLL_DECL long
CreateConstantProperty(const TCHAR *n, const TCHAR *pcm_fn, long o, long d, int v, long id, long e_id)
{
  AFX_MANAGE_STATE(AfxGetStaticModuleState());
  ASSERT_EX( CManager::theInstance, WTHIS_FILE, _T("manager not instantiated") );
  return CManager::theInstance->CreateConstantProperty(n, pcm_fn, o, d, v, id, e_id);
}

*/
DLL_DECL long
CreateVariableProperty(const TCHAR *n, const TCHAR *pcm_fn, long o, long d)
{
  AFX_MANAGE_STATE(AfxGetStaticModuleState());
  ASSERT_EX( CManager::theInstance, WTHIS_FILE, _T("manager not instantiated") );
  return CManager::theInstance->CreateVariableProperty(n, pcm_fn, o, d);
}

DLL_DECL long
CreateVariableProperty(const TCHAR *n, const TCHAR *pcm_fn, long o)
{
  AFX_MANAGE_STATE(AfxGetStaticModuleState());
  ASSERT_EX( CManager::theInstance, WTHIS_FILE, _T("manager not instantiated") );
  return CManager::theInstance->CreateVariableProperty(n, pcm_fn, o);
}

DLL_DECL long 
createParametricVariableProperty(const TCHAR *name, long owner, const TCHAR *text)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
  ASSERT_EX( CManager::theInstance, WTHIS_FILE, _T("manager not instantiated") );
  return CManager::theInstance->CreateParametricVariableProperty(name, owner, text);
}
/*
DLL_DECL long
CreateVariableProperty(const TCHAR *n, const TCHAR *pcm_fn, long o, long d, long id, long e_id)
{
  AFX_MANAGE_STATE(AfxGetStaticModuleState());
  ASSERT_EX( CManager::theInstance, WTHIS_FILE, _T("manager not instantiated") );
  return CManager::theInstance->CreateVariableProperty(n, pcm_fn, o, d, id, e_id);
}
*/

DLL_DECL long
CreateRelation(long c, long s, long d)
{
  AFX_MANAGE_STATE(AfxGetStaticModuleState());
  ASSERT_EX( CManager::theInstance, WTHIS_FILE, _T("manager not instantiated") );
  return CManager::theInstance->CreateRelation(c, s, d);
}

/*
DLL_DECL long
CreateRelation(long c, long s, long d, long id, long e_id)
{
  AFX_MANAGE_STATE(AfxGetStaticModuleState());
  ASSERT_EX( CManager::theInstance, WTHIS_FILE, _T("manager not instantiated") );
  return CManager::theInstance->CreateRelation(c, s, d, id, e_id);
}
*/


DLL_DECL long
CreateConstraintSet(const TCHAR *n)
{
  AFX_MANAGE_STATE(AfxGetStaticModuleState());
  ASSERT_EX( CManager::theInstance, WTHIS_FILE, _T("manager not instantiated") );
  return CManager::theInstance->CreateConstraintSet(n);
}

/*
DLL_DECL long
CreateConstraintSet(const TCHAR *n, long id, long e_id)
{
  AFX_MANAGE_STATE(AfxGetStaticModuleState());
  ASSERT_EX( CManager::theInstance, WTHIS_FILE, _T("manager not instantiated") );
  return CManager::theInstance->CreateConstraintSet(n, id, e_id);
}
*/

DLL_DECL long
CreateConstraint(const TCHAR *n, long cs, long c, const TCHAR *t)
{
  AFX_MANAGE_STATE(AfxGetStaticModuleState());
  ASSERT_EX( CManager::theInstance, WTHIS_FILE, _T("manager not instantiated") );
  return CManager::theInstance->CreateConstraint(n, cs, c, t);
}

/*
DLL_DECL long
CreateConstraint(const TCHAR *n, long cs, long c, const TCHAR *t, long id, long e_id)
{
  AFX_MANAGE_STATE(AfxGetStaticModuleState());
  ASSERT_EX( CManager::theInstance, WTHIS_FILE, _T("manager not instantiated") );
  return CManager::theInstance->CreateConstraint(n, cs, c, t, id, e_id);
}

DLL_DECL long
AddtoVariableProperty(const TCHAR *n, long o, long v, long d)
{
  AFX_MANAGE_STATE(AfxGetStaticModuleState());
  ASSERT_EX( CManager::theInstance, WTHIS_FILE, _T("manager not instantiated") );
  return CManager::theInstance->AddtoVariableProperty(n, o, v, d);
}

DLL_DECL long
AddtoVariableProperty(long p, long o, long v, long d)
{
  AFX_MANAGE_STATE(AfxGetStaticModuleState());
  ASSERT_EX( CManager::theInstance, WTHIS_FILE, _T("manager not instantiated") );
  return CManager::theInstance->AddtoVariableProperty(p, o, v, d);
}*/

#ifndef DOUBLE_MTBDD
DLL_DECL long
AddtoVariableProperty(const TCHAR *n, const TCHAR * name, long o, long v, long d)
{
  AFX_MANAGE_STATE(AfxGetStaticModuleState());
  ASSERT_EX( CManager::theInstance, WTHIS_FILE, _T("manager not instantiated") );
  return CManager::theInstance->AddtoVariableProperty(n, name, o, v, d);
}

DLL_DECL long
AddtoVariableProperty(long p, const TCHAR * name, long o, long v, long d)
{
  AFX_MANAGE_STATE(AfxGetStaticModuleState());
  ASSERT_EX( CManager::theInstance, WTHIS_FILE, _T("manager not instantiated") );
  return CManager::theInstance->AddtoVariableProperty(p, name, o, v, d);
}
#else
DLL_DECL long
AddtoVariableProperty(const TCHAR *n, const TCHAR * name, long o, double v, long d)
{
  AFX_MANAGE_STATE(AfxGetStaticModuleState());
  ASSERT_EX( CManager::theInstance, WTHIS_FILE, _T("manager not instantiated") );
  return CManager::theInstance->AddtoVariableProperty(n, name, o, v, d);
}

DLL_DECL long
AddtoVariableProperty(long p, const TCHAR * name, long o, double v, long d)
{
  AFX_MANAGE_STATE(AfxGetStaticModuleState());
  ASSERT_EX( CManager::theInstance, WTHIS_FILE, _T("manager not instantiated") );
  return CManager::theInstance->AddtoVariableProperty(p, name, o, v, d);
}
#endif

DLL_DECL long
AddtoSimpleFormulaVariableProperty(long p, const TCHAR *n, long o, long sp, long so)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	ASSERT_EX( CManager::theInstance, WTHIS_FILE, _T("manager not instantiated") );
	return CManager::theInstance->AddtoSimpleFormulaVariableProperty(p, n, o, sp, so);
}

DLL_DECL long 
CreateSimpleFormula(const TCHAR *pcfn, std::map<long, long> &srcProps, std::map<long, long> &dstProps)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	ASSERT_EX( CManager::theInstance, WTHIS_FILE, _T("manager not instantiated") );
	CMap<long, long, long, long> sprops, dprops;
	std::map<long, long>::iterator pos;
	for(pos=srcProps.begin();pos!=srcProps.end();++pos)
	{
		sprops[(*pos).first] = (*pos).second;
	}
	for(pos=dstProps.begin();pos!=dstProps.end();++pos)
	{
		dprops[(*pos).first] = (*pos).second;
	}
	return CManager::theInstance->CreateSimpleFormula(pcfn,sprops, dprops);
}

DLL_DECL bool VerifyConstraints(const TCHAR *applyConstraints)
{
  AFX_MANAGE_STATE(AfxGetStaticModuleState());
  ASSERT_EX( CManager::theInstance, _T("CoreInit"), _T("CManager::theInstance IS null"));
 
  CCSetErrDialog cse_dialog;

  if(!CManager::theInstance->VerifyConstraints(&cse_dialog))
  {
  	  cse_dialog.DoModal();
	  return false;
  }

  return true;
}

DLL_DECL void *
DesertFinitNoGui(bool noError,bool noGui,const TCHAR *applyConstraints)
{
  AFX_MANAGE_STATE(AfxGetStaticModuleState());
  ASSERT_EX( CManager::theInstance, _T("CoreInit"), _T("CManager::theInstance IS null"));
 
  CCSetErrDialog cse_dialog;

  if(!CManager::theInstance->VerifyConstraints(&cse_dialog))
  {
	  delete CManager::theInstance;
	  CManager::theInstance = NULL;
	  throw new CDesertException(cse_dialog.m_strErrCts);
  }

  CManager::theInstance->AnalyseConstraints();
  CManager::theInstance->GenerateNextHierarchy();
  
  void * retval = NULL;
  if (noError)
  {
    if (CManager::theInstance->HasConstraints())
    {
		if (applyConstraints)
		{
			CStringList cNames;
			bool applyAll = _tcsncmp(applyConstraints, _T("applyAll"), 8) == 0;
			if (!applyAll)
			{
				TCHAR *cons = _tcsdup(applyConstraints);
				TCHAR *cName = _tcstok( cons, _T(":") );
				while(cName)
				{
					cNames.AddTail( cName );
					cName = _tcstok( NULL, _T(":") );
				}
			}
			CDynConstraintSet *set = new CDynConstraintSet(0);
			set->RemoveAll();
			CDynConstraintSetList & setlist = CManager::theInstance->GetConstraintSets();

			POSITION sl_pos = setlist.GetHeadPosition();
			while (sl_pos)
			{
				CDynConstraintSet * setlist_i = setlist.GetNext(sl_pos);
				CDynConstraintList& list = setlist_i->GetConstraints();
				POSITION pos1 = list.GetHeadPosition();
				while(pos1)
				{
					CDynConstraint *cur = list.GetNext(pos1);
					const CString& nm  = cur->GetName();

					if (applyAll || cNames.Find(nm))
					{
						Info(_T("DesertFinit"), _T("Applying Constraint: %s"), nm);
						cur->SetApplied();
						set->InsertConstraint(cur);
					}
				}
			}
			// prune & generate next hierarchy
			double dspSize;
			long repSize;
			long clockTime;
			try{
				CManager::theInstance->GetSizeInfo(dspSize, repSize, clockTime, set);			
			}catch(CDesertException *e)
			{
				CManager::theInstance->GenerateNextHierarchy();
				set->RemoveAll();
				delete set;
				delete CManager::theInstance;
				CManager::theInstance = NULL;
				StopLogging();
				throw e;
			}
			CManager::theInstance->GenerateNextHierarchy();
			Info(_T("DesertFinit"), _T("Design Space Size Info: %f %d %d"), dspSize, repSize, clockTime);
			set->RemoveAll();
			delete set;
		}
    }
//	CManager::theInstance->
//#ifdef DO_STORE_CONFIGURATIONS
    // dump the configurations
    CString fname = projectName + _T(".cfg");
	tstring errmsg;
	try{
		retval = CManager::theInstance->StoreConfigurations(fname, errmsg, &CManager::BuildConfigurations);
	 }
	catch(CDesertException *e)
	{
		delete CManager::theInstance;
		CManager::theInstance = NULL;
		StopLogging();
		throw e;
	}
//#endif
  }

  // delete the manager instance
  delete CManager::theInstance;
  CManager::theInstance = NULL;

  Info(_T("DesertFinit"), _T("Generated Configurations"));

  // stop logging
  StopLogging();
  return retval;
}

DLL_DECL bool
DesertFinit_preApply()
{
  AFX_MANAGE_STATE(AfxGetStaticModuleState());
  ASSERT_EX( CManager::theInstance, _T("CoreInit"), _T("CManager::theInstance IS null"));
 
  CCSetErrDialog cse_dialog;

  if(!CManager::theInstance->VerifyConstraints(&cse_dialog))
  {
	//  delete CManager::theInstance;
	//  CManager::theInstance = NULL;
	  if(cse_dialog.DoModal()==IDOK)
		  return true;
	 // throw new CDesertException(cse_dialog.m_strErrCts);
	  return false;
  }

//	CManager::theInstance->AnalyseConstraints();
//	CManager::theInstance->GenerateNextHierarchy();
  return true;
}

DLL_DECL void 
DesertFinit_Apply(const TCHAR *applyConstraints)
{
	CManager::theInstance->AnalyseConstraints();
	CManager::theInstance->GenerateNextHierarchy();

    if (CManager::theInstance->HasConstraints())
    {
		if (applyConstraints)
		{
			CStringList cNames;
			bool applyAll = _tcsncmp(applyConstraints, _T("applyAll"), 8) == 0;
			if (!applyAll)
			{
				TCHAR *cons = _tcsdup(applyConstraints);
				TCHAR *cName = _tcstok( cons, _T(":") );
				while(cName)
				{
					cNames.AddTail( cName );
					cName = _tcstok( NULL, _T(":") );
				}
				if(cNames.IsEmpty()) return;
			}
			
			CDynConstraintSet *set = new CDynConstraintSet(0);
			set->RemoveAll();
			CDynConstraintSetList & setlist = CManager::theInstance->GetConstraintSets();

			POSITION sl_pos = setlist.GetHeadPosition();
			while (sl_pos)
			{
				CDynConstraintSet * setlist_i = setlist.GetNext(sl_pos);
				CDynConstraintList& list = setlist_i->GetConstraints();
				POSITION pos1 = list.GetHeadPosition();
				while(pos1)
				{
					CDynConstraint *cur = list.GetNext(pos1);
					const CString& nm  = cur->GetName();

					if (applyAll || cNames.Find(nm))
					{
						Info(_T("DesertFinit"), _T("Applying Constraint: %s"), nm);
						cur->SetApplied();
						set->InsertConstraint(cur);
					}
				}
			}
			// prune & generate next hierarchy
			double dspSize;
			long repSize;
			long clockTime;
			try{
				CManager::theInstance->GetSizeInfo(dspSize, repSize, clockTime, set);
				
			}catch(CDesertException *e)
			{
				CManager::theInstance->GenerateNextHierarchy();
				set->RemoveAll();
				delete set;
				throw e;
			}

		//	Info(_T("DesertFinit"), _T("Design Space Size Info: %f %d %d"), dspSize, repSize, clockTime);
			CManager::theInstance->GenerateNextHierarchy();
			set->RemoveAll();
			delete set;
		}
	 }
}

DLL_DECL void *
DesertFinit_postApply()
{ 
	void * retval = NULL; 
	CString fname(_T(""));
	tstring errmsg;
	try{
		retval = CManager::theInstance->StoreConfigurations(fname, errmsg, &CManager::BuildConfigurations);
	 }
	catch(CDesertException *e)
	{
		throw e;
	}
	if(!errmsg.empty())
	{
		AfxMessageBox(errmsg.c_str(), MB_ICONSTOP);
	}

	return retval;
}

DLL_DECL void 
closeDesertManageInstace()
{
	// delete the manager instance
	if(CManager::theInstance)
	{
		delete CManager::theInstance;
		CManager::theInstance = NULL;
	}
}

DLL_DECL long
Desert_getRealNoOfConfigurations(volatile bool& cancel)
{
	try{
		return CManager::theInstance->CalcRealNoOfConfigurations(cancel);
	}
	catch(CDesertException *e)
	{
		throw e;
	}
}

DLL_DECL void 
Desert_goBack()
{
	ASSERT_EX(CManager::theInstance,_T("CManaget->NavigateBack"), _T("Manager is not present"));
	ASSERT_EX(CManager::theInstance->IsBackNavigable(),_T("CManaget->NavigateBack"),
		_T("Manager is not back!"));
	CManager::theInstance->NavigateBack();
}

DLL_DECL void 
Desert_goForward()
{
	ASSERT_EX(CManager::theInstance,_T("CManaget->NavigateForward"), _T("Manager is not present"));
	ASSERT_EX(CManager::theInstance->IsForwardNavigable(),_T("CManaget->NavigateForward"),
		_T("Manager is not forward!"));
	CManager::theInstance->NavigateForward();
}

DLL_DECL bool 
isDesertBackNavigable()
{
	ASSERT_EX(CManager::theInstance, _T("Check BackNavigable"), _T("Manager is NULL"));
  	return CManager::theInstance->IsBackNavigable();
}

DLL_DECL bool 
isDesertForwardNavigable()
{
	ASSERT_EX(CManager::theInstance, _T("Check ForwardNavigable"), _T("Manager is NULL"));
	return CManager::theInstance->IsForwardNavigable();
}

DLL_DECL void 
getDesertAppliedConstraintSet(std::set<tstring> &consSet)
{
	if(CManager::theInstance->HasGenerations()) {
		CDynConstraintSetList& consets = CManager::theInstance->GetConstraintSets();
		POSITION pos = consets.GetHeadPosition();
		while(pos)
		{
			CDynConstraintSet* set = consets.GetNext(pos);
			CDynConstraintList& constraints = set->GetConstraints();
			POSITION pos1 = constraints.GetHeadPosition();
			while(pos1)
			{
				CDynConstraint* cons = constraints.GetNext(pos1);
				CConstraint * static_cons =(CConstraint *) cons->GetCore();
				if(cons->WasApplied())
				//fill only not-implicit constraints
				//if (!static_cons->IsImplicit())
				{
					consSet.insert((const TCHAR *)*cons);
				}
			}
		}
	}
}

DLL_DECL _int64 
getDesertSize()
{
	ASSERT_EX( CManager::theInstance, _T("get Desert DesignSpace Size Info"), _T("manager not instantiated") );
	long dummy = 0;                       // we want the clocktime from the previous apply
	
	if (CManager::theInstance->ui_refresh_needed)
	{
		double dspSize;
		long repSize;
		try{
  			CManager::theInstance->GetSizeInfo(dspSize, repSize, dummy);
		}catch(CDesertException *e)
		{
			throw e;
		}
		CManager::theInstance->ui_refresh_needed = false;
	}
	//return CManager::theInstance->GetDSSize();
	return CManager::theInstance->ComputeSize();
}


////for Formula
DLL_DECL long
CreateFormulaSet(const TCHAR *n)
{
  AFX_MANAGE_STATE(AfxGetStaticModuleState());
  ASSERT_EX( CManager::theInstance, WTHIS_FILE, _T("manager not instantiated") );
  return CManager::theInstance->CreateFormulaSet(n);
}

DLL_DECL long
CreateFormula(const TCHAR *n, long cs, long c, const TCHAR *t)
{
  AFX_MANAGE_STATE(AfxGetStaticModuleState());
  ASSERT_EX( CManager::theInstance, WTHIS_FILE, _T("manager not instantiated") );
  return CManager::theInstance->CreateFormula(n, cs, c, t);
}

DLL_DECL void 
closeDesertLogFile()
{
	StopLogging();
}

DLL_DECL bool 
VerifyConstraint(const TCHAR *constraint)
{
	bool ret = true;
    if (!CManager::theInstance->HasConstraints())
		return ret;

	CDynConstraintSetList & setlist = CManager::theInstance->GetConstraintSets();

	POSITION sl_pos = setlist.GetHeadPosition();
	bool found=false;
	while (sl_pos && !found)
	{
		CDynConstraintSet * setlist_i = setlist.GetNext(sl_pos);
		CDynConstraintList& list = setlist_i->GetConstraints();
		POSITION pos1 = list.GetHeadPosition();
		while(pos1)
		{
			CDynConstraint *cur = list.GetNext(pos1);
			const CString& nm  = cur->GetName();

			if (nm==CString(constraint))
			{
				CConstraint *con = (CConstraint *)cur->GetCore();
				CString err_str, what;
				bool isvalid = con->Verify(err_str, what);
				if(!isvalid)
				{
					CCSetErrDialog cse_dialog;
					cse_dialog.AddError(NULL,*cur, *(cur->GetContext()), (LPCTSTR)err_str,  what);
				}
				found = true;
				break;
			}
		}
	}

	return ret;
}

void ThrowDesertException(TCHAR *message)
{
	throw new CDesertException(true, message);
}

extern "C" {
void (*bdd_fatal_hook)(TCHAR *message) = ThrowDesertException;
}
