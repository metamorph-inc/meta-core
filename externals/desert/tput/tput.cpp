// tput.cpp : Defines the entry point for the DLL application.
//


#include "stdafx.h"



//DESERT includes

#include "desert.h"
#include "CSetErrDialog.h"
#include "core\cosmic.h"
#include "core\clmain.h"
#include "core\static.h"
#include "core\dynamic.h"
#include "core\manager.h"
#include "core\bddmain.h"
#include "core\digraph.h"


//this DLL 
#include "tput.h"

/*
BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
    }
    return TRUE;
}
*/





// This is an example of an exported variable


TPUT_API CBdd DoPathTput(int& maxSize, int limit,  CDynElement *p_this)
{
	CDynElementList& children = p_this->GetChildren();
	CDynRelationList& relations = p_this->GetRelations();

  CDiGraph graph;

  {
                                       // create nodes in graph
    POSITION pos = children.GetHeadPosition();
    while(pos) graph.CreateNode( children.GetNext(pos) );
  }
  
  {                                     // connect nodes in graph
    POSITION pos = relations.GetHeadPosition();
    while(pos)
    {
      CDynRelation *rel = relations.GetNext(pos);
      graph.ConnectNodes( rel->GetSource(), rel->GetDestination() );
    }
  }

  CBdd ret = CBdd::ArithZero();
  {
    int numPaths = graph.FindAllPaths(); // have to find all paths first
    int fullPath = graph.GetFullPath();  // then find if there is a path containing all nodes
    int begin = fullPath < 0 ? 0 : fullPath; // if there is (fullPath >= 0) then begin looping there
    int end = fullPath < 0 ? numPaths : fullPath+1; // and also end looping there

    for (int i=begin; i<end; i++)
    {
      int sz;
      CDynElementList l1;
        
      // get the path with idx i
      graph.GetPath(i, l1);

      // invoke extend tput expression on the nodes (componennts) on this path
      CBdd r = CBdd::ArithZero();
      POSITION pos = l1.GetHeadPosition();
      while(pos)
      {
        //CBdd tmp = l1.GetNext(pos)->Tput_0(maxSize, limit);
		  CBdd tmp = PCM_Eval_tput_0(maxSize, limit, l1.GetNext(pos));

		// printf("%s: a_sz=%d b_sz=%d\n",(const TCHAR *)(*this), tmp.size(), r.size());
		  r = CBdd::Min(r,tmp);
        sz = r.size();
		// printf("a+b_sz=%d\n", sz);
        if (sz > maxSize) maxSize = sz;
      }
        
      // return a bdd that retains the larger path
      ret = CBdd::Sum(ret, r, limit);
      sz = ret.size();
      if (sz > maxSize) maxSize = sz;
    }
  }
  return ret;
}

//CBdd CDynElement::Tput(int limit, ClRelExpr::RelOp op)

TPUT_API CBdd PCM_Eval_tput(int lmt, ClRelExpr::RelOp op, CDynElement *p_this)
{
  CBdd ret = CBdd::One();
  CBdd encoding = p_this->GetEncoding();

#ifndef DOUBLE_MTBDD
  int limit = lmt;
#else
  double limit = lmt;
#endif
  //// if limit is greater than the max sum possible than do nothing
  //if (CManager::theInstance->CheckTputLimit(limit))
  //{ 
    // use mtbdd's to compose tput property
    maxLatBddSz=0;
    //ret = Tput_0(maxLatBddSz, limit);
	ret = PCM_Eval_tput_0(maxLatBddSz, limit, p_this);

    switch (op)
	{
		//equals to relation operator
		case ClRelExpr::eqOp:
		{
			ret = ret.equals_to(limit);
			int sz = ret.size();
			ret = CBdd::Implies(encoding, ret);    // conditionalize the property with our encoding
			return ret;
		}

		//not equals to relation operator
		case ClRelExpr::neOp:
		{
			ret = ret.not_equals_to(limit);
			int sz = ret.size();
			ret = CBdd::Implies(encoding, ret);    // conditionalize the property with our encoding
			return ret;
		}
		//less then or equal relation operator
		case ClRelExpr::leOp:
			limit++;
		//less then relation operator
		case ClRelExpr::ltOp:
		{
			ret = ret.threshold(limit);
			int sz = ret.size();
			ret = CBdd::Implies(encoding, ret);    // conditionalize the property with our encoding
			return ret;
		}

		//greater then or equal relation operator
		case ClRelExpr::geOp:
			limit--;
		//greater then relation operator
		case ClRelExpr::gtOp:
		{
			ret = ret.low_threshold(limit);
			int sz = ret.size();
			ret = CBdd::Implies(encoding, ret);    // conditionalize the property with our encoding
			return ret;
		}

		//ret = ret.threshold(limit);
		//int sz = ret.size();
		//if (sz > maxLatBddSz) maxLatBddSz = sz;
  }

  //ret = CBdd::Implies(encoding, ret);    // conditionalize the tput with our encoding

  return ret;
}


static CMap<CDynElement *, CDynElement*, CBdd, CBdd> tputBdd_cache;


TPUT_API CBdd PCM_Eval_tput_0(int& maxSize, int limit, CDynElement * p_this)
{
  CDynProperty *prop;
  int propVal = 0;
  POSITION pos;
  CBdd ret;


  CBdd encoding = p_this->GetEncoding();
  CBdd baseEnc = p_this->GetBaseEncoding();

  const CDynElementList& children = p_this->GetChildren();  
  const CDynElement * parent = p_this->GetParent();

  if (tputBdd_cache.Lookup(p_this, ret)) return ret;
	
/*
  if (tputDone)
    return tputBdd;
*/
  switch((CDecomposition)*p_this)
  {
  case decompAnd:
    ret = DoPathTput(maxSize, limit, p_this);
    break;

  case decompOr:
    ret = CBdd::Zero();
    pos = children.GetHeadPosition();
    while(pos)
      //ret = ret || children.GetNext(pos)->Tput_0(maxSize, limit);
	  ret = ret || PCM_Eval_tput_0(maxSize, limit,  children.GetNext(pos));

    break;

  case decompLeaf:
    prop = ((CDynElement*)p_this)->FindProperty(_T("tput"));
   // ret = prop ? prop->MaptoBdd( encoding ) : CBdd::One();
	 ret = prop ? prop->MapValuetoBdd( encoding ) : CBdd::One();
    break;
  }

  /*
   * if parent is OR decomposed, then
   * append the orthogonality factor to the child
   */
  if (parent && (decompOr == (CDecomposition)(*parent)))
    ret = CBdd::Mtbdd_Ite(baseEnc, ret);
/*
  tputBdd = ret;
  tputDone = true;
  */
  
  tputBdd_cache[p_this] = ret;

  int sz = ret.size();
  if (sz > maxSize) maxSize = sz;
  return ret;
}



