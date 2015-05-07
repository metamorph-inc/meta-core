#ifndef _OuterLoop_sl_H_
#define _OuterLoop_sl_H_

#include <math.h>
#define min(x,y) (x>y ? y : x)

#include "Ssignal1CA4singleACsignal2CA12singleAS.h"

#ifndef MsingleC1C1M_GUARD
#define MsingleC1C1M_GUARD
typedef float MsingleC1C1M;
#endif
#ifndef MsingleC9C1M_GUARD
#define MsingleC9C1M_GUARD
typedef float MsingleC9C1M[9];
#endif
#ifndef MsingleC6C1M_GUARD
#define MsingleC6C1M_GUARD
typedef float MsingleC6C1M[6];
#endif
#ifndef MsingleC4C1M_GUARD
#define MsingleC4C1M_GUARD
typedef float MsingleC4C1M[4];
#endif
#ifndef MsingleC12C1M_GUARD
#define MsingleC12C1M_GUARD
typedef float MsingleC12C1M[12];
#endif
#ifndef MsingleC15C1M_GUARD
#define MsingleC15C1M_GUARD
typedef float MsingleC15C1M[15];
#endif
#ifndef MsingleC1C15M_GUARD
#define MsingleC1C15M_GUARD
typedef float MsingleC1C15M[15];
#endif
#ifndef MsingleC10C1M_GUARD
#define MsingleC10C1M_GUARD
typedef float MsingleC10C1M[10];
#endif
#ifndef MsingleC1C10M_GUARD
#define MsingleC1C10M_GUARD
typedef float MsingleC1C10M[10];
#endif
#ifndef MsingleC16C1M_GUARD
#define MsingleC16C1M_GUARD
typedef float MsingleC16C1M[16];
#endif
#ifndef MsingleC1C16M_GUARD
#define MsingleC1C16M_GUARD
typedef float MsingleC1C16M[16];
#endif
#ifndef MintC1C1M_GUARD
#define MintC1C1M_GUARD
typedef int MintC1C1M;
#endif
#ifndef MintC2C1M_GUARD
#define MintC2C1M_GUARD
typedef int MintC2C1M[2];
#endif
#ifndef MintC1C2M_GUARD
#define MintC1C2M_GUARD
typedef int MintC1C2M[2];
#endif
#ifndef MintC3C1M_GUARD
#define MintC3C1M_GUARD
typedef int MintC3C1M[3];
#endif
#ifndef MintC1C3M_GUARD
#define MintC1C3M_GUARD
typedef int MintC1C3M[3];
#endif
#ifndef MintC4C1M_GUARD
#define MintC4C1M_GUARD
typedef int MintC4C1M[4];
#endif
#ifndef MintC1C4M_GUARD
#define MintC1C4M_GUARD
typedef int MintC1C4M[4];
#endif
#ifndef MintC5C1M_GUARD
#define MintC5C1M_GUARD
typedef int MintC5C1M[5];
#endif
#ifndef MintC1C5M_GUARD
#define MintC1C5M_GUARD
typedef int MintC1C5M[5];
#endif
#ifndef MintC6C1M_GUARD
#define MintC6C1M_GUARD
typedef int MintC6C1M[6];
#endif
#ifndef MintC1C6M_GUARD
#define MintC1C6M_GUARD
typedef int MintC1C6M[6];
#endif
#ifndef MintC7C1M_GUARD
#define MintC7C1M_GUARD
typedef int MintC7C1M[7];
#endif
#ifndef MintC1C7M_GUARD
#define MintC1C7M_GUARD
typedef int MintC1C7M[7];
#endif
#ifndef MintC8C1M_GUARD
#define MintC8C1M_GUARD
typedef int MintC8C1M[8];
#endif
#ifndef MintC1C8M_GUARD
#define MintC1C8M_GUARD
typedef int MintC1C8M[8];
#endif
#ifndef MintC9C1M_GUARD
#define MintC9C1M_GUARD
typedef int MintC9C1M[9];
#endif
#ifndef MintC1C9M_GUARD
#define MintC1C9M_GUARD
typedef int MintC1C9M[9];
#endif
#ifndef MintC10C1M_GUARD
#define MintC10C1M_GUARD
typedef int MintC10C1M[10];
#endif
#ifndef MintC1C10M_GUARD
#define MintC1C10M_GUARD
typedef int MintC1C10M[10];
#endif
#ifndef MintC2C10M_GUARD
#define MintC2C10M_GUARD
typedef int MintC2C10M[2][10];
#endif
#ifndef MintC3C10M_GUARD
#define MintC3C10M_GUARD
typedef int MintC3C10M[3][10];
#endif
#ifndef MintC4C10M_GUARD
#define MintC4C10M_GUARD
typedef int MintC4C10M[4][10];
#endif
#ifndef MintC11C1M_GUARD
#define MintC11C1M_GUARD
typedef int MintC11C1M[11];
#endif
#ifndef MintC1C11M_GUARD
#define MintC1C11M_GUARD
typedef int MintC1C11M[11];
#endif
#ifndef MintC12C1M_GUARD
#define MintC12C1M_GUARD
typedef int MintC12C1M[12];
#endif
#ifndef MintC1C12M_GUARD
#define MintC1C12M_GUARD
typedef int MintC1C12M[12];
#endif
#ifndef MintC13C1M_GUARD
#define MintC13C1M_GUARD
typedef int MintC13C1M[13];
#endif
#ifndef MintC1C13M_GUARD
#define MintC1C13M_GUARD
typedef int MintC1C13M[13];
#endif
#ifndef MintC14C1M_GUARD
#define MintC14C1M_GUARD
typedef int MintC14C1M[14];
#endif
#ifndef MintC1C14M_GUARD
#define MintC1C14M_GUARD
typedef int MintC1C14M[14];
#endif
#ifndef MintC15C1M_GUARD
#define MintC15C1M_GUARD
typedef int MintC15C1M[15];
#endif
#ifndef MintC1C15M_GUARD
#define MintC1C15M_GUARD
typedef int MintC1C15M[15];
#endif
#ifndef MintC16C1M_GUARD
#define MintC16C1M_GUARD
typedef int MintC16C1M[16];
#endif
#ifndef MintC1C16M_GUARD
#define MintC1C16M_GUARD
typedef int MintC1C16M[16];
#endif
#ifndef MintC2C16M_GUARD
#define MintC2C16M_GUARD
typedef int MintC2C16M[2][16];
#endif
#ifndef MintC3C16M_GUARD
#define MintC3C16M_GUARD
typedef int MintC3C16M[3][16];
#endif
#ifndef MintC4C16M_GUARD
#define MintC4C16M_GUARD
typedef int MintC4C16M[4][16];
#endif
#ifndef MintC2C15M_GUARD
#define MintC2C15M_GUARD
typedef int MintC2C15M[2][15];
#endif
#ifndef MintC3C15M_GUARD
#define MintC3C15M_GUARD
typedef int MintC3C15M[3][15];
#endif
#ifndef MintC4C15M_GUARD
#define MintC4C15M_GUARD
typedef int MintC4C15M[4][15];
#endif
#ifndef MintC5C15M_GUARD
#define MintC5C15M_GUARD
typedef int MintC5C15M[5][15];
#endif
#ifndef MintC6C15M_GUARD
#define MintC6C15M_GUARD
typedef int MintC6C15M[6][15];
#endif
#ifndef MintC7C15M_GUARD
#define MintC7C15M_GUARD
typedef int MintC7C15M[7][15];
#endif
#ifndef MintC8C15M_GUARD
#define MintC8C15M_GUARD
typedef int MintC8C15M[8][15];
#endif
#ifndef MintC9C15M_GUARD
#define MintC9C15M_GUARD
typedef int MintC9C15M[9][15];
#endif
#ifndef MintC10C15M_GUARD
#define MintC10C15M_GUARD
typedef int MintC10C15M[10][15];
#endif
#ifndef MintC11C15M_GUARD
#define MintC11C15M_GUARD
typedef int MintC11C15M[11][15];
#endif
#ifndef MintC12C15M_GUARD
#define MintC12C15M_GUARD
typedef int MintC12C15M[12][15];
#endif
#ifndef MdoubleC1C1M_GUARD
#define MdoubleC1C1M_GUARD
typedef double MdoubleC1C1M;
#endif
#ifndef MdoubleC4C1M_GUARD
#define MdoubleC4C1M_GUARD
typedef double MdoubleC4C1M[4];
#endif
#ifndef MdoubleC12C1M_GUARD
#define MdoubleC12C1M_GUARD
typedef double MdoubleC12C1M[12];
#endif
#ifndef MdoubleC6C1M_GUARD
#define MdoubleC6C1M_GUARD
typedef double MdoubleC6C1M[6];
#endif

#ifndef OuterLoop_100000163_context_GUARD
#define OuterLoop_100000163_context_GUARD
typedef struct {
  int Gain534[4][16];
  double Gain967;
} OuterLoop_100000163_context;
#endif

/* SIMPLIFIED PROGRAM CONTEXT */
typedef OuterLoop_100000163_class OuterLoop_class;

void OuterLoop_100000163_main( OuterLoop_100000163_context *context, float pos_ref_2[4], float pos_data_3[12], float (*att_ref_4)[4] );
void OuterLoop_100000163_init( OuterLoop_100000163_context *context );

/* SIMPLIFIED PROGRAM FUNCTIONS */
void OuterLoop_main( OuterLoop_100000163_context *context, float pos_ref_2[4], float pos_data_3[12], float (*att_ref_4)[4] ) {
  OuterLoop_100000163_main( context, pos_ref_2, pos_data_3, att_ref_4 );
}

void OuterLoop_init( OuterLoop_100000163_context *context ) {
  OuterLoop_100000163_init( context );
}



#endif
