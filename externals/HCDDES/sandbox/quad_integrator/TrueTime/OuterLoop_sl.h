#ifndef _OuterLoop_sl_H_
#define _OuterLoop_sl_H_

#ifndef NO_MATH_H
#include <math.h>
#endif
#ifndef min
#define min(x,y) (((x)>(y)) ? (y) : (x))
#endif

#include "Ssignal1CsingleCsignal2CsingleCsignal3CsingleCsignal4CsingleCsignal5CsingleS.h"

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

#ifndef vel_est_100000787_context_GUARD
#define vel_est_100000787_context_GUARD
typedef struct {
  float X0325;
  float X0329;
  float Gain352;
  float Gain367;
  float Gain382;
} vel_est_100000787_context;
#endif

#ifndef OuterLoop_100000642_context_GUARD
#define OuterLoop_100000642_context_GUARD
typedef struct {
  float Gain158;
  float Gain174;
  float LowerLimit185;
  float UpperLimit186;
  vel_est_100000787_context vel_est_100000787_class_member4;
} OuterLoop_100000642_context;
#endif

#ifndef vel_est_100000787_context_GUARD
#define vel_est_100000787_context_GUARD
typedef struct {
  float X0325;
  float X0329;
  float Gain352;
  float Gain367;
  float Gain382;
} vel_est_100000787_context;
#endif

/* SIMPLIFIED PROGRAM CONTEXT */
typedef OuterLoop_100000642_context OuterLoop_context;

void OuterLoop_100000642_main( OuterLoop_100000642_context *context, float pos_ref_2, float pos_3, float (*ang_ref_4)[5] );
void OuterLoop_100000642_init( OuterLoop_100000642_context *context );



/* SIMPLIFIED PROGRAM FUNCTIONS */
void OuterLoop_main( OuterLoop_100000642_context *context, float pos_ref_2, float pos_3, float (*ang_ref_4)[5] );
void OuterLoop_init( OuterLoop_100000642_context *context );
#endif
