#include "OuterLoop_sl.h"

/* SIMPLIFIED PROGRAM FUNCTIONS */
void OuterLoop_main( OuterLoop_100000163_context *context, float pos_ref_2[4], float pos_data_3[12], float (*att_ref_4)[4] ) {
  OuterLoop_100000163_main( context, pos_ref_2, pos_data_3, att_ref_4 );
}

void OuterLoop_init( OuterLoop_100000163_context *context ) {
  OuterLoop_100000163_init( context );
}

void OuterLoop_100000163_main( OuterLoop_100000163_context *context, float pos_ref_2[4], float pos_data_3[12], float (*att_ref_4)[4] )
{
  float sig_0[16];
  float sig_1[4];
  Ssignal1CA4singleACsignal2CA12singleAS sig_2;
  int ix88;
  int ix101;
  int kx541;
  float sum542;
  int ix544;
  int ix972;

  ix88 = 0;
  while( ix88 < 4 )
  {
    sig_2.signal1[ (int)( ix88 ) ] = pos_ref_2[ (int)( ix88 ) ];
    ++ix88;
  }

  ix101 = 0;
  while( ix101 < 12 )
  {
    sig_2.signal2[ (int)( ix101 ) ] = pos_data_3[ (int)( ix101 ) ];
    ++ix101;
  }

  sig_0[ (int)( 0 ) ] = sig_2.signal1[ (int)( 0 ) ];
  sig_0[ (int)( 1 ) ] = sig_2.signal1[ (int)( 1 ) ];
  sig_0[ (int)( 2 ) ] = sig_2.signal1[ (int)( 2 ) ];
  sig_0[ (int)( 3 ) ] = sig_2.signal1[ (int)( 3 ) ];
  sig_0[ (int)( 4 ) ] = sig_2.signal2[ (int)( 0 ) ];
  sig_0[ (int)( 5 ) ] = sig_2.signal2[ (int)( 1 ) ];
  sig_0[ (int)( 6 ) ] = sig_2.signal2[ (int)( 2 ) ];
  sig_0[ (int)( 7 ) ] = sig_2.signal2[ (int)( 3 ) ];
  sig_0[ (int)( 8 ) ] = sig_2.signal2[ (int)( 4 ) ];
  sig_0[ (int)( 9 ) ] = sig_2.signal2[ (int)( 5 ) ];
  sig_0[ (int)( 10 ) ] = sig_2.signal2[ (int)( 6 ) ];
  sig_0[ (int)( 11 ) ] = sig_2.signal2[ (int)( 7 ) ];
  sig_0[ (int)( 12 ) ] = sig_2.signal2[ (int)( 8 ) ];
  sig_0[ (int)( 13 ) ] = sig_2.signal2[ (int)( 9 ) ];
  sig_0[ (int)( 14 ) ] = sig_2.signal2[ (int)( 10 ) ];
  sig_0[ (int)( 15 ) ] = sig_2.signal2[ (int)( 11 ) ];
  ix544 = 0;
  while( ix544 < 4 )
  {
    kx541 = 0;
    sum542 = 0;
    while( kx541 < 16 )
    {
      sum542 += ( *context ).Gain534[ (int)( ix544 ) ][ (int)( kx541 ) ] * sig_0[ (int)( kx541 ) ];
      ++kx541;
    }

    sig_1[ (int)( ix544 ) ] = sum542;
    ++ix544;
  }

  ix972 = 0;
  while( ix972 < 4 )
  {
    ( *att_ref_4 )[ (int)( ix972 ) ] = ( *context ).Gain967 * sig_1[ (int)( ix972 ) ];
    ++ix972;
  }

}

void OuterLoop_100000163_init( OuterLoop_100000163_context *context )
{
  ( *context ).Gain534[ (int)( 0 ) ][ (int)( 0 ) ] = 1;
  ( *context ).Gain534[ (int)( 0 ) ][ (int)( 1 ) ] = 1;
  ( *context ).Gain534[ (int)( 0 ) ][ (int)( 2 ) ] = 1;
  ( *context ).Gain534[ (int)( 0 ) ][ (int)( 3 ) ] = 1;
  ( *context ).Gain534[ (int)( 0 ) ][ (int)( 4 ) ] = 1;
  ( *context ).Gain534[ (int)( 0 ) ][ (int)( 5 ) ] = 1;
  ( *context ).Gain534[ (int)( 0 ) ][ (int)( 6 ) ] = 1;
  ( *context ).Gain534[ (int)( 0 ) ][ (int)( 7 ) ] = 1;
  ( *context ).Gain534[ (int)( 0 ) ][ (int)( 8 ) ] = 1;
  ( *context ).Gain534[ (int)( 0 ) ][ (int)( 9 ) ] = 1;
  ( *context ).Gain534[ (int)( 0 ) ][ (int)( 10 ) ] = 1;
  ( *context ).Gain534[ (int)( 0 ) ][ (int)( 11 ) ] = 1;
  ( *context ).Gain534[ (int)( 0 ) ][ (int)( 12 ) ] = 1;
  ( *context ).Gain534[ (int)( 0 ) ][ (int)( 13 ) ] = 1;
  ( *context ).Gain534[ (int)( 0 ) ][ (int)( 14 ) ] = 1;
  ( *context ).Gain534[ (int)( 0 ) ][ (int)( 15 ) ] = 1;
  ( *context ).Gain534[ (int)( 1 ) ][ (int)( 0 ) ] = 1;
  ( *context ).Gain534[ (int)( 1 ) ][ (int)( 1 ) ] = 1;
  ( *context ).Gain534[ (int)( 1 ) ][ (int)( 2 ) ] = 1;
  ( *context ).Gain534[ (int)( 1 ) ][ (int)( 3 ) ] = 1;
  ( *context ).Gain534[ (int)( 1 ) ][ (int)( 4 ) ] = 1;
  ( *context ).Gain534[ (int)( 1 ) ][ (int)( 5 ) ] = 1;
  ( *context ).Gain534[ (int)( 1 ) ][ (int)( 6 ) ] = 1;
  ( *context ).Gain534[ (int)( 1 ) ][ (int)( 7 ) ] = 1;
  ( *context ).Gain534[ (int)( 1 ) ][ (int)( 8 ) ] = 1;
  ( *context ).Gain534[ (int)( 1 ) ][ (int)( 9 ) ] = 1;
  ( *context ).Gain534[ (int)( 1 ) ][ (int)( 10 ) ] = 1;
  ( *context ).Gain534[ (int)( 1 ) ][ (int)( 11 ) ] = 1;
  ( *context ).Gain534[ (int)( 1 ) ][ (int)( 12 ) ] = 1;
  ( *context ).Gain534[ (int)( 1 ) ][ (int)( 13 ) ] = 1;
  ( *context ).Gain534[ (int)( 1 ) ][ (int)( 14 ) ] = 1;
  ( *context ).Gain534[ (int)( 1 ) ][ (int)( 15 ) ] = 1;
  ( *context ).Gain534[ (int)( 2 ) ][ (int)( 0 ) ] = 1;
  ( *context ).Gain534[ (int)( 2 ) ][ (int)( 1 ) ] = 1;
  ( *context ).Gain534[ (int)( 2 ) ][ (int)( 2 ) ] = 1;
  ( *context ).Gain534[ (int)( 2 ) ][ (int)( 3 ) ] = 1;
  ( *context ).Gain534[ (int)( 2 ) ][ (int)( 4 ) ] = 1;
  ( *context ).Gain534[ (int)( 2 ) ][ (int)( 5 ) ] = 1;
  ( *context ).Gain534[ (int)( 2 ) ][ (int)( 6 ) ] = 1;
  ( *context ).Gain534[ (int)( 2 ) ][ (int)( 7 ) ] = 1;
  ( *context ).Gain534[ (int)( 2 ) ][ (int)( 8 ) ] = 1;
  ( *context ).Gain534[ (int)( 2 ) ][ (int)( 9 ) ] = 1;
  ( *context ).Gain534[ (int)( 2 ) ][ (int)( 10 ) ] = 1;
  ( *context ).Gain534[ (int)( 2 ) ][ (int)( 11 ) ] = 1;
  ( *context ).Gain534[ (int)( 2 ) ][ (int)( 12 ) ] = 1;
  ( *context ).Gain534[ (int)( 2 ) ][ (int)( 13 ) ] = 1;
  ( *context ).Gain534[ (int)( 2 ) ][ (int)( 14 ) ] = 1;
  ( *context ).Gain534[ (int)( 2 ) ][ (int)( 15 ) ] = 1;
  ( *context ).Gain534[ (int)( 3 ) ][ (int)( 0 ) ] = 1;
  ( *context ).Gain534[ (int)( 3 ) ][ (int)( 1 ) ] = 1;
  ( *context ).Gain534[ (int)( 3 ) ][ (int)( 2 ) ] = 1;
  ( *context ).Gain534[ (int)( 3 ) ][ (int)( 3 ) ] = 1;
  ( *context ).Gain534[ (int)( 3 ) ][ (int)( 4 ) ] = 1;
  ( *context ).Gain534[ (int)( 3 ) ][ (int)( 5 ) ] = 1;
  ( *context ).Gain534[ (int)( 3 ) ][ (int)( 6 ) ] = 1;
  ( *context ).Gain534[ (int)( 3 ) ][ (int)( 7 ) ] = 1;
  ( *context ).Gain534[ (int)( 3 ) ][ (int)( 8 ) ] = 1;
  ( *context ).Gain534[ (int)( 3 ) ][ (int)( 9 ) ] = 1;
  ( *context ).Gain534[ (int)( 3 ) ][ (int)( 10 ) ] = 1;
  ( *context ).Gain534[ (int)( 3 ) ][ (int)( 11 ) ] = 1;
  ( *context ).Gain534[ (int)( 3 ) ][ (int)( 12 ) ] = 1;
  ( *context ).Gain534[ (int)( 3 ) ][ (int)( 13 ) ] = 1;
  ( *context ).Gain534[ (int)( 3 ) ][ (int)( 14 ) ] = 1;
  ( *context ).Gain534[ (int)( 3 ) ][ (int)( 15 ) ] = 1;
  ( *context ).Gain967 = 2.0 * 0.06250000;
}

