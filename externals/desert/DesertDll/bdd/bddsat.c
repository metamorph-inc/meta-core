/* BDD satisfying valuation routines */

#define NO_FREES

#include "bddint.h"
#include <tchar.h>

/* JASON - I'm using regular malloc's and free's here */
/* no real reason to use their get_mem_block() */

#include <stdlib.h>
#include <malloc.h>
#include <string.h>

#undef DEBUG

#if defined(__STDC__)
#include <stdlib.h>
#else
extern void qsort();
#endif

static
bdd
#if defined(__STDC__)
bdd_satisfy_step(bdd_manager bddm, bdd f)
#else
bdd_satisfy_step(bddm, f)
     bdd_manager bddm;
     bdd f;
#endif
{
  bdd temp;
  bdd result;

  BDD_SETUP(f);
  if (BDD_IS_CONST(f))
    return (f);
  if (BDD_THEN(f) == BDD_ZERO(bddm))
    {
      temp=bdd_satisfy_step(bddm, BDD_ELSE(f));
      result=bdd_find(bddm, BDD_INDEXINDEX(f), BDD_ZERO(bddm), temp);
    }
  else
    {
      temp=bdd_satisfy_step(bddm, BDD_THEN(f));
      result=bdd_find(bddm, BDD_INDEXINDEX(f), temp, BDD_ZERO(bddm));
    }
  return (result);
}


/* bdd_satisfy(bddm, f) returns a BDD which implies f, is true for */
/* some valuation on which f is true, and which has at most one node */
/* at each level. */

bdd
#if defined(__STDC__)
bdd_satisfy(bdd_manager bddm, bdd f)
#else
bdd_satisfy(bddm, f)
     bdd_manager bddm;
     bdd f;
#endif
{
  if (bdd_check_arguments(1, f))
    {
      if (f == BDD_ZERO(bddm))
	{
	  bdd_warning(_T("bdd_satisfy: argument is false"));
	  return (f);
	}
      FIREWALL(bddm);
      RETURN_BDD(bdd_satisfy_step(bddm, f));
    }
  return ((bdd)0);
}


static
bdd
#if defined(__STDC__)
bdd_satisfy_support_step(bdd_manager bddm, bdd f, bdd_indexindex_type *support)
#else
bdd_satisfy_support_step(bddm, f, support)
     bdd_manager bddm;
     bdd f;
     bdd_indexindex_type *support;
#endif
{
  bdd temp;
  bdd result;

  BDD_SETUP(f);
  if (!*support)
    return (bdd_satisfy_step(bddm, f));
  if (BDD_INDEX(bddm, f) <= bddm->indexes[*support])
    {
      if (BDD_INDEXINDEX(f) == *support)
	++support;
      if (BDD_THEN(f) == BDD_ZERO(bddm))
	{
	  temp=bdd_satisfy_support_step(bddm, BDD_ELSE(f), support);
	  result=bdd_find(bddm, BDD_INDEXINDEX(f), BDD_ZERO(bddm), temp);
	}
      else
	{
	  temp=bdd_satisfy_support_step(bddm, BDD_THEN(f), support);
	  result=bdd_find(bddm, BDD_INDEXINDEX(f), temp, BDD_ZERO(bddm));
	}
    }
  else
    {
      temp=bdd_satisfy_support_step(bddm, f, support+1);
      result=bdd_find(bddm, *support, BDD_ZERO(bddm), temp);
    }
  return (result);
}

static
int
#if defined(__STDC__)
index_cmp(pointer p1, pointer p2)
#else
index_cmp(p1, p2)
     pointer p1;
     pointer p2;
#endif
{
  bdd_index_type i1, i2;

  i1= *(bdd_indexindex_type *)p1;
  i2= *(bdd_indexindex_type *)p2;
  if (i1 < i2)
    return (-1);
  if (i1 > i2)
    return (1);
  return (0);
}


/* bdd_satisfy_support(bddm, f) returns a BDD which implies f, is true */
/* for some valuation on which f is true, which has at most one node */
/* at each level, and which has exactly one node corresponding to each */
/* variable which is associated with something in the current variable */
/* association. */

bdd
#if defined(__STDC__)
bdd_satisfy_support(bdd_manager bddm, bdd f)
#else
bdd_satisfy_support(bddm, f)
     bdd_manager bddm;
     bdd f;
#endif
{
  bdd_indexindex_type *support, *p;
  long i;
  bdd result;

  if (bdd_check_arguments(1, f))
    {
      if (f == BDD_ZERO(bddm))
	{
	  bdd_warning(_T("bdd_satisfy_support: argument is false"));
	  return (f);
	}
      support=(bdd_indexindex_type *)mem_get_block((bddm->vars+1)*sizeof(bdd));
      FIREWALL1(bddm,
		if (retcode == BDD_ABORTED || retcode == BDD_OVERFLOWED)
		  {
		    mem_free_block((pointer)support);
		    return ((bdd)0);
		  }
		);
      for (i=0, p=support; i < bddm->vars; ++i)
	if (bddm->curr_assoc->assoc[i+1])
	  {
	    *p=bddm->indexes[i+1];
	    ++p;
	  }
      *p=0;
      qsort((pointer)support, (unsigned)(p-support), sizeof(bdd_indexindex_type), index_cmp);
      while (p != support)
	{
	  --p;
	  *p=bddm->indexindexes[*p];
	}
      result=bdd_satisfy_support_step(bddm, f, support);
      mem_free_block((pointer)support);
      RETURN_BDD(result);
    }
  return ((bdd)0);
}


double
#if defined(__STDC__)
bdd_satisfying_fraction_step(bdd_manager bddm, bdd f)
#else
bdd_satisfying_fraction_step(bddm, f)
     bdd_manager bddm;
     bdd f;
#endif
{
  long cache_result[2];
  INT_PTR cache_result0, cache_result1;
  double result;

  BDD_SETUP(f);
  if (BDD_IS_CONST(f))
    {
      if (f == BDD_ZERO(bddm))
	return (0.0);
      return (1.0);
    }
  if (bdd_lookup_in_cache1d(bddm, OP_SATFRAC, f, &cache_result0, &cache_result1))
    {
	  cache_result[0] = cache_result0;
	  cache_result[1] = cache_result1;
      result= *((double *)cache_result);
      return (result);
    }
  result=0.5*bdd_satisfying_fraction_step(bddm, BDD_THEN(f))+
    0.5*bdd_satisfying_fraction_step(bddm, BDD_ELSE(f));
  *((double *)cache_result)=result;
  cache_result0 = cache_result[0];
  cache_result1 = cache_result[1];
  bdd_insert_in_cache1d(bddm, OP_SATFRAC, f, &cache_result0, &cache_result1);
  return (result);
}


/* bdd_satisfying_fraction(bddm, f) returns the fraction of valuations */
/* which make f true.  (Note that this fraction is independent of */
/* whatever set of variables f is supposed to be a function of.) */

double
#if defined(__STDC__)
bdd_satisfying_fraction(bdd_manager bddm, bdd f)
#else
bdd_satisfying_fraction(bddm, f)
     bdd_manager bddm;
     bdd f;
#endif
{
  if (bdd_check_arguments(1, f))
    return (bdd_satisfying_fraction_step(bddm, f));
  return (0.0);
}

/*****************************************************************************/
/*                                                                           */
/* 5/25/97 - This function returns a matrix of all the true paths (paths     */
/*           leading to the one terminal). This represents all cases where   */
/*           the bdd may be true - don't cares are not expanded.             */
/*           This is not exactly the same as the typical satisfy which       */
/*           returns *all* combinations which satisfy the function           */
/*****************************************************************************/

void bdd_sat_f_step_mat(bdd_manager bddm, bdd f, int *a, int len, 
            int prev_index, int num_vars, int **m, int *rows)
{
  int *b, *c;
  int i;
  int cur_index;
  int fint;

  BDD_SETUP(f);
#ifdef DEBUG
  printf("Just Entered - len =%d\n",len);
#endif
  cur_index = BDD_INDEX(bddm, f);

  if (*rows >= BDD_MAX_PATHS) {
    // give up here, runtime will be too long (and OOM handling is not great)
    return;
  }

  b = (int *)malloc(sizeof(int)*(num_vars+1));
  c = (int *)malloc(sizeof(int)*(num_vars+1));

  if( (b==NULL) || (c==NULL) ) {
	 bdd_fatal(_T("bdd_sat_f_mat(): Malloc failed.\n"));
  }

  for(i=0; i<(num_vars+1); i++) {
     b[i]=c[i]=-1;
  }

  for(i=0; i<len; i++) {      // Duplicate a from prev node into b and c
#ifdef DEBUG
     printf("    a[%d] = %d\n",i,a[i]);
#endif
     b[i] = a[i];
     c[i] = a[i];
  }

  /* if cur_index is 65535, it doesn't matter, we will leave here */  
  if ( (BDD_IS_CONST(f)) ) 
  {
#ifdef DEBUG
      if(f == BDD_ZERO(bddm))
        printf("  BDD is 0\n");
      if(f == BDD_ONE(bddm))
        printf("  BDD is 1\n");
#endif
 
      if(len < num_vars) {  /* we have don't care at end */
#ifdef DEBUG
        printf("    Don't Care Found at end:\n");
#endif
      for(i=len; i < num_vars; i++) {
#ifdef DEBUG
        printf("    END: a[%d] = -1\n",i);
#endif
  	   b[i] = -1;
	   c[i] = -1;
        }
      }
      len = num_vars;

	 // int fint = 0;
	  fint = bdd_type_aux(bddm,f);
	 

      if (f == BDD_ZERO(bddm)) {
#ifdef DEBUG
        printf("    FALSE\n");
#endif
        //for(i=0; i<len; i++)
        // printf("    FALSE: Vars[%d] = %d \n", i, b[i]);

        free(a);
        free(b);
        free(c);

  	return;
      }
      else if (f == BDD_ONE(bddm)) {
#ifdef DEBUG
        printf("^  ");
        for(i=0; i<len; i++) {
           if(b[i] == -1)
               printf("x");
           else
               printf("%d", b[i]);
        }
        printf("\n");
#endif
#ifdef DEBUG
		if (*rows >= 100000 /*BDD_MAX_PATH*/) {
            bdd_fatal(_T("Design space is too big"));
		}
#endif
        m[*rows] = b;        
        *rows = *rows + 1;
        free(a);
        free(c);
        return;
      }
      else {
        bdd_fatal(_T("Error in bdd_sat_f_mat().\n"));
      }
  }

  if(cur_index > (prev_index+1) ) {
#ifdef DEBUG
     printf("Don't Care Found:\n");
#endif
     if(prev_index != (len-1) ) fprintf(stderr, "Error in bdd_sat_f_step_mat()\n");
     for(i=(prev_index+1); i < cur_index; i++) {
#ifdef DEBUG
        printf("a[%d] = -1\n",i);
#endif
        b[i] = -1;
        c[i] = -1;
        len++;
     } 
  }

  b[len] = 1;
  c[len] = 0; 
  len++; 

#if 0
  if(BDD_THEN(f) == BDD_ELSE(f)) {
     printf("BDD_THEN == BDD_ELSE!!!!\n");
  } 
#endif

  free(a);

  bdd_sat_f_step_mat(bddm, BDD_THEN(f), b, len, cur_index, num_vars, m, rows);   
  bdd_sat_f_step_mat(bddm, BDD_ELSE(f), c, len, cur_index, num_vars, m, rows);    

  return;
}

void bdd_sat_f_mat(bdd_manager bddm, bdd f, int begin_var_index, int end_var_index, 
                                            int **matrix, int *num_rows) 
{
  int *a;
  int index=0;
  int num_vars=0;
  int i=0;
  int len=0;
  int size_of_a_list=0;

  int ret=0;

  int r=0;
  int c=0;

  BDD_SETUP(f);      /* Must be directly after declarations */

  if (!bdd_check_arguments(1, f))
  {
      fprintf(stderr, "bdd_sat_f(): overflow\n");
      return;
  }
  /* else */

  if ( (BDD_IS_CONST(f)) ) {
     if(f == BDD_ZERO(bddm))
        printf("  BDD is 0\n");
     if(f == BDD_ONE(bddm))
        printf("  BDD is 1\n");
     return;
  }

    index = BDD_INDEX(bddm, f); 

    // if(index == 65535) {
    //   fprintf(stderr,"index is screwed up!\n");
    // return;
    // }
      
    num_vars = (end_var_index - begin_var_index) + 1;

    size_of_a_list = (index - begin_var_index) + 1;  // +1 is bonus

#ifdef DEBUG
    printf("size_of_a_list = %d, index=%d, begin_var_index=%d\n", size_of_a_list,
                                    index, begin_var_index);
#endif

    if(size_of_a_list > 0) {
       a = (int *)malloc(sizeof(int)*size_of_a_list);
       if( a == NULL ) {
         bdd_fatal(_T("bdd_sat_f_mat(): Malloc failed.\n"));
       }
       for(i=0; i<size_of_a_list; i++) {
         a[i]=-1;
       }
    }

#ifdef DEBUG
    printf("          1st index is: %d\n",index);
#endif
    if(index > begin_var_index) { 
#ifdef DEBUG
       printf("Top node(s) are don't care\n");
#endif
       for(i=begin_var_index, len=0; i< index; i++) {
          a[i] = -1; 
          len++;
       }
    }

  bdd_sat_f_step_mat(bddm, f, a, len, index, num_vars, matrix, num_rows);  /* # vars */

  return;
}

void bdd_sat_f_step(bdd_manager bddm, bdd f, int *a, int len, 
            int prev_index, int num_vars)
{
  int *b, *c;
  int i;
  int cur_index;

  BDD_SETUP(f);
#ifdef DEBUG
  printf("Just Entered - len =%d\n",len);
#endif
  cur_index = BDD_INDEX(bddm, f);

  b = (int *)malloc(sizeof(int)*(num_vars+1));
  c = (int *)malloc(sizeof(int)*(num_vars+1));

  if( (b==NULL) || (c==NULL) ) {
     bdd_fatal(_T("bdd_sat_f_mat(): Malloc failed.\n"));
  }

  for(i=0; i<(num_vars+1); i++) {
     b[i]=c[i]=-1;
  }

  for(i=0; i<len; i++) {      // Duplicate a from prev node into b and c
#ifdef DEBUG
     printf("    a[%d] = %d\n",i,a[i]);
#endif
     b[i] = a[i];
     c[i] = a[i];
  }
#ifdef NO_FREES
  free(a);
#endif

  /* if cur_index is 65535, it doesn't matter, we will leave here */  
  if ( (BDD_IS_CONST(f)) ) 
  {
#ifdef DEBUG
      if(f == BDD_ZERO(bddm))
        printf("  BDD is 0\n");
      if(f == BDD_ONE(bddm))
        printf("  BDD is 1\n");
#endif
 
      if(len < num_vars) {  /* we have don't care at end */
#ifdef DEBUG
        printf("    Don't Care Found at end:\n");
#endif
      for(i=len; i < num_vars; i++) {
#ifdef DEBUG
        printf("    END: a[%d] = -1\n",i);
#endif
  	   b[i] = -1;
	   c[i] = -1;
        }
      }
      len = num_vars;

      if (f == BDD_ZERO(bddm)) {
#ifdef DEBUG
        printf("    FALSE\n");
#endif
        //for(i=0; i<len; i++)
        // printf("    FALSE: Vars[%d] = %d \n", i, b[i]);
  	return;
      }
      else if (f == BDD_ONE(bddm)) {
        printf("  ");
        for(i=0; i<len; i++) {
           if(b[i] == -1)
               printf("x");
           else
               printf("%d", b[i]);
        }
        printf("\n");
        return;
      }
      else {
        bdd_fatal(_T("Error in bdd_sat_f().\n"));
      }
  }

  if(cur_index > (prev_index+1) ) {
#ifdef DEBUG
     printf("Don't Care Found:\n");
#endif
     if(prev_index != (len-1) ) fprintf(stderr, "Error in bdd_sat_f_step()\n");
     for(i=(prev_index+1); i < cur_index; i++) {
#ifdef DEBUG
        printf("a[%d] = -1\n",i);
#endif
        b[i] = -1;
        c[i] = -1;
        len++;
     } 
  }

  b[len] = 1;
  c[len] = 0; 
  len++; 

#if 0
  if(BDD_THEN(f) == BDD_ELSE(f)) {
     printf("BDD_THEN == BDD_ELSE!!!!\n");
  }
#endif

  bdd_sat_f_step(bddm, BDD_THEN(f), b, len, cur_index, num_vars);   

  bdd_sat_f_step(bddm, BDD_ELSE(f), c, len, cur_index, num_vars); 
   
  return;
}

void bdd_sat_f(bdd_manager bddm, bdd f, int begin_var_index, int end_var_index) 
{
  int *a;
  int index=0;
  int num_vars=0;
  int i=0;
  int len=0;
  int size_of_a_list=0;

  BDD_SETUP(f);      /* Must be directly after declarations */

  if (!bdd_check_arguments(1, f))
  {
      fprintf(stderr, "bdd_sat_f(): overflow\n");
      return;
  }
  /* else */

  if ( (BDD_IS_CONST(f)) ) {
     if(f == BDD_ZERO(bddm))
        printf("  BDD is 0\n");
     if(f == BDD_ONE(bddm))
        printf("  BDD is 1\n");
     return;
  }

    index = BDD_INDEX(bddm, f); 

    // if(index == 65535) {
    //   fprintf(stderr,"index is screwed up!\n");
    // return;
    // }
      
    num_vars = (end_var_index - begin_var_index) + 1;

    size_of_a_list = (index - begin_var_index) + 1;  // +1 is bonus
    if(size_of_a_list > 0) {
       a = (int *)malloc(sizeof(int)*size_of_a_list);
       if( a == NULL ) {
         bdd_fatal(_T("bdd_sat_f(): Malloc failed.\n"));
       }
       for(i=0; i<size_of_a_list; i++) {
         a[i]=-1;
       }
    }

#ifdef DEBUG
    printf("          1st index is: %d\n",index);
#endif
    if(index > begin_var_index) { 
#ifdef DEBUG
       printf("Top node(s) are don't care\n");
#endif
       for(i=begin_var_index, len=0; i< index; i++) {
          a[i] = -1; 
          len++;
       }
    }
#ifdef DEBUG
    printf("len=%d, index=%d, num_vars=%d\n",len,index,num_vars);
#endif

#ifdef DEBUG
    printf("bdd_sat_f: len = %d, index=%d, num_vars=%d\n",len,index,num_vars);
#endif
    bdd_sat_f_step(bddm, f, a, len, index, num_vars);  /* # vars */
  
  return;
}









