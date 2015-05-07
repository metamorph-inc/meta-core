/* Support functions need by miscellaneous BDD routines */


#include "bddint.h"
#include <tchar.h>

void
#if defined(__STDC__)
bdd_mark_shared_nodes(bdd_manager bddm, bdd f)
#else
bdd_mark_shared_nodes(bddm, f)
     bdd_manager bddm;
     bdd f;
#endif
{
  BDD_SETUP(f);
  f=BDD_OUTPOS(f);
  if (BDD_IS_CONST(f) || bdd_type_aux(bddm, f) == BDD_TYPE_POSVAR)
    return;
  if (BDD_MARK(f))
    {
      if (BDD_MARK(f) == 1)
	BDD_MARK(f)=2;
      return;
    }
  BDD_MARK(f)=1;
  bdd_mark_shared_nodes(bddm, BDD_THEN(f));
  bdd_mark_shared_nodes(bddm, BDD_ELSE(f));
}


void
#if defined(__STDC__)
bdd_number_shared_nodes(bdd_manager bddm, bdd f, hash_table h, long *next)
#else
bdd_number_shared_nodes(bddm, f, h, next)
     bdd_manager bddm;
     bdd f;
     hash_table h;
     long *next;
#endif
{
  BDD_SETUP(f);
  if (BDD_IS_CONST(f) || ((1 << bdd_type_aux(bddm, f)) & ((1 << BDD_TYPE_POSVAR) | (1 << BDD_TYPE_NEGVAR))))
    return;
  if (BDD_MARK(f) == 0)
    return;
  if (BDD_MARK(f) == 2)
    {
      bdd_insert_in_hash_table(h, f, (pointer)next);
      ++*next;
    }
  BDD_MARK(f)=0;
  bdd_number_shared_nodes(bddm, BDD_THEN(f), h, next);
  bdd_number_shared_nodes(bddm, BDD_ELSE(f), h, next);
}


static TCHAR default_terminal_id[]= _T("terminal XXXXXXXXXX XXXXXXXXXX");
static TCHAR default_var_name[]=_T("var.XXXXXXXXXX");


TCHAR *
#if defined(__STDC__)
bdd_terminal_id(bdd_manager bddm, bdd f, TCHAR *(*terminal_id_fn)(bdd_manager, INT_PTR, INT_PTR, pointer), pointer env)
#else
bdd_terminal_id(bddm, f, terminal_id_fn, env)
     bdd_manager bddm;
     bdd f;
     TCHAR *(*terminal_id_fn)();
     pointer env;
#endif
{
  TCHAR *id;
  INT_PTR v1, v2;

  mtbdd_terminal_value_aux(bddm, f, &v1, &v2);
  if (terminal_id_fn)
    id=(*terminal_id_fn)(bddm, v1, v2, env);
  else
    id=0;
  if (!id)
    {
      if (f == BDD_ONE(bddm))
	return (_T("1"));
      if (f == BDD_ZERO(bddm))
	return (_T("0"));
	  _sntprintf(default_terminal_id, _countof(default_terminal_id), _T("terminal %ld %ld"), (long)v1, (long)v2);
      id=default_terminal_id;
    }
  return (id);
}


TCHAR *
#if defined(__STDC__)
bdd_var_name(bdd_manager bddm, bdd v, TCHAR *(*var_naming_fn)(bdd_manager, bdd, pointer), pointer env)
#else
bdd_var_name(bddm, v, var_naming_fn, env)
     bdd_manager bddm;
     bdd v;
     TCHAR *(*var_naming_fn)();
     pointer env;
#endif
{
  TCHAR *name;

  if (var_naming_fn)
    name=(*var_naming_fn)(bddm, v, env);
  else
    name=0;
  if (!name)
    {
      BDD_SETUP(v);
	  _sntprintf(default_var_name, _countof(default_var_name), _T("var.%d"), BDD_INDEX(bddm, v));
      name=default_var_name;
    }
  return (name);
}


void
#if defined(__STDC__)
mtbdd_terminal_value_aux(bdd_manager bddm, bdd f, INT_PTR *value1, INT_PTR *value2)
#else
mtbdd_terminal_value_aux(bddm, f, value1, value2)
     bdd_manager bddm;
     bdd f;
     INT_PTR *value1;
     INT_PTR *value2;
#endif
{
  BDD_SETUP(f);
  if (BDD_IS_OUTPOS(f))
    {
      *value1=BDD_DATA0(f);
      *value2=BDD_DATA1(f);
    }
  else
    (*bddm->transform_fn)(bddm, BDD_DATA0(f), BDD_DATA1(f), value1, value2, bddm->transform_env);
}
