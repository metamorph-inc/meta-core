// -*-C++-*-
// dynamic.h
// Declaration of Dynamic classes

#ifndef DYNAMIC_H
#define DYNAMIC_H

#include "core/coredefs.h"
#include "core/bddmain.h"
#include "core/clmain.h"
#include "core/cosmic.h"
#include "core/static.h"
#include "core/utils.h"
#include "common/config.h"

#include <boost/dynamic_bitset.hpp>

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

class CDynFormulaSet;
class CDynFormula;

#define DYN_MANAGER_CHECK(FUNC_NAME)	if(!CManager::theInstance)	throw new CDesertException(CString(FUNC_NAME) + CString(": Manager not instantiated!"))

class  CDynamicObj : public CCosmic
{
protected:
  CStaticObj *core;

protected:
  CDynamicObj();
  CDynamicObj(CStaticObj *core);
public:
	virtual ~CDynamicObj() ;
	DESERT_API const CString& GetName() const;
	DESERT_API operator const TCHAR *() const;
	DESERT_API CStaticObj *GetCore();
};//eo class CDyamicObj


class CDynElementContainer 
{
	//this class should contain all the  behaviour
	//related to  hierarchy , elements, containment
	//and so on 
protected:
	CDynElementList elements;
	CDynElementList rootElements;
public:
	DESERT_API virtual CDynElementList* GetElements(int);
	DESERT_API virtual CDynElementList& GetElements();
	DESERT_API virtual CDynElementList& GetRootElements();
	DESERT_API virtual CDynElementList* GetRootElements(int);
	DESERT_API virtual CDynElement *FindElement(long id);
	DESERT_API virtual CDynElement *FindElement(const TCHAR *name);
	DESERT_API virtual CDynElement *FindElement(const CElement *core);
	virtual CDynElement *RemoveElement(CDynElement *e);
	virtual CDynElement *RemoveElement(long id);
	virtual CDynElement *RemoveElement(const TCHAR *name);
	virtual ~CDynElementContainer();

	
	//for getting static objects from CDynCustomDomain and CDynSpace classes
	virtual CStaticObj* GetCore() = 0;

	//functions defined elsewhere 
	virtual void Clone();
	DESERT_API virtual void Dump(FILE *f);


	virtual  void Clone(CDynElementContainer* from);
	virtual CDynElement *InsertElement(CDynElement *elem);
	
	DESERT_API virtual CType GetType()					=	0;
	DESERT_API virtual operator long() const			=	0;
	DESERT_API virtual operator const TCHAR * () const	=	0;

};//eo CDynElementContainer



class  CDynSpace : public CDynamicObj, public CDynElementContainer
{
protected:

	bool alive;
	int encodingLen;
	int encodingVal;
	int startVar;
	CMapStringToPtr startPropertyVec;     // the start vector of a property - hashed by prop name
	CMapStringToPtr numPropertyVecs;      // the number of vectors of a property - hashed by prop name
	CBdd encoding;

public:
	DESERT_API static CDynSpace *Make(CSpace *c);
	DESERT_API static CDynSpace *Make(CDynSpace *f);
	CDynSpace(CSpace *c);
	virtual ~CDynSpace();
	void DESERT_API SetEncodingLen(int l);
	DESERT_API CBdd GetEncoding();
	DESERT_API bool IsAlive() const;
	DESERT_API virtual CStaticObj * GetCore(); 
  

	DESERT_API double ComputeSize();
	DESERT_API int FindEncodingLen();
	int SetEncodingValue(int enc, int sb);
	DESERT_API CBdd MaptoBdd(CBdd& parentEncoding, int props, ...);
	DESERT_API int FindPropertyEncodingLen(const TCHAR *prop);
	int SetPropertyEncodingValue(const TCHAR *prop, int sb);
	DESERT_API int FindNonOrthElementCount();
	bool Restrict(CBdd& res);

	DESERT_API virtual CType GetType();
	DESERT_API virtual operator long() const;
	DESERT_API virtual operator const TCHAR * () const	;
};//CDynSpace


class  CDynElement : public CDynamicObj
{
protected:
	bool alive;
	CDynDomain *domain;
	CDynSpace *space;
	CDynElement *parent;
	CDynElementList children;
	CDynRelationList relations;
	CDynPropertyList properties;
	int childEncodingLen;                 // the number of bits taken by children after my encoding bits
	int encodingLen;
	int resourceEncodingLen;              // the number of bits required by resource
	int propertyEncodingLen;              // the number of bits required by dyn-variable properties
	int encodingVal;
	int startVar;
	CMapStringToPtr startPropertyVec;     // the start vector of a property - hashed by prop name
	CMapStringToPtr numPropertyVecs;      // the number of vectors of a property - hashed by prop name
	CBdd encoding;
	CBdd baseEnc;
	FunctionType funcType;                // this is the function to be used in Eval
	CString funcProperty;                 // this is the property name to be used in PCM - funcs

	CDynFormulaList formulas;

public:
	CDynElement(CElement *core, CDynElementContainer *container, CDynElement *parent);
  
	
	virtual ~CDynElement();
	void Clone();
	void Clone(CDynElement* from);
	void DESERT_API Dump(FILE *f);

	DESERT_API CDynSpace *GetSpace();
	DESERT_API CDynElement *GetParent();
	DESERT_API CDynElementList& GetChildren();
	DESERT_API CDynRelationList& GetRelations();
	DESERT_API CDynPropertyList& GetProperties();
	DESERT_API CDynFormulaList& GetFormulas();
	DESERT_API CDecomposition GetDecomposition() const;
	DESERT_API operator CDecomposition() const;
	DESERT_API long GetExternal() const;
	DESERT_API CDynElement *FindChild(long id);
	DESERT_API CDynElement *FindChild(const TCHAR *n);
	DESERT_API CDynElement *FindChild(const CElement *c);
	CDynElement *RemoveChild(CDynElement *e);
	CDynElement *RemoveChild(long id);
	CDynElement *RemoveChild(const TCHAR *n);
	CDynElement *InsertChild(CDynElement *e);
	DESERT_API CDynProperty *FindProperty(long i);
	DESERT_API CDynProperty *FindProperty(const TCHAR *n);	
	////
	DESERT_API CDynFormula *FindFormula(const TCHAR *n);
	////
	CDynProperty *RemoveProperty(CDynProperty *e);
	CDynProperty *RemoveProperty(long i);
	CDynProperty *RemoveProperty(const TCHAR *n);
	CDynProperty *InsertProperty(CDynProperty *e);
	DESERT_API CDynRelation *FindRelation(long i);
	CDynRelation *RemoveRelation(CDynRelation *e);
	CDynRelation *RemoveRelation(long i);
	CDynRelation *InsertRelation(CDynRelation *e);
	void SetFunction(FunctionType f);
	void SetFunction(FunctionType f, const CString& pn);
	DESERT_API void SetEncodingLen(int l);
	DESERT_API CBdd GetEncoding();
	DESERT_API CBdd GetBaseEncoding();

	bool IsAlive() const;
	DESERT_API CCosmic* Parent();
	DESERT_API CCosmic* Space();				

	double ComputeSize();
	DESERT_API int FindEncodingLen();
	int SetEncodingValue(int enc, int sb);
	DESERT_API CBdd MaptoBdd(CBdd& parentEncoding, int prop, ...);
	DESERT_API CBdd MaptoBdd_0(CBdd& parentEncoding, int prop, va_list ap);
	bool Restrict(CBdd& res);
	bool NotRedundant(const boost::dynamic_bitset<>& enc);
	DESERT_API CCosmic* Domain();
	DESERT_API CCosmicList* Children();
	ClData Eval(const CCosmic *other) const;
	//add by Feng
	ClData Eval(ClRelExpr::RelOp, const CCosmic *other) const; 
	ClData Eval(ClAddExpr::AddOp op, const CCosmic *other) const;
	ClData Eval(ClAddExpr::AddOp op, const CBdd& left) const;
	ClData Eval(ClMulExpr::MulOp op, const CCosmic *other) const;
	ClData Eval() const;
	bool IsNumericFunc() const; 

#ifndef DOUBLE_MTBDD
	ClData Eval(ClRelExpr::RelOp, int right) const;
#else
	ClData Eval(ClRelExpr::RelOp, double right) const;
#endif
	//DESERT_API void GetVectors(const TCHAR *prop, CUIntArray& vecList);

	
	void BuildConfiguration(BackIfaceFunctions::DBConfiguration *, CBdd& enc);
	int FindPropertyEncodingLen(const TCHAR *prop);
	int SetPropertyEncodingValue(const TCHAR *prop, int sb);
	int FindNonOrthElementCount();
	DESERT_API int GetCount() const;                 // recursively counts all the children under this element

	bool FindRecurse(CDynElement *toFind, int& count);

	//Added by Feng
	DESERT_API CString GetFuncProperty() const;
	//

	//11/30/2011 Added
	ClData Eval(int limit, const TCHAR *prop, FunctionType fn) const;
private:
	void Complement(CDynElementList& list, CDynElementList& complement); // complement = children - list
//	void AddToArray(int val, CUIntArray& array) const;

	//PCM Functions - Natural Domains

	CBdd PCMAdd(int limit, ClRelExpr::RelOp op);
	CBdd PCMMul(int limit, ClRelExpr::RelOp op);
	CBdd PCMMin(int limit, ClRelExpr::RelOp op);
	CBdd PCMMax(int limit, ClRelExpr::RelOp op);
	CBdd PCMAmed(int limit, ClRelExpr::RelOp op);
	CBdd PCMGmed(int limit, ClRelExpr::RelOp op);
	CBdd PCMCust(int limit, ClRelExpr::RelOp op);
	CBdd PCMNop(int limit, ClRelExpr::RelOp op);
	CBdd PCMNone(int limit, ClRelExpr::RelOp op);


	CBdd PCMAdd_0(int limit, const TCHAR *propName);
	CBdd PCMMul_0(int limit, const TCHAR *propName);
	CBdd PCMMin_0(int limit, const TCHAR *propName);
	CBdd PCMMax_0(int limit, const TCHAR *propName);
	CBdd PCMAmed_0(int limit, const TCHAR *propName);
	CBdd PCMGmed_0(int limit, const TCHAR *propName);
	CBdd PCMNop_0(int limit, const TCHAR *propName);

	//PCM Functions - Custom Domains
	CBdd PCMNop(const CCosmic * other);
	CBdd PCMNop_0(const CCosmic * other, const TCHAR * propName);

	CBdd PCMGen_0(int limit, const TCHAR *propName);

	CBdd PCMGen_decompAnd(int limit, const TCHAR *propName, FunctionType f);
	CBdd PCMGen_decompOr(int limit, const TCHAR *propName, FunctionType f);
	CBdd PCMGen_decompLeaf(int limit, const TCHAR *propName, FunctionType f);
	CBdd PCMGen_SimpleFormula(int limit, CDynPropertyList &propList, FunctionType f);
};



class  CDynProperty : public CDynamicObj
{
protected:
  CDynDomain *domain;
  CDynElement *owner;
  CDynPropertyList properties;

protected:
	CDynProperty(CProperty *c, CDynDomain *d, CDynElement *o) ;  
	CDynProperty(CProperty *c, CDynElement *o) ;
public:
	static CDynProperty *Make(CProperty *core, CDynDomain *domain, CDynElement *owner);
	virtual ~CDynProperty();
	DESERT_API CDynDomain *GetDomain();
	DESERT_API CDynElement *GetOwner();
	DESERT_API virtual bool GetValue(int& r) const;
	DESERT_API virtual bool GetValue(CDynElement*& r) const;
	DESERT_API virtual int FindEncodingLen();
	DESERT_API virtual int FindDomainSize();
	virtual int SetEncodingValue(int sb, int encLen);
	virtual int SetEncodingValue(int sv);
	DESERT_API virtual CBdd MaptoBdd(CBdd& parentEncoding);
	DESERT_API virtual CBdd MapValuetoBdd(CBdd& parentEncoding);

	// virtual CBdd MaptoBdd();              // for constant properties
	DESERT_API virtual double ComputeSize();
	DESERT_API virtual int GetVector();
	DESERT_API virtual void Dump(FILE *f) = 0;
	virtual void Restrict(CBdd& res);

	DESERT_API virtual CDynProperty *RemoveProperty(CDynProperty *e);
	DESERT_API virtual CDynProperty *InsertProperty(CDynProperty *e);
	DESERT_API virtual CDynPropertyList& GetProperties();
};



class  CDynConstantProperty : public CDynProperty
{
private:
  int vector;

public:
  CDynConstantProperty(CConstantProperty *c, CDynDomain* d, CDynElement *o);
  virtual ~CDynConstantProperty();
  DESERT_API bool GetValue(int& r) const;
  DESERT_API int GetVector();
  int SetEncodingValue(int vec);
  DESERT_API CBdd MaptoBdd(CBdd& parentEncoding);
  virtual void Dump(FILE *f);
};


class  CDynVariableProperty : public CDynProperty
{
private:
	CDynElementList range;
#ifndef DOUBLE_MTBDD
	CIntegerList naturalRange;
#else
	CDoubleList naturalRange;
#endif
	int startVar;
	int encodingLen;
	int vector;                           // when we are a natural domain member this needs to be done
	CBdd encoding;
	
	bool isParametric;
public:
  CDynVariableProperty(CVariableProperty *core, CDynDomain *domain, CDynElement *owner);
  CDynVariableProperty(CVariableProperty *core, CDynElement *owner);
  virtual ~CDynVariableProperty();
  DESERT_API int FindEncodingLen();
  int SetEncodingValue(int sb, int encLen);
  int SetEncodingValue(int sv);         // this works for natural variable properties
  double ComputeSize();
  DESERT_API CDynElementList& GetRange();
#ifndef DOUBLE_MTBDD
  DESERT_API CIntegerList& GetNaturalRange();
#else
  DESERT_API CDoubleList& GetNaturalRange();
#endif
  DESERT_API CBdd MapValuetoBdd(CBdd& parentEncoding);
  void BuildConfiguration(BackIfaceFunctions::DBConfiguration* cfg, CBdd& enc, long elementID);
  DESERT_API CBdd GetEncoding(CDynElement *rangeElement); // find the encoding of a given resource (element)
  DESERT_API virtual void Dump(FILE *f);
  void Restrict(CBdd& res);
  DESERT_API bool IsParametric();
};




class  CDynDomain : public CDynamicObj
{
public:
  DESERT_API static CDynDomain* Make(CDomain *c);
  DESERT_API static CDynDomain* Make(CDynDomain *f);
  CDynDomain(CDomain *core);
  virtual ~CDynDomain();

public:
	DESERT_API virtual int GetSize() const									;
	DESERT_API virtual void Clone()										= 0;//{};
	DESERT_API virtual void Clone(CDynDomain* from)						= 0;//{};		
	DESERT_API virtual CDynElement *FindElement(long id);
	DESERT_API virtual CDynElement *FindElement(const TCHAR *name);
	DESERT_API virtual CDynElement *FindElement(const CElement *core);
	virtual CDynElement *RemoveElement(CDynElement *elem);
	virtual CDynElement *RemoveElement(long id);
	virtual CDynElement *RemoveElement(const TCHAR *name);
	virtual CDynElement *InsertElement(CDynElement *elem);
	DESERT_API virtual bool IsAlive() const;
	DESERT_API virtual int IndexElement(CDynElement *elem);
	DESERT_API virtual void Dump(FILE *f)											= 0;
};

  
class  CDynNaturalDomain : public CDynDomain
{
protected:
  int minVal;
  int maxVal;

public:
  CDynNaturalDomain(CNaturalDomain *c);
  CDynNaturalDomain(CNaturalDomain *c, int mx, int mn);
  virtual void Clone();
  virtual void Dump(FILE *f);
  virtual void Clone(CDynDomain* from);		
  virtual ~CDynNaturalDomain();
  DESERT_API int GetSize() const;
  DESERT_API long GetValue(CBdd& enc, int sv, int encLen);
};



class  CDynCustomDomain : public CDynDomain, public CDynElementContainer
{

public:
	CDynCustomDomain(CCustomDomain *c);
	virtual ~CDynCustomDomain();
	DESERT_API int GetSize() const;
	DESERT_API CDynElement *FindElement(long i);
	DESERT_API CDynElement *FindElement(const TCHAR *n);
	DESERT_API CDynElement *FindElement(const CElement *c);
	CDynElement *RemoveElement(CDynElement *e);
	CDynElement *RemoveElement(long i);
	CDynElement *RemoveElement(const TCHAR *n);
	CDynElement *InsertElement(CDynElement *e);
	DESERT_API int IndexElement(CDynElement *elem);
	DESERT_API virtual CStaticObj * GetCore();
	DESERT_API virtual CType GetType();
	DESERT_API virtual operator long() const;
	DESERT_API virtual operator const TCHAR * () const;
	DESERT_API virtual void Dump(FILE *f);
	DESERT_API long GetExternal(CBdd& enc, int sv, int encLen);
	DESERT_API CBdd GetEncoding(CDynElement *rangeElement, int sv, int encLen);
	virtual void Clone();
	void Clone(CDynDomain* from);
	
	
};


class  CDynConstraintSet : public CDynamicObj
{
protected:
  bool alive;
  CDynConstraintList constraints;

public:
  DESERT_API static CDynConstraintSet* Make(CConstraintSet *c);
  DESERT_API static CDynConstraintSet* Make(CDynConstraintSet *f);
  DESERT_API CDynConstraintSet(CConstraintSet *c);
  virtual ~CDynConstraintSet();
  DESERT_API CDynConstraintList& GetConstraints();
  DESERT_API CDynConstraint *FindConstraint(long i);
  DESERT_API CDynConstraint *FindConstraint(const TCHAR *n);
  DESERT_API CDynConstraint *FindConstraint(const CConstraint *c);
  CDynConstraint *RemoveConstraint(CDynConstraint *e);
  CDynConstraint *RemoveConstraint(long i);
  CDynConstraint *RemoveConstraint(const TCHAR *n);
  DESERT_API CDynConstraint *InsertConstraint(CDynConstraint *e);
  DESERT_API void RemoveAll();
  DESERT_API bool IsEmpty() const;
  DESERT_API bool IsAlive() const;
  
  void Clone();
  void Clone(CDynConstraintSet* from);
  DESERT_API CBdd MaptoBdd();
  DESERT_API CDynElement *FindAffectedSpace(CDynSpaceList& spaces, bool& propertyDependence);
  bool Restrict();  
};

class  CDynConstraint : public CDynamicObj
{
  friend class CDynConstraintSet;
protected:
  bool alive;
  bool wasApplied;                      // keep track if a constraint has been applied before
  bool valid;
  CDynConstraintSet *set;
  CDynElement *context;

public:
  CDynConstraint(CConstraint *c, CDynConstraintSet *s, CDynElement* e);
  virtual ~CDynConstraint();
  DESERT_API CDynConstraintSet *GetSet();
  DESERT_API CDynElement *GetContext();
  DESERT_API CString& GetText();
  DESERT_API ClConstraint *GetExpression();
  DESERT_API bool IsAlive() const;
  DESERT_API bool WasApplied() const;
  DESERT_API void SetApplied();
  DESERT_API bool IsValid() const;
  DESERT_API void SetInvalid();
  bool IsPropertyDependent() const;
  DESERT_API CBdd MaptoBdd();
  void FillDependency(CDynamicObjList& dependency);
  bool Restrict();
};


class  CDynRelation : public CDynamicObj
{
protected:
  CDynElement *context;
  CDynElement *source;
  CDynElement *destination;

public:
  CDynRelation(CRelation *core, CDynElement *c, CDynElement *s, CDynElement *d);
  virtual ~CDynRelation();
  DESERT_API CDynElement *GetContext();
  DESERT_API CDynElement *GetSource();				
  DESERT_API CDynElement *GetDestination();
};



///CDynFormulaSet
class  CDynFormulaSet : public CDynamicObj
{
protected:
  bool alive;
  CDynFormulaList formulas;

public:
  DESERT_API static CDynFormulaSet* Make(CFormulaSet *c);
  DESERT_API static CDynFormulaSet* Make(CDynFormulaSet *f);
  DESERT_API CDynFormulaSet(CFormulaSet *c);
  virtual ~CDynFormulaSet();
  DESERT_API CDynFormulaList& GetFormulas();
  DESERT_API CDynFormula *FindFormula(long i);
  DESERT_API CDynFormula *FindFormula(const TCHAR *n);
  DESERT_API CDynFormula *FindFormula(const CFormula *c);
  CDynFormula *RemoveFormula(CDynFormula *e);
  CDynFormula *RemoveFormula(long i);
  CDynFormula *RemoveFormula(const TCHAR *n);
  DESERT_API CDynFormula *InsertFormula(CDynFormula *e);
  DESERT_API void RemoveAll();
  DESERT_API bool IsEmpty() const;
  DESERT_API bool IsAlive() const;
  
  void Clone();
  void Clone(CDynFormulaSet* from);
  DESERT_API CBdd MaptoBdd();
  DESERT_API CDynElement *FindAffectedSpace(CDynSpaceList& spaces, bool& propertyDependence);
  bool Restrict();  
};

////CDynFormula
class  CDynFormula : public CDynamicObj
{
  friend class CDynFormulaSet;
protected:
  bool alive;
  CDynFormulaSet *set;
  CDynElement *context;

  CDynFormula(CFormula *c, CDynElement* e);
  CDynFormula(CFormula *c, CDynFormulaSet *s, CDynElement* e);
public:
  static CDynFormula *Make(CFormula *core, CDynElement *owner);
  virtual ~CDynFormula();
  DESERT_API CDynFormulaSet *GetSet();
  DESERT_API CDynElement *GetContext();
  DESERT_API CString& GetText();
  DESERT_API ClFormula *GetExpression();
  DESERT_API bool IsAlive() const;
  bool IsPropertyDependent() const;
  DESERT_API CBdd MaptoBdd();
  void FillDependency(CDynamicObjList& dependency);
  bool Restrict();
};


#endif //ifndef DYNAMIC_H
