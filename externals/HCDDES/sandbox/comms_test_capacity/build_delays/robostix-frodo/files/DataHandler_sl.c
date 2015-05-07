#include "DataHandler_sl.h"



/* SIMPLIFIED PROGRAM FUNCTIONS */
void DataHandler_main( DataHandler_100000155_context *context, float data1_2[9], float data2_3[6], float (*att_data_4)[6], float (*pos_data_5)[12] ) {
  DataHandler_100000155_main( context, data1_2, data2_3, att_data_4, pos_data_5 );
}

void DataHandler_init( DataHandler_100000155_context *context ) {
  DataHandler_100000155_init( context );
}

void DataHandler_100000155_main( DataHandler_100000155_context *context, float data1_2[9], float data2_3[6], float (*att_data_4)[6], float (*pos_data_5)[12] )
{
  float sig_0[15];
  float sig_1[6];
  float sig_2[12];
  Ssignal1CA9floatACsignal2CA6floatAS sig_3;
  int ix78;
  int ix92;
  int ix_0;
  int ix_1;
  int ix_2;
  int kx605;
  float sum606;
  int ix608;
  int ix896;
  int kx1125;
  float sum1126;
  int ix1128;
  int ix1152;

  ix78 = 0;
  while( ix78 < 9 )
  {
    sig_3.signal1[ (int)( ix78 ) ] = data1_2[ (int)( ix78 ) ];
    ++ix78;
  }

  ix92 = 0;
  while( ix92 < 6 )
  {
    sig_3.signal2[ (int)( ix92 ) ] = data2_3[ (int)( ix92 ) ];
    ++ix92;
  }

  ix_0 = 0;
  ix_1 = 0;
  ix_2 = 0;
  while( ix_2 < 9 )
  {
    sig_0[ (int)( ix_1 ) ] = sig_3.signal1[ (int)( ix_0 ) ];
    ++ix_0;
    ++ix_1;
    ++ix_2;
  }

  ix_0 = 0;
  ix_2 = 0;
  while( ix_2 < 6 )
  {
    sig_0[ (int)( ix_1 ) ] = sig_3.signal2[ (int)( ix_0 ) ];
    ++ix_1;
    ++ix_0;
    ++ix_2;
  }

  ix608 = 0;
  while( ix608 < 12 )
  {
    kx605 = 0;
    sum606 = 0;
    while( kx605 < 15 )
    {
      sum606 += ( *context ).Gain599[ (int)( ix608 ) ][ (int)( kx605 ) ] * sig_0[ (int)( kx605 ) ];
      ++kx605;
    }

    sig_2[ (int)( ix608 ) ] = sum606;
    ++ix608;
  }

  ix896 = 0;
  while( ix896 < 12 )
  {
    ( *pos_data_5 )[ (int)( ix896 ) ] = sig_2[ (int)( ix896 ) ] * ( *context ).Gain891;
    ++ix896;
  }

  ix1128 = 0;
  while( ix1128 < 6 )
  {
    kx1125 = 0;
    sum1126 = 0;
    while( kx1125 < 15 )
    {
      sum1126 += ( *context ).Gain1119[ (int)( ix1128 ) ][ (int)( kx1125 ) ] * sig_0[ (int)( kx1125 ) ];
      ++kx1125;
    }

    sig_1[ (int)( ix1128 ) ] = sum1126;
    ++ix1128;
  }

  ix1152 = 0;
  while( ix1152 < 6 )
  {
    ( *att_data_4 )[ (int)( ix1152 ) ] = sig_1[ (int)( ix1152 ) ] * ( *context ).Gain1147;
    ++ix1152;
  }

}

void DataHandler_100000155_init( DataHandler_100000155_context *context )
{
  ( *context ).Gain599[ (int)( 0 ) ][ (int)( 0 ) ] = 1;
  ( *context ).Gain599[ (int)( 0 ) ][ (int)( 1 ) ] = 1;
  ( *context ).Gain599[ (int)( 0 ) ][ (int)( 2 ) ] = 1;
  ( *context ).Gain599[ (int)( 0 ) ][ (int)( 3 ) ] = 1;
  ( *context ).Gain599[ (int)( 0 ) ][ (int)( 4 ) ] = 1;
  ( *context ).Gain599[ (int)( 0 ) ][ (int)( 5 ) ] = 1;
  ( *context ).Gain599[ (int)( 0 ) ][ (int)( 6 ) ] = 1;
  ( *context ).Gain599[ (int)( 0 ) ][ (int)( 7 ) ] = 1;
  ( *context ).Gain599[ (int)( 0 ) ][ (int)( 8 ) ] = 1;
  ( *context ).Gain599[ (int)( 0 ) ][ (int)( 9 ) ] = 1;
  ( *context ).Gain599[ (int)( 0 ) ][ (int)( 10 ) ] = 1;
  ( *context ).Gain599[ (int)( 0 ) ][ (int)( 11 ) ] = 1;
  ( *context ).Gain599[ (int)( 0 ) ][ (int)( 12 ) ] = 1;
  ( *context ).Gain599[ (int)( 0 ) ][ (int)( 13 ) ] = 1;
  ( *context ).Gain599[ (int)( 0 ) ][ (int)( 14 ) ] = 1;
  ( *context ).Gain599[ (int)( 1 ) ][ (int)( 0 ) ] = 1;
  ( *context ).Gain599[ (int)( 1 ) ][ (int)( 1 ) ] = 1;
  ( *context ).Gain599[ (int)( 1 ) ][ (int)( 2 ) ] = 1;
  ( *context ).Gain599[ (int)( 1 ) ][ (int)( 3 ) ] = 1;
  ( *context ).Gain599[ (int)( 1 ) ][ (int)( 4 ) ] = 1;
  ( *context ).Gain599[ (int)( 1 ) ][ (int)( 5 ) ] = 1;
  ( *context ).Gain599[ (int)( 1 ) ][ (int)( 6 ) ] = 1;
  ( *context ).Gain599[ (int)( 1 ) ][ (int)( 7 ) ] = 1;
  ( *context ).Gain599[ (int)( 1 ) ][ (int)( 8 ) ] = 1;
  ( *context ).Gain599[ (int)( 1 ) ][ (int)( 9 ) ] = 1;
  ( *context ).Gain599[ (int)( 1 ) ][ (int)( 10 ) ] = 1;
  ( *context ).Gain599[ (int)( 1 ) ][ (int)( 11 ) ] = 1;
  ( *context ).Gain599[ (int)( 1 ) ][ (int)( 12 ) ] = 1;
  ( *context ).Gain599[ (int)( 1 ) ][ (int)( 13 ) ] = 1;
  ( *context ).Gain599[ (int)( 1 ) ][ (int)( 14 ) ] = 1;
  ( *context ).Gain599[ (int)( 2 ) ][ (int)( 0 ) ] = 1;
  ( *context ).Gain599[ (int)( 2 ) ][ (int)( 1 ) ] = 1;
  ( *context ).Gain599[ (int)( 2 ) ][ (int)( 2 ) ] = 1;
  ( *context ).Gain599[ (int)( 2 ) ][ (int)( 3 ) ] = 1;
  ( *context ).Gain599[ (int)( 2 ) ][ (int)( 4 ) ] = 1;
  ( *context ).Gain599[ (int)( 2 ) ][ (int)( 5 ) ] = 1;
  ( *context ).Gain599[ (int)( 2 ) ][ (int)( 6 ) ] = 1;
  ( *context ).Gain599[ (int)( 2 ) ][ (int)( 7 ) ] = 1;
  ( *context ).Gain599[ (int)( 2 ) ][ (int)( 8 ) ] = 1;
  ( *context ).Gain599[ (int)( 2 ) ][ (int)( 9 ) ] = 1;
  ( *context ).Gain599[ (int)( 2 ) ][ (int)( 10 ) ] = 1;
  ( *context ).Gain599[ (int)( 2 ) ][ (int)( 11 ) ] = 1;
  ( *context ).Gain599[ (int)( 2 ) ][ (int)( 12 ) ] = 1;
  ( *context ).Gain599[ (int)( 2 ) ][ (int)( 13 ) ] = 1;
  ( *context ).Gain599[ (int)( 2 ) ][ (int)( 14 ) ] = 1;
  ( *context ).Gain599[ (int)( 3 ) ][ (int)( 0 ) ] = 1;
  ( *context ).Gain599[ (int)( 3 ) ][ (int)( 1 ) ] = 1;
  ( *context ).Gain599[ (int)( 3 ) ][ (int)( 2 ) ] = 1;
  ( *context ).Gain599[ (int)( 3 ) ][ (int)( 3 ) ] = 1;
  ( *context ).Gain599[ (int)( 3 ) ][ (int)( 4 ) ] = 1;
  ( *context ).Gain599[ (int)( 3 ) ][ (int)( 5 ) ] = 1;
  ( *context ).Gain599[ (int)( 3 ) ][ (int)( 6 ) ] = 1;
  ( *context ).Gain599[ (int)( 3 ) ][ (int)( 7 ) ] = 1;
  ( *context ).Gain599[ (int)( 3 ) ][ (int)( 8 ) ] = 1;
  ( *context ).Gain599[ (int)( 3 ) ][ (int)( 9 ) ] = 1;
  ( *context ).Gain599[ (int)( 3 ) ][ (int)( 10 ) ] = 1;
  ( *context ).Gain599[ (int)( 3 ) ][ (int)( 11 ) ] = 1;
  ( *context ).Gain599[ (int)( 3 ) ][ (int)( 12 ) ] = 1;
  ( *context ).Gain599[ (int)( 3 ) ][ (int)( 13 ) ] = 1;
  ( *context ).Gain599[ (int)( 3 ) ][ (int)( 14 ) ] = 1;
  ( *context ).Gain599[ (int)( 4 ) ][ (int)( 0 ) ] = 1;
  ( *context ).Gain599[ (int)( 4 ) ][ (int)( 1 ) ] = 1;
  ( *context ).Gain599[ (int)( 4 ) ][ (int)( 2 ) ] = 1;
  ( *context ).Gain599[ (int)( 4 ) ][ (int)( 3 ) ] = 1;
  ( *context ).Gain599[ (int)( 4 ) ][ (int)( 4 ) ] = 1;
  ( *context ).Gain599[ (int)( 4 ) ][ (int)( 5 ) ] = 1;
  ( *context ).Gain599[ (int)( 4 ) ][ (int)( 6 ) ] = 1;
  ( *context ).Gain599[ (int)( 4 ) ][ (int)( 7 ) ] = 1;
  ( *context ).Gain599[ (int)( 4 ) ][ (int)( 8 ) ] = 1;
  ( *context ).Gain599[ (int)( 4 ) ][ (int)( 9 ) ] = 1;
  ( *context ).Gain599[ (int)( 4 ) ][ (int)( 10 ) ] = 1;
  ( *context ).Gain599[ (int)( 4 ) ][ (int)( 11 ) ] = 1;
  ( *context ).Gain599[ (int)( 4 ) ][ (int)( 12 ) ] = 1;
  ( *context ).Gain599[ (int)( 4 ) ][ (int)( 13 ) ] = 1;
  ( *context ).Gain599[ (int)( 4 ) ][ (int)( 14 ) ] = 1;
  ( *context ).Gain599[ (int)( 5 ) ][ (int)( 0 ) ] = 1;
  ( *context ).Gain599[ (int)( 5 ) ][ (int)( 1 ) ] = 1;
  ( *context ).Gain599[ (int)( 5 ) ][ (int)( 2 ) ] = 1;
  ( *context ).Gain599[ (int)( 5 ) ][ (int)( 3 ) ] = 1;
  ( *context ).Gain599[ (int)( 5 ) ][ (int)( 4 ) ] = 1;
  ( *context ).Gain599[ (int)( 5 ) ][ (int)( 5 ) ] = 1;
  ( *context ).Gain599[ (int)( 5 ) ][ (int)( 6 ) ] = 1;
  ( *context ).Gain599[ (int)( 5 ) ][ (int)( 7 ) ] = 1;
  ( *context ).Gain599[ (int)( 5 ) ][ (int)( 8 ) ] = 1;
  ( *context ).Gain599[ (int)( 5 ) ][ (int)( 9 ) ] = 1;
  ( *context ).Gain599[ (int)( 5 ) ][ (int)( 10 ) ] = 1;
  ( *context ).Gain599[ (int)( 5 ) ][ (int)( 11 ) ] = 1;
  ( *context ).Gain599[ (int)( 5 ) ][ (int)( 12 ) ] = 1;
  ( *context ).Gain599[ (int)( 5 ) ][ (int)( 13 ) ] = 1;
  ( *context ).Gain599[ (int)( 5 ) ][ (int)( 14 ) ] = 1;
  ( *context ).Gain599[ (int)( 6 ) ][ (int)( 0 ) ] = 1;
  ( *context ).Gain599[ (int)( 6 ) ][ (int)( 1 ) ] = 1;
  ( *context ).Gain599[ (int)( 6 ) ][ (int)( 2 ) ] = 1;
  ( *context ).Gain599[ (int)( 6 ) ][ (int)( 3 ) ] = 1;
  ( *context ).Gain599[ (int)( 6 ) ][ (int)( 4 ) ] = 1;
  ( *context ).Gain599[ (int)( 6 ) ][ (int)( 5 ) ] = 1;
  ( *context ).Gain599[ (int)( 6 ) ][ (int)( 6 ) ] = 1;
  ( *context ).Gain599[ (int)( 6 ) ][ (int)( 7 ) ] = 1;
  ( *context ).Gain599[ (int)( 6 ) ][ (int)( 8 ) ] = 1;
  ( *context ).Gain599[ (int)( 6 ) ][ (int)( 9 ) ] = 1;
  ( *context ).Gain599[ (int)( 6 ) ][ (int)( 10 ) ] = 1;
  ( *context ).Gain599[ (int)( 6 ) ][ (int)( 11 ) ] = 1;
  ( *context ).Gain599[ (int)( 6 ) ][ (int)( 12 ) ] = 1;
  ( *context ).Gain599[ (int)( 6 ) ][ (int)( 13 ) ] = 1;
  ( *context ).Gain599[ (int)( 6 ) ][ (int)( 14 ) ] = 1;
  ( *context ).Gain599[ (int)( 7 ) ][ (int)( 0 ) ] = 1;
  ( *context ).Gain599[ (int)( 7 ) ][ (int)( 1 ) ] = 1;
  ( *context ).Gain599[ (int)( 7 ) ][ (int)( 2 ) ] = 1;
  ( *context ).Gain599[ (int)( 7 ) ][ (int)( 3 ) ] = 1;
  ( *context ).Gain599[ (int)( 7 ) ][ (int)( 4 ) ] = 1;
  ( *context ).Gain599[ (int)( 7 ) ][ (int)( 5 ) ] = 1;
  ( *context ).Gain599[ (int)( 7 ) ][ (int)( 6 ) ] = 1;
  ( *context ).Gain599[ (int)( 7 ) ][ (int)( 7 ) ] = 1;
  ( *context ).Gain599[ (int)( 7 ) ][ (int)( 8 ) ] = 1;
  ( *context ).Gain599[ (int)( 7 ) ][ (int)( 9 ) ] = 1;
  ( *context ).Gain599[ (int)( 7 ) ][ (int)( 10 ) ] = 1;
  ( *context ).Gain599[ (int)( 7 ) ][ (int)( 11 ) ] = 1;
  ( *context ).Gain599[ (int)( 7 ) ][ (int)( 12 ) ] = 1;
  ( *context ).Gain599[ (int)( 7 ) ][ (int)( 13 ) ] = 1;
  ( *context ).Gain599[ (int)( 7 ) ][ (int)( 14 ) ] = 1;
  ( *context ).Gain599[ (int)( 8 ) ][ (int)( 0 ) ] = 1;
  ( *context ).Gain599[ (int)( 8 ) ][ (int)( 1 ) ] = 1;
  ( *context ).Gain599[ (int)( 8 ) ][ (int)( 2 ) ] = 1;
  ( *context ).Gain599[ (int)( 8 ) ][ (int)( 3 ) ] = 1;
  ( *context ).Gain599[ (int)( 8 ) ][ (int)( 4 ) ] = 1;
  ( *context ).Gain599[ (int)( 8 ) ][ (int)( 5 ) ] = 1;
  ( *context ).Gain599[ (int)( 8 ) ][ (int)( 6 ) ] = 1;
  ( *context ).Gain599[ (int)( 8 ) ][ (int)( 7 ) ] = 1;
  ( *context ).Gain599[ (int)( 8 ) ][ (int)( 8 ) ] = 1;
  ( *context ).Gain599[ (int)( 8 ) ][ (int)( 9 ) ] = 1;
  ( *context ).Gain599[ (int)( 8 ) ][ (int)( 10 ) ] = 1;
  ( *context ).Gain599[ (int)( 8 ) ][ (int)( 11 ) ] = 1;
  ( *context ).Gain599[ (int)( 8 ) ][ (int)( 12 ) ] = 1;
  ( *context ).Gain599[ (int)( 8 ) ][ (int)( 13 ) ] = 1;
  ( *context ).Gain599[ (int)( 8 ) ][ (int)( 14 ) ] = 1;
  ( *context ).Gain599[ (int)( 9 ) ][ (int)( 0 ) ] = 1;
  ( *context ).Gain599[ (int)( 9 ) ][ (int)( 1 ) ] = 1;
  ( *context ).Gain599[ (int)( 9 ) ][ (int)( 2 ) ] = 1;
  ( *context ).Gain599[ (int)( 9 ) ][ (int)( 3 ) ] = 1;
  ( *context ).Gain599[ (int)( 9 ) ][ (int)( 4 ) ] = 1;
  ( *context ).Gain599[ (int)( 9 ) ][ (int)( 5 ) ] = 1;
  ( *context ).Gain599[ (int)( 9 ) ][ (int)( 6 ) ] = 1;
  ( *context ).Gain599[ (int)( 9 ) ][ (int)( 7 ) ] = 1;
  ( *context ).Gain599[ (int)( 9 ) ][ (int)( 8 ) ] = 1;
  ( *context ).Gain599[ (int)( 9 ) ][ (int)( 9 ) ] = 1;
  ( *context ).Gain599[ (int)( 9 ) ][ (int)( 10 ) ] = 1;
  ( *context ).Gain599[ (int)( 9 ) ][ (int)( 11 ) ] = 1;
  ( *context ).Gain599[ (int)( 9 ) ][ (int)( 12 ) ] = 1;
  ( *context ).Gain599[ (int)( 9 ) ][ (int)( 13 ) ] = 1;
  ( *context ).Gain599[ (int)( 9 ) ][ (int)( 14 ) ] = 1;
  ( *context ).Gain599[ (int)( 10 ) ][ (int)( 0 ) ] = 1;
  ( *context ).Gain599[ (int)( 10 ) ][ (int)( 1 ) ] = 1;
  ( *context ).Gain599[ (int)( 10 ) ][ (int)( 2 ) ] = 1;
  ( *context ).Gain599[ (int)( 10 ) ][ (int)( 3 ) ] = 1;
  ( *context ).Gain599[ (int)( 10 ) ][ (int)( 4 ) ] = 1;
  ( *context ).Gain599[ (int)( 10 ) ][ (int)( 5 ) ] = 1;
  ( *context ).Gain599[ (int)( 10 ) ][ (int)( 6 ) ] = 1;
  ( *context ).Gain599[ (int)( 10 ) ][ (int)( 7 ) ] = 1;
  ( *context ).Gain599[ (int)( 10 ) ][ (int)( 8 ) ] = 1;
  ( *context ).Gain599[ (int)( 10 ) ][ (int)( 9 ) ] = 1;
  ( *context ).Gain599[ (int)( 10 ) ][ (int)( 10 ) ] = 1;
  ( *context ).Gain599[ (int)( 10 ) ][ (int)( 11 ) ] = 1;
  ( *context ).Gain599[ (int)( 10 ) ][ (int)( 12 ) ] = 1;
  ( *context ).Gain599[ (int)( 10 ) ][ (int)( 13 ) ] = 1;
  ( *context ).Gain599[ (int)( 10 ) ][ (int)( 14 ) ] = 1;
  ( *context ).Gain599[ (int)( 11 ) ][ (int)( 0 ) ] = 1;
  ( *context ).Gain599[ (int)( 11 ) ][ (int)( 1 ) ] = 1;
  ( *context ).Gain599[ (int)( 11 ) ][ (int)( 2 ) ] = 1;
  ( *context ).Gain599[ (int)( 11 ) ][ (int)( 3 ) ] = 1;
  ( *context ).Gain599[ (int)( 11 ) ][ (int)( 4 ) ] = 1;
  ( *context ).Gain599[ (int)( 11 ) ][ (int)( 5 ) ] = 1;
  ( *context ).Gain599[ (int)( 11 ) ][ (int)( 6 ) ] = 1;
  ( *context ).Gain599[ (int)( 11 ) ][ (int)( 7 ) ] = 1;
  ( *context ).Gain599[ (int)( 11 ) ][ (int)( 8 ) ] = 1;
  ( *context ).Gain599[ (int)( 11 ) ][ (int)( 9 ) ] = 1;
  ( *context ).Gain599[ (int)( 11 ) ][ (int)( 10 ) ] = 1;
  ( *context ).Gain599[ (int)( 11 ) ][ (int)( 11 ) ] = 1;
  ( *context ).Gain599[ (int)( 11 ) ][ (int)( 12 ) ] = 1;
  ( *context ).Gain599[ (int)( 11 ) ][ (int)( 13 ) ] = 1;
  ( *context ).Gain599[ (int)( 11 ) ][ (int)( 14 ) ] = 1;
  ( *context ).Gain891 = 0.0666999999999999950000;
  ( *context ).Gain1119[ (int)( 0 ) ][ (int)( 0 ) ] = 1;
  ( *context ).Gain1119[ (int)( 0 ) ][ (int)( 1 ) ] = 1;
  ( *context ).Gain1119[ (int)( 0 ) ][ (int)( 2 ) ] = 1;
  ( *context ).Gain1119[ (int)( 0 ) ][ (int)( 3 ) ] = 1;
  ( *context ).Gain1119[ (int)( 0 ) ][ (int)( 4 ) ] = 1;
  ( *context ).Gain1119[ (int)( 0 ) ][ (int)( 5 ) ] = 1;
  ( *context ).Gain1119[ (int)( 0 ) ][ (int)( 6 ) ] = 1;
  ( *context ).Gain1119[ (int)( 0 ) ][ (int)( 7 ) ] = 1;
  ( *context ).Gain1119[ (int)( 0 ) ][ (int)( 8 ) ] = 1;
  ( *context ).Gain1119[ (int)( 0 ) ][ (int)( 9 ) ] = 1;
  ( *context ).Gain1119[ (int)( 0 ) ][ (int)( 10 ) ] = 1;
  ( *context ).Gain1119[ (int)( 0 ) ][ (int)( 11 ) ] = 1;
  ( *context ).Gain1119[ (int)( 0 ) ][ (int)( 12 ) ] = 1;
  ( *context ).Gain1119[ (int)( 0 ) ][ (int)( 13 ) ] = 1;
  ( *context ).Gain1119[ (int)( 0 ) ][ (int)( 14 ) ] = 1;
  ( *context ).Gain1119[ (int)( 1 ) ][ (int)( 0 ) ] = 1;
  ( *context ).Gain1119[ (int)( 1 ) ][ (int)( 1 ) ] = 1;
  ( *context ).Gain1119[ (int)( 1 ) ][ (int)( 2 ) ] = 1;
  ( *context ).Gain1119[ (int)( 1 ) ][ (int)( 3 ) ] = 1;
  ( *context ).Gain1119[ (int)( 1 ) ][ (int)( 4 ) ] = 1;
  ( *context ).Gain1119[ (int)( 1 ) ][ (int)( 5 ) ] = 1;
  ( *context ).Gain1119[ (int)( 1 ) ][ (int)( 6 ) ] = 1;
  ( *context ).Gain1119[ (int)( 1 ) ][ (int)( 7 ) ] = 1;
  ( *context ).Gain1119[ (int)( 1 ) ][ (int)( 8 ) ] = 1;
  ( *context ).Gain1119[ (int)( 1 ) ][ (int)( 9 ) ] = 1;
  ( *context ).Gain1119[ (int)( 1 ) ][ (int)( 10 ) ] = 1;
  ( *context ).Gain1119[ (int)( 1 ) ][ (int)( 11 ) ] = 1;
  ( *context ).Gain1119[ (int)( 1 ) ][ (int)( 12 ) ] = 1;
  ( *context ).Gain1119[ (int)( 1 ) ][ (int)( 13 ) ] = 1;
  ( *context ).Gain1119[ (int)( 1 ) ][ (int)( 14 ) ] = 1;
  ( *context ).Gain1119[ (int)( 2 ) ][ (int)( 0 ) ] = 1;
  ( *context ).Gain1119[ (int)( 2 ) ][ (int)( 1 ) ] = 1;
  ( *context ).Gain1119[ (int)( 2 ) ][ (int)( 2 ) ] = 1;
  ( *context ).Gain1119[ (int)( 2 ) ][ (int)( 3 ) ] = 1;
  ( *context ).Gain1119[ (int)( 2 ) ][ (int)( 4 ) ] = 1;
  ( *context ).Gain1119[ (int)( 2 ) ][ (int)( 5 ) ] = 1;
  ( *context ).Gain1119[ (int)( 2 ) ][ (int)( 6 ) ] = 1;
  ( *context ).Gain1119[ (int)( 2 ) ][ (int)( 7 ) ] = 1;
  ( *context ).Gain1119[ (int)( 2 ) ][ (int)( 8 ) ] = 1;
  ( *context ).Gain1119[ (int)( 2 ) ][ (int)( 9 ) ] = 1;
  ( *context ).Gain1119[ (int)( 2 ) ][ (int)( 10 ) ] = 1;
  ( *context ).Gain1119[ (int)( 2 ) ][ (int)( 11 ) ] = 1;
  ( *context ).Gain1119[ (int)( 2 ) ][ (int)( 12 ) ] = 1;
  ( *context ).Gain1119[ (int)( 2 ) ][ (int)( 13 ) ] = 1;
  ( *context ).Gain1119[ (int)( 2 ) ][ (int)( 14 ) ] = 1;
  ( *context ).Gain1119[ (int)( 3 ) ][ (int)( 0 ) ] = 1;
  ( *context ).Gain1119[ (int)( 3 ) ][ (int)( 1 ) ] = 1;
  ( *context ).Gain1119[ (int)( 3 ) ][ (int)( 2 ) ] = 1;
  ( *context ).Gain1119[ (int)( 3 ) ][ (int)( 3 ) ] = 1;
  ( *context ).Gain1119[ (int)( 3 ) ][ (int)( 4 ) ] = 1;
  ( *context ).Gain1119[ (int)( 3 ) ][ (int)( 5 ) ] = 1;
  ( *context ).Gain1119[ (int)( 3 ) ][ (int)( 6 ) ] = 1;
  ( *context ).Gain1119[ (int)( 3 ) ][ (int)( 7 ) ] = 1;
  ( *context ).Gain1119[ (int)( 3 ) ][ (int)( 8 ) ] = 1;
  ( *context ).Gain1119[ (int)( 3 ) ][ (int)( 9 ) ] = 1;
  ( *context ).Gain1119[ (int)( 3 ) ][ (int)( 10 ) ] = 1;
  ( *context ).Gain1119[ (int)( 3 ) ][ (int)( 11 ) ] = 1;
  ( *context ).Gain1119[ (int)( 3 ) ][ (int)( 12 ) ] = 1;
  ( *context ).Gain1119[ (int)( 3 ) ][ (int)( 13 ) ] = 1;
  ( *context ).Gain1119[ (int)( 3 ) ][ (int)( 14 ) ] = 1;
  ( *context ).Gain1119[ (int)( 4 ) ][ (int)( 0 ) ] = 1;
  ( *context ).Gain1119[ (int)( 4 ) ][ (int)( 1 ) ] = 1;
  ( *context ).Gain1119[ (int)( 4 ) ][ (int)( 2 ) ] = 1;
  ( *context ).Gain1119[ (int)( 4 ) ][ (int)( 3 ) ] = 1;
  ( *context ).Gain1119[ (int)( 4 ) ][ (int)( 4 ) ] = 1;
  ( *context ).Gain1119[ (int)( 4 ) ][ (int)( 5 ) ] = 1;
  ( *context ).Gain1119[ (int)( 4 ) ][ (int)( 6 ) ] = 1;
  ( *context ).Gain1119[ (int)( 4 ) ][ (int)( 7 ) ] = 1;
  ( *context ).Gain1119[ (int)( 4 ) ][ (int)( 8 ) ] = 1;
  ( *context ).Gain1119[ (int)( 4 ) ][ (int)( 9 ) ] = 1;
  ( *context ).Gain1119[ (int)( 4 ) ][ (int)( 10 ) ] = 1;
  ( *context ).Gain1119[ (int)( 4 ) ][ (int)( 11 ) ] = 1;
  ( *context ).Gain1119[ (int)( 4 ) ][ (int)( 12 ) ] = 1;
  ( *context ).Gain1119[ (int)( 4 ) ][ (int)( 13 ) ] = 1;
  ( *context ).Gain1119[ (int)( 4 ) ][ (int)( 14 ) ] = 1;
  ( *context ).Gain1119[ (int)( 5 ) ][ (int)( 0 ) ] = 1;
  ( *context ).Gain1119[ (int)( 5 ) ][ (int)( 1 ) ] = 1;
  ( *context ).Gain1119[ (int)( 5 ) ][ (int)( 2 ) ] = 1;
  ( *context ).Gain1119[ (int)( 5 ) ][ (int)( 3 ) ] = 1;
  ( *context ).Gain1119[ (int)( 5 ) ][ (int)( 4 ) ] = 1;
  ( *context ).Gain1119[ (int)( 5 ) ][ (int)( 5 ) ] = 1;
  ( *context ).Gain1119[ (int)( 5 ) ][ (int)( 6 ) ] = 1;
  ( *context ).Gain1119[ (int)( 5 ) ][ (int)( 7 ) ] = 1;
  ( *context ).Gain1119[ (int)( 5 ) ][ (int)( 8 ) ] = 1;
  ( *context ).Gain1119[ (int)( 5 ) ][ (int)( 9 ) ] = 1;
  ( *context ).Gain1119[ (int)( 5 ) ][ (int)( 10 ) ] = 1;
  ( *context ).Gain1119[ (int)( 5 ) ][ (int)( 11 ) ] = 1;
  ( *context ).Gain1119[ (int)( 5 ) ][ (int)( 12 ) ] = 1;
  ( *context ).Gain1119[ (int)( 5 ) ][ (int)( 13 ) ] = 1;
  ( *context ).Gain1119[ (int)( 5 ) ][ (int)( 14 ) ] = 1;
  ( *context ).Gain1147 = 0.0666999999999999950000;
}

