#include "OuterLoop_sl.h"


void OuterLoop_100000011_main( OuterLoop_100000011_context *context, float pos_2, float (*ang_ref_3) )
{
  ( *ang_ref_3 ) = ( *context ).Gain27 * pos_2;
}

void OuterLoop_100000011_init( OuterLoop_100000011_context *context )
{
  ( *context ).Gain27 = 0.50000;
}

