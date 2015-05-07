#ifndef _robo_stix_sl_H_
#define _robo_stix_sl_H_

#include <math.h>
#define min(x,y) (x>y ? y : x)


#ifndef euler_controller_100000267_context_GUARD
#define euler_controller_100000267_context_GUARD
typedef struct {
  float Gain696;
  float Gain893;
  float LowerLimit955[3];
  float UpperLimit960[3];
} euler_controller_100000267_context;
#endif

#ifndef lead_motor_100000272_context_GUARD
#define lead_motor_100000272_context_GUARD
typedef struct {
  float X0450[4];
  float X0464[4];
  float Gain620;
  float Gain712;
  float Gain847;
} lead_motor_100000272_context;
#endif

#ifndef sensor_convert_100000275_context_GUARD
#define sensor_convert_100000275_context_GUARD
typedef struct {
  float Gain636;
  float Gain728;
  float Gain863;
} sensor_convert_100000275_context;
#endif

#ifndef u_T_u_torque_motor_thrust_100000285_context_GUARD
#define u_T_u_torque_motor_thrust_100000285_context_GUARD
typedef struct {
  float Gain776[4][4];
} u_T_u_torque_motor_thrust_100000285_context;
#endif

#ifndef robo_stix_100000010_context_GUARD
#define robo_stix_100000010_context_GUARD
typedef struct {
  float LowerLimit349[4];
  float UpperLimit356[4];
  sensor_convert_100000275_context sensor_convert_100000275_class_member2;
  euler_controller_100000267_context euler_controller_100000267_class_member3;
  u_T_u_torque_motor_thrust_100000285_context u_T_u_torque_motor_thrust_100000285_class_member4;
  lead_motor_100000272_context lead_motor_100000272_class_member5;
} robo_stix_100000010_context;
#endif

void robo_stix_100000010_main( robo_stix_100000010_context *context, float euler_s_2[3], float Ab_s_3[3], float omega_s_4[3], float Xe_s_5[3], float Ve_s_6[3], float euler_set_7[3], float u_T_8, float (*cont_motor_thrust_9)[4], float (*euler_10)[3], float (*Ab_11)[3], float (*Xe_12)[3], float (*Ve_13)[3] );
void robo_stix_100000010_init( robo_stix_100000010_context *context );


#endif
