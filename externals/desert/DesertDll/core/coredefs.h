// -*-C++-*-
// coredefs.h

#ifndef DEFS_H
#define DEFS_H

#include <afxtempl.h>

// classes
class CCosmic;
class CManager;
class CStaticObj;
class CSpace;
class CElement;
class CProperty;
class CConstantProperty;
class CVariableProperty;
class CConstraintSet;
class CConstraint;
///
class CFormulaSet;
class CFormula;
///
class CDomain;
class CNaturalDomain;
class CCustomDomain;
class CRelation;
class CAssignment;
///
class CSimpleFormula;
///


class CDynamicObj;
class CDynSpace;
class CDynElement;
class CDynProperty;
class CDynConstantProperty;
class CDynVariableProperty;
class CDynConstraintSet;
class CDynConstraint;
class CDynDomain;
class CDynNaturalDomain;
class CDynCustomDomain;
class CDynRelation;
class CDiGraph;
class CGraphNode;
class CGraphPath;
class ClData;
class ClContext;
class ClFunction;
class CConfigurationStore;
class CConfigurationRoot;
class CConfiguration;
class CComponent;
class CElementContainer;
class CDynElementContainer;

/////
class CDynFormulaSet;
class CDynFormula;
/////

// project specific typedefs
typedef CTypedPtrList<CPtrList, CCosmic*> CCosmicList;
typedef CTypedPtrList<CPtrList, CStaticObj*> CStaticObjList;
typedef CTypedPtrList<CPtrList, CSpace*> CSpaceList;
typedef CTypedPtrList<CPtrList, CElement*> CElementList;
typedef CTypedPtrList<CPtrList, CProperty*> CPropertyList;
typedef CTypedPtrList<CPtrList, CConstantProperty*> CConstantPropertyList;
typedef CTypedPtrList<CPtrList, CVariableProperty*> CVariablePropertyList;
typedef CTypedPtrList<CPtrList, CConstraintSet*> CConstraintSetList;
typedef CTypedPtrList<CPtrList, CConstraint*> CConstraintList;
///////////////////////
typedef CTypedPtrList<CPtrList, CFormulaSet*> CFormulaSetList;
typedef CTypedPtrList<CPtrList, CFormula*> CFormulaList;
typedef CTypedPtrList<CPtrList, CSimpleFormula*> CSimpleFormulaList;
///////////////////////
typedef CTypedPtrList<CPtrList, CDomain*> CDomainList;
typedef CTypedPtrList<CPtrList, CNaturalDomain*> CNaturalDomainList;
typedef CTypedPtrList<CPtrList, CCustomDomain*> CCustomDomainList;
typedef CTypedPtrList<CPtrList, CRelation*> CRelationList;
typedef CTypedPtrList<CPtrList, CDynamicObj*> CDynamicObjList;
typedef CTypedPtrList<CPtrList, CDynSpace*> CDynSpaceList;
typedef CTypedPtrList<CPtrList, CDynElement*> CDynElementList;
typedef CTypedPtrList<CPtrList, CDynProperty*> CDynPropertyList;
typedef CTypedPtrList<CPtrList, CDynConstantProperty*> CDynConstantPropertyList;
typedef CTypedPtrList<CPtrList, CDynVariableProperty*> CDynVariablePropertyList;
typedef CTypedPtrList<CPtrList, CDynConstraintSet*> CDynConstraintSetList;
typedef CTypedPtrList<CPtrList, CDynConstraint*> CDynConstraintList;
typedef CTypedPtrList<CPtrList, CDynDomain*> CDynDomainList;
typedef CTypedPtrList<CPtrList, CDynNaturalDomain*> CDynNaturalDomainList;
typedef CTypedPtrList<CPtrList, CDynCustomDomain*> CDynCustomDomainList;
typedef CTypedPtrList<CPtrList, CDynRelation*> CDynRelationList;
typedef CTypedPtrList<CPtrList, CGraphNode*> CGraphNodeList;
typedef CTypedPtrList<CPtrList, CGraphPath*> CGraphPathList;
typedef CTypedPtrList<CPtrList, CElementContainer*> CElementContainerList;
typedef CTypedPtrList<CPtrList, CDynElementContainer*> CDynElementContainerList;
typedef CTypedPtrList<CPtrList, CAssignment*> CAssignmentList;

typedef CTypedPtrList<CPtrList, CDynFormulaSet*> CDynFormulaSetList;
typedef CTypedPtrList<CPtrList, CDynFormula*> CDynFormulaList;

typedef CList<int, int> CIntegerList;
typedef CList<double, double> CDoubleList;

typedef enum
{
  typeCosmic=0,
  typeManager,
  typeStatic,
  typeSpace,
  typeElement,
  typeProperty,
  typeConstantProperty,
  typeVariableProperty,
  typeConstraintSet,
  typeConstraint,
  typeDomain,
  typeNaturalDomain,
  typeCustomDomain,
  typeRelation,
  typeAssignment,
  typeDynamic,
  typeDynSpace,
  typeDynElement,
  typeDynProperty,
  typeDynConstantProperty,
  typeDynVariableProperty,
  typeDynConstraintSet,
  typeDynConstraint,
  typeDynDomain,
  typeDynNaturalDomain,
  typeDynCustomDomain,
  typeDynRelation,
  /////
	typeFormulaSet,
	typeFormula,
	typeDynFormulaSet,
	typeDynFormula,
	typeSimpleFormula,
	typeDynSimpleFormula
/////
} CType;

typedef enum
{
  decompLeaf=0,
  decompAnd,
  decompOr
} CDecomposition;

typedef enum
{
  typeUndef=0,
  typeUniSpace,
  typeBiSpace,
  typeUniDomain,
  typeSpaceDomain,
  typeBiSpaceDomain,
  typeBiDomain,
  typeBiDomainSpace,
  typeBiSpaceBiDomain
} ConstraintType;

typedef enum
{
  Undefined = 0, Integer, Double, String, Boolean,
  Cosmic, CosmicList, Bdd
} ClDataType;

typedef enum
{
  funcNofunc=0,
  funcLatency,
  funcArea,
  funcPower,
  funcImplementedBy,
//  funcAssignedTo,
  funcSystemMode,
  funcPCMAdd,
  funcPCMMul,
  funcPCMAmed,
  funcPCMGmed,
  funcPCMMin,
  funcPCMMax,
  funcPCMCust,
  funcPCMNone,
  funcPCMNop,
  funcPCMOr
} FunctionType;

// global macro defs
//#ifdef _DEBUG
//#define new DEBUG_NEW
//#endif

#define BDD_MAX_PATHS 100000
#define MAX_CONFIGURATIONS 1000000         // how many configurations can we deal with

#ifdef DESERT_EXPORTS
#define DESERT_API __declspec(dllexport)
#else
#define DESERT_API __declspec(dllimport)
#endif

#define MY_DECLARE_DYNAMIC(class_name) \
protected: \
	static CRuntimeClass* PASCAL _GetBaseClass(); \
public: \
	static const AFX_DATA CRuntimeClass class##class_name; \
	virtual DESERT_API CRuntimeClass* GetRuntimeClass() const; \


#include "common/error.h"

//error handling with exceptions

class CDesertException : public CException
{
	DECLARE_DYNAMIC(CDesertException)
	bool fatal;
	TCHAR *what;
	TCHAR *constraint_name;

public:
//	MY_DECLARE_DYNAMIC(CDesertException)

	DESERT_API CDesertException();			
	DESERT_API CDesertException(bool _fatal);
	DESERT_API CDesertException(const TCHAR * _what);
	DESERT_API CDesertException(const TCHAR * _what, const TCHAR * _name);
	DESERT_API CDesertException(const CString _what);
	DESERT_API CDesertException(const CString _what, const CString _name);
	DESERT_API CDesertException(bool _fatal, const TCHAR * _what);
	DESERT_API CDesertException(bool _fatal, const CString _what);
	DESERT_API BOOL GetErrorMessage( LPTSTR lpszError, UINT nMaxError, PUINT pnHelpContext = NULL)	;
	DESERT_API CString GetErrorMessage();
	DESERT_API CString GetConstraintName();
	DESERT_API bool Fatal();
	DESERT_API ~CDesertException();						
};

class CDesertNoneElementException : public CDesertException
{
	DECLARE_DYNAMIC(CDesertNoneElementException)
	TCHAR *element;
public:
	DESERT_API CDesertNoneElementException();
	DESERT_API CDesertNoneElementException(const TCHAR * _what);
};
#endif // #ifndef DEFS_H