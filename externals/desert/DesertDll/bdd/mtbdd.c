/* Basic multi-terminal BDD routines */

#include "bddint.h"
#include <math.h>
#include <tchar.h>

/* mtbdd_transform_closure(bddm, canonical_fn, transform_fn, env) sets */
/* the transformation for MTBDD terminal values for the "negative-output" */
/* pointer flag.  The canonical_fn receives the BDD manager, two longs */
/* representing the input value, and the value of env.  It should return */
/* a non-zero value if the result needs to be transformed.  The */
/* transform_fn receives the BDD manager, two longs (the input value), */
/* pointers to two longs (for the output) and the value of env.  This */
/* should not be called after any MTBDD terminals are created. */
int	mtbdd_threshold_step_count = 0;
int	mtbdd_sum_step_count = 0;
int mtbdd_sum_cache_hit = 0;

double dzero = 0.0000001;

double getEncodingValue(unsigned long u32, unsigned long l32)
{
	union val_type {
		double d;
		unsigned long ul[2];
	} double_union;

	double_union.ul[1] = l32;
	double_union.ul[0] = u32;
	return double_union.d;	
}

void getDecodingValue(double val, INT_PTR *u32, INT_PTR *l32)
{
	//unsigned long long *bits = (unsigned long long*)(&val);
	//*u32 = (unsigned long) (*bits & 0xffffffffL); 
	//*l32 = (unsigned long) (*bits >> 32); 

	union val_type {
		double d;
		unsigned long ul[2];
	} double_union;

	double_union.d = val;
	*u32 = double_union.ul[0];
	*l32 = double_union.ul[1];
}

void
#if defined(__STDC__)
DESERT_API mtbdd_transform_closure(bdd_manager bddm,
			int (*canonical_fn)(bdd_manager, INT_PTR, INT_PTR, pointer),
			void (*transform_fn)(bdd_manager, INT_PTR, INT_PTR, INT_PTR *, INT_PTR *, pointer),
			pointer transform_env)
#else
DESERT_API mtbdd_transform_closure(bddm, canonical_fn, transform_fn, transform_env)
     bdd_manager bddm;
     int (*canonical_fn)();
     void (*transform_fn)();
     pointer transform_env;
#endif
{
  bddm->transform_fn=transform_fn;
  bddm->transform_env=transform_env;
  bddm->canonical_fn=canonical_fn;
}

/* mtbdd_one_data(bddm, value1, value2) sets the MTBDD value for TRUE. */
/* This should not be called after MTBDD terminals have been created. */

int
#if defined(__STDC__)
DESERT_API mtbdd_one_data(bdd_manager bddm, INT_PTR value1, INT_PTR value2)
#else
DESERT_API mtbdd_one_data(bddm, value1, value2)
     bdd_manager bddm;
     INT_PTR value1;
     INT_PTR value2;
#endif
{
  var_table table;
  long hash;

  table=bddm->unique_table.tables[BDD_CONST_INDEXINDEX];
  if (table->entries != 1)
  {
    //bdd_fatal("mtbdd_one_data: other terminal nodes already exist");
	  return 1;
  }
  hash=HASH_NODE(bddm->one->data[0], bddm->one->data[1]);
  REDUCE(hash, table->size);
  table->table[hash]=0;
  bddm->one->data[0]=value1;
  bddm->one->data[1]=value2;
  hash=HASH_NODE(bddm->one->data[0], bddm->one->data[1]);
  REDUCE(hash, table->size);
  table->table[hash]=bddm->one;
  return 0;
}


/* mtbdd_free_terminal_closure(bddm, free_terminal_fn, free_terminal_env) */
/* sets the closure to be invoked on when freeing MTBDD terminals.  If */
/* free_terminal_fn is null, it indicates that no function should be */
/* called.  The free_terminal_fn gets the BDD manager, two longs */
/* holding the data for the terminal, and the value of free_terminal_env. */

void
#if defined(__STDC__)
DESERT_API mtbdd_free_terminal_closure(bdd_manager bddm,
			    void (*free_terminal_fn)(bdd_manager, INT_PTR, INT_PTR, pointer),
			    pointer free_terminal_env)
#else
DESERT_API mtbdd_free_terminal_closure(bddm, free_terminal_fn, free_terminal_env)
     bdd_manager bddm;
     void (*free_terminal_fn)();
     pointer free_terminal_env;
#endif
{
  bddm->unique_table.free_terminal_fn=free_terminal_fn;
  bddm->unique_table.free_terminal_env=free_terminal_env;
}


/* mtbdd_get_terminal(bddm, value1, value2) returns the multi-terminal */
/* BDD for a constant. */

bdd
#if defined(__STDC__)
DESERT_API mtbdd_get_terminal(bdd_manager bddm, INT_PTR value1, INT_PTR value2)
#else
DESERT_API mtbdd_get_terminal(bddm, value1, value2)
     bdd_manager bddm;
     INT_PTR value1;
     INT_PTR value2;
#endif
{
  FIREWALL(bddm);
  RETURN_BDD(bdd_find_terminal(bddm, value1, value2));
}


/* mtbdd_terminal_value(bddm, f, value1, value2) returns the data value */
/* for the terminal node f. */

void
#if defined(__STDC__)
DESERT_API mtbdd_terminal_value(bdd_manager bddm, bdd f, INT_PTR *value1, INT_PTR *value2)
#else
DESERT_API mtbdd_terminal_value(bddm, f, value1, value2)
     bdd_manager bddm;
     bdd f;
     INT_PTR *value1;
     INT_PTR *value2;
#endif
{
  if (bdd_check_arguments(1, f))
    {
      BDD_SETUP(f);
      if (!BDD_IS_CONST(f))
	{
	  bdd_warning(_T("mtbdd_terminal_data: argument is terminal node"));
	  *value1=0;
	  *value2=0;
	  return;
	}
      mtbdd_terminal_value_aux(bddm, f, value1, value2);
    }
}


static
bdd
#if defined(__STDC__)
mtbdd_ite_step(bdd_manager bddm, bdd f, bdd g, bdd h)
#else
mtbdd_ite_step(bddm, f, g, h)
     bdd_manager bddm;
     bdd f;
     bdd g;
     bdd h;
#endif
{
  bdd_indexindex_type top_indexindex;
  bdd f1, f2;
  bdd g1, g2;
  bdd h1, h2;
  bdd temp1, temp2;
  bdd result;

  BDD_SETUP(f);
  BDD_SETUP(g);
  BDD_SETUP(h);
  if (BDD_IS_CONST(f))
    {
      if (f == BDD_ONE(bddm))
	{
	  BDD_TEMP_INCREFS(g);
	  return (g);
	}
      BDD_TEMP_INCREFS(h);
      return (h);
    }
  /* f is not constant. */
  if (g == h)
    {
      BDD_TEMP_INCREFS(g);
      return (g);
    }
  /* f is not constant, g and h are distinct. */
  if (!BDD_IS_OUTPOS(f))
    {
      f=BDD_NOT(f);
      BDD_SWAP(g, h);
    }
  /* f is now an uncomplemented output pointer. */
  if (bdd_lookup_in_cache31(bddm, CACHE_TYPE_ITE, (INT_PTR)f, (INT_PTR)g, (INT_PTR)h, (INT_PTR *)&result))
    return (result);
  BDD_TOP_VAR3(top_indexindex, bddm, f, g, h);
  BDD_COFACTOR(top_indexindex, f, f1, f2);
  BDD_COFACTOR(top_indexindex, g, g1, g2);
  BDD_COFACTOR(top_indexindex, h, h1, h2);
  temp1=mtbdd_ite_step(bddm, f1, g1, h1);
  temp2=mtbdd_ite_step(bddm, f2, g2, h2);
  result=bdd_find(bddm, top_indexindex, temp1, temp2);
  bdd_insert_in_cache31(bddm, CACHE_TYPE_ITE, (INT_PTR)f, (INT_PTR)g, (INT_PTR)h, (INT_PTR)result);
  return (result);
}


/* mtbdd_ite(bddm, f, g, h) returns the BDD for "if f then g else h", */
/* where g and h are multi-terminal BDDs. */

bdd
#if defined(__STDC__)
DESERT_API mtbdd_ite(bdd_manager bddm, bdd f, bdd g, bdd h)
#else
DESERT_API mtbdd_ite(bddm, f, g, h)
     bdd_manager bddm;
     bdd f;
     bdd g;
     bdd h;
#endif
{
  if (bdd_check_arguments(3, f, g, h))
    {
      FIREWALL(bddm);
      RETURN_BDD(mtbdd_ite_step(bddm, f, g, h));
    }
  return ((bdd)0);
}


/* mtbdd_substitute(bddm, f) does the analog of bdd_substitute for MTBDDs. */

bdd
#if defined(__STDC__)
DESERT_API mtbdd_substitute(bdd_manager bddm, bdd f)
#else
DESERT_API mtbdd_substitute(bddm, f)
     bdd_manager bddm;
     bdd f;
#endif
{
  long op;

  if (bdd_check_arguments(1, f))
    {
      FIREWALL(bddm);
      if (bddm->curr_assoc_id == -1)
	op=bddm->temp_op--;
      else
	op=OP_SUBST+bddm->curr_assoc_id;
      RETURN_BDD(bdd_substitute_step(bddm, f, op, mtbdd_ite_step, bddm->curr_assoc));
    }
  return ((bdd)0);
}


static
bdd
#if defined(__STDC__)
mtbdd_equal_step(bdd_manager bddm, bdd f, bdd g)
#else
mtbdd_equal_step(bddm, f, g)
     bdd_manager bddm;
     bdd f;
     bdd g;
#endif
{
  bdd_indexindex_type top_indexindex;
  bdd f1, f2;
  bdd g1, g2;
  bdd temp1, temp2;
  bdd result;

  BDD_SETUP(f);
  BDD_SETUP(g);
  if (f == g)
    return (BDD_ONE(bddm));
  if (BDD_IS_CONST(f) && BDD_IS_CONST(g))
    return (BDD_ZERO(bddm));
  if (BDD_OUT_OF_ORDER(f, g))
    BDD_SWAP(f, g);
  if (bdd_lookup_in_cache2(bddm, OP_EQUAL, f, g, &result))
    return (result);
  BDD_TOP_VAR2(top_indexindex, bddm, f, g);
  BDD_COFACTOR(top_indexindex, f, f1, f2);
  BDD_COFACTOR(top_indexindex, g, g1, g2);
  temp1=mtbdd_equal_step(bddm, f1, g1);
  temp2=mtbdd_equal_step(bddm, f2, g2);
  result=bdd_find(bddm, top_indexindex, temp1, temp2);
  bdd_insert_in_cache2(bddm, OP_EQUAL, f, g, result);
  return (result);
}


/* mtbdd_equal(bddm, f, g) returns a BDD indicating when the */
/* multi-terminal BDDs f and g are equal. */

bdd
#if defined(__STDC__)
DESERT_API mtbdd_equal(bdd_manager bddm, bdd f, bdd g)
#else
DESERT_API mtbdd_equal(bddm, f, g)
     bdd_manager bddm;
     bdd f;
     bdd g;
#endif
{
  if (bdd_check_arguments(2, f, g))
    {
      FIREWALL(bddm);
      RETURN_BDD(mtbdd_equal_step(bddm, f, g));
    }
  return ((bdd)0);
}


/* defines the function mtbdd_sum */

bdd
#if defined(__STDC__)
mtbdd_sum_step(bdd_manager bddm, bdd f, bdd g)
#else
mtbdd_sum_step(bddm, f, g)
  bdd_manager bddm;
  bdd f,g;
#endif
{
  bdd_indexindex_type top_indexindex;
  bdd f1,f2;
  bdd g1,g2;
  bdd temp1,temp2;
  bdd result;

  BDD_SETUP(f);
  BDD_SETUP(g);
 
  /* if one of the nodes is a logical zero, then
     it implies that no integer value is defined for
     that path
     we return the other node, we will consider
     undefined+val = undefined
  */
  if (BDD_IS_CONST(f))
  {
	  /*
			Tests if the node represents the constant true or false or an MTBDD terminal node
	  */
    if (f == BDD_ZERO(bddm))
    {
	  //BDD_TEMP_INCREFS(f); 
      return (f);
    }

    {
      unsigned long v1,v2;
      mtbdd_terminal_value_aux(bddm, f, &v1, &v2);
      /* add with zero? */
      if (v1 == 0 && v2 == 0)
      {
        BDD_TEMP_INCREFS(g);
        return(g);
      }
    }
  }
  if (BDD_IS_CONST(g))
  {
    if (g == BDD_ZERO(bddm))
    {
		/* BDD_TEMP_INCREFS(g); */
      return (g);
    }
    {
       unsigned long v1,v2;
      mtbdd_terminal_value_aux(bddm, g, &v1, &v2);
      /* add with zero? */
      if (v1 == 0 && v2 == 0)
      {
        BDD_TEMP_INCREFS(f);
        return(f);
      }
    }
  }

  /* none is logical/arithmetic zero */
  /* are both mtbdd terminals */
  if (BDD_IS_CONST(f) && BDD_IS_CONST(g))
  {
    unsigned long v1,v2,v3,v4;
	double dv12, dv34;
    /* fart if both constants are not mtbdd terminals */
    if (bdd_type_aux(bddm,f) != BDD_TYPE_CONSTANT ||
        bdd_type_aux(bddm,g) != BDD_TYPE_CONSTANT)
    {
      /* error condition */
   /*   bdd_fatal(" arithmetic sum on boolean terminals ");
      return ((bdd)0);*/
		longjmp(bddm->abort.context, BDD_FATAL);
    }
    
    mtbdd_terminal_value_aux(bddm, f, &v1, &v2);
    mtbdd_terminal_value_aux(bddm, g, &v3, &v4);

	
#ifndef DOUBLE_MTBDD
	/* arithmetic 1 is represented with (0,-1) */
    if (v2 == -1) v2 = -v2;
	if (v4 == -1) v4 = -v4;

    v1 += v3;
	v2 += v4;

	if (v1 == 0 && v2 == 1) v2 = -1;
#else
	dv12 = getEncodingValue(v1, v2);
	dv34 = getEncodingValue(v3, v4);
	dv12 = dv12 + dv34;
	getDecodingValue(dv12, &v1, &v2);
#endif
	result = bdd_find_terminal(bddm, v1, v2);
    {/*
      BDD_SETUP(result);
      BDD_TEMP_INCREFS(result); */
    }
    return (result);
  }

  if (BDD_OUT_OF_ORDER(f, g))
    BDD_SWAP(f, g);
  
  if (bdd_lookup_in_cache2(bddm, OP_SUM, f, g, &result))
  {
	  mtbdd_sum_cache_hit++;
      return (result);
  }

  BDD_TOP_VAR2(top_indexindex, bddm, f, g);
  BDD_COFACTOR(top_indexindex, f, f1, f2);
  BDD_COFACTOR(top_indexindex, g, g1, g2);
  temp1 = mtbdd_sum_step(bddm, f1, g1);
  temp2 = mtbdd_sum_step(bddm, f2, g2);
  result = bdd_find(bddm, top_indexindex, temp1, temp2);
  bdd_insert_in_cache2(bddm, OP_SUM, f, g, result);
  return (result);
}



bdd
#if defined(__STDC__)
DESERT_API mtbdd_sum(bdd_manager bddm, bdd f, bdd g)
#else
DESERT_API mtbdd_sum(bddm, f, g)
  bdd_manager bddm;
  bdd f,g;
#endif
{
	bdd ret;
  if (bdd_check_arguments(2, f, g))
  {
    FIREWALL(bddm);
	if(setjmp((bddm)->abort.context)==0)
	{
		ret = mtbdd_sum_step(bddm, f, g);
		RETURN_BDD(ret);
	}
	else 
		return ((bdd)0);
  }
  return ((bdd)0);
}


/* defines the function mtbdd_sub */
//Added by Feng 2/11/11
bdd
#if defined(__STDC__)
mtbdd_sub_step(bdd_manager bddm, bdd f, bdd g)
#else
mtbdd_sub_step(bddm, f, g)
  bdd_manager bddm;
  bdd f,g;
#endif
{
  bdd_indexindex_type top_indexindex;
  bdd f1,f2;
  bdd g1,g2;
  bdd temp1,temp2;
  bdd result;

  BDD_SETUP(f);
  BDD_SETUP(g);
 
  /* if one of the nodes is a logical zero, then
     it implies that no integer value is defined for
     that path
     we return the other node, we will consider
     undefined+val = undefined
  */
  if (BDD_IS_CONST(f))
  {
	  /*
			Tests if the node represents the constant true or false or an MTBDD terminal node
	  */
    if (f == BDD_ZERO(bddm))
    {
	  //BDD_TEMP_INCREFS(f); 
      return (f);
    }

    //{
    //  long v1,v2;
    //  mtbdd_terminal_value_aux(bddm, f, &v1, &v2);
    //  /* add with zero? */
    //  if (v1 == 0 && v2 == 0)
    //  {
    //    BDD_TEMP_INCREFS(g);
    //    return(g);
    //  }
    //}
  }
  if (BDD_IS_CONST(g))
  {
    if (g == BDD_ZERO(bddm))
    {
		/* BDD_TEMP_INCREFS(g); */
      return (g);
    }
    //{
    //  long v1,v2;
    //  mtbdd_terminal_value_aux(bddm, g, &v1, &v2);
    //  /* add with zero? */
    //  if (v1 == 0 && v2 == 0)
    //  {
    //    BDD_TEMP_INCREFS(f);
    //    return(f);
    //  }
    //}
  }

  /* none is logical/arithmetic zero */
  /* are both mtbdd terminals */
  if (BDD_IS_CONST(f) && BDD_IS_CONST(g))
  {
    unsigned long v1,v2,v3,v4;
	double dv12, dv34;
    /* fart if both constants are not mtbdd terminals */
    if (bdd_type_aux(bddm,f) != BDD_TYPE_CONSTANT ||
        bdd_type_aux(bddm,g) != BDD_TYPE_CONSTANT)
    {
      /* error condition */
      //bdd_fatal(" arithmetic sub on boolean terminals ");
      //return ((bdd)0);
		longjmp(bddm->abort.context, BDD_FATAL);
    }
    
    mtbdd_terminal_value_aux(bddm, f, &v1, &v2);
    mtbdd_terminal_value_aux(bddm, g, &v3, &v4);

#ifndef DOUBLE_MTBDD
	/* arithmetic 1 is represented with (0,-1) */
	/* arithmetic -x is represented with (-x,0) */
	if (v2 == -1) v2 = -v2;
	if (v4 == -1) v4 = -v4;

#if  1 // ndef NEGATIVE_MTBDD
    v1 -= v3;
	v2 -= v4;

	if (v1 == 0 && v2 == 1) v2 = -1; //could be potential bug if v2 itself is -1
#else
	{
		long val1, val2, res;
		if (v1>0) val1 = -v1;
		else val1 = v2;
		if (v3>0) val2 = -v3;
		else val2 = v4;

		res = val1 - val2;
		if (res < 0) {	v1 = -res; v2 = 0; }
		else { v1 = 0; v2 = res;}
	
	}
#endif
#else
	dv12 = getEncodingValue(v1, v2);
	dv34 = getEncodingValue(v3, v4);
	dv12 = dv12 - dv34;
	getDecodingValue(dv12, &v1, &v2);
#endif
    result = bdd_find_terminal(bddm, v1, v2);
    return (result);
  }


  /*if (BDD_OUT_OF_ORDER(f, g))
    BDD_SWAP(f, g);*/
  
  if (bdd_lookup_in_cache2(bddm, OP_SUB, f, g, &result))
  {
	  mtbdd_sum_cache_hit++;
      return (result);
  }

  BDD_TOP_VAR2(top_indexindex, bddm, f, g);
  BDD_COFACTOR(top_indexindex, f, f1, f2);
  BDD_COFACTOR(top_indexindex, g, g1, g2);
  temp1 = mtbdd_sub_step(bddm, f1, g1);
  temp2 = mtbdd_sub_step(bddm, f2, g2);
  result = bdd_find(bddm, top_indexindex, temp1, temp2);
  bdd_insert_in_cache2(bddm, OP_SUB, f, g, result);
  return (result);
}



bdd
#if defined(__STDC__)
DESERT_API mtbdd_sub(bdd_manager bddm, bdd f, bdd g)
#else
DESERT_API mtbdd_sub(bddm, f, g)
  bdd_manager bddm;
  bdd f,g;
#endif
{
	bdd ret;
  if (bdd_check_arguments(2, f, g))
  {
	  FIREWALL(bddm);
    if(setjmp((bddm)->abort.context)==0)
	{
		ret = mtbdd_sub_step(bddm, f, g);
		RETURN_BDD(ret);
	}
	else 
		return ((bdd)0);
	  
 //   RETURN_BDD(mtbdd_sub_step(bddm, f, g));
  }
  return ((bdd)0);
}


/* defines the function mtbdd_div between two Bdds*/
//Added by Feng 2/19/11
bdd
#if defined(__STDC__)
mtbdd_div_step(bdd_manager bddm, bdd f, bdd g)
#else
mtbdd_div_step(bddm, f, g)
  bdd_manager bddm;
  bdd f,g;
#endif
{
  bdd_indexindex_type top_indexindex;
  bdd f1,f2;
  bdd g1,g2;
  bdd temp1,temp2;
  bdd result;

  BDD_SETUP(f);
  BDD_SETUP(g);
 
  /* if one of the nodes is a logical zero, then
     it implies that no integer value is defined for
     that path
     we return the other node, we will consider
     undefined+val = undefined
  */
  if (BDD_IS_CONST(f))
  {
	  /*
			Tests if the node represents the constant true or false or an MTBDD terminal node
	  */
    if (f == BDD_ZERO(bddm))
    {
	  //BDD_TEMP_INCREFS(f); 
      return (f);
    }

    //{
    //  long v1,v2;
    //  mtbdd_terminal_value_aux(bddm, f, &v1, &v2);
    //  /* add with zero? */
    //  if (v1 == 0 && v2 == 0)
    //  {
    //    BDD_TEMP_INCREFS(g);
    //    return(g);
    //  }
    //}
  }
  if (BDD_IS_CONST(g))
  {
    if (g == BDD_ZERO(bddm))
    {
		/* BDD_TEMP_INCREFS(g); */
      return (g);
    }
    {
      unsigned long v1,v2;
      mtbdd_terminal_value_aux(bddm, g, &v1, &v2);
      /* add with zero? */
      if (v1 == 0 && v2 == 0)
      {
        BDD_TEMP_INCREFS(f);
        return(f);
      }
    }
  }

  /* none is logical/arithmetic zero */
  /* are both mtbdd terminals */
  if (BDD_IS_CONST(f) && BDD_IS_CONST(g))
  {
    unsigned long v1,v2,v3,v4;
	double dv12, dv34;
    /* fart if both constants are not mtbdd terminals */
    if (bdd_type_aux(bddm,f) != BDD_TYPE_CONSTANT ||
        bdd_type_aux(bddm,g) != BDD_TYPE_CONSTANT)
    {
      /* error condition */
 /*     bdd_fatal(" arithmetic sub on boolean terminals ");
      return ((bdd)0);*/		
	longjmp(bddm->abort.context, BDD_FATAL);
    }
    
    mtbdd_terminal_value_aux(bddm, f, &v1, &v2);
    mtbdd_terminal_value_aux(bddm, g, &v3, &v4);

#ifndef DOUBLE_MTBDD
	/* arithmetic 1 is represented with (0,-1) */

    if (v2 == -1) v2 = -v2;
	if (v4 == -1) v4 = -v4;

    
	if(v4!=0) v2 =  v2 / v4;

	if (v1 == 0 && v2 == 1) v2 = -1; //could be potential bug if v2 itself is -1
#else
	dv12 = getEncodingValue(v1, v2);
	dv34 = getEncodingValue(v3, v4);
	if(dv34!=0)
		dv12 = dv12 / dv34;
	getDecodingValue(dv12, &v1, &v2);
#endif

    result = bdd_find_terminal(bddm, v1, v2);
    {/*
      BDD_SETUP(result);
      BDD_TEMP_INCREFS(result); */
    }
    return (result);
  }


  /*if (BDD_OUT_OF_ORDER(f, g))
    BDD_SWAP(f, g);*/
  
  if (bdd_lookup_in_cache2(bddm, OP_DIV, f, g, &result))
  {
      return (result);
  }

  BDD_TOP_VAR2(top_indexindex, bddm, f, g);
  BDD_COFACTOR(top_indexindex, f, f1, f2);
  BDD_COFACTOR(top_indexindex, g, g1, g2);
  temp1 = mtbdd_div_step(bddm, f1, g1);
  temp2 = mtbdd_div_step(bddm, f2, g2);
  result = bdd_find(bddm, top_indexindex, temp1, temp2);
  bdd_insert_in_cache2(bddm, OP_DIV, f, g, result);
  return (result);
}



bdd
#if defined(__STDC__)
DESERT_API mtbdd_div(bdd_manager bddm, bdd f, bdd g)
#else
DESERT_API mtbdd_div(bddm, f, g)
  bdd_manager bddm;
  bdd f,g;
#endif
{
	bdd ret;
  if (bdd_check_arguments(2, f, g))
  {
    FIREWALL(bddm);
	if(setjmp((bddm)->abort.context)==0)
	{
		ret = mtbdd_div_step(bddm, f, g);
		RETURN_BDD(ret);
	}
	else 
		return ((bdd)0);
  //  RETURN_BDD(mtbdd_div_step(bddm, f, g));
  }
  return ((bdd)0);
}




/* -------------------------------------------------
///defines the function mtbdd_mul 
----------------------------------------------------*/

bdd
#if defined(__STDC__)
mtbdd_mul_step(bdd_manager bddm, bdd f, bdd g)
#else
mtbdd_mul_step(bddm, f, g)
  bdd_manager bddm;
  bdd f,g;
#endif
{
  bdd_indexindex_type top_indexindex;
  bdd f1,f2;
  bdd g1,g2;
  bdd temp1,temp2;
  bdd result;

  BDD_SETUP(f);
  BDD_SETUP(g);
  //mtbdd_sum_step_count++;
  /* if one of the nodes is a logical zero, then
     it implies that no integer value is defined for
     that path
     we return the other node, we will consider
     undefined+val = undefined
  */
  if (BDD_IS_CONST(f))
  {
    if (f == BDD_ZERO(bddm))
    {
		/* BDD_TEMP_INCREFS(f); */
      return (f);
    }
    {
      unsigned long v1,v2;
      mtbdd_terminal_value_aux(bddm, f, &v1, &v2);
      /* add with zero? */
      if (v1 == 0 && v2 == 0)
      {
        BDD_TEMP_INCREFS(g);
        return(g);
      }
    }
  }
  if (BDD_IS_CONST(g))
  {
    if (g == BDD_ZERO(bddm))
    {
		/* BDD_TEMP_INCREFS(g); */
      return (g);
    }
    {
      unsigned long v1,v2;
      mtbdd_terminal_value_aux(bddm, g, &v1, &v2);
      /* add with zero? */
      if (v1 == 0 && v2 == 0)
      {
        BDD_TEMP_INCREFS(f);
        return(f);
      }
    }
  }
  /* none is logical/arithmetic zero */
  /* are both mtbdd terminals */
  if (BDD_IS_CONST(f) && BDD_IS_CONST(g))
  {
    unsigned long v1,v2,v3,v4;
	double dv12, dv34;
    /* fart if both constants are not mtbdd terminals */
    if (bdd_type_aux(bddm,f) != BDD_TYPE_CONSTANT ||
        bdd_type_aux(bddm,g) != BDD_TYPE_CONSTANT)
    {
      /* error condition */
      //bdd_fatal(" arithmetic mul on boolean terminals ");
      //return ((bdd)0);
		longjmp(bddm->abort.context, BDD_FATAL);	
    }
    
    mtbdd_terminal_value_aux(bddm, f, &v1, &v2);
    mtbdd_terminal_value_aux(bddm, g, &v3, &v4);
#ifndef DOUBLE_MTBDD
	/* arithmetic 1 is represented with (0,-1) */

    if (v2 == -1) v2 = -v2;
	if (v4 == -1) v4 = -v4;

    v1 *= v3;
	v2 *= v4;

	if (v1 == 0 && v2 == 1) v2 = -1;
#else
	dv12 = getEncodingValue(v1, v2);
	dv34 = getEncodingValue(v3, v4);
	dv12 = dv12 * dv34;
	getDecodingValue(dv12, &v1, &v2);
#endif
    result = bdd_find_terminal(bddm, v1, v2);
    {/*
      BDD_SETUP(result);
      BDD_TEMP_INCREFS(result); */
    }
    return (result);
  }


  if (BDD_OUT_OF_ORDER(f, g))
    BDD_SWAP(f, g);
  if (bdd_lookup_in_cache2(bddm, OP_MUL, f, g, &result))
  {
	  mtbdd_sum_cache_hit++;
    return (result);
  }
  BDD_TOP_VAR2(top_indexindex, bddm, f, g);
  BDD_COFACTOR(top_indexindex, f, f1, f2);
  BDD_COFACTOR(top_indexindex, g, g1, g2);
  temp1 = mtbdd_mul_step(bddm, f1, g1);
  temp2 = mtbdd_mul_step(bddm, f2, g2);
  result = bdd_find(bddm, top_indexindex, temp1, temp2);
  bdd_insert_in_cache2(bddm, OP_MUL, f, g, result);
  return (result);
}



bdd
#if defined(__STDC__)
DESERT_API mtbdd_mul(bdd_manager bddm, bdd f, bdd g)
#else
DESERT_API mtbdd_mul(bddm, f, g)
  bdd_manager bddm;
  bdd f,g;
#endif
{
	bdd ret;
  if (bdd_check_arguments(2, f, g))
  {
    FIREWALL(bddm);
	if(setjmp((bddm)->abort.context)==0)
	{
		ret = mtbdd_mul_step(bddm, f, g);
		RETURN_BDD(ret);
	}
	else 
		return ((bdd)0);

 //   RETURN_BDD(mtbdd_mul_step(bddm, f, g));
  }
  return ((bdd)0);
}



/* 
	mtbdd thresholding operation 
	basically, less then will be implemented 
	by this	function
*/

bdd
#ifndef DOUBLE_MTBDD
#if defined(__STDC__)
mtbdd_threshold_step(bdd_manager bddm, bdd f, INT_PTR l1, INT_PTR l2, unsigned long op, int eq)
#else
mtbdd_threshold_step(bddm, f, l1, l2, op, eq)
  bdd_manager bddm;
  bdd f;
  INT_PTR l1,l2;
  unsigned long op;
  int eq;
#endif
#else
#if defined(__STDC__)
mtbdd_threshold_step(bdd_manager bddm, bdd f, double l1, double l2, unsigned long op, int eq)
#else
mtbdd_threshold_step(bddm, f, l1, l2, op, eq)
  bdd_manager bddm;
  bdd f;
  double l1,l2;
  unsigned long op;
  int eq;
#endif
#endif
{
  bdd temp1,temp2;
  bdd result;

  BDD_SETUP(f);

  /*
   * dumb implementation
   */
	mtbdd_threshold_step_count++;

  if (BDD_IS_CONST(f))
  {
    if (bdd_type_aux(bddm, f) == BDD_TYPE_CONSTANT)
    {
      /* if mtbdd terminal */
      unsigned long v1,v2;
		double ret_val, diff;
	  mtbdd_terminal_value_aux(bddm, f, &v1, &v2);
	 
#ifndef DOUBLE_MTBDD
	  /* arithmetic 1 is represented as (0,-1) */
	  if (v2 == -1) v2 = -v2; 
	 tmp = v2-l2; 
	  if ((v1 < l1) || ((v1 == l1) && (tmp<0)))
#else
	  ret_val = getEncodingValue(v1, v2);
	  diff = fabs(ret_val-l2);
	  if( ret_val < l2 || (diff<dzero && eq==1) )
#endif
      /* thresholding to less than only */
        result = BDD_ONE(bddm);
      else
        result = BDD_ZERO(bddm);
      {/*
        BDD_SETUP(result);
        BDD_TEMP_INCREFS(result);*/
      }
        return (result);
    }
    
    /*BDD_TEMP_INCREFS(f);*/
    return (f);
  }

  if (op > 0 )
  {
	if (bdd_lookup_in_cache1(bddm, op, f, &result))
		return (result);
  }

  temp1 = mtbdd_threshold_step(bddm, BDD_THEN(f), l1, l2, op, eq);
  temp2 = mtbdd_threshold_step(bddm, BDD_ELSE(f), l1, l2, op, eq);
  result = bdd_find(bddm, BDD_INDEXINDEX(f), temp1, temp2);

  if (op > 0 )
	bdd_insert_in_cache1(bddm, op, f, result);
  
  return (result);
}


bdd
#ifndef	DOUBLE_MTBDD
#if defined(__STDC__)
DESERT_API mtbdd_threshold(bdd_manager bddm, bdd f, INT_PTR l1, INT_PTR l2, unsigned long op_offset, int eq)
#else
DESERT_API mtbdd_threshold(bddm, f, l1, l2, op_offset, eq)
  bdd_manager bddm;
  bdd f;
  INT_PTR l1,l2;
  unsigned long op_offset;
  int eq;
#endif
#else
#if defined(__STDC__)
DESERT_API mtbdd_threshold(bdd_manager bddm, bdd f, double l1, double l2, unsigned long op_offset, int eq)
#else
DESERT_API mtbdd_threshold(bddm, f, l1, l2, op_offset, eq)
  bdd_manager bddm;
  bdd f;
  double l1,l2;
  unsigned long op_offset;
  int eq;
#endif
#endif
{
  if (bdd_check_arguments(1, f))
  {
  
	FIREWALL(bddm);
	if ((op_offset > 0) && (op_offset <   MAX_CACHE_OFFSET))
		RETURN_BDD(mtbdd_threshold_step(bddm, f, l1, l2, OP_THRESH+op_offset, eq));	 
	else
		RETURN_BDD(mtbdd_threshold_step(bddm, f, l1, l2, 0, eq));	 

  }
  return ((bdd)0);
}


/* 
	mtbdd lower_thresholding operation 
	basically, greater then will be implemented 
	by this	function
*/

bdd
#ifndef DOUBLE_MTBDD
#if defined(__STDC__)
mtbdd_low_threshold_step(bdd_manager bddm, bdd f, INT_PTR l1, INT_PTR l2, unsigned long op, int eq)
#else
mtbdd_low_threshold_step(bddm, f, l1, l2, op, eq)
  bdd_manager bddm;
  bdd f;
  INT_PTR l1,l2;
  unsigned long op;
  int eq;
#endif
#else
#if defined(__STDC__)
mtbdd_low_threshold_step(bdd_manager bddm, bdd f, double l1, double l2, unsigned long op, int eq)
#else
mtbdd_low_threshold_step(bddm, f, l1, l2, op, eq)
  bdd_manager bddm;
  bdd f;
  double l1,l2;
  unsigned long op;
  int eq;
#endif
#endif
{
  bdd temp1,temp2;
  bdd result;
  double diff;
  double ret_val;

  BDD_SETUP(f);

  /*
   * dumb implementation
   */
	mtbdd_threshold_step_count++;

  if (BDD_IS_CONST(f))
  {
    if (bdd_type_aux(bddm, f) == BDD_TYPE_CONSTANT)
    {
      /* if mtbdd terminal */
      unsigned long v1,v2;

      mtbdd_terminal_value_aux(bddm, f, &v1, &v2);
#ifndef DOUBLE_MTBDD
	  /* arithmetic 1 is represented as (0,-1) */
	  if (v2 == -1) v2 = -v2;	  
	  tmp = v2-l2;
	  if(tmp > 0 || (tmp==0&&eq==1))
#else
	  ret_val = getEncodingValue(v1, v2);
	  diff = fabs(ret_val-l2);
	  if(ret_val-l2 > dzero || (diff<dzero && eq==1))
#endif
      /* thresholding to greater than only */
      //if ((v1 < l1) || ((v1 == l1) && (tmp>0)))
        result = BDD_ONE(bddm);
      else
        result = BDD_ZERO(bddm);

      {/*
        BDD_SETUP(result);
        BDD_TEMP_INCREFS(result);*/
      }
        return (result);
    }
    
    /*BDD_TEMP_INCREFS(f);*/
    return (f);
  }

  if (op > 0 )
  {
	  if (bdd_lookup_in_cache1(bddm, op, f, &result))
		return (result);
  }

  temp1 = mtbdd_low_threshold_step(bddm, BDD_THEN(f), l1, l2, op,eq);
  temp2 = mtbdd_low_threshold_step(bddm, BDD_ELSE(f), l1, l2, op,eq);
  result = bdd_find(bddm, BDD_INDEXINDEX(f), temp1, temp2);

  if (op > 0 )
	bdd_insert_in_cache1(bddm, op, f, result);
  

  return (result);
}


bdd
#ifndef DOUBLE_MTBDD
#if defined(__STDC__)
DESERT_API mtbdd_low_threshold(bdd_manager bddm, bdd f, INT_PTR l1, INT_PTR l2, unsigned long op_offset, int eq)
#else
DESERT_API mtbdd_low_threshold(bddm, f, l1, l2, op_offset)
  bdd_manager bddm;
  bdd f;
  INT_PTR l1,l2;
  unsigned long op_offset;
  int eq;
#endif
#else
#if defined(__STDC__)
DESERT_API mtbdd_low_threshold(bdd_manager bddm, bdd f, double l1, double l2, unsigned long op_offset, int eq)
#else
DESERT_API mtbdd_low_threshold(bddm, f, l1, l2, op_offset)
  bdd_manager bddm;
  bdd f;
  double l1,l2;
  unsigned long op_offset;
  int eq;
#endif
#endif
{
  if (bdd_check_arguments(1, f))
  {

	  FIREWALL(bddm);
	  if ((op_offset > 0) && (op_offset <   MAX_CACHE_OFFSET))
	  	RETURN_BDD(mtbdd_low_threshold_step(bddm, f, l1, l2, OP_LTHRESH+op_offset, eq));	 
	  else
		RETURN_BDD(mtbdd_low_threshold_step(bddm, f, l1, l2, 0, eq));	 

  }
  return ((bdd)0);
}

/* 
	mtbdd equals_to operation 
	This will set all the terminals not equal to l2  to false
	one bDD operation which returns a bdd
*/

bdd
#ifndef DOUBLE_MTBDD
#if defined(__STDC__)
mtbdd_equals_to_step(bdd_manager bddm, bdd f, INT_PTR l1, INT_PTR l2, unsigned long op)
#else
mtbdd_equals_to_step(bddm, f, l1, l2, op)
  bdd_manager bddm;
  bdd f;
  INT_PTR l1,l2;
  unsigned long op;
#endif
#else
#if defined(__STDC__)
mtbdd_equals_to_step(bdd_manager bddm, bdd f, double l1, double l2, unsigned long op)
#else
mtbdd_equals_to_step(bddm, f, l1, l2, op)
  bdd_manager bddm;
  bdd f;
  double l1,l2;
  unsigned long op;
#endif
#endif
{
  bdd temp1,temp2;
  bdd result;

  BDD_SETUP(f);
  if (BDD_IS_CONST(f))
  {
    if (bdd_type_aux(bddm, f) == BDD_TYPE_CONSTANT)
    {
      /* if mtbdd terminal */
      unsigned long v1,v2;
	  double ret_val, diff;

      mtbdd_terminal_value_aux(bddm, f, &v1, &v2);

#ifndef DOUBLE_MTBDD
	  /* arithmetic 1 is represented as (0,-1) */
	  if (v2 == -1) v2 = -v2;
	  /* limiting to equals to*/
      if ((v1 < l1) || ((v1 == l1) && (v2 == l2)))   // FENG : Check equality constraint
#else
	  ret_val = getEncodingValue(v1, v2);
	  diff = fabs(ret_val - l2);
	  //if(ret_val == l2)    //some precision consideration here: 2.2 vs 2.1999994277954102
	  if(diff < dzero)
#endif  
        result = BDD_ONE(bddm);
      else
        result = BDD_ZERO(bddm);

      {/*
        BDD_SETUP(result);
        BDD_TEMP_INCREFS(result);*/
      }
        return (result);
    }
    
    /*BDD_TEMP_INCREFS(f);*/
    return (f);
  }

  if (op > 0 )
  {
	  if (bdd_lookup_in_cache1(bddm, op, f, &result))
		return (result);
  }


  temp1 = mtbdd_equals_to_step(bddm, BDD_THEN(f), l1, l2, op);
  temp2 = mtbdd_equals_to_step(bddm, BDD_ELSE(f), l1, l2, op);
  result = bdd_find(bddm, BDD_INDEXINDEX(f), temp1, temp2);

  if (op > 0) 
  {
	  bdd_insert_in_cache1(bddm, op, f, result);
  }

  return (result);
}


bdd
#ifndef DOUBLE_MTBDD
#if defined(__STDC__)
DESERT_API mtbdd_equals_to(bdd_manager bddm, bdd f, INT_PTR l1, INT_PTR l2, unsigned long op_offset)
#else
DESERT_API mtbdd_equals_to(bddm, f, l1, l2, op_offset)
  bdd_manager bddm;
  bdd f;
  INT_PTR l1,l2;
  unsigned long op_offset;
#endif
#else
#if defined(__STDC__)
DESERT_API mtbdd_equals_to(bdd_manager bddm, bdd f, double l1, double l2, unsigned long op_offset)
#else
DESERT_API mtbdd_equals_to(bddm, f, l1, l2, op_offset)
  bdd_manager bddm;
  bdd f;
  double l1,l2;
  unsigned long op_offset;
#endif
#endif
{
  if (bdd_check_arguments(1, f))
  {
	  FIREWALL(bddm);
	  if ((op_offset > 0) && (op_offset <   MAX_CACHE_OFFSET))
	  	RETURN_BDD(mtbdd_equals_to_step(bddm, f, l1, l2, OP_EQUALSTO+op_offset ));	 
	  else
		RETURN_BDD(mtbdd_equals_to_step(bddm, f, l1, l2, 0));	 
  }
  return ((bdd)0);
}



/* 
	mtbdd not_equals_to operation 
	This will set all the terminals equal to l2  to false
*/

bdd
#ifndef DOUBLE_MTBDD
#if defined(__STDC__)
mtbdd_not_equals_to_step(bdd_manager bddm, bdd f, INT_PTR l1, INT_PTR l2, unsigned long op)
#else
mtbdd_not_equals_to_step(bddm, f, l1, l2)
  bdd_manager bddm;
  bdd f;
  INT_PTR l1,l2;
#endif
#else
#if defined(__STDC__)
mtbdd_not_equals_to_step(bdd_manager bddm, bdd f, double l1, double l2, unsigned long op)
#else
mtbdd_not_equals_to_step(bddm, f, l1, l2)
  bdd_manager bddm;
  bdd f;
  double l1,l2;
#endif
#endif
{
  bdd temp1,temp2;
  bdd result;

  BDD_SETUP(f);

  if (BDD_IS_CONST(f))
  {
    if (bdd_type_aux(bddm, f) == BDD_TYPE_CONSTANT)
    {
      /* if mtbdd terminal */
      unsigned long v1,v2;
	  double ret_val, diff;
      mtbdd_terminal_value_aux(bddm, f, &v1, &v2);
#ifndef DOUBLE_MTBDD
	  /* arithmetic 1 is represented as (0,-1) */
	  if (v2 == -1) v2 = -v2;
	  
	  /* limiting to not equals to*/
      if ((v1 < l1) || ((v1 == l1) && (v2 != l2)))
#else
	  ret_val = getEncodingValue(v1, v2);
	   diff = fabs(ret_val - l2);
	   if(diff > dzero)
	  //if(ret_val != l2)
#endif
        result = BDD_ONE(bddm);
      else
        result = BDD_ZERO(bddm);

      {/*
        BDD_SETUP(result);
        BDD_TEMP_INCREFS(result);*/
      }
        return (result);
    }
    
    /*BDD_TEMP_INCREFS(f);*/
    return (f);
  }

  if (op > 0 )
  {
		if (bdd_lookup_in_cache1(bddm, op, f, &result))
		return (result);
  }


  temp1 = mtbdd_not_equals_to_step(bddm, BDD_THEN(f), l1, l2, op);
  temp2 = mtbdd_not_equals_to_step(bddm, BDD_ELSE(f), l1, l2, op);
  result = bdd_find(bddm, BDD_INDEXINDEX(f), temp1, temp2);

  if (op > 0) 
  {
	  bdd_insert_in_cache1(bddm, op, f, result);
  }

  return (result);
}


bdd
#ifndef DOUBLE_MTBDD
#if defined(__STDC__)
DESERT_API mtbdd_not_equals_to(bdd_manager bddm, bdd f, INT_PTR l1, INT_PTR l2, unsigned long op_offset)
#else
DESERT_API mtbdd_equals_to(bddm, f, l1, l2, op_offset)
  bdd_manager bddm;
  bdd f;
  INT_PTR l1,l2;
  unsigned long op_offset;
#endif
#else
#if defined(__STDC__)
DESERT_API mtbdd_not_equals_to(bdd_manager bddm, bdd f, double l1, double l2, unsigned long op_offset)
#else
DESERT_API mtbdd_equals_to(bddm, f, l1, l2, op_offset)
  bdd_manager bddm;
  bdd f;
  double l1,l2;
  unsigned long op_offset;
#endif
#endif
{
  if (bdd_check_arguments(1, f))
  {
    FIREWALL(bddm);
	
	if ((op_offset > 0) && (op_offset <   MAX_CACHE_OFFSET))
	  	RETURN_BDD(mtbdd_not_equals_to_step(bddm, f, l1, l2, OP_NEQUALSTO+op_offset ));	 
	  else
		RETURN_BDD(mtbdd_not_equals_to_step(bddm, f, l1, l2, 0));	 
  }
  return ((bdd)0);
}

/*--------------------------------------------------
////// defines the function mtbdd_max 
----------------------------------------------------*/

bdd
#if defined(__STDC__)
DESERT_API mtbdd_max_step(bdd_manager bddm, bdd f, bdd g)
#else
DESERT_API mtbdd_max_step(bddm, f, g)
  bdd_manager bddm;
  bdd f,g;
#endif
{
  bdd_indexindex_type top_indexindex;
  bdd f1,f2;
  bdd g1,g2;
  bdd temp1,temp2;
  bdd result;

  BDD_SETUP(f);
  BDD_SETUP(g);
  
  /* if one of the nodes is a logical zero, then
     it implies that no integer value is defined for
     that path
     we will return undefined
  */
  if (BDD_IS_CONST(f))
  {
    if (f == BDD_ZERO(bddm))
    {
      /* BDD_TEMP_INCREFS(f); */
      return (f);
    }
    {
      unsigned long v1,v2;
      mtbdd_terminal_value_aux(bddm, f, &v1, &v2);
      /* compare with zero? */
      if (v1 == 0 && v2 == 0)
      {
        BDD_TEMP_INCREFS(g);
        return(g);
      }
    }

  }
  if (BDD_IS_CONST(g))
  {
    if (g == BDD_ZERO(bddm))
    {
      /* BDD_TEMP_INCREFS(g); */
      return (g);
    }
    {
      unsigned long v1,v2;
      mtbdd_terminal_value_aux(bddm, g, &v1, &v2);
      /* compare with zero? */
      if (v1 == 0 && v2 == 0)
      {
        BDD_TEMP_INCREFS(f);
        return(f);
      }
    }
  }
  /* none is logical zero */
  /* are both mtbdd terminals */
  if (BDD_IS_CONST(f) && BDD_IS_CONST(g))
  {
    unsigned long v1,v2,v3,v4;

    /* fart if both constants are not mtbdd terminals */
    if (bdd_type_aux(bddm,f) != BDD_TYPE_CONSTANT ||
        bdd_type_aux(bddm,g) != BDD_TYPE_CONSTANT)
    {
      /* error condition */
      //bdd_fatal(" arithmetic max on boolean terminals ");
      //return ((bdd)0);
		longjmp(bddm->abort.context, BDD_FATAL);
    }
    
    mtbdd_terminal_value_aux(bddm, f, &v1, &v2);
    mtbdd_terminal_value_aux(bddm, g, &v3, &v4);

	/* arithmetic 1 is represented as (0,-1) */
	if (v2 == -1) v2 = -v2;
	if (v4 == -1) v4 = -v4;
	  

    if ( (v1 > v3) || ((v1 == v3) &&  (v2 >= v4)) )
    {
      BDD_TEMP_INCREFS(f);
      return (f);
    }

    BDD_TEMP_INCREFS(g);
    return (g);
  }


  if (BDD_OUT_OF_ORDER(f, g))
    BDD_SWAP(f, g);
  if (bdd_lookup_in_cache2(bddm, OP_MAX, f, g, &result))
  {
    return (result);
  }
  BDD_TOP_VAR2(top_indexindex, bddm, f, g);
  BDD_COFACTOR(top_indexindex, f, f1, f2);
  BDD_COFACTOR(top_indexindex, g, g1, g2);
  temp1 = mtbdd_max_step(bddm, f1, g1);
  temp2 = mtbdd_max_step(bddm, f2, g2);
  result = bdd_find(bddm, top_indexindex, temp1, temp2);
  bdd_insert_in_cache2(bddm, OP_MAX, f, g, result);
  return (result);
}



bdd
#if defined(__STDC__)
DESERT_API  mtbdd_max(bdd_manager bddm, bdd f, bdd g)
#else
DESERT_API mtbdd_max(bddm, f, g)
  bdd_manager bddm;
  bdd f,g;
#endif
{
	bdd ret;
  if (bdd_check_arguments(2, f, g))
  {
    FIREWALL(bddm);
	if(setjmp((bddm)->abort.context)==0)
	{
		ret = mtbdd_max_step(bddm, f, g);
		RETURN_BDD(ret);
	}
	else 
		return ((bdd)0);
    //RETURN_BDD(mtbdd_max_step(bddm, f, g));
  }
  return ((bdd)0);
}


/*--------------------------------------------------------
	--------------------------------
	defines the function mtbdd_min 
	--------------------------------
----------------------------------------------------------*/

bdd
#if defined(__STDC__)
mtbdd_min_step(bdd_manager bddm, bdd f, bdd g)
#else
mtbdd_min_step(bddm, f, g)
  bdd_manager bddm;
  bdd f,g;
#endif
{
  bdd_indexindex_type top_indexindex;
  bdd f1,f2;
  bdd g1,g2;
  bdd temp1,temp2;
  bdd result;

  BDD_SETUP(f);
  BDD_SETUP(g);
  
  /* if one of the nodes is a logical zero, then
     it implies that no integer value is defined for
     that path
     we will return undefined
  */
  if (BDD_IS_CONST(f))
  {
    if (f == BDD_ZERO(bddm))
    {
      /* BDD_TEMP_INCREFS(f); */
      return (f);
    }
    {
      unsigned long v1,v2;
      mtbdd_terminal_value_aux(bddm, f, &v1, &v2);
      /* compare with zero? */
      if (v1 == 0 && v2 == 0)
      {
        BDD_TEMP_INCREFS(g);
        return(g);
      }
    }

  }
  if (BDD_IS_CONST(g))
  {
    if (g == BDD_ZERO(bddm))
    {
      /* BDD_TEMP_INCREFS(g); */
      return (g);
    }
    {
      unsigned long v1,v2;
      mtbdd_terminal_value_aux(bddm, g, &v1, &v2);
      /* compare with zero? */
      if (v1 == 0 && v2 == 0)
      {
        BDD_TEMP_INCREFS(f);
        return(f);
      }
    }
  }
  /* none is logical zero */
  /* are both mtbdd terminals */
  if (BDD_IS_CONST(f) && BDD_IS_CONST(g))
  {
    unsigned long v1,v2,v3,v4;

    /* fart if both constants are not mtbdd terminals */
    if (bdd_type_aux(bddm,f) != BDD_TYPE_CONSTANT ||
        bdd_type_aux(bddm,g) != BDD_TYPE_CONSTANT)
    {
      /* error condition */
      //bdd_fatal(" arithmetic max on boolean terminals ");
      //return ((bdd)0);
		longjmp(bddm->abort.context, BDD_FATAL);
    }
    
    mtbdd_terminal_value_aux(bddm, f, &v1, &v2);
    mtbdd_terminal_value_aux(bddm, g, &v3, &v4);
	
	/* arithmetic 1 is represented as (0,-1) */
	if (v2 == -1) v2 = -v2;
	if (v4 == -1) v4 = -v4;
	  


    if ( (v1 > v3) || ((v1 == v3) &&  (v2 <= v4)) )
    {
      BDD_TEMP_INCREFS(f);
      return (f);
    }

    BDD_TEMP_INCREFS(g);
    return (g);
  }


  if (BDD_OUT_OF_ORDER(f, g))
    BDD_SWAP(f, g);
 
  if (bdd_lookup_in_cache2(bddm, OP_MIN, f, g, &result))
  {
    return (result);
  }
  BDD_TOP_VAR2(top_indexindex, bddm, f, g);
  BDD_COFACTOR(top_indexindex, f, f1, f2);
  BDD_COFACTOR(top_indexindex, g, g1, g2);
  temp1 = mtbdd_min_step(bddm, f1, g1);
  temp2 = mtbdd_min_step(bddm, f2, g2);
  result = bdd_find(bddm, top_indexindex, temp1, temp2);
  bdd_insert_in_cache2(bddm, OP_MIN, f, g, result);
  return (result);
}

bdd
#if defined(__STDC__)
DESERT_API mtbdd_min(bdd_manager bddm, bdd f, bdd g)
#else
DESERT_API mtbdd_min(bddm, f, g)
  bdd_manager bddm;
  bdd f,g;
#endif
{
	bdd ret;
  if (bdd_check_arguments(2, f, g))
  {
    FIREWALL(bddm);
	if(setjmp((bddm)->abort.context)==0)
	{
		ret = mtbdd_min_step(bddm, f, g);
		RETURN_BDD(ret);
	}
	else 
		return ((bdd)0);
    //RETURN_BDD(mtbdd_min_step(bddm, f, g));
  }
  return ((bdd)0);
}

/*--------------------------------------------
///defines the function mtbdd_sum with threshold
----------------------------------------------*/
bdd
#ifndef DOUBLE_MTBDD
#if defined(__STDC__)
mtbdd_sum_thresh_step(bdd_manager bddm, bdd f, bdd g, INT_PTR l1, INT_PTR l2, unsigned long op)
#else
mtbdd_sum_thresh_step(bddm, f, g, l1, l2, op)
  bdd_manager bddm;
  bdd f,g;
  INT_PTR l1,l2;
  unsigned long op;
#endif
#else
#if defined(__STDC__)
mtbdd_sum_thresh_step(bdd_manager bddm, bdd f, bdd g, double l1, double l2, unsigned long op)
#else
mtbdd_sum_thresh_step(bddm, f, g, l1, l2, op)
  bdd_manager bddm;
  bdd f,g;
  double l1,l2;
  unsigned long op;
#endif
#endif
{
  bdd_indexindex_type top_indexindex;
  bdd f1,f2;
  bdd g1,g2;
  bdd temp1,temp2;
  bdd result;

  BDD_SETUP(f);
  BDD_SETUP(g);
  mtbdd_sum_step_count++;
  /* if one of the nodes is a logical zero, then
     it implies that no integer value is defined for
     that path
     we return the other node, we will consider
     undefined+val = undefined
  */
  if (BDD_IS_CONST(f))
  {
    if (f == BDD_ZERO(bddm))
    {
		/* BDD_TEMP_INCREFS(f); */
      return (f);
    }
    {
      unsigned long v1,v2;
      mtbdd_terminal_value_aux(bddm, f, &v1, &v2);
      /* add with zero? */
      if (v1 == 0 && v2 == 0)
      {
        BDD_TEMP_INCREFS(g);
        return(g);
      }
    }
  }
  if (BDD_IS_CONST(g))
  {
    if (g == BDD_ZERO(bddm))
    {
		/* BDD_TEMP_INCREFS(g); */
      return (g);
    }
    {
      unsigned long v1,v2;
      mtbdd_terminal_value_aux(bddm, g, &v1, &v2);
      /* add with zero? */
      if (v1 == 0 && v2 == 0)
      {
        BDD_TEMP_INCREFS(f);
        return(f);
      }
    }
  }
  /* none is logical/arithmetic zero */
  /* are both mtbdd terminals */
  if (BDD_IS_CONST(f) && BDD_IS_CONST(g))
  {
    unsigned long v1,v2,v3,v4;
	double dv12, dv34;
    /* fart if both constants are not mtbdd terminals */
    if (bdd_type_aux(bddm,f) != BDD_TYPE_CONSTANT ||
        bdd_type_aux(bddm,g) != BDD_TYPE_CONSTANT)
    {
      /* error condition */
      //bdd_fatal(" arithmetic sum on boolean terminals ");
      //return ((bdd)0);
		longjmp(bddm->abort.context, BDD_FATAL);
    }
		
  
    mtbdd_terminal_value_aux(bddm, f, &v1, &v2);
    mtbdd_terminal_value_aux(bddm, g, &v3, &v4);
	
#ifndef DOUBLE_MTBDD
	if (v2 == -1) v2 = -v2;
	if (v4 == -1) v4 = -v4;

    v1 += v3;
    v2 += v4;

	if (v1 == 0 && v2 == 1) v2 = -1;

	if ((v1 >l1) || ((v1 == l1) && (v2 >= l2)))
		return BDD_ZERO(bddm);
#else
	dv12 = getEncodingValue(v1, v2);
	dv34 = getEncodingValue(v3, v4);
	dv12 = dv12 + dv34;
	if(dv12 >= l2)
		return BDD_ZERO(bddm);
	getDecodingValue(dv12, &v1, &v2);
#endif
    result = bdd_find_terminal(bddm, v1, v2);
    {/*
      BDD_SETUP(result);
      BDD_TEMP_INCREFS(result); */
    }
    return (result);
  }


  if (BDD_OUT_OF_ORDER(f, g))
    BDD_SWAP(f, g);

  if (op > 0 )
  {
	if (bdd_lookup_in_cache2(bddm, op, f, g,  &result))
	return (result);
  }


  BDD_TOP_VAR2(top_indexindex, bddm, f, g);
  BDD_COFACTOR(top_indexindex, f, f1, f2);
  BDD_COFACTOR(top_indexindex, g, g1, g2);
  temp1 = mtbdd_sum_thresh_step(bddm, f1, g1, l1, l2, op);
  temp2 = mtbdd_sum_thresh_step(bddm, f2, g2, l1, l2, op);
  result = bdd_find(bddm, top_indexindex, temp1, temp2);

  if (op > 0)
  {
	  bdd_insert_in_cache2(bddm, op, f, g, result);
  }
  
  return (result);
}


bdd
#ifndef DOUBLE_MTBDD
#if defined(__STDC__)
DESERT_API mtbdd_sum_thresh(bdd_manager bddm, bdd f, bdd g, INT_PTR l1, INT_PTR l2, unsigned long op_offset)
#else
DESERT_API mtbdd_sum(bddm, f, g, l1, l2)
  bdd_manager bddm;
  bdd f,g;
  INT_PTR l1,l2;
  unsigned long op_offset;
#endif
#else
#if defined(__STDC__)
DESERT_API mtbdd_sum_thresh(bdd_manager bddm, bdd f, bdd g, double l1, double l2, unsigned long op_offset)
#else
DESERT_API mtbdd_sum(bddm, f, g, l1, l2)
  bdd_manager bddm;
  bdd f,g;
  double l1,l2;
  unsigned long op_offset;
#endif
#endif
{
	bdd ret;
  if (bdd_check_arguments(2, f, g))
  {
    FIREWALL(bddm);
	if(setjmp((bddm)->abort.context)==0)
	{
		if ((op_offset > 0) && (op_offset <   MAX_CACHE_OFFSET))
	  		ret = mtbdd_sum_thresh_step(bddm, f, g, l1, l2, OP_SUM_THRESH+op_offset );	 
		else
			ret = mtbdd_sum_thresh_step(bddm, f, g, l1, l2, 0);	 
		RETURN_BDD(ret);
	}
	else
		return ((bdd)0);
  }
  return ((bdd)0);
}


/*--------------------------------------------

///defines the function mtbdd_mul with threshold 

----------------------------------------------*/

bdd
#ifndef DOUBLE_MTBDD
#if defined(__STDC__)
mtbdd_mul_thresh_step(bdd_manager bddm, bdd f, bdd g, INT_PTR l1, INT_PTR l2, unsigned long op)
#else
mtbdd_mul_thresh_step(bddm, f, g, l1, l2)
  bdd_manager bddm;
  bdd f,g;
  INT_PTR l1,l2;
  unsigned long op;
#endif
#else
#if defined(__STDC__)
mtbdd_mul_thresh_step(bdd_manager bddm, bdd f, bdd g, double l1, double l2, unsigned long op)
#else
mtbdd_mul_thresh_step(bddm, f, g, l1, l2)
  bdd_manager bddm;
  bdd f,g;
  double l1,l2;
  unsigned long op;
#endif
#endif
{
  bdd_indexindex_type top_indexindex;
  bdd f1,f2;
  bdd g1,g2;
  bdd temp1,temp2;
  bdd result;

  BDD_SETUP(f);
  BDD_SETUP(g);
  //mtbdd_sum_step_count++;
  /* if one of the nodes is a logical zero, then
     it implies that no integer value is defined for
     that path
     we return the other node, we will consider
     undefined+val = undefined
  */
  if (BDD_IS_CONST(f))
  {
    if (f == BDD_ZERO(bddm))
    {
		/* BDD_TEMP_INCREFS(f); */
      return (f);
    }
    {
      unsigned long v1,v2;
      mtbdd_terminal_value_aux(bddm, f, &v1, &v2);
      /* add with zero? */
      if (v1 == 0 && v2 == 0)
      {
        BDD_TEMP_INCREFS(g);
        return(g);
      }
    }
  }
  if (BDD_IS_CONST(g))
  {
    if (g == BDD_ZERO(bddm))
    {
		/* BDD_TEMP_INCREFS(g); */
      return (g);
    }
    {
      unsigned long v1,v2;
      mtbdd_terminal_value_aux(bddm, g, &v1, &v2);
      /* add with zero? */
      if (v1 == 0 && v2 == 0)
      {
        BDD_TEMP_INCREFS(f);
        return(f);
      }
    }
  }
  /* none is logical/arithmetic zero */
  /* are both mtbdd terminals */
  if (BDD_IS_CONST(f) && BDD_IS_CONST(g))
  {
    unsigned long v1,v2,v3,v4;
	double dv12, dv34;
    /* fart if both constants are not mtbdd terminals */
    if (bdd_type_aux(bddm,f) != BDD_TYPE_CONSTANT ||
        bdd_type_aux(bddm,g) != BDD_TYPE_CONSTANT)
    {
      /* error condition */
      //bdd_fatal(" arithmetic mul on boolean terminals ");
      //return ((bdd)0);
		longjmp(bddm->abort.context, BDD_FATAL);
    }
    
    mtbdd_terminal_value_aux(bddm, f, &v1, &v2);
    mtbdd_terminal_value_aux(bddm, g, &v3, &v4);

#ifndef DOUBLE_MTBDD
	if (v2 == -1) v2 = -v2;
	if (v4 == -1) v4 = -v4;

    v1 *= v3;
    v2 *= v4;

	if (v1 == 0 && v2 == 1) v2 = -1;

	if ((v1 >l1) || ((v1 == l1) && (v2 >= l2)))
		return BDD_ZERO(bddm);
#else
	dv12 = getEncodingValue(v1, v2);
	dv34 = getEncodingValue(v3, v4);
	dv12 = dv12 * dv34;
	if(dv12 >= l2)
		return BDD_ZERO(bddm);
	getDecodingValue(dv12, &v1, &v2);
#endif
    result = bdd_find_terminal(bddm, v1, v2);
    {/*
      BDD_SETUP(result);
      BDD_TEMP_INCREFS(result); */
    }
    return (result);
  }

  if (BDD_OUT_OF_ORDER(f, g))    BDD_SWAP(f, g);

  if (op > 0 )
  {
	if (bdd_lookup_in_cache2(bddm, op, f, g,  &result))
	return (result);
  }

  
  BDD_TOP_VAR2(top_indexindex, bddm, f, g);
  BDD_COFACTOR(top_indexindex, f, f1, f2);
  BDD_COFACTOR(top_indexindex, g, g1, g2);

  
  temp1 = mtbdd_mul_thresh_step(bddm, f1, g1, l1, l2, op);
  temp2 = mtbdd_mul_thresh_step(bddm, f2, g2, l1, l2, op);
  result = bdd_find(bddm, top_indexindex, temp1, temp2);

  if (op > 0)
  {
	  bdd_insert_in_cache2(bddm, op, f, g, result);
  }
  return (result);
}



bdd
#ifndef DOUBLE_MTBDD
#if defined(__STDC__)
DESERT_API  mtbdd_mul_thresh(bdd_manager bddm, bdd f, bdd g, INT_PTR l1, INT_PTR l2, unsigned long op_offset)
#else
DESERT_API mtbdd_mul_tresh(bddm, f, g, l1, l2, op_offset)
  bdd_manager bddm;
  bdd f,g;
  INT_PTR l1,l2;
  unsigned long op_offset;
#endif
#else
#if defined(__STDC__)
DESERT_API  mtbdd_mul_thresh(bdd_manager bddm, bdd f, bdd g, double l1, double l2, unsigned long op_offset)
#else
DESERT_API mtbdd_mul_tresh(bddm, f, g, l1, l2, op_offset)
  bdd_manager bddm;
  bdd f,g;
  double l1,l2;
  unsigned long op_offset;
#endif
#endif
{
	bdd ret;
  if (bdd_check_arguments(2, f, g))
  {
	FIREWALL(bddm);
	if(setjmp((bddm)->abort.context)==0)
	{
  		if ((op_offset > 0) && (op_offset <   MAX_CACHE_OFFSET))
	  		ret = mtbdd_mul_thresh_step(bddm, f, g, l1, l2, OP_MUL_THRESH+op_offset );	 
		else
			ret = mtbdd_mul_thresh_step(bddm, f, g, l1, l2, 0);	 
		RETURN_BDD(ret);
	}
	else
		return ((bdd)0);
  }
  return ((bdd)0);
}

/* 
	mtbdd divby
	divide all the terminal values with an integer and store the quoatient
*/
bdd
#ifndef DOUBLE_MTBDD
#if defined(__STDC__)
mtbdd_divby_step(bdd_manager bddm, bdd f, INT_PTR l1, INT_PTR l2)
#else
mtbdd_divby_step(bddm, f, l1, l2)
  bdd_manager bddm;
  bdd f;
  INT_PTR l1,l2;
#endif
#else
#if defined(__STDC__)
mtbdd_divby_step(bdd_manager bddm, bdd f, double l1, double l2)
#else
mtbdd_divby_step(bddm, f, l1, l2)
  bdd_manager bddm;
  bdd f;
  double l1,l2;
#endif
#endif
{
  bdd temp1,temp2;
  bdd result;

  BDD_SETUP(f);

  if (BDD_IS_CONST(f))
  {
    if (bdd_type_aux(bddm, f) == BDD_TYPE_CONSTANT)
    {
      /* if mtbdd terminal */
      unsigned long v1,v2;
	  double ret_val;

      mtbdd_terminal_value_aux(bddm, f, &v1, &v2);
	  
#ifndef DOUBLE_MTBDD	  
	  if (v2 == -1) v2 = -v2;

	  v2 =  v2 / l2;

	  if (v1 == 0 && v2 == 1) v2 = -1;
#else
	  ret_val = getEncodingValue(v1, v2);
	  ret_val = ret_val / l2;
	  getDecodingValue(ret_val, &v1, &v2);
#endif
	  result = bdd_find_terminal(bddm, v1, v2);
      {/*
        BDD_SETUP(result);
        BDD_TEMP_INCREFS(result);*/
      }
        return (result);
    }

    /*BDD_TEMP_INCREFS(f);*/
    return (f);
  }

  // currently, the cache can't contain a bdd and float
  // bdd_lookup_in_cache2 won't work because bdd_two_gc_fn will interpret l2 as result
  // (and how could this work without putting l1 in the cache??)

  // if (bdd_lookup_in_cached2(bddm, OP_DIVBY, f,  l2, &result))
  //  return (result);

  temp1 = mtbdd_divby_step(bddm, BDD_THEN(f), l1, l2);
  temp2 = mtbdd_divby_step(bddm, BDD_ELSE(f), l1, l2);
  result = bdd_find(bddm, BDD_INDEXINDEX(f), temp1, temp2);
  // bdd_insert_in_cached2(bddm, OP_DIVBY, f,  l2, result);

  return (result);
}

bdd
#ifndef DOUBLE_MTBDD
#if defined(__STDC__)
DESERT_API mtbdd_divby(bdd_manager bddm, bdd f, INT_PTR l1, INT_PTR l2)
#else
DESERT_API  mtbdd_divby(bddm, f, l1, l2)
  bdd_manager bddm;
  bdd f;
  INT_PTR l1,l2;
#endif
#else
#if defined(__STDC__)
DESERT_API mtbdd_divby(bdd_manager bddm, bdd f, double l1, double l2)
#else
DESERT_API  mtbdd_divby(bddm, f, l1, l2)
  bdd_manager bddm;
  bdd f;
  double l1,l2;
#endif
#endif
{
	bdd ret;
  if (bdd_check_arguments(1, f))
  {
    FIREWALL(bddm);
	if(setjmp((bddm)->abort.context)==0)
	{
		ret = mtbdd_divby_step(bddm, f, l1, l2);
		RETURN_BDD(ret);
	}
	else 
		return ((bdd)0);
   // RETURN_BDD(mtbdd_divby_step(bddm, f, l1, l2));
  }
  return ((bdd)0);
}



/* 
	mtbdd div_from
	divide an integer with all the terminal values and store the quoatient
*/
bdd
#ifndef DOUBLE_MTBDD
#if defined(__STDC__)
mtbdd_div_from_step(bdd_manager bddm, bdd f, INT_PTR l1, INT_PTR l2)
#else
mtbdd_div_from_step(bddm, f, l1, l2)
  bdd_manager bddm;
  bdd f;
  INT_PTR l1,l2;
#endif
#else
#if defined(__STDC__)
mtbdd_div_from_step(bdd_manager bddm, bdd f, double l1, double l2)
#else
mtbdd_div_from_step(bddm, f, l1, l2)
  bdd_manager bddm;
  bdd f;
  double l1,l2;
#endif
#endif
{
  bdd temp1,temp2;
  bdd result;

  BDD_SETUP(f);

  if (BDD_IS_CONST(f))
  {
    if (bdd_type_aux(bddm, f) == BDD_TYPE_CONSTANT)
    {
      /* if mtbdd terminal */
      unsigned long v1,v2;
	  double ret_val;

      mtbdd_terminal_value_aux(bddm, f, &v1, &v2);
	  
#ifndef DOUBLE_MTBDD	  
	  if (v2 == -1) v2 = -v2;

	  v2 = l2 / v2;

	  if (v1 == 0 && v2 == 1) v2 = -1;
#else
	  ret_val = getEncodingValue(v1, v2);
	  if(ret_val!=0)
		ret_val = l2 / ret_val;
	  getDecodingValue(ret_val, &v1, &v2);
#endif
	  result = bdd_find_terminal(bddm, v1, v2);
      {/*
        BDD_SETUP(result);
        BDD_TEMP_INCREFS(result);*/
      }
        return (result);
    }
    
    /*BDD_TEMP_INCREFS(f);*/
    return (f);
  }

  //if (bdd_lookup_in_cache2(bddm, OP_DIVBY, f,  l2, &result))
  //  return (result);

  temp1 = mtbdd_div_from_step(bddm, BDD_THEN(f), l1, l2);
  temp2 = mtbdd_div_from_step(bddm, BDD_ELSE(f), l1, l2);
  result = bdd_find(bddm, BDD_INDEXINDEX(f), temp1, temp2);
  // bdd_insert_in_cache2(bddm, OP_DIV_FROM, f,  l2, result);

  return (result);
}

bdd
#ifndef DOUBLE_MTBDD
#if defined(__STDC__)
DESERT_API mtbdd_div_from(bdd_manager bddm, bdd f, INT_PTR l1, INT_PTR l2)
#else
DESERT_API  mtbdd_div_from(bddm, f, l1, l2)
  bdd_manager bddm;
  bdd f;
  INT_PTR l1,l2;
#endif
#else
#if defined(__STDC__)
DESERT_API mtbdd_div_from(bdd_manager bddm, bdd f, double l1, double l2)
#else
DESERT_API  mtbdd_div_from(bddm, f, l1, l2)
  bdd_manager bddm;
  bdd f;
  double l1,l2;
#endif
#endif
{
	bdd ret;
  if (bdd_check_arguments(1, f))
  {
    FIREWALL(bddm);
	if(setjmp((bddm)->abort.context)==0)
	{
		ret = mtbdd_div_from_step(bddm, f, l1, l2);
		RETURN_BDD(ret);
	}
	else 
		return ((bdd)0);
 //   RETURN_BDD(mtbdd_div_from_step(bddm, f, l1, l2));
  }
  return ((bdd)0);
}

/* 
	mtbdd root by
	divide all the terminal values with an integer and store the quoatient
*/

bdd
#ifndef DOUBLE_MTBDD
#if defined(__STDC__)
  mtbdd_rootby_step(bdd_manager bddm, bdd f, INT_PTR l1, INT_PTR l2)
#else
  mtbdd_rootby_step(bddm, f, l1, l2)
  bdd_manager bddm;
  bdd f;
  INT_PTR l1,l2;
#endif
#else
#if defined(__STDC__)
  mtbdd_rootby_step(bdd_manager bddm, bdd f, double l1, double l2)
#else
  mtbdd_rootby_step(bddm, f, l1, l2)
  bdd_manager bddm;
  bdd f;
  double l1,l2;
#endif
#endif
{
  bdd temp1,temp2;
  bdd result;

  BDD_SETUP(f);

  /*
   * dumb implementation
   */
	mtbdd_threshold_step_count++;

  if (BDD_IS_CONST(f))
  {
    if (bdd_type_aux(bddm, f) == BDD_TYPE_CONSTANT)
    {
      /* if mtbdd terminal */
      unsigned long v1,v2;
	  double ret_val;
      mtbdd_terminal_value_aux(bddm, f, &v1, &v2);
	  
#ifndef DOUBLE_MTBDD
	  if (v2 == -1) v2 = -v2;
	  
	  v2 = (int) pow( (double)v2 , 1.0/(double)l2 );
	  
	  if (v1 == 0 && v2 == 1) v2 = -1;
#else
	  ret_val = getEncodingValue(v1, v2);
	  ret_val = pow( (double)ret_val , 1.0/l2 );
	  getDecodingValue(ret_val, &v1, &v2);
#endif
	  result = bdd_find_terminal(bddm, v1, v2);

      {/*
        BDD_SETUP(result);
        BDD_TEMP_INCREFS(result);*/
      }
        return (result);
    }
    
    /*BDD_TEMP_INCREFS(f);*/
    return (f);
  }

  // if (bdd_lookup_in_cache2(bddm, OP_ROOTBY, f,  l2,  &result))
  //  return (result);

  temp1 = mtbdd_rootby_step(bddm, BDD_THEN(f), l1, l2);
  temp2 = mtbdd_rootby_step(bddm, BDD_ELSE(f), l1, l2);
  result = bdd_find(bddm, BDD_INDEXINDEX(f), temp1, temp2);
  // bdd_insert_in_cache2(bddm, OP_ROOTBY, f,  l2, result);

  return (result);
}

bdd
#ifndef DOUBLE_MTBDD
#if defined(__STDC__)
DESERT_API  mtbdd_rootby(bdd_manager bddm, bdd f, INT_PTR l1, INT_PTR l2)
#else
mtbdd_rootby(bddm, f, l1, l2)
  bdd_manager bddm;
  bdd f;
  INT_PTR l1,l2;
#endif
#else
#if defined(__STDC__)
DESERT_API  mtbdd_rootby(bdd_manager bddm, bdd f, double l1, double l2)
#else
mtbdd_rootby(bddm, f, l1, l2)
  bdd_manager bddm;
  bdd f;
  double l1,l2;
#endif
#endif
{
	bdd ret;
  if (bdd_check_arguments(1, f))
  {
    FIREWALL(bddm);
	if(setjmp((bddm)->abort.context)==0)
	{
		mtbdd_rootby_step(bddm, f, l1, l2);
		RETURN_BDD(ret);
	}
	else 
		return ((bdd)0);
//    RETURN_BDD(mtbdd_rootby_step(bddm, f, l1, l2));
  }
  return ((bdd)0);
}


/* 
	mtbdd sumby
	add all the terminal values with an integer and store the quoatient
*/
bdd
#ifndef DOUBLE_MTBDD
#if defined(__STDC__)
mtbdd_sumby_step(bdd_manager bddm, bdd f, INT_PTR l1, INT_PTR l2)
#else
mtbdd_sumby_step(bddm, f, l1, l2)
  bdd_manager bddm;
  bdd f;
  INT_PTR l1,l2;
#endif
#else
#if defined(__STDC__)
mtbdd_sumby_step(bdd_manager bddm, bdd f, double l1, double l2)
#else
mtbdd_sumby_step(bddm, f, l1, l2)
  bdd_manager bddm;
  bdd f;
  double l1,l2;
#endif
#endif
{
  bdd temp1,temp2;
  bdd result;

  BDD_SETUP(f);

  if (BDD_IS_CONST(f))
  {
    if (bdd_type_aux(bddm, f) == BDD_TYPE_CONSTANT)
    {
      /* if mtbdd terminal */
      unsigned long v1,v2;
	  double dv12;
      mtbdd_terminal_value_aux(bddm, f, &v1, &v2);
	  
	  
#ifndef DOUBLE_MTBDD
	  if (v2 == -1) v2 = -v2;

	  v2 += l2;

	  if (v1 == 0 && v2 == 1) v2 = -1;
#else
	  dv12 = getEncodingValue(v1, v2);
	  dv12 = dv12 + l2;
	  getDecodingValue(dv12, &v1, &v2);
#endif
	  result = bdd_find_terminal(bddm, v1, v2);
      {/*
        BDD_SETUP(result);
        BDD_TEMP_INCREFS(result);*/
      }
        return (result);
    }
  
    /*BDD_TEMP_INCREFS(f);*/
    return (f);
  }

  // if (bdd_lookup_in_cached2(bddm, OP_SUMBY, f,  l2, &result))
  //  return (result);

  temp1 = mtbdd_sumby_step(bddm, BDD_THEN(f), l1, l2);
  temp2 = mtbdd_sumby_step(bddm, BDD_ELSE(f), l1, l2);
  result = bdd_find(bddm, BDD_INDEXINDEX(f), temp1, temp2);
  // bdd_insert_in_cached2(bddm, OP_SUMBY, f,  l2, result);

  return (result);
}

bdd
#ifndef DOUBLE_MTBDD
#if defined(__STDC__)
DESERT_API mtbdd_sumby(bdd_manager bddm, bdd f, INT_PTR l1, INT_PTR l2)
#else
DESERT_API  mtbdd_sumby(bddm, f, l1, l2)
  bdd_manager bddm;
  bdd f;
  INT_PTR l1,l2;
#endif
#else
#if defined(__STDC__)
DESERT_API mtbdd_sumby(bdd_manager bddm, bdd f, double l1, double l2)
#else
DESERT_API  mtbdd_sumby(bddm, f, l1, l2)
  bdd_manager bddm;
  bdd f;
  doubel l1,l2;
#endif
#endif
{
	bdd ret;
  if (bdd_check_arguments(1, f))
  {
    FIREWALL(bddm);
	if(setjmp((bddm)->abort.context)==0)
	{
		ret = mtbdd_sumby_step(bddm, f, l1, l2);
		RETURN_BDD(ret);
	}
	else 
		return ((bdd)0);
   // RETURN_BDD(mtbdd_sumby_step(bddm, f, l1, l2));
  }
  return ((bdd)0);
}


/* 
	mtbdd subby
	substract all the terminal values with an integer and store the quoatient
*/
bdd
#ifndef DOUBLE_MTBDD
#if defined(__STDC__)
mtbdd_subby_step(bdd_manager bddm, bdd f, INT_PTR l1, INT_PTR l2)
#else
mtbdd_subby_step(bddm, f, l1, l2)
  bdd_manager bddm;
  bdd f;
  INT_PTR l1,l2;
#endif
#else
#if defined(__STDC__)
mtbdd_subby_step(bdd_manager bddm, bdd f, double l1, double l2)
#else
mtbdd_subby_step(bddm, f, l1, l2)
  bdd_manager bddm;
  bdd f;
  double l1,l2;
#endif
#endif
{
  bdd temp1,temp2;
  bdd result;

  BDD_SETUP(f);

  if (BDD_IS_CONST(f))
  {
    if (bdd_type_aux(bddm, f) == BDD_TYPE_CONSTANT)
    {
      /* if mtbdd terminal */
      unsigned long v1,v2;
	  double dv12;
      mtbdd_terminal_value_aux(bddm, f, &v1, &v2);
	  
	  
#ifndef DOUBLE_MTBDD
	  if (v2 == -1) v2 = -v2;

	  v2 -= l2;

	  if (v1 == 0 && v2 == 1) v2 = -1;
#else
	  dv12 = getEncodingValue(v1, v2);
	  dv12 = dv12 - l2;
	  getDecodingValue(dv12, &v1, &v2);
#endif
	  result = bdd_find_terminal(bddm, v1, v2);
      {/*
        BDD_SETUP(result);
        BDD_TEMP_INCREFS(result);*/
      }
        return (result);
    }
     /*BDD_TEMP_INCREFS(f);*/
    return (f);
  }

  // if (bdd_lookup_in_cache2(bddm, OP_SUBBY, f,  l2, &result))
  //  return (result);

  temp1 = mtbdd_subby_step(bddm, BDD_THEN(f), l1, l2);
  temp2 = mtbdd_subby_step(bddm, BDD_ELSE(f), l1, l2);
  result = bdd_find(bddm, BDD_INDEXINDEX(f), temp1, temp2);
  // bdd_insert_in_cache2(bddm, OP_SUBBY, f,  l2, result);

  return (result);
}

bdd
#ifndef DOUBLE_MTBDD
#if defined(__STDC__)
DESERT_API mtbdd_subby(bdd_manager bddm, bdd f, INT_PTR l1, INT_PTR l2)
#else
DESERT_API  mtbdd_subby(bddm, f, l1, l2)
  bdd_manager bddm;
  bdd f;
  INT_PTR l1,l2;
#endif
#else
#if defined(__STDC__)
DESERT_API mtbdd_subby(bdd_manager bddm, bdd f, double l1, double l2)
#else
DESERT_API  mtbdd_subby(bddm, f, l1, l2)
  bdd_manager bddm;
  bdd f;
  double l1,l2;
#endif
#endif
{
	bdd ret;
  if (bdd_check_arguments(1, f))
  {
    FIREWALL(bddm);
	if(setjmp((bddm)->abort.context)==0)
	{
		ret = mtbdd_subby_step(bddm, f, l1, l2);
		RETURN_BDD(ret);
	}
	else 
		return ((bdd)0);
//#ifndef NEGATIVE_MTBDD
   // RETURN_BDD(mtbdd_subby_step(bddm, f, l1, l2));
//#else
//	{
//		bdd g = bdd_find_terminal(bddm, l1, l2);
//		RETURN_BDD(mtbdd_sub_step(bddm, f, g));
//	}
//#endif
  }
  return ((bdd)0);
}

/* 
	mtbdd mulby
	multiple all the terminal values with an integer and store the quoatient
*/
bdd
#ifndef DOUBLE_MTBDD
#if defined(__STDC__)
mtbdd_mulby_step(bdd_manager bddm, bdd f, INT_PTR l1, INT_PTR l2)
#else
mtbdd_mulby_step(bddm, f, l1, l2)
  bdd_manager bddm;
  bdd f;
  INT_PTR l1,l2;
#endif
#else
#if defined(__STDC__)
mtbdd_mulby_step(bdd_manager bddm, bdd f, double l1, double l2)
#else
mtbdd_mulby_step(bddm, f, l1, l2)
  bdd_manager bddm;
  bdd f;
  double l1,l2;
#endif
#endif
{
  bdd temp1,temp2;
  bdd result;

  BDD_SETUP(f);

  if (BDD_IS_CONST(f))
  {
    if (bdd_type_aux(bddm, f) == BDD_TYPE_CONSTANT)
    {
      /* if mtbdd terminal */
      unsigned long v1,v2;
	  double dv12;
      mtbdd_terminal_value_aux(bddm, f, &v1, &v2);
	  
	  
#ifndef DOUBLE_MTBDD
	  if (v2 == -1) v2 = -v2;

	  v2 *= l2;

	  if (v1 == 0 && v2 == 1) v2 = -1;
#else
	  dv12 = getEncodingValue(v1, v2);
	  dv12 = dv12 * l2;
	  getDecodingValue(dv12, &v1, &v2);
#endif
	  result = bdd_find_terminal(bddm, v1, v2);
      {/*
        BDD_SETUP(result);
        BDD_TEMP_INCREFS(result);*/
      }
        return (result);
    }    
	
  /*BDD_TEMP_INCREFS(f);*/
    return (f);
  }

  //if (bdd_lookup_in_cached2(bddm, OP_MULBY, f,  l2, &result))
  //  return (result);

  temp1 = mtbdd_mulby_step(bddm, BDD_THEN(f), l1, l2);
  temp2 = mtbdd_mulby_step(bddm, BDD_ELSE(f), l1, l2);
  result = bdd_find(bddm, BDD_INDEXINDEX(f), temp1, temp2);
  //bdd_insert_in_cached2(bddm, OP_MULBY, f,  l2, result);

  return (result);
}

bdd
#ifndef DOUBLE_MTBDD
#if defined(__STDC__)
DESERT_API mtbdd_mulby(bdd_manager bddm, bdd f, INT_PTR l1, INT_PTR l2)
#else
DESERT_API  mtbdd_mulby(bddm, f, l1, l2)
  bdd_manager bddm;
  bdd f;
  INT_PTR l1,l2;
#endif
#else
#if defined(__STDC__)
DESERT_API mtbdd_mulby(bdd_manager bddm, bdd f, double l1, double l2)
#else
DESERT_API  mtbdd_mulby(bddm, f, l1, l2)
  bdd_manager bddm;
  bdd f;
  double l1,l2;
#endif
#endif
{
	bdd ret;
  if (bdd_check_arguments(1, f))
  {
    FIREWALL(bddm);
	if(setjmp((bddm)->abort.context)==0)
	{
		ret = mtbdd_mulby_step(bddm, f, l1, l2);
		RETURN_BDD(ret);
	}
	else 
		return ((bdd)0);
  }
  return ((bdd)0);
}


/*
 * Himanshu: Added 02/24/2012: Math functions support in MTBDD
 * mtbdd apply unary math function (step function for recursive exploration)
 * apply the math function to all the terminal values and store the quotient
*/
bdd
#if defined(__STDC__)
DESERT_API mtbdd_apply_unary_math_function_step(bdd_manager bddm, bdd f, bdd_unary_math_function math_function)
#else
DESERT_API mtbdd_apply_unary_math_function_step(bddm, f, math_function)
  bdd_manager bddm;
  bdd f;
  bdd_unary_math_function math_function;
#endif
{
  bdd temp1,temp2;
  bdd result;

  BDD_SETUP(f);

  if (BDD_IS_CONST(f))
  {
    if (bdd_type_aux(bddm, f) == BDD_TYPE_CONSTANT)
    {
      /* if mtbdd terminal */
      unsigned long v1,v2;
	  double dv12;
	  int math_ret;
      mtbdd_terminal_value_aux(bddm, f, &v1, &v2);
	  
	  
#ifndef DOUBLE_MTBDD
	  if (v2 == -1) v2 = -v2;

	  v2 = mtbdd_call_muParser_math_function(v2, math_function);

	  if (v1 == 0 && v2 == 1) v2 = -1;
#else
	  dv12 = getEncodingValue(v1, v2);
	  math_ret = call_muParser_math_function(dv12, &dv12, math_function);
	  if(math_ret)
		  longjmp(bddm->abort.context, BDD_FATAL);
//	  dv12 = mtbdd_call_muParser_math_function(dv12,math_function);	 
	  getDecodingValue(dv12, &v1, &v2);
#endif
	  result = bdd_find_terminal(bddm, v1, v2);
      {/*
        BDD_SETUP(result);
        BDD_TEMP_INCREFS(result);*/
      }
        return (result);
    }
    
    /*BDD_TEMP_INCREFS(f);*/
    return (f);
  }

  // TODO: Himanshu: Need to implement caching
  //if (bdd_lookup_in_cache2(bddm, math_function, f, &result))
  //  return (result);

  temp1 = mtbdd_apply_unary_math_function_step(bddm, BDD_THEN(f), math_function);
  temp2 = mtbdd_apply_unary_math_function_step(bddm, BDD_ELSE(f), math_function);
  result = bdd_find(bddm, BDD_INDEXINDEX(f), temp1, temp2);

  // TODO: Himanshu: Need to implement caching
  // bdd_insert_in_cache2(bddm, math_function, f,  result);

  return (result);
}

/*
 * Himanshu: Added 02/24/2012: Math functions support in MTBDD
 * mtbdd apply unary math function (main/top-level function)
 * apply the math function to all the terminal values and store the quotient
*/
bdd
#if defined(__STDC__)
DESERT_API mtbdd_apply_unary_math_function(bdd_manager bddm, bdd f, bdd_unary_math_function math_function)
#else
DESERT_API mtbdd_apply_unary_math_function(bddm, f, math_function)
  bdd_manager bddm;
  bdd f;
  bdd_unary_math_function math_function;
#endif
{
	bdd ret;
  if (bdd_check_arguments(1, f))
  {
    FIREWALL(bddm);

	if(setjmp((bddm)->abort.context)==0)
	{
		ret = mtbdd_apply_unary_math_function_step(bddm, f, math_function);
		RETURN_BDD(ret);
	}
	else 
		return ((bdd)0);
   // RETURN_BDD(mtbdd_apply_unary_math_function_step(bddm, f, math_function));
  }
  return ((bdd)0);
}
