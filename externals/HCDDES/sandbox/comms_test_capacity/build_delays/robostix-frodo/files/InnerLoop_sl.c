#include "InnerLoop_sl.h"



/* SIMPLIFIED PROGRAM FUNCTIONS */
void InnerLoop_main( InnerLoop_100000156_context *context, float att_ref_2[4], float att_data_3[6], float (*Torque_4)[4] ) {
  InnerLoop_100000156_main( context, att_ref_2, att_data_3, Torque_4 );
}

void InnerLoop_init( InnerLoop_100000156_context *context ) {
  InnerLoop_100000156_init( context );
}

void InnerLoop_100000156_main( InnerLoop_100000156_context *context, float att_ref_2[4], float att_data_3[6], float (*Torque_4)[4] )
{
  float sig_0[10];
  float sig_1[4];
  Ssignal1CA4floatACsignal2CA6floatAS sig_2;
  int ix105;
  int ix118;
  int ix_1;
  int ix_0;
  int ix_3;
  int kx713;
  float sum714;
  int ix716;
  int ix912;

  ix105 = 0;
  while( ix105 < 4 )
  {
    sig_2.signal1[ (int)( ix105 ) ] = att_ref_2[ (int)( ix105 ) ];
    ++ix105;
  }

  ix118 = 0;
  while( ix118 < 6 )
  {
    sig_2.signal2[ (int)( ix118 ) ] = att_data_3[ (int)( ix118 ) ];
    ++ix118;
  }

  ix_1 = 0;
  ix_0 = 0;
  ix_3 = 0;
  while( ix_3 < 4 )
  {
    sig_0[ (int)( ix_0 ) ] = sig_2.signal1[ (int)( ix_1 ) ];
    ++ix_1;
    ++ix_0;
    ++ix_3;
  }

  ix_1 = 0;
  ix_3 = 0;
  while( ix_3 < 6 )
  {
    sig_0[ (int)( ix_0 ) ] = sig_2.signal2[ (int)( ix_1 ) ];
    ++ix_0;
    ++ix_1;
    ++ix_3;
  }

  ix716 = 0;
  while( ix716 < 4 )
  {
    kx713 = 0;
    sum714 = 0;
    while( kx713 < 10 )
    {
      sum714 += ( *context ).Gain707[ (int)( ix716 ) ][ (int)( kx713 ) ] * sig_0[ (int)( kx713 ) ];
      ++kx713;
    }

    sig_1[ (int)( ix716 ) ] = sum714;
    ++ix716;
  }

  ix912 = 0;
  while( ix912 < 4 )
  {
    ( *Torque_4 )[ (int)( ix912 ) ] = sig_1[ (int)( ix912 ) ] * ( *context ).Gain907;
    ++ix912;
  }

}

void InnerLoop_100000156_init( InnerLoop_100000156_context *context )
{
  ( *context ).Gain707[ (int)( 0 ) ][ (int)( 0 ) ] = 1;
  ( *context ).Gain707[ (int)( 0 ) ][ (int)( 1 ) ] = 0;
  ( *context ).Gain707[ (int)( 0 ) ][ (int)( 2 ) ] = 0;
  ( *context ).Gain707[ (int)( 0 ) ][ (int)( 3 ) ] = 0;
  ( *context ).Gain707[ (int)( 0 ) ][ (int)( 4 ) ] = 0;
  ( *context ).Gain707[ (int)( 0 ) ][ (int)( 5 ) ] = 0;
  ( *context ).Gain707[ (int)( 0 ) ][ (int)( 6 ) ] = 0;
  ( *context ).Gain707[ (int)( 0 ) ][ (int)( 7 ) ] = 0;
  ( *context ).Gain707[ (int)( 0 ) ][ (int)( 8 ) ] = 0;
  ( *context ).Gain707[ (int)( 0 ) ][ (int)( 9 ) ] = 0;
  ( *context ).Gain707[ (int)( 1 ) ][ (int)( 0 ) ] = 0;
  ( *context ).Gain707[ (int)( 1 ) ][ (int)( 1 ) ] = 1;
  ( *context ).Gain707[ (int)( 1 ) ][ (int)( 2 ) ] = 1;
  ( *context ).Gain707[ (int)( 1 ) ][ (int)( 3 ) ] = 1;
  ( *context ).Gain707[ (int)( 1 ) ][ (int)( 4 ) ] = 0;
  ( *context ).Gain707[ (int)( 1 ) ][ (int)( 5 ) ] = 0;
  ( *context ).Gain707[ (int)( 1 ) ][ (int)( 6 ) ] = 0;
  ( *context ).Gain707[ (int)( 1 ) ][ (int)( 7 ) ] = 0;
  ( *context ).Gain707[ (int)( 1 ) ][ (int)( 8 ) ] = 0;
  ( *context ).Gain707[ (int)( 1 ) ][ (int)( 9 ) ] = 0;
  ( *context ).Gain707[ (int)( 2 ) ][ (int)( 0 ) ] = 0;
  ( *context ).Gain707[ (int)( 2 ) ][ (int)( 1 ) ] = 0;
  ( *context ).Gain707[ (int)( 2 ) ][ (int)( 2 ) ] = 0;
  ( *context ).Gain707[ (int)( 2 ) ][ (int)( 3 ) ] = 0;
  ( *context ).Gain707[ (int)( 2 ) ][ (int)( 4 ) ] = 1;
  ( *context ).Gain707[ (int)( 2 ) ][ (int)( 5 ) ] = 1;
  ( *context ).Gain707[ (int)( 2 ) ][ (int)( 6 ) ] = 1;
  ( *context ).Gain707[ (int)( 2 ) ][ (int)( 7 ) ] = 0;
  ( *context ).Gain707[ (int)( 2 ) ][ (int)( 8 ) ] = 0;
  ( *context ).Gain707[ (int)( 2 ) ][ (int)( 9 ) ] = 0;
  ( *context ).Gain707[ (int)( 3 ) ][ (int)( 0 ) ] = 0;
  ( *context ).Gain707[ (int)( 3 ) ][ (int)( 1 ) ] = 0;
  ( *context ).Gain707[ (int)( 3 ) ][ (int)( 2 ) ] = 0;
  ( *context ).Gain707[ (int)( 3 ) ][ (int)( 3 ) ] = 0;
  ( *context ).Gain707[ (int)( 3 ) ][ (int)( 4 ) ] = 0;
  ( *context ).Gain707[ (int)( 3 ) ][ (int)( 5 ) ] = 0;
  ( *context ).Gain707[ (int)( 3 ) ][ (int)( 6 ) ] = 0;
  ( *context ).Gain707[ (int)( 3 ) ][ (int)( 7 ) ] = 1;
  ( *context ).Gain707[ (int)( 3 ) ][ (int)( 8 ) ] = 1;
  ( *context ).Gain707[ (int)( 3 ) ][ (int)( 9 ) ] = 1;
  ( *context ).Gain907 = 0.010000;
}

