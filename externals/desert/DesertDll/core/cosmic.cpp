// -*-C++-*-
// cosmic.cpp



#include "stdafx.h"
#include "desert.h"
#include "CSetErrDialog.h"
#include "core/cosmic.h"

long CCosmic::CCosmicCount = 0;



CCosmic *
CCosmic::
Project()
{
  ASSERT_EX( CManager::theInstance, _T("CCosmic::Project"), _T("manager not instantiated") );
  return (CCosmic *)CManager::theInstance;
}

CCosmic *
CCosmic::
SystemMode()
{
  ASSERT_EX( CManager::theInstance, _T("CCosmic::Project"), _T("manager not instantiated") );
  return (CCosmic *)CManager::theInstance;
}

// inline functions

inline
CCosmic::
CCosmic() : type(typeCosmic)
{
  id = CCosmicCount++;
  // Info( _T("CCosmic::CCosmic()"), _T("Object %d coming up"), id);
}

inline
CCosmic::
~CCosmic()
{
  // Info( _T("CCosmic::~CCosmic()"), _T("Object %d going down"), id);
}

inline
CCosmic::
operator CType() const
{
  return type;
}

inline CType
CCosmic::
GetType() const
{
  return type;
}

inline bool
CCosmic::
IsType(CType t) const
{
  return t==type;
}

inline
CCosmic::
operator long() const
{
  return id;
}

inline long
CCosmic::
GetID() const
{
  return id;
}


inline
CCosmic::
operator const TCHAR*() const
{
  return _T("");
}

inline CCosmic *
CCosmic::
Parent()
{
  return NULL;
}

inline CCosmic *
CCosmic::
Space()
{
  return NULL;
}

inline CCosmic *
CCosmic::
Domain()
{
  return NULL;
}

inline CCosmicList *
CCosmic::
Children()
{
  return NULL;
}
/*inline CCosmicList *
CCosmic::
Resources()
{
  return NULL;
}

inline CCosmicList *
CCosmic::
Modes()
{
  return NULL;
}

inline CCosmicList *
CCosmic::
Processes()
{
  return NULL;
}*/

inline ClData
CCosmic::
Eval(const CCosmic*) const
{
  return ClData(false);
}

inline ClData
CCosmic::
Eval(ClRelExpr::RelOp, int) const
{
  return ClData(false);
}

inline ClData
CCosmic::
Eval(ClRelExpr::RelOp, double) const
{
  return ClData(false);
}

inline ClData
CCosmic::
Eval(ClRelExpr::RelOp, const CCosmic*) const
{
  return ClData(false);
}

inline bool
CCosmic::
IsNumericFunc() const
{
  return false;
}

inline ClData
CCosmic::
Eval() const
{
  return ClData((CCosmic *) this);
}

inline ClData
CCosmic::
Eval(ClAddExpr::AddOp, const CCosmic*) const
{
  return ClData((CCosmic *) this);
}

inline ClData
CCosmic::
Eval(ClAddExpr::AddOp, const CBdd&) const
{
  return ClData((CCosmic *) this);
}

inline ClData
CCosmic::
Eval(ClMulExpr::MulOp, const CCosmic*) const
{
  return ClData((CCosmic *) this);
}

inline ClData
CCosmic::
Eval(ClMulExpr::MulOp, const CBdd&) const
{
  return ClData((CCosmic *) this);
}

inline void
CCosmic::
SetFunction(FunctionType)
{
}

inline void
CCosmic::
SetFunction(FunctionType, const CString&)
{
}
