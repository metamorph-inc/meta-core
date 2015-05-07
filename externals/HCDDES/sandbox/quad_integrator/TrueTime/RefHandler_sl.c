#include "RefHandler_sl.h"



/* SIMPLIFIED PROGRAM FUNCTIONS */
void RefHandler_main( RefHandler_100000666_context *context, float pos_ref_in_2, float *pos_ref_out_3 ) {
  RefHandler_100000666_main( context, pos_ref_in_2, pos_ref_out_3 );
}

void RefHandler_init( RefHandler_100000666_context *context ) {
  RefHandler_100000666_init( context );
}

void RefHandler_100000666_main( RefHandler_100000666_context *context, float pos_ref_in_2, float *pos_ref_out_3 )
{
  *pos_ref_out_3 = ( *context ).Gain130 * pos_ref_in_2;
}

void RefHandler_100000666_init( RefHandler_100000666_context *context )
{
  ( *context ).Gain130 = 1;
}

