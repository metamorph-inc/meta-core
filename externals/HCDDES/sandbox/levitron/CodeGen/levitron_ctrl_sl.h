#ifndef _levitron_ctrl_sl_H_
#define _levitron_ctrl_sl_H_

#include <math.h>
#define min(x,y) (x>y ? y : x)


#ifndef PD_Controller_100000004_context_GUARD
#define PD_Controller_100000004_context_GUARD
typedef struct {
  float X043;
  int Gain52;
  int Gain57;
  int Gain68;
  int LowerLimit73;
  int UpperLimit74;
} PD_Controller_100000004_context;
#endif

#ifndef levitron_ctrl_100000001_context_GUARD
#define levitron_ctrl_100000001_context_GUARD
typedef struct {
  PD_Controller_100000004_context PD_Controller_100000004_class_member1;
} levitron_ctrl_100000001_context;
#endif

void levitron_ctrl_100000001_main( levitron_ctrl_100000001_context *context, float Position_2, float *Force_3 );
void levitron_ctrl_100000001_init( levitron_ctrl_100000001_context *context );


#endif
