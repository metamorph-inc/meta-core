#ifndef _PassThrough_sl_H_
#define _PassThrough_sl_H_

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

#ifndef PassThrough_100000204_context_GUARD
#define PassThrough_100000204_context_GUARD
typedef struct {
  float Gain869;
} PassThrough_100000204_context;
#endif

/* SIMPLIFIED PROGRAM CONTEXT */
typedef PassThrough_100000204_context PassThrough_context;

void PassThrough_100000204_main( PassThrough_100000204_context *context, float In_2, float *Out_3 );
void PassThrough_100000204_init( PassThrough_100000204_context *context );



/* SIMPLIFIED PROGRAM FUNCTIONS */
void PassThrough_main( PassThrough_100000204_context *context, float In_2, float *Out_3 );
void PassThrough_init( PassThrough_100000204_context *context );
#endif
