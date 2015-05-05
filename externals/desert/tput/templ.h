// -*-C++-*-
// templ.h

#ifndef TEMPL_H
#define TEMPL_H

#include "core/coredefs.h"

template<class T> void Destroy(CTypedPtrList<CPtrList, T>& list);
template<class T> bool Find(CTypedPtrList<CPtrList, T>& list, T& ret, long id);
template<class T> bool Find(CTypedPtrList<CPtrList, T>& list, T& ret, const char *name);
template<class T> bool Find(CTypedPtrList<CPtrList, T>& list, T& ret, const CStaticObj *core);
template<class T> bool Remove(CTypedPtrList<CPtrList, T>& list, T toremove);
template<class T> bool Remove(CTypedPtrList<CPtrList, T>& list, T& ret, long id);
template<class T> bool Remove(CTypedPtrList<CPtrList, T>& list, T& ret, const char *name);
template<class T> bool Insert(CTypedPtrList<CPtrList, T>& list, T toinsert);
template<class T> int  Index(CTypedPtrList<CPtrList, T>& list, T tofind);
template<class T> CElement *FindElement(CTypedPtrList<CPtrList, T>& list, long id);
template<class T> CDynElement *FindElement(CTypedPtrList<CPtrList, T>& list,
                                           const CElement *core);
template<class T> CConstraint *FindConstraint(CTypedPtrList<CPtrList, T>& list, long id);
template<class T> CDynConstraint *FindConstraint(CTypedPtrList<CPtrList, T>& list,
                                                 const CConstraint *core);
template<class T> CCosmicList *CloneList(CTypedPtrList<CPtrList, T>& list);
template<class T> T FindRoot(CTypedPtrList<CPtrList, T>& list);
template<class T> bool FindRoot(T anchor, CTypedPtrList<CPtrList, T>& list);
template<class T> bool IsAncestor(T ancestor, T me);

//exporting functions
DESERT_API bool FindEx(CTypedPtrList<CPtrList, CDynProperty*>&, CDynProperty*&, const char*);
DESERT_API bool FindEx<CCosmic*>(CTypedPtrList<CPtrList, CCosmic*>&, CCosmic*&, const char*);
DESERT_API bool FindEx<CElementContainer*>(CTypedPtrList<CPtrList, CElementContainer*>&, CElementContainer*&, const char *);
DESERT_API bool FindEx<CElement*>(CTypedPtrList<CPtrList, CElement*>&, CElement*&, const char*);
DESERT_API bool FindEx<CDomain*>(CTypedPtrList<CPtrList, CDomain*>&, CDomain*&, const char*);
DESERT_API bool FindEx<CConstraintSet*>(CTypedPtrList<CPtrList, CConstraintSet*>&, CConstraintSet*&, const char*);
DESERT_API bool FindEx<CSpace*>(CTypedPtrList<CPtrList, CSpace*>&, CSpace*&, const char*);
DESERT_API bool FindEx<CConstraint*>(CTypedPtrList<CPtrList, CConstraint*>&, CConstraint*&, const char*);
DESERT_API bool FindEx<CProperty*>(CTypedPtrList<CPtrList, CProperty*>&, CProperty*&, const char*);
DESERT_API bool FindEx<CDynDomain*>(CTypedPtrList<CPtrList, CDynDomain*>&, CDynDomain*&, const char*);
DESERT_API bool FindEx<CDynElement*>(CTypedPtrList<CPtrList, CDynElement*>&, CDynElement*&, const char*);
DESERT_API bool FindEx<CDynSpace*>(CTypedPtrList<CPtrList, CDynSpace*>&, CDynSpace*&, const char*);
DESERT_API bool FindEx<CDynConstraint*>(CTypedPtrList<CPtrList, CDynConstraint*>&, CDynConstraint*&, const char*);
DESERT_API bool FindEx<CDynConstraintSet*>(CTypedPtrList<CPtrList, CDynConstraintSet*>&, CDynConstraintSet*&, const char*);
DESERT_API bool FindEx<CDynProperty*>(CTypedPtrList<CPtrList, CDynProperty*>&, CDynProperty*&, const char*);



DESERT_API bool FindEx<CSpace*>(CTypedPtrList<CPtrList, CSpace*>&, CSpace*&, long);
DESERT_API bool FindEx<CElementContainer*>(CTypedPtrList<CPtrList, CElementContainer*>&, CElementContainer*&, long);
DESERT_API bool FindEx<CDynElementContainer*>(CTypedPtrList<CPtrList, CDynElementContainer*>&, CDynElementContainer*&, const char*);
DESERT_API bool FindEx<CElement*>(CTypedPtrList<CPtrList, CElement*>&, CElement*&, long);
DESERT_API bool FindEx<CDomain*>(CTypedPtrList<CPtrList, CDomain*>&, CDomain*&, long);
DESERT_API bool FindEx<CConstraintSet*>(CTypedPtrList<CPtrList, CConstraintSet*>&, CConstraintSet*&, long);
DESERT_API bool FindEx<CConstraint*>(CTypedPtrList<CPtrList, CConstraint*>&, CConstraint*&, long);
DESERT_API bool FindEx<CProperty*>(CTypedPtrList<CPtrList, CProperty*>&, CProperty*&, long);
DESERT_API bool FindEx<CRelation*>(CTypedPtrList<CPtrList, CRelation*>&, CRelation*&, long);
DESERT_API bool FindEx<CDynDomain*>(CTypedPtrList<CPtrList, CDynDomain*>&, CDynDomain*&, long);
DESERT_API bool FindEx<CDynDomain*>(CTypedPtrList<CPtrList, CDynDomain*>&, CDynDomain*&, const CStaticObj*);
DESERT_API bool FindEx<CDynSpace*>(CTypedPtrList<CPtrList, CDynSpace*>&, CDynSpace*&, long);
DESERT_API bool FindEx<CDynSpace*>(CTypedPtrList<CPtrList, CDynSpace*>&, CDynSpace*&, const CStaticObj*);
DESERT_API bool FindEx<CDynElement*>(CTypedPtrList<CPtrList, CDynElement*>&, CDynElement*&, long);
DESERT_API bool FindEx<CDynElement*>(CTypedPtrList<CPtrList, CDynElement*>&, CDynElement*&, const CStaticObj*);
DESERT_API bool FindEx<CDynConstraint*>(CTypedPtrList<CPtrList, CDynConstraint*>&, CDynConstraint*&, long);
DESERT_API bool FindEx<CDynConstraintSet*>(CTypedPtrList<CPtrList, CDynConstraintSet*>&, CDynConstraintSet*&, long);
DESERT_API bool FindEx<CDynProperty*>(CTypedPtrList<CPtrList, CDynProperty*>&, CDynProperty*&, long);
DESERT_API bool FindEx<CDynRelation*>(CTypedPtrList<CPtrList, CDynRelation*>&, CDynRelation*&, long);


DESERT_API CCosmic *		FindRootEx<CCosmic *>(CTypedPtrList<CPtrList, CCosmic *>& list);
DESERT_API CDynamicObj *	FindRootEx<CDynamicObj *>(CTypedPtrList<CPtrList, CDynamicObj *>& list);
DESERT_API bool				FindRootEx<CDynamicObj *>(CDynamicObj *, CTypedPtrList<CPtrList, CDynamicObj *>& list);

DESERT_API bool				IsAncestorEx<CCosmic *>(CCosmic *a, CCosmic *b);
DESERT_API bool				IsAncestorEx<CDynamicObj *>(CDynamicObj *a, CDynamicObj *b);
DESERT_API CElement*		FindElementEx<CSpace*>(CTypedPtrList<CPtrList, CSpace*>&, long);
DESERT_API CElement*		FindElementEx<CSpace*>(CTypedPtrList<CPtrList, CDomain*>&, long);
DESERT_API CDynElement*		FindElementEx<CDynSpace*>(CTypedPtrList<CPtrList, CDynSpace*>&, const CElement*);
DESERT_API CDynElement*		FindElementEx<CDynDomain*>(CTypedPtrList<CPtrList, CDynDomain*>&, const CElement*);
DESERT_API int				IndexEx<CDynElement*>(CTypedPtrList<CPtrList, CDynElement*>&, CDynElement*);



#endif // #ifndef TEMPL_H
