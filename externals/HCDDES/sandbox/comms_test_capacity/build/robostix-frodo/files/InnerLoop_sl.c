#include "InnerLoop_sl.h"

/* SIMPLIFIED PROGRAM FUNCTIONS */
void InnerLoop_main( InnerLoop_100000159_context *context, float att_ref_2[4], float att_data_3[6], float (*Torque_4)[4] ) {
  InnerLoop_100000159_main( context, att_ref_2, att_data_3, Torque_4 );
}

void InnerLoop_init( InnerLoop_100000159_context *context ) {
  InnerLoop_100000159_init( context );
}

void InnerLoop_100000159_main( InnerLoop_100000159_context *context, float att_ref_2[4], float att_data_3[6], float (*Torque_4)[4] )
{
  float sig_0[10];
  float sig_1[4];
  Ssignal1CA4singleACsignal2CA6singleAS sig_2;
  int ix61;
  int ix75;
  int kx386;
  float sum387;
  int ix389;
  int ix956;

  ix61 = 0;
  while( ix61 < 4 )
  {
    sig_2.signal1[ (int)( ix61 ) ] = att_ref_2[ (int)( ix61 ) ];
    ++ix61;
  }

  ix75 = 0;
  while( ix75 < 6 )
  {
    sig_2.signal2[ (int)( ix75 ) ] = att_data_3[ (int)( ix75 ) ];
    ++ix75;
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
  ix389 = 0;
  while( ix389 < 4 )
  {
    kx386 = 0;
    sum387 = 0;
    while( kx386 < 10 )
    {
      sum387 += ( *context ).Gain379[ (int)( ix389 ) ][ (int)( kx386 ) ] * sig_0[ (int)( kx386 ) ];
      ++kx386;
    }

    sig_1[ (int)( ix389 ) ] = sum387;
    ++ix389;
  }

  ix956 = 0;
  while( ix956 < 4 )
  {
    ( *Torque_4 )[ (int)( ix956 ) ] = ( *context ).Gain950 * sig_1[ (int)( ix956 ) ];
    //( *Torque_4 )[ (int)( ix956 ) ] = 0.10 * sig_1[ (int)( ix956 ) ];
    //( *Torque_4 )[ (int)( ix956 ) ] = ( *context ).Gain950 * sig_0[ (int)( ix956 )+2 ];
    //( *Torque_4 )[ (int)( ix956 ) ] = sig_2.signal2[ (int)( ix956 ) ];
    //( *Torque_4 )[ (int)( ix956 ) ] = att_ref_2[ (int)( ix956 ) ];
    ++ix956;
  }

}

void InnerLoop_100000159_init( InnerLoop_100000159_context *context )
{
  ( *context ).Gain379[ (int)( 0 ) ][ (int)( 0 ) ] = 1;
  ( *context ).Gain379[ (int)( 0 ) ][ (int)( 1 ) ] = 1;
  ( *context ).Gain379[ (int)( 0 ) ][ (int)( 2 ) ] = 1;
  ( *context ).Gain379[ (int)( 0 ) ][ (int)( 3 ) ] = 1;
  ( *context ).Gain379[ (int)( 0 ) ][ (int)( 4 ) ] = 1;
  ( *context ).Gain379[ (int)( 0 ) ][ (int)( 5 ) ] = 1;
  ( *context ).Gain379[ (int)( 0 ) ][ (int)( 6 ) ] = 1;
  ( *context ).Gain379[ (int)( 0 ) ][ (int)( 7 ) ] = 1;
  ( *context ).Gain379[ (int)( 0 ) ][ (int)( 8 ) ] = 1;
  ( *context ).Gain379[ (int)( 0 ) ][ (int)( 9 ) ] = 1;
  ( *context ).Gain379[ (int)( 1 ) ][ (int)( 0 ) ] = 1;
  ( *context ).Gain379[ (int)( 1 ) ][ (int)( 1 ) ] = 1;
  ( *context ).Gain379[ (int)( 1 ) ][ (int)( 2 ) ] = 1;
  ( *context ).Gain379[ (int)( 1 ) ][ (int)( 3 ) ] = 1;
  ( *context ).Gain379[ (int)( 1 ) ][ (int)( 4 ) ] = 1;
  ( *context ).Gain379[ (int)( 1 ) ][ (int)( 5 ) ] = 1;
  ( *context ).Gain379[ (int)( 1 ) ][ (int)( 6 ) ] = 1;
  ( *context ).Gain379[ (int)( 1 ) ][ (int)( 7 ) ] = 1;
  ( *context ).Gain379[ (int)( 1 ) ][ (int)( 8 ) ] = 1;
  ( *context ).Gain379[ (int)( 1 ) ][ (int)( 9 ) ] = 1;
  ( *context ).Gain379[ (int)( 2 ) ][ (int)( 0 ) ] = 1;
  ( *context ).Gain379[ (int)( 2 ) ][ (int)( 1 ) ] = 1;
  ( *context ).Gain379[ (int)( 2 ) ][ (int)( 2 ) ] = 1;
  ( *context ).Gain379[ (int)( 2 ) ][ (int)( 3 ) ] = 1;
  ( *context ).Gain379[ (int)( 2 ) ][ (int)( 4 ) ] = 1;
  ( *context ).Gain379[ (int)( 2 ) ][ (int)( 5 ) ] = 1;
  ( *context ).Gain379[ (int)( 2 ) ][ (int)( 6 ) ] = 1;
  ( *context ).Gain379[ (int)( 2 ) ][ (int)( 7 ) ] = 1;
  ( *context ).Gain379[ (int)( 2 ) ][ (int)( 8 ) ] = 1;
  ( *context ).Gain379[ (int)( 2 ) ][ (int)( 9 ) ] = 1;
  ( *context ).Gain379[ (int)( 3 ) ][ (int)( 0 ) ] = 1;
  ( *context ).Gain379[ (int)( 3 ) ][ (int)( 1 ) ] = 1;
  ( *context ).Gain379[ (int)( 3 ) ][ (int)( 2 ) ] = 1;
  ( *context ).Gain379[ (int)( 3 ) ][ (int)( 3 ) ] = 1;
  ( *context ).Gain379[ (int)( 3 ) ][ (int)( 4 ) ] = 1;
  ( *context ).Gain379[ (int)( 3 ) ][ (int)( 5 ) ] = 1;
  ( *context ).Gain379[ (int)( 3 ) ][ (int)( 6 ) ] = 1;
  ( *context ).Gain379[ (int)( 3 ) ][ (int)( 7 ) ] = 1;
  ( *context ).Gain379[ (int)( 3 ) ][ (int)( 8 ) ] = 1;
  ( *context ).Gain379[ (int)( 3 ) ][ (int)( 9 ) ] = 1;
  ( *context ).Gain950 = 0.5 * 0.10000;
}

