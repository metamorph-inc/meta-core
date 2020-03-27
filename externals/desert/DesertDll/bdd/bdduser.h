/* BDD user-visible definitions */


#if !defined(_BDDUSERH)
#define _BDDUSERH


#include <stdio.h>
#include <wtypes.h>
#include "mem/memuser.h"

#define BDD_MAX_PATHS 100000

#if defined(__STDC__)
#define ARGS(args) args
#else
#define ARGS(args) ()
#endif


/* Types */

typedef struct bdd_ *bdd;
typedef struct bdd_manager_ *bdd_manager;
typedef struct block_ *block;

// Himanshu: Added for math function
typedef enum {
	bdd_sin, 
	bdd_cos, 
	bdd_tan, 
	bdd_asin, 
	bdd_acos, 
	bdd_atan, 
	bdd_sinh, 
	bdd_cosh, 
	bdd_tanh, 
	bdd_asinh, 
	bdd_acosh, 
	bdd_atanh, 
	bdd_log2, 
	bdd_log10, 
	bdd_ln, 
	bdd_exp, 
	bdd_sqrt, 
	bdd_sign, 
	bdd_rint, 
	bdd_abs 
} bdd_unary_math_function;


/* Return values for bdd_type */

#define BDD_TYPE_NONTERMINAL 0
#define BDD_TYPE_ZERO 1
#define BDD_TYPE_ONE 2
#define BDD_TYPE_POSVAR 3
#define BDD_TYPE_NEGVAR 4
#define BDD_TYPE_OVERFLOW 5
#define BDD_TYPE_CONSTANT 6


/* Error codes for bdd_undump_bdd */

#define BDD_UNDUMP_FORMAT 1
#define BDD_UNDUMP_OVERFLOW 2
#define BDD_UNDUMP_IOERROR 3
#define BDD_UNDUMP_EOF 4


/* Basic BDD routine declarations */

extern DESERT_API bdd bdd_one ARGS((bdd_manager));
extern DESERT_API bdd bdd_zero ARGS((bdd_manager));
extern DESERT_API bdd bdd_new_var_first ARGS((bdd_manager));
extern DESERT_API bdd bdd_new_var_last ARGS((bdd_manager));
extern DESERT_API bdd bdd_new_var_before ARGS((bdd_manager, bdd));
extern DESERT_API bdd bdd_new_var_after ARGS((bdd_manager, bdd));
extern DESERT_API bdd bdd_var_with_index ARGS((bdd_manager, long));
extern DESERT_API bdd bdd_var_with_id ARGS((bdd_manager, long));
extern DESERT_API bdd bdd_ite ARGS((bdd_manager, bdd, bdd, bdd));
extern DESERT_API bdd bdd_and ARGS((bdd_manager, bdd, bdd));
extern DESERT_API bdd bdd_nand ARGS((bdd_manager, bdd, bdd));
extern DESERT_API bdd bdd_or ARGS((bdd_manager, bdd, bdd));
extern DESERT_API bdd bdd_nor ARGS((bdd_manager, bdd, bdd));
extern DESERT_API bdd bdd_xor ARGS((bdd_manager, bdd, bdd));
extern DESERT_API bdd bdd_xnor ARGS((bdd_manager, bdd, bdd));
extern DESERT_API bdd bdd_identity ARGS((bdd_manager, bdd));
extern DESERT_API bdd bdd_not ARGS((bdd_manager, bdd));
extern DESERT_API bdd bdd_if ARGS((bdd_manager, bdd));
extern DESERT_API long bdd_if_index ARGS((bdd_manager, bdd));
extern DESERT_API long bdd_if_id ARGS((bdd_manager, bdd));
extern DESERT_API bdd bdd_then ARGS((bdd_manager, bdd));
extern DESERT_API bdd bdd_else ARGS((bdd_manager, bdd));
extern DESERT_API bdd bdd_intersects ARGS((bdd_manager, bdd, bdd));
extern DESERT_API bdd bdd_implies ARGS((bdd_manager, bdd, bdd));
extern DESERT_API int bdd_type ARGS((bdd_manager, bdd));
extern DESERT_API void bdd_unfree ARGS((bdd_manager, bdd));
extern DESERT_API void bdd_free ARGS((bdd_manager, bdd));
extern DESERT_API long bdd_vars ARGS((bdd_manager));
extern DESERT_API long bdd_total_size ARGS((bdd_manager));
extern DESERT_API int bdd_cache_ratio ARGS((bdd_manager, int));
extern DESERT_API long bdd_node_limit ARGS((bdd_manager, long));
extern DESERT_API int bdd_overflow ARGS((bdd_manager));
extern DESERT_API void bdd_overflow_closure ARGS((bdd_manager, void (*) ARGS((bdd_manager, pointer)), pointer));
extern DESERT_API void bdd_abort_closure ARGS((bdd_manager, void (*) ARGS((bdd_manager, pointer)), pointer));
extern DESERT_API void bdd_stats ARGS((bdd_manager, FILE *));
extern DESERT_API bdd_manager bdd_init ARGS((void));
extern DESERT_API TCHAR *bdd_version ARGS((void));
extern DESERT_API void bdd_quit ARGS((bdd_manager));


/* Variable association routine declarations */

extern int bdd_new_assoc ARGS((bdd_manager, bdd *, int));
extern void bdd_free_assoc ARGS((bdd_manager, int));
extern void bdd_temp_assoc ARGS((bdd_manager, bdd *, int));
extern void bdd_augment_temp_assoc ARGS((bdd_manager, bdd *, int));
extern int bdd_assoc ARGS((bdd_manager, int));


/* Comparison routine declarations */

extern DESERT_API int bdd_compare ARGS((bdd_manager, bdd, bdd, bdd));


/* Composition routine declarations */

extern DESERT_API bdd bdd_compose ARGS((bdd_manager, bdd, bdd, bdd));
extern DESERT_API bdd bdd_substitute ARGS((bdd_manager, bdd));


/* Variable exchange routine declarations */

extern DESERT_API bdd bdd_swap_vars ARGS((bdd_manager, bdd, bdd, bdd));


/* Quantification routine declarations */

extern DESERT_API bdd bdd_exists ARGS((bdd_manager, bdd));
extern DESERT_API bdd bdd_forall ARGS((bdd_manager, bdd));


/* Reduce routine declarations */

extern DESERT_API bdd bdd_reduce ARGS((bdd_manager, bdd, bdd));
extern DESERT_API bdd bdd_cofactor ARGS((bdd_manager, bdd, bdd));


/* Relational product routine declarations */

extern DESERT_API bdd bdd_rel_prod ARGS((bdd_manager, bdd, bdd));


/* Satisfying valuation routine declarations */

extern DESERT_API bdd bdd_satisfy ARGS((bdd_manager, bdd));
extern DESERT_API bdd bdd_satisfy_support ARGS((bdd_manager, bdd));
extern DESERT_API double bdd_satisfying_fraction ARGS((bdd_manager, bdd));

extern DESERT_API void bdd_sat_f ARGS((bdd_manager, bdd, int, int));  /* JASON */
extern DESERT_API void bdd_sat_f_mat ARGS((bdd_manager, bdd, int, int, int **, int *)); 

/* Generic apply routine declarations */

extern DESERT_API bdd bdd_apply2 ARGS((bdd_manager, bdd (*) ARGS((bdd_manager, bdd *, bdd *, pointer)), bdd, bdd, pointer));
extern DESERT_API bdd bdd_apply1 ARGS((bdd_manager, bdd (*) ARGS((bdd_manager, bdd *, pointer)), bdd, pointer));


/* Size and profile routine declarations */

extern DESERT_API long bdd_size ARGS((bdd_manager, bdd, int));
extern DESERT_API long bdd_size_multiple ARGS((bdd_manager, bdd *, int));
extern DESERT_API void bdd_profile ARGS((bdd_manager, bdd, long *, int));
extern DESERT_API void bdd_profile_multiple ARGS((bdd_manager, bdd *, long *, int));
extern DESERT_API void bdd_function_profile ARGS((bdd_manager, bdd, long *));
extern DESERT_API void bdd_function_profile_multiple ARGS((bdd_manager, bdd *, long *));


/* Print routine declarations */

#if defined(__STDC__)
#define bdd_naming_fn_none ((TCHAR *(*)(bdd_manager, bdd, pointer))0)
#define bdd_terminal_id_fn_none ((TCHAR *(*)(bdd_manager, INT_PTR, INT_PTR, pointer))0)
#else
#define bdd_naming_fn_none ((TCHAR *(*)())0)
#define bdd_terminal_id_fn_none ((TCHAR *(*)())0)
#endif

extern DESERT_API void bdd_print_bdd ARGS((bdd_manager,
				bdd,
				TCHAR *(*) ARGS((bdd_manager, bdd, pointer)),
				TCHAR *(*) ARGS((bdd_manager, INT_PTR, INT_PTR, pointer)),
				pointer,
				FILE *));
extern DESERT_API void bdd_print_profile_aux ARGS((bdd_manager,
					long *,
					TCHAR *(*) ARGS((bdd_manager, bdd, pointer)),
					pointer,
					int,
					FILE *));
extern DESERT_API void bdd_print_profile ARGS((bdd_manager,
				    bdd,
				    TCHAR *(*) ARGS((bdd_manager, bdd, pointer)),
				    pointer,
				    int,
				    FILE *));
extern DESERT_API void bdd_print_profile_multiple ARGS((bdd_manager,
					     bdd *,
					     TCHAR *(*) ARGS((bdd_manager, bdd, pointer)),
					     pointer,
					     int,
					     FILE *));
extern DESERT_API void bdd_print_function_profile ARGS((bdd_manager,
					     bdd,
					     TCHAR *(*) ARGS((bdd_manager, bdd, pointer)),
					     pointer,
					     int,
					     FILE *));
extern DESERT_API void bdd_print_function_profile_multiple ARGS((bdd_manager,
						      bdd *,
						      TCHAR *(*) ARGS((bdd_manager, bdd, pointer)),
						      pointer,
						      int,
						      FILE *));


/* Dump/undump routine declarations */

extern DESERT_API int bdd_dump_bdd ARGS((bdd_manager, bdd, bdd *, FILE *));
extern DESERT_API bdd bdd_undump_bdd ARGS((bdd_manager, bdd *, FILE *, int *));


/* Support routine declarations */

extern DESERT_API int bdd_depends_on ARGS((bdd_manager, bdd, bdd));
extern DESERT_API void bdd_support ARGS((bdd_manager, bdd, bdd *));


/* Unique table routine declarations */

extern DESERT_API void bdd_gc ARGS((bdd_manager));
extern DESERT_API void bdd_clear_refs ARGS((bdd_manager));


/* Dynamic reordering routines */

#if defined(__STDC__)
#define bdd_reorder_none ((void (*)(bdd_manager))0)
#else
#define bdd_reorder_none ((void (*)())0)
#endif

extern DESERT_API void bdd_reorder_stable_window3 ARGS((bdd_manager));
extern DESERT_API void bdd_reorder_sift ARGS((bdd_manager));
extern DESERT_API void bdd_reorder_hybrid ARGS((bdd_manager));
extern DESERT_API void bdd_var_block_reorderable ARGS((bdd_manager, block, int));
extern DESERT_API void bdd_dynamic_reordering ARGS((bdd_manager, void (*) ARGS((bdd_manager))));
extern DESERT_API void bdd_reorder ARGS((bdd_manager));


/* Variable block routines */

extern block bdd_new_var_block ARGS((bdd_manager, bdd, long));


/* Multi-terminal BDD routine declarations */

extern DESERT_API void mtbdd_transform_closure ARGS((bdd_manager,
					  int (*) ARGS((bdd_manager, INT_PTR, INT_PTR, pointer)),
					  void (*) ARGS((bdd_manager, INT_PTR, INT_PTR, INT_PTR *, INT_PTR *, pointer)),
					  pointer));
extern DESERT_API int mtbdd_one_data ARGS((bdd_manager, INT_PTR, INT_PTR));
extern DESERT_API void mtbdd_free_terminal_closure ARGS((bdd_manager,
					      void (*) ARGS((bdd_manager, INT_PTR, INT_PTR, pointer)),
					      pointer));

extern DESERT_API bdd mtbdd_get_terminal ARGS((bdd_manager, INT_PTR, INT_PTR));
extern DESERT_API void mtbdd_terminal_value ARGS((bdd_manager, bdd, INT_PTR *, INT_PTR *));

//two bdd functions
extern DESERT_API bdd mtbdd_ite ARGS((bdd_manager, bdd, bdd, bdd));
extern DESERT_API bdd mtbdd_equal ARGS((bdd_manager, bdd, bdd));
extern DESERT_API bdd mtbdd_sum ARGS((bdd_manager, bdd, bdd));
extern DESERT_API bdd mtbdd_sub ARGS((bdd_manager, bdd, bdd));
extern DESERT_API bdd mtbdd_mul ARGS((bdd_manager, bdd, bdd));
extern DESERT_API bdd mtbdd_div ARGS((bdd_manager, bdd, bdd));
extern DESERT_API bdd mtbdd_max ARGS((bdd_manager, bdd, bdd));
extern DESERT_API bdd mtbdd_min ARGS((bdd_manager, bdd, bdd));


//one bdd functions
#ifndef DOUBLE_MTBDD
extern DESERT_API bdd mtbdd_threshold ARGS((bdd_manager, bdd, INT_PTR, INT_PTR, unsigned long, int eq));
extern DESERT_API bdd mtbdd_low_threshold ARGS((bdd_manager, bdd, INT_PTR, INT_PTR, unsigned long, int eq));
extern DESERT_API bdd mtbdd_equals_to ARGS((bdd_manager, bdd, INT_PTR, INT_PTR, unsigned long));
extern DESERT_API bdd mtbdd_not_equals_to ARGS((bdd_manager, bdd, INT_PTR, INT_PTR, unsigned long));
extern DESERT_API bdd mtbdd_sum_thresh ARGS((bdd_manager, bdd, bdd, INT_PTR, INT_PTR, unsigned long));
extern DESERT_API bdd mtbdd_mul_thresh ARGS((bdd_manager, bdd, bdd, INT_PTR, INT_PTR, unsigned long));
extern DESERT_API bdd mtbdd_divby ARGS((bdd_manager, bdd, INT_PTR, INT_PTR));
extern DESERT_API bdd mtbdd_div_from ARGS((bdd_manager, bdd, INT_PTR, INT_PTR));
extern DESERT_API bdd mtbdd_rootby ARGS((bdd_manager, bdd, INT_PTR, INT_PTR));
extern DESERT_API bdd mtbdd_sumby ARGS((bdd_manager, bdd, INT_PTR, INT_PTR));
extern DESERT_API bdd mtbdd_subby ARGS((bdd_manager, bdd, INT_PTR, INT_PTR));
extern DESERT_API bdd mtbdd_mulby ARGS((bdd_manager, bdd, INT_PTR, INT_PTR));
#else
extern DESERT_API bdd mtbdd_threshold ARGS((bdd_manager, bdd, double, double, unsigned long, int eq));
extern DESERT_API bdd mtbdd_low_threshold ARGS((bdd_manager, bdd, double, double, unsigned long, int eq));
extern DESERT_API bdd mtbdd_equals_to ARGS((bdd_manager, bdd, double, double, unsigned long));
extern DESERT_API bdd mtbdd_not_equals_to ARGS((bdd_manager, bdd, double, double, unsigned long));
extern DESERT_API bdd mtbdd_sum_thresh ARGS((bdd_manager, bdd, bdd, double, double, unsigned long));
extern DESERT_API bdd mtbdd_mul_thresh ARGS((bdd_manager, bdd, bdd, double, double, unsigned long));
extern DESERT_API bdd mtbdd_divby ARGS((bdd_manager, bdd, double, double));
extern DESERT_API bdd mtbdd_div_from ARGS((bdd_manager, bdd, double, double));
extern DESERT_API bdd mtbdd_rootby ARGS((bdd_manager, bdd, double, double));
extern DESERT_API bdd mtbdd_sumby ARGS((bdd_manager, bdd, double, double));
extern DESERT_API bdd mtbdd_subby ARGS((bdd_manager, bdd, double, double));
extern DESERT_API bdd mtbdd_mulby ARGS((bdd_manager, bdd, double, double));
extern DESERT_API bdd mtbdd_apply_unary_math_function ARGS((bdd_manager, bdd, bdd_unary_math_function));
#endif

extern DESERT_API bdd mtbdd_substitute ARGS((bdd_manager, bdd));
#define mtbdd_transform(bddm, f) (bdd_not(bddm, f))


#undef ARGS

#endif
