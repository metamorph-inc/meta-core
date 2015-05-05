#ifndef _CruiseController_sl_H_
#define _CruiseController_sl_H_

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

#ifndef DT_Int_100000046_context_GUARD
#define DT_Int_100000046_context_GUARD
typedef struct {
  float X0148;
  float X0152;
  float Gain161;
  float LowerLimit189;
  float UpperLimit190;
} DT_Int_100000046_context;
#endif

#ifndef DT_Int_100000077_context_GUARD
#define DT_Int_100000077_context_GUARD
typedef struct {
  float X0218;
  float X0222;
  float Gain226;
  float LowerLimit238;
  float UpperLimit239;
} DT_Int_100000077_context;
#endif

#ifndef D_Term_100000066_context_GUARD
#define D_Term_100000066_context_GUARD
typedef struct {
  float X0156;
  float Gain166;
  float Gain174;
  DT_Int_100000077_context DT_Int_100000077_class_member3;
} D_Term_100000066_context;
#endif

#ifndef DPID_100000036_context_GUARD
#define DPID_100000036_context_GUARD
typedef struct {
  float Gain108;
  float Gain119;
  float LowerLimit124;
  float UpperLimit125;
  D_Term_100000066_context D_Term_100000066_class_member4;
  DT_Int_100000046_context DT_Int_100000046_class_member5;
} DPID_100000036_context;
#endif

#ifndef CruiseController_100000017_context_GUARD
#define CruiseController_100000017_context_GUARD
typedef struct {
  float Value65;
  float LowerLimit81;
  float UpperLimit83;
  DPID_100000036_context DPID_100000036_class_member3;
} CruiseController_100000017_context;
#endif

#ifndef DT_Int_100000046_context_GUARD
#define DT_Int_100000046_context_GUARD
typedef struct {
  float X0148;
  float X0152;
  float Gain161;
  float LowerLimit189;
  float UpperLimit190;
} DT_Int_100000046_context;
#endif

#ifndef DT_Int_100000077_context_GUARD
#define DT_Int_100000077_context_GUARD
typedef struct {
  float X0218;
  float X0222;
  float Gain226;
  float LowerLimit238;
  float UpperLimit239;
} DT_Int_100000077_context;
#endif

#ifndef D_Term_100000066_context_GUARD
#define D_Term_100000066_context_GUARD
typedef struct {
  float X0156;
  float Gain166;
  float Gain174;
  DT_Int_100000077_context DT_Int_100000077_class_member3;
} D_Term_100000066_context;
#endif

#ifndef DPID_100000036_context_GUARD
#define DPID_100000036_context_GUARD
typedef struct {
  float Gain108;
  float Gain119;
  float LowerLimit124;
  float UpperLimit125;
  D_Term_100000066_context D_Term_100000066_class_member4;
  DT_Int_100000046_context DT_Int_100000046_class_member5;
} DPID_100000036_context;
#endif

#ifndef DT_Int_100000046_context_GUARD
#define DT_Int_100000046_context_GUARD
typedef struct {
  float X0148;
  float X0152;
  float Gain161;
  float LowerLimit189;
  float UpperLimit190;
} DT_Int_100000046_context;
#endif

#ifndef DT_Int_100000077_context_GUARD
#define DT_Int_100000077_context_GUARD
typedef struct {
  float X0218;
  float X0222;
  float Gain226;
  float LowerLimit238;
  float UpperLimit239;
} DT_Int_100000077_context;
#endif

#ifndef D_Term_100000066_context_GUARD
#define D_Term_100000066_context_GUARD
typedef struct {
  float X0156;
  float Gain166;
  float Gain174;
  DT_Int_100000077_context DT_Int_100000077_class_member3;
} D_Term_100000066_context;
#endif

#ifndef DT_Int_100000077_context_GUARD
#define DT_Int_100000077_context_GUARD
typedef struct {
  float X0218;
  float X0222;
  float Gain226;
  float LowerLimit238;
  float UpperLimit239;
} DT_Int_100000077_context;
#endif

/* SIMPLIFIED PROGRAM CONTEXT */
typedef CruiseController_100000017_context CruiseController_context;

void CruiseController_100000017_main( CruiseController_100000017_context *context, float V_demand_2, float velocity_3, float *Throttle_4 );
void CruiseController_100000017_init( CruiseController_100000017_context *context );



/* SIMPLIFIED PROGRAM FUNCTIONS */
void CruiseController_main( CruiseController_100000017_context *context, float V_demand_2, float velocity_3, float *Throttle_4 );
void CruiseController_init( CruiseController_100000017_context *context );
#endif
