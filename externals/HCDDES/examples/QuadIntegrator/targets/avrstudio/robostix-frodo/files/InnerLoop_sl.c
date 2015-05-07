#include "InnerLoop_sl.h"



/* SIMPLIFIED PROGRAM FUNCTIONS */
void InnerLoop_main( InnerLoop_100000016_context *context, float ang_ref_2, float angle_3, float *ang_err_4, float *Torque_5, float *ang_vel_6 ) {
  InnerLoop_100000016_main( context, ang_ref_2, angle_3, ang_err_4, Torque_5, ang_vel_6 );
}

void InnerLoop_init( InnerLoop_100000016_context *context ) {
  InnerLoop_100000016_init( context );
}

void LeadComp_100000028_main( LeadComp_100000028_context *context, float x_2, float *y_3 );
void LeadComp_100000028_init( LeadComp_100000028_context *context );
void omega_est_100000059_main( omega_est_100000059_context *context, float x_2, float *y_3 );
void omega_est_100000059_init( omega_est_100000059_context *context );


void InnerLoop_100000016_main( InnerLoop_100000016_context *context, float ang_ref_2, float angle_3, float *ang_err_4, float *Torque_5, float *ang_vel_6 )
{
  float sig_0;
  float sig_1;
  float sig_2;

  omega_est_100000059_main( &( *context ).omega_est_100000059_class_member2, angle_3, &*ang_vel_6 );
  sig_2 = ang_ref_2 - angle_3;
  sig_0 = ( *context ).Gain81 * sig_2;
  *ang_err_4 = sig_0 - *ang_vel_6;
  sig_1 = ( *context ).Gain99 * *ang_err_4;
  LeadComp_100000028_main( &( *context ).LeadComp_100000028_class_member3, sig_1, &*Torque_5 );
}

void InnerLoop_100000016_init( InnerLoop_100000016_context *context )
{
  ( *context ).Gain81 = 2;
  ( *context ).Gain99 = 0.50000;
  LeadComp_100000028_init( &( *context ).LeadComp_100000028_class_member3 );
  omega_est_100000059_init( &( *context ).omega_est_100000059_class_member2 );
}

void LeadComp_100000028_main( LeadComp_100000028_context *context, float x_2, float *y_3 )
{
  float sig_0;
  float sig_1;
  float sig_2;
  float sig_3;
  float sig_4;

  sig_3 = ( *context ).X0135;
  sig_4 = ( *context ).X0139;
  ( *context ).X0135 = x_2;
  sig_2 = ( *context ).Gain168 * sig_4;
  sig_1 = ( *context ).Gain183 * sig_3;
  sig_0 = ( *context ).Gain198 * x_2;
  *y_3 = sig_1 + sig_0 - sig_2;
  ( *context ).X0139 = *y_3;
}

void LeadComp_100000028_init( LeadComp_100000028_context *context )
{
  ( *context ).X0135 = 0;
  ( *context ).X0139 = 0;
  ( *context ).Gain168 = -0.0432000000000000020000;
  ( *context ).Gain183 = -4.52259999999999970000;
  ( *context ).Gain198 = 5.47940000;
}

void omega_est_100000059_main( omega_est_100000059_context *context, float x_2, float *y_3 )
{
  float sig_0;
  float sig_1;
  float sig_2;
  float sig_3;
  float sig_4;

  sig_3 = ( *context ).X0143;
  sig_4 = ( *context ).X0147;
  ( *context ).X0143 = x_2;
  sig_2 = ( *context ).Gain173 * sig_4;
  sig_1 = ( *context ).Gain188 * sig_3;
  sig_0 = ( *context ).Gain203 * x_2;
  *y_3 = sig_1 + sig_0 - sig_2;
  ( *context ).X0147 = *y_3;
}

void omega_est_100000059_init( omega_est_100000059_context *context )
{
  ( *context ).X0143 = 0;
  ( *context ).X0147 = 0;
  ( *context ).Gain173 = -0.0432000000000000020000;
  ( *context ).Gain188 = -47.8393000000000010000;
  ( *context ).Gain203 = 47.8393000000000010000;
}

