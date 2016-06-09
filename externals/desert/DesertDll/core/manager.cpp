// -*-C++-*-
// manager.cpp
// Implementation of manager class

//UDM stuff

//#include <string>
//#include "../iface/DesertIfaceBack.h"


#include "stdafx.h"
#include "desert.h"

#include <math.h>
#include <signal.h>
#include "CSetErrDialog.h"
#include "core/manager.h"
#include "core/clmain.h"
#include "core/static.h"
#include "core/dynamic.h"
#include "common/config.h"


#include "BackIface.h"



#pragma warning(disable : 4244)

bool fp_except = false;
extern "C" extern void fp_handler( int sig )
{
	Warning(_T("fp_handler"), _T("Floating point Exception"));
	fp_except = true;
}

//global list of CManager generations
CList<int, int> CManager::generations;
CManager *CManager::theInstance=NULL;
int CManager::unique=1;

static TCHAR *setNames[] =
{
  _T("Undef"),
  _T("UniSpace"),
  _T("BiSpace"),
  _T("UniDomain"),
  _T("SpaceDomain"),
  _T("BiSpaceDomain"),
  _T("BiDomain"),
  _T("SpaceBiDomain"),
  _T("BiSpaceBiDomain")
};

#define MAX_LONG (long)(1 << 30)
#define DESIGN_SPACE_TOO_LARGE -2

CManager::
CManager() : CCosmic()
{
  type = typeManager;
  //currentGeneration = -1;
  //prevGeneration = -1;
  //nextGeneration = -1;
  ui_refresh_needed = true;
  currentGenerationPosition = NULL;
  designSpaceSize = 0;

  ClFunction::CreateAll();
}

CManager::
~CManager()
{
  Destroy(baseSpaces);
  Destroy(baseDomains);
  Destroy(baseConstraintSets);
  // FIXME: Destroy(baseFormulaSets); ?
  Destroy(baseFormulaSets);
  Destroy(simpleFormulas);
  for (int i=0; i<MAX_GENERATIONS; i++)
  {
	  // FIXME Destroy dynContainers[i] ?
    Destroy(dynSpaces[i]);
    Destroy(dynDomains[i]);
    Destroy(dynConstraintSets[i]);
  }
  ClFunction::DestroyAll();

  generations.RemoveAll();
}

CCosmicList* CManager::GetContainer(const TCHAR * ContainerName)
{
	CCosmicList *list = NULL;



	if (generations.IsEmpty())
	{
		CElementContainer* cont;
		bool ret = Find(baseContainers, cont, ContainerName);
		if (!ret) return NULL;
		list = (CCosmicList*)(cont->GetRootElements(0));
	}
	else
	{
		GET_CURRENTGENERATION

		CDynElementContainer* cont;
		bool ret = Find(dynContainers[currentGeneration], cont, ContainerName);
		if (!ret) return NULL;
		list = (CCosmicList*)(cont->GetRootElements(0));

	}
	return list;
};

CCosmicList* CManager::GetContainer(CString &ContainerName)
{
	CCosmicList *list = NULL;

	if (generations.IsEmpty())
	{
		CElementContainer* cont;
		bool ret = Find(baseContainers, cont, ContainerName);
		if (!ret) return NULL;
		list = (CCosmicList*)(cont->GetRootElements(0));
	}
	else
	{
		GET_CURRENTGENERATION

		CDynElementContainer* cont;
		bool ret = Find(dynContainers[currentGeneration], cont, ContainerName);
		if (!ret) return NULL;
		list = (CCosmicList*)(cont->GetRootElements(0));

	}
	return list;
};


long CManager::CreateSpace(CString &n)
{
  CSpace *s = new CSpace(n);
  ASSERT_EX( s != NULL, _T("CManager::Space"), _T("new CSpace failed") );
  baseSpaces.AddTail(s);
  baseContainers.AddTail(s);
  //create a containment function
  //so project().space_name() .. can work
  ClFunction::CreateContainer(n);
  return *s;
}

long CManager::CreateSpace(const TCHAR *n)
{
	CString cs= n;
  return CreateSpace( cs);
}


long CManager::CreateElement(CString &n, long s, short d, long p, long e)
{
	CElementContainer * container = 0 ;
	bool ret;

	//check for correct decomposition
	//ASSERT_EX( d>=0 && d<=2, _T("CManager::CreateElement"), _T("invalid decomposition") );
	if (d < 0 || d >2) throw new CDesertException( _T("CManager::CreateElement(): invalid decompostion"));

	//find conatiner
	ret = Find(baseContainers, container, s);

	//ASSERT_EX( ret, _T("CManager::Element"), _T("Container (space _or_ domain) not found") );
	if (!ret) throw new CDesertException(_T("CManager::CreateElement(): Container space or domain not found!"));

	//check for alternatives in domains
	//ASSERT_EX( container->GetType() != typeCustomDomain|| d<2, _T("CManager::CreateElement"), _T("Alternatives not yet supported in Domains") );
	if ((container->GetType() == typeCustomDomain) && (d >=2))
		throw new CDesertException(_T("CManager::CreateElement(): Alternatives not yet supported in Domains!"));

	//find parent
	CElement *parent  = container->FindElement(p);
	//create element
	CElement * el =	new CElement(n, container, (CDecomposition)d,  parent, e);
	//assertation if creation failed
	//ASSERT_EX( el, _T("CManager::Element"), _T("new CElement failed") );
	if (!el)
		throw new CDesertException(_T("CManager::CreateElement(): Create Element failed!"));
	//insert into container
	container->InsertElement(el);
	//insert into parent
	if (parent) parent->InsertChild(el);
	//return new element
	return *el;
}


long CManager::CreateElement(const TCHAR *n, long s, short d, long p, long e)
{
	CString cs= n;
  return CreateElement(cs, s, d, p, e);
}

long CManager::CreateNaturalDomain(CString& n, int mx, int mn)
{
  CDomain *d = new CNaturalDomain(n, mx, mn);
 // ASSERT_EX( d, _T("CManager::NaturalDomain"), _T("new CNaturalDomain failed") );
  if (!d)
	  throw new CDesertException(_T("CManager::NaturalDomain(): Create NaturalDomain failed!"));
  baseDomains.AddTail(d);
  return *d;
}

long CManager::CreateNaturalDomain(const TCHAR *n, int mx, int mn)
{
	CString cs= n;
  return CreateNaturalDomain( cs, mx, mn );
}


long CManager::CreateCustomDomain(CString& n)
{
  CDomain *d = new CCustomDomain(n);
//  ASSERT_EX( d, _T("CManager::CustomDomain"), _T("new CCustomDomain failed") );
  if (!d)
	  throw new CDesertException(_T("CManager::NaturalDomain(): Create CustomDomain failed!"));
  baseDomains.AddTail(d);
  baseContainers.AddTail((CCustomDomain*)d);
  //Create a function for this customdomain
  //so project().domain-name() could work
  ClFunction::CreateContainer(n);

  return *d;
}


long CManager::CreateCustomDomain(const TCHAR *n)
{
		CString cs= n;
  return CreateCustomDomain( cs );
}




long CManager::CreateConstantProperty(CString& n, CString& pcm_fn, long o, long d, int v)
{
  CDomain *domain = 0;
  bool ret = Find(baseDomains, domain, d);

  //ASSERT_EX( ret, _T("CManager::ConstantProperty"), _T("domain not found") );
  if (!ret)
	  throw new CDesertException(_T("CManager::ConstantProperty(): domain not found!"));

  CElement *owner = FindElement(baseSpaces, o); // what about property of domain elements???
  if (!owner)
	  throw new CDesertException(_T("CManager::ConstantProperty(): owner not found!"));


 // ASSERT_EX( owner, _T("CManager::ConstantProperty"), _T("owner not found") );

  CProperty *property = new CConstantProperty(n, pcm_fn, domain, owner, v);
  if (!property)
	  throw new CDesertException(_T("CManager::ConstantProperty(): Create Constant Property Failed!"));

  //ASSERT_EX( property, _T("CManager::ConstantProperty"), _T("new CConstantProperty failed") );
  owner->InsertProperty(property);

  // Create a clfunction object for the built-in pcm functions
  ClFunction::CreatePCMFn(n, pcm_fn);

  return *property;
}

long CManager::CreateConstantProperty(const TCHAR *n, const TCHAR *pcm_fn, long o, long d, int v)
{
		CString cs= n;
		CString cs_pcm_fn= pcm_fn;
  return CreateConstantProperty( cs, cs_pcm_fn, o, d, v);
}

long
CManager::
CreateVariableProperty(CString& n, CString& pcm_fn, long o, long d)
{
  CDomain *domain = 0;
  bool ret = Find(baseDomains, domain, d);
  //ASSERT_EX( ret, _T("CManager::VariableProperty"), _T("domain not found") );
  if (!ret)
	  throw new CDesertException(_T("CManager::ConstantProperty(): domain not found!"));


  CElement *owner = FindElement(baseSpaces, o); // what about property of domain elements???
  //ASSERT_EX( owner, _T("CManager::VariableProperty"), _T("owner not found") );
   if (!owner)
	  throw new CDesertException(_T("CManager::ConstantProperty(): owner not found!"));



  CProperty *property = new CVariableProperty(n, pcm_fn, domain, owner);
  //ASSERT_EX( property, _T("CManager::VariableProperty"), _T("new CVariableProperty failed") );
	if (!property)
	  throw new CDesertException(_T("CManager::ConstantProperty(): Create Constant Property Failed!"));



  owner->InsertProperty(property);

  // Create a clfunction object for the built-in pcm functions
  ClFunction::CreatePCMFn(n, pcm_fn);

  return *property;
}

long CManager::CreateVariableProperty(const TCHAR *n, const TCHAR *pcm_fn, long o, long d)
{
	CString cs= n;
	CString cs_pcm_fn= pcm_fn;
	return CreateVariableProperty( cs, cs_pcm_fn, o, d );
}

long
CManager::
CreateVariableProperty(CString& n, CString& pcm_fn, long o)
{
  CElement *owner = FindElement(baseSpaces, o); // what about property of domain elements???
  //ASSERT_EX( owner, _T("CManager::VariableProperty"), _T("owner not found") );
   if (!owner)
	  throw new CDesertException(_T("CManager::ConstantProperty(): owner not found!"));

  CProperty *property = new CVariableProperty(n, pcm_fn, owner);
  //ASSERT_EX( property, _T("CManager::VariableProperty"), _T("new CVariableProperty failed") );
	if (!property)
	  throw new CDesertException(_T("CManager::ConstantProperty(): Create Constant Property Failed!"));

  owner->InsertProperty(property);
  ClFunction::CreatePCMFn(n, _T("PCM_ADD"));
  return *property;
}

long CManager::CreateVariableProperty(const TCHAR *n, const TCHAR *pcm_fn, long o)
{
	CString cs= n;
	CString cs_pcm_fn= pcm_fn;
	return CreateVariableProperty( cs, cs_pcm_fn, o);
}

long CManager::CreateParametricVariableProperty(const TCHAR *name, long o,  const TCHAR * text)
{
	CElement *owner = FindElement(baseSpaces, o); // what about property of domain elements???
  //ASSERT_EX( owner, _T("CManager::VariableProperty"), _T("owner not found") );
   if (!owner)
	  throw new CDesertException(_T("CManager::CreateParametricVariableProperty(): owner not found!"));

   CFormula *formula = new CFormula(name, owner, text);

  //ASSERT_EX( property, _T("CManager::VariableProperty"), _T("new CVariableProperty failed") );
	if (!formula)
	  throw new CDesertException(_T("CManager::CreateParametricVariableProperty(): Create Formula Failed!"));


   bool ret = formula->Parse();
	  if (!ret)
	  {
		delete formula;
		return -1;
	  }

  owner->InsertFormula(formula);

  CString cs= name;
	CString cs_pcm_fn= _T("PCM_ADD");
	 CProperty *prop = new CVariableProperty(cs, cs_pcm_fn, owner);
	owner->InsertProperty(prop);

  // Create a clfunction object for the built-in pcm functions
  ClFunction::CreatePCMFn(name, _T("PCM_ADD"));
  return *prop;
}

long CManager::CreateRelation(long c, long s, long d)
{
  CElement *sp_ctx = FindElement(baseSpaces, c);
  CElement *dm_ctx = FindElement(baseDomains, c);
  CRelation *rel = NULL;

  if (sp_ctx)
  {
    // context is a space element -- relations must also be between space element
    CElement *source = FindElement(baseSpaces, s);
    //ASSERT_EX( source, _T("CManager::CreateRelation"), _T("source not found") );
	if (!source) throw new CDesertException(_T("CManager::CreateRelation(): Relation source not found!"));

    CElement *destination = FindElement(baseSpaces, d);
    //ASSERT_EX( destination, _T("CManager::CreateRelation"), _T("destination not found") );
	if (!destination) throw new CDesertException(_T("CManager::CreateRelation(): Relation destination not found!"));

	rel = new CRelation(sp_ctx, source, destination);

	sp_ctx->InsertRelation(rel);

  }
  else if (dm_ctx)
  {
    // context is a domain element -- relations must also be between domain elements
    CElement *source = FindElement(baseDomains, s);
    //ASSERT_EX( source, _T("CManager::CreateRelation"), _T("source not found") );
	if (!source) throw new CDesertException(_T("CManager::CreateRelation(): Relation source not found!"));

    CElement *destination = FindElement(baseDomains, d);
    //ASSERT_EX( destination, _T("CManager::CreateRelation"), _T("destination not found") );
	if (!destination) throw new CDesertException(_T("CManager::CreateRelation(): Relation destination not found!"));

    rel = new CRelation(dm_ctx, source, destination);

    dm_ctx->InsertRelation(rel);
  }
  else
    //ASSERT_EX( false, _T("CManager::CreateRelation"), _T("context not found") );
	throw new CDesertException(_T("CManager::CreateRelation(): context not found!"));

  //ASSERT_EX( rel, _T("CManager::CreateRelation"), _T("new failed") );

  if (!rel) throw new CDesertException(_T("CManager::CreateRelation(): Create relation failed!"));

  return *rel;
}

long CManager::CreateConstraintSet(CString& n)
{
  CConstraintSet *s = new CConstraintSet(n);
 // ASSERT_EX( s, _T("CManager::ConstraintSet"), _T("new CConstraintSet failed") );

  if (!s) throw new CDesertException();
  baseConstraintSets.AddTail(s);
  return *s;
}

long CManager::CreateConstraintSet(const TCHAR *n)
{
  CConstraintSet *s = new CConstraintSet(n);
  ASSERT_EX( s, _T("CManager::ConstraintSet"), _T("new CConstraintSet failed") );
  baseConstraintSets.AddTail(s);
  return *s;
}

//////
long CManager::CreateFormulaSet(CString& n)
{
  CFormulaSet *s = new CFormulaSet(n);
 // ASSERT_EX( s, _T("CManager::ConstraintSet"), _T("new CConstraintSet failed") );

  if (!s) throw new CDesertException();
  baseFormulaSets.AddTail(s);
  return *s;
}

long CManager::CreateFormulaSet(const TCHAR *n)
{
  CFormulaSet *s = new CFormulaSet(n);
  ASSERT_EX( s, _T("CManager::FormulaSet"), _T("new CFormulaSet failed") );
  baseFormulaSets.AddTail(s);
  return *s;
}
//////

long CManager::CreateConstraint(CString& n, long s, long c, CString& t)
{
  CConstraintSet *set = 0;
  bool ret = Find(baseConstraintSets, set, s);
  ASSERT_EX( ret, _T("CManager::Constraint"), _T("constraint set not found") );
  CElement *context = FindElement(baseSpaces, c);
  if (!context) context = FindElement(baseDomains, c);
  ASSERT_EX( context, _T("CManager::Constraint"), _T("context not found") );
  CConstraint *constraint = new CConstraint(n, set, context, t);
  ASSERT_EX( constraint, _T("CManager::Constraint"), _T("new CConstraint failed") );
  ret = constraint->Parse();
  if (!ret)
  {
    delete constraint;
    return -1;
  }
  set->InsertConstraint(constraint);
  return *constraint;
}

long
CManager::
CreateConstraint(const TCHAR *n, long s, long c, CString& t)
{
	CString cs= n;
	return CreateConstraint( cs, s, c, t );
}




long CManager::CreateConstraint(CString& n, long s, long c, const TCHAR *t)
{
		CString cs_t= t;
  return CreateConstraint( n, s, c, cs_t );
}

long CManager::CreateConstraint(const TCHAR *n, long s, long c, const TCHAR *t)
{
	CString cs= n;
	CString cs_t= t;
  return CreateConstraint( cs, s, c, cs_t );
}

//////CreateFormula
long CManager::CreateFormula(CString& n, long s, long c, CString& t)
{
  CFormulaSet *set = 0;
  bool ret = Find(baseFormulaSets, set, s);
  ASSERT_EX( ret, _T("CManager::Formula"), _T("Formula set not found") );
  CElement *context = FindElement(baseSpaces, c);
  if (!context) context = FindElement(baseDomains, c);
  ASSERT_EX( context, _T("CManager::Formula"), _T("context not found") );
  CFormula *formula = new CFormula(n, set, context, t);
  ASSERT_EX( formula, _T("CManager::Formula"), _T("new CFormula failed") );

  ret = formula->Parse();
  if (!ret)
  {
    delete formula;
    return -1;
  }
  set->InsertFormula(formula);
  return *formula;
}

long CManager::CreateFormula(const TCHAR *n, long s, long c, CString& t)
{
	CString cs= n;
	return CreateFormula( cs, s, c, t );
}

long CManager::CreateFormula(CString& n, long s, long c, const TCHAR *t)
{
	CString cs_t= t;
	return CreateFormula( n, s, c, cs_t );
}

long CManager::CreateFormula(const TCHAR *n, long s, long c, const TCHAR *t)
{
	CString cs= n;
	CString cs_t= t;
	return CreateFormula( cs, s, c, cs_t );
}
//////
#ifndef DOUBLE_MTBDD
long CManager::AddtoVariableProperty(const TCHAR *n, CString &name, long o, long v, long d)
{
	CDomain *domain = 0;
	bool ret = Find(baseDomains, domain, d);
	//ASSERT_EX( ret, _T("CManager::AddToVariableProperty"), _T("domain not found") );
	if (!ret) throw new CDesertException(_T("CManager::AddToVariableProperty(): domain not found") );


	CElement *owner = FindElement(baseSpaces, o); // what about property of domain elements??? - they don't exist!
	//ASSERT_EX( owner, _T("CManager::AddToVariableProperty"), _T("owner not found") );
	if (!owner) throw new CDesertException(_T("CManager::AddToVariableProperty(): owner not found"));

	CVariableProperty *property = (CVariableProperty *)owner->FindProperty(n);
	//ASSERT_EX( property && property->IsType(typeVariableProperty), _T("CManager::AddToVariableProperty"), _T("property not found") );
	if (!property || !property->IsType(typeVariableProperty))
		throw new CDesertException(_T("CManager::AddToVariableProperty(): property not found"));

	CElement *value = domain->FindElement(v);
	//ASSERT_EX( value || domain->IsType(typeNaturalDomain), _T("CManager::AddToVariableProperty"), _T("value not found in domain and domain is not a natural domain") );

	if (!value && !domain->IsType(typeNaturalDomain))
		throw new CDesertException(_T("CManager::AddToVariableProperty(): Value not found in custom domain!"));

	if (value)
	return property->AddToRange(value, name);
	else
	return property->AddToRange(v, name);

};
long CManager::AddtoVariableProperty(long p, CString &name, long o, long v, long d)
{
	CDomain *domain = 0;
	bool ret = Find(baseDomains, domain, d);
	//ASSERT_EX( ret, _T("CManager::AddToVariableProperty"), _T("domain not found") );
	if (!ret) throw new CDesertException(_T("CManager::AddToVariableProperty(): domain not found") );

	CElement *owner = FindElement(baseSpaces, o); // what about property of domain elements???
	//ASSERT_EX( owner, _T("CManager::AddToVariableProperty"), _T("owner not found") );
	if (!owner) throw new CDesertException(_T("CManager::AddToVariableProperty(): owner not found"));

	CVariableProperty *property = (CVariableProperty *)owner->FindProperty(p);
	//ASSERT_EX( property && property->IsType(typeVariableProperty), _T("CManager::AddToVariableProperty"), _T("property not found") );
	if (!property || !property->IsType(typeVariableProperty))
		throw new CDesertException(_T("CManager::AddToVariableProperty(): property not found"));

	CElement *value = domain->FindElement(v);
	//ASSERT_EX( value || domain->IsType(typeNaturalDomain), _T("CManager::AddToVariableProperty"), _T("value not found in domain and domain is not a natural domain") );
	if (!value && !domain->IsType(typeNaturalDomain))
		throw new CDesertException(_T("CManager::AddToVariableProperty(): Value not found in custom domain!"));



	if (value)
		return property->AddToRange(value, name);
	else
		return property->AddToRange(v, name);

};

long CManager::AddtoVariableProperty(const TCHAR *p, const TCHAR *n, long o, long v, long d)
{
		CString cs= n;
	return CManager::AddtoVariableProperty( p, cs,o, v, d);
};

long CManager::AddtoVariableProperty(long p, const TCHAR *n, long o, long v, long d)
{
	CString cs= n;
	return CManager::AddtoVariableProperty( p, cs, o, v, d);
};
#else
long CManager::AddtoVariableProperty(const TCHAR *n, CString &name, long o, double v, long d)
{
	CDomain *domain = 0;
	bool ret = Find(baseDomains, domain, d);
	//ASSERT_EX( ret, _T("CManager::AddToVariableProperty"), _T("domain not found") );
	if (!ret) throw new CDesertException(_T("CManager::AddToVariableProperty(): domain not found") );


	CElement *owner = FindElement(baseSpaces, o); // what about property of domain elements??? - they don't exist!
	//ASSERT_EX( owner, _T("CManager::AddToVariableProperty"), _T("owner not found") );
	if (!owner) throw new CDesertException(_T("CManager::AddToVariableProperty(): owner not found"));

	CVariableProperty *property = (CVariableProperty *)owner->FindProperty(n);
	//ASSERT_EX( property && property->IsType(typeVariableProperty), _T("CManager::AddToVariableProperty"), _T("property not found") );
	if (!property || !property->IsType(typeVariableProperty))
		throw new CDesertException(_T("CManager::AddToVariableProperty(): property not found"));

	//CElement *value = domain->FindElement(v);
	////ASSERT_EX( value || domain->IsType(typeNaturalDomain), _T("CManager::AddToVariableProperty"), _T("value not found in domain and domain is not a natural domain") );

	//if (!value && !domain->IsType(typeNaturalDomain))
	//	throw new CDesertException(_T("CManager::AddToVariableProperty(): Value not found in custom domain!"));
	CElement *value = 0;
	if(!domain->IsType(typeNaturalDomain))
	{
		long lv = long(v);
		double diff = v - lv;
		if(diff == 0)
			value = domain->FindElement(lv);
		if(!value)
			throw new CDesertException(_T("CManager::AddToVariableProperty(): Value not found in custom domain!"));
	}

	if (value)
		return property->AddToRange(value, name);
	else
		return property->AddToRange(v, name);
};

long CManager::AddtoVariableProperty(long p, CString &name, long o, double v, long d)
{
	CDomain *domain = 0;
	bool ret = Find(baseDomains, domain, d);
	//ASSERT_EX( ret, _T("CManager::AddToVariableProperty"), _T("domain not found") );
	if (!ret) throw new CDesertException(_T("CManager::AddToVariableProperty(): domain not found") );

	CElement *owner = FindElement(baseSpaces, o); // what about property of domain elements???
	//ASSERT_EX( owner, _T("CManager::AddToVariableProperty"), _T("owner not found") );
	if (!owner) throw new CDesertException(_T("CManager::AddToVariableProperty(): owner not found"));

	CVariableProperty *property = (CVariableProperty *)owner->FindProperty(p);
	//ASSERT_EX( property && property->IsType(typeVariableProperty), _T("CManager::AddToVariableProperty"), _T("property not found") );
	if (!property || !property->IsType(typeVariableProperty))
		throw new CDesertException(_T("CManager::AddToVariableProperty(): property not found"));

	//CElement *value = domain->FindElement(v);
	////ASSERT_EX( value || domain->IsType(typeNaturalDomain), _T("CManager::AddToVariableProperty"), _T("value not found in domain and domain is not a natural domain") );
	//if (!value && !domain->IsType(typeNaturalDomain))
	//	throw new CDesertException(_T("CManager::AddToVariableProperty(): Value not found in custom domain!"));
	CElement *value = 0;
	if(!domain->IsType(typeNaturalDomain))
	{
		long lv = long(v);
		double diff = v - lv;
		if(diff == 0)
			value = domain->FindElement(lv);
		if(!value)
			throw new CDesertException(_T("CManager::AddToVariableProperty(): Value not found in custom domain!"));
	}

	if (value)
		return property->AddToRange(value, name);
	else
		return property->AddToRange(v, name);

};

long CManager::AddtoVariableProperty(const TCHAR *p, const TCHAR *n, long o, double v, long d)
{
	CString cs= n;
	return CManager::AddtoVariableProperty( p, cs,o, v, d);
};

long CManager::AddtoVariableProperty(long p, const TCHAR *n, long o, double v, long d)
{
	CString cs= n;
	return CManager::AddtoVariableProperty( p, cs, o, v, d);
};
#endif

CProperty * CManager::GetProperty(long p, long o)
{
	CElement *owner = FindElement(baseSpaces, o); // what about property of domain elements???
	//ASSERT_EX( owner, _T("CManager::AddToVariableProperty"), _T("owner not found") );
	if (!owner) throw new CDesertException(_T("CManager::AddToVariableProperty(): owner not found"));

	CProperty *vp = (CProperty *)owner->FindProperty(p);
	return vp;
}

long CManager::CreateSimpleFormula(const TCHAR *fnType, CMap<long, long, long, long> &srcProps, CMap<long, long, long, long> &dstProps)
{
	CSimpleFormula *sf = new CSimpleFormula(fnType);
	POSITION pos = srcProps.GetStartPosition();
	while(pos)
	{
		long prop, owner;
		srcProps.GetNextAssoc(pos, prop, owner);
		CProperty *sp = GetProperty(prop, owner);
		if(sp)
			sf->InsertSourceProperty(sp);
	}
	pos = dstProps.GetStartPosition();
	while(pos)
	{
		long prop, owner;
		dstProps.GetNextAssoc(pos, prop, owner);
		CProperty *dp = GetProperty(prop, owner);
		if(dp)
			sf->InsertDestinationProperty(dp);
	}
	simpleFormulas.AddTail(sf);
	return *sf;
}

long CManager::AddtoSimpleFormulaVariableProperty(long p, const TCHAR *n, long o, long sp, long so)
{
	CElement *owner = FindElement(baseSpaces, o); // what about property of domain elements???
	//ASSERT_EX( owner, _T("CManager::AddToVariableProperty"), _T("owner not found") );
	if (!owner) throw new CDesertException(_T("CManager::AddToVariableProperty(): owner not found"));

	CVariableProperty *vp = (CVariableProperty *)owner->FindProperty(p);
	//ASSERT_EX( property && property->IsType(typeVariableProperty), _T("CManager::AddToVariableProperty"), _T("property not found") );
	if (!vp|| !vp->IsType(typeVariableProperty))
		throw new CDesertException(_T("CManager::AddToVariableProperty(): property not found"));

	CElement *src_owner = FindElement(baseSpaces, so); // what about property of domain elements???
	//ASSERT_EX( owner, _T("CManager::AddToVariableProperty"), _T("owner not found") );
	if (!src_owner) throw new CDesertException(_T("CManager::AddToVariableProperty(): owner not found"));

	CVariableProperty *src_vp = (CVariableProperty *)src_owner->FindProperty(sp);
	//ASSERT_EX( property && property->IsType(typeVariableProperty), _T("CManager::AddToVariableProperty"), _T("property not found") );
	if (!src_vp|| !src_vp->IsType(typeVariableProperty))
		throw new CDesertException(_T("CManager::AddToVariableProperty(): property not found"));

	vp->AddToPropertyList(src_vp);
};

double CManager::ComputeSize() const
{
  double ret = 1;
  if (generations.IsEmpty())
  {
    POSITION pos = baseSpaces.GetHeadPosition();
    while(pos) ret *= baseSpaces.GetNext(pos)->ComputeSize();
  }
  else
  {
	  GET_CURRENTGENERATION

    POSITION pos = dynSpaces[currentGeneration].GetHeadPosition();
    while(pos) ret *= dynSpaces[currentGeneration].GetNext(pos)->ComputeSize();
  }
  return ret;
}

double CManager::ComputeSize(long elid)
{
  CElement *elm = FindElement(elid);

  if (generations.IsEmpty())
    return elm ? elm->ComputeSize() : 0;

  CDynElement *delm = elm ? FindElement(elm) : 0;
  return delm ? delm->ComputeSize() : 0;
}

bool CManager::VerifyConstraints(CCSetErrDialog *  csed)
{
  bool ret=true;
  POSITION pos = baseConstraintSets.GetHeadPosition();
   while(pos) ret = ret && baseConstraintSets.GetNext(pos)->Verify(csed);
  return ret;
}

void CManager::AnalyseConstraints()
{
  POSITION pos = baseConstraintSets.GetHeadPosition();
  while(pos) baseConstraintSets.GetNext(pos)->Analyse();
  SortConstraints();
}

void CManager::SortConstraints()
{
  int i;
  CConstraintSet *sets[9];
  for(i=0; i<9; i++) sets[i] = 0;

  POSITION pos = baseConstraintSets.GetHeadPosition();
  while(pos)
  {
    CConstraintSet *bset = baseConstraintSets.GetNext(pos);
    CConstraintList& list = bset->GetConstraints();
    POSITION pos1 = list.GetHeadPosition();
    while(pos1)
    {
      CConstraint *cur = list.GetNext(pos1);
      int consType = (int)cur->GetConstraintType();
      if (!sets[consType]) sets[consType] = new CConstraintSet(setNames[consType]);
      sets[consType]->InsertConstraint(cur);
    }
    list.RemoveAll();
  }
  ::Destroy(baseConstraintSets);

  for(i=0; i<9; i++)
    if (sets[i]) baseConstraintSets.AddTail(sets[i]);
}

bool CManager::IsDone()
{
  Todo(_T("CManager::IsDone"), _T("implement this"));
//  return currentGeneration < 0 ? false : true;
  return generations.IsEmpty() ? false : true;
}

bool CManager::HasGenerations()
{
	return generations.IsEmpty() ? false : true;
}

void CManager::ShowStats()
{
  double size = ComputeSize();
  _tprintf(_T("\n\nSize of the design space = %2.2e\n"), size);
  Todo(_T("CManager::ShowStats"), _T("stats on the active constraints etc."));
}

int CManager :: GetCurrStepsBack()
{
	//see where are we in the list of generations
	POSITION pos = currentGenerationPosition;
	int curr_steps_back = 0;
	while (pos && (curr_steps_back < MAX_GENERATIONS))
	{
		int i = generations.GetNext(pos);
		if(pos)curr_steps_back++;
	}
	return curr_steps_back ;

}
bool CManager :: IsBackNavigable()
{
	/*
		1. list is not empty
		2. no more then MAX_GENERATION steps

	*/

	//if list is empty, than it's not navigable
	if (generations.IsEmpty()) return false;
	//if ther's no current position
	if (!currentGenerationPosition) return false;
	//if there's only one configurations
	if (generations.GetCount() <= 1) return false;
	//if we are at the first configuration
	if (currentGenerationPosition == generations.GetHeadPosition()) return false;


	int curr_steps_back =	GetCurrStepsBack();

	if(!curr_steps_back)
	{
		//if this is the first back-navigation,
		//we check for two steps back, the last one is just a copy of the previous one
		if (generations.GetCount() <= 2) return false;
		//if (generations.GetCount() < 2) return false;
	}

	//array boundary check

	return ( curr_steps_back < MAX_GENERATIONS);

};

bool CManager :: IsForwardNavigable()
{
	//if list is empty, than it's not navigable
	if (generations.IsEmpty()) return false;
	//if ther's no current position
	if (!currentGenerationPosition) return false;

	POSITION  pos = currentGenerationPosition;
	int i = generations.GetNext(pos);
	return (pos != NULL);
};

void CManager :: NavigateBack()
{
	//check
	//ASSERT_EX(IsBackNavigable(), _T("CManager::NavigateBack()"), _T("Manager is not backnavigable!"));
	if (!IsBackNavigable())
		throw new CDesertException(_T("CManager::NavigateBack(): Manager is not backnavigable!"));


	//we actually step two step backwards
	//to effectively step back a configuration
	//(this is needed because when applying constraints, the constraints are applied in the current generation
	//and then also copied(with SetApplied()) to the newly generated generation)

	//obtain how many back steps there have been
	int curr_steps_back = GetCurrStepsBack();

	//actually step back
	int i = generations.GetPrev(currentGenerationPosition);

	if (!currentGenerationPosition)
		throw new CDesertException(_T("CManager::NavigateBack(): BackNavigation failed!"));

	//force the refresh of the enconding length and compute time on the UI
	ui_refresh_needed = true;

	////if it's not the first back-navigation, step back only one...
	//if (curr_steps_back)
	//{
	//	i = generations.GetPrev(currentGenerationPosition);
	//		return;
	//}

	//else, step back one more
	i = generations.GetPrev(currentGenerationPosition);


	//check
	//ASSERT_EX(currentGenerationPosition,_T("CManager::NavigateBack()"), _T("BackNavigation failed"));


};

void CManager :: NavigateForward()
{
	//check
	//ASSERT_EX(IsForwardNavigable(), _T("CManager::NavigateForward()"), _T("Manager is not forward-navigable!"));
	if (!IsForwardNavigable())
		throw new CDesertException(_T("CManager::NavigateForward(): Manager is not forward-navigable!"));

	int i = generations.GetNext(currentGenerationPosition);
	if(currentGenerationPosition)
		i = generations.GetNext(currentGenerationPosition);
	//force the refresh of the enconding length and compute time on the UI
	ui_refresh_needed = true;

	///check
	//ASSERT_EX(currentGenerationPosition,_T("CManager::NavigateForward()"), _T("ForwardNavigation failed"));
	if (!currentGenerationPosition)
		throw new CDesertException(_T("CManager::NavigateBack(): BackNavigation failed!"));

};

void CManager::GenerateNextHierarchy()
{
	int prev;

	if (generations.IsEmpty())
		prev = -1;
	else
	{
		//ASSERT_EX(currentGenerationPosition, _T("CManager::GenerateNextHierarchy"), _T("current generation is NULL"));
		if (!currentGenerationPosition)
			throw new CDesertException(_T("CManager::GenerateNextHierarchy(): current generation is NULL!"));
		prev = generations.GetAt(currentGenerationPosition) % MAX_GENERATIONS;
	}


	/*
		Remove any configurations behind the current one,
		if we are generating a configuration from a backnavigated one!
	*/

	POSITION pos;
	bool more_to_come = false;

	if(!generations.IsEmpty())
	do
	{
		pos = currentGenerationPosition;
		int i  = generations.GetNext(pos);
		if (pos)
		{
			generations.RemoveAt(pos);
			more_to_come = true;
		} else {
			more_to_come = false;
		}

	} while (more_to_come);



	static int generation_id = -1;
	if(generations.IsEmpty())
		generation_id = -1;

	generation_id++;
	currentGenerationPosition = generations.AddTail(generation_id);
	int currentGeneration = generation_id % MAX_GENERATIONS;



  CDynSpaceList& spaces = dynSpaces[currentGeneration];
  CDynDomainList& domains = dynDomains[currentGeneration];
  CDynElementContainerList& containers = dynContainers[currentGeneration];
  CDynConstraintSetList& consets = dynConstraintSets[currentGeneration];

  ::Destroy(domains);                   // MAX_GENERATIONS must be atleast 2
  ::Destroy(spaces);
  ::Destroy(consets);
  // FIXME this crashes when over MAX_GENERATIONS
  ::Destroy(containers);
  dynContainers[currentGeneration].RemoveAll();

  if (prev < 0)
  {
	{
		  POSITION pos = baseDomains.GetHeadPosition();
		  while(pos)
		  {
			CDynDomain *dmn = CDynDomain::Make(baseDomains.GetNext(pos));
			domains.AddTail(dmn);
			//there's no sense to express project().domain(), if that domain is not a customdomain.
			//anyway, it won't have any elements.
			if(dmn->GetType() == typeDynCustomDomain)
			{
				containers.AddHead((CDynCustomDomain*)dmn);
			}

		  }
		  pos = domains.GetHeadPosition();
		  while(pos) domains.GetNext(pos)->Clone();
    }
    {
		  POSITION pos = baseSpaces.GetHeadPosition();
		  while(pos)
		  {
			CDynSpace *spc = CDynSpace::Make(baseSpaces.GetNext(pos));
			spaces.AddTail(spc);
			containers.AddTail(spc);
		  }
		  pos = spaces.GetHeadPosition();
		  while(pos) ((CDynElementContainer*)spaces.GetNext(pos))->Clone();
    }
	{
		//For simpleFormula
		POSITION pos = simpleFormulas.GetHeadPosition();
		while(pos)
		{
			CSimpleFormula *sf = simpleFormulas.GetNext(pos);
			CPropertyList &dstlist = sf->GetDestinationList();
			CDynVariablePropertyList dyndstlist;
			POSITION pos1 = dstlist.GetHeadPosition();
			while(pos1)
			{
				CProperty *dstp = dstlist.GetNext(pos1);
				CElement *dstelem = dstp->GetOwner();
				CDynElement *dstDynElem = FindElement(dstelem);
				if(!dstDynElem) continue;
				CDynVariableProperty *dstDynProp = (CDynVariableProperty *)dstDynElem->FindProperty(dstp->GetName());
				dyndstlist.AddTail(dstDynProp);
			}
			CPropertyList &srclist = sf->GetSourceList();
			POSITION pos2 = srclist.GetHeadPosition();
			while(pos2)
			{
				CProperty *srcp = srclist.GetNext(pos2);
				CElement *srcelem = srcp->GetOwner();
				CDynElement *srcDynElem = FindElement(srcelem);
				if(!srcDynElem) continue;
				CDynProperty *srcDynProp = (CDynProperty *)srcDynElem->FindProperty(srcp->GetName());
				POSITION pos3 = dyndstlist.GetHeadPosition();
				while(pos3)
				{
					dyndstlist.GetNext(pos3)->InsertProperty(srcDynProp);
				}
			}
		}
	}
    {
		  POSITION pos = baseConstraintSets.GetHeadPosition();
		  while(pos)
		  {
			CDynConstraintSet *set = CDynConstraintSet::Make(baseConstraintSets.GetNext(pos));
			consets.AddTail(set);
		  }
		  pos = consets.GetHeadPosition();
		  while(pos) consets.GetNext(pos)->Clone();
    }
  }
  else
  {
    CDynSpaceList& prevSpaces = dynSpaces[prev];
    CDynDomainList& prevDomains = dynDomains[prev];
    CDynConstraintSetList& prevConsets = dynConstraintSets[prev];
	CDynElementContainerList &prevContainers = dynContainers[prev];


    {
      POSITION pos = prevDomains.GetHeadPosition();
      while(pos)
      {
        CDynDomain *frm = prevDomains.GetNext(pos);
        CDynDomain *dmn = CDynDomain::Make(frm);

        if (dmn)
        {
			domains.AddTail(dmn);
			if(dmn->GetType() == typeDynCustomDomain)
			{
				containers.AddTail((CDynCustomDomain*)dmn);
			}
			dmn->Clone(frm);
        }
      }

    }
    {
      POSITION pos = prevSpaces.GetHeadPosition();
      while(pos)
      {
        CDynSpace *frm = prevSpaces.GetNext(pos);
        CDynSpace *spc = CDynSpace::Make(frm);
        if (spc)
        {
          spaces.AddTail(spc);
		  containers.AddTail(spc);
          spc->Clone(frm);
        }
      }
    }
    {
      POSITION pos = prevConsets.GetHeadPosition();
      while(pos)
      {
        CDynConstraintSet *frm = prevConsets.GetNext(pos);
        CDynConstraintSet *set = CDynConstraintSet::Make(frm);
        if (set)
        {
          consets.AddTail(set);
          set->Clone(frm);
        }
      }
    }
	{
		//For simpleFormula
		POSITION pos = simpleFormulas.GetHeadPosition();
		while(pos)
		{
			CSimpleFormula *sf = simpleFormulas.GetNext(pos);
			CPropertyList &dstlist = sf->GetDestinationList();
			CDynVariablePropertyList dyndstlist;
			POSITION pos1 = dstlist.GetHeadPosition();
			while(pos1)
			{
				CProperty *dstp = dstlist.GetNext(pos1);
				CElement *dstelem = dstp->GetOwner();
				CDynElement *dstDynElem = FindElement(dstelem);
				if(!dstDynElem) continue;
				CDynVariableProperty *dstDynProp = (CDynVariableProperty *)dstDynElem->FindProperty(dstp->GetName());
				dyndstlist.AddTail(dstDynProp);
			}
			CPropertyList &srclist = sf->GetSourceList();
			POSITION pos2 = srclist.GetHeadPosition();
			while(pos2)
			{
				CProperty *srcp = srclist.GetNext(pos2);
				CElement *srcelem = srcp->GetOwner();
				CDynElement *srcDynElem = FindElement(srcelem);
				if(!srcDynElem) continue;
				CDynProperty *srcDynProp = (CDynProperty *)srcDynElem->FindProperty(srcp->GetName());
				POSITION pos3 = dyndstlist.GetHeadPosition();
				while(pos3)
				{
					dyndstlist.GetNext(pos3)->InsertProperty(srcDynProp);
				}
			}
		}
	}
  }
}

void CManager::Dump(FILE *f, const TCHAR * comment)
{
	if (!f) return;
	GET_CURRENTGENERATION

	CDynSpaceList& spaces = dynSpaces[currentGeneration];
	CDynDomainList& domains = dynDomains[currentGeneration];
	CDynElementContainerList& containers = dynContainers[currentGeneration];
	CDynConstraintSetList& consets = dynConstraintSets[currentGeneration];

	{
		//debug
		//sorry for the var names -- I was kind-of angry
		POSITION pos;

		if (f) _ftprintf(f, _T("---->>begin of curr gen at %s<<----------------------------------------------------\n"), comment);

		pos = domains.GetHeadPosition();
		while(pos)
		{
			CDynDomain *fuck_domain = domains.GetNext(pos);
			if(f)_ftprintf(f, _T(" Domain element: %s at address: %p \n"), static_cast<const TCHAR*>(fuck_domain->GetName()), fuck_domain);
			fuck_domain->Dump(f);
		}

		pos = spaces.GetHeadPosition();
		while(pos)
		{
			CDynSpace *fuck_space = spaces.GetNext(pos);
			if(f)_ftprintf(f, _T(" Space element: %s at address: %p \n"), static_cast<const TCHAR*>(fuck_space->GetName()), fuck_space);
			fuck_space->Dump(f);
		}

		pos = containers.GetHeadPosition();
		while(pos)
		{
			CDynElementContainer *fuck_ec = containers.GetNext(pos);
			_ftprintf(f, _T(" Container element: at address: %p \n"), fuck_ec);
			fuck_ec->Dump(f);
		}

		_ftprintf(f, _T("===========<<end of curr========================================================\n"));


	}

}

CDynConstraintSet* CManager::GetConstraintSet(ConstraintType t)
{
  if (!generations.IsEmpty())
  {
	  GET_CURRENTGENERATION

    CDynConstraintSetList& consets = dynConstraintSets[currentGeneration];
    POSITION pos = consets.GetHeadPosition();
    while(pos)
    {
      CDynConstraintSet *set = consets.GetNext(pos);
      if (_tcscmp(setNames[t], (const TCHAR *)*set) == 0)
        return set;
    }
  }
  return 0;
}

bool CManager::Prune(CDynConstraintSet* set)
{
  	if (!set || set->IsEmpty() || generations.IsEmpty()) return false;


	GET_CURRENTGENERATION

  // find the root of the (sub)space/s affected by the constraints in the set
	bool propertyDependent = false;
	CDynSpaceList spaces;
	CDynElement *root = set->FindAffectedSpace(spaces, propertyDependent);

	return root ?
		(propertyDependent ? Prune(set, root, propertyDependent) : Prune(set, root)) :
		(propertyDependent ? Prune(set, spaces, propertyDependent) : Prune(set, spaces));

}

bool CManager::Prune(CDynConstraintSet *set, CDynElement *root)
{
  // compute encoding length
  root->SetEncodingLen(0);
  int encodingLength = root->FindEncodingLen();

  // init bdd library
  CBdd::Init(encodingLength, 0);

  // now encode the space/subspace
  //
  // a) assign an encoding value to each element in the space
  //    encoding value is basically a string of bits, which represents the binary encoding
  //    of the element. don't cares in this string are represented by -1
  // b) translate the encoding bit string to a bdd, and combine all the bdds (conjunction/disjunction) to form a
  //    single bdd, that encodes the entire space
  //

  root->SetEncodingValue(0, 0);		// this sets the encoding value (fills bit strings)
  CBdd config = root->MaptoBdd(CBdd::One(), 0); // this tranlates the bit strings to bdd's and combines the bdds of all elements in the space
  CBdd cons = set->MaptoBdd();        // translate the constraints to bdd

  // conjunct the configbdd & constraintbdd. bingo!! there is your pruned (bdd) space
  CBdd res = config && cons;

  // now restrict (actually prune the space), remove the dead alternatives
  bool r1 = root->Restrict(res);

  // the constraints whose dependency is entirely pruned, mark those as dead
  bool r2 = set->Restrict();

  // terminate bdd library
  CBdd::Finit();

  return r1 || r2;
}

bool CManager::Prune(CDynConstraintSet *set, CDynSpaceList& spaces)
{
  int encodingLength = 0;
  // compute encoding length
  POSITION pos = spaces.GetHeadPosition();
  while(pos)
  {
    CDynSpace *space = spaces.GetNext(pos);
    space->SetEncodingLen(0);
    encodingLength += space->FindEncodingLen();
  }

  // init bdd library
  CBdd::Init(encodingLength, 0);
  CBdd one = CBdd::One();
  CBdd config = one;

  // now encode the space/subspace
  //
  // a) assign an encoding value to each element in the space
  //    encoding value is basically a string of bits, which represents the binary encoding
  //    of the element. don't cares in this string are represented by -1
  // b) translate the encoding bit string to a bdd, and combine all the bdds (conjunction/disjunction) to form a
  //    single bdd, that encodes the entire space
  //

  int sb=0, psv=0;
  pos = spaces.GetHeadPosition();
  while(pos)
  {
    CDynSpace *space = spaces.GetNext(pos);
    sb += space->SetEncodingValue(0, sb);
    config = space->MaptoBdd(one, 0) && config;
  }
  CBdd cons = set->MaptoBdd();        // translate the constraints to bdd

  // conjunct the configbdd & constraintbdd. bingo!! there is your pruned (bdd) space
  CBdd res = config && cons;

  // now restrict (actually prune the space), remove the dead alternatives
  bool r1 = false;
  pos = spaces.GetHeadPosition();
  while(pos) r1 = spaces.GetNext(pos)->Restrict(res) || r1;

  // the constraints whose dependency is entirely pruned, mark those as dead
  bool r2 = set->Restrict();

  // terminate bdd library
  CBdd::Finit();

  return r1 || r2;
}

bool CManager::Prune(CDynConstraintSet *set, CDynElement *root, bool)
{
#ifdef ENCODE_PROPERTY
  // this is property dependent -- treat accordingly
  int vectors = root->FindNonOrthElementCount();
  int propEncodingLength = root->FindPropertyEncodingLen(_T("latency"));
  // the largest sum could be vectors*(2^propEncodingLen-1), so increment propEncodingLen
  // to hold that many more bits
  propEncodingLength += log2(vectors);
  propertyVectors = vectors;
  propertyEncodingLength = propEncodingLength; // store as class variables to be used later

  // compute encoding length
  root->SetEncodingLen(0);
  int encodingLength = root->FindEncodingLen();


  // init bdd library
  int start = encodingLength;
  encodingLength += propEncodingLength*(vectors+3);
  CBdd::Init(encodingLength, 1, _T("latency"), start, propEncodingLength, vectors+3);

  root->SetEncodingValue(0, 0);		// this sets the encoding value (fills bit strings)
  root->SetPropertyEncodingValue(_T("latency"), 0); // this is setting the vector number of the property

  CBdd config = root->MaptoBdd(CBdd::One(), 1, _T("latency")); // this tranlates the bit strings to bdd's and combines the bdds of all elements in the space alongwith the properties
  int size = config.size();

  CBdd cons = set->MaptoBdd();        // translate the constraints to bdd

  // conjunct the configbdd & constraintbdd. bingo!! there is your pruned (bdd) space
  CBdd res = config && cons;


  for (int i=vectors+2; i>=0; i--)
    res = CBdd::QuantifyPropertyExistentially(_T("latency"), res, i);
#else

  int vectors = root->FindNonOrthElementCount();
  int propEncodingLength = root->FindPropertyEncodingLen(_T("latency"));
  // the largest sum could be vectors*(2^propEncodingLen-1), so increment propEncodingLen
  // to hold that many more bits
  propEncodingLength += log2(vectors);
  propertyVectors = vectors;
  propertyEncodingLength = propEncodingLength; // store as class variables to be used later


  // compute encoding length
  root->SetEncodingLen(0);
  int encodingLength = root->FindEncodingLen();

  // init bdd library
  int start = encodingLength;
  CBdd::Init(encodingLength, 0);

  root->SetEncodingValue(0, 0);		// this sets the encoding value (fills bit strings)

  // this tranlates the bit strings to bdd's and combines the bdds of all elements in the space
  CBdd config = root->MaptoBdd(CBdd::One(), 0);
  int size = config.size();

  CBdd cons = set->MaptoBdd();

  CBdd res = config && cons;
#endif
  // now restrict (actually prune the space), remove the dead alternatives
  bool r1 = root->Restrict(res);

  // the constraints whose dependency is entirely pruned, mark those as dead
  bool r2 = set->Restrict();

  // terminate bdd library
  CBdd::Finit();

  return r1 || r2;
}

bool CManager::Prune(CDynConstraintSet *set, CDynSpaceList& spaces, bool)
{
  return false;
}

void * CManager::StoreConfigurations(const TCHAR *fname, tstring &errmsg)
{
//  if (currentGeneration < 0) GenerateNextHierarchy();
	if (generations.IsEmpty()) GenerateNextHierarchy();

	double spaceSize = ComputeSize();
	if (spaceSize > MAX_CONFIGURATIONS)
	if(designSpaceSize > MAX_CONFIGURATIONS)
	{
		TCHAR buffer[65];
		_i64tot(spaceSize, buffer, 10);
		errmsg = _T("Space is extremely large <")+(tstring)buffer+_T(">.\nPrune the space further before storing configurations");
		Error(_T("Process "), _T("Space is extremely large <%2.2e>.\nPrune the space further before storing configurations"),spaceSize);
		return NULL;
  }

    GET_CURRENTGENERATION

  CDynSpaceList& spaces = dynSpaces[currentGeneration];
  CDynConstraintSetList& consets = dynConstraintSets[currentGeneration];
  int encodingLength = 0;

  // compute encoding length
  POSITION pos = spaces.GetHeadPosition();
  while(pos)
  {
    CDynSpace *space = spaces.GetNext(pos);
    space->SetEncodingLen(0);
    encodingLength += space->FindEncodingLen();
  }

  // init bdd library
  int start = encodingLength;

  CBdd::Init(encodingLength, 0);

  CBdd one = CBdd::One();
  CBdd config = one;

  // encode the space
  int sb=0, psv=0, vec=0;
  pos = spaces.GetHeadPosition();
  while(pos)
  {
    CDynSpace *space = spaces.GetNext(pos);
    sb += space->SetEncodingValue(0, sb);
    config = space->MaptoBdd(one, 0) && config;
  }

  CDynConstraintSet *toSet = new CDynConstraintSet(0);
  toSet->RemoveAll();

  pos = consets.GetHeadPosition();
  while(pos)
  {
    CDynConstraintSet *set = consets.GetNext(pos);
    CDynConstraintList& cons = set->GetConstraints();
    POSITION consPos = cons.GetHeadPosition();


    while(consPos)
    {
      CDynConstraint *c = cons.GetNext(consPos);
      if (c->WasApplied())
		  toSet->InsertConstraint(c);
    }
  }

  BackIfaceFunctions::DBConfigurations * configurations = NULL;
  try{
	  CBdd cons = toSet->MaptoBdd();
	  CBdd prun = config && cons;

	  // now build the configurations wrt the prun bdd
	  {

		// get the root elements of the processSpace

		// we create a configuration object hierarchy that is used to serialize all configurations
		// create a configuration store
		//CConfigurationStore *cs = new CConfigurationStore(fname);
		//ASSERT_EX(cs, _T("CManager::StoreConfigurations"), _T("new failed"));

		POSITION mPos = spaces.GetHeadPosition();

		while (mPos)
		{
			CDynSpace *procSpace = spaces.GetNext(mPos);
			CDynElementList& roots = procSpace->GetRootElements();

			// build configuration for each root element
			POSITION pos = roots.GetHeadPosition();
			while(pos)
			{
			  CDynElement *root = roots.GetNext(pos);
			  // create a configuration root object in the configurationstore
			  //CConfigurationRoot *cfgRoot = cs->CreateConfigurationRoot(root->GetExternal());

			  //this list of structs will be returned
			  configurations = new BackIfaceFunctions::DBConfigurations;


			  BuildConfigurations(configurations, root, prun, encodingLength);


			}//eo while(pos)
		}//eo while (mPos)
		// store all the built configurations
		//cs->Store();
		//delete cs;
	  }
  }catch(CDesertException *e)
  {
	  CBdd::Finit();
	  toSet->RemoveAll();
	  delete toSet;
	  throw e;
  }
  // terminate bdd library
  CBdd::Finit();

  toSet->RemoveAll();
  delete toSet;

  return configurations;
}

// Himanshu: Adding a function to calculate real no. of configurations
long CManager::CalcRealNoOfConfigurations()
{
	if (generations.IsEmpty()) GenerateNextHierarchy();

	double sizeOfDesignSpace = ComputeSize();
	if (sizeOfDesignSpace > MAX_CONFIGURATIONS) {
		// TODO: If design space is too large, check the prun.sat_frac()
		// at the end of this function
		return DESIGN_SPACE_TOO_LARGE;
	}

	GET_CURRENTGENERATION

	// get design space and constraints corresponding to the current generation
	CDynSpaceList& spaces = dynSpaces[currentGeneration];
	CDynConstraintSetList& consets = dynConstraintSets[currentGeneration];
	int encodingLength = 0;

	// compute encoding length
	POSITION pos = spaces.GetHeadPosition();
	while(pos)
	{
		CDynSpace *space = spaces.GetNext(pos);
		space->SetEncodingLen(0);
		encodingLength += space->FindEncodingLen();
	}

	// init bdd library
	int start = encodingLength;

	CBdd::Init(encodingLength, 0);

	CBdd one = CBdd::One();
	CBdd config = one;

	// encode the space
	int sb=0, psv=0, vec=0;
	pos = spaces.GetHeadPosition();
	while(pos)
	{
		CDynSpace *space = spaces.GetNext(pos);
		sb += space->SetEncodingValue(0, sb);
		config = space->MaptoBdd(one, 0) && config;
	}

	CDynConstraintSet *toSet = new CDynConstraintSet(0);
	toSet->RemoveAll();

	pos = consets.GetHeadPosition();
	while(pos)
	{
		CDynConstraintSet *set = consets.GetNext(pos);
		CDynConstraintList& cons = set->GetConstraints();
		POSITION consPos = cons.GetHeadPosition();

		while(consPos)
		{
			CDynConstraint *c = cons.GetNext(consPos);
			if (c->WasApplied())
				toSet->InsertConstraint(c);
		}
	}

	long totalNoOfConfigs = 0;
	try{
		CBdd cons = toSet->MaptoBdd();
		CBdd prun = config && cons;

		// now calculate how many configurations are there (wrt the prun bdd)
		// get the root elements of the processSpace
		POSITION mPos = spaces.GetHeadPosition();
		while (mPos)
		{
			CDynSpace *procSpace = spaces.GetNext(mPos);
			CDynElementList& roots = procSpace->GetRootElements();

			// get no. of configurations for each root element
			POSITION pos = roots.GetHeadPosition();
			long numCfgsInEachRoot = 0;
			while(pos)
			{
				CDynElement *root = roots.GetNext(pos);
				CPtrList encVectors;
				CBdd::Satisfy(prun, encVectors);

				// Get rid of double-counting
				POSITION pos = encVectors.GetHeadPosition();
				while(pos)
				{
					int *encVec = (int *)encVectors.GetNext(pos);

					// convert the value to bdd, to write configuration
					CBdd enc = CBdd::Encode(encVec, 0, encodingLength);
					if ( root->NotRedundant(encVec) ) {
						numCfgsInEachRoot++;
					}

					delete[] encVec;
				}

			} //eo while(pos)

			totalNoOfConfigs += numCfgsInEachRoot;

		} //eo while (mPos)

	}catch(CDesertException *e)
	  {
		CBdd::Finit();
		toSet->RemoveAll();
		delete toSet;
		  throw e;
	  }

	// terminate bdd library
	CBdd::Finit();

	toSet->RemoveAll();
	delete toSet;

	return totalNoOfConfigs;
}

//inline long CManager::GetConfNumber(){static long unique = 1; return unique++;};
inline long CManager::GetConfNumber(){return unique++;};

void CManager:: BuildConfigurations(BackIfaceFunctions::DBConfigurations *configs , CDynElement *root, CBdd& config, int encLen)
{
  CPtrList encVectors;
  int count = CBdd::Satisfy(config, encVectors);
  POSITION pos = encVectors.GetHeadPosition();

  while(pos)
  {
    int *encVec = (int *)encVectors.GetNext(pos);

//	int j = 0;
//	FILE * debug = fopen(_T("C:\\tmp\\bdd_debug.txt"), _T("a"));
//	_ftprintf(debug, _T("\nencVec: %d; "), j++);

//	int i =0;
//	while (i<encLen)
//		if (debug) _ftprintf(debug, _T(" %d,"), encVec[i++]);
//	fclose(debug);


    // convert the value to bdd, to write configuration
    CBdd enc = CBdd::Encode(encVec, 0, encLen);
    if ( root->NotRedundant(encVec) )
    {

		BackIfaceFunctions::DBConfiguration *conf = new BackIfaceFunctions::DBConfiguration;
		conf->id = theInstance->GetConfNumber();
		configs->AddTail(conf);
		root->BuildConfiguration(conf,  enc);
    }

    delete[] encVec;
  }
  encVectors.RemoveAll();
 }

bool CManager::HasConstraints()
{

	GET_CURRENTGENERATION

  CDynConstraintSetList& sets = dynConstraintSets[currentGeneration];

  bool empty = true;
  POSITION pos = sets.GetHeadPosition();
  while(pos) empty = sets.GetNext(pos)->IsEmpty() && empty;

  return !empty;
}

bool CManager::HasImplicitConstraints()
{
	GET_CURRENTGENERATION
	CDynConstraintSetList& sets = dynConstraintSets[currentGeneration];
	POSITION pos = sets.GetHeadPosition();
	bool has_ic = false;
	while(pos && !has_ic)
	{
		CDynConstraintSet * set = sets.GetNext(pos);
		CDynConstraintList & cons_list = set->GetConstraints();
		POSITION pos1 = cons_list.GetHeadPosition();
		while(pos1 && !has_ic)
		{
			CDynConstraint * cons = cons_list.GetNext(pos1);
			has_ic = ((CConstraint*)cons->GetCore())->IsImplicit() && !cons->WasApplied();
		}
	};

	return has_ic;
};

ClData CManager::Eval(const CCosmic *other) const
{
  // check function type??
  ClData ret = true;
  if (other)
  {
    switch(other->GetType())
    {
    case typeDynElement:
      ret = ((CDynElement *)other)->GetEncoding();
      break;

    default:
      Todo(_T("CDynElement::Eval"), _T("Eval for unimplemented type %d"), (int)(other->GetType()));
    }
  }
  else
  {
    ret = CBdd::One();
  }
  return ret;
}


bool CManager::Profile(double& dspSize, long& repSize, long& encSize)
{

	  GET_CURRENTGENERATION

  CDynSpaceList& spaces = dynSpaces[currentGeneration];
  int encodingLength = 0;

  // compute encoding length
  POSITION pos = spaces.GetHeadPosition();
  while(pos)
  {
    CDynSpace *space = spaces.GetNext(pos);
    space->SetEncodingLen(0);
    encodingLength += space->FindEncodingLen();
  }

  // init bdd library
  CBdd::Init(encodingLength, 0);
  CBdd one = CBdd::One();
  CBdd config = one;

  int sb=0, psv=0;
  pos = spaces.GetHeadPosition();
  while(pos)
  {
    CDynSpace *space = spaces.GetNext(pos);
    sb += space->SetEncodingValue(0, sb);
    config = space->MaptoBdd(one, 0) && config;
  }

  dspSize = ComputeSize();
  repSize = config.size();
  encSize = encodingLength;

  // terminate bdd library
  CBdd::Finit();

  return true;
}

//extern int maxLatBddSz;

bool CManager::Profile(double& dspSize, long& repSize, long& encSize, long& consSize, long& prRepSize, double& prDegree)
{
  GET_CURRENTGENERATION

  CDynSpaceList& spaces = dynSpaces[currentGeneration];
  CDynConstraintSetList& consets = dynConstraintSets[currentGeneration];
  int encodingLength = 0;

  // compute encoding length
  POSITION pos = spaces.GetHeadPosition();
  while(pos)
  {
    CDynSpace *space = spaces.GetNext(pos);
    space->SetEncodingLen(0);
    encodingLength += space->FindEncodingLen();
  }

  int vectors = 0;
  propertyEncodingLength = 0;
  pos = spaces.GetHeadPosition();
  while(pos)
  {
    CDynSpace *space = spaces.GetNext(pos);
    vectors += space->FindNonOrthElementCount();
	propertyEncodingLength += space->FindPropertyEncodingLen(_T("latency"));
  }
  propertyEncodingLength += log2(vectors);
  propertyVectors = vectors;

  // init bdd library
  int start = encodingLength;
  CBdd::Init(encodingLength, 0);

  CBdd one = CBdd::One();
  CBdd config = one;

  // encode the space
  int sb=0, psv=0, vec=0;
  pos = spaces.GetHeadPosition();
  while(pos)
  {
    CDynSpace *space = spaces.GetNext(pos);
    sb += space->SetEncodingValue(0, sb);
    config = space->MaptoBdd(one, 0) && config;
  }

  int size = config.size();

  CBdd cons = CBdd::One();
  pos = consets.GetHeadPosition();
  while(pos)
  {
    CDynConstraintSet *set = consets.GetNext(pos);
    cons = cons && set->MaptoBdd();
  }

  CBdd prun = config && cons;


  // set the propertyEncodingLength, so that checklatencylimits succeeds

  // encode the constraints


  dspSize = ComputeSize();
  repSize = config.size();
  encSize = encodingLength;
//  consSize = maxLatBddSz;               // cons.size();
  prRepSize = vectors;                  // prun.size();

  prDegree = prun.sat_fract()/config.sat_fract();

  // terminate bdd library
  CBdd::Finit();

  return true;
}

_int64 CManager:: GetDSSize()
{
	return designSpaceSize;
}

bool CManager::GetSizeInfo(double& dspSize, long& repSize, long& clockTime, CDynConstraintSet* toSet)
{
  TRACE( _T("CManager::GetSizeInfo \n") );

  GET_CURRENTGENERATION

  CDynSpaceList& spaces = dynSpaces[currentGeneration];
  CDynConstraintSetList& consets = dynConstraintSets[currentGeneration];
  int encodingLength = 0;

  // compute encoding length
  POSITION pos = spaces.GetHeadPosition();
  while(pos)
  {
    CDynSpace *space = spaces.GetNext(pos);
    space->SetEncodingLen(0);
    encodingLength += space->FindEncodingLen();
  }

  TRACE( _T("CManager::GetSizeInfo : encoding length = %d\n"), encodingLength );

  int vectors = 0;
  propertyEncodingLength = 0;
  pos = spaces.GetHeadPosition();

  // init bdd library
  int start = encodingLength;
  CBdd::Init(encodingLength, 0);

  CBdd one = CBdd::One();
  CBdd config = one;

  clockTime = 0;

  clock_t t1,t2;
  t1 = clock();
  // encode the space
  int sb=0, psv=0, vec=0;
  pos = spaces.GetHeadPosition();
  while(pos)
  {
    CDynSpace *space = spaces.GetNext(pos);
     sb += space->SetEncodingValue(0, sb);

    config = space->MaptoBdd(one, 0) && config;

  }
  t2 = clock();

  clockTime += (t2-t1);

  int size = config.size();

  bool doRestrict = (toSet != NULL);


  if (!doRestrict)
  {
    toSet = new CDynConstraintSet(0);
    toSet->RemoveAll();
  }

  pos = consets.GetHeadPosition();


  while(pos)
  {
    CDynConstraintSet *set = consets.GetNext(pos);
    CDynConstraintList& cons = set->GetConstraints();
    POSITION consPos = cons.GetHeadPosition();

    while(consPos)
    {
      CDynConstraint *c = cons.GetNext(consPos);
	  if (c->WasApplied())
		  toSet->InsertConstraint(c);
    }
  }

  t1 = clock();
  try{
	  CBdd cons = toSet->MaptoBdd();
	  CBdd prun = config && cons;


	  t2 = clock();

	  clockTime += (t2-t1);


	  // set the propertyEncodingLength, so that checklatencylimits succeeds

	  // encode the constraints
	  void (*old_handler)(int) = signal( SIGFPE, fp_handler );
	  dspSize = ldexp(prun.sat_fract(),encodingLength);

	  designSpaceSize = dspSize;

	  signal( SIGFPE, old_handler );
	  if (fp_except)
	  {
		  dspSize = HUGE_VAL;
		  fp_except = false;
	  }
	  repSize = encodingLength; // prun.size();
	  clockTime = (clockTime/(double)CLOCKS_PER_SEC)*1000;


	  // now restrict (actually prune the space), remove the dead alternatives

	  bool r1 = false;
	  pos = spaces.GetHeadPosition();
	  while(pos) r1 = spaces.GetNext(pos)->Restrict(prun) || r1;



	  // the constraints whose dependency is entirely pruned, mark those as dead
	  bool r2 = toSet->Restrict();
  }
  catch(CDesertException *e)
  {
		CBdd::Finit();

	  if (!doRestrict)
	  {
		toSet->RemoveAll();
		delete toSet;
	  }
	  throw e;
  }

  // terminate bdd library
  CBdd::Finit();

  if (!doRestrict)
  {
    toSet->RemoveAll();
    delete toSet;
  }

  return true;

}


// Implementation of manager class inline functions

inline CSpace * CManager::FindSpace(long i)
{
	CSpace *s=0;
	return Find(baseSpaces, s, i) ? s : 0;
}

inline CDomain * CManager::FindDomain(long i)
{
	CDomain *s=0;
	return Find(baseDomains, s, i) ? s : 0;
}

inline CConstraintSet * CManager::FindConstraintSet(long i)
{
	CConstraintSet *s=0;
	return Find(baseConstraintSets, s, i) ? s : 0;
}

inline CElement * CManager::FindElement(long i)
{
	CElement *el = FindElement(baseSpaces, i);
	return  el ? el : FindElement(baseDomains, i);
}

inline CElement * CManager::FindElement(CSpaceList& s, long i)
{
	return ::FindElement(s, i);
}

inline CElement * CManager::FindElement(CDomainList& d, long i)
{
	return ::FindElement(d, i);
}

inline CConstraint * CManager::FindConstraint(long i)
{
	return ::FindConstraint(baseConstraintSets, i);
}

inline CDynSpace * CManager:: FindSpace(const CSpace *c)
{

	GET_CURRENTGENERATION

	CDynSpace *s;
	return Find( dynSpaces[currentGeneration], s, (const CStaticObj *)c) ? s : 0;
}

inline CDynSpace * CManager::FindSpace(CString& n)
{
	GET_CURRENTGENERATION
	CDynSpace *s;
	return Find( dynSpaces[currentGeneration], s, (const TCHAR *)n) ? s : 0;
}

inline CDynDomain * CManager::FindDomain(const CDomain *c)
{
	GET_CURRENTGENERATION
	CDynDomain *d;
	return Find( dynDomains[currentGeneration], d, (const CStaticObj *)c) ? d : 0;
}

inline CDynDomain * CManager::FindDomain(CString& n)
{
	GET_CURRENTGENERATION
	CDynDomain *d;
	return Find( dynDomains[currentGeneration], d, (const TCHAR *)n) ? d : 0;
}

inline CDynConstraintSet * CManager::FindConstraintSet(const CConstraintSet *c)
{
	GET_CURRENTGENERATION
	CDynConstraintSet *s;
	return Find( dynConstraintSets[currentGeneration], s, (const CStaticObj *)c) ? s : 0;
}

inline CDynElement * CManager::FindElement(const CElement *c)
{
	GET_CURRENTGENERATION
	CDynElement *el = FindElement( dynSpaces[currentGeneration], c );
	return el ? el : FindElement( dynDomains[currentGeneration], c );
}

inline CDynElement * CManager::FindElement(CDynSpaceList& s, const CElement *c)
{
	return ::FindElement(s, c);
}

inline CDynElement * CManager::FindElement(CDynDomainList& d, const CElement *c)
{
	return ::FindElement(d, c);
}

inline CDynConstraint * CManager::FindConstraint(const CConstraint *c)
{
	GET_CURRENTGENERATION
	return ::FindConstraint(dynConstraintSets[currentGeneration], c);
}

inline CDynConstraintSetList& CManager::GetConstraintSets()
{
	GET_CURRENTGENERATION
	return dynConstraintSets[currentGeneration];
}

inline CDynSpaceList& CManager::GetSpaces()
{
	GET_CURRENTGENERATION
	return dynSpaces[currentGeneration];
}

inline CDynDomainList& CManager::GetDomains()
{
	GET_CURRENTGENERATION
	return dynDomains[currentGeneration];
}

inline void CManager::SetFunction(FunctionType f)
{
  funcType = f;
}

