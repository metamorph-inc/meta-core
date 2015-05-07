#ifndef _levitron_ctrl_sl_H_
#define _levitron_ctrl_sl_H_

#include <math.h>
#define min(x,y) (x>y ? y : x)


#ifndef vel_est_100000029_context_GUARD
#define vel_est_100000029_context_GUARD
typedef struct {
  float X097;
  float X0101;
  double Gain105;
  double Gain110;
  double Gain115;
} vel_est_100000029_context;
#endif

#ifndef PD_Controller_100000006_context_GUARD
#define PD_Controller_100000006_context_GUARD
typedef struct {
  double Gain64;
  double Gain72;
  int LowerLimit77;
  int UpperLimit78;
  vel_est_100000029_context vel_est_100000029_class_member4;
} PD_Controller_100000006_context;
#endif

#ifndef levitron_ctrl_100000001_context_GUARD
#define levitron_ctrl_100000001_context_GUARD
typedef struct {
  PD_Controller_100000006_context PD_Controller_100000006_class_member1;
} levitron_ctrl_100000001_context;
#endif

void levitron_ctrl_100000001_main( levitron_ctrl_100000001_context *context, float Position_2, float *Force_3 );
void levitron_ctrl_100000001_init( levitron_ctrl_100000001_context *context );


#endif
