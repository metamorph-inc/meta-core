// -*-C++-*-
// static.cpp
// Implementation of static classes


#include "stdafx.h"
#include "desert.h"
#include "CSetErrDialog.h"
#include "core/static.h"
#include "core/clmain.h"
#include "core/manager.h"
#include "core/utils.h"


extern bool ParseMCLString(const TCHAR *str);
extern char *GetMCLErrorMessage(void);

#ifdef UNICODE
#define GetMCLErrorMessage GetMCLErrorMessageW
extern wchar_t *GetMCLErrorMessageW(void);
#endif

// ---------- StaticObj

/*
		Basic constructor, not meant to use it.
*/

inline CStaticObj::CStaticObj():CCosmic()
{
	type = typeStatic;
	name.Format(_T("Static_%d"), id);
	Warning( _T("CStaticObj::CStaticObj()"), _T("Unnamed object") );
};

/*
		Construction with names
*/
	
inline CStaticObj::CStaticObj(CString& n)							:	CCosmic(), name(n) { type = typeStatic;};
inline CStaticObj::CStaticObj(CString& n, long id, long e_id)		:	CCosmic(), name(n), iface_id(id), iface_external_id(e_id) { type = typeStatic; };
	
inline CStaticObj::CStaticObj(const TCHAR* n)						:	CCosmic(), name(n) { type = typeStatic; iface_id = -1; iface_external_id = -1;};
inline CStaticObj::CStaticObj(const TCHAR* n, long id, long e_id)	:	CCosmic(), name(n), iface_id(id), iface_external_id(e_id) { type = typeStatic; };

/*
		Getters for name
*/

inline const CString& CStaticObj::GetName() const { return name;};
inline CStaticObj::operator const TCHAR *() const { return name;};
/*
		Getters for Ids
*/
inline long CStaticObj::GetIfaceId() const { return iface_id;};
inline long CStaticObj::GetIfaceExternalId() const { return iface_external_id;};

inline CStaticObj::~CStaticObj(){};

// -------------- CElementContainer


inline CElementList& CElementContainer::GetElements()					{return elements;};
inline CElementList* CElementContainer::GetElements(int)				{return &elements;};
inline CElementList& CElementContainer::GetRootElements()				{return rootElements;};
inline CElementList* CElementContainer::GetRootElements(int)			{return &rootElements;};

inline CElement* CElementContainer::FindElement(long i)
{
	CElement * ret;
	return Find(elements, ret, i) ? ret : 0;
};
inline CElement* CElementContainer::FindElement(const TCHAR *n)
{
	CElement* ret;
	return Find(elements, ret, n) ? ret : 0;
};

/*
	Element Removers by pointer, id, name
*/
inline CElement* CElementContainer::RemoveElement(CElement *e)
{
	Remove(rootElements, e);
	return Remove(elements, e) ? e : 0;
};

inline CElement* CElementContainer::RemoveElement(long id)
{
	CElement *e;
	Remove(rootElements, e, id);
	return Remove(elements, e, id) ? e : 0;
};

inline CElement* CElementContainer::RemoveElement(const TCHAR *n)
{
	CElement *e;
	Remove(rootElements, e, n);
	return Remove(elements, e, n) ? e : 0;
};

inline CElement* CElementContainer::InsertElement(CElement *e)
{
	if (e->GetParent() == 0) Insert(rootElements, e);
	return Insert(elements, e) ? e : 0;
};


inline CElementContainer::~CElementContainer()	{Destroy(elements);};


//------------- CSpace
inline CSpace::CSpace(CString& n) : CStaticObj(n) { type = typeSpace;}; 
inline CSpace::CSpace(const TCHAR *n) : CStaticObj(n) { type = typeSpace;};
	
inline CSpace::CSpace(CString &n, long id, long e_id) : CStaticObj(n, id, e_id) { type = typeSpace;};
inline CSpace::CSpace(const TCHAR *n, long id, long e_id) : CStaticObj(n, id, e_id) { type = typeSpace;};

inline CType CSpace::GetType()					{return CStaticObj::GetType();				};
inline CSpace::operator long() const			{return CStaticObj::operator long();		};
inline CSpace::operator const TCHAR * () const	{return CStaticObj::operator const TCHAR *();};

inline CSpace:: ~CSpace() {};  

double CSpace::ComputeSize() const
{
  double ret=1;

  POSITION pos = rootElements.GetHeadPosition();
  while(pos) ret *= rootElements.GetNext(pos)->ComputeSize();

  return ret;
}

//------------ CElement


/*
	Construction of static elements:
*/
inline CElement::CElement(CString& n, CElementContainer *c, CDecomposition d, CElement *p, long e)
	: CStaticObj(n),  container(c), parent(p), decomposition(d), external(e)  {type = typeElement; };
inline CElement::CElement(const TCHAR *n, CElementContainer *c, CDecomposition d, CElement *p, long e)
	: CStaticObj(n),  container(c), parent(p), decomposition(d), external(e)  {type = typeElement; };

inline CElement::CElement(CString& n, CElementContainer *c, CDecomposition d, CElement *p, long id, long e_id)
	: CStaticObj(n, id, e_id),  container(c), parent(p), decomposition(d)  {type = typeElement; };

inline CElement::CElement(const TCHAR *n, CElementContainer *c, CDecomposition d, CElement *p, long id, long e_id)
	: CStaticObj(n, id, e_id),  container(c), parent(p), decomposition(d) {type = typeElement; };

	
/*
	Destruction of static elements
*/
inline CElement::~CElement() { Destroy(relations);  Destroy(properties);Destroy(formulas);};
/*
	Various getter functions/
*/

inline CElementContainer * CElement::GetContainer()				{return container;};
inline CElement * CElement::GetParent()							{return parent;};
inline CElementList& CElement::GetChildren()					{return children;};
inline CRelationList& CElement::GetRelations()					{return relations;};
inline CPropertyList& CElement::GetProperties()					{return properties;};
inline CDecomposition CElement::GetDecomposition() const		{return decomposition;};
inline CElement::operator CDecomposition() const				{return decomposition;};
inline long CElement::GetExternal() const						{return external;};

inline CCosmic* CElement::Parent()								{return parent;}
inline CCosmicList* CElement::Children()							{CCosmicList *list = (CCosmicList *)&children;return list;};

inline CFormulaList& CElement::GetFormulas()					{return formulas;};
/*
	Element find/remove/insert in the subtree rooted at this
*/
inline CElement	* CElement::FindChild(long i)					{CElement *e;  return Find(children, e, i) ? e : 0;};
inline CElement	* CElement::FindChild(const TCHAR *n)			{CElement *e;  return Find(children, e, n) ? e : 0;};
inline CElement	* CElement::RemoveChild(CElement *e)			{return Remove(children, e) ? e : 0;};
inline CElement	* CElement::RemoveChild(long i)				{CElement *e;  return Remove(children, e, i) ? e : 0;};
inline CElement	* CElement::RemoveChild(const TCHAR *n)			{CElement *e;  return Remove(children, e, n) ? e : 0;}
inline CElement	* CElement::InsertChild(CElement *e)			{return Insert(children, e) ? e : 0;}

/*
	Property manipulation
*/
inline CProperty*	CElement::FindProperty(long i)				{CProperty *e;  return Find(properties, e, i) ? e : 0;};
inline CProperty*	CElement::FindProperty(const TCHAR *n)		{CProperty *e;  return Find(properties, e, n) ? e : 0;}
inline CProperty*	CElement::RemoveProperty(CProperty *e)		{return Remove(properties, e) ? e : 0;};
inline CProperty*	CElement::RemoveProperty(long i)				{CProperty *e;  return Remove(properties, e, i) ? e : 0;};
inline CProperty*	CElement::RemoveProperty(const TCHAR *n)		{CProperty *e;  return Remove(properties, e, n) ? e : 0;};
inline CProperty*	CElement::InsertProperty(CProperty *e)		{return Insert(properties, e) ? e : 0;};

inline CFormula*	CElement::InsertFormula(CFormula *f)		{return Insert(formulas, f) ? f : 0;};
/*
	Relation manipulation
*/
inline CRelation*	CElement::FindRelation(long i)				{CRelation *e;  return Find(relations, e, i) ? e : 0;};
inline CRelation*	CElement::RemoveRelation(CRelation *e)		{return Remove(relations, e) ? e : 0;};
inline CRelation*	CElement::RemoveRelation(long i)				{CRelation *e;  return Remove(relations, e, i) ? e : 0;};
inline CRelation*	CElement::InsertRelation(CRelation *e)		{return Insert(relations, e) ? e : 0;}


double CElement::ComputeSize() const
{
  double ret;
  POSITION pos;

  switch(decomposition) {
  case decompAnd:
    pos = children.GetHeadPosition();
    ret = 1;
    while(pos) ret *= children.GetNext(pos)->ComputeSize();
    break;

  case decompOr:
    pos = children.GetHeadPosition();
    ret = 0;
    while(pos) ret += children.GetNext(pos)->ComputeSize();
    if (ret==0)
    {
      Warning(_T("CElement::ComputeSize"), _T("OR decomposed element <%s> has no child"),
              (const TCHAR *)*this);
      ret=1;                            // empty template
    }
    break;

  case decompLeaf:
    pos = properties.GetHeadPosition();
    ret = 1;
    while(pos) ret *= properties.GetNext(pos)->ComputeSize();
    break;
  }
  
  return ret;
};

// CAssignment



inline CAssignment::CAssignment(CElement * value, CString &name, long id, long e_id)
	:CStaticObj(name, id, e_id) { e_value = value; type = typeAssignment; };

inline CAssignment::CAssignment(CElement * value, const TCHAR *name, long id, long e_id)
	:CStaticObj(name, id, e_id) { e_value = value; type = typeAssignment; };

inline CAssignment::CAssignment(CElement * value, const TCHAR *name)
	:CStaticObj(name) { e_value = value; type = typeAssignment; };

inline CElement * CAssignment::GetEValue(){return e_value;};

#ifndef DOUBLE_MTBDD
inline CAssignment::CAssignment(long value, CString &name, long id, long e_id)
	:CStaticObj(name, id, e_id) { l_value = value; type = typeAssignment; };

inline CAssignment::CAssignment(long value, const TCHAR *name, long id, long e_id)
	:CStaticObj(name, id, e_id) { l_value = value; type = typeAssignment; };

inline CAssignment::CAssignment(long value, const TCHAR *name)
	:CStaticObj(name) { l_value = value; type = typeAssignment; };

inline long CAssignment::GetLValue(){return l_value;};
#else
inline CAssignment::CAssignment(double value, CString &name, long id, long e_id)
	:CStaticObj(name, id, e_id) { d_value = value; type = typeAssignment; };

inline CAssignment::CAssignment(double value, const TCHAR *name, long id, long e_id)
	:CStaticObj(name, id, e_id) { d_value = value; type = typeAssignment; };

inline CAssignment::CAssignment(double value, const TCHAR *name)
	:CStaticObj(name) { d_value = value; type = typeAssignment; };

inline double CAssignment::GetLValue(){return d_value;};
#endif
//---------------- CDomain



inline CDomain::CDomain(CString& n)  : CStaticObj(n){  type = typeDomain;};
inline CDomain::CDomain(const TCHAR *n)  : CStaticObj(n){  type = typeDomain;};

inline CDomain::CDomain(CString& n, long id, long e_id)		: CStaticObj(n, id, e_id){  type = typeDomain;};
inline CDomain::CDomain(const TCHAR *n, long id, long e_id)	: CStaticObj(n, id, e_id){  type = typeDomain;};

inline CElement * CDomain::FindElement(long i)			{return 0;};
inline CElement * CDomain::FindElement(const TCHAR *n)	{return 0;};
inline CElement * CDomain::RemoveElement(CElement *e)	{return 0;};
inline CElement * CDomain::RemoveElement(long i)		{return 0;};
inline CElement * CDomain::RemoveElement(const TCHAR *n){return 0;};
inline CElement * CDomain::InsertElement(CElement *e)	{return 0;};

  
inline CDomain::~CDomain() {};


// -------------------CNaturalDomain

inline CNaturalDomain::CNaturalDomain(CString& name, int mx, int mn) 
		: CDomain(name), maxVal(mx), minVal(mn) { type = typeNaturalDomain;};
inline CNaturalDomain::CNaturalDomain(const TCHAR *name, int mx, int mn)
		: CDomain(name), maxVal(mx), minVal(mn) { type = typeNaturalDomain;};

inline CNaturalDomain::CNaturalDomain(int id, int e_id, CString& name, int mx, int mn) 
		: CDomain(name, id, e_id), maxVal(mx), minVal(mn) { type = typeNaturalDomain;};
inline CNaturalDomain::CNaturalDomain( int id, int e_id, const TCHAR *name, int mx, int mn)
		: CDomain(name, id, e_id), maxVal(mx), minVal(mn) { type = typeNaturalDomain;};


inline int CNaturalDomain::GetSize() const		{ return maxVal - minVal;};
inline int CNaturalDomain::GetMax() const		{ return maxVal;};
inline int CNaturalDomain::GetMin() const		{ return minVal;};
inline CNaturalDomain::~CNaturalDomain(){};


//---------------------CCustomDomain

inline CCustomDomain::CCustomDomain(CString& n)		: CDomain(n){  type = typeCustomDomain;};
inline CCustomDomain::CCustomDomain(const TCHAR *n)	: CDomain(n){  type = typeCustomDomain;};
inline CCustomDomain::CCustomDomain(CString& n, long id, long e_id)		: CDomain(n, id, e_id){  type = typeCustomDomain;};
inline CCustomDomain::CCustomDomain(const TCHAR *n, long id, long e_id)	: CDomain(n, id, e_id){  type = typeCustomDomain;};
inline CCustomDomain::~CCustomDomain(){};

inline int CCustomDomain::GetSize() const{  return elements.GetCount();};

inline CElement *CCustomDomain::FindElement(long i)				{return CElementContainer::FindElement(i);};
inline CElement *CCustomDomain::FindElement(const TCHAR *n)		{return CElementContainer::FindElement(n);};
inline CElement *CCustomDomain::RemoveElement(CElement *e)		{return CElementContainer::RemoveElement(e);};
inline CElement *CCustomDomain::RemoveElement(long i)				{return CElementContainer::RemoveElement(i);};
inline CElement *CCustomDomain::RemoveElement(const TCHAR *n)		{return CElementContainer::RemoveElement(n);};
inline CElement *CCustomDomain::InsertElement(CElement *e)		{return CElementContainer::InsertElement(e);};

inline CType CCustomDomain::GetType()						{return CDomain::GetType();					};
inline CCustomDomain::operator long () const				{return CDomain::operator long();			};
inline CCustomDomain::operator const TCHAR * () const		{return CDomain::operator const TCHAR * ();	};
  

//----------------------CProperty
inline CProperty::CProperty(const TCHAR *n, const TCHAR *p, CDomain *d, CElement *o)
	:CStaticObj(n), pcm_fn(p), domain(d), owner(o){type = typeProperty;}

inline CProperty::CProperty(CString &n, const TCHAR *p, CDomain *d, CElement *o)
	:CStaticObj(n), pcm_fn(p), domain(d), owner(o){type = typeProperty;}

inline CProperty::CProperty(const TCHAR *n, const TCHAR *p, CDomain *d, CElement *o, long id, long e_id)
	:CStaticObj(n, id, e_id), pcm_fn(p), domain(d), owner(o){type = typeProperty;}

inline CProperty::CProperty(CString &n, const TCHAR *p, CDomain *d, CElement *o, long id, long e_id)
	:CStaticObj(n, id, e_id), pcm_fn(p), domain(d), owner(o){type = typeProperty;}

//Without domain (for simpleformula property
inline CProperty::CProperty(const TCHAR *n, const TCHAR *p,CElement *o)
	:CStaticObj(n), pcm_fn(p), owner(o), domain(NULL)
{
	type = typeProperty;
}

inline CProperty::CProperty(CString &n, const TCHAR *p, CElement *o)
	:CStaticObj(n), pcm_fn(p), owner(o), domain(NULL)
{
	type = typeProperty;
}

inline CProperty::CProperty(const TCHAR *n, const TCHAR *p,CElement *o, long id, long e_id)
	:CStaticObj(n, id, e_id), pcm_fn(p), owner(o), domain(NULL)
{
	type = typeProperty;
}

inline CProperty::CProperty(CString &n, const TCHAR *p, CElement *o, long id, long e_id)
	:CStaticObj(n, id, e_id), pcm_fn(p), owner(o), domain(NULL)
{
	type = typeProperty;
}



inline CProperty::~CProperty()				{}; 
inline CDomain*		CProperty::GetDomain()	{return domain;};
inline CElement*	CProperty::GetOwner()	{return owner;};
inline bool			CProperty::GetValue(int& r) const			{r=0; return false;};
inline bool			CProperty::GetValue(CElement*& r) const		{r=0; return false;};	
inline const FunctionType CProperty::GetFuncType() const 
{ 
	if (pcm_fn == _T("PCM_ADD"))				//arithmetical sum
		return funcPCMAdd;			
	
	else if (pcm_fn == _T("PCM_MUL"))			//arithmetical product
		return funcPCMMul;

	else if (pcm_fn == _T("PCM_AMED"))			//arithmetical medium
		return funcPCMAmed;

	else if (pcm_fn == _T("PCM_GMED"))			//geomethric medium
		return  funcPCMGmed;

	else if (pcm_fn == _T("PCM_MIN"))			//arithmetic minimum
		return funcPCMMin;

	else if (pcm_fn == _T("PCM_MAX"))			//arithmetic maximum
		return funcPCMMax;

	else if (pcm_fn == _T("PCM_NOP"))			
		return funcPCMNop; 

	else if (pcm_fn == _T("PCM_NONE"))			//non-composable property
		return funcPCMNone;
	
	else if (pcm_fn == _T("PCM_CUST"))			//custom PCM function
		return funcPCMCust;

	else if (pcm_fn == _T("PCM_OR"))			//Or function
		return funcPCMOr;

	else
		return funcNofunc;

};

inline bool CProperty::IsNatural() const
{
	ASSERT_EX(domain, _T("CProperty::IsNatural"), _T("domain is NULL"));
	return domain->IsType(typeNaturalDomain);
};

inline double CProperty::ComputeSize() const			{return 1;};


//----------------------CConstantProperty

inline CConstantProperty::CConstantProperty(CString& n, CString& p, CDomain* d, CElement *o, int v)
		: CProperty(n, p, d, o), value(v){  type = typeConstantProperty;}

inline CConstantProperty::CConstantProperty(const TCHAR *n, const TCHAR *p, CDomain* d, CElement *o, int v)
		: CProperty(n, p, d, o), value(v){  type = typeConstantProperty;}
	
inline CConstantProperty::CConstantProperty(CString& n, CString& p, CDomain* d, CElement *o, int v, long id, long e_id)
		: CProperty(n, p, d, o, id, e_id), value(v){  type = typeConstantProperty;}

inline CConstantProperty::CConstantProperty(const TCHAR *n, const TCHAR *p, CDomain* d, CElement *o, int v, long id, long e_id)
		: CProperty(n, p, d, o, id, e_id), value(v){  type = typeConstantProperty;}
	
inline CConstantProperty::~CConstantProperty(){}

inline bool CConstantProperty::GetValue(int& r) const {  r = value;  return true;};

//----------------------CVariableProperty
inline CVariableProperty::CVariableProperty(CString& n, CString& p, CDomain* d, CElement *o)
		: CProperty(n, p, d, o), isParametric(false){type = typeVariableProperty; }

inline CVariableProperty::CVariableProperty(const TCHAR *n, const TCHAR *p, CDomain* d, CElement *o)
		: CProperty(n, p, d, o),isParametric(false){type = typeVariableProperty; }

inline CVariableProperty::CVariableProperty(CString& n, CString& p, CDomain* d, CElement *o, long id, long e_id)
		: CProperty(n, p, d, o, id, e_id), isParametric(false){type = typeVariableProperty;}

inline CVariableProperty::CVariableProperty(const TCHAR *n, const TCHAR *p, CDomain* d, CElement *o, long id, long e_id)
		: CProperty(n, p, d, o, id, e_id),isParametric(false){type = typeVariableProperty;}

//for variableproperty without domain (simpleformula)
inline CVariableProperty::CVariableProperty(CString& n, CString& p,CElement *o)
		: CProperty(n, p, o), isParametric(true){type = typeVariableProperty; }

inline CVariableProperty::CVariableProperty(const TCHAR *n, const TCHAR *p, CElement *o)
		: CProperty(n, p, o),isParametric(true){type = typeVariableProperty; }

inline CVariableProperty::CVariableProperty(CString& n, CString& p,CElement *o, long id, long e_id)
		: CProperty(n, p, o, id, e_id), isParametric(true){type = typeVariableProperty;}

inline CVariableProperty::CVariableProperty(const TCHAR *n, const TCHAR *p, CElement *o, long id, long e_id)
		: CProperty(n, p, o, id, e_id),isParametric(true){type = typeVariableProperty;}

CVariableProperty::~CVariableProperty()
{
	
	POSITION mPos = range_lmap.GetStartPosition();
	while (mPos)
	{
		CAssignmentList * ass_l = NULL;
		long key;
		range_lmap.GetNextAssoc(mPos, key, ass_l);
		if (ass_l) 
		{
			POSITION mPos1 = ass_l->GetHeadPosition();
			while (mPos1)
			{
				CAssignment * ass = ass_l->GetNext(mPos1);
				if (ass) delete ass;
			}
			delete ass_l;
		}			
	}
	mPos = natural_range_lmap.GetStartPosition();
	while (mPos)
	{
		CAssignmentList * ass_l = NULL;
#ifndef DOUBLE_MTBDD		
		long key;
#else
		double key;
#endif
		natural_range_lmap.GetNextAssoc(mPos, key, ass_l);
		if (ass_l) 
		{
			POSITION mPos1 = ass_l->GetHeadPosition();
			while (mPos1)
			{
				CAssignment * ass = ass_l->GetNext(mPos1);
				if (ass) delete ass;
			}
		
				delete ass_l;
		}
	}
	range_lmap.RemoveAll();
	natural_range_lmap.RemoveAll();
}


double CVariableProperty::ComputeSize() const
{
  return (domain->IsType(typeNaturalDomain)) ?
    1 :                                 // it could also be -- naturalRange.GetCount()
                                        // but we will keep it small for now
    domain->GetSize();
}

long CVariableProperty::AddToRange(CElement * e, CString &n)
{
	// make sure that the element belongs in our domain
  //ASSERT_EX( domain->FindElement((long)*e) == e, _T("CVariableProperty::AddToRange"), _T("element does not belong to domain"));
	if (domain->FindElement((long)*e) != e)
		throw new CDesertException(_T("CVariableProperty::AddToRange(): element does not belong to domain!"));

  CAssignment * ass = new CAssignment(e, n);
  ASSERT_EX(ass, _T("CVariableProperty::AddToRange"), _T("assignment failed!"));

  CAssignment * ass_try = NULL;

  CAssignmentList * ass_ltry = NULL;


  if (!range_lmap.Lookup(*e, ass_ltry))
  {
	  range.AddTail(e);
 
	  ASSERT(ass_ltry == NULL);
	  ass_ltry = new CAssignmentList;
	 // ASSERT_EX(ass_ltry,_T("CVariableProperty::AddToRange"), _T("Failed to create the assignment list"));	
	  if (!ass_ltry)
		  throw new CDesertException(_T("CVariableProperty::AddToRange: Failed to create the assignment list"));
	  range_lmap[*e] = ass_ltry;
  }
  ass_ltry->AddTail(ass);
  

  return *ass;
};

long CVariableProperty::AddToRange(CElement * e, const TCHAR *n)
{
	// make sure that the element belongs in our domain
  //ASSERT_EX( domain->FindElement((long)*e) == e, _T("CVariableProperty::AddToRange"), _T("element does not belong to domain"));
	if (domain->FindElement((long)*e) != e)
		throw new CDesertException(_T("CVariableProperty::AddToRange(): element does not belong to domain"));
 
  // we will need to delete all this....
  CAssignment * ass = new CAssignment(e, n);
  //ASSERT_EX(ass, _T("CVariableProperty::AddToRange"), _T("assignment failed!"));
  if (!ass)
	  throw new CDesertException(_T("CVariableProperty::AddToRange(): Create assignment failed!"));

  //maintaing the map element -> list of assignments
  CAssignmentList * ass_ltry = NULL;

  if (!range_lmap.Lookup(*e, ass_ltry))
  {
	  range.AddTail(e);

	  ASSERT(ass_ltry == NULL);

	  ass_ltry = new CAssignmentList;
	  //ASSERT_EX(ass_ltry,_T("CVariableProperty::AddToRange"), _T("Failed to create the assignment list"));	
	  if (!ass_ltry)
		  throw new CDesertException(_T("CVariableProperty::AddToRange(): Failed to create the assignment list!"));
	  range_lmap[*e] = ass_ltry;
  }
  

  ass_ltry->AddTail(ass);
 

  return *ass; //implicit cast to long

};

#ifndef DOUBLE_MTBDD
long CVariableProperty::AddToRange(long e, CString &n)
{
	 // make sure that the element belongs in our -*-natural-*- domain
  //ASSERT_EX( domain->IsType(typeNaturalDomain), _T("CVariableProperty::AddToRange"), _T("the domain is not a natural domain"));
	if (!domain->IsType(typeNaturalDomain))
		throw new CDesertException(_T("CVariableProperty::AddToRange(): the domain is not a natural domain"));
  CNaturalDomain *nd = (CNaturalDomain *)domain;

  //ASSERT_EX( e <= nd->GetMax() && e >= nd->GetMin(), _T("CVariableProperty::AddToRange"), _T("element is outside the domain range"));
  if (e > nd->GetMax() || e < nd->GetMin())
  {
	  int mx = nd->GetMax();
	  throw new CDesertException(_T("CVariableProperty::AddToRange(): element is outside the domain range"));
  }

  naturalRange.AddTail( (int)e );
  CAssignment * ass = new CAssignment(e, n);
  ASSERT_EX(ass, _T("CVariableProperty::AddToRange"), _T("assignment failed!"));
 
  CAssignmentList * ass_ltry = NULL;

  if (!natural_range_lmap.Lookup(e, ass_ltry))
  {
	  ASSERT(ass_ltry == NULL);
	  ass_ltry = new CAssignmentList;
	  //ASSERT_EX(ass_ltry,_T("CVariableProperty::AddToRange"), _T("Failed to create the assignment list"));	
	  if (!ass_ltry)
		  throw new CDesertException(_T("CVariableProperty::AddToRange(): Failed to create the assignment list!"));
	  natural_range_lmap[e] = ass_ltry;
  }
  ass_ltry->AddTail(ass);
  return *ass;	//implicit cast to long
};

long CVariableProperty::AddToRange(long e, const TCHAR *n)
{
	 // make sure that the element belongs in our -*-natural-*- domain

  //ASSERT_EX( domain->IsType(typeNaturalDomain), _T("CVariableProperty::AddToRange"), _T("the domain is not a natural domain"));
  if (!domain->IsType(typeNaturalDomain))
		throw new CDesertException(_T("CVariableProperty::AddToRange(): the domain is not a natural domain"));
  
  CNaturalDomain *nd = (CNaturalDomain *)domain;
  //ASSERT_EX( e <= nd->GetMax() && e >= nd->GetMin(), _T("CVariableProperty::AddToRange"), _T("element is outside the domain range"));

  if (e > nd->GetMax() || e < nd->GetMin())
	  throw new CDesertException(_T("CVariableProperty::AddToRange(): element is outside the domain range"));


  //always create an assignment object ...
  //but actually add to our range if it wasn't added before

  
  CAssignment * ass = new CAssignment(e, n);
  //ASSERT_EX(ass, _T("CVariableProperty::AddToRange"), _T("assignment failed!"));
  if (!ass)
	  throw new CDesertException(_T("CVariableProperty::AddToRange(): Create assignment failed!"));

  CAssignment * ass_try = NULL;
  
  //maintaing the map value -> list of assignments
  CAssignmentList * ass_ltry = NULL;
 
  if (!natural_range_lmap.Lookup(e, ass_ltry))
  {
	  //it means, that value e was not added yet to our list
	  naturalRange.AddTail( (int)e );
	  ASSERT(ass_ltry == NULL);
	  ass_ltry = new CAssignmentList;
	  //ASSERT_EX(ass_ltry,_T("CVariableProperty::AddToRange"), _T("Failed to create the assignment list"));	
	   if (!ass_ltry)
		  throw new CDesertException(_T("CVariableProperty::AddToRange(): Failed to create the assignment list!"));
	 
	  natural_range_lmap[e] = ass_ltry;
  }
 

  ass_ltry->AddTail(ass);
 
  
  return *ass;
};

inline CIntegerList& CVariableProperty::GetNaturalRange()			{return naturalRange;};		
#else
long CVariableProperty::AddToRange(double e, CString &n)
{
	 // make sure that the element belongs in our -*-natural-*- domain
  //ASSERT_EX( domain->IsType(typeNaturalDomain), _T("CVariableProperty::AddToRange"), _T("the domain is not a natural domain"));
	if (!domain->IsType(typeNaturalDomain))
		throw new CDesertException(_T("CVariableProperty::AddToRange(): the domain is not a natural domain"));
  CNaturalDomain *nd = (CNaturalDomain *)domain;

  //ASSERT_EX( e <= nd->GetMax() && e >= nd->GetMin(), _T("CVariableProperty::AddToRange"), _T("element is outside the domain range"));
 /* if (e > nd->GetMax() || e < nd->GetMin())
  {
	  int mx = nd->GetMax();
	  int mn = nd->GetMin();
	  throw new CDesertException(_T("CVariableProperty::AddToRange(): element is outside the domain range"));
  }*/

 
  CAssignment * ass = new CAssignment(e, n);
  ASSERT_EX(ass, _T("CVariableProperty::AddToRange"), _T("assignment failed!"));
 
  CAssignmentList * ass_ltry = NULL;

  if (!natural_range_lmap.Lookup(e, ass_ltry))
  {
	    naturalRange.AddTail( e );
	  ASSERT(ass_ltry == NULL);
	  ass_ltry = new CAssignmentList;
	  //ASSERT_EX(ass_ltry,_T("CVariableProperty::AddToRange"), _T("Failed to create the assignment list"));	
	  if (!ass_ltry)
		  throw new CDesertException(_T("CVariableProperty::AddToRange(): Failed to create the assignment list!"));
	  natural_range_lmap[e] = ass_ltry;
  }
  ass_ltry->AddTail(ass);
  return *ass;	//implicit cast to long
};

long CVariableProperty::AddToRange(double e, const TCHAR *n)
{
	 // make sure that the element belongs in our -*-natural-*- domain

  //ASSERT_EX( domain->IsType(typeNaturalDomain), _T("CVariableProperty::AddToRange"), _T("the domain is not a natural domain"));
  if (!domain->IsType(typeNaturalDomain))
		throw new CDesertException(_T("CVariableProperty::AddToRange(): the domain is not a natural domain"));
  
  CNaturalDomain *nd = (CNaturalDomain *)domain;
  //ASSERT_EX( e <= nd->GetMax() && e >= nd->GetMin(), _T("CVariableProperty::AddToRange"), _T("element is outside the domain range"));

 /* if (e > nd->GetMax() || e < nd->GetMin())
	  throw new CDesertException(_T("CVariableProperty::AddToRange(): element is outside the domain range"));*/


  //always create an assignment object ...
  //but actually add to our range if it wasn't added before

  
  CAssignment * ass = new CAssignment(e, n);
  //ASSERT_EX(ass, _T("CVariableProperty::AddToRange"), _T("assignment failed!"));
  if (!ass)
	  throw new CDesertException(_T("CVariableProperty::AddToRange(): Create assignment failed!"));

  CAssignment * ass_try = NULL;
  
  //maintaing the map value -> list of assignments
  CAssignmentList * ass_ltry = NULL;
 
  if (!natural_range_lmap.Lookup(e, ass_ltry))
  {
	  //it means, that value e was not added yet to our list
	  naturalRange.AddTail( e );
	  ASSERT(ass_ltry == NULL);
	  ass_ltry = new CAssignmentList;
	  //ASSERT_EX(ass_ltry,_T("CVariableProperty::AddToRange"), _T("Failed to create the assignment list"));	
	   if (!ass_ltry)
		  throw new CDesertException(_T("CVariableProperty::AddToRange(): Failed to create the assignment list!"));
	 
	  natural_range_lmap[e] = ass_ltry;
  }
  ass_ltry->AddTail(ass); 
  return *ass;
};

inline CDoubleList& CVariableProperty::GetNaturalRange()			{return naturalRange;};		
#endif

void CVariableProperty::AddToPropertyList(CProperty *prop)
{
	properties.AddTail(prop);
};

inline CElementList& CVariableProperty::GetRange()					{return range;};
inline CPropertyList& CVariableProperty::GetProperties()			{return properties;};
inline bool CVariableProperty::IsParametric()						{return isParametric;};
	
// -----CConstraintSet

inline CConstraintSet::CConstraintSet(CString &n): CStaticObj(n)		{type = typeConstraintSet;}
inline CConstraintSet::CConstraintSet(const TCHAR *n): CStaticObj(n)		{type = typeConstraintSet;}
inline CConstraintSet::CConstraintSet(CString &n, long id, long e_id)		: CStaticObj(n, id, e_id)		{type = typeConstraintSet;}
inline CConstraintSet::CConstraintSet(const TCHAR *n, long id, long e_id)	: CStaticObj(n, id, e_id)		{type = typeConstraintSet;}	
inline CConstraintSet::~CConstraintSet()									{Destroy(constraints);}

inline CConstraintList&	CConstraintSet::GetConstraints()						{return constraints;}
inline CConstraint*		CConstraintSet::FindConstraint(long i)					{CConstraint *ret;  return Find(constraints, ret, i) ? ret : 0;};
inline CConstraint*		CConstraintSet::FindConstraint(const TCHAR *n)			{CConstraint *ret;  return Find(constraints, ret, n) ? ret : 0;};
inline CConstraint*		CConstraintSet::RemoveConstraint(CConstraint *e)			{return Remove(constraints, e) ? e : 0;};
inline CConstraint*		CConstraintSet::RemoveConstraint(long i)					{CConstraint *e; return Remove(constraints, e, i) ? e : 0;};
inline CConstraint*		CConstraintSet::RemoveConstraint(const TCHAR *n)			{CConstraint *e;  return Remove(constraints, e, n) ? e : 0;};
inline CConstraint*		CConstraintSet::InsertConstraint(CConstraint *e)			{return Insert(constraints, e) ? e : 0;};


bool CConstraintSet::Verify(CCSetErrDialog * csed)
{

  bool ret=true;
  POSITION pos = constraints.GetHeadPosition();
  while(pos)
  {
    POSITION curpos=pos;
    CConstraint *cur = constraints.GetNext(pos);
	CString err_str, what;

    if (cur->Verify(err_str, what)) continue;

	if (what.GetLength()) {
		if(csed)
			csed->AddError(*this,*cur, *(cur->GetContext()), (LPCTSTR)err_str,  what);
	}
	else {
		if(csed)
			csed->AddError(*this,*cur, *(cur->GetContext()), (LPCTSTR)err_str,  NULL);
	}

    ret = false;
    constraints.RemoveAt(curpos);
    delete cur;
  }
  return ret;
}
//-----Constraint


inline CConstraint::CConstraint(CString& n, CConstraintSet *s, CElement* c, CString& t)
		: CStaticObj(n), set(s), context(c), text(t), expression(0), consType(typeUndef),valid(true)	
{
	type = typeConstraint;
	implicit = (s->GetName().Compare(ICS_NAME) == 0);
}

inline CConstraint::CConstraint(const TCHAR *n, CConstraintSet *s, CElement* c, CString& t)
		: CStaticObj(n), set(s), context(c), text(t), expression(0), consType(typeUndef),valid(true)
{
	type = typeConstraint;
	implicit = (s->GetName().Compare(ICS_NAME) == 0);
}

inline CConstraint::CConstraint(const TCHAR *n, CElement* c, const TCHAR *t)
		: CStaticObj(n), context(c), text(t), expression(0), consType(typeUndef),valid(true)
{
	type = typeConstraint;
//	implicit = (s->GetName().Compare(ICS_NAME) == 0);
}

inline CConstraint::CConstraint(CString& n, CConstraintSet *s, CElement* c, const TCHAR *t)
		: CStaticObj(n), set(s), context(c), text(t), expression(0), consType(typeUndef),valid(true)
{
	type = typeConstraint;
	implicit = (s->GetName().Compare(ICS_NAME) == 0);
}

inline CConstraint::CConstraint(const TCHAR *n, CConstraintSet *s, CElement* c, const TCHAR *t)
		: CStaticObj(n), set(s), context(c), text(t), expression(0), consType(typeUndef),valid(true)	
{
	type = typeConstraint;
	implicit = (s->GetName().Compare(ICS_NAME) == 0);

}

	
inline CConstraint::CConstraint(CString& n, CConstraintSet *s, CElement* c, CString& t, long id, long e_id)
		: CStaticObj(n, id, e_id), set(s), context(c), text(t), expression(0), consType(typeUndef),valid(true)	
{
	type = typeConstraint;
	implicit = (s->GetName().Compare(ICS_NAME) == 0);

}

inline CConstraint::CConstraint(const TCHAR *n, CConstraintSet *s, CElement* c, CString& t, long id, long e_id)
		: CStaticObj(n, id, e_id), set(s), context(c), text(t), expression(0), consType(typeUndef),valid(true)	
{
	type = typeConstraint;
	implicit = (s->GetName().Compare(ICS_NAME) == 0);

}

inline CConstraint::CConstraint(CString& n, CConstraintSet *s, CElement* c, const TCHAR *t, long id, long e_id)
		: CStaticObj(n, id, e_id), set(s), context(c), text(t), expression(0), consType(typeUndef),valid(true)
{
	type = typeConstraint;
	implicit = (s->GetName().Compare(ICS_NAME) == 0);

}

inline CConstraint::CConstraint(const TCHAR *n, CConstraintSet *s, CElement* c, const TCHAR *t, long id, long e_id)
		: CStaticObj(n, id, e_id), set(s), context(c), text(t), expression(0), consType(typeUndef),valid(true)	
{
	type = typeConstraint;
	implicit = (s->GetName().Compare(ICS_NAME) == 0);
}

	
inline CConstraint::~CConstraint()	 {delete expression;}

inline CConstraintSet *	CConstraint::GetSet()					{return set;};
inline CElement *		CConstraint::GetContext()				{return context;};
inline CString&			CConstraint::GetText()					{return text;};
inline ClConstraint *	CConstraint::GetExpression()			{return expression;};
inline ConstraintType	CConstraint::GetConstraintType() const	{return consType;};
inline					CConstraint::operator ConstraintType() const				{return consType;};
inline CCosmic *		CConstraint::GetRoot()					{return root;};
inline CSpaceList&		CConstraint::GetSpaces()				{return depSpaces;};
inline CDomainList&		CConstraint::GetDomains()				{return depDomains;};
inline bool				CConstraint::IsPropertyDependent() const{return !propertyDependency.IsEmpty();};
inline bool				CConstraint::IsImplicit() const			{return implicit;};
inline CCosmicList&		CConstraint::GetDependency()			{return dependency;};

inline bool				CConstraint::IsValid() const			{return valid;};
inline void				CConstraint::SetValid(bool isvalid) 	{valid = isvalid;};

void CConstraintSet::Analyse()
{
  POSITION pos = constraints.GetHeadPosition();
  while(pos) constraints.GetNext(pos)->Analyse();
}


bool CConstraint::Parse(void)
{
  bool ret = ParseMCLString(text);
  if (!ret) {
	  Error(_T("CConstraint::Parse"), _T("Constraint <%s> in <%s>: %s"), name,
		  (const TCHAR *)*context, GetMCLErrorMessage());
  }
  else
  {
    RemoveCRLF(text);
    expression = ClConstraint::GetLastConstraint();
    //ASSERT_EX( expression, _T("CConstraint::Parse"), _T("expression is nul"));
	if (!expression)
		throw new CDesertException(_T("CConstraint::Parse(): expression is NULL"));
  }
  return ret;
}

bool CConstraint::Verify(CString &err_str, CString &what)
{
  TCHAR *errString;
  ClContext clCtx = context;
  bool ret;

  try 
  {
	  ret=clCtx.Verify(expression, &dependency, &propertyDependency, errString);
  }
  catch (CDesertException *e)
  {
//	  if (e->Fatal()) throw e;			//should not be a fatal error.
	  err_str = clCtx.GetErrorString();
	  TCHAR c_str_what[1000];
	  if (e->GetErrorMessage(c_str_what, 1000))
		what = c_str_what;
	  this->valid = false;
	  return false;
  }
  /*
  if (!ret) Error(_T("CConstraint::Verify"),
                  _T("Constraint <%s> in <%s> has errors. \n%s"),
                  (const TCHAR *)*this, (const TCHAR *)*context, errString);
				  */
  return ret;
}

void CConstraint::Analyse()
{
  int sc = FindSpaces();
  int dc = FindDomains();

  //ASSERT_EX( sc<3 && dc<3, _T("CConstraint::Analyse"), _T("invalid dependency") );
  if (sc >= 3 || dc >= 3) throw new CDesertException(_T("CConstraint::Analyse(): invalid dependency"));
  consType = (ConstraintType)(sc + 3*dc);
  if (consType == typeUniSpace)
  {
    root = FindRoot(dependency);
    if (!root) root = depSpaces.GetHead();
  }
}

int CConstraint::FindSpaces()
{
  depSpaces.RemoveAll();
  POSITION pos = dependency.GetHeadPosition();
  while(pos)
  {
    CSpace *spc = (CSpace *)dependency.GetNext(pos)->Space();
    // pos1 should be non-null in event of spc being null
    bool found = !spc || depSpaces.Find(spc);
    if (!found) depSpaces.AddTail(spc);
  }
  return depSpaces.GetCount();
}

int CConstraint::FindDomains()
{
  depDomains.RemoveAll();
  POSITION pos = dependency.GetHeadPosition();
  while(pos)
  {
    CDomain *dmn = (CDomain *)dependency.GetNext(pos)->Domain();
    // pos1 should be non-null in event of spc being null
    bool found = !dmn || depDomains.Find(dmn);
    if (!found) depDomains.AddTail(dmn);
  }
  return depDomains.GetCount();
}

//--------------CRelation


inline CRelation::CRelation(CElement *c, CElement *s, CElement *d) 
	: CStaticObj(_T("relation")), context(c), source(s), destination(d){ type = typeRelation;}

inline CRelation::CRelation(CElement *c, CElement *s, CElement *d, long id, long e_id) 
	: CStaticObj(_T("relation"), id, e_id), context(c), source(s), destination(d){ type = typeRelation;}
	
inline CRelation::~CRelation(){}

inline CElement * CRelation::GetContext()		{return context;}
inline CElement * CRelation::GetSource()		{return source;}
inline CElement * CRelation::GetDestination()	{return destination;}


//-----FormulaSet
inline CFormulaSet::CFormulaSet(CString &n): CConstraintSet(n)		{type = typeFormulaSet;}
inline CFormulaSet::CFormulaSet(const TCHAR *n):  CConstraintSet(n)		{type = typeFormulaSet;}
inline CFormulaSet::CFormulaSet(CString &n, long id, long e_id)		:  CConstraintSet(n, id, e_id)		{type = typeFormulaSet;}
inline CFormulaSet::CFormulaSet(const TCHAR *n, long id, long e_id)	:  CConstraintSet(n, id, e_id)		{type = typeFormulaSet;}	
inline CFormulaSet::~CFormulaSet()									{Destroy(formulas);}

inline CFormulaList&	CFormulaSet::GetFormulas()						{return formulas;}
inline CFormula*		CFormulaSet::FindFormula(long i)					{CFormula *ret;  return Find(formulas, ret, i) ? ret : 0;};
inline CFormula*		CFormulaSet::FindFormula(const TCHAR *n)			{CFormula *ret;  return Find(formulas, ret, n) ? ret : 0;};
inline CFormula*		CFormulaSet::RemoveFormula(CFormula *e)			{return Remove(formulas, e) ? e : 0;};
inline CFormula*		CFormulaSet::RemoveFormula(long i)					{CFormula *e; return Remove(formulas, e, i) ? e : 0;};
inline CFormula*		CFormulaSet::RemoveFormula(const TCHAR *n)			{CFormula *e;  return Remove(formulas, e, n) ? e : 0;};
inline CFormula*		CFormulaSet::InsertFormula(CFormula *e)			{return Insert(formulas, e) ? e : 0;};


//bool CConstraintSet::Verify(CCSetErrDialog * csed)
//{
//
//  bool ret=true;
//  POSITION pos = constraints.GetHeadPosition();
//  while(pos)
//  {
//    POSITION curpos=pos;
//    CConstraint *cur = constraints.GetNext(pos);
//	CString err_str, what;
//
//    if (cur->Verify(err_str, what)) continue;
//
//	if (what.GetLength())
//		csed->AddError(*this,*cur, *(cur->GetContext()), (LPCTSTR)err_str,  what);
//	else
//		csed->AddError(*this,*cur, *(cur->GetContext()), (LPCTSTR)err_str,  NULL);
//
//    ret = false;
//    constraints.RemoveAt(curpos);
//    delete cur;
//  }
//  return ret;
//}


//-----Formula
inline CFormula::CFormula(CString& n, CFormulaSet *s, CElement* c, CString& t)
		:  CConstraint(n, s, c, t), expression(0)
{
	type = typeFormula;
}

inline CFormula::CFormula(const TCHAR *n, CFormulaSet *s, CElement* c, CString& t)
		: CConstraint(n,s,c,t), expression(0)
{
	type = typeFormula;
	//implicit = (s->GetName().Compare(ICS_NAME) == 0);
}

inline CFormula::CFormula(CString& n, CFormulaSet *s, CElement* c, const TCHAR *t)
		:CConstraint(n,s,c,t), expression(0)
{
	type = typeFormula;
//	implicit = (s->GetName().Compare(ICS_NAME) == 0);
}

inline CFormula::CFormula(const TCHAR *n, CFormulaSet *s, CElement* c, const TCHAR *t)
		: CConstraint(n,s,c,t), expression(0)	
{
	type = typeFormula;
//	implicit = (s->GetName().Compare(ICS_NAME) == 0);
}

inline CFormula::CFormula(CString& n, CFormulaSet *s, CElement* c, CString& t, long id, long e_id)
		: CConstraint(n,s,c,t,id,e_id), expression(0)	
{
	type = typeFormula;
//	implicit = (s->GetName().Compare(ICS_NAME) == 0);
}

inline CFormula::CFormula(const TCHAR *n, CFormulaSet *s, CElement* c, CString& t, long id, long e_id)
		: CConstraint(n,s,c,t,id,e_id), expression(0)	
{
	type = typeFormula;
//	implicit = (s->GetName().Compare(ICS_NAME) == 0);
}

inline CFormula::CFormula(CString& n, CFormulaSet *s, CElement* c, const TCHAR *t, long id, long e_id)
			: CConstraint(n,s,c,t,id,e_id), expression(0)	
{
	type = typeFormula;
//	implicit = (s->GetName().Compare(ICS_NAME) == 0);
}

inline CFormula::CFormula(const TCHAR *n, CFormulaSet *s, CElement* c, const TCHAR *t, long id, long e_id)
			: CConstraint(n,s,c,t,id,e_id), expression(0)	
{
	type = typeFormula;
//	implicit = (s->GetName().Compare(ICS_NAME) == 0);
}
	
inline CFormula::CFormula(const TCHAR *n, CElement* c, const TCHAR *t)
	: CConstraint(n,c,t), expression(0)	
{
	type = typeFormula;
//	implicit = (s->GetName().Compare(ICS_NAME) == 0);
}

inline CFormula::~CFormula()	 {delete expression;}

inline CFormulaSet *	CFormula::GetSet()					{return set;};
inline ClFormula *		CFormula::GetExpression()			{return expression;};

bool CFormula::Parse(void)
{
  bool ret = ParseMCLString(text);
  if (!ret) Error( _T("CFormula::Parse"), _T("Formula <%s> in <%s>: %s"), name,
                   (const TCHAR *)*context, GetMCLErrorMessage());
  else
  {
    RemoveCRLF(text);
    ClConstraint* cexpr = ClConstraint::GetLastConstraint();
		
	expression = cexpr->Clone();
    //ASSERT_EX( expression, _T("CConstraint::Parse"), _T("expression is nul"));
	if (!expression)
		throw new CDesertException(_T("CConstraint::Parse(): expression is NULL"));
  }
  return ret;
}

//--------------CSimpleFormala
inline CSimpleFormula::CSimpleFormula()
	: CStaticObj(_T("SimpleFormula"))	{ type = typeSimpleFormula;}
inline CSimpleFormula::CSimpleFormula(const TCHAR *fn)
	: CStaticObj(_T("SimpleFormula")),pcm_fn(fn)	{ type = typeSimpleFormula;}

inline CSimpleFormula::~CSimpleFormula(){}
inline void CSimpleFormula::InsertSourceProperty(CProperty *srcp)		{srclist.AddTail(srcp);}
inline void CSimpleFormula::InsertDestinationProperty(CProperty *dstp)	{dstlist.AddTail(dstp);}
inline CPropertyList& CSimpleFormula::GetSourceList()					{return srclist;}
inline CPropertyList& CSimpleFormula::GetDestinationList()				{return dstlist;}
inline const FunctionType CSimpleFormula::GetFuncType() const 
{ 
	if (pcm_fn == _T("PCM_ADD"))				//arithmetical sum
		return funcPCMAdd;			
	
	else if (pcm_fn == _T("PCM_MUL"))			//arithmetical product
		return funcPCMMul;

	else if (pcm_fn == _T("PCM_AMED"))			//arithmetical medium
		return funcPCMAmed;

	else if (pcm_fn == _T("PCM_GMED"))			//geomethric medium
		return  funcPCMGmed;

	else if (pcm_fn == _T("PCM_MIN"))			//arithmetic minimum
		return funcPCMMin;

	else if (pcm_fn == _T("PCM_MAX"))			//arithmetic maximum
		return funcPCMMax;	
	else
		return funcNofunc;
};