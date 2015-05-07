#include "DataHandler_sl.h"



/* SIMPLIFIED PROGRAM FUNCTIONS */
void DataHandler_main( void *context, float ang_in_2, float pos_in_3, float *ang_out_4, float *pos_out_5 ) {
  DataHandler_100000617_main( context, ang_in_2, pos_in_3, ang_out_4, pos_out_5 );
}

void DataHandler_init( void *context ) {
  DataHandler_100000617_init( context );
}

void DataHandler_100000617_main( void *context, float ang_in_2, float pos_in_3, float *ang_out_4, float *pos_out_5 )
{
  Ssignal1CsingleCsignal2CsingleS sig_0;

  sig_0.signal1 = ang_in_2;
  sig_0.signal2 = pos_in_3;
  *ang_out_4 = sig_0.signal1;
  *pos_out_5 = sig_0.signal2;
}

void DataHandler_100000617_init( void *context )
{
}

