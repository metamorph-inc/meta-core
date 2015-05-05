#include "CruiseController_sl.h"



/* SIMPLIFIED PROGRAM FUNCTIONS */
void CruiseController_main( CruiseController_100000017_context *context, float V_demand_2, float velocity_3, float *Throttle_4 ) {
  CruiseController_100000017_main( context, V_demand_2, velocity_3, Throttle_4 );
}

void CruiseController_init( CruiseController_100000017_context *context ) {
  CruiseController_100000017_init( context );
}

void DPID_100000036_main( DPID_100000036_context *context, float E_z__2, float *U_z__3 );
void DPID_100000036_init( DPID_100000036_context *context );
void DT_Int_100000046_main( DT_Int_100000046_context *context, float U_z__2, float *Y_z__3 );
void DT_Int_100000046_init( DT_Int_100000046_context *context );
void D_Term_100000066_main( D_Term_100000066_context *context, float E_s__2, float *U_D_s__3 );
void D_Term_100000066_init( D_Term_100000066_context *context );
void DT_Int_100000077_main( DT_Int_100000077_context *context, float U_z__2, float *Y_z__3 );
void DT_Int_100000077_init( DT_Int_100000077_context *context );


void CruiseController_100000017_main( CruiseController_100000017_context *context, float V_demand_2, float velocity_3, float *Throttle_4 )
{
  float sig_0;
  float sig_1;
  float sig_2;
  float sig_3;
  float sig_4;
  float sig_5;
  float sig_6;

  sig_4 = ( *context ).Value65;
  sig_3 = sig_0 * sig_4;
  sig_6 = sig_3 - sig_1;
  DPID_100000036_main( &( *context ).DPID_100000036_class_member3, sig_6, &sig_5 );
  if ( sig_5 > ( *context ).UpperLimit83 )
  {
    sig_2 = ( *context ).UpperLimit83;
  }
  else if ( sig_5 < ( *context ).LowerLimit81 )
  {
    sig_2 = ( *context ).LowerLimit81;
  }
  else if ( 1 )
  {
    sig_2 = sig_5;
  }

}

void CruiseController_100000017_init( CruiseController_100000017_context *context )
{
  ( *context ).Value65 = 1.020000;
  ( *context ).LowerLimit81 = 0;
  ( *context ).UpperLimit83 = 1;
  DPID_100000036_init( &( *context ).DPID_100000036_class_member3 );
}

void DPID_100000036_main( DPID_100000036_context *context, float E_z__2, float *U_z__3 )
{
  float sig_0;
  float sig_1;
  float sig_2;
  float sig_3;
  float sig_4;

  D_Term_100000066_main( &( *context ).D_Term_100000066_class_member4, E_z__2, &sig_4 );
  sig_2 = E_z__2 * ( *context ).Gain108;
  DT_Int_100000046_main( &( *context ).DT_Int_100000046_class_member5, sig_2, &sig_3 );
  sig_1 = E_z__2 * ( *context ).Gain119;
  if ( sig_1 > ( *context ).UpperLimit125 )
  {
    sig_0 = ( *context ).UpperLimit125;
  }
  else if ( sig_1 < ( *context ).LowerLimit124 )
  {
    sig_0 = ( *context ).LowerLimit124;
  }
  else if ( 1 )
  {
    sig_0 = sig_1;
  }

  *U_z__3 = sig_0 + sig_4 + sig_3;
}

void DPID_100000036_init( DPID_100000036_context *context )
{
  ( *context ).Gain108 = 0;
  ( *context ).Gain119 = 7;
  ( *context ).LowerLimit124 = -3000;
  ( *context ).UpperLimit125 = 3000;
  DT_Int_100000046_init( &( *context ).DT_Int_100000046_class_member5 );
  D_Term_100000066_init( &( *context ).D_Term_100000066_class_member4 );
}

void DT_Int_100000046_main( DT_Int_100000046_context *context, float U_z__2, float *Y_z__3 )
{
  float sig_0;
  float sig_1;
  float sig_2;
  float sig_3;

  sig_0 = ( *context ).X0148;
  sig_1 = ( *context ).X0152;
  sig_2 = U_z__2 * ( *context ).Gain161;
  ( *context ).X0152 = sig_2;
  sig_3 = sig_1 + sig_2 + sig_0;
  if ( sig_3 > ( *context ).UpperLimit190 )
  {
    *Y_z__3 = ( *context ).UpperLimit190;
  }
  else if ( sig_3 < ( *context ).LowerLimit189 )
  {
    *Y_z__3 = ( *context ).LowerLimit189;
  }
  else if ( 1 )
  {
    *Y_z__3 = sig_3;
  }

  ( *context ).X0148 = *Y_z__3;
}

void DT_Int_100000046_init( DT_Int_100000046_context *context )
{
  ( *context ).X0148 = 0;
  ( *context ).X0152 = 0;
  ( *context ).Gain161 = 0.0500000000000000030000;
  ( *context ).LowerLimit189 = -1500;
  ( *context ).UpperLimit190 = 1500;
}

void D_Term_100000066_main( D_Term_100000066_context *context, float E_s__2, float *U_D_s__3 )
{
  float sig_0;
  float sig_1;
  float sig_2;
  float sig_3;

  sig_0 = ( *context ).X0156;
  sig_2 = E_s__2 * ( *context ).Gain166;
  *U_D_s__3 = sig_0 * ( *context ).Gain174;
  DT_Int_100000077_main( &( *context ).DT_Int_100000077_class_member3, *U_D_s__3, &sig_3 );
  sig_1 = sig_2 - sig_3;
  ( *context ).X0156 = sig_1;
}

void D_Term_100000066_init( D_Term_100000066_context *context )
{
  ( *context ).X0156 = 0;
  ( *context ).Gain166 = 0;
  ( *context ).Gain174 = 31.4159265358979310000;
  DT_Int_100000077_init( &( *context ).DT_Int_100000077_class_member3 );
}

void DT_Int_100000077_main( DT_Int_100000077_context *context, float U_z__2, float *Y_z__3 )
{
  float sig_0;
  float sig_1;
  float sig_2;
  float sig_3;

  sig_0 = ( *context ).X0218;
  sig_1 = ( *context ).X0222;
  sig_2 = U_z__2 * ( *context ).Gain226;
  ( *context ).X0222 = sig_2;
  sig_3 = sig_1 + sig_2 + sig_0;
  if ( sig_3 > ( *context ).UpperLimit239 )
  {
    *Y_z__3 = ( *context ).UpperLimit239;
  }
  else if ( sig_3 < ( *context ).LowerLimit238 )
  {
    *Y_z__3 = ( *context ).LowerLimit238;
  }
  else if ( 1 )
  {
    *Y_z__3 = sig_3;
  }

  ( *context ).X0218 = *Y_z__3;
}

void DT_Int_100000077_init( DT_Int_100000077_context *context )
{
  ( *context ).X0218 = 0;
  ( *context ).X0222 = 0;
  ( *context ).Gain226 = 0.0500000000000000030000;
  ( *context ).LowerLimit238 = -95.49296585513720000;
  ( *context ).UpperLimit239 = 95.49296585513720000;
}

