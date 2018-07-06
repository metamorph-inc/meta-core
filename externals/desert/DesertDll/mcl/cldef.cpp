// -*- C++ -*-
// cldef.C
// Copyright (c) Vanderbilt University, 1998
// ALL RIGHTS RESERVED
#include <ctype.h>

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
#include "cldef.h"

template<class T> 
void destroy(CTypedPtrList<CPtrList,T>& list)
{
  POSITION pos = list.GetHeadPosition();
  while(pos)
    delete list.GetNext(pos);
}

ClConstraint* ClConstraint::lastConstraint = 0;

/////////////////////////////////// ClPar /////////////////////////////////////

void ClPar::Print(ofstream& fout) const
{
	std::string printStr = getPrintString();
	fout << printStr;
}

void ClPar::Print() const
{
	std::cout << getPrintString();
}

std::string ClPar::getPrintString() const
{
  std::stringstream printStr;
  printStr << type << " " << name;
  return printStr.str();
}


/////////////////////////////////// ClConstraint /////////////////////////////////////

ClConstraint::~ClConstraint()
{ 
  destroy(*cpars);
  delete cpars;
  delete cexpr;
}

void ClConstraint::Print(ofstream& fout) const
{
	std::string printStr = getPrintString();
	fout << printStr;
}

void ClConstraint::Print() const
{
	std::cout << getPrintString();
}

std::string ClConstraint::getPrintString() const
{
  std::stringstream printStr;
  printStr << "Constraint:    " << name << "\n";
  printStr << "\tCategory:    " << categoryName << "\n";
  printStr << "\tModel:       " << modelName << "\n";
  printStr << "\tAspect:      " << aspectName << "\n";
  printStr << "\tAction:      " << actionName << "\n";
  printStr << "\tPriority:    " << priority << "\n";
  printStr << "\tDescription: " << description << "\n\n";
 
  printStr << "\tParameters: ";
  POSITION pos = cpars->GetHeadPosition();
  while(pos) {
    printStr << cpars->GetNext(pos)->getPrintString();
    if(pos)
      printStr << ", ";
  }
  printStr << "\n";

  printStr << "\tExpression: ";
  if(cexpr)
    printStr << cexpr->getPrintString();
  printStr << "\n\n";

  return printStr.str();
}

std::string ClFormula::getPrintString() const
{
	std::stringstream printStr;
  printStr << "Formula:    " << name << "\n";
  printStr << "\tExpression: ";
  if(cexpr)
    printStr << cexpr->getPrintString();
  printStr << "\n\n";

  return printStr.str();
}

/////////////////////////////////// Cl /////////////////////////////////////

ClExpr::~ClExpr()
{
  delete lexpr;
}

void ClExpr::Print(ofstream& fout) const
{
	std::string printStr = getPrintString();
	fout << printStr;
}

void ClExpr::Print() const
{
	std::cout << getPrintString();
}

std::string ClExpr::getPrintString() const
{
  std::stringstream printStr;
  if(lexpr)
    printStr << lexpr->getPrintString();
  else
    printStr << "<empty>";

  return printStr.str();
}


/////////////////////////////////// ClLExpr /////////////////////////////////////

ClLExpr::~ClLExpr()
{
  delete relExpr;
  delete next;
}

char *ClLExpr::opNames[] = {
  "NOP",
  " && ",
  " || ",
  " ^^ ",
  " ==> "
};

void ClLExpr::Print(ofstream& fout) const
{
	std::string printStr = getPrintString();
	fout << printStr;
}

void ClLExpr::Print() const
{
	std::cout << getPrintString();
}

std::string ClLExpr::getPrintString() const
{
  std::stringstream printStr;
  if(relExpr)
	printStr << relExpr->getPrintString();
  if(lOp != noOp)
	printStr << opNames[lOp];
  if(next)
	printStr << next->getPrintString();

  return printStr.str();
}


/////////////////////////////////// ClIfExpr /////////////////////////////////////

ClIfExpr::~ClIfExpr()
{
  delete condE;
  delete thenE;
  delete elseE;
}

void ClIfExpr::Print(ofstream& fout) const
{
	std::string printStr = getPrintString();
	fout << printStr;
}

void ClIfExpr::Print() const
{
	std::cout << getPrintString();
}

std::string ClIfExpr::getPrintString() const
{
  std::stringstream printStr;
  printStr << " if ";
  if(condE)
	printStr << condE->getPrintString();
  printStr << " then ";
  if(thenE)
	printStr << thenE->getPrintString();
  if(elseE) {
    printStr << " else ";
	printStr << elseE->getPrintString();
  }

  return printStr.str();
}


/////////////////////////////////// ClRelExpr /////////////////////////////////////

ClRelExpr::~ClRelExpr()
{
  delete addExpr;
  delete next;
}

char *ClRelExpr::opNames[] = {
  "NOP",
  " == ",
  " < ",
  " > ",
  " >= ",
  " <= ",
  " != "
};

void ClRelExpr::Print(ofstream& fout) const
{
	std::string printStr = getPrintString();
	fout << printStr;
}

void ClRelExpr::Print() const
{
	std::cout << getPrintString();
}

std::string ClRelExpr::getPrintString() const
{
  std::stringstream printStr;
  if(addExpr)
	printStr << addExpr->getPrintString();
  if(relOp != noOp)
    printStr << opNames[relOp];
  if(next)
	printStr << next->getPrintString();

  return printStr.str();
}

/////////////////////////////////// ClAddExpr /////////////////////////////////////

ClAddExpr::~ClAddExpr()
{
  delete mulExpr;
  delete next;
}

char *ClAddExpr::opNames[] = {
  "NOP",
  " + ",
  " - "
};

void ClAddExpr::Print(ofstream& fout) const
{
	std::string printStr = getPrintString();
	fout << printStr;
}

void ClAddExpr::Print() const
{
	std::cout << getPrintString();
}

std::string ClAddExpr::getPrintString() const
{
  std::stringstream printStr;
  if(mulExpr)
	printStr << mulExpr->getPrintString();
  if(addOp != noOp)
    printStr << opNames[addOp];
  if(next)
	printStr << next->getPrintString();

  return printStr.str();
}

/////////////////////////////////// ClMulExpr /////////////////////////////////////

ClMulExpr::~ClMulExpr()
{
  delete unaExpr; 
  delete next;
}

char *ClMulExpr::opNames[] = {
  "NOP",
  " * ",
  " / "
};

void ClMulExpr::Print(ofstream& fout) const
{
	std::string printStr = getPrintString();
	fout << printStr;
}

void ClMulExpr::Print() const
{
	std::cout << getPrintString();
}

std::string ClMulExpr::getPrintString() const
{
  std::stringstream printStr;
  if(unaExpr)
	printStr << unaExpr->getPrintString();
  if(mulOp != noOp)
    printStr << opNames[mulOp];
  if(next)
	printStr << next->getPrintString();

  return printStr.str();
}


/////////////////////////////////// ClUnaExpr /////////////////////////////////////

ClUnaExpr::~ClUnaExpr()
{
  delete postfixExpr;
}

char *ClUnaExpr::opNames[] = {
  "NOP",
  " -",
  " !",
  "sin",
  "cos",
  "tan",
  "asin",
  "acos",
  "atan",
  "sinh",
  "cosh",
  "tanh",
  "asinh",
  "acosh",
  "atanh",
  "log2",
  "log10",
  "ln",
  "exp",
  "sqrt",
  "sign",
  "rint",
  "abs"
};

void ClUnaExpr::Print(ofstream& fout) const
{
	std::string printStr = getPrintString();
	fout << printStr;
}

void ClUnaExpr::Print() const
{
	std::cout << getPrintString();
}

std::string ClUnaExpr::getPrintString() const
{
  std::stringstream printStr;
  if(unaOp != noOp) {
    printStr << opNames[unaOp];
  }
  if(unaOp != noOp && unaOp != notOp) {
      printStr << "(";
  }
  if(postfixExpr)
	printStr << postfixExpr->getPrintString();
  if(unaOp != noOp && unaOp != notOp) {
      printStr << ")";
  }


  return printStr.str();
}


/////////////////////////////////// ClPostfixExpr /////////////////////////////////////

ClPostfixExpr::~ClPostfixExpr()
{
  delete primExpr;
  delete call;
}

void ClPostfixExpr::Print(ofstream& fout) const
{
	std::string printStr = getPrintString();
	fout << printStr;
}

void ClPostfixExpr::Print() const
{
	std::cout << getPrintString();
}

std::string ClPostfixExpr::getPrintString() const
{
  std::stringstream printStr;
  if(primExpr)
	printStr << primExpr->getPrintString();
  if(call)
	printStr << call->getPrintString();

  return printStr.str();
}

/////////////////////////////////// ClPrimExpr /////////////////////////////////////

ClPrimExpr::~ClPrimExpr()
{
  switch(type) {
  case litCollType: 
    delete expr.litColl;
    break;
  case litType: 
    delete expr.lit;
    break;
  case objType:
  case objCallType:
    delete expr.objCall.objName;
    delete expr.objCall.callPars;
    break;
  case exprType:
    delete expr.expr;
    break;
  case ifExprType:
    delete expr.ifExpr;
    break;
  }
}

void ClPrimExpr::Print(ofstream& fout) const
{
	std::string printStr = getPrintString();
	fout << printStr;
}

void ClPrimExpr::Print() const
{
	std::cout << getPrintString();
}

std::string ClPrimExpr::getPrintString() const
{
  std::stringstream printStr;
  switch(type) {
  case litCollType: 
    if(expr.litColl)
	  printStr << expr.litColl->getPrintString();
    break;
  case litType: 
    if(expr.lit)
	  printStr << expr.lit->getPrintString();
    break;
  case objType:
    if(expr.objCall.objName)
	  printStr << expr.objCall.objName->getPrintString();
    break;
  case objCallType:
    if(expr.objCall.objName)
	  printStr << expr.objCall.objName->getPrintString();
    if(expr.objCall.callPars) {
      printStr << "(";
      printStr << expr.objCall.callPars->getPrintString();
      printStr << ")";
    }
    break;
  case exprType:
    if(expr.expr)
      printStr << expr.expr->getPrintString();
    break;
  case ifExprType:
    if(expr.ifExpr)
      printStr << expr.ifExpr->getPrintString();
    break;
  }

  return printStr.str();
}

/////////////////////////////////// ClPostFixCall /////////////////////////////////////

ClPostfixCall::~ClPostfixCall()
{
  delete call;
  delete next;
}

char *ClPostfixCall::opNames[] = {
  "NOP",
  ".",
  "^",
  "->"
};

void ClPostfixCall::Print(ofstream& fout) const
{
	std::string printStr = getPrintString();
	fout << printStr;
}

void ClPostfixCall::Print() const
{
	std::cout << getPrintString();
}

std::string ClPostfixCall::getPrintString() const
{
  std::stringstream printStr;
  if(callOp != noOp)
    printStr << opNames[callOp];
  if(call)
    printStr << call->getPrintString();
  if(next)
    printStr << next->getPrintString();

  return printStr.str();
}

/////////////////////////////////// ClCallPars /////////////////////////////////////

ClCallPars::~ClCallPars()
{
  delete cDecl;
  delete actParList;
}

ClExprList *ClCallPars::GetExpressions()
{
  return actParList ? actParList->GetExpressions() : 0;
}

void ClCallPars::Print(ofstream& fout) const
{
	std::string printStr = getPrintString();
	fout << printStr;
}

void ClCallPars::Print() const
{
	std::cout << getPrintString();
}

std::string ClCallPars::getPrintString() const
{
  std::stringstream printStr;
  if(cDecl) {
    printStr << cDecl->getPrintString();
    printStr << " | ";
  }
  if(actParList)
    printStr << actParList->getPrintString();

  return printStr.str();
}


/////////////////////////////////// ClLit /////////////////////////////////////

ClLit::~ClLit()
{
  delete num;
}

void ClLit::Print(ofstream& fout) const
{
	std::string printStr = getPrintString();
	fout << printStr;
}

void ClLit::Print() const
{
	std::cout << getPrintString();
}

std::string ClLit::getPrintString() const
{
  std::stringstream printStr;
  if(type == strType)
    printStr << str;
  else
    printStr << num->getPrintString();

  return printStr.str();
}


/////////////////////////////////// ClType /////////////////////////////////////

void ClType::Print(ofstream& fout) const
{
	std::string printStr = getPrintString();
	fout << printStr;
}

void ClType::Print() const
{
	std::cout << getPrintString();
}

std::string ClType::getPrintString() const
{
  std::stringstream printStr;
  printStr << typeName << " ";
  return printStr.str();
}


/////////////////////////////////// ClLitColl /////////////////////////////////////

char *ClLitColl::typeNames[] = {
  " SET ",
  " BAG ",
  " SEQ ",
  " COLL ",
};

ClLitColl::~ClLitColl()
{
  delete range;
}

void ClLitColl::Print(ofstream& fout) const
{
	std::string printStr = getPrintString();
	fout << printStr;
}

void ClLitColl::Print() const
{
	std::cout << getPrintString();
}

std::string ClLitColl::getPrintString() const
{
  std::stringstream printStr;
  printStr <<  typeNames[type] << "(";
  if(range)
    printStr << range->getPrintString();
  printStr << ")";

  return printStr.str();
}


/////////////////////////////////// ClExprlRange /////////////////////////////////////

ClExprlRange::~ClExprlRange()
{
  delete from;
  delete to; 
  if(elems) {
    destroy(*elems);
    delete elems;
  }
}

void ClExprlRange::Print(ofstream& fout) const
{
	std::string printStr = getPrintString();
	fout << printStr;
}

void ClExprlRange::Print() const
{
	std::cout << getPrintString();
}

std::string ClExprlRange::getPrintString() const
{
  std::stringstream printStr;
  ASSERT(from);
  if(from)
    printStr << from->getPrintString();
  if(to) {
    printStr << " .. ";
    printStr << to->getPrintString();
  }
  if(elems) {
    POSITION pos = elems->GetHeadPosition();
    printStr << ", ";
    while(pos) {
      printStr << elems->GetNext(pos)->getPrintString();
      if(pos)
        printStr << ", ";
    }
  }

  return printStr.str();
}

/////////////////////////////////// ClCall /////////////////////////////////////

ClCall::~ClCall()
{
  delete pars;
}

void ClCall::Print(ofstream& fout) const
{
	std::string printStr = getPrintString();
	fout << printStr;
}

void ClCall::Print() const
{
	std::cout << getPrintString();
}

std::string ClCall::getPrintString() const
{
  std::stringstream printStr;
  printStr << name << "(";
  if(pars)
    printStr << pars->getPrintString();
  printStr << ")";

  return printStr.str();
}


/////////////////////////////////// ClDecl /////////////////////////////////////

ClDecl::~ClDecl()
{
  delete vars;
  delete type;
}

void ClDecl::Print(ofstream& fout) const
{
	std::string printStr = getPrintString();
	fout << printStr;
}

void ClDecl::Print() const
{
	std::cout << getPrintString();
}

std::string ClDecl::getPrintString() const
{
  std::stringstream printStr;
  if(type)
    printStr << type->getPrintString();
  if(vars) {
    POSITION pos = vars->GetHeadPosition();
    while(pos) {
      printStr << vars->GetNext(pos);
      if(pos)
        printStr << " , ";
    }
  }

  return printStr.str();
}


/////////////////////////////////// ClObjName /////////////////////////////////////

void ClObjName::Print(ofstream& fout) const
{
	std::string printStr = getPrintString();
	fout << printStr;
}

void ClObjName::Print() const
{
	std::cout << getPrintString();
}

std::string ClObjName::getPrintString() const
{
  std::stringstream printStr;
  if(selfP)
    printStr << "self";
  else
    printStr << objName;

  return printStr.str();
}


/////////////////////////////////// ClActParList /////////////////////////////////////

ClActParList::~ClActParList()
{
  if(exprList) {
    destroy(*exprList);
    delete exprList;
  }
}

ClExprList *ClActParList::GetExpressions()
{
  return exprList;
}

void ClActParList::Print(ofstream& fout) const
{
	std::string printStr = getPrintString();
	fout << printStr;
}

void ClActParList::Print() const
{
	std::cout << getPrintString();
}

std::string ClActParList::getPrintString() const
{
  std::stringstream printStr;
  if(exprList) {
    POSITION pos = exprList->GetHeadPosition();
    while(pos) {
      printStr << exprList->GetNext(pos)->getPrintString();
      if(pos)
        printStr << " , ";
    }
  }

  return printStr.str();
}


/////////////////////////////////// ClNumber /////////////////////////////////////

void ClNumber::Print(ofstream& fout) const
{
	std::string printStr = getPrintString();
	fout << printStr;
}

void ClNumber::Print() const
{
	std::cout << getPrintString();
}

std::string ClNumber::getPrintString() const
{
  std::stringstream printStr;
  printStr << (type == intType ? value.i : value.d);
  return printStr.str();
}


bool ParseMCLString(char const *buf);
bool ParseMCLString(wchar_t const *buf) {
	return ParseMCLString(CStringA(buf));
}

char *GetMCLErrorMessage(void);
#ifdef UNICODE
wchar_t *GetMCLErrorMessageW(void) {
	CStringA err = GetMCLErrorMessage();
	// parser returns 0xff for eof
	err.Replace("\xff", "<eof>");
	CStringW ret = err;
	static wchar_t errmsg[200];
	wcsncpy(errmsg, static_cast<const wchar_t*>(ret), sizeof(errmsg) / sizeof(errmsg[0]));
	return errmsg;
}
#endif
