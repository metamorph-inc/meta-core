#ifndef _InnerLoop_sl_H_
#define _InnerLoop_sl_H_

#ifndef NO_MATH_H
#include <math.h>
#endif
#ifndef min
#define min(x,y) (((x)>(y)) ? (y) : (x))
#endif

#include "Ssignal1CsingleCsignal2CsingleCsignal3CsingleCsignal4CsingleCsignal5CsingleCsignal6CsingleCsignal7CsingleCsignal8CsingleCsignal9CsingleS.h"

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

#ifndef LeadComp_100000695_context_GUARD
#define LeadComp_100000695_context_GUARD
typedef struct {
  float X0309;
  float X0313;
  float Gain342;
  float Gain357;
  float Gain372;
} LeadComp_100000695_context;
#endif

#ifndef omega_est_100000717_context_GUARD
#define omega_est_100000717_context_GUARD
typedef struct {
  float X0317;
  float X0321;
  float Gain347;
  float Gain362;
  float Gain377;
} omega_est_100000717_context;
#endif

#ifndef InnerLoop_100000634_context_GUARD
#define InnerLoop_100000634_context_GUARD
typedef struct {
  float Gain163;
  float Gain180;
  omega_est_100000717_context omega_est_100000717_class_member2;
  LeadComp_100000695_context LeadComp_100000695_class_member3;
} InnerLoop_100000634_context;
#endif

#ifndef LeadComp_100000695_context_GUARD
#define LeadComp_100000695_context_GUARD
typedef struct {
  float X0309;
  float X0313;
  float Gain342;
  float Gain357;
  float Gain372;
} LeadComp_100000695_context;
#endif

#ifndef omega_est_100000717_context_GUARD
#define omega_est_100000717_context_GUARD
typedef struct {
  float X0317;
  float X0321;
  float Gain347;
  float Gain362;
  float Gain377;
} omega_est_100000717_context;
#endif

/* SIMPLIFIED PROGRAM CONTEXT */
typedef InnerLoop_100000634_context InnerLoop_context;

void InnerLoop_100000634_main( InnerLoop_100000634_context *context, float ang_ref_2[5], float angle_3, float (*Torque_4)[9] );
void InnerLoop_100000634_init( InnerLoop_100000634_context *context );



/* SIMPLIFIED PROGRAM FUNCTIONS */
void InnerLoop_main( InnerLoop_100000634_context *context, float ang_ref_2[5], float angle_3, float (*Torque_4)[9] );
void InnerLoop_init( InnerLoop_100000634_context *context );
#endif
