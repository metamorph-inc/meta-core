#ifndef _OuterLoop_sl_H_
#define _OuterLoop_sl_H_

#include <math.h>
#define min(x,y) (x>y ? y : x)


#ifndef MsingleC1C1M_GUARD
#define MsingleC1C1M_GUARD
typedef float MsingleC1C1M;
#endif
#ifndef MintC1C1M_GUARD
#define MintC1C1M_GUARD
typedef int MintC1C1M;
#endif
#ifndef MdoubleC1C1M_GUARD
#define MdoubleC1C1M_GUARD
typedef double MdoubleC1C1M;
#endif

#ifndef OuterLoop_100000011_context_GUARD
#define OuterLoop_100000011_context_GUARD
typedef struct {
  double Gain27;
} OuterLoop_100000011_context;
#endif

void OuterLoop_100000011_main( OuterLoop_100000011_context *context, float pos_2, float (*ang_ref_3) );
void OuterLoop_100000011_init( OuterLoop_100000011_context *context );


#endif
