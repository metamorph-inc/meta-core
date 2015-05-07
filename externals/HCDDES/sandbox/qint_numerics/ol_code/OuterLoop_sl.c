#include "OuterLoop_sl.h"



/* SIMPLIFIED PROGRAM FUNCTIONS */
void OuterLoop_main( OuterLoop_100000642_context *context, float pos_ref_2, float pos_3, float (*ang_ref_4)[5] ) {
  OuterLoop_100000642_main( context, pos_ref_2, pos_3, ang_ref_4 );
}

void OuterLoop_init( OuterLoop_100000642_context *context ) {
  OuterLoop_100000642_init( context );
}

void vel_est_100000787_main( vel_est_100000787_context *context, float x_2, float *y_3 );
void vel_est_100000787_init( vel_est_100000787_context *context );

void OuterLoop_100000642_main( OuterLoop_100000642_context *context, float pos_ref_2, float pos_3, float (*ang_ref_4)[5] )
{
  float sig_0;
  float sig_1;
  Ssignal1CsingleCsignal2CsingleCsignal3CsingleCsignal4CsingleCsignal5CsingleS sig_2;
  float sig_3;
  float sig_4;
  float sig_5;
  float sig_6;

  vel_est_100000787_main( &( *context ).vel_est_100000787_class_member4, pos_3, &sig_6 );
  sig_5 = pos_ref_2 - pos_3;
  sig_1 = ( *context ).Gain158 * sig_5;
  sig_4 = sig_1 - sig_6;
  sig_0 = ( *context ).Gain174 * sig_4;
  if ( sig_0 < ( *context ).LowerLimit185 )
  {
    sig_3 = ( *context ).LowerLimit185;
  }
  else if ( sig_0 > ( *context ).UpperLimit186 )
  {
    sig_3 = ( *context ).UpperLimit186;
  }
  else if ( 1 )
  {
    sig_3 = sig_0;
  }

  sig_2.signal1 = sig_3;
  sig_2.signal2 = sig_1;
  sig_2.signal3 = pos_3;
  sig_2.signal4 = sig_0;
  sig_2.signal5 = sig_6;
  ( *ang_ref_4 )[ (int)( 0 ) ] = sig_2.signal1;
  ( *ang_ref_4 )[ (int)( 1 ) ] = sig_2.signal2;
  ( *ang_ref_4 )[ (int)( 2 ) ] = sig_2.signal3;
  ( *ang_ref_4 )[ (int)( 3 ) ] = sig_2.signal4;
//#warning "DEBUG CODE in OuterLoop_100000642_main: "##__LINE__
  ( *ang_ref_4 )[ (int)( 4 ) ] = sig_2.signal5;  
  //( *ang_ref_4 )[ (int)( 4 ) ] = sig_0; DEBUG!!
}

void OuterLoop_100000642_init( OuterLoop_100000642_context *context )
{
  ( *context ).Gain158 = 0.299999999999999990000 * 1.0;
  ( *context ).Gain174 = 2 * 1.0;
  ( *context ).LowerLimit185 = -0.785398163397448280000;
  ( *context ).UpperLimit186 = 0.785398163397448280000;
  vel_est_100000787_init( &( *context ).vel_est_100000787_class_member4 );
}

void vel_est_100000787_main( vel_est_100000787_context *context, float x_2, float *y_3 )
{
  float sig_0;
  float sig_1;
  float sig_2;
  float sig_3;
  float sig_4;

  sig_3 = ( *context ).X0325;
  sig_4 = ( *context ).X0329;
  ( *context ).X0325 = x_2;
  sig_2 = ( *context ).Gain352 * sig_4;
  sig_1 = ( *context ).Gain367 * sig_3;
  sig_0 = ( *context ).Gain382 * x_2;
  *y_3 = sig_1 + sig_0 - sig_2;
  ( *context ).X0329 = *y_3;
}

void vel_est_100000787_init( vel_est_100000787_context *context )
{
  ( *context ).X0325 = 0;
  ( *context ).X0329 = 0;
  ( *context ).Gain352 = -0.0432000000000000020000;
  ( *context ).Gain367 = -47.8393000000000010000;
  ( *context ).Gain382 = 47.8393000000000010000;
}

