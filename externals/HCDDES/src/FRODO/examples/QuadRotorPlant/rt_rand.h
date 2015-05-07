/*
 * File: rt_rand.h
 *
 * Real-Time Workshop code generated for Simulink model QuadRotorPlant.
 *
 * Model version                        : 1.156
 * Real-Time Workshop file version      : 7.5  (R2010a)  25-Jan-2010
 * Real-Time Workshop file generated on : Mon Jun 28 10:45:43 2010
 * TLC version                          : 7.5 (Jan 19 2010)
 * C/C++ source code generated on       : Mon Jun 28 10:45:44 2010
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Generic->32-bit x86 compatible
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef RTW_HEADER_rt_rand_h_
#define RTW_HEADER_rt_rand_h_
#include <math.h>
#include "rtwtypes.h"
# define MAXSEED                       2147483646                /* 2^31-2 */
# define SEED0                         1144108930                /* Seed #6, starting from seed = 1 */
# define RT_BIT16                      32768                     /* 2^15   */

extern real_T rt_Urand(uint32_T *seed);
extern real_T rt_NormalRand(uint32_T *seed);

#endif                                 /* RTW_HEADER_rt_rand_h_ */

/*
 * File trailer for Real-Time Workshop generated code.
 *
 * [EOF]
 */
