/* BDD library profile printing routines */


#include "bddint.h"
#include <string.h>
#include <tchar.h>

static TCHAR profile_width[]=_T("XXXXXXXXX");


static
void
#if defined(__STDC__)
chars(TCHAR c, int n, FILE *fp)
#else
chars(c, n, fp)
     TCHAR c;
     int n;
     FILE *fp;
#endif
{
  int i;

  for (i=0; i < n; ++i)
    fputc(c, fp);
}


/* bdd_print_profile_aux(bddm, level_counts, var_naming_fn, line_length, */
/* env, fp) prints a profile to the file given by fp.  The var_naming_fn */
/* is as in bdd_print_bdd.  line_length gives the line width to scale the */
/* profile to. */

void
#if defined(__STDC__)
bdd_print_profile_aux(bdd_manager bddm,
		      long *level_counts,
		      TCHAR *(*var_naming_fn)(bdd_manager, bdd, pointer),
		      pointer env,
		      int line_length,
		      FILE *fp)
#else
bdd_print_profile_aux(bddm, level_counts, var_naming_fn, env, line_length, fp)
     bdd_manager bddm;
     long *level_counts;
     TCHAR *(*var_naming_fn)();
     pointer env;
     int line_length;
     FILE *fp;
#endif
{
  long i, n;
  int l;
  TCHAR *name;
  int max_prefix_len;
  int max_profile_width;
  int histogram_column;
  int histogram_width;
  int profile_scale;
  long total;

  n=bddm->vars;
  /* max_... initialized with values for leaf nodes */
  max_prefix_len=5;
  max_profile_width=level_counts[n];
  total=level_counts[n];
  for (i=0; i < n; ++i)
    if (level_counts[i])
      {
	_sntprintf(profile_width, _countof(profile_width), _T("%d"), level_counts[i]);
	l=_tcslen(bdd_var_name(bddm, bddm->variables[bddm->indexindexes[i]], var_naming_fn, env))+_tcslen(profile_width);
	if (l > max_prefix_len)
	  max_prefix_len=l;
	if (level_counts[i] > max_profile_width)
	  max_profile_width=level_counts[i];
	total+=level_counts[i];
      }
  histogram_column=max_prefix_len+3;
  histogram_width=line_length-histogram_column-1;
  if (histogram_width < 20)
    histogram_width=20;		/* Random minimum width */
  if (histogram_width >= max_profile_width)
    profile_scale=1;
  else
    profile_scale=(max_profile_width+histogram_width-1)/histogram_width;
  for (i=0; i < n; ++i)
    if (level_counts[i])
      {
	name=bdd_var_name(bddm, bddm->variables[bddm->indexindexes[i]], var_naming_fn, env);
	_fputts(name, fp);
	fputc(':', fp);
	_sntprintf(profile_width, _countof(profile_width), _T("%d"), level_counts[i]);
	chars(' ', (int)(max_prefix_len-_tcslen(name)-_tcslen(profile_width)+1), fp);
	_fputts(profile_width, fp);
	fputc(' ', fp);
	chars('#', level_counts[i]/profile_scale, fp);
	fputc('\n', fp);
      }
  fputs("leaf:", fp);
  _sntprintf(profile_width, _countof(profile_width), _T("%d"), level_counts[n]);
  chars(' ', (int)(max_prefix_len-4-_tcslen(profile_width)+1), fp);
  _fputts(profile_width, fp);
  fputc(' ', fp);
  chars('#', level_counts[n]/profile_scale, fp);
  fputc('\n', fp);
  fprintf(fp, "Total: %d\n", total);
}


/* bdd_print_profile(bddm, f, var_naming_fn, env, line_length, fp) displays */
/* the node profile for f on fp.  line_length specifies the maximum line */
/* length.  var_naming_fn is as in bdd_print_bdd. */

void
#if defined(__STDC__)
bdd_print_profile(bdd_manager bddm,
		  bdd f,
		  TCHAR *(*var_naming_fn)(bdd_manager, bdd, pointer),
		  pointer env,
		  int line_length,
		  FILE *fp)
#else
bdd_print_profile(bddm, f, var_naming_fn, env, line_length, fp)
     bdd_manager bddm;
     bdd f;
     TCHAR *(*var_naming_fn)();
     pointer env;
     int line_length;
     FILE *fp;
#endif
{
  long *level_counts;

  if (bdd_check_arguments(1, f))
    {
      level_counts=(long *)mem_get_block((SIZE_T)((bddm->vars+1)*sizeof(long)));
      bdd_profile(bddm, f, level_counts, 1);
      bdd_print_profile_aux(bddm, level_counts, var_naming_fn, env, line_length, fp);
      mem_free_block((pointer)level_counts);
    }
  else
    fputs("overflow\n", fp);
}


/* bdd_print_profile_multiple is like bdd_print_profile except it displays */
/* the profile for a set of BDDs. */

void
#if defined(__STDC__)
bdd_print_profile_multiple(bdd_manager bddm,
			   bdd* fs,
			   TCHAR *(*var_naming_fn)(bdd_manager, bdd, pointer),
			   pointer env,
			   int line_length,
			   FILE *fp)
#else
bdd_print_profile_multiple(bddm, fs, var_naming_fn, env, line_length, fp)
     bdd_manager bddm;
     bdd *fs;
     TCHAR *(*var_naming_fn)();
     pointer env;
     int line_length;
     FILE *fp;
#endif
{
  long *level_counts;

  bdd_check_array(fs);
  level_counts=(long *)mem_get_block((SIZE_T)((bddm->vars+1)*sizeof(long)));
  bdd_profile_multiple(bddm, fs, level_counts, 1);
  bdd_print_profile_aux(bddm, level_counts, var_naming_fn, env, line_length, fp);
  mem_free_block((pointer)level_counts);
}


/* bdd_print_function_profile is like bdd_print_profile except it displays */
/* a function profile for f. */

void
#if defined(__STDC__)
bdd_print_function_profile(bdd_manager bddm,
			   bdd f,
			   TCHAR *(*var_naming_fn)(bdd_manager, bdd, pointer),
			   pointer env,
			   int line_length,
			   FILE *fp)
#else
bdd_print_function_profile(bddm, f, var_naming_fn, env, line_length, fp)
     bdd_manager bddm;
     bdd f;
     TCHAR *(*var_naming_fn)();
     pointer env;
     int line_length;
     FILE *fp;
#endif
{
  long *level_counts;

  if (bdd_check_arguments(1, f))
    {
      level_counts=(long *)mem_get_block((SIZE_T)((bddm->vars+1)*sizeof(long)));
      bdd_function_profile(bddm, f, level_counts);
      bdd_print_profile_aux(bddm, level_counts, var_naming_fn, env, line_length, fp);
      mem_free_block((pointer)level_counts);
    }
  else
    fputs("overflow\n", fp);
}


/* bdd_print_function_profile_multiple is like bdd_print_function_profile */
/* except for multiple BDDs. */

void
#if defined(__STDC__)
bdd_print_function_profile_multiple(bdd_manager bddm,
				    bdd* fs,
				    TCHAR *(*var_naming_fn)(bdd_manager, bdd, pointer),
				    pointer env,
				    int line_length,
				    FILE *fp)
#else
bdd_print_function_profile_multiple(bddm, fs, var_naming_fn, env, line_length, fp)
     bdd_manager bddm;
     bdd* fs;
     TCHAR *(*var_naming_fn)();
     pointer env;
     int line_length;
     FILE *fp;
#endif
{
  long *level_counts;

  bdd_check_array(fs);
  level_counts=(long *)mem_get_block((SIZE_T)((bddm->vars+1)*sizeof(long)));
  bdd_function_profile_multiple(bddm, fs, level_counts);
  bdd_print_profile_aux(bddm, level_counts, var_naming_fn, env, line_length, fp);
  mem_free_block((pointer)level_counts);
}
