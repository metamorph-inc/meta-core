#ifndef _gum_stix_sl_H_
#define _gum_stix_sl_H_

#include <math.h>
#define min(x,y) (x>y ? y : x)


#ifndef DT_integrator_100000057_context_GUARD
#define DT_integrator_100000057_context_GUARD
typedef struct {
  float X01611[3];
  float X01625[3];
  float Gain1757;
} DT_integrator_100000057_context;
#endif

#ifndef DT_integrator1_100000060_context_GUARD
#define DT_integrator1_100000060_context_GUARD
typedef struct {
  float X01639[3];
  float X01653[3];
  float Gain1773;
} DT_integrator1_100000060_context;
#endif

#ifndef inertial_vel_and_pos_est_100000053_context_GUARD
#define inertial_vel_and_pos_est_100000053_context_GUARD
typedef struct {
  DT_integrator_100000057_context DT_integrator_100000057_class_member0;
  DT_integrator1_100000060_context DT_integrator1_100000060_class_member1;
} inertial_vel_and_pos_est_100000053_context;
#endif

#ifndef body_to_inertial_100000031_context_GUARD
#define body_to_inertial_100000031_context_GUARD
typedef struct {
  inertial_vel_and_pos_est_100000053_context inertial_vel_and_pos_est_100000053_class_member0;
} body_to_inertial_100000031_context;
#endif

#ifndef vel_est_100000150_context_GUARD
#define vel_est_100000150_context_GUARD
typedef struct {
  float X01245[3];
  float X01259[3];
  float Gain1279;
  float Gain1341;
  float Gain1385;
} vel_est_100000150_context;
#endif

#ifndef inertial_pos_controller1_100000036_context_GUARD
#define inertial_pos_controller1_100000036_context_GUARD
typedef struct {
  float Value586;
  float Value804[3];
  float Gain1014;
  float Gain1092;
  float Gain1146;
  float LowerLimit1151;
  float UpperLimit1152;
  float Gain1171;
  float LowerLimit1200[3];
  float UpperLimit1207[3];
  vel_est_100000150_context vel_est_100000150_class_member10;
} inertial_pos_controller1_100000036_context;
#endif

#ifndef sensor_fusion1_100000042_context_GUARD
#define sensor_fusion1_100000042_context_GUARD
typedef struct {
  float X0394[3];
  float X0408[3];
  float X0422[3];
  float X0436[3];
} sensor_fusion1_100000042_context;
#endif

#ifndef gum_stix_100000002_context_GUARD
#define gum_stix_100000002_context_GUARD
typedef struct {
  body_to_inertial_100000031_context body_to_inertial_100000031_class_member0;
  sensor_fusion1_100000042_context sensor_fusion1_100000042_class_member1;
  inertial_pos_controller1_100000036_context inertial_pos_controller1_100000036_class_member2;
} gum_stix_100000002_context;
#endif

void gum_stix_100000002_main( gum_stix_100000002_context *context, float euler_2[3], float Ab_3[3], float Xe_4[3], float Ve_5[3], float Xe_set_6[3], float *u_T_7, float (*u_euler_8)[3] );
void gum_stix_100000002_init( gum_stix_100000002_context *context );


#endif
