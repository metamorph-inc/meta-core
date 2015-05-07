#ifndef _InnerLoop_sl_H_
#define _InnerLoop_sl_H_

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

#ifndef LeadComp_100000033_context_GUARD
#define LeadComp_100000033_context_GUARD
typedef struct {
  float X0135;
  float X0139;
  float Gain168;
  float Gain183;
  float Gain198;
} LeadComp_100000033_context;
#endif

#ifndef omega_est_100000064_context_GUARD
#define omega_est_100000064_context_GUARD
typedef struct {
  float X0143;
  float X0147;
  float Gain173;
  float Gain188;
  float Gain203;
} omega_est_100000064_context;
#endif

#ifndef InnerLoop_100000021_context_GUARD
#define InnerLoop_100000021_context_GUARD
typedef struct {
  float Gain81;
  float Gain98;
  omega_est_100000064_context omega_est_100000064_class_member2;
  LeadComp_100000033_context LeadComp_100000033_class_member3;
} InnerLoop_100000021_context;
#endif

#ifndef LeadComp_100000033_context_GUARD
#define LeadComp_100000033_context_GUARD
typedef struct {
  float X0135;
  float X0139;
  float Gain168;
  float Gain183;
  float Gain198;
} LeadComp_100000033_context;
#endif

#ifndef omega_est_100000064_context_GUARD
#define omega_est_100000064_context_GUARD
typedef struct {
  float X0143;
  float X0147;
  float Gain173;
  float Gain188;
  float Gain203;
} omega_est_100000064_context;
#endif

/* SIMPLIFIED PROGRAM CONTEXT */
typedef InnerLoop_100000021_context InnerLoop_context;

void InnerLoop_100000021_main( InnerLoop_100000021_context *context, float ang_ref_2, float angle_3, float *ang_err_4, float *Torque_5, float *ang_vel_6 );
void InnerLoop_100000021_init( InnerLoop_100000021_context *context );



/* SIMPLIFIED PROGRAM FUNCTIONS */
void InnerLoop_main( InnerLoop_100000021_context *context, float ang_ref_2, float angle_3, float *ang_err_4, float *Torque_5, float *ang_vel_6 );
void InnerLoop_init( InnerLoop_100000021_context *context );
#endif
