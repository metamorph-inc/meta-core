#include "InnerLoop_sl.h"


void InnerLoop_100000008_main( InnerLoop_100000008_context *context, float angle_2, float (*Torque_3) )
{
  ( *Torque_3 ) = ( *context ).Gain19 * angle_2;
}

void InnerLoop_100000008_init( InnerLoop_100000008_context *context )
{
  ( *context ).Gain19 = 2;
}

