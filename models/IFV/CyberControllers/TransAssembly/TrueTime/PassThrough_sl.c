#include "PassThrough_sl.h"



/* SIMPLIFIED PROGRAM FUNCTIONS */
void PassThrough_main( PassThrough_100000204_context *context, float In_2, float *Out_3 ) {
  PassThrough_100000204_main( context, In_2, Out_3 );
}

void PassThrough_init( PassThrough_100000204_context *context ) {
  PassThrough_100000204_init( context );
}

void PassThrough_100000204_main( PassThrough_100000204_context *context, float In_2, float *Out_3 )
{
  *Out_3 = In_2 * ( *context ).Gain869;
}

void PassThrough_100000204_init( PassThrough_100000204_context *context )
{
  ( *context ).Gain869 = 1;
}

