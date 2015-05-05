#include "AutoTransController_sl.h"



/* SIMPLIFIED PROGRAM FUNCTIONS */
void AutoTransController_main( AutoTransController_100000117_context *context, float w_2, float *gear_ratio_3, float *gear_num_4 ) {
  AutoTransController_100000117_main( context, w_2, gear_ratio_3, gear_num_4 );
}

void AutoTransController_init( AutoTransController_100000117_context *context ) {
  AutoTransController_100000117_init( context );
}

void AutoTransController_100000117_main( AutoTransController_100000117_context *context, float w_2, float *gear_ratio_3, float *gear_num_4 )
{
  float sig_0;
  float sig_1;
  float sig_2;
  float sig_3;
  float sig_4;
  float sig_5;
  float sig_6;
  float sig_7;

  sig_0 = ( *context ).X0592;
  sig_2 = ( *context ).Value597;
  sig_1 = ( *context ).Value601;
  sig_4 = sig_0;
  sig_3 = 0.120000;
  if ( sig_7 > sig_5 * 2 * 3.14159265358979310000 / 60 && sig_0 < 4 )
  {
    sig_4 = sig_0 + 1;
  }
  else if ( sig_7 < sig_6 * 2 * 3.14159265358979310000 / 60 && sig_0 > 1 )
  {
    sig_4 = sig_0 - 1;
  }

  if ( sig_0 < 1 )
  {
    sig_4 = 1;
  }

  if ( sig_0 > 4 )
  {
    sig_4 = 4;
  }

  if ( sig_4 == 1 )
  {
    sig_3 = 0.034285714285714280000;
  }
  else if ( sig_4 == 2 )
  {
    sig_3 = 0.0545454545454545430000;
  }
  else if ( sig_4 == 3 )
  {
    sig_3 = 0.0857142857142857150000;
  }
  else if ( sig_4 == 4 )
  {
    sig_3 = 0.120000;
  }

  ( *context ).X0592 = sig_4;
}

void AutoTransController_100000117_init( AutoTransController_100000117_context *context )
{
  ( *context ).X0592 = 0;
  ( *context ).Value597 = 2400;
  ( *context ).Value601 = 1400;
}

