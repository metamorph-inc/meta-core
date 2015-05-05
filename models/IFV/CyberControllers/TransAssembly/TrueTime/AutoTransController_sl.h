#ifndef _AutoTransController_sl_H_
#define _AutoTransController_sl_H_

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

#ifndef AutoTransController_100000117_context_GUARD
#define AutoTransController_100000117_context_GUARD
typedef struct {
  float X0592;
  float Value597;
  float Value601;
} AutoTransController_100000117_context;
#endif

/* SIMPLIFIED PROGRAM CONTEXT */
typedef AutoTransController_100000117_context AutoTransController_context;

void AutoTransController_100000117_main( AutoTransController_100000117_context *context, float w_2, float *gear_ratio_3, float *gear_num_4 );
void AutoTransController_100000117_init( AutoTransController_100000117_context *context );



/* SIMPLIFIED PROGRAM FUNCTIONS */
void AutoTransController_main( AutoTransController_100000117_context *context, float w_2, float *gear_ratio_3, float *gear_num_4 );
void AutoTransController_init( AutoTransController_100000117_context *context );
#endif
