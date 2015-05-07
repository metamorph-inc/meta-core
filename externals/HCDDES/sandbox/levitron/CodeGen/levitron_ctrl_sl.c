#include "levitron_ctrl_sl.h"


void PD_Controller_100000004_main( PD_Controller_100000004_context *context, float RefPosition_2, float Position_3, float *Force_4 );
void PD_Controller_100000004_init( PD_Controller_100000004_context *context );
void Waypoint_list___Interpolation_100000008_main( void *context, float *NextPosition_2 );


void levitron_ctrl_100000001_main( levitron_ctrl_100000001_context *context, float Position_2, float *Force_3 )
{
  float sig_0;

  Waypoint_list___Interpolation_100000008_main( (void *)0, &sig_0 );
  PD_Controller_100000004_main( &( *context ).PD_Controller_100000004_class_member1, sig_0, Position_2, &*Force_3 );
}

void levitron_ctrl_100000001_init( levitron_ctrl_100000001_context *context )
{
  PD_Controller_100000004_init( &( *context ).PD_Controller_100000004_class_member1 );
}

void PD_Controller_100000004_main( PD_Controller_100000004_context *context, float RefPosition_2, float Position_3, float *Force_4 )
{
  float sig_0;
  float sig_1;
  float sig_2;
  float sig_3;
  float sig_4;
  float sig_5;

  sig_5 = ( *context ).X043;
  ( *context ).X043 = Position_3;
  sig_2 = ( *context ).Gain52 * Position_3;
  sig_1 = ( *context ).Gain57 * sig_5;
  sig_4 = sig_2 + sig_1;
  sig_3 = RefPosition_2 - sig_4;
  sig_0 = ( *context ).Gain68 * sig_3;
  if ( sig_0 < ( *context ).LowerLimit73 )
  {
    *Force_4 = ( *context ).LowerLimit73;
  }
  else if ( sig_0 > ( *context ).UpperLimit74 )
  {
    *Force_4 = ( *context ).UpperLimit74;
  }
  else if ( 1 )
  {
    *Force_4 = sig_0;
  }

}

void PD_Controller_100000004_init( PD_Controller_100000004_context *context )
{
  ( *context ).Gain68 = 10;
  ( *context ).Gain57 = -5;
  ( *context ).Gain52 = 6;
  ( *context ).LowerLimit73 = 0;
  ( *context ).UpperLimit74 = 100;
  ( *context ).X043 = 10;
}

void Waypoint_list___Interpolation_100000008_main( void *context, float *NextPosition_2 )
{
  *NextPosition_2 = 0;
}

