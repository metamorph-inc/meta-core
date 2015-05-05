// -*- C++ -*-
// cldef.h
// Copyright (c) Vanderbilt University, 1998
// ALL RIGHTS RESERVED
//

#ifndef cldef_h
#define cldef_h

#include "afxwin.h"
#include "afxcoll.h"
#include "afxtempl.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using std::ofstream;
using std::endl;

#include "defs.h"

#define MAX_FUNCTION_PARAM_NUM	16

class ClData;
class ClContext;
class ClBase;
class ClPar;
class ClExpr;
class ClLExpr;
class ClConstraint;
////
class ClFormula;
////
class ClIfExpr;
class ClRelExpr;
class ClAddExpr;
class ClMulExpr;
class ClUnaExpr;
class ClPostfixExpr;
class ClPrimExpr;
class ClCall;
class ClLitColl;
class ClLit;
class ClObjName;
class ClCallPars;
class ClDecl;
class ClActParList;
class ClNumber;
class ClExprlRange;

class CBdd;
class CCosmic;


typedef CTypedPtrList<CPtrList, ClExpr*> ClExprList;
typedef CTypedPtrList<CPtrList, ClRelExpr*> ClRelExprList;
typedef CTypedPtrList<CPtrList, ClPar*>	ClParList;
typedef CTypedPtrList<CPtrList, ClConstraint*> ClConstraintList;
typedef CTypedPtrMap<CMapStringToPtr, CString, ClConstraint *> ClConstraintTable;

class ClBase
{
public:
  ClBase();
  virtual ~ClBase();
};

class ClPar : public ClBase
{
public:
  ClPar(const CString& name, const CString& type);
  std::string getPrintString() const;
  void Print() const;
  void Print(ofstream& fout) const;

private:
  CString name,type;
};

class ClConstraint : public ClBase
{
public:
  static void Init();
  static bool Make(const CString& _name,ClParList* _cpars,ClExpr* _cexpr,
                   const CString& _catName,const CString& _modName,
                   const CString& _aspName,const CString& _actName,
                   const int _priority, const CString& _desc);
  ClConstraint(const CString& _name,ClParList* _cpars,ClExpr* _cexpr,
               const CString& _catName,const CString& _modName,
               const CString& _aspName,const CString& _actName,
               const int _priority,const CString& _desc);
  ~ClConstraint();
  static ClConstraint *GetLastConstraint();
  const CString& Name() const;
  operator const CString() const;

  std::string getPrintString() const;
  void Print() const;
  void Print(ofstream& fout) const;
  ClData Eval(ClContext& context) const;
  ClFormula *Clone();

private:
  ClParList* cpars;
  ClExpr* cexpr;
  CString name;
  CString categoryName, modelName, aspectName;
  CString actionName;
  CString description;
  int priority;
  static ClConstraint *lastConstraint;
};

////
class ClFormula : public  ClBase
{
public:
  static void Init();
  static bool Make(const CString& _name,ClParList* _cpars,ClExpr* _cexpr,
                   const CString& _catName,const CString& _modName,
                   const CString& _aspName,const CString& _actName,
                   const int _priority, const CString& _desc);
  ClFormula(const CString& _name,ClParList* _cpars,ClExpr* _cexpr,
               const CString& _catName,const CString& _modName,
               const CString& _aspName,const CString& _actName,
               const int _priority,const CString& _desc);
  
  static ClFormula *GetLastFormula();
  const CString& Name() const;
  operator const CString() const;

  std::string getPrintString() const;
  void Print() const;
  void Print(ofstream& fout) const;
  ClData Eval(ClContext& context) const;

private:
  ClParList* cpars;
  ClExpr* cexpr;
  CString name;
  CString categoryName, modelName, aspectName;
  CString actionName;
  CString description;
  int priority;
  static ClFormula *lastFormula;
};
////

class ClExpr : public ClBase
{
public:
  ClExpr(ClLExpr* lexpr);
  ~ClExpr();
  std::string getPrintString() const;
  void Print() const;
  void Print(ofstream& fout) const;
  ClData Eval(ClContext& context) const;

private:
  ClLExpr* lexpr;
};

class ClLExpr : public ClBase
{
public:
  typedef enum { noOp = 0, andOp, orOp, xorOp, implOp } LOp;
  ClLExpr(ClRelExpr* _relExpr, LOp _lOp = noOp,ClLExpr* _next = nil);
  ~ClLExpr();
  std::string getPrintString() const;
  void Print() const;
  void Print(ofstream& fout) const;
  ClData Eval(ClContext& context) const;

private:
  ClData Eval(ClContext& context, const ClData& preVal, LOp preOp) const;
  ClData Eval(const ClData& left, LOp op, const ClData& right) const;
  bool Eval(bool left, LOp op, bool right) const;
  CBdd Eval(const CBdd& left, LOp op, const CBdd& right) const;

private:
  ClRelExpr* relExpr;
  LOp lOp;
  static char *opNames[implOp + 1];
  ClLExpr* next;
};

class ClIfExpr : public ClBase
{
public:
  ClIfExpr(ClExpr* _condE, ClExpr* _thenE, ClExpr* _elseE);
  ~ClIfExpr();
  std::string getPrintString() const;
  void Print() const;
  void Print(ofstream& fout) const;
  ClData Eval(ClContext& context) const;

private:
  ClData Eval(const ClData& _if, const ClData& _then, const ClData& _else) const;
  CBdd Eval(const CBdd& _if, const CBdd& _then) const;
  CBdd Eval(const CBdd& _if, const CBdd& _then, const CBdd& _else) const;

private:
  ClExpr* condE;
  ClExpr* thenE;
  ClExpr* elseE;
};

class ClRelExpr : public ClBase
{
public:
  typedef enum { noOp = 0, eqOp, ltOp, gtOp, geOp, leOp, neOp } RelOp;
  ClRelExpr(ClAddExpr* _addExpr, RelOp _relOp = noOp, ClAddExpr* _next = nil);
  ~ClRelExpr();
  std::string getPrintString() const;
  void Print() const;
  void Print(ofstream& fout) const;
  ClData Eval(ClContext& context) const;

private:
  ClData Eval(ClContext& context, const ClData& left, const ClData& right) const;
#ifndef DOUBLE_MTBDD  
  bool Eval(int left, int right) const;
  ClData Eval(ClContext& context, const CCosmic *left, int right) const;
   ClData Eval(ClContext& context, int left, const CCosmic *right) const;
#else
  bool Eval(double left, double right) const;
  ClData Eval(ClContext& context, const CCosmic *left, double right) const;
   ClData Eval(ClContext& context, double left, const CCosmic *right) const;
#endif
  ClData Eval(const CCosmic *left, const CCosmic *right) const;
  ClData Eval(const CBdd& left, const CBdd& right) const;
  ClData Eval(const CBdd& left, double right) const;
  ClData Eval(double left, const CBdd& right) const;
private:
  ClAddExpr* addExpr;
  static char *opNames[neOp + 1];
  RelOp relOp;
  ClAddExpr* next;
};

class ClAddExpr : public ClBase
{
public:
  typedef enum { noOp = 0, plusOp, minusOp } AddOp;
  ClAddExpr(ClMulExpr* _mulExpr, AddOp _addOp = noOp, ClAddExpr* _next = nil);
  ~ClAddExpr();
  std::string getPrintString() const;
  void Print() const;
  void Print(ofstream& fout) const;
  ClData Eval(ClContext& context) const;

private:
  ClData Eval(ClContext& context, const ClData& preVal, AddOp preOp) const;
  ClData Eval(const ClData& left, AddOp op, const ClData& right) const;  
  

  //2/17 addded
  ClData Eval(const CCosmic *left, AddOp op, const CCosmic *right) const;
  ClData Eval(const CBdd& left, AddOp op, const CCosmic *right) const;
  ClData Eval(const CBdd& left, AddOp op, const CBdd& right) const;
#ifndef DOUBLE_MTBDD  
  int Eval(int left, AddOp op, int right) const;
  ClData Eval(const CBdd& left, AddOp op, int right) const;
  ClData Eval(int left, AddOp op, const CBdd& right) const;
#else
  double Eval(double left, AddOp op, double right) const;
  ClData Eval(const CBdd& left, AddOp op, double right) const;
  ClData Eval(double left, AddOp op, const CBdd& right) const;
#endif

private:
  ClMulExpr* mulExpr; 
  AddOp addOp;
  static char *opNames[minusOp + 1];
  ClAddExpr* next;
};

class ClMulExpr : public ClBase
{
public:
  typedef enum { noOp = 0, starOp, perOp } MulOp;
  ClMulExpr(ClUnaExpr* _unaExpr, MulOp _mulOp = noOp, ClMulExpr* _next = nil);
  ~ClMulExpr();
  std::string getPrintString() const;
  void Print() const;
  void Print(ofstream& fout) const;
  ClData Eval(ClContext& context) const;

private:
  ClData Eval(ClContext& context, const ClData& preVal, MulOp preOp) const;
  ClData Eval(const ClData& left, MulOp op, const ClData& right) const;
  

   //2/17 added
  ClData Eval(const CCosmic *left, MulOp op, const CCosmic *right) const;
  //5/11 added
  ClData Eval(const CBdd& left, MulOp op, const CCosmic *right) const;
  ClData Eval(const CBdd& left, MulOp op, const CBdd& right) const;
#ifndef DOUBLE_MTBDD
  int Eval(int left, MulOp op, int right) const; 
  ClData Eval(const CBdd& left, MulOp op, int right) const;
  ClData Eval(int left, MulOp op, const CBdd& right) const;
#else
  double Eval(double left, MulOp op, double right) const;
  ClData Eval(const CBdd& left, MulOp op, double right) const;
  ClData Eval(double left, MulOp op, const CBdd& right) const;
#endif

private:
  ClUnaExpr* unaExpr;
  MulOp mulOp;
  static char *opNames[perOp + 1];
  ClMulExpr* next;
};

class ClUnaExpr : public ClBase
{
public:
  typedef enum { noOp = 0, minOp, notOp, sinOp, cosOp, tanOp, asinOp, acosOp, atanOp, sinhOp, coshOp, tanhOp, asinhOp, acoshOp, atanhOp, log2Op, log10Op, lnOp, expOp, sqrtOp, signOp, rintOp, absOp } UnaOp;
  ClUnaExpr(ClPostfixExpr * _postfixExpr, UnaOp _unaOp = noOp);
  ~ClUnaExpr();
  std::string getPrintString() const;
  void Print() const;
  void Print(ofstream& fout) const;
  ClData Eval(ClContext& context) const;

private:
  ClData Eval(const ClData& right) const;
  double Eval(int right) const;
  double Eval(double right) const;
  bool Eval(bool right) const;
  CBdd Eval(const CBdd& right) const;
  double applyUnaryMathFunc(double right) const;

private:
  ClPostfixExpr* postfixExpr;
  UnaOp unaOp;
  static char *opNames[absOp + 1];
};

class ClPostfixCall : public ClBase
{
public:
  typedef enum { noOp = 0, objCallOp, stdCallOp, collCallOp } CallOp;
  ClPostfixCall(CallOp _callOp, ClCall* _call, ClPostfixCall* _next = nil);
  ~ClPostfixCall();
  std::string getPrintString() const;
  void Print() const;
  void Print(ofstream& fout) const;
  ClData Eval(ClContext& context) const;

private:
  CallOp callOp;
  static char *opNames[collCallOp + 1];
  ClCall* call;
  ClPostfixCall* next;
};

class ClPostfixExpr : public ClBase
{
public:
  ClPostfixExpr(ClPrimExpr* _primExpr, ClPostfixCall* _call = nil);
  ~ClPostfixExpr();
  std::string getPrintString() const;
  void Print() const;
  void Print(ofstream& fout) const;
  ClData Eval(ClContext& context) const;

private:
  ClPrimExpr *primExpr;
  ClPostfixCall* call;
};

class ClPrimExpr : public ClBase
{
public:
  ClPrimExpr(ClLitColl* _litColl);
  ClPrimExpr(ClLit* _lit);
  ClPrimExpr(ClObjName* _objName, ClCallPars* _callPars);
  ClPrimExpr(ClExpr* _expr);
  ClPrimExpr(ClIfExpr* _ifExpr);
  ~ClPrimExpr();
  std::string getPrintString() const;
  void Print() const;
  void Print(ofstream& fout) const;
  ClData Eval(ClContext& context) const;

private:
  typedef enum { litCollType, litType, objType, objCallType, exprType, ifExprType } PrimExprType;
  PrimExprType type;
  union
  {
    ClLitColl* litColl;
    ClLit *lit;
    struct { ClObjName* objName; ClCallPars* callPars; } objCall;
    ClExpr *expr;
    ClIfExpr *ifExpr;
  } expr;
};

class ClCallPars : public ClBase
{
public:
  ClCallPars(ClDecl* _cDecl,ClActParList* _actParList);
  ~ClCallPars();
  ClDecl* GetDeclarations();
  ClExprList *GetExpressions();
  std::string getPrintString() const;
  void Print() const;
  void Print(ofstream& fout) const;

private:
  ClDecl* cDecl;
  ClActParList* actParList;
};

class ClLit : public ClBase
{
public:
  ClLit(const CString& _str);
  ClLit(ClNumber* _num);
  ~ClLit();
  std::string getPrintString() const;
  void Print() const;
  void Print(ofstream& fout) const;
  ClData Eval(ClContext& context) const;

private:
  typedef enum { strType, numType } LitType;
  LitType type;
  CString str;
  ClNumber* num;
};

class ClType : public ClBase
{
public:
  ClType(const CString& _typeName);
  ~ClType() { }
  std::string getPrintString() const;
  void Print() const;
  void Print(ofstream& fout) const;

private:
  CString typeName;
};

class ClLitColl : public ClBase
{
public:
  typedef enum { setColl, bagColl, seqColl, collColl } CollType;
  ClLitColl(CollType _type,ClExprlRange* _range);
  ~ClLitColl();
  std::string getPrintString() const;
  void Print() const;
  void Print(ofstream& fout) const;

private:
  CollType type;
  static char *typeNames[collColl + 1];
  ClExprlRange* range;
};

class ClExprlRange : public ClBase
{
public:
  ClExprlRange(ClExpr* _from);
  ClExprlRange(ClExpr* _from,ClExpr* _to);
  ClExprlRange(ClExpr* _from,ClExprList* _elems);
  ~ClExprlRange();
  std::string getPrintString() const;
  void Print() const;
  void Print(ofstream& fout) const;

private:
  ClExpr* from;
  ClExpr* to;
  ClExprList* elems;
};

class ClCall : public ClBase
{
public:
  ClCall(const CString& _name,ClCallPars* _pars);
  ~ClCall();
  std::string getPrintString() const;
  void Print() const;
  void Print(ofstream& fout) const;
  ClData Eval(ClContext& context) const;

private:
  CString name;
  ClCallPars *pars;
};

class ClDecl : public ClBase
{
public:
  ClDecl(CStringList* _vars,ClType* _type);
  ~ClDecl();
  CStringList* GetVariables();
  ClType* GetType();
  std::string getPrintString() const;
  void Print() const;
  void Print(ofstream& fout) const;

private:
  CStringList* vars;
  ClType* type;
};

class ClObjName : public ClBase
{
public:
  ClObjName();
  ClObjName(const CString& _objName);
  ~ClObjName();
  CString &GetName();
  bool IsSelf() const;
  std::string getPrintString() const;
  void Print() const;
  void Print(ofstream& fout) const;
  ClData Eval(ClContext& context) const;
  ClData Eval(ClContext& context, ClCallPars* pars) const;

private:
  CString objName;
  bool selfP;
};

class ClActParList : public ClBase
{
public:
  ClActParList(ClExprList* _exprList);
  ~ClActParList();
  ClExprList *GetExpressions();
  std::string getPrintString() const;
  void Print() const;
  void Print(ofstream& fout) const;

private:
  ClExprList* exprList;
};

class ClNumber : public ClBase
{
public:
  ClNumber(int i);
  ClNumber(double d);
  ~ClNumber();
  std::string getPrintString() const;
  void Print() const;
  void Print(ofstream& fout) const;
  ClData Eval(ClContext& context) const;

private:
  typedef enum { intType, doubleType } ValueType;
  ValueType type;
  union
  {
    int i;
    double d;
  } value;
};

inline
ClBase::
ClBase()
{
}

inline
ClBase::
~ClBase()
{
}

inline
ClPar::
ClPar(const CString& n, const CString& t) : name(n), type(t)
{
}

inline bool
ClConstraint::
Make(const CString& _name,ClParList* _cpars,ClExpr* _cexpr,
     const CString& _catName,const CString& _modName,
     const CString& _aspName,const CString& _actName,
     const int _priority, const CString& _desc)
{
  lastConstraint = new ClConstraint(_name, _cpars, _cexpr, _catName, _modName, _aspName,
                                    _actName, _priority, _desc);
  return (lastConstraint != nil);
}

inline
ClConstraint::
ClConstraint(const CString& _name,ClParList* _cpars,ClExpr* _cexpr,
             const CString& _catName,const CString& _modName,
             const CString& _aspName,const CString& _actName,
             const int _priority,const CString& _desc) :
  name(_name), cpars(_cpars), cexpr(_cexpr), categoryName(_catName),
  modelName(_modName), aspectName(_aspName), actionName(_actName),
  priority(_priority), description(_desc)
{
}

inline const CString&
ClConstraint::
Name() const
{
  return name;
}

inline
ClConstraint::
operator const CString() const
{
  return name;
}

inline ClConstraint*
ClConstraint::
GetLastConstraint()
{
  return lastConstraint;
}

inline ClFormula *
ClConstraint::
Clone()
{
	ClFormula *formula = new ClFormula(name, cpars, cexpr, categoryName, modelName, aspectName, actionName, priority, description);
	return formula;
}

inline
ClExpr::
ClExpr(ClLExpr* l) : lexpr(l)
{
}

inline
ClLExpr::
ClLExpr(ClRelExpr* _relExpr, LOp _lOp,ClLExpr* _next) 
    : relExpr(_relExpr), lOp(_lOp), next(_next)
{
}

inline
ClIfExpr::
ClIfExpr(ClExpr* _condE, ClExpr* _thenE, ClExpr* _elseE)
  : condE(_condE), thenE(_thenE), elseE(_elseE)
{
}

inline
ClRelExpr::
ClRelExpr(ClAddExpr* _addExpr, RelOp _relOp, ClAddExpr* _next)
    : addExpr(_addExpr), relOp(_relOp), next(_next)
{
}

inline
ClAddExpr::
ClAddExpr(ClMulExpr* _mulExpr, AddOp _addOp, ClAddExpr* _next)
  : mulExpr(_mulExpr), addOp(_addOp), next(_next)
{
}

inline
ClMulExpr::
ClMulExpr(ClUnaExpr* _unaExpr, MulOp _mulOp, ClMulExpr* _next)
  : unaExpr(_unaExpr), mulOp(_mulOp), next(_next)
{
}

inline
ClUnaExpr::
ClUnaExpr(ClPostfixExpr * _postfixExpr, UnaOp _unaOp)
  : postfixExpr(_postfixExpr), unaOp(_unaOp)
{
}

inline
ClPostfixCall::
ClPostfixCall(CallOp _callOp, ClCall* _call, ClPostfixCall* _next)
  : callOp(_callOp), call(_call), next(_next)
{
}

inline
ClPostfixExpr::
ClPostfixExpr(ClPrimExpr* _primExpr, ClPostfixCall* _call)
  : primExpr(_primExpr), call(_call)
{
}

inline
ClPrimExpr::
ClPrimExpr(ClLitColl* _litColl)
{
  type = litCollType;
  expr.litColl = _litColl;
}

inline
ClPrimExpr::
ClPrimExpr(ClLit* _lit)
{
  type = litType;
  expr.lit = _lit;
}

inline
ClPrimExpr::
ClPrimExpr(ClObjName* _objName, ClCallPars* _callPars)
{
  type = _callPars ? objCallType : objType;
  expr.objCall.objName = _objName; expr.objCall.callPars = _callPars;
}

inline
ClPrimExpr::
ClPrimExpr(ClExpr* _expr)
{
  type = exprType;
  expr.expr = _expr;
}

inline
ClPrimExpr::
ClPrimExpr(ClIfExpr* _ifExpr)
{
  type = ifExprType;
  expr.ifExpr = _ifExpr;
}

inline
ClCallPars::
ClCallPars(ClDecl* _cDecl,ClActParList* _actParList)
{
  cDecl = _cDecl;
  actParList = _actParList;
}

inline ClDecl*
ClCallPars::
GetDeclarations()
{
  return cDecl;
}

inline
ClLit::
ClLit(const CString& _str)
{
  type = strType;
  str = _str;
  num = nil;
}

inline
ClLit::  
ClLit(ClNumber* _num)
{
  type = numType;
  num = _num;
}

inline
ClType::
ClType(const CString& _typeName) : typeName(_typeName)
{
}

inline
ClLitColl::
ClLitColl(CollType _type,ClExprlRange* _range)
{
  type = _type;
  range = _range;
}

inline
ClExprlRange::
ClExprlRange(ClExpr* _from)
{
  from = _from;
  to = nil;
  elems = nil;
}

inline
ClExprlRange::
ClExprlRange(ClExpr* _from,ClExpr* _to)
{
  from = _from;
  to = _to;
  elems = nil;
}

inline
ClExprlRange::
ClExprlRange(ClExpr* _from,ClExprList* _elems)
{
  from = _from;
  to = nil;
  elems = _elems;
}

inline
ClCall::
ClCall(const CString& _name,ClCallPars* _pars)
  : name(_name), pars(_pars)
{
}

inline
ClDecl::
ClDecl(CStringList* _vars,ClType* _type)
{
  vars = _vars;
  type = _type;
}

inline CStringList*
ClDecl::
GetVariables()
{
  return vars;
}

inline ClType*
ClDecl::
GetType()
{
  return type;
}

inline
ClObjName::
ClObjName()
{
  selfP = true;
}

inline
ClObjName::
ClObjName(const CString& _objName) : objName(_objName)
{
  selfP = false;
}

inline
ClObjName::
~ClObjName()
{
}

inline CString &
ClObjName::
GetName()
{
  return objName;		// tbd skn - what about self?
}

inline bool
ClObjName::
IsSelf() const
{
  return selfP;
}

inline
ClActParList::
ClActParList(ClExprList* _exprList) : exprList(_exprList)
{
} 

inline
ClNumber::
ClNumber(int i)
{
  type = intType;
  value.i = i;
}

inline
ClNumber::
ClNumber(double d)
{
  type = doubleType;
  value.d = d;
}

inline
ClNumber::
~ClNumber()
{
}


inline bool
ClFormula::
Make(const CString& _name,ClParList* _cpars,ClExpr* _cexpr,
     const CString& _catName,const CString& _modName,
     const CString& _aspName,const CString& _actName,
     const int _priority, const CString& _desc)
{
  lastFormula = new ClFormula(_name, _cpars, _cexpr, _catName, _modName, _aspName,
                                    _actName, _priority, _desc);
  return (lastFormula != nil);
}

inline
ClFormula::
ClFormula(const CString& _name,ClParList* _cpars,ClExpr* _cexpr,
             const CString& _catName,const CString& _modName,
             const CString& _aspName,const CString& _actName,
             const int _priority,const CString& _desc) :
  name(_name), cpars(_cpars), cexpr(_cexpr), categoryName(_catName),
  modelName(_modName), aspectName(_aspName), actionName(_actName),
  priority(_priority), description(_desc)
{
}

inline const CString&
ClFormula::
Name() const
{
  return name;
}

inline
ClFormula::
operator const CString() const
{
  return name;
}

inline ClFormula*
ClFormula::
GetLastFormula()
{
  return lastFormula;
}

#endif // #ifndef cldef_h
