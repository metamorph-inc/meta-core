// -*-C++-*-
// clmain.h
// Constraint class declarations

#ifndef CLMAIN_H
#define CLMAIN_H

#include "core/coredefs.h"
#include "core/bddmain.h"
#include "mcl/cldef.h"

typedef CTypedPtrMap<CMapStringToPtr, CString, ClFunction*> ClFunctionTable;

typedef ClData (ClFunction::*ClFnPtr)(ClContext& context, ClCallPars* params);

class ClData
{
public:
  ClData();
  ClData(const ClData& data);
  ClData(int i);
  ClData(double d);
  ClData(const CString &s);
  ClData(bool b);
  ClData(CCosmic *c);
  ClData(CCosmicList *cl);
  ClData(const CBdd& bdd);
  void Set(int i);
  void Set(double d);
  void Set(const CString &s);
  void Set(bool b);
  void Set(CCosmic *c);
  void Set(CCosmicList *cl);
  void Set(const CBdd &bd);
  bool Get(int &i);
  bool Get(double &d);
  bool Get(CString &s);
  bool Get(bool &b);
  bool Get(CCosmic *&c);
  bool Get(CCosmicList *&cl);
  bool Get(CBdd &bd);
  const ClData& operator= (const ClData& data);
  const ClData& operator= (int i);
  const ClData& operator= (double d);
  const ClData& operator= (const CString &s);
  const ClData& operator= (bool b);
  const ClData& operator= (CCosmic *c);
  const ClData& operator= (CCosmicList *cl);
  const ClData& operator= (const CBdd &bd);
  operator int() const;
  operator bool() const;
  operator double() const;
  operator const CString&() const;
  operator CCosmic*();
  operator const CCosmic*() const;
  operator CCosmicList*();
  operator const CCosmicList*() const;
  operator const CBdd&() const;
  ClDataType GetType() const;
  operator ClDataType() const;

private:
  ClDataType type;
  int ival;
  double dval;
  CString sval;
  bool bval;
  CCosmic *cval;
  CCosmicList *clval;
  CBdd bdval;
};

class ClContext
{
  friend class ClFunction;
public:
  ClContext();
  ClContext(const ClContext& c);
  ClContext(CCosmic *c);
  ClContext(CCosmicList *cl);
  virtual ~ClContext();
  void Set(CCosmic *c);
  void Set(CCosmicList *cl);
  bool Get(CCosmic *&c);
  bool Get(CCosmicList *&cl);
  const ClContext& operator= (const ClContext& ctx);
  const ClContext& operator= (CCosmic *c);
  const ClContext& operator= (CCosmicList *cl);
  operator CCosmic*();
  operator CCosmicList*();
  ClDataType GetType() const;
  operator ClDataType() const;

public:
  ClData Eval();		// for self objects
  ClData Eval(const CString& name);
  CBdd Eval(ClConstraint *constraint);
  CBdd Eval(ClFormula *formula);
  bool Verify(ClConstraint *constraint, CCosmicList *dependency, CStringList *pdep, TCHAR*& errString);
  const TCHAR *GetErrorString() const;
  bool GetError();
  void SetError(bool err=true);
  void SetError(bool err, const TCHAR* errString);
  CCosmic *UpdateDependency(CCosmic *obj);
  CCosmicList *UpdateDependency(CCosmicList *list);
  CCosmic *UpdatePropertyDependency(CCosmic *obj, const TCHAR *prop);

private:
  CCosmic *cosmic;
  CCosmicList *cosmicList;
  CCosmicList *dependency;
  CStringList *propertyDependency;
  ClDataType type;
  bool errFlag;
  TCHAR *errString;
  static CCosmicList dependencyList;
};

class ClFunction
{
public:
  ClFunction(const CString &nm, ClFnPtr ptr);
  ~ClFunction();
  static void CreatePCMFn(const CString& prop, const CString& pcm_fn);
  static void CreateContainer(const CString& name);
  static void Create(const CString& nm, ClFnPtr ptr);
  static void CreateAll();
  static void DestroyAll();
  static ClFunction *Find(const CString &which);

  ClData Eval(ClContext &context, ClCallPars *pars);

private:

	/*OCL Extensions  and/or/leaf tree - specific*/
	ClData Parent(ClContext& context, ClCallPars *pars);
	ClData Project(ClContext& context, ClCallPars *pars);
	ClData ImplementedBy(ClContext& context, ClCallPars *pars);
	ClData Children(ClContext& context, ClCallPars *pars);

	/* Generic Container, which will handle 
		expressions like
		project().custom_domain_name() or
		project().space_name()..
		*/
	ClData Container(ClContext& context, ClCallPars *pars);


	/* Property composing mode(PCM) custom functions */
	ClData Latency(ClContext& context, ClCallPars *pars);

	/*PCM standard functions */
	ClData PCMAdd(ClContext& context, ClCallPars *pars);
	ClData PCMMul(ClContext& context, ClCallPars *pars);
	ClData PCMAmed(ClContext& context, ClCallPars *pars);
	ClData PCMGmed(ClContext& context, ClCallPars *pars);
	ClData PCMMin(ClContext& context, ClCallPars *pars);
	ClData PCMMax(ClContext& context, ClCallPars *pars);
	ClData PCMOr(ClContext& context, ClCallPars *pars);
	//the non-composable properties would get this type
	//for example:
	//self.resource() = project().resources()...
	ClData PCMNone(ClContext& context, ClCallPars *pars);
	ClData PCMNop(ClContext& context, ClCallPars *pars);
	ClData PCMCust(ClContext& context, ClCallPars *pars);

private:
  CString name;
  ClFnPtr fnPtr;
  static ClFunctionTable fnTable;
};

// Constraint class inline method implementation

inline
ClData::
ClData() : type(Undefined)
{
  ival = 0; dval = 0; sval = _T("");  bval = false;  cval = 0; clval = 0;
}

inline
ClData::
ClData(const ClData& data)
{
  type = data.type;
  ival = data.ival;
  dval = data.dval;
  sval = data.sval;
  bval = data.bval;
  cval = data.cval;
  clval = data.clval;
  bdval = data.bdval;
}

inline
ClData::
ClData(int v) : type(Integer), ival(v)
{
  dval = 0; sval = _T("");  bval = false;  cval = 0; clval = 0;
}

inline
ClData::
ClData(double v) : type(Double), dval(v)
{
  ival = 0; sval = _T("");  bval = false;  cval = 0; clval = 0;
}

inline
ClData::
ClData(const CString& v) : type(String), sval(v)
{
  ival = 0; dval = 0;  bval = false;  cval = 0; clval = 0;
}

inline
ClData::
ClData(bool v) : type(Boolean), bval(v)
{
  ival = 0; dval = 0;  sval = _T("");  cval = 0; clval = 0;
}

inline
ClData::
ClData(CCosmic *v) : type(Cosmic), cval(v)
{
  ival = 0; dval = 0;  sval = _T("");  bval = false; clval = 0;
}

inline
ClData::
ClData(CCosmicList *v) : type(CosmicList), clval(v)
{
  ival = 0; dval = 0;  sval = _T("");  bval = false; cval = 0;
}

inline
ClData::
ClData(const CBdd& v) : type(Bdd), bdval(v)
{
  ival = 0; dval = 0;  sval = _T("");  bval = false; cval = 0;
}

inline void
ClData::
Set(int v)
{
  type = Integer;
  ival = v;
}

inline void
ClData::
Set(double v)
{
  type = Double;
  dval = v;
}

inline void
ClData::
Set(const CString& v)
{
  type = String;
  sval = v;
}

inline void
ClData::
Set(bool v)
{
  type = Boolean;
  bval = v;
}

inline void
ClData::
Set(CCosmic *v)
{
  type = Cosmic;
  cval = v;
}

inline void
ClData::
Set(CCosmicList *v)
{
  type = CosmicList;
  clval = v;
}

inline void
ClData::
Set(const CBdd& v)
{
  type = Bdd;
  bdval = v;
}

inline bool
ClData::
Get(int& v)
{
  v = ival;
  return type == Integer;
}

inline bool
ClData::
Get(double& v)
{
  v = dval;
  return type == Double;
}

inline bool
ClData::
Get(CString& v)
{
  v = sval;
  return type == String;
}

inline bool
ClData::
Get(bool& v)
{
  v = bval;
  return type == Boolean;
}

inline bool
ClData::
Get(CCosmic*& v)
{
  v = cval;
  return type == Cosmic;
}

inline bool
ClData::
Get(CCosmicList*& v)
{
  v = clval;
  return type == CosmicList;
}

inline bool
ClData::
Get(CBdd& v)
{
  v = bdval;
  return type == Bdd;
}

inline const ClData&
ClData::
operator= (const ClData& data)
{
  type = data.type;
  ival = data.ival;
  dval = data.dval;
  sval = data.sval;
  bval = data.bval;
  cval = data.cval;
  clval = data.clval;
  bdval = data.bdval;
  return *this;
}

inline const ClData&
ClData::
operator= (int v)
{
  type = Integer;
  ival = v;
  return *this;
}

inline const ClData&
ClData::
operator= (double v)
{
  type = Double;
  dval = v;
  return *this;
}

inline const ClData&
ClData::
operator= (const CString& v)
{
  type = String;
  sval = v;
  return *this;
}

inline const ClData&
ClData::
operator= (bool v)
{
  type = Boolean;
  bval = v;
  return *this;
}

inline const ClData&
ClData::
operator= (CCosmic *v)
{
  type = Cosmic;
  cval = v;
  return *this;
}

inline const ClData&
ClData::
operator= (CCosmicList *v)
{
  type = CosmicList;
  clval = v;
  return *this;
}

inline const ClData&
ClData::
operator= (const CBdd& v)
{
  type = Bdd;
  bdval = v;
  return *this;
}

inline
ClData::
operator int() const
{
  ASSERT_EX( type == Integer, _T("ClData::operator int()"), _T("type mismatch") );
  return ival;
}

inline
ClData::
operator double() const
{
  ASSERT_EX( type == Double, _T("ClData::operator double()"), _T("type mismatch") );
  return dval;
}

inline
ClData::
operator bool() const
{
  ASSERT_EX( type == Boolean, _T("ClData::operator bool()"), _T("type mismatch") );
  return bval;
}

inline
ClData::
operator const CString&() const
{
  ASSERT_EX( type == String, _T("ClData::operator const CString&()"), _T("type mismatch") );
  return sval;
}

inline
ClData::
operator CCosmic*()
{
  ASSERT_EX( type == Cosmic, _T("ClData::operator CCosmic*()"), _T("type mismatch") );
  return cval;
}

inline
ClData::
operator const CCosmic*() const
{
  ASSERT_EX( type == Cosmic, _T("ClData::operator CCosmic*()"), _T("type mismatch") );
  return cval;
}

inline
ClData::
operator CCosmicList*()
{
  ASSERT_EX( type == CosmicList, _T("ClData::operator CCosmicList*()"), _T("type mismatch") );
  return clval;
}

inline
ClData::
operator const CCosmicList*() const
{
  ASSERT_EX( type == CosmicList, _T("ClData::operator CCosmicList*()"), _T("type mismatch") );
  return clval;
}

inline
ClData::
operator const CBdd&() const
{
  ASSERT_EX( type == Bdd, _T("ClData::operator const CBdd&()"), _T("type mismatch") );
  return bdval;
}

inline ClDataType
ClData::
GetType() const
{
  return type;
}

inline
ClData::
operator ClDataType() const
{
  return type;
}

inline
ClContext::
~ClContext()
{
  free(errString);
}

inline void
ClContext::
Set(CCosmic *e)
{
  type = Cosmic;
  cosmic = e;
}

inline void
ClContext::
Set(CCosmicList *e)
{
  type = CosmicList;
  cosmicList = e;
}

inline bool
ClContext::
Get(CCosmic*& e)
{
  e = cosmic;
  return type == Cosmic;
}

inline bool
ClContext::
Get(CCosmicList*& e)
{
  e = cosmicList;
  return type == CosmicList;
}

inline const ClContext&
ClContext::
operator= (const ClContext& from)
{
  type = from.type;
  cosmic = from.cosmic;
  cosmicList = from.cosmicList;
  errFlag = from.errFlag;
  dependency = from.dependency;
  return *this;
}

inline const ClContext&
ClContext::
operator= (CCosmic *c)
{
  cosmic = c;
  type = Cosmic;
  return *this;
}

inline const ClContext&
ClContext::
operator= (CCosmicList *c)
{
  cosmicList = c;
  type = CosmicList;
  return *this;
}

inline
ClContext::
operator CCosmic*()
{
  ASSERT_EX( type == Cosmic, _T("ClContext::operator CCosmic*()"), _T("type mismatch") );
  return cosmic;
}

inline
ClContext::
operator CCosmicList*()
{
  ASSERT_EX( type == Cosmic, _T("ClContext::operator CCosmic*()"), _T("type mismatch") );
  return cosmicList;
}

inline ClDataType
ClContext::
GetType() const
{
  return type;
}

inline
ClContext::
operator ClDataType() const
{
  return type;
}

inline const TCHAR *
ClContext::
GetErrorString() const
{
  return errString;
}

inline bool
ClContext::
GetError()
{
  return errFlag;
}

inline void
ClContext::
SetError(bool err /* = true */)
{
  errFlag = err;
}

inline void
ClContext::
SetError(bool err, const TCHAR *string)
{
  errFlag = err;
  if (errString) free(errString);
  errString = _tcsdup(string);
}

inline
ClFunction::
ClFunction(const CString& n, ClFnPtr p) : name(n), fnPtr(p)
{
}

inline
ClFunction::
~ClFunction()
{
}

inline void
ClFunction::
Create(const CString &n, ClFnPtr p)
{
  fnTable[n] = new ClFunction(n, p);
}

inline ClData
ClFunction::
Eval(ClContext& c, ClCallPars *p)
{
  return (this->*fnPtr)(c, p);
}

inline ClFunction *
ClFunction::
Find(const CString& n)
{
  ClFunction *p=0;
  return fnTable.Lookup(n, p) ? p : 0;
}



//macros

#define SWITCH_CONTEXT(to, from) \
do { \
  switch((ClDataType)from) { \
  case Cosmic: \
    to=(CCosmic *)from; \
    break; \
  case CosmicList: \
    to=(CCosmicList *)from; \
    break; \
  default: \
    throw new CDesertException(_T("SWITCH_CONTEXT: no switch for from!"));\
    /*Warning(_T("SWITCH_CONTEXT"), _T("<%s:%d> no switch"), __FILE__, __LINE__);*/ \
  } \
} while(0);

#define RESTORE_CONTEXT(to, from) \
do { \
  to.SetError(from.GetError(), from.GetErrorString()); \
} while(0);

#define CHECK_PARS_NODECL_ISEMPTY(context, pars, bret) \
do { \
  bret = bret && pars; \
  if (bret) { \
    ClExprList *exprs = pars ? pars->GetExpressions() : 0;\
    ClDecl *decl = pars ? pars->GetDeclarations() : 0;\
    if (bret = ((exprs && exprs->IsEmpty() || !exprs) && !decl)) break; \
  } \
  /*context.SetError(true, _T("function argument error")); */\
  throw new CDesertException(false, _T("CHECK_PARS_NODECL_ISEMPTY: function argument error: expected no arguments!"));\
} while(0);

#define CHECK_PARS_NODECL_HASONEORNONE(context, pars, expr, bret) \
do { \
  bret = bret && pars; \
  if (bret) { \
    ClExprList *exprs = pars->GetExpressions(); \
    ClDecl *decl = pars->GetDeclarations(); \
    expr = exprs && exprs->GetCount()>0 ? exprs->GetHead() : 0; \
    if (bret = ((exprs && exprs->GetCount()<=1 || !exprs) && !decl)) break; \
  } \
  /*context.SetError(true, _T("function argument error")); */\
  throw new CDesertException(false, _T("CHECK_PARS_NODECL_HASONEORNONE: function argument error: expected one or none arguments!"));\
} while(0);

#define CHECK_PARS_NODECL_HASONE(context, pars, expr, bret) \
do { \
  bret = bret && pars; \
  if (bret) { \
    ClExprList *exprs = pars->GetExpressions(); \
    ClDecl *decl = pars->GetDeclarations(); \
    expr = exprs && exprs->GetCount()>0 ? exprs->GetHead() : 0; \
    if (bret = ((exprs && exprs->GetCount()==1 || !exprs) && !decl)) break; \
  } \
  /*context.SetError(true, _T("function argument error"));*/ \
  throw new CDesertException(false, _T("CHECK_PARS_NODECL_HASONE: function argument error: expected one argument!"));\
} while(0);

#define PREPARE_OBJ_CALL(context, obj, ret) \
do { \
  ret = ret && (ClDataType)context == Cosmic; \
  obj = ret ? (CCosmic *)context : 0; \
  if (ret && obj) break; \
  /*context.SetError(true, _T("invalid context")); */\
  throw new CDesertException(true, _T("PREPARE_OBJ_CALL: invalid context!"));\
} while(0);

#endif // #ifndef CLMAIN_H
