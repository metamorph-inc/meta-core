#include "ModeController_sl.h"



/* SIMPLIFIED PROGRAM FUNCTIONS */
void ModeController_main( ModeController_100000151_context *context, float Velocity_2, float BatteryVoltage_3, float VelocityDemand_4, float *Mode_5 ) {
  ModeController_100000151_main( context, Velocity_2, BatteryVoltage_3, VelocityDemand_4, Mode_5 );
}

void ModeController_init( ModeController_100000151_context *context ) {
  ModeController_100000151_init( context );
}

void ModeController_100000151_main( ModeController_100000151_context *context, float Velocity_2, float BatteryVoltage_3, float VelocityDemand_4, float *Mode_5 )
{
  float sig_0;
  float sig_1;
  float sig_2;
  float sig_3;
  float sig_4;
  float sig_5;
  float sig_6;
  float sig_7;

  sig_7 = ( *context ).Value771;
  sig_6 = ( *context ).Value776;
  if ( sig_4 < 200 * sig_1 )
  {
    sig_5 = 1;
  }
  else if ( sig_3 < sig_2 )
  {
    sig_5 = 0;
  }
  else if ( sig_4 < 200 * sig_0 )
  {
    sig_5 = 1;
  }
  else if ( 1 )
  {
    sig_5 = 2;
  }

}

void ModeController_100000151_init( ModeController_100000151_context *context )
{
  ( *context ).Value771 = 0.939999999999999950000;
  ( *context ).Value776 = 0.0500000000000000030000;
}

