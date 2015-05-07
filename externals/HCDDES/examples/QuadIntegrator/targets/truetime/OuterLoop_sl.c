#include "OuterLoop_sl.h"



/* SIMPLIFIED PROGRAM FUNCTIONS */
void OuterLoop_main( OuterLoop_100000095_context *context, float pos_ref_2, float pos_3, float *ang_ref_5 ) {
  OuterLoop_100000095_main( context, pos_ref_2, pos_3, ang_ref_5 );
}

void OuterLoop_init( OuterLoop_100000095_context *context ) {
  OuterLoop_100000095_init( context );
}

void vel_est_100000117_main( vel_est_100000117_context *context, float x_2, float *y_3 );
void vel_est_100000117_init( vel_est_100000117_context *context );


void OuterLoop_100000095_main( OuterLoop_100000095_context *context, float pos_ref_2, float pos_3, float *ang_ref_5 )
{
  float sig_0;
  float sig_1;
  float sig_2;
  float sig_3;
  float sig_4;

  vel_est_100000117_main( &( *context ).vel_est_100000117_class_member4, pos_3, &sig_4 );
  sig_3 = pos_ref_2 - pos_3;
  sig_1 = sig_3 * ( *context ).Gain86;
  sig_2 = sig_1 - sig_4;
  sig_0 = sig_2 * ( *context ).Gain104;
  if ( sig_0 > ( *context ).UpperLimit116 )
  {
    *ang_ref_5 = ( *context ).UpperLimit116;
  }
  else if ( sig_0 < ( *context ).LowerLimit115 )
  {
    *ang_ref_5 = ( *context ).LowerLimit115;
  }
  else if ( 1 )
  {
    *ang_ref_5 = sig_0;
  }

}

void OuterLoop_100000095_init( OuterLoop_100000095_context *context )
{
  ( *context ).Gain86 = 0.299999999999999990000;
  ( *context ).Gain104 = 2;
  ( *context ).LowerLimit115 = -0.785398163397448280000;
  ( *context ).UpperLimit116 = 0.785398163397448280000;
  vel_est_100000117_init( &( *context ).vel_est_100000117_class_member4 );
}

void vel_est_100000117_main( vel_est_100000117_context *context, float x_2, float *y_3 )
{
  float sig_0;
  float sig_1;
  float sig_2;
  float sig_3;
  float sig_4;

  sig_3 = ( *context ).X0151;
  sig_4 = ( *context ).X0155;
  ( *context ).X0151 = x_2;
  sig_2 = sig_4 * ( *context ).Gain178;
  sig_1 = sig_3 * ( *context ).Gain193;
  sig_0 = x_2 * ( *context ).Gain208;
  *y_3 = sig_1 + sig_0 - sig_2;
  ( *context ).X0155 = *y_3;
}

void vel_est_100000117_init( vel_est_100000117_context *context )
{
  ( *context ).X0151 = 0;
  ( *context ).X0155 = 0;
  ( *context ).Gain178 = -0.0432000000000000020000;
  ( *context ).Gain193 = -47.8393000000000010000;
  ( *context ).Gain208 = 47.8393000000000010000;
}

