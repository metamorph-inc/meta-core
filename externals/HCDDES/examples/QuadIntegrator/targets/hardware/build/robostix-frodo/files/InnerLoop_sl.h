#ifndef _InnerLoop_sl_H_
#define _InnerLoop_sl_H_

#include <math.h>
#ifndef min
#define min(x,y) (((x)>(y)) ? (y) : (x))
#endif


#ifndef MsingleC1C1M_GUARD
#define MsingleC1C1M_GUARD
typedef float MsingleC1C1M;
#endif
#ifndef MintC1C1M_GUARD
#define MintC1C1M_GUARD
typedef int MintC1C1M;
#endif
#ifndef MdoubleC1C1M_GUARD
#define MdoubleC1C1M_GUARD
typedef double MdoubleC1C1M;
#endif

#ifndef LeadComp_100000028_context_GUARD
#define LeadComp_100000028_context_GUARD
typedef struct {
  float X0135;
  float X0139;
  double Gain168;
  double Gain183;
  double Gain198;
} LeadComp_100000028_context;
#endif

#ifndef omega_est_100000059_context_GUARD
#define omega_est_100000059_context_GUARD
typedef struct {
  float X0143;
  float X0147;
  double Gain173;
  double Gain188;
  double Gain203;
} omega_est_100000059_context;
#endif

#ifndef InnerLoop_100000016_context_GUARD
#define InnerLoop_100000016_context_GUARD
typedef struct {
  int Gain81;
  double Gain99;
  omega_est_100000059_context omega_est_100000059_class_member2;
  LeadComp_100000028_context LeadComp_100000028_class_member3;
} InnerLoop_100000016_context;
#endif

#ifndef LeadComp_100000028_context_GUARD
#define LeadComp_100000028_context_GUARD
typedef struct {
  float X0135;
  float X0139;
  double Gain168;
  double Gain183;
  double Gain198;
} LeadComp_100000028_context;
#endif

#ifndef omega_est_100000059_context_GUARD
#define omega_est_100000059_context_GUARD
typedef struct {
  float X0143;
  float X0147;
  double Gain173;
  double Gain188;
  double Gain203;
} omega_est_100000059_context;
#endif

/* SIMPLIFIED PROGRAM CONTEXT */
typedef InnerLoop_100000016_context InnerLoop_context;

void InnerLoop_100000016_main( InnerLoop_100000016_context *context, float ang_ref_2, float angle_3, float *ang_err_4, float *Torque_5, float *ang_vel_6 );
void InnerLoop_100000016_init( InnerLoop_100000016_context *context );



/* SIMPLIFIED PROGRAM FUNCTIONS */
void InnerLoop_main( InnerLoop_100000016_context *context, float ang_ref_2, float angle_3, float *ang_err_4, float *Torque_5, float *ang_vel_6 );
void InnerLoop_init( InnerLoop_100000016_context *context );
#endif
