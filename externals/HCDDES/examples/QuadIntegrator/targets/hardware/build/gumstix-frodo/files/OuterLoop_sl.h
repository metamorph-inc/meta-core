#ifndef _OuterLoop_sl_H_
#define _OuterLoop_sl_H_

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

#ifndef vel_est_100000110_context_GUARD
#define vel_est_100000110_context_GUARD
typedef struct {
  float X0151;
  float X0155;
  double Gain178;
  double Gain193;
  double Gain208;
} vel_est_100000110_context;
#endif

#ifndef OuterLoop_100000088_context_GUARD
#define OuterLoop_100000088_context_GUARD
typedef struct {
  double Gain87;
  int Gain104;
  double LowerLimit115;
  double UpperLimit116;
  vel_est_100000110_context vel_est_100000110_class_member4;
} OuterLoop_100000088_context;
#endif

#ifndef vel_est_100000110_context_GUARD
#define vel_est_100000110_context_GUARD
typedef struct {
  float X0151;
  float X0155;
  double Gain178;
  double Gain193;
  double Gain208;
} vel_est_100000110_context;
#endif

/* SIMPLIFIED PROGRAM CONTEXT */
typedef OuterLoop_100000088_context OuterLoop_context;

void OuterLoop_100000088_main( OuterLoop_100000088_context *context, float pos_ref_2, float pos_3, float *vel_err_4, float *ang_ref_5 );
void OuterLoop_100000088_init( OuterLoop_100000088_context *context );



/* SIMPLIFIED PROGRAM FUNCTIONS */
void OuterLoop_main( OuterLoop_100000088_context *context, float pos_ref_2, float pos_3, float *vel_err_4, float *ang_ref_5 );
void OuterLoop_init( OuterLoop_100000088_context *context );
#endif
