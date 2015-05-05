/* BDD quantification routines */


#include "bddint.h"


static
bdd
#if defined(__STDC__)
bdd_exists_step(bdd_manager bddm, bdd f, long op, var_assoc vars)
#else
bdd_exists_step(bddm, f, op, vars)
     bdd_manager bddm;
     bdd f;
     long op;
     var_assoc vars;
#endif
{
  bdd temp1, temp2;
  bdd result;
  int quantifying;

  BDD_SETUP(f);
  if ((long)BDD_INDEX(bddm, f) > vars->last)
    {
      BDD_TEMP_INCREFS(f);
      return (f);
    }
  if (bdd_lookup_in_cache1(bddm, op, f, &result))
    return (result);
  quantifying=(vars->assoc[BDD_INDEXINDEX(f)] != 0);
  temp1=bdd_exists_step(bddm, BDD_THEN(f), op, vars);
  if (quantifying && temp1 == BDD_ONE(bddm))
    result=temp1;
  else
    {
      temp2=bdd_exists_step(bddm, BDD_ELSE(f), op, vars);
      if (quantifying)
	{
	  BDD_SETUP(temp1);
	  BDD_SETUP(temp2);
	  bddm->op_cache.cache_level++;
	  result=bdd_ite_step(bddm, temp1, BDD_ONE(bddm), temp2);
	  BDD_TEMP_DECREFS(temp1);
	  BDD_TEMP_DECREFS(temp2);
	  bddm->op_cache.cache_level--;
	}
      else
	result=bdd_find(bddm, BDD_INDEXINDEX(f), temp1, temp2);
    }
  bdd_insert_in_cache1(bddm, op, f, result);
  return (result);
}


/* bdd_exists_temp is used internally by bdd_rel_prod. */

bdd
#if defined(__STDC__)
bdd_exists_temp(bdd_manager bddm, bdd f, long op)
#else
bdd_exists_temp(bddm, f, op)
     bdd_manager bddm;
     bdd f;
     long op;
#endif
{
  if (bddm->curr_assoc_id != -1)
    op=OP_QNT+bddm->curr_assoc_id;
  return (bdd_exists_step(bddm, f, op, bddm->curr_assoc));
}


/* bdd_exists(bddm, f) returns the BDD for existentially quantifying */
/* out in f all variables which are associated with something in the */
/* current variable association. */

bdd
#if defined(__STDC__)
bdd_exists(bdd_manager bddm, bdd f)
#else
bdd_exists(bddm, f)
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
	op=OP_QNT+bddm->curr_assoc_id;
      RETURN_BDD(bdd_exists_step(bddm, f, op, bddm->curr_assoc));
    }
  return ((bdd)0);
}


/* bdd_forall(bddm, f) returns the BDD for universally quantifying */
/* out in f all variables which are associated with something in the */
/* current variable association. */

bdd
#if defined(__STDC__)
bdd_forall(bdd_manager bddm, bdd f)
#else
bdd_forall(bddm, f)
     bdd_manager bddm;
     bdd f;
#endif
{
  bdd temp;

  if ((temp=bdd_exists(bddm, BDD_NOT(f))))
    return (BDD_NOT(temp));
  return ((bdd)0);
}
