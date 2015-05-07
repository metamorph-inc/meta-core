// -*-C++-*-
// cosmic.h
// Declaration of cosmic classes and global functions

#include "core/coredefs.h"
#include "core/clmain.h"
#include "BackIface.h"
#include "core/manager.h"

#ifndef COSMIC_H
#define COSMIC_H


class DESERT_API CCosmic {
  static long CCosmicCount;

protected:
  CType type;
  long id;

protected:
  CCosmic();

public:
  virtual ~CCosmic();

public:
  operator CType() const;
  CType GetType() const;
  bool IsType(CType type) const;
  operator long() const;
  long GetID() const;
  
public:
  virtual operator const TCHAR*() const;
  CCosmic* Project();
  CCosmic* SystemMode();
  virtual CCosmic* Parent();
  virtual CCosmic* Space();
  virtual CCosmic* Domain();
  virtual CCosmicList* Children();
  //These will be obsolated
  //virtual CCosmicList* Resources();
  //virtual CCosmicList* Modes();
  //virtual CCosmicList* Processes();
  //This generic function will be used
  virtual CCosmicList* GetContainer(const TCHAR *) {return NULL;};
  virtual CCosmicList* GetContainer(CString &) {return NULL;};

  virtual ClData Eval(const CCosmic*) const;
  virtual ClData Eval(ClRelExpr::RelOp, int) const;
  virtual ClData Eval(ClRelExpr::RelOp, const CCosmic*) const;
  virtual ClData Eval(ClRelExpr::RelOp, double) const;
  virtual ClData Eval(ClAddExpr::AddOp, const CCosmic*) const;
  virtual ClData Eval(ClAddExpr::AddOp, const CBdd&) const;
  virtual ClData Eval(ClMulExpr::MulOp, const CCosmic*) const;
  virtual ClData Eval(ClMulExpr::MulOp, const CBdd&) const;
  virtual ClData Eval() const;
  virtual bool IsNumericFunc() const;
  
  virtual void SetFunction(FunctionType);
  virtual void SetFunction(FunctionType, const CString&);
};


#endif // ifndef COSMIC_H
