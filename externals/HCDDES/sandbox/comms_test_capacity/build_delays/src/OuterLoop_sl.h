#ifndef _OuterLoop_sl_H_
#define _OuterLoop_sl_H_

#ifndef NO_MATH_H
#include <math.h>
#endif
#ifndef min
#define min(x,y) (((x)>(y)) ? (y) : (x))
#endif

#ifndef sign
#define sign(x) ( ((x) > 0.0) ? (1.0)  : (((x) < 0.0 ) ? (-1.0) : (0.0)) )
#endif

#include "Ssignal1CA4floatACsignal2CA12floatAS.h"

#ifndef single_GUARD
#define single_GUARD
typedef float single;
#endif
#ifndef A9singleA_GUARD
#define A9singleA_GUARD
typedef float A9singleA[9];
#endif
#ifndef A6singleA_GUARD
#define A6singleA_GUARD
typedef float A6singleA[6];
#endif
#ifndef A4singleA_GUARD
#define A4singleA_GUARD
typedef float A4singleA[4];
#endif
#ifndef A12singleA_GUARD
#define A12singleA_GUARD
typedef float A12singleA[12];
#endif
#ifndef A15singleA_GUARD
#define A15singleA_GUARD
typedef float A15singleA[15];
#endif
#ifndef A1A15singleAA_GUARD
#define A1A15singleAA_GUARD
typedef float A1A15singleAA[15];
#endif
#ifndef A10singleA_GUARD
#define A10singleA_GUARD
typedef float A10singleA[10];
#endif
#ifndef A1A10singleAA_GUARD
#define A1A10singleAA_GUARD
typedef float A1A10singleAA[10];
#endif
#ifndef A16singleA_GUARD
#define A16singleA_GUARD
typedef float A16singleA[16];
#endif
#ifndef A1A16singleAA_GUARD
#define A1A16singleAA_GUARD
typedef float A1A16singleAA[16];
#endif
#ifndef A9floatA_GUARD
#define A9floatA_GUARD
typedef float A9floatA[9];
#endif
#ifndef A6floatA_GUARD
#define A6floatA_GUARD
typedef float A6floatA[6];
#endif
#ifndef A12floatA_GUARD
#define A12floatA_GUARD
typedef float A12floatA[12];
#endif
#ifndef A4floatA_GUARD
#define A4floatA_GUARD
typedef float A4floatA[4];
#endif
#ifndef A15floatA_GUARD
#define A15floatA_GUARD
typedef float A15floatA[15];
#endif
#ifndef A10floatA_GUARD
#define A10floatA_GUARD
typedef float A10floatA[10];
#endif
#ifndef A16floatA_GUARD
#define A16floatA_GUARD
typedef float A16floatA[16];
#endif
#ifndef A2intA_GUARD
#define A2intA_GUARD
typedef int A2intA[2];
#endif
#ifndef A1A2intAA_GUARD
#define A1A2intAA_GUARD
typedef int A1A2intAA[2];
#endif
#ifndef A3intA_GUARD
#define A3intA_GUARD
typedef int A3intA[3];
#endif
#ifndef A1A3intAA_GUARD
#define A1A3intAA_GUARD
typedef int A1A3intAA[3];
#endif
#ifndef A4intA_GUARD
#define A4intA_GUARD
typedef int A4intA[4];
#endif
#ifndef A1A4intAA_GUARD
#define A1A4intAA_GUARD
typedef int A1A4intAA[4];
#endif
#ifndef A5intA_GUARD
#define A5intA_GUARD
typedef int A5intA[5];
#endif
#ifndef A1A5intAA_GUARD
#define A1A5intAA_GUARD
typedef int A1A5intAA[5];
#endif
#ifndef A6intA_GUARD
#define A6intA_GUARD
typedef int A6intA[6];
#endif
#ifndef A1A6intAA_GUARD
#define A1A6intAA_GUARD
typedef int A1A6intAA[6];
#endif
#ifndef A7intA_GUARD
#define A7intA_GUARD
typedef int A7intA[7];
#endif
#ifndef A1A7intAA_GUARD
#define A1A7intAA_GUARD
typedef int A1A7intAA[7];
#endif
#ifndef A8intA_GUARD
#define A8intA_GUARD
typedef int A8intA[8];
#endif
#ifndef A1A8intAA_GUARD
#define A1A8intAA_GUARD
typedef int A1A8intAA[8];
#endif
#ifndef A9intA_GUARD
#define A9intA_GUARD
typedef int A9intA[9];
#endif
#ifndef A1A9intAA_GUARD
#define A1A9intAA_GUARD
typedef int A1A9intAA[9];
#endif
#ifndef A10intA_GUARD
#define A10intA_GUARD
typedef int A10intA[10];
#endif
#ifndef A1A10intAA_GUARD
#define A1A10intAA_GUARD
typedef int A1A10intAA[10];
#endif
#ifndef A11intA_GUARD
#define A11intA_GUARD
typedef int A11intA[11];
#endif
#ifndef A1A11intAA_GUARD
#define A1A11intAA_GUARD
typedef int A1A11intAA[11];
#endif
#ifndef A12intA_GUARD
#define A12intA_GUARD
typedef int A12intA[12];
#endif
#ifndef A1A12intAA_GUARD
#define A1A12intAA_GUARD
typedef int A1A12intAA[12];
#endif
#ifndef A13intA_GUARD
#define A13intA_GUARD
typedef int A13intA[13];
#endif
#ifndef A1A13intAA_GUARD
#define A1A13intAA_GUARD
typedef int A1A13intAA[13];
#endif
#ifndef A14intA_GUARD
#define A14intA_GUARD
typedef int A14intA[14];
#endif
#ifndef A1A14intAA_GUARD
#define A1A14intAA_GUARD
typedef int A1A14intAA[14];
#endif
#ifndef A15intA_GUARD
#define A15intA_GUARD
typedef int A15intA[15];
#endif
#ifndef A1A15intAA_GUARD
#define A1A15intAA_GUARD
typedef int A1A15intAA[15];
#endif
#ifndef A2A15intAA_GUARD
#define A2A15intAA_GUARD
typedef int A2A15intAA[2][15];
#endif
#ifndef A3A15intAA_GUARD
#define A3A15intAA_GUARD
typedef int A3A15intAA[3][15];
#endif
#ifndef A4A15intAA_GUARD
#define A4A15intAA_GUARD
typedef int A4A15intAA[4][15];
#endif
#ifndef A5A15intAA_GUARD
#define A5A15intAA_GUARD
typedef int A5A15intAA[5][15];
#endif
#ifndef A6A15intAA_GUARD
#define A6A15intAA_GUARD
typedef int A6A15intAA[6][15];
#endif
#ifndef A7A15intAA_GUARD
#define A7A15intAA_GUARD
typedef int A7A15intAA[7][15];
#endif
#ifndef A8A15intAA_GUARD
#define A8A15intAA_GUARD
typedef int A8A15intAA[8][15];
#endif
#ifndef A9A15intAA_GUARD
#define A9A15intAA_GUARD
typedef int A9A15intAA[9][15];
#endif
#ifndef A10A15intAA_GUARD
#define A10A15intAA_GUARD
typedef int A10A15intAA[10][15];
#endif
#ifndef A11A15intAA_GUARD
#define A11A15intAA_GUARD
typedef int A11A15intAA[11][15];
#endif
#ifndef A12A15intAA_GUARD
#define A12A15intAA_GUARD
typedef int A12A15intAA[12][15];
#endif
#ifndef A12A15floatAA_GUARD
#define A12A15floatAA_GUARD
typedef float A12A15floatAA[12][15];
#endif
#ifndef A2A10intAA_GUARD
#define A2A10intAA_GUARD
typedef int A2A10intAA[2][10];
#endif
#ifndef A3A10intAA_GUARD
#define A3A10intAA_GUARD
typedef int A3A10intAA[3][10];
#endif
#ifndef A4A10intAA_GUARD
#define A4A10intAA_GUARD
typedef int A4A10intAA[4][10];
#endif
#ifndef A4A10floatAA_GUARD
#define A4A10floatAA_GUARD
typedef float A4A10floatAA[4][10];
#endif
#ifndef A16intA_GUARD
#define A16intA_GUARD
typedef int A16intA[16];
#endif
#ifndef A1A16intAA_GUARD
#define A1A16intAA_GUARD
typedef int A1A16intAA[16];
#endif
#ifndef A2A16intAA_GUARD
#define A2A16intAA_GUARD
typedef int A2A16intAA[2][16];
#endif
#ifndef A3A16intAA_GUARD
#define A3A16intAA_GUARD
typedef int A3A16intAA[3][16];
#endif
#ifndef A4A16intAA_GUARD
#define A4A16intAA_GUARD
typedef int A4A16intAA[4][16];
#endif
#ifndef A4A16floatAA_GUARD
#define A4A16floatAA_GUARD
typedef float A4A16floatAA[4][16];
#endif
#ifndef A6A15floatAA_GUARD
#define A6A15floatAA_GUARD
typedef float A6A15floatAA[6][15];
#endif

#ifndef OuterLoop_100000157_context_GUARD
#define OuterLoop_100000157_context_GUARD
typedef struct {
  float Gain863[4][16];
  float Gain923;
} OuterLoop_100000157_context;
#endif

/* SIMPLIFIED PROGRAM CONTEXT */
typedef OuterLoop_100000157_context OuterLoop_context;

void OuterLoop_100000157_main( OuterLoop_100000157_context *context, float pos_ref_2[4], float pos_data_3[12], float (*att_ref_4)[4] );
void OuterLoop_100000157_init( OuterLoop_100000157_context *context );



/* SIMPLIFIED PROGRAM FUNCTIONS */
void OuterLoop_main( OuterLoop_100000157_context *context, float pos_ref_2[4], float pos_data_3[12], float (*att_ref_4)[4] );
void OuterLoop_init( OuterLoop_100000157_context *context );
#endif
