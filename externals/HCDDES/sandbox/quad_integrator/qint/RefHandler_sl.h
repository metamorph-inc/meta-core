#ifndef _RefHandler_sl_H_
#define _RefHandler_sl_H_

#ifndef NO_MATH_H
#include <math.h>
#endif
#ifndef min
#define min(x,y) (((x)>(y)) ? (y) : (x))
#endif


#ifndef single_GUARD
#define single_GUARD
typedef float single;
#endif
#ifndef A5singleA_GUARD
#define A5singleA_GUARD
typedef float A5singleA[5];
#endif
#ifndef A9singleA_GUARD
#define A9singleA_GUARD
typedef float A9singleA[9];
#endif
#ifndef A1A9singleAA_GUARD
#define A1A9singleAA_GUARD
typedef float A1A9singleAA[9];
#endif
#ifndef A1A5singleAA_GUARD
#define A1A5singleAA_GUARD
typedef float A1A5singleAA[5];
#endif

#ifndef RefHandler_100000666_context_GUARD
#define RefHandler_100000666_context_GUARD
typedef struct {
  float Gain130;
} RefHandler_100000666_context;
#endif

/* SIMPLIFIED PROGRAM CONTEXT */
typedef RefHandler_100000666_context RefHandler_context;

void RefHandler_100000666_main( RefHandler_100000666_context *context, float pos_ref_in_2, float *pos_ref_out_3 );
void RefHandler_100000666_init( RefHandler_100000666_context *context );



/* SIMPLIFIED PROGRAM FUNCTIONS */
void RefHandler_main( RefHandler_100000666_context *context, float pos_ref_in_2, float *pos_ref_out_3 );
void RefHandler_init( RefHandler_100000666_context *context );
#endif
