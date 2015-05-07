#ifndef _InnerLoop_sl_H_
#define _InnerLoop_sl_H_

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

#ifndef InnerLoop_100000008_context_GUARD
#define InnerLoop_100000008_context_GUARD
typedef struct {
  int Gain19;
} InnerLoop_100000008_context;
#endif

void InnerLoop_100000008_main( InnerLoop_100000008_context *context, float angle_2, float (*Torque_3) );
void InnerLoop_100000008_init( InnerLoop_100000008_context *context );


#endif
