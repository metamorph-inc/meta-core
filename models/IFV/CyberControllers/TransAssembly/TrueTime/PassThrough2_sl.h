#ifndef _PassThrough2_sl_H_
#define _PassThrough2_sl_H_

#ifndef NO_MATH_H
#include <math.h>
#endif
#ifndef min
#define min(x,y) (((x)>(y)) ? (y) : (x))
#endif

#ifndef sign
#define sign(x) ( ((x) > 0.0) ? (1.0)  : (((x) < 0.0 ) ? (-1.0) : (0.0)) )
#endif


#ifndef unsigned_char_GUARD
#define unsigned_char_GUARD
typedef char unsigned_char;
#endif

#ifndef PassThrough2_100000186_context_GUARD
#define PassThrough2_100000186_context_GUARD
typedef struct {
  float Gain863;
  float Gain874;
} PassThrough2_100000186_context;
#endif

/* SIMPLIFIED PROGRAM CONTEXT */
typedef PassThrough2_100000186_context PassThrough2_context;

void PassThrough2_100000186_main( PassThrough2_100000186_context *context, float In1_2, float In2_3, float *Out1_4, float *Out2_5 );
void PassThrough2_100000186_init( PassThrough2_100000186_context *context );



/* SIMPLIFIED PROGRAM FUNCTIONS */
void PassThrough2_main( PassThrough2_100000186_context *context, float In1_2, float In2_3, float *Out1_4, float *Out2_5 );
void PassThrough2_init( PassThrough2_100000186_context *context );
#endif
