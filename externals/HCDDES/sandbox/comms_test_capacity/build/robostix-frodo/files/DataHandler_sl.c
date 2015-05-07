#include "DataHandler_sl.h"

/* SIMPLIFIED PROGRAM FUNCTIONS */
void DataHandler_main( DataHandler_100000171_context *context, float data1_2[9], float data2_3[6], float (*att_data_4)[6], float (*pos_data_5)[12] ) {
  DataHandler_100000171_main( context, data1_2, data2_3, att_data_4, pos_data_5 );
}

void DataHandler_init( DataHandler_100000171_context *context ) {
  DataHandler_100000171_init( context );
}


void DataHandler_100000171_main( DataHandler_100000171_context *context, float data1_2[9], float data2_3[6], float (*att_data_4)[6], float (*pos_data_5)[12] )
{
  Ssignal1CA9singleACsignal2CA6singleAS sig_0;
  float sig_1[15];
  float sig_2[6];
  float sig_3[12];
  int ix114;
  int ix127;
  int kx928;
  float sum929;
  int ix931;
  int ix988;
  int kx1184;
  float sum1185;
  int ix1187;
  int ix1211;

  ix114 = 0;
  while( ix114 < 9 )
  {
    sig_0.signal1[ (int)( ix114 ) ] = data1_2[ (int)( ix114 ) ];
    ++ix114;
  }

  ix127 = 0;
  while( ix127 < 6 )
  {
    sig_0.signal2[ (int)( ix127 ) ] = data2_3[ (int)( ix127 ) ];
    ++ix127;
  }

  sig_1[ (int)( 0 ) ] = sig_0.signal1[ (int)( 0 ) ];
  sig_1[ (int)( 1 ) ] = sig_0.signal1[ (int)( 1 ) ];
  sig_1[ (int)( 2 ) ] = sig_0.signal1[ (int)( 2 ) ];
  sig_1[ (int)( 3 ) ] = sig_0.signal1[ (int)( 3 ) ];
  sig_1[ (int)( 4 ) ] = sig_0.signal1[ (int)( 4 ) ];
  sig_1[ (int)( 5 ) ] = sig_0.signal1[ (int)( 5 ) ];
  sig_1[ (int)( 6 ) ] = sig_0.signal1[ (int)( 6 ) ];
  sig_1[ (int)( 7 ) ] = sig_0.signal1[ (int)( 7 ) ];
  sig_1[ (int)( 8 ) ] = sig_0.signal1[ (int)( 8 ) ];
  sig_1[ (int)( 9 ) ] = sig_0.signal2[ (int)( 0 ) ];
  sig_1[ (int)( 10 ) ] = sig_0.signal2[ (int)( 1 ) ];
  sig_1[ (int)( 11 ) ] = sig_0.signal2[ (int)( 2 ) ];
  sig_1[ (int)( 12 ) ] = sig_0.signal2[ (int)( 3 ) ];
  sig_1[ (int)( 13 ) ] = sig_0.signal2[ (int)( 4 ) ];
  sig_1[ (int)( 14 ) ] = sig_0.signal2[ (int)( 5 ) ];
  ix931 = 0;
  while( ix931 < 12 )
  {
    kx928 = 0;
    sum929 = 0;
    while( kx928 < 15 )
    {
      sum929 += ( *context ).Gain921[ (int)( ix931 ) ][ (int)( kx928 ) ] * sig_1[ (int)( kx928 ) ];
      ++kx928;
    }

    sig_3[ (int)( ix931 ) ] = sum929;
    ++ix931;
  }

  ix988 = 0;
  while( ix988 < 12 )
  {
    ( *pos_data_5 )[ (int)( ix988 ) ] = ( *context ).Gain983 * sig_3[ (int)( ix988 ) ];
    ++ix988;
  }

  ix1187 = 0;
  while( ix1187 < 6 )
  {
    kx1184 = 0;
    sum1185 = 0;
    while( kx1184 < 15 )
    {
      sum1185 += ( *context ).Gain1177[ (int)( ix1187 ) ][ (int)( kx1184 ) ] * sig_1[ (int)( kx1184 ) ];
      ++kx1184;
    }

    sig_2[ (int)( ix1187 ) ] = sum1185;
    ++ix1187;
  }

  ix1211 = 0;
  while( ix1211 < 6 )
  {
    ( *att_data_4 )[ (int)( ix1211 ) ] = ( *context ).Gain1206 * sig_2[ (int)( ix1211 ) ];
    ++ix1211;
  }

}

void DataHandler_100000171_init( DataHandler_100000171_context *context )
{
  ( *context ).Gain921[ (int)( 0 ) ][ (int)( 0 ) ] = 1;
  ( *context ).Gain921[ (int)( 0 ) ][ (int)( 1 ) ] = 1;
  ( *context ).Gain921[ (int)( 0 ) ][ (int)( 2 ) ] = 1;
  ( *context ).Gain921[ (int)( 0 ) ][ (int)( 3 ) ] = 1;
  ( *context ).Gain921[ (int)( 0 ) ][ (int)( 4 ) ] = 1;
  ( *context ).Gain921[ (int)( 0 ) ][ (int)( 5 ) ] = 1;
  ( *context ).Gain921[ (int)( 0 ) ][ (int)( 6 ) ] = 1;
  ( *context ).Gain921[ (int)( 0 ) ][ (int)( 7 ) ] = 1;
  ( *context ).Gain921[ (int)( 0 ) ][ (int)( 8 ) ] = 1;
  ( *context ).Gain921[ (int)( 0 ) ][ (int)( 9 ) ] = 1;
  ( *context ).Gain921[ (int)( 0 ) ][ (int)( 10 ) ] = 1;
  ( *context ).Gain921[ (int)( 0 ) ][ (int)( 11 ) ] = 1;
  ( *context ).Gain921[ (int)( 0 ) ][ (int)( 12 ) ] = 1;
  ( *context ).Gain921[ (int)( 0 ) ][ (int)( 13 ) ] = 1;
  ( *context ).Gain921[ (int)( 0 ) ][ (int)( 14 ) ] = 1;
  ( *context ).Gain921[ (int)( 1 ) ][ (int)( 0 ) ] = 1;
  ( *context ).Gain921[ (int)( 1 ) ][ (int)( 1 ) ] = 1;
  ( *context ).Gain921[ (int)( 1 ) ][ (int)( 2 ) ] = 1;
  ( *context ).Gain921[ (int)( 1 ) ][ (int)( 3 ) ] = 1;
  ( *context ).Gain921[ (int)( 1 ) ][ (int)( 4 ) ] = 1;
  ( *context ).Gain921[ (int)( 1 ) ][ (int)( 5 ) ] = 1;
  ( *context ).Gain921[ (int)( 1 ) ][ (int)( 6 ) ] = 1;
  ( *context ).Gain921[ (int)( 1 ) ][ (int)( 7 ) ] = 1;
  ( *context ).Gain921[ (int)( 1 ) ][ (int)( 8 ) ] = 1;
  ( *context ).Gain921[ (int)( 1 ) ][ (int)( 9 ) ] = 1;
  ( *context ).Gain921[ (int)( 1 ) ][ (int)( 10 ) ] = 1;
  ( *context ).Gain921[ (int)( 1 ) ][ (int)( 11 ) ] = 1;
  ( *context ).Gain921[ (int)( 1 ) ][ (int)( 12 ) ] = 1;
  ( *context ).Gain921[ (int)( 1 ) ][ (int)( 13 ) ] = 1;
  ( *context ).Gain921[ (int)( 1 ) ][ (int)( 14 ) ] = 1;
  ( *context ).Gain921[ (int)( 2 ) ][ (int)( 0 ) ] = 1;
  ( *context ).Gain921[ (int)( 2 ) ][ (int)( 1 ) ] = 1;
  ( *context ).Gain921[ (int)( 2 ) ][ (int)( 2 ) ] = 1;
  ( *context ).Gain921[ (int)( 2 ) ][ (int)( 3 ) ] = 1;
  ( *context ).Gain921[ (int)( 2 ) ][ (int)( 4 ) ] = 1;
  ( *context ).Gain921[ (int)( 2 ) ][ (int)( 5 ) ] = 1;
  ( *context ).Gain921[ (int)( 2 ) ][ (int)( 6 ) ] = 1;
  ( *context ).Gain921[ (int)( 2 ) ][ (int)( 7 ) ] = 1;
  ( *context ).Gain921[ (int)( 2 ) ][ (int)( 8 ) ] = 1;
  ( *context ).Gain921[ (int)( 2 ) ][ (int)( 9 ) ] = 1;
  ( *context ).Gain921[ (int)( 2 ) ][ (int)( 10 ) ] = 1;
  ( *context ).Gain921[ (int)( 2 ) ][ (int)( 11 ) ] = 1;
  ( *context ).Gain921[ (int)( 2 ) ][ (int)( 12 ) ] = 1;
  ( *context ).Gain921[ (int)( 2 ) ][ (int)( 13 ) ] = 1;
  ( *context ).Gain921[ (int)( 2 ) ][ (int)( 14 ) ] = 1;
  ( *context ).Gain921[ (int)( 3 ) ][ (int)( 0 ) ] = 1;
  ( *context ).Gain921[ (int)( 3 ) ][ (int)( 1 ) ] = 1;
  ( *context ).Gain921[ (int)( 3 ) ][ (int)( 2 ) ] = 1;
  ( *context ).Gain921[ (int)( 3 ) ][ (int)( 3 ) ] = 1;
  ( *context ).Gain921[ (int)( 3 ) ][ (int)( 4 ) ] = 1;
  ( *context ).Gain921[ (int)( 3 ) ][ (int)( 5 ) ] = 1;
  ( *context ).Gain921[ (int)( 3 ) ][ (int)( 6 ) ] = 1;
  ( *context ).Gain921[ (int)( 3 ) ][ (int)( 7 ) ] = 1;
  ( *context ).Gain921[ (int)( 3 ) ][ (int)( 8 ) ] = 1;
  ( *context ).Gain921[ (int)( 3 ) ][ (int)( 9 ) ] = 1;
  ( *context ).Gain921[ (int)( 3 ) ][ (int)( 10 ) ] = 1;
  ( *context ).Gain921[ (int)( 3 ) ][ (int)( 11 ) ] = 1;
  ( *context ).Gain921[ (int)( 3 ) ][ (int)( 12 ) ] = 1;
  ( *context ).Gain921[ (int)( 3 ) ][ (int)( 13 ) ] = 1;
  ( *context ).Gain921[ (int)( 3 ) ][ (int)( 14 ) ] = 1;
  ( *context ).Gain921[ (int)( 4 ) ][ (int)( 0 ) ] = 1;
  ( *context ).Gain921[ (int)( 4 ) ][ (int)( 1 ) ] = 1;
  ( *context ).Gain921[ (int)( 4 ) ][ (int)( 2 ) ] = 1;
  ( *context ).Gain921[ (int)( 4 ) ][ (int)( 3 ) ] = 1;
  ( *context ).Gain921[ (int)( 4 ) ][ (int)( 4 ) ] = 1;
  ( *context ).Gain921[ (int)( 4 ) ][ (int)( 5 ) ] = 1;
  ( *context ).Gain921[ (int)( 4 ) ][ (int)( 6 ) ] = 1;
  ( *context ).Gain921[ (int)( 4 ) ][ (int)( 7 ) ] = 1;
  ( *context ).Gain921[ (int)( 4 ) ][ (int)( 8 ) ] = 1;
  ( *context ).Gain921[ (int)( 4 ) ][ (int)( 9 ) ] = 1;
  ( *context ).Gain921[ (int)( 4 ) ][ (int)( 10 ) ] = 1;
  ( *context ).Gain921[ (int)( 4 ) ][ (int)( 11 ) ] = 1;
  ( *context ).Gain921[ (int)( 4 ) ][ (int)( 12 ) ] = 1;
  ( *context ).Gain921[ (int)( 4 ) ][ (int)( 13 ) ] = 1;
  ( *context ).Gain921[ (int)( 4 ) ][ (int)( 14 ) ] = 1;
  ( *context ).Gain921[ (int)( 5 ) ][ (int)( 0 ) ] = 1;
  ( *context ).Gain921[ (int)( 5 ) ][ (int)( 1 ) ] = 1;
  ( *context ).Gain921[ (int)( 5 ) ][ (int)( 2 ) ] = 1;
  ( *context ).Gain921[ (int)( 5 ) ][ (int)( 3 ) ] = 1;
  ( *context ).Gain921[ (int)( 5 ) ][ (int)( 4 ) ] = 1;
  ( *context ).Gain921[ (int)( 5 ) ][ (int)( 5 ) ] = 1;
  ( *context ).Gain921[ (int)( 5 ) ][ (int)( 6 ) ] = 1;
  ( *context ).Gain921[ (int)( 5 ) ][ (int)( 7 ) ] = 1;
  ( *context ).Gain921[ (int)( 5 ) ][ (int)( 8 ) ] = 1;
  ( *context ).Gain921[ (int)( 5 ) ][ (int)( 9 ) ] = 1;
  ( *context ).Gain921[ (int)( 5 ) ][ (int)( 10 ) ] = 1;
  ( *context ).Gain921[ (int)( 5 ) ][ (int)( 11 ) ] = 1;
  ( *context ).Gain921[ (int)( 5 ) ][ (int)( 12 ) ] = 1;
  ( *context ).Gain921[ (int)( 5 ) ][ (int)( 13 ) ] = 1;
  ( *context ).Gain921[ (int)( 5 ) ][ (int)( 14 ) ] = 1;
  ( *context ).Gain921[ (int)( 6 ) ][ (int)( 0 ) ] = 1;
  ( *context ).Gain921[ (int)( 6 ) ][ (int)( 1 ) ] = 1;
  ( *context ).Gain921[ (int)( 6 ) ][ (int)( 2 ) ] = 1;
  ( *context ).Gain921[ (int)( 6 ) ][ (int)( 3 ) ] = 1;
  ( *context ).Gain921[ (int)( 6 ) ][ (int)( 4 ) ] = 1;
  ( *context ).Gain921[ (int)( 6 ) ][ (int)( 5 ) ] = 1;
  ( *context ).Gain921[ (int)( 6 ) ][ (int)( 6 ) ] = 1;
  ( *context ).Gain921[ (int)( 6 ) ][ (int)( 7 ) ] = 1;
  ( *context ).Gain921[ (int)( 6 ) ][ (int)( 8 ) ] = 1;
  ( *context ).Gain921[ (int)( 6 ) ][ (int)( 9 ) ] = 1;
  ( *context ).Gain921[ (int)( 6 ) ][ (int)( 10 ) ] = 1;
  ( *context ).Gain921[ (int)( 6 ) ][ (int)( 11 ) ] = 1;
  ( *context ).Gain921[ (int)( 6 ) ][ (int)( 12 ) ] = 1;
  ( *context ).Gain921[ (int)( 6 ) ][ (int)( 13 ) ] = 1;
  ( *context ).Gain921[ (int)( 6 ) ][ (int)( 14 ) ] = 1;
  ( *context ).Gain921[ (int)( 7 ) ][ (int)( 0 ) ] = 1;
  ( *context ).Gain921[ (int)( 7 ) ][ (int)( 1 ) ] = 1;
  ( *context ).Gain921[ (int)( 7 ) ][ (int)( 2 ) ] = 1;
  ( *context ).Gain921[ (int)( 7 ) ][ (int)( 3 ) ] = 1;
  ( *context ).Gain921[ (int)( 7 ) ][ (int)( 4 ) ] = 1;
  ( *context ).Gain921[ (int)( 7 ) ][ (int)( 5 ) ] = 1;
  ( *context ).Gain921[ (int)( 7 ) ][ (int)( 6 ) ] = 1;
  ( *context ).Gain921[ (int)( 7 ) ][ (int)( 7 ) ] = 1;
  ( *context ).Gain921[ (int)( 7 ) ][ (int)( 8 ) ] = 1;
  ( *context ).Gain921[ (int)( 7 ) ][ (int)( 9 ) ] = 1;
  ( *context ).Gain921[ (int)( 7 ) ][ (int)( 10 ) ] = 1;
  ( *context ).Gain921[ (int)( 7 ) ][ (int)( 11 ) ] = 1;
  ( *context ).Gain921[ (int)( 7 ) ][ (int)( 12 ) ] = 1;
  ( *context ).Gain921[ (int)( 7 ) ][ (int)( 13 ) ] = 1;
  ( *context ).Gain921[ (int)( 7 ) ][ (int)( 14 ) ] = 1;
  ( *context ).Gain921[ (int)( 8 ) ][ (int)( 0 ) ] = 1;
  ( *context ).Gain921[ (int)( 8 ) ][ (int)( 1 ) ] = 1;
  ( *context ).Gain921[ (int)( 8 ) ][ (int)( 2 ) ] = 1;
  ( *context ).Gain921[ (int)( 8 ) ][ (int)( 3 ) ] = 1;
  ( *context ).Gain921[ (int)( 8 ) ][ (int)( 4 ) ] = 1;
  ( *context ).Gain921[ (int)( 8 ) ][ (int)( 5 ) ] = 1;
  ( *context ).Gain921[ (int)( 8 ) ][ (int)( 6 ) ] = 1;
  ( *context ).Gain921[ (int)( 8 ) ][ (int)( 7 ) ] = 1;
  ( *context ).Gain921[ (int)( 8 ) ][ (int)( 8 ) ] = 1;
  ( *context ).Gain921[ (int)( 8 ) ][ (int)( 9 ) ] = 1;
  ( *context ).Gain921[ (int)( 8 ) ][ (int)( 10 ) ] = 1;
  ( *context ).Gain921[ (int)( 8 ) ][ (int)( 11 ) ] = 1;
  ( *context ).Gain921[ (int)( 8 ) ][ (int)( 12 ) ] = 1;
  ( *context ).Gain921[ (int)( 8 ) ][ (int)( 13 ) ] = 1;
  ( *context ).Gain921[ (int)( 8 ) ][ (int)( 14 ) ] = 1;
  ( *context ).Gain921[ (int)( 9 ) ][ (int)( 0 ) ] = 1;
  ( *context ).Gain921[ (int)( 9 ) ][ (int)( 1 ) ] = 1;
  ( *context ).Gain921[ (int)( 9 ) ][ (int)( 2 ) ] = 1;
  ( *context ).Gain921[ (int)( 9 ) ][ (int)( 3 ) ] = 1;
  ( *context ).Gain921[ (int)( 9 ) ][ (int)( 4 ) ] = 1;
  ( *context ).Gain921[ (int)( 9 ) ][ (int)( 5 ) ] = 1;
  ( *context ).Gain921[ (int)( 9 ) ][ (int)( 6 ) ] = 1;
  ( *context ).Gain921[ (int)( 9 ) ][ (int)( 7 ) ] = 1;
  ( *context ).Gain921[ (int)( 9 ) ][ (int)( 8 ) ] = 1;
  ( *context ).Gain921[ (int)( 9 ) ][ (int)( 9 ) ] = 1;
  ( *context ).Gain921[ (int)( 9 ) ][ (int)( 10 ) ] = 1;
  ( *context ).Gain921[ (int)( 9 ) ][ (int)( 11 ) ] = 1;
  ( *context ).Gain921[ (int)( 9 ) ][ (int)( 12 ) ] = 1;
  ( *context ).Gain921[ (int)( 9 ) ][ (int)( 13 ) ] = 1;
  ( *context ).Gain921[ (int)( 9 ) ][ (int)( 14 ) ] = 1;
  ( *context ).Gain921[ (int)( 10 ) ][ (int)( 0 ) ] = 1;
  ( *context ).Gain921[ (int)( 10 ) ][ (int)( 1 ) ] = 1;
  ( *context ).Gain921[ (int)( 10 ) ][ (int)( 2 ) ] = 1;
  ( *context ).Gain921[ (int)( 10 ) ][ (int)( 3 ) ] = 1;
  ( *context ).Gain921[ (int)( 10 ) ][ (int)( 4 ) ] = 1;
  ( *context ).Gain921[ (int)( 10 ) ][ (int)( 5 ) ] = 1;
  ( *context ).Gain921[ (int)( 10 ) ][ (int)( 6 ) ] = 1;
  ( *context ).Gain921[ (int)( 10 ) ][ (int)( 7 ) ] = 1;
  ( *context ).Gain921[ (int)( 10 ) ][ (int)( 8 ) ] = 1;
  ( *context ).Gain921[ (int)( 10 ) ][ (int)( 9 ) ] = 1;
  ( *context ).Gain921[ (int)( 10 ) ][ (int)( 10 ) ] = 1;
  ( *context ).Gain921[ (int)( 10 ) ][ (int)( 11 ) ] = 1;
  ( *context ).Gain921[ (int)( 10 ) ][ (int)( 12 ) ] = 1;
  ( *context ).Gain921[ (int)( 10 ) ][ (int)( 13 ) ] = 1;
  ( *context ).Gain921[ (int)( 10 ) ][ (int)( 14 ) ] = 1;
  ( *context ).Gain921[ (int)( 11 ) ][ (int)( 0 ) ] = 1;
  ( *context ).Gain921[ (int)( 11 ) ][ (int)( 1 ) ] = 1;
  ( *context ).Gain921[ (int)( 11 ) ][ (int)( 2 ) ] = 1;
  ( *context ).Gain921[ (int)( 11 ) ][ (int)( 3 ) ] = 1;
  ( *context ).Gain921[ (int)( 11 ) ][ (int)( 4 ) ] = 1;
  ( *context ).Gain921[ (int)( 11 ) ][ (int)( 5 ) ] = 1;
  ( *context ).Gain921[ (int)( 11 ) ][ (int)( 6 ) ] = 1;
  ( *context ).Gain921[ (int)( 11 ) ][ (int)( 7 ) ] = 1;
  ( *context ).Gain921[ (int)( 11 ) ][ (int)( 8 ) ] = 1;
  ( *context ).Gain921[ (int)( 11 ) ][ (int)( 9 ) ] = 1;
  ( *context ).Gain921[ (int)( 11 ) ][ (int)( 10 ) ] = 1;
  ( *context ).Gain921[ (int)( 11 ) ][ (int)( 11 ) ] = 1;
  ( *context ).Gain921[ (int)( 11 ) ][ (int)( 12 ) ] = 1;
  ( *context ).Gain921[ (int)( 11 ) ][ (int)( 13 ) ] = 1;
  ( *context ).Gain921[ (int)( 11 ) ][ (int)( 14 ) ] = 1;
  ( *context ).Gain983 = 0.0666999999999999950000;
  ( *context ).Gain1177[ (int)( 0 ) ][ (int)( 0 ) ] = 1;
  ( *context ).Gain1177[ (int)( 0 ) ][ (int)( 1 ) ] = 1;
  ( *context ).Gain1177[ (int)( 0 ) ][ (int)( 2 ) ] = 1;
  ( *context ).Gain1177[ (int)( 0 ) ][ (int)( 3 ) ] = 1;
  ( *context ).Gain1177[ (int)( 0 ) ][ (int)( 4 ) ] = 1;
  ( *context ).Gain1177[ (int)( 0 ) ][ (int)( 5 ) ] = 1;
  ( *context ).Gain1177[ (int)( 0 ) ][ (int)( 6 ) ] = 1;
  ( *context ).Gain1177[ (int)( 0 ) ][ (int)( 7 ) ] = 1;
  ( *context ).Gain1177[ (int)( 0 ) ][ (int)( 8 ) ] = 1;
  ( *context ).Gain1177[ (int)( 0 ) ][ (int)( 9 ) ] = 1;
  ( *context ).Gain1177[ (int)( 0 ) ][ (int)( 10 ) ] = 1;
  ( *context ).Gain1177[ (int)( 0 ) ][ (int)( 11 ) ] = 1;
  ( *context ).Gain1177[ (int)( 0 ) ][ (int)( 12 ) ] = 1;
  ( *context ).Gain1177[ (int)( 0 ) ][ (int)( 13 ) ] = 1;
  ( *context ).Gain1177[ (int)( 0 ) ][ (int)( 14 ) ] = 1;
  ( *context ).Gain1177[ (int)( 1 ) ][ (int)( 0 ) ] = 1;
  ( *context ).Gain1177[ (int)( 1 ) ][ (int)( 1 ) ] = 1;
  ( *context ).Gain1177[ (int)( 1 ) ][ (int)( 2 ) ] = 1;
  ( *context ).Gain1177[ (int)( 1 ) ][ (int)( 3 ) ] = 1;
  ( *context ).Gain1177[ (int)( 1 ) ][ (int)( 4 ) ] = 1;
  ( *context ).Gain1177[ (int)( 1 ) ][ (int)( 5 ) ] = 1;
  ( *context ).Gain1177[ (int)( 1 ) ][ (int)( 6 ) ] = 1;
  ( *context ).Gain1177[ (int)( 1 ) ][ (int)( 7 ) ] = 1;
  ( *context ).Gain1177[ (int)( 1 ) ][ (int)( 8 ) ] = 1;
  ( *context ).Gain1177[ (int)( 1 ) ][ (int)( 9 ) ] = 1;
  ( *context ).Gain1177[ (int)( 1 ) ][ (int)( 10 ) ] = 1;
  ( *context ).Gain1177[ (int)( 1 ) ][ (int)( 11 ) ] = 1;
  ( *context ).Gain1177[ (int)( 1 ) ][ (int)( 12 ) ] = 1;
  ( *context ).Gain1177[ (int)( 1 ) ][ (int)( 13 ) ] = 1;
  ( *context ).Gain1177[ (int)( 1 ) ][ (int)( 14 ) ] = 1;
  ( *context ).Gain1177[ (int)( 2 ) ][ (int)( 0 ) ] = 1;
  ( *context ).Gain1177[ (int)( 2 ) ][ (int)( 1 ) ] = 1;
  ( *context ).Gain1177[ (int)( 2 ) ][ (int)( 2 ) ] = 1;
  ( *context ).Gain1177[ (int)( 2 ) ][ (int)( 3 ) ] = 1;
  ( *context ).Gain1177[ (int)( 2 ) ][ (int)( 4 ) ] = 1;
  ( *context ).Gain1177[ (int)( 2 ) ][ (int)( 5 ) ] = 1;
  ( *context ).Gain1177[ (int)( 2 ) ][ (int)( 6 ) ] = 1;
  ( *context ).Gain1177[ (int)( 2 ) ][ (int)( 7 ) ] = 1;
  ( *context ).Gain1177[ (int)( 2 ) ][ (int)( 8 ) ] = 1;
  ( *context ).Gain1177[ (int)( 2 ) ][ (int)( 9 ) ] = 1;
  ( *context ).Gain1177[ (int)( 2 ) ][ (int)( 10 ) ] = 1;
  ( *context ).Gain1177[ (int)( 2 ) ][ (int)( 11 ) ] = 1;
  ( *context ).Gain1177[ (int)( 2 ) ][ (int)( 12 ) ] = 1;
  ( *context ).Gain1177[ (int)( 2 ) ][ (int)( 13 ) ] = 1;
  ( *context ).Gain1177[ (int)( 2 ) ][ (int)( 14 ) ] = 1;
  ( *context ).Gain1177[ (int)( 3 ) ][ (int)( 0 ) ] = 1;
  ( *context ).Gain1177[ (int)( 3 ) ][ (int)( 1 ) ] = 1;
  ( *context ).Gain1177[ (int)( 3 ) ][ (int)( 2 ) ] = 1;
  ( *context ).Gain1177[ (int)( 3 ) ][ (int)( 3 ) ] = 1;
  ( *context ).Gain1177[ (int)( 3 ) ][ (int)( 4 ) ] = 1;
  ( *context ).Gain1177[ (int)( 3 ) ][ (int)( 5 ) ] = 1;
  ( *context ).Gain1177[ (int)( 3 ) ][ (int)( 6 ) ] = 1;
  ( *context ).Gain1177[ (int)( 3 ) ][ (int)( 7 ) ] = 1;
  ( *context ).Gain1177[ (int)( 3 ) ][ (int)( 8 ) ] = 1;
  ( *context ).Gain1177[ (int)( 3 ) ][ (int)( 9 ) ] = 1;
  ( *context ).Gain1177[ (int)( 3 ) ][ (int)( 10 ) ] = 1;
  ( *context ).Gain1177[ (int)( 3 ) ][ (int)( 11 ) ] = 1;
  ( *context ).Gain1177[ (int)( 3 ) ][ (int)( 12 ) ] = 1;
  ( *context ).Gain1177[ (int)( 3 ) ][ (int)( 13 ) ] = 1;
  ( *context ).Gain1177[ (int)( 3 ) ][ (int)( 14 ) ] = 1;
  ( *context ).Gain1177[ (int)( 4 ) ][ (int)( 0 ) ] = 1;
  ( *context ).Gain1177[ (int)( 4 ) ][ (int)( 1 ) ] = 1;
  ( *context ).Gain1177[ (int)( 4 ) ][ (int)( 2 ) ] = 1;
  ( *context ).Gain1177[ (int)( 4 ) ][ (int)( 3 ) ] = 1;
  ( *context ).Gain1177[ (int)( 4 ) ][ (int)( 4 ) ] = 1;
  ( *context ).Gain1177[ (int)( 4 ) ][ (int)( 5 ) ] = 1;
  ( *context ).Gain1177[ (int)( 4 ) ][ (int)( 6 ) ] = 1;
  ( *context ).Gain1177[ (int)( 4 ) ][ (int)( 7 ) ] = 1;
  ( *context ).Gain1177[ (int)( 4 ) ][ (int)( 8 ) ] = 1;
  ( *context ).Gain1177[ (int)( 4 ) ][ (int)( 9 ) ] = 1;
  ( *context ).Gain1177[ (int)( 4 ) ][ (int)( 10 ) ] = 1;
  ( *context ).Gain1177[ (int)( 4 ) ][ (int)( 11 ) ] = 1;
  ( *context ).Gain1177[ (int)( 4 ) ][ (int)( 12 ) ] = 1;
  ( *context ).Gain1177[ (int)( 4 ) ][ (int)( 13 ) ] = 1;
  ( *context ).Gain1177[ (int)( 4 ) ][ (int)( 14 ) ] = 1;
  ( *context ).Gain1177[ (int)( 5 ) ][ (int)( 0 ) ] = 1;
  ( *context ).Gain1177[ (int)( 5 ) ][ (int)( 1 ) ] = 1;
  ( *context ).Gain1177[ (int)( 5 ) ][ (int)( 2 ) ] = 1;
  ( *context ).Gain1177[ (int)( 5 ) ][ (int)( 3 ) ] = 1;
  ( *context ).Gain1177[ (int)( 5 ) ][ (int)( 4 ) ] = 1;
  ( *context ).Gain1177[ (int)( 5 ) ][ (int)( 5 ) ] = 1;
  ( *context ).Gain1177[ (int)( 5 ) ][ (int)( 6 ) ] = 1;
  ( *context ).Gain1177[ (int)( 5 ) ][ (int)( 7 ) ] = 1;
  ( *context ).Gain1177[ (int)( 5 ) ][ (int)( 8 ) ] = 1;
  ( *context ).Gain1177[ (int)( 5 ) ][ (int)( 9 ) ] = 1;
  ( *context ).Gain1177[ (int)( 5 ) ][ (int)( 10 ) ] = 1;
  ( *context ).Gain1177[ (int)( 5 ) ][ (int)( 11 ) ] = 1;
  ( *context ).Gain1177[ (int)( 5 ) ][ (int)( 12 ) ] = 1;
  ( *context ).Gain1177[ (int)( 5 ) ][ (int)( 13 ) ] = 1;
  ( *context ).Gain1177[ (int)( 5 ) ][ (int)( 14 ) ] = 1;
  ( *context ).Gain1206 = 0.0666999999999999950000;
}

