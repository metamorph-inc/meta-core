#include "PassThrough2_sl.h"



/* SIMPLIFIED PROGRAM FUNCTIONS */
void PassThrough2_main( PassThrough2_100000186_context *context, float In1_2, float In2_3, float *Out1_4, float *Out2_5 ) {
  PassThrough2_100000186_main( context, In1_2, In2_3, Out1_4, Out2_5 );
}

void PassThrough2_init( PassThrough2_100000186_context *context ) {
  PassThrough2_100000186_init( context );
}

void PassThrough2_100000186_main( PassThrough2_100000186_context *context, float In1_2, float In2_3, float *Out1_4, float *Out2_5 )
{
  *Out1_4 = In1_2 * ( *context ).Gain863;
  *Out2_5 = In2_3 * ( *context ).Gain874;
}

void PassThrough2_100000186_init( PassThrough2_100000186_context *context )
{
  ( *context ).Gain863 = 1;
  ( *context ).Gain874 = 1;
}

