#ifndef _DataHandler_sl_H_
#define _DataHandler_sl_H_

#ifndef NO_MATH_H
#include <math.h>
#endif
#ifndef min
#define min(x,y) (((x)>(y)) ? (y) : (x))
#endif

#include "Ssignal1CsingleCsignal2CsingleS.h"

#ifndef single_GUARD
#define single_GUARD
typedef float single;
#endif
#ifndef A5singleA_GUARD
#define A5singleA_GUARD
typedef float A5singleA[5];
#endif
#ifndef A9singleA_GUARD
#define A9singleA_GUARD
typedef float A9singleA[9];
#endif
#ifndef A1A9singleAA_GUARD
#define A1A9singleAA_GUARD
typedef float A1A9singleAA[9];
#endif
#ifndef A1A5singleAA_GUARD
#define A1A5singleAA_GUARD
typedef float A1A5singleAA[5];
#endif

/* SIMPLIFIED PROGRAM CONTEXT */
typedef void *DataHandler_context;

void DataHandler_100000617_main( void *context, float ang_in_2, float pos_in_3, float *ang_out_4, float *pos_out_5 );
void DataHandler_100000617_init( void *context );



/* SIMPLIFIED PROGRAM FUNCTIONS */
void DataHandler_main( void *context, float ang_in_2, float pos_in_3, float *ang_out_4, float *pos_out_5 );
void DataHandler_init( void *context );
#endif
