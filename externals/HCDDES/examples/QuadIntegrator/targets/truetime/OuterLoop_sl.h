#ifndef _OuterLoop_sl_H_
#define _OuterLoop_sl_H_

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

#ifndef vel_est_100000117_context_GUARD
#define vel_est_100000117_context_GUARD
typedef struct {
  float X0151;
  float X0155;
  float Gain178;
  float Gain193;
  float Gain208;
} vel_est_100000117_context;
#endif

#ifndef OuterLoop_100000095_context_GUARD
#define OuterLoop_100000095_context_GUARD
typedef struct {
  float Gain86;
  float Gain104;
  double LowerLimit115;
  double UpperLimit116;
  vel_est_100000117_context vel_est_100000117_class_member4;
} OuterLoop_100000095_context;
#endif

#ifndef vel_est_100000117_context_GUARD
#define vel_est_100000117_context_GUARD
typedef struct {
  float X0151;
  float X0155;
  float Gain178;
  float Gain193;
  float Gain208;
} vel_est_100000117_context;
#endif

/* SIMPLIFIED PROGRAM CONTEXT */
typedef OuterLoop_100000095_context OuterLoop_context;

void OuterLoop_100000095_main( OuterLoop_100000095_context *context, float pos_ref_2, float pos_3, float *ang_ref_5 );
void OuterLoop_100000095_init( OuterLoop_100000095_context *context );



/* SIMPLIFIED PROGRAM FUNCTIONS */
void OuterLoop_main( OuterLoop_100000095_context *context, float pos_ref_2, float pos_3, float *ang_ref_5 );
void OuterLoop_init( OuterLoop_100000095_context *context );
#endif
