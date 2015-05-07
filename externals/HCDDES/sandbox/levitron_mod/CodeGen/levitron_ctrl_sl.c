#include "levitron_ctrl_sl.h"


void Waypoint_list___Interpolation_100000004_main( void *context, float *NextPosition_2 );
void PD_Controller_100000006_main( PD_Controller_100000006_context *context, float RefPosition_2, float Position_3, float *Force_4 );
void PD_Controller_100000006_init( PD_Controller_100000006_context *context );
void vel_est_100000029_main( vel_est_100000029_context *context, float X_2, float *V_est_3 );
void vel_est_100000029_init( vel_est_100000029_context *context );


void levitron_ctrl_100000001_main( levitron_ctrl_100000001_context *context, float Position_2, float *Force_3 )
{
  float sig_0;

  Waypoint_list___Interpolation_100000004_main( (void *)0, &sig_0 );
  PD_Controller_100000006_main( &( *context ).PD_Controller_100000006_class_member1, sig_0, Position_2, &*Force_3 );
}

void levitron_ctrl_100000001_init( levitron_ctrl_100000001_context *context )
{
  PD_Controller_100000006_init( &( *context ).PD_Controller_100000006_class_member1 );
}

void Waypoint_list___Interpolation_100000004_main( void *context, float *NextPosition_2 )
{
  *NextPosition_2 = 0;
}

void PD_Controller_100000006_main( PD_Controller_100000006_context *context, float RefPosition_2, float Position_3, float *Force_4 )
{
  float sig_0;
  float sig_1;
  float sig_2;
  float sig_3;
  float sig_4;

  vel_est_100000029_main( &( *context ).vel_est_100000029_class_member4, Position_3, &sig_4 );
  sig_2 = RefPosition_2 - Position_3;
  sig_0 = ( *context ).Gain64 * sig_2;
  sig_3 = sig_0 - sig_4;
  sig_1 = ( *context ).Gain72 * sig_3;
  if ( sig_1 < ( *context ).LowerLimit77 )
  {
    *Force_4 = ( *context ).LowerLimit77;
  }
  else if ( sig_1 > ( *context ).UpperLimit78 )
  {
    *Force_4 = ( *context ).UpperLimit78;
  }
  else if ( 1 )
  {
    *Force_4 = sig_1;
  }

}

void PD_Controller_100000006_init( PD_Controller_100000006_context *context )
{
  ( *context ).Gain64 = 1.57075000000000010000;
  ( *context ).Gain72 = 1.57075000000000010000;
  ( *context ).LowerLimit77 = 0;
  ( *context ).UpperLimit78 = 100;
  vel_est_100000029_init( &( *context ).vel_est_100000029_class_member4 );
}

void vel_est_100000029_main( vel_est_100000029_context *context, float X_2, float *V_est_3 )
{
  float sig_0;
  float sig_1;
  float sig_2;
  float sig_3;
  float sig_4;

  sig_0 = ( *context ).X097;
  sig_1 = ( *context ).X0101;
  sig_4 = ( *context ).Gain105 * X_2;
  sig_2 = ( *context ).Gain110 * sig_1;
  sig_3 = ( *context ).Gain115 * sig_0;
  ( *context ).X097 = X_2;
  *V_est_3 = sig_3 + sig_4 - sig_2;
  ( *context ).X0101 = *V_est_3;
}

void vel_est_100000029_init( vel_est_100000029_context *context )
{
  ( *context ).X097 = 0;
  ( *context ).X0101 = 0;
  ( *context ).Gain110 = 0.0432140000000000020000;
  ( *context ).Gain115 = -47.8393060000000010000;
  ( *context ).Gain105 = 47.8393060000000010000;
}

