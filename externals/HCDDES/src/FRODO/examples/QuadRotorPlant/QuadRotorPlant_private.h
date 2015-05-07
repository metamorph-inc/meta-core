/*
 * File: QuadRotorPlant_private.h
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

#ifndef RTW_HEADER_QuadRotorPlant_private_h_
#define RTW_HEADER_QuadRotorPlant_private_h_
#include "rtwtypes.h"

/* Private macros used by the generated code to access rtModel */
#ifndef rtmSetFirstInitCond
# define rtmSetFirstInitCond(rtm, val) ((rtm)->Timing.firstInitCondFlag = (val))
#endif

#ifndef rtmIsFirstInitCond
# define rtmIsFirstInitCond(rtm)       ((rtm)->Timing.firstInitCondFlag)
#endif

#ifndef rtmIsMajorTimeStep
# define rtmIsMajorTimeStep(rtm)       (((rtm)->Timing.simTimeStep) == MAJOR_TIME_STEP)
#endif

#ifndef rtmIsMinorTimeStep
# define rtmIsMinorTimeStep(rtm)       (((rtm)->Timing.simTimeStep) == MINOR_TIME_STEP)
#endif

#ifndef rtmGetTPtr
# define rtmGetTPtr(rtm)               ((rtm)->Timing.t)
#endif

#ifndef rtmSetTPtr
# define rtmSetTPtr(rtm, val)          ((rtm)->Timing.t = (val))
#endif

#ifndef __RTWTYPES_H__
#error This file requires rtwtypes.h to be included
#else
#ifdef TMWTYPES_PREVIOUSLY_INCLUDED
#error This file requires rtwtypes.h to be included before tmwtypes.h
#else

/* Check for inclusion of an incorrect version of rtwtypes.h */
#ifndef RTWTYPES_ID_C08S16I32L32N32F1
#error This code was generated with a different "rtwtypes.h" than the file included
#endif                                 /* RTWTYPES_ID_C08S16I32L32N32F1 */
#endif                                 /* TMWTYPES_PREVIOUSLY_INCLUDED */
#endif                                 /* __RTWTYPES_H__ */

real_T look_SplNBinXSd(uint32_T numDims, const real_T* u, const rt_LUTSplineWork
  * const SWork);
real_T intrp_NSpld(uint32_T numDims, const rt_LUTSplineWork * const splWork,
                   uint32_T extrapMethod);
extern uint32_T plook_binxp(real_T u, const real_T bp[], uint32_T maxIndex,
  real_T *fraction, uint32_T *prevIndex);
extern uint32_T binsearch_u32d_prevIdx(real_T u, const real_T bp[], uint32_T
  startIndex, uint32_T maxIndex);
void QuadRotorPlant_step0(RT_MODEL_QuadRotorPlant *const QuadRotorPlant_M,
  BlockIO_QuadRotorPlant *QuadRotorPlant_B, D_Work_QuadRotorPlant
  *QuadRotorPlant_DWork, ContinuousStates_QuadRotorPlant *QuadRotorPlant_X,
  ExternalInputs_QuadRotorPlant *QuadRotorPlant_U,
  ExternalOutputs_QuadRotorPlant *QuadRotorPlant_Y);
void QuadRotorPlant_step2(RT_MODEL_QuadRotorPlant *const QuadRotorPlant_M,
  BlockIO_QuadRotorPlant *QuadRotorPlant_B, D_Work_QuadRotorPlant
  *QuadRotorPlant_DWork, ExternalInputs_QuadRotorPlant *QuadRotorPlant_U,
  ExternalOutputs_QuadRotorPlant *QuadRotorPlant_Y);
void QuadRotorPlant_step3(BlockIO_QuadRotorPlant *QuadRotorPlant_B,
  D_Work_QuadRotorPlant *QuadRotorPlant_DWork);
void QuadRotorPlant_step4(BlockIO_QuadRotorPlant *QuadRotorPlant_B,
  D_Work_QuadRotorPlant *QuadRotorPlant_DWork);/* private model entry point functions */
extern void QuadRotorPlant_derivatives(RT_MODEL_QuadRotorPlant *const
  QuadRotorPlant_M, BlockIO_QuadRotorPlant *QuadRotorPlant_B,
  ContinuousStates_QuadRotorPlant *QuadRotorPlant_X);

#endif                                 /* RTW_HEADER_QuadRotorPlant_private_h_ */

/*
 * File trailer for Real-Time Workshop generated code.
 *
 * [EOF]
 */
