#ifndef desertdll_H
#define desertdll_H

// prototype of the desert dll exported functions

#define DLL_DECL __declspec (dllimport)

//error handling with exceptions

class CDesertException : public CException
{
	bool fatal;
	char *what;
	char *constraint_name;
public:
	DECLARE_DYNAMIC(CDesertException)				//MFC inheritence

	//different constructors
	CDesertException();			
	CDesertException(bool _fatal);

	CDesertException(const char * _what);
	CDesertException(const char * _what, const char * _name);
	CDesertException(const CString _what);
	CDesertException(const CString _what, const CString _name);

	CDesertException(bool _fatal, const char * _what);
	CDesertException(bool _fatal, const CString _what);
	
	BOOL GetErrorMessage( LPTSTR lpszError, UINT nMaxError, PUINT pnHelpContext = NULL)	;
	CString GetErrorMessage();
	CString GetConstraintName();
	bool Fatal();
	~CDesertException();						

};

DLL_DECL void DesertInit(const char *prjName, bool append=false);
DLL_DECL void * DesertFinit(bool noError, bool isSilent, const char *applyConstraints);
DLL_DECL void * DesertFinitNoGui(bool noError,bool noGui,const char *applyConstraints);

DLL_DECL void DesertFinitWithMultirun_Pre(int numConsGroups, char ** consGroupNames, char** consGroups);
DLL_DECL int DesertFinitWithMultirun_Exec(bool noError, const char* prjName, char * consGroupName, char* consGroups);
DLL_DECL void DesertFinitWithMultirun_Post();

DLL_DECL bool VerifyConstraints(const char *applyConstraints);

//Verify single constraint
DLL_DECL bool VerifyConstraint(const char *constraint);
/*
DLL_DECL void * DesertFinit(bool noError, double& dspSize, long& repSize, long& encSize);
DLL_DECL void * DesertFinit(bool noError, double& dspSize, long& repSize, long& encSize, long& consSize, long& prunedRepSize, double& prunedSpaceSize);
*/
DLL_DECL long CreateSpace(const char *name);
DLL_DECL long CreateElement(const char *name, long space_or_domain, short decomp, long parent, long ext);
DLL_DECL long CreateNaturalDomain(const char *name, int mx, int mn);
DLL_DECL long CreateCustomDomain(const char *name);
DLL_DECL long CreateConstantProperty(const char *name, const char *pcfn, long owner, long domain, int value);
DLL_DECL long CreateVariableProperty(const char *name, const char *pcfn, long owner, long domain);
DLL_DECL long CreateRelation(long context, long source, long destination);
DLL_DECL long CreateConstraintSet(const char *name);
DLL_DECL long CreateConstraint(const char *name, long constraintSet, long context, const char *text);
/////
DLL_DECL long createParametricVariableProperty(const char *name, long owner, const char *text);
DLL_DECL long CreateFormulaSet(const char *name);
DLL_DECL long CreateFormula(const char *name, long constraintSet, long context, const char *text);
DLL_DECL long CreateVariableProperty(const char *name, const char *pcfn, long owner);
/////
#ifndef DOUBLE_MTBDD
//DLL_DECL long AddtoVariableProperty(const char *name, long owner, long value, long domain);
//DLL_DECL long AddtoVariableProperty(long property, long owner, long value, long domain);
DLL_DECL long AddtoVariableProperty(const char *name, const char *n, long owner, long value, long domain);
DLL_DECL long AddtoVariableProperty(long property, const char *n, long owner, long value, long domain);
#else
DLL_DECL long AddtoVariableProperty(const char *name, const char *n, long owner, double value, long domain);
DLL_DECL long AddtoVariableProperty(long property, const char *n, long owner, double value, long domain);
#endif

DLL_DECL long AddtoSimpleFormulaVariableProperty(long property, const char *n, long owner, long srcProperty, long srcOwner);
DLL_DECL long CreateSimpleFormula(const char *pcfn, std::map<long, long> &srcProps, std::map<long, long> &dstProps);
//added by Feng 6/9/2011
DLL_DECL bool DesertFinit_preApply();
DLL_DECL void DesertFinit_Apply(const char *applyConstraints);
DLL_DECL void * DesertFinit_postApply();
DLL_DECL void closeDesertManageInstace();
DLL_DECL long Desert_getRealNoOfConfigurations();
DLL_DECL void Desert_goBack();
DLL_DECL void Desert_goForward();
DLL_DECL bool isDesertBackNavigable();
DLL_DECL bool isDesertForwardNavigable();
DLL_DECL void getDesertAppliedConstraintSet(std::set<std::string> &consSet);
DLL_DECL _int64 getDesertSize();
DLL_DECL void closeDesertLogFile();

#endif // #ifndef desertdll_H
