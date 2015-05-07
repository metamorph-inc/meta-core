
#include "stdafx.h"
#include "desert.h"
#include "CSetErrDialog.h"

#include "core/templ.h"



template<class T>
void 
__DestroyEx(CTypedPtrList<CPtrList, T>& l)
{
  POSITION pos = l.GetHeadPosition();
  while(pos) delete l.GetNext(pos);
  l.RemoveAll();
}
/*
template<class T>
bool
Find(CTypedPtrList<CPtrList, T>& l, T& t, long i)
/*
 * the members of the list must have an operator (long)
 * all classes
 */
/*
{
  POSITION pos = l.GetHeadPosition();
  while(pos)
  {
    t = l.GetNext(pos);
    if ((long)*t == i) return true;
  }
  t = 0;
  return false;
}
*/
template<class T>
bool
__FindEx(CTypedPtrList<CPtrList, T>& l, T& t, long i)
/*
 * the members of the list must have an operator (long)
 * all classes
 */
{
  POSITION pos = l.GetHeadPosition();
  while(pos)
  {
    t = l.GetNext(pos);
    if ((long)*t == i) return true;
  }
  t = 0;
  return false;
}


template<class T> bool 
__FindEx(CTypedPtrList<CPtrList, T>& l, T& t, const TCHAR *n)
/*
 * the members of the list must have an operator (const TCHAR *)
 * all classes
 */
{
  POSITION pos = l.GetHeadPosition();
  while(pos)
  {
    t = l.GetNext(pos);
    if (_tcscmp(n, (const TCHAR *)*t) == 0) return true;
  }
  t = 0;
  return false;
}


template<class T>
bool
__FindEx(CTypedPtrList<CPtrList, T>& l, T& t, const CStaticObj *c)
/*
 * the members of the list must have a GetCore function
 * all classes inherited from CDynamicObj
 */
{
  POSITION pos = l.GetHeadPosition();
  while(pos)
  {
    t = l.GetNext(pos);
    if ( c == t->GetCore() ) return true;
  }
  t = 0;
  return false;
}


template<class T>
CElement *
__FindElementEx(CTypedPtrList<CPtrList, T>& l, long i)
/*
 * the members of the list must have a FindElement function
 * CSpace, CCustomDomain,
 */
{
  POSITION pos = l.GetHeadPosition();
  while(pos)
  {
    CElement *ret = l.GetNext(pos)->FindElement(i);
    if (ret) return ret;
  }
  return 0;
}

template<class T>
CConstraint *
__FindConstraintEx(CTypedPtrList<CPtrList, T>& l, long i)
/*
 * the members of the list must have a FindConstraint function
 * CConstraint
 */
{
  POSITION pos = l.GetHeadPosition();
  while(pos)
  {
    CConstraint *ret = l.GetNext(pos)->FindConstraint(i);
    if (ret) return ret;
  }
  return 0;
}

template<class T>
CDynElement *
__FindElementEx(CTypedPtrList<CPtrList, T>& l, const CElement *c)
/*
 * the members of the list must have a FindElement function
 * that takes a const CElement* as arg
 * CDynSpace, CDynCustomDomain
 */
{
  POSITION pos = l.GetHeadPosition();
  while(pos)
  {
    CDynElement *ret = l.GetNext(pos)->FindElement(c);
    if (ret) return ret;
  }
  return 0;
}

template<class T>
CDynConstraint *
__FindConstraintEx(CTypedPtrList<CPtrList, T>& l, const CConstraint *c)
/*
 * the members of the list must have a FindConstraint function
 * that takes a const CElement* as arg
 * CDynConstraintSet
 */
{
  POSITION pos = l.GetHeadPosition();
  while(pos)
  {
    CDynConstraint *ret = l.GetNext(pos)->FindConstraint(c);
    if (ret) return ret;
  }
  return 0;
}

template<class T>
bool
__RemoveEx(CTypedPtrList<CPtrList, T>& l, T t)
{
  POSITION pos = l.Find(t);
  if (pos)
  {
    l.RemoveAt(pos);
    return true;
  }
  Warning( _T("::Remove"), _T("Object <%s> not in list"), (const TCHAR *)(*t));
  return false;
}

template<class T>
bool
__RemoveEx(CTypedPtrList<CPtrList, T>& l, T& t, long i)
{
  return __FindEx(l, t, i) ? __RemoveEx(l, t) : false;
}

template<class T>
bool
__RemoveEx(CTypedPtrList<CPtrList, T>& l, T& t, const TCHAR *n)
{
  return __FindEx(l, t, n) ? __RemoveEx(l, t) : false;
}

template<class T>
bool
__InsertEx(CTypedPtrList<CPtrList, T>& l, T t)
{
  if (l.Find(t) == 0)
  {
    l.AddTail(t);
    return true;
  }
  Warning( _T("::Insert"), _T("Object <%s> already in list"),(const TCHAR *)(*t));
  return false;
}

template<class T>
int
__IndexEx(CTypedPtrList<CPtrList, T>& l, T t)
{
  int ret = 0;
  POSITION pos = l.GetHeadPosition();
  while(pos)
  {
    if (t == l.GetNext(pos)) return ret;
    ret++;
  }
  return -1;
}

template<class T>
CCosmicList *
__CloneListEx(CTypedPtrList<CPtrList, T>& l)
{
  CCosmicList *ret = new CCosmicList;
  POSITION pos = l.GetHeadPosition();
  while(pos) ret->AddTail(l.GetNext(pos));
  return ret;
}


template<class T>
T
__FindRootEx(CTypedPtrList<CPtrList, T>& l)
{
  T obj = l.IsEmpty() ? 0 : l.GetHead();
  while(obj && !__FindRootEx(obj, l)) obj = (T)(obj->Parent());
  return obj;
}

template<class T>
bool
__FindRootEx(T a, CTypedPtrList<CPtrList, T>& l)
{
  bool ret = true;
  POSITION pos = l.GetHeadPosition();
  while(pos && ret)
    ret = IsAncestor(a, l.GetNext(pos));

  return ret;
}

template<class T>
bool
__IsAncestorEx(T a, T b)
{
  return (a == b) ? true : b != 0 && __IsAncestorEx(a, (T)(b->Parent()));
}

#include "cosmic.h"
#include "static.h"
#include "dynamic.h"
#include "manager.h"
#include "digraph.h"

// explicit instantiation of template functions
#define CALL_FINDEX(a, b, c) {return __FindEx(a,b,c);}


DESERT_API bool Find(CTypedPtrList<CPtrList, CCosmic*>& a, CCosmic*& b, const TCHAR* c) CALL_FINDEX(a, b, c);
DESERT_API bool Find(CTypedPtrList<CPtrList, CSpace*>& a, CSpace*& b, const TCHAR* c) CALL_FINDEX(a, b, c);
DESERT_API bool Find(CTypedPtrList<CPtrList, CElementContainer*>& a, CElementContainer*& b, const TCHAR * c) CALL_FINDEX(a, b, c);
DESERT_API bool Find(CTypedPtrList<CPtrList, CDynElementContainer*>& a, CDynElementContainer*& b, const TCHAR* c) CALL_FINDEX(a, b, c);
DESERT_API bool Find(CTypedPtrList<CPtrList, CElement*>& a, CElement*& b, const TCHAR* c) CALL_FINDEX(a, b, c);
DESERT_API bool Find(CTypedPtrList<CPtrList, CDomain*>& a, CDomain*& b, const TCHAR* c) CALL_FINDEX(a, b, c);
DESERT_API bool Find(CTypedPtrList<CPtrList, CConstraintSet*>& a, CConstraintSet*& b, const TCHAR* c) CALL_FINDEX(a, b, c);
DESERT_API bool Find(CTypedPtrList<CPtrList, CConstraint*>& a, CConstraint*& b, const TCHAR* c) CALL_FINDEX(a, b, c);
/////
DESERT_API bool Find(CTypedPtrList<CPtrList, CFormulaSet*>& a, CFormulaSet*& b, const TCHAR* c) CALL_FINDEX(a, b, c);
DESERT_API bool Find(CTypedPtrList<CPtrList, CFormula*>& a, CFormula*& b, const TCHAR* c) CALL_FINDEX(a, b, c);
DESERT_API bool Find(CTypedPtrList<CPtrList, CDynFormulaSet*>& a, CDynFormulaSet*& b, const TCHAR* c) CALL_FINDEX(a, b, c);
DESERT_API bool Find(CTypedPtrList<CPtrList, CDynFormula*>& a, CDynFormula*& b, const TCHAR* c) CALL_FINDEX(a, b, c);
/////
DESERT_API bool Find(CTypedPtrList<CPtrList, CProperty*>& a, CProperty*& b, const TCHAR* c) CALL_FINDEX(a, b, c);
DESERT_API bool Find(CTypedPtrList<CPtrList, CDynSpace*>& a, CDynSpace*& b, const TCHAR* c) CALL_FINDEX(a, b, c);
DESERT_API bool Find(CTypedPtrList<CPtrList, CDynElement*>& a, CDynElement*& b, const TCHAR* c) CALL_FINDEX(a, b, c);
DESERT_API bool Find(CTypedPtrList<CPtrList, CDynConstraint*>& a,  CDynConstraint*& b, const TCHAR* c) CALL_FINDEX(a, b, c);
DESERT_API bool Find(CTypedPtrList<CPtrList, CDynConstraintSet*>& a, CDynConstraintSet*& b, const TCHAR* c) CALL_FINDEX(a, b, c);
DESERT_API bool Find(CTypedPtrList<CPtrList, CDynProperty*>& a, CDynProperty*& b, const TCHAR* c) CALL_FINDEX(a, b, c);
DESERT_API bool Find(CTypedPtrList<CPtrList, CDynDomain*>& a, CDynDomain*& b, const TCHAR* c) CALL_FINDEX(a, b, c);


DESERT_API bool Find(CTypedPtrList<CPtrList, CSpace*>& a, CSpace*& b, long c) CALL_FINDEX(a, b, c);
DESERT_API bool Find(CTypedPtrList<CPtrList, CElementContainer*>& a, CElementContainer*& b, long c) CALL_FINDEX(a, b, c);
DESERT_API bool Find(CTypedPtrList<CPtrList, CElement*>& a, CElement*& b, long c) CALL_FINDEX(a, b, c);
DESERT_API bool Find(CTypedPtrList<CPtrList, CDomain*>& a, CDomain*& b, long c)CALL_FINDEX(a, b, c);
DESERT_API bool Find(CTypedPtrList<CPtrList, CConstraintSet*>& a, CConstraintSet*& b, long c)CALL_FINDEX(a, b, c);
DESERT_API bool Find(CTypedPtrList<CPtrList, CConstraint*>& a, CConstraint*& b, long c)CALL_FINDEX(a, b, c);
/////
DESERT_API bool Find(CTypedPtrList<CPtrList, CFormulaSet*>& a, CFormulaSet*& b, long c)CALL_FINDEX(a, b, c);
DESERT_API bool Find(CTypedPtrList<CPtrList, CFormula*>& a, CFormula*& b, long c)CALL_FINDEX(a, b, c);
/////
DESERT_API bool Find(CTypedPtrList<CPtrList, CProperty*>& a, CProperty*& b, long c)CALL_FINDEX(a, b, c);
DESERT_API bool Find(CTypedPtrList<CPtrList, CRelation*>& a, CRelation*& b, long c)CALL_FINDEX(a, b, c);
DESERT_API bool Find(CTypedPtrList<CPtrList, CDynDomain*>& a, CDynDomain*& b, long c)CALL_FINDEX(a, b, c);
DESERT_API bool Find(CTypedPtrList<CPtrList, CDynConstraint*>& a, CDynConstraint*& b, long c)CALL_FINDEX(a, b, c);
DESERT_API bool Find(CTypedPtrList<CPtrList, CDynConstraintSet*>& a, CDynConstraintSet*& b, long c)CALL_FINDEX(a, b, c);
DESERT_API bool Find(CTypedPtrList<CPtrList, CDynProperty*>& a, CDynProperty*& b, long c)CALL_FINDEX(a, b, c);
DESERT_API bool Find(CTypedPtrList<CPtrList, CDynRelation*>& a, CDynRelation*& b, long c)CALL_FINDEX(a, b, c);
DESERT_API bool Find(CTypedPtrList<CPtrList, CDynSpace*>& a, CDynSpace*& b, long c)CALL_FINDEX(a, b, c);
DESERT_API bool Find(CTypedPtrList<CPtrList, CDynElement*>& a, CDynElement*& b, long c)CALL_FINDEX(a, b, c);

DESERT_API bool Find(CTypedPtrList<CPtrList, CDynDomain*>& a, CDynDomain*& b, const CStaticObj *c)CALL_FINDEX(a, b, c);
DESERT_API bool Find(CTypedPtrList<CPtrList, CDynConstraint*>& a, CDynConstraint*& b, const CStaticObj *c)CALL_FINDEX(a, b, c);
DESERT_API bool Find(CTypedPtrList<CPtrList, CDynConstraintSet*>& a, CDynConstraintSet*& b, const CStaticObj *c)CALL_FINDEX(a, b, c);
DESERT_API bool Find(CTypedPtrList<CPtrList, CDynProperty*>& a, CDynProperty*& b, const CStaticObj *c)CALL_FINDEX(a, b, c);
DESERT_API bool Find(CTypedPtrList<CPtrList, CDynRelation*>& a, CDynRelation*& b, const CStaticObj *c)CALL_FINDEX(a, b, c);
DESERT_API bool Find(CTypedPtrList<CPtrList, CDynSpace*>& a, CDynSpace*& b, const CStaticObj *c)CALL_FINDEX(a, b, c);
DESERT_API bool Find(CTypedPtrList<CPtrList, CDynElement*>& a, CDynElement*& b, const CStaticObj *c)CALL_FINDEX(a, b, c);

DESERT_API bool Find(CTypedPtrList<CPtrList, CDynFormula*>& a, CDynFormula*& b, long c)CALL_FINDEX(a, b, c);
DESERT_API bool Find(CTypedPtrList<CPtrList, CDynFormulaSet*>& a, CDynFormulaSet*& b, long c)CALL_FINDEX(a, b, c);
DESERT_API bool Find(CTypedPtrList<CPtrList, CDynFormula*>& a, CDynFormula*& b, const CStaticObj *c)CALL_FINDEX(a, b, c);
DESERT_API bool Find(CTypedPtrList<CPtrList, CDynFormulaSet*>& a, CDynFormulaSet*& b, const CStaticObj *c)CALL_FINDEX(a, b, c);

#define CALL_FINDELEMENTEX(a,b) {return __FindElementEx(a, b);}

DESERT_API CElement* FindElement(CTypedPtrList<CPtrList , CSpace*>& b, long c) CALL_FINDELEMENTEX(b,c);
DESERT_API CElement* FindElement(CTypedPtrList<CPtrList , CDomain*>& b, long c) CALL_FINDELEMENTEX(b,c);
DESERT_API CDynElement* FindElement(CTypedPtrList<CPtrList , CDynSpace*>& b, const CElement* c) CALL_FINDELEMENTEX(b,c);
DESERT_API CDynElement* FindElement(CTypedPtrList<CPtrList, CDynDomain*>& b, const CElement* c) CALL_FINDELEMENTEX(b,c);


DESERT_API CCosmic *	FindRoot(CTypedPtrList<CPtrList, CCosmic *>& list) {return __FindRootEx(list);};
DESERT_API CDynamicObj* FindRoot(CTypedPtrList<CPtrList, CDynamicObj *>& list) {return __FindRootEx(list);};
DESERT_API bool			FindRoot(CDynamicObj * a, CTypedPtrList<CPtrList, CDynamicObj *>& list) {return __FindRootEx(a, list);};


DESERT_API bool IsAncestor(CCosmic *a, CCosmic *b)			{return __IsAncestorEx(a, b);};
DESERT_API bool IsAncestor(CDynamicObj *a, CDynamicObj *b)	{return __IsAncestorEx(a, b);};

DESERT_API void Destroy(CTypedPtrList<CPtrList, CSpace*>& a) {__DestroyEx(a);};
DESERT_API void Destroy(CTypedPtrList<CPtrList, CDomain*>& a) {__DestroyEx(a);};
DESERT_API void Destroy(CTypedPtrList<CPtrList, CConstraintSet*>& a) {__DestroyEx(a);};
/////
DESERT_API void Destroy(CTypedPtrList<CPtrList, CFormulaSet*>& a) {__DestroyEx(a);};
/////
DESERT_API void Destroy(CTypedPtrList<CPtrList, CElement*>& a) {__DestroyEx(a);};
DESERT_API void Destroy(CTypedPtrList<CPtrList, CProperty*>& a) {__DestroyEx(a);};
DESERT_API void Destroy(CTypedPtrList<CPtrList, CRelation*>& a) {__DestroyEx(a);};
DESERT_API void Destroy(CTypedPtrList<CPtrList, CConstraint*>& a) {__DestroyEx(a);};
/////
DESERT_API void Destroy(CTypedPtrList<CPtrList, CFormula*>& a) {__DestroyEx(a);};
DESERT_API void Destroy(CTypedPtrList<CPtrList, CSimpleFormula*>& a) {__DestroyEx(a);};
/////
DESERT_API void Destroy(CTypedPtrList<CPtrList, CDynSpace*>& a) {__DestroyEx(a);};
DESERT_API void Destroy(CTypedPtrList<CPtrList, CDynDomain*>& a) {__DestroyEx(a);};
DESERT_API void Destroy(CTypedPtrList<CPtrList, CDynConstraintSet*>& a) {__DestroyEx(a);};
DESERT_API void Destroy(CTypedPtrList<CPtrList, CDynConstraint*>& a) {__DestroyEx(a);};
DESERT_API void Destroy(CTypedPtrList<CPtrList, CDynElement*>& a) {__DestroyEx(a);};
DESERT_API void Destroy(CTypedPtrList<CPtrList, CDynProperty*>& a) {__DestroyEx(a);};
DESERT_API void Destroy(CTypedPtrList<CPtrList, CDynRelation*>& a) {__DestroyEx(a);};

DESERT_API void Destroy(CTypedPtrList<CPtrList, CGraphNode*>& a) {__DestroyEx(a);};
DESERT_API void Destroy(CTypedPtrList<CPtrList, CGraphPath*>& a) {__DestroyEx(a);};
DESERT_API void Destroy(CTypedPtrList<CPtrList, CDynElementContainer*>& a) {__DestroyEx(a);};

DESERT_API void Destroy(CTypedPtrList<CPtrList, CDynFormulaSet*>& a) {__DestroyEx(a);};
DESERT_API void Destroy(CTypedPtrList<CPtrList, CDynFormula*>& a) {__DestroyEx(a);};

/*

template void Destroy<CSpace*>(CTypedPtrList<CPtrList, CSpace*>&);
template void Destroy<CDomain*>(CTypedPtrList<CPtrList, CDomain*>&);
template void Destroy<CConstraintSet*>(CTypedPtrList<CPtrList, CConstraintSet*>&);
template void Destroy<CElement*>(CTypedPtrList<CPtrList, CElement*>&);
template void Destroy<CProperty*>(CTypedPtrList<CPtrList, CProperty*>&);
template void Destroy<CRelation*>(CTypedPtrList<CPtrList, CRelation*>&);
template void Destroy<CConstraint*>(CTypedPtrList<CPtrList, CConstraint*>&);
template void Destroy<CDynSpace*>(CTypedPtrList<CPtrList, CDynSpace*>&);
template void Destroy<CDynDomain*>(CTypedPtrList<CPtrList, CDynDomain*>&);
template void Destroy<CDynConstraintSet*>(CTypedPtrList<CPtrList, CDynConstraintSet*>&);
template void Destroy<CDynElement*>(CTypedPtrList<CPtrList, CDynElement*>&);
template void Destroy<CDynProperty*>(CTypedPtrList<CPtrList, CDynProperty*>&);
template void Destroy<CDynRelation*>(CTypedPtrList<CPtrList, CDynRelation*>&);
template void Destroy<CDynConstraint*>(CTypedPtrList<CPtrList, CDynConstraint*>&);
template void Destroy<CGraphNode*>(CTypedPtrList<CPtrList, CGraphNode*>&);
template void Destroy<CGraphPath*>(CTypedPtrList<CPtrList, CGraphPath*>&);
template void Destroy<CDynElementContainer*>(CTypedPtrList<CPtrList, CDynElementContainer*>&);

template bool IsAncestor<CCosmic *>(CCosmic *a, CCosmic *b);
template bool IsAncestor<CDynamicObj *>(CDynamicObj *a, CDynamicObj *b);


template CCosmic *FindRoot<CCosmic *>(CTypedPtrList<CPtrList, CCosmic *>& list);
template CDynamicObj *FindRoot<CDynamicObj *>(CTypedPtrList<CPtrList, CDynamicObj *>& list);
template bool FindRoot<CDynamicObj *>(CDynamicObj *, CTypedPtrList<CPtrList, CDynamicObj *>& list);


template CElement* FindElement<CSpace*>(CTypedPtrList<CPtrList, CSpace*>&, long);
template CElement* FindElement<CSpace*>(CTypedPtrList<CPtrList, CDomain*>&, long);
template CDynElement* FindElement<CDynSpace*>(CTypedPtrList<CPtrList, CDynSpace*>&, const CElement*);
template CDynElement* FindElement<CDynDomain*>(CTypedPtrList<CPtrList, CDynDomain*>&, const CElement*);


template bool __FindEx<CCosmic*>(CTypedPtrList<CPtrList, CCosmic*>&, CCosmic*&, const TCHAR*);
template bool __FindEx<CSpace*>(CTypedPtrList<CPtrList, CSpace*>&, CSpace*&, const TCHAR*);
template bool __FindEx<CElementContainer*>(CTypedPtrList<CPtrList, CElementContainer*>&, CElementContainer*&, const TCHAR *);
template bool __FindEx<CDynElementContainer*>(CTypedPtrList<CPtrList, CDynElementContainer*>&, CDynElementContainer*&, const TCHAR*);
template bool __FindEx<CElement*>(CTypedPtrList<CPtrList, CElement*>&, CElement*&, const TCHAR*);
template bool __FindEx<CDomain*>(CTypedPtrList<CPtrList, CDomain*>&, CDomain*&, const TCHAR*);
template bool __FindEx<CConstraintSet*>(CTypedPtrList<CPtrList, CConstraintSet*>&, CConstraintSet*&, const TCHAR*);
template bool __FindEx<CConstraint*>(CTypedPtrList<CPtrList, CConstraint*>&, CConstraint*&, const TCHAR*);
template bool __FindEx<CProperty*>(CTypedPtrList<CPtrList, CProperty*>&, CProperty*&, const TCHAR*);
template bool __FindEx<CDynSpace*>(CTypedPtrList<CPtrList, CDynSpace*>&, CDynSpace*&, const TCHAR*);
template bool __FindEx<CDynElement*>(CTypedPtrList<CPtrList, CDynElement*>&, CDynElement*&, const TCHAR*);
template bool __FindEx<CDynConstraint*>(CTypedPtrList<CPtrList, CDynConstraint*>&, CDynConstraint*&, const TCHAR*);
template bool __FindEx<CDynConstraintSet*>(CTypedPtrList<CPtrList, CDynConstraintSet*>&, CDynConstraintSet*&, const TCHAR*);
template bool __FindEx<CDynProperty*>(CTypedPtrList<CPtrList, CDynProperty*>&, CDynProperty*&, const TCHAR*);
template bool __FindEx<CDynDomain*>(CTypedPtrList<CPtrList, CDynDomain*>&, CDynDomain*&, const TCHAR*);

  
template bool Find<CSpace*>(CTypedPtrList<CPtrList, CSpace*>&, CSpace*&, long);
template bool Find<CElementContainer*>(CTypedPtrList<CPtrList, CElementContainer*>&, CElementContainer*&, long);
template bool Find<CElement*>(CTypedPtrList<CPtrList, CElement*>&, CElement*&, long);
template bool Find<CDomain*>(CTypedPtrList<CPtrList, CDomain*>&, CDomain*&, long);
template bool Find<CConstraintSet*>(CTypedPtrList<CPtrList, CConstraintSet*>&, CConstraintSet*&, long);
template bool Find<CConstraint*>(CTypedPtrList<CPtrList, CConstraint*>&, CConstraint*&, long);
template bool Find<CProperty*>(CTypedPtrList<CPtrList, CProperty*>&, CProperty*&, long);
template bool Find<CRelation*>(CTypedPtrList<CPtrList, CRelation*>&, CRelation*&, long);
template bool Find<CDynDomain*>(CTypedPtrList<CPtrList, CDynDomain*>&, CDynDomain*&, long);
template bool Find<CDynConstraint*>(CTypedPtrList<CPtrList, CDynConstraint*>&, CDynConstraint*&, long);
template bool Find<CDynConstraintSet*>(CTypedPtrList<CPtrList, CDynConstraintSet*>&, CDynConstraintSet*&, long);
template bool Find<CDynProperty*>(CTypedPtrList<CPtrList, CDynProperty*>&, CDynProperty*&, long);
template bool Find<CDynRelation*>(CTypedPtrList<CPtrList, CDynRelation*>&, CDynRelation*&, long);
template bool Find<CDynSpace*>(CTypedPtrList<CPtrList, CDynSpace*>&, CDynSpace*&, long);
template bool Find<CDynElement*>(CTypedPtrList<CPtrList, CDynElement*>&, CDynElement*&, long);



template bool __FindEx<CSpace*>(CTypedPtrList<CPtrList, CSpace*>&, CSpace*&, long);
template bool __FindEx<CElementContainer*>(CTypedPtrList<CPtrList, CElementContainer*>&, CElementContainer*&, long);
template bool __FindEx<CElement*>(CTypedPtrList<CPtrList, CElement*>&, CElement*&, long);
template bool __FindEx<CDomain*>(CTypedPtrList<CPtrList, CDomain*>&, CDomain*&, long);
template bool __FindEx<CConstraintSet*>(CTypedPtrList<CPtrList, CConstraintSet*>&, CConstraintSet*&, long);
template bool __FindEx<CConstraint*>(CTypedPtrList<CPtrList, CConstraint*>&, CConstraint*&, long);
template bool __FindEx<CProperty*>(CTypedPtrList<CPtrList, CProperty*>&, CProperty*&, long);
template bool __FindEx<CRelation*>(CTypedPtrList<CPtrList, CRelation*>&, CRelation*&, long);
template bool __FindEx<CDynDomain*>(CTypedPtrList<CPtrList, CDynDomain*>&, CDynDomain*&, long);
template bool __FindEx<CDynConstraint*>(CTypedPtrList<CPtrList, CDynConstraint*>&, CDynConstraint*&, long);
template bool __FindEx<CDynConstraintSet*>(CTypedPtrList<CPtrList, CDynConstraintSet*>&, CDynConstraintSet*&, long);
template bool __FindEx<CDynProperty*>(CTypedPtrList<CPtrList, CDynProperty*>&, CDynProperty*&, long);
template bool __FindEx<CDynRelation*>(CTypedPtrList<CPtrList, CDynRelation*>&, CDynRelation*&, long);
template bool __FindEx<CDynSpace*>(CTypedPtrList<CPtrList, CDynSpace*>&, CDynSpace*&, long);
template bool __FindEx<CDynElement*>(CTypedPtrList<CPtrList, CDynElement*>&, CDynElement*&, long);
*/

/*
template bool Find<CDynElement*>(CTypedPtrList<CPtrList, CDynElement*>&, CDynElement*&, const CStaticObj*);
template bool Find<CDynSpace*>(CTypedPtrList<CPtrList, CDynSpace*>&, CDynSpace*&, const CStaticObj*);
template bool Find<CDynDomain*>(CTypedPtrList<CPtrList, CDynDomain*>&, CDynDomain*&, const CStaticObj*);

*/


#define CALL_REMOVE_EX3	{return __RemoveEx(a, b, c);}

DESERT_API bool Remove(CTypedPtrList<CPtrList, CSpace*>& a, CSpace*& b, long c)CALL_REMOVE_EX3;
DESERT_API bool Remove(CTypedPtrList<CPtrList, CDomain*>& a, CDomain*& b, long c)CALL_REMOVE_EX3;
DESERT_API bool Remove(CTypedPtrList<CPtrList, CConstraintSet*>& a, CConstraintSet*& b, long c)CALL_REMOVE_EX3;
////
DESERT_API bool Remove(CTypedPtrList<CPtrList, CFormulaSet*>& a, CFormulaSet*& b, long c)CALL_REMOVE_EX3;
////
DESERT_API bool Remove(CTypedPtrList<CPtrList, CElement*>& a, CElement*& b, long c)CALL_REMOVE_EX3;
DESERT_API bool Remove(CTypedPtrList<CPtrList, CProperty*>& a, CProperty*& b, long c)CALL_REMOVE_EX3;

DESERT_API bool Remove(CTypedPtrList<CPtrList, CRelation*>& a, CRelation*& b, long c)CALL_REMOVE_EX3;

DESERT_API bool Remove(CTypedPtrList<CPtrList, CConstraint*>& a, CConstraint*& b, long c)CALL_REMOVE_EX3;
/////
DESERT_API bool Remove(CTypedPtrList<CPtrList, CFormula*>& a, CFormula*& b, long c)CALL_REMOVE_EX3;
/////
DESERT_API bool Remove(CTypedPtrList<CPtrList, CDynSpace*>& a, CDynSpace*& b, long c)CALL_REMOVE_EX3;
DESERT_API bool Remove(CTypedPtrList<CPtrList, CDynDomain*>& a, CDynDomain*& b, long c)CALL_REMOVE_EX3;
DESERT_API bool Remove(CTypedPtrList<CPtrList, CDynConstraintSet*>& a, CDynConstraintSet*& b, long c)CALL_REMOVE_EX3;
DESERT_API bool Remove(CTypedPtrList<CPtrList, CDynConstraint*>& a, CDynConstraint*& b, long c)CALL_REMOVE_EX3;
DESERT_API bool Remove(CTypedPtrList<CPtrList, CDynElement*>& a, CDynElement*& b, long c)CALL_REMOVE_EX3;
DESERT_API bool Remove(CTypedPtrList<CPtrList, CDynProperty*>& a, CDynProperty*& b, long c)CALL_REMOVE_EX3;

DESERT_API bool Remove(CTypedPtrList<CPtrList, CDynRelation*>& a, CDynRelation*& b, long c)CALL_REMOVE_EX3;


DESERT_API bool Remove(CTypedPtrList<CPtrList, CSpace*>& a, CSpace*& b, const TCHAR* c)CALL_REMOVE_EX3;
DESERT_API bool Remove(CTypedPtrList<CPtrList, CDomain*>& a, CDomain*& b, const TCHAR* c)CALL_REMOVE_EX3;
DESERT_API bool Remove(CTypedPtrList<CPtrList, CConstraintSet*>& a, CConstraintSet*& b, const TCHAR* c)CALL_REMOVE_EX3;
DESERT_API bool Remove(CTypedPtrList<CPtrList, CElement*>& a, CElement*& b, const TCHAR* c)CALL_REMOVE_EX3;
DESERT_API bool Remove(CTypedPtrList<CPtrList, CProperty*>& a, CProperty*& b, const TCHAR* c)CALL_REMOVE_EX3;
DESERT_API bool Remove(CTypedPtrList<CPtrList, CRelation*>& a, CRelation*& b, const TCHAR * c)CALL_REMOVE_EX3;
DESERT_API bool Remove(CTypedPtrList<CPtrList, CConstraint*>& a, CConstraint*& b, const TCHAR* c)CALL_REMOVE_EX3;
DESERT_API bool Remove(CTypedPtrList<CPtrList, CDynSpace*>& a, CDynSpace*& b, const TCHAR* c)CALL_REMOVE_EX3;
DESERT_API bool Remove(CTypedPtrList<CPtrList, CDynDomain*>& a, CDynDomain*& b, const TCHAR* c)CALL_REMOVE_EX3;
DESERT_API bool Remove(CTypedPtrList<CPtrList, CDynConstraintSet*>& a, CDynConstraintSet*& b, const TCHAR* c)CALL_REMOVE_EX3;
DESERT_API bool Remove(CTypedPtrList<CPtrList, CDynElement*>& a, CDynElement*& b, const TCHAR* c)CALL_REMOVE_EX3;
DESERT_API bool Remove(CTypedPtrList<CPtrList, CDynProperty*>& a, CDynProperty*& b, const TCHAR* c)CALL_REMOVE_EX3;
DESERT_API bool Remove(CTypedPtrList<CPtrList, CDynConstraint*>& a, CDynConstraint*& b, const TCHAR* c)CALL_REMOVE_EX3;
DESERT_API bool Remove(CTypedPtrList<CPtrList, CDynRelation*>& a, CDynRelation*& b, const TCHAR * c)CALL_REMOVE_EX3;

DESERT_API bool Remove(CTypedPtrList<CPtrList, CDynFormulaSet*>& a, CDynFormulaSet*& b, long c)CALL_REMOVE_EX3;
DESERT_API bool Remove(CTypedPtrList<CPtrList, CDynFormula*>& a, CDynFormula*& b, long c)CALL_REMOVE_EX3;


#define CALL_REMOVE_EX2	{return __RemoveEx(a, b);}

DESERT_API bool Remove(CTypedPtrList<CPtrList, CSpace*>& a, CSpace* b) CALL_REMOVE_EX2;
DESERT_API bool Remove(CTypedPtrList<CPtrList, CDomain*>& a, CDomain* b) CALL_REMOVE_EX2;
DESERT_API bool Remove(CTypedPtrList<CPtrList, CConstraintSet*>& a, CConstraintSet* b) CALL_REMOVE_EX2;
DESERT_API bool Remove(CTypedPtrList<CPtrList, CConstraint*>& a, CConstraint* b) CALL_REMOVE_EX2;
/////
DESERT_API bool Remove(CTypedPtrList<CPtrList, CFormulaSet*>& a, CFormulaSet* b) CALL_REMOVE_EX2;
DESERT_API bool Remove(CTypedPtrList<CPtrList, CFormula*>& a, CFormula* b) CALL_REMOVE_EX2;
/////
DESERT_API bool Remove(CTypedPtrList<CPtrList, CElement*>& a, CElement* b) CALL_REMOVE_EX2;
DESERT_API bool Remove(CTypedPtrList<CPtrList, CProperty*>& a, CProperty* b) CALL_REMOVE_EX2;
DESERT_API bool Remove(CTypedPtrList<CPtrList, CRelation*>& a, CRelation* b)CALL_REMOVE_EX2;
DESERT_API bool Remove(CTypedPtrList<CPtrList, CDynSpace*>& a, CDynSpace* b) CALL_REMOVE_EX2;
DESERT_API bool Remove(CTypedPtrList<CPtrList, CDynDomain*>& a, CDynDomain* b) CALL_REMOVE_EX2;
DESERT_API bool Remove(CTypedPtrList<CPtrList, CDynConstraintSet*>& a, CDynConstraintSet* b) CALL_REMOVE_EX2;
DESERT_API bool Remove(CTypedPtrList<CPtrList, CDynElement*>& a, CDynElement* b) CALL_REMOVE_EX2;
DESERT_API bool Remove(CTypedPtrList<CPtrList, CDynProperty*>& a, CDynProperty* b) CALL_REMOVE_EX2;
DESERT_API bool Remove(CTypedPtrList<CPtrList, CDynConstraint*>& a, CDynConstraint* b) CALL_REMOVE_EX2;
DESERT_API bool Remove(CTypedPtrList<CPtrList, CDynRelation*>& a, CDynRelation* b)CALL_REMOVE_EX2;


#define CALL_INSERT_EX {return __InsertEx(a, b);}
DESERT_API bool Insert(CTypedPtrList<CPtrList, CElement*>& a, CElement* b)CALL_INSERT_EX;
DESERT_API bool Insert(CTypedPtrList<CPtrList, CSpace*>& a, CSpace* b)CALL_INSERT_EX;
DESERT_API bool Insert(CTypedPtrList<CPtrList, CDomain*>& a, CDomain* b)CALL_INSERT_EX;
DESERT_API bool Insert(CTypedPtrList<CPtrList, CRelation*>& a, CRelation* b)CALL_INSERT_EX;
DESERT_API bool Insert(CTypedPtrList<CPtrList, CProperty*>& a, CProperty* b)CALL_INSERT_EX;
DESERT_API bool Insert(CTypedPtrList<CPtrList, CConstraintSet*>& a,  CConstraintSet* b)CALL_INSERT_EX;
DESERT_API bool Insert(CTypedPtrList<CPtrList, CConstraint*>& a, CConstraint* b)CALL_INSERT_EX;
/////
DESERT_API bool Insert(CTypedPtrList<CPtrList, CFormulaSet*>& a,  CFormulaSet* b)CALL_INSERT_EX;
DESERT_API bool Insert(CTypedPtrList<CPtrList, CFormula*>& a, CFormula* b)CALL_INSERT_EX;
DESERT_API bool Insert(CTypedPtrList<CPtrList, CDynFormulaSet*>& a,  CDynFormulaSet* b)CALL_INSERT_EX;
DESERT_API bool Insert(CTypedPtrList<CPtrList, CDynFormula*>& a, CDynFormula* b)CALL_INSERT_EX;
/////
DESERT_API bool Insert(CTypedPtrList<CPtrList, CDynElement*>& a, CDynElement* b)CALL_INSERT_EX;
DESERT_API bool Insert(CTypedPtrList<CPtrList, CDynSpace*>& a, CDynSpace* b)CALL_INSERT_EX;
DESERT_API bool Insert(CTypedPtrList<CPtrList, CDynDomain*>& a, CDynDomain* b)CALL_INSERT_EX;
DESERT_API bool Insert(CTypedPtrList<CPtrList, CDynConstraintSet*>& a, CDynConstraintSet* b)CALL_INSERT_EX;
DESERT_API bool Insert(CTypedPtrList<CPtrList, CDynProperty*>& a, CDynProperty* b)CALL_INSERT_EX;
DESERT_API bool Insert(CTypedPtrList<CPtrList, CDynRelation*>& a, CDynRelation* b)CALL_INSERT_EX;
DESERT_API bool Insert(CTypedPtrList<CPtrList, CDynConstraint*>& a, CDynConstraint* b)CALL_INSERT_EX;

#define CALL_CLIST_EX	{return __CloneListEx(a);}

DESERT_API CCosmicList* CloneList(CTypedPtrList<CPtrList, CElement*>& a) CALL_CLIST_EX;
DESERT_API CCosmicList* CloneList(CTypedPtrList<CPtrList, CDynElement*>& a) CALL_CLIST_EX;

DESERT_API int			Index(CTypedPtrList<CPtrList, CDynElement*>& a, CDynElement* b) {return __IndexEx(a,b);};

DESERT_API CConstraint * FindConstraint(CTypedPtrList<CPtrList, CConstraintSet*>& list , long id) { return __FindConstraintEx(list, id);};
DESERT_API CDynConstraint * FindConstraint(CTypedPtrList<CPtrList, CDynConstraintSet*>& list, const CConstraint *core){ return __FindConstraintEx(list, core);};


/*
template bool Remove<CSpace*>(CTypedPtrList<CPtrList, CSpace*>&, CSpace*&, long);
template bool Remove<CDomain*>(CTypedPtrList<CPtrList, CDomain*>&, CDomain*&, long);
template bool Remove<CConstraintSet*>(CTypedPtrList<CPtrList, CConstraintSet*>&, CConstraintSet*&, long);
template bool Remove<CElement*>(CTypedPtrList<CPtrList, CElement*>&, CElement*&, long);
template bool Remove<CProperty*>(CTypedPtrList<CPtrList, CProperty*>&, CProperty*&, long);
template bool Remove<CRelation*>(CTypedPtrList<CPtrList, CRelation*>&, CRelation*&, long);
template bool Remove<CConstraint*>(CTypedPtrList<CPtrList, CConstraint*>&, CConstraint*&, long);
template bool Remove<CDynSpace*>(CTypedPtrList<CPtrList, CDynSpace*>&, CDynSpace*&, long);
template bool Remove<CDynDomain*>(CTypedPtrList<CPtrList, CDynDomain*>&, CDynDomain*&, long);
template bool Remove<CDynConstraintSet*>(CTypedPtrList<CPtrList, CDynConstraintSet*>&, CDynConstraintSet*&, long);
template bool Remove<CDynElement*>(CTypedPtrList<CPtrList, CDynElement*>&, CDynElement*&, long);
template bool Remove<CDynProperty*>(CTypedPtrList<CPtrList, CDynProperty*>&, CDynProperty*&, long);
template bool Remove<CDynConstraint*>(CTypedPtrList<CPtrList, CDynConstraint*>&, CDynConstraint*&, long);
template bool Remove<CDynRelation*>(CTypedPtrList<CPtrList, CDynRelation*>&, CDynRelation*&, long);


template bool Remove<CSpace*>(CTypedPtrList<CPtrList, CSpace*>&, CSpace*&, const TCHAR*);
template bool Remove<CDomain*>(CTypedPtrList<CPtrList, CDomain*>&, CDomain*&, const TCHAR*);
template bool Remove<CConstraintSet*>(CTypedPtrList<CPtrList, CConstraintSet*>&, CConstraintSet*&, const TCHAR*);
template bool Remove<CElement*>(CTypedPtrList<CPtrList, CElement*>&, CElement*&, const TCHAR*);
template bool Remove<CProperty*>(CTypedPtrList<CPtrList, CProperty*>&, CProperty*&, const TCHAR*);
template bool Remove<CConstraint*>(CTypedPtrList<CPtrList, CConstraint*>&, CConstraint*&, const TCHAR*);
template bool Remove<CDynSpace*>(CTypedPtrList<CPtrList, CDynSpace*>&, CDynSpace*&, const TCHAR*);
template bool Remove<CDynDomain*>(CTypedPtrList<CPtrList, CDynDomain*>&, CDynDomain*&, const TCHAR*);
template bool Remove<CDynConstraintSet*>(CTypedPtrList<CPtrList, CDynConstraintSet*>&, CDynConstraintSet*&, const TCHAR*);
template bool Remove<CDynElement*>(CTypedPtrList<CPtrList, CDynElement*>&, CDynElement*&, const TCHAR*);
template bool Remove<CDynProperty*>(CTypedPtrList<CPtrList, CDynProperty*>&, CDynProperty*&, const TCHAR*);
template bool Remove<CDynConstraint*>(CTypedPtrList<CPtrList, CDynConstraint*>&, CDynConstraint*&, const TCHAR*);


template bool Remove<CElement*>(CTypedPtrList<CPtrList, CElement*>&, CElement*);
template bool Remove<CDynElement*>(CTypedPtrList<CPtrList, CDynElement*>&, CDynElement*);

*/



/*
template bool Insert<CElement*>(CTypedPtrList<CPtrList, CElement*>&, CElement*);
template bool Insert<CSpace*>(CTypedPtrList<CPtrList, CSpace*>&, CSpace*);
template bool Insert<CDomain*>(CTypedPtrList<CPtrList, CDomain*>&, CDomain*);
template bool Insert<CRelation*>(CTypedPtrList<CPtrList, CRelation*>&, CRelation*);
template bool Insert<CProperty*>(CTypedPtrList<CPtrList, CProperty*>&, CProperty*);
template bool Insert<CConstraintSet*>(CTypedPtrList<CPtrList, CConstraintSet*>&,  CConstraintSet*);
template bool Insert<CConstraint*>(CTypedPtrList<CPtrList, CConstraint*>&, CConstraint*);
template bool Insert<CDynElement*>(CTypedPtrList<CPtrList, CDynElement*>&, CDynElement*);
template bool Insert<CDynSpace*>(CTypedPtrList<CPtrList, CDynSpace*>&, CDynSpace*);
template bool Insert<CDynDomain*>(CTypedPtrList<CPtrList, CDynDomain*>&, CDynDomain*);
template bool Insert<CDynConstraintSet*>(CTypedPtrList<CPtrList, CDynConstraintSet*>&, CDynConstraintSet*);
template bool Insert<CDynProperty*>(CTypedPtrList<CPtrList, CDynProperty*>&, CDynProperty*);
template bool Insert<CDynRelation*>(CTypedPtrList<CPtrList, CDynRelation*>&, CDynRelation*);
template bool Insert<CDynConstraint*>(CTypedPtrList<CPtrList, CDynConstraint*>&, CDynConstraint*);

template CCosmicList* CloneList<CElement*>(CTypedPtrList<CPtrList, CElement*>&);
template CCosmicList* CloneList<CDynElement*>(CTypedPtrList<CPtrList, CDynElement*>&);

template int  Index<CDynElement*>(CTypedPtrList<CPtrList, CDynElement*>&, CDynElement*);
*/