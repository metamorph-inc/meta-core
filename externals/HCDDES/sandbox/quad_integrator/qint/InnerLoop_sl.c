#include "InnerLoop_sl.h"



/* SIMPLIFIED PROGRAM FUNCTIONS */
void InnerLoop_main( InnerLoop_100000634_context *context, float ang_ref_2[5], float angle_3, float (*Torque_4)[9] ) {
  InnerLoop_100000634_main( context, ang_ref_2, angle_3, Torque_4 );
}

void InnerLoop_init( InnerLoop_100000634_context *context ) {
  InnerLoop_100000634_init( context );
}

void LeadComp_100000695_main( LeadComp_100000695_context *context, float x_2, float *y_3 );
void LeadComp_100000695_init( LeadComp_100000695_context *context );
void omega_est_100000717_main( omega_est_100000717_context *context, float x_2, float *y_3 );
void omega_est_100000717_init( omega_est_100000717_context *context );


void InnerLoop_100000634_main( InnerLoop_100000634_context *context, float ang_ref_2[5], float angle_3, float (*Torque_4)[9] )
{
  float sig_0;
  float sig_1;
  float sig_2;
  float sig_3;
  float sig_4;
  float sig_5;
  float sig_6;
  float sig_7;
  Ssignal1CsingleCsignal2CsingleCsignal3CsingleCsignal4CsingleCsignal5CsingleCsignal6CsingleCsignal7CsingleCsignal8CsingleCsignal9CsingleS sig_8;
  float sig_9;
  float sig_10;
  float sig_11;

  omega_est_100000717_main( &( *context ).omega_est_100000717_class_member2, angle_3, &sig_11 );
  sig_0 = ang_ref_2[ (int)( 0 ) ];
  sig_1 = ang_ref_2[ (int)( 1 ) ];
  sig_2 = ang_ref_2[ (int)( 2 ) ];
  sig_3 = ang_ref_2[ (int)( 3 ) ];
  sig_4 = ang_ref_2[ (int)( 4 ) ];
  sig_10 = sig_0 - angle_3;
  sig_5 = ( *context ).Gain163 * sig_10;
  sig_9 = sig_5 - sig_11;
  sig_6 = ( *context ).Gain180 * sig_9;
  LeadComp_100000695_main( &( *context ).LeadComp_100000695_class_member3, sig_6, &sig_7 );
  sig_8.signal1 = sig_7;
  sig_8.signal2 = sig_1;
  sig_8.signal3 = sig_2;
  sig_8.signal4 = sig_3;
  sig_8.signal5 = sig_4;
  sig_8.signal6 = sig_5;
  sig_8.signal7 = angle_3;
  sig_8.signal8 = sig_6;
  sig_8.signal9 = sig_11;
  ( *Torque_4 )[ (int)( 0 ) ] = sig_8.signal1;
  ( *Torque_4 )[ (int)( 1 ) ] = sig_8.signal2;
  ( *Torque_4 )[ (int)( 2 ) ] = sig_8.signal3;
  ( *Torque_4 )[ (int)( 3 ) ] = sig_8.signal4;
  ( *Torque_4 )[ (int)( 4 ) ] = sig_8.signal5;
  ( *Torque_4 )[ (int)( 5 ) ] = sig_8.signal6;
  ( *Torque_4 )[ (int)( 6 ) ] = sig_8.signal7;
  ( *Torque_4 )[ (int)( 7 ) ] = sig_8.signal8;
  ( *Torque_4 )[ (int)( 8 ) ] = sig_8.signal9;
}

void InnerLoop_100000634_init( InnerLoop_100000634_context *context )
{
  ( *context ).Gain163 = 2;
  ( *context ).Gain180 = 0.50000;
  LeadComp_100000695_init( &( *context ).LeadComp_100000695_class_member3 );
  omega_est_100000717_init( &( *context ).omega_est_100000717_class_member2 );
}

void LeadComp_100000695_main( LeadComp_100000695_context *context, float x_2, float *y_3 )
{
  float sig_0;
  float sig_1;
  float sig_2;
  float sig_3;
  float sig_4;

  sig_3 = ( *context ).X0309;
  sig_4 = ( *context ).X0313;
  ( *context ).X0309 = x_2;
  sig_2 = ( *context ).Gain342 * sig_4;
  sig_1 = ( *context ).Gain357 * sig_3;
  sig_0 = ( *context ).Gain372 * x_2;
  *y_3 = sig_1 + sig_0 - sig_2;
  ( *context ).X0313 = *y_3;
}

void LeadComp_100000695_init( LeadComp_100000695_context *context )
{
  ( *context ).X0309 = 0;
  ( *context ).X0313 = 0;
  ( *context ).Gain342 = -0.0432000000000000020000;
  ( *context ).Gain357 = -4.52259999999999970000;
  ( *context ).Gain372 = 5.47940000;
}

void omega_est_100000717_main( omega_est_100000717_context *context, float x_2, float *y_3 )
{
  float sig_0;
  float sig_1;
  float sig_2;
  float sig_3;
  float sig_4;

  sig_3 = ( *context ).X0317;
  sig_4 = ( *context ).X0321;
  ( *context ).X0317 = x_2;
  sig_2 = ( *context ).Gain347 * sig_4;
  sig_1 = ( *context ).Gain362 * sig_3;
  sig_0 = ( *context ).Gain377 * x_2;
  *y_3 = sig_1 + sig_0 - sig_2;
  ( *context ).X0321 = *y_3;
}

void omega_est_100000717_init( omega_est_100000717_context *context )
{
  ( *context ).X0317 = 0;
  ( *context ).X0321 = 0;
  ( *context ).Gain347 = -0.0432000000000000020000;
  ( *context ).Gain362 = -47.8393000000000010000;
  ( *context ).Gain377 = 47.8393000000000010000;
}

