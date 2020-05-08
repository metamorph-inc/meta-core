#ifndef desertdll_H
#define desertdll_H

// prototype of the desert dll exported functions

#define DLL_DECL __declspec (dllimport)

#include "BackIface.h"

#define DESIGN_SPACE_TOO_LARGE -2

//error handling with exceptions

class CDesertException : public CException
{
	bool fatal;
	TCHAR *what;
	TCHAR *constraint_name;
public:
	DECLARE_DYNAMIC(CDesertException)				//MFC inheritence

	//different constructors
	CDesertException();			
	CDesertException(bool _fatal);

	CDesertException(const TCHAR * _what);
	CDesertException(const TCHAR * _what, const TCHAR * _name);
	CDesertException(const CString _what);
	CDesertException(const CString _what, const CString _name);

	CDesertException(bool _fatal, const TCHAR * _what);
	CDesertException(bool _fatal, const CString _what);
	
	BOOL GetErrorMessage( LPTSTR lpszError, UINT nMaxError, PUINT pnHelpContext = NULL)	;
	CString GetErrorMessage();
	CString GetConstraintName();
	bool Fatal();
	~CDesertException();						

};

DLL_DECL void DesertInit(const TCHAR *prjName, bool append=false);

DLL_DECL void *DesertFinit(bool noError, bool isSilent, const TCHAR *applyConstraints, int(*BuildConfigurationsCallbackFunction)(void* callbackArg, const BackIfaceFunctions::DBConfiguration&), void* callbackArg);
DLL_DECL void * DesertFinit(bool noError, bool isSilent, const TCHAR *applyConstraints);
DLL_DECL void * DesertFinitNoGui(bool noError,bool noGui,const TCHAR *applyConstraints);

DLL_DECL void DesertFinitWithMultirun_Pre(int numConsGroups, TCHAR ** consGroupNames, TCHAR** consGroups);
DLL_DECL int DesertFinitWithMultirun_Exec(bool noError, const TCHAR* prjName, TCHAR * consGroupName, TCHAR* consGroups);
DLL_DECL void DesertFinitWithMultirun_Post();

DLL_DECL bool VerifyConstraints(const TCHAR *applyConstraints);

//Verify single constraint
DLL_DECL bool VerifyConstraint(const TCHAR *constraint);
/*
DLL_DECL void * DesertFinit(bool noError, double& dspSize, long& repSize, long& encSize);
DLL_DECL void * DesertFinit(bool noError, double& dspSize, long& repSize, long& encSize, long& consSize, long& prunedRepSize, double& prunedSpaceSize);
*/
DLL_DECL long CreateSpace(const TCHAR *name);
DLL_DECL long CreateElement(const TCHAR *name, long space_or_domain, short decomp, long parent, long ext);
DLL_DECL long CreateNaturalDomain(const TCHAR *name, int mx, int mn);
DLL_DECL long CreateCustomDomain(const TCHAR *name);
DLL_DECL long CreateConstantProperty(const TCHAR *name, const TCHAR *pcfn, long owner, long domain, int value);
DLL_DECL long CreateVariableProperty(const TCHAR *name, const TCHAR *pcfn, long owner, long domain);
DLL_DECL long CreateRelation(long context, long source, long destination);
DLL_DECL long CreateConstraintSet(const TCHAR *name);
DLL_DECL long CreateConstraint(const TCHAR *name, long constraintSet, long context, const TCHAR *text);
/////
DLL_DECL long createParametricVariableProperty(const TCHAR *name, long owner, const TCHAR *text);
DLL_DECL long CreateFormulaSet(const TCHAR *name);
DLL_DECL long CreateFormula(const TCHAR *name, long constraintSet, long context, const TCHAR *text);
DLL_DECL long CreateVariableProperty(const TCHAR *name, const TCHAR *pcfn, long owner);
/////
#ifndef DOUBLE_MTBDD
//DLL_DECL long AddtoVariableProperty(const TCHAR *name, long owner, long value, long domain);
//DLL_DECL long AddtoVariableProperty(long property, long owner, long value, long domain);
DLL_DECL long AddtoVariableProperty(const TCHAR *name, const TCHAR *n, long owner, long value, long domain);
DLL_DECL long AddtoVariableProperty(long property, const TCHAR *n, long owner, long value, long domain);
#else
DLL_DECL long AddtoVariableProperty(const TCHAR *name, const TCHAR *n, long owner, double value, long domain);
DLL_DECL long AddtoVariableProperty(long property, const TCHAR *n, long owner, double value, long domain);
#endif

DLL_DECL long AddtoSimpleFormulaVariableProperty(long property, const TCHAR *n, long owner, long srcProperty, long srcOwner);
DLL_DECL long CreateSimpleFormula(const TCHAR *pcfn, std::map<long, long> &srcProps, std::map<long, long> &dstProps);
//added by Feng 6/9/2011
DLL_DECL bool DesertFinit_preApply();
DLL_DECL void DesertFinit_Apply(const TCHAR *applyConstraints);
DLL_DECL void * DesertFinit_postApply();
DLL_DECL void closeDesertManageInstace();
DLL_DECL long Desert_getRealNoOfConfigurations(volatile bool& cancel);
DLL_DECL void Desert_goBack();
DLL_DECL void Desert_goForward();
DLL_DECL bool isDesertBackNavigable();
DLL_DECL bool isDesertForwardNavigable();
DLL_DECL void getDesertAppliedConstraintSet(std::set<tstring> &consSet);
DLL_DECL _int64 getDesertSize();
DLL_DECL void closeDesertLogFile();

#endif // #ifndef desertdll_H
