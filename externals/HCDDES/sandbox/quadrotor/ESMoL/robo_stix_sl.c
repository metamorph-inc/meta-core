#include "robo_stix_sl.h"


void euler_controller_100000267_main( euler_controller_100000267_context *context, float euler_set_2[3], float euler_3[3], float omega_4[3], float (*u_torque_5)[3] );
void euler_controller_100000267_init( euler_controller_100000267_context *context );
void lead_motor_100000272_main( lead_motor_100000272_context *context, float thrust_in_2[4], float (*thrust_out_3)[4] );
void lead_motor_100000272_init( lead_motor_100000272_context *context );
void sensor_convert_100000275_main( sensor_convert_100000275_context *context, float euler_s_2[3], float Ab_s_3[3], float omega_s_4[3], float (*euler_5)[3], float (*Ab_6)[3], float (*omega_7)[3] );
void sensor_convert_100000275_init( sensor_convert_100000275_context *context );
void u_T_u_torque_motor_thrust_100000285_main( u_T_u_torque_motor_thrust_100000285_context *context, float u_torque_2[3], float u_T_3, float (*cont_motor_thrust_4)[4] );
void u_T_u_torque_motor_thrust_100000285_init( u_T_u_torque_motor_thrust_100000285_context *context );


void robo_stix_100000010_main( robo_stix_100000010_context *context, float euler_s_2[3], float Ab_s_3[3], float omega_s_4[3], float Xe_s_5[3], float Ve_s_6[3], float euler_set_7[3], float u_T_8, float (*cont_motor_thrust_9)[4], float (*euler_10)[3], float (*Ab_11)[3], float (*Xe_12)[3], float (*Ve_13)[3] )
{
  float sig_0[3];
  float sig_1[4];
  float sig_2[3];
  float sig_3[4];
  int ix246;
  int ix258;
  int ix48_0;

  ix246 = 0;
  while( ix246 < 3 )
  {
    ( *Xe_12 )[ (int)( ix246 ) ] = Xe_s_5[ (int)( ix246 ) ];
    ++ix246;
  }

  ix258 = 0;
  while( ix258 < 3 )
  {
    ( *Ve_13 )[ (int)( ix258 ) ] = Ve_s_6[ (int)( ix258 ) ];
    ++ix258;
  }

  sensor_convert_100000275_main( &( *context ).sensor_convert_100000275_class_member2, euler_s_2, Ab_s_3, omega_s_4, &*euler_10, &*Ab_11, &sig_2 );
  euler_controller_100000267_main( &( *context ).euler_controller_100000267_class_member3, euler_set_7, *euler_10, sig_2, &sig_0 );
  u_T_u_torque_motor_thrust_100000285_main( &( *context ).u_T_u_torque_motor_thrust_100000285_class_member4, sig_0, u_T_8, &sig_3 );
  lead_motor_100000272_main( &( *context ).lead_motor_100000272_class_member5, sig_3, &sig_1 );
  ix48_0 = 0;
  while( ix48_0 < 4 )
  {
    if ( sig_1[ (int)( ix48_0 ) ] < ( *context ).LowerLimit349[ (int)( ix48_0 ) ] )
    {
      ( *cont_motor_thrust_9 )[ (int)( ix48_0 ) ] = ( *context ).LowerLimit349[ (int)( ix48_0 ) ];
    }
    else if ( sig_1[ (int)( ix48_0 ) ] > ( *context ).UpperLimit356[ (int)( ix48_0 ) ] )
    {
      ( *cont_motor_thrust_9 )[ (int)( ix48_0 ) ] = ( *context ).UpperLimit356[ (int)( ix48_0 ) ];
    }
    else if ( 1 )
    {
      ( *cont_motor_thrust_9 )[ (int)( ix48_0 ) ] = sig_1[ (int)( ix48_0 ) ];
    }

    ++ix48_0;
  }

}

void robo_stix_100000010_init( robo_stix_100000010_context *context )
{
  int ix1916;
  int ix1932;

  ix1916 = 0;
  while( ix1916 < 4 )
  {
    ( *context ).LowerLimit349[ (int)( ix1916 ) ] = 0 * 1;
    ++ix1916;
  }

  ix1932 = 0;
  while( ix1932 < 4 )
  {
    ( *context ).UpperLimit356[ (int)( ix1932 ) ] = 10 * 1;
    ++ix1932;
  }

  euler_controller_100000267_init( &( *context ).euler_controller_100000267_class_member3 );
  lead_motor_100000272_init( &( *context ).lead_motor_100000272_class_member5 );
  sensor_convert_100000275_init( &( *context ).sensor_convert_100000275_class_member2 );
  u_T_u_torque_motor_thrust_100000285_init( &( *context ).u_T_u_torque_motor_thrust_100000285_class_member4 );
}

void euler_controller_100000267_main( euler_controller_100000267_context *context, float euler_set_2[3], float euler_3[3], float omega_4[3], float (*u_torque_5)[3] )
{
  float sig_0[3];
  float sig_1[3];
  float sig_2[3];
  float sig_3[3];
  int ix609;
  int ix701;
  int ix836;
  int ix898;
  int ix98_0;

  ix609 = 0;
  while( ix609 < 3 )
  {
    sig_3[ (int)( ix609 ) ] = euler_set_2[ (int)( ix609 ) ] - euler_3[ (int)( ix609 ) ];
    ++ix609;
  }

  ix701 = 0;
  while( ix701 < 3 )
  {
    sig_0[ (int)( ix701 ) ] = ( *context ).Gain696 * sig_3[ (int)( ix701 ) ];
    ++ix701;
  }

  ix836 = 0;
  while( ix836 < 3 )
  {
    sig_2[ (int)( ix836 ) ] = sig_0[ (int)( ix836 ) ] - omega_4[ (int)( ix836 ) ];
    ++ix836;
  }

  ix898 = 0;
  while( ix898 < 3 )
  {
    sig_1[ (int)( ix898 ) ] = ( *context ).Gain893 * sig_2[ (int)( ix898 ) ];
    ++ix898;
  }

  ix98_0 = 0;
  while( ix98_0 < 3 )
  {
    if ( sig_1[ (int)( ix98_0 ) ] < ( *context ).LowerLimit955[ (int)( ix98_0 ) ] )
    {
      ( *u_torque_5 )[ (int)( ix98_0 ) ] = ( *context ).LowerLimit955[ (int)( ix98_0 ) ];
    }
    else if ( sig_1[ (int)( ix98_0 ) ] > ( *context ).UpperLimit960[ (int)( ix98_0 ) ] )
    {
      ( *u_torque_5 )[ (int)( ix98_0 ) ] = ( *context ).UpperLimit960[ (int)( ix98_0 ) ];
    }
    else if ( 1 )
    {
      ( *u_torque_5 )[ (int)( ix98_0 ) ] = sig_1[ (int)( ix98_0 ) ];
    }

    ++ix98_0;
  }

}

void euler_controller_100000267_init( euler_controller_100000267_context *context )
{
  int ix2058;
  int ix2071;

  ( *context ).Gain696 = 11.5384620000000010000;
  ( *context ).Gain893 = 0.50000;
  ix2058 = 0;
  while( ix2058 < 3 )
  {
    ( *context ).LowerLimit955[ (int)( ix2058 ) ] = -2.98703999999999990000 * 1;
    ++ix2058;
  }

  ix2071 = 0;
  while( ix2071 < 3 )
  {
    ( *context ).UpperLimit960[ (int)( ix2071 ) ] = 2.98703999999999990000 * 1;
    ++ix2071;
  }

}

void lead_motor_100000272_main( lead_motor_100000272_context *context, float thrust_in_2[4], float (*thrust_out_3)[4] )
{
  float sig_0[4];
  float sig_1[4];
  float sig_2[4];
  float sig_3[4];
  float sig_4[4];
  int ix454;
  int ix468;
  int ix625;
  int ix717;
  int ix852;
  int ix912;
  int ix1001;
  int ix1046;

  ix454 = 0;
  while( ix454 < 4 )
  {
    sig_0[ (int)( ix454 ) ] = ( *context ).X0450[ (int)( ix454 ) ];
    ++ix454;
  }

  ix468 = 0;
  while( ix468 < 4 )
  {
    sig_1[ (int)( ix468 ) ] = ( *context ).X0464[ (int)( ix468 ) ];
    ++ix468;
  }

  ix625 = 0;
  while( ix625 < 4 )
  {
    sig_4[ (int)( ix625 ) ] = ( *context ).Gain620 * thrust_in_2[ (int)( ix625 ) ];
    ++ix625;
  }

  ix717 = 0;
  while( ix717 < 4 )
  {
    sig_2[ (int)( ix717 ) ] = ( *context ).Gain712 * sig_1[ (int)( ix717 ) ];
    ++ix717;
  }

  ix852 = 0;
  while( ix852 < 4 )
  {
    sig_3[ (int)( ix852 ) ] = ( *context ).Gain847 * sig_0[ (int)( ix852 ) ];
    ++ix852;
  }

  ix912 = 0;
  while( ix912 < 4 )
  {
    ( *context ).X0450[ (int)( ix912 ) ] = thrust_in_2[ (int)( ix912 ) ];
    ++ix912;
  }

  ix1001 = 0;
  while( ix1001 < 4 )
  {
    ( *thrust_out_3 )[ (int)( ix1001 ) ] = sig_3[ (int)( ix1001 ) ] + sig_4[ (int)( ix1001 ) ] - sig_2[ (int)( ix1001 ) ];
    ++ix1001;
  }

  ix1046 = 0;
  while( ix1046 < 4 )
  {
    ( *context ).X0464[ (int)( ix1046 ) ] = ( *thrust_out_3 )[ (int)( ix1046 ) ];
    ++ix1046;
  }

}

void lead_motor_100000272_init( lead_motor_100000272_context *context )
{
  int ix2081;
  int ix2091;

  ix2081 = 0;
  while( ix2081 < 4 )
  {
    ( *context ).X0450[ (int)( ix2081 ) ] = 0;
    ++ix2081;
  }

  ix2091 = 0;
  while( ix2091 < 4 )
  {
    ( *context ).X0464[ (int)( ix2091 ) ] = 0;
    ++ix2091;
  }

  ( *context ).Gain712 = 0.0432140000000000020000;
  ( *context ).Gain847 = -4.52259000000000010000;
  ( *context ).Gain620 = 5.47937600000000020000;
}

void sensor_convert_100000275_main( sensor_convert_100000275_context *context, float euler_s_2[3], float Ab_s_3[3], float omega_s_4[3], float (*euler_5)[3], float (*Ab_6)[3], float (*omega_7)[3] )
{
  int ix641;
  int ix733;
  int ix868;

  ix641 = 0;
  while( ix641 < 3 )
  {
    ( *euler_5 )[ (int)( ix641 ) ] = ( *context ).Gain636 * euler_s_2[ (int)( ix641 ) ];
    ++ix641;
  }

  ix733 = 0;
  while( ix733 < 3 )
  {
    ( *omega_7 )[ (int)( ix733 ) ] = ( *context ).Gain728 * omega_s_4[ (int)( ix733 ) ];
    ++ix733;
  }

  ix868 = 0;
  while( ix868 < 3 )
  {
    ( *Ab_6 )[ (int)( ix868 ) ] = ( *context ).Gain863 * Ab_s_3[ (int)( ix868 ) ];
    ++ix868;
  }

}

void sensor_convert_100000275_init( sensor_convert_100000275_context *context )
{
  ( *context ).Gain863 = 0.00209600000000000020000;
  ( *context ).Gain728 = 0.000259000000000000010000;
  ( *context ).Gain636 = 9.60000000000000020000e-005;
}

void u_T_u_torque_motor_thrust_100000285_main( u_T_u_torque_motor_thrust_100000285_context *context, float u_torque_2[3], float u_T_3, float (*cont_motor_thrust_4)[4] )
{
  float sig_0[4];
  int ix657;
  int kx782;
  float sum783;
  int ix785;

  ix657 = 0;
  while( ix657 < 3 )
  {
    sig_0[ (int)( ix657 ) ] = u_torque_2[ (int)( ix657 ) ];
    ++ix657;
  }

  sig_0[ (int)( 3 ) ] = u_T_3;
  ix785 = 0;
  while( ix785 < 4 )
  {
    kx782 = 0;
    sum783 = 0;
    while( kx782 < 4 )
    {
      sum783 += ( *context ).Gain776[ (int)( ix785 ) ][ (int)( kx782 ) ] * sig_0[ (int)( kx782 ) ];
      ++kx782;
    }

    ( *cont_motor_thrust_4 )[ (int)( ix785 ) ] = sum783;
    ++ix785;
  }

}

void u_T_u_torque_motor_thrust_100000285_init( u_T_u_torque_motor_thrust_100000285_context *context )
{
  ( *context ).Gain776[ (int)( 0 ) ][ (int)( 0 ) ] = 0;
  ( *context ).Gain776[ (int)( 0 ) ][ (int)( 1 ) ] = 1.64040000000000010000;
  ( *context ).Gain776[ (int)( 0 ) ][ (int)( 2 ) ] = -0.531900000000000040000;
  ( *context ).Gain776[ (int)( 0 ) ][ (int)( 3 ) ] = 0.250000;
  ( *context ).Gain776[ (int)( 1 ) ][ (int)( 0 ) ] = -1.64040000000000010000;
  ( *context ).Gain776[ (int)( 1 ) ][ (int)( 1 ) ] = 0;
  ( *context ).Gain776[ (int)( 1 ) ][ (int)( 2 ) ] = 0.531900000000000040000;
  ( *context ).Gain776[ (int)( 1 ) ][ (int)( 3 ) ] = 0.250000;
  ( *context ).Gain776[ (int)( 2 ) ][ (int)( 0 ) ] = 0;
  ( *context ).Gain776[ (int)( 2 ) ][ (int)( 1 ) ] = -1.64040000000000010000;
  ( *context ).Gain776[ (int)( 2 ) ][ (int)( 2 ) ] = -0.531900000000000040000;
  ( *context ).Gain776[ (int)( 2 ) ][ (int)( 3 ) ] = 0.250000;
  ( *context ).Gain776[ (int)( 3 ) ][ (int)( 0 ) ] = 1.64040000000000010000;
  ( *context ).Gain776[ (int)( 3 ) ][ (int)( 1 ) ] = 0;
  ( *context ).Gain776[ (int)( 3 ) ][ (int)( 2 ) ] = 0.531900000000000040000;
  ( *context ).Gain776[ (int)( 3 ) ][ (int)( 3 ) ] = 0.250000;
}

