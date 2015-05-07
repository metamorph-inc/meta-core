#include "gum_stix_sl.h"


void body_to_inertial_100000031_main( body_to_inertial_100000031_context *context, float euler_2[3], float Ab_3[3], float (*Xe_4)[3], float (*Ve_5)[3] );
void body_to_inertial_100000031_init( body_to_inertial_100000031_context *context );
void inertial_pos_controller1_100000036_main( inertial_pos_controller1_100000036_context *context, float Xe_set_2[3], float Xe_3[3], float Ve_4[3], float *u_T_5, float (*u_euler_6)[3] );
void inertial_pos_controller1_100000036_init( inertial_pos_controller1_100000036_context *context );
void sensor_fusion1_100000042_main( sensor_fusion1_100000042_context *context, float Xe_f_2[3], float Ve_f_3[3], float Xe_s_4[3], float Ve_s_5[3], float (*Xe_6)[3], float (*Ve_7)[3] );
void sensor_fusion1_100000042_init( sensor_fusion1_100000042_context *context );
void inertial_vel_and_pos_est_100000053_main( inertial_vel_and_pos_est_100000053_context *context, float Ab_2[3], float (*Vb_3)[3], float (*Xb_4)[3] );
void inertial_vel_and_pos_est_100000053_init( inertial_vel_and_pos_est_100000053_context *context );
void vel_est_100000150_main( vel_est_100000150_context *context, float Xe_set_2[3], float (*Ve_set_3)[3] );
void vel_est_100000150_init( vel_est_100000150_context *context );
void if_end_100000183_main( void *context, float Xe_s_2[3], float Xe_f_3[3], float dX_1p_4[3], float Xe_sh_1p_5[3], float (*Xe_sh_1_6)[3], float (*dX_1_7)[3] );
void if_end1_100000190_main( void *context, float Ve_s_2[3], float Ve_f_3[3], float dV_1p_4[3], float Ve_sh_1p_5[3], float (*Ve_sh_1_6)[3], float (*dV_1_7)[3] );
void DT_integrator_100000057_main( DT_integrator_100000057_context *context, float Ab_2[3], float (*Vb_3)[3] );
void DT_integrator_100000057_init( DT_integrator_100000057_context *context );
void DT_integrator1_100000060_main( DT_integrator1_100000060_context *context, float Vb_2[3], float (*Xb_3)[3] );
void DT_integrator1_100000060_init( DT_integrator1_100000060_context *context );
void If_Action_Subsystem_100000214_main( void *context, int action_1, float Xe_s_3[3], float Xe_f_4[3], float (*Xe_sh_1_5)[3], float (*dX_1_6)[3] );
void If_Action_Subsystem1_100000220_main( void *context, int action_1, float Xe_sh_1p_3[3], float dX_1p_4[3], float (*Xe_sh_1_5)[3], float (*dX_1_6)[3] );
void If_Action_Subsystem_100000243_main( void *context, int action_1, float Ve_s_3[3], float Ve_f_4[3], float (*Ve_sh_1_5)[3], float (*dV_1_6)[3] );
void If_Action_Subsystem1_100000249_main( void *context, int action_1, float Ve_sh_1p_3[3], float dV_1p_4[3], float (*Ve_sh_1_5)[3], float (*dV_1_6)[3] );


void gum_stix_100000002_main( gum_stix_100000002_context *context, float euler_2[3], float Ab_3[3], float Xe_4[3], float Ve_5[3], float Xe_set_6[3], float *u_T_7, float (*u_euler_8)[3] )
{
  float sig_0[3];
  float sig_1[3];
  float sig_2[3];
  float sig_3[3];

  body_to_inertial_100000031_main( &( *context ).body_to_inertial_100000031_class_member0, euler_2, Ab_3, &sig_0, &sig_1 );
  sensor_fusion1_100000042_main( &( *context ).sensor_fusion1_100000042_class_member1, sig_0, sig_1, Xe_4, Ve_5, &sig_2, &sig_3 );
  inertial_pos_controller1_100000036_main( &( *context ).inertial_pos_controller1_100000036_class_member2, Xe_set_6, sig_2, sig_3, &*u_T_7, &*u_euler_8 );
  // WARNING: SL_CodeGen(.\SLPrims.cpp, line:1542): Output Device (e.g. Display, Scope) block: do nothing.;
  // WARNING: SL_CodeGen(.\SLPrims.cpp, line:1542): Output Device (e.g. Display, Scope) block: do nothing.;
}

void gum_stix_100000002_init( gum_stix_100000002_context *context )
{
  body_to_inertial_100000031_init( &( *context ).body_to_inertial_100000031_class_member0 );
  inertial_pos_controller1_100000036_init( &( *context ).inertial_pos_controller1_100000036_class_member2 );
  sensor_fusion1_100000042_init( &( *context ).sensor_fusion1_100000042_class_member1 );
}

void body_to_inertial_100000031_main( body_to_inertial_100000031_context *context, float euler_2[3], float Ab_3[3], float (*Xe_4)[3], float (*Ve_5)[3] )
{
  float sig_0[3];
  float phi;
  float theta;
  float psi;
  float c_phi;
  float c_theta;
  float c_psi;
  float s_phi;
  float s_theta;
  float s_psi;
  float R[3][3];
  int kx565;
  float sum566;
  int ix568;

  phi = euler_2[ (int)( 0 ) ];
  theta = euler_2[ (int)( 1 ) ];
  psi = euler_2[ (int)( 2 ) ];
  c_phi = cos( phi );
  c_theta = cos( theta );
  c_psi = cos( psi );
  s_phi = sin( phi );
  s_theta = sin( theta );
  s_psi = sin( psi );
  R[ (int)( 0 ) ][ (int)( 0 ) ] = c_theta * c_psi;
  R[ (int)( 0 ) ][ (int)( 1 ) ] = c_theta * s_psi;
  R[ (int)( 0 ) ][ (int)( 2 ) ] = -s_theta;
  R[ (int)( 1 ) ][ (int)( 0 ) ] = s_phi * s_theta * c_psi - c_phi * s_psi;
  R[ (int)( 1 ) ][ (int)( 1 ) ] = s_phi * s_theta * s_psi + c_phi * c_psi;
  R[ (int)( 1 ) ][ (int)( 2 ) ] = c_theta * s_phi;
  R[ (int)( 2 ) ][ (int)( 0 ) ] = c_phi * s_theta * c_psi + s_phi * s_psi;
  R[ (int)( 2 ) ][ (int)( 1 ) ] = c_phi * s_theta * s_psi - s_phi * c_psi;
  R[ (int)( 2 ) ][ (int)( 2 ) ] = c_theta * c_phi;
  ix568 = 0;
  while( ix568 < 3 )
  {
    kx565 = 0;
    sum566 = 0;
    while( kx565 < 3 )
    {
      sum566 += transpose( R[ (int)( ix568 ) ][ (int)( kx565 ) ] ) * Ab_3[ (int)( kx565 ) ];
      ++kx565;
    }

    sig_0[ (int)( ix568 ) ] = sum566;
    ++ix568;
  }

  inertial_vel_and_pos_est_100000053_main( &( *context ).inertial_vel_and_pos_est_100000053_class_member0, sig_0, &*Ve_5, &*Xe_4 );
}

void body_to_inertial_100000031_init( body_to_inertial_100000031_context *context )
{
  inertial_vel_and_pos_est_100000053_init( &( *context ).inertial_vel_and_pos_est_100000053_class_member0 );
}

void inertial_pos_controller1_100000036_main( inertial_pos_controller1_100000036_context *context, float Xe_set_2[3], float Xe_3[3], float Ve_4[3], float *u_T_5, float (*u_euler_6)[3] )
{
  float sig_0;
  float sig_1;
  float sig_2[3];
  float sig_3[3];
  float sig_4[3];
  float sig_5[3];
  float sig_6[3];
  float sig_7;
  float sig_8;
  float sig_9;
  float sig_10;
  float sig_11[3];
  float sig_12[3];
  float sig_13;
  float sig_14[3];
  float sig_15;
  int ix808;
  int ix927;
  int ix1019;
  int ix1063;
  int ix1097;
  int ix1129;
  int ix95_0;

  sig_15 = ( *context ).Value586;
  vel_est_100000150_main( &( *context ).vel_est_100000150_class_member10, Xe_set_2, &sig_14 );
  ix808 = 0;
  while( ix808 < 3 )
  {
    sig_11[ (int)( ix808 ) ] = ( *context ).Value804[ (int)( ix808 ) ];
    ++ix808;
  }

  // WARNING: SL_CodeGen(.\SLPrims.cpp, line:1542): Output Device (e.g. Display, Scope) block: do nothing.;
  ix927 = 0;
  while( ix927 < 3 )
  {
    sig_6[ (int)( ix927 ) ] = Xe_set_2[ (int)( ix927 ) ] - Xe_3[ (int)( ix927 ) ];
    ++ix927;
  }

  ix1019 = 0;
  while( ix1019 < 3 )
  {
    sig_2[ (int)( ix1019 ) ] = ( *context ).Gain1014 * sig_6[ (int)( ix1019 ) ];
    ++ix1019;
  }

  ix1063 = 0;
  while( ix1063 < 3 )
  {
    sig_4[ (int)( ix1063 ) ] = sig_2[ (int)( ix1063 ) ] - Ve_4[ (int)( ix1063 ) ] + sig_14[ (int)( ix1063 ) ];
    ++ix1063;
  }

  ix1097 = 0;
  while( ix1097 < 3 )
  {
    sig_3[ (int)( ix1097 ) ] = ( *context ).Gain1092 * sig_4[ (int)( ix1097 ) ];
    ++ix1097;
  }

  ix1129 = 0;
  while( ix1129 < 3 )
  {
    sig_5[ (int)( ix1129 ) ] = sig_3[ (int)( ix1129 ) ] - sig_11[ (int)( ix1129 ) ];
    ++ix1129;
  }

  sig_7 = sig_5[ (int)( 0 ) ];
  sig_8 = sig_5[ (int)( 1 ) ];
  sig_9 = sig_5[ (int)( 2 ) ];
  sig_13 = ( *context ).Gain1146 * sig_8;
  if ( sig_9 < ( *context ).LowerLimit1151 )
  {
    sig_10 = ( *context ).LowerLimit1151;
  }
  else if ( sig_9 > ( *context ).UpperLimit1152 )
  {
    sig_10 = ( *context ).UpperLimit1152;
  }
  else if ( 1 )
  {
    sig_10 = sig_9;
  }

  *u_T_5 = ( *context ).Gain1171 * sig_10;
  sig_1 = sig_7 / sig_10;
  sig_0 = sig_13 / sig_10;
  sig_12[ (int)( 0 ) ] = sig_0;
  sig_12[ (int)( 1 ) ] = sig_1;
  sig_12[ (int)( 2 ) ] = sig_15;
  ix95_0 = 0;
  while( ix95_0 < 3 )
  {
    if ( sig_12[ (int)( ix95_0 ) ] < ( *context ).LowerLimit1200[ (int)( ix95_0 ) ] )
    {
      ( *u_euler_6 )[ (int)( ix95_0 ) ] = ( *context ).LowerLimit1200[ (int)( ix95_0 ) ];
    }
    else if ( sig_12[ (int)( ix95_0 ) ] > ( *context ).UpperLimit1207[ (int)( ix95_0 ) ] )
    {
      ( *u_euler_6 )[ (int)( ix95_0 ) ] = ( *context ).UpperLimit1207[ (int)( ix95_0 ) ];
    }
    else if ( 1 )
    {
      ( *u_euler_6 )[ (int)( ix95_0 ) ] = sig_12[ (int)( ix95_0 ) ];
    }

    ++ix95_0;
  }

}

void inertial_pos_controller1_100000036_init( inertial_pos_controller1_100000036_context *context )
{
  int ix1983;
  int ix1999;

  ( *context ).Gain1014 = 1.750000;
  ( *context ).Gain1092 = 1;
  ( *context ).LowerLimit1151 = -39.2400000000000020000;
  ( *context ).UpperLimit1152 = -2.45250000000000010000;
  ( *context ).Value804[ (int)( 0 ) ] = 0;
  ( *context ).Value804[ (int)( 1 ) ] = 0;
  ( *context ).Value804[ (int)( 2 ) ] = 17.8977399999999990000;
  ( *context ).Gain1146 = -1;
  ( *context ).Gain1171 = -1;
  ix1983 = 0;
  while( ix1983 < 3 )
  {
    ( *context ).LowerLimit1200[ (int)( ix1983 ) ] = -0.785398000000000040000 * 1;
    ++ix1983;
  }

  ix1999 = 0;
  while( ix1999 < 3 )
  {
    ( *context ).UpperLimit1207[ (int)( ix1999 ) ] = 0.785398000000000040000 * 1;
    ++ix1999;
  }

  ( *context ).Value586 = 0;
  vel_est_100000150_init( &( *context ).vel_est_100000150_class_member10 );
}

void sensor_fusion1_100000042_main( sensor_fusion1_100000042_context *context, float Xe_f_2[3], float Ve_f_3[3], float Xe_s_4[3], float Ve_s_5[3], float (*Xe_6)[3], float (*Ve_7)[3] )
{
  float sig_0[3];
  float sig_1[3];
  float sig_2[3];
  float sig_3[3];
  float sig_4[3];
  float sig_5[3];
  float sig_6[3];
  float sig_7[3];
  int ix398;
  int ix412;
  int ix426;
  int ix440;
  int ix686;
  int ix821;
  int ix941;
  int ix1033;
  int ix1081;
  int ix1113;

  ix398 = 0;
  while( ix398 < 3 )
  {
    sig_4[ (int)( ix398 ) ] = ( *context ).X0394[ (int)( ix398 ) ];
    ++ix398;
  }

  ix412 = 0;
  while( ix412 < 3 )
  {
    sig_5[ (int)( ix412 ) ] = ( *context ).X0408[ (int)( ix412 ) ];
    ++ix412;
  }

  ix426 = 0;
  while( ix426 < 3 )
  {
    sig_6[ (int)( ix426 ) ] = ( *context ).X0422[ (int)( ix426 ) ];
    ++ix426;
  }

  ix440 = 0;
  while( ix440 < 3 )
  {
    sig_7[ (int)( ix440 ) ] = ( *context ).X0436[ (int)( ix440 ) ];
    ++ix440;
  }

  if_end1_100000190_main( (void *)0, Ve_s_5, Ve_f_3, sig_4, sig_6, &sig_2, &sig_3 );
  ix686 = 0;
  while( ix686 < 3 )
  {
    ( *context ).X0422[ (int)( ix686 ) ] = sig_2[ (int)( ix686 ) ];
    ++ix686;
  }

  ix821 = 0;
  while( ix821 < 3 )
  {
    ( *context ).X0394[ (int)( ix821 ) ] = sig_3[ (int)( ix821 ) ];
    ++ix821;
  }

  if_end_100000183_main( (void *)0, Xe_s_4, Xe_f_2, sig_5, sig_7, &sig_0, &sig_1 );
  ix941 = 0;
  while( ix941 < 3 )
  {
    ( *context ).X0436[ (int)( ix941 ) ] = sig_0[ (int)( ix941 ) ];
    ++ix941;
  }

  ix1033 = 0;
  while( ix1033 < 3 )
  {
    ( *context ).X0408[ (int)( ix1033 ) ] = sig_1[ (int)( ix1033 ) ];
    ++ix1033;
  }

  ix1081 = 0;
  while( ix1081 < 3 )
  {
    ( *Ve_7 )[ (int)( ix1081 ) ] = Ve_f_3[ (int)( ix1081 ) ] - sig_3[ (int)( ix1081 ) ];
    ++ix1081;
  }

  ix1113 = 0;
  while( ix1113 < 3 )
  {
    ( *Xe_6 )[ (int)( ix1113 ) ] = Xe_f_2[ (int)( ix1113 ) ] - sig_1[ (int)( ix1113 ) ];
    ++ix1113;
  }

}

void sensor_fusion1_100000042_init( sensor_fusion1_100000042_context *context )
{
  int ix2011;
  int ix2021;
  int ix2031;
  int ix2041;

  ix2011 = 0;
  while( ix2011 < 3 )
  {
    ( *context ).X0394[ (int)( ix2011 ) ] = 0;
    ++ix2011;
  }

  ix2021 = 0;
  while( ix2021 < 3 )
  {
    ( *context ).X0408[ (int)( ix2021 ) ] = 0;
    ++ix2021;
  }

  ix2031 = 0;
  while( ix2031 < 3 )
  {
    ( *context ).X0422[ (int)( ix2031 ) ] = 0;
    ++ix2031;
  }

  ix2041 = 0;
  while( ix2041 < 3 )
  {
    ( *context ).X0436[ (int)( ix2041 ) ] = 0;
    ++ix2041;
  }

}

void inertial_vel_and_pos_est_100000053_main( inertial_vel_and_pos_est_100000053_context *context, float Ab_2[3], float (*Vb_3)[3], float (*Xb_4)[3] )
{
  DT_integrator_100000057_main( &( *context ).DT_integrator_100000057_class_member0, Ab_2, &*Vb_3 );
  DT_integrator1_100000060_main( &( *context ).DT_integrator1_100000060_class_member1, *Vb_3, &*Xb_4 );
}

void inertial_vel_and_pos_est_100000053_init( inertial_vel_and_pos_est_100000053_context *context )
{
  DT_integrator_100000057_init( &( *context ).DT_integrator_100000057_class_member0 );
  DT_integrator1_100000060_init( &( *context ).DT_integrator1_100000060_class_member1 );
}

void vel_est_100000150_main( vel_est_100000150_context *context, float Xe_set_2[3], float (*Ve_set_3)[3] )
{
  float sig_0[3];
  float sig_1[3];
  float sig_2[3];
  float sig_3[3];
  float sig_4[3];
  int ix1249;
  int ix1263;
  int ix1284;
  int ix1346;
  int ix1390;
  int ix1432;
  int ix1481;
  int ix1529;

  ix1249 = 0;
  while( ix1249 < 3 )
  {
    sig_0[ (int)( ix1249 ) ] = ( *context ).X01245[ (int)( ix1249 ) ];
    ++ix1249;
  }

  ix1263 = 0;
  while( ix1263 < 3 )
  {
    sig_1[ (int)( ix1263 ) ] = ( *context ).X01259[ (int)( ix1263 ) ];
    ++ix1263;
  }

  ix1284 = 0;
  while( ix1284 < 3 )
  {
    sig_4[ (int)( ix1284 ) ] = ( *context ).Gain1279 * Xe_set_2[ (int)( ix1284 ) ];
    ++ix1284;
  }

  ix1346 = 0;
  while( ix1346 < 3 )
  {
    sig_2[ (int)( ix1346 ) ] = ( *context ).Gain1341 * sig_1[ (int)( ix1346 ) ];
    ++ix1346;
  }

  ix1390 = 0;
  while( ix1390 < 3 )
  {
    sig_3[ (int)( ix1390 ) ] = ( *context ).Gain1385 * sig_0[ (int)( ix1390 ) ];
    ++ix1390;
  }

  ix1432 = 0;
  while( ix1432 < 3 )
  {
    ( *context ).X01245[ (int)( ix1432 ) ] = Xe_set_2[ (int)( ix1432 ) ];
    ++ix1432;
  }

  ix1481 = 0;
  while( ix1481 < 3 )
  {
    ( *Ve_set_3 )[ (int)( ix1481 ) ] = sig_3[ (int)( ix1481 ) ] + sig_4[ (int)( ix1481 ) ] - sig_2[ (int)( ix1481 ) ];
    ++ix1481;
  }

  ix1529 = 0;
  while( ix1529 < 3 )
  {
    ( *context ).X01259[ (int)( ix1529 ) ] = ( *Ve_set_3 )[ (int)( ix1529 ) ];
    ++ix1529;
  }

}

void vel_est_100000150_init( vel_est_100000150_context *context )
{
  int ix2213;
  int ix2223;

  ix2213 = 0;
  while( ix2213 < 3 )
  {
    ( *context ).X01245[ (int)( ix2213 ) ] = 0;
    ++ix2213;
  }

  ix2223 = 0;
  while( ix2223 < 3 )
  {
    ( *context ).X01259[ (int)( ix2223 ) ] = 0;
    ++ix2223;
  }

  ( *context ).Gain1341 = 0.0432140000000000020000;
  ( *context ).Gain1385 = -47.8393060000000010000;
  ( *context ).Gain1279 = 47.8393060000000010000;
}

void if_end_100000183_main( void *context, float Xe_s_2[3], float Xe_f_3[3], float dX_1p_4[3], float Xe_sh_1p_5[3], float (*Xe_sh_1_6)[3], float (*dX_1_7)[3] )
{
  int sig_0;
  int sig_1;
  float sig_2[3];
  float sig_3[3];
  float sig_4[3];
  float sig_5[3];
  int ifval1308;
  int ix1447;
  int ix1499;

  sig_0 = 0;
  sig_1 = 0;
  if ( Xe_s_2[ (int)( 1 ) ] != Xe_sh_1p_5[ (int)( 1 ) ] || Xe_s_2[ (int)( 2 ) ] != Xe_sh_1p_5[ (int)( 2 ) ] || Xe_s_2[ (int)( 3 ) ] != Xe_sh_1p_5[ (int)( 3 ) ] )
  {
    sig_0 = 1;
  }
  else if ( 1 )
  {
    sig_1 = 1;
  }

  ifval1308 = -1;
  if ( sig_0 != 0 )
  {
    ifval1308 = 1;
  }
  else if ( sig_1 != 0 )
  {
    ifval1308 = 2;
  }

  if ( sig_1 > 0 )
  {
    If_Action_Subsystem1_100000220_main( (void *)0, sig_1, Xe_sh_1p_5, dX_1p_4, &sig_4, &sig_5 );
  }

  if ( sig_0 > 0 )
  {
    If_Action_Subsystem_100000214_main( (void *)0, sig_0, Xe_s_2, Xe_f_3, &sig_2, &sig_3 );
  }

  ix1447 = 0;
  while( ix1447 < 3 )
  {
    ( *dX_1_7 )[ (int)( ix1447 ) ] = sig_3[ (int)( ix1447 ) ] + sig_5[ (int)( ix1447 ) ];
    ++ix1447;
  }

  ix1499 = 0;
  while( ix1499 < 3 )
  {
    ( *Xe_sh_1_6 )[ (int)( ix1499 ) ] = sig_2[ (int)( ix1499 ) ] + sig_4[ (int)( ix1499 ) ];
    ++ix1499;
  }

}

void if_end1_100000190_main( void *context, float Ve_s_2[3], float Ve_f_3[3], float dV_1p_4[3], float Ve_sh_1p_5[3], float (*Ve_sh_1_6)[3], float (*dV_1_7)[3] )
{
  int sig_0;
  int sig_1;
  float sig_2[3];
  float sig_3[3];
  float sig_4[3];
  float sig_5[3];
  int ifval1328;
  int ix1463;
  int ix1515;

  sig_0 = 0;
  sig_1 = 0;
  if ( Ve_s_2[ (int)( 1 ) ] != Ve_sh_1p_5[ (int)( 1 ) ] || Ve_s_2[ (int)( 2 ) ] != Ve_sh_1p_5[ (int)( 2 ) ] || Ve_s_2[ (int)( 3 ) ] != Ve_sh_1p_5[ (int)( 3 ) ] )
  {
    sig_0 = 1;
  }
  else if ( 1 )
  {
    sig_1 = 1;
  }

  ifval1328 = -1;
  if ( sig_0 != 0 )
  {
    ifval1328 = 1;
  }
  else if ( sig_1 != 0 )
  {
    ifval1328 = 2;
  }

  if ( sig_1 > 0 )
  {
    If_Action_Subsystem1_100000249_main( (void *)0, sig_1, Ve_sh_1p_5, dV_1p_4, &sig_4, &sig_5 );
  }

  if ( sig_0 > 0 )
  {
    If_Action_Subsystem_100000243_main( (void *)0, sig_0, Ve_s_2, Ve_f_3, &sig_2, &sig_3 );
  }

  ix1463 = 0;
  while( ix1463 < 3 )
  {
    ( *dV_1_7 )[ (int)( ix1463 ) ] = sig_3[ (int)( ix1463 ) ] + sig_5[ (int)( ix1463 ) ];
    ++ix1463;
  }

  ix1515 = 0;
  while( ix1515 < 3 )
  {
    ( *Ve_sh_1_6 )[ (int)( ix1515 ) ] = sig_2[ (int)( ix1515 ) ] + sig_4[ (int)( ix1515 ) ];
    ++ix1515;
  }

}

void DT_integrator_100000057_main( DT_integrator_100000057_context *context, float Ab_2[3], float (*Vb_3)[3] )
{
  float sig_0[3];
  float sig_1[3];
  float sig_2[3];
  float sig_3[3];
  int ix1615;
  int ix1629;
  int ix1670;
  int ix1730;
  int ix1762;
  int ix1794;
  int ix1824;

  ix1615 = 0;
  while( ix1615 < 3 )
  {
    sig_2[ (int)( ix1615 ) ] = ( *context ).X01611[ (int)( ix1615 ) ];
    ++ix1615;
  }

  ix1629 = 0;
  while( ix1629 < 3 )
  {
    sig_3[ (int)( ix1629 ) ] = ( *context ).X01625[ (int)( ix1629 ) ];
    ++ix1629;
  }

  ix1670 = 0;
  while( ix1670 < 3 )
  {
    ( *context ).X01611[ (int)( ix1670 ) ] = Ab_2[ (int)( ix1670 ) ];
    ++ix1670;
  }

  ix1730 = 0;
  while( ix1730 < 3 )
  {
    sig_0[ (int)( ix1730 ) ] = sig_2[ (int)( ix1730 ) ] + Ab_2[ (int)( ix1730 ) ];
    ++ix1730;
  }

  ix1762 = 0;
  while( ix1762 < 3 )
  {
    sig_1[ (int)( ix1762 ) ] = ( *context ).Gain1757 * sig_0[ (int)( ix1762 ) ];
    ++ix1762;
  }

  ix1794 = 0;
  while( ix1794 < 3 )
  {
    ( *Vb_3 )[ (int)( ix1794 ) ] = sig_1[ (int)( ix1794 ) ] + sig_3[ (int)( ix1794 ) ];
    ++ix1794;
  }

  ix1824 = 0;
  while( ix1824 < 3 )
  {
    ( *context ).X01625[ (int)( ix1824 ) ] = ( *Vb_3 )[ (int)( ix1824 ) ];
    ++ix1824;
  }

}

void DT_integrator_100000057_init( DT_integrator_100000057_context *context )
{
  int ix2269;
  int ix2279;

  ( *context ).Gain1757 = 0.010000;
  ix2269 = 0;
  while( ix2269 < 3 )
  {
    ( *context ).X01611[ (int)( ix2269 ) ] = 0;
    ++ix2269;
  }

  ix2279 = 0;
  while( ix2279 < 3 )
  {
    ( *context ).X01625[ (int)( ix2279 ) ] = 0;
    ++ix2279;
  }

}

void DT_integrator1_100000060_main( DT_integrator1_100000060_context *context, float Vb_2[3], float (*Xb_3)[3] )
{
  float sig_0[3];
  float sig_1[3];
  float sig_2[3];
  float sig_3[3];
  int ix1643;
  int ix1657;
  int ix1683;
  int ix1746;
  int ix1778;
  int ix1810;
  int ix1837;

  ix1643 = 0;
  while( ix1643 < 3 )
  {
    sig_2[ (int)( ix1643 ) ] = ( *context ).X01639[ (int)( ix1643 ) ];
    ++ix1643;
  }

  ix1657 = 0;
  while( ix1657 < 3 )
  {
    sig_3[ (int)( ix1657 ) ] = ( *context ).X01653[ (int)( ix1657 ) ];
    ++ix1657;
  }

  ix1683 = 0;
  while( ix1683 < 3 )
  {
    ( *context ).X01639[ (int)( ix1683 ) ] = Vb_2[ (int)( ix1683 ) ];
    ++ix1683;
  }

  ix1746 = 0;
  while( ix1746 < 3 )
  {
    sig_0[ (int)( ix1746 ) ] = sig_2[ (int)( ix1746 ) ] + Vb_2[ (int)( ix1746 ) ];
    ++ix1746;
  }

  ix1778 = 0;
  while( ix1778 < 3 )
  {
    sig_1[ (int)( ix1778 ) ] = ( *context ).Gain1773 * sig_0[ (int)( ix1778 ) ];
    ++ix1778;
  }

  ix1810 = 0;
  while( ix1810 < 3 )
  {
    ( *Xb_3 )[ (int)( ix1810 ) ] = sig_1[ (int)( ix1810 ) ] + sig_3[ (int)( ix1810 ) ];
    ++ix1810;
  }

  ix1837 = 0;
  while( ix1837 < 3 )
  {
    ( *context ).X01653[ (int)( ix1837 ) ] = ( *Xb_3 )[ (int)( ix1837 ) ];
    ++ix1837;
  }

}

void DT_integrator1_100000060_init( DT_integrator1_100000060_context *context )
{
  int ix2291;
  int ix2301;

  ( *context ).Gain1773 = 0.010000;
  ix2291 = 0;
  while( ix2291 < 3 )
  {
    ( *context ).X01639[ (int)( ix2291 ) ] = 0;
    ++ix2291;
  }

  ix2301 = 0;
  while( ix2301 < 3 )
  {
    ( *context ).X01653[ (int)( ix2301 ) ] = 0;
    ++ix2301;
  }

}

void If_Action_Subsystem_100000214_main( void *context, int action_1, float Xe_s_3[3], float Xe_f_4[3], float (*Xe_sh_1_5)[3], float (*dX_1_6)[3] )
{
  int ix1542;
  int ix1698;

  ix1542 = 0;
  while( ix1542 < 3 )
  {
    ( *Xe_sh_1_5 )[ (int)( ix1542 ) ] = Xe_s_3[ (int)( ix1542 ) ];
    ++ix1542;
  }

  ix1698 = 0;
  while( ix1698 < 3 )
  {
    ( *dX_1_6 )[ (int)( ix1698 ) ] = -Xe_s_3[ (int)( ix1698 ) ] + Xe_f_4[ (int)( ix1698 ) ];
    ++ix1698;
  }

}

void If_Action_Subsystem1_100000220_main( void *context, int action_1, float Xe_sh_1p_3[3], float dX_1p_4[3], float (*Xe_sh_1_5)[3], float (*dX_1_6)[3] )
{
  int ix1554;
  int ix1566;

  ix1554 = 0;
  while( ix1554 < 3 )
  {
    ( *Xe_sh_1_5 )[ (int)( ix1554 ) ] = Xe_sh_1p_3[ (int)( ix1554 ) ];
    ++ix1554;
  }

  ix1566 = 0;
  while( ix1566 < 3 )
  {
    ( *dX_1_6 )[ (int)( ix1566 ) ] = dX_1p_4[ (int)( ix1566 ) ];
    ++ix1566;
  }

}

void If_Action_Subsystem_100000243_main( void *context, int action_1, float Ve_s_3[3], float Ve_f_4[3], float (*Ve_sh_1_5)[3], float (*dV_1_6)[3] )
{
  int ix1578;
  int ix1714;

  ix1578 = 0;
  while( ix1578 < 3 )
  {
    ( *Ve_sh_1_5 )[ (int)( ix1578 ) ] = Ve_s_3[ (int)( ix1578 ) ];
    ++ix1578;
  }

  ix1714 = 0;
  while( ix1714 < 3 )
  {
    ( *dV_1_6 )[ (int)( ix1714 ) ] = -Ve_s_3[ (int)( ix1714 ) ] + Ve_f_4[ (int)( ix1714 ) ];
    ++ix1714;
  }

}

void If_Action_Subsystem1_100000249_main( void *context, int action_1, float Ve_sh_1p_3[3], float dV_1p_4[3], float (*Ve_sh_1_5)[3], float (*dV_1_6)[3] )
{
  int ix1590;
  int ix1602;

  ix1590 = 0;
  while( ix1590 < 3 )
  {
    ( *Ve_sh_1_5 )[ (int)( ix1590 ) ] = Ve_sh_1p_3[ (int)( ix1590 ) ];
    ++ix1590;
  }

  ix1602 = 0;
  while( ix1602 < 3 )
  {
    ( *dV_1_6 )[ (int)( ix1602 ) ] = dV_1p_4[ (int)( ix1602 ) ];
    ++ix1602;
  }

}

