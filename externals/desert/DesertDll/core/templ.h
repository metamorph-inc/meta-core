// -*-C++-*-
// templ.h

#ifndef TEMPL_H
#define TEMPL_H

#include "core/coredefs.h"

//template<class T> void Destroy(CTypedPtrList<CPtrList, T>& list);
/*
template<class T> bool Remove(CTypedPtrList<CPtrList, T>& list, T toremove);
template<class T> bool Remove(CTypedPtrList<CPtrList, T>& list, T& ret, long id);
template<class T> bool Remove(CTypedPtrList<CPtrList, T>& list, T& ret, const TCHAR *name);
template<class T> bool Insert(CTypedPtrList<CPtrList, T>& list, T toinsert);

template<class T> int  Index(CTypedPtrList<CPtrList, T>& list, T tofind);
template<class T> CCosmicList *CloneList(CTypedPtrList<CPtrList, T>& list);
*/


/*
template<class T> CElement *FindElement(CTypedPtrList<CPtrList, T>& list, long id);
template<class T> CDynElement *FindElement(CTypedPtrList<CPtrList, T>& list, const CElement *core);
*/

/*
template<class T> CConstraint *FindConstraint(CTypedPtrList<CPtrList, T>& list, long id);
template<class T> CDynConstraint *FindConstraint(CTypedPtrList<CPtrList, T>& list, const CConstraint *core);
*/

DESERT_API CConstraint * FindConstraint(CTypedPtrList<CPtrList, CConstraintSet *>& list, long id);
DESERT_API CDynConstraint * FindConstraint(CTypedPtrList<CPtrList, CDynConstraintSet *>& list, const CConstraint *core);


/*
template<class T> T FindRoot(CTypedPtrList<CPtrList, T>& list);
template<class T> bool FindRoot(T anchor, CTypedPtrList<CPtrList, T>& list);
*/
/*
template<class T> bool IsAncestor(T ancestor, T me);
*/



/*
template<class T> bool __Find(CTypedPtrList<CPtrList, T>& list, T& ret, const TCHAR *name);
template<class T> bool __Find(CTypedPtrList<CPtrList, T>& list, T& ret, long id);
template<class T> bool __Find(CTypedPtrList<CPtrList, T>& list, T& ret, const CStaticObj *core);
*/

DESERT_API bool Find(CTypedPtrList<CPtrList, CCosmic*>&, CCosmic*&, const TCHAR*);
DESERT_API bool Find(CTypedPtrList<CPtrList, CSpace*>&, CSpace*&, const TCHAR*);
DESERT_API bool Find(CTypedPtrList<CPtrList, CElementContainer*>&, CElementContainer*&, const TCHAR *);
DESERT_API bool Find(CTypedPtrList<CPtrList, CDynElementContainer*>&, CDynElementContainer*&, const TCHAR*);
DESERT_API bool Find(CTypedPtrList<CPtrList, CElement*>&, CElement*&, const TCHAR*);
DESERT_API bool Find(CTypedPtrList<CPtrList, CDomain*>&, CDomain*&, const TCHAR*);
DESERT_API bool Find(CTypedPtrList<CPtrList, CConstraintSet*>&, CConstraintSet*&, const TCHAR*);
DESERT_API bool Find(CTypedPtrList<CPtrList, CConstraint*>&, CConstraint*&, const TCHAR*);
/////
DESERT_API bool Find(CTypedPtrList<CPtrList, CFormulaSet*>&, CFormulaSet*&, const TCHAR*);
DESERT_API bool Find(CTypedPtrList<CPtrList, CFormula*>&, CFormula*&, const TCHAR*);
DESERT_API bool Find(CTypedPtrList<CPtrList, CDynFormulaSet*>&, CDynFormulaSet*&, const TCHAR*);
DESERT_API bool Find(CTypedPtrList<CPtrList, CDynFormula*>&, CDynFormula*&, const TCHAR*);
/////
DESERT_API bool Find(CTypedPtrList<CPtrList, CProperty*>&, CProperty*&, const TCHAR*);
DESERT_API bool Find(CTypedPtrList<CPtrList, CDynSpace*>&, CDynSpace*&, const TCHAR*);
DESERT_API bool Find(CTypedPtrList<CPtrList, CDynElement*>&, CDynElement*&, const TCHAR*);
DESERT_API bool Find(CTypedPtrList<CPtrList, CDynConstraint*>&, CDynConstraint*&, const TCHAR*);
DESERT_API bool Find(CTypedPtrList<CPtrList, CDynConstraintSet*>&, CDynConstraintSet*&, const TCHAR*);
DESERT_API bool Find(CTypedPtrList<CPtrList, CDynProperty*>&, CDynProperty*&, const TCHAR*);
DESERT_API bool Find(CTypedPtrList<CPtrList, CDynDomain*>&, CDynDomain*&, const TCHAR*);



DESERT_API bool Find(CTypedPtrList<CPtrList, CSpace*>&, CSpace*&, long);
DESERT_API bool Find(CTypedPtrList<CPtrList, CElementContainer*>&, CElementContainer*&, long);
DESERT_API bool Find(CTypedPtrList<CPtrList, CElement*>&, CElement*&, long);
DESERT_API bool Find(CTypedPtrList<CPtrList, CDomain*>&, CDomain*&, long);
DESERT_API bool Find(CTypedPtrList<CPtrList, CConstraintSet*>&, CConstraintSet*&, long);
DESERT_API bool Find(CTypedPtrList<CPtrList, CConstraint*>&, CConstraint*&, long);
/////
DESERT_API bool Find(CTypedPtrList<CPtrList, CFormulaSet*>&, CFormulaSet*&, long);
DESERT_API bool Find(CTypedPtrList<CPtrList, CFormula*>&, CFormula*&, long);
DESERT_API bool Find(CTypedPtrList<CPtrList, CDynFormulaSet*>&, CDynFormulaSet*&, long);
DESERT_API bool Find(CTypedPtrList<CPtrList, CDynFormula*>&, CDynFormula*&, long);
/////
DESERT_API bool Find(CTypedPtrList<CPtrList, CProperty*>&, CProperty*&, long);
DESERT_API bool Find(CTypedPtrList<CPtrList, CRelation*>&, CRelation*&, long);
DESERT_API bool Find(CTypedPtrList<CPtrList, CDynDomain*>&, CDynDomain*&, long);
DESERT_API bool Find(CTypedPtrList<CPtrList, CDynConstraint*>&, CDynConstraint*&, long);
DESERT_API bool Find(CTypedPtrList<CPtrList, CDynConstraintSet*>&, CDynConstraintSet*&, long);
DESERT_API bool Find(CTypedPtrList<CPtrList, CDynProperty*>&, CDynProperty*&, long);
DESERT_API bool Find(CTypedPtrList<CPtrList, CDynRelation*>&, CDynRelation*&, long);
DESERT_API bool Find(CTypedPtrList<CPtrList, CDynSpace*>&, CDynSpace*&, long);
DESERT_API bool Find(CTypedPtrList<CPtrList, CDynElement*>&, CDynElement*&, long);


DESERT_API bool Find(CTypedPtrList<CPtrList, CDynDomain*>&, CDynDomain*&, const CStaticObj *);
DESERT_API bool Find(CTypedPtrList<CPtrList, CDynConstraint*>&, CDynConstraint*&, const CStaticObj *);
DESERT_API bool Find(CTypedPtrList<CPtrList, CDynConstraintSet*>&, CDynConstraintSet*&, const CStaticObj *);
DESERT_API bool Find(CTypedPtrList<CPtrList, CDynProperty*>&, CDynProperty*&, const CStaticObj *);
DESERT_API bool Find(CTypedPtrList<CPtrList, CDynRelation*>&, CDynRelation*&, const CStaticObj *);
DESERT_API bool Find(CTypedPtrList<CPtrList, CDynSpace*>&, CDynSpace*&, const CStaticObj *);
DESERT_API bool Find(CTypedPtrList<CPtrList, CDynElement*>&, CDynElement*&, const CStaticObj *);
/////
DESERT_API bool Find(CTypedPtrList<CPtrList, CDynFormula*>&, CDynFormula*&, const CStaticObj *);
DESERT_API bool Find(CTypedPtrList<CPtrList, CDynFormulaSet*>&, CDynFormulaSet*&, const CStaticObj *);
/////
DESERT_API void Destroy(CTypedPtrList<CPtrList, CSpace*>&);
DESERT_API void Destroy(CTypedPtrList<CPtrList, CDomain*>&);
DESERT_API void Destroy(CTypedPtrList<CPtrList, CConstraintSet*>&);
DESERT_API void Destroy(CTypedPtrList<CPtrList, CFormulaSet*>&); 
DESERT_API void Destroy(CTypedPtrList<CPtrList, CElement*>&);
DESERT_API void Destroy(CTypedPtrList<CPtrList, CProperty*>&);
DESERT_API void Destroy(CTypedPtrList<CPtrList, CRelation*>&);
DESERT_API void Destroy(CTypedPtrList<CPtrList, CConstraint*>&);
DESERT_API void Destroy(CTypedPtrList<CPtrList, CFormula*>&);
DESERT_API void Destroy(CTypedPtrList<CPtrList, CSimpleFormula*>& a);
DESERT_API void Destroy(CTypedPtrList<CPtrList, CDynSpace*>&);
DESERT_API void Destroy(CTypedPtrList<CPtrList, CDynDomain*>&);
DESERT_API void Destroy(CTypedPtrList<CPtrList, CDynConstraintSet*>&);
DESERT_API void Destroy(CTypedPtrList<CPtrList, CDynConstraint*>&);
DESERT_API void Destroy(CTypedPtrList<CPtrList, CDynElement*>&);
DESERT_API void Destroy(CTypedPtrList<CPtrList, CDynProperty*>&);
DESERT_API void Destroy(CTypedPtrList<CPtrList, CDynRelation*>&);

DESERT_API void Destroy(CTypedPtrList<CPtrList, CGraphNode*>&);
DESERT_API void Destroy(CTypedPtrList<CPtrList, CGraphPath*>&);
DESERT_API void Destroy(CTypedPtrList<CPtrList, CDynElementContainer*>&);

DESERT_API void Destroy(CTypedPtrList<CPtrList, CDynFormulaSet*>&);
DESERT_API void Destroy(CTypedPtrList<CPtrList, CDynFormula*>&);


DESERT_API CElement*	FindElement(CTypedPtrList<CPtrList , CSpace*>& , long );
DESERT_API CElement*	FindElement(CTypedPtrList<CPtrList , CDomain*>& , long );
DESERT_API CDynElement* FindElement(CTypedPtrList<CPtrList , CDynSpace*>& , const CElement* );
DESERT_API CDynElement* FindElement(CTypedPtrList<CPtrList, CDynDomain*>& , const CElement* );

DESERT_API CCosmic*		FindRoot(CTypedPtrList<CPtrList, CCosmic *>& list);
DESERT_API CDynamicObj*	FindRoot(CTypedPtrList<CPtrList, CDynamicObj *>& list);
DESERT_API bool			FindRoot(CDynamicObj *, CTypedPtrList<CPtrList, CDynamicObj *>& list);


DESERT_API bool IsAncestor(CCosmic *a, CCosmic *b) ;
DESERT_API bool IsAncestor(CDynamicObj *a, CDynamicObj *b);



DESERT_API bool Remove(CTypedPtrList<CPtrList, CSpace*>&, CSpace*&, long);
DESERT_API bool Remove(CTypedPtrList<CPtrList, CDomain*>&, CDomain*&, long);
DESERT_API bool Remove(CTypedPtrList<CPtrList, CConstraintSet*>&, CConstraintSet*&, long);
DESERT_API bool Remove(CTypedPtrList<CPtrList, CElement*>&, CElement*&, long);
DESERT_API bool Remove(CTypedPtrList<CPtrList, CProperty*>&, CProperty*&, long);
DESERT_API bool Remove(CTypedPtrList<CPtrList, CRelation*>&, CRelation*&, long);
DESERT_API bool Remove(CTypedPtrList<CPtrList, CConstraint*>&, CConstraint*&, long);
////
DESERT_API bool Remove(CTypedPtrList<CPtrList, CFormula*>&, CFormula*&, long);
////
DESERT_API bool Remove(CTypedPtrList<CPtrList, CDynSpace*>&, CDynSpace*&, long);
DESERT_API bool Remove(CTypedPtrList<CPtrList, CDynDomain*>&, CDynDomain*&, long);
DESERT_API bool Remove(CTypedPtrList<CPtrList, CDynConstraintSet*>&, CDynConstraintSet*&, long);
DESERT_API bool Remove(CTypedPtrList<CPtrList, CDynConstraint*>&, CDynConstraint*&, long);
DESERT_API bool Remove(CTypedPtrList<CPtrList, CDynElement*>&, CDynElement*&, long);
DESERT_API bool Remove(CTypedPtrList<CPtrList, CDynProperty*>&, CDynProperty*&, long);

DESERT_API bool Remove(CTypedPtrList<CPtrList, CDynRelation*>&, CDynRelation*&, long);

////////
DESERT_API bool Remove(CTypedPtrList<CPtrList, CDynFormulaSet*>&, CDynFormulaSet*&, long);
DESERT_API bool Remove(CTypedPtrList<CPtrList, CDynFormula*>&, CDynFormula*&, long);
DESERT_API bool Remove(CTypedPtrList<CPtrList, CDynFormulaSet*>&, CDynFormulaSet*&, const TCHAR*);
DESERT_API bool Remove(CTypedPtrList<CPtrList, CDynFormula*>&, CDynFormula*&, const TCHAR*);
DESERT_API bool Remove(CTypedPtrList<CPtrList, CDynFormulaSet*>&, CDynFormulaSet*);
DESERT_API bool Remove(CTypedPtrList<CPtrList, CDynFormula*>&, CDynFormula*);
/////////

DESERT_API bool Remove(CTypedPtrList<CPtrList, CSpace*>&, CSpace*&, const TCHAR*);
DESERT_API bool Remove(CTypedPtrList<CPtrList, CDomain*>&, CDomain*&, const TCHAR*);
DESERT_API bool Remove(CTypedPtrList<CPtrList, CConstraintSet*>&, CConstraintSet*&, const TCHAR*);
DESERT_API bool Remove(CTypedPtrList<CPtrList, CElement*>&, CElement*&, const TCHAR*);
DESERT_API bool Remove(CTypedPtrList<CPtrList, CProperty*>&, CProperty*&, const TCHAR*);
DESERT_API bool Remove(CTypedPtrList<CPtrList, CRelation*>&, CRelation*&, const TCHAR*);
DESERT_API bool Remove(CTypedPtrList<CPtrList, CConstraint*>&, CConstraint*&, const TCHAR*);
/////
DESERT_API bool Remove(CTypedPtrList<CPtrList, CFormula*>&, CFormula*&, const TCHAR*);
/////
DESERT_API bool Remove(CTypedPtrList<CPtrList, CDynSpace*>&, CDynSpace*&, const TCHAR*);
DESERT_API bool Remove(CTypedPtrList<CPtrList, CDynDomain*>&, CDynDomain*&, const TCHAR*);
DESERT_API bool Remove(CTypedPtrList<CPtrList, CDynConstraintSet*>&, CDynConstraintSet*&, const TCHAR*);
DESERT_API bool Remove(CTypedPtrList<CPtrList, CDynElement*>&, CDynElement*&, const TCHAR*);
DESERT_API bool Remove(CTypedPtrList<CPtrList, CDynProperty*>&, CDynProperty*&, const TCHAR*);
DESERT_API bool Remove(CTypedPtrList<CPtrList, CDynRelation*>&, CDynRelation*&, const TCHAR*);
DESERT_API bool Remove(CTypedPtrList<CPtrList, CDynConstraint*>&, CDynConstraint*&, const TCHAR*);


DESERT_API bool Remove(CTypedPtrList<CPtrList, CSpace*>&, CSpace*);
DESERT_API bool Remove(CTypedPtrList<CPtrList, CDomain*>&, CDomain*);
DESERT_API bool Remove(CTypedPtrList<CPtrList, CConstraintSet*>&, CConstraintSet*);
DESERT_API bool Remove(CTypedPtrList<CPtrList, CElement*>&, CElement*);
DESERT_API bool Remove(CTypedPtrList<CPtrList, CProperty*>&, CProperty*);
DESERT_API bool Remove(CTypedPtrList<CPtrList, CRelation*>&, CRelation*);
DESERT_API bool Remove(CTypedPtrList<CPtrList, CConstraint*>&, CConstraint*);
/////
DESERT_API bool Remove(CTypedPtrList<CPtrList, CFormula*>&, CFormula*);
/////
DESERT_API bool Remove(CTypedPtrList<CPtrList, CDynSpace*>&, CDynSpace*);
DESERT_API bool Remove(CTypedPtrList<CPtrList, CDynDomain*>&, CDynDomain*);
DESERT_API bool Remove(CTypedPtrList<CPtrList, CDynConstraintSet*>&, CDynConstraintSet*);
DESERT_API bool Remove(CTypedPtrList<CPtrList, CDynElement*>&, CDynElement*);
DESERT_API bool Remove(CTypedPtrList<CPtrList, CDynProperty*>&, CDynProperty*);
DESERT_API bool Remove(CTypedPtrList<CPtrList, CDynRelation*>&, CDynRelation*);
DESERT_API bool Remove(CTypedPtrList<CPtrList, CDynConstraint*>&, CDynConstraint*);


DESERT_API bool Insert(CTypedPtrList<CPtrList, CElement*>&, CElement*);
DESERT_API bool Insert(CTypedPtrList<CPtrList, CSpace*>&, CSpace*);
DESERT_API bool Insert(CTypedPtrList<CPtrList, CDomain*>&, CDomain*);
DESERT_API bool Insert(CTypedPtrList<CPtrList, CRelation*>&, CRelation*);
DESERT_API bool Insert(CTypedPtrList<CPtrList, CProperty*>&, CProperty*);
DESERT_API bool Insert(CTypedPtrList<CPtrList, CConstraintSet*>&,  CConstraintSet*);
DESERT_API bool Insert(CTypedPtrList<CPtrList, CConstraint*>&, CConstraint*);
////
DESERT_API bool Insert(CTypedPtrList<CPtrList, CFormulaSet*>&,  CFormulaSet*);
DESERT_API bool Insert(CTypedPtrList<CPtrList, CFormula*>&, CFormula*);
DESERT_API bool Insert(CTypedPtrList<CPtrList, CDynFormulaSet*>&,  CDynFormulaSet*);
DESERT_API bool Insert(CTypedPtrList<CPtrList, CDynFormula*>&, CDynFormula*);
////
DESERT_API bool Insert(CTypedPtrList<CPtrList, CDynElement*>&, CDynElement*);
DESERT_API bool Insert(CTypedPtrList<CPtrList, CDynSpace*>&, CDynSpace*);
DESERT_API bool Insert(CTypedPtrList<CPtrList, CDynDomain*>&, CDynDomain*);
DESERT_API bool Insert(CTypedPtrList<CPtrList, CDynConstraintSet*>&, CDynConstraintSet*);
DESERT_API bool Insert(CTypedPtrList<CPtrList, CDynProperty*>&, CDynProperty*);
DESERT_API bool Insert(CTypedPtrList<CPtrList, CDynRelation*>&, CDynRelation*);
DESERT_API bool Insert(CTypedPtrList<CPtrList, CDynConstraint*>&, CDynConstraint*);

DESERT_API CCosmicList* CloneList(CTypedPtrList<CPtrList, CElement*>&);
DESERT_API CCosmicList* CloneList(CTypedPtrList<CPtrList, CDynElement*>&);
DESERT_API int			Index(CTypedPtrList<CPtrList, CDynElement*>&, CDynElement*);




#endif // #ifndef TEMPL_H
