// -*-C++-*-
// manager.h
// Declaration of the manager class

#ifndef MANAGER_H
#define MANAGER_H

#ifndef BDD_MAX_PATHS
#define BDD_MAX_PATHS 100000
#endif


#include <string>
#ifdef _UNICODE
typedef std::wstring tstring;
#else
typedef std::string tstring;
#endif

#include "core/coredefs.h"
#include "core/cosmic.h"
#include "core/bddmain.h"
#include "core/templ.h"

#define MAX_GENERATIONS 32
typedef CBdd (*fnptrPCMCust) (int, ClRelExpr::RelOp, CDynElement *);		// custom eval functions 
#define ICS_NAME _T("Implicit Constraints")

#include <map>
class CManager : public CCosmic
{
	
private:

	//static space
	CSpaceList baseSpaces;
	CDomainList baseDomains;
	CElementContainerList baseContainers;
	CConstraintSetList baseConstraintSets;
	////
	CFormulaSetList baseFormulaSets;
	CSimpleFormulaList simpleFormulas;
	////

	//dynamic spaces
	CDynSpaceList dynSpaces[MAX_GENERATIONS];
	CDynDomainList dynDomains[MAX_GENERATIONS];
	CDynElementContainerList dynContainers[MAX_GENERATIONS];
	CDynConstraintSetList dynConstraintSets[MAX_GENERATIONS];

	//current generation
	POSITION currentGenerationPosition;

	//list with generations
	static CList<int, int> generations;
	
	//function to get a configuration  number
	static long GetConfNumber();
	
	//table, lookup&cache function for Custom Properties
	CMapStringToPtr fnPCMtable;	
	
	int propertyVectors;
	int propertyEncodingLength;
	FunctionType funcType;
	
	
	_int64 designSpaceSize;

public:
	CManager();
	~CManager();
	static DESERT_API CManager* theInstance;
	//static CPtrList allocatedLists;
	static int unique;
	bool ui_refresh_needed;

public:

	//functions exported through the PCM interface

	DESERT_API virtual CCosmicList* GetContainer(const TCHAR * ContainerName); 
	DESERT_API virtual CCosmicList* GetContainer(CString& ContainerName);
	DESERT_API CSpace *FindSpace(long id);
	DESERT_API CDomain *FindDomain(long id);
	DESERT_API CConstraintSet *FindConstraintSet(long id);
	DESERT_API CElement *FindElement(long id);
	DESERT_API CElement *FindElement(CSpaceList& spaces, long id);
	DESERT_API CElement *FindElement(CDomainList& domains, long id);
	DESERT_API CConstraint *FindConstraint(long id);
	DESERT_API CDynSpace *FindSpace(const CSpace* core);
	DESERT_API CDynSpace *FindSpace(CString& name);
	DESERT_API CDynDomain *FindDomain(const CDomain* core);
	DESERT_API CDynDomain *FindDomain(CString& name);
	DESERT_API CDynConstraintSet *FindConstraintSet(const CConstraintSet* core);
	DESERT_API CDynElement *FindElement(const CElement *core);
	DESERT_API CDynElement *FindElement(CDynSpaceList& spaces, const CElement *core);
	DESERT_API CDynElement *FindElement(CDynDomainList& domains, const CElement *core);
	DESERT_API CDynConstraint *FindConstraint(const CConstraint *core);
	DESERT_API CDynSpaceList& GetSpaces();
	DESERT_API CDynDomainList& GetDomains();
	DESERT_API CDynConstraintSet* GetConstraintSet(ConstraintType type);
	DESERT_API CDynConstraintSetList& GetConstraintSets();
	//debug function which dumps out the current generation to a file
	DESERT_API void Dump(FILE *f, const TCHAR *comment);

	

	
	//functions used when evaluating constraints
	fnptrPCMCust GetPCMCustomFunction(CString &propName);
	ClData Eval(const CCosmic *other) const;
	void SetFunction(FunctionType func);

	
  


	//functions used by the user interface 
	//to create the space
	long CreateSpace(CString &name);
	long CreateSpace(const TCHAR *name);


	long CreateElement(CString &name, long space_or_domain, short decomp, long parent, long ext=0);
	long CreateElement(const TCHAR *name, long space_or_domain, short decomp, long parent, long ext=0);

	long CreateNaturalDomain(CString &name, int mx=0, int mn=0);
	long CreateNaturalDomain(const TCHAR *name, int mx=0, int mn=0);

	long CreateCustomDomain(CString &name);
	long CreateCustomDomain(const TCHAR *name);

	long CreateConstantProperty(CString &name, CString& pcm_fn, long owner, long domain, int value);
	long CreateConstantProperty(const TCHAR *name, const TCHAR *pcm_fn, long owner, long domain, int value);

	long CreateVariableProperty(CString &name, CString& pcm_fn, long owner, long custom_or_natural_domain);
	long CreateVariableProperty(const TCHAR *name, const TCHAR *pcm_fn, long owner, long custom_or_natural_domain);
  
	/////
	//long CreateParametricVariableProperty(CString &name, long owner, CString& text);
	long CreateParametricVariableProperty(const TCHAR *name, long owner,  const TCHAR *text);
	long CreateVariableProperty(CString &name, CString& pcm_fn, long owner);
	long CreateVariableProperty(const TCHAR *name, const TCHAR *pcm_fn, long owner);
	/////
  
	long CreateRelation(long owner, long source, long destination);
	long CreateConstraintSet(CString &name);
	long CreateConstraintSet(const TCHAR *name);

	long CreateConstraint(CString& name, long constraintSet, long context, CString& text);
	long CreateConstraint(const TCHAR *name, long constraintSet, long context, CString& text);

	long CreateConstraint(CString& name, long constraintSet, long context, const TCHAR *text);
	long CreateConstraint(const TCHAR *name, long constraintSet, long context, const TCHAR *text);

#ifndef DOUBLE_MTBDD
	long AddtoVariableProperty(const TCHAR *property, CString &name, long owner, long custom_member_or_natural_value, long custom_or_natural_domain);
	long AddtoVariableProperty(long property, CString &name, long owner, long custom_member_or_natural_value, long custom_or_natural_domain);
	long AddtoVariableProperty(const TCHAR *property, const TCHAR *name, long owner, long custom_member_or_natural_value, long custom_or_natural_domain);
	long AddtoVariableProperty(long property, const TCHAR *name, long owner, long custom_member_or_natural_value, long custom_or_natural_domain);
#else
	long AddtoVariableProperty(const TCHAR *property, CString &name, long owner, double custom_member_or_natural_value, long custom_or_natural_domain);
	long AddtoVariableProperty(long property, CString &name, long owner, double custom_member_or_natural_value, long custom_or_natural_domain);
	long AddtoVariableProperty(const TCHAR *property, const TCHAR *name, long owner, double custom_member_or_natural_value, long custom_or_natural_domain);
	long AddtoVariableProperty(long property, const TCHAR *name, long owner, double custom_member_or_natural_value, long custom_or_natural_domain);
#endif

	long AddtoSimpleFormulaVariableProperty(long property, const TCHAR *name, long owner, long srcProperty, long srcOwner);
	long CreateSimpleFormula(const TCHAR *fnType, CMap<long, long, long, long> &srcProps, CMap<long, long, long, long> &dstProps);
/////
	long CreateFormulaSet(CString &name);
	long CreateFormulaSet(const TCHAR *name);

	long CreateFormula(CString& name, long constraintSet, long context, CString& text);
	long CreateFormula(const TCHAR *name, long constraintSet, long context, CString& text);

	long CreateFormula(CString& name, long constraintSet, long context, const TCHAR *text);
	long CreateFormula(const TCHAR *name, long constraintSet, long context, const TCHAR *text);
/////


	//functions used by the usre interface 
	//when applying constraints

	double ComputeSize() const;
	double ComputeSize(long elid);
	bool VerifyConstraints(CCSetErrDialog * );
	void AnalyseConstraints();
	bool IsDone();
	void ShowStats();
	void GenerateNextHierarchy();
	bool Prune(CDynConstraintSet* set);
	
	typedef void (CManager::*fBuildConfigurations)(BackIfaceFunctions::DBConfigurations *configs, CDynElement *root, CBdd& config, int encLen);
	void * StoreConfigurations(const TCHAR *fname, tstring &errmsg, fBuildConfigurations BuildConfigurations);

	bool HasGenerations();
	long CalcRealNoOfConfigurations();

	void* BuildConfigurationsCallbackFunctionArg;
	int(*BuildConfigurationsCallbackFunction)(void*, const BackIfaceFunctions::DBConfiguration&);
	void BuildConfigurationsCallback(BackIfaceFunctions::DBConfigurations *, CDynElement *root, CBdd& config, int encLen);
	void BuildConfigurations(BackIfaceFunctions::DBConfigurations *, CDynElement *root, CBdd& config, int encLen);
	bool HasConstraints();
	bool HasImplicitConstraints();

	bool Profile(double& dspSize, long& repSize, long& encSize);
	bool Profile(double& dspSize, long& repSize, long& encSize, long& consSize, long& prRepSize, double& prDspSize);
	bool GetSizeInfo(double& dspSize, long& repSize, long& clockTime, CDynConstraintSet* set=NULL);
	_int64 GetDSSize();

  //back/forward navigation
	bool IsBackNavigable();
	void NavigateBack();
	bool IsForwardNavigable();
	void NavigateForward();
	int GetCurrStepsBack();


private:
	void SortConstraints();
	// four variant of prune function
	// operates on uni-space constraints, that are not property dependent
	bool Prune(CDynConstraintSet* set, CDynElement *root);
	// operates on multi-space constraints, that are not property dependent
	bool Prune(CDynConstraintSet* set, CDynSpaceList& spaces);
	// operates on uni-space constraints, that are property dependent (latency/area/throughput)
	bool Prune(CDynConstraintSet* set, CDynElement *root, bool propertyDependent);
	// operates on multi-space constraints, that are property dependent (latency/area/throughput)
	bool Prune(CDynConstraintSet* set, CDynSpaceList& spaces, bool propertyDependent);
	CProperty * GetProperty(long p, long o);
};

/*
this macro declares a variable with the correct value
of the current generation index
*/

#define GET_CURRENTGENERATION \
	if (generations.IsEmpty()) throw new CDesertException( _T("CManager::GET_CURRENTGENERATION MACRO: invalid generation, no generations " ));\
	if (!currentGenerationPosition) throw new CDesertException(_T("CManager::GET_CURRENTGENERATION MACRO: invalid current generation position " ));\
	int currentGeneration = generations.GetAt(currentGenerationPosition) % MAX_GENERATIONS;



#endif // #ifndef MANAGER_H

