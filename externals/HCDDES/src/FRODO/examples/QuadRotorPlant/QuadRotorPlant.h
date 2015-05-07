/*
 * File: QuadRotorPlant.h
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

#ifndef RTW_HEADER_QuadRotorPlant_h_
#define RTW_HEADER_QuadRotorPlant_h_
#ifndef QuadRotorPlant_COMMON_INCLUDES_
#define QuadRotorPlant_COMMON_INCLUDES_
#include <math.h>
#include <string.h>
#include "rtwtypes.h"
#include "rtw_continuous.h"
#include "rtw_solver.h"
#include "rtsplntypes.h"
#include "rt_SATURATE.h"
#include "rt_rand.h"
#endif                                 /* QuadRotorPlant_COMMON_INCLUDES_ */

#include "QuadRotorPlant_types.h"

/* Macros for accessing real-time model data structure */
#ifndef rtmCounterLimit
# define rtmCounterLimit(rtm, idx)     ((rtm)->Timing.TaskCounters.cLimit[(idx)])
#endif

#ifndef rtmGetErrorStatus
# define rtmGetErrorStatus(rtm)        ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
# define rtmSetErrorStatus(rtm, val)   ((rtm)->errorStatus = (val))
#endif

#ifndef rtmStepTask
# define rtmStepTask(rtm, idx)         ((rtm)->Timing.TaskCounters.TID[(idx)] == 0)
#endif

#ifndef rtmGetStopRequested
# define rtmGetStopRequested(rtm)      ((rtm)->Timing.stopRequestedFlag)
#endif

#ifndef rtmSetStopRequested
# define rtmSetStopRequested(rtm, val) ((rtm)->Timing.stopRequestedFlag = (val))
#endif

#ifndef rtmGetStopRequestedPtr
# define rtmGetStopRequestedPtr(rtm)   (&((rtm)->Timing.stopRequestedFlag))
#endif

#ifndef rtmGetT
# define rtmGetT(rtm)                  (rtmGetTPtr((rtm))[0])
#endif

#ifndef rtmTaskCounter
# define rtmTaskCounter(rtm, idx)      ((rtm)->Timing.TaskCounters.TID[(idx)])
#endif

/* Block signals (auto storage) */
typedef struct {
  real_T integrator[3];                /* '<S11>/integrator' */
  real_T Output[4];                    /* '<S28>/Output' */
  real_T Sum[4];                       /* '<S7>/Sum' */
  real_T RateTransition[4];            /* '<S6>/Rate Transition' */
  real_T Sum1[3];                      /* '<S4>/Sum1' */
  real_T Gain[3];                      /* '<S9>/Gain' */
  real_T integrator_f[3];              /* '<S10>/integrator' */
  real_T ZeroOrderHold[9];             /* '<S2>/Zero-Order Hold' */
  real_T RateTransition_b[6];          /* '<S3>/Rate Transition' */
  real_T Position[3];                  /* '<S8>/Position' */
  real_T Product2[3];                  /* '<S8>/Product2' */
  real_T ZOH[6];                       /* '<S3>/ZOH' */
  real_T dtc[4];                       /* '<S1>/dtc' */
  real_T Sum_e[3];                     /* '<S9>/Sum' */
  real_T Product[3];                   /* '<S10>/Product' */
  real_T Fcn;                          /* '<S15>/Fcn' */
  real_T Fcn1;                         /* '<S15>/Fcn1' */
  real_T Fcn2;                         /* '<S15>/Fcn2' */
  real_T Abs;                          /* '<S4>/Abs' */
  real_T ZeroOrderHold2[3];            /* '<S24>/Zero-Order Hold2' */
  real_T ZeroOrderHold1;               /* '<S24>/Zero-Order Hold1' */
  real_T ZeroOrderHold2_b[3];          /* '<S25>/Zero-Order Hold2' */
  real_T ZeroOrderHold1_e;             /* '<S25>/Zero-Order Hold1' */
  real_T ZeroOrderHold2_e[3];          /* '<S26>/Zero-Order Hold2' */
  real_T ZeroOrderHold1_n;             /* '<S26>/Zero-Order Hold1' */
  real_T ZeroOrderHold2_p[3];          /* '<S27>/Zero-Order Hold2' */
  real_T ZeroOrderHold1_g;             /* '<S27>/Zero-Order Hold1' */
  real32_T RateTransition2[4];         /* '<S1>/Rate Transition2' */
} BlockIO_QuadRotorPlant;

/* Block states (auto storage) for system '<Root>' */
typedef struct {
  real_T NextOutput[4];                /* '<S28>/White Noise' */
  real_T RateTransition_Buffer0[4];    /* '<S6>/Rate Transition' */
  real_T RateTransition_Buffer0_e[6];  /* '<S3>/Rate Transition' */
  real_T m_bpLambda[3];                /* '<S27>/v_i Lookup Table (n-D)' */
  real_T m_yyA[338];                   /* '<S27>/v_i Lookup Table (n-D)' */
  real_T m_yyB[338];                   /* '<S27>/v_i Lookup Table (n-D)' */
  real_T m_yy2[338];                   /* '<S27>/v_i Lookup Table (n-D)' */
  real_T m_up[35];                     /* '<S27>/v_i Lookup Table (n-D)' */
  real_T m_y2[11830];                  /* '<S27>/v_i Lookup Table (n-D)' */
  real_T m_bpLambda_e[3];              /* '<S26>/v_i Lookup Table (n-D)' */
  real_T m_yyA_j[338];                 /* '<S26>/v_i Lookup Table (n-D)' */
  real_T m_yyB_c[338];                 /* '<S26>/v_i Lookup Table (n-D)' */
  real_T m_yy2_p[338];                 /* '<S26>/v_i Lookup Table (n-D)' */
  real_T m_up_k[35];                   /* '<S26>/v_i Lookup Table (n-D)' */
  real_T m_y2_o[11830];                /* '<S26>/v_i Lookup Table (n-D)' */
  real_T m_bpLambda_c[3];              /* '<S25>/v_i Lookup Table (n-D)' */
  real_T m_yyA_h[338];                 /* '<S25>/v_i Lookup Table (n-D)' */
  real_T m_yyB_a[338];                 /* '<S25>/v_i Lookup Table (n-D)' */
  real_T m_yy2_a[338];                 /* '<S25>/v_i Lookup Table (n-D)' */
  real_T m_up_l[35];                   /* '<S25>/v_i Lookup Table (n-D)' */
  real_T m_y2_e[11830];                /* '<S25>/v_i Lookup Table (n-D)' */
  real_T m_bpLambda_g[3];              /* '<S24>/v_i Lookup Table (n-D)' */
  real_T m_yyA_hq[338];                /* '<S24>/v_i Lookup Table (n-D)' */
  real_T m_yyB_g[338];                 /* '<S24>/v_i Lookup Table (n-D)' */
  real_T m_yy2_g[338];                 /* '<S24>/v_i Lookup Table (n-D)' */
  real_T m_up_p[35];                   /* '<S24>/v_i Lookup Table (n-D)' */
  real_T m_y2_g[11830];                /* '<S24>/v_i Lookup Table (n-D)' */
  void * m_bpDataSet[3];               /* '<S27>/v_i Lookup Table (n-D)' */
  void * TWork[6];                     /* '<S27>/v_i Lookup Table (n-D)' */
  void * SWork[7];                     /* '<S27>/v_i Lookup Table (n-D)' */
  void * m_bpDataSet_m[3];             /* '<S26>/v_i Lookup Table (n-D)' */
  void * TWork_c[6];                   /* '<S26>/v_i Lookup Table (n-D)' */
  void * SWork_l[7];                   /* '<S26>/v_i Lookup Table (n-D)' */
  void * m_bpDataSet_c[3];             /* '<S25>/v_i Lookup Table (n-D)' */
  void * TWork_d[6];                   /* '<S25>/v_i Lookup Table (n-D)' */
  void * SWork_o[7];                   /* '<S25>/v_i Lookup Table (n-D)' */
  void * m_bpDataSet_e[3];             /* '<S24>/v_i Lookup Table (n-D)' */
  void * TWork_o[6];                   /* '<S24>/v_i Lookup Table (n-D)' */
  void * SWork_d[7];                   /* '<S24>/v_i Lookup Table (n-D)' */
  real32_T RateTransition2_Buffer0[4]; /* '<S1>/Rate Transition2' */
  uint32_T RandSeed[4];                /* '<S28>/White Noise' */
  uint32_T m_bpIndex[3];               /* '<S27>/v_i Lookup Table (n-D)' */
  uint32_T m_bpIndex_j[3];             /* '<S26>/v_i Lookup Table (n-D)' */
  uint32_T m_bpIndex_p[3];             /* '<S25>/v_i Lookup Table (n-D)' */
  uint32_T m_bpIndex_b[3];             /* '<S24>/v_i Lookup Table (n-D)' */
  struct {
    int_T IcNeedsLoading;
  } integrator_IWORK;                  /* '<S11>/integrator' */

  struct {
    int_T IcNeedsLoading;
  } Velocity_IWORK;                    /* '<S9>/Velocity' */

  struct {
    int_T IcNeedsLoading;
  } integrator_IWORK_m;                /* '<S10>/integrator' */

  struct {
    int_T IcNeedsLoading;
  } Position_IWORK;                    /* '<S8>/Position' */
} D_Work_QuadRotorPlant;

/* Continuous states (auto storage) */
typedef struct {
  real_T Integrator_CSTATE;            /* '<S4>/Integrator' */
  real_T integrator_CSTATE[3];         /* '<S11>/integrator' */
  real_T StateSpace_CSTATE[4];         /* '<S7>/State-Space' */
  real_T Velocity_CSTATE[3];           /* '<S9>/Velocity' */
  real_T integrator_CSTATE_n[3];       /* '<S10>/integrator' */
  real_T Position_CSTATE[3];           /* '<S8>/Position' */
} ContinuousStates_QuadRotorPlant;

/* State derivatives (auto storage) */
typedef struct {
  real_T Integrator_CSTATE;            /* '<S4>/Integrator' */
  real_T integrator_CSTATE[3];         /* '<S11>/integrator' */
  real_T StateSpace_CSTATE[4];         /* '<S7>/State-Space' */
  real_T Velocity_CSTATE[3];           /* '<S9>/Velocity' */
  real_T integrator_CSTATE_n[3];       /* '<S10>/integrator' */
  real_T Position_CSTATE[3];           /* '<S8>/Position' */
} StateDerivatives_QuadRotorPlant;

/* State disabled  */
typedef struct {
  boolean_T Integrator_CSTATE;         /* '<S4>/Integrator' */
  boolean_T integrator_CSTATE[3];      /* '<S11>/integrator' */
  boolean_T StateSpace_CSTATE[4];      /* '<S7>/State-Space' */
  boolean_T Velocity_CSTATE[3];        /* '<S9>/Velocity' */
  boolean_T integrator_CSTATE_n[3];    /* '<S10>/integrator' */
  boolean_T Position_CSTATE[3];        /* '<S8>/Position' */
} StateDisabled_QuadRotorPlant;

/* Invariant block signals (auto storage) */
typedef struct {
  const real_T Constant[3];            /* '<S11>/Constant' */
  const real_T Constant_a[3];          /* '<S9>/Constant' */
  const real_T Constant2[3];           /* '<S10>/Constant2' */
  const real_T Constant_f[3];          /* '<S8>/Constant' */
} ConstBlockIO_QuadRotorPlant;

#ifndef ODE8_INTG
#define ODE8_INTG

/* ODE8 Integration Data */
typedef struct {
  real_T *deltaY;                      /* output diff */
  real_T *f[13];                       /* derivatives */
  real_T *x0;                          /* Initial State */
} ODE8_IntgData;

#endif

/* Constant parameters (auto storage) */
typedef struct {
  /* Pooled Parameter (Expression: ToverTh)
   * Referenced by:
   *   '<S24>/v_i Lookup Table (n-D)'
   *   '<S25>/v_i Lookup Table (n-D)'
   *   '<S26>/v_i Lookup Table (n-D)'
   *   '<S27>/v_i Lookup Table (n-D)'
   */
  real_T pooled1[11830];

  /* Pooled Parameter (Expression: Alpha)
   * Referenced by:
   *   '<S24>/v_i Lookup Table (n-D)'
   *   '<S25>/v_i Lookup Table (n-D)'
   *   '<S26>/v_i Lookup Table (n-D)'
   *   '<S27>/v_i Lookup Table (n-D)'
   */
  real_T pooled2[35];

  /* Pooled Parameter (Expression: Vinf)
   * Referenced by:
   *   '<S24>/v_i Lookup Table (n-D)'
   *   '<S25>/v_i Lookup Table (n-D)'
   *   '<S26>/v_i Lookup Table (n-D)'
   *   '<S27>/v_i Lookup Table (n-D)'
   */
  real_T pooled3[13];

  /* Pooled Parameter (Expression: Vh)
   * Referenced by:
   *   '<S24>/v_i Lookup Table (n-D)'
   *   '<S25>/v_i Lookup Table (n-D)'
   *   '<S26>/v_i Lookup Table (n-D)'
   *   '<S27>/v_i Lookup Table (n-D)'
   */
  real_T pooled4[26];

  /* Expression: [1 0 0;0 0 0;0 0 0]
   * Referenced by: '<S12>/Gain'
   */
  real_T Gain_Gain[9];

  /* Expression: [0 1 0;0 0 0;0 0 0]
   * Referenced by: '<S12>/Gain1'
   */
  real_T Gain1_Gain[9];

  /* Expression: [0 0 1;0 0 0;0 0 0]
   * Referenced by: '<S12>/Gain2'
   */
  real_T Gain2_Gain[9];

  /* Expression: [0 0 0;1 0 0;0 0 0]
   * Referenced by: '<S12>/Gain3'
   */
  real_T Gain3_Gain[9];

  /* Expression: [0 0 0;0 1 0;0 0 0]
   * Referenced by: '<S12>/Gain4'
   */
  real_T Gain4_Gain[9];

  /* Expression: [0 0 0;0 0 1;0 0 0]
   * Referenced by: '<S12>/Gain5'
   */
  real_T Gain5_Gain[9];

  /* Expression: [0 0 0;0 0 0;1 0 0]
   * Referenced by: '<S12>/Gain6'
   */
  real_T Gain6_Gain[9];

  /* Expression: [0 0 0;0 0 0;0 1 0]
   * Referenced by: '<S12>/Gain7'
   */
  real_T Gain7_Gain[9];

  /* Expression: [0 0 0;0 0 0;0 0 1]
   * Referenced by: '<S12>/Gain8'
   */
  real_T Gain8_Gain[9];

  /* Expression: [1 0 0; 0 1 0; 0 0 0]
   * Referenced by: '<S6>/Pick First 2 Elements'
   */
  real_T PickFirst2Elements_Va[9];

  /* Expression: inv(I)
   * Referenced by: '<S10>/Constant'
   */
  real_T Constant_Value_e[9];

  /* Expression: I
   * Referenced by: '<S10>/Constant1'
   */
  real_T Constant1_Value[9];

  /* Expression: [0 -1 0;1 0 0;0 0 1]
   * Referenced by: '<S17>/rotate 90 degrees'
   */
  real_T rotate90degrees_Gain[9];

  /* Pooled Parameter (Expression: )
   * Referenced by:
   *   '<S24>/v_i Lookup Table (n-D)'
   *   '<S25>/v_i Lookup Table (n-D)'
   *   '<S26>/v_i Lookup Table (n-D)'
   *   '<S27>/v_i Lookup Table (n-D)'
   */
  uint32_T pooled16[3];

  /* Pooled Parameter (Expression: )
   * Referenced by:
   *   '<S24>/v_i Lookup Table (n-D)'
   *   '<S25>/v_i Lookup Table (n-D)'
   *   '<S26>/v_i Lookup Table (n-D)'
   *   '<S27>/v_i Lookup Table (n-D)'
   */
  uint32_T pooled17[3];

  /* Pooled Parameter (Expression: )
   * Referenced by:
   *   '<S24>/v_i Lookup Table (n-D)'
   *   '<S25>/v_i Lookup Table (n-D)'
   *   '<S26>/v_i Lookup Table (n-D)'
   *   '<S27>/v_i Lookup Table (n-D)'
   */
  uint32_T pooled18[4];
} ConstParam_QuadRotorPlant;

/* External inputs (root inport signals with auto storage) */
typedef struct {
  real32_T In1[4];                     /* '<Root>/In1' */
} ExternalInputs_QuadRotorPlant;

/* External outputs (root outports fed by signals with auto storage) */
typedef struct {
  real_T Out3;                         /* '<Root>/Out3' */
  real32_T Out4[3];                    /* '<Root>/Out4' */
  real32_T Out5[3];                    /* '<Root>/Out5' */
  real32_T Out6[3];                    /* '<Root>/Out6' */
  real32_T Out7[3];                    /* '<Root>/Out7' */
  real32_T Out8[3];                    /* '<Root>/Out8' */
} ExternalOutputs_QuadRotorPlant;

/* Real-time Model Data Structure */
struct RT_MODEL_QuadRotorPlant {
  const char_T *errorStatus;
  RTWSolverInfo solverInfo;

  /*
   * ModelData:
   * The following substructure contains information regarding
   * the data used in the model.
   */
  struct {
    real_T *contStates;
    real_T *derivs;
    boolean_T *contStateDisabled;
    boolean_T zCCacheNeedsReset;
    boolean_T derivCacheNeedsReset;
    boolean_T blkStateChange;
    real_T OdeDeltaY[17];
    real_T odeF[13][17];
    real_T odeX0[17];
    ODE8_IntgData intgData;
  } ModelData;

  /*
   * Sizes:
   * The following substructure contains sizes information
   * for many of the model attributes such as inputs, outputs,
   * dwork, sample times, etc.
   */
  struct {
    int_T numContStates;
    int_T numSampTimes;
  } Sizes;

  /*
   * Timing:
   * The following substructure contains information regarding
   * the timing information for the model.
   */
  struct {
    uint32_T clockTick0;
    uint32_T clockTickH0;
    time_T stepSize0;
    uint32_T clockTick1;
    uint32_T clockTickH1;
    boolean_T firstInitCondFlag;
    struct {
      uint8_T TID[5];
      uint8_T cLimit[5];
    } TaskCounters;

    struct {
      uint8_T TID0_2;
      uint8_T TID0_3;
      uint8_T TID0_4;
      uint8_T TID1_2;
      uint8_T TID1_3;
      uint8_T TID2_4;
    } RateInteraction;

    SimTimeStep simTimeStep;
    boolean_T stopRequestedFlag;
    time_T *t;
    time_T tArray[5];
  } Timing;
};

extern ConstBlockIO_QuadRotorPlant QuadRotorPlant_ConstB;/* constant block i/o */

/* Constant parameters (auto storage) */
extern const ConstParam_QuadRotorPlant QuadRotorPlant_ConstP;

/* Model entry point functions */
extern void QuadRotorPlant_initialize(RT_MODEL_QuadRotorPlant *const
  QuadRotorPlant_M, BlockIO_QuadRotorPlant *QuadRotorPlant_B,
  D_Work_QuadRotorPlant *QuadRotorPlant_DWork, ContinuousStates_QuadRotorPlant
  *QuadRotorPlant_X, ExternalInputs_QuadRotorPlant *QuadRotorPlant_U,
  ExternalOutputs_QuadRotorPlant *QuadRotorPlant_Y);
extern void QuadRotorPlant_step(int_T tid, RT_MODEL_QuadRotorPlant *const
  QuadRotorPlant_M, BlockIO_QuadRotorPlant *QuadRotorPlant_B,
  D_Work_QuadRotorPlant *QuadRotorPlant_DWork, ContinuousStates_QuadRotorPlant
  *QuadRotorPlant_X, ExternalInputs_QuadRotorPlant *QuadRotorPlant_U,
  ExternalOutputs_QuadRotorPlant *QuadRotorPlant_Y);
extern void QuadRotorPlant_step0(RT_MODEL_QuadRotorPlant *const QuadRotorPlant_M,
  BlockIO_QuadRotorPlant *QuadRotorPlant_B, D_Work_QuadRotorPlant
  *QuadRotorPlant_DWork, ContinuousStates_QuadRotorPlant *QuadRotorPlant_X,
  ExternalInputs_QuadRotorPlant *QuadRotorPlant_U,
  ExternalOutputs_QuadRotorPlant *QuadRotorPlant_Y);
extern void QuadRotorPlant_terminate(void);

/*-
 * These blocks were eliminated from the model due to optimizations:
 *
 * Block '<S2>/dtc1' : Eliminate redundant data type conversion
 * Block '<S2>/dtc2' : Eliminate redundant data type conversion
 * Block '<S2>/dtc3' : Eliminate redundant data type conversion
 * Block '<S1>/Zero-Order Hold' : Eliminated since input and output rates are identical
 * Block '<S24>/Rate Transition' : Eliminated since input and output rates are identical
 * Block '<S25>/Rate Transition' : Eliminated since input and output rates are identical
 * Block '<S26>/Rate Transition' : Eliminated since input and output rates are identical
 * Block '<S27>/Rate Transition' : Eliminated since input and output rates are identical
 * Block '<S7>/Switch' : Eliminated due to constant selection input
 * Block '<S7>/Constant' : Unused code path elimination
 */

/*-
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is <system>/block_name, where system is the system number (uniquely
 * assigned by Simulink) and block_name is the name of the block.
 *
 * Note that this particular code originates from a subsystem build,
 * and has its own system numbers different from the parent model.
 * Refer to the system hierarchy for this subsystem below, and use the
 * MATLAB hilite_system command to trace the generated code back
 * to the parent model.  For example,
 *
 * hilite_system('quadrotor_demo_plant/STARMAC/QuadRotorPlant')    - opens subsystem quadrotor_demo_plant/STARMAC/QuadRotorPlant
 * hilite_system('quadrotor_demo_plant/STARMAC/QuadRotorPlant/Kp') - opens and selects block Kp
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : quadrotor_demo_plant/STARMAC
 * '<S1>'   : quadrotor_demo_plant/STARMAC/QuadRotorPlant
 * '<S2>'   : quadrotor_demo_plant/STARMAC/QuadRotorPlant/3DM_GX1
 * '<S3>'   : quadrotor_demo_plant/STARMAC/QuadRotorPlant/SUPERSTAR_II
 * '<S4>'   : quadrotor_demo_plant/STARMAC/QuadRotorPlant/starmac_dynamics
 * '<S5>'   : quadrotor_demo_plant/STARMAC/QuadRotorPlant/starmac_dynamics/6DOF_Euler_Angles
 * '<S6>'   : quadrotor_demo_plant/STARMAC/QuadRotorPlant/starmac_dynamics/starmac_blade_flapping_effects
 * '<S7>'   : quadrotor_demo_plant/STARMAC/QuadRotorPlant/starmac_dynamics/starmac_motor_dynamics
 * '<S8>'   : quadrotor_demo_plant/STARMAC/QuadRotorPlant/starmac_dynamics/6DOF_Euler_Angles/body_euler_to_earth
 * '<S9>'   : quadrotor_demo_plant/STARMAC/QuadRotorPlant/starmac_dynamics/6DOF_Euler_Angles/continuous_body_dynamics
 * '<S10>'  : quadrotor_demo_plant/STARMAC/QuadRotorPlant/starmac_dynamics/6DOF_Euler_Angles/continuous_inertia_body_dynamics
 * '<S11>'  : quadrotor_demo_plant/STARMAC/QuadRotorPlant/starmac_dynamics/6DOF_Euler_Angles/omega_to_euler
 * '<S12>'  : quadrotor_demo_plant/STARMAC/QuadRotorPlant/starmac_dynamics/6DOF_Euler_Angles/body_euler_to_earth/Create_DCM
 * '<S13>'  : quadrotor_demo_plant/STARMAC/QuadRotorPlant/starmac_dynamics/6DOF_Euler_Angles/continuous_body_dynamics/CrossProduct(AXB)
 * '<S14>'  : quadrotor_demo_plant/STARMAC/QuadRotorPlant/starmac_dynamics/6DOF_Euler_Angles/continuous_inertia_body_dynamics/CrossProduct(AXB)
 * '<S15>'  : quadrotor_demo_plant/STARMAC/QuadRotorPlant/starmac_dynamics/6DOF_Euler_Angles/omega_to_euler/Body-based-to-ref-frame-based
 * '<S16>'  : quadrotor_demo_plant/STARMAC/QuadRotorPlant/starmac_dynamics/starmac_blade_flapping_effects/starmac_body_moments
 * '<S17>'  : quadrotor_demo_plant/STARMAC/QuadRotorPlant/starmac_dynamics/starmac_blade_flapping_effects/starmac_flapping_dynamics
 * '<S18>'  : quadrotor_demo_plant/STARMAC/QuadRotorPlant/starmac_dynamics/starmac_blade_flapping_effects/starmac_vertical_effect_dynamics
 * '<S19>'  : quadrotor_demo_plant/STARMAC/QuadRotorPlant/starmac_dynamics/starmac_blade_flapping_effects/starmac_body_moments/CrossProduct(AXB)
 * '<S20>'  : quadrotor_demo_plant/STARMAC/QuadRotorPlant/starmac_dynamics/starmac_blade_flapping_effects/starmac_body_moments/CrossProduct(AXB)1
 * '<S21>'  : quadrotor_demo_plant/STARMAC/QuadRotorPlant/starmac_dynamics/starmac_blade_flapping_effects/starmac_body_moments/CrossProduct(AXB)2
 * '<S22>'  : quadrotor_demo_plant/STARMAC/QuadRotorPlant/starmac_dynamics/starmac_blade_flapping_effects/starmac_body_moments/CrossProduct(AXB)3
 * '<S23>'  : quadrotor_demo_plant/STARMAC/QuadRotorPlant/starmac_dynamics/starmac_blade_flapping_effects/starmac_body_moments/Motor Thrust to Torques
 * '<S24>'  : quadrotor_demo_plant/STARMAC/QuadRotorPlant/starmac_dynamics/starmac_blade_flapping_effects/starmac_vertical_effect_dynamics/starmac_motor_vertical_effect_dynamics
 * '<S25>'  : quadrotor_demo_plant/STARMAC/QuadRotorPlant/starmac_dynamics/starmac_blade_flapping_effects/starmac_vertical_effect_dynamics/starmac_motor_vertical_effect_dynamics1
 * '<S26>'  : quadrotor_demo_plant/STARMAC/QuadRotorPlant/starmac_dynamics/starmac_blade_flapping_effects/starmac_vertical_effect_dynamics/starmac_motor_vertical_effect_dynamics2
 * '<S27>'  : quadrotor_demo_plant/STARMAC/QuadRotorPlant/starmac_dynamics/starmac_blade_flapping_effects/starmac_vertical_effect_dynamics/starmac_motor_vertical_effect_dynamics3
 * '<S28>'  : quadrotor_demo_plant/STARMAC/QuadRotorPlant/starmac_dynamics/starmac_motor_dynamics/Band-Limited White Noise
 */
#endif                                 /* RTW_HEADER_QuadRotorPlant_h_ */

/*
 * File trailer for Real-Time Workshop generated code.
 *
 * [EOF]
 */
