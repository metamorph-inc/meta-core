/* GenCyPhy2SFC.h generated on Mon Feb 11 16:44:06 2013
 */

#include <UdmBase.h>
#include <UdmUtil.h>
#include <cint_string.h>
#include "Cyber2SFC.h"
#include "CyberComposition.h"
#include "LINKS.h"
#include "SFC.h"

typedef std::list< Udm::Object> Packets_t;

// Forward declarations.
class TL_0;
class ProcessProgram_7;
class CreateStatusFunction_1a;
class CreateStatusFunction_23;
class CreateFSFunctionBodies_50;
class PopulateGRFSFunction_59;
class GetGRFuncStateFunction_62;
class DirectInFunction_93;
class TransStartTransitions_b1;
class GetStartTC_b8;
class InitTCVarLoop_f2;
class EnterTransConnectorRecurse_136;
class EnterTransitions_15b;
class EnterProcessTransitions_16c;
class EnterNextState_175;
class NextJunction_1d2;
class GetDstJuncFromRef_23a;
class GetDstStateFromRef_28f;
class DeadEndJunction_2e4;
class TransCond_330;
class Test_37b;
class DstIsState_3db;
class DstIsRefState_42d;
class DstIsJuncWithTrans_482;
class DstIsJuncWithRefTrans_4da;
class DstIsJuncRefWithTrans_536;
class DstIsJuncRefWithJuncTrans_591;
class DeadEnd_5ec;
class OrderTransitions_63e;
class GetAllTransitions_6b3;
class GetTransitionsFromJunctionRefs_6c4;
class GetDirectTransitions_710;
class GetTransitionsFromStateRefs_758;
class CreateTransitionCG_7c8;
class ProcessedFilter_819;
class TCNotProcessed_846;
class NoDefaultTransition_88c;
class ExitLoop_895;
class NDTTest_8da;
class HasDefaultTransitionDouble_907;
class HasDefaultTransitionInt_94f;
class NoDefault_997;
class TransStartMap_a07;
class InitTransConnectorMap_a10;
class TransConnectorMapRecurse_a2a;
class GetAllTransitions_a3d;
class GetTransitionsFromStateRefs_a46;
class GetDirectTransitions_a7b;
class GetTransitionsFromJunctionRefs_aa3;
class AddTransConnector_ad8;
class ProcessTransitionsMap_af6;
class DerefConnRef_afb;
class TransConnTest_b1f;
class IsConnectorRef_b3a;
class Otherwise_b5c;
class OrderTransitions_b7e;
class GetDstTransConnectors_ba0;
class ProcessedFilter_bd5;
class TCNotProcessed_bec;
class PopulateEMFSFunction_c2e;
class FunctionBody_c37;
class GetEMFuncStateFunction_c54;
class GetSubStates_c8b;
class ClearTables_cbd;
class CreateStateLabels_cde;
class CreateCompoundStates_ce7;
class CreateStateLabel_cf0;
class SetPath_cf9;
class Create_d21;
class InitMask_d47;
class IsANDORGROUPCompound_d7f;
class HasSubstates_d96;
class CreateStateVar_dcb;
class CreateLeafStates_ded;
class IsLeaf_df6;
class HasSubstates_e11;
class Otherwise_e3d;
class CreateFuncStates_psuedo_e6d;
class IsFuncState_e76;
class HasSubstates_e91;
class Otherwise_ebd;
class CreateFuncStateFunctions_ef9;
class PopulateEMFSFunction_f02;
class CreateEMFuncStateFunction_f0b;
class FuncArgsAndVars_f38;
class FuncInputArgs_f41;
class InputArgs_f4a;
class FuncOutputArgs_f7f;
class OutputArgs_f88;
class FuncLocalVars_fc0;
class FuncLocalVars_fc9;
class PopulateGRFSFunction_1010;
class CreateGRFuncStateFunction_1019;
class PopulateFunctions_1057;
class PopulateStatusFunction_1060;
class ElimState_1069;
class HasChildren_1088;
class HasChildren_10a3;
class NoChildren_10cf;
class NewIndent_10f4;
class ReturnValue_112f;
class CreateConditionals_1148;
class DecompTest_11ad;
class AndState_11d5;
class OrState_1212;
class GetChildStates_124f;
class CreateConditionalGroup_128c;
class GetStatusFunction_12c2;
class NoFunctionStates_1308;
class NotFunctionState_131f;
class PopulateExecFunction_1346;
class ExecChildren_134f;
class ExecFunctionCondition_1352;
class CreateDefaultCB_1357;
class ChildStatesExec_137f;
class ExecChildAndStates_1384;
class GetChildStates_13af;
class EnterFunctionCondition_13e9;
class CallChildren_13f2;
class CreateCB_1432;
class CreateCG_1472;
class TopLevelState_1498;
class IsTopLevelState_14b3;
class Otherwise_14dc;
class DefaultCondition_1501;
class ChildStateType_152e;
class ChildANDStates_1549;
class Otherwise_1575;
class CallActiveChildExec_159a;
class ExecActiveChildState_15a3;
class CG_15e7;
class GetExecFcn_160d;
class InnerTransitions_164b;
class ExecInnerTransitions_1650;
class GetInnerTransitions_1661;
class ExecProcessTransitions_16af;
class HighestTransParent_16b8;
class ExecNextState_1720;
class ExecTransConnectorRecurse_178a;
class ExecTransitions_17af;
class CheckForInnerTransitions_1821;
class HasInnerTransition_182d;
class Init_184e;
class GetStartTC_1859;
class InitHTPVar_1885;
class GetExecFunction_18c2;
class InitTransConnMapInner_18fd;
class GetInnerTransitions_1902;
class InitTransConnectorMap_1930;
class DuringAction_194b;
class OuterTransitions_1962;
class CheckForOuterTransitions_1967;
class HasOuterTransition_1973;
class HasInnerTransition_198b;
class HasRefOuterTransition_19a5;
class Otherwise_19c1;
class InitTransConnMap_19d8;
class PopulateExitFunction_19ee;
class GoToParent_19f7;
class ArgToParent_1a00;
class ExitChildStates_1a32;
class DeactivateState_1a3b;
class CallChildExits_1a81;
class OrStateCG_1a8e;
class CG_1a97;
class ChildStateType_1abd;
class ChildAndStates_1ad8;
class ChildOrStates_1b04;
class ROrderChildStates_1b2f;
class ExitChildStates_1b5c;
class ArgNegOneOrState_1bab;
class GetExitFunction_1bf0;
class PopulateEnterFunction_1c15;
class Mode0or3_1c1e;
class CallLOSibs_1c23;
class GetLOSibs_1c2c;
class IsAndState_1c5d;
class AndState_1c74;
class CallSibsOrChildren_1c98;
class CallParent_1cec;
class TestMode03_1d2a;
class Mode0or2_1d5c;
class ChildTest_1d61;
class HasChildTransConnector_1d6d;
class RefetchEnterFcn_1d86;
class TestMode02_1db0;
class Mode0or1or2_1df2;
class TestMode012EnterAction_1df7;
class Mode0or1_1e33;
class GetHOSibs_1e38;
class TestMode01CallParent_1e69;
class ReturnAtTopState_1ebc;
class TopState_1ec1;
class TestEAIOff_1eea;
class EAIOff_1ef8;
class Otherwise_1f20;
class CreateRootFunction_1f53;
class CreateRootExecCall_1f5c;
class CreateRootFunction_1f82;
class CreateInputArgs_1fa8;
class CreateLEInit_1fb1;
class CreateIDArgs_1fe2;
class CreateIDArg_1feb;
class GetInData_2010;
class CreateIEArgs_2042;
class CreateIEArg_204b;
class GetInEvent_2070;
class CheckStateDecomp_20a2;
class IsFuncState_20bd;
class Otherwise_20e4;
class CreateOutputArgs_2118;
class CreateODArgs_2121;
class GetOutData_212a;
class CreateODArg_2156;
class CreateOEArgs_2181;
class GetOutEvent_218a;
class CreateOEArg_21b6;
class CreateInitFunction_21f9;
class CreateFunction_FunctionCall_2202;
class FunctionCall_Vals_222c;
class CreateInitFunction_2259;
class InitLocalVars_228c;
class GetSortedVars_2299;
class InitArrayVar_22d2;
class ArrayVars_230d;
class ArrayVar_2335;
class Otherwise_236d;
class InitScalarVar_23a2;
class TestEAIOn_23db;
class EAIOn_23f2;
class MarkLegacy_243c;
class CreateFunctions_2459;
class CreateFunctions_2462;
class CreateDEVars_249b;
class CreateDataVar_24a4;
class CreateDataVar_24ad;
class CreateEventVar_24e5;
class CreateEventVar_24ee;
class CreateStoreVar_251f;
class GetStoreSubsystem_2528;
class InitScope_2531;
class NextHigherSubsystem_2552;
class StoreTest_257a;
class HasStore_2595;
class Otherwise_25c6;
class GetContainingSubsystem_25f4;
class CreateDataVar_2626;
class GetHighestState_2667;
class HighestStateTest_2670;
class HighestState_268b;
class Otherwise_26b6;
class NextHigherState_26db;
class Register_2709;
class GetScope_2726;
class AddToScope_272f;
class SubSubsystemTest_2757;
class NotTopSubsystem_2772;
class Otherwise_279a;
class CreateTypes_2807;
class CreateTypesInner_2812;
class CreateArrayOrBasicType_2829;
class RegisterType_282e;
class MakeAssoc_284f;
class ProcessOther_286c;
class ProcessRowVector_2877;
class ProcessArray2_2882;
class GetArray2_288f;
class Array2Exists_28b7;
class Otherwise_28f6;
class UseArray2_292b;
class CreateArray2_296a;
class ProcessScalar_29ac;
class CreateBasicType_29b7;
class UseBasicType_29e1;
class GetBasicType_2a11;
class BasicTypeExists_2a2c;
class Otherwise_2a58;
class ProcessArray1_2a8d;
class CreateArray1_2a9a;
class UseArray1_2ad0;
class GetArray1_2b0e;
class Array1Exists_2b36;
class Otherwise_2b74;
class ProcessColumn_2bbc;
class DimensionTest_2bce;
class Scalar_2bed;
class ColumnVector_2c14;
class Other_2c3b;
class StructOrMatrix_2c72;
class IsStruct_2c8d;
class IsMatrix_2cb2;
class CreateStructType_2cd7;
class RegisterStruct_2d06;
class StructMembers_2d17;
class CreateStructMembers_2d1e;
class GetTypeBlock_2d47;
class GetTypesFromLib_2d50;
class GetTypes_2d81;
class GetProject_2dbf;
class SetFileName_2dee;
class ExecTransConnectorRecurse2_2e22;
class NoDefaultTransition_2e47;
class ExitLoop_2e50;
class NDTTest_2e95;
class HasDefaultTransitionDouble_2ec2;
class HasDefaultTransitionInt_2f0a;
class NoDefault_2f52;
class ExecTransitions_2fa1;
class ExecProcessTransitions_2fb2;
class HighestTransParent_2fbb;
class GetDstJuncFromRef_3023;
class TransCond_3078;
class Test_30c3;
class DstIsState_3123;
class DstIsRefState_3175;
class DstIsJuncWithTrans_31ca;
class DstIsJuncWithRefTrans_3222;
class DstIsJuncRefWithTrans_327e;
class DstIsJuncRefWithJuncTrans_32d9;
class DeadEnd_3334;
class OrderTransitions_3386;
class DeadEndJunction_33c8;
class NextJunction_3414;
class GetDstStateFromRef_347c;
class ExecNextState_34d1;
class GetAllTransitions_3574;
class GetTransitionsFromJunctionRefs_3585;
class GetDirectTransitions_35d1;
class GetTransitionsFromStateRefs_3619;
class ProcessedFilter_3689;
class TCNotProcessed_36b6;
class CreateTransitionCG_36fc;
class T2L_3761;
class CreateTypes_3768;
class CreateTypesInner_3773;
class StructOrMatrix_378a;
class IsStruct_37a5;
class IsMatrix_37ca;
class CreateArrayOrBasicType_37ef;
class RegisterType_37f4;
class MakeAssoc_3815;
class ProcessOther_3832;
class ProcessRowVector_383d;
class ProcessArray2_3848;
class GetArray2_3855;
class Array2Exists_387d;
class Otherwise_38bc;
class UseArray2_38f1;
class CreateArray2_3930;
class ProcessScalar_3972;
class CreateBasicType_397d;
class UseBasicType_39a7;
class GetBasicType_39d7;
class BasicTypeExists_39f2;
class Otherwise_3a1e;
class ProcessArray1_3a53;
class CreateArray1_3a60;
class UseArray1_3a96;
class GetArray1_3ad4;
class Array1Exists_3afc;
class Otherwise_3b3a;
class ProcessColumn_3b82;
class DimensionTest_3b94;
class Scalar_3bb3;
class ColumnVector_3bda;
class Other_3c01;
class CreateStructType_3c38;
class RegisterStruct_3c67;
class StructMembers_3c78;
class CreateStructMembers_3c7f;
class GetTypeBlock_3ca8;
class GetTypes_3cb1;
class GetTypesFromLib_3cde;
class GetProject_3d20;
class SetFileName_3d4f;
class ProcessProgram_3d75;
class CreateStatusFunction_3d88;
class CreateStatusFunction_3d91;
class CreateFSFunctionBodies_3dbe;
class PopulateGRFSFunction_3dc7;
class GetGRFuncStateFunction_3dd0;
class DirectInFunction_3e01;
class TransStartTransitions_3e1f;
class GetStartTC_3e26;
class EnterTransConnectorRecurse2_3e60;
class EnterTransitions_3e85;
class EnterProcessTransitions_3e96;
class EnterNextState_3e9f;
class InitTCVarLoop_3f4d;
class TransStartMap_3f9e;
class InitTransConnectorMap_3fa7;
class TransConnectorMapRecurse2_3fc1;
class GetAllTransitions_3fd4;
class GetTransitionsFromStateRefs_3fdd;
class GetDirectTransitions_4012;
class GetTransitionsFromJunctionRefs_403a;
class ProcessTransitionsMap_406f;
class DerefConnRef_4074;
class TransConnTest_4098;
class IsConnectorRef_40b3;
class Otherwise_40d5;
class OrderTransitions_40f7;
class GetDstTransConnectors_4119;
class ProcessedFilter_414e;
class TCNotProcessed_4165;
class AddTransConnector_4189;
class PopulateEMFSFunction_41c5;
class FunctionBody_41ce;
class GetEMFuncStateFunction_41eb;
class GetSubStates_4222;
class ClearTables_4254;
class CreateStateLabels_4275;
class CreateFuncStates_psuedo_427e;
class IsFuncState_4287;
class HasSubstates_42a2;
class Otherwise_42ce;
class CreateStateLabel_42f5;
class SetPath_42fe;
class Create_4326;
class InitMask_434c;
class CreateCompoundStates_438d;
class IsANDORGROUPCompound_4396;
class HasSubstates_43ad;
class CreateLeafStates_43e2;
class IsLeaf_43eb;
class HasSubstates_4406;
class Otherwise_4432;
class CreateStateVar_4462;
class CreateDEVars_4490;
class CreateDataVar_4499;
class CreateDataVar_44a2;
class CreateStoreVar_44da;
class GetHighestState_44e3;
class HighestStateTest_44ec;
class HighestState_4507;
class Otherwise_4532;
class NextHigherState_4557;
class Register_4585;
class GetScope_45a2;
class AddToScope_45ab;
class SubSubsystemTest_45d3;
class NotTopSubsystem_45ee;
class Otherwise_4616;
class GetStoreSubsystem_4641;
class StoreTest_464a;
class HasStore_4665;
class Otherwise_4691;
class NextHigherSubsystem_46b6;
class InitScope_46de;
class GetContainingSubsystem_4708;
class CreateDataVar_4730;
class CreateEventVar_4783;
class CreateEventVar_478c;
class CheckStateDecomp_47bd;
class IsFuncState_47d8;
class Otherwise_47ff;
class CreateFuncStateFunctions_4833;
class PopulateEMFSFunction_483c;
class CreateEMFuncStateFunction_4845;
class FuncArgsAndVars_4872;
class FuncOutputArgs_487b;
class OutputArgs_4884;
class FuncInputArgs_48bc;
class InputArgs_48c5;
class FuncLocalVars_48fa;
class FuncLocalVars_4903;
class PopulateGRFSFunction_494a;
class CreateGRFuncStateFunction_4953;
class PopulateFunctions_4991;
class PopulateStatusFunction_499a;
class ElimState_49a3;
class HasChildren_49c2;
class HasChildren_49dd;
class NoChildren_4a09;
class NewIndent_4a2e;
class ReturnValue_4a69;
class CreateConditionals_4a82;
class DecompTest_4ae7;
class AndState_4b0f;
class OrState_4b4c;
class GetChildStates_4b89;
class CreateConditionalGroup_4bc6;
class GetStatusFunction_4bfc;
class NoFunctionStates_4c42;
class NotFunctionState_4c59;
class PopulateEnterFunction_4c80;
class TestEAIOff_4c89;
class EAIOff_4c97;
class Otherwise_4cb6;
class Mode0or3_4ccb;
class CallLOSibs_4cd0;
class GetLOSibs_4cd9;
class IsAndState_4d0a;
class AndState_4d21;
class CallSibsOrChildren_4d45;
class CallParent_4d99;
class TestMode03_4dd7;
class Mode0or2_4e09;
class ChildTest_4e0e;
class HasChildTransConnector_4e1a;
class RefetchEnterFcn_4e33;
class TestMode02_4e5d;
class GetChildStates_4e87;
class ChildStateType_4eb4;
class ChildANDStates_4ecf;
class Otherwise_4efb;
class Mode0or1or2_4f38;
class TestMode012EnterAction_4f3d;
class Mode0or1_4f79;
class GetHOSibs_4f7e;
class TestMode01CallParent_4faf;
class ReturnAtTopState_5002;
class TopState_5007;
class PopulateExecFunction_503c;
class ExecChildren_5045;
class ExecFunctionCondition_5048;
class CreateDefaultCB_504d;
class ChildStatesExec_5075;
class ExecChildAndStates_507a;
class EnterFunctionCondition_50b2;
class CallChildren_50bb;
class CreateCB_50fb;
class CreateCG_513b;
class TopLevelState_5161;
class IsTopLevelState_517c;
class Otherwise_51a5;
class DefaultCondition_51ca;
class CallActiveChildExec_51f7;
class ExecActiveChildState_5200;
class CG_5244;
class GetExecFcn_526a;
class InnerTransitions_52a8;
class ExecInnerTransitions_52ad;
class GetInnerTransitions_52be;
class CheckForInnerTransitions_5316;
class HasInnerTransition_5322;
class Init_5343;
class GetStartTC_534e;
class InitHTPVar_537a;
class GetExecFunction_53b7;
class InitTransConnMapInner_53f2;
class GetInnerTransitions_53f7;
class InitTransConnectorMap_5425;
class DuringAction_5440;
class InitTransConnMap_5457;
class OuterTransitions_5461;
class CheckForOuterTransitions_5466;
class HasOuterTransition_5472;
class HasInnerTransition_548a;
class HasRefOuterTransition_54a4;
class Otherwise_54c0;
class PopulateExitFunction_54e3;
class GoToParent_54ec;
class ArgToParent_54f5;
class ExitChildStates_5527;
class DeactivateState_5530;
class CallChildExits_5576;
class OrStateCG_5583;
class CG_558c;
class ChildStateType_55b2;
class ChildAndStates_55cd;
class ChildOrStates_55f9;
class ROrderChildStates_5624;
class ExitChildStates_5651;
class ArgNegOneOrState_56a0;
class GetExitFunction_56e5;
class CreateFunctions_571c;
class CreateFunctions_5725;
class CreateRootFunction_575e;
class CreateRootExecCall_5767;
class CreateRootFunction_578d;
class CreateInputArgs_57b3;
class CreateLEInit_57bc;
class CreateIDArgs_57ed;
class CreateIDArg_57f6;
class GetInData_581b;
class CreateIEArgs_584d;
class CreateIEArg_5856;
class GetInEvent_587b;
class CreateOutputArgs_58bc;
class CreateODArgs_58c5;
class GetOutData_58ce;
class CreateODArg_58fa;
class CreateOEArgs_5925;
class GetOutEvent_592e;
class CreateOEArg_595a;
class CreateInitFunction_599d;
class TestEAIOn_59a6;
class EAIOn_59bd;
class CreateFunction_FunctionCall_59ed;
class FunctionCall_Vals_5a17;
class CreateInitFunction_5a44;
class InitLocalVars_5a77;
class GetSortedVars_5a84;
class InitArrayVar_5abd;
class ArrayVars_5af8;
class ArrayVar_5b20;
class Otherwise_5b58;
class InitScalarVar_5b8d;
class MarkLegacy_5bd7;

// Class declarations.
class TL_0
{
public:
	void operator()( const Packets_t& rootFolders_1, const Packets_t& rootStates_3, const Packets_t& projects_5);

protected:
	void callGetProject_2e14( const Packets_t& rootFolders_2dc0, const Packets_t& states_2dc3, const Packets_t& projects_2dc6);
	void callCreateTypes_2e18( const Packets_t& rootFolders_2808, const Packets_t& states_280a, const Packets_t& projects_280c);
	void callSetFileName_2e1c( const Packets_t& states_2def, const Packets_t& projects_2df3);
	void callProcessProgram_2e1f( const Packets_t& states_8, const Packets_t& programs_a);
};

class ProcessProgram_7
{
public:
	void operator()( const Packets_t& states_8, const Packets_t& programs_a);

protected:
	void executeOne( const Packets_t& states_8, const Packets_t& programs_a);
	bool isInputUnique( const Udm::Object& state_e, const Udm::Object& program_15);
	void callClearTables_27e6( const Packets_t& states_cbe, const Packets_t& programs_cc1);
	void callCreateStateLabels_27e9( const Packets_t& states_cdf, const Packets_t& programs_ce1);
	void callCreateDEVars_27ec( const Packets_t& states_249c, const Packets_t& programs_249e);
	void callCreateFuncStateFunctions_27ef( const Packets_t& states_efa, const Packets_t& programs_efc);
	void callCreateFSFunctionBodies_27f2( const Packets_t& states_51, const Packets_t& programs_53);
	void callCreateFunctions_27f5( const Packets_t& states_245a, const Packets_t& programs_245c);
	void callPopulateFunctions_27f8( const Packets_t& states_1058, const Packets_t& programs_105a);
	void callCreateRootFunction_27fb( const Packets_t& states_1f54, const Packets_t& programs_1f56);
	void callCreateInitFunction_27fe( const Packets_t& states_21fa, const Packets_t& programs_21fc);
	void callCreateStatusFunction_2801( const Packets_t& states_1b, const Packets_t& programs_1d);
	void callMarkLegacy_2804( const Packets_t& states_243d, const Packets_t& programs_243f);

private:
	Packets_t _state_c;
	Packets_t _program_13;
};

class CreateStatusFunction_1a
{
public:
	void operator()( const Packets_t& states_1b, const Packets_t& programs_1d, Packets_t& states_1f, Packets_t& programs_20);

protected:
	void callCreateStatusFunction_4d( const Packets_t& states_24, const Packets_t& programs_26);

private:
	Packets_t* _state_21;
	Packets_t* _program_22;
};

class CreateStatusFunction_23
{
public:
	void operator()( const Packets_t& states_24, const Packets_t& programs_26);

protected:
	bool isInputUnique( const Udm::Object& state_2c, const Udm::Object& program_35);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& states_24, const Packets_t& programs_26);
	bool patternMatcher( const Udm::Object& state_2a, const Udm::Object& program_33);
	void effector();

private:
	Packets_t _state_28;
	Packets_t _program_31;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_44;
		SFC::Program program_45;
		SFC::Arg arg_46;
		SFC::Function status_47;
	};

	std::list< Match> _matches;
};

class CreateFSFunctionBodies_50
{
public:
	void operator()( const Packets_t& states_51, const Packets_t& programs_53, Packets_t& states_55, Packets_t& programs_56);

protected:
	void callPopulateGRFSFunction_cb4( const Packets_t& states_5a, const Packets_t& programs_5c);
	void callPopulateEMFSFunction_cb7( const Packets_t& states_c2f, const Packets_t& programs_c31);
	void callGetSubStates_cba( const Packets_t& states_c8c, const Packets_t& parStmnts_c8f);

private:
	Packets_t* _state_57;
	Packets_t* _program_58;
};

class PopulateGRFSFunction_59
{
public:
	void operator()( const Packets_t& states_5a, const Packets_t& programs_5c, Packets_t& states_5e, Packets_t& programs_5f);

protected:
	void callGetGRFuncStateFunction_c21( const Packets_t& states_63, const Packets_t& programs_66);
	void callTransStartMap_c24( const Packets_t& states_a08, const Packets_t& css_a0a);
	void callDirectInFunction_c27( const Packets_t& states_94, const Packets_t& functions_97);
	void callTransStartTransitions_c2a( const Packets_t& states_b2, const Packets_t& fcns_b4, const Packets_t& css_b6);

private:
	Packets_t* _state_60;
	Packets_t* _program_61;
};

class GetGRFuncStateFunction_62
{
public:
	void operator()( const Packets_t& states_63, const Packets_t& programs_66, Packets_t& states_65, Packets_t& execs_68);

protected:
	bool isInputUnique( const Udm::Object& state_6f, const Udm::Object& program_78);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::Function& Exec, SFC::Program& Program, CyberComposition::Simulink::State& State, SFC::StateLabel& StateLabel);
	void processInputPackets( const Packets_t& states_63, const Packets_t& programs_66);
	bool patternMatcher( const Udm::Object& state_6d, const Udm::Object& program_76);
	void effector();
	void outputAppender( const CyberComposition::Simulink::State& state_8f, const SFC::Function& exec_91);

private:
	Packets_t* _state_69;
	Packets_t* _exec_6a;
	Packets_t _state_6b;
	Packets_t _program_74;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_87;
		SFC::Program program_88;
		SFC::StateLabel stateLabel_89;
		SFC::Function exec_8a;
	};

	std::list< Match> _matches;
};

class DirectInFunction_93
{
public:
	void operator()( const Packets_t& states_94, const Packets_t& functions_97, Packets_t& states_96, Packets_t& functions_99);

protected:
	bool isInputUnique( const Udm::Object& state_a0, const Udm::Object& function_a9);
	void processInputPackets( const Packets_t& states_94, const Packets_t& functions_97);
	bool patternMatcher( const Udm::Object& state_9e, const Udm::Object& function_a7);
	void effector();
	void forwardInputs();

private:
	Packets_t* _state_9a;
	Packets_t* _function_9b;
	Packets_t _state_9c;
	Packets_t _function_a5;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_af;
		SFC::Function function_b0;
	};

	std::list< Match> _matches;
};

class TransStartTransitions_b1
{
public:
	void operator()( const Packets_t& states_b2, const Packets_t& fcns_b4, const Packets_t& css_b6);

protected:
	void callInitTCVarLoop_9fa( const Packets_t& states_f3, const Packets_t& functions_f6, const Packets_t& compoundStatements_f9);
	void callGetStartTC_9fe( const Packets_t& states_b9, const Packets_t& functions_bd, const Packets_t& conditionalGroups_c0);
	void callEnterTransConnectorRecurse_a02( const Packets_t& states_137, const Packets_t& tcs_139, const Packets_t& fcns_13b, const Packets_t& cgs_13d);
};

class GetStartTC_b8
{
public:
	void operator()( const Packets_t& states_b9, const Packets_t& functions_bd, const Packets_t& conditionalGroups_c0, Packets_t& states_bb, Packets_t& transStarts_bc, Packets_t& functions_bf, Packets_t& conditionalGroups_c2);

protected:
	bool isInputUnique( const Udm::Object& state_cb, const Udm::Object& function_d4, const Udm::Object& conditionalGroup_dd);
	void processInputPackets( const Packets_t& states_b9, const Packets_t& functions_bd, const Packets_t& conditionalGroups_c0);
	bool patternMatcher( const Udm::Object& state_c9, const Udm::Object& function_d2, const Udm::Object& conditionalGroup_db);
	void effector();
	void outputAppender( const CyberComposition::Simulink::State& state_ea, const CyberComposition::Simulink::TransStart& transStart_ec, const SFC::Function& function_ee, const SFC::ConditionalGroup& conditionalGroup_f0);

private:
	Packets_t* _state_c3;
	Packets_t* _transStart_c4;
	Packets_t* _function_c5;
	Packets_t* _conditionalGroup_c6;
	Packets_t _state_c7;
	Packets_t _function_d0;
	Packets_t _conditionalGroup_d9;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_e6;
		SFC::Function function_e7;
		SFC::ConditionalGroup conditionalGroup_e8;
		CyberComposition::Simulink::TransStart transStart_e9;
	};

	std::list< Match> _matches;
};

class InitTCVarLoop_f2
{
public:
	void operator()( const Packets_t& states_f3, const Packets_t& functions_f6, const Packets_t& compoundStatements_f9, Packets_t& states_f5, Packets_t& functions_f8, Packets_t& conditionalGroups_fb);

protected:
	bool isInputUnique( const Udm::Object& state_103, const Udm::Object& function_10c, const Udm::Object& compoundStatement_115);
	bool isGuardTrue( SFC::CompoundStatement& CompoundStatement, SFC::Function& Function, CyberComposition::Simulink::State& State, SFC::LocalVar& TCVar);
	void processInputPackets( const Packets_t& states_f3, const Packets_t& functions_f6, const Packets_t& compoundStatements_f9);
	bool patternMatcher( const Udm::Object& state_101, const Udm::Object& function_10a, const Udm::Object& compoundStatement_113);
	void effector();
	void outputAppender( const CyberComposition::Simulink::State& state_130, const SFC::Function& function_132, const SFC::ConditionalGroup& conditionalGroup_134);

private:
	Packets_t* _state_fc;
	Packets_t* _function_fd;
	Packets_t* _conditionalGroup_fe;
	Packets_t _state_ff;
	Packets_t _function_108;
	Packets_t _compoundStatement_111;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_11e;
		SFC::Function function_11f;
		SFC::CompoundStatement compoundStatement_120;
		SFC::LocalVar tCVar_121;
	};

	std::list< Match> _matches;
};

class EnterTransConnectorRecurse_136
{
public:
	void operator()( const Packets_t& states_137, const Packets_t& tcs_139, const Packets_t& fcns_13b, const Packets_t& cgs_13d);

protected:
	void executeOne( const Packets_t& states_137, const Packets_t& tcs_139, const Packets_t& fcns_13b, const Packets_t& cgs_13d);
	bool isInputUnique( const Udm::Object& state_141, const Udm::Object& tc_148, const Udm::Object& fcn_14f, const Udm::Object& cg_156);
	void callProcessedFilter_9e6( const Packets_t& states_81a, const Packets_t& tcs_81c, const Packets_t& fcns_81e, const Packets_t& cgs_820);
	void callCreateTransitionCG_9eb( const Packets_t& states_7c9, const Packets_t& transConnectors_7cc, const Packets_t& functions_7cf, const Packets_t& tCVarCGs_7d3);
	void callEnterTransitions_9f0( const Packets_t& states_15c, const Packets_t& tcs_15e, const Packets_t& fcns_160, const Packets_t& css_162);
	void callNoDefaultTransition_9f5( const Packets_t& states_88d, const Packets_t& tcs_88f, const Packets_t& fcns_891, const Packets_t& css_893);

private:
	Packets_t _state_13f;
	Packets_t _tc_146;
	Packets_t _fcn_14d;
	Packets_t _cg_154;
};

class EnterTransitions_15b
{
public:
	void operator()( const Packets_t& states_15c, const Packets_t& tcs_15e, const Packets_t& fcns_160, const Packets_t& css_162, Packets_t& states_164, Packets_t& tcs_165, Packets_t& fcns_166, Packets_t& css_167);

protected:
	void callGetAllTransitions_7be( const Packets_t& states_6b4, const Packets_t& tcs_6b6, const Packets_t& fcns_6b8, const Packets_t& css_6ba);
	void callEnterProcessTransitions_7c3( const Packets_t& states_16d, const Packets_t& transs_16f, const Packets_t& fcns_171, const Packets_t& css_173);

private:
	Packets_t* _state_168;
	Packets_t* _tc_169;
	Packets_t* _fcn_16a;
	Packets_t* _cs_16b;
};

class EnterProcessTransitions_16c
{
public:
	void operator()( const Packets_t& states_16d, const Packets_t& transs_16f, const Packets_t& fcns_171, const Packets_t& css_173);

protected:
	void callOrderTransitions_680( const Packets_t& states_63f, const Packets_t& transitions_642, const Packets_t& functions_645, const Packets_t& compoundStatements_648);
	void callTransCond_685( const Packets_t& states_331, const Packets_t& transitions_335, const Packets_t& functions_338, const Packets_t& transitionCGs_33b);
	void callTest_68a( const Packets_t& states_37c, const Packets_t& dsts_37e, const Packets_t& transs_380, const Packets_t& fcns_382, const Packets_t& cbs_384);
	void callEnterNextState_690( const Packets_t& states_176, const Packets_t& dstStates_178, const Packets_t& transitions_17a, const Packets_t& functions_17c, const Packets_t& transitionCBs_17e);
	void callNextJunction_696( const Packets_t& states_1d3, const Packets_t& junctions_1d6, const Packets_t& transitions_1d9, const Packets_t& functions_1db, const Packets_t& transitionCBs_1de);
	void callDeadEndJunction_69c( const Packets_t& states_2e5, const Packets_t& transConnectors_2e7, const Packets_t& transitions_2e9, const Packets_t& functions_2eb, const Packets_t& transitionCBs_2ed);
	void callGetDstJuncFromRef_6a2( const Packets_t& states_23b, const Packets_t& connectorRefs_23e, const Packets_t& transitions_241, const Packets_t& functions_244, const Packets_t& conditionalBlocks_247);
	void callGetDstStateFromRef_6a8( const Packets_t& states_290, const Packets_t& connectorRefs_294, const Packets_t& transitions_296, const Packets_t& functions_299, const Packets_t& conditionalBlocks_29c);
	void callEnterTransConnectorRecurse_6ae( const Packets_t& states_137, const Packets_t& tcs_139, const Packets_t& fcns_13b, const Packets_t& cgs_13d);
};

class EnterNextState_175
{
public:
	void operator()( const Packets_t& states_176, const Packets_t& dstStates_178, const Packets_t& transitions_17a, const Packets_t& functions_17c, const Packets_t& transitionCBs_17e);

protected:
	bool isInputUnique( const Udm::Object& state_184, const Udm::Object& dstState_18d, const Udm::Object& transition_196, const Udm::Object& function_19f, const Udm::Object& transitionCB_1a8);
	bool isGuardTrue( CyberComposition::Simulink::State& DstState, SFC::Function& Enter, SFC::Arg& EnterArg0, SFC::Arg& EnterArg1, SFC::Function& Function, CyberComposition::Simulink::State& State, SFC::StateLabel& StateLabel, CyberComposition::Simulink::Transition& Transition, SFC::ConditionalBlock& TransitionCB);
	void processInputPackets( const Packets_t& states_176, const Packets_t& dstStates_178, const Packets_t& transitions_17a, const Packets_t& functions_17c, const Packets_t& transitionCBs_17e);
	bool patternMatcher( const Udm::Object& state_182, const Udm::Object& dstState_18b, const Udm::Object& transition_194, const Udm::Object& function_19d, const Udm::Object& transitionCB_1a6);
	void effector();

private:
	Packets_t _state_180;
	Packets_t _dstState_189;
	Packets_t _transition_192;
	Packets_t _function_19b;
	Packets_t _transitionCB_1a4;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_1ba;
		CyberComposition::Simulink::State dstState_1bb;
		CyberComposition::Simulink::Transition transition_1bc;
		SFC::Function function_1bd;
		SFC::ConditionalBlock transitionCB_1be;
		SFC::StateLabel stateLabel_1bf;
		SFC::Arg enterArg0_1c0;
		SFC::Function enter_1c1;
		SFC::Arg enterArg1_1c2;
	};

	std::list< Match> _matches;
};

class NextJunction_1d2
{
public:
	void operator()( const Packets_t& states_1d3, const Packets_t& junctions_1d6, const Packets_t& transitions_1d9, const Packets_t& functions_1db, const Packets_t& transitionCBs_1de, Packets_t& states_1d5, Packets_t& junctions_1d8, Packets_t& functions_1dd, Packets_t& conditionalGroups_1e0);

protected:
	bool isInputUnique( const Udm::Object& state_1e9, const Udm::Object& junction_1f2, const Udm::Object& transition_1fb, const Udm::Object& function_204, const Udm::Object& transitionCB_20d);
	bool isGuardTrue( SFC::ConditionalBlock& ConditionalBlock, SFC::ConditionalGroup& ConditionalGroup, SFC::Function& Function, CyberComposition::Simulink::SF_Junction& Junction, CyberComposition::Simulink::State& State, SFC::LocalVar& TCVar, CyberComposition::Simulink::Transition& Transition, SFC::ConditionalBlock& TransitionCB, SFC::ConditionalGroup& TransitionCG);
	void processInputPackets( const Packets_t& states_1d3, const Packets_t& junctions_1d6, const Packets_t& transitions_1d9, const Packets_t& functions_1db, const Packets_t& transitionCBs_1de);
	bool patternMatcher( const Udm::Object& state_1e7, const Udm::Object& junction_1f0, const Udm::Object& transition_1f9, const Udm::Object& function_202, const Udm::Object& transitionCB_20b);
	void effector();
	void outputAppender( const CyberComposition::Simulink::State& state_232, const CyberComposition::Simulink::SF_Junction& junction_234, const SFC::Function& function_236, const SFC::ConditionalGroup& conditionalGroup_238);

private:
	Packets_t* _state_1e1;
	Packets_t* _junction_1e2;
	Packets_t* _function_1e3;
	Packets_t* _conditionalGroup_1e4;
	Packets_t _state_1e5;
	Packets_t _junction_1ee;
	Packets_t _transition_1f7;
	Packets_t _function_200;
	Packets_t _transitionCB_209;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_21c;
		CyberComposition::Simulink::SF_Junction junction_21d;
		CyberComposition::Simulink::Transition transition_21e;
		SFC::Function function_21f;
		SFC::ConditionalBlock transitionCB_220;
		SFC::LocalVar tCVar_221;
		SFC::ConditionalGroup conditionalGroup_222;
		SFC::ConditionalGroup transitionCG_223;
		SFC::ConditionalBlock conditionalBlock_224;
	};

	std::list< Match> _matches;
};

class GetDstJuncFromRef_23a
{
public:
	void operator()( const Packets_t& states_23b, const Packets_t& connectorRefs_23e, const Packets_t& transitions_241, const Packets_t& functions_244, const Packets_t& conditionalBlocks_247, Packets_t& states_23d, Packets_t& junctions_240, Packets_t& transitions_243, Packets_t& functions_246, Packets_t& conditionalBlocks_249);

protected:
	bool isInputUnique( const Udm::Object& state_253, const Udm::Object& connectorRef_25c, const Udm::Object& transition_265, const Udm::Object& function_26e, const Udm::Object& conditionalBlock_277);
	void processInputPackets( const Packets_t& states_23b, const Packets_t& connectorRefs_23e, const Packets_t& transitions_241, const Packets_t& functions_244, const Packets_t& conditionalBlocks_247);
	bool patternMatcher( const Udm::Object& state_251, const Udm::Object& connectorRef_25a, const Udm::Object& transition_263, const Udm::Object& function_26c, const Udm::Object& conditionalBlock_275);
	void effector();
	void outputAppender( const CyberComposition::Simulink::State& state_285, const CyberComposition::Simulink::SF_Junction& junction_287, const CyberComposition::Simulink::Transition& transition_289, const SFC::Function& function_28b, const SFC::ConditionalBlock& conditionalBlock_28d);

private:
	Packets_t* _state_24a;
	Packets_t* _junction_24b;
	Packets_t* _transition_24c;
	Packets_t* _function_24d;
	Packets_t* _conditionalBlock_24e;
	Packets_t _state_24f;
	Packets_t _connectorRef_258;
	Packets_t _transition_261;
	Packets_t _function_26a;
	Packets_t _conditionalBlock_273;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_27f;
		CyberComposition::Simulink::ConnectorRef connectorRef_280;
		CyberComposition::Simulink::Transition transition_281;
		SFC::Function function_282;
		SFC::ConditionalBlock conditionalBlock_283;
		CyberComposition::Simulink::SF_Junction junction_284;
	};

	std::list< Match> _matches;
};

class GetDstStateFromRef_28f
{
public:
	void operator()( const Packets_t& states_290, const Packets_t& connectorRefs_294, const Packets_t& transitions_296, const Packets_t& functions_299, const Packets_t& conditionalBlocks_29c, Packets_t& states_292, Packets_t& dstStates_293, Packets_t& transitions_298, Packets_t& functions_29b, Packets_t& conditionalBlocks_29e);

protected:
	bool isInputUnique( const Udm::Object& state_2a8, const Udm::Object& connectorRef_2b1, const Udm::Object& transition_2ba, const Udm::Object& function_2c3, const Udm::Object& conditionalBlock_2cc);
	void processInputPackets( const Packets_t& states_290, const Packets_t& connectorRefs_294, const Packets_t& transitions_296, const Packets_t& functions_299, const Packets_t& conditionalBlocks_29c);
	bool patternMatcher( const Udm::Object& state_2a6, const Udm::Object& connectorRef_2af, const Udm::Object& transition_2b8, const Udm::Object& function_2c1, const Udm::Object& conditionalBlock_2ca);
	void effector();
	void outputAppender( const CyberComposition::Simulink::State& state_2da, const CyberComposition::Simulink::State& dstState_2dc, const CyberComposition::Simulink::Transition& transition_2de, const SFC::Function& function_2e0, const SFC::ConditionalBlock& conditionalBlock_2e2);

private:
	Packets_t* _state_29f;
	Packets_t* _dstState_2a0;
	Packets_t* _transition_2a1;
	Packets_t* _function_2a2;
	Packets_t* _conditionalBlock_2a3;
	Packets_t _state_2a4;
	Packets_t _connectorRef_2ad;
	Packets_t _transition_2b6;
	Packets_t _function_2bf;
	Packets_t _conditionalBlock_2c8;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_2d4;
		CyberComposition::Simulink::ConnectorRef connectorRef_2d5;
		CyberComposition::Simulink::Transition transition_2d6;
		SFC::Function function_2d7;
		SFC::ConditionalBlock conditionalBlock_2d8;
		CyberComposition::Simulink::State dstState_2d9;
	};

	std::list< Match> _matches;
};

class DeadEndJunction_2e4
{
public:
	void operator()( const Packets_t& states_2e5, const Packets_t& transConnectors_2e7, const Packets_t& transitions_2e9, const Packets_t& functions_2eb, const Packets_t& transitionCBs_2ed);

protected:
	bool isInputUnique( const Udm::Object& state_2f3, const Udm::Object& transConnector_2fc, const Udm::Object& transition_305, const Udm::Object& function_30e, const Udm::Object& transitionCB_317);
	bool isGuardTrue( SFC::Function& Function, CyberComposition::Simulink::State& State, SFC::LocalVar& TCVar, CyberComposition::Simulink::TransConnector& TransConnector, CyberComposition::Simulink::Transition& Transition, SFC::ConditionalBlock& TransitionCB);
	void processInputPackets( const Packets_t& states_2e5, const Packets_t& transConnectors_2e7, const Packets_t& transitions_2e9, const Packets_t& functions_2eb, const Packets_t& transitionCBs_2ed);
	bool patternMatcher( const Udm::Object& state_2f1, const Udm::Object& transConnector_2fa, const Udm::Object& transition_303, const Udm::Object& function_30c, const Udm::Object& transitionCB_315);
	void effector();

private:
	Packets_t _state_2ef;
	Packets_t _transConnector_2f8;
	Packets_t _transition_301;
	Packets_t _function_30a;
	Packets_t _transitionCB_313;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_320;
		CyberComposition::Simulink::TransConnector transConnector_321;
		CyberComposition::Simulink::Transition transition_322;
		SFC::Function function_323;
		SFC::ConditionalBlock transitionCB_324;
		SFC::LocalVar tCVar_325;
	};

	std::list< Match> _matches;
};

class TransCond_330
{
public:
	void operator()( const Packets_t& states_331, const Packets_t& transitions_335, const Packets_t& functions_338, const Packets_t& transitionCGs_33b, Packets_t& states_333, Packets_t& dstTransConnectors_334, Packets_t& transitions_337, Packets_t& functions_33a, Packets_t& conditionalBlocks_33d);

protected:
	bool isInputUnique( const Udm::Object& state_347, const Udm::Object& transition_350, const Udm::Object& function_359, const Udm::Object& transitionCG_362);
	void processInputPackets( const Packets_t& states_331, const Packets_t& transitions_335, const Packets_t& functions_338, const Packets_t& transitionCGs_33b);
	bool patternMatcher( const Udm::Object& state_345, const Udm::Object& transition_34e, const Udm::Object& function_357, const Udm::Object& transitionCG_360);
	void effector();
	void outputAppender( const CyberComposition::Simulink::State& state_371, const CyberComposition::Simulink::TransConnector& dstTransConnector_373, const CyberComposition::Simulink::Transition& transition_375, const SFC::Function& function_377, const SFC::ConditionalBlock& conditionalBlock_379);

private:
	Packets_t* _state_33e;
	Packets_t* _dstTransConnector_33f;
	Packets_t* _transition_340;
	Packets_t* _function_341;
	Packets_t* _conditionalBlock_342;
	Packets_t _state_343;
	Packets_t _transition_34c;
	Packets_t _function_355;
	Packets_t _transitionCG_35e;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_36a;
		CyberComposition::Simulink::Transition transition_36b;
		SFC::Function function_36c;
		SFC::ConditionalGroup transitionCG_36d;
		CyberComposition::Simulink::TransConnector dstTransConnector_36e;
		CyberComposition::Simulink::TransConnector srcTransConnector_36f;
	};

	std::list< Match> _matches;
};

class Test_37b
{
public:
	void operator()( const Packets_t& states_37c, const Packets_t& dsts_37e, const Packets_t& transs_380, const Packets_t& fcns_382, const Packets_t& cbs_384, Packets_t& states_386, Packets_t& dsts_387, Packets_t& transs_388, Packets_t& fcns_389, Packets_t& cbs_38a, Packets_t& states_38b, Packets_t& dsts_38c, Packets_t& transs_38d, Packets_t& fcns_38e, Packets_t& cbs_38f, Packets_t& states_390, Packets_t& dsts_391, Packets_t& transs_392, Packets_t& fcns_393, Packets_t& cbs_394, Packets_t& states_395, Packets_t& dsts_396, Packets_t& transs_397, Packets_t& fcns_398, Packets_t& cbs_399, Packets_t& states_39a, Packets_t& dsts_39b, Packets_t& transs_39c, Packets_t& fcns_39d, Packets_t& cbs_39e);

protected:
	void executeOne( const Packets_t& states_37c, const Packets_t& dsts_37e, const Packets_t& transs_380, const Packets_t& fcns_382, const Packets_t& cbs_384);
	bool isInputUnique( const Udm::Object& state_3ba, const Udm::Object& dst_3c1, const Udm::Object& trans_3c8, const Udm::Object& fcn_3cf, const Udm::Object& cb_3d6);

private:
	Packets_t* _state_39f;
	Packets_t* _dst_3a0;
	Packets_t* _trans_3a1;
	Packets_t* _fcn_3a2;
	Packets_t* _cb_3a3;
	Packets_t* _state_3a4;
	Packets_t* _dst_3a5;
	Packets_t* _trans_3a6;
	Packets_t* _fcn_3a7;
	Packets_t* _cb_3a8;
	Packets_t* _state_3a9;
	Packets_t* _dst_3aa;
	Packets_t* _trans_3ab;
	Packets_t* _fcn_3ac;
	Packets_t* _cb_3ad;
	Packets_t* _state_3ae;
	Packets_t* _dst_3af;
	Packets_t* _trans_3b0;
	Packets_t* _fcn_3b1;
	Packets_t* _cb_3b2;
	Packets_t* _state_3b3;
	Packets_t* _dst_3b4;
	Packets_t* _trans_3b5;
	Packets_t* _fcn_3b6;
	Packets_t* _cb_3b7;
	Packets_t _state_3b8;
	Packets_t _dst_3bf;
	Packets_t _trans_3c6;
	Packets_t _fcn_3cd;
	Packets_t _cb_3d4;
};

class DstIsState_3db
{
public:
	bool operator()( const Packets_t& states_3dc, const Packets_t& dstStates_3df, const Packets_t& transitions_3e2, const Packets_t& functions_3e5, const Packets_t& conditionalBlocks_3e8, Packets_t& states_3de, Packets_t& dstStates_3e1, Packets_t& transitions_3e4, Packets_t& functions_3e7, Packets_t& conditionalBlocks_3ea);

protected:
	bool isInputUnique( const Udm::Object& state_3f4, const Udm::Object& dstState_3fd, const Udm::Object& transition_406, const Udm::Object& function_40f, const Udm::Object& conditionalBlock_418);
	void processInputPackets( const Packets_t& states_3dc, const Packets_t& dstStates_3df, const Packets_t& transitions_3e2, const Packets_t& functions_3e5, const Packets_t& conditionalBlocks_3e8);
	bool patternMatcher( const Udm::Object& state_3f2, const Udm::Object& dstState_3fb, const Udm::Object& transition_404, const Udm::Object& function_40d, const Udm::Object& conditionalBlock_416);
	void outputAppender( const CyberComposition::Simulink::State& state_423, const CyberComposition::Simulink::State& dstState_425, const CyberComposition::Simulink::Transition& transition_427, const SFC::Function& function_429, const SFC::ConditionalBlock& conditionalBlock_42b);

private:
	Packets_t* _state_3eb;
	Packets_t* _dstState_3ec;
	Packets_t* _transition_3ed;
	Packets_t* _function_3ee;
	Packets_t* _conditionalBlock_3ef;
	Packets_t _state_3f0;
	Packets_t _dstState_3f9;
	Packets_t _transition_402;
	Packets_t _function_40b;
	Packets_t _conditionalBlock_414;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_41e;
		CyberComposition::Simulink::State dstState_41f;
		CyberComposition::Simulink::Transition transition_420;
		SFC::Function function_421;
		SFC::ConditionalBlock conditionalBlock_422;
	};

	std::list< Match> _matches;
};

class DstIsRefState_42d
{
public:
	bool operator()( const Packets_t& states_42e, const Packets_t& connectorRefs_431, const Packets_t& transitions_434, const Packets_t& functions_437, const Packets_t& conditionalBlocks_43a, Packets_t& states_430, Packets_t& connectorRefs_433, Packets_t& transitions_436, Packets_t& functions_439, Packets_t& conditionalBlocks_43c);

protected:
	bool isInputUnique( const Udm::Object& state_446, const Udm::Object& connectorRef_44f, const Udm::Object& transition_458, const Udm::Object& function_461, const Udm::Object& conditionalBlock_46a);
	void processInputPackets( const Packets_t& states_42e, const Packets_t& connectorRefs_431, const Packets_t& transitions_434, const Packets_t& functions_437, const Packets_t& conditionalBlocks_43a);
	bool patternMatcher( const Udm::Object& state_444, const Udm::Object& connectorRef_44d, const Udm::Object& transition_456, const Udm::Object& function_45f, const Udm::Object& conditionalBlock_468);
	void outputAppender( const CyberComposition::Simulink::State& state_478, const CyberComposition::Simulink::ConnectorRef& connectorRef_47a, const CyberComposition::Simulink::Transition& transition_47c, const SFC::Function& function_47e, const SFC::ConditionalBlock& conditionalBlock_480);

private:
	Packets_t* _state_43d;
	Packets_t* _connectorRef_43e;
	Packets_t* _transition_43f;
	Packets_t* _function_440;
	Packets_t* _conditionalBlock_441;
	Packets_t _state_442;
	Packets_t _connectorRef_44b;
	Packets_t _transition_454;
	Packets_t _function_45d;
	Packets_t _conditionalBlock_466;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_472;
		CyberComposition::Simulink::ConnectorRef connectorRef_473;
		CyberComposition::Simulink::Transition transition_474;
		SFC::Function function_475;
		SFC::ConditionalBlock conditionalBlock_476;
		CyberComposition::Simulink::State dstState_477;
	};

	std::list< Match> _matches;
};

class DstIsJuncWithTrans_482
{
public:
	bool operator()( const Packets_t& states_483, const Packets_t& junctions_486, const Packets_t& transitions_489, const Packets_t& functions_48c, const Packets_t& conditionalBlocks_48f, Packets_t& states_485, Packets_t& junctions_488, Packets_t& transitions_48b, Packets_t& functions_48e, Packets_t& conditionalBlocks_491);

protected:
	bool isInputUnique( const Udm::Object& state_49b, const Udm::Object& junction_4a4, const Udm::Object& transition_4ad, const Udm::Object& function_4b6, const Udm::Object& conditionalBlock_4bf);
	void processInputPackets( const Packets_t& states_483, const Packets_t& junctions_486, const Packets_t& transitions_489, const Packets_t& functions_48c, const Packets_t& conditionalBlocks_48f);
	bool patternMatcher( const Udm::Object& state_499, const Udm::Object& junction_4a2, const Udm::Object& transition_4ab, const Udm::Object& function_4b4, const Udm::Object& conditionalBlock_4bd);
	void outputAppender( const CyberComposition::Simulink::State& state_4d0, const CyberComposition::Simulink::SF_Junction& junction_4d2, const CyberComposition::Simulink::Transition& transition_4d4, const SFC::Function& function_4d6, const SFC::ConditionalBlock& conditionalBlock_4d8);

private:
	Packets_t* _state_492;
	Packets_t* _junction_493;
	Packets_t* _transition_494;
	Packets_t* _function_495;
	Packets_t* _conditionalBlock_496;
	Packets_t _state_497;
	Packets_t _junction_4a0;
	Packets_t _transition_4a9;
	Packets_t _function_4b2;
	Packets_t _conditionalBlock_4bb;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_4c9;
		CyberComposition::Simulink::SF_Junction junction_4ca;
		CyberComposition::Simulink::Transition transition_4cb;
		SFC::Function function_4cc;
		SFC::ConditionalBlock conditionalBlock_4cd;
		CyberComposition::Simulink::Transition transition2_4ce;
		CyberComposition::Simulink::TransConnector transConnector_4cf;
	};

	std::list< Match> _matches;
};

class DstIsJuncWithRefTrans_4da
{
public:
	bool operator()( const Packets_t& states_4db, const Packets_t& junctions_4de, const Packets_t& transitions_4e1, const Packets_t& functions_4e4, const Packets_t& conditionalBlocks_4e7, Packets_t& states_4dd, Packets_t& junctions_4e0, Packets_t& transitions_4e3, Packets_t& functions_4e6, Packets_t& conditionalBlocks_4e9);

protected:
	bool isInputUnique( const Udm::Object& state_4f3, const Udm::Object& junction_4fc, const Udm::Object& transition_505, const Udm::Object& function_50e, const Udm::Object& conditionalBlock_517);
	void processInputPackets( const Packets_t& states_4db, const Packets_t& junctions_4de, const Packets_t& transitions_4e1, const Packets_t& functions_4e4, const Packets_t& conditionalBlocks_4e7);
	bool patternMatcher( const Udm::Object& state_4f1, const Udm::Object& junction_4fa, const Udm::Object& transition_503, const Udm::Object& function_50c, const Udm::Object& conditionalBlock_515);
	void outputAppender( const CyberComposition::Simulink::State& state_52c, const CyberComposition::Simulink::SF_Junction& junction_52e, const CyberComposition::Simulink::Transition& transition_530, const SFC::Function& function_532, const SFC::ConditionalBlock& conditionalBlock_534);

private:
	Packets_t* _state_4ea;
	Packets_t* _junction_4eb;
	Packets_t* _transition_4ec;
	Packets_t* _function_4ed;
	Packets_t* _conditionalBlock_4ee;
	Packets_t _state_4ef;
	Packets_t _junction_4f8;
	Packets_t _transition_501;
	Packets_t _function_50a;
	Packets_t _conditionalBlock_513;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_524;
		CyberComposition::Simulink::SF_Junction junction_525;
		CyberComposition::Simulink::Transition transition_526;
		SFC::Function function_527;
		SFC::ConditionalBlock conditionalBlock_528;
		CyberComposition::Simulink::ConnectorRef connectorRef_529;
		CyberComposition::Simulink::Transition transition2_52a;
		CyberComposition::Simulink::TransConnector transConnector_52b;
	};

	std::list< Match> _matches;
};

class DstIsJuncRefWithTrans_536
{
public:
	bool operator()( const Packets_t& states_537, const Packets_t& connectorRefs_53a, const Packets_t& transitions_53d, const Packets_t& functions_540, const Packets_t& conditionalBlocks_543, Packets_t& states_539, Packets_t& connectorRefs_53c, Packets_t& transitions_53f, Packets_t& functions_542, Packets_t& conditionalBlocks_545);

protected:
	bool isInputUnique( const Udm::Object& state_54f, const Udm::Object& connectorRef_558, const Udm::Object& transition_561, const Udm::Object& function_56a, const Udm::Object& conditionalBlock_573);
	void processInputPackets( const Packets_t& states_537, const Packets_t& connectorRefs_53a, const Packets_t& transitions_53d, const Packets_t& functions_540, const Packets_t& conditionalBlocks_543);
	bool patternMatcher( const Udm::Object& state_54d, const Udm::Object& connectorRef_556, const Udm::Object& transition_55f, const Udm::Object& function_568, const Udm::Object& conditionalBlock_571);
	void outputAppender( const CyberComposition::Simulink::State& state_587, const CyberComposition::Simulink::ConnectorRef& connectorRef_589, const CyberComposition::Simulink::Transition& transition_58b, const SFC::Function& function_58d, const SFC::ConditionalBlock& conditionalBlock_58f);

private:
	Packets_t* _state_546;
	Packets_t* _connectorRef_547;
	Packets_t* _transition_548;
	Packets_t* _function_549;
	Packets_t* _conditionalBlock_54a;
	Packets_t _state_54b;
	Packets_t _connectorRef_554;
	Packets_t _transition_55d;
	Packets_t _function_566;
	Packets_t _conditionalBlock_56f;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_57f;
		CyberComposition::Simulink::ConnectorRef connectorRef_580;
		CyberComposition::Simulink::Transition transition_581;
		SFC::Function function_582;
		SFC::ConditionalBlock conditionalBlock_583;
		CyberComposition::Simulink::SF_Junction junction_584;
		CyberComposition::Simulink::Transition transition2_585;
		CyberComposition::Simulink::TransConnector transConnector_586;
	};

	std::list< Match> _matches;
};

class DstIsJuncRefWithJuncTrans_591
{
public:
	bool operator()( const Packets_t& states_592, const Packets_t& connectorRefs_595, const Packets_t& transitions_598, const Packets_t& functions_59b, const Packets_t& conditionalBlocks_59e, Packets_t& states_594, Packets_t& connectorRefs_597, Packets_t& transitions_59a, Packets_t& functions_59d, Packets_t& conditionalBlocks_5a0);

protected:
	bool isInputUnique( const Udm::Object& state_5aa, const Udm::Object& connectorRef_5b3, const Udm::Object& transition_5bc, const Udm::Object& function_5c5, const Udm::Object& conditionalBlock_5ce);
	void processInputPackets( const Packets_t& states_592, const Packets_t& connectorRefs_595, const Packets_t& transitions_598, const Packets_t& functions_59b, const Packets_t& conditionalBlocks_59e);
	bool patternMatcher( const Udm::Object& state_5a8, const Udm::Object& connectorRef_5b1, const Udm::Object& transition_5ba, const Udm::Object& function_5c3, const Udm::Object& conditionalBlock_5cc);
	void outputAppender( const CyberComposition::Simulink::State& state_5e2, const CyberComposition::Simulink::ConnectorRef& connectorRef_5e4, const CyberComposition::Simulink::Transition& transition_5e6, const SFC::Function& function_5e8, const SFC::ConditionalBlock& conditionalBlock_5ea);

private:
	Packets_t* _state_5a1;
	Packets_t* _connectorRef_5a2;
	Packets_t* _transition_5a3;
	Packets_t* _function_5a4;
	Packets_t* _conditionalBlock_5a5;
	Packets_t _state_5a6;
	Packets_t _connectorRef_5af;
	Packets_t _transition_5b8;
	Packets_t _function_5c1;
	Packets_t _conditionalBlock_5ca;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_5da;
		CyberComposition::Simulink::ConnectorRef connectorRef_5db;
		CyberComposition::Simulink::Transition transition_5dc;
		SFC::Function function_5dd;
		SFC::ConditionalBlock conditionalBlock_5de;
		CyberComposition::Simulink::SF_Junction junction_5df;
		CyberComposition::Simulink::Transition transition2_5e0;
		CyberComposition::Simulink::TransConnector transConnector_5e1;
	};

	std::list< Match> _matches;
};

class DeadEnd_5ec
{
public:
	bool operator()( const Packets_t& states_5ed, const Packets_t& transConnectors_5f0, const Packets_t& transitions_5f3, const Packets_t& functions_5f6, const Packets_t& conditionalBlocks_5f9, Packets_t& states_5ef, Packets_t& transConnectors_5f2, Packets_t& transitions_5f5, Packets_t& functions_5f8, Packets_t& conditionalBlocks_5fb);

protected:
	bool isInputUnique( const Udm::Object& state_605, const Udm::Object& transConnector_60e, const Udm::Object& transition_617, const Udm::Object& function_620, const Udm::Object& conditionalBlock_629);
	void processInputPackets( const Packets_t& states_5ed, const Packets_t& transConnectors_5f0, const Packets_t& transitions_5f3, const Packets_t& functions_5f6, const Packets_t& conditionalBlocks_5f9);
	bool patternMatcher( const Udm::Object& state_603, const Udm::Object& transConnector_60c, const Udm::Object& transition_615, const Udm::Object& function_61e, const Udm::Object& conditionalBlock_627);
	void outputAppender( const CyberComposition::Simulink::State& state_634, const CyberComposition::Simulink::TransConnector& transConnector_636, const CyberComposition::Simulink::Transition& transition_638, const SFC::Function& function_63a, const SFC::ConditionalBlock& conditionalBlock_63c);

private:
	Packets_t* _state_5fc;
	Packets_t* _transConnector_5fd;
	Packets_t* _transition_5fe;
	Packets_t* _function_5ff;
	Packets_t* _conditionalBlock_600;
	Packets_t _state_601;
	Packets_t _transConnector_60a;
	Packets_t _transition_613;
	Packets_t _function_61c;
	Packets_t _conditionalBlock_625;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_62f;
		CyberComposition::Simulink::TransConnector transConnector_630;
		CyberComposition::Simulink::Transition transition_631;
		SFC::Function function_632;
		SFC::ConditionalBlock conditionalBlock_633;
	};

	std::list< Match> _matches;
};

class OrderTransitions_63e
{
public:
	void operator()( const Packets_t& states_63f, const Packets_t& transitions_642, const Packets_t& functions_645, const Packets_t& compoundStatements_648, Packets_t& states_641, Packets_t& transitions_644, Packets_t& functions_647, Packets_t& compoundStatements_64a);

protected:
	bool isInputUnique( const Udm::Object& state_653, const Udm::Object& transition_65c, const Udm::Object& function_665, const Udm::Object& compoundStatement_66e);
	void processInputPackets( const Packets_t& states_63f, const Packets_t& transitions_642, const Packets_t& functions_645, const Packets_t& compoundStatements_648);
	bool patternMatcher( const Udm::Object& state_651, const Udm::Object& transition_65a, const Udm::Object& function_663, const Udm::Object& compoundStatement_66c);
	void effector();
	void outputAppender( const CyberComposition::Simulink::State& state_678, const CyberComposition::Simulink::Transition& transition_67a, const SFC::Function& function_67c, const SFC::CompoundStatement& compoundStatement_67e);
	void sortOutputs();

private:
	Packets_t* _state_64b;
	Packets_t* _transition_64c;
	Packets_t* _function_64d;
	Packets_t* _compoundStatement_64e;
	Packets_t _state_64f;
	Packets_t _transition_658;
	Packets_t _function_661;
	Packets_t _compoundStatement_66a;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_674;
		CyberComposition::Simulink::Transition transition_675;
		SFC::Function function_676;
		SFC::CompoundStatement compoundStatement_677;
	};

	std::list< Match> _matches;
};

class GetAllTransitions_6b3
{
public:
	void operator()( const Packets_t& states_6b4, const Packets_t& tcs_6b6, const Packets_t& fcns_6b8, const Packets_t& css_6ba, Packets_t& states_6bc, Packets_t& transs_6bd, Packets_t& fcns_6be, Packets_t& css_6bf);

protected:
	void callGetTransitionsFromJunctionRefs_7af( const Packets_t& states_6c5, const Packets_t& junctions_6c9, const Packets_t& functions_6cb, const Packets_t& compoundStatements_6ce);
	void callGetDirectTransitions_7b4( const Packets_t& states_711, const Packets_t& srcTransConnectors_714, const Packets_t& functions_717, const Packets_t& compoundStatements_71a);
	void callGetTransitionsFromStateRefs_7b9( const Packets_t& states_759, const Packets_t& tCStates_75d, const Packets_t& functions_75f, const Packets_t& compoundStatements_762);

private:
	Packets_t* _state_6c0;
	Packets_t* _trans_6c1;
	Packets_t* _fcn_6c2;
	Packets_t* _cs_6c3;
};

class GetTransitionsFromJunctionRefs_6c4
{
public:
	void operator()( const Packets_t& states_6c5, const Packets_t& junctions_6c9, const Packets_t& functions_6cb, const Packets_t& compoundStatements_6ce, Packets_t& states_6c7, Packets_t& transitions_6c8, Packets_t& functions_6cd, Packets_t& compoundStatements_6d0);

protected:
	bool isInputUnique( const Udm::Object& state_6d9, const Udm::Object& junction_6e2, const Udm::Object& function_6eb, const Udm::Object& compoundStatement_6f4);
	void processInputPackets( const Packets_t& states_6c5, const Packets_t& junctions_6c9, const Packets_t& functions_6cb, const Packets_t& compoundStatements_6ce);
	bool patternMatcher( const Udm::Object& state_6d7, const Udm::Object& junction_6e0, const Udm::Object& function_6e9, const Udm::Object& compoundStatement_6f2);
	void effector();
	void outputAppender( const CyberComposition::Simulink::State& state_708, const CyberComposition::Simulink::Transition& transition_70a, const SFC::Function& function_70c, const SFC::CompoundStatement& compoundStatement_70e);

private:
	Packets_t* _state_6d1;
	Packets_t* _transition_6d2;
	Packets_t* _function_6d3;
	Packets_t* _compoundStatement_6d4;
	Packets_t _state_6d5;
	Packets_t _junction_6de;
	Packets_t _function_6e7;
	Packets_t _compoundStatement_6f0;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_701;
		CyberComposition::Simulink::SF_Junction junction_702;
		SFC::Function function_703;
		SFC::CompoundStatement compoundStatement_704;
		CyberComposition::Simulink::Transition transition_705;
		CyberComposition::Simulink::ConnectorRef connectorRef_706;
		CyberComposition::Simulink::TransConnector dstTransConnector_707;
	};

	std::list< Match> _matches;
};

class GetDirectTransitions_710
{
public:
	void operator()( const Packets_t& states_711, const Packets_t& srcTransConnectors_714, const Packets_t& functions_717, const Packets_t& compoundStatements_71a, Packets_t& states_713, Packets_t& transitions_716, Packets_t& functions_719, Packets_t& compoundStatements_71c);

protected:
	bool isInputUnique( const Udm::Object& state_725, const Udm::Object& srcTransConnector_72e, const Udm::Object& function_737, const Udm::Object& compoundStatement_740);
	void processInputPackets( const Packets_t& states_711, const Packets_t& srcTransConnectors_714, const Packets_t& functions_717, const Packets_t& compoundStatements_71a);
	bool patternMatcher( const Udm::Object& state_723, const Udm::Object& srcTransConnector_72c, const Udm::Object& function_735, const Udm::Object& compoundStatement_73e);
	void effector();
	void outputAppender( const CyberComposition::Simulink::State& state_750, const CyberComposition::Simulink::Transition& transition_752, const SFC::Function& function_754, const SFC::CompoundStatement& compoundStatement_756);

private:
	Packets_t* _state_71d;
	Packets_t* _transition_71e;
	Packets_t* _function_71f;
	Packets_t* _compoundStatement_720;
	Packets_t _state_721;
	Packets_t _srcTransConnector_72a;
	Packets_t _function_733;
	Packets_t _compoundStatement_73c;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_74a;
		CyberComposition::Simulink::TransConnector srcTransConnector_74b;
		SFC::Function function_74c;
		SFC::CompoundStatement compoundStatement_74d;
		CyberComposition::Simulink::Transition transition_74e;
		CyberComposition::Simulink::TransConnector dstTransConnector_74f;
	};

	std::list< Match> _matches;
};

class GetTransitionsFromStateRefs_758
{
public:
	void operator()( const Packets_t& states_759, const Packets_t& tCStates_75d, const Packets_t& functions_75f, const Packets_t& compoundStatements_762, Packets_t& states_75b, Packets_t& transitions_75c, Packets_t& functions_761, Packets_t& compoundStatements_764);

protected:
	bool isInputUnique( const Udm::Object& state_76d, const Udm::Object& tCState_776, const Udm::Object& function_77f, const Udm::Object& compoundStatement_788);
	bool isGuardTrue( SFC::CompoundStatement& CompoundStatement, CyberComposition::Simulink::ConnectorRef& ConnectorRef, CyberComposition::Simulink::TransConnector& DstTransConnector, SFC::Function& Function, CyberComposition::Simulink::State& RefParentState, CyberComposition::Simulink::State& State, CyberComposition::Simulink::State& TCState, CyberComposition::Simulink::Transition& Transition);
	void processInputPackets( const Packets_t& states_759, const Packets_t& tCStates_75d, const Packets_t& functions_75f, const Packets_t& compoundStatements_762);
	bool patternMatcher( const Udm::Object& state_76b, const Udm::Object& tCState_774, const Udm::Object& function_77d, const Udm::Object& compoundStatement_786);
	void effector();
	void outputAppender( const CyberComposition::Simulink::State& state_7a7, const CyberComposition::Simulink::Transition& transition_7a9, const SFC::Function& function_7ab, const SFC::CompoundStatement& compoundStatement_7ad);

private:
	Packets_t* _state_765;
	Packets_t* _transition_766;
	Packets_t* _function_767;
	Packets_t* _compoundStatement_768;
	Packets_t _state_769;
	Packets_t _tCState_772;
	Packets_t _function_77b;
	Packets_t _compoundStatement_784;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_797;
		CyberComposition::Simulink::State tCState_798;
		SFC::Function function_799;
		SFC::CompoundStatement compoundStatement_79a;
		CyberComposition::Simulink::State refParentState_79b;
		CyberComposition::Simulink::Transition transition_79c;
		CyberComposition::Simulink::ConnectorRef connectorRef_79d;
		CyberComposition::Simulink::TransConnector dstTransConnector_79e;
	};

	std::list< Match> _matches;
};

class CreateTransitionCG_7c8
{
public:
	void operator()( const Packets_t& states_7c9, const Packets_t& transConnectors_7cc, const Packets_t& functions_7cf, const Packets_t& tCVarCGs_7d3, Packets_t& states_7cb, Packets_t& transConnectors_7ce, Packets_t& functions_7d1, Packets_t& transitionCGs_7d2);

protected:
	bool isInputUnique( const Udm::Object& state_7dd, const Udm::Object& transConnector_7e6, const Udm::Object& function_7ef, const Udm::Object& tCVarCG_7f8);
	bool isGuardTrue( SFC::Function& Function, CyberComposition::Simulink::State& State, SFC::LocalVar& TCVar, SFC::ConditionalGroup& TCVarCG, CyberComposition::Simulink::TransConnector& TransConnector);
	void processInputPackets( const Packets_t& states_7c9, const Packets_t& transConnectors_7cc, const Packets_t& functions_7cf, const Packets_t& tCVarCGs_7d3);
	bool patternMatcher( const Udm::Object& state_7db, const Udm::Object& transConnector_7e4, const Udm::Object& function_7ed, const Udm::Object& tCVarCG_7f6);
	void effector();
	void outputAppender( const CyberComposition::Simulink::State& state_811, const CyberComposition::Simulink::TransConnector& transConnector_813, const SFC::Function& function_815, const SFC::ConditionalGroup& transitionCG_817);

private:
	Packets_t* _state_7d5;
	Packets_t* _transConnector_7d6;
	Packets_t* _function_7d7;
	Packets_t* _transitionCG_7d8;
	Packets_t _state_7d9;
	Packets_t _transConnector_7e2;
	Packets_t _function_7eb;
	Packets_t _tCVarCG_7f4;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_801;
		CyberComposition::Simulink::TransConnector transConnector_802;
		SFC::Function function_803;
		SFC::ConditionalGroup tCVarCG_804;
		SFC::LocalVar tCVar_805;
	};

	std::list< Match> _matches;
};

class ProcessedFilter_819
{
public:
	void operator()( const Packets_t& states_81a, const Packets_t& tcs_81c, const Packets_t& fcns_81e, const Packets_t& cgs_820, Packets_t& states_822, Packets_t& tcs_823, Packets_t& fcns_824, Packets_t& cgs_825);

protected:
	void executeOne( const Packets_t& states_81a, const Packets_t& tcs_81c, const Packets_t& fcns_81e, const Packets_t& cgs_820);
	bool isInputUnique( const Udm::Object& state_82c, const Udm::Object& tc_833, const Udm::Object& fcn_83a, const Udm::Object& cg_841);

private:
	Packets_t* _state_826;
	Packets_t* _tc_827;
	Packets_t* _fcn_828;
	Packets_t* _cg_829;
	Packets_t _state_82a;
	Packets_t _tc_831;
	Packets_t _fcn_838;
	Packets_t _cg_83f;
};

class TCNotProcessed_846
{
public:
	bool operator()( const Packets_t& states_847, const Packets_t& transConnectors_84a, const Packets_t& functions_84d, const Packets_t& conditionalGroups_850, Packets_t& states_849, Packets_t& transConnectors_84c, Packets_t& functions_84f, Packets_t& conditionalGroups_852);

protected:
	bool isInputUnique( const Udm::Object& state_85b, const Udm::Object& transConnector_864, const Udm::Object& function_86d, const Udm::Object& conditionalGroup_876);
	bool isGuardTrue( SFC::ConditionalGroup& ConditionalGroup, SFC::Function& Function, CyberComposition::Simulink::State& State, CyberComposition::Simulink::TransConnector& TransConnector);
	void processInputPackets( const Packets_t& states_847, const Packets_t& transConnectors_84a, const Packets_t& functions_84d, const Packets_t& conditionalGroups_850);
	bool patternMatcher( const Udm::Object& state_859, const Udm::Object& transConnector_862, const Udm::Object& function_86b, const Udm::Object& conditionalGroup_874);
	void outputAppender( const CyberComposition::Simulink::State& state_884, const CyberComposition::Simulink::TransConnector& transConnector_886, const SFC::Function& function_888, const SFC::ConditionalGroup& conditionalGroup_88a);

private:
	Packets_t* _state_853;
	Packets_t* _transConnector_854;
	Packets_t* _function_855;
	Packets_t* _conditionalGroup_856;
	Packets_t _state_857;
	Packets_t _transConnector_860;
	Packets_t _function_869;
	Packets_t _conditionalGroup_872;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_87c;
		CyberComposition::Simulink::TransConnector transConnector_87d;
		SFC::Function function_87e;
		SFC::ConditionalGroup conditionalGroup_87f;
	};

	std::list< Match> _matches;
};

class NoDefaultTransition_88c
{
public:
	void operator()( const Packets_t& states_88d, const Packets_t& tcs_88f, const Packets_t& fcns_891, const Packets_t& css_893);

protected:
	void callNDTTest_9dc( const Packets_t& states_8db, const Packets_t& tcs_8dd, const Packets_t& fcns_8df, const Packets_t& css_8e1);
	void callExitLoop_9e1( const Packets_t& states_896, const Packets_t& srcTransConnectors_898, const Packets_t& functions_89a, const Packets_t& compoundStatements_89c);
};

class ExitLoop_895
{
public:
	void operator()( const Packets_t& states_896, const Packets_t& srcTransConnectors_898, const Packets_t& functions_89a, const Packets_t& compoundStatements_89c);

protected:
	bool isInputUnique( const Udm::Object& state_8a2, const Udm::Object& srcTransConnector_8ab, const Udm::Object& function_8b4, const Udm::Object& compoundStatement_8bd);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::CompoundStatement& CompoundStatement, SFC::Function& Function, CyberComposition::Simulink::TransConnector& SrcTransConnector, CyberComposition::Simulink::State& State, SFC::LocalVar& TCVar);
	void processInputPackets( const Packets_t& states_896, const Packets_t& srcTransConnectors_898, const Packets_t& functions_89a, const Packets_t& compoundStatements_89c);
	bool patternMatcher( const Udm::Object& state_8a0, const Udm::Object& srcTransConnector_8a9, const Udm::Object& function_8b2, const Udm::Object& compoundStatement_8bb);
	void effector();

private:
	Packets_t _state_89e;
	Packets_t _srcTransConnector_8a7;
	Packets_t _function_8b0;
	Packets_t _compoundStatement_8b9;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_8c9;
		CyberComposition::Simulink::TransConnector srcTransConnector_8ca;
		SFC::Function function_8cb;
		SFC::CompoundStatement compoundStatement_8cc;
		SFC::LocalVar tCVar_8cd;
	};

	std::list< Match> _matches;
};

class NDTTest_8da
{
public:
	void operator()( const Packets_t& states_8db, const Packets_t& tcs_8dd, const Packets_t& fcns_8df, const Packets_t& css_8e1, Packets_t& states_8e3, Packets_t& tcs_8e4, Packets_t& fcns_8e5, Packets_t& css_8e6);

protected:
	void executeOne( const Packets_t& states_8db, const Packets_t& tcs_8dd, const Packets_t& fcns_8df, const Packets_t& css_8e1);
	bool isInputUnique( const Udm::Object& state_8ed, const Udm::Object& tc_8f4, const Udm::Object& fcn_8fb, const Udm::Object& cs_902);

private:
	Packets_t* _state_8e7;
	Packets_t* _tc_8e8;
	Packets_t* _fcn_8e9;
	Packets_t* _cs_8ea;
	Packets_t _state_8eb;
	Packets_t _tc_8f2;
	Packets_t _fcn_8f9;
	Packets_t _cs_900;
};

class HasDefaultTransitionDouble_907
{
public:
	bool operator()( const Packets_t& states_908, const Packets_t& srcTransConnectors_90a, const Packets_t& functions_90c, const Packets_t& compoundStatements_90e);

protected:
	bool isInputUnique( const Udm::Object& state_914, const Udm::Object& srcTransConnector_91d, const Udm::Object& function_926, const Udm::Object& compoundStatement_92f);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::CompoundStatement& CompoundStatement, SFC::ConditionalBlock& ConditionalBlock, SFC::Double& Double, SFC::Function& Function, CyberComposition::Simulink::TransConnector& SrcTransConnector, CyberComposition::Simulink::State& State, SFC::UserCode& UserCode);
	void processInputPackets( const Packets_t& states_908, const Packets_t& srcTransConnectors_90a, const Packets_t& functions_90c, const Packets_t& compoundStatements_90e);
	bool patternMatcher( const Udm::Object& state_912, const Udm::Object& srcTransConnector_91b, const Udm::Object& function_924, const Udm::Object& compoundStatement_92d);
	void outputAppender();

private:
	Packets_t _state_910;
	Packets_t _srcTransConnector_919;
	Packets_t _function_922;
	Packets_t _compoundStatement_92b;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_941;
		CyberComposition::Simulink::TransConnector srcTransConnector_942;
		SFC::Function function_943;
		SFC::CompoundStatement compoundStatement_944;
		SFC::UserCode userCode_945;
		SFC::Double double_946;
		SFC::ConditionalBlock conditionalBlock_947;
	};

	std::list< Match> _matches;
};

class HasDefaultTransitionInt_94f
{
public:
	bool operator()( const Packets_t& states_950, const Packets_t& srcTransConnectors_952, const Packets_t& functions_954, const Packets_t& compoundStatements_956);

protected:
	bool isInputUnique( const Udm::Object& state_95c, const Udm::Object& srcTransConnector_965, const Udm::Object& function_96e, const Udm::Object& compoundStatement_977);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::CompoundStatement& CompoundStatement, SFC::ConditionalBlock& ConditionalBlock, SFC::Function& Function, SFC::Int& Int, CyberComposition::Simulink::TransConnector& SrcTransConnector, CyberComposition::Simulink::State& State, SFC::UserCode& UserCode);
	void processInputPackets( const Packets_t& states_950, const Packets_t& srcTransConnectors_952, const Packets_t& functions_954, const Packets_t& compoundStatements_956);
	bool patternMatcher( const Udm::Object& state_95a, const Udm::Object& srcTransConnector_963, const Udm::Object& function_96c, const Udm::Object& compoundStatement_975);
	void outputAppender();

private:
	Packets_t _state_958;
	Packets_t _srcTransConnector_961;
	Packets_t _function_96a;
	Packets_t _compoundStatement_973;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_989;
		CyberComposition::Simulink::TransConnector srcTransConnector_98a;
		SFC::Function function_98b;
		SFC::CompoundStatement compoundStatement_98c;
		SFC::UserCode userCode_98d;
		SFC::Int int_98e;
		SFC::ConditionalBlock conditionalBlock_98f;
	};

	std::list< Match> _matches;
};

class NoDefault_997
{
public:
	bool operator()( const Packets_t& states_998, const Packets_t& transConnectors_99b, const Packets_t& functions_99e, const Packets_t& compoundStatements_9a1, Packets_t& states_99a, Packets_t& transConnectors_99d, Packets_t& functions_9a0, Packets_t& compoundStatements_9a3);

protected:
	bool isInputUnique( const Udm::Object& state_9ac, const Udm::Object& transConnector_9b5, const Udm::Object& function_9be, const Udm::Object& compoundStatement_9c7);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& states_998, const Packets_t& transConnectors_99b, const Packets_t& functions_99e, const Packets_t& compoundStatements_9a1);
	bool patternMatcher( const Udm::Object& state_9aa, const Udm::Object& transConnector_9b3, const Udm::Object& function_9bc, const Udm::Object& compoundStatement_9c5);
	void outputAppender( const CyberComposition::Simulink::State& state_9d4, const CyberComposition::Simulink::TransConnector& transConnector_9d6, const SFC::Function& function_9d8, const SFC::CompoundStatement& compoundStatement_9da);

private:
	Packets_t* _state_9a4;
	Packets_t* _transConnector_9a5;
	Packets_t* _function_9a6;
	Packets_t* _compoundStatement_9a7;
	Packets_t _state_9a8;
	Packets_t _transConnector_9b1;
	Packets_t _function_9ba;
	Packets_t _compoundStatement_9c3;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_9d0;
		CyberComposition::Simulink::TransConnector transConnector_9d1;
		SFC::Function function_9d2;
		SFC::CompoundStatement compoundStatement_9d3;
	};

	std::list< Match> _matches;
};

class TransStartMap_a07
{
public:
	void operator()( const Packets_t& states_a08, const Packets_t& css_a0a, Packets_t& states_a0c, Packets_t& css_a0d);

protected:
	void callInitTransConnectorMap_c1c( const Packets_t& states_a11);
	void callTransConnectorMapRecurse_c1e( const Packets_t& states_a2b, const Packets_t& tcs_a2d);

private:
	Packets_t* _state_a0e;
	Packets_t* _cs_a0f;
};

class InitTransConnectorMap_a10
{
public:
	void operator()( const Packets_t& states_a11, Packets_t& states_a13, Packets_t& transStarts_a14);

protected:
	bool isInputUnique( const Udm::Object& state_a1b);
	void processInputPackets( const Packets_t& states_a11);
	bool patternMatcher( const Udm::Object& state_a19);
	void effector();
	void outputAppender( const CyberComposition::Simulink::State& state_a26, const CyberComposition::Simulink::TransStart& transStart_a28);

private:
	Packets_t* _state_a15;
	Packets_t* _transStart_a16;
	Packets_t _state_a17;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_a24;
		CyberComposition::Simulink::TransStart transStart_a25;
	};

	std::list< Match> _matches;
};

class TransConnectorMapRecurse_a2a
{
public:
	void operator()( const Packets_t& states_a2b, const Packets_t& tcs_a2d);

protected:
	void executeOne( const Packets_t& states_a2b, const Packets_t& tcs_a2d);
	bool isInputUnique( const Udm::Object& state_a31, const Udm::Object& tc_a38);
	void callProcessedFilter_c10( const Packets_t& states_bd6, const Packets_t& tcs_bd8);
	void callAddTransConnector_c13( const Packets_t& states_ad9, const Packets_t& transConnectors_adc);
	void callGetAllTransitions_c16( const Packets_t& states_a3e, const Packets_t& tcs_a40);
	void callProcessTransitionsMap_c19( const Packets_t& states_af7, const Packets_t& transs_af9);

private:
	Packets_t _state_a2f;
	Packets_t _tc_a36;
};

class GetAllTransitions_a3d
{
public:
	void operator()( const Packets_t& states_a3e, const Packets_t& tcs_a40, Packets_t& states_a42, Packets_t& transs_a43);

protected:
	void callGetTransitionsFromJunctionRefs_acf( const Packets_t& states_aa4, const Packets_t& junctions_aa8);
	void callGetDirectTransitions_ad2( const Packets_t& states_a7c, const Packets_t& srcTransConnectors_a7f);
	void callGetTransitionsFromStateRefs_ad5( const Packets_t& states_a47, const Packets_t& tCStates_a4b);

private:
	Packets_t* _state_a44;
	Packets_t* _trans_a45;
};

class GetTransitionsFromStateRefs_a46
{
public:
	void operator()( const Packets_t& states_a47, const Packets_t& tCStates_a4b, Packets_t& states_a49, Packets_t& transitions_a4a);

protected:
	bool isInputUnique( const Udm::Object& state_a53, const Udm::Object& tCState_a5c);
	bool isGuardTrue( CyberComposition::Simulink::ConnectorRef& ConnectorRef, CyberComposition::Simulink::TransConnector& DstTransConnector, CyberComposition::Simulink::State& RefParentState, CyberComposition::Simulink::State& State, CyberComposition::Simulink::State& TCState, CyberComposition::Simulink::Transition& Transition);
	void processInputPackets( const Packets_t& states_a47, const Packets_t& tCStates_a4b);
	bool patternMatcher( const Udm::Object& state_a51, const Udm::Object& tCState_a5a);
	void effector();
	void outputAppender( const CyberComposition::Simulink::State& state_a77, const CyberComposition::Simulink::Transition& transition_a79);

private:
	Packets_t* _state_a4d;
	Packets_t* _transition_a4e;
	Packets_t _state_a4f;
	Packets_t _tCState_a58;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_a6b;
		CyberComposition::Simulink::State tCState_a6c;
		CyberComposition::Simulink::State refParentState_a6d;
		CyberComposition::Simulink::Transition transition_a6e;
		CyberComposition::Simulink::ConnectorRef connectorRef_a6f;
		CyberComposition::Simulink::TransConnector dstTransConnector_a70;
	};

	std::list< Match> _matches;
};

class GetDirectTransitions_a7b
{
public:
	void operator()( const Packets_t& states_a7c, const Packets_t& srcTransConnectors_a7f, Packets_t& states_a7e, Packets_t& transitions_a81);

protected:
	bool isInputUnique( const Udm::Object& state_a88, const Udm::Object& srcTransConnector_a91);
	void processInputPackets( const Packets_t& states_a7c, const Packets_t& srcTransConnectors_a7f);
	bool patternMatcher( const Udm::Object& state_a86, const Udm::Object& srcTransConnector_a8f);
	void effector();
	void outputAppender( const CyberComposition::Simulink::State& state_a9f, const CyberComposition::Simulink::Transition& transition_aa1);

private:
	Packets_t* _state_a82;
	Packets_t* _transition_a83;
	Packets_t _state_a84;
	Packets_t _srcTransConnector_a8d;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_a9b;
		CyberComposition::Simulink::TransConnector srcTransConnector_a9c;
		CyberComposition::Simulink::Transition transition_a9d;
		CyberComposition::Simulink::TransConnector dstTransConnector_a9e;
	};

	std::list< Match> _matches;
};

class GetTransitionsFromJunctionRefs_aa3
{
public:
	void operator()( const Packets_t& states_aa4, const Packets_t& junctions_aa8, Packets_t& states_aa6, Packets_t& transitions_aa7);

protected:
	bool isInputUnique( const Udm::Object& state_ab0, const Udm::Object& junction_ab9);
	void processInputPackets( const Packets_t& states_aa4, const Packets_t& junctions_aa8);
	bool patternMatcher( const Udm::Object& state_aae, const Udm::Object& junction_ab7);
	void effector();
	void outputAppender( const CyberComposition::Simulink::State& state_acb, const CyberComposition::Simulink::Transition& transition_acd);

private:
	Packets_t* _state_aaa;
	Packets_t* _transition_aab;
	Packets_t _state_aac;
	Packets_t _junction_ab5;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_ac6;
		CyberComposition::Simulink::SF_Junction junction_ac7;
		CyberComposition::Simulink::Transition transition_ac8;
		CyberComposition::Simulink::ConnectorRef connectorRef_ac9;
		CyberComposition::Simulink::TransConnector dstTransConnector_aca;
	};

	std::list< Match> _matches;
};

class AddTransConnector_ad8
{
public:
	void operator()( const Packets_t& states_ad9, const Packets_t& transConnectors_adc, Packets_t& states_adb, Packets_t& transConnectors_ade);

protected:
	bool isInputUnique( const Udm::Object& state_ae5, const Udm::Object& transConnector_aee);
	void processInputPackets( const Packets_t& states_ad9, const Packets_t& transConnectors_adc);
	bool patternMatcher( const Udm::Object& state_ae3, const Udm::Object& transConnector_aec);
	void effector();
	void forwardInputs();

private:
	Packets_t* _state_adf;
	Packets_t* _transConnector_ae0;
	Packets_t _state_ae1;
	Packets_t _transConnector_aea;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_af4;
		CyberComposition::Simulink::TransConnector transConnector_af5;
	};

	std::list< Match> _matches;
};

class ProcessTransitionsMap_af6
{
public:
	void operator()( const Packets_t& states_af7, const Packets_t& transs_af9);

protected:
	void callOrderTransitions_bc6( const Packets_t& states_b7f, const Packets_t& transitions_b82);
	void callgetDstTransConnectors_bc9( const Packets_t& states_ba1, const Packets_t& transitions_ba5);
	void callTransConnTest_bcc( const Packets_t& states_b20, const Packets_t& tcs_b22);
	void callTransConnectorMapRecurse_bcf( const Packets_t& states_a2b, const Packets_t& tcs_a2d);
	void callDerefConnRef_bd2( const Packets_t& states_afc, const Packets_t& connectorRefs_aff);
};

class DerefConnRef_afb
{
public:
	void operator()( const Packets_t& states_afc, const Packets_t& connectorRefs_aff, Packets_t& states_afe, Packets_t& transConnectors_b01);

protected:
	bool isInputUnique( const Udm::Object& state_b08, const Udm::Object& connectorRef_b11);
	void processInputPackets( const Packets_t& states_afc, const Packets_t& connectorRefs_aff);
	bool patternMatcher( const Udm::Object& state_b06, const Udm::Object& connectorRef_b0f);
	void effector();
	void outputAppender( const CyberComposition::Simulink::State& state_b1b, const CyberComposition::Simulink::TransConnector& transConnector_b1d);

private:
	Packets_t* _state_b02;
	Packets_t* _transConnector_b03;
	Packets_t _state_b04;
	Packets_t _connectorRef_b0d;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_b18;
		CyberComposition::Simulink::ConnectorRef connectorRef_b19;
		CyberComposition::Simulink::TransConnector transConnector_b1a;
	};

	std::list< Match> _matches;
};

class TransConnTest_b1f
{
public:
	void operator()( const Packets_t& states_b20, const Packets_t& tcs_b22, Packets_t& states_b24, Packets_t& tcs_b25, Packets_t& states_b26, Packets_t& tcs_b27);

protected:
	void executeOne( const Packets_t& states_b20, const Packets_t& tcs_b22);
	bool isInputUnique( const Udm::Object& state_b2e, const Udm::Object& tc_b35);

private:
	Packets_t* _state_b28;
	Packets_t* _tc_b29;
	Packets_t* _state_b2a;
	Packets_t* _tc_b2b;
	Packets_t _state_b2c;
	Packets_t _tc_b33;
};

class IsConnectorRef_b3a
{
public:
	bool operator()( const Packets_t& states_b3b, const Packets_t& connectorRefs_b3e, Packets_t& states_b3d, Packets_t& connectorRefs_b40);

protected:
	bool isInputUnique( const Udm::Object& state_b47, const Udm::Object& connectorRef_b50);
	void processInputPackets( const Packets_t& states_b3b, const Packets_t& connectorRefs_b3e);
	bool patternMatcher( const Udm::Object& state_b45, const Udm::Object& connectorRef_b4e);
	void outputAppender( const CyberComposition::Simulink::State& state_b58, const CyberComposition::Simulink::ConnectorRef& connectorRef_b5a);

private:
	Packets_t* _state_b41;
	Packets_t* _connectorRef_b42;
	Packets_t _state_b43;
	Packets_t _connectorRef_b4c;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_b56;
		CyberComposition::Simulink::ConnectorRef connectorRef_b57;
	};

	std::list< Match> _matches;
};

class Otherwise_b5c
{
public:
	bool operator()( const Packets_t& states_b5d, const Packets_t& junctions_b60, Packets_t& states_b5f, Packets_t& junctions_b62);

protected:
	bool isInputUnique( const Udm::Object& state_b69, const Udm::Object& junction_b72);
	void processInputPackets( const Packets_t& states_b5d, const Packets_t& junctions_b60);
	bool patternMatcher( const Udm::Object& state_b67, const Udm::Object& junction_b70);
	void outputAppender( const CyberComposition::Simulink::State& state_b7a, const CyberComposition::Simulink::SF_Junction& junction_b7c);

private:
	Packets_t* _state_b63;
	Packets_t* _junction_b64;
	Packets_t _state_b65;
	Packets_t _junction_b6e;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_b78;
		CyberComposition::Simulink::SF_Junction junction_b79;
	};

	std::list< Match> _matches;
};

class OrderTransitions_b7e
{
public:
	void operator()( const Packets_t& states_b7f, const Packets_t& transitions_b82, Packets_t& states_b81, Packets_t& transitions_b84);

protected:
	bool isInputUnique( const Udm::Object& state_b8b, const Udm::Object& transition_b94);
	void processInputPackets( const Packets_t& states_b7f, const Packets_t& transitions_b82);
	bool patternMatcher( const Udm::Object& state_b89, const Udm::Object& transition_b92);
	void effector();
	void outputAppender( const CyberComposition::Simulink::State& state_b9c, const CyberComposition::Simulink::Transition& transition_b9e);
	void sortOutputs();

private:
	Packets_t* _state_b85;
	Packets_t* _transition_b86;
	Packets_t _state_b87;
	Packets_t _transition_b90;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_b9a;
		CyberComposition::Simulink::Transition transition_b9b;
	};

	std::list< Match> _matches;
};

class GetDstTransConnectors_ba0
{
public:
	void operator()( const Packets_t& states_ba1, const Packets_t& transitions_ba5, Packets_t& states_ba3, Packets_t& dstTransConnectors_ba4);

protected:
	bool isInputUnique( const Udm::Object& state_bad, const Udm::Object& transition_bb6);
	void processInputPackets( const Packets_t& states_ba1, const Packets_t& transitions_ba5);
	bool patternMatcher( const Udm::Object& state_bab, const Udm::Object& transition_bb4);
	void effector();
	void outputAppender( const CyberComposition::Simulink::State& state_bc2, const CyberComposition::Simulink::TransConnector& dstTransConnector_bc4);

private:
	Packets_t* _state_ba7;
	Packets_t* _dstTransConnector_ba8;
	Packets_t _state_ba9;
	Packets_t _transition_bb2;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_bbe;
		CyberComposition::Simulink::Transition transition_bbf;
		CyberComposition::Simulink::TransConnector transConnector_bc0;
		CyberComposition::Simulink::TransConnector dstTransConnector_bc1;
	};

	std::list< Match> _matches;
};

class ProcessedFilter_bd5
{
public:
	void operator()( const Packets_t& states_bd6, const Packets_t& tcs_bd8, Packets_t& states_bda, Packets_t& tcs_bdb);

protected:
	void executeOne( const Packets_t& states_bd6, const Packets_t& tcs_bd8);
	bool isInputUnique( const Udm::Object& state_be0, const Udm::Object& tc_be7);

private:
	Packets_t* _state_bdc;
	Packets_t* _tc_bdd;
	Packets_t _state_bde;
	Packets_t _tc_be5;
};

class TCNotProcessed_bec
{
public:
	bool operator()( const Packets_t& states_bed, const Packets_t& transConnectors_bf0, Packets_t& states_bef, Packets_t& transConnectors_bf2);

protected:
	bool isInputUnique( const Udm::Object& state_bf9, const Udm::Object& transConnector_c02);
	bool isGuardTrue( CyberComposition::Simulink::State& State, CyberComposition::Simulink::TransConnector& TransConnector);
	void processInputPackets( const Packets_t& states_bed, const Packets_t& transConnectors_bf0);
	bool patternMatcher( const Udm::Object& state_bf7, const Udm::Object& transConnector_c00);
	void outputAppender( const CyberComposition::Simulink::State& state_c0c, const CyberComposition::Simulink::TransConnector& transConnector_c0e);

private:
	Packets_t* _state_bf3;
	Packets_t* _transConnector_bf4;
	Packets_t _state_bf5;
	Packets_t _transConnector_bfe;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_c08;
		CyberComposition::Simulink::TransConnector transConnector_c09;
	};

	std::list< Match> _matches;
};

class PopulateEMFSFunction_c2e
{
public:
	void operator()( const Packets_t& states_c2f, const Packets_t& programs_c31, Packets_t& states_c33, Packets_t& programs_c34);

protected:
	void callGetEMFuncStateFunction_c85( const Packets_t& states_c55, const Packets_t& programs_c58);
	void callFunctionBody_c88( const Packets_t& states_c38, const Packets_t& execs_c3a);

private:
	Packets_t* _state_c35;
	Packets_t* _program_c36;
};

class FunctionBody_c37
{
public:
	void operator()( const Packets_t& states_c38, const Packets_t& execs_c3a);

protected:
	bool isInputUnique( const Udm::Object& state_c40, const Udm::Object& exec_c49);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& states_c38, const Packets_t& execs_c3a);
	bool patternMatcher( const Udm::Object& state_c3e, const Udm::Object& exec_c47);
	void effector();

private:
	Packets_t _state_c3c;
	Packets_t _exec_c45;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_c52;
		SFC::Function exec_c53;
	};

	std::list< Match> _matches;
};

class GetEMFuncStateFunction_c54
{
public:
	void operator()( const Packets_t& states_c55, const Packets_t& programs_c58, Packets_t& states_c57, Packets_t& execs_c5a);

protected:
	bool isInputUnique( const Udm::Object& state_c61, const Udm::Object& program_c6a);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::Function& Exec, SFC::Program& Program, CyberComposition::Simulink::State& State, SFC::StateLabel& StateLabel);
	void processInputPackets( const Packets_t& states_c55, const Packets_t& programs_c58);
	bool patternMatcher( const Udm::Object& state_c5f, const Udm::Object& program_c68);
	void effector();
	void outputAppender( const CyberComposition::Simulink::State& state_c81, const SFC::Function& exec_c83);

private:
	Packets_t* _state_c5b;
	Packets_t* _exec_c5c;
	Packets_t _state_c5d;
	Packets_t _program_c66;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_c79;
		SFC::Program program_c7a;
		SFC::StateLabel stateLabel_c7b;
		SFC::Function exec_c7c;
	};

	std::list< Match> _matches;
};

class GetSubStates_c8b
{
public:
	void operator()( const Packets_t& states_c8c, const Packets_t& parStmnts_c8f, Packets_t& subStates_c8e, Packets_t& parStmnts_c91);

protected:
	bool isInputUnique( const Udm::Object& state_c98, const Udm::Object& parStmnt_ca1);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& states_c8c, const Packets_t& parStmnts_c8f);
	bool patternMatcher( const Udm::Object& state_c96, const Udm::Object& parStmnt_c9f);
	void effector();
	void outputAppender( const CyberComposition::Simulink::State& subState_cb0, const SFC::CompoundStatement& parStmnt_cb2);

private:
	Packets_t* _subState_c92;
	Packets_t* _parStmnt_c93;
	Packets_t _state_c94;
	Packets_t _parStmnt_c9d;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_cad;
		SFC::CompoundStatement parStmnt_cae;
		CyberComposition::Simulink::State subState_caf;
	};

	std::list< Match> _matches;
};

class ClearTables_cbd
{
public:
	void operator()( const Packets_t& states_cbe, const Packets_t& programs_cc1, Packets_t& states_cc0, Packets_t& programs_cc3);

protected:
	bool isInputUnique( const Udm::Object& state_cca, const Udm::Object& program_cd3);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& states_cbe, const Packets_t& programs_cc1);
	bool patternMatcher( const Udm::Object& state_cc8, const Udm::Object& program_cd1);
	void effector();
	void forwardInputs();

private:
	Packets_t* _state_cc4;
	Packets_t* _program_cc5;
	Packets_t _state_cc6;
	Packets_t _program_ccf;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_cdc;
		SFC::Program program_cdd;
	};

	std::list< Match> _matches;
};

class CreateStateLabels_cde
{
public:
	void operator()( const Packets_t& states_cdf, const Packets_t& programs_ce1, Packets_t& states_ce3, Packets_t& programs_ce4);

protected:
	void callCreateCompoundStates_eed( const Packets_t& states_ce8, const Packets_t& programs_cea);
	void callCreateStateVar_ef0( const Packets_t& states_dcc, const Packets_t& programs_dcf);
	void callCreateLeafStates_ef3( const Packets_t& states_dee, const Packets_t& programs_df0);
	void callCreateFuncStates_psuedo_ef6( const Packets_t& states_e6e, const Packets_t& programs_e70);

private:
	Packets_t* _state_ce5;
	Packets_t* _program_ce6;
};

class CreateCompoundStates_ce7
{
public:
	void operator()( const Packets_t& states_ce8, const Packets_t& programs_cea, Packets_t& states_cec, Packets_t& programs_ced);

protected:
	void callIsANDORGROUPCompound_dc2( const Packets_t& states_d80, const Packets_t& programs_d82);
	void callCreateStateLabel_dc5( const Packets_t& states_cf1, const Packets_t& programs_cf3);
	void callGetSubStates_dc8( const Packets_t& states_c8c, const Packets_t& parStmnts_c8f);

private:
	Packets_t* _state_cee;
	Packets_t* _program_cef;
};

class CreateStateLabel_cf0
{
public:
	void operator()( const Packets_t& states_cf1, const Packets_t& programs_cf3, Packets_t& states_cf5, Packets_t& programs_cf6);

protected:
	void callCreate_d76( const Packets_t& states_d22, const Packets_t& programs_d25);
	void callInitMask_d79( const Packets_t& states_d48, const Packets_t& stateLabels_d4b);
	void callSetPath_d7c( const Packets_t& states_cfa, const Packets_t& stateLabels_cfd);

private:
	Packets_t* _state_cf7;
	Packets_t* _program_cf8;
};

class SetPath_cf9
{
public:
	void operator()( const Packets_t& states_cfa, const Packets_t& stateLabels_cfd, Packets_t& superStates_cfc, Packets_t& stateLabels_cff);

protected:
	bool isInputUnique( const Udm::Object& state_d06, const Udm::Object& stateLabel_d0f);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& states_cfa, const Packets_t& stateLabels_cfd);
	bool patternMatcher( const Udm::Object& state_d04, const Udm::Object& stateLabel_d0d);
	void effector();
	void outputAppender( const CyberComposition::Simulink::State& superState_d1d, const SFC::StateLabel& stateLabel_d1f);

private:
	Packets_t* _superState_d00;
	Packets_t* _stateLabel_d01;
	Packets_t _state_d02;
	Packets_t _stateLabel_d0b;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_d1a;
		SFC::StateLabel stateLabel_d1b;
		CyberComposition::Simulink::State superState_d1c;
	};

	std::list< Match> _matches;
};

class Create_d21
{
public:
	void operator()( const Packets_t& states_d22, const Packets_t& programs_d25, Packets_t& states_d24, Packets_t& stateLabels_d27);

protected:
	bool isInputUnique( const Udm::Object& state_d2e, const Udm::Object& program_d37);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& states_d22, const Packets_t& programs_d25);
	bool patternMatcher( const Udm::Object& state_d2c, const Udm::Object& program_d35);
	void effector();
	void outputAppender( const CyberComposition::Simulink::State& state_d43, const SFC::StateLabel& stateLabel_d45);

private:
	Packets_t* _state_d28;
	Packets_t* _stateLabel_d29;
	Packets_t _state_d2a;
	Packets_t _program_d33;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_d40;
		SFC::Program program_d41;
	};

	std::list< Match> _matches;
};

class InitMask_d47
{
public:
	void operator()( const Packets_t& states_d48, const Packets_t& stateLabels_d4b, Packets_t& states_d4a, Packets_t& stateLabels_d4d);

protected:
	bool isInputUnique( const Udm::Object& state_d54, const Udm::Object& stateLabel_d5d);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( CyberComposition::Simulink::State& State, SFC::StateLabel& StateLabel, CyberComposition::Simulink::State& SuperState, SFC::StateLabel& SuperStateLabel);
	void processInputPackets( const Packets_t& states_d48, const Packets_t& stateLabels_d4b);
	bool patternMatcher( const Udm::Object& state_d52, const Udm::Object& stateLabel_d5b);
	void effector();
	void forwardInputs();

private:
	Packets_t* _state_d4e;
	Packets_t* _stateLabel_d4f;
	Packets_t _state_d50;
	Packets_t _stateLabel_d59;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_d6e;
		SFC::StateLabel stateLabel_d6f;
		CyberComposition::Simulink::State superState_d70;
		SFC::StateLabel superStateLabel_d71;
	};

	std::list< Match> _matches;
};

class IsANDORGROUPCompound_d7f
{
public:
	void operator()( const Packets_t& states_d80, const Packets_t& programs_d82, Packets_t& states_d84, Packets_t& programs_d85);

protected:
	void executeOne( const Packets_t& states_d80, const Packets_t& programs_d82);
	bool isInputUnique( const Udm::Object& state_d8a, const Udm::Object& program_d91);

private:
	Packets_t* _state_d86;
	Packets_t* _program_d87;
	Packets_t _state_d88;
	Packets_t _program_d8f;
};

class HasSubstates_d96
{
public:
	bool operator()( const Packets_t& states_d97, const Packets_t& programs_d9a, Packets_t& states_d99, Packets_t& programs_d9c);

protected:
	bool isInputUnique( const Udm::Object& state_da3, const Udm::Object& program_dac);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::Program& Program, CyberComposition::Simulink::State& State, CyberComposition::Simulink::State& SubState);
	void processInputPackets( const Packets_t& states_d97, const Packets_t& programs_d9a);
	bool patternMatcher( const Udm::Object& state_da1, const Udm::Object& program_daa);
	void outputAppender( const CyberComposition::Simulink::State& state_dbe, const SFC::Program& program_dc0);

private:
	Packets_t* _state_d9d;
	Packets_t* _program_d9e;
	Packets_t _state_d9f;
	Packets_t _program_da8;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_db8;
		SFC::Program program_db9;
		CyberComposition::Simulink::State subState_dba;
	};

	std::list< Match> _matches;
};

class CreateStateVar_dcb
{
public:
	void operator()( const Packets_t& states_dcc, const Packets_t& programs_dcf, Packets_t& states_dce, Packets_t& programs_dd1);

protected:
	bool isInputUnique( const Udm::Object& state_dd8, const Udm::Object& program_de1);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& states_dcc, const Packets_t& programs_dcf);
	bool patternMatcher( const Udm::Object& state_dd6, const Udm::Object& program_ddf);
	void effector();
	void forwardInputs();

private:
	Packets_t* _state_dd2;
	Packets_t* _program_dd3;
	Packets_t _state_dd4;
	Packets_t _program_ddd;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_dea;
		SFC::Program program_deb;
	};

	std::list< Match> _matches;
};

class CreateLeafStates_ded
{
public:
	void operator()( const Packets_t& states_dee, const Packets_t& programs_df0, Packets_t& states_df2, Packets_t& programs_df3);

protected:
	void callIsLeaf_e64( const Packets_t& states_df7, const Packets_t& programs_df9);
	void callCreateStateLabel_e67( const Packets_t& states_cf1, const Packets_t& programs_cf3);
	void callGetSubStates_e6a( const Packets_t& states_c8c, const Packets_t& parStmnts_c8f);

private:
	Packets_t* _state_df4;
	Packets_t* _program_df5;
};

class IsLeaf_df6
{
public:
	void operator()( const Packets_t& states_df7, const Packets_t& programs_df9, Packets_t& states_dfb, Packets_t& programs_dfc, Packets_t& states_dfd, Packets_t& programs_dfe);

protected:
	void executeOne( const Packets_t& states_df7, const Packets_t& programs_df9);
	bool isInputUnique( const Udm::Object& state_e05, const Udm::Object& program_e0c);

private:
	Packets_t* _state_dff;
	Packets_t* _program_e00;
	Packets_t* _state_e01;
	Packets_t* _program_e02;
	Packets_t _state_e03;
	Packets_t _program_e0a;
};

class HasSubstates_e11
{
public:
	bool operator()( const Packets_t& states_e12, const Packets_t& programs_e15, Packets_t& states_e14, Packets_t& programs_e17);

protected:
	bool isInputUnique( const Udm::Object& state_e1e, const Udm::Object& program_e27);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::Program& Program, CyberComposition::Simulink::State& State, CyberComposition::Simulink::State& SubState);
	void processInputPackets( const Packets_t& states_e12, const Packets_t& programs_e15);
	bool patternMatcher( const Udm::Object& state_e1c, const Udm::Object& program_e25);
	void outputAppender( const CyberComposition::Simulink::State& state_e39, const SFC::Program& program_e3b);

private:
	Packets_t* _state_e18;
	Packets_t* _program_e19;
	Packets_t _state_e1a;
	Packets_t _program_e23;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_e33;
		SFC::Program program_e34;
		CyberComposition::Simulink::State subState_e35;
	};

	std::list< Match> _matches;
};

class Otherwise_e3d
{
public:
	bool operator()( const Packets_t& states_e3e, const Packets_t& programs_e41, Packets_t& states_e40, Packets_t& programs_e43);

protected:
	bool isInputUnique( const Udm::Object& state_e4a, const Udm::Object& program_e53);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::Program& Program, CyberComposition::Simulink::State& State);
	void processInputPackets( const Packets_t& states_e3e, const Packets_t& programs_e41);
	bool patternMatcher( const Udm::Object& state_e48, const Udm::Object& program_e51);
	void outputAppender( const CyberComposition::Simulink::State& state_e60, const SFC::Program& program_e62);

private:
	Packets_t* _state_e44;
	Packets_t* _program_e45;
	Packets_t _state_e46;
	Packets_t _program_e4f;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_e5c;
		SFC::Program program_e5d;
	};

	std::list< Match> _matches;
};

class CreateFuncStates_psuedo_e6d
{
public:
	void operator()( const Packets_t& states_e6e, const Packets_t& programs_e70, Packets_t& states_e72, Packets_t& programs_e73);

protected:
	void callIsFuncState_ee4( const Packets_t& states_e77, const Packets_t& programs_e79);
	void callGetSubStates_ee7( const Packets_t& states_c8c, const Packets_t& parStmnts_c8f);
	void callCreateStateLabel_eea( const Packets_t& states_cf1, const Packets_t& programs_cf3);

private:
	Packets_t* _state_e74;
	Packets_t* _program_e75;
};

class IsFuncState_e76
{
public:
	void operator()( const Packets_t& states_e77, const Packets_t& programs_e79, Packets_t& states_e7b, Packets_t& programs_e7c, Packets_t& states_e7d, Packets_t& programs_e7e);

protected:
	void executeOne( const Packets_t& states_e77, const Packets_t& programs_e79);
	bool isInputUnique( const Udm::Object& state_e85, const Udm::Object& program_e8c);

private:
	Packets_t* _state_e7f;
	Packets_t* _program_e80;
	Packets_t* _state_e81;
	Packets_t* _program_e82;
	Packets_t _state_e83;
	Packets_t _program_e8a;
};

class HasSubstates_e91
{
public:
	bool operator()( const Packets_t& states_e92, const Packets_t& programs_e95, Packets_t& states_e94, Packets_t& programs_e97);

protected:
	bool isInputUnique( const Udm::Object& state_e9e, const Udm::Object& program_ea7);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::Program& Program, CyberComposition::Simulink::State& State, CyberComposition::Simulink::State& SubState);
	void processInputPackets( const Packets_t& states_e92, const Packets_t& programs_e95);
	bool patternMatcher( const Udm::Object& state_e9c, const Udm::Object& program_ea5);
	void outputAppender( const CyberComposition::Simulink::State& state_eb9, const SFC::Program& program_ebb);

private:
	Packets_t* _state_e98;
	Packets_t* _program_e99;
	Packets_t _state_e9a;
	Packets_t _program_ea3;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_eb3;
		SFC::Program program_eb4;
		CyberComposition::Simulink::State subState_eb5;
	};

	std::list< Match> _matches;
};

class Otherwise_ebd
{
public:
	bool operator()( const Packets_t& states_ebe, const Packets_t& programs_ec1, Packets_t& states_ec0, Packets_t& programs_ec3);

protected:
	bool isInputUnique( const Udm::Object& state_eca, const Udm::Object& program_ed3);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::Program& Program, CyberComposition::Simulink::State& State);
	void processInputPackets( const Packets_t& states_ebe, const Packets_t& programs_ec1);
	bool patternMatcher( const Udm::Object& state_ec8, const Udm::Object& program_ed1);
	void outputAppender( const CyberComposition::Simulink::State& state_ee0, const SFC::Program& program_ee2);

private:
	Packets_t* _state_ec4;
	Packets_t* _program_ec5;
	Packets_t _state_ec6;
	Packets_t _program_ecf;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_edc;
		SFC::Program program_edd;
	};

	std::list< Match> _matches;
};

class CreateFuncStateFunctions_ef9
{
public:
	void operator()( const Packets_t& states_efa, const Packets_t& programs_efc, Packets_t& states_efe, Packets_t& programs_eff);

protected:
	void callPopulateGRFSFunction_104e( const Packets_t& states_1011, const Packets_t& programs_1013);
	void callPopulateEMFSFunction_1051( const Packets_t& states_f03, const Packets_t& programs_f05);
	void callGetSubStates_1054( const Packets_t& states_c8c, const Packets_t& parStmnts_c8f);

private:
	Packets_t* _state_f00;
	Packets_t* _program_f01;
};

class PopulateEMFSFunction_f02
{
public:
	void operator()( const Packets_t& states_f03, const Packets_t& programs_f05, Packets_t& states_f07, Packets_t& programs_f08);

protected:
	void callCreateEMFuncStateFunction_100a( const Packets_t& states_f0c, const Packets_t& programs_f0f);
	void callFuncArgsAndVars_100d( const Packets_t& states_f39, const Packets_t& fcns_f3b);

private:
	Packets_t* _state_f09;
	Packets_t* _program_f0a;
};

class CreateEMFuncStateFunction_f0b
{
public:
	void operator()( const Packets_t& states_f0c, const Packets_t& programs_f0f, Packets_t& states_f0e, Packets_t& execs_f11);

protected:
	bool isInputUnique( const Udm::Object& state_f18, const Udm::Object& program_f21);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::Program& Program, CyberComposition::Simulink::State& State, SFC::StateLabel& StateLabel);
	void processInputPackets( const Packets_t& states_f0c, const Packets_t& programs_f0f);
	bool patternMatcher( const Udm::Object& state_f16, const Udm::Object& program_f1f);
	void effector();
	void outputAppender( const CyberComposition::Simulink::State& state_f34, const SFC::Function& exec_f36);

private:
	Packets_t* _state_f12;
	Packets_t* _exec_f13;
	Packets_t _state_f14;
	Packets_t _program_f1d;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_f2d;
		SFC::Program program_f2e;
		SFC::StateLabel stateLabel_f2f;
	};

	std::list< Match> _matches;
};

class FuncArgsAndVars_f38
{
public:
	void operator()( const Packets_t& states_f39, const Packets_t& fcns_f3b, Packets_t& states_f3d, Packets_t& fcns_f3e);

protected:
	void callFuncInputArgs_1001( const Packets_t& states_f42, const Packets_t& fcns_f44);
	void callFuncOutputArgs_1004( const Packets_t& states_f80, const Packets_t& fcns_f82);
	void callFuncLocalVars_1007( const Packets_t& states_fc1, const Packets_t& fcns_fc3);

private:
	Packets_t* _state_f3f;
	Packets_t* _fcn_f40;
};

class FuncInputArgs_f41
{
public:
	void operator()( const Packets_t& states_f42, const Packets_t& fcns_f44, Packets_t& states_f46, Packets_t& fcns_f47);

protected:
	void callInputArgs_f7c( const Packets_t& states_f4b, const Packets_t& execs_f4d);

private:
	Packets_t* _state_f48;
	Packets_t* _fcn_f49;
};

class InputArgs_f4a
{
public:
	void operator()( const Packets_t& states_f4b, const Packets_t& execs_f4d);

protected:
	bool isInputUnique( const Udm::Object& state_f53, const Udm::Object& exec_f5c);
	bool isGuardTrue( SFC::DT& DT, CyberComposition::Simulink::Data& Data, SFC::Function& Exec, CyberComposition::Simulink::State& State, SFC::StateLabel& StateLabel, CyberComposition::Simulink::SF_TypeBase& TypeBase, CyberComposition::Simulink::TypeBaseRef& TypeBaseRef);
	void processInputPackets( const Packets_t& states_f4b, const Packets_t& execs_f4d);
	bool patternMatcher( const Udm::Object& state_f51, const Udm::Object& exec_f5a);
	void effector();

private:
	Packets_t _state_f4f;
	Packets_t _exec_f58;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_f6d;
		SFC::Function exec_f6e;
		SFC::StateLabel stateLabel_f6f;
		CyberComposition::Simulink::Data data_f70;
		CyberComposition::Simulink::TypeBaseRef typeBaseRef_f71;
		CyberComposition::Simulink::SF_TypeBase typeBase_f72;
		SFC::DT dT_f73;
	};

	std::list< Match> _matches;
};

class FuncOutputArgs_f7f
{
public:
	void operator()( const Packets_t& states_f80, const Packets_t& fcns_f82, Packets_t& states_f84, Packets_t& fcns_f85);

protected:
	void callOutputArgs_fbd( const Packets_t& states_f89, const Packets_t& execs_f8b);

private:
	Packets_t* _state_f86;
	Packets_t* _fcn_f87;
};

class OutputArgs_f88
{
public:
	void operator()( const Packets_t& states_f89, const Packets_t& execs_f8b);

protected:
	bool isInputUnique( const Udm::Object& state_f91, const Udm::Object& exec_f9a);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::DT& DT, CyberComposition::Simulink::Data& Data, SFC::Function& Exec, CyberComposition::Simulink::State& State, SFC::StateLabel& StateLabel, CyberComposition::Simulink::SF_TypeBase& TypeBase, CyberComposition::Simulink::TypeBaseRef& TypeBaseRef);
	void processInputPackets( const Packets_t& states_f89, const Packets_t& execs_f8b);
	bool patternMatcher( const Udm::Object& state_f8f, const Udm::Object& exec_f98);
	void effector();

private:
	Packets_t _state_f8d;
	Packets_t _exec_f96;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_fae;
		SFC::Function exec_faf;
		SFC::StateLabel stateLabel_fb0;
		CyberComposition::Simulink::Data data_fb1;
		CyberComposition::Simulink::TypeBaseRef typeBaseRef_fb2;
		CyberComposition::Simulink::SF_TypeBase typeBase_fb3;
		SFC::DT dT_fb4;
	};

	std::list< Match> _matches;
};

class FuncLocalVars_fc0
{
public:
	void operator()( const Packets_t& states_fc1, const Packets_t& fcns_fc3, Packets_t& states_fc5, Packets_t& fcns_fc6);

protected:
	void callFuncLocalVars_ffe( const Packets_t& states_fca, const Packets_t& execs_fcc);

private:
	Packets_t* _state_fc7;
	Packets_t* _fcn_fc8;
};

class FuncLocalVars_fc9
{
public:
	void operator()( const Packets_t& states_fca, const Packets_t& execs_fcc);

protected:
	bool isInputUnique( const Udm::Object& state_fd2, const Udm::Object& exec_fdb);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::DT& DT, CyberComposition::Simulink::Data& Data, SFC::Function& Exec, CyberComposition::Simulink::State& State, SFC::StateLabel& StateLabel, CyberComposition::Simulink::SF_TypeBase& TypeBase, CyberComposition::Simulink::TypeBaseRef& TypeBaseRef);
	void processInputPackets( const Packets_t& states_fca, const Packets_t& execs_fcc);
	bool patternMatcher( const Udm::Object& state_fd0, const Udm::Object& exec_fd9);
	void effector();

private:
	Packets_t _state_fce;
	Packets_t _exec_fd7;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_fef;
		SFC::Function exec_ff0;
		SFC::StateLabel stateLabel_ff1;
		CyberComposition::Simulink::Data data_ff2;
		CyberComposition::Simulink::TypeBaseRef typeBaseRef_ff3;
		CyberComposition::Simulink::SF_TypeBase typeBase_ff4;
		SFC::DT dT_ff5;
	};

	std::list< Match> _matches;
};

class PopulateGRFSFunction_1010
{
public:
	void operator()( const Packets_t& states_1011, const Packets_t& programs_1013, Packets_t& states_1015, Packets_t& programs_1016);

protected:
	void callCreateGRFuncStateFunction_1048( const Packets_t& states_101a, const Packets_t& programs_101d);
	void callFuncArgsAndVars_104b( const Packets_t& states_f39, const Packets_t& fcns_f3b);

private:
	Packets_t* _state_1017;
	Packets_t* _program_1018;
};

class CreateGRFuncStateFunction_1019
{
public:
	void operator()( const Packets_t& states_101a, const Packets_t& programs_101d, Packets_t& states_101c, Packets_t& execs_101f);

protected:
	bool isInputUnique( const Udm::Object& state_1026, const Udm::Object& program_102f);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::Program& Program, CyberComposition::Simulink::State& State, SFC::StateLabel& StateLabel);
	void processInputPackets( const Packets_t& states_101a, const Packets_t& programs_101d);
	bool patternMatcher( const Udm::Object& state_1024, const Udm::Object& program_102d);
	void effector();
	void outputAppender( const CyberComposition::Simulink::State& state_1044, const SFC::Function& exec_1046);

private:
	Packets_t* _state_1020;
	Packets_t* _exec_1021;
	Packets_t _state_1022;
	Packets_t _program_102b;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_103b;
		SFC::Program program_103c;
		SFC::StateLabel stateLabel_103d;
	};

	std::list< Match> _matches;
};

class PopulateFunctions_1057
{
public:
	void operator()( const Packets_t& states_1058, const Packets_t& programs_105a, Packets_t& states_105c, Packets_t& programs_105d);

protected:
	void callNoFunctionStates_1f41( const Packets_t& states_1309, const Packets_t& programs_130b);
	void callPopulateEnterFunction_1f44( const Packets_t& states_1c16, const Packets_t& programs_1c18);
	void callPopulateExecFunction_1f47( const Packets_t& states_1347, const Packets_t& programs_1349);
	void callPopulateExitFunction_1f4a( const Packets_t& states_19ef, const Packets_t& programs_19f1);
	void callPopulateStatusFunction_1f4d( const Packets_t& states_1061, const Packets_t& programs_1063);
	void callGetSubStates_1f50( const Packets_t& states_c8c, const Packets_t& parStmnts_c8f);

private:
	Packets_t* _state_105e;
	Packets_t* _program_105f;
};

class PopulateStatusFunction_1060
{
public:
	void operator()( const Packets_t& states_1061, const Packets_t& programs_1063, Packets_t& states_1065, Packets_t& programs_1066);

protected:
	void callGetStatusFunction_12ea( const Packets_t& states_12c3);
	void callHasChildren_12ec( const Packets_t& states_1089, const Packets_t& statuss_108b);
	void callElimState_12ef( const Packets_t& states_106a, const Packets_t& statuss_106c);
	void callnewIndent_12f2( const Packets_t& states_10f5, const Packets_t& statuss_10f8);
	void callReturnValue_12f5( const Packets_t& statuss_1130);
	void callDecompTest_12f7( const Packets_t& states_11ae, const Packets_t& statuss_11b0, const Packets_t& svs_11b2);
	void callGetChildStates_12fb( const Packets_t& states_1250, const Packets_t& compoundStatements_1254, const Packets_t& stateVars_1257);
	void callCreateConditionalGroup_12ff( const Packets_t& states_128d, const Packets_t& statuss_1290, const Packets_t& stateVars_1293);
	void callCreateConditionals_1303( const Packets_t& states_1149, const Packets_t& childStates_114c, const Packets_t& compoundStatements_114e, const Packets_t& stateVars_1150);

private:
	Packets_t* _state_1067;
	Packets_t* _program_1068;
};

class ElimState_1069
{
public:
	void operator()( const Packets_t& states_106a, const Packets_t& statuss_106c, Packets_t& statuss_106e);

protected:
	bool isInputUnique( const Udm::Object& state_1074, const Udm::Object& status_107d);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& states_106a, const Packets_t& statuss_106c);
	bool patternMatcher( const Udm::Object& state_1072, const Udm::Object& status_107b);
	void effector();
	void forwardInputs();

private:
	Packets_t* _status_106f;
	Packets_t _state_1070;
	Packets_t _status_1079;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_1086;
		SFC::Function status_1087;
	};

	std::list< Match> _matches;
};

class HasChildren_1088
{
public:
	void operator()( const Packets_t& states_1089, const Packets_t& statuss_108b, Packets_t& states_108d, Packets_t& statuss_108e, Packets_t& states_108f, Packets_t& statuss_1090);

protected:
	void executeOne( const Packets_t& states_1089, const Packets_t& statuss_108b);
	bool isInputUnique( const Udm::Object& state_1097, const Udm::Object& status_109e);

private:
	Packets_t* _state_1091;
	Packets_t* _status_1092;
	Packets_t* _state_1093;
	Packets_t* _status_1094;
	Packets_t _state_1095;
	Packets_t _status_109c;
};

class HasChildren_10a3
{
public:
	bool operator()( const Packets_t& states_10a4, const Packets_t& statuss_10a7, Packets_t& states_10a6, Packets_t& statuss_10a9);

protected:
	bool isInputUnique( const Udm::Object& state_10b0, const Udm::Object& status_10b9);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( CyberComposition::Simulink::State& ChildState, CyberComposition::Simulink::State& State, SFC::Function& Status);
	void processInputPackets( const Packets_t& states_10a4, const Packets_t& statuss_10a7);
	bool patternMatcher( const Udm::Object& state_10ae, const Udm::Object& status_10b7);
	void outputAppender( const CyberComposition::Simulink::State& state_10cb, const SFC::Function& status_10cd);

private:
	Packets_t* _state_10aa;
	Packets_t* _status_10ab;
	Packets_t _state_10ac;
	Packets_t _status_10b5;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_10c5;
		SFC::Function status_10c6;
		CyberComposition::Simulink::State childState_10c7;
	};

	std::list< Match> _matches;
};

class NoChildren_10cf
{
public:
	bool operator()( const Packets_t& states_10d0, const Packets_t& statuss_10d3, Packets_t& states_10d2, Packets_t& statuss_10d5);

protected:
	bool isInputUnique( const Udm::Object& state_10dc, const Udm::Object& status_10e5);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& states_10d0, const Packets_t& statuss_10d3);
	bool patternMatcher( const Udm::Object& state_10da, const Udm::Object& status_10e3);
	void outputAppender( const CyberComposition::Simulink::State& state_10f0, const SFC::Function& status_10f2);

private:
	Packets_t* _state_10d6;
	Packets_t* _status_10d7;
	Packets_t _state_10d8;
	Packets_t _status_10e1;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_10ee;
		SFC::Function status_10ef;
	};

	std::list< Match> _matches;
};

class NewIndent_10f4
{
public:
	void operator()( const Packets_t& states_10f5, const Packets_t& statuss_10f8, Packets_t& states_10f7, Packets_t& statuss_10fa, Packets_t& stateVars_10fb);

protected:
	bool isInputUnique( const Udm::Object& state_1103, const Udm::Object& status_110c);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& states_10f5, const Packets_t& statuss_10f8);
	bool patternMatcher( const Udm::Object& state_1101, const Udm::Object& status_110a);
	void effector();
	void outputAppender( const CyberComposition::Simulink::State& state_1129, const SFC::Function& status_112b, const SFC::StateVar& stateVar_112d);

private:
	Packets_t* _state_10fc;
	Packets_t* _status_10fd;
	Packets_t* _stateVar_10fe;
	Packets_t _state_10ff;
	Packets_t _status_1108;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_111d;
		SFC::Function status_111e;
		SFC::Arg arg_111f;
		SFC::Program program_1120;
		SFC::StateVar stateVar_1121;
	};

	std::list< Match> _matches;
};

class ReturnValue_112f
{
public:
	void operator()( const Packets_t& statuss_1130);

protected:
	bool isInputUnique( const Udm::Object& status_1136);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::Function& Status, SFC::LocalVar& retval);
	void processInputPackets( const Packets_t& statuss_1130);
	bool patternMatcher( const Udm::Object& status_1134);
	void effector();

private:
	Packets_t _status_1132;
	class Match
	{
	public:
		SFC::Function status_1142;
		SFC::LocalVar retval_1143;
	};

	std::list< Match> _matches;
};

class CreateConditionals_1148
{
public:
	void operator()( const Packets_t& states_1149, const Packets_t& childStates_114c, const Packets_t& compoundStatements_114e, const Packets_t& stateVars_1150, Packets_t& statuss_114b);

protected:
	bool isInputUnique( const Udm::Object& state_1157, const Udm::Object& childState_1160, const Udm::Object& compoundStatement_1169, const Udm::Object& stateVar_1172);
	bool isGuardTrue( CyberComposition::Simulink::State& ChildState, SFC::StateLabel& ChildStateLabel, SFC::Function& ChildStatus, SFC::CompoundStatement& CompoundStatement, SFC::Arg& IndentArg, CyberComposition::Simulink::State& State, SFC::StateLabel& StateLabel, SFC::StateVar& StateVar, SFC::Function& Status, SFC::LocalVar& newIndent, SFC::LocalVar& retval);
	void processInputPackets( const Packets_t& states_1149, const Packets_t& childStates_114c, const Packets_t& compoundStatements_114e, const Packets_t& stateVars_1150);
	bool patternMatcher( const Udm::Object& state_1155, const Udm::Object& childState_115e, const Udm::Object& compoundStatement_1167, const Udm::Object& stateVar_1170);
	void effector();
	void outputAppender( const SFC::Function& status_11ab);

private:
	Packets_t* _status_1152;
	Packets_t _state_1153;
	Packets_t _childState_115c;
	Packets_t _compoundStatement_1165;
	Packets_t _stateVar_116e;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_118d;
		CyberComposition::Simulink::State childState_118e;
		SFC::CompoundStatement compoundStatement_118f;
		SFC::StateVar stateVar_1190;
		SFC::Function status_1191;
		SFC::LocalVar retval_1192;
		SFC::Arg indentArg_1193;
		SFC::Function childStatus_1194;
		SFC::LocalVar newIndent_1195;
		SFC::StateLabel stateLabel_1196;
		SFC::StateLabel childStateLabel_1197;
	};

	std::list< Match> _matches;
};

class DecompTest_11ad
{
public:
	void operator()( const Packets_t& states_11ae, const Packets_t& statuss_11b0, const Packets_t& svs_11b2, Packets_t& states_11b4, Packets_t& statuss_11b5, Packets_t& svs_11b6, Packets_t& states_11b7, Packets_t& statuss_11b8, Packets_t& svs_11b9);

protected:
	void executeOne( const Packets_t& states_11ae, const Packets_t& statuss_11b0, const Packets_t& svs_11b2);
	bool isInputUnique( const Udm::Object& state_11c2, const Udm::Object& status_11c9, const Udm::Object& sv_11d0);

private:
	Packets_t* _state_11ba;
	Packets_t* _status_11bb;
	Packets_t* _sv_11bc;
	Packets_t* _state_11bd;
	Packets_t* _status_11be;
	Packets_t* _sv_11bf;
	Packets_t _state_11c0;
	Packets_t _status_11c7;
	Packets_t _sv_11ce;
};

class AndState_11d5
{
public:
	bool operator()( const Packets_t& states_11d6, const Packets_t& statuss_11d9, const Packets_t& stateVars_11dc, Packets_t& states_11d8, Packets_t& statuss_11db, Packets_t& stateVars_11de);

protected:
	bool isInputUnique( const Udm::Object& state_11e6, const Udm::Object& status_11ef, const Udm::Object& stateVar_11f8);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( CyberComposition::Simulink::State& ChildState, CyberComposition::Simulink::State& State, SFC::StateVar& StateVar, SFC::Function& Status);
	void processInputPackets( const Packets_t& states_11d6, const Packets_t& statuss_11d9, const Packets_t& stateVars_11dc);
	bool patternMatcher( const Udm::Object& state_11e4, const Udm::Object& status_11ed, const Udm::Object& stateVar_11f6);
	void outputAppender( const CyberComposition::Simulink::State& state_120c, const SFC::Function& status_120e, const SFC::StateVar& stateVar_1210);

private:
	Packets_t* _state_11df;
	Packets_t* _status_11e0;
	Packets_t* _stateVar_11e1;
	Packets_t _state_11e2;
	Packets_t _status_11eb;
	Packets_t _stateVar_11f4;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_1204;
		SFC::Function status_1205;
		SFC::StateVar stateVar_1206;
		CyberComposition::Simulink::State childState_1207;
	};

	std::list< Match> _matches;
};

class OrState_1212
{
public:
	bool operator()( const Packets_t& states_1213, const Packets_t& statuss_1216, const Packets_t& stateVars_1219, Packets_t& states_1215, Packets_t& statuss_1218, Packets_t& stateVars_121b);

protected:
	bool isInputUnique( const Udm::Object& state_1223, const Udm::Object& status_122c, const Udm::Object& stateVar_1235);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( CyberComposition::Simulink::State& ChildState, CyberComposition::Simulink::State& State, SFC::StateVar& StateVar, SFC::Function& Status);
	void processInputPackets( const Packets_t& states_1213, const Packets_t& statuss_1216, const Packets_t& stateVars_1219);
	bool patternMatcher( const Udm::Object& state_1221, const Udm::Object& status_122a, const Udm::Object& stateVar_1233);
	void outputAppender( const CyberComposition::Simulink::State& state_1249, const SFC::Function& status_124b, const SFC::StateVar& stateVar_124d);

private:
	Packets_t* _state_121c;
	Packets_t* _status_121d;
	Packets_t* _stateVar_121e;
	Packets_t _state_121f;
	Packets_t _status_1228;
	Packets_t _stateVar_1231;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_1241;
		SFC::Function status_1242;
		SFC::StateVar stateVar_1243;
		CyberComposition::Simulink::State childState_1244;
	};

	std::list< Match> _matches;
};

class GetChildStates_124f
{
public:
	void operator()( const Packets_t& states_1250, const Packets_t& compoundStatements_1254, const Packets_t& stateVars_1257, Packets_t& states_1252, Packets_t& childStates_1253, Packets_t& compoundStatements_1256, Packets_t& stateVars_1259);

protected:
	bool isInputUnique( const Udm::Object& state_1262, const Udm::Object& compoundStatement_126b, const Udm::Object& stateVar_1274);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& states_1250, const Packets_t& compoundStatements_1254, const Packets_t& stateVars_1257);
	bool patternMatcher( const Udm::Object& state_1260, const Udm::Object& compoundStatement_1269, const Udm::Object& stateVar_1272);
	void effector();
	void outputAppender( const CyberComposition::Simulink::State& state_1284, const CyberComposition::Simulink::State& childState_1286, const SFC::CompoundStatement& compoundStatement_1288, const SFC::StateVar& stateVar_128a);
	void sortOutputs();

private:
	Packets_t* _state_125a;
	Packets_t* _childState_125b;
	Packets_t* _compoundStatement_125c;
	Packets_t* _stateVar_125d;
	Packets_t _state_125e;
	Packets_t _compoundStatement_1267;
	Packets_t _stateVar_1270;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_1280;
		SFC::CompoundStatement compoundStatement_1281;
		SFC::StateVar stateVar_1282;
		CyberComposition::Simulink::State childState_1283;
	};

	std::list< Match> _matches;
};

class CreateConditionalGroup_128c
{
public:
	void operator()( const Packets_t& states_128d, const Packets_t& statuss_1290, const Packets_t& stateVars_1293, Packets_t& states_128f, Packets_t& conditionalGroups_1292, Packets_t& stateVars_1295);

protected:
	bool isInputUnique( const Udm::Object& state_129d, const Udm::Object& status_12a6, const Udm::Object& stateVar_12af);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& states_128d, const Packets_t& statuss_1290, const Packets_t& stateVars_1293);
	bool patternMatcher( const Udm::Object& state_129b, const Udm::Object& status_12a4, const Udm::Object& stateVar_12ad);
	void effector();
	void outputAppender( const CyberComposition::Simulink::State& state_12bc, const SFC::ConditionalGroup& conditionalGroup_12be, const SFC::StateVar& stateVar_12c0);

private:
	Packets_t* _state_1296;
	Packets_t* _conditionalGroup_1297;
	Packets_t* _stateVar_1298;
	Packets_t _state_1299;
	Packets_t _status_12a2;
	Packets_t _stateVar_12ab;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_12b8;
		SFC::Function status_12b9;
		SFC::StateVar stateVar_12ba;
	};

	std::list< Match> _matches;
};

class GetStatusFunction_12c2
{
public:
	void operator()( const Packets_t& states_12c3, Packets_t& states_12c5, Packets_t& statuss_12c6);

protected:
	bool isInputUnique( const Udm::Object& state_12cd);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& states_12c3);
	bool patternMatcher( const Udm::Object& state_12cb);
	void effector();
	void outputAppender( const CyberComposition::Simulink::State& state_12e6, const SFC::Function& status_12e8);

private:
	Packets_t* _state_12c7;
	Packets_t* _status_12c8;
	Packets_t _state_12c9;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_12dc;
		SFC::Function status_12dd;
		SFC::Arg arg_12de;
	};

	std::list< Match> _matches;
};

class NoFunctionStates_1308
{
public:
	void operator()( const Packets_t& states_1309, const Packets_t& programs_130b, Packets_t& states_130d, Packets_t& programs_130e);

protected:
	void executeOne( const Packets_t& states_1309, const Packets_t& programs_130b);
	bool isInputUnique( const Udm::Object& state_1313, const Udm::Object& program_131a);

private:
	Packets_t* _state_130f;
	Packets_t* _program_1310;
	Packets_t _state_1311;
	Packets_t _program_1318;
};

class NotFunctionState_131f
{
public:
	bool operator()( const Packets_t& states_1320, const Packets_t& programs_1323, Packets_t& states_1322, Packets_t& programs_1325);

protected:
	bool isInputUnique( const Udm::Object& state_132c, const Udm::Object& program_1335);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::Program& Program, CyberComposition::Simulink::State& State);
	void processInputPackets( const Packets_t& states_1320, const Packets_t& programs_1323);
	bool patternMatcher( const Udm::Object& state_132a, const Udm::Object& program_1333);
	void outputAppender( const CyberComposition::Simulink::State& state_1342, const SFC::Program& program_1344);

private:
	Packets_t* _state_1326;
	Packets_t* _program_1327;
	Packets_t _state_1328;
	Packets_t _program_1331;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_133e;
		SFC::Program program_133f;
	};

	std::list< Match> _matches;
};

class PopulateExecFunction_1346
{
public:
	void operator()( const Packets_t& states_1347, const Packets_t& programs_1349, Packets_t& states_134b, Packets_t& programs_134c);

protected:
	void callInitTransConnMap_19e2( const Packets_t& states_19d9);
	void callOuterTransitions_19e4( const Packets_t& states_1963);
	void callDuringAction_19e6( const Packets_t& states_194c);
	void callInitTransConnMapInner_19e8( const Packets_t& states_18fe);
	void callInnerTransitions_19ea( const Packets_t& states_164c);
	void callExecChildren_19ec( const Packets_t& states_1350);

private:
	Packets_t* _state_134d;
	Packets_t* _program_134e;
};

class ExecChildren_134f
{
public:
	void operator()( const Packets_t& states_1350);

protected:
	void callGetExecFcn_1627( const Packets_t& states_160e);
	void callChildStateType_1629( const Packets_t& states_152f, const Packets_t& css_1531);
	void callTopLevelState_162c( const Packets_t& states_1499, const Packets_t& css_149b);
	void callCG_162f( const Packets_t& states_15e8, const Packets_t& compoundStatements_15eb);
	void callChildStatesExec_1632( const Packets_t& states_1380, const Packets_t& css_1382);
	void callCreateCG_1635( const Packets_t& states_1473, const Packets_t& execs_1476);
	void callCallActiveChildExec_1638( const Packets_t& states_159b, const Packets_t& css_159d);
	void callEnterFunctionCondition_163b( const Packets_t& states_13ea, const Packets_t& cgs_13ec);
	void callTransStartMap_163e( const Packets_t& states_a08, const Packets_t& css_a0a);
	void callExecFunctionCondition_1641( const Packets_t& states_1353, const Packets_t& cgs_1355);
	void callDefaultCondition_1644( const Packets_t& states_1502, const Packets_t& compoundStatements_1506);
	void callTransStartTransitions_1647( const Packets_t& states_b2, const Packets_t& fcns_b4, const Packets_t& css_b6);
};

class ExecFunctionCondition_1352
{
public:
	void operator()( const Packets_t& states_1353, const Packets_t& cgs_1355);

protected:
	void callCreateDefaultCB_13e3( const Packets_t& states_1358, const Packets_t& conditionalGroups_135b);
	void callChildStatesExec_13e6( const Packets_t& states_1380, const Packets_t& css_1382);
};

class CreateDefaultCB_1357
{
public:
	void operator()( const Packets_t& states_1358, const Packets_t& conditionalGroups_135b, Packets_t& states_135a, Packets_t& conditionalBlocks_135d);

protected:
	bool isInputUnique( const Udm::Object& state_1364, const Udm::Object& conditionalGroup_136d);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& states_1358, const Packets_t& conditionalGroups_135b);
	bool patternMatcher( const Udm::Object& state_1362, const Udm::Object& conditionalGroup_136b);
	void effector();
	void outputAppender( const CyberComposition::Simulink::State& state_137b, const SFC::ConditionalBlock& conditionalBlock_137d);

private:
	Packets_t* _state_135e;
	Packets_t* _conditionalBlock_135f;
	Packets_t _state_1360;
	Packets_t _conditionalGroup_1369;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_1376;
		SFC::ConditionalGroup conditionalGroup_1377;
	};

	std::list< Match> _matches;
};

class ChildStatesExec_137f
{
public:
	void operator()( const Packets_t& states_1380, const Packets_t& css_1382);

protected:
	void callGetChildStates_13dc( const Packets_t& states_13b0, const Packets_t& compoundStatements_13b4);
	void callExecChildAndStates_13df( const Packets_t& states_1385, const Packets_t& childStates_1387, const Packets_t& compoundStatements_1389);
};

class ExecChildAndStates_1384
{
public:
	void operator()( const Packets_t& states_1385, const Packets_t& childStates_1387, const Packets_t& compoundStatements_1389);

protected:
	bool isInputUnique( const Udm::Object& state_138f, const Udm::Object& childState_1398, const Udm::Object& compoundStatement_13a1);
	void processInputPackets( const Packets_t& states_1385, const Packets_t& childStates_1387, const Packets_t& compoundStatements_1389);
	bool patternMatcher( const Udm::Object& state_138d, const Udm::Object& childState_1396, const Udm::Object& compoundStatement_139f);
	void effector();

private:
	Packets_t _state_138b;
	Packets_t _childState_1394;
	Packets_t _compoundStatement_139d;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_13aa;
		CyberComposition::Simulink::State childState_13ab;
		SFC::CompoundStatement compoundStatement_13ac;
		SFC::Function function_13ad;
	};

	std::list< Match> _matches;
};

class GetChildStates_13af
{
public:
	void operator()( const Packets_t& states_13b0, const Packets_t& compoundStatements_13b4, Packets_t& states_13b2, Packets_t& childStates_13b3, Packets_t& compoundStatements_13b6);

protected:
	bool isInputUnique( const Udm::Object& state_13be, const Udm::Object& compoundStatement_13c7);
	bool isGuardTrue( CyberComposition::Simulink::State& ChildState, SFC::CompoundStatement& CompoundStatement, CyberComposition::Simulink::State& State);
	void processInputPackets( const Packets_t& states_13b0, const Packets_t& compoundStatements_13b4);
	bool patternMatcher( const Udm::Object& state_13bc, const Udm::Object& compoundStatement_13c5);
	void effector();
	void outputAppender( const CyberComposition::Simulink::State& state_13d6, const CyberComposition::Simulink::State& childState_13d8, const SFC::CompoundStatement& compoundStatement_13da);
	void sortOutputs();

private:
	Packets_t* _state_13b7;
	Packets_t* _childState_13b8;
	Packets_t* _compoundStatement_13b9;
	Packets_t _state_13ba;
	Packets_t _compoundStatement_13c3;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_13d0;
		SFC::CompoundStatement compoundStatement_13d1;
		CyberComposition::Simulink::State childState_13d2;
	};

	std::list< Match> _matches;
};

class EnterFunctionCondition_13e9
{
public:
	void operator()( const Packets_t& states_13ea, const Packets_t& cgs_13ec, Packets_t& states_13ee, Packets_t& cgs_13ef);

protected:
	void callCreateCB_1468( const Packets_t& states_1433, const Packets_t& conditionalGroups_1436);
	void callGetChildStates_146b( const Packets_t& states_13b0, const Packets_t& compoundStatements_13b4);
	void callCallChildren_146e( const Packets_t& states_13f3, const Packets_t& childStates_13f5, const Packets_t& compoundStatements_13f7);

private:
	Packets_t* _state_13f0;
	Packets_t* _cg_13f1;
};

class CallChildren_13f2
{
public:
	void operator()( const Packets_t& states_13f3, const Packets_t& childStates_13f5, const Packets_t& compoundStatements_13f7);

protected:
	bool isInputUnique( const Udm::Object& state_13fd, const Udm::Object& childState_1406, const Udm::Object& compoundStatement_140f);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::Arg& ChildArg0, SFC::Arg& ChildArg1, SFC::Function& ChildEnter, CyberComposition::Simulink::State& ChildState, SFC::CompoundStatement& CompoundStatement, CyberComposition::Simulink::State& State);
	void processInputPackets( const Packets_t& states_13f3, const Packets_t& childStates_13f5, const Packets_t& compoundStatements_13f7);
	bool patternMatcher( const Udm::Object& state_13fb, const Udm::Object& childState_1404, const Udm::Object& compoundStatement_140d);
	void effector();

private:
	Packets_t _state_13f9;
	Packets_t _childState_1402;
	Packets_t _compoundStatement_140b;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_1421;
		CyberComposition::Simulink::State childState_1422;
		SFC::CompoundStatement compoundStatement_1423;
		SFC::Arg childArg0_1424;
		SFC::Arg childArg1_1425;
		SFC::Function childEnter_1426;
	};

	std::list< Match> _matches;
};

class CreateCB_1432
{
public:
	void operator()( const Packets_t& states_1433, const Packets_t& conditionalGroups_1436, Packets_t& states_1435, Packets_t& conditionalBlocks_1438);

protected:
	bool isInputUnique( const Udm::Object& state_143f, const Udm::Object& conditionalGroup_1448);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& states_1433, const Packets_t& conditionalGroups_1436);
	bool patternMatcher( const Udm::Object& state_143d, const Udm::Object& conditionalGroup_1446);
	void effector();
	void outputAppender( const CyberComposition::Simulink::State& state_1464, const SFC::ConditionalBlock& conditionalBlock_1466);

private:
	Packets_t* _state_1439;
	Packets_t* _conditionalBlock_143a;
	Packets_t _state_143b;
	Packets_t _conditionalGroup_1444;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_1458;
		SFC::ConditionalGroup conditionalGroup_1459;
		SFC::Function function_145a;
		SFC::Program program_145b;
		SFC::StateVar stateVar_145c;
	};

	std::list< Match> _matches;
};

class CreateCG_1472
{
public:
	void operator()( const Packets_t& states_1473, const Packets_t& execs_1476, Packets_t& states_1475, Packets_t& conditionalGroups_1478);

protected:
	bool isInputUnique( const Udm::Object& state_147f, const Udm::Object& exec_1488);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& states_1473, const Packets_t& execs_1476);
	bool patternMatcher( const Udm::Object& state_147d, const Udm::Object& exec_1486);
	void effector();
	void outputAppender( const CyberComposition::Simulink::State& state_1494, const SFC::ConditionalGroup& conditionalGroup_1496);

private:
	Packets_t* _state_1479;
	Packets_t* _conditionalGroup_147a;
	Packets_t _state_147b;
	Packets_t _exec_1484;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_1491;
		SFC::Function exec_1492;
	};

	std::list< Match> _matches;
};

class TopLevelState_1498
{
public:
	void operator()( const Packets_t& states_1499, const Packets_t& css_149b, Packets_t& states_149d, Packets_t& css_149e, Packets_t& states_149f, Packets_t& css_14a0);

protected:
	void executeOne( const Packets_t& states_1499, const Packets_t& css_149b);
	bool isInputUnique( const Udm::Object& state_14a7, const Udm::Object& cs_14ae);

private:
	Packets_t* _state_14a1;
	Packets_t* _cs_14a2;
	Packets_t* _state_14a3;
	Packets_t* _cs_14a4;
	Packets_t _state_14a5;
	Packets_t _cs_14ac;
};

class IsTopLevelState_14b3
{
public:
	bool operator()( const Packets_t& states_14b4, const Packets_t& compoundStatements_14b7, Packets_t& states_14b6, Packets_t& compoundStatements_14b9);

protected:
	bool isInputUnique( const Udm::Object& state_14c0, const Udm::Object& compoundStatement_14c9);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& states_14b4, const Packets_t& compoundStatements_14b7);
	bool patternMatcher( const Udm::Object& state_14be, const Udm::Object& compoundStatement_14c7);
	void outputAppender( const CyberComposition::Simulink::State& state_14d8, const SFC::CompoundStatement& compoundStatement_14da);

private:
	Packets_t* _state_14ba;
	Packets_t* _compoundStatement_14bb;
	Packets_t _state_14bc;
	Packets_t _compoundStatement_14c5;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_14d5;
		SFC::CompoundStatement compoundStatement_14d6;
		CyberComposition::Simulink::ConnectorRef connectorRef_14d7;
	};

	std::list< Match> _matches;
};

class Otherwise_14dc
{
public:
	bool operator()( const Packets_t& states_14dd, const Packets_t& compoundStatements_14e0, Packets_t& states_14df, Packets_t& compoundStatements_14e2);

protected:
	bool isInputUnique( const Udm::Object& state_14e9, const Udm::Object& compoundStatement_14f2);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& states_14dd, const Packets_t& compoundStatements_14e0);
	bool patternMatcher( const Udm::Object& state_14e7, const Udm::Object& compoundStatement_14f0);
	void outputAppender( const CyberComposition::Simulink::State& state_14fd, const SFC::CompoundStatement& compoundStatement_14ff);

private:
	Packets_t* _state_14e3;
	Packets_t* _compoundStatement_14e4;
	Packets_t _state_14e5;
	Packets_t _compoundStatement_14ee;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_14fb;
		SFC::CompoundStatement compoundStatement_14fc;
	};

	std::list< Match> _matches;
};

class DefaultCondition_1501
{
public:
	void operator()( const Packets_t& states_1502, const Packets_t& compoundStatements_1506, Packets_t& states_1504, Packets_t& functions_1505, Packets_t& conditionalBlocks_1508);

protected:
	bool isInputUnique( const Udm::Object& state_1510, const Udm::Object& compoundStatement_1519);
	void processInputPackets( const Packets_t& states_1502, const Packets_t& compoundStatements_1506);
	bool patternMatcher( const Udm::Object& state_150e, const Udm::Object& compoundStatement_1517);
	void effector();
	void outputAppender( const CyberComposition::Simulink::State& state_1528, const SFC::Function& function_152a, const SFC::ConditionalBlock& conditionalBlock_152c);

private:
	Packets_t* _state_1509;
	Packets_t* _function_150a;
	Packets_t* _conditionalBlock_150b;
	Packets_t _state_150c;
	Packets_t _compoundStatement_1515;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_1522;
		SFC::CompoundStatement compoundStatement_1523;
		SFC::Function function_1524;
	};

	std::list< Match> _matches;
};

class ChildStateType_152e
{
public:
	void operator()( const Packets_t& states_152f, const Packets_t& css_1531, Packets_t& states_1533, Packets_t& css_1534, Packets_t& states_1535, Packets_t& css_1536);

protected:
	void executeOne( const Packets_t& states_152f, const Packets_t& css_1531);
	bool isInputUnique( const Udm::Object& state_153d, const Udm::Object& cs_1544);

private:
	Packets_t* _state_1537;
	Packets_t* _cs_1538;
	Packets_t* _state_1539;
	Packets_t* _cs_153a;
	Packets_t _state_153b;
	Packets_t _cs_1542;
};

class ChildANDStates_1549
{
public:
	bool operator()( const Packets_t& states_154a, const Packets_t& compoundStatements_154d, Packets_t& states_154c, Packets_t& compoundStatements_154f);

protected:
	bool isInputUnique( const Udm::Object& state_1556, const Udm::Object& compoundStatement_155f);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( CyberComposition::Simulink::State& ChildState, SFC::CompoundStatement& CompoundStatement, CyberComposition::Simulink::State& State);
	void processInputPackets( const Packets_t& states_154a, const Packets_t& compoundStatements_154d);
	bool patternMatcher( const Udm::Object& state_1554, const Udm::Object& compoundStatement_155d);
	void outputAppender( const CyberComposition::Simulink::State& state_1571, const SFC::CompoundStatement& compoundStatement_1573);

private:
	Packets_t* _state_1550;
	Packets_t* _compoundStatement_1551;
	Packets_t _state_1552;
	Packets_t _compoundStatement_155b;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_156b;
		SFC::CompoundStatement compoundStatement_156c;
		CyberComposition::Simulink::State childState_156d;
	};

	std::list< Match> _matches;
};

class Otherwise_1575
{
public:
	bool operator()( const Packets_t& states_1576, const Packets_t& compoundStatements_1579, Packets_t& states_1578, Packets_t& compoundStatements_157b);

protected:
	bool isInputUnique( const Udm::Object& state_1582, const Udm::Object& compoundStatement_158b);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& states_1576, const Packets_t& compoundStatements_1579);
	bool patternMatcher( const Udm::Object& state_1580, const Udm::Object& compoundStatement_1589);
	void outputAppender( const CyberComposition::Simulink::State& state_1596, const SFC::CompoundStatement& compoundStatement_1598);

private:
	Packets_t* _state_157c;
	Packets_t* _compoundStatement_157d;
	Packets_t _state_157e;
	Packets_t _compoundStatement_1587;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_1594;
		SFC::CompoundStatement compoundStatement_1595;
	};

	std::list< Match> _matches;
};

class CallActiveChildExec_159a
{
public:
	void operator()( const Packets_t& states_159b, const Packets_t& css_159d, Packets_t& states_159f, Packets_t& css_15a0);

protected:
	void callGetChildStates_15e0( const Packets_t& states_13b0, const Packets_t& compoundStatements_13b4);
	void callExecActiveChildState_15e3( const Packets_t& states_15a4, const Packets_t& childStates_15a6, const Packets_t& compoundStatements_15a8);

private:
	Packets_t* _state_15a1;
	Packets_t* _cs_15a2;
};

class ExecActiveChildState_15a3
{
public:
	void operator()( const Packets_t& states_15a4, const Packets_t& childStates_15a6, const Packets_t& compoundStatements_15a8);

protected:
	bool isInputUnique( const Udm::Object& state_15ae, const Udm::Object& childState_15b7, const Udm::Object& compoundStatement_15c0);
	void processInputPackets( const Packets_t& states_15a4, const Packets_t& childStates_15a6, const Packets_t& compoundStatements_15a8);
	bool patternMatcher( const Udm::Object& state_15ac, const Udm::Object& childState_15b5, const Udm::Object& compoundStatement_15be);
	void effector();

private:
	Packets_t _state_15aa;
	Packets_t _childState_15b3;
	Packets_t _compoundStatement_15bc;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_15d4;
		CyberComposition::Simulink::State childState_15d5;
		SFC::CompoundStatement compoundStatement_15d6;
		SFC::StateLabel stateLabel_15d7;
		SFC::StateLabel childStateLabel_15d8;
		SFC::Function function_15d9;
		SFC::Program program_15da;
		SFC::StateVar stateVar_15db;
	};

	std::list< Match> _matches;
};

class CG_15e7
{
public:
	void operator()( const Packets_t& states_15e8, const Packets_t& compoundStatements_15eb, Packets_t& states_15ea, Packets_t& conditionalGroups_15ed);

protected:
	bool isInputUnique( const Udm::Object& state_15f4, const Udm::Object& compoundStatement_15fd);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& states_15e8, const Packets_t& compoundStatements_15eb);
	bool patternMatcher( const Udm::Object& state_15f2, const Udm::Object& compoundStatement_15fb);
	void effector();
	void outputAppender( const CyberComposition::Simulink::State& state_1609, const SFC::ConditionalGroup& conditionalGroup_160b);

private:
	Packets_t* _state_15ee;
	Packets_t* _conditionalGroup_15ef;
	Packets_t _state_15f0;
	Packets_t _compoundStatement_15f9;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_1606;
		SFC::CompoundStatement compoundStatement_1607;
	};

	std::list< Match> _matches;
};

class GetExecFcn_160d
{
public:
	void operator()( const Packets_t& states_160e, Packets_t& states_1610, Packets_t& functions_1611);

protected:
	bool isInputUnique( const Udm::Object& state_1618);
	void processInputPackets( const Packets_t& states_160e);
	bool patternMatcher( const Udm::Object& state_1616);
	void effector();
	void outputAppender( const CyberComposition::Simulink::State& state_1623, const SFC::Function& function_1625);

private:
	Packets_t* _state_1612;
	Packets_t* _function_1613;
	Packets_t _state_1614;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_1621;
		SFC::Function function_1622;
	};

	std::list< Match> _matches;
};

class InnerTransitions_164b
{
public:
	void operator()( const Packets_t& states_164c, Packets_t& states_164e);

protected:
	void callCheckForInnerTransitions_18ea( const Packets_t& states_1822);
	void callInit_18ec( const Packets_t& states_184f);
	void callCreateTransitionCG_18ee( const Packets_t& states_7c9, const Packets_t& transConnectors_7cc, const Packets_t& functions_7cf, const Packets_t& tCVarCGs_7d3);
	void callExecInnerTransitions_18f3( const Packets_t& states_1651, const Packets_t& tcs_1653, const Packets_t& fcns_1655, const Packets_t& css_1657);
	void callNoDefaultTransition_18f8( const Packets_t& states_88d, const Packets_t& tcs_88f, const Packets_t& fcns_891, const Packets_t& css_893);

private:
	Packets_t* _state_164f;
};

class ExecInnerTransitions_1650
{
public:
	void operator()( const Packets_t& states_1651, const Packets_t& tcs_1653, const Packets_t& fcns_1655, const Packets_t& css_1657, Packets_t& states_1659, Packets_t& tcs_165a, Packets_t& fcns_165b, Packets_t& css_165c);

protected:
	void callGetInnerTransitions_1817( const Packets_t& states_1662, const Packets_t& parentStates_1665, const Packets_t& functions_1668, const Packets_t& compoundStatements_166b);
	void callExecProcessTransitions_181c( const Packets_t& states_16b0, const Packets_t& transs_16b2, const Packets_t& fcns_16b4, const Packets_t& css_16b6);

private:
	Packets_t* _state_165d;
	Packets_t* _tc_165e;
	Packets_t* _fcn_165f;
	Packets_t* _cs_1660;
};

class GetInnerTransitions_1661
{
public:
	void operator()( const Packets_t& states_1662, const Packets_t& parentStates_1665, const Packets_t& functions_1668, const Packets_t& compoundStatements_166b, Packets_t& states_1664, Packets_t& transitions_1667, Packets_t& functions_166a, Packets_t& compoundStatements_166d);

protected:
	bool isInputUnique( const Udm::Object& state_1676, const Udm::Object& parentState_167f, const Udm::Object& function_1688, const Udm::Object& compoundStatement_1691);
	void processInputPackets( const Packets_t& states_1662, const Packets_t& parentStates_1665, const Packets_t& functions_1668, const Packets_t& compoundStatements_166b);
	bool patternMatcher( const Udm::Object& state_1674, const Udm::Object& parentState_167d, const Udm::Object& function_1686, const Udm::Object& compoundStatement_168f);
	void effector();
	void outputAppender( const CyberComposition::Simulink::State& state_16a7, const CyberComposition::Simulink::Transition& transition_16a9, const SFC::Function& function_16ab, const SFC::CompoundStatement& compoundStatement_16ad);

private:
	Packets_t* _state_166e;
	Packets_t* _transition_166f;
	Packets_t* _function_1670;
	Packets_t* _compoundStatement_1671;
	Packets_t _state_1672;
	Packets_t _parentState_167b;
	Packets_t _function_1684;
	Packets_t _compoundStatement_168d;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_16a0;
		CyberComposition::Simulink::State parentState_16a1;
		SFC::Function function_16a2;
		SFC::CompoundStatement compoundStatement_16a3;
		CyberComposition::Simulink::ConnectorRef connectorRef_16a4;
		CyberComposition::Simulink::TransConnector transConnector_16a5;
		CyberComposition::Simulink::Transition transition_16a6;
	};

	std::list< Match> _matches;
};

class ExecProcessTransitions_16af
{
public:
	void operator()( const Packets_t& states_16b0, const Packets_t& transs_16b2, const Packets_t& fcns_16b4, const Packets_t& css_16b6);

protected:
	void callOrderTransitions_17de( const Packets_t& states_63f, const Packets_t& transitions_642, const Packets_t& functions_645, const Packets_t& compoundStatements_648);
	void callTransCond_17e3( const Packets_t& states_331, const Packets_t& transitions_335, const Packets_t& functions_338, const Packets_t& transitionCGs_33b);
	void callHighestTransParent_17e8( const Packets_t& states_16b9, const Packets_t& dstTransConnectors_16bc, const Packets_t& transitions_16bf, const Packets_t& functions_16c2, const Packets_t& conditionalBlocks_16c5);
	void callTest_17ee( const Packets_t& states_37c, const Packets_t& dsts_37e, const Packets_t& transs_380, const Packets_t& fcns_382, const Packets_t& cbs_384);
	void callGetDstJuncFromRef_17f4( const Packets_t& states_23b, const Packets_t& connectorRefs_23e, const Packets_t& transitions_241, const Packets_t& functions_244, const Packets_t& conditionalBlocks_247);
	void callGetDstStateFromRef_17fa( const Packets_t& states_290, const Packets_t& connectorRefs_294, const Packets_t& transitions_296, const Packets_t& functions_299, const Packets_t& conditionalBlocks_29c);
	void callDeadEndJunction_1800( const Packets_t& states_2e5, const Packets_t& transConnectors_2e7, const Packets_t& transitions_2e9, const Packets_t& functions_2eb, const Packets_t& transitionCBs_2ed);
	void callNextJunction_1806( const Packets_t& states_1d3, const Packets_t& junctions_1d6, const Packets_t& transitions_1d9, const Packets_t& functions_1db, const Packets_t& transitionCBs_1de);
	void callExecNextState_180c( const Packets_t& states_1721, const Packets_t& dstStates_1723, const Packets_t& transitions_1725, const Packets_t& functions_1727, const Packets_t& transitionCBs_1729);
	void callExecTransConnectorRecurse_1812( const Packets_t& states_178b, const Packets_t& tcs_178d, const Packets_t& fcns_178f, const Packets_t& cgs_1791);
};

class HighestTransParent_16b8
{
public:
	void operator()( const Packets_t& states_16b9, const Packets_t& dstTransConnectors_16bc, const Packets_t& transitions_16bf, const Packets_t& functions_16c2, const Packets_t& conditionalBlocks_16c5, Packets_t& states_16bb, Packets_t& dstTransConnectors_16be, Packets_t& transitions_16c1, Packets_t& functions_16c4, Packets_t& conditionalBlocks_16c7);

protected:
	bool isInputUnique( const Udm::Object& state_16d1, const Udm::Object& dstTransConnector_16da, const Udm::Object& transition_16e3, const Udm::Object& function_16ec, const Udm::Object& conditionalBlock_16f5);
	bool isGuardTrue( SFC::ConditionalBlock& ConditionalBlock, CyberComposition::Simulink::TransConnector& DstTransConnector, SFC::Function& Function, SFC::LocalVar& HTPVar, CyberComposition::Simulink::State& State, SFC::StateLabel& StateLabel, CyberComposition::Simulink::Transition& Transition, CyberComposition::Simulink::State& TransitionParentState);
	void processInputPackets( const Packets_t& states_16b9, const Packets_t& dstTransConnectors_16bc, const Packets_t& transitions_16bf, const Packets_t& functions_16c2, const Packets_t& conditionalBlocks_16c5);
	bool patternMatcher( const Udm::Object& state_16cf, const Udm::Object& dstTransConnector_16d8, const Udm::Object& transition_16e1, const Udm::Object& function_16ea, const Udm::Object& conditionalBlock_16f3);
	void effector();
	void forwardInputs();

private:
	Packets_t* _state_16c8;
	Packets_t* _dstTransConnector_16c9;
	Packets_t* _transition_16ca;
	Packets_t* _function_16cb;
	Packets_t* _conditionalBlock_16cc;
	Packets_t _state_16cd;
	Packets_t _dstTransConnector_16d6;
	Packets_t _transition_16df;
	Packets_t _function_16e8;
	Packets_t _conditionalBlock_16f1;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_1703;
		CyberComposition::Simulink::TransConnector dstTransConnector_1704;
		CyberComposition::Simulink::Transition transition_1705;
		SFC::Function function_1706;
		SFC::ConditionalBlock conditionalBlock_1707;
		CyberComposition::Simulink::State transitionParentState_1708;
		SFC::StateLabel stateLabel_1709;
		SFC::LocalVar hTPVar_170a;
	};

	std::list< Match> _matches;
};

class ExecNextState_1720
{
public:
	void operator()( const Packets_t& states_1721, const Packets_t& dstStates_1723, const Packets_t& transitions_1725, const Packets_t& functions_1727, const Packets_t& transitionCBs_1729);

protected:
	bool isInputUnique( const Udm::Object& state_172f, const Udm::Object& dstState_1738, const Udm::Object& transition_1741, const Udm::Object& function_174a, const Udm::Object& transitionCB_1753);
	bool isGuardTrue( CyberComposition::Simulink::State& DstState, SFC::Function& Enter, SFC::Arg& EnterArg0, SFC::Arg& EnterArg1, SFC::Function& Exit, SFC::Arg& ExitArg, SFC::Function& Function, SFC::LocalVar& HTPVar, CyberComposition::Simulink::State& State, CyberComposition::Simulink::Transition& Transition, SFC::ConditionalBlock& TransitionCB);
	void processInputPackets( const Packets_t& states_1721, const Packets_t& dstStates_1723, const Packets_t& transitions_1725, const Packets_t& functions_1727, const Packets_t& transitionCBs_1729);
	bool patternMatcher( const Udm::Object& state_172d, const Udm::Object& dstState_1736, const Udm::Object& transition_173f, const Udm::Object& function_1748, const Udm::Object& transitionCB_1751);
	void effector();

private:
	Packets_t _state_172b;
	Packets_t _dstState_1734;
	Packets_t _transition_173d;
	Packets_t _function_1746;
	Packets_t _transitionCB_174f;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_176b;
		CyberComposition::Simulink::State dstState_176c;
		CyberComposition::Simulink::Transition transition_176d;
		SFC::Function function_176e;
		SFC::ConditionalBlock transitionCB_176f;
		SFC::Arg exitArg_1770;
		SFC::Function exit_1771;
		SFC::Arg enterArg0_1772;
		SFC::Function enter_1773;
		SFC::Arg enterArg1_1774;
		SFC::LocalVar hTPVar_1775;
	};

	std::list< Match> _matches;
};

class ExecTransConnectorRecurse_178a
{
public:
	void operator()( const Packets_t& states_178b, const Packets_t& tcs_178d, const Packets_t& fcns_178f, const Packets_t& cgs_1791);

protected:
	void executeOne( const Packets_t& states_178b, const Packets_t& tcs_178d, const Packets_t& fcns_178f, const Packets_t& cgs_1791);
	bool isInputUnique( const Udm::Object& state_1795, const Udm::Object& tc_179c, const Udm::Object& fcn_17a3, const Udm::Object& cg_17aa);
	void callProcessedFilter_17ca( const Packets_t& states_81a, const Packets_t& tcs_81c, const Packets_t& fcns_81e, const Packets_t& cgs_820);
	void callCreateTransitionCG_17cf( const Packets_t& states_7c9, const Packets_t& transConnectors_7cc, const Packets_t& functions_7cf, const Packets_t& tCVarCGs_7d3);
	void callExecTransitions_17d4( const Packets_t& states_17b0, const Packets_t& tcs_17b2, const Packets_t& fcns_17b4, const Packets_t& css_17b6);
	void callNoDefaultTransition_17d9( const Packets_t& states_88d, const Packets_t& tcs_88f, const Packets_t& fcns_891, const Packets_t& css_893);

private:
	Packets_t _state_1793;
	Packets_t _tc_179a;
	Packets_t _fcn_17a1;
	Packets_t _cg_17a8;
};

class ExecTransitions_17af
{
public:
	void operator()( const Packets_t& states_17b0, const Packets_t& tcs_17b2, const Packets_t& fcns_17b4, const Packets_t& css_17b6, Packets_t& states_17b8, Packets_t& tcs_17b9, Packets_t& fcns_17ba, Packets_t& css_17bb);

protected:
	void callGetAllTransitions_17c0( const Packets_t& states_6b4, const Packets_t& tcs_6b6, const Packets_t& fcns_6b8, const Packets_t& css_6ba);
	void callExecProcessTransitions_17c5( const Packets_t& states_16b0, const Packets_t& transs_16b2, const Packets_t& fcns_16b4, const Packets_t& css_16b6);

private:
	Packets_t* _state_17bc;
	Packets_t* _tc_17bd;
	Packets_t* _fcn_17be;
	Packets_t* _cs_17bf;
};

class CheckForInnerTransitions_1821
{
public:
	void operator()( const Packets_t& states_1822, Packets_t& states_1824);

protected:
	void executeOne( const Packets_t& states_1822);
	bool isInputUnique( const Udm::Object& state_1828);

private:
	Packets_t* _state_1825;
	Packets_t _state_1826;
};

class HasInnerTransition_182d
{
public:
	bool operator()( const Packets_t& states_182e, Packets_t& states_1830);

protected:
	bool isInputUnique( const Udm::Object& state_1836);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& states_182e);
	bool patternMatcher( const Udm::Object& state_1834);
	void outputAppender( const CyberComposition::Simulink::State& state_184c);

private:
	Packets_t* _state_1831;
	Packets_t _state_1832;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_1848;
		CyberComposition::Simulink::Transition transition_1849;
		CyberComposition::Simulink::ConnectorRef connectorRef_184a;
		CyberComposition::Simulink::TransConnector transConnector_184b;
	};

	std::list< Match> _matches;
};

class Init_184e
{
public:
	void operator()( const Packets_t& states_184f, Packets_t& states_1851, Packets_t& tcs_1852, Packets_t& fcns_1853, Packets_t& cgs_1854);

protected:
	void callGetExecFunction_18dc( const Packets_t& states_18c3);
	void callInitHTPVar_18de( const Packets_t& states_1886, const Packets_t& functions_1889, const Packets_t& compoundStatements_188c);
	void callInitTCVarLoop_18e2( const Packets_t& states_f3, const Packets_t& functions_f6, const Packets_t& compoundStatements_f9);
	void callGetStartTC_18e6( const Packets_t& states_185a, const Packets_t& functions_185d, const Packets_t& conditionalGroups_1860);

private:
	Packets_t* _state_1855;
	Packets_t* _tc_1856;
	Packets_t* _fcn_1857;
	Packets_t* _cg_1858;
};

class GetStartTC_1859
{
public:
	void operator()( const Packets_t& states_185a, const Packets_t& functions_185d, const Packets_t& conditionalGroups_1860, Packets_t& states_185c, Packets_t& functions_185f, Packets_t& conditionalGroups_1862);

protected:
	bool isInputUnique( const Udm::Object& state_186a, const Udm::Object& function_1873, const Udm::Object& conditionalGroup_187c);
	void processInputPackets( const Packets_t& states_185a, const Packets_t& functions_185d, const Packets_t& conditionalGroups_1860);
	bool patternMatcher( const Udm::Object& state_1868, const Udm::Object& function_1871, const Udm::Object& conditionalGroup_187a);
	void effector();
	void forwardInputs();

private:
	Packets_t* _state_1863;
	Packets_t* _function_1864;
	Packets_t* _conditionalGroup_1865;
	Packets_t _state_1866;
	Packets_t _function_186f;
	Packets_t _conditionalGroup_1878;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_1882;
		SFC::Function function_1883;
		SFC::ConditionalGroup conditionalGroup_1884;
	};

	std::list< Match> _matches;
};

class InitHTPVar_1885
{
public:
	void operator()( const Packets_t& states_1886, const Packets_t& functions_1889, const Packets_t& compoundStatements_188c, Packets_t& states_1888, Packets_t& functions_188b, Packets_t& compoundStatements_188e);

protected:
	bool isInputUnique( const Udm::Object& state_1896, const Udm::Object& function_189f, const Udm::Object& compoundStatement_18a8);
	bool isGuardTrue( SFC::CompoundStatement& CompoundStatement, SFC::Function& Function, SFC::LocalVar& HTPVar, CyberComposition::Simulink::State& State, SFC::StateLabel& StateLabel);
	void processInputPackets( const Packets_t& states_1886, const Packets_t& functions_1889, const Packets_t& compoundStatements_188c);
	bool patternMatcher( const Udm::Object& state_1894, const Udm::Object& function_189d, const Udm::Object& compoundStatement_18a6);
	void effector();
	void forwardInputs();

private:
	Packets_t* _state_188f;
	Packets_t* _function_1890;
	Packets_t* _compoundStatement_1891;
	Packets_t _state_1892;
	Packets_t _function_189b;
	Packets_t _compoundStatement_18a4;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_18b4;
		SFC::Function function_18b5;
		SFC::CompoundStatement compoundStatement_18b6;
		SFC::StateLabel stateLabel_18b7;
		SFC::LocalVar hTPVar_18b8;
	};

	std::list< Match> _matches;
};

class GetExecFunction_18c2
{
public:
	void operator()( const Packets_t& states_18c3, Packets_t& states_18c5, Packets_t& execs_18c6);

protected:
	bool isInputUnique( const Udm::Object& state_18cd);
	void processInputPackets( const Packets_t& states_18c3);
	bool patternMatcher( const Udm::Object& state_18cb);
	void effector();
	void outputAppender( const CyberComposition::Simulink::State& state_18d8, const SFC::Function& exec_18da);

private:
	Packets_t* _state_18c7;
	Packets_t* _exec_18c8;
	Packets_t _state_18c9;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_18d6;
		SFC::Function exec_18d7;
	};

	std::list< Match> _matches;
};

class InitTransConnMapInner_18fd
{
public:
	void operator()( const Packets_t& states_18fe, Packets_t& states_1900);

protected:
	void callInitTransConnectorMap_1940( const Packets_t& states_1931);
	void callAddTransConnector_1942( const Packets_t& states_ad9, const Packets_t& transConnectors_adc);
	void callGetInnerTransitions_1945( const Packets_t& states_1903, const Packets_t& states_1907);
	void callProcessTransitionsMap_1948( const Packets_t& states_af7, const Packets_t& transs_af9);

private:
	Packets_t* _state_1901;
};

class GetInnerTransitions_1902
{
public:
	void operator()( const Packets_t& states_1903, const Packets_t& states_1907, Packets_t& states_1905, Packets_t& transitions_1906);

protected:
	bool isInputUnique( const Udm::Object& state_190f, const Udm::Object& state_1918);
	void processInputPackets( const Packets_t& states_1903, const Packets_t& states_1907);
	bool patternMatcher( const Udm::Object& state_190d, const Udm::Object& state_1916);
	void effector();
	void outputAppender( const CyberComposition::Simulink::State& state_192c, const CyberComposition::Simulink::Transition& transition_192e);

private:
	Packets_t* _state_1909;
	Packets_t* _transition_190a;
	Packets_t _state_190b;
	Packets_t _state_1914;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_1927;
		CyberComposition::Simulink::State state_1928;
		CyberComposition::Simulink::TransConnector dstTransConnector_1929;
		CyberComposition::Simulink::Transition transition_192a;
		CyberComposition::Simulink::ConnectorRef connectorRef_192b;
	};

	std::list< Match> _matches;
};

class InitTransConnectorMap_1930
{
public:
	void operator()( const Packets_t& states_1931, Packets_t& states_1933);

protected:
	bool isInputUnique( const Udm::Object& state_1939);
	void processInputPackets( const Packets_t& states_1931);
	bool patternMatcher( const Udm::Object& state_1937);
	void effector();
	void forwardInputs();

private:
	Packets_t* _state_1934;
	Packets_t _state_1935;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_193f;
	};

	std::list< Match> _matches;
};

class DuringAction_194b
{
public:
	void operator()( const Packets_t& states_194c, Packets_t& states_194e);

protected:
	bool isInputUnique( const Udm::Object& state_1954);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& states_194c);
	bool patternMatcher( const Udm::Object& state_1952);
	void effector();
	void forwardInputs();

private:
	Packets_t* _state_194f;
	Packets_t _state_1950;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_1960;
		SFC::Function exec_1961;
	};

	std::list< Match> _matches;
};

class OuterTransitions_1962
{
public:
	void operator()( const Packets_t& states_1963, Packets_t& states_1965);

protected:
	void callCheckForOuterTransitions_19cf( const Packets_t& states_1968);
	void callInit_19d1( const Packets_t& states_184f);
	void callExecTransConnectorRecurse_19d3( const Packets_t& states_178b, const Packets_t& tcs_178d, const Packets_t& fcns_178f, const Packets_t& cgs_1791);

private:
	Packets_t* _state_1966;
};

class CheckForOuterTransitions_1967
{
public:
	void operator()( const Packets_t& states_1968, Packets_t& states_196a);

protected:
	void executeOne( const Packets_t& states_1968);
	bool isInputUnique( const Udm::Object& state_196e);

private:
	Packets_t* _state_196b;
	Packets_t _state_196c;
};

class HasOuterTransition_1973
{
public:
	bool operator()( const Packets_t& states_1974, Packets_t& states_1976);

protected:
	bool isInputUnique( const Udm::Object& state_197c);
	void processInputPackets( const Packets_t& states_1974);
	bool patternMatcher( const Udm::Object& state_197a);
	void outputAppender( const CyberComposition::Simulink::State& state_1989);

private:
	Packets_t* _state_1977;
	Packets_t _state_1978;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_1986;
		CyberComposition::Simulink::Transition transition_1987;
		CyberComposition::Simulink::TransConnector transConnector_1988;
	};

	std::list< Match> _matches;
};

class HasInnerTransition_198b
{
public:
	bool operator()( const Packets_t& states_198c);

protected:
	bool isInputUnique( const Udm::Object& state_1992);
	void processInputPackets( const Packets_t& states_198c);
	bool patternMatcher( const Udm::Object& state_1990);
	void outputAppender();

private:
	Packets_t _state_198e;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_19a1;
		CyberComposition::Simulink::Transition transition_19a2;
		CyberComposition::Simulink::ConnectorRef connectorRef_19a3;
		CyberComposition::Simulink::TransConnector transConnector_19a4;
	};

	std::list< Match> _matches;
};

class HasRefOuterTransition_19a5
{
public:
	bool operator()( const Packets_t& states_19a6, Packets_t& states_19a8);

protected:
	bool isInputUnique( const Udm::Object& state_19ae);
	void processInputPackets( const Packets_t& states_19a6);
	bool patternMatcher( const Udm::Object& state_19ac);
	void outputAppender( const CyberComposition::Simulink::State& state_19bf);

private:
	Packets_t* _state_19a9;
	Packets_t _state_19aa;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_19bb;
		CyberComposition::Simulink::Transition transition_19bc;
		CyberComposition::Simulink::ConnectorRef connectorRef_19bd;
		CyberComposition::Simulink::TransConnector transConnector_19be;
	};

	std::list< Match> _matches;
};

class Otherwise_19c1
{
public:
	bool operator()( const Packets_t& states_19c2);

protected:
	bool isInputUnique( const Udm::Object& state_19c8);
	void processInputPackets( const Packets_t& states_19c2);
	bool patternMatcher( const Udm::Object& state_19c6);
	void outputAppender();

private:
	Packets_t _state_19c4;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_19ce;
	};

	std::list< Match> _matches;
};

class InitTransConnMap_19d8
{
public:
	void operator()( const Packets_t& states_19d9, Packets_t& states_19db);

protected:
	void callInitTransConnectorMap_19dd( const Packets_t& states_1931);
	void callTransConnectorMapRecurse_19df( const Packets_t& states_a2b, const Packets_t& tcs_a2d);

private:
	Packets_t* _state_19dc;
};

class PopulateExitFunction_19ee
{
public:
	void operator()( const Packets_t& states_19ef, const Packets_t& programs_19f1, Packets_t& states_19f3, Packets_t& programs_19f4);

protected:
	void callGetExitFunction_1c0d( const Packets_t& states_1bf1);
	void callExitChildStates_1c0f( const Packets_t& states_1a33, const Packets_t& fcns_1a35);
	void callGoToParent_1c12( const Packets_t& states_19f8, const Packets_t& fcns_19fa);

private:
	Packets_t* _state_19f5;
	Packets_t* _program_19f6;
};

class GoToParent_19f7
{
public:
	void operator()( const Packets_t& states_19f8, const Packets_t& fcns_19fa, Packets_t& states_19fc, Packets_t& fcns_19fd);

protected:
	void callArgToParent_1a2f( const Packets_t& states_1a01, const Packets_t& functions_1a03);

private:
	Packets_t* _state_19fe;
	Packets_t* _fcn_19ff;
};

class ArgToParent_1a00
{
public:
	void operator()( const Packets_t& states_1a01, const Packets_t& functions_1a03);

protected:
	bool isInputUnique( const Udm::Object& state_1a09, const Udm::Object& function_1a12);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& states_1a01, const Packets_t& functions_1a03);
	bool patternMatcher( const Udm::Object& state_1a07, const Udm::Object& function_1a10);
	void effector();

private:
	Packets_t _state_1a05;
	Packets_t _function_1a0e;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_1a26;
		SFC::Function function_1a27;
		CyberComposition::Simulink::State parentState_1a28;
		SFC::Arg parentArg_1a29;
		SFC::Function parentExit_1a2a;
		SFC::Arg arg_1a2b;
	};

	std::list< Match> _matches;
};

class ExitChildStates_1a32
{
public:
	void operator()( const Packets_t& states_1a33, const Packets_t& fcns_1a35, Packets_t& states_1a37, Packets_t& fcns_1a38);

protected:
	void callArgNegOneOrState_1be5( const Packets_t& states_1bac, const Packets_t& functions_1bb0);
	void callCallChildExits_1be8( const Packets_t& states_1a82, const Packets_t& args_1a84, const Packets_t& cbs_1a86);
	void callDeactivateState_1bec( const Packets_t& states_1a3c, const Packets_t& args_1a3e, const Packets_t& childrenCalledCBs_1a40);

private:
	Packets_t* _state_1a39;
	Packets_t* _fcn_1a3a;
};

class DeactivateState_1a3b
{
public:
	void operator()( const Packets_t& states_1a3c, const Packets_t& args_1a3e, const Packets_t& childrenCalledCBs_1a40);

protected:
	bool isInputUnique( const Udm::Object& state_1a46, const Udm::Object& arg_1a4f, const Udm::Object& childrenCalledCB_1a58);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& states_1a3c, const Packets_t& args_1a3e, const Packets_t& childrenCalledCBs_1a40);
	bool patternMatcher( const Udm::Object& state_1a44, const Udm::Object& arg_1a4d, const Udm::Object& childrenCalledCB_1a56);
	void effector();

private:
	Packets_t _state_1a42;
	Packets_t _arg_1a4b;
	Packets_t _childrenCalledCB_1a54;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_1a71;
		SFC::Arg arg_1a72;
		SFC::ConditionalBlock childrenCalledCB_1a73;
		CyberComposition::Simulink::State parentState_1a74;
		SFC::Function function_1a75;
		SFC::StateLabel stateLabel_1a76;
		SFC::StateLabel parentStateLabel_1a77;
		SFC::Program program_1a78;
		SFC::StateVar stateVar_1a79;
	};

	std::list< Match> _matches;
};

class CallChildExits_1a81
{
public:
	void operator()( const Packets_t& states_1a82, const Packets_t& args_1a84, const Packets_t& cbs_1a86, Packets_t& states_1a88, Packets_t& args_1a89, Packets_t& cbs_1a8a);

protected:
	void callOrStateCG_1ba1( const Packets_t& states_1a8f, const Packets_t& css_1a91);
	void callROrderChildStates_1ba4( const Packets_t& states_1b30, const Packets_t& compoundStatements_1b34);
	void callExitChildStates_1ba7( const Packets_t& states_1b5d, const Packets_t& childStates_1b5f, const Packets_t& compoundStatements_1b61);

private:
	Packets_t* _state_1a8b;
	Packets_t* _arg_1a8c;
	Packets_t* _cb_1a8d;
};

class OrStateCG_1a8e
{
public:
	void operator()( const Packets_t& states_1a8f, const Packets_t& css_1a91, Packets_t& states_1a93, Packets_t& css_1a94);

protected:
	void callChildStateType_1b29( const Packets_t& states_1abe, const Packets_t& css_1ac0);
	void callCG_1b2c( const Packets_t& states_1a98, const Packets_t& compoundStatements_1a9b);

private:
	Packets_t* _state_1a95;
	Packets_t* _cs_1a96;
};

class CG_1a97
{
public:
	void operator()( const Packets_t& states_1a98, const Packets_t& compoundStatements_1a9b, Packets_t& states_1a9a, Packets_t& conditionalGroups_1a9d);

protected:
	bool isInputUnique( const Udm::Object& state_1aa4, const Udm::Object& compoundStatement_1aad);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& states_1a98, const Packets_t& compoundStatements_1a9b);
	bool patternMatcher( const Udm::Object& state_1aa2, const Udm::Object& compoundStatement_1aab);
	void effector();
	void outputAppender( const CyberComposition::Simulink::State& state_1ab9, const SFC::ConditionalGroup& conditionalGroup_1abb);

private:
	Packets_t* _state_1a9e;
	Packets_t* _conditionalGroup_1a9f;
	Packets_t _state_1aa0;
	Packets_t _compoundStatement_1aa9;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_1ab6;
		SFC::CompoundStatement compoundStatement_1ab7;
	};

	std::list< Match> _matches;
};

class ChildStateType_1abd
{
public:
	void operator()( const Packets_t& states_1abe, const Packets_t& css_1ac0, Packets_t& states_1ac2, Packets_t& css_1ac3, Packets_t& states_1ac4, Packets_t& css_1ac5);

protected:
	void executeOne( const Packets_t& states_1abe, const Packets_t& css_1ac0);
	bool isInputUnique( const Udm::Object& state_1acc, const Udm::Object& cs_1ad3);

private:
	Packets_t* _state_1ac6;
	Packets_t* _cs_1ac7;
	Packets_t* _state_1ac8;
	Packets_t* _cs_1ac9;
	Packets_t _state_1aca;
	Packets_t _cs_1ad1;
};

class ChildAndStates_1ad8
{
public:
	bool operator()( const Packets_t& states_1ad9, const Packets_t& compoundStatements_1adc, Packets_t& states_1adb, Packets_t& compoundStatements_1ade);

protected:
	bool isInputUnique( const Udm::Object& state_1ae5, const Udm::Object& compoundStatement_1aee);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( CyberComposition::Simulink::State& ChildState, SFC::CompoundStatement& CompoundStatement, CyberComposition::Simulink::State& State);
	void processInputPackets( const Packets_t& states_1ad9, const Packets_t& compoundStatements_1adc);
	bool patternMatcher( const Udm::Object& state_1ae3, const Udm::Object& compoundStatement_1aec);
	void outputAppender( const CyberComposition::Simulink::State& state_1b00, const SFC::CompoundStatement& compoundStatement_1b02);

private:
	Packets_t* _state_1adf;
	Packets_t* _compoundStatement_1ae0;
	Packets_t _state_1ae1;
	Packets_t _compoundStatement_1aea;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_1afa;
		SFC::CompoundStatement compoundStatement_1afb;
		CyberComposition::Simulink::State childState_1afc;
	};

	std::list< Match> _matches;
};

class ChildOrStates_1b04
{
public:
	bool operator()( const Packets_t& states_1b05, const Packets_t& compoundStatements_1b08, Packets_t& states_1b07, Packets_t& compoundStatements_1b0a);

protected:
	bool isInputUnique( const Udm::Object& state_1b11, const Udm::Object& compoundStatement_1b1a);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& states_1b05, const Packets_t& compoundStatements_1b08);
	bool patternMatcher( const Udm::Object& state_1b0f, const Udm::Object& compoundStatement_1b18);
	void outputAppender( const CyberComposition::Simulink::State& state_1b25, const SFC::CompoundStatement& compoundStatement_1b27);

private:
	Packets_t* _state_1b0b;
	Packets_t* _compoundStatement_1b0c;
	Packets_t _state_1b0d;
	Packets_t _compoundStatement_1b16;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_1b23;
		SFC::CompoundStatement compoundStatement_1b24;
	};

	std::list< Match> _matches;
};

class ROrderChildStates_1b2f
{
public:
	void operator()( const Packets_t& states_1b30, const Packets_t& compoundStatements_1b34, Packets_t& states_1b32, Packets_t& states_1b33, Packets_t& compoundStatements_1b36);

protected:
	bool isInputUnique( const Udm::Object& state_1b3e, const Udm::Object& compoundStatement_1b47);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& states_1b30, const Packets_t& compoundStatements_1b34);
	bool patternMatcher( const Udm::Object& state_1b3c, const Udm::Object& compoundStatement_1b45);
	void effector();
	void outputAppender( const CyberComposition::Simulink::State& state_1b56, const CyberComposition::Simulink::State& state_1b58, const SFC::CompoundStatement& compoundStatement_1b5a);
	void sortOutputs();

private:
	Packets_t* _state_1b37;
	Packets_t* _state_1b38;
	Packets_t* _compoundStatement_1b39;
	Packets_t _state_1b3a;
	Packets_t _compoundStatement_1b43;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_1b53;
		SFC::CompoundStatement compoundStatement_1b54;
		CyberComposition::Simulink::State state_1b55;
	};

	std::list< Match> _matches;
};

class ExitChildStates_1b5c
{
public:
	void operator()( const Packets_t& states_1b5d, const Packets_t& childStates_1b5f, const Packets_t& compoundStatements_1b61);

protected:
	bool isInputUnique( const Udm::Object& state_1b67, const Udm::Object& childState_1b70, const Udm::Object& compoundStatement_1b79);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& states_1b5d, const Packets_t& childStates_1b5f, const Packets_t& compoundStatements_1b61);
	bool patternMatcher( const Udm::Object& state_1b65, const Udm::Object& childState_1b6e, const Udm::Object& compoundStatement_1b77);
	void effector();

private:
	Packets_t _state_1b63;
	Packets_t _childState_1b6c;
	Packets_t _compoundStatement_1b75;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_1b93;
		CyberComposition::Simulink::State childState_1b94;
		SFC::CompoundStatement compoundStatement_1b95;
		SFC::StateLabel childStateLabel_1b96;
		SFC::StateLabel stateLabel_1b97;
		SFC::StateVar stateVar_1b98;
		SFC::Arg arg_1b99;
		SFC::Function exit_1b9a;
		SFC::Program program_1b9b;
	};

	std::list< Match> _matches;
};

class ArgNegOneOrState_1bab
{
public:
	void operator()( const Packets_t& states_1bac, const Packets_t& functions_1bb0, Packets_t& states_1bae, Packets_t& args_1baf, Packets_t& conditionalBlocks_1bb2);

protected:
	bool isInputUnique( const Udm::Object& state_1bba, const Udm::Object& function_1bc3);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& states_1bac, const Packets_t& functions_1bb0);
	bool patternMatcher( const Udm::Object& state_1bb8, const Udm::Object& function_1bc1);
	void effector();
	void outputAppender( const CyberComposition::Simulink::State& state_1bdf, const SFC::Arg& arg_1be1, const SFC::ConditionalBlock& conditionalBlock_1be3);

private:
	Packets_t* _state_1bb3;
	Packets_t* _arg_1bb4;
	Packets_t* _conditionalBlock_1bb5;
	Packets_t _state_1bb6;
	Packets_t _function_1bbf;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_1bd2;
		SFC::Function function_1bd3;
		SFC::StateLabel stateLabel_1bd4;
		SFC::Arg arg_1bd5;
	};

	std::list< Match> _matches;
};

class GetExitFunction_1bf0
{
public:
	void operator()( const Packets_t& states_1bf1, Packets_t& states_1bf3, Packets_t& exits_1bf4);

protected:
	bool isInputUnique( const Udm::Object& state_1bfb);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& states_1bf1);
	bool patternMatcher( const Udm::Object& state_1bf9);
	void effector();
	void outputAppender( const CyberComposition::Simulink::State& state_1c09, const SFC::Function& exit_1c0b);

private:
	Packets_t* _state_1bf5;
	Packets_t* _exit_1bf6;
	Packets_t _state_1bf7;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_1c07;
		SFC::Function exit_1c08;
	};

	std::list< Match> _matches;
};

class PopulateEnterFunction_1c15
{
public:
	void operator()( const Packets_t& states_1c16, const Packets_t& programs_1c18, Packets_t& states_1c1a, Packets_t& programs_1c1b);

protected:
	void callTestEAIOff_1f35( const Packets_t& states_1eeb);
	void callReturnAtTopState_1f37( const Packets_t& states_1ebd);
	void callMode0or1_1f39( const Packets_t& states_1e34);
	void callMode0or1or2_1f3b( const Packets_t& states_1df3);
	void callMode0or2_1f3d( const Packets_t& states_1d5d);
	void callMode0or3_1f3f( const Packets_t& states_1c1f);

private:
	Packets_t* _state_1c1c;
	Packets_t* _program_1c1d;
};

class Mode0or3_1c1e
{
public:
	void operator()( const Packets_t& states_1c1f, Packets_t& states_1c21);

protected:
	void callTestMode03_1d54( const Packets_t& states_1d2b);
	void callCallLOSibs_1d56( const Packets_t& states_1c24, const Packets_t& css_1c26);
	void callCallParent_1d59( const Packets_t& states_1ced, const Packets_t& conditionalBlocks_1cef);

private:
	Packets_t* _state_1c22;
};

class CallLOSibs_1c23
{
public:
	void operator()( const Packets_t& states_1c24, const Packets_t& css_1c26, Packets_t& states_1c28, Packets_t& css_1c29);

protected:
	void callIsAndState_1ce2( const Packets_t& states_1c5e, const Packets_t& css_1c60);
	void callGetLOSibs_1ce5( const Packets_t& states_1c2d, const Packets_t& compoundStatements_1c31);
	void callCallSibsOrChildren_1ce8( const Packets_t& states_1c99, const Packets_t& sOCStates_1c9b, const Packets_t& compoundStatements_1c9d);

private:
	Packets_t* _state_1c2a;
	Packets_t* _cs_1c2b;
};

class GetLOSibs_1c2c
{
public:
	void operator()( const Packets_t& states_1c2d, const Packets_t& compoundStatements_1c31, Packets_t& states_1c2f, Packets_t& sibStates_1c30, Packets_t& compoundStatements_1c33);

protected:
	bool isInputUnique( const Udm::Object& state_1c3b, const Udm::Object& compoundStatement_1c44);
	bool isGuardTrue( SFC::CompoundStatement& CompoundStatement, CyberComposition::Simulink::State& ParentState, CyberComposition::Simulink::State& SibState, CyberComposition::Simulink::State& State);
	void processInputPackets( const Packets_t& states_1c2d, const Packets_t& compoundStatements_1c31);
	bool patternMatcher( const Udm::Object& state_1c39, const Udm::Object& compoundStatement_1c42);
	void effector();
	void outputAppender( const CyberComposition::Simulink::State& state_1c57, const CyberComposition::Simulink::State& sibState_1c59, const SFC::CompoundStatement& compoundStatement_1c5b);
	void sortOutputs();

private:
	Packets_t* _state_1c34;
	Packets_t* _sibState_1c35;
	Packets_t* _compoundStatement_1c36;
	Packets_t _state_1c37;
	Packets_t _compoundStatement_1c40;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_1c4f;
		SFC::CompoundStatement compoundStatement_1c50;
		CyberComposition::Simulink::State parentState_1c51;
		CyberComposition::Simulink::State sibState_1c52;
	};

	std::list< Match> _matches;
};

class IsAndState_1c5d
{
public:
	void operator()( const Packets_t& states_1c5e, const Packets_t& css_1c60, Packets_t& states_1c62, Packets_t& css_1c63);

protected:
	void executeOne( const Packets_t& states_1c5e, const Packets_t& css_1c60);
	bool isInputUnique( const Udm::Object& state_1c68, const Udm::Object& cs_1c6f);

private:
	Packets_t* _state_1c64;
	Packets_t* _cs_1c65;
	Packets_t _state_1c66;
	Packets_t _cs_1c6d;
};

class AndState_1c74
{
public:
	bool operator()( const Packets_t& states_1c75, const Packets_t& compoundStatements_1c78, Packets_t& states_1c77, Packets_t& compoundStatements_1c7a);

protected:
	bool isInputUnique( const Udm::Object& state_1c81, const Udm::Object& compoundStatement_1c8a);
	bool isGuardTrue( SFC::CompoundStatement& CompoundStatement, CyberComposition::Simulink::State& State);
	void processInputPackets( const Packets_t& states_1c75, const Packets_t& compoundStatements_1c78);
	bool patternMatcher( const Udm::Object& state_1c7f, const Udm::Object& compoundStatement_1c88);
	void outputAppender( const CyberComposition::Simulink::State& state_1c94, const SFC::CompoundStatement& compoundStatement_1c96);

private:
	Packets_t* _state_1c7b;
	Packets_t* _compoundStatement_1c7c;
	Packets_t _state_1c7d;
	Packets_t _compoundStatement_1c86;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_1c90;
		SFC::CompoundStatement compoundStatement_1c91;
	};

	std::list< Match> _matches;
};

class CallSibsOrChildren_1c98
{
public:
	void operator()( const Packets_t& states_1c99, const Packets_t& sOCStates_1c9b, const Packets_t& compoundStatements_1c9d);

protected:
	bool isInputUnique( const Udm::Object& state_1ca3, const Udm::Object& sOCState_1cac, const Udm::Object& compoundStatement_1cb5);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::Arg& Arg1, SFC::CompoundStatement& CompoundStatement, SFC::Function& Enter, SFC::Arg& SOCArg0, SFC::Arg& SOCArg1, SFC::Function& SOCEnter, CyberComposition::Simulink::State& SOCState, CyberComposition::Simulink::State& State);
	void processInputPackets( const Packets_t& states_1c99, const Packets_t& sOCStates_1c9b, const Packets_t& compoundStatements_1c9d);
	bool patternMatcher( const Udm::Object& state_1ca1, const Udm::Object& sOCState_1caa, const Udm::Object& compoundStatement_1cb3);
	void effector();

private:
	Packets_t _state_1c9f;
	Packets_t _sOCState_1ca8;
	Packets_t _compoundStatement_1cb1;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_1ccd;
		CyberComposition::Simulink::State sOCState_1cce;
		SFC::CompoundStatement compoundStatement_1ccf;
		SFC::Function enter_1cd0;
		SFC::Arg sOCArg0_1cd1;
		SFC::Arg sOCArg1_1cd2;
		SFC::Arg arg1_1cd3;
		SFC::Function sOCEnter_1cd4;
	};

	std::list< Match> _matches;
};

class CallParent_1cec
{
public:
	void operator()( const Packets_t& states_1ced, const Packets_t& conditionalBlocks_1cef);

protected:
	bool isInputUnique( const Udm::Object& state_1cf5, const Udm::Object& conditionalBlock_1cfe);
	bool isGuardTrue( SFC::Arg& Arg1, SFC::ConditionalBlock& ConditionalBlock, SFC::Function& Enter, SFC::Arg& ParentArg0, SFC::Arg& ParentArg1, SFC::Function& ParentEnter, CyberComposition::Simulink::State& ParentState, CyberComposition::Simulink::State& State);
	void processInputPackets( const Packets_t& states_1ced, const Packets_t& conditionalBlocks_1cef);
	bool patternMatcher( const Udm::Object& state_1cf3, const Udm::Object& conditionalBlock_1cfc);
	void effector();

private:
	Packets_t _state_1cf1;
	Packets_t _conditionalBlock_1cfa;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_1d15;
		SFC::ConditionalBlock conditionalBlock_1d16;
		CyberComposition::Simulink::State parentState_1d17;
		SFC::Arg parentArg1_1d18;
		SFC::Arg parentArg0_1d19;
		SFC::Function parentEnter_1d1a;
		SFC::Function enter_1d1b;
		SFC::Arg arg1_1d1c;
	};

	std::list< Match> _matches;
};

class TestMode03_1d2a
{
public:
	void operator()( const Packets_t& states_1d2b, Packets_t& states_1d2d, Packets_t& conditionalBlocks_1d2e);

protected:
	bool isInputUnique( const Udm::Object& state_1d35);
	bool isGuardTrue( SFC::Arg& Arg0, SFC::Function& Enter, CyberComposition::Simulink::State& State);
	void processInputPackets( const Packets_t& states_1d2b);
	bool patternMatcher( const Udm::Object& state_1d33);
	void effector();
	void outputAppender( const CyberComposition::Simulink::State& state_1d50, const SFC::ConditionalBlock& conditionalBlock_1d52);

private:
	Packets_t* _state_1d2f;
	Packets_t* _conditionalBlock_1d30;
	Packets_t _state_1d31;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_1d41;
		SFC::Arg arg0_1d42;
		SFC::Function enter_1d43;
	};

	std::list< Match> _matches;
};

class Mode0or2_1d5c
{
public:
	void operator()( const Packets_t& states_1d5d, Packets_t& states_1d5f);

protected:
	void callChildTest_1dda( const Packets_t& states_1d62);
	void callTestMode02_1ddc( const Packets_t& states_1db1);
	void callChildStateType_1dde( const Packets_t& states_152f, const Packets_t& css_1531);
	void callTransStartMap_1de1( const Packets_t& states_a08, const Packets_t& css_a0a);
	void callGetChildStates_1de4( const Packets_t& states_13b0, const Packets_t& compoundStatements_13b4);
	void callRefetchEnterFcn_1de7( const Packets_t& states_1d87, const Packets_t& compoundStatements_1d8b);
	void callCallSibsOrChildren_1dea( const Packets_t& states_1c99, const Packets_t& sOCStates_1c9b, const Packets_t& compoundStatements_1c9d);
	void callTransStartTransitions_1dee( const Packets_t& states_b2, const Packets_t& fcns_b4, const Packets_t& css_b6);

private:
	Packets_t* _state_1d60;
};

class ChildTest_1d61
{
public:
	void operator()( const Packets_t& states_1d62, Packets_t& states_1d64);

protected:
	void executeOne( const Packets_t& states_1d62);
	bool isInputUnique( const Udm::Object& state_1d68);

private:
	Packets_t* _state_1d65;
	Packets_t _state_1d66;
};

class HasChildTransConnector_1d6d
{
public:
	bool operator()( const Packets_t& states_1d6e, Packets_t& states_1d70);

protected:
	bool isInputUnique( const Udm::Object& state_1d76);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& states_1d6e);
	bool patternMatcher( const Udm::Object& state_1d74);
	void outputAppender( const CyberComposition::Simulink::State& state_1d84);

private:
	Packets_t* _state_1d71;
	Packets_t _state_1d72;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_1d82;
		CyberComposition::Simulink::TransConnector transConnector_1d83;
	};

	std::list< Match> _matches;
};

class RefetchEnterFcn_1d86
{
public:
	void operator()( const Packets_t& states_1d87, const Packets_t& compoundStatements_1d8b, Packets_t& states_1d89, Packets_t& functions_1d8a, Packets_t& compoundStatements_1d8d);

protected:
	bool isInputUnique( const Udm::Object& state_1d95, const Udm::Object& compoundStatement_1d9e);
	void processInputPackets( const Packets_t& states_1d87, const Packets_t& compoundStatements_1d8b);
	bool patternMatcher( const Udm::Object& state_1d93, const Udm::Object& compoundStatement_1d9c);
	void effector();
	void outputAppender( const CyberComposition::Simulink::State& state_1daa, const SFC::Function& function_1dac, const SFC::CompoundStatement& compoundStatement_1dae);

private:
	Packets_t* _state_1d8e;
	Packets_t* _function_1d8f;
	Packets_t* _compoundStatement_1d90;
	Packets_t _state_1d91;
	Packets_t _compoundStatement_1d9a;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_1da7;
		SFC::CompoundStatement compoundStatement_1da8;
		SFC::Function function_1da9;
	};

	std::list< Match> _matches;
};

class TestMode02_1db0
{
public:
	void operator()( const Packets_t& states_1db1, Packets_t& states_1db3, Packets_t& conditionalBlocks_1db4);

protected:
	bool isInputUnique( const Udm::Object& state_1dbb);
	bool isGuardTrue( SFC::Arg& Arg0, SFC::Function& Enter, CyberComposition::Simulink::State& State);
	void processInputPackets( const Packets_t& states_1db1);
	bool patternMatcher( const Udm::Object& state_1db9);
	void effector();
	void outputAppender( const CyberComposition::Simulink::State& state_1dd6, const SFC::ConditionalBlock& conditionalBlock_1dd8);

private:
	Packets_t* _state_1db5;
	Packets_t* _conditionalBlock_1db6;
	Packets_t _state_1db7;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_1dc7;
		SFC::Arg arg0_1dc8;
		SFC::Function enter_1dc9;
	};

	std::list< Match> _matches;
};

class Mode0or1or2_1df2
{
public:
	void operator()( const Packets_t& states_1df3, Packets_t& states_1df5);

protected:
	void callTestMode012EnterAction_1e31( const Packets_t& states_1df8);

private:
	Packets_t* _state_1df6;
};

class TestMode012EnterAction_1df7
{
public:
	void operator()( const Packets_t& states_1df8);

protected:
	bool isInputUnique( const Udm::Object& state_1dfe);
	bool isGuardTrue( SFC::Arg& Arg0, SFC::Function& Enter, CyberComposition::Simulink::State& ParentState, SFC::StateLabel& ParentStateLabel, SFC::Program& Program, CyberComposition::Simulink::State& State, SFC::StateLabel& StateLabel, SFC::StateVar& StateVar);
	void processInputPackets( const Packets_t& states_1df8);
	bool patternMatcher( const Udm::Object& state_1dfc);
	void effector();

private:
	Packets_t _state_1dfa;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_1e17;
		CyberComposition::Simulink::State parentState_1e18;
		SFC::StateLabel parentStateLabel_1e19;
		SFC::StateLabel stateLabel_1e1a;
		SFC::Arg arg0_1e1b;
		SFC::Function enter_1e1c;
		SFC::Program program_1e1d;
		SFC::StateVar stateVar_1e1e;
	};

	std::list< Match> _matches;
};

class Mode0or1_1e33
{
public:
	void operator()( const Packets_t& states_1e34, Packets_t& states_1e36);

protected:
	void callTestMode01CallParent_1eb0( const Packets_t& states_1e6a);
	void callIsAndState_1eb2( const Packets_t& states_1c5e, const Packets_t& css_1c60);
	void callGetHOSibs_1eb5( const Packets_t& states_1e39, const Packets_t& compoundStatements_1e3d);
	void callCallSibsOrChildren_1eb8( const Packets_t& states_1c99, const Packets_t& sOCStates_1c9b, const Packets_t& compoundStatements_1c9d);

private:
	Packets_t* _state_1e37;
};

class GetHOSibs_1e38
{
public:
	void operator()( const Packets_t& states_1e39, const Packets_t& compoundStatements_1e3d, Packets_t& states_1e3b, Packets_t& sibStates_1e3c, Packets_t& compoundStatements_1e3f);

protected:
	bool isInputUnique( const Udm::Object& state_1e47, const Udm::Object& compoundStatement_1e50);
	bool isGuardTrue( SFC::CompoundStatement& CompoundStatement, CyberComposition::Simulink::State& ParentState, CyberComposition::Simulink::State& SibState, CyberComposition::Simulink::State& State);
	void processInputPackets( const Packets_t& states_1e39, const Packets_t& compoundStatements_1e3d);
	bool patternMatcher( const Udm::Object& state_1e45, const Udm::Object& compoundStatement_1e4e);
	void effector();
	void outputAppender( const CyberComposition::Simulink::State& state_1e63, const CyberComposition::Simulink::State& sibState_1e65, const SFC::CompoundStatement& compoundStatement_1e67);
	void sortOutputs();

private:
	Packets_t* _state_1e40;
	Packets_t* _sibState_1e41;
	Packets_t* _compoundStatement_1e42;
	Packets_t _state_1e43;
	Packets_t _compoundStatement_1e4c;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_1e5b;
		SFC::CompoundStatement compoundStatement_1e5c;
		CyberComposition::Simulink::State parentState_1e5d;
		CyberComposition::Simulink::State sibState_1e5e;
	};

	std::list< Match> _matches;
};

class TestMode01CallParent_1e69
{
public:
	void operator()( const Packets_t& states_1e6a, Packets_t& states_1e6c, Packets_t& conditionalBlocks_1e6d);

protected:
	bool isInputUnique( const Udm::Object& state_1e74);
	bool isGuardTrue( SFC::Arg& Arg0, SFC::Arg& Arg1, SFC::Function& Enter, SFC::Arg& ParentArg0, SFC::Arg& ParentArg1, SFC::Function& ParentEnter, CyberComposition::Simulink::State& ParentState, CyberComposition::Simulink::State& State);
	void processInputPackets( const Packets_t& states_1e6a);
	bool patternMatcher( const Udm::Object& state_1e72);
	void effector();
	void outputAppender( const CyberComposition::Simulink::State& state_1eac, const SFC::ConditionalBlock& conditionalBlock_1eae);

private:
	Packets_t* _state_1e6e;
	Packets_t* _conditionalBlock_1e6f;
	Packets_t _state_1e70;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_1e8e;
		CyberComposition::Simulink::State parentState_1e8f;
		SFC::Arg parentArg0_1e90;
		SFC::Function parentEnter_1e91;
		SFC::Arg arg0_1e92;
		SFC::Arg arg1_1e93;
		SFC::Arg parentArg1_1e94;
		SFC::Function enter_1e95;
	};

	std::list< Match> _matches;
};

class ReturnAtTopState_1ebc
{
public:
	void operator()( const Packets_t& states_1ebd, Packets_t& states_1ebf);

protected:
	void callTopState_1ee8( const Packets_t& states_1ec2);

private:
	Packets_t* _state_1ec0;
};

class TopState_1ec1
{
public:
	void operator()( const Packets_t& states_1ec2);

protected:
	bool isInputUnique( const Udm::Object& state_1ec8);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::Arg& Arg, SFC::Function& Enter, CyberComposition::Simulink::State& State, SFC::StateLabel& StateLabel);
	void processInputPackets( const Packets_t& states_1ec2);
	bool patternMatcher( const Udm::Object& state_1ec6);
	void effector();

private:
	Packets_t _state_1ec4;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_1eda;
		SFC::StateLabel stateLabel_1edb;
		SFC::Function enter_1edc;
		SFC::Arg arg_1edd;
	};

	std::list< Match> _matches;
};

class TestEAIOff_1eea
{
public:
	void operator()( const Packets_t& states_1eeb, Packets_t& states_1eed, Packets_t& states_1eee);

protected:
	void executeOne( const Packets_t& states_1eeb);
	bool isInputUnique( const Udm::Object& state_1ef3);

private:
	Packets_t* _state_1eef;
	Packets_t* _state_1ef0;
	Packets_t _state_1ef1;
};

class EAIOff_1ef8
{
public:
	bool operator()( const Packets_t& states_1ef9, Packets_t& states_1efb);

protected:
	bool isInputUnique( const Udm::Object& state_1f01);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( CyberComposition::Simulink::ConnectorRef& ConnectorRef, CyberComposition::Simulink::SF_Parameter& Parameter, CyberComposition::Simulink::Primitive& Primitive, CyberComposition::Simulink::State& State, CyberComposition::Simulink::Subsystem& Subsystem);
	void processInputPackets( const Packets_t& states_1ef9);
	bool patternMatcher( const Udm::Object& state_1eff);
	void outputAppender( const CyberComposition::Simulink::State& state_1f1e);

private:
	Packets_t* _state_1efc;
	Packets_t _state_1efd;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_1f14;
		CyberComposition::Simulink::ConnectorRef connectorRef_1f15;
		CyberComposition::Simulink::Primitive primitive_1f16;
		CyberComposition::Simulink::SF_Parameter parameter_1f17;
		CyberComposition::Simulink::Subsystem subsystem_1f18;
	};

	std::list< Match> _matches;
};

class Otherwise_1f20
{
public:
	bool operator()( const Packets_t& states_1f21, Packets_t& states_1f23);

protected:
	bool isInputUnique( const Udm::Object& state_1f29);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& states_1f21);
	bool patternMatcher( const Udm::Object& state_1f27);
	void outputAppender( const CyberComposition::Simulink::State& state_1f33);

private:
	Packets_t* _state_1f24;
	Packets_t _state_1f25;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_1f32;
	};

	std::list< Match> _matches;
};

class CreateRootFunction_1f53
{
public:
	void operator()( const Packets_t& states_1f54, const Packets_t& programs_1f56, Packets_t& states_1f58, Packets_t& programs_1f59);

protected:
	void callCreateRootFunction_21ed( const Packets_t& states_1f83, const Packets_t& programs_1f86);
	void callCreateInputArgs_21f0( const Packets_t& states_1fa9, const Packets_t& rootFxns_1fab);
	void callCreateRootExecCall_21f3( const Packets_t& states_1f5d, const Packets_t& rootFunctions_1f60);
	void callCreateOutputArgs_21f6( const Packets_t& states_2119, const Packets_t& rootFxns_211b);

private:
	Packets_t* _state_1f5a;
	Packets_t* _program_1f5b;
};

class CreateRootExecCall_1f5c
{
public:
	void operator()( const Packets_t& states_1f5d, const Packets_t& rootFunctions_1f60, Packets_t& states_1f5f, Packets_t& rootFunctions_1f62);

protected:
	bool isInputUnique( const Udm::Object& state_1f69, const Udm::Object& rootFunction_1f72);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& states_1f5d, const Packets_t& rootFunctions_1f60);
	bool patternMatcher( const Udm::Object& state_1f67, const Udm::Object& rootFunction_1f70);
	void effector();
	void forwardInputs();

private:
	Packets_t* _state_1f63;
	Packets_t* _rootFunction_1f64;
	Packets_t _state_1f65;
	Packets_t _rootFunction_1f6e;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_1f7e;
		SFC::Function rootFunction_1f7f;
		SFC::Function exec_1f80;
	};

	std::list< Match> _matches;
};

class CreateRootFunction_1f82
{
public:
	void operator()( const Packets_t& states_1f83, const Packets_t& programs_1f86, Packets_t& states_1f85, Packets_t& rootFunctions_1f88);

protected:
	bool isInputUnique( const Udm::Object& state_1f8f, const Udm::Object& program_1f98);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& states_1f83, const Packets_t& programs_1f86);
	bool patternMatcher( const Udm::Object& state_1f8d, const Udm::Object& program_1f96);
	void effector();
	void outputAppender( const CyberComposition::Simulink::State& state_1fa4, const SFC::Function& rootFunction_1fa6);

private:
	Packets_t* _state_1f89;
	Packets_t* _rootFunction_1f8a;
	Packets_t _state_1f8b;
	Packets_t _program_1f94;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_1fa1;
		SFC::Program program_1fa2;
	};

	std::list< Match> _matches;
};

class CreateInputArgs_1fa8
{
public:
	void operator()( const Packets_t& states_1fa9, const Packets_t& rootFxns_1fab, Packets_t& states_1fad, Packets_t& rootFxns_1fae);

protected:
	void callCheckStateDecomp_2109( const Packets_t& states_20a3, const Packets_t& programs_20a5);
	void callGetSubStates_210c( const Packets_t& states_c8c, const Packets_t& parStmnts_c8f);
	void callCreateIDArgs_210f( const Packets_t& states_1fe3, const Packets_t& rootFxns_1fe5);
	void callCreateIEArgs_2112( const Packets_t& states_2043, const Packets_t& rootFxns_2045);
	void callCreateLEInit_2115( const Packets_t& states_1fb2, const Packets_t& rootFunctions_1fb5);

private:
	Packets_t* _state_1faf;
	Packets_t* _rootFxn_1fb0;
};

class CreateLEInit_1fb1
{
public:
	void operator()( const Packets_t& states_1fb2, const Packets_t& rootFunctions_1fb5, Packets_t& states_1fb4, Packets_t& rootFunctions_1fb7);

protected:
	bool isInputUnique( const Udm::Object& state_1fbe, const Udm::Object& rootFunction_1fc7);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( CyberComposition::Simulink::Event& Event, SFC::LocalVar& LocalVar, SFC::Function& RootFunction, CyberComposition::Simulink::State& State);
	void processInputPackets( const Packets_t& states_1fb2, const Packets_t& rootFunctions_1fb5);
	bool patternMatcher( const Udm::Object& state_1fbc, const Udm::Object& rootFunction_1fc5);
	void effector();
	void forwardInputs();

private:
	Packets_t* _state_1fb8;
	Packets_t* _rootFunction_1fb9;
	Packets_t _state_1fba;
	Packets_t _rootFunction_1fc3;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_1fd6;
		SFC::Function rootFunction_1fd7;
		CyberComposition::Simulink::Event event_1fd8;
		SFC::LocalVar localVar_1fd9;
	};

	std::list< Match> _matches;
};

class CreateIDArgs_1fe2
{
public:
	void operator()( const Packets_t& states_1fe3, const Packets_t& rootFxns_1fe5, Packets_t& states_1fe7, Packets_t& rootFxns_1fe8);

protected:
	void callGetInData_203c( const Packets_t& states_2012, const Packets_t& rootFunctions_2014);
	void callCreateIDArg_203f( const Packets_t& datas_1fec, const Packets_t& rootFunctions_1fee);

private:
	Packets_t* _state_1fe9;
	Packets_t* _rootFxn_1fea;
};

class CreateIDArg_1feb
{
public:
	void operator()( const Packets_t& datas_1fec, const Packets_t& rootFunctions_1fee);

protected:
	bool isInputUnique( const Udm::Object& data_1ff4, const Udm::Object& rootFunction_1ffd);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& datas_1fec, const Packets_t& rootFunctions_1fee);
	bool patternMatcher( const Udm::Object& data_1ff2, const Udm::Object& rootFunction_1ffb);
	void effector();

private:
	Packets_t _data_1ff0;
	Packets_t _rootFunction_1ff9;
	class Match
	{
	public:
		CyberComposition::Simulink::Data data_200a;
		SFC::Function rootFunction_200b;
		SFC::LocalVar localVar_200c;
		SFC::DT dT_200d;
	};

	std::list< Match> _matches;
};

class GetInData_2010
{
public:
	void operator()( const Packets_t& states_2012, const Packets_t& rootFunctions_2014, Packets_t& datas_2011, Packets_t& rootFunctions_2016);

protected:
	bool isInputUnique( const Udm::Object& state_201d, const Udm::Object& rootFunction_2026);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( CyberComposition::Simulink::Data& Data, SFC::Function& RootFunction, CyberComposition::Simulink::State& State);
	void processInputPackets( const Packets_t& states_2012, const Packets_t& rootFunctions_2014);
	bool patternMatcher( const Udm::Object& state_201b, const Udm::Object& rootFunction_2024);
	void effector();
	void outputAppender( const CyberComposition::Simulink::Data& data_2038, const SFC::Function& rootFunction_203a);
	void sortOutputs();

private:
	Packets_t* _data_2017;
	Packets_t* _rootFunction_2018;
	Packets_t _state_2019;
	Packets_t _rootFunction_2022;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_2032;
		SFC::Function rootFunction_2033;
		CyberComposition::Simulink::Data data_2034;
	};

	std::list< Match> _matches;
};

class CreateIEArgs_2042
{
public:
	void operator()( const Packets_t& states_2043, const Packets_t& rootFxns_2045, Packets_t& states_2047, Packets_t& rootFxns_2048);

protected:
	void callGetInEvent_209c( const Packets_t& states_2071, const Packets_t& rootFunctions_2074);
	void callCreateIEArg_209f( const Packets_t& events_204c, const Packets_t& rootFunctions_204e);

private:
	Packets_t* _state_2049;
	Packets_t* _rootFxn_204a;
};

class CreateIEArg_204b
{
public:
	void operator()( const Packets_t& events_204c, const Packets_t& rootFunctions_204e);

protected:
	bool isInputUnique( const Udm::Object& event_2054, const Udm::Object& rootFunction_205d);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& events_204c, const Packets_t& rootFunctions_204e);
	bool patternMatcher( const Udm::Object& event_2052, const Udm::Object& rootFunction_205b);
	void effector();

private:
	Packets_t _event_2050;
	Packets_t _rootFunction_2059;
	class Match
	{
	public:
		CyberComposition::Simulink::Event event_206a;
		SFC::Function rootFunction_206b;
		SFC::LocalVar localVar_206c;
		SFC::DT dT_206d;
	};

	std::list< Match> _matches;
};

class GetInEvent_2070
{
public:
	void operator()( const Packets_t& states_2071, const Packets_t& rootFunctions_2074, Packets_t& events_2073, Packets_t& rootFunctions_2076);

protected:
	bool isInputUnique( const Udm::Object& state_207d, const Udm::Object& rootFunction_2086);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( CyberComposition::Simulink::Event& Event, SFC::Function& RootFunction, CyberComposition::Simulink::State& State);
	void processInputPackets( const Packets_t& states_2071, const Packets_t& rootFunctions_2074);
	bool patternMatcher( const Udm::Object& state_207b, const Udm::Object& rootFunction_2084);
	void effector();
	void outputAppender( const CyberComposition::Simulink::Event& event_2098, const SFC::Function& rootFunction_209a);
	void sortOutputs();

private:
	Packets_t* _event_2077;
	Packets_t* _rootFunction_2078;
	Packets_t _state_2079;
	Packets_t _rootFunction_2082;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_2092;
		SFC::Function rootFunction_2093;
		CyberComposition::Simulink::Event event_2094;
	};

	std::list< Match> _matches;
};

class CheckStateDecomp_20a2
{
public:
	void operator()( const Packets_t& states_20a3, const Packets_t& programs_20a5, Packets_t& states_20a7, Packets_t& programs_20a8, Packets_t& states_20a9, Packets_t& programs_20aa);

protected:
	void executeOne( const Packets_t& states_20a3, const Packets_t& programs_20a5);
	bool isInputUnique( const Udm::Object& state_20b1, const Udm::Object& program_20b8);

private:
	Packets_t* _state_20ab;
	Packets_t* _program_20ac;
	Packets_t* _state_20ad;
	Packets_t* _program_20ae;
	Packets_t _state_20af;
	Packets_t _program_20b6;
};

class IsFuncState_20bd
{
public:
	bool operator()( const Packets_t& states_20be, const Packets_t& programs_20c1, Packets_t& states_20c0, Packets_t& programs_20c3);

protected:
	bool isInputUnique( const Udm::Object& state_20ca, const Udm::Object& program_20d3);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::CompoundStatement& Program, CyberComposition::Simulink::State& State);
	void processInputPackets( const Packets_t& states_20be, const Packets_t& programs_20c1);
	bool patternMatcher( const Udm::Object& state_20c8, const Udm::Object& program_20d1);
	void outputAppender( const CyberComposition::Simulink::State& state_20e0, const SFC::CompoundStatement& program_20e2);

private:
	Packets_t* _state_20c4;
	Packets_t* _program_20c5;
	Packets_t _state_20c6;
	Packets_t _program_20cf;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_20dc;
		SFC::CompoundStatement program_20dd;
	};

	std::list< Match> _matches;
};

class Otherwise_20e4
{
public:
	bool operator()( const Packets_t& states_20e5, const Packets_t& programs_20e8, Packets_t& states_20e7, Packets_t& programs_20ea);

protected:
	bool isInputUnique( const Udm::Object& state_20f1, const Udm::Object& program_20fa);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& states_20e5, const Packets_t& programs_20e8);
	bool patternMatcher( const Udm::Object& state_20ef, const Udm::Object& program_20f8);
	void outputAppender( const CyberComposition::Simulink::State& state_2105, const SFC::CompoundStatement& program_2107);

private:
	Packets_t* _state_20eb;
	Packets_t* _program_20ec;
	Packets_t _state_20ed;
	Packets_t _program_20f6;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_2103;
		SFC::CompoundStatement program_2104;
	};

	std::list< Match> _matches;
};

class CreateOutputArgs_2118
{
public:
	void operator()( const Packets_t& states_2119, const Packets_t& rootFxns_211b, Packets_t& states_211d, Packets_t& rootFxns_211e);

protected:
	void callCheckStateDecomp_21e1( const Packets_t& states_20a3, const Packets_t& programs_20a5);
	void callGetSubStates_21e4( const Packets_t& states_c8c, const Packets_t& parStmnts_c8f);
	void callCreateODArgs_21e7( const Packets_t& states_2122, const Packets_t& rootFxns_2124);
	void callCreateOEArgs_21ea( const Packets_t& states_2182, const Packets_t& rootFxns_2184);

private:
	Packets_t* _state_211f;
	Packets_t* _rootFxn_2120;
};

class CreateODArgs_2121
{
public:
	void operator()( const Packets_t& states_2122, const Packets_t& rootFxns_2124, Packets_t& states_2126, Packets_t& rootFxns_2127);

protected:
	void callGetOutData_217b( const Packets_t& states_212c, const Packets_t& rootFunctions_212e);
	void callCreateODArg_217e( const Packets_t& datas_2157, const Packets_t& rootFunctions_2159);

private:
	Packets_t* _state_2128;
	Packets_t* _rootFxn_2129;
};

class GetOutData_212a
{
public:
	void operator()( const Packets_t& states_212c, const Packets_t& rootFunctions_212e, Packets_t& datas_212b, Packets_t& rootFunctions_2130);

protected:
	bool isInputUnique( const Udm::Object& state_2137, const Udm::Object& rootFunction_2140);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( CyberComposition::Simulink::Data& Data, SFC::Function& RootFunction, CyberComposition::Simulink::State& State);
	void processInputPackets( const Packets_t& states_212c, const Packets_t& rootFunctions_212e);
	bool patternMatcher( const Udm::Object& state_2135, const Udm::Object& rootFunction_213e);
	void effector();
	void outputAppender( const CyberComposition::Simulink::Data& data_2152, const SFC::Function& rootFunction_2154);
	void sortOutputs();

private:
	Packets_t* _data_2131;
	Packets_t* _rootFunction_2132;
	Packets_t _state_2133;
	Packets_t _rootFunction_213c;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_214c;
		SFC::Function rootFunction_214d;
		CyberComposition::Simulink::Data data_214e;
	};

	std::list< Match> _matches;
};

class CreateODArg_2156
{
public:
	void operator()( const Packets_t& datas_2157, const Packets_t& rootFunctions_2159);

protected:
	bool isInputUnique( const Udm::Object& data_215f, const Udm::Object& rootFunction_2168);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& datas_2157, const Packets_t& rootFunctions_2159);
	bool patternMatcher( const Udm::Object& data_215d, const Udm::Object& rootFunction_2166);
	void effector();

private:
	Packets_t _data_215b;
	Packets_t _rootFunction_2164;
	class Match
	{
	public:
		CyberComposition::Simulink::Data data_2175;
		SFC::Function rootFunction_2176;
		SFC::LocalVar localVar_2177;
		SFC::DT dT_2178;
	};

	std::list< Match> _matches;
};

class CreateOEArgs_2181
{
public:
	void operator()( const Packets_t& states_2182, const Packets_t& rootFxns_2184, Packets_t& states_2186, Packets_t& rootFxns_2187);

protected:
	void callGetOutEvent_21db( const Packets_t& states_218b, const Packets_t& rootFunctions_218e);
	void callCreateOEArg_21de( const Packets_t& events_21b7, const Packets_t& rootFunctions_21b9);

private:
	Packets_t* _state_2188;
	Packets_t* _rootFxn_2189;
};

class GetOutEvent_218a
{
public:
	void operator()( const Packets_t& states_218b, const Packets_t& rootFunctions_218e, Packets_t& events_218d, Packets_t& rootFunctions_2190);

protected:
	bool isInputUnique( const Udm::Object& state_2197, const Udm::Object& rootFunction_21a0);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( CyberComposition::Simulink::Event& Event, SFC::Function& RootFunction, CyberComposition::Simulink::State& State);
	void processInputPackets( const Packets_t& states_218b, const Packets_t& rootFunctions_218e);
	bool patternMatcher( const Udm::Object& state_2195, const Udm::Object& rootFunction_219e);
	void effector();
	void outputAppender( const CyberComposition::Simulink::Event& event_21b2, const SFC::Function& rootFunction_21b4);
	void sortOutputs();

private:
	Packets_t* _event_2191;
	Packets_t* _rootFunction_2192;
	Packets_t _state_2193;
	Packets_t _rootFunction_219c;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_21ac;
		SFC::Function rootFunction_21ad;
		CyberComposition::Simulink::Event event_21ae;
	};

	std::list< Match> _matches;
};

class CreateOEArg_21b6
{
public:
	void operator()( const Packets_t& events_21b7, const Packets_t& rootFunctions_21b9);

protected:
	bool isInputUnique( const Udm::Object& event_21bf, const Udm::Object& rootFunction_21c8);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& events_21b7, const Packets_t& rootFunctions_21b9);
	bool patternMatcher( const Udm::Object& event_21bd, const Udm::Object& rootFunction_21c6);
	void effector();

private:
	Packets_t _event_21bb;
	Packets_t _rootFunction_21c4;
	class Match
	{
	public:
		CyberComposition::Simulink::Event event_21d5;
		SFC::Function rootFunction_21d6;
		SFC::LocalVar localVar_21d7;
		SFC::DT dT_21d8;
	};

	std::list< Match> _matches;
};

class CreateInitFunction_21f9
{
public:
	void operator()( const Packets_t& states_21fa, const Packets_t& programs_21fc, Packets_t& states_21fe, Packets_t& programs_21ff);

protected:
	void callCreateInitFunction_242b( const Packets_t& states_225a, const Packets_t& programs_225d);
	void callInitLocalVars_242e( const Packets_t& states_228d, const Packets_t& programs_228f, const Packets_t& initFns_2291, const Packets_t& initIdxs_2293);
	void callTestEAIOn_2433( const Packets_t& states_23dc, const Packets_t& initfns_23de);
	void callCreateFunction_FunctionCall_2436( const Packets_t& states_2203, const Packets_t& initFns_2206);
	void callFunctionCall_Vals_2439( const Packets_t& enters_222d, const Packets_t& functionCalls_222f);

private:
	Packets_t* _state_2200;
	Packets_t* _program_2201;
};

class CreateFunction_FunctionCall_2202
{
public:
	void operator()( const Packets_t& states_2203, const Packets_t& initFns_2206, Packets_t& enters_2205, Packets_t& functionCalls_2208);

protected:
	bool isInputUnique( const Udm::Object& state_220f, const Udm::Object& initFn_2218);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& states_2203, const Packets_t& initFns_2206);
	bool patternMatcher( const Udm::Object& state_220d, const Udm::Object& initFn_2216);
	void effector();
	void outputAppender( const SFC::Function& enter_2228, const SFC::FunctionCall& functionCall_222a);

private:
	Packets_t* _enter_2209;
	Packets_t* _functionCall_220a;
	Packets_t _state_220b;
	Packets_t _initFn_2214;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_2224;
		SFC::Function initFn_2225;
		SFC::Function enter_2226;
	};

	std::list< Match> _matches;
};

class FunctionCall_Vals_222c
{
public:
	void operator()( const Packets_t& enters_222d, const Packets_t& functionCalls_222f);

protected:
	bool isInputUnique( const Udm::Object& enter_2235, const Udm::Object& functionCall_223e);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::Arg& Arg0, SFC::Arg& Arg1, SFC::Function& Enter, SFC::FunctionCall& FunctionCall);
	void processInputPackets( const Packets_t& enters_222d, const Packets_t& functionCalls_222f);
	bool patternMatcher( const Udm::Object& enter_2233, const Udm::Object& functionCall_223c);
	void effector();

private:
	Packets_t _enter_2231;
	Packets_t _functionCall_223a;
	class Match
	{
	public:
		SFC::Function enter_224d;
		SFC::FunctionCall functionCall_224e;
		SFC::Arg arg0_224f;
		SFC::Arg arg1_2250;
	};

	std::list< Match> _matches;
};

class CreateInitFunction_2259
{
public:
	void operator()( const Packets_t& states_225a, const Packets_t& programs_225d, Packets_t& states_225c, Packets_t& programs_225f, Packets_t& initFunctions_2260, Packets_t& initIdxs_2261);

protected:
	bool isInputUnique( const Udm::Object& state_226a, const Udm::Object& program_2273);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& states_225a, const Packets_t& programs_225d);
	bool patternMatcher( const Udm::Object& state_2268, const Udm::Object& program_2271);
	void effector();
	void outputAppender( const CyberComposition::Simulink::State& state_2284, const SFC::Program& program_2286, const SFC::Function& initFunction_2288, const SFC::LocalVar& initIdx_228a);

private:
	Packets_t* _state_2262;
	Packets_t* _program_2263;
	Packets_t* _initFunction_2264;
	Packets_t* _initIdx_2265;
	Packets_t _state_2266;
	Packets_t _program_226f;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_227f;
		SFC::Program program_2280;
		SFC::StateVar stateVar_2281;
	};

	std::list< Match> _matches;
};

class InitLocalVars_228c
{
public:
	void operator()( const Packets_t& states_228d, const Packets_t& programs_228f, const Packets_t& initFns_2291, const Packets_t& initIdxs_2293, Packets_t& states_2295, Packets_t& initfns_2296);

protected:
	void callGetSortedVars_23cb( const Packets_t& programs_229a, const Packets_t& initFns_229d, const Packets_t& initIdxs_22a0);
	void callArrayVars_23cf( const Packets_t& programs_230e, const Packets_t& initFns_2310, const Packets_t& initIdxs_2312);
	void callInitArrayVar_23d3( const Packets_t& vars_22d3, const Packets_t& initFns_22d5, const Packets_t& idxVars_22d7);
	void callInitScalarVar_23d7( const Packets_t& vars_23a3, const Packets_t& initFns_23a5, const Packets_t& idxVars_23a7);

private:
	Packets_t* _state_2297;
	Packets_t* _initfn_2298;
};

class GetSortedVars_2299
{
public:
	void operator()( const Packets_t& programs_229a, const Packets_t& initFns_229d, const Packets_t& initIdxs_22a0, Packets_t& vars_229c, Packets_t& initFns_229f, Packets_t& initIdxs_22a2);

protected:
	bool isInputUnique( const Udm::Object& program_22aa, const Udm::Object& initFn_22b3, const Udm::Object& initIdx_22bc);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& programs_229a, const Packets_t& initFns_229d, const Packets_t& initIdxs_22a0);
	bool patternMatcher( const Udm::Object& program_22a8, const Udm::Object& initFn_22b1, const Udm::Object& initIdx_22ba);
	void effector();
	void outputAppender( const SFC::Var& var_22cc, const SFC::Function& initFn_22ce, const SFC::LocalVar& initIdx_22d0);
	void sortOutputs();

private:
	Packets_t* _var_22a3;
	Packets_t* _initFn_22a4;
	Packets_t* _initIdx_22a5;
	Packets_t _program_22a6;
	Packets_t _initFn_22af;
	Packets_t _initIdx_22b8;
	class Match
	{
	public:
		SFC::Program program_22c8;
		SFC::Function initFn_22c9;
		SFC::LocalVar initIdx_22ca;
		SFC::Var var_22cb;
	};

	std::list< Match> _matches;
};

class InitArrayVar_22d2
{
public:
	void operator()( const Packets_t& vars_22d3, const Packets_t& initFns_22d5, const Packets_t& idxVars_22d7);

protected:
	bool isInputUnique( const Udm::Object& var_22dd, const Udm::Object& initFn_22e6, const Udm::Object& idxVar_22ef);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& vars_22d3, const Packets_t& initFns_22d5, const Packets_t& idxVars_22d7);
	bool patternMatcher( const Udm::Object& var_22db, const Udm::Object& initFn_22e4, const Udm::Object& idxVar_22ed);
	void effector();

private:
	Packets_t _var_22d9;
	Packets_t _initFn_22e2;
	Packets_t _idxVar_22eb;
	class Match
	{
	public:
		SFC::Var var_22f8;
		SFC::Function initFn_22f9;
		SFC::LocalVar idxVar_22fa;
	};

	std::list< Match> _matches;
};

class ArrayVars_230d
{
public:
	void operator()( const Packets_t& programs_230e, const Packets_t& initFns_2310, const Packets_t& initIdxs_2312, Packets_t& vars_2314, Packets_t& initfns_2315, Packets_t& initidxs_2316, Packets_t& vars_2317, Packets_t& initfns_2318, Packets_t& initidxs_2319);

protected:
	void executeOne( const Packets_t& programs_230e, const Packets_t& initFns_2310, const Packets_t& initIdxs_2312);
	bool isInputUnique( const Udm::Object& program_2322, const Udm::Object& initFn_2329, const Udm::Object& initIdx_2330);

private:
	Packets_t* _var_231a;
	Packets_t* _initfn_231b;
	Packets_t* _initidx_231c;
	Packets_t* _var_231d;
	Packets_t* _initfn_231e;
	Packets_t* _initidx_231f;
	Packets_t _program_2320;
	Packets_t _initFn_2327;
	Packets_t _initIdx_232e;
};

class ArrayVar_2335
{
public:
	bool operator()( const Packets_t& vars_2336, const Packets_t& initFns_2339, const Packets_t& initIdxs_233c, Packets_t& vars_2338, Packets_t& initFns_233b, Packets_t& initIdxs_233e);

protected:
	bool isInputUnique( const Udm::Object& var_2346, const Udm::Object& initFn_234f, const Udm::Object& initIdx_2358);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::Function& InitFn, SFC::LocalVar& InitIdx, SFC::Var& Var);
	void processInputPackets( const Packets_t& vars_2336, const Packets_t& initFns_2339, const Packets_t& initIdxs_233c);
	bool patternMatcher( const Udm::Object& var_2344, const Udm::Object& initFn_234d, const Udm::Object& initIdx_2356);
	void outputAppender( const SFC::Var& var_2367, const SFC::Function& initFn_2369, const SFC::LocalVar& initIdx_236b);
	void sortOutputs();

private:
	Packets_t* _var_233f;
	Packets_t* _initFn_2340;
	Packets_t* _initIdx_2341;
	Packets_t _var_2342;
	Packets_t _initFn_234b;
	Packets_t _initIdx_2354;
	class Match
	{
	public:
		SFC::Var var_2361;
		SFC::Function initFn_2362;
		SFC::LocalVar initIdx_2363;
	};

	std::list< Match> _matches;
};

class Otherwise_236d
{
public:
	bool operator()( const Packets_t& vars_236e, const Packets_t& initFns_2371, const Packets_t& initIdxs_2374, Packets_t& vars_2370, Packets_t& initFns_2373, Packets_t& initIdxs_2376);

protected:
	bool isInputUnique( const Udm::Object& var_237e, const Udm::Object& initFn_2387, const Udm::Object& initIdx_2390);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& vars_236e, const Packets_t& initFns_2371, const Packets_t& initIdxs_2374);
	bool patternMatcher( const Udm::Object& var_237c, const Udm::Object& initFn_2385, const Udm::Object& initIdx_238e);
	void outputAppender( const SFC::Var& var_239c, const SFC::Function& initFn_239e, const SFC::LocalVar& initIdx_23a0);
	void sortOutputs();

private:
	Packets_t* _var_2377;
	Packets_t* _initFn_2378;
	Packets_t* _initIdx_2379;
	Packets_t _var_237a;
	Packets_t _initFn_2383;
	Packets_t _initIdx_238c;
	class Match
	{
	public:
		SFC::Var var_2399;
		SFC::Function initFn_239a;
		SFC::LocalVar initIdx_239b;
	};

	std::list< Match> _matches;
};

class InitScalarVar_23a2
{
public:
	void operator()( const Packets_t& vars_23a3, const Packets_t& initFns_23a5, const Packets_t& idxVars_23a7);

protected:
	bool isInputUnique( const Udm::Object& var_23ad, const Udm::Object& initFn_23b6, const Udm::Object& idxVar_23bf);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& vars_23a3, const Packets_t& initFns_23a5, const Packets_t& idxVars_23a7);
	bool patternMatcher( const Udm::Object& var_23ab, const Udm::Object& initFn_23b4, const Udm::Object& idxVar_23bd);
	void effector();

private:
	Packets_t _var_23a9;
	Packets_t _initFn_23b2;
	Packets_t _idxVar_23bb;
	class Match
	{
	public:
		SFC::Var var_23c8;
		SFC::Function initFn_23c9;
		SFC::LocalVar idxVar_23ca;
	};

	std::list< Match> _matches;
};

class TestEAIOn_23db
{
public:
	void operator()( const Packets_t& states_23dc, const Packets_t& initfns_23de, Packets_t& states_23e0, Packets_t& initfns_23e1);

protected:
	void executeOne( const Packets_t& states_23dc, const Packets_t& initfns_23de);
	bool isInputUnique( const Udm::Object& state_23e6, const Udm::Object& initfn_23ed);

private:
	Packets_t* _state_23e2;
	Packets_t* _initfn_23e3;
	Packets_t _state_23e4;
	Packets_t _initfn_23eb;
};

class EAIOn_23f2
{
public:
	bool operator()( const Packets_t& states_23f3, const Packets_t& initFns_23f6, Packets_t& states_23f5, Packets_t& initFns_23f8);

protected:
	bool isInputUnique( const Udm::Object& state_23ff, const Udm::Object& initFn_2408);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( CyberComposition::Simulink::ConnectorRef& ConnectorRef, SFC::Function& InitFn, CyberComposition::Simulink::SF_Parameter& Parameter, CyberComposition::Simulink::Primitive& Primitive, CyberComposition::Simulink::State& State, CyberComposition::Simulink::Subsystem& Subsystem);
	void processInputPackets( const Packets_t& states_23f3, const Packets_t& initFns_23f6);
	bool patternMatcher( const Udm::Object& state_23fd, const Udm::Object& initFn_2406);
	void outputAppender( const CyberComposition::Simulink::State& state_2427, const SFC::Function& initFn_2429);

private:
	Packets_t* _state_23f9;
	Packets_t* _initFn_23fa;
	Packets_t _state_23fb;
	Packets_t _initFn_2404;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_241b;
		SFC::Function initFn_241c;
		CyberComposition::Simulink::ConnectorRef connectorRef_241d;
		CyberComposition::Simulink::Primitive primitive_241e;
		CyberComposition::Simulink::SF_Parameter parameter_241f;
		CyberComposition::Simulink::Subsystem subsystem_2420;
	};

	std::list< Match> _matches;
};

class MarkLegacy_243c
{
public:
	void operator()( const Packets_t& states_243d, const Packets_t& programs_243f);

protected:
	bool isInputUnique( const Udm::Object& state_2445, const Udm::Object& program_244e);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& states_243d, const Packets_t& programs_243f);
	bool patternMatcher( const Udm::Object& state_2443, const Udm::Object& program_244c);
	void effector();

private:
	Packets_t _state_2441;
	Packets_t _program_244a;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_2457;
		SFC::Program program_2458;
	};

	std::list< Match> _matches;
};

class CreateFunctions_2459
{
public:
	void operator()( const Packets_t& states_245a, const Packets_t& programs_245c, Packets_t& states_245e, Packets_t& programs_245f);

protected:
	void callCreateFunctions_2495( const Packets_t& states_2463, const Packets_t& programs_2466);
	void callGetSubStates_2498( const Packets_t& states_c8c, const Packets_t& parStmnts_c8f);

private:
	Packets_t* _state_2460;
	Packets_t* _program_2461;
};

class CreateFunctions_2462
{
public:
	void operator()( const Packets_t& states_2463, const Packets_t& programs_2466, Packets_t& states_2465, Packets_t& programs_2468);

protected:
	bool isInputUnique( const Udm::Object& state_246f, const Udm::Object& program_2478);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::Program& Program, CyberComposition::Simulink::State& State, SFC::StateLabel& StateLabel);
	void processInputPackets( const Packets_t& states_2463, const Packets_t& programs_2466);
	bool patternMatcher( const Udm::Object& state_246d, const Udm::Object& program_2476);
	void effector();
	void forwardInputs();

private:
	Packets_t* _state_2469;
	Packets_t* _program_246a;
	Packets_t _state_246b;
	Packets_t _program_2474;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_2484;
		SFC::Program program_2485;
		SFC::StateLabel stateLabel_2486;
	};

	std::list< Match> _matches;
};

class CreateDEVars_249b
{
public:
	void operator()( const Packets_t& states_249c, const Packets_t& programs_249e, Packets_t& states_24a0, Packets_t& programs_24a1);

protected:
	void callCheckStateDecomp_27d7( const Packets_t& states_20a3, const Packets_t& programs_20a5);
	void callCreateDataVar_27da( const Packets_t& states_24a5, const Packets_t& programs_24a7);
	void callGetSubStates_27dd( const Packets_t& states_c8c, const Packets_t& parStmnts_c8f);
	void callCreateStoreVar_27e0( const Packets_t& states_2520, const Packets_t& programs_2522);
	void callCreateEventVar_27e3( const Packets_t& states_24e6, const Packets_t& programs_24e8);

private:
	Packets_t* _state_24a2;
	Packets_t* _program_24a3;
};

class CreateDataVar_24a4
{
public:
	void operator()( const Packets_t& states_24a5, const Packets_t& programs_24a7, Packets_t& states_24a9, Packets_t& programs_24aa);

protected:
	void callCreateDataVar_24e2( const Packets_t& states_24ae, const Packets_t& programs_24b0);

private:
	Packets_t* _state_24ab;
	Packets_t* _program_24ac;
};

class CreateDataVar_24ad
{
public:
	void operator()( const Packets_t& states_24ae, const Packets_t& programs_24b0);

protected:
	bool isInputUnique( const Udm::Object& state_24b6, const Udm::Object& program_24bf);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::DT& DT, CyberComposition::Simulink::Data& Data, SFC::Program& Program, CyberComposition::Simulink::State& State, SFC::StateLabel& StateLabel, CyberComposition::Simulink::SF_TypeBase& TypeBase, CyberComposition::Simulink::TypeBaseRef& TypeBaseRef);
	void processInputPackets( const Packets_t& states_24ae, const Packets_t& programs_24b0);
	bool patternMatcher( const Udm::Object& state_24b4, const Udm::Object& program_24bd);
	void effector();

private:
	Packets_t _state_24b2;
	Packets_t _program_24bb;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_24d3;
		SFC::Program program_24d4;
		SFC::StateLabel stateLabel_24d5;
		CyberComposition::Simulink::Data data_24d6;
		CyberComposition::Simulink::TypeBaseRef typeBaseRef_24d7;
		CyberComposition::Simulink::SF_TypeBase typeBase_24d8;
		SFC::DT dT_24d9;
	};

	std::list< Match> _matches;
};

class CreateEventVar_24e5
{
public:
	void operator()( const Packets_t& states_24e6, const Packets_t& programs_24e8, Packets_t& states_24ea, Packets_t& programs_24eb);

protected:
	void callCreateEventVar_251c( const Packets_t& states_24ef, const Packets_t& programs_24f1);

private:
	Packets_t* _state_24ec;
	Packets_t* _program_24ed;
};

class CreateEventVar_24ee
{
public:
	void operator()( const Packets_t& states_24ef, const Packets_t& programs_24f1);

protected:
	bool isInputUnique( const Udm::Object& state_24f7, const Udm::Object& program_2500);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& states_24ef, const Packets_t& programs_24f1);
	bool patternMatcher( const Udm::Object& state_24f5, const Udm::Object& program_24fe);
	void effector();

private:
	Packets_t _state_24f3;
	Packets_t _program_24fc;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_2514;
		SFC::Program program_2515;
		SFC::StateLabel stateLabel_2516;
		CyberComposition::Simulink::Event event_2517;
		CyberComposition::Simulink::TypeBaseRef typeBaseRef_2518;
		CyberComposition::Simulink::SF_TypeBase typeBase_2519;
		SFC::DT dT_251a;
	};

	std::list< Match> _matches;
};

class CreateStoreVar_251f
{
public:
	void operator()( const Packets_t& states_2520, const Packets_t& programs_2522, Packets_t& states_2524, Packets_t& programs_2525);

protected:
	void callCreateDataVar_27c5( const Packets_t& states_2627, const Packets_t& programs_262b);
	void callGetHighestState_27c8( const Packets_t& states_2668, const Packets_t& localVars_266a);
	void callGetContainingSubsystem_27cb( const Packets_t& states_25f5, const Packets_t& localVars_25f8);
	void callGetStoreSubsystem_27ce( const Packets_t& systems_2529, const Packets_t& localVars_252b);
	void callGetScope_27d1( const Packets_t& systems_2727, const Packets_t& localVars_2729);
	void callRegister_27d4( const Packets_t& subsystems_270a, const Packets_t& localVars_270c);

private:
	Packets_t* _state_2526;
	Packets_t* _program_2527;
};

class GetStoreSubsystem_2528
{
public:
	void operator()( const Packets_t& systems_2529, const Packets_t& localVars_252b, Packets_t& systems_252d, Packets_t& localVars_252e);

protected:
	void callStoreTest_25eb( const Packets_t& systems_257b, const Packets_t& localVars_257d);
	void callInitScope_25ee( const Packets_t& subsystems_2532, const Packets_t& localVars_2535);
	void callNextHigherSubsystem_25f1( const Packets_t& subSubsystems_2553, const Packets_t& localVars_2556);

private:
	Packets_t* _system_252f;
	Packets_t* _localVar_2530;
};

class InitScope_2531
{
public:
	void operator()( const Packets_t& subsystems_2532, const Packets_t& localVars_2535, Packets_t& subsystems_2534, Packets_t& localVars_2537);

protected:
	bool isInputUnique( const Udm::Object& subsystem_253e, const Udm::Object& localVar_2547);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& subsystems_2532, const Packets_t& localVars_2535);
	bool patternMatcher( const Udm::Object& subsystem_253c, const Udm::Object& localVar_2545);
	void effector();
	void forwardInputs();

private:
	Packets_t* _subsystem_2538;
	Packets_t* _localVar_2539;
	Packets_t _subsystem_253a;
	Packets_t _localVar_2543;
	class Match
	{
	public:
		CyberComposition::Simulink::Subsystem subsystem_2550;
		SFC::LocalVar localVar_2551;
	};

	std::list< Match> _matches;
};

class NextHigherSubsystem_2552
{
public:
	void operator()( const Packets_t& subSubsystems_2553, const Packets_t& localVars_2556, Packets_t& subsystems_2555, Packets_t& localVars_2558);

protected:
	bool isInputUnique( const Udm::Object& subSubsystem_255f, const Udm::Object& localVar_2568);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& subSubsystems_2553, const Packets_t& localVars_2556);
	bool patternMatcher( const Udm::Object& subSubsystem_255d, const Udm::Object& localVar_2566);
	void effector();
	void outputAppender( const CyberComposition::Simulink::Subsystem& subsystem_2576, const SFC::LocalVar& localVar_2578);

private:
	Packets_t* _subsystem_2559;
	Packets_t* _localVar_255a;
	Packets_t _subSubsystem_255b;
	Packets_t _localVar_2564;
	class Match
	{
	public:
		CyberComposition::Simulink::Subsystem subSubsystem_2573;
		SFC::LocalVar localVar_2574;
		CyberComposition::Simulink::Subsystem subsystem_2575;
	};

	std::list< Match> _matches;
};

class StoreTest_257a
{
public:
	void operator()( const Packets_t& systems_257b, const Packets_t& localVars_257d, Packets_t& systems_257f, Packets_t& localVars_2580, Packets_t& systems_2581, Packets_t& localVars_2582);

protected:
	void executeOne( const Packets_t& systems_257b, const Packets_t& localVars_257d);
	bool isInputUnique( const Udm::Object& system_2589, const Udm::Object& localVar_2590);

private:
	Packets_t* _system_2583;
	Packets_t* _localVar_2584;
	Packets_t* _system_2585;
	Packets_t* _localVar_2586;
	Packets_t _system_2587;
	Packets_t _localVar_258e;
};

class HasStore_2595
{
public:
	bool operator()( const Packets_t& subsystems_2596, const Packets_t& localVars_2599, Packets_t& subsystems_2598, Packets_t& localVars_259b);

protected:
	bool isInputUnique( const Udm::Object& subsystem_25a2, const Udm::Object& localVar_25ab);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::LocalVar& LocalVar, CyberComposition::Simulink::SF_Parameter& Parameter, CyberComposition::Simulink::Primitive& Primitive, CyberComposition::Simulink::Subsystem& Subsystem);
	void processInputPackets( const Packets_t& subsystems_2596, const Packets_t& localVars_2599);
	bool patternMatcher( const Udm::Object& subsystem_25a0, const Udm::Object& localVar_25a9);
	void outputAppender( const CyberComposition::Simulink::Subsystem& subsystem_25c2, const SFC::LocalVar& localVar_25c4);

private:
	Packets_t* _subsystem_259c;
	Packets_t* _localVar_259d;
	Packets_t _subsystem_259e;
	Packets_t _localVar_25a7;
	class Match
	{
	public:
		CyberComposition::Simulink::Subsystem subsystem_25ba;
		SFC::LocalVar localVar_25bb;
		CyberComposition::Simulink::SF_Parameter parameter_25bc;
		CyberComposition::Simulink::Primitive primitive_25bd;
	};

	std::list< Match> _matches;
};

class Otherwise_25c6
{
public:
	bool operator()( const Packets_t& subsystems_25c7, const Packets_t& localVars_25ca, Packets_t& subsystems_25c9, Packets_t& localVars_25cc);

protected:
	bool isInputUnique( const Udm::Object& subsystem_25d3, const Udm::Object& localVar_25dc);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& subsystems_25c7, const Packets_t& localVars_25ca);
	bool patternMatcher( const Udm::Object& subsystem_25d1, const Udm::Object& localVar_25da);
	void outputAppender( const CyberComposition::Simulink::Subsystem& subsystem_25e7, const SFC::LocalVar& localVar_25e9);

private:
	Packets_t* _subsystem_25cd;
	Packets_t* _localVar_25ce;
	Packets_t _subsystem_25cf;
	Packets_t _localVar_25d8;
	class Match
	{
	public:
		CyberComposition::Simulink::Subsystem subsystem_25e5;
		SFC::LocalVar localVar_25e6;
	};

	std::list< Match> _matches;
};

class GetContainingSubsystem_25f4
{
public:
	void operator()( const Packets_t& states_25f5, const Packets_t& localVars_25f8, Packets_t& subsystems_25f7, Packets_t& localVars_25fa);

protected:
	bool isInputUnique( const Udm::Object& state_2601, const Udm::Object& localVar_260a);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& states_25f5, const Packets_t& localVars_25f8);
	bool patternMatcher( const Udm::Object& state_25ff, const Udm::Object& localVar_2608);
	void effector();
	void outputAppender( const CyberComposition::Simulink::Subsystem& subsystem_2622, const SFC::LocalVar& localVar_2624);

private:
	Packets_t* _subsystem_25fb;
	Packets_t* _localVar_25fc;
	Packets_t _state_25fd;
	Packets_t _localVar_2606;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_261c;
		SFC::LocalVar localVar_261d;
		CyberComposition::Simulink::Subsystem subSubsystem_261e;
		CyberComposition::Simulink::Subsystem subsystem_261f;
		CyberComposition::Simulink::ConnectorRef connectorRef_2620;
		CyberComposition::Simulink::Primitive primitive_2621;
	};

	std::list< Match> _matches;
};

class CreateDataVar_2626
{
public:
	void operator()( const Packets_t& states_2627, const Packets_t& programs_262b, Packets_t& states_2629, Packets_t& localVars_262a);

protected:
	bool isInputUnique( const Udm::Object& state_2633, const Udm::Object& program_263c);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::DT& DT, CyberComposition::Simulink::Data& Data, SFC::Program& Program, SFC::Project& Project, CyberComposition::Simulink::State& State, SFC::StateLabel& StateLabel, CyberComposition::Simulink::SF_TypeBase& TypeBase, CyberComposition::Simulink::TypeBaseRef& TypeBaseRef);
	void processInputPackets( const Packets_t& states_2627, const Packets_t& programs_262b);
	bool patternMatcher( const Udm::Object& state_2631, const Udm::Object& program_263a);
	void effector();
	void outputAppender( const CyberComposition::Simulink::State& state_2663, const SFC::LocalVar& localVar_2665);

private:
	Packets_t* _state_262d;
	Packets_t* _localVar_262e;
	Packets_t _state_262f;
	Packets_t _program_2638;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_2652;
		SFC::Program program_2653;
		SFC::StateLabel stateLabel_2654;
		CyberComposition::Simulink::Data data_2655;
		CyberComposition::Simulink::TypeBaseRef typeBaseRef_2656;
		CyberComposition::Simulink::SF_TypeBase typeBase_2657;
		SFC::DT dT_2658;
		SFC::Project project_2659;
	};

	std::list< Match> _matches;
};

class GetHighestState_2667
{
public:
	void operator()( const Packets_t& states_2668, const Packets_t& localVars_266a, Packets_t& states_266c, Packets_t& localVars_266d);

protected:
	void callHighestStateTest_2703( const Packets_t& states_2671, const Packets_t& localVars_2673);
	void callNextHigherState_2706( const Packets_t& subStates_26dc, const Packets_t& localVars_26df);

private:
	Packets_t* _state_266e;
	Packets_t* _localVar_266f;
};

class HighestStateTest_2670
{
public:
	void operator()( const Packets_t& states_2671, const Packets_t& localVars_2673, Packets_t& states_2675, Packets_t& localVars_2676, Packets_t& states_2677, Packets_t& localVars_2678);

protected:
	void executeOne( const Packets_t& states_2671, const Packets_t& localVars_2673);
	bool isInputUnique( const Udm::Object& state_267f, const Udm::Object& localVar_2686);

private:
	Packets_t* _state_2679;
	Packets_t* _localVar_267a;
	Packets_t* _state_267b;
	Packets_t* _localVar_267c;
	Packets_t _state_267d;
	Packets_t _localVar_2684;
};

class HighestState_268b
{
public:
	bool operator()( const Packets_t& subStates_268c, const Packets_t& localVars_268f, Packets_t& subStates_268e, Packets_t& localVars_2691);

protected:
	bool isInputUnique( const Udm::Object& subState_2698, const Udm::Object& localVar_26a1);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& subStates_268c, const Packets_t& localVars_268f);
	bool patternMatcher( const Udm::Object& subState_2696, const Udm::Object& localVar_269f);
	void outputAppender( const CyberComposition::Simulink::State& subState_26b2, const SFC::LocalVar& localVar_26b4);

private:
	Packets_t* _subState_2692;
	Packets_t* _localVar_2693;
	Packets_t _subState_2694;
	Packets_t _localVar_269d;
	class Match
	{
	public:
		CyberComposition::Simulink::State subState_26ae;
		SFC::LocalVar localVar_26af;
		CyberComposition::Simulink::State state_26b0;
		CyberComposition::Simulink::Block stateflow_26b1;
	};

	std::list< Match> _matches;
};

class Otherwise_26b6
{
public:
	bool operator()( const Packets_t& states_26b7, const Packets_t& localVars_26ba, Packets_t& states_26b9, Packets_t& localVars_26bc);

protected:
	bool isInputUnique( const Udm::Object& state_26c3, const Udm::Object& localVar_26cc);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& states_26b7, const Packets_t& localVars_26ba);
	bool patternMatcher( const Udm::Object& state_26c1, const Udm::Object& localVar_26ca);
	void outputAppender( const CyberComposition::Simulink::State& state_26d7, const SFC::LocalVar& localVar_26d9);

private:
	Packets_t* _state_26bd;
	Packets_t* _localVar_26be;
	Packets_t _state_26bf;
	Packets_t _localVar_26c8;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_26d5;
		SFC::LocalVar localVar_26d6;
	};

	std::list< Match> _matches;
};

class NextHigherState_26db
{
public:
	void operator()( const Packets_t& subStates_26dc, const Packets_t& localVars_26df, Packets_t& states_26de, Packets_t& localVars_26e1);

protected:
	bool isInputUnique( const Udm::Object& subState_26e8, const Udm::Object& localVar_26f1);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& subStates_26dc, const Packets_t& localVars_26df);
	bool patternMatcher( const Udm::Object& subState_26e6, const Udm::Object& localVar_26ef);
	void effector();
	void outputAppender( const CyberComposition::Simulink::State& state_26ff, const SFC::LocalVar& localVar_2701);

private:
	Packets_t* _state_26e2;
	Packets_t* _localVar_26e3;
	Packets_t _subState_26e4;
	Packets_t _localVar_26ed;
	class Match
	{
	public:
		CyberComposition::Simulink::State subState_26fc;
		SFC::LocalVar localVar_26fd;
		CyberComposition::Simulink::State state_26fe;
	};

	std::list< Match> _matches;
};

class Register_2709
{
public:
	void operator()( const Packets_t& subsystems_270a, const Packets_t& localVars_270c);

protected:
	bool isInputUnique( const Udm::Object& subsystem_2712, const Udm::Object& localVar_271b);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& subsystems_270a, const Packets_t& localVars_270c);
	bool patternMatcher( const Udm::Object& subsystem_2710, const Udm::Object& localVar_2719);
	void effector();

private:
	Packets_t _subsystem_270e;
	Packets_t _localVar_2717;
	class Match
	{
	public:
		CyberComposition::Simulink::Subsystem subsystem_2724;
		SFC::LocalVar localVar_2725;
	};

	std::list< Match> _matches;
};

class GetScope_2726
{
public:
	void operator()( const Packets_t& systems_2727, const Packets_t& localVars_2729, Packets_t& systems_272b, Packets_t& localVars_272c);

protected:
	void callSubSubsystemTest_27bf( const Packets_t& systems_2758, const Packets_t& localVars_275a);
	void callAddToScope_27c2( const Packets_t& subSubsystems_2730, const Packets_t& localVars_2733);

private:
	Packets_t* _system_272d;
	Packets_t* _localVar_272e;
};

class AddToScope_272f
{
public:
	void operator()( const Packets_t& subSubsystems_2730, const Packets_t& localVars_2733, Packets_t& subsystems_2732, Packets_t& localVars_2735);

protected:
	bool isInputUnique( const Udm::Object& subSubsystem_273c, const Udm::Object& localVar_2745);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& subSubsystems_2730, const Packets_t& localVars_2733);
	bool patternMatcher( const Udm::Object& subSubsystem_273a, const Udm::Object& localVar_2743);
	void effector();
	void outputAppender( const CyberComposition::Simulink::Subsystem& subsystem_2753, const SFC::LocalVar& localVar_2755);

private:
	Packets_t* _subsystem_2736;
	Packets_t* _localVar_2737;
	Packets_t _subSubsystem_2738;
	Packets_t _localVar_2741;
	class Match
	{
	public:
		CyberComposition::Simulink::Subsystem subSubsystem_2750;
		SFC::LocalVar localVar_2751;
		CyberComposition::Simulink::Subsystem subsystem_2752;
	};

	std::list< Match> _matches;
};

class SubSubsystemTest_2757
{
public:
	void operator()( const Packets_t& systems_2758, const Packets_t& localVars_275a, Packets_t& systems_275c, Packets_t& localVars_275d, Packets_t& systems_275e, Packets_t& localVars_275f);

protected:
	void executeOne( const Packets_t& systems_2758, const Packets_t& localVars_275a);
	bool isInputUnique( const Udm::Object& system_2766, const Udm::Object& localVar_276d);

private:
	Packets_t* _system_2760;
	Packets_t* _localVar_2761;
	Packets_t* _system_2762;
	Packets_t* _localVar_2763;
	Packets_t _system_2764;
	Packets_t _localVar_276b;
};

class NotTopSubsystem_2772
{
public:
	bool operator()( const Packets_t& subSubsystems_2773, const Packets_t& localVars_2776, Packets_t& subSubsystems_2775, Packets_t& localVars_2778);

protected:
	bool isInputUnique( const Udm::Object& subSubsystem_277f, const Udm::Object& localVar_2788);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& subSubsystems_2773, const Packets_t& localVars_2776);
	bool patternMatcher( const Udm::Object& subSubsystem_277d, const Udm::Object& localVar_2786);
	void outputAppender( const CyberComposition::Simulink::Subsystem& subSubsystem_2796, const SFC::LocalVar& localVar_2798);

private:
	Packets_t* _subSubsystem_2779;
	Packets_t* _localVar_277a;
	Packets_t _subSubsystem_277b;
	Packets_t _localVar_2784;
	class Match
	{
	public:
		CyberComposition::Simulink::Subsystem subSubsystem_2793;
		SFC::LocalVar localVar_2794;
		CyberComposition::Simulink::Subsystem subsystem_2795;
	};

	std::list< Match> _matches;
};

class Otherwise_279a
{
public:
	bool operator()( const Packets_t& subsystems_279b, const Packets_t& localVars_279e, Packets_t& subsystems_279d, Packets_t& localVars_27a0);

protected:
	bool isInputUnique( const Udm::Object& subsystem_27a7, const Udm::Object& localVar_27b0);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& subsystems_279b, const Packets_t& localVars_279e);
	bool patternMatcher( const Udm::Object& subsystem_27a5, const Udm::Object& localVar_27ae);
	void outputAppender( const CyberComposition::Simulink::Subsystem& subsystem_27bb, const SFC::LocalVar& localVar_27bd);

private:
	Packets_t* _subsystem_27a1;
	Packets_t* _localVar_27a2;
	Packets_t _subsystem_27a3;
	Packets_t _localVar_27ac;
	class Match
	{
	public:
		CyberComposition::Simulink::Subsystem subsystem_27b9;
		SFC::LocalVar localVar_27ba;
	};

	std::list< Match> _matches;
};

class CreateTypes_2807
{
public:
	void operator()( const Packets_t& rootFolders_2808, const Packets_t& states_280a, const Packets_t& projects_280c, Packets_t& states_280e, Packets_t& projects_280f);

protected:
	void callGetTypeBlock_2db4( const Packets_t& rootFolders_2d48, const Packets_t& projects_2d4a);
	void callCreateTypesInner_2db7( const Packets_t& typess_2813, const Packets_t& projects_2815);
	void callStructMembers_2dba( const Packets_t& typess_2d18, const Packets_t& newStructs_2d1a);
	void callRegisterStruct_2dbd( const Packets_t& sfcStructs_2d07);

private:
	Packets_t* _state_2810;
	Packets_t* _project_2811;
};

class CreateTypesInner_2812
{
public:
	void operator()( const Packets_t& typess_2813, const Packets_t& projects_2815, Packets_t& typess_2817, Packets_t& newStructs_2818);

protected:
	void executeOne( const Packets_t& typess_2813, const Packets_t& projects_2815);
	bool isInputUnique( const Udm::Object& types_281d, const Udm::Object& project_2824);
	void callStructOrMatrix_2cfd( const Packets_t& typess_2c73, const Packets_t& projects_2c75);
	void callCreateStructType_2d00( const Packets_t& ecsl_dp_Structs_2cd8, const Packets_t& projects_2cdb);
	void callCreateArrayOrBasicType_2d03( const Packets_t& typess_282a, const Packets_t& projects_282c);

private:
	Packets_t* _types_2819;
	Packets_t* _newStruct_281a;
	Packets_t _types_281b;
	Packets_t _project_2822;
};

class CreateArrayOrBasicType_2829
{
public:
	void operator()( const Packets_t& typess_282a, const Packets_t& projects_282c);

protected:
	void callDimensionTest_2c60( const Packets_t& typess_2bcf, const Packets_t& projects_2bd1);
	void callProcessOther_2c63( const Packets_t& typess_286d, const Packets_t& projects_286f);
	void callProcessColumn_2c66( const Packets_t& typess_2bbd, const Packets_t& projects_2bbf);
	void callProcessScalar_2c69( const Packets_t& typess_29ad, const Packets_t& projects_29af);
	void callRegisterType_2c6c( const Packets_t& matrixs_282f, const Packets_t& dTs_2832);
	void callMakeAssoc_2c6f( const Packets_t& matrixs_2850, const Packets_t& dTs_2852);
};

class RegisterType_282e
{
public:
	void operator()( const Packets_t& matrixs_282f, const Packets_t& dTs_2832, Packets_t& matrixs_2831, Packets_t& dTs_2834);

protected:
	bool isInputUnique( const Udm::Object& matrix_283b, const Udm::Object& dT_2844);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& matrixs_282f, const Packets_t& dTs_2832);
	bool patternMatcher( const Udm::Object& matrix_2839, const Udm::Object& dT_2842);
	void effector();
	void forwardInputs();

private:
	Packets_t* _matrix_2835;
	Packets_t* _dT_2836;
	Packets_t _matrix_2837;
	Packets_t _dT_2840;
	class Match
	{
	public:
		CyberComposition::Simulink::SF_Matrix matrix_284d;
		SFC::DT dT_284e;
	};

	std::list< Match> _matches;
};

class MakeAssoc_284f
{
public:
	void operator()( const Packets_t& matrixs_2850, const Packets_t& dTs_2852);

protected:
	bool isInputUnique( const Udm::Object& matrix_2858, const Udm::Object& dT_2861);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& matrixs_2850, const Packets_t& dTs_2852);
	bool patternMatcher( const Udm::Object& matrix_2856, const Udm::Object& dT_285f);
	void effector();

private:
	Packets_t _matrix_2854;
	Packets_t _dT_285d;
	class Match
	{
	public:
		CyberComposition::Simulink::SF_Matrix matrix_286a;
		SFC::DT dT_286b;
	};

	std::list< Match> _matches;
};

class ProcessOther_286c
{
public:
	void operator()( const Packets_t& typess_286d, const Packets_t& projects_286f, Packets_t& typess_2871, Packets_t& array1s_2872, Packets_t& projects_2873);

protected:
	void callProcessRowVector_2bb5( const Packets_t& typess_2878, const Packets_t& projects_287a);
	void callProcessArray1_2bb8( const Packets_t& typess_2a8e, const Packets_t& basictypes_2a90, const Packets_t& projects_2a92);

private:
	Packets_t* _types_2874;
	Packets_t* _array1_2875;
	Packets_t* _project_2876;
};

class ProcessRowVector_2877
{
public:
	void operator()( const Packets_t& typess_2878, const Packets_t& projects_287a, Packets_t& typess_287c, Packets_t& array2s_287d, Packets_t& projectss_287e);

protected:
	void callProcessScalar_2a86( const Packets_t& typess_29ad, const Packets_t& projects_29af);
	void callProcessArray2_2a89( const Packets_t& typess_2883, const Packets_t& basictypes_2885, const Packets_t& projects_2887);

private:
	Packets_t* _types_287f;
	Packets_t* _array2_2880;
	Packets_t* _projects_2881;
};

class ProcessArray2_2882
{
public:
	void operator()( const Packets_t& typess_2883, const Packets_t& basictypes_2885, const Packets_t& projects_2887, Packets_t& typess_2889, Packets_t& array2s_288a, Packets_t& projectss_288b);

protected:
	void callGetArray2_29a0( const Packets_t& typess_2890, const Packets_t& basictypes_2892, const Packets_t& projects_2894);
	void callUseArray2_29a4( const Packets_t& matrixs_292c, const Packets_t& basicTypes_2930, const Packets_t& projects_2932);
	void callCreateArray2_29a8( const Packets_t& matrixs_296b, const Packets_t& basicTypes_296f, const Packets_t& projects_2971);

private:
	Packets_t* _types_288c;
	Packets_t* _array2_288d;
	Packets_t* _projects_288e;
};

class GetArray2_288f
{
public:
	void operator()( const Packets_t& typess_2890, const Packets_t& basictypes_2892, const Packets_t& projects_2894, Packets_t& typess_2896, Packets_t& basictypes_2897, Packets_t& projectss_2898, Packets_t& typess_2899, Packets_t& basictypes_289a, Packets_t& projectss_289b);

protected:
	void executeOne( const Packets_t& typess_2890, const Packets_t& basictypes_2892, const Packets_t& projects_2894);
	bool isInputUnique( const Udm::Object& types_28a4, const Udm::Object& basictype_28ab, const Udm::Object& project_28b2);

private:
	Packets_t* _types_289c;
	Packets_t* _basictype_289d;
	Packets_t* _projects_289e;
	Packets_t* _types_289f;
	Packets_t* _basictype_28a0;
	Packets_t* _projects_28a1;
	Packets_t _types_28a2;
	Packets_t _basictype_28a9;
	Packets_t _project_28b0;
};

class Array2Exists_28b7
{
public:
	bool operator()( const Packets_t& matrixs_28b8, const Packets_t& basicTypes_28bb, const Packets_t& projects_28be, Packets_t& matrixs_28ba, Packets_t& basicTypes_28bd, Packets_t& projects_28c0);

protected:
	bool isInputUnique( const Udm::Object& matrix_28c8, const Udm::Object& basicType_28d1, const Udm::Object& project_28da);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::Array& Array, SFC::BasicType& BasicType, CyberComposition::Simulink::SF_Matrix& Matrix, SFC::Project& Project);
	void processInputPackets( const Packets_t& matrixs_28b8, const Packets_t& basicTypes_28bb, const Packets_t& projects_28be);
	bool patternMatcher( const Udm::Object& matrix_28c6, const Udm::Object& basicType_28cf, const Udm::Object& project_28d8);
	void outputAppender( const CyberComposition::Simulink::SF_Matrix& matrix_28f0, const SFC::BasicType& basicType_28f2, const SFC::Project& project_28f4);

private:
	Packets_t* _matrix_28c1;
	Packets_t* _basicType_28c2;
	Packets_t* _project_28c3;
	Packets_t _matrix_28c4;
	Packets_t _basicType_28cd;
	Packets_t _project_28d6;
	class Match
	{
	public:
		CyberComposition::Simulink::SF_Matrix matrix_28e8;
		SFC::BasicType basicType_28e9;
		SFC::Project project_28ea;
		SFC::Array array_28eb;
	};

	std::list< Match> _matches;
};

class Otherwise_28f6
{
public:
	bool operator()( const Packets_t& matrixs_28f7, const Packets_t& basicTypes_28fa, const Packets_t& projects_28fd, Packets_t& matrixs_28f9, Packets_t& basicTypes_28fc, Packets_t& projects_28ff);

protected:
	bool isInputUnique( const Udm::Object& matrix_2907, const Udm::Object& basicType_2910, const Udm::Object& project_2919);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& matrixs_28f7, const Packets_t& basicTypes_28fa, const Packets_t& projects_28fd);
	bool patternMatcher( const Udm::Object& matrix_2905, const Udm::Object& basicType_290e, const Udm::Object& project_2917);
	void outputAppender( const CyberComposition::Simulink::SF_Matrix& matrix_2925, const SFC::BasicType& basicType_2927, const SFC::Project& project_2929);

private:
	Packets_t* _matrix_2900;
	Packets_t* _basicType_2901;
	Packets_t* _project_2902;
	Packets_t _matrix_2903;
	Packets_t _basicType_290c;
	Packets_t _project_2915;
	class Match
	{
	public:
		CyberComposition::Simulink::SF_Matrix matrix_2922;
		SFC::BasicType basicType_2923;
		SFC::Project project_2924;
	};

	std::list< Match> _matches;
};

class UseArray2_292b
{
public:
	void operator()( const Packets_t& matrixs_292c, const Packets_t& basicTypes_2930, const Packets_t& projects_2932, Packets_t& matrixs_292e, Packets_t& arrays_292f, Packets_t& projects_2934);

protected:
	bool isInputUnique( const Udm::Object& matrix_293c, const Udm::Object& basicType_2945, const Udm::Object& project_294e);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::Array& Array, SFC::BasicType& BasicType, CyberComposition::Simulink::SF_Matrix& Matrix, SFC::Project& Project);
	void processInputPackets( const Packets_t& matrixs_292c, const Packets_t& basicTypes_2930, const Packets_t& projects_2932);
	bool patternMatcher( const Udm::Object& matrix_293a, const Udm::Object& basicType_2943, const Udm::Object& project_294c);
	void effector();
	void outputAppender( const CyberComposition::Simulink::SF_Matrix& matrix_2964, const SFC::Array& array_2966, const SFC::Project& project_2968);

private:
	Packets_t* _matrix_2935;
	Packets_t* _array_2936;
	Packets_t* _project_2937;
	Packets_t _matrix_2938;
	Packets_t _basicType_2941;
	Packets_t _project_294a;
	class Match
	{
	public:
		CyberComposition::Simulink::SF_Matrix matrix_295c;
		SFC::BasicType basicType_295d;
		SFC::Project project_295e;
		SFC::Array array_295f;
	};

	std::list< Match> _matches;
};

class CreateArray2_296a
{
public:
	void operator()( const Packets_t& matrixs_296b, const Packets_t& basicTypes_296f, const Packets_t& projects_2971, Packets_t& matrixs_296d, Packets_t& arrays_296e, Packets_t& projects_2973);

protected:
	bool isInputUnique( const Udm::Object& matrix_297b, const Udm::Object& basicType_2984, const Udm::Object& project_298d);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& matrixs_296b, const Packets_t& basicTypes_296f, const Packets_t& projects_2971);
	bool patternMatcher( const Udm::Object& matrix_2979, const Udm::Object& basicType_2982, const Udm::Object& project_298b);
	void effector();
	void outputAppender( const CyberComposition::Simulink::SF_Matrix& matrix_299a, const SFC::Array& array_299c, const SFC::Project& project_299e);

private:
	Packets_t* _matrix_2974;
	Packets_t* _array_2975;
	Packets_t* _project_2976;
	Packets_t _matrix_2977;
	Packets_t _basicType_2980;
	Packets_t _project_2989;
	class Match
	{
	public:
		CyberComposition::Simulink::SF_Matrix matrix_2996;
		SFC::BasicType basicType_2997;
		SFC::Project project_2998;
	};

	std::list< Match> _matches;
};

class ProcessScalar_29ac
{
public:
	void operator()( const Packets_t& typess_29ad, const Packets_t& projects_29af, Packets_t& typess_29b1, Packets_t& basictypes_29b2, Packets_t& projects_29b3);

protected:
	void callGetBasicType_2a7d( const Packets_t& typess_2a12, const Packets_t& projects_2a14);
	void callCreateBasicType_2a80( const Packets_t& matrixs_29b8, const Packets_t& projects_29bc);
	void callUseBasicType_2a83( const Packets_t& matrixs_29e2, const Packets_t& projects_29e6);

private:
	Packets_t* _types_29b4;
	Packets_t* _basictype_29b5;
	Packets_t* _project_29b6;
};

class CreateBasicType_29b7
{
public:
	void operator()( const Packets_t& matrixs_29b8, const Packets_t& projects_29bc, Packets_t& matrixs_29ba, Packets_t& basicTypes_29bb, Packets_t& projects_29be);

protected:
	bool isInputUnique( const Udm::Object& matrix_29c6, const Udm::Object& project_29cf);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& matrixs_29b8, const Packets_t& projects_29bc);
	bool patternMatcher( const Udm::Object& matrix_29c4, const Udm::Object& project_29cd);
	void effector();
	void outputAppender( const CyberComposition::Simulink::SF_Matrix& matrix_29db, const SFC::BasicType& basicType_29dd, const SFC::Project& project_29df);

private:
	Packets_t* _matrix_29bf;
	Packets_t* _basicType_29c0;
	Packets_t* _project_29c1;
	Packets_t _matrix_29c2;
	Packets_t _project_29cb;
	class Match
	{
	public:
		CyberComposition::Simulink::SF_Matrix matrix_29d8;
		SFC::Project project_29d9;
	};

	std::list< Match> _matches;
};

class UseBasicType_29e1
{
public:
	void operator()( const Packets_t& matrixs_29e2, const Packets_t& projects_29e6, Packets_t& matrixs_29e4, Packets_t& basicTypes_29e5, Packets_t& projects_29e8);

protected:
	bool isInputUnique( const Udm::Object& matrix_29f0, const Udm::Object& project_29f9);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::BasicType& BasicType, CyberComposition::Simulink::SF_Matrix& Matrix, SFC::Project& Project);
	void processInputPackets( const Packets_t& matrixs_29e2, const Packets_t& projects_29e6);
	bool patternMatcher( const Udm::Object& matrix_29ee, const Udm::Object& project_29f7);
	void effector();
	void outputAppender( const CyberComposition::Simulink::SF_Matrix& matrix_2a0b, const SFC::BasicType& basicType_2a0d, const SFC::Project& project_2a0f);

private:
	Packets_t* _matrix_29e9;
	Packets_t* _basicType_29ea;
	Packets_t* _project_29eb;
	Packets_t _matrix_29ec;
	Packets_t _project_29f5;
	class Match
	{
	public:
		CyberComposition::Simulink::SF_Matrix matrix_2a05;
		SFC::Project project_2a06;
		SFC::BasicType basicType_2a07;
	};

	std::list< Match> _matches;
};

class GetBasicType_2a11
{
public:
	void operator()( const Packets_t& typess_2a12, const Packets_t& projects_2a14, Packets_t& typess_2a16, Packets_t& projectss_2a17, Packets_t& typess_2a18, Packets_t& projectss_2a19);

protected:
	void executeOne( const Packets_t& typess_2a12, const Packets_t& projects_2a14);
	bool isInputUnique( const Udm::Object& types_2a20, const Udm::Object& project_2a27);

private:
	Packets_t* _types_2a1a;
	Packets_t* _projects_2a1b;
	Packets_t* _types_2a1c;
	Packets_t* _projects_2a1d;
	Packets_t _types_2a1e;
	Packets_t _project_2a25;
};

class BasicTypeExists_2a2c
{
public:
	bool operator()( const Packets_t& matrixs_2a2d, const Packets_t& projects_2a30, Packets_t& matrixs_2a2f, Packets_t& projects_2a32);

protected:
	bool isInputUnique( const Udm::Object& matrix_2a39, const Udm::Object& project_2a42);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::BasicType& BasicType, CyberComposition::Simulink::SF_Matrix& Matrix, SFC::Project& Project);
	void processInputPackets( const Packets_t& matrixs_2a2d, const Packets_t& projects_2a30);
	bool patternMatcher( const Udm::Object& matrix_2a37, const Udm::Object& project_2a40);
	void outputAppender( const CyberComposition::Simulink::SF_Matrix& matrix_2a54, const SFC::Project& project_2a56);

private:
	Packets_t* _matrix_2a33;
	Packets_t* _project_2a34;
	Packets_t _matrix_2a35;
	Packets_t _project_2a3e;
	class Match
	{
	public:
		CyberComposition::Simulink::SF_Matrix matrix_2a4e;
		SFC::Project project_2a4f;
		SFC::BasicType basicType_2a50;
	};

	std::list< Match> _matches;
};

class Otherwise_2a58
{
public:
	bool operator()( const Packets_t& matrixs_2a59, const Packets_t& projects_2a5c, Packets_t& matrixs_2a5b, Packets_t& projects_2a5e);

protected:
	bool isInputUnique( const Udm::Object& matrix_2a65, const Udm::Object& project_2a6e);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& matrixs_2a59, const Packets_t& projects_2a5c);
	bool patternMatcher( const Udm::Object& matrix_2a63, const Udm::Object& project_2a6c);
	void outputAppender( const CyberComposition::Simulink::SF_Matrix& matrix_2a79, const SFC::Project& project_2a7b);

private:
	Packets_t* _matrix_2a5f;
	Packets_t* _project_2a60;
	Packets_t _matrix_2a61;
	Packets_t _project_2a6a;
	class Match
	{
	public:
		CyberComposition::Simulink::SF_Matrix matrix_2a77;
		SFC::Project project_2a78;
	};

	std::list< Match> _matches;
};

class ProcessArray1_2a8d
{
public:
	void operator()( const Packets_t& typess_2a8e, const Packets_t& basictypes_2a90, const Packets_t& projects_2a92, Packets_t& typess_2a94, Packets_t& array1s_2a95, Packets_t& projects_2a96);

protected:
	void callGetArray1_2ba9( const Packets_t& typess_2b0f, const Packets_t& dts_2b11, const Packets_t& projects_2b13);
	void callCreateArray1_2bad( const Packets_t& matrixs_2a9b, const Packets_t& dTs_2a9f, const Packets_t& projects_2aa1);
	void callUseArray1_2bb1( const Packets_t& matrixs_2ad1, const Packets_t& dTs_2ad5, const Packets_t& projects_2ad7);

private:
	Packets_t* _types_2a97;
	Packets_t* _array1_2a98;
	Packets_t* _project_2a99;
};

class CreateArray1_2a9a
{
public:
	void operator()( const Packets_t& matrixs_2a9b, const Packets_t& dTs_2a9f, const Packets_t& projects_2aa1, Packets_t& matrixs_2a9d, Packets_t& arrays_2a9e, Packets_t& projects_2aa3);

protected:
	bool isInputUnique( const Udm::Object& matrix_2aab, const Udm::Object& dT_2ab4, const Udm::Object& project_2abd);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& matrixs_2a9b, const Packets_t& dTs_2a9f, const Packets_t& projects_2aa1);
	bool patternMatcher( const Udm::Object& matrix_2aa9, const Udm::Object& dT_2ab2, const Udm::Object& project_2abb);
	void effector();
	void outputAppender( const CyberComposition::Simulink::SF_Matrix& matrix_2aca, const SFC::Array& array_2acc, const SFC::Project& project_2ace);

private:
	Packets_t* _matrix_2aa4;
	Packets_t* _array_2aa5;
	Packets_t* _project_2aa6;
	Packets_t _matrix_2aa7;
	Packets_t _dT_2ab0;
	Packets_t _project_2ab9;
	class Match
	{
	public:
		CyberComposition::Simulink::SF_Matrix matrix_2ac6;
		SFC::DT dT_2ac7;
		SFC::Project project_2ac8;
	};

	std::list< Match> _matches;
};

class UseArray1_2ad0
{
public:
	void operator()( const Packets_t& matrixs_2ad1, const Packets_t& dTs_2ad5, const Packets_t& projects_2ad7, Packets_t& matrixs_2ad3, Packets_t& arrays_2ad4, Packets_t& projects_2ad9);

protected:
	bool isInputUnique( const Udm::Object& matrix_2ae1, const Udm::Object& dT_2aea, const Udm::Object& project_2af3);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::Array& Array, SFC::DT& DT, CyberComposition::Simulink::SF_Matrix& Matrix, SFC::Project& Project);
	void processInputPackets( const Packets_t& matrixs_2ad1, const Packets_t& dTs_2ad5, const Packets_t& projects_2ad7);
	bool patternMatcher( const Udm::Object& matrix_2adf, const Udm::Object& dT_2ae8, const Udm::Object& project_2af1);
	void effector();
	void outputAppender( const CyberComposition::Simulink::SF_Matrix& matrix_2b08, const SFC::Array& array_2b0a, const SFC::Project& project_2b0c);

private:
	Packets_t* _matrix_2ada;
	Packets_t* _array_2adb;
	Packets_t* _project_2adc;
	Packets_t _matrix_2add;
	Packets_t _dT_2ae6;
	Packets_t _project_2aef;
	class Match
	{
	public:
		CyberComposition::Simulink::SF_Matrix matrix_2b00;
		SFC::DT dT_2b01;
		SFC::Project project_2b02;
		SFC::Array array_2b03;
	};

	std::list< Match> _matches;
};

class GetArray1_2b0e
{
public:
	void operator()( const Packets_t& typess_2b0f, const Packets_t& dts_2b11, const Packets_t& projects_2b13, Packets_t& typess_2b15, Packets_t& dts_2b16, Packets_t& projectss_2b17, Packets_t& typess_2b18, Packets_t& dts_2b19, Packets_t& projectss_2b1a);

protected:
	void executeOne( const Packets_t& typess_2b0f, const Packets_t& dts_2b11, const Packets_t& projects_2b13);
	bool isInputUnique( const Udm::Object& types_2b23, const Udm::Object& dt_2b2a, const Udm::Object& project_2b31);

private:
	Packets_t* _types_2b1b;
	Packets_t* _dt_2b1c;
	Packets_t* _projects_2b1d;
	Packets_t* _types_2b1e;
	Packets_t* _dt_2b1f;
	Packets_t* _projects_2b20;
	Packets_t _types_2b21;
	Packets_t _dt_2b28;
	Packets_t _project_2b2f;
};

class Array1Exists_2b36
{
public:
	bool operator()( const Packets_t& matrixs_2b37, const Packets_t& dTs_2b3a, const Packets_t& projects_2b3d, Packets_t& matrixs_2b39, Packets_t& dTs_2b3c, Packets_t& projects_2b3f);

protected:
	bool isInputUnique( const Udm::Object& matrix_2b47, const Udm::Object& dT_2b50, const Udm::Object& project_2b59);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::Array& Array, SFC::DT& DT, CyberComposition::Simulink::SF_Matrix& Matrix, SFC::Project& Project);
	void processInputPackets( const Packets_t& matrixs_2b37, const Packets_t& dTs_2b3a, const Packets_t& projects_2b3d);
	bool patternMatcher( const Udm::Object& matrix_2b45, const Udm::Object& dT_2b4e, const Udm::Object& project_2b57);
	void outputAppender( const CyberComposition::Simulink::SF_Matrix& matrix_2b6e, const SFC::DT& dT_2b70, const SFC::Project& project_2b72);

private:
	Packets_t* _matrix_2b40;
	Packets_t* _dT_2b41;
	Packets_t* _project_2b42;
	Packets_t _matrix_2b43;
	Packets_t _dT_2b4c;
	Packets_t _project_2b55;
	class Match
	{
	public:
		CyberComposition::Simulink::SF_Matrix matrix_2b66;
		SFC::DT dT_2b67;
		SFC::Project project_2b68;
		SFC::Array array_2b69;
	};

	std::list< Match> _matches;
};

class Otherwise_2b74
{
public:
	bool operator()( const Packets_t& matrixs_2b75, const Packets_t& dTs_2b78, const Packets_t& projects_2b7b, Packets_t& matrixs_2b77, Packets_t& dTs_2b7a, Packets_t& projects_2b7d);

protected:
	bool isInputUnique( const Udm::Object& matrix_2b85, const Udm::Object& dT_2b8e, const Udm::Object& project_2b97);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& matrixs_2b75, const Packets_t& dTs_2b78, const Packets_t& projects_2b7b);
	bool patternMatcher( const Udm::Object& matrix_2b83, const Udm::Object& dT_2b8c, const Udm::Object& project_2b95);
	void outputAppender( const CyberComposition::Simulink::SF_Matrix& matrix_2ba3, const SFC::DT& dT_2ba5, const SFC::Project& project_2ba7);

private:
	Packets_t* _matrix_2b7e;
	Packets_t* _dT_2b7f;
	Packets_t* _project_2b80;
	Packets_t _matrix_2b81;
	Packets_t _dT_2b8a;
	Packets_t _project_2b93;
	class Match
	{
	public:
		CyberComposition::Simulink::SF_Matrix matrix_2ba0;
		SFC::DT dT_2ba1;
		SFC::Project project_2ba2;
	};

	std::list< Match> _matches;
};

class ProcessColumn_2bbc
{
public:
	void operator()( const Packets_t& typess_2bbd, const Packets_t& projects_2bbf, Packets_t& typess_2bc1, Packets_t& array1s_2bc2, Packets_t& projects_2bc3);

protected:
	void callProcessScalar_2bc7( const Packets_t& typess_29ad, const Packets_t& projects_29af);
	void callProcessArray1_2bca( const Packets_t& typess_2a8e, const Packets_t& basictypes_2a90, const Packets_t& projects_2a92);

private:
	Packets_t* _types_2bc4;
	Packets_t* _array1_2bc5;
	Packets_t* _project_2bc6;
};

class DimensionTest_2bce
{
public:
	void operator()( const Packets_t& typess_2bcf, const Packets_t& projects_2bd1, Packets_t& typess_2bd3, Packets_t& projects_2bd4, Packets_t& typess_2bd5, Packets_t& projects_2bd6, Packets_t& typess_2bd7, Packets_t& projects_2bd8);

protected:
	void executeOne( const Packets_t& typess_2bcf, const Packets_t& projects_2bd1);
	bool isInputUnique( const Udm::Object& types_2be1, const Udm::Object& project_2be8);

private:
	Packets_t* _types_2bd9;
	Packets_t* _project_2bda;
	Packets_t* _types_2bdb;
	Packets_t* _project_2bdc;
	Packets_t* _types_2bdd;
	Packets_t* _project_2bde;
	Packets_t _types_2bdf;
	Packets_t _project_2be6;
};

class Scalar_2bed
{
public:
	bool operator()( const Packets_t& matrixs_2bee, const Packets_t& projects_2bf1, Packets_t& matrixs_2bf0, Packets_t& projects_2bf3);

protected:
	bool isInputUnique( const Udm::Object& matrix_2bfa, const Udm::Object& project_2c03);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( CyberComposition::Simulink::SF_Matrix& Matrix, SFC::Project& Project);
	void processInputPackets( const Packets_t& matrixs_2bee, const Packets_t& projects_2bf1);
	bool patternMatcher( const Udm::Object& matrix_2bf8, const Udm::Object& project_2c01);
	void outputAppender( const CyberComposition::Simulink::SF_Matrix& matrix_2c10, const SFC::Project& project_2c12);

private:
	Packets_t* _matrix_2bf4;
	Packets_t* _project_2bf5;
	Packets_t _matrix_2bf6;
	Packets_t _project_2bff;
	class Match
	{
	public:
		CyberComposition::Simulink::SF_Matrix matrix_2c0c;
		SFC::Project project_2c0d;
	};

	std::list< Match> _matches;
};

class ColumnVector_2c14
{
public:
	bool operator()( const Packets_t& matrixs_2c15, const Packets_t& projects_2c18, Packets_t& matrixs_2c17, Packets_t& projects_2c1a);

protected:
	bool isInputUnique( const Udm::Object& matrix_2c21, const Udm::Object& project_2c2a);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( CyberComposition::Simulink::SF_Matrix& Matrix, SFC::Project& Project);
	void processInputPackets( const Packets_t& matrixs_2c15, const Packets_t& projects_2c18);
	bool patternMatcher( const Udm::Object& matrix_2c1f, const Udm::Object& project_2c28);
	void outputAppender( const CyberComposition::Simulink::SF_Matrix& matrix_2c37, const SFC::Project& project_2c39);

private:
	Packets_t* _matrix_2c1b;
	Packets_t* _project_2c1c;
	Packets_t _matrix_2c1d;
	Packets_t _project_2c26;
	class Match
	{
	public:
		CyberComposition::Simulink::SF_Matrix matrix_2c33;
		SFC::Project project_2c34;
	};

	std::list< Match> _matches;
};

class Other_2c3b
{
public:
	bool operator()( const Packets_t& matrixs_2c3c, const Packets_t& projects_2c3f, Packets_t& matrixs_2c3e, Packets_t& projects_2c41);

protected:
	bool isInputUnique( const Udm::Object& matrix_2c48, const Udm::Object& project_2c51);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& matrixs_2c3c, const Packets_t& projects_2c3f);
	bool patternMatcher( const Udm::Object& matrix_2c46, const Udm::Object& project_2c4f);
	void outputAppender( const CyberComposition::Simulink::SF_Matrix& matrix_2c5c, const SFC::Project& project_2c5e);

private:
	Packets_t* _matrix_2c42;
	Packets_t* _project_2c43;
	Packets_t _matrix_2c44;
	Packets_t _project_2c4d;
	class Match
	{
	public:
		CyberComposition::Simulink::SF_Matrix matrix_2c5a;
		SFC::Project project_2c5b;
	};

	std::list< Match> _matches;
};

class StructOrMatrix_2c72
{
public:
	void operator()( const Packets_t& typess_2c73, const Packets_t& projects_2c75, Packets_t& typess_2c77, Packets_t& projects_2c78, Packets_t& typess_2c79, Packets_t& projects_2c7a);

protected:
	void executeOne( const Packets_t& typess_2c73, const Packets_t& projects_2c75);
	bool isInputUnique( const Udm::Object& types_2c81, const Udm::Object& project_2c88);

private:
	Packets_t* _types_2c7b;
	Packets_t* _project_2c7c;
	Packets_t* _types_2c7d;
	Packets_t* _project_2c7e;
	Packets_t _types_2c7f;
	Packets_t _project_2c86;
};

class IsStruct_2c8d
{
public:
	bool operator()( const Packets_t& typeStructs_2c8e, const Packets_t& projects_2c91, Packets_t& typeStructs_2c90, Packets_t& projects_2c93);

protected:
	bool isInputUnique( const Udm::Object& typeStruct_2c9a, const Udm::Object& project_2ca3);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& typeStructs_2c8e, const Packets_t& projects_2c91);
	bool patternMatcher( const Udm::Object& typeStruct_2c98, const Udm::Object& project_2ca1);
	void outputAppender( const CyberComposition::Simulink::TypeStruct& typeStruct_2cae, const SFC::Project& project_2cb0);

private:
	Packets_t* _typeStruct_2c94;
	Packets_t* _project_2c95;
	Packets_t _typeStruct_2c96;
	Packets_t _project_2c9f;
	class Match
	{
	public:
		CyberComposition::Simulink::TypeStruct typeStruct_2cac;
		SFC::Project project_2cad;
	};

	std::list< Match> _matches;
};

class IsMatrix_2cb2
{
public:
	bool operator()( const Packets_t& matrixs_2cb3, const Packets_t& projects_2cb6, Packets_t& matrixs_2cb5, Packets_t& projects_2cb8);

protected:
	bool isInputUnique( const Udm::Object& matrix_2cbf, const Udm::Object& project_2cc8);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& matrixs_2cb3, const Packets_t& projects_2cb6);
	bool patternMatcher( const Udm::Object& matrix_2cbd, const Udm::Object& project_2cc6);
	void outputAppender( const CyberComposition::Simulink::SF_Matrix& matrix_2cd3, const SFC::Project& project_2cd5);

private:
	Packets_t* _matrix_2cb9;
	Packets_t* _project_2cba;
	Packets_t _matrix_2cbb;
	Packets_t _project_2cc4;
	class Match
	{
	public:
		CyberComposition::Simulink::SF_Matrix matrix_2cd1;
		SFC::Project project_2cd2;
	};

	std::list< Match> _matches;
};

class CreateStructType_2cd7
{
public:
	void operator()( const Packets_t& ecsl_dp_Structs_2cd8, const Packets_t& projects_2cdb, Packets_t& ecsl_dp_Structs_2cda, Packets_t& sfc_Structs_2cdd);

protected:
	bool isInputUnique( const Udm::Object& ecsl_dp_Struct_2ce4, const Udm::Object& project_2ced);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& ecsl_dp_Structs_2cd8, const Packets_t& projects_2cdb);
	bool patternMatcher( const Udm::Object& ecsl_dp_Struct_2ce2, const Udm::Object& project_2ceb);
	void effector();
	void outputAppender( const CyberComposition::Simulink::TypeStruct& ecsl_dp_Struct_2cf9, const SFC::Struct& sfc_Struct_2cfb);

private:
	Packets_t* _ecsl_dp_Struct_2cde;
	Packets_t* _sfc_Struct_2cdf;
	Packets_t _ecsl_dp_Struct_2ce0;
	Packets_t _project_2ce9;
	class Match
	{
	public:
		CyberComposition::Simulink::TypeStruct ecsl_dp_Struct_2cf6;
		SFC::Project project_2cf7;
	};

	std::list< Match> _matches;
};

class RegisterStruct_2d06
{
public:
	void operator()( const Packets_t& sfcStructs_2d07);

protected:
	bool isInputUnique( const Udm::Object& sfcStruct_2d0d);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& sfcStructs_2d07);
	bool patternMatcher( const Udm::Object& sfcStruct_2d0b);
	void effector();

private:
	Packets_t _sfcStruct_2d09;
	class Match
	{
	public:
		SFC::Struct sfcStruct_2d16;
	};

	std::list< Match> _matches;
};

class StructMembers_2d17
{
public:
	void operator()( const Packets_t& typess_2d18, const Packets_t& newStructs_2d1a, Packets_t& newStructs_2d1c);

protected:
	void callCreateStructMembers_2d44( const Packets_t& ecsl_dp_Structs_2d1f, const Packets_t& sfc_Structs_2d21);

private:
	Packets_t* _newStruct_2d1d;
};

class CreateStructMembers_2d1e
{
public:
	void operator()( const Packets_t& ecsl_dp_Structs_2d1f, const Packets_t& sfc_Structs_2d21);

protected:
	bool isInputUnique( const Udm::Object& ecsl_dp_Struct_2d27, const Udm::Object& sfc_Struct_2d30);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& ecsl_dp_Structs_2d1f, const Packets_t& sfc_Structs_2d21);
	bool patternMatcher( const Udm::Object& ecsl_dp_Struct_2d25, const Udm::Object& sfc_Struct_2d2e);
	void effector();

private:
	Packets_t _ecsl_dp_Struct_2d23;
	Packets_t _sfc_Struct_2d2c;
	class Match
	{
	public:
		CyberComposition::Simulink::TypeStruct ecsl_dp_Struct_2d3e;
		SFC::Struct sfc_Struct_2d3f;
		CyberComposition::Simulink::TypeBaseRef ecsl_dp_StructRef_2d40;
		CyberComposition::Simulink::SF_TypeBase ecsl_dp_TypeBase_2d41;
		SFC::DT sfc_DT_2d42;
	};

	std::list< Match> _matches;
};

class GetTypeBlock_2d47
{
public:
	void operator()( const Packets_t& rootFolders_2d48, const Packets_t& projects_2d4a, Packets_t& typess_2d4c, Packets_t& projects_2d4d);

protected:
	void callGetTypes_2dae( const Packets_t& rootFolders_2d82, const Packets_t& projects_2d85);
	void callGetTypesFromLib_2db1( const Packets_t& rootFolders_2d51, const Packets_t& projects_2d54);

private:
	Packets_t* _types_2d4e;
	Packets_t* _project_2d4f;
};

class GetTypesFromLib_2d50
{
public:
	void operator()( const Packets_t& rootFolders_2d51, const Packets_t& projects_2d54, Packets_t& typeBases_2d53, Packets_t& projects_2d56);

protected:
	bool isInputUnique( const Udm::Object& rootFolder_2d5d, const Udm::Object& project_2d66);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& rootFolders_2d51, const Packets_t& projects_2d54);
	bool patternMatcher( const Udm::Object& rootFolder_2d5b, const Udm::Object& project_2d64);
	void effector();
	void outputAppender( const CyberComposition::Simulink::SF_TypeBase& typeBase_2d7d, const SFC::Project& project_2d7f);

private:
	Packets_t* _typeBase_2d57;
	Packets_t* _project_2d58;
	Packets_t _rootFolder_2d59;
	Packets_t _project_2d62;
	class Match
	{
	public:
		CyberComposition::RootFolder rootFolder_2d78;
		SFC::Project project_2d79;
		CyberComposition::RootFolder rootFolder_2d7a;
		CyberComposition::Simulink::Types types_2d7b;
		CyberComposition::Simulink::SF_TypeBase typeBase_2d7c;
	};

	std::list< Match> _matches;
};

class GetTypes_2d81
{
public:
	void operator()( const Packets_t& rootFolders_2d82, const Packets_t& projects_2d85, Packets_t& typeBases_2d84, Packets_t& projects_2d87);

protected:
	bool isInputUnique( const Udm::Object& rootFolder_2d8e, const Udm::Object& project_2d97);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& rootFolders_2d82, const Packets_t& projects_2d85);
	bool patternMatcher( const Udm::Object& rootFolder_2d8c, const Udm::Object& project_2d95);
	void effector();
	void outputAppender( const CyberComposition::Simulink::SF_TypeBase& typeBase_2daa, const SFC::Project& project_2dac);

private:
	Packets_t* _typeBase_2d88;
	Packets_t* _project_2d89;
	Packets_t _rootFolder_2d8a;
	Packets_t _project_2d93;
	class Match
	{
	public:
		CyberComposition::RootFolder rootFolder_2da6;
		SFC::Project project_2da7;
		CyberComposition::Simulink::Types types_2da8;
		CyberComposition::Simulink::SF_TypeBase typeBase_2da9;
	};

	std::list< Match> _matches;
};

class GetProject_2dbf
{
public:
	void operator()( const Packets_t& rootFolders_2dc0, const Packets_t& states_2dc3, const Packets_t& projects_2dc6, Packets_t& rootFolders_2dc2, Packets_t& states_2dc5, Packets_t& projects_2dc8);

protected:
	bool isInputUnique( const Udm::Object& rootFolder_2dd0, const Udm::Object& state_2dd9, const Udm::Object& project_2de2);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& rootFolders_2dc0, const Packets_t& states_2dc3, const Packets_t& projects_2dc6);
	bool patternMatcher( const Udm::Object& rootFolder_2dce, const Udm::Object& state_2dd7, const Udm::Object& project_2de0);
	void effector();
	void forwardInputs();

private:
	Packets_t* _rootFolder_2dc9;
	Packets_t* _state_2dca;
	Packets_t* _project_2dcb;
	Packets_t _rootFolder_2dcc;
	Packets_t _state_2dd5;
	Packets_t _project_2dde;
	class Match
	{
	public:
		CyberComposition::RootFolder rootFolder_2deb;
		CyberComposition::Simulink::State state_2dec;
		SFC::Project project_2ded;
	};

	std::list< Match> _matches;
};

class SetFileName_2dee
{
public:
	void operator()( const Packets_t& states_2def, const Packets_t& projects_2df3, Packets_t& states_2df1, Packets_t& programs_2df2);

protected:
	bool isInputUnique( const Udm::Object& state_2dfb, const Udm::Object& project_2e04);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& states_2def, const Packets_t& projects_2df3);
	bool patternMatcher( const Udm::Object& state_2df9, const Udm::Object& project_2e02);
	void effector();
	void outputAppender( const CyberComposition::Simulink::State& state_2e10, const SFC::Program& program_2e12);

private:
	Packets_t* _state_2df5;
	Packets_t* _program_2df6;
	Packets_t _state_2df7;
	Packets_t _project_2e00;
	class Match
	{
	public:
		CyberComposition::Simulink::State state_2e0d;
		SFC::Project project_2e0e;
	};

	std::list< Match> _matches;
};

class ExecTransConnectorRecurse2_2e22
{
public:
	void operator()( const Packets_t& states_2e23, const Packets_t& tcs_2e25, const Packets_t& fcns_2e27, const Packets_t& cgs_2e29);

protected:
	void executeOne( const Packets_t& states_2e23, const Packets_t& tcs_2e25, const Packets_t& fcns_2e27, const Packets_t& cgs_2e29);
	bool isInputUnique( const Udm::Object& state_2e2d, const Udm::Object& tc_2e34, const Udm::Object& fcn_2e3b, const Udm::Object& cg_2e42);
	void callProcessedFilter_374d( const Packets_t& states_368a, const Packets_t& tcs_368c, const Packets_t& fcns_368e, const Packets_t& cgs_3690);
	void callCreateTransitionCG_3752( const Packets_t& sFStates_36fd, const Packets_t& sFTransConnectors_3700, const Packets_t& functions_3703, const Packets_t& tCVarCGs_3707);
	void callExecTransitions_3757( const Packets_t& states_2fa2, const Packets_t& tcs_2fa4, const Packets_t& fcns_2fa6, const Packets_t& css_2fa8);
	void callNoDefaultTransition_375c( const Packets_t& states_2e48, const Packets_t& tcs_2e4a, const Packets_t& fcns_2e4c, const Packets_t& css_2e4e);

private:
	Packets_t _state_2e2b;
	Packets_t _tc_2e32;
	Packets_t _fcn_2e39;
	Packets_t _cg_2e40;
};

class NoDefaultTransition_2e47
{
public:
	void operator()( const Packets_t& states_2e48, const Packets_t& tcs_2e4a, const Packets_t& fcns_2e4c, const Packets_t& css_2e4e);

protected:
	void callNDTTest_2f97( const Packets_t& states_2e96, const Packets_t& tcs_2e98, const Packets_t& fcns_2e9a, const Packets_t& css_2e9c);
	void callExitLoop_2f9c( const Packets_t& sFStates_2e51, const Packets_t& sFSrcTransConnectors_2e53, const Packets_t& functions_2e55, const Packets_t& compoundStatements_2e57);
};

class ExitLoop_2e50
{
public:
	void operator()( const Packets_t& sFStates_2e51, const Packets_t& sFSrcTransConnectors_2e53, const Packets_t& functions_2e55, const Packets_t& compoundStatements_2e57);

protected:
	bool isInputUnique( const Udm::Object& sFState_2e5d, const Udm::Object& sFSrcTransConnector_2e66, const Udm::Object& function_2e6f, const Udm::Object& compoundStatement_2e78);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::CompoundStatement& CompoundStatement, SFC::Function& Function, CyberComposition::Simulink::SFTransConnector& SFSrcTransConnector, CyberComposition::Simulink::SFState& SFState, SFC::LocalVar& TCVar);
	void processInputPackets( const Packets_t& sFStates_2e51, const Packets_t& sFSrcTransConnectors_2e53, const Packets_t& functions_2e55, const Packets_t& compoundStatements_2e57);
	bool patternMatcher( const Udm::Object& sFState_2e5b, const Udm::Object& sFSrcTransConnector_2e64, const Udm::Object& function_2e6d, const Udm::Object& compoundStatement_2e76);
	void effector();

private:
	Packets_t _sFState_2e59;
	Packets_t _sFSrcTransConnector_2e62;
	Packets_t _function_2e6b;
	Packets_t _compoundStatement_2e74;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_2e84;
		CyberComposition::Simulink::SFTransConnector sFSrcTransConnector_2e85;
		SFC::Function function_2e86;
		SFC::CompoundStatement compoundStatement_2e87;
		SFC::LocalVar tCVar_2e88;
	};

	std::list< Match> _matches;
};

class NDTTest_2e95
{
public:
	void operator()( const Packets_t& states_2e96, const Packets_t& tcs_2e98, const Packets_t& fcns_2e9a, const Packets_t& css_2e9c, Packets_t& states_2e9e, Packets_t& tcs_2e9f, Packets_t& fcns_2ea0, Packets_t& css_2ea1);

protected:
	void executeOne( const Packets_t& states_2e96, const Packets_t& tcs_2e98, const Packets_t& fcns_2e9a, const Packets_t& css_2e9c);
	bool isInputUnique( const Udm::Object& state_2ea8, const Udm::Object& tc_2eaf, const Udm::Object& fcn_2eb6, const Udm::Object& cs_2ebd);

private:
	Packets_t* _state_2ea2;
	Packets_t* _tc_2ea3;
	Packets_t* _fcn_2ea4;
	Packets_t* _cs_2ea5;
	Packets_t _state_2ea6;
	Packets_t _tc_2ead;
	Packets_t _fcn_2eb4;
	Packets_t _cs_2ebb;
};

class HasDefaultTransitionDouble_2ec2
{
public:
	bool operator()( const Packets_t& sFStates_2ec3, const Packets_t& sFSrcTransConnectors_2ec5, const Packets_t& functions_2ec7, const Packets_t& compoundStatements_2ec9);

protected:
	bool isInputUnique( const Udm::Object& sFState_2ecf, const Udm::Object& sFSrcTransConnector_2ed8, const Udm::Object& function_2ee1, const Udm::Object& compoundStatement_2eea);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::CompoundStatement& CompoundStatement, SFC::ConditionalBlock& ConditionalBlock, SFC::Double& Double, SFC::Function& Function, CyberComposition::Simulink::SFTransConnector& SFSrcTransConnector, CyberComposition::Simulink::SFState& SFState, SFC::UserCode& UserCode);
	void processInputPackets( const Packets_t& sFStates_2ec3, const Packets_t& sFSrcTransConnectors_2ec5, const Packets_t& functions_2ec7, const Packets_t& compoundStatements_2ec9);
	bool patternMatcher( const Udm::Object& sFState_2ecd, const Udm::Object& sFSrcTransConnector_2ed6, const Udm::Object& function_2edf, const Udm::Object& compoundStatement_2ee8);
	void outputAppender();

private:
	Packets_t _sFState_2ecb;
	Packets_t _sFSrcTransConnector_2ed4;
	Packets_t _function_2edd;
	Packets_t _compoundStatement_2ee6;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_2efc;
		CyberComposition::Simulink::SFTransConnector sFSrcTransConnector_2efd;
		SFC::Function function_2efe;
		SFC::CompoundStatement compoundStatement_2eff;
		SFC::UserCode userCode_2f00;
		SFC::Double double_2f01;
		SFC::ConditionalBlock conditionalBlock_2f02;
	};

	std::list< Match> _matches;
};

class HasDefaultTransitionInt_2f0a
{
public:
	bool operator()( const Packets_t& sFStates_2f0b, const Packets_t& sFSrcTransConnectors_2f0d, const Packets_t& functions_2f0f, const Packets_t& compoundStatements_2f11);

protected:
	bool isInputUnique( const Udm::Object& sFState_2f17, const Udm::Object& sFSrcTransConnector_2f20, const Udm::Object& function_2f29, const Udm::Object& compoundStatement_2f32);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::CompoundStatement& CompoundStatement, SFC::ConditionalBlock& ConditionalBlock, SFC::Function& Function, SFC::Int& Int, CyberComposition::Simulink::SFTransConnector& SFSrcTransConnector, CyberComposition::Simulink::SFState& SFState, SFC::UserCode& UserCode);
	void processInputPackets( const Packets_t& sFStates_2f0b, const Packets_t& sFSrcTransConnectors_2f0d, const Packets_t& functions_2f0f, const Packets_t& compoundStatements_2f11);
	bool patternMatcher( const Udm::Object& sFState_2f15, const Udm::Object& sFSrcTransConnector_2f1e, const Udm::Object& function_2f27, const Udm::Object& compoundStatement_2f30);
	void outputAppender();

private:
	Packets_t _sFState_2f13;
	Packets_t _sFSrcTransConnector_2f1c;
	Packets_t _function_2f25;
	Packets_t _compoundStatement_2f2e;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_2f44;
		CyberComposition::Simulink::SFTransConnector sFSrcTransConnector_2f45;
		SFC::Function function_2f46;
		SFC::CompoundStatement compoundStatement_2f47;
		SFC::UserCode userCode_2f48;
		SFC::Int int_2f49;
		SFC::ConditionalBlock conditionalBlock_2f4a;
	};

	std::list< Match> _matches;
};

class NoDefault_2f52
{
public:
	bool operator()( const Packets_t& sFStates_2f53, const Packets_t& sFTransConnectors_2f56, const Packets_t& functions_2f59, const Packets_t& compoundStatements_2f5c, Packets_t& sFStates_2f55, Packets_t& sFTransConnectors_2f58, Packets_t& functions_2f5b, Packets_t& compoundStatements_2f5e);

protected:
	bool isInputUnique( const Udm::Object& sFState_2f67, const Udm::Object& sFTransConnector_2f70, const Udm::Object& function_2f79, const Udm::Object& compoundStatement_2f82);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& sFStates_2f53, const Packets_t& sFTransConnectors_2f56, const Packets_t& functions_2f59, const Packets_t& compoundStatements_2f5c);
	bool patternMatcher( const Udm::Object& sFState_2f65, const Udm::Object& sFTransConnector_2f6e, const Udm::Object& function_2f77, const Udm::Object& compoundStatement_2f80);
	void outputAppender( const CyberComposition::Simulink::SFState& sFState_2f8f, const CyberComposition::Simulink::SFTransConnector& sFTransConnector_2f91, const SFC::Function& function_2f93, const SFC::CompoundStatement& compoundStatement_2f95);

private:
	Packets_t* _sFState_2f5f;
	Packets_t* _sFTransConnector_2f60;
	Packets_t* _function_2f61;
	Packets_t* _compoundStatement_2f62;
	Packets_t _sFState_2f63;
	Packets_t _sFTransConnector_2f6c;
	Packets_t _function_2f75;
	Packets_t _compoundStatement_2f7e;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_2f8b;
		CyberComposition::Simulink::SFTransConnector sFTransConnector_2f8c;
		SFC::Function function_2f8d;
		SFC::CompoundStatement compoundStatement_2f8e;
	};

	std::list< Match> _matches;
};

class ExecTransitions_2fa1
{
public:
	void operator()( const Packets_t& states_2fa2, const Packets_t& tcs_2fa4, const Packets_t& fcns_2fa6, const Packets_t& css_2fa8, Packets_t& states_2faa, Packets_t& tcs_2fab, Packets_t& fcns_2fac, Packets_t& css_2fad);

protected:
	void callGetAllTransitions_367f( const Packets_t& states_3575, const Packets_t& tcs_3577, const Packets_t& fcns_3579, const Packets_t& css_357b);
	void callExecProcessTransitions_3684( const Packets_t& states_2fb3, const Packets_t& transs_2fb5, const Packets_t& fcns_2fb7, const Packets_t& css_2fb9);

private:
	Packets_t* _state_2fae;
	Packets_t* _tc_2faf;
	Packets_t* _fcn_2fb0;
	Packets_t* _cs_2fb1;
};

class ExecProcessTransitions_2fb2
{
public:
	void operator()( const Packets_t& states_2fb3, const Packets_t& transs_2fb5, const Packets_t& fcns_2fb7, const Packets_t& css_2fb9);

protected:
	void callOrderTransitions_353b( const Packets_t& sFStates_3387, const Packets_t& sFTransitions_338a, const Packets_t& functions_338d, const Packets_t& compoundStatements_3390);
	void callTransCond_3540( const Packets_t& sFStates_3079, const Packets_t& sFTransitions_307d, const Packets_t& functions_3080, const Packets_t& transitionCGs_3083);
	void callHighestTransParent_3545( const Packets_t& sFStates_2fbc, const Packets_t& sFDstTransConnectors_2fbf, const Packets_t& sFTransitions_2fc2, const Packets_t& functions_2fc5, const Packets_t& conditionalBlocks_2fc8);
	void callTest_354b( const Packets_t& states_30c4, const Packets_t& dsts_30c6, const Packets_t& transs_30c8, const Packets_t& fcns_30ca, const Packets_t& cbs_30cc);
	void callDeadEndJunction_3551( const Packets_t& sFStates_33c9, const Packets_t& sFTransConnectors_33cb, const Packets_t& sFTransitions_33cd, const Packets_t& functions_33cf, const Packets_t& transitionCBs_33d1);
	void callNextJunction_3557( const Packets_t& sFStates_3415, const Packets_t& sFJunctions_3418, const Packets_t& sFTransitions_341b, const Packets_t& functions_341d, const Packets_t& transitionCBs_3420);
	void callGetDstJuncFromRef_355d( const Packets_t& sFStates_3024, const Packets_t& sFConnectorRefs_3027, const Packets_t& sFTransitions_302a, const Packets_t& functions_302d, const Packets_t& conditionalBlocks_3030);
	void callExecNextState_3563( const Packets_t& sFStates_34d2, const Packets_t& sFDstStates_34d4, const Packets_t& sFTransitions_34d6, const Packets_t& functions_34d8, const Packets_t& transitionCBs_34da);
	void callGetDstStateFromRef_3569( const Packets_t& sFStates_347d, const Packets_t& sFConnectorRefs_3481, const Packets_t& sFTransitions_3483, const Packets_t& functions_3486, const Packets_t& conditionalBlocks_3489);
	void callExecTransConnectorRecurse_356f( const Packets_t& states_2e23, const Packets_t& tcs_2e25, const Packets_t& fcns_2e27, const Packets_t& cgs_2e29);
};

class HighestTransParent_2fbb
{
public:
	void operator()( const Packets_t& sFStates_2fbc, const Packets_t& sFDstTransConnectors_2fbf, const Packets_t& sFTransitions_2fc2, const Packets_t& functions_2fc5, const Packets_t& conditionalBlocks_2fc8, Packets_t& sFStates_2fbe, Packets_t& sFDstTransConnectors_2fc1, Packets_t& sFTransitions_2fc4, Packets_t& functions_2fc7, Packets_t& conditionalBlocks_2fca);

protected:
	bool isInputUnique( const Udm::Object& sFState_2fd4, const Udm::Object& sFDstTransConnector_2fdd, const Udm::Object& sFTransition_2fe6, const Udm::Object& function_2fef, const Udm::Object& conditionalBlock_2ff8);
	bool isGuardTrue( SFC::ConditionalBlock& ConditionalBlock, SFC::Function& Function, SFC::LocalVar& HTPVar, CyberComposition::Simulink::SFTransConnector& SFDstTransConnector, CyberComposition::Simulink::SFState& SFState, CyberComposition::Simulink::SFTransition& SFTransition, CyberComposition::Simulink::SFState& SFTransitionParentState, SFC::StateLabel& StateLabel);
	void processInputPackets( const Packets_t& sFStates_2fbc, const Packets_t& sFDstTransConnectors_2fbf, const Packets_t& sFTransitions_2fc2, const Packets_t& functions_2fc5, const Packets_t& conditionalBlocks_2fc8);
	bool patternMatcher( const Udm::Object& sFState_2fd2, const Udm::Object& sFDstTransConnector_2fdb, const Udm::Object& sFTransition_2fe4, const Udm::Object& function_2fed, const Udm::Object& conditionalBlock_2ff6);
	void effector();
	void forwardInputs();

private:
	Packets_t* _sFState_2fcb;
	Packets_t* _sFDstTransConnector_2fcc;
	Packets_t* _sFTransition_2fcd;
	Packets_t* _function_2fce;
	Packets_t* _conditionalBlock_2fcf;
	Packets_t _sFState_2fd0;
	Packets_t _sFDstTransConnector_2fd9;
	Packets_t _sFTransition_2fe2;
	Packets_t _function_2feb;
	Packets_t _conditionalBlock_2ff4;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_3006;
		CyberComposition::Simulink::SFTransConnector sFDstTransConnector_3007;
		CyberComposition::Simulink::SFTransition sFTransition_3008;
		SFC::Function function_3009;
		SFC::ConditionalBlock conditionalBlock_300a;
		CyberComposition::Simulink::SFState sFTransitionParentState_300b;
		SFC::StateLabel stateLabel_300c;
		SFC::LocalVar hTPVar_300d;
	};

	std::list< Match> _matches;
};

class GetDstJuncFromRef_3023
{
public:
	void operator()( const Packets_t& sFStates_3024, const Packets_t& sFConnectorRefs_3027, const Packets_t& sFTransitions_302a, const Packets_t& functions_302d, const Packets_t& conditionalBlocks_3030, Packets_t& sFStates_3026, Packets_t& sFJunctions_3029, Packets_t& sFTransitions_302c, Packets_t& functions_302f, Packets_t& conditionalBlocks_3032);

protected:
	bool isInputUnique( const Udm::Object& sFState_303c, const Udm::Object& sFConnectorRef_3045, const Udm::Object& sFTransition_304e, const Udm::Object& function_3057, const Udm::Object& conditionalBlock_3060);
	void processInputPackets( const Packets_t& sFStates_3024, const Packets_t& sFConnectorRefs_3027, const Packets_t& sFTransitions_302a, const Packets_t& functions_302d, const Packets_t& conditionalBlocks_3030);
	bool patternMatcher( const Udm::Object& sFState_303a, const Udm::Object& sFConnectorRef_3043, const Udm::Object& sFTransition_304c, const Udm::Object& function_3055, const Udm::Object& conditionalBlock_305e);
	void effector();
	void outputAppender( const CyberComposition::Simulink::SFState& sFState_306e, const CyberComposition::Simulink::SFJunction& sFJunction_3070, const CyberComposition::Simulink::SFTransition& sFTransition_3072, const SFC::Function& function_3074, const SFC::ConditionalBlock& conditionalBlock_3076);

private:
	Packets_t* _sFState_3033;
	Packets_t* _sFJunction_3034;
	Packets_t* _sFTransition_3035;
	Packets_t* _function_3036;
	Packets_t* _conditionalBlock_3037;
	Packets_t _sFState_3038;
	Packets_t _sFConnectorRef_3041;
	Packets_t _sFTransition_304a;
	Packets_t _function_3053;
	Packets_t _conditionalBlock_305c;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_3068;
		CyberComposition::Simulink::SFConnectorRef sFConnectorRef_3069;
		CyberComposition::Simulink::SFTransition sFTransition_306a;
		SFC::Function function_306b;
		SFC::ConditionalBlock conditionalBlock_306c;
		CyberComposition::Simulink::SFJunction sFJunction_306d;
	};

	std::list< Match> _matches;
};

class TransCond_3078
{
public:
	void operator()( const Packets_t& sFStates_3079, const Packets_t& sFTransitions_307d, const Packets_t& functions_3080, const Packets_t& transitionCGs_3083, Packets_t& sFStates_307b, Packets_t& sFDstTransConnectors_307c, Packets_t& sFTransitions_307f, Packets_t& functions_3082, Packets_t& conditionalBlocks_3085);

protected:
	bool isInputUnique( const Udm::Object& sFState_308f, const Udm::Object& sFTransition_3098, const Udm::Object& function_30a1, const Udm::Object& transitionCG_30aa);
	void processInputPackets( const Packets_t& sFStates_3079, const Packets_t& sFTransitions_307d, const Packets_t& functions_3080, const Packets_t& transitionCGs_3083);
	bool patternMatcher( const Udm::Object& sFState_308d, const Udm::Object& sFTransition_3096, const Udm::Object& function_309f, const Udm::Object& transitionCG_30a8);
	void effector();
	void outputAppender( const CyberComposition::Simulink::SFState& sFState_30b9, const CyberComposition::Simulink::SFTransConnector& sFDstTransConnector_30bb, const CyberComposition::Simulink::SFTransition& sFTransition_30bd, const SFC::Function& function_30bf, const SFC::ConditionalBlock& conditionalBlock_30c1);

private:
	Packets_t* _sFState_3086;
	Packets_t* _sFDstTransConnector_3087;
	Packets_t* _sFTransition_3088;
	Packets_t* _function_3089;
	Packets_t* _conditionalBlock_308a;
	Packets_t _sFState_308b;
	Packets_t _sFTransition_3094;
	Packets_t _function_309d;
	Packets_t _transitionCG_30a6;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_30b2;
		CyberComposition::Simulink::SFTransition sFTransition_30b3;
		SFC::Function function_30b4;
		SFC::ConditionalGroup transitionCG_30b5;
		CyberComposition::Simulink::SFTransConnector sFDstTransConnector_30b6;
		CyberComposition::Simulink::SFTransConnector sFSrcTransConnector_30b7;
	};

	std::list< Match> _matches;
};

class Test_30c3
{
public:
	void operator()( const Packets_t& states_30c4, const Packets_t& dsts_30c6, const Packets_t& transs_30c8, const Packets_t& fcns_30ca, const Packets_t& cbs_30cc, Packets_t& states_30ce, Packets_t& dsts_30cf, Packets_t& transs_30d0, Packets_t& fcns_30d1, Packets_t& cbs_30d2, Packets_t& states_30d3, Packets_t& dsts_30d4, Packets_t& transs_30d5, Packets_t& fcns_30d6, Packets_t& cbs_30d7, Packets_t& states_30d8, Packets_t& dsts_30d9, Packets_t& transs_30da, Packets_t& fcns_30db, Packets_t& cbs_30dc, Packets_t& states_30dd, Packets_t& dsts_30de, Packets_t& transs_30df, Packets_t& fcns_30e0, Packets_t& cbs_30e1, Packets_t& states_30e2, Packets_t& dsts_30e3, Packets_t& transs_30e4, Packets_t& fcns_30e5, Packets_t& cbs_30e6);

protected:
	void executeOne( const Packets_t& states_30c4, const Packets_t& dsts_30c6, const Packets_t& transs_30c8, const Packets_t& fcns_30ca, const Packets_t& cbs_30cc);
	bool isInputUnique( const Udm::Object& state_3102, const Udm::Object& dst_3109, const Udm::Object& trans_3110, const Udm::Object& fcn_3117, const Udm::Object& cb_311e);

private:
	Packets_t* _state_30e7;
	Packets_t* _dst_30e8;
	Packets_t* _trans_30e9;
	Packets_t* _fcn_30ea;
	Packets_t* _cb_30eb;
	Packets_t* _state_30ec;
	Packets_t* _dst_30ed;
	Packets_t* _trans_30ee;
	Packets_t* _fcn_30ef;
	Packets_t* _cb_30f0;
	Packets_t* _state_30f1;
	Packets_t* _dst_30f2;
	Packets_t* _trans_30f3;
	Packets_t* _fcn_30f4;
	Packets_t* _cb_30f5;
	Packets_t* _state_30f6;
	Packets_t* _dst_30f7;
	Packets_t* _trans_30f8;
	Packets_t* _fcn_30f9;
	Packets_t* _cb_30fa;
	Packets_t* _state_30fb;
	Packets_t* _dst_30fc;
	Packets_t* _trans_30fd;
	Packets_t* _fcn_30fe;
	Packets_t* _cb_30ff;
	Packets_t _state_3100;
	Packets_t _dst_3107;
	Packets_t _trans_310e;
	Packets_t _fcn_3115;
	Packets_t _cb_311c;
};

class DstIsState_3123
{
public:
	bool operator()( const Packets_t& sFStates_3124, const Packets_t& sFDstStates_3127, const Packets_t& sFTransitions_312a, const Packets_t& functions_312d, const Packets_t& conditionalBlocks_3130, Packets_t& sFStates_3126, Packets_t& sFDstStates_3129, Packets_t& sFTransitions_312c, Packets_t& functions_312f, Packets_t& conditionalBlocks_3132);

protected:
	bool isInputUnique( const Udm::Object& sFState_313c, const Udm::Object& sFDstState_3145, const Udm::Object& sFTransition_314e, const Udm::Object& function_3157, const Udm::Object& conditionalBlock_3160);
	void processInputPackets( const Packets_t& sFStates_3124, const Packets_t& sFDstStates_3127, const Packets_t& sFTransitions_312a, const Packets_t& functions_312d, const Packets_t& conditionalBlocks_3130);
	bool patternMatcher( const Udm::Object& sFState_313a, const Udm::Object& sFDstState_3143, const Udm::Object& sFTransition_314c, const Udm::Object& function_3155, const Udm::Object& conditionalBlock_315e);
	void outputAppender( const CyberComposition::Simulink::SFState& sFState_316b, const CyberComposition::Simulink::SFState& sFDstState_316d, const CyberComposition::Simulink::SFTransition& sFTransition_316f, const SFC::Function& function_3171, const SFC::ConditionalBlock& conditionalBlock_3173);

private:
	Packets_t* _sFState_3133;
	Packets_t* _sFDstState_3134;
	Packets_t* _sFTransition_3135;
	Packets_t* _function_3136;
	Packets_t* _conditionalBlock_3137;
	Packets_t _sFState_3138;
	Packets_t _sFDstState_3141;
	Packets_t _sFTransition_314a;
	Packets_t _function_3153;
	Packets_t _conditionalBlock_315c;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_3166;
		CyberComposition::Simulink::SFState sFDstState_3167;
		CyberComposition::Simulink::SFTransition sFTransition_3168;
		SFC::Function function_3169;
		SFC::ConditionalBlock conditionalBlock_316a;
	};

	std::list< Match> _matches;
};

class DstIsRefState_3175
{
public:
	bool operator()( const Packets_t& sFStates_3176, const Packets_t& sFConnectorRefs_3179, const Packets_t& sFTransitions_317c, const Packets_t& functions_317f, const Packets_t& conditionalBlocks_3182, Packets_t& sFStates_3178, Packets_t& sFConnectorRefs_317b, Packets_t& sFTransitions_317e, Packets_t& functions_3181, Packets_t& conditionalBlocks_3184);

protected:
	bool isInputUnique( const Udm::Object& sFState_318e, const Udm::Object& sFConnectorRef_3197, const Udm::Object& sFTransition_31a0, const Udm::Object& function_31a9, const Udm::Object& conditionalBlock_31b2);
	void processInputPackets( const Packets_t& sFStates_3176, const Packets_t& sFConnectorRefs_3179, const Packets_t& sFTransitions_317c, const Packets_t& functions_317f, const Packets_t& conditionalBlocks_3182);
	bool patternMatcher( const Udm::Object& sFState_318c, const Udm::Object& sFConnectorRef_3195, const Udm::Object& sFTransition_319e, const Udm::Object& function_31a7, const Udm::Object& conditionalBlock_31b0);
	void outputAppender( const CyberComposition::Simulink::SFState& sFState_31c0, const CyberComposition::Simulink::SFConnectorRef& sFConnectorRef_31c2, const CyberComposition::Simulink::SFTransition& sFTransition_31c4, const SFC::Function& function_31c6, const SFC::ConditionalBlock& conditionalBlock_31c8);

private:
	Packets_t* _sFState_3185;
	Packets_t* _sFConnectorRef_3186;
	Packets_t* _sFTransition_3187;
	Packets_t* _function_3188;
	Packets_t* _conditionalBlock_3189;
	Packets_t _sFState_318a;
	Packets_t _sFConnectorRef_3193;
	Packets_t _sFTransition_319c;
	Packets_t _function_31a5;
	Packets_t _conditionalBlock_31ae;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_31ba;
		CyberComposition::Simulink::SFConnectorRef sFConnectorRef_31bb;
		CyberComposition::Simulink::SFTransition sFTransition_31bc;
		SFC::Function function_31bd;
		SFC::ConditionalBlock conditionalBlock_31be;
		CyberComposition::Simulink::SFState sFDstState_31bf;
	};

	std::list< Match> _matches;
};

class DstIsJuncWithTrans_31ca
{
public:
	bool operator()( const Packets_t& sFStates_31cb, const Packets_t& sFJunctions_31ce, const Packets_t& sFTransitions_31d1, const Packets_t& functions_31d4, const Packets_t& conditionalBlocks_31d7, Packets_t& sFStates_31cd, Packets_t& sFJunctions_31d0, Packets_t& sFTransitions_31d3, Packets_t& functions_31d6, Packets_t& conditionalBlocks_31d9);

protected:
	bool isInputUnique( const Udm::Object& sFState_31e3, const Udm::Object& sFJunction_31ec, const Udm::Object& sFTransition_31f5, const Udm::Object& function_31fe, const Udm::Object& conditionalBlock_3207);
	void processInputPackets( const Packets_t& sFStates_31cb, const Packets_t& sFJunctions_31ce, const Packets_t& sFTransitions_31d1, const Packets_t& functions_31d4, const Packets_t& conditionalBlocks_31d7);
	bool patternMatcher( const Udm::Object& sFState_31e1, const Udm::Object& sFJunction_31ea, const Udm::Object& sFTransition_31f3, const Udm::Object& function_31fc, const Udm::Object& conditionalBlock_3205);
	void outputAppender( const CyberComposition::Simulink::SFState& sFState_3218, const CyberComposition::Simulink::SFJunction& sFJunction_321a, const CyberComposition::Simulink::SFTransition& sFTransition_321c, const SFC::Function& function_321e, const SFC::ConditionalBlock& conditionalBlock_3220);

private:
	Packets_t* _sFState_31da;
	Packets_t* _sFJunction_31db;
	Packets_t* _sFTransition_31dc;
	Packets_t* _function_31dd;
	Packets_t* _conditionalBlock_31de;
	Packets_t _sFState_31df;
	Packets_t _sFJunction_31e8;
	Packets_t _sFTransition_31f1;
	Packets_t _function_31fa;
	Packets_t _conditionalBlock_3203;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_3211;
		CyberComposition::Simulink::SFJunction sFJunction_3212;
		CyberComposition::Simulink::SFTransition sFTransition_3213;
		SFC::Function function_3214;
		SFC::ConditionalBlock conditionalBlock_3215;
		CyberComposition::Simulink::SFTransition sFTransition2_3216;
		CyberComposition::Simulink::SFTransConnector sFTransConnector_3217;
	};

	std::list< Match> _matches;
};

class DstIsJuncWithRefTrans_3222
{
public:
	bool operator()( const Packets_t& sFStates_3223, const Packets_t& sFJunctions_3226, const Packets_t& sFTransitions_3229, const Packets_t& functions_322c, const Packets_t& conditionalBlocks_322f, Packets_t& sFStates_3225, Packets_t& sFJunctions_3228, Packets_t& sFTransitions_322b, Packets_t& functions_322e, Packets_t& conditionalBlocks_3231);

protected:
	bool isInputUnique( const Udm::Object& sFState_323b, const Udm::Object& sFJunction_3244, const Udm::Object& sFTransition_324d, const Udm::Object& function_3256, const Udm::Object& conditionalBlock_325f);
	void processInputPackets( const Packets_t& sFStates_3223, const Packets_t& sFJunctions_3226, const Packets_t& sFTransitions_3229, const Packets_t& functions_322c, const Packets_t& conditionalBlocks_322f);
	bool patternMatcher( const Udm::Object& sFState_3239, const Udm::Object& sFJunction_3242, const Udm::Object& sFTransition_324b, const Udm::Object& function_3254, const Udm::Object& conditionalBlock_325d);
	void outputAppender( const CyberComposition::Simulink::SFState& sFState_3274, const CyberComposition::Simulink::SFJunction& sFJunction_3276, const CyberComposition::Simulink::SFTransition& sFTransition_3278, const SFC::Function& function_327a, const SFC::ConditionalBlock& conditionalBlock_327c);

private:
	Packets_t* _sFState_3232;
	Packets_t* _sFJunction_3233;
	Packets_t* _sFTransition_3234;
	Packets_t* _function_3235;
	Packets_t* _conditionalBlock_3236;
	Packets_t _sFState_3237;
	Packets_t _sFJunction_3240;
	Packets_t _sFTransition_3249;
	Packets_t _function_3252;
	Packets_t _conditionalBlock_325b;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_326c;
		CyberComposition::Simulink::SFJunction sFJunction_326d;
		CyberComposition::Simulink::SFTransition sFTransition_326e;
		SFC::Function function_326f;
		SFC::ConditionalBlock conditionalBlock_3270;
		CyberComposition::Simulink::SFConnectorRef sFConnectorRef_3271;
		CyberComposition::Simulink::SFTransition sFTransition2_3272;
		CyberComposition::Simulink::SFTransConnector sFTransConnector_3273;
	};

	std::list< Match> _matches;
};

class DstIsJuncRefWithTrans_327e
{
public:
	bool operator()( const Packets_t& sFStates_327f, const Packets_t& sFConnectorRefs_3282, const Packets_t& sFTransitions_3285, const Packets_t& functions_3288, const Packets_t& conditionalBlocks_328b, Packets_t& sFStates_3281, Packets_t& sFConnectorRefs_3284, Packets_t& sFTransitions_3287, Packets_t& functions_328a, Packets_t& conditionalBlocks_328d);

protected:
	bool isInputUnique( const Udm::Object& sFState_3297, const Udm::Object& sFConnectorRef_32a0, const Udm::Object& sFTransition_32a9, const Udm::Object& function_32b2, const Udm::Object& conditionalBlock_32bb);
	void processInputPackets( const Packets_t& sFStates_327f, const Packets_t& sFConnectorRefs_3282, const Packets_t& sFTransitions_3285, const Packets_t& functions_3288, const Packets_t& conditionalBlocks_328b);
	bool patternMatcher( const Udm::Object& sFState_3295, const Udm::Object& sFConnectorRef_329e, const Udm::Object& sFTransition_32a7, const Udm::Object& function_32b0, const Udm::Object& conditionalBlock_32b9);
	void outputAppender( const CyberComposition::Simulink::SFState& sFState_32cf, const CyberComposition::Simulink::SFConnectorRef& sFConnectorRef_32d1, const CyberComposition::Simulink::SFTransition& sFTransition_32d3, const SFC::Function& function_32d5, const SFC::ConditionalBlock& conditionalBlock_32d7);

private:
	Packets_t* _sFState_328e;
	Packets_t* _sFConnectorRef_328f;
	Packets_t* _sFTransition_3290;
	Packets_t* _function_3291;
	Packets_t* _conditionalBlock_3292;
	Packets_t _sFState_3293;
	Packets_t _sFConnectorRef_329c;
	Packets_t _sFTransition_32a5;
	Packets_t _function_32ae;
	Packets_t _conditionalBlock_32b7;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_32c7;
		CyberComposition::Simulink::SFConnectorRef sFConnectorRef_32c8;
		CyberComposition::Simulink::SFTransition sFTransition_32c9;
		SFC::Function function_32ca;
		SFC::ConditionalBlock conditionalBlock_32cb;
		CyberComposition::Simulink::SFJunction sFJunction_32cc;
		CyberComposition::Simulink::SFTransition sFTransition2_32cd;
		CyberComposition::Simulink::SFTransConnector sFTransConnector_32ce;
	};

	std::list< Match> _matches;
};

class DstIsJuncRefWithJuncTrans_32d9
{
public:
	bool operator()( const Packets_t& sFStates_32da, const Packets_t& sFConnectorRefs_32dd, const Packets_t& sFTransitions_32e0, const Packets_t& functions_32e3, const Packets_t& conditionalBlocks_32e6, Packets_t& sFStates_32dc, Packets_t& sFConnectorRefs_32df, Packets_t& sFTransitions_32e2, Packets_t& functions_32e5, Packets_t& conditionalBlocks_32e8);

protected:
	bool isInputUnique( const Udm::Object& sFState_32f2, const Udm::Object& sFConnectorRef_32fb, const Udm::Object& sFTransition_3304, const Udm::Object& function_330d, const Udm::Object& conditionalBlock_3316);
	void processInputPackets( const Packets_t& sFStates_32da, const Packets_t& sFConnectorRefs_32dd, const Packets_t& sFTransitions_32e0, const Packets_t& functions_32e3, const Packets_t& conditionalBlocks_32e6);
	bool patternMatcher( const Udm::Object& sFState_32f0, const Udm::Object& sFConnectorRef_32f9, const Udm::Object& sFTransition_3302, const Udm::Object& function_330b, const Udm::Object& conditionalBlock_3314);
	void outputAppender( const CyberComposition::Simulink::SFState& sFState_332a, const CyberComposition::Simulink::SFConnectorRef& sFConnectorRef_332c, const CyberComposition::Simulink::SFTransition& sFTransition_332e, const SFC::Function& function_3330, const SFC::ConditionalBlock& conditionalBlock_3332);

private:
	Packets_t* _sFState_32e9;
	Packets_t* _sFConnectorRef_32ea;
	Packets_t* _sFTransition_32eb;
	Packets_t* _function_32ec;
	Packets_t* _conditionalBlock_32ed;
	Packets_t _sFState_32ee;
	Packets_t _sFConnectorRef_32f7;
	Packets_t _sFTransition_3300;
	Packets_t _function_3309;
	Packets_t _conditionalBlock_3312;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_3322;
		CyberComposition::Simulink::SFConnectorRef sFConnectorRef_3323;
		CyberComposition::Simulink::SFTransition sFTransition_3324;
		SFC::Function function_3325;
		SFC::ConditionalBlock conditionalBlock_3326;
		CyberComposition::Simulink::SFJunction sFJunction_3327;
		CyberComposition::Simulink::SFTransition sFTransition2_3328;
		CyberComposition::Simulink::SFTransConnector sFTransConnector_3329;
	};

	std::list< Match> _matches;
};

class DeadEnd_3334
{
public:
	bool operator()( const Packets_t& sFStates_3335, const Packets_t& sFTransConnectors_3338, const Packets_t& sFTransitions_333b, const Packets_t& functions_333e, const Packets_t& conditionalBlocks_3341, Packets_t& sFStates_3337, Packets_t& sFTransConnectors_333a, Packets_t& sFTransitions_333d, Packets_t& functions_3340, Packets_t& conditionalBlocks_3343);

protected:
	bool isInputUnique( const Udm::Object& sFState_334d, const Udm::Object& sFTransConnector_3356, const Udm::Object& sFTransition_335f, const Udm::Object& function_3368, const Udm::Object& conditionalBlock_3371);
	void processInputPackets( const Packets_t& sFStates_3335, const Packets_t& sFTransConnectors_3338, const Packets_t& sFTransitions_333b, const Packets_t& functions_333e, const Packets_t& conditionalBlocks_3341);
	bool patternMatcher( const Udm::Object& sFState_334b, const Udm::Object& sFTransConnector_3354, const Udm::Object& sFTransition_335d, const Udm::Object& function_3366, const Udm::Object& conditionalBlock_336f);
	void outputAppender( const CyberComposition::Simulink::SFState& sFState_337c, const CyberComposition::Simulink::SFTransConnector& sFTransConnector_337e, const CyberComposition::Simulink::SFTransition& sFTransition_3380, const SFC::Function& function_3382, const SFC::ConditionalBlock& conditionalBlock_3384);

private:
	Packets_t* _sFState_3344;
	Packets_t* _sFTransConnector_3345;
	Packets_t* _sFTransition_3346;
	Packets_t* _function_3347;
	Packets_t* _conditionalBlock_3348;
	Packets_t _sFState_3349;
	Packets_t _sFTransConnector_3352;
	Packets_t _sFTransition_335b;
	Packets_t _function_3364;
	Packets_t _conditionalBlock_336d;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_3377;
		CyberComposition::Simulink::SFTransConnector sFTransConnector_3378;
		CyberComposition::Simulink::SFTransition sFTransition_3379;
		SFC::Function function_337a;
		SFC::ConditionalBlock conditionalBlock_337b;
	};

	std::list< Match> _matches;
};

class OrderTransitions_3386
{
public:
	void operator()( const Packets_t& sFStates_3387, const Packets_t& sFTransitions_338a, const Packets_t& functions_338d, const Packets_t& compoundStatements_3390, Packets_t& sFStates_3389, Packets_t& sFTransitions_338c, Packets_t& functions_338f, Packets_t& compoundStatements_3392);

protected:
	bool isInputUnique( const Udm::Object& sFState_339b, const Udm::Object& sFTransition_33a4, const Udm::Object& function_33ad, const Udm::Object& compoundStatement_33b6);
	void processInputPackets( const Packets_t& sFStates_3387, const Packets_t& sFTransitions_338a, const Packets_t& functions_338d, const Packets_t& compoundStatements_3390);
	bool patternMatcher( const Udm::Object& sFState_3399, const Udm::Object& sFTransition_33a2, const Udm::Object& function_33ab, const Udm::Object& compoundStatement_33b4);
	void effector();
	void outputAppender( const CyberComposition::Simulink::SFState& sFState_33c0, const CyberComposition::Simulink::SFTransition& sFTransition_33c2, const SFC::Function& function_33c4, const SFC::CompoundStatement& compoundStatement_33c6);
	void sortOutputs();

private:
	Packets_t* _sFState_3393;
	Packets_t* _sFTransition_3394;
	Packets_t* _function_3395;
	Packets_t* _compoundStatement_3396;
	Packets_t _sFState_3397;
	Packets_t _sFTransition_33a0;
	Packets_t _function_33a9;
	Packets_t _compoundStatement_33b2;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_33bc;
		CyberComposition::Simulink::SFTransition sFTransition_33bd;
		SFC::Function function_33be;
		SFC::CompoundStatement compoundStatement_33bf;
	};

	std::list< Match> _matches;
};

class DeadEndJunction_33c8
{
public:
	void operator()( const Packets_t& sFStates_33c9, const Packets_t& sFTransConnectors_33cb, const Packets_t& sFTransitions_33cd, const Packets_t& functions_33cf, const Packets_t& transitionCBs_33d1);

protected:
	bool isInputUnique( const Udm::Object& sFState_33d7, const Udm::Object& sFTransConnector_33e0, const Udm::Object& sFTransition_33e9, const Udm::Object& function_33f2, const Udm::Object& transitionCB_33fb);
	bool isGuardTrue( SFC::Function& Function, CyberComposition::Simulink::SFState& SFState, CyberComposition::Simulink::SFTransConnector& SFTransConnector, CyberComposition::Simulink::SFTransition& SFTransition, SFC::LocalVar& TCVar, SFC::ConditionalBlock& TransitionCB);
	void processInputPackets( const Packets_t& sFStates_33c9, const Packets_t& sFTransConnectors_33cb, const Packets_t& sFTransitions_33cd, const Packets_t& functions_33cf, const Packets_t& transitionCBs_33d1);
	bool patternMatcher( const Udm::Object& sFState_33d5, const Udm::Object& sFTransConnector_33de, const Udm::Object& sFTransition_33e7, const Udm::Object& function_33f0, const Udm::Object& transitionCB_33f9);
	void effector();

private:
	Packets_t _sFState_33d3;
	Packets_t _sFTransConnector_33dc;
	Packets_t _sFTransition_33e5;
	Packets_t _function_33ee;
	Packets_t _transitionCB_33f7;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_3404;
		CyberComposition::Simulink::SFTransConnector sFTransConnector_3405;
		CyberComposition::Simulink::SFTransition sFTransition_3406;
		SFC::Function function_3407;
		SFC::ConditionalBlock transitionCB_3408;
		SFC::LocalVar tCVar_3409;
	};

	std::list< Match> _matches;
};

class NextJunction_3414
{
public:
	void operator()( const Packets_t& sFStates_3415, const Packets_t& sFJunctions_3418, const Packets_t& sFTransitions_341b, const Packets_t& functions_341d, const Packets_t& transitionCBs_3420, Packets_t& sFStates_3417, Packets_t& sFJunctions_341a, Packets_t& functions_341f, Packets_t& conditionalGroups_3422);

protected:
	bool isInputUnique( const Udm::Object& sFState_342b, const Udm::Object& sFJunction_3434, const Udm::Object& sFTransition_343d, const Udm::Object& function_3446, const Udm::Object& transitionCB_344f);
	bool isGuardTrue( SFC::ConditionalBlock& ConditionalBlock, SFC::ConditionalGroup& ConditionalGroup, SFC::Function& Function, CyberComposition::Simulink::SFJunction& SFJunction, CyberComposition::Simulink::SFState& SFState, CyberComposition::Simulink::SFTransition& SFTransition, SFC::LocalVar& TCVar, SFC::ConditionalBlock& TransitionCB, SFC::ConditionalGroup& TransitionCG);
	void processInputPackets( const Packets_t& sFStates_3415, const Packets_t& sFJunctions_3418, const Packets_t& sFTransitions_341b, const Packets_t& functions_341d, const Packets_t& transitionCBs_3420);
	bool patternMatcher( const Udm::Object& sFState_3429, const Udm::Object& sFJunction_3432, const Udm::Object& sFTransition_343b, const Udm::Object& function_3444, const Udm::Object& transitionCB_344d);
	void effector();
	void outputAppender( const CyberComposition::Simulink::SFState& sFState_3474, const CyberComposition::Simulink::SFJunction& sFJunction_3476, const SFC::Function& function_3478, const SFC::ConditionalGroup& conditionalGroup_347a);

private:
	Packets_t* _sFState_3423;
	Packets_t* _sFJunction_3424;
	Packets_t* _function_3425;
	Packets_t* _conditionalGroup_3426;
	Packets_t _sFState_3427;
	Packets_t _sFJunction_3430;
	Packets_t _sFTransition_3439;
	Packets_t _function_3442;
	Packets_t _transitionCB_344b;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_345e;
		CyberComposition::Simulink::SFJunction sFJunction_345f;
		CyberComposition::Simulink::SFTransition sFTransition_3460;
		SFC::Function function_3461;
		SFC::ConditionalBlock transitionCB_3462;
		SFC::LocalVar tCVar_3463;
		SFC::ConditionalGroup conditionalGroup_3464;
		SFC::ConditionalGroup transitionCG_3465;
		SFC::ConditionalBlock conditionalBlock_3466;
	};

	std::list< Match> _matches;
};

class GetDstStateFromRef_347c
{
public:
	void operator()( const Packets_t& sFStates_347d, const Packets_t& sFConnectorRefs_3481, const Packets_t& sFTransitions_3483, const Packets_t& functions_3486, const Packets_t& conditionalBlocks_3489, Packets_t& sFStates_347f, Packets_t& sFDstStates_3480, Packets_t& sFTransitions_3485, Packets_t& functions_3488, Packets_t& conditionalBlocks_348b);

protected:
	bool isInputUnique( const Udm::Object& sFState_3495, const Udm::Object& sFConnectorRef_349e, const Udm::Object& sFTransition_34a7, const Udm::Object& function_34b0, const Udm::Object& conditionalBlock_34b9);
	void processInputPackets( const Packets_t& sFStates_347d, const Packets_t& sFConnectorRefs_3481, const Packets_t& sFTransitions_3483, const Packets_t& functions_3486, const Packets_t& conditionalBlocks_3489);
	bool patternMatcher( const Udm::Object& sFState_3493, const Udm::Object& sFConnectorRef_349c, const Udm::Object& sFTransition_34a5, const Udm::Object& function_34ae, const Udm::Object& conditionalBlock_34b7);
	void effector();
	void outputAppender( const CyberComposition::Simulink::SFState& sFState_34c7, const CyberComposition::Simulink::SFState& sFDstState_34c9, const CyberComposition::Simulink::SFTransition& sFTransition_34cb, const SFC::Function& function_34cd, const SFC::ConditionalBlock& conditionalBlock_34cf);

private:
	Packets_t* _sFState_348c;
	Packets_t* _sFDstState_348d;
	Packets_t* _sFTransition_348e;
	Packets_t* _function_348f;
	Packets_t* _conditionalBlock_3490;
	Packets_t _sFState_3491;
	Packets_t _sFConnectorRef_349a;
	Packets_t _sFTransition_34a3;
	Packets_t _function_34ac;
	Packets_t _conditionalBlock_34b5;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_34c1;
		CyberComposition::Simulink::SFConnectorRef sFConnectorRef_34c2;
		CyberComposition::Simulink::SFTransition sFTransition_34c3;
		SFC::Function function_34c4;
		SFC::ConditionalBlock conditionalBlock_34c5;
		CyberComposition::Simulink::SFState sFDstState_34c6;
	};

	std::list< Match> _matches;
};

class ExecNextState_34d1
{
public:
	void operator()( const Packets_t& sFStates_34d2, const Packets_t& sFDstStates_34d4, const Packets_t& sFTransitions_34d6, const Packets_t& functions_34d8, const Packets_t& transitionCBs_34da);

protected:
	bool isInputUnique( const Udm::Object& sFState_34e0, const Udm::Object& sFDstState_34e9, const Udm::Object& sFTransition_34f2, const Udm::Object& function_34fb, const Udm::Object& transitionCB_3504);
	bool isGuardTrue( SFC::Function& Enter, SFC::Arg& EnterArg0, SFC::Arg& EnterArg1, SFC::Function& Exit, SFC::Arg& ExitArg, SFC::Function& Function, SFC::LocalVar& HTPVar, CyberComposition::Simulink::SFState& SFDstState, CyberComposition::Simulink::SFState& SFState, CyberComposition::Simulink::SFTransition& SFTransition, SFC::ConditionalBlock& TransitionCB);
	void processInputPackets( const Packets_t& sFStates_34d2, const Packets_t& sFDstStates_34d4, const Packets_t& sFTransitions_34d6, const Packets_t& functions_34d8, const Packets_t& transitionCBs_34da);
	bool patternMatcher( const Udm::Object& sFState_34de, const Udm::Object& sFDstState_34e7, const Udm::Object& sFTransition_34f0, const Udm::Object& function_34f9, const Udm::Object& transitionCB_3502);
	void effector();

private:
	Packets_t _sFState_34dc;
	Packets_t _sFDstState_34e5;
	Packets_t _sFTransition_34ee;
	Packets_t _function_34f7;
	Packets_t _transitionCB_3500;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_351c;
		CyberComposition::Simulink::SFState sFDstState_351d;
		CyberComposition::Simulink::SFTransition sFTransition_351e;
		SFC::Function function_351f;
		SFC::ConditionalBlock transitionCB_3520;
		SFC::Arg exitArg_3521;
		SFC::Function exit_3522;
		SFC::Arg enterArg0_3523;
		SFC::Function enter_3524;
		SFC::Arg enterArg1_3525;
		SFC::LocalVar hTPVar_3526;
	};

	std::list< Match> _matches;
};

class GetAllTransitions_3574
{
public:
	void operator()( const Packets_t& states_3575, const Packets_t& tcs_3577, const Packets_t& fcns_3579, const Packets_t& css_357b, Packets_t& states_357d, Packets_t& transs_357e, Packets_t& fcns_357f, Packets_t& css_3580);

protected:
	void callGetTransitionsFromJunctionRefs_3670( const Packets_t& sFStates_3586, const Packets_t& sFJunctions_358a, const Packets_t& functions_358c, const Packets_t& compoundStatements_358f);
	void callGetDirectTransitions_3675( const Packets_t& sFStates_35d2, const Packets_t& sFSrcTransConnectors_35d5, const Packets_t& functions_35d8, const Packets_t& compoundStatements_35db);
	void callGetTransitionsFromStateRefs_367a( const Packets_t& sFStates_361a, const Packets_t& sFTCStates_361e, const Packets_t& functions_3620, const Packets_t& compoundStatements_3623);

private:
	Packets_t* _state_3581;
	Packets_t* _trans_3582;
	Packets_t* _fcn_3583;
	Packets_t* _cs_3584;
};

class GetTransitionsFromJunctionRefs_3585
{
public:
	void operator()( const Packets_t& sFStates_3586, const Packets_t& sFJunctions_358a, const Packets_t& functions_358c, const Packets_t& compoundStatements_358f, Packets_t& sFStates_3588, Packets_t& sFTransitions_3589, Packets_t& functions_358e, Packets_t& compoundStatements_3591);

protected:
	bool isInputUnique( const Udm::Object& sFState_359a, const Udm::Object& sFJunction_35a3, const Udm::Object& function_35ac, const Udm::Object& compoundStatement_35b5);
	void processInputPackets( const Packets_t& sFStates_3586, const Packets_t& sFJunctions_358a, const Packets_t& functions_358c, const Packets_t& compoundStatements_358f);
	bool patternMatcher( const Udm::Object& sFState_3598, const Udm::Object& sFJunction_35a1, const Udm::Object& function_35aa, const Udm::Object& compoundStatement_35b3);
	void effector();
	void outputAppender( const CyberComposition::Simulink::SFState& sFState_35c9, const CyberComposition::Simulink::SFTransition& sFTransition_35cb, const SFC::Function& function_35cd, const SFC::CompoundStatement& compoundStatement_35cf);

private:
	Packets_t* _sFState_3592;
	Packets_t* _sFTransition_3593;
	Packets_t* _function_3594;
	Packets_t* _compoundStatement_3595;
	Packets_t _sFState_3596;
	Packets_t _sFJunction_359f;
	Packets_t _function_35a8;
	Packets_t _compoundStatement_35b1;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_35c2;
		CyberComposition::Simulink::SFJunction sFJunction_35c3;
		SFC::Function function_35c4;
		SFC::CompoundStatement compoundStatement_35c5;
		CyberComposition::Simulink::SFTransition sFTransition_35c6;
		CyberComposition::Simulink::SFConnectorRef sFConnectorRef_35c7;
		CyberComposition::Simulink::SFTransConnector sFDstTransConnector_35c8;
	};

	std::list< Match> _matches;
};

class GetDirectTransitions_35d1
{
public:
	void operator()( const Packets_t& sFStates_35d2, const Packets_t& sFSrcTransConnectors_35d5, const Packets_t& functions_35d8, const Packets_t& compoundStatements_35db, Packets_t& sFStates_35d4, Packets_t& sFTransitions_35d7, Packets_t& functions_35da, Packets_t& compoundStatements_35dd);

protected:
	bool isInputUnique( const Udm::Object& sFState_35e6, const Udm::Object& sFSrcTransConnector_35ef, const Udm::Object& function_35f8, const Udm::Object& compoundStatement_3601);
	void processInputPackets( const Packets_t& sFStates_35d2, const Packets_t& sFSrcTransConnectors_35d5, const Packets_t& functions_35d8, const Packets_t& compoundStatements_35db);
	bool patternMatcher( const Udm::Object& sFState_35e4, const Udm::Object& sFSrcTransConnector_35ed, const Udm::Object& function_35f6, const Udm::Object& compoundStatement_35ff);
	void effector();
	void outputAppender( const CyberComposition::Simulink::SFState& sFState_3611, const CyberComposition::Simulink::SFTransition& sFTransition_3613, const SFC::Function& function_3615, const SFC::CompoundStatement& compoundStatement_3617);

private:
	Packets_t* _sFState_35de;
	Packets_t* _sFTransition_35df;
	Packets_t* _function_35e0;
	Packets_t* _compoundStatement_35e1;
	Packets_t _sFState_35e2;
	Packets_t _sFSrcTransConnector_35eb;
	Packets_t _function_35f4;
	Packets_t _compoundStatement_35fd;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_360b;
		CyberComposition::Simulink::SFTransConnector sFSrcTransConnector_360c;
		SFC::Function function_360d;
		SFC::CompoundStatement compoundStatement_360e;
		CyberComposition::Simulink::SFTransition sFTransition_360f;
		CyberComposition::Simulink::SFTransConnector sFDstTransConnector_3610;
	};

	std::list< Match> _matches;
};

class GetTransitionsFromStateRefs_3619
{
public:
	void operator()( const Packets_t& sFStates_361a, const Packets_t& sFTCStates_361e, const Packets_t& functions_3620, const Packets_t& compoundStatements_3623, Packets_t& sFStates_361c, Packets_t& sFTransitions_361d, Packets_t& functions_3622, Packets_t& compoundStatements_3625);

protected:
	bool isInputUnique( const Udm::Object& sFState_362e, const Udm::Object& sFTCState_3637, const Udm::Object& function_3640, const Udm::Object& compoundStatement_3649);
	bool isGuardTrue( SFC::CompoundStatement& CompoundStatement, SFC::Function& Function, CyberComposition::Simulink::SFConnectorRef& SFConnectorRef, CyberComposition::Simulink::SFTransConnector& SFDstTransConnector, CyberComposition::Simulink::SFState& SFRefParentState, CyberComposition::Simulink::SFState& SFState, CyberComposition::Simulink::SFState& SFTCState, CyberComposition::Simulink::SFTransition& SFTransition);
	void processInputPackets( const Packets_t& sFStates_361a, const Packets_t& sFTCStates_361e, const Packets_t& functions_3620, const Packets_t& compoundStatements_3623);
	bool patternMatcher( const Udm::Object& sFState_362c, const Udm::Object& sFTCState_3635, const Udm::Object& function_363e, const Udm::Object& compoundStatement_3647);
	void effector();
	void outputAppender( const CyberComposition::Simulink::SFState& sFState_3668, const CyberComposition::Simulink::SFTransition& sFTransition_366a, const SFC::Function& function_366c, const SFC::CompoundStatement& compoundStatement_366e);

private:
	Packets_t* _sFState_3626;
	Packets_t* _sFTransition_3627;
	Packets_t* _function_3628;
	Packets_t* _compoundStatement_3629;
	Packets_t _sFState_362a;
	Packets_t _sFTCState_3633;
	Packets_t _function_363c;
	Packets_t _compoundStatement_3645;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_3658;
		CyberComposition::Simulink::SFState sFTCState_3659;
		SFC::Function function_365a;
		SFC::CompoundStatement compoundStatement_365b;
		CyberComposition::Simulink::SFState sFRefParentState_365c;
		CyberComposition::Simulink::SFTransition sFTransition_365d;
		CyberComposition::Simulink::SFConnectorRef sFConnectorRef_365e;
		CyberComposition::Simulink::SFTransConnector sFDstTransConnector_365f;
	};

	std::list< Match> _matches;
};

class ProcessedFilter_3689
{
public:
	void operator()( const Packets_t& states_368a, const Packets_t& tcs_368c, const Packets_t& fcns_368e, const Packets_t& cgs_3690, Packets_t& states_3692, Packets_t& tcs_3693, Packets_t& fcns_3694, Packets_t& cgs_3695);

protected:
	void executeOne( const Packets_t& states_368a, const Packets_t& tcs_368c, const Packets_t& fcns_368e, const Packets_t& cgs_3690);
	bool isInputUnique( const Udm::Object& state_369c, const Udm::Object& tc_36a3, const Udm::Object& fcn_36aa, const Udm::Object& cg_36b1);

private:
	Packets_t* _state_3696;
	Packets_t* _tc_3697;
	Packets_t* _fcn_3698;
	Packets_t* _cg_3699;
	Packets_t _state_369a;
	Packets_t _tc_36a1;
	Packets_t _fcn_36a8;
	Packets_t _cg_36af;
};

class TCNotProcessed_36b6
{
public:
	bool operator()( const Packets_t& sFStates_36b7, const Packets_t& sFTransConnectors_36ba, const Packets_t& functions_36bd, const Packets_t& conditionalGroups_36c0, Packets_t& sFStates_36b9, Packets_t& sFTransConnectors_36bc, Packets_t& functions_36bf, Packets_t& conditionalGroups_36c2);

protected:
	bool isInputUnique( const Udm::Object& sFState_36cb, const Udm::Object& sFTransConnector_36d4, const Udm::Object& function_36dd, const Udm::Object& conditionalGroup_36e6);
	bool isGuardTrue( SFC::ConditionalGroup& ConditionalGroup, SFC::Function& Function, CyberComposition::Simulink::SFState& SFState, CyberComposition::Simulink::SFTransConnector& SFTransConnector);
	void processInputPackets( const Packets_t& sFStates_36b7, const Packets_t& sFTransConnectors_36ba, const Packets_t& functions_36bd, const Packets_t& conditionalGroups_36c0);
	bool patternMatcher( const Udm::Object& sFState_36c9, const Udm::Object& sFTransConnector_36d2, const Udm::Object& function_36db, const Udm::Object& conditionalGroup_36e4);
	void outputAppender( const CyberComposition::Simulink::SFState& sFState_36f4, const CyberComposition::Simulink::SFTransConnector& sFTransConnector_36f6, const SFC::Function& function_36f8, const SFC::ConditionalGroup& conditionalGroup_36fa);

private:
	Packets_t* _sFState_36c3;
	Packets_t* _sFTransConnector_36c4;
	Packets_t* _function_36c5;
	Packets_t* _conditionalGroup_36c6;
	Packets_t _sFState_36c7;
	Packets_t _sFTransConnector_36d0;
	Packets_t _function_36d9;
	Packets_t _conditionalGroup_36e2;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_36ec;
		CyberComposition::Simulink::SFTransConnector sFTransConnector_36ed;
		SFC::Function function_36ee;
		SFC::ConditionalGroup conditionalGroup_36ef;
	};

	std::list< Match> _matches;
};

class CreateTransitionCG_36fc
{
public:
	void operator()( const Packets_t& sFStates_36fd, const Packets_t& sFTransConnectors_3700, const Packets_t& functions_3703, const Packets_t& tCVarCGs_3707, Packets_t& sFStates_36ff, Packets_t& sFTransConnectors_3702, Packets_t& functions_3705, Packets_t& transitionCGs_3706);

protected:
	bool isInputUnique( const Udm::Object& sFState_3711, const Udm::Object& sFTransConnector_371a, const Udm::Object& function_3723, const Udm::Object& tCVarCG_372c);
	bool isGuardTrue( SFC::Function& Function, CyberComposition::Simulink::SFState& SFState, CyberComposition::Simulink::SFTransConnector& SFTransConnector, SFC::LocalVar& TCVar, SFC::ConditionalGroup& TCVarCG);
	void processInputPackets( const Packets_t& sFStates_36fd, const Packets_t& sFTransConnectors_3700, const Packets_t& functions_3703, const Packets_t& tCVarCGs_3707);
	bool patternMatcher( const Udm::Object& sFState_370f, const Udm::Object& sFTransConnector_3718, const Udm::Object& function_3721, const Udm::Object& tCVarCG_372a);
	void effector();
	void outputAppender( const CyberComposition::Simulink::SFState& sFState_3745, const CyberComposition::Simulink::SFTransConnector& sFTransConnector_3747, const SFC::Function& function_3749, const SFC::ConditionalGroup& transitionCG_374b);

private:
	Packets_t* _sFState_3709;
	Packets_t* _sFTransConnector_370a;
	Packets_t* _function_370b;
	Packets_t* _transitionCG_370c;
	Packets_t _sFState_370d;
	Packets_t _sFTransConnector_3716;
	Packets_t _function_371f;
	Packets_t _tCVarCG_3728;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_3735;
		CyberComposition::Simulink::SFTransConnector sFTransConnector_3736;
		SFC::Function function_3737;
		SFC::ConditionalGroup tCVarCG_3738;
		SFC::LocalVar tCVar_3739;
	};

	std::list< Match> _matches;
};

class T2L_3761
{
public:
	void operator()( const Packets_t& rootFolders_3762, const Packets_t& rootStates_3764, const Packets_t& projects_3766);

protected:
	void callGetProject_5c15( const Packets_t& rootFolders_3d21, const Packets_t& sFStates_3d24, const Packets_t& projects_3d27);
	void callCreateTypes_5c19( const Packets_t& rootFolders_3769, const Packets_t& states_376b, const Packets_t& projects_376d);
	void callSetFileName_5c1d( const Packets_t& sFStates_3d50, const Packets_t& projects_3d54);
	void callProcessProgram_5c20( const Packets_t& states_3d76, const Packets_t& programs_3d78);
};

class CreateTypes_3768
{
public:
	void operator()( const Packets_t& rootFolders_3769, const Packets_t& states_376b, const Packets_t& projects_376d, Packets_t& states_376f, Packets_t& projects_3770);

protected:
	void callGetTypeBlock_3d15( const Packets_t& rootFolders_3ca9, const Packets_t& projects_3cab);
	void callCreateTypesInner_3d18( const Packets_t& typess_3774, const Packets_t& projects_3776);
	void callStructMembers_3d1b( const Packets_t& typess_3c79, const Packets_t& newStructs_3c7b);
	void callRegisterStruct_3d1e( const Packets_t& sfcStructs_3c68);

private:
	Packets_t* _state_3771;
	Packets_t* _project_3772;
};

class CreateTypesInner_3773
{
public:
	void operator()( const Packets_t& typess_3774, const Packets_t& projects_3776, Packets_t& typess_3778, Packets_t& newStructs_3779);

protected:
	void executeOne( const Packets_t& typess_3774, const Packets_t& projects_3776);
	bool isInputUnique( const Udm::Object& types_377e, const Udm::Object& project_3785);
	void callStructOrMatrix_3c5e( const Packets_t& typess_378b, const Packets_t& projects_378d);
	void callCreateStructType_3c61( const Packets_t& ecsl_dp_Structs_3c39, const Packets_t& projects_3c3d);
	void callCreateArrayOrBasicType_3c64( const Packets_t& typess_37f0, const Packets_t& projects_37f2);

private:
	Packets_t* _types_377a;
	Packets_t* _newStruct_377b;
	Packets_t _types_377c;
	Packets_t _project_3783;
};

class StructOrMatrix_378a
{
public:
	void operator()( const Packets_t& typess_378b, const Packets_t& projects_378d, Packets_t& typess_378f, Packets_t& projects_3790, Packets_t& typess_3791, Packets_t& projects_3792);

protected:
	void executeOne( const Packets_t& typess_378b, const Packets_t& projects_378d);
	bool isInputUnique( const Udm::Object& types_3799, const Udm::Object& project_37a0);

private:
	Packets_t* _types_3793;
	Packets_t* _project_3794;
	Packets_t* _types_3795;
	Packets_t* _project_3796;
	Packets_t _types_3797;
	Packets_t _project_379e;
};

class IsStruct_37a5
{
public:
	bool operator()( const Packets_t& typeStructs_37a6, const Packets_t& projects_37a9, Packets_t& typeStructs_37a8, Packets_t& projects_37ab);

protected:
	bool isInputUnique( const Udm::Object& typeStruct_37b2, const Udm::Object& project_37bb);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& typeStructs_37a6, const Packets_t& projects_37a9);
	bool patternMatcher( const Udm::Object& typeStruct_37b0, const Udm::Object& project_37b9);
	void outputAppender( const CyberComposition::Simulink::TypeStruct& typeStruct_37c6, const SFC::Project& project_37c8);

private:
	Packets_t* _typeStruct_37ac;
	Packets_t* _project_37ad;
	Packets_t _typeStruct_37ae;
	Packets_t _project_37b7;
	class Match
	{
	public:
		CyberComposition::Simulink::TypeStruct typeStruct_37c4;
		SFC::Project project_37c5;
	};

	std::list< Match> _matches;
};

class IsMatrix_37ca
{
public:
	bool operator()( const Packets_t& matrixs_37cb, const Packets_t& projects_37ce, Packets_t& matrixs_37cd, Packets_t& projects_37d0);

protected:
	bool isInputUnique( const Udm::Object& matrix_37d7, const Udm::Object& project_37e0);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& matrixs_37cb, const Packets_t& projects_37ce);
	bool patternMatcher( const Udm::Object& matrix_37d5, const Udm::Object& project_37de);
	void outputAppender( const CyberComposition::Simulink::SF_Matrix& matrix_37eb, const SFC::Project& project_37ed);

private:
	Packets_t* _matrix_37d1;
	Packets_t* _project_37d2;
	Packets_t _matrix_37d3;
	Packets_t _project_37dc;
	class Match
	{
	public:
		CyberComposition::Simulink::SF_Matrix matrix_37e9;
		SFC::Project project_37ea;
	};

	std::list< Match> _matches;
};

class CreateArrayOrBasicType_37ef
{
public:
	void operator()( const Packets_t& typess_37f0, const Packets_t& projects_37f2);

protected:
	void callDimensionTest_3c26( const Packets_t& typess_3b95, const Packets_t& projects_3b97);
	void callProcessOther_3c29( const Packets_t& typess_3833, const Packets_t& projects_3835);
	void callProcessColumn_3c2c( const Packets_t& typess_3b83, const Packets_t& projects_3b85);
	void callProcessScalar_3c2f( const Packets_t& typess_3973, const Packets_t& projects_3975);
	void callRegisterType_3c32( const Packets_t& matrixs_37f5, const Packets_t& dTs_37f8);
	void callMakeAssoc_3c35( const Packets_t& matrixs_3816, const Packets_t& dTs_3818);
};

class RegisterType_37f4
{
public:
	void operator()( const Packets_t& matrixs_37f5, const Packets_t& dTs_37f8, Packets_t& matrixs_37f7, Packets_t& dTs_37fa);

protected:
	bool isInputUnique( const Udm::Object& matrix_3801, const Udm::Object& dT_380a);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& matrixs_37f5, const Packets_t& dTs_37f8);
	bool patternMatcher( const Udm::Object& matrix_37ff, const Udm::Object& dT_3808);
	void effector();
	void forwardInputs();

private:
	Packets_t* _matrix_37fb;
	Packets_t* _dT_37fc;
	Packets_t _matrix_37fd;
	Packets_t _dT_3806;
	class Match
	{
	public:
		CyberComposition::Simulink::SF_Matrix matrix_3813;
		SFC::DT dT_3814;
	};

	std::list< Match> _matches;
};

class MakeAssoc_3815
{
public:
	void operator()( const Packets_t& matrixs_3816, const Packets_t& dTs_3818);

protected:
	bool isInputUnique( const Udm::Object& matrix_381e, const Udm::Object& dT_3827);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& matrixs_3816, const Packets_t& dTs_3818);
	bool patternMatcher( const Udm::Object& matrix_381c, const Udm::Object& dT_3825);
	void effector();

private:
	Packets_t _matrix_381a;
	Packets_t _dT_3823;
	class Match
	{
	public:
		CyberComposition::Simulink::SF_Matrix matrix_3830;
		SFC::DT dT_3831;
	};

	std::list< Match> _matches;
};

class ProcessOther_3832
{
public:
	void operator()( const Packets_t& typess_3833, const Packets_t& projects_3835, Packets_t& typess_3837, Packets_t& array1s_3838, Packets_t& projects_3839);

protected:
	void callProcessRowVector_3b7b( const Packets_t& typess_383e, const Packets_t& projects_3840);
	void callProcessArray1_3b7e( const Packets_t& typess_3a54, const Packets_t& basictypes_3a56, const Packets_t& projects_3a58);

private:
	Packets_t* _types_383a;
	Packets_t* _array1_383b;
	Packets_t* _project_383c;
};

class ProcessRowVector_383d
{
public:
	void operator()( const Packets_t& typess_383e, const Packets_t& projects_3840, Packets_t& typess_3842, Packets_t& array2s_3843, Packets_t& projectss_3844);

protected:
	void callProcessScalar_3a4c( const Packets_t& typess_3973, const Packets_t& projects_3975);
	void callProcessArray2_3a4f( const Packets_t& typess_3849, const Packets_t& basictypes_384b, const Packets_t& projects_384d);

private:
	Packets_t* _types_3845;
	Packets_t* _array2_3846;
	Packets_t* _projects_3847;
};

class ProcessArray2_3848
{
public:
	void operator()( const Packets_t& typess_3849, const Packets_t& basictypes_384b, const Packets_t& projects_384d, Packets_t& typess_384f, Packets_t& array2s_3850, Packets_t& projectss_3851);

protected:
	void callGetArray2_3966( const Packets_t& typess_3856, const Packets_t& basictypes_3858, const Packets_t& projects_385a);
	void callUseArray2_396a( const Packets_t& matrixs_38f2, const Packets_t& basicTypes_38f6, const Packets_t& projects_38f8);
	void callCreateArray2_396e( const Packets_t& matrixs_3931, const Packets_t& basicTypes_3935, const Packets_t& projects_3937);

private:
	Packets_t* _types_3852;
	Packets_t* _array2_3853;
	Packets_t* _projects_3854;
};

class GetArray2_3855
{
public:
	void operator()( const Packets_t& typess_3856, const Packets_t& basictypes_3858, const Packets_t& projects_385a, Packets_t& typess_385c, Packets_t& basictypes_385d, Packets_t& projectss_385e, Packets_t& typess_385f, Packets_t& basictypes_3860, Packets_t& projectss_3861);

protected:
	void executeOne( const Packets_t& typess_3856, const Packets_t& basictypes_3858, const Packets_t& projects_385a);
	bool isInputUnique( const Udm::Object& types_386a, const Udm::Object& basictype_3871, const Udm::Object& project_3878);

private:
	Packets_t* _types_3862;
	Packets_t* _basictype_3863;
	Packets_t* _projects_3864;
	Packets_t* _types_3865;
	Packets_t* _basictype_3866;
	Packets_t* _projects_3867;
	Packets_t _types_3868;
	Packets_t _basictype_386f;
	Packets_t _project_3876;
};

class Array2Exists_387d
{
public:
	bool operator()( const Packets_t& matrixs_387e, const Packets_t& basicTypes_3881, const Packets_t& projects_3884, Packets_t& matrixs_3880, Packets_t& basicTypes_3883, Packets_t& projects_3886);

protected:
	bool isInputUnique( const Udm::Object& matrix_388e, const Udm::Object& basicType_3897, const Udm::Object& project_38a0);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::Array& Array, SFC::BasicType& BasicType, CyberComposition::Simulink::SF_Matrix& Matrix, SFC::Project& Project);
	void processInputPackets( const Packets_t& matrixs_387e, const Packets_t& basicTypes_3881, const Packets_t& projects_3884);
	bool patternMatcher( const Udm::Object& matrix_388c, const Udm::Object& basicType_3895, const Udm::Object& project_389e);
	void outputAppender( const CyberComposition::Simulink::SF_Matrix& matrix_38b6, const SFC::BasicType& basicType_38b8, const SFC::Project& project_38ba);

private:
	Packets_t* _matrix_3887;
	Packets_t* _basicType_3888;
	Packets_t* _project_3889;
	Packets_t _matrix_388a;
	Packets_t _basicType_3893;
	Packets_t _project_389c;
	class Match
	{
	public:
		CyberComposition::Simulink::SF_Matrix matrix_38ae;
		SFC::BasicType basicType_38af;
		SFC::Project project_38b0;
		SFC::Array array_38b1;
	};

	std::list< Match> _matches;
};

class Otherwise_38bc
{
public:
	bool operator()( const Packets_t& matrixs_38bd, const Packets_t& basicTypes_38c0, const Packets_t& projects_38c3, Packets_t& matrixs_38bf, Packets_t& basicTypes_38c2, Packets_t& projects_38c5);

protected:
	bool isInputUnique( const Udm::Object& matrix_38cd, const Udm::Object& basicType_38d6, const Udm::Object& project_38df);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& matrixs_38bd, const Packets_t& basicTypes_38c0, const Packets_t& projects_38c3);
	bool patternMatcher( const Udm::Object& matrix_38cb, const Udm::Object& basicType_38d4, const Udm::Object& project_38dd);
	void outputAppender( const CyberComposition::Simulink::SF_Matrix& matrix_38eb, const SFC::BasicType& basicType_38ed, const SFC::Project& project_38ef);

private:
	Packets_t* _matrix_38c6;
	Packets_t* _basicType_38c7;
	Packets_t* _project_38c8;
	Packets_t _matrix_38c9;
	Packets_t _basicType_38d2;
	Packets_t _project_38db;
	class Match
	{
	public:
		CyberComposition::Simulink::SF_Matrix matrix_38e8;
		SFC::BasicType basicType_38e9;
		SFC::Project project_38ea;
	};

	std::list< Match> _matches;
};

class UseArray2_38f1
{
public:
	void operator()( const Packets_t& matrixs_38f2, const Packets_t& basicTypes_38f6, const Packets_t& projects_38f8, Packets_t& matrixs_38f4, Packets_t& arrays_38f5, Packets_t& projects_38fa);

protected:
	bool isInputUnique( const Udm::Object& matrix_3902, const Udm::Object& basicType_390b, const Udm::Object& project_3914);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::Array& Array, SFC::BasicType& BasicType, CyberComposition::Simulink::SF_Matrix& Matrix, SFC::Project& Project);
	void processInputPackets( const Packets_t& matrixs_38f2, const Packets_t& basicTypes_38f6, const Packets_t& projects_38f8);
	bool patternMatcher( const Udm::Object& matrix_3900, const Udm::Object& basicType_3909, const Udm::Object& project_3912);
	void effector();
	void outputAppender( const CyberComposition::Simulink::SF_Matrix& matrix_392a, const SFC::Array& array_392c, const SFC::Project& project_392e);

private:
	Packets_t* _matrix_38fb;
	Packets_t* _array_38fc;
	Packets_t* _project_38fd;
	Packets_t _matrix_38fe;
	Packets_t _basicType_3907;
	Packets_t _project_3910;
	class Match
	{
	public:
		CyberComposition::Simulink::SF_Matrix matrix_3922;
		SFC::BasicType basicType_3923;
		SFC::Project project_3924;
		SFC::Array array_3925;
	};

	std::list< Match> _matches;
};

class CreateArray2_3930
{
public:
	void operator()( const Packets_t& matrixs_3931, const Packets_t& basicTypes_3935, const Packets_t& projects_3937, Packets_t& matrixs_3933, Packets_t& arrays_3934, Packets_t& projects_3939);

protected:
	bool isInputUnique( const Udm::Object& matrix_3941, const Udm::Object& basicType_394a, const Udm::Object& project_3953);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& matrixs_3931, const Packets_t& basicTypes_3935, const Packets_t& projects_3937);
	bool patternMatcher( const Udm::Object& matrix_393f, const Udm::Object& basicType_3948, const Udm::Object& project_3951);
	void effector();
	void outputAppender( const CyberComposition::Simulink::SF_Matrix& matrix_3960, const SFC::Array& array_3962, const SFC::Project& project_3964);

private:
	Packets_t* _matrix_393a;
	Packets_t* _array_393b;
	Packets_t* _project_393c;
	Packets_t _matrix_393d;
	Packets_t _basicType_3946;
	Packets_t _project_394f;
	class Match
	{
	public:
		CyberComposition::Simulink::SF_Matrix matrix_395c;
		SFC::BasicType basicType_395d;
		SFC::Project project_395e;
	};

	std::list< Match> _matches;
};

class ProcessScalar_3972
{
public:
	void operator()( const Packets_t& typess_3973, const Packets_t& projects_3975, Packets_t& typess_3977, Packets_t& basictypes_3978, Packets_t& projects_3979);

protected:
	void callGetBasicType_3a43( const Packets_t& typess_39d8, const Packets_t& projects_39da);
	void callCreateBasicType_3a46( const Packets_t& matrixs_397e, const Packets_t& projects_3982);
	void callUseBasicType_3a49( const Packets_t& matrixs_39a8, const Packets_t& projects_39ac);

private:
	Packets_t* _types_397a;
	Packets_t* _basictype_397b;
	Packets_t* _project_397c;
};

class CreateBasicType_397d
{
public:
	void operator()( const Packets_t& matrixs_397e, const Packets_t& projects_3982, Packets_t& matrixs_3980, Packets_t& basicTypes_3981, Packets_t& projects_3984);

protected:
	bool isInputUnique( const Udm::Object& matrix_398c, const Udm::Object& project_3995);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& matrixs_397e, const Packets_t& projects_3982);
	bool patternMatcher( const Udm::Object& matrix_398a, const Udm::Object& project_3993);
	void effector();
	void outputAppender( const CyberComposition::Simulink::SF_Matrix& matrix_39a1, const SFC::BasicType& basicType_39a3, const SFC::Project& project_39a5);

private:
	Packets_t* _matrix_3985;
	Packets_t* _basicType_3986;
	Packets_t* _project_3987;
	Packets_t _matrix_3988;
	Packets_t _project_3991;
	class Match
	{
	public:
		CyberComposition::Simulink::SF_Matrix matrix_399e;
		SFC::Project project_399f;
	};

	std::list< Match> _matches;
};

class UseBasicType_39a7
{
public:
	void operator()( const Packets_t& matrixs_39a8, const Packets_t& projects_39ac, Packets_t& matrixs_39aa, Packets_t& basicTypes_39ab, Packets_t& projects_39ae);

protected:
	bool isInputUnique( const Udm::Object& matrix_39b6, const Udm::Object& project_39bf);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::BasicType& BasicType, CyberComposition::Simulink::SF_Matrix& Matrix, SFC::Project& Project);
	void processInputPackets( const Packets_t& matrixs_39a8, const Packets_t& projects_39ac);
	bool patternMatcher( const Udm::Object& matrix_39b4, const Udm::Object& project_39bd);
	void effector();
	void outputAppender( const CyberComposition::Simulink::SF_Matrix& matrix_39d1, const SFC::BasicType& basicType_39d3, const SFC::Project& project_39d5);

private:
	Packets_t* _matrix_39af;
	Packets_t* _basicType_39b0;
	Packets_t* _project_39b1;
	Packets_t _matrix_39b2;
	Packets_t _project_39bb;
	class Match
	{
	public:
		CyberComposition::Simulink::SF_Matrix matrix_39cb;
		SFC::Project project_39cc;
		SFC::BasicType basicType_39cd;
	};

	std::list< Match> _matches;
};

class GetBasicType_39d7
{
public:
	void operator()( const Packets_t& typess_39d8, const Packets_t& projects_39da, Packets_t& typess_39dc, Packets_t& projectss_39dd, Packets_t& typess_39de, Packets_t& projectss_39df);

protected:
	void executeOne( const Packets_t& typess_39d8, const Packets_t& projects_39da);
	bool isInputUnique( const Udm::Object& types_39e6, const Udm::Object& project_39ed);

private:
	Packets_t* _types_39e0;
	Packets_t* _projects_39e1;
	Packets_t* _types_39e2;
	Packets_t* _projects_39e3;
	Packets_t _types_39e4;
	Packets_t _project_39eb;
};

class BasicTypeExists_39f2
{
public:
	bool operator()( const Packets_t& matrixs_39f3, const Packets_t& projects_39f6, Packets_t& matrixs_39f5, Packets_t& projects_39f8);

protected:
	bool isInputUnique( const Udm::Object& matrix_39ff, const Udm::Object& project_3a08);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::BasicType& BasicType, CyberComposition::Simulink::SF_Matrix& Matrix, SFC::Project& Project);
	void processInputPackets( const Packets_t& matrixs_39f3, const Packets_t& projects_39f6);
	bool patternMatcher( const Udm::Object& matrix_39fd, const Udm::Object& project_3a06);
	void outputAppender( const CyberComposition::Simulink::SF_Matrix& matrix_3a1a, const SFC::Project& project_3a1c);

private:
	Packets_t* _matrix_39f9;
	Packets_t* _project_39fa;
	Packets_t _matrix_39fb;
	Packets_t _project_3a04;
	class Match
	{
	public:
		CyberComposition::Simulink::SF_Matrix matrix_3a14;
		SFC::Project project_3a15;
		SFC::BasicType basicType_3a16;
	};

	std::list< Match> _matches;
};

class Otherwise_3a1e
{
public:
	bool operator()( const Packets_t& matrixs_3a1f, const Packets_t& projects_3a22, Packets_t& matrixs_3a21, Packets_t& projects_3a24);

protected:
	bool isInputUnique( const Udm::Object& matrix_3a2b, const Udm::Object& project_3a34);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& matrixs_3a1f, const Packets_t& projects_3a22);
	bool patternMatcher( const Udm::Object& matrix_3a29, const Udm::Object& project_3a32);
	void outputAppender( const CyberComposition::Simulink::SF_Matrix& matrix_3a3f, const SFC::Project& project_3a41);

private:
	Packets_t* _matrix_3a25;
	Packets_t* _project_3a26;
	Packets_t _matrix_3a27;
	Packets_t _project_3a30;
	class Match
	{
	public:
		CyberComposition::Simulink::SF_Matrix matrix_3a3d;
		SFC::Project project_3a3e;
	};

	std::list< Match> _matches;
};

class ProcessArray1_3a53
{
public:
	void operator()( const Packets_t& typess_3a54, const Packets_t& basictypes_3a56, const Packets_t& projects_3a58, Packets_t& typess_3a5a, Packets_t& array1s_3a5b, Packets_t& projects_3a5c);

protected:
	void callGetArray1_3b6f( const Packets_t& typess_3ad5, const Packets_t& dts_3ad7, const Packets_t& projects_3ad9);
	void callCreateArray1_3b73( const Packets_t& matrixs_3a61, const Packets_t& dTs_3a65, const Packets_t& projects_3a67);
	void callUseArray1_3b77( const Packets_t& matrixs_3a97, const Packets_t& dTs_3a9b, const Packets_t& projects_3a9d);

private:
	Packets_t* _types_3a5d;
	Packets_t* _array1_3a5e;
	Packets_t* _project_3a5f;
};

class CreateArray1_3a60
{
public:
	void operator()( const Packets_t& matrixs_3a61, const Packets_t& dTs_3a65, const Packets_t& projects_3a67, Packets_t& matrixs_3a63, Packets_t& arrays_3a64, Packets_t& projects_3a69);

protected:
	bool isInputUnique( const Udm::Object& matrix_3a71, const Udm::Object& dT_3a7a, const Udm::Object& project_3a83);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& matrixs_3a61, const Packets_t& dTs_3a65, const Packets_t& projects_3a67);
	bool patternMatcher( const Udm::Object& matrix_3a6f, const Udm::Object& dT_3a78, const Udm::Object& project_3a81);
	void effector();
	void outputAppender( const CyberComposition::Simulink::SF_Matrix& matrix_3a90, const SFC::Array& array_3a92, const SFC::Project& project_3a94);

private:
	Packets_t* _matrix_3a6a;
	Packets_t* _array_3a6b;
	Packets_t* _project_3a6c;
	Packets_t _matrix_3a6d;
	Packets_t _dT_3a76;
	Packets_t _project_3a7f;
	class Match
	{
	public:
		CyberComposition::Simulink::SF_Matrix matrix_3a8c;
		SFC::DT dT_3a8d;
		SFC::Project project_3a8e;
	};

	std::list< Match> _matches;
};

class UseArray1_3a96
{
public:
	void operator()( const Packets_t& matrixs_3a97, const Packets_t& dTs_3a9b, const Packets_t& projects_3a9d, Packets_t& matrixs_3a99, Packets_t& arrays_3a9a, Packets_t& projects_3a9f);

protected:
	bool isInputUnique( const Udm::Object& matrix_3aa7, const Udm::Object& dT_3ab0, const Udm::Object& project_3ab9);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::Array& Array, SFC::DT& DT, CyberComposition::Simulink::SF_Matrix& Matrix, SFC::Project& Project);
	void processInputPackets( const Packets_t& matrixs_3a97, const Packets_t& dTs_3a9b, const Packets_t& projects_3a9d);
	bool patternMatcher( const Udm::Object& matrix_3aa5, const Udm::Object& dT_3aae, const Udm::Object& project_3ab7);
	void effector();
	void outputAppender( const CyberComposition::Simulink::SF_Matrix& matrix_3ace, const SFC::Array& array_3ad0, const SFC::Project& project_3ad2);

private:
	Packets_t* _matrix_3aa0;
	Packets_t* _array_3aa1;
	Packets_t* _project_3aa2;
	Packets_t _matrix_3aa3;
	Packets_t _dT_3aac;
	Packets_t _project_3ab5;
	class Match
	{
	public:
		CyberComposition::Simulink::SF_Matrix matrix_3ac6;
		SFC::DT dT_3ac7;
		SFC::Project project_3ac8;
		SFC::Array array_3ac9;
	};

	std::list< Match> _matches;
};

class GetArray1_3ad4
{
public:
	void operator()( const Packets_t& typess_3ad5, const Packets_t& dts_3ad7, const Packets_t& projects_3ad9, Packets_t& typess_3adb, Packets_t& dts_3adc, Packets_t& projectss_3add, Packets_t& typess_3ade, Packets_t& dts_3adf, Packets_t& projectss_3ae0);

protected:
	void executeOne( const Packets_t& typess_3ad5, const Packets_t& dts_3ad7, const Packets_t& projects_3ad9);
	bool isInputUnique( const Udm::Object& types_3ae9, const Udm::Object& dt_3af0, const Udm::Object& project_3af7);

private:
	Packets_t* _types_3ae1;
	Packets_t* _dt_3ae2;
	Packets_t* _projects_3ae3;
	Packets_t* _types_3ae4;
	Packets_t* _dt_3ae5;
	Packets_t* _projects_3ae6;
	Packets_t _types_3ae7;
	Packets_t _dt_3aee;
	Packets_t _project_3af5;
};

class Array1Exists_3afc
{
public:
	bool operator()( const Packets_t& matrixs_3afd, const Packets_t& dTs_3b00, const Packets_t& projects_3b03, Packets_t& matrixs_3aff, Packets_t& dTs_3b02, Packets_t& projects_3b05);

protected:
	bool isInputUnique( const Udm::Object& matrix_3b0d, const Udm::Object& dT_3b16, const Udm::Object& project_3b1f);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::Array& Array, SFC::DT& DT, CyberComposition::Simulink::SF_Matrix& Matrix, SFC::Project& Project);
	void processInputPackets( const Packets_t& matrixs_3afd, const Packets_t& dTs_3b00, const Packets_t& projects_3b03);
	bool patternMatcher( const Udm::Object& matrix_3b0b, const Udm::Object& dT_3b14, const Udm::Object& project_3b1d);
	void outputAppender( const CyberComposition::Simulink::SF_Matrix& matrix_3b34, const SFC::DT& dT_3b36, const SFC::Project& project_3b38);

private:
	Packets_t* _matrix_3b06;
	Packets_t* _dT_3b07;
	Packets_t* _project_3b08;
	Packets_t _matrix_3b09;
	Packets_t _dT_3b12;
	Packets_t _project_3b1b;
	class Match
	{
	public:
		CyberComposition::Simulink::SF_Matrix matrix_3b2c;
		SFC::DT dT_3b2d;
		SFC::Project project_3b2e;
		SFC::Array array_3b2f;
	};

	std::list< Match> _matches;
};

class Otherwise_3b3a
{
public:
	bool operator()( const Packets_t& matrixs_3b3b, const Packets_t& dTs_3b3e, const Packets_t& projects_3b41, Packets_t& matrixs_3b3d, Packets_t& dTs_3b40, Packets_t& projects_3b43);

protected:
	bool isInputUnique( const Udm::Object& matrix_3b4b, const Udm::Object& dT_3b54, const Udm::Object& project_3b5d);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& matrixs_3b3b, const Packets_t& dTs_3b3e, const Packets_t& projects_3b41);
	bool patternMatcher( const Udm::Object& matrix_3b49, const Udm::Object& dT_3b52, const Udm::Object& project_3b5b);
	void outputAppender( const CyberComposition::Simulink::SF_Matrix& matrix_3b69, const SFC::DT& dT_3b6b, const SFC::Project& project_3b6d);

private:
	Packets_t* _matrix_3b44;
	Packets_t* _dT_3b45;
	Packets_t* _project_3b46;
	Packets_t _matrix_3b47;
	Packets_t _dT_3b50;
	Packets_t _project_3b59;
	class Match
	{
	public:
		CyberComposition::Simulink::SF_Matrix matrix_3b66;
		SFC::DT dT_3b67;
		SFC::Project project_3b68;
	};

	std::list< Match> _matches;
};

class ProcessColumn_3b82
{
public:
	void operator()( const Packets_t& typess_3b83, const Packets_t& projects_3b85, Packets_t& typess_3b87, Packets_t& array1s_3b88, Packets_t& projects_3b89);

protected:
	void callProcessScalar_3b8d( const Packets_t& typess_3973, const Packets_t& projects_3975);
	void callProcessArray1_3b90( const Packets_t& typess_3a54, const Packets_t& basictypes_3a56, const Packets_t& projects_3a58);

private:
	Packets_t* _types_3b8a;
	Packets_t* _array1_3b8b;
	Packets_t* _project_3b8c;
};

class DimensionTest_3b94
{
public:
	void operator()( const Packets_t& typess_3b95, const Packets_t& projects_3b97, Packets_t& typess_3b99, Packets_t& projects_3b9a, Packets_t& typess_3b9b, Packets_t& projects_3b9c, Packets_t& typess_3b9d, Packets_t& projects_3b9e);

protected:
	void executeOne( const Packets_t& typess_3b95, const Packets_t& projects_3b97);
	bool isInputUnique( const Udm::Object& types_3ba7, const Udm::Object& project_3bae);

private:
	Packets_t* _types_3b9f;
	Packets_t* _project_3ba0;
	Packets_t* _types_3ba1;
	Packets_t* _project_3ba2;
	Packets_t* _types_3ba3;
	Packets_t* _project_3ba4;
	Packets_t _types_3ba5;
	Packets_t _project_3bac;
};

class Scalar_3bb3
{
public:
	bool operator()( const Packets_t& matrixs_3bb4, const Packets_t& projects_3bb7, Packets_t& matrixs_3bb6, Packets_t& projects_3bb9);

protected:
	bool isInputUnique( const Udm::Object& matrix_3bc0, const Udm::Object& project_3bc9);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( CyberComposition::Simulink::SF_Matrix& Matrix, SFC::Project& Project);
	void processInputPackets( const Packets_t& matrixs_3bb4, const Packets_t& projects_3bb7);
	bool patternMatcher( const Udm::Object& matrix_3bbe, const Udm::Object& project_3bc7);
	void outputAppender( const CyberComposition::Simulink::SF_Matrix& matrix_3bd6, const SFC::Project& project_3bd8);

private:
	Packets_t* _matrix_3bba;
	Packets_t* _project_3bbb;
	Packets_t _matrix_3bbc;
	Packets_t _project_3bc5;
	class Match
	{
	public:
		CyberComposition::Simulink::SF_Matrix matrix_3bd2;
		SFC::Project project_3bd3;
	};

	std::list< Match> _matches;
};

class ColumnVector_3bda
{
public:
	bool operator()( const Packets_t& matrixs_3bdb, const Packets_t& projects_3bde, Packets_t& matrixs_3bdd, Packets_t& projects_3be0);

protected:
	bool isInputUnique( const Udm::Object& matrix_3be7, const Udm::Object& project_3bf0);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( CyberComposition::Simulink::SF_Matrix& Matrix, SFC::Project& Project);
	void processInputPackets( const Packets_t& matrixs_3bdb, const Packets_t& projects_3bde);
	bool patternMatcher( const Udm::Object& matrix_3be5, const Udm::Object& project_3bee);
	void outputAppender( const CyberComposition::Simulink::SF_Matrix& matrix_3bfd, const SFC::Project& project_3bff);

private:
	Packets_t* _matrix_3be1;
	Packets_t* _project_3be2;
	Packets_t _matrix_3be3;
	Packets_t _project_3bec;
	class Match
	{
	public:
		CyberComposition::Simulink::SF_Matrix matrix_3bf9;
		SFC::Project project_3bfa;
	};

	std::list< Match> _matches;
};

class Other_3c01
{
public:
	bool operator()( const Packets_t& matrixs_3c02, const Packets_t& projects_3c05, Packets_t& matrixs_3c04, Packets_t& projects_3c07);

protected:
	bool isInputUnique( const Udm::Object& matrix_3c0e, const Udm::Object& project_3c17);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& matrixs_3c02, const Packets_t& projects_3c05);
	bool patternMatcher( const Udm::Object& matrix_3c0c, const Udm::Object& project_3c15);
	void outputAppender( const CyberComposition::Simulink::SF_Matrix& matrix_3c22, const SFC::Project& project_3c24);

private:
	Packets_t* _matrix_3c08;
	Packets_t* _project_3c09;
	Packets_t _matrix_3c0a;
	Packets_t _project_3c13;
	class Match
	{
	public:
		CyberComposition::Simulink::SF_Matrix matrix_3c20;
		SFC::Project project_3c21;
	};

	std::list< Match> _matches;
};

class CreateStructType_3c38
{
public:
	void operator()( const Packets_t& ecsl_dp_Structs_3c39, const Packets_t& projects_3c3d, Packets_t& ecsl_dp_Structs_3c3b, Packets_t& sfc_Structs_3c3c);

protected:
	bool isInputUnique( const Udm::Object& ecsl_dp_Struct_3c45, const Udm::Object& project_3c4e);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& ecsl_dp_Structs_3c39, const Packets_t& projects_3c3d);
	bool patternMatcher( const Udm::Object& ecsl_dp_Struct_3c43, const Udm::Object& project_3c4c);
	void effector();
	void outputAppender( const CyberComposition::Simulink::TypeStruct& ecsl_dp_Struct_3c5a, const SFC::Struct& sfc_Struct_3c5c);

private:
	Packets_t* _ecsl_dp_Struct_3c3f;
	Packets_t* _sfc_Struct_3c40;
	Packets_t _ecsl_dp_Struct_3c41;
	Packets_t _project_3c4a;
	class Match
	{
	public:
		CyberComposition::Simulink::TypeStruct ecsl_dp_Struct_3c57;
		SFC::Project project_3c58;
	};

	std::list< Match> _matches;
};

class RegisterStruct_3c67
{
public:
	void operator()( const Packets_t& sfcStructs_3c68);

protected:
	bool isInputUnique( const Udm::Object& sfcStruct_3c6e);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& sfcStructs_3c68);
	bool patternMatcher( const Udm::Object& sfcStruct_3c6c);
	void effector();

private:
	Packets_t _sfcStruct_3c6a;
	class Match
	{
	public:
		SFC::Struct sfcStruct_3c77;
	};

	std::list< Match> _matches;
};

class StructMembers_3c78
{
public:
	void operator()( const Packets_t& typess_3c79, const Packets_t& newStructs_3c7b, Packets_t& newStructs_3c7d);

protected:
	void callCreateStructMembers_3ca5( const Packets_t& ecsl_dp_Structs_3c80, const Packets_t& sfc_Structs_3c82);

private:
	Packets_t* _newStruct_3c7e;
};

class CreateStructMembers_3c7f
{
public:
	void operator()( const Packets_t& ecsl_dp_Structs_3c80, const Packets_t& sfc_Structs_3c82);

protected:
	bool isInputUnique( const Udm::Object& ecsl_dp_Struct_3c88, const Udm::Object& sfc_Struct_3c91);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& ecsl_dp_Structs_3c80, const Packets_t& sfc_Structs_3c82);
	bool patternMatcher( const Udm::Object& ecsl_dp_Struct_3c86, const Udm::Object& sfc_Struct_3c8f);
	void effector();

private:
	Packets_t _ecsl_dp_Struct_3c84;
	Packets_t _sfc_Struct_3c8d;
	class Match
	{
	public:
		CyberComposition::Simulink::TypeStruct ecsl_dp_Struct_3c9f;
		SFC::Struct sfc_Struct_3ca0;
		CyberComposition::Simulink::TypeBaseRef ecsl_dp_StructRef_3ca1;
		CyberComposition::Simulink::SF_TypeBase ecsl_dp_TypeBase_3ca2;
		SFC::DT sfc_DT_3ca3;
	};

	std::list< Match> _matches;
};

class GetTypeBlock_3ca8
{
public:
	void operator()( const Packets_t& rootFolders_3ca9, const Packets_t& projects_3cab, Packets_t& typess_3cad, Packets_t& projects_3cae);

protected:
	void callGetTypesFromLib_3d0f( const Packets_t& rootFolders_3cdf, const Packets_t& projects_3ce2);
	void callGetTypes_3d12( const Packets_t& rootFolders_3cb2, const Packets_t& projects_3cb5);

private:
	Packets_t* _types_3caf;
	Packets_t* _project_3cb0;
};

class GetTypes_3cb1
{
public:
	void operator()( const Packets_t& rootFolders_3cb2, const Packets_t& projects_3cb5, Packets_t& typeBases_3cb4, Packets_t& projects_3cb7);

protected:
	bool isInputUnique( const Udm::Object& rootFolder_3cbe, const Udm::Object& project_3cc7);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& rootFolders_3cb2, const Packets_t& projects_3cb5);
	bool patternMatcher( const Udm::Object& rootFolder_3cbc, const Udm::Object& project_3cc5);
	void effector();
	void outputAppender( const CyberComposition::Simulink::SF_TypeBase& typeBase_3cda, const SFC::Project& project_3cdc);

private:
	Packets_t* _typeBase_3cb8;
	Packets_t* _project_3cb9;
	Packets_t _rootFolder_3cba;
	Packets_t _project_3cc3;
	class Match
	{
	public:
		CyberComposition::RootFolder rootFolder_3cd6;
		SFC::Project project_3cd7;
		CyberComposition::Simulink::Types types_3cd8;
		CyberComposition::Simulink::SF_TypeBase typeBase_3cd9;
	};

	std::list< Match> _matches;
};

class GetTypesFromLib_3cde
{
public:
	void operator()( const Packets_t& rootFolders_3cdf, const Packets_t& projects_3ce2, Packets_t& typeBases_3ce1, Packets_t& projects_3ce4);

protected:
	bool isInputUnique( const Udm::Object& rootFolder_3ceb, const Udm::Object& project_3cf4);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& rootFolders_3cdf, const Packets_t& projects_3ce2);
	bool patternMatcher( const Udm::Object& rootFolder_3ce9, const Udm::Object& project_3cf2);
	void effector();
	void outputAppender( const CyberComposition::Simulink::SF_TypeBase& typeBase_3d0b, const SFC::Project& project_3d0d);

private:
	Packets_t* _typeBase_3ce5;
	Packets_t* _project_3ce6;
	Packets_t _rootFolder_3ce7;
	Packets_t _project_3cf0;
	class Match
	{
	public:
		CyberComposition::RootFolder rootFolder_3d06;
		SFC::Project project_3d07;
		CyberComposition::RootFolder rootFolder_3d08;
		CyberComposition::Simulink::Types types_3d09;
		CyberComposition::Simulink::SF_TypeBase typeBase_3d0a;
	};

	std::list< Match> _matches;
};

class GetProject_3d20
{
public:
	void operator()( const Packets_t& rootFolders_3d21, const Packets_t& sFStates_3d24, const Packets_t& projects_3d27, Packets_t& rootFolders_3d23, Packets_t& sFStates_3d26, Packets_t& projects_3d29);

protected:
	bool isInputUnique( const Udm::Object& rootFolder_3d31, const Udm::Object& sFState_3d3a, const Udm::Object& project_3d43);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& rootFolders_3d21, const Packets_t& sFStates_3d24, const Packets_t& projects_3d27);
	bool patternMatcher( const Udm::Object& rootFolder_3d2f, const Udm::Object& sFState_3d38, const Udm::Object& project_3d41);
	void effector();
	void forwardInputs();

private:
	Packets_t* _rootFolder_3d2a;
	Packets_t* _sFState_3d2b;
	Packets_t* _project_3d2c;
	Packets_t _rootFolder_3d2d;
	Packets_t _sFState_3d36;
	Packets_t _project_3d3f;
	class Match
	{
	public:
		CyberComposition::RootFolder rootFolder_3d4c;
		CyberComposition::Simulink::SFState sFState_3d4d;
		SFC::Project project_3d4e;
	};

	std::list< Match> _matches;
};

class SetFileName_3d4f
{
public:
	void operator()( const Packets_t& sFStates_3d50, const Packets_t& projects_3d54, Packets_t& sFStates_3d52, Packets_t& programs_3d53);

protected:
	bool isInputUnique( const Udm::Object& sFState_3d5c, const Udm::Object& project_3d65);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& sFStates_3d50, const Packets_t& projects_3d54);
	bool patternMatcher( const Udm::Object& sFState_3d5a, const Udm::Object& project_3d63);
	void effector();
	void outputAppender( const CyberComposition::Simulink::SFState& sFState_3d71, const SFC::Program& program_3d73);

private:
	Packets_t* _sFState_3d56;
	Packets_t* _program_3d57;
	Packets_t _sFState_3d58;
	Packets_t _project_3d61;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_3d6e;
		SFC::Project project_3d6f;
	};

	std::list< Match> _matches;
};

class ProcessProgram_3d75
{
public:
	void operator()( const Packets_t& states_3d76, const Packets_t& programs_3d78);

protected:
	void executeOne( const Packets_t& states_3d76, const Packets_t& programs_3d78);
	bool isInputUnique( const Udm::Object& state_3d7c, const Udm::Object& program_3d83);
	void callClearTables_5bf4( const Packets_t& sFStates_4255, const Packets_t& programs_4258);
	void callCreateStateLabels_5bf7( const Packets_t& states_4276, const Packets_t& programs_4278);
	void callCreateDEVars_5bfa( const Packets_t& states_4491, const Packets_t& programs_4493);
	void callCreateFuncStateFunctions_5bfd( const Packets_t& states_4834, const Packets_t& programs_4836);
	void callCreateFSFunctionBodies_5c00( const Packets_t& states_3dbf, const Packets_t& programs_3dc1);
	void callCreateFunctions_5c03( const Packets_t& states_571d, const Packets_t& programs_571f);
	void callPopulateFunctions_5c06( const Packets_t& states_4992, const Packets_t& programs_4994);
	void callCreateRootFunction_5c09( const Packets_t& states_575f, const Packets_t& programs_5761);
	void callCreateInitFunction_5c0c( const Packets_t& states_599e, const Packets_t& programs_59a0);
	void callCreateStatusFunction_5c0f( const Packets_t& states_3d89, const Packets_t& programs_3d8b);
	void callMarkLegacy_5c12( const Packets_t& sFStates_5bd8, const Packets_t& programs_5bda);

private:
	Packets_t _state_3d7a;
	Packets_t _program_3d81;
};

class CreateStatusFunction_3d88
{
public:
	void operator()( const Packets_t& states_3d89, const Packets_t& programs_3d8b, Packets_t& states_3d8d, Packets_t& programs_3d8e);

protected:
	void callCreateStatusFunction_3dbb( const Packets_t& sFStates_3d92, const Packets_t& programs_3d94);

private:
	Packets_t* _state_3d8f;
	Packets_t* _program_3d90;
};

class CreateStatusFunction_3d91
{
public:
	void operator()( const Packets_t& sFStates_3d92, const Packets_t& programs_3d94);

protected:
	bool isInputUnique( const Udm::Object& sFState_3d9a, const Udm::Object& program_3da3);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& sFStates_3d92, const Packets_t& programs_3d94);
	bool patternMatcher( const Udm::Object& sFState_3d98, const Udm::Object& program_3da1);
	void effector();

private:
	Packets_t _sFState_3d96;
	Packets_t _program_3d9f;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_3db2;
		SFC::Program program_3db3;
		SFC::Arg arg_3db4;
		SFC::Function status_3db5;
	};

	std::list< Match> _matches;
};

class CreateFSFunctionBodies_3dbe
{
public:
	void operator()( const Packets_t& states_3dbf, const Packets_t& programs_3dc1, Packets_t& states_3dc3, Packets_t& programs_3dc4);

protected:
	void callPopulateGRFSFunction_424b( const Packets_t& states_3dc8, const Packets_t& programs_3dca);
	void callPopulateEMFSFunction_424e( const Packets_t& states_41c6, const Packets_t& programs_41c8);
	void callGetSubStates_4251( const Packets_t& sFStates_4223, const Packets_t& parStmnts_4226);

private:
	Packets_t* _state_3dc5;
	Packets_t* _program_3dc6;
};

class PopulateGRFSFunction_3dc7
{
public:
	void operator()( const Packets_t& states_3dc8, const Packets_t& programs_3dca, Packets_t& states_3dcc, Packets_t& programs_3dcd);

protected:
	void callGetGRFuncStateFunction_41b8( const Packets_t& sFStates_3dd1, const Packets_t& programs_3dd4);
	void callTransStartMap_41bb( const Packets_t& states_3f9f, const Packets_t& css_3fa1);
	void callDirectInFunction_41be( const Packets_t& sFStates_3e02, const Packets_t& functions_3e05);
	void callTransStartTransitions_41c1( const Packets_t& states_3e20, const Packets_t& fcns_3e22, const Packets_t& css_3e24);

private:
	Packets_t* _state_3dce;
	Packets_t* _program_3dcf;
};

class GetGRFuncStateFunction_3dd0
{
public:
	void operator()( const Packets_t& sFStates_3dd1, const Packets_t& programs_3dd4, Packets_t& sFStates_3dd3, Packets_t& execs_3dd6);

protected:
	bool isInputUnique( const Udm::Object& sFState_3ddd, const Udm::Object& program_3de6);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::Function& Exec, SFC::Program& Program, CyberComposition::Simulink::SFState& SFState, SFC::StateLabel& StateLabel);
	void processInputPackets( const Packets_t& sFStates_3dd1, const Packets_t& programs_3dd4);
	bool patternMatcher( const Udm::Object& sFState_3ddb, const Udm::Object& program_3de4);
	void effector();
	void outputAppender( const CyberComposition::Simulink::SFState& sFState_3dfd, const SFC::Function& exec_3dff);

private:
	Packets_t* _sFState_3dd7;
	Packets_t* _exec_3dd8;
	Packets_t _sFState_3dd9;
	Packets_t _program_3de2;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_3df5;
		SFC::Program program_3df6;
		SFC::StateLabel stateLabel_3df7;
		SFC::Function exec_3df8;
	};

	std::list< Match> _matches;
};

class DirectInFunction_3e01
{
public:
	void operator()( const Packets_t& sFStates_3e02, const Packets_t& functions_3e05, Packets_t& sFStates_3e04, Packets_t& functions_3e07);

protected:
	bool isInputUnique( const Udm::Object& sFState_3e0e, const Udm::Object& function_3e17);
	void processInputPackets( const Packets_t& sFStates_3e02, const Packets_t& functions_3e05);
	bool patternMatcher( const Udm::Object& sFState_3e0c, const Udm::Object& function_3e15);
	void effector();
	void forwardInputs();

private:
	Packets_t* _sFState_3e08;
	Packets_t* _function_3e09;
	Packets_t _sFState_3e0a;
	Packets_t _function_3e13;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_3e1d;
		SFC::Function function_3e1e;
	};

	std::list< Match> _matches;
};

class TransStartTransitions_3e1f
{
public:
	void operator()( const Packets_t& states_3e20, const Packets_t& fcns_3e22, const Packets_t& css_3e24);

protected:
	void callInitTCVarLoop_3f91( const Packets_t& sFStates_3f4e, const Packets_t& functions_3f51, const Packets_t& compoundStatements_3f54);
	void callGetStartTC_3f95( const Packets_t& sFStates_3e27, const Packets_t& functions_3e2b, const Packets_t& conditionalGroups_3e2e);
	void callEnterTransConnectorRecurse_3f99( const Packets_t& states_3e61, const Packets_t& tcs_3e63, const Packets_t& fcns_3e65, const Packets_t& cgs_3e67);
};

class GetStartTC_3e26
{
public:
	void operator()( const Packets_t& sFStates_3e27, const Packets_t& functions_3e2b, const Packets_t& conditionalGroups_3e2e, Packets_t& sFStates_3e29, Packets_t& sFTransStarts_3e2a, Packets_t& functions_3e2d, Packets_t& conditionalGroups_3e30);

protected:
	bool isInputUnique( const Udm::Object& sFState_3e39, const Udm::Object& function_3e42, const Udm::Object& conditionalGroup_3e4b);
	void processInputPackets( const Packets_t& sFStates_3e27, const Packets_t& functions_3e2b, const Packets_t& conditionalGroups_3e2e);
	bool patternMatcher( const Udm::Object& sFState_3e37, const Udm::Object& function_3e40, const Udm::Object& conditionalGroup_3e49);
	void effector();
	void outputAppender( const CyberComposition::Simulink::SFState& sFState_3e58, const CyberComposition::Simulink::SFTransStart& sFTransStart_3e5a, const SFC::Function& function_3e5c, const SFC::ConditionalGroup& conditionalGroup_3e5e);

private:
	Packets_t* _sFState_3e31;
	Packets_t* _sFTransStart_3e32;
	Packets_t* _function_3e33;
	Packets_t* _conditionalGroup_3e34;
	Packets_t _sFState_3e35;
	Packets_t _function_3e3e;
	Packets_t _conditionalGroup_3e47;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_3e54;
		SFC::Function function_3e55;
		SFC::ConditionalGroup conditionalGroup_3e56;
		CyberComposition::Simulink::SFTransStart sFTransStart_3e57;
	};

	std::list< Match> _matches;
};

class EnterTransConnectorRecurse2_3e60
{
public:
	void operator()( const Packets_t& states_3e61, const Packets_t& tcs_3e63, const Packets_t& fcns_3e65, const Packets_t& cgs_3e67);

protected:
	void executeOne( const Packets_t& states_3e61, const Packets_t& tcs_3e63, const Packets_t& fcns_3e65, const Packets_t& cgs_3e67);
	bool isInputUnique( const Udm::Object& state_3e6b, const Udm::Object& tc_3e72, const Udm::Object& fcn_3e79, const Udm::Object& cg_3e80);
	void callProcessedFilter_3f39( const Packets_t& states_368a, const Packets_t& tcs_368c, const Packets_t& fcns_368e, const Packets_t& cgs_3690);
	void callCreateTransitionCG_3f3e( const Packets_t& sFStates_36fd, const Packets_t& sFTransConnectors_3700, const Packets_t& functions_3703, const Packets_t& tCVarCGs_3707);
	void callEnterTransitions_3f43( const Packets_t& states_3e86, const Packets_t& tcs_3e88, const Packets_t& fcns_3e8a, const Packets_t& css_3e8c);
	void callNoDefaultTransition_3f48( const Packets_t& states_2e48, const Packets_t& tcs_2e4a, const Packets_t& fcns_2e4c, const Packets_t& css_2e4e);

private:
	Packets_t _state_3e69;
	Packets_t _tc_3e70;
	Packets_t _fcn_3e77;
	Packets_t _cg_3e7e;
};

class EnterTransitions_3e85
{
public:
	void operator()( const Packets_t& states_3e86, const Packets_t& tcs_3e88, const Packets_t& fcns_3e8a, const Packets_t& css_3e8c, Packets_t& states_3e8e, Packets_t& tcs_3e8f, Packets_t& fcns_3e90, Packets_t& css_3e91);

protected:
	void callGetAllTransitions_3f2f( const Packets_t& states_3575, const Packets_t& tcs_3577, const Packets_t& fcns_3579, const Packets_t& css_357b);
	void callEnterProcessTransitions_3f34( const Packets_t& states_3e97, const Packets_t& transs_3e99, const Packets_t& fcns_3e9b, const Packets_t& css_3e9d);

private:
	Packets_t* _state_3e92;
	Packets_t* _tc_3e93;
	Packets_t* _fcn_3e94;
	Packets_t* _cs_3e95;
};

class EnterProcessTransitions_3e96
{
public:
	void operator()( const Packets_t& states_3e97, const Packets_t& transs_3e99, const Packets_t& fcns_3e9b, const Packets_t& css_3e9d);

protected:
	void callOrderTransitions_3efc( const Packets_t& sFStates_3387, const Packets_t& sFTransitions_338a, const Packets_t& functions_338d, const Packets_t& compoundStatements_3390);
	void callTransCond_3f01( const Packets_t& sFStates_3079, const Packets_t& sFTransitions_307d, const Packets_t& functions_3080, const Packets_t& transitionCGs_3083);
	void callTest_3f06( const Packets_t& states_30c4, const Packets_t& dsts_30c6, const Packets_t& transs_30c8, const Packets_t& fcns_30ca, const Packets_t& cbs_30cc);
	void callEnterNextState_3f0c( const Packets_t& sFStates_3ea0, const Packets_t& sFDstStates_3ea2, const Packets_t& sFTransitions_3ea4, const Packets_t& functions_3ea6, const Packets_t& transitionCBs_3ea8);
	void callNextJunction_3f12( const Packets_t& sFStates_3415, const Packets_t& sFJunctions_3418, const Packets_t& sFTransitions_341b, const Packets_t& functions_341d, const Packets_t& transitionCBs_3420);
	void callDeadEndJunction_3f18( const Packets_t& sFStates_33c9, const Packets_t& sFTransConnectors_33cb, const Packets_t& sFTransitions_33cd, const Packets_t& functions_33cf, const Packets_t& transitionCBs_33d1);
	void callGetDstJuncFromRef_3f1e( const Packets_t& sFStates_3024, const Packets_t& sFConnectorRefs_3027, const Packets_t& sFTransitions_302a, const Packets_t& functions_302d, const Packets_t& conditionalBlocks_3030);
	void callGetDstStateFromRef_3f24( const Packets_t& sFStates_347d, const Packets_t& sFConnectorRefs_3481, const Packets_t& sFTransitions_3483, const Packets_t& functions_3486, const Packets_t& conditionalBlocks_3489);
	void callEnterTransConnectorRecurse_3f2a( const Packets_t& states_3e61, const Packets_t& tcs_3e63, const Packets_t& fcns_3e65, const Packets_t& cgs_3e67);
};

class EnterNextState_3e9f
{
public:
	void operator()( const Packets_t& sFStates_3ea0, const Packets_t& sFDstStates_3ea2, const Packets_t& sFTransitions_3ea4, const Packets_t& functions_3ea6, const Packets_t& transitionCBs_3ea8);

protected:
	bool isInputUnique( const Udm::Object& sFState_3eae, const Udm::Object& sFDstState_3eb7, const Udm::Object& sFTransition_3ec0, const Udm::Object& function_3ec9, const Udm::Object& transitionCB_3ed2);
	bool isGuardTrue( SFC::Function& Enter, SFC::Arg& EnterArg0, SFC::Arg& EnterArg1, SFC::Function& Function, CyberComposition::Simulink::SFState& SFDstState, CyberComposition::Simulink::SFState& SFState, CyberComposition::Simulink::SFTransition& SFTransition, SFC::StateLabel& StateLabel, SFC::ConditionalBlock& TransitionCB);
	void processInputPackets( const Packets_t& sFStates_3ea0, const Packets_t& sFDstStates_3ea2, const Packets_t& sFTransitions_3ea4, const Packets_t& functions_3ea6, const Packets_t& transitionCBs_3ea8);
	bool patternMatcher( const Udm::Object& sFState_3eac, const Udm::Object& sFDstState_3eb5, const Udm::Object& sFTransition_3ebe, const Udm::Object& function_3ec7, const Udm::Object& transitionCB_3ed0);
	void effector();

private:
	Packets_t _sFState_3eaa;
	Packets_t _sFDstState_3eb3;
	Packets_t _sFTransition_3ebc;
	Packets_t _function_3ec5;
	Packets_t _transitionCB_3ece;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_3ee4;
		CyberComposition::Simulink::SFState sFDstState_3ee5;
		CyberComposition::Simulink::SFTransition sFTransition_3ee6;
		SFC::Function function_3ee7;
		SFC::ConditionalBlock transitionCB_3ee8;
		SFC::StateLabel stateLabel_3ee9;
		SFC::Arg enterArg0_3eea;
		SFC::Function enter_3eeb;
		SFC::Arg enterArg1_3eec;
	};

	std::list< Match> _matches;
};

class InitTCVarLoop_3f4d
{
public:
	void operator()( const Packets_t& sFStates_3f4e, const Packets_t& functions_3f51, const Packets_t& compoundStatements_3f54, Packets_t& sFStates_3f50, Packets_t& functions_3f53, Packets_t& conditionalGroups_3f56);

protected:
	bool isInputUnique( const Udm::Object& sFState_3f5e, const Udm::Object& function_3f67, const Udm::Object& compoundStatement_3f70);
	bool isGuardTrue( SFC::CompoundStatement& CompoundStatement, SFC::Function& Function, CyberComposition::Simulink::SFState& SFState, SFC::LocalVar& TCVar);
	void processInputPackets( const Packets_t& sFStates_3f4e, const Packets_t& functions_3f51, const Packets_t& compoundStatements_3f54);
	bool patternMatcher( const Udm::Object& sFState_3f5c, const Udm::Object& function_3f65, const Udm::Object& compoundStatement_3f6e);
	void effector();
	void outputAppender( const CyberComposition::Simulink::SFState& sFState_3f8b, const SFC::Function& function_3f8d, const SFC::ConditionalGroup& conditionalGroup_3f8f);

private:
	Packets_t* _sFState_3f57;
	Packets_t* _function_3f58;
	Packets_t* _conditionalGroup_3f59;
	Packets_t _sFState_3f5a;
	Packets_t _function_3f63;
	Packets_t _compoundStatement_3f6c;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_3f79;
		SFC::Function function_3f7a;
		SFC::CompoundStatement compoundStatement_3f7b;
		SFC::LocalVar tCVar_3f7c;
	};

	std::list< Match> _matches;
};

class TransStartMap_3f9e
{
public:
	void operator()( const Packets_t& states_3f9f, const Packets_t& css_3fa1, Packets_t& states_3fa3, Packets_t& css_3fa4);

protected:
	void callInitTransConnectorMap_41b3( const Packets_t& sFStates_3fa8);
	void callTransConnectorMapRecurse_41b5( const Packets_t& states_3fc2, const Packets_t& tcs_3fc4);

private:
	Packets_t* _state_3fa5;
	Packets_t* _cs_3fa6;
};

class InitTransConnectorMap_3fa7
{
public:
	void operator()( const Packets_t& sFStates_3fa8, Packets_t& sFStates_3faa, Packets_t& sFTransStarts_3fab);

protected:
	bool isInputUnique( const Udm::Object& sFState_3fb2);
	void processInputPackets( const Packets_t& sFStates_3fa8);
	bool patternMatcher( const Udm::Object& sFState_3fb0);
	void effector();
	void outputAppender( const CyberComposition::Simulink::SFState& sFState_3fbd, const CyberComposition::Simulink::SFTransStart& sFTransStart_3fbf);

private:
	Packets_t* _sFState_3fac;
	Packets_t* _sFTransStart_3fad;
	Packets_t _sFState_3fae;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_3fbb;
		CyberComposition::Simulink::SFTransStart sFTransStart_3fbc;
	};

	std::list< Match> _matches;
};

class TransConnectorMapRecurse2_3fc1
{
public:
	void operator()( const Packets_t& states_3fc2, const Packets_t& tcs_3fc4);

protected:
	void executeOne( const Packets_t& states_3fc2, const Packets_t& tcs_3fc4);
	bool isInputUnique( const Udm::Object& state_3fc8, const Udm::Object& tc_3fcf);
	void callProcessedFilter_41a7( const Packets_t& states_414f, const Packets_t& tcs_4151);
	void callAddTransConnector_41aa( const Packets_t& sFStates_418a, const Packets_t& sFTransConnectors_418d);
	void callGetAllTransitions_41ad( const Packets_t& states_3fd5, const Packets_t& tcs_3fd7);
	void callProcessTransitionsMap_41b0( const Packets_t& states_4070, const Packets_t& transs_4072);

private:
	Packets_t _state_3fc6;
	Packets_t _tc_3fcd;
};

class GetAllTransitions_3fd4
{
public:
	void operator()( const Packets_t& states_3fd5, const Packets_t& tcs_3fd7, Packets_t& states_3fd9, Packets_t& transs_3fda);

protected:
	void callGetTransitionsFromJunctionRefs_4066( const Packets_t& sFStates_403b, const Packets_t& sFJunctions_403f);
	void callGetDirectTransitions_4069( const Packets_t& sFStates_4013, const Packets_t& sFSrcTransConnectors_4016);
	void callGetTransitionsFromStateRefs_406c( const Packets_t& sFStates_3fde, const Packets_t& sFTCStates_3fe2);

private:
	Packets_t* _state_3fdb;
	Packets_t* _trans_3fdc;
};

class GetTransitionsFromStateRefs_3fdd
{
public:
	void operator()( const Packets_t& sFStates_3fde, const Packets_t& sFTCStates_3fe2, Packets_t& sFStates_3fe0, Packets_t& sFTransitions_3fe1);

protected:
	bool isInputUnique( const Udm::Object& sFState_3fea, const Udm::Object& sFTCState_3ff3);
	bool isGuardTrue( CyberComposition::Simulink::SFConnectorRef& SFConnectorRef, CyberComposition::Simulink::SFTransConnector& SFDstTransConnector, CyberComposition::Simulink::SFState& SFRefParentState, CyberComposition::Simulink::SFState& SFState, CyberComposition::Simulink::SFState& SFTCState, CyberComposition::Simulink::SFTransition& SFTransition);
	void processInputPackets( const Packets_t& sFStates_3fde, const Packets_t& sFTCStates_3fe2);
	bool patternMatcher( const Udm::Object& sFState_3fe8, const Udm::Object& sFTCState_3ff1);
	void effector();
	void outputAppender( const CyberComposition::Simulink::SFState& sFState_400e, const CyberComposition::Simulink::SFTransition& sFTransition_4010);

private:
	Packets_t* _sFState_3fe4;
	Packets_t* _sFTransition_3fe5;
	Packets_t _sFState_3fe6;
	Packets_t _sFTCState_3fef;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_4002;
		CyberComposition::Simulink::SFState sFTCState_4003;
		CyberComposition::Simulink::SFState sFRefParentState_4004;
		CyberComposition::Simulink::SFTransition sFTransition_4005;
		CyberComposition::Simulink::SFConnectorRef sFConnectorRef_4006;
		CyberComposition::Simulink::SFTransConnector sFDstTransConnector_4007;
	};

	std::list< Match> _matches;
};

class GetDirectTransitions_4012
{
public:
	void operator()( const Packets_t& sFStates_4013, const Packets_t& sFSrcTransConnectors_4016, Packets_t& sFStates_4015, Packets_t& sFTransitions_4018);

protected:
	bool isInputUnique( const Udm::Object& sFState_401f, const Udm::Object& sFSrcTransConnector_4028);
	void processInputPackets( const Packets_t& sFStates_4013, const Packets_t& sFSrcTransConnectors_4016);
	bool patternMatcher( const Udm::Object& sFState_401d, const Udm::Object& sFSrcTransConnector_4026);
	void effector();
	void outputAppender( const CyberComposition::Simulink::SFState& sFState_4036, const CyberComposition::Simulink::SFTransition& sFTransition_4038);

private:
	Packets_t* _sFState_4019;
	Packets_t* _sFTransition_401a;
	Packets_t _sFState_401b;
	Packets_t _sFSrcTransConnector_4024;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_4032;
		CyberComposition::Simulink::SFTransConnector sFSrcTransConnector_4033;
		CyberComposition::Simulink::SFTransition sFTransition_4034;
		CyberComposition::Simulink::SFTransConnector sFDstTransConnector_4035;
	};

	std::list< Match> _matches;
};

class GetTransitionsFromJunctionRefs_403a
{
public:
	void operator()( const Packets_t& sFStates_403b, const Packets_t& sFJunctions_403f, Packets_t& sFStates_403d, Packets_t& sFTransitions_403e);

protected:
	bool isInputUnique( const Udm::Object& sFState_4047, const Udm::Object& sFJunction_4050);
	void processInputPackets( const Packets_t& sFStates_403b, const Packets_t& sFJunctions_403f);
	bool patternMatcher( const Udm::Object& sFState_4045, const Udm::Object& sFJunction_404e);
	void effector();
	void outputAppender( const CyberComposition::Simulink::SFState& sFState_4062, const CyberComposition::Simulink::SFTransition& sFTransition_4064);

private:
	Packets_t* _sFState_4041;
	Packets_t* _sFTransition_4042;
	Packets_t _sFState_4043;
	Packets_t _sFJunction_404c;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_405d;
		CyberComposition::Simulink::SFJunction sFJunction_405e;
		CyberComposition::Simulink::SFTransition sFTransition_405f;
		CyberComposition::Simulink::SFConnectorRef sFConnectorRef_4060;
		CyberComposition::Simulink::SFTransConnector sFDstTransConnector_4061;
	};

	std::list< Match> _matches;
};

class ProcessTransitionsMap_406f
{
public:
	void operator()( const Packets_t& states_4070, const Packets_t& transs_4072);

protected:
	void callOrderTransitions_413f( const Packets_t& sFStates_40f8, const Packets_t& sFTransitions_40fb);
	void callgetDstTransConnectors_4142( const Packets_t& sFStates_411a, const Packets_t& sFTransitions_411e);
	void callTransConnTest_4145( const Packets_t& states_4099, const Packets_t& tcs_409b);
	void callTransConnectorMapRecurse_4148( const Packets_t& states_3fc2, const Packets_t& tcs_3fc4);
	void callDerefConnRef_414b( const Packets_t& sFStates_4075, const Packets_t& sFConnectorRefs_4078);
};

class DerefConnRef_4074
{
public:
	void operator()( const Packets_t& sFStates_4075, const Packets_t& sFConnectorRefs_4078, Packets_t& sFStates_4077, Packets_t& sFTransConnectors_407a);

protected:
	bool isInputUnique( const Udm::Object& sFState_4081, const Udm::Object& sFConnectorRef_408a);
	void processInputPackets( const Packets_t& sFStates_4075, const Packets_t& sFConnectorRefs_4078);
	bool patternMatcher( const Udm::Object& sFState_407f, const Udm::Object& sFConnectorRef_4088);
	void effector();
	void outputAppender( const CyberComposition::Simulink::SFState& sFState_4094, const CyberComposition::Simulink::SFTransConnector& sFTransConnector_4096);

private:
	Packets_t* _sFState_407b;
	Packets_t* _sFTransConnector_407c;
	Packets_t _sFState_407d;
	Packets_t _sFConnectorRef_4086;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_4091;
		CyberComposition::Simulink::SFConnectorRef sFConnectorRef_4092;
		CyberComposition::Simulink::SFTransConnector sFTransConnector_4093;
	};

	std::list< Match> _matches;
};

class TransConnTest_4098
{
public:
	void operator()( const Packets_t& states_4099, const Packets_t& tcs_409b, Packets_t& states_409d, Packets_t& tcs_409e, Packets_t& states_409f, Packets_t& tcs_40a0);

protected:
	void executeOne( const Packets_t& states_4099, const Packets_t& tcs_409b);
	bool isInputUnique( const Udm::Object& state_40a7, const Udm::Object& tc_40ae);

private:
	Packets_t* _state_40a1;
	Packets_t* _tc_40a2;
	Packets_t* _state_40a3;
	Packets_t* _tc_40a4;
	Packets_t _state_40a5;
	Packets_t _tc_40ac;
};

class IsConnectorRef_40b3
{
public:
	bool operator()( const Packets_t& sFStates_40b4, const Packets_t& sFConnectorRefs_40b7, Packets_t& sFStates_40b6, Packets_t& sFConnectorRefs_40b9);

protected:
	bool isInputUnique( const Udm::Object& sFState_40c0, const Udm::Object& sFConnectorRef_40c9);
	void processInputPackets( const Packets_t& sFStates_40b4, const Packets_t& sFConnectorRefs_40b7);
	bool patternMatcher( const Udm::Object& sFState_40be, const Udm::Object& sFConnectorRef_40c7);
	void outputAppender( const CyberComposition::Simulink::SFState& sFState_40d1, const CyberComposition::Simulink::SFConnectorRef& sFConnectorRef_40d3);

private:
	Packets_t* _sFState_40ba;
	Packets_t* _sFConnectorRef_40bb;
	Packets_t _sFState_40bc;
	Packets_t _sFConnectorRef_40c5;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_40cf;
		CyberComposition::Simulink::SFConnectorRef sFConnectorRef_40d0;
	};

	std::list< Match> _matches;
};

class Otherwise_40d5
{
public:
	bool operator()( const Packets_t& sFStates_40d6, const Packets_t& sFJunctions_40d9, Packets_t& sFStates_40d8, Packets_t& sFJunctions_40db);

protected:
	bool isInputUnique( const Udm::Object& sFState_40e2, const Udm::Object& sFJunction_40eb);
	void processInputPackets( const Packets_t& sFStates_40d6, const Packets_t& sFJunctions_40d9);
	bool patternMatcher( const Udm::Object& sFState_40e0, const Udm::Object& sFJunction_40e9);
	void outputAppender( const CyberComposition::Simulink::SFState& sFState_40f3, const CyberComposition::Simulink::SFJunction& sFJunction_40f5);

private:
	Packets_t* _sFState_40dc;
	Packets_t* _sFJunction_40dd;
	Packets_t _sFState_40de;
	Packets_t _sFJunction_40e7;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_40f1;
		CyberComposition::Simulink::SFJunction sFJunction_40f2;
	};

	std::list< Match> _matches;
};

class OrderTransitions_40f7
{
public:
	void operator()( const Packets_t& sFStates_40f8, const Packets_t& sFTransitions_40fb, Packets_t& sFStates_40fa, Packets_t& sFTransitions_40fd);

protected:
	bool isInputUnique( const Udm::Object& sFState_4104, const Udm::Object& sFTransition_410d);
	void processInputPackets( const Packets_t& sFStates_40f8, const Packets_t& sFTransitions_40fb);
	bool patternMatcher( const Udm::Object& sFState_4102, const Udm::Object& sFTransition_410b);
	void effector();
	void outputAppender( const CyberComposition::Simulink::SFState& sFState_4115, const CyberComposition::Simulink::SFTransition& sFTransition_4117);
	void sortOutputs();

private:
	Packets_t* _sFState_40fe;
	Packets_t* _sFTransition_40ff;
	Packets_t _sFState_4100;
	Packets_t _sFTransition_4109;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_4113;
		CyberComposition::Simulink::SFTransition sFTransition_4114;
	};

	std::list< Match> _matches;
};

class GetDstTransConnectors_4119
{
public:
	void operator()( const Packets_t& sFStates_411a, const Packets_t& sFTransitions_411e, Packets_t& sFStates_411c, Packets_t& sFDstTransConnectors_411d);

protected:
	bool isInputUnique( const Udm::Object& sFState_4126, const Udm::Object& sFTransition_412f);
	void processInputPackets( const Packets_t& sFStates_411a, const Packets_t& sFTransitions_411e);
	bool patternMatcher( const Udm::Object& sFState_4124, const Udm::Object& sFTransition_412d);
	void effector();
	void outputAppender( const CyberComposition::Simulink::SFState& sFState_413b, const CyberComposition::Simulink::SFTransConnector& sFDstTransConnector_413d);

private:
	Packets_t* _sFState_4120;
	Packets_t* _sFDstTransConnector_4121;
	Packets_t _sFState_4122;
	Packets_t _sFTransition_412b;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_4137;
		CyberComposition::Simulink::SFTransition sFTransition_4138;
		CyberComposition::Simulink::SFTransConnector sFTransConnector_4139;
		CyberComposition::Simulink::SFTransConnector sFDstTransConnector_413a;
	};

	std::list< Match> _matches;
};

class ProcessedFilter_414e
{
public:
	void operator()( const Packets_t& states_414f, const Packets_t& tcs_4151, Packets_t& states_4153, Packets_t& tcs_4154);

protected:
	void executeOne( const Packets_t& states_414f, const Packets_t& tcs_4151);
	bool isInputUnique( const Udm::Object& state_4159, const Udm::Object& tc_4160);

private:
	Packets_t* _state_4155;
	Packets_t* _tc_4156;
	Packets_t _state_4157;
	Packets_t _tc_415e;
};

class TCNotProcessed_4165
{
public:
	bool operator()( const Packets_t& sFStates_4166, const Packets_t& sFTransConnectors_4169, Packets_t& sFStates_4168, Packets_t& sFTransConnectors_416b);

protected:
	bool isInputUnique( const Udm::Object& sFState_4172, const Udm::Object& sFTransConnector_417b);
	bool isGuardTrue( CyberComposition::Simulink::SFState& SFState, CyberComposition::Simulink::SFTransConnector& SFTransConnector);
	void processInputPackets( const Packets_t& sFStates_4166, const Packets_t& sFTransConnectors_4169);
	bool patternMatcher( const Udm::Object& sFState_4170, const Udm::Object& sFTransConnector_4179);
	void outputAppender( const CyberComposition::Simulink::SFState& sFState_4185, const CyberComposition::Simulink::SFTransConnector& sFTransConnector_4187);

private:
	Packets_t* _sFState_416c;
	Packets_t* _sFTransConnector_416d;
	Packets_t _sFState_416e;
	Packets_t _sFTransConnector_4177;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_4181;
		CyberComposition::Simulink::SFTransConnector sFTransConnector_4182;
	};

	std::list< Match> _matches;
};

class AddTransConnector_4189
{
public:
	void operator()( const Packets_t& sFStates_418a, const Packets_t& sFTransConnectors_418d, Packets_t& sFStates_418c, Packets_t& sFTransConnectors_418f);

protected:
	bool isInputUnique( const Udm::Object& sFState_4196, const Udm::Object& sFTransConnector_419f);
	void processInputPackets( const Packets_t& sFStates_418a, const Packets_t& sFTransConnectors_418d);
	bool patternMatcher( const Udm::Object& sFState_4194, const Udm::Object& sFTransConnector_419d);
	void effector();
	void forwardInputs();

private:
	Packets_t* _sFState_4190;
	Packets_t* _sFTransConnector_4191;
	Packets_t _sFState_4192;
	Packets_t _sFTransConnector_419b;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_41a5;
		CyberComposition::Simulink::SFTransConnector sFTransConnector_41a6;
	};

	std::list< Match> _matches;
};

class PopulateEMFSFunction_41c5
{
public:
	void operator()( const Packets_t& states_41c6, const Packets_t& programs_41c8, Packets_t& states_41ca, Packets_t& programs_41cb);

protected:
	void callGetEMFuncStateFunction_421c( const Packets_t& sFStates_41ec, const Packets_t& programs_41ef);
	void callFunctionBody_421f( const Packets_t& sFStates_41cf, const Packets_t& execs_41d1);

private:
	Packets_t* _state_41cc;
	Packets_t* _program_41cd;
};

class FunctionBody_41ce
{
public:
	void operator()( const Packets_t& sFStates_41cf, const Packets_t& execs_41d1);

protected:
	bool isInputUnique( const Udm::Object& sFState_41d7, const Udm::Object& exec_41e0);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& sFStates_41cf, const Packets_t& execs_41d1);
	bool patternMatcher( const Udm::Object& sFState_41d5, const Udm::Object& exec_41de);
	void effector();

private:
	Packets_t _sFState_41d3;
	Packets_t _exec_41dc;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_41e9;
		SFC::Function exec_41ea;
	};

	std::list< Match> _matches;
};

class GetEMFuncStateFunction_41eb
{
public:
	void operator()( const Packets_t& sFStates_41ec, const Packets_t& programs_41ef, Packets_t& sFStates_41ee, Packets_t& execs_41f1);

protected:
	bool isInputUnique( const Udm::Object& sFState_41f8, const Udm::Object& program_4201);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::Function& Exec, SFC::Program& Program, CyberComposition::Simulink::SFState& SFState, SFC::StateLabel& StateLabel);
	void processInputPackets( const Packets_t& sFStates_41ec, const Packets_t& programs_41ef);
	bool patternMatcher( const Udm::Object& sFState_41f6, const Udm::Object& program_41ff);
	void effector();
	void outputAppender( const CyberComposition::Simulink::SFState& sFState_4218, const SFC::Function& exec_421a);

private:
	Packets_t* _sFState_41f2;
	Packets_t* _exec_41f3;
	Packets_t _sFState_41f4;
	Packets_t _program_41fd;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_4210;
		SFC::Program program_4211;
		SFC::StateLabel stateLabel_4212;
		SFC::Function exec_4213;
	};

	std::list< Match> _matches;
};

class GetSubStates_4222
{
public:
	void operator()( const Packets_t& sFStates_4223, const Packets_t& parStmnts_4226, Packets_t& sFSubStates_4225, Packets_t& parStmnts_4228);

protected:
	bool isInputUnique( const Udm::Object& sFState_422f, const Udm::Object& parStmnt_4238);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& sFStates_4223, const Packets_t& parStmnts_4226);
	bool patternMatcher( const Udm::Object& sFState_422d, const Udm::Object& parStmnt_4236);
	void effector();
	void outputAppender( const CyberComposition::Simulink::SFState& sFSubState_4247, const SFC::CompoundStatement& parStmnt_4249);

private:
	Packets_t* _sFSubState_4229;
	Packets_t* _parStmnt_422a;
	Packets_t _sFState_422b;
	Packets_t _parStmnt_4234;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_4244;
		SFC::CompoundStatement parStmnt_4245;
		CyberComposition::Simulink::SFState sFSubState_4246;
	};

	std::list< Match> _matches;
};

class ClearTables_4254
{
public:
	void operator()( const Packets_t& sFStates_4255, const Packets_t& programs_4258, Packets_t& sFStates_4257, Packets_t& programs_425a);

protected:
	bool isInputUnique( const Udm::Object& sFState_4261, const Udm::Object& program_426a);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& sFStates_4255, const Packets_t& programs_4258);
	bool patternMatcher( const Udm::Object& sFState_425f, const Udm::Object& program_4268);
	void effector();
	void forwardInputs();

private:
	Packets_t* _sFState_425b;
	Packets_t* _program_425c;
	Packets_t _sFState_425d;
	Packets_t _program_4266;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_4273;
		SFC::Program program_4274;
	};

	std::list< Match> _matches;
};

class CreateStateLabels_4275
{
public:
	void operator()( const Packets_t& states_4276, const Packets_t& programs_4278, Packets_t& states_427a, Packets_t& programs_427b);

protected:
	void callCreateCompoundStates_4484( const Packets_t& states_438e, const Packets_t& programs_4390);
	void callCreateStateVar_4487( const Packets_t& sFStates_4463, const Packets_t& programs_4466);
	void callCreateLeafStates_448a( const Packets_t& states_43e3, const Packets_t& programs_43e5);
	void callCreateFuncStates_psuedo_448d( const Packets_t& states_427f, const Packets_t& programs_4281);

private:
	Packets_t* _state_427c;
	Packets_t* _program_427d;
};

class CreateFuncStates_psuedo_427e
{
public:
	void operator()( const Packets_t& states_427f, const Packets_t& programs_4281, Packets_t& states_4283, Packets_t& programs_4284);

protected:
	void callIsFuncState_4384( const Packets_t& states_4288, const Packets_t& programs_428a);
	void callGetSubStates_4387( const Packets_t& sFStates_4223, const Packets_t& parStmnts_4226);
	void callCreateStateLabel_438a( const Packets_t& states_42f6, const Packets_t& programs_42f8);

private:
	Packets_t* _state_4285;
	Packets_t* _program_4286;
};

class IsFuncState_4287
{
public:
	void operator()( const Packets_t& states_4288, const Packets_t& programs_428a, Packets_t& states_428c, Packets_t& programs_428d, Packets_t& states_428e, Packets_t& programs_428f);

protected:
	void executeOne( const Packets_t& states_4288, const Packets_t& programs_428a);
	bool isInputUnique( const Udm::Object& state_4296, const Udm::Object& program_429d);

private:
	Packets_t* _state_4290;
	Packets_t* _program_4291;
	Packets_t* _state_4292;
	Packets_t* _program_4293;
	Packets_t _state_4294;
	Packets_t _program_429b;
};

class HasSubstates_42a2
{
public:
	bool operator()( const Packets_t& sFStates_42a3, const Packets_t& programs_42a6, Packets_t& sFStates_42a5, Packets_t& programs_42a8);

protected:
	bool isInputUnique( const Udm::Object& sFState_42af, const Udm::Object& program_42b8);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::Program& Program, CyberComposition::Simulink::SFState& SFState, CyberComposition::Simulink::SFState& SFSubState);
	void processInputPackets( const Packets_t& sFStates_42a3, const Packets_t& programs_42a6);
	bool patternMatcher( const Udm::Object& sFState_42ad, const Udm::Object& program_42b6);
	void outputAppender( const CyberComposition::Simulink::SFState& sFState_42ca, const SFC::Program& program_42cc);

private:
	Packets_t* _sFState_42a9;
	Packets_t* _program_42aa;
	Packets_t _sFState_42ab;
	Packets_t _program_42b4;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_42c4;
		SFC::Program program_42c5;
		CyberComposition::Simulink::SFState sFSubState_42c6;
	};

	std::list< Match> _matches;
};

class Otherwise_42ce
{
public:
	bool operator()( const Packets_t& sFStates_42cf, const Packets_t& programs_42d2, Packets_t& sFStates_42d1, Packets_t& programs_42d4);

protected:
	bool isInputUnique( const Udm::Object& sFState_42db, const Udm::Object& program_42e4);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::Program& Program, CyberComposition::Simulink::SFState& SFState);
	void processInputPackets( const Packets_t& sFStates_42cf, const Packets_t& programs_42d2);
	bool patternMatcher( const Udm::Object& sFState_42d9, const Udm::Object& program_42e2);
	void outputAppender( const CyberComposition::Simulink::SFState& sFState_42f1, const SFC::Program& program_42f3);

private:
	Packets_t* _sFState_42d5;
	Packets_t* _program_42d6;
	Packets_t _sFState_42d7;
	Packets_t _program_42e0;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_42ed;
		SFC::Program program_42ee;
	};

	std::list< Match> _matches;
};

class CreateStateLabel_42f5
{
public:
	void operator()( const Packets_t& states_42f6, const Packets_t& programs_42f8, Packets_t& states_42fa, Packets_t& programs_42fb);

protected:
	void callCreate_437b( const Packets_t& sFStates_4327, const Packets_t& programs_432a);
	void callInitMask_437e( const Packets_t& sFStates_434d, const Packets_t& stateLabels_4350);
	void callSetPath_4381( const Packets_t& sFStates_42ff, const Packets_t& stateLabels_4302);

private:
	Packets_t* _state_42fc;
	Packets_t* _program_42fd;
};

class SetPath_42fe
{
public:
	void operator()( const Packets_t& sFStates_42ff, const Packets_t& stateLabels_4302, Packets_t& sFSuperStates_4301, Packets_t& stateLabels_4304);

protected:
	bool isInputUnique( const Udm::Object& sFState_430b, const Udm::Object& stateLabel_4314);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& sFStates_42ff, const Packets_t& stateLabels_4302);
	bool patternMatcher( const Udm::Object& sFState_4309, const Udm::Object& stateLabel_4312);
	void effector();
	void outputAppender( const CyberComposition::Simulink::SFState& sFSuperState_4322, const SFC::StateLabel& stateLabel_4324);

private:
	Packets_t* _sFSuperState_4305;
	Packets_t* _stateLabel_4306;
	Packets_t _sFState_4307;
	Packets_t _stateLabel_4310;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_431f;
		SFC::StateLabel stateLabel_4320;
		CyberComposition::Simulink::SFState sFSuperState_4321;
	};

	std::list< Match> _matches;
};

class Create_4326
{
public:
	void operator()( const Packets_t& sFStates_4327, const Packets_t& programs_432a, Packets_t& sFStates_4329, Packets_t& stateLabels_432c);

protected:
	bool isInputUnique( const Udm::Object& sFState_4333, const Udm::Object& program_433c);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& sFStates_4327, const Packets_t& programs_432a);
	bool patternMatcher( const Udm::Object& sFState_4331, const Udm::Object& program_433a);
	void effector();
	void outputAppender( const CyberComposition::Simulink::SFState& sFState_4348, const SFC::StateLabel& stateLabel_434a);

private:
	Packets_t* _sFState_432d;
	Packets_t* _stateLabel_432e;
	Packets_t _sFState_432f;
	Packets_t _program_4338;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_4345;
		SFC::Program program_4346;
	};

	std::list< Match> _matches;
};

class InitMask_434c
{
public:
	void operator()( const Packets_t& sFStates_434d, const Packets_t& stateLabels_4350, Packets_t& sFStates_434f, Packets_t& stateLabels_4352);

protected:
	bool isInputUnique( const Udm::Object& sFState_4359, const Udm::Object& stateLabel_4362);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( CyberComposition::Simulink::SFState& SFState, CyberComposition::Simulink::SFState& SFSuperState, SFC::StateLabel& StateLabel, SFC::StateLabel& SuperStateLabel);
	void processInputPackets( const Packets_t& sFStates_434d, const Packets_t& stateLabels_4350);
	bool patternMatcher( const Udm::Object& sFState_4357, const Udm::Object& stateLabel_4360);
	void effector();
	void forwardInputs();

private:
	Packets_t* _sFState_4353;
	Packets_t* _stateLabel_4354;
	Packets_t _sFState_4355;
	Packets_t _stateLabel_435e;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_4373;
		SFC::StateLabel stateLabel_4374;
		CyberComposition::Simulink::SFState sFSuperState_4375;
		SFC::StateLabel superStateLabel_4376;
	};

	std::list< Match> _matches;
};

class CreateCompoundStates_438d
{
public:
	void operator()( const Packets_t& states_438e, const Packets_t& programs_4390, Packets_t& states_4392, Packets_t& programs_4393);

protected:
	void callIsANDORGROUPCompound_43d9( const Packets_t& states_4397, const Packets_t& programs_4399);
	void callCreateStateLabel_43dc( const Packets_t& states_42f6, const Packets_t& programs_42f8);
	void callGetSubStates_43df( const Packets_t& sFStates_4223, const Packets_t& parStmnts_4226);

private:
	Packets_t* _state_4394;
	Packets_t* _program_4395;
};

class IsANDORGROUPCompound_4396
{
public:
	void operator()( const Packets_t& states_4397, const Packets_t& programs_4399, Packets_t& states_439b, Packets_t& programs_439c);

protected:
	void executeOne( const Packets_t& states_4397, const Packets_t& programs_4399);
	bool isInputUnique( const Udm::Object& state_43a1, const Udm::Object& program_43a8);

private:
	Packets_t* _state_439d;
	Packets_t* _program_439e;
	Packets_t _state_439f;
	Packets_t _program_43a6;
};

class HasSubstates_43ad
{
public:
	bool operator()( const Packets_t& sFStates_43ae, const Packets_t& programs_43b1, Packets_t& sFStates_43b0, Packets_t& programs_43b3);

protected:
	bool isInputUnique( const Udm::Object& sFState_43ba, const Udm::Object& program_43c3);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::Program& Program, CyberComposition::Simulink::SFState& SFState, CyberComposition::Simulink::SFState& SFSubState);
	void processInputPackets( const Packets_t& sFStates_43ae, const Packets_t& programs_43b1);
	bool patternMatcher( const Udm::Object& sFState_43b8, const Udm::Object& program_43c1);
	void outputAppender( const CyberComposition::Simulink::SFState& sFState_43d5, const SFC::Program& program_43d7);

private:
	Packets_t* _sFState_43b4;
	Packets_t* _program_43b5;
	Packets_t _sFState_43b6;
	Packets_t _program_43bf;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_43cf;
		SFC::Program program_43d0;
		CyberComposition::Simulink::SFState sFSubState_43d1;
	};

	std::list< Match> _matches;
};

class CreateLeafStates_43e2
{
public:
	void operator()( const Packets_t& states_43e3, const Packets_t& programs_43e5, Packets_t& states_43e7, Packets_t& programs_43e8);

protected:
	void callIsLeaf_4459( const Packets_t& states_43ec, const Packets_t& programs_43ee);
	void callCreateStateLabel_445c( const Packets_t& states_42f6, const Packets_t& programs_42f8);
	void callGetSubStates_445f( const Packets_t& sFStates_4223, const Packets_t& parStmnts_4226);

private:
	Packets_t* _state_43e9;
	Packets_t* _program_43ea;
};

class IsLeaf_43eb
{
public:
	void operator()( const Packets_t& states_43ec, const Packets_t& programs_43ee, Packets_t& states_43f0, Packets_t& programs_43f1, Packets_t& states_43f2, Packets_t& programs_43f3);

protected:
	void executeOne( const Packets_t& states_43ec, const Packets_t& programs_43ee);
	bool isInputUnique( const Udm::Object& state_43fa, const Udm::Object& program_4401);

private:
	Packets_t* _state_43f4;
	Packets_t* _program_43f5;
	Packets_t* _state_43f6;
	Packets_t* _program_43f7;
	Packets_t _state_43f8;
	Packets_t _program_43ff;
};

class HasSubstates_4406
{
public:
	bool operator()( const Packets_t& sFStates_4407, const Packets_t& programs_440a, Packets_t& sFStates_4409, Packets_t& programs_440c);

protected:
	bool isInputUnique( const Udm::Object& sFState_4413, const Udm::Object& program_441c);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::Program& Program, CyberComposition::Simulink::SFState& SFState, CyberComposition::Simulink::SFState& SFSubState);
	void processInputPackets( const Packets_t& sFStates_4407, const Packets_t& programs_440a);
	bool patternMatcher( const Udm::Object& sFState_4411, const Udm::Object& program_441a);
	void outputAppender( const CyberComposition::Simulink::SFState& sFState_442e, const SFC::Program& program_4430);

private:
	Packets_t* _sFState_440d;
	Packets_t* _program_440e;
	Packets_t _sFState_440f;
	Packets_t _program_4418;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_4428;
		SFC::Program program_4429;
		CyberComposition::Simulink::SFState sFSubState_442a;
	};

	std::list< Match> _matches;
};

class Otherwise_4432
{
public:
	bool operator()( const Packets_t& sFStates_4433, const Packets_t& programs_4436, Packets_t& sFStates_4435, Packets_t& programs_4438);

protected:
	bool isInputUnique( const Udm::Object& sFState_443f, const Udm::Object& program_4448);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::Program& Program, CyberComposition::Simulink::SFState& SFState);
	void processInputPackets( const Packets_t& sFStates_4433, const Packets_t& programs_4436);
	bool patternMatcher( const Udm::Object& sFState_443d, const Udm::Object& program_4446);
	void outputAppender( const CyberComposition::Simulink::SFState& sFState_4455, const SFC::Program& program_4457);

private:
	Packets_t* _sFState_4439;
	Packets_t* _program_443a;
	Packets_t _sFState_443b;
	Packets_t _program_4444;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_4451;
		SFC::Program program_4452;
	};

	std::list< Match> _matches;
};

class CreateStateVar_4462
{
public:
	void operator()( const Packets_t& sFStates_4463, const Packets_t& programs_4466, Packets_t& sFStates_4465, Packets_t& programs_4468);

protected:
	bool isInputUnique( const Udm::Object& sFState_446f, const Udm::Object& program_4478);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& sFStates_4463, const Packets_t& programs_4466);
	bool patternMatcher( const Udm::Object& sFState_446d, const Udm::Object& program_4476);
	void effector();
	void forwardInputs();

private:
	Packets_t* _sFState_4469;
	Packets_t* _program_446a;
	Packets_t _sFState_446b;
	Packets_t _program_4474;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_4481;
		SFC::Program program_4482;
	};

	std::list< Match> _matches;
};

class CreateDEVars_4490
{
public:
	void operator()( const Packets_t& states_4491, const Packets_t& programs_4493, Packets_t& states_4495, Packets_t& programs_4496);

protected:
	void callCheckStateDecomp_4824( const Packets_t& states_47be, const Packets_t& programs_47c0);
	void callGetSubStates_4827( const Packets_t& sFStates_4223, const Packets_t& parStmnts_4226);
	void callCreateDataVar_482a( const Packets_t& states_449a, const Packets_t& programs_449c);
	void callCreateStoreVar_482d( const Packets_t& states_44db, const Packets_t& programs_44dd);
	void callCreateEventVar_4830( const Packets_t& states_4784, const Packets_t& programs_4786);

private:
	Packets_t* _state_4497;
	Packets_t* _program_4498;
};

class CreateDataVar_4499
{
public:
	void operator()( const Packets_t& states_449a, const Packets_t& programs_449c, Packets_t& states_449e, Packets_t& programs_449f);

protected:
	void callCreateDataVar_44d7( const Packets_t& sFStates_44a3, const Packets_t& programs_44a5);

private:
	Packets_t* _state_44a0;
	Packets_t* _program_44a1;
};

class CreateDataVar_44a2
{
public:
	void operator()( const Packets_t& sFStates_44a3, const Packets_t& programs_44a5);

protected:
	bool isInputUnique( const Udm::Object& sFState_44ab, const Udm::Object& program_44b4);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::DT& DT, SFC::Program& Program, CyberComposition::Simulink::SFData& SFData, CyberComposition::Simulink::SFState& SFState, SFC::StateLabel& StateLabel, CyberComposition::Simulink::SF_TypeBase& TypeBase, CyberComposition::Simulink::TypeBaseRef& TypeBaseRef);
	void processInputPackets( const Packets_t& sFStates_44a3, const Packets_t& programs_44a5);
	bool patternMatcher( const Udm::Object& sFState_44a9, const Udm::Object& program_44b2);
	void effector();

private:
	Packets_t _sFState_44a7;
	Packets_t _program_44b0;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_44c8;
		SFC::Program program_44c9;
		SFC::StateLabel stateLabel_44ca;
		CyberComposition::Simulink::SFData sFData_44cb;
		CyberComposition::Simulink::TypeBaseRef typeBaseRef_44cc;
		CyberComposition::Simulink::SF_TypeBase typeBase_44cd;
		SFC::DT dT_44ce;
	};

	std::list< Match> _matches;
};

class CreateStoreVar_44da
{
public:
	void operator()( const Packets_t& states_44db, const Packets_t& programs_44dd, Packets_t& states_44df, Packets_t& programs_44e0);

protected:
	void callCreateDataVar_4771( const Packets_t& sFStates_4731, const Packets_t& programs_4735);
	void callGetHighestState_4774( const Packets_t& states_44e4, const Packets_t& localVars_44e6);
	void callGetContainingSubsystem_4777( const Packets_t& sFStates_4709, const Packets_t& localVars_470c);
	void callGetStoreSubsystem_477a( const Packets_t& systems_4642, const Packets_t& localVars_4644);
	void callGetScope_477d( const Packets_t& systems_45a3, const Packets_t& localVars_45a5);
	void callRegister_4780( const Packets_t& subsystems_4586, const Packets_t& localVars_4588);

private:
	Packets_t* _state_44e1;
	Packets_t* _program_44e2;
};

class GetHighestState_44e3
{
public:
	void operator()( const Packets_t& states_44e4, const Packets_t& localVars_44e6, Packets_t& states_44e8, Packets_t& localVars_44e9);

protected:
	void callHighestStateTest_457f( const Packets_t& states_44ed, const Packets_t& localVars_44ef);
	void callNextHigherState_4582( const Packets_t& sFSubStates_4558, const Packets_t& localVars_455b);

private:
	Packets_t* _state_44ea;
	Packets_t* _localVar_44eb;
};

class HighestStateTest_44ec
{
public:
	void operator()( const Packets_t& states_44ed, const Packets_t& localVars_44ef, Packets_t& states_44f1, Packets_t& localVars_44f2, Packets_t& states_44f3, Packets_t& localVars_44f4);

protected:
	void executeOne( const Packets_t& states_44ed, const Packets_t& localVars_44ef);
	bool isInputUnique( const Udm::Object& state_44fb, const Udm::Object& localVar_4502);

private:
	Packets_t* _state_44f5;
	Packets_t* _localVar_44f6;
	Packets_t* _state_44f7;
	Packets_t* _localVar_44f8;
	Packets_t _state_44f9;
	Packets_t _localVar_4500;
};

class HighestState_4507
{
public:
	bool operator()( const Packets_t& sFSubStates_4508, const Packets_t& localVars_450b, Packets_t& sFSubStates_450a, Packets_t& localVars_450d);

protected:
	bool isInputUnique( const Udm::Object& sFSubState_4514, const Udm::Object& localVar_451d);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& sFSubStates_4508, const Packets_t& localVars_450b);
	bool patternMatcher( const Udm::Object& sFSubState_4512, const Udm::Object& localVar_451b);
	void outputAppender( const CyberComposition::Simulink::SFState& sFSubState_452e, const SFC::LocalVar& localVar_4530);

private:
	Packets_t* _sFSubState_450e;
	Packets_t* _localVar_450f;
	Packets_t _sFSubState_4510;
	Packets_t _localVar_4519;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFSubState_452a;
		SFC::LocalVar localVar_452b;
		CyberComposition::Simulink::SFState sFState_452c;
		CyberComposition::Simulink::Subsystem subsystem_452d;
	};

	std::list< Match> _matches;
};

class Otherwise_4532
{
public:
	bool operator()( const Packets_t& sFStates_4533, const Packets_t& localVars_4536, Packets_t& sFStates_4535, Packets_t& localVars_4538);

protected:
	bool isInputUnique( const Udm::Object& sFState_453f, const Udm::Object& localVar_4548);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& sFStates_4533, const Packets_t& localVars_4536);
	bool patternMatcher( const Udm::Object& sFState_453d, const Udm::Object& localVar_4546);
	void outputAppender( const CyberComposition::Simulink::SFState& sFState_4553, const SFC::LocalVar& localVar_4555);

private:
	Packets_t* _sFState_4539;
	Packets_t* _localVar_453a;
	Packets_t _sFState_453b;
	Packets_t _localVar_4544;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_4551;
		SFC::LocalVar localVar_4552;
	};

	std::list< Match> _matches;
};

class NextHigherState_4557
{
public:
	void operator()( const Packets_t& sFSubStates_4558, const Packets_t& localVars_455b, Packets_t& sFStates_455a, Packets_t& localVars_455d);

protected:
	bool isInputUnique( const Udm::Object& sFSubState_4564, const Udm::Object& localVar_456d);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& sFSubStates_4558, const Packets_t& localVars_455b);
	bool patternMatcher( const Udm::Object& sFSubState_4562, const Udm::Object& localVar_456b);
	void effector();
	void outputAppender( const CyberComposition::Simulink::SFState& sFState_457b, const SFC::LocalVar& localVar_457d);

private:
	Packets_t* _sFState_455e;
	Packets_t* _localVar_455f;
	Packets_t _sFSubState_4560;
	Packets_t _localVar_4569;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFSubState_4578;
		SFC::LocalVar localVar_4579;
		CyberComposition::Simulink::SFState sFState_457a;
	};

	std::list< Match> _matches;
};

class Register_4585
{
public:
	void operator()( const Packets_t& subsystems_4586, const Packets_t& localVars_4588);

protected:
	bool isInputUnique( const Udm::Object& subsystem_458e, const Udm::Object& localVar_4597);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& subsystems_4586, const Packets_t& localVars_4588);
	bool patternMatcher( const Udm::Object& subsystem_458c, const Udm::Object& localVar_4595);
	void effector();

private:
	Packets_t _subsystem_458a;
	Packets_t _localVar_4593;
	class Match
	{
	public:
		CyberComposition::Simulink::Subsystem subsystem_45a0;
		SFC::LocalVar localVar_45a1;
	};

	std::list< Match> _matches;
};

class GetScope_45a2
{
public:
	void operator()( const Packets_t& systems_45a3, const Packets_t& localVars_45a5, Packets_t& systems_45a7, Packets_t& localVars_45a8);

protected:
	void callSubSubsystemTest_463b( const Packets_t& systems_45d4, const Packets_t& localVars_45d6);
	void callAddToScope_463e( const Packets_t& subSubsystems_45ac, const Packets_t& localVars_45af);

private:
	Packets_t* _system_45a9;
	Packets_t* _localVar_45aa;
};

class AddToScope_45ab
{
public:
	void operator()( const Packets_t& subSubsystems_45ac, const Packets_t& localVars_45af, Packets_t& subsystems_45ae, Packets_t& localVars_45b1);

protected:
	bool isInputUnique( const Udm::Object& subSubsystem_45b8, const Udm::Object& localVar_45c1);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& subSubsystems_45ac, const Packets_t& localVars_45af);
	bool patternMatcher( const Udm::Object& subSubsystem_45b6, const Udm::Object& localVar_45bf);
	void effector();
	void outputAppender( const CyberComposition::Simulink::Subsystem& subsystem_45cf, const SFC::LocalVar& localVar_45d1);

private:
	Packets_t* _subsystem_45b2;
	Packets_t* _localVar_45b3;
	Packets_t _subSubsystem_45b4;
	Packets_t _localVar_45bd;
	class Match
	{
	public:
		CyberComposition::Simulink::Subsystem subSubsystem_45cc;
		SFC::LocalVar localVar_45cd;
		CyberComposition::Simulink::Subsystem subsystem_45ce;
	};

	std::list< Match> _matches;
};

class SubSubsystemTest_45d3
{
public:
	void operator()( const Packets_t& systems_45d4, const Packets_t& localVars_45d6, Packets_t& systems_45d8, Packets_t& localVars_45d9, Packets_t& systems_45da, Packets_t& localVars_45db);

protected:
	void executeOne( const Packets_t& systems_45d4, const Packets_t& localVars_45d6);
	bool isInputUnique( const Udm::Object& system_45e2, const Udm::Object& localVar_45e9);

private:
	Packets_t* _system_45dc;
	Packets_t* _localVar_45dd;
	Packets_t* _system_45de;
	Packets_t* _localVar_45df;
	Packets_t _system_45e0;
	Packets_t _localVar_45e7;
};

class NotTopSubsystem_45ee
{
public:
	bool operator()( const Packets_t& subSubsystems_45ef, const Packets_t& localVars_45f2, Packets_t& subSubsystems_45f1, Packets_t& localVars_45f4);

protected:
	bool isInputUnique( const Udm::Object& subSubsystem_45fb, const Udm::Object& localVar_4604);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& subSubsystems_45ef, const Packets_t& localVars_45f2);
	bool patternMatcher( const Udm::Object& subSubsystem_45f9, const Udm::Object& localVar_4602);
	void outputAppender( const CyberComposition::Simulink::Subsystem& subSubsystem_4612, const SFC::LocalVar& localVar_4614);

private:
	Packets_t* _subSubsystem_45f5;
	Packets_t* _localVar_45f6;
	Packets_t _subSubsystem_45f7;
	Packets_t _localVar_4600;
	class Match
	{
	public:
		CyberComposition::Simulink::Subsystem subSubsystem_460f;
		SFC::LocalVar localVar_4610;
		CyberComposition::Simulink::Subsystem subsystem_4611;
	};

	std::list< Match> _matches;
};

class Otherwise_4616
{
public:
	bool operator()( const Packets_t& subsystems_4617, const Packets_t& localVars_461a, Packets_t& subsystems_4619, Packets_t& localVars_461c);

protected:
	bool isInputUnique( const Udm::Object& subsystem_4623, const Udm::Object& localVar_462c);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& subsystems_4617, const Packets_t& localVars_461a);
	bool patternMatcher( const Udm::Object& subsystem_4621, const Udm::Object& localVar_462a);
	void outputAppender( const CyberComposition::Simulink::Subsystem& subsystem_4637, const SFC::LocalVar& localVar_4639);

private:
	Packets_t* _subsystem_461d;
	Packets_t* _localVar_461e;
	Packets_t _subsystem_461f;
	Packets_t _localVar_4628;
	class Match
	{
	public:
		CyberComposition::Simulink::Subsystem subsystem_4635;
		SFC::LocalVar localVar_4636;
	};

	std::list< Match> _matches;
};

class GetStoreSubsystem_4641
{
public:
	void operator()( const Packets_t& systems_4642, const Packets_t& localVars_4644, Packets_t& systems_4646, Packets_t& localVars_4647);

protected:
	void callStoreTest_46ff( const Packets_t& systems_464b, const Packets_t& localVars_464d);
	void callInitScope_4702( const Packets_t& subsystems_46df, const Packets_t& localVars_46e2);
	void callNextHigherSubsystem_4705( const Packets_t& subSubsystems_46b7, const Packets_t& localVars_46ba);

private:
	Packets_t* _system_4648;
	Packets_t* _localVar_4649;
};

class StoreTest_464a
{
public:
	void operator()( const Packets_t& systems_464b, const Packets_t& localVars_464d, Packets_t& systems_464f, Packets_t& localVars_4650, Packets_t& systems_4651, Packets_t& localVars_4652);

protected:
	void executeOne( const Packets_t& systems_464b, const Packets_t& localVars_464d);
	bool isInputUnique( const Udm::Object& system_4659, const Udm::Object& localVar_4660);

private:
	Packets_t* _system_4653;
	Packets_t* _localVar_4654;
	Packets_t* _system_4655;
	Packets_t* _localVar_4656;
	Packets_t _system_4657;
	Packets_t _localVar_465e;
};

class HasStore_4665
{
public:
	bool operator()( const Packets_t& subsystems_4666, const Packets_t& localVars_4669, Packets_t& subsystems_4668, Packets_t& localVars_466b);

protected:
	bool isInputUnique( const Udm::Object& subsystem_4672, const Udm::Object& localVar_467b);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::LocalVar& LocalVar, CyberComposition::Simulink::SF_Parameter& Parameter, CyberComposition::Simulink::Subsystem& Subsystem);
	void processInputPackets( const Packets_t& subsystems_4666, const Packets_t& localVars_4669);
	bool patternMatcher( const Udm::Object& subsystem_4670, const Udm::Object& localVar_4679);
	void outputAppender( const CyberComposition::Simulink::Subsystem& subsystem_468d, const SFC::LocalVar& localVar_468f);

private:
	Packets_t* _subsystem_466c;
	Packets_t* _localVar_466d;
	Packets_t _subsystem_466e;
	Packets_t _localVar_4677;
	class Match
	{
	public:
		CyberComposition::Simulink::Subsystem subsystem_4687;
		SFC::LocalVar localVar_4688;
		CyberComposition::Simulink::SF_Parameter parameter_4689;
	};

	std::list< Match> _matches;
};

class Otherwise_4691
{
public:
	bool operator()( const Packets_t& subsystems_4692, const Packets_t& localVars_4695, Packets_t& subsystems_4694, Packets_t& localVars_4697);

protected:
	bool isInputUnique( const Udm::Object& subsystem_469e, const Udm::Object& localVar_46a7);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& subsystems_4692, const Packets_t& localVars_4695);
	bool patternMatcher( const Udm::Object& subsystem_469c, const Udm::Object& localVar_46a5);
	void outputAppender( const CyberComposition::Simulink::Subsystem& subsystem_46b2, const SFC::LocalVar& localVar_46b4);

private:
	Packets_t* _subsystem_4698;
	Packets_t* _localVar_4699;
	Packets_t _subsystem_469a;
	Packets_t _localVar_46a3;
	class Match
	{
	public:
		CyberComposition::Simulink::Subsystem subsystem_46b0;
		SFC::LocalVar localVar_46b1;
	};

	std::list< Match> _matches;
};

class NextHigherSubsystem_46b6
{
public:
	void operator()( const Packets_t& subSubsystems_46b7, const Packets_t& localVars_46ba, Packets_t& subsystems_46b9, Packets_t& localVars_46bc);

protected:
	bool isInputUnique( const Udm::Object& subSubsystem_46c3, const Udm::Object& localVar_46cc);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& subSubsystems_46b7, const Packets_t& localVars_46ba);
	bool patternMatcher( const Udm::Object& subSubsystem_46c1, const Udm::Object& localVar_46ca);
	void effector();
	void outputAppender( const CyberComposition::Simulink::Subsystem& subsystem_46da, const SFC::LocalVar& localVar_46dc);

private:
	Packets_t* _subsystem_46bd;
	Packets_t* _localVar_46be;
	Packets_t _subSubsystem_46bf;
	Packets_t _localVar_46c8;
	class Match
	{
	public:
		CyberComposition::Simulink::Subsystem subSubsystem_46d7;
		SFC::LocalVar localVar_46d8;
		CyberComposition::Simulink::Subsystem subsystem_46d9;
	};

	std::list< Match> _matches;
};

class InitScope_46de
{
public:
	void operator()( const Packets_t& subsystems_46df, const Packets_t& localVars_46e2, Packets_t& subsystems_46e1, Packets_t& localVars_46e4);

protected:
	bool isInputUnique( const Udm::Object& subsystem_46eb, const Udm::Object& localVar_46f4);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& subsystems_46df, const Packets_t& localVars_46e2);
	bool patternMatcher( const Udm::Object& subsystem_46e9, const Udm::Object& localVar_46f2);
	void effector();
	void forwardInputs();

private:
	Packets_t* _subsystem_46e5;
	Packets_t* _localVar_46e6;
	Packets_t _subsystem_46e7;
	Packets_t _localVar_46f0;
	class Match
	{
	public:
		CyberComposition::Simulink::Subsystem subsystem_46fd;
		SFC::LocalVar localVar_46fe;
	};

	std::list< Match> _matches;
};

class GetContainingSubsystem_4708
{
public:
	void operator()( const Packets_t& sFStates_4709, const Packets_t& localVars_470c, Packets_t& subsystems_470b, Packets_t& localVars_470e);

protected:
	bool isInputUnique( const Udm::Object& sFState_4715, const Udm::Object& localVar_471e);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& sFStates_4709, const Packets_t& localVars_470c);
	bool patternMatcher( const Udm::Object& sFState_4713, const Udm::Object& localVar_471c);
	void effector();
	void outputAppender( const CyberComposition::Simulink::Subsystem& subsystem_472c, const SFC::LocalVar& localVar_472e);

private:
	Packets_t* _subsystem_470f;
	Packets_t* _localVar_4710;
	Packets_t _sFState_4711;
	Packets_t _localVar_471a;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_4729;
		SFC::LocalVar localVar_472a;
		CyberComposition::Simulink::Subsystem subsystem_472b;
	};

	std::list< Match> _matches;
};

class CreateDataVar_4730
{
public:
	void operator()( const Packets_t& sFStates_4731, const Packets_t& programs_4735, Packets_t& sFStates_4733, Packets_t& localVars_4734);

protected:
	bool isInputUnique( const Udm::Object& sFState_473d, const Udm::Object& program_4746);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::DT& DT, SFC::Program& Program, SFC::Project& Project, CyberComposition::Simulink::SFData& SFData, CyberComposition::Simulink::SFState& SFState, SFC::StateLabel& StateLabel, CyberComposition::Simulink::SF_TypeBase& TypeBase, CyberComposition::Simulink::TypeBaseRef& TypeBaseRef);
	void processInputPackets( const Packets_t& sFStates_4731, const Packets_t& programs_4735);
	bool patternMatcher( const Udm::Object& sFState_473b, const Udm::Object& program_4744);
	void effector();
	void outputAppender( const CyberComposition::Simulink::SFState& sFState_476d, const SFC::LocalVar& localVar_476f);

private:
	Packets_t* _sFState_4737;
	Packets_t* _localVar_4738;
	Packets_t _sFState_4739;
	Packets_t _program_4742;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_475c;
		SFC::Program program_475d;
		SFC::StateLabel stateLabel_475e;
		CyberComposition::Simulink::SFData sFData_475f;
		CyberComposition::Simulink::TypeBaseRef typeBaseRef_4760;
		CyberComposition::Simulink::SF_TypeBase typeBase_4761;
		SFC::DT dT_4762;
		SFC::Project project_4763;
	};

	std::list< Match> _matches;
};

class CreateEventVar_4783
{
public:
	void operator()( const Packets_t& states_4784, const Packets_t& programs_4786, Packets_t& states_4788, Packets_t& programs_4789);

protected:
	void callCreateEventVar_47ba( const Packets_t& sFStates_478d, const Packets_t& programs_478f);

private:
	Packets_t* _state_478a;
	Packets_t* _program_478b;
};

class CreateEventVar_478c
{
public:
	void operator()( const Packets_t& sFStates_478d, const Packets_t& programs_478f);

protected:
	bool isInputUnique( const Udm::Object& sFState_4795, const Udm::Object& program_479e);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& sFStates_478d, const Packets_t& programs_478f);
	bool patternMatcher( const Udm::Object& sFState_4793, const Udm::Object& program_479c);
	void effector();

private:
	Packets_t _sFState_4791;
	Packets_t _program_479a;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_47b2;
		SFC::Program program_47b3;
		SFC::StateLabel stateLabel_47b4;
		CyberComposition::Simulink::SFEvent sFEvent_47b5;
		CyberComposition::Simulink::TypeBaseRef typeBaseRef_47b6;
		CyberComposition::Simulink::SF_TypeBase typeBase_47b7;
		SFC::DT dT_47b8;
	};

	std::list< Match> _matches;
};

class CheckStateDecomp_47bd
{
public:
	void operator()( const Packets_t& states_47be, const Packets_t& programs_47c0, Packets_t& states_47c2, Packets_t& programs_47c3, Packets_t& states_47c4, Packets_t& programs_47c5);

protected:
	void executeOne( const Packets_t& states_47be, const Packets_t& programs_47c0);
	bool isInputUnique( const Udm::Object& state_47cc, const Udm::Object& program_47d3);

private:
	Packets_t* _state_47c6;
	Packets_t* _program_47c7;
	Packets_t* _state_47c8;
	Packets_t* _program_47c9;
	Packets_t _state_47ca;
	Packets_t _program_47d1;
};

class IsFuncState_47d8
{
public:
	bool operator()( const Packets_t& sFStates_47d9, const Packets_t& programs_47dc, Packets_t& sFStates_47db, Packets_t& programs_47de);

protected:
	bool isInputUnique( const Udm::Object& sFState_47e5, const Udm::Object& program_47ee);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::CompoundStatement& Program, CyberComposition::Simulink::SFState& SFState);
	void processInputPackets( const Packets_t& sFStates_47d9, const Packets_t& programs_47dc);
	bool patternMatcher( const Udm::Object& sFState_47e3, const Udm::Object& program_47ec);
	void outputAppender( const CyberComposition::Simulink::SFState& sFState_47fb, const SFC::CompoundStatement& program_47fd);

private:
	Packets_t* _sFState_47df;
	Packets_t* _program_47e0;
	Packets_t _sFState_47e1;
	Packets_t _program_47ea;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_47f7;
		SFC::CompoundStatement program_47f8;
	};

	std::list< Match> _matches;
};

class Otherwise_47ff
{
public:
	bool operator()( const Packets_t& sFStates_4800, const Packets_t& programs_4803, Packets_t& sFStates_4802, Packets_t& programs_4805);

protected:
	bool isInputUnique( const Udm::Object& sFState_480c, const Udm::Object& program_4815);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& sFStates_4800, const Packets_t& programs_4803);
	bool patternMatcher( const Udm::Object& sFState_480a, const Udm::Object& program_4813);
	void outputAppender( const CyberComposition::Simulink::SFState& sFState_4820, const SFC::CompoundStatement& program_4822);

private:
	Packets_t* _sFState_4806;
	Packets_t* _program_4807;
	Packets_t _sFState_4808;
	Packets_t _program_4811;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_481e;
		SFC::CompoundStatement program_481f;
	};

	std::list< Match> _matches;
};

class CreateFuncStateFunctions_4833
{
public:
	void operator()( const Packets_t& states_4834, const Packets_t& programs_4836, Packets_t& states_4838, Packets_t& programs_4839);

protected:
	void callPopulateGRFSFunction_4988( const Packets_t& states_494b, const Packets_t& programs_494d);
	void callPopulateEMFSFunction_498b( const Packets_t& states_483d, const Packets_t& programs_483f);
	void callGetSubStates_498e( const Packets_t& sFStates_4223, const Packets_t& parStmnts_4226);

private:
	Packets_t* _state_483a;
	Packets_t* _program_483b;
};

class PopulateEMFSFunction_483c
{
public:
	void operator()( const Packets_t& states_483d, const Packets_t& programs_483f, Packets_t& states_4841, Packets_t& programs_4842);

protected:
	void callCreateEMFuncStateFunction_4944( const Packets_t& sFStates_4846, const Packets_t& programs_4849);
	void callFuncArgsAndVars_4947( const Packets_t& states_4873, const Packets_t& fcns_4875);

private:
	Packets_t* _state_4843;
	Packets_t* _program_4844;
};

class CreateEMFuncStateFunction_4845
{
public:
	void operator()( const Packets_t& sFStates_4846, const Packets_t& programs_4849, Packets_t& sFStates_4848, Packets_t& execs_484b);

protected:
	bool isInputUnique( const Udm::Object& sFState_4852, const Udm::Object& program_485b);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::Program& Program, CyberComposition::Simulink::SFState& SFState, SFC::StateLabel& StateLabel);
	void processInputPackets( const Packets_t& sFStates_4846, const Packets_t& programs_4849);
	bool patternMatcher( const Udm::Object& sFState_4850, const Udm::Object& program_4859);
	void effector();
	void outputAppender( const CyberComposition::Simulink::SFState& sFState_486e, const SFC::Function& exec_4870);

private:
	Packets_t* _sFState_484c;
	Packets_t* _exec_484d;
	Packets_t _sFState_484e;
	Packets_t _program_4857;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_4867;
		SFC::Program program_4868;
		SFC::StateLabel stateLabel_4869;
	};

	std::list< Match> _matches;
};

class FuncArgsAndVars_4872
{
public:
	void operator()( const Packets_t& states_4873, const Packets_t& fcns_4875, Packets_t& states_4877, Packets_t& fcns_4878);

protected:
	void callFuncInputArgs_493b( const Packets_t& states_48bd, const Packets_t& fcns_48bf);
	void callFuncOutputArgs_493e( const Packets_t& states_487c, const Packets_t& fcns_487e);
	void callFuncLocalVars_4941( const Packets_t& states_48fb, const Packets_t& fcns_48fd);

private:
	Packets_t* _state_4879;
	Packets_t* _fcn_487a;
};

class FuncOutputArgs_487b
{
public:
	void operator()( const Packets_t& states_487c, const Packets_t& fcns_487e, Packets_t& states_4880, Packets_t& fcns_4881);

protected:
	void callOutputArgs_48b9( const Packets_t& sFStates_4885, const Packets_t& execs_4887);

private:
	Packets_t* _state_4882;
	Packets_t* _fcn_4883;
};

class OutputArgs_4884
{
public:
	void operator()( const Packets_t& sFStates_4885, const Packets_t& execs_4887);

protected:
	bool isInputUnique( const Udm::Object& sFState_488d, const Udm::Object& exec_4896);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::DT& DT, SFC::Function& Exec, CyberComposition::Simulink::SFData& SFData, CyberComposition::Simulink::SFState& SFState, SFC::StateLabel& StateLabel, CyberComposition::Simulink::SF_TypeBase& TypeBase, CyberComposition::Simulink::TypeBaseRef& TypeBaseRef);
	void processInputPackets( const Packets_t& sFStates_4885, const Packets_t& execs_4887);
	bool patternMatcher( const Udm::Object& sFState_488b, const Udm::Object& exec_4894);
	void effector();

private:
	Packets_t _sFState_4889;
	Packets_t _exec_4892;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_48aa;
		SFC::Function exec_48ab;
		SFC::StateLabel stateLabel_48ac;
		CyberComposition::Simulink::SFData sFData_48ad;
		CyberComposition::Simulink::TypeBaseRef typeBaseRef_48ae;
		CyberComposition::Simulink::SF_TypeBase typeBase_48af;
		SFC::DT dT_48b0;
	};

	std::list< Match> _matches;
};

class FuncInputArgs_48bc
{
public:
	void operator()( const Packets_t& states_48bd, const Packets_t& fcns_48bf, Packets_t& states_48c1, Packets_t& fcns_48c2);

protected:
	void callInputArgs_48f7( const Packets_t& sFStates_48c6, const Packets_t& execs_48c8);

private:
	Packets_t* _state_48c3;
	Packets_t* _fcn_48c4;
};

class InputArgs_48c5
{
public:
	void operator()( const Packets_t& sFStates_48c6, const Packets_t& execs_48c8);

protected:
	bool isInputUnique( const Udm::Object& sFState_48ce, const Udm::Object& exec_48d7);
	bool isGuardTrue( SFC::DT& DT, SFC::Function& Exec, CyberComposition::Simulink::SFData& SFData, CyberComposition::Simulink::SFState& SFState, SFC::StateLabel& StateLabel, CyberComposition::Simulink::SF_TypeBase& TypeBase, CyberComposition::Simulink::TypeBaseRef& TypeBaseRef);
	void processInputPackets( const Packets_t& sFStates_48c6, const Packets_t& execs_48c8);
	bool patternMatcher( const Udm::Object& sFState_48cc, const Udm::Object& exec_48d5);
	void effector();

private:
	Packets_t _sFState_48ca;
	Packets_t _exec_48d3;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_48e8;
		SFC::Function exec_48e9;
		SFC::StateLabel stateLabel_48ea;
		CyberComposition::Simulink::SFData sFData_48eb;
		CyberComposition::Simulink::TypeBaseRef typeBaseRef_48ec;
		CyberComposition::Simulink::SF_TypeBase typeBase_48ed;
		SFC::DT dT_48ee;
	};

	std::list< Match> _matches;
};

class FuncLocalVars_48fa
{
public:
	void operator()( const Packets_t& states_48fb, const Packets_t& fcns_48fd, Packets_t& states_48ff, Packets_t& fcns_4900);

protected:
	void callFuncLocalVars_4938( const Packets_t& sFStates_4904, const Packets_t& execs_4906);

private:
	Packets_t* _state_4901;
	Packets_t* _fcn_4902;
};

class FuncLocalVars_4903
{
public:
	void operator()( const Packets_t& sFStates_4904, const Packets_t& execs_4906);

protected:
	bool isInputUnique( const Udm::Object& sFState_490c, const Udm::Object& exec_4915);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::DT& DT, SFC::Function& Exec, CyberComposition::Simulink::SFData& SFData, CyberComposition::Simulink::SFState& SFState, SFC::StateLabel& StateLabel, CyberComposition::Simulink::SF_TypeBase& TypeBase, CyberComposition::Simulink::TypeBaseRef& TypeBaseRef);
	void processInputPackets( const Packets_t& sFStates_4904, const Packets_t& execs_4906);
	bool patternMatcher( const Udm::Object& sFState_490a, const Udm::Object& exec_4913);
	void effector();

private:
	Packets_t _sFState_4908;
	Packets_t _exec_4911;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_4929;
		SFC::Function exec_492a;
		SFC::StateLabel stateLabel_492b;
		CyberComposition::Simulink::SFData sFData_492c;
		CyberComposition::Simulink::TypeBaseRef typeBaseRef_492d;
		CyberComposition::Simulink::SF_TypeBase typeBase_492e;
		SFC::DT dT_492f;
	};

	std::list< Match> _matches;
};

class PopulateGRFSFunction_494a
{
public:
	void operator()( const Packets_t& states_494b, const Packets_t& programs_494d, Packets_t& states_494f, Packets_t& programs_4950);

protected:
	void callCreateGRFuncStateFunction_4982( const Packets_t& sFStates_4954, const Packets_t& programs_4957);
	void callFuncArgsAndVars_4985( const Packets_t& states_4873, const Packets_t& fcns_4875);

private:
	Packets_t* _state_4951;
	Packets_t* _program_4952;
};

class CreateGRFuncStateFunction_4953
{
public:
	void operator()( const Packets_t& sFStates_4954, const Packets_t& programs_4957, Packets_t& sFStates_4956, Packets_t& execs_4959);

protected:
	bool isInputUnique( const Udm::Object& sFState_4960, const Udm::Object& program_4969);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::Program& Program, CyberComposition::Simulink::SFState& SFState, SFC::StateLabel& StateLabel);
	void processInputPackets( const Packets_t& sFStates_4954, const Packets_t& programs_4957);
	bool patternMatcher( const Udm::Object& sFState_495e, const Udm::Object& program_4967);
	void effector();
	void outputAppender( const CyberComposition::Simulink::SFState& sFState_497e, const SFC::Function& exec_4980);

private:
	Packets_t* _sFState_495a;
	Packets_t* _exec_495b;
	Packets_t _sFState_495c;
	Packets_t _program_4965;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_4975;
		SFC::Program program_4976;
		SFC::StateLabel stateLabel_4977;
	};

	std::list< Match> _matches;
};

class PopulateFunctions_4991
{
public:
	void operator()( const Packets_t& states_4992, const Packets_t& programs_4994, Packets_t& states_4996, Packets_t& programs_4997);

protected:
	void callNoFunctionStates_570a( const Packets_t& states_4c43, const Packets_t& programs_4c45);
	void callPopulateEnterFunction_570d( const Packets_t& states_4c81, const Packets_t& programs_4c83);
	void callPopulateExecFunction_5710( const Packets_t& states_503d, const Packets_t& programs_503f);
	void callPopulateExitFunction_5713( const Packets_t& states_54e4, const Packets_t& programs_54e6);
	void callPopulateStatusFunction_5716( const Packets_t& states_499b, const Packets_t& programs_499d);
	void callGetSubStates_5719( const Packets_t& sFStates_4223, const Packets_t& parStmnts_4226);

private:
	Packets_t* _state_4998;
	Packets_t* _program_4999;
};

class PopulateStatusFunction_499a
{
public:
	void operator()( const Packets_t& states_499b, const Packets_t& programs_499d, Packets_t& states_499f, Packets_t& programs_49a0);

protected:
	void callGetStatusFunction_4c24( const Packets_t& sFStates_4bfd);
	void callHasChildren_4c26( const Packets_t& states_49c3, const Packets_t& statuss_49c5);
	void callElimState_4c29( const Packets_t& sFStates_49a4, const Packets_t& statuss_49a6);
	void callnewIndent_4c2c( const Packets_t& sFStates_4a2f, const Packets_t& statuss_4a32);
	void callReturnValue_4c2f( const Packets_t& statuss_4a6a);
	void callDecompTest_4c31( const Packets_t& states_4ae8, const Packets_t& statuss_4aea, const Packets_t& svs_4aec);
	void callGetChildStates_4c35( const Packets_t& sFStates_4b8a, const Packets_t& compoundStatements_4b8e, const Packets_t& stateVars_4b91);
	void callCreateConditionalGroup_4c39( const Packets_t& sFStates_4bc7, const Packets_t& statuss_4bca, const Packets_t& stateVars_4bcd);
	void callCreateConditionals_4c3d( const Packets_t& sFStates_4a83, const Packets_t& sFChildStates_4a86, const Packets_t& compoundStatements_4a88, const Packets_t& stateVars_4a8a);

private:
	Packets_t* _state_49a1;
	Packets_t* _program_49a2;
};

class ElimState_49a3
{
public:
	void operator()( const Packets_t& sFStates_49a4, const Packets_t& statuss_49a6, Packets_t& statuss_49a8);

protected:
	bool isInputUnique( const Udm::Object& sFState_49ae, const Udm::Object& status_49b7);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& sFStates_49a4, const Packets_t& statuss_49a6);
	bool patternMatcher( const Udm::Object& sFState_49ac, const Udm::Object& status_49b5);
	void effector();
	void forwardInputs();

private:
	Packets_t* _status_49a9;
	Packets_t _sFState_49aa;
	Packets_t _status_49b3;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_49c0;
		SFC::Function status_49c1;
	};

	std::list< Match> _matches;
};

class HasChildren_49c2
{
public:
	void operator()( const Packets_t& states_49c3, const Packets_t& statuss_49c5, Packets_t& states_49c7, Packets_t& statuss_49c8, Packets_t& states_49c9, Packets_t& statuss_49ca);

protected:
	void executeOne( const Packets_t& states_49c3, const Packets_t& statuss_49c5);
	bool isInputUnique( const Udm::Object& state_49d1, const Udm::Object& status_49d8);

private:
	Packets_t* _state_49cb;
	Packets_t* _status_49cc;
	Packets_t* _state_49cd;
	Packets_t* _status_49ce;
	Packets_t _state_49cf;
	Packets_t _status_49d6;
};

class HasChildren_49dd
{
public:
	bool operator()( const Packets_t& sFStates_49de, const Packets_t& statuss_49e1, Packets_t& sFStates_49e0, Packets_t& statuss_49e3);

protected:
	bool isInputUnique( const Udm::Object& sFState_49ea, const Udm::Object& status_49f3);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( CyberComposition::Simulink::SFState& SFChildState, CyberComposition::Simulink::SFState& SFState, SFC::Function& Status);
	void processInputPackets( const Packets_t& sFStates_49de, const Packets_t& statuss_49e1);
	bool patternMatcher( const Udm::Object& sFState_49e8, const Udm::Object& status_49f1);
	void outputAppender( const CyberComposition::Simulink::SFState& sFState_4a05, const SFC::Function& status_4a07);

private:
	Packets_t* _sFState_49e4;
	Packets_t* _status_49e5;
	Packets_t _sFState_49e6;
	Packets_t _status_49ef;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_49ff;
		SFC::Function status_4a00;
		CyberComposition::Simulink::SFState sFChildState_4a01;
	};

	std::list< Match> _matches;
};

class NoChildren_4a09
{
public:
	bool operator()( const Packets_t& sFStates_4a0a, const Packets_t& statuss_4a0d, Packets_t& sFStates_4a0c, Packets_t& statuss_4a0f);

protected:
	bool isInputUnique( const Udm::Object& sFState_4a16, const Udm::Object& status_4a1f);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& sFStates_4a0a, const Packets_t& statuss_4a0d);
	bool patternMatcher( const Udm::Object& sFState_4a14, const Udm::Object& status_4a1d);
	void outputAppender( const CyberComposition::Simulink::SFState& sFState_4a2a, const SFC::Function& status_4a2c);

private:
	Packets_t* _sFState_4a10;
	Packets_t* _status_4a11;
	Packets_t _sFState_4a12;
	Packets_t _status_4a1b;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_4a28;
		SFC::Function status_4a29;
	};

	std::list< Match> _matches;
};

class NewIndent_4a2e
{
public:
	void operator()( const Packets_t& sFStates_4a2f, const Packets_t& statuss_4a32, Packets_t& sFStates_4a31, Packets_t& statuss_4a34, Packets_t& stateVars_4a35);

protected:
	bool isInputUnique( const Udm::Object& sFState_4a3d, const Udm::Object& status_4a46);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& sFStates_4a2f, const Packets_t& statuss_4a32);
	bool patternMatcher( const Udm::Object& sFState_4a3b, const Udm::Object& status_4a44);
	void effector();
	void outputAppender( const CyberComposition::Simulink::SFState& sFState_4a63, const SFC::Function& status_4a65, const SFC::StateVar& stateVar_4a67);

private:
	Packets_t* _sFState_4a36;
	Packets_t* _status_4a37;
	Packets_t* _stateVar_4a38;
	Packets_t _sFState_4a39;
	Packets_t _status_4a42;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_4a57;
		SFC::Function status_4a58;
		SFC::Arg arg_4a59;
		SFC::Program program_4a5a;
		SFC::StateVar stateVar_4a5b;
	};

	std::list< Match> _matches;
};

class ReturnValue_4a69
{
public:
	void operator()( const Packets_t& statuss_4a6a);

protected:
	bool isInputUnique( const Udm::Object& status_4a70);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::Function& Status, SFC::LocalVar& retval);
	void processInputPackets( const Packets_t& statuss_4a6a);
	bool patternMatcher( const Udm::Object& status_4a6e);
	void effector();

private:
	Packets_t _status_4a6c;
	class Match
	{
	public:
		SFC::Function status_4a7c;
		SFC::LocalVar retval_4a7d;
	};

	std::list< Match> _matches;
};

class CreateConditionals_4a82
{
public:
	void operator()( const Packets_t& sFStates_4a83, const Packets_t& sFChildStates_4a86, const Packets_t& compoundStatements_4a88, const Packets_t& stateVars_4a8a, Packets_t& statuss_4a85);

protected:
	bool isInputUnique( const Udm::Object& sFState_4a91, const Udm::Object& sFChildState_4a9a, const Udm::Object& compoundStatement_4aa3, const Udm::Object& stateVar_4aac);
	bool isGuardTrue( SFC::StateLabel& ChildStateLabel, SFC::Function& ChildStatus, SFC::CompoundStatement& CompoundStatement, SFC::Arg& IndentArg, CyberComposition::Simulink::SFState& SFChildState, CyberComposition::Simulink::SFState& SFState, SFC::StateLabel& StateLabel, SFC::StateVar& StateVar, SFC::Function& Status, SFC::LocalVar& newIndent, SFC::LocalVar& retval);
	void processInputPackets( const Packets_t& sFStates_4a83, const Packets_t& sFChildStates_4a86, const Packets_t& compoundStatements_4a88, const Packets_t& stateVars_4a8a);
	bool patternMatcher( const Udm::Object& sFState_4a8f, const Udm::Object& sFChildState_4a98, const Udm::Object& compoundStatement_4aa1, const Udm::Object& stateVar_4aaa);
	void effector();
	void outputAppender( const SFC::Function& status_4ae5);

private:
	Packets_t* _status_4a8c;
	Packets_t _sFState_4a8d;
	Packets_t _sFChildState_4a96;
	Packets_t _compoundStatement_4a9f;
	Packets_t _stateVar_4aa8;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_4ac7;
		CyberComposition::Simulink::SFState sFChildState_4ac8;
		SFC::CompoundStatement compoundStatement_4ac9;
		SFC::StateVar stateVar_4aca;
		SFC::Function status_4acb;
		SFC::LocalVar retval_4acc;
		SFC::Arg indentArg_4acd;
		SFC::Function childStatus_4ace;
		SFC::LocalVar newIndent_4acf;
		SFC::StateLabel stateLabel_4ad0;
		SFC::StateLabel childStateLabel_4ad1;
	};

	std::list< Match> _matches;
};

class DecompTest_4ae7
{
public:
	void operator()( const Packets_t& states_4ae8, const Packets_t& statuss_4aea, const Packets_t& svs_4aec, Packets_t& states_4aee, Packets_t& statuss_4aef, Packets_t& svs_4af0, Packets_t& states_4af1, Packets_t& statuss_4af2, Packets_t& svs_4af3);

protected:
	void executeOne( const Packets_t& states_4ae8, const Packets_t& statuss_4aea, const Packets_t& svs_4aec);
	bool isInputUnique( const Udm::Object& state_4afc, const Udm::Object& status_4b03, const Udm::Object& sv_4b0a);

private:
	Packets_t* _state_4af4;
	Packets_t* _status_4af5;
	Packets_t* _sv_4af6;
	Packets_t* _state_4af7;
	Packets_t* _status_4af8;
	Packets_t* _sv_4af9;
	Packets_t _state_4afa;
	Packets_t _status_4b01;
	Packets_t _sv_4b08;
};

class AndState_4b0f
{
public:
	bool operator()( const Packets_t& sFStates_4b10, const Packets_t& statuss_4b13, const Packets_t& stateVars_4b16, Packets_t& sFStates_4b12, Packets_t& statuss_4b15, Packets_t& stateVars_4b18);

protected:
	bool isInputUnique( const Udm::Object& sFState_4b20, const Udm::Object& status_4b29, const Udm::Object& stateVar_4b32);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( CyberComposition::Simulink::SFState& SFChildState, CyberComposition::Simulink::SFState& SFState, SFC::StateVar& StateVar, SFC::Function& Status);
	void processInputPackets( const Packets_t& sFStates_4b10, const Packets_t& statuss_4b13, const Packets_t& stateVars_4b16);
	bool patternMatcher( const Udm::Object& sFState_4b1e, const Udm::Object& status_4b27, const Udm::Object& stateVar_4b30);
	void outputAppender( const CyberComposition::Simulink::SFState& sFState_4b46, const SFC::Function& status_4b48, const SFC::StateVar& stateVar_4b4a);

private:
	Packets_t* _sFState_4b19;
	Packets_t* _status_4b1a;
	Packets_t* _stateVar_4b1b;
	Packets_t _sFState_4b1c;
	Packets_t _status_4b25;
	Packets_t _stateVar_4b2e;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_4b3e;
		SFC::Function status_4b3f;
		SFC::StateVar stateVar_4b40;
		CyberComposition::Simulink::SFState sFChildState_4b41;
	};

	std::list< Match> _matches;
};

class OrState_4b4c
{
public:
	bool operator()( const Packets_t& sFStates_4b4d, const Packets_t& statuss_4b50, const Packets_t& stateVars_4b53, Packets_t& sFStates_4b4f, Packets_t& statuss_4b52, Packets_t& stateVars_4b55);

protected:
	bool isInputUnique( const Udm::Object& sFState_4b5d, const Udm::Object& status_4b66, const Udm::Object& stateVar_4b6f);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( CyberComposition::Simulink::SFState& SFChildState, CyberComposition::Simulink::SFState& SFState, SFC::StateVar& StateVar, SFC::Function& Status);
	void processInputPackets( const Packets_t& sFStates_4b4d, const Packets_t& statuss_4b50, const Packets_t& stateVars_4b53);
	bool patternMatcher( const Udm::Object& sFState_4b5b, const Udm::Object& status_4b64, const Udm::Object& stateVar_4b6d);
	void outputAppender( const CyberComposition::Simulink::SFState& sFState_4b83, const SFC::Function& status_4b85, const SFC::StateVar& stateVar_4b87);

private:
	Packets_t* _sFState_4b56;
	Packets_t* _status_4b57;
	Packets_t* _stateVar_4b58;
	Packets_t _sFState_4b59;
	Packets_t _status_4b62;
	Packets_t _stateVar_4b6b;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_4b7b;
		SFC::Function status_4b7c;
		SFC::StateVar stateVar_4b7d;
		CyberComposition::Simulink::SFState sFChildState_4b7e;
	};

	std::list< Match> _matches;
};

class GetChildStates_4b89
{
public:
	void operator()( const Packets_t& sFStates_4b8a, const Packets_t& compoundStatements_4b8e, const Packets_t& stateVars_4b91, Packets_t& sFStates_4b8c, Packets_t& sFChildStates_4b8d, Packets_t& compoundStatements_4b90, Packets_t& stateVars_4b93);

protected:
	bool isInputUnique( const Udm::Object& sFState_4b9c, const Udm::Object& compoundStatement_4ba5, const Udm::Object& stateVar_4bae);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& sFStates_4b8a, const Packets_t& compoundStatements_4b8e, const Packets_t& stateVars_4b91);
	bool patternMatcher( const Udm::Object& sFState_4b9a, const Udm::Object& compoundStatement_4ba3, const Udm::Object& stateVar_4bac);
	void effector();
	void outputAppender( const CyberComposition::Simulink::SFState& sFState_4bbe, const CyberComposition::Simulink::SFState& sFChildState_4bc0, const SFC::CompoundStatement& compoundStatement_4bc2, const SFC::StateVar& stateVar_4bc4);
	void sortOutputs();

private:
	Packets_t* _sFState_4b94;
	Packets_t* _sFChildState_4b95;
	Packets_t* _compoundStatement_4b96;
	Packets_t* _stateVar_4b97;
	Packets_t _sFState_4b98;
	Packets_t _compoundStatement_4ba1;
	Packets_t _stateVar_4baa;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_4bba;
		SFC::CompoundStatement compoundStatement_4bbb;
		SFC::StateVar stateVar_4bbc;
		CyberComposition::Simulink::SFState sFChildState_4bbd;
	};

	std::list< Match> _matches;
};

class CreateConditionalGroup_4bc6
{
public:
	void operator()( const Packets_t& sFStates_4bc7, const Packets_t& statuss_4bca, const Packets_t& stateVars_4bcd, Packets_t& sFStates_4bc9, Packets_t& conditionalGroups_4bcc, Packets_t& stateVars_4bcf);

protected:
	bool isInputUnique( const Udm::Object& sFState_4bd7, const Udm::Object& status_4be0, const Udm::Object& stateVar_4be9);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& sFStates_4bc7, const Packets_t& statuss_4bca, const Packets_t& stateVars_4bcd);
	bool patternMatcher( const Udm::Object& sFState_4bd5, const Udm::Object& status_4bde, const Udm::Object& stateVar_4be7);
	void effector();
	void outputAppender( const CyberComposition::Simulink::SFState& sFState_4bf6, const SFC::ConditionalGroup& conditionalGroup_4bf8, const SFC::StateVar& stateVar_4bfa);

private:
	Packets_t* _sFState_4bd0;
	Packets_t* _conditionalGroup_4bd1;
	Packets_t* _stateVar_4bd2;
	Packets_t _sFState_4bd3;
	Packets_t _status_4bdc;
	Packets_t _stateVar_4be5;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_4bf2;
		SFC::Function status_4bf3;
		SFC::StateVar stateVar_4bf4;
	};

	std::list< Match> _matches;
};

class GetStatusFunction_4bfc
{
public:
	void operator()( const Packets_t& sFStates_4bfd, Packets_t& sFStates_4bff, Packets_t& statuss_4c00);

protected:
	bool isInputUnique( const Udm::Object& sFState_4c07);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& sFStates_4bfd);
	bool patternMatcher( const Udm::Object& sFState_4c05);
	void effector();
	void outputAppender( const CyberComposition::Simulink::SFState& sFState_4c20, const SFC::Function& status_4c22);

private:
	Packets_t* _sFState_4c01;
	Packets_t* _status_4c02;
	Packets_t _sFState_4c03;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_4c16;
		SFC::Function status_4c17;
		SFC::Arg arg_4c18;
	};

	std::list< Match> _matches;
};

class NoFunctionStates_4c42
{
public:
	void operator()( const Packets_t& states_4c43, const Packets_t& programs_4c45, Packets_t& states_4c47, Packets_t& programs_4c48);

protected:
	void executeOne( const Packets_t& states_4c43, const Packets_t& programs_4c45);
	bool isInputUnique( const Udm::Object& state_4c4d, const Udm::Object& program_4c54);

private:
	Packets_t* _state_4c49;
	Packets_t* _program_4c4a;
	Packets_t _state_4c4b;
	Packets_t _program_4c52;
};

class NotFunctionState_4c59
{
public:
	bool operator()( const Packets_t& sFStates_4c5a, const Packets_t& programs_4c5d, Packets_t& sFStates_4c5c, Packets_t& programs_4c5f);

protected:
	bool isInputUnique( const Udm::Object& sFState_4c66, const Udm::Object& program_4c6f);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::Program& Program, CyberComposition::Simulink::SFState& SFState);
	void processInputPackets( const Packets_t& sFStates_4c5a, const Packets_t& programs_4c5d);
	bool patternMatcher( const Udm::Object& sFState_4c64, const Udm::Object& program_4c6d);
	void outputAppender( const CyberComposition::Simulink::SFState& sFState_4c7c, const SFC::Program& program_4c7e);

private:
	Packets_t* _sFState_4c60;
	Packets_t* _program_4c61;
	Packets_t _sFState_4c62;
	Packets_t _program_4c6b;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_4c78;
		SFC::Program program_4c79;
	};

	std::list< Match> _matches;
};

class PopulateEnterFunction_4c80
{
public:
	void operator()( const Packets_t& states_4c81, const Packets_t& programs_4c83, Packets_t& states_4c85, Packets_t& programs_4c86);

protected:
	void callTestEAIOff_5030( const Packets_t& states_4c8a);
	void callReturnAtTopState_5032( const Packets_t& states_5003);
	void callMode0or1_5034( const Packets_t& states_4f7a);
	void callMode0or1or2_5036( const Packets_t& states_4f39);
	void callMode0or2_5038( const Packets_t& states_4e0a);
	void callMode0or3_503a( const Packets_t& states_4ccc);

private:
	Packets_t* _state_4c87;
	Packets_t* _program_4c88;
};

class TestEAIOff_4c89
{
public:
	void operator()( const Packets_t& states_4c8a, Packets_t& states_4c8c, Packets_t& states_4c8d);

protected:
	void executeOne( const Packets_t& states_4c8a);
	bool isInputUnique( const Udm::Object& state_4c92);

private:
	Packets_t* _state_4c8e;
	Packets_t* _state_4c8f;
	Packets_t _state_4c90;
};

class EAIOff_4c97
{
public:
	bool operator()( const Packets_t& sFStates_4c98, Packets_t& sFStates_4c9a);

protected:
	bool isInputUnique( const Udm::Object& sFState_4ca0);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( CyberComposition::Simulink::SF_Parameter& Parameter, CyberComposition::Simulink::SFState& SFState, CyberComposition::Simulink::Subsystem& Subsystem);
	void processInputPackets( const Packets_t& sFStates_4c98);
	bool patternMatcher( const Udm::Object& sFState_4c9e);
	void outputAppender( const CyberComposition::Simulink::SFState& sFState_4cb4);

private:
	Packets_t* _sFState_4c9b;
	Packets_t _sFState_4c9c;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_4cae;
		CyberComposition::Simulink::SF_Parameter parameter_4caf;
		CyberComposition::Simulink::Subsystem subsystem_4cb0;
	};

	std::list< Match> _matches;
};

class Otherwise_4cb6
{
public:
	bool operator()( const Packets_t& sFStates_4cb7, Packets_t& sFStates_4cb9);

protected:
	bool isInputUnique( const Udm::Object& sFState_4cbf);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& sFStates_4cb7);
	bool patternMatcher( const Udm::Object& sFState_4cbd);
	void outputAppender( const CyberComposition::Simulink::SFState& sFState_4cc9);

private:
	Packets_t* _sFState_4cba;
	Packets_t _sFState_4cbb;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_4cc8;
	};

	std::list< Match> _matches;
};

class Mode0or3_4ccb
{
public:
	void operator()( const Packets_t& states_4ccc, Packets_t& states_4cce);

protected:
	void callTestMode03_4e01( const Packets_t& sFStates_4dd8);
	void callCallLOSibs_4e03( const Packets_t& states_4cd1, const Packets_t& css_4cd3);
	void callCallParent_4e06( const Packets_t& sFStates_4d9a, const Packets_t& conditionalBlocks_4d9c);

private:
	Packets_t* _state_4ccf;
};

class CallLOSibs_4cd0
{
public:
	void operator()( const Packets_t& states_4cd1, const Packets_t& css_4cd3, Packets_t& states_4cd5, Packets_t& css_4cd6);

protected:
	void callIsAndState_4d8f( const Packets_t& states_4d0b, const Packets_t& css_4d0d);
	void callGetLOSibs_4d92( const Packets_t& sFStates_4cda, const Packets_t& compoundStatements_4cde);
	void callCallSibsOrChildren_4d95( const Packets_t& sFStates_4d46, const Packets_t& sFSOCStates_4d48, const Packets_t& compoundStatements_4d4a);

private:
	Packets_t* _state_4cd7;
	Packets_t* _cs_4cd8;
};

class GetLOSibs_4cd9
{
public:
	void operator()( const Packets_t& sFStates_4cda, const Packets_t& compoundStatements_4cde, Packets_t& sFStates_4cdc, Packets_t& sFSibStates_4cdd, Packets_t& compoundStatements_4ce0);

protected:
	bool isInputUnique( const Udm::Object& sFState_4ce8, const Udm::Object& compoundStatement_4cf1);
	bool isGuardTrue( SFC::CompoundStatement& CompoundStatement, CyberComposition::Simulink::SFState& SFParentState, CyberComposition::Simulink::SFState& SFSibState, CyberComposition::Simulink::SFState& SFState);
	void processInputPackets( const Packets_t& sFStates_4cda, const Packets_t& compoundStatements_4cde);
	bool patternMatcher( const Udm::Object& sFState_4ce6, const Udm::Object& compoundStatement_4cef);
	void effector();
	void outputAppender( const CyberComposition::Simulink::SFState& sFState_4d04, const CyberComposition::Simulink::SFState& sFSibState_4d06, const SFC::CompoundStatement& compoundStatement_4d08);
	void sortOutputs();

private:
	Packets_t* _sFState_4ce1;
	Packets_t* _sFSibState_4ce2;
	Packets_t* _compoundStatement_4ce3;
	Packets_t _sFState_4ce4;
	Packets_t _compoundStatement_4ced;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_4cfc;
		SFC::CompoundStatement compoundStatement_4cfd;
		CyberComposition::Simulink::SFState sFParentState_4cfe;
		CyberComposition::Simulink::SFState sFSibState_4cff;
	};

	std::list< Match> _matches;
};

class IsAndState_4d0a
{
public:
	void operator()( const Packets_t& states_4d0b, const Packets_t& css_4d0d, Packets_t& states_4d0f, Packets_t& css_4d10);

protected:
	void executeOne( const Packets_t& states_4d0b, const Packets_t& css_4d0d);
	bool isInputUnique( const Udm::Object& state_4d15, const Udm::Object& cs_4d1c);

private:
	Packets_t* _state_4d11;
	Packets_t* _cs_4d12;
	Packets_t _state_4d13;
	Packets_t _cs_4d1a;
};

class AndState_4d21
{
public:
	bool operator()( const Packets_t& sFStates_4d22, const Packets_t& compoundStatements_4d25, Packets_t& sFStates_4d24, Packets_t& compoundStatements_4d27);

protected:
	bool isInputUnique( const Udm::Object& sFState_4d2e, const Udm::Object& compoundStatement_4d37);
	bool isGuardTrue( SFC::CompoundStatement& CompoundStatement, CyberComposition::Simulink::SFState& SFState);
	void processInputPackets( const Packets_t& sFStates_4d22, const Packets_t& compoundStatements_4d25);
	bool patternMatcher( const Udm::Object& sFState_4d2c, const Udm::Object& compoundStatement_4d35);
	void outputAppender( const CyberComposition::Simulink::SFState& sFState_4d41, const SFC::CompoundStatement& compoundStatement_4d43);

private:
	Packets_t* _sFState_4d28;
	Packets_t* _compoundStatement_4d29;
	Packets_t _sFState_4d2a;
	Packets_t _compoundStatement_4d33;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_4d3d;
		SFC::CompoundStatement compoundStatement_4d3e;
	};

	std::list< Match> _matches;
};

class CallSibsOrChildren_4d45
{
public:
	void operator()( const Packets_t& sFStates_4d46, const Packets_t& sFSOCStates_4d48, const Packets_t& compoundStatements_4d4a);

protected:
	bool isInputUnique( const Udm::Object& sFState_4d50, const Udm::Object& sFSOCState_4d59, const Udm::Object& compoundStatement_4d62);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::Arg& Arg1, SFC::CompoundStatement& CompoundStatement, SFC::Function& Enter, CyberComposition::Simulink::SFState& SFSOCState, CyberComposition::Simulink::SFState& SFState, SFC::Arg& SOCArg0, SFC::Arg& SOCArg1, SFC::Function& SOCEnter);
	void processInputPackets( const Packets_t& sFStates_4d46, const Packets_t& sFSOCStates_4d48, const Packets_t& compoundStatements_4d4a);
	bool patternMatcher( const Udm::Object& sFState_4d4e, const Udm::Object& sFSOCState_4d57, const Udm::Object& compoundStatement_4d60);
	void effector();

private:
	Packets_t _sFState_4d4c;
	Packets_t _sFSOCState_4d55;
	Packets_t _compoundStatement_4d5e;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_4d7a;
		CyberComposition::Simulink::SFState sFSOCState_4d7b;
		SFC::CompoundStatement compoundStatement_4d7c;
		SFC::Function enter_4d7d;
		SFC::Arg sOCArg0_4d7e;
		SFC::Arg sOCArg1_4d7f;
		SFC::Arg arg1_4d80;
		SFC::Function sOCEnter_4d81;
	};

	std::list< Match> _matches;
};

class CallParent_4d99
{
public:
	void operator()( const Packets_t& sFStates_4d9a, const Packets_t& conditionalBlocks_4d9c);

protected:
	bool isInputUnique( const Udm::Object& sFState_4da2, const Udm::Object& conditionalBlock_4dab);
	bool isGuardTrue( SFC::Arg& Arg1, SFC::ConditionalBlock& ConditionalBlock, SFC::Function& Enter, SFC::Arg& ParentArg0, SFC::Arg& ParentArg1, SFC::Function& ParentEnter, CyberComposition::Simulink::SFState& SFParentState, CyberComposition::Simulink::SFState& SFState);
	void processInputPackets( const Packets_t& sFStates_4d9a, const Packets_t& conditionalBlocks_4d9c);
	bool patternMatcher( const Udm::Object& sFState_4da0, const Udm::Object& conditionalBlock_4da9);
	void effector();

private:
	Packets_t _sFState_4d9e;
	Packets_t _conditionalBlock_4da7;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_4dc2;
		SFC::ConditionalBlock conditionalBlock_4dc3;
		CyberComposition::Simulink::SFState sFParentState_4dc4;
		SFC::Arg parentArg1_4dc5;
		SFC::Arg parentArg0_4dc6;
		SFC::Function parentEnter_4dc7;
		SFC::Function enter_4dc8;
		SFC::Arg arg1_4dc9;
	};

	std::list< Match> _matches;
};

class TestMode03_4dd7
{
public:
	void operator()( const Packets_t& sFStates_4dd8, Packets_t& sFStates_4dda, Packets_t& conditionalBlocks_4ddb);

protected:
	bool isInputUnique( const Udm::Object& sFState_4de2);
	bool isGuardTrue( SFC::Arg& Arg0, SFC::Function& Enter, CyberComposition::Simulink::SFState& SFState);
	void processInputPackets( const Packets_t& sFStates_4dd8);
	bool patternMatcher( const Udm::Object& sFState_4de0);
	void effector();
	void outputAppender( const CyberComposition::Simulink::SFState& sFState_4dfd, const SFC::ConditionalBlock& conditionalBlock_4dff);

private:
	Packets_t* _sFState_4ddc;
	Packets_t* _conditionalBlock_4ddd;
	Packets_t _sFState_4dde;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_4dee;
		SFC::Arg arg0_4def;
		SFC::Function enter_4df0;
	};

	std::list< Match> _matches;
};

class Mode0or2_4e09
{
public:
	void operator()( const Packets_t& states_4e0a, Packets_t& states_4e0c);

protected:
	void callChildTest_4f20( const Packets_t& states_4e0f);
	void callTestMode02_4f22( const Packets_t& sFStates_4e5e);
	void callChildStateType_4f24( const Packets_t& states_4eb5, const Packets_t& css_4eb7);
	void callTransStartMap_4f27( const Packets_t& states_3f9f, const Packets_t& css_3fa1);
	void callGetChildStates_4f2a( const Packets_t& sFStates_4e88, const Packets_t& compoundStatements_4e8c);
	void callRefetchEnterFcn_4f2d( const Packets_t& sFStates_4e34, const Packets_t& compoundStatements_4e38);
	void callCallSibsOrChildren_4f30( const Packets_t& sFStates_4d46, const Packets_t& sFSOCStates_4d48, const Packets_t& compoundStatements_4d4a);
	void callTransStartTransitions_4f34( const Packets_t& states_3e20, const Packets_t& fcns_3e22, const Packets_t& css_3e24);

private:
	Packets_t* _state_4e0d;
};

class ChildTest_4e0e
{
public:
	void operator()( const Packets_t& states_4e0f, Packets_t& states_4e11);

protected:
	void executeOne( const Packets_t& states_4e0f);
	bool isInputUnique( const Udm::Object& state_4e15);

private:
	Packets_t* _state_4e12;
	Packets_t _state_4e13;
};

class HasChildTransConnector_4e1a
{
public:
	bool operator()( const Packets_t& sFStates_4e1b, Packets_t& sFStates_4e1d);

protected:
	bool isInputUnique( const Udm::Object& sFState_4e23);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& sFStates_4e1b);
	bool patternMatcher( const Udm::Object& sFState_4e21);
	void outputAppender( const CyberComposition::Simulink::SFState& sFState_4e31);

private:
	Packets_t* _sFState_4e1e;
	Packets_t _sFState_4e1f;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_4e2f;
		CyberComposition::Simulink::SFTransConnector sFTransConnector_4e30;
	};

	std::list< Match> _matches;
};

class RefetchEnterFcn_4e33
{
public:
	void operator()( const Packets_t& sFStates_4e34, const Packets_t& compoundStatements_4e38, Packets_t& sFStates_4e36, Packets_t& functions_4e37, Packets_t& compoundStatements_4e3a);

protected:
	bool isInputUnique( const Udm::Object& sFState_4e42, const Udm::Object& compoundStatement_4e4b);
	void processInputPackets( const Packets_t& sFStates_4e34, const Packets_t& compoundStatements_4e38);
	bool patternMatcher( const Udm::Object& sFState_4e40, const Udm::Object& compoundStatement_4e49);
	void effector();
	void outputAppender( const CyberComposition::Simulink::SFState& sFState_4e57, const SFC::Function& function_4e59, const SFC::CompoundStatement& compoundStatement_4e5b);

private:
	Packets_t* _sFState_4e3b;
	Packets_t* _function_4e3c;
	Packets_t* _compoundStatement_4e3d;
	Packets_t _sFState_4e3e;
	Packets_t _compoundStatement_4e47;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_4e54;
		SFC::CompoundStatement compoundStatement_4e55;
		SFC::Function function_4e56;
	};

	std::list< Match> _matches;
};

class TestMode02_4e5d
{
public:
	void operator()( const Packets_t& sFStates_4e5e, Packets_t& sFStates_4e60, Packets_t& conditionalBlocks_4e61);

protected:
	bool isInputUnique( const Udm::Object& sFState_4e68);
	bool isGuardTrue( SFC::Arg& Arg0, SFC::Function& Enter, CyberComposition::Simulink::SFState& SFState);
	void processInputPackets( const Packets_t& sFStates_4e5e);
	bool patternMatcher( const Udm::Object& sFState_4e66);
	void effector();
	void outputAppender( const CyberComposition::Simulink::SFState& sFState_4e83, const SFC::ConditionalBlock& conditionalBlock_4e85);

private:
	Packets_t* _sFState_4e62;
	Packets_t* _conditionalBlock_4e63;
	Packets_t _sFState_4e64;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_4e74;
		SFC::Arg arg0_4e75;
		SFC::Function enter_4e76;
	};

	std::list< Match> _matches;
};

class GetChildStates_4e87
{
public:
	void operator()( const Packets_t& sFStates_4e88, const Packets_t& compoundStatements_4e8c, Packets_t& sFStates_4e8a, Packets_t& sFChildStates_4e8b, Packets_t& compoundStatements_4e8e);

protected:
	bool isInputUnique( const Udm::Object& sFState_4e96, const Udm::Object& compoundStatement_4e9f);
	bool isGuardTrue( SFC::CompoundStatement& CompoundStatement, CyberComposition::Simulink::SFState& SFChildState, CyberComposition::Simulink::SFState& SFState);
	void processInputPackets( const Packets_t& sFStates_4e88, const Packets_t& compoundStatements_4e8c);
	bool patternMatcher( const Udm::Object& sFState_4e94, const Udm::Object& compoundStatement_4e9d);
	void effector();
	void outputAppender( const CyberComposition::Simulink::SFState& sFState_4eae, const CyberComposition::Simulink::SFState& sFChildState_4eb0, const SFC::CompoundStatement& compoundStatement_4eb2);
	void sortOutputs();

private:
	Packets_t* _sFState_4e8f;
	Packets_t* _sFChildState_4e90;
	Packets_t* _compoundStatement_4e91;
	Packets_t _sFState_4e92;
	Packets_t _compoundStatement_4e9b;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_4ea8;
		SFC::CompoundStatement compoundStatement_4ea9;
		CyberComposition::Simulink::SFState sFChildState_4eaa;
	};

	std::list< Match> _matches;
};

class ChildStateType_4eb4
{
public:
	void operator()( const Packets_t& states_4eb5, const Packets_t& css_4eb7, Packets_t& states_4eb9, Packets_t& css_4eba, Packets_t& states_4ebb, Packets_t& css_4ebc);

protected:
	void executeOne( const Packets_t& states_4eb5, const Packets_t& css_4eb7);
	bool isInputUnique( const Udm::Object& state_4ec3, const Udm::Object& cs_4eca);

private:
	Packets_t* _state_4ebd;
	Packets_t* _cs_4ebe;
	Packets_t* _state_4ebf;
	Packets_t* _cs_4ec0;
	Packets_t _state_4ec1;
	Packets_t _cs_4ec8;
};

class ChildANDStates_4ecf
{
public:
	bool operator()( const Packets_t& sFStates_4ed0, const Packets_t& compoundStatements_4ed3, Packets_t& sFStates_4ed2, Packets_t& compoundStatements_4ed5);

protected:
	bool isInputUnique( const Udm::Object& sFState_4edc, const Udm::Object& compoundStatement_4ee5);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::CompoundStatement& CompoundStatement, CyberComposition::Simulink::SFState& SFChildState, CyberComposition::Simulink::SFState& SFState);
	void processInputPackets( const Packets_t& sFStates_4ed0, const Packets_t& compoundStatements_4ed3);
	bool patternMatcher( const Udm::Object& sFState_4eda, const Udm::Object& compoundStatement_4ee3);
	void outputAppender( const CyberComposition::Simulink::SFState& sFState_4ef7, const SFC::CompoundStatement& compoundStatement_4ef9);

private:
	Packets_t* _sFState_4ed6;
	Packets_t* _compoundStatement_4ed7;
	Packets_t _sFState_4ed8;
	Packets_t _compoundStatement_4ee1;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_4ef1;
		SFC::CompoundStatement compoundStatement_4ef2;
		CyberComposition::Simulink::SFState sFChildState_4ef3;
	};

	std::list< Match> _matches;
};

class Otherwise_4efb
{
public:
	bool operator()( const Packets_t& sFStates_4efc, const Packets_t& compoundStatements_4eff, Packets_t& sFStates_4efe, Packets_t& compoundStatements_4f01);

protected:
	bool isInputUnique( const Udm::Object& sFState_4f08, const Udm::Object& compoundStatement_4f11);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& sFStates_4efc, const Packets_t& compoundStatements_4eff);
	bool patternMatcher( const Udm::Object& sFState_4f06, const Udm::Object& compoundStatement_4f0f);
	void outputAppender( const CyberComposition::Simulink::SFState& sFState_4f1c, const SFC::CompoundStatement& compoundStatement_4f1e);

private:
	Packets_t* _sFState_4f02;
	Packets_t* _compoundStatement_4f03;
	Packets_t _sFState_4f04;
	Packets_t _compoundStatement_4f0d;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_4f1a;
		SFC::CompoundStatement compoundStatement_4f1b;
	};

	std::list< Match> _matches;
};

class Mode0or1or2_4f38
{
public:
	void operator()( const Packets_t& states_4f39, Packets_t& states_4f3b);

protected:
	void callTestMode012EnterAction_4f77( const Packets_t& sFStates_4f3e);

private:
	Packets_t* _state_4f3c;
};

class TestMode012EnterAction_4f3d
{
public:
	void operator()( const Packets_t& sFStates_4f3e);

protected:
	bool isInputUnique( const Udm::Object& sFState_4f44);
	bool isGuardTrue( SFC::Arg& Arg0, SFC::Function& Enter, SFC::StateLabel& ParentStateLabel, SFC::Program& Program, CyberComposition::Simulink::SFState& SFParentState, CyberComposition::Simulink::SFState& SFState, SFC::StateLabel& StateLabel, SFC::StateVar& StateVar);
	void processInputPackets( const Packets_t& sFStates_4f3e);
	bool patternMatcher( const Udm::Object& sFState_4f42);
	void effector();

private:
	Packets_t _sFState_4f40;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_4f5d;
		CyberComposition::Simulink::SFState sFParentState_4f5e;
		SFC::StateLabel parentStateLabel_4f5f;
		SFC::StateLabel stateLabel_4f60;
		SFC::Arg arg0_4f61;
		SFC::Function enter_4f62;
		SFC::Program program_4f63;
		SFC::StateVar stateVar_4f64;
	};

	std::list< Match> _matches;
};

class Mode0or1_4f79
{
public:
	void operator()( const Packets_t& states_4f7a, Packets_t& states_4f7c);

protected:
	void callTestMode01CallParent_4ff6( const Packets_t& sFStates_4fb0);
	void callIsAndState_4ff8( const Packets_t& states_4d0b, const Packets_t& css_4d0d);
	void callGetHOSibs_4ffb( const Packets_t& sFStates_4f7f, const Packets_t& compoundStatements_4f83);
	void callCallSibsOrChildren_4ffe( const Packets_t& sFStates_4d46, const Packets_t& sFSOCStates_4d48, const Packets_t& compoundStatements_4d4a);

private:
	Packets_t* _state_4f7d;
};

class GetHOSibs_4f7e
{
public:
	void operator()( const Packets_t& sFStates_4f7f, const Packets_t& compoundStatements_4f83, Packets_t& sFStates_4f81, Packets_t& sFSibStates_4f82, Packets_t& compoundStatements_4f85);

protected:
	bool isInputUnique( const Udm::Object& sFState_4f8d, const Udm::Object& compoundStatement_4f96);
	bool isGuardTrue( SFC::CompoundStatement& CompoundStatement, CyberComposition::Simulink::SFState& SFParentState, CyberComposition::Simulink::SFState& SFSibState, CyberComposition::Simulink::SFState& SFState);
	void processInputPackets( const Packets_t& sFStates_4f7f, const Packets_t& compoundStatements_4f83);
	bool patternMatcher( const Udm::Object& sFState_4f8b, const Udm::Object& compoundStatement_4f94);
	void effector();
	void outputAppender( const CyberComposition::Simulink::SFState& sFState_4fa9, const CyberComposition::Simulink::SFState& sFSibState_4fab, const SFC::CompoundStatement& compoundStatement_4fad);
	void sortOutputs();

private:
	Packets_t* _sFState_4f86;
	Packets_t* _sFSibState_4f87;
	Packets_t* _compoundStatement_4f88;
	Packets_t _sFState_4f89;
	Packets_t _compoundStatement_4f92;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_4fa1;
		SFC::CompoundStatement compoundStatement_4fa2;
		CyberComposition::Simulink::SFState sFParentState_4fa3;
		CyberComposition::Simulink::SFState sFSibState_4fa4;
	};

	std::list< Match> _matches;
};

class TestMode01CallParent_4faf
{
public:
	void operator()( const Packets_t& sFStates_4fb0, Packets_t& sFStates_4fb2, Packets_t& conditionalBlocks_4fb3);

protected:
	bool isInputUnique( const Udm::Object& sFState_4fba);
	bool isGuardTrue( SFC::Arg& Arg0, SFC::Arg& Arg1, SFC::Function& Enter, SFC::Arg& ParentArg0, SFC::Arg& ParentArg1, SFC::Function& ParentEnter, CyberComposition::Simulink::SFState& SFParentState, CyberComposition::Simulink::SFState& SFState);
	void processInputPackets( const Packets_t& sFStates_4fb0);
	bool patternMatcher( const Udm::Object& sFState_4fb8);
	void effector();
	void outputAppender( const CyberComposition::Simulink::SFState& sFState_4ff2, const SFC::ConditionalBlock& conditionalBlock_4ff4);

private:
	Packets_t* _sFState_4fb4;
	Packets_t* _conditionalBlock_4fb5;
	Packets_t _sFState_4fb6;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_4fd4;
		CyberComposition::Simulink::SFState sFParentState_4fd5;
		SFC::Arg parentArg0_4fd6;
		SFC::Function parentEnter_4fd7;
		SFC::Arg arg0_4fd8;
		SFC::Arg arg1_4fd9;
		SFC::Arg parentArg1_4fda;
		SFC::Function enter_4fdb;
	};

	std::list< Match> _matches;
};

class ReturnAtTopState_5002
{
public:
	void operator()( const Packets_t& states_5003, Packets_t& states_5005);

protected:
	void callTopState_502e( const Packets_t& sFStates_5008);

private:
	Packets_t* _state_5006;
};

class TopState_5007
{
public:
	void operator()( const Packets_t& sFStates_5008);

protected:
	bool isInputUnique( const Udm::Object& sFState_500e);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::Arg& Arg, SFC::Function& Enter, CyberComposition::Simulink::SFState& SFState, SFC::StateLabel& StateLabel);
	void processInputPackets( const Packets_t& sFStates_5008);
	bool patternMatcher( const Udm::Object& sFState_500c);
	void effector();

private:
	Packets_t _sFState_500a;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_5020;
		SFC::StateLabel stateLabel_5021;
		SFC::Function enter_5022;
		SFC::Arg arg_5023;
	};

	std::list< Match> _matches;
};

class PopulateExecFunction_503c
{
public:
	void operator()( const Packets_t& states_503d, const Packets_t& programs_503f, Packets_t& states_5041, Packets_t& programs_5042);

protected:
	void callInitTransConnMap_54d7( const Packets_t& states_5458);
	void callOuterTransitions_54d9( const Packets_t& states_5462);
	void callDuringAction_54db( const Packets_t& sFStates_5441);
	void callInitTransConnMapInner_54dd( const Packets_t& states_53f3);
	void callInnerTransitions_54df( const Packets_t& states_52a9);
	void callExecChildren_54e1( const Packets_t& states_5046);

private:
	Packets_t* _state_5043;
	Packets_t* _program_5044;
};

class ExecChildren_5045
{
public:
	void operator()( const Packets_t& states_5046);

protected:
	void callGetExecFcn_5284( const Packets_t& sFStates_526b);
	void callChildStateType_5286( const Packets_t& states_4eb5, const Packets_t& css_4eb7);
	void callTopLevelState_5289( const Packets_t& states_5162, const Packets_t& css_5164);
	void callCG_528c( const Packets_t& sFStates_5245, const Packets_t& compoundStatements_5248);
	void callChildStatesExec_528f( const Packets_t& states_5076, const Packets_t& css_5078);
	void callCreateCG_5292( const Packets_t& sFStates_513c, const Packets_t& execs_513f);
	void callCallActiveChildExec_5295( const Packets_t& states_51f8, const Packets_t& css_51fa);
	void callEnterFunctionCondition_5298( const Packets_t& states_50b3, const Packets_t& cgs_50b5);
	void callTransStartMap_529b( const Packets_t& states_3f9f, const Packets_t& css_3fa1);
	void callExecFunctionCondition_529e( const Packets_t& states_5049, const Packets_t& cgs_504b);
	void callDefaultCondition_52a1( const Packets_t& sFStates_51cb, const Packets_t& compoundStatements_51cf);
	void callTransStartTransitions_52a4( const Packets_t& states_3e20, const Packets_t& fcns_3e22, const Packets_t& css_3e24);
};

class ExecFunctionCondition_5048
{
public:
	void operator()( const Packets_t& states_5049, const Packets_t& cgs_504b);

protected:
	void callCreateDefaultCB_50ac( const Packets_t& sFStates_504e, const Packets_t& conditionalGroups_5051);
	void callChildStatesExec_50af( const Packets_t& states_5076, const Packets_t& css_5078);
};

class CreateDefaultCB_504d
{
public:
	void operator()( const Packets_t& sFStates_504e, const Packets_t& conditionalGroups_5051, Packets_t& sFStates_5050, Packets_t& conditionalBlocks_5053);

protected:
	bool isInputUnique( const Udm::Object& sFState_505a, const Udm::Object& conditionalGroup_5063);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& sFStates_504e, const Packets_t& conditionalGroups_5051);
	bool patternMatcher( const Udm::Object& sFState_5058, const Udm::Object& conditionalGroup_5061);
	void effector();
	void outputAppender( const CyberComposition::Simulink::SFState& sFState_5071, const SFC::ConditionalBlock& conditionalBlock_5073);

private:
	Packets_t* _sFState_5054;
	Packets_t* _conditionalBlock_5055;
	Packets_t _sFState_5056;
	Packets_t _conditionalGroup_505f;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_506c;
		SFC::ConditionalGroup conditionalGroup_506d;
	};

	std::list< Match> _matches;
};

class ChildStatesExec_5075
{
public:
	void operator()( const Packets_t& states_5076, const Packets_t& css_5078);

protected:
	void callGetChildStates_50a5( const Packets_t& sFStates_4e88, const Packets_t& compoundStatements_4e8c);
	void callExecChildAndStates_50a8( const Packets_t& sFStates_507b, const Packets_t& sFChildStates_507d, const Packets_t& compoundStatements_507f);
};

class ExecChildAndStates_507a
{
public:
	void operator()( const Packets_t& sFStates_507b, const Packets_t& sFChildStates_507d, const Packets_t& compoundStatements_507f);

protected:
	bool isInputUnique( const Udm::Object& sFState_5085, const Udm::Object& sFChildState_508e, const Udm::Object& compoundStatement_5097);
	void processInputPackets( const Packets_t& sFStates_507b, const Packets_t& sFChildStates_507d, const Packets_t& compoundStatements_507f);
	bool patternMatcher( const Udm::Object& sFState_5083, const Udm::Object& sFChildState_508c, const Udm::Object& compoundStatement_5095);
	void effector();

private:
	Packets_t _sFState_5081;
	Packets_t _sFChildState_508a;
	Packets_t _compoundStatement_5093;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_50a0;
		CyberComposition::Simulink::SFState sFChildState_50a1;
		SFC::CompoundStatement compoundStatement_50a2;
		SFC::Function function_50a3;
	};

	std::list< Match> _matches;
};

class EnterFunctionCondition_50b2
{
public:
	void operator()( const Packets_t& states_50b3, const Packets_t& cgs_50b5, Packets_t& states_50b7, Packets_t& cgs_50b8);

protected:
	void callCreateCB_5131( const Packets_t& sFStates_50fc, const Packets_t& conditionalGroups_50ff);
	void callGetChildStates_5134( const Packets_t& sFStates_4e88, const Packets_t& compoundStatements_4e8c);
	void callCallChildren_5137( const Packets_t& sFStates_50bc, const Packets_t& sFChildStates_50be, const Packets_t& compoundStatements_50c0);

private:
	Packets_t* _state_50b9;
	Packets_t* _cg_50ba;
};

class CallChildren_50bb
{
public:
	void operator()( const Packets_t& sFStates_50bc, const Packets_t& sFChildStates_50be, const Packets_t& compoundStatements_50c0);

protected:
	bool isInputUnique( const Udm::Object& sFState_50c6, const Udm::Object& sFChildState_50cf, const Udm::Object& compoundStatement_50d8);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::Arg& ChildArg0, SFC::Arg& ChildArg1, SFC::Function& ChildEnter, SFC::CompoundStatement& CompoundStatement, CyberComposition::Simulink::SFState& SFChildState, CyberComposition::Simulink::SFState& SFState);
	void processInputPackets( const Packets_t& sFStates_50bc, const Packets_t& sFChildStates_50be, const Packets_t& compoundStatements_50c0);
	bool patternMatcher( const Udm::Object& sFState_50c4, const Udm::Object& sFChildState_50cd, const Udm::Object& compoundStatement_50d6);
	void effector();

private:
	Packets_t _sFState_50c2;
	Packets_t _sFChildState_50cb;
	Packets_t _compoundStatement_50d4;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_50ea;
		CyberComposition::Simulink::SFState sFChildState_50eb;
		SFC::CompoundStatement compoundStatement_50ec;
		SFC::Arg childArg0_50ed;
		SFC::Arg childArg1_50ee;
		SFC::Function childEnter_50ef;
	};

	std::list< Match> _matches;
};

class CreateCB_50fb
{
public:
	void operator()( const Packets_t& sFStates_50fc, const Packets_t& conditionalGroups_50ff, Packets_t& sFStates_50fe, Packets_t& conditionalBlocks_5101);

protected:
	bool isInputUnique( const Udm::Object& sFState_5108, const Udm::Object& conditionalGroup_5111);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& sFStates_50fc, const Packets_t& conditionalGroups_50ff);
	bool patternMatcher( const Udm::Object& sFState_5106, const Udm::Object& conditionalGroup_510f);
	void effector();
	void outputAppender( const CyberComposition::Simulink::SFState& sFState_512d, const SFC::ConditionalBlock& conditionalBlock_512f);

private:
	Packets_t* _sFState_5102;
	Packets_t* _conditionalBlock_5103;
	Packets_t _sFState_5104;
	Packets_t _conditionalGroup_510d;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_5121;
		SFC::ConditionalGroup conditionalGroup_5122;
		SFC::Function function_5123;
		SFC::Program program_5124;
		SFC::StateVar stateVar_5125;
	};

	std::list< Match> _matches;
};

class CreateCG_513b
{
public:
	void operator()( const Packets_t& sFStates_513c, const Packets_t& execs_513f, Packets_t& sFStates_513e, Packets_t& conditionalGroups_5141);

protected:
	bool isInputUnique( const Udm::Object& sFState_5148, const Udm::Object& exec_5151);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& sFStates_513c, const Packets_t& execs_513f);
	bool patternMatcher( const Udm::Object& sFState_5146, const Udm::Object& exec_514f);
	void effector();
	void outputAppender( const CyberComposition::Simulink::SFState& sFState_515d, const SFC::ConditionalGroup& conditionalGroup_515f);

private:
	Packets_t* _sFState_5142;
	Packets_t* _conditionalGroup_5143;
	Packets_t _sFState_5144;
	Packets_t _exec_514d;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_515a;
		SFC::Function exec_515b;
	};

	std::list< Match> _matches;
};

class TopLevelState_5161
{
public:
	void operator()( const Packets_t& states_5162, const Packets_t& css_5164, Packets_t& states_5166, Packets_t& css_5167, Packets_t& states_5168, Packets_t& css_5169);

protected:
	void executeOne( const Packets_t& states_5162, const Packets_t& css_5164);
	bool isInputUnique( const Udm::Object& state_5170, const Udm::Object& cs_5177);

private:
	Packets_t* _state_516a;
	Packets_t* _cs_516b;
	Packets_t* _state_516c;
	Packets_t* _cs_516d;
	Packets_t _state_516e;
	Packets_t _cs_5175;
};

class IsTopLevelState_517c
{
public:
	bool operator()( const Packets_t& sFStates_517d, const Packets_t& compoundStatements_5180, Packets_t& sFStates_517f, Packets_t& compoundStatements_5182);

protected:
	bool isInputUnique( const Udm::Object& sFState_5189, const Udm::Object& compoundStatement_5192);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& sFStates_517d, const Packets_t& compoundStatements_5180);
	bool patternMatcher( const Udm::Object& sFState_5187, const Udm::Object& compoundStatement_5190);
	void outputAppender( const CyberComposition::Simulink::SFState& sFState_51a1, const SFC::CompoundStatement& compoundStatement_51a3);

private:
	Packets_t* _sFState_5183;
	Packets_t* _compoundStatement_5184;
	Packets_t _sFState_5185;
	Packets_t _compoundStatement_518e;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_519e;
		SFC::CompoundStatement compoundStatement_519f;
		CyberComposition::Simulink::SFConnectorRef sFConnectorRef_51a0;
	};

	std::list< Match> _matches;
};

class Otherwise_51a5
{
public:
	bool operator()( const Packets_t& sFStates_51a6, const Packets_t& compoundStatements_51a9, Packets_t& sFStates_51a8, Packets_t& compoundStatements_51ab);

protected:
	bool isInputUnique( const Udm::Object& sFState_51b2, const Udm::Object& compoundStatement_51bb);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& sFStates_51a6, const Packets_t& compoundStatements_51a9);
	bool patternMatcher( const Udm::Object& sFState_51b0, const Udm::Object& compoundStatement_51b9);
	void outputAppender( const CyberComposition::Simulink::SFState& sFState_51c6, const SFC::CompoundStatement& compoundStatement_51c8);

private:
	Packets_t* _sFState_51ac;
	Packets_t* _compoundStatement_51ad;
	Packets_t _sFState_51ae;
	Packets_t _compoundStatement_51b7;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_51c4;
		SFC::CompoundStatement compoundStatement_51c5;
	};

	std::list< Match> _matches;
};

class DefaultCondition_51ca
{
public:
	void operator()( const Packets_t& sFStates_51cb, const Packets_t& compoundStatements_51cf, Packets_t& sFStates_51cd, Packets_t& functions_51ce, Packets_t& conditionalBlocks_51d1);

protected:
	bool isInputUnique( const Udm::Object& sFState_51d9, const Udm::Object& compoundStatement_51e2);
	void processInputPackets( const Packets_t& sFStates_51cb, const Packets_t& compoundStatements_51cf);
	bool patternMatcher( const Udm::Object& sFState_51d7, const Udm::Object& compoundStatement_51e0);
	void effector();
	void outputAppender( const CyberComposition::Simulink::SFState& sFState_51f1, const SFC::Function& function_51f3, const SFC::ConditionalBlock& conditionalBlock_51f5);

private:
	Packets_t* _sFState_51d2;
	Packets_t* _function_51d3;
	Packets_t* _conditionalBlock_51d4;
	Packets_t _sFState_51d5;
	Packets_t _compoundStatement_51de;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_51eb;
		SFC::CompoundStatement compoundStatement_51ec;
		SFC::Function function_51ed;
	};

	std::list< Match> _matches;
};

class CallActiveChildExec_51f7
{
public:
	void operator()( const Packets_t& states_51f8, const Packets_t& css_51fa, Packets_t& states_51fc, Packets_t& css_51fd);

protected:
	void callGetChildStates_523d( const Packets_t& sFStates_4e88, const Packets_t& compoundStatements_4e8c);
	void callExecActiveChildState_5240( const Packets_t& sFStates_5201, const Packets_t& sFChildStates_5203, const Packets_t& compoundStatements_5205);

private:
	Packets_t* _state_51fe;
	Packets_t* _cs_51ff;
};

class ExecActiveChildState_5200
{
public:
	void operator()( const Packets_t& sFStates_5201, const Packets_t& sFChildStates_5203, const Packets_t& compoundStatements_5205);

protected:
	bool isInputUnique( const Udm::Object& sFState_520b, const Udm::Object& sFChildState_5214, const Udm::Object& compoundStatement_521d);
	void processInputPackets( const Packets_t& sFStates_5201, const Packets_t& sFChildStates_5203, const Packets_t& compoundStatements_5205);
	bool patternMatcher( const Udm::Object& sFState_5209, const Udm::Object& sFChildState_5212, const Udm::Object& compoundStatement_521b);
	void effector();

private:
	Packets_t _sFState_5207;
	Packets_t _sFChildState_5210;
	Packets_t _compoundStatement_5219;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_5231;
		CyberComposition::Simulink::SFState sFChildState_5232;
		SFC::CompoundStatement compoundStatement_5233;
		SFC::StateLabel stateLabel_5234;
		SFC::StateLabel childStateLabel_5235;
		SFC::Function function_5236;
		SFC::Program program_5237;
		SFC::StateVar stateVar_5238;
	};

	std::list< Match> _matches;
};

class CG_5244
{
public:
	void operator()( const Packets_t& sFStates_5245, const Packets_t& compoundStatements_5248, Packets_t& sFStates_5247, Packets_t& conditionalGroups_524a);

protected:
	bool isInputUnique( const Udm::Object& sFState_5251, const Udm::Object& compoundStatement_525a);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& sFStates_5245, const Packets_t& compoundStatements_5248);
	bool patternMatcher( const Udm::Object& sFState_524f, const Udm::Object& compoundStatement_5258);
	void effector();
	void outputAppender( const CyberComposition::Simulink::SFState& sFState_5266, const SFC::ConditionalGroup& conditionalGroup_5268);

private:
	Packets_t* _sFState_524b;
	Packets_t* _conditionalGroup_524c;
	Packets_t _sFState_524d;
	Packets_t _compoundStatement_5256;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_5263;
		SFC::CompoundStatement compoundStatement_5264;
	};

	std::list< Match> _matches;
};

class GetExecFcn_526a
{
public:
	void operator()( const Packets_t& sFStates_526b, Packets_t& sFStates_526d, Packets_t& functions_526e);

protected:
	bool isInputUnique( const Udm::Object& sFState_5275);
	void processInputPackets( const Packets_t& sFStates_526b);
	bool patternMatcher( const Udm::Object& sFState_5273);
	void effector();
	void outputAppender( const CyberComposition::Simulink::SFState& sFState_5280, const SFC::Function& function_5282);

private:
	Packets_t* _sFState_526f;
	Packets_t* _function_5270;
	Packets_t _sFState_5271;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_527e;
		SFC::Function function_527f;
	};

	std::list< Match> _matches;
};

class InnerTransitions_52a8
{
public:
	void operator()( const Packets_t& states_52a9, Packets_t& states_52ab);

protected:
	void callCheckForInnerTransitions_53df( const Packets_t& states_5317);
	void callInit_53e1( const Packets_t& states_5344);
	void callCreateTransitionCG_53e3( const Packets_t& sFStates_36fd, const Packets_t& sFTransConnectors_3700, const Packets_t& functions_3703, const Packets_t& tCVarCGs_3707);
	void callExecInnerTransitions_53e8( const Packets_t& states_52ae, const Packets_t& tcs_52b0, const Packets_t& fcns_52b2, const Packets_t& css_52b4);
	void callNoDefaultTransition_53ed( const Packets_t& states_2e48, const Packets_t& tcs_2e4a, const Packets_t& fcns_2e4c, const Packets_t& css_2e4e);

private:
	Packets_t* _state_52ac;
};

class ExecInnerTransitions_52ad
{
public:
	void operator()( const Packets_t& states_52ae, const Packets_t& tcs_52b0, const Packets_t& fcns_52b2, const Packets_t& css_52b4, Packets_t& states_52b6, Packets_t& tcs_52b7, Packets_t& fcns_52b8, Packets_t& css_52b9);

protected:
	void callGetInnerTransitions_530c( const Packets_t& sFStates_52bf, const Packets_t& sFParentStates_52c2, const Packets_t& functions_52c5, const Packets_t& compoundStatements_52c8);
	void callExecProcessTransitions_5311( const Packets_t& states_2fb3, const Packets_t& transs_2fb5, const Packets_t& fcns_2fb7, const Packets_t& css_2fb9);

private:
	Packets_t* _state_52ba;
	Packets_t* _tc_52bb;
	Packets_t* _fcn_52bc;
	Packets_t* _cs_52bd;
};

class GetInnerTransitions_52be
{
public:
	void operator()( const Packets_t& sFStates_52bf, const Packets_t& sFParentStates_52c2, const Packets_t& functions_52c5, const Packets_t& compoundStatements_52c8, Packets_t& sFStates_52c1, Packets_t& sFTransitions_52c4, Packets_t& functions_52c7, Packets_t& compoundStatements_52ca);

protected:
	bool isInputUnique( const Udm::Object& sFState_52d3, const Udm::Object& sFParentState_52dc, const Udm::Object& function_52e5, const Udm::Object& compoundStatement_52ee);
	void processInputPackets( const Packets_t& sFStates_52bf, const Packets_t& sFParentStates_52c2, const Packets_t& functions_52c5, const Packets_t& compoundStatements_52c8);
	bool patternMatcher( const Udm::Object& sFState_52d1, const Udm::Object& sFParentState_52da, const Udm::Object& function_52e3, const Udm::Object& compoundStatement_52ec);
	void effector();
	void outputAppender( const CyberComposition::Simulink::SFState& sFState_5304, const CyberComposition::Simulink::SFTransition& sFTransition_5306, const SFC::Function& function_5308, const SFC::CompoundStatement& compoundStatement_530a);

private:
	Packets_t* _sFState_52cb;
	Packets_t* _sFTransition_52cc;
	Packets_t* _function_52cd;
	Packets_t* _compoundStatement_52ce;
	Packets_t _sFState_52cf;
	Packets_t _sFParentState_52d8;
	Packets_t _function_52e1;
	Packets_t _compoundStatement_52ea;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_52fd;
		CyberComposition::Simulink::SFState sFParentState_52fe;
		SFC::Function function_52ff;
		SFC::CompoundStatement compoundStatement_5300;
		CyberComposition::Simulink::SFConnectorRef sFConnectorRef_5301;
		CyberComposition::Simulink::SFTransConnector sFTransConnector_5302;
		CyberComposition::Simulink::SFTransition sFTransition_5303;
	};

	std::list< Match> _matches;
};

class CheckForInnerTransitions_5316
{
public:
	void operator()( const Packets_t& states_5317, Packets_t& states_5319);

protected:
	void executeOne( const Packets_t& states_5317);
	bool isInputUnique( const Udm::Object& state_531d);

private:
	Packets_t* _state_531a;
	Packets_t _state_531b;
};

class HasInnerTransition_5322
{
public:
	bool operator()( const Packets_t& sFStates_5323, Packets_t& sFStates_5325);

protected:
	bool isInputUnique( const Udm::Object& sFState_532b);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& sFStates_5323);
	bool patternMatcher( const Udm::Object& sFState_5329);
	void outputAppender( const CyberComposition::Simulink::SFState& sFState_5341);

private:
	Packets_t* _sFState_5326;
	Packets_t _sFState_5327;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_533d;
		CyberComposition::Simulink::SFTransition sFTransition_533e;
		CyberComposition::Simulink::SFConnectorRef sFConnectorRef_533f;
		CyberComposition::Simulink::SFTransConnector sFTransConnector_5340;
	};

	std::list< Match> _matches;
};

class Init_5343
{
public:
	void operator()( const Packets_t& states_5344, Packets_t& states_5346, Packets_t& tcs_5347, Packets_t& fcns_5348, Packets_t& cgs_5349);

protected:
	void callGetExecFunction_53d1( const Packets_t& sFStates_53b8);
	void callInitHTPVar_53d3( const Packets_t& sFStates_537b, const Packets_t& functions_537e, const Packets_t& compoundStatements_5381);
	void callInitTCVarLoop_53d7( const Packets_t& sFStates_3f4e, const Packets_t& functions_3f51, const Packets_t& compoundStatements_3f54);
	void callGetStartTC_53db( const Packets_t& sFStates_534f, const Packets_t& functions_5352, const Packets_t& conditionalGroups_5355);

private:
	Packets_t* _state_534a;
	Packets_t* _tc_534b;
	Packets_t* _fcn_534c;
	Packets_t* _cg_534d;
};

class GetStartTC_534e
{
public:
	void operator()( const Packets_t& sFStates_534f, const Packets_t& functions_5352, const Packets_t& conditionalGroups_5355, Packets_t& sFStates_5351, Packets_t& functions_5354, Packets_t& conditionalGroups_5357);

protected:
	bool isInputUnique( const Udm::Object& sFState_535f, const Udm::Object& function_5368, const Udm::Object& conditionalGroup_5371);
	void processInputPackets( const Packets_t& sFStates_534f, const Packets_t& functions_5352, const Packets_t& conditionalGroups_5355);
	bool patternMatcher( const Udm::Object& sFState_535d, const Udm::Object& function_5366, const Udm::Object& conditionalGroup_536f);
	void effector();
	void forwardInputs();

private:
	Packets_t* _sFState_5358;
	Packets_t* _function_5359;
	Packets_t* _conditionalGroup_535a;
	Packets_t _sFState_535b;
	Packets_t _function_5364;
	Packets_t _conditionalGroup_536d;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_5377;
		SFC::Function function_5378;
		SFC::ConditionalGroup conditionalGroup_5379;
	};

	std::list< Match> _matches;
};

class InitHTPVar_537a
{
public:
	void operator()( const Packets_t& sFStates_537b, const Packets_t& functions_537e, const Packets_t& compoundStatements_5381, Packets_t& sFStates_537d, Packets_t& functions_5380, Packets_t& compoundStatements_5383);

protected:
	bool isInputUnique( const Udm::Object& sFState_538b, const Udm::Object& function_5394, const Udm::Object& compoundStatement_539d);
	bool isGuardTrue( SFC::CompoundStatement& CompoundStatement, SFC::Function& Function, SFC::LocalVar& HTPVar, CyberComposition::Simulink::SFState& SFState, SFC::StateLabel& StateLabel);
	void processInputPackets( const Packets_t& sFStates_537b, const Packets_t& functions_537e, const Packets_t& compoundStatements_5381);
	bool patternMatcher( const Udm::Object& sFState_5389, const Udm::Object& function_5392, const Udm::Object& compoundStatement_539b);
	void effector();
	void forwardInputs();

private:
	Packets_t* _sFState_5384;
	Packets_t* _function_5385;
	Packets_t* _compoundStatement_5386;
	Packets_t _sFState_5387;
	Packets_t _function_5390;
	Packets_t _compoundStatement_5399;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_53a9;
		SFC::Function function_53aa;
		SFC::CompoundStatement compoundStatement_53ab;
		SFC::StateLabel stateLabel_53ac;
		SFC::LocalVar hTPVar_53ad;
	};

	std::list< Match> _matches;
};

class GetExecFunction_53b7
{
public:
	void operator()( const Packets_t& sFStates_53b8, Packets_t& sFStates_53ba, Packets_t& execs_53bb);

protected:
	bool isInputUnique( const Udm::Object& sFState_53c2);
	void processInputPackets( const Packets_t& sFStates_53b8);
	bool patternMatcher( const Udm::Object& sFState_53c0);
	void effector();
	void outputAppender( const CyberComposition::Simulink::SFState& sFState_53cd, const SFC::Function& exec_53cf);

private:
	Packets_t* _sFState_53bc;
	Packets_t* _exec_53bd;
	Packets_t _sFState_53be;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_53cb;
		SFC::Function exec_53cc;
	};

	std::list< Match> _matches;
};

class InitTransConnMapInner_53f2
{
public:
	void operator()( const Packets_t& states_53f3, Packets_t& states_53f5);

protected:
	void callInitTransConnectorMap_5435( const Packets_t& sFStates_5426);
	void callAddTransConnector_5437( const Packets_t& sFStates_418a, const Packets_t& sFTransConnectors_418d);
	void callGetInnerTransitions_543a( const Packets_t& sFStates_53f8, const Packets_t& sFStates_53fc);
	void callProcessTransitionsMap_543d( const Packets_t& states_4070, const Packets_t& transs_4072);

private:
	Packets_t* _state_53f6;
};

class GetInnerTransitions_53f7
{
public:
	void operator()( const Packets_t& sFStates_53f8, const Packets_t& sFStates_53fc, Packets_t& sFStates_53fa, Packets_t& sFTransitions_53fb);

protected:
	bool isInputUnique( const Udm::Object& sFState_5404, const Udm::Object& sFState_540d);
	void processInputPackets( const Packets_t& sFStates_53f8, const Packets_t& sFStates_53fc);
	bool patternMatcher( const Udm::Object& sFState_5402, const Udm::Object& sFState_540b);
	void effector();
	void outputAppender( const CyberComposition::Simulink::SFState& sFState_5421, const CyberComposition::Simulink::SFTransition& sFTransition_5423);

private:
	Packets_t* _sFState_53fe;
	Packets_t* _sFTransition_53ff;
	Packets_t _sFState_5400;
	Packets_t _sFState_5409;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_541c;
		CyberComposition::Simulink::SFState sFState_541d;
		CyberComposition::Simulink::SFTransConnector sFDstTransConnector_541e;
		CyberComposition::Simulink::SFTransition sFTransition_541f;
		CyberComposition::Simulink::SFConnectorRef sFConnectorRef_5420;
	};

	std::list< Match> _matches;
};

class InitTransConnectorMap_5425
{
public:
	void operator()( const Packets_t& sFStates_5426, Packets_t& sFStates_5428);

protected:
	bool isInputUnique( const Udm::Object& sFState_542e);
	void processInputPackets( const Packets_t& sFStates_5426);
	bool patternMatcher( const Udm::Object& sFState_542c);
	void effector();
	void forwardInputs();

private:
	Packets_t* _sFState_5429;
	Packets_t _sFState_542a;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_5434;
	};

	std::list< Match> _matches;
};

class DuringAction_5440
{
public:
	void operator()( const Packets_t& sFStates_5441, Packets_t& sFStates_5443);

protected:
	bool isInputUnique( const Udm::Object& sFState_5449);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& sFStates_5441);
	bool patternMatcher( const Udm::Object& sFState_5447);
	void effector();
	void forwardInputs();

private:
	Packets_t* _sFState_5444;
	Packets_t _sFState_5445;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_5455;
		SFC::Function exec_5456;
	};

	std::list< Match> _matches;
};

class InitTransConnMap_5457
{
public:
	void operator()( const Packets_t& states_5458, Packets_t& states_545a);

protected:
	void callInitTransConnectorMap_545c( const Packets_t& sFStates_5426);
	void callTransConnectorMapRecurse_545e( const Packets_t& states_3fc2, const Packets_t& tcs_3fc4);

private:
	Packets_t* _state_545b;
};

class OuterTransitions_5461
{
public:
	void operator()( const Packets_t& states_5462, Packets_t& states_5464);

protected:
	void callCheckForOuterTransitions_54ce( const Packets_t& states_5467);
	void callInit_54d0( const Packets_t& states_5344);
	void callExecTransConnectorRecurse_54d2( const Packets_t& states_2e23, const Packets_t& tcs_2e25, const Packets_t& fcns_2e27, const Packets_t& cgs_2e29);

private:
	Packets_t* _state_5465;
};

class CheckForOuterTransitions_5466
{
public:
	void operator()( const Packets_t& states_5467, Packets_t& states_5469);

protected:
	void executeOne( const Packets_t& states_5467);
	bool isInputUnique( const Udm::Object& state_546d);

private:
	Packets_t* _state_546a;
	Packets_t _state_546b;
};

class HasOuterTransition_5472
{
public:
	bool operator()( const Packets_t& sFStates_5473, Packets_t& sFStates_5475);

protected:
	bool isInputUnique( const Udm::Object& sFState_547b);
	void processInputPackets( const Packets_t& sFStates_5473);
	bool patternMatcher( const Udm::Object& sFState_5479);
	void outputAppender( const CyberComposition::Simulink::SFState& sFState_5488);

private:
	Packets_t* _sFState_5476;
	Packets_t _sFState_5477;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_5485;
		CyberComposition::Simulink::SFTransition sFTransition_5486;
		CyberComposition::Simulink::SFTransConnector sFTransConnector_5487;
	};

	std::list< Match> _matches;
};

class HasInnerTransition_548a
{
public:
	bool operator()( const Packets_t& sFStates_548b);

protected:
	bool isInputUnique( const Udm::Object& sFState_5491);
	void processInputPackets( const Packets_t& sFStates_548b);
	bool patternMatcher( const Udm::Object& sFState_548f);
	void outputAppender();

private:
	Packets_t _sFState_548d;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_54a0;
		CyberComposition::Simulink::SFTransition sFTransition_54a1;
		CyberComposition::Simulink::SFConnectorRef sFConnectorRef_54a2;
		CyberComposition::Simulink::SFTransConnector sFTransConnector_54a3;
	};

	std::list< Match> _matches;
};

class HasRefOuterTransition_54a4
{
public:
	bool operator()( const Packets_t& sFStates_54a5, Packets_t& sFStates_54a7);

protected:
	bool isInputUnique( const Udm::Object& sFState_54ad);
	void processInputPackets( const Packets_t& sFStates_54a5);
	bool patternMatcher( const Udm::Object& sFState_54ab);
	void outputAppender( const CyberComposition::Simulink::SFState& sFState_54be);

private:
	Packets_t* _sFState_54a8;
	Packets_t _sFState_54a9;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_54ba;
		CyberComposition::Simulink::SFTransition sFTransition_54bb;
		CyberComposition::Simulink::SFConnectorRef sFConnectorRef_54bc;
		CyberComposition::Simulink::SFTransConnector sFTransConnector_54bd;
	};

	std::list< Match> _matches;
};

class Otherwise_54c0
{
public:
	bool operator()( const Packets_t& sFStates_54c1);

protected:
	bool isInputUnique( const Udm::Object& sFState_54c7);
	void processInputPackets( const Packets_t& sFStates_54c1);
	bool patternMatcher( const Udm::Object& sFState_54c5);
	void outputAppender();

private:
	Packets_t _sFState_54c3;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_54cd;
	};

	std::list< Match> _matches;
};

class PopulateExitFunction_54e3
{
public:
	void operator()( const Packets_t& states_54e4, const Packets_t& programs_54e6, Packets_t& states_54e8, Packets_t& programs_54e9);

protected:
	void callGetExitFunction_5702( const Packets_t& sFStates_56e6);
	void callExitChildStates_5704( const Packets_t& states_5528, const Packets_t& fcns_552a);
	void callGoToParent_5707( const Packets_t& states_54ed, const Packets_t& fcns_54ef);

private:
	Packets_t* _state_54ea;
	Packets_t* _program_54eb;
};

class GoToParent_54ec
{
public:
	void operator()( const Packets_t& states_54ed, const Packets_t& fcns_54ef, Packets_t& states_54f1, Packets_t& fcns_54f2);

protected:
	void callArgToParent_5524( const Packets_t& sFStates_54f6, const Packets_t& functions_54f8);

private:
	Packets_t* _state_54f3;
	Packets_t* _fcn_54f4;
};

class ArgToParent_54f5
{
public:
	void operator()( const Packets_t& sFStates_54f6, const Packets_t& functions_54f8);

protected:
	bool isInputUnique( const Udm::Object& sFState_54fe, const Udm::Object& function_5507);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& sFStates_54f6, const Packets_t& functions_54f8);
	bool patternMatcher( const Udm::Object& sFState_54fc, const Udm::Object& function_5505);
	void effector();

private:
	Packets_t _sFState_54fa;
	Packets_t _function_5503;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_551b;
		SFC::Function function_551c;
		CyberComposition::Simulink::SFState sFParentState_551d;
		SFC::Arg parentArg_551e;
		SFC::Function parentExit_551f;
		SFC::Arg arg_5520;
	};

	std::list< Match> _matches;
};

class ExitChildStates_5527
{
public:
	void operator()( const Packets_t& states_5528, const Packets_t& fcns_552a, Packets_t& states_552c, Packets_t& fcns_552d);

protected:
	void callArgNegOneOrState_56da( const Packets_t& sFStates_56a1, const Packets_t& functions_56a5);
	void callCallChildExits_56dd( const Packets_t& states_5577, const Packets_t& args_5579, const Packets_t& cbs_557b);
	void callDeactivateState_56e1( const Packets_t& sFStates_5531, const Packets_t& args_5533, const Packets_t& childrenCalledCBs_5535);

private:
	Packets_t* _state_552e;
	Packets_t* _fcn_552f;
};

class DeactivateState_5530
{
public:
	void operator()( const Packets_t& sFStates_5531, const Packets_t& args_5533, const Packets_t& childrenCalledCBs_5535);

protected:
	bool isInputUnique( const Udm::Object& sFState_553b, const Udm::Object& arg_5544, const Udm::Object& childrenCalledCB_554d);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& sFStates_5531, const Packets_t& args_5533, const Packets_t& childrenCalledCBs_5535);
	bool patternMatcher( const Udm::Object& sFState_5539, const Udm::Object& arg_5542, const Udm::Object& childrenCalledCB_554b);
	void effector();

private:
	Packets_t _sFState_5537;
	Packets_t _arg_5540;
	Packets_t _childrenCalledCB_5549;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_5566;
		SFC::Arg arg_5567;
		SFC::ConditionalBlock childrenCalledCB_5568;
		CyberComposition::Simulink::SFState sFParentState_5569;
		SFC::Function function_556a;
		SFC::StateLabel stateLabel_556b;
		SFC::StateLabel parentStateLabel_556c;
		SFC::Program program_556d;
		SFC::StateVar stateVar_556e;
	};

	std::list< Match> _matches;
};

class CallChildExits_5576
{
public:
	void operator()( const Packets_t& states_5577, const Packets_t& args_5579, const Packets_t& cbs_557b, Packets_t& states_557d, Packets_t& args_557e, Packets_t& cbs_557f);

protected:
	void callOrStateCG_5696( const Packets_t& states_5584, const Packets_t& css_5586);
	void callROrderChildStates_5699( const Packets_t& sFStates_5625, const Packets_t& compoundStatements_5629);
	void callExitChildStates_569c( const Packets_t& sFStates_5652, const Packets_t& sFChildStates_5654, const Packets_t& compoundStatements_5656);

private:
	Packets_t* _state_5580;
	Packets_t* _arg_5581;
	Packets_t* _cb_5582;
};

class OrStateCG_5583
{
public:
	void operator()( const Packets_t& states_5584, const Packets_t& css_5586, Packets_t& states_5588, Packets_t& css_5589);

protected:
	void callChildStateType_561e( const Packets_t& states_55b3, const Packets_t& css_55b5);
	void callCG_5621( const Packets_t& sFStates_558d, const Packets_t& compoundStatements_5590);

private:
	Packets_t* _state_558a;
	Packets_t* _cs_558b;
};

class CG_558c
{
public:
	void operator()( const Packets_t& sFStates_558d, const Packets_t& compoundStatements_5590, Packets_t& sFStates_558f, Packets_t& conditionalGroups_5592);

protected:
	bool isInputUnique( const Udm::Object& sFState_5599, const Udm::Object& compoundStatement_55a2);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& sFStates_558d, const Packets_t& compoundStatements_5590);
	bool patternMatcher( const Udm::Object& sFState_5597, const Udm::Object& compoundStatement_55a0);
	void effector();
	void outputAppender( const CyberComposition::Simulink::SFState& sFState_55ae, const SFC::ConditionalGroup& conditionalGroup_55b0);

private:
	Packets_t* _sFState_5593;
	Packets_t* _conditionalGroup_5594;
	Packets_t _sFState_5595;
	Packets_t _compoundStatement_559e;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_55ab;
		SFC::CompoundStatement compoundStatement_55ac;
	};

	std::list< Match> _matches;
};

class ChildStateType_55b2
{
public:
	void operator()( const Packets_t& states_55b3, const Packets_t& css_55b5, Packets_t& states_55b7, Packets_t& css_55b8, Packets_t& states_55b9, Packets_t& css_55ba);

protected:
	void executeOne( const Packets_t& states_55b3, const Packets_t& css_55b5);
	bool isInputUnique( const Udm::Object& state_55c1, const Udm::Object& cs_55c8);

private:
	Packets_t* _state_55bb;
	Packets_t* _cs_55bc;
	Packets_t* _state_55bd;
	Packets_t* _cs_55be;
	Packets_t _state_55bf;
	Packets_t _cs_55c6;
};

class ChildAndStates_55cd
{
public:
	bool operator()( const Packets_t& sFStates_55ce, const Packets_t& compoundStatements_55d1, Packets_t& sFStates_55d0, Packets_t& compoundStatements_55d3);

protected:
	bool isInputUnique( const Udm::Object& sFState_55da, const Udm::Object& compoundStatement_55e3);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::CompoundStatement& CompoundStatement, CyberComposition::Simulink::SFState& SFChildState, CyberComposition::Simulink::SFState& SFState);
	void processInputPackets( const Packets_t& sFStates_55ce, const Packets_t& compoundStatements_55d1);
	bool patternMatcher( const Udm::Object& sFState_55d8, const Udm::Object& compoundStatement_55e1);
	void outputAppender( const CyberComposition::Simulink::SFState& sFState_55f5, const SFC::CompoundStatement& compoundStatement_55f7);

private:
	Packets_t* _sFState_55d4;
	Packets_t* _compoundStatement_55d5;
	Packets_t _sFState_55d6;
	Packets_t _compoundStatement_55df;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_55ef;
		SFC::CompoundStatement compoundStatement_55f0;
		CyberComposition::Simulink::SFState sFChildState_55f1;
	};

	std::list< Match> _matches;
};

class ChildOrStates_55f9
{
public:
	bool operator()( const Packets_t& sFStates_55fa, const Packets_t& compoundStatements_55fd, Packets_t& sFStates_55fc, Packets_t& compoundStatements_55ff);

protected:
	bool isInputUnique( const Udm::Object& sFState_5606, const Udm::Object& compoundStatement_560f);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& sFStates_55fa, const Packets_t& compoundStatements_55fd);
	bool patternMatcher( const Udm::Object& sFState_5604, const Udm::Object& compoundStatement_560d);
	void outputAppender( const CyberComposition::Simulink::SFState& sFState_561a, const SFC::CompoundStatement& compoundStatement_561c);

private:
	Packets_t* _sFState_5600;
	Packets_t* _compoundStatement_5601;
	Packets_t _sFState_5602;
	Packets_t _compoundStatement_560b;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_5618;
		SFC::CompoundStatement compoundStatement_5619;
	};

	std::list< Match> _matches;
};

class ROrderChildStates_5624
{
public:
	void operator()( const Packets_t& sFStates_5625, const Packets_t& compoundStatements_5629, Packets_t& sFStates_5627, Packets_t& sFStates_5628, Packets_t& compoundStatements_562b);

protected:
	bool isInputUnique( const Udm::Object& sFState_5633, const Udm::Object& compoundStatement_563c);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& sFStates_5625, const Packets_t& compoundStatements_5629);
	bool patternMatcher( const Udm::Object& sFState_5631, const Udm::Object& compoundStatement_563a);
	void effector();
	void outputAppender( const CyberComposition::Simulink::SFState& sFState_564b, const CyberComposition::Simulink::SFState& sFState_564d, const SFC::CompoundStatement& compoundStatement_564f);
	void sortOutputs();

private:
	Packets_t* _sFState_562c;
	Packets_t* _sFState_562d;
	Packets_t* _compoundStatement_562e;
	Packets_t _sFState_562f;
	Packets_t _compoundStatement_5638;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_5648;
		SFC::CompoundStatement compoundStatement_5649;
		CyberComposition::Simulink::SFState sFState_564a;
	};

	std::list< Match> _matches;
};

class ExitChildStates_5651
{
public:
	void operator()( const Packets_t& sFStates_5652, const Packets_t& sFChildStates_5654, const Packets_t& compoundStatements_5656);

protected:
	bool isInputUnique( const Udm::Object& sFState_565c, const Udm::Object& sFChildState_5665, const Udm::Object& compoundStatement_566e);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& sFStates_5652, const Packets_t& sFChildStates_5654, const Packets_t& compoundStatements_5656);
	bool patternMatcher( const Udm::Object& sFState_565a, const Udm::Object& sFChildState_5663, const Udm::Object& compoundStatement_566c);
	void effector();

private:
	Packets_t _sFState_5658;
	Packets_t _sFChildState_5661;
	Packets_t _compoundStatement_566a;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_5688;
		CyberComposition::Simulink::SFState sFChildState_5689;
		SFC::CompoundStatement compoundStatement_568a;
		SFC::StateLabel childStateLabel_568b;
		SFC::StateLabel stateLabel_568c;
		SFC::StateVar stateVar_568d;
		SFC::Arg arg_568e;
		SFC::Function exit_568f;
		SFC::Program program_5690;
	};

	std::list< Match> _matches;
};

class ArgNegOneOrState_56a0
{
public:
	void operator()( const Packets_t& sFStates_56a1, const Packets_t& functions_56a5, Packets_t& sFStates_56a3, Packets_t& args_56a4, Packets_t& conditionalBlocks_56a7);

protected:
	bool isInputUnique( const Udm::Object& sFState_56af, const Udm::Object& function_56b8);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& sFStates_56a1, const Packets_t& functions_56a5);
	bool patternMatcher( const Udm::Object& sFState_56ad, const Udm::Object& function_56b6);
	void effector();
	void outputAppender( const CyberComposition::Simulink::SFState& sFState_56d4, const SFC::Arg& arg_56d6, const SFC::ConditionalBlock& conditionalBlock_56d8);

private:
	Packets_t* _sFState_56a8;
	Packets_t* _arg_56a9;
	Packets_t* _conditionalBlock_56aa;
	Packets_t _sFState_56ab;
	Packets_t _function_56b4;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_56c7;
		SFC::Function function_56c8;
		SFC::StateLabel stateLabel_56c9;
		SFC::Arg arg_56ca;
	};

	std::list< Match> _matches;
};

class GetExitFunction_56e5
{
public:
	void operator()( const Packets_t& sFStates_56e6, Packets_t& sFStates_56e8, Packets_t& exits_56e9);

protected:
	bool isInputUnique( const Udm::Object& sFState_56f0);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& sFStates_56e6);
	bool patternMatcher( const Udm::Object& sFState_56ee);
	void effector();
	void outputAppender( const CyberComposition::Simulink::SFState& sFState_56fe, const SFC::Function& exit_5700);

private:
	Packets_t* _sFState_56ea;
	Packets_t* _exit_56eb;
	Packets_t _sFState_56ec;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_56fc;
		SFC::Function exit_56fd;
	};

	std::list< Match> _matches;
};

class CreateFunctions_571c
{
public:
	void operator()( const Packets_t& states_571d, const Packets_t& programs_571f, Packets_t& states_5721, Packets_t& programs_5722);

protected:
	void callCreateFunctions_5758( const Packets_t& sFStates_5726, const Packets_t& programs_5729);
	void callGetSubStates_575b( const Packets_t& sFStates_4223, const Packets_t& parStmnts_4226);

private:
	Packets_t* _state_5723;
	Packets_t* _program_5724;
};

class CreateFunctions_5725
{
public:
	void operator()( const Packets_t& sFStates_5726, const Packets_t& programs_5729, Packets_t& sFStates_5728, Packets_t& programs_572b);

protected:
	bool isInputUnique( const Udm::Object& sFState_5732, const Udm::Object& program_573b);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::Program& Program, CyberComposition::Simulink::SFState& SFState, SFC::StateLabel& StateLabel);
	void processInputPackets( const Packets_t& sFStates_5726, const Packets_t& programs_5729);
	bool patternMatcher( const Udm::Object& sFState_5730, const Udm::Object& program_5739);
	void effector();
	void forwardInputs();

private:
	Packets_t* _sFState_572c;
	Packets_t* _program_572d;
	Packets_t _sFState_572e;
	Packets_t _program_5737;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_5747;
		SFC::Program program_5748;
		SFC::StateLabel stateLabel_5749;
	};

	std::list< Match> _matches;
};

class CreateRootFunction_575e
{
public:
	void operator()( const Packets_t& states_575f, const Packets_t& programs_5761, Packets_t& states_5763, Packets_t& programs_5764);

protected:
	void callCreateRootFunction_5991( const Packets_t& sFStates_578e, const Packets_t& programs_5791);
	void callCreateInputArgs_5994( const Packets_t& states_57b4, const Packets_t& rootFxns_57b6);
	void callCreateRootExecCall_5997( const Packets_t& sFStates_5768, const Packets_t& rootFunctions_576b);
	void callCreateOutputArgs_599a( const Packets_t& states_58bd, const Packets_t& rootFxns_58bf);

private:
	Packets_t* _state_5765;
	Packets_t* _program_5766;
};

class CreateRootExecCall_5767
{
public:
	void operator()( const Packets_t& sFStates_5768, const Packets_t& rootFunctions_576b, Packets_t& sFStates_576a, Packets_t& rootFunctions_576d);

protected:
	bool isInputUnique( const Udm::Object& sFState_5774, const Udm::Object& rootFunction_577d);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& sFStates_5768, const Packets_t& rootFunctions_576b);
	bool patternMatcher( const Udm::Object& sFState_5772, const Udm::Object& rootFunction_577b);
	void effector();
	void forwardInputs();

private:
	Packets_t* _sFState_576e;
	Packets_t* _rootFunction_576f;
	Packets_t _sFState_5770;
	Packets_t _rootFunction_5779;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_5789;
		SFC::Function rootFunction_578a;
		SFC::Function exec_578b;
	};

	std::list< Match> _matches;
};

class CreateRootFunction_578d
{
public:
	void operator()( const Packets_t& sFStates_578e, const Packets_t& programs_5791, Packets_t& sFStates_5790, Packets_t& rootFunctions_5793);

protected:
	bool isInputUnique( const Udm::Object& sFState_579a, const Udm::Object& program_57a3);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& sFStates_578e, const Packets_t& programs_5791);
	bool patternMatcher( const Udm::Object& sFState_5798, const Udm::Object& program_57a1);
	void effector();
	void outputAppender( const CyberComposition::Simulink::SFState& sFState_57af, const SFC::Function& rootFunction_57b1);

private:
	Packets_t* _sFState_5794;
	Packets_t* _rootFunction_5795;
	Packets_t _sFState_5796;
	Packets_t _program_579f;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_57ac;
		SFC::Program program_57ad;
	};

	std::list< Match> _matches;
};

class CreateInputArgs_57b3
{
public:
	void operator()( const Packets_t& states_57b4, const Packets_t& rootFxns_57b6, Packets_t& states_57b8, Packets_t& rootFxns_57b9);

protected:
	void callCheckStateDecomp_58ad( const Packets_t& states_47be, const Packets_t& programs_47c0);
	void callGetSubStates_58b0( const Packets_t& sFStates_4223, const Packets_t& parStmnts_4226);
	void callCreateIDArgs_58b3( const Packets_t& states_57ee, const Packets_t& rootFxns_57f0);
	void callCreateIEArgs_58b6( const Packets_t& states_584e, const Packets_t& rootFxns_5850);
	void callCreateLEInit_58b9( const Packets_t& sFStates_57bd, const Packets_t& rootFunctions_57c0);

private:
	Packets_t* _state_57ba;
	Packets_t* _rootFxn_57bb;
};

class CreateLEInit_57bc
{
public:
	void operator()( const Packets_t& sFStates_57bd, const Packets_t& rootFunctions_57c0, Packets_t& sFStates_57bf, Packets_t& rootFunctions_57c2);

protected:
	bool isInputUnique( const Udm::Object& sFState_57c9, const Udm::Object& rootFunction_57d2);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::LocalVar& LocalVar, SFC::Function& RootFunction, CyberComposition::Simulink::SFEvent& SFEvent, CyberComposition::Simulink::SFState& SFState);
	void processInputPackets( const Packets_t& sFStates_57bd, const Packets_t& rootFunctions_57c0);
	bool patternMatcher( const Udm::Object& sFState_57c7, const Udm::Object& rootFunction_57d0);
	void effector();
	void forwardInputs();

private:
	Packets_t* _sFState_57c3;
	Packets_t* _rootFunction_57c4;
	Packets_t _sFState_57c5;
	Packets_t _rootFunction_57ce;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_57e1;
		SFC::Function rootFunction_57e2;
		CyberComposition::Simulink::SFEvent sFEvent_57e3;
		SFC::LocalVar localVar_57e4;
	};

	std::list< Match> _matches;
};

class CreateIDArgs_57ed
{
public:
	void operator()( const Packets_t& states_57ee, const Packets_t& rootFxns_57f0, Packets_t& states_57f2, Packets_t& rootFxns_57f3);

protected:
	void callGetInData_5847( const Packets_t& sFStates_581d, const Packets_t& rootFunctions_581f);
	void callCreateIDArg_584a( const Packets_t& sFDatas_57f7, const Packets_t& rootFunctions_57f9);

private:
	Packets_t* _state_57f4;
	Packets_t* _rootFxn_57f5;
};

class CreateIDArg_57f6
{
public:
	void operator()( const Packets_t& sFDatas_57f7, const Packets_t& rootFunctions_57f9);

protected:
	bool isInputUnique( const Udm::Object& sFData_57ff, const Udm::Object& rootFunction_5808);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& sFDatas_57f7, const Packets_t& rootFunctions_57f9);
	bool patternMatcher( const Udm::Object& sFData_57fd, const Udm::Object& rootFunction_5806);
	void effector();

private:
	Packets_t _sFData_57fb;
	Packets_t _rootFunction_5804;
	class Match
	{
	public:
		CyberComposition::Simulink::SFData sFData_5815;
		SFC::Function rootFunction_5816;
		SFC::LocalVar localVar_5817;
		SFC::DT dT_5818;
	};

	std::list< Match> _matches;
};

class GetInData_581b
{
public:
	void operator()( const Packets_t& sFStates_581d, const Packets_t& rootFunctions_581f, Packets_t& sFDatas_581c, Packets_t& rootFunctions_5821);

protected:
	bool isInputUnique( const Udm::Object& sFState_5828, const Udm::Object& rootFunction_5831);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::Function& RootFunction, CyberComposition::Simulink::SFData& SFData, CyberComposition::Simulink::SFState& SFState);
	void processInputPackets( const Packets_t& sFStates_581d, const Packets_t& rootFunctions_581f);
	bool patternMatcher( const Udm::Object& sFState_5826, const Udm::Object& rootFunction_582f);
	void effector();
	void outputAppender( const CyberComposition::Simulink::SFData& sFData_5843, const SFC::Function& rootFunction_5845);
	void sortOutputs();

private:
	Packets_t* _sFData_5822;
	Packets_t* _rootFunction_5823;
	Packets_t _sFState_5824;
	Packets_t _rootFunction_582d;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_583d;
		SFC::Function rootFunction_583e;
		CyberComposition::Simulink::SFData sFData_583f;
	};

	std::list< Match> _matches;
};

class CreateIEArgs_584d
{
public:
	void operator()( const Packets_t& states_584e, const Packets_t& rootFxns_5850, Packets_t& states_5852, Packets_t& rootFxns_5853);

protected:
	void callGetInEvent_58a7( const Packets_t& sFStates_587c, const Packets_t& rootFunctions_587f);
	void callCreateIEArg_58aa( const Packets_t& sFEvents_5857, const Packets_t& rootFunctions_5859);

private:
	Packets_t* _state_5854;
	Packets_t* _rootFxn_5855;
};

class CreateIEArg_5856
{
public:
	void operator()( const Packets_t& sFEvents_5857, const Packets_t& rootFunctions_5859);

protected:
	bool isInputUnique( const Udm::Object& sFEvent_585f, const Udm::Object& rootFunction_5868);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& sFEvents_5857, const Packets_t& rootFunctions_5859);
	bool patternMatcher( const Udm::Object& sFEvent_585d, const Udm::Object& rootFunction_5866);
	void effector();

private:
	Packets_t _sFEvent_585b;
	Packets_t _rootFunction_5864;
	class Match
	{
	public:
		CyberComposition::Simulink::SFEvent sFEvent_5875;
		SFC::Function rootFunction_5876;
		SFC::LocalVar localVar_5877;
		SFC::DT dT_5878;
	};

	std::list< Match> _matches;
};

class GetInEvent_587b
{
public:
	void operator()( const Packets_t& sFStates_587c, const Packets_t& rootFunctions_587f, Packets_t& sFEvents_587e, Packets_t& rootFunctions_5881);

protected:
	bool isInputUnique( const Udm::Object& sFState_5888, const Udm::Object& rootFunction_5891);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::Function& RootFunction, CyberComposition::Simulink::SFEvent& SFEvent, CyberComposition::Simulink::SFState& SFState);
	void processInputPackets( const Packets_t& sFStates_587c, const Packets_t& rootFunctions_587f);
	bool patternMatcher( const Udm::Object& sFState_5886, const Udm::Object& rootFunction_588f);
	void effector();
	void outputAppender( const CyberComposition::Simulink::SFEvent& sFEvent_58a3, const SFC::Function& rootFunction_58a5);
	void sortOutputs();

private:
	Packets_t* _sFEvent_5882;
	Packets_t* _rootFunction_5883;
	Packets_t _sFState_5884;
	Packets_t _rootFunction_588d;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_589d;
		SFC::Function rootFunction_589e;
		CyberComposition::Simulink::SFEvent sFEvent_589f;
	};

	std::list< Match> _matches;
};

class CreateOutputArgs_58bc
{
public:
	void operator()( const Packets_t& states_58bd, const Packets_t& rootFxns_58bf, Packets_t& states_58c1, Packets_t& rootFxns_58c2);

protected:
	void callCheckStateDecomp_5985( const Packets_t& states_47be, const Packets_t& programs_47c0);
	void callGetSubStates_5988( const Packets_t& sFStates_4223, const Packets_t& parStmnts_4226);
	void callCreateODArgs_598b( const Packets_t& states_58c6, const Packets_t& rootFxns_58c8);
	void callCreateOEArgs_598e( const Packets_t& states_5926, const Packets_t& rootFxns_5928);

private:
	Packets_t* _state_58c3;
	Packets_t* _rootFxn_58c4;
};

class CreateODArgs_58c5
{
public:
	void operator()( const Packets_t& states_58c6, const Packets_t& rootFxns_58c8, Packets_t& states_58ca, Packets_t& rootFxns_58cb);

protected:
	void callGetOutData_591f( const Packets_t& sFStates_58d0, const Packets_t& rootFunctions_58d2);
	void callCreateODArg_5922( const Packets_t& sFDatas_58fb, const Packets_t& rootFunctions_58fd);

private:
	Packets_t* _state_58cc;
	Packets_t* _rootFxn_58cd;
};

class GetOutData_58ce
{
public:
	void operator()( const Packets_t& sFStates_58d0, const Packets_t& rootFunctions_58d2, Packets_t& sFDatas_58cf, Packets_t& rootFunctions_58d4);

protected:
	bool isInputUnique( const Udm::Object& sFState_58db, const Udm::Object& rootFunction_58e4);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::Function& RootFunction, CyberComposition::Simulink::SFData& SFData, CyberComposition::Simulink::SFState& SFState);
	void processInputPackets( const Packets_t& sFStates_58d0, const Packets_t& rootFunctions_58d2);
	bool patternMatcher( const Udm::Object& sFState_58d9, const Udm::Object& rootFunction_58e2);
	void effector();
	void outputAppender( const CyberComposition::Simulink::SFData& sFData_58f6, const SFC::Function& rootFunction_58f8);
	void sortOutputs();

private:
	Packets_t* _sFData_58d5;
	Packets_t* _rootFunction_58d6;
	Packets_t _sFState_58d7;
	Packets_t _rootFunction_58e0;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_58f0;
		SFC::Function rootFunction_58f1;
		CyberComposition::Simulink::SFData sFData_58f2;
	};

	std::list< Match> _matches;
};

class CreateODArg_58fa
{
public:
	void operator()( const Packets_t& sFDatas_58fb, const Packets_t& rootFunctions_58fd);

protected:
	bool isInputUnique( const Udm::Object& sFData_5903, const Udm::Object& rootFunction_590c);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& sFDatas_58fb, const Packets_t& rootFunctions_58fd);
	bool patternMatcher( const Udm::Object& sFData_5901, const Udm::Object& rootFunction_590a);
	void effector();

private:
	Packets_t _sFData_58ff;
	Packets_t _rootFunction_5908;
	class Match
	{
	public:
		CyberComposition::Simulink::SFData sFData_5919;
		SFC::Function rootFunction_591a;
		SFC::LocalVar localVar_591b;
		SFC::DT dT_591c;
	};

	std::list< Match> _matches;
};

class CreateOEArgs_5925
{
public:
	void operator()( const Packets_t& states_5926, const Packets_t& rootFxns_5928, Packets_t& states_592a, Packets_t& rootFxns_592b);

protected:
	void callGetOutEvent_597f( const Packets_t& sFStates_592f, const Packets_t& rootFunctions_5932);
	void callCreateOEArg_5982( const Packets_t& sFEvents_595b, const Packets_t& rootFunctions_595d);

private:
	Packets_t* _state_592c;
	Packets_t* _rootFxn_592d;
};

class GetOutEvent_592e
{
public:
	void operator()( const Packets_t& sFStates_592f, const Packets_t& rootFunctions_5932, Packets_t& sFEvents_5931, Packets_t& rootFunctions_5934);

protected:
	bool isInputUnique( const Udm::Object& sFState_593b, const Udm::Object& rootFunction_5944);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::Function& RootFunction, CyberComposition::Simulink::SFEvent& SFEvent, CyberComposition::Simulink::SFState& SFState);
	void processInputPackets( const Packets_t& sFStates_592f, const Packets_t& rootFunctions_5932);
	bool patternMatcher( const Udm::Object& sFState_5939, const Udm::Object& rootFunction_5942);
	void effector();
	void outputAppender( const CyberComposition::Simulink::SFEvent& sFEvent_5956, const SFC::Function& rootFunction_5958);
	void sortOutputs();

private:
	Packets_t* _sFEvent_5935;
	Packets_t* _rootFunction_5936;
	Packets_t _sFState_5937;
	Packets_t _rootFunction_5940;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_5950;
		SFC::Function rootFunction_5951;
		CyberComposition::Simulink::SFEvent sFEvent_5952;
	};

	std::list< Match> _matches;
};

class CreateOEArg_595a
{
public:
	void operator()( const Packets_t& sFEvents_595b, const Packets_t& rootFunctions_595d);

protected:
	bool isInputUnique( const Udm::Object& sFEvent_5963, const Udm::Object& rootFunction_596c);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& sFEvents_595b, const Packets_t& rootFunctions_595d);
	bool patternMatcher( const Udm::Object& sFEvent_5961, const Udm::Object& rootFunction_596a);
	void effector();

private:
	Packets_t _sFEvent_595f;
	Packets_t _rootFunction_5968;
	class Match
	{
	public:
		CyberComposition::Simulink::SFEvent sFEvent_5979;
		SFC::Function rootFunction_597a;
		SFC::LocalVar localVar_597b;
		SFC::DT dT_597c;
	};

	std::list< Match> _matches;
};

class CreateInitFunction_599d
{
public:
	void operator()( const Packets_t& states_599e, const Packets_t& programs_59a0, Packets_t& states_59a2, Packets_t& programs_59a3);

protected:
	void callCreateInitFunction_5bc6( const Packets_t& sFStates_5a45, const Packets_t& programs_5a48);
	void callInitLocalVars_5bc9( const Packets_t& states_5a78, const Packets_t& programs_5a7a, const Packets_t& initFns_5a7c, const Packets_t& initIdxs_5a7e);
	void callTestEAIOn_5bce( const Packets_t& states_59a7, const Packets_t& initfns_59a9);
	void callCreateFunction_FunctionCall_5bd1( const Packets_t& sFStates_59ee, const Packets_t& initFns_59f1);
	void callFunctionCall_Vals_5bd4( const Packets_t& enters_5a18, const Packets_t& functionCalls_5a1a);

private:
	Packets_t* _state_59a4;
	Packets_t* _program_59a5;
};

class TestEAIOn_59a6
{
public:
	void operator()( const Packets_t& states_59a7, const Packets_t& initfns_59a9, Packets_t& states_59ab, Packets_t& initfns_59ac);

protected:
	void executeOne( const Packets_t& states_59a7, const Packets_t& initfns_59a9);
	bool isInputUnique( const Udm::Object& state_59b1, const Udm::Object& initfn_59b8);

private:
	Packets_t* _state_59ad;
	Packets_t* _initfn_59ae;
	Packets_t _state_59af;
	Packets_t _initfn_59b6;
};

class EAIOn_59bd
{
public:
	bool operator()( const Packets_t& sFStates_59be, const Packets_t& initFns_59c1, Packets_t& sFStates_59c0, Packets_t& initFns_59c3);

protected:
	bool isInputUnique( const Udm::Object& sFState_59ca, const Udm::Object& initFn_59d3);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::Function& InitFn, CyberComposition::Simulink::SF_Parameter& Parameter, CyberComposition::Simulink::SFState& SFState, CyberComposition::Simulink::Subsystem& Subsystem);
	void processInputPackets( const Packets_t& sFStates_59be, const Packets_t& initFns_59c1);
	bool patternMatcher( const Udm::Object& sFState_59c8, const Udm::Object& initFn_59d1);
	void outputAppender( const CyberComposition::Simulink::SFState& sFState_59e9, const SFC::Function& initFn_59eb);

private:
	Packets_t* _sFState_59c4;
	Packets_t* _initFn_59c5;
	Packets_t _sFState_59c6;
	Packets_t _initFn_59cf;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_59e1;
		SFC::Function initFn_59e2;
		CyberComposition::Simulink::SF_Parameter parameter_59e3;
		CyberComposition::Simulink::Subsystem subsystem_59e4;
	};

	std::list< Match> _matches;
};

class CreateFunction_FunctionCall_59ed
{
public:
	void operator()( const Packets_t& sFStates_59ee, const Packets_t& initFns_59f1, Packets_t& enters_59f0, Packets_t& functionCalls_59f3);

protected:
	bool isInputUnique( const Udm::Object& sFState_59fa, const Udm::Object& initFn_5a03);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& sFStates_59ee, const Packets_t& initFns_59f1);
	bool patternMatcher( const Udm::Object& sFState_59f8, const Udm::Object& initFn_5a01);
	void effector();
	void outputAppender( const SFC::Function& enter_5a13, const SFC::FunctionCall& functionCall_5a15);

private:
	Packets_t* _enter_59f4;
	Packets_t* _functionCall_59f5;
	Packets_t _sFState_59f6;
	Packets_t _initFn_59ff;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_5a0f;
		SFC::Function initFn_5a10;
		SFC::Function enter_5a11;
	};

	std::list< Match> _matches;
};

class FunctionCall_Vals_5a17
{
public:
	void operator()( const Packets_t& enters_5a18, const Packets_t& functionCalls_5a1a);

protected:
	bool isInputUnique( const Udm::Object& enter_5a20, const Udm::Object& functionCall_5a29);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::Arg& Arg0, SFC::Arg& Arg1, SFC::Function& Enter, SFC::FunctionCall& FunctionCall);
	void processInputPackets( const Packets_t& enters_5a18, const Packets_t& functionCalls_5a1a);
	bool patternMatcher( const Udm::Object& enter_5a1e, const Udm::Object& functionCall_5a27);
	void effector();

private:
	Packets_t _enter_5a1c;
	Packets_t _functionCall_5a25;
	class Match
	{
	public:
		SFC::Function enter_5a38;
		SFC::FunctionCall functionCall_5a39;
		SFC::Arg arg0_5a3a;
		SFC::Arg arg1_5a3b;
	};

	std::list< Match> _matches;
};

class CreateInitFunction_5a44
{
public:
	void operator()( const Packets_t& sFStates_5a45, const Packets_t& programs_5a48, Packets_t& sFStates_5a47, Packets_t& programs_5a4a, Packets_t& initFunctions_5a4b, Packets_t& initIdxs_5a4c);

protected:
	bool isInputUnique( const Udm::Object& sFState_5a55, const Udm::Object& program_5a5e);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& sFStates_5a45, const Packets_t& programs_5a48);
	bool patternMatcher( const Udm::Object& sFState_5a53, const Udm::Object& program_5a5c);
	void effector();
	void outputAppender( const CyberComposition::Simulink::SFState& sFState_5a6f, const SFC::Program& program_5a71, const SFC::Function& initFunction_5a73, const SFC::LocalVar& initIdx_5a75);

private:
	Packets_t* _sFState_5a4d;
	Packets_t* _program_5a4e;
	Packets_t* _initFunction_5a4f;
	Packets_t* _initIdx_5a50;
	Packets_t _sFState_5a51;
	Packets_t _program_5a5a;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_5a6a;
		SFC::Program program_5a6b;
		SFC::StateVar stateVar_5a6c;
	};

	std::list< Match> _matches;
};

class InitLocalVars_5a77
{
public:
	void operator()( const Packets_t& states_5a78, const Packets_t& programs_5a7a, const Packets_t& initFns_5a7c, const Packets_t& initIdxs_5a7e, Packets_t& states_5a80, Packets_t& initfns_5a81);

protected:
	void callGetSortedVars_5bb6( const Packets_t& programs_5a85, const Packets_t& initFns_5a88, const Packets_t& initIdxs_5a8b);
	void callArrayVars_5bba( const Packets_t& programs_5af9, const Packets_t& initFns_5afb, const Packets_t& initIdxs_5afd);
	void callInitArrayVar_5bbe( const Packets_t& vars_5abe, const Packets_t& initFns_5ac0, const Packets_t& idxVars_5ac2);
	void callInitScalarVar_5bc2( const Packets_t& vars_5b8e, const Packets_t& initFns_5b90, const Packets_t& idxVars_5b92);

private:
	Packets_t* _state_5a82;
	Packets_t* _initfn_5a83;
};

class GetSortedVars_5a84
{
public:
	void operator()( const Packets_t& programs_5a85, const Packets_t& initFns_5a88, const Packets_t& initIdxs_5a8b, Packets_t& vars_5a87, Packets_t& initFns_5a8a, Packets_t& initIdxs_5a8d);

protected:
	bool isInputUnique( const Udm::Object& program_5a95, const Udm::Object& initFn_5a9e, const Udm::Object& initIdx_5aa7);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& programs_5a85, const Packets_t& initFns_5a88, const Packets_t& initIdxs_5a8b);
	bool patternMatcher( const Udm::Object& program_5a93, const Udm::Object& initFn_5a9c, const Udm::Object& initIdx_5aa5);
	void effector();
	void outputAppender( const SFC::Var& var_5ab7, const SFC::Function& initFn_5ab9, const SFC::LocalVar& initIdx_5abb);
	void sortOutputs();

private:
	Packets_t* _var_5a8e;
	Packets_t* _initFn_5a8f;
	Packets_t* _initIdx_5a90;
	Packets_t _program_5a91;
	Packets_t _initFn_5a9a;
	Packets_t _initIdx_5aa3;
	class Match
	{
	public:
		SFC::Program program_5ab3;
		SFC::Function initFn_5ab4;
		SFC::LocalVar initIdx_5ab5;
		SFC::Var var_5ab6;
	};

	std::list< Match> _matches;
};

class InitArrayVar_5abd
{
public:
	void operator()( const Packets_t& vars_5abe, const Packets_t& initFns_5ac0, const Packets_t& idxVars_5ac2);

protected:
	bool isInputUnique( const Udm::Object& var_5ac8, const Udm::Object& initFn_5ad1, const Udm::Object& idxVar_5ada);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& vars_5abe, const Packets_t& initFns_5ac0, const Packets_t& idxVars_5ac2);
	bool patternMatcher( const Udm::Object& var_5ac6, const Udm::Object& initFn_5acf, const Udm::Object& idxVar_5ad8);
	void effector();

private:
	Packets_t _var_5ac4;
	Packets_t _initFn_5acd;
	Packets_t _idxVar_5ad6;
	class Match
	{
	public:
		SFC::Var var_5ae3;
		SFC::Function initFn_5ae4;
		SFC::LocalVar idxVar_5ae5;
	};

	std::list< Match> _matches;
};

class ArrayVars_5af8
{
public:
	void operator()( const Packets_t& programs_5af9, const Packets_t& initFns_5afb, const Packets_t& initIdxs_5afd, Packets_t& vars_5aff, Packets_t& initfns_5b00, Packets_t& initidxs_5b01, Packets_t& vars_5b02, Packets_t& initfns_5b03, Packets_t& initidxs_5b04);

protected:
	void executeOne( const Packets_t& programs_5af9, const Packets_t& initFns_5afb, const Packets_t& initIdxs_5afd);
	bool isInputUnique( const Udm::Object& program_5b0d, const Udm::Object& initFn_5b14, const Udm::Object& initIdx_5b1b);

private:
	Packets_t* _var_5b05;
	Packets_t* _initfn_5b06;
	Packets_t* _initidx_5b07;
	Packets_t* _var_5b08;
	Packets_t* _initfn_5b09;
	Packets_t* _initidx_5b0a;
	Packets_t _program_5b0b;
	Packets_t _initFn_5b12;
	Packets_t _initIdx_5b19;
};

class ArrayVar_5b20
{
public:
	bool operator()( const Packets_t& vars_5b21, const Packets_t& initFns_5b24, const Packets_t& initIdxs_5b27, Packets_t& vars_5b23, Packets_t& initFns_5b26, Packets_t& initIdxs_5b29);

protected:
	bool isInputUnique( const Udm::Object& var_5b31, const Udm::Object& initFn_5b3a, const Udm::Object& initIdx_5b43);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	bool isGuardTrue( SFC::Function& InitFn, SFC::LocalVar& InitIdx, SFC::Var& Var);
	void processInputPackets( const Packets_t& vars_5b21, const Packets_t& initFns_5b24, const Packets_t& initIdxs_5b27);
	bool patternMatcher( const Udm::Object& var_5b2f, const Udm::Object& initFn_5b38, const Udm::Object& initIdx_5b41);
	void outputAppender( const SFC::Var& var_5b52, const SFC::Function& initFn_5b54, const SFC::LocalVar& initIdx_5b56);
	void sortOutputs();

private:
	Packets_t* _var_5b2a;
	Packets_t* _initFn_5b2b;
	Packets_t* _initIdx_5b2c;
	Packets_t _var_5b2d;
	Packets_t _initFn_5b36;
	Packets_t _initIdx_5b3f;
	class Match
	{
	public:
		SFC::Var var_5b4c;
		SFC::Function initFn_5b4d;
		SFC::LocalVar initIdx_5b4e;
	};

	std::list< Match> _matches;
};

class Otherwise_5b58
{
public:
	bool operator()( const Packets_t& vars_5b59, const Packets_t& initFns_5b5c, const Packets_t& initIdxs_5b5f, Packets_t& vars_5b5b, Packets_t& initFns_5b5e, Packets_t& initIdxs_5b61);

protected:
	bool isInputUnique( const Udm::Object& var_5b69, const Udm::Object& initFn_5b72, const Udm::Object& initIdx_5b7b);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& vars_5b59, const Packets_t& initFns_5b5c, const Packets_t& initIdxs_5b5f);
	bool patternMatcher( const Udm::Object& var_5b67, const Udm::Object& initFn_5b70, const Udm::Object& initIdx_5b79);
	void outputAppender( const SFC::Var& var_5b87, const SFC::Function& initFn_5b89, const SFC::LocalVar& initIdx_5b8b);
	void sortOutputs();

private:
	Packets_t* _var_5b62;
	Packets_t* _initFn_5b63;
	Packets_t* _initIdx_5b64;
	Packets_t _var_5b65;
	Packets_t _initFn_5b6e;
	Packets_t _initIdx_5b77;
	class Match
	{
	public:
		SFC::Var var_5b84;
		SFC::Function initFn_5b85;
		SFC::LocalVar initIdx_5b86;
	};

	std::list< Match> _matches;
};

class InitScalarVar_5b8d
{
public:
	void operator()( const Packets_t& vars_5b8e, const Packets_t& initFns_5b90, const Packets_t& idxVars_5b92);

protected:
	bool isInputUnique( const Udm::Object& var_5b98, const Udm::Object& initFn_5ba1, const Udm::Object& idxVar_5baa);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& vars_5b8e, const Packets_t& initFns_5b90, const Packets_t& idxVars_5b92);
	bool patternMatcher( const Udm::Object& var_5b96, const Udm::Object& initFn_5b9f, const Udm::Object& idxVar_5ba8);
	void effector();

private:
	Packets_t _var_5b94;
	Packets_t _initFn_5b9d;
	Packets_t _idxVar_5ba6;
	class Match
	{
	public:
		SFC::Var var_5bb3;
		SFC::Function initFn_5bb4;
		SFC::LocalVar idxVar_5bb5;
	};

	std::list< Match> _matches;
};

class MarkLegacy_5bd7
{
public:
	void operator()( const Packets_t& sFStates_5bd8, const Packets_t& programs_5bda);

protected:
	bool isInputUnique( const Udm::Object& sFState_5be0, const Udm::Object& program_5be9);
	bool isValidBound( set< pair<int, Udm::Object> >& boundObjs, Udm::Object& currObj, bool isInputObj);
	void processInputPackets( const Packets_t& sFStates_5bd8, const Packets_t& programs_5bda);
	bool patternMatcher( const Udm::Object& sFState_5bde, const Udm::Object& program_5be7);
	void effector();

private:
	Packets_t _sFState_5bdc;
	Packets_t _program_5be5;
	class Match
	{
	public:
		CyberComposition::Simulink::SFState sFState_5bf2;
		SFC::Program program_5bf3;
	};

	std::list< Match> _matches;
};

