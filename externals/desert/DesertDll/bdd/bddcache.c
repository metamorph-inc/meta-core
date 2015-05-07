/* BDD system cache routines */


#include "bddint.h"
#include <tchar.h>


#define HASH1(d1) ((INT_PTR)d1)
#define HASH2(d1, d2) ((INT_PTR)(d1)+(((INT_PTR)(d2)) << 1))
#define HASH3(d1, d2, d3) ((INT_PTR)(d1)+(((INT_PTR)(d2)) << 1)+(((INT_PTR)(d3)) << 2))


static
void
#if defined(__STDC__)
bdd_purge_entry(bdd_manager bddm, cache_entry *bin)
#else
bdd_purge_entry(bddm, bin)
     bdd_manager bddm;
     cache_entry *bin;
#endif
{
#if defined(__STDC__)
  void (*purge_fn)(bdd_manager, cache_entry);
#else
  void (*purge_fn)();
#endif
  cache_entry p;

  p= *bin;
  purge_fn=bddm->op_cache.purge_fn[TAG(p)];
  p=CACHE_POINTER(p);
  if (purge_fn)
    (*purge_fn)(bddm, p);
  bddm->op_cache.entries--;
  BDD_FREE_REC(bddm, (pointer)p, sizeof(struct cache_entry_));
  *bin=0;
}


static
void
#if defined(__STDC__)
bdd_purge_lru(bdd_manager bddm, cache_entry *bin)
#else
bdd_purge_lru(bddm, bin)
     bdd_manager bddm;
     cache_entry *bin;
#endif
{
  if (bin[1])
    bdd_purge_entry(bddm, bin+1);
  bin[1]=bin[0];
}


static
cache_entry
#if defined(__STDC__)
bdd_get_entry(bdd_manager bddm, int tag, cache_entry *bin)
#else
bdd_get_entry(bddm, tag, bin)
     bdd_manager bddm;
     int tag;
     cache_entry *bin;
#endif
{
#if defined(__STDC__)
  void (*purge_fn)(bdd_manager, cache_entry);
#else
  void (*purge_fn)();
#endif
  cache_entry p;

  if (bin[0] && bin[1])
    {
      p=bin[1];
      purge_fn=bddm->op_cache.purge_fn[TAG(p)];
      p=CACHE_POINTER(p);
      if (purge_fn)
	(*purge_fn)(bddm, p);
      bddm->op_cache.collisions++;
      if (bddm->op_cache.cache_level == 0)
	bin[1]=bin[0];
      else
	++bin;
    }
  else
    {
      p=(cache_entry)BDD_NEW_REC(bddm, sizeof(struct cache_entry_));
      bddm->op_cache.entries++;
      if (bin[0])
	++bin;
    }
  *bin=(cache_entry)SET_TAG(p, tag);
  return (p);
}


static
long
#if defined(__STDC__)
bdd_rehash1(bdd_manager bddm, cache_entry p)
#else
bdd_rehash1(bddm, p)
     bdd_manager bddm;
     cache_entry p;
#endif
{
  return (HASH1(p->slot[0]));
}


static
long
#if defined(__STDC__)
bdd_rehash2(bdd_manager bddm, cache_entry p)
#else
bdd_rehash2(bddm, p)
     bdd_manager bddm;
     cache_entry p;
#endif
{
  return (HASH2(p->slot[0], p->slot[1]));
}


static
long
#if defined(__STDC__)
bdd_rehash3(bdd_manager bddm, cache_entry p)
#else
bdd_rehash3(bddm, p)
     bdd_manager bddm;
     cache_entry p;
#endif
{
  return (HASH3(p->slot[0], p->slot[1], p->slot[2]));
}


void
#if defined(__STDC__)
bdd_rehash_cache(bdd_manager bddm, int grow)
#else
bdd_rehash_cache(bddm, grow)
     bdd_manager bddm;
     int grow;
#endif
{
  long i;
  long hash;
  int j;
  long oldsize;
  cache_bin *newtable;
  cache_entry *bin;
  cache_entry *newbin;
  cache_entry p;
  cache_entry q;

  oldsize=bddm->op_cache.size;
  if (grow)
    bddm->op_cache.size_index++;
  else
    bddm->op_cache.size_index--;
  bddm->op_cache.size=TABLE_SIZE(bddm->op_cache.size_index);
  newtable=(cache_bin *)mem_get_block((SIZE_T)(bddm->op_cache.size*sizeof(struct cache_bin_)));
  for (i=0; i < bddm->op_cache.size; ++i)
    for (j=0; j < 2; ++j)
      newtable[i].entry[j]=0;
  /* Rehash LRU first. */
  for (j=1; j >= 0; --j)
    for (i=0; i < oldsize; ++i)
      {
	bin=bddm->op_cache.table[i].entry;
	if ((p=bin[j]))
	  {
	    q=CACHE_POINTER(p);
	    hash=(*bddm->op_cache.rehash_fn[TAG(p)])(bddm, q);
	    REDUCE(hash, bddm->op_cache.size);
	    newbin=newtable[hash].entry;
	    bdd_purge_lru(bddm, newbin);
	    newbin[0]=p;
	  }
    }
  mem_free_block((pointer)(bddm->op_cache.table));
  bddm->op_cache.table=newtable;
}


/* The routines bdd_insert_in_cachex insert things in the cache. */
/* The routines bdd_lookup_in_cachex look up things in the cache. */

void
#if defined(__STDC__)
bdd_insert_in_cache31(bdd_manager bddm, int tag, INT_PTR d1, INT_PTR d2, INT_PTR d3, INT_PTR result)
#else
bdd_insert_in_cache31(bddm, tag, d1, d2, d3, result)
     bdd_manager bddm;
     int tag;
     INT_PTR d1;
     INT_PTR d2;
     INT_PTR d3;
     INT_PTR result;
#endif
{
  long hash;
  cache_entry p;

  hash=HASH3(d1, d2, d3);
  REDUCE(hash, bddm->op_cache.size);
  if (hash < 0)
    hash= -hash;
  p=bdd_get_entry(bddm, tag, bddm->op_cache.table[hash].entry);
  p->slot[0]=d1;
  p->slot[1]=d2;
  p->slot[2]=d3;
  p->slot[3]=result;
  bddm->op_cache.inserts++;
}


#if defined(__STDC__)
#define RETURN_BDD_FN ((void (*)(bdd_manager, cache_entry))-1)
#else
#define RETURN_BDD_FN ((void (*)())-1)
#endif


int
#if defined(__STDC__)
bdd_lookup_in_cache31(bdd_manager bddm, int tag, INT_PTR d1, INT_PTR d2, INT_PTR d3, INT_PTR *result)
#else
bdd_lookup_in_cache31(bddm, tag, d1, d2, d3, result)
     bdd_manager bddm;
     int tag;
     INT_PTR d1;
     INT_PTR d2;
     INT_PTR d3;
     INT_PTR *result;
#endif
{
  long hash;
  cache_entry *bin;
  cache_entry p;
  cache_entry q;
  bdd f;
#if defined(__STDC__)
  void (*return_fn)(bdd_manager, cache_entry);
#else
  void (*return_fn)();
#endif

  bddm->op_cache.lookups++;
  hash=HASH3(d1, d2, d3);
  REDUCE(hash, bddm->op_cache.size);
  bin=bddm->op_cache.table[hash].entry;
  if ((p=bin[0]))
    {
      q=CACHE_POINTER(p);
      if (q->slot[0] != d1 || q->slot[1] != d2 || q->slot[2] != d3 || TAG(p) != tag)
	if ((p=bin[1]))
	  {
	    q=CACHE_POINTER(p);
	    if (q->slot[0] != d1 || q->slot[1] != d2 || q->slot[2] != d3 || TAG(p) != tag)
	      return (0);
	    bin[1]=bin[0];
	    bin[0]=p;
	  }
	else
	  return (0);
    }
  else
    return (0);
  bddm->op_cache.hits++;
  if ((return_fn=bddm->op_cache.return_fn[TAG(p)]))
    if (return_fn == RETURN_BDD_FN)
      {
	f=(bdd)q->slot[3];
	{
	  BDD_SETUP(f);
	  BDD_TEMP_INCREFS(f);
	}
      }
    else
      (*return_fn)(bddm, q);
  *result=q->slot[3];
  return (1);
}


void
#if defined(__STDC__)
bdd_insert_in_cache22(bdd_manager bddm, int tag, INT_PTR d1, INT_PTR d2, INT_PTR result1, INT_PTR result2)
#else
bdd_insert_in_cache22(bddm, tag, d1, d2, result1, result2)
     bdd_manager bddm;
     int tag;
     INT_PTR d1;
     INT_PTR d2;
     INT_PTR result1;
     INT_PTR result2;
#endif
{
  long hash;
  cache_entry p;

  hash=HASH2(d1, d2);
  REDUCE(hash, bddm->op_cache.size);
  p=bdd_get_entry(bddm, tag, bddm->op_cache.table[hash].entry);
  p->slot[0]=d1;
  p->slot[1]=d2;
  p->slot[2]=result1;
  p->slot[3]=result2;
  bddm->op_cache.inserts++;
}


int
#if defined(__STDC__)
bdd_lookup_in_cache22(bdd_manager bddm, int tag, INT_PTR d1, INT_PTR d2, INT_PTR *result1, INT_PTR *result2)
#else
bdd_lookup_in_cache22(bddm, tag, d1, d2, result1, result2)
     bdd_manager bddm;
     int tag;
     INT_PTR d1;
     INT_PTR d2;
     INT_PTR *result1;
     INT_PTR *result2;
#endif
{
  long hash;
  cache_entry *bin;
  cache_entry p;
  cache_entry q;
#if defined(__STDC__)
  void (*return_fn)(bdd_manager, cache_entry);
#else
  void (*return_fn)();
#endif

  bddm->op_cache.lookups++;
  hash=HASH2(d1, d2);
  REDUCE(hash, bddm->op_cache.size);
  bin=bddm->op_cache.table[hash].entry;
  if ((p=bin[0]))
    {
      q=CACHE_POINTER(p);
      if (q->slot[0] != d1 || q->slot[1] != d2 || TAG(p) != tag)
	if ((p=bin[1]))
	  {
	    q=CACHE_POINTER(p);
	    if (q->slot[0] != d1 || q->slot[1] != d2 || TAG(p) != tag)
	      return (0);
	    bin[1]=bin[0];
	    bin[0]=p;
	  }
	else
	  return (0);
    }
  else
    return (0);
  bddm->op_cache.hits++;
  if ((return_fn=bddm->op_cache.return_fn[TAG(p)]))
    (*return_fn)(bddm, q);
  *result1=q->slot[2];
  *result2=q->slot[3];
  return (1);
}


void
#if defined(__STDC__)
bdd_insert_in_cache13(bdd_manager bddm, int tag, INT_PTR d1, INT_PTR result1, INT_PTR result2, INT_PTR result3)
#else
bdd_insert_in_cache13(bddm, tag, d1, result1, result2, result3)
     bdd_manager bddm;
     int tag;
     INT_PTR d1;
     INT_PTR result1;
     INT_PTR result2;
     INT_PTR result3;
#endif
{
  long hash;
  cache_entry p;

  hash=HASH1(d1);
  REDUCE(hash, bddm->op_cache.size);
  p=bdd_get_entry(bddm, tag, bddm->op_cache.table[hash].entry);
  p->slot[0]=d1;
  p->slot[1]=result1;
  p->slot[2]=result2;
  p->slot[3]=result3;
  bddm->op_cache.inserts++;
}


int
#if defined(__STDC__)
bdd_lookup_in_cache13(bdd_manager bddm, int tag, INT_PTR d1, INT_PTR *result1, INT_PTR *result2, INT_PTR *result3)
#else
bdd_lookup_in_cache13(bddm, tag, d1, result1, result2, result3)
     bdd_manager bddm;
     int tag;
     INT_PTR d1;
     INT_PTR *result1;
     INT_PTR *result2;
     INT_PTR *result3;
#endif
{
  long hash;
  cache_entry *bin;
  cache_entry p;
  cache_entry q;
#if defined(__STDC__)
  void (*return_fn)(bdd_manager, cache_entry);
#else
  void (*return_fn)();
#endif

  bddm->op_cache.lookups++;
  hash=HASH1(d1);
  REDUCE(hash, bddm->op_cache.size);
  bin=bddm->op_cache.table[hash].entry;
  if ((p=bin[0]))
    {
      q=CACHE_POINTER(p);
      if (q->slot[0] != d1 || TAG(p) != tag)
	if ((p=bin[1]))
	  {
	    q=CACHE_POINTER(p);
	    if (q->slot[0] != d1 || TAG(p) != tag)
	      return (0);
	    bin[1]=bin[0];
	    bin[0]=p;
	  }
	else
	  return (0);
    }
  else
    return (0);
  bddm->op_cache.hits++;
  if ((return_fn=bddm->op_cache.return_fn[TAG(p)]))
    (*return_fn)(bddm, q);
  *result1=q->slot[1];
  *result2=q->slot[2];
  *result3=q->slot[3];
  return (1);
}


static
int
#if defined(__STDC__)
bdd_ite_gc_fn(bdd_manager bddm, cache_entry p)
#else
bdd_ite_gc_fn(bddm, p)
     bdd_manager bddm;
     cache_entry p;
#endif
{
  int i;
  bdd f;

  for (i=0; i < 4; ++i)
    {
      f=(bdd)p->slot[i];
      {
	BDD_SETUP(f);
	if (!BDD_IS_USED(f))
	  return (1);
      }
    }
  return (0);
}


static
int
#if defined(__STDC__)
bdd_two_gc_fn(bdd_manager bddm, cache_entry p)
#else
bdd_two_gc_fn(bddm, p)
     bdd_manager bddm;
     cache_entry p;
#endif
{
  int i;
  bdd f;

  for (i=1; i < 4; ++i)
    {
      f=(bdd)p->slot[i];
      {
	BDD_SETUP(f);
	if (!BDD_IS_USED(f))
	  return (1);
      }
    }
  return (0);
}


static
int
#if defined(__STDC__)
bdd_two_data_gc_fn(bdd_manager bddm, cache_entry p)
#else
bdd_two_data_gc_fn(bddm, p)
     bdd_manager bddm;
     cache_entry p;
#endif
{
  int i;
  bdd f;

  for (i=1; i < 3; ++i)
    {
      f=(bdd)p->slot[i];
      {
	BDD_SETUP(f);
	if (!BDD_IS_USED(f))
	  return (1);
      }
    }
  return (0);
}


static
int
#if defined(__STDC__)
bdd_one_data_gc_fn(bdd_manager bddm, cache_entry p)
#else
bdd_one_data_gc_fn(bddm, p)
     bdd_manager bddm;
     cache_entry p;
#endif
{
  bdd f;

  f=(bdd)p->slot[1];
  {
    BDD_SETUP(f);
    return (!BDD_IS_USED(f));
  }
}


/* bdd_purge_cache(bddm) purges the cache of any entries which mention */
/* a BDD node that is about to be garbage collected. */

void
#if defined(__STDC__)
bdd_purge_cache(bdd_manager bddm)
#else
bdd_purge_cache(bddm)
     bdd_manager bddm;
#endif
{
  long i;
  int j;
  cache_entry *bin;
  cache_entry p;
  cache_entry q;

  for (i=0; i < bddm->op_cache.size; ++i)
    {
      bin= &bddm->op_cache.table[i].entry[0];
      for (j=0; j < 2; ++j)
	if ((p=bin[j]))
	  {
	    q=CACHE_POINTER(p);
	    if ((*bddm->op_cache.gc_fn[TAG(p)])(bddm, q))
	      bdd_purge_entry(bddm, bin+j);
	    else if (j == 1 && !bin[0])
	      {
		bin[0]=bin[1];	/* LRU is only one left */
		bin[1]=0;
	      }
	  }
	else
	  break;
    }
}


/* bdd_flush_cache(bddm, flush_fn, closure) purges all entries for which */
/* the given function returns true. */

void
#if defined(__STDC__)
bdd_flush_cache(bdd_manager bddm, int (*flush_fn)(bdd_manager, cache_entry, pointer), pointer closure)
#else
bdd_flush_cache(bddm, flush_fn, closure)
     bdd_manager bddm;
     int (*flush_fn)();
     pointer closure;
#endif
{
  long i;
  int j;
  cache_entry *bin;

  for (i=0; i < bddm->op_cache.size; ++i)
    {
      bin=bddm->op_cache.table[i].entry;
      for (j=0; j < 2; ++j)
	if (bin[j])
	  {
	    if ((*flush_fn)(bddm, bin[j], closure))
	      bdd_purge_entry(bddm, bin+j);
	    else if (j == 1 && !bin[0])
	      {
		bin[0]=bin[1];	/* LRU is only one left */
		bin[1]=0;
	      }
	  }
	else
	  break;
    }
}


/* bdd_flush_all(bddm) flushes the entire cache. */

void
#if defined(__STDC__)
bdd_flush_all(bdd_manager bddm)
#else
bdd_flush_all(bddm)
     bdd_manager bddm;
#endif
{
  long i;
  int j;
  cache_entry *bin;

  for (i=0; i < bddm->op_cache.size; ++i)
    {
      bin=bddm->op_cache.table[i].entry;
      for (j=0; j < 2; ++j)
	if (bin[j])
	  bdd_purge_entry(bddm, bin+j);
	else
	  break;
    }
}


/* bdd_cache_functions(bddm, args, gc_fn, purge_fn, return_fn, flush_fn) */
/* controls the user cache types.  Allocates an unused cache entry type */
/* tag and returns the tag, or -1 if no more tags are available. */

int
#if defined(__STDC__)
bdd_cache_functions(bdd_manager bddm,
		    int args,
		    int (*gc_fn)(bdd_manager, cache_entry),
		    void (*purge_fn)(bdd_manager, cache_entry),
		    void (*return_fn)(bdd_manager, cache_entry),
		    int (*flush_fn)(bdd_manager, cache_entry, pointer))
#else
bdd_cache_functions(bddm, args, gc_fn, purge_fn, return_fn, flush_fn)
     bdd_manager bddm;
     int args;
     int (*gc_fn)();
     void (*purge_fn)();
     void (*return_fn)();
     int (*flush_fn)();
#endif
{
#if defined(__STDC__)
  long (*rehash_fn)(bdd_manager, cache_entry);
#else
  long (*rehash_fn)();
#endif
  int i;

  if (args == 1)
    rehash_fn=bdd_rehash1;
  else if (args == 2)
    rehash_fn=bdd_rehash2;
  else if (args == 3)
    rehash_fn=bdd_rehash3;
  else
    {
      rehash_fn=0;
      bdd_fatal(_T("bdd_cache_functions: illegal number of cache arguments"));
    }
  for (i=CACHE_TYPE_USER1; i < CACHE_TYPE_USER1+USER_ENTRY_TYPES; ++i)
    if (!bddm->op_cache.rehash_fn[i])
      break;
  if (i == CACHE_TYPE_USER1+USER_ENTRY_TYPES)
    return (-1);
  bddm->op_cache.rehash_fn[i]=rehash_fn;
  bddm->op_cache.gc_fn[i]=gc_fn;
  bddm->op_cache.purge_fn[i]=purge_fn;
  bddm->op_cache.return_fn[i]=return_fn;
  bddm->op_cache.flush_fn[i]=flush_fn;
  return (i);
}


static
int
#if defined(__STDC__)
bdd_flush_tag(bdd_manager bddm, cache_entry p, pointer tag)
#else
bdd_flush_tag(bddm, p, tag)
     bdd_manager bddm;
     cache_entry p;
     pointer tag;
#endif
{
  return (TAG(p) == (int)tag);
}


/* bdd_free_cache_tag(bddm, tag) frees a previously allocated user */
/* cache tag. */

void
#if defined(__STDC__)
bdd_free_cache_tag(bdd_manager bddm, int tag)
#else
bdd_free_cache_tag(bddm, tag)
     bdd_manager bddm;
     int tag;
#endif
{
  if (tag < CACHE_TYPE_USER1 ||
      tag >= CACHE_TYPE_USER1+USER_ENTRY_TYPES ||
      !bddm->op_cache.rehash_fn[tag])
    bdd_fatal(_T("bdd_free_cache_tag: attempt to free unallocated tag"));
  bdd_flush_cache(bddm, bdd_flush_tag, (pointer)tag);
  bddm->op_cache.rehash_fn[tag]=0;
  bddm->op_cache.gc_fn[tag]=0;
  bddm->op_cache.purge_fn[tag]=0;
  bddm->op_cache.return_fn[tag]=0;
  bddm->op_cache.flush_fn[tag]=0;
}


static
int
#if defined(__STDC__)
bdd_two_flush_fn(bdd_manager bddm, cache_entry p, pointer closure)
#else
bdd_two_flush_fn(bddm, p, closure)
     bdd_manager bddm;
     cache_entry p;
     pointer closure;
#endif
{
  int id_to_nuke;

  id_to_nuke=(int)closure;
  return (p->slot[0] == OP_RELPROD+id_to_nuke ||
	  p->slot[0] == OP_QNT+id_to_nuke ||
	  p->slot[0] == OP_SUBST+id_to_nuke);
}


/* bdd_init_cache(bddm) initializes the cache for a BDD manager. */

void
#if defined(__STDC__)
bdd_init_cache(bdd_manager bddm)
#else
bdd_init_cache(bddm)
     bdd_manager bddm;
#endif
{
  long i;
  int j;

  bddm->op_cache.size_index=13;
  bddm->op_cache.size=TABLE_SIZE(bddm->op_cache.size_index);
  bddm->op_cache.table=(cache_bin *)mem_get_block((SIZE_T)(bddm->op_cache.size*sizeof(cache_bin)));
  for (i=0; i < bddm->op_cache.size; ++i)
    for (j=0; j < 2; ++j)
      bddm->op_cache.table[i].entry[j]=0;
  /* ITE cache control functions. */
  bddm->op_cache.rehash_fn[CACHE_TYPE_ITE]=bdd_rehash3;
  bddm->op_cache.gc_fn[CACHE_TYPE_ITE]=bdd_ite_gc_fn;
  bddm->op_cache.purge_fn[CACHE_TYPE_ITE]=0;
  bddm->op_cache.return_fn[CACHE_TYPE_ITE]=RETURN_BDD_FN;
  bddm->op_cache.flush_fn[CACHE_TYPE_ITE]=0;
  /* Two argument op cache control functions. */
  bddm->op_cache.rehash_fn[CACHE_TYPE_TWO]=bdd_rehash3;
  bddm->op_cache.gc_fn[CACHE_TYPE_TWO]=bdd_two_gc_fn;
  bddm->op_cache.purge_fn[CACHE_TYPE_TWO]=0;
  bddm->op_cache.return_fn[CACHE_TYPE_TWO]=RETURN_BDD_FN;
  bddm->op_cache.flush_fn[CACHE_TYPE_TWO]=bdd_two_flush_fn;
  /* One argument op w/ data result cache control functions. */
  bddm->op_cache.rehash_fn[CACHE_TYPE_ONEDATA]=bdd_rehash2;
  bddm->op_cache.gc_fn[CACHE_TYPE_ONEDATA]=bdd_one_data_gc_fn;
  bddm->op_cache.purge_fn[CACHE_TYPE_ONEDATA]=0;
  bddm->op_cache.return_fn[CACHE_TYPE_ONEDATA]=0;
  bddm->op_cache.flush_fn[CACHE_TYPE_ONEDATA]=0;
  /* Two argument op w/ data result cache control functions. */
  bddm->op_cache.rehash_fn[CACHE_TYPE_TWODATA]=bdd_rehash3;
  bddm->op_cache.gc_fn[CACHE_TYPE_TWODATA]=bdd_two_data_gc_fn;
  bddm->op_cache.purge_fn[CACHE_TYPE_TWODATA]=0;
  bddm->op_cache.return_fn[CACHE_TYPE_TWODATA]=0;
  bddm->op_cache.flush_fn[CACHE_TYPE_TWODATA]=0;
  /* User-defined cache control functions. */
  for (j=CACHE_TYPE_USER1; j < CACHE_TYPE_USER1+USER_ENTRY_TYPES; ++j)
    {
      bddm->op_cache.rehash_fn[j]=0;
      bddm->op_cache.gc_fn[j]=0;
      bddm->op_cache.purge_fn[j]=0;
      bddm->op_cache.return_fn[j]=0;
      bddm->op_cache.flush_fn[j]=0;
    }
  bddm->op_cache.cache_ratio=4;
  bddm->op_cache.cache_level=0;
  bddm->op_cache.entries=0;
  bddm->op_cache.lookups=0;
  bddm->op_cache.hits=0;
  bddm->op_cache.inserts=0;
  bddm->op_cache.collisions=0;
}


/* bdd_free_cache(bddm) frees the storage associated with the cache of */
/* the indicated BDD manager. */

void
#if defined(__STDC__)
bdd_free_cache(bdd_manager bddm)
#else
bdd_free_cache(bddm)
     bdd_manager bddm;
#endif
{
  bdd_flush_all(bddm);
  mem_free_block((pointer)bddm->op_cache.table);
}
