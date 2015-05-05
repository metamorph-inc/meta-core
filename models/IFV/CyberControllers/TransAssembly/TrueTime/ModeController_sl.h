#ifndef _ModeController_sl_H_
#define _ModeController_sl_H_

#ifndef NO_MATH_H
#include <math.h>
#endif
#ifndef min
#define min(x,y) (((x)>(y)) ? (y) : (x))
#endif

#ifndef sign
#define sign(x) ( ((x) > 0.0) ? (1.0)  : (((x) < 0.0 ) ? (-1.0) : (0.0)) )
#endif


#ifndef unsigned_char_GUARD
#define unsigned_char_GUARD
typedef char unsigned_char;
#endif

#ifndef ModeController_100000151_context_GUARD
#define ModeController_100000151_context_GUARD
typedef struct {
  float Value771;
  float Value776;
} ModeController_100000151_context;
#endif

/* SIMPLIFIED PROGRAM CONTEXT */
typedef ModeController_100000151_context ModeController_context;

void ModeController_100000151_main( ModeController_100000151_context *context, float Velocity_2, float BatteryVoltage_3, float VelocityDemand_4, float *Mode_5 );
void ModeController_100000151_init( ModeController_100000151_context *context );



/* SIMPLIFIED PROGRAM FUNCTIONS */
void ModeController_main( ModeController_100000151_context *context, float Velocity_2, float BatteryVoltage_3, float VelocityDemand_4, float *Mode_5 );
void ModeController_init( ModeController_100000151_context *context );
#endif
