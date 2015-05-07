#include "OuterLoop_sl.h"



/* SIMPLIFIED PROGRAM FUNCTIONS */
void OuterLoop_main( OuterLoop_100000157_context *context, float pos_ref_2[4], float pos_data_3[12], float (*att_ref_4)[4] ) {
  OuterLoop_100000157_main( context, pos_ref_2, pos_data_3, att_ref_4 );
}

void OuterLoop_init( OuterLoop_100000157_context *context ) {
  OuterLoop_100000157_init( context );
}

void OuterLoop_100000157_main( OuterLoop_100000157_context *context, float pos_ref_2[4], float pos_data_3[12], float (*att_ref_4)[4] )
{
  float sig_0[16];
  float sig_1[4];
  Ssignal1CA4floatACsignal2CA12floatAS sig_2;
  int ix131;
  int ix144;
  int ix_0;
  int ix_1;
  int ix_4;
  int kx869;
  float sum870;
  int ix872;
  int ix928;

  ix131 = 0;
  while( ix131 < 4 )
  {
    sig_2.signal1[ (int)( ix131 ) ] = pos_ref_2[ (int)( ix131 ) ];
    ++ix131;
  }

  ix144 = 0;
  while( ix144 < 12 )
  {
    sig_2.signal2[ (int)( ix144 ) ] = pos_data_3[ (int)( ix144 ) ];
    ++ix144;
  }

  ix_0 = 0;
  ix_1 = 0;
  ix_4 = 0;
  while( ix_4 < 4 )
  {
    sig_0[ (int)( ix_1 ) ] = sig_2.signal1[ (int)( ix_0 ) ];
    ++ix_0;
    ++ix_1;
    ++ix_4;
  }

  ix_0 = 0;
  ix_4 = 0;
  while( ix_4 < 12 )
  {
    sig_0[ (int)( ix_1 ) ] = sig_2.signal2[ (int)( ix_0 ) ];
    ++ix_1;
    ++ix_0;
    ++ix_4;
  }

  ix872 = 0;
  while( ix872 < 4 )
  {
    kx869 = 0;
    sum870 = 0;
    while( kx869 < 16 )
    {
      sum870 += ( *context ).Gain863[ (int)( ix872 ) ][ (int)( kx869 ) ] * sig_0[ (int)( kx869 ) ];
      ++kx869;
    }

    sig_1[ (int)( ix872 ) ] = sum870;
    ++ix872;
  }

  ix928 = 0;
  while( ix928 < 4 )
  {
    ( *att_ref_4 )[ (int)( ix928 ) ] = sig_1[ (int)( ix928 ) ] * ( *context ).Gain923;
    ++ix928;
  }

}

void OuterLoop_100000157_init( OuterLoop_100000157_context *context )
{
  ( *context ).Gain863[ (int)( 0 ) ][ (int)( 0 ) ] = 1;
  ( *context ).Gain863[ (int)( 0 ) ][ (int)( 1 ) ] = 1;
  ( *context ).Gain863[ (int)( 0 ) ][ (int)( 2 ) ] = 1;
  ( *context ).Gain863[ (int)( 0 ) ][ (int)( 3 ) ] = 1;
  ( *context ).Gain863[ (int)( 0 ) ][ (int)( 4 ) ] = 0;
  ( *context ).Gain863[ (int)( 0 ) ][ (int)( 5 ) ] = 0;
  ( *context ).Gain863[ (int)( 0 ) ][ (int)( 6 ) ] = 0;
  ( *context ).Gain863[ (int)( 0 ) ][ (int)( 7 ) ] = 0;
  ( *context ).Gain863[ (int)( 0 ) ][ (int)( 8 ) ] = 0;
  ( *context ).Gain863[ (int)( 0 ) ][ (int)( 9 ) ] = 0;
  ( *context ).Gain863[ (int)( 0 ) ][ (int)( 10 ) ] = 0;
  ( *context ).Gain863[ (int)( 0 ) ][ (int)( 11 ) ] = 0;
  ( *context ).Gain863[ (int)( 0 ) ][ (int)( 12 ) ] = 0;
  ( *context ).Gain863[ (int)( 0 ) ][ (int)( 13 ) ] = 0;
  ( *context ).Gain863[ (int)( 0 ) ][ (int)( 14 ) ] = 0;
  ( *context ).Gain863[ (int)( 0 ) ][ (int)( 15 ) ] = 0;
  ( *context ).Gain863[ (int)( 1 ) ][ (int)( 0 ) ] = 0;
  ( *context ).Gain863[ (int)( 1 ) ][ (int)( 1 ) ] = 0;
  ( *context ).Gain863[ (int)( 1 ) ][ (int)( 2 ) ] = 0;
  ( *context ).Gain863[ (int)( 1 ) ][ (int)( 3 ) ] = 0;
  ( *context ).Gain863[ (int)( 1 ) ][ (int)( 4 ) ] = 1;
  ( *context ).Gain863[ (int)( 1 ) ][ (int)( 5 ) ] = 1;
  ( *context ).Gain863[ (int)( 1 ) ][ (int)( 6 ) ] = 1;
  ( *context ).Gain863[ (int)( 1 ) ][ (int)( 7 ) ] = 1;
  ( *context ).Gain863[ (int)( 1 ) ][ (int)( 8 ) ] = 0;
  ( *context ).Gain863[ (int)( 1 ) ][ (int)( 9 ) ] = 0;
  ( *context ).Gain863[ (int)( 1 ) ][ (int)( 10 ) ] = 0;
  ( *context ).Gain863[ (int)( 1 ) ][ (int)( 11 ) ] = 0;
  ( *context ).Gain863[ (int)( 1 ) ][ (int)( 12 ) ] = 0;
  ( *context ).Gain863[ (int)( 1 ) ][ (int)( 13 ) ] = 0;
  ( *context ).Gain863[ (int)( 1 ) ][ (int)( 14 ) ] = 0;
  ( *context ).Gain863[ (int)( 1 ) ][ (int)( 15 ) ] = 0;
  ( *context ).Gain863[ (int)( 2 ) ][ (int)( 0 ) ] = 0;
  ( *context ).Gain863[ (int)( 2 ) ][ (int)( 1 ) ] = 0;
  ( *context ).Gain863[ (int)( 2 ) ][ (int)( 2 ) ] = 0;
  ( *context ).Gain863[ (int)( 2 ) ][ (int)( 3 ) ] = 0;
  ( *context ).Gain863[ (int)( 2 ) ][ (int)( 4 ) ] = 0;
  ( *context ).Gain863[ (int)( 2 ) ][ (int)( 5 ) ] = 0;
  ( *context ).Gain863[ (int)( 2 ) ][ (int)( 6 ) ] = 0;
  ( *context ).Gain863[ (int)( 2 ) ][ (int)( 7 ) ] = 0;
  ( *context ).Gain863[ (int)( 2 ) ][ (int)( 8 ) ] = 1;
  ( *context ).Gain863[ (int)( 2 ) ][ (int)( 9 ) ] = 1;
  ( *context ).Gain863[ (int)( 2 ) ][ (int)( 10 ) ] = 1;
  ( *context ).Gain863[ (int)( 2 ) ][ (int)( 11 ) ] = 1;
  ( *context ).Gain863[ (int)( 2 ) ][ (int)( 12 ) ] = 0;
  ( *context ).Gain863[ (int)( 2 ) ][ (int)( 13 ) ] = 0;
  ( *context ).Gain863[ (int)( 2 ) ][ (int)( 14 ) ] = 0;
  ( *context ).Gain863[ (int)( 2 ) ][ (int)( 15 ) ] = 0;
  ( *context ).Gain863[ (int)( 3 ) ][ (int)( 0 ) ] = 0;
  ( *context ).Gain863[ (int)( 3 ) ][ (int)( 1 ) ] = 0;
  ( *context ).Gain863[ (int)( 3 ) ][ (int)( 2 ) ] = 0;
  ( *context ).Gain863[ (int)( 3 ) ][ (int)( 3 ) ] = 0;
  ( *context ).Gain863[ (int)( 3 ) ][ (int)( 4 ) ] = 0;
  ( *context ).Gain863[ (int)( 3 ) ][ (int)( 5 ) ] = 0;
  ( *context ).Gain863[ (int)( 3 ) ][ (int)( 6 ) ] = 0;
  ( *context ).Gain863[ (int)( 3 ) ][ (int)( 7 ) ] = 0;
  ( *context ).Gain863[ (int)( 3 ) ][ (int)( 8 ) ] = 0;
  ( *context ).Gain863[ (int)( 3 ) ][ (int)( 9 ) ] = 0;
  ( *context ).Gain863[ (int)( 3 ) ][ (int)( 10 ) ] = 0;
  ( *context ).Gain863[ (int)( 3 ) ][ (int)( 11 ) ] = 0;
  ( *context ).Gain863[ (int)( 3 ) ][ (int)( 12 ) ] = 1;
  ( *context ).Gain863[ (int)( 3 ) ][ (int)( 13 ) ] = 1;
  ( *context ).Gain863[ (int)( 3 ) ][ (int)( 14 ) ] = 1;
  ( *context ).Gain863[ (int)( 3 ) ][ (int)( 15 ) ] = 1;
  ( *context ).Gain923 = 0.06250000;
}

