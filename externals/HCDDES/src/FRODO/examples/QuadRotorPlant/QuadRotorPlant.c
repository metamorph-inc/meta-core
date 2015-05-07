/*
 * File: QuadRotorPlant.c
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

#include "QuadRotorPlant.h"
#include "QuadRotorPlant_private.h"

/* n-D Spline interpolation function */
real_T look_SplNBinXSd(uint32_T numDims, const real_T* u, const rt_LUTSplineWork
  * const SWork)
{
  /*
   *   n-D column-major table lookup operating on real_T with:
   *       - Spline interpolation
   *       - Linear extrapolation
   *       - Binary breakpoint search
   *       - Uses previous index search result
   */
  rt_LUTnWork * const TWork_look = SWork->m_TWork;
  real_T* const fraction = (real_T*) TWork_look->m_bpLambda;
  uint32_T* const bpIdx = TWork_look->m_bpIndex;
  const uint32_T* const maxIndex = TWork_look->m_maxIndex;
  uint32_T k;
  for (k = 0U; k < numDims; k++) {
    const real_T* const bpData = ((const real_T * const *)
      TWork_look->m_bpDataSet)[k];
    bpIdx[k] = plook_binxp(u[k], &bpData[0], maxIndex[k], &fraction[k], &bpIdx[k]);
  }

  return(intrp_NSpld(numDims, SWork, 2U));
}

static void rt_Spline2Derivd(const real_T *x,
  const real_T *y,
  uint32_T n, real_T *u,
  real_T *y2)
  ;

/*
 * Second derivative initialization function for spline
 * for last dimension.
 */
static void rt_Spline2Derivd(const real_T *x,
  const real_T *y,
  uint32_T n, real_T *u,
  real_T *y2)
{
  real_T p, qn, sig, un;
  uint32_T n1, i, k;
  n1 = n - 1U;
  y2[0U] = 0.0;
  u[0U] = 0.0;
  for (i = 1U; i < n1; i++) {
    real_T dxm1 = x[i] - x[i - 1U];
    real_T dxp1 = x[i + 1U] - x[i];
    real_T dxpm = dxp1 + dxm1;
    sig = dxm1 / dxpm;
    p = (sig * y2[i - 1U]) + 2.0;
    y2[i] = (sig - 1.0) / p;
    u[i] = ((y[i + 1U] - y[i]) / dxp1) - ((y[i] - y[i - 1U]) / dxm1);
    u[i] = (((6.0 * u[i]) / dxpm) - (sig * u[i - 1U])) / p;
  }

  qn = 0.0;
  un = 0.0;
  y2[n1] = (un - (qn * u[n1 - 1U])) / ((qn * y2[n1 - 1U]) + 1.0);
  for (k = n1; k > 0U; k--) {
    y2[k-1U] = (y2[k-1U] * y2[k]) + u[k-1U];
  }

  return;
}

/* n-D natural spline calculation function */
real_T intrp_NSpld(uint32_T numDims, const rt_LUTSplineWork * const splWork,
                   uint32_T extrapMethod)
{
  uint32_T il;
  uint32_T iu, k, i;
  real_T h, s, p, smsq, pmsq;

  /* intermediate results work areas "this" and "next" */
  const rt_LUTnWork *TWork_interp = (const rt_LUTnWork *)splWork->m_TWork;
  const real_T *fraction = (real_T *) TWork_interp->m_bpLambda;
  const real_T *yp = (real_T *) TWork_interp->m_tableData;
  real_T *yyA = (real_T *) splWork->m_yyA;
  real_T *yyB = (real_T *) splWork->m_yyB;
  real_T *yy2 = (real_T *) splWork->m_yy2;
  real_T *up = (real_T *) splWork->m_up;
  real_T *y2 = (real_T *) splWork->m_y2;
  const real_T **bpDataSet = (const real_T **) TWork_interp->m_bpDataSet;
  const real_T *xp = bpDataSet[0U];
  real_T *yy = yyA;
  uint32_T bufBank = 0U;
  uint32_T len = TWork_interp->m_maxIndex[0U] + 1U;

  /* Generate first dimension's second derivatives */
  for (i = 0U; i < splWork->m_numYWorkElts[0U]; i++) {
    rt_Spline2Derivd(xp, yp, len, up, y2);
    yp = &yp[len];
    y2 = &y2[len];
  }

  /* Set pointers back to beginning */
  yp = (const real_T *) TWork_interp->m_tableData;
  y2 = (real_T *) splWork->m_y2;

  /* Generate at-point splines in each dimension */
  for (k = 0U; k < numDims; k++ ) {
    /* this dimension's input setup */
    xp = bpDataSet[k];
    len = TWork_interp->m_maxIndex[k] + 1U;
    il = TWork_interp->m_bpIndex[k];
    iu = il + 1U;
    h = xp[iu] - xp[il];
    p = fraction[k];
    s = 1.0 - p;
    pmsq = p * ((p*p) - 1.0);
    smsq = s * ((s*s) - 1.0);

    /*
     * Calculate spline curves for input in this
     * dimension at each value of the higher
     * other dimensions\' points in the table.
     */
    if ((p > 1.0) && (extrapMethod == 2U) ) {
      real_T slope;
      for (i = 0U; i < splWork->m_numYWorkElts[k]; i++) {
        slope = (yp[iu] - yp[il]) + ((y2[il]*h*h)/6.0);
        yy[i] = yp[iu] + (slope * (p-1.0));
        yp = &yp[len];
        y2 = &y2[len];
      }
    } else if ((p < 0.0) && (extrapMethod == 2U) ) {
      real_T slope;
      for (i = 0U; i < splWork->m_numYWorkElts[k]; i++) {
        slope = (yp[iu] - yp[il]) - ((y2[iu]*h*h)/6.0);
        yy[i] = yp[il] + (slope * p);
        yp = &yp[len];
        y2 = &y2[len];
      }
    } else {
      for (i = 0U; i < splWork->m_numYWorkElts[k]; i++) {
        yy[i] = yp[il] + p * (yp[iu] - yp[il]) +
          ((smsq * y2[il] + pmsq * y2[iu])*h*h)/6.0;
        yp = &yp[len];
        y2 = &y2[len];
      }
    }

    /* set pointers to new result and calculate second derivatives */
    yp = yy;
    y2 = yy2;
    if (splWork->m_numYWorkElts[k+1U] > 0U ) {
      uint32_T nextLen = TWork_interp->m_maxIndex[k+1U] + 1U;
      const real_T *nextXp = bpDataSet[k+1U];
      for (i = 0U; i < splWork->m_numYWorkElts[k+1U]; i++) {
        rt_Spline2Derivd(nextXp, yp, nextLen, up, y2);
        yp = &yp[nextLen];
        y2 = &y2[nextLen];
      }
    }

    /*
     * Set work vectors yp, y2 and yy for next iteration;
     * the yy just calculated becomes the yp in the
     * next iteration, y2 was just calculated for these
     * new points and the yy buffer is swapped to the space
     * for storing the next iteration\'s results.
     */
    yp = yy;
    y2 = yy2;

    /*
     * Swap buffers for next dimension and
     * toggle bufBank for next iteration.
     */
    if (bufBank == 0U) {
      yy = yyA;
      bufBank = 1U;
    } else {
      yy = yyB;
      bufBank = 0U;
    }
  }

  return( yp[0U] );
}

uint32_T plook_binxp(real_T u, const real_T bp[], uint32_T maxIndex, real_T
                     *fraction, uint32_T *prevIndex)
{
  uint32_T bpIndex;
  uint32_T startIndex;

  /* Prelookup - Index and Fraction
     Index Search method: 'binary'
     Process out of range input: 'Linear extrapolation'
     Use previous index: 'on'
     Use last breakpoint for index at or above upper limit: 'off'
   */
  if (u <= bp[0U]) {
    bpIndex = 0U;
    *fraction = (u - bp[0U]) / (bp[1U] - bp[0U]);
  } else if (u < bp[maxIndex]) {
    startIndex = *prevIndex;
    bpIndex = binsearch_u32d_prevIdx(u, bp, startIndex, maxIndex);
    *fraction = (u - bp[bpIndex]) / (bp[bpIndex + 1U] - bp[bpIndex]);
  } else {
    bpIndex = maxIndex - 1U;
    *fraction = (u - bp[maxIndex - 1U]) / (bp[maxIndex] - bp[maxIndex - 1U]);
  }

  *prevIndex = bpIndex;
  return bpIndex;
}

uint32_T binsearch_u32d_prevIdx(real_T u, const real_T bp[], uint32_T startIndex,
  uint32_T maxIndex)
{
  uint32_T bpIndex;
  uint32_T iRght;
  uint32_T iLeft;
  uint32_T bpIdx;
  uint32_T found;

  /* Binary Search using Previous Index */
  bpIdx = startIndex;
  iLeft = 0U;
  iRght = maxIndex;
  found = 0U;
  while (found == 0U) {
    if (u < bp[bpIdx]) {
      iRght = bpIdx - 1U;
      bpIdx = (iRght + iLeft) >> 1U;
    } else if (u < bp[bpIdx + 1U]) {
      found = 1U;
    } else {
      iLeft = bpIdx + 1U;
      bpIdx = (iRght + iLeft) >> 1U;
    }
  }

  bpIndex = bpIdx;
  return bpIndex;
}

/* This function updates continuous states using the ODE8 fixed-step
 * solver algorithm
 */
static void rt_ertODEUpdateContinuousStates(RTWSolverInfo *si ,
  RT_MODEL_QuadRotorPlant *const QuadRotorPlant_M, BlockIO_QuadRotorPlant
  *QuadRotorPlant_B, D_Work_QuadRotorPlant *QuadRotorPlant_DWork,
  ContinuousStates_QuadRotorPlant *QuadRotorPlant_X,
  ExternalInputs_QuadRotorPlant *QuadRotorPlant_U,
  ExternalOutputs_QuadRotorPlant *QuadRotorPlant_Y)
{
  /* Solver Matrices */
#define QuadRotorPlant_NSTAGES         13

  static real_T rt_ODE8_B[13] = {
    4.174749114153025E-2, 0.0, 0.0, 0.0,
    0.0, -5.54523286112393E-2, 2.393128072011801E-1, 7.03510669403443E-1,
    -7.597596138144609E-1, 6.605630309222863E-1, 1.581874825101233E-1,
    -2.381095387528628E-1, 2.5E-1
  };

  static real_T rt_ODE8_C[13] = {
    0.0, 5.555555555555556E-2, 8.333333333333333E-2, 1.25E-1,
    3.125E-1, 3.75E-1, 1.475E-1, 4.65E-1,
    5.648654513822596E-1, 6.5E-1, 9.246562776405044E-1, 1.0, 1.0
  };

  static real_T rt_ODE8_A[13][13] = {
    /* rt_ODE8_A[0][] */
    { 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0 },

    /* rt_ODE8_A[1][] */
    { 5.555555555555556E-2, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0 },

    /* rt_ODE8_A[2][] */
    { 2.083333333333333E-2, 6.25E-2, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0 },

    /* rt_ODE8_A[3][] */
    { 3.125E-2, 0.0, 9.375E-2, 0.0,
      0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0 },

    /* rt_ODE8_A[4][] */
    { 3.125E-1, 0.0, -1.171875, 1.171875,
      0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0 },

    /* rt_ODE8_A[5][] */
    { 3.75E-2, 0.0, 0.0, 1.875E-1,
      1.5E-1, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0 },

    /* rt_ODE8_A[6][] */
    { 4.791013711111111E-2, 0.0, 0.0, 1.122487127777778E-1,
      -2.550567377777778E-2, 1.284682388888889E-2, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0 },

    /* rt_ODE8_A[7][] */
    { 1.691798978729228E-2, 0.0, 0.0, 3.878482784860432E-1,
      3.597736985150033E-2, 1.969702142156661E-1, -1.727138523405018E-1, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0 },

    /* rt_ODE8_A[8][] */
    { 6.90957533591923E-2, 0.0, 0.0, -6.342479767288542E-1,
      -1.611975752246041E-1, 1.386503094588253E-1, 9.409286140357563E-1,
      2.11636326481944E-1,
      0.0, 0.0, 0.0, 0.0, 0.0 },

    /* rt_ODE8_A[9][] */
    { 1.835569968390454E-1, 0.0, 0.0, -2.468768084315592,
      -2.912868878163005E-1, -2.647302023311738E-2, 2.8478387641928,
      2.813873314698498E-1,
      1.237448998633147E-1, 0.0, 0.0, 0.0, 0.0 },

    /* rt_ODE8_A[10][] */
    { -1.215424817395888, 0.0, 0.0, 1.667260866594577E1,
      9.157418284168179E-1, -6.056605804357471, -1.600357359415618E1,
      1.484930308629766E1,
      -1.337157573528985E1, 5.134182648179638, 0.0, 0.0, 0.0 },

    /* rt_ODE8_A[11][] */
    { 2.588609164382643E-1, 0.0, 0.0, -4.774485785489205,
      -4.350930137770325E-1, -3.049483332072241, 5.577920039936099,
      6.155831589861039,
      -5.062104586736938, 2.193926173180679, 1.346279986593349E-1, 0.0, 0.0 },

    /* rt_ODE8_A[12][] */
    { 8.224275996265075E-1, 0.0, 0.0, -1.165867325727766E1,
      -7.576221166909362E-1, 7.139735881595818E-1, 1.207577498689006E1,
      -2.127659113920403,
      1.990166207048956, -2.342864715440405E-1, 1.758985777079423E-1, 0.0, 0.0 },
  };

  time_T t = rtsiGetT(si);
  time_T tnew = rtsiGetSolverStopTime(si);
  time_T h = rtsiGetStepSize(si);
  real_T *x = rtsiGetContStates(si);
  ODE8_IntgData *intgData = (ODE8_IntgData *) rtsiGetSolverData(si);
  real_T *deltaY = intgData->deltaY;
  real_T *x0 = intgData->x0;
  real_T* f[QuadRotorPlant_NSTAGES];
  int idx,stagesIdx,statesIdx;
  double deltaX;
  int_T nXc = 17;
  rtsiSetSimTimeStep(si,MINOR_TIME_STEP);
  f[0] = intgData->f[0];
  f[1] = intgData->f[1];
  f[2] = intgData->f[2];
  f[3] = intgData->f[3];
  f[4] = intgData->f[4];
  f[5] = intgData->f[5];
  f[6] = intgData->f[6];
  f[7] = intgData->f[7];
  f[8] = intgData->f[8];
  f[9] = intgData->f[9];
  f[10] = intgData->f[10];
  f[11] = intgData->f[11];
  f[12] = intgData->f[12];

  /* Save the state values at time t in y and x0*/
  (void) memset(deltaY, 0,
                nXc*sizeof(real_T));
  (void) memcpy(x0, x,
                nXc*sizeof(real_T));
  for (stagesIdx=0;stagesIdx<QuadRotorPlant_NSTAGES;stagesIdx++) {
    (void) memcpy(x, x0,
                  nXc*sizeof(real_T));
    for (statesIdx=0;statesIdx<nXc;statesIdx++) {
      deltaX = 0;
      for (idx=0;idx<stagesIdx;idx++) {
        deltaX = deltaX + h*rt_ODE8_A[stagesIdx][idx]*f[idx][statesIdx];
      }

      x[statesIdx] = x0[statesIdx] + deltaX;
    }

    if (stagesIdx==0) {
      rtsiSetdX(si, f[stagesIdx]);
      QuadRotorPlant_derivatives(QuadRotorPlant_M, QuadRotorPlant_B,
        QuadRotorPlant_X);
    } else {
      (stagesIdx==QuadRotorPlant_NSTAGES-1)? rtsiSetT(si, tnew) : rtsiSetT(si, t
        + h*rt_ODE8_C[stagesIdx]);
      rtsiSetdX(si, f[stagesIdx]);
      QuadRotorPlant_step0(QuadRotorPlant_M, QuadRotorPlant_B,
                           QuadRotorPlant_DWork, QuadRotorPlant_X,
                           QuadRotorPlant_U, QuadRotorPlant_Y);
      QuadRotorPlant_derivatives(QuadRotorPlant_M, QuadRotorPlant_B,
        QuadRotorPlant_X);
    }

    for (statesIdx=0;statesIdx<nXc;statesIdx++) {
      deltaY[statesIdx] = deltaY[statesIdx] + h*rt_ODE8_B[stagesIdx]*f[stagesIdx]
        [statesIdx];
    }
  }

  for (statesIdx=0;statesIdx<nXc;statesIdx++) {
    x[statesIdx] = x0[statesIdx] + deltaY[statesIdx];
  }

  rtsiSetSimTimeStep(si,MAJOR_TIME_STEP);
}

/* Model step function for TID0 */
void QuadRotorPlant_step0(RT_MODEL_QuadRotorPlant *const QuadRotorPlant_M,
  BlockIO_QuadRotorPlant *QuadRotorPlant_B, D_Work_QuadRotorPlant
  *QuadRotorPlant_DWork, ContinuousStates_QuadRotorPlant *QuadRotorPlant_X,
  ExternalInputs_QuadRotorPlant *QuadRotorPlant_U,
  ExternalOutputs_QuadRotorPlant *QuadRotorPlant_Y) /* Sample time: [0.0s, 0.0s] */
{
  /* local block i/o variables */
  real_T rtb_V[3];
  real_T rtb_Product[4];
  real_T rtb_Fcn_ab_0;
  real_T rtb_Fcn2_g;
  real_T rtb_Fcn1_o;
  real_T rtb_Fcn_a;
  real_T rtb_Fcn2_l;
  real_T rtb_Fcn1_f;
  real_T rtb_flapangle;
  real_T rtb_Fcn_ab;
  real_T rtb_Product8[3];
  real_T rtb_SumofElements;
  real_T rtb_unitvector[3];
  real32_T rtb_Gain1_n;
  real_T rtb_TrigonometricFunction1;
  real_T rtb_Product4[3];
  real_T rtb_Sum1_m[3];
  real_T rtb_Fcn1_e;
  real_T rtb_Fcn2_i;
  real_T rtb_Gain[9];
  real_T rtb_Gain1[9];
  real_T rtb_Gain2[9];
  real_T rtb_Gain3[9];
  real_T rtb_Gain4[9];
  real_T rtb_Gain5[9];
  real_T rtb_Gain6[9];
  int32_T i;
  real_T rtb_Gain_0[3];
  int32_T tmp;
  real_T tmp_0[6];
  real32_T tmp_1[3];
  real_T rtb_Product_0;
  real_T rtb_Sum_0;
  real32_T rtb_Gain1_f_idx;
  real32_T rtb_Gain1_f_idx_0;
  real32_T rtb_Gain1_f_idx_1;
  real32_T rtb_Gain1_f_idx_2;
  real_T rtb_Sum_idx;
  real_T rtb_Sum_idx_0;
  if (rtmIsMajorTimeStep(QuadRotorPlant_M)) {
    /* set solver stop time */
    if (!(QuadRotorPlant_M->Timing.clockTick0+1)) {
      rtsiSetSolverStopTime(&QuadRotorPlant_M->solverInfo,
                            ((QuadRotorPlant_M->Timing.clockTickH0 + 1) *
        QuadRotorPlant_M->Timing.stepSize0 * 4294967296.0));
    } else {
      rtsiSetSolverStopTime(&QuadRotorPlant_M->solverInfo,
                            ((QuadRotorPlant_M->Timing.clockTick0 + 1) *
        QuadRotorPlant_M->Timing.stepSize0 +
        QuadRotorPlant_M->Timing.clockTickH0 *
        QuadRotorPlant_M->Timing.stepSize0 * 4294967296.0));
    }

    if (rtmIsMajorTimeStep(QuadRotorPlant_M)) {
      /* Update the flag to indicate when data transfers from
       *  Sample time: [0.0s, 0.0s] to Sample time: [0.02s, 0.0s]  */
      (QuadRotorPlant_M->Timing.RateInteraction.TID0_2)++;
      if ((QuadRotorPlant_M->Timing.RateInteraction.TID0_2) > 1) {
        QuadRotorPlant_M->Timing.RateInteraction.TID0_2 = 0;
      }

      /* Update the flag to indicate when data transfers from
       *  Sample time: [0.0s, 0.0s] to Sample time: [0.1s, 0.0s]  */
      (QuadRotorPlant_M->Timing.RateInteraction.TID0_3)++;
      if ((QuadRotorPlant_M->Timing.RateInteraction.TID0_3) > 9) {
        QuadRotorPlant_M->Timing.RateInteraction.TID0_3 = 0;
      }

      /* Update the flag to indicate when data transfers from
       *  Sample time: [0.0s, 0.0s] to Sample time: [0.2s, 0.0s]  */
      (QuadRotorPlant_M->Timing.RateInteraction.TID0_4)++;
      if ((QuadRotorPlant_M->Timing.RateInteraction.TID0_4) > 19) {
        QuadRotorPlant_M->Timing.RateInteraction.TID0_4 = 0;
      }

      /* Update the flag to indicate when data transfers from
       *  Sample time: [0.01s, 0.0s] to Sample time: [0.02s, 0.0s]  */
      (QuadRotorPlant_M->Timing.RateInteraction.TID1_2)++;
      if ((QuadRotorPlant_M->Timing.RateInteraction.TID1_2) > 1) {
        QuadRotorPlant_M->Timing.RateInteraction.TID1_2 = 0;
      }

      /* Update the flag to indicate when data transfers from
       *  Sample time: [0.01s, 0.0s] to Sample time: [0.1s, 0.0s]  */
      (QuadRotorPlant_M->Timing.RateInteraction.TID1_3)++;
      if ((QuadRotorPlant_M->Timing.RateInteraction.TID1_3) > 9) {
        QuadRotorPlant_M->Timing.RateInteraction.TID1_3 = 0;
      }
    }
  }                                    /* end MajorTimeStep */

  /* Update absolute time of base rate at minor time step */
  if (rtmIsMinorTimeStep(QuadRotorPlant_M)) {
    QuadRotorPlant_M->Timing.t[0] = rtsiGetT(&QuadRotorPlant_M->solverInfo);
  }

  /* Outport: '<Root>/Out3' incorporates:
   *  Integrator: '<S4>/Integrator'
   */
  QuadRotorPlant_Y->Out3 = QuadRotorPlant_X->Integrator_CSTATE;

  /* Integrator: '<S11>/integrator' */
  if (QuadRotorPlant_DWork->integrator_IWORK.IcNeedsLoading) {
    QuadRotorPlant_X->integrator_CSTATE[0] = QuadRotorPlant_ConstB.Constant[0];
    QuadRotorPlant_X->integrator_CSTATE[1] = QuadRotorPlant_ConstB.Constant[1];
    QuadRotorPlant_X->integrator_CSTATE[2] = QuadRotorPlant_ConstB.Constant[2];
    QuadRotorPlant_DWork->integrator_IWORK.IcNeedsLoading = 0;
  }

  QuadRotorPlant_B->integrator[0] = QuadRotorPlant_X->integrator_CSTATE[0];
  QuadRotorPlant_B->integrator[1] = QuadRotorPlant_X->integrator_CSTATE[1];
  QuadRotorPlant_B->integrator[2] = QuadRotorPlant_X->integrator_CSTATE[2];

  /* Trigonometry: '<S12>/Trigonometric Function' */
  rtb_Fcn2_g = sin(QuadRotorPlant_B->integrator[0]);

  /* Trigonometry: '<S12>/Trigonometric Function1' */
  rtb_Fcn1_o = cos(QuadRotorPlant_B->integrator[0]);

  /* Trigonometry: '<S12>/Trigonometric Function2' */
  rtb_Fcn_a = sin(QuadRotorPlant_B->integrator[1]);

  /* Trigonometry: '<S12>/Trigonometric Function3' */
  rtb_Fcn2_l = cos(QuadRotorPlant_B->integrator[1]);

  /* Trigonometry: '<S12>/Trigonometric Function4' */
  rtb_Fcn1_f = sin(QuadRotorPlant_B->integrator[2]);

  /* Trigonometry: '<S12>/Trigonometric Function5' */
  rtb_flapangle = cos(QuadRotorPlant_B->integrator[2]);

  /* Fcn: '<S12>/ct x cpsi' */
  rtb_Fcn_ab = rtb_Fcn2_l * rtb_flapangle;

  /* Gain: '<S12>/Gain' */
  for (i = 0; i < 9; i++) {
    rtb_Gain[i] = QuadRotorPlant_ConstP.Gain_Gain[i] * rtb_Fcn_ab;
  }

  /* Fcn: '<S12>/ct x spsi' */
  rtb_Fcn_ab = rtb_Fcn2_l * rtb_Fcn1_f;

  /* Gain: '<S12>/Gain1' */
  for (i = 0; i < 9; i++) {
    rtb_Gain1[i] = QuadRotorPlant_ConstP.Gain1_Gain[i] * rtb_Fcn_ab;
  }

  /* Fcn: '<S12>/-st' */
  rtb_Fcn_ab = -rtb_Fcn_a;

  /* Gain: '<S12>/Gain2' */
  for (i = 0; i < 9; i++) {
    rtb_Gain2[i] = QuadRotorPlant_ConstP.Gain2_Gain[i] * rtb_Fcn_ab;
  }

  /* Fcn: '<S12>/-cphi x spsi + sphi x st x cpsi' */
  rtb_Fcn_ab = rtb_Fcn2_g * rtb_Fcn_a * rtb_flapangle + (-rtb_Fcn1_o) *
    rtb_Fcn1_f;

  /* Gain: '<S12>/Gain3' */
  for (i = 0; i < 9; i++) {
    rtb_Gain3[i] = QuadRotorPlant_ConstP.Gain3_Gain[i] * rtb_Fcn_ab;
  }

  /* Fcn: '<S12>/cpsi x cphi + sphi x st x spsi' */
  rtb_Fcn_ab = rtb_Fcn2_g * rtb_Fcn_a * rtb_Fcn1_f + rtb_flapangle * rtb_Fcn1_o;

  /* Gain: '<S12>/Gain4' */
  for (i = 0; i < 9; i++) {
    rtb_Gain4[i] = QuadRotorPlant_ConstP.Gain4_Gain[i] * rtb_Fcn_ab;
  }

  /* Fcn: '<S12>/sphi x ct' */
  rtb_Fcn_ab = rtb_Fcn2_g * rtb_Fcn2_l;

  /* Gain: '<S12>/Gain5' */
  for (i = 0; i < 9; i++) {
    rtb_Gain5[i] = QuadRotorPlant_ConstP.Gain5_Gain[i] * rtb_Fcn_ab;
  }

  /* Fcn: '<S12>/sphi x spsi + cphi x cpsi st' */
  rtb_Fcn_ab = rtb_Fcn1_o * rtb_flapangle * rtb_Fcn_a + rtb_Fcn2_g * rtb_Fcn1_f;

  /* Gain: '<S12>/Gain6' */
  for (i = 0; i < 9; i++) {
    rtb_Gain6[i] = QuadRotorPlant_ConstP.Gain6_Gain[i] * rtb_Fcn_ab;
  }

  /* Fcn: '<S12>/-cpsi x sphi + cphi x st x spsi' */
  rtb_Fcn_ab = rtb_Fcn1_o * rtb_Fcn_a * rtb_Fcn1_f + (-rtb_flapangle) *
    rtb_Fcn2_g;

  /* Fcn: '<S12>/cphi x ct' */
  rtb_Fcn2_g = rtb_Fcn1_o * rtb_Fcn2_l;

  /* Sum: '<S12>/Sum' incorporates:
   *  Gain: '<S12>/Gain7'
   *  Gain: '<S12>/Gain8'
   */
  for (i = 0; i < 9; i++) {
    rtb_Gain[i] = (((((((rtb_Gain[i] + rtb_Gain1[i]) + rtb_Gain2[i]) +
                       rtb_Gain3[i]) + rtb_Gain4[i]) + rtb_Gain5[i]) +
                    rtb_Gain6[i]) + QuadRotorPlant_ConstP.Gain7_Gain[i] *
                   rtb_Fcn_ab) + QuadRotorPlant_ConstP.Gain8_Gain[i] *
      rtb_Fcn2_g;
  }

  /* Product: '<S4>/Product' incorporates:
   *  Constant: '<S4>/gravity_effects'
   */
  for (i = 0; i < 3; i++) {
    rtb_Product8[i] = 0.0;
    rtb_Product8[i] += rtb_Gain[i] * 0.0;
    rtb_Product8[i] += rtb_Gain[i + 3] * 0.0;
    rtb_Product8[i] += rtb_Gain[i + 6] * 18.25054;
  }

  /* StateSpace: '<S7>/State-Space' */
  {
    rtb_Product[0] = (1.0)*QuadRotorPlant_X->StateSpace_CSTATE[0];
    rtb_Product[1] = (1.0)*QuadRotorPlant_X->StateSpace_CSTATE[1];
    rtb_Product[2] = (1.0)*QuadRotorPlant_X->StateSpace_CSTATE[2];
    rtb_Product[3] = (1.0)*QuadRotorPlant_X->StateSpace_CSTATE[3];
  }

  if (rtmIsMajorTimeStep(QuadRotorPlant_M)) {
    /* Gain: '<S28>/Output' incorporates:
     *  RandomNumber: '<S28>/White Noise'
     */
    QuadRotorPlant_B->Output[0] = 1.4051713699571909E-02 *
      QuadRotorPlant_DWork->NextOutput[0];
    QuadRotorPlant_B->Output[1] = 1.4051713699571909E-02 *
      QuadRotorPlant_DWork->NextOutput[1];
    QuadRotorPlant_B->Output[2] = 1.4051713699571909E-02 *
      QuadRotorPlant_DWork->NextOutput[2];
    QuadRotorPlant_B->Output[3] = 1.4051713699571909E-02 *
      QuadRotorPlant_DWork->NextOutput[3];
  }

  /* Sum: '<S7>/Sum' incorporates:
   *  Saturate: '<S7>/Saturation'
   */
  QuadRotorPlant_B->Sum[0] = rt_SATURATE(rtb_Product[0], 0.0, 9.8) +
    QuadRotorPlant_B->Output[0];
  QuadRotorPlant_B->Sum[1] = rt_SATURATE(rtb_Product[1], 0.0, 9.8) +
    QuadRotorPlant_B->Output[1];
  QuadRotorPlant_B->Sum[2] = rt_SATURATE(rtb_Product[2], 0.0, 9.8) +
    QuadRotorPlant_B->Output[2];
  QuadRotorPlant_B->Sum[3] = rt_SATURATE(rtb_Product[3], 0.0, 9.8) +
    QuadRotorPlant_B->Output[3];

  /* RateTransition: '<S6>/Rate Transition' */
  if (rtmIsMajorTimeStep(QuadRotorPlant_M) &&
      (QuadRotorPlant_M->Timing.RateInteraction.TID1_3 == 1)) {
    QuadRotorPlant_B->RateTransition[0] =
      QuadRotorPlant_DWork->RateTransition_Buffer0[0];
    QuadRotorPlant_B->RateTransition[1] =
      QuadRotorPlant_DWork->RateTransition_Buffer0[1];
    QuadRotorPlant_B->RateTransition[2] =
      QuadRotorPlant_DWork->RateTransition_Buffer0[2];
    QuadRotorPlant_B->RateTransition[3] =
      QuadRotorPlant_DWork->RateTransition_Buffer0[3];
  }

  /* Product: '<S6>/Product' */
  rtb_Product[0] = QuadRotorPlant_B->Sum[0] * QuadRotorPlant_B->RateTransition[0];
  rtb_Product[1] = QuadRotorPlant_B->Sum[1] * QuadRotorPlant_B->RateTransition[1];
  rtb_Product[2] = QuadRotorPlant_B->Sum[2] * QuadRotorPlant_B->RateTransition[2];
  rtb_Product[3] = QuadRotorPlant_B->Sum[3] * QuadRotorPlant_B->RateTransition[3];

  /* Sum: '<S6>/Sum of Elements' */
  rtb_SumofElements = ((rtb_Product[0] + rtb_Product[1]) + rtb_Product[2]) +
    rtb_Product[3];

  /* Integrator: '<S9>/Velocity' */
  if (QuadRotorPlant_DWork->Velocity_IWORK.IcNeedsLoading) {
    QuadRotorPlant_X->Velocity_CSTATE[0] = QuadRotorPlant_ConstB.Constant_a[0];
    QuadRotorPlant_X->Velocity_CSTATE[1] = QuadRotorPlant_ConstB.Constant_a[1];
    QuadRotorPlant_X->Velocity_CSTATE[2] = QuadRotorPlant_ConstB.Constant_a[2];
    QuadRotorPlant_DWork->Velocity_IWORK.IcNeedsLoading = 0;
  }

  rtb_V[0] = QuadRotorPlant_X->Velocity_CSTATE[0];
  rtb_V[1] = QuadRotorPlant_X->Velocity_CSTATE[1];
  rtb_V[2] = QuadRotorPlant_X->Velocity_CSTATE[2];

  /* Sum: '<S4>/Sum1' incorporates:
   *  Constant: '<S1>/gravity_effects'
   *  Product: '<S4>/Product1'
   */
  for (i = 0; i < 3; i++) {
    rtb_Gain_0[i] = 0.0;
    rtb_Gain_0[i] += rtb_Gain[i] * 0.0;
    rtb_Gain_0[i] += rtb_Gain[i + 3] * 0.0;
    rtb_Gain_0[i] += rtb_Gain[i + 6] * 0.0;
    QuadRotorPlant_B->Sum1[i] = rtb_V[i] - rtb_Gain_0[i];
  }

  /* DotProduct: '<S17>/Dot Product' */
  rtb_Fcn_a = 0.0;
  for (i = 0; i < 3; i++) {
    /* Product: '<S6>/Product2' incorporates:
     *  Constant: '<S6>/Pick First 2 Elements'
     */
    rtb_unitvector[i] = 0.0;
    rtb_unitvector[i] += QuadRotorPlant_ConstP.PickFirst2Elements_Va[i] *
      QuadRotorPlant_B->Sum1[0];
    rtb_unitvector[i] += QuadRotorPlant_ConstP.PickFirst2Elements_Va[i + 3] *
      QuadRotorPlant_B->Sum1[1];
    rtb_unitvector[i] += QuadRotorPlant_ConstP.PickFirst2Elements_Va[i + 6] *
      QuadRotorPlant_B->Sum1[2];
    rtb_Fcn_a += rtb_unitvector[i] * rtb_unitvector[i];
  }

  /* Math: '<S17>/Math Function'
   *
   * About '<S17>/Math Function':
   *  Operator: sqrt
   */
  rtb_Fcn_ab_0 = rtb_Fcn_a < 0.0 ? -sqrt(fabs(rtb_Fcn_a)) : sqrt(rtb_Fcn_a);

  /* Product: '<S17>/Product4' incorporates:
   *  Constant: '<S17>/Constant'
   *  Gain: '<S17>/Flap Angle Gain'
   */
  rtb_flapangle = (real32_T)(0.009 * rtb_Fcn_ab_0);

  /* Trigonometry: '<S17>/Trigonometric Function1' */
  rtb_TrigonometricFunction1 = cos(rtb_flapangle);

  /* Trigonometry: '<S17>/Trigonometric Function' */
  rtb_Fcn1_f = sin(rtb_flapangle);

  /* Sum: '<S17>/Sum1' incorporates:
   *  Constant: '<S17>/minor_offset'
   */
  rtb_Fcn_ab_0 += 0.001;

  /* Product: '<S17>/Divide' */
  rtb_Fcn_a = rtb_unitvector[0] / rtb_Fcn_ab_0;

  /* Sum: '<S17>/Sum' incorporates:
   *  Constant: '<S17>/Body Z '
   *  Gain: '<S17>/Gain'
   *  Product: '<S17>/Product1'
   *  Product: '<S17>/Product3'
   */
  rtb_Sum_0 = (real_T)(real32_T)(-rtb_Fcn_a) * rtb_Fcn1_f + 0.0 *
    rtb_TrigonometricFunction1;

  /* Product: '<S6>/Product4' */
  rtb_Product_0 = rtb_SumofElements * rtb_Sum_0;

  /* Gain: '<S9>/Gain' incorporates:
   *  Gain: '<S4>/Viscous Drag'
   *  Sum: '<S4>/Sum'
   */
  QuadRotorPlant_B->Gain[0] = ((rtb_Product8[0] + rtb_Product_0) + (real_T)
    (real32_T)(-0.1 * QuadRotorPlant_B->Sum1[0])) * 5.3697041293024750E-01;
  rtb_Product4[0] = rtb_Product_0;
  rtb_Sum_idx = rtb_Sum_0;
  rtb_unitvector[0] = rtb_Fcn_a;

  /* Product: '<S17>/Divide' */
  rtb_Fcn_a = rtb_unitvector[1] / rtb_Fcn_ab_0;

  /* Sum: '<S17>/Sum' incorporates:
   *  Constant: '<S17>/Body Z '
   *  Gain: '<S17>/Gain'
   *  Product: '<S17>/Product1'
   *  Product: '<S17>/Product3'
   */
  rtb_Sum_0 = (real_T)(real32_T)(-rtb_Fcn_a) * rtb_Fcn1_f + 0.0 *
    rtb_TrigonometricFunction1;

  /* Product: '<S6>/Product4' */
  rtb_Product_0 = rtb_SumofElements * rtb_Sum_0;

  /* Gain: '<S9>/Gain' incorporates:
   *  Gain: '<S4>/Viscous Drag'
   *  Sum: '<S4>/Sum'
   */
  QuadRotorPlant_B->Gain[1] = ((rtb_Product8[1] + rtb_Product_0) + (real_T)
    (real32_T)(-0.1 * QuadRotorPlant_B->Sum1[1])) * 5.3697041293024750E-01;
  rtb_Product4[1] = rtb_Product_0;
  rtb_Sum_idx_0 = rtb_Sum_0;
  rtb_unitvector[1] = rtb_Fcn_a;

  /* Product: '<S17>/Divide' */
  rtb_Fcn_a = rtb_unitvector[2] / rtb_Fcn_ab_0;

  /* Sum: '<S17>/Sum' incorporates:
   *  Constant: '<S17>/Body Z '
   *  Gain: '<S17>/Gain'
   *  Product: '<S17>/Product1'
   *  Product: '<S17>/Product3'
   */
  rtb_Sum_0 = (real_T)(real32_T)(-rtb_Fcn_a) * rtb_Fcn1_f +
    (-rtb_TrigonometricFunction1);

  /* Product: '<S6>/Product4' */
  rtb_Product_0 = rtb_SumofElements * rtb_Sum_0;

  /* Gain: '<S9>/Gain' incorporates:
   *  Gain: '<S4>/Viscous Drag'
   *  Sum: '<S4>/Sum'
   */
  QuadRotorPlant_B->Gain[2] = ((rtb_Product8[2] + rtb_Product_0) + (real_T)
    (real32_T)(-0.1 * QuadRotorPlant_B->Sum1[2])) * 5.3697041293024750E-01;
  rtb_Product4[2] = rtb_Product_0;
  rtb_unitvector[2] = rtb_Fcn_a;

  /* Integrator: '<S10>/integrator' */
  if (QuadRotorPlant_DWork->integrator_IWORK_m.IcNeedsLoading) {
    QuadRotorPlant_X->integrator_CSTATE_n[0] = QuadRotorPlant_ConstB.Constant2[0];
    QuadRotorPlant_X->integrator_CSTATE_n[1] = QuadRotorPlant_ConstB.Constant2[1];
    QuadRotorPlant_X->integrator_CSTATE_n[2] = QuadRotorPlant_ConstB.Constant2[2];
    QuadRotorPlant_DWork->integrator_IWORK_m.IcNeedsLoading = 0;
  }

  QuadRotorPlant_B->integrator_f[0] = QuadRotorPlant_X->integrator_CSTATE_n[0];
  QuadRotorPlant_B->integrator_f[1] = QuadRotorPlant_X->integrator_CSTATE_n[1];
  QuadRotorPlant_B->integrator_f[2] = QuadRotorPlant_X->integrator_CSTATE_n[2];

  /* ZeroOrderHold: '<S2>/Zero-Order Hold' */
  if (rtmIsMajorTimeStep(QuadRotorPlant_M) &&
      (QuadRotorPlant_M->Timing.RateInteraction.TID0_2 == 1)) {
    QuadRotorPlant_B->ZeroOrderHold[0] = QuadRotorPlant_B->integrator[0];
    QuadRotorPlant_B->ZeroOrderHold[1] = QuadRotorPlant_B->integrator[1];
    QuadRotorPlant_B->ZeroOrderHold[2] = QuadRotorPlant_B->integrator[2];
    QuadRotorPlant_B->ZeroOrderHold[3] = QuadRotorPlant_B->Gain[0];
    QuadRotorPlant_B->ZeroOrderHold[4] = QuadRotorPlant_B->Gain[1];
    QuadRotorPlant_B->ZeroOrderHold[5] = QuadRotorPlant_B->Gain[2];
    QuadRotorPlant_B->ZeroOrderHold[6] = QuadRotorPlant_B->integrator_f[0];
    QuadRotorPlant_B->ZeroOrderHold[7] = QuadRotorPlant_B->integrator_f[1];
    QuadRotorPlant_B->ZeroOrderHold[8] = QuadRotorPlant_B->integrator_f[2];
  }

  /* RateTransition: '<S1>/Rate Transition2' */
  if (rtmIsMajorTimeStep(QuadRotorPlant_M) &&
      (QuadRotorPlant_M->Timing.RateInteraction.TID1_2 == 1)) {
    QuadRotorPlant_B->RateTransition2[0] =
      QuadRotorPlant_DWork->RateTransition2_Buffer0[0];
    QuadRotorPlant_B->RateTransition2[1] =
      QuadRotorPlant_DWork->RateTransition2_Buffer0[1];
    QuadRotorPlant_B->RateTransition2[2] =
      QuadRotorPlant_DWork->RateTransition2_Buffer0[2];
    QuadRotorPlant_B->RateTransition2[3] =
      QuadRotorPlant_DWork->RateTransition2_Buffer0[3];
  }

  /* Integrator: '<S8>/Position' */
  if (QuadRotorPlant_DWork->Position_IWORK.IcNeedsLoading) {
    QuadRotorPlant_X->Position_CSTATE[0] = QuadRotorPlant_ConstB.Constant_f[0];
    QuadRotorPlant_X->Position_CSTATE[1] = QuadRotorPlant_ConstB.Constant_f[1];
    QuadRotorPlant_X->Position_CSTATE[2] = QuadRotorPlant_ConstB.Constant_f[2];
    QuadRotorPlant_DWork->Position_IWORK.IcNeedsLoading = 0;
  }

  QuadRotorPlant_B->Position[0] = QuadRotorPlant_X->Position_CSTATE[0];
  QuadRotorPlant_B->Position[1] = QuadRotorPlant_X->Position_CSTATE[1];
  QuadRotorPlant_B->Position[2] = QuadRotorPlant_X->Position_CSTATE[2];

  /* Math: '<S8>/Math Function' */
  i = 0;
  for (tmp = 0; tmp < 3; tmp++) {
    rtb_Gain1[i] = rtb_Gain[tmp];
    rtb_Gain1[i + 1] = rtb_Gain[tmp + 3];
    rtb_Gain1[i + 2] = rtb_Gain[tmp + 6];
    i += 3;
  }

  i = 0;
  for (tmp = 0; tmp < 3; tmp++) {
    rtb_Gain[i] = rtb_Gain1[i];
    rtb_Gain[i + 1] = rtb_Gain1[i + 1];
    rtb_Gain[i + 2] = rtb_Gain1[i + 2];
    i += 3;
  }

  /* Product: '<S8>/Product2' */
  for (i = 0; i < 3; i++) {
    QuadRotorPlant_B->Product2[i] = 0.0;
    QuadRotorPlant_B->Product2[i] = rtb_Gain[i] * rtb_V[0] +
      QuadRotorPlant_B->Product2[i];
    QuadRotorPlant_B->Product2[i] = rtb_Gain[i + 3] * rtb_V[1] +
      QuadRotorPlant_B->Product2[i];
    QuadRotorPlant_B->Product2[i] = rtb_Gain[i + 6] * rtb_V[2] +
      QuadRotorPlant_B->Product2[i];
  }

  /* ZeroOrderHold: '<S3>/ZOH' */
  if (rtmIsMajorTimeStep(QuadRotorPlant_M) &&
      (QuadRotorPlant_M->Timing.RateInteraction.TID0_4 == 1)) {
    QuadRotorPlant_B->ZOH[0] = QuadRotorPlant_B->Position[0];
    QuadRotorPlant_B->ZOH[1] = QuadRotorPlant_B->Position[1];
    QuadRotorPlant_B->ZOH[2] = QuadRotorPlant_B->Position[2];
    QuadRotorPlant_B->ZOH[3] = QuadRotorPlant_B->Product2[0];
    QuadRotorPlant_B->ZOH[4] = QuadRotorPlant_B->Product2[1];
    QuadRotorPlant_B->ZOH[5] = QuadRotorPlant_B->Product2[2];
  }

  if (rtmIsMajorTimeStep(QuadRotorPlant_M)) {
    /* DataTypeConversion: '<S1>/dtc' */
    QuadRotorPlant_B->dtc[0] = QuadRotorPlant_B->RateTransition2[0];
    QuadRotorPlant_B->dtc[1] = QuadRotorPlant_B->RateTransition2[1];
    QuadRotorPlant_B->dtc[2] = QuadRotorPlant_B->RateTransition2[2];
    QuadRotorPlant_B->dtc[3] = QuadRotorPlant_B->RateTransition2[3];
  }

  /* Sum: '<S9>/Sum' incorporates:
   *  Fcn: '<S13>/Fcn'
   *  Fcn: '<S13>/Fcn1'
   *  Fcn: '<S13>/Fcn2'
   */
  QuadRotorPlant_B->Sum_e[0] = QuadRotorPlant_B->Gain[0] -
    (QuadRotorPlant_B->integrator_f[1] * rtb_V[2] -
     QuadRotorPlant_B->integrator_f[2] * rtb_V[1]);
  QuadRotorPlant_B->Sum_e[1] = QuadRotorPlant_B->Gain[1] -
    (QuadRotorPlant_B->integrator_f[2] * rtb_V[0] -
     QuadRotorPlant_B->integrator_f[0] * rtb_V[2]);
  tmp_0[0] = QuadRotorPlant_B->integrator_f[0];
  tmp_0[1] = QuadRotorPlant_B->integrator_f[1];
  tmp_0[2] = QuadRotorPlant_B->integrator_f[2];
  for (i = 0; i < 3; i++) {
    tmp_0[i + 3] = rtb_V[i];

    /* Product: '<S10>/Product1' incorporates:
     *  Constant: '<S10>/Constant1'
     */
    rtb_Sum1_m[i] = 0.0;
    rtb_Sum1_m[i] += QuadRotorPlant_ConstP.Constant1_Value[i] *
      QuadRotorPlant_B->integrator_f[0];
    rtb_Sum1_m[i] += QuadRotorPlant_ConstP.Constant1_Value[i + 3] *
      QuadRotorPlant_B->integrator_f[1];
    rtb_Sum1_m[i] += QuadRotorPlant_ConstP.Constant1_Value[i + 6] *
      QuadRotorPlant_B->integrator_f[2];
  }

  QuadRotorPlant_B->Sum_e[2] = QuadRotorPlant_B->Gain[2] -
    (QuadRotorPlant_B->integrator_f[0] * rtb_V[1] -
     QuadRotorPlant_B->integrator_f[1] * tmp_0[3]);

  /* Fcn: '<S14>/Fcn' */
  rtb_Fcn_ab = QuadRotorPlant_B->integrator_f[1] * rtb_Sum1_m[2] -
    QuadRotorPlant_B->integrator_f[2] * rtb_Sum1_m[1];

  /* Fcn: '<S14>/Fcn1' */
  rtb_Fcn1_f = QuadRotorPlant_B->integrator_f[2] * rtb_Sum1_m[0] -
    QuadRotorPlant_B->integrator_f[0] * rtb_Sum1_m[2];

  /* Fcn: '<S14>/Fcn2' */
  rtb_Fcn2_l = QuadRotorPlant_B->integrator_f[0] * rtb_Sum1_m[1] -
    QuadRotorPlant_B->integrator_f[1] * rtb_Sum1_m[0];

  /* Product: '<S16>/Product3' */
  rtb_Sum1_m[0] = rtb_Product[0] * rtb_Sum_idx;
  rtb_Sum1_m[1] = rtb_Product[0] * rtb_Sum_idx_0;
  rtb_Sum1_m[2] = rtb_Product[0] * rtb_Sum_0;

  /* Fcn: '<S19>/Fcn' incorporates:
   *  Constant: '<S16>/Moment Arm1'
   */
  rtb_Fcn_a = 0.0 * rtb_Sum1_m[2] - -0.07 * rtb_Sum1_m[1];

  /* Fcn: '<S19>/Fcn1' incorporates:
   *  Constant: '<S16>/Moment Arm1'
   */
  rtb_Fcn1_o = -0.07 * rtb_Sum1_m[0] - 0.3048 * rtb_Sum1_m[2];

  /* Fcn: '<S19>/Fcn2' incorporates:
   *  Constant: '<S16>/Moment Arm1'
   */
  rtb_Fcn2_g = 0.3048 * rtb_Sum1_m[1] - 0.0 * rtb_Sum1_m[0];

  /* Product: '<S16>/Product1' */
  rtb_Sum1_m[0] = rtb_Product[1] * rtb_Sum_idx;
  rtb_Sum1_m[1] = rtb_Product[1] * rtb_Sum_idx_0;
  rtb_Sum1_m[2] = rtb_Product[1] * rtb_Sum_0;

  /* Fcn: '<S20>/Fcn' incorporates:
   *  Constant: '<S16>/Moment Arm2'
   */
  rtb_Fcn_ab_0 = 0.3048 * rtb_Sum1_m[2] - -0.07 * rtb_Sum1_m[1];

  /* Fcn: '<S20>/Fcn1' incorporates:
   *  Constant: '<S16>/Moment Arm2'
   */
  rtb_TrigonometricFunction1 = -0.07 * rtb_Sum1_m[0] - 0.0 * rtb_Sum1_m[2];

  /* Fcn: '<S20>/Fcn2' incorporates:
   *  Constant: '<S16>/Moment Arm2'
   */
  rtb_SumofElements = 0.0 * rtb_Sum1_m[1] - 0.3048 * rtb_Sum1_m[0];

  /* Product: '<S16>/Product2' */
  rtb_Sum1_m[0] = rtb_Product[2] * rtb_Sum_idx;
  rtb_Sum1_m[1] = rtb_Product[2] * rtb_Sum_idx_0;
  rtb_Sum1_m[2] = rtb_Product[2] * rtb_Sum_0;

  /* Fcn: '<S21>/Fcn' incorporates:
   *  Constant: '<S16>/Moment Arm3'
   */
  rtb_Product_0 = 0.0 * rtb_Sum1_m[2] - -0.07 * rtb_Sum1_m[1];

  /* Fcn: '<S21>/Fcn1' incorporates:
   *  Constant: '<S16>/Moment Arm3'
   */
  rtb_Fcn1_e = -0.07 * rtb_Sum1_m[0] - -0.3048 * rtb_Sum1_m[2];

  /* Fcn: '<S21>/Fcn2' incorporates:
   *  Constant: '<S16>/Moment Arm3'
   */
  rtb_Fcn2_i = -0.3048 * rtb_Sum1_m[1] - 0.0 * rtb_Sum1_m[0];

  /* Product: '<S16>/Product4' */
  rtb_Sum1_m[0] = rtb_Product[3] * rtb_Sum_idx;
  rtb_Sum1_m[1] = rtb_Product[3] * rtb_Sum_idx_0;
  rtb_Sum1_m[2] = rtb_Product[3] * rtb_Sum_0;

  /* Gain: '<S23>/Gain1' */
  rtb_Gain1_f_idx = (real32_T)(0.47 * rtb_Product[0]);
  rtb_Gain1_f_idx_0 = (real32_T)(0.47 * rtb_Product[1]);
  rtb_Gain1_f_idx_1 = (real32_T)(0.47 * rtb_Product[2]);
  rtb_Gain1_f_idx_2 = (real32_T)(0.47 * rtb_Product[3]);

  /* Gain: '<S17>/Gain1' */
  rtb_Gain1_n = (real32_T)(0.23 * rtb_flapangle);

  /* Product: '<S10>/Product' incorporates:
   *  Constant: '<S10>/Constant'
   *  Constant: '<S16>/Moment Arm4'
   *  Fcn: '<S22>/Fcn'
   *  Fcn: '<S22>/Fcn1'
   *  Fcn: '<S22>/Fcn2'
   *  Gain: '<S16>/Motor 1'
   *  Gain: '<S16>/Motor 2'
   *  Gain: '<S16>/Motor 3'
   *  Gain: '<S16>/Motor 4'
   *  Gain: '<S17>/Gain2'
   *  Gain: '<S17>/rotate 90 degrees'
   *  Product: '<S16>/Product5'
   *  Product: '<S16>/Product6'
   *  Product: '<S16>/Product7'
   *  Product: '<S16>/Product8'
   *  Product: '<S17>/Product2'
   *  Sum: '<S10>/Sum1'
   *  Sum: '<S16>/Sum'
   *  Sum: '<S16>/Sum1'
   *  Sum: '<S16>/Sum2'
   *  Sum: '<S6>/Sum'
   */
  tmp_0[0] = 0.0;
  tmp_0[1] = -0.3048;
  tmp_0[2] = -0.07;
  for (i = 0; i < 3; i++) {
    tmp_0[i + 3] = rtb_Sum1_m[i];
    tmp_1[i] = 0.0F;
    tmp_1[i] += (real32_T)(QuadRotorPlant_ConstP.rotate90degrees_Gain[i] *
      rtb_unitvector[0]);
    tmp_1[i] += (real32_T)(QuadRotorPlant_ConstP.rotate90degrees_Gain[i + 3] *
      rtb_unitvector[1]);
    tmp_1[i] += (real32_T)(QuadRotorPlant_ConstP.rotate90degrees_Gain[i + 6] *
      rtb_unitvector[2]);
  }

  rtb_flapangle = (((real_T)((((real32_T)((real_T)rtb_Gain1_f_idx_0 *
    rtb_Sum_idx * -1.0) + (real32_T)((real_T)rtb_Gain1_f_idx * rtb_Sum_idx)) +
    (real32_T)((real_T)rtb_Gain1_f_idx_1 * rtb_Sum_idx)) + (real32_T)((real_T)
    rtb_Gain1_f_idx_2 * rtb_Sum_idx * -1.0)) + (((rtb_Fcn_a + rtb_Fcn_ab_0) +
    rtb_Product_0) + (-0.3048 * rtb_Sum1_m[2] - -0.07 * rtb_Sum1_m[1]))) +
                   (real_T)(rtb_Gain1_n * tmp_1[0] * 4.0F)) - rtb_Fcn_ab;
  rtb_Fcn_ab = (((real_T)((((real32_T)((real_T)rtb_Gain1_f_idx_0 * rtb_Sum_idx_0
    * -1.0) + (real32_T)((real_T)rtb_Gain1_f_idx * rtb_Sum_idx_0)) + (real32_T)
                   ((real_T)rtb_Gain1_f_idx_1 * rtb_Sum_idx_0)) + (real32_T)
    ((real_T)rtb_Gain1_f_idx_2 * rtb_Sum_idx_0 * -1.0)) + (((rtb_Fcn1_o +
    rtb_TrigonometricFunction1) + rtb_Fcn1_e) + (-0.07 * rtb_Sum1_m[0] - 0.0 *
    rtb_Sum1_m[2]))) + (real_T)(rtb_Gain1_n * tmp_1[1] * 4.0F)) - rtb_Fcn1_f;
  rtb_Fcn_a = (((real_T)((((real32_T)((real_T)rtb_Gain1_f_idx_0 * rtb_Sum_0 *
    -1.0) + (real32_T)((real_T)rtb_Gain1_f_idx * rtb_Sum_0)) + (real32_T)
    ((real_T)rtb_Gain1_f_idx_1 * rtb_Sum_0)) + (real32_T)((real_T)
    rtb_Gain1_f_idx_2 * rtb_Sum_0 * -1.0)) + (((rtb_Fcn2_g + rtb_SumofElements)
    + rtb_Fcn2_i) + (0.0 * rtb_Sum1_m[1] - -0.3048 * tmp_0[3]))) + (real_T)
               (rtb_Gain1_n * tmp_1[2] * 4.0F)) - rtb_Fcn2_l;

  /* Fcn: '<S15>/Fcn' */
  QuadRotorPlant_B->Fcn = (QuadRotorPlant_B->integrator_f[1] * sin
    (QuadRotorPlant_B->integrator[0]) * tan(QuadRotorPlant_B->integrator[1]) +
    QuadRotorPlant_B->integrator_f[0]) + QuadRotorPlant_B->integrator_f[2] * cos
    (QuadRotorPlant_B->integrator[0]) * tan(QuadRotorPlant_B->integrator[1]);

  /* Fcn: '<S15>/Fcn1' */
  QuadRotorPlant_B->Fcn1 = QuadRotorPlant_B->integrator_f[1] * cos
    (QuadRotorPlant_B->integrator[0]) - QuadRotorPlant_B->integrator_f[2] * sin
    (QuadRotorPlant_B->integrator[0]);

  /* Fcn: '<S15>/Fcn2' */
  QuadRotorPlant_B->Fcn2 = sin(QuadRotorPlant_B->integrator[0]) / cos
    (QuadRotorPlant_B->integrator[1]) * QuadRotorPlant_B->integrator_f[1] + cos
    (QuadRotorPlant_B->integrator[0]) / cos(QuadRotorPlant_B->integrator[1]) *
    QuadRotorPlant_B->integrator_f[2];

  /* Abs: '<S4>/Abs' incorporates:
   *  DotProduct: '<S4>/Dot Product'
   */
  rtb_Product_0 = 0.0;
  for (i = 0; i < 3; i++) {
    QuadRotorPlant_B->Product[i] = 0.0;
    QuadRotorPlant_B->Product[i] = QuadRotorPlant_ConstP.Constant_Value_e[i] *
      rtb_flapangle + QuadRotorPlant_B->Product[i];
    QuadRotorPlant_B->Product[i] = QuadRotorPlant_ConstP.Constant_Value_e[i + 3]
      * rtb_Fcn_ab + QuadRotorPlant_B->Product[i];
    QuadRotorPlant_B->Product[i] = QuadRotorPlant_ConstP.Constant_Value_e[i + 6]
      * rtb_Fcn_a + QuadRotorPlant_B->Product[i];
    rtb_Product_0 += rtb_Product4[i] * rtb_V[i];
  }

  QuadRotorPlant_B->Abs = fabs(rtb_Product_0);

  /* ZeroOrderHold: '<S24>/Zero-Order Hold2' */
  if (rtmIsMajorTimeStep(QuadRotorPlant_M) &&
      (QuadRotorPlant_M->Timing.RateInteraction.TID0_3 == 1)) {
    QuadRotorPlant_B->ZeroOrderHold2[0] = QuadRotorPlant_B->Sum1[0];
    QuadRotorPlant_B->ZeroOrderHold2[1] = QuadRotorPlant_B->Sum1[1];
    QuadRotorPlant_B->ZeroOrderHold2[2] = QuadRotorPlant_B->Sum1[2];
  }

  /* ZeroOrderHold: '<S24>/Zero-Order Hold1' */
  if (rtmIsMajorTimeStep(QuadRotorPlant_M) &&
      (QuadRotorPlant_M->Timing.RateInteraction.TID0_3 == 1)) {
    QuadRotorPlant_B->ZeroOrderHold1 = QuadRotorPlant_B->Sum[0];
  }

  /* ZeroOrderHold: '<S25>/Zero-Order Hold2' */
  if (rtmIsMajorTimeStep(QuadRotorPlant_M) &&
      (QuadRotorPlant_M->Timing.RateInteraction.TID0_3 == 1)) {
    QuadRotorPlant_B->ZeroOrderHold2_b[0] = QuadRotorPlant_B->Sum1[0];
    QuadRotorPlant_B->ZeroOrderHold2_b[1] = QuadRotorPlant_B->Sum1[1];
    QuadRotorPlant_B->ZeroOrderHold2_b[2] = QuadRotorPlant_B->Sum1[2];
  }

  /* ZeroOrderHold: '<S25>/Zero-Order Hold1' */
  if (rtmIsMajorTimeStep(QuadRotorPlant_M) &&
      (QuadRotorPlant_M->Timing.RateInteraction.TID0_3 == 1)) {
    QuadRotorPlant_B->ZeroOrderHold1_e = QuadRotorPlant_B->Sum[1];
  }

  /* ZeroOrderHold: '<S26>/Zero-Order Hold2' */
  if (rtmIsMajorTimeStep(QuadRotorPlant_M) &&
      (QuadRotorPlant_M->Timing.RateInteraction.TID0_3 == 1)) {
    QuadRotorPlant_B->ZeroOrderHold2_e[0] = QuadRotorPlant_B->Sum1[0];
    QuadRotorPlant_B->ZeroOrderHold2_e[1] = QuadRotorPlant_B->Sum1[1];
    QuadRotorPlant_B->ZeroOrderHold2_e[2] = QuadRotorPlant_B->Sum1[2];
  }

  /* ZeroOrderHold: '<S26>/Zero-Order Hold1' */
  if (rtmIsMajorTimeStep(QuadRotorPlant_M) &&
      (QuadRotorPlant_M->Timing.RateInteraction.TID0_3 == 1)) {
    QuadRotorPlant_B->ZeroOrderHold1_n = QuadRotorPlant_B->Sum[2];
  }

  /* ZeroOrderHold: '<S27>/Zero-Order Hold2' */
  if (rtmIsMajorTimeStep(QuadRotorPlant_M) &&
      (QuadRotorPlant_M->Timing.RateInteraction.TID0_3 == 1)) {
    QuadRotorPlant_B->ZeroOrderHold2_p[0] = QuadRotorPlant_B->Sum1[0];
    QuadRotorPlant_B->ZeroOrderHold2_p[1] = QuadRotorPlant_B->Sum1[1];
    QuadRotorPlant_B->ZeroOrderHold2_p[2] = QuadRotorPlant_B->Sum1[2];
  }

  /* ZeroOrderHold: '<S27>/Zero-Order Hold1' */
  if (rtmIsMajorTimeStep(QuadRotorPlant_M) &&
      (QuadRotorPlant_M->Timing.RateInteraction.TID0_3 == 1)) {
    QuadRotorPlant_B->ZeroOrderHold1_g = QuadRotorPlant_B->Sum[3];
  }

  if (rtmIsMajorTimeStep(QuadRotorPlant_M)) {
    if (rtmIsMajorTimeStep(QuadRotorPlant_M)) {
      /* Update for RandomNumber: '<S28>/White Noise' */
      QuadRotorPlant_DWork->NextOutput[0] = rt_NormalRand
        (&QuadRotorPlant_DWork->RandSeed[0]);
      QuadRotorPlant_DWork->NextOutput[1] = rt_NormalRand
        (&QuadRotorPlant_DWork->RandSeed[1]);
      QuadRotorPlant_DWork->NextOutput[2] = rt_NormalRand
        (&QuadRotorPlant_DWork->RandSeed[2]);
      QuadRotorPlant_DWork->NextOutput[3] = rt_NormalRand
        (&QuadRotorPlant_DWork->RandSeed[3]);
    }
  }                                    /* end MajorTimeStep */

  if (rtmIsMajorTimeStep(QuadRotorPlant_M)) {
    rt_ertODEUpdateContinuousStates(&QuadRotorPlant_M->solverInfo,
      QuadRotorPlant_M, QuadRotorPlant_B, QuadRotorPlant_DWork, QuadRotorPlant_X,
      QuadRotorPlant_U, QuadRotorPlant_Y);

    /* Update absolute time */
    /* The "clockTick0" counts the number of times the code of this task has
     * been executed. The absolute time is the multiplication of "clockTick0"
     * and "Timing.stepSize0". Size of "clockTick0" ensures timer will not
     * overflow during the application lifespan selected.
     * Timer of this task consists of two 32 bit unsigned integers.
     * The two integers represent the low bits Timing.clockTick0 and the high bits
     * Timing.clockTickH0. When the low bit overflows to 0, the high bits increment.
     */
    if (!(++QuadRotorPlant_M->Timing.clockTick0)) {
      ++QuadRotorPlant_M->Timing.clockTickH0;
    }

    QuadRotorPlant_M->Timing.t[0] = rtsiGetSolverStopTime
      (&QuadRotorPlant_M->solverInfo);

    /* Update absolute time */
    /* The "clockTick1" counts the number of times the code of this task has
     * been executed. The resolution of this integer timer is 0.01, which is the step size
     * of the task. Size of "clockTick1" ensures timer will not overflow during the
     * application lifespan selected.
     * Timer of this task consists of two 32 bit unsigned integers.
     * The two integers represent the low bits Timing.clockTick1 and the high bits
     * Timing.clockTickH1. When the low bit overflows to 0, the high bits increment.
     */
    QuadRotorPlant_M->Timing.clockTick1++;
    if (!QuadRotorPlant_M->Timing.clockTick1) {
      QuadRotorPlant_M->Timing.clockTickH1++;
    }
  }                                    /* end MajorTimeStep */
}

/* Derivatives for root system: '<Root>' */
void QuadRotorPlant_derivatives(RT_MODEL_QuadRotorPlant *const QuadRotorPlant_M,
  BlockIO_QuadRotorPlant *QuadRotorPlant_B, ContinuousStates_QuadRotorPlant
  *QuadRotorPlant_X)
{
  /* Derivatives for Integrator: '<S4>/Integrator' */
  ((StateDerivatives_QuadRotorPlant *) QuadRotorPlant_M->ModelData.derivs)
    ->Integrator_CSTATE = QuadRotorPlant_B->Abs;

  /* Derivatives for Integrator: '<S11>/integrator' */
  {
    ((StateDerivatives_QuadRotorPlant *) QuadRotorPlant_M->ModelData.derivs)
      ->integrator_CSTATE[0] = QuadRotorPlant_B->Fcn;
    ((StateDerivatives_QuadRotorPlant *) QuadRotorPlant_M->ModelData.derivs)
      ->integrator_CSTATE[1] = QuadRotorPlant_B->Fcn1;
    ((StateDerivatives_QuadRotorPlant *) QuadRotorPlant_M->ModelData.derivs)
      ->integrator_CSTATE[2] = QuadRotorPlant_B->Fcn2;
  }

  /* Derivatives for StateSpace: '<S7>/State-Space' */
  {
    ((StateDerivatives_QuadRotorPlant *) QuadRotorPlant_M->ModelData.derivs)
      ->StateSpace_CSTATE[0] = (-10.0)*QuadRotorPlant_X->StateSpace_CSTATE[0];
    ((StateDerivatives_QuadRotorPlant *) QuadRotorPlant_M->ModelData.derivs)
      ->StateSpace_CSTATE[0] += (10.0)*QuadRotorPlant_B->dtc[0];
    ((StateDerivatives_QuadRotorPlant *) QuadRotorPlant_M->ModelData.derivs)
      ->StateSpace_CSTATE[1] = (-10.0)*QuadRotorPlant_X->StateSpace_CSTATE[1];
    ((StateDerivatives_QuadRotorPlant *) QuadRotorPlant_M->ModelData.derivs)
      ->StateSpace_CSTATE[1] += (10.0)*QuadRotorPlant_B->dtc[1];
    ((StateDerivatives_QuadRotorPlant *) QuadRotorPlant_M->ModelData.derivs)
      ->StateSpace_CSTATE[2] = (-10.0)*QuadRotorPlant_X->StateSpace_CSTATE[2];
    ((StateDerivatives_QuadRotorPlant *) QuadRotorPlant_M->ModelData.derivs)
      ->StateSpace_CSTATE[2] += (10.0)*QuadRotorPlant_B->dtc[2];
    ((StateDerivatives_QuadRotorPlant *) QuadRotorPlant_M->ModelData.derivs)
      ->StateSpace_CSTATE[3] = (-10.0)*QuadRotorPlant_X->StateSpace_CSTATE[3];
    ((StateDerivatives_QuadRotorPlant *) QuadRotorPlant_M->ModelData.derivs)
      ->StateSpace_CSTATE[3] += (10.0)*QuadRotorPlant_B->dtc[3];
  }

  /* Derivatives for Integrator: '<S9>/Velocity' */
  {
    ((StateDerivatives_QuadRotorPlant *) QuadRotorPlant_M->ModelData.derivs)
      ->Velocity_CSTATE[0] = QuadRotorPlant_B->Sum_e[0];
    ((StateDerivatives_QuadRotorPlant *) QuadRotorPlant_M->ModelData.derivs)
      ->Velocity_CSTATE[1] = QuadRotorPlant_B->Sum_e[1];
    ((StateDerivatives_QuadRotorPlant *) QuadRotorPlant_M->ModelData.derivs)
      ->Velocity_CSTATE[2] = QuadRotorPlant_B->Sum_e[2];
  }

  /* Derivatives for Integrator: '<S10>/integrator' */
  {
    ((StateDerivatives_QuadRotorPlant *) QuadRotorPlant_M->ModelData.derivs)
      ->integrator_CSTATE_n[0] = QuadRotorPlant_B->Product[0];
    ((StateDerivatives_QuadRotorPlant *) QuadRotorPlant_M->ModelData.derivs)
      ->integrator_CSTATE_n[1] = QuadRotorPlant_B->Product[1];
    ((StateDerivatives_QuadRotorPlant *) QuadRotorPlant_M->ModelData.derivs)
      ->integrator_CSTATE_n[2] = QuadRotorPlant_B->Product[2];
  }

  /* Derivatives for Integrator: '<S8>/Position' */
  {
    ((StateDerivatives_QuadRotorPlant *) QuadRotorPlant_M->ModelData.derivs)
      ->Position_CSTATE[0] = QuadRotorPlant_B->Product2[0];
    ((StateDerivatives_QuadRotorPlant *) QuadRotorPlant_M->ModelData.derivs)
      ->Position_CSTATE[1] = QuadRotorPlant_B->Product2[1];
    ((StateDerivatives_QuadRotorPlant *) QuadRotorPlant_M->ModelData.derivs)
      ->Position_CSTATE[2] = QuadRotorPlant_B->Product2[2];
  }
}

/* Model step function for TID2 */
void QuadRotorPlant_step2(RT_MODEL_QuadRotorPlant *const QuadRotorPlant_M,
  BlockIO_QuadRotorPlant *QuadRotorPlant_B, D_Work_QuadRotorPlant
  *QuadRotorPlant_DWork, ExternalInputs_QuadRotorPlant *QuadRotorPlant_U,
  ExternalOutputs_QuadRotorPlant *QuadRotorPlant_Y) /* Sample time: [0.02s, 0.0s] */
{
  int32_T i;
  real32_T rtb_dtc;

  /* Update the flag to indicate when data transfers from
   *  Sample time: [0.02s, 0.0s] to Sample time: [0.2s, 0.0s]  */
  (QuadRotorPlant_M->Timing.RateInteraction.TID2_4)++;
  if ((QuadRotorPlant_M->Timing.RateInteraction.TID2_4) > 9) {
    QuadRotorPlant_M->Timing.RateInteraction.TID2_4 = 0;
  }

  /* Outport: '<Root>/Out4' incorporates:
   *  Gain: '<S2>/counts_per_radian'
   *  Quantizer: '<S2>/Quantizer'
   */
  QuadRotorPlant_Y->Out4[0] = (real32_T)((QuadRotorPlant_B->ZeroOrderHold[0] <
    0.0 ? (-floor(fabs(QuadRotorPlant_B->ZeroOrderHold[0] /
                       9.5873799242852573E-05) + 0.5)) * 9.5873799242852573E-05 :
    floor(fabs(QuadRotorPlant_B->ZeroOrderHold[0] / 9.5873799242852573E-05) +
          0.5) * 9.5873799242852573E-05) * 1.0430378350470453E+04);

  /* Outport: '<Root>/Out4' incorporates:
   *  Gain: '<S2>/counts_per_radian'
   *  Quantizer: '<S2>/Quantizer'
   */
  QuadRotorPlant_Y->Out4[1] = (real32_T)((QuadRotorPlant_B->ZeroOrderHold[1] <
    0.0 ? (-floor(fabs(QuadRotorPlant_B->ZeroOrderHold[1] /
                       9.5873799242852573E-05) + 0.5)) * 9.5873799242852573E-05 :
    floor(fabs(QuadRotorPlant_B->ZeroOrderHold[1] / 9.5873799242852573E-05) +
          0.5) * 9.5873799242852573E-05) * 1.0430378350470453E+04);

  /* Outport: '<Root>/Out4' incorporates:
   *  Gain: '<S2>/counts_per_radian'
   *  Quantizer: '<S2>/Quantizer'
   */
  QuadRotorPlant_Y->Out4[2] = (real32_T)((QuadRotorPlant_B->ZeroOrderHold[2] <
    0.0 ? (-floor(fabs(QuadRotorPlant_B->ZeroOrderHold[2] /
                       9.5873799242852573E-05) + 0.5)) * 9.5873799242852573E-05 :
    floor(fabs(QuadRotorPlant_B->ZeroOrderHold[2] / 9.5873799242852573E-05) +
          0.5) * 9.5873799242852573E-05) * 1.0430378350470453E+04);

  /* Gain: '<S2>/counts_per_accel' incorporates:
   *  Quantizer: '<S2>/Quantizer1'
   */
  rtb_dtc = (real32_T)((QuadRotorPlant_B->ZeroOrderHold[3] < 0.0 ? (-floor(fabs
    (QuadRotorPlant_B->ZeroOrderHold[3] / 1.4968872070312501E-03) + 0.5)) *
                        1.4968872070312501E-03 : floor(fabs
    (QuadRotorPlant_B->ZeroOrderHold[3] / 1.4968872070312501E-03) + 0.5) *
                        1.4968872070312501E-03) * 4.7718071938255423E+02);

  /* Outport: '<Root>/Out5' incorporates:
   *  Saturate: '<S2>/Saturation'
   */
  QuadRotorPlant_Y->Out5[0] = rt_SATURATE(rtb_dtc, -32768.0F, 32767.0F);

  /* Gain: '<S2>/counts_per_accel' incorporates:
   *  Quantizer: '<S2>/Quantizer1'
   */
  rtb_dtc = (real32_T)((QuadRotorPlant_B->ZeroOrderHold[4] < 0.0 ? (-floor(fabs
    (QuadRotorPlant_B->ZeroOrderHold[4] / 1.4968872070312501E-03) + 0.5)) *
                        1.4968872070312501E-03 : floor(fabs
    (QuadRotorPlant_B->ZeroOrderHold[4] / 1.4968872070312501E-03) + 0.5) *
                        1.4968872070312501E-03) * 4.7718071938255423E+02);

  /* Outport: '<Root>/Out5' incorporates:
   *  Saturate: '<S2>/Saturation'
   */
  QuadRotorPlant_Y->Out5[1] = rt_SATURATE(rtb_dtc, -32768.0F, 32767.0F);

  /* Gain: '<S2>/counts_per_accel' incorporates:
   *  Quantizer: '<S2>/Quantizer1'
   */
  rtb_dtc = (real32_T)((QuadRotorPlant_B->ZeroOrderHold[5] < 0.0 ? (-floor(fabs
    (QuadRotorPlant_B->ZeroOrderHold[5] / 1.4968872070312501E-03) + 0.5)) *
                        1.4968872070312501E-03 : floor(fabs
    (QuadRotorPlant_B->ZeroOrderHold[5] / 1.4968872070312501E-03) + 0.5) *
                        1.4968872070312501E-03) * 4.7718071938255423E+02);

  /* Outport: '<Root>/Out5' incorporates:
   *  Saturate: '<S2>/Saturation'
   */
  QuadRotorPlant_Y->Out5[2] = rt_SATURATE(rtb_dtc, -32768.0F, 32767.0F);

  /* Gain: '<S2>/counts_per_radian_p_sec' incorporates:
   *  Quantizer: '<S2>/Quantizer2'
   */
  rtb_dtc = (real32_T)((QuadRotorPlant_B->ZeroOrderHold[6] < 0.0 ? (-floor(fabs
    (QuadRotorPlant_B->ZeroOrderHold[6] / 1.5978966540475430E-04) + 0.5)) *
                        1.5978966540475430E-04 : floor(fabs
    (QuadRotorPlant_B->ZeroOrderHold[6] / 1.5978966540475430E-04) + 0.5) *
                        1.5978966540475430E-04) * 3.8550588235294117E+03);

  /* Outport: '<Root>/Out6' incorporates:
   *  Saturate: '<S2>/Saturation1'
   */
  QuadRotorPlant_Y->Out6[0] = rt_SATURATE(rtb_dtc, -32768.0F, 32767.0F);

  /* Gain: '<S2>/counts_per_radian_p_sec' incorporates:
   *  Quantizer: '<S2>/Quantizer2'
   */
  rtb_dtc = (real32_T)((QuadRotorPlant_B->ZeroOrderHold[7] < 0.0 ? (-floor(fabs
    (QuadRotorPlant_B->ZeroOrderHold[7] / 1.5978966540475430E-04) + 0.5)) *
                        1.5978966540475430E-04 : floor(fabs
    (QuadRotorPlant_B->ZeroOrderHold[7] / 1.5978966540475430E-04) + 0.5) *
                        1.5978966540475430E-04) * 3.8550588235294117E+03);

  /* Outport: '<Root>/Out6' incorporates:
   *  Saturate: '<S2>/Saturation1'
   */
  QuadRotorPlant_Y->Out6[1] = rt_SATURATE(rtb_dtc, -32768.0F, 32767.0F);

  /* Gain: '<S2>/counts_per_radian_p_sec' incorporates:
   *  Quantizer: '<S2>/Quantizer2'
   */
  rtb_dtc = (real32_T)((QuadRotorPlant_B->ZeroOrderHold[8] < 0.0 ? (-floor(fabs
    (QuadRotorPlant_B->ZeroOrderHold[8] / 1.5978966540475430E-04) + 0.5)) *
                        1.5978966540475430E-04 : floor(fabs
    (QuadRotorPlant_B->ZeroOrderHold[8] / 1.5978966540475430E-04) + 0.5) *
                        1.5978966540475430E-04) * 3.8550588235294117E+03);

  /* Outport: '<Root>/Out6' incorporates:
   *  Saturate: '<S2>/Saturation1'
   */
  QuadRotorPlant_Y->Out6[2] = rt_SATURATE(rtb_dtc, -32768.0F, 32767.0F);

  /* RateTransition: '<S3>/Rate Transition' */
  if ((QuadRotorPlant_M->Timing.RateInteraction.TID2_4 == 1)) {
    for (i = 0; i < 6; i++) {
      QuadRotorPlant_B->RateTransition_b[i] =
        QuadRotorPlant_DWork->RateTransition_Buffer0_e[i];
    }
  }

  /* Outport: '<Root>/Out7' incorporates:
   *  DataTypeConversion: '<S3>/dtc1'
   */
  QuadRotorPlant_Y->Out7[0] = (real32_T)QuadRotorPlant_B->RateTransition_b[0];

  /* Outport: '<Root>/Out8' incorporates:
   *  DataTypeConversion: '<S3>/dtc2'
   */
  QuadRotorPlant_Y->Out8[0] = (real32_T)QuadRotorPlant_B->RateTransition_b[3];

  /* Outport: '<Root>/Out7' incorporates:
   *  DataTypeConversion: '<S3>/dtc1'
   */
  QuadRotorPlant_Y->Out7[1] = (real32_T)QuadRotorPlant_B->RateTransition_b[1];

  /* Outport: '<Root>/Out8' incorporates:
   *  DataTypeConversion: '<S3>/dtc2'
   */
  QuadRotorPlant_Y->Out8[1] = (real32_T)QuadRotorPlant_B->RateTransition_b[4];

  /* Outport: '<Root>/Out7' incorporates:
   *  DataTypeConversion: '<S3>/dtc1'
   */
  QuadRotorPlant_Y->Out7[2] = (real32_T)QuadRotorPlant_B->RateTransition_b[2];

  /* Outport: '<Root>/Out8' incorporates:
   *  DataTypeConversion: '<S3>/dtc2'
   */
  QuadRotorPlant_Y->Out8[2] = (real32_T)QuadRotorPlant_B->RateTransition_b[5];
  QuadRotorPlant_DWork->RateTransition2_Buffer0[0] = QuadRotorPlant_U->In1[0];
  QuadRotorPlant_DWork->RateTransition2_Buffer0[1] = QuadRotorPlant_U->In1[1];
  QuadRotorPlant_DWork->RateTransition2_Buffer0[2] = QuadRotorPlant_U->In1[2];
  QuadRotorPlant_DWork->RateTransition2_Buffer0[3] = QuadRotorPlant_U->In1[3];
}

/* Model step function for TID3 */
void QuadRotorPlant_step3(BlockIO_QuadRotorPlant *QuadRotorPlant_B,
  D_Work_QuadRotorPlant *QuadRotorPlant_DWork) /* Sample time: [0.1s, 0.0s] */
{
  /* local block i/o variables */
  real_T rtb_v_inf;
  real_T rtb_alpha;
  real_T rtb_Saturation1;
  real_T rtb_v_inf_k;
  real_T rtb_alpha_m;
  real_T rtb_Saturation1_b;
  real_T rtb_v_inf_h;
  real_T rtb_alpha_o;
  real_T rtb_Saturation1_d;
  real_T rtb_v_inf_n;
  real_T rtb_alpha_f;
  real_T rtb_Saturation1_c;
  real_T rtb_v_iLookupTablenD;
  real_T rtb_v_iLookupTablenD_f;
  real_T rtb_v_iLookupTablenD_h;
  real_T rtb_v_iLookupTablenD_p;
  real_T rtb_v_h;
  real_T rtb_v_h_j;
  real_T rtb_v_h_o;
  real_T rtb_v_h_d;
  real_T rtb_Switch_0;

  /* DotProduct: '<S24>/Dot Product' */
  rtb_Switch_0 = (QuadRotorPlant_B->ZeroOrderHold2[0] *
                  QuadRotorPlant_B->ZeroOrderHold2[0] +
                  QuadRotorPlant_B->ZeroOrderHold2[1] *
                  QuadRotorPlant_B->ZeroOrderHold2[1]) +
    QuadRotorPlant_B->ZeroOrderHold2[2] * QuadRotorPlant_B->ZeroOrderHold2[2];

  /* Math: '<S24>/Math Function'
   *
   * About '<S24>/Math Function':
   *  Operator: sqrt
   */
  rtb_v_inf = rtb_Switch_0 < 0.0 ? -sqrt(fabs(rtb_Switch_0)) : sqrt(rtb_Switch_0);

  /* Product: '<S24>/Divide' incorporates:
   *  Saturate: '<S24>/Saturation'
   *  UnaryMinus: '<S24>/Unary Minus'
   */
  rtb_Switch_0 = (-QuadRotorPlant_B->ZeroOrderHold2[2]) / rt_SATURATE(rtb_v_inf,
    1.0E-10, 5.0E+05);

  /* Trigonometry: '<S24>/Trigonometric Function' */
  rtb_alpha = asin(rt_SATURATE(rtb_Switch_0, -1.0, 1.0));

  /* Switch: '<S24>/Switch' incorporates:
   *  Abs: '<S24>/Abs'
   *  Constant: '<S24>/Constant'
   */
  if (fabs(rtb_alpha) >= 1.2) {
    rtb_Switch_0 = 1.0;
  } else {
    /* Fcn: '<S24>/v_h ' */
    rtb_Switch_0 = QuadRotorPlant_B->ZeroOrderHold1 / 1.2160979498339947E-01;
    if (rtb_Switch_0 < 0.0) {
      rtb_v_h_d = -sqrt(-rtb_Switch_0);
    } else {
      rtb_v_h_d = sqrt(rtb_Switch_0);
    }

    /* Lookup_n-D: '<S24>/v_i Lookup Table (n-D)'
     * About '<S24>/v_i Lookup Table (n-D)':
     *       Table size:  35 x 13 x 26
     *    Interpolation:  Spline
     *    Extrapolation:  Linear
     *   Breakpt Search:  Binary
     *    Breakpt Cache:  ON
     */
    {
      real_T rt_LUTuVect[3];
      rt_LUTuVect[0] = rtb_alpha;
      rt_LUTuVect[1] = rtb_v_inf;
      rt_LUTuVect[2] = rtb_v_h_d;
      rtb_v_iLookupTablenD_p = look_SplNBinXSd(3U, rt_LUTuVect,
        (rt_LUTSplineWork*)&QuadRotorPlant_DWork->SWork_d[0]);
    }

    rtb_Switch_0 = rtb_v_iLookupTablenD_p;
  }

  /* Saturate: '<S24>/Saturation1' */
  rtb_Saturation1 = rt_SATURATE(rtb_Switch_0, 0.5, 2.0);

  /* DotProduct: '<S25>/Dot Product' */
  rtb_Switch_0 = (QuadRotorPlant_B->ZeroOrderHold2_b[0] *
                  QuadRotorPlant_B->ZeroOrderHold2_b[0] +
                  QuadRotorPlant_B->ZeroOrderHold2_b[1] *
                  QuadRotorPlant_B->ZeroOrderHold2_b[1]) +
    QuadRotorPlant_B->ZeroOrderHold2_b[2] * QuadRotorPlant_B->ZeroOrderHold2_b[2];

  /* Math: '<S25>/Math Function'
   *
   * About '<S25>/Math Function':
   *  Operator: sqrt
   */
  rtb_v_inf_k = rtb_Switch_0 < 0.0 ? -sqrt(fabs(rtb_Switch_0)) : sqrt
    (rtb_Switch_0);

  /* Product: '<S25>/Divide' incorporates:
   *  Saturate: '<S25>/Saturation'
   *  UnaryMinus: '<S25>/Unary Minus'
   */
  rtb_Switch_0 = (-QuadRotorPlant_B->ZeroOrderHold2_b[2]) / rt_SATURATE
    (rtb_v_inf_k, 1.0E-10, 5.0E+05);

  /* Trigonometry: '<S25>/Trigonometric Function' */
  rtb_alpha_m = asin(rt_SATURATE(rtb_Switch_0, -1.0, 1.0));

  /* Switch: '<S25>/Switch' incorporates:
   *  Abs: '<S25>/Abs'
   *  Constant: '<S25>/Constant'
   */
  if (fabs(rtb_alpha_m) >= 1.2) {
    rtb_Switch_0 = 1.0;
  } else {
    /* Fcn: '<S25>/v_h ' */
    rtb_Switch_0 = QuadRotorPlant_B->ZeroOrderHold1_e / 1.2160979498339947E-01;
    if (rtb_Switch_0 < 0.0) {
      rtb_v_h_o = -sqrt(-rtb_Switch_0);
    } else {
      rtb_v_h_o = sqrt(rtb_Switch_0);
    }

    /* Lookup_n-D: '<S25>/v_i Lookup Table (n-D)'
     * About '<S25>/v_i Lookup Table (n-D)':
     *       Table size:  35 x 13 x 26
     *    Interpolation:  Spline
     *    Extrapolation:  Linear
     *   Breakpt Search:  Binary
     *    Breakpt Cache:  ON
     */
    {
      real_T rt_LUTuVect[3];
      rt_LUTuVect[0] = rtb_alpha_m;
      rt_LUTuVect[1] = rtb_v_inf_k;
      rt_LUTuVect[2] = rtb_v_h_o;
      rtb_v_iLookupTablenD_h = look_SplNBinXSd(3U, rt_LUTuVect,
        (rt_LUTSplineWork*)&QuadRotorPlant_DWork->SWork_o[0]);
    }

    rtb_Switch_0 = rtb_v_iLookupTablenD_h;
  }

  /* Saturate: '<S25>/Saturation1' */
  rtb_Saturation1_b = rt_SATURATE(rtb_Switch_0, 0.5, 2.0);

  /* DotProduct: '<S26>/Dot Product' */
  rtb_Switch_0 = (QuadRotorPlant_B->ZeroOrderHold2_e[0] *
                  QuadRotorPlant_B->ZeroOrderHold2_e[0] +
                  QuadRotorPlant_B->ZeroOrderHold2_e[1] *
                  QuadRotorPlant_B->ZeroOrderHold2_e[1]) +
    QuadRotorPlant_B->ZeroOrderHold2_e[2] * QuadRotorPlant_B->ZeroOrderHold2_e[2];

  /* Math: '<S26>/Math Function'
   *
   * About '<S26>/Math Function':
   *  Operator: sqrt
   */
  rtb_v_inf_h = rtb_Switch_0 < 0.0 ? -sqrt(fabs(rtb_Switch_0)) : sqrt
    (rtb_Switch_0);

  /* Product: '<S26>/Divide' incorporates:
   *  Saturate: '<S26>/Saturation'
   *  UnaryMinus: '<S26>/Unary Minus'
   */
  rtb_Switch_0 = (-QuadRotorPlant_B->ZeroOrderHold2_e[2]) / rt_SATURATE
    (rtb_v_inf_h, 1.0E-10, 5.0E+05);

  /* Trigonometry: '<S26>/Trigonometric Function' */
  rtb_alpha_o = asin(rt_SATURATE(rtb_Switch_0, -1.0, 1.0));

  /* Switch: '<S26>/Switch' incorporates:
   *  Abs: '<S26>/Abs'
   *  Constant: '<S26>/Constant'
   */
  if (fabs(rtb_alpha_o) >= 1.2) {
    rtb_Switch_0 = 1.0;
  } else {
    /* Fcn: '<S26>/v_h ' */
    rtb_Switch_0 = QuadRotorPlant_B->ZeroOrderHold1_n / 1.2160979498339947E-01;
    if (rtb_Switch_0 < 0.0) {
      rtb_v_h_j = -sqrt(-rtb_Switch_0);
    } else {
      rtb_v_h_j = sqrt(rtb_Switch_0);
    }

    /* Lookup_n-D: '<S26>/v_i Lookup Table (n-D)'
     * About '<S26>/v_i Lookup Table (n-D)':
     *       Table size:  35 x 13 x 26
     *    Interpolation:  Spline
     *    Extrapolation:  Linear
     *   Breakpt Search:  Binary
     *    Breakpt Cache:  ON
     */
    {
      real_T rt_LUTuVect[3];
      rt_LUTuVect[0] = rtb_alpha_o;
      rt_LUTuVect[1] = rtb_v_inf_h;
      rt_LUTuVect[2] = rtb_v_h_j;
      rtb_v_iLookupTablenD_f = look_SplNBinXSd(3U, rt_LUTuVect,
        (rt_LUTSplineWork*)&QuadRotorPlant_DWork->SWork_l[0]);
    }

    rtb_Switch_0 = rtb_v_iLookupTablenD_f;
  }

  /* Saturate: '<S26>/Saturation1' */
  rtb_Saturation1_d = rt_SATURATE(rtb_Switch_0, 0.5, 2.0);

  /* DotProduct: '<S27>/Dot Product' */
  rtb_Switch_0 = (QuadRotorPlant_B->ZeroOrderHold2_p[0] *
                  QuadRotorPlant_B->ZeroOrderHold2_p[0] +
                  QuadRotorPlant_B->ZeroOrderHold2_p[1] *
                  QuadRotorPlant_B->ZeroOrderHold2_p[1]) +
    QuadRotorPlant_B->ZeroOrderHold2_p[2] * QuadRotorPlant_B->ZeroOrderHold2_p[2];

  /* Math: '<S27>/Math Function'
   *
   * About '<S27>/Math Function':
   *  Operator: sqrt
   */
  rtb_v_inf_n = rtb_Switch_0 < 0.0 ? -sqrt(fabs(rtb_Switch_0)) : sqrt
    (rtb_Switch_0);

  /* Product: '<S27>/Divide' incorporates:
   *  Saturate: '<S27>/Saturation'
   *  UnaryMinus: '<S27>/Unary Minus'
   */
  rtb_Switch_0 = (-QuadRotorPlant_B->ZeroOrderHold2_p[2]) / rt_SATURATE
    (rtb_v_inf_n, 1.0E-10, 5.0E+05);

  /* Trigonometry: '<S27>/Trigonometric Function' */
  rtb_alpha_f = asin(rt_SATURATE(rtb_Switch_0, -1.0, 1.0));

  /* Switch: '<S27>/Switch' incorporates:
   *  Abs: '<S27>/Abs'
   *  Constant: '<S27>/Constant'
   */
  if (fabs(rtb_alpha_f) >= 1.2) {
    rtb_Switch_0 = 1.0;
  } else {
    /* Fcn: '<S27>/v_h ' */
    rtb_Switch_0 = QuadRotorPlant_B->ZeroOrderHold1_g / 1.2160979498339947E-01;
    if (rtb_Switch_0 < 0.0) {
      rtb_v_h = -sqrt(-rtb_Switch_0);
    } else {
      rtb_v_h = sqrt(rtb_Switch_0);
    }

    /* Lookup_n-D: '<S27>/v_i Lookup Table (n-D)'
     * About '<S27>/v_i Lookup Table (n-D)':
     *       Table size:  35 x 13 x 26
     *    Interpolation:  Spline
     *    Extrapolation:  Linear
     *   Breakpt Search:  Binary
     *    Breakpt Cache:  ON
     */
    {
      real_T rt_LUTuVect[3];
      rt_LUTuVect[0] = rtb_alpha_f;
      rt_LUTuVect[1] = rtb_v_inf_n;
      rt_LUTuVect[2] = rtb_v_h;
      rtb_v_iLookupTablenD = look_SplNBinXSd(3U, rt_LUTuVect, (rt_LUTSplineWork*)
        &QuadRotorPlant_DWork->SWork[0]);
    }

    rtb_Switch_0 = rtb_v_iLookupTablenD;
  }

  /* Saturate: '<S27>/Saturation1' */
  rtb_Saturation1_c = rt_SATURATE(rtb_Switch_0, 0.5, 2.0);
  QuadRotorPlant_DWork->RateTransition_Buffer0[0] = rtb_Saturation1;
  QuadRotorPlant_DWork->RateTransition_Buffer0[1] = rtb_Saturation1_b;
  QuadRotorPlant_DWork->RateTransition_Buffer0[2] = rtb_Saturation1_d;
  QuadRotorPlant_DWork->RateTransition_Buffer0[3] = rtb_Saturation1_c;
}

/* Model step function for TID4 */
void QuadRotorPlant_step4(BlockIO_QuadRotorPlant *QuadRotorPlant_B,
  D_Work_QuadRotorPlant *QuadRotorPlant_DWork) /* Sample time: [0.2s, 0.0s] */
{
  int32_T i;
  for (i = 0; i < 6; i++) {
    QuadRotorPlant_DWork->RateTransition_Buffer0_e[i] = QuadRotorPlant_B->ZOH[i];
  }
}

void QuadRotorPlant_step(int_T tid, RT_MODEL_QuadRotorPlant *const
  QuadRotorPlant_M, BlockIO_QuadRotorPlant *QuadRotorPlant_B,
  D_Work_QuadRotorPlant *QuadRotorPlant_DWork, ContinuousStates_QuadRotorPlant
  *QuadRotorPlant_X, ExternalInputs_QuadRotorPlant *QuadRotorPlant_U,
  ExternalOutputs_QuadRotorPlant *QuadRotorPlant_Y)
{
  switch (tid) {
   case 0 :
    QuadRotorPlant_step0(QuadRotorPlant_M, QuadRotorPlant_B,
                         QuadRotorPlant_DWork, QuadRotorPlant_X,
                         QuadRotorPlant_U, QuadRotorPlant_Y);
    break;

   case 2 :
    QuadRotorPlant_step2(QuadRotorPlant_M, QuadRotorPlant_B,
                         QuadRotorPlant_DWork, QuadRotorPlant_U,
                         QuadRotorPlant_Y);
    break;

   case 3 :
    QuadRotorPlant_step3(QuadRotorPlant_B, QuadRotorPlant_DWork);
    break;

   case 4 :
    QuadRotorPlant_step4(QuadRotorPlant_B, QuadRotorPlant_DWork);
    break;

   default :
    break;
  }
}

/* Model initialize function */
void QuadRotorPlant_initialize(RT_MODEL_QuadRotorPlant *const QuadRotorPlant_M,
  BlockIO_QuadRotorPlant *QuadRotorPlant_B, D_Work_QuadRotorPlant
  *QuadRotorPlant_DWork, ContinuousStates_QuadRotorPlant *QuadRotorPlant_X,
  ExternalInputs_QuadRotorPlant *QuadRotorPlant_U,
  ExternalOutputs_QuadRotorPlant *QuadRotorPlant_Y)
{
  /* Registration code */

  /* initialize real-time model */
  (void) memset((void *)QuadRotorPlant_M, 0,
                sizeof(RT_MODEL_QuadRotorPlant));
  (QuadRotorPlant_M)->Timing.TaskCounters.cLimit[0] = 1;
  (QuadRotorPlant_M)->Timing.TaskCounters.cLimit[1] = 1;
  (QuadRotorPlant_M)->Timing.TaskCounters.cLimit[2] = 2;
  (QuadRotorPlant_M)->Timing.TaskCounters.cLimit[3] = 10;
  (QuadRotorPlant_M)->Timing.TaskCounters.cLimit[4] = 20;

  {
    /* Setup solver object */
    rtsiSetSimTimeStepPtr(&QuadRotorPlant_M->solverInfo,
                          &QuadRotorPlant_M->Timing.simTimeStep);
    rtsiSetTPtr(&QuadRotorPlant_M->solverInfo, &rtmGetTPtr(QuadRotorPlant_M));
    rtsiSetStepSizePtr(&QuadRotorPlant_M->solverInfo,
                       &QuadRotorPlant_M->Timing.stepSize0);
    rtsiSetdXPtr(&QuadRotorPlant_M->solverInfo,
                 &QuadRotorPlant_M->ModelData.derivs);
    rtsiSetContStatesPtr(&QuadRotorPlant_M->solverInfo,
                         &QuadRotorPlant_M->ModelData.contStates);
    rtsiSetNumContStatesPtr(&QuadRotorPlant_M->solverInfo,
      &QuadRotorPlant_M->Sizes.numContStates);
    rtsiSetErrorStatusPtr(&QuadRotorPlant_M->solverInfo, (&rtmGetErrorStatus
      (QuadRotorPlant_M)));
    rtsiSetRTModelPtr(&QuadRotorPlant_M->solverInfo, QuadRotorPlant_M);
  }

  rtsiSetSimTimeStep(&QuadRotorPlant_M->solverInfo, MAJOR_TIME_STEP);
  QuadRotorPlant_M->ModelData.intgData.deltaY=
    QuadRotorPlant_M->ModelData.OdeDeltaY;
  QuadRotorPlant_M->ModelData.intgData.f[0] = QuadRotorPlant_M->ModelData.odeF[0];
  QuadRotorPlant_M->ModelData.intgData.f[1] = QuadRotorPlant_M->ModelData.odeF[1];
  QuadRotorPlant_M->ModelData.intgData.f[2] = QuadRotorPlant_M->ModelData.odeF[2];
  QuadRotorPlant_M->ModelData.intgData.f[3] = QuadRotorPlant_M->ModelData.odeF[3];
  QuadRotorPlant_M->ModelData.intgData.f[4] = QuadRotorPlant_M->ModelData.odeF[4];
  QuadRotorPlant_M->ModelData.intgData.f[5] = QuadRotorPlant_M->ModelData.odeF[5];
  QuadRotorPlant_M->ModelData.intgData.f[6] = QuadRotorPlant_M->ModelData.odeF[6];
  QuadRotorPlant_M->ModelData.intgData.f[7] = QuadRotorPlant_M->ModelData.odeF[7];
  QuadRotorPlant_M->ModelData.intgData.f[8] = QuadRotorPlant_M->ModelData.odeF[8];
  QuadRotorPlant_M->ModelData.intgData.f[9] = QuadRotorPlant_M->ModelData.odeF[9];
  QuadRotorPlant_M->ModelData.intgData.f[10] = QuadRotorPlant_M->ModelData.odeF
    [10];
  QuadRotorPlant_M->ModelData.intgData.f[11] = QuadRotorPlant_M->ModelData.odeF
    [11];
  QuadRotorPlant_M->ModelData.intgData.f[12] = QuadRotorPlant_M->ModelData.odeF
    [12];
  QuadRotorPlant_M->ModelData.intgData.x0 = QuadRotorPlant_M->ModelData.odeX0;
  QuadRotorPlant_M->ModelData.contStates = ((real_T *) QuadRotorPlant_X);
  rtsiSetSolverData(&QuadRotorPlant_M->solverInfo, (void *)
                    &QuadRotorPlant_M->ModelData.intgData);
  rtsiSetSolverName(&QuadRotorPlant_M->solverInfo,"ode8");
  rtmSetTPtr(QuadRotorPlant_M, &QuadRotorPlant_M->Timing.tArray[0]);
  QuadRotorPlant_M->Timing.stepSize0 = 0.01;
  rtmSetFirstInitCond(QuadRotorPlant_M, 1);

  /* block I/O */
  {
    int_T i;
    for (i = 0; i < 9; i++) {
      QuadRotorPlant_B->ZeroOrderHold[i] = 0.0;
    }

    for (i = 0; i < 6; i++) {
      QuadRotorPlant_B->RateTransition_b[i] = 0.0;
    }

    for (i = 0; i < 6; i++) {
      QuadRotorPlant_B->ZOH[i] = 0.0;
    }

    QuadRotorPlant_B->integrator[0] = 0.0;
    QuadRotorPlant_B->integrator[1] = 0.0;
    QuadRotorPlant_B->integrator[2] = 0.0;
    QuadRotorPlant_B->Output[0] = 0.0;
    QuadRotorPlant_B->Output[1] = 0.0;
    QuadRotorPlant_B->Output[2] = 0.0;
    QuadRotorPlant_B->Output[3] = 0.0;
    QuadRotorPlant_B->Sum[0] = 0.0;
    QuadRotorPlant_B->Sum[1] = 0.0;
    QuadRotorPlant_B->Sum[2] = 0.0;
    QuadRotorPlant_B->Sum[3] = 0.0;
    QuadRotorPlant_B->RateTransition[0] = 0.0;
    QuadRotorPlant_B->RateTransition[1] = 0.0;
    QuadRotorPlant_B->RateTransition[2] = 0.0;
    QuadRotorPlant_B->RateTransition[3] = 0.0;
    QuadRotorPlant_B->Sum1[0] = 0.0;
    QuadRotorPlant_B->Sum1[1] = 0.0;
    QuadRotorPlant_B->Sum1[2] = 0.0;
    QuadRotorPlant_B->Gain[0] = 0.0;
    QuadRotorPlant_B->Gain[1] = 0.0;
    QuadRotorPlant_B->Gain[2] = 0.0;
    QuadRotorPlant_B->integrator_f[0] = 0.0;
    QuadRotorPlant_B->integrator_f[1] = 0.0;
    QuadRotorPlant_B->integrator_f[2] = 0.0;
    QuadRotorPlant_B->Position[0] = 0.0;
    QuadRotorPlant_B->Position[1] = 0.0;
    QuadRotorPlant_B->Position[2] = 0.0;
    QuadRotorPlant_B->Product2[0] = 0.0;
    QuadRotorPlant_B->Product2[1] = 0.0;
    QuadRotorPlant_B->Product2[2] = 0.0;
    QuadRotorPlant_B->dtc[0] = 0.0;
    QuadRotorPlant_B->dtc[1] = 0.0;
    QuadRotorPlant_B->dtc[2] = 0.0;
    QuadRotorPlant_B->dtc[3] = 0.0;
    QuadRotorPlant_B->Sum_e[0] = 0.0;
    QuadRotorPlant_B->Sum_e[1] = 0.0;
    QuadRotorPlant_B->Sum_e[2] = 0.0;
    QuadRotorPlant_B->Product[0] = 0.0;
    QuadRotorPlant_B->Product[1] = 0.0;
    QuadRotorPlant_B->Product[2] = 0.0;
    QuadRotorPlant_B->Fcn = 0.0;
    QuadRotorPlant_B->Fcn1 = 0.0;
    QuadRotorPlant_B->Fcn2 = 0.0;
    QuadRotorPlant_B->Abs = 0.0;
    QuadRotorPlant_B->ZeroOrderHold2[0] = 0.0;
    QuadRotorPlant_B->ZeroOrderHold2[1] = 0.0;
    QuadRotorPlant_B->ZeroOrderHold2[2] = 0.0;
    QuadRotorPlant_B->ZeroOrderHold1 = 0.0;
    QuadRotorPlant_B->ZeroOrderHold2_b[0] = 0.0;
    QuadRotorPlant_B->ZeroOrderHold2_b[1] = 0.0;
    QuadRotorPlant_B->ZeroOrderHold2_b[2] = 0.0;
    QuadRotorPlant_B->ZeroOrderHold1_e = 0.0;
    QuadRotorPlant_B->ZeroOrderHold2_e[0] = 0.0;
    QuadRotorPlant_B->ZeroOrderHold2_e[1] = 0.0;
    QuadRotorPlant_B->ZeroOrderHold2_e[2] = 0.0;
    QuadRotorPlant_B->ZeroOrderHold1_n = 0.0;
    QuadRotorPlant_B->ZeroOrderHold2_p[0] = 0.0;
    QuadRotorPlant_B->ZeroOrderHold2_p[1] = 0.0;
    QuadRotorPlant_B->ZeroOrderHold2_p[2] = 0.0;
    QuadRotorPlant_B->ZeroOrderHold1_g = 0.0;
    QuadRotorPlant_B->RateTransition2[0] = 0.0F;
    QuadRotorPlant_B->RateTransition2[1] = 0.0F;
    QuadRotorPlant_B->RateTransition2[2] = 0.0F;
    QuadRotorPlant_B->RateTransition2[3] = 0.0F;
  }

  /* states (continuous) */
  {
    (void) memset((void *)QuadRotorPlant_X, 0,
                  sizeof(ContinuousStates_QuadRotorPlant));
  }

  /* states (dwork) */
  (void) memset((void *)QuadRotorPlant_DWork, 0,
                sizeof(D_Work_QuadRotorPlant));
  QuadRotorPlant_DWork->NextOutput[0] = 0.0;
  QuadRotorPlant_DWork->NextOutput[1] = 0.0;
  QuadRotorPlant_DWork->NextOutput[2] = 0.0;
  QuadRotorPlant_DWork->NextOutput[3] = 0.0;
  QuadRotorPlant_DWork->RateTransition_Buffer0[0] = 0.0;
  QuadRotorPlant_DWork->RateTransition_Buffer0[1] = 0.0;
  QuadRotorPlant_DWork->RateTransition_Buffer0[2] = 0.0;
  QuadRotorPlant_DWork->RateTransition_Buffer0[3] = 0.0;

  {
    int_T i;
    for (i = 0; i < 6; i++) {
      QuadRotorPlant_DWork->RateTransition_Buffer0_e[i] = 0.0;
    }
  }

  QuadRotorPlant_DWork->m_bpLambda[0] = 0.0;
  QuadRotorPlant_DWork->m_bpLambda[1] = 0.0;
  QuadRotorPlant_DWork->m_bpLambda[2] = 0.0;

  {
    int_T i;
    for (i = 0; i < 338; i++) {
      QuadRotorPlant_DWork->m_yyA[i] = 0.0;
    }
  }

  {
    int_T i;
    for (i = 0; i < 338; i++) {
      QuadRotorPlant_DWork->m_yyB[i] = 0.0;
    }
  }

  {
    int_T i;
    for (i = 0; i < 338; i++) {
      QuadRotorPlant_DWork->m_yy2[i] = 0.0;
    }
  }

  {
    int_T i;
    for (i = 0; i < 35; i++) {
      QuadRotorPlant_DWork->m_up[i] = 0.0;
    }
  }

  {
    int_T i;
    for (i = 0; i < 11830; i++) {
      QuadRotorPlant_DWork->m_y2[i] = 0.0;
    }
  }

  QuadRotorPlant_DWork->m_bpLambda_e[0] = 0.0;
  QuadRotorPlant_DWork->m_bpLambda_e[1] = 0.0;
  QuadRotorPlant_DWork->m_bpLambda_e[2] = 0.0;

  {
    int_T i;
    for (i = 0; i < 338; i++) {
      QuadRotorPlant_DWork->m_yyA_j[i] = 0.0;
    }
  }

  {
    int_T i;
    for (i = 0; i < 338; i++) {
      QuadRotorPlant_DWork->m_yyB_c[i] = 0.0;
    }
  }

  {
    int_T i;
    for (i = 0; i < 338; i++) {
      QuadRotorPlant_DWork->m_yy2_p[i] = 0.0;
    }
  }

  {
    int_T i;
    for (i = 0; i < 35; i++) {
      QuadRotorPlant_DWork->m_up_k[i] = 0.0;
    }
  }

  {
    int_T i;
    for (i = 0; i < 11830; i++) {
      QuadRotorPlant_DWork->m_y2_o[i] = 0.0;
    }
  }

  QuadRotorPlant_DWork->m_bpLambda_c[0] = 0.0;
  QuadRotorPlant_DWork->m_bpLambda_c[1] = 0.0;
  QuadRotorPlant_DWork->m_bpLambda_c[2] = 0.0;

  {
    int_T i;
    for (i = 0; i < 338; i++) {
      QuadRotorPlant_DWork->m_yyA_h[i] = 0.0;
    }
  }

  {
    int_T i;
    for (i = 0; i < 338; i++) {
      QuadRotorPlant_DWork->m_yyB_a[i] = 0.0;
    }
  }

  {
    int_T i;
    for (i = 0; i < 338; i++) {
      QuadRotorPlant_DWork->m_yy2_a[i] = 0.0;
    }
  }

  {
    int_T i;
    for (i = 0; i < 35; i++) {
      QuadRotorPlant_DWork->m_up_l[i] = 0.0;
    }
  }

  {
    int_T i;
    for (i = 0; i < 11830; i++) {
      QuadRotorPlant_DWork->m_y2_e[i] = 0.0;
    }
  }

  QuadRotorPlant_DWork->m_bpLambda_g[0] = 0.0;
  QuadRotorPlant_DWork->m_bpLambda_g[1] = 0.0;
  QuadRotorPlant_DWork->m_bpLambda_g[2] = 0.0;

  {
    int_T i;
    for (i = 0; i < 338; i++) {
      QuadRotorPlant_DWork->m_yyA_hq[i] = 0.0;
    }
  }

  {
    int_T i;
    for (i = 0; i < 338; i++) {
      QuadRotorPlant_DWork->m_yyB_g[i] = 0.0;
    }
  }

  {
    int_T i;
    for (i = 0; i < 338; i++) {
      QuadRotorPlant_DWork->m_yy2_g[i] = 0.0;
    }
  }

  {
    int_T i;
    for (i = 0; i < 35; i++) {
      QuadRotorPlant_DWork->m_up_p[i] = 0.0;
    }
  }

  {
    int_T i;
    for (i = 0; i < 11830; i++) {
      QuadRotorPlant_DWork->m_y2_g[i] = 0.0;
    }
  }

  QuadRotorPlant_DWork->RateTransition2_Buffer0[0] = 0.0F;
  QuadRotorPlant_DWork->RateTransition2_Buffer0[1] = 0.0F;
  QuadRotorPlant_DWork->RateTransition2_Buffer0[2] = 0.0F;
  QuadRotorPlant_DWork->RateTransition2_Buffer0[3] = 0.0F;

  /* external inputs */
  QuadRotorPlant_U->In1[0] = 0.0F;
  QuadRotorPlant_U->In1[1] = 0.0F;
  QuadRotorPlant_U->In1[2] = 0.0F;
  QuadRotorPlant_U->In1[3] = 0.0F;

  /* external outputs */
  QuadRotorPlant_Y->Out3 = 0.0;
  QuadRotorPlant_Y->Out4[0] = 0.0F;
  QuadRotorPlant_Y->Out4[1] = 0.0F;
  QuadRotorPlant_Y->Out4[2] = 0.0F;
  QuadRotorPlant_Y->Out5[0] = 0.0F;
  QuadRotorPlant_Y->Out5[1] = 0.0F;
  QuadRotorPlant_Y->Out5[2] = 0.0F;
  QuadRotorPlant_Y->Out6[0] = 0.0F;
  QuadRotorPlant_Y->Out6[1] = 0.0F;
  QuadRotorPlant_Y->Out6[2] = 0.0F;
  QuadRotorPlant_Y->Out7[0] = 0.0F;
  QuadRotorPlant_Y->Out7[1] = 0.0F;
  QuadRotorPlant_Y->Out7[2] = 0.0F;
  QuadRotorPlant_Y->Out8[0] = 0.0F;
  QuadRotorPlant_Y->Out8[1] = 0.0F;
  QuadRotorPlant_Y->Out8[2] = 0.0F;

  {
    uint32_T tseed;
    int32_T r;

    /* Start for RandomNumber: '<S28>/White Noise' */
    r = (int32_T)(22341U >> 16U);
    tseed = ((22341U - ((uint32_T)r << 16)) << 16U) + (uint32_T)r;
    if (tseed < 1U) {
      tseed = 1144108930U;
    } else {
      if (tseed > 2147483646U) {
        tseed = 2147483646U;
      }
    }

    QuadRotorPlant_DWork->RandSeed[0] = tseed;
    QuadRotorPlant_DWork->NextOutput[0] = rt_NormalRand
      (&QuadRotorPlant_DWork->RandSeed[0]);
    r = (int32_T)(22342U >> 16U);
    tseed = ((22342U - ((uint32_T)r << 16)) << 16U) + (uint32_T)r;
    if (tseed < 1U) {
      tseed = 1144108930U;
    } else {
      if (tseed > 2147483646U) {
        tseed = 2147483646U;
      }
    }

    QuadRotorPlant_DWork->RandSeed[1] = tseed;
    QuadRotorPlant_DWork->NextOutput[1] = rt_NormalRand
      (&QuadRotorPlant_DWork->RandSeed[1]);
    r = (int32_T)(22343U >> 16U);
    tseed = ((22343U - ((uint32_T)r << 16)) << 16U) + (uint32_T)r;
    if (tseed < 1U) {
      tseed = 1144108930U;
    } else {
      if (tseed > 2147483646U) {
        tseed = 2147483646U;
      }
    }

    QuadRotorPlant_DWork->RandSeed[2] = tseed;
    QuadRotorPlant_DWork->NextOutput[2] = rt_NormalRand
      (&QuadRotorPlant_DWork->RandSeed[2]);
    r = (int32_T)(22344U >> 16U);
    tseed = ((22344U - ((uint32_T)r << 16)) << 16U) + (uint32_T)r;
    if (tseed < 1U) {
      tseed = 1144108930U;
    } else {
      if (tseed > 2147483646U) {
        tseed = 2147483646U;
      }
    }

    QuadRotorPlant_DWork->RandSeed[3] = tseed;
    QuadRotorPlant_DWork->NextOutput[3] = rt_NormalRand
      (&QuadRotorPlant_DWork->RandSeed[3]);

    /* Start for Lookup_n-D: '<S24>/v_i Lookup Table (n-D)' */
    {
      rt_LUTnWork *TWork_start = (rt_LUTnWork *) &QuadRotorPlant_DWork->TWork_o
        [0];
      void **bpDataSet = (void **) &QuadRotorPlant_DWork->m_bpDataSet_e[0];
      TWork_start->m_dimSizes = (const uint32_T *)
        QuadRotorPlant_ConstP.pooled17;
      TWork_start->m_tableData = (void *) QuadRotorPlant_ConstP.pooled1;
      TWork_start->m_bpDataSet = bpDataSet;
      TWork_start->m_bpIndex = &QuadRotorPlant_DWork->m_bpIndex_b[0];
      TWork_start->m_bpLambda = (void *) &QuadRotorPlant_DWork->m_bpLambda_g[0];
      TWork_start->m_maxIndex = (const uint32_T *)
        QuadRotorPlant_ConstP.pooled16;
      bpDataSet[0] = (void *)QuadRotorPlant_ConstP.pooled2;
      bpDataSet[1] = (void *)QuadRotorPlant_ConstP.pooled3;
      bpDataSet[2] = (void *)QuadRotorPlant_ConstP.pooled4;
    }

    {
      rt_LUTSplineWork *rt_SplWk = (rt_LUTSplineWork*)
        &QuadRotorPlant_DWork->SWork_d[0];
      rt_SplWk->m_TWork = (rt_LUTnWork*)&QuadRotorPlant_DWork->TWork_o[0];
      rt_SplWk->m_yyA = &QuadRotorPlant_DWork->m_yyA_hq[0];
      rt_SplWk->m_yyB = &QuadRotorPlant_DWork->m_yyB_g[0];
      rt_SplWk->m_yy2 = &QuadRotorPlant_DWork->m_yy2_g[0];
      rt_SplWk->m_up = &QuadRotorPlant_DWork->m_up_p[0];
      rt_SplWk->m_y2 = &QuadRotorPlant_DWork->m_y2_g[0];
      rt_SplWk->m_numYWorkElts = QuadRotorPlant_ConstP.pooled18;
    }

    /* Start for Lookup_n-D: '<S25>/v_i Lookup Table (n-D)' */
    {
      rt_LUTnWork *TWork_start = (rt_LUTnWork *) &QuadRotorPlant_DWork->TWork_d
        [0];
      void **bpDataSet = (void **) &QuadRotorPlant_DWork->m_bpDataSet_c[0];
      TWork_start->m_dimSizes = (const uint32_T *)
        QuadRotorPlant_ConstP.pooled17;
      TWork_start->m_tableData = (void *) QuadRotorPlant_ConstP.pooled1;
      TWork_start->m_bpDataSet = bpDataSet;
      TWork_start->m_bpIndex = &QuadRotorPlant_DWork->m_bpIndex_p[0];
      TWork_start->m_bpLambda = (void *) &QuadRotorPlant_DWork->m_bpLambda_c[0];
      TWork_start->m_maxIndex = (const uint32_T *)
        QuadRotorPlant_ConstP.pooled16;
      bpDataSet[0] = (void *)QuadRotorPlant_ConstP.pooled2;
      bpDataSet[1] = (void *)QuadRotorPlant_ConstP.pooled3;
      bpDataSet[2] = (void *)QuadRotorPlant_ConstP.pooled4;
    }

    {
      rt_LUTSplineWork *rt_SplWk = (rt_LUTSplineWork*)
        &QuadRotorPlant_DWork->SWork_o[0];
      rt_SplWk->m_TWork = (rt_LUTnWork*)&QuadRotorPlant_DWork->TWork_d[0];
      rt_SplWk->m_yyA = &QuadRotorPlant_DWork->m_yyA_h[0];
      rt_SplWk->m_yyB = &QuadRotorPlant_DWork->m_yyB_a[0];
      rt_SplWk->m_yy2 = &QuadRotorPlant_DWork->m_yy2_a[0];
      rt_SplWk->m_up = &QuadRotorPlant_DWork->m_up_l[0];
      rt_SplWk->m_y2 = &QuadRotorPlant_DWork->m_y2_e[0];
      rt_SplWk->m_numYWorkElts = QuadRotorPlant_ConstP.pooled18;
    }

    /* Start for Lookup_n-D: '<S26>/v_i Lookup Table (n-D)' */
    {
      rt_LUTnWork *TWork_start = (rt_LUTnWork *) &QuadRotorPlant_DWork->TWork_c
        [0];
      void **bpDataSet = (void **) &QuadRotorPlant_DWork->m_bpDataSet_m[0];
      TWork_start->m_dimSizes = (const uint32_T *)
        QuadRotorPlant_ConstP.pooled17;
      TWork_start->m_tableData = (void *) QuadRotorPlant_ConstP.pooled1;
      TWork_start->m_bpDataSet = bpDataSet;
      TWork_start->m_bpIndex = &QuadRotorPlant_DWork->m_bpIndex_j[0];
      TWork_start->m_bpLambda = (void *) &QuadRotorPlant_DWork->m_bpLambda_e[0];
      TWork_start->m_maxIndex = (const uint32_T *)
        QuadRotorPlant_ConstP.pooled16;
      bpDataSet[0] = (void *)QuadRotorPlant_ConstP.pooled2;
      bpDataSet[1] = (void *)QuadRotorPlant_ConstP.pooled3;
      bpDataSet[2] = (void *)QuadRotorPlant_ConstP.pooled4;
    }

    {
      rt_LUTSplineWork *rt_SplWk = (rt_LUTSplineWork*)
        &QuadRotorPlant_DWork->SWork_l[0];
      rt_SplWk->m_TWork = (rt_LUTnWork*)&QuadRotorPlant_DWork->TWork_c[0];
      rt_SplWk->m_yyA = &QuadRotorPlant_DWork->m_yyA_j[0];
      rt_SplWk->m_yyB = &QuadRotorPlant_DWork->m_yyB_c[0];
      rt_SplWk->m_yy2 = &QuadRotorPlant_DWork->m_yy2_p[0];
      rt_SplWk->m_up = &QuadRotorPlant_DWork->m_up_k[0];
      rt_SplWk->m_y2 = &QuadRotorPlant_DWork->m_y2_o[0];
      rt_SplWk->m_numYWorkElts = QuadRotorPlant_ConstP.pooled18;
    }

    /* Start for Lookup_n-D: '<S27>/v_i Lookup Table (n-D)' */
    {
      rt_LUTnWork *TWork_start = (rt_LUTnWork *) &QuadRotorPlant_DWork->TWork[0];
      void **bpDataSet = (void **) &QuadRotorPlant_DWork->m_bpDataSet[0];
      TWork_start->m_dimSizes = (const uint32_T *)
        QuadRotorPlant_ConstP.pooled17;
      TWork_start->m_tableData = (void *) QuadRotorPlant_ConstP.pooled1;
      TWork_start->m_bpDataSet = bpDataSet;
      TWork_start->m_bpIndex = &QuadRotorPlant_DWork->m_bpIndex[0];
      TWork_start->m_bpLambda = (void *) &QuadRotorPlant_DWork->m_bpLambda[0];
      TWork_start->m_maxIndex = (const uint32_T *)
        QuadRotorPlant_ConstP.pooled16;
      bpDataSet[0] = (void *)QuadRotorPlant_ConstP.pooled2;
      bpDataSet[1] = (void *)QuadRotorPlant_ConstP.pooled3;
      bpDataSet[2] = (void *)QuadRotorPlant_ConstP.pooled4;
    }

    {
      rt_LUTSplineWork *rt_SplWk = (rt_LUTSplineWork*)
        &QuadRotorPlant_DWork->SWork[0];
      rt_SplWk->m_TWork = (rt_LUTnWork*)&QuadRotorPlant_DWork->TWork[0];
      rt_SplWk->m_yyA = &QuadRotorPlant_DWork->m_yyA[0];
      rt_SplWk->m_yyB = &QuadRotorPlant_DWork->m_yyB[0];
      rt_SplWk->m_yy2 = &QuadRotorPlant_DWork->m_yy2[0];
      rt_SplWk->m_up = &QuadRotorPlant_DWork->m_up[0];
      rt_SplWk->m_y2 = &QuadRotorPlant_DWork->m_y2[0];
      rt_SplWk->m_numYWorkElts = QuadRotorPlant_ConstP.pooled18;
    }

    /* Switch Block: '<S7>/Switch'
     *
     * Regarding '<S7>/Switch':
     *   Eliminated due to constant selection input
     */

    /* RateTransition Block: '<S24>/Rate Transition'
     *
     * Regarding '<S24>/Rate Transition':
     *   Eliminated since input and output rates are identical
     */

    /* RateTransition Block: '<S25>/Rate Transition'
     *
     * Regarding '<S25>/Rate Transition':
     *   Eliminated since input and output rates are identical
     */

    /* RateTransition Block: '<S26>/Rate Transition'
     *
     * Regarding '<S26>/Rate Transition':
     *   Eliminated since input and output rates are identical
     */

    /* RateTransition Block: '<S27>/Rate Transition'
     *
     * Regarding '<S27>/Rate Transition':
     *   Eliminated since input and output rates are identical
     */
  }

  /* InitializeConditions for Integrator: '<S4>/Integrator' */
  QuadRotorPlant_X->Integrator_CSTATE = 0.0;

  /* InitializeConditions for Integrator: '<S11>/integrator' */
  if (rtmIsFirstInitCond(QuadRotorPlant_M)) {
    QuadRotorPlant_X->integrator_CSTATE[0] = 0.0;
    QuadRotorPlant_X->integrator_CSTATE[1] = 0.0;
    QuadRotorPlant_X->integrator_CSTATE[2] = 0.0;
  }

  QuadRotorPlant_DWork->integrator_IWORK.IcNeedsLoading = 1;

  /* InitializeConditions for StateSpace: '<S7>/State-Space' */
  QuadRotorPlant_X->StateSpace_CSTATE[0] = (4.4744350000000006E+00);
  QuadRotorPlant_X->StateSpace_CSTATE[1] = (4.4744350000000006E+00);
  QuadRotorPlant_X->StateSpace_CSTATE[2] = (4.4744350000000006E+00);
  QuadRotorPlant_X->StateSpace_CSTATE[3] = (4.4744350000000006E+00);

  /* InitializeConditions for Integrator: '<S9>/Velocity' */
  if (rtmIsFirstInitCond(QuadRotorPlant_M)) {
    QuadRotorPlant_X->Velocity_CSTATE[0] = 0.0;
    QuadRotorPlant_X->Velocity_CSTATE[1] = 0.0;
    QuadRotorPlant_X->Velocity_CSTATE[2] = 0.0;
  }

  QuadRotorPlant_DWork->Velocity_IWORK.IcNeedsLoading = 1;

  /* InitializeConditions for Integrator: '<S10>/integrator' */
  if (rtmIsFirstInitCond(QuadRotorPlant_M)) {
    QuadRotorPlant_X->integrator_CSTATE_n[0] = 0.0;
    QuadRotorPlant_X->integrator_CSTATE_n[1] = 0.0;
    QuadRotorPlant_X->integrator_CSTATE_n[2] = 0.0;
  }

  QuadRotorPlant_DWork->integrator_IWORK_m.IcNeedsLoading = 1;

  /* InitializeConditions for Integrator: '<S8>/Position' */
  if (rtmIsFirstInitCond(QuadRotorPlant_M)) {
    QuadRotorPlant_X->Position_CSTATE[0] = 0.1;
    QuadRotorPlant_X->Position_CSTATE[1] = 0.2;
    QuadRotorPlant_X->Position_CSTATE[2] = 0.0;
  }

  QuadRotorPlant_DWork->Position_IWORK.IcNeedsLoading = 1;

  /* set "at time zero" to false */
  if (rtmIsFirstInitCond(QuadRotorPlant_M)) {
    rtmSetFirstInitCond(QuadRotorPlant_M, 0);
  }
}

/* Model terminate function */
void QuadRotorPlant_terminate(void)
{
  /* (no terminate code required) */
}

/*
 * File trailer for Real-Time Workshop generated code.
 *
 * [EOF]
 */
