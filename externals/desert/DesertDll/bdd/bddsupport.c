/* BDD support routines */


#include "bddint.h"
#include <tchar.h>


static
int
#if defined(__STDC__)
bdd_depends_on_step(bdd_manager bddm, bdd f, bdd_index_type var_index, int mark)
#else
bdd_depends_on_step(bddm, f, var_index, mark)
     bdd_manager bddm;
     bdd f;
     bdd_index_type var_index;
     int mark;
#endif
{
  bdd_index_type f_index;

  BDD_SETUP(f);
  f_index=BDD_INDEX(bddm, f);
  if (f_index > var_index)
    return (0);
  if (f_index == var_index)
    return (1);
  if (BDD_MARK(f) == mark)
    return (0);
  BDD_MARK(f)=mark;
  if (bdd_depends_on_step(bddm, BDD_THEN(f), var_index, mark))
    return (1);
  return (bdd_depends_on_step(bddm, BDD_ELSE(f), var_index, mark));
}


/* bdd_depends_on(bddm, f, var) returns 1 if f depends on var and */
/* returns 0 otherwise. */

int
#if defined(__STDC__)
bdd_depends_on(bdd_manager bddm, bdd f, bdd var)
#else
bdd_depends_on(bddm, f, var)
     bdd_manager bddm;
     bdd f;
     bdd var;
#endif
{
  if (bdd_check_arguments(2, f, var))
    {
      BDD_SETUP(var);
      if (bdd_type_aux(bddm, var) != BDD_TYPE_POSVAR)
	{
	  bdd_warning(_T("bdd_depends_on: second argument is not a positive variable"));
	  if (BDD_IS_CONST(var))
	    return (1);
	}
      (void)bdd_depends_on_step(bddm, f, BDD_INDEX(bddm, var), 1);
      return (bdd_depends_on_step(bddm, f, BDD_INDEX(bddm, var), 0));
    }
  return (0);
}


static
void
#if defined(__STDC__)
bdd_unmark_nodes(bdd_manager bddm, bdd f)
#else
bdd_unmark_nodes(bddm, f)
     bdd_manager bddm;
     bdd f;
#endif
{
  bdd temp;

  BDD_SETUP(f);
  if (!BDD_MARK(f) || BDD_IS_CONST(f))
    return;
  BDD_MARK(f)=0;
  temp=BDD_IF(bddm, f);
  {
    BDD_SETUP(temp);
    BDD_MARK(temp)=0;
  }
  bdd_unmark_nodes(bddm, BDD_THEN(f));
  bdd_unmark_nodes(bddm, BDD_ELSE(f));
}


static
bdd *
#if defined(__STDC__)
bdd_support_step(bdd_manager bddm, bdd f, bdd *support)
#else
bdd_support_step(bddm, f, support)
     bdd_manager bddm;
     bdd f;
     bdd *support;
#endif
{
  bdd temp;

  BDD_SETUP(f);
  if (BDD_MARK(f) || BDD_IS_CONST(f))
    return (support);
  temp=BDD_IF(bddm, f);
  {
    BDD_SETUP(temp);
    if (!BDD_MARK(temp))
      {
	BDD_MARK(temp)=1;
	*support=temp;
	++support;
      }
  }
  BDD_MARK(f)=1;
  support=bdd_support_step(bddm, BDD_THEN(f), support);
  return (bdd_support_step(bddm, BDD_ELSE(f), support));
}


/* bdd_support(bddm, f, support) returns the support of f as a */
/* null-terminated array of variables. */

void
#if defined(__STDC__)
bdd_support(bdd_manager bddm, bdd f, bdd *support)
#else
bdd_support(bddm, f, support)
     bdd_manager bddm;
     bdd f;
     bdd *support;
#endif
{
  bdd *end;

  if (bdd_check_arguments(1, f))
    {
      end=bdd_support_step(bddm, f, support);
      *end=0;
      bdd_unmark_nodes(bddm, f);
    }
  else
    *support=0;
}
