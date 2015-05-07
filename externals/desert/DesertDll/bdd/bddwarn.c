/* BDD error and argument checking routines */


#include <stdio.h>
#if defined(__STDC__)
#include <stdarg.h>
#include <assert.h>
#else
#include <varargs.h>
#endif
#include "bddint.h"
#include <tchar.h>


#if defined(__STDC__)
#include <stdlib.h>
#else
extern void exit();
#endif


/* bdd_warning(message) prints a warning and returns. */

void
#if defined(__STDC__)
bdd_warning(TCHAR *message)
#else
bdd_warning(message)
     TCHAR *message;
#endif
{
  _ftprintf(stderr, _T("BDD library: warning: %s\n"), message);
}


extern void (*bdd_fatal_hook)(TCHAR *message);

/* bdd_fatal(message) prints an error message and exits. */
void
#if defined(__STDC__)
bdd_fatal(TCHAR *message)
#else
bdd_fatal(message)
     TCHAR *message;
#endif
{
  _ftprintf(stderr, _T("BDD library: error: %s\n"), message);
  if (bdd_fatal_hook == NULL)
  {
	assert(0);
	exit(1);
  }
  else
  {
	  (*bdd_fatal_hook)(message);
  }
  /* NOTREACHED */
}


int
#if defined(__STDC__)
bdd_check_arguments(int count, ...)
{
  int all_valid;
  va_list ap;
  bdd f;

  va_start(ap, count);
#else
bdd_check_arguments(va_alist)
     va_dcl
{
  int count;
  int all_valid;
  va_list ap;
  bdd f;

  va_start(ap);
  count=va_arg(ap, int);
#endif
  all_valid=1;
  while (count)
    {
      f=va_arg(ap, bdd);
      {
	BDD_SETUP(f);
	if (!f)
	  all_valid=0;
	else if (BDD_REFS(f) == 0)
	  bdd_fatal(_T("bdd_check_arguments: argument has zero references"));
      }
      --count;
    }
  return (all_valid);
}


void
#if defined(__STDC__)
bdd_check_array(bdd *fs)
#else
bdd_check_array(fs)
     bdd *fs;
#endif
{
  while (*fs)
    {
      bdd_check_arguments(1, *fs);
      ++fs;
    }
}
