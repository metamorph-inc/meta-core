// -*-C++-*-
// clmain.cpp
// Constraint class implementations
/*
CHANGE LOG

SKN<06.26.2002>   ClObjName::Eval Uncommented the throw exception when function not found

*/



#include "stdafx.h"
#include "desert.h"
#include "CSetErrDialog.h"
#include "core/clmain.h"
#include "BackIface.h"
#include "core/manager.h"
#include "core/dynamic.h"

#ifdef _UNICODE
typedef std::wstring tstring;
typedef std::wstringstream tstringstream;

#else
typedef std::string tstring;
typedef std::stringstream tstringstream;
#endif

#include "muparser_wrapper.h"

#include "bdd/bdd_math.h"



ClFunctionTable ClFunction::fnTable;


ClContext::ClContext() : cosmic(0), cosmicList(0), dependency(0), propertyDependency(0)
{
	type = Undefined;
	errFlag = false;
	errString = 0;
}

ClContext::ClContext(const ClContext& from) : cosmic(from.cosmic), cosmicList(from.cosmicList)
{
	type = from.type;
	errFlag = from.errFlag;
	errString = _tcsdup(from.errString);
	dependency = from.dependency;
	propertyDependency = from.propertyDependency;
}

ClContext::ClContext(CCosmic *e) : cosmic(e), cosmicList(0), dependency(0), propertyDependency(0)
{
	type = Cosmic;
	errFlag = false;
	errString = 0;
}

ClContext::ClContext(CCosmicList *e) : cosmic(0), cosmicList(e), dependency(0), propertyDependency(0)
{
	type = CosmicList;
	errFlag = false;
	errString = 0;
}

//Eval and Verifiy are the entry points to this set of functions

CBdd ClContext::Eval(ClConstraint *c)
{
	//ASSERT_EX( c, "ClContext::Eval(ClConstraint *c)", "constraint is null" );
	
	if (!c) 
		throw new CDesertException("ClContext::Eval() : constraint is NULL!");

	ClData val = c->Eval(*this);
	CBdd ret;
	if((ClDataType)val == Boolean)
	{
		bool ret_bool;
		val.Get(ret_bool);
		ret = ret_bool ? CBdd::One() : CBdd::Zero();
	}
	else
		ret = ((ClDataType)val == Bdd) && !errFlag ? (const CBdd &)(val) : CBdd::One();
	return ret;
}

CBdd ClContext::Eval(ClFormula *f)
{
	if (!f) 
		throw new CDesertException("ClContext::Eval() : constraint is NULL!");

	ClData val = f->Eval(*this);
	CBdd ret = ((ClDataType)val == Bdd) && !errFlag ? (const CBdd &)(val) : CBdd::One();
	return ret;
}

bool ClContext::Verify(ClConstraint *c, CCosmicList* dep, CStringList *pdep, TCHAR*& err)
{

	//ASSERT_EX( c && dep && cosmic && pdep, "ClContext::Verify",
    //             "constraint | dependency | context | propertyDependency is null");

	if (!(c && dep && cosmic && pdep))
		throw new CDesertException("ClContext::Verify() : Either constraint and/or dependency and/or context and/or propertyDependecy are NULL!");

	(dependency = dep)->RemoveAll();
	(propertyDependency = pdep)->RemoveAll();
	ClData data = c->Eval(*this);
	errFlag = errFlag || (ClDataType)data == Undefined;
	err = errString;
	return !errFlag;
}

ClData ClContext::Eval()
{
	bool check = (type==Cosmic) && cosmic;
	if (!check)
	{
		//Error("ClContext::Eval", "invalid context");
		throw new CDesertException("ClContext::Eval(): invalid context!");
		errFlag = true;
		return (CCosmic *)0;
	}
	return ClData(UpdateDependency(cosmic));
}

ClData ClContext::Eval(const CString& name)
{
	
	CCosmic *obj=0;
	bool check = (type==Cosmic) ? cosmic!=0 : cosmicList!=0;
	if (!check)
	{
		//Error("ClContext::Eval", "invalid context");
		throw new CDesertException("ClContext::Eval: invalid context<"+name+">");
		//errFlag = true;
		//return obj;
	}
	CCosmicList *list = (type==Cosmic) ? cosmic->Children() : cosmicList;
	check = list ? ::Find(*list, obj, name) : false;
	if (!check) 
		//errFlag = true;
		throw new CDesertException("ClContext::Eval: invalid context<"+name+">");
	return ClData(UpdateDependency(obj));
}

CCosmic * ClContext::UpdateDependency(CCosmic *obj)
{
	if (dependency && obj && obj != CManager::theInstance && !dependency->Find(obj))
		dependency->AddTail(obj);
	return obj;
}

CCosmicList * ClContext::UpdateDependency(CCosmicList *list)
{
  if (dependency && list)
  {
    POSITION pos = list->GetHeadPosition();
    while(pos)
	{
	  CCosmic *obj = list->GetNext(pos);
	  if (!dependency->Find(obj)) dependency->AddTail(obj);
	}
  }
  return list;
}

CCosmic * ClContext::UpdatePropertyDependency(CCosmic *obj, const TCHAR *prop)
{
	UpdateDependency(obj);
	if (propertyDependency && !propertyDependency->Find(prop))
		propertyDependency->AddTail(prop);
	return obj;
}

void ClFunction::CreateAll()
{
	static bool init=false;
	if (!init)
	{
		fnTable.InitHashTable(127);
		init = true;
	}
	Create("parent", &Parent);
	Create("children", &Children);
	Create("project", &Project);
	Create("implementedBy", &ImplementedBy);

	// this one needs to be take out from here
	//Create("latency", &Latency);
}

void ClFunction::CreatePCMFn(const CString& prop, const CString& pcm_fn)
{
	ClFunction *fn=0;
	if (fnTable.Lookup(prop, fn)) return;

	if (pcm_fn == "PCM_ADD")				//arithmetical sum
		Create(prop, &PCMAdd);				
	
	else if (pcm_fn == "PCM_MUL")			//arithmetical product
		Create(prop, &PCMMul);

	else if (pcm_fn == "PCM_AMED")			//arithmetical medium
		Create(prop, &PCMAmed);

	else if (pcm_fn == "PCM_GMED")			//geomethric medium
		Create(prop, &PCMGmed);

	else if (pcm_fn == "PCM_MIN")			//arithmetic minimum
		Create(prop, &PCMMin);

	else if (pcm_fn == "PCM_MAX")			//arithmetic maximum
		Create(prop, &PCMMax);

	else if (pcm_fn == "PCM_NOP")			
		Create(prop, &PCMNop);


	else if (pcm_fn == "PCM_NONE")			//non-composable property
		Create(prop, &PCMNone);

	else if (pcm_fn == "PCM_OR")			//non-composable property
		Create(prop, &PCMOr);

	else /*if (pcm_fn == "PCM_CUST")*/		//otherwise it's a custom PCM function
		Create(prop, &PCMCust);

/*
	else
	Info("Unknown PCM Function %s \n", pcm_fn);
	*/
}

void ClFunction::CreateContainer(const CString& name)
{
	Create(name, &Container);
};

void ClFunction::DestroyAll()
{
	POSITION pos = fnTable.GetStartPosition();
	while(pos)
	{
		CString nm;
		ClFunction *next;
		fnTable.GetNextAssoc(pos, nm, next);
		delete next;
	}

	fnTable.RemoveAll();
}

ClData ClFunction::Parent(ClContext& c, ClCallPars *p)
{
	bool ret=true;
	CHECK_PARS_NODECL_ISEMPTY(c, p, ret);
	CCosmic *obj;
	PREPARE_OBJ_CALL(c, obj, ret);
	CCosmic *par = obj ? obj->Parent() : obj;
	return c.UpdateDependency(par);
}

ClData ClFunction::Project(ClContext& c, ClCallPars *p)
{
	bool ret=true;
	CHECK_PARS_NODECL_ISEMPTY(c, p, ret);
	CCosmic *obj;
	switch((ClDataType)c)
	{
	case Cosmic:
		obj = (CCosmic *)c;
		break;
	case CosmicList:
	{
		CCosmicList *list = (CCosmicList *)c;
		obj = list && list->GetCount()>0 ? list->GetHead() : 0;
	}
		break;
	}
	if (!obj)
	{
		c.SetError(true, _T("ClFunction::Project(): invalid context"));
		//throw new CDesertException(false, "ClFunction::Project(ClContext& c, ClCallPars *p): Invalid context");
		return obj;
	}
	CCosmic *prj = obj->Project();
	return c.UpdateDependency(prj);
}

ClData ClFunction::ImplementedBy(ClContext& c, ClCallPars *p)
{
	bool ret=true;
	CHECK_PARS_NODECL_ISEMPTY(c, p, ret);
	CCosmic *obj;
	PREPARE_OBJ_CALL(c, obj, ret);
	if (obj) obj->SetFunction(funcImplementedBy);
	return c.UpdateDependency(obj);
}


ClData ClFunction::Children(ClContext& c, ClCallPars *p)
{
	bool ret=true;
	ClExpr *parExp;
	CHECK_PARS_NODECL_HASONEORNONE(c, p, parExp, ret);
	CCosmic *ctx_obj;
	PREPARE_OBJ_CALL(c, ctx_obj, ret);
	CCosmicList *list = ctx_obj ? ctx_obj->Children() : 0;
	if (parExp)
	{
		CCosmic *obj;
		ClData val = parExp->Eval(c);
		const CString& cname = (ClDataType)val == String ? (const CString &)val : CString(_T(""));
		ret = list ? ::Find(*list, obj, cname) : false;
		if (ret) return c.UpdateDependency(obj);
		CString buf; buf.Format(_T("ClFunction::Children : child <%s> not found"), cname);

		if (ctx_obj && ctx_obj->IsType(typeDynElement))
		{
		//	Warning(_T(""),"Child \"%s\" not found in \"%s\"", cname, ((CDynElement *)ctx_obj)->GetName());
			throw new CDesertNoneElementException();
		}

		c.SetError(true, buf);
		//throw new CDesertException(false, buf);
		return (CCosmic *)0;
	}
  return c.UpdateDependency(list);
}

ClData ClFunction::Container(ClContext& c, ClCallPars *p)
{
	bool ret=true;
	ClExpr *parExp;
	CHECK_PARS_NODECL_HASONEORNONE(c, p, parExp, ret);
	CCosmic *obj;
	PREPARE_OBJ_CALL(c, obj, ret);
	CCosmicList *list = obj ? obj->GetContainer(name) : 0;

	if (parExp)
	{
		ClData val = parExp->Eval(c);
		const CString& cname = (ClDataType)val == String ? (const CString &)val : CString(_T(""));
		ret = list ? ::Find(*list, obj, cname) : false;
		if (ret) return c.UpdateDependency(obj);
		CString buf; buf.Format(_T("ClFunction::Container element <%s> not found in container: <%s>"), cname, (LPCTSTR)name);
		c.SetError(true, buf);
		
		//throw new CDesertException(false, buf);
		return (CCosmic *)0;
	}
	return c.UpdateDependency(list);
}

ClData ClFunction::PCMCust(ClContext& c, ClCallPars *p)
{
  bool ret=true;
  CHECK_PARS_NODECL_ISEMPTY(c, p, ret);
  CCosmic *obj;
  PREPARE_OBJ_CALL(c, obj, ret);
  if (obj) obj->SetFunction(funcPCMCust, name); // indicate that the property to be used is the name of the property!!
  return c.UpdatePropertyDependency(obj, (LPCTSTR)name);
}


ClData ClFunction::PCMNop(ClContext& c, ClCallPars *p)
{
	bool ret=true;
	CHECK_PARS_NODECL_ISEMPTY(c, p, ret);
	CCosmic *obj;
	PREPARE_OBJ_CALL(c, obj, ret);
	if (obj) obj->SetFunction(funcPCMNop, name); // indicate that the property to be used is the name of this function
	return c.UpdatePropertyDependency(obj, (LPCTSTR)name);
}


ClData ClFunction::PCMNone(ClContext& c, ClCallPars *p)
{
	bool ret=true;
	CHECK_PARS_NODECL_ISEMPTY(c, p, ret);
	CCosmic *obj;
	PREPARE_OBJ_CALL(c, obj, ret);
	if (obj) obj->SetFunction(funcPCMNone, name); // indicate that the property to be used is the name of this function
	return c.UpdatePropertyDependency(obj, (LPCTSTR)name);
}


ClData ClFunction::PCMAdd(ClContext& c, ClCallPars *p)
{
	bool ret=true;
	CHECK_PARS_NODECL_ISEMPTY(c, p, ret);
	CCosmic *obj;
	PREPARE_OBJ_CALL(c, obj, ret);
	if (obj) obj->SetFunction(funcPCMAdd, name); // indicate that the property to be used is latency
	return c.UpdatePropertyDependency(obj, (LPCTSTR)name);
}

ClData ClFunction::PCMMul(ClContext& c, ClCallPars *p)
{
	bool ret=true;
	CHECK_PARS_NODECL_ISEMPTY(c, p, ret);
	CCosmic *obj;
	PREPARE_OBJ_CALL(c, obj, ret);
	if (obj) obj->SetFunction(funcPCMMul, name); // indicate that the property to be used is latency
	return c.UpdatePropertyDependency(obj, (LPCTSTR)name);
}

ClData ClFunction::PCMAmed(ClContext& c, ClCallPars *p)
{
	bool ret=true;
	CHECK_PARS_NODECL_ISEMPTY(c, p, ret);
	CCosmic *obj;
	PREPARE_OBJ_CALL(c, obj, ret);
	if (obj) obj->SetFunction(funcPCMAmed, name); // indicate that the property to be used is latency
	return c.UpdatePropertyDependency(obj, (LPCTSTR)name);
}

ClData
ClFunction::
PCMGmed(ClContext& c, ClCallPars *p)
{
	bool ret=true;
	CHECK_PARS_NODECL_ISEMPTY(c, p, ret);
	CCosmic *obj;
	PREPARE_OBJ_CALL(c, obj, ret);
	if (obj) obj->SetFunction(funcPCMGmed, name); // indicate that the property to be used is latency
	return c.UpdatePropertyDependency(obj, (LPCTSTR)name);
}

ClData ClFunction::PCMMin(ClContext& c, ClCallPars *p)
{
	bool ret=true;
	CHECK_PARS_NODECL_ISEMPTY(c, p, ret);
	CCosmic *obj;
	PREPARE_OBJ_CALL(c, obj, ret);
	if (obj) obj->SetFunction(funcPCMMin, name); // indicate that the property to be used is latency
	return c.UpdatePropertyDependency(obj, (LPCTSTR)name);
}

ClData ClFunction::PCMMax(ClContext& c, ClCallPars *p)
{
	bool ret=true;
	CHECK_PARS_NODECL_ISEMPTY(c, p, ret);
	CCosmic *obj;
	PREPARE_OBJ_CALL(c, obj, ret);
	if (obj) obj->SetFunction(funcPCMMax, name); // indicate that the property to be used is latency
	return c.UpdatePropertyDependency(obj, (LPCTSTR)name);
}

ClData ClFunction::PCMOr(ClContext& c, ClCallPars *p)
{
	bool ret=true;
	CHECK_PARS_NODECL_ISEMPTY(c, p, ret);
	CCosmic *obj;
	PREPARE_OBJ_CALL(c, obj, ret);
	if (obj) obj->SetFunction(funcPCMOr, name); // indicate that the property to be used is latency
	return c.UpdatePropertyDependency(obj, (LPCTSTR)name);
}

ClData ClConstraint::Eval(ClContext& c) const
{
	if (cpars && cpars->GetCount()>0)
	Todo(_T("ClConstraint::Eval"), _T("<%s:%d> tbd skn cpars"), _T(__FILE__), __LINE__);
	
	if (!cexpr) 
		throw new CDesertException(_T("ClConstraint::Eval(ClContext& c) : cexpr is NULL!"));

	//ASSERT_EX( cexpr, _T("ClConstraint::Eval"), _T("cexpr is null") );
	return cexpr->Eval(c);
}

ClData ClFormula::Eval(ClContext& c) const
{
	if (cpars && cpars->GetCount()>0)
	Todo(_T("ClConstraint::Eval"), _T("<%s:%d> tbd skn cpars"), _T(__FILE__), __LINE__);
	
	if (!cexpr) 
		throw new CDesertException(_T("ClConstraint::Eval(ClContext& c) : cexpr is NULL!"));

	//ASSERT_EX( cexpr, _T("ClConstraint::Eval"), _T("cexpr is null") );
	return cexpr->Eval(c);
}

ClData ClExpr::Eval(ClContext& c) const
{
	//ASSERT_EX( lexpr, _T("ClExpr::Eval"), _T("lexpr is null") );
	if (!lexpr) 
		throw new CDesertException(_T("ClExpr::Eval(ClContext& c) : lexpr is NULL!"));

	return lexpr->Eval(c);
}

ClData ClLExpr::Eval(ClContext& c) const
{
	//ASSERT_EX( relExpr, _T("ClLExpr::Eval"), _T("relExpr is null") );

	if (!relExpr) 
		throw new CDesertException(_T("ClLExpr::Eval(ClContext& c) : relExpr is NULL!"));
	
	ClData left;
	try{
		left = relExpr->Eval(c);
	}
	//if(c.GetError() && lOp != implOp)
	catch (CDesertNoneElementException *e)
	{
		e->Delete();
		left = CBdd::Zero();
	} ///Feng: if there is err caused by different reason, for example, the children node dosen't exist(JIRA META-102), set to Bdd::Zero 

	return next ? next->Eval(c, left, lOp) : left;
}

ClData ClLExpr::Eval(ClContext&c, const ClData& preVal, LOp preOp) const
{
	//ASSERT_EX( relExpr, _T("ClLExpr::Eval"), _T("relExpr is null") );
	if (!relExpr) 
		throw new CDesertException(_T("ClLExpr::Eval(ClContext&c, const ClData& preVal, LOp preOp) : relExpr is NULL!"));

	ClData right;
	try{
		right = relExpr->Eval(c);
	}
	//if(c.GetError() && lOp != implOp)
	catch (CDesertNoneElementException *e)
	{
		e->Delete();
		right = CBdd::Zero();
	}
	ClData left = Eval(preVal, preOp, right);
	
	return next ? next->Eval(c, left, lOp) : left;
}

ClData ClLExpr::Eval(const ClData& left, LOp op, const ClData& right) const
{

	if ( (ClDataType)left != (ClDataType)right )
	{
		//Error( _T("ClLExpr::Eval"), _T("a LOP b : a & b not of the same type") );
		throw new CDesertException(false, _T("ClLExpr::Eval(): ") _T("a LOP b : a & b not of the same type"));
		return ClData();
	}

	switch( (ClDataType)left )
	{
	case Boolean:
		return Eval((bool)left, op, (bool)right);
	case Bdd:
		return Eval((const CBdd&)left, op, (const CBdd&)right);
	default:
		//Error(_T("ClLExpr::Eval"), _T("invalid operands for a Logical Op"));
		throw new CDesertException(false, _T("ClLExpr::Eval(): ") _T("invalid operands for a Logical Op"));
		return ClData();
	}
}

bool ClLExpr::Eval(bool left, LOp op, bool right) const
{
	//ASSERT_EX( op != noOp, _T("ClLExpr::Eval"), _T("invalid operator") );
	if (op == noOp)
		throw new CDesertException(_T("ClLExpr::Eval(bool left, LOp op, bool right) : invalid operator!"));


	switch(op)
	{
	case andOp:
		return left && right;
	case orOp:
		return left || right;
	case implOp:
		return !left || right;
	case xorOp:
		return (left && !right) || (!left && right);
	}
	return false;
}

CBdd ClLExpr::Eval(const CBdd& left, LOp op, const CBdd& right) const
{
	//ASSERT_EX( op != noOp, _T("ClLExpr::Eval"), _T("invalid operator") );
	if (op == noOp)
		throw new CDesertException(_T("ClLExpr::Eval(const CBdd& left, LOp op, const CBdd& right) const : invalid operator!"));

	switch(op)
	{
	case andOp:
		return ClData(left && right);
	case orOp:
		return ClData(left || right);
	case implOp:
		return ClData(CBdd::Implies(left, right));
	case xorOp:
		return ClData(CBdd::Xor(left, right));
	}
	return ClData(CBdd::Zero());
}

ClData ClIfExpr::Eval(ClContext& c) const
{
	if (!(condE && thenE))
		throw new CDesertException(_T("ClIfExpr::Eval(ClContext& c) const : condE or thenE is null!"));

	//ASSERT_EX( condE && thenE, _T("ClIfExpr::Eval"), _T("condE or thenE is null") );
	ClData cval, tval, eval; 
	try{
		cval = condE->Eval(c);
	}
	catch (CDesertNoneElementException *e)
	{
		e->Delete();
		cval = CBdd::Zero();
	}
	try{
		tval = thenE->Eval(c);
	}
	catch (CDesertNoneElementException *e)
	{
		e->Delete();
		tval = CBdd::Zero();
	} 
	 
	if (elseE) 
	{
		try{
			eval = elseE->Eval(c);
		}
		catch (CDesertNoneElementException *e)
		{
			e->Delete();
			eval = CBdd::Zero();
		} 
	}

	return Eval(cval, tval, eval);
}

ClData ClIfExpr::Eval(const ClData& _cond, const ClData& _then, const ClData& _else) const
{
	if ( (ClDataType)_cond != (ClDataType)_then || ((ClDataType)_else != (ClDataType)_cond && (ClDataType)_else != Undefined) )
	{
		//Error(_T("ClIfExpr::Eval"), _T("if a then b else c : a/b/c not of the same type") );
		throw new CDesertException(false, _T("ClIfExpr::Eval: if a then b else c : a/b/c not of the same type") );
		return ClData();
	}

	switch((ClDataType)_cond)
	{
	case Boolean:
		return Undefined == (ClDataType)_else ? (bool)_cond && (bool)_then : (bool)_cond ? (bool)_then : (bool)_else;
	case Bdd:
		return (ClDataType)_else == Undefined ? Eval((CBdd)_cond, (CBdd)_then) : Eval((CBdd)_cond, (CBdd)_then, (CBdd)_else);
	default:
		//Error(_T("ClIfExpr::Eval"), _T("invalid operands for an i-t-e"));
		throw new CDesertException(false, _T("ClIfExpr::Eval: invalid operands for an if-then-else!"));
	return ClData();
	}
}

CBdd ClIfExpr::Eval(const CBdd& _if, const CBdd& _then) const
{
	Todo(_T("ClIfExpr::Eval"), _T("<%s:%d> implement this"));
	//return Eval(_if, _then, CBdd::Zero());
	return CBdd::Implies(_if, _then);
}
  
CBdd ClIfExpr::Eval(const CBdd& _if, const CBdd& _then, const CBdd& _else) const
{
	return CBdd::IfThenElse(_if, _then, _else);
}

ClData ClRelExpr::Eval(ClContext& c) const
{
	//ASSERT_EX( addExpr, _T("ClRelExpr::Eval"), _T("addExpr is null") );
	if (!addExpr)
		throw new CDesertException(_T("ClRelExpr::Eval(ClContext& c) const : addExpr is null!"));
	
	ClData left;
	try{
		left = addExpr->Eval(c);
	}
	catch (CDesertNoneElementException *e)
	{
		if(next)
		{
			e->Delete();
			left = CBdd::ArithZero();
		}
		else 
			throw e;
	} 

	if(next)
	{
		if( (ClDataType)left == Cosmic &&  ((const CCosmic *)left)->IsNumericFunc())
		{
			left = ((const CCosmic *)left)->Eval();
		}
		return Eval(c, left, next->Eval(c) );
	}
	else if((ClDataType)left == Cosmic)
	{
		left = ((const CCosmic *)left)->Eval();
	}
	return left;
}

ClData ClRelExpr::Eval(ClContext& c, const ClData& left, const ClData& right) const
{
	CBdd ret;
	ClData lBdd, rBdd;
	if ( (ClDataType)left == (ClDataType)right )
	{
		switch ((ClDataType)left)
		{
		case Integer:
		  return Eval((int)left, (int)right); 
		case Double:
		  return Eval((double)left, (double)right);
		case Cosmic:
		  return Eval((const CCosmic *)left, (const CCosmic *)right);
		case Bdd:
			return Eval((const CBdd&)left, (const CBdd&)right);
		default:
			throw new CDesertException(_T("ClRelExpr::Eval(): invalid operands for a Relational OP"));
		  return ClData();
		}
	}
	else if ((ClDataType)left == Cosmic)
	{
		lBdd = ((const CCosmic *)left)->Eval();
		switch ((ClDataType)right)
		{
		case Integer:			
		  return Eval(c, (const CCosmic *)left, (int)right);
		case Double:
			{
				if((ClDataType)lBdd==Cosmic)
					return Eval(c, (const CCosmic *)left, (double)right);
				else
					return Eval((const CBdd&)lBdd,(double)right);
			}
		case Bdd:		
			return Eval((const CBdd&)lBdd, (const CBdd&)right);
		default:
			throw new CDesertException(_T("ClRelExpr::Eval(): invalid operands for a Relational OP"));
			return ClData();
		}
	}
	else if ((ClDataType)right == Cosmic)
	{
		rBdd = ((const CCosmic *)right)->Eval();
		switch ((ClDataType)left)
		{
		case Integer:
		  return Eval(c, (int)left, (const CCosmic *)right);
		case Double:
			{
				if((ClDataType)rBdd==Cosmic)
					return Eval(c, (double)left, (const CCosmic *)right);
				else
					return Eval((double)left, (const CBdd&)rBdd);
			}
		case Bdd:			
			return Eval((const CBdd&)left, (const CBdd&)rBdd);
		default:
			throw new CDesertException(_T("ClRelExpr::Eval(): invalid operands for a Relational OP"));
			return ClData();
		}
	}
	else if ((ClDataType)left == Bdd)
	{
#ifndef DOUBLE_MTBDD
		int limit;
		switch ((ClDataType)right)
		{
		case Integer:
			limit = (int)right;
			break;
		  //return ((CBdd)left).low_threshold((int)right);
		case Double:
			limit = (int)(double)right;
			break;
		  //return ((CBdd)left).low_threshold((int)(double)right);
		default:
		  Error(_T("ClRelExpr::Eval"), _T("invalid operands for a Relational OP"));
			return ClData();
		}
#endif
		switch(relOp)
		{
			case ClRelExpr::eqOp:
			{
				return ((CBdd)left).equals_to((double)right);
			}

			//not equals to relation operator
			case ClRelExpr::neOp:
			{
				return ((CBdd)left).not_equals_to((double)right);
			}
			//less then or equal relation operator
			case ClRelExpr::leOp:
#ifndef DOUBLE_MTBDD
				limit++;
#else
				return ((const CBdd)left).threshold((double)right, true);	
#endif
			//less then relation operator
			case ClRelExpr::ltOp:
			{
#ifndef DOUBLE_MTBDD
				return ((const CBdd)left).threshold(limit);	
#else
				return ((const CBdd)left).threshold((double)right);	
#endif
			}

			//greater then or equal relation operator
			case ClRelExpr::geOp:
#ifndef DOUBLE_MTBDD				
				limit--;
#else
				return ((const CBdd)left).low_threshold((double)right, true);
#endif
			//greater then relation operator
			case ClRelExpr::gtOp:
			{
				return ((const CBdd)left).low_threshold((double)right);
			}

			default:
			{
				//ASSERT_EX(false, _T("CDynElement::PCMAdd()"), _T(" Operator unimplemented!"));
				throw new CDesertException(_T("unknow error!"));
				return ret;
			}
		}

	}
	else if ((ClDataType)left == Boolean)
	{
		return ClData(left);
	}
	else
	{
		//Error( _T("ClRelExpr::Eval"), _T("a ROP b : a & b not of the same type") );
		throw new CDesertException(_T("ClRelExpr::Eval(): a ROP b : a & b not of the same type"));
		return ClData(false);
	}
}

ClData ClRelExpr::Eval(const CBdd& left, double right) const
{
	switch(relOp)
	{
		case ClRelExpr::eqOp:
		{
			return ((CBdd)left).equals_to((double)right);
		}

		//not equals to relation operator
		case ClRelExpr::neOp:
		{
			return ((CBdd)left).not_equals_to((double)right);
		}
		//less then or equal relation operator
		case ClRelExpr::leOp:
			return ((const CBdd)left).threshold((double)right, true);	
		//less then relation operator
		case ClRelExpr::ltOp:
		{
			return ((const CBdd)left).threshold((double)right);	
		}

		//greater then or equal relation operator
		case ClRelExpr::geOp:
		{
			return ((const CBdd)left).low_threshold((double)right, true);
		}
		//greater then relation operator
		case ClRelExpr::gtOp:
		{
			return ((const CBdd)left).low_threshold((double)right);
		}

		default:
		{
			throw new CDesertException(_T("unknow error!"));
		}
	}
}

ClData ClRelExpr::Eval(double left, const CBdd& right) const
{
	switch(relOp)
	{
		case ClRelExpr::eqOp:
		{
			return ((CBdd)right).equals_to((double)left);
		}

		//not equals to relation operator
		case ClRelExpr::neOp:
		{
			return ((CBdd)right).not_equals_to((double)left);
		}
		//less then or equal relation operator
		case ClRelExpr::geOp:
			return ((const CBdd)right).threshold((double)left, true);	
		//less then relation operator
		case ClRelExpr::gtOp:
		{
			return ((const CBdd)right).threshold((double)left);	
		}

		//greater then or equal relation operator
		case ClRelExpr::leOp:
		{
			return ((const CBdd)right).low_threshold((double)left, true);
		}
		//greater then relation operator
		case ClRelExpr::ltOp:
		{
			return ((const CBdd)right).low_threshold((double)left);
		}

		default:
		{
			throw new CDesertException(_T("unknow error!"));
		}
	}
}

#ifndef DOUBLE_MTBDD  
bool ClRelExpr::Eval(int left, int right) const
#else
bool ClRelExpr::Eval(double left, double right) const
#endif
{
	//ASSERT_EX( relOp != noOp, _T("ClRelExpr::Eval"), _T("invalid operator") );

	if (relOp == noOp)
		throw new CDesertException(_T("ClRelExpr::Eval(int left, int right): invalid operator"));
	
	switch(relOp)
	{
	case eqOp:
		return left==right;
	case ltOp:
		return left<right;
	case gtOp:
		return left>right;
	case leOp:
		return left<=right;
	case geOp:
		return left>=right;
	case neOp:
		return left!=right;
	}
  return false;
}

ClData ClRelExpr::Eval(const CBdd& left, const CBdd& right) const
{
	CBdd ret = CBdd::One();

	ret = left - right;	
	int limit = 0;
	switch (relOp)
	{
		//equals to relation operator
		case eqOp:
		{
			ret = ret.equals_to(limit);
			return ret;
		}

		//not equals to relation operator
		case neOp:
		{
			ret = ret.not_equals_to(limit);						
			return ret;
		}
		//less then or equal relation operator
		case leOp:
#ifndef DOUBLE_MTBDD
			limit++;
#else
			ret = ret.threshold(limit, true);
#endif
		//less then relation operator
		case ltOp:
		{
			ret = ret.threshold(limit);						
			return ret;
		}

		//greater then or equal relation operator
		case geOp:
#ifndef DOUBLE_MTBDD
			limit--;
#else
			ret = ret.low_threshold(limit, true);
#endif
		//greater then relation operator
		case gtOp:
		{
			ret = ret.low_threshold(limit);
			return ret;
		}

		default:
		{
			//ASSERT_EX(false, _T("CDynElement::PCMAdd()"), _T(" Operator unimplemented!"));
			throw new CDesertException(_T("unknow error!"));
			return ret;
		}
	};

	return ClData(ret);
}

ClData ClRelExpr::Eval(const CCosmic *left, const CCosmic *right) const
{
	//ASSERT_EX( relOp == eqOp, _T("ClRelExpr::Eval"), _T("invalid operator") );
	//fatal error
	if (relOp == noOp)
		throw new CDesertException(_T("ClRelExpr:: Eval(const CCosmic *left, const CCosmic *right: invalid operator"));
	

	//that's a user error!
//	if (relOp != eqOp)
//		throw new CDesertException(false, _T("ClRelExpr:: Eval(ClContext& c, const CCosmic *left, const CCosmic *right: invalid operator. The only suported operator on objects is the equality '=' operator!"));
	

	if (!left || !right)
	{
		// tbd skn - rel expr between two cosmic objects, either of which may be
		// null - return a bdd of one instead of farting out
		// careful - this might lead to unwarranted side effects
		// c.SetError(true);

		//if manager is not instantaniated yet, then we are in the stage of verifying constraints
		CBdd one_bdd;
		try
		{
			one_bdd = CBdd::One();
		}
		catch (CDesertException *e)
		{
			//catche the fatal BDD exception here and generate a non-fatal exception
			e->Delete();
			
			throw new CDesertException(false, _T("ClRelExpr:: Eval(ClContext& c, const CCosmic *left, const CCosmic *right): At least one of the operands does not exists!"));
		}

		return ClData(one_bdd);
	}

	return left->Eval(relOp, right);
}

#ifndef DOUBLE_MTBDD  
ClData ClRelExpr::Eval(ClContext& c, const CCosmic *left, int right) const
#else
ClData ClRelExpr::Eval(ClContext& c, const CCosmic *left, double right) const
#endif
{
	if (!left)
	{
		c.SetError(true);
		return ClData(false);
	}
	return left->Eval(relOp, right);
}

#ifndef DOUBLE_MTBDD  
ClData ClRelExpr::Eval(ClContext& c, int left, const CCosmic *right) const
#else
ClData ClRelExpr::Eval(ClContext& c, double left, const CCosmic *right) const
#endif
{
	if (!right)
	{
		c.SetError(true);
		return ClData(false);
	}
	switch(relOp)
	{
	case eqOp:
		return right->Eval(eqOp, left);
	case ltOp:
		return right->Eval(gtOp, left);
	case leOp:
		return right->Eval(geOp, left);
	case gtOp:
		return right->Eval(ltOp, left);
	case geOp:
		return right->Eval(leOp, left);
	case neOp:
		return right->Eval(neOp, left);
	}
	return ClData(false);
}

ClData ClAddExpr::Eval(ClContext& c) const
{
	//ASSERT_EX( mulExpr, _T("ClAddExpr::Eval"), _T("mulExpr is null") );
	if (!mulExpr)
		throw new CDesertException(_T("ClAddExpr::Eval(ClContext& c): mulExpr is NULL!"));

	ClData left;
	try{
		left = mulExpr->Eval(c);
	}
	catch (CDesertNoneElementException *e)
	{
		if(next)
		{
			e->Delete();
			left = CBdd::ArithZero();
		}
		else 
			throw e;
	}

	return  next ? next->Eval( c, left, addOp ) : left;
}

ClData ClAddExpr::Eval(ClContext& c, const ClData& preVal, AddOp preOp) const
{
	//ASSERT_EX( mulExpr, _T("ClAddExpr::Eval"), _T("mulExpr is null") );
	if (!mulExpr)
		throw new CDesertException(_T("ClAddExpr::Eval(ClContext& c, const ClData& preVal, AddOp preOp): mulExpr is NULL!"));
	
	ClData pre_val = preVal;
	if(preOp != noOp && (ClDataType)pre_val == Cosmic)
	{
		pre_val = ((const CCosmic *)pre_val)->Eval();
	}

	ClData right;
	try{
		right = mulExpr->Eval(c);
	}
	catch (CDesertNoneElementException *e)
	{
		e->Delete();
		right = CBdd::ArithZero();
	}

	ClData left = Eval(pre_val, preOp, right);

	if (preOp != noOp && (ClDataType)left == Cosmic)
		left = ((const CCosmic *)left)->Eval();
	return  next ? next->Eval( c, left, addOp ) : left;
}

ClData ClAddExpr::Eval(const ClData& left, AddOp op, const ClData& right) const
{
	 if((ClDataType)left==Boolean || (ClDataType)right==Boolean)
	 {
		throw new CDesertException(_T("ClAddExpr::Eval(): invalid operands for ADDOP"));
		return ClData();
	 }
	ClData lBdd, rBdd;
	if ( (ClDataType)left == (ClDataType)right )
	{
		switch ((ClDataType)left)
		{
		case Integer:
		  return Eval((int)left, op, (int)right);
		case Double:
		  return Eval((double)left, op, (double)right);
		case Cosmic:
			lBdd = ((const CCosmic *)left)->Eval();
			rBdd = ((const CCosmic *)right)->Eval();
			if((ClDataType)lBdd==Cosmic && (ClDataType)rBdd==Cosmic)
				return ClData((CCosmic *)lBdd);
			else if((ClDataType)lBdd==Bdd && (ClDataType)rBdd==Bdd)
				return Eval((const CBdd&)lBdd, op,  (const CBdd&)rBdd);
			else
				throw new CDesertException(_T("ClAddExpr::Eval(): invalid operands for ADDOP"));
				return ClData();
		case Bdd:
			return Eval((const CBdd&)left, op, (const CBdd&)right);
		default:
		  throw new CDesertException(_T("ClAddExpr::Eval(): invalid operands for ADDOP"));
		  return ClData();
		}
	}
	else if ((ClDataType)left == Cosmic)
	{
		lBdd = ((const CCosmic *)left)->Eval();
		switch ((ClDataType)right)
		{
		case Integer:	
			if((ClDataType)lBdd==Cosmic)
				return ClData((CCosmic *)lBdd);
			else if((ClDataType)lBdd==Bdd)
				return Eval((const CBdd&)lBdd, op, (int)right);
			else
				throw new CDesertException(_T("ClAddExpr::Eval(): invalid operands for ADDOP"));
				return ClData();
		case Double:
			if((ClDataType)lBdd==Cosmic)
				return ClData((CCosmic *)lBdd);
			else if((ClDataType)lBdd==Bdd)
#ifndef DOUBLE_MTBDD
				return Eval((const CBdd&)lBdd, op, (int)(double)right);
#else
				return Eval((const CBdd&)lBdd, op, (double)right);
#endif
			else
				throw new CDesertException(_T("ClAddExpr::Eval(): invalid operands for ADDOP"));
				return ClData();
		case Bdd:			
			if((ClDataType)lBdd==Cosmic)
				return ClData((CCosmic *)lBdd);
			else if((ClDataType)lBdd==Bdd)
				return Eval((const CBdd&)lBdd, op, (const CBdd&)right);
			else
				throw new CDesertException(_T("ClAddExpr::Eval(): invalid operands for ADDOP"));
				return ClData();
		default:
			throw new CDesertException(_T("ClAddExpr::Eval(): invalid operands for a ADDOP"));
			return ClData();
		}
	}
	else if ((ClDataType)right == Cosmic)
	{
		rBdd = ((const CCosmic *)right)->Eval();
		switch ((ClDataType)left)
		{
		case Bdd:			
			if((ClDataType)rBdd==Cosmic)
				return ClData((CCosmic *)rBdd);
			else if((ClDataType)rBdd==Bdd)
				return Eval((const CBdd&)left, op, (const CBdd&)rBdd);
			else
				throw new CDesertException(_T("ClAddExpr::Eval(): invalid operands for ADDOP"));
				return ClData();
		case Double:
			if((ClDataType)rBdd==Cosmic)
				return ClData((CCosmic *)rBdd);
			else if((ClDataType)rBdd==Bdd)
#ifndef DOUBLE_MTBDD
				return Eval((int)(double)left, op, (const CBdd&)rBdd);
#else
				return Eval((double)left, op, (const CBdd&)rBdd);
#endif
			else
				throw new CDesertException(_T("ClAddExpr::Eval(): invalid operands for ADDOP"));
				return ClData();
		default:
			throw new CDesertException(_T("ClAddExpr::Eval(): invalid operands for ADDOP"));
			return ClData();
		}
	}
	else if ((ClDataType)right == Bdd)
	{
		rBdd = (const CBdd&)right;
		switch ((ClDataType)left)
		{
		case Bdd:			
			return Eval((const CBdd&)left, op, (const CBdd&)rBdd);		
		case Double:
#ifndef DOUBLE_MTBDD
			return Eval((int)(double)left, op, (const CBdd&)rBdd);
#else
			return Eval((double)left, op, (const CBdd&)rBdd);
#endif
		default:
			throw new CDesertException(_T("ClAddExpr::Eval(): invalid operands for ADDOP"));
			return ClData();
		}
	}
	else if ((ClDataType)right == Double)
	{		
		switch ((ClDataType)left)
		{	
#ifndef DOUBLE_MTBDD
		case Bdd:	
			return Eval((const CBdd&)left, op, (int)(double)right);		
		case Double:
			return Eval((int)(double)left, op, (int)(double)right);
#else
		case Bdd:	
			return Eval((const CBdd&)left, op, (double)right);	
		case Double:
			return Eval((double)left, op, (double)right);
#endif
		default:
			throw new CDesertException(_T("ClAddExpr::Eval(): invalid operands for ADDOP"));
			return ClData();
		}
	}
	else
	{	
		throw new CDesertException(_T("ClAddExpr::Eval(): a AddOP b : a & b not of the same type"));
		return ClData(false);
	}
}

ClData ClAddExpr::Eval(const CCosmic *left,  AddOp op, const CCosmic *right) const
{
	if (op == noOp)
		throw new CDesertException(_T("ClAddExpr::Eval(const CCosmic *left, AddOp op, const CCosmic *right): invalid operator!"));

	if (!left || !right)
	{
		// tbd skn - rel expr between two cosmic objects, either of which may be
		// null - return a bdd of one instead of farting out
		// careful - this might lead to unwarranted side effects
		// c.SetError(true);

		//if manager is not instantaniated yet, then we are in the stage of verifying constraints
		CBdd one_bdd;
		try
		{
			one_bdd = CBdd::One();
		}
		catch (CDesertException *e)
		{
			//catche the fatal BDD exception here and generate a non-fatal exception
			e->Delete();			
			throw new CDesertException(false, _T("ClAddExpr::Eval(const CCosmic *left, AddOp op, const CCosmic *right): At least one of the operands does not exists!"));
		}

		return ClData(one_bdd);
	}

	return left->Eval(op, right);
}

ClData ClAddExpr::Eval(const CBdd& left, AddOp op, const CBdd& right) const
{
	//ASSERT_EX( op != noOp, _T("ClLExpr::Eval"), _T("invalid operator") );
	if (op == noOp)
		throw new CDesertException(_T("ClAddExpr::Eval(const CBdd& left, AddOp op, const CBdd& right) const : invalid operator!"));

	switch(op)
	{
	case plusOp:
		return ClData(left + right);
	case minusOp:
		return ClData(left - right);
	}
	return ClData(CBdd::Zero());
}

#ifndef DOUBLE_MTBDD
ClData ClAddExpr::Eval(const CBdd& left, AddOp op, int right) const
#else
ClData ClAddExpr::Eval(const CBdd& left, AddOp op, double right) const
#endif
{
	//ASSERT_EX( op != noOp, _T("ClLExpr::Eval"), _T("invalid operator") );
	if (op == noOp)
		throw new CDesertException(_T("ClAddExpr::Eval(const CBdd& left, AddOp op, int right) const : invalid operator!"));

	switch(op)
	{
	case plusOp:
		return ClData(left + right);
	case minusOp:
		return ClData(left - right);
	}
	return ClData(CBdd::Zero());
}

#ifndef DOUBLE_MTBDD
ClData ClAddExpr::Eval(int left, AddOp op, const CBdd& right) const
#else
ClData ClAddExpr::Eval(double left, AddOp op, const CBdd& right) const
#endif
{
	//ASSERT_EX( op != noOp, _T("ClLExpr::Eval"), _T("invalid operator") );
	if (op == noOp)
		throw new CDesertException(_T("ClAddExpr::Eval(int left, AddOp op, const CBdd& right) const : invalid operator!"));

	switch(op)
	{
	case plusOp:
		return ClData(right + left);
	case minusOp:
		{
			CBdd l = CBdd::EncodeArith(left);
			return ClData(l-right);
		}
	}
	return ClData(CBdd::Zero());
}

#ifndef DOUBLE_MTBDD
int ClAddExpr::Eval(int left, AddOp op, int right) const
#else
double ClAddExpr::Eval(double left, AddOp op, double right) const
#endif
{
	//ASSERT_EX( op != noOp, _T("ClAddExpr::Eval"), _T("invalid operator") );
	if (op == noOp)
		throw new CDesertException(_T("ClAddExpr::Eval(int left, AddOp op, int right): invalid operator!"));

	switch(op)
	{
	case plusOp:
		return left + right;
	case minusOp:
		return left - right;
	default:
		return 0;
	}
}

ClData ClAddExpr::Eval(const CBdd& left,  AddOp op, const CCosmic *right) const
{
	if (op == noOp)
		throw new CDesertException(_T("ClAddExpr::Eval(const CBdd& left, AddOp op, const CCosmic *right): invalid operator!"));
	
	if (!right)
	{
		CBdd one_bdd;
		try
		{
			one_bdd = CBdd::One();
		}
		catch (CDesertException *e)
		{
			//catche the fatal BDD exception here and generate a non-fatal exception
			e->Delete();			
			throw new CDesertException(false, _T("ClAddExpr::Eval(const CBdd& left, AddOp op, const CCosmic *right):right operands does not exists!"));
		}

		return ClData(one_bdd);
	}

	return right->Eval(op, left);
}

ClData ClMulExpr::Eval(ClContext& c) const
{
	//ASSERT_EX( unaExpr, _T("ClMulExpr::Eval"), _T("unaExpr not null") );

	if (!unaExpr)
		throw new CDesertException(_T("ClMulExpr::Eval(ClContext& c): unaExpr is NULL!"));

	ClData left;
	try{
		left = unaExpr->Eval(c);
	}
	catch (CDesertNoneElementException *e)
	{
		if(next)
		{
			e->Delete();
			left = CBdd::ArithZero();
		}
		else
			throw e;
	}
	return next ? next->Eval(c, left, mulOp) : left;
}

ClData ClMulExpr::Eval(ClContext& c, const ClData& preVal, MulOp preOp) const
{
//	ASSERT_EX( unaExpr, _T("ClMulExpr::Eval"), _T("unaExpr not null") );
	if (!unaExpr)
		throw new CDesertException(_T("ClMulExpr::Eval(ClContext& c): unaExpr is NULL!"));

	ClData pre_val = preVal;
	if(preOp != noOp && (ClDataType)pre_val == Cosmic)
	{
		pre_val = ((const CCosmic *)pre_val)->Eval();
	}

	ClData right;
	try{
		right = unaExpr->Eval(c);
	}
	catch (CDesertNoneElementException *e)
	{
		e->Delete();
		if(preOp == perOp)
			right = CBdd::ArithOne();
		else
			right = CBdd::ArithZero();
	} 

	ClData left = Eval(pre_val, preOp, right);
	if(preOp != noOp && (ClDataType)left == Cosmic)
	{
		left = ((const CCosmic *)left)->Eval();
	}

	return next ? next->Eval(c, left, mulOp) : left;
}

ClData ClMulExpr::Eval(const ClData& left, MulOp op, const ClData& right) const
{
	if((ClDataType)left==Boolean || (ClDataType)right==Boolean)
	 {
		throw new CDesertException(_T("ClMulExpr::Eval(): invalid operands for MULOP"));
		return ClData();
	 }
	ClData lBdd, rBdd;
	if ( (ClDataType)left == (ClDataType)right )
	{
		switch ((ClDataType)left)
		{
		case Integer:
		  return Eval((int)left, op, (int)right);
		case Double:
		  return Eval((double)left, op, (double)right);
		case Cosmic:
			lBdd = ((const CCosmic *)left)->Eval();
			rBdd = ((const CCosmic *)right)->Eval();
			if((ClDataType)lBdd==Cosmic && (ClDataType)rBdd==Cosmic)
				return ClData((CCosmic *)lBdd);
			else if((ClDataType)lBdd==Bdd && (ClDataType)rBdd==Bdd)
				return Eval((const CBdd&)lBdd, op,  (const CBdd&)rBdd);
			else
				throw new CDesertException(_T("ClMulExpr::Eval(): invalid operands for MULOP"));
				return ClData();
		case Bdd:
			return Eval((const CBdd&)left, op, (const CBdd&)right);
		default:
		  throw new CDesertException(_T("ClMulExpr::Eval(): invalid operands for MULOP"));
		  return ClData();
		}
	}
	else if ((ClDataType)left == Cosmic)
	{
		lBdd = ((const CCosmic *)left)->Eval();
		switch ((ClDataType)right)
		{
		case Integer:	
			if((ClDataType)lBdd==Cosmic)
				return ClData((CCosmic *)lBdd);
			else if((ClDataType)lBdd==Bdd)
				return Eval((const CBdd&)lBdd, op, (int)right);
			else
				throw new CDesertException(_T("ClMulExpr::Eval(): invalid operands for MULOP"));
				return ClData();
		case Double:
			if((ClDataType)lBdd==Cosmic)
				return ClData((CCosmic *)lBdd);
			else if((ClDataType)lBdd==Bdd)
#ifndef DOUBLE_MTBDD
				return Eval((const CBdd&)lBdd, op, (int)(double)right);
#else
				return Eval((const CBdd&)lBdd, op, (double)right);
#endif
			else
				throw new CDesertException(_T("ClMulExpr::Eval(): invalid operands for MULOP"));
				return ClData();
		case Bdd:			
			if((ClDataType)lBdd==Cosmic)
				return ClData((CCosmic *)lBdd);
			else if((ClDataType)lBdd==Bdd)
				return Eval((const CBdd&)lBdd, op, (const CBdd&)right);
			else
				throw new CDesertException(_T("ClMulExpr::Eval(): invalid operands for MULOP"));
				return ClData();
		default:
			throw new CDesertException(_T("ClMulExpr::Eval(): invalid operands for MULOP"));
			return ClData();
		}
	}
	else if ((ClDataType)right == Cosmic)
	{
		rBdd = ((const CCosmic *)right)->Eval();
		switch ((ClDataType)left)
		{
		case Bdd:			
			if((ClDataType)rBdd==Cosmic)
				return ClData((CCosmic *)rBdd);
			else if((ClDataType)rBdd==Bdd)
				return Eval((const CBdd&)left, op, (const CBdd&)rBdd);
			else
				throw new CDesertException(_T("ClMulExpr::Eval(): invalid operands for MULOP"));
				return ClData();
		case Double:
			if((ClDataType)rBdd==Cosmic)
				return ClData((CCosmic *)rBdd);
			else if((ClDataType)rBdd==Bdd)
#ifndef DOUBLE_MTBDD
				return Eval((int)(double)left, op, (const CBdd&)rBdd);
#else
				return Eval((double)left, op, (const CBdd&)rBdd);
#endif
			else
				throw new CDesertException(_T("ClMulExpr::Eval(): invalid operands for MULOP"));
				return ClData();
		default:
			throw new CDesertException(_T("ClMulExpr::Eval(): invalid operands for MULOP"));
			return ClData();
		}
	}
	else if ((ClDataType)right == Bdd)
	{
		rBdd = (const CBdd&)right;
		switch ((ClDataType)left)
		{
		case Bdd:			
			return Eval((const CBdd&)left, op, (const CBdd&)rBdd);		
		case Double:
#ifndef DOUBLE_MTBDD
			return Eval((int)(double)left, op, (const CBdd&)rBdd);
#else
			return Eval((double)left, op, (const CBdd&)rBdd);
#endif
		default:
			throw new CDesertException(_T("ClMulExpr::Eval(): invalid operands for MulOP"));
			return ClData();
		}
	}
	else if ((ClDataType)right == Double)
	{		
		switch ((ClDataType)left)
		{
		case Bdd:			
			return Eval((const CBdd&)left, op, (double)right);		
		case Double:
#ifndef DOUBLE_MTBDD
			return Eval((int)(double)left, op, (int)(double)right);
#else
			return Eval((double)left, op, (double)right);
#endif
		default:
			throw new CDesertException(_T("ClMulExpr::Eval(): invalid operands for MulOP"));
			return ClData();
		}
	}
	else
	{	
		throw new CDesertException(_T("ClMulExpr::Eval(): a MulOP b : a & b not of the same type"));
		return ClData(false);
	}
}

#ifndef DOUBLE_MTBDD
int ClMulExpr::Eval(int left, MulOp op, int right) const
#else
double ClMulExpr::Eval(double left, MulOp op, double right) const
#endif
{
	//ASSERT_EX( op != noOp, _T("ClMulExpr::Eval"), _T("invalid operator") );
	if (op == noOp)
		throw new CDesertException(_T("ClMulExpr::Eval(double left, MulOp op, double right): invalid opertor"));
	switch (op)
	{
	case starOp:
		return left*right;
	case perOp:
		{
			if(right!=0) return left/right;
			else return 0;
		}
	default:
		return 0;
	}
}

ClData ClMulExpr::Eval(const CCosmic *left,  MulOp op, const CCosmic *right) const
{
	if (op == noOp)
		throw new CDesertException(_T("ClAddExpr::Eval(const CCosmic *left, AddOp op, const CCosmic *right): invalid operator!"));
	
	if (!left || !right)
	{
		// tbd skn - rel expr between two cosmic objects, either of which may be
		// null - return a bdd of one instead of farting out
		// careful - this might lead to unwarranted side effects
		// c.SetError(true);

		//if manager is not instantaniated yet, then we are in the stage of verifying constraints
		CBdd one_bdd;
		try
		{
			one_bdd = CBdd::One();
		}
		catch (CDesertException *e)
		{
			//catche the fatal BDD exception here and generate a non-fatal exception
			e->Delete();			
			throw new CDesertException(false, _T("ClAddExpr::Eval(const CCosmic *left, AddOp op, const CCosmic *right): At least one of the operands does not exists!"));
		}

		return ClData(one_bdd);
	}

	return left->Eval(op, right);
}

ClData  ClMulExpr::Eval(const CBdd& left, MulOp op, const CCosmic *right) const
{
	if (op == noOp)
		throw new CDesertException(_T("ClMulExpr::Eval(const CBdd& left, AddOp op, const CCosmic *right): invalid operator!"));
	
	if (!right)
	{
		CBdd one_bdd;
		try
		{
			one_bdd = CBdd::One();
		}
		catch (CDesertException *e)
		{
			//catche the fatal BDD exception here and generate a non-fatal exception
			e->Delete();			
			throw new CDesertException(false, _T("ClMulExpr::Eval(const CBdd& left, AddOp op, const CCosmic *right):right operands does not exists!"));
		}

		return ClData(one_bdd);
	}

	return right->Eval(op, left); 
}

ClData ClMulExpr::Eval(const CBdd& left, MulOp op, const CBdd& right) const
{
	//ASSERT_EX( op != noOp, _T("ClLExpr::Eval"), _T("invalid operator") );
	if (op == noOp)
		throw new CDesertException(_T("ClMulExpr::Eval(const CBdd& left, MulOp op, const CBdd& right) const : invalid operator!"));

	switch(op)
	{
	case starOp:
		return ClData(left * right);
	case perOp:
		return ClData(left / right);
	}
	return ClData(CBdd::Zero());
}

#ifndef DOUBLE_MTBDD
ClData ClMulExpr::Eval(const CBdd& left, MulOp op, int right) const
#else
ClData ClMulExpr::Eval(const CBdd& left, MulOp op, double right) const
#endif
{
	//ASSERT_EX( op != noOp, _T("ClLExpr::Eval"), _T("invalid operator") );
	if (op == noOp)
		throw new CDesertException(_T("ClMulExpr::Eval(const CBdd& left, MulOp op, int right) const : invalid operator!"));

	switch(op)
	{
	case starOp:
		return ClData(left * right);
	case perOp:
		return ClData(left / right);
	}
	return ClData(CBdd::Zero());
}

#ifndef DOUBLE_MTBDD
ClData ClMulExpr::Eval(int left, MulOp op, const CBdd& right) const
#else
ClData ClMulExpr::Eval(double left, MulOp op, const CBdd& right) const
#endif
{
 	//ASSERT_EX( op != noOp, _T("ClLExpr::Eval"), _T("invalid operator") );
	if (op == noOp)
		throw new CDesertException(_T("ClMulExpr::Eval(int left, MulOp op, const CBdd& right) : invalid operator!"));

	switch(op)
	{
	case starOp:
		return ClData(right * left);
	case perOp:
		return ((CBdd)right).devides_from(left);
		//throw new CDesertException(_T("ClMulExpr::Eval(int left, MulOp op, const CBdd& right): please rewrite the constraint"));   //bug here
	}
	return ClData(CBdd::Zero());
}

ClData ClUnaExpr::Eval(ClContext& c) const
{
	//ASSERT_EX( postfixExpr, _T("ClUnaExpr::Eval"), _T("postfixExpr is null") );
	if (!postfixExpr)
		throw new CDesertException(_T("ClUnaExpr::Eval(ClContext& c): postfixExpr is NULL!"));
	ClData right = postfixExpr->Eval(c);
	//if ((ClDataType)right == Cosmic)
	//	right = ((const CCosmic *)right)->Eval();
	return unaOp != noOp ? Eval(right) : right;
}

ClData ClUnaExpr::Eval(const ClData& right) const
{
	ClData rightCosmicData;

	switch( (ClDataType)right )
	{
	case Integer:
		return Eval((int)right);
	case Double:
		return Eval((double)right);
	case Boolean:
		return Eval((bool)right);
	case Bdd:
		return Eval((const CBdd&)right);
	case Cosmic:
		rightCosmicData = ((const CCosmic *)right)->Eval();
		if((ClDataType)rightCosmicData == Cosmic)
			return rightCosmicData;
		else if((ClDataType)rightCosmicData == Bdd)
			return Eval((const CBdd&) rightCosmicData);
		else
			throw new CDesertException(_T("ClUnaExpr::Eval(const ClData&): invalid operands for Unary Op"));
	default:
		//Error(_T("ClUnaExpr::Eval"), _T("invalid operands for a Unary Op"));
		throw new CDesertException(false, _T("ClUnaExpr::Eval: invalid operands for a Unary Op"));
		return ClData();
	}
}

double ClUnaExpr::Eval(int right) const
{
	return Eval((double) right);
}

double ClUnaExpr::Eval(double right) const
{
	//ASSERT_EX( unaOp == minOp, _T("ClUnaExpr::Eval"), _T("invalid operator") );
	//fatal error
	if (unaOp == noOp)
		throw new CDesertException(_T("ClUnaExpr::Eval(double right): invalid operator"));


	// Himanshu: 02/20/2012: Checking specifically for the use of notOp with doubles
	//user error
	if (unaOp == notOp)
		throw new CDesertException(false, _T("ClUnaExpr::Eval(double right): invalid '!' operator used."));
	else if(unaOp == minOp)
		return -right;

	// It's not unary operator, so has to be a valid a math function call
	return applyUnaryMathFunc(right);
}

double ClUnaExpr::applyUnaryMathFunc(double right) const
{	
	try{
   switch(unaOp)
   {
   case noOp:
	   throw new CDesertException(false, _T("ClUnaExpr::applyUnaryMathFunc(double): noOp should have already been handled in Eval()."));
   case notOp:
	   throw new CDesertException(false, _T("ClUnaExpr::applyUnaryMathFunc(double): noOp should have already been handled in Eval()."));
   case minOp:
	   throw new CDesertException(false, _T("ClUnaExpr::applyUnaryMathFunc(double): noOp should have already been handled in Eval()."));
   case sinOp:
	   return mtbdd_call_muParser_math_function(right, bdd_sin);
   case cosOp:
	   return mtbdd_call_muParser_math_function(right, bdd_cos);
   case tanOp:
	   return mtbdd_call_muParser_math_function(right, bdd_tan);
   case asinOp:
	   return mtbdd_call_muParser_math_function(right, bdd_asin);
   case acosOp:
	   return mtbdd_call_muParser_math_function(right, bdd_acos);
   case atanOp:
	   return mtbdd_call_muParser_math_function(right, bdd_atan);
   case sinhOp:
	   return mtbdd_call_muParser_math_function(right, bdd_sinh);
   case coshOp:
	  return mtbdd_call_muParser_math_function(right, bdd_cosh);
   case tanhOp:
	   return mtbdd_call_muParser_math_function(right, bdd_tanh);
   case asinhOp:
	   return mtbdd_call_muParser_math_function(right, bdd_asinh);
   case acoshOp:
	   return mtbdd_call_muParser_math_function(right, bdd_acosh);
   case atanhOp:
	   return mtbdd_call_muParser_math_function(right, bdd_atanh);
   case log2Op:
	   if(right<=0)
		   throw new CDesertException(false, _T("log2() cannot take 0 and negative value."));
	   return mtbdd_call_muParser_math_function(right, bdd_log2);
   case log10Op:
	   if(right<=0)
		   throw new CDesertException(false, _T("log10() cannot take 0 and negative value."));
		return mtbdd_call_muParser_math_function(right, bdd_log10);
   case lnOp:
	   if(right<=0)
		   throw new CDesertException(false, _T("ln() cannot take 0 and negative value."));
		return mtbdd_call_muParser_math_function(right, bdd_ln);
   case expOp:
	   return mtbdd_call_muParser_math_function(right, bdd_exp);
   case sqrtOp:
	   if(right<0)
		   throw new CDesertException(false, _T("sqrt cannot take negative value."));
	  return mtbdd_call_muParser_math_function(right, bdd_sqrt);
   case signOp:
	  return mtbdd_call_muParser_math_function(right, bdd_sign);
   case rintOp:
	   return mtbdd_call_muParser_math_function(right, bdd_rint);
   case absOp:
	   return mtbdd_call_muParser_math_function(right, bdd_abs);
   default:
	   throw new CDesertException(_T("ClUnaExpr::applyUnaryMathFunc(double right): invalid math function used."));
   }
	}catch(const muparser_exception &e)
	{
		throw new CDesertException(CString(e.what()));
	}
}

bool ClUnaExpr::Eval(bool right) const
{
	//ASSERT_EX( unaOp == notOp, _T("ClUnaExpr::Eval"), _T("invalid operator") );
	//fatal error!
	if (unaOp == noOp)
		throw new CDesertException(_T("ClUnaExpr::Eval(bool right): invalid operator"));

	//user error
	if (unaOp != notOp)
		throw new CDesertException(false, _T("ClUnaExpr::Eval(bool right): invalid operator. Only '!' operator is supported unary operator for bools."));

	return !right;
}

CBdd ClUnaExpr::Eval(const CBdd& right) const
{
	//ASSERT_EX( unaOp == notOp, _T("ClUnaExpr::Eval"), _T("invalid operator") );
	
	//fatal error
	if (unaOp == noOp)
		throw new CDesertException(_T("ClUnaExpr::Eval(const CBdd& right): invalid operator"));

	if (unaOp == notOp)
		return !right;

	if (unaOp == minOp) {
		CBdd zeroBDD = CBdd::ArithZero();
		return zeroBDD - right;
	}

	// Himanshu: Math function implementation below
   try{
	switch(unaOp)
   {
   case sinOp:
		return ClData(right.ApplyUnaryMathFunc(bdd_unary_math_function::bdd_sin));
   case cosOp:
		return ClData(right.ApplyUnaryMathFunc(bdd_unary_math_function::bdd_cos));
   case tanOp:
		return ClData(right.ApplyUnaryMathFunc(bdd_unary_math_function::bdd_tan));
   case asinOp:
		return ClData(right.ApplyUnaryMathFunc(bdd_unary_math_function::bdd_asin));
   case acosOp:
		return ClData(right.ApplyUnaryMathFunc(bdd_unary_math_function::bdd_acos));
   case atanOp:
		return ClData(right.ApplyUnaryMathFunc(bdd_unary_math_function::bdd_atan));
   case sinhOp:
		return ClData(right.ApplyUnaryMathFunc(bdd_unary_math_function::bdd_sinh));
   case coshOp:
		return ClData(right.ApplyUnaryMathFunc(bdd_unary_math_function::bdd_cosh));
   case tanhOp:
		return ClData(right.ApplyUnaryMathFunc(bdd_unary_math_function::bdd_tanh));
   case asinhOp:
		return ClData(right.ApplyUnaryMathFunc(bdd_unary_math_function::bdd_asinh));
   case acoshOp:
		return ClData(right.ApplyUnaryMathFunc(bdd_unary_math_function::bdd_acosh));
   case atanhOp:
		return ClData(right.ApplyUnaryMathFunc(bdd_unary_math_function::bdd_atanh));
   case log2Op:
		return ClData(right.ApplyUnaryMathFunc(bdd_unary_math_function::bdd_log2));
   case log10Op:
		return ClData(right.ApplyUnaryMathFunc(bdd_unary_math_function::bdd_log10));
   case lnOp:
		return ClData(right.ApplyUnaryMathFunc(bdd_unary_math_function::bdd_ln));
   case expOp:
		return ClData(right.ApplyUnaryMathFunc(bdd_unary_math_function::bdd_exp));
   case sqrtOp:
		return ClData(right.ApplyUnaryMathFunc(bdd_unary_math_function::bdd_sqrt));
   case signOp:
		return ClData(right.ApplyUnaryMathFunc(bdd_unary_math_function::bdd_sign));
   case rintOp:
		return ClData(right.ApplyUnaryMathFunc(bdd_unary_math_function::bdd_rint));
   case absOp:
		return ClData(right.ApplyUnaryMathFunc(bdd_unary_math_function::bdd_abs));
   default:
	   throw new CDesertException(_T("ClUnaExpr::Eval(const CBdd&): Invalid math function used."));
	}
   }
   catch (const muparser_exception &e)
   {
	   throw new CDesertException(CString(e.what()));
   }
}

ClData ClPostfixExpr::Eval(ClContext& c) const
{
//	ASSERT_EX( primExpr, _T("ClPostfixExpr::Eval"), _T("primExpr is null") );

	if (!primExpr)
		throw new CDesertException(_T("ClPostfixExpr::Eval(ClContext& c): primExpr is NULL!"));

	ClData caller = primExpr->Eval(c);
	if (call)
	{
		ClContext newContext(c);
		SWITCH_CONTEXT(newContext, caller);
		caller = call->Eval(newContext);
		RESTORE_CONTEXT(c, newContext);
	}
	return caller;
}

ClData ClPrimExpr::Eval(ClContext& c) const
{
	switch(type)
	{
	case litCollType:
		//ASSERT_EX( expr.litColl, _T("ClPrimExpr::Eval"), _T("expr.litColl is null") );
		if (!expr.litColl)
			throw new CDesertException(_T("ClPrimExpr::Eval(ClContext& c): expr.litColl is NULL!"));

		// Error( _T("ClPrimExpr::litColl"), _T("<%s:%d> litColl not implemented"), __FILE__, __LINE__);
		return ClData();


	case litType:
		//ASSERT_EX( expr.lit, _T("ClPrimExpr::Eval"), _T("expr.lit is null") );
		if (!expr.lit)
			throw new CDesertException(_T("ClPrimExpr::Eval(ClContext& c): expr.lit is NULL!"));

		return expr.lit->Eval(c);

	case objType:
		//ASSERT_EX( expr.objCall.objName, _T("ClPrimExpr::Eval"), _T("expr.objCall.objName is null") );
		if (!expr.lit)
			throw new CDesertException(_T("ClPrimExpr::Eval(ClContext& c): expr.lit is NULL!"));

		return expr.objCall.objName->Eval(c);

	case objCallType:
		//ASSERT_EX( expr.objCall.objName && expr.objCall.callPars, _T("ClPrimExpr::Eval"),
		//		   _T("expr.objCall.objName | expr.objCall.callPars is null") );
		if (!expr.objCall.objName || !expr.objCall.callPars)
			throw new CDesertException(_T("ClPrimExpr::Eval(ClContext& c): either expr.objCall.objName and/or expr.objCall.callPars is/are NULL!"));


		return expr.objCall.objName->Eval(c, expr.objCall.callPars);

	case exprType:
		//ASSERT_EX( expr.expr, _T("ClPrimExpr::Eval"), _T("expr.expr is null") );

		if (!expr.expr)
			throw new CDesertException(_T("ClPrimExpr::Eval(ClContext& c):  expr.expr is NULL!"));

		return expr.expr->Eval(c);

	case ifExprType:
		//ASSERT_EX( expr.ifExpr, _T("ClPrimExpr::Eval"), _T("expr.ifExpr is null") );
		if (!expr.ifExpr)
			throw new CDesertException(_T("ClPrimExpr::Eval(ClContext& c):  expr.ifExpr is NULL!"));

		return expr.ifExpr->Eval(c);

	default:
		return ClData();
	}
}

ClData ClPostfixCall::Eval(ClContext& c) const
{
//	ASSERT_EX( call, _T("ClPostfixCall::Eval"), _T("call is null") );
	if (!call)
		throw new CDesertException(_T("ClPostfixCall::Eval(ClContext& c):  call is NULL!"));

	//ASSERT_EX( callOp != noOp, _T("ClPostfixCall::Eval"), _T("invalid operator") );
	if (callOp == noOp)
		throw new CDesertException(_T("ClPostfixCall::Eval(ClContext& c):  invalid operator!"));

	CString buf; 
	switch (callOp)
	{
	case objCallOp:
		if ( (ClDataType)c == Cosmic ) break;
		buf.Format(_T("ClPostfixCall::Eval(): a DOT b : a is not an object"));
		c.SetError(true, buf);
		//throw new CDesertException(false, buf);
		return ClData();
	case collCallOp:
		if ( (ClDataType)c == CosmicList ) break;
		buf.Format(_T("ClPostfixCall::Eval(): a ARROW b : a is not a list"));
		c.SetError(true, buf);
		//throw new CDesertException(false, buf);
		return ClData();
	case stdCallOp:
		buf.Format( _T("ClPostfixCall::Eval <%s:%d> operation not implemented"), _T(__FILE__), __LINE__);
		c.SetError(true, buf);
		//throw new CDesertException(false, buf);
		return ClData();
	}
	ClData caller = call->Eval(c);
	if (next)
	{
		ClContext newContext(c);
		SWITCH_CONTEXT(newContext, caller);
		caller = next->Eval(newContext);
		RESTORE_CONTEXT(c, newContext);
	}
	return caller;
}

ClData ClLit::Eval(ClContext& c) const
{
	//ASSERT_EX( type == strType || num, _T("ClLit::Eval"), _T("num is null") );
	if (type != strType && !num)
		throw new CDesertException(_T("Eval(ClContext& c): num is NULL!"));
	return type == strType ? ClData(str) : ClData(num->Eval(c));
}

ClData ClCall::Eval(ClContext& c) const
{
	ClFunction *fn = ClFunction::Find(name);
	if (fn) return fn->Eval(c, pars);
	CString buf; buf.Format(_T("ClCall::Eval(): function <%s> not found"), name);
	c.SetError(true, buf);
	throw new CDesertException(false, buf);
	return ClData();
}

ClData ClObjName::Eval(ClContext& c) const
{
	return selfP ? c.Eval() : c.Eval(objName);
}

ClData ClObjName::Eval(ClContext& c, ClCallPars* pars) const
{
//	ASSERT_EX( !selfP, _T("ClObjName::Eval"), _T("objcall is self") );
	if (selfP)
		throw new CDesertException(_T("ClObjName::Eval(): objcall is self"));
	ClFunction *fn = ClFunction::Find(objName);
	if (fn) 
		return fn->Eval(c, pars);
	else
	{
		
	}
	CString buf; buf.Format(_T("ClCall::Eval : function <%s> not found"), objName);
	c.SetError(true, buf);
	throw new CDesertException(false, buf);
	return ClData();
}

ClData ClNumber::Eval(ClContext& c) const
{
	return type == intType ? value.i : value.d;
}
