// -*-C++-*-
// static.h
// Declaration of the static classes

#ifndef STATIC_H
#define STATIC_H

#include "core/coredefs.h"
#include "core/cosmic.h"
#include "mcl/cldef.h"

/*
	Base class for all static objects.
	Extends the CCosmic class with name.
*/
class  CStaticObj : public CCosmic
{
protected:
	CString name;
	//iface requirements
	long iface_id;
	long iface_external_id;



	/*
		Basic constructor, not meant to use it.
	*/
	CStaticObj();
	/*
		Construction with names
	*/
	
	DESERT_API CStaticObj(CString& n);
	DESERT_API CStaticObj(CString& n, long id, long e_id);
	
	DESERT_API CStaticObj(const TCHAR* n);
	DESERT_API CStaticObj(const TCHAR* n, long id, long e_id);



public:
	/*
		Getters for name
	*/
	DESERT_API const CString& GetName() const;
	DESERT_API operator const TCHAR *() const;
	/*
		Getters for Ids
	*/
	DESERT_API long GetIfaceId() const;
	DESERT_API long GetIfaceExternalId() const;

	/*
		Destructor. Does nothing.
	*/
	virtual ~CStaticObj();

};//eo class CStaticObj

/*
	Container base class for CElements.
	Implements functionalities like inserting, finding, removing 
	elements(nodes)from a tree(graph)
*/
class  CElementContainer {
protected:

	CElementList elements;							//list of all the elements in the tree
	CElementList rootElements;						//list of root elements
public:
	/*
		Pointer and Reference getters of elements and root elements
	*/
	DESERT_API virtual CElementList& GetElements();
	DESERT_API virtual CElementList* GetElements(int);
	DESERT_API virtual CElementList& GetRootElements();
	DESERT_API virtual CElementList* GetRootElements(int);
	/*
		Element finders by id, name
	*/
	DESERT_API virtual CElement *FindElement(long i);
	DESERT_API virtual CElement *FindElement(const TCHAR *n);
	virtual CElement *RemoveElement(CElement *e);
	virtual CElement *RemoveElement(long id);
	virtual CElement *RemoveElement(const TCHAR *n);
	virtual CElement *InsertElement(CElement *e);
	virtual ~CElementContainer();
	/*
		Pure virtual functions:
			- GetType() will return the type of the instances of derived classes
			- operator long() will return the id of the instances of derived classes;
			- operator const TCHAR * will return the name of the instances of derived classes;
	*/
	virtual CType GetType()					=	0;
	virtual operator long() const			=	0;
	virtual operator const TCHAR * () const	=	0;
};//eo class CElementContainer

/*
	CSpace class for the static version of the designspace
	It's derivated from CElementContainer and CStaticObj
*/

class  CSpace : public CStaticObj, public CElementContainer
{

public:
	/*
		Construction
	*/
	DESERT_API CSpace(CString& n) ; 
	CSpace(const TCHAR *n) ;
	
	CSpace(CString &n, long id, long e_id) ;
	CSpace(const TCHAR *n, long id, long e_id);

	virtual ~CSpace();
  

public:
	/*
		ComputeSize function, computes the size of the space
	*/
	double ComputeSize() const;
	/*
		Implementations of pure virtual functions in base class
		CElementContainer.
		These are only wrappers because of multipleinheritence:
			If a CSpace is accessed as a CElementContainer,
			it's type, name, and id should be accessible 
		
	*/
	DESERT_API virtual CType GetType()	;
	DESERT_API virtual operator long() const;
	DESERT_API virtual operator const TCHAR * () const;

};//eo class CSpace

	/*
		CElement class - Basic static building block
		Are contained hierarchically in CElementContainers
		can form either spaces or customdomains

	*/

class CElement : public CStaticObj
{
protected:
	CElementContainer * container;	//the container object, either space or customdomain
	CElement *parent;				//the parent element. Null,if this is the root element	
	CElementList children;			//chidlren elements
	CRelationList relations;        //all connections within the context of this element
	CDecomposition decomposition;	//decomposition(and/or/leaf)
	CPropertyList properties;		//list of the properties
	long external;					//external value set by the API functions
	CFormulaList formulas;          //list of the formula for parametric Property

public:
	/*
		Construction of static elements:
	*/
	DESERT_API CElement(CString& n, CElementContainer *c, CDecomposition d, CElement *p, long e=0);
	CElement(const TCHAR *n, CElementContainer *c, CDecomposition d, CElement *p, long e=0);
	CElement(CString& n, CElementContainer *c, CDecomposition d, CElement *p, long id, long e_id);
	CElement(const TCHAR *n, CElementContainer *c, CDecomposition d, CElement *p, long id, long e_id);
		
	/*
		Destruction of static elements
	*/
	virtual ~CElement();
	/*
		Various getter functions/
	*/

	DESERT_API	CElementContainer * GetContainer();
	DESERT_API	CElement *GetParent();
	DESERT_API	CElementList& GetChildren();
	DESERT_API	CRelationList& GetRelations();
	DESERT_API	CPropertyList& GetProperties();
	DESERT_API	CDecomposition GetDecomposition() const;
	DESERT_API	operator CDecomposition() const;
	DESERT_API	long GetExternal() const;

	DESERT_API	CCosmic* Parent();
	DESERT_API	CCosmicList* Children();

	DESERT_API CFormulaList& GetFormulas();

	/*
		Element find/remove/insert in the subtree rooted at this
	*/
	DESERT_API	CElement	*FindChild(long i);
	DESERT_API	CElement	*FindChild(const TCHAR *n);
	CElement	*RemoveChild(CElement *e);
	CElement	*RemoveChild(long i);
	CElement	*RemoveChild(const TCHAR *n);
	DESERT_API	CElement	*InsertChild(CElement *e);

	/*
		Property manipulation
	*/
	DESERT_API CProperty	*FindProperty(long i);
	DESERT_API CProperty	*FindProperty(const TCHAR *n);
	CProperty	*RemoveProperty(CProperty *e);
	CProperty	*RemoveProperty(long i);
	CProperty	*RemoveProperty(const TCHAR *n);
	DESERT_API	CProperty	*InsertProperty(CProperty *e);
	
	DESERT_API	CFormula	*InsertFormula(CFormula *f);

	DESERT_API	void	InsertPropertyFuncTable(CString p, CString f);
	/*
		Relation manipulation
	*/
	DESERT_API	CRelation	*FindRelation(long i);
	CRelation	*RemoveRelation(CRelation *e);
	CRelation	*RemoveRelation(long i);
	DESERT_API	CRelation	*InsertRelation(CRelation *e);

	double ComputeSize() const;
	

};//eo class CElement

/*
	CAssignment - static class for VariableProperty Assignments
*/

class CAssignment : public CStaticObj
{
	CElement * e_value;
	long	l_value; //if e_value is null;
#ifdef DOUBLE_MTBDD
	double d_value;
#endif

public:
	CAssignment(CElement * value, CString &name, long id, long e_id);
	CAssignment(CElement * value, const TCHAR *name, long id, long e_id);
	CAssignment(CElement * value, const TCHAR *name);
	DESERT_API CElement * GetEValue();
#ifndef DOUBLE_MTBDD
	CAssignment(long value, CString &name, long id, long e_id);
	CAssignment(long value, const TCHAR *name, long id, long e_id);
	CAssignment(long value, const TCHAR *name);
	DESERT_API long GetLValue();
#else
	CAssignment(double value, CString &name, long id, long e_id);
	CAssignment(double value, const TCHAR *name, long id, long e_id);
	CAssignment(double value, const TCHAR *name);
	DESERT_API double GetLValue();
#endif
};

/*
	CDomain - static class for domains
*/

class CDomain : public CStaticObj
{
public:
  DESERT_API CDomain(CString& n);
  DESERT_API CDomain(const TCHAR *n);
  DESERT_API CDomain(CString& n, long id, long e_id);
  DESERT_API CDomain(const TCHAR *n, long id, long e_id);
  virtual ~CDomain();

public:
	virtual int GetSize() const = 0;
	
	DESERT_API virtual CElement *FindElement(long i);
	DESERT_API virtual CElement *FindElement(const TCHAR *n);
	DESERT_API virtual CElement *RemoveElement(CElement *e);
	DESERT_API virtual CElement *RemoveElement(long i);
	DESERT_API virtual CElement *RemoveElement(const TCHAR *n);
	DESERT_API virtual CElement *InsertElement(CElement *e);

};


class CNaturalDomain : public CDomain
{
protected:
	  int minVal;
	  int maxVal;

public:
	DESERT_API CNaturalDomain(CString& name, int mx=1024, int mn=0) ;
	CNaturalDomain(const TCHAR *name, int mx=1024, int mn=0);
	CNaturalDomain(int id, int e_id, CString& name, int mx=1024, int mn=0) ;
	CNaturalDomain( int id, int e_id, const TCHAR *name, int mx=1024, int mn=0);

	virtual ~CNaturalDomain();

public:
	DESERT_API	int GetSize() const	;
	DESERT_API	int GetMax() const	;
	DESERT_API	int GetMin() const	;
};

class  CCustomDomain : public CDomain, public CElementContainer
{
public:
	DESERT_API CCustomDomain(CString& n);
	CCustomDomain(const TCHAR *n);
	CCustomDomain(CString& n, long id, long e_id);
	CCustomDomain(const TCHAR *n, long id, long e_id);
	virtual ~CCustomDomain();
  
public:
	int GetSize() const;

	DESERT_API CElement *FindElement(long i);
	DESERT_API CElement *FindElement(const TCHAR *n);
	CElement *RemoveElement(CElement *e);
	CElement *RemoveElement(long i);
	CElement *RemoveElement(const TCHAR *n);
	CElement *InsertElement(CElement *e);

	virtual CType GetType();
	virtual operator long () const;
	virtual operator const TCHAR * () const;
};//eo CCustomDomain


/*
	class static Property

*/
class  CProperty : public CStaticObj
{
protected:
  CDomain *domain;						//the domain this property belongs to
  CElement *owner;						//the owner element
  CString pcm_fn;                       //the composition function -- built-in or custom

protected:
	/*
		Construction, destruction
	*/
	CProperty(const TCHAR *n, const TCHAR *p, CDomain *d, CElement *o);
	CProperty(CString &n, const TCHAR *p, CDomain *d, CElement *o);	
	CProperty(const TCHAR *n, const TCHAR *p, CDomain *d, CElement *o, long id, long e_id);
	CProperty(CString &n, const TCHAR *p, CDomain *d, CElement *o, long id, long e_id);
	
	CProperty(const TCHAR *n, const TCHAR *p,CElement *o);
	CProperty(CString &n, const TCHAR *p, CElement *o);
	CProperty(const TCHAR *n, const TCHAR *p, CElement *o, long id, long e_id);
	CProperty(CString &n, const TCHAR *p, CElement *o, long id, long e_id);

public:
	virtual ~CProperty();
	DESERT_API CDomain *GetDomain();
	DESERT_API CElement *GetOwner();
	DESERT_API virtual bool GetValue(int& r)const;
	DESERT_API virtual bool GetValue(CElement*& r) const;	
	DESERT_API bool IsNatural() const;
	virtual double ComputeSize() const;
	DESERT_API const FunctionType GetFuncType() const;
};


class  CConstantProperty : public CProperty
{

protected:
	int value;

public:
	DESERT_API CConstantProperty(CString& n, CString& p, CDomain* d, CElement *o, int v);
	CConstantProperty(const TCHAR *n, const TCHAR *p, CDomain* d, CElement *o, int v);
	CConstantProperty(CString& n, CString& p, CDomain* d, CElement *o, int v, long id, long e_id);
	CConstantProperty(const TCHAR *n, const TCHAR *p, CDomain* d, CElement *o, int v, long id, long e_id);
	CConstantProperty::~CConstantProperty();

public:
	DESERT_API bool GetValue(int& r) const;
};


class  CVariableProperty : public CProperty
{
protected:
	CElementList range;

#ifndef DOUBLE_MTBDD
	CIntegerList naturalRange;
#else
	CDoubleList naturalRange;
#endif

	CAssignmentList assignments;

	bool isParametric;
	CPropertyList properties;

public:

	//reverse mapping of elements to assignments
	CMap<long, long, CAssignmentList*, CAssignmentList*> range_lmap;
#ifndef DOUBLE_MTBDD
	CMap<long, long, CAssignmentList*, CAssignmentList*> natural_range_lmap;
#else
	CMap<double, double, CAssignmentList*, CAssignmentList*> natural_range_lmap;
#endif

	DESERT_API CVariableProperty(CString& n, CString& p, CDomain* d, CElement *o);
	CVariableProperty(const TCHAR *n, const TCHAR *p, CDomain* d, CElement *o);
	CVariableProperty(CString& n, CString& p, CDomain* d, CElement *o, long id, long e_id);
	CVariableProperty(const TCHAR *n, const TCHAR *p, CDomain* d, CElement *o, long id, long e_id);

	DESERT_API CVariableProperty(CString& n, CString& p, CElement *o);
	CVariableProperty(const TCHAR *n, const TCHAR *p, CElement *o);
	CVariableProperty(CString& n, CString& p, CElement *o, long id, long e_id);
	CVariableProperty(const TCHAR *n, const TCHAR *p, CElement *o, long id, long e_id);

	~CVariableProperty();

public:
	double ComputeSize() const;

	long AddToRange(CElement * element, CString &n);
	long AddToRange(CElement * element, const TCHAR *n);	

#ifndef DOUBLE_MTBDD
	long AddToRange(long value, CString &n);
	long AddToRange(long value, const TCHAR *n);
	DESERT_API CIntegerList& GetNaturalRange();
#else
	long AddToRange(double value, CString &n);
	long AddToRange(double value, const TCHAR *n);
	DESERT_API CDoubleList& GetNaturalRange();
#endif

	DESERT_API CElementList& GetRange();
	DESERT_API CPropertyList& GetProperties();
	DESERT_API bool IsParametric();
	void AddToPropertyList(CProperty *prop);
};




class  CConstraintSet : public CStaticObj
{
protected:
  CConstraintList constraints;

public:
	DESERT_API CConstraintSet(CString &n);
	DESERT_API CConstraintSet(const TCHAR *n);
	CConstraintSet(CString &n, long id, long e_id);
	CConstraintSet(const TCHAR *n, long id, long e_id);
	~CConstraintSet();

public:
	DESERT_API CConstraintList& GetConstraints();
	DESERT_API CConstraint *FindConstraint(long i);
	DESERT_API CConstraint *FindConstraint(const TCHAR *n);
	CConstraint *RemoveConstraint(CConstraint *e);
	CConstraint *RemoveConstraint(long i);
	CConstraint *RemoveConstraint(const TCHAR *n);
	DESERT_API CConstraint *InsertConstraint(CConstraint *e);

public:
	bool Verify(CCSetErrDialog *);
	void Analyse();
};


class  CConstraint : public CStaticObj
{
protected:
  ConstraintType consType;
  CConstraintSet *set;
  CElement *context;
  CString text;
  ClConstraint *expression;
  CCosmicList dependency;
  CStringList propertyDependency;
  CSpaceList depSpaces;
  CDomainList depDomains;
  CCosmic *root;                        // root of the sub-space affected by this constraint
                                        // if this is a uni-space constraint
  bool implicit;						//true if this is an implicit constraint
  bool valid;							//true if it is valid constraint from the context, maybe invalid in the later evaluation step

private:
	int FindSpaces();
	int FindDomains();

public:
	DESERT_API CConstraint(CString& n, CConstraintSet *s, CElement* c, CString& t);
	CConstraint(const TCHAR *n, CConstraintSet *s, CElement* c, CString& t);
	CConstraint(CString& n, CConstraintSet *s, CElement* c, const TCHAR *t);
	CConstraint(const TCHAR *n, CConstraintSet *s, CElement* c, const TCHAR *t);
	CConstraint(CString& n, CConstraintSet *s, CElement* c, CString& t, long id, long e_id);
	CConstraint::CConstraint(const TCHAR *n, CConstraintSet *s, CElement* c, CString& t, long id, long e_id);
	CConstraint::CConstraint(CString& n, CConstraintSet *s, CElement* c, const TCHAR *t, long id, long e_id);
	CConstraint::CConstraint(const TCHAR *n, CConstraintSet *s, CElement* c, const TCHAR *t, long id, long e_id);
	DESERT_API CConstraint::CConstraint(const TCHAR *n, CElement* c, const TCHAR *t);
	CConstraint::~CConstraint();

public:

	CConstraintSet *GetSet();
	DESERT_API CElement *GetContext();
	DESERT_API CString& GetText();
	DESERT_API ClConstraint *GetExpression();
	DESERT_API ConstraintType GetConstraintType() const;
	DESERT_API operator ConstraintType() const	;
	DESERT_API CCosmic *GetRoot();
	DESERT_API CSpaceList& GetSpaces()	;
	DESERT_API CDomainList& GetDomains();
	DESERT_API bool IsPropertyDependent() const;
	DESERT_API bool IsImplicit() const;
	DESERT_API CCosmicList& GetDependency();

	DESERT_API bool IsValid() const;
	DESERT_API void SetValid(bool isvalid);

	bool Parse();
	bool Verify(CString &errStr, CString &what);
	void Analyse();
};

///////////////////////////
class  CFormulaSet : public CConstraintSet
{
protected:
	CFormulaList formulas;

public:
	DESERT_API CFormulaSet(CString &n);
	DESERT_API CFormulaSet(const TCHAR *n);
	CFormulaSet(CString &n, long id, long e_id);
	CFormulaSet(const TCHAR *n, long id, long e_id);
	~CFormulaSet();

public:
	DESERT_API CFormulaList& GetFormulas();
	DESERT_API CFormula *FindFormula(long i);
	DESERT_API CFormula *FindFormula(const TCHAR *n);
	CFormula *RemoveFormula(CFormula *e);
	CFormula *RemoveFormula(long i);
	CFormula *RemoveFormula(const TCHAR *n);
	DESERT_API CFormula *InsertFormula(CFormula *e);
};


class  CFormula : public CConstraint
{
protected:
    CFormulaSet *set;
	ClFormula *expression;
  
public:
	DESERT_API CFormula(CString& n, CFormulaSet *s, CElement* c, CString& t);
	CFormula(const TCHAR *n, CFormulaSet *s, CElement* c, CString& t);
	CFormula(CString& n, CFormulaSet *s, CElement* c, const TCHAR *t);
	CFormula(const TCHAR *n, CFormulaSet *s, CElement* c, const TCHAR *t);
	CFormula(CString& n, CFormulaSet *s, CElement* c, CString& t, long id, long e_id);
	CFormula::CFormula(const TCHAR *n, CFormulaSet *s, CElement* c, CString& t, long id, long e_id);
	CFormula::CFormula(CString& n, CFormulaSet *s, CElement* c, const TCHAR *t, long id, long e_id);
	CFormula::CFormula(const TCHAR *n, CFormulaSet *s, CElement* c, const TCHAR *t, long id, long e_id);
	DESERT_API CFormula::CFormula(const TCHAR *n, CElement* c, const TCHAR *t);
	CFormula::~CFormula();

public:
	CFormulaSet *GetSet();
	DESERT_API ClFormula *GetExpression();
	bool Parse();
};
///////////////////////////



//////////////////////////
class  CRelation : public CStaticObj
{
protected:
	CElement *context;
	CElement *source;
	CElement *destination;

public:
 	DESERT_API CRelation(CElement *c, CElement *s, CElement *d) ;
	CRelation(CElement *c, CElement *s, CElement *d, long id, long e_id) ;
	~CRelation();


public:
	DESERT_API CElement * GetContext();
	DESERT_API CElement * GetSource()	;
	DESERT_API CElement * GetDestination()	;
};

/////////////////////////////
class  CSimpleFormula : public CStaticObj
{
protected:
	CPropertyList srclist;
	CPropertyList dstlist;
	CString pcm_fn;

public:
	DESERT_API CSimpleFormula();
	DESERT_API CSimpleFormula(const TCHAR *fn);
	~CSimpleFormula();
	DESERT_API void InsertSourceProperty(CProperty *srcp);
	DESERT_API void InsertDestinationProperty(CProperty *dstp);
	DESERT_API CPropertyList& GetSourceList();
	DESERT_API CPropertyList& GetDestinationList();
	DESERT_API const FunctionType GetFuncType() const; 
};
#endif //ifndef STATIC_H
